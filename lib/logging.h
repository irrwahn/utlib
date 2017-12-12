/*
 * logging.h
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

/*
 **** logging_h 3
 **
 ** NAME
 **   logging - log a formatted message to file and/or syslog
 **
 ** SYNOPSIS
 **   #include <logging.h>
 **
 ** DESCRIPTION
 **   MACROS
 **     Of the following symbolic constants, zero or more may be OR'ed
 **     together to form the mode option of log_open():
 **     LOG_TO_FILE     Log to a user supplied file pointer.
 **     LOG_TO_SYSLOG   Log to system log.
 **
 **     The following symbolic constants, borrowed from syslog.h(7),
 **     are to be used as the lvl argument of log_open() and log_fopen(),
 **     and the priority option of log_printf():
 **
 **     LOG_EMERG      system is unusable
 **     LOG_ALERT      action must be taken immediately
 **     LOG_CRIT       critical conditions
 **     LOG_ERR        error conditions
 **     LOG_WARNING    warning conditions
 **     LOG_NOTICE     normal, but significant, condition
 **     LOG_INFO       informational message
 **     LOG_DEBUG      debug-level message
 **
 **     A function-like macro to include source file position information
 **     in the logged message:
 **
 **     log_xprintf(PRI, ...)
 **
 **   FUNCTIONS
 **     log_open()     initialize the logging system
 **     log_fopen()    initialize the logging system
 **     log_close()    close the logging system
 **
 **     log_printf()   log a formatted message
 **
 **     log_vprintf()  log a formatted message from a va_list
 **
 ** NOTES
 **   If the logging module was built without syslog(3) support, only
 **   the file logging mode is available and LOG_TO_SYSLOG is silently
 **   ignored.
 **
 ** SEE ALSO
 **   log_printf(3), syslog(3), syslog.h(7)
 **
 */

#ifndef LOGGING_H_INCLUDED
#define LOGGING_H_INCLUDED

#ifdef _cplusplus
extern "C" {
#endif

#include <stdarg.h>
#include <stdio.h>

/* Supported logging modes. */
#define LOG_TO_FILE     1
#define LOG_TO_SYSLOG   2

/* Priority levels, borrowed from syslog.h. */
#ifndef LOG_EMERG
    #define LOG_EMERG   0     /* system is unusable */
#endif
#ifndef LOG_ALERT
    #define LOG_ALERT   1     /* action must be taken immediately */
#endif
#ifndef LOG_CRIT
    #define LOG_CRIT    2     /* critical conditions */
#endif
#ifndef LOG_ERR
    #define LOG_ERR     3     /* error conditions */
#endif
#ifndef LOG_WARNING
    #define LOG_WARNING 4     /* warning conditions */
#endif
#ifndef LOG_NOTICE
    #define LOG_NOTICE  5     /* normal, but significant condition */
#endif
#ifndef LOG_INFO
    #define LOG_INFO    6     /* informational message */
#endif
#ifndef LOG_DEBUG
    #define LOG_DEBUG   7     /* debug-level message */
#endif


void log_open( int lvl, unsigned mode, FILE *fp, const char *id, int option, int facility );
void log_fopen( int lvl, unsigned mode, const char *filename, const char *id, int option, int facility );
void log_close( void );

void log_printf( int pri, const char *fmt, ... );

void log_vprintf( int pri, const char *fmt, va_list arglist );

/* NOTE: Never call log_xprintf_() directly, always use the log_xprintf() macro instead! */
void log_xprintf_( const char *file, const char *func, int line, int pri, const char *fmt, ... );
#define log_xprintf(PRI, ...)    log_xprintf_( __FILE__, __func__, __LINE__, (PRI), __VA_ARGS__ )


#ifdef _cplusplus
} /* extern "C" */
#endif

#endif /* ndef LOGGING_H_INCLUDED */

/* EOF */
