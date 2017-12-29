/*
 * bendian_test.c
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

#include <stdint.h>

#include <bendian.h>

#include "../testsupp.h"

#define U16  0xa5c3U
#define U16n 0xc3a5U
#define U32  0xa5c3d01fUL
#define U32n 0x1fd0c3a5UL
#define U64  0xa5c3d01fbe294768ULL
#define U64n 0x684729be1fd0c3a5ULL

/* This is not sanctioned by ISO C, but most compilers
   should do the sensible thing.  We Take a chance! */
static const union {
    uint8_t b[8];
    uint16_t u16;
    uint32_t u32;
    uint64_t u64;
} u = {
    .b = { 0xa5, 0xc3, 0xd0, 0x1f, 0xbe, 0x29, 0x47, 0x68 },
};

REGISTER( bendian_test )
{
    int err = 0;
    int isbe = ( u.u16 == U16 && u.u32 == U32 && u.u64 == U64 );

    MESG( "System is %s ENDIAN.", isbe ? "BIG" : "LITTLE" );
    if ( !isbe )
    {
        if ( letoh16(u.u16) != U16n || letoh32(u.u32) != U32n || letoh64(u.u64) != U64n )
            { ++err; FAIL( "letohNN()" ); }
        if ( htole16(u.u16) != U16n || htole32(u.u32) != U32n || htole64(u.u64) != U64n )
            { ++err; FAIL( "htoleNN()" ); }

        if ( betoh16(u.u16) != U16 || betoh32(u.u32) != U32 || betoh64(u.u64) != U64 )
            { ++err; FAIL( "betohNN()" ); }
        if ( htobe16(u.u16) != U16 || htobe32(u.u32) != U32 || htobe64(u.u64) != U64 )
            { ++err; FAIL( "htobeNN()" ); }

        if ( ntoh16(u.u16) != U16 || ntoh32(u.u32) != U32 || ntoh64(u.u64) != U64 )
            { ++err; FAIL( "ntohNN()" ); }
        if ( hton16(u.u16) != U16 || hton32(u.u32) != U32 || hton64(u.u64) != U64 )
            { ++err; FAIL( "htonNN()" ); }
    }
    else
    {
        if ( letoh16(u.u16) != U16 || letoh32(u.u32) != U32 || letoh64(u.u64) != U64 )
            { ++err; FAIL( "letohNN()" ); }
        if ( htole16(u.u16) != U16 || htole32(u.u32) != U32 || htole64(u.u64) != U64 )
            { ++err; FAIL( "htoleNN()" ); }

        if ( betoh16(u.u16) != U16n || betoh32(u.u32) != U32n || betoh64(u.u64) != U64n )
            { ++err; FAIL( "betohNN()" ); }
        if ( htobe16(u.u16) != U16n || htobe32(u.u32) != U32n || htobe64(u.u64) != U64n )
            { ++err; FAIL( "htobeNN()" ); }

        if ( ntoh16(u.u16) != U16n || ntoh32(u.u32) != U32n || ntoh64(u.u64) != U64n )
            { ++err; FAIL( "ntohNN()" ); }
        if ( hton16(u.u16) != U16n || hton32(u.u32) != U32n || hton64(u.u64) != U64n )
            { ++err; FAIL( "htonNN()" ); }
    }

    if ( !err )
        PASS( "bendian" );
    return err;
}

/* EOF */
