#include "elf_analyzer.h"
#include <iostream>
#include <fstream>
#include <cstring>
#include <elf.h>

// Librerías nativas requeridas para la auditoría de RAM
#include <fcntl.h>
#include <unistd.h>

ElfReport ElfAnalyzer::Analyze(const std::string& filePath) {
    ElfReport report = {};
    report.isElf = false;
    report.sectionCount = 0;
    report.hasNetworkSymbols = false;
    report.hasMemExecSymbols = false;

    std::ifstream file(filePath, std::ios::binary);
    if (!file.is_open()) {
        return report;
    }

    Elf64_Ehdr ehdr;
    file.read(reinterpret_cast<char*>(&ehdr), sizeof(ehdr));
    if (file.gcount() < (std::streamsize)sizeof(ehdr)) {
        return report;
    }

    if (memcmp(ehdr.e_ident, ELFMAG, SELFMAG) != 0) {
        return report;
    }

    report.isElf = true;

    switch (ehdr.e_type) {
        case ET_EXEC: report.type = "EXEC (Executable)"; break;
        case ET_DYN:  report.type = "DYN (Shared object / PIE)"; break;
        case ET_REL:  report.type = "REL (Relocatable)"; break;
        case ET_CORE: report.type = "CORE (Core dump)"; break;
        default:      report.type = "UNKNOWN"; break;
    }

    report.sectionCount = ehdr.e_shnum;

    if (ehdr.e_shoff == 0 || ehdr.e_shnum == 0) {
        return report;
    }

    file.seekg(ehdr.e_shoff);

    std::vector<Elf64_Shdr> sections(ehdr.e_shnum);
    file.read(reinterpret_cast<char*>(sections.data()),
              ehdr.e_shnum * sizeof(Elf64_Shdr));

    std::vector<char> shstrtab;
    if (ehdr.e_shstrndx < ehdr.e_shnum) {
        Elf64_Shdr& strSec = sections[ehdr.e_shstrndx];
        shstrtab.resize(strSec.sh_size);
        file.seekg(strSec.sh_offset);
        file.read(shstrtab.data(), strSec.sh_size);
    }

    for (int i = 0; i < ehdr.e_shnum; i++) {
        std::string secName;
        if (!shstrtab.empty() && sections[i].sh_name < shstrtab.size()) {
            secName = &shstrtab[sections[i].sh_name];
        }

        if (secName == ".text" || secName == ".plt" || secName == ".got.plt") {
            continue;
        }

        if (secName == ".init_array" || secName == ".fini_array" ||
            secName == ".dynamic" || secName == ".bss") {
            report.suspiciousSections.push_back(secName);
        }
    }

    for (int i = 0; i < ehdr.e_shnum; i++) {
        if (sections[i].sh_type != SHT_DYNSYM && sections[i].sh_type != SHT_SYMTAB) {
            continue;
        }

        uint32_t strTabIdx = sections[i].sh_link;
        if (strTabIdx >= ehdr.e_shnum) continue;

        std::vector<char> strTab(sections[strTabIdx].sh_size);
        file.seekg(sections[strTabIdx].sh_offset);
        file.read(strTab.data(), sections[strTabIdx].sh_size);

        int symCount = sections[i].sh_size / sizeof(Elf64_Sym);
        std::vector<Elf64_Sym> syms(symCount);
        file.seekg(sections[i].sh_offset);
        file.read(reinterpret_cast<char*>(syms.data()),
                  symCount * sizeof(Elf64_Sym));

        for (int j = 0; j < symCount; j++) {
            if (syms[j].st_name == 0 || syms[j].st_name >= strTab.size()) continue;

            std::string symName = &strTab[syms[j].st_name];

            if (symName == "socket" || symName == "connect" ||
                symName == "send" || symName == "recv" ||
                symName == "bind" || symName == "listen" ||
                symName == "accept" || symName == "getaddrinfo") {
                report.importedSymbols.push_back(symName);
                report.hasNetworkSymbols = true;
            }

            // --- [PARCHE APLICADO]: Inclusión del vector de inyección (ptrace) ---
            if (symName == "mmap" || symName == "mprotect" ||
                symName == "memcpy" || symName == "dlopen" ||
                symName == "dlsym" || symName == "execve" ||
                symName == "system" || symName == "popen" ||
                symName == "ptrace") { 
                report.importedSymbols.push_back(symName);
                report.hasMemExecSymbols = true;
            }
        }
    }

    return report;
}

void ElfAnalyzer::PrintReport(const ElfReport& report, const std::string& processName) {
    std::cout << "    [ELF] Tipo: " << report.type << std::endl;
    std::cout << "    [ELF] Secciones: " << report.sectionCount << std::endl;

    if (!report.suspiciousSections.empty()) {
        std::cout << "    [ELF] Secciones notables: ";
        for (size_t i = 0; i < report.suspiciousSections.size(); i++) {
            std::cout << report.suspiciousSections[i];
            if (i < report.suspiciousSections.size() - 1) std::cout << ", ";
        }
        std::cout << std::endl;
    }

    if (!report.importedSymbols.empty()) {
        std::cout << "    [ELF] Simbolos importados relevantes: ";
        for (size_t i = 0; i < report.importedSymbols.size(); i++) {
            std::cout << report.importedSymbols[i];
            if (i < report.importedSymbols.size() - 1) std::cout << ", ";
        }
        std::cout << std::endl;
    }
}

// ==============================================================================
// NUEVO MÓDULO: Análisis Dinámico de RAM (Aislamiento de inyecciones Fileless)
// ==============================================================================

bool ElfAnalyzer::ScanRuntimeMemory(int target_pid) {
    std::string maps_path = "/proc/" + std::to_string(target_pid) + "/maps";
    std::string mem_path = "/proc/" + std::to_string(target_pid) + "/mem";
    
    std::ifstream maps_file(maps_path);
    if (!maps_file.is_open()) {
        std::cerr << "[-] [Monitor] Error: No se pudo acceder a la RAM del PID " << target_pid << " (Requiere sudo)\n";
        return false;
    }

    int mem_fd = open(mem_path.c_str(), O_RDONLY);
    if (mem_fd < 0) {
        std::cerr << "[-] [Monitor] Error al abrir el descriptor de memoria.\n";
        return false;
    }

    std::string line;
    bool threat_detected = false;
    
    // Firma heurística: Opcodes del secuestro activo (EB FE)
    const std::vector<unsigned char> yara_signature = {0xeb, 0xfe}; 

    std::cout << "\n[*] [EDR] Iniciando escaneo heurístico de RAM en PID: " << target_pid << "...\n";

    while (std::getline(maps_file, line)) {
        // Intercepción exclusiva de segmentos en ejecución (r-xp)
        if (line.find("r-xp") != std::string::npos) {
            unsigned long start_addr, end_addr;
            sscanf(line.c_str(), "%lx-%lx", &start_addr, &end_addr);
            
            size_t region_size = end_addr - start_addr;
            std::vector<unsigned char> buffer(region_size);

            lseek(mem_fd, start_addr, SEEK_SET);
            if (read(mem_fd, buffer.data(), region_size) > 0) {
                
                // Búsqueda lineal de la firma YARA
                for (size_t i = 0; i < region_size - yara_signature.size(); i++) {
                    if (buffer[i] == yara_signature[0] && buffer[i+1] == yara_signature[1]) {
                        std::cout << "[!!!] INCIDENTE CRÍTICO: Ejecución anómala detectada en RAM.\n";
                        std::cout << "      -> Coordenada comprometida: 0x" << std::hex << (start_addr + i) << std::dec << "\n";
                        std::cout << "      -> Patrón validado: EB FE (Loop Infinito)\n";
                        threat_detected = true;
                        break; 
                    }
                }
            }
        }
    }
    
    close(mem_fd);
    if (!threat_detected) {
        std::cout << "[+] [EDR] La memoria del proceso objetivo está limpia.\n";
    }
    return threat_detected;
}