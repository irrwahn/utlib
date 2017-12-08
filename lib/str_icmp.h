/*
 * str_icmp.h
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
 **** str_icmp_h 3
 **
 ** NAME
 **   str_icmp - case insensitive string compare
 **
 ** SYNOPSIS
 **   #include <str_icmp.h>
 **
 ** DESCRIPTION
 **   FUNCTIONS
 **     str_icmp(), str_nicmp()  case ignoring string compare functions
 **
 ** SEE ALSO
 **   ntoh16(3)
 **
 */

#ifndef STR_ICMP_H_INCLUDED
#define STR_ICMP_H_INCLUDED

#ifdef _cplusplus
extern "C" {
#endif

#include <stdlib.h>     /* need size_t */

extern int str_icmp( const char *s1, const char *s2 );
extern int str_nicmp( const char *s1, const char *s2, size_t n );

#ifdef _cplusplus
} /* extern "C" */
#endif

#endif /* ndef STR_ICMP_H_INCLUDED */

/* EOF */
