#ifdef RCSID
static char RcsId[] = "@(#)$Revision$";
#endif
/*
$Header$

$Log$
Revision 1.14  1993/09/21 20:21:05  georgev
Changed  DFTAG_SDD tag.

 * Revision 1.13  1993/08/16  21:46:05  koziol
 * Wrapped in changes for final, working version on the PC.
 *
 * Revision 1.12  1993/07/01  20:08:09  chouck
 * Made the hash table use fewer malloc() and free() pairs to improve
 * efficiency and (hopefully) reduce PC memory fragmentation.
 *
 * Revision 1.11  1993/04/22  20:24:22  koziol
 * Added new Hfind() routine to hfile.c which duplicates older DFsetfind/DFfind
 * utility...
 *
 * Revision 1.9  1993/04/14  21:39:22  georgev
 * Had to add some VOIDP casts to some functions to make the compiler happy.
 *
 * Revision 1.8  1993/04/05  22:35:51  koziol
 * Fixed goofups made in haste when patching code.
 *
 * Revision 1.7  1993/03/29  17:21:03  chouck
 * Fixed a prototype problem
 *
 * Revision 1.6  1993/01/26  19:42:48  koziol
 * Added support for reading and writing Little-Endian data on all
 * platforms.  This has been tested on: Cray, Sun, and PCs so far.
 *
 * Revision 1.5  1993/01/19  05:56:03  koziol
 * Merged Hyperslab and JPEG routines with beginning of DEC ALPHA
 * port.  Lots of minor annoyances fixed.
 *
 * Revision 1.4  1992/11/05  18:59:26  chouck
 * Added (unix) wrapper to realloc()
 *
 * Revision 1.3  1992/11/02  16:35:41  koziol
 * Updates from 3.2r2 -> 3.3
 *
 * Revision 1.2  1992/08/26  19:44:25  chouck
 * Moved HDgettagname() into hkit.c and added calibration tag
 *
 * Revision 1.1  1992/08/25  21:40:44  koziol
 * Initial revision
 *
*/
#include <ctype.h>
#include "hdf.h"
#include "herr.h"
#include "hfile.h"

#if defined PROTOTYPE
intn HDc2fstr(char *str, intn len)
#else
intn HDc2fstr(str, len)
char* str;
intn len;
#endif /* PROTOTYPE */
{
    int i;

    for(i=0; (str[i]); i++)
        /* EMPTY */;
    for(; i<len; i++) str[i] = ' ';
    return 0;
}

#if defined PROTOTYPE
char _HUGE *HDf2cstring(_fcd fdesc, intn len)
#else
char _HUGE *HDf2cstring(fdesc, len)
    _fcd fdesc;
    intn len;
#endif /* PROTOTYPE */
{
    char *cstr, *str;
    int i;

    str = _fcdtocp(fdesc);
    for(i=len-1;i>=0 && (!isascii(str[i]) || !isgraph(str[i])); i--)
        /*EMPTY*/;
    cstr = (char *)HDgetspace(i+2);
    cstr[i+1] = '\0';
    for (; i>=0; i--) cstr[i] = str[i];
    return cstr;
}

/*--------------------------------------------------------------------------
 HIlookup_dd

 find the dd with tag and ref, by returning the block where the dd resides
 and the index of the dd in the ddblock ddlist.

 This function is different from HIfind_dd in that it does not understand
 any ordering in the file.  Wildcards sent to this routine (i.e. to
 get the 'next' widget get passed off to HIfind_dd().

 Return FAIL or SUCCEED
--------------------------------------------------------------------------*/
#ifdef PROTOTYPE
int HIlookup_dd(filerec_t *file_rec, uint16 look_tag, uint16 look_ref, 
                ddblock_t **pblock, int32 *pidx)
#else
int HIlookup_dd(file_rec, look_tag, look_ref, pblock, pidx)
     filerec_t *file_rec;       /* the file */
     uint16 look_tag;           /* tag of dd to look for */
     uint16 look_ref;           /* ref of dd to look for */
     ddblock_t **pblock;        /* OUT: ddblock where dd is found */
     int32 *pidx;               /* OUT: index into ddlist where dd is found */
#endif
{ 
  char *FUNC="HIlookup_dd";       /* for HERROR */
  register intn tag, ref, key, i;
  register tag_ref_list_ptr p;

  if(look_tag == DFTAG_WILDCARD || look_ref == DFREF_WILDCARD)
    return (HIfind_dd(look_tag, look_ref, pblock, pidx, DF_FORWARD));

  tag = (intn) look_tag;
  ref = (intn) look_ref;

  /*
   * Look for the normal version 
   */
  key = tag + ref;

  for(p = file_rec->hash[key & HASH_MASK]; p; p = p->next) {
      for(i = 0; i < p->count; i++) {
          if(p->objects[i].tag == tag && p->objects[i].ref == ref) {
              *pblock = p->objects[i].pblock;
              *pidx   = p->objects[i].pidx;
              return SUCCEED;
          }
      }
  }

  /*
   * Try looking for the special version of this tag
   */
  tag = (intn) MKSPECIALTAG(look_tag);
  key = tag + ref;

  for(p = file_rec->hash[key & HASH_MASK]; p; p = p->next) {
      for(i = 0; i < p->count; i++) {
          if(p->objects[i].tag == tag && p->objects[i].ref == ref) {
              *pblock = p->objects[i].pblock;
              *pidx   = p->objects[i].pidx;
              return SUCCEED;
          }
      }
  }

  return FAIL;

} /* HIlookup_dd */

/* ---------------------------- HIadd_hash_dd ----------------------------- */
/*
  Add a new dd into the hash table

  Return SUCCEED or FAIL
*/
#ifdef PROTOTYPE
int HIadd_hash_dd(filerec_t *file_rec, uint16 look_tag, uint16 look_ref, 
                ddblock_t *pblock, int32 pidx)
#else
int HIadd_hash_dd(file_rec, look_tag, look_ref, pblock, pidx)
     filerec_t *file_rec;      /* the file */
     uint16 look_tag;           /* tag of dd to add */
     uint16 look_ref;           /* ref of dd to add */
     ddblock_t *pblock;         /* ddblock where dd is  */
     int32 pidx;                /* index into ddlist where dd is */
#endif
{
    char *FUNC="HIadd_hash_dd";       /* for HERROR */
    register intn tag, ref, key, i;
    register tag_ref_list_ptr p, where;

    if(look_tag == DFTAG_NULL)
        return SUCCEED;

    tag = (intn) look_tag;
    ref = (intn) look_ref;
    key = tag + ref;

    where = file_rec->hash[key & HASH_MASK];

    if(where && where->count < HASH_BLOCK_SIZE) {
        
        i = where->count++;

        where->objects[i].pblock = pblock;
        where->objects[i].pidx   = pidx;
        where->objects[i].tag    = tag;
        where->objects[i].ref    = ref;

    } else {

        if((p = (tag_ref_list_ptr) HDgetspace((uint32)sizeof(tag_ref_list)))==NULL)
            HRETURN_ERROR(DFE_NOSPACE, FAIL);
        
        p->objects[0].pblock = pblock;
        p->objects[0].pidx   = pidx;
        p->objects[0].tag    = tag;
        p->objects[0].ref    = ref;

        p->next   = where;
        p->count  = 1;
        file_rec->hash[key & HASH_MASK] = p;
  
    }

    return SUCCEED;
} /* HIadd_hash_dd */


/* ---------------------------- HIdel_hash_dd ----------------------------- */
/*
  Delete a dd from the hash table

  Return SUCCEED or FAIL
*/
#ifdef PROTOTYPE
int HIdel_hash_dd(filerec_t *file_rec, uint16 look_tag, uint16 look_ref)
#else
int HIdel_hash_dd(file_rec, look_tag, look_ref)
     filerec_t *file_rec;      /* the file */
     uint16 look_tag;           /* tag of dd to add */
     uint16 look_ref;           /* ref of dd to add */
#endif
{
  char *FUNC="HIdel_hash_dd";       /* for HERROR */
  register intn tag, ref, key, i;
  register tag_ref_list_ptr p;

  tag = (intn) look_tag;
  ref = (intn) look_ref;
  key = tag + ref;
  
  p = file_rec->hash[key & HASH_MASK]; 

  if(!p) return SUCCEED;

  for(p = file_rec->hash[key & HASH_MASK]; p; p = p->next) {
      for(i = 0; i < p->count; i++) {
          if(p->objects[i].tag == tag && p->objects[i].ref == ref) {
              p->objects[i].tag = DFTAG_NULL;
              return SUCCEED;
          }
      }
  }
  
  return SUCCEED;
  
} /* HIdel_hash_dd */


/*--------------------------------------------------------------------------
 HIfind_dd

 find the dd with tag and ref, by returning the block where the dd resides
 and the index of the dd in the ddblock ddlist.
 Revised to go either forward or backward through the DD list. (4/21/93 QAK)
--------------------------------------------------------------------------*/
#ifdef PROTOTYPE
int HIfind_dd(uint16 look_tag, uint16 look_ref, ddblock_t **pblock, int32 *pidx,
    intn direction)
#else
int HIfind_dd(look_tag, look_ref, pblock, pidx, direction)
    uint16 look_tag;           /* tag of dd to look for */
    uint16 look_ref;           /* ref of dd to look for */
    ddblock_t **pblock;        /* IN: ddblock to start looking for the dd */
                               /* OUT: ddblock where dd is found */
    int32 *pidx;               /* IN: index before place in ddlist
                                  to start searching */
                               /* OUT: index into ddlist where dd is found */
    intn direction;            /* IN : Direction to search the DD list in */
#endif
{
    register intn idx;          /* index into ddlist of current dd searched */
    register ddblock_t *block;  /* ptr to current ddblock searched */
    register dd_t *list;        /* ptr to current ddlist searched */

#ifndef oldspecial
    uint16 special_tag;                /* corresponding special tag */

    /* search for special version also */

    special_tag = MKSPECIALTAG(look_tag);
#endif

    if(direction==DF_FORWARD) {     /* search forward through the DD list */
        /* start searching on the next dd */
        idx = (intn)*pidx + 1;
        for (block = *pblock; block; block = block->next) {

            list = block->ddlist;
            for (; idx < block->ndds; idx++) {

               /* skip the empty dd's */
                if (list[idx].tag == DFTAG_NULL && look_tag != DFTAG_NULL)
                    continue;

                if(((look_tag == DFTAG_WILDCARD || list[idx].tag == look_tag)
#ifndef oldspecial
                        || (special_tag != DFTAG_NULL &&
                        list[idx].tag == special_tag)
#endif
                        ) && (look_ref == DFREF_WILDCARD ||
                        list[idx].ref == look_ref)) {

                    /* we have a match !! */
                    *pblock = block;
                    *pidx = idx;
                    return(SUCCEED);
                 }  /* end if */
              } /* end for */

            /* start from beginning of the next dd list */
            idx = 0;
          } /* end for */
      } /* end if */
    else if(direction==DF_BACKWARD) {   /* search backward through the DD list */
        /* start searching on the previous dd */
        idx = (intn)*pidx - 1;
        for (block = *pblock; block; ) {

            list = block->ddlist;
            for (; idx >= 0; idx--) {

               /* skip the empty dd's */
                if (list[idx].tag == DFTAG_NULL && look_tag != DFTAG_NULL)
                    continue;

                if(((look_tag == DFTAG_WILDCARD || list[idx].tag == look_tag)
#ifndef oldspecial
                        || (special_tag != DFTAG_NULL &&
                        list[idx].tag == special_tag)
#endif
                        ) && (look_ref == DFREF_WILDCARD ||
                        list[idx].ref == look_ref)) {

                    /* we have a match !! */
                    *pblock = block;
                    *pidx = idx;
                    return(SUCCEED);
                 }  /* end if */
              } /* end for */

            /* start from beginning of the next dd list */
            block = block->prev;
            if(block!=NULL)
                idx = block->ndds -1;
          } /* end for */
      } /* end if */

    /* nothing found or bad direction */

    return(FAIL);
} /* HIfind_dd */

/* ------------------------------- HDflush -------------------------------- */
/*

  Force the system to flush the HDF file stream

  This should be primarily used for debugging

  The MAC does not really support fflush() so this routine just returns
	SUCCEED always on a MAC w/o really doing anything.

*/

#ifdef PROTOTYPE
intn HDflush(int32 file_id)
#else
intn HDflush(file_id)
    int32 file_id;             /* id of file to flush */
#endif
{
    char *FUNC="HDflush";       /* for HERROR */

#ifndef MAC

    filerec_t *file_rec;
    
    file_rec = FID2REC(file_id);
    if (!file_rec || file_rec->refcount == 0) {
        HERROR(DFE_ARGS);
        return FAIL;
    }
    
    fflush(file_rec->file);

#endif /* MAC */

    return SUCCEED;

} /* HDflush */


/* ---------------------------- HDpackFstring ----------------------------- */
/*

  HDpackFstring -- given a NULL terminated C string 'src' convert it to
  a space padded Fortran string 'dest' of length 'len'

*/
intn
#ifdef PROTOTYPE
HDpackFstring(char *src, char *dest, intn len)
#else
HDpackFstring(src, dest, len)
char *src, *dest;
intn len;
#endif
{

    intn sofar;
    
    for(sofar = 0; (sofar < len) && (*src != '\0'); sofar++)
        *dest++ = *src++;

    while(sofar++ < len)
        *dest++ = ' ';

    return SUCCEED;

}


/*--------------------------------------------------------------------------
 *  HDgettagname(tag) : map a tag to its corresponding name
 *                      return NULL if tag is unknown.
 *
 *  NOTE: Please keep tag names <= 30 characters - a 
 *        lot of pretty-printing code depends on it.
--------------------------------------------------------------------------*/
#ifdef PROTOTYPE
char _HUGE *HDgettagname(uint16 tag)
#else
char _HUGE *HDgettagname(tag)
     uint16 tag;
#endif /* PROTOTYPE */
{

  char *name;

  switch(tag) {
      
      /* Utility Tags */
  case DFTAG_NULL  :
      name = "No Data"; break;
  case DFTAG_VERSION :
      name = "Version Descriptor"; break;
  case DFTAG_LINKED :
      name = "Linked Blocks Indicator"; break;
  case DFTAG_FID   : 
      name = "File Identifier"; break;
  case DFTAG_FD    :   
      name = "File Description"; break;
  case DFTAG_TID   :
      name = "Tag Identifier"; break;
  case DFTAG_TD    : 
      name = "Tag Description"; break;
  case DFTAG_DIL   :
      name = "Data Id Label"; break;
  case DFTAG_DIA   :    
      name = "Data Id Annotation"; break;
  case DFTAG_NT    :    
      name = "Number type"; break;
  case DFTAG_MT    :   
      name = "Machine type"; break;
      
      /* raster-8 Tags */
  case DFTAG_ID8   :   
      name = "Image Dimensions-8"; break;
  case DFTAG_IP8   :   
      name = "Image Palette-8"; break;
  case DFTAG_RI8   :  
      name = "Raster Image-8"; break;
  case DFTAG_CI8   : 
      name = "RLE Compressed Image-8"; break;
  case DFTAG_II8   :  
      name = "Imcomp Image-8"; break;
      
      /* Raster Image Tags */
  case DFTAG_ID    :  
      name = "Image Dimensions"; break;
  case DFTAG_LUT   :  
      name = "Image Palette"; break;
  case DFTAG_RI    : 
      name = "Raster Image Data"; break;
  case DFTAG_CI    :  
      name = "Compressed Image"; break;
  case DFTAG_RIG   : 
      name = "Raster Image Group"; break;
  case DFTAG_LD    : 
      name = "Palette Dimension"; break;
  case DFTAG_MD    :
      name = "Matte Dimension"; break;
  case DFTAG_MA    :
      name = "Matte Data"; break;
  case DFTAG_CCN   :   
      name = "Color Correction"; break;
  case DFTAG_CFM   : 
      name = "Color Format"; break;
  case DFTAG_AR    :   
      name = "Aspect Ratio"; break;
  case DFTAG_DRAW  :
      name = "Sequenced images"; break;
  case DFTAG_RUN   :   
      name = "Runable program / script"; break;
  case DFTAG_XYP   : 
      name = "X-Y position"; break;
  case DFTAG_MTO   :  
      name = "M/c-Type override"; break;
      
      /* Tektronix */
  case DFTAG_T14   :   
      name = "TEK 4014 Data"; break;
  case DFTAG_T105  :
      name = "TEK 4105 data"; break;
      
      /* Compression Schemes */
  case DFTAG_RLE   : 
      name = "Run Length Encoding"; break;
  case DFTAG_IMCOMP : 
      name = "IMCOMP Encoding"; break;
  case DFTAG_JPEG :
      name = "24-bit JPEG Encoding"; break;
  case DFTAG_GREYJPEG :
      name = "8-bit JPEG Encoding"; break;
      
      /* Scientific / Numeric Data Sets */
  case DFTAG_SDG   : 
      name = "Scientific Data Group"; break;
  case DFTAG_NDG   : 
      name = "Numeric Data Group"; break;
  case DFTAG_SD    :
      name = "Scientific Data"; break;
  case DFTAG_SDD   : 
    name = "SciData dimension record"; break;
  case DFTAG_SDL   :   
      name = "SciData labels"; break;
  case DFTAG_SDU   : 
      name = "SciData units"; break;
  case DFTAG_SDF   :  
      name = "SciData formats"; break;
  case DFTAG_SDS   :  
      name = "SciData scales"; break;
  case DFTAG_SDM   :  
      name = "SciData max/min"; break;
  case DFTAG_SDC   :  
      name = "SciData coordsys"; break;
  case DFTAG_SDT   :  
      name = "Transpose"; break;
  case DFTAG_SDLNK :  
      name = "Links related to the dataset"; break;
  case DFTAG_CAL   :  
      name = "Calibration information"; break;
      
      /* V Group Tags */
  case DFTAG_VG   :  
      name = "Vgroup"; break;
  case DFTAG_VH   : 
      name = "Vdata"; break;
  case DFTAG_VS   : 
      name = "Vdata Storage"; break;
  default:
      name = (char *) NULL;
      break;
  }
  
  return name;
  
}
