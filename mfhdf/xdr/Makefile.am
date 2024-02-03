## Makefile.am
## Run automake to generate a Makefile.in from this file.
#
# XDR Library Makefile(.in)
#

include $(top_srcdir)/config/commence.am
include $(top_srcdir)/config/lt_vers.am

## Setup the different includes and preprocessor #defines we need.
AM_CPPFLAGS=-I$(srcdir) -I$(top_srcdir)/hdf/src

# XDR Library
lib_LTLIBRARIES = libxdr.la

# Add libtool numbers to the HDF4 library (from config/lt_vers.am)
libxdr_la_LDFLAGS= -version-info $(LT_XDR_VERS_INTERFACE):$(LT_XDR_VERS_REVISION):$(LT_XDR_VERS_AGE) $(AM_LDFLAGS)

libxdr_la_SOURCES = xdr.c xdrfloat.c xdrstdio.c xdrarray.c
include_HEADERS = xdr.h types.h

LDADD = ./libxdr.la

# Test program. Link using libxdr.la
check_PROGRAMS = xdrtest
TEST_PROG = xdrtest
xdrtest_SOURCES = xdrtest.c

include $(top_srcdir)/config/conclude.am
