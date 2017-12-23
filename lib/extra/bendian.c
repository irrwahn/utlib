/*
 * bendian.c
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
 **** ntoh 3
 **
 ** NAME
 **   htobe16, htobe32, htobe64, betoh16, betoh32, betoh64, htole16, htole32, htole64, letoh16, letoh32, letoh64, hton16, hton32, hton64, ntoh16, ntoh32, ntoh64 - byte order conversion functions
 **
 ** SYNOPSIS
 **   #include <bendian.h>
 **
 ** extern uint16_t htobe16(uint16_t v);
 ** extern uint32_t htobe32(uint32_t v);
 ** extern uint64_t htobe64(uint64_t v);
 ** extern uint16_t betoh16(uint16_t v);
 ** extern uint32_t betoh32(uint32_t v);
 ** extern uint64_t betoh64(uint64_t v);
 **
 ** extern uint16_t htole16(uint16_t v);
 ** extern uint32_t htole32(uint32_t v);
 ** extern uint64_t htole64(uint64_t v);
 ** extern uint16_t letoh16(uint16_t v);
 ** extern uint32_t letoh32(uint32_t v);
 ** extern uint64_t letoh64(uint64_t v);
 **
 ** extern uint16_t hton16(uint16_t v);
 ** extern uint32_t hton32(uint32_t v);
 ** extern uint64_t hton64(uint64_t v);
 ** extern uint16_t ntoh16(uint16_t v);
 ** extern uint32_t ntoh32(uint32_t v);
 ** extern uint64_t ntoh64(uint64_t v);
 **
 ** DESCRIPTION
 **
 **   All conversion functions accept and return unsigned integer values
 **   with a width as indicated by the bit count in the function name.
 **
 **   The htobe16(), htobe32(), htobe64() functions convert their argument
 **   from host byte order to big endian byte order.
 **
 **   The betoh16(), betoh32(), betoh64() functions convert their argument
 **   from big endian byte order to host byte order.
 **
 **   The htole16(), htole32(), htole64() functions convert their argument
 **   from host byte order to little endian byte order.
 **
 **   The letoh16(), letoh32(), letoh64() functions convert their argument
 **   from little endian byte order to host byte order.
 **
 **   The hton16(), hton32(), hton64() functions convert their argument
 **   from host byte order to network byte order.
 **
 **   The ntoh16(), ntoh32(), ntoh64() functions convert their argument
 **   from network byte order to host byte order.
 **
 ** RETURN VALUE
 **   All functions return the result of the respective conversion.
 **
 ** NOTES
 **   These functions are trivial wrappers around their uppercase macro
 **   counterparts and are provided to help avoid undesirable side effects
 **   caused by macros that evaluate their argument more than once,
 **   see NTOH(3).
 **
 ** SEE ALSO
 **   bendian_h(3), NTOH(3)
 **
 */

#include <bendian.h>

uint16_t letoh16( uint16_t v ) { return LETOH16(v); }
uint32_t letoh32( uint32_t v ) { return LETOH32(v); }
uint64_t letoh64( uint64_t v ) { return LETOH64(v); }

uint16_t htole16( uint16_t v ) { return HTOLE16(v); }
uint32_t htole32( uint32_t v ) { return HTOLE32(v); }
uint64_t htole64( uint64_t v ) { return HTOLE64(v); }


uint16_t betoh16( uint16_t v ) { return BETOH16(v); }
uint32_t betoh32( uint32_t v ) { return BETOH32(v); }
uint64_t betoh64( uint64_t v ) { return BETOH64(v); }

uint16_t htobe16( uint16_t v ) { return HTOBE16(v); }
uint32_t htobe32( uint32_t v ) { return HTOBE32(v); }
uint64_t htobe64( uint64_t v ) { return HTOBE64(v); }


uint16_t ntoh16( uint16_t v ) { return NTOH16(v); }
uint32_t ntoh32( uint32_t v ) { return NTOH32(v); }
uint64_t ntoh64( uint64_t v ) { return NTOH64(v); }

uint16_t hton16( uint16_t v ) { return HTON16(v); }
uint32_t hton32( uint32_t v ) { return HTON32(v); }
uint64_t hton64( uint64_t v ) { return HTON64(v); }


/* EOF */
