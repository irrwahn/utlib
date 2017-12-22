/*
 * utf8_encode_ex01.c
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
#include <stdlib.h>
#include <string.h>

#include <utf8_encode.h>
#include <logging.h>

typedef
    struct {
        FILE *ifp;
        FILE *ofp;
    } fpset_t;

/* Read single 32-bit unsigned value in host byte order from input stream. */
uint32_t get( void *usr ) {
    uint32_t c;
    if ( sizeof c != fread( &c, 1, sizeof c, ((fpset_t *)usr)->ifp ) )
        return EOF;
    return c;
}

/* Write result of single conversion to UTF-8 to output stream. */
int put( uint8_t *b, int l, void *usr ) {
    for ( int i = 0; i < l; ++i )
        if ( EOF == fputc( b[i], ((fpset_t *)usr)->ofp ) )
            return EOF;
    return 0;
}

int main( int argc, char *argv[] )
{
    size_t e, n;
    fpset_t fpset = {
        .ifp = stdin,
        .ofp = stdout,
    };
    int res = EXIT_SUCCESS;

    log_open( LOG_INFO, LOG_TO_FILE, stderr, argv[0], 0, 0 );

    if ( argc > 1 && 0 != strcmp( argv[1], "-" ) )
    {
        if ( NULL == ( fpset.ifp = fopen( argv[1], "rb" ) ) )
        {
            log_printf( LOG_ERR, "fopen %s: %m\n", argv[1] );
            fprintf( stderr, "Usage: %s [file|-]\n", argv[0] );
            exit( EXIT_FAILURE );
        }
    }

    /* Read UTF-32 in host byte order from input and write UTF-8 to stdout. */
    n = utf8_stream_encode( get, put, &fpset, &e );
    if ( e )
    {
        log_printf( LOG_ERR, "utf8_stream_encode(): %d error%s\n", e, e>1?"s":"" );
        res = EXIT_FAILURE;
    }
    log_printf( LOG_INFO, "utf8_stream_encode(): %d conversions\n", n );
    exit( res );
}

/* EOF */
