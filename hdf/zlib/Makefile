# This file was generated automatically by configure.  Do not edit.
VPATH = .
links = 
host_alias = i386-unknown-freebsd2.1.0
host_cpu = i386
host_vendor = unknown
host_os = freebsd2.1.0
host_canonical = i386-unknown-freebsd2.1.0
target_alias = i386-unknown-freebsd2.1.0
target_cpu = i386
target_vendor = unknown
target_os = freebsd2.1.0
target_canonical = i386-unknown-freebsd2.1.0
host_makefile_frag = ../../config/mh-fbsd
# Makefile for zlib
# Copyright (C) 1995 Jean-loup Gailly.
# For conditions of distribution and use, see copyright notice in zlib.h 

# Makefile.in is edited by Cygnus configure to produce a custom Makefile.

# Read installation instructions before saying "make" !!

# For compiling with source and object files in different directories.
srcdir = .
TOP_SRCDIR = ../..

# Where to install the programs and man pages.
prefix = /usr/local
exec_prefix = $(prefix)
bindir = $(exec_prefix)/bin
libdir = $(exec_prefix)/lib
includedir = $(prefix)/include
binprefix =
manprefix =
manext = 1
mandir = $(prefix)/man/man$(manext)

# The following default options are overriden in the makefile fragment
# section below
#
# The name of your C compiler:
CC= gcc

# You may need to adjust these cc options:
CFLAGS= -O -I$(srcdir)

# Link-time cc options:
LDFLAGS= 

# To link any special libraries, add the necessary -l commands here.
LDLIBS= 

# miscellaneous OS-dependent stuff
SHELL = /bin/sh
# linker
LN= $(CC)
# file deletion command
RM = /bin/rm 
RMFLAGS = -f

# file rename command
MV= mv
# library (.a) file creation command
#AR= ar rc
AR = ar
ARFLAGS = rc

# Name of achive randomizer, usually ranlib (use 'touch' if non-existant)
RANLIB = ranlib

# second step in .a creation (use "touch" if not needed)
AR2= $(RANLIB)

# installation program
INSTALL= cp
INSTALL_PROGRAM= $(INSTALL)
INSTALL_DATA= $(INSTALL)

# Old stuff
#CC=cc
#CFLAGS=-O
#use -O3 for gcc to take advantage of inlining
#CFLAGS="-O -DMAX_WBITS=14 -DMAX_MEM_LEVEL=7"
#CFLAGS=-g -DDEBUG
#LDFLAGS=-L. -lz

#RANLIB=ranlib

#prefix=/usr/local
#----------------- End of configurable options.---------------------------
# ==> specify directories where to find inludes and library
#     HDF library, and HDF utilities
HDFINC= $(srcdir)/../src
HDFLIB= $(srcdir)/../src
HDFBIN= $(srcdir)/../bin

#### makefile fragment from 'config/' goes here ##############
# You can override the following variables here
#
# Compiliers:
# For gcc version
CC=gcc
CFLAGS=-ansi -Wall -pedantic -g
#CFLAGS=-ansi
#
#Using ANSI compilier
#CC=cc
#CFLAGS=

# Fortran compilier
FC = f77
FFLAGS = -g

# Name of achive randomizer, usually ranlib (use 'touch' if non-existant)
RANLIB = ranlib

# Name of library archiver and flags to send, default 'AR=ar', 'ARFLAGS=r'
AR = ar
ARFLAGS = r

# Name of remove utility, default 'RM=/bin/rm', 'RMFLAGS=-f'
RM = /bin/rm
RMFLAGS = -f

# ------------ Macros for Pablo Instrumentation  --------------------
# Uncomment the following lines to create a Pablo Instrumentation
# version of the HDF core library called 'libdf-inst.a'
# See the documentation in the directory 'hdf/pablo' for further 
# information about Pablo and what platforms it is supported on
# before enabling. 
# You need to set 'PABLO_INCLUDE' to the Pablo distribution 
# include directory to get the files 'IOTrace.h' and 'IOTrace_SD.h'.

#PABLO_FLAGS  = -DHAVE_PABLO
#PABLO_INCLUDE = -I/usr/local/include/pablo-4.0

# ------------ Macros for Shared Memory File Buffer Pool(fmpool) ------
# Uncomment the following lines to enable shared memory file buffer pool
# version of the HDF core library libdf.a. Please read the
# documentation before enabling this feature.

#FMPOOL_FLAGS  = -DHAVE_FMPOOL

# ------------ General Macros for NETCDF --------------------
# Operating system, Used in fortran directory to generate fortran
# wrappers. Possible values are osf, aix, hpux, irix, sunos, ultrix
# unicos, convex, Linux, freebsd
OS	= freebsd

CPP	      = $(CC) -E

# for endianess, for little endian byte order need -DSWAP
# for FreeBSD and Linux(anny x86 UNIX)
SWAP          = -DSWAP
#SWAP          = 

# for 32bit 'network long' integer, possible value -DNETLONG=int
NETLONG       = 

# additional CFLAGS
CFLAGS_NETCDF = 

# Additional flags for preproccesor. Some no longer used because
#   library now requires ANSI compilier.
#
# for no function prototypes add -DNO_HAVE_PROTOTYPES
# for no strerror() add -DNO_STRERROR
# for no variadic function support add -DNO_STDARG
# for Unicos 
# CPPFLAGS_HDF  = $(FMPOOL_FLAGS) -DNDEBUG -DHDF -DBIG_SHORTS -DBIG_LONGS
# for OSF(dec alpha) 
# CPPFLAGS_HDF  = $(FMPOOL_FLAGS) -DNDEBUG -DHDF -DBIG_LONGS -std1
# for IRIX6(64-bit) 
# CPPFLAGS_HDF  = $(FMPOOL_FLAGS) -DNDEBUG -DHDF -DBIG_LONGS
# for AIX
# CPPFLAGS_HDF  = $(FMPOOL_FLAGS) -DNDEBUG -DHDF -D_ALL_SOURCE
# for Sunos and Linux
# CPPFLAGS_HDF  = $(FMPOOL_FLAGS) -DNDEBUG -DHDF -DNO_STRERROR
# normal
CPPFLAGS_HDF  = $(FMPOOL_FLAGS) -DNDEBUG -DHDF

# neqn(1)
NEQN	= neqn
# tbl(1)
TBL	= tbl
# which(1)
WHICH	= which
# lex(1)
LEX	= lex    # GNU flex?
# yacc(1)
YACC	= yacc   # GNU bison?

#diff(1)
DIFF    = diff   # GNU diff?
DIFF_FLAGS = -w

NCDUMP	= 
NCGEN	= 
FTPDIR	= 
VERSION_NETCDF	= 2.3.2

# ------------ XDR Macros for NETCDF --------------------
# Location of <xdr.h>
CPP_XDR		= -I/usr/include/rpc

# If XDR library not present then you need
XDR_LIBOBJS    = xdr.o xdrfloat.o xdrstdio.o xdrarray.o
#XDR_LIBOBJS     =

# location of library, sometimes the sunos requires -lsun
LD_XDR		= 
XDR_INSTALL_DEPS =

# ------------ Port Macros for NETCDF --------------------
# Major and Minor numbe of NETCDF library version
MAJOR_NO	= 2
MINOR_NO	= 3
PORT_CFORTRAN	= 
NEED_FORTC	= 
FORTC		= 
# possible values are limits.h, float.h, stddef.h, stdlib.h,
# string.h time.h signal.h unistd.h
PORT_HEADERS	= 
PORT_SUBDIRS    =
LIBOBJS		= uddummy.o 

# ------------ libsrc Macros for NETCDF --------------------
# possible values are xdrposix and xdrstdio
XDRFILE		= xdrposix

## ------------------------ end of makefile fragments--------------------
HDF_FLAGS       = \
        CC="$(CC)" \
        CFLAGS="$(CFLAGS)" \
        FC="$(FC)" \
        FFLAGS="$(FFLAGS)" \
        RANLIB="$(RANLIB)" \
        AR="$(AR)" \
        ARFLAGS="$(ARFLAGS)" \
        RM="$(RM)" \
        RMFLAGS="$(RMFLAGS)" \
        MACHINE="$(MACHINE)" 

LOCAL_MACROS  = CC="$(CC)" \
	CFLAGS="$(CFLAGS)" \
	CPP="$(CPP)" \
	CPPFLAGS="$(CPPFLAGS)" \
	exec_prefix="$(exec_prefix)" \
	FC="$(FC)" \
	prefix="$(prefix)"

OBJS = adler32.o compress.o crc32.o gzio.o uncompr.o deflate.o trees.o \
       zutil.o inflate.o infblock.o inftrees.o infcodes.o infutil.o inffast.o

TEST_OBJS = example.o minigzip.o

all: example minigzip

test: all
	./example
	echo hello world | ./minigzip | ./minigzip -d 

install-lib: install

install: libz.a
	-@mkdir $(prefix)/include
	-@mkdir $(prefix)/lib
	cp zlib.h zconf.h $(prefix)/include
	chmod 644 $(prefix)/include/zlib.h $(prefix)/include/zconf.h
	cp libz.a $(prefix)/lib
	chmod 644 $(prefix)/lib/libz.a

libz.a: $(OBJS)
	ar rc $@ $(OBJS)
	$(RANLIB) $@

example: example.o libz.a
	$(CC) $(CFLAGS) -o $@ example.o $(LDFLAGS)

minigzip: minigzip.o libz.a
	$(CC) $(CFLAGS) -o $@ minigzip.o $(LDFLAGS)

clean distclean:
	rm -f *.o example minigzip libz.a foo.gz

zip:
	zip -ul9 zlib README ChangeLog Makefile Make????.??? Makefile.?? \
	  descrip.mms *.[ch]

tgz:
	cd ..; tar cfz zlib/zlib.tgz zlib/README zlib/ChangeLog zlib/Makefile \
	  zlib/Make????.??? zlib/Makefile.?? zlib/descrip.mms zlib/*.[ch]

# DO NOT DELETE THIS LINE -- make depend depends on it.

adler32.o: zutil.h zlib.h zconf.h
compress.o: zlib.h zconf.h
crc32.o: zutil.h zlib.h zconf.h
deflate.o: deflate.h zutil.h zlib.h zconf.h
example.o: zlib.h zconf.h
gzio.o: zutil.h zlib.h zconf.h
infblock.o: zutil.h zlib.h zconf.h infblock.h inftrees.h infcodes.h infutil.h
infcodes.o: zutil.h zlib.h zconf.h inftrees.h infutil.h infcodes.h inffast.h
inffast.o: zutil.h zlib.h zconf.h inftrees.h infutil.h inffast.h
inflate.o: zutil.h zlib.h zconf.h infblock.h
inftrees.o: zutil.h zlib.h zconf.h inftrees.h
infutil.o: zutil.h zlib.h zconf.h inftrees.h infutil.h
minigzip.o: zlib.h zconf.h
trees.o: deflate.h zutil.h zlib.h zconf.h
uncompr.o: zlib.h zconf.h
zutil.o: zutil.h zlib.h zconf.h
