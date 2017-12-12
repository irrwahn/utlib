#include "testmain.h"

int main( int argc, char *argv[] )
{
    (void)argc;
    (void)argv;
    int r, cnt = 0;

    for ( int t = 0; testf[t]; ++t )
    {
        r = testf[t]( t );
        cnt += !!r;
    }
    return r;
}
