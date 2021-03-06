##
## Subordinate Makefile for project: utlib
##
## Copyright 2017 Urban Wallasch <irrwahn35@freenet.de>
##
## Redistribution and use in source and binary forms, with or without
## modification, are permitted provided that the following conditions
## are met:
##
## * Redistributions of source code must retain the above copyright
##   notice, this list of conditions and the following disclaimer.
## * Redistributions in binary form must reproduce the above copyright
##   notice, this list of conditions and the following disclaimer
##   in the documentation and/or other materials provided with the
##   distribution.
## * Neither the name of the copyright holder nor the names of its
##   contributors may be used to endorse or promote products derived
##   from this software without specific prior written permission.
##
## THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
## "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
## LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
## A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
## OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
## SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
## LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
## DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
## THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
## (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
## OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
##
##
##


ifneq ($(MAKE_DEPTH),1)
  $(error This Makefile must not be run on its own!)
endif

ARNAME     := $(LIBNAME).a
SONAME     := $(LIBNAME).so
SONAME_X   := $(SONAME).$(VER_MJR)
SONAME_XX  := $(SONAME_X).$(VER_MNR)
SONAME_XXX := $(SONAME_XX).$(VER_INC)

SELF  := $(lastword $(MAKEFILE_LIST))
SRC   := $(wildcard *.c)
HDR   := $(wildcard *.h)
ifneq ($(BUILD_XTRA),0)
  SRC   += $(wildcard extra/*.c)
  HDR   += $(wildcard extra/*.h)
endif
OBJ   := $(SRC:%.c=%.o)
DEP   := $(OBJ:%.o=%.d)
BIN   := runtests
TST_H := tests.h

test: $(BIN)
	@echo "Running tests ..."
	@./$(BIN)

$(BIN): $(TST_H) $(OBJ) $(SELF)
	$(LD) $(LDFLAGS) -L$(LIBDIR) -o$(BIN) $(OBJ) -lut

testmain.o: $(TST_H) $(SELF)
	$(CC) -c $(CFLAGS) -I$(LIBDIR) -I$(LIBDIR)/extra -o $*.o $*.c

%.o: %.c $(SELF)
	$(CC) -c $(CFLAGS) -I$(LIBDIR) -I$(LIBDIR)/extra -o $*.o $*.c

$(TST_H): $(SRC) $(SELF)
	@echo "Collecting test modules ..."
	@echo "/* Generated file, do not edit! */" > $(TST_H)
	@for f in $(SRC); do echo "  $$f"; $(GREP) -o -E 'REGISTER *\( *([^ ]+) *\)' $$f >> $(TST_H); done ||:
	@echo Generated $(TST_H)

clean:
	$(RM) $(BIN) $(TST_H) *.o *.d extra/*.o extra/*.d

distclean: clean

-include $(DEP)

.PHONY: test clean distclean

## EOF
