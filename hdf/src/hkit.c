#ifdef RCSID
static char RcsId[] = "@(#)$Revision$";
#endif

/* $Id$ */

#include <ctype.h>
#include "hdf.h"
#include "herr.h"
#include "hfile.h"
#include "hkit.h"

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

    uint16 special_tag;                /* corresponding special tag */

    /* search for special version also */
    special_tag = MKSPECIALTAG(look_tag);

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
                        || (special_tag != DFTAG_NULL
                        && list[idx].tag == special_tag))
                        && (look_ref == DFREF_WILDCARD
                        || list[idx].ref == look_ref)) {

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
                        || (special_tag != DFTAG_NULL
                        && list[idx].tag == special_tag))
                        && (look_ref == DFREF_WILDCARD
                        || list[idx].ref == look_ref)) {

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
--------------------------------------------------------------------------*/
#ifdef PROTOTYPE
char _HUGE *HDgettagname(uint16 tag)
#else
char _HUGE *HDgettagname(tag)
     uint16 tag;
#endif /* PROTOTYPE */
{
    intn i;

    for(i=0; i<sizeof(tag_descriptions)/sizeof(tag_descript_t); i++)
	if(tag_descriptions[i].tag==tag)
	    return(tag_descriptions[i].desc);
    return(NULL);
}
