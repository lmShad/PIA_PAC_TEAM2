# PIA PAC TEAM 2 - Guía de Ejecución y Pruebas del Simulador EDR

Este repositorio contiene las pruebas de concepto (PoC) y la simulación de técnicas de evasión, inyección de código y persistencia en un entorno Linux aislado, evaluadas mediante nuestro **Monitor Defensivo (EDR Híbrido v2.0)**.

---

## Guía de Ejecución de Componentes

A continuación se detallan los pasos y las salidas de consola de cada uno de los binarios que forman parte de la simulación. Todos los binarios se compilan y ejecutan desde la ruta raíz del proyecto:

```bash
~/Escritorio/PIA_PAC_TEAM2-main/PIA_PAC_TEAM2

1. Stager (stager_sys)
Este componente simula la apertura de un canal de comunicación benigno simulando un stager malicioso. Abre un puerto local para interactuar de manera aislada.

Comando: ./build/stager_sys

Características:

Abre un socket TCP en 127.0.0.1:8080 (Localhost).

No genera tráfico hacia el exterior.

Capturable localmente mediante interfaces de red como Wireshark (lo).

Salida de consola esperada:

==========================================
PIA PAC TEAM 2 - Stager Simulador
Entorno: VM Aislada (Linux)
==========================================

[*] Este programa simula un stager benigno.
[*] Abre un socket TCP en 127.0.0.1:8080
[*] Solo escucha en localhost, no se conecta a ningun servidor externo.
[*] Capturable con Wireshark en la interfaz loopback (lo).

[+] Socket abierto en 127.0.0.1:8080
[+] Esperando conexiones (Ctrl+C para salir)...
[*] El monitor defensivo deberia detectar este proceso como 'stager'.
^C
[*] Cerrando socket...
[+] Stager finalizado.

2. Víctima (target_sys)
Este binario actúa como el proceso legítimo del sistema operativo que recibirá el impacto o la manipulación de flujos.

Comando: ./build/target_sys

Salida de consola esperada:

[TARGET] PID de la víctima: 7375
[TARGET] Esperando interrupción... (Modo: Suspensión de hilos)
