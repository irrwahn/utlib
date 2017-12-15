#include <stdint.h>

#include <bendian.h>

#include "testsupp.h"

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

int REGISTER(bendian)( int id )
{
    int err = 0;
    int isbe = ( u.u16 == U16 && u.u32 == U32 && u.u64 == U64 );

    MESG( id, "System is %s ENDIAN.", isbe ? "BIG" : "LITTLE" );
    if ( !isbe )
    {
        if ( letoh16(u.u16) != U16n || letoh32(u.u32) != U32n || letoh64(u.u64) != U64n )
            { ++err; FAIL( id, "letohNN()" ); }
        if ( htole16(u.u16) != U16n || htole32(u.u32) != U32n || htole64(u.u64) != U64n )
            { ++err; FAIL( id, "htoleNN()" ); }

        if ( betoh16(u.u16) != U16 || betoh32(u.u32) != U32 || betoh64(u.u64) != U64 )
            { ++err; FAIL( id, "betohNN()" ); }
        if ( htobe16(u.u16) != U16 || htobe32(u.u32) != U32 || htobe64(u.u64) != U64 )
            { ++err; FAIL( id, "htobeNN()" ); }

        if ( ntoh16(u.u16) != U16 || ntoh32(u.u32) != U32 || ntoh64(u.u64) != U64 )
            { ++err; FAIL( id, "ntohNN()" ); }
        if ( hton16(u.u16) != U16 || hton32(u.u32) != U32 || hton64(u.u64) != U64 )
            { ++err; FAIL( id, "htonNN()" ); }
    }
    else
    {
        if ( letoh16(u.u16) != U16 || letoh32(u.u32) != U32 || letoh64(u.u64) != U64 )
            { ++err; FAIL( id, "letohNN()" ); }
        if ( htole16(u.u16) != U16 || htole32(u.u32) != U32 || htole64(u.u64) != U64 )
            { ++err; FAIL( id, "htoleNN()" ); }

        if ( betoh16(u.u16) != U16n || betoh32(u.u32) != U32n || betoh64(u.u64) != U64n )
            { ++err; FAIL( id, "betohNN()" ); }
        if ( htobe16(u.u16) != U16n || htobe32(u.u32) != U32n || htobe64(u.u64) != U64n )
            { ++err; FAIL( id, "htobeNN()" ); }

        if ( ntoh16(u.u16) != U16n || ntoh32(u.u32) != U32n || ntoh64(u.u64) != U64n )
            { ++err; FAIL( id, "ntohNN()" ); }
        if ( hton16(u.u16) != U16n || hton32(u.u32) != U32n || hton64(u.u64) != U64n )
            { ++err; FAIL( id, "htonNN()" ); }
    }

    if ( !err )
        PASS( id, "bendian" );
    return err;
}

/* EOF */