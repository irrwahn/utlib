/*
 * prng.c
 *
 * THIS SOFTWARE IS UNFIT FOR CRYPTOGRAPHIC PURPOSES!
 *
 * Copyright 2016-2017 Urban Wallasch <irrwahn35@freenet.de>
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

#include <prng.h>

static prng_random_ctx_t ctx_unsafe = PRNG_RANDOM_CTX_INITIALIZER;

/*
 **** prng_random 3
 **
 ** NAME
 **   prng_random, prng_random_r, prng_random_uni, prng_random_uni_r, prng_srandom, prng_srandom_r - fast pseudo random number generator
 **
 ** SYNOPSIS
 **   #include <prng.h>
 **
 **   extern uint64_t prng_random(void);
 **   extern uint64_t prng_random_r(prng_random_ctx_t *ctx);
 **
 **   extern uint64_t prng_random_uni(uint64_t upper);
 **   extern uint64_t prng_random_uni_r(prng_random_ctx_t *ctx, uint64_t upper);
 **
 **   extern void prng_srandom(uint64_t seed);
 **   extern void prng_srandom_r(prng_random_ctx_t *ctx, uint64_t seed);
 **
 ** DESCRIPTION
 **   The prng_random() function generates a pseudo random number in the
 **   range 0 <= n <= PRNG_RANDOM_MAX, that is, it picks from the closed
 **   interval [0;PRNG_RANDOM_MAX]. The prng_random_r() function does the
 **   same, but stores the PRNG state information via the pointer ctx in
 **   a user-supplied object of type prng_random_ctx_t.
 **
 **   The prng_random_uni() function generates an unbiased pseudo random
 **   number in the range 0 <= n < upper, that is, it picks from the
 **   right-open interval [0;upper[. The prng_random_uni_r() function does
 **   the same, but stores the PRNG state information via the pointer ctx
 **   in a user-supplied object of type prng_random_ctx_t.
 **
 **   The prng_srandom() function initializes the PRNG state using the
 **   seed provided. The prng_srandom_r() function does the same, but
 **   stores the PRNG state information via the pointer ctx in a 
 **   user-supplied object of type prng_random_ctx_t.
 **
 ** RETURN VALUE
 **   The prng_random() and prng_random_r() functions return the generated
 **   pseudo random number.
 **
 **   The prng_random_uni() and prng_random_uni_r() functions return the
 **   generated pseudo random number, or 0 if upper equals 0.
 **
 ** ERRORS
 **   EINVAL  The upper argument given to prng_srandom() or prng_srandom_r() was 0.
 **
 ** NOTES
 **   The prng_random(), prng_random_uni() and prng_srandom() functions
 **   are not thread safe by design.
 **   The prng_random_r(), prng_random_uni_r() and prng_srandom_r() should
 **   be used in situations where thread safety is a requirement.
 **
 **   This PRNG is a 64-bit three-rotate variety of Bob Jenkins' simple
 **   and fast PRNG, based on the public domain implementation by
 **   Robert J. Jenkins Jr.
 **
 **   Pros: simple; good average cycle length; decent avalanche; fast mixing
 **
 **   Cons: conceptually limited to values no wider than 64 bits; not fit for any cryptographic purpose
 **
 **   Info: http://burtleburtle.net/bob/rand/smallprng.html
 **
 **       https://web.archive.org/web/20170518164425/http://www.burtleburtle.net:80/bob/rand/smallprng.html
 **
 **   It is strongly recommended against using this pseudo random
 **   generator implementation for any cryptographic purpose!
 **
 ** BUGS
 **   The prng_random_r(), prng_random_uni_r() and prng_srandom_r()
 **   functions are not safeguarded against NULL pointer values passed
 **   for ctx; this is a deliberate design decision.
 **
 ** SEE ALSO
 **   prng_h(3)
 **
 */

uint64_t prng_random( void )
{
    return prng_random_r( &ctx_unsafe );
}

uint64_t prng_random_r( prng_random_ctx_t *ctx )
{
#define LEFTROT(x,k)    (((x)<<(k))|((x)>>(64-(k))))
    uint64_t
         e = ctx->a - LEFTROT( ctx->b,  7 );
    ctx->a = ctx->b ^ LEFTROT( ctx->c, 13 );
    ctx->b = ctx->c + LEFTROT( ctx->d, 37 );
    ctx->c = ctx->d + e;
    ctx->d = e + ctx->a;
    return ctx->d & PRNG_RANDOM_MAX;
#undef LEFTROT
}


uint64_t prng_random_uni( uint64_t upper )
{
    return prng_random_uni_r( &ctx_unsafe, upper );
}

uint64_t prng_random_uni_r( prng_random_ctx_t *ctx, uint64_t upper )
{
    if ( 0 == upper )
        return errno = EINVAL, 0;

    /* Wait for a random number in the biggest range divisible by
     * `upper` without remainder.  This way the modulo operation
     * cannot introduce any bias.  Not expected to loop terribly
     * often, if at all.
     */
    uint64_t r = 0;
    uint64_t cutoff = (PRNG_RANDOM_MAX % upper) + 1;
    cutoff = PRNG_RANDOM_MAX - ( cutoff == upper ? 0 : cutoff );
    while ( ( r = prng_random_r( ctx ) ) > cutoff )
        ;
    return r % upper;
}


void prng_srandom( uint64_t seed )
{
    prng_srandom_r( &ctx_unsafe, seed );
}

void prng_srandom_r( prng_random_ctx_t *ctx, uint64_t seed )
{
    ctx->a = PRNG_RANDOM_FLEASEED;
    ctx->b = ctx->c = ctx->d = seed;
    for ( int i = 20; i--; )
        prng_random_r( ctx );
}


/* EOF */
