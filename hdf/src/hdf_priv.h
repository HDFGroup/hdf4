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

#ifndef H4_HDF_PRIV_H
#define H4_HDF_PRIV_H

/* Define the I/O scheme before hdf.h to avoid an ordering mess in the
 * vconv.c code
 */

/* I/O library constants */
#define UNIXUNBUFIO 1
#define UNIXBUFIO   2

/* The library always uses UNIXBUFIO */
#define FILELIB UNIXBUFIO

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

/* Common library headers */
#include "hdf.h"
#include "herr_priv.h"

/* Standard C library headers */
#include <assert.h>
#include <ctype.h>
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
#ifdef H4_HAVE_SYS_TIME_H
#include <sys/time.h>
#endif

/* MSDN docs say that sys/types.h has to come before sys/stat */
#ifdef H4_HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif
#ifdef H4_HAVE_SYS_STAT_H
#include <sys/stat.h>
#endif

/* Windows headers */
#ifdef H4_HAVE_WIN32_API
/* Needed for XDR. Must come BEFORE windows.h!!! */
#include <winsock2.h>

#include <windows.h>
#include <direct.h>
#include <io.h>
#include <process.h>
#endif

/*-------------------------------------------------------------------------
 * Is this an LP64 system?
 *-------------------------------------------------------------------------*/
#if LONG_MAX == INT64_MAX
#define H4_HAVE_LP64
#endif

/*-------------------------------------------------------------------------
 * Fortran definitions
 *-------------------------------------------------------------------------*/

/* Integer that is the same size as a pointer */
typedef intptr_t hdf_pint_t;

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

/* DECODE converts big endian bytes pointed by p to integer values and stores
 * it in i.  For signed values, we need to do sign-extension when converting
 * the 1st byte which carries the sign bit.
 * The macros do not require i be of a certain byte size.  They just require
 * i to be big enough to hold the intended value range.  e.g., INT16DECODE works
 * correctly even if i is actually a 64bit int like in a Cray.
 */

#define INT16DECODE(p, i)                                                                                    \
    {                                                                                                        \
        (i) = (int16)((*(p)&0x80) ? ~0xffff : 0x00) | ((int16)(*(p)&0xff) << 8);                             \
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
        (i) = ((int32)(((*(p)&0x80) ? ~0xffffffffULL : 0x0ULL)) | ((*(p) & (unsigned)0xff) << 24));          \
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

#endif /* H4_HDF_PRIV_H */
