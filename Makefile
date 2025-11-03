# ====================================================================
# MAKEFILE VERSÓN FINAL CON REGLA GRAPHIC
# ====================================================================

# Directorios
SRC_DIR = src
OBJ_DIR = obj

# Nombre del script de gráficas
GRAPHIC_SCRIPT = plot_graphics.py

# Nombre del programa ejecutable
TARGET = ./case_converter

# Compilador y Flags
CXX = g++
CXXFLAGS = -std=c++17 -O3 -march=native -Wall

# Archivos fuente (EN src/)
SRCS = main.cpp case_converter_serial.cpp case_converter_SIMD.cpp generate_random_string.cpp 

# Archivos objeto (EN obj/)
OBJS = $(addprefix $(OBJ_DIR)/, $(SRCS:.cpp=.o))

# Regla por defecto: compilar
all: $(OBJ_DIR) $(TARGET)
	@echo "Compilación exitosa. Ejecutable: $(TARGET)"

# Crea el directorio obj si no existe
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# Regla de linkeo
$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $(TARGET)

# REGLA DE COMPILACIÓN GENÉRICA
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp $(SRC_DIR)/common.h
	$(CXX) $(CXXFLAGS) -c $< -o $@ -I$(SRC_DIR)

# 1. REGLA: run (Prueba Simple de Funcionamiento)
run: $(TARGET)
	@echo "--- 🚀 Ejecutando Prueba Simple de Funcionamiento y Validación ---"
	$(TARGET) 4000 32 75

# 2. REGLA: benchmark (Ejecución Completa del Script)
benchmark: $(TARGET) run_benchmark.sh
	@echo "--- 📊 Ejecutando Benchmark Completo (Sección 5.e) ---"
	chmod +x run_benchmark.sh
	./run_benchmark.sh

# 3. REGLA: graphic (Generación de Gráficos)
# Asegura que el benchmark se haya ejecutado (para tener el CSV) y luego ejecuta el script de Python.
graphic: $(TARGET) benchmark
	@echo "--- 📈 Generando Gráficas de Desempeño (Sección 5.e y 6.b) ---"
	python3 $(GRAPHIC_SCRIPT)

# Regla para limpiar archivos generados
clean:
	rm -rf $(OBJ_DIR) $(TARGET) performance_results.csv graphics/
	@echo "Archivos compilados, ejecutable, directorio 'obj' y gráficas eliminados."

.PHONY: all run benchmark graphic clean