// p152stream.c by Eric Brown. Submitted for Fall CSC 152 on 9/24/2024.
// Programmed by myself and didn't use any outside resources.
// Originally incorrectly submitted as mix.c.

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

// Increment buf as if it's a 64 byte big-endian integer
static void increment_block(void *buf) {
    unsigned char *blk = buf;
    int i = 63;
    blk[i] += 1;
    while (blk[i] == 0) {
        i -= 1;
        blk[i] += 1;
    }
}

// k is 32 bytes, n is 16 bytes, 0* is 16 bytes
// Use blk = k || n || 0* to produce output as blk xor p152(blk), then increment blk
// This output is xor'd with the next 64 bytes of buf to encrypt/decrypt
void p152stream(void *k, void *n, void *inbuf, void *outbuf, int nbytes) {
    unsigned char *pinbuf = inbuf;
    unsigned char *poutbuf = outbuf;
    unsigned char in[64];
    unsigned char out[64];
    // Init in as k || n || 0*
    memcpy(in, k, 32);
    memcpy(in+32, n, 16);
    memset(in+48, 0, 16);

    int i = 0;

    while (nbytes >= 64) {
        p152(in, out);

        for (int byte = 0; byte < 64; byte++) {
            out[byte] ^= in[byte] ^ pinbuf[byte + i];
        }

        memcpy(&poutbuf[i], out, 64);
        increment_block(in);
        nbytes -= 64;
        i += 64;
    }

    if (nbytes > 0) {
        p152(in, out);

        for (int byte = 0; byte < nbytes; byte++) {
            out[byte] ^= in[byte] ^ pinbuf[byte + i];
        }

        memcpy(&poutbuf[i], out, nbytes);
    }
}

#if 0  // Set to 1 while testing and 0 for submission

#include <stdio.h>

int main() {
    unsigned char inbuf[200] = {1};
    unsigned char outbuf[200];
    unsigned char k[32] = {1,2,3,4};
    unsigned char n[16] = {1,2,3,4};
    p152stream(k, n, inbuf, outbuf, 200);
    printf("Is       : ");
    for (int i=0; i<200; i++)
        printf("%02x", outbuf[i]);
    printf("\n");
    printf("Should be: d6c0e5ef8745f9fc4657510ce896e96b9f27c18ba5a8cadc7e2a"
           "6872c51c704983726c6633a9e924a5e9a75b8b9980cfad91501f74315fea6da"
           "0936286e5866ac66e8c3d766b6248f88ee99b468dd9fdcf2c4e65e6df35637b"
           "b245246e0cb97ce689c0b91dbd7212257f98744fae42484ea3afbd419db90ca"
           "38a96d4c6e68cd6c003af8b842733ad4162099b2b2d10bfd48a3fb6e8c5e5ea"
           "59dde8bae3206ce3e80f0acad1540e83e2858f39bccec0a4ece5172194f6d15"
           "e7fd5a26a05cb3b8b8fea979965daf5c1\n");
}

#endif
