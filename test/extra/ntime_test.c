/*
 * ntime_test.c
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

#if defined(_POSIX_C_SOURCE) && (_POSIX_C_SOURCE < 199309L)
    #undef _POSIX_C_SOURCE
#endif
#define _POSIX_C_SOURCE 199309L


#include <time.h>
#include <unistd.h>

#include <ntime.h>

#include "../testsupp.h"

REGISTER( ntime_test )
{
    int err = 0;
    const ntime_t ec = -1;
    ntime_t a, b, k, l, ta;
    time_t t;
    struct timespec req, rem;

    if ( ntime_res() == ec )    { ++err; FAIL( "ntime_res()" ); }
    if ( ntime_get() == ec )    { ++err; FAIL( "ntime_get()" ); }
    if ( nclock_res() == ec )   { ++err; FAIL( "nclock_res()" ); }
    if ( nclock_get() == ec )   { ++err; FAIL( "nclock_get()" ); }

    t = time( NULL );
    a = ntime_get();
    k = nclock_get();

    /* sleep for 10 ms */
    req = (struct timespec){ .tv_sec = 0, req.tv_nsec = 10 * NT_NS_PER_MS };
    while ( 0 > nanosleep( &req, &rem ) )
        req = rem;

    b = ntime_get();
    l = nclock_get();
    ta = a / NT_NS_PER_S;
    a /= NT_NS_PER_MS;
    b /= NT_NS_PER_MS;
    k /= NT_NS_PER_MS;
    l /= NT_NS_PER_MS;

    /* This should pass on any system: */
    if ( ta - t > 1 || ta - t < 0 ) { ++err; FAIL( "ntime_get() deviates from time()" ); }
    /* These should pass on systems with ticksize <= 10ms: */
    if ( b - a < 10 || b - a > 20 ) { ++err; FAIL( "ntime_get() wobble" ); }
    if ( l - k < 10 || l - k > 20 ) { ++err; FAIL( "nclock_get() wobble" ); }

    if ( !err )
        PASS( "ntime" );
    return err;
}

/* EOF */
