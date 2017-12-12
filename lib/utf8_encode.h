/*
 * utf8_encode.h
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
 **** utf8_encode_h 3
 **
 ** NAME
 **   utf8_encode - UTF-8 encoding functions
 **
 ** SYNOPSIS
 **   #include <utf8_encode.h>
 **
 ** DESCRIPTION
 **
 **   MACROS
 **     UTF_REPLACE_CHAR  Unicode code point used as replacement char
 **
 **   FUNCTIONS
 **     utf8_stream_encode()  call-back driven UTF-32 to UTF-8 encoder
 **
 ** SEE ALSO
 **   locale(1), locale(7)
 **
 */

#ifndef UTF8_ENCODE_H_INLCLUDED
#define UTF8_ENCODE_H_INLCLUDED

#ifdef _cplusplus
extern "C" {
#endif


#include <stdlib.h>
#include <stdint.h>

#undef UTF_REPLACE_CHAR
#define UTF_REPLACE_CHAR    0xFFFD


extern size_t utf8_stream_encode( uint32_t(*get)(void*), int(*put)(char*,int,void*), void *usr, size_t *errcnt );


#ifdef _cplusplus
} /* extern "C" { */
#endif

#endif  /* ndef UTF8_ENCODE_H_INLCLUDED */

/* EOF */
