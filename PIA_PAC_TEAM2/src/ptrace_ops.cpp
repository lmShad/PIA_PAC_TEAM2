#include "ptrace_ops.hpp"
#include <sys/ptrace.h>
#include <cstring>
#include <iostream>
#include <cstdlib>

void inject_data(pid_t pid, unsigned long dest_addr, const char *source, int size) {
    union {
        long val;
        char chars[sizeof(long)];
    } data;

    // Escritura en memoria alineada a palabras de 64 bits
    for (int i = 0; i < size; i += sizeof(long)) {
        std::memcpy(data.chars, source + i, sizeof(long));
        if (ptrace(PTRACE_POKETEXT, pid, dest_addr + i, data.val) == -1) {
            std::cerr << "[!] Error de escritura en memoria. Violación de segmento.\n";
            exit(1);
        }
    }
}