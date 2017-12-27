/*
 * logging_test.c
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

#include <errno.h>
#include <string.h>

#include <logging.h>

#include "../testsupp.h"

/*
 * If someone can come up with a way to test syslog logging, without
 * actually messing up the system log and yet still be able to check
 * for the message to hit the log: Please let me know!
 */

REGISTER( logging_test )
{
    int err = 0;
    FILE *fp;
    char buf[500];

    errno = 0;
    log_printf( LOG_DEBUG, "log_%s %m\n", "default" );

    while ( NULL == ( fp = tmpfile() ) )
        ;
    errno = 0;
    log_close();
    log_open( LOG_DEBUG, LOG_TO_FILE, fp, "log_to_file", 0, 0 );
    log_xprintf( LOG_INFO, "Test: %m\n" );
    rewind( fp );
    if ( NULL != fgets( buf, sizeof buf, fp ) )
    {
        fprintf(stderr,"%s", buf);
        if ( !strstr( buf, "Test:" )
            || !strstr( buf, ": (" __FILE__ ":logging_test:" ) )
            ++err;
    }
    else
        ++err;

#line 775 "fake.file"
    log_close();
    log_open( LOG_DEBUG, LOG_TO_FILE, stderr, "log_to_stderr", 0, 0 );
    errno = 0;
    log_xprintf( LOG_DEBUG, "%m\n" );
#line 80 "extra/logging_test.c"

    if ( !err )
        PASS( "logging ok" );
    else
        FAIL( "logging failed, %d errors", err );
    return err;
}

/* EOF */
