#############################################################################
##                      Setup for building programs                        ##
#############################################################################

include $(top_srcdir)/config/commence.am

AM_CPPFLAGS = -I$(top_srcdir)/hdf/src

#############################################################################
##                              Testing                                    ##
#############################################################################

if HDF_BUILD_FORTRAN
TEST_PROG = testhdf buffer fortest
check_PROGRAMS = testhdf buffer fortest fortestF
else
TEST_PROG = testhdf buffer
check_PROGRAMS = testhdf buffer
endif

testhdf_SOURCES = an.c anfile.c bitio.c blocks.c chunks.c comp.c   \
                  conv.c extelt.c file.c file1.c litend.c macros.c man.c    \
                  mgr.c nbit.c rig.c sdmms.c sdnmms.c sdstr.c slab.c tbv.c  \
                  tattdatainfo.c tdatainfo.c tdfr8.c tdupimgs.c testhdf.c   \
		  tmgrattr.c tmgrcomp.c tree.c tszip.c tusejpegfuncs.c      \
		  tutils.c tvattr.c tvnameclass.c tvset.c tvsfpack.c vers.c
testhdf_LDADD = $(LIBHDF)
testhdf_DEPENDENCIES = testdir $(LIBHDF) 

buffer_LDADD = $(LIBHDF)
buffer_DEPENDENCIES = $(LIBHDF)

if HDF_BUILD_FORTRAN
fortest_SOURCES = fortest.c
fortest_LDADD = $(LIBHDF)
fortest_DEPENDENCIES = testdir fortestF$(EXEEXT) $(LIBHDF)

fortestF_SOURCES = forsupff.f forsupf.c fortestF.f manf.f mgrf.f slabwf.f   \
                   t24f.f tanf.f tanfilef.f tpf.f tr8f.f tsdmmsf.f          \
                   tsdnmmsf.f tsdnntf.f tsdntf.f tsdstrf.f tstubsf.f        \
                   tvattrf.f tvsetf.f
FFLAGS += $(AM_CPPFLAGS)
fortestF_LDADD = $(LIBHDF)
fortestF_DEPENDENCIES = testdir $(LIBHDF)
endif

testdir:
	-mkdir testdir

#############################################################################
##                          And the cleanup                                ##
#############################################################################

CHECK_CLEANFILES += fortest.arg Fortran_err.dat testdir/t5.hdf Tables_External_File

# Automake's distclean won't remove directories, so we can add an additional
# hook target which will do so during 'make distclean'.
distclean-local:
	-rm -rf testdir

DISTCLEANFILES = 

include $(top_srcdir)/config/conclude.am
