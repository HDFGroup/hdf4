#
# HDF Java native interface (JNI) Library Makefile(.in)
#

include $(top_srcdir)/config/commence.am

# Mark this directory as part of the JNI API
JAVA_API=yes
AM_CFLAGS=-DNDEBUG -fPIC

# Include src directory and JNI flags
AM_CPPFLAGS=-I$(top_srcdir)/hdf/src        \
         -I$(top_srcdir)/mfhdf/libsrc   \
         -I$(top_builddir)/mfhdf/libsrc -I$(top_srcdir)/java/src/jni $(JNIFLAGS)

# This is our main target
lib_LTLIBRARIES=libhdf_java.la

# Source files for the library
libhdf_java_la_SOURCES=hdfanImp.c hdfdfpalImp.c \
   hdfdfuImp.c hdfexceptionImp.c hdfgrImp.c hdfheImp.c hdfhxImp.c hdfImp.c hdfnativeImp.c \
   hdfr24Imp.c hdfr8Imp.c hdfsdsImp.c hdfstructsutil.c hdfvdataImp.c hdfvfImp.c hdfvgroupImp.c \
   hdfvhImp.c hdfvqImp.c hdfvsqImp.c

# HDF Java (JNI) library depends on HDF Library.
libhdf_java_la_LIBADD=$(LIBMFHDF) $(LIBHDF) @LIBS@
libhdf_java_la_DEPENDENCIES = $(LIBMFHDF) $(LIBHDF)
libhdf_java_la_INCLUDES=-I$(top_srcdir)/hdf/src        \
         -I$(top_srcdir)/mfhdf/libsrc   \
         -I$(top_builddir)/mfhdf/libsrc

include $(top_srcdir)/config/conclude.am
