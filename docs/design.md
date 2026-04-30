# Diseño Técnico: Simulador Benigno PAC

## Arquitectura General
El proyecto es un binario modular en C++ diseñado para sistemas Linux. Utiliza una arquitectura de "descubrimiento pasivo" interactuando con el kernel a través del sistema de archivos `/proc`.

## Módulos y Funciones
1. **Core (`main.cpp`)**: Gestiona el flujo principal y la interfaz de usuario.
2. **Enumerator (`enumerator.cpp/h`)**: Implementa la lógica de bajo nivel para iterar descriptores de procesos y extraer metadatos de `/proc/[pid]/comm`.

## Flujo del Programa
1. Inicialización de banderas de entorno.
2. Apertura del socket/directorio de sistema.
3. Bucle de lectura de PIDs activos.
4. Formateo y salida de telemetría a la consola.

## Dependencias
* **POSIX APIs**: Para el manejo de directorios (`dirent.h`).
* **ISO C++11**: Estándar base para la compilación.
* **G++**: Compilador objetivo.
