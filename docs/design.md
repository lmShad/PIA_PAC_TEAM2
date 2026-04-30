# Diseño técnico 


---

## Arquitectura general

Se compone de dos componentes que trabajan juntos: un **monitor defensivo**
que detecta comportamiento sospechoso, y **simuladores ofensivos** que generan
ese comportamiento de forma benigna. El monitor detecta a los simuladores en
tiempo real, demostrando ataque y defensa simultáneamente.

```
┌─────────────────────────────┐     ┌──────────────────────────────┐
│   Monitor defensivo         │     │   Simuladores ofensivos      │
│   (build/simulador)         │     │   (programas separados)      │
│                             │     │                              │
│  main.cpp                   │     │  stager.cpp                  │
│   ├── enumerator  ──────────┼─────┤── detecta proceso stager     │
│   ├── elf_analyzer          │     │                              │
│   ├── hash_engine           │     │  inject_sim.cpp              │
│   ├── scorer                │     │── demo mmap/memcpy benigna   │
│   └── logger → events.log  │     │                              │
└─────────────────────────────┘     │  spoof_demo.cpp              │
                                    │── falsea /proc/self/comm     │
                                    └──────────────────────────────┘
        Tambien se usa
        Wireshark captura 
        tráfico del stager
        GDB / Radare2 / Ghidra analizan ambos binarios
```

---

## Módulos y su función

### Monitor defensivo

| Módulo | Archivo | Función | Fase |
|--------|---------|---------|------|
| Core | `main.cpp` | Orquesta el flujo del monitor | I |
| Enumerator | `enumerator.cpp` | Itera `/proc/[pid]/comm` y `/proc/[pid]/exe` | I |
| ELF Analyzer | `elf_analyzer.cpp` | Lee `Elf64_Ehdr`, secciones, símbolos | II |
| Hash Engine | `hash_engine.cpp` | SHA256 con OpenSSL para IOCs | I |
| Scorer | `scorer.cpp` | Puntaje acumulado 0–100 por indicadores | III |
| Logger | `logger.cpp` | Escribe `logs/events.log` con timestamp | I |

### Simuladores ofensivos

| Módulo | Archivo | Función | Fase |
|--------|---------|---------|------|
| Stager | `stager.cpp` | Socket TCP a `127.0.0.1:8080`, capturable con Wireshark | II |
| Inject sim | `inject_sim.cpp` | Demo benigna de `mmap` + `memcpy` + ejecución | II |
| Spoof demo | `spoof_demo.cpp` | Falsea nombre en `/proc/self/comm` | IV |

---

## Flujo general del programa

```
Terminal 1: ./stager
  └─► Abre socket a 127.0.0.1:8080
  └─► Aparece como proceso en /proc/

Terminal 2: ./simulador
  └─► Enumera procesos activos (/proc/)
        └─► Detecta "stager" como proceso sospechoso
              ├─► Calcula SHA256 del binario
              ├─► Analiza ELF (headers, símbolos)
              ├─► Acumula score de riesgo
              └─► Escribe alerta en events.log

Terminal 3: wireshark
  └─► Captura tráfico del stager en loopback (lo)

GDB / Radare2 / Ghidra
  └─► Analizan build/simulador y build/stager
```

---

## Dependencias técnicas

| Dependencia | Uso |
|-------------|-----|
| `g++` (C++11) | Compilación |
| `libssl-dev` / OpenSSL | SHA256 |
| `POSIX` (`dirent.h`, `unistd.h`) | Lectura de `/proc/` |
| `sys/socket.h` | Stager localhost |
| `ISO C++11` |  Estándar base para la compilación. |
| `elf.h` | Análisis de binarios ELF |
| Ghidra / Radare2 / GDB | Ingeniería inversa (metodología) |
| Wireshark | Sniffing del tráfico localhost |

---

Todo se hara en de manera Benigna. Probado en VM aislada (Linux).*


