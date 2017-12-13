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
    fprintf( stderr, "Total: %d of %d tests failed.\n", errcnt, t );
    return errcnt;
}

/* EOF */
