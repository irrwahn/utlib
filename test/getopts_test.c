
#include <stdio.h>
#include <string.h>

#include <getopts.h>

#include "testsupp.h"


#define MY_OPT_a    101
#define MY_OPT_b    102
#define MY_OPT_c    103
#define MY_OPT_d    104
#define MY_OPT_e    105

static getopts_t opts[] = {
    { MY_OPT_a, OPTARG_REQUIRED, 'a',  "aaa",    "laber" },
    { MY_OPT_b,               0, 'b',  "bbb",  "schwall" },
    { MY_OPT_c, OPTARG_OPTIONAL, 'c',   NULL,    "blubb" },
    { MY_OPT_d,               0,   0,   "dd",   "sabbel" },
    { MY_OPT_e, OPTARG_OPTIONAL, 'e', "eeee", "salbader" },
    { 0 }
};

static int n = 0, pos;
static char out[500];

static int my_cb( int idx, int id, const char *arg )
{
    pos = idx;
    n += snprintf( out + n, sizeof out - n, "%d:%d:'%s'|", idx, id, arg );
    return 0;
}

static size_t str_diff( const char *s1, const char *s2 )
{
    size_t i;
    for ( i = 0; s1[i] && s2[i] && s1[i] == s2[i]; ++i )
        ;
    return i;
}

REGISTER( getopts_test )
{
    int argc, err = 0;
    size_t n;
    const char *expect =
        "1:101:'AAA'|3:101:'BBB'|4:104:''|5:0:'nonopt'|6:102:''|6:103:'YY'|"
        "7:102:''|8:103:''|9:103:'b'|10:101:'-2'|11:-2:'a'|12:-1:'3'|"
        "14:101:'4'|15:105:'XXXX'|16:105:''|17:-1:'d'|19:0:''|";
    char *argv[] = {
        "dummy",
        "--aaa=AAA",
        "--aaa=",
        "BBB",
        "--dd",
        "nonopt",
        "-bcYY",
        "-b",
        "-c",
        "-cb",
        "-a-2",
        "-a",
        "-3",
        "-a",
        "4",
        "--eeee=XXXX",
        "-e",
        "-d",
        "--",
        "fin",
        NULL
    };
    for ( argc = 0; argv[argc]; ++argc )
        ;
    getopts( argc, argv, opts, my_cb );

    if ( ( n = str_diff( out, expect ) ) != strlen( out ) )
    {
        MESG( "%s", out );
        MESG( "%*s", (int)n, "^" );
        MESG( "%s", expect );
        FAIL( "getopts_test at position %zu", n );
    }
    else
        PASS( "getopts_test %d/%d", argc-1, argc-1 );
    return err;
}

/* EOF */
