#
# HDF Java native interface (JNI) Library Makefile(.in)

include $(top_srcdir)/config/commence.am

## Only recurse into subdirectories if the Java (JNI) interface is enabled.
if BUILD_JAVA_CONDITIONAL

# Mark this directory as part of the JNI API
JAVA_API=yes

SUBDIRS=src test examples

endif

include $(top_srcdir)/config/conclude.am
