#include <time.h>
#include <unistd.h>

#include <ntime.h>

#include "testsupp.h"

int REGISTER(ntime)( int id )
{
    int err = 0;
    const ntime_t ec = -1;
    ntime_t a, b, k, l;
    time_t t;

    if ( ntime_res() == ec )    { ++err; FAIL( id, "ntime_res()" ); }
    if ( ntime_get() == ec )    { ++err; FAIL( id, "ntime_get()" ); }
    if ( nclock_res() == ec )   { ++err; FAIL( id, "nclock_res()" ); }
    if ( nclock_get() == ec )   { ++err; FAIL( id, "nclock_get()" ); }

    t = time( NULL );
    a = ntime_get() / 1000 / 1000 / 1000;
    k = nclock_get() / 1000 / 1000 / 1000;
    sleep( 2 );
    b = ntime_get() / 1000 / 1000 / 1000;
    l = nclock_get() / 1000 / 1000 / 1000;
    if ( a - t > 1 || a - t < 0 ) { ++err; FAIL( id, "ntime_get() deviates from time()" ); }
    if ( b - a < 1 || b - a > 2 ) { ++err; FAIL( id, "ntime_get() wobble" ); }
    if ( l - k < 1 || l - k > 2 ) { ++err; FAIL( id, "nclock_get() wobble" ); }

    if ( !err )
        PASS( id, "ntime" );
    return err;
}

/* EOF */
