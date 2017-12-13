#include "testsupp.h"

int REGISTER(example_test)( int id )
{
    if ( 1 )
        PASS( id, "<example pass>" );
    else
        FAIL( id, "<example fail>" );
    return 0;
}

/* EOF */
