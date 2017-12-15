#include <string.h>

#include <str_icmp.h>

#include "testsupp.h"


REGISTER( str_icmp_test )
{
    int i, err = 0;
    static const struct {
        const char *s;
        const char *t;
        int exp;
    } str_itest[] = {
        /*          s             t    exp */
        {          "",           "",     0 },
        {       "aaa",        "aaa",     0 },
        {       "aaa",        "AAA",     0 },
        {  "a B !\tc",   "A b !\tC",     0 },
        {         "0",          "1",    -1 },
        {         "1",          "0",     1 },
        {        NULL,         NULL,     0 }
    };

    for ( i = 0; str_itest[i].s; ++i )
    {
        if ( str_icmp( str_itest[i].s, str_itest[i].t ) != str_itest[i].exp )
        {
            ++err;
            FAIL( "str_icmp failed on index %d", i );
        }
    }
    if ( !err )
        PASS( "str_icmp_test %d/%d", i, i );
    return 0;
}

/* EOF */
