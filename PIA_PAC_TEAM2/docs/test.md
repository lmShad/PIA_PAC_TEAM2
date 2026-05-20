Markdown
# Informe de Pruebas: Inyección y Detección (PIA PAC TEAM 2)

Este documento detalla el procedimiento de validación del vector de ataque y la capacidad de detección del monitor EDR implementado.

## 1. Compilación
Para generar los binarios del proyecto, se utilizó el siguiente comando de compilación (o el equivalente definido en el `Makefile` del proyecto):

```bash
# Ejemplo de compilación estandarizada
gcc src/target.c -o build/target_raw
gcc src/injector.c -o build/inject_raw
gcc src/monitor.c -o build/simulador_raw
2. Validación del Vector de Ataque
El objetivo de esta prueba es confirmar la capacidad del inyector para realizar un secuestro de flujo de ejecución (hijacking) exitoso sobre el proceso objetivo.

Ejecución
Iniciar el proceso víctima en segundo plano:

Bash
./build/target_raw
Ejecutar el inyector pasando el PID obtenido del proceso víctima (ejemplo con PID 5022):

Bash
./build/inject_raw 5022
Referencia: Imagen 1: Ejecución del Ataque

Salida del sistema (Log)
Plaintext
[TARGET] PID de la víctima: 5022
[TARGET] Esperando interrupción... (Modo: Suspensión de hilos)

[*] Iniciando secuestro del proceso: 5022
[+] Proceso interceptado exitosamente.
[+] RIP actual: 0x7f259fe9b687
[+] Liberando proceso. El flujo de ejecución ha sido comprometido.
[*] Presiona ENTER para finalizar el injector...
3. Validación del Monitor Defensivo
El objetivo es verificar que el monitor pueda identificar el comportamiento anómalo del inyector basándose en la inspección de memoria en tiempo real.

Ejecución
Bash
./build/simulador_raw
Referencia: Imagen 2: Detección del Monitor

Salida del sistema (Log)
Plaintext
PROCESO SOSPECHOSO DETECTADO
===========================================
  PID:   5036
  Nombre: inject_raw
  Ruta:   /home/carlinhos/PIA_PAC_TEAM2-main/PIA_PAC_TEAM2/build/inject_raw
[2026-05-20 02:02:36] [ALERT] Proceso sospechoso: inject_raw [PID: 5036]
[2026-05-20 02:02:36] [INFO] Calculando SHA256 de: /home/carlinhos/PIA_PAC_TEAM2-main/PIA_PAC_TEAM2/build/inject_raw
  SHA256: 74643cfb878bf095804beac6d71061577cc8c86e910a26b6f8f476a5d1387fd8
[2026-05-20 02:02:36] [INFO] Analizando ELF: /home/carlinhos/PIA_PAC_TEAM2-main/PIA_PAC_TEAM2/build/inject_raw
    [ELF] Tipo: DYN (Shared object / PIE)
    [ELF] Secciones: 30
    [ELF] Simbolos importados relevantes: ptrace

[!!!] INCIDENTE CRÍTICO: Ejecución anómala detectada en RAM.
      -> Coordenada comprometida: 0x7efd6b90c252
      -> Patrón validado: EB FE (Loop Infinito)
[!!!] INCIDENTE CRÍTICO: Ejecución anómala detectada en RAM.
      -> Coordenada comprometida: 0x7efd6ba187ef
      -> Patrón validado: EB FE (Loop Infinito)
...
[ALERT] ¡ALERTA CRÍTICA! Infeccion Fileless detectada en RAM - PID: 5036

  >>> SCORE DE RIESGO: 75/100 [ALTO]
  >>> PENALIZACIÓN: Se sobreescribe prioridad a CRÍTICA por hallazgos en memoria viva.
  >>> Razones: nombre sospechoso (+30); coincide con patron 'inject' (+15); simbolos de ejecucion/memoria (+20); secciones notables en ELF (+10); 

===========================================
 RESUMEN FORENSE
===========================================
 Procesos analizados: 302
 Alertas generadas:   7
 Log guardado en:     logs/events.log
4. Conclusión de las pruebas
Las pruebas realizadas confirman la eficacia del sistema. El inyector logró comprometer la integridad del proceso víctima, y el monitor defensivo fue capaz de detectar el patrón de inyección (EB FE) en la RAM, generando una alerta de nivel CRÍTICO con un score de riesgo de 75/100.


---

### Recomendaciones técnicas:
* **Compilación:** Si el comando para compilar es diferente (por ejemplo, si usas `make all` o un script específico), asegúrate de cambiar la sección "1. Compilación" con el comando exacto que usas en tu terminal.
* **Consistencia:** Verifica que los nombres de los archivos en tu carpeta `images/` coincidan exactamente con lo que puse en los corchetes (ej. `Ataque.png` y `Monitor.png`).


