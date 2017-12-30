/*
 * utf8_indec.h
 *
 */

/*
 * License
 *
 * Copyright (c) 2008-2010 Bjoern Hoehrmann <bjoern@hoehrmann.de>
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 */

/*
 * Minor code modifications and additional macro definition:
 * Copyright (c) 2017 Urban Wallasch <irrwahn35@freenet.de>
 * Above license terms apply.
 *
 * This _private_ utlib header provides a slightly modified version
 * of Bjoern Hoehrmann's DFA based inline UTF-8 stream decoder,
 * see http://bjoern.hoehrmann.de/utf-8/decoder/dfa/ for details.
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

#ifndef UTF8_INDEC_H_INCLUDED
#define UTF8_INDEC_H_INCLUDED

#ifdef _cplusplus
extern "C" {
#endif

#include <stdint.h>

#define UTF8_ACCEPT 0
#define UTF8_REJECT 12

static const uint8_t utf8d[] = {
    /* The first part of the table maps bytes to character classes to
       reduce the size of the transition table and create bit masks. */
     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
     1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,  9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,
     7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,  7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
     8,8,2,2,2,2,2,2,2,2,2,2,2,2,2,2,  2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
    10,3,3,3,3,3,3,3,3,3,3,3,3,4,3,3, 11,6,6,6,5,8,8,8,8,8,8,8,8,8,8,8,
    /* The second part is a transition table that maps a combination
       of a state of the automaton and a character class to a state. */
     0,12,24,36,60,96,84,12,12,12,48,72, 12,12,12,12,12,12,12,12,12,12,12,12,
    12, 0,12,12,12,12,12, 0,12, 0,12,12, 12,24,12,12,12,12,12,24,12,24,12,12,
    12,12,12,12,12,12,12,24,12,12,12,12, 12,24,12,12,12,12,12,12,12,24,12,12,
    12,12,12,12,12,12,12,36,12,36,12,12, 12,36,12,12,12,12,12,36,12,36,12,12,
    12,36,12,12,12,12,12,12,12,12,12,12,
};

static inline int utf8_c( uint8_t b, int st, uint32_t *cp )
{
    int t = utf8d[b];
    *cp = st ? (*cp << 6) | (b & 0x3f) : b & (0xff >> t);
    return utf8d[256 + st + t];
}

/* A fast stream validation macro, no code point calculation performed. */
#define utf8_v(b,st)    (utf8d[ 256 + (st) + utf8d[(uint8_t)(b)] ])


#ifdef _cplusplus
} /* extern "C" */
#endif

#endif /* ndef UTF8_INDEC_H_INCLUDED */

/* EOF */
