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

/*------------------------------------------------------------------------------
 * File:    mfan.h
 * Purpose: header file for the Multi-file Annotation Interface
 * Invokes: 
 * Contents:
 *          Structure definitions: ANnode, ANentry, ANfile
 *          Constant definitions:  AN_DATA_LABEL, AN_DATA_DESC
 *                                 AN_FILE_LABEL, AN_FILE_DESC
 * Remarks: none
 *----------------------------------------------------------------------------*/

#ifndef MFAN_H  /* avoid re-inclusion */
#define MFAN_H

#include "hdf.h"
#include "rb.h"  /* Red-Black tree routines */

/* enumerated types of the varous annotation types */
typedef enum 
{ 
    AN_DATA_LABEL = 0, /* Data label */
    AN_DATA_DESC,      /* Data description */
    AN_FILE_LABEL,     /* File label */
    AN_FILE_DESC       /* File description */
} ann_type;

/* This sturcture is used to find which file the annotation belongs to
 * and use the subsequent file specific annotation 'key' to find the 
 * annotation. The ANnodelist keeps track of all anotations across 
 * all the open files. */
typedef struct ANnode
{
  int32   file_id;  /* which file this annotation belongs to */
  int32   ann_key;  /* type/ref -used to find annotation in file's type RBtree*/
  intn    new_ann;  /* flag */
} ANnode;

/*
 * This structure is an entry in the label/desc red-black tree
 * for a label/desc in the file, it gives the ref of the label/desc,
 * and the tag/ref of the data item to which the label/desc relates */
typedef struct ANentry
{
  int32   ann_id;      /* annotation id */
  uint16  annref;      /* ref of annotation */
  uint16  elmtag;      /* tag of data */
  uint16  elmref;      /* ref of data */
} ANentry;

/* Structure for each file opened to insert in red-black tree */
typedef struct ANfile
{
  char    *filename;      /* File name */
  int32   access_mode;    /* access mode this file was opened with */
  intn    an_num[4];      /* Holds number of annotations found of each type */
  Rb_node an_tree[4];     /* Red-Black tree of annotations in file 
                           * i.e. file/data labels and descriptions */
} ANfile;

#ifdef MFAN_C
/* We are in annotation source file */

PRIVATE Rb_node ANfilelist = NULL; /* List of open files */
PRIVATE Rb_node ANnodelist = NULL; /* List of all anotations across files */
PRIVATE intn    num_anns   = 0;    /* total number of annotations 
                                      i.e. all files */

/* Used to create unique 32bit keys from annotation type and reference number 
 *  This key is used to add nodes to ANnodelist. */
/*  ----------------------------
 *  | t(16bits) | r(16bits) |
 *  -----------------------------*/
#define AN_CREATE_KEY(t,r) ((((int32)t & 0xff) << 16) | r)
#define AN_KEY2REF(k)      ((uint16)((int32)k & 0xff))
#define AN_KEY2TYPE(k)     ((int32)((int32)k & 0xff00) >> 16)

#else /* !MFAN_C */
/* We dont't EXPORT any global variables */

#endif /* !MFAN_C */


#endif /* DFAN_H */
