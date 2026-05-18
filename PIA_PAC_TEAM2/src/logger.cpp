#include "logger.h"
#include <iostream>
#include <fstream>
#include <ctime>
#include <sys/stat.h>

static std::string g_logPath = "logs/events.log";

static std::string GetTimestamp() {
    time_t now = time(nullptr);
    struct tm* t = localtime(&now);
    char buf[64];
    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", t);
    return std::string(buf);
}

void Logger::Init(const std::string& logDir) {
    mkdir(logDir.c_str(), 0755);
    g_logPath = logDir + "/events.log";

    std::ofstream ofs(g_logPath, std::ios::app);
    if (ofs.is_open()) {
        ofs << "\n[" << GetTimestamp() << "] [SESSION] === Monitor iniciado ===" << std::endl;
    }
}

void Logger::Log(const std::string& level, const std::string& message) {
    std::string line = "[" + GetTimestamp() + "] [" + level + "] " + message;
    std::cout << line << std::endl;

    std::ofstream ofs(g_logPath, std::ios::app);
    if (ofs.is_open()) {
        ofs << line << std::endl;
    }
}

void Logger::Info(const std::string& message) {
    Log("INFO", message);
}

void Logger::Warning(const std::string& message) {
    Log("WARNING", message);
}

void Logger::Alert(const std::string& message) {
    Log("ALERT", message);
}
