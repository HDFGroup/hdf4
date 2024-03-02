include $(top_srcdir)/config/commence.am

if HDF_BUILD_FORTRAN
FORTRAN_DIR = fortran
else
FORTRAN_DIR =
endif

# src folder in root Makefile, build other folders now
SUBDIRS = libsrc $(FORTRAN_DIR) test nctest dumper hdfimport hdiff hrepack

DIST_SUBDIRS = libsrc fortran test nctest dumper hdfimport hdiff hrepack

# 'make install-all'
install-all:
	@$(MAKE) $(AM_MAKEFLAGS) install
uninstall-all:
	@$(MAKE) $(AM_MAKEFLAGS) uninstall
