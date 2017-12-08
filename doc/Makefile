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
TXTF  := $(wildcard *.?.txt)
MANF  := $(TXTF:%.txt=%)

release debug:

doc: doc_pre $(MANF) $(SELF)

doc_pre:
	@echo "Generating docs ..."

%: %.txt $(SELF)
# remove comment format remnants from .txt files
	@t=$*.txt ; echo -n "  fix up $$t" ; $(SED) -i -E 's/^ \*\*( |$$)//' $$t
# if possible, run .txt files through txt2man to produce manpages,
# then regenerate pretty-printed .txt files from those man pages
	@t=$*.txt ; command -pv $(TXT2MAN) >/dev/null && \
	  TIT=$$(echo $$t | $(CUT) -d. -f1) && SECT=$$(echo $$t | $(CUT) -d. -f2) && \
	  MAN=$$TIT.$$SECT && TIT=$$(echo $$TIT | $(SED) 's/.*/\U&/') && \
	  echo -n ", convert to $$MAN" && \
	  $(TXT2MAN) -t$$TIT -r$(PROJECT) -s$$SECT -v'$(DOCNAME)' $$t > $$MAN && \
	  command -pv $(MAN) >/dev/null && echo ", regenerate $$t" && \
	  MANWIDTH=78 $(MAN) -l $$MAN > "$$t.m" && $(MV) "$$t.m" "$$t" || echo ''

install:
	@echo Installing compressed man pages ...
	@$(MKDIR) $(INST_MANDIR) && for f in $(MANF); do $(GZIP_CV) $$f > $(INST_MANDIR)/$$f.gz ; done ||:
	@echo Installing documentation ...
	@$(MKDIR) $(INST_DOCDIR) && for f in $(TXTF); do $(CPV) $$f $(INST_DOCDIR) ; done ||:

uninstall:
	@echo Removing compressed man pages ...
	@cd $(INST_MANDIR) && for f in $(MANF); do $(RMV) $$f.gz ; done ||:
	@echo Removing documentation ...
	@cd $(INST_DOCDIR) && for f in $(TXTF); do $(RMV) $$f ; done ||:

clean:
	$(RM) $(TXTF) $(MANF)

distclean: clean

.PHONY: release debug doc doc_pre install uninstall clean distclean

## EOF
