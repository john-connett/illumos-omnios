#
# CDDL HEADER START
#
# The contents of this file are subject to the terms of the
# Common Development and Distribution License (the "License").
# You may not use this file except in compliance with the License.
#
# You can obtain a copy of the license at usr/src/OPENSOLARIS.LICENSE
# or http://www.opensolaris.org/os/licensing.
# See the License for the specific language governing permissions
# and limitations under the License.
#
# When distributing Covered Code, include this CDDL HEADER in each
# file and include the License file at usr/src/OPENSOLARIS.LICENSE.
# If applicable, add the following below this CDDL HEADER, with the
# fields enclosed by brackets "[]" replaced with your own identifying
# information: Portions Copyright [yyyy] [name of copyright owner]
#
# CDDL HEADER END
#

#
# Copyright 2008 Sun Microsystems, Inc.  All rights reserved.
# Use is subject to license terms.
#

#
#	This makefile drives the production of unix (and unix.o).
#
#	sun4u serengeti implementation architecture dependent
#

#
#	Path to the base of the uts directory tree (usually /usr/src/uts).
#
UTSBASE	= ../../..

#
#	Define the module and object file sets.
#
UNIX		= unix
OBJECTS		= $(SPECIAL_OBJS:%=$(OBJS_DIR)/%) \
		  $(CORE_OBJS:%=$(OBJS_DIR)/%) \
		  $(MACH_NOT_YET_KMODS:%=$(OBJS_DIR)/%)
LINTS		= $(SPECIAL_OBJS:%.o=$(LINTS_DIR)/%.ln) \
		  $(CORE_OBJS:%.o=$(LINTS_DIR)/%.ln) \
		  $(MACH_NOT_YET_KMODS:%.o=$(LINTS_DIR)/%.ln) \
		  $(LINTS_DIR)/vers.ln \
		  $(LINTS_DIR)/modstubs.ln

KRTLD_MAPFILE	= $(UTSBASE)/sparc/krtld/mapfile
KRTLD_OBJECTS	= $(KRTLD_OBJS:%=$(OBJS_DIR)/%)
KRTLD_O		= $(OBJS_DIR)/krtld.o

ROOTMODULE	= $(ROOT_SERENGETI_KERN_DIR)/$(UNIX)
UNIX_BIN	= $(OBJS_DIR)/$(UNIX)

LIBS		= $(GENLIB) $(PLATLIB) $(CPULIB)

GENUNIX		= genunix
GENUNIX_DIR	= ../../$(GENUNIX)
GENOPTS		= -L $(GENUNIX_DIR)/$(OBJS_DIR) -l $(GENUNIX)

CPU_DIR		= .
CPUOPTS		= -L $(CPU_DIR)/$(OBJS_DIR) -l $(CPUNAME)

PLAT_DIR	= ../../platmod
PLATOPTS	= -L $(PLAT_DIR)/$(OBJS_DIR) -l $(PLATMOD)

LIBOPTS		= $(GENOPTS) $(PLATOPTS) $(CPUOPTS)

CTFEXTRAOBJS	= $(OBJS_DIR)/vers.o

#
#	Include common rules.
#
include $(UTSBASE)/sun4u/serengeti/Makefile.serengeti

#
#	Define targets
#
ALL_TARGET	= $(UNIX_BIN)
LINT_TARGET	= $(LINT_LIB)
INSTALL_TARGET	= $(UNIX_BIN) $(ROOTMODULE)

#
#	Overrides
#
ALL_BUILDS	= $(ALL_BUILDSONLY64)
DEF_BUILDS	= $(DEF_BUILDSONLY64)
SYM_BUILDS	= $(DEF_BUILDSONLY64)
CLEANLINTFILES	+= $(LINT32_FILES)

#
#	This is UNIX_DIR. Use a short path.
#
UNIX_DIR	= .

#
#	Overrides
#
CLEANFILES	+= $(UNIX_O) $(MODSTUBS_O) $(KRTLD_O)  $(KRTLD_OBJECTS) \
		   $(OBJS_DIR)/vers.c  $(OBJS_DIR)/vers.o \
		   $(CPU_OBJ) $(CPULIB) \
		   $(DTRACESTUBS_O) $(DTRACESTUBS)

CLOBBERFILES	= $(CLEANFILES) $(UNIX_BIN)
CLEANLINTFILES	+= $(LINT_LIB)

#
# lint pass one enforcement 
# Turn on doubleword alignment for 64 bit counter timer registers
#
CFLAGS += $(CCVERBOSE) -dalign

CERRWARN	+= -_gcc=-Wno-parentheses
CERRWARN	+= $(CNOWARN_UNINIT)
CERRWARN	+= -_gcc=-Wno-char-subscripts
CERRWARN	+= -_gcc=-Wno-unused-variable
CERRWARN	+= -_gcc=-Wno-unused-function
CERRWARN	+= -_gcc=-Wno-unused-label
CERRWARN	+= -_gcc=-Wno-type-limits
CERRWARN	+= -_gcc=-Wno-clobbered
CERRWARN	+= -_gcc=-Wno-empty-body
CERRWARN	+= -_gcc=-Wno-unused-value
CERRWARN	+= -_gcc=-Wno-switch

#
#	Default build targets.
#
.KEEP_STATE:

def:		$(DEF_DEPS)

all:		$(ALL_DEPS)

clean:		$(CLEAN_DEPS)

clobber:	$(CLOBBER_DEPS)

lint:		$(LINT_DEPS)

clean.lint:	$(CLEAN_LINT_DEPS)

install:  	$(INSTALL_DEPS)

symcheck:	$(SYM_DEPS)

$(UNIX_BIN):	$(UNIX_O) $(KRTLD_O) $(MODSTUBS_O) $(MAPFILE) $(LIBS) \
		$(DTRACESTUBS) 
	$(LD) -dy -b -o $@ -e _start -M $(MAPFILE) \
	    $(UNIX_O) $(KRTLD_O) $(MODSTUBS_O) $(LIBOPTS) $(DTRACESTUBS) 
	$(CTFMERGE_UNIQUIFY_AGAINST_GENUNIX)
	$(POST_PROCESS)

symcheck.targ:	$(UNIX_O) $(KRTLD_O) $(MODSTUBS_O) $(LIBS)
	$(LD) -dy -b -o $(SYM_MOD) -M $(MAPFILE) \
	$(UNIX_O) $(KRTLD_O) $(MODSTUBS_O) $(LIBOPTS) $(DTRACESTUBS) 

$(UNIX_O):	$(OBJECTS) $(OBJS_DIR)/vers.o
	$(LD) -r -o $@ $(OBJECTS) $(OBJS_DIR)/vers.o

$(KRTLD_O):	$(KRTLD_OBJECTS)
	$(LD) -r -o $@ -M $(KRTLD_MAPFILE) $(KRTLD_OBJECTS)

#
#	CPU_OBJ now comprises of 2 object files which come from sun4 common
#	and from architecture dependent code.  OBJS_DIR is prepended where
#	CPU_OBJ is defined to allow for building multiple CPU_OBJ's
#
$(CPULIB):	$(CPU_OBJ)
	$(BUILD.SO) $(CPU_OBJ)

#
#	The global lint target builds the kernel lint library (llib-lunix.ln)
#	which is equivalent to a lint of /unix.o. Then all kernel modules for
#	this architecture are linted against the kernel lint library.
#
#	Note:	lint errors in the kernel lint library will be repeated for
#		each module. It is important that the kernel lint library
#		be clean to keep the textual output to a reasonable level.
#

$(LINT_LIB):	$(LINT_LIB_DIR) $(LINTS)
	@-$(ECHO) "\n$(UNIX): (library construction):"
	@$(LINT) -o $(UNIX) $(LINTFLAGS) $(LINTS)
	@$(MV) $(@F) $@

lintlib:	$(LINT_DEPS)

#
#	Include common targets.
#
include $(UTSBASE)/sun4u/serengeti/Makefile.targ
