# Análisis Preliminar (Reversing)

El siguiente análisis documenta la fase inicial de ingeniería inversa estática realizada sobre el binario inyector, priorizando la identificación de su comportamiento operativo y las interfaces de comunicación con el Kernel de Linux.

## 5.1. Extracción de Strings Relevantes

A través del análisis de las secciones de datos del binario (ej. `.rodata`), se identificaron cadenas de texto hardcodeadas que revelan el flujo de ejecución diseñado por el autor. No se listan todas las cadenas, sino exclusivamente aquellas con valor táctico para entender las fases del ataque:

* `"[*] Iniciando secuestro del proceso: "` -> Indica el inicio de la rutina de inyección y la captura del PID objetivo.
* `"[X] Fallo ptrace_attach. Verifique permisos YAMA."` -> Confirma que el binario es consciente de las mitigaciones del Kernel (YAMA ptrace_scope) y maneja excepciones si se le deniega el acceso.
* `"[+] Proceso interceptado exitosamente."` -> Imprime confirmación tras evadir protecciones y anclarse al proceso.
* `"[+] RIP actual: "` -> Revela la intención de leer los registros del CPU, específicamente el *Instruction Pointer*, para ubicar el punto exacto de inyección en memoria.
* `"[+] Liberando proceso. El flujo de ejecución ha sido comprometido."` -> Cadena final que indica el desprendimiento (*detach*) del proceso víctima.

## 5.2. Identificación de Funciones Clave

El desensamblado del binario en Ghidra revela dependencias directas con la librería estándar de C y llamadas al sistema críticas para la manipulación de procesos:

* **`<EXTERNAL>::ptrace`**: Es la función núcleo del malware/inyector. Se utiliza repetidamente para tomar control del hilo de ejecución del proceso víctima, leer su memoria, alterarla y modificar sus registros.
* **`<EXTERNAL>::wait`** (o `waitpid`): Utilizada inmediatamente después del intento de anclaje para pausar la ejecución del inyector hasta que el sistema operativo confirme que el proceso víctima ha cambiado de estado (ej. se ha detenido).
* **`<EXTERNAL>::std::ostream::operator<<`**: Revela que el binario fue compilado en C++ (uso de `std::cout` y `std::cerr` para la salida de mensajes por consola en lugar del `printf` estándar de C).

## 5.3. Notas de Ingeniería Inversa (Flujo de Ejecución)

El análisis del *Function Graph* y la descompilación permiten establecer el siguiente flujo operativo a nivel de arquitectura x64:

1.  **Fase de Preparación y Anclaje:** Se identificó la carga del valor `0x10` (16 en decimal) en el registro `EDI`, seguido de una llamada a `ptrace`. Esto corresponde a la constante `PTRACE_ATTACH`, confirmando la intención del binario de forzar una relación tracer-tracee con un proceso externo.
2.  **Lectura de Estado:** Posteriormente, el binario pasa el valor `0xc` (12) a `ptrace`, invocando `PTRACE_GETREGS`. El objetivo documentado es extraer la estructura de registros de la arquitectura x64 para localizar el registro `RIP` (Puntero de Instrucción).
3.  **Inyección y Toma de Control:** Una vez mapeada la memoria de la víctima, el flujo lógico indica el uso de rutinas de escritura de memoria (típicamente `PTRACE_POKETEXT`, valor `0x4`) para insertar el *payload* malicioso (ej. el bucle infinito `EB FE`), seguido de una manipulación del registro `RIP` (`PTRACE_SETREGS`, valor `0xd`) para forzar la ejecución del código inyectado.
4.  **Evasión/Liberación:** El ciclo concluye liberando al proceso mediante `PTRACE_DETACH`, dejando a la víctima ejecutando el código foráneo de manera asíncrona (*fileless*).
