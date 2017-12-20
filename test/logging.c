
#include <errno.h>
#include <string.h>

#include <logging.h>

#include "testsupp.h"

/*
 * If someone can come up with a way to test syslog logging, without
 * actually messing up the system log and yet still be able to check
 * for the message to hit the log: Please let me know!
 */

REGISTER(logging_test)
{
    int err = 0;
    FILE *fp;
    char buf[500];

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
        if ( !strstr( buf, "Test: Success" )
            || !strstr( buf, ": (logging.c:logging_test:" ) )
            ++err;
    }
    else
        ++err;

#define THIS_FILE __FILE__
#line 775 "fake.file"
    log_close();
    log_open( LOG_DEBUG, LOG_TO_FILE, stderr, "log_to_stderr", 0, 0 );
    log_xprintf( LOG_DEBUG, "%m\n" );
#line 45 THIS_FILE

    if ( !err )
        PASS( "logging ok" );
    else
        FAIL( "logging failed, %d errors", err );
    return err;
}

/* EOF */
