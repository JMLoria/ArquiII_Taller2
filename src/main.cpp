#include "common.h"
#include <iostream>
#include <cstring>
#include <stdexcept>
#include <algorithm>
#include <malloc.h> // Para _mm_free

// Función de benchmarking que corre una función y mide el tiempo
long long run_benchmark(void (*func)(char*, size_t, CaseMode), char* text_source, size_t m, CaseMode mode) {
    // Es esencial trabajar sobre una copia temporal para aislar la ejecución
    char* data_copy = (char*)_mm_malloc(m + 1, VECTOR_SIZE_BYTES);
    if (!data_copy) return -1;
    std::memcpy(data_copy, text_source, m + 1);

    auto start = Clock::now();
    
    // Ejecutar el benchmark varias veces (ej. 10 veces) y tomar el tiempo más rápido 
    // para mitigar el ruido del sistema operativo (Opcional: usar solo una para simplicidad)
    // Para simplificar el script, lo ejecutamos una sola vez en este ejemplo.
    func(data_copy, m, mode); 
    
    auto end = Clock::now();

    _mm_free(data_copy);
    
    return std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
}


int main(int argc, char* argv[]) {
    // Verificar que se pasen 3 argumentos (Length, Alignment, Alpha_Percent)
    if (argc != 4) {
        // Imprimir el uso correcto en caso de error
        std::cerr << "Uso: " << argv[0] << " <Length> <Alignment> <Alpha_Percent>" << std::endl;
        return 1;
    }

    // Leer argumentos de la línea de comandos (Sección 5.e)
    const size_t LENGTH = std::stoul(argv[1]);
    const size_t ALIGNMENT = std::stoul(argv[2]);
    const int ALPHA_PERCENT = std::stoi(argv[3]);
    
    // --- 1. Generación de Datos ---
    char* original_text = nullptr;
    try {
        // Generar la cadena de texto con la configuración dada
        original_text = generate_random_string(LENGTH, ALIGNMENT, ALPHA_PERCENT);
        
        // --- 2. Preparación para el benchmark TO_LOWER ---
        // Se necesita una cadena 100% mayúscula para medir la conversión TO_LOWER, 
        // ya que es el peor caso (más conversiones)
        case_converter_serial(original_text, LENGTH, TO_UPPER); 
        
    } catch (const std::exception& e) {
        std::cerr << "Error en la ejecución con parámetros " << LENGTH << "," << ALIGNMENT << "," << ALPHA_PERCENT << ": " << e.what() << std::endl;
        return 1;
    }
    
    // =========================================================================================
    // 3. MEDICIÓN DE RENDIMIENTO (TO_LOWER)
    // Se utiliza TO_LOWER porque la conversión de Mayúsculas a Minúsculas es la operación real.
    // =========================================================================================
    
    long long time_serial = run_benchmark(case_converter_serial, original_text, LENGTH, TO_LOWER);
    long long time_simd = run_benchmark(case_converter_SIMD, original_text, LENGTH, TO_LOWER);
    
    // Imprimir el resultado en formato CSV: Time_Serial,Time_SIMD
    // El script run_benchmarks.sh añadirá los parámetros (Length, Alignment, Alpha_Percent)
    std::cout << time_serial << "," << time_simd << std::endl;

    // --- 4. Liberación de memoria ---
    _mm_free(original_text);

    return 0;
}