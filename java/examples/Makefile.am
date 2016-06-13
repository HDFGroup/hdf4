#
# HDF Java native interface (JNI) Library Examples Makefile(.in)

include $(top_srcdir)/config/commence.am

# Mark this directory as part of the JNI API
JAVA_API=yes

JAVAROOT = .classes

classes:
	test -d $(@D)/$(JAVAROOT) || $(MKDIR_P) $(@D)/$(JAVAROOT)

pkgpath = examples
hdfjarfile = jar$(PACKAGE_TARNAME)-$(PACKAGE_VERSION).jar
jarfile = jar$(PACKAGE_TARNAME)examples.jar

CLASSPATH_ENV=CLASSPATH=.:$(JAVAROOT):$(top_builddir)/java/src/$(hdfjarfile):$(top_srcdir)/java/lib/slf4j-api-1.7.5.jar:$(top_srcdir)/java/lib/ext/slf4j-simple-1.7.5.jar:$$CLASSPATH
AM_JAVACFLAGS = $(H4_JAVACFLAGS) -deprecation

noinst_JAVA = \
	HDF4FileCreate.java \
	HDF4GroupCreate.java \
	HDF4DatasetCreate.java

$(jarfile): classnoinst.stamp classes
	$(JAR) cvf $@ -C $(JAVAROOT)/ $(pkgpath)

noinst_DATA = $(jarfile)

.PHONY: classes

check_SCRIPTS = 
TEST_SCRIPT = $(check_SCRIPTS)

CLEANFILES = classnoinst.stamp $(jarfile) $(JAVAROOT)/$(pkgpath)/*.class

#JAVA_JUNIT  = $(JAVA_SRCS)
#noinst_JAVA = @JAVA_JUNIT@
#EXTRA_JAVA  = $(JAVA_JUNIT)
#EXTRA_TEST  = $(TESTS_JUNIT)

clean:
	rm -rf $(JAVAROOT)/*
	rm -f $(jarfile)
	rm -f classnoinst.stamp

include $(top_srcdir)/config/conclude.am
