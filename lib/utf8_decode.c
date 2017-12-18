/*
 * utf8_decode.c
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


#include <stddef.h>

#include <utf8_decode.h>

#include "inc_priv/utf8_indec.h"


/*
 **** utf8_str_count 3
 **
 ** NAME
 **   utf8_str_count, utf8_mem_count - count UTF-8 encoded sequences
 **
 ** SYNOPSIS
 **   #include <utf8_decode.h>
 **
 **   size_t utf8_str_count(const char *s, size_t *errcnt);
 **
 **   size_t utf8_mem_count(void *s, size_t size, size_t *errcnt);
 **
 ** DESCRIPTION
 **   The utf8_str_count() function counts valid UTF-8 encoded Unicode
 **   code points in the character array s until it encounters a
 **   terminating null byte ('\0'), which is not included in the count.
 **   If errcnt is not NULL, utf8_str_count() stores the number of
 **   malformed sequences detected in *errcnt.
 **   The utf8_mem_count() function is similar to utf8_str_count(),
 **   except it inspects exactly size bytes from array s and does not
 **   treat null bytes special.
 **
 ** RETURN VALUE
 **   The utf8_str_count() and utf8_mem_count() functions return the
 **   number of valid UTF-8 encodings found.
 **
 ** NOTES
 **   The utf8_str_count() and utf8_mem_count() functions are not
 **   affected by the current locale setting.
 **
 **   In total the number of valid and rejected sequences equals the
 **   number of code points produced by a decoder that replaces every
 **   rejected sequence with a replacement character.
 **
 ** SEE ALSO
 **   utf8_decode_h(3), utf8_str_decode(3), utf8_stream_decode(3)
 **
 */

size_t utf8_str_count( const char *s, size_t *errcnt )
{
    const char *p;
    int st = UTF8_ACCEPT;
    size_t ok = 0, bad = 0;

    for ( p = s; *p; ++p )
    {
        st = utf8_v( (unsigned char)*p, st );
        if ( UTF8_ACCEPT == st )
            ++ok;
        else if ( UTF8_REJECT == st )
            ++bad, st = UTF8_ACCEPT;
    }
    if ( UTF8_ACCEPT != st )
        ++bad;
    if ( NULL != errcnt )
        *errcnt = bad;
    return ok;
}

size_t utf8_mem_count( void *s, size_t size, size_t *errcnt )
{
    const char *p;
    int st = UTF8_ACCEPT;
    size_t ok = 0, bad = 0;

    for ( p = s; size--; ++p )
    {
        st = utf8_v( (unsigned char)*p, st );
        if ( UTF8_ACCEPT == st )
            ++ok;
        else if ( UTF8_REJECT == st )
            ++bad, st = UTF8_ACCEPT;
    }
    if ( UTF8_ACCEPT != st )
        ++bad;
    if ( NULL != errcnt )
        *errcnt = bad;
    return ok;
}


/*
 **** utf8_str_decode 3
 **
 ** NAME
 **   utf8_str_decode, utf8_mem_decode - decode UTF-8 encoded sequences
 **
 ** SYNOPSIS
 **   #include <utf8_decode.h>
 **
 **   size_t utf8_str_decode(uint32_t *buf, size_t max, const char *s, size_t *errcnt);
 **   size_t utf8_mem_decode(uint32_t *buf, size_t max, void *s, size_t size, size_t *errcnt);
 **
 ** DESCRIPTION
 **   The utf8_str_decode() function decodes valid UTF-8 encoded Unicode
 **   code points in the character array s and stores up to max code
 **   points in buf, until it encounters a terminating null byte ('\0'),
 **   which is not included in the count. For each rejected sequence the
 **   replacement character U+FFFD is produced.
 **   If errcnt is not NULL, utf8_str_decode() stores the number of
 **   malformed sequences detected in *errcnt.
 **   The utf8_mem_decode() function is similar to utf8_str_decode(),
 **   except it inspects exactly size bytes from array s and does not
 **   treat null bytes special.
 **
 ** RETURN VALUE
 **   The utf8_str_decode() and utf8_mem_decode() functions return the
 **   number of valid UTF-8 encodings found.
 **
 ** NOTES
 **   The utf8_str_decode() and utf8_mem_decode() functions are not
 **   affected by the current locale setting.
 **
 **   All decoder output has been successfully stored in buf, if the
 **   total sum of good and bad sequences is less than or equal to max.
 **
 ** SEE ALSO
 **   utf8_decode_h(3), utf8_str_count(3), utf8_stream_decode(3)
 **
 */

size_t utf8_str_decode( uint32_t *buf, size_t max, const char *s, size_t *errcnt )
{
    const char *p;
    int st = UTF8_ACCEPT;
    size_t cnt = 0, bad = 0;
    uint32_t cp = 0;

    for ( p = s; *p; ++p )
    {
        st = utf8_c( (unsigned char)*p, st, &cp );
        if ( UTF8_ACCEPT == st )
        {
            if ( cnt < max )
                buf[cnt] = cp;
            ++cnt;
            cp = 0;
        }
        else if ( UTF8_REJECT == st )
        {
            if ( cnt < max )
                buf[cnt] = UTF_REPLACE_CHAR;
            ++cnt;
            ++bad;
            st = UTF8_ACCEPT;
            cp = 0;
        }
    }
    if ( UTF8_ACCEPT != st )
        ++bad;
    if ( NULL != errcnt )
        *errcnt = bad;
    return cnt - bad;
}

size_t utf8_mem_decode( uint32_t *buf, size_t max, void *s, size_t size, size_t *errcnt )
{
    const char *p;
    int st = UTF8_ACCEPT;
    size_t cnt = 0, bad = 0;
    uint32_t cp = 0;

    for ( p = s; size--; ++p )
    {
        st = utf8_c( (unsigned char)*p, st, &cp );
        if ( UTF8_ACCEPT == st )
        {
            if ( cnt < max )
                buf[cnt] = cp;
            ++cnt;
            cp = 0;
        }
        else if ( UTF8_REJECT == st )
        {
            if ( cnt < max )
                buf[cnt] = UTF_REPLACE_CHAR;
            ++cnt;
            ++bad;
            st = UTF8_ACCEPT;
            cp = 0;
       }
    }
    if ( UTF8_ACCEPT != st )
        ++bad;
    if ( NULL != errcnt )
        *errcnt = bad;
    return cnt - bad;
}


/*
 **** utf8_stream_decode 3
 **
 ** NAME
 **   utf8_stream_decode - decode a stream of UTF-8 encoded data
 **
 ** SYNOPSIS
 **   #include <utf8_decode.h>
 **
 **   size_t utf8_stream_decode(int (*get)(void*), int (*put)(uint32_t,void*), void *usr, size_t *errcnt);
 **
 ** DESCRIPTION
 **   The utf8_stream_decode() function decodes a stream of UTF-8 encoded
 **   data byte by byte. Input and output are delegated to caller
 **   supplied call-back functions.
 **
 **   The call-back function pointed to by get is called whenever a new
 **   byte of input is required. It shall return an unsigned character
 **   value cast to an int, or EOF in case an error occurred.
 **   The function pointed to by put is called whenever a Unicode code
 **   point was successfully decoded and is passed that value as argument.
 **   It shall return a value greater than or equal to zero upon success,
 **   or a negative value to signal an error condition. Whenever one of
 **   these call-back functions returns a negative value,
 **   utf8_stream_decode() immediately stops processing and returns the
 **   counts it accumulated so far.
 **
 **   The user supplied pointer usr is passed to either call-back function
 **   upon each invocation. It can be used to pass context information
 **   around. It is never modified or dereferenced by utf8_stream_decode().
 **
 **   If errcnt is not NULL, utf8_stream_decode() stores the number of
 **   malformed sequences detected in *errcnt.
 **
 ** RETURN VALUE
 **   The utf8_stream_decode() functions returns the number of valid
 **   UTF-8 sequences decoded.
 **
 ** NOTES
 **   For each rejected sequence, utf8_stream_decode() generates the
 **   Unicode replacement character U+FFFD.
 **   The utf8_stream_decode() function is not affected by the current
 **   locale setting.
 **
 ** EXAMPLE
 **  The following minimalistic program reads from stdin and prints
 **  code points in hex notation to stdout:
 **
 ** 	#include <stdio.h>
 ** 	#include <utf8_decode.h>
 **
 ** 	int get( void *usr ) {
 ** 	    return fgetc( stdin );
 ** 	}
 **
 ** 	int put( uint32_t cp, void *usr ) {
 ** 	    return printf( "U+%04lX\n", (unsigned long)cp );
 ** 	}
 **
 ** 	int main( void )
 ** 	{
 ** 	    size_t e, n = utf8_stream_decode( get, put, NULL, &e );
 ** 	    fprintf( stderr, "good: %zd, bad: %zd\\n", n, e );
 ** 	    return !!e;
 ** 	}
 **
 ** SEE ALSO
 **   utf8_decode_h(3), utf8_str_count(3), utf8_str_decode(3)
 **
 */

size_t utf8_stream_decode( int(*get)(void*), int(*put)(uint32_t,void*), void *usr, size_t *errcnt )
{
    int b = 0;
    int st = 0;
    size_t ok = 0, bad = 0;
    uint32_t cp = 0;

    while ( 0 <= ( b = get( usr ) ) )
    {
        st = utf8_c( (unsigned char)b, st, &cp );
        if ( UTF8_ACCEPT == st )
        {
            ++ok;
            if ( 0 > put( cp, usr ) )
                break;
            cp = 0;
        }
        else if ( UTF8_REJECT == st )
        {
            ++bad;
            st = UTF8_ACCEPT;
            if ( 0 > put( UTF_REPLACE_CHAR, usr ) )
                break;
            cp = 0;
        }
    }
    if ( UTF8_ACCEPT != st )
        ++bad;
    if ( NULL != errcnt )
        *errcnt = bad;
    return ok;
}


/* EOF */
