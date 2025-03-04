#include "4096_t.h"
#include <string.h>
#include <stdint.h>
#include <stdio.h>


uint64_t bigadd(uint64_t *a, uint64_t *b, uint64_t *result) {
    uint64_t carry = 0;
    for (size_t i = 0; i < S; i++) {
        uint64_t sum = a[i] + b[i] + carry;
        carry = (sum < a[i]) ? 1 : 0;
        result[i] = sum;
    }
    return 0;
}

uint64_t bigsub(uint64_t *a, uint64_t *b, uint64_t *result) {
    uint64_t borrow = 0;
    for (size_t i = 0; i < S; i++) {
        uint64_t diff = a[i] - b[i] - borrow;
        borrow = (a[i] < b[i] + borrow) ? 1 : 0;
        result[i] = diff;
    }
    return 0;
}

uint64_t bigmul(uint64_t *in0, uint64_t *in1, uint64_t *out) {
    uint64_t carry = 0;
    
    // Initialize the output array to zero
    memset(out, 0, BYTES);
    
    // Loop through each element of in0 and in1
    for (size_t i = 0; i < S; i++) {
        for (size_t j = 0; j < S; j++) {
            // Calculate the multiplication of the two 64-bit values
            __uint128_t result = (__uint128_t)in0[i] * (__uint128_t)in1[j] + out[i + j] + carry; 
            
            // Store the lower 64 bits of the result in out[i+j]
            out[i + j] = (uint64_t)result;
            
            // Calculate the new carry from the upper 64 bits of the result
            carry = (uint64_t)(result >> 64);
        }
    }

    // Return any final carry if necessary
    return carry;
}



/* I'm tapping out here lol. Couldnt figure out how to get the division to work
and the multiplacation already took a good while for me to fully get right*/

uint64_t bigquo(uint64_t *num, uint64_t *den, uint64_t *quo) {
    // Return 0 for now since this function is a pass through
    (void)num; 
    (void)den;
    (void)quo;
    return 0;  
}

uint64_t bigrem(uint64_t *num, uint64_t *den, uint64_t *rem) {
    // Return 0 for now since this function is a pass through
    (void)num; 
    (void)den;
    (void)rem;
    return 0;  
}
