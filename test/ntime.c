#if defined(_POSIX_C_SOURCE) && (_POSIX_C_SOURCE < 199309L)
    #undef _POSIX_C_SOURCE
#endif
#define _POSIX_C_SOURCE 199309L


#include <time.h>
#include <unistd.h>

#include <ntime.h>

#include "testsupp.h"

int REGISTER(ntime)( int id )
{
    int err = 0;
    const ntime_t ec = -1;
    ntime_t a, b, k, l, ta;
    time_t t;
    struct timespec req, rem;

    if ( ntime_res() == ec )    { ++err; FAIL( id, "ntime_res()" ); }
    if ( ntime_get() == ec )    { ++err; FAIL( id, "ntime_get()" ); }
    if ( nclock_res() == ec )   { ++err; FAIL( id, "nclock_res()" ); }
    if ( nclock_get() == ec )   { ++err; FAIL( id, "nclock_get()" ); }

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
    if ( ta - t > 1 || ta - t < 0 ) { ++err; FAIL( id, "ntime_get() deviates from time()" ); }
    /* These should pass on systems with ticksize <= 10ms: */
    if ( b - a < 10 || b - a > 20 ) { ++err; FAIL( id, "ntime_get() wobble" ); }
    if ( l - k < 10 || l - k > 20 ) { ++err; FAIL( id, "nclock_get() wobble" ); }

    if ( !err )
        PASS( id, "ntime" );
    return err;
}

/* EOF */
