INCLUDES=-I${top_srcdir}/hdf/src        \
         -I${top_srcdir}/mfhdf/libsrc   \
         -I${top_srcdir}/mfhdf/port
DEFINES=-DNDEBUG -DHDF

lib_LIBRARIES = libmfhdf.a
libmfhdf_a_SOURCES = array.c attr.c cdf.c dim.c file.c hdfsds.c         \
                     iarray.c error.c globdef.c mfsd.c nssdc.c putget.c \
                     putgetg.c sharray.c string.c var.c xdrposix.c      \
                     xdrstdio.c
libmfhdf_a_CPPFLAGS = ${INCLUDES} ${DEFINES} @CPPFLAGS@
include_HEADERS = hdf2netcdf.h local_nc.h mfhdf.h netcdf.h 
