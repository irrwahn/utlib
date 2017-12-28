#include <stdio.h>
#include <getopts.h>

static int optind = 1;

static getopts_t opts[] = {
    { 1, OPTARG_REQUIRED, 'x',  "xxx", "" },
    { 2,     OPTARG_NONE,   0, "abcd", "" },
    { 3, OPTARG_OPTIONAL, 'c',     "", "" },
    { 0, 0, 0, NULL, NULL }
};

static int opt_cb( int idx, int id, const char *arg )
{
    optind = idx;
    printf( "next idx=%d, ", idx );
    switch ( id )
    {
    case OPTERR_NON_OPT:
        printf( "non-option %s\n", arg );
        break;
    case OPTERR_LAST_OPT:
        printf( "end-of-options\n" );
        break;
    case OPTERR_UNKNOWN_OPT:
        printf( "unrecognized option %s\n", arg );
        break;
    case OPTERR_MISSING_ARG:
        printf( "missing arg ument for option %s\n", arg );
        break;
    case 1:
    case 2:
    case 3:
        printf( "opt ID: %d%s%s\n", id,
            arg&&*arg?", optarg=":"", arg&&*arg?arg:"" );
        break;
    default:
        break;  // unreachable
    }
    return 0;
}

int main( int argc, char *argv[] )
{
    int r = getopts( argc, argv, opts, opt_cb );
    printf( "getopts() returned %d\n", r );
    // Print remaining arguments, if any:
    for ( int i = optind; i < argc; ++i )
        printf( "cmdline arg #%d: %s\n", i, argv[i] );
    return 0;
}

/* EOF */
