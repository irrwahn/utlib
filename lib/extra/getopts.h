/*
 * getopts.h
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
 **** getopts_h 3
 **
 ** NAME
 **   getopts - blah
 **
 ** SYNOPSIS
 **   #include <getopts.h>
 **
 ** DESCRIPTION
 **   TYPES
 **     getopts_t  structure type to configure the getopts() option parser
 **
 **   MACROS
 **     OPTARG_NONE, OPTARG_REQUIRED, OPTARG_OPTIONAL  constants to tell getopts() about option arguments
 **
 **     OPTERR_NON_OPT, OPTERR_LAST_OPT, OPTERR_UNKNOWN_OPT, OPTERR_MISSING_ARG  error codes used by getopts()
 **
 **   FUNCTIONS
 **     getopts()  function to parse argument vectors and extract option and non-option arguments
 **
 ** SEE ALSO
 **   getopts(3)
 **
 */

#ifndef GETOPTS_H_INCLUDED
#define GETOPTS_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

#define OPTARG_NONE         0
#define OPTARG_REQUIRED     1
#define OPTARG_OPTIONAL     2

#define OPTERR_NON_OPT      0
#define OPTERR_LAST_OPT    -1
#define OPTERR_UNKNOWN_OPT -2
#define OPTERR_MISSING_ARG -3

typedef
    struct getopts_t_struct {
        int optid;
        int arg;
        char sopt;
        const char *lopt;
        const char *usemsg;
    }
    getopts_t;

extern int getopts( int argc, char *argv[], getopts_t *odef, int (*cb)(int,int,const char *) );


#ifdef __cplusplus
} /* extern "C" { */
#endif

#endif  /* ndef GETOPTS_H_INCLUDED */

/* EOF */
