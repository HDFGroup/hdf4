include $(top_srcdir)/config/commence.am

if HDF_BUILD_XDR
XDR_DIR = xdr
else
XDR_DIR =
endif

if HDF_BUILD_FORTRAN
FORTRAN_DIR = fortran
else
FORTRAN_DIR =
endif

# src folder in root Makefile, build other folders now
SUBDIRS = $(XDR_DIR) libsrc $(FORTRAN_DIR) test ncdump ncgen dumper hdfimport hdiff hrepack nctest

DIST_SUBDIRS = xdr libsrc fortran test ncdump ncgen dumper hdfimport hdiff hrepack nctest examples

installcheck-local:
	@(cd examples && $(MAKE) $(AM_MAKEFLAGS) $@) || exit 1;

# 'make install-all' also installs examples
install-all:
	@$(MAKE) $(AM_MAKEFLAGS) install
	@$(MAKE) $(AM_MAKEFLAGS) install-examples
uninstall-all:
	@$(MAKE) $(AM_MAKEFLAGS) uninstall
	@$(MAKE) $(AM_MAKEFLAGS) uninstall-examples

# Install examples recursively
install-examples uninstall-examples:
	@@SETX@; for d in examples $(FORTRAN_DIR); do \
	    (cd $$d && $(MAKE) $(AM_MAKEFLAGS) $@) || exit 1; \
	done
