#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <unistd.h>
#include <sys/prctl.h>
#include <csignal>

static volatile bool running = true;

void handleSignal(int) {
    running = false;
}

int main() {
    std::cout << "==========================================" << std::endl;
    std::cout << " PIA PAC TEAM 2 - Spoof " << std::endl;
    std::cout << " Entorno: VM Aislada (Linux)" << std::endl;
    std::cout << "==========================================" << std::endl;
    std::cout << std::endl;
    std::cout << "[*] Este programa demuestra como un proceso puede" << std::endl;
    std::cout << "[*] falsear su propio nombre en /proc/self/comm." << std::endl;
    std::cout << "[*] Tecnica educativa de evasion (solo afecta a si mismo)." << std::endl;
    std::cout << std::endl;

    signal(SIGINT, handleSignal);

    pid_t pid = getpid();

    std::ifstream commFile("/proc/self/comm");
    std::string originalName;
    std::getline(commFile, originalName);
    commFile.close();

    std::cout << "[*] PID actual:     " << pid << std::endl;
    std::cout << "[*] Nombre original: " << originalName << std::endl;
    std::cout << std::endl;

    const char* fakeName = "systemd-helper";
    std::cout << "[1] Falseando nombre a: '" << fakeName << "'" << std::endl;

    if (prctl(PR_SET_NAME, fakeName, 0, 0, 0) == 0) {
        std::cout << "[+] prctl(PR_SET_NAME) exitoso." << std::endl;
    } else {
        std::cerr << "[!] Error en prctl(PR_SET_NAME)." << std::endl;
        return 1;
    }

    std::ifstream checkFile("/proc/self/comm");
    std::string spoofedName;
    std::getline(checkFile, spoofedName);
    checkFile.close();

    std::cout << "[+] Nombre en /proc/self/comm ahora es: '" << spoofedName << "'" << std::endl;
    std::cout << std::endl;

    std::cout << "[2] Verificacion:" << std::endl;
    std::cout << "    Nombre real del binario:   " << originalName << std::endl;
    std::cout << "    Nombre reportado en /proc: " << spoofedName << std::endl;
    std::cout << "    PID:                       " << pid << std::endl;
    std::cout << std::endl;
    std::cout << "[*] Un monitor que solo lea /proc/[pid]/comm veria '" << spoofedName << "'" << std::endl;
    std::cout << "[*] Pero /proc/[pid]/exe seguiria apuntando al binario real." << std::endl;
    std::cout << "[*] Esta es la razon por la que el monitor defensivo lee AMBOS." << std::endl;
    std::cout << std::endl;

    std::cout << "[*] Proceso activo con nombre falseado (Ctrl+C para salir)..." << std::endl;
    std::cout << "[*] Ejecuta ./simulador en otra terminal para ver si lo detecta." << std::endl;

    while (running) {
        sleep(2);
    }

    std::cout << std::endl;
    std::cout << "[3] Restaurando nombre original..." << std::endl;
    prctl(PR_SET_NAME, originalName.c_str(), 0, 0, 0);
    std::cout << "[+] Nombre restaurado a: " << originalName << std::endl;
    std::cout << "[+] Spoof finalizado." << std::endl;

    return 0;
}
