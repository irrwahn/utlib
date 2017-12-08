/*
 * str_trim.c
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
#include <string.h>

#include <str_trim.h>


static inline char *inline_str_move( char *d, char *s )
{
    return memmove( d, s, strlen( s ) + 1 );
}


static inline char *inline_str_ltrim( char *s )
{
    char *p = s;

    while ( *p && isspace( (unsigned char)*p ) )
        ++p;
    return p > s ? inline_str_move( s, p ) : s;
}


static inline char *inline_str_rtrim( char *s )
{
    char *p = s + strlen( s );

    while ( p > s && isspace( (unsigned char)*(p - 1) ) )
        --p;
    *p = '\0';
    return s;
}


/*
 **** str_move 3
 **
 ** NAME
 **   str_move - move a string to a potentially overlapping buffer
 **
 ** SYNOPSIS
 **   #include <str_trim.h>
 **
 **   char *str_move(char *d, char *s);
 **
 ** DESCRIPTION
 **   The str_move() function moves the string pointed to by s to the
 **   buffer pointed to by d. The strings are allowed to overlap, but
 **   the destination buffer d must be large enough to receive the copy
 **   of s.  Beware of buffer overruns!
 **
 ** RETURN VALUE
 **   The str_move() function returns a pointer to the destination
 **   string d.
 **
 ** SEE ALSO
 **   strcpy(3), memmove(3)
 **
 */

char *str_move( char *d, char *s )
{
    return inline_str_move( d, s );
}


/*
 **** str_trim 3
 **
 ** NAME
 **   str_ltrim, str_rtrim, str_trim - remove leading or trailing
 **   whitespace from a string
 **
 ** SYNOPSIS
 **   #include <str_trim.h>
 **
 **   char *str_ltrim(char *s);
 **
 **   char *str_rtrim(char *s);
 **
 **   char *str_trim(char *s);
 **
 ** DESCRIPTION
 **   The str_ltrim() function modifies the string s by removing any
 **   leading whitespace characters, as defined by the isspace() function.
 **
 **   The str_rtrim() function is similar, except it removes trailing
 **   whitespace from the string s.
 **
 **   The str_trim() function removes leading and trailing whitespace
 **   and is equivalent to str_rtrim( strltrim(s) ).
 **
 ** RETURN VALUE
 **   The str_ltrim(), str_rtrim() and str_trim() functions return a
 **   pointer to the first character in the modified string s.
 **
 ** SEE ALSO
 **   str_skip(3)
 **
 */

char *str_ltrim( char *s )
{
    return inline_str_ltrim( s );
}

char *str_rtrim( char *s )
{
    return inline_str_rtrim( s );
}

char *str_trim( char *s )
{
    return inline_str_rtrim( inline_str_ltrim( s ) );
}


/*
 **** str_skip 3
 **
 ** NAME
 **   str_skip, str_skipspace - skip leading characters of a string
 **
 ** SYNOPSIS
 **   #include <str_trim.h>
 **
 **   const char *str_skip(const char *s, const char *skipset);
 **
 **   const char *str_skipspace(const char *s);
 **
 ** DESCRIPTION
 **   The str_skip() function returns a pointer to the first character
 **   in the string s that is not contained in the string skipset.
 **
 **   The str_skipspace() function is similar, except it skips only
 **   whitespace as defined by the isspace() function.
 **
 ** RETURN VALUE
 **   The str_skip() and str_skipspace() functions return a pointer
 **   to the first non-skipped character.
 **
 ** SEE ALSO
 **   str_ltrim(3)
 **
 */

const char *str_skip( const char *s, const char *skipset )
{
    while ( *s && strchr( skipset, *s ) )
        ++s;
    return s;
}

const char *str_skipspace( const char *s )
{
    while ( *s && isspace( (unsigned char)*s ) )
        ++s;
    return s;
}


/* EOF */
