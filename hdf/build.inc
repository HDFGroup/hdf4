# ****************************************************************************
# * NCSA HDF                                                                 *
# * Software Development Group                                               *
# * National Center for Supercomputing Applications                          *
# * University of Illinois at Urbana-Champaign                               *
# * 605 E. Springfield, Champaign IL 61820                                   *
# *                                                                          *
# * For conditions of distribution and use, see the accompanying             *
# * hdf/COPYING file.                                                        *
# *                                                                          *
# ***************************************************************************
#
# $Id$
#
# ##################################################################
#
#
# This is the top level Makefile to build HDF 4.0 on Unix based
#       platforms
#

#
#
# Flags to recursively send
#

HDF_FLAGS       = \
        CC="$(CC)" \
        CFLAGS="$(CFLAGS)" \
        FC="$(FC)" \
        FFLAGS="$(FFLAGS)" \
	GLLIB="$(GLLIB)" \
        RANLIB="$(RANLIB)" \
        AR="$(AR)" \
        ARFLAGS="$(ARFLAGS)" \
        RM="$(RM)" \
        RMFLAGS="$(RMFLAGS)" \
        MACHINE="$(MACHINE)" \
        HDFLIB="$(HDFLIB)" \
        HDFINC="$(HDFINC)" \
	HDFBIN="$(HDFBIN)"	

#
#
# General rules
#
all:
	@$(MAKE) $(MFLAGS) $(HDF_FLAGS) TARG=$@ \
          SUBDIRS="src jpeg util test" _sub message 

allnofortran:
	@$(MAKE) $(MFLAGS) $(HDF_FLAGS) TARG=nofortran \
          SUBDIRS="src jpeg util test" _sub message 

rebuild rebuildnofortran:
	@$(MAKE) $(MFLAGS) $(HDF_FLAGS) TARG=$@ \
          SUBDIRS="src jpeg util test" _sub message 

libnofortran:
	@$(MAKE) $(MYFLAGS) $(HDF_FLAGS) TARG=nofortran \
	  SUBDIRS="src jpeg" _sub message

libnostub: libnofortran

allnostub: allnofortran

libdf:
	@$(MAKE) $(MFLAGS) $(HDF_FLAGS) TARG=all \
          SUBDIRS="src" _sub

libjpeg:
	@$(MAKE) $(MFLAGS) $(HDF_FLAGS) TARG=all \
          SUBDIRS="jpeg" _sub

utilities:
	@$(MAKE) $(MFLAGS) $(HDF_FLAGS) TARG=all \
          SUBDIRS="src jpeg util" _sub

tests:
	@$(MAKE) $(MFLAGS) $(HDF_FLAGS) TARG=all \
          SUBDIRS="src jpeg test" _sub

testnofortran:
	@$(MAKE) $(MFLAGS) $(HDF_FLAGS) TARG=nofortran \
          SUBDIRS="src jpeg test" _sub message 

perfs:
	@$(MAKE) $(MFLAGS) $(HDF_FLAGS) TARG=all \
          SUBDIRS="src jpeg perf" _sub

#install:      $(INSTALL_DIRS)
#	@$(MAKE) $(MFLAGS) $(HDF_FLAGS) TARG=$@  \
#          SUBDIRS="src util test" _sub

debug:
	@$(MAKE) $(MFLAGS) $(HDF_FLAGS) TARG=debug \
          SUBDIRS="src jpeg test" _sub message 

saber:
	@$(MAKE) $(MFLAGS) $(HDF_FLAGS) TARG=saber \
          SUBDIRS="src jpeg test" _sub

clean:
	@$(MAKE) $(MFLAGS) $(HDF_FLAGS) TARG=$@  \
          SUBDIRS="src jpeg util test" _sub
	$(RM) $(RMFLAGS) core *.log

distclean:
	@$(MAKE) $(MFLAGS) $(HDF_FLAGS) TARG=$@  \
          SUBDIRS="src jpeg util test" _sub
	$(RM) $(RMFLAGS) core *.log
	$(RM) -rf bin lib include

_sub:
	@for dir in $(SUBDIRS); do \
          (cd $$dir; echo Making \`$(TARG)\' in `pwd`; \
              $(MAKE) $(MFLAGS) $(HDF_FLAGS) $(TARG)); \
	done

message:
	@echo ""
	@echo "***********************************************************"
	@echo " HDF 4.0 library successfully created."
	@echo ""
	@echo "If you have any old vset files (v 1.0) you must use vcompat"
	@echo "to make them compatible with Vset 2.0 applications"
	@echo ""
	@echo "User programs can be created as follows:"
	@echo ""
	@echo "   ${CC} program.c libdf.a -o program"
	@echo ""
	@echo "***********************************************************"
	@echo ""
