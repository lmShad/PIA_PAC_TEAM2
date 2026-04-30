#include "enumerator.h"
#include <iostream>
#include <dirent.h>
#include <fstream>
#include <string>
#include <cctype>

bool isNumeric(const std::string& str) {
    for (char const &c : str) {
        if (std::isdigit(c) == 0) return false;
    }
    return true;
}

void ProcessEnumerator::PrintActiveProcesses() {
    std::cout << "[*] Iniciando simulador en Linux..." << std::endl;
    std::cout << "[*] Procesos:" << std::endl;

    DIR* dir = opendir("/proc");
    if (dir == nullptr) {
        std::cerr << "[!] Error al abrir el directorio /proc." << std::endl;
        return;
    }

    struct dirent* entry;
    int count = 0;

    while ((entry = readdir(dir)) != nullptr) {
        std::string name = entry->d_name;

        if (isNumeric(name)) {
            std::string commPath = "/proc/" + name + "/comm";
            std::ifstream commFile(commPath);
            std::string processName;

            if (std::getline(commFile, processName)) {
                std::cout << " [PID: " << name << "] " << processName << std::endl;
                count++;
            }

        }
    }

    closedir(dir);
    std::cout << "[+] Enumeracion finalizada." << std::endl;
}
