dnl Process this file with autoconf to produce configure.
dnl
dnl Copyright by the Board of Trustees of the University of Illinois.
dnl All rights reserved.
dnl
AC_REVISION($Id$)

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
AC_CONFIG_SRCDIR([hdf/src/atom.c])
AC_CONFIG_AUX_DIR([bin])
AM_INIT_AUTOMAKE([HDF], [4.1r5])

dnl ----------------------------------------------------------------------
dnl Information on the package
dnl ----------------------------------------------------------------------

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
AC_PROG_CPP
AC_PROG_F77
AC_PROG_CXX

AC_PROG_INSTALL
AC_PROG_LEX
AC_PROG_LN_S
AC_PROG_RANLIB
AC_PROG_YACC

AC_CHECK_PROGS([AR], [ar xar], [:], [$PATH])

AC_CHECK_PROG([DIFF],     [diff],     [diff -w])
AC_CHECK_PROG([MAKEINFO], [makeinfo], [makeinfo])
AC_CHECK_PROG([NEQN],     [neqn],     [neqn])
AC_CHECK_PROG([TBL],      [tbl],      [tbl])

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
                            [Use zlib library [default=yes]])],,
            withval=yes)

case $withval in
  yes)
    HAVE_ZLIB="yes"
    AC_CHECK_HEADERS([zlib.h],, [unset HAVE_ZLIB])
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

    dnl Trying to include -I/usr/include and -L/usr/lib is redundant and
    dnl can mess some compilers up.
    if test "X$zlib_inc" = "X/usr/include"; then
      zlib_inc=""
    fi
    if test "X$zlib_lib" = "X/usr/lib"; then
      zlib_lib=""
    fi

    if test -n "$zlib_inc"; then
      CPPFLAGS="$CPPFLAGS -I$zlib_inc"
    fi

    AC_CHECK_HEADERS([zlib.h],, [unset HAVE_ZLIB])

    if test -n "$zlib_lib"; then
      LDFLAGS="$LDFLAGS -L$zlib_lib"
    fi

    AC_CHECK_LIB([z], [compress2],, [unset HAVE_ZLIB])

    if test -z "$HAVE_ZLIB"; then
      AC_MSG_ERROR([couldn't find zlib library])
    fi
    ;;
esac

dnl Is the JPEG library present?
AC_ARG_WITH([jpeg],
            [AC_HELP_STRING([--with-jpeg=DIR],
                            [Use jpeg library [default=yes]])],,
            withval=yes)

case $withval in
  yes)
    HAVE_JPEG="yes"
    AC_CHECK_HEADERS([jpeglib.h],, [unset HAVE_JPEG])
    AC_CHECK_LIB([jpeg], [jpeg_start_decompress],, [unset HAVE_JPEG])

    if test -z "$HAVE_JPEG"; then
      AC_MSG_ERROR([couldn't find jpeg library])
    fi
    ;;
  no)
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

    dnl Trying to include -I/usr/include and -L/usr/lib is redundant and
    dnl can mess some compilers up.
    if test "X$jpeg_inc" = "X/usr/include"; then
      jpeg_inc=""
    fi
    if test "X$jpeg_lib" = "X/usr/lib"; then
      jpeg_lib=""
    fi

    if test -n "$jpeg_inc"; then
      CPPFLAGS="$CPPFLAGS -I$jpeg_inc"
    fi

    AC_CHECK_HEADERS([jpeglib.h],, [unset HAVE_JPEG])

    if test -n "$jpeg_lib"; then
      LDFLAGS="$LDFLAGS -L$jpeg_lib"
    fi

    AC_CHECK_LIB([jpeg], [jpeg_start_decompress],, [unset HAVE_JPEG])

    if test -z "$HAVE_JPEG"; then
      AC_MSG_ERROR([couldn't find jpeg library])
    fi
    ;;
esac

dnl Is the SZip library present?
AC_ARG_WITH([szlib],
            [AC_HELP_STRING([--with-szlib=DIR],
                            [Use szlib library [default=yes]])],,
            withval=yes)

case $withval in
  yes)
    HAVE_SZIP="yes"
    AC_CHECK_HEADERS([szlib.h],, [unset HAVE_SZIP])
    AC_CHECK_LIB([sz], [SZ_BufftoBuffCompress],, [unset HAVE_SZIP])

    if test -z "$HAVE_SZIP"; then
      AC_MSG_ERROR([couldn't find szlib library])
    fi
    ;;
  no)
    AC_MSG_ERROR([szlib library required to build HDF4])
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

    dnl Trying to include -I/usr/include and -L/usr/lib is redundant and
    dnl can mess some compilers up.
    if test "X$szip_inc" = "X/usr/include"; then
      szip_inc=""
    fi
    if test "X$szip_lib" = "X/usr/lib"; then
      szip_lib=""
    fi

    if test -n "$szip_inc"; then
      CPPFLAGS="$CPPFLAGS -I$szip_inc"
    fi

    AC_CHECK_HEADERS([szlib.h],, [unset HAVE_SZIP])

    if test -n "$szip_lib"; then
      LDFLAGS="$LDFLAGS -L$szip_lib"
    fi

    AC_CHECK_LIB([sz], [SZ_BufftoBuffCompress],, [unset HAVE_SZIP])

    if test -z "$HAVE_SZIP"; then
      AC_MSG_ERROR([couldn't find szlib library])
    fi
    ;;
esac

dnl Is XDR support present?
AC_MSG_CHECKING([for xdr library support])

AC_TRY_LINK([#include <rpc/xdr.h>], [xdr_int],
            [AC_MSG_RESULT([yes]); build_xdr="no"],
            [AC_MSG_RESULT([no]); build_xdr="yes"])
AM_CONDITIONAL(HDF_BUILD_XDR, test "X$build_xdr" = "Xyes")

dnl For Solaris and Fuji systems, add the -nsl for XDR support
case "${host}" in
  *-solaris*)           LIBS="$LIBS -lnsl"  ;;
  S370-fujitsu-sysv*)   LIBS="$LIBS -lnsl"  ;;
  *)                    ;;
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

AC_C_BIGENDIAN([], [CPPFLAGS="$CPPFLAGS -DSWAP"])

dnl ----------------------------------------------------------------------
dnl Checks for library functions
dnl ----------------------------------------------------------------------

AC_MSG_CHECKING([for math library support])
AC_TRY_LINK([#include <math.h>], [sqrt(37.927)],
            [AC_MSG_RESULT([yes])],
            [AC_MSG_RESULT([no]); LIBS="$LIBS -lm"])

dnl ----------------------------------------------------------------------
dnl Checks for system services
dnl ----------------------------------------------------------------------

dnl Copy NetCDF header files.
dnl
dnl FIXME: This is code stolen^Wborrowed from the old HDF4 configure.
dnl These header files should probably be generated by autoconf instead
dnl of being copied depending upon the host.
src_files="mfhdf/libsrc/config/netcdf-FOO.h:mfhdf/libsrc/netcdf.h
           mfhdf/fortran/config/jackets-FOO.c:mfhdf/fortran/jackets.c
           mfhdf/fortran/config/netcdf-FOO.inc:mfhdf/fortran/netcdf.inc
           mfhdf/fortran/config/ftest-FOO.f:mfhdf/fortran/ftest.f"

case "${host}" in
  alpha-*-*)            BAR="alpha"       ;;
  mips*-dec-ultrix*)    BAR="dec"         ;;
  mips*-sgi-irix6.*)    BAR="irix6"       ;;
  mips*-sgi-irix6_32*)  BAR="irix32"      ;;
  mips*-sgi-irix5*)     BAR="irix5"       ;;
  mips*-sgi-irix4*)     BAR="irix4"       ;;
  *-linux*)             BAR="linux"       ;;
  *-freebsd*)           BAR="fbsd"        ;;
  ia64-*-*)             BAR="ia64"        ;;
  *-ibm-aix*)           BAR="aix"         ;;
  i386-*-solaris2*)     BAR="solarisx86"  ;;
  sparc64-*-solaris2*)  BAR="solaris64"   ;;
  *-*-solaris2*)        BAR="solaris"     ;;
  *-sun-*)              BAR="sun"         ;;
  *-hp-hpux*)           BAR="hpux"        ;;
  *-convex-bsd*)        BAR="convex"      ;;
  *-cray-unicos*)       BAR="unicos"      ;;
  t3*-cray-craympp)     BAR="t3e"         ;;
  S370-fujitsu-sysv*)   BAR="fujivp"      ;;
  *-mac-*)              BAR="mac"         ;;
  *)                    echo "*** unknown host $host!"; exit 1 ;;
esac

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

AC_CONFIG_FILES([Makefile
                 hdf/Makefile
                 hdf/src/Makefile
                 hdf/test/Makefile
                 hdf/util/Makefile
                 man/Makefile
                 mfhdf/Makefile
                 mfhdf/dumper/Makefile
                 mfhdf/dumper/testhdp.sh
                 mfhdf/fortran/Makefile
                 mfhdf/hdfimport/Makefile
                 mfhdf/hdfimport/testutil.sh
                 mfhdf/hdiff/Makefile
                 mfhdf/hzip/Makefile
                 mfhdf/libsrc/Makefile
                 mfhdf/ncdump/Makefile
                 mfhdf/ncgen/Makefile
                 mfhdf/pablo/Makefile
                 mfhdf/port/Makefile
                 mfhdf/xdr/Makefile])
AC_OUTPUT
