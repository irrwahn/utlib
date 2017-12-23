/*
 * logging.c
 *
 * Copyright 2013-2017 Urban Wallasch <irrwahn35@freenet.de>
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

#define _POSIX_C_SOURCE 200809L     /* localtime_r, strerror_r */

#if !defined(WITHOUT_SYSLOG) && defined(WITHOUT_OWN_VSYSLOG)
    #define _DEFAULT_SOURCE         /* vsyslog */
#endif

#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <errno.h>
#include <time.h>

#include <unistd.h>
#include <sys/time.h>

#ifndef WITHOUT_SYSLOG
    #include <syslog.h>
#endif

#ifdef WITH_PTHREAD
    #include <pthread.h>
    static pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;
    #define LOCK_LOG()      pthread_mutex_lock( &mtx )
    #define UNLOCK_LOG()    pthread_mutex_unlock( &mtx )
#else
    #define LOCK_LOG()
    #define UNLOCK_LOG()
#endif /* WITH_PTHREAD */

#include <ntime.h>
#include <logging.h>

#define NULLSTR "(null)"

static volatile struct {
    int init;
    int level;
    unsigned mode;
    const char *ident;
    FILE *file;
    int file_is_our;
} cfg = {
    0,
    0,
    0,
    NULL,
    NULL,
    0
};

/*
 * Module static functions.
 */

#ifndef WITHOUT_SYSLOG
#ifndef WITHOUT_OWN_VSYSLOG
    static void vsyslog_( int pri, const char *fmt, va_list arglist )
    {
        va_list argcopy;
        va_copy( argcopy, arglist );
        int n = vsnprintf( NULL, 0, fmt, argcopy );
        va_end( argcopy );
        char buf[n];
        vsnprintf( buf, sizeof buf, fmt, arglist );
        syslog( pri, "%s", buf );
        return;
    }
#else
    #define vsyslog_(...) vsyslog(__VA_ARGS__)
#endif /* WITHOUT_OWN_SYSLOG */
#endif /* WITHOUT_SYSLOG */

static void log_close_( void )
{
    if ( !cfg.init )
        return;
    cfg.init = 0;
    if ( (cfg.mode & LOG_TO_FILE) && cfg.file && cfg.file_is_our )
        fclose( cfg.file );
#ifndef WITHOUT_SYSLOG
    if ( cfg.mode & LOG_TO_SYSLOG )
        closelog();
#endif
    return;
}

static void log_open_( int lvl, unsigned mode, FILE *fp, const char *id, int option, int facility )
{
    if ( !cfg.init )
        log_close_();
    cfg.level = lvl;
    cfg.mode  = mode;
    cfg.ident = id ? id : NULLSTR;
    cfg.file  = fp ? fp : stderr;
#ifndef WITHOUT_SYSLOG
    if ( mode & LOG_TO_SYSLOG )
        openlog( id, option, facility );
#endif
    cfg.init = 1;
    return;
}


/*
 * Exported functions.
 */

/*
 **** log_printf 3
 **
 ** NAME
 **   log_open, log_fopen, log_printf, log_vprintf, log_xprintf - write formatted messages to a file and/or the system logger
 **
 ** SYNOPSIS
 **   #include <logging.h>
 **
 **   void log_open(int lvl, unsigned mode, FILE *fp, const char *id, int option, int facility);
 **   void log_fopen(int lvl, unsigned mode, const char *filename, const char *id, int option, int facility);
 **   void log_close(void);
 **
 **   void log_printf(int pri, const char *fmt, ...);
 **   void log_vprintf(int pri, const char *fmt, va_list arglist);
 **   log_xprintf(pri, ...)
 **
 ** DESCRIPTION
 **   The log_open() function initializes the logging system. Its use
 **   is optional; if omitted it is called implicitly with default
 **   values (LOG_DEBUG, LOG_TO_FILE, stderr, NULL, 0, 0) the first
 **   time any of the log_printf(), log_vprintf() or log_xprintf()
 **   functions is called.
 **   The lvl argument specifies the cutoff priority up to which a
 **   message is logged at all.
 **   The mode argument selects between logging to a file, or the system
 **   logger, or both, or none at all.
 **   If LOG_TO_FILE is included in mode, fp specifies the file pointer
 **   to write log messages to, otherwise it is ignored.
 **   If fp equals NULL, output is written to stderr instead.
 **   The string pointed to by id is prepended to every message when
 **   logging to a file, and passed on to open_log(3) when logging to
 **   the system log.
 **   If LOG_TO_SYSLOG is included in mode, the option and facility
 **   arguments are passed on to openlog(3), otherwise they are ignored.
 **   See openlog(3) for their respective meaning and valid values.
 **   Both options can be set to 0, if logging to syslog is not enabled,
 **   or logging was built with the -DWITHOUT_SYSLOG compile time setting.
 **
 **   The log_fopen() function is similar to log_open(), but takes a
 **   filename string instead of a file pointer. Any failure to open the
 **   specified file for appending is silently ignored and any
 **   subsequent output is written to stderr instead.
 **
 **   The log_close() function closes the logging system. Its use is
 **   optional, as it is implicitly called first thing whenever log_open()
 **   or log_fopen() is called.
 **
 **   The log_printf() function writes a formatted message with priority
 **   pri to the logging system. It provides an interface similar to
 **   that of printf(3).
 **   Supported are all format specifiers recognized by printf(3), and
 **   additionally the "%m" sequence as recognized by syslog(3), which
 **   consumes no argument but instead is replaced by a textual error
 **   description equivalent to what can be obtained by a call to
 **   strerror(3).
 **
 **   The log_vprintf() function is similar to log_printf(), but expects
 **   a va_list variable argument list arglist in place of an unspecified
 **   number of actual arguments.
 **
 **   The log_xprintf() macro behaves similar to the log_printf() function,
 **   but additionally records the exact current source file position
 **   and adds a "(FILE_NAME:FUNCTION_NAME:LINE_NUMBER) " prefix to the
 **   formatted message. It is guaranteed to evaluate it's arguments
 **   only once.
 **
 ** RETURN VALUE
 **   These functions do not return any values.
 **
 ** NOTES
 **   The log_open() and log_fopen() functions can safely be called
 **   repeatedly to reconfigure the logging system on the fly, without
 **   leaking resources.
 **
 **   The log_vprintf() function does not call the va_end macro,
 **   consequently the value of arglist is undefined after the call.
 **
 **   The log_printf() and log_vprintf() functions as well as the
 **   log_xprintf() macro support the "%m" format specifier even in file
 **   mode, via an emulation. This comes at the cost of increased
 **   processing effort and memory usage. In cases where logging
 **   performance is absolutely paramount, it is recommended to avoid
 **   using the "%m" format specifier in LOG_TO_FILE mode.
 **
 **   In LOG_TO_FILE mode the log lines follow the general pattern
 **   "TS ID[PID]:PRI: MSG", where the various fields stand for:
 **     TS   local time in ISO8601 format: yyyy-mm-ddTHH:MM:SS.uuuuuu±ZZ:zz
 **     ID   source identifier as set via log_open()
 **     PID  process ID
 **     PRI  priority
 **     MSG  formatted message
 **
 ** BUGS
 **   When logging to a file, a terminating newline is not automatically
 **   appended to each message, but must be explicitly included in the
 **   message itself.
 **
 **   When simultaneously logging to the system logger and a file, the
 **   two different timestamps may not match, as they may be generated
 **   at slightly different times, or even obtained by different means.
 **
 **   When built without -DWITH_PTHREAD, logging is not thread safe and
 **   log file corruption and other problems may arise when logging from
 **   multiple threads simultaneously.
 **
 **   The general interface follows the syslog(3) pattern, which makes
 **   decent error handling nearly impossible.
 **
 ** SEE ALSO
 **   logging_h(3), syslog(3), printf(3)
 **
 */
void log_open( int lvl, unsigned mode, FILE *fp, const char *id, int option, int facility )
{
    LOCK_LOG();
    log_open_( lvl, mode, fp, id, option, facility );
    cfg.file_is_our = 0;
    UNLOCK_LOG();
}

void log_fopen( int lvl, unsigned mode, const char *filename, const char *id, int option, int facility )
{
    LOCK_LOG();
    FILE *fp = fopen( filename, "a" );
    log_open_( lvl, mode, fp, id, option, facility );
    cfg.file_is_our = 1;
    UNLOCK_LOG();
}

void log_close( void )
{
    LOCK_LOG();
    log_close_();
    UNLOCK_LOG();
}

void log_printf( int pri, const char *fmt, ... )
{
    va_list arglist;
    va_start( arglist, fmt );
    log_vprintf( pri, fmt, arglist );
    va_end( arglist );
    return;
}

void log_vprintf( int pri, const char *fmt, va_list arglist )
{
    int eno = errno;

    LOCK_LOG();
    if ( !cfg.init )
        log_open_( LOG_DEBUG, LOG_TO_FILE, stderr, NULL, 0, 0 );
    if ( pri <= cfg.level )
    {
#ifndef WITHOUT_SYSLOG
        if ( cfg.mode & LOG_TO_SYSLOG )
        {
            va_list argcopy;
            va_copy( argcopy, arglist );
            errno = eno;
            vsyslog_( pri, fmt, argcopy );
            va_end( argcopy );
        }
#endif
        if ( cfg.mode & LOG_TO_FILE )
        {
            /* Write time, ident, pid and priority stamps. */
            char tbuf[32];
            struct tm ts;
            struct timeval tv;
            ntime_to_timeval( ntime_get(), &tv );
            localtime_r( &tv.tv_sec, &ts );
            strftime( tbuf, sizeof tbuf, "%FT%T", &ts );
            fprintf( cfg.file, "%s.%06d", tbuf, (int)tv.tv_usec );
            strftime( tbuf, sizeof tbuf, "%z", &ts );
            fprintf( cfg.file, "%.3s:%s %s[%u]:%d: ", tbuf, tbuf + 3,
                     cfg.ident, (unsigned)getpid(), pri );

            /* Process any %m sequences embedded in format string. */
            const char *f = fmt;
            const char *m = strstr( f, "%m" );
            if ( m )
            {
                /* The POSIX strerror_r() specification does not provide
                   any means to determine the maximum length of the
                   resulting string beforehand. The GNU C Library however
                   uses a buffer of 1024 characters internally, thus we
                   simply take that as a suitable buffer size and hope
                   for the best.  N.B.: We expect the XSI-compliant
                   version of strerror_r()! */
                char estr[1024];
                if ( strerror_r( eno, estr, sizeof estr ) )
                    snprintf( estr, sizeof estr, "Error %d occurred", eno );
                /* Incrementally write the formatted message, handle
                   all %m sequences in format string. */
                do
                {
                    /* Keep %%m, do not replace! */
                    int have_ppm;
                    have_ppm = ( m > f && m[-1] == '%' );
                    if ( have_ppm )
                        m += 2;
                    /* Write partial message using temporary format string. */
                    char xfmt[m-f+1];
                    strncpy( xfmt, f, m-f );
                    xfmt[m-f] = '\0';
                    vfprintf( cfg.file, xfmt, arglist );
                    /* Write replacement text only for a genuine %m! */
                    if ( !have_ppm )
                    {
                        fputs( estr, cfg.file );
                        m += 2;
                    }
                    /* Continue with next %m sequence, if any. */
                    f = m;
                    m = strstr( f, "%m" );
                }
                while ( m );
            }

            /* Write remainder of message - i.e. all of it, if no %m
               was present in the format string to begin with. */
            vfprintf( cfg.file, f, arglist );
            fflush( cfg.file );
        }
    }
    UNLOCK_LOG();
    return;
}

/* This function is intentionally left undocumented. It is exported
   exclusively as the expansion target of the log_xprintf() macro! */
void log_xprintf_( const char *file, const char *func, int line, int pri, const char *fmt, ... )
{
    int eno = errno;
    va_list arglist;
    int n = snprintf( NULL, 0, "(%s:%s:%d) %s", file, func, line, fmt );
    char xfmt[n+1];

    snprintf( xfmt, sizeof xfmt, "(%s:%s:%d) %s", file, func, line, fmt );
    errno = eno;
    va_start( arglist, fmt );
    log_vprintf( pri, xfmt, arglist );
    va_end( arglist );
    return;
}


/* EOF */
