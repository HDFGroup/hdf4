#############################################################################
##                      Setup for building programs                        ##
#############################################################################

include $(top_srcdir)/config/commence.am

AM_CPPFLAGS=-I$(top_srcdir)/hdf/src

#############################################################################
##                              Testing                                    ##
#############################################################################

if HDF_BUILD_FORTRAN
TESTS = testhdf buffer fortest
check_PROGRAMS = testhdf buffer fortest fortestF
else
TESTS = testhdf buffer
check_PROGRAMS = testhdf buffer
endif

testhdf_SOURCES = an.c anfile.c bitio.c blocks.c chunks.c comp.c   \
                  conv.c extelt.c file.c file1.c litend.c macros.c man.c    \
                  mgr.c nbit.c rig.c sdmms.c sdnmms.c sdstr.c slab.c tbv.c  \
                  tdupimgs.c testhdf.c tree.c tszip.c tvattr.c tvset.c      \
		  tvsfpack.c vers.c
testhdf_LDADD = ../src/libdf.la
testhdf_DEPENDENCIES = copyfiles testdir ../src/libdf.la 

buffer_LDADD = ../src/libdf.la
buffer_DEPENDENCIES = ../src/libdf.la

if HDF_BUILD_FORTRAN
fortest_SOURCES = fortest.c
fortest_LDADD = ../../mfhdf/libsrc/libmfhdf.la ../src/libdf.la
fortest_DEPENDENCIES = copyfiles testdir fortestF$(EXEEXT) ../../mfhdf/libsrc/libmfhdf.la ../src/libdf.la

fortestF_SOURCES = forsupff.f forsupf.c fortestF.f manf.f mgrf.f slabwf.f   \
                   t24f.f tanf.f tanfilef.f tpf.f tr8f.f tsdmmsf.f          \
                   tsdnmmsf.f tsdnntf.f tsdntf.f tsdstrf.f tstubsf.f        \
                   tvattrf.f tvsetf.f
fortestF_LDADD = ../../mfhdf/libsrc/libmfhdf.la ../src/libdf.la
fortestF_DEPENDENCIES = copyfiles testdir ../../mfhdf/libsrc/libmfhdf.la ../src/libdf.la
endif

# Copy the data files (*.dat) to the build directory and make sure they have rw permissions. Do this to
# avoid modifying *.dat files in the source directory when running the tests.
copyfiles:
	-cp $(top_srcdir)/hdf/test/*.dat ./
	-chmod 666 ./*.dat

testdir:
	-mkdir testdir
#############################################################################
##                          And the cleanup                                ##
#############################################################################

DISTCLEANFILES = fortest.arg Fortran_err.dat 8bit.dat bitio.dat greyjpeg.dat gr_r24.dat jpeg.dat \
	litend.dat nbit.dat tmgr.dat tvattr.dat testdir/t5.hdf
