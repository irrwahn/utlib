#include "testmain.h"
#include "testsupp.h"

int main( int argc, char *argv[] )
{
    (void)argc;
    (void)argv;
    int r, t, errcnt = 0;

    for ( t = 0; testf[t]; ++t )
    {
        r = testf[t]( t );
        errcnt += !!r;
    }
    if ( errcnt )
        fprintf( stderr, "%d of %d tests failed!\n", errcnt, t );
    else
        fprintf( stderr, "All %d linked test modules succeeded.\n", t );
    return errcnt;
}

/* EOF */
