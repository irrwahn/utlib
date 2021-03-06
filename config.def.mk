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
#   -DWITHOUT_SYSLOG        build logging.c without syslog() support
#   -DWITHOUT_OWN_VSYSLOG   rely on system's vsyslog() in logging.c
#   -DWITH_PTHREAD          use pthread mutexes in logging.c
export RLS_OPT := -DWITH_PTHREAD
export DBG_OPT := -DWITH_PTHREAD

# Set this to 0 to exclude the more demanding parts:
export BUILD_XTRA := 1

# Set this to 0 to build only the static library:
export BUILD_SO := 1

# Set this to either 'release' or 'debug':
export BUILD_TARGET := release

# Default install directories:
export INST_PREFIX ?= /usr/local
export INST_LIBDIR := $(INST_PREFIX)/lib/utlib
export INST_INCDIR := $(INST_PREFIX)/include/utlib
export INST_MANDIR := $(INST_PREFIX)/share/man/man3
export INST_DOCDIR := $(INST_PREFIX)/share/doc/utlib
export INST_EXDIR  := $(DOCDIR)/examples

# Adjust to match the (native or cross) build system tools:
export CC      := cc
export CCSO    := $(CC) -shared
export LD      := $(CC)
export STRIP   := strip --strip-unneeded
export AR      := ar -c -rs

# Do not edit these flags unless you really know what you're doing!
export CFLAGS  := -std=c99 -pedantic -Wall -Wextra -fstrict-aliasing -MMD -MP
export CRFLAGS := -O2 -DNDEBUG
export CDFLAGS := -O0 -DDEBUG -g3 -pg -ggdb
export CSFLAGS := -I.
export LDFLAGS :=

ifneq ($(BUILD_XTRA),0)
  export CSFLAGS += -I./extra
endif
ifneq ($(BUILD_SO),0)
  export CSFLAGS += -fPIC
endif
ifeq ($(BUILD_TARGET),release)
  CFLAGS += $(CRFLAGS) $(RLS_OPT)
else
  CFLAGS += $(CDFLAGS) $(DBG_OPT)
endif

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
