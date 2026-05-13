#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <thread>
#include <chrono>
 
int main() {
    std::cout << "[*] Stager iniciado - Simulador de C2 benigno" << std::endl;
    std::cout << "[*] PID: " << getpid() << std::endl;
 
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        std::cerr << "[!] Error al crear socket" << std::endl;
        return 1;
    }
 
    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8080);
    inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr);
 
    std::cout << "[*] Intentando conectar a 127.0.0.1:8080..." << std::endl;
    int conn = connect(sock, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
 
    if (conn < 0) {
        std::cout << "[!] No hay listener en 8080 (comportamiento normal)" << std::endl;
        std::cout << "[*] Simulando heartbeat C2 de todas formas..." << std::endl;
    } else {
        std::cout << "[+] Conectado al servidor C2 simulado" << std::endl;
        const char* msg = "BEACON from stager";
        send(sock, msg, strlen(msg), 0);
        close(sock);
    }
 
    std::cout << "[*] Stager activo. Presiona Enter para terminar..." << std::endl;
    std::cin.get();
    return 0;
}
