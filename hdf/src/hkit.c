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

/*
LOCAL ROUTINES
  None
EXPORTED ROUTINES
  HDc2fstr      -- convert a C string into a Fortran string IN PLACE
  HDf2sstring   -- convert a Fortran string to a C string
  HIlookup_dd   -- find the dd record for an element
  HIadd_hash_dd -- add a dd to the hash table
  HIdel_hash_dd -- remove a dd from the hash table
  HIfind_dd     -- find the dd record for an element
  HIcount_dd    -- counts the dd's of a certain type in file
  HDflush       -- flush the HDF file
  HDpackFstring -- convert a C string into a Fortran string
  HDgettagdesc  -- return a text description of a tag
  HDgettagsname -- return a text name of a tag
  HDgettagnum   -- return the tag number for a text description of a tag
  HDgetNTdesc   -- return a text description of a number-type
  HDfidtoname   -- return the filename the file ID corresponds to
*/

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

---------------------------------------------------------------------------*/
intn 
HDc2fstr(char *str, intn len)
{
    int         i;

#ifdef OLD_WAY
    for(i=0; (str[i]); i++)
        /* EMPTY */;
#else /* OLD_WAY */
    i=HDstrlen(str)+1;
#endif /* OLD_WAY */
    for (; i < len; i++)
        str[i] = ' ';
    return SUCCEED;
}   /* HDc2fstr */

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

---------------------------------------------------------------------------*/
char _HUGE *
HDf2cstring(_fcd fdesc, intn len)
{
    char       *cstr, *str;
    int         i;

    str = _fcdtocp(fdesc);
#ifdef OLD_WAY
    for (i = len - 1; i >= 0 && ((str[i] & 0x80) || !isgraph(str[i])); i--)
        /*EMPTY */ ;
#else /* OLD_WAY */
    /* This should be equivalent to the above test -QAK */
    for(i=len-1; i>=0 && !isgraph(str[i]); i--)
        /*EMPTY*/;
#endif /* OLD_WAY */
    cstr = (char *) HDmalloc((uint32) (i + 2));
    cstr[i + 1] = '\0';
#ifdef OLD_WAY
    for (; i >= 0; i--)
        cstr[i] = str[i];
#else /* OLD_WAY */
    HDmemcpy(cstr,str,i+1);
#endif /* OLD_WAY */
    return cstr;
}   /* HDf2cstring */

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

---------------------------------------------------------------------------*/
int 
HIlookup_dd(filerec_t * file_rec, uint16 look_tag, uint16 look_ref,
            ddblock_t ** pblock, int32 *pidx)
{
#ifdef LATER
    CONSTR(FUNC, "HIlookup_dd");    /* for HERROR */
#endif
    intn tag, ref, key, i;
    tag_ref_list_ptr p;
    tag_ref_ptr o_ptr;

    if (look_tag == DFTAG_WILDCARD || look_ref == DFREF_WILDCARD)
        return (HIfind_dd(look_tag, look_ref, pblock, pidx, DF_FORWARD));

    tag = (intn) look_tag;
    ref = (intn) look_ref;

    /*
     * Look for the normal version
     */
    key = tag + ref;

    for (p = file_rec->hash[key & HASH_MASK]; p; p = p->next)
      {
          for (i = 0, o_ptr = &p->objects[0]; i < p->count; i++, o_ptr++)
            {
                if (o_ptr->tag == tag && o_ptr->ref == ref)
                  {
                      *pblock = o_ptr->pblock;
                      *pidx = o_ptr->pidx;
                      return SUCCEED;
                  }
            }
      }

    /* Try looking for the special version of this tag */
    tag = (intn) MKSPECIALTAG(look_tag);
    key = tag + ref;

    for (p = file_rec->hash[key & HASH_MASK]; p; p = p->next)
      {
          for (i = 0, o_ptr = &p->objects[0]; i < p->count; i++, o_ptr++)
            {
                if (o_ptr->tag == tag && o_ptr->ref == ref)
                  {
                      *pblock = o_ptr->pblock;
                      *pidx = o_ptr->pidx;
                      return SUCCEED;
                  }
            }
      }

    return FAIL;
}	/* HIlookup_dd */

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

---------------------------------------------------------------------------*/
int 
HIadd_hash_dd(filerec_t * file_rec, uint16 look_tag, uint16 look_ref,
              ddblock_t * pblock, int32 pidx)
{
    CONSTR(FUNC, "HIadd_hash_dd");  /* for HERROR */
    intn tag, ref, key, i;
    tag_ref_list_ptr p, where;

    if (look_tag == DFTAG_NULL)
        return SUCCEED;

    tag = (intn) look_tag;
    ref = (intn) look_ref;
    key = tag + ref;

    where = file_rec->hash[key & HASH_MASK];

    if (where && where->count < HASH_BLOCK_SIZE)
      {

          i = where->count++;

          where->objects[i].pblock = pblock;
          where->objects[i].pidx = pidx;
          where->objects[i].tag = tag;
          where->objects[i].ref = ref;

      }
    else
      {

          if ((p = (tag_ref_list_ptr) HDmalloc((uint32) sizeof(tag_ref_list))) == NULL)
              HRETURN_ERROR(DFE_NOSPACE, FAIL);

          p->objects[0].pblock = pblock;
          p->objects[0].pidx = pidx;
          p->objects[0].tag = tag;
          p->objects[0].ref = ref;

          p->next = where;
          p->count = 1;
          file_rec->hash[key & HASH_MASK] = p;

      }

    return SUCCEED;
}   /* HIadd_hash_dd */

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

---------------------------------------------------------------------------*/
int 
HIdel_hash_dd(filerec_t * file_rec, uint16 look_tag, uint16 look_ref)
{
#ifdef LATER
    CONSTR(FUNC, "HIdel_hash_dd");  /* for HERROR */
#endif
    intn tag, ref, key, i;
    tag_ref_list_ptr p;

    tag = (intn) look_tag;
    ref = (intn) look_ref;
    key = tag + ref;

    p = file_rec->hash[key & HASH_MASK];

    if (!p)
        return SUCCEED;

    for (p = file_rec->hash[key & HASH_MASK]; p; p = p->next)
      {
          for (i = 0; i < p->count; i++)
            {
                if (p->objects[i].tag == tag && p->objects[i].ref == ref)
                  {
                      p->objects[i].tag = DFTAG_NULL;
                      return SUCCEED;
                  }
            }
      }

    return SUCCEED;
}	/* HIdel_hash_dd */

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

---------------------------------------------------------------------------*/
int 
HIfind_dd(uint16 look_tag, uint16 look_ref, ddblock_t ** pblock, int32 *pidx,
          intn direction)
{
    intn idx;          /* index into ddlist of current dd searched */
    ddblock_t *block;  /* ptr to current ddblock searched */
    dd_t *list;        /* ptr to current ddlist searched */

    uint16      special_tag;    /* corresponding special tag */

    /* search for special version also */
    special_tag = MKSPECIALTAG(look_tag);

    if (direction == DF_FORWARD)
      {   /* search forward through the DD list */
	  /* start searching on the next dd */
	  idx = (intn) *pidx + 1;
	  for (block = *pblock; block; block = block->next)
	    {
		list = block->ddlist;
		for (; idx < block->ndds; idx++)
		  {
		      /* skip the empty dd's */
		      if (list[idx].tag == DFTAG_NULL && look_tag != DFTAG_NULL)
			  continue;

		      if (((look_tag == DFTAG_WILDCARD || list[idx].tag == look_tag)
			   || (special_tag != DFTAG_NULL
			       && list[idx].tag == special_tag))
			  && (look_ref == DFREF_WILDCARD
			      || list[idx].ref == look_ref))
			{
			    /* we have a match !! */
			    *pblock = block;
			    *pidx = idx;
			    return (SUCCEED);
			}	/* end if */
		  }	/* end for */

		/* start from beginning of the next dd list */
		idx = 0;
	    }	/* end for */
      }		/* end if */
    else if (direction == DF_BACKWARD)
      {	  /* search backward through the DD list */
	  /* start searching on the previous dd */
	  idx = (intn) *pidx - 1;
	  for (block = *pblock; block;)
	    {
		list = block->ddlist;
		for (; idx >= 0; idx--)
		  {
		      /* skip the empty dd's */
		      if (list[idx].tag == DFTAG_NULL && look_tag != DFTAG_NULL)
			  continue;

		      if (((look_tag == DFTAG_WILDCARD || list[idx].tag == look_tag)
			   || (special_tag != DFTAG_NULL
			       && list[idx].tag == special_tag))
			  && (look_ref == DFREF_WILDCARD
			      || list[idx].ref == look_ref))
			{

			    /* we have a match !! */
			    *pblock = block;
			    *pidx = idx;
			    return (SUCCEED);
			}	/* end if */
		  }	/* end for */

		/* start from beginning of the next dd list */
		block = block->prev;
		if (block != NULL)
		    idx = block->ndds - 1;
	    }	/* end for */
      }		/* end if */

    /* nothing found or bad direction */
    return (FAIL);
}	/* HIfind_dd */

/* ----------------------------- HIcount_dd ------------------------------ */
/*
NAME
   HIcount_dd -- counts the dd's of a certain type in file
USAGE
   intn HIcount_dd(file_rec, tag, ref, all_cnt, real_cnt)
   filerec_t *  file_rec;       IN:  file record to search
   uint16       tag;            IN:  tag of element to find
                                     (can be DFTAG_WILDCARD)
   uint16       ref;            IN:  ref of element to find
                                     (can be DFREF_WILDCARD)
   uintn       *all_cnt;        OUT: Count of all the tag/ref pairs
                                     found, including DFTAG_NULL and
                                     DFTAG_FREE
   uintn       *real_cnt;       OUT: Count of all the tag/ref pairs
                                     found, excluding DFTAG_NULL and 
                                     DFTAG_FREE
RETURNS
   SUCCEED / FAIL
DESCRIPTION
   Counts the number of tag/ref pairs in a file.

   This routine keeps track of and returns to the user the number
   of all tag/refs and the number of "real" tag/refs found.
   "Real" tag/refs are any except DFTAG_NULL & DFTAG_FREE.

   This routine always counts the total tag/refs in the file, no
   provision is made for partial searches.

---------------------------------------------------------------------------*/
intn 
HIcount_dd(filerec_t * file_rec, uint16 cnt_tag, uint16 cnt_ref,
           uintn *all_cnt, uintn *real_cnt)
{
    uintn       t_all_cnt = 0;  /* count of all tag/refs found */
    uintn       t_real_cnt = 0; /* count of all tag/refs except NULL & FREE */
    intn        idx;            /* index into ddlist of current dd searched */
    ddblock_t  *block;          /* ptr to current ddblock searched */
    dd_t       *list;           /* ptr to current ddlist searched */
    uint16      special_tag;    /* corresponding special tag */

    /* search for special version also */
    special_tag = MKSPECIALTAG(cnt_tag);

    for (block = file_rec->ddhead; block != NULL; block = block->next)
      {
          t_all_cnt += block->ndds;

	  list = block->ddlist;
	  for (idx = 0; idx < block->ndds; idx++)
	    {
		/* skip the empty dd's */
		if ((list[idx].tag == DFTAG_NULL && cnt_tag != DFTAG_NULL)
		  || (list[idx].tag == DFTAG_FREE && cnt_tag != DFTAG_FREE))
		    continue;

		if (((cnt_tag == DFTAG_WILDCARD || list[idx].tag == cnt_tag)
		  || (special_tag != DFTAG_NULL && special_tag != DFTAG_FREE
		      && list[idx].tag == special_tag))
		 && (cnt_ref == DFREF_WILDCARD || list[idx].ref == cnt_ref))
		  { /* we have a match !! */
		      t_real_cnt++;
		  }	/* end if */
	    }	/* end for */
      }		/* end for */

    *all_cnt = t_all_cnt;
    *real_cnt = t_real_cnt;
    return (SUCCEED);
}	/* HIcount_dd */

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

---------------------------------------------------------------------------*/
intn 
HDflush(int32 file_id)
{
    CONSTR(FUNC, "HDflush");    /* for HERROR */

#ifndef MAC
    filerec_t  *file_rec;

    file_rec = FID2REC(file_id);
    if (BADFREC(file_rec))
        HRETURN_ERROR(DFE_ARGS, FAIL);

    fflush(file_rec->file);
#endif /* MAC */

    return SUCCEED;
}	/* HDflush */

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

---------------------------------------------------------------------------*/
intn 
HDpackFstring(char *src, char *dest, intn len)
{
    intn        sofar;

    for (sofar = 0; (sofar < len) && (*src != '\0'); sofar++)
        *dest++ = *src++;

    while (sofar++ < len)
        *dest++ = ' ';

    return SUCCEED;
}	/* HDpackFstring */

/* ----------------------------- HDgettagdesc ----------------------------- */
/*
NAME
   HDgettagdesc -- return a text description of a tag
USAGE
   char * HDgettagdesc(tag)
   uint16   tag;          IN: tag of element to find
RETURNS
   Descriptive text or NULL
DESCRIPTION
   Map a tag to a statically allocated text description of it.

---------------------------------------------------------------------------*/
const char _HUGE *
HDgettagdesc(uint16 tag)
{
    intn        i;

    for (i = 0; i < sizeof(tag_descriptions) / sizeof(tag_descript_t); i++)
        if (tag_descriptions[i].tag == tag)
            return (tag_descriptions[i].desc);
    return (NULL);
} /* HDgettagdesc */

/* ----------------------------- HDgettagsname ----------------------------- */
/*
NAME
   HDgettagsname -- return a text name of a tag
USAGE
   char * HDgettagsname(tag)
   uint16   tag;          IN: tag of element to find
RETURNS
   Descriptive text or NULL
DESCRIPTION
   Map a tag to a dynamically allocated text name of it.
   Checks for special elements now.

--------------------------------------------------------------------------- */
char _HUGE *
HDgettagsname(uint16 tag)
{
    CONSTR(FUNC, "HDgettagsname");  /* for HERROR */
    char       *ret = NULL;
    intn        i;

    if (SPECIALTAG(tag))
        ret = (char *) HDstrdup("Special ");
    tag = BASETAG(tag);
    for (i = 0; i < sizeof(tag_descriptions) / sizeof(tag_descript_t); i++)
        if (tag_descriptions[i].tag == tag)
          {
              if (ret == NULL)
                  ret = (char *) HDstrdup((char *) tag_descriptions[i].name);
              else
                {
                    char       *t;

                    t = (char *) HDmalloc(HDstrlen(ret) +
                                    HDstrlen(tag_descriptions[i].name) + 2);
                    if (t == NULL)
                      {
                          HDfree(ret);
                          HRETURN_ERROR(DFE_NOSPACE, NULL);
                      }     /* end if */
                    HDstrcpy(t, ret);
                    HDstrcat(t, tag_descriptions[i].name);
                    HDfree(ret);
                    ret = t;
                }   /* end else */
          }     /* end if */
    return (ret);
}  /* HDgettagsname */

/* ----------------------------- HDgettagnum ------------------------------ */
/*
NAME
   HDgettagnum -- return the tag number for a text description of a tag
USAGE
   intn HDgettagnum(tag_name)
   char *   tag_name;         IN: name of tag to find
RETURNS
   Tag number (>=0) on success or FAIL on failure
DESCRIPTION
   Map a tag name to a statically allocated tag number for it.

---------------------------------------------------------------------------*/
intn 
HDgettagnum(const char *tag_name)
{
    intn        i;

    for (i = 0; i < sizeof(tag_descriptions) / sizeof(tag_descript_t); i++)
        if (0 == HDstrcmp(tag_descriptions[i].name, tag_name))
            return (tag_descriptions[i].tag);
    return (FAIL);
} /* HDgettagnum */

/* ----------------------------- HDgetNTdesc ----------------------------- */
/*
NAME
   HDgetNTdesc -- return a text description of a number-type
USAGE
   char * HDgetNTdesc(nt)
   int32   nt;          IN: tag of element to find
RETURNS
   Descriptive text or NULL
DESCRIPTION
   Map a number-type to a dynamically allocated text description of it.

---------------------------------------------------------------------------*/
char _HUGE *
HDgetNTdesc(int32 nt)
{
    CONSTR(FUNC, "HDgetNTdesc");    /* for HERROR */
    intn        i;
    char       *ret_desc = NULL;

    /* evil hard-coded values */
    if (nt & DFNT_NATIVE)
        ret_desc = (char *) HDstrdup((char *) nt_descriptions[0].desc);
    else if (nt & DFNT_CUSTOM)
        ret_desc = (char *) HDstrdup((char *) nt_descriptions[1].desc);
    else if (nt & DFNT_LITEND)
        ret_desc = (char *) HDstrdup((char *) nt_descriptions[2].desc);

    nt &= DFNT_MASK;    /* mask off unusual format types */
    for (i = 3; i < sizeof(nt_descriptions) / sizeof(nt_descript_t); i++)
        if (nt_descriptions[i].nt == nt)
          {
              if (ret_desc == NULL)
                  ret_desc = (char *) HDstrdup((char *) nt_descriptions[i].desc);
              else
                {
                    char       *t;

                    t = (char *) HDmalloc(HDstrlen(ret_desc) +
                                     HDstrlen(nt_descriptions[i].desc) + 2);
                    if (t == NULL)
                      {
                          HDfree(ret_desc);
                          HRETURN_ERROR(DFE_NOSPACE, NULL);
                      }     /* end if */
                    HDstrcpy(t, ret_desc);
                    HDstrcat(t, " ");
                    HDstrcat(t, nt_descriptions[i].desc);
                    HDfree(ret_desc);
                    ret_desc = t;
                }   /* end else */
              return (ret_desc);
          }     /* end if */
    return (NULL);
}   /* end HDgetNTdesc() */

/* ------------------------------- HDfidtoname ------------------------------ */
/*
NAME
   HDfidtoname -- return the filename the file ID corresponds to
USAGE
   const char _HUGE * HDfidtoname(fid)
   int32 fid;            IN: file ID
RETURNS
   SUCCEED - pointer to filename / FAIL - NULL
DESCRIPTION
   Map a file ID to the filename used to get it.  This is useful for 
   mixing old style single-file interfaces (which take filenames) and
   newer interfaces which use file IDs.

---------------------------------------------------------------------------*/
const char _HUGE *
HDfidtoname(int32 file_id)
{
    CONSTR(FUNC, "HDfidtoname");    /* for HERROR */
    filerec_t  *file_rec;

    if ((file_rec = FID2REC(file_id)) == NULL)
        HRETURN_ERROR(DFE_ARGS, NULL);

    return (file_rec->path);
}   /* HDfidtoname */

/*--------------------------------------------------------------------------
 NAME
    HDFend
 PURPOSE
    Terminate various static buffers and shutdown the library.
 USAGE
    intn HDFend()
 RETURNS
    Returns SUCCEED/FAIL
 DESCRIPTION
    Walk through the shutdown routines for the various interfaces and 
    terminate them all.
 GLOBAL VARIABLES
 COMMENTS, BUGS, ASSUMPTIONS
    Should only ever be called by the "atexit" function, or real power-users.
 EXAMPLES
 REVISION LOG
--------------------------------------------------------------------------*/
void HDFend(void)
{
    CONSTR(FUNC, "HDFend");    /* for HERROR */

    /* can't issue errors if you're free'ing the error stack. */
    VSPfreebuf();
    Hshutdown();
    HEshutdown();
} /* end HDFend() */

