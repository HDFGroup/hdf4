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

/*-----------------------------------------------------------------------------
 * File:    mfgr.h
 * Purpose: header file for multi-file general raster information
 * Dependencies: 
 * Invokes:
 * Contents:
 * Structure definitions: 
 * Constant definitions: 
 *---------------------------------------------------------------------------*/

/* avoid re-inclusion */
#ifndef __MFGR_H
#define __MFGR_H

#include "tbbt.h"       /* Get tbbt routines */

/* Interlace types available */
typedef enum
  {
      MFGR_INTERLACE_PIXEL = 0, /* pixel interlacing scheme */
      MFGR_INTERLACE_LINE,      /* line interlacing scheme */
      MFGR_INTERLACE_COMPONENT  /* component interlacing scheme */
  }
gr_interlace_t;

#ifdef MFGR_MASTER

/* By default this is the same as the number of files allowed to be open */
#define MAX_GR_FILES    MAX_FILE

/* The names of the Vgroups created by the GR interface */
#define GR_NAME "RIG0.0"    /* name of the Vgroup containing all the images */
#define RI_NAME "RI0.0"     /* name of a Vgroup containing information about one image */
#define RIGATTRNAME  "RIATTR0.0N"  /* name of a Vdata containing an attribute */
#define RIGATTRCLASS "RIATTR0.0C"  /* class of a Vdata containing an attribute */

/* The tag of the attribute data */
#define ATTR_TAG    DFTAG_VH    /* Current GR attributes are stored in VDatas */

/* The default threshhold for attributes which will be cached */
#define GR_ATTR_THRESHHOLD  2048    

/*
 * NOTE:  People at large should not use this macro as they do not
 *        have access to gr_tab[]
 */
#define Get_grfile(f) (f>=0 ? (gr_tab[(f & 0xffff)]) : NULL)

#define GRIDTYPE  11    /* for GR access (also defined in hfile.h) */
#define RIIDTYPE  12    /* for RI access (also defined in hfile.h) */

/* GRID and RIID's are composed of the following fields:    */
/*      Top 8 Bits:GRID/RIID constant (for identification) */
/*      Next 8 Bits: File ID (can be used for Get_vfile)     */
/*      Bottom 16 Bits: ID for the individual GR group/RI image */

#define GRSLOT2ID(f,s) ( (((uint32)f & 0xff) << 16) | \
                    (((uint32)GRIDTYPE & 0xff) << 24) | ((s) & 0xffff) )
#define VALIDGRID(i) (((((uint32)(i) >> 24) & 0xff) == GRIDTYPE) && \
                    ((((uint32)(i) >> 16)  & 0xff) < MAX_GR_FILES))
#define GRID2SLOT(i) (VALIDGRID(i) ? (uint32)(i) & 0xffff : -1)
#define GRID2VFILE(i) (VALIDGRID(i) ? ((uint32)(i) >> 16) & 0xff : -1)

#define RISLOT2ID(g,s) ( (((uint32)g & 0xff) << 16) | \
                    (((uint32)RIIDTYPE & 0xff) << 24) | ((s) & 0xffff) )
#define VALIDRIID(i) ((((uint32)(i) >> 24) & 0xff) == RIIDTYPE)
#define RIID2SLOT(i) (VALIDRIID(i) ? (uint32)(i) & 0xffff : -1)
#define RIID2GRID(i) (VALIDRIID(i) ? ((uint32)(i) >> 16) & 0xff : -1)


/*
 * Each gr_info_t maintains 2 threaded-balanced-binary-tress: one of
 * raster images and one of global attributes
 */

typedef int32 treekey;          /* type of keys for */

typedef struct gr_info {
    int32       hdf_file_id;    /* the corresponding HDF file ID */
    uint16      gr_ref;         /* ref # of the Vgroup of the GR in the file */

    int32       gr_count;       /* # of image entries in gr_tab so far */
    TBBT_TREE  *grtree;         /* Root of image B-Tree */
    uintn       gr_modified;    /* whether any images have been modified */

    int32       gattr_count;    /* # of global attr entries in gr_tab so far */
    TBBT_TREE  *gattree;        /* Root of global attribute B-Tree */
    uintn       gattr_modified; /* whether any global attributes have been modified */

    intn        access;         /* the number of active pointers to this file's GRstuff */
    uint32      attr_cache;     /* the threshhold for the attribute sizes to cache */
} gr_info_t;

gr_info_t *gr_tab[MAX_GR_FILES]={0};

typedef struct at_info {
    int32 index;            /* index of the attribute (needs to be first in the struct) */
    int32 nt;               /* number type of the attribute */
    int32 len;              /* length/order of the attribute */
    uint16 ref;             /* ref of the attribute (stored in VData) */
    uintn data_modified;    /* flag to indicate whether the attribute data has been modified */
    uintn new;              /* flag to indicate whether the attribute was added to the Vgroup */
    char *name;             /* name of the attribute */
    VOIDP data;             /* data for the attribute */
} at_info_t;

typedef struct dim_info {
    uint16  dim_ref;            /* reference # of the Dim record */
    int32   xdim,ydim,          /* dimensions of the image */
            ncomps,             /* number of components of each pixel in image */
            nt,                 /* number type of the components */
            file_nt_subclass,   /* number type subclass of data on disk */
            il;                 /* interlace of the components */
    uint16  nt_tag,nt_ref;      /* tag & ref of the number-type info */
    uint16  comp_tag,comp_ref;  /* tag & ref of the compression info */
} dim_info_t;

typedef struct ri_info {
    int32   index;              /* index of this image (needs to be first in the struct) */
    uint16  ri_ref;             /* ref # of the RI Vgroup */
    uint16  rig_ref;            /* ref # of the RIG group */
    dim_info_t img_dim;         /* image dimension information */
    dim_info_t lut_dim;         /* palette dimension information */
    uint16  img_tag,img_ref;    /* tag & ref of the image data */
    uint16  lut_tag,lut_ref;    /* tag & ref of the palette data */
    uintn data_modified;        /* whether the image or palette data has been modified */
    uintn meta_modified;        /* whether the image or palette meta-info has been modified */
    uintn attr_modified;        /* whether the attributes have been modified */
    char   *name;               /* name of the image */
    int32   lattr_count;        /* # of local attr entries in ri_info so far */
    TBBT_TREE *lattree;         /* Root of the local attribute B-Tree */
    intn access;                /* the number of times this image has been selected */
} ri_info_t;

#endif /* MFGR_MASTER */

#endif /* __MFGR_H */
