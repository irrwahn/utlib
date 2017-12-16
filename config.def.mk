#!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
#!! IMPORTANT NOTE: DO NOT DIRECTLY EDIT THIS TEMPLATE!    !!
#!! Instead, run `make config´ first and edit config.mk,   !!
#!! or just run plain `make´ to use the defaults.          !!
#!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

#
# This build configuration is included by the utlib top level Makefile.
#

# Build type (release, debug) specific library configuration flags.
# Valid flags include:
#   -DWITHOUT_SYSLOG		build logging.c without syslog() support
#   -DWITHOUT_OWN_VSYSLOG	rely on system's vsyslog() in logging.c
#   -DWITH_PTHREAD			use pthread mutexes in logging.c
export RLS_OPT := -DWITH_PTHREAD
export DBG_OPT := -DWITH_PTHREAD

# Default install directories:
export INST_PREFIX ?= /usr/local
export INST_LIBDIR := $(INST_PREFIX)/lib/utlib
export INST_INCDIR := $(INST_PREFIX)/include/utlib
export INST_MANDIR := $(INST_PREFIX)/share/man/man1
export INST_DOCDIR := $(INST_PREFIX)/share/doc/utlib
export INST_EXDIR  := $(DOCDIR)/examples

# Adjust to match the (native or cross) build system tools:
export CC      ?= cc
export CCSO    := $(CC) -shared
export LD      := $(CC)
export STRIP   := strip
export AR      := ar

# Do not edit these flags unless you really know what you're doing!
export CFLAGS  := -std=c99 -pedantic -Wall -Wextra -fstrict-aliasing -MMD -MP
export CRFLAGS := -O2 -DNDEBUG
export CDFLAGS := -O0 -DDEBUG -g3 -pg -ggdb
export CSFLAGS := -I. -fPIC

# Generic tool shorts:
export SH      := sh
export CP      := cp -af
export CPV     := cp -afv
export MV      := mv -f
export RM      := rm -f
export RMV     := rm -rfv
export RMDIR   := rmdir -v
export MKDIR   := mkdir -pv
export TOUCH   := touch
export LN      := ln -sf
export FIND    := find
export GREP    := grep
export BASENAME:= basename
export CUT     := cut
export TXT2MAN := txt2man
export GZIP_C  := gzip -c
export GZIP_CV := gzip -cv
UNAME_S:=$(strip $(shell uname -s 2> /dev/null))
ifeq ($(UNAME_S),FreeBSD)
    export TAR := gtar
    export AWK := gawk
    export SED := gsed
    export MAN_L := false
else
    export TAR := tar
    export AWK := awk
    export SED := sed
    export MAN_L := man -l
endif

# EOF
