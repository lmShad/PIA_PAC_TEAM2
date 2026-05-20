# Informe de Análisis: Inyección de Procesos y Monitorización Defensiva

## 1. Descripción del Payload
El payload desarrollado consiste en un bucle infinito a nivel de ensamblador, representado por la instrucción `EB FE` (Jump to self). El objetivo táctico de este payload no es la exfiltración de datos, sino la demostración de la capacidad de secuestro de flujo de ejecución (hijacking). Al insertar este op-code en el segmento de texto de un proceso víctima mediante la API `ptrace`, se fuerza al proceso a detener su ejecución legítima y entrar en un estado de bloqueo, lo cual es fácilmente identificable mediante herramientas de monitoreo de integridad de memoria.

## 2. Diseño
El sistema se compone de dos módulos independientes:

* **Inyector (`inject_raw`):** Utiliza la llamada al sistema `ptrace` para manipular procesos externos. Su ciclo operativo es: `PTRACE_ATTACH` (anclaje), `PTRACE_GETREGS` (lectura de estado), `PTRACE_POKETEXT` (inyección de payload), `PTRACE_SETREGS` (redirección del Instruction Pointer - RIP) y `PTRACE_DETACH` (liberación).
* **Monitor Defensivo:** Implementado como un proceso de monitoreo que audita el sistema de archivos `/proc`. Su lógica se basa en comparar el estado actual del segmento de memoria de los procesos con un baseline de integridad, detectando anomalías (como el loop infinito) y asignando un score de riesgo basado en el comportamiento detectado.

## 3. Pruebas Realizadas
Se ejecutó un entorno de pruebas controlado sobre una distribución Linux x64:
1.  **Análisis Estático:** Utilizando **Ghidra**, se realizó la descompilación del binario para validar la lógica de llamadas a `ptrace` (identificación de constantes `0x10`, `0xc`, `0x4` correspondientes a las syscalls).
2.  **Validación Dinámica:** Mediante `strace`, se capturó la traza de ejecución del inyector. Se validó empíricamente que todas las llamadas al sistema retornaron estado de éxito (`= 0`), confirmando la inyección exitosa.
3.  **Detección:** Se ejecutó el monitor en paralelo, verificando la capacidad del sistema para correlacionar la anomalía en la RAM del proceso víctima con el log de eventos generados.

## 4. Hallazgos Iniciales
* **Vulnerabilidad de Diseño:** El binario inyector requiere privilegios de superusuario (`sudo`) o capacidades específicas (`CAP_SYS_PTRACE`), lo que lo limita en entornos de usuario estándar.
* **Mitigaciones del Kernel:** Se confirmó que la configuración `kernel.yama.ptrace_scope` es la defensa principal ante este tipo de ataques. La imposibilidad de realizar el *attach* sin privilegios es la medida mitigante más efectiva identificada.
* **Detección Fileless:** El monitor demostró ser eficiente detectando la inyección en memoria, confirmando que la integridad de la RAM es un vector crítico de auditoría frente a ataques de ejecución de código arbitrario.

## 5. Riesgos y Mitigaciones
* **Riesgo:** Escalada de privilegios o ejecución de código arbitrario en procesos críticos.
* **Mitigación:** * **Hardening de Kernel:** Implementación estricta de políticas de YAMA (`ptrace_scope = 1` o superior).
    * **Monitorización Activa:** El uso de herramientas de EDR (Endpoint Detection and Response) para auditar llamadas al sistema sospechosas.
    * **Restricciones de Capacidades:** Aplicar el principio de menor privilegio, limitando los binarios que pueden ejecutar `ptrace` mediante grupos de usuarios.

## 6. Trabajo Pendiente
* **Escalabilidad:** Migrar la lógica de detección de `/proc` a **eBPF** (Extended Berkeley Packet Filter) para obtener una monitorización a nivel de Kernel más eficiente y menos intrusiva.
* **Evasión/Ofuscación:** Evaluar técnicas de inyección que evadan la detección de baselines de memoria (ej. *Reflective DLL Injection* o técnicas de *Process Hollowing* avanzadas).
* **Soporte Multi-Arquitectura:** Extender el inyector para soportar arquitecturas diferentes a x64 (ej. ARM64).
