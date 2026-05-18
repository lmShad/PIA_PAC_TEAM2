#ifndef SCORER_H
#define SCORER_H

#include "elf_analyzer.h"
#include <string>

struct ScoreResult {
    int score;
    std::string level;
    std::string summary;
};

class Scorer {
public:
    static ScoreResult Evaluate(const std::string& processName,
                                const ElfReport& elfReport,
                                bool nameMatch);
    static std::string LevelFromScore(int score);
};

#endif
