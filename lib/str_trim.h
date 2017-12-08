/*
 * str_trim.h
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
 **** str_trim_h 3
 **
 ** NAME
 **   str_trim - trim or skip leading or trailing characters of a string
 **
 ** SYNOPSIS
 **   #include <str_trim.h>
 **
 ** DESCRIPTION
 **   FUNCTIONS
 **     str_move()  copy a string between overlapping buffers
 **
 **     str_ltrim(), str_rtrim(), str_trim()  strip leading and/or trailing characters from a string
 **
 **     str_skipspace(), str_skip()  skip leading characters of a string
 **
 ** NOTES
 **   None.
 **
 ** SEE ALSO
 **   str_move(3), str_trim(3), str_skip(3)
 **
 */

#ifndef STR_TRIM_H_INCLUDED
#define STR_TRIM_H_INCLUDED

#ifdef _cplusplus
extern "C" {
#endif

extern char *str_move( char *d, char *s );

extern char *str_ltrim( char *s );
extern char *str_rtrim( char *s );
extern char *str_trim( char *s );

extern const char *str_skipspace( const char *s );
extern const char *str_skip( const char *s, const char *skipset );

#ifdef _cplusplus
} /* extern "C" */
#endif

#endif /* ndef STR_TRIM_H_INCLUDED */

/* EOF */
