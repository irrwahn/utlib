/*
 * str_escape.c
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

#define ESC_URL_MASK  0x01
#define ESC_NUM_MASK  0x02
#define ESC_SYM_MASK  0x04

static const unsigned char esc_d[] = {
/*        _0 _1 _2 _3 _4 _5 _6 _7  _8 _9 _a _b _c _d _e _f */
/* 0_ */   7, 3, 3, 3, 3, 3, 3, 7,  7, 7, 7, 7, 7, 7, 3, 3,
/* 1_ */   3, 3, 3, 3, 3, 3, 3, 3,  3, 3, 3, 3, 3, 3, 3, 3,
/* 2_ */   1, 1, 7, 1, 1, 1, 1, 1,  1, 1, 1, 1, 1, 0, 0, 1,
/* 3_ */   0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 1, 1, 1, 1, 1, 1,
/* 4_ */   1, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0,
/* 5_ */   0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 1, 7, 1, 1, 0,
/* 6_ */   1, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0,
/* 7_ */   0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 1, 1, 1, 0, 1,
};

#define ESC_URL(c)  ((c & 0x80) || (esc_d[(c)] & ESC_URL_MASK))
#define ESC_NUM(c)  ((c & 0x80) || (esc_d[(c)] & ESC_NUM_MASK))
#define ESC_SYM(c) (!(c & 0x80) && (esc_d[(c)] & ESC_SYM_MASK))

static const char *sym =
        "0......abtnvfr.."     /* 00 .. 0f */
        "................"     /* 10 .. 1f */
        "..\"............."    /* 20 .. 2f */
        "................"     /* 30 .. 3f */
        "................"     /* 40 .. 4f */
        "............\\..."    /* 50 .. 5f */
        ;

/*
 **** str_escape 3
 **
 ** NAME
 **   str_escape, str_urlencode - escape special characters in a string
 **
 ** SYNOPSIS
 **   #include <str_escape.h>
 **
 **   size_t str_escape(char *buf, size_t sz, const char *s);
 **   size_t str_urlencode(char *buf, size_t sz, const char *s);
 **
 ** DESCRIPTION
 **   The str_escape() function copies characters from the null
 **   terminated source character array s to the destination buf.
 **   For special characters either symbolic or octal escape sequences
 **   are generated, that follow the C language rules regarding string
 **   literals. At most sz bytes are written to buf, which is always
 **   null terminated. The objects pointed to by buf and s,
 **   respectively, shall not overlap.
 **
 **   The str_urlencode() function works similar, but escapes special
 **   characters according to RFC3986 sect. 2.2, to URL encode
 **   ("percent-encode") the string.
 **
 ** RETURN VALUE
 **   The str_escape() and str_urlencode() functions return the total
 **   number of bytes required for the conversion (excluding the null
 **   byte used to terminate the string). In particular, if the returned
 **   value is less than sz, there was sufficient space in buf and the
 **   conversion was successful.
 **
 ** NOTES
 **   While it would be feasible to generate hexadecimal instead of
 **   octal escape sequences, that would lead to ambiguous results, and
 **   it could not be guaranteed that the original string can be
 **   reconstructed by str_unescape(3). This is due to the problematic
 **   grammar production for hexadecimal escape sequences in the C
 **   standard.
 **
 ** SEE ALSO
 **   str_unescape(3), str_urldecode(3)
 **
 */

size_t str_escape( char *buf, size_t sz, const char *s )
{
    const unsigned char *p = (const unsigned char *)s;
    size_t n, e;

    for ( n = e = 0; *p; ++p )
    {
        if ( ESC_SYM( *p ) )
        {
            if ( n + 2 < sz )
            {
                buf[n++] = '\\';
                buf[n++] = sym[*p];
                e = n;
            }
            else
                n += 2;
        }
        else if ( ESC_NUM( *p ) )
        {
            if ( n + 4 < sz )
            {
                buf[n++] = '\\';
                buf[n++] = DTOO(*p >> 6);
                buf[n++] = DTOO(*p >> 3);
                buf[n++] = DTOO(*p);
                e = n;
            }
            else
                n += 4;
        }
        else
        {
            if ( n + 1 < sz )
            {
                buf[n++] = *p;
                e = n;
            }
            else
                n++;
        }
    }
    buf[e] = '\0';
    return n;
}

size_t str_urlencode( char *buf, size_t sz, const char *s )
{
    const unsigned char *p = ( const unsigned char *)s;
    size_t n, e;

    for ( n = e = 0; *p; ++p )
    {
        if ( ESC_URL( *p ) )
        {
            if ( n + 3 < sz )
            {
                buf[n++] = '%';
                buf[n++] = DTOX(*p >> 4);
                buf[n++] = DTOX(*p);
                e = n;
            }
            else
                n += 3;
        }
        else
        {
            if ( n + 1 < sz )
            {
                buf[n++] = *p;
                e = n;
            }
            else
                n++;
        }
    }
    buf[e] = '\0';
    return n;
}

/* EOF */
