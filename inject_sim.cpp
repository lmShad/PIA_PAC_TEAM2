#include <iostream>
#include <sys/mman.h>
#include <cstring>
#include <unistd.h>

int main() {
    std::cout << "[*] Inject_sim - Simulador de inyección de código" << std::endl;
    std::cout << "[*] PID: " << getpid() << std::endl;

    // 1. Asignar memoria ejecutable (como haría un injector real)
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

    // 2. Copiar shellcode NOP benigno (90 = NOP, C3 = RET)
    // Equivalente Linux al ejercicio 38 con VirtualAlloc
    unsigned char shellcode[] = { 0x90, 0x90, 0x90, 0xC3 };
    memcpy(exec_mem, shellcode, sizeof(shellcode));

    std::cout << "[*] Shellcode NOP copiado a memoria" << std::endl;
    std::cout << "[*] Bytes: 90 90 90 C3 (NOP NOP NOP RET)" << std::endl;

    // 3. Ejecutar shellcode benigno
    ((void(*)())exec_mem)();
    std::cout << "[*] Shellcode ejecutado correctamente" << std::endl;

    // 4. Mantener proceso vivo para que el monitor lo detecte
    std::cout << "\n[*] Proceso activo - presiona Enter para terminar..." << std::endl;
    std::cout << "[*] Abre otra terminal y corre ./simulador para detectarlo" << std::endl;
    std::cin.get();

    munmap(exec_mem, size);
    std::cout << "[*] Memoria liberada. Saliendo..." << std::endl;
    return 0;
}
