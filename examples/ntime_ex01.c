#include <stdio.h>
#include <ntime.h>

int main( void )
{
    ntime_t nt;
    time_t t;
    char buf[20];

    printf( "Wall clock time resolution is %"PRI_ntime" ns\n", ntime_res() );
    printf( "Wall clock time is %"PRI_ntime" ns\n", nt = ntime_get() );
    t = ntime_to_s( nt );
    strftime( buf, sizeof buf, "%F %T", localtime( &t ) );
    printf( "\t(%s.%06"PRI_ntime")\n\n", buf, (nt - ntime_from_s(t))/1000 );

    printf( "Monotonic clock resolution is %"PRI_ntime" ns\n", nclock_res() );
    printf( "Monotonic clock is %"PRI_ntime" ns\n", nt = nclock_get() );
    printf( "\t(%"PRI_ntime" mins)\n", ntime_to_s( nt ) / 60 );

    return 0;
}

/* EOF */
