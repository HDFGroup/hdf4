#############################################################################
##                      Subdirectories to build in                         ##
#############################################################################

include $(top_srcdir)/config/commence.am

# include Doxygen rules (requires autoconf-archive >2016-03-20)
@DX_RULES@

if BUILD_JAVA_CONDITIONAL
   JAVA_DIR=java
else
   JAVA_DIR=
endif

SUBDIRS = hdf mfhdf $(JAVA_DIR)
# install libhdf4.settings in lib directory
settingsdir = $(libdir)
settings_DATA = libhdf4.settings

# doxygen support
if BUILD_DOXYGEN_CONDITIONAL
doxygen: doxygen-doc
endif

# 'make-install-all' will be redundant but will still work.
install: install-recursive
uninstall: uninstall-recursive

# 'make install-all'
install-all:
	@$(MAKE) $(AM_MAKEFLAGS) install
uninstall-all:
	@$(MAKE) $(AM_MAKEFLAGS) uninstall
