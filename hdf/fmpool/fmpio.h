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

/*****************************************************************************
 * File: fmpio.h
 *
 *****************************************************************************/ 

/* $Id$ */

#ifndef _FMPIO_H
#define _FMPIO_H

#ifdef __FMPINTERFACE_PRIVATE
#include "fmpconf.h"
#include "fmptypes.h"
#include "fmpool.h"

/* Memory Pool file structure 
* Note that we keep a ptr to the last  page read. 
* This is probably not a good idea in a threaded environment */
typedef struct mpfile_st 
{
  fmp_file_t  fd;     /* file handle */
  int         oflags; /* open flags */
  pageno_t    curpr;  /* current page read */
  pageno_t    curp;   /* current page */
  off_t       poff;   /* offset into current page */
  off_t       foff;   /* current offset into file */
  void        *rpage; /* ptr to last page read */
  MPOOL       *mp;    /* memory pool for this file */
} MPFILE;

#else /* __FMPINTERFACE_PRIVATE */
/* What we define to the user */

/* Hidden data types */
typedef void *MPFILE;

/* file access codes (also found in fmptypes.h) */
#ifndef DFACC_READ
#define DFACC_READ   1
#define DFACC_WRITE  2
#define DFACC_CREATE 4
#define DFACC_ALL    7
#define DFACC_RDONLY 1
#define DFACC_RDWR   3
#define DFACC_CLOBBER 4
#endif  /* DFACC_READ */

#endif /* __FMPINTERFACE_PRIVATE */

/* File memory pool fucntions */
extern MPFILE *MPopen( const char * path, int flags );
extern int     MPclose(MPFILE *mpfs);
extern int     MPflush(MPFILE *mpfs);
extern int     MPseek(MPFILE *mpfs, off_t offset, int whence );
extern int     MPread(MPFILE *mpfs, void *buf, size_t nbytes );
extern int     MPwrite(MPFILE *mpfs, void *buf, size_t nbytes );
#endif /* _FMPIO_H */
