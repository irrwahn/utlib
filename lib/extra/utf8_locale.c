/*
 * utf8_locale.c
 *
 * Copyright 2013-2017 Urban Wallasch <irrwahn35@freenet.de>
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


#include <string.h>
#include <locale.h>

#include <utf8_locale.h>

/*
 **** locale_is_utf8 3
 **
 ** NAME
 **   locale_is_utf8 - test, if locale uses UTF-8 encoding
 **
 ** SYNOPSIS
 **   #include <ntime.h>
 **
 **   int locale_is_utf8(const char *locale);
 **
 ** DESCRIPTION
 **   The locale_is_utf8() function tests, if locale supports the UTF-8
 **   character encoding. If locale is NULL, it first tries to determine
 **   the current locale's LC_CTYPE setting and then perform the test.
 **
 ** RETURN VALUE
 **   The locale_is_utf8() function returns a non-zero value, if the
 **   locale supports UTF-8 character encoding, and zero otherwise.
 **
 ** SEE ALSO
 **   setlocale(3), locale(1), locale(7)
 **
 */
int locale_is_utf8( const char *locale )
{
    const char *delim = "@+,;";     // mind the \0

    if ( NULL == locale )
        locale = setlocale( LC_CTYPE, NULL );
    if ( NULL == locale )
        return 0;
    for ( const char *p = locale; NULL == strchr( delim, *p ); ++p )
    {
        if ( '.' == *p )
        {
            const char *enc;
            for ( enc = ++p; !strchr( delim, *p ); ++p )
                ;
            if ( ( p - enc == 5 && 0 == strncmp( enc, "UTF-8", 5 ) )
              || ( p - enc == 4 && 0 == strncmp( enc, "utf8", 4 ) ) )
                return 1;
            break;
        }
    }
    return 0;
}


/* EOF */
