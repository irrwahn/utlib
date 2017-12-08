/*
 * str_icmp.c
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


#include <ctype.h>

#include <str_icmp.h>


/*
 **** str_icmp 3
 **
 ** NAME
 **   str_icmp, str_nicmp - compare two strings ignoring case
 **
 ** SYNOPSIS
 **   #include <str_icmp.h>
 **
 **   int str_icmp(const char *s1, const char *s2);
 **
 **   int str_nicmp(const char *s1, const char *s2);
 **
 ** DESCRIPTION
 **   The str_icmp() function performs a byte-by-byte comparison of the
 **   strings s1 and s2, ignoring the case of the characters.  It returns
 **   an integer less than, equal to, or greater than zero if s1 is found,
 **   respectively, to be less than, to match, or be greater than s2.
 **
 **   The str_nicmp() function is similar, except that it compares no
 **   more than n bytes of s1 and s2.
 **
 ** RETURN VALUE
 **   The str_icmp() and str_nicmp() functions return an integer less
 **   than, equal to, or greater than zero if s1 (or the first n bytes
 **   thereof) is, after ignoring case, found to be less than, to match,
 **   or be greater than s2, respectively.
 **
 ** SEE ALSO
 **   strcmp(3), strncmp(3)
 **
 */

int str_icmp( const char *s1, const char *s2 )
{
    int r;

    do
    {
        r = tolower( (unsigned char)*s1 ) - tolower( (unsigned char)*s2 );
    }
    while ( 0 == r && *s1++ && *s2++ );
    return r;
}

int str_nicmp( const char *s1, const char *s2, size_t n )
{
    int r;

    if ( 0 == n )
        return 0;
    do
    {
        r = tolower( (unsigned char)*s1 ) - tolower( (unsigned char)*s2 );
    }
    while ( 0 == r && --n && *s1++ && *s2++ );
    return r;
}


/* EOF */
