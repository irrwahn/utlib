/*
 * base16_test.c
 *
 * Copyright 2017 Urban Wallasch <irrwahn35@freenet.de>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * * Redistributions of source code must retain the above copyright
 *   notice, this list of conditions and the following disclaimer.
 * * Redistributions in binary form must reproduce the above copyright
 *   notice, this list of conditions and the following disclaimer
 *   in the documentation and/or other materials provided with the
 *   distribution.
 * * Neither the name of the copyright holder nor the names of its
 *   contributors may be used to endorse or promote products derived
 *   from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *
 */

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
