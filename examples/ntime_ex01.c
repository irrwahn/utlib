/*
 * ntime_ex01.c
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

#include <stdio.h>
#include <ntime.h>

int main( void )
{
    ntime_t nt;
    time_t t;
    char buf[20];

    printf( "Wall clock time resolution is %"PRI_ntime" ns\n", ntime_res() );
    printf( "Wall clock time is %"PRI_ntime" ns\n", nt = ntime_get() );
    t = ntime_to_s( nt );
    strftime( buf, sizeof buf, "%F %T", localtime( &t ) );
    printf( "\t(%s.%06"PRI_ntime")\n\n", buf, (nt - ntime_from_s(t))/1000 );

    printf( "Monotonic clock resolution is %"PRI_ntime" ns\n", nclock_res() );
    printf( "Monotonic clock is %"PRI_ntime" ns\n", nt = nclock_get() );
    printf( "\t(%"PRI_ntime" mins)\n", ntime_to_s( nt ) / 60 );

    return 0;
}

/* EOF */
