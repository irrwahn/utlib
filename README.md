# utlib

A collection of various utility functions, implemented in C.


## Description

### General

The utlib library is a collection of loosely related simple utility
functions addressing several fields of application, ranging from
basic string manipulation, random number generation and byte order
conversion, up to high resolution scalar time handling.

Utlib is designed to depend as little as possible on operating system
specifics, stick to ISO C as far as feasible and resort to POSIX
functionality where necessary. It should work with little or no
tweaking on most modern Linux and BSD distributions, so far it is
however only known to work with Debian GNU/Linux and FreeBSD.


### Motivation

After re-implementing various utility functions time and time again
over the past decades, I decided to finally carry out the long
standing plan to put together a library containing decent reference
implementations of those functions.

While equivalent functionality is available in many other utility
libraries, these often come tied to general application frameworks
or burdened with other considerable overhead, hence the motivation
to create a lightweight alternative that is not entangled with
other 3rd party tools or libraries.


### Overview

The following table contains a list of header files supplied by
utlib along with a short description of their respective purpose:


| Header           | Functionality                                 |
|------------------|-----------------------------------------------|
| `base16.h`       | base16 (hexadecimal) data en- and decoding    |
| `bendian.h`      | byte order conversion                         |
| `getopts.h`      | alternative command-line option parser        |
| `logging.h`      | log formatted messages to files or system log |
| `ntime.h`        | clock time in nanosecond resolution           |
| `prng.h`         | fast 64-bit pseudo random number generator    |
| `str_escape.h`   | C-style string escaping; URL encoding         |
| `str_icmp.h`     | case insensitive string compare               |
| `str_trim.h`     | string trimming (whitespace and other)        |
| `str_unescape.h` | C-style string un-escaping                    |
| `utf8_decode.h`  | UTF-8 to UTF-32 transcoding                   |
| `utf8_encode.h`  | UTF-32 to UTF-8 transcoding                   |
| `utf8_locale.h`  | locale related utilities                      |


### Usage

In order to use utlib in your project, you should:

* Make the utlib include path known to the compiler, e.g.:
   `-I/usr/local/include/utlib/`

* Include the appropriate header files in source modules that
  intend use it, e.g.: `#include <ntime.h>`

* Make the utlib library path known to the linker and instruct
   it to link your executable with utlib, e.g.:
   `-L/usr/local/lib/utlib -lut`


## Installation

After running `make config` the build environment settings can be
customized by editing the generated `config.mk` file. The tools
shortcuts declared there should also give an idea which utilities
are required to build utlib.  A subsequent `make && make install`
will build and install utlib.  The build system will always build
both a static library as well as a shared object.

**NOTE:** If the BUILD_XTRA variable is set to 0 in config.mk, the
following modules will be excluded from the build: `bendian.h`,
`getopts.h`, `logging.h`, `ntime.h`, `utf8_locale.h`. This may aid
in building the remaining modules for freestanding (non-hosted)
implementations.

### Linux

#### Prerequisites

Building utlib under Linux requires the following tools:

* Mandatory: gcc or clang, make, awk, sed
* Optional: tar, txt2man

#### Invocation

    $ git clone https://github.com/irrwahn/utlib.git
    $ cd utlib
    $ make config
    $ # customize config.mk
    $ make all
    $ make install


### FreeBSD

#### Prerequisites

Building utlib under FreeBSD requires the following tools:

* Mandatory: clang, gmake, gawk, gsed
* Optional: gtar, txt2man

#### Invocation

    $ git clone https://github.com/irrwahn/utlib.git
    $ cd utlib
    $ gmake config
    $ # customize config.mk
    $ gmake all
    $ gmake install


#### Useful make targets

| Target      | Effect                      |
|-------------|-----------------------------|
| `all`       | build library and docs      |
| `config`    | (re-)create `config.mk`     |
| `doc`       | build only documentation    |
| `test`      | build and run tests         |
| `clean`     | remove build artifacts      |
| `distclean` | remove even `config.mk`     |
| `examples`  | build example programs      |
| `lib`       | build only library          |
| `tarball`   | create distribution archive |
| `install`   | install utlib to prefix     |
| `uninstall` | remove utlib from prefix    |


## Release History

| Version                                                      | Comment            |
|--------------------------------------------------------------|--------------------|
| [0.0.1](https://github.com/irrwahn/utlib/releases/tag/0.0.1) | "For the record"   |
| [0.0.2](https://github.com/irrwahn/utlib/releases/tag/0.0.2) | "Enter Santa"      |
| [0.0.3](https://github.com/irrwahn/utlib/releases/tag/0.0.3) | "Happy 12018"      |


## Contributing

1. Fork it!
2. Create your feature branch: `git checkout -b my-new-feature`
3. Commit your changes: `git commit -am 'Add some feature'`
4. Push to the branch: `git push origin my-new-feature`
5. Submit a pull request

See the `CREDITS` file for a list of people who have contributed
to the project.


## License

Utlib is distributed under the Modified ("3-clause") BSD License.
Consult the `LICENSE` file for more information.
