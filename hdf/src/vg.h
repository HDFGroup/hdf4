/*
$Header$

$Log$
Revision 1.9  1993/04/19 22:48:28  koziol
General Code Cleanup to reduce/remove errors on the PC

 * Revision 1.8  1993/04/08  18:33:52  chouck
 * Various Vset modifications (additions of Vdelete and VSdelete)
 *
 * Revision 1.7  1993/04/06  17:23:41  chouck
 * Added Vset macros
 *
 * Revision 1.6  1993/03/29  16:50:30  koziol
 * Updated JPEG code to new JPEG 4 code.
 * Changed VSets to use Threaded-Balanced-Binary Tree for internal
 * 	(in memory) representation.
 * Changed VGROUP * and VDATA * returns/parameters for all VSet functions
 * 	to use 32-bit integer keys instead of pointers.
 * Backed out speedups for Cray, until I get the time to fix them.
 * Fixed a bunch of bugs in the little-endian support in DFSD.
 *
 * Revision 1.5  1993/01/19  05:56:15  koziol
 * Merged Hyperslab and JPEG routines with beginning of DEC ALPHA
 * port.  Lots of minor annoyances fixed.
 *
 * Revision 1.4  1992/11/30  22:00:01  chouck
 * Added fixes for changing to Vstart and Vend
 *
 * Revision 1.3  1992/11/24  17:43:26  chouck
 * Fixed memory over-write when VGroups have lots of members
 *
 * Revision 1.2  1992/11/02  16:35:41  koziol
 * Updates from 3.2r2 -> 3.3
 *
 * Revision 1.1  1992/08/25  21:40:44  koziol
 * Initial revision
 *
*/
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
#include "herr.h"

/* Include file for Threaded, Balanced Binary Tree implementation */
#include "tbbt.h"

/* H-level customization jason ng 12-Feb-92 */
typedef int32           HFILEID;

/* 
* interlacing supported by the vset. 
*/

#define FULL_INTERLACE	0
#define NO_INTERLACE    1

/* 
* some max lengths 
*
* Except for FIELDNAMELENMAX, change these as you please, they 
* affect memory only, not the file. 
*
*/

#define FIELDNAMELENMAX     16  /* fieldname   : 16 chars max */

#define VSFIELDMAX          20  /* max no of fields per vdata */
#define VSNAMELENMAX		64	/* vdata name  : 64 chars max */    
#define VGNAMELENMAX		64	/* vgroup name : 64 chars max */ 

/* maximum number of files (number of slots for file records) */

#ifndef MAX_VFILE
#   define MAX_VFILE 16
#endif

/*
* definition of the 2 data elements of the vset.
*/

typedef struct vgroup_desc     	VGROUP;
typedef struct vdata_desc       VDATA;

typedef VDATA VSUBGROUP;


/*
* -----------------------------------------------------------------
* structures that are part of the VDATA structure
* -----------------------------------------------------------------
*/

typedef struct symdef_struct 
{
  char* name;			/* symbol name */
  intn	type;			/* whether int, char, float etc */
  intn	isize;			/* field size as stored in vdata */
  intn	order;			/* order of field */
} SYMDEF;

typedef struct vdata_memory_struct
{
  int32 n;                   /* byte size */
  uint8 * mem;
  struct vdata_memory_struct * next;
} VMBLOCK;

typedef struct write_struct	
{
  intn  n;                  /* S actual # fields in element */
  int16 ivsize;             /* S size of element as stored in vdata */
  char 	name[VSFIELDMAX][FIELDNAMELENMAX+1]; /* S name of each field */
  
  int16	len[VSFIELDMAX];     /* S length of each fieldname */
  intn  type[VSFIELDMAX];    /* S field type */
  int16	off[VSFIELDMAX];     /* S field offset in element in vdata */
  intn  isize[VSFIELDMAX];   /* S internal (HDF) size [incl order] */
  intn  order[VSFIELDMAX];   /* S order of field */
  int16	esize[VSFIELDMAX];   /*  external (local machine) size [incl order] */
  int32	(*toIEEEfn  [VSFIELDMAX] )();
  int32	(*fromIEEEfn[VSFIELDMAX] )();
} VWRITELIST;

typedef struct read_struct
{
  intn  n; 			/* # fields to read */
  intn	item[VSFIELDMAX]; 	/* index into vftable_struct */
} VREADLIST;

/* 
*  ----------------------------------------------- 
        V G R O U P     definition     
*  ----------------------------------------------- 
*/

#define MAXNVELT  64		/* max no of objects in a vgroup */

struct vgroup_desc
{ 
  uint16  otag, oref;	   	/* tag-ref of this vgroup */
  HFILEID f;	 	    	/* HDF file id  */
  uint16  nvelt;            /* S no of objects */
  intn    access;           /* 'r' or 'w' */
  uint16  *tag;             /* S tag of objects */
  uint16  *ref;             /* S ref of objects */
  char    vgname[VGNAMELENMAX+1];   /* S name of this vgroup */
  char    vgclass[VGNAMELENMAX+1];  /* S class name of this vgroup */
  intn    marked;           /* =1 if new info has been added to vgroup */
  uint16  extag, exref;     /* expansion tag-ref */
  int16	  version, more;	/* version and "more" field */	
  intn    msize;            /* max size of storage arrays */
};      
/* VGROUP */

/*
*  ----------------------------------------------- 
*         V D A T A      definition   
*  ----------------------------------------------- 
*/

#define USYMMAX 36			/* max user-defined symbols allowed */

struct vdata_desc { 
  uint16	otag, oref; 		/* tag,ref of this vdata */
  HFILEID   f;                  /* HDF file id */
  intn      access;             /* 'r' or 'w' */
  char      vsname[VSNAMELENMAX+1]; /* S name of this vdata */
  char		vsclass[VSNAMELENMAX+1];/* S class name of this vdata */
  int16     interlace;          /* S  interlace as in file */
  int32     nvertices;          /* S  #vertices in this vdata */
  VWRITELIST	wlist;
  VREADLIST	rlist;
  int16  	nusym;
  SYMDEF 	usym[USYMMAX];
  intn 	        marked;			/* =1 if new info has been added to vdata */
  intn          islinked; 		/* =1 if vdata is a linked-block in file */
  
  uint16	extag, exref;		/* expansion tag-ref */
  int16		version, more;		/* version and "more" field */	
  
  VMBLOCK * vm;
  int32 	aid;  /* access id - for LINKED blocks */
  struct vs_instance_struct *instance; /* ptr to the intance struct for this VData */
}; /* VDATA */ 

/* 
  with the definition of Vobject handles these macros have been replaced
  with functions of the *SAME* name
*/
#ifdef OLD_MACROS
/* macros - Use these for accessing items in a vdata or a group. */
#define VQuerytag(vgroup)	(vgroup->otag)
#define VQueryref(vgroup)	(vgroup->oref)
#define VSQuerytag(vdata)	(vdata->otag)
#define VSQueryref(vdata)	(vdata->oref)

/* macros - Use these for accessing user-defined fields in a vdata. */
#define VFnfields(vdata)        (vdata->wlist.n)
#define VFfieldname(vdata,t) 	(vdata->wlist.name[t])
#define VFfieldtype(vdata,t) 	(vdata->wlist.type[t])
#define VFfieldisize(vdata,t) 	(vdata->wlist.isize[t])
#define VFfieldesize(vdata,t) 	(vdata->wlist.esize[t])
#define VFfieldorder(vdata,t) 	(vdata->wlist.order[t])
#endif /* OLD_MACROS */

/* --------------  H D F    V S E T   tags  ---------------------------- */

#define OLD_VGDESCTAG  	61820	/* tag for a vgroup d*/ 
#define OLD_VSDESCTAG 	61821	/* tag for a vdata descriptor */
#define OLD_VSDATATAG 	61822	/* tag for actual raw data of a vdata */ 

#define NEW_VGDESCTAG    1965
#define NEW_VSDESCTAG    1962
#define NEW_VSDATATAG    1963

#define VGDESCTAG 		NEW_VGDESCTAG 
#define VSDESCTAG 		NEW_VSDESCTAG 
#define VSDATATAG  		NEW_VSDATATAG 

/*
* types used in defining a new field via a call to VSfdefine
*/

#define LOCAL_NOTYPE        0
#define LOCAL_CHARTYPE      1   /* 8-bit ascii text stream */
#define LOCAL_INTTYPE       2   /* 32-bit integers - don't use */
#define LOCAL_FLOATTYPE		3	/* as opposed to DOUBLE */ 
#define LOCAL_LONGTYPE      4   /* 32-bit integers */
#define LOCAL_BYTETYPE 	 	5	/* 8-bit byte stream - unsupported */
#define LOCAL_SHORTTYPE     6   /* 16-bit integers - unsupported */
#define LOCAL_DOUBLETYPE 	7	/* as opposed to FLOAT - unsupported */

/*
* actual LOCAL MACHINE sizes of the above types
*/

#define LOCAL_UNTYPEDSIZE  0
#define LOCAL_CHARSIZE  	sizeof(char)
#define LOCAL_INTSIZE  		sizeof(int)
#define LOCAL_FLOATSIZE 	sizeof(float)
#define LOCAL_LONGSIZE 		sizeof(long)
#define LOCAL_BYTESIZE  	sizeof(unsigned char)
#define LOCAL_SHORTSIZE  	sizeof(short)
#define LOCAL_DOUBLESIZE  	sizeof(double)

/* need to tune these to vertex boundary later.  Jason Ng 6-APR-92 */
#define VDEFAULTBLKSIZE    1024
#define VDEFAULTNBLKS      32

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
typedef struct vg_instance_struct {
    int32    key;           /* key to look up with the B-tree routines */
                            /* needs to be first in the structure */
    uintn    ref;           /* ref # of this vgroup in the file */
                            /* needs to be second in the structure */
    intn    nattach;        /* # of current attachs to this vgroup */
    int32   nentries;       /* # of entries in that vgroup initially */
    VGROUP 	*vg;			/* points to the vg when it is attached */
} vginstance_t; 

/* this is a memory copy of a vs tag/ref found in the file */
typedef struct vs_instance_struct {
    int32    key;           /* key to look up with the B-tree routines */
                            /* needs to be first in the structure */
    uintn    ref;           /* ref # of this vdata in the file */
                            /* needs to be second in the structure */
    intn    nattach;        /* # of current attachs to this vdata */
    int32   nvertices;      /* # of elements in that vdata initially */
	VDATA	*vs; 			/* points to the vdata when it is attached */
}	vsinstance_t; 

/* each vfile_t maintains 2 linked lists: one of vgs and one of vdatas
* that already exist or are just created for a given file.  */

typedef int32 treekey;          /* type of keys for */

typedef struct vfiledir_struct {
    int32       vgtabn;         /* # of vg entries in vgtab so far */
    TBBT_TREE   *vgtree;        /* Root of VGroup B-Tree */
  
    int32       vstabn;         /* # of vs entries in vstab so far */
    TBBT_TREE   *vstree;         /* Root of VSet B-Tree */
    intn        access;     /* the number of active pointers to this file's Vstuff */
} vfile_t;

/*
 * NOTE:  People at large should not use this macro as they do not
 *        have access to vfile[]
 */
#define Get_vfile(f) (f>=0 ? (&vfile[(f & 0xffff)]) : NULL)

#define VGIDTYPE  8         /* Also defined in hfile.h */
#define VSIDTYPE  9         /* Also defined in hfile.h */

/* VGID and VSID's are composed of the following fields:    */
/*      Top 8 Bits: File ID (can be used for Get_vfile)     */
/*      Next 8 Bits:VGID/VSID constant (for identification) */
/*      Bottom 16 Bits: ID for the individual VGroup/VSet   */

#define VGSLOT2ID(f,s) ( (((uint32)f & 0xff) << 16) | \
                    (((uint32)VGIDTYPE & 0xff) << 24) | ((s) & 0xffff) )
#define VALIDVGID(i) (((((uint32)(i) >> 24) & 0xff) == VGIDTYPE) && \
                    ((((uint32)(i) >> 16)  & 0xff) < MAX_VFILE))
#define VGID2SLOT(i) (VALIDVGID(i) ? (uint32)(i) & 0xffff : -1)
#define VGID2VFILE(i) (VALIDVGID(i) ? ((uint32)(i) >> 16) & 0xff : -1)

#define VSSLOT2ID(f,s) ( (((uint32)f & 0xff) << 16) | \
                    (((uint32)VSIDTYPE & 0xff) << 24) | ((s) & 0xffff) )
#define VALIDVSID(i) (((((uint32)(i) >> 24) & 0xff) == VSIDTYPE) && \
                    ((((uint32)(i) >> 16)  & 0xff) < MAX_VFILE))
#define VSID2SLOT(i) (VALIDVSID(i) ? (uint32)(i) & 0xffff : -1)
#define VSID2VFILE(i) (VALIDVSID(i) ? ((uint32)(i) >> 16) & 0xff : -1)

/* .................................................................. */
#define VSET_VERSION   3     /* DO NOT CHANGE!! */
#define VSET_OLD_TYPES 2     /* All version <= 2 use old type mappings */

/* .................................................................. */

/* all these macros should be public for users */
/* macros for VSinquire */

#define VSQuerycount(vs, count) \
        (VSinquire (vs, (int32 *) count, (int32*) NULL, (char*) NULL, (int32*) NULL, (char*) NULL))

#define VSQueryinterlace(vs, intr) \
        (VSinquire (vs, (int32 *) NULL, (int32*) intr, (char*) NULL, (int32*) NULL, (char*) NULL))

#define VSQueryfields(vs, flds) \
        (VSinquire (vs, (int32 *) NULL, (int32*) NULL, (char*) flds, (int32*) NULL, (char*) NULL))

#define VSQueryvsize(vs, size) \
        (VSinquire (vs, (int32 *) NULL, (int32*) NULL, (char*) NULL, (int32*) size, (char*) NULL))

#define VSQueryname(vs, name) \
        (VSinquire (vs, (int32 *) NULL, (int32*) NULL, (char*) NULL, (int32*) size, (char*) name))

/*
 *   Macros to provide fast access to the name and class of a Vset
 *     element.  The information returned is only guarenteed to be
 *     valid as long as the provided Vstructure is attached
 *
 *   The macros are NO LONGER SUPPORTED
 */

#ifdef OLD_MACROS

#define VGCLASS(vg) ((vg)->vgclass)
#define VGNAME(vg)  ((vg)->vgname)

#define VSCLASS(vs) ((vs)->vsclass)
#define VSNAME(vs)  ((vs)->vsname)
#endif /* OLD_MACROS */

#include "vproto.h"

#define Vstart(f)          Vinitialize((f))
#define Vend(f)            Vfinish((f))

#ifdef OLD_MACROS
#define DFvsetopen(x,y,z)  Vopen((x),(y),(z))
#define DFvsetclose(x)     Vclose((x))
#endif /* OLD_MACROS */

#endif /* _VG_H */

