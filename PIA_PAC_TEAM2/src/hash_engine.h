#ifndef HASH_ENGINE_H
#define HASH_ENGINE_H

#include <string>

class HashEngine {
public:
    static std::string ComputeSHA256(const std::string& filePath);
};

#endif
