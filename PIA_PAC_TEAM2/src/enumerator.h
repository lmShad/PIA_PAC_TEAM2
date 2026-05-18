#ifndef ENUMERATOR_H
#define ENUMERATOR_H

#include <string>
#include <vector>

struct ProcessInfo {
    int pid;
    std::string name;
    std::string exePath;
};

class ProcessEnumerator {
public:
    static std::vector<ProcessInfo> EnumerateAll();
    static std::vector<ProcessInfo> FindSuspicious(const std::vector<ProcessInfo>& procs);
    static void PrintActiveProcesses();
};

#endif
