/*
 * utf8_encode.c
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


#include <stdio.h>

#include <utf8_encode.h>

#include "inc_priv/utf8_inenc.h"


/*
 **** utf8_str_encode 3
 **
 ** NAME
 **   utf8_str_encode, utf8_mem_encode - encode UTF-32 to UTF-8 sequences
 **
 ** SYNOPSIS
 **   #include <utf8_encode.h>
 **
 **   size_t utf8_str_encode(uint8_t *buf, size_t max, const uint32_t *s, size_t *errcnt);
 **   size_t utf8_mem_encode(uint8_t *buf, size_t max, const uint32_t *s, size_t size, size_t *errcnt);
 **
 ** DESCRIPTION
 **   The utf8_str_encode() function reads valid UTF-32 encoded Unicode
 **   code points from the array s and stores up to max UTF-8 encoded
 **   code points in buf, until it encounters a terminating null word,
 **   which is not included in the count. For each rejected sequence the
 **   replacement character U+FFFD is produced.
 **   If errcnt is not NULL, utf8_str_encode() stores the number of
 **   malformed sequences detected in *errcnt.
 **   The utf8_mem_encode() function is similar to utf8_str_encode(),
 **   except it inspects exactly size bytes from array s and does not
 **   treat null words special.
 **
 ** RETURN VALUE
 **   The utf8_str_encode() and utf8_mem_encode() functions return the
 **   number of valid UTF-8 encodings found.
 **
 ** NOTES
 **   The utf8_str_decode() and utf8_mem_decode() functions are not
 **   affected by the current locale setting.
 **
 **   All decoder output has been successfully stored in buf, if the
 **   returned number of good sequences is less than or equal to max.
 **
 ** SEE ALSO
 **   utf8_encode_h(3), utf8_stream_encode(3)
 **
 */

size_t utf8_str_encode( uint8_t *buf, size_t max, const uint32_t *s, size_t *errcnt )
{
    uint8_t b[5];
    int n;
    size_t ok, bad, cnt, end;
    const uint32_t *p = s;

    for ( cnt = end = ok = bad = 0, p = s; *p; ++p )
    {
        n = utf8_ec( *p, b );
        if ( 0 < n )
            ++ok;
        else
        {
            ++bad;
            n = utf8_ec( UTF_REPLACE_CHAR, b );
        }
        if ( cnt + n < max )
        {
            for ( int i = 0; i < n; ++i )
                buf[cnt++] = b[i];
            end = cnt;
        }
        else
            cnt += n;
    }
    buf[end] = '\0';
    if ( NULL != errcnt )
        *errcnt = bad;
    return ok;
}

size_t utf8_mem_encode( uint8_t *buf, size_t max, const uint32_t *s, size_t size, size_t *errcnt )
{
    uint8_t b[5];
    int n;
    size_t ok, bad, cnt, end;
    const uint32_t *p = s;

    for ( cnt = end = ok = bad = 0, p = s; size--; ++p )
    {
        n = utf8_ec( *p, b );
        if ( 0 < n )
            ++ok;
        else
        {
            ++bad;
            n = utf8_ec( UTF_REPLACE_CHAR, b );
        }
        if ( cnt + n < max )
        {
            for ( int i = 0; i < n; ++i )
                buf[cnt++] = b[i];
            end = cnt;
        }
        else
            cnt += n;
    }
    if ( NULL != errcnt )
        *errcnt = bad;
    return ok;
}


/*
 **** utf8_stream_encode 3
 **
 ** NAME
 **   utf8_stream_encode - encode a UTF-32 character stream to UTF-8
 **
 ** SYNOPSIS
 **   #include <utf8_decode.h>
 **
 **   size_t utf8_stream_encode(uint32_t (*get)(void*), int (*put)(uint8_t*,int,void*), void *usr, size_t *errcnt)
 **
 ** DESCRIPTION
 **   The utf8_stream_encode() function encodes a stream of UTF-32 encoded
 **   data to UTF-8. Input and output are delegated to caller supplied
 **   call-back functions.
 **
 **   The call-back function pointed to by get is called whenever a new
 **   word of input is required. It shall return an 32-bit unsigned integer
 **   value that is a valid Unicode code point, or EOF in case an error
 **   occurred.
 **   The function pointed to by put is called whenever a Unicode code
 **   point was successfully encoded. It is passed the encoding result in
 **   a null terminated byte array and an int containing associated
 **   length information. It shall return a value greater than or equal
 **   to zero upon success, or a negative value to signal an error
 **   condition. Whenever one of these call-back functions returns a
 **   negative value, utf8_stream_encode() immediately stops processing
 **   and returns the counts it accumulated so far.
 **
 **   The user supplied pointer usr is passed to either call-back function
 **   upon each invocation. It can be used to pass context information
 **   around. It is never modified or dereferenced by utf8_stream_encode().
 **
 **   If errcnt is not NULL, utf8_stream_encode() stores the number of
 **   invalid code points detected in *errcnt.
 **
 ** RETURN VALUE
 **   The utf8_stream_encode() functions returns the number of valid
 **   code points successfully encoded.
 **
 ** NOTES
 **   For each rejected code point, utf8_stream_encode() generates the
 **   Unicode replacement character U+FFFD.
 **   The utf8_stream_decode() function is not affected by the current
 **   locale setting.
 **
 ** EXAMPLE
 **  The following pretty useless program reads from stdin and prints
 **  UTF-8 encoded byte sequences to stdout:
 **
 ** 	#include <stdio.h>
 ** 	#include <utf8_encode.h>
 **
 ** 	uint32_t get( void *usr ) {
 ** 	    return fgetc( stdin );
 ** 	}
 **
 ** 	int put( uint8_t *b, int l, void *usr ) {
 ** 	    return printf( "%s", (char *)b );
 ** 	}
 **
 ** 	int main( void )
 ** 	{
 ** 	    size_t e, n = utf8_stream_encode( get, put, NULL, &e );
 ** 	    fprintf( stderr, "good: %zd, bad: %zd\\n", n, e );
 ** 	    return !!e;
 ** 	}
 **
 ** SEE ALSO
 **   utf8_encode_h(3)
 **
 */

size_t utf8_stream_encode( uint32_t(*get)(void*), int(*put)(uint8_t*,int,void*), void *usr, size_t *errcnt )
{
    uint8_t b[5];
    int n;
    size_t ok = 0, bad = 0;
    uint32_t cp = 0;

    while ( (uint32_t)EOF != ( cp = get( usr ) ) )
    {
        n = utf8_ec( cp, b );
        if ( 0 < n )
            ++ok;
        else
        {
            ++bad;
            n = utf8_ec( UTF_REPLACE_CHAR, b );
        }
        b[n] = '\0';
        if ( 0 > put( b, n, usr ) )
            break;
    }
    if ( NULL != errcnt )
        *errcnt = bad;
    return ok;
}


/* EOF */
