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
gif2hdf_LDADD = $(LIBHDF)
gif2hdf_DEPENDENCIES = $(LIBHDF)

hdf2gif_SOURCES = hdf2gif.c hdfgifwr.c
hdf2gif_LDADD = $(LIBHDF)
hdf2gif_DEPENDENCIES = $(LIBHDF)

hdf2jpeg_SOURCES = hdf2jpeg.c
hdf2jpeg_LDADD = $(LIBHDF)
hdf2jpeg_DEPENDENCIES = $(LIBHDF)

hdf24to8_SOURCES = hdf24to8.c
hdf24to8_LDADD = $(LIBHDF)
hdf24to8_DEPENDENCIES = $(LIBHDF)

hdf8to24_SOURCES = hdf8to24.c
hdf8to24_LDADD = $(LIBHDF)
hdf8to24_DEPENDENCIES = $(LIBHDF)

hdfcomp_SOURCES = hdfcomp.c
hdfcomp_LDADD = $(LIBHDF)
hdfcomp_DEPENDENCIES = $(LIBHDF)

hdfed_SOURCES = he_cntrl.c he_disp.c he_file.c he_main.c
hdfed_LDADD = $(LIBHDF)
hdfed_DEPENDENCIES = $(LIBHDF)

hdfls_SOURCES = hdfls.c
hdfls_LDADD = $(LIBHDF)
hdfls_DEPENDENCIES = $(LIBHDF)

hdfpack_SOURCES = hdfpack.c
hdfpack_LDADD = $(LIBHDF)
hdfpack_DEPENDENCIES = $(LIBHDF)

hdftopal_SOURCES = hdftopal.c
hdftopal_LDADD = $(LIBHDF)
hdftopal_DEPENDENCIES = $(LIBHDF)

hdftor8_SOURCES = hdftor8.c
hdftor8_LDADD = $(LIBHDF)
hdftor8_DEPENDENCIES = $(LIBHDF)

hdfunpac_SOURCES = hdfunpac.c
hdfunpac_LDADD = $(LIBHDF)
hdfunpac_DEPENDENCIES = $(LIBHDF)

jpeg2hdf_SOURCES = jpeg2hdf.c
jpeg2hdf_LDADD = $(LIBHDF)
jpeg2hdf_DEPENDENCIES = $(LIBHDF)

paltohdf_SOURCES = paltohdf.c
paltohdf_LDADD = $(LIBHDF)
paltohdf_DEPENDENCIES = $(LIBHDF)

r8tohdf_SOURCES = r8tohdf.c
r8tohdf_LDADD = $(LIBHDF)
r8tohdf_DEPENDENCIES = $(LIBHDF)

ristosds_SOURCES = ristosds.c
ristosds_LDADD = $(LIBHDF)
ristosds_DEPENDENCIES = $(LIBHDF)

vmake_SOURCES = vmake.c
vmake_LDADD = $(LIBHDF)
vmake_DEPENDENCIES = $(LIBHDF)

vshow_SOURCES = vshow.c
vshow_LDADD = ../src/libdf.la
vshow_DEPENDENCIES = ../src/libdf.la

#############################################################################
##                   Testing -- Here there be dragons.                     ##
#############################################################################

TEST_SCRIPT = testutil.sh
check_SCRIPTS = testutil.sh
check_PROGRAMS = $(bin_PROGRAMS)

#############################################################################
##                          And the cleanup                                ##
#############################################################################

# Automake's distclean won't remove directories, so we can add an additional
# hook target which will do so during 'make distclean'.
distclean-local:
	-rm -rf gif_hdf_temp

DISTCLEANFILES = gif_hdf_temp/bttrfly.hdf gif_hdf_temp/skull.gif    \
                 gif_hdf_temp/SunWheel.hdf

include $(top_srcdir)/config/conclude.am
                 