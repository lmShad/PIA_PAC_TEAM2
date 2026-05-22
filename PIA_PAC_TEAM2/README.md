# Monitor y Simulador Benigno de Comportamiento

**Producto Integrador de Aprendizaje (PIA) | Equipo 2**

---

## Integrantes

- Brandon Yahir Flores Garcia
- Jonathan Emir Jacobo Martinez
- Jose Miguel Castellanos Martinez
- Juan Carlos Fernandez Flores

---

## Objetivo del Proyecto

Desarrollar un sistema modular en C++ para Linux compuesto de dos componentes integrados:

1. **Monitor defensivo** — Detecta comportamiento sospechoso en tiempo real mediante análisis de procesos, ELF estático y scoring de riesgo.
2. **Simuladores ofensivos benignos** — Generan comportamiento malicioso controlado para demostrar el funcionamiento del monitor.

Despues de tener los procesos sospechosos de la parte defensiva, el componente ofensivo se analizan con herramientas de ingeniería inversa, demostrando dominio de las Fases I–IV del curso: análisis de procesos, ingeniería inversa estática, análisis dinámico y auditoría de seguridad.
El

---

## Componentes Principales

### Monitor Defensivo (`simulador`)
Programa que corre en una terminal y detecta en tiempo real:
- **Enumeración de procesos** — Lee y obtiene procesos activos.
- **Análisis ELF estático** — Inspecciona headers, secciones y símbolos importados del binario de cada proceso.
- **Hashing SHA256** — Calcula hash de ejecutables con OpenSSL para identificación por IOCs.
- **Scoring de riesgo** — Correlaciona indicadores y genera un puntaje 0–100 con nivel BAJO / MEDIO / ALTO.
- **Logger** — Registra todos los eventos en logs/events.log con timestamp.

### Simuladores Ofensivos
Programas independientes que generan comportamiento detectado por el monitor:
- **stager** — Abre socket TCP a 127.0.0.1:8080, simula comportamiento de comando y control.
- **inject** — Inyecta código arbitrario en procesos víctima usando ptrace, demuestra process hijacking.
- **spoof** — Falsea metadatos de proceso (/proc/self/comm), demuestra técnicas de evasión.
- **target** — Proceso víctima para ser inyectado o analizado.

---

## Dependencias

**Sistema operativo:** Linux x64 (Ubuntu 20.04+, Debian 11+)

**Herramientas de compilación:**
- g++ (GCC 9+)
- make
- libssl-dev (OpenSSL development headers)

**Herramientas de análisis (opcionales):**
- Ghidra


**Instalación en Debian/Ubuntu:**

```bash
sudo apt update
sudo apt install build-essential g++ make libssl-dev
```

---

## Cómo Compilar

**Comando exacto:**

```bash
make clean && make all
```

Esto compila:
- Monitor defensivo: `build/simulador_raw`, `build/simulador_sys`
- Stager: `build/stager_raw`, `build/stager_sys`
- Inyector: `build/inject_raw`, `build/inject_sys`
- Spoof: `build/spoof_raw`, `build/spoof_sys`
- Target: `build/target_raw`, `build/target_sys`

**Nota:** Los binarios con sufijo `_raw` están stripeados (sin símbolos), ideal para análisis de ingeniería inversa. Los con sufijo `_sys` mantienen símbolos de debug.

---

## Cómo Ejecutar

### Flujo de demostración básico (5 terminales):



**Terminal 1 — Simulador Ofensivo (Stager):**
```bash
./build/stager_sys
```
Abre un socket TCP en `127.0.0.1:8080` y genera comportamiento sospechoso.


### Flujo avanzado (Inyección de procesos):

**Terminal 2 — Target (víctima):**
```bash
./build/target_sys
```

**Terminal 3 — Inyector (requiere sudo):**
```bash
sudo ./build/inject_sys <PID_del_target>
```


### Flujo con Spoof (falsificación de metadatos):
**Terminal 4 —:**

```bash
./build/spoof_sys
```

**Terminal 5  — Monitor Defensivo:**
```bash
./build/simulador_sys
```
Ejecuta el monitor de procesos para detectar procesos sospechosos

---


---

## Estructura del Proyecto

```
PIA_PAC_TEAM2/
├── Makefile                    # Configuración de compilación
├── README.md                   # Este archivo
├── src/
│   ├── main.cpp               # Orquestador del monitor
│   ├── enumerator.cpp/.h      # Enumeración de procesos
│   ├── elf_analyzer.cpp/.h    # Análisis estático ELF
│   ├── hash_engine.cpp/.h     # SHA256 con OpenSSL
│   ├── scorer.cpp/.h          # Scoring de riesgo
│   ├── logger.cpp/.h          # Logging con timestamp
│   ├── ptrace_ops.cpp/.hpp    # Operaciones ptrace
│   ├── injector_main.cpp      # Orquestador del inyector
│   ├── stager.cpp             # Simulador stager
│   ├── spoof.cpp              # Simulador spoof
│   └── target.cpp             # Proceso víctima
├── build/                      # Binarios compilados
│   ├── simulador_raw/.sys
│   ├── stager_raw/.sys
│   ├── inject_raw/.sys
│   ├── spoof_raw/.sys
│   └── target_raw/.sys
├── logs/                       # Logs de ejecución (events.log)
├── docs/
│   ├── design.md              # Arquitectura técnica
│   ├── test.md                # Pruebas realizadas
│   └── reporte_draft.md       # Hallazgos iniciales
└── evidence/                   # Capturas de pantalla y evidencias
```

---

## Alcance y Límites

| Sí implementará | No implementará |
|---|---|
| Enumeración de procesos vía /proc/ | Persistencia en el sistema |
| Análisis estático de binarios ELF | Exfiltración de datos al exterior |
| Hashing SHA256 de ejecutables | Cifrado o destrucción de archivos |
| Detección de comportamiento sospechoso | Conexión a servidores externos |
| Simuladores ofensivos benignos | Modificación de procesos ajenos (sin consentimiento) |
| Scoring de riesgo correlacionado | Escalación de privilegios real |
| Logging de eventos con timestamp | Evasión de antivirus reales |
| Análisis con herramientas estándar | Técnicas de kernel exploitation |

---

## Recursos

- **Reporte Final:** [Enlace al reporte completo](#)
- **Video de Demostración:** [Enlace al video de demostración](#)

---

## Notas Importantes


- Se ejecuta en Linux x64; no soporta otras arquitecturas en esta versión.

---

