#include <iostream>
#include <sys/mman.h>
#include <cstring>
#include <unistd.h>
#include <thread>
#include <chrono>

// Función de prueba benigna
void payload_benigno() {
    std::cout << "[*] Payload inyectado ejecutándose (benigno)" << std::endl;
    std::cout << "[*] Esto solo simula comportamiento de inyección" << std::endl;
}

int main() {
    std::cout << "[*] Inject_sim - Simulador de inyección de código" << std::endl;
    std::cout << "[*] PID: " << getpid() << std::endl;
    std::cout << "[*] Este proceso permanecerá activo para detección" << std::endl;
    
    // 1. Asignar memoria ejecutable (como haría un injector)
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
    
    // 2. Copiar código a la memoria (simulando inyección)
    void (*func_ptr)() = NULL;
    memcpy(exec_mem, (void*)payload_benigno, 4096);
    
    std::cout << "[*] Código copiado a memoria (simulando inyección)" << std::endl;
    
    // 3. Ejecutar el payload inyectado
    std::cout << "[*] Ejecutando payload inyectado..." << std::endl;
    func_ptr = (void(*)())exec_mem;
    func_ptr();
    
    std::cout << "[*] Inyección simulada completada" << std::endl;
    
    //NUEVO: Mantener el proceso vivo para que el monitor lo detecte
    std::cout << std::endl;
    std::cout << "╔══════════════════════════════════════════════════╗" << std::endl;
    std::cout << "║  PROCESO INYECTADO ACTIVO - PID: " << getpid() << "              ║" << std::endl;
    std::cout << "║  Memoria ejecutable en: " << exec_mem << " ║" << std::endl;
    std::cout << "║  Esperando detección por el monitor defensivo... ║" << std::endl;
    std::cout << "║  Presiona ENTER para terminar                     ║" << std::endl;
    std::cout << "╚══════════════════════════════════════════════════╝" << std::endl;
    
    std::cin.get();
    
    // Limpiar
    munmap(exec_mem, size);
    std::cout << "[*] Memoria liberada. Saliendo..." << std::endl;
    return 0;
}
