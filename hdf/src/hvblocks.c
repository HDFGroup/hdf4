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
static char RcsId[] = "$Revision$";
#endif

/* $Id$ */

/*LINTLIBRARY */
/* ------------------------------ hvblocks.c -------------------------------
   routines to implement variable-length stream elements

   Variable-length block  element in HDF files created in two ways
   -- created from the start or
   -- converted from a normal data element

   A variable-length block element is a special element.

   Special elements are
   flagged with a set high-bit in their tag.  Thus, a tag t has
   BASETAG == t & 0x7f and is a special tag if t & 0x80 != 0x00

   The first 16 bits of the meta-element that this tag/ref points to
   tells us what type of special element this is.  If these 16 bits is
   SPECIAL_STREAM, then it contains information about the var-len stream.
   After this 16 bits, is a 16-bit version number of the var-len stream info
   (in case the format changes later).

=======Update this for the variable length streams when its done -QAK

   32 bit which is the length of each block, after
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

========= End of Section to update -QAK

   For now, HVcreate() has the best description of what the on-disk
   representation of a variable-lentgh stream element looks like.

EXPORTED ROUTINES

   HVcreate       -- create a variable-length stream element
   HDinqblockinfo -- return info about variable-length stream
   HVPstread      -- open an access record for reading
   HVPstwrite     -- open an access record for writing
   HVPseek        -- set the seek posn
   HVPread        -- read some data out of a variable-length stream element
   HVPwrite       -- write out some data to a variable-length stream
   HVPinquire     -- Hinquire for variable-length stream
   HVPendacess    -- close a variable-length stream AID
   HVPinfo        -- return info about a variable-length stream element
LOCAL ROUTINES
   HVIstaccess -- set up AID to access a variable-length stream elem
   HVIgetlink  -- get link information
   HVInewlink  -- write out some data to a variable-length stream
*/

#include "hdf.h"
#include "hfile.h"
#ifdef PURIFY
#include "purify.h"
#endif /* PURIFY */

/* block_t - record of a stream block. contains the tag, ref & length of the
   data elt that forms the linked block */
typedef struct block_t
  {
      uint16 tag;  /* tag of the stream block */
      uint16 ref;  /* ref of the stream block */
      int32 len;   /* length of the stream block */
  }
block_t;
/* Size of each block information on the disk */
#define DISK_BLOCK_INFO_SIZE 8

/* vlnk_t - a linked list block table.
   similar to the dd block structure */
typedef struct vlnk_t
  {
      intn        dirty;        /* if the header needs to be updated */
      uint16      myref;        /* ref of the current block table */
      uint16      nextref;      /* ref of the next block table */
      uint16      block_len;    /* total length of the blocks in this table */
      int32       num_blocks;   /* number of blocks in this table */
      int32       max_blocks;   /* max. number of allocted blocks in table */
      struct vlnk_t *next;      /* ptr to the next block table */
      struct block_t *block_list;   /* ptr to the block list for this table */
  }
vlnk_t;

/* information on this special linked block data elt */
typedef struct vlnkinfo_t
  {
      intn        attached;     /* how many access records refer to this elt */
      intn        dirty;        /* if the header needs to be updated */
      int32       length;       /* the actual length of the data elt */
      int32       min_size;     /* the smallest block to allocate in the file */
      uint16      link_ref;     /* ref of the first block table structure */
      vlnk_t     *first_vlnk;   /* pointer to the first block table */
      vlnk_t     *last_vlnk;    /* pointer to the last block table */
  }
vlnkinfo_t;

/* Change this if the header information changes */
#define VLINK_RECORD_V 1
/* The initial number of blocks to allocate */
#define VLINK_START_BLOCKS 16
/* The number of blocks to increment by */
#define VLINK_INC_BLOCKS 16
/* The maximum number of blocks in a table (arbitrarily set at 1024) */
#define VLINK_MAX_BLOCKS 1024 

/* private functions */
PRIVATE int32 HVIstaccess
            (accrec_t * access_rec, int16 acc_mode);

PRIVATE intn HVIfreelinks
            (accrec_t * access_rec);

PRIVATE vlnk_t * HVInewlink
            (int32 file_id, uint16 link_ref, dd_t *data_ptr);

PRIVATE vlnk_t * HVIgetlink
            (int32 file_id, uint16 ref);

/* the accessing function table for variable-length linked-blocks */
funclist_t  vlnk_funcs =
{
    HVPstread,
    HVPstwrite,
    HVPseek,
    HVPinquire,
    HVPread,
    HVPwrite,
    HVPendaccess,
    HVPinfo
};

/* ------------------------------------------------------------------------
NAME
   HVcreate -- create a varibale-length stream element
USAGE
   int32 HVcreate(fid, tag, ref)
   int32   fid;         IN: file to put linked block element in
   uint16  tag;         IN: tag of element
   uint16  ref;         IN: ref of element

RETURNS
   The AID of newly created variable-langeht stream element, FAIL on error.
DESCRIPTION
   This routine takes an HDF element and promotes it into a variable-length
   stream element.  Basically, the element becomes a linked list of variable
   length blocks allowing easy appending.  If the element already exists, it
   is promoted to being a variable-length stream element, otherwise a new
   element is created.

   The different blocks of the linked list can be of varying size.  They
   are generally created by appending to the end of the file until another
   piece of data is written to the end of the file and then a new block
   is started at the end of the file.

 --------------------------------------------------------------------------- */
int32
HVcreate(int32 file_id, uint16 tag, uint16 ref)
{
    CONSTR(FUNC, "HVcreate");   /* for HERROR */
    filerec_t  *file_rec;       /* file record */
    accrec_t   *access_rec;     /* access record */
    int         slot;           /* slot in access records */
    dd_t       *dd;             /* ptr to created dd */
    ddblock_t  *data_block;     /* dd lbock of the data dd */
    int32       data_idx;       /* dd list index of data dd */
    vlnkinfo_t *info;           /* information for the linked blocks elt */
    uint16      link_ref;       /* the ref of the link structure
                                   (block table) */
    dd_t       *data_dd;        /* dd of existing regular element */
    uint16      special_tag;    /* special version of this tag */
    uint8       local_ptbuf[16];

    /* clear error stack and validate file record id */
    HEclear();
    file_rec = FID2REC(file_id);
    if (BADFREC(file_rec) || SPECIALTAG(tag)
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

          data_dd->tag = DFTAG_VLINKED_DATA;
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
    dd->length = 14;

    if ((dd->offset = HPgetdiskblock(file_rec, dd->length, TRUE)) == FAIL)
      {
          access_rec->used = FALSE;
          HRETURN_ERROR(DFE_INTERNAL, FAIL);
      }     /* end if */
    access_rec->special_info = (VOIDP) HDmalloc((uint32) sizeof(vlnkinfo_t));
    if (!access_rec->special_info)
      {
          access_rec->used = FALSE;
          HRETURN_ERROR(DFE_NOSPACE, FAIL);
      }

    info = (vlnkinfo_t *) access_rec->special_info;
    info->attached = 1;
    info->length = data_dd ? data_dd->length : 0;
    info->min_size = 0;
    info->dirty = FALSE;
    {
        uint8      *p;
        p = local_ptbuf;
        UINT16ENCODE(p, SPECIAL_VLINKED);
        UINT16ENCODE(p, VLINK_RECORD_V);
        INT32ENCODE(p, info->length);
        INT32ENCODE(p, info->min_size);
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
    info->first_vlnk = HVInewlink(file_id, link_ref,(data_dd ? data_dd : NULL));
    if (!info->first_vlnk)
      {
          access_rec->used = FALSE;
          HRETURN_ERROR(DFE_INTERNAL, FAIL);
      }
    info->last_vlnk=info->first_vlnk;
    info->link_ref = link_ref;
    info->length=(data_dd ? data_dd->length : 0);

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
    access_rec->special_func = &vlnk_funcs;
    access_rec->special = SPECIAL_VLINKED;
    access_rec->posn = 0;
    access_rec->access = DFACC_RDWR;
    access_rec->file_id = file_id;
    access_rec->appendable = TRUE;     /* start data as non-appendable */
    access_rec->flush = FALSE;  /* start data as not needing flushing */
    file_rec->attach++;

    return ASLOT2ID(slot);
}


/* ----------------------------- HVIstaccess ------------------------------ */
/*
NAME
   HVIstaccess -- set up AID to access a variable-length linked block elem
USAGE
   int32 HVIstaccess(access_rec, acc_mode)
   access_t * access_rec;   IN: access record to fill in
   int16      acc_mode;     IN: access mode
RETURNS
   The AID of the access record on success FAIL on error.
DESCRIPTION
   Calls to HSIstread and HSIstwrite resolve to this function.
   Given an active AID fill in all of the special information.
   If this information has already been read in for a different
   element use that else we must go out to the HDF file and
   pull in the information ourselves

----------------------------------------------------------------------------*/
PRIVATE int32
HVIstaccess(accrec_t * access_rec, int16 acc_mode)
{
    CONSTR(FUNC, "HVIstaccess");    /* for HERROR */
    filerec_t  *file_rec;       /* file record */
    vlnkinfo_t *info;           /* information about data elt */
    dd_t       *dd;             /* dd for the special elt */
    int32       sinfo_aid;      /* AID for the special element information */
    uint16      vnum;           /* version number of the vlnk header */
    uint8       local_ptbuf[16];

    /* validate file record id */
    file_rec = FID2REC(access_rec->file_id);
    if (BADFREC(file_rec) || !(file_rec->access & acc_mode))
        HRETURN_ERROR(DFE_ARGS, FAIL);

    /* set up some data in access record */
    access_rec->special = SPECIAL_VLINKED;
    access_rec->posn = 0;
    access_rec->access = acc_mode|DFACC_READ;
    dd = &access_rec->block->ddlist[access_rec->idx];

    /* if the special information are already in some other acc elt,
     * point to it 
     */
    access_rec->special_info = HIgetspinfo(access_rec, dd->tag, dd->ref);
    if (access_rec->special_info)
      {
          ((vlnkinfo_t *) access_rec->special_info)->attached++;
          file_rec->attach++;
          return ASLOT2ID(access_rec - access_records);
      }

    /* Get access to the special element information */
    if((sinfo_aid=Hstartaccess(access_rec->file_id,dd->tag,dd->ref,DFACC_READ))==FAIL)
        HRETURN_ERROR(DFE_BADAID, FAIL);

    /* seek past the special element identifier */
    if(Hseek(sinfo_aid, 2, DF_START) == FAIL)
        HRETURN_ERROR(DFE_SEEKERROR, FAIL);

    /* read in the information from file */
    if(Hread(sinfo_aid, 14, local_ptbuf) == FAIL)
        HRETURN_ERROR(DFE_READERROR, FAIL);

    access_rec->special_info = (VOIDP) HDmalloc((uint32) sizeof(vlnkinfo_t));
    info = (vlnkinfo_t *) access_rec->special_info;
    if (!info)
        HRETURN_ERROR(DFE_NOSPACE, FAIL);

    {
        uint8      *p = local_ptbuf;
        UINT16DECODE(p, vnum);
        INT32DECODE(p, info->length);
        INT32DECODE(p, info->min_size);
        UINT16DECODE(p, info->link_ref);
    }
    info->dirty=FALSE; /* mark the header as unmodified */

    /* close the special element identifier */
    if(Hendaccess(sinfo_aid) == FAIL)
        HRETURN_ERROR(DFE_INTERNAL, FAIL);

    /* set up the block tables of the information */
    info->first_vlnk = HVIgetlink(access_rec->file_id, info->link_ref);
    if (!info->first_vlnk)
      {
          HDfree((VOIDP) info);
          HRETURN_ERROR(DFE_INTERNAL, FAIL);
      }

    info->last_vlnk = info->first_vlnk;
    while (info->last_vlnk->nextref != 0)
      {
          info->last_vlnk->next = HVIgetlink(access_rec->file_id,
                                             info->last_vlnk->nextref);
          if (NULL==info->last_vlnk->next)
            {
                vlnk_t     *l, *next;

                for (l = info->first_vlnk; l; l = next)
                  {
                      next = l->next;
                      if (l->block_list)
                          HDfree((VOIDP) l->block_list);
                      HDfree((VOIDP) l);
                  }
                HDfree((VOIDP) info);
                HRETURN_ERROR(DFE_INTERNAL, FAIL);
            }
          info->last_vlnk = info->last_vlnk->next;
      }

    /* update data */
    info->attached = 1;
    file_rec->attach++;

    return ASLOT2ID(access_rec - access_records);
}   /* HVIstaccess */

/* ------------------------------ HVPstread ------------------------------- */
/*
NAME
   HVPstread -- open an access record for reading
USAGE
   int32 HVPstread(access_rec)
   access_t * access_rec;   IN: access record to fill in
RETURNS
   The AID of the access record on success FAIL on error.
DESCRIPTION
   Calls to HVIstaccess to fill in the access rec for
   reading

---------------------------------------------------------------------------*/
int32
HVPstread(accrec_t * access_rec)
{
    return(HVIstaccess(access_rec, DFACC_READ));
}   /* HVPstread */

/* ------------------------------ HVPstwrite ------------------------------- */
/*
NAME
   HVPstwrite -- open an access record for writing
USAGE
   int32 HVPstwrite(access_rec)
   access_t * access_rec;   IN: access record to fill in
RETURNS
   The AID of the access record on success FAIL on error.
DESCRIPTION
   Calls to HVIstaccess to fill in the access rec for
   writing

---------------------------------------------------------------------------*/
int32
HVPstwrite(accrec_t * access_rec)
{
    return(HVIstaccess(access_rec, DFACC_WRITE));
}   /* HVPstwrite */

/* ------------------------------ HVIfreelinks ------------------------------ */
/*
NAME
   HVIfreelinks -- free link information
USAGE
   intn HVIfreelinks(access_rec)
   accrec_t  *access_rec;      IN: the file
RETURNS
   SUCCEED/FAIL
DESCRIPTION
   Frees the linked blocks for an access record

---------------------------------------------------------------------------*/
PRIVATE intn
HVIfreelinks(accrec_t *access_rec)
{
    CONSTR(FUNC, "HVIfreelinks");     /* for HERROR */
    vlnkinfo_t *s_info = (vlnkinfo_t *) access_rec->special_info;
    vlnk_t     *s_link; /* current link to free */
    vlnk_t     *next;   /* next link to free */
    int32      aid;     /* aid to use when writing data */
    uint8      local_ptbuf[16];

    /* free the linked list of links/block tables */
    if(s_info->dirty==TRUE)
      { /* check if we need to update special info header */
          if((aid=Hstartaccess(access_rec->file_id,ACCREC_TAG(access_rec),ACCREC_REF(access_rec),DFACC_RDWR))==FAIL)
              HRETURN_ERROR(DFE_CANTACCESS,FAIL);
          {
              uint8      *p;

              p = local_ptbuf;
              UINT16ENCODE(p, SPECIAL_VLINKED);
              UINT16ENCODE(p, VLINK_RECORD_V);
              INT32ENCODE(p, s_info->length);
              INT32ENCODE(p, s_info->min_size);
              UINT16ENCODE(p, s_info->link_ref);  /* link_ref */
          }
          if(Hwrite(aid, ACCREC_LEN(access_rec), local_ptbuf) == FAIL)
              HRETURN_ERROR(DFE_WRITEERROR, FAIL);
          if(Hendaccess(aid)==FAIL)
              HRETURN_ERROR(DFE_CANTENDACCESS,FAIL);
      } /* end if */

    for (s_link = s_info->first_vlnk; s_link!=NULL; s_link = next)
      {
        if(s_link->dirty==TRUE)
          { /* update the v-link header on disk */
              int32 header_len, /* header length on the disk */
                new_len;        /* new header length */
              uint8 *buf;       /* pointer to temporary buffer space */
              intn i;           /* local counting variable */

              new_len=2+4+4+2+(s_link->num_blocks*8);
              if(Hexist(access_rec->file_id,DFTAG_VLINKED,s_link->myref)==SUCCEED)
                {
                  if((header_len=Hlength(access_rec->file_id,DFTAG_VLINKED,s_link->myref))==FAIL)
                      HRETURN_ERROR(DFE_INTERNAL,FAIL);
                  if(new_len!=header_len)
                    { /* delete header and re-write if it is too small */
                      if(Hdeldd(access_rec->file_id,DFTAG_VLINKED,s_link->myref)==FAIL)
                          HRETURN_ERROR(DFE_INTERNAL,FAIL);
                    } /* end if */
                } /* end if */
              if((buf=(uint8 *)HDmalloc(new_len))==NULL)
                  HRETURN_ERROR(DFE_NOSPACE,FAIL);
              {
                  uint8      *p;

                  p = buf;
                  UINT16ENCODE(p, VLINK_RECORD_V);
                  INT32ENCODE(p, s_link->block_len);
                  INT32ENCODE(p, s_link->num_blocks);
                  UINT16ENCODE(p, s_link->nextref); 
                  for(i=0; i<s_link->num_blocks; i++)
                    {
                      UINT16ENCODE(p, s_link->block_list[i].tag);
                      UINT16ENCODE(p, s_link->block_list[i].ref);
                      INT32ENCODE(p, s_link->block_list[i].len);
                    } /* end for */
              }

              if((aid=Hstartaccess(access_rec->file_id,DFTAG_VLINKED,s_link->myref,DFACC_RDWR))==FAIL)
                  HRETURN_ERROR(DFE_CANTACCESS,FAIL);
              if(Hwrite(aid, new_len, buf) == FAIL)
                  HRETURN_ERROR(DFE_WRITEERROR, FAIL);
              if(Hendaccess(aid)==FAIL)
                  HRETURN_ERROR(DFE_CANTENDACCESS,FAIL);
              HDfree(buf);
          } /* end if */
        next = s_link->next;
        HDfree((VOIDP) s_link->block_list);
        HDfree((VOIDP) s_link);
      } /* end for */
    HDfree((VOIDP) s_info);

    return(SUCCEED);
}   /* HVIfreelinks */


/* ------------------------------ HVIgetlink ------------------------------ */
/*
NAME
   HVIgetlink -- get link information
USAGE
   vlnk_t * HVIgetlink(fid, ref)
   int32  file_id;             IN: the file
   uint16 ref;                 IN: ref number of the link table
RETURNS
   A pointer to a link_t or NULL.
DESCRIPTION
   Read a block table out of the file and return a pointer to
   the internal table representing it.

   It seems that num_blocks is redundant.

---------------------------------------------------------------------------*/
PRIVATE vlnk_t *
HVIgetlink(int32 file_id, uint16 ref)
{
    CONSTR(FUNC, "HVIgetlink"); /* for HERROR */
    int32       access_id;      /* access record id */
    uint8      *buffer;
    uint16      tag = DFTAG_VLINKED;
    vlnk_t     *new_link;
    uint8      tmp_pbuf[12];    /* temporary buffer for reading block info */
    uint16     v_num;           /* version number of the header */

    if (( new_link = (vlnk_t *) HDmalloc((uint32) sizeof(vlnk_t))) == NULL)
        HRETURN_ERROR(DFE_NOSPACE, NULL);

    new_link->next = NULL;

    if (( access_id = Hstartread(file_id, tag, ref)) == FAIL) 
      {
          HDfree((VOIDP) new_link);
          HRETURN_ERROR(DFE_BADAID, NULL);
      } /* end if */

    if( Hread(access_id, 10, tmp_pbuf) == FAIL)
      {
          HDfree((VOIDP) new_link);
          HRETURN_ERROR(DFE_READERROR, NULL);
      }

    {
        uint8      *p = tmp_pbuf;

        UINT16DECODE(p, v_num);    /* get the version number */
        INT32DECODE(p, new_link->block_len);    /* get the block's byte len */
        UINT16DECODE(p, new_link->num_blocks);    /* get the number of blocks */
        UINT16DECODE(p, new_link->nextref);
    }
    new_link->dirty=FALSE; /* mark the header as unmodified */
    new_link->max_blocks=(new_link->num_blocks>0 ? new_link->num_blocks : VLINK_START_BLOCKS);

    new_link->block_list = (block_t *) HDmalloc((uint32) new_link->max_blocks
                                                  * sizeof(block_t));
    if (new_link->block_list == NULL)
      {
          HDfree((VOIDP) new_link);
          HRETURN_ERROR(DFE_NOSPACE, NULL);
      }
    if(new_link->num_blocks>0) 
      {
        /* read block table into buffer */
        buffer = (uint8 *) HDmalloc((uint32) (DISK_BLOCK_INFO_SIZE * new_link->max_blocks));
        if (buffer == NULL)
          {
              HDfree((VOIDP) new_link->block_list);
              HDfree((VOIDP) new_link);
              HRETURN_ERROR(DFE_NOSPACE, NULL);
          }     

        if( Hread(access_id, DISK_BLOCK_INFO_SIZE * new_link->num_blocks, buffer) == FAIL)
          {
              HDfree((VOIDP) buffer);
              HDfree((VOIDP) new_link->block_list);
              HDfree((VOIDP) new_link);
              HRETURN_ERROR(DFE_READERROR, NULL);
          }

        {
            int32 i;
            uint8      *p = buffer;

            for (i = 0; i < new_link->num_blocks; i++)
          {
                UINT16DECODE(p, new_link->block_list[i].tag);
                UINT16DECODE(p, new_link->block_list[i].ref);
                INT32DECODE(p, new_link->block_list[i].len);
          } /* end for */
        }
        HDfree((VOIDP) buffer);
      } /* end if */
    Hendaccess(access_id);

    return(new_link);
}   /* HVIgetlink */

/* ------------------------------- HVPseek -------------------------------- */
/*
NAME
   HVPseek -- set the seek posn
USAGE
   int32 HVPseek(access_rec, offset, origin)
   access_t * access_rec;      IN: access record to mess with
   int32      offset;          IN: seek offset
   int32      origin;          IN: where we should calc the offset from
RETURNS
   SUCCEED / FAIL
DESCRIPTION
   Set the seek posn in the given linked block element

---------------------------------------------------------------------------*/
int32
HVPseek(accrec_t * access_rec, int32 offset, int origin)
{
    CONSTR(FUNC, "HVPseek");    /* for HERROR */

    /* validate access record */
    if (access_rec->special != SPECIAL_VLINKED)
        HRETURN_ERROR(DFE_INTERNAL, FAIL);

    /* adjust the offset according to origin and validate */
    /* there is no upper bound to posn */
    if (origin == DF_CURRENT)
        offset += access_rec->posn;
    if (origin == DF_END)
        offset += ((vlnkinfo_t *) (access_rec->special_info))->length;
    if (offset < 0)
        HRETURN_ERROR(DFE_RANGE, FAIL);

    /* set position */
    access_rec->posn = offset;

    return(SUCCEED);
}   /* HVPseek */

/* ------------------------------- HVPread -------------------------------- */
/*
NAME
   HVPread -- read some data out of a linked block element
USAGE
   int32 HVPread(access_rec, length, data)
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
HVPread(accrec_t * access_rec, int32 length, VOIDP datap)
{
    CONSTR(FUNC, "HVPread");    /* for HERROR */
    uint8      *data = (uint8 *) datap;

    /* information record for this special data elt */
    vlnkinfo_t *info = (vlnkinfo_t *) (access_rec->special_info);
    vlnk_t     *t_link = info->first_vlnk;    /* block table record */

    /* relative position in linked block of data elt */
    int32       relative_posn = access_rec->posn;
    int32       block_idx;      /* block table index of current block */
    int32       nbytes = 0;     /* # bytes read on any single Hread() */
    int32       bytes_read = 0; /* total # bytes read for this call of HLIread */

    /* validate length */
#ifdef PURIFY
purify_printf("length=%d, info->length=%d, access_rec->posn=%d\n",(int)length,(int)info->length,(int)access_rec->posn);
#endif /* PURIFY */
    if (length == 0) {
        length = info->length - access_rec->posn;
      } /* end if */
    else
/*      if (length < 0 || access_rec->posn + length > info->length)  */
    if (length < 0)
        HRETURN_ERROR(DFE_RANGE, FAIL);

    if (access_rec->posn + length > info->length)
        length = info->length - access_rec->posn;

    if(length==0)
        return(0); /* nothing to read */

    /* search for linked block to start reading from */
    /* find the correct block header */
    while(relative_posn>t_link->block_len)
      {
        relative_posn-=t_link->block_len;
        t_link=t_link->next;
        if(t_link==NULL)  /* can't find the correct header */
            HRETURN_ERROR(DFE_INTERNAL, FAIL);
      } /* end while */

    /* search for correct block */
    block_idx=0;
#ifdef PURIFY
purify_printf("relative_posn=%d, t_link->num_blocks=%d\n",(int)relative_posn,(int)t_link->num_blocks);
purify_printf("length=%d, t_link->block_len=%d\n",(int)length,(int)t_link->block_len);
#endif /* PURIFY */
    while(relative_posn>t_link->block_list[block_idx].len)
      {
        relative_posn-=t_link->block_list[block_idx].len;
        block_idx++;
        if(block_idx>=t_link->num_blocks) /* can't find the correct block */
            HRETURN_ERROR(DFE_INTERNAL, FAIL);
      } /* end while */

    /* found the starting block, now read in the data */
    do
      {
          int32 remaining;    /* remaining data in current block */

          remaining = t_link->block_list[block_idx].len-relative_posn;

          /* read in the data in this block */
          if (remaining > length)
              remaining = length;
          if (t_link->block_list[block_idx].ref != 0)
            {
                int32       access_id;  /* access record id for this block */
                block_t    *current_block=     /* record on the current block */
                    &(t_link->block_list[block_idx]);

                access_id = Hstartaccess(access_rec->file_id,
                current_block->tag, current_block->ref, DFACC_READ);
                if (access_id == (int32) FAIL)
                    HRETURN_ERROR(DFE_INTERNAL, FAIL);

                if(FAIL == Hseek(access_id, relative_posn, DF_START))
                    HRETURN_ERROR(DFE_SEEKERROR, FAIL);

                if(FAIL == (nbytes = Hread(access_id, remaining, data)))
                    HRETURN_ERROR(DFE_READERROR, FAIL);

                if(Hendaccess(access_id)==FAIL)
                    HRETURN_ERROR(DFE_INTERNAL, FAIL);

                bytes_read += nbytes;
            }
          else
            {   /*if block is missing, fill this part of buffer with zero's */
                HDmemset(data, 0, remaining);
                bytes_read += remaining;
            }

          /* move variables for the next block */
          data += remaining;
          length -= remaining;
          if (length > 0 && ++block_idx >= t_link->num_blocks)
            {
                block_idx = 0;
                t_link = t_link->next;
                if (t_link == NULL)
                    HRETURN_ERROR(DFE_INTERNAL, FAIL);
            }
          relative_posn = 0;
      }
    while (length > 0);     /* if still somemore to read in, repeat */

    access_rec->posn += bytes_read;
    return(bytes_read);
}   /* HVPread  */

/* ------------------------------- HVPwrite ------------------------------- */
/*
NAME
   HVPwrite -- write out some data to a linked block
USAGE
   int32 HVPwrite(access_rec, length, data)
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
HVPwrite(accrec_t * access_rec, int32 length, const VOIDP datap)
{
    CONSTR(FUNC, "HVPwrite");   /* for HERROR */
    const uint8 *data = (const uint8 *) datap;
    filerec_t  *file_rec =      /* file record */
        FID2REC(access_rec->file_id);
    vlnkinfo_t *info =          /* linked blocks information record */
        (vlnkinfo_t *) (access_rec->special_info);
    vlnk_t     *t_link =        /* ptr to link block table */
        info->first_vlnk;
    int32       relative_posn = /* relative position in linked block */
        access_rec->posn;
    int32       block_idx;      /* block table index of current block */
    vlnk_t     *prev_link = NULL;   /* ptr to block table before current block
                                       table.  For grokking the offset of
                                       current block table */
    int32       old_length=length; /* original length to write */
    int32       data_aid;       /* AID of the data to write */
    intn        new_block=0;    /* if a new block was created for writing */

    /* validate length and file records */
    if(length <= 0)
        HRETURN_ERROR(DFE_RANGE, FAIL);
    if(BADFREC(file_rec))
        HRETURN_ERROR(DFE_INTERNAL, FAIL);

    /* find the correct block header */
    prev_link=NULL;
    while(relative_posn>t_link->block_len)
      {
        relative_posn-=t_link->block_len;
        prev_link=t_link;
        t_link=t_link->next;
        if(t_link==NULL)  /* can't find the correct header */
            break;
      } /* end while */

    if(t_link==NULL)
      {  /* walked off the existing dataset */
        intn blank_block; /* indicate whether there is a blank block or not */

	/* this is used below in order to figure out how many blocks to add */
        blank_block=(relative_posn>0); 

        if(prev_link->max_blocks>=VLINK_MAX_BLOCKS
            && (prev_link->num_blocks+blank_block)>=VLINK_MAX_BLOCKS)
          { /* have to create a new link header */
            uint16 new_ref;

            new_ref=Hnewref(access_rec->file_id);
            t_link=HVInewlink(access_rec->file_id,new_ref,NULL);
            if(t_link==NULL)
                HRETURN_ERROR(DFE_INTERNAL,FAIL);
            prev_link->next=t_link;
            prev_link->nextref=new_ref;

            if(blank_block)
              { /* insert room for "missing" data */
                t_link->block_list[0].tag=DFTAG_NULL;
                t_link->block_list[0].ref=0;
                t_link->block_list[0].len=relative_posn;
                t_link->num_blocks=1;
                block_idx=1;
              } /* end if */
            else
                block_idx=0;
            t_link->block_len=relative_posn;

            t_link->dirty=TRUE; /* mark the header as modified */
            relative_posn=0;
            new_block=1;  /* indicate that this is a new block */
          } /* end if */
        else
          { /* try to extend existing table */

            t_link=prev_link;
            if((t_link->num_blocks+blank_block)>=t_link->max_blocks)
              { /* extend the current table */
                  block_t *new_blocks;  /* new block table */

                  new_blocks=(block_t *)HDmalloc(sizeof(block_t)
                       *(t_link->max_blocks+VLINK_INC_BLOCKS));
                  if(new_blocks==NULL)
                      HRETURN_ERROR(DFE_NOSPACE,FAIL);
                  HDmemcpy(new_blocks,t_link->block_list,t_link->max_blocks*sizeof(block_t));
                  t_link->max_blocks+=VLINK_INC_BLOCKS;
                  HDfree(t_link->block_list);
                  t_link->block_list=new_blocks;
              } /* end if */
            if(blank_block)
              { /* insert room for "missing" data */
                t_link->block_list[t_link->num_blocks].tag=DFTAG_NULL;
                t_link->block_list[t_link->num_blocks].ref=0;
                t_link->block_list[t_link->num_blocks].len=relative_posn;
                t_link->num_blocks++;
                t_link->dirty=TRUE; /* mark the header as modified */
              } /* end if */
            block_idx=t_link->num_blocks;
            t_link->block_len+=relative_posn;
            relative_posn=0;
            new_block=1;  /* indicate that this is a new block */
          } /* end else */
      } /* end if */
    else
      { /* search for correct block */
printf("t_link->num_blocks=%d, relative_posn=%d\n",(int)t_link->num_blocks,(int)relative_posn);
        if(t_link->num_blocks==0) 
          {
            if(relative_posn>0) 
              { /* insert room for "missing" data */
                t_link->block_list[t_link->num_blocks].tag=DFTAG_NULL;
                t_link->block_list[t_link->num_blocks].ref=0;
                t_link->block_list[t_link->num_blocks].len=relative_posn;
                t_link->num_blocks++;
                t_link->dirty=TRUE; /* mark the header as modified */
              } /* end if */
            block_idx=t_link->num_blocks;
            t_link->block_len+=relative_posn;
            relative_posn=0;
            new_block=1;  /* indicate that this is a new block */
          } /* end if */
        else
          {
            block_idx=0;
            while(relative_posn>t_link->block_list[block_idx].len)
              {
                relative_posn-=t_link->block_list[block_idx].len;
                block_idx++;
                if(block_idx>=t_link->num_blocks) /* can't find the correct block */
                    HRETURN_ERROR(DFE_INTERNAL, FAIL);
              } /* end while */
            if(t_link->block_list[block_idx].len==0)
                new_block=1;
          } /* end else */
      } /* end else */

    /* determine linked block and position to start writing into */
    if(new_block) 
      { /* a new block, therefore put the whole chunk of data out at once */
        uint16 new_ref;

        new_ref=Hnewref(access_rec->file_id);
        if((data_aid=Hstartaccess(access_rec->file_id,DFTAG_VLINKED_DATA,new_ref,DFACC_RDWR))==FAIL)
            HRETURN_ERROR(DFE_CANTACCESS, FAIL);
        if(Hwrite(data_aid,length,(const VOIDP)data)==FAIL)
            HRETURN_ERROR(DFE_WRITEERROR, FAIL);
        if(Hendaccess(data_aid)==FAIL)
            HRETURN_ERROR(DFE_CANTENDACCESS, FAIL);
        t_link->block_list[block_idx].tag=DFTAG_VLINKED_DATA;
        t_link->block_list[block_idx].ref=new_ref;
        t_link->block_list[block_idx].len=length;
        t_link->dirty=TRUE; /* mark the header as modified */
      } /* end if */
    else
      { /* not the last block or the last block is not appendable */
        while(length>0) 
          { /* write out the data in nice chunks */
            int32 write_length; /* the amount of data to write at once */

            write_length=MIN(length,t_link->block_list[block_idx].len-relative_posn);
            if((data_aid=Hstartaccess(access_rec->file_id,t_link->block_list[block_idx].tag,t_link->block_list[block_idx].ref,DFACC_RDWR))==FAIL)
                HRETURN_ERROR(DFE_CANTACCESS, FAIL);
            if(HPisappendable(data_aid))
              /* if current block is appendable & is the last block */
              /* in the variable linked list, then append data to it */
              {
                  if(t_link->next==NULL && block_idx>=t_link->num_blocks)
                    { /* ok, block is the last one the list */
                        if(Happendable(data_aid)!=FAIL)
                          /* block is now appendable, adjust amount of data to write */
                          {
                              write_length=length;
                              t_link->block_list[block_idx].len=relative_posn+length;
                              t_link->dirty=TRUE; /* mark the header as modified */
                          } /* end if */
                    } /* end if */
              } /* end if */
            if(relative_posn>0)
              {
                if(Hseek(data_aid,relative_posn,DF_START)==FAIL)
                    HRETURN_ERROR(DFE_SEEKERROR, FAIL);
                relative_posn=0; /* reset the relative position in the block */
              } /* end if */
            if(Hwrite(data_aid,write_length,(const VOIDP)data)==FAIL)
                HRETURN_ERROR(DFE_WRITEERROR, FAIL);
            if(Hendaccess(data_aid)==FAIL)
                HRETURN_ERROR(DFE_CANTENDACCESS, FAIL);

            length-=write_length; /* decrement # of bytes to write out */
            data+=write_length;

            if((++block_idx)>=t_link->num_blocks && length>0)
              { /* advance to the next block header (if there is one) */
                if(t_link->next!=NULL) 
                  { /* advance to next block header */
                    t_link=t_link->next;
                    block_idx=0;
                    if(t_link->block_list[block_idx].tag==DFTAG_NULL)
                      {
                        t_link->block_list[block_idx].tag=DFTAG_VLINKED_DATA;
                        t_link->block_list[block_idx].ref=Hnewref(access_rec->file_id);
                        t_link->dirty=TRUE; /* mark the header as modified */
                      } /* end if */
                  } /* end if */
                else
                  { /* try to create a new block (and header if necessary) */
                    if(t_link->num_blocks<t_link->max_blocks)
                      { /* add another block to the current header */
                        t_link->num_blocks++;
                        block_idx++;
                        new_block=1;
                      } /* end if */
                    else
                      { /* increase the number of blocks in the header */
                        if(t_link->max_blocks<VLINK_MAX_BLOCKS)
                          { /* try to expand the block table */
                              block_t *new_blocks;  /* new block table */

                              new_blocks=(block_t *)HDmalloc(sizeof(block_t)
                                   *(t_link->max_blocks+VLINK_INC_BLOCKS));
                              if(new_blocks==NULL)
                                  HRETURN_ERROR(DFE_NOSPACE,FAIL);
                              HDmemcpy(new_blocks,t_link->block_list,t_link->max_blocks*sizeof(block_t));
                              t_link->max_blocks+=VLINK_INC_BLOCKS;
                              HDfree(t_link->block_list);
                              t_link->block_list=new_blocks;

                              t_link->num_blocks++;
                              block_idx++;
                              new_block=1;
                          } /* end if */
                        else
                          { /* allocate a new header & table */
                            vlnk_t *new_link;
                            uint16 new_ref;

                            new_ref=Hnewref(access_rec->file_id);
                            new_link=HVInewlink(access_rec->file_id,new_ref,NULL);
                            if(new_link==NULL)
                                HRETURN_ERROR(DFE_INTERNAL,FAIL);
                            
                            /* link the headers together */
                            t_link->next=new_link;
                            new_ref=Hnewref(access_rec->file_id);
                            new_link->myref=new_ref;;
                            t_link->nextref=new_ref;
                            new_link->nextref=0;
                            t_link=new_link;

                            t_link->num_blocks++;
                            block_idx=0;
                            new_block=1;
                          } /* end else */
                      } /* end else */
                  } /* end else */
                    
              } /* end if */
            if(new_block)
              /* we've created a new block in some fashion, write the */
              /* remainder of the data to it */
              { 
                uint16 new_ref;

                new_ref=Hnewref(access_rec->file_id);
                if((data_aid=Hstartaccess(access_rec->file_id,DFTAG_VLINKED_DATA,new_ref,DFACC_RDWR))==FAIL)
                    HRETURN_ERROR(DFE_CANTACCESS, FAIL);
                if(Hwrite(data_aid,length,(const VOIDP)data)==FAIL)
                    HRETURN_ERROR(DFE_WRITEERROR, FAIL);
                if(Hendaccess(data_aid)==FAIL)
                    HRETURN_ERROR(DFE_CANTENDACCESS, FAIL);
                t_link->block_list[block_idx].tag=DFTAG_VLINKED_DATA;
                t_link->block_list[block_idx].ref=new_ref;
                t_link->block_list[block_idx].len=length;
                t_link->dirty=TRUE; /* mark the header as modified */

                length=0; /* no more to write out now */
              } /* end if */
          } /* end while */
      } /* end else */

    return(old_length);
}   /* HVPwrite */

/* ------------------------------ HVInewlink ------------------------------ */
/*
NAME
   HVInewlink -- write out some data to a linked block
USAGE
   vlnk_t * HVInewlink(fid, link_ref, data_ptr)
   int32  fid;               IN: file ID
   uint16 link_ref;          IN: ref number for the table
   dd_t *data_ptr;           IN: ptr to the first data block
RETURNS
   A pointer to a new link/block table or NULL
DESCRIPTION
   Create a new link/block table in memory and in file returns
   ptr to the new link/block table.
NOTES
   Unlike the regular linked-block code, variable length linked blocks are
   not written to the file until they are closed.

---------------------------------------------------------------------------*/
PRIVATE vlnk_t *
HVInewlink(int32 file_id, uint16 link_ref, dd_t *data_ptr)
{
    CONSTR(FUNC, "HVInewlink"); /* for HERROR */
    vlnk_t     *t_link;         /* ptr to the new header */

    if ((t_link=(vlnk_t *)HDmalloc(sizeof(vlnk_t)))==NULL)
        HRETURN_ERROR(DFE_NOSPACE, NULL);

    t_link->block_list = (block_t *) HDmalloc((uint32) VLINK_START_BLOCKS
                                                * sizeof(block_t));
    if (!t_link->block_list)
      {
          HDfree((VOIDP) t_link);
          HRETURN_ERROR(DFE_NOSPACE, NULL);
      }
    t_link->next = NULL;
    t_link->myref = link_ref;
    t_link->nextref = 0;
    t_link->max_blocks = VLINK_START_BLOCKS;

    if(data_ptr!=NULL) /* check for first data block */
      { /* create first data block */
        t_link->block_list[0].tag=data_ptr->tag;
        t_link->block_list[0].ref=data_ptr->ref;
        t_link->block_list[0].len=data_ptr->length;
        t_link->block_len=data_ptr->length;
        t_link->num_blocks=1;
      } /* end if */
    else
        t_link->block_len=t_link->num_blocks=0;

    t_link->dirty=TRUE; /* mark the header as modified */
    return(t_link);
}   /* HVInewlink */

/* ------------------------------ HVPinquire ------------------------------ */
/*
NAME
   HVPinquire -- Hinquire for v-linked blocks
USAGE
   int32 HVPinquire(access_rec, fid, tag, ref, len, off, pos, acc, sp)
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
HVPinquire(accrec_t * access_rec, int32 *pfile_id, uint16 *ptag,
           uint16 *pref, int32 *plength, int32 *poffset, int32 *pposn,
           int16 *paccess, int16 *pspecial)
{
    dd_t       *info_dd =       /* dd of special information */
        &(access_rec->block->ddlist[access_rec->idx]);
    vlnkinfo_t *info =          /* special information record */
        (vlnkinfo_t *) access_rec->special_info;

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

    return(SUCCEED);
}   /* HVPinquire */

/* ----------------------------- HVPendaccess ----------------------------- */
/*
NAME
   HVPendacess -- close a v-linked block AID
USAGE
   intn HVPendaccess(access_rec)
   access_t * access_rec;      IN:  access record to close
RETURNS
   SUCCEED / FAIL
DESCRIPTION
   Free up all of the space used to store information about a
   v-linked block element.  Information is NOT flushed to disk as
   it is created so this routine must write anything modified out.

--------------------------------------------------------------------------- */
intn
HVPendaccess(accrec_t * access_rec)
{
    CONSTR(FUNC, "HVPendaccess");   /* for HERROR */
    vlnkinfo_t *info =          /* special information record */
        (vlnkinfo_t *) access_rec->special_info;
    filerec_t  *file_rec =      /* file record */
        FID2REC(access_rec->file_id);

    /* detach the special information record.
       If no more references to that, free the record */
    if (--(info->attached) <= 0)
      {
          if(HVIfreelinks(access_rec)==FAIL)
              HRETURN_ERROR(DFE_INTERNAL,FAIL);
          access_rec->special_info = NULL;
      }

    /* detach from the file */
    file_rec->attach--;

    /* free the access record */
    access_rec->used = FALSE;

    return SUCCEED;
}   /* HLPendaccess */

/* ------------------------------- HVPinfo -------------------------------- */
/*
NAME
   HVPinfo -- return info about a linked block element
USAGE
   int32 HVPinfo(access_rec, info_block)
   access_t        * access_rec; IN: access record of access element
   sp_info_block_t * info_block; OUT: information about the special element
RETURNS
   SUCCEED / FAIL
DESCRIPTION
   Return information about the given v-linked block.  Info_block is
   assumed to be non-NULL.

--------------------------------------------------------------------------- */
int32
HVPinfo(accrec_t * access_rec, sp_info_block_t * info_block)
{
    char       *FUNC = "HLPinfo";   /* for HERROR */
    vlnkinfo_t *info =          /* special information record */
        (vlnkinfo_t *) access_rec->special_info;

    /* validate access record */
    if (access_rec->special != SPECIAL_LINKED)
        HRETURN_ERROR(DFE_INTERNAL, FAIL);

    /* fill in the info_block */
    info_block->key = SPECIAL_VLINKED;

    info_block->min_block = info->min_size;

    return(SUCCEED);
}   /* HVPinfo */

