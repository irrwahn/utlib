/*
 * prng.h
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

/*
 **** prng_h 3
 **
 ** NAME
 **   prng - fast pseudo random number generator
 **
 ** SYNOPSIS
 **   #include <prng.h>
 **
 ** DESCRIPTION
 **   TYPES
 **     prng_random_ctx_t  opaque structure type to hold pseudo random generator state information
 **
 **   MACROS
 **     PRNG_RANDOM_MAX  largest possible value for PRNG to produce; evaluates to UINT64_MAX
 **
 **     PRNG_RANDOM_CTX_INITIALIZER  evaluates to an expression suitable to initialize static objects of type prng_random_ctx_t
 **
 **   FUNCTIONS
 **     prng_random(), prng_random_r()  generate a pseudo random number from the interval [0;RANDOM_MAX]
 **
 **     prng_random_uni(), prng_random_uni_r()  generate an unbiased pseudo random number less than a given upper bound
 **
 **     prng_srandom(), prng_srandom_r()  seed the pseudo random generator
 **
 ** NOTES
 **   It is strongly recommended against using this pseudo random
 **   generator implementation for any cryptographic purpose!
 **
 ** SEE ALSO
 **   prng_random(3)
 **
 */

#ifndef PRNG_H_INCLUDED
#define PRNG_H_INCLUDED

#ifdef _cplusplus
extern "C" {
#endif

#include <limits.h>
#include <stdint.h>

#define PRNG_RANDOM_MAX  (UINT64_MAX)

#define PRNG_RANDOM_FLEASEED         0xf1ea5eedULL
#define PRNG_RANDOM_CTX_INITIALIZER  { PRNG_RANDOM_FLEASEED, 0, 0, 0 }

struct prng_random_ctx_t_struct {
    uint64_t a, b, c, d;
};

typedef
    struct prng_random_ctx_t_struct
    prng_random_ctx_t;

uint64_t prng_random( void );
uint64_t prng_random_r( prng_random_ctx_t *ctx );

uint64_t prng_random_uni( uint64_t upper );
uint64_t prng_random_uni_r( prng_random_ctx_t *ctx, uint64_t upper );

void prng_srandom( uint64_t seed );
void prng_srandom_r( prng_random_ctx_t *ctx, uint64_t seed );


#ifdef _cplusplus
} /* extern "C" { */
#endif

#endif  //ndef PRNG_H_INCLUDED

/* EOF */
