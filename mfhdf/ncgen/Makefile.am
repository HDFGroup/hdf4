#############################################################################
##                      Setup for building programs                        ##
#############################################################################

include $(top_srcdir)/config/commence.am

if HDF_BUILD_SHARED
    SHLIBLOC = $(top_builddir)/mfhdf/libsrc/.libs/libmfhdf.$(SHARED_EXTENSION) $(top_builddir)/hdf/src/.libs/libdf.$(SHARED_EXTENSION)
else
    SHLIBLOC = $(top_builddir)/mfhdf/libsrc/.libs/libmfhdf.a $(top_builddir)/hdf/src/.libs/libdf.a
endif

## Setup the different includes and preprocessor #defines we need.
ncgen_INCLUDES=-I$(top_srcdir)/hdf/src        \
         -I$(top_srcdir)/mfhdf/libsrc   \
         -I$(top_builddir)/mfhdf/libsrc
DEFINES=-DNDEBUG -DHDF
AM_CPPFLAGS=$(ncgen_INCLUDES) $(DEFINES)

## Add ncgen specific linker flags here
ncgen_LDFLAGS = $(LT_STATIC_EXEC) $(AM_LDFLAGS)

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
ncgen_LDADD = $(LIBMFHDF) $(LIBHDF)
ncgen_DEPENDENCIES = $(LIBMFHDF) $(LIBHDF)

ctest0_SOURCES=
ctest0_LDADD = $(LIBMFHDF) $(LIBHDF)

ftest0_SOURCES=

## Recipe for building the ncgentab.c file
##ncgentab.c ncgentab.h: ncgen.h ncgen.y ncgenyy.c
##	$(YACC) -d $(srcdir)/ncgen.y
##	mv y.tab.c ncgentab.c
##	mv y.tab.h ncgentab.h

##ncgenyy.c: ncgen.l
##	$(LEX) $(srcdir)/ncgen.l
##	mv lex.yy.c ncgenyy.c

#############################################################################
##                            Documentation                                ##
#############################################################################

man1_MANS = ncgen.1

#############################################################################
##                   Testing -- Here there be dragons.                     ##
#############################################################################
NCDUMP=$(top_builddir)/mfhdf/ncdump/ncdump

check_PROGRAMS = ctest0
if HDF_BUILD_NETCDF
if HDF_BUILD_FORTRAN
check_PROGRAMS += ftest0

netcdf.inc:
	ln -s $(top_builddir)/mfhdf/fortran/$@ .

ftest0$(EXEEXT):		ncgen$(EXEEXT) test0.cdl netcdf.inc
	$(TESTS_ENVIRONMENT) ./ncgen -f -o ftest0.nc $(srcdir)/test0.cdl > test0.f
	$(F77) $(FFLAGS) -o $@ test0.f $(LDFLAGS) $(SHLIBLOC) $(LIBS)
endif
endif

ctest0$(EXEEXT):		ncgen$(EXEEXT) $(srcdir)/test0.cdl
	$(TESTS_ENVIRONMENT) ./ncgen -c -o ctest0.nc $(srcdir)/test0.cdl > test0.c
	$(COMPILE) -c -o ctest0$(EXEEXT).o test0.c
	$(LINK) ctest0$(EXEEXT).o $(ctest0_LDADD) $(LDFLAGS) $(SHLIBLOC) $(LIBS)

TEST_SCRIPT = testncgen.sh
check_SCRIPTS = testncgen.sh

#############################################################################
##                            Miscellaneous                                ##
#############################################################################

#############################################################################
##                          And the cleanup                                ##
#############################################################################

CHECK_CLEANFILES += ctest1.cdl ftest1.cdl \
                 test0.nc test1.cdl test1.nc test2.cdl

DISTCLEANFILES = ctest0$(EXEEXT) ctest0.nc ftest0$(EXEEXT) ftest0.nc    \
                 netcdf.inc test0.c test0.f

include $(top_srcdir)/config/conclude.am
