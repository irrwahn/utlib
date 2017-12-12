#include "testsupp.h"

#include <utf8_decode.h>

static struct tstruct {
    char *s;
    size_t good;
    size_t bad;
} tst[] = {
    {
        "Hällø, wörld!\xc3\x95"     /* some common and diacritic characters */
        "\xE2\x82\xAC"              /* genuine euro sign */
        "\xF0\x82\x82\xAC "         /* overlong encoding of euro sign */
        "\xc1\xaf "                 /* overlong 2 byte ASCII encoding */
        "\xf5\x8f\x8f\x8f\x8f "     /* invalid 5 byte encoding */
        "\xf0\x9d\x92\x9c"          /* valid 4 byte encoding */
        "\xf0\x9f\xa1\x95"          /* ditto */
        "\xf0\x9f\x98\x8e"          /* ditto */
        "\xEF\xBF\xBD"              /* replacement character U-FFFD */
        "\xEF\xBF\xBE"              /* noncharacter U-FFFE */
        "\xEF\xBF\xBF"              /* noncharacter U-FFFF */
        "", 24, 10
    },
    { NULL, 0, 0 }
};


int REGISTER(utf8_test)( int id )
{
    size_t e, n;

    for ( int i = 0; tst[i].s; ++i )
    {
        n = utf8_str_count( tst[i].s, &e );
        if ( n != tst[i].good || e != tst[i].bad )
            FAIL( id, "" );
        else
            PASS( id, "" );
    }
    return 0;
}
