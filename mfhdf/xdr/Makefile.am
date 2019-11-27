## Makefile.am
## Run automake to generate a Makefile.in from this file.
#
# XDR Library Makefile(.in)
#

include $(top_srcdir)/config/commence.am

## Setup the different includes and preprocessor #defines we need.
AM_CPPFLAGS=-I$(top_srcdir)/hdf/src

# XDR Library
lib_LTLIBRARIES = libxdr.la
libxdr_la_SOURCES = xdr_array.c xdr_float.c xdr_mem.c xdr_reference.c \
	xdr.c xdr_rec.c xdr_stdio.c
include_HEADERS = xdr.h types.h

# Test program. Link using libxdr.la
check_PROGRAMS = xdrtest
TEST_PROG = xdrtest
xdrtest_SOURCES = xdrtest.c

include $(top_srcdir)/config/conclude.am
