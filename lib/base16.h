/*
 * base16.h
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
 **** base16_h 3
 **
 ** NAME
 **   base16 - convert data into textual hex representation and back
 **
 ** SYNOPSIS
 **   #include <base16.h>
 **
 ** DESCRIPTION
 **   FUNCTIONS
 **     b16_encode()  convert binary data to textual hexadecimal representation
 **
 **     b16_decode()  convert textual hexadecimal representation to binary data
 **
 ** SEE ALSO
 **   b16_encode(3), b16_decode(3)
 **
 */

#ifndef BASE16_H_INCLUDED
#define BASE16_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>

extern size_t b16_encode( char *buf, size_t sz, const void *s, size_t len );
extern int b16_decode( char *buf, size_t sz, const void *s, size_t len, size_t *errcnt );


#ifdef __cplusplus
} /* extern "C" { */
#endif

#endif  /* ndef BASE16_H_INCLUDED */

/* EOF */
