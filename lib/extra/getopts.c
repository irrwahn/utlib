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

#define IS_NONOPT(X)    ((X) && *(X)!='-')
#define NEXT_ARG()      (idx>=argc ? NULL : argv[idx++])
#define CHR2STR(C)      ((char[]){(C),0})

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
 **   Its arguments argc and argv are the argument count and array as
 **   passed to the main() function on program invocation. An element
 **   of argv that starts with '-' (which is not immediately followed by
 **   another '-') is interpreted as a short option element.
 **   The characters of this element following the initial '-' are taken
 **   as option characters. A single '-' not followed by any other
 **   characters is interpreted as a non-option argument. Any character
 **   sequence starting with "--" is interpreted as a long option. An
 **   element consisting only of the sequence "--" not immediately
 **   followed by any other characters immediately stops any further
 **   option processing by the getopts() function.
 **
 **   The odef argument to the getopts() function is a pointer to the
 **   first element of an array of element of the structure type
 **   getopts_t, defined in <getopts.h> as
 **
 **     typedef
 **         struct getopts_t_struct {
 **             unsigned optid;
 **             unsigned arg;
 **             char sopt;
 **             const char *lopt;
 **             const char *usemsg;
 **         }
 **         getopts_t;
 **
 **   The respective meanings of the different fields are:
 **
 **   optid  a positive number to uniquely identify an entry; a value of zero indicates the last entry in the array
 **   arg  one of OPTARG_NONE, OPTARG_REQUIRED or OPTARG_OPTIONAL to indicate if the option takes an (possibly optional) argument
 **   sopt  a single character constant declaring a short option
 **   lopt  a character string declaring a long option, excluding the "--" prefix
 **   usemsg  a string that can be used to store a human readable textual description of the option
 **
 **   The cb argument is a pointer to a caller supplied function which
 **   is called once for every argument that is processed by the getopts()
 **   function. It shall have a prototype compatible to
 **
 **     int my_cb( int idx, int id, const char *arg );
 **
 **   @ToDO
 **
 ** RETURN VALUE
 **   @ToDO
 **
 ** NOTES
 **   * The usemsg field of an object of type getopts_t is never touched
 **     by the getopts() function.
 **
 **   @ToDO
 **
 ** BUGS
 **   @ToDO
 **
 ** EXAMPLE
 **   @ToDO
 **
 **		<TAB indented code>
 **
 ** SEE ALSO
 **   getopts_h(3)
 **
 */

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
                    r = cb( idx-1, OPTERR_SUCCESS, "--" );
                    break;
                }
                /* long option */
                size_t len;
                optarg = strchr( opt, '=' );
                len = optarg ? (size_t)(optarg++ - opt) : strlen( opt );
                for ( i = 0; 0 < odef[i].optid; ++i )
                    if ( odef[i].lopt && !strncmp( opt, odef[i].lopt, len ) )
                        break;
                if ( 0 == odef[i].optid )
                {
                    r = cb( idx-1, OPTERR_UNKNOWN_OPT, opt );
                    continue;
                }
                if ( OPTARG_REQUIRED == odef[i].arg )
                {
                    if ( ( !optarg || !*optarg ) && IS_NONOPT(argv[idx]) )
                        optarg = NEXT_ARG();
                    if ( !optarg || !*optarg )
                        r = cb( idx-1, OPTERR_MISSING_ARG, opt );
                    else
                        r = cb( idx-1, odef[i].optid, optarg );
                    continue;
                }
                if ( OPTARG_OPTIONAL == odef[i].arg )
                {
                    if ( (!optarg || !*optarg) && IS_NONOPT(argv[idx]) )
                        optarg = NEXT_ARG();
                }
                r = cb( idx-1, odef[i].optid, optarg ? optarg : "" );
                continue;
            }
            /* short option(s) */
            for ( const char *o = opt; !r && *o; ++o )
            {
                for ( i = 0; 0 < odef[i].optid && *o != odef[i].sopt; ++i )
                    ;
                if ( 0 == odef[i].optid )
                {
                    r = cb( idx-1, OPTERR_UNKNOWN_OPT, CHR2STR(*o) );
                    continue;
                }
                optarg = NULL;
                if ( OPTARG_REQUIRED == odef[i].arg )
                {
                    optarg = o + 1;
                    if ( !*optarg && IS_NONOPT(argv[idx]) )
                        optarg = NEXT_ARG();
                    if ( !optarg || !*optarg )
                        r = cb( idx-1, OPTERR_MISSING_ARG, CHR2STR(*o) );
                    else
                        r = cb( idx-1, odef[i].optid, optarg );
                    break;
                }
                if ( OPTARG_OPTIONAL == odef[i].arg )
                {
                    optarg = o + 1;
                    if ( !*optarg && IS_NONOPT(argv[idx]) )
                        optarg = NEXT_ARG();
                    r = cb( idx-1, odef[i].optid, optarg ? optarg : "" );
                    break;
                }
                r = cb( idx-1, odef[i].optid, "" );
            }
        }
        else
        {   /* non-option argument */
            r = cb( idx-1, 0, opt );
        }
    }
    return r;
}

/* EOF */
