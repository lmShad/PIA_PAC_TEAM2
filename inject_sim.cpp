#include <iostream>
#include <sys/mman.h>
#include <cstring>
#include <unistd.h>

void payload_benigno() {
    std::cout << "[*] Payload inyectado ejecutándose" << std::endl;
    std::cout << "[*] Esto solo simula comportamiento de inyección" << std::endl;
}

int main() {
    std::cout << "[*] Inject_sim - Simulador de inyección de código" << std::endl;
    std::cout << "[*] PID: " << getpid() << std::endl;
    
    size_t size = 4096;
    void* exec_mem = mmap(NULL, size, 
                          PROT_READ | PROT_WRITE | PROT_EXEC,
                          MAP_PRIVATE | MAP_ANONYMOUS, 
                          -1, 0);
    
    if (exec_mem == MAP_FAILED) {
        std::cerr << "[!] Error en mmap" << std::endl;
        return 1;
    }
    
    std::cout << "[*] Memoria ejecutable asignada en: " << exec_mem << std::endl;
    
    void (*func_ptr)() = NULL;
    memcpy(exec_mem, (void*)payload_benigno, 4096);
    
    std::cout << "[*] Código copiado a memoria (simulando inyección)" << std::endl;
    
    std::cout << "[*] Ejecutando payload inyectado..." << std::endl;
    func_ptr = (void(*)())exec_mem;
    func_ptr();
    
    std::cout << "[*] Inyección simulada completada" << std::endl;
    std::cout << "[*] Presiona Enter para liberar memoria..." << std::endl;
    std::cin.get();
    
    munmap(exec_mem, size);
    return 0;
}
