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

#ifndef _MFAN_H  /* avoid re-inclusion */
#define _MFAN_H

#include "hdf.h"

#ifdef HAVE_RBTREE
#include "rb.h"  /* Red-Black tree routines */
#else /* use tbbt */
#include "tbbt.h"
#endif /* use tbbt */

#if 0
/* enumerated types of the varous annotation types 
 * NOTE: moved to hdf.h */
typedef enum 
{ 
  AN_DATA_LABEL = 0, /* Data label */
  AN_DATA_DESC,      /* Data description */
  AN_FILE_LABEL,     /* File label */
  AN_FILE_DESC       /* File description */
} ann_type;
#endif

/* This sturcture is used to find which file the annotation belongs to
 * and use the subsequent file specific annotation 'key' to find the 
 * annotation. The ANnodelist keeps track of all anotations across 
 * all the open files. */
typedef struct ANnode
{
  int32   file_id;  /* which file this annotation belongs to */
  int32   ann_key;  /* type/ref -used to find annotation in file's type tree*/
  intn    new_ann;  /* flag */
} ANnode;

/*
 * This structure is an entry in the label/desc tree
 * for a label/desc in the file, it gives the ref of the label/desc,
 * and the tag/ref of the data item to which the label/desc relates */
typedef struct ANentry
{
  int32   ann_id;      /* annotation id */
  uint16  annref;      /* ref of annotation */
  uint16  elmtag;      /* tag of data */
  uint16  elmref;      /* ref of data */
} ANentry;

/* Structure for each file opened to insert in tree */
typedef struct ANfile
{
  char    *filename;      /* File name */
  int32   access_mode;    /* access mode this file was opened with */
  intn    an_num[4];      /* Holds number of annotations found of each type */
#ifdef HAVE_RBTREE
  Rb_node an_tree[4];     /* RB-tress for each type of annotation in file 
                           * i.e. file/data labels and descriptions */
#else  /* use tbbt */
  TBBT_TREE *an_tree[4];  /* tbbt trees for each type of annotation in file 
                           * i.e. file/data labels and descriptions */
#endif /* use tbbt */
} ANfile;

#ifdef MFAN_C
/* WE ARE IN MAIN ANNOTATION SOURCE FILE "mfan.c" */

/* PRIVATE variables and defintions */

#ifdef HAVE_RBTREE
EXPORT Rb_node ANfilelist = NULL; /* List of open files */
EXPORT Rb_node ANnodelist = NULL; /* List of all anotations across files */
#else /* use tbbt */
EXPORT TBBT_TREE *ANfilelist = NULL; /* List of open files */
EXPORT TBBT_TREE *ANnodelist = NULL; /* List of all anotations across files */
#endif /* use tbbt */
PRIVATE intn    num_anns   = 0;    /* total number of annotations 
                                      i.e. all files */

/* Used to create unique 32bit keys from annotation type and reference number 
 *  This key is used to add nodes to ANnodelist. 
 *  ----------------------------
 *  | t(16bits) | r(16bits) |
 *  -----------------------------*/
#define AN_CREATE_KEY(t,r) ((((int32)t & 0xffff) << 16) | r)

/* Obtain Reference number from key */
#define AN_KEY2REF(k)      ((uint16)((int32)k & 0xffff))

/* Obtain Annotation type from key */
#define AN_KEY2TYPE(k)     ((int32)((int32)k >> 16))

#else /* !MFAN_C */
/* WE are NOT in main ANNOTATION source file
 * Nothing EXPORTED  */

#endif /* !MFAN_C */

#endif /* _MFAN_H */
