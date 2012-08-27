#############################################################################
##                      Setup for building programs                        ##
#############################################################################

include $(top_srcdir)/config/commence.am

## Setup the different includes and preprocessor #defines we need.
AM_CPPFLAGS=-I$(top_srcdir)/hdf/src

#############################################################################
##                          Programs to build                              ##
#############################################################################

bin_PROGRAMS = gif2hdf hdf2gif hdf2jpeg hdf24to8 hdf8to24 hdfcomp hdfed     \
               hdfls hdfpack hdftopal hdftor8 hdfunpac jpeg2hdf paltohdf    \
               r8tohdf ristosds vmake vshow 

if HDF_BUILD_FORTRAN
bin_SCRIPTS = h4redeploy h4cc h4fc
else
bin_SCRIPTS = h4redeploy h4cc
endif

gif2hdf_SOURCES = gif2hdf.c gif2mem.c gifread.c decompress.c writehdf.c
gif2hdf_LDADD = ../src/libdf.la
gif2hdf_DEPENDENCIES = ../src/libdf.la

hdf2gif_SOURCES = hdf2gif.c hdfgifwr.c
hdf2gif_LDADD = ../src/libdf.la
hdf2gif_DEPENDENCIES = ../src/libdf.la

hdf2jpeg_SOURCES = hdf2jpeg.c
hdf2jpeg_LDADD = ../src/libdf.la
hdf2jpeg_DEPENDENCIES = ../src/libdf.la

hdf24to8_SOURCES = hdf24to8.c
hdf24to8_LDADD = ../src/libdf.la
hdf24to8_DEPENDENCIES = ../src/libdf.la

hdf8to24_SOURCES = hdf8to24.c
hdf8to24_LDADD = ../src/libdf.la
hdf8to24_DEPENDENCIES = ../src/libdf.la

hdfcomp_SOURCES = hdfcomp.c
hdfcomp_LDADD = ../src/libdf.la
hdfcomp_DEPENDENCIES = ../src/libdf.la

hdfed_SOURCES = he_cntrl.c he_disp.c he_file.c he_main.c
hdfed_LDADD = ../src/libdf.la
hdfed_DEPENDENCIES = ../src/libdf.la

hdfls_SOURCES = hdfls.c
hdfls_LDADD = ../src/libdf.la
hdfls_DEPENDENCIES = ../src/libdf.la

hdfpack_SOURCES = hdfpack.c
hdfpack_LDADD = ../src/libdf.la
hdfpack_DEPENDENCIES = ../src/libdf.la

hdftopal_SOURCES = hdftopal.c
hdftopal_LDADD = ../src/libdf.la
hdftopal_DEPENDENCIES = ../src/libdf.la

hdftor8_SOURCES = hdftor8.c
hdftor8_LDADD = ../src/libdf.la
hdftor8_DEPENDENCIES = ../src/libdf.la

hdfunpac_SOURCES = hdfunpac.c
hdfunpac_LDADD = ../src/libdf.la
hdfunpac_DEPENDENCIES = ../src/libdf.la

jpeg2hdf_SOURCES = jpeg2hdf.c
jpeg2hdf_LDADD = ../src/libdf.la
jpeg2hdf_DEPENDENCIES = ../src/libdf.la

paltohdf_SOURCES = paltohdf.c
paltohdf_LDADD = ../src/libdf.la
paltohdf_DEPENDENCIES = ../src/libdf.la

r8tohdf_SOURCES = r8tohdf.c
r8tohdf_LDADD = ../src/libdf.la
r8tohdf_DEPENDENCIES = ../src/libdf.la

ristosds_SOURCES = ristosds.c
ristosds_LDADD = ../src/libdf.la
ristosds_DEPENDENCIES = ../src/libdf.la

vmake_SOURCES = vmake.c
vmake_LDADD = ../src/libdf.la
vmake_DEPENDENCIES = ../src/libdf.la

vshow_SOURCES = vshow.c
vshow_LDADD = ../src/libdf.la
vshow_DEPENDENCIES = ../src/libdf.la

#############################################################################
##                   Testing -- Here there be dragons.                     ##
#############################################################################

TESTS = testutil.sh
check_SCRIPTS = testutil.sh
check_PROGRAMS = $(bin_PROGRAMS)

#############################################################################
##                          And the cleanup                                ##
#############################################################################

# Automake's distclean won't remove directories, so we can add an additional
# hook target which will do so during 'make distclean'.
distclean-local:
	-rm -rf gif_hdf_temp

include $(top_srcdir)/config/conclude.am

DISTCLEANFILES = gif_hdf_temp/bttrfly.hdf gif_hdf_temp/skull.gif    \
                 gif_hdf_temp/SunWheel.hdf
                 