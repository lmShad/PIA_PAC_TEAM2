#include "enumerator.h"
#include <iostream>
#include <dirent.h>
#include <fstream>
#include <string>
#include <cctype>
#include <unistd.h>
#include <cstring>

static bool isNumeric(const std::string& str) {
    for (char const &c : str) {
        if (std::isdigit(c) == 0) return false;
    }
    return true;
}

static const char* SUSPICIOUS_NAMES[] = {
    "stager", "payload", "dropper", "beacon", "implant",
    "meterpreter", "reverse", "shell", "inject", "loader",
    "rat", "trojan", "backdoor", "keylogger", "rootkit", 
    nullptr
};

std::vector<ProcessInfo> ProcessEnumerator::EnumerateAll() {
    std::vector<ProcessInfo> result;

    DIR* dir = opendir("/proc");
    if (dir == nullptr) {
        std::cerr << "[!] Error al abrir /proc" << std::endl;
        return result;
    }

    struct dirent* entry;
    while ((entry = readdir(dir)) != nullptr) {
        std::string pidStr = entry->d_name;
        if (!isNumeric(pidStr)) continue;

        ProcessInfo info;
        info.pid = std::stoi(pidStr);

        std::string commPath = "/proc/" + pidStr + "/comm";
        std::ifstream commFile(commPath);
        if (!std::getline(commFile, info.name)) continue;

        std::string exeLink = "/proc/" + pidStr + "/exe";
        char buf[1024];
        ssize_t len = readlink(exeLink.c_str(), buf, sizeof(buf) - 1);
        if (len > 0) {
            buf[len] = '\0';
            info.exePath = std::string(buf);
        }

        result.push_back(info);
    }

    closedir(dir);
    return result;
}

std::vector<ProcessInfo> ProcessEnumerator::FindSuspicious(const std::vector<ProcessInfo>& procs) {
    std::vector<ProcessInfo> suspicious;

    for (const auto& proc : procs) {
        for (int i = 0; SUSPICIOUS_NAMES[i] != nullptr; i++) {
            if (proc.name.find(SUSPICIOUS_NAMES[i]) != std::string::npos) {
                suspicious.push_back(proc);
                break;
            }
        }
    }

    return suspicious;
}

void ProcessEnumerator::PrintActiveProcesses() {
    std::cout << "[*] Iniciando enumeracion de procesos..." << std::endl;

    auto procs = EnumerateAll();
    std::cout << "[*] Total de procesos encontrados: " << procs.size() << std::endl;

    for (const auto& p : procs) {
        std::cout << "  [PID: " << p.pid << "] " << p.name << std::endl;
    }

    std::cout << "[+] Enumeracion finalizada." << std::endl;
}
