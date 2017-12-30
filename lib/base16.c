/*
 * base16.c
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

#include <stdint.h>

#include <base16.h>

#include <inc_priv/baseconv.h>


/*
 **** b16_encode 3
 **
 ** NAME
 **   b16_encode - convert data into textual hexadecimal representation
 **
 ** SYNOPSIS
 **   #include <base16.h>
 **
 **   size_t b16_encode(char *buf, size_t sz, const void *s, size_t len);
 **
 ** DESCRIPTION
 **   The b16_encode() function stores len bytes from s into buf, in
 **   their respective hexadecimal character representation. At most
 **   sz bytes are placed in buf, which is always null terminated.
 **   The objects pointed to by buf and s shall not overlap.
 **
 ** RETURN VALUE
 **   The b16_encode() function returns the total number of bytes
 **   required for the conversion excluding the null terminator. In
 **   particular, if the returned value is less than sz, there was
 **   sufficient space in buf and the conversion was successful.
 **
 ** SEE ALSO
 **   b16_decode(3), base16_h(3)
 **
 */
size_t b16_encode( char *buf, size_t sz, const void *s, size_t len )
{
    const uint8_t *p = s;
    size_t i, n, e;

    for ( i = n = e = 0; i < len; ++i )
    {
        if ( n + 2 < sz )
        {
            buf[n++] = DTOX(p[i] >> 4);
            buf[n++] = DTOX(p[i] & 0xf);
            e = n;
        }
        else
            n += 2;
    }
    buf[e] = '\0';
    return n;
}

/*
 **** b16_decode 3
 **
 ** NAME
 **   b16_decode - convert hexadecimal character data to binary
 **
 ** SYNOPSIS
 **   #include <base16.h>
 **
 **   int b16_decode(char *buf, size_t sz, const void *s, size_t len, size_t *errcnt);
 **
 ** DESCRIPTION
 **   The b16_decode() function reads pairs of hexadecimal digits
 **   from s and stores their binary representation in buf.
 **   At most sz bytes are written to buf, which is always null
 **   terminated. The objects pointed to by buf and s, respectively,
 **   are allowed to overlap, to allow for in-place conversion of
 **   mutable strings. If errcnt is not NULL, the number of failed
 **   conversions is stored in *errcnt.
 **
 ** RETURN VALUE
 **   The b16_decode() function returns the total number of bytes
 **   required for the conversion (excluding the null terminator).
 **   In particular, if the returned value is less than sz, there
 **   was sufficient space in buf and the conversion was successful.
 **
 ** SEE ALSO
 **   b16_encode(3), base16_h(3)
 **
 */
int b16_decode( char *buf, size_t sz, const void *s, size_t len, size_t *errcnt )
{
    const uint8_t *p = s;
    int c, d;
    size_t i, n, e, err = 0;
    int st = 0;

    for ( i = n = e = 0; i < len; ++i )
    {
        if ( 0 == st )
        {
            c = XTOD( p[i] );
            if ( 0 >  c )
            {
                ++err;
                continue;
            }
        }
        else
        {
            d = XTOD( p[i] );
            if ( 0 >  d )
                ++err;
            else if ( n + 1 < sz )
            {
                buf[n++] = (c << 4) | d;
                e = n;
            }
            else
                ++n;
        }
        st = !st;
    }
    buf[e] = '\0';
    if ( errcnt )
        *errcnt = err;
    return n;
}


/* EOF */
