#ifndef COMMON_H
#define COMMON_H

#include <stddef.h>
#include <immintrin.h>
#include <chrono>

#define ASCII_CASE_DIFF 0x20
#define VECTOR_SIZE_BYTES 32

enum CaseMode { TO_UPPER, TO_LOWER };

char* generate_random_string(size_t length, size_t alignment, int alpha_percent);
void case_converter_serial(char* text, size_t m, CaseMode mode);
void case_converter_SIMD(char* text, size_t m, CaseMode mode);

using Clock = std::chrono::high_resolution_clock;

#endif // COMMON_H