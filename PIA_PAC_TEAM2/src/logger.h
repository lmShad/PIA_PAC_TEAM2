#ifndef LOGGER_H
#define LOGGER_H

#include <string>

class Logger {
public:
    static void Init(const std::string& logDir = "logs");
    static void Log(const std::string& level, const std::string& message);
    static void Info(const std::string& message);
    static void Warning(const std::string& message);
    static void Alert(const std::string& message);
};

#endif
