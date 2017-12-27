/*
 * getopts.c
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
#include <string.h>

#include <getopts.h>

/*
 **** getopts 3
 **
 ** NAME
 **   getopts - parse argument vector and interpret option and non-option arguments
 **
 ** SYNOPSIS
 **   #include <getopts.h>
 **
 **   int getopts(int argc, char *argv[], getopts_t *odef, int (*cb)(int,int,const char *));
 **
 ** DESCRIPTION
 **   The getopts() function parses an argument vector, most commonly
 **   the arguments passed on the command-line on program invocation.
 **   It is roughly comparable to, and is provided as an alternative to,
 **   the getopt(3) function, but takes a slightly different approach
 **   with respect to the method of invocation.
 **
 **   Its arguments argc and argv are the argument count and array as
 **   passed to the main() function on program invocation. An element
 **   of argv that starts with '-' (which is not immediately followed by
 **   another '-') is interpreted as a short option element.
 **   The characters of this element following the initial '-' are taken
 **   as option characters. A single '-' not followed by any other
 **   characters is interpreted as the literal non-option argument "-".
 **   Any character sequence starting with "--" is interpreted as a long
 **   option. An element consisting only of the sequence "--" not
 **   immediately followed by any other characters immediately stops any
 **   further option processing by the getopts() function.
 **
 **   The odef argument to the getopts() function is a pointer to the
 **   first element of an array of elements of the structure type
 **   getopts_t, defined in <getopts.h> as
 **
 ** 	typedef
 ** 		struct getopts_t_struct {
 ** 			unsigned optid;
 ** 			unsigned arg;
 ** 			char sopt;
 ** 			const char *lopt;
 ** 			const char *usemsg;
 ** 		}
 ** 		getopts_t;
 **
 **   The respective meanings of the different fields are:
 **
 **   optid  a positive integer to uniquely identify an entry; a value of zero indicates the last entry in the array
 **   arg  one of OPTARG_NONE, OPTARG_REQUIRED or OPTARG_OPTIONAL to indicate if the option takes an (possibly optional) argument
 **   sopt  a single character constant declaring a short option
 **   lopt  a character string declaring a long option, excluding the "--" prefix
 **   usemsg  a string that can be used to store a human readable textual description of the option
 **
 **   Either sopt or lopt (but not both at the same time) may be set to
 **   0 or the empty string ("") respectively, to indicate that there is
 **   no short or long variant of the option available.
 **
 **   The cb argument is a pointer to a caller supplied function which
 **   is called once for every argument that is processed by the getopts()
 **   function. It shall have a signature compatible to the prototype
 **
 ** 	int cb( int idx, int id, const char *arg );
 **
 **   Upon invocation it will receive the following arguments substituted
 **   for its formal parameters:
 **
 **   idx  the index of the next element in argv not yet processed.
 **   id  either a (positive) option ID as defined in odef, or one of OPTERR_NON_OPT, OPTERR_LAST_OPT, OPTERR_UNKNOWN_OPT or OPTERR_MISSING_ARG.
 **   arg  either the argument provided with an option, or the offending element in the error case.
 **
 **   By checking the value of id it is possible to identify, which option
 **   has been successfully identified, or if an error occurred, or if
 **   the end-of-options mark "--" has been encountered. The arg parameter
 **   contains the argument passed along with the option, or the empty
 **   string if the option did not require an argument (or an optional
 **   argument was not provided), or the pertaining element itself in
 **   case an error occurred or the end-of-options marker was hit.
 **
 **   If the value returned by cb() is different from zero, option
 **   processing stops immediately, and the getopts() function itself
 **   returns this value unaltered.
 **
 ** RETURN VALUE
 **   The getopts() function always returns the value it received from
 **   its most recent call to cb().
 **
 ** NOTES
 **   * The getopts() function never modifies argv in any way, i.e. it
 **     never alters or reorders the pointers in argv, nor does it
 **     modify any of the pointed to strings.
 **
 **   * In contrast to e.g. getopt(3), the getopts() function does not
 **     maintain any persistent internal or external state, thus does
 **     not pollute global name space.
 **
 **   * The usemsg field of an object of type getopts_t is never touched
 **     or used by the getopts() function. It is purely provided as a
 **     convenient way to keep textual option descriptions close to their
 **     respective definitions.
 **
 **   * If one is interested in scanning argument vector elements that
 **     may follow an end-of-options mark ("--"), one may want to keep
 **     track of the value of idx passed to cb() in a suitable way.
 **
 **   * Short options may be grouped together on the command line, but
 **     only the last in line may have an optional or required argument
 **     associated with it.
 **
 **   * Arguments that belong to a short option can either immediately
 **     follow that option without separator, or as the successive
 **     element in argv. Exception: If the argument starts with a '-'
 **     (e.g. a negative number), it must immediately follow the option,
 **     as it would otherwise be interpreted as a short option itself.
 **     Long options can have their arguments attached with an interposed
 **     '=', or as the next element in argv, or both. For example, given
 **     the following option definition fragment
 **
 ** 	  ...
 ** 	  { 1, OPTARG_REQUIRED, 'x', "xxx", "" },
 ** 	  ...
 **
 **     any of the following alternative program invocations will produce
 **     the exact same results:
 **
 ** 	progname -xARG
 ** 	progname -x ARG
 ** 	progname --xxx ARG
 ** 	progname --xxx=ARG
 ** 	progname --xxx= ARG
 **
 ** EXAMPLE
 **  The following is a simple example program to illustrate the most
 **  important concepts in using the getopts() function:
 **
 ** 	#include <stdio.h>
 ** 	#include <getopts.h>
 **
 ** 	static int optind = 1;
 **
 ** 	static getopts_t opts[] = {
 ** 	    { 1, OPTARG_REQUIRED, 'x',  "xxx", "" },
 ** 	    { 2,     OPTARG_NONE,   0, "abcd", "" },
 ** 	    { 3, OPTARG_OPTIONAL, 'c',     "", "" },
 ** 	    { 0 }
 ** 	};
 **
 ** 	static int opt_cb( int idx, int id, const char *arg )
 ** 	{
 ** 	    optind = idx;
 ** 	    printf( "next idx=%d, ", idx );
 ** 	    switch ( id )
 ** 	    {
 ** 	    case OPTERR_NON_OPT:
 ** 	        printf( "non-option %s\n", arg );
 ** 	        break;
 ** 	    case OPTERR_LAST_OPT:
 ** 	        printf( "end-of-options\n" );
 ** 	        break;
 ** 	    case OPTERR_UNKNOWN_OPT:
 ** 	        printf( "unrecognized option %s\n", arg );
 ** 	        break;
 ** 	    case OPTERR_MISSING_ARG:
 ** 	        printf( "missing arg ument for option %s\n", arg );
 ** 	        break;
 ** 	    case 1:
 ** 	    case 2:
 ** 	    case 3:
 ** 	        printf( "opt ID: %d%s%s\n", id,
 ** 	            arg&&*arg?", optarg=":"", arg&&*arg?arg:"" );
 ** 	        break;
 ** 	    default:
 ** 	        break;  // unreachable
 ** 	    }
 ** 	    return 0;
 ** 	}
 **
 ** 	int main( int argc, char *argv[] )
 ** 	{
 ** 	    int r = getopts( argc, argv, opts, opt_cb );
 ** 	    printf( "getopts() returned %d\n", r );
 ** 	    // Print remaining arguments, if any:
 ** 	    for ( int i = optind; i < argc; ++i )
 ** 	        printf( "cmdline arg #%d: %s\n", i, argv[i] );
 ** 	    return 0;
 ** 	}
 **
 ** SEE ALSO
 **   getopts_h(3)
 **
 */

#define IS_NONOPT(X)    ((X) && *(X)!='-')
#define NEXT_ARG()      (idx>=argc ? NULL : argv[idx++])
#define CHR2STR(C)      ((char[]){(C),0})
#define MAX(a,b)        ((a)>(b)?(a):(b))

int getopts( int argc, char *argv[], getopts_t *odef, int (*cb)(int,int,const char *) )
{
    int i, idx = 1, r = 0;
    const char *opt, *optarg;

    while ( 0 == r && NULL != ( opt = NEXT_ARG() ) )
    {
        if ( '-' == *opt && '\0' != *(opt+1) )
        {
            if ( '-' == *++opt )
            {
                if ( !*++opt )
                {   /* "--" ends option processing */
                    r = cb( idx, OPTERR_LAST_OPT, "" );
                    break;
                }
                /* long option */
                size_t len;
                optarg = strchr( opt, '=' );
                len = optarg ? (size_t)(optarg++ - opt) : strlen( opt );
                for ( i = 0; 0 < odef[i].optid; ++i )
                    if ( odef[i].lopt && !strncmp( opt, odef[i].lopt, MAX(len,strlen(odef[i].lopt)) ) )
                        break;
                if ( 0 == odef[i].optid )
                {
                    r = cb( idx, OPTERR_UNKNOWN_OPT, opt );
                    continue;
                }
                if ( OPTARG_REQUIRED == odef[i].arg )
                {
                    if ( ( !optarg || !*optarg ) && IS_NONOPT(argv[idx]) )
                        optarg = NEXT_ARG();
                    if ( !optarg || !*optarg )
                        r = cb( idx, OPTERR_MISSING_ARG, opt );
                    else
                        r = cb( idx, odef[i].optid, optarg );
                    continue;
                }
                if ( OPTARG_OPTIONAL == odef[i].arg )
                {
                    if ( (!optarg || !*optarg) && IS_NONOPT(argv[idx]) )
                        optarg = NEXT_ARG();
                }
                r = cb( idx, odef[i].optid, optarg ? optarg : "" );
                continue;
            }
            /* short option(s) */
            for ( const char *o = opt; !r && *o; ++o )
            {
                for ( i = 0; 0 < odef[i].optid && *o != odef[i].sopt; ++i )
                    ;
                if ( 0 == odef[i].optid )
                {
                    r = cb( idx, OPTERR_UNKNOWN_OPT, CHR2STR(*o) );
                    continue;
                }
                optarg = NULL;
                if ( OPTARG_REQUIRED == odef[i].arg )
                {
                    optarg = o + 1;
                    if ( !*optarg && IS_NONOPT(argv[idx]) )
                        optarg = NEXT_ARG();
                    if ( !optarg || !*optarg )
                        r = cb( idx, OPTERR_MISSING_ARG, CHR2STR(*o) );
                    else
                        r = cb( idx, odef[i].optid, optarg );
                    break;
                }
                if ( OPTARG_OPTIONAL == odef[i].arg )
                {
                    optarg = o + 1;
                    if ( !*optarg && IS_NONOPT(argv[idx]) )
                        optarg = NEXT_ARG();
                    r = cb( idx, odef[i].optid, optarg ? optarg : "" );
                    break;
                }
                r = cb( idx, odef[i].optid, "" );
            }
        }
        else
        {   /* non-option argument */
            r = cb( idx, OPTERR_NON_OPT, opt );
        }
    }
    return r;
}

/* EOF */
