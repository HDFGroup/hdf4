define(diversion_number, divnum)dnl
divert(-1)


# Check for fill value usage.
#
define(UL_FILLVALUES, [dnl
echo checking for fill value usage
if test "${OLD_FILLVALUES-}" = yes; then
    OLD_FILLVALUES=1
    AC_DEFINE(OLD_FILLVALUES)
else
    OLD_FILLVALUES=0
fi
AC_SUBST(OLD_FILLVALUES)dnl
UC_CREATE(libsrc/netcdf.h)dnl
])


# Check endianness.
#
define(UL_SWAP, [dnl
echo checking endianess
SWAP=
AC_TEST_PROGRAM([main() {
    long i = 0;
    char *ip = (char*)&i;
    *ip       = 'a';
    exit(i != 'a'); /* false if little endian or sizeof(long) == sizeof(char) */
}], [SWAP=-DSWAP])dnl
AC_SUBST(SWAP)dnl
])


# Check type of 32-bit `network long' integer.
#
define([UL_NETLONG], [dnl
AC_REQUIRE([UC_OS])dnl
if test "$OS" = unicos; then
NETLONG=
else
echo checking type of netlong
AC_TEST_PROGRAM([main() {exit(sizeof(long) == 4);}],
[NETLONG='-DNETLONG=int'], [NETLONG=])
fi
AC_SUBST(NETLONG)dnl
])


# Check for XDR header-file directory.
#
define(UL_CPP_XDR, [dnl
echo checking for XDR header-file
AC_REQUIRE([UC_PREFIX])dnl
AC_PROVIDE([$0])dnl
UC_TEST_DIR(CPP_XDR, /usr/[[include]]/rpc /usr/local/[[include]]/rpc \
    $prefix/[[include]] UC_ABSPATH(xdr), xdr.h,
    XDR [[[include]]]-directory, -I/usr/[[[include]]]/rpc)dnl
if test -n "${CPP_XDR-}"; then
  CPP_XDR=-I${CPP_XDR}
fi
])


# Ensure a valid XDR library.
#
define(UL_LIB_XDR, [dnl
echo checking XDR library
AC_REQUIRE([UC_OS])dnl
AC_PROVIDE([$0])dnl
case "${OS}" in
  irix*)	LD_XDR=-lsun
#                UC_ENSURE(HDF_INC, -DIRIS4)
                UC_ENSURE(HDF_INC, -cckr);;
  ultrix*)	LD_XDR=
#                UC_ENSURE(HDF_INC, -DMIPSEL);;
  unicos*)	LD_XDR=
		UC_ENSURE(CPPFLAGS, -DBIG_SHORTS -DBIG_LONGS)
#                UC_ENSURE(HDF_INC, -DUNICOS);;
  aix*)         
#               UC_ENSURE(HDF_INC, -DIBM6000)
                LD_XDR=;;
  sunos*)       
#               UC_ENSURE(HDF_INC, -DSUN)
                LD_XDR=;;
  hpux*)        
#               UC_ENSURE(HDF_INC, -DHP9000)
                if test -z "$GCC"; then
                        UC_ENSURE(CPPFLAGS, -Aa)
                fi
                LD_XDR=;;
  osf*)         
#               UC_ENSURE(HDF_INC, -DDEC_ALPHA)
		UC_ENSURE(CPPFLAGS, -DBIG_LONGS)
		UC_ENSURE(CPPFLAGS, -std1)
                LD_XDR=;;
  convex*)      
#               UC_ENSURE(HDF_INC, -DCONVEX)
                LD_XDR=;;
  *)		LD_XDR=;;
esac
UC_ENSURE(DEFS, ${CPP_XDR-})dnl
UC_ENSURE(DEFS, ${CPPFLAGS-})dnl
UC_ENSURE(LIBS, ${LD_XDR-})dnl
AC_TEST_PROGRAM(dnl
[
#undef NDEBUG

#ifdef _AIX
#   ifndef _ALL_SOURCE
#     define _ALL_SOURCE
#   endif
#endif

#include <assert.h>
#include <stdio.h>
#include <types.h>
#include <xdr.h>

#define TESTFILE	"conftest.xdr"
#define EPSILON		.0005
#ifndef FLT_MIN
#   define    FLT_MIN 1e-37           /* Standard C maximum */
#endif
#ifndef FLT_EPSILON
#   define FLT_EPSILON        1e-5            /* Standard C maximum */
#endif

main(ac,av)
    int			ac ;
    char		*av[] ;
{
    int			ii, jj	= 0 ;
    char		*fname	= ac > 1 ? av[1] : TESTFILE;
    FILE		*F	= fopen(fname,"w");
    XDR			xdrs[1] ;
    unsigned int	count ;
    unsigned int	szof ;

    /*
     * The last value below should be a subnormal number on a DECstation,
     * RS6000, and SPARCstation.  Hopefully, it will catch bad xdr(3)
     * implementations (such as a VAXstation's) which don't correctly handle
     * such values.  Such a value should be the last one in the list.
     */
    static float	floats[]	= { 100.125, 100.25, 100.375, 
					    100.5, 100.625, FLT_MIN/32 } ;
    float *fp , got_af[sizeof(floats)/sizeof(floats[0])] ;

/* create */

    if( F == NULL)
    {
	    perror("fopen failed") ;
	    exit(-1) ;
    }
    xdrstdio_create(xdrs, F, XDR_ENCODE) ;

/* populate */

    szof = sizeof(float) ;
    count = sizeof(floats)/sizeof(float) ;
    assert( xdr_vector(xdrs, (char *)floats, count, szof, xdr_float)) ;

/* flush, rewind  and reopen */

    assert(fflush((FILE *)xdrs->x_private) != EOF) ; /* xdr_destroy(xdrs) */

    assert(fclose(F) != EOF) ;
    F = fopen(fname,"r") ;
    if( F == NULL)
    {
	    perror("fopen failed") ;
	    exit(-1) ;
    }
    xdrstdio_create(xdrs, F, XDR_DECODE) ;

/* check */

    szof = sizeof(float) ;
    count = sizeof(floats)/sizeof(float) ;
    assert( xdr_vector(xdrs, (char *)got_af, count, szof, xdr_float)) ;
    for(ii = 0, fp = got_af;
	ii < count - 1;
	ii++, fp++) 
    {
	assert(*fp <= floats[ii]*(1+FLT_EPSILON) && *fp >= floats[ii]*(1-FLT_EPSILON)) ;
    }
    assert(*fp == 0 || (*fp <= floats[ii]*(1+FLT_EPSILON) && *fp >= floats[ii]*(1-FLT_EPSILON))) ;

    exit(0) ;
}
],dnl
XDR_LIBOBJS=
XDR_INSTALL_DEPS=,dnl
XDR_LIBOBJS="xdr.o xdrfloat.o xdrstdio.o xdrarray.o"
XDR_INSTALL_DEPS=installed_headers
LD_XDR=)dnl
AC_SUBST(XDR_LIBOBJS)dnl
AC_SUBST(XDR_INSTALL_DEPS)dnl
AC_SUBST(LD_XDR)dnl
])


# Check for XDR implementation (header file and library).
#
define(UL_XDR, [dnl
AC_REQUIRE([UL_CPP_XDR])dnl
AC_REQUIRE([UL_LIB_XDR])dnl
])


# Define C macros for function prototyping
#
define([UL_PROTOTYPES],
[AC_COMPILE_CHECK([function prototypes], ,
extern int foo(int bar);
, dnl
, dnl
[UC_ENSURE(CPPFLAGS, -DNO_HAVE_PROTOTYPES)])])


# Check for functioning `const' keyword
#
define(UL_CONST,
[AC_COMPILE_CHECK([working const], , [/* Ultrix mips cc rejects this.  */
typedef int charset[2]; const charset x;
], dnl
, dnl
[UC_ENSURE(CPPFLAGS, -Dconst=)])])


# Define macros for variadic function support
#
define(UL_VARIADIC_FUNCTIONS,[dnl
AC_PROVIDE([$0])dnl
AC_REQUIRE([UC_PROG_CPP])dnl
AC_COMPILE_CHECK([variadic function support], , [} 
#include <stdarg.h>
int foo(int bar, ...) {
    va_list     alist;
    va_start(alist, bar);
    bar = (int)va_arg(alist, int);
    va_end(alist);
    return bar;
], dnl
, dnl
[UC_ENSURE(CPPFLAGS, -DNO_STDARG)])])


# Check for strerror().
#
define(UL_STRERROR, [dnl
AC_REQUIRE([UC_PROG_FC])dnl
if test `$WHICH "${FC-}" | wc -w` != 1; then
AC_FUNC_CHECK(strerror, , [[UC_ENSURE(CPPFLAGS, -DNO_STRERROR)]])dnl
else
  echo checking for strerror
  echo '       CALL STRERROR' > conftest.f
  if eval "$FC conftest.f > /dev/null 2>&1"; then
    : true
  else
    UC_ENSURE(CPPFLAGS, -DNO_STRERROR)dnl
  fi
  rm -rf a.out conftest.f
fi
])


divert(diversion_number)dnl
