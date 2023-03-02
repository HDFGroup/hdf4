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

#ifndef H4_HDFI_H
#define H4_HDFI_H

#include "H4api_adpt.h"

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
/*          (i.e. Big-Endian, 32-bit architecture w/IEEE Floats, but        */
/*           no 16-bit type)                                                */
/*      8 - Cray IEEE                                                       */
/*          (i.e. Big-Endian, all 64-bit architecture w/IEEE Floats)        */
/*--------------------------------------------------------------------------*/

/* There are only two configurations now, both of which have IEEE-754
 * floating-point support. Type sizes are now specified by C99 fixed-width
 * integers, so 32- vs 64-bit support doesn't matter in this file.
 */
#define DFMT_LE 0x4441 /* Little-endian, IEEE-754 architectures */
#define DFMT_BE 0x1111 /* Big-endian, IEEE-754 architectures */

#ifdef H4_WORDS_BIGENDIAN
#define DF_MT DFMT_BE
#else
#define DF_MT DFMT_LE
#endif

/* I/O library constants */
#define UNIXUNBUFIO 1
#define UNIXBUFIO   2
#define MACIO       3

/* The library always uses UNIXBUFIO */
#define FILELIB UNIXBUFIO

/* Standard C library headers */
#include <assert.h>
#include <ctype.h>
#include <inttypes.h>
#include <limits.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* POSIX headers
 *
 * Checked for individually, since POSIX compliance is spotty and even
 * non-POSIX platforms like Windows have some of these headers.
 */
#ifdef H4_HAVE_FCNTL_H
#include <fcntl.h>
#endif
#ifdef H4_HAVE_UNISTD_H
#include <unistd.h>
#endif
#ifdef H4_HAVE_SYS_FILE_H
#include <sys/file.h>
#endif
#ifdef H4_HAVE_SYS_STAT_H
#include <sys/stat.h>
#endif
#ifdef H4_HAVE_SYS_TIME_H
#include <sys/time.h>
#endif
#ifdef H4_HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif

/* Windows headers */
#ifdef H4_HAVE_WIN32_API
#include <windows.h>
#include <io.h>
#include <process.h>
#endif

/*-------------------------------------------------------------------------
 * Pre-C99 platform-independent type scheme
 *
 * These types were added long before C99 was widely supported (or even
 * existed). They were formerly mapped to native C types on a machine-specific
 * basis, but they are now mapped to their equivalent C99 types.
 *-------------------------------------------------------------------------*/

/* Floating-point types */
typedef float  float32;
typedef double float64;

/* Characters */
typedef char          char8;
typedef unsigned char uchar8;
typedef char         *_fcd;
#define _fcdtocp(desc) (desc)

/* Fixed-width integer types */
typedef int8_t   int8;
typedef uint8_t  uint8;
typedef int16_t  int16;
typedef uint16_t uint16;
typedef int32_t  int32;
typedef uint32_t uint32;

/* Native integer types */
typedef int          intn;
typedef unsigned int uintn;

/* These are no longer used in the library, but other software uses them */
#ifndef VOID
/* winnt.h defines VOID to `void` via a macro */
typedef void VOID;
#endif
typedef void *VOIDP;

/*-------------------------------------------------------------------------
 * Is this an LP64 system?
 *-------------------------------------------------------------------------*/
#if LONG_MAX == INT64_MAX
#define H4_HAVE_LP64
#endif

/*-------------------------------------------------------------------------
 * Fortran definitions
 *-------------------------------------------------------------------------*/

/* size of INTEGERs in Fortran compiler */
typedef int intf;

/* Integer that is the same size as a pointer */
typedef intptr_t hdf_pint_t;

#define FNAME_POST_UNDERSCORE

/*-----------------------------------------------------*/
/*              encode and decode macros               */
/*-----------------------------------------------------*/

#define INT16ENCODE(p, i)                                                                                    \
    {                                                                                                        \
        *(p) = (uint8)(((uintn)(i) >> 8) & 0xff);                                                            \
        (p)++;                                                                                               \
        *(p) = (uint8)((uintn)(i)&0xff);                                                                     \
        (p)++;                                                                                               \
    }

#define UINT16ENCODE(p, i)                                                                                   \
    {                                                                                                        \
        *(p) = (uint8)(((uintn)(i) >> 8) & 0xff);                                                            \
        (p)++;                                                                                               \
        *(p) = (uint8)((i)&0xff);                                                                            \
        (p)++;                                                                                               \
    }

#define INT32ENCODE(p, i)                                                                                    \
    {                                                                                                        \
        *(p) = (uint8)(((uint32)(i) >> 24) & 0xff);                                                          \
        (p)++;                                                                                               \
        *(p) = (uint8)(((uint32)(i) >> 16) & 0xff);                                                          \
        (p)++;                                                                                               \
        *(p) = (uint8)(((uint32)(i) >> 8) & 0xff);                                                           \
        (p)++;                                                                                               \
        *(p) = (uint8)((uint32)(i)&0xff);                                                                    \
        (p)++;                                                                                               \
    }

#define UINT32ENCODE(p, i)                                                                                   \
    {                                                                                                        \
        *(p) = (uint8)(((i) >> 24) & 0xff);                                                                  \
        (p)++;                                                                                               \
        *(p) = (uint8)(((i) >> 16) & 0xff);                                                                  \
        (p)++;                                                                                               \
        *(p) = (uint8)(((i) >> 8) & 0xff);                                                                   \
        (p)++;                                                                                               \
        *(p) = (uint8)((i)&0xff);                                                                            \
        (p)++;                                                                                               \
    }

#define NBYTEENCODE(d, s, n)                                                                                 \
    {                                                                                                        \
        memcpy(d, s, n);                                                                                     \
        p += n                                                                                               \
    }

/* DECODE converts big endian bytes pointed by p to integer values and store
 * it in i.  For signed values, need to do sign-extension when converting
 * the 1st byte which carries the sign bit.
 * The macros does not require i be of a certain byte sizes.  It just requires
 * i be big enough to hold the intended value range.  E.g. INT16DECODE works
 * correctly even if i is actually a 64bit int like in a Cray.
 */

#define INT16DECODE(p, i)                                                                                    \
    {                                                                                                        \
        (i) = ((*(p)&0x80) ? ~0xffff : 0x00) | ((int16)(*(p)&0xff) << 8);                                    \
        (p)++;                                                                                               \
        (i) |= (int16)((*(p)&0xff));                                                                         \
        (p)++;                                                                                               \
    }

#define UINT16DECODE(p, i)                                                                                   \
    {                                                                                                        \
        (i) = (uint16)((*(p)&0xff) << 8);                                                                    \
        (p)++;                                                                                               \
        (i) |= (uint16)(*(p)&0xff);                                                                          \
        (p)++;                                                                                               \
    }

#define INT32DECODE(p, i)                                                                                    \
    {                                                                                                        \
        (i) = (int32)(((int32) * (p)&0x80) ? ~0xffffffff : 0x00) | ((int32)(*(p)&0xff) << 24);               \
        (p)++;                                                                                               \
        (i) |= ((int32)(*(p)&0xff) << 16);                                                                   \
        (p)++;                                                                                               \
        (i) |= ((int32)(*(p)&0xff) << 8);                                                                    \
        (p)++;                                                                                               \
        (i) |= (*(p)&0xff);                                                                                  \
        (p)++;                                                                                               \
    }

#define UINT32DECODE(p, i)                                                                                   \
    {                                                                                                        \
        (i) = ((uint32)(*(p)&0xff) << 24);                                                                   \
        (p)++;                                                                                               \
        (i) |= ((uint32)(*(p)&0xff) << 16);                                                                  \
        (p)++;                                                                                               \
        (i) |= ((uint32)(*(p)&0xff) << 8);                                                                   \
        (p)++;                                                                                               \
        (i) |= (uint32)(*(p)&0xff);                                                                          \
        (p)++;                                                                                               \
    }

/* Note! the NBYTEDECODE macro is backwards from the memcpy() routine, */
/*      in the spirit of the other DECODE macros */
#define NBYTEDECODE(s, d, n)                                                                                 \
    {                                                                                                        \
        memcpy(d, s, n);                                                                                     \
        p += n                                                                                               \
    }

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

#ifndef FRETVAL    /* !MAC */
#define FCALLKEYW  /*NONE*/
#define FRETVAL(x) x
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
#define FNAME(x) _##x##_
#endif
#if defined(FNAME_PRE_UNDERSCORE) && !defined(FNAME_POST_UNDERSCORE)
#define FNAME(x) _##x
#endif
#if !defined(FNAME_PRE_UNDERSCORE) && defined(FNAME_POST_UNDERSCORE)
#define FNAME(x) x##_
#endif
#if !defined(FNAME_PRE_UNDERSCORE) && !defined(FNAME_POST_UNDERSCORE)
#define FNAME(x) x
#endif

/**************************************************************************
 *  Generally useful macro definitions
 **************************************************************************/
#ifndef MIN
#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#endif
#ifndef MAX
#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#endif

/**************************************************************************
 *  Memory functions
 **************************************************************************/

/* DO NOT USE THESE MACROS */

/* These will be removed from a future version of the library and are
 * only kept here to avoid breakage in programs that unwisely used
 * them.
 */
#define HDmalloc(s)     malloc(s)
#define HDcalloc(a, b)  calloc(a, b)
#define HDfree(p)       free(p)
#define HDrealloc(p, s) realloc(p, s)

/* Macro to free space and clear pointer to NULL */
#define HDfreenclear(p)                                                                                      \
    {                                                                                                        \
        free(p);                                                                                             \
        (p) = NULL;                                                                                          \
    }

/**************************************************************************
 *  String functions defined differently
 **************************************************************************/

#define HDstrcat(s1, s2)     (strcat((s1), (s2)))
#define HDstrcmp(s, t)       (strcmp((s), (t)))
#define HDstrcpy(s, d)       (strcpy((s), (d)))
#define HDstrlen(s)          (strlen((const char *)(s)))
#define HDstrncmp(s1, s2, n) (strncmp((s1), (s2), (n)))
#define HDstrncpy(s1, s2, n) (strncpy((s1), (s2), (n)))
#define HDstrchr(s, c)       (strchr((s), (c)))
#define HDstrrchr(s, c)      (strrchr((s), (c)))
#define HDstrtol(s, e, b)    (strtol((s), (e), (b)))

/**************************************************************************
 *  JPEG #define's - Look in the JPEG docs before changing - (Q)
 **************************************************************************/

/* Determine the memory manager we are going to use. Valid values are:
 *  - MEM_DOS
 *  - MEM_ANSI
 *  - MEM_NAME
 *  - MEM_NOBS
 *
 *  See the JPEG docs for details on what each does.
 */
#define JMEMSYS MEM_ANSI

#endif /* H4_HDFI_H */
