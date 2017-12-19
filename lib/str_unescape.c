/*
 * str_unescape.c
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

/*

RFC3986 defines two sets of characters you can use in a URI:

  Reserved Characters: :/?#[]@!$&'()*+,;=

    reserved = gen-delims / sub-delims

    gen-delims = ":" / "/" / "?" / "#" / "[" / "]" / "@"

    sub-delims = "!" / "$" / "&" / "'" / "(" / ")" / "*" / "+" / "," / ";" / "="

    The purpose of reserved characters is to provide a set of
    delimiting characters that are distinguishable from other data
    within a URI. URIs that differ in the replacement of a reserved
    character with its corresponding percent-encoded octet are not
    equivalent.

  Unreserved Characters: A-Za-z0-9-_.~

    unreserved = ALPHA / DIGIT / "-" / "." / "_" / "~"

    Characters that are allowed in a URI but do not have a reserved
    purpose are called unreserved.


ISO/IEC 9899:201x N1570 p67 defines these escape-sequences:

    escape-sequence:
        simple-escape-sequence
        octal-escape-sequence
        hexadecimal-escape-sequence
        universal-character-name        <-- CAVEAT: NOT SUPPORTED HERE!

    simple-escape-sequence: one of
        \' \" \? \\
        \a \b \f \n \r \t \v

    octal-escape-sequence:
        \ octal-digit
        \ octal-digit octal-digit
        \ octal-digit octal-digit octal-digit

    hexadecimal-escape-sequence:
        \x hexadecimal-digit
        hexadecimal-escape-sequence hexadecimal-digit

*/

#include <stddef.h>

#include <str_escape.h>

#include <inc_priv/baseconv.h>

#define ST_REJ    -1
#define ST_ACC     0
#define ST_ACC1    1
#define ST_ESC     2
#define ST_OCT1    3
#define ST_OCT2    4
#define ST_HEX0    5
#define ST_HEX1    6
#define ST_HEXN    7

static inline int unesc( unsigned char b, int st, char *cp )
{
    int d;

    switch ( st )
    {
        case ST_ACC:
        case ST_ACC1:
            if ( '\\' == b )
                return ST_ESC;
            return *cp = b, ST_ACC;
            break;
        case ST_ESC:
            if ( 'x' == b )
                return ST_HEX0;
            if ( 0 <= ( d = OTOD( b ) ) )
                return *cp = d, ST_OCT1;
            switch ( b )
            {
            case '\'': *cp = '\''; break;
            case '"':  *cp = '"';  break;
            case '?':  *cp = '?';  break;
            case '\\': *cp = '\\'; break;
            case 'a':  *cp = '\a'; break;
            case 'b':  *cp = '\b'; break;
            case 'f':  *cp = '\f'; break;
            case 'n':  *cp = '\n'; break;
            case 'r':  *cp = '\r'; break;
            case 't':  *cp = '\t'; break;
            case 'v':  *cp = '\v'; break;
            default:
                return *cp = b, ST_REJ;
                break;
            }
            return ST_ACC;
            break;
        case ST_HEX0:
            if ( 0 <= ( d = XTOD( b ) ) )
                return *cp = d, ST_HEXN;
            return *cp = b, ST_REJ;
            break;
        case ST_HEXN:
            if ( 0 <= ( d = XTOD( b ) ) )
                return *cp = (*cp << 4) | d, ST_HEXN;
            return ST_ACC1;
            break;
        case ST_OCT1:
            if ( 0 <= ( d = OTOD( b ) ) )
                return *cp = (*cp << 3) | d, ST_OCT2;
            return ST_ACC1;
            break;
        case ST_OCT2:
            if ( 0 <= ( d = OTOD( b ) ) )
                return *cp = (*cp << 3) | d, ST_ACC;
            return ST_ACC1;
            break;
        default:
            break;
    }
    return ST_REJ;
}

static inline int urldec( unsigned char b, int st, char *cp )
{
    int d;

    switch ( st )
    {
        case ST_ACC:
        case ST_ACC1:
            if ( '%' == b )
                return ST_HEX0;
            return *cp = b, ST_ACC;
            break;
        case ST_HEX0:
            if ( 0 <= ( d = XTOD( b ) ) )
                return *cp = d, ST_HEX1;
            return *cp = b, ST_REJ;
            break;
        case ST_HEX1:
            if ( 0 <= ( d = XTOD( b ) ) )
                return *cp = (*cp << 4) | d, ST_ACC;
            return *cp = b, ST_REJ;
            break;
        default:
            break;
    }
    return ST_REJ;
}


/*
 **** str_unescape 3
 **
 ** NAME
 **   str_unescape, str_urldecode - decode escaped or URL encoded strings
 **
 ** SYNOPSIS
 **   #include <str_unescape.h>
 **
 **   size_t str_unescape(char *buf, size_t sz, const char *s, size_t *errcnt);
 **   size_t str_urldecode(char *buf, size_t sz, const char *s, size_t *errcnt);
 **
 ** DESCRIPTION
 **   The str_unescape() function copies characters from the null
 **   terminated source character array s to the destination buf,
 **   replacing any C-style escape sequences by their designated
 **   replacement characters.
 **   At most sz bytes are written to buf, which is always null
 **   terminated. The objects pointed to by buf and s, respectively,
 **   are allowed to overlap, to allow for in-place conversion of
 **   mutable strings.
 **   If errcnt is not NULL, the number of failed conversions is
 **   stored in *errcnt.
 **
 **   The str_urldecode() function works similar, but decodes URL
 **   encoded ("percent-encoded") sequences, according to RFC3986
 **   sect. 2.2.
 **
 ** RETURN VALUE
 **   The str_unescape() and str_urldecode() functions return the total
 **   number of bytes required for the conversion (excluding the null
 **   byte used to terminate the string). In particular, if the returned
 **   value is less than sz, there was sufficient space in buf and the
 **   conversion was successful.
 **
 ** NOTES
 **   It is possible for the str_unescape() function to produce unexpected
 **   results, when a '\\xHH' hexadecimal escape sequence is immediately
 **   followed by one or more ordinary characters representing hexadecimal
 **   digits, which causes str_unescape() to take them as being part of
 **   the escape sequence.
 **   This behavior is owed to the problematic grammar production for
 **   hexadecimal escape sequences in the C standard, and not a genuine
 **   bug in str_unescape().
 **
 ** SEE ALSO
 **   str_escape(3), str_urlencode(3)
 **
 */

size_t str_unescape( char *buf, size_t sz, const char *s, size_t *errcnt )
{
    char c = '\0';
    const char *p = s;
    size_t n = 0, e = 0, err = 0;
    int st = ST_ACC;

    while ( *p )
    {
        st = unesc( *p, st, &c );
        if ( ST_REJ == st )
        {
            ++err;
            st = ST_ACC;
        }
        if ( ST_ACC == st || ST_ACC1 == st )
        {
            if ( n + 1 < sz )
            {
                buf[n++] = c;
                e = n;
            }
            else
                ++n;
            if ( ST_ACC1 == st )
                continue;
        }
        ++p;
    }
    /* Handle dangling conversions. */
    if ( ST_HEXN == st || ST_OCT1 == st || ST_OCT2 == st )
    {
        if ( n + 1 < sz )
        {
            buf[n++] = c;
            e = n;
        }
        else
            ++n;
    }
    else if ( ST_ESC == st || ST_HEX0 == st )
        ++err;
    buf[e] = '\0';
    if ( errcnt )
        *errcnt = err;
    return n;
}

size_t str_urldecode( char *buf, size_t sz, const char *s, size_t *errcnt )
{
    char c = '\0';
    const char *p = s;
    size_t n = 0, e = 0, err = 0;
    int st = ST_ACC;

    while ( *p )
    {
        st = urldec( *p, st, &c );
        if ( ST_REJ == st )
        {
            ++err;
            st = ST_ACC;
        }
        if ( ST_ACC == st || ST_ACC1 == st )
        {
            if ( n + 1 < sz )
            {
                buf[n++] = c;
                e = n;
            }
            else
                ++n;
            if ( ST_ACC1 == st )
                continue;
        }
        ++p;
    }
    /* Handle dangling conversions. */
    if ( ST_ESC == st || ST_HEX0 == st || ST_HEX1 == st )
        ++err;
    buf[e] = '\0';
    if ( errcnt )
        *errcnt = err;
    return n;
}

/* EOF */
