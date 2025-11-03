#include "common.h"
#include <algorithm>

void case_converter_SIMD(char* text, size_t m, CaseMode mode) {
    size_t i = 0;
    
    const __m256i v_diff = _mm256_set1_epi8(ASCII_CASE_DIFF); 
    const __m256i v_zero = _mm256_setzero_si256();             
    const __m256i v_all_ones = _mm256_set1_epi8(0xFF); 

    if (mode == TO_UPPER) {
        const __m256i v_a_minus_1 = _mm256_set1_epi8('a' - 1);
        const __m256i v_z = _mm256_set1_epi8('z');

        for (; i + VECTOR_SIZE_BYTES <= m; i += VECTOR_SIZE_BYTES) {
            __m256i v_data = _mm256_loadu_si256((__m256i*)(text + i));

            __m256i mask_ge_a = _mm256_cmpgt_epi8(v_data, v_a_minus_1);
            __m256i mask_gt_z = _mm256_cmpgt_epi8(v_data, v_z);
            __m256i mask_le_z = _mm256_xor_si256(mask_gt_z, v_all_ones);

            __m256i v_mask = _mm256_and_si256(mask_ge_a, mask_le_z);

            __m256i v_shift = _mm256_blendv_epi8(v_zero, v_diff, v_mask);
            __m256i v_result = _mm256_sub_epi8(v_data, v_shift);

            _mm256_storeu_si256((__m256i*)(text + i), v_result);
        }
    } else {
        const __m256i v_A_minus_1 = _mm256_set1_epi8('A' - 1);
        const __m256i v_Z = _mm256_set1_epi8('Z');

        for (; i + VECTOR_SIZE_BYTES <= m; i += VECTOR_SIZE_BYTES) {
            __m256i v_data = _mm256_loadu_si256((__m256i*)(text + i));

            __m256i mask_ge_A = _mm256_cmpgt_epi8(v_data, v_A_minus_1);
            __m256i mask_gt_Z = _mm256_cmpgt_epi8(v_data, v_Z);
            __m256i mask_le_Z = _mm256_xor_si256(mask_gt_Z, v_all_ones);

            __m256i v_mask = _mm256_and_si256(mask_ge_A, mask_le_Z);

            __m256i v_shift = _mm256_blendv_epi8(v_zero, v_diff, v_mask);
            __m256i v_result = _mm256_add_epi8(v_data, v_shift);

            _mm256_storeu_si256((__m256i*)(text + i), v_result);
        }
    }
    
    for (; i < m; ++i) {
        char& c = text[i];
        if (mode == TO_UPPER) {
            if (c >= 'a' && c <= 'z') {
                c -= ASCII_CASE_DIFF;
            }
        } else {
            if (c >= 'A' && c <= 'Z') {
                c += ASCII_CASE_DIFF;
            }
        }
    }
}