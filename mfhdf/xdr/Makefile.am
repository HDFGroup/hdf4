## Makefile.am
## Run automake to generate a Makefile.in from this file.
#
# XDR Library Makefile(.in)
#

include $(top_srcdir)/config/commence.am

## Setup the different includes and preprocessor #defines we need.
INCLUDES=-I$(top_srcdir)/hdf/src

# XDR Library
lib_LTLIBRARIES = libxdr.la
libxdr_la_SOURCES = xdr.c xdrfloat.c xdrstdio.c xdrarray.c
include_HEADERS = xdr.h types.h

# Test program. Link using libxdr.la
check_PROGRAMS = xdrtest
TESTS = xdrtest
xdrtest_SOURCES = xdrtest.c
