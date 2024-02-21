## Process this file with autoconf to produce configure.
##
##
## Copyright by The HDF Group.
## Copyright by the Board of Trustees of the University of Illinois.
## All rights reserved.
##
## This file is part of HDF.  The full HDF copyright notice, including
## terms governing use, modification, and redistribution, is contained in
## the COPYING file, which can be found at the root of the source code
## distribution tree, or in https://support.hdfgroup.org/ftp/HDF/releases/.
## If you do not have access to either file, you may request a copy from
## help@hdfgroup.org.
##

## ----------------------------------------------------------------------
## Initialize configure.
##

## AC_INIT takes the name of the package, the version number, and an
## email address to report bugs. AC_CONFIG_SRCDIR takes a unique file
## as its argument.
##
## NOTE: Do not forget to change the version number here when we do a
## release!!!
##
AC_INIT([HDF], [4.2.17-1], [help@hdfgroup.org])

AC_CONFIG_SRCDIR([hdf/src/atom.c])
AC_CONFIG_HEADERS([hdf/src/h4config.h])

AC_CONFIG_AUX_DIR([bin])
AC_CONFIG_MACRO_DIR([m4])

## AM_INIT_AUTOMAKE takes a list of options that should be applied to
## every Makefile.am when automake is run.
##
## NOTE: Don't add subdir-objects here just yet. Modern Automake will
##       complain that it's missing, but adding it causes failures on
##       older systems.
AM_INIT_AUTOMAKE([foreign])
m4_ifdef([AM_SILENT_RULES], [AM_SILENT_RULES([yes])]) # use silent rules where available - automake 1.11

## AM_MAINTAINER_MODE determines the behavior of "rebuild rules" that contain
## dependencies for Makefile.in files, configure, config.h, etc. If
## AM_MAINTAINER_MODE is enabled, these files will be rebuilt if out of date.
## When disabled, the autotools build files can get out of sync and the build
## system will not complain or try to regenerate downstream files.
##
## The AM_MAINTAINER_MODE macro also determines whether the
## --(enable|disable)-maintainer-mode configure option is available. When the
## macro is present, with or without a parameter, the option will be added
## to the generated configure script.
##
## In summary:
##
##  AM_MAINTAINER_MODE([enable])
##      - Build dependencies ON by default
##      - Configure option exists
##
##  AM_MAINTAINER_MODE([disable])
##      - Build dependencies OFF by default
##      - Configure option exists
##
##  AM_MAINTAINER_MODE
##      - Build dependencies OFF by default
##      - Configure option exists
##
##  No AM_MAINTAINER_MODE macro
##      - Build dependencies ON by default
##      - No configure option to control build dependencies
##
## The biggest concern for us is that version control systems like git
## usually don't preserve dependencies between timestamps, so the build
## system will often think that upstream build files like Makefile.am are
## dirty and that rebuilding needs to occur when it doesn't. This is a problem
## in release branches where we provide the autotools-generated files. Users
## who don't have autoconf, automake, etc. will then have difficulty building
## release branches checked out from git.
##
## By default, maintainer mode is enabled in development branches and disabled
## in release branches.
AM_MAINTAINER_MODE([enable])

## ----------------------------------------------------------------------
## Set prefix default (install directory) to a directory in the build area.
## This allows multiple src-dir builds within one host.
AC_PREFIX_DEFAULT([`pwd`/hdf4])

## Run post processing on files created by configure.
## * src/h4config.h
## * libhdf4.settings
##
## ([[:blank:]])* matches and captures any whitespace and drops it
## where the \1 is located on the right-hand side so the indenting
## doesn't change. ([[:blank:]] is POSIX-compliant \s)
AC_CONFIG_COMMANDS([h4config], [
  echo "modifying hdf/src/h4config.h"
  sed 's/#\([[:blank:]]\)*define /#\1define H4_/' < hdf/src/h4config.h |\
    sed 's/#\([[:blank:]]\)*undef /#\1undef H4_/' > h4config
  cp h4config hdf/src/h4config.h
  rm -f h4config

  echo "Post process libhdf4.settings"
  sed '/^#/d' < libhdf4.settings > libhdf4.settings.TMP
  cp libhdf4.settings.TMP libhdf4.settings
  rm -f libhdf4.settings.TMP
])

## It's possible to configure for a host other than the one on which
## configure is currently running by using the --host=foo flag.
## For machines on which HDF4 is often configured, it can be convenient
## to specify the name of the machine rather than its canonical type.
##
## There are currently no hosts, but if there were they would be
## listed by hostname and the alias would point to a file in
## the config directory:
##
##case $host_alias in
##  <some host>)
##    host_alias=<config file in config directory>
##    ;;
##esac

AC_CANONICAL_HOST
AC_SUBST([JNIFLAGS])
AC_SUBST([AR_FLAGS])

##
##  FUTURE H4_XXFLAGS GO HERE
##

## ----------------------------------------------------------------------
## Dump all shell variables values.
##
AC_MSG_CHECKING([shell variables initial values])
set >&AS_MESSAGE_LOG_FD
AC_MSG_RESULT([done])

## ----------------------------------------------------------------------
## Save system information for the library settings file.
##
AC_SUBST([UNAME_INFO])
UNAME_INFO=`uname -a`

## ----------------------------------------------------------------------
## Some platforms have broken basename, and/or xargs programs. Check
## that it actually does what it's supposed to do. Catch this early
## since configure and scripts relies upon them heavily and there's
## no use continuing if it's broken.
##
AC_MSG_CHECKING([if basename works])
BASENAME_TEST="`basename /foo/bar/baz/qux/basename_works`"
if test $BASENAME_TEST != "basename_works"; then
  AC_MSG_ERROR([basename program doesn't work])
else
  AC_MSG_RESULT([yes])
fi

## xargs basename used in configure to get the CC_BASENAME value
AC_MSG_CHECKING([if xargs works])
XARGS_TEST="`echo /foo/bar/baz/qux/xargs_works | xargs basename`"
if test $XARGS_TEST != "xargs_works"; then
  AC_MSG_ERROR([xargs program doesn't work])
else
  AC_MSG_RESULT([yes])
fi

## ----------------------------------------------------------------------
## Check that the cache file was build on the same host as what we're
## running on now.
##
AC_CACHE_CHECK([for cached host], [hdf4_cv_host], [hdf4_cv_host="none"]);
if test $hdf4_cv_host = "none"; then
  hdf4_cv_host=$host
elif test $hdf4_cv_host != $host; then
  AC_MSG_ERROR([
     The config.cache file was generated on $hdf4_cv_host but
     this is $host.  Please remove that file and try again.
     config.cache file is invalid])
fi

## Source any special files that we need. These files normally aren't
## present but can be used by the maintainers to fine tune things like
## turning on debug or profiling flags for the compiler. The search order
## is:
##
##         CPU-VENDOR-OS
##         VENDOR-OS
##         CPU-OS
##         CPU-VENDOR
##         OS
##         VENDOR
##         CPU
##
## If the `OS' ends with a version number then remove it. For instance,
## `freebsd3.1' would become `freebsd'
##
case "$host_os" in
  aix4.*)       host_os_novers="aix4.x"     ;;
  aix5.*)       host_os_novers="aix5.x"     ;;
  darwin10.*)   host_os_novers="darwin10.x" ;;
  darwin11.*)   host_os_novers="darwin11.x" ;;
  darwin12.*)   host_os_novers="darwin12.x" ;;
  freebsd*)     host_os_novers="freebsd"    ;;
  solaris2.*)   host_os_novers="solaris2.x" ;;
  *)            host_os_novers="$host_os"   ;;
esac

host_config="none"
for f in $host_cpu-$host_vendor-$host_os        \
         $host_cpu-$host_vendor-$host_os_novers \
         $host_vendor-$host_os                  \
         $host_vendor-$host_os_novers           \
         $host_cpu-$host_os                     \
         $host_cpu-$host_os_novers              \
         $host_cpu-$host_vendor                 \
         $host_os                               \
         $host_os_novers                        \
         $host_vendor                           \
         $host_cpu ; do
  AC_MSG_CHECKING([for config $f])
  if test -f "$srcdir/config/$f"; then
    host_config=$srcdir/config/$f
    AC_MSG_RESULT([found])
    break
  fi
  AC_MSG_RESULT([no])
done
if test "X$host_config" != "Xnone"; then
  CC_BASENAME="`echo $CC | cut -f1 -d' ' | xargs basename 2>/dev/null`"
  F77_BASENAME="`echo $F77 | cut -f1 -d' ' | xargs basename 2>/dev/null`"
  . $host_config
fi

## ======================================================================
## Checks for netCDF-2.3.2 support
## ======================================================================

# NOTE!     We disable Fortran netCDF APIs and their testing when
#           --disable-netcdf is used to avoid conflicts


# Define a proper variable to be used in mfhdf/testfortran.sh.in to run
# the netCDF Fortran APIs test program "ftest"
AC_SUBST(TEST_FORTRAN_NETCDF)
TEST_FORTRAN_NETCDF="yes"

AC_ARG_ENABLE([netcdf],
              [AS_HELP_STRING([--enable-netcdf],
                              [HDF4 builds replacement netCDF 2.3.2 API calls that are exported (e.g., ncopen()).
                               Enabling this option uses the original netCDF names when building these functions.
                               Disable prefixes them with 'sd_' to avoid name clashes.
                               This does NOT affect building the HDF4-built netCDF tools ncdump and ncgen,
                               nor does it disable any HDF4 functionality - this option only affects
                               API call names.
                               Disabling this option disables building the netCDF Fortran wrappers (they
                               will not be built with either the original or sd_ prefixed names).
                               [default=yes (HDF4-built netCDF API calls will be exported undecorated)]])],,
              [enableval="yes"])

case "$enableval" in
  yes)
    BUILD_NETCDF="yes"
    AC_DEFINE([HAVE_NETCDF], [1], [Define if we export HDF4-built unmangled netCDF 2.3.2 API calls])
    ;;
  no)
    BUILD_NETCDF="no"
    TEST_FORTRAN_NETCDF="no"
    ;;
esac
AM_CONDITIONAL([HDF_BUILD_NETCDF], [test "X$BUILD_NETCDF" = "Xyes"])
AC_SUBST([BUILD_NETCDF])

# It is also possible to disable building the netCDF tools ncdump and ncgen.
# Some users may want to do this since our tools will conflict with netCDF's
# if they are installed. This will also disable building nctest.
AC_ARG_ENABLE([netcdf-tools],
              [AS_HELP_STRING([--enable-netcdf-tools],
                              [Build HDF4 versions of NetCDF tools (ncgen, ncdump) [default=yes]])],,
              [enableval="yes"])

case "$enableval" in
  yes)
    BUILD_NETCDF_TOOLS="yes"
    ;;
  no)
    BUILD_NETCDF_TOOLS="no"
    ;;
esac
AM_CONDITIONAL([HDF_BUILD_NETCDF_TOOLS], [test "X$BUILD_NETCDF_TOOLS" = "Xyes"])
AC_SUBST([BUILD_NETCDF_TOOLS])

## ======================================================================
## Checks for programs
## ======================================================================

AC_PROG_CC
AC_PROG_CPP

## ----------------------------------------------------------------------
## Check if they would like the Fortran interface compiled
##
## This interface is UNSAFE on 64-bit systems as the interface will
## attempt to store pointers in 32-bit integers.
##
AC_ARG_ENABLE([fortran],
              [AS_HELP_STRING([--enable-fortran],
                              [Build Fortran into library. This interface
                               is unsafe on 64-bit systems. [default=no]])],,
              [enableval="no"])

case "$enableval" in
  yes)
    BUILD_FORTRAN="yes"
    AC_PROG_F77
    AC_F77_WRAPPERS

    if test "X$F77" = "X"; then
      BUILD_FORTRAN="no"
    fi
    ;;
  no)
    BUILD_FORTRAN="no"
    F77="no"
    ;;
esac
AM_CONDITIONAL([HDF_BUILD_FORTRAN], [test "X$BUILD_FORTRAN" = "Xyes"])
AC_SUBST([BUILD_FORTRAN])

## -------------------------------------------------------------------------
## Build static libraries by default. Furthermore, fortran shared libraries
## are unsupported. Disallow a user from enabling both shared libraries and
## fortran.
if test "X${enable_shared}" != "Xyes"; then
    enable_shared="no"
fi

if test "X${enable_shared}" = "Xyes"; then
    if test "X${BUILD_FORTRAN}" = "Xyes"; then
        AC_MSG_ERROR([Cannot build shared fortran libraries. Please configure with --disable-fortran flag.])
    fi
fi

## ----------------------------------------------------------------------
## Check if they would like the Java native interface (JNI) compiled
##
AC_SUBST([H4_JAVACFLAGS])
AC_SUBST([H4_JAVAFLAGS])

## This needs to be exposed for the library info file even if Java is disabled.
AC_SUBST([HDF_JAVA])

## Default is no Java
HDF_JAVA=no

AC_SUBST([H4_CLASSPATH]) H4_CLASSPATH=""
AC_MSG_CHECKING([if Java JNI interface enabled])

AC_ARG_ENABLE([java],
              [AS_HELP_STRING([--enable-java],
                              [Compile the Java JNI interface [default=no]])],
              [HDF_JAVA=$enableval])

if test "X$HDF_JAVA" = "Xyes"; then
  if test "X${enable_shared}" != "Xno"; then
    AC_MSG_RESULT([yes])
    if test "X$CLASSPATH" = "X"; then
      H4_CLASSPATH=".:$srcdir/java/lib"
    else
      H4_CLASSPATH=".:$CLASSPATH:$srcdir/java/lib"
    fi
    ## Checks for programs.
    AX_JAVA_OPTIONS
    H4_JAVACFLAGS=$JAVACFLAGS
    H4_JAVAFLAGS=$JAVAFLAGS
    AX_PROG_JAVAC
    AX_PROG_JAVA
    AX_PROG_JAR
    AX_PROG_JAVADOC
    ## Find the include directories needed for building JNI code
    AX_JNI_INCLUDE_DIR()
    for JNI_INCLUDE_DIR in $JNI_INCLUDE_DIRS
    do
      JNIFLAGS="$JNIFLAGS -I$JNI_INCLUDE_DIR"
    done
    ## Find junit for testing the JNI code
    AX_CHECK_CLASSPATH()
    CLASSPATH_ENV=$H4_CLASSPATH
    AX_CHECK_JUNIT()
    AX_CHECK_JAVA_HOME

    AC_MSG_RESULT([yes])
  else
    AC_MSG_ERROR([Java requires shared libraries to be built])
    HDF_JAVA="no"
    AC_MSG_RESULT([no])
  fi
else
  AC_MSG_RESULT([no])
fi

AC_PROG_LN_S

## ----------------------------------------------------------------------
## Check which archiving tool to use. This needs to be done before
## the LT_INIT macro.
##
if test -z "$AR"; then
  AC_CHECK_PROGS([AR], [ar xar], [:], [$PATH])
fi
AC_SUBST([AR])

# Set the default ar flags to cr
# The Automake default is to use cru and the 'u' causes ar
# to emit warnings on some platforms.
AR_FLAGS=cr

## Export the AR macro so that it will be placed in the libtool file
## correctly.
export AR

AC_PROG_MAKE_SET
AC_PROG_INSTALL

AC_CHECK_PROG([DIFF],     [diff],     [diff -w])
AC_CHECK_PROG([MAKEINFO], [makeinfo], [makeinfo])
AC_CHECK_PROG([NEQN],     [neqn],     [neqn])
AC_CHECK_PROG([TBL],      [tbl],      [tbl])

AC_SUBST([DIFF])
AC_SUBST([STATIC_SHARED])
AC_SUBST([SHARED_EXTENSION])
AC_SUBST([enable_shared])
AC_SUBST([enable_static])
AC_SUBST([STATIC_EXEC]) STATIC_EXEC=no
AC_SUBST([LT_STATIC_EXEC])

## ----------------------------------------------------------------------
## Check if they would like to enable building doxygen files
##

## This needs to be exposed for the library info file.
AC_SUBST([HDF4_DOXYGEN])

## Default is to not build DOXYGEN
HDF4_DOXYGEN=no

AC_MSG_CHECKING([if building doxygen is enabled])

AC_ARG_ENABLE([doxygen],
              [AS_HELP_STRING([--enable-doxygen],
                              [Compile the HDF4 doxygen files [default=no]])],
              [HDF4_DOXYGEN=$enableval])

AC_MSG_RESULT([$HDF4_DOXYGEN])

## Check if they would like to enable doxygen warnings as errors
##

## This needs to be exposed for the library info file.
AC_SUBST([HDF4_DOXY_WARNINGS])

## Default is to consider doxygen warnings as errors
DOXY_ERR=yes

AC_MSG_CHECKING([if doxygen warnings as errors is enabled])

AC_ARG_ENABLE([doxygen-errors],
              [AS_HELP_STRING([--enable-doxygen-errors],
                              [Error on HDF4 doxygen warnings [default=yes]])],
              [DOXY_ERR=$enableval])

if test "X$DOXY_ERR" = "Xyes"; then
    HDF4_DOXY_WARNINGS="FAIL_ON_WARNINGS"
else
    HDF4_DOXY_WARNINGS="NO"

fi
AC_MSG_RESULT([$HDF4_DOXY_WARNINGS])

if test "X$HDF4_DOXYGEN" = "Xyes"; then
  DX_DOXYGEN_FEATURE(ON)
  DX_DOT_FEATURE(OFF)
  DX_HTML_FEATURE(ON)
  DX_CHM_FEATURE(OFF)
  DX_CHI_FEATURE(OFF)
  DX_MAN_FEATURE(ON)
  DX_RTF_FEATURE(OFF)
  DX_XML_FEATURE(OFF)
  DX_PDF_FEATURE(OFF)
  DX_PS_FEATURE(OFF)

  AC_SUBST([DOXYGEN_PACKAGE])
  AC_SUBST([DOXYGEN_VERSION_STRING])
  AC_SUBST([DOXYGEN_DIR])
  AC_SUBST([DOXYGEN_INCLUDE_ALIASES])
  AC_SUBST([DOXYGEN_PROJECT_LOGO])
  AC_SUBST([DOXYGEN_PROJECT_BRIEF])
  AC_SUBST([DOXYGEN_INPUT_DIRECTORY])
  AC_SUBST([DOXYGEN_OPTIMIZE_OUTPUT_FOR_C])
  AC_SUBST([DOXYGEN_MACRO_EXPANSION])
  AC_SUBST([DOXYGEN_OUTPUT_DIRECTORY])
  AC_SUBST([DOXYGEN_EXAMPLES_DIRECTORY])
  AC_SUBST([DOXYGEN_LAYOUT_FILE])
  AC_SUBST([DOXYGEN_HTML_HEADER])
  AC_SUBST([DOXYGEN_HTML_FOOTER])
  AC_SUBST([DOXYGEN_HTML_EXTRA_STYLESHEET])
  AC_SUBST([DOXYGEN_HTML_EXTRA_FILES])
  AC_SUBST([DOXYGEN_TAG_FILE])
  AC_SUBST([DOXYGEN_SERVER_BASED_SEARCH])
  AC_SUBST([DOXYGEN_EXTERNAL_SEARCH])
  AC_SUBST([DOXYGEN_SEARCHENGINE_URL])
  AC_SUBST([DOXYGEN_STRIP_FROM_PATH])
  AC_SUBST([DOXYGEN_STRIP_FROM_INC_PATH])
  AC_SUBST([DOXYGEN_PREDEFINED])

# SRCDIR Environment variables used inside doxygen macro for the source location:
  DOXYGEN_PACKAGE=${PACKAGE_NAME}
  DOXYGEN_VERSION_STRING=${PACKAGE_VERSION}
  DOXYGEN_DIR='$(SRCDIR)/doxygen'
  DOXYGEN_INCLUDE_ALIASES_PATH='$(SRCDIR)/doxygen'
  DOXYGEN_INCLUDE_ALIASES='$(SRCDIR)/doxygen/aliases'
  DOXYGEN_VERBATIM_VARS='DOXYGEN_INCLUDE_ALIASES'
  DOXYGEN_PROJECT_LOGO='$(SRCDIR)/doxygen/img/HDFG-logo.png'
  DOXYGEN_PROJECT_BRIEF='API Reference'
  DOXYGEN_INPUT_DIRECTORY='$(SRCDIR) $(SRCDIR)/doxygen/dox'
  DOXYGEN_OPTIMIZE_OUTPUT_FOR_C=YES
  DOXYGEN_MACRO_EXPANSION=YES
  DOXYGEN_OUTPUT_DIRECTORY=hdf4lib_docs
  DOXYGEN_EXAMPLES_DIRECTORY='$(SRCDIR)'
  DOXYGEN_LAYOUT_FILE='$(SRCDIR)/doxygen/hdf4doxy_layout.xml'
  DOXYGEN_HTML_HEADER='$(SRCDIR)/doxygen/hdf4_header.html'
  DOXYGEN_HTML_FOOTER='$(SRCDIR)/doxygen/hdf4_footer.html'
  DOXYGEN_HTML_EXTRA_STYLESHEET='$(SRCDIR)/doxygen/hdf4doxy.css'
  DOXYGEN_HTML_EXTRA_FILES='$(SRCDIR)/doxygen/hdf4_navtree_hacks.js'
  DOXYGEN_TAG_FILE=hdf4.tag
  DOXYGEN_SERVER_BASED_SEARCH=NO
  DOXYGEN_EXTERNAL_SEARCH=NO
  DOXYGEN_SEARCHENGINE_URL=
  DOXYGEN_STRIP_FROM_PATH='$(SRCDIR)'
  DOXYGEN_STRIP_FROM_INC_PATH='$(SRCDIR)'
  DOXYGEN_PREDEFINED='H4_DOXYGEN'
  DOXYGEN_WARN_AS_ERROR=${HDF4_DOXY_WARNINGS}

  DX_INIT_DOXYGEN([HDF4], [./doxygen/Doxyfile], [hdf4lib_docs])
fi
AM_CONDITIONAL([BUILD_DOXYGEN_CONDITIONAL], [test "X$HDF4_DOXYGEN" = "Xyes"])


## ======================================================================
## Libtool initialization
## ======================================================================
## disable-shared sets the default libtool behavior to disable shared libs
LT_INIT([disable-shared])
LT_OUTPUT

## ----------------------------------------------------------------------
## Check if we should install only statically linked executables.
##   This check needs to occur after libtool is initialized because
##   we check a libtool cache value and may issue a warning based
##   on its result.
AC_MSG_CHECKING([if we should install only statically linked executables])
AC_ARG_ENABLE([static_exec],
              [AS_HELP_STRING([--enable-static-exec],
                              [Install only statically linked executables
                               [default=no]])],
              [STATIC_EXEC=$enableval])

if test "X$STATIC_EXEC" = "Xyes"; then
  AC_MSG_RESULT([yes])
  ## Issue a warning if -static flag is not supported.
  if test "X$lt_cv_prog_compiler_static_works" = "Xno"; then
      AC_MSG_WARN([-static flag not supported on this system; executable won't statically link shared system libraries])
  fi
  LT_STATIC_EXEC="-all-static"
else
  AC_MSG_RESULT([no])
  LT_STATIC_EXEC=""
fi

AC_SUBST([LT_STATIC_EXEC])

## ======================================================================
## Checks for libraries
## ======================================================================

## ----------------------------------------------------------------------
## Fake --with-xxx option to allow us to create a help message for the
## following --with-xxx options which can take either a =DIR or =INC,LIB
## specifier.
##
AC_ARG_WITH([fnord],
  [
 For the following --with-xxx options, you can specify where the header
 files and libraries are in two different ways:

    --with-xxx=INC,LIB - Specify individually the include directory and
                         library directory separated by a comma
    --with-xxx=DIR     - Specify only the directory which contains the
                         include/ and lib/ subdirectories
  ])

## ----------------------------------------------------------------------
## Is the GNU zlib present? It has a header file `zlib.h' and a library
## `-lz' and their locations might be specified with the `--with-zlib'
## command-line switch. The value is an include path and/or a library path.
## If the library path is specified then it must be preceded by a comma.
AC_ARG_WITH([zlib],
            [AS_HELP_STRING([--with-zlib=DIR],
                            [Use zlib library [default=yes]])],,
            [withval=yes])

case "X-$withval" in
  X-yes)
    HAVE_ZLIB="yes"
    AC_CHECK_HEADERS([zlib.h], [HAVE_ZLIB_H="yes"], [unset HAVE_ZLIB])
    if test "x$HAVE_ZLIB" = "xyes" -a "x$HAVE_ZLIB_H" = "xyes"; then
      AC_CHECK_LIB([z], [compress2],, [unset HAVE_ZLIB])
    fi
    if test -z "$HAVE_ZLIB"; then
      if test -n "$HDF4_CONFIG_ABORT"; then
        AC_MSG_ERROR([couldn't find zlib library])
      fi
    else
      AC_CHECK_FUNC([compress2], [HAVE_COMPRESS2="yes"])
    fi
    ;;
  X-|X-no|X-none)
    HAVE_ZLIB="no"
    AC_MSG_ERROR([zlib library required to build HDF4])
    ;;
  *)
    HAVE_ZLIB="yes"
    case "$withval" in
      *,*)
        zlib_inc="`echo $withval | cut -f1 -d,`"
        zlib_lib="`echo $withval | cut -f2 -d, -s`"
        ;;
      *)
        if test -n "$withval"; then
          zlib_inc="$withval/include"
          zlib_lib="$withval/lib"
        fi
        ;;
    esac

    ## Trying to include -I/usr/include and -L/usr/lib is redundant and
    ## can mess some compilers up.
    if test "X$zlib_inc" = "X/usr/include"; then
      zlib_inc=""
    fi
    if test "X$zlib_lib" = "X/usr/lib"; then
      zlib_lib=""
    fi

    if test -n "$zlib_inc"; then
      CPPFLAGS="$CPPFLAGS -I$zlib_inc"
    fi

    AC_CHECK_HEADERS([zlib.h], [HAVE_ZLIB_H="yes"], [unset HAVE_ZLIB])

    if test -n "$zlib_lib"; then
      LDFLAGS="$LDFLAGS -L$zlib_lib"
    fi

    if test "x$HAVE_ZLIB" = "xyes" -a "x$HAVE_ZLIB_H" = "xyes"; then
      AC_CHECK_LIB([z], [compress2],, [unset HAVE_ZLIB])
    fi

    if test -z "$HAVE_ZLIB"; then
      AC_MSG_ERROR([couldn't find zlib library])
    else
      AC_CHECK_FUNC([compress2], [HAVE_COMPRESS2="yes"])
    fi
    ;;
esac

## ----------------------------------------------------------------------
## Is the JPEG library present?
AC_ARG_WITH([jpeg],
            [AS_HELP_STRING([--with-jpeg=DIR],
                            [Use jpeg library [default=yes]])],,
            [withval=yes])

case "X-$withval" in
  X-yes)
    HAVE_JPEG="yes"
    AC_CHECK_HEADERS([jpeglib.h], [HAVE_JPEG_H="yes"], [unset HAVE_JPEG])
    if test "x$HAVE_JPEG" = "xyes" -a "x$HAVE_JPEG_H" = "xyes"; then
      AC_CHECK_LIB([jpeg], [jpeg_start_decompress],, [unset HAVE_JPEG])
    fi

    if test -z "$HAVE_JPEG"; then
      AC_MSG_ERROR([couldn't find jpeg library])
    fi
    ;;
  X-|X-no|X-none)
    HAVE_JPEG="no"
    AC_MSG_ERROR([jpeg library required to build HDF4])
    ;;
  *)
    HAVE_JPEG="yes"
    case "$withval" in
      *,*)
        jpeg_inc="`echo $withval | cut -f1 -d,`"
        jpeg_lib="`echo $withval | cut -f2 -d, -s`"
        ;;
      *)
        if test -n "$withval"; then
          jpeg_inc="$withval/include"
          jpeg_lib="$withval/lib"
        fi
        ;;
    esac

    ## Trying to include -I/usr/include and -L/usr/lib is redundant and
    ## can mess some compilers up.
    if test "X$jpeg_inc" = "X/usr/include"; then
      jpeg_inc=""
    fi
    if test "X$jpeg_lib" = "X/usr/lib"; then
      jpeg_lib=""
    fi

    if test -n "$jpeg_inc"; then
      CPPFLAGS="$CPPFLAGS -I$jpeg_inc"
    fi

    AC_CHECK_HEADERS([jpeglib.h], [HAVE_JPEG_H="yes"], [unset HAVE_JPEG])

    if test -n "$jpeg_lib"; then
      LDFLAGS="$LDFLAGS -L$jpeg_lib"
    fi

    if test "x$HAVE_JPEG" = "xyes" -a "x$HAVE_JPEG_H" = "xyes"; then
      AC_CHECK_LIB([jpeg], [jpeg_start_decompress],, [unset HAVE_JPEG])
    fi

    if test -z "$HAVE_JPEG"; then
      AC_MSG_ERROR([couldn't find jpeg library])
    fi
    ;;
esac

## ----------------------------------------------------------------------
## Is the szip library present?
AC_SUBST(USE_COMP_SZIP) USE_COMP_SZIP="no"
AC_SUBST(SZIP_HAS_ENCODER) SZIP_HAS_ENCODER="no"
AC_ARG_WITH([szlib],
            [AS_HELP_STRING([--with-szlib=DIR],
                            [Use szlib library [default=no]])],,
            [withval=no])

case "X-$withval" in
  X-yes)
    HAVE_SZIP="yes"
    AC_CHECK_HEADERS([szlib.h], [HAVE_SZLIB_H="yes"], [unset HAVE_SZIP])
    if test "x$HAVE_SZIP" = "xyes" -a "x$HAVE_SZLIB_H" = "xyes"; then
      AC_CHECK_LIB([sz], [SZ_BufftoBuffCompress],, [unset HAVE_SZIP])
    fi

    if test -z "$HAVE_SZIP"; then
      AC_MSG_ERROR([couldn't find szlib library])
    else
      USE_COMP_SZIP="yes"
    fi
    ;;
  X-|X-no|X-none)
    HAVE_SZIP="no"
    AC_MSG_CHECKING([for szlib])
    AC_MSG_RESULT([suppressed])
    SZIP_INFO="${SZIP_INFO}disabled"
    ;;
  *)
    HAVE_SZIP="yes"
    case "$withval" in
      *,*)
        szip_inc="`echo $withval | cut -f1 -d,`"
        szip_lib="`echo $withval | cut -f2 -d, -s`"
        ;;
      *)
        if test -n "$withval"; then
          szip_inc="$withval/include"
          szip_lib="$withval/lib"
        fi
        ;;
    esac

    ## Trying to include -I/usr/include and -L/usr/lib is redundant and
    ## can mess some compilers up.
    if test "X$szip_inc" = "X/usr/include"; then
      szip_inc=""
    fi
    if test "X$szip_lib" = "X/usr/lib"; then
      szip_lib=""
    fi

    if test -n "$szip_inc"; then
      CPPFLAGS="$CPPFLAGS -I$szip_inc"
    fi

    AC_CHECK_HEADERS([szlib.h], [HAVE_SZLIB_H="yes"], [unset HAVE_SZIP])

    if test -n "$szip_lib"; then
      LDFLAGS="$LDFLAGS -L$szip_lib"
    fi

    if test "x$HAVE_SZIP" = "xyes" -a "x$HAVE_SZLIB_H" = "xyes"; then
      AC_CHECK_LIB([sz], [SZ_BufftoBuffCompress],, [unset HAVE_SZIP])
    fi

    if test -z "$HAVE_SZIP"; then
      AC_MSG_ERROR([couldn't find szlib library])
    else
      USE_COMP_SZIP="yes"
    fi
    ;;
esac

## Check to see if SZIP has encoder
if test "X$HAVE_SZIP" = "Xyes" -a "x$HAVE_SZLIB_H" = "xyes"; then
    ## SZLIB library is available. Check if it can encode.
    AC_MSG_CHECKING([for szlib encoder])

    ## Set LD_LIBRARY_PATH so encoder test can find the library and run.
    if test -z "$LD_LIBRARY_PATH"; then
        export LD_LIBRARY_PATH="$szip_lib"
    else
        export LD_LIBRARY_PATH="${szip_lib}:$LD_LIBRARY_PATH"
    fi

    AC_SUBST([LL_PATH]) LL_PATH="$LD_LIBRARY_PATH"

    AC_RUN_IFELSE([AC_LANG_SOURCE([[
    #include <stdlib.h>
    #include <szlib.h>

    int main(void)
    {
        /* SZ_encoder_enabled returns 1 if encoder is present */
        if (SZ_encoder_enabled() == 1)
            exit(0);
        else
            exit(1);
    }
    ]])],[CAN_ENCODE="yes"],[CAN_ENCODE="no"],[])

    ## Report szip encoder test results
    if test "X$CAN_ENCODE" = "Xyes"; then
        AC_MSG_RESULT([yes])
    fi
    if test "X$CAN_ENCODE" = "Xno"; then
        AC_MSG_RESULT([no])
    fi

    ## Add "szip" to external filter list
    if test "X$CAN_ENCODE" = "Xyes"; then
        if test "X$SZIP_INFO" != "X"; then
            SZIP_INFO="${SZIP_INFO}"
        fi
        SZIP_INFO="${SZIP_INFO}enabled with encoder"
    fi

    if test "X$CAN_ENCODE" = "Xno"; then
        if test "X$SZIP_INFO" != "X"; then
            SZIP_INFO="${SZIP_INFO}"
        fi
        SZIP_INFO="${SZIP_INFO}enabled with decoder only"
    fi

    ## Create macro to specify if encoder is present
    if test "X$CAN_ENCODE" = "Xyes"; then
        AC_DEFINE([HAVE_SZIP_ENCODER], [1],
                [Define if szip has encoder])
        SZIP_HAS_ENCODER="yes"
    fi
fi
AC_SUBST([SZIP_INFO])
AC_SUBST([SZIP_HAS_ENCODER])

AM_CONDITIONAL([BUILD_SHARED_SZIP_CONDITIONAL], [test "X$USE_COMP_SZIP" = "Xyes" && test "X$LL_PATH" != "X"])

## ======================================================================
## Set POSIX level
## ======================================================================

## This is needed so strdup, etc. are exposed properly
##
## POSIX feature information can be found in the gcc manual at:
## http://www.gnu.org/s/libc/manual/html_node/Feature-Test-Macros.html
CPPFLAGS="-D_POSIX_C_SOURCE=200809L $CPPFLAGS"

## ======================================================================
## Checks for headers
## ======================================================================
AC_CHECK_HEADERS([fcntl.h unistd.h])

AC_CHECK_HEADERS([sys/file.h sys/resource.h sys/stat.h sys/time.h sys/wait.h])
AC_CHECK_HEADERS([sys/types.h])

AC_CHECK_HEADERS([io.h])
AC_CHECK_HEADERS([arpa/inet.h netinet/in.h])

## Special MinGW checks
case "`uname`" in
    MINGW*)
      # Check for Winsock library
      AC_CHECK_LIB([ws2_32], [htonl])
      ;;
esac

## ----------------------------------------------------------------------
## XDR grew up in a world where long integers were 32 bits in size. Now
## that many platforms have 64-bit longs, we need some hacks to paper
## over the differences in sizes, particularly on big-endian machines.
##

AC_CHECK_SIZEOF([long])

## ----------------------------------------------------------------------

AC_PROG_CC_C_O

if test "X$BUILD_FORTRAN" = "Xyes"; then
  AC_PROG_F77_C_O
fi

## ----------------------------------------------------------------------
## Set some variables for general configuration information to be saved
## and installed with the libraries.
##

## HDF4 version from the first line of the README.md file.
H4_VERSION="`cut -d' ' -f3 $srcdir/README.md | head -1`"
AC_SUBST([H4_VERSION])

## Configuration date
AC_SUBST([CONFIG_DATE]) CONFIG_DATE="`date`"

## User doing the configuration
AC_SUBST([CONFIG_USER]) CONFIG_USER="`whoami`@`hostname`"

## Configuration mode (production, development, etc)
AC_SUBST([CONFIG_MODE])

AC_C_BIGENDIAN

## Are we building this in debug or production mode?
AC_MSG_CHECKING([for build mode])
AC_ARG_ENABLE([production],
              [AS_HELP_STRING([--enable-production],
                              [Determines how to run the compiler.])])

case "X-$enable_production" in
  X-|X-yes)
    AC_MSG_RESULT([production])
    CONFIG_MODE=production
    CFLAGS="$CFLAGS $PROD_CFLAGS"
    FFLAGS="$FFLAGS $PROD_FFLAGS"
    CPPFLAGS="$CPPFLAGS -DNDEBUG $PROD_CPPFLAGS"
    ;;
  X-no)
    AC_MSG_RESULT([development])
    CONFIG_MODE=development
    CFLAGS="$CFLAGS $DEBUG_CFLAGS"
    FFLAGS="$FFLAGS $DEBUG_FFLAGS"
    CPPFLAGS="$CPPFLAGS -UNDEBUG $DEBUG_CPPFLAGS"
    ;;
  *)
    AC_MSG_RESULT([user-defined])
    CONFIG_MODE="user-defined (most CFLAGS, etc. set by user)"
    ;;
esac

## ======================================================================
## Checks for library functions
## ======================================================================

AC_MSG_CHECKING([for math library support])
AC_LINK_IFELSE([AC_LANG_PROGRAM([[#include <math.h>]], [[sinh(37.927)]])],[AC_MSG_RESULT([yes])],[AC_MSG_RESULT([no]); LIBS="$LIBS -lm"])

AC_CHECK_FUNCS([fork getrusage system wait])


## ======================================================================
## Checks for system services
## ======================================================================

## Copy NetCDF header files.
#
## FIXME: This is code borrowed from the old HDF4 configure.
## These header files should probably be generated by autoconf instead
## of being copied depending upon the host.
case "$host" in
  *-linux*|*-k*bsd*-gnu|*-gnu*)BAR="linux"       ;;
  *-freebsd*)           BAR="fbsd"        ;;
  *-ibm-aix*)           BAR="aix"         ;;
  sparc64-*-solaris2*)  BAR="solaris64"   ;;
  *-*-solaris2*)        BAR="solaris"     ;;
  *-apple*)             BAR="apple"       ;;
  *-pc-cygwin*)         BAR="linux"       ;;
  *)                    echo "*** unknown host $host!"; exit 1 ;;
esac
src_files=""
src_files="`echo $src_files | sed -e s/FOO/${BAR}/g`"

for config_file in $src_files; do
  src_file="${srcdir}/`echo $config_file | sed -e s/:.*$//`"
  target_file="`echo $config_file | sed -e s/^[[^:]]*://g`"

  if test ! -r ${src_file}; then
    echo '***' "${progname}: cannot copy file \"${target_file}\"," 1>&2
    echo '***' "since the file \"${src_file}\" does not exist." 1>&2
    exit 1
  fi

  ## this sed command emulates the dirname command
  dstdir=`echo "$target_file" | sed -e 's,[[^/]]*$,,;s,/$,,;s,^$,.,'`

  ## Create directory for the target_file if it's not there.
  if test ! -d $dstdir; then
    ${srcdir}/bin/mkinstalldirs $dstdir
  fi

  echo "Copying \"${src_file}\" to \"${target_file}\" ."
  cp ${src_file} ${target_file}

  if test ! -r ${target_file}; then
    echo '***' "${progname}: cannot copy file \"${target_file}\"," 1>&2
    exit 1
  fi
done

## ------------------------------------------------------------------------
## Check to see if libtool has enabled shared libraries. Set a conditional
## as some Makefiles will build based on availability of shared libraries.
if (./libtool --features | grep '^enable shared libraries' > /dev/null); then
  enable_shared=yes
else
  enable_shared=no
fi

## ------------------------------------------------------------------------
## Specify shared library extension the host machine should recognize.
case "$host_os" in
  darwin*)
      SHARED_EXTENSION="dylib"
      ;;
  *)
      SHARED_EXTENSION="so"
      ;;
esac

## We don't need to say when we're entering directories if we're using
## GNU make because make does it for us.
if test "X$GMAKE" = "Xyes"; then
  AC_SUBST([SETX]) SETX=":"
else
  AC_SUBST([SETX]) SETX="set -x"
fi

AM_CONDITIONAL([HDF_BUILD_SHARED], [test "X$enable_shared" = "Xyes"])

## Compiler with version information. This consists of the full path
## name of the compiler and the reported version number.
AC_SUBST([CC_VERSION])
## Strip anything that looks like a flag off of $CC
CC_NOFLAGS=`echo $CC | sed 's/ -.*//'`

if `echo $CC_NOFLAGS | grep ^/ >/dev/null 2>&1`; then
  CC_VERSION="$CC"
else
  CC_VERSION="$CC";
  for x in `echo $PATH | sed -e 's/:/ /g'`; do
    if test -x $x/$CC_NOFLAGS; then
      CC_VERSION="$x/$CC"
      break
    fi
  done
fi


## ----------------------------------------------------------------------
##
## If --enable-static-exec and are specified together, there will be ld failures for
## "attempted static link of dynamic object" when the tools are built.  This check
## will prevent that error during configure instead.  It could go with the other
## --enable-static-exec checks, but since enable_static is set by default later in
## the configure process this check has to be delayed.
if test "X$STATIC_EXEC" = "Xyes"; then
  if test "X${enable_static}" != "Xyes"; then
    AC_MSG_ERROR([--enable-static-exec flag to build static executables requires static libraries.  Please configure with --enable-static flag.])
  fi
fi

## This part doesn't work yet since HDF4 config files do not contain
## information for cc_vendor and cc_version as HDF4 similar files do.
## Needs to be fixed EIP 2010-01-21
## if test -n "$cc_vendor" && test -n "$cc_version"; then
##  CC_VERSION="$CC_VERSION ($cc_vendor-$cc_version)"
## fi

if test -n "$cc_version_info"; then
  CC_VERSION="$CC_VERSION ( $cc_version_info)"
fi

## Fortran compiler with version information. This consists of the full path
## name of the compiler and the reported version number.
AC_SUBST([F77_VERSION])
## Strip anything that looks like a flag off of $F77
F77_NOFLAGS=`echo $F77 | sed 's/ -.*//'`

if `echo $F77_NOFLAGS | grep ^/ >/dev/null 2>&1`; then
  F77_VERSION="$F77"
else
  F77_VERSION="$F77";
  for x in `echo $PATH | sed -e 's/:/ /g'`; do
    if test -x $x/$F77_NOFLAGS; then
      F77_VERSION="$x/$F77"
      break
    fi
  done
fi
if test -n "$fc_version_info"; then
  F77_VERSION="$F77_VERSION ( $fc_version_info)"
fi

## This part doesn't work yet since HDF4 config files do not contain
## information for fortran_vendor and fortran_version.
## Needs to be fixed EIP 2010-01-21
## if test -n "$fortran_vendor" && test -n "$fortran_version"; then
##   F77_VERSION="$F77_VERSION ($fortran_vendor-$fortran_version)"
## fi

AC_SUBST([JAVA_VERSION])
## Strip anything that looks like a flag off of $JAVA
JAVA_NOFLAGS=`echo $JAVA | sed 's/ -.*//'`

if `echo $JAVA_NOFLAGS | grep ^/ >/dev/null 2>&1`; then
  JAVA_VERSION="$JAVA"
else
  JAVA_VERSION="$JAVA";
  for x in `echo $PATH | sed -e 's/:/ /g'`; do
    if test -x $x/$JAVA_NOFLAGS; then
      JAVA_VERSION="$x/$JAVA"
      break
    fi
  done
fi
java_version_info=`$JAVA -version 2>&1 |\
  grep 'version' | sed -e 's/version "//' | sed -e 's/"//'`
if test -n "$java_version_info"; then
  JAVA_VERSION="$JAVA_VERSION ($java_version_info)"
fi
AM_CONDITIONAL([BUILD_JAVA_CONDITIONAL], [test "X$HDF_JAVA" = "Xyes"])

## ----------------------------------------------------------------------
## Some platforms require that all symbols are resolved when a library
## is linked. We can use the -no-undefined flag to tell libtool that
## it will be able to build shared libraries on these architectures,
## as it will not do so by default.
##
if test "X${enable_shared}" = "Xyes"; then
  AC_MSG_CHECKING([if libtool needs -no-undefined flag to build shared libraries])
  case "`uname`" in
    CYGWIN*|MINGW*|AIX*)
      ## Add in the -no-undefined flag to LDFLAGS for libtool.
      AC_MSG_RESULT([yes])
      LDFLAGS="$LDFLAGS -no-undefined"
      ;;
    *)
      ## Don't add in anything.
      AC_MSG_RESULT([no])
      ;;
  esac
fi


## ----------------------------------------------------------------------
## Enable deprecated public API symbols
##
AC_SUBST([DEPRECATED_SYMBOLS])
AC_MSG_CHECKING([if deprecated public symbols are available]);
AC_ARG_ENABLE([deprecated-symbols],
              [AS_HELP_STRING([--enable-deprecated-symbols],
                     [Enable deprecated public API symbols [default=yes]])],
             [DEPREC_SYMBOLS=$enableval],
             [DEPREC_SYMBOLS=yes])

case "X-$DEPREC_SYMBOLS" in
  X-yes)
    AC_MSG_RESULT([yes])
    DEPRECATED_SYMBOLS=yes
    ;;
  X-no|*)
    AC_MSG_RESULT([no])
    DEPRECATED_SYMBOLS=no
    AC_DEFINE([NO_DEPRECATED_SYMBOLS], [1],
              [Define if deprecated public API symbols are disabled])
    ;;
esac

AC_CONFIG_FILES([Makefile
                 doxygen/Doxyfile
                 libhdf4.settings
                 hdf/Makefile
                 hdf/fortran/Makefile
                 hdf/src/Makefile
                 hdf/test/Makefile
                 hdf/test/srcdir_str.h
                 hdf/util/Makefile
                 hdf/util/h4cc
                 hdf/util/h4fc
                 hdf/util/h4redeploy
                 hdf/util/testutil.sh
                 mfhdf/fortran/ftest.f
                 mfhdf/fortran/jackets.c
                 mfhdf/fortran/netcdf.inc
                 mfhdf/libsrc/netcdf.h
                 mfhdf/Makefile
                 mfhdf/dumper/Makefile
                 mfhdf/dumper/testhdp.sh
                 mfhdf/fortran/Makefile
                 mfhdf/fortran/testfortran.sh
                 mfhdf/hdfimport/Makefile
                 mfhdf/hdfimport/testutil.sh
                 mfhdf/hdiff/Makefile
                 mfhdf/hdiff/testhdiff.sh
                 mfhdf/hrepack/Makefile
                 mfhdf/hrepack/hrepack.sh
                 mfhdf/hrepack/hrepack_all.sh
                 mfhdf/libsrc/Makefile
                 mfhdf/ncdump/Makefile
                 mfhdf/ncdump/testncdump.sh
                 mfhdf/ncgen/Makefile
                 mfhdf/ncgen/testncgen.sh
                 mfhdf/nctest/Makefile
                 mfhdf/test/Makefile
                 mfhdf/test/srcdir_str.h
                 mfhdf/test/testmfhdf.sh
                 java/Makefile
                 java/src/Makefile
                 java/src/jni/Makefile
                 java/test/Makefile
                 java/test/junit.sh])

AC_CONFIG_COMMANDS([.classes], [], [$MKDIR_P java/src/.classes;
                $MKDIR_P java/test/.classes])

AC_OUTPUT

## A temporary patch. These code were after where it did
## cp config/netcdf-XXX.h netcdf.h before.
## Moved it here after AC_CONFIG_FILES(... mfhdf/libsrc/netcdf.h ).
## Need a better and correct solution.
##
if test "X$BUILD_NETCDF" != "Xyes"; then
    echo "Renaming \"mfhdf/libsrc/netcdf.h\" to \"mfhdf/libsrc/hdf4_netcdf.h\" ."
    mv mfhdf/libsrc/netcdf.h mfhdf/libsrc/hdf4_netcdf.h
fi

chmod 755 hdf/util/h4cc hdf/util/h4redeploy

if test "X$BUILD_FORTRAN" = "Xyes"; then
  chmod 755 hdf/util/h4fc
fi

## show the configure settings
cat libhdf4.settings

