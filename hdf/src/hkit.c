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

#ifdef RCSID
static char RcsId[] = "@(#)$Revision$";
#endif

/* $Id$ */

#include <ctype.h>
#include "hdf.h"
#include "hfile.h"
#include "hkit.h"


/* ------------------------------- HDc2fstr ------------------------------- */
/*

 NAME
	HDc2fstr -- convert a C string into a Fortran string IN PLACE
 USAGE
	intn HDc2fstr(str, len)
        char * str;       IN: string to convert
        intn   len;       IN: length of Fortran string
 RETURNS
        SUCCEED
 DESCRIPTION
        Change a C string (NULL terminated) into a Fortran string.
        Basically, all that is done is that the NULL is ripped out
        and the string is padded with spaces

--------------------------------------------------------------------------- */ 
intn HDc2fstr(char *str, intn len)
{
    int i;

    for(i=0; (str[i]); i++)
        /* EMPTY */;
    for(; i<len; i++) str[i] = ' ';
    return SUCCEED;
} /* HDc2fstr */


/* ----------------------------- HDf2sstring ------------------------------ */
/*

 NAME
	HDf2sstring -- convert a Fortran string to a C string
 USAGE
	char * HDf2cstring(fdesc, len)
        _fcd  fdesc;     IN: Fortran string descriptor
        intn  len;       IN: length of Fortran string
 RETURNS
        SUCCEED
 DESCRIPTION
        Chop off trailing blanks off of a Fortran string and
        move it into a newly allocated C string.  It is up
        to the user to free this string.

--------------------------------------------------------------------------- */ 
char _HUGE *HDf2cstring(_fcd fdesc, intn len)
{
    char *cstr, *str;
    int i;

    str = _fcdtocp(fdesc);
    for(i=len-1;i>=0 && (!isascii(str[i]) || !isgraph(str[i])); i--)
        /*EMPTY*/;
    cstr = (char *)HDgetspace((uint32)(i+2));
    cstr[i+1] = '\0';
    for (; i>=0; i--) cstr[i] = str[i];
    return cstr;
} /* HDf2cstring */


/* ----------------------------- HIlookup_dd ------------------------------ */
/*

 NAME
	HIlookup_dd -- find the dd record for an element
 USAGE
	int HIlookup_dd(file_rec, tag, ref, block, idx)
        filerec_t *  file_rec;       IN:  file record to search
        uint16       tag;            IN:  tag of element to find
        uint16       ref;            IN:  ref of element to find
        ddblock_t ** block;          OUT: block element is in
        int32     *  idx;            OUT: element's index in block
 RETURNS
        SUCCEED / FAIL
 DESCRIPTION
        find the dd with tag and ref, by returning the block 
        where the dd resides and the index of the dd in the 
        ddblock ddlist.

        This function is different from HIfind_dd in that it 
        does not understand any ordering in the file.  Wildcards 
        sent to this routine (i.e. to get the 'next' widget 
        get passed off to HIfind_dd().

--------------------------------------------------------------------------- */
int HIlookup_dd(filerec_t *file_rec, uint16 look_tag, uint16 look_ref, 
                ddblock_t **pblock, int32 *pidx)
{ 
    CONSTR(FUNC,"HIlookup_dd");       /* for HERROR */
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

 NAME
	HIadd_hash_dd -- add a dd to the hash table
 USAGE	
        int HIadd_hash_dd(file_rec, tag, ref, block, idx)
        filerec_t  * file_rec;       IN:  file record
        uint16       tag;            IN:  tag of element to add
        uint16       ref;            IN:  ref of element to add
        ddblock_t  * block;          OUT: block element is in
        int32        idx;            OUT: element's index in block
 RETURNS
        SUCCEED / FAIL
 DESCRIPTION
        Stick an new element into the file's hash table.  The hash table 
        is keyed on the low order bits of (tag + ref) and gives a quick
        means of mapping tag and ref to the DD record for this element

--------------------------------------------------------------------------- */
int HIadd_hash_dd(filerec_t *file_rec, uint16 look_tag, uint16 look_ref, 
                ddblock_t *pblock, int32 pidx)
{
    CONSTR(FUNC,"HIadd_hash_dd");       /* for HERROR */
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

 NAME
	HIdel_hash_dd -- remove a dd from the hash table
 USAGE	
        int HIdel_hash_dd(file_rec, tag, ref)
        filerec_t  * file_rec;       IN:  file record
        uint16       tag;            IN:  tag of element to delete
        uint16       ref;            IN:  ref of element to delete
 RETURNS
        SUCCEED
 DESCRIPTION
        Remove an element from the hash table.  Return succeed even
        if the element does not exist in the table
 
--------------------------------------------------------------------------- */
int HIdel_hash_dd(filerec_t *file_rec, uint16 look_tag, uint16 look_ref)
{
  CONSTR(FUNC,"HIdel_hash_dd");       /* for HERROR */
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



/* ----------------------------- HIfind_dd ------------------------------ */
/*

 NAME
	HIfind_dd -- find the dd record for an element
 USAGE
	int HIfind_dd(file_rec, tag, ref, block, idx, direction)
        filerec_t *  file_rec;       IN:  file record to search
        uint16       tag;            IN:  tag of element to find
        uint16       ref;            IN:  ref of element to find
        ddblock_t ** block;          IN:  block to start search
                                     OUT: block element is in
        int32     *  idx;            IN:  index to start search
                                     OUT: element's index in block
        intn         direction;      IN:  direction to search 
                                          (DF_FORWARD / DF_BACKWARD)
 RETURNS
        SUCCEED / FAIL
 DESCRIPTION
        find the dd with tag and ref, by returning the block 
        where the dd resides and the index of the dd in the 
        ddblock ddlist.  This is a more powerful, but slower 
        version of HIlookup_dd()

        By setting the direction and giving a starting block and
        index we can handle searches like "give me the next widget"
        or "give me the previous thing with ref = 3"

--------------------------------------------------------------------------- */
int HIfind_dd(uint16 look_tag, uint16 look_ref, ddblock_t **pblock, int32 *pidx,
    intn direction)
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

 NAME
	HDflush -- flush the HDF file
 USAGE
	intn HDflush(fid)
        int32 fid;            IN: file ID
 RETURNS
        SUCCEED / FAIL
 DESCRIPTION
        Force the system to flush the HDF file stream

        This should be primarily used for debugging
        
        The MAC does not really support fflush() so this r
        outine just returns SUCCEED always on a MAC w/o 
        really doing anything.

--------------------------------------------------------------------------- */
intn HDflush(int32 file_id)
{
    CONSTR(FUNC,"HDflush");       /* for HERROR */

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

 NAME
	HDpackFstring -- convert a C string into a Fortran string
 USAGE
	intn HDpackFstring(src, dest, len)
        char * src;          IN:  source string
        char * dest;         OUT: destination
        intn   len;          IN:  length of string
 RETURNS
        SUCCEED / FAIL
 DESCRIPTION
        given a NULL terminated C string 'src' convert it to
        a space padded Fortran string 'dest' of length 'len'

        This is very similar to HDc2fstr except that function does
        it in place and this one copies.  We should probably only
        support one of these.

--------------------------------------------------------------------------- */
intn HDpackFstring(char *src, char *dest, intn len)
{

    intn sofar;
    
    for(sofar = 0; (sofar < len) && (*src != '\0'); sofar++)
        *dest++ = *src++;

    while(sofar++ < len)
        *dest++ = ' ';

    return SUCCEED;

} /* HDpackFstring */


/* ----------------------------- HDgettagname ----------------------------- */
/*

 NAME
	HDgettagname -- return a text description of a tag
 USAGE
	char * HDgettagname(tag)
        uint16   tag;          IN: tag of element to find
 RETURNS
        Descriptive text or NULL
 DESCRIPTION
        Map a tag to a statically allocated text description of it.

--------------------------------------------------------------------------- */
const char _HUGE *HDgettagname(uint16 tag)
{
    intn i;

    for(i=0; i<sizeof(tag_descriptions)/sizeof(tag_descript_t); i++)
	if(tag_descriptions[i].tag==tag)
	    return(tag_descriptions[i].desc);
    return(NULL);
}
