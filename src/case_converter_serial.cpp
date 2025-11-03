#include "common.h"

void case_converter_serial(char* text, size_t m, CaseMode mode) {
    for (size_t i = 0; i < m; ++i) {
        char& c = text[i]; 

        if (mode == TO_UPPER) {
            if (c >= 'a' && c <= 'z') {
                c -= ASCII_CASE_DIFF;
            }
        } else { // TO_LOWER
            if (c >= 'A' && c <= 'Z') {
                c += ASCII_CASE_DIFF;
            }
        }
    }
}