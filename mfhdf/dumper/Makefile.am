#############################################################################
##                      Setup for building programs                        ##
#############################################################################

include $(top_srcdir)/config/commence.am

hdp_INCLUDES=-I$(top_srcdir)/hdf/src        \
         -I$(top_srcdir)/mfhdf/libsrc   \
         -I$(top_builddir)/mfhdf/libsrc
DEFINES=-DNDEBUG -DHDF
AM_CPPFLAGS=$(hdp_INCLUDES) $(DEFINES)

## Add hdp specific linker flags here
hdp_LDFLAGS = $(LT_STATIC_EXEC) $(AM_LDFLAGS)

#############################################################################
##                          Programs to build                              ##
#############################################################################

bin_PROGRAMS = hdp

## Information for building the "hdp" program
hdp_SOURCES = hdp.c hdp_dump.c hdp_gr.c hdp_list.c hdp_rig.c hdp_sds.c      \
              hdp_util.c hdp_vd.c hdp_vg.c show.c
hdp_LDADD = $(LIBMFHDF) $(LIBHDF) @LIBS@
hdp_DEPENDENCIES = $(LIBMFHDF) $(LIBHDF)

#############################################################################
##                   Testing -- Here there be dragons.                     ##
#############################################################################

TEST_SCRIPT = testhdp.sh
check_SCRIPTS = testhdp.sh

include $(top_srcdir)/config/conclude.am
