dnl Process this file with autoconf to produce configure.
dnl
dnl Copyright by the Board of Trustees of the University of Illinois.
dnl All rights reserved.
dnl
AC_REVISION($Id$)
AC_PREREQ([2.57])   dnl We need autoconf version 2.57 or greater

dnl ----------------------------------------------------------------------
dnl Initialize configure.
dnl ----------------------------------------------------------------------
dnl AC_INIT takes the name of the package, the version number, and an
dnl email address to report bugs. AC_CONFIG_SRCDIR takes a unique file
dnl as its argument.
dnl
dnl NOTE: Don't forget to change the version number here when we do a
dnl release!!!
dnl
AC_INIT([HDF], [4.1r5], [hdfhelp@ncsa.uiuc.edu])
AM_INIT_AUTOMAKE([HDF], [4.1r5])

dnl ----------------------------------------------------------------------
dnl Information on the package
dnl ----------------------------------------------------------------------

AC_CONFIG_SRCDIR([hdf/src/atom.c])
AC_CONFIG_AUX_DIR([bin])

dnl Dump all shell variables values.
AC_MSG_CHECKING([shell variables initial values])
set >&AS_MESSAGE_LOG_FD
AC_MSG_RESULT([done])

AC_CANONICAL_HOST

dnl Source any special files that we need. These files normally aren't
dnl present but can be used by the maintainers to fine tune things like
dnl turning on debug or profiling flags for the compiler. The search order
dnl is:
dnl
dnl	CPU-VENDOR-OS
dnl	VENDOR-OS
dnl	CPU-OS
dnl	CPU-VENDOR
dnl	OS
dnl	VENDOR
dnl	CPU
dnl
dnl If the `OS' ends with a version number then remove it. For instance,
dnl `freebsd3.1' would become `freebsd'
dnl
case $host_os in
  aix4.*)       host_os_novers="aix4.x"     ;;
  aix5.*)       host_os_novers="aix5.x"     ;;
  freebsd*)     host_os_novers="freebsd"    ;;
  irix5.*)      host_os_novers="irix5.x"    ;;
  irix6.*)      host_os_novers="irix6.x"    ;;
  osf4.*)       host_os_novers="osf4.x"     ;;
  osf5.*)       host_os_novers="osf5.x"     ;;
  solaris2.*)   host_os_novers="solaris2.x" ;;
  *)            host_os_novers="$host_os"   ;;
esac

host_config="none"
for f in $host_cpu-$host_vendor-$host_os \
         $host_cpu-$host_vendor-$host_os_novers \
         $host_vendor-$host_os \
         $host_vendor-$host_os_novers \
         $host_cpu-$host_os \
         $host_cpu-$host_os_novers \
         $host_cpu-$host_vendor \
         $host_os \
         $host_os_novers \
         $host_vendor \
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
  . $host_config
fi

dnl ----------------------------------------------------------------------
dnl Checks for programs
dnl ----------------------------------------------------------------------

AC_PROG_MAKE_SET

AC_PROG_CC
AC_PROG_F77
AC_PROG_CXX

AC_PROG_INSTALL
AC_PROG_LEX
AC_PROG_LN_S
AC_PROG_RANLIB
AC_PROG_YACC

AC_CHECK_PROGS([AR], [ar xar], [:], [$PATH])

AC_CHECK_PROG([DIFF],   [diff],     [diff -w])
AC_CHECK_PROG([NEQN],   [neqn],     [neqn])
AC_CHECK_PROG([TBL],    [tbl],      [tbl])

AC_SUBST([AR])
AC_SUBST([DIFF])

dnl ----------------------------------------------------------------------
dnl Checks for libraries
dnl ----------------------------------------------------------------------

dnl Is the GNU zlib present? It has a header file `zlib.h' and a library
dnl `-lz' and their locations might be specified with the `--with-zlib'
dnl command-line switch. The value is an include path and/or a library path.
dnl If the library path is specified then it must be preceded by a comma.
AC_ARG_WITH([zlib],
            [AC_HELP_STRING([--with-zlib=DIR],
                            [Use zlib library for external deflate I/O
                             filter [default=yes]])],,
            withval=yes)

case $withval in
  yes)
    HAVE_ZLIB="yes"
    AC_CHECK_HEADERS([zlib.h], [HAVE_ZLIB_H="yes"])
    AC_CHECK_LIB([z], [compress2],, [unset HAVE_ZLIB])

    if test -z "$HAVE_ZLIB"; then
      AC_MSG_ERROR([couldn't find zlib library])
    fi
    ;;
  no)
    AC_MSG_ERROR([zlib library required to build HDF4])
    ;;
  *)
    HAVE_ZLIB="yes"
    case "$withval" in
      *,*)
        zlib_inc="`echo $withval |cut -f1 -d,`"
        zlib_lib="`echo $withval |cut -f2 -d, -s`"
        ;;
      *)
        if test -n "$withval"; then
          zlib_inc="$withval/include"
          zlib_lib="$withval/lib"
        fi
        ;;
    esac

    dnl Trying to include -I/usr/include and -L/usr/lib is redundant and
    dnl can mess some compilers up.
    if test "X$zlib_inc" = "X/usr/include"; then
      zlib_inc=""
    fi
    if test "X$zlib_lib" = "X/usr/lib"; then
      zlib_lib=""
    fi

    saved_CPPFLAGS="$CPPFLAGS"
    saved_LDFLAGS="$LDFLAGS"

    if test -n "$zlib_inc"; then
      CPPFLAGS="$CPPFLAGS -I$zlib_inc"
    fi

    AC_CHECK_HEADERS([zlib.h],
                     [HAVE_ZLIB_H="yes"],
                     [CPPFLAGS="$saved_CPPFLAGS"])

    if test -n "$zlib_lib"; then
      LDFLAGS="$LDFLAGS -L$zlib_lib"
    fi

    AC_CHECK_LIB([z], [compress2],,
                 [LDFLAGS="$saved_LDFLAGS"; unset HAVE_ZLIB])

    if test -z "$HAVE_ZLIB"; then
      AC_MSG_ERROR([couldn't find zlib library])
    fi
    ;;
esac

dnl ----------------------------------------------------------------------
dnl Checks for header files
dnl ----------------------------------------------------------------------

AC_HEADER_STDC

dnl ----------------------------------------------------------------------
dnl Checks for types
dnl ----------------------------------------------------------------------

dnl ----------------------------------------------------------------------
dnl Checks for structures
dnl ----------------------------------------------------------------------

dnl ----------------------------------------------------------------------
dnl Checks for compiler characteristics
dnl ----------------------------------------------------------------------

AC_PROG_CC_C_O
AC_PROG_F77_C_O
AC_PROG_CXXCPP

AC_SUBST([SWAP])
AC_C_BIGENDIAN([SWAP=""], [SWAP="-DSWAP"])

dnl ----------------------------------------------------------------------
dnl Checks for library functions
dnl ----------------------------------------------------------------------

AC_MSG_CHECKING([for math library support])
AC_TRY_LINK([#include <math.h>], [fabs(-37.927)],
            [AC_MSG_RESULT([yes])],
            [AC_MSG_RESULT([no]); LIBS="$LIBS -lm"])

dnl ----------------------------------------------------------------------
dnl Checks for system services
dnl ----------------------------------------------------------------------

AC_CONFIG_FILES([Makefile
                 hdf/Makefile
                 hdf/jpeg/Makefile
                 hdf/src/Makefile
                 hdf/test/Makefile
                 hdf/util/Makefile])

AC_OUTPUT
