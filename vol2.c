// vol2.c - precalculated volume scaling table
// Chris Tyler 2017.11.29-2021.11.16 - Licensed under GPLv3.
// For the Seneca College SPO600 Course

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "vol.h"

int main() {
        int             x;
        int             ttl=0;

// ---- Create in[] and out[] arrays
        int16_t*        in;
        int16_t*        out;
        in=(int16_t*) calloc(SAMPLES, sizeof(int16_t));
        out=(int16_t*) calloc(SAMPLES, sizeof(int16_t));

        static int16_t* precalc;

// ---- Create dummy samples in in[]
        vol_createsample(in, SAMPLES);

// ---- This is the part we're interested in!
// ---- Scale the samples from in[], placing results in out[]

        precalc = (int16_t*) calloc(65536,2);
        if (precalc == NULL) {
                printf("malloc failed!\n");
                return 1;
        }

        for (x = -32768; x <= 32767; x++) {
                // Q: What is the purpose of the cast to unint16_t in the next line?
                // A: Effectively converts the sign bit into 32768, adding 32768 to all values to prevent negative indexes
                precalc[(uint16_t) x] = (int16_t) ((float) x * VOLUME / 100.0);
        }

        for (x = 0; x < SAMPLES; x++) {
                out[x]=precalc[(uint16_t) in[x]];
        }

// ---- This part sums the samples. (Why is this needed?)
        for (x = 0; x < SAMPLES; x++) {
                ttl=(ttl+out[x])%1000;
        }
// ---- Print the sum of the samples. (Why is this needed?)
        printf("Result: %d\n", ttl);

        return 0;
}