NAME
  Utlib

DESCRIPTION
  The utlib library is a collection of loosely related simple utility
  functions addressing several fields of application, ranging from
  basic string manipulation, random number generation and byte order
  conversion, up to high resolution scalar time handling.

  Utlib is designed to depend as little as possible on operating system
  specifics, stick to ISO C as far as feasible and resort to POSIX
  functionality where necessary. It should work with little or no
  tweaking on most modern Linux and BSD distributions, so far it is
  however only known to work with Debian GNU/Linux and FreeBSD.


OVERVIEW
  The following table contains a list of header files supplied by
  utlib along with a short description of their respective purpose:

  base16.h        base16 (hexadecimal) data en- and decoding
  bendian.h       byte order conversion
  getopts.h       alternative command-line option parser
  logging.h       log formatted messages to files or system log
  ntime.h         clock time in nanosecond resolution
  prng.h          fast 64-bit pseudo random number generator
  str_escape.h    C-style and string escaping; URL encoding
  str_icmp.h      case insensitive string compare
  str_trim.h      string trimming (whitespace and other)
  str_unescape.h  C-style string un-escaping
  utf8_decode.h   UTF-8 to UTF-32 transcoding
  utf8_encode.h   UTF-32 to UTF-8 transcoding
  utf8_locale.h   locale related utilities


USAGE
  In order to use utlib in your project, you should:

  * Make the utlib include path known to the compiler, e.g.:
     -I/usr/local/include/utlib/

  * Include the appropriate header files in source modules that
    intend use it, e.g.: #include <ntime.h>

  * Make the utlib library path known to the linker and instruct
     it to link your executable with utlib, e.g.:
     -L/usr/local/lib/utlib -lut


SEE ALSO
  base16_h(3), bendian_h(3), getopts_h(3), logging_h(3), ntime_h(3), prng_h(3), str_escape_h(3), str_icmp_h(3), str_trim_h(3), str_unescape_h(3), utf8_decode_h(3), utf8_encode_h(3), utf8_locale_h(3)
