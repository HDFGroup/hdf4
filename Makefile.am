#############################################################################
##                      Subdirectories to build in                         ##
#############################################################################

include $(top_srcdir)/config/commence.am

if HDF_BUILD_XDR
XDR_DIR = mfhdf/xdr
else
XDR_DIR = 
endif

SUBDIRS = hdf/src $(XDR_DIR) mfhdf/libsrc hdf mfhdf man
# install libhdf4.settings in lib directory
settingsdir = $(libdir)
settings_DATA = libhdf4.settings

# 'make install-all' also installs examples
install-all:
	@$(MAKE) $(AM_MAKEFLAGS) install
	@$(MAKE) $(AM_MAKEFLAGS) install-examples
uninstall-all:
	@$(MAKE) $(AM_MAKEFLAGS) uninstall
	@$(MAKE) $(AM_MAKEFLAGS) uninstall-examples

# Install examples recursively
install-examples uninstall-examples:
	@@SETX@; for d in hdf mfhdf; do \
	    (cd $$d && $(MAKE) $(AM_MAKEFLAGS) $@) || exit 1; \
	done
