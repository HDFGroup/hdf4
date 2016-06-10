#
# HDF Java native interface (JNI) Library Test Makefile(.in)

include $(top_srcdir)/config/commence.am

# Mark this directory as part of the JNI API
JAVA_API=yes

JAVAROOT = .classes

classes:
	test -d $(@D)/$(JAVAROOT) || $(MKDIR_P) $(@D)/$(JAVAROOT)

pkgpath = test
hdfjarfile = jar$(PACKAGE_TARNAME)-$(PACKAGE_VERSION).jar
jarfile = jar$(PACKAGE_TARNAME)test.jar

CLASSPATH_ENV=CLASSPATH=.:$(JAVAROOT):$(top_builddir)/java/src/$(hdfjarfile):$(top_srcdir)/java/lib/junit.jar:$(top_srcdir)/java/lib/hamcrest-core.jar:$(top_srcdir)/java/lib/slf4j-api-1.7.5.jar:$(top_srcdir)/java/lib/ext/slf4j-simple-1.7.5.jar:$$CLASSPATH
AM_JAVACFLAGS = $(H4_JAVACFLAGS) -deprecation

noinst_JAVA = \
	TestH4.java \
	TestH4ANparams.java \
	TestH4DFparams.java \
	TestH4DFPparams.java \
	TestH4DFRparams.java \
	TestH4GRparams.java \
	TestH4HCparams.java \
	TestH4SDparams.java \
	TestH4Vparams.java \
	TestH4VSparams.java \
	TestAll.java

$(jarfile): classnoinst.stamp classes
	$(JAR) cvf $@ -C $(JAVAROOT)/ $(pkgpath)

noinst_DATA = $(jarfile)

.PHONY: classes

check_SCRIPTS = junit.sh
TEST_SCRIPT = $(check_SCRIPTS)

CLEANFILES = classnoinst.stamp $(jarfile) $(JAVAROOT)/$(pkgpath)/*.class junit.sh

#JAVA_JUNIT  = $(JAVA_SRCS)
#noinst_JAVA = @JAVA_JUNIT@
#EXTRA_JAVA  = $(JAVA_JUNIT)
TESTS_JUNIT = AllJunitTests
#EXTRA_TEST  = $(TESTS_JUNIT)

AllJunitTests :
	echo "#! /bin/sh" > $@
	echo "exec @JUNIT@ test.TestAll" >> $@
	chmod +x $@

clean:
	rm -rf $(JAVAROOT)/*
	rm -f $(jarfile)
	rm -f classnoinst.stamp

include $(top_srcdir)/config/conclude.am
