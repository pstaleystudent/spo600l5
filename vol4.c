// vol4.c :: volume scaling in C using AArch64 SIMD inline assembler
// Chris Tyler 2017.11.29-2021.11.16 - Licensed under GPLv3.
// For the Seneca College SPO600 Course

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "vol.h"

int main() {

#ifndef __aarch64__
        printf("Wrong architecture - written for aarch64 only.\n");
#else


        // these variables will also be accessed by our assembler code
        int16_t*        in_cursor;              // input cursor
        int16_t*        out_cursor;             // output cursor
        int16_t         vol_int;                // volume as int16_t

        int16_t*        limit;                  // end of input array

        int             x;                      // array interator
        int             ttl=0 ;                 // array total

// ---- Create in[] and out[] arrays
        int16_t*        in;
        int16_t*        out;
        in=(int16_t*) calloc(SAMPLES, sizeof(int16_t));
        out=(int16_t*) calloc(SAMPLES, sizeof(int16_t));

// ---- Create dummy samples in in[]
        vol_createsample(in, SAMPLES);

// ---- This is the part we're interested in!
// ---- Scale the samples from in[], placing results in out[]


        // set vol_int to fixed-point representation of the volume factor
        // Q: should we use 32767 or 32768 in next line? why?
        // A: 32767 is correct - this is converting a fraction of 100 to a fraction of max int16, which is 32767
        vol_int = (int16_t)(VOLUME/100.0 * 32767.0);

        // Q: what is the purpose of these next two lines?
        // A: Initialize both cursors to the start of the arrays, and limit to point to the end of the array
        in_cursor = in;
        out_cursor = out;
        limit = in + SAMPLES;

        // Q: what does it mean to "duplicate" values in the next line?
        // A: Copies volume into each of the 8 elements in v1
        //    It copies the value into multiple destinations
		
        __asm__ ("dup v1.8h,%w0"::"r"(vol_int)); // duplicate vol_int into v1.8h

        while ( in_cursor < limit ) {
                __asm__ (
                        "ldr q0, [%[in_cursor]], #16    \n\t"
                        // load eight samples into q0 (same as v0.8h)
                        // from [in_cursor]
                        // post-increment in_cursor by 16 bytes
                        // ans store back into the pointer register

                        "sqrdmulh v0.8h, v0.8h, v1.8h   \n\t"
                        // with 32 signed integer output,
                        // multiply each lane in v0 * v1 * 2
                        // saturate results
                        // store upper 16 bits of results into
                        // the corresponding lane in v0

                        "str q0, [%[out_cursor]],#16            \n\t"
                        // store eight samples to [out_cursor]
                        // post-increment out_cursor by 16 bytes
                        // and store back into the pointer register

                        // Q: What do these next three lines do?

                        // A: Output operands - defines C variables to access in ASM
                        : [in_cursor]"+r"(in_cursor), [out_cursor]"+r"(out_cursor)
                        // A: Input operands - defines C variables to access in ASM
                        : "r"(in_cursor),"r"(out_cursor)
                        // A: Clobbers - alerts compiler that ASM accesses additional memory not listed above
                        : "memory"
                        );
        }

// --------------------------------------------------------------------

        for (x = 0; x < SAMPLES; x++) {
                ttl=(ttl+out[x])%1000;
        }

        // Q: are the results usable? are they correct?
        printf("Result: %d\n", ttl);

        return 0;

#endif
}