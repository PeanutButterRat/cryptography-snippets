// miner.c by Eric Brown. Submitted for CSC 152 for Fall 2024.
// Programmed by myself.
// Used https://wiki.openssl.org/index.php/EVP_Message_Digests for example reference.

#include <openssl/evp.h>
#include <ctype.h>

int count_leading_zero_bits(unsigned char *data, int length);
int increment_data(unsigned char *data, int length);
int max(int a, int b);

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("usage: miner <bits>\n");
        return 1;
    }

    int leading_zero_bits = atoi(argv[1]);
    unsigned char data[61] = { 0 };
    unsigned char digest[32] = { 0 };
    EVP_MD_CTX* context = EVP_MD_CTX_new();
    
    int bytes_needed_for_data = 0;
    unsigned int digest_length = 0;
    
    do {  // Cruch through SHAs until a match is found with sufficient leading 0's, or theoretically run out of combitations.
        bytes_needed_for_data = max(bytes_needed_for_data, increment_data(data, sizeof(data) - 1));
        EVP_DigestInit_ex(context, EVP_sha256(), NULL);
        EVP_DigestUpdate(context, data, bytes_needed_for_data);
        EVP_DigestFinal_ex(context, digest, &digest_length);
    } while (bytes_needed_for_data < sizeof(data) - 1 && count_leading_zero_bits(digest, sizeof(digest)) < leading_zero_bits);

    EVP_MD_CTX_free(context);

    if (bytes_needed_for_data >= sizeof(data)) {  // Shouldn't ever reach here.
        printf("No hash found.");
        return 1;
    }

    printf("%s\n", data);
    printf("0x");

    for (int byte = 0; byte < digest_length; byte++) {
        printf("%02x", digest[byte]);
    }
    
    printf("\n");

    return 0;
}

// Counts the number of 0 bits in a row from left ot right in the buffer.
int count_leading_zero_bits(unsigned char *data, int length) {
    int bits = 0;
    int mask = 1 << 7;

    for (int byte = 0; byte < length; byte++) {
        for (int bit = 0; bit < 8; bit++) {
            int leading_bit = (data[byte] << bit) & mask;
            
            if (leading_bit != 0) {
                return bits;
            }

            bits++;
        }
    }

    return bits;
}

// Increments the buffer data like an integer over alphanumeric ASCII values. Returns the number of bytes the carry needed.
int increment_data(unsigned char *data, int length) {
    for (int byte = 0; byte < length; byte++) {
        data[byte]++;

        while (!isalnum(data[byte]) && data[byte] <= 'z') {
            data[byte]++;
        }

        if (data[byte] < 'z') {  // Byte didn't carry over to next byte.
            return byte + 1;
        } else {
            data[byte] = '0';
        }
    }

    return length + 1;  // The carry carried past the length of the buffer.
}

// Returns the largest of two integers.
int max(int a, int b) {
    return (a > b) ? a : b;
}
