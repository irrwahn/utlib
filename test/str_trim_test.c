#include <string.h>

#include <str_trim.h>

#include "testsupp.h"


REGISTER( str_move_test )
{
    int i, err = 0;
    char s[100];
    const char *o  = "Hello World";
    const char *r[] = {
        "World",
        "Hello Hello World",
    };

    i = 0;
    strcpy( s, o );
    if ( strcmp( str_move( s, s + 6 ), r[i] ) )
    {
        ++err;
        FAIL( "str_move failed on index %d", i );
    }
    ++i;

    strcpy( s, o );
    if ( strcmp( str_move( s + 6, s ), o ) || strcmp( s, r[i] ) )
    {
        ++err;
        FAIL( "str_move failed on index %d", i );
    }
    ++i;

    if ( !err )
        PASS( "str_move_test %d/%d", i, i );
    return err;
}


REGISTER( str_trim_test )
{
    int i, err = 0;
    char buf[400];
    static const struct {
        const char *o;
        const char *tr;
        const char *ltr;
        const char *rtr;
    } str_ttest[] = {
        /*    orig    trim   ltrim   rtrim */
        {       "",     "",     "",     "" },
        {      " ",     "",     "",     "" },
        {     "  ",     "",     "",     "" },
        {      "x",    "x",    "x",    "x" },
        {     " x",    "x",    "x",   " x" },
        {     "x ",    "x",   "x ",    "x" },
        {    " x ",    "x",   "x ",   " x" },
        {  " x\t ",    "x", "x\t ",   " x" },
        {  " x y ",  "x y", "x y ", " x y" },
        {     NULL,   NULL,   NULL,   NULL }
    };

    for ( i = 0; str_ttest[i].o; ++i )
    {
        strcpy( buf, str_ttest[i].o );
        if ( strcmp( str_trim(buf), str_ttest[i].tr ) )
        {
            ++err;
            FAIL( "str_trim failed on index %d", i );
        }
        strcpy( buf, str_ttest[i].o );
        if ( strcmp( str_ltrim(buf), str_ttest[i].ltr ) )
        {
            ++err;
            FAIL( "str_ltrim failed on index %d", i );
        }
        strcpy( buf, str_ttest[i].o );
        if ( strcmp( str_rtrim(buf), str_ttest[i].rtr ) )
        {
            ++err;
            FAIL( "strrtrim failed on index %d", i );
        }
    }
    if ( !err )
        PASS( "str_trim_test %d/%d", i, i );
    return 0;
}


REGISTER( str_skip_test )
{
    int i, err = 0;
    static const struct {
        const char *o;
        const char *set;
        const char *sk;
        const char *sksp;
    } str_stest[] = {
        /*        orig      skipset          skip    skipspace */
        {           "",         "",           "",           "" },
        {          " ",         "",          " ",           "" },
        {          " ",        " ",           "",           "" },
        {  " \r\n\v\t",        " ",   "\r\n\v\t",           "" },
        {          "Y",        " ",          "Y",          "Y" },
        {         " Y",        " ",          "Y",          "Y" },
        {        " xY",        "x",        " xY",         "xY" },
        {        " xY",       "x ",          "Y",         "xY" },
        {        " xY",       " x",          "Y",         "xY" },
        {        " xY",      "Yx ",           "",         "xY" },
        {     "abc Y ",     "abc ",         "Y ",     "abc Y " },
        {       " abc",     " aBC",         "bc",        "abc" },
        {         NULL,       NULL,         NULL,         NULL }
    };

    for ( i = 0; str_stest[i].o; ++i )
    {
        if ( strcmp( str_skip( str_stest[i].o, str_stest[i].set ), str_stest[i].sk ) )
        {
            ++err;
            FAIL( "str_skip failed on index %d", i );
        }
        if ( strcmp( str_skipspace( str_stest[i].o ), str_stest[i].sksp ) )
        {
            ++err;
            FAIL( "str_skipspace failed on index %d", i );
        }
    }
    if ( !err )
        PASS( "str_skip_test %d/%d", i, i );
    return 0;
}

/* EOF */
