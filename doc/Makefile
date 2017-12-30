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

SELF  := $(lastword $(MAKEFILE_LIST))
GENMK := gen.mk

ORGF  := $(wildcard *.t)
CHSRC  = $(wildcard $(LIBDIR)/*.c)
CHSRC  += $(subst $(LIBDIR)/str_all.h,,$(subst $(LIBDIR)/version.h,,$(wildcard $(LIBDIR)/*.h)))
ifneq ($(BUILD_XTRA),0)
  CHSRC += $(wildcard $(LIBDIR)/extra/*.c)
  CHSRC += $(wildcard $(LIBDIR)/extra/*.h)
endif
RAWF  = $(wildcard *.tx_)
TXTF  = $(RAWF:%.tx_=%.txt) $(ORGF:%.t=%.txt)
MANF  = $(TXTF:%.txt=%)

doc: $(GENMK) $(TXTF) $(SELF)

$(GENMK): $(CHSRC) $(SELF)
	@echo "Extracting manpages from source comments:"
	@echo -n "gen: " > $(GENMK)
	@$(AWK) '/ \*{4} /{close(f); f=$$2"."$$3".tx_"; print " ",FILENAME,"-->",f; print f,"\\" >>"$(GENMK)"} \
			 / \*{2}( |$$)/{print $$0 >f}' $(CHSRC)
	@echo >> $(GENMK)
	@echo "Processing man pages:"

.PRECIOUS: %.tx_
%.tx_: %.t $(SELF)
	@$(CP) $*.t $*.tx_

%.txt: %.tx_ $(SELF)
# Remove comment remnants from .tx_ files.
	@t=$*.tx_ ; echo -n "  fix up $$t" ; $(SED) -i -E 's/^ \*\*( |$$)//' $$t
# If possible, run .tx_ files through txt2man to produce manpages and
# generate pretty-printed .txt files from those man pages, or else simply
# copy the .tx_ to .txt as a last ditch effort.
	@t=$*.tx_ ; \
		TIT=$$(echo $$t | $(CUT) -d. -f1) && SECT=$$(echo $$t | $(CUT) -d. -f2) \
		&& MAN=$$TIT.$$SECT && TIT=$$(echo $$TIT | $(SED) 's/.*/\U&/') \
		&& $(TXT2MAN) -t$$TIT -r$(PROJECT) -s$$SECT -v'$(DOCNAME)' $$t > $$MAN \
		&& echo -n ", convert to $$MAN"; \
		( MANWIDTH=78 $(MAN_L) $$MAN > "$$MAN.txt" && echo ", generate $$MAN.txt" ) \
		|| ( echo ", copy to $$MAN.txt" && $(CP) $*.tx_ $*.txt )

install:
	@echo Installing compressed man pages ...
	@$(MKDIR) $(INST_MANDIR) && for f in $(MANF); do if [ -f $$f ] ; then $(GZIP_CV) $$f > $(INST_MANDIR)/$$f.gz ; fi ; done ||:
	@echo Installing documentation ...
	@$(MKDIR) $(INST_DOCDIR) && for f in $(TXTF); do $(CPV) $$f $(INST_DOCDIR) ; done ||:

uninstall:
	@echo Removing compressed man pages ...
	@cd $(INST_MANDIR) && for f in $(MANF); do $(RMV) $$f.gz ; done ||:
	@echo Removing documentation ...
	@cd $(INST_DOCDIR) && for f in $(TXTF); do $(RMV) $$f ; done ||:

clean:
	$(RM) $(GENMK) *.tx_ $(TXTF) $(MANF)

distclean: clean

ifeq ($(MAKECMDGOALS),doc)
  -include $(GENMK)
endif

.PHONY: doc gen install uninstall clean distclean

## EOF
