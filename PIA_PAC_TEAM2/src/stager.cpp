#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <csignal>

static volatile bool running = true;

void handleSignal(int) {
    running = false;
}

int main() {
    std::cout << "==========================================" << std::endl;
    std::cout << " PIA PAC TEAM 2 - Stager Simulador" << std::endl;
    std::cout << " Entorno: VM Aislada (Linux)" << std::endl;
    std::cout << "==========================================" << std::endl;
    std::cout << std::endl;
    std::cout << "[*] Este programa simula un stager benigno." << std::endl;
    std::cout << "[*] Abre un socket TCP en 127.0.0.1:8080" << std::endl;
    std::cout << "[*] Solo escucha en localhost, no se conecta a ningun servidor externo." << std::endl;
    std::cout << "[*] Capturable con Wireshark en la interfaz loopback (lo)." << std::endl;
    std::cout << std::endl;

    signal(SIGINT, handleSignal);

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        std::cerr << "[!] Error al crear socket: " << strerror(errno) << std::endl;
        return 1;
    }

    int opt = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(8080);
    inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);

    if (bind(sockfd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        std::cerr << "[!] Error en bind: " << strerror(errno) << std::endl;
        close(sockfd);
        return 1;
    }

    if (listen(sockfd, 1) < 0) {
        std::cerr << "[!] Error en listen: " << strerror(errno) << std::endl;
        close(sockfd);
        return 1;
    }

    std::cout << "[+] Socket abierto en 127.0.0.1:8080" << std::endl;
    std::cout << "[+] Esperando conexiones (Ctrl+C para salir)..." << std::endl;
    std::cout << "[*] El monitor defensivo deberia detectar este proceso como 'stager'." << std::endl;
    std::cout << std::endl;

    while (running) {
        fd_set fds;
        FD_ZERO(&fds);
        FD_SET(sockfd, &fds);

        struct timeval tv;
        tv.tv_sec = 2;
        tv.tv_usec = 0;

        int ret = select(sockfd + 1, &fds, nullptr, nullptr, &tv);
        if (ret > 0 && FD_ISSET(sockfd, &fds)) {
            struct sockaddr_in clientAddr;
            socklen_t clientLen = sizeof(clientAddr);
            int clientfd = accept(sockfd, (struct sockaddr*)&clientAddr, &clientLen);
            if (clientfd >= 0) {
                std::cout << "[+] Conexion recibida desde localhost" << std::endl;
                const char* msg = "[stager] Conexion simulada - solo demostracion educativa\n";
                send(clientfd, msg, strlen(msg), 0);
                close(clientfd);
            }
        }
    }

    std::cout << std::endl;
    std::cout << "[*] Cerrando socket..." << std::endl;
    close(sockfd);
    std::cout << "[+] Stager finalizado." << std::endl;

    return 0;
}
