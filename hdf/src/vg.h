/*
$Header$

$Log$
Revision 1.4  1992/11/30 22:00:01  chouck
Added fixes for changing to Vstart and Vend

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
*  Likkai Ng MAY 91  NCSA
* 
* vg.h
* Part of HDF VSet interface
*
* defines symbols and structures used in all v*.c files    
*
* NOTES:
* This include file depends on the basic HDF *.h files dfi.h and df.h.
* An 'S' in the comment means that that data field is saved in the HDF file.
*
******************************************************************************/

#ifndef _VG_H
#define _VG_H

#include "hdf.h"
#include "herr.h"

/* H-level customization jason ng 12-Feb-92 */
typedef int32           HFILEID;
typedef unsigned char   BYTE;

#define QQnewref Hnewref
#define QQstartread Hstartread 
#define QQstartwrite Hstartwrite
#define QQendaccess  Hendaccess
#define QQQueryspecial HQueryspecial
#define QQQuerytagref  HQuerytagref 
#define QQQuerylength  HQuerylength
#define QQgetelement  Hgetelement 
#define QQputelement  Hputelement
#define QQnextread  Hnextread
#define QQdupdd  Hdupdd
#define QQopen Hopen
#define QQclose Hclose
#define QQseek Hseek
#define QQread Hread
#define QQwrite Hwrite

/* 
* interlacing supported by the vset. 
*/

#define FULL_INTERLACE	0
#define NO_INTERLACE		1

/* 
* some max lengths 
*
* Except for FIELDNAMELENMAX, change these as you please, they 
* affect memory only, not the file. 
*
*/

#define FIELDNAMELENMAX	        16	/* fieldname   : 16 chars max */

#define VSFIELDMAX		20  	/* max no of fields per vdata */
#define VSNAMELENMAX		64	/* vdata name  : 64 chars max */    
#define VGNAMELENMAX		64	/* vgroup name : 64 chars max */ 


/*
* definition of the 2 data elements of the vset.
*/

typedef struct vgroup_desc     	VGROUP;
typedef struct vdata_desc			VDATA;

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
  BYTE  * mem;
  struct vdata_memory_struct * next;
} VMBLOCK;


typedef struct write_struct	
{
  int16	n;		/* S actual # fields in element */
  int16	ivsize;		/* S size of element as stored in vdata */
  char 	name[VSFIELDMAX][FIELDNAMELENMAX+1]; /* S name of each field */
  
  int16	len[VSFIELDMAX];     /* S length of each fieldname */
  int16	type[VSFIELDMAX];    /* S field type */
  int16	off[VSFIELDMAX];     /* S field offset in element in vdata */
  int16 isize[VSFIELDMAX];   /* S internal (HDF) size [incl order] */
  int16	order[VSFIELDMAX];   /* S order of field */
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

#define MAXNVELT  32		/* max no of objects in a vgroup */

struct vgroup_desc
{ 
  uint16  otag, oref;	   	/* tag-ref of this vgroup */
  HFILEID f;	 	    	/* HDF file id  */
  uint16  nvelt; 		/* S no of objects */
  intn	  access;		/* 'r' or 'w' */
  uint16  *tag;	                /* S tag of objects */
  uint16  *ref;	                /* S ref of objects */
  char	  vgname[VGNAMELENMAX+1];/* S name of this vgroup */
  char	  vgclass[VGNAMELENMAX+1];/* S class name of this vgroup */
  intn	  marked;		/* =1 if new info has been added to vgroup */
  uint16  extag, exref;	        /* expansion tag-ref */
  int16	  version, more;	/* version and "more" field */	
  intn    msize;                /* max size of storage arrays */
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
  HFILEID	f; 	  		/* HDF file id */
  intn		access;			/* 'r' or 'w' */
  char		vsname[VSNAMELENMAX+1]; /* S name of this vdata */
  char		vsclass[VSNAMELENMAX+1];/* S class name of this vdata */
  int16		interlace;		/* S  interlace as in file */
  int32		nvertices;		/* S  #vertices in this vdata */
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

/* macros - Use these for accessing items in a vdata or a group. */
#define VQuerytag(vgroup)	(vgroup->otag)
#define VQueryref(vgroup)	(vgroup->oref)
#define VSQuerytag(vdata)	(vdata->otag)
#define VSQueryref(vdata)	(vdata->oref)

/* macros - Use these for accessing user-defined fields in a vdata. */
#define VFnfields(vdata) 	(vdata->wlist.n)
#define VFfieldname(vdata,t) 	(vdata->wlist.name[t])
#define VFfieldtype(vdata,t) 	(vdata->wlist.type[t])
#define VFfieldisize(vdata,t) 	(vdata->wlist.isize[t])
#define VFfieldesize(vdata,t) 	(vdata->wlist.esize[t])
#define VFfieldorder(vdata,t) 	(vdata->wlist.order[t])

/* --------------  H D F    V S E T   tags  ---------------------------- */

#define OLD_VGDESCTAG  	61820		/* tag for a vgroup d*/ 
#define OLD_VSDESCTAG 	61821		/* tag for a vdata descriptor */
#define OLD_VSDATATAG 	61822		/* tag for actual raw data of a vdata */ 

#define NEW_VGDESCTAG    1965
#define NEW_VSDESCTAG    1962
#define NEW_VSDATATAG    1963

#define VGDESCTAG 		NEW_VGDESCTAG 
#define VSDESCTAG 		NEW_VSDESCTAG 
#define VSDATATAG  		NEW_VSDATATAG 

/*
* Actual sizes of data types stored in HDF file. see hdf.h
*/

/*
* Actual sizes of data types stored in HDF file, and are IEEE-defined. 
*  == Obsolete. ==  24/March/92 
*/
#define IEEE_UNTYPEDSIZE   0
#define IEEE_CHARSIZE      1
#define IEEE_INT16SIZE     2
#define IEEE_INT32SIZE     4
#define IEEE_FLOATSIZE     4

/*
* types used in defining a new field via a call to VSfdefine
*/

#define LOCAL_NOTYPE			0
#define LOCAL_CHARTYPE  	1  /* 8-bit ascii text stream */
#define LOCAL_INTTYPE 	 	2  /* 32-bit integers - don't use */
#define LOCAL_FLOATTYPE		3	/* as opposed to DOUBLE */ 
#define LOCAL_LONGTYPE 	 	4  /* 32-bit integers */
#define LOCAL_BYTETYPE 	 	5	/* 8-bit byte stream - unsupported */
#define LOCAL_SHORTTYPE 	6  /* 16-bit integers - unsupported */
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

/* kkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkk */

/* ------------------------------------------------------------------ */
/* 2 GLOBAL VARIABLES (int vjv and char sj[]) provide a simple
* debugging scheme. Debugging is turned on and off via calls to 
* setjj and setnojj. These globals and functions are found in vgp.c.
* 
* If the debug feature is no longer needed, delete all these, and
* all statements that refer to zj,sjs, and vjv from the source
*
*/


extern int16 	vjv; 		/* debugger switch */
extern char 	sjs[]; 		/* contains the debug/error message */

/* zj just prints out the contents of the text buffer sjs. */
/* sjs contains debugging messages when debug is on */
/* for the Mac, replace define zj to be any print msg routine */

#ifdef MAC
#define zj
#else
#define zj \
   {fprintf(stderr,"%s L-%d: %s",__FILE__,__LINE__,sjs); fflush(stderr); }
#endif

/* Macros for returning null, -1 or no value with a message */

#define RTNULL(ss) { sprintf(sjs,"@%s\n",ss); zj; return(NULL); }
#define RTNEG(ss) { sprintf(sjs,"@%s\n",ss); zj; return(FAIL); }
#define RT(ss) 	{ sprintf(sjs,"@%s\n",ss); zj; return; }

/* need to tune these to vertex boundary later.  Jason Ng 6-APR-92 */
#define VDEFAULTBLKSIZE    512
#define VDEFAULTNBLKS      8

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

/* this is a memory copy of a vs tag/ref found in the file */
typedef struct vg_instance_struct {
    uintn    ref;           /* ref # of this vgroup in the file */
	intn 	nattach;		/* # of current attachs to this vgroup */
	int32 	nentries;	/* # of entries in that vgroup initially */
	VGROUP 	*vg;			/* points to the vg when it is attached */
	struct vg_instance_struct * next;
}	vginstance_t; 

/* this is a memory copy of a vs tag/ref found in the file */
typedef struct vs_instance_struct {
    uintn    ref;           /* ref # of this vdata in the file */
	intn	nattach;		/* # of current attachs to this vdata */
        int32   nvertices;  /* # of elements in that vdata initially */
	VDATA	*vs; 			/* points to the vdata when it is attached */
	struct vs_instance_struct * next;
}	vsinstance_t; 

/* each vfile_t maintains 2 linked lists: one of vgs and one of vdatas
* that already exist or are just created for a given file.  */

typedef struct vfiledir_struct {

  int32			vgtabn;		/* # of vg entries in vgtab so far */
  vginstance_t	vgtab;			/* start of vg linked list */
  vginstance_t	*vgtabtail;	 	/* its tail end */
  
  int32			vstabn;		/* # of vs entries in vstab so far */
  vsinstance_t	vstab;		/* start of vs linked list */
  vsinstance_t	*vstabtail;		/* its tail end */
} vfile_t;

/*
 * NOTE:  People at large should not use this macro as they do not
 *        have access to vfile[]
 */
#define Get_vfile(f) (f > 0 ? (&vfile[(f & 0xffff)]) : NULL)

/* .................................................................. */
#define VSET_VERSION   3     /* DO NOT CHANGE!! */
#define VSET_OLD_TYPES 2     /* All version <= 2 use old type mappings */

#ifdef MAC
#include "vproto.h"
#include <String.h>
#endif
#include <ctype.h>

/**  Quincy, these are to be deletedma dn put into hfile.c

* DFopen and DFclose are redefined to perform additional work, 
* ie Vset data structure initialization and clean-up.
* Does not affect usage by other HDF interfaces.
*/

/* .................................................................. */

/* all these macros should be public for users */
/* macros for VSinquire */

#define VSQuerycount(vs, count) \
        (VSinquire ((VDATA *) vs, (int32 *) count, (int32*) NULL, (char*) NULL, (int32*) NULL, (char*) NULL))

#define VSQueryinterlace(vs, intr) \
        (VSinquire ((VDATA *) vs, (int32 *) NULL, (int32*) intr, (char*) NULL, (int32*) NULL, (char*) NULL))

#define VSQueryfields(vs, flds) \
        (VSinquire ((VDATA *) vs, (int32 *) NULL, (int32*) NULL, (char*) flds, (int32*) NULL, (char*) NULL))

#define VSQueryvsize(vs, size) \
        (VSinquire ((VDATA *) vs, (int32 *) NULL, (int32*) NULL, (char*) NULL, (int32*) size, (char*) NULL))

#define VSQueryname(vs, name) \
        (VSinquire ((VDATA *) vs, (int32 *) NULL, (int32*) NULL, (char*) NULL, (int32*) size, (char*) name))

/*
 *   Macros to provide fast access to the name and class of a Vset
 *     element.  The information returned is only guarenteed to be
 *     valid as long as the provided Vstructure is attached
 */

#define VGCLASS(vg) ((vg)->vgclass)
#define VGNAME(vg)  ((vg)->vgname)

#define VSCLASS(vs) ((vs)->vsclass)
#define VSNAME(vs)  ((vs)->vsname)

#include "vproto.h"

#define Vinitialize(f)     Vstart((f))
#define Vfinish(f)         Vend((f))

#endif /* _VG_H */

