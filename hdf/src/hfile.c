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

/*LINTLIBRARY*/
/*+
 FILE
       hfile.c
       HDF low level file I/O routines
 EXPORTED ROUTINES
       Hopen       -- open or create a HDF file
       Hclose      -- close HDF file
       Hstartread  -- locate and position a read access elt on a tag/ref
       Hnextread   -- locate and position a read access elt on next tag/ref.
       Hinquire    -- inquire stats of an access elt
       Hstartwrite -- set up a WRITE access elt for a write
       Happendable -- attempt make a dataset appendable
       Hseek       -- position an access element to an offset in data element
       Hread       -- read the next segment from data element
       Hwrite      -- write next data segment to data element
       Hendaccess  -- to dispose of an access element
       Hgetelement -- read in a data element
       Hlength     -- returns length of a data element
       Htrunc      -- truncate a dataset to a length
       Hoffset     -- get offset of data element in the file
       Hputelement -- writes a data element
       Hdupdd      -- duplicate a data descriptor
       Hdeldd      -- delete a data descriptor
       Hnewref     -- returns a ref that is guaranteed to be unique in the file
       Hishdf      -- tells if a file is an HDF file
       Hsync       -- sync file with memory
       Hnumber     -- count number of occurrances of tag/ref in file
       Hgetlibversion  -- return version info on current HDF library
       Hgetfileversion -- return version info on HDF file
       Hfind       -- locate the next object of a search in an HDF file
+*/

#define HMASTER
#include "hdf.h"
#undef HMASTER
#include "hfile.h"

/* Array of file records that contains all relevant
   information on an opened HDF file.
   See hfile.h for structure and members definition of filerec_t. */

#if defined(macintosh) | defined(THINK_C) | defined(DMEM) /* Dynamic memory */
struct filerec_t *file_records = NULL;
#else /* !macintosh */
struct filerec_t file_records[MAX_FILE];
#endif /* !macintosh */

/* Array of records of information on each access elements.
   These will contain information like how to access the data element,
   where in the data element the current access should start from, etc.
   Allocated dynamically.
   See hfile.h for definition. */

struct accrec_t *access_records = NULL;

/* Temporary memory space for doing some general stuff so we don't
   have to allocate and deallocate memory all the time.  This space should
   be "sufficiently" large, or at least 64 bytes long.  Routines using
   ptbuf should not assume that the buffer is longer than that. */

#ifdef OLD_WAY /* replaced with dynamic memory */
int32 int_tbuf[TBUF_SZ];
uint8 *tbuf = (uint8 *)int_tbuf;
#endif
PRIVATE uint8 *ptbuf = NULL;

/* Function tables declarations.  These function tables contain pointers
   to functions that help access each type of special element. */

/* Functions for accessing the linked block special
   data element.  For definition of the linked block, see hblocks.c. */

extern funclist_t linked_funcs;

/* Functions for accessing external data elements, or data
   elements that are in some other files.  For definition of the external
   data element, see hextelt.c. */

extern funclist_t ext_funcs;

/* Functions for accessing compressed data elements.  
   For definition of the compressed data element, see hcomp.c. */

extern funclist_t comp_funcs;

/* Functions for accessing big external data elements, or data
   elements that are in some other files.  For definition of the big external
   data element, see hbigext.c. */

extern funclist_t bigext_funcs;

/* Table of these function tables for accessing special elements.  The first
   member of each record is the speical code for that type of data element. */
functab_t functab[] = {
    {SPECIAL_LINKED, &linked_funcs},
    {SPECIAL_EXT, &ext_funcs},
    {SPECIAL_COMP, &comp_funcs},
    {SPECIAL_BIGEXT, &bigext_funcs},
    {0, NULL}                  /* terminating record; add new record */
                               /* before this line */
};

/*
** Declaration of private functions.
*/
#ifdef DELETE_FOR_40_RELEASE_IF_NOT_USED
PRIVATE intn HIlock
  (int32 file_id);
#endif

PRIVATE intn HIunlock
  (int32 file_id);

#ifdef DELETE_FOR_40_RELEASE_IF_NOT_USED
PRIVATE intn HIchangedd
  (dd_t *datadd, ddblock_t *block, intn idx, int16 special,
	 VOIDP special_info, funclist_t *special_func);
#endif

PRIVATE intn HIget_file_slot
  (const char *path, char *FUNC);

PRIVATE intn HIvalid_magic
  (hdf_file_t file, char *FUNC);

PRIVATE intn HIfill_file_rec
  (filerec_t *file_rec, char *FUNC);

PRIVATE intn HIinit_file_dds
  (filerec_t *file_rec, int16 ndds, char *FUNC);

PRIVATE intn HIflush_dds
  (filerec_t *file_rec);

PRIVATE intn HIextend_file
  (filerec_t *file_rec);

PRIVATE funclist_t *HIget_function_table 
  (accrec_t *access_rec, char *FUNC);

PRIVATE intn HIupdate_version
  (int32);

PRIVATE intn HIread_version
  (int32);

/* #define TESTING */

/*--------------------------------------------------------------------------
 NAME
       Hopen -- Opens a HDF file.
 USAGE
       int32 Hopen(path, access, ndds)
       char *path;             IN: Name of file to be opened.
       int access;             IN: DFACC_READ, DFACC_WRITE, DFACC_CREATE
                                   or any bitwise-or of the above.
       int16 ndds;             IN: Number of dds in a block if this
                                   file needs to be created.
 RETURNS
       On success returns file id, on failure returns -1.
 DESCRIPTION
       Opens a HDF file.  Returns the the file ID on success, or -1
       on failure.

       Access equals DFACC_CREATE means discard existing file and
       create new file.  If access is a bitwise-or of DFACC_CREATE
       and anything else, the file is only created if it does not
       exist.  DFACC_WRITE set in access also means that if the file
       does not exist, it is created.  DFACC_READ is assumed to be
       implied even if it is not set.  DFACC_CREATE implies
       DFACC_WRITE.

       If the file is already opened and access is DFACC_CREATE:
       error DFE_ALROPEN.
       If the file is already opened, the requested access contains
       DFACC_WRITE, and previous open does not allow write: attempt
       to reopen the file with write permission.

       On successful exit,
       * file_rec members are filled in correctly.
       * file is opened with the relevant permission.
       * information about dd's are set up in memory.
       For new file, in addition,
       * the file headers and initial information are set up properly.

--------------------------------------------------------------------------*/
int32 Hopen(const char *path, intn acc_mode, int16 ndds)
{
    CONSTR(FUNC,"Hopen");       /* For HERROR */
    int slot;               /* File record slot */
    filerec_t *file_rec;    /* File record */
    int vtag = 0;	/* write version tag? */

    /* Clear errors and check args and all the boring stuff. */
    HEclear();
    if (!path || ((acc_mode & DFACC_ALL) != acc_mode)) 
       HRETURN_ERROR(DFE_ARGS,FAIL);

    /* Check if temproray buffer has been allocated */
    /* Note: ptbuf is not actually used in Hopen, but many other routines */
    /*     depend on having it allocated, so this is the best place to put */
    /*     it, not scattered throughout the file. */
    if(ptbuf==NULL) 
        if((ptbuf=(uint8 *)HDgetspace(TBUF_SZ*sizeof(uint8)))==NULL)
            HRETURN_ERROR(DFE_NOSPACE,FAIL);

    /* Get a space to put the file information.
     * HIget_file_slot() also copies path into the record. */
    slot = HIget_file_slot(path, FUNC);
    if (slot == FAIL) 
       HRETURN_ERROR(DFE_TOOMANY, FAIL); /* The slots are full. */

    file_rec = &(file_records[slot]);
    if (file_rec->refcount) {
    /* File is already opened, check that permission is okay. */

    /* If this request is to create a new file and file is still
     * in use, return error. */
    if (acc_mode == DFACC_CREATE) 
       HRETURN_ERROR(DFE_ALROPEN,FAIL);

    if ((acc_mode & DFACC_WRITE) && !(file_rec->access & DFACC_WRITE)) {
       /* If the request includes writing, and if original open does not
          provide for write, then try to reopen file for writing.
          This cannot be done on OS (such as the SXOS) where only one
          open is allowed per file at any time. */
#ifndef NO_MULTI_OPEN
           hdf_file_t f;

           f = HI_OPEN(file_rec->path, acc_mode);
           if (OPENERR(f)) 
               HRETURN_ERROR(DFE_DENIED,FAIL);

           /* Replace file_rec->file with new file pointer and
              close old one. */
           if (HI_CLOSE(file_rec->file) == FAIL) {
               HI_CLOSE(f);
               HRETURN_ERROR(DFE_CANTCLOSE,FAIL);
           }
           file_rec->file = f;
#else /* NO_MULTI_OPEN */
           HRETURN_ERROR(DFE_DENIED,FAIL);
           return FAIL;
#endif /* NO_MULTI_OPEN */
       }

       /* There is now one more open to this file. */
       file_rec->refcount++;
    } else {

       /* Flag to see if file is new and needs to be set up. */
       intn new_file=FALSE;

       /* Open the file, fill in the blanks and all the good stuff. */
       if (acc_mode != DFACC_CREATE) {
           /* try to open existing file */

           file_rec->file = HI_OPEN(file_rec->path, acc_mode);
           if (OPENERR(file_rec->file)) {
               if (acc_mode & DFACC_WRITE) {
                   /* Seems like the file is not there, try to create it. */
                   new_file = TRUE;
               } else 
                   HRETURN_ERROR(DFE_BADOPEN,FAIL);

           } else {
               /* Open existing file successfully. */
               file_rec->access = acc_mode | DFACC_READ;

               /* Check to see if file is a HDF file. */
               if (!HIvalid_magic(file_rec->file, FUNC)) {
                   HI_CLOSE(file_rec->file);
                   HRETURN_ERROR(DFE_NOTDFFILE,FAIL);
               }

               /* Read in all the relevant data descriptor records. */
               if (HIfill_file_rec(file_rec, FUNC) == FAIL) {
                   HI_CLOSE(file_rec->file);
                   HRETURN_ERROR(DFE_BADOPEN,FAIL);
               }
           }
       }
       /* do *not* use else here */

       if (acc_mode == DFACC_CREATE || new_file) {
           /* create the file */
           
           /* make user we get a version tag */
           vtag = 1;
           
           file_rec->file = HI_CREATE(path);
           if (OPENERR(file_rec->file))
               HRETURN_ERROR(DFE_BADOPEN,FAIL);

           /* set up the newly created (and empty) file with
              the magic cookie and initial data descriptor records */
           if (HI_WRITE(file_rec->file, HDFMAGIC, MAGICLEN) == FAIL) 
               HRETURN_ERROR(DFE_WRITEERROR,FAIL);

           if (HI_FLUSH(file_rec->file) == FAIL)  /* flush the cookie */
               HRETURN_ERROR(DFE_WRITEERROR,FAIL);

           if (HIinit_file_dds(file_rec, ndds, FUNC) == FAIL) 
               HRETURN_ERROR(DFE_WRITEERROR,FAIL);

           file_rec->maxref = 0;
           file_rec->access = new_file ? acc_mode | DFACC_READ : DFACC_ALL;
       }
       file_rec->refcount = 1;
       file_rec->attach = 0;

       /* currently, default is caching OFF */
       file_rec->cache=FALSE;
       file_rec->dirty=0; /* mark all dirty flags off to start */

       /* Set up the new pointers for empty space */
       file_rec->null_block = file_rec->ddhead;
       file_rec->null_idx   = -1;
       
       /* version tags */
       if (vtag == 1)
           HIupdate_version(FSLOT2ID(slot));
     } /* end else */

    file_rec->version_set = FALSE;

    if(vtag==0)
        HIread_version(FSLOT2ID(slot));
    /* end version tags */

    return FSLOT2ID(slot);
} /* Hopen */

/*--------------------------------------------------------------------------
 NAME
       Hclose -- close HDF file
 USAGE
       intn Hclose(id)
       int id;                 IN: the file id to be closed
 RETURNS
       returns SUCCEED (0) if successful and FAIL (-1) if failed.
 DESCRIPTION
       closes an HDF file given the file id.  Id is first validated.  If
       there are still access objects attached to the file, an error is
       returned and the file is not closed.

--------------------------------------------------------------------------*/
intn Hclose(int32 file_id)
{
    register intn i;
    CONSTR(FUNC,"Hclose");       /* for HERROR */
    filerec_t *file_rec;       /* file record pointer */
    register tag_ref_list_ptr p, q;
    
    /* convert file id to file rec and check for validity */
    file_rec = FID2REC(file_id);
    if (BADFREC(file_rec))
        HRETURN_ERROR(DFE_ARGS,FAIL);
    
    /* version tags */
    if ((file_rec->refcount > 0) && (file_rec->version.modified == 1))
        HIupdate_version(file_id);
    /* end version tags */
    
    /* decrease the reference count */
    if (--file_rec->refcount == 0) {
        ddblock_t *bl, *next;  /* current ddblock and next ddblock pointers.
                                  for freeing ddblock linked list*/
        
        /* if file reference count is zero but there are still attached
           access elts, reject this close. */
        
        if (file_rec->attach > 0) {
            file_rec->refcount++;
            HEreport("There are still %d active aids attached",file_rec->attach);
            HRETURN_ERROR(DFE_OPENAID,FAIL);
        }
        
	/* before closing file, check whether to flush file info */
	if(file_rec->cache && file_rec->dirty) {

            /* flush DD blocks if necessary */
	    if(file_rec->dirty&DDLIST_DIRTY)
	        if(HIflush_dds(file_rec)==FAIL)
                    HRETURN_ERROR(DFE_CANTFLUSH,FAIL);

            /* extend the end of the file if necessary */
	    if(file_rec->dirty&FILE_END_DIRTY) 
	        if(HIextend_file(file_rec)==FAIL)
                    HRETURN_ERROR(DFE_CANTFLUSH,FAIL);
	  } /* end if */

        /* otherwise, nothing should still be using this file, close it */
#if 0
        if (HI_CLOSE(file_rec->file) == FAIL)
            HRETURN_ERROR(DFE_CLOSE,FAIL);
#else
        /* ignore any close error */
        HI_CLOSE(file_rec->file);
#endif
        
        /* free the ddblock linked list of this file record, the path name;
           and reset some variables */
        
        for (bl = file_rec->ddhead; bl; bl = next) {
            next = bl->next;
            if (bl->ddlist)
                HDfreespace((VOIDP) bl->ddlist);
            if (bl)
                HDfreespace((VOIDP)bl);
        }
        
        for(i = 0; i < HASH_MASK + 1; i++) {
            for(p = file_rec->hash[i]; p; p = q) {
                q = p->next;
                HDfreespace((VOIDP)p);
            }
            file_rec->hash[i] = NULL;
        }
        
        file_rec->ddhead = (ddblock_t *) NULL;
        if (file_rec->path)
            HDfreespace(file_rec->path);
        file_rec->path = (char *)NULL;
    }
    
    return SUCCEED;
} /* Hclose */

/*--------------------------------------------------------------------------

 NAME
       Hstartread -- locate and position a read access elt on a tag/ref
 USAGE
       int32 Hstartread(fileid, tag, ref)
       int fileid;             IN: id of file to attach access element to
       int tag;                IN: tag to search for
       int ref;                IN: ref to search for
 RETURNS
       returns id of access element if successful, otherwise FAIL (-1)
 DESCRIPTION
       Searches the DD's for a particular tag/ref combination.  The
       searching starts from the head of the DD list.  Wildcards can be
       used for tag or ref (DFTAG_WILDCARD, DFREF_WILDCARD) and they match
       any values.  If the search is successful, the access elt is
       positioned to the start of that tag/ref, otherwise it is an error.
       An access element is created and attached to the file.

--------------------------------------------------------------------------*/
int32 Hstartread(int32 file_id, uint16 tag, uint16 ref)
{
    CONSTR(FUNC,"Hstartread");   /* for HERROR */
    int slot;                  /* slot in access record array */
    filerec_t *file_rec;       /* file record */
    accrec_t *access_rec;      /* access record */

    /* clear error stack */
    HEclear();

    /* convert file id to file record and check for validity */
    file_rec = FID2REC(file_id);
    if (BADFREC(file_rec))
       HRETURN_ERROR(DFE_ARGS,FAIL);

    /* get a slot in the access record array */
    slot = HIget_access_slot();
    if (slot == FAIL)
       HRETURN_ERROR(DFE_TOOMANY,FAIL);
    access_rec = &(access_records[slot]);

    /* convert tag to base form */
    tag = BASETAG(tag);

    /* search for the data element in the dd lists of the file */
    access_rec->file_id = file_id;
    access_rec->block = file_rec->ddhead;
    access_rec->idx = -1;
    access_rec->appendable=FALSE;   /* start data as non-appendable */
    access_rec->flush=FALSE;        /* start data as not needing flushing */
    if(HIlookup_dd(file_rec,tag,ref,&access_rec->block,&access_rec->idx)==FAIL) {
       access_rec->used = FALSE;
       HRETURN_ERROR(DFE_NOMATCH,FAIL);
    }

    /* if special data element, get the relevant special function table
       and run the START READ function on this element */
    if (SPECIALTAG(access_rec->block->ddlist[access_rec->idx].tag)) {
        access_rec->special_func = HIget_function_table(access_rec, FUNC);
        if(access_rec->special_func==NULL) {
            access_rec->used = FALSE;
            HRETURN_ERROR(DFE_INTERNAL,FAIL);
        }
        return (*access_rec->special_func->stread)(access_rec);
    }
    
    /* reset the data element and update file record */
    access_rec->posn = 0;
    access_rec->access = DFACC_READ;
    access_rec->special = 0;
    file_rec->attach++;

    return ASLOT2ID(slot);

}   /* Hstartread() */

/*--------------------------------------------------------------------------

 NAME
       Hnextread -- locate and position a read access elt on tag/ref.
 USAGE
       intn Hnextread(access_id, tag, ref, origin)
       int32 access_id;         IN: id of a READ access elt
       uint16 tag;              IN: the tag to search for
       uint16 ref;              IN: ref to search for
       int origin;              IN: from where to start searching
 RETURNS
       returns SUCCEED (0) if successful and FAIL (-1) otherwise
 DESCRIPTION
       Searches for the `next' DD that fits the tag/ref.  Wildcards
       apply.  If origin is DF_START, search from start of DD list,
       if origin is DF_CURRENT, search from current position, otherwise
       origin should be DF_END which searches from end of file.
       If the search is successful, then the access elt
       is positioned at the start of that tag/ref, otherwise, it is not
       modified.
 COMMENTS, BUGS, ASSUMPTIONS
    DF_END _not_ supported yet!

--------------------------------------------------------------------------*/
intn Hnextread(int32 access_id, uint16 tag, uint16 ref, intn origin)
{
    CONSTR(FUNC,"Hnextread");    /* for HERROR */
    filerec_t *file_rec;       /* file record */
    accrec_t *access_rec;      /* access record */
    ddblock_t *block;
    int32 idx;

    /* clear error stack and check validity of the access id */
    HEclear();
    access_rec = AID2REC(access_id);
    if (access_rec == (accrec_t *) NULL || !access_rec->used
            || access_rec->access != DFACC_READ
            || (origin != DF_START && origin != DF_CURRENT)) 
       HRETURN_ERROR(DFE_ARGS,FAIL);

    /* DF_END is NOT supported yet !!!! */
    file_rec = FID2REC(access_rec->file_id);
    if (BADFREC(file_rec))
        HRETURN_ERROR(DFE_INTERNAL,FAIL);

    /*
     * if access record used to point to an external element we
     * need to close the file before moving on
     */
    if(access_rec->special == SPECIAL_EXT) {
        if(HXPcloseAID(access_rec) == FAIL)
            HRETURN_ERROR(DFE_CANTCLOSE,FAIL);
    }

    if (origin == DF_START) {
       /* set up variables to start searching from beginning of file */
       block = file_rec->ddhead;
       idx = -1;

    } else {                   /* origin == CURRENT */
       /* set up variables to start searching from the current position */

       block = access_rec->block;
       idx = access_rec->idx;
    }

    /* go look for the dd */
    if (HIlookup_dd(file_rec, tag, ref, &block, &idx) == FAIL)
       HRETURN_ERROR(DFE_NOMATCH,FAIL);

    /* found, so update the access record */
    access_rec->block = block;
    access_rec->idx = idx;
    access_rec->appendable=FALSE;   /* start data as non-appendable */
    access_rec->flush=FALSE;        /* start data as not needing flushing */
    if (SPECIALTAG(access_rec->block->ddlist[access_rec->idx].tag)) {
	int32 ret;

       /* special element, call special function to handle */
       access_rec->special_func = HIget_function_table(access_rec, FUNC);
       if (!access_rec->special_func)
           HRETURN_ERROR(DFE_INTERNAL,FAIL);
       HIunlock(access_rec->file_id); /* remove old attach to the file_rec */
#ifdef OLD_WAY
       return (int)(*access_rec->special_func->stread)(access_rec);
#else
	if((ret=(*access_rec->special_func->stread)(access_rec))!=FAIL)
		return(SUCCEED);
	else
		return(FAIL);
#endif
    }

    access_rec->special = 0;
    access_rec->posn = 0;

    return SUCCEED;

}   /* end Hnextread() */

/*--------------------------------------------------------------------------

 NAME
       Hfind -- locate the next object of a search in an HDF file
 USAGE
       intn Hfind(file_id ,search_tag, search_ref, find_tag, find_ref,
            find_offset, find_length, position)
       int32 file_id;           IN: file ID to search in
       uint16 search_tag;       IN: the tag to search for
                                    (can be DFTAG_WILDCARD)
       uint16 search_ref;       IN: ref to search for
                                    (can be DFREF_WILDCARD)
       uint16 *find_tag;        IN: if (*find_tag==0) and (*find_ref==0)
                                    then start search
                                OUT: tag matching the search tag
       uint16 *find_ref;        IN: if (*find_tag==0) and (*find_ref==0)
                                    then start search
                                OUT: ref matching the search ref
       int32 *find_offset;      OUT: offset of the data element found
       int32 *find_length;      OUT: length of the data element found
       intn direction;          IN: Direction to search in - DF_FORWARD
                                    searches forward from the current location,
                                    DF_BACKWARD searches backward from the
                                    current location.
 RETURNS
       returns SUCCEED (0) if successful and FAIL (-1) otherwise
 DESCRIPTION
       Searches for the `next' DD that fits the search tag/ref.  Wildcards
       apply.  If origin is DF_FORWARD, search from current position forwards
       in the file, otherwise DF_BACKWARD searches backward from the current
       position in the file.  If *find_tag and *find_ref are both set to
       0, this indicates the beginning of a search, and the search will
       start from the beginning of the file if the direction is DF_FORWARD
       and from the and of the file if the direction is DF_BACKWARD.

--------------------------------------------------------------------------*/
intn Hfind(int32 file_id, uint16 search_tag, uint16 search_ref,
    uint16 *find_tag,uint16 *find_ref,int32 *find_offset,int32 *find_length,
    intn direction)
{
    CONSTR(FUNC,"Hfind");         /* for HERROR */
    filerec_t *file_rec;        /* file record */
    ddblock_t *block;
    int32 idx;
    dd_t *list;                 /* ptr to current ddlist searched */

    /* clear error stack and check validity of the access id */
    HEclear();
    if(file_id == FAIL || search_ref>MAX_REF || find_tag==NULL
            || find_ref==NULL || find_offset==NULL || find_length==NULL
            || (direction != DF_FORWARD && direction != DF_BACKWARD))
        HRETURN_ERROR(DFE_ARGS,FAIL);

    file_rec = FID2REC(file_id);
    if (BADFREC(file_rec))
        HRETURN_ERROR(DFE_INTERNAL,FAIL);

    if(*find_ref==0 && *find_tag==0) {  /* starting search */
        if(direction==DF_FORWARD) {     /* start at beginning of the DD list */
            block = file_rec->ddhead;
            idx = -1;
          } /* end if */
        else {      /* start the very end of the DD list */
            block = file_rec->ddlast;
            idx = block->ndds;
          } /* end else */
      } /* end if */
    else {      /* continue a search */
        /* get the block and index of the last tag/ref found, to continue */
        if(HIlookup_dd(file_rec, *find_tag, *find_ref, &block, &idx)==FAIL)
            HRETURN_ERROR(DFE_NOMATCH,FAIL);
      } /* end else */

    /* Go get the next match in the given direction */
    if(HIfind_dd(search_tag,search_ref,&block,&idx,direction)==FAIL)
        HRETURN_ERROR(DFE_NOMATCH,FAIL);

    list=block->ddlist;         /* get a pointer to the DD list to look in */
    *find_tag=list[idx].tag;
    *find_ref=list[idx].ref;
    *find_offset=list[idx].offset;
    *find_length=list[idx].length;

    return(SUCCEED);
}   /* end Hfind() */

/*--------------------------------------------------------------------------

 NAME
       Hexist -- locate an object in an HDF file
 USAGE
       intn Hfind(file_id ,search_tag, search_ref)
       int32 file_id;           IN: file ID to search in
       uint16 search_tag;       IN: the tag to search for
                                    (can be DFTAG_WILDCARD)
       uint16 search_ref;       IN: ref to search for
                                    (can be DFREF_WILDCARD)
 RETURNS
       returns SUCCEED (0) if successful and FAIL (-1) otherwise
 DESCRIPTION
       Simple interface to Hfind which just determines if a given
       tag/ref pair exists in a file.  Wildcards apply.
 GLOBAL VARIABLES
 COMMENTS, BUGS, ASSUMPTIONS
        Hfind() does all validity checking, this is just a _very_
        simple wrapper around it.
 EXAMPLES
 REVISION LOG
--------------------------------------------------------------------------*/
intn Hexist(int32 file_id, uint16 search_tag, uint16 search_ref)
{
#ifdef LATER
    CONSTR(FUNC,"Hexist");        /* for HERROR */
#endif
    uint16 find_tag=0,find_ref=0;
    int32 find_offset,find_length;

    return(Hfind(file_id,search_tag,search_ref,&find_tag,&find_ref,
            &find_offset,&find_length,DF_FORWARD));
}   /* end Hexist() */

/*--------------------------------------------------------------------------

 NAME
       Hinquire -- inquire stats of an access elt
 USAGE
       intn Hinquire(access_id, pfile_id, ptag, pref, plength,
                       poffset, pposn, paccess, pspecial)
       int access_id;          IN: id of an access elt
       int32 *pfile_id;        OUT: file id
       uint16 *ptag;           OUT: tag of the element pointed to
       uint16 *pref;           OUT: ref of the element pointed to
       int32 *plength;         OUT: length of the element pointed to
       int32 *poffset;         OUT: offset of elt in the file
       int32 *pposn;           OUT: position pointed to within the data elt
       int16 *paccess;         OUT: the access type of this access elt
       int16 *pspecial;        OUT: special code
 RETURNS
       returns SUCCEED (0) if the access elt points to some data element,
       otherwise FAIL (-1)
 DESCRIPTION
       Inquire statistics of the data element pointed to by access elt and
       the access elt.  The access type is set if the access_id is valid even
       if FAIL is returned.  If access_id is not valid then access is set to
       zero (0). If statistic is not needed, pass NULL for the appropriate
       value.

--------------------------------------------------------------------------*/
intn Hinquire(int32 access_id, int32 *pfile_id, uint16 *ptag, uint16 *pref,
              int32 *plength, int32 *poffset, int32 *pposn, int16 *paccess,
              int16 *pspecial)
{
    CONSTR(FUNC,"Hinquire");     /* for HERROR */
    register accrec_t *access_rec;      /* access record */
    register dd_t *dd;                  /* dd of access record */

    /* clear error stack and check validity of access id */

    HEclear();
    access_rec = AID2REC(access_id);
    if (access_rec == (accrec_t *) NULL || !access_rec->used)
       HRETURN_ERROR(DFE_ARGS,FAIL);

    /* if special elt, let special functions handle it */
    if (access_rec->special)
       return (int)(*access_rec->special_func->inquire)(access_rec, pfile_id,
            ptag, pref, plength, poffset, pposn, paccess, pspecial);

    /* get dd and fill in variables asked for (i.e. not NULL) */
    dd = &(access_rec->block->ddlist[access_rec->idx]);
    if (pfile_id!=NULL)
        *pfile_id = access_rec->file_id;
    if (ptag!=NULL)
        *ptag = dd->tag;
    if (pref!=NULL)
        *pref = dd->ref;
    if (plength!=NULL)
        *plength = dd->length;
    if (poffset!=NULL)
        *poffset = dd->offset;
    if (pposn!=NULL)
        *pposn = access_rec->posn;
    if (paccess!=NULL)
        *paccess = access_rec->access;
    if (pspecial!=NULL)
        *pspecial = 0;

    return SUCCEED;

}   /* end Hinquire */

/*--------------------------------------------------------------------------

 NAME
       Hstartwrite -- set up a WRITE access elt for a write
 USAGE
       int32 Hstartwrite(fileid, tag, ref, len)
       int fileid;             IN: id of file to write to
       int tag;                IN: tag to write to
       int ref;                IN: ref to write to
       long length;            IN: the length of the data element
 RETURNS
       returns id of access element if successful and FAIL otherwise
 DESCRIPTION
       Set up a WRITE access elt to write out a data element.  The DD list
       of the file is searched first.  If the tag/ref is found, it is
       NOT replaced - the seek position is presumably at 0.
                If it does not exist, it is created.

--------------------------------------------------------------------------*/
int32 Hstartwrite(int32 file_id, uint16 tag, uint16 ref, int32 length)
{
    CONSTR(FUNC,"Hstartwrite");  /* for HERROR */
    int slot;                  /* free access records array slot */
    intn ddnew = FALSE;                /* is the dd a new one? */
    filerec_t *file_rec;       /* file record */
    accrec_t *access_rec;      /* access record */
    /* version tags */
    uint32 lmajorv, lminorv, lrelease;
    uint32 fmajorv, fminorv, frelease;
    char string[LIBVSTR_LEN+1]; /* len 80+1  */
    int newver;
    /* end version tags */

    /* clear error stack and check validity of file id */
    HEclear();
    file_rec = FID2REC(file_id);
    if (BADFREC(file_rec))
        HRETURN_ERROR(DFE_ARGS,FAIL);

    /* can write in this file? */
    if (!(file_rec->access & DFACC_WRITE))
        HRETURN_ERROR(DFE_DENIED,FAIL);

    /* get empty slot in access records */
    slot = HIget_access_slot();
    if (slot == FAIL)
        HRETURN_ERROR(DFE_TOOMANY,FAIL);

#ifdef OLD_WAY
    /* Check if temproray buffer has been allocated */
    if (ptbuf == NULL) 
        if (( ptbuf = (uint8 *)HDgetspace(TBUF_SZ * sizeof(uint8)))== NULL)
            HRETURN_ERROR(DFE_NOSPACE, FAIL);
#endif
    
    /* convert tag to base form */
    tag = BASETAG(tag);

    /* set up access record to look for the dd */
    access_rec = &(access_records[slot]);
    access_rec->file_id = file_id;
    access_rec->block = file_rec->ddhead;
    access_rec->idx = -1;
    access_rec->appendable=FALSE;   /* start data as non-appendable */
    access_rec->flush=FALSE;        /* start data as not needing flushing */
    access_rec->special_info = NULL;
    if(HIlookup_dd(file_rec,tag,ref,&access_rec->block,&access_rec->idx)==FAIL){
	/* dd not found, so have to create new element */

        /* look for empty dd slot */
        if (HIfind_dd((uint16)DFTAG_NULL, (uint16)DFREF_WILDCARD,
                &file_rec->null_block, &file_rec->null_idx, DF_FORWARD)!=FAIL) {
            access_rec->block = file_rec->null_block;
            access_rec->idx   = file_rec->null_idx;
          } /* end if */
	else {     /* cannot find empty dd slot, so create new dd block */
             if (HInew_dd_block(file_rec, FILE_NDDS(file_rec), FUNC) == FAIL) {
                 access_rec->used = FALSE;
                 HRETURN_ERROR(DFE_NOFREEDD,FAIL);
               } /* end if */
	     else {     /* use dd slot in new dd block */
                 access_rec->block = file_rec->ddlast;
                 access_rec->idx = 0;
               } /* end else */
          } /* end else */

        ddnew = TRUE;
        if(HIadd_hash_dd(file_rec, tag, ref, access_rec->block, access_rec->idx)
                == FAIL)
            HRETURN_ERROR(DFE_CANTHASH,FAIL);

      } /* end if */
    else /* tag/ref already exists */
	if (SPECIALTAG(access_rec->block->ddlist[access_rec->idx].tag)) {
       
	   /* found, if this elt is special, let special function handle it */
	   access_rec->special_func = HIget_function_table(access_rec, FUNC);
	   if (!access_rec->special_func) {
	       access_rec->used = FALSE;
	       HRETURN_ERROR(DFE_INTERNAL,FAIL);
	   }
	   return (*access_rec->special_func->stwrite)(access_rec);
         } /* end if */

    /* the dd is pointed to by access_rec->block and access_rec->idx */

    /* cannot write more bytes than are allocated for element */
    if(!ddnew && (access_rec->block->ddlist[access_rec->idx].length<length)) {
        HEreport("Values: old length %d   new length%d",
                access_rec->block->ddlist[access_rec->idx].length, length);
        access_rec->used = FALSE;
        HRETURN_ERROR(DFE_BADLEN,FAIL);
    }

    if(ddnew) {    /* have to allocate new space in the file for the data */
        int32 offset;           /* offset of this data element in file */

       /* place the data element at the end of the file and record its offset */
        if ((offset=HPgetdiskblock(file_rec,length,FALSE)) == FAIL) { 
	    access_rec->used = FALSE;
            HRETURN_ERROR(DFE_SEEKERROR,FAIL);
        }

       /* fill in dd record */
        access_rec->block->ddlist[access_rec->idx].tag = tag;
        access_rec->block->ddlist[access_rec->idx].ref = ref;
        access_rec->block->ddlist[access_rec->idx].offset = offset;
        if(length>0)
            access_rec->block->ddlist[access_rec->idx].length = length;
        access_rec->appendable=TRUE;     /* mark data as appendable */
    }

    /* update dd in the file */
    if(HIupdate_dd(file_rec,access_rec->block,access_rec->idx,FUNC) == FAIL) {
        access_rec->used = FALSE;
        HRETURN_ERROR(DFE_CANTUPDATE,FAIL);
    }

    /* update the access record, and the file record */
    access_rec->posn = 0;
    access_rec->access = DFACC_WRITE;
    access_rec->file_id = file_id;
    access_rec->special = 0;
    file_rec->attach++;
    if(ref>file_rec->maxref)
        file_rec->maxref = ref;

    /*
     *  If this is the first time we are writting to this file
     *    update the version tags as needed
     */
    if(!file_rec->version_set) {
      /* version tags */
      /* get file version and set newver condition */
        newver = 0;
      
        if(Hgetfileversion(file_id,&fmajorv,&fminorv,
		&frelease,string)!=SUCCEED) {
            newver = 1;
            HEclear();
          } /* end if */
      
      /* get library version */
        Hgetlibversion(&lmajorv, &lminorv, &lrelease, string);

        /* check whether we need to update the file version tag */
        if (newver == 1 ||
              (lmajorv > fmajorv || lminorv > fminorv || lrelease > frelease)) {
            file_rec->version.majorv = lmajorv;
            file_rec->version.minorv = lminorv;
            file_rec->version.release = lrelease;
            HIstrncpy(file_rec->version.string, string, LIBVSTR_LEN+1);
            file_rec->version.modified = 1;
          } /* end if */
      
        file_rec->version_set = TRUE;

    } /* test to set version tags */
      
    return ASLOT2ID(slot);
}   /* end Hstartwrite */

/*--------------------------------------------------------------------------

 NAME
       Happendable -- Allow a data set to be appended to without the
        use of linked blocks
 USAGE
       intn Happendable(aid)
       int32 aid;              IN: aid of the dataset to make appendable
 RETURNS
       returns 0 if dataset is allowed to be appendable, FAIL otherwise
 DESCRIPTION
       If a dataset is at the end of a file, allow Hwrite()s to write
       past the end of a file.  Allows expanding datasets without the use
       of linked blocks.

--------------------------------------------------------------------------*/
intn Happendable(int32 aid)
{
    CONSTR(FUNC,"Happendable");   /* for HERROR */
    filerec_t *file_rec;        /* file record */
    accrec_t *access_rec;       /* access record */
    int32 file_off;             /* offset in the file we are at currently */
    int32 data_len;             /* length of the data we are checking */
    int32 data_off;             /* offset of the data we are checking */

    /* clear error stack and check validity of file id */
    HEclear();

    if((access_rec=AID2REC(aid))==NULL)    /* get the access_rec pointer */
        HRETURN_ERROR(DFE_ARGS,FAIL);

    file_rec = FID2REC(access_rec->file_id);
    if (BADFREC(file_rec))
        HRETURN_ERROR(DFE_ARGS,FAIL);

    /* get the offset and length of the dataset */
    data_len=access_rec->block->ddlist[access_rec->idx].length;
    data_off=access_rec->block->ddlist[access_rec->idx].offset;

    file_off=HI_TELL(file_rec->file);   /* get the current offset */
    HI_SEEKEND(file_rec->file);
    if(data_len+data_off==HI_TELL(file_rec->file))    /* dataset at end? */
        access_rec->appendable=TRUE;
    else
        access_rec->appendable=FALSE;

    HI_SEEK(file_rec->file,file_off);   /* restore the previous position */

    if(access_rec->appendable)      /* return an appropriate status */
        return(SUCCEED);
    else
        return(FAIL);

}   /* end Happendable */

/*--------------------------------------------------------------------------

 NAME
       Hseek -- position an access element to an offset in data element
 USAGE
       intn Hseek(access_id, offset, origin)
       int32 access_id;        IN: id of access element
       long offset;            IN: offset to seek to
       int origin;             IN: position to seek from by offset, 0: from
                               beginning; 1: current position; 2: end of
                               data element
 RETURNS
       returns FAIL (-1) if fail, SUCCEED (0) otherwise.
 DESCRIPTION
       Sets the position of an access element in a data element so that the
       next Hread or Hwrite will start from that position.  origin
       determines the position from which the offset should be added.  This
       routine fails if the access elt is not associated with any data
       element and if the seeked position is outside of the data element.

--------------------------------------------------------------------------*/
intn Hseek(int32 access_id, int32 offset, intn origin)
{
    CONSTR(FUNC,"Hseek");                /* for HERROR */
    accrec_t *access_rec;      /* access record */

    /* clear error stack and check validity of this access id */
    HEclear();

    access_rec = AID2REC(access_id);
    if (access_rec == (accrec_t *) NULL || !access_rec->used
        || (origin != DF_START && origin != DF_CURRENT && origin != DF_END))
       HRETURN_ERROR(DFE_ARGS,FAIL);

    /* if special elt, use special function */
    if (access_rec->special)
       return (intn)(*access_rec->special_func->seek)(access_rec, offset, origin);

    /* calculate real offset based on the origin and check for range */
    if (origin == DF_CURRENT)
        offset += access_rec->posn;
    if (origin == DF_END)
        offset += access_rec->block->ddlist[access_rec->idx].length;
    if (offset < 0 ||
            (!access_rec->appendable &&
            offset >= access_rec->block->ddlist[access_rec->idx].length)) {
      HEreport("Tried to seek to %d (object length:  %d)", offset,
               access_rec->block->ddlist[access_rec->idx].length);

      HRETURN_ERROR(DFE_BADSEEK,FAIL);
    }

    if(access_rec->appendable &&
            offset >= access_rec->block->ddlist[access_rec->idx].length) {
        filerec_t *file_rec;    /* file record */
        int32 file_off;         /* offset in the file we are at currently */
        int32 file_end;         /* length of the file */
        int32 data_len;         /* length of the data we are checking */
        int32 data_off;         /* offset of the data we are checking */

        /* get the offset and length of the dataset */
        data_len=access_rec->block->ddlist[access_rec->idx].length;
        data_off=access_rec->block->ddlist[access_rec->idx].offset;

        file_rec = FID2REC(access_rec->file_id);
        file_off=HI_TELL(file_rec->file);   /* get the current offset */
        HI_SEEKEND(file_rec->file);
        file_end=HI_TELL(file_rec->file);
        HI_SEEK(file_rec->file,file_off);   /* restore the previous position */
        if(data_len+data_off!=file_end) {    /* dataset at end? */
            access_rec->appendable=FALSE;
              HEreport("Tried to seek to %d (object length:  %d)", offset,
                       access_rec->block->ddlist[access_rec->idx].length);
              HRETURN_ERROR(DFE_BADSEEK,FAIL);
          } /* end if */
      } /* end if */

    /* set the new position */

    access_rec->posn = offset;

    return SUCCEED;
}   /* Hseek() */

/*--------------------------------------------------------------------------

 NAME
       Hread -- read the next segment from data element
 USAGE
       int32 Hread(access_id, length, data)
       int32 access_id;        IN: id of READ access element
       int32 length;           IN: length of segment to read in
       char *data;             OUT: pointer to data array to read to
 RETURNS
       returns length of segment actually read in if successful and FAIL
       (-1) otherwise
 DESCRIPTION
       Read in the next segment in the data element pointed to by the
       access elt.  If length is zero or larger than the remaining bytes
       of the object, read until the end of the object.

--------------------------------------------------------------------------*/
int32 Hread(int32 access_id, int32 length, VOIDP data)
{
    CONSTR(FUNC,"Hread");                /* for HERROR */
    filerec_t *file_rec;       /* file record */
    accrec_t *access_rec;      /* access record */
    dd_t *dd;                  /* current dd pointer */

    /* clear error stack and check validity of access id */

    HEclear();
    access_rec = AID2REC(access_id);
    if (access_rec == (accrec_t *) NULL || !access_rec->used || data == NULL)
        HRETURN_ERROR(DFE_ARGS,FAIL);
    
    /* special elt, so call special function */
    if (access_rec->special)
      return (*access_rec->special_func->read)(access_rec, length, data);

    /* check validity of file record */
    file_rec = FID2REC(access_rec->file_id);
    if (BADFREC(file_rec))
        HRETURN_ERROR(DFE_INTERNAL,FAIL);
    
    /* get the dd of this data elt */
    dd = &(access_rec->block->ddlist[access_rec->idx]);
    if (length < 0)
        HRETURN_ERROR(DFE_BADSEEK,FAIL);
    
    /* seek to position to start reading and read in data */
    if (HI_SEEK(file_rec->file, access_rec->posn + dd->offset) == FAIL)
        HRETURN_ERROR(DFE_SEEKERROR,FAIL);
    
    /* length == 0 means to read to end of element,
       if read length exceeds length of elt, read till end of elt */
    if (length == 0 || length + access_rec->posn > dd->length)
        length = dd->length - access_rec->posn;
    
    if (HI_READ(file_rec->file, data, length) == FAIL)
        HRETURN_ERROR(DFE_READERROR,FAIL);
    
    /* move the position of the access record */
    access_rec->posn += length;
    
    return length;

}   /* Hread */

/*--------------------------------------------------------------------------

 NAME
       Hwrite -- write next data segment to data element
 USAGE
       int32 Hwrite(access_id, len, data)
       int32 access_id;        IN: id of WRITE access element
       int32 len;              IN: length of segment to write
       const char *data;       IN: pointer to data to write
 RETURNS
       returns length of segment successfully written, FAIL (-1) otherwise
 DESCRIPTION
       Write the data to data element where the last write or Hseek()
       stopped.  If the space reserved is less than the length to write,
       then only as much as can fit is written.  It is the responsibility
       of the user to insure that no two access elements are writing to the
       same data element.  It is possible to interlace writes to more than
       one data elements in the same file though.
       Calling with length == 0 is an error.

--------------------------------------------------------------------------*/
int32 Hwrite(int32 access_id, int32 length, const VOIDP data)
{
    CONSTR(FUNC,"Hwrite");       /* for HERROR */
    filerec_t *file_rec;       /* file record */
    accrec_t *access_rec;      /* access record */
    dd_t *dd;                  /* ptr to dd of current elt */

    /* clear error stack and check validity of access id */

#ifdef TESTING
printf("Hwrite(): entering\n");
#endif
    HEclear();
    access_rec = AID2REC(access_id);
    if (access_rec == (accrec_t *) NULL || !access_rec->used
            || access_rec->access != DFACC_WRITE || data==NULL)
        HRETURN_ERROR(DFE_ARGS,FAIL);

#ifdef TESTING
printf("Hwrite(): before special element function call\n");
#endif
    /* if special elt, call special function */
    if(access_rec->special)
       return(*access_rec->special_func->write)(access_rec,length,(VOIDP)data);

    /* check validity of file record and get dd ptr */
    file_rec = FID2REC(access_rec->file_id);
    if (BADFREC(file_rec))
       HRETURN_ERROR(DFE_INTERNAL,FAIL);
    dd = &(access_rec->block->ddlist[access_rec->idx]);

    /* check validity of length and write data.
       NOTE: it is an error to attempt write past the end of the elt */
    if (length <= 0 ||
            (!access_rec->appendable && length+access_rec->posn > dd->length))
       HRETURN_ERROR(DFE_BADSEEK,FAIL);

#ifdef TESTING
printf("Hwrite(): access_id=%d, length=%d, data=%p\n",access_id, length, data);
#endif
    if(access_rec->appendable && length + access_rec->posn > dd->length) {
        int32 file_off;         /* offset in the file we are at currently */
        int32 file_end;         /* length of the file */
        int32 data_len;         /* length of the data we are checking */
        int32 data_off;         /* offset of the data we are checking */

#ifdef TESTING
printf("Hwrite(): appending to a dataset posn=%d, dd->length=%d\n",access_rec->posn, dd->length);
#endif
        /* get the offset and length of the dataset */
        data_len=access_rec->block->ddlist[access_rec->idx].length;
        data_off=access_rec->block->ddlist[access_rec->idx].offset;

        file_off=HI_TELL(file_rec->file);   /* get the current offset */
        HI_SEEKEND(file_rec->file);
        file_end=HI_TELL(file_rec->file);
        HI_SEEK(file_rec->file,file_off);   /* restore the previous position */
        if(data_len+data_off!=file_end) {    /* dataset at end? */
            access_rec->appendable=FALSE;
            HRETURN_ERROR(DFE_BADSEEK,FAIL);
          } /* end if */
        dd->length=access_rec->posn+length;   /* update the DD length */
#ifdef OLD_WAY
        access_rec->flush=TRUE; /* make certain the DD gets updated on disk */
#else
        if(HIupdate_dd(file_rec, access_rec->block, access_rec->idx,FUNC)==FAIL)
            HRETURN_ERROR(DFE_CANTFLUSH,FAIL);
#endif
#ifdef TESTING
printf("Hwrite(): appending to a dataset, ok to append\n");
#endif
      } /* end if */

    if (HI_SEEK(file_rec->file, access_rec->posn + dd->offset) == FAIL)
       HRETURN_ERROR(DFE_SEEKERROR,FAIL);
    if (HI_WRITE(file_rec->file, data, length) == FAIL)
       HRETURN_ERROR(DFE_WRITEERROR,FAIL);

    /* update position of access in elt */
    access_rec->posn += length;

#ifdef TESTING
printf("Hwrite(): successful I/O\n");
#endif
    return length;
}   /* end Hwrite */


/*--------------------------------------------------------------------------

 NAME
       HDgetc -- read a byte from data element
 USAGE
       intn HDgetc(access_id)
       int access_id;          IN: id of READ access element

 RETURNS
       returns byte read in from data if successful and FAIL
       (-1) otherwise

 DESCRIPTION
        Calls Hread() to read a single byte and reports errors.

--------------------------------------------------------------------------*/
intn HDgetc(int32 access_id)
{
    CONSTR(FUNC,"HDgetc");     /* for HERROR */
    uint8 c;                /* character read in */

    if(Hread(access_id,1,&c)==FAIL)
        HRETURN_ERROR(DFE_READERROR,FAIL);
    return((intn)c);

}   /* HDgetc */


/*--------------------------------------------------------------------------

 NAME
       HDputc -- write a byte to data element
 USAGE
       intn HDputc(c,access_id)
       uint8 c;                 IN: byte to write out
       int32 access_id;         IN: id of WRITE access element

 RETURNS
       returns byte written out to data if successful and FAIL
       (-1) otherwise

 DESCRIPTION
        Calls Hwrite() to write a single byte and reports errors.

--------------------------------------------------------------------------*/
intn HDputc(uint8 c,int32 access_id)
{
    CONSTR(FUNC,"HDputc");    /* for HERROR */

    if(Hwrite(access_id,1,&c)==FAIL)
        HRETURN_ERROR(DFE_WRITEERROR,FAIL);
    return((intn)c);
}   /* HDputc */


/*--------------------------------------------------------------------------

 NAME
       Hendaccess -- to dispose of an access element
 USAGE
       intn Hendaccess(access_id)
       int32 access_id;          IN: id of access element to dispose of
 RETURNS
       returns SUCCEED (0) if successful, FAIL (-1) otherwise
 DESCRIPTION
       Used to dispose of an access element.  If access elements are not
       disposed it will eventually become impossible to allocate new
       ones and close the file.

       If there are active aids Hclose will *NOT* close the file.  This
       is a very common problem when developing new code.

--------------------------------------------------------------------------*/
intn Hendaccess(int32 access_id)
{
    CONSTR(FUNC,"Hendaccess");   /* for HERROR */
    filerec_t *file_rec;       /* file record */
    accrec_t *access_rec;      /* access record */

    /* check validity of access id */
    access_rec = AID2REC(access_id);
    if (!access_rec || !access_rec->used)
        HRETURN_ERROR(DFE_ARGS,FAIL);
    
    /* if special elt, call special function */
    if (access_rec->special)
        return (*access_rec->special_func->endaccess)(access_rec);
    
    /* check validity of file record */
    file_rec = FID2REC(access_rec->file_id);
    if (BADFREC(file_rec))
        HRETURN_ERROR(DFE_INTERNAL,FAIL);
    
    /* update file and access records */
    if(access_rec->flush) { /* check whether to flush the assoc. DD */
#ifdef OLD_WAY
        if(HIupdate_dd(file_rec, access_rec->block, access_rec->idx,FUNC)==FAIL)
            HRETURN_ERROR(DFE_CANTFLUSH,FAIL);
#endif
    } 
    
    file_rec->attach--;
    access_rec->used = FALSE;

    return SUCCEED;
}   /* Hendaccess */


/*--------------------------------------------------------------------------

 NAME
       Hgetelement -- read in a data element
 USAGE
       int32 Hgetelement(file_id, tag, ref, data)
       int32 file_id;          IN: id of the file to read from
       int16 tag;              IN: tag of data element to read
       int16 ref;              IN: ref of data element to read
       char *data;             OUT: buffer to read into
 RETURNS
       returns  number of bytes read if successful, FAIL (-1)
       otherwise
 DESCRIPTION
       Read in a data element from a HDF file and puts it into buffer
       pointed to by data.  The space allocated for buffer is assumed to
       be large enough.

--------------------------------------------------------------------------*/
int32 Hgetelement(int32 file_id, uint16 tag, uint16 ref, uint8 *data)
{
    CONSTR(FUNC,"Hgetelement");  /* for HERROR */
    int32 access_id;           /* access record id */
    int32 length;              /* length of this elt */

    /* clear error stack */
    HEclear();

    /* get the access record, get the length of the elt, read in data,
       and dispose of access record */

    access_id = Hstartread(file_id, tag, ref);
    if (access_id == FAIL)
       HRETURN_ERROR(DFE_NOMATCH,FAIL);

    if ((length = Hread(access_id, (int32) 0, data)) == FAIL)
       HERROR(DFE_READERROR);

    Hendaccess(access_id);

    return(length);
}   /* Hgetelement() */

/*--------------------------------------------------------------------------

 NAME
       Hputelement -- writes a data element
 USAGE
       int Hputelement(fileid, tag, ref, data, length)
       int32 fileid;             IN: id of file
       int16 tag;                IN: tag of data element to put
       int16 ref;                IN: ref of data element to put
       char *data;               IN: pointer to buffer
       int32 length;             IN: length of data
 RETURNS
       returns length of bytes written if successful and FAIL (-1) 
       otherwise
 DESCRIPTION
       Writes a data element or replaces an existing data element 
       in an HDF file.  Uses Hwrite and its associated routines.

--------------------------------------------------------------------------*/
int32 Hputelement(int32 file_id, uint16 tag, uint16 ref, uint8 *data,
               int32 length)
{
    CONSTR(FUNC,"Hputelement");  /* for HERROR */
    int32 access_id;           /* access record id */
    int32 ret;                 /* return code */

    /* clear error stack */
    HEclear();

    /* get access record, write out data and dispose of access record */
    access_id = Hstartwrite(file_id, (uint16)tag, (uint16)ref, length);
    if (access_id == FAIL)
       HRETURN_ERROR(DFE_NOMATCH,FAIL);

    if ((ret = Hwrite(access_id, length, data)) == FAIL)
        HERROR(DFE_WRITEERROR);
    Hendaccess(access_id);

    return (ret);
}   /* end Hputelement() */


/*--------------------------------------------------------------------------

 NAME
       Hlength -- returns length of a data element
 USAGE
       int32 Hlength(fileid, tag, ref)
       int fileid;             IN: id of file
       int tag;                IN: tag of data element
       int ref;                IN: ref of data element
 RETURNS
       return the length of a data element or FAIL if there is a problem.
 DESCRIPTION
       returns length of data element if it is present in the file.  
       Return FAIL (-1) if it is not in the file or an error occurs.

       The current implementation is probably less efficient than it 
       could be.  However, because of special elements the code is much 
       cleaner this way.

--------------------------------------------------------------------------*/
int32 Hlength(int32 file_id, uint16 tag, uint16 ref)
{
    CONSTR(FUNC,"Hlength");      /* for HERROR */
    int32 access_id;           /* access record id */
    int32 length;              /* length of elt inquired */
    int ret;                   /* return code */

    /* clear error stack */
    HEclear();

    /* get access record, inquire about lebngth and then dispose of
       access record */
    access_id = Hstartread(file_id, tag, ref);
    if (access_id == FAIL)
       HRETURN_ERROR(DFE_ARGS,FAIL);

    if ((ret = HQuerylength(access_id, &length)) == FAIL)
       HERROR(DFE_INTERNAL);
    Hendaccess(access_id);

    return length;

}   /* end Hlength */


/*--------------------------------------------------------------------------

 NAME
       Hoffset -- get offset of data element in the file
 USAGE
       int32 Hoffset(fileid, tag, ref)
       int32 fileid;           IN: id of file
       uint16 tag;             IN: tag of data element
       uint16 ref;             IN: ref of data element
 RETURNS
       returns offset of data element if it is present in the 
       file or FAIL (-1) if it is not.  

 DESCRIPTION 
       This should be used for debugging purposes only since
       the user should not have to know the actual offset of 
       a data element in a file.

       Like Hlength().  This could be sped up by not going through 
       Hstartread() but because of special elements it is easier 
       this way

--------------------------------------------------------------------------*/
int32 Hoffset(int32 file_id, uint16 tag, uint16 ref)
{
    CONSTR(FUNC,"Hoffset");      /* for HERROR */
    int32 access_id;           /* access record id */
    int32 offset;              /* offset of elt inquired */
    int ret;                   /* return code */

    /* clear error stack */
    HEclear();

    /* get access record, inquire offset, and dispose of access record */
    access_id = Hstartread(file_id, tag, ref);
    if (access_id == FAIL)
        HRETURN_ERROR(DFE_ARGS,FAIL);

    if ((ret = HQueryoffset(access_id, &offset)) == FAIL)
        HERROR(DFE_INTERNAL);
    Hendaccess(access_id);

    return offset;

}   /* Hoffset */


/*--------------------------------------------------------------------------

 NAME
       Hdupdd -- duplicate a data descriptor
 USAGE
       intn Hdupdd(file_id, tag, ref, old_tag, old_ref)
       int32 file_id;          IN: id of file
       uint16 tag;             IN: tag of new data descriptor
       uint16 ref;             IN: ref of new data descriptor
       uint16 old_tag;         IN: tag of data descriptor to duplicate
       uint16 old_ref;         IN: ref of data descriptor to duplicate
 RETURNS
       returns SUCCEED (0) if successful, FAIL (-1) otherwise
 DESCRIPTION
       Duplicates a data descriptor so that the new tag/ref points to the
       same data element pointed to by the old tag/ref.  Return FAIL if
       the given tag/ref are already in use.

--------------------------------------------------------------------------*/
intn Hdupdd(int32 file_id, uint16 tag, uint16 ref,
          uint16 old_tag, uint16 old_ref)
{
    CONSTR(FUNC,"Hdupdd");       /* for HERROR */
    filerec_t *file_rec;       /* file record */
    ddblock_t *block;          /* dd block fo old dd */
    ddblock_t *new_block;      /* dd block of new dd */
    int32 idx;                 /* index into dd list for old dd */
    int32 new_idx;             /* index into dd list for new dd */

    /* clear error stack and check validity of file id */

    HEclear();
    file_rec = FID2REC(file_id);
    if (BADFREC(file_rec))
       HRETURN_ERROR(DFE_ARGS,FAIL);

    /* look for old dd and new dd in file record */
    new_block = block = file_rec->ddhead;
    new_idx = idx = -1;
    if (HIlookup_dd(file_rec, old_tag, old_ref, &block, &idx) == FAIL)
       HRETURN_ERROR(DFE_NOMATCH,FAIL);

    /* dd already exist, cannot modify */
    if (FAIL != HIlookup_dd(file_rec, tag, ref, &new_block, &new_idx))
        HRETURN_ERROR(DFE_DUPDD,FAIL);

    /* look for empty dd to put new dd */
    new_block = file_rec->ddhead;
    new_idx = -1;
    if (HIlookup_dd(file_rec, (uint16)DFTAG_NULL, (uint16)DFTAG_WILDCARD,
                    &file_rec->null_block, &file_rec->null_idx) == FAIL) {
        if (HInew_dd_block(file_rec, FILE_NDDS(file_rec), FUNC) == FAIL) 
            HRETURN_ERROR(DFE_NOFREEDD,FAIL)
	else {
            new_block = file_rec->ddlast;
            new_idx = 0;
          } /* end else */
      } /* end if */
    else {
        new_block = file_rec->null_block;
        new_idx   = file_rec->null_idx;
      } /* end else */
    
    /* fill in the new dd with details from old dd and update file with
       new dd */

    new_block->ddlist[new_idx].tag = tag;
    new_block->ddlist[new_idx].ref = ref;
    new_block->ddlist[new_idx].offset = block->ddlist[idx].offset;
    new_block->ddlist[new_idx].length = block->ddlist[idx].length;

    /* add the new thing to the hash table */
    if(HIadd_hash_dd(file_rec, tag, ref,  new_block, new_idx))
       HRETURN_ERROR(DFE_CANTHASH,FAIL);

    return HIupdate_dd(file_rec, new_block, new_idx, FUNC);
}   /* end Hdupdd */


/*--------------------------------------------------------------------------

 NAME
       HIupdate_dd - write an updated dd to the file
 USAGE
       int HIupdate_dd(file_id, block, idx, FUNC)
       int32 file_id;          IN: id of file
       ddblock_t * block       IN: ddblock that this dd is in
       int32 index;            IN: index of dd to update
       char * FUNC;            IN: the function that is asking for this
 RETURNS
       returns SUCCEED (0) if successful, FAIL (-1) otherwise
 DESCRIPTION
       Flushes updated dd information out to the file
       Even though this is an HI routine it is not private so that
       special element functions can call it.

--------------------------------------------------------------------------*/
int HIupdate_dd(filerec_t *file_rec, ddblock_t *block, int32 idx, char *FUNC)
{
    int32 offset;              /* offset of updated dd in file */
    uint8 *p;                  /* temp buffer ptr */

#ifdef QAK
printf("HIupdate_dd(): file_rec->cache=%d, file_rec->dirty=%d\n",file_rec->cache,file_rec->dirty);
#endif
if(file_rec->cache) {  /* if caching is on, postpone update until later */
    file_rec->dirty|=DDLIST_DIRTY;
    block->dirty=TRUE;
  } /* end if */
else {
    /* look for offset of updated dd block in the file */
#ifdef OLD_WAY
    if(block==file_rec->ddhead)      /* updated ddblock is the first one */
        offset=MAGICLEN+NDDS_SZ+OFFSET_SZ+(idx*DD_SZ);
    else
        offset = block->prev->nextoffset + NDDS_SZ + OFFSET_SZ + (idx * DD_SZ);
#else
    offset=block->myoffset+(NDDS_SZ+OFFSET_SZ)+(idx*DD_SZ);
#endif /* end OLD_WAY */
    
    /* write in the updated dd */
    if (HI_SEEK(file_rec->file, offset) == FAIL)
        HRETURN_ERROR(DFE_SEEKERROR,FAIL);
    
#ifdef OLD_WAY
    /* Check if temproray buffer has been allocated */
    if(ptbuf == NULL) 
        if((ptbuf=(uint8 *)HDgetspace(TBUF_SZ * sizeof(uint8))) == NULL)
            HRETURN_ERROR(DFE_NOSPACE, FAIL);
#endif
    
    p = ptbuf;
    UINT16ENCODE(p, block->ddlist[idx].tag);
    UINT16ENCODE(p, block->ddlist[idx].ref);
    INT32ENCODE(p, block->ddlist[idx].offset);
    INT32ENCODE(p, block->ddlist[idx].length);
    if (HI_WRITE(file_rec->file, ptbuf, DD_SZ) == FAIL)
        HRETURN_ERROR(DFE_WRITEERROR,FAIL);
  } /* end else */

    /* check whether to incr. offset of end of file */
    /* not certain whether this is actually necessary, but better safe than */
    /* sorry later... -QAK */
    if((block->ddlist[idx].offset+block->ddlist[idx].length)
	    >file_rec->f_end_off)
        file_rec->f_end_off=block->ddlist[idx].offset+block->ddlist[idx].length;
    
    return SUCCEED;
} /* HIupdate_dd */


/*--------------------------------------------------------------------------

 NAME
       Hdeldd -- delete a data descriptor
 USAGE
       intn Hdeldd(file_id, tag, ref)
       int32 file_id;            IN: id of file
       int16 tag;                IN: tag of data descriptor to delete
       int16 ref;                IN: ref of data descriptor to delete
 RETURNS
       returns SUCCEED (0) if successful, FAIL (-1) otherwise
 DESCRIPTION
       Deletes a data descriptor of tag/ref from the dd list of the file.
       This routine is unsafe and may leave a file in a condition that is
       not usable by some routines.  Use with care.
       For example, if this element is contained in a Vgroup, that group
       will *NOT* get updated to reflect that this element has been deleted.

--------------------------------------------------------------------------*/
intn Hdeldd(int32 file_id, uint16 tag, uint16 ref)
{
    CONSTR(FUNC,"Hdeldd");       /* for HERROR */
    filerec_t *file_rec;       /* file record */
    ddblock_t *block;          /* dd block of deleted dd */
    int32 idx;                 /* dd list index of deleted dd */

    /* clear error stack and check validity of file record id */

    HEclear();
    file_rec = FID2REC(file_id);
    if (BADFREC(file_rec) || tag == DFTAG_WILDCARD || ref == DFREF_WILDCARD)
       HRETURN_ERROR(DFE_ARGS,FAIL);

    /* look for the deleted dd */
    if (HIlookup_dd(file_rec, tag, ref, &block, &idx) == FAIL)
       HRETURN_ERROR(DFE_NOMATCH,FAIL);

    /* this may have thrown off our ending markers, reset to beginning */
    file_rec->null_block = file_rec->ddhead;
    file_rec->null_idx   = -1;

    /* mark the dd as empty and then update the file */
    block->ddlist[idx].tag = DFTAG_NULL;

    /* remove it from the hash table */
    if(HIdel_hash_dd(file_rec, tag, ref) == FAIL)
       HRETURN_ERROR(DFE_CANTDELHASH,FAIL);

    if(HPfreediskblock(file_rec, block->ddlist[idx].offset, block->ddlist[idx].length)==FAIL)
       HRETURN_ERROR(DFE_INTERNAL,FAIL);

    return HIupdate_dd(file_rec, block, idx, FUNC);

}   /* end Hdeldd */


/*--------------------------------------------------------------------------

 NAME
       Hnewref -- returns a ref that is guaranteed to be unique in the file
 USAGE
       uint16 Hnewref(file_id)
       int32 file_id;          IN: id of file
 RETURNS
       returns the ref number, 0 otherwise
 DESCRIPTION
       Returns a ref number that can be used with any tag to produce a
       unique tag/ref.  Successive calls to Hnewref will generate a
       strictly increasing sequence until the highest possible ref had been
       returned, then Hnewref will return unused ref's starting from 1.

--------------------------------------------------------------------------*/
uint16 Hnewref(int32 file_id)
{
    CONSTR(FUNC,"Hnewref");      /* for HERROR */
    filerec_t *file_rec;       /* file record */
    uint16 ref;                        /* the new ref */

    /* clear error stack and check validity of file record id */

    HEclear();
    file_rec = FID2REC(file_id);
    if (BADFREC(file_rec))
       HRETURN_ERROR(DFE_ARGS,0);

    /* if maxref of this file is still below the maximum,
       just return next number */
    if (file_rec->maxref < MAX_REF)
       return ++(file_rec->maxref);

    /* otherwise, search for an empty ref */

    /* incredibly slow but unlikely situation */
    for (ref = 1; ref < MAX_REF; ref++) {
       ddblock_t *bl;
       int32 idx;

       bl = file_rec->ddhead;
       idx = -1;
       if (HIfind_dd((uint16)DFTAG_WILDCARD, ref, &bl, &idx, DF_FORWARD) == FAIL)
           return ref;
    }

    return 0;
} /* Hnewref */


/*--------------------------------------------------------------------------

 NAME
       Hishdf -- tells if a file is an HDF file
 USAGE
       intn Hishdf(path)
       const char *path;             IN: name of file
 RETURNS
       returns TRUE (non-zero) if file is HDF, FALSE (0) otherwise
 DESCRIPTION
       This user level routine can be used to determine if a file
       with a given name is an HDF file.  Note, just because a file
       is not an HDF file does not imply that all HDF library
       functions can not work on it.

--------------------------------------------------------------------------*/

intn
Hishdf(const char *filename)
{
    CONSTR(FUNC,"Hishdf");

#if defined(VMS) || defined(MAC) || (defined(PC) & !defined(UNIX386))
  
    int32 fid;

    fid = Hopen(filename, DFACC_READ, 0);
    if(fid == FAIL)
        return FALSE;

    Hclose(fid);
    return TRUE;

#else

    intn ret;
    hdf_file_t fp;
  
    fp = HI_OPEN(filename, DFACC_READ);
    if (OPENERR(fp)) {
        return(FALSE);
    } else {
        ret = HIvalid_magic(fp, FUNC);
        HI_CLOSE(fp);
        return((int)ret);
    }
#endif
} /* Hishdf */


/*--------------------------------------------------------------------------

 NAME
       Htrunc -- truncate a data element to a length
 USAGE
       int32 Htrunc(aid, len)
       int32 aid;             IN: id of file
       int32 len;             IN: length at which to truncate data element
 RETURNS
       return the length of a data element
 DESCRIPTION
       truncates a data element in the file.  Return
       FAIL (-1) if it is not in the file or an error occurs.

--------------------------------------------------------------------------*/
int32 Htrunc(int32 aid, int32 trunc_len)
{
    CONSTR(FUNC,"Htrunc");       /* for HERROR */
    accrec_t *access_rec;      /* access record */

    /* clear error stack and check validity of access id */

    HEclear();
    access_rec = AID2REC(aid);
    if (access_rec == (accrec_t *) NULL || !access_rec->used
            || access_rec->access != DFACC_WRITE)
        HRETURN_ERROR(DFE_ARGS,FAIL);

    /* Dunno about truncating special elements... -QAK */
#ifdef OLD_WAY
    /* if special elt, call special function */
    if (access_rec->special)
       return (*access_rec->special_func->write)(access_rec, length, data);
#endif

    /* check for actually being able to truncate the data */
    if(access_rec->block->ddlist[access_rec->idx].length>trunc_len) {
        access_rec->block->ddlist[access_rec->idx].length=trunc_len;
        if(access_rec->posn>trunc_len)  /* move the seek position back */
            access_rec->posn=trunc_len;
        access_rec->flush=TRUE; /* make certain the DD gets updated on disk */
        return(trunc_len);
      } /* end if */
    else
        HRETURN_ERROR(DFE_BADLEN,FAIL);
}   /* end Htrunc() */


/*--------------------------------------------------------------------------
 NAME
       Hsync -- sync file with memory
 USAGE
       intn Hsync(file_id)
       int32 file_id;            IN: id of file
 RETURNS
       returns SUCCEED (0) if sucessful, FAIL (-1) otherwise
 DESCRIPTION
       Currently, the on-disk and in-memory representations are always
       the same.  Thus there is no real use for Hsync().  In the future,
       things may be buffered before being written out at which time
       Hsync() will be useful to sync up the on-disk representation.
 NOTE
       First tests of caching DD's until close.

--------------------------------------------------------------------------*/
intn Hsync(int32 file_id)
{
    CONSTR(FUNC,"Hsync");       /* for HERROR */
    filerec_t *file_rec;       /* file record */

    /* check validity of file record and get dd ptr */
    file_rec=FID2REC(file_id);
    if(BADFREC(file_rec))
        HRETURN_ERROR(DFE_INTERNAL,FAIL);

    /* check whether to flush the file info */
    if(file_rec->cache && file_rec->dirty) {
        /* flush DD blocks if necessary */
        if(file_rec->dirty&DDLIST_DIRTY)
            if(HIflush_dds(file_rec)==FAIL)
                HRETURN_ERROR(DFE_CANTFLUSH,FAIL);

        /* extend the end of the file if necessary */
        if(file_rec->dirty&FILE_END_DIRTY) 
            if(HIextend_file(file_rec)==FAIL)
                HRETURN_ERROR(DFE_CANTFLUSH,FAIL);
	file_rec->dirty=0; /* file doesn't need to be flushed now */
      } /* end if */

    return(SUCCEED);
} /* Hsync */


/*--------------------------------------------------------------------------
 NAME
       Hcache -- set low-level caching for a file
 USAGE
       intn Hcache(file_id,cache_on)
           int32 file_id;            IN: id of file
           intn cache_on;            IN: whether to cache or not
 RETURNS
       returns SUCCEED (0) if sucessful, FAIL (-1) otherwise
 DESCRIPTION
       Set/reset the caching in an HDF file.
--------------------------------------------------------------------------*/
intn Hcache(int32 file_id,intn cache_on)
{
    CONSTR(FUNC,"Hcache");     /* for HERROR */
    filerec_t *file_rec;       /* file record */

    /* check validity of file record and get dd ptr */
    file_rec=FID2REC(file_id);
    if(BADFREC(file_rec))
        HRETURN_ERROR(DFE_INTERNAL,FAIL);

    /* check whether to flush the file info */
    if(cache_on==FALSE && (file_rec->cache && file_rec->dirty)) {
        /* flush DD blocks if necessary */
        if(file_rec->dirty&DDLIST_DIRTY)
            if(HIflush_dds(file_rec)==FAIL)
                HRETURN_ERROR(DFE_CANTFLUSH,FAIL);

        /* extend the end of the file if necessary */
        if(file_rec->dirty&FILE_END_DIRTY) 
            if(HIextend_file(file_rec)==FAIL)
                HRETURN_ERROR(DFE_CANTFLUSH,FAIL);
	file_rec->dirty=0; /* file doesn't need to be flushed now */
      } /* end if */
    file_rec->cache=(cache_on!=0 ? TRUE : FALSE);

    return(SUCCEED);
} /* Hcache */


/*--------------------------------------------------------------------------
 NAME
       HDvalidfid -- check if a file ID is valid
 USAGE
       int HDvalidfid(file_id)
       int32 file_id;            IN: id of file
 RETURNS
       returns TRUE if valid ID else FALSE
 DESCRIPTION
       Determine whether a given int32 is a valid HDF file ID or not

--------------------------------------------------------------------------*/
intn HDvalidfid(int32 file_id)
{
    filerec_t *file_rec = FID2REC(file_id);
    if (BADFREC(file_rec))
       return FALSE;
    else
       return TRUE;
} /* HDvalidfid */


/*--------------------------------------------------------------------------
 HDerr

 Closes a file and return FAIL.  Replacement for DFIerr in HDF3.1 and before
--------------------------------------------------------------------------*/
int HDerr(int32 file_id)
{
    Hclose(file_id);
    return FAIL;
}


/*--------------------------------------------------------------------------

 NAME
       Hsetacceesstype -- set the I/O access type (serial, parallel, ...)
       of a data element
 USAGE
       intn Hsetacceesstype(access_id, accesstype)
       int32 access_id;        IN: id of access element
       uintn accesstype;       IN: I/O access type
 RETURNS
       returns FAIL (-1) if fail, SUCCEED (0) otherwise.
 DESCRIPTION
       Set the type of I/O for accessing the data element to
       accesstype.

--------------------------------------------------------------------------*/

#ifdef CM5
extern CM_DEBUG;
#endif

intn Hsetaccesstype(int32 access_id, uintn accesstype)
{
    CONSTR(FUNC,"Hsetaccesstype");       /* for HERROR */
    accrec_t *access_rec;      /* access record */

    /* clear error stack and check validity of this access id */
    HEclear();

    access_rec = AID2REC(access_id);
    if (access_rec == (accrec_t *) NULL || !access_rec->used)
       HRETURN_ERROR(DFE_ARGS,FAIL);
    if (accesstype != DFACC_DEFAULT && accesstype != DFACC_SERIAL &&
	    accesstype != DFACC_PARALLEL)
       HRETURN_ERROR(DFE_ARGS,FAIL);

    if (accesstype == access_rec->access_type)
	return SUCCEED;

    /* kludge mode on */
    if (accesstype != DFACC_PARALLEL)	/* go to PARALLEL only */
	return FAIL;
    if(access_rec->flush)
	printf("need to flush access_rec\n");
#ifdef CM5
if (CM_DEBUG > 0){
    printf("need to close old access\n");
printf("old access_type is %d\n", access_rec->access_type);
printf("new access_type is %d\n", accesstype);
}
    access_rec->access_type = accesstype;
#endif
    /* if special elt, call special function */
    if (access_rec->special)
    	return(HXPsetaccesstype(access_rec));
    return(SUCCEED);
}   /* Hsetacceesstype() */

/*==========================================================================

  Internal Routines 

==========================================================================*/

#ifdef DELETE_FOR_40_RELEASE_IF_NOT_USED
/*--------------------------------------------------------------------------
 NAME
       HIchangedd -- update the internal contents of an AID
 USAGE
       int HIchangedd(datadd, block, idx, special, special_info, funcs)
       dd_t       * datadd;         IN: pointer to dd record for this object
       ddblock_t  * block;          IN: block this object resides in
       int          idx;            IN: index into above block
       int16        special;        IN: type of special element
       VOIDP        special_info;   IN: special element specific information
       funclist_t * special_func;   IN: list of functions for special element
                                        handling
 RETURNS
       Not sure really.
 DESCRIPTION
       This routine is mainly used to promote a dd into a special 
       element dd.  It updates the internal tables and specialness
       for the given object.

--------------------------------------------------------------------------*/
PRIVATE int HIchangedd(dd_t *datadd, ddblock_t *block, int idx, int16 special,
              VOIDP special_info, funclist_t *special_func)
{
    int i;                     /* temp index */
    int attached = 0;          /* number of accesses attached to this dd */

    /* go through access records to look for converted dd,
       and then update the matching records */

    for (i=0; i<MAX_FILE; i++)
       if (access_records[i].used) {
           dd_t *tdd =         /* ptr to dd of current access record */
               &access_records[i].block->ddlist[access_records[i].idx];
           if (tdd == datadd) {
               access_records[i].block = block;
               access_records[i].idx = idx;
               access_records[i].special = special;
               access_records[i].special_func = special_func;
               access_records[i].special_info = special_info;
               attached++;
           }
       }

    return attached;
}
#endif


/*--------------------------------------------------------------------------
 NAME
       HIinit_file_dds -- Initialize DD blocks for a new file
 USAGE
       int HIinit_file_dds(file_rec, ndds, FUNC)
       filerec_t  * file_rec        IN: pointer to file structure to initialize
       int16        ndds;           IN: number of DDs to be put in DDblocks
       char       * FUNC;           IN: function we are creating the file for
 RETURNS
       SUCCEED / FAIL
 DESCRIPTION
       Note, this routine no longer messes with the version tag
       stuff.  That is now all handled in Hopen() upon file creation.

       This routine sets up the in memory and on-disk datastructures
       for the first DDblock in a newly created file

--------------------------------------------------------------------------*/
PRIVATE int HIinit_file_dds(filerec_t *file_rec, int16 ndds, char *FUNC)
{
    ddblock_t *block;          /* dd block to intialize */
    uint8 *p;                  /* temp buffer ptr */
    dd_t *list;                /* list of dd */
    register int i;            /* temp ints */
    register int16 n;

    /* 'reasonablize' the value of ndds.  0 means use default */
    if (0 == ndds)
       ndds = DEF_NDDS;
    else if (ndds < MIN_NDDS)
       ndds = MIN_NDDS;

    /* allocate the dd block in memory and initialize it */
    file_rec->ddhead = (ddblock_t *) HDgetspace(sizeof(ddblock_t));
    if (file_rec->ddhead == (ddblock_t *) NULL)
        HRETURN_ERROR(DFE_NOSPACE,FAIL);
    block = file_rec->ddlast = file_rec->ddhead;
    block->prev = (ddblock_t *) NULL;
    block->ndds = ndds;
    block->next = (ddblock_t *) NULL;
    block->nextoffset = 0;
    block->myoffset=MAGICLEN;
    block->dirty=FALSE;

#ifdef OLD_WAY
    /* Check if temproray buffer has been allocated */
    if (ptbuf == NULL)  {
        ptbuf = (uint8 *)HDgetspace(TBUF_SZ * sizeof(uint8));
        if (ptbuf == NULL)
            HRETURN_ERROR(DFE_NOSPACE, FAIL);
    }
#endif
    
    /* write first dd block to file */
    p = ptbuf;
    INT16ENCODE(p, block->ndds);
    INT32ENCODE(p, (int32) 0);
    if (HI_WRITE(file_rec->file, ptbuf, NDDS_SZ+OFFSET_SZ) == FAIL) 
        HRETURN_ERROR(DFE_WRITEERROR,FAIL);

    /* allocate and initialize dd list */
    list = block->ddlist = (dd_t *) HDgetspace((uint32) ndds * DD_SZ);
    if (list == (dd_t *) NULL)
        HRETURN_ERROR(DFE_NOSPACE,FAIL);
    for (i = 0; i < ndds; i++) {
        list[i].tag = DFTAG_NULL;
        list[i].ref = 0;
        list[i].length = list[i].offset = 0;
    }
    
    /* write dd list to file */
    
    /* n is the maximum number of dd's in tbuf */
    n = TBUF_SZ / DD_SZ;
    if (n > ndds) 
	n = ndds;
    p = ptbuf;

    for (i = 0; i < n; i++) {
        UINT16ENCODE(p, (uint16)DFTAG_NULL);
        UINT16ENCODE(p, (uint16)0);
        INT32ENCODE(p, (int32)0);
        INT32ENCODE(p, (int32)0);
    }
    while (ndds > 0) {
        if (HI_WRITE(file_rec->file, ptbuf, n*DD_SZ) == FAIL)
            HRETURN_ERROR(DFE_WRITEERROR,FAIL);
        ndds -= n;
        if (n > ndds) 
	    n = ndds;
    }
    
    /* set the end of the file currently to the end of the first DD block */
    file_rec->f_end_off=block->myoffset+(NDDS_SZ+OFFSET_SZ)+(block->ndds*DD_SZ);

    /* no dd's yet, so maximum ref is 0 */
    file_rec->maxref = 0;

    /* nothing in the hash table yet */
    HDmemset(file_rec->hash, 0, sizeof(tag_ref_list_ptr) * (HASH_MASK+1));

    return SUCCEED;
} /* HIinit_file_dds */


/*--------------------------------------------------------------------------
 NAME
       HIflush_dds -- flush changed DD blocks to file
 USAGE
       int HIflush_dds(file_rec)
           filerec_t  * file_rec        IN: pointer to file structure to flush
 RETURNS
       SUCCEED / FAIL
 DESCRIPTION
       This routine syncronizes the in memory and on-disk datastructures
       for the all DD blocks in a file.
--------------------------------------------------------------------------*/
PRIVATE intn HIflush_dds(filerec_t *file_rec)
{
    CONSTR(FUNC,"HIflush_dds");       /* for HERROR */
    ddblock_t *block; /* dd block to intialize */
    uint8 *p;         /* temp buffer ptr */
    dd_t *list;       /* list of dd */
    int16 ndds;       /* # of DDs per block */
    int i;            /* temp ints */
    int16 n;

    block=file_rec->ddhead;
    if(block==NULL) /* check for DD list */
       HRETURN_ERROR(DFE_BADDDLIST,FAIL);

    while(block!=NULL) { /* check all the blocks for flushing */
	if(block->dirty==TRUE) { /* flush this block? */
            if(HI_SEEK(file_rec->file,block->myoffset)==FAIL) 
                HRETURN_ERROR(DFE_SEEKERROR,FAIL);

            /* write dd block header to file */
            p = ptbuf;
            INT16ENCODE(p,block->ndds);
            INT32ENCODE(p,block->nextoffset);
            if(HI_WRITE(file_rec->file,ptbuf,NDDS_SZ+OFFSET_SZ)==FAIL) 
                HRETURN_ERROR(DFE_WRITEERROR,FAIL);

	    /* n is the maximum number of dd's in tbuf */
	    ndds=block->ndds;
            n=TBUF_SZ/DD_SZ;
            if(n>ndds) 
	        n=ndds;

            /* write dd list to file */
	    list=&block->ddlist[0]; /* start at the first DD, go from there */
            while(ndds>0) {
                p=ptbuf;
                for(i=0; i<n; i++,list++) {
                    UINT16ENCODE(p, list->tag);
                    UINT16ENCODE(p, list->ref);
                    INT32ENCODE(p, list->length);
                    INT32ENCODE(p, list->offset);
                  } /* end for */

                if(HI_WRITE(file_rec->file,ptbuf,n*DD_SZ)==FAIL)
                    HRETURN_ERROR(DFE_WRITEERROR,FAIL);

                ndds-=n;
                if(n>ndds) 
	            n=ndds;
              } /* end while */
	    block->dirty=FALSE; /* block has been flushed */
	  } /* end if */
	block=block->next; /* advance to next block for file */
      } /* end while */

    return(SUCCEED);
} /* HIflush_dds */


/*--------------------------------------------------------------------------
 NAME
       HIextend_file -- extend file to current length
 USAGE
       int HIextend_file(file_rec)
           filerec_t  * file_rec        IN: pointer to file structure to extend
 RETURNS
       SUCCEED / FAIL
 DESCRIPTION
       The routine extends an HDF file to be the length on the f_end_off
       member of the file_rec.  This is mainly written as a function so that
       the functionality if localized.
--------------------------------------------------------------------------*/
PRIVATE intn HIextend_file(filerec_t *file_rec)
{
    CONSTR(FUNC,"HIextend_file");       /* for HERROR */
    uint8 temp=0;

    if(HI_SEEK(file_rec->file,file_rec->f_end_off)==FAIL)
        HRETURN_ERROR(DFE_SEEKERROR,FAIL);
    if(HI_WRITE(file_rec->file,&temp,1)==FAIL)
        HRETURN_ERROR(DFE_WRITEERROR,FAIL);

    return(SUCCEED);
} /* HIextend_file */


/*--------------------------------------------------------------------------
 NAME
       HIget_function_table -- create special function table
 USAGE
       int HIget_func_table(access_rec, FUNC)
       accrec_t * access_rec;     IN: access record we are working on
       char     * FUNC;           IN: function we are working for
 RETURNS
       NULL no matter what (seems odd....)
 DESCRIPTION
       Set up the table of special functions for a given special element

--------------------------------------------------------------------------*/
PRIVATE funclist_t *HIget_function_table(accrec_t *access_rec, char *FUNC)
{
    dd_t *dd;                  /* ptr to current dd */
    filerec_t *file_rec;       /* file record */

#ifdef OLD_WAY
    /* Check if temproray buffer has been allocated */
    if (ptbuf == NULL) {
        ptbuf = (uint8 *)HDgetspace(TBUF_SZ * sizeof(uint8));
        if (ptbuf == NULL)
            HRETURN_ERROR(DFE_NOSPACE, NULL);
    }
#endif

    /* read in the special code in the special elt */

    dd = &access_rec->block->ddlist[access_rec->idx];
    file_rec = FID2REC(access_rec->file_id);
    if (HI_SEEK(file_rec->file, dd->offset) == FAIL)
       HRETURN_ERROR(DFE_SEEKERROR,NULL);
    if (HI_READ(file_rec->file, ptbuf, 2) == FAIL)
       HRETURN_ERROR(DFE_READERROR,NULL);

    /* using special code, look up function table in associative table */
    {
        register int i;
        uint8 *p;
        
        p = ptbuf;
        INT16DECODE(p, access_rec->special);
        for (i=0; functab[i].key != 0; i++) {
            if (access_rec->special == functab[i].key)
                return functab[i].tab;
        }
    }
    
    return NULL;

} /* HIget_function_table */


/*--------------------------------------------------------------------------
 NAME
       HIgetspinfo -- return special info
 USAGE
       int HIget_func_table(access_rec, tag, ref)
       accrec_t * access_rec;     IN: access record we are working on
       int16      tag;            IN: tag to look for
       int16      ref;            IN: ref to look for
 RETURNS
       special_info field or NULL if not found
 DESCRIPTION
       given the tag and ref of a given element return the special
       info field of the access element.  

       Basically, this function checks if any other AIDs in the file
       have read in the special information for this object.  If so,
       this special information will be reused.  Otherwise, the 
       special element handling code needs to read in the information
       from disk
  GLOBALS
       Reads from the global access_records

--------------------------------------------------------------------------*/
VOIDP HIgetspinfo(accrec_t *access_rec, uint16 tag, uint16 ref)
{
    register int i;            /* temp index */

    /* search access records for the matching dd,
       and return special information */

    for (i = 0; i < MAX_ACC; i++)
        if (access_records + i != access_rec
            && access_records[i].used
            && access_records[i].file_id == access_rec->file_id
            && access_records[i].block->ddlist[access_records[i].idx].tag == tag
            && access_records[i].block->ddlist[access_records[i].idx].ref == ref)

           return (VOIDP) access_records[i].special_info;

    return NULL;

} /* HIgetspinfo */

#ifdef DELETE_FOR_40_RELEASE_IF_NOT_USED
/*--------------------------------------------------------------------------
 HIlock

 lock a file record.  This is used by special functions to prevent
 losing files that are still accessed
--------------------------------------------------------------------------*/
PRIVATE int HIlock(int32 file_id)
{
    CONSTR(FUNC,"HIlock");       /* for HERROR */

    /* get file record and check validity */
    filerec_t *file_rec=FID2REC(file_id);
    if (BADFREC(file_rec))
        HRETURN_ERROR(DFE_ARGS,FAIL);

    /* lock the file record */
    file_rec->attach++;

    return SUCCEED;
}
#endif

/*--------------------------------------------------------------------------
 HIunlock

 unlock a previously locked file record
--------------------------------------------------------------------------*/
PRIVATE int HIunlock(int32 file_id)
{
    CONSTR(FUNC,"HIunlock");     /* for HERROR */

    /* get file record and validate */
    filerec_t *file_rec=FID2REC(file_id);
    if (BADFREC(file_rec))
        HRETURN_ERROR(DFE_ARGS,FAIL);

    /* unlock the file record */
    file_rec->attach--;

    return SUCCEED;
}


/*--------------------------------------------------------------------------
 NAME
       Hnumber -- determine number of objects of a given type
 USAGE
       int32 Hnumber(fid, tag)
       int32 fid;               IN: file ID
       int16 tag;               IN: tag to look for
 RETURNS
       the number of objects of type 'tag' else FAIL
 DESCRIPTION
       Determine how many objects of the given tag are in the file.
       tag may be set to DFTAG_WILDCARD to get back the total number 
       of objects in the file.

       Note, a return value of zero is not a fail condition.

--------------------------------------------------------------------------*/
int32 Hnumber(int32 file_id, uint16 tag)
{
    CONSTR(FUNC,"Hnumber");
    uintn all_cnt;
    uintn real_cnt;
    filerec_t *file_rec = FID2REC(file_id);

    HEclear();
    if (BADFREC(file_rec))
       HRETURN_ERROR(DFE_ARGS,FAIL);

    if(HIcount_dd(file_rec,tag,DFREF_WILDCARD,&all_cnt,&real_cnt)==FAIL)
       HRETURN_ERROR(DFE_INTERNAL,FAIL);
    return (int32)real_cnt;
} /* Hnumber */


/* ------------------------- SPECIAL TAG ROUTINES ------------------------- */
/* 
  The HDF tag space is divided as follows based on the 2 highest bits:
  00: NCSA reserved ordinary tags
  01: NCSA reserved special tags
  10, 11: User tags.
  
  It is relatively cheap to operate with special tags within the NCSA
  reserved tags range.  For users to specify special tags and their
  corresponding ordinary tag, the pair has to be added to the
  special_table.
  
  The special_table contains pairs of each tag and its corrsponding
  special tag.  The same table is also used to determine if a tag is
  special.  Add to this table any additional tag/special_tag pairs
  that might be necessary.  
  
*/

/*
  The functionality of these routines is covered by the SPECIALTAG,
  MKSPECIALTAG and BASETAG macros
*/

#ifdef SPECIAL_TABLE

typedef struct special_table_t {
    uint16 tag;
    uint16 special_tag;
} special_table_t;

PRIVATE special_table_t special_table[] = {
{0x8010, 0x4000 | 0x8010},             /* dummy */
};

#define SP_TAB_SZ (sizeof(special_table) / sizeof(special_table[0]))

/*--------------------------------------------------------------------------
--------------------------------------------------------------------------*/
uint16 HDmake_special_tag(uint16 tag)
{
    register int i;

    if (~tag & 0x8000)
       return ((uint16)(tag | 0x4000));

    for (i=0; i<SP_TAB_SZ; i++)
       if (special_table[i].tag == tag)
           return special_table[i].special_tag;

    return DFTAG_NULL;
}

/*--------------------------------------------------------------------------
--------------------------------------------------------------------------*/
intn HDis_special_tag(uint16 tag)
{
    register int i;

    if (~tag & 0x8000)
       return (tag & 0x4000) ? TRUE : FALSE;

    for (i=0; i<SP_TAB_SZ; i++)
       if (special_table[i].special_tag == tag)
           return TRUE;

    return FALSE;
}

/*--------------------------------------------------------------------------
--------------------------------------------------------------------------*/
uint16 HDbase_tag(uint16 tag)
{
    register int i;

    if (~tag & 0x8000)
       return ((uint16)(tag & ~0x4000));

    for (i=0; i<SP_TAB_SZ; i++)
       if (special_table[i].special_tag == tag)
           return special_table[i].special_tag;

    return tag;                        /* return itself */
}

#endif /* SPECIAL_TABLE */

/*--------------------------------------------------------------------------

 NAME
	Hgetlibversion -- return version info for current HDF library
 USAGE
	intn Hgetlibversion(majorv, minorv, release, string)
	uint32 *majorv;		OUT: majorv version number
	uint32 *minorv;		OUT: minorv versoin number
	uint32 *release;	OUT: release number
	char   string[];	OUT: informational text string (80 chars)
 RETURNS
	returns SUCCEED (0).
 DESCRIPTION
	Copies values from #defines in hfile.h to provided buffers. This
        information is statistically compilied into the HDF library, so
        it is not necessary to have any files open to get this information.

--------------------------------------------------------------------------*/
intn Hgetlibversion(uint32 *majorv, uint32 *minorv, uint32 *releasev, char *string)
{
#ifdef LATER
    CONSTR(FUNC,"Hgetlibversion");
#endif

    HEclear();

    *majorv = LIBVER_MAJOR;
    *minorv = LIBVER_MINOR;
    *releasev = LIBVER_RELEASE;
    HIstrncpy(string, LIBVER_STRING, LIBVSTR_LEN+1);

    return(SUCCEED);
} /* HDgetlibversion */


/*--------------------------------------------------------------------------

 NAME
	Hgetfileversion -- return version info for HDF file
 USAGE
	intn Hgetfileversion(file_id, majorv, minorv, release, string)
	int32 file_id;		IN: handle of file
	uint32 *majorv;		OUT: majorv version number
	uint32 *minorv;		OUT: minorv versoin number
	uint32 *release;	OUT: release number
	char *string;		OUT: informational text string (80 chars)
 RETURNS
	returns SUCCEED (0) if successful and FAIL (-1) if failed.
 DESCRIPTION
	Copies values from file_records[] structure for a given file to 
        provided buffers.
 GLOBAL VARIABLES
	Reads file_records[]

--------------------------------------------------------------------------*/
intn Hgetfileversion(int32 file_id, uint32 *majorv, uint32 *minorv,
		    uint32 *release, char *string)
{
    filerec_t *file_rec;
    CONSTR(FUNC,"Hgetfileversion");


    HEclear();

    file_rec = FID2REC(file_id);
    if (BADFREC(file_rec))
        HRETURN_ERROR(DFE_ARGS,FAIL);

    *majorv = file_rec->version.majorv;
    *minorv = file_rec->version.minorv;
    *release = file_rec->version.release;
    HIstrncpy(string, file_rec->version.string, LIBVSTR_LEN+1);

    if (majorv == 0) {
        HRETURN_ERROR(DFE_NOMATCH,FAIL);
    }
    else
        return(SUCCEED);

} /* Hgetfileversion */

/* -------------------------- PC Specific Stuff --------------------------- */
#if defined PC && !defined PC386
/*--------------------------------------------------------------------------

 NAME
      HDfreadbig -- function specific to the PC to read in 32-bit sized buffers
 USAGE
      int32 HDfreadbig(buffer,size,fp)
      VOIDP buffer;       IN: the buffer to put bytes into
      int32 size;         IN: the number of bytes to read
      FILE *fp;           IN: the file pointer for the file to read
 RETURNS
      returns the number of bytes read
 DESCRIPTION
      Because the IBM PC compilers use 16-bit number for fread, this function
      blocks that up into 64kb blocks to read from a file.

--------------------------------------------------------------------------*/
#ifdef WIN3
int32 HDfreadbig(VOIDP buffer,int32 size,HFILE fp)
#else /* !WIN3 */
int32 HDfreadbig(VOIDP buffer,int32 size,FILE *fp)
#endif /* WIN3 */
{
    uint8 *b;           /* alias for the buffer */
    int32 bytes_read;   /* variable to accumulate the number of bytes read in */

    if(size<=UINT_MAX)   /* if the size is small enough read it in all at once */
#ifdef WIN3
        bytes_read=_lread(fp,buffer,size);
#else
        bytes_read=fread(buffer,1,(uint16)size,fp);
#endif
    else {  /* number of bytes to read */
        bytes_read=0;
        b=buffer;
        while(size>UINT_MAX) {
#ifdef WIN3
            bytes_read+=_lread(fp,b,(uint16)UINT_MAX);
#else
            bytes_read+=fread(b,1,UINT_MAX,fp);
#endif
            b+=UINT_MAX;
            size-=UINT_MAX;
          } /* end while */
        if(size>0)
#ifdef WIN3
            bytes_read+=_lread(fp,b,(uint16)size);
#else
            bytes_read+=fread(b,1,(uint16)size,fp);
#endif
      } /* end else */
    return(bytes_read);
}   /* end HDfreadbig() */


/*--------------------------------------------------------------------------

  NAME
      HDfwritebig -- function specific to the PC to write out 32-bit sized buffers
  USAGE
      int32 HDfwritebig(buffer,size,fp)
      VOIDP buffer;       IN: the buffer to get bytes from
      int32 size;         IN: the number of bytes to write
      FILE *fp;           IN: the file pointer for the file to write
  RETURNS
      returns the number of bytes written
  DESCRIPTION
      Because the IBM PC compilers use 16-bit number for fwrite, this function
      blocks that up into 64kb blocks to write to a file.

--------------------------------------------------------------------------*/
#ifdef WIN3
int32 HDfwritebig(VOIDP buffer,int32 size,HFILE fp)
#else /* !WIN3 */
int32 HDfwritebig(VOIDP buffer,int32 size,FILE *fp)
#endif /* WIN3 */
{
    uint8 *b;              /* alias for the buffer */
    int32 bytes_written;   /* variable to accum. the number of bytes written */

    if(size<=UINT_MAX)  /* if the size is small enough read it in all at once */
#ifdef WIN3
        bytes_written=_lwrite(fp,buffer,(uint16)size);
#else
        bytes_written=fwrite(buffer,1,(uint16)size,fp);
#endif
    else {  /* number of bytes to write */
        bytes_written=0;
        b=buffer;
        while(size>UINT_MAX) {
#ifdef WIN3
            bytes_written+=_lwrite(fp,b,UINT_MAX);
#else
            bytes_written+=fwrite(b,1,UINT_MAX,fp);
#endif
            b+=UINT_MAX;
            size-=UINT_MAX;
          } /* end while */
        if(size>0)
#ifdef WIN3
            bytes_written+=_lwrite(fp,b,(uint16)size);
#else
            bytes_written+=fwrite(b,1,(uint16)size,fp);
#endif
      } /* end else */
    return(bytes_written);
}   /* end HDfwritebig() */
#endif
/* ---------------------- (end of) PC Specific Stuff ---------------------- */


/*--------------------------------------------------------------------------

 NAME
       HIget_file_slot -- find a slot for the file
 USAGE
       int32 HIget_file_slot(path, FUNC)
       char * path;             IN: name of file
       char * FUNC;             IN: calling function
 RETURNS
       a file slot or else FAIL
 DESCRIPTION
       Search the file record array for a matching record, or an empty slot.
       The file is considered the same if the path matches exactly.  This
       routine is unable to detect aliases, or how to compare relative and
       absolute paths.
       
       Error occurred is charged to the calling function.

--------------------------------------------------------------------------*/
PRIVATE int HIget_file_slot(const char *path, char *FUNC)
{
    int i;
    int slot;

#if defined(macintosh) | defined(THINK_C) | defined(DMEM) 

    if (!file_records) {
        /* The array has not been allocated.  Allocating file records
          dynamically. */

       file_records = (filerec_t *) HDgetspace((uint32)MAX_FILE * sizeof(filerec_t));
       if (!file_records) {
           HERROR(DFE_NOSPACE);
           return FAIL;
       }

       /* Initialize file records. */

       for (i = 0; i < MAX_FILE; i++) {
           file_records[i].path = (char *) NULL;
           file_records[i].ddhead = (ddblock_t *) NULL;
           file_records[i].refcount = 0;
       }

       /* Use the first slot. */

       file_records[0].version_set = FALSE;

       file_records[0].path = HDgetspace(HDstrlen(path)+1);
       if(file_records[0].path)
           HDstrcpy(file_records[0].path,path);
       return file_records[0].path ? 0 : FAIL;
    }

#endif /* macintosh or THINK_C or Dynamic Memory */

    /* Search for a matching or free slot. */

    slot = FAIL;
    for (i = 0; i < MAX_FILE; i++) {

        /* If there already is an active FID for this file return it
           thus, there will only ever be one FID per open file.
           This is a BUG if you want to open the file twice with
           different access privs each time. */

       if (file_records[i].path && STREQ(file_records[i].path, path))
           return i;

       /* Otherwise, record first free slot. */

       if (!file_records[i].refcount && slot == FAIL) {
           slot = i;
           file_records[i].path = (char *) NULL;
           file_records[i].ddhead = (ddblock_t *) NULL;
       }

    }

    if (slot == FAIL)      /* No matching or free slot. */
        HRETURN_ERROR(DFE_FNF,FAIL);

    /* Fill empty slot with data. */
    file_records[slot].version_set = FALSE;

    if (file_records[slot].path)
        HDfreespace(file_records[slot].path);
    file_records[slot].path = (char *) HDstrdup((char *) path);
    return file_records[slot].path ? slot : FAIL;

} /* HIget_file_slot */


/*--------------------------------------------------------------------------

 NAME
       HIvalid_magic -- verify the magic number in a file
 USAGE
       int32 HIvalid_magic(path, FUNC)
       hdf_file_t file;             IN: FILE pointer
       char * FUNC;                 IN: calling function
 RETURNS
       TRUE if valid magic number else FALSE
 DESCRIPTION
       Given an open file pointer, see if the first four bytes of the
       file are the HDF "magic number" HDFMAGIC

--------------------------------------------------------------------------*/
PRIVATE intn HIvalid_magic(hdf_file_t file, char *FUNC)
{
    char b[MAGICLEN];          /* Temporary buffer */

    /* Seek to beginning of the file. */
    if (HI_SEEK(file, 0) == FAIL)
       HRETURN_ERROR(DFE_SEEKERROR,FALSE);

    /* Read in magic cookie and compare. */
    if (HI_READ(file, b, MAGICLEN) == FAIL)
       HRETURN_ERROR(DFE_READERROR,FALSE);

    if (NSTREQ(b, HDFMAGIC, MAGICLEN))
        return TRUE;
    else
        return FALSE;
}


/*--------------------------------------------------------------------------
 NAME
	HIget_access_slot -- find a new access record slot
 USAGE
	int HIget_access_slot(void)
 RETURNS
	returns access_record index or FAIL (-1) if failed.
 DESCRIPTION
        Return an index into the access_records array of a location
        to use for a new AID.  If the access_records has not yet
        been allocated it gets allocated with room for MAX_ACC
        access records

--------------------------------------------------------------------------*/
int HIget_access_slot(void)
{
    int i;                     /* temp index */

    /* access records not allocated yet.
       Allocate dynamically and initialize*/

    if (!access_records) {
        access_records = (accrec_t *) HDgetspace(MAX_ACC * sizeof(accrec_t));
        if (!access_records)
            return FAIL;
#ifdef OLD_WAY
        for (i = 0; i < MAX_ACC; i++)
            access_records[i].used = FALSE;
#else /* OLD_WAY */
        HDmemset(access_records,0,MAX_ACC*sizeof(accrec_t));
#endif /* OLD_WAY */

       /* use the first record */

        access_records[0].used = TRUE;
        return 0;
    }

    /* return the first unused record */

    for (i = 0; i < MAX_ACC; i++)
       if (!access_records[i].used) {
           access_records[i].used = TRUE;
           return i;
       }

    return FAIL;

} /* HIget_access_slot */


/*--------------------------------------------------------------------------
 NAME
	HInew_dd_block -- create a new (empty) DD block
 USAGE
	int HInew_dd_block(file_rec, FUNC)
	filerec_t  * file_rec;        IN: file record 
        int16        ndds;            IN: number of DDs in this block
        char       * FUNC;            IN: function to charge errors to
 RETURNS
	returns SUCCEED (0) if successful and FAIL (-1) if failed.
 DESCRIPTION
        Create a new DDblock in the file.  Update the previously
        last DDblock so that its offset points to the newly created
        one.

--------------------------------------------------------------------------*/
int HInew_dd_block(filerec_t *file_rec, int16 ndds, char *FUNC)
{
    ddblock_t *block;          /* current dd block */
    int32 nextoffset;          /* offset of new ddblock */
    int32 offset;              /* offset to the offset of new ddblock */
    uint8 *p;                  /* temp buffer ptr */
    dd_t *list;                /* dd list array of new dd block */
    intn i;                    /* temp integers */
    int16 n;

    /* check integrity of file record */
    if (!file_rec->ddhead || !file_rec->ddlast)
        HRETURN_ERROR(DFE_INTERNAL,FAIL);
    
    /* allocate new dd block record and fill in data */
    if ((block = (ddblock_t *) HDgetspace(sizeof(ddblock_t))) == NULL)
        HRETURN_ERROR(DFE_NOSPACE,FAIL);
    block->ndds = ndds;
    block->next = (ddblock_t *) NULL;
    block->nextoffset = 0;
    
#ifdef OLD_WAY
    /* Check if temproray buffer has been allocated */
    if (ptbuf == NULL) {
        ptbuf = (uint8 *)HDgetspace(TBUF_SZ * sizeof(uint8));
        if (ptbuf == NULL)
            HRETURN_ERROR(DFE_NOSPACE, FAIL);
    }
#endif
    
    /* get room for the new DD block in the file */
    if((nextoffset=HPgetdiskblock(file_rec,NDDS_SZ+OFFSET_SZ+(ndds*DD_SZ),TRUE))
	    == FAIL)
        HRETURN_ERROR(DFE_SEEKERROR,FAIL);
    block->myoffset = nextoffset; /* set offset of new block */
    block->dirty=file_rec->cache; /* if we're caching, wait to write DD block */

if(file_rec->cache) /* if we are caching, wait to update previous DD block */
    file_rec->dirty|=DDLIST_DIRTY;  /* indicate file needs to be flushed */
else {
    p = ptbuf;
    INT16ENCODE(p, block->ndds);
    INT32ENCODE(p, (int32)0);
    if (HI_WRITE(file_rec->file, ptbuf, NDDS_SZ+OFFSET_SZ) == FAIL)
        HRETURN_ERROR(DFE_WRITEERROR,FAIL);
  } /* end else */
    
    /* set up the dd list of this dd block and put it in the file
       after the dd block header */
    list = block->ddlist = (dd_t *) HDgetspace((uint32) ndds * DD_SZ);
    if (list == (dd_t *) NULL)
        HRETURN_ERROR(DFE_NOSPACE,FAIL);
    for (i = 0; i < ndds; i++) {
        list[i].tag = DFTAG_NULL;
        list[i].ref = 0;
        list[i].length = list[i].offset = 0;
    }
    
if(!file_rec->cache) { /* if we are caching, wait to update previous DD block */
    /* n is the number of dds that could fit into ptbuf at one time */
    n = TBUF_SZ / DD_SZ;
    if (n > ndds) 
	n = ndds;
    p = ptbuf;
    for (i = 0; i < n; i++) {
        UINT16ENCODE(p, (uint16)DFTAG_NULL);
        UINT16ENCODE(p, (uint16)0);
        INT32ENCODE(p, (int32)0);
        INT32ENCODE(p, (int32)0);
    }
    while (ndds > 0) {
        if (HI_WRITE(file_rec->file, ptbuf, n*DD_SZ) == FAIL)
            HRETURN_ERROR(DFE_WRITEERROR,FAIL);
        ndds -= n;
        if(n > ndds) 
	    n = ndds;
    }
  } /* end if */
    
    /* update previously last ddblock to point to this new dd block */
    file_rec->ddlast->nextoffset = nextoffset;
    block->prev = file_rec->ddlast;
    file_rec->ddlast->next = block;
if(file_rec->cache) { /* if we are caching, wait to update previous DD block */
    file_rec->dirty|=DDLIST_DIRTY; /* indicate file needs to be flushed */
    file_rec->ddlast->dirty=TRUE; /* indicate this block needs to be flushed */
  } /* end if */
else {
    if (file_rec->ddhead == file_rec->ddlast)
        offset = MAGICLEN + NDDS_SZ;
    else
        offset = file_rec->ddlast->prev->nextoffset + NDDS_SZ;
    p = ptbuf;
    INT32ENCODE(p, nextoffset);
    if (HI_SEEK(file_rec->file, offset) == FAIL)
        HRETURN_ERROR(DFE_SEEKERROR,FAIL);
    if (HI_WRITE(file_rec->file, ptbuf, OFFSET_SZ) == FAIL)
        HRETURN_ERROR(DFE_WRITEERROR,FAIL);
  } /* end else */

    /* update file record */
#ifdef OLD_WAY
    file_rec->ddlast = file_rec->ddlast->next;
#else /* more obvious... */
    file_rec->ddlast = block;
#endif
    /* set the end of the file to the end of the current DD block */
    file_rec->f_end_off=block->myoffset+(NDDS_SZ+OFFSET_SZ)+(block->ndds*DD_SZ);

    return(SUCCEED);
} /* HInew_dd_block */


/*--------------------------------------------------------------------------
 NAME
	HIfill_file_rec -- read in all of the DDs
 USAGE
	int HIfill_file_rec(file_rec, FUNC)
	filerec_t  * file_rec;        IN: file record to parse
        char       * FUNC;            IN: function to charge errors to
 RETURNS
	returns SUCCEED (0) if successful and FAIL (-1) if failed.
 DESCRIPTION
        Fill in a file record with data from the file, especially
        the data descriptors.
--------------------------------------------------------------------------*/
PRIVATE int HIfill_file_rec(filerec_t *file_rec, char *FUNC)
{
    uint8 *p;               /* Temporary pointer. */
    int32 n;
    register intn ndds, i, idx;     /* Temporary integers. */
    uint32 end_off=0;       /* offset of the end of the file */
  
#ifdef OLD_WAY
  /* Check if temproray buffer has been allocated */
  if (ptbuf == NULL) {
      ptbuf = (uint8 *)HDgetspace(TBUF_SZ * sizeof(uint8));
      if (ptbuf == NULL)
          HRETURN_ERROR(DFE_NOSPACE, FAIL);
  }
#endif
  
    /* Alloc start of linked list of ddblocks. */
    file_rec->ddhead = (ddblock_t *) HDgetspace(sizeof(ddblock_t));
    if (file_rec->ddhead == (ddblock_t *) NULL)
        HRETURN_ERROR(DFE_NOSPACE,FAIL);

    /* Only one elt in linked list so head is also last. */
    file_rec->ddlast = file_rec->ddhead;
    file_rec->ddlast->next = (ddblock_t *) NULL;
    file_rec->ddlast->prev = (ddblock_t *) NULL;

    /* The first ddblock always starts after the magic number.
     Set it up so that we start reading from there. */
    if (HI_SEEK(file_rec->file, MAGICLEN) == FAIL)
        HRETURN_ERROR(DFE_SEEKERROR,FAIL);
    file_rec->ddlast->myoffset = MAGICLEN; /* set offset of block in file */

    /* Blank out hash table */
    HDmemset(file_rec->hash, 0, sizeof(tag_ref_list_ptr) * (HASH_MASK+1));

    /* Read in the dd's one at a time and determine the max ref in the file
     at the same time. */
    file_rec->maxref = 0;
    for (;;) {

        /* Read in the start of this dd block.
         Read data consists of ndds (number of dd's in this block) and
         offset (offset to the next ddblock). */
        if (HI_READ(file_rec->file, ptbuf, NDDS_SZ+OFFSET_SZ) == FAIL)
            HRETURN_ERROR(DFE_READERROR,FAIL);
      
      /* Decode the numbers. */
        p = ptbuf;
        INT16DECODE(p, FILE_NDDS(file_rec));
        if (FILE_NDDS(file_rec) <= 0)   /* validity check */
            HRETURN_ERROR(DFE_CORRUPT,FAIL);
        INT32DECODE(p, file_rec->ddlast->nextoffset);
        file_rec->ddlast->dirty = FALSE; /* block does not need to be flushed */
      
        /* Now that we know how many dd's are in this block,
         alloc memory for the records. */
        file_rec->ddlast->ddlist =
            (dd_t *) HDgetspace((uint32)FILE_NDDS(file_rec) * DD_SZ);
        if (!file_rec->ddlast->ddlist)
            HRETURN_ERROR(DFE_NOSPACE,FAIL);
      
        /* Read in dd's. */
        ndds = FILE_NDDS(file_rec);

	/* check if the DD block is the last thing in the file */
	/* (Unlikely, but possible (I think)) */
	if(file_rec->ddlast->myoffset+(NDDS_SZ+OFFSET_SZ)+(ndds*DD_SZ)>end_off)
	    end_off=file_rec->ddlast->myoffset+(NDDS_SZ+OFFSET_SZ)+(ndds*DD_SZ);
      
        /* Since the ptbuf might not be large enough to read in all the dd's
         at once, we try to read in chunks as large as possible. */
      
        /* n is number of dd's that could fit into ptbuf at one time */
        n = TBUF_SZ / DD_SZ;
        if (n > ndds)
            n = ndds;
      
        /* Index of current dd in ddlist of this ddblock is 0. */
      
        idx = 0;
        while (ndds > 0) {
          /* ndds is the remaining number of dd's
             to be read in this block. */
          
            /* Read in a chunk of dd's from the file. */
          
            if (HI_READ(file_rec->file, ptbuf, n * DD_SZ) == FAIL)
                HRETURN_ERROR(DFE_READERROR, FAIL);
          
            /* decode the dd's */
          
            p = ptbuf;
            for (i = 0; i < n; i++, idx++) {
                UINT16DECODE(p, file_rec->ddlast->ddlist[idx].tag);
                UINT16DECODE(p, file_rec->ddlast->ddlist[idx].ref);
                INT32DECODE(p, file_rec->ddlast->ddlist[idx].offset);
                INT32DECODE(p, file_rec->ddlast->ddlist[idx].length);

		/* check if maximum ref # exceeded */
                if (file_rec->maxref < file_rec->ddlast->ddlist[idx].ref)
                    file_rec->maxref = file_rec->ddlast->ddlist[idx].ref;

	        /* check if the data element is the last thing in the file */
	        if((file_rec->ddlast->ddlist[idx].offset
			+file_rec->ddlast->ddlist[idx].length)>end_off)
	            end_off=file_rec->ddlast->ddlist[idx].offset
			    +file_rec->ddlast->ddlist[idx].length;
              
                if(HIadd_hash_dd(file_rec, file_rec->ddlast->ddlist[idx].tag,
                               file_rec->ddlast->ddlist[idx].ref,
                               file_rec->ddlast, idx) == FAIL)
                    return FAIL;
              
            }
          
            /* number of remaining dd's in this ddblock */
            ndds -= (intn)n;
            if (n > ndds)
                n = ndds;
        }
      
        if (file_rec->ddlast->nextoffset != 0) { /* More ddblocks in the file */
          
            /* extend the linked list */
            file_rec->ddlast->next=(ddblock_t *)HDgetspace((uint32)sizeof(ddblock_t));
            if (file_rec->ddlast->next == (ddblock_t *) NULL)
                HRETURN_ERROR(DFE_NOSPACE, FAIL);
          
            /* set up the file so next read will be at the next ddblock */
            if (HI_SEEK(file_rec->file, file_rec->ddlast->nextoffset) == FAIL)
                HRETURN_ERROR(DFE_SEEKERROR, FAIL);
          
            file_rec->ddlast->next->prev = file_rec->ddlast;
            file_rec->ddlast = file_rec->ddlast->next;
            file_rec->ddlast->next = (ddblock_t *) NULL;
            file_rec->ddlast->ddlist = (dd_t *) NULL;

	    /* get the offset for the new block */
            file_rec->ddlast->myoffset=file_rec->ddlast->prev->nextoffset;
          
          } /* end if */
	else
            break;
      } /* end for */
  
    file_rec->f_end_off=end_off; /* keep the end of the file around */

    return SUCCEED;
} /* HIfill_file_rec */


/*--------------------------------------------------------------------------
 PRIVATE	PRIVATE		PRIVATE		PRIVATE		PRIVATE
 NAME
	HIupdate_version -- determine whether new version tag should be written
 USAGE
	int HIupdate_version(file_id)
	int32 file_id;		IN: handle of file
 RETURNS
	returns SUCCEED (0) if successful and FAIL (-1) if failed.
 DESCRIPTION
	Writes out version numbers of current library as file version.
 GLOBAL VARIABLES
	Resets modified field of version field of appropriate file_records[]
	entry.

--------------------------------------------------------------------------*/
PRIVATE int HIupdate_version(int32 file_id)
{
    /* uint32 lmajorv, lminorv, lrelease; */
    uint8 /*lstring[81],*/ lversion[LIBVER_LEN];
    filerec_t *file_rec;
    int ret, i;
    CONSTR(FUNC,"Hupdate_version");


    HEclear();

    file_rec = FID2REC(file_id);
    if (BADFREC(file_rec))
        HRETURN_ERROR(DFE_ARGS,FAIL);

    /* copy in-memory version to file */
    Hgetlibversion(&(file_rec->version.majorv), &(file_rec->version.minorv),
    		   &(file_rec->version.release), file_rec->version.string);

    {
    	uint8 *p;

    	p = lversion;
    	UINT32ENCODE(p, file_rec->version.majorv);
    	UINT32ENCODE(p, file_rec->version.minorv);
    	UINT32ENCODE(p, file_rec->version.release);
        HIstrncpy((char*) p, file_rec->version.string, LIBVSTR_LEN);
        for (i = HDstrlen((char *)p); i < LIBVSTR_LEN; i++)
            p[i] = (uint8) 0;
    }

    ret = (int)Hputelement(file_id, (uint16)DFTAG_VERSION, (uint16)1, lversion,
		      (int32)LIBVER_LEN);

    if (ret == SUCCEED) {
        file_rec->version.modified = 0;
        return(SUCCEED);
    } else
        HRETURN_ERROR(DFE_INTERNAL,FAIL);

} /* HIupdate_version */


/*--------------------------------------------------------------------------
 PRIVATE	PRIVATE		PRIVATE		PRIVATE		PRIVATE
 NAME
	HIread_version -- reads a version tag from a file
 USAGE
	int HIread_version(file_id)
	int32 file_id;		IN: handle of file
 RETURNS
	returns SUCCEED (0) if successful and FAIL (-1) if failed.
 DESCRIPTION
	Reads a version tag from the specified file into the version fields
	of the appropriate filerec_t.  On failure, zeros are put in the version
	number fields and NULLS in the string.
 GLOBAL VARIABLES
	Writes to version fields of appropriate file_records[] entry.

--------------------------------------------------------------------------*/
PRIVATE int HIread_version(int32 file_id)
{
    filerec_t *file_rec;
    uint8 fversion[LIBVER_LEN];
    CONSTR(FUNC,"Hread_version");

    HEclear();

    file_rec = FID2REC(file_id);
    if (BADFREC(file_rec))
        HRETURN_ERROR(DFE_ARGS,FAIL);

    if(Hgetelement(file_id,(uint16)DFTAG_VERSION,(uint16)1,fversion) == FAIL) {
        file_rec->version.majorv = 0;
        file_rec->version.minorv = 0;
        file_rec->version.release = 0;
        HDstrcpy(file_rec->version.string, "");
        file_rec->version.modified = 0;
        HRETURN_ERROR(DFE_INTERNAL,FAIL);
    } else {
        uint8 *p;

        p = fversion;
        UINT32DECODE(p, file_rec->version.majorv);
        UINT32DECODE(p, file_rec->version.minorv);
        UINT32DECODE(p, file_rec->version.release);
        HIstrncpy(file_rec->version.string, (char*) p, LIBVSTR_LEN);
    }
    file_rec->version.modified = 0;

    return(SUCCEED);

} /* HIread_version */

/* ----------------------------------------------------------------------- 

 NAME
	HPgetdiskblock --- Get the offset of a free block in the file.
 USAGE
	int32 HPgetdiskblock(file_rec, block_size)
	    filerec_t *file_rec;     IN: ptr to the file record
	    int32 block_size;        IN: size of the block needed
	    intn moveto;             IN: whether to move the file position
					 to the allocated position or leave
					 it undefined.
 RETURNS
	returns offset of block in the file if successful, FAIL (-1) if failed.
 DESCRIPTION
        Used to "allocate" space in the file.  Currently, it just appends
	blocks to the end of the file willy-nilly.  At some point in the
	future, this could be changed to use a "real" free-list of empty 
	blocks in the file and dole those out.

-------------------------------------------------------------------------*/
int32 HPgetdiskblock(filerec_t *file_rec, int32 block_size, intn moveto)
{
    CONSTR(FUNC,"HPgetdiskblock");
    uint8 temp;
    int32 ret;

    /* check for valid arguments */
    if(file_rec==NULL || block_size<0)
        HRETURN_ERROR(DFE_ARGS,FAIL);

#ifdef OLD_WAY
    /* "allocate" space by appending to the end of the file */
    if(HI_SEEKEND(file_rec->file) == FAIL) 
        HRETURN_ERROR(DFE_SEEKERROR,FAIL);
    if((ret=HI_TELL(file_rec->file))==FAIL)
	HRETURN_ERROR(DFE_BADSEEK,FAIL);

    /* reserve the space by marking the end of the element */
    if(block_size>0) {
        if (HI_SEEK(file_rec->file, ret+block_size-1) == FAIL) 
            HRETURN_ERROR(DFE_SEEKERROR,FAIL);
        if (HI_WRITE(file_rec->file, &temp, 1) == FAIL) 
            HRETURN_ERROR(DFE_WRITEERROR,FAIL);
	if(moveto==TRUE)  /* move back to the beginning of the element */
            if (HI_SEEK(file_rec->file, ret) == FAIL) 
                HRETURN_ERROR(DFE_SEEKERROR,FAIL);
      } /* end if */
#else
    /* get the offset of the end of the file */
    ret=file_rec->f_end_off;

    /* reserve the space by marking the end of the element */
    if(block_size>0) {
if(file_rec->cache) 
    file_rec->dirty|=FILE_END_DIRTY;
else {
        if (HI_SEEK(file_rec->file, ret+block_size-1) == FAIL) 
            HRETURN_ERROR(DFE_SEEKERROR,FAIL);
        if (HI_WRITE(file_rec->file, &temp, 1) == FAIL) 
            HRETURN_ERROR(DFE_WRITEERROR,FAIL);
  } /* end else */
      } /* end if */
    if(moveto==TRUE)  /* move back to the beginning of the element */
        if (HI_SEEK(file_rec->file, ret) == FAIL) 
            HRETURN_ERROR(DFE_SEEKERROR,FAIL);
    file_rec->f_end_off=ret+block_size; /* incr. offset of end of file */
#endif
    return(ret);
}   /* HPgetdiskblock() */

/* ----------------------------------------------------------------------- 

 NAME
	HPfreediskblock --- Release a block in a file to be re-used.
 USAGE
	intn HPfreediskblock(file_rec, block_off, block_size)
	filerec_t *file_rec;     IN: ptr to the file record
	int32 block_off;         IN: offset of the block to release
	int32 block_size;        IN: size of the block to release
 RETURNS
	returns SUCCEED (0) if successful, FAIL (-1) if failed.
 DESCRIPTION
        Used to "release" space in the file.  Currently, it does nothing.
	At some point in the future, this could be changed to add the block
	to a "real" free-list of empty blocks in the file and manage those.

-------------------------------------------------------------------------*/
intn HPfreediskblock(filerec_t *file_rec, int32 block_off, int32 block_size)
{
#ifdef LATER
    CONSTR(FUNC,"HPfreediskblock");
#endif

    /* shut compiler up */
    file_rec=file_rec; block_off=block_off; block_size=block_size;

    return(SUCCEED);
}   /* HPfreediskblock() */


/*--------------------------------------------------------------------------

 NAME
       HDget_special_info -- get information about a special element
 USAGE
       intn HDget_special_info(access_id, info_block)
       int32 access_id;        IN: id of READ access element
       sp_info_block_t * info_block; 
                               OUT: information about the special element
 RETURNS
       SUCCEED / FAIL
 DESCRIPTION
       Fill in the given info_block with information about the special
       element.  Return FAIL if it is not a speical element AND set
       the 'key' field to FAIL in info_block.

--------------------------------------------------------------------------*/
int32 HDget_special_info(int32 access_id, sp_info_block_t * info_block)
{
    char *FUNC="HDget_special_info";  /* for HERROR */
    accrec_t *access_rec;             /* access record */

    /* clear error stack and check validity of access id */
    HEclear();
    access_rec = AID2REC(access_id);
    if (access_rec == (accrec_t *) NULL || !access_rec->used || 
        	info_block == NULL)
        HRETURN_ERROR(DFE_ARGS,FAIL);
    
    /* special elt, so call special function */
    if (access_rec->special)
        return (*access_rec->special_func->info)(access_rec, info_block);
    
    /* else is not special so FAIL */

    info_block->key = FAIL;
    return FAIL;

} /* HDget_special_info */


/*--------------------------------------------------------------------------

 NAME
       HDset_special_info -- reset information about a special element
 USAGE
       intn HDet_special_info(access_id, info_block)
       int32 access_id;        IN: id of READ access element
       sp_info_block_t * info_block; 
                               IN: information about the special element
 RETURNS
       SUCCEED / FAIL
 DESCRIPTION
       Attempt to replace the special information for the given element
       with new information.  This routine should be used to rename
       external elements for example.  Doing things like changing the
       blocking of a linekd block element are beyond the scope of this
       routine.

--------------------------------------------------------------------------*/
int32 HDset_special_info(int32 access_id, sp_info_block_t * info_block)
{
    char *FUNC="HDset_special_info";  /* for HERROR */
    accrec_t *access_rec;             /* access record */

    /* clear error stack and check validity of access id */

    access_rec = AID2REC(access_id);
    if (access_rec == (accrec_t *) NULL || 
        !access_rec->used || 
        info_block == NULL)
        HRETURN_ERROR(DFE_ARGS,FAIL);
    
    /* special elt, so call special function */
    if (access_rec->special)
        return (*access_rec->special_func->reset)(access_rec, info_block);

    /* else is not special so fail */
    return FAIL;

} /* HDset_special_info */


/* -------------------------- MAC Specific Stuff -------------------------- */
#if defined(MAC) & !defined(THINK_C)
/*
*  Macintosh file stubs for HDF
*
*  Implement a subset of the C unbuffered file I/O stubs in the
*  Mac toolbox.
*/

#include "Errors.h"
#ifdef MPW
#include "Strings.h"
#endif

PRIVATE int32 hdfc = 1061109567L;    /* equal to 4 '?' in a row in ascii */
					/* yes, I am deliberately avoiding the trigraph :-) */
PRIVATE int32 hdft = 1600085855L;    /* equal to '_HDF' in ascii */

#ifdef MPW
hdf_file_t
mopen(char *name, intn flags)
{
    hdf_file_t volref,rn;
    OSErr result;
    FInfo fndrInfo;

    GetVol(NULL,&volref);
    
    if (flags == DFACC_CREATE)   { /* we need to create it */
        
        result = getfinfo(name, volref, &fndrInfo);
        if (result != fnfErr)
            if( noErr != (result = fsdelete(name, volref)))
                return FAIL;
        
        if (noErr != (result = create(name, volref, hdfc, hdft)))
            return FAIL;
        
    }
    
    if (noErr != (result = fsopen(name, volref, &rn)))
        return FAIL;
    
    if (flags & O_CREAT)    /* and truncate it */
        SetEOF(rn, 0);
    
    return(rn);  
}
#else

PRIVATE Str255 pname;

hdf_file_t
mopen(char *name, intn flags)
{
    hdf_file_t volref,rn;
    OSErr result;
    FInfo fndrInfo;

    HDstrcpy((char *) pname, (char *) name);
    CtoPstr(pname);

    result = GetVol(NULL,&volref);
    
    if (flags == DFACC_CREATE)   { /* we need to create it */
        
        result = GetFInfo(name, volref, &fndrInfo);
        if (result != fnfErr)
            if( noErr != (result = FSDelete(pname, volref)))
                return FAIL;
        
        if (noErr != (result = Create(pname, volref, hdfc, hdft)))
            return FAIL;
        
    }
    
    if (noErr != (result = FSOpen(pname, volref, &rn)))
        return FAIL;
    
    if (flags & O_CREAT)    /* and truncate it */
        SetEOF(rn, 0L);
    
    return(rn);  
}

#endif

int32
mclose(hdf_file_t rn)
{
        return(FSClose(rn));
}

int32 
mread(hdf_file_t rn, char *buf, int32 n)
{
	OSErr result;

        if (noErr != (result = FSRead( rn, &n, buf)))
                return(FAIL);

        return(n);
}

int32
mwrite(hdf_file_t rn, char *buf, int32 n)
{
    OSErr result;

    if (noErr != (result = FSWrite( rn, &n, buf)))
        return(FAIL);
    
    return(n);
}

int32
mlseek(hdf_file_t rn, int32 n, intn m)
{
    OSErr result;
    int32 newEOF;

#ifdef OLD_EXTD
    long pos, oldpos, logEOF;
    Ptr buffy;
#endif
    
    switch(m) {
    case 0:
    default:
        m = fsFromStart;
        break;
    case 1:
        m = fsFromMark;
        break;
    case 2:
        m = fsFromLEOF;
        break;
    }
    
    if (noErr != (result = SetFPos(rn, m, n)))
        {
            if(result == eofErr)
                {
#ifdef OLD_EXTD
                    if(noErr != (result = GetEOF(rn, &logEOF)))
                        return FAIL;

                    oldpos = pos = n - logEOF;
                    if(NULL == (buffy = NewPtr((Size) pos)))
                        return FAIL;
                    if (noErr != (result = FSWrite(rn, &pos, buffy)))
                        {
                            DisposPtr(buffy);
                            return FAIL;
                        }
                    DisposPtr(buffy);
#else


                    if(m != fsFromStart)
                        return FAIL;

                    newEOF = n;
                    if(noErr != (result = SetEOF(rn, newEOF)))
                        return FAIL;    
                    
#endif                     
                    
                    if (noErr != (result = SetFPos(rn, fsFromStart, n)))
                        return FAIL;
                }
            else return FAIL;
        }
    
    if (noErr != (result = GetFPos(rn, &n)))
        return FAIL;

    if (m == fsFromMark) {
        return(n);
    } else {
        return(SUCCEED);
    }

}

#endif /* MAC */
/* --------------------- (end of) MAC Specific Stuff ---------------------- */
