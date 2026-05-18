#ifndef ELF_ANALYZER_H
#define ELF_ANALYZER_H

#include <string>
#include <vector>

struct ElfReport {
    bool isElf;
    std::string type;
    int sectionCount;
    std::vector<std::string> suspiciousSections;
    std::vector<std::string> importedSymbols;
    bool hasNetworkSymbols;
    bool hasMemExecSymbols;
};

class ElfAnalyzer {
public:
    // Fase Estática
    static ElfReport Analyze(const std::string& filePath);
    static void PrintReport(const ElfReport& report, const std::string& processName);
    
    // Fase Dinámica (Módulo de escaneo en memoria viva)
    static bool ScanRuntimeMemory(int target_pid);
};

#endif
