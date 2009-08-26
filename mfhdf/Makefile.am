include $(top_srcdir)/config/commence.am

if HDF_BUILD_FORTRAN
FORTRAN_DIR = fortran
else
FORTRAN_DIR = 
endif

if HDF_BUILD_XDR
XDR_DIR = xdr
else
XDR_DIR = 
endif

SUBDIRS=port $(FORTRAN_DIR) $(XDR_DIR) libsrc ncdump ncgen dumper hdfimport hdiff hrepack nctest
DIST_SUBDIRS=$(SUBDIRS) examples

installcheck-local:
	@(cd examples && $(MAKE) $(AM_MAKEFLAGS) $@) || exit 1;

