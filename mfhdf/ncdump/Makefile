# Generated automatically from Makefile.in by configure.
# Makefile for ncdump(1).
#
# $Id$

PROGRAM		= ncdump
BINFILES	= $(PROGRAM)
HDF_LIB         = /hdf/home/chouck/hdf/33/lib/libdf.a
HDF_INC         = -I/hdf/home/chouck/hdf/33/include -DSUN
CPP_NETCDF	= -I../libsrc -I../port $(HDF_INC)
CPPFLAGS	= $(CPP_NETCDF) -DNDEBUG -DHDF -DNO_HAVE_PROTOTYPES -Dconst= -DNO_STDARG -DNO_STRERROR
CFLAGS		= -O
FFLAGS		= -O
MANIFEST	= Makefile.in ncdump.c ncdump.h  depend msoft.mk \
		  test0.cdl vardata.c vardata.h dumplib.c dumplib.h ncdump.1
MANUALS		= ncdump.1
LIBNAME		= netcdf
LD_NETCDF	= -L../libsrc -l$(LIBNAME)
OBJS		=  ncdump.o vardata.o dumplib.o
LD_XDR		= 
LD_NETCDF	= -L../libsrc -lnetcdf
LIBS		= $(LD_NETCDF) $(LD_XDR) $(HDF_LIB) -lm
prefix		= /hdf/home/chouck/hdf
GARBAGE		= $(PROGRAM) test0.nc test1.nc test1.cdl test2.cdl

all::		$(PROGRAM)


test:		$(PROGRAM) test0.cdl
	$(NCGEN) -o test0.nc -n test0.cdl
	./$(PROGRAM) test0.nc > test1.cdl
	$(NCGEN) -o test1.nc -n test1.cdl
	./$(PROGRAM) -n test0 test1.nc > test2.cdl
	@cmp test1.cdl test2.cdl && \
	    echo "*** $(PROGRAM) test successful ***" ; \
	    rm test1.cdl test1.nc test2.cdl

install::	installed_program installed_manuals

# Generated automatically from master.mk.in by configure.
# This is the master makefile.  It contains general rules.
#
# $Id$


.SUFFIXES:
.SUFFIXES:	.so .o .i .c .fc .f .F .l .y

############################################################################
# General Macros:
############################################################################
prefix		= /hdf/home/chouck/hdf
exec_prefix	= /hdf/home/chouck/hdf/bin
AR		= ar
CC		= cc
CPP		= cc -E
INCDIR		= $(prefix)/include
INSTALL		= /bin/install -c
INSTALL_DATA	= $(INSTALL) -m 664
INSTALL_PROGRAM	= $(INSTALL)
FC		= f77
NCDUMP		= @NCDUMP@
NCGEN		= @NCGEN@
NEQN		= neqn
OS		= sunos
SHELL		= /bin/sh
TBL		= tbl


############################################################################
# Recursive make(1)s in Subdirectories:
############################################################################

subdir_dummy_target $(SUBDIR_TARGETS):
	@echo ""
	@dir=`echo $@ | sed 's,/.*,,'`; \
	    target=`echo $@ | sed "s,$$dir/,,"`; \
	    cd $$dir && \
	    echo "making \`$$target' in directory `pwd`" && \
	    echo "" && \
	    $(MAKE) $(MFLAGS) prefix=$(prefix) exec_prefix=$(exec_prefix) \
		$$target || exit 1
	@echo ""
	@echo "returning to directory `pwd`"
	@echo ""


############################################################################
# Compilation:
############################################################################
FORTC		= @FORTC@

# Compile FORTRAN code.
#
.f.o:
	$(FC) -c $(FFLAGS) $<
.F.o:
	ln -s $< $$$$.c \
	&& $(CPP) -DUD_FORTRAN_SOURCE $(CPPFLAGS) $$$$.c \
	    | sed '/^# *[0-9]/d' > $*.f \
	|| (rm -f $*.f $$$$.c; exit 1) \
	&& rm $$$$.c
	$(FC) -c $(FFLAGS) $*.f
.F.f:
	ln -s $< $$$$.c \
	&& $(CPP) -DUD_FORTRAN_SOURCE $(CPPFLAGS) $$$$.c \
	    | sed '/^# *[0-9]/d' > $@ \
	|| (rm -f $@ $$$$.c; exit 1) \
	&& rm $$$$.c

# Generate FORTRAN-callable C code.
#
.fc.o:
	$(FORTC) -O $(OS) $< > $*.c \
	    || ( rm -f $*.c; exit 1)
	$(CC) -c $(CFLAGS) $(CPPFLAGS) $*.c
.fc.c:
	$(FORTC) -O $(OS) $< > $@ \
	    || (rm -f $*.c; exit 1)

# Compile C code.
#
.c.o:
	$(CC) -c $(CPPFLAGS) $(CFLAGS) $<

# Compile lex(1) code.  This rule is here to ensure the use of CPPFLAGS.
# 
.l.o:
	rm -f $*.c
	$(LEX) -t $< > $*.c
	$(CC) -c $(CPPFLAGS) $(CFLAGS) $*.c
	rm $*.c

# C preprocess.  
#   NB: 
#	1. The output `file' is not created but appears on standard output;
#	2. Line numbering information is kept.
#
.c.i:
	$(CPP) $(CPPFLAGS) $<


############################################################################
# Libraries:
############################################################################
RANLIB		= ranlib
LIBDIR		= $(prefix)/lib
LIB_A		= lib$(LIBRARY).a

library::		$(LIB_A)

# The following rule is forced because the library can be created by a
# makefile in another directory prior to the execution of the following rule.
#
$(LIB_A):		$(LIBOBJS) FORCE
	$(AR) rcuv $@ $(LIBOBJS)
	$(RANLIB) $@

# The following rule is for those makefiles needing to install object
# modules in a library in another directory.  They need only define 
# REMOTE_LIBRARY and LIBOBJS.  The rule is forced for the same reason as the
# $(LIB_A) rule.
#
lib_dummy $(REMOTE_LIBRARY):	$(LIBOBJS) FORCE
	$(AR) rcuv $@ $(LIBOBJS)
	$(RANLIB) $@

installed_libfiles:	FORCE
	@if [ -n "$(LIBFILES)" ]; then \
	    libfiles="$(LIBFILES)"; \
	    for libfile in $$libfiles; do \
		$(MAKE) $(MFLAGS) LIBDIR=$(LIBDIR) LIBFILE=$$libfile \
		    installed_libfile || exit 1; \
	    done; \
	else \
	    :; \
	fi

installed_library::	FORCE
	@$(MAKE) $(MFLAGS) LIBDIR=$(LIBDIR) LIBFILE=$(LIB_A) \
	    installed_libfile

# The following target decouples the previous targets from the actual rule;
# thus, eliminating extraneous make(1) messages.
#
installed_libfile:	$(LIBDIR)/$(LIBFILE)

$(LIBDIR)/$(LIBFILE):	$(LIBDIR) $(LIBFILE)
	$(INSTALL_DATA) $(LIBFILE) $@

dummy_libdir $(LIBDIR):
	mkdir -p $@
	chmod u+rwx,g+rws,o=rx $@

LIB_SO		= lib$(LIBRARY).so.$(MAJOR_NO).$(MINOR_NO)
LIB_SA		= lib$(LIBRARY).sa.$(MAJOR_NO).$(MINOR_NO)

#library::		FORCE
#	@so_objs=`echo $(LIBOBJS) | sed 's/\.o/.so/g'`; \
#	if [ -z "$$so_objs" ]; then \
#	    :; \
#	else \
#	    $(MAKE) $(MFLAGS) $(LIB_SO) \
#		SO_OBJS="$$so_objs"; \
#	fi
#	@if [ -z "$(SA_OBJS)" ]; then \
#	    :; \
#	else \
#	    $(MAKE) $(MFLAGS) $(LIB_SA); \
#	fi

$(LIB_SO):	$(SO_OBJS)
	ld -assert pure-text $(SO_OBJS) -o $@
$(LIB_SA):	$(SA_OBJS) FORCE
	$(AR) rcuv $@ $(SA_OBJS)

.fc.so:
	$(FORTC) -O $(OS) $< > $*.c \
	    || (rm -f $*.c; exit 1)
	$(CC) -c -pic $(CFLAGS) $(CPPFLAGS) $*.c -o $$$$.o \
	    || (rm -f $$$$.o; exit 1)
	mv $$$$.o $@
.c.so:
	$(CC) -c -pic $(CFLAGS) $(CPPFLAGS) $< -o $$$$.o \
	    || (rm -f $$$$.o; exit 1)
	mv $$$$.o $@

#installed_library::	$(LIBDIR)/$(LIB_SO)
#	@if [ -z "$(SA_OBJS)" ]; then \
#	    :; \
#	else \
#	    $(MAKE) $(MFLAGS) $(LIBDIR)/$(LIB_SA); \
#	fi

$(LIBDIR)/$(LIB_SO):	$(LIBDIR) $(LIB_SO)
	$(INSTALL_DATA) $(LIB_SO) $(LIBDIR)/$(LIB_SO)
$(LIBDIR)/$(LIB_SA):	$(LIBDIR) $(LIB_SA)
	$(INSTALL_DATA) $(LIB_SA) $(LIBDIR)/$(LIB_SA)


############################################################################
# Programs:
############################################################################
BINDIR		= $(exec_prefix)

programs:	$(PROGRAMS)
program:
	@if [ -z "$(PROGRAM)" ]; then \
	    : true; \
	else \
	    if [ -z "$(LIBRARY)" ]; then \
		: true; \
	    else \
		$(MAKE) $(MFLAGS) library; \
	    fi; \
	    $(MAKE) $(MFLAGS) $(PROGRAM); \
	fi

dummy_prog $(PROGRAM):	$(OBJS)
	@if [ -r $@.f -o -r $@.F ]; then \
	    ld='$(FC) $(FFLAGS)'; \
	else \
	    ld='$(CC) $(CFLAGS)'; \
	fi; \
	cmd="$$ld $(OBJS) $(LIBS) -o $@"; \
	echo $$cmd; \
	$$cmd \
	    || (rm -f $@; exit 1)

installed_programs:	FORCE
	@$(MAKE) $(MFLAGS) installed_binfiles \
	    BINDIR=$(BINDIR) BINFILES="$(PROGRAMS)"

installed_binfiles:	FORCE
	@if [ -n "$(BINFILES)" ]; then \
	    binfiles="$(BINFILES)"; \
	    for binfile in $$binfiles; do \
		$(MAKE) $(MFLAGS) BINDIR=$(BINDIR) BINFILE=$$binfile \
		    installed_binfile || exit 1; \
	    done; \
	else \
	    :; \
	fi

installed_program:	FORCE
	@$(MAKE) $(MFLAGS) BINDIR=$(BINDIR) BINFILE=$(PROGRAM) \
	    installed_binfile

# The following target decouples the previous targets from the actual rule;
# thus, eliminating extraneous make(1) messages.
#
installed_binfile:	$(BINDIR)/$(BINFILE)

$(BINDIR)/$(BINFILE):	$(BINDIR) $(BINFILE)
	$(INSTALL_PROGRAM) $(BINFILE) $@

dummy_bindir $(BINDIR):
	mkdir -p $@
	chmod u+rwx,g+rws,o=rx $@


############################################################################
# Header files:
############################################################################
installed_headers:	FORCE
	@if [ -n "$(HEADERS)" ]; then \
	    headers="$(HEADERS)"; \
	    for header in $$headers; do \
		$(MAKE) $(MFLAGS) INCDIR=$(INCDIR) HEADER=$$header \
		    installed_header || exit 1; \
	    done; \
	else \
	    :; \
	fi

# The following target decouples the previous targets from the actual rule;
# thus, eliminating extraneous make(1) messages.
#
installed_header:	$(INCDIR)/$(HEADER)

$(INCDIR)/$(HEADER):	$(INCDIR) $(HEADER)
	$(INSTALL_DATA) $(HEADER) $@

dummy_incdir $(INCDIR):
	mkdir -p $@
	chmod u+rwx,g+rws,o=rx $@


############################################################################
# `etc/' files:
############################################################################
ETCDIR	= $(prefix)/etc

installed_etcfiles:
	@if [ -n "$(ETCFILES)" ]; then \
	    etcfiles="$(ETCFILES)"; \
	    for etcfile in $$etcfiles; do \
		$(MAKE) $(MFLAGS) ETCDIR=$(ETCDIR) ETCFILE=$$etcfile \
		    installed_etcfile || exit 1; \
	    done; \
	else \
	    :; \
	fi

# The following target decouples the previous targets from the actual rule;
# thus, eliminating extraneous make(1) messages.
#
installed_etcfile:	$(ETCDIR)/$(ETCFILE)

$(ETCDIR)/$(ETCFILE):	$(ETCDIR) $(ETCFILE)
	$(INSTALL_DATA) $(ETCFILE) $@

dummy_etcdir $(ETCDIR):
	mkdir -p $@
	chmod u+rwx,g+rws,o=rx $@


############################################################################
# Manual Pages:
############################################################################
MANDIR	= $(prefix)/man

installed_manuals:	FORCE
	@if [ -n "$(MANUALS)" ]; then \
	    manuals="$(MANUALS)"; \
	    for manual in $$manuals; do \
		mandir=$(MANDIR)/man`echo $$manual | sed 's/.*\.\(.\).*/\1/'`; \
		$(MAKE) $(MFLAGS) MANDIR=$$mandir MANUAL=$$manual \
		    installed_manual || exit 1; \
	    done; \
	else \
	    :; \
	fi

# The following target decouples the previous targets from the actual rule;
# thus, eliminating extraneous make(1) messages.
#
installed_manual:	$(MANDIR)/$(MANUAL)

# The first sed(1) editing command in the following replaces the expanded
# RCS keyword `$Date ...' in the `.TH' macro line with something more 
# appropriate to a manual page.
#
$(MANDIR)/$(MANUAL):	$(MANDIR) $(MANUAL)
	sed \
	 -e '/^\.TH/s/$$D\ate: \([0-9]*\)\/\([0-9]*\)\/\([0-9]*\).*\$$/\1-\2-\3/'\
	 $(MANUAL) | $(TBL) | $(NEQN) > $@

dummy_mandir $(MANDIR):
	mkdir -p $@
	chmod u+rwx,g+rws,o=rx $@


############################################################################
# Cleanup:
############################################################################
clean::
	rm -f $(GARBAGE) $(PROGRAM) a.out core *.o *.a *.so.* *.sa.* *.so \
	    *.log

realclean::	distclean
distclean::
	rm -f $(GARBAGE) $(PROGRAM) a.out core *.o *.a *.so.* *.sa.* *.so \
	    *.log Makefile config.status cc_id tags


############################################################################
# Configuration:
############################################################################

# The following target is touched because config.status won't update it if
# it hasn't changed.
#
# The following is commented-out because the HPUX make(1) goes into a loop.
#
#Makefile:	config.status
#	./config.status
#	touch $@

config.status:	cc_id configure
	./configure --no-create

cc_id:		FORCE
	if test -r $@ && test "`cat $@`" = "$(CC)"; then \
	    :; \
	else \
	    echo "$(CC)" > $@; \
	fi

configure:		configure.in
	autoconf


############################################################################
# Dependency maintenance:
############################################################################
.PRECIOUS:	depend
#
# Note: The following rule will only work if the C compiler behaves correctly
# when given the `-M' option.  AIX's cc(1) and SunOS's acc(1) behave
# incorrectly.  --Steve Emmerson 1992-09-24
#
# Furthermore, we do not attempt to maintain dependencies on installed
# header files (e.g. those in $(INCDIR)) for two reasons: 1) the dependency
# list generated at Unidata mightn't correspond to the situation at a client
# site; and 2) clients using broken compilers (c.f. previous note) can't
# regenerate the dependency list.
#
depend:		FORCE
	-incdir=`echo $(INCDIR) | sed s',/,\\\/,g'`; \
	cc -M $(CPPFLAGS) *.c 2> /dev/null | \
	    awk '{ \
		if ($$0 ~ /:/) { \
		    target	= $$1; \
		    if (target ~ /:/) { \
			target	= substr(target,1,length(target)-1); \
			start	= 2; \
		    } else { \
			start	= 3; \
		    } \
		} else { \
		    start	= 1; \
		} \
		for (i = start; i <= NF; ++i) { \
		    if ($$i !~ /^\\/ && \
			$$i !~ /port\// && \
			$$i !~ /^\//) \
			print target ": " $$i \
		} \
	    }' > $@
	tag='### DO NOT DELETE THIS LINE.  make depend DEPENDS ON IT ###'; \
	    sed -e "/^$$tag$$/r depend" \
		-e "/^$$tag$$/q" Makefile > Makefile.new
	mv Makefile.new Makefile


############################################################################
# Distributions:
############################################################################
FTPDIR		= /home/ftp/pub/sdm
VERSION		= 2.3

ftp:		$(FTPDIR)/$(PACKAGE)-$(VERSION).tar.Z
	test ! -h $(FTPDIR)/$(PACKAGE).tar.Z && exit 0; \
	cd $(FTPDIR) || exit 1; \
	rm $(PACKAGE).tar.Z || exit 1; \
	ln -s $(PACKAGE)-$(VERSION).tar.Z $(PACKAGE).tar.Z;

$(FTPDIR)/$(PACKAGE)-$(VERSION).tar.Z:	$(PACKAGE)-$(VERSION).tar.Z
	cp $(PACKAGE)-$(VERSION).tar.Z $@ 
	chmod u+rw,g+rw,o=r $@

tar.Z:		$(PACKAGE)-$(VERSION).tar.Z

$(PACKAGE)-$(VERSION).tar.Z:	MANIFEST
	dir=`basename \`pwd\`` \
	&& id=$(PACKAGE)-$(VERSION) \
	&& cd .. \
	&& ln -s $$dir $$id \
	&& tar -chof - `sed "s|^|$$id/|" $$id/MANIFEST` \
	    | compress > $$id/$@ \
	&& rm $$id

MANIFEST:	FORCE
	@echo 1>&2 Creating MANIFEST
	@$(MAKE) -s $(MFLAGS) MANIFEST.echo > $@

MANIFEST.echo:	FORCE
	@$(MAKE) -s $(MFLAGS) \
	    MANIFEST=`echo $(MANIFEST) | sed 's/MANIFEST//'` \
	    ensure_manifest > /dev/null
	@echo $(MANIFEST) | fmt -1
	@if [ -n "$(SUBDIRS)" ]; then \
	    subdirs="$(SUBDIRS)"; \
	    for subdir in $$subdirs; do \
		(cd $$subdir && \
		$(MAKE) $(MFLAGS) MANIFEST.echo | \
		    sed "s|^|$$subdir/|") || exit 1; \
	    done; \
	else \
	    :; \
	fi

ensure_manifest:	$(MANIFEST)


############################################################################
# Miscellaneous:
############################################################################

# The following dummy target is useful as a dependency to ensure that a
# rule is always executed.
#
FORCE:

# Override the default definition for ncgen(1) in the master makefile.
#
NCGEN		= ../ncgen/ncgen

### Everything after the following line might be overwritten ###
### DO NOT DELETE THIS LINE.  make depend DEPENDS ON IT ###
dumplib.o: dumplib.c
dumplib.o: ../libsrc/netcdf.h
dumplib.o: ./dumplib.h
ncdump.o: ncdump.c
ncdump.o: ../libsrc/netcdf.h
ncdump.o: ./ncdump.h
ncdump.o: ./dumplib.h
ncdump.o: ./vardata.h
p.o: p.c
test.o: test.c
test.o: ../libsrc/netcdf.h
var.o: var.c
vardata.o: vardata.c
vardata.o: ../libsrc/netcdf.h
vardata.o: ./ncdump.h
vardata.o: ./dumplib.h
vardata.o: ./vardata.h
vlist.o: vlist.c
vlist.o: ./vlist.h
x.o: x.c
x.o: ../libsrc/netcdf.h
