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
#include "fmpconf.h"
#include "fmptypes.h"
#include "fmpool.h"

/* Memory Pool file structure */
typedef struct mpfile_st 
{
  fmp_file_t  fd;    /* file handle */
  pgno_t    curp;  /* current page */
  off_t     poff;  /* offset into current page */
  off_t     foff;  /* current offset into file */
  MPOOL     *mp;   /* memory pool for this file */
} MPFILE;

/* File memory pool fucntions */
extern MPFILE *MPopen( const char * path, int flags );
extern int     MPclose(MPFILE *mpfs);
extern int     MPflush(MPFILE *mpfs);
extern int     MPseek(MPFILE *mpfs, off_t offset, int whence );
extern int     MPread(MPFILE *mpfs, void *buf, size_t nbytes );
extern int     MPwrite(MPFILE *mpfs, void *buf, size_t nbytes );
#endif /* _FMPIO_H */
