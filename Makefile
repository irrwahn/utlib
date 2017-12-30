##
## Makefile for project: utlib
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


export PROJECT    := utlib
export LIBNAME    := libut

include version.mk
export $(PROJECT)_VER_MJR := $(VER_MJR)
export $(PROJECT)_VER_MNR := $(VER_MNR)
export $(PROJECT)_VER_INC := $(VER_INC)
export $(PROJECT)_VERSION := $(VER_MJR).$(VER_MNR).$(VER_INC)
export $(PROJECT)_REVISION := $(VER_REV)

export DOCNAME    := Utlib $($(PROJECT)_VERSION) Reference Manual

export MAKE_DEPTH := 1

SELF := $(lastword $(MAKEFILE_LIST))
export PRJDIR     := $(dir $(realpath $(SELF)))
export LIBDIR     := $(PRJDIR)/lib
export DOCDIR     := $(PRJDIR)/doc
export TSTDIR     := $(PRJDIR)/test
export EXDIR      := $(PRJDIR)/examples

BLDCFG  := config.mk
DEFCFG  := config.def.mk
DOCX    := BUGS CREDITS LICENSE README.md TODO

ifneq ($(MAKECMDGOALS),config)
  include $(BLDCFG)
endif

all: lib doc

lib: $(BUILD_TARGET)

release debug:
	@echo $@ build, version [$($(PROJECT)_VERSION)], revision [$($(PROJECT)_REVISION)]
	$(MAKE) -C $(LIBDIR) $@

doc:
	$(MAKE) -C $(DOCDIR) $@

test: lib
	$(MAKE) -C $(TSTDIR) $@

examples: lib
	@echo Building examples ...
	$(MAKE) -C $(EXDIR) $@

tarball:
	$(eval TARNAME := $(PROJECT)-$($(PROJECT)_VERSION)$($(PROJECT)_REVISION))
	$(TAR) --transform "s|^|$(TARNAME)/|" -cvzf "$(TARNAME).tar.gz" -T dist.lst

install: lib doc
	@echo Installing to $(INST_PREFIX) ...
	$(MAKE) -C $(LIBDIR) $@
	$(MAKE) -C $(DOCDIR) $@
	@echo Installing extra documents ...
	@$(MKDIR) $(INST_DOCDIR) && for f in $(DOCX); do $(CPV) $$f $(INST_DOCDIR) ; done ||:

uninstall:
	@echo Uninstalling from $(INST_PREFIX) ...
	$(MAKE) -C $(LIBDIR) $@
	$(MAKE) -C $(DOCDIR) $@
	@echo Removing extra documents ...
	@cd $(INST_DOCDIR) && for f in $(DOCX); do $(RMV) $$f ; done && $(RMDIR) $(INST_DOCDIR) ||:

clean:
	$(MAKE) -C $(LIBDIR) $@
	$(MAKE) -C $(DOCDIR) $@
	$(MAKE) -C $(TSTDIR) $@
	$(MAKE) -C $(EXDIR) $@

distclean: clean
	$(MAKE) -C $(LIBDIR) $@
	$(MAKE) -C $(DOCDIR) $@
	$(MAKE) -C $(TSTDIR) $@
	$(MAKE) -C $(EXDIR) $@
	$(RM) $(BLDCFG) *.tar.gz

.PRECIOUS: $(BLDCFG)
$(BLDCFG) config:
	@sed '/^#!!/d' $(DEFCFG) > $(BLDCFG) 2> /dev/null \
		|| grep -v '^#!!' $(DEFCFG) > $(BLDCFG) 2> /dev/null \
		|| tail -n +6 $(DEFCFG) > $(BLDCFG) 2> /dev/null \
		|| ( rm -f $(BLDCFG) ; echo "    WORKAROUND: manually copy '$(DEFCFG)' to '$(BLDCFG)'." ; false )
	@echo "Generated $(BLDCFG)"

ifneq ($(MAKECMDGOALS),config)
  include $(BLDCFG)
endif

# Handy hack: Get the value of any makefile variable by executing 'make print-VARIABLE_NAME'
print-% : ; $(info $* is a $(flavor $*) variable set to "$($*)") @true

.PHONY: all lib doc test examples release debug tarball config version clean distclean install uninstall

## EOF
