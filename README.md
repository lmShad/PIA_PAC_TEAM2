# Monitor y Simulador Benigno de Comportamiento
Proyecto Integrador Aplicado | Equipo 2

---

Integrantes:
-Brandon Yahir Flores Garcia

-Jonathan Emir Jacobo Martinez

-Jose Miguel Castellanos Martinez

-Juan Carlos Fernandez Flores

---

## Objetivo del proyecto

Desarrollar un sistema modular en C++ para Linux compuesto de dos partes: un monitor
defensivo que detecta comportamiento sospechoso en tiempo real, y un conjunto de
simuladores ofensivos benignos que generan ese comportamiento de forma controlada.
Ambas partes se analizan con herramientas de ingeniería inversa, demostrando dominio
de las Fases I–IV del curso.



---

## Descripción técnica del componente educativo

El proyecto se divide en dos componentes que trabajan juntos:

### Monitor defensivo (`simulador`)
Programa principal que corre en una terminal y detecta en tiempo real:
- **Enumeración de procesos** — lee /proc/[pid]/comm y /proc/[pid]/exe
  para obtener procesos activos y marcar los sospechosos.
- **Análisis ELF estático** — inspecciona headers, secciones y símbolos
  importados del binario de cada proceso detectado.
- **Hashing SHA256** — calcula hash de ejecutables con OpenSSL para
  identificación por IOC.
- **Scoring de riesgo** — correlaciona indicadores y genera un puntaje
  0–100 con nivel BAJO / MEDIO / ALTO.
- **Logger** — registra todos los eventos en logs/events.log.

### Simuladores ofensivos (programas separados)
Programas cortos que se ejecutan en otra terminal y generan comportamiento
que el monitor detecta:
- **stager** — abre socket TCP a 127.0.0.1:8080.
- **inject_sim** — demo benigna de asignación y ejecución de memoria
  (mmap, memcpy), equivalente Linux al ejercicio de VirtualAlloc.
- **spoof_demo** — falsea metadatos de proceso (nombre en /proc/self/comm)
  para demostrar técnicas de evasión de forma educativa.

### Flujo de demostración
```
Terminal 1: ./stager        → simula comportamiento sospechoso
Terminal 2: ./simulador     → detecta al stager, calcula hash, genera alerta
Terminal 3: wireshark       → captura tráfico del stager en localhost
```

---

## Alcance y límites

| Sí implementará | No implementará |
|-----------------|----------------|
| Enumeración de procesos vía /proc/ | Persistencia en el sistema |
| Análisis  estático | Exfiltración de datos al exterior |
| Hash SHA256 con OpenSSL | Cifrado o destrucción de archivos |
| Detección de comportamiento sospechoso | Conexión a servidores externos |
| Simuladores ofensivos benignos | Modificación de procesos ajenos |
| Scoring de riesgo | Escalación de privilegios real |
| Log de eventos con timestamp | Evasión de antivirus reales |

---

## Cómo compilar

Requiere: g++, libssl-dev. 

En Debian/Ubuntu:

mkdir -p build && g++ -o build/simulador src/main.cpp src/enumerator.cpp

**Cómo ejecutar:**

./build/simulador

---

## Estructura del proyecto

```
SentinelEDU/
 ├── src/
 │    ├── main.cpp           # Orquestador del monitor
 │    ├── enumerator.cpp     # Enumeración de procesos /proc/
 │    ├── elf_analyzer.cpp   # Análisis estático ELF
 │    ├── hash_engine.cpp    # SHA256 con OpenSSL
 │    ├── scorer.cpp         # Scoring de riesgo 
 │    ├── logger.cpp         # Log con timestamp
 │    ├── stager.cpp         # Simulador: socket localhost
 │    ├── inject_sim.cpp     # Simulador: inyección benigna
 │    └── spoof_demo.cpp     # Simulador: spoofing de metadatos
 ├── include/
 │    └── *.h                # Headers de cada módulo
 ├── docs/
 │    └── design.md          # Diseño técnico
 ├── evidence/
 │    └── *.png              # Capturas de VM y ejecución
 ├── logs/
 │    └── events.log         # Generado al ejecutar
 └── README.md
```

---

## Integrantes y responsabilidades técnicas

| Integrante | Módulo | Descripción |
|------------|--------|-------------|
| Jonathan | main.cpp, enumerator.cpp | Core del monitor, enumeración de procesos vía /proc/ |
| José | elf_analyzer.cpp, hash_engine.cpp | Análisis estático de binarios ELF y hashing SHA256 |
| Brandon | stager.cpp, inject_sim.cpp | Simuladores ofensivos: socket localhost e inyección benigna |
| Carlos | scorer.cpp, logger.cpp, spoof_demo.cpp | Scoring de riesgo, logging y tema de spoofing |

---

## Dependencias

| Dependencia | Uso |
|-------------|-----|
| g++ (C++11) | Compilador |
| libssl-dev / OpenSSL | Hash SHA256 |
| POSIX (dirent.h, unistd.h`) | Lectura de /proc/` |
| sys/socket.h | Stager localhost |
| Ghidra / Radare2 / GDB | Análisis externo|
| Wireshark | Sniffing del tráfico del stager |

---


