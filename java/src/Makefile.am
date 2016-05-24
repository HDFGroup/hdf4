#
# HDF Java native interface (JNI) Library Makefile(.in)

include $(top_srcdir)/config/commence.am

# Mark this directory as part of the JNI API
JAVA_API=yes

SUBDIRS=jni

JAVAROOT = .classes

classes:
	test -d $(@D)/$(JAVAROOT) || $(MKDIR_P) $(@D)/$(JAVAROOT)

jarfile = jar$(PACKAGE_TARNAME)-$(PACKAGE_VERSION).jar
hdf_javadir = $(libdir)

pkgpath = hdf/hdflib
CLASSPATH_ENV=CLASSPATH=.:$(top_srcdir)/java/lib/slf4j-api-1.7.5.jar:$$CLASSPATH

AM_JAVACFLAGS = $(H4_JAVACFLAGS) -deprecation

hdf_java_JAVA =  \
	${pkgpath}/HDFException.java \
    ${pkgpath}/HDFJavaException.java \
    ${pkgpath}/HDFArray.java \
    ${pkgpath}/HDFChunkInfo.java \
    ${pkgpath}/HDFCompInfo.java \
    ${pkgpath}/HDFConstants.java \
    ${pkgpath}/HDFDeflateCompInfo.java \
    ${pkgpath}/HDFIMCOMPCompInfo.java \
    ${pkgpath}/HDFJPEGCompInfo.java \
    ${pkgpath}/HDFLibrary.java \
    ${pkgpath}/HDFLibraryException.java \
    ${pkgpath}/HDFNativeData.java \
    ${pkgpath}/HDFNewCompInfo.java \
    ${pkgpath}/HDFNBITChunkInfo.java \
    ${pkgpath}/HDFNBITCompInfo.java \
    ${pkgpath}/HDFNotImplementedException.java \
    ${pkgpath}/HDFOldCompInfo.java \
    ${pkgpath}/HDFOldRLECompInfo.java \
    ${pkgpath}/HDFRLECompInfo.java \
    ${pkgpath}/HDFSKPHUFFCompInfo.java \
    ${pkgpath}/HDFSZIPCompInfo.java


$(jarfile): classhdf_java.stamp classes docs
	$(JAR) cvf $@ -C $(JAVAROOT)/ $(pkgpath)

hdf_java_DATA = $(jarfile)

.PHONY: docs classes

WINDOWTITLE = 'HDF Java'
DOCTITLE = '<h1>HDF Java Wrapper</h1>'
SRCDIR = '$(pkgpath)'

docs:
	$(JAVADOC) -sourcepath $(srcdir) -d javadoc -use -splitIndex -windowtitle $(WINDOWTITLE) -doctitle $(DOCTITLE) -J-Xmx180m -verbose -overview $(top_srcdir)/java/src/hdf/overview.html -classpath $(CLASSPATH_ENV) hdf.hdflib

CLEANFILES = classhdf_java.stamp $(jarfile) $(JAVAROOT)/$(pkgpath)/*.class

clean:
	rm -rf $(JAVAROOT)/*
	rm -f $(jarfile)
	rm -f classhdf_java.stamp


# Clean examples when check-clean is invoked
check-clean :: ;

#include $(top_srcdir)/config/conclude.am
