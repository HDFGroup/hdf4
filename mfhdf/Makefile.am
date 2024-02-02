include $(top_srcdir)/config/commence.am

if HDF_BUILD_FORTRAN
FORTRAN_DIR = fortran
else
FORTRAN_DIR =
endif

if HDF_BUILD_NETCDF_TOOLS
NETCDF_DIRS = ncdump ncgen nctest
else
NETCDF_DIRS = nctest
endif

# src folder in root Makefile, build other folders now
SUBDIRS = libsrc $(FORTRAN_DIR) test $(NETCDF_DIRS) dumper hdfimport hdiff hrepack

DIST_SUBDIRS = libsrc fortran test $(NETCDF_DIRS)  dumper hdfimport hdiff hrepack examples

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
