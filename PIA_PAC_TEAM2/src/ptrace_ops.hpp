#ifndef PTRACE_OPS_HPP
#define PTRACE_OPS_HPP

#include <sys/types.h>

// Declaración de la función de inyección de carga útil
void inject_data(pid_t pid, unsigned long dest_addr, const char *source, int size);

#endif