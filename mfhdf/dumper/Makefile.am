bin_PROGRAMS = hdp

INCLUDES=-I${top_srcdir}/hdf/src        \
         -I${top_srcdir}/mfhdf/libsrc   \
         -I${top_srcdir}/mfhdf/port     \
         -I../libsrc
DEFINES=-DNDEBUG -DHDF

hdp_SOURCES = hdp.c hdp_dump.c hdp_gr.c hdp_list.c hdp_rig.c hdp_sds.c      \
              hdp_util.c hdp_vd.c hdp_vg.c show.c
hdp_CPPFLAGS = ${INCLUDES} ${DEFINES} @CPPFLAGS@
hdp_LDADD = ../libsrc/libmfhdf.a ${top_builddir}/hdf/src/libdf.a @LIBS@
hdp_DEPENPENDENCIES = ${top_builddir}/hdf/src/libdf.a ../libsrc/libmfhdf.a

check: hdp
	sh testhdp.sh
