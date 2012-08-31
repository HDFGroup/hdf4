#############################################################################
##                      Setup for building programs                        ##
#############################################################################

include $(top_srcdir)/config/commence.am

if HDF_BUILD_SHARED
    SHLIBLOC = ../libsrc/.libs/libmfhdf.$(SHARED_EXTENSION) $(top_builddir)/hdf/src/.libs/libdf.$(SHARED_EXTENSION)
else
    SHLIBLOC = ../libsrc/.libs/libmfhdf.a $(top_builddir)/hdf/src/.libs/libdf.a
endif

## Setup the different includes and preprocessor #defines we need.
INCLUDES=-I$(top_srcdir)/hdf/src        \
         -I$(top_srcdir)/mfhdf/libsrc   \
         -I../libsrc
DEFINES=-DNDEBUG -DHDF
AM_CPPFLAGS=$(INCLUDES) $(DEFINES)

#############################################################################
##                          Programs to build                              ##
#############################################################################

bin_PROGRAMS = ncgen

if HDF_BUILD_NETCDF
if HDF_BUILD_FORTRAN
noinst_PROGRAMS = ctest0 ftest0
else 
noinst_PROGRAMS = ctest0
endif
else
noinst_PROGRAMS = ctest0
endif

## Information for building the "ncgen" program
ncgen_SOURCES = close.c escapes.c generate.c genlib.c getfill.c init.c      \
                load.c main.c ncgentab.c
ncgen_LDADD = ../libsrc/libmfhdf.la $(top_builddir)/hdf/src/libdf.la
ncgen_DEPENDENCIES = $(top_builddir)/hdf/src/libdf.la ../libsrc/libmfhdf.la

ctest0_SOURCES=
ctest0_LDADD = ../libsrc/libmfhdf.la $(top_builddir)/hdf/src/libdf.la

ftest0_SOURCES=

## Recipe for building the ncgentab.c file
ncgentab.c ncgentab.h: ncgen.h ncgen.y ncgenyy.c
	$(YACC) -d $(srcdir)/ncgen.y
	mv y.tab.c ncgentab.c
	mv y.tab.h ncgentab.h

ncgenyy.c: ncgen.l
	$(LEX) $(srcdir)/ncgen.l
	mv lex.yy.c ncgenyy.c

#############################################################################
##                            Documentation                                ##
#############################################################################

man1_MANS = ncgen.1

#############################################################################
##                   Testing -- Here there be dragons.                     ##
#############################################################################
TESTS = testncgen.sh
check_SCRIPTS = testncgen.sh

NCDUMP=../ncdump/ncdump

check_PROGRAMS = ctest0
if HDF_BUILD_NETCDF
if HDF_BUILD_FORTRAN
check_PROGRAMS += ftest0

netcdf.inc:
	ln -s ../fortran/$@ .

ftest0$(EXEEXT):		ncgen$(EXEEXT) test0.cdl netcdf.inc
	$(TESTS_ENVIRONMENT) ./ncgen -f -o ftest0.nc $(srcdir)/test0.cdl > test0.f
	$(F77) $(FFLAGS) -o $@ test0.f $(LDFLAGS) $(SHLIBLOC) $(LIBS)
endif
endif

ctest0$(EXEEXT):		ncgen$(EXEEXT) $(srcdir)/test0.cdl
	$(TESTS_ENVIRONMENT) ./ncgen -c -o ctest0.nc $(srcdir)/test0.cdl > test0.c
	$(COMPILE) -c -o ctest0$(EXEEXT).o test0.c
	$(LINK) ctest0$(EXEEXT).o $(ctest0_LDADD) $(LDFLAGS) $(SHLIBLOC) $(LIBS)

#############################################################################
##                            Miscellaneous                                ##
#############################################################################

## This was in the original Makefile.
vms-stuff:	ncgentab.h ncgentab.c ncgenyy.c
	cp $(srcdir)/ncgentab.h vmstab.h
	cp $(srcdir)/ncgentab.c vmstab.c
	cp $(srcdir)/ncgenyy.c vms_yy.c

#include $(top_srcdir)/config/conclude.am

VERBOSE = debug

TEST_EXTENSIONS = .sh
SH_LOG_COMPILER = $(SHELL)
AM_SH_LOG_FLAGS =

#############################################################################
##                          And the cleanup                                ##
#############################################################################

DISTCLEANFILES = ctest0$(EXEEXT) ctest0.nc ctest1.cdl ftest0$(EXEEXT) ftest0.nc ftest1.cdl    \
                 ncgentab.c ncgentab.h ncgenyy.c netcdf.inc test0.c test0.f \
                 test0.nc test1.cdl test1.nc test2.cdl
