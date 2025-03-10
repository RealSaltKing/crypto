#include <stdio.h>
#include <stdlib.h>
#include <string.h>

unsigned long modexp(unsigned long base, unsigned long exp, unsigned long mod) {
    unsigned long result = 1;
    while (exp > 0) {
        if (exp % 2 == 1) {
            result = (result * base) % mod;
        }
        base = (base * base) % mod;
        exp /= 2;
    }
    return result;
}

int main(int argc, char *argv[]) {
    FILE *keyfile, *infile, *outfile;
    unsigned long n, e, d, message, result;
    size_t len;
    unsigned char *buffer;
    size_t read_result;

    if (argc != 4) {
        printf("Usage: %s <-e|-d> <input_file> <output_file>\n", argv[0]);
        return 1;
    }

    if (strcmp(argv[1], "-e") == 0) {
        keyfile = fopen("unsafe.pub", "r");
        if (!keyfile) return 1;
        if (fscanf(keyfile, "-----BEGIN UNSAFE PUBLIC KEY-----\n%lx\n%lx\n-----END UNSAFE PUBLIC KEY-----\n", &n, &e) != 2) {
            fclose(keyfile);
            return 1;
        }
        fclose(keyfile);
    } else if (strcmp(argv[1], "-d") == 0) {
        keyfile = fopen("unsafe.bad", "r");
        if (!keyfile) return 1;
        if (fscanf(keyfile, "-----BEGIN UNSAFE PRIVATE KEY-----\n%lx\n%lx\n%lx\n-----END UNSAFE PRIVATE KEY-----\n", &n, &e, &d) != 3) {
            fclose(keyfile);
            return 1;
        }
        fclose(keyfile);
    } else {
        printf("Invalid flag. Use -e to encrypt or -d to decrypt.\n");
        return 1;
    }

    infile = fopen(argv[2], "rb");
    if (!infile) return 1;
    outfile = fopen(argv[3], "wb");
    if (!outfile) {
        fclose(infile);
        return 1;
    }

    fseek(infile, 0, SEEK_END);
    len = ftell(infile);
    rewind(infile);

    buffer = malloc(len);
    if (!buffer) {
        fclose(infile);
        fclose(outfile);
        return 1;
    }

    read_result = fread(buffer, 1, len, infile);
    if ((size_t)read_result != len) {
        fclose(infile);
        fclose(outfile);
        free(buffer);
        return 1;
    }

    message = *(unsigned long*)buffer;
    result = (strcmp(argv[1], "-e") == 0) ? modexp(message, e, n) : modexp(message, d, n);

    fwrite(&result, sizeof(unsigned long), 1, outfile);

    fclose(infile);
    fclose(outfile);
    free(buffer);

    return 0;
}