# Evidencias del Proyecto: Inyección y Detección de Procesos

Esta sección documenta de manera gráfica las pruebas realizadas para validar tanto el funcionamiento del vector de ataque como la capacidad de detección del monitor EDR implementado.

---

## 1. Ciclo de Ejecución (Ataque)

Aquí validamos que el proceso inyector es capaz de secuestrar al proceso víctima (`target_raw`) mediante la manipulación de hilos.

* **[Imagen 1: Ejecución del Ataque](Caps/Ataque.png)**
*Contexto:* Se observa la terminal del `target` esperando la interrupción y el `injector` confirmando el secuestro del PID 5022.

* **[Imagen 2: Detección del Monitor](Caps/Monitor.png)**
*Contexto:* Captura del monitor defensivo donde se evidencia la alerta crítica (Score 75/100) tras detectar el bucle `EB FE` (Loop Infinito) en la memoria RAM del proceso infectado.

---
## 2. Análisis Estático 

Para verificar la estructura del binario, realizamos un análisis en Ghidra.

* **[Imagen 3: Análisis en Ghidra](Caps/InicioG)**

*Contexto:* Mediante la vista del descompilador, se rastreó la lógica de ejecución del binario identificando las cadenas de texto  correspondientes  que confirman la fase de preparación del proceso objetivo . Esto confirma que el payload no está ofuscado y su flujo de ejecución es completamente auditable.

## 3. Análisis de Comportamiento (Syscalls)

Para entender qué está pasando realmente en el Kernel, utilizamos `strace` para trazar las llamadas al sistema.

* **[Imagen 4: Log de Syscalls](../images/strace_log.png)**

*Contexto:* Evidencia irrefutable de la inyección. Se aprecia la secuencia de `PTRACE_ATTACH` seguida de `PTRACE_POKETEXT`.

---

### Nota técnica

Como se puede observar en las capturas, el monitor fue capaz de correlacionar el nombre del proceso sospechoso con la actividad anómala en memoria, permitiendo una detección eficaz incluso antes de que el proceso dañara otros servicios del sistema.
