#
# This fragment is included by the top level Makefile.
#

# These should be bumped manually upon release, but will be overwritten
# by most recent tag info, if we have a git repository:
export VER_MJR := 0
export VER_MNR := 0
export VER_INC := 3

# Git
# Try to extract a revision tag from repository:
TAG := $(shell git for-each-ref refs/tags --sort=-taggerdate --format='%(refname)' --count=1 | cut -d/ -f3)
ifneq ($(strip $(TAG)),)
  XTAG := $(subst ., ,$(TAG))
  VER_MJR := $(word 1,$(XTAG))
  VER_MNR := $(word 2,$(XTAG))
  VER_INC := $(word 3,$(XTAG))
  TAG_REV := $(shell git rev-list --pretty=format:%h -n 1 $(TAG) | tail -n1)
endif

# Get current HEAD hash and compare with tag hash:
VER_REV := $(strip $(shell git show -s --pretty=format:%h 2> /dev/null))
ifneq ($(VER_REV),)
  ifeq ($(VER_REV),$(TAG_REV))
    VER_REV := git_tag_is_head
  else
    VER_REV := -git_$(VER_REV)
  endif
endif

# SVN
ifeq ($(VER_REV),)
    VER_REV := $(strip $(shell svnversion 2> /dev/null | grep -E '^[0-9]+[^\s]*' | tr ':' '_'))
endif

# Neither repository nor working copy detected, use a sane default:
ifeq ($(VER_REV)),)
    VER_REV := local
endif

# Have Git repository and tag points to current head, drop revision info:
ifeq ($(VER_REV),git_tag_is_head)
    VER_REV :=
endif

# EOF
