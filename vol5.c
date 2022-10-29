// vol_intrinsics.c :: volume scaling in C using AArch64 Intrinsics
// Chris Tyler 2017.11.29-2021.11.16 - Licensed under GPLv3.
// For the Seneca College SPO600 Course

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#ifdef __aarch64__
#include <arm_neon.h>
#endif
#include "vol.h"

int main() {

#ifndef __aarch64__
        printf("Wrong architecture - written for aarch64 only.\n");
#else

        register int16_t*       in_cursor       asm("r20");     // input cursor (pointer)
        register int16_t*       out_cursor      asm("r21");     // output cursor (pointer)
        register int16_t        vol_int         asm("r22");     // volume as int16_t

        int16_t*                limit;          // end of input array

        int                     x;              // array interator
        int                     ttl=0;          // array total

// ---- Create in[] and out[] arrays
        int16_t*        in;
        int16_t*        out;
        in=(int16_t*) calloc(SAMPLES, sizeof(int16_t));
        out=(int16_t*) calloc(SAMPLES, sizeof(int16_t));

// ---- Create dummy samples in in[]
        vol_createsample(in, SAMPLES);

// ---- This is the part we're interested in!
// ---- Scale the samples from in[], placing results in out[]

        vol_int = (int16_t) (VOLUME/100.0 * 32767.0);

        in_cursor = in;
        out_cursor = out;
        limit = in + SAMPLES ;

        while ( in_cursor < limit ) {
                // What do these intrinsic functions do?
                // (See gcc intrinsics documentation)
                vst1q_s16(out_cursor, vqrdmulhq_s16(vld1q_s16(in_cursor), vdupq_n_s16(vol_int)));
				

                // Q: Why is the increment below 8 instead of 16 or some other value?
                // A: _s16 means the operations are being done on 16-bit values. 8 values fit in each 128-bit register.

                // Q: Why is this line not needed in the inline assembler version of this program?
                // A: Addition was done in the ldr and str operations
                in_cursor += 8;
                out_cursor += 8;
        }

// --------------------------------------------------------------------

        for (x = 0; x < SAMPLES; x++) {
                ttl=(ttl+out[x])%1000;

        }

        // Q: Are the results usable? Are they accurate?
        // A: Presumably
        printf("Result: %d\n", ttl);

        return 0;
#endif
}
