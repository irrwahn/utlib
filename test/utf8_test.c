/*
 * utf8_test.c
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

#include <string.h>

#include "testsupp.h"

#include <utf8_decode.h>

static const struct {
    char *s;
    size_t good;
    size_t bad;
} dectst[] = {
    { "Hällø, wörld!\xc3\x95", 14, 0 }, /* some common and diacritic chars */
    { "\xE2\x82\xAC", 1, 0 },           /* genuine euro sign */
    { "\xF0\x82\x82\xAC", 0, 3 },       /* overlong encoding of euro sign */
    { "\xc1\xaf", 0, 2 },               /* overlong 2 byte ASCII encoding */
    { "\xf5\x8f\x8f\x8f\x8f", 0, 5 },   /* invalid 5 byte encoding */
    { "\xf0\x9d\x92\x9c", 1, 0 },       /* valid 4 byte encoding */
    { "\xf0\x9f\xa1\x95", 1, 0 },       /* - ditto - */
    { "\xf0\x9f\x98\x8e", 1, 0 },       /* - ditto - */
    { "\xEF\xBF\xBD", 1, 0 },           /* replacement character U+FFFD */
    { "\xEF\xBF\xBE", 1, 0 },           /* noncharacter U+FFFE */
    { "\xEF\xBF\xBF", 1, 0 },           /* noncharacter U+FFFF */
    { "", 0, 0 },                       /* empty string */
    { NULL, 0, 0 }
};

REGISTER( utf8_dectest )
{
    size_t e, n;
    size_t g, eg, b, eb;
    int r = 0;

    g = eg = b = eb = 0;
    for ( int i = 0; dectst[i].s; ++i )
    {
        n = utf8_str_count( dectst[i].s, &e );
        if ( n != dectst[i].good || e != dectst[i].bad )
        {
            ++r;
            FAIL( "utf8_str_count: i=%d %zu/%zu good, %zu/%zu expected bad",
                        i, n, dectst[i].good, e, dectst[i].bad );
        }
        g += n;
        eg += dectst[i].good;
        b += e;
        eb += dectst[i].bad;

        n = utf8_mem_count( dectst[i].s, strlen(dectst[i].s), &e );
        if ( n != dectst[i].good || e != dectst[i].bad )
        {
            ++r;
            FAIL( "utf8_mem_count: i=%d %zu/%zu good, %zu/%zu expected bad",
                        i, n, dectst[i].good, e, dectst[i].bad );
        }
        g += n;
        eg += dectst[i].good;
        b += e;
        eb += dectst[i].bad;
    }
    if ( !r )
        PASS( "utf8_str_count, utf8_mem_count: %zu/%zu good, %zu/%zu expected bad",
                    g, eg, b, eb );
    return r;
}

/*******************************************/

#include <utf8_encode.h>

static const struct {
    uint32_t u32;
    char *u8;
    int exp;
} enctst[] = {
    { 0x00,   "\x00", 1 },
    { 0x7F,   "\x7F", 1 },
    { 0x80,   "\xC2\x80", 1 },
    { 0x7FF,  "\xDF\xBF", 1 },
    { 0x800,  "\xE0\xA0\x80", 1 },
    { 0xFFFD, "\xEF\xBF\xBD",  1 }, /* replacement character U-FFFD */
    { 0x20AC, "\xE2\x82\xAC",  1 }, /* euro sign */
    { 0xd800, "\xED\xA0\x80", -1 }, /* first invalid UTF-16 surrogate */
    { 0xdbff, "\xED\xB7\xBF", -1 }, /* last invalid UTF-16 surrogate */
    { 0xffff, "\xEF\xBF\xBF", 1 },  /* noncharacter U+FFFF */
    { EOF, NULL, 0 }
};

struct encstruct {
    size_t idx;
    size_t expgood;
    size_t expbad;
};

static uint32_t get( void *usr )
{
    struct encstruct *p = (struct encstruct *)usr;
    uint32_t u = enctst[p->idx].u32;
    if ( (uint32_t)EOF == u )
        return EOF;
    if ( 0 < enctst[p->idx].exp )
        p->expgood += enctst[p->idx].exp;
    else
        p->expbad += -enctst[p->idx].exp;
    return u;
}

static int put( uint8_t *s, int n, void *usr )
{
    (void)s;
    (void)n;
    struct encstruct *p = (struct encstruct *)usr;
    p->idx++;
    return 0;
}

REGISTER( utf8_enctest )
{
    size_t e, n;
    struct encstruct usr = { 0, 0, 0 };

    usr.idx = usr.expgood = usr.expbad = 0;
    n = utf8_stream_encode( get, put, &usr, &e );
    if ( n != usr.expgood || e != usr.expbad )
    {
        FAIL( "utf8_stream_encode: %zu/%zu good, %zu/%zu expected bad",
                    n, usr.expgood, e, usr.expbad );
        return 1;
    }
    PASS( "utf8_stream_encode: %zu/%zu good, %zu/%zu expected bad",
                n, usr.expgood, e, usr.expbad );
    return 0;
}

/* EOF */
