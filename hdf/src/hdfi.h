/*
$Header$

$Log$
Revision 1.3  1992/09/11 14:15:04  koziol
Changed Fortran stubs' parameter passing to use a new typedef, intf,
which should be typed to the size of an INTEGER*4 in whatever Fortran
compiler the C functions need to be compatible with.  (This is mostly
for the PC and possibly for the Mac)

 * Revision 1.2  1992/09/04  22:05:02  georgev
 * Fixed "const" problem for MIPSEL.
 *
 * Revision 1.1  1992/08/25  21:40:44  koziol
 * Initial revision
 *
*/
#ifndef HDFI_H
#define HDFI_H

#ifdef OLD_MACHINE_TEST
/* &&& Replace this line with #define MachineType &&& */

/**IMPORTANT** this is now in the in the makefile */
#if !defined(SUN) && !defined(SUN386) && !defined(VMS) && !defined(ALLIANT)
#if !defined(IRIS4) && !defined(MAC) && !defined(UNICOS) && !defined(MIPSEL)
#if !defined(PC) && !defined(WIN3) && !defined(CONVEX) && !defined(IBM6000)
#if !defined(APOLLO)
If there is an error on this line, the machine type is defined wrong or
not defined.  Somewhere you need to define SUN or some other machine
among those tested for above.  This definition can occuer in the compile
line, or in the line above that says /* &&& Replace this line... */,
or possibly elsewhere if it suits your needs.
#endif
#endif
#endif
#endif
#endif /* OLD_MACHINE_TEST */

#ifdef GOT_MACHINE
#undef GOT_MACHINE
#endif

/*--------------------------------------------------------------------------*/
/*                              MT/NT constants                             */
/*  four MT nibbles represent double, float, int, uchar (from most          */
/*  significant to least significant).                                      */
/*  The values for each nibble are:                                         */
/*      1 - Big Endian                                                      */
/*      2 - VAX                                                             */
/*      3 - Cray                                                            */
/*      4 - Little Endian                                                   */
/*--------------------------------------------------------------------------*/
#define        DFMT_SUN        0x1111
#define        DFMT_ALLIANT    0x1111
#define        DFMT_IRIS4      0x1111
#define        DFMT_APOLLO     0x1111
#define        DFMT_IBM6000    0x1111
#define        DFMT_HP9000     0x1111
#define        DFMT_CONVEX     0x1111
#define        DFMT_UNICOS     0x3331
#define        DFMT_CTSS       0x3331
#define        DFMT_VAX        0x2221
#define        DFMT_MIPSEL     0x4441
#ifdef OLD_WAY
#define        DFMT_PC         0x4144  /* note byte swapping ??? */
                               /* check this... */
#else
#define        DFMT_PC         0x4441  /* note byte swapping ??? */
                               /* check this... */
#endif
#define        DFMT_MAC        0x1111
#define        DFMT_SUN386     0x4441

/* I/O library constants */
#define UNIXUNBUFIO 1
#define UNIXBUFIO   2
#define MACIO       3
#define PCIO        4

#include <stdio.h>

#ifdef __STDC__
#define PROTOTYPE
#include <stdlib.h>
#endif

#ifdef SUN

#ifdef GOT_MACHINE
If you get an error on this line more than one machine type has been defined.
Please check your Makefile.
#endif
#define GOT_MACHINE

#   define BSD
#include <string.h>
#ifndef __GNUC__
#include <memory.h>
#endif /* __GNUC__ */
#include <unistd.h>
#include <sys/file.h>               /* for unbuffered i/o stuff */
#define DF_MT             DFMT_SUN
typedef void              VOID;
typedef char              *VOIDP;
typedef char              *_fcd;
typedef int               bool;
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
typedef int               intf;     /* size of INTEGERs in Fortran compiler */
typedef float             float32;
typedef double            float64;
#define DFmovmem(from, to, len) memcpy(to, from, len)
#define FNAME_POST_UNDERSCORE
#define _fcdtocp(desc) (desc)
#define FILELIB UNIXBUFIO
#ifndef __STDC__
#define const
#endif
#endif /* SUN */


#ifdef IBM6000

#ifdef GOT_MACHINE
If you get an error on this line more than one machine type has been defined.
Please check your Makefile.
#endif
#define GOT_MACHINE

#   define BSD
#include <string.h>
#ifndef __GNUC__
#include <memory.h>
#endif /* __GNUC__ */
#include <sys/file.h>               /* for unbuffered i/o stuff */
#define DF_MT             DFMT_IBM6000
typedef void              VOID;
typedef void              *VOIDP;
typedef char              *_fcd;
typedef int               bool;
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
typedef int               intf;     /* size of INTEGERs in Fortran compiler */
#define DFmovmem(from, to, len) memcpy(to, from, len)
#define _fcdtocp(desc) (desc)
#define FILELIB UNIXBUFIO
#endif /* IBM6000 */

#ifdef HP9000

#ifdef GOT_MACHINE
If you get an error on this line more than one machine type has been defined.
Please check your Makefile.
#endif
#define GOT_MACHINE

#   define BSD
#include <string.h>
#ifndef __GNUC__
#include <memory.h>
#endif /* __GNUC__ */
#include <sys/file.h>               /* for unbuffered i/o stuff */
#define DF_MT             DFMT_HP9000
typedef void              VOID;
typedef void              *VOIDP;
typedef char              *_fcd;
typedef int               bool;
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
typedef int               intf;     /* size of INTEGERs in Fortran compiler */
#define DFmovmem(from, to, len) memcpy(to, from, len)
#define _fcdtocp(desc) (desc)
#define FILELIB UNIXBUFIO
#endif /* HP9000 */


#ifdef IRIS4

#ifdef GOT_MACHINE
If you get an error on this line more than one machine type has been defined.
Please check your Makefile.
#endif
#define GOT_MACHINE 1

#   define BSD
#include <string.h>
#ifndef __GNUC__
#include <memory.h>
#endif /* __GNUC__ */
#include <sys/file.h>               /* for unbuffered i/o stuff */
#define DF_MT              DFMT_IRIS4
typedef void               VOID;
typedef char               *VOIDP;
typedef char               *_fcd;
typedef int                bool;
typedef char               char8;
typedef unsigned char      uchar8;
typedef char               int8;
typedef unsigned char      uint8;
typedef short int          int16;
typedef unsigned short int uint16;
typedef long int           int32;
typedef unsigned long int  uint32;
typedef int                intn;
typedef unsigned int       uintn;
typedef float              float32;
typedef double             float64;
typedef int                intf;     /* size of INTEGERs in Fortran compiler */
#define DFmovmem(from, to, len) memcpy(to, from, len)
#define FNAME_POST_UNDERSCORE
#define _fcdtocp(desc) (desc)
#define FILELIB UNIXBUFIO
#endif /* IRIS4 */


#ifdef UNICOS

#ifdef GOT_MACHINE
If you get an error on this line more than one machine type has been defined.
Please check your Makefile.
#endif
#define GOT_MACHINE 1

#include <string.h>
#include <memory.h>
#include <fortran.h>
#ifndef O_RDONLY
#include <fcntl.h>              /* for unbuffered i/o stuff */
#define L_INCR  1
#endif /*O_RDONLY*/

#define DF_MT   DFMT_UNICOS
typedef void            VOID;
typedef char            *VOIDP;
typedef char            *_fcd;
typedef int             bool;
typedef char            char8;
typedef unsigned char   uchar8;
typedef char            int8;
typedef unsigned char   uint8;
typedef int             int16;
typedef unsigned int    uint16;
typedef int             int32;
typedef unsigned int    uint32;
typedef int             intn;
typedef unsigned int    uintn;
typedef float           float32;
typedef double          float64;
typedef int             intf;     /* size of INTEGERs in Fortran compiler */

#define DFmovmem(from, to, len) memcpy(to, from, len)
#define DF_CAPFNAMES            /* fortran names are in all caps */
#define FILELIB UNIXBUFIO

#endif /* UNICOS */

#ifdef VMS

#ifdef GOT_MACHINE
If you get an error on this line more than one machine type has been defined.
Please check your Makefile.
#endif
#define GOT_MACHINE 1
#include <file.h>               /* for unbuffered i/o stuff */

#define DF_MT              DFMT_VAX
typedef int                VOID;
typedef char               *VOIDP;
typedef char               *_fcd;
typedef int                bool;
typedef char               char8;
typedef unsigned char      uchar8;
typedef char               int8;
typedef unsigned char      uint8;
typedef short int          int16;
typedef unsigned short int uint16;
typedef long int           int32;
typedef unsigned long int  uint32;
typedef int                intn;
typedef unsigned int       uintn;
typedef float              float32;
typedef double             float64;
typedef int                intf;     /* size of INTEGERs in Fortran compiler */
#define DFmovmem(from, to, len) memcpy(to, from, len)
#define _fcdtocp(desc)  ((char *) *((char **) &desc[4]))

/* 
  Redef a couple of C routine names to avoid conflicts
  since the VMS link command is case-insensitive
*/
#define FILELIB UNIXBUFIO
#include "dfivms.h"

#endif /* VMS */

#ifdef CONVEX

#ifdef GOT_MACHINE
If you get an error on this line more than one machine type has been defined.
Please check your Makefile.
#endif
#define GOT_MACHINE

#include <sys/file.h>               /* for unbuffered i/o stuff */
#include <stdlib.h>
#define DF_MT             DFMT_CONVEX
typedef void              VOID;
typedef char              *VOIDP;
typedef char              *_fcd;
typedef int               bool;
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
typedef int               intf;     /* size of INTEGERs in Fortran compiler */
#define DFmovmem(from, to, len) bcopy(from, to, len)
#define FNAME_POST_UNDERSCORE
#define _fcdtocp(desc) (desc)
#define FILELIB UNIXBUFIO
#endif /* CONVEX */


#ifdef MIPSEL

#ifdef GOT_MACHINE
If you get an error on this line more than one machine type has been defined.
Please check your Makefile.
#endif
#define GOT_MACHINE 1

#include <sys/types.h>
#include <sys/file.h>               /* for unbuffered i/o stuff */
#define DF_MT   DFMT_MIPSEL
typedef void            VOID;
typedef char            *VOIDP;
typedef char            *_fcd;
typedef int             bool;
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
#define DFmovmem(from, to, len) memcpy(to, from, len)
#define _fcdtocp(desc) (desc)
#define FNAME_POST_UNDERSCORE
#define FILELIB UNIXBUFIO
#ifndef __STDC__
#define const
#endif /* __STDC__ */
#endif /* MIPSEL */

#ifdef MAC

#ifdef GOT_MACHINE
If you get an error on this line more than one machine type has been defined.
Please check your Makefile.
#endif
#define GOT_MACHINE 1
#define PROTOTYPE

#include <memory.h>             /* malloc stuff for MPW 3.0 */
#include <fcntl.h>              /* unbuffered IO stuff for MPW 3.0 */
#include <String.h>
#ifdef THINK_C                  /* for LightSpeed C */
#include <unix.h>
#else /*THINK_C                   MPW, possibly others */
#include <Files.h>              /* for unbuffered i/o stuff */
#endif /*THINK_C*/
#define	DF_CAPFNAMES            /* fortran names are in all caps */
#define DF_DYNAMIC				/* use dynamic allocation */
#define DFmovmem(from, to, len) memcpy(to, from, len)
#define malloc(x)   NewPtr((Size)   (x))    /* don't use malloc on the Mac */
#define free(x)     DisposPtr((Ptr) (x))    /* don't use free on the Nac   */ 
#define DF_MT   DFMT_MAC

typedef void              VOID;
typedef char              *VOIDP;
typedef char              *_fcd;
typedef int               bool;
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
typedef int               intf;     /* size of INTEGERs in Fortran compiler */
#define _fcdtocp(desc) (desc)
void exit(int status);

#define register   /* don't mess with the MAC compiler's register allocation */
#define FILELIB MACIO
#endif /*MAC*/


#ifdef WIN3
#define PC
#endif  /* WIN3 */

#ifdef PC

#ifdef GOT_MACHINE
If you get an error on this line more than one machine type has been defined.
Please check your Makefile.
#endif
#define GOT_MACHINE 1

#include <fcntl.h>
#include <sys\types.h>      /* for unbuffered file I/O */
#include <sys\stat.h>
#include <io.h>
#include <malloc.h>
#include <conio.h>          /* for debugging getch() calls */
#include <string.h>         /* for vaious string functions */
#include <limits.h>         /* for UINT_MAX used in various places */
#ifdef WIN3
#ifndef GMEM_MOVEABLE       /* check if windows header is already included */
#include <windows.h>        /* include the windows headers */
#endif
#endif /* WIN3 */

#define DF_MT             DFMT_PC

#ifndef PROTOTYPE
/* Want prototypes for the PC even if we're not completely ANSI C compliant */
#define PROTOTYPE
#endif  /* !PROTOTYPE */

typedef void              VOID;
typedef void              *VOIDP;
typedef char              *_fcd;
typedef int               bool;
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

#ifdef OLD_WAY
#ifdef WIN3
#define DFmovmem(from, to, len) _fmemmove(to, from, len)
#else   /* !WIN3 */
#define DFmovmem(from, to, len) memcpy(to, from, len)
#endif  /* WIN3 */
#else
#define DFmovmem(from, to, len) HDmemcopy(from, to, len)
#endif

#define _fcdtocp(desc) (desc)

#define register    /* don't mess with the PC compiler's register allocation */
#define FILELIB PCIO
#endif /* PC */


#ifndef GOT_MACHINE
No machine type has been defined.  Your Makefile needs to have someing like
-DSUN or -DUNICOS in order for the HDF internal structures to be defined
correctly.
#endif

/*-----------------------------------------------------*/
/*              encode and decode macros               */
/*-----------------------------------------------------*/

#   define INT16ENCODE(p, i) \
{ *(p) = (uint8)(((uint16)(i) >> 8) & 0xff); (p)++; \
        *(p) = (uint8)((uint16)(i) & 0xff); (p)++; }

#   define UINT16ENCODE(p, i) \
{ *(p) = (uint8)(((uint16)(i) >> 8) & 0xff); (p)++; *(p) = (uint8)((i) & 0xff); (p)++; }

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

#   define INT16DECODE(p, i) \
{ (i) = (*(p) & 0xff) << 8; (p)++; (i) |= (*(p) & 0xff); (p)++; }

#   define UINT16DECODE(p, i) \
{ (i) = ((*(p) & 0xff) << 8); (p)++; (i) |= (*(p) & 0xff); (p)++; }

#   define INT32DECODE(p, i) \
{ (i) = ((int32)(*(p) & 0xff) << 24); (p)++; \
        (i) |= ((int32)(*(p) & 0xff) << 16); (p)++; \
        (i) |= ((int32)(*(p) & 0xff) << 8); (p)++; \
        (i) |= (*(p) & 0xff); (p)++; }

#   define UINT32DECODE(p, i) \
{ (i) = ((uint32)(*(p) & 0xff) << 24); (p)++; \
        (i) |= ((uint32)(*(p) & 0xff) << 16); (p)++; \
        (i) |= ((uint32)(*(p) & 0xff) << 8); (p)++; \
        (i) |= (*(p) & 0xff); (p)++; }

/**************************************************************************
*                   Conversion Routine Pointers
***************************************************************************/
#    ifndef DFKMASTER
#if defined __STDC__ || defined PC
extern int (*DFKnumin)(void * source, void * dest, uint32 num_elm,
            uint32 source_stride,uint32 dest_stride);
extern int (*DFKnumout)(void * source, void * dest, uint32 num_elm,
            uint32 source_stride,uint32 dest_stride);
#else
extern int (*DFKnumin)();
extern int (*DFKnumout)();
#endif
#     endif /* DFKMASTER */

/*****************************************************************
*  Create DFtbuf: a temporary buffer used when a small amount of
*  memory is needed, as when small conversions are done
******************************************************************/
#define DF_TBUFSZ       512     /* buffer size can be smaller */
#ifdef  HMASTER
    int    int_DFtbuf[DF_TBUFSZ]; /* int declaration to force word boundary */
    uint8  *DFtbuf = (uint8 *) int_DFtbuf;
#else /* !HMASTER */
extern uint8 *DFtbuf;
#endif /*HMASTER*/

/*----------------------------------------------------------------
** MACRO FCALLKEYW for any special fortran-C stub keyword
**
** MacIntosh MPW LS-fortran needs pascal since it can interface
**  best with pascal functions.
** Microsoft C and Fortran need _fortran for Fortran callable C
**  routines.
**
** MACRO FRETVAL for any return value from a fortran-C stub function
**  Replaces the older FCALLKEYW macro.
**---------------------------------------------------------------*/
#ifdef FRETVAL
#undef FRETVAL
#endif

#if defined(MAC)                /* with LS FORTRAN */
#   define FCALLKEYW    pascal
#   define FRETVAL(x)   pascal x
#endif

#if defined(PC)   /* with MS Fortran */
#   define FCALLKEYW    _fortran
#   define FRETVAL(x)   x _fortran
#endif

#ifndef FRETVAL /* !MAC && !PC */
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
#ifdef __STDC__
#   define FNAME(x)     _##x##_
#else
#   define FNAME(x)     _/**/x/**/_
#endif
#endif
#if defined(FNAME_PRE_UNDERSCORE) && !defined(FNAME_POST_UNDERSCORE)
#ifdef __STDC__
#   define FNAME(x)     _##x
#else
#   define FNAME(x)     _/**/x
#endif
#endif
#if !defined(FNAME_PRE_UNDERSCORE) && defined(FNAME_POST_UNDERSCORE)
#ifdef __STDC__
#   define FNAME(x)     x##_
#else
#   define FNAME(x)     x/**/_
#endif
#endif
#if !defined(FNAME_PRE_UNDERSCORE) && !defined(FNAME_POST_UNDERSCORE)
#   define FNAME(x)     x
#endif

/**************************************************************************
*  String functions defined differently under MS Windows
**************************************************************************/

#ifdef WIN3
#  define DFIstrlen(s)   (_fstrlen(s))
#  define DFIstrcpy(s,d) (_fstrcpy((s),(d)))
#  define DFIstrcmp(s,t) (_fstrcmp((s),(t)))
#  define DFIstrncmp(s1,s2,n) (_fstrncmp((s1),(s2),(n)))
#else
#  define DFIstrlen(s)   (strlen(s))
#  define DFIstrcpy(s,d) (strcpy((s),(d)))
#  define DFIstrcmp(s,t) (strcmp((s),(t)))
#  define DFIstrncmp(s1,s2,n) (strncmp((s1),(s2),(n)))
#endif /* WIN3 */


#endif /* HDFI_H */
