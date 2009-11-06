include $(top_srcdir)/config/commence.am

SUBDIRS=src test util

DIST_SUBDIRS=$(SUBDIRS) examples

installcheck-local:
	@(cd examples && $(MAKE) $(AM_MAKEFLAGS) $@) || exit 1;

