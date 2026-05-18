#include <iostream>
#include <string>
#include "enumerator.h"
#include "elf_analyzer.h"
#include "hash_engine.h"
#include "scorer.h"
#include "logger.h"

int main(int argc, char* argv[]) {
    std::cout << "==========================================" << std::endl;
    std::cout << " PIA PAC TEAM 2 - Monitor Defensivo v2.0" << std::endl;
    std::cout << " EDR Híbrido: Estático + Dinámico (RAM)" << std::endl;
    std::cout << "==========================================" << std::endl;
    std::cout << std::endl;

    Logger::Init("logs");

    // =========================================================================
    // MODO 1: RESPUESTA A INCIDENTES MANUAL (Ejecución Quirúrgica)
    // =========================================================================
    if (argc > 1) {
        std::string arg1 = argv[1];
        Logger::Info("Modo de Respuesta a Incidentes (IR) invocado manualmente.");

        if (arg1 == "--ram") {
            if (argc != 3) {
                std::cerr << "[-] Sintaxis incorrecta. Uso: " << argv[0] << " --ram <PID>\n";
                return 1;
            }
            int target_pid = std::stoi(argv[2]);
            ElfAnalyzer::ScanRuntimeMemory(target_pid);
        } else {
            // Si el argumento no es --ram, se asume que es la ruta de un archivo
            ElfReport report = ElfAnalyzer::Analyze(arg1);
            ElfAnalyzer::PrintReport(report, arg1);
        }
        
        Logger::Info("Análisis manual finalizado.");
        return 0; // Termina la ejecución para no correr el escaneo masivo
    }

    // =========================================================================
    // MODO 2: MONITOREO AUTOMATIZADO MASIVO (El diseño original mejorado)
    // =========================================================================
    Logger::Info("Monitor defensivo automatizado iniciado");

    Logger::Info("Fase 1: Enumeracion de procesos activos via /proc/");
    auto allProcs = ProcessEnumerator::EnumerateAll();
    Logger::Info("Procesos encontrados: " + std::to_string(allProcs.size()));

    Logger::Info("Fase 2: Filtrando procesos sospechosos...");
    auto suspicious = ProcessEnumerator::FindSuspicious(allProcs);

    if (suspicious.empty()) {
        Logger::Info("No se detectaron procesos sospechosos.");
        Logger::Info("Tip: Ejecuta el inyector en otra terminal para generar actividad.");
        std::cout << std::endl;
        std::cout << "[*] Listado completo de procesos:" << std::endl;
        ProcessEnumerator::PrintActiveProcesses();
        Logger::Info("Monitor finalizado - sin alertas");
        return 0;
    }

    Logger::Alert("Se detectaron " + std::to_string(suspicious.size()) + " proceso(s) sospechoso(s)");
    std::cout << std::endl;

    for (const auto& proc : suspicious) {
        std::cout << "===========================================" << std::endl;
        std::cout << " PROCESO SOSPECHOSO DETECTADO" << std::endl;
        std::cout << "===========================================" << std::endl;
        std::cout << "  PID:    " << proc.pid << std::endl;
        std::cout << "  Nombre: " << proc.name << std::endl;
        std::cout << "  Ruta:   " << (proc.exePath.empty() ? "(no accesible)" : proc.exePath) << std::endl;

        Logger::Alert("Proceso sospechoso: " + proc.name + " [PID: " + std::to_string(proc.pid) + "]");

        // 1. Análisis de Firma (Hash)
        std::string hashValue;
        if (!proc.exePath.empty()) {
            Logger::Info("Calculando SHA256 de: " + proc.exePath);
            hashValue = HashEngine::ComputeSHA256(proc.exePath);
            if (!hashValue.empty()) {
                std::cout << "  SHA256: " << hashValue << std::endl;
                Logger::Info("SHA256: " + hashValue);
            } else {
                std::cout << "  SHA256: (no se pudo calcular)" << std::endl;
                Logger::Warning("No se pudo calcular SHA256 para " + proc.exePath);
            }
        }

        // 2. Análisis Estático (Estructura ELF en Disco)
        ElfReport elfReport = {};
        if (!proc.exePath.empty()) {
            Logger::Info("Analizando ELF: " + proc.exePath);
            elfReport = ElfAnalyzer::Analyze(proc.exePath);
            if (elfReport.isElf) {
                ElfAnalyzer::PrintReport(elfReport, proc.name);
                Logger::Info("ELF tipo: " + elfReport.type + ", secciones: " + std::to_string(elfReport.sectionCount));
                if (elfReport.hasNetworkSymbols)
                    Logger::Warning("Simbolos de red detectados en " + proc.name);
                if (elfReport.hasMemExecSymbols)
                    Logger::Warning("Simbolos de ejecucion/memoria detectados en " + proc.name);
            } else {
                std::cout << "    [ELF] No es un binario ELF valido" << std::endl;
            }
        }

        // 3. Análisis Dinámico Heurístico (Memoria RAM Viva) --- ¡NUEVA INTEGRACIÓN! ---
        Logger::Info("Ejecutando auditoria profunda en RAM para PID: " + std::to_string(proc.pid));
        bool isMemCompromised = ElfAnalyzer::ScanRuntimeMemory(proc.pid);
        if (isMemCompromised) {
            Logger::Alert("¡ALERTA CRÍTICA! Infeccion Fileless detectada en RAM - PID: " + std::to_string(proc.pid));
            // Nota: Aquí, en un entorno de producción, se invocaría kill(proc.pid, SIGKILL);
        }

        // 4. Evaluación Final de Riesgo
        ScoreResult score = Scorer::Evaluate(proc.name, elfReport, true);
        std::cout << std::endl;
        std::cout << "  >>> SCORE DE RIESGO: " << score.score << "/100 [" << score.level << "]" << std::endl;
        
        // Ponderación visual si se encontró código malicioso en RAM
        if (isMemCompromised) {
            std::cout << "  >>> PENALIZACIÓN: Se sobreescribe prioridad a CRÍTICA por hallazgos en memoria viva." << std::endl;
        }

        if (!score.summary.empty()) {
            std::cout << "  >>> Razones: " << score.summary << std::endl;
        }
        std::cout << std::endl;

        Logger::Alert("Score: " + std::to_string(score.score) + "/100 [" + score.level + "] - " + proc.name);
        if (!score.summary.empty()) {
            Logger::Info("Detalle: " + score.summary);
        }
    }

    std::cout << "===========================================" << std::endl;
    std::cout << " RESUMEN FORENSE" << std::endl;
    std::cout << "===========================================" << std::endl;
    std::cout << " Procesos analizados: " << allProcs.size() << std::endl;
    std::cout << " Alertas generadas:   " << suspicious.size() << std::endl;
    std::cout << " Log guardado en:     logs/events.log" << std::endl;
    std::cout << "===========================================" << std::endl;

    Logger::Info("Monitor finalizado - " + std::to_string(suspicious.size()) + " alerta(s) generada(s)");

    return 0;
}