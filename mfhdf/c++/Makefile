# $Id$
#
#	Development Makefile for netcdf c++ interface
#

INCDIR = ../libsrc
LIBDIR = ../libsrc

CCC = CC
INCLUDES = -I$(INCDIR)

CCFLAGS=-g
MANIFEST	= Makefile README doc.txt example.c example.cc example.cdl \
		  nctst.cc ncvalues.cc ncvalues.hh netcdf.cc netcdf.hh
DEFINES =
CPPFLAGS = $(INCLUDES) $(DEFINES)
LIBS = -L$(LIBDIR) -lnetcdf

# a test program that uses some of the draft C++ interface
PROG = nctst
PROG_SRCS = netcdf.cc nctst.cc ncvalues.cc
PROG_OBJS = netcdf.o nctst.o ncvalues.o

all: $(PROG)

$(PROG) : $(PROG_OBJS)
	$(LINK.cc) $(PROG_OBJS) $(LIBS) -o $@

clean:
	rm -f $(PROG_OBJS) $(PROG) example.cdf *~

distclean:	clean

netcdf.o: netcdf.hh $(INCDIR)/netcdf.h
ncvalues.o: ncvalues.hh

nctst_src: nctst.cc netcdf.cc ncvalues.cc
	#load nctst.cc
	#load netcdf.cc
	#load ncvalues.cc
	#load -L../../lib ../../lib/libnetcdf.a
	#link

nvtst: nvtst.o ncvalues.o
	$(LINK.cc) -g -o $@ nvtst.cc ncvalues.o


nvtst_src: nvtst.cc ncvalues.cc
	#load nvtst.cc
	#load ncvalues.cc
	#link

print: nctst.cc netcdf.hh netcdf.cc ncvalues.hh ncvalues.cc
	vgrind -s11 -lC++ -t nctst.cc netcdf.hh netcdf.cc ncvalues.hh \
				ncvalues.cc | psrev | lpr

MANIFEST.echo:
	@echo $(MANIFEST) | fmt -1
