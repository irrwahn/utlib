/*
 * ntime.c
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

#if defined(_POSIX_C_SOURCE) && (_POSIX_C_SOURCE < 199309L)
    #undef _POSIX_C_SOURCE
#endif
#define _POSIX_C_SOURCE 199309L

#include <ntime.h>

#if defined(__unix__) || defined(__unix) || defined(unix) || \
    (defined(__APPLE__) && defined(__MACH__))
    #include <unistd.h>
#endif

#if !defined(_POSIX_TIMERS) || !(_POSIX_TIMERS > 0) || \
    !defined(CLOCK_REALTIME)
    #error "POSIX timers required but not available!"
#endif

/*
 Select the monotonic clock source best suited for relative timing
 measurements.

 Note 1:
 CLOCK_MONOTONIC_RAW provided by modern Linux based systems seems a
 better choice over CLOCK_MONOTONIC, since it is guaranteed to not
 be influenced by NTP slew, thus running at a constant speed. The
 drawback is it might run slightly slower or faster than the adjusted
 monotonic clock.

 Note 2:
 Untested on AIX, BSD, Solaris.
*/
#if defined(CLOCK_MONOTONIC_PRECISE)
    /* BSD */
    #define NT_MONOCLOCK    CLOCK_MONOTONIC_PRECISE
#elif defined(CLOCK_MONOTONIC_RAW)
    /* Linux */
    #define NT_MONOCLOCK    CLOCK_MONOTONIC_RAW
#elif defined(CLOCK_HIGHRES)
    /* Solaris */
    #define NT_MONOCLOCK    CLOCK_HIGHRES
#elif defined(CLOCK_MONOTONIC)
    /* AIX, BSD, Linux, Solaris, and generally POSIX compliant */
    #define NT_MONOCLOCK    CLOCK_MONOTONIC
#else
    #error "No monotonic clock source available!"
#endif

#define NT_REALCLOCK    CLOCK_REALTIME


/*
 **** ntime_to_us 3
 **
 ** NAME
 **   ntime_to_us, ntime_from_us, ntime_to_ms, ntime_from_ms, ntime_to_s, ntime_from_s, ntime_to_time_t, ntime_from_time_t, ntime_to_timeval, ntime_from_timeval, ntime_to_timespec, ntime_from_timespec - convert time values between ntime_t and other representations
 **
 ** SYNOPSIS
 **   #include <ntime.h>
 **
 **   ntime_to_us(ns)
 **   ntime_from_us(us)
 **
 **   ntime_to_ms(ns)
 **   ntime_from_ms(ms)
 **
 **   ntime_to_s(ns)
 **   ntime_from_s(s)
 **
 **   ntime_to_time_t(ns)
 **   ntime_from_time_t(s)
 **
 **   extern struct timeval *ntime_to_timeval(ntime_t t, struct timeval *tv);
 **   extern ntime_t ntime_from_timeval(struct timeval *tv);
 **
 **   extern struct timespec *ntime_to_timespec(ntime_t t, struct timespec *ts);
 **   extern ntime_t ntime_from_timespec(struct timespec *ts);
 **
 ** DESCRIPTION
 **   The ntime_to_us(), ntime_from_us(), ntime_to_ms(), ntime_from_ms(),
 **   ntime_to_s(), ntime_from_s(), ntime_to_time_t() and ntime_from_time_t()
 **   macros convert nanoseconds to and from other useful scalar time
 **   representations of various precision.
 **
 **   The ntime_to_timeval(), ntime_from_timeval(), ntime_to_timespec(),
 **   ntime_from_timespec() functions convert nanoseconds to and from
 **   structural time types.
 **
 ** RETURN VALUE
 **   The ntime_to_*() and ntime_from_*() functions return the result
 **   of the respective conversion.
 **
 ** NOTES
 **   No rounding is performed during conversion is applied, thus the
 **   results should be comparable with those returned by time() and
 **   gettimeofday().
 **
 **   While the simple (scalar) conversions are implemented as reasonably
 **   safe to use macros, the functions involving conversions to or from
 **   structural types are implemented as genuine functions to avoid
 **   unwanted side effects due to macros evaluating their argument more
 **   than once.
 **
 ** BUGS
 **   The ntime_to_timeval(), ntime_from_timeval(), ntime_to_timespec()
 **   and ntime_from_timespec() functions are not safeguarded against
 **   NULL pointer values passed for tv or ts, respectively; this is a
 **   deliberate design decision.
 **
 ** SEE ALSO
 **   ntime_h(3), mktime(3), gmtime(3), localtime(3)
 **
 */

struct timeval *ntime_to_timeval( ntime_t t, struct timeval *tv )
{
    tv->tv_sec = t / NT_NS_PER_S;
    tv->tv_usec = t % NT_NS_PER_S / NT_NS_PER_US;
    return tv;
}

ntime_t ntime_from_timeval( struct timeval *tv )
{
    return (ntime_t)tv->tv_sec * NT_NS_PER_S + tv->tv_usec * NT_NS_PER_US;
}

struct timespec *ntime_to_timespec( ntime_t t, struct timespec *ts )
{
    ts->tv_sec = t / NT_NS_PER_S;
    ts->tv_nsec = t % NT_NS_PER_S;
    return ts;
}

ntime_t ntime_from_timespec( struct timespec *ts )
{
    return (ntime_t)ts->tv_sec * NT_NS_PER_S + ts->tv_nsec;
}


/*
 **** ntime_get 3
 **
 ** NAME
 **   ntime_get, ntime_res, nclock_get, nclock_res - get time and resolution in nanoseconds
 **
 ** SYNOPSIS
 **   #include <ntime.h>
 **
 **   ntime_t ntime_get(void);
 **   ntime_t ntime_res(void);
 **
 **   ntime_t nclock_get(void);
 **   ntime_t nclock_res(void);
 **
 ** DESCRIPTION
 **   The ntime_get() function samples the system's best approximation
 **   of current ("wall-clock") calendar time and returns it as the
 **   number of nanoseconds elapsed since the UNIX epoch (00:00:00 1
 **   January 1970), represented as a signed integer with a width of at
 **   least 64 bit.
 **   The returned time values may experience discontinuous jumps due to
 **   time adjustments, e.g. daylight saving time switching.
 **
 **   The ntime_res() function returns the resolution, in nanoseconds,
 **   of the system clock source queried by the ntime_get() function.
 **
 **   The nclock_get() function samples the system's best approximation
 **   of monotonic clock time and returns it as nanoseconds elapsed since
 **   an arbitrary epoch (often boot time), represented as a signed
 **   integer with a width of at least 64 bit. The returned time values
 **   are guaranteed to _not_ be subject to sudden coarse time adjustments
 **   that would otherwise lead to discontinuities, they _may_ however
 **   be affected by NTP slew, depending on system specifics.
 **
 **   The nclock_res() function returns the resolution, in nanoseconds,
 **   of the system clock source queried by the nclock_get() function.
 **
 ** RETURN VALUE
 **   The ntime_get(), ntime_res(), nclock_get() and nclock_res()
 **   functions return -1 on error, or a positive number on success.
 **
 ** NOTES
 **   The ntime_get() function is intended to assist in absolute calendar
 **   time calculations, and nclock_get() for relative timing purposes.
 **
 **   The getter functions basically wrap around the POSIX clock_get*()
 **   interface, which must be available on the target system. Depending
 **   on system and implementation it may be necessary to link with librt.
 **
 ** BUGS
 **   For ntime_get(), the value returned on error is within the range
 **   of legitimate return values. This should not be a big deal however,
 **   at least until time travel really is a thing or the system clock
 **   is deliberately tampered with, e.g. set to a date before the epoch.
 **
 **   If the internal representation of ntime_t is no wider than 64 bit,
 **   the values returned by ntime_get() will silently wrap in 2262.
 **
 ** SEE ALSO
 **   ntime_h(3), time(2), gettimeofday(2), clock_gettime(2)
 **
 */

ntime_t ntime_get( void )
{
    struct timespec ts;
    if ( 0 != clock_gettime( NT_REALCLOCK, &ts ) )
        return -1;
    return ntime_from_timespec( &ts );
}

ntime_t ntime_res( void )
{
    struct timespec ts;
    if ( 0 != clock_getres( NT_REALCLOCK, &ts ) )
        return -1;
    return ntime_from_timespec( &ts );
}


ntime_t nclock_get( void )
{
    struct timespec ts;
    if ( 0 != clock_gettime( NT_MONOCLOCK, &ts ) )
        return -1;
    return ntime_from_timespec( &ts );
}

ntime_t nclock_res( void )
{
    struct timespec ts;
    if ( 0 != clock_getres( NT_MONOCLOCK, &ts ) )
        return -1;
    return ntime_from_timespec( &ts );
}


/* EOF */
