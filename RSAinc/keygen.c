#include <stdio.h>
#include <stdlib.h>
#include <time.h>

unsigned long generate_prime() {
    unsigned long primes[] = {10007, 10009, 10037, 10039, 10061};
    int index = rand() % 5;
    return primes[index];
}

unsigned long mod_inverse(unsigned long e, unsigned long phi) {
    long t = 0, new_t = 1;
    long r = phi, new_r = e;
    long quotient, temp;

    while (new_r != 0) {
        quotient = r / new_r;
        temp = t;
        t = new_t;
        new_t = temp - quotient * new_t;
        temp = r;
        r = new_r;
        new_r = temp - quotient * new_r;
    }

    if (t < 0) t += phi;
    return (unsigned long)t;
}

int main() {
    unsigned long p, q, n, phi, e, d;
    FILE *bad;
    FILE *pub;

    srand(time(NULL));

    p = generate_prime();
    q = generate_prime();
    n = p * q;
    phi = (p-1)*(q-1);
    e = 65537;
    d = mod_inverse(e, phi);

    bad = fopen("unsafe.bad", "w");
    if (!bad) return 1;
    fprintf(bad, "-----BEGIN UNSAFE PRIVATE KEY-----\n");
    fprintf(bad, "%lx\n", n);
    fprintf(bad, "%lx\n", e);
    fprintf(bad, "%lx\n", d);
    fprintf(bad, "-----END UNSAFE PRIVATE KEY-----\n");
    fclose(bad);

    pub = fopen("unsafe.pub", "w");
    if (!pub) return 1;
    fprintf(pub, "-----BEGIN UNSAFE PUBLIC KEY-----\n");
    fprintf(pub, "%lx\n", n);
    fprintf(pub, "%lx\n", e);
    fprintf(pub, "-----END UNSAFE PUBLIC KEY-----\n");
    fclose(pub);

    return 0;
}
