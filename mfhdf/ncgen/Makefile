# Generated automatically from Makefile.in by configure.
# Makefile for ncgen(1).
#
# $Id$

PROGRAM		= ncgen
BINFILES	= $(PROGRAM)
CPP_NETCDF	= -I../libsrc
CPPFLAGS	= $(CPP_NETCDF) -DNDEBUG -DHDF -DNO_HAVE_PROTOTYPES -Dconst= -DNO_STDARG -DNO_STRERROR -I/hdf/home/chouck/hdf/33/include -DSUN
CFLAGS		= -g
FFLAGS		= 
MANUALS		= ncgen.1
GARBAGE		= $(PROGRAM) \
		  ncgenyy.c ncgentab.c ncgentab.h \
		  test0.nc test1.nc test1.cdl test2.cdl test0.f ctest0 \
		  ftest0 ftest0.nc ftest1.cdl test0.c ctest0.nc ctest1.cdl
vms_stuff	= vmstab.c vmstab.h vms_yy.c
MANIFEST	= Makefile.in depend README \
		  close.c descrip.mms escapes.c generate.c generic.h \
		  genlib.c genlib.h getfill.c init.c lexyacc.com load.c \
		  main.c make.com msoft.mk msofttab.c msofttab.h \
		  msoftyy.c ncgen.1 ncgen.h ncgen.l ncgen.opt ncgen.y \
		  test.com test0.cdl \
		  $(vms_stuff)
LIBNAME		= netcdf
LD_NETCDF	= ../libsrc/libnetcdf.a /hdf/home/chouck/hdf/33/lib/libdf.a
OBJS		= main.o generate.o load.o ncgentab.o escapes.o \
		  getfill.o init.o close.o genlib.o
LD_XDR		= 
LIBS		= $(LD_NETCDF) $(LD_XDR)
LEX		= lex
YACC		= yacc
prefix		= /nfs/hdf/home/chouck/hdf

all::		$(PROGRAM)

test:           $(PROGRAM) test0.cdl btest ctest ftest FORCE

install::	installed_program installed_manuals

$(PROGRAM):	main.o

ncgentab.c \
ncgentab.h:	ncgen.y ncgenyy.c ncgen.h
	$(YACC) -d ncgen.y
	mv y.tab.c ncgentab.c
	mv y.tab.h ncgentab.h

ncgenyy.c:	ncgen.l
	$(LEX) ncgen.l
	mv lex.yy.c ncgenyy.c

# The SunOS 4.1.3 yacc(1) doesn't generate correct code.
#
vms-stuff:	ncgentab.h ncgentab.c ncgenyy.c
	cp ncgentab.h vmstab.h
	cp ncgentab.c vmstab.c
	cp ncgenyy.c vms_yy.c

#
# test "-b" option of ncgen
#
btest:		$(PROGRAM) test0.cdl test1.cdl
	./$(PROGRAM) -b test1.cdl
	$(NCDUMP) test1.nc > test2.cdl
	@diff test1.cdl test2.cdl && \
	    echo "*** $(PROGRAM) -b test successful ***"

#
# test "-c" option of ncgen
#
ctest:		test1.cdl ctest0
	./ctest0		# tests `-c' option, creates ctest0.nc
	$(NCDUMP) -n test1 ctest0.nc > ctest1.cdl
	@diff test1.cdl ctest1.cdl && \
	    echo "*** $(PROGRAM) -c test successful ***"

ctest0:		ncgen test0.cdl
	./$(PROGRAM) -c -o ctest0.nc test0.cdl > test0.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -o $@ test0.c $(LIBS)

#
# test "-f" option of ncgen
#
ftest:		test1.cdl
	@case "$(FC)" in \
	NONE*) \
	    echo 1>&2 "\`$@' not made because no FORTRAN compiler";; \
	*) \
	    $(MAKE) $(MFLAGS) ftest0; \
	    ./ftest0 ; \
	    $(NCDUMP) -n test1 ftest0.nc > ftest1.cdl; \
	    if diff test1.cdl ftest1.cdl; then \
		echo "*** ncgen -f test successful ***"; \
	    else \
		echo "*** ncgen -f test failed " \
		    "(but roundoff differences are OK) ***"; \
	    fi;; \
	esac

ftest0:		$(PROGRAM) test0.cdl
	@case "$(FC)" in \
	NONE*) \
	    echo 1>&2 "\`$@' not made because no FORTRAN compiler";; \
	*) \
	    $(MAKE) $(MFLAGS) netcdf.inc; \
	    ./$(PROGRAM) -f -o ftest0.nc test0.cdl > test0.f; \
	    $(FC) $(FFLAGS) -o $@ test0.f $(LIBS);; \
	esac

test1.cdl:	test0.nc
	$(NCDUMP) -n test1 test0.nc > $@

test0.nc:	$(PROGRAM) test0.cdl
	./$(PROGRAM) -b -o test0.nc test0.cdl

netcdf.inc:
	@case "$(FC)" in \
	NONE*) \
	    echo 1>&2 "\`$@' not made because no FORTRAN compiler";; \
	*) \
	    ln -s ../fortran/$@ .;; \
	esac

# Generated automatically from master.mk.in by configure.
# This is the master makefile.  It contains general rules.
#
# $Id$


.SUFFIXES:
.SUFFIXES:	.so .o .i .c .fc .f .F .l .y

############################################################################
# General Macros:
############################################################################
prefix		= /nfs/hdf/home/chouck/hdf
exec_prefix	= /nfs/hdf/home/chouck/hdf/bin
AR		= ar
CC		= cc
CPP		= cc -E
INCDIR		= $(prefix)/include
INSTALL		= /bin/install -c
INSTALL_DATA	= $(INSTALL) -m 664
INSTALL_PROGRAM	= $(INSTALL)
FC		= f77
MAKEINFO        = 
NCDUMP		= @NCDUMP@
NCGEN		= @NCGEN@
NEQN		= neqn
OS		= sunos
SHELL		= /bin/sh
TBL		= tbl
WHICH		= which

############################################################################
# Recursive Macros (command-line changes to others aren't supported):
############################################################################
SUBDIR_MACROS = CC="$(CC)" \
	CPP="$(CPP)" \
	exec_prefix="$(exec_prefix)" \
	FC="$(FC)" \
	prefix="$(prefix)"

LOCAL_MACROS  = CC="$(CC)" \
	CFLAGS="$(CFLAGS)" \
	CPP="$(CPP)" \
	CPPFLAGS="$(CPPFLAGS)" \
	exec_prefix="$(exec_prefix)" \
	FC="$(FC)" \
	prefix="$(prefix)"

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
	    $(MAKE) $(MFLAGS) $(SUBDIR_MACROS) \
		prefix=$(prefix) exec_prefix=$(exec_prefix) \
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
		$(MAKE) $(MFLAGS) $(LOCAL_MACROS) \
		    LIBDIR=$(LIBDIR) LIBFILE=$$libfile \
		    installed_libfile || exit 1; \
	    done; \
	else \
	    :; \
	fi

installed_library::	FORCE
	@$(MAKE) $(MFLAGS) $(LOCAL_MACROS) \
	    LIBDIR=$(LIBDIR) LIBFILE=$(LIB_A) \
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
		$(MAKE) $(MFLAGS) $(LOCAL_MACROS) library; \
	    fi; \
	    $(MAKE) $(MFLAGS) $(LOCAL_MACROS) $(PROGRAM); \
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
	@$(MAKE) $(MFLAGS) $(LOCAL_MACROS) installed_binfiles \
	    BINDIR=$(BINDIR) BINFILES="$(PROGRAMS)"

installed_binfiles:	FORCE
	@if [ -n "$(BINFILES)" ]; then \
	    binfiles="$(BINFILES)"; \
	    for binfile in $$binfiles; do \
		$(MAKE) $(MFLAGS) $(LOCAL_MACROS) \
		    BINDIR=$(BINDIR) BINFILE=$$binfile \
		    installed_binfile || exit 1; \
	    done; \
	else \
	    :; \
	fi

installed_program:	FORCE
	@$(MAKE) $(MFLAGS) $(LOCAL_MACROS) BINDIR=$(BINDIR) BINFILE=$(PROGRAM) \
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
		$(MAKE) $(MFLAGS) $(LOCAL_MACROS) INCDIR=$(INCDIR) HEADER=$$header \
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
		$(MAKE) $(MFLAGS) $(LOCAL_MACROS) ETCDIR=$(ETCDIR) ETCFILE=$$etcfile \
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
		$(MAKE) $(MFLAGS) $(LOCAL_MACROS) MANDIR=$$mandir MANUAL=$$manual \
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
# TeX-Info Pages:
############################################################################
INFODIR       = $(prefix)/info

installed_infos:      FORCE
	if [ -z "$(MAKEINFO)" ]; then \
		echo 1>&2 "Can't install info(1) pages because no makeinfo(1)"; \
	else \
	  if [ -n "$(INFOS)" ]; then \
	      infos="$(INFOS)"; \
	      for info in $$infos; do \
		$(MAKE) $(MFLAGS) $(LOCAL_MACROS) INFODIR=$(INFODIR) INFO=$$info \
		installed_info || exit 1; \
	      done; \
	   else \
	      :; \
	   fi; \
	fi;
# The following target decouples the previous targets from the actual rule;
# thus, eliminating extraneous make(1) messages.
#
installed_info:       $(INFODIR)/$(INFO)

$(INFODIR)/$(INFO):   $(INFODIR) $(INFO)
	$(INSTALL_DATA) $(INFO) $@

dummy_infodir $(INFODIR):
	mkdir -p $@
	chmod u+rwx,g+rws,o=rx $@

# Rules for generating info pages:
#
.SUFFIXES:    .info .texi .texinfo .txn
.texi.info:
	@if [ -z "$(MAKEINFO)" ]; then \
	  echo 1>&2 "Can't create $@ because no makeinfo(1)"; \
	else \
	  $(MAKEINFO) $< --no-split --output=$@; \
	fi
.texinfo.info:
	@if [ -z "$(MAKEINFO)" ]; then \
	  echo 1>&2 "Can't create $@ because no makeinfo(1)"; \
	else \
	  $(MAKEINFO) $< --no-split --output=$@; \
	fi
.txn.info:
	@if [ -z "$(MAKEINFO)" ]; then \
	  echo 1>&2 "Can't create $@ because no makeinfo(1)"; \
	else \
	  $(MAKEINFO) $< --no-split --output=$@; \
	fi

# Ensure that info page depends on canonical TeX-Info macros.
#
dummy_info $(INFO):   texinfo.tex

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
VERSION		= 2.3.2

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
	@$(MAKE) -s $(MFLAGS) $(LOCAL_MACROS) MANIFEST.echo > $@

MANIFEST.echo:	FORCE
	@$(MAKE) -s $(MFLAGS) $(LOCAL_MACROS) \
	    MANIFEST=`echo $(MANIFEST) | sed 's/MANIFEST//'` \
	    ensure_manifest > /dev/null
	@echo $(MANIFEST) | fmt -1
	@if [ -n "$(SUBDIRS)" ]; then \
	    subdirs="$(SUBDIRS)"; \
	    for subdir in $$subdirs; do \
		(cd $$subdir && \
	        echo 1>&2 Creating $@ in `pwd` && \
		$(MAKE) $(MFLAGS) $(LOCAL_MACROS) MANIFEST.echo | \
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

# Override the default definition for ncdump(1) in the master makefile.
#
NCDUMP		= ../ncdump/ncdump

### Everything after the following line might be overwritten ###
### DO NOT DELETE THIS LINE.  make depend DEPENDS ON IT ###
close.o: close.c
close.o: ./ncgen.h
close.o: ../libsrc/netcdf.h
close.o: ./generic.h
close.o: ./genlib.h
derror.o: derror.c
emalloc.o: emalloc.c
escapes.o: escapes.c
fstrcat.o: fstrcat.c
generate.o: generate.c
generate.o: ./ncgen.h
generate.o: ../libsrc/netcdf.h
generate.o: ./generic.h
generate.o: ./genlib.h
genlib.o: genlib.c
genlib.o: ./genlib.h
getfill.o: getfill.c
getfill.o: ../libsrc/netcdf.h
getfill.o: ./generic.h
getfill.o: ./genlib.h
init.o: init.c
init.o: ./ncgen.h
init.o: ../libsrc/netcdf.h
init.o: ./generic.h
load.o: load.c
load.o: ./ncgen.h
load.o: ../libsrc/netcdf.h
load.o: ./generic.h
load.o: ./genlib.h
main.o: main.c
main.o: ./genlib.h
msofttab.o: msofttab.c
msofttab.o: ./ncgen.h
msofttab.o: ../libsrc/netcdf.h
msofttab.o: ./generic.h
msofttab.o: ./ncgenyy.c
msofttab.o: ./ncgentab.h
msoftyy.o: msoftyy.c
msoftyy.o: ./ncgentab.h
ncgentab.o: ncgentab.c
ncgentab.o: ./ncgen.h
ncgentab.o: ../libsrc/netcdf.h
ncgentab.o: ./generic.h
ncgentab.o: ./ncgenyy.c
ncgentab.o: ./ncgentab.h
ncgenyy.o: ncgenyy.c
ncgenyy.o: ./ncgentab.h
test0.o: test0.c
test0.o: ../libsrc/netcdf.h
vms_yy.o: vms_yy.c
vms_yy.o: ./ncgentab.h
vmstab.o: vmstab.c
vmstab.o: ./ncgen.h
vmstab.o: ../libsrc/netcdf.h
vmstab.o: ./generic.h
vmstab.o: ./ncgenyy.c
vmstab.o: ./ncgentab.h
