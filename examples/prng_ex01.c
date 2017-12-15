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
