// p152.c by Eric Brown. Submitted for Fall CSC 152 on 9/24/2024.
// Programmed by myself and didn't use any outside resources.

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

// Static means that it is not exported for linking and so
// can only be called from this file.
static uint32_t rotl32(uint32_t x, int n) {
    return (x << n) | (x >> (32-n));
}

// ap - dp are references to four variables to be mixed.
// A good compiler will keep everything in registers, so even
// though it looks like we're reading memory, we probably aren't.
void mix(uint32_t *ap, uint32_t *bp, uint32_t *cp, uint32_t *dp) {
    // Receive the four words to be mixed
    uint32_t a = *ap;
    uint32_t b = *bp;
    uint32_t c = *cp;
    uint32_t d = *dp;
    // Mix the four words

    a += b;
    d ^= a;
    d = rotl32(d, 16);
    c += d;
    b ^= c;
    b = rotl32(b, 12);
    a += b;
    d ^= a;
    d = rotl32(d, 8);
    c += d;
    b ^= c;
    b = rotl32(b, 7);

    // Update the caller's values
    *ap = a;
    *bp = b;
    *cp = c;
    *dp = d;
}

void p152(void *in, void *out) {
    uint32_t input[16];
    memcpy(input, in, 64);

    for (int i = 0; i < 10; i++) {
        for (int column = 0; column < 4; column++) {
            mix(&input[column], &input[column + 4], &input[column + 8], &input[column + 12]);
        }

        for (int diagonal = 0; diagonal < 4; diagonal++) {
            mix(&input[diagonal], &input[(diagonal + 1) % 4 + 4], &input[(diagonal + 2) % 4 + 8], &input[(diagonal + 3) % 4 + 12]);
        }
    }

    memcpy(out, input, 64);
}

#ifdef MAIN  // Set to 1 while testing and 0 for submission

#include <stdio.h>

int main() {
    unsigned char buf[64] = {1};  // Puts 1 in frist byte, rest get auto zeroed
    p152(buf, buf);
    // As a test, output the first 16 bytes of the output
    printf("Is       : ");
    for (int i=0; i<16; i++)
        printf("%02x", buf[i]);
    printf("\n");
    printf("Should be: 14627e9771052d97a8a025cc5531572f\n");
}

#endif