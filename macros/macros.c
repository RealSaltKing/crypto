#include <stdio.h>
#include <stdint.h>

/* Bitwise CHOICE macro */
#define CHOICE(e, f, g) ((e & f) | (~e & g))

/* Bitwise MEDIAN macro */
#define MEDIAN(e, f, g) ((e & f) | (f & g) | (g & e))

/* Bitwise Right ROTATE macro */
#define ROTATE(x, n) ((x >> n) | (x << (32 - n)))

