/*
 * prng_ex01.c
 *
 * Copyright 2017 Urban Wallasch <irrwahn35@freenet.de>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * * Redistributions of source code must retain the above copyright
 *   notice, this list of conditions and the following disclaimer.
 * * Redistributions in binary form must reproduce the above copyright
 *   notice, this list of conditions and the following disclaimer
 *   in the documentation and/or other materials provided with the
 *   distribution.
 * * Neither the name of the copyright holder nor the names of its
 *   contributors may be used to endorse or promote products derived
 *   from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

#include <prng.h>
#include <ntime.h>

#define UPPER       20
#define MIO    1000000
#define RUNS       150

int main( void )
{
    prng_random_ctx_t rctx;
    static uint64_t rnd[UPPER] = { 0 };
    int64_t avg, dev;
    int i, l;

    /* Initialize PRNG from wall-clock time. */
    prng_srandom_r( &rctx, ntime_get() );

    for ( l = 1; l <= RUNS; ++l )
    {
        /* Sample 10**6 pseudo random numbers from the interval [0;UPPER[. */
        for ( i = 0; i < MIO; ++i )
            ++rnd[ prng_random_uni_r( &rctx, UPPER ) ];

        /* Find average of counts. */
        for ( i = 0, avg = 0; i < UPPER; ++i )
            avg += rnd[i];
        avg /= UPPER;

        /* Calculate and print average deviation. */
        for ( i = 0, dev = 0; i < UPPER; ++i )
            dev += imaxabs( rnd[i] - avg );
        dev /= UPPER;
        printf( "\rIteration %d/%d, average deviation %"PRIi64" ppm  ",
                l, RUNS, dev * MIO / avg );
        fflush( stdout );
    }
    printf( "\n" );
    return 0;
}

/* EOF */
