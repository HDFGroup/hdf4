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

/*LINTLIBRARY */
/* ------------------------------ hblocks.c -------------------------------
   routines to implement linked-block elements

   Linked element in HDF files created in two ways
   -- created from the start or
   -- converted from a normal data element

   A linked-block element is a special element.

   Special elements are
   flagged with a set high-bit in their tag.  Thus, a tag t has
   BASETAG == t & 0x7f and is a special tag if t & 0x80 != 0x00

   The first 16 bits of the meta-element that this tag/ref points to
   tells us what type of special element this is.  If these 16 bits is
   SPECIAL_LINKED, then it contains information about the linked blocks.
   After this 16 bits, 32 bit which is the length of each block, after
   which is the information header:

   ----------------------------------------------------------------------
   | # blocks in | tag/ref of | tag/ref of blocks list .......          |
   | this header | next header|                                         |
   ----------------------------------------------------------------------

   File Description of Linked Block Element
   ****************************************
   DD for Linked Block pointing to Linked Block Description Record
   ==============================================================
   <-  2 bytes -> <- 2 bytes -> <- 4 bytes -> <- 4bytes ->
   --------------------------------------------------------
   |extended tag | reference # |  Offset     |  Length    |
   --------------------------------------------------------
                                    \______________/
   __________________________________________|
   V
   LINKED BLOCK DESCRIPTION RECORD(LBDR - 22 bytes)
   ===============================================
   <-  4 bytes -> <- 4 bytes  -> <-   4 bytes  -> <- 4bytes ->
   --------------------------------------------------------------
   |ext_tag_desc | elem_tot_len | blk_first_len  | blk_length |   ... cont'd
   --------------------------------------------------------------
    
   <- 4 bytes -> <- 2 bytes ->
   --------------------------
...  num_blk   | link_ref   |
   --------------------------

   ext_tag_desc   - EXT_LINKED(32 bit constant), identifies this as
                    a linked block description record
   elem_tot_len   - Length of the entire element(32 bit field)
   blk_first_len  - Length of the first data block(32 bit field)
   blk_length     - Length of successive data blocks(32 bit field)
   num_blk        - Number of blocks per block table(32 bit field)
   link_ref       - Reference number of the first block table(16 bit field)

   Linked Block Table(12 + 2 + 2 + 2 + 2 + ... bytes)
   ===================================================
   <-  2 bytes -> <- 2 bytes -> <- 4 bytes -> <- 4bytes ->
   --------------------------------------------------------
   |link_blk_tag | link_ref    |  Offset     |  Length    |
   --------------------------------------------------------
                                    \______________/
   __________________________________________|
   V
   <-  2 bytes -> <- 2 bytes -> <- 2 bytes -> <- 2 bytes -> <-...
   -----------------------------------------------------------...
   | next_ref    | block_ref_1 | block_ref_2 | block_ref_3 |  ...
   -----------------------------------------------------------...
    
   link_blk_tag   - DFTAG_LINKED(16 bit)
   link_ref       - Reference number for this table(16 bit)
   next_ref       - Reference number for next block table(16 bit)
                    Zero(0) signifies no more block tables for this element.
   blk_ref_x      - Reference number for data block X (16 bit). 
                  e.g. for data block 1
                  <-  2 bytes ->  <- 2 bytes -> <- 4 bytes -> <- 4bytes ->
                  --------------------------------------------------------
                  | DFTAG_LINKED | block_ref_1 |  Offset     |  Length    |
                  --------------------------------------------------------
                                                    \______________/
                  __________________________________________|
                  V
                  -----------------------
                  | Data_block          |
                  -----------------------
                  Note: The "Length" here is specified by either 
                        "elem_first_len" or "blk_length".

   For now, HLcreate() has the best description of what the on-disk
   representation of a linked block element looks like.

EXPORTED ROUTINES

   HLcreate       -- create a linked block element
   HLconvert      -- convert an AID into a linked block element
   HDinqblockinfo -- return info about linked blocks
   HLPstread      -- open an access record for reading
   HLPstwrite     -- open an access record for writing
   HLPseek        -- set the seek posn
   HLPread        -- read some data out of a linked block element
   HLPwrite       -- write out some data to a linked block
   HLPinquire     -- Hinquire for linked blocks
   HLPendacess    -- close a linked block AID
   HLPinfo        -- return info about a linked block element
LOCAL ROUTINES
   HLIstaccess -- set up AID to access a linked block elem
   HLIgetlink  -- get link information
   HLInewlink  -- write out some data to a linked block
*/

#include "hdf.h"
#include "hfile.h"

/* block_t - record of a linked block. contains the tag and ref of the
   data elt that forms the linked block */
typedef struct block_t
  {
      uint16      ref;          /* ref of the linked block */
  }
block_t;

/* link_t - a linked list block table.
   Very similar to the dd block structure */
typedef struct link_t
  {
      uint16      nextref;      /* ref of the next block table */
      struct link_t *next;      /* ptr to the next block table */
      struct block_t *block_list;   /* ptr to the block list for this table */
  }
link_t;

/* information on this special linked block data elt */
typedef struct linkinfo_t
  {
      int         attached;     /* how many access records refer to this elt */
      int32       length;       /* the actual length of the data elt */
      int32       first_length; /* length of first block */
      int32       block_length; /* the length of the remaining blocks */
      int32       number_blocks;    /* total number of blocks in each link/block table */
      uint16      link_ref;     /* ref of the first block table structure */
      link_t     *link;         /* pointer to the first block table */
      link_t     *last_link;    /* pointer to the last block table */
  }
linkinfo_t;

/* private functions */
PRIVATE int32 HLIstaccess
            (accrec_t * access_rec, int16 acc_mode);

PRIVATE link_t *HLInewlink
            (int32 file_id, int32 number_blocks, uint16 link_ref, uint16 first_block_ref);

PRIVATE link_t *HLIgetlink
            (int32 file_id, uint16 ref, int32 number_blocks);

/* the accessing function table for linked blocks */
funclist_t  linked_funcs =
{
    HLPstread,
    HLPstwrite,
    HLPseek,
    HLPinquire,
    HLPread,
    HLPwrite,
    HLPendaccess,
    HLPinfo
};

/* ------------------------------------------------------------------------
NAME
   HLcreate -- create a linked block element
USAGE
   int32 HLcreate(fid, tag, ref, blocklen, numblocks)
   int32   fid;         IN: file to put linked block element in
   uint16  tag;         IN: tag of element
   uint16  ref;         IN: ref of element
   int32   blocklen;    IN: length of standard block
   int32   numblocks;   IN: number of blocks per block list
RETURNS
   The AID of newly created linked block element, FAIL on error.
DESCRIPTION
   This routine takes an HDF element and promotes it into a linked
   block element.  Basically, the element becomes a linked list
   allowing easy appending.  If the element already exists, it
   is promoted to being a linked block element, otherwise a new
   element is created.

   All of the pieces of the linked list are the same size (blocklen)
   except for the first one which stays the size of the element
   at the time HLcreate was called.

   numblocks gives the number of linked list objects in each
   block header.

   The ideal setting for numblocks and blocklen are very data
   and application depedent.

 --------------------------------------------------------------------------- */
int32
HLcreate(int32 file_id, uint16 tag, uint16 ref, int32 block_length,
         int32 number_blocks)
{
    CONSTR(FUNC, "HLcreate");   /* for HERROR */
    filerec_t  *file_rec;       /* file record */
    accrec_t   *access_rec;     /* access record */
    int         slot;           /* slot in access records */
    dd_t       *dd;             /* ptr to created dd */
    ddblock_t  *data_block;     /* dd lbock of the data dd */
    int32       data_idx;       /* dd list index of data dd */
    linkinfo_t *info;           /* information for the linked blocks elt */
    uint16      link_ref;       /* the ref of the link structure
                                   (block table) */
    dd_t       *data_dd;        /* dd of existing regular element */
    uint16      special_tag;    /* special version of this tag */
    uint8       local_ptbuf[16];

    /* clear error stack and validate file record id */
    HEclear();
    file_rec = FID2REC(file_id);
    if (BADFREC(file_rec) || block_length < 0 || number_blocks < 0
        || SPECIALTAG(tag)
        || (special_tag = MKSPECIALTAG(tag)) == DFTAG_NULL)
        HRETURN_ERROR(DFE_ARGS, FAIL);

    if (!(file_rec->access & DFACC_WRITE))
        HRETURN_ERROR(DFE_DENIED, FAIL);

    /* get empty slot in access records */
    slot = HIget_access_slot();
    if (slot == FAIL)
        HRETURN_ERROR(DFE_TOOMANY, FAIL);

    access_rec = &access_records[slot];

    /* update maxref if necessary */
    if (ref > file_rec->maxref)
        file_rec->maxref = ref;

    /* search for identical dd */
    if (HIlookup_dd(file_rec, tag, ref, &data_block, &data_idx) != FAIL)
      {
          data_dd = &(data_block->ddlist[data_idx]);
          if (SPECIALTAG(data_dd->tag))
            {
                access_rec->used = FALSE;
                HRETURN_ERROR(DFE_CANTMOD, FAIL);
            }   /* end if */
      }     /* end if */
    else
        data_dd = (dd_t *) NULL;

    /* search for the empty dd to put new dd */
    if (HIlookup_dd(file_rec, DFTAG_NULL, DFREF_WILDCARD, &file_rec->null_block,
                    &file_rec->null_idx) == FAIL)
      {
          if (HInew_dd_block(file_rec, FILE_NDDS(file_rec), FUNC) == FAIL)
            {
                access_rec->used = FALSE;
                HRETURN_ERROR(DFE_NOFREEDD, FAIL);
            }
          else
            {
                access_rec->block = file_rec->ddlast;
                access_rec->idx = 0;
            }
      }
    else
      {
          access_rec->block = file_rec->null_block;
          access_rec->idx = file_rec->null_idx;
      }
    dd = &access_rec->block->ddlist[access_rec->idx];

    if (data_dd)
      {   /* remove old tag from hash table */
          if (HIdel_hash_dd(file_rec, data_dd->tag, data_dd->ref) == FAIL)
            {
                access_rec->used = FALSE;
                HRETURN_ERROR(DFE_CANTDELHASH, FAIL);
            }   

          data_dd->tag = DFTAG_LINKED;
          data_dd->ref = Hnewref(file_id);
          if (HIupdate_dd(file_rec, data_block, data_idx, FUNC) == FAIL)
            {
                access_rec->used = FALSE;
                HRETURN_ERROR(DFE_CANTUPDATE, FAIL);
            }

          /* update hash table */
          if (HIadd_hash_dd(file_rec, data_dd->tag, data_dd->ref, data_block,
                            data_idx) == FAIL)
            {
                access_rec->used = FALSE;
                HRETURN_ERROR(DFE_CANTHASH, FAIL);
            }   
      }

    /* write the special info structure to fill */
    link_ref = Hnewref(file_id);
    dd->length = 16;

    if ((dd->offset = HPgetdiskblock(file_rec, dd->length, TRUE)) == FAIL)
      {
          access_rec->used = FALSE;
          HRETURN_ERROR(DFE_INTERNAL, FAIL);
      }     /* end if */
    if (( info = (linkinfo_t *) HDgetspace((uint32) sizeof(linkinfo_t)))==NULL)
      {
          access_rec->used = FALSE;
          HRETURN_ERROR(DFE_NOSPACE, FAIL);
      }

    info->attached = 1;
    info->length = data_dd ? data_dd->length : 0;
    info->first_length = data_dd ? data_dd->length : block_length;
    info->block_length = block_length;
    info->number_blocks = number_blocks;

    {
        uint8      *p;
        p = local_ptbuf;
        UINT16ENCODE(p, SPECIAL_LINKED);
        INT32ENCODE(p, info->length);
        INT32ENCODE(p, block_length);
        INT32ENCODE(p, number_blocks);
        UINT16ENCODE(p, link_ref);  /* link_ref */
    }
    if (HI_WRITE(file_rec->file, local_ptbuf, dd->length) == FAIL)
      {
          access_rec->used = FALSE;
          HRETURN_ERROR(DFE_WRITEERROR, FAIL);
      }
    dd->tag = special_tag;
    dd->ref = ref;

    /* allocate info structure and file it in */
    info->link = HLInewlink(file_id, number_blocks, link_ref,
                            (uint16) (data_dd ? data_dd->ref : 0));
    if (!info->link)
      {
          access_rec->used = FALSE;
          return FAIL;
      }
    info->link_ref = link_ref;

    /* update dd list in file */
    if (HIupdate_dd(file_rec, access_rec->block, access_rec->idx, FUNC) == FAIL)
      {
          access_rec->used = FALSE;
          HRETURN_ERROR(DFE_CANTUPDATE, FAIL);
      }

    /* update hash table */
    if (HIadd_hash_dd(file_rec, dd->tag, dd->ref, access_rec->block,
                      access_rec->idx) == FAIL)
      {
          access_rec->used = FALSE;
          HRETURN_ERROR(DFE_CANTHASH, FAIL);
      }     

    /* update access record and file record */
    access_rec->special_func = &linked_funcs;
    access_rec->special_info=(VOIDP)info;
    access_rec->special = SPECIAL_LINKED;
    access_rec->posn = 0;
    access_rec->access = DFACC_RDWR;
    access_rec->file_id = file_id;
    access_rec->appendable = FALSE;     /* start data as non-appendable */
    access_rec->flush = FALSE;  /* start data as not needing flushing */
    file_rec->attach++;

    return ASLOT2ID(slot);
}

/* ------------------------------------------------------------------------
NAME
   HLconvert -- convert an AID into a linked block element
USAGE
   intn HLconvert(aid, blocklen, numblocks)
   int32   aid;         IN: AID to convert
   int32   blocklen;    IN: length of standard block
   int32   numblocks;   IN: number of blocks per block list
RETURNS
   SUCCEED / FAIL
DESCRIPTION
   This routine takes an HDF element and promotes it into a linked
   block element.  Basically, the element becomes a linked list
   allowing easy appending.  If the element already exists, it
   is promoted to being a linked block element, otherwise a new
   element is created.

   All of the pieces of the linked list are the same size (blocklen)
   except for the first one which stays the size of the element
   at the time HLcreate was called.

   This routine is similar to HLcreate but is used to convert an
   existing AID into a linked block element "in-place".  This is
   done for convenience and ease-of-use mostly internally to the
   library in various places, but it is allowable for user-level
   code to do this also.

   Hopefully HLcreate will get re-written to call this routine for
   most of it's work...

   numblocks gives the number of linked list objects in each
   block header.

   The ideal setting for numblocks and blocklen are very data
   and application depedent.

---------------------------------------------------------------------------*/
intn
HLconvert(int32 aid, int32 block_length, int32 number_blocks)
{
    CONSTR(FUNC, "HLconvert");  /* for HERROR */
    filerec_t  *file_rec;       /* file record */
    accrec_t   *access_rec;     /* access record */
    dd_t       *dd;             /* ptr to created dd */
    ddblock_t  *data_block;     /* dd lbock of the data dd */
    int32       data_idx;       /* dd list index of data dd */
    linkinfo_t *info;           /* information for the linked blocks elt */
    uint16      link_ref;       /* the ref of the link structure
                                   (block table) */
    dd_t       *data_dd;        /* dd of existing regular element */
    uint16      special_tag;    /* special version of this tag */
    int32       file_id;        /* file ID for the access record */
    uint8       local_ptbuf[16];
    int32       old_posn;       /* position in the access element */

    /* clear error stack */
    HEclear();

    /* start checking the func. args */
    if (!VALIDAID(aid) || block_length < 0 || number_blocks < 0)
        HRETURN_ERROR(DFE_ARGS, FAIL);

    /* get the access_rec pointer */
    if ((access_rec = AID2REC(aid)) == NULL)    
        HRETURN_ERROR(DFE_ARGS, FAIL);

    file_id = access_rec->file_id;
    file_rec = FID2REC(file_id);
    if (BADFREC(file_rec))
        HRETURN_ERROR(DFE_ARGS, FAIL);

    if (!(file_rec->access & DFACC_WRITE))
        HRETURN_ERROR(DFE_DENIED, FAIL);

    /* get ptrs to DD info and verify that the object is not already special */
    data_block = access_rec->block;
    data_idx = access_rec->idx;
    data_dd = &(data_block->ddlist[data_idx]);
    if (SPECIALTAG(data_dd->tag))
        HRETURN_ERROR(DFE_CANTMOD, FAIL);

    /* Save previous position in data element so that we can come back to it */
    old_posn=access_rec->posn;

    if ((special_tag = MKSPECIALTAG(data_dd->tag)) == DFTAG_NULL)
        HRETURN_ERROR(DFE_BADDDLIST, FAIL);

    /* search for the empty dd to put new dd */
    if (HIlookup_dd(file_rec, DFTAG_NULL, DFREF_WILDCARD, &file_rec->null_block,
                    &file_rec->null_idx) == FAIL)
      {
          if (HInew_dd_block(file_rec, FILE_NDDS(file_rec), FUNC) == FAIL)
            {
                HRETURN_ERROR(DFE_NOFREEDD, FAIL)
            }   
          else
            {
                access_rec->block = file_rec->ddlast;
                access_rec->idx = 0;
            }   
      }     
    else
      {
          access_rec->block = file_rec->null_block;
          access_rec->idx = file_rec->null_idx;
      }     
    dd = &access_rec->block->ddlist[access_rec->idx];

    if (data_dd)
      {   /* modify old DD entry */
          /* remove old tag from hash table */
          if (HIdel_hash_dd(file_rec, data_dd->tag, data_dd->ref) == FAIL)
              HRETURN_ERROR(DFE_CANTDELHASH, FAIL);

          data_dd->tag = DFTAG_LINKED;
          if (HIupdate_dd(file_rec, data_block, data_idx, FUNC) == FAIL)
              HRETURN_ERROR(DFE_CANTUPDATE, FAIL);

          /* update hash table */
          if (HIadd_hash_dd(file_rec, data_dd->tag, data_dd->ref, data_block,
                            data_idx) == FAIL)
              HRETURN_ERROR(DFE_CANTHASH, FAIL);
      }     

    /* write the special info structure to fill */
    link_ref = Hnewref(file_id);
    dd->length = 16;

    if ((dd->offset = HPgetdiskblock(file_rec, dd->length, TRUE)) == FAIL)
      {
          access_rec->used = FALSE;
          HRETURN_ERROR(DFE_INTERNAL, FAIL);
      }     

    access_rec->special_info = (VOIDP) HDgetspace((uint32) sizeof(linkinfo_t));
    if (!access_rec->special_info)
      {
          access_rec->used = FALSE;
          HRETURN_ERROR(DFE_NOSPACE, FAIL);
      }     

    info = (linkinfo_t *) access_rec->special_info;
    info->attached = 1;
    info->length = data_dd ? data_dd->length : 0;
    info->first_length = data_dd ? data_dd->length : block_length;
    info->block_length = block_length;
    info->number_blocks = number_blocks;

    {
        uint8      *p;

        p = local_ptbuf;
        UINT16ENCODE(p, SPECIAL_LINKED);
        INT32ENCODE(p, info->length);
        INT32ENCODE(p, block_length);
        INT32ENCODE(p, number_blocks);
        UINT16ENCODE(p, link_ref);  /* link_ref */
    }

    if (HI_WRITE(file_rec->file, local_ptbuf, dd->length) == FAIL)
      {
          access_rec->used = FALSE;
          HRETURN_ERROR(DFE_WRITEERROR, FAIL);
      }
    dd->tag = special_tag;
    dd->ref = data_dd->ref;

    /* allocate info structure and file it in */
    info->link = HLInewlink(file_id, number_blocks, link_ref,
                            (uint16) (data_dd ? data_dd->ref : 0));
    if (!info->link)
      {
          access_rec->used = FALSE;
          HRETURN_ERROR(DFE_CANTLINK, FAIL);
      }     
    info->link_ref = link_ref;

    /* update dd list in file */
    if (HIupdate_dd(file_rec, access_rec->block, access_rec->idx, FUNC) == FAIL)
      {
          access_rec->used = FALSE;
          HRETURN_ERROR(DFE_CANTUPDATE, FAIL);
      }     

    /* update hash table */
    if (HIadd_hash_dd(file_rec, dd->tag, dd->ref, access_rec->block,
                      access_rec->idx) == FAIL)
      {
          access_rec->used = FALSE;
          HRETURN_ERROR(DFE_CANTHASH, FAIL);
      }     

    /* update access record and file record */
    access_rec->special_func = &linked_funcs;
    access_rec->special = SPECIAL_LINKED;
    access_rec->appendable = FALSE;     /* start data as non-appendable */
    access_rec->flush = FALSE;  /* start data as not needing flushing */

    /* check whether we should seek out to the proper position */
    if(old_posn>0)
      {
	if(Hseek(aid,old_posn,DF_START)==FAIL)
	  {
              access_rec->used = FALSE;
              HRETURN_ERROR(DFE_BADSEEK, FAIL);
	  } /* end if */
      } /* end if */

    return (SUCCEED);
}   /* end HLconvert() */

/* ---------------------------- HDinqblockinfo ---------------------------- */
/*
NAME
   HDinqblockinfo -- return info about linked blocks
USAGE
   int32 HDinqblockinfo(aid, length, flength, blen, nblocks)
   int32   aid;          IN:  aid of element
   int32 * length;       OUT: total element length
   int32 * flength;      OUT: length of first element
   int32 * blen;         OUT: length of block elements
   int32 * nblocks;      OUT: number of blocks per block header
RETURNS
   SUCCEED / FAIL
DESCRIPTION
   Given an aid, return low level special info for linked-block
   element in space provided.  This function works like HDinquire()
   but provides more low level info than HLPinquire.  NULL can
   be passed for any non-interesting entries.

   hdfpack is the only application that I know of which uses
   this function.

---------------------------------------------------------------------------*/
int
HDinqblockinfo(int32 aid, int32 *length, int32 *first_length,
               int32 *block_length, int32 *number_blocks)
{
    accrec_t   *arec;
    CONSTR(FUNC, "HDinqblockinfo");

    HEclear();
    if ((arec = AID2REC(aid)) == (accrec_t *) NULL)
        HRETURN_ERROR(DFE_BADAID, FAIL);

    if (arec->special != SPECIAL_LINKED)
        HRETURN_ERROR(DFE_ARGS, FAIL);

    if (length)
        *length = ((linkinfo_t *) (arec->special_info))->length;
    if (first_length)
        *first_length = ((linkinfo_t *) (arec->special_info))->first_length;
    if (block_length)
        *block_length = ((linkinfo_t *) (arec->special_info))->block_length;
    if (number_blocks)
        *number_blocks = ((linkinfo_t *) (arec->special_info))->number_blocks;

    return (SUCCEED);
}   /* HDinqblockinfo */

/* ----------------------------- HLIstaccess ------------------------------ */
/*
NAME
   HLIstaccess -- set up AID to access a linked block elem
USAGE
   int32 HLIstaccess(access_rec, acc_mode)
   access_t * access_rec;   IN: access record to fill in
   int16      acc_mode;     IN: access mode
RETURNS
   The AID of the access record on success FAIL on error.
DESCRIPTION
   Calls to HLIstread and HLIstwrite resolve to this function.
   Given an active AID fill in all of the special information.
   If this information has already been read in for a different
   element use that else we must go out to the HDF file and
   pull in the information ourselves

----------------------------------------------------------------------------*/
PRIVATE int32
HLIstaccess(accrec_t * access_rec, int16 acc_mode)
{
    CONSTR(FUNC, "HLIstaccess");    /* for HERROR */
    filerec_t  *file_rec;       /* file record */
    linkinfo_t *info;           /* information about data elt */
    dd_t       *dd;             /* dd for the special elt */
    uint8       local_ptbuf[14];

    /* validate file record id */
    file_rec = FID2REC(access_rec->file_id);
    if (BADFREC(file_rec) || !(file_rec->access & acc_mode))
        HRETURN_ERROR(DFE_ARGS, FAIL);

    /* set up some data in access record */
    access_rec->special = SPECIAL_LINKED;
    access_rec->posn = 0;
    access_rec->access = acc_mode|DFACC_READ;
    dd = &access_rec->block->ddlist[access_rec->idx];

    /*
     * Lets free old special info first,if one exists,
     * before copying a new one
     */
    if (access_rec->special_info != NULL)
      {   /* special information record */
          linkinfo_t *t_info = (linkinfo_t *) access_rec->special_info;

          if (--(t_info->attached) == 0)
            {
                link_t     *t_link; /* current link to free */
                link_t     *next;   /* next link to free */

                /* free the linked list of links/block tables */
                for (t_link = t_info->link; t_link; t_link = next)
                  {
                      next = t_link->next;
                      HDfreespace((VOIDP) t_link->block_list);
                      HDfreespace((VOIDP) t_link);
                  }
                HDfreespace((VOIDP) t_info);
                access_rec->special_info = NULL;
            }
      }

    /* if the special information are already in some other acc elt,
     * point to it 
     */
    access_rec->special_info = HIgetspinfo(access_rec, dd->tag, dd->ref);
    if (access_rec->special_info)
      {
          ((linkinfo_t *) access_rec->special_info)->attached++;
          file_rec->attach++;
          return ASLOT2ID(access_rec - access_records);
      }

    /* read in the information from file */
    if (HI_SEEK(file_rec->file, dd->offset + 2) == FAIL)
        HRETURN_ERROR(DFE_SEEKERROR, FAIL);

    if (HI_READ(file_rec->file, local_ptbuf, 14) == FAIL)
        HRETURN_ERROR(DFE_READERROR, FAIL);

    access_rec->special_info = (VOIDP) HDgetspace((uint32) sizeof(linkinfo_t));
    info = (linkinfo_t *) access_rec->special_info;
    if (!info)
        HRETURN_ERROR(DFE_NOSPACE, FAIL);

    {
        uint8      *p = local_ptbuf;
        INT32DECODE(p, info->length);
        INT32DECODE(p, info->block_length);
        INT32DECODE(p, info->number_blocks);
        UINT16DECODE(p, info->link_ref);
    }

    /* set up the block tables of the information */
    info->link = HLIgetlink(access_rec->file_id,
                            info->link_ref, info->number_blocks);
    if (!info->link)
      {
          HDfreespace((VOIDP) info);
          return FAIL;
      }
    if (info->link->block_list[0].ref)
      {
          info->first_length = Hlength(access_rec->file_id, DFTAG_LINKED,
                                       info->link->block_list[0].ref);
          if (info->first_length == FAIL)
            {
                HDfreespace((VOIDP) info->link);
                HDfreespace((VOIDP) info);
                HRETURN_ERROR(DFE_INTERNAL, FAIL);
            }
      }
    else
        info->first_length = info->block_length;

    info->last_link = info->link;
    while (info->last_link->nextref != 0)
      {
          info->last_link->next = HLIgetlink(access_rec->file_id,
                                             info->last_link->nextref,
                                             info->number_blocks);
          if (!info->last_link->next)
            {
                link_t     *l, *next;

                for (l = info->link; l; l = next)
                  {
                      next = l->next;
                      if (l->block_list)
                          HDfreespace((VOIDP) l->block_list);
                      HDfreespace((VOIDP) l);
                  }
                HDfreespace((VOIDP) info);
                HRETURN_ERROR(DFE_INTERNAL, FAIL);
            }
          info->last_link = info->last_link->next;
      }
    /* update data */
    info->attached = 1;
    file_rec->attach++;

    return ASLOT2ID(access_rec - access_records);
}   /* HLIstaccess */

/* ------------------------------ HLPstread ------------------------------- */
/*
NAME
   HLPstread -- open an access record for reading
USAGE
   int32 HLPstread(access_rec)
   access_t * access_rec;   IN: access record to fill in
RETURNS
   The AID of the access record on success FAIL on error.
DESCRIPTION
   Calls to HLIstaccess to fill in the access rec for
   reading

---------------------------------------------------------------------------*/
int32
HLPstread(accrec_t * access_rec)
{
    return HLIstaccess(access_rec, DFACC_READ);
}   /* HLPstread */

/* ------------------------------ HLPstwrite ------------------------------- */
/*
NAME
   HLPstwrite -- open an access record for writing
USAGE
   int32 HLPstwrite(access_rec)
   access_t * access_rec;   IN: access record to fill in
RETURNS
   The AID of the access record on success FAIL on error.
DESCRIPTION
   Calls to HLIstaccess to fill in the access rec for
   writing

---------------------------------------------------------------------------*/
int32
HLPstwrite(accrec_t * access_rec)
{
    return HLIstaccess(access_rec, DFACC_WRITE);
}   /* HLPstwrite */

/* ------------------------------ HLIgetlink ------------------------------ */
/*
NAME
   HLIgetlink -- get link information
USAGE
   link_t * HLIgetlink(fid, ref, num_blocks)
   int32  file_id;             IN: the file
   uint16 ref;                 IN: ref number of the link table
   int32  num_blocks;          IN: number of blocks in the table
RETURNS
   A pointer to a link_t or NULL.
DESCRIPTION
   Read a block table out of the file and return a pointer to
   the internal table representing it.

   It seems that num_blocks is redundant.

---------------------------------------------------------------------------*/
PRIVATE link_t *
HLIgetlink(int32 file_id, uint16 ref, int32 number_blocks)
{
    CONSTR(FUNC, "HLIgetlink");     /* for HERROR */
    int32       access_id;      /* access record id */
    uint8      *buffer;
    uint16      tag = DFTAG_LINKED;

    /* allocate necessary memory for block table */
    link_t     *new_link = (link_t *) HDgetspace((uint32) sizeof(link_t));
    if (new_link == NULL)
        HRETURN_ERROR(DFE_NOSPACE, NULL);

    new_link->block_list = (block_t *) HDgetspace((uint32) number_blocks
                                                  * sizeof(block_t));
    if (new_link->block_list == NULL)
      {
          HDfreespace((VOIDP) new_link);
          HRETURN_ERROR(DFE_NOSPACE, NULL);
      }
    new_link->next = (link_t *) NULL;

    /* read block table into buffer */
    buffer = (uint8 *) HDgetspace((uint32) (2 + 2 * number_blocks));
    if (buffer == NULL)
      {
          HDfreespace((VOIDP) new_link->block_list);
          HDfreespace((VOIDP) new_link);
          HRETURN_ERROR(DFE_NOSPACE, NULL);
      }     

    access_id = Hstartread(file_id, tag, ref);
    if (access_id == FAIL ||
        Hread(access_id, 2 + 2 * number_blocks, (VOIDP)buffer) == FAIL)
      {
          HDfreespace((VOIDP) buffer);
          HDfreespace((VOIDP) new_link->block_list);
          HDfreespace((VOIDP) new_link);
          HRETURN_ERROR(DFE_READERROR, NULL);
      }

    {
        int32 i;
        uint8      *p = buffer;

        UINT16DECODE(p, new_link->nextref);
        for (i = 0; i < number_blocks; i++)
            UINT16DECODE(p, new_link->block_list[i].ref);
    }
    Hendaccess(access_id);
    HDfreespace((VOIDP) buffer);

    return new_link;
}   /* HLIgetlink */

/* ------------------------------- HLPseek -------------------------------- */
/*
NAME
   HLPseek -- set the seek posn
USAGE
   int32 HLPseek(access_rec, offset, origin)
   access_t * access_rec;      IN: access record to mess with
   int32      offset;          IN: seek offset
   int32      origin;          IN: where we should calc the offset from
RETURNS
   SUCCEED / FAIL
DESCRIPTION
   Set the seek posn in the given linked block element

---------------------------------------------------------------------------*/
int32
HLPseek(accrec_t * access_rec, int32 offset, int origin)
{
    CONSTR(FUNC, "HLPseek");    /* for HERROR */

    /* validate access record */
    if (access_rec->special != SPECIAL_LINKED)
        HRETURN_ERROR(DFE_INTERNAL, FAIL);

    /* adjust the offset according to origin and validate */
    /* there is no upper bound to posn */
    if (origin == DF_CURRENT)
        offset += access_rec->posn;
    if (origin == DF_END)
        offset += ((linkinfo_t *) (access_rec->special_info))->length;
    if (offset < 0)
        HRETURN_ERROR(DFE_RANGE, FAIL);

    /* set position */
    access_rec->posn = offset;

    return SUCCEED;
}   /* HLPseek */

/* ------------------------------- HLPread -------------------------------- */
/*
NAME
   HLPread -- read some data out of a linked block element
USAGE
   int32 HLPseek(access_rec, length, data)
   access_t * access_rec;      IN: access record to mess with
   int32      length;          IN: number of bytes to read
   VOIDP      data;            IN: buffer for data
RETURNS
   The number of bytes read or FAIL on error
DESCRIPTION
   Read in some data from a linked block element.  If length
   is zero read until the end of the element.  It is assumed
   that the data buffer is big enough to store the data.
   If length would take us off the end of the element only
   read what has been written.

--------------------------------------------------------------------------- */
int32
HLPread(accrec_t * access_rec, int32 length, VOIDP datap)
{
    CONSTR(FUNC, "HLPread");    /* for HERROR */
    uint8      *data = (uint8 *) datap;

    /* information record for this special data elt */
    linkinfo_t *info = (linkinfo_t *) (access_rec->special_info);
    link_t     *t_link = info->link;    /* block table record */

    /* relative position in linked block of data elt */
    int32       relative_posn = access_rec->posn;

    int32       block_idx;      /* block table index of current block */
    int32       current_length; /* length of current block */
    int32       nbytes = 0;     /* # bytes read on any single Hread() */
    int32       bytes_read = 0; /* total # bytes read for this call of HLIread */

    /* validate length */
    if (length == 0)
        length = info->length - access_rec->posn;
    else
/*      if (length < 0 || access_rec->posn + length > info->length) { */
    if (length < 0)
        HRETURN_ERROR(DFE_RANGE, FAIL);

    if (access_rec->posn + length > info->length)
        length = info->length - access_rec->posn;

    /* search for linked block to start reading from */
    if (relative_posn < info->first_length)
      {
          block_idx = 0;
          current_length = info->first_length;
      }
    else
      {
          relative_posn -= info->first_length;
          block_idx = relative_posn / info->block_length + 1;
          relative_posn %= info->block_length;
          current_length = info->block_length;
      }

    {
        int32 i;

        for (i = 0; i < block_idx / info->number_blocks; i++)
          {
              if (t_link == NULL)
                  HRETURN_ERROR(DFE_INTERNAL, FAIL);
              t_link = t_link->next;
          }
    }
    block_idx %= info->number_blocks;

    /* found the starting block, now read in the data */
    do
      {
          int32 remaining =    /* remaining data in current block */
          current_length - relative_posn;

          /* read in the data in this block */
          if (remaining > length)
              remaining = length;
          if (t_link->block_list[block_idx].ref != 0)
            {
                int32       access_id;  /* access record id for this block */
                block_t    *current_block =     /* record on the current block */
                &(t_link->block_list[block_idx]);

                access_id = Hstartread(access_rec->file_id, DFTAG_LINKED,
                                       current_block->ref);
                if (access_id == (int32) FAIL
                    || (relative_posn
                && (int32) FAIL == Hseek(access_id, relative_posn, DF_START))
                    || (int32) FAIL == (nbytes = Hread(access_id, remaining, (VOIDP)data)))
                    HRETURN_ERROR(DFE_READERROR, FAIL);

                bytes_read += nbytes;
                Hendaccess(access_id);
            }
          else
            {   /*if block is missing, fill this part of buffer with zero's */
#ifdef OLD_WAY
                int32 i;

                for (i = 0; i < remaining; i++)
                    data[i] = '\0';
#else  /* OLD_WAY */
                HDmemset(data, 0, remaining);
#endif /* OLD_WAY */
                bytes_read += nbytes;
            }

          /* move variables for the next block */
          data += remaining;
          length -= remaining;
          if (length > 0 && ++block_idx >= info->number_blocks)
            {
                block_idx = 0;
                t_link = t_link->next;
                if (t_link == NULL)
                    HRETURN_ERROR(DFE_INTERNAL, FAIL);
            }
          relative_posn = 0;
          current_length = info->block_length;
      }
    while (length > 0);     /* if still somemore to read in, repeat */

    access_rec->posn += bytes_read;
    return bytes_read;
}   /* HLPread  */

/* ------------------------------- HLPwrite ------------------------------- */
/*
NAME
   HLPwrite -- write out some data to a linked block
USAGE
   int32 HLPseek(access_rec, length, data)
   access_t * access_rec;      IN: access record to mess with
   int32      length;          IN: number of bytes to read
   VOIDP      data;            IN: buffer for data
RETURNS
   The number of bytes written or FAIL on error
DESCRIPTION
   Write out some data from a linked block element.  If we write
   passed the end of the existing element new blocks are created
   as needed.

---------------------------------------------------------------------------*/
int32
HLPwrite(accrec_t * access_rec, int32 length, const VOIDP datap)
{
    CONSTR(FUNC, "HLPwrite");   /* for HERROR */
    uint8      *data = (uint8 *) datap;
    filerec_t  *file_rec =      /* file record */
    FID2REC(access_rec->file_id);
    dd_t       *info_dd =       /* dd of the special info record */
    &(access_rec->block->ddlist[access_rec->idx]);
    linkinfo_t *info =          /* linked blocks information record */
    (linkinfo_t *) (access_rec->special_info);
    link_t     *t_link =        /* ptr to link block table */
    info->link;
    int32       relative_posn = /* relative position in linked block */
    access_rec->posn;
    int32       block_idx;      /* block table index of current block */
    link_t     *prev_link = NULL;   /* ptr to block table before current block table.
                                       for groking the offset of
                                       current block table */
    int32       current_length; /* length of current block */
    int32       nbytes = 0;     /* #bytes written by any single Hwrite */
    int32       bytes_written = 0;  /* total #bytes written by HLIwrite */
    uint8       local_ptbuf[4];

    /* validate length and file records */
    if (length <= 0)
        HRETURN_ERROR(DFE_RANGE, FAIL);
    if (BADFREC(file_rec))
        HRETURN_ERROR(DFE_INTERNAL, FAIL);

    /* determine linked block and position to start writing into */
    /* determine where to start.  Setup missing block tables
       along the way. */
    if (relative_posn < info->first_length)
      {
          block_idx = 0;
          current_length = info->first_length;
      }
    else
      {
          relative_posn -= info->first_length;
          block_idx = relative_posn / info->block_length + 1;
          relative_posn %= info->block_length;
          current_length = info->block_length;
      }
    {
        /* follow the links of block tables and create missing
           block tables along the way */
        int32 num_links;   /* number of links to follow */

        for (num_links = block_idx / info->number_blocks; num_links > 0; num_links--)
          {
              if (!t_link->next)
                {   /* create missing link (block table) */
                    t_link->nextref = Hnewref(access_rec->file_id);
                    t_link->next = HLInewlink(access_rec->file_id,
                                   info->number_blocks, t_link->nextref, 0);
                    if (!t_link->next)
                        return FAIL;
                    {   /* AA */
                        /* update previous link with information about new link */

                        uint16      link_tag = DFTAG_LINKED;
                        uint16      link_ref =  /* ref of current link */
                        (uint16) (prev_link != NULL ?
                                  prev_link->nextref : info->link_ref);

                        uint8      *p = local_ptbuf;    /* temp buf ptr */

                        /* write file the updated portion of current link */

                        int32       link_id =   /* access id for current link */
                        Hstartwrite(access_rec->file_id, link_tag, link_ref, 0);

                        if (link_id == FAIL)
                            HRETURN_ERROR(DFE_WRITEERROR, FAIL);
                        UINT16ENCODE(p, t_link->nextref);
                        if (Hwrite(link_id, 2, (VOIDP)local_ptbuf) == FAIL)
                            HRETURN_ERROR(DFE_WRITEERROR, FAIL);
                        Hendaccess(link_id);
                    }   /* AA */
                }   /* if not t_link->next */

              /* move to the next link */
              prev_link = t_link;
              t_link = t_link->next;
          }     /* end for */
    }   /* end block statement(bad) */

    block_idx %= info->number_blocks;

    /* start writing in that block */
    do
      {
          int32       access_id;    /* access record id */
          int32       remaining =   /* remaining data length in this block */
          current_length - relative_posn;
          uint16      new_ref = 0;  /* ref of newly created block */

          /* determine length and write this block */
          if (remaining > length)
              remaining = length;

          /* this block already exist, so just set up access to it */
          if (t_link->block_list[block_idx].ref != 0)
            {
                block_t    *current_block =     /* ptr to current block record */
                &(t_link->block_list[block_idx]);

                access_id = Hstartwrite(access_rec->file_id, DFTAG_LINKED,
                                        current_block->ref, current_length);
            }   
          else
            {   /* block is missing, set up a new block */
                new_ref = Hnewref(access_rec->file_id);
                access_id = Hstartwrite(access_rec->file_id, DFTAG_LINKED,
                                        new_ref, current_length);
            }

          if (access_id == (int32) FAIL)
              HRETURN_ERROR(DFE_WRITEERROR, FAIL);

          if ((relative_posn &&
               (int32) FAIL == Hseek(access_id, relative_posn, DF_START)) ||
              (int32) FAIL == (nbytes = Hwrite(access_id, remaining, (VOIDP)data)))
            {
                HRETURN_ERROR(DFE_WRITEERROR, FAIL);
            }
          Hendaccess(access_id);
          bytes_written += nbytes;

          if (new_ref)
            {   /* created a new block, so update the link/block table */
                uint16      link_tag = DFTAG_LINKED;
                uint16      link_ref =  /* ref of the current link/block table */
                (uint16) (prev_link ? prev_link->nextref : info->link_ref);
                uint8      *p = /* temp buffer ptr */
                local_ptbuf;
                int32       link_id =   /* access record id of the current
                                           link/block table */
                Hstartwrite(access_rec->file_id, link_tag, link_ref, 0);

                if (link_id == FAIL)
                    HRETURN_ERROR(DFE_WRITEERROR, FAIL);
                UINT16ENCODE(p, new_ref);
                if (Hseek(link_id, 2 + 2 * block_idx, DF_START) == FAIL)
                    HRETURN_ERROR(DFE_SEEKERROR, FAIL);
                if (Hwrite(link_id, 2, (VOIDP)local_ptbuf) == FAIL)
                    HRETURN_ERROR(DFE_WRITEERROR, FAIL);
                Hendaccess(link_id);

                /* update memory structure */
                t_link->block_list[block_idx].ref = new_ref;
            }   /* if new_ref */

          /* move ptrs and counters for next phase */
          data += remaining;
          length -= remaining;

          if (length > 0 && ++block_idx >= info->number_blocks)
            {  /* move to the next link/block table */
                block_idx = 0;
                if (!t_link->next)
                  {     /* create missing link/block table */
                      t_link->nextref = Hnewref(access_rec->file_id);
                      t_link->next = HLInewlink(access_rec->file_id,
                                   info->number_blocks, t_link->nextref, 0);
                      if (!t_link->next)
                          HRETURN_ERROR(DFE_NOSPACE, FAIL);

                      {     /* BB */
                          uint16      link_tag = DFTAG_LINKED;
                          uint16      link_ref =    /* ref of current link/block table */
                          (uint16) (prev_link ? prev_link->nextref : info->link_ref);
                          uint8      *p =   /* temp buffer ptr */
                          local_ptbuf;
                          int32       link_id =     /* access record id of
                                                       current link/block table */
                          Hstartwrite(access_rec->file_id, link_tag,
                                      link_ref, 0);

                          if (link_id == FAIL)
                              HRETURN_ERROR(DFE_WRITEERROR, FAIL);
                          UINT16ENCODE(p, t_link->nextref);
                          if (Hwrite(link_id, 2, (VOIDP)local_ptbuf) == FAIL)
                              HRETURN_ERROR(DFE_WRITEERROR, FAIL);
                          Hendaccess(link_id);
                      }     /* BB */
                  }     /* if not t_link->next  */

                /* move to the next link/block table */
                prev_link = t_link;
                t_link = t_link->next;
            }   /* end if "length" */

          /* update vars for next phase */
          relative_posn = 0;
          current_length = info->block_length;
      }
    while (length > 0);

    if (HI_SEEK(file_rec->file, info_dd->offset + 2) == FAIL)
        HRETURN_ERROR(DFE_SEEKERROR, FAIL);

    {
        int32       tmp;

        tmp = bytes_written + access_rec->posn;
        if (tmp > info->length)
            info->length = tmp;
    }
    {
        uint8      *p = local_ptbuf;
        INT32ENCODE(p, info->length);
    }
    if (HI_WRITE(file_rec->file, local_ptbuf, 4) == FAIL)
        HRETURN_ERROR(DFE_WRITEERROR, FAIL);

    access_rec->posn += bytes_written;
    /* return SUCCEED; */
    /* if wrong # bytes written, FAIL has already been returned */
    return bytes_written;
}   /* HLPwrite */

/* ------------------------------ HLInewlink ------------------------------ */
/*
NAME
   HLInewlink -- write out some data to a linked block
USAGE
   link_t * HLInewlink(fid, nblocks, link_ref, first_block_ref)
   int32  fid;               IN: file ID
   int32  nblocks;           IN: number of blocks in the table
   uint16 link_ref;          IN: ref number for the table
   uint16 first_block_ref;   IN: ref number for first block
RETURNS
   A pointer to a new link/block table or NULL
DESCRIPTION
   Create a new link/block table in memory and in file returns
   ptr to the new link/block table.

---------------------------------------------------------------------------*/
PRIVATE link_t *
HLInewlink(int32 file_id, int32 number_blocks,
           uint16 link_ref, uint16 first_block_ref)
{
    CONSTR(FUNC, "HLInewlink");     /* for HERROR */
    int32       link_id;        /* access record id of new link */
    uint8      *buf;            /* temp buffer */

    /* set up new link record in memory */
    /* new link record */
    link_t     *t_link = (link_t *) HDgetspace((uint32) sizeof(link_t));

    if (!t_link)
        HRETURN_ERROR(DFE_NOSPACE, NULL);

    t_link->block_list = (block_t *) HDgetspace((uint32) number_blocks
                                                * sizeof(block_t));
    if (!t_link->block_list)
      {
          HDfreespace((VOIDP) t_link);
          HRETURN_ERROR(DFE_NOSPACE, NULL);
      }
    t_link->next = NULL;

    /* write the new link to file */
    link_id = Hstartwrite(file_id, DFTAG_LINKED, link_ref, 2 + 2 * number_blocks);
    if (link_id == FAIL)
      {
          HDfreespace((VOIDP) t_link->block_list);
          HDfreespace((VOIDP) t_link);
          HRETURN_ERROR(DFE_WRITEERROR, NULL);
      }
    {   /* CC */
        int32 i;       /* temp int index */
        uint8      *p;          /* temp buffer ptr */

        p = buf = (uint8 *) HDgetspace((uint32) (2 + 2 * number_blocks));
        if (!buf)
          {
              HDfreespace((VOIDP) t_link->block_list);
              HDfreespace((VOIDP) t_link);
              HRETURN_ERROR(DFE_NOSPACE, NULL);
          }

        /* set up the record and write to file */
        t_link->nextref = 0;
        UINT16ENCODE(p, 0);
        t_link->block_list[0].ref = first_block_ref;
        UINT16ENCODE(p, first_block_ref);

        for (i = 1; i < number_blocks; i++)
          {     /* set up each block in this link */
              t_link->block_list[i].ref = 0;
              UINT16ENCODE(p, 0);
          }
    }   /* CC */

    /* write the link */
    if (Hwrite(link_id, 2 + 2 * number_blocks, (VOIDP)buf) == FAIL)
      {
          HDfreespace((VOIDP) buf);
          HDfreespace((VOIDP) t_link->block_list);
          HDfreespace((VOIDP) t_link);
          HRETURN_ERROR(DFE_WRITEERROR, NULL);
      }
    HDfreespace((VOIDP) buf);
    Hendaccess(link_id);

    return t_link;
}   /* HLInewlink */

/* ------------------------------ HLPinquire ------------------------------ */
/*
NAME
   HLPinquire -- Hinquire for linked blocks
USAGE
   int32 HLPinquire(access_rec, fid, tag, ref, len, off, pos, acc, sp)
   access_t * access_rec;      IN:  access record to return info about
   uint16   * file;            OUT: file ID;
   uint16   * tag;             OUT: tag of info record;
   uint16   * ref;             OUT: ref of info record;
   int32    * len;             OUT: length of element;
   int32    * off;             OUT: offset of element -- meaningless
   int32    * pos;             OUT: current position in element;
   int16    * acc;             OUT: access mode;
   int16    * sp;              OUT: special code;
RETURNS
   SUCCEED
DESCRIPTION
   Return interesting information about a linked block element.
   NULL can be passed for any of the OUT parameters if their
   value is not needed.

--------------------------------------------------------------------------- */
int32
HLPinquire(accrec_t * access_rec, int32 *pfile_id, uint16 *ptag,
           uint16 *pref, int32 *plength, int32 *poffset, int32 *pposn,
           int16 *paccess, int16 *pspecial)
{
    dd_t       *info_dd =       /* dd of special information */
    &(access_rec->block->ddlist[access_rec->idx]);
    linkinfo_t *info =          /* special information record */
    (linkinfo_t *) access_rec->special_info;

    /* fill in the variables if they are present */
    if (pfile_id)
        *pfile_id = access_rec->file_id;
    if (ptag)
        *ptag = info_dd->tag;
    if (pref)
        *pref = info_dd->ref;
    if (plength)
        *plength = info->length;
    if (poffset)
        *poffset = 0;   /* meaningless */
    if (pposn)
        *pposn = access_rec->posn;
    if (paccess)
        *paccess = access_rec->access;
    if (pspecial)
        *pspecial = access_rec->special;

    return SUCCEED;
}   /* HLPinquire */

/* ----------------------------- HLPendaccess ----------------------------- */
/*
NAME
   HLPendacess -- close a linked block AID
USAGE
   intn HLPendaccess(access_rec)
   access_t * access_rec;      IN:  access record to close
RETURNS
   SUCCEED / FAIL
DESCRIPTION
   Free up all of the space used to store information about a
   linked block element.  Information is flushed to disk as
   it is created so this routine does NOT have to write anything
   out.

--------------------------------------------------------------------------- */
intn
HLPendaccess(accrec_t * access_rec)
{
#ifdef LATER
    CONSTR(FUNC, "HLPendaccess");   /* for HERROR */
#endif
    linkinfo_t *info =          /* special information record */
    (linkinfo_t *) access_rec->special_info;
    filerec_t  *file_rec =      /* file record */
    FID2REC(access_rec->file_id);

    /* detach the special information record.
       If no more references to that, free the record */
    if (--(info->attached) == 0)
      {
          link_t     *t_link;   /* current link to free */
          link_t     *next;     /* next link to free */

          /* free the linked list of links/block tables */
          for (t_link = info->link; t_link; t_link = next)
            {
                next = t_link->next;
                HDfreespace((VOIDP) t_link->block_list);
                HDfreespace((VOIDP) t_link);
            }

          HDfreespace((VOIDP) info);
          access_rec->special_info = NULL;
      }

    /* detach from the file */
    file_rec->attach--;

    /* free the access record */
    access_rec->used = FALSE;

    return SUCCEED;
}   /* HLPendaccess */

/* ------------------------------- HLPinfo -------------------------------- */
/*
NAME
   HLPinfo -- return info about a linked block element
USAGE
   int32 HLPinfo(access_rec, info_block)
   access_t        * access_rec;
   IN: access record of access element
   sp_info_block_t * info_block;
   OUT: information about the special element
RETURNS
   SUCCEED / FAIL
DESCRIPTION
   Return information about the given linked block.  Info_block is
   assumed to be non-NULL.

--------------------------------------------------------------------------- */
int32
HLPinfo(accrec_t * access_rec, sp_info_block_t * info_block)
{
    char       *FUNC = "HLPinfo";   /* for HERROR */
    linkinfo_t *info =          /* special information record */
    (linkinfo_t *) access_rec->special_info;

    /* validate access record */
    if (access_rec->special != SPECIAL_LINKED)
        HRETURN_ERROR(DFE_INTERNAL, FAIL);

    /* fill in the info_block */
    info_block->key = SPECIAL_LINKED;

    info_block->first_len = info->first_length;
    info_block->block_len = info->block_length;
    info_block->nblocks = info->number_blocks;

    return SUCCEED;

}   /* HLPinfo */
