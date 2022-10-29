// vol3.c - dummy volume scaling function
// Chris Tyler 2017.11.29-2021.11.16 - Licensed under GPLv3.
// For the Seneca College SPO600 Course

// Q: What's the point of this dummy program? How does it help
// with benchmarking?
// A: It can be used as a reference to remove time taken from setup from the other algorithms

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "vol.h"

int16_t scale_sample(int16_t sample, int volume) {

        return (int16_t) 100;
}

int main() {
        int             x;
        int             ttl=0;

// ---- Create in[] and out[] arrays
        int16_t*        in;
        int16_t*        out;
        in=(int16_t*) calloc(SAMPLES, sizeof(int16_t));
        out=(int16_t*) calloc(SAMPLES, sizeof(int16_t));

// ---- Create dummy samples in in[]
        vol_createsample(in, SAMPLES);

// ---- This is the part we're interested in!
// ---- Scale the samples from in[], placing results in out[]
        for (x = 0; x < SAMPLES; x++) {
                out[x]=scale_sample(in[x], VOLUME);
        }

// ---- This part sum the samples. (Why is this needed?)
        for (x = 0; x < SAMPLES; x++) {
                ttl=(ttl+out[x])%1000;
        }

// ---- Print the sum of the samples. (Why is this needed?)
        printf("Result: %d\n", ttl);

        return 0;
}