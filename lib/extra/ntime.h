/*
 * ntime.h
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
 **** ntime_h 3
 **
 ** NAME
 **   ntime - time manipulation in nanosecond resolution
 **
 ** SYNOPSIS
 **   #include <ntime.h>
 **
 ** DESCRIPTION
 **   TYPES
 **     ntime_t  signed integer type with a width of at least 64 bit to hold time and clock values
 **
 **   MACROS
 **     PRI_ntime, SCA_ntime  inttypes.h-style format macros for use with printf() and scanf()
 **
 **     NT_NS_PER_S, NT_NS_PER_MS, NT_NS_PER_US, NT_US_PER_S, NT_US_PER_MS, NT_MS_PER_S  simple powers-of-thousand numerical constants
 **
 **     ntime_to_us(), ntime_from_us(), ntime_to_ms(), ntime_from_ms(), ntime_to_s(), ntime_from_s(), ntime_to_time_t(), ntime_from_time_t()  macros to convert nanosecond time to and from scalar time representations
 **
 **   FUNCTIONS
 **     ntime_to_timeval(), ntime_from_timeval(), ntime_to_timespec(), ntime_from_timespec()  functions to convert nanosecond time to and from structural time representations
 **
 **     ntime_get(), ntime_res(), nclock_get(), nclock_res()  get time, clock and resolution values
 **
 ** NOTES
 **   The conversions to and from different time representations are
 **   implemented as macros that never evaluate their argument more
 **   than once for simple (i.e. scalar) types, and as genuine functions
 **   for the structural time representations struct timeval and struct
 **   timespec, to avoid undesirable side effects.
 **
 ** SEE ALSO
 **   ntime_get(3), ntime_to_us(3)
 **
 */

#ifndef NTIME_H_INCLUDED
#define NTIME_H_INCLUDED

#ifdef _cplusplus
extern "C" {
#endif

#include <inttypes.h>
#include <time.h>
#include <sys/time.h>


#define PRI_ntime       PRIdLEAST64
#define SCA_ntime       SCAdLEAST64

#define NT_NS_PER_S     (1000*1000*1000)
#define NT_NS_PER_MS    (1000*1000)
#define NT_NS_PER_US    (1000)
#define NT_US_PER_S     (1000*1000)
#define NT_US_PER_MS    (1000)
#define NT_MS_PER_S     (1000)


typedef int_least64_t ntime_t;


#define ntime_to_us(ns)       ( (ns) / NT_NS_PER_US )
#define ntime_from_us(us)     ( (ntime_t)(us) * NT_NS_PER_US )

#define ntime_to_ms(ns)       ( (ns) / NT_NS_PER_MS )
#define ntime_from_ms(ms)     ( (ntime_t)(ms) * NT_NS_PER_MS )

#define ntime_to_s(ns)        ( (ns) / NT_NS_PER_S )
#define ntime_from_s(s)       ( (ntime_t)(s) * NT_NS_PER_S )

#define ntime_to_time_t(ns)   ( (time_t)ntime_to_s((ns)) )
#define ntime_from_time_t(s)  ( ntime_from_s((s)) )


extern struct timeval *ntime_to_timeval( ntime_t t, struct timeval *tv );
extern ntime_t ntime_from_timeval( struct timeval *tv );

extern struct timespec *ntime_to_timespec( ntime_t t, struct timespec *ts );
extern ntime_t ntime_from_timespec( struct timespec *ts );

extern ntime_t ntime_get( void );
extern ntime_t ntime_res( void );

extern ntime_t nclock_get( void );
extern ntime_t nclock_res( void );


#ifdef _cplusplus
} /* extern "C" { */
#endif

#endif  /* ndef NTIME_H_INCLUDED */

/* EOF */
