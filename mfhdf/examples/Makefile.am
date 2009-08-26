#############################################################################
##                      Setup for building programs                        ##
#############################################################################

include $(top_srcdir)/config/commence.am

INCLUDES=-I$(top_srcdir)/hdf/src        \
         -I$(top_srcdir)/mfhdf/libsrc   \
         -I$(top_srcdir)/mfhdf/port     \
         -I../libsrc
DEFINES=-DNDEBUG -DHDF
AM_CPPFLAGS=$(INCLUDES) $(DEFINES)

#############################################################################
##                          Programs to build                              ##
#############################################################################

bin_PROGRAMS = create_sds write_to_sds write_slab \
	alter_sds_values unlimited_sds compress_sds \
	mv_sds_to_external read_from_sds read_subsets get_info \
	find_sds_by_name set_get_dim_info dimscale_vs_sds \
	set_attr get_attr chunking_example

create_sds_SOURCES = create_sds.c
create_sds_LDADD = ../libsrc/libmfhdf.la $(top_builddir)/hdf/src/libdf.la @LIBS@
create_sds_DEPENDENCIES = $(top_builddir)/hdf/src/libdf.la ../libsrc/libmfhdf.la

write_to_sds_SOURCES = write_to_sds.c
write_to_sds_LDADD = ../libsrc/libmfhdf.la $(top_builddir)/hdf/src/libdf.la @LIBS@
write_to_sds_DEPENDENCIES = $(top_builddir)/hdf/src/libdf.la ../libsrc/libmfhdf.la

write_slab_SOURCES = write_slab.c
write_slab_LDADD = ../libsrc/libmfhdf.la $(top_builddir)/hdf/src/libdf.la @LIBS@
write_slab_DEPENDENCIES = $(top_builddir)/hdf/src/libdf.la ../libsrc/libmfhdf.la

alter_sds_values_SOURCES = alter_sds_values.c
alter_sds_values_LDADD = ../libsrc/libmfhdf.la $(top_builddir)/hdf/src/libdf.la @LIBS@
alter_sds_values_DEPENDENCIES = $(top_builddir)/hdf/src/libdf.la ../libsrc/libmfhdf.la

unlimited_sds_SOURCES = unlimited_sds.c
unlimited_sds_LDADD = ../libsrc/libmfhdf.la $(top_builddir)/hdf/src/libdf.la @LIBS@
unlimited_sds_DEPENDENCIES = $(top_builddir)/hdf/src/libdf.la ../libsrc/libmfhdf.la

compress_sds_SOURCES = compress_sds.c
compress_sds_LDADD = ../libsrc/libmfhdf.la $(top_builddir)/hdf/src/libdf.la @LIBS@
compress_sds_DEPENDENCIES = $(top_builddir)/hdf/src/libdf.la ../libsrc/libmfhdf.la

mv_sds_to_external_SOURCES = mv_sds_to_external.c
mv_sds_to_external_LDADD = ../libsrc/libmfhdf.la $(top_builddir)/hdf/src/libdf.la @LIBS@
mv_sds_to_external_DEPENDENCIES = $(top_builddir)/hdf/src/libdf.la ../libsrc/libmfhdf.la

read_from_sds_SOURCES = read_from_sds.c
read_from_sds_LDADD = ../libsrc/libmfhdf.la $(top_builddir)/hdf/src/libdf.la @LIBS@
read_from_sds_DEPENDENCIES = $(top_builddir)/hdf/src/libdf.la ../libsrc/libmfhdf.la

read_subsets_SOURCES = read_subsets.c
read_subsets_LDADD = ../libsrc/libmfhdf.la $(top_builddir)/hdf/src/libdf.la @LIBS@
read_subsets_DEPENDENCIES = $(top_builddir)/hdf/src/libdf.la ../libsrc/libmfhdf.la

get_info_SOURCES = get_info.c
get_info_LDADD = ../libsrc/libmfhdf.la $(top_builddir)/hdf/src/libdf.la @LIBS@
get_info_DEPENDENCIES = $(top_builddir)/hdf/src/libdf.la ../libsrc/libmfhdf.la

find_sds_by_name_SOURCES = find_sds_by_name.c
find_sds_by_name_LDADD = ../libsrc/libmfhdf.la $(top_builddir)/hdf/src/libdf.la @LIBS@
find_sds_by_name_DEPENDENCIES = $(top_builddir)/hdf/src/libdf.la ../libsrc/libmfhdf.la

set_get_dim_info_SOURCES = set_get_dim_info.c
set_get_dim_info_LDADD = ../libsrc/libmfhdf.la $(top_builddir)/hdf/src/libdf.la @LIBS@
set_get_dim_info_DEPENDENCIES = $(top_builddir)/hdf/src/libdf.la ../libsrc/libmfhdf.la

dimscale_vs_sds_SOURCES = dimscale_vs_sds.c
dimscale_vs_sds_LDADD = ../libsrc/libmfhdf.la $(top_builddir)/hdf/src/libdf.la @LIBS@
dimscale_vs_sds_DEPENDENCIES = $(top_builddir)/hdf/src/libdf.la ../libsrc/libmfhdf.la

set_attr_SOURCES = set_attr.c
set_attr_LDADD = ../libsrc/libmfhdf.la $(top_builddir)/hdf/src/libdf.la @LIBS@
set_attr_DEPENDENCIES = $(top_builddir)/hdf/src/libdf.la ../libsrc/libmfhdf.la

get_attr_SOURCES = get_attr.c
get_attr_LDADD = ../libsrc/libmfhdf.la $(top_builddir)/hdf/src/libdf.la @LIBS@
get_attr_DEPENDENCIES = $(top_builddir)/hdf/src/libdf.la ../libsrc/libmfhdf.la

chunking_example_SOURCES = chunking_example.c
chunking_example_LDADD = ../libsrc/libmfhdf.la $(top_builddir)/hdf/src/libdf.la @LIBS@
chunking_example_DEPENDENCIES = $(top_builddir)/hdf/src/libdf.la ../libsrc/libmfhdf.la

installcheck-local:
	$(MAKE) check;

#############################################################################
##                   Testing -- Here there be dragons.                     ##
#############################################################################

check_SCRIPTS = testexamples.sh

check:
	@$(SHELL) ./testexamples.sh

#############################################################################
##                          And the cleanup                                ##
#############################################################################

DISTCLEANFILES = *.o $(bin_PROGRAMS)
