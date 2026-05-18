#include "scorer.h"
#include <sstream>

static const char* SUSPICIOUS_NAMES[] = {
    "stager", "payload", "dropper", "beacon", "implant",
    "meterpreter", "reverse", "shell", "inject", "loader",
    "rat", "trojan", "backdoor", "keylogger", "rootkit"
};

std::string Scorer::LevelFromScore(int score) {
    if (score >= 70) return "ALTO";
    if (score >= 40) return "MEDIO";
    return "BAJO";
}

ScoreResult Scorer::Evaluate(const std::string& processName,
                             const ElfReport& elfReport,
                             bool nameMatch) {
    ScoreResult result;
    result.score = 0;
    std::ostringstream reasons;

    if (nameMatch) {
        result.score += 30;
        reasons << "nombre sospechoso (+30); ";
    }

    for (const auto& name : SUSPICIOUS_NAMES) {
        if (processName.find(name) != std::string::npos) {
            result.score += 15;
            reasons << "coincide con patron '" << name << "' (+15); ";
            break;
        }
    }

    if (elfReport.isElf) {
        if (elfReport.hasNetworkSymbols) {
            result.score += 25;
            reasons << "simbolos de red detectados (+25); ";
        }

        if (elfReport.hasMemExecSymbols) {
            result.score += 20;
            reasons << "simbolos de ejecucion/memoria (+20); ";
        }

        if (!elfReport.suspiciousSections.empty()) {
            result.score += 10;
            reasons << "secciones notables en ELF (+10); ";
        }
    }

    if (result.score > 100) result.score = 100;

    result.level = LevelFromScore(result.score);
    result.summary = reasons.str();

    return result;
}
