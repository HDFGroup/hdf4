#############################################################################
##                      Subdirectories to build in                         ##
#############################################################################

SUBDIRS=hdf mfhdf man

# Keep Makefile.ins, aclocal.m4, etc. from regenerating...
$(ACLOCAL_M4):  configure.ac 
	@true
$(srcdir)/configure:  $(srcdir)/configure.ac $(ACLOCAL_M4) $(CONFIGURE_DEPENDENCIES)
	@true
$(srcdir)/Makefile.in:  Makefile.am  $(top_srcdir)/configure.ac $(ACLOCAL_M4)
	@true
