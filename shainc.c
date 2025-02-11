#include <stdio.h>
#include <stdint.h>
#include <string.h>

/* SHA-256 Constants */
static const uint32_t k[64] = {
    0x428a2f98,0x71374491,0xb5c0fbcf,0xe9b5dba5,
    0x3956c25b,0x59f111f1,0x923f82a4,0xab1c5ed5,
    0xd807aa98,0x12835b01,0x243185be,0x550c7dc3,
    0x72be5d74,0x80deb1fe,0x9bdc06a7,0xc19bf174,
    0xe49b69c1,0xefbe4786,0x0fc19dc6,0x240ca1cc,
    0x2de92c6f,0x4a7484aa,0x5cb0a9dc,0x76f988da,
    0x983e5152,0xa831c66d,0xb00327c8,0xbf597fc7,
    0xc6e00bf3,0xd5a79147,0x06ca6351,0x14292967,
    0x27b70a85,0x2e1b2138,0x4d2c6dfc,0x53380d13,
    0x650a7354,0x766a0abb,0x81c2c92e,0x92722c85,
    0xa2bfe8a1,0xa81a664b,0xc24b8b70,0xc76c51a3,
    0xd192e819,0xd6990624,0xf40e3585,0x106aa070,
    0x19a4c116,0x1e376c08,0x2748774c,0x34b0bcb5,
    0x391c0cb3,0x4ed8aa4a,0x5b9cca4f,0x682e6ff3,
    0x748f82ee,0x78a5636f,0x84c87814,0x8cc70208,
    0x90befffa,0xa4506ceb,0xbef9a3f7,0xc67178f2
};

/* Initial hash values */
static uint32_t h[8] = {
    0x6a09e667,0xbb67ae85,0x3c6ef372,0xa54ff53a,
    0x510e527f,0x9b05688c,0x1f83d9ab,0x5be0cd19
};

/* Helper macros for SHA-256 operations */
#define ROTR(x,n) ((x >> n) | (x << (32 - n)))
#define SHR(x,n)  (x >> n)
#define CH(x,y,z) ((x & y) ^ (~x & z))
#define MAJ(x,y,z) ((x & y) ^ (x & z) ^ (y & z))
#define SIGMA0(x) (ROTR(x,2) ^ ROTR(x,13) ^ ROTR(x,22))
#define SIGMA1(x) (ROTR(x,6) ^ ROTR(x,11) ^ ROTR(x,25))
#define sigma0(x) (ROTR(x,7) ^ ROTR(x,18) ^ SHR(x,3))
#define sigma1(x) (ROTR(x,17) ^ ROTR(x,19) ^ SHR(x,10))

/* Padding function */
void sha256_pad(uint8_t *message, uint64_t length, uint8_t *padded_message, uint64_t *new_length) {
    uint64_t bit_len = length * 8;
    *new_length = ((length + 9 + 63) / 64) * 64;
    memcpy(padded_message, message, length);
    padded_message[length] = 0x80;
    memset(padded_message + length + 1, 0, *new_length - length - 9);
    for (int i = 0; i < 8; i++) {
        padded_message[*new_length - 8 + i] = (bit_len >> (8 * (7 - i))) & 0xFF;
    }
}

/* Processing function */
void sha256_process_chunk(uint8_t *chunk, uint32_t *hash) {
    uint32_t w[64], a, b, c, d, e, f, g, h;
    int i;
    for (i = 0; i < 16; i++) {
        w[i] = (chunk[i * 4] << 24) | (chunk[i * 4 + 1] << 16) | (chunk[i * 4 + 2] << 8) | chunk[i * 4 + 3];
    }
    for (i = 16; i < 64; i++) {
        w[i] = sigma1(w[i - 2]) + w[i - 7] + sigma0(w[i - 15]) + w[i - 16];
    }
    a = hash[0]; b = hash[1]; c = hash[2]; d = hash[3];
    e = hash[4]; f = hash[5]; g = hash[6]; h = hash[7];
    for (i = 0; i < 64; i++) {
        uint32_t temp1 = h + SIGMA1(e) + CH(e,f,g) + k[i] + w[i];
        uint32_t temp2 = SIGMA0(a) + MAJ(a,b,c);
        h = g;
        g = f;
        f = e;
        e = d + temp1;
        d = c;
        c = b;
        b = a;
        a = temp1 + temp2;
    }
    hash[0] += a; hash[1] += b; hash[2] += c; hash[3] += d;
    hash[4] += e; hash[5] += f; hash[6] += g; hash[7] += h;
}

/* SHA-256 Main Function */
void sha256(uint8_t *message, uint64_t length, uint8_t *hash) {
    uint8_t padded_message[128];
    uint64_t new_length;
    sha256_pad(message, length, padded_message, &new_length);
    for (uint64_t i = 0; i < new_length; i += 64) {
        sha256_process_chunk(padded_message + i, h);
    }
    for (int i = 0; i < 8; i++) {
        hash[i * 4] = (h[i] >> 24) & 0xFF;
        hash[i * 4 + 1] = (h[i] >> 16) & 0xFF;
        hash[i * 4 + 2] = (h[i] >> 8) & 0xFF;
        hash[i * 4 + 3] = h[i] & 0xFF;
    }
}

/* Main function to test SHA-256 */
int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <filename>\n", argv[0]);
        return 1;
    }
    FILE *file = fopen(argv[1], "rb");
    if (!file) {
        printf("Error opening file\n");
        return 1;
    }
    uint8_t message[64];
    uint8_t hash[32];
    fread(message, 1, 64, file);
    fclose(file);
    sha256(message, strlen((char *)message), hash);
    for (int i = 0; i < 32; i++) {
        printf("%02x", hash[i]);
    }
    printf("\n");
    return 0;
}