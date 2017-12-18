#include "testsupp.h"

#include <string.h>

#include <str_escape.h>
#include <str_unescape.h>

REGISTER( str_escape_test )
{
    int i, err = 0, bad = 0;
    size_t n, e;
    char buf[500];
    static const struct {
        const char *org;
        const char *esc;
        const char *url;
    } str_otest[] = {
        { "",   /* original */
          "",   /* escaped */
          ""    /* url encoded */
        },
        { " ",
          " ",
          "%20"
        },
        { "\\",
          "\\\\",
          "%5C"
        },
        { "abc\n",
          "abc\\n",
          "abc%0A"
        },
        { "\a\b\f\n\r\t\v",
          "\\a\\b\\f\\n\\r\\t\\v",
          "%07%08%0C%0A%0D%09%0B"
        },
        { "ÄÖÜäöü",
          "\\xC3\\x84\\xC3\\x96\\xC3\\x9C\\xC3\\xA4\\xC3\\xB6\\xC3\\xBC",
          "%C3%84%C3%96%C3%9C%C3%A4%C3%B6%C3%BC"
        },
        { "€",
          "\\xE2\\x82\\xAC",
          "%E2%82%AC"
        },
        {
          NULL,
          NULL,
          NULL
        }
    };

    for ( i = 0; str_otest[i].org; ++i )
    {
        n = str_escape( buf, sizeof buf, str_otest[i].org );
        if ( n >= sizeof buf || strcmp( buf, str_otest[i].esc ) )
        {
            ++err;
            FAIL( "str_escape failed on index %d", i );
            printf( "*******%s******\n", buf );
        }
        n = str_unescape( buf, sizeof buf, buf, &e );
        if ( n >= sizeof buf || e || strcmp( buf, str_otest[i].org ) )
        {
            ++err;
            FAIL( "str_unescape failed on index %d", i );
        }
    }
    if ( !err )
        PASS( "str_escape/str_unescape test %d/%d", i, i );
    bad += err;
    err = 0;
    for ( i = 0; str_otest[i].org; ++i )
    {
        n = str_urlencode( buf, sizeof buf, str_otest[i].org );
        if ( n >= sizeof buf || strcmp( buf, str_otest[i].url ) )
        {
            ++err;
            FAIL( "str_urlencode failed on index %d", i );
        }
        n = str_urldecode( buf, sizeof buf, buf, &e );
        if ( n >= sizeof buf || e || strcmp( buf, str_otest[i].org ) )
        {
            ++err;
            FAIL( "str_urldecode failed on index %d", i );
        }
    }
    if ( !err )
        PASS( "str_urlencode/str_urldecode test %d/%d", i, i );
    bad += err;
    err = 0;

    static const struct {
        const char *s;
        size_t n_esc;
        size_t e_esc;
        size_t n_url;
        size_t e_url;
    } str_fail[] = {
        { " \\",   1, 1,   2, 0 },
        { "\\x",   0, 1,   2, 0 },
        { "\\xD",  1, 0,   3, 0 },
        { " %",    2, 0,   1, 1 },
        { "%1",    2, 0,   0, 1 },
        { NULL,    0, 0,   0, 0 }
    };
    int n_exp = 0, e_exp = 0, gd = 0, bd = 0;
    for ( i = 0; str_fail[i].s; ++i )
    {
        n_exp += str_fail[i].n_esc + str_fail[i].n_url;
        e_exp += str_fail[i].e_esc + str_fail[i].e_url;

        n = str_unescape( buf, sizeof buf, str_fail[i].s, &e );
        gd += n;
        bd += e;
        if ( n != str_fail[i].n_esc || e != str_fail[i].e_esc )
        {
            ++err;
            FAIL( "str_unescape failed on index %d", i );
        }
        n = str_urldecode( buf, sizeof buf, str_fail[i].s, &e );
        gd += n;
        bd += e;
        if ( n != str_fail[i].n_url || e != str_fail[i].e_url )
        {
            ++err;
            FAIL( "str_urldecode failed on index %d", i );
        }
    }
    if ( !err )
        PASS( "str_unescape/str_urldecode test %d/%d good, %d/%d expected bad", gd, n_exp, bd, e_exp );
    else
        FAIL( "str_unescape/str_urldecode test %d/%d good, %d/%d expected bad", gd, n_exp, bd, e_exp );
    bad += err;
    return bad;
}


/* EOF */
