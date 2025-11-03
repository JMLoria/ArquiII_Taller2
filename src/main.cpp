#include "common.h"
#include <iostream>
#include <cstring>
#include <stdexcept>
#include <algorithm>
#include <malloc.h>

// Función auxiliar para imprimir un fragmento de la cadena
void print_test_string(const char* label, const char* text) {
    // Imprimir hasta 80 caracteres para no saturar la salida
    size_t len = std::min((size_t)80, std::strlen(text)); 
    std::cout << label << " (" << std::strlen(text) << " bytes):\n\t" 
              << std::string(text, len) << (std::strlen(text) > 80 ? "..." : "") << std::endl;
}

// Función de benchmarking que corre una función y mide el tiempo
long long run_benchmark(void (*func)(char*, size_t, CaseMode), char* text_source, size_t m, CaseMode mode) {
    // Es esencial trabajar sobre una copia temporal para aislar la ejecución
    char* data_copy = (char*)_mm_malloc(m + 1, VECTOR_SIZE_BYTES);
    if (!data_copy) return -1;
    std::memcpy(data_copy, text_source, m + 1);

    auto start = Clock::now();
    func(data_copy, m, mode);
    auto end = Clock::now();

    _mm_free(data_copy);
    
    return std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
}


int main() {
    // --- Configuración ---
    const size_t LENGTH = 4000;
    const size_t ALIGNMENT = 32;
    const int ALPHA_PERCENT = 75;

    std::cout << "--- Taller SIMD: Conversor de Mayúsculas/Minúsculas ---" << std::endl;
    std::cout << "Configuración: Longitud=" << LENGTH << ", Alineamiento=" << ALIGNMENT 
              << ", % Alfabético=" << ALPHA_PERCENT << "%" << std::endl;

    // --- 1. Generación de Datos ---
    char* original_text = nullptr;
    try {
        original_text = generate_random_string(LENGTH, ALIGNMENT, ALPHA_PERCENT);
        
        std::cout << "Dirección inicial: 0x" << (void*)original_text 
                  << " (Alineamiento mod " << ALIGNMENT << ": " << (size_t)original_text % ALIGNMENT << ")" << std::endl;
        print_test_string("Cadena ORIGINAL (Fragmento)", original_text);
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    // --- 2. Preparación de copias para validación ---
    char* serial_copy = (char*)_mm_malloc(LENGTH + 1, ALIGNMENT);
    char* simd_copy = (char*)_mm_malloc(LENGTH + 1, ALIGNMENT);
    std::memcpy(serial_copy, original_text, LENGTH + 1);
    std::memcpy(simd_copy, original_text, LENGTH + 1);
    
    // --- 3. Preparación de copia para benchmark TO_LOWER ---
    // Creamos una copia que usaremos como entrada 100% mayúscula para medir TO_LOWER
    char* upper_for_lower_benchmark = (char*)_mm_malloc(LENGTH + 1, ALIGNMENT);
    std::memcpy(upper_for_lower_benchmark, original_text, LENGTH + 1);
    // La convertimos a Mayúsculas usando la función serial para asegurar una entrada limpia
    case_converter_serial(upper_for_lower_benchmark, LENGTH, TO_UPPER); 
    
    // =========================================================================================
    // VALIDACIÓN 1: TO_UPPER (Mezclada -> Mayúsculas)
    // =========================================================================================
    
    std::cout << "\n--- 3. Validación de Correctitud (TO_UPPER) ---" << std::endl;
    
    // Ejecución in-place
    case_converter_serial(serial_copy, LENGTH, TO_UPPER);
    case_converter_SIMD(simd_copy, LENGTH, TO_UPPER);
    
    // Impresión de resultados
    print_test_string("Serial (TO_UPPER)", serial_copy);
    print_test_string("SIMD (TO_UPPER)", simd_copy);
    
    // Comprobación
    bool correct_upper = (std::memcmp(serial_copy, simd_copy, LENGTH + 1) == 0);
    std::cout << "Resultado de Validación TO_UPPER: " << (correct_upper ? "CORRECTO ✅" : "FALLÓ ❌") << std::endl;

    // =========================================================================================
    // VALIDACIÓN 2: TO_LOWER (Mayúsculas -> Minúsculas) - Muestra el caso de minúsculas
    // =========================================================================================
    
    std::cout << "\n--- 4. Validación de Correctitud (TO_LOWER) ---" << std::endl;
    
    // 'serial_copy' y 'simd_copy' están ahora en Mayúsculas. Los usamos como entrada para TO_LOWER.
    
    // Ejecución in-place
    case_converter_serial(serial_copy, LENGTH, TO_LOWER);
    case_converter_SIMD(simd_copy, LENGTH, TO_LOWER);
    
    // Impresión de resultados
    print_test_string("Serial (TO_LOWER)", serial_copy);
    print_test_string("SIMD (TO_LOWER)", simd_copy);
    
    // Comprobación
    bool correct_lower = (std::memcmp(serial_copy, simd_copy, LENGTH + 1) == 0);
    std::cout << "Resultado de Validación TO_LOWER: " << (correct_lower ? "CORRECTO ✅" : "FALLÓ ❌") << std::endl;
    
    // =========================================================================================
    // 5. MEDICIÓN DE RENDIMIENTO
    // =========================================================================================
    
    std::cout << "\n--- 5. Medición de Rendimiento (TO_LOWER) ---" << std::endl;
    
    // Usamos la copia 100% mayúscula como entrada (upper_for_lower_benchmark)
    long long time_serial = run_benchmark(case_converter_serial, upper_for_lower_benchmark, LENGTH, TO_LOWER);
    long long time_simd = run_benchmark(case_converter_SIMD, upper_for_lower_benchmark, LENGTH, TO_LOWER);

    std::cout << "Tiempo Serial (nanosegundos): " << time_serial << std::endl;
    std::cout << "Tiempo SIMD (nanosegundos): " << time_simd << std::endl;
    
    if (time_simd > 0 && time_serial > 0) {
        double speedup = (double)time_serial / time_simd;
        std::cout << "Speedup (Aceleración): " << speedup << "x" << std::endl;
    }

    // --- 6. Liberación de memoria ---
    _mm_free(original_text);
    _mm_free(serial_copy);
    _mm_free(simd_copy);
    _mm_free(upper_for_lower_benchmark);

    return 0;
}