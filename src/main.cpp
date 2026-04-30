#include <iostream>
#include "enumerator.h"

int main() {
    std::cout << "==========================================" << std::endl;
    std::cout << " PIA PAC TEAM 2 - Simulador v0" << std::endl;
    std::cout << " Entorno de ejecucion: VM Aislada (Linux)" << std::endl;
    std::cout << "==========================================" << std::endl;

    ProcessEnumerator::PrintActiveProcesses();

    return 0;
}
