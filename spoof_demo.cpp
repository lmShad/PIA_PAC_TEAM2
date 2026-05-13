#include <iostream>
#include <unistd.h>
#include <sys/prctl.h>
#include <cstring>

int main() {
    std::cout << "[*] Spoof_demo - Simulador de evasión de procesos" << std::endl;
    std::cout << "[*] PID real: " << getpid() << std::endl;
    std::cout << "[*] Nombre real (antes): ";
    fflush(stdout);
    system("cat /proc/self/comm");
    
    // Cambiar el nombre del proceso (técnica de spoofing)
    const char* fake_name = "[kworker/0:0]";
    prctl(PR_SET_NAME, fake_name);
    
    std::cout << "[*] Nombre falseado a: " << fake_name << std::endl;
    std::cout << "[*] Verificando /proc/self/comm: ";
    fflush(stdout);
    system("cat /proc/self/comm");
    
    std::cout << std::endl << "[*] Técnica de spoofing demostrada" << std::endl;
    std::cout << "[*] El monitor defensivo debería detectar esta evasión" << std::endl;
    std::cout << "[*] Presiona Enter para salir..." << std::endl;
    std::cin.get();
    return 0;
}