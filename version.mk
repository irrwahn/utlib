#
# This fragment is included by the top level Makefile.
#

# These must be bumped manually upon release:
export VER_MJR := 0
export VER_MNR := 0
export VER_INC := 1

# Try to extract a revision tag from an assumed repository:
# Git
VER_REV := $(shell git show -s --pretty=format:%h 2> /dev/null)
ifneq ($(strip $(VER_REV)),)
    VER_REV := "git_$(VER_REV)"
endif
# SVN
ifeq ($(strip $(VER_REV)),)
    VER_REV := $(shell svnversion 2> /dev/null | grep -E '^[0-9]+[^\s]*' | tr ':' '_')
endif
# No repository or working copy detected, use default.
ifeq ($(strip $(VER_REV)),)
    VER_REV := local
endif

# EOF
