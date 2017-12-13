#include "testsupp.h"

#include <utf8_decode.h>

static struct {
    char *s;
    size_t good;
    size_t bad;
} dectst[] = {
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
    {
        NULL, 0, 0
    }
};

int REGISTER(utf8_dectest)( int id )
{
    size_t e, n;
    int r = 0;

    for ( int i = 0; dectst[i].s; ++i )
    {
        n = utf8_str_count( dectst[i].s, &e );
        if ( n != dectst[i].good || e != dectst[i].bad )
        {
            ++r;
            FAIL( id, "%zu/%zu %zu/%zu", n, dectst[i].good, e, dectst[i].bad );
        }
        else
            PASS( id, "%zu/%zu %zu/%zu", n, dectst[i].good, e, dectst[i].bad );
    }
    return r;
}

/*******************************************/

#include <utf8_encode.h>

static struct {
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
        { EOF, NULL, 0 }
};

static struct encstruct{
    int idx;
    int expgood;
    int expbad;
} usr = {
    0,
    0,
    0,
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
    int *p = (int *)usr;
    *p += 1;
    return 0;
}

int REGISTER(utf8_enctest)( int id )
{
    size_t e, n;

    usr.idx = usr.expgood = usr.expbad = 0;
    n = utf8_stream_encode( get, put, &usr, &e );
    if ( (int)n != usr.expgood || (int)e != usr.expbad )
    {
        FAIL( id, "%zu/%d %zu/%d", n, usr.expgood, e, usr.expbad );
        return 1;
    }
    PASS( id, "%zu/%d %zu/%d", n, usr.expgood, e, usr.expbad );
    return 0;
}

/* EOF */
