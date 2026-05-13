#include <iostream>
#include <unistd.h>

int main() {
    std::cout << "[TARGET] PID de la víctima: " << getpid() << "\n";
    std::cout << "[TARGET] Esperando interrupción... (Modo: Suspensión de hilos)\n";
    
    // Ciclo infinito simulando un servicio activo
    while (true) {
        sleep(2);
    }
    return 0;
}
