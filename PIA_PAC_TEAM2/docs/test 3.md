# PIA PAC TEAM 2 - Guía de Ejecución y Pruebas del Simulador EDR

Este repositorio contiene las pruebas de concepto (PoC) y la simulación de técnicas de evasión, inyección de código y persistencia en un entorno Linux aislado, evaluadas mediante nuestro Monitor Defensivo (EDR Híbrido v2.0).

## Guía de Ejecución de Componentes

A continuación se detallan los pasos y las salidas de consola de cada uno de los binarios que forman parte de la simulación. Todos los binarios se compilan y ejecutan desde la ruta raíz del proyecto:

`~/Escritorio/PIA_PAC_TEAM2-main/PIA_PAC_TEAM2`

### 1. Stager (`stager_sys`)

Este componente simula la apertura de un canal de comunicación benigno simulando un stager malicioso. Abre un puerto local para interactuar de manera aislada.

**Comando:** `./build/stager_sys`

**Características:**
- Abre un socket TCP en `127.0.0.1:8080` (Localhost).
- No genera tráfico hacia el exterior.
- Capturable localmente mediante interfaces de red como Wireshark (`lo`).

**Salida esperada:**

**==========================================**
PIA PAC TEAM 2 - Stager Simulador

Entorno: VM Aislada (Linux)
**==========================================**

[] Este programa simula un stager benigno.

[] Abre un socket TCP en 127.0.0.1:8080

[] Solo escucha en localhost, no se conecta a ningun servidor externo.

[] Capturable con Wireshark en la interfaz loopback (lo).

[+] Socket abierto en 127.0.0.1:8080

[+] Esperando conexiones (Ctrl+C para salir)...

[] El monitor defensivo deberia detectar este proceso como 'stager'.

^C

[] Cerrando socket...

[+] Stager finalizado.


### 2. Víctima (`target_sys`)

Este binario actúa como el proceso legítimo del sistema operativo que recibirá el impacto o la manipulación de flujos.

**Comando:** `./build/target_sys`

**Salida esperada:**

[TARGET] PID de la víctima: 7375
[TARGET] Esperando interrupción... (Modo: Suspensión de hilos)


### 3. Inyector (`inject_sys`)

Simula el secuestro de un proceso (Process Hijacking) interrumpiendo el flujo de ejecución del `target_sys` mediante la lectura/modificación de registros del procesador (como el puntero de instrucciones RIP).

**Comando:** `sudo ./build/inject_sys <PID>` (Requiere privilegios de administrador)

**Salida esperada:**

[sudo] contraseña para brandon:
[*] Iniciando secuestro del proceso: 7375
[+] Proceso interceptado exitosamente.
[+] RIP actual: 0x7fa84f29a687
[+] Liberando proceso. El flujo de ejecución ha sido comprometido.
[*] Presiona ENTER para finalizar el injector...


### 4. Spoofing de Proceso (`spoof_sys`)

Demuestra una técnica educativa de evasión donde un proceso manipula su estructura en `/proc/self/comm` para enmascarar su nombre real frente a herramientas básicas de monitoreo.

**Comando:** `./build/spoof_sys`

**Salida esperada:**

==========================================
PIA PAC TEAM 2 - Spoof
Entorno: VM Aislada (Linux)
==========================================

[] Este programa demuestra como un proceso puede
[] falsear su propio nombre en /proc/self/comm.
[*] Tecnica educativa de evasion (solo afecta a si mismo).

[] PID actual: 7359
[] Nombre original: spoof_sys

[1] Falseando nombre a: 'systemd-helper'
[+] prctl(PR_SET_NAME) exitoso.
[+] Nombre en /proc/self/comm ahora es: 'systemd-helper'

[2] Verificacion:
Nombre real del binario: spoof_sys
Nombre reportado en /proc: systemd-helper
PID: 7359

[] Un monitor que solo lea /proc/[pid]/comm veria 'systemd-helper'
[] Pero /proc/[pid]/exe seguiria apuntando al binario real.
[] Esta es la razon por la que el monitor defensivo lee AMBOS.
[] Proceso activo con nombre falseado (Ctrl+C para salir)...
[*] Ejecuta ./simulador en otra terminal para ver si lo detecta.
^C
[3] Restaurando nombre original...
[+] Nombre restaurado a: spoof_sys
[+] Spoof finalizado.


## Auditoría del Monitor Defensivo (`simulador_sys`)

El Monitor Defensivo v2.0 (EDR Híbrido) realiza un análisis en dos fases: análisis estático (estructuras ELF, firmas SHA256, símbolos) y análisis dinámico profundo (escaneo heurístico de la memoria RAM).

**Comando:** `./build/simulador_sys`

### Resumen del Análisis

==========================================
PIA PAC TEAM 2 - Monitor Defensivo v2.0
EDR Híbrido: Estático + Dinámico (RAM)
==========================================

[2026-05-22 02:42:49] [INFO] Monitor defensivo automatizado iniciado
[2026-05-22 02:42:49] [INFO] Fase 1: Enumeración de procesos activos via /proc/
[2026-05-22 02:42:49] [INFO] Procesos encontrados: 258
[2026-05-22 02:42:49] [INFO] Fase 2: Filtrando procesos sospechosos...
[2026-05-22 02:42:49] [ALERT] Se detectaron 6 proceso(s) sospechoso(s)


#### Hallazgos Principales Detallados

**Detección de Falsos Positivos del Sistema / Nombres Sospechosos:**
El EDR detectó procesos legítimos del kernel de Linux (`migration`, `idle_inject`) debido a que contienen patrones heurísticos de alerta preconfigurados (como `rat` o `inject`), asignándoles un riesgo de 45/100 **MEDIO**.

> *Nota: Al no correr el monitor con sudo, saltó un error esperado al intentar acceder a los descriptores de memoria RAM de estos procesos).*

**Incidente Crítico - Infección Fileless (`stager_sys`):**
El EDR detectó exitosamente el binario del stager activo bajo el PID 7366, desencadenando las siguientes acciones de respuesta:

- **Análisis ELF:** Identificación de llamadas críticas de red (`bind`, `socket`, `accept`, etc.).
- **Análisis de Memoria:** Escaneo heurístico en vivo que localizó coordenadas comprometidas con bucles infinitos (`EB FE`), confirmando un comportamiento anómalo en la memoria viva.
- **Resultado:** Calificación de 80/100 **ALTO** y penalización automática que sobrescribió la prioridad directamente a **CRÍTICA**.

[2026-05-22 02:42:49] [ALERT] Proceso sospechoso: stager_sys [PID: 7366]
[2026-05-22 02:42:49] [INFO] Calculando SHA256 de: /home/brandon/Escritorio/PIA_PAC_TEAM2-main/PIA_PAC_TEAM2/build/stager_sys
[2026-05-22 02:42:49] [INFO] SHA256: 6bd4a155e2a86e38ac37e010e0f51482cb14ca8fd2e230861a97d29cb9f7615b
[2026-05-22 02:42:49] [INFO] Analizando ELF: /home/brandon/Escritorio/PIA_PAC_TEAM2-main/PIA_PAC_TEAM2/build/stager_sys
[ELF] Tipo: DYN (Shared object / PIE) | Secciones: 37
[ELF] Simbolos importados relevantes: bind, socket, accept, send, listen
[2026-05-22 02:42:49] [WARNING] Simbolos de red detectados en stager_sys
[2026-05-22 02:42:49] [INFO] Ejecutando auditoria profunda en RAM para PID: 7366
[*] [EDR] Iniciando escaneo heurístico de RAM en PID: 7366...
[!!!] INCIDENTE CRÍTICO: Ejecución anómala detectada en RAM.
-> Coordenada comprometida: 0x7f01318f06ef -> Patrón: EB FE (Loop Infinito)
-> Coordenada comprometida: 0x7f013192d252 -> Patrón: EB FE (Loop Infinito)
-> Coordenada comprometida: 0x7f0131aa8fcf -> Patrón: EB FE (Loop Infinito)
-> Coordenada comprometida: 0x7f0131ca77ef -> Patrón: EB FE (Loop Infinito)
-> Coordenada comprometida: 0x7f0131e831ef -> Patrón: EB FE (Loop Infinito)
[2026-05-22 02:42:49] [ALERT] ¡ALERTA CRÍTICA! Infeccion Fileless detectada en RAM - PID: 7366

|| SCORE DE RIESGO: 80/100 [ALTO]
|| PENALIZACIÓN: Se sobrescribe prioridad a CRÍTICA por hallazgos en memoria viva.


## Resumen Forense Final

Al concluir la auditoría, el monitor exporta las métricas globales de telemetría y genera un archivo log persistente:

| Métrica | Resultado |
|---------|-----------|
| Procesos Analizados | 258 |
| Alertas Generadas | 6 |
| Ruta del Log | `logs/events.log` |

[2026-05-22 02:42:49] [INFO] Monitor finalizado - 6 alerta(s) generada(s)

