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

/*------------------------------------------------------------------------------
 * File:    mfani.h
 * Purpose: private header file for the Multi-file Annotation Interface
 *----------------------------------------------------------------------------*/

#ifndef H4_MFANI_H
#define H4_MFANI_H

#include "hdfi.h"

/* This structure is used to find which file the annotation belongs to
 * and use the subsequent file specific annotation 'key' to find the
 * annotation. The annotation atom group(ANIDGROUP) keeps track of
 * all annotations across the file. */
typedef struct ANnode {
    int32 file_id; /* which file this annotation belongs to */
    int32 ann_key; /* type/ref: used to find annotation in corresponding
                      TBBT in filerec_t->tree[]. */
    intn new_ann;  /* flag */
} ANnode;

/*
 * This structure is an entry in the label/desc tree
 * for a label/desc in the file, it gives the ref of the label/desc,
 * and the tag/ref of the data item to which the label/desc relates
 * The filerec_t->an_tree[] TBBT members will contain these entries.
 **/
typedef struct ANentry {
    int32  ann_id; /* annotation id */
    uint16 annref; /* ref of annotation */
    uint16 elmtag; /* tag of data */
    uint16 elmref; /* ref of data */
} ANentry;

/* This is the size of the hash tables used for annotation IDs */
#define ANATOM_HASH_SIZE 64

/* Used to create unique 32bit keys from annotation type and reference number
 *  This key is used to add nodes to a corresponding TBBT in
 *  filrerec_t->an_tree[].
 *  ----------------------------
 *  | type(16bits) | ref(16bits) |
 *  -----------------------------*/
#define AN_CREATE_KEY(t, r) ((((int32)t & 0xffff) << 16) | r)

/* Obtain Reference number from key */
#define AN_KEY2REF(k) ((uint16)((int32)k & 0xffff))

/* Obtain Annotation type from key */
#define AN_KEY2TYPE(k) ((int32)((int32)k >> 16))

#endif /* H4_MFANI_H */
