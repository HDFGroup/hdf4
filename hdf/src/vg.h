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

/*****************************************************************************
*
* vg.h
*
* Part of HDF VSet interface
*
* defines symbols and structures used in all v*.c files
*
* NOTES:
* This include file depends on the basic HDF *.h files hdfi.h and hdf.h.
* An 'S' in the comment means that that data field is saved in the HDF file.
*
******************************************************************************/

#ifndef _VG_H
#define _VG_H

#include "hdf.h"
#include "hfile.h"

/* Include file for Threaded, Balanced Binary Tree implementation */
#include "tbbt.h"

/*
   * definition of the 2 data elements of the vset.
 */

typedef struct vgroup_desc VGROUP;
typedef struct vdata_desc VDATA;

typedef VDATA VSUBGROUP;

/*
   * -----------------------------------------------------------------
   * structures that are part of the VDATA structure
   * -----------------------------------------------------------------
 */

typedef struct symdef_struct
  {
      char       *name;         /* symbol name */
      int16       type;         /* whether int, char, float etc */
      uint16      isize;        /* field size as stored in vdata */
      uint16      order;        /* order of field */
  }
SYMDEF;

typedef struct write_struct
  {
      intn        n;            /* S actual # fields in element */
      uint16      ivsize;       /* S size of element as stored in vdata */

      char        name[VSFIELDMAX][FIELDNAMELENMAX + 1];    /* S name of each field */

      int16       len[VSFIELDMAX];  /* S length of each fieldname */
      int16       type[VSFIELDMAX];     /* S field type */
      uint16      off[VSFIELDMAX];  /* S field offset in element in vdata */
      uint16      isize[VSFIELDMAX];    /* S internal (HDF) size [incl order] */
      uint16      order[VSFIELDMAX];    /* S order of field */
      uint16      esize[VSFIELDMAX];    /*  external (local machine) size [incl order] */
  }
VWRITELIST;

typedef struct dyn_write_struct
  {
      intn        n;        /* S actual # fields in element */
      uint16      ivsize;   /* S size of element as stored in vdata */
      char        **name;   /* S name of each field */
      int16       *type;    /* S field type */
      uint16      *off;     /* S field offset in element in vdata */
      uint16      *isize;   /* S internal (HDF) size [incl order] */
      uint16      *order;   /* S order of field */
      uint16      *esize;   /*  external (local machine) size [incl order] */
  }
DYN_VWRITELIST;

typedef struct dyn_read_struct
  {
      intn        n;            /* # fields to read */
      intn        *item;        /* index into vftable_struct */
  }
DYN_VREADLIST;

/*
   *  -----------------------------------------------
   V G R O U P     definition
   *  -----------------------------------------------
 */

struct vgroup_desc
  {
      uint16      otag, oref;   /* tag-ref of this vgroup */
      HFILEID     f;            /* HDF file id  */
      uint16      nvelt;        /* S no of objects */
      intn        access;       /* 'r' or 'w' */
      uint16     *tag;          /* S tag of objects */
      uint16     *ref;          /* S ref of objects */
      char        vgname[VGNAMELENMAX + 1];     /* S name of this vgroup */
      char        vgclass[VGNAMELENMAX + 1];    /* S class name of this vgroup */
      intn        marked;       /* =1 if new info has been added to vgroup */
      intn        new_vg;          /* =1 if this is a new Vgroup */
      uint16      extag, exref; /* expansion tag-ref */
      int16       version, more;    /* version and "more" field */
      intn        msize;        /* max size of storage arrays */
  };
/* VGROUP */

/*
   *  -----------------------------------------------
   *         V D A T A      definition
   *  -----------------------------------------------
 */

struct vdata_desc
  {
      uint16      otag, oref;   /* tag,ref of this vdata */
      HFILEID     f;            /* HDF file id */
      intn        access;       /* 'r' or 'w' */
      char        vsname[VSNAMELENMAX + 1];     /* S name of this vdata */
      char        vsclass[VSNAMELENMAX + 1];    /* S class name of this vdata */
      int16       interlace;    /* S  interlace as in file */
      int32       nvertices;    /* S  #vertices in this vdata */
      DYN_VWRITELIST  wlist;
      DYN_VREADLIST   rlist;
      int16       nusym;
      SYMDEF      *usym;
      intn        marked;       /* =1 if new info has been added to vdata */
      intn        islinked;     /* =1 if vdata is a linked-block in file */

      uint16      extag, exref; /* expansion tag-ref */
      int16       version, more;    /* version and "more" field */

      int32       aid;          /* access id - for LINKED blocks */
      struct vs_instance_struct *instance;  /* ptr to the intance struct for this VData */
  };                            /* VDATA */

/* --------------  H D F    V S E T   tags  ---------------------------- */

#define OLD_VGDESCTAG   61820   /* tag for a vgroup d */
#define OLD_VSDESCTAG   61821   /* tag for a vdata descriptor */
#define OLD_VSDATATAG   61822   /* tag for actual raw data of a vdata */

#define NEW_VGDESCTAG    1965
#define NEW_VSDESCTAG    1962
#define NEW_VSDATATAG    1963

#define VGDESCTAG       NEW_VGDESCTAG
#define VSDESCTAG       NEW_VSDESCTAG
#define VSDATATAG       NEW_VSDATATAG
#define _HDF_VSPACK 0
#define _HDF_VSUNPACK 1

/* .................................................................. */
/* Private data structures. Unlikely to be of interest to applications */
/*
   * These are just typedefs. Actual vfile_ts are declared PRIVATE and
   * are not accessible by applications. However, you may change VFILEMAX
   * to allow however many files to be opened.
   *
   * These are memory-resident copies of the tag-refs of the vgroups
   * and vdatas for each file that is opened.
   *
 */

/* this is a memory copy of a vg tag/ref found in the file */
typedef struct vg_instance_struct
  {
      int32       key;          /* key to look up with the B-tree routines */
      /* needs to be first in the structure */
      uintn       ref;          /* ref # of this vgroup in the file */
      /* needs to be second in the structure */
      intn        nattach;      /* # of current attachs to this vgroup */
      int32       nentries;     /* # of entries in that vgroup initially */
      VGROUP     *vg;           /* points to the vg when it is attached */
  }
vginstance_t;

/* this is a memory copy of a vs tag/ref found in the file */
typedef struct vs_instance_struct
  {
      int32       key;          /* key to look up with the B-tree routines */
      /* needs to be first in the structure */
      uintn       ref;          /* ref # of this vdata in the file */
      /* needs to be second in the structure */
      intn        nattach;      /* # of current attachs to this vdata */
      int32       nvertices;    /* # of elements in that vdata initially */
      VDATA      *vs;           /* points to the vdata when it is attached */
  }
vsinstance_t;

/* each vfile_t maintains 2 linked lists: one of vgs and one of vdatas
   * that already exist or are just created for a given file.  */

typedef struct vfiledir_struct
  {
      int32            f;       /* HDF File ID */

      int32       vgtabn;       /* # of vg entries in vgtab so far */
      TBBT_TREE  *vgtree;       /* Root of VGroup B-Tree */

      int32       vstabn;       /* # of vs entries in vstab so far */
      TBBT_TREE  *vstree;       /* Root of VSet B-Tree */
      intn        access;       /* the number of active pointers to this file's Vstuff */
  }
vfile_t;

/* Size of the atom hash table */
#define VATOM_HASH_SIZE 256

/* .................................................................. */
#define VSET_VERSION   3    /* DO NOT CHANGE!! */
#define VSET_OLD_TYPES 2    /* All version <= 2 use old type mappings */

/* .................................................................. */

#if defined c_plusplus || defined __cplusplus
extern      "C"
{
#endif                          /* c_plusplus || __cplusplus */

/*
 * Routines public to the VSet layer
 */
    extern vfile_t *Get_vfile(HFILEID f);

    extern vsinstance_t *vsinst
                (HFILEID f, uint16 vsid);

    extern vginstance_t *vginst
            (HFILEID f, uint16 vgid);

    extern DYN_VWRITELIST *vswritelist
                (int32 vskey);

    extern void vpackvg
                (VGROUP * vg, uint8 buf[], int32 * size);

    extern int32 vinsertpair
                (VGROUP * vg, uint16 tag, uint16 ref);

    extern void vpackvs
                (VDATA * vs, uint8 buf[], int32 * size);

    extern VGROUP *VPgetinfo
                (HFILEID f,uint16 ref);

    extern VDATA *VSPgetinfo
                (HFILEID f,uint16 ref);

    extern int16 map_from_old_types
                (intn type);

#if defined c_plusplus || defined __cplusplus
}
#endif                          /* c_plusplus || __cplusplus */

#endif                          /* _VG_H */
