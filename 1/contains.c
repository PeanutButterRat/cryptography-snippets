// contains.c by Eric Brown. Submitted for CSC 152 Fall 2024 semester.
// Programmed by myself.
// Used https://www.geeksforgeeks.org/bool-in-c/ for boolean data type reference.

#include <stdint.h>
#include <stdbool.h>

bool is_equal(uint8_t *, uint8_t *, int);

// a points to an alen byte buffer, b points to a blen byte buffer,
// Returns non-zero iff b's buffer is found contiguously inside a's.
int contains(void *a, int alen, void *b, int blen) {
    uint8_t *ap = (uint8_t *) a;
    uint8_t *bp = (uint8_t *) b;
    
    for (int i = 0; i <= alen - blen; i++) {
        if (is_equal(&ap[i], bp, blen)) {
            return 1;
        }
    }

    return 0;
}

// a and b both points to buffers of size length,
// Returns true if they have the same elements.
bool is_equal(uint8_t *a, uint8_t *b, int length) {
    for (int i = 0; i < length; i++) {
        if (a[i] != b[i]) {
            return false;
        }
    }

    return true;
}
