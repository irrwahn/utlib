/*
 * bendian.h
 *
 * Copyright 2016-2017 Urban Wallasch <irrwahn35@freenet.de>
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
 **** bendian_h 3
 **
 ** NAME
 **   bendian - convert between host and network byte order
 **
 ** SYNOPSIS
 **   #include <bendian.h>
 **
 ** DESCRIPTION
 **   MACROS
 **     HTOBE16(), HTOBE32(), HTOBE64(), HTOLE16(), HTOLE32(), HTOLE64(), BETOH16(), BETOH32(), BETOH64(), LETOH16(), LETOH32(), LETOH64(), NTOH16(), NTOH32(), NTOH64(), HTON16(), HTON32(), HTON64()  byte order conversion macros
 **
 **   FUNCTIONS
 **     letoh16(), letoh32(), letoh64(), htole16(), htole32(), htole64(), betoh16(), betoh32(), betoh64(), htobe16(), htobe32(), htobe64(), ntoh16(), ntoh32(), ntoh64(), hton16(), hton32(), hton64()  byte order conversion functions
 **
 ** SEE ALSO
 **   NTOH(3), ntoh(3)
 **
 */

/*
 **** NTOH 3
 **
 ** NAME
 **   HTOBE16, HTOBE32, HTOBE64, BETOH16, BETOH32, BETOH64, HTOLE16, HTOLE32, HTOLE64, LETOH16, LETOH32, LETOH64, HTON16, HTON32, HTON64, NTOH16, NTOH32, NTOH64 - byte order conversion macros
 **
 ** SYNOPSIS
 **   #include <bendian.h>
 **
 **   HTOBE16(V)
 **   HTOBE32(V)
 **   HTOBE64(V)
 **
 **   BETOH16(V)
 **   BETOH32(V)
 **   BETOH64(V)
 **
 **   HTOLE16(V)
 **   HTOLE32(V)
 **   HTOLE64(V)
 **
 **   LETOH16(V)
 **   LETOH32(V)
 **   LETOH64(V)
 **
 **   HTON16(V)
 **   HTON32(V)
 **   HTON64(V)
 **
 **   NTOH16(V)
 **   NTOH32(V)
 **   NTOH64(V)
 **
 ** DESCRIPTION
 **   All conversion macros accept, and evaluate to, unsigned integer
 **   values with a width as indicated by the bit count in the macro name.
 **
 **   The HTOBE16(), HTOBE32(), HTOBE64() macros convert their argument
 **   from host byte order to big endian byte order.
 **
 **   The BETOH16(), BETOH32(), BETOH64() macros convert their argument
 **   from big endian byte order to host byte order.
 **
 **   The HTOLE16(), HTOLE32(), HTOLE64() macros convert their argument
 **   from host byte order to little endian byte order.
 **
 **   The LETOH16(), LETOH32(), LETOH64() macros convert their argument
 **   from little endian byte order to host byte order.
 **
 **   The HTON16(), HTON32(), HTON64() macros convert their argument
 **   from host byte order to network byte order.
 **
 **   The NTOH16(), NTOH32(), NTOH64() macros convert their argument
 **   from network byte order to host byte order.
 **
 ** RETURN VALUE
 **   All macros evaluate to the result of the respective conversion.
 **
 ** NOTES
 **   The conversion macros may evaluate their argument more than once,
 **   which can cause undesirable side effects. A set of trivial wrapper
 **   functions is provided to avoid such effects, see ntoh(3).
 **
 ** SEE ALSO
 **   bendian_h(3), ntoh(3)
 **
 */

#ifndef BENDIAN_H_INCLUDED
#define BENDIAN_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

#include <inttypes.h>


/* Sigh. There's a POSIX standard for everything except the most useful
   things. The following is a cheap attempt at making the code at least
   somewhat portable.
*/
#if 0

    /* Use non-standard C library extensions.
       Reportedly breaks on Android, which defines __linux__
       but provides the OpenBSD API. Go figure!
    */
    #define _DEFAULT_SOURCE
    #if defined(__linux__)
        #include <endian.h>
    #elif defined(__FreeBSD__) || defined(__NetBSD__)
        #include <sys/endian.h>
    #elif defined(__OpenBSD__)
        #include <sys/types.h>
        #define be16toh(x) betoh16(x)
        #define be32toh(x) betoh32(x)
        #define be64toh(x) betoh64(x)
    #endif
    #define NTOH16(V)       be16toh((uint16_t)(V))
    #define NTOH32(V)       be32toh((uint32_t)(V))
    #define NTOH64(V)       be64toh((uint64_t)(V))
    #define HTON16(V)       htobe16((uint16_t)(V))
    #define HTON32(V)       htobe32((uint32_t)(V))
    #define HTON64(V)       htobe64((uint64_t)(V))

#else

    /* This version does not require __DEFAULT_SOURCE defined. */

    /* Prelude: "One Header To Find Them" */
    #if defined(__linux__)
        #include <endian.h>
    #elif defined(__FreeBSD__)
        #include <sys/endian.h>
    #elif defined(__NetBSD__)
        #include <sys/endian.h>
    #elif defined(__OpenBSD__)
        #include <sys/types.h>
    #elif defined(__APPLE__)
        #include <libkern/OSByteOrder.h>
    #else
        #error "Get a real OS, peasant!"
    #endif

    /* Intermezzo: "The Underscore Sarabande" */
    #if !defined(__BYTE_ORDER)
        #if defined(_BYTE_ORDER)
            #define __BYTE_ORDER    _BYTE_ORDER
        #elif defined (BYTE_ORDER)
            #define __BYTE_ORDER    BYTE_ORDER
        #else
            #error "Go order your bytes, minstrel!"
        #endif
    #endif
    #if !defined(__BIG_ENDIAN)
        #if defined(_BIG_ENDIAN)
            #define __BIG_ENDIAN    _BIG_ENDIAN
        #elif defined (BIG_ENDIAN)
            #define __BIG_ENDIAN    BIG_ENDIAN
        #else
            #error "Your endian is too big, minstrel!"
        #endif
    #endif
    #if !defined(__LITTLE_ENDIAN)
        #if defined(_LITTLE_ENDIAN)
            #define __LITTLE_ENDIAN    _LITTLE_ENDIAN
        #elif defined (LITTLE_ENDIAN)
            #define __LITTLE_ENDIAN    LITTLE_ENDIAN
        #else
            #error "Your endian is too small, minstrel!"
        #endif
    #endif

    /* Fugue: "Buckle Your Bytes, Dorothy, 'Cause Endiana Is Going Bye-Bye" */
    #define USWAP2(V) \
     ( (((V) >>  8) & 0x00FF) | (((V) << 8) & 0xFF00) )

    #define USWAP4(V) \
     ( (((V) >> 24) & 0x000000FF) | (((V) >>  8) & 0x0000FF00) | \
       (((V) <<  8) & 0x00FF0000) | (((V) << 24) & 0xFF000000) )

    #define USWAP8(V) \
     ( (((V) >> 56) & 0x00000000000000FF) | (((V) >> 40) & 0x000000000000FF00) | \
       (((V) >> 24) & 0x0000000000FF0000) | (((V) >>  8) & 0x00000000FF000000) | \
       (((V) <<  8) & 0x000000FF00000000) | (((V) << 24) & 0x0000FF0000000000) | \
       (((V) << 40) & 0x00FF000000000000) | (((V) << 56) & 0xFF00000000000000) )

    #if __BYTE_ORDER == __BIG_ENDIAN
        #define HTOBE16(V)   (V)
        #define HTOBE32(V)   (V)
        #define HTOBE64(V)   (V)
        #define BETOH16(V)   (V)
        #define BETOH32(V)   (V)
        #define BETOH64(V)   (V)
        #define HTOLE16(V)   USWAP2((uint16_t)(V))
        #define HTOLE32(V)   USWAP4((uint32_t)(V))
        #define HTOLE64(V)   USWAP8((uint64_t)(V))
        #define LETOH16(V)   USWAP2((uint16_t)(V))
        #define LETOH32(V)   USWAP4((uint32_t)(V))
        #define LETOH64(V)   USWAP8((uint64_t)(V))
    #elif __BYTE_ORDER == __LITTLE_ENDIAN
        #define HTOBE16(V)   USWAP2((uint16_t)(V))
        #define HTOBE32(V)   USWAP4((uint32_t)(V))
        #define HTOBE64(V)   USWAP8((uint64_t)(V))
        #define BETOH16(V)   USWAP2((uint16_t)(V))
        #define BETOH32(V)   USWAP4((uint32_t)(V))
        #define BETOH64(V)   USWAP8((uint64_t)(V))
        #define HTOLE16(V)   (V)
        #define HTOLE32(V)   (V)
        #define HTOLE64(V)   (V)
        #define LETOH16(V)   (V)
        #define LETOH32(V)   (V)
        #define LETOH64(V)   (V)
    #else
        #error "Oh no, not these endians again! Go trade your PDP for a real computer!"
    #endif

    #define HTON16(V)   HTOBE16((V))
    #define HTON32(V)   HTOBE32((V))
    #define HTON64(V)   HTOBE64((V))
    #define NTOH16(V)   BETOH16((V))
    #define NTOH32(V)   BETOH32((V))
    #define NTOH64(V)   BETOH64((V))

#endif

#ifdef __FreeBSD__
#undef htobe16
#undef htobe32
#undef htobe64
#undef betoh16
#undef betoh32
#undef betoh64

#undef htole16
#undef htole32
#undef htole64
#undef letoh16
#undef letoh32
#undef letoh64

#undef hton16
#undef hton32
#undef hton64
#undef ntoh16
#undef ntoh32
#undef ntoh64
#endif

extern uint16_t htobe16( uint16_t v );
extern uint32_t htobe32( uint32_t v );
extern uint64_t htobe64( uint64_t v );
extern uint16_t betoh16( uint16_t v );
extern uint32_t betoh32( uint32_t v );
extern uint64_t betoh64( uint64_t v );

extern uint16_t htole16( uint16_t v );
extern uint32_t htole32( uint32_t v );
extern uint64_t htole64( uint64_t v );
extern uint16_t letoh16( uint16_t v );
extern uint32_t letoh32( uint32_t v );
extern uint64_t letoh64( uint64_t v );

extern uint16_t hton16( uint16_t v );
extern uint32_t hton32( uint32_t v );
extern uint64_t hton64( uint64_t v );
extern uint16_t ntoh16( uint16_t v );
extern uint32_t ntoh32( uint32_t v );
extern uint64_t ntoh64( uint64_t v );


#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* ndef BENDIAN_H_INCLUDED */

/* EOF */
