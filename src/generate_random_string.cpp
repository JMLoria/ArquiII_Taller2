#include "common.h"
#include <string>
#include <random>
#include <stdexcept>
#include <malloc.h>

char* generate_random_string(size_t length, size_t alignment, int alpha_percent) {
    const std::string ALPHABET_LOWER = "abcdefghijklmnopqrstuvwxyz";
    const std::string ALPHABET_UPPER = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    const std::string NON_ALPHABETIC = "0123456789 !@#$%^&*()_+-=[]{};':\",.<>/?|`~\\";

    std::random_device rd;
    std::mt19937 gen(rd());
    
    std::uniform_int_distribution<> alpha_dist(0, 99);
    std::uniform_int_distribution<> lower_dist(0, ALPHABET_LOWER.length() - 1);
    std::uniform_int_distribution<> upper_dist(0, ALPHABET_UPPER.length() - 1);
    std::uniform_int_distribution<> non_alpha_dist(0, NON_ALPHABETIC.length() - 1);
    
    char* text = (char*)_mm_malloc(length + 1, alignment);
    if (!text) {
        throw std::bad_alloc();
    }
    
    for (size_t i = 0; i < length; ++i) {
        if (alpha_percent > 0 && alpha_dist(gen) < alpha_percent) {
            if (alpha_dist(gen) < 50) {
                text[i] = ALPHABET_LOWER[lower_dist(gen)];
            } else {
                text[i] = ALPHABET_UPPER[upper_dist(gen)];
            }
        } else {
            text[i] = NON_ALPHABETIC[non_alpha_dist(gen)];
        }
    }
    
    text[length] = '\0';
    
    return text;
}