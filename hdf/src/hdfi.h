/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Copyright by The HDF Group.                                               *
 * Copyright by the Board of Trustees of the University of Illinois.         *
 * All rights reserved.                                                      *
 *                                                                           *
 * This file is part of HDF.  The full HDF copyright notice, including       *
 * terms governing use, modification, and redistribution, is contained in    *
 * the COPYING file, which can be found at the root of the source code       *
 * distribution tree, or in https://support.hdfgroup.org/ftp/HDF/releases/.  *
 * If you do not have access to either file, you may request a copy from     *
 * help@hdfgroup.org.                                                        *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


#ifndef HDFI_H
#define HDFI_H

#ifdef GOT_MACHINE
#undef GOT_MACHINE
#endif

/*--------------------------------------------------------------------------*/
/*                              MT/NT constants                             */
/*  Four MT nibbles represent double, float, int, uchar (from most          */
/*      significant to least significant).                                  */
/*  Each "column" in the "table" below is essentially independent of the    */
/*      other "columns", for example the CONVEXNATIVE entry means that the  */
/*      floating point formats are in Convex native format but the integers */
/*      are big-endian and standard sizes                                   */
/*  If you add another value to this "table", you need to add another       */
/*      DFNTF_xxx entry in hntdefs.h                                        */
/*  The values for each nibble are:                                         */
/*      1 - Big Endian                                                      */
/*          (i.e. Big-Endian, 32-bit architecture w/IEEE Floats)            */
/*      2 - VAX                                                             */
/*          (i.e. Middle-Endian, 32-bit architecture w/VAX Floats)          */
/*      3 - Cray                                                            */
/*          (i.e. Big-Endian, all 64-bit architecture w/Cray Floats)        */
/*      4 - Little Endian                                                   */
/*          (i.e. Little-Endian, 32-bit architecture w/IEEE Floats)         */
/*      5 - Convex                                                          */
/*          (i.e. Big-Endian, 32-bit architecture w/Convex Native Floats)   */
/*      6 - Fujitsu VP                                                      */
/*          (i.e. Big-Endian, 32-bit architecture w/Fujitsu Native Floats)  */
/*      7 - Cray MPP                                                        */
/*          (i.e. Big-Endian, 32-bit architecture w/IEEE Floats, but no 16-bit type)            */
/*      8 - Cray IEEE                                                       */
/*          (i.e. Big-Endian, all 64-bit architecture w/IEEE Floats)        */
/*--------------------------------------------------------------------------*/
#define     DFMT_SUN            0x1111
#define     DFMT_APOLLO         0x1111
#define     DFMT_IBM6000        0x1111
#define     DFMT_PC             0x4441
#define     DFMT_APPLE          0x1111
#define     DFMT_APPLE_INTEL    0x4441
#define     DFMT_LINUX64        0x4441
#define     DFMT_POWERPC64      0x1111

/* I/O library constants */
#define UNIXUNBUFIO 1
#define UNIXBUFIO   2
#define MACIO       3


/* Standard header files needed all the time */
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>

#include "H4api_adpt.h"


/*-------------------------------------------------------------------------
 * Define options for each platform
 *-------------------------------------------------------------------------*/

#if (defined(SUN) || defined(sun) || defined(__sun__) || defined(__SUNPRO_C)) & !defined(__i386)

#if !defined(SUN)
#define SUN
#endif

#ifdef GOT_MACHINE
If you get an error on this line more than one machine type has been defined.
Please check your Makefile.
#endif
#define GOT_MACHINE

#include <unistd.h>                 /* for some file I/O stuff */
#include <sys/time.h>
#include <sys/file.h>               /* for unbuffered i/o stuff */
#include <sys/stat.h>
#define DF_MT   DFMT_SUN
typedef void              VOID;
typedef void              *VOIDP;
typedef char              *_fcd;
typedef char              char8;
typedef unsigned char     uchar8;
typedef char              int8;
typedef unsigned char     uint8;
typedef short int         int16;
typedef unsigned short int uint16;
#ifdef _LP64 /* 64-bit environment */
typedef int               int32;
typedef unsigned int      uint32;
#else /* 32-bit environment */
typedef long int          int32;
typedef unsigned long int uint32;
#endif
typedef int               intn;
typedef unsigned int      uintn;
typedef int               intf;     /* size of INTEGERs in Fortran compiler */
typedef float             float32;
typedef double            float64;
#ifdef _LP64 /* 64-bit environment */
typedef long              hdf_pint_t;   /* an integer the same size as a pointer */
#else /* 32-bit environment */
typedef int               hdf_pint_t;   /* an integer the same size as a pointer */
#endif
#define FNAME_POST_UNDERSCORE
#define _fcdtocp(desc) (desc)
#define FILELIB UNIXBUFIO

/* JPEG #define's - Look in the JPEG docs before changing - (Q) */

/* Determine the memory manager we are going to use. Valid values are: */
/*  MEM_DOS, MEM_ANSI, MEM_NAME, MEM_NOBS.  See the JPEG docs for details on */
/*  what each does */
#define JMEMSYS         MEM_ANSI
#define HAVE_STDC
#define INCLUDES_ARE_ANSI

#endif /* SUN */

/*-----------------------------------------------------*/
#if defined(IBM6000) || defined(_AIX)

#ifndef IBM6000
#define IBM6000
#endif

#ifdef GOT_MACHINE
If you get an error on this line more than one machine type has been defined.
Please check your Makefile.
#endif
#define GOT_MACHINE

#   define BSD

#ifndef __GNUC__
#include <memory.h>
#endif /* __GNUC__ */
#include <sys/file.h>               /* for unbuffered i/o stuff */
#include <sys/stat.h>
#define DF_MT             DFMT_IBM6000
typedef void              VOID;
typedef void              *VOIDP;
typedef char              *_fcd;
#ifndef _ALL_SOURCE
typedef char              int8;
typedef short int         int16;
typedef int               int32;
#endif
typedef char              char8;
typedef unsigned char     uchar8;
typedef unsigned char     uint8;
typedef unsigned short int uint16;
typedef unsigned int      uint32;
typedef int               intn;
typedef unsigned int      uintn;
typedef float             float32;
typedef double            float64;
typedef int               intf;     /* size of INTEGERs in Fortran compiler */
#ifdef AIX5L64
typedef long              hdf_pint_t;   /* an integer the same size as a pointer */
#else /*AIX5L64 */
typedef int               hdf_pint_t;   /* an integer the same size as a pointer */
#endif /*AIX5L64 */
#define _fcdtocp(desc) (desc)
#define FILELIB UNIXBUFIO

/* JPEG #define's - Look in the JPEG docs before changing - (Q) */

/* Determine the memory manager we are going to use. Valid values are: */
/*  MEM_DOS, MEM_ANSI, MEM_NAME, MEM_NOBS.  See the JPEG docs for details on */
/*  what each does */
#define JMEMSYS         MEM_ANSI

#define HAVE_STDC
#define INCLUDES_ARE_ANSI

#endif /* IBM6000 */


/*-----------------------------------------------------*/
#if defined (__APPLE__)

#ifndef __APPLE__
#define __APPLE__
#endif
#ifdef __LITTLE_ENDIAN__
#define DF_MT DFMT_APPLE_INTEL
#else
#define DF_MT DFMT_APPLE
#endif
#ifdef GOT_MACHINE
If you get an error on this line more than one machine type has been defined.
Please check your Makefile.
#endif
#define GOT_MACHINE 1

#ifndef __GNUC__
#define DUMBCC     /* because it is.  for later use in macros */
#endif /* __GNUC__ */

#include <sys/types.h>
#include <sys/file.h>               /* for unbuffered i/o stuff */
#include <sys/stat.h>
#ifdef __i386
#ifndef INTEL86
#define INTEL86   /* we need this Intel define or bad things happen later */
#endif /* INTEL86 */
#endif /* __i386 */

typedef void            VOID;
typedef void            *VOIDP;
typedef char            *_fcd;
typedef char            char8;
typedef unsigned char   uchar8;
typedef char            int8;
typedef unsigned char   uint8;
typedef short           int16;
typedef unsigned short  uint16;
typedef int             int32;
typedef unsigned int    uint32;
typedef int             intn;
typedef unsigned int    uintn;
typedef float           float32;
typedef double          float64;
typedef int             intf;     /* size of INTEGERs in Fortran compiler */
typedef long            hdf_pint_t;   /* an integer the same size as a pointer */
#define _fcdtocp(desc) (desc)
#define FILELIB UNIXBUFIO

/* JPEG #define's - Look in the JPEG docs before changing - (Q) */

/* Determine the memory manager we are going to use. Valid values are: */
/*  MEM_DOS, MEM_ANSI, MEM_NAME, MEM_NOBS.  See the JPEG docs for details on */
/*  what each does */
#define JMEMSYS         MEM_ANSI

#endif /* __APPLE__ */


/*-----------------------------------------------------*/

/* Metrowerks Mac compiler defines some PC stuff so need to exclude this on the Mac */
#if !(defined (__APPLE__)) && !(defined(__CYGWIN__))

#if defined _M_ARM64 || defined _M_X64 || defined _M_IX86 || defined INTEL86 || defined M_I86 || defined M_I386 || defined DOS386 || defined __i386 || defined UNIX386 || defined i386
#ifndef INTEL86
#define INTEL86
#endif /* INTEL86 */

#if !defined UNIX386 && (defined unix || defined __unix)
#define UNIX386
#endif /* UNIX386 */

#if !defined DOS386 && defined M_I386
#define DOS386
#endif /* M_I386 && !DOS386 */

#if defined _WINDOWS || defined _WIN32
#define WIN386
#endif  /* _WINDOWS | _WIN32_ */

#if defined WIN386 || defined DOS386 || defined UNIX386
#define INTEL386
#endif /* WIN386 | DOS386 | UNIX386 */

#ifdef GOT_MACHINE
If you get an error on this line more than one machine type has been defined.
Please check your Makefile.
#endif
#define GOT_MACHINE 1

#if defined _WINDOWS || defined _WIN32
#pragma comment( lib, "oldnames" )
#endif

#include <fcntl.h>
#ifdef UNIX386
#include <sys/types.h>      /* for unbuffered file I/O */
#include <sys/stat.h>
#include <unistd.h>
#else /* !UNIX386 */
#ifdef H4_HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif
#ifdef H4_HAVE_SYS_STAT_H
#include <sys/stat.h>
#endif
#include <io.h>
#include <conio.h>          /* for debugging getch() calls */
#include <malloc.h>
#endif /* UNIX386 */
#include <ctype.h>          /* for character macros */
#ifdef __WATCOMC__
#include <stddef.h>         /* for the 'fortran' pragma */
#endif


#if defined WIN386
#ifndef GMEM_MOVEABLE       /* check if windows header is already included */
#include <windows.h>        /* include the windows headers */
#include <winnt.h>
#define HAVE_BOOLEAN
#endif /* GMEM_MOVEABLE */
#endif /* WIN386 */

#define DF_MT             DFMT_PC

#ifndef VOID    /* The stupid windows.h header file uses a #define instead of a typedef */
typedef void              VOID;
#endif  /* end VOID */
typedef void *            VOIDP;
typedef char *            _fcd;
typedef char              char8;
typedef unsigned char     uchar8;
typedef char              int8;
typedef unsigned char     uint8;
typedef short int         int16;
typedef unsigned short int uint16;
typedef long int          int32;
typedef unsigned long int uint32;
typedef int               intn;
typedef unsigned int      uintn;
typedef float             float32;
typedef double            float64;
typedef long              intf;     /* size of INTEGERs in Fortran compiler */
#ifdef _WIN64
typedef long long         hdf_pint_t;   /* 8-byte pointer */
#else
typedef int               hdf_pint_t;   /* 4-byte pointer */
#endif /* _WIN64 */

#if defined UNIX386
#ifdef H4_ABSOFT
#define FNAME(x) x
#define DF_CAPFNAMES
#else
#define FNAME_POST_UNDERSCORE
#endif
#elif defined INTEL386
#define DF_CAPFNAMES
#endif
#define _fcdtocp(desc) (desc)

#define FILELIB UNIXBUFIO

/* JPEG #define's - Look in the JPEG docs before changing - (Q) */

/* Determine the memory manager we are going to use. Valid values are: */
/*  MEM_DOS, MEM_ANSI, MEM_NAME, MEM_NOBS.  See the JPEG docs for details on */
/*  what each does */
#define JMEMSYS         MEM_ANSI
#define HAVE_STDC
#define INCLUDES_ARE_ANSI

#endif /* INTEL86 */
#endif /* !(defined(__APPLE__)) */

/*-----------------------------------------------------*/
/* Power PC 5 64 */
#if defined __powerpc64__

#ifdef GOT_MACHINE
If you get an error on this line more than one machine type has been defined.
Please check your Makefile.
#endif
#define GOT_MACHINE

#include <sys/file.h>               /* for unbuffered i/o stuff */
#include <sys/stat.h>
#define DF_MT             DFMT_POWERPC64
typedef void              VOID;
typedef void              *VOIDP;
typedef char              *_fcd;
typedef char              char8;
typedef unsigned char     uchar8;
typedef char              int8;
typedef unsigned char     uint8;
typedef short int         int16;
typedef unsigned short int uint16;
typedef int               int32;
typedef unsigned int      uint32;
typedef int               intn;
typedef unsigned int      uintn;
typedef int               intf;     /* size of INTEGERs in Fortran compiler */
typedef float             float32;
typedef double            float64;
typedef long              hdf_pint_t;   /* an integer the same size as a pointer */
#if defined __GNUC__
#define FNAME_POST_UNDERSCORE
#endif
#define _fcdtocp(desc) (desc)
#define FILELIB UNIXBUFIO

/* JPEG #define's - Look in the JPEG docs before changing - (Q) */

/* Determine the memory manager we are going to use. Valid values are: */
/*  MEM_DOS, MEM_ANSI, MEM_NAME, MEM_NOBS.  See the JPEG docs for details on */
/*  what each does */
#define JMEMSYS         MEM_ANSI

#ifdef __GNUC__
#define HAVE_STDC
#define INCLUDES_ARE_ANSI
#endif

#endif /*power PC 5 64 */

/*-----------------------------------------------------*/
/* Linux 64 */
#if (defined(__linux__) && defined __x86_64__  && !(defined  SUN)) || defined(__CYGWIN__)  /* i.e. 64-bit Linux  but not SunOS on Intel */
                                                                                           /* it should work also for Cygwin 32 & 64 bit */
#ifdef GOT_MACHINE
If you get an error on this line more than one machine type has been defined.
Please check your Makefile.
#endif
#define GOT_MACHINE

#include <sys/file.h>               /* for unbuffered i/o stuff */
#include <sys/stat.h>
#define DF_MT             DFMT_LINUX64
typedef void              VOID;
typedef void              *VOIDP;
typedef char              *_fcd;
typedef char              char8;
typedef unsigned char     uchar8;
typedef char              int8;
typedef unsigned char     uint8;
typedef short int         int16;
typedef unsigned short int uint16;
typedef int               int32;
typedef unsigned int      uint32;
typedef int               intn;
typedef unsigned int      uintn;
typedef int               intf;     /* size of INTEGERs in Fortran compiler */
typedef float             float32;
typedef double            float64;
typedef long              hdf_pint_t;   /* an integer the same size as a pointer */
#define FNAME_POST_UNDERSCORE
#define _fcdtocp(desc) (desc)
#define FILELIB UNIXBUFIO

/* JPEG #define's - Look in the JPEG docs before changing - (Q) */

/* Determine the memory manager we are going to use. Valid values are: */
/*  MEM_DOS, MEM_ANSI, MEM_NAME, MEM_NOBS.  See the JPEG docs for details on */
/*  what each does */
#define JMEMSYS         MEM_ANSI

#ifdef __GNUC__
#define HAVE_STDC
#define INCLUDES_ARE_ANSI
#endif

#endif /*Linux 64 */

/*-----------------------------------------------------*/
/* 64-bit Free BSD */

#if defined __FreeBSD__ && defined __x86_64__

#ifdef GOT_MACHINE
If you get an error on this line more than one machine type has been defined.
Please check your Makefile.
#endif
#define GOT_MACHINE

#include <sys/file.h>               /* for unbuffered i/o stuff */
#include <sys/stat.h>
#define DF_MT             DFMT_LINUX64
typedef void              VOID;
typedef void              *VOIDP;
typedef char              *_fcd;
typedef char              char8;
typedef unsigned char     uchar8;
typedef char              int8;
typedef unsigned char     uint8;
typedef short int         int16;
typedef unsigned short int uint16;
typedef int               int32;
typedef unsigned int      uint32;
typedef int               intn;
typedef unsigned int      uintn;
typedef int               intf;     /* size of INTEGERs in Fortran compiler */
typedef float             float32;
typedef double            float64;
typedef long              hdf_pint_t;   /* an integer the same size as a pointer */
#define FNAME_POST_UNDERSCORE
#define _fcdtocp(desc) (desc)
#define FILELIB UNIXBUFIO

/* JPEG #define's - Look in the JPEG docs before changing - (Q) */

/* Determine the memory manager we are going to use. Valid values are: */
/*  MEM_DOS, MEM_ANSI, MEM_NAME, MEM_NOBS.  See the JPEG docs for details on */
/*  what each does */
#define JMEMSYS         MEM_ANSI

#ifdef __GNUC__
#define HAVE_STDC
#define INCLUDES_ARE_ANSI
#endif

#endif /*64-bit FreeBSD */

#ifndef GOT_MACHINE
No machine type has been defined.  Your Makefile needs to have someing like
-DSUN or -DUNICOS in order for the HDF internal structures to be defined
correctly.
#endif

/*-----------------------------------------------------*/
/*              encode and decode macros               */
/*-----------------------------------------------------*/

#   define INT16ENCODE(p, i) \
{ *(p) = (uint8)(((uintn)(i) >> 8) & 0xff); (p)++; \
        *(p) = (uint8)((uintn)(i) & 0xff); (p)++; }

#   define UINT16ENCODE(p, i) \
{ *(p) = (uint8)(((uintn)(i) >> 8) & 0xff); (p)++; *(p) = (uint8)((i) & 0xff); (p)++; }

#   define INT32ENCODE(p, i) \
{ *(p) = (uint8)(((uint32)(i) >> 24) & 0xff); (p)++; \
        *(p) = (uint8)(((uint32)(i) >> 16) & 0xff); (p)++; \
        *(p) = (uint8)(((uint32)(i) >> 8) & 0xff); (p)++; \
        *(p) = (uint8)((uint32)(i) & 0xff); (p)++; }

#   define UINT32ENCODE(p, i) \
{ *(p) = (uint8)(((i) >> 24) & 0xff); (p)++; \
        *(p) = (uint8)(((i) >> 16) & 0xff); (p)++; \
        *(p) = (uint8)(((i) >> 8) & 0xff); (p)++; \
        *(p) = (uint8)((i) & 0xff); (p)++; }

#   define NBYTEENCODE(d, s, n) \
{   HDmemcpy(d,s,n); p+=n }

/* DECODE converts big endian bytes pointed by p to integer values and store
 * it in i.  For signed values, need to do sign-extension when converting
 * the 1st byte which carries the sign bit.
 * The macros does not require i be of a certain byte sizes.  It just requires
 * i be big enough to hold the intended value range.  E.g. INT16DECODE works
 * correctly even if i is actually a 64bit int like in a Cray.
 */

#   define INT16DECODE(p, i) \
{ (i) = ((*(p) & 0x80) ? ~0xffff : 0x00) | ((int16)(*(p) & 0xff) << 8); (p)++; \
        (i) |= (int16)((*(p) & 0xff)); (p)++; }

#   define UINT16DECODE(p, i) \
{ (i) = (uint16)((*(p) & 0xff) << 8); (p)++; \
        (i) |= (uint16)(*(p) & 0xff); (p)++; }

#   define INT32DECODE(p, i) \
{ (i) = (int32)(((int32)*(p) & 0x80) ? ~0xffffffff : 0x00) | ((int32)(*(p) & 0xff) << 24); (p)++; \
        (i) |= ((int32)(*(p) & 0xff) << 16); (p)++; \
        (i) |= ((int32)(*(p) & 0xff) << 8); (p)++; \
        (i) |= (*(p) & 0xff); (p)++; }

#   define UINT32DECODE(p, i) \
{ (i) = ((uint32)(*(p) & 0xff) << 24); (p)++; \
        (i) |= ((uint32)(*(p) & 0xff) << 16); (p)++; \
        (i) |= ((uint32)(*(p) & 0xff) << 8); (p)++; \
        (i) |= (uint32)(*(p) & 0xff); (p)++; }

/* Note! the NBYTEDECODE macro is backwards from the memcpy() routine, */
/*      in the spirit of the other DECODE macros */
#   define NBYTEDECODE(s, d, n) \
{   HDmemcpy(d,s,n); p+=n }

/*----------------------------------------------------------------
** MACRO FCALLKEYW for any special fortran-C stub keyword
**
** Microsoft C and Fortran need __fortran for Fortran callable C
**  routines.
**
** MACRO FRETVAL for any return value from a fortran-C stub function
**  Replaces the older FCALLKEYW macro.
**---------------------------------------------------------------*/
#ifdef FRETVAL
#undef FRETVAL
#endif

#ifndef FRETVAL /* !MAC */
#   define FCALLKEYW    /*NONE*/
#   define FRETVAL(x)   x
#endif


/*----------------------------------------------------------------
** MACRO FNAME for any fortran callable routine name.
**
**  This macro prepends, appends, or does not modify a name
**  passed as a macro parameter to it based on the FNAME_PRE_UNDERSCORE,
**  FNAME_POST_UNDERSCORE macros set for a specific system.
**
**---------------------------------------------------------------*/
#if defined(FNAME_PRE_UNDERSCORE) && defined(FNAME_POST_UNDERSCORE)
#   define FNAME(x)     _##x##_
#endif
#if defined(FNAME_PRE_UNDERSCORE) && !defined(FNAME_POST_UNDERSCORE)
#   define FNAME(x)     _##x
#endif
#if !defined(FNAME_PRE_UNDERSCORE) && defined(FNAME_POST_UNDERSCORE)
#   define FNAME(x)     x##_
#endif
#if !defined(FNAME_PRE_UNDERSCORE) && !defined(FNAME_POST_UNDERSCORE)
#   define FNAME(x)     x
#endif

/**************************************************************************
*  Generally useful macro definitions
**************************************************************************/
#ifndef MIN
#define MIN(a,b)    (((a)<(b)) ? (a) : (b))
#endif
#ifndef MAX
#define MAX(a,b)    (((a)>(b)) ? (a) : (b))
#endif

/**************************************************************************
*  Debugging Allocation functions
**************************************************************************/
#ifdef MALDEBUG
#include "maldebug.h"
#endif

/**************************************************************************
*  Macros to work around ANSI C portability problems.
**************************************************************************/
#ifdef DUMBCC
#define CONSTR(v,s) char *v=s
#else
#define CONSTR(v,s) static const char v[]=s
#endif

/**************************************************************************
*  Allocation functions defined differently
**************************************************************************/
#if !defined(MALLOC_CHECK)
#  define HDmalloc(s)      (malloc((size_t)s))
#  define HDcalloc(a,b)    (calloc((size_t)a,(size_t)b))
#  define HDfree(p)        (free((void*)p))
#  define HDrealloc(p,s)   (realloc((void*)p,(size_t)s))
#endif /* !defined MALLOC_CHECK */
/* Macro to free space and clear pointer to NULL */
#define HDfreenclear(p) { if((p)!=NULL) HDfree(p); p=NULL; }

/**************************************************************************
*  String functions defined differently
**************************************************************************/

#  define HDstrcat(s1,s2)   (strcat((s1),(s2)))
#  define HDstrcmp(s,t)     (strcmp((s),(t)))
#  define HDstrcpy(s,d)     (strcpy((s),(d)))
#  define HDstrlen(s)       (strlen((const char *)(s)))
#  define HDstrncmp(s1,s2,n)    (strncmp((s1),(s2),(n)))
#  define HDstrncpy(s1,s2,n)    (strncpy((s1),(s2),(n)))
#  define HDstrchr(s,c)         (strchr((s),(c)))
#  define HDstrrchr(s,c)        (strrchr((s),(c)))
#  define HDstrtol(s,e,b)       (strtol((s),(e),(b)))


/**************************************************************************
*  Memory functions defined differently
**************************************************************************/

# define HDmemcpy(dst,src,n)   (memcpy((void *)(dst),(const void *)(src),(size_t)(n)))
# define HDmemset(dst,c,n)     (memset((void *)(dst),(intn)(c),(size_t)(n)))
# define HDmemcmp(dst,src,n)   (memcmp((const void *)(dst),(const void *)(src),(size_t)(n)))


/**************************************************************************
*  Misc. functions
**************************************************************************/
#define HDstat(path, result)    (stat(path, result))
#define HDgetenv(s1)            (getenv(s1))
#define HDputenv(s1)            (putenv(s1))
#define HDltoa(v)               (ltoa(v))
#if defined (SUN) && defined(__GNUC__)
#define HDatexit(f)             (0) /* we punt on the SUN using gcc */
#else /* !SUN & GCC */
#define HDatexit(f)             (atexit(f))
#endif /* !SUN & GCC */

/* Compatibility #define for V3.3, should be taken out by v4.0 - QAK */
/* Commented out only, just in case any legacy code is still using it out there.
   Will be removed in a few maintenance releases.  -BMR, Jun 5, 2016
#define DFSDnumber DFSDndatasets */

#endif /* HDFI_H */

