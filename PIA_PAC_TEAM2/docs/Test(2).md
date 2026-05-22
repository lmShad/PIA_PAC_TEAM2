# ═══════════════════════════════════════════════════
#     PIA PAC TEAM 2 — EJECUCIÓN DEL SIMULADOR
# ═══════════════════════════════════════════════════

> Entorno: Máquina Virtual Aislada (Linux)
> Propósito: Demostración de técnicas de ataque y defensa (EDR híbrido)

---

## [1] STAGER — Escucha local

**Archivo:** `./build/stager_sys`

brandon@brandon:~/Escritorio/PIA_PAC_TEAM2-main/PIA_PAC_TEAM2$ ./build/stager_sys

==========================================
PIA PAC TEAM 2 - Stager Simulador
Entorno: VM Aislada (Linux)
==========================================

[] Este programa simula un stager benigno.
[] Abre un socket TCP en 127.0.0.1:8080
[] Solo escucha en localhost, no se conecta a ningun servidor externo.
[] Capturable con Wireshark en la interfaz loopback (lo).

[+] Socket abierto en 127.0.0.1:8080
[+] Esperando conexiones (Ctrl+C para salir)...


---

## [2] TARGET — Proceso víctima

**Archivo:** `./build/target_sys`

brandon@brandon:~/Escritorio/PIA_PAC_TEAM2-main/PIA_PAC_TEAM2$ ./build/target_sys

[TARGET] PID de la víctima: 7375
[TARGET] Esperando interrupción... (Modo: Suspensión de hilos)


---

## [3] INJECT — Secuestro del proceso

**Archivo:** `sudo ./build/inject_sys 7375`

brandon@brandon:~/Escritorio/PIA_PAC_TEAM2-main/PIA_PAC_TEAM2$ sudo ./build/inject_sys 7375
[sudo] contraseña para brandon:

[*] Iniciando secuestro del proceso: 7375
[+] Proceso interceptado exitosamente.
[+] RIP actual: 0x7fa84f29a687
[+] Liberando proceso. El flujo de ejecución ha sido comprometido.
[*] Presiona ENTER para finalizar el injector...


---

## [4] SPOOF — Falsificación de nombre en /proc

**Archivo:** `./build/spoof_sys`

brandon@brandon:~/Escritorio/PIA_PAC_TEAM2-main/PIA_PAC_TEAM2$ ./build/spoof_sys

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

^C
[3] Restaurando nombre original...
[+] Nombre restaurado a: spoof_sys
[+] Spoof finalizado.


---

## [5] SIMULADOR — Monitor Defensivo (EDR Híbrido)

**Archivo:** `./build/simulador_sys`

brandon@brandon:~/Escritorio/PIA_PAC_TEAM2-main/PIA_PAC_TEAM2$ ./build/simulador_sys

==========================================
PIA PAC TEAM 2 - Monitor Defensivo v2.0
EDR Híbrido: Estático + Dinámico (RAM)
==========================================

[2026-05-22 02:42:49] [INFO] Monitor defensivo automatizado iniciado
[2026-05-22 02:42:49] [INFO] Fase 1: Enumeracion de procesos activos via /proc/
[2026-05-22 02:42:49] [INFO] Procesos encontrados: 258
[2026-05-22 02:42:49] [INFO] Fase 2: Filtrando procesos sospechosos...
[2026-05-22 02:42:49] [ALERT] Se detectaron 6 proceso(s) sospechoso(s)


### Procesos sospechosos detectados

---

#### PROCESO 1 — migration/0 (PID: 21)

===========================================
PROCESO SOSPECHOSO DETECTADO
===========================================
PID: 21
Nombre: migration/0
Ruta: (no accesible)

[2026-05-22 02:42:49] [ALERT] Proceso sospechoso: migration/0 [PID: 21]
[2026-05-22 02:42:49] [INFO] Ejecutando auditoria profunda en RAM para PID: 21
[-] [Monitor] Error al abrir el descriptor de memoria.
|||
||| SCORE DE RIESGO: 45/100 [MEDIO]
||| Razones: nombre sospechoso (+30); coincide con patron 'rat' (+15);


---

#### PROCESO 2 — idle_inject/0 (PID: 22)

===========================================
PROCESO SOSPECHOSO DETECTADO
===========================================
PID: 22
Nombre: idle_inject/0
Ruta: (no accesible)
|||
||| SCORE DE RIESGO: 45/100 [MEDIO]
||| Razones: nombre sospechoso (+30); coincide con patron 'inject' (+15);


---

#### PROCESO 3 — idle_inject/1 (PID: 25)

||| SCORE DE RIESGO: 45/100 [MEDIO]
||| Razones: nombre sospechoso (+30); coincide con patron 'inject' (+15);


---

#### PROCESO 4 — migration/1 (PID: 26)
||| SCORE DE RIESGO: 45/100 [MEDIO]
||| Razones: nombre sospechoso (+30); coincide con patron 'rat' (+15);


---

#### PROCESO 5 — stager_sys (PID: 7366) — CRÍTICO
===========================================
PROCESO SOSPECHOSO DETECTADO
===========================================
PID: 7366
Nombre: stager_sys
Ruta: /home/brandon/Escritorio/PIA_PAC_TEAM2-main/PIA_PAC_TEAM2/build/stager_sys

[2026-05-22 02:42:49] [ALERT] Proceso sospechoso: stager_sys [PID: 7366]
[2026-05-22 02:42:49] [INFO] Calculando SHA256...
SHA256: 6bd4a155e2a86e38ac37e010e0f51482cb14ca8fd2e230861a97d29cb9f7615b

[2026-05-22 02:42:49] [INFO] Analizando ELF...
[ELF] Tipo: DYN (Shared object / PIE)
[ELF] Secciones: 37
[ELF] Secciones notables: .init_array, .fini_array, .dynamic, .bss
[ELF] Simbolos importados relevantes: bind, socket, accept, send, listen

[2026-05-22 02:42:49] [WARNING] Simbolos de red detectados en stager_sys
[2026-05-22 02:42:49] [INFO] Ejecutando auditoria profunda en RAM para PID: 7366

[*] [EDR] Iniciando escaneo heurístico de RAM en PID: 7366...

[!!!] INCIDENTE CRÍTICO: Ejecución anómala detectada en RAM.

Coordenada comprometida: 0x7f01318f06ef

Patrón validado: EB FE (Loop Infinito)

[!!!] INCIDENTE CRÍTICO: Ejecución anómala detectada en RAM.

Coordenada comprometida: 0x7f013192d252

Patrón validado: EB FE (Loop Infinito)

[!!!] INCIDENTE CRÍTICO: Ejecución anómala detectada en RAM.

Coordenada comprometida: 0x7f0131aa8fcf

Patrón validado: EB FE (Loop Infinito)

[2026-05-22 02:42:49] [ALERT] ¡ALERTA CRÍTICA! Infeccion Fileless detectada en RAM - PID: 7366

||| SCORE DE RIESGO: 80/100 [ALTO]
||| PENALIZACIÓN: Se sobreescribe prioridad a CRÍTICA por hallazgos en memoria viva.
||| Razones: nombre sospechoso (+30); coincide con patron 'stager' (+15);
||| simbolos de red detectados (+25); secciones notables en ELF (+10);


---

#### PROCESO 6 — inject_sys (PID: 7424)
===========================================
PROCESO SOSPECHOSO DETECTADO
===========================================
PID: 7424
Nombre: inject_sys
Ruta: (no accesible)

[2026-05-22 02:42:49] [INFO] Ejecutando auditoria profunda en RAM para PID: 7424
[-] [Monitor] Error: No se pudo acceder a la RAM del PID 7424 (Requiere sudo)

||| SCORE DE RIESGO: 45/100 [MEDIO]
||| Razones: nombre sospechoso (+30); coincide con patron 'inject' (+15);


---

## [6] RESUMEN FORENSE
==========================================
RESUMEN FORENSE
==========================================
Procesos analizados: 258
Alertas generadas: 6
Log guardado en: logs/events.log
==========================================

[2026-05-22 02:42:49] [INFO] Monitor finalizado - 6 alerta(s) generada(s)


---

## CONCLUSIÓN

El monitor defensivo (EDR híbrido) demostró ser capaz de:

- Detectar nombres sospechosos en `/proc/`
- Analizar secciones ELF y símbolos de red (bind, socket, accept, send, listen)
- Escanear memoria RAM viva en busca de patrones anómalos (`EB FE` — loop infinito)
- Asignar puntajes de riesgo (45/100 a 80/100)
- Identificar técnicas de spoofing (no fueron suficientes para evadir al monitor, ya que también verifica `/proc/[pid]/exe`)


