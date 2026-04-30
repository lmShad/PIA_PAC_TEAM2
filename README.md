# Simulador Benigno de Comportamiento (Proyecto Educativo de Ciberseguridad)

**Objetivo del proyecto:** Desarrollar un simulador de comportamiento a nivel de sistema que emule tácticas de software malicioso de forma completamente benigna, controlada y sin persistencia, diseñado para entrenamiento en análisis estático y dinámico.

**Descripción técnica:** El componente está desarrollado en C++ para entornos Linux. Su función principal actual es simular técnicas de bajo nivel como la enumeración de procesos leyendo el pseudo-sistema de archivos `/proc`. Está diseñado para generar firmas y ser diseccionado con herramientas como Radare2 o Ghidra.

**Alcance y límites:**
* **Implementará:** Interacción con el sistema de archivos POSIX, enumeración de procesos mediante `/proc`, lectura de archivos locales y futuros módulos de sockets.
* **NO implementará:** Mecanismos de persistencia, exfiltración de datos hacia el exterior, ni destrucción/cifrado real de archivos.

**Cómo compilar:**
`mkdir -p build && g++ -o build/simulador src/main.cpp src/enumerator.cpp`

**Cómo ejecutar:**
`./build/simulador`

**Integrantes y responsabilidades técnicas:**
* **Jonathan :** Desarrollo core, manejo de memoria e interacción a bajo nivel con el sistema de archivos de Linux.
* **José:** Módulo de red (sockets base) y futuras simulaciones de ataques (sniffing/spoofing).
* **Carlos:** Análisis estático/dinámico (Ingeniería inversa del binario) y generación de reglas defensivas.
* **Brandon:** Control benigno (cero persistencia), técnicas de evasión y documentación técnica general.
