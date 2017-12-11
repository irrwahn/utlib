/*
 * utf8_decode.h
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
 **** utf8_decode_h 3
 **
 ** NAME
 **   utf8_decode - UTF-8 decoding functions
 **
 ** SYNOPSIS
 **   #include <utf8_decode.h>
 **
 ** DESCRIPTION
 **
 **   MACROS
 **     UTF_REPLACE_CHAR  Unicode code point used as replacement char
 **
 **   FUNCTIONS
 **    utf8_str_count()  count UTF-8 code points in string
 **    utf8_mem_count()  count UTF-8 code points in memory
 **
 **    utf8_str_decode() decode UTF-8 code points from string
 **    utf8_mem_decode() decode UTF-8 code points from memory
 **
 **    utf8_stream_decode()  call-back driven UTF-8 decoder
 **
 ** SEE ALSO
 **   locale(1), locale(7)
 **
 */

#ifndef UTF8_DECODE_H_INLCLUDED
#define UTF8_DECODE_H_INLCLUDED

#ifdef _cplusplus
extern "C" {
#endif


#include <stdlib.h>
#include <stdint.h>


#define UTF_REPLACE_CHAR    0xFFFD


extern size_t utf8_str_count( const char *s, size_t *errcnt );
extern size_t utf8_mem_count( void *s, size_t size, size_t *errcnt );

extern size_t utf8_str_decode( uint32_t *buf, size_t max, const char *s, size_t *errcnt );
extern size_t utf8_mem_decode( uint32_t *buf, size_t max, void *s, size_t size, size_t *errcnt );

extern size_t utf8_stream_decode( int(*get)(void*), int(*put)(uint32_t,void*), void *usr, size_t *errcnt );


#ifdef _cplusplus
} /* extern "C" { */
#endif

#endif  /* ndef UTF8_DECODE_H_INLCLUDED */

/* EOF */
