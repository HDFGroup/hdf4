/****************************************************************************
 * NCSA HDF                                                                 *
 * Software Development Group                                               *
 * National Center for Supercomputing Applications                          *
 * University of Illinois at Urbana-Champaign                               *
 * 605 E. Springfield, Champaign IL 61820                                   *
 *                                                                          *
 * For conditions of distribution and use, see the accompanying             *
 * hdf/COPYING file.                                                        *
 *                                                                          *
 ****************************************************************************/

/* $Id$ */

/*+ hlimits.h
   *** This file contains all hard coded limits for the library
   + */

#ifndef _HLIMITS_H
#define _HLIMITS_H

/* ------------------------- General Constants hdf.h  --------------------- */
/* tbuf used as a temporary buffer for small jobs.  The size is
   preferably > 512 but MUST be > ~256.  It is advised that if an
   arbitrarily large buffer (> 100 bytes) is require, dynamic space be
   used.  tbuf lives in the hfile.c */

#ifndef TBUF_SZ
#if defined(macintosh) | defined(THINK_C)
#   define TBUF_SZ      256
#else  /* !macintosh */
#   define TBUF_SZ     1024
#endif /* !macintosh */
#endif

/*****************************************************************
*  Create DFtbuf: a temporary buffer used when a small amount of
*  memory is needed, as when small conversions are done
******************************************************************/
#define DF_TBUFSZ       512     /* buffer size can be smaller */

/*  File name max length (old annotations)  */
#define DF_MAXFNLEN     256

/*
   * some max lengths for the Vset interface
   *
   * Except for FIELDNAMELENMAX, change these as you please, they
   * affect memory only, not the file.
   *
 */

#define FIELDNAMELENMAX    128  /* fieldname   : 16 chars max */
#if defined macintosh | defined THINK_C
#define VSFIELDMAX          64   /* max no of fields per vdata */
#else  /* !macintosh */
#define VSFIELDMAX         256  /* max no of fields per vdata */
#endif /* !macintosh */
#define VSNAMELENMAX        64  /* vdata name  : 64 chars max */
#define VGNAMELENMAX        64  /* vgroup name : 64 chars max */

/*
 * default max no of objects in a vgroup
 * VGroup will grow dynamically if needed
 */
#define MAXNVELT            64

/*
 * Defaults for linked block operations with Vsets
 */
#define VDEFAULTBLKSIZE    4096
#define VDEFAULTNBLKS        32

/* Max order of a field in a Vdata */
#define MAX_ORDER          32000
#define MAX_FIELD_SIZE     32000


/* ------------------------- Constants for hfile.c --------------------- */
/* Maximum number of files (number of slots for file records) */
#ifndef MAX_FILE
#if defined PC && !(defined PC386 || defined UNIX386)
#   define MAX_FILE    8
#else  /* !PC */
#   define MAX_FILE   32
#endif /* !PC */
#endif /* MAX_FILE */

/* Maximum length of external filename(s) (used in hextelt.c) */
#ifndef MAX_PATH_LEN
#if defined PC && !(defined PC386 || defined UNIX386)
#define MAX_PATH_LEN      256
#else  /* non-DOS systems */
#define MAX_PATH_LEN     1024
#endif /* PATH_LEN defines */
#endif /* MAX_PATH_LEN */

/* Maximum number of access elements */
#ifndef MAX_ACC
#   define MAX_ACC 256
#endif /* MAX_ACC */

/* ndds (number of dd's in a block) default,
   so user need not specify */
#ifndef DEF_NDDS
#   define DEF_NDDS 16
#endif /* DEF_NDDS */

/* ndds minimum, to prevent excessive overhead of very small dd-blocks */
#ifndef MIN_NDDS
#   define MIN_NDDS 4
#endif /* MIN_NDDS */

/* largest number that will fit into 16-bit word ref variable */
#define MAX_REF ((uint16)65535)

/* length of block and number of blocks for converting 'appendable' data */
/* elements into linked blocks (will eventually be replaced by the newer */
/* variable-length blocks */
#define HDF_APPENDABLE_BLOCK_LEN 4096
#define HDF_APPENDABLE_BLOCK_NUM 16

/* hashing information */
#define HASH_MASK       0xff
#define HASH_BLOCK_SIZE 100

/* ------------------------- Constants for Vxx interface --------------------- */

/* maximum number of files (number of slots for file records in Vxx interface) 
*  based on Hxx interface defintion of MAX_FILE */
#ifndef MAX_VFILE
#   define MAX_VFILE MAX_FILE
#endif

/*
 * Private conversion buffer stuff
 * VDATA_BUFFER_MAX is the largest buffer that can be allocated for
 *   writing (haven't implemented reading yet).
 * Vtbuf is the buffer
 * Vtbufsize is the buffer size in bytes at any given time.
 * Vtbuf is increased in size as need be
 * BUG: the final Vtbuf never gets freed
 */
#define VDATA_BUFFER_MAX 1000000

/* --------------------- Constants for DFSDxx interface --------------------- */

#define DFS_MAXLEN 255       /*  Max length of label/unit/format strings */
#define DFSD_MAXFILL_LEN 16  /* Current max length for fill_value space */

/* --------------------- Constants for HBITIO interface --------------------- */
/* maximum number of bitfile access elements */
/* (can be less than the MAX_ACC defined in hfile.h, but never greater) */
#ifndef MAX_BITFILE
#   define MAX_BITFILE MAX_ACC
#endif

/* ----------------- Constants for COMPRESSION interface --------------------- */

/* Set the following macro to the value the highest compression scheme is */
#define COMP_MAX_COMP   12

/* ----------------- Constants for DGROUP interface --------------------- */
#define MAX_GROUPS 8

/* ----------------- Constants for HERROR interface --------------------- */
#define FUNC_NAME_LEN   32

/* error_stack is the error stack.  error_top is the stack top pointer, 
   and points tothe next available slot on the stack */
#ifndef ERR_STACK_SZ
#   define ERR_STACK_SZ 10
#endif

/* max size of a stored error description */
#ifndef ERR_STRING_SIZE
#   define ERR_STRING_SIZE 512
#endif

/* ----------------- Constants for NETCDF interface(netcdf.h) ---------------- */
/*
 * This can be as large as the maximum number of stdio streams
 * you can have open on your system.
 */
#define MAX_NC_OPEN MAX_FILE

/*
 * These maximums are enforced by the interface, to facilitate writing
 * applications and utilities.  However, nothing is statically allocated to
 * these sizes internally.
 */
#define MAX_NC_DIMS 5000	 /* max dimensions per file */
#define MAX_NC_ATTRS 3000	 /* max global or per variable attributes */
#define MAX_NC_VARS 5000	 /* max variables per file */
#define MAX_NC_NAME 256		 /* max length of a name */
#define MAX_VAR_DIMS 32          /* max per variable dimensions */

#endif /* _HLIMITS_H */
