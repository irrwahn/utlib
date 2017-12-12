/*
 * utf8_inenc.h
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

/*
 * This _private_ utlib header provides an inline UTF-32 to UTF-8 transcoder.
 *
 */

/*
 * UTF-8 encoding:
 *  # Bits  First c.p.  Last c.p.  Byte 1    Byte 2    Byte 3    Byte 4
 *  1    7  U+0000      U+007F     0xxxxxxx
 *  2   11  U+0080      U+07FF     110xxxxx  10xxxxxx
 *  3   16  U+0800      U+FFFF     1110xxxx  10xxxxxx  10xxxxxx
 *  4   21  U+10000     U+10FFFF   11110xxx  10xxxxxx  10xxxxxx  10xxxxxx
 *
 * Invalid and problematic bytes and sequences:
 *  - C0, C1 : invalid (would be lead byte of overlong encoding)
 *  - ED     : _can_ start U+D800-U+DFFF (UTF-16 surrogate halves)
 *  - E0, F0 : _can_ start overlong encodings
 *  - F4     : _can_ start code points greater than U+10FFFF
 *  - F5-FD  : invalid (lead byte of a code point larger than U+10FFFF)
 *  - FE, FF : invalid (never defined for any purpose in UTF-8)
 *  - continuation byte, where none was expected
 *  - less continuation bytes than expected for lead byte
 *
 */

#ifndef UTF8_INENC_H_INCLUDED
#define UTF8_INENC_H_INCLUDED

#ifdef _cplusplus
extern "C" {
#endif

#include <stdint.h>

static inline int utf8_ec( uint32_t cp, uint8_t *b )
{
    if ( cp < 0x80 )
    {
        b[0] = cp;
        return 1;
    }
    if ( cp < 0x800 )
    {
        b[0] = 0xc0 | (cp >> 6);
        b[1] = 0x80 | (cp & 0x3f);
        return 2;
    }
    if ( cp < 0x10000 )
    {
        if ( cp < 0xd800 || cp > 0xdfff )
        {
            b[0] = 0xe0 | (cp >> 12);
            b[1] = 0x80 | ((cp >> 6) & 0x3f);
            b[2] = 0x80 | (cp & 0x3f);
            return 3;
        }
        return 0;
    }
    if ( cp < 0x110000 )
    {
        b[0] = 0xf0 | (cp >> 18);
        b[1] = 0x80 | ((cp >> 12) & 0x3f);
        b[2] = 0x80 | ((cp >> 6) & 0x3f);
        b[3] = 0x80 | (cp & 0x3f);
        return 4;
    }
    return 0;
}

/* A fast macro to validate and get byte count, no actual encoding performed. */
#define utf8_ev(c)   ((((uint32_t)(c))<0x80)?1:(((uint32_t)(c))<0x800)?2: \
  (((uint32_t)(c))<0x10000)?((((uint32_t)(c))< 0xd800||((uint32_t)(c))>0xdfff)?3:0): \
  (((uint32_t)(c))<0x110000)?4:0)


#ifdef _cplusplus
} /* extern "C" */
#endif

#endif /* ndef UTF8_INENC_H_INCLUDED */

/* EOF */
