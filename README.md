# ArquiII_Taller2: Conversor SIMD de Mayúsculas/Minúsculas

Este repositorio contiene la implementación de un conversor de mayúsculas a minúsculas (y viceversa) utilizando implementaciones seriales y vectorizadas (SIMD con Intrinsics AVX2) en C++, como parte del Taller 02 del curso CE-4302 Arquitectura de Computadores II.

## Estructura del Proyecto

* **`src/`**: Contiene todo el código fuente (`.cpp` y `.h`).
* **`Makefile`**: Script para automatizar la compilación, ejecución y generación de resultados.
* **`run_benchmark.sh`**: Script de shell para automatizar las mediciones exhaustivas del punto 5.e.
* **`plot_graphics.py`**: Script de Python para generar las gráficas de desempeño a partir del CSV.
* **`obj/`**: Directorio generado para los archivos objeto (`.o`). Ignorado por Git.
* **`case_converter`**: Ejecutable principal generado por el `Makefile`. Ignorado por Git.

## Requisitos del Sistema

Para compilar y ejecutar el proyecto, necesitas:

1.  Sistema Operativo basado en **Linux** (requisito del taller).
2.  Compilador **GCC/G++** o **Clang** con soporte para C++17.
3.  Soporte de hardware y compilador para extensiones **AVX2** (`-march=native`).
4.  Librerías de Python para graficación: `pandas` y `matplotlib`.

## Uso y Comandos

Todos los comandos se ejecutan desde el directorio raíz del proyecto usando `make`.

### 1. Compilación

Para compilar todos los archivos fuente (`.cpp`) y generar el ejecutable `case_converter`:

```bash
make all
```

### 2. Ejecución de Prueba Simple

Para ejecutar el programa con un tamaño de cadena fijo (4000 bytes) y validar la correctitud de las versiones Serial y SIMD

```bash
make run
```

### 3. Generación del Benchmark

Para ejecutar el script run_benchmark.sh para realizar las mediciones exhaustivas, variando el tamaño de la cadena (50 valores logarítmicos), el alineamiento (2 valores) y el porcentaje alfabético (11 valores).

```bash
make benchmark
```

### 4. Generación de Gráficas

Para asegurar que las mediciones se hayan completado y luego ejecuta el script de Python (plot_graphics.py) para generar los gráficos requeridos

```bash
make graphics
```


### 5. Limpieza

Para eliminar el ejecutable, los archivos objeto y los resultados generados:

```bash
make clean
```

