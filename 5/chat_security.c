// chat_security.c by Eric Brown. Submitted for CSC 152 for Fall 2024.
// Programmed by myself and used https://www.tutorialspoint.com/c_standard_library/ for library functions like memcmp.
// Used https://wiki.openssl.org/index.php/EVP_Symmetric_Encryption_and_Decryption for example reference.

#include <openssl/evp.h>
#include <openssl/rand.h>
#include <string.h>

void chat_encrypt(void *k, void *pt, int ptlen, void *ct) {
        EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
        unsigned char *key = (unsigned char *) k;
        unsigned char *plaintext = (unsigned char *) pt;
        unsigned char *ciphertext = (unsigned char *) ct;
        unsigned char *ciphertext_start = ciphertext + 12;  // First 12 bytes are reserved for nonce.
        unsigned char iv[16];
        unsigned char nonce[12];
        int len = 0;

        // Generate the nonce and IV.
        RAND_bytes(nonce, 12);
        memcpy(ct, nonce, 12);
        memcpy(iv, nonce, 12);
        memset(iv + 12, 0, 4);

        // Encryption.
        EVP_EncryptInit_ex(ctx, EVP_aes_256_ctr(), NULL, key, iv);
        EVP_EncryptUpdate(ctx, ciphertext_start, &len, plaintext, ptlen);
        EVP_EncryptFinal_ex(ctx, ciphertext_start + len, &len);

        EVP_CIPHER_CTX_free(ctx);
}

void chat_decrypt(void *k, void *ct, int ctlen, void *pt) {
        EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
        unsigned char *key = (unsigned char *) k;
        unsigned char *plaintext = (unsigned char *) pt;
        unsigned char *ciphertext = (unsigned char *) ct;
        unsigned char *ciphertext_start = ciphertext + 12;  // First 12 bytes are reserved for nonce.
        unsigned char iv[16];
        unsigned char nonce[12];
        int len = 0;

        // Extract nonce from ciphertext and build IV with it.
        memcpy(nonce, ciphertext, 12);
        memcpy(iv, nonce, 12);
        memset(iv + 12, 0, 4);

        // Decryption.
        EVP_DecryptInit_ex(ctx, EVP_aes_256_ctr(), NULL, key, iv);
        EVP_DecryptUpdate(ctx, plaintext, &len, ciphertext_start, ctlen);
        EVP_DecryptFinal_ex(ctx, plaintext + len, &len);

        EVP_CIPHER_CTX_free(ctx);
}

#ifdef MAIN
#include <stdio.h>

const int NUMBER_OF_TESTS = 10000;
const int PLAINTEXT_LENGTH = 500;
const int CIPHERTEXT_LENGTH = PLAINTEXT_LENGTH + 12;

int main() {
        unsigned char key[32];
        unsigned char original_plaintext[PLAINTEXT_LENGTH];
        unsigned char decrypted_plaintext[PLAINTEXT_LENGTH];
        unsigned char ciphertext[CIPHERTEXT_LENGTH];
        int tests_passed = 0;

        // Run through a bunch of tests with randomly generated plaintexts and keys.
        for (int test = 0; test < NUMBER_OF_TESTS; test++) {
                RAND_bytes(original_plaintext, PLAINTEXT_LENGTH);
                RAND_bytes(key, sizeof(key));

                chat_encrypt(key, original_plaintext, PLAINTEXT_LENGTH, ciphertext);
                chat_decrypt(key, ciphertext, CIPHERTEXT_LENGTH, decrypted_plaintext);

                if (memcmp(original_plaintext, decrypted_plaintext, PLAINTEXT_LENGTH) == 0) {
                        tests_passed++;
                }
        }

        printf("(%d/%d) test cases passed.", tests_passed, NUMBER_OF_TESTS);
        
        return 0;
}

#endif
