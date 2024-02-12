include $(top_srcdir)/config/commence.am

if HDF_BUILD_FORTRAN
FORTRAN_DIR = fortran
else
FORTRAN_DIR = 
endif

# src folder in root Makefile, build other folders now
SUBDIRS = src $(FORTRAN_DIR) test util

DIST_SUBDIRS = src fortran test util

# 'make install-all'
install-all:
	@$(MAKE) $(AM_MAKEFLAGS) install
uninstall-all:
	@$(MAKE) $(AM_MAKEFLAGS) uninstall
