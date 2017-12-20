#include "testsupp.h"

#include <string.h>

#include <base16.h>

REGISTER( base16_test )
{
    int i, err = 0;
    size_t n, e;
    char buf[500];
    static const struct {
        const char *org;
        const char *esc;
    } str_otest[] = {
        { "",   /* original */
          "",   /* escaped */
        },
        { " ",
          "20",
        },
        { "\\\\",
          "5C5C",
        },
        { "abc\n",
          "6162630A",
        },
        { "\a\b\f\n\r\t\v",
          "07080C0A0D090B",
        },
        { "ÄÖÜäöü",
          "C384C396C39CC3A4C3B6C3BC",
        },
        { "€",
          "E282AC",
        },
        {
          NULL,
          NULL,
        }
    };

    for ( i = 0; str_otest[i].org; ++i )
    {
        n = b16_encode( buf, sizeof buf, str_otest[i].org, strlen(str_otest[i].org) );
        if ( n >= sizeof buf || strcmp( buf, str_otest[i].esc ) )
        {
            ++err;
            FAIL( "b16_encode failed on index %d", i );
        }
        n = b16_decode( buf, sizeof buf, buf, strlen( buf ), &e );
        if ( n >= sizeof buf || e || strcmp( buf, str_otest[i].org ) )
        {
            ++err;
            FAIL( "b16_decode failed on index %d", i );
        }
    }
    if ( !err )
        PASS( "b16_encode/b16_decode test %d/%d", i, i );
    return err;
}

/* EOF */
