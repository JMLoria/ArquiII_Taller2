# Directorios
BUILD_DIR = obj
SRC_DIR = src

# Nombre del programa ejecutable (se almacenará en obj/)
TARGET = case_converter

# Compilador y flags
CXX = g++
# Flags: C++17, optimización máxima (-O3), y optimización para la CPU actual (-march=native)
CXXFLAGS = -std=c++17 -O3 -march=native

# Archivos fuente y de header
SRCS = $(wildcard $(SRC_DIR)/*.cpp)
HDRS = $(SRC_DIR)/common.h

# Archivos objeto: cambia el directorio de src/ a obj/ y la extensión de .cpp a .o
OBJS = $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRCS))

# Regla por defecto: compilar todo
all: $(BUILD_DIR)/$(TARGET)
	@echo "Compilación exitosa. El ejecutable se encuentra en $(BUILD_DIR)/."
	@echo "Para ejecutar, use 'make run'"

# 1. Asegurar que el directorio de objetos exista
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# 2. Linkeo: enlaza los objetos en el ejecutable final
$(BUILD_DIR)/$(TARGET): $(OBJS) | $(BUILD_DIR)
	$(CXX) $(OBJS) -o $@

# 3. Regla de compilación: compila los archivos fuente en obj/
# Depende del archivo .cpp en src/ y el header común
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp $(HDRS) | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Regla para ejecutar el programa
run: $(BUILD_DIR)/$(TARGET)
	@echo "--- Ejecutando el benchmark (Sección 5.e) ---"
	./$(BUILD_DIR)/$(TARGET)

# Regla para limpiar archivos generados
clean:
	rm -rf $(BUILD_DIR)
	@echo "Directorio $(BUILD_DIR) y su contenido (objetos y ejecutable) eliminados."

.PHONY: all run clean