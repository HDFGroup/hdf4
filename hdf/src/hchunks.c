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
/* ------------------------------ HMChunks.c -------------------------------
   Routines to implement chunked elements via a Vdatas for
   the chunk table and using a new data tag DFTAG_CHUNK to represent
   each chunk object. As a result the total number of chunks
   for all the chunked elements in an HDF file can only be as 
   large as sizeof(uint16) = 65,536.
   
   NOTE: GeorgeV's standard Disclaimer <here>. I was coerced to
         do it this way....


   Description of file format headers for chunked element
   ------------------------------------------------------
   A chunked element is a special element.

   Special elements are
   flagged with a set high-bit in their tag.  Thus, a tag t has
   BASETAG == t & 0x7f and is a special tag if t & 0x80 != 0x00

   The first 16 bits of the meta-element that this tag/ref points to
   tells us what type of special element this is.  If these 16 bits is
   SPECIAL_CHUNKED, then it contains information about the chunked element.
   After this 16 bits, 32 bit which is the length of each chunk, after
   which is the information header:

   File Description of Chunked Element
   ****************************************

   NOTE: I know some of the fields could be 1 byte but instead of 4 bytes
         but I decided to make them 4 to allow the fields to change
         their behaviour in the future.....i.e some could hold tag/refs..

   DD for Chunked Element pointing to Chunked Description Record(12 byes )
   =======================================================================
   <-  2 bytes -> <- 2 bytes -> <- 4 bytes -> <- 4bytes ->
   --------------------------------------------------------
   |extended tag | reference # |  Offset     |  Length    |
   --------------------------------------------------------
                                    \______________/
   __________________________________________|
   V
   CHUNKED DESCRIPTION RECORD(6 + 9 + 8 + 5 + 12 x ndims + 4 + fill_val_len + 
                              'specialness' bytes) > 45 bytes
   ================================================================================
   <-  2 bytes -> <-  4 bytes  -> (6 bytes)
   --------------------------------
   |sp_tag_desc  |sp_tag_head_len | ... cont'd
   --------------------------------

   <-1 byte-> <- 4 bytes -> <-  4bytes  -> ( 9 bytes)
   ------------------------------------------
   | version |    flag     | elm_tot_length |... cont'd
   ------------------------------------------
        
   <- 4 bytes  -> <- 2 bytes -> <- 2 bytes -> (8 bytes)
   -------------------------------------------
...| chunk_size  | chk_tbl_tag | chk_tbl_ref | ...cont'd
   -------------------------------------------

   <- 2 bytes -> <- 2 bytes -> <- 1 byte -> (5 bytes)
   ----------------------------------------
...| sp_tag     | sp_ref      |  ndims   |  ...cont'd
   ----------------------------------------

   <- 4 bytes   -> <- 4 bytes -> <- 4 bytes -> (12 x ndims bytes)
   --------------------------------------------
...| distrib_type | dim_length  | chunk_length |  ... x Number of dimensions
   --------------------------------------------

   <-      4 bytes    -> <- variable bytes -> 
   ------------------------------------------
...| fill_val_num_bytes | fill value......  |   ...(optional cont'd)
   ------------------------------------------

          Optinal number of these depending on multiple 'specialness'
          set in 'flag' field.

   <- 2 byte   -> <-      4 bytes  -> <- variable bytes -> (6 + variable bytes)
   --------------------------------------------------------
...| sp_tag_desc | sp_tag_header len | sp_tag_header......|...
   --------------------------------------------------------

   Fields
   ------
   sp_tag_desc   - SPECIAL_CHUNKED(16 bit constant), identifies this as
                   a chunked element description record
   sp_tag_head_len - length of this special element header only.
                     Does not include length of header with additional 
                     'specialness' headers.
                     This is include to make this header layout similiar to the 
                     multiple 'specialiness' layout.
   version        - version info (8 bit field)
   flag           - bit field to set additional specialness  (32 bit field)
   elem_tot_len   - Valid Length of the entire element(32 bit field)
                    The actual physical length used for storage can be
                    greater than the dataset size due to ghost areas in
                    chunks. Partial chunks are not distinguished from
                    regular chunks.
   chunk_size     - Size of data chunks(32 bit field)
   chk_tbl_tag    - Tag of chunk table i.e. Vdata (16 bit field)
   chk_tbl_ref    - Reference number of the chunk table
                    i.e. Vdata (16 bit field)
   sp_tag         - For future use i.e. special table for 'ghost' chunks
   sp_ref         - For fture use
   ndims          - number of dimensions for the chunked element.(8 bit field)
   file_val_num_bytes - number of bytes in fill value (32 bit field)
   fill value         - fill value (variable bytes)

   Fields for each dimension: (12 x ndims bytes)
   --------------------------------------
   distrib_type   - type of data distribution along this dimension (32 bit field)
                    1 - Block
                    Currently only block distribution is supported.
   dim_length     - length of this dimension. (32 bit field)
   chunk_length   - length of the chunk along this dimension  (32 bit field)

   Fields for each additional 'specialness' (Optional)
   -------------------------------------------
   sp_tag_desc    - SPECIAL_xxx(16 bit constant), identifies this as
                    a 'xxx' element description record .(16 bit field)
   sp_tag_header_len - length of special element header(32 bit field)
   sp_tag_header     - special header.(variable bytes)


   Chunk Table(variable bytes per record in Vdata due to size of origin)
   ====================================================================
   <-  variable bytes -> <- 2 bytes -> <- 2 bytes -> 
   -------------------------------------------------
   |      origin        |  chunk_tag  | chunk_ref_1 |   
   -------------------------------------------------
           -                 -             -             N is number of
           -                 -             -             chunk records
           -                 -             -             in Vdata
   -------------------------------------------------
   |      origin        |  chunk_tag  | chunk_ref_N |
   -------------------------------------------------
                                 \______________/
   __________________________________________|
   V
    <-   2 bytes  -> <- 2 bytes -> <- 4 bytes -> <- 4bytes ->
    --------------------------------------------------------
    | DFTAG_CHUNK   | chunk_ref_N |  Offset     |  Length    |
    --------------------------------------------------------
                                         \______________/
        __________________________________________|
        V
        -----------------------
        | Data_chunk          |
        -----------------------
                  Note: The "Length" here is specified by "chk_size". 

   Fields
   ------
   origin    - specifies the coordinates of the chunk in the overall
               chunk array. (variable field, depends on number of 
               dimensions of chunked element).
   chunk_tag - DFTAG_CHUNK for now(could be another chunked element to
               allow recursive chunked elements(DFTAG_CHUNKED)) (16 bit field)
   chunk_ref - Reference number of chunk itself   (16 bit field)

EXPORTED ROUTINES
=================
   User Public  
   -----------
   HMCcreate       -- create a chunked element
   HMCwriteChunk   -- write out the specified chunk to a chunked element
   HMCsetMaxcache  -- maximum number of chunks to cache 

   Library Private
   ---------------
   HMCPstread      -- open an access record for reading
   HMCPstwrite     -- open an access record for writing
   HMCPseek        -- set the seek posn
   HMCPchunkread   -- read a single chunk out of a chunked element
   HMCPread        -- read some data out of a chunked element
   HMCPchunkwrite  -- write out a single chunk to a chunked element
   HMCPwrite       -- write out some data to a chunked element
   HMCPinquire     -- Hinquire for chunked element
   HMCPcloseAID    -- close file but keep AID active
   HMCPendacess    -- close a chunked element AID
   HMCPinfo        -- return info about a chunked element

   TBBT helper rotuines
   -------------------
   chkcompare      -- comprares 2 chunk records
   chkfreekey      -- frees chunk key
   chkdestroynode  -- destroys chunk record

LOCAL ROUTINES
==============
   Chunking helper routines
   ------------------------
   create_dim_recs
   calculate_chunk_indices
   update_chunk_indices
   calculate_chunk_seek      -- not used
   compute_array_to_chunk    -- not used
   calculate_num_to_chunk    -- not used
   calculate_seek_in_chunk
   calculate_chunk_num
   calculate_chunk_for_chunk

   Common Routine
   -------------
   HMCIstaccess -- set up AID to access a chunked element

*/

/* for debugging */

/*
#define CHK_DEBUG
*/

#define  _HCHUNKS_MAIN_
#include "hdf.h"
#include "hfile.h"
#include "hcache.h" /* cache */
#include "hchunks.h"

/* private functions */
PRIVATE int32 
HMCIstaccess(accrec_t * access_rec,  /* IN: access record to fill in */
             int16 acc_mode          /* IN: access mode */ );

/* -------------------------------------------------------------------------
NAME
   create_dim_recs
DESCRIPTION
  Given number of dimensions create the following 3 arrays.
  1. Dimension record array contains a record for each dimension.
  2. Seek chunk indice array contains the seek postion relative to
     the logical representation of the chunked array.
  3. The seek position chunk array contains the seek postion 
     relative to the chunk itself.
RETURNS
  SUCCEED/FAIL
---------------------------------------------------------------------------*/
PRIVATE int32
create_dim_recs(DIM_REC **dptr, /* OUT: dimension record pointers */
                int32  **sbi,   /* OUT: seek chunk indices array */
                int32  **spb,   /* OUT: seek pos w/ chunk array */
                int32 ndims     /* IN: number of dimension of element */)
{
    CONSTR(FUNC, "create_dim_recs");   /* for HERROR */
    int32 i;
    int32 ret_value = SUCCEED;

    /* allocate space for demension records pointers */
    if ((*dptr = (DIM_REC *)HDmalloc(sizeof(DIM_REC) * ndims)) == NULL)
        HGOTO_ERROR(DFE_NOSPACE, FAIL);

    /* allocate space for seek chunk indices and chunk seek positions */
    if ((*sbi = (int32 *)HDmalloc(sizeof(int32)*ndims)) == NULL)
              HGOTO_ERROR(DFE_NOSPACE, FAIL);

    if ((*spb = (int32 *)HDmalloc(sizeof(int32)*ndims)) == NULL)
        HGOTO_ERROR(DFE_NOSPACE, FAIL);

    /* for each dimension */
    for (i = 0; i < ndims; i++)
      {
          /* Initialize values for dimension record */
          (*dptr)[i].dim_length = 0;
          (*dptr)[i].chunk_length = 0;
          (*dptr)[i].num_chunks = 0;

          (*sbi)[i] = 0;
          (*spb)[i] = 0;
      } /* end for i */

  done:
    if(ret_value == FAIL)   
      { /* Error condition cleanup */
          if (*dptr != NULL)
              HDfree(*dptr);
          if (*sbi != NULL)
              HDfree(*sbi);
          if (*spb != NULL)
              HDfree(*spb);
      } /* end if */

    /* Normal function cleanup */
    return ret_value;
} /* end create_dim_recs() */

/* -------------------------------------------------------------------------
NAME
    calculate_chunk_indices
DESCRIPTION
  Give seek location within an element, calculate which chunk in
  chunk array and position within chunk.
RETURNS
    Nothing
---------------------------------------------------------------------------*/
PRIVATE void
calculate_chunk_indices(int32 sloc,    /* IN: Seek location in element */
                        int32 ndims,   /* IN: number of dimensions of elem */
                        int32 *sbi,    /* IN: seek chunk indicies array */
                        int32 *spb,    /* IN: seek pos w/ chunk array */
                        DIM_REC *ddims /* IN: dim record ptrs */)
{
    int32 i;
    int32 stmp;

    stmp = sloc;
    for(i = 0; i < ndims; i++) 
      { /* Calculate which chunk index in chunk representation */
          sbi[i] = (int32)((stmp % ddims[i].dim_length) 
                           / ddims[i].chunk_length);
          /* calculate starting postion in the chunk itself */
          spb[i] = (int32)((stmp % ddims[i].dim_length) 
                           % ddims[i].chunk_length);

#ifdef CHK_DEBUG
          printf("Dim %d: chunk_array =%d, chunk_pos_array = %d\n", 
                 i, sbi[i], spb[i]);
#endif
          stmp = stmp / ddims[i].dim_length;
      } /* end for i */

} /* calculdate_chunk_indices()*/

/* -------------------------------------------------------------------------
NAME
    update_chunk_indices
DESCRIPTION
    Given chunk size and current chunk in chunk array and postion within
    that chunk, update to new chunk in chunk array and postion within 
    the new chunk.
RETURNS
    Nothing
---------------------------------------------------------------------------*/
PRIVATE void
update_chunk_indices(int32 chunk_size, /* IN: size of chunk read/written */
                     int32 *sbi,       /* IN: seek chunk indicies array */
                     int32 *spb,       /* IN: seek pos w/ chunk array */
                     int32 ndims,      /* IN: number of dimensions */
                     DIM_REC *ddims    /* IN: dim record ptrs */)
{
    int32 change = 1;
    int32 index = 0;

    while(change && index < ndims) 
      { /* add chunk written to current chunk dimension */
          spb[index] += chunk_size;
          change = 0;
          if(spb[index] == ddims[index].chunk_length) 
            { /* we've move to next chunk since we filled the previous one */
#ifdef CHK_DEBUG
                fprintf(stderr,"going to next chunk along spb[%d] to access\n",index);
#endif
                spb[index] = 0; /* position at beginning of chunk */
                if(++(sbi[index]) == ddims[index].num_chunks) 
                  { /* we've written to all the chunks in this demension, 
                       so reset for this dimension */
#ifdef CHK_DEBUG
                fprintf(stderr,"accessed all chunks along sbi[%d] so reset \n", 
                        index);
#endif
                      sbi[index] = 0;
                      change = 1;
                  } 
                index++; /* go to next dimension */
                chunk_size = 1; 
            }
      } /* end while "change" */
}

#if 0 /* NOT USED */

/* -------------------------------------------------------------------------
NAME
    calculate_chunk_seek
DESCRIPTION
    Calculate new seek postion given seek chunk array and seek postion
    within that chunk array. 
RETURNS
    Nothing
---------------------------------------------------------------------------*/
PRIVATE void
calculate_chunk_seek(int32 *chunk_seek, /* OUT: new seek position within element */
                     int32 ndims,       /* IN: number of dims */
                     int32 *sbi,        /* IN: seek chunk array */
                     int32 *spb,        /* IN; seek pos w/ chunk array */
                     DIM_REC *ddims,    /* IN: dim record ptrs */
                     int32 chunk_size   /* IN: size of chunk */)
{
    int32 j;
    int32 new_seek;

    /* Calculate Seek Location in element 
     * First calculste seek-chunk position in file 
     * i.e seek position according to chunk first */
    *chunk_seek = sbi[ndims -1];
    for(j = ndims - 1; j; j--) 
      {
          *chunk_seek = (*chunk_seek * ddims[j-1].num_chunks)
              + sbi[j-1]; 
      }

    /* must get chunk_size from somewhere else
     * to give us position in file relative to chunk.
     * Next comes adjustment of seek for postion inside chunk*/
    *chunk_seek *= chunk_size; 
#ifdef CHK_DEBUG
    printf("  chunk_seek = %d(chunk# %d)\n", *chunk_seek,
           *chunk_seek/chunk_size);       
#endif
    /* Calculate seek position in chunk */
    new_seek = spb[ndims - 1];
    for(j = ndims - 1; j; j--) 
      {
          new_seek = (new_seek * ddims[j - 1].chunk_length) 
              + spb[j - 1];
      }

    /* add seek position in chunk to seek-chunk offset */
    new_seek += *chunk_seek;
#ifdef CHK_DEBUG
    printf("   calculated seek position in file is %d\n", new_seek);
#endif

    *chunk_seek = new_seek;

} /* calculate_chunk_seek() */

/* -------------------------------------------------------------------------
NAME
    compute_array_to_chunk
DESCRIPTION
    Calculate chunk array indicies and position within chunk 
    given user array indicies
     i.e. translates array postion in user array to position in overall
     chunk array and within the chunk.
RETURNS
    Nothing
---------------------------------------------------------------------------*/
PRIVATE void
compute_array_to_chunk(int32 *chunk_indicies, /* OUT: chunk indicies */
                       int32 *chunk_array_ind,/* OUT: chunk array indices */
                       int32 *array_indicies, /* IN: array indicies */
                       int32 ndims,           /* IN: number of dims */
                       DIM_REC *ddims         /* IN: dim record ptrs */ )
{
    int32 j;

    for(j = 0; j < ndims; j++) 
      {   /* set postion in overall chunk array */
          chunk_indicies[j] = array_indicies[j] / ddims[j].chunk_length;
          /* set postion within the chunk itself */
          chunk_array_ind[j] = array_indicies[j] % ddims[j].chunk_length;
      }
} /* compute_array_to_chunk() */

/* -------------------------------------------------------------------------
NAME
    calculate_num_to_chunk
DESCRIPTION
    Calculate seek chunk array given chunk number

    Not implemented
RETURNS
    Nothing
---------------------------------------------------------------------------*/
PRIVATE void
calculate_num_to_chunk(int32 chunk_num, /* IN: chunk number within element */
                       int32 ndims,     /* IN: number of dims */
                       int32 *sbi,      /* IN/OUT: seek chunk array */
                       DIM_REC *ddims   /* IN: dim record ptrs */ )
{
    int32 j;
    int nchunk_num = chunk_num;
   

} /* calculate_num_chunk() */

#endif /* NOT USED */

/* -------------------------------------------------------------------------
NAME
    calculate_seek_in_chunk
DESCRIPTION
    Calculate seek postion within chunk
RETURNS
    Nothing
---------------------------------------------------------------------------*/
PRIVATE void
calculate_seek_in_chunk(int32 *chunk_seek, /* OUT: new seek position within element */
                        int32 ndims,       /* IN: number of dims */
                        int32 *spb,        /* IN; seek pos w/ chunk array */
                        DIM_REC *ddims     /* IN: dim record ptrs */ )
{
    int32 j;

    /* Calculate seek position within chunk */
    *chunk_seek = spb[ndims - 1];
    for(j = ndims - 1; j; j--) 
      {
          *chunk_seek = (*chunk_seek * ddims[j - 1].chunk_length) 
              + spb[j - 1];
      }

} /* calculate_seek_in_chunk() */


/* -------------------------------------------------------------------------
NAME
    calculate_chunk_num
DESCRIPTION
    Calculate new chunk number given seek chunk array and seek postion
    within that chunk array. 
RETURNS
    Nothing
---------------------------------------------------------------------------*/
PRIVATE void
calculate_chunk_num(int32 *chunk_num, /* OUT: new chunk number within element */
                    int32 ndims,      /* IN: number of dims */
                    int32 *sbi,       /* IN: seek chunk array */
                    DIM_REC *ddims    /* IN: dim record ptrs */ )
{
    int32 j;

    /* Calculate Seek Location in element 
     * First calculste seek-chunk position in file 
     * i.e seek position according to chunk first */
    *chunk_num = sbi[ndims -1];
    for(j = ndims - 1; j; j--) 
      {
          *chunk_num = (*chunk_num * ddims[j-1].num_chunks)
              + sbi[j-1]; 
      }
} /* calculate_chunk_num() */

/* -------------------------------------------------------------------------
NAME
    calculate_chunk_for_chunk
DESCRIPTION
  Given the length of bytes to operate on and the size of bytes
  already operated on, calculate how big of chunk can be written
  to the current chunk.
RETURNS
    Nothing
---------------------------------------------------------------------------*/
PRIVATE void
calculate_chunk_for_chunk(int32 *chunk_size,    /* OUT: chunk size for this chunk */
                          int32 len,            /* IN: total length to operate on */
                          int32 bytes_finished, /* IN: bytes already operted on*/
                          int32 *spb,           /* IN: seek pos w/ chunk array */
                          DIM_REC *ddims        /* IN: dim record ptrs */)
{
    /* Calculate size of chunk to write in this chunk */
    if ((ddims[0].chunk_length - spb[0]) > (len - bytes_finished))
        *chunk_size = len - bytes_finished;   /* less than a chunk to write */
    else
        *chunk_size = ddims[0].chunk_length - spb[0]; /* full chunk */

} /* calculate_chunk_for_chunk() */


/* -------------------------------------------------------------------------
NAME
    chkcompare
DESCRIPTION
   Compares two chunk B-tree keys for equality.  Similar to memcmp.

   *** Only called by B-tree routines, should _not_ be called externally ***
RETURNS
   
---------------------------------------------------------------------------*/
intn
chkcompare(VOIDP k1,   /* IN: first key */
           VOIDP k2,   /* IN: second key */
           intn cmparg /* IN: not sure? */)
{
    intn  ret_value;
    /* shut compiler up */
    cmparg = cmparg;

    /* valid for integer keys */
    ret_value = ((intn) ((*(int32 *) k1) - (*(int32 *) k2)));    

    return ret_value;
}   /* chkcompare */

/*********** Helper fcns for dealing with chunk table TBBT tree ***************/

/* free key - used by tbbt routines */
VOID
chkfreekey(VOIDP key /*IN: chunk key */ )
{
    if (key != NULL)
        HDfree(key);
} /* chkfreekey() */

/* -------------------------------------------------------------------------
NAME
    chkdestroynode
DESCRIPTION
   Frees chunk B-Tree nodes
   *** Only called by B-tree routines, should _not_ be called externally ***

   Note that currently this routine also does ann Hendaccess on open
   chunks. This is probably not a good place to do this as any errors
   would be ignored due to the fact that this routine returns nothing.
RETURNS
   Nothing
---------------------------------------------------------------------------*/
VOID
chkdestroynode(VOIDP n /* IN: chunk record */ )
{
    CHUNK_REC *t=(CHUNK_REC *)n;

    if (t != NULL)
      {   
          /* free orgin first */
          if (t->origin != NULL)
              HDfree(t->origin);

          /* free chunk record structure */
          HDfree((VOIDP) t);
      }
}   /* chkdestroynode */

/* ----------------------------- HMCIstaccess ------------------------------
NAME
   HMCIstaccess -- set up AID to access a chunked elem

DESCRIPTION
   Calls to HMCIstread and HMCIstwrite resolve to this function.
   Given an active AID fill in all of the special information.
   If this information has already been read in for a different
   element use that else we must go out to the HDF file and
   pull in the information ourselves

RETURNS
   The AID of the access record on success FAIL on error.
----------------------------------------------------------------------------*/
PRIVATE int32
HMCIstaccess(accrec_t * access_rec, /* IN: access record to fill in */
             int16 acc_mode         /* IN: access mode */)
{
    CONSTR(FUNC, "HMCIstaccess");    /* for HERROR */
    filerec_t  *file_rec = NULL;     /* file record */
    chunkinfo_t *info    = NULL;     /* information about data elt */
    int32       dd_aid;              /* AID for writing the special info */
    uint16      data_tag, data_ref;  /* Tag/ref of the data in the file */
    uint8       local_ptbuf[4];      /* 4 bytes for special header length */
    uint8       *sp_header = NULL;   /* special element header */
    int32       nfields;             /* fields in Vdata */
    int32       interlace;           /* type of interlace */
    int32       vdata_size;          /* size of Vdata */
    int32       num_recs;            /* number of Vdatas */
    uint8       *v_data = NULL;      /* Vdata record */
    CHUNK_REC   *chkptr = NULL;      /* Chunk record */
    int32       *chk_key   = NULL;   /* chunk key */
    int32       npages     = 1;      /* number of chunks */
    int32       access_aid = FAIL;   /* access id */
    int32       ret_value = SUCCEED;
    char        name[VSNAMELENMAX + 1];  /* Vdata name */
    char        class[VSNAMELENMAX + 1]; /* Vdata class */
    intn        j,k;                     /* loop indicies */

    /* Check args */
    if (access_rec == NULL)
        HGOTO_ERROR(DFE_ARGS, FAIL);
        
    /* validate file record id */
    file_rec = HAatom_object(access_rec->file_id);
    if (BADFREC(file_rec) || !(file_rec->access & acc_mode))
        HGOTO_ERROR(DFE_ARGS, FAIL);

    /* set up some data in access record */
    access_rec->special = SPECIAL_CHUNKED;
    access_rec->posn    = 0;
    access_rec->access  = acc_mode|DFACC_READ;

    /*
     * Lets free old special info first,if one exists,
     * before copying a new one
     *
     * Hmm.....this is what other special elements do currently
     * don't know if this is really necessary.....
     */
    if (access_rec->special_info != NULL)
      {   /* special information record */
          chunkinfo_t *tmpinfo = (chunkinfo_t *) access_rec->special_info;

          if (--(tmpinfo->attached) == 0)
            {   /* the last one so now.. */
                /* free old info from Chunk tables ..etc*/

                /* Sync chunk cache */
                mcache_sync(info->chk_cache);

                /* close/free chunk cache */
                mcache_close(info->chk_cache);

                /* Use Vxxx interface to free Vdata info */
                VSdetach(info->aid);

                /* free chunk tree */
                tbbtdfree(info->chk_tree, chkdestroynode, chkfreekey);

                /* free up stuff in special info */
                if (tmpinfo->ddims != NULL)
                    HDfree(tmpinfo->ddims);
                if (tmpinfo->seek_chunk_indices != NULL)
                    HDfree(tmpinfo->seek_chunk_indices);
                if (tmpinfo->seek_pos_chunk != NULL)
                    HDfree(tmpinfo->seek_pos_chunk);

                if (tmpinfo->fill_val != NULL)
                    HDfree(tmpinfo->fill_val);
                /* free info struct last */
                HDfree((VOIDP) tmpinfo);

                access_rec->special_info = NULL;
            }
      } /* end if special->info already */

    /* get the info for the dataset i.e. tag/ref*/
    /* get info about chunk table i.e. Vdata? */
    if(HTPinquire(access_rec->ddid,&data_tag,&data_ref,NULL,NULL)==FAIL)
        HGOTO_ERROR(DFE_INTERNAL, FAIL);

    /* if the special information are already in some other acc elt,
     * point to it  and return */
    access_rec->special_info = HIgetspinfo(access_rec);
    if (access_rec->special_info)
      { /* special info exists */
          ((chunkinfo_t *) access_rec->special_info)->attached++;
          file_rec->attach++;
          info = (chunkinfo_t *) access_rec->special_info;          
          /* set return value */
          access_aid = HAregister_atom(AIDGROUP,access_rec);
      }
    else /* need to allocate a new special info and get it */
      {
          /* allocate space for specail chunk info */
          if ((info = (VOIDP) HDmalloc((uint32) sizeof(chunkinfo_t))) == NULL)
              HGOTO_ERROR(DFE_NOSPACE, FAIL);

          info->seek_chunk_indices = NULL;
          info->seek_pos_chunk     = NULL;
          info->ddims     = NULL;
          info->chk_tree  = NULL;
          info->chk_cache = NULL;
          info->fill_val  = NULL;
          info->num_recs  = 0; /* zero records to start with */

          /* read the special info structure from the file */
          if((dd_aid=Hstartaccess(access_rec->file_id,data_tag,data_ref,DFACC_READ))==FAIL)
              HGOTO_ERROR(DFE_CANTACCESS, FAIL);

          if (Hseek(dd_aid, 2, DF_START) == FAIL)
              HGOTO_ERROR(DFE_SEEKERROR, FAIL);

          /* first read specail tag header length which is 4 bytes */
          if (Hread(dd_aid, 4, local_ptbuf) == FAIL)
              HGOTO_ERROR(DFE_READERROR, FAIL);

          /* Decode it */
          {
              uint8      *p = local_ptbuf;
              INT32DECODE(p, info->sp_tag_header_len);   /* 4 bytes */          
          }

          /* Sanity check, the 256 limit is arbitrary and can 
             be removed later....*/
          if (info->sp_tag_header_len < 0 && info->sp_tag_header_len > 256)
              HGOTO_ERROR(DFE_INTERNAL, FAIL);

          /* Allocate buffer space for rest of special header */
          if (( sp_header = (uint8 *) HDcalloc(info->sp_tag_header_len,1))==NULL)
              HGOTO_ERROR(DFE_NOSPACE, FAIL);

          /* first read special header in */
          if (Hread(dd_aid, info->sp_tag_header_len, sp_header) == FAIL)
              HGOTO_ERROR(DFE_READERROR, FAIL);

          /* decode first special element header  */
          {
              uint8      *p = sp_header;

              /* version info */
              HDmemcpy(&info->version,p,1);      /* 1 byte  */
              p = p + 1;

              /* Should check version here to see if we can handle 
                 this version of special format header before we go on */
              if (info->version != _HDF_CHK_HDR_VER)
                  HGOTO_ERROR(DFE_INTERNAL, FAIL);

              INT32DECODE(p, info->flag);         /* 4 bytes */
              INT32DECODE(p, info->length);       /* 4 bytes */
              INT32DECODE(p, info->chunk_size);   /* 4 bytes */
              UINT16DECODE(p, info->chktbl_tag);  /* 2 bytes */
              UINT16DECODE(p, info->chktbl_ref);  /* 2 bytes */
              UINT16DECODE(p, info->sp_tag);      /* 2 bytes */
              UINT16DECODE(p, info->sp_ref);      /* 2 bytes */
              HDmemcpy(&info->ndims,p, 1);        /* 1 bytes */
              p = p + 1;
                                                 /* = 22 bytes */

              /* create dimension, seek_block and seek_pos arrays given number of dims */
              if (create_dim_recs(&(info->ddims),&(info->seek_chunk_indices),
                                  &(info->seek_pos_chunk),info->ndims) == FAIL)
                  HGOTO_ERROR(DFE_INTERNAL, FAIL);

              /* decode dimension stuff */
              for (j = 0; j < info->ndims; j++)
                {
                    INT32DECODE(p,(info->ddims[j].distrib_type));  /* 4 bytes */
                    INT32DECODE(p,(info->ddims[j].dim_length));   /* 4 bytes */
                    INT32DECODE(p,(info->ddims[j].chunk_length)); /* 4 bytes */
                    info->ddims[j].num_chunks = info->ddims[j].dim_length /
                        info->ddims[j].chunk_length;
                    /* check to see if need to increase # of chunks along this dim */
                    if (info->ddims[j].dim_length % info->ddims[j].chunk_length)
                        info->ddims[j].num_chunks++; /* increase by one */
                    npages = npages * info->ddims[j].num_chunks;
                }                                           /* = 12 x ndims bytes */

              /* decode fill value length */
              INT32DECODE(p,(info->fill_val_len));   /* 4 bytes */

              /* allocate space for fill value */
              if ((info->fill_val = (VOID *) HDmalloc((uint32)info->fill_val_len ))==NULL)
                  HGOTO_ERROR(DFE_NOSPACE, FAIL);

              /* finally decode fill value */
              HDmemcpy(info->fill_val,p, info->fill_val_len); /* 1 byte */

          } /* end decode special header */

          /* end access to special info stuff */
          if(Hendaccess(dd_aid)==FAIL)
              HGOTO_ERROR(DFE_CANTENDACCESS, FAIL);

          /* set up the chunk tables of the information */
          /* intialize TBBT tree of CHUNK records*/
          info->chk_tree = tbbtdmake(chkcompare, sizeof(int32));

          /* Use Vdata interface to read in chunk table and
             store per chunk-info in memory using TBBT trees  */ 

          /* Start access on Vdata */
          if(Vstart(access_rec->file_id) == FAIL)
              HGOTO_ERROR(DFE_INTERNAL, FAIL);

          /* Attach to Vdata with write access if we are writing else read access*/
          if (access_rec->access & DFACC_WRITE)
            {
                if((info->aid = VSattach(access_rec->file_id, info->chktbl_ref, "w")) == FAIL)
                    HGOTO_ERROR(DFE_CANTATTACH, FAIL);
            }
          else /* attach with read access only */
            {
                if((info->aid = VSattach(access_rec->file_id, info->chktbl_ref, "r")) == FAIL)
                    HGOTO_ERROR(DFE_CANTATTACH, FAIL);
            }

          /* get relevant info on Vdata */
          if ((VSinquire(info->aid, &num_recs, &interlace, NULL, &vdata_size, name)) == FAIL)
              HGOTO_ERROR(DFE_INTERNAL, FAIL);

          /* Get class of Vdata */
          if ((VSgetclass(info->aid, class)) == FAIL)
              HGOTO_ERROR(DFE_INTERNAL, FAIL);

          /* Get number of fields in Vdata */
          if ((nfields = VFnfields(info->aid)) == FAIL)
              HGOTO_ERROR(DFE_INTERNAL, FAIL);

          /* Set the fields to read */
          if(VSsetfields(info->aid,_HDF_CHK_FIELD_NAMES)==FAIL)
              HGOTO_ERROR(DFE_BADFIELDS,FAIL);

          /* Allocate space for a single Vdata record */
          if ((v_data = (VOIDP) HDmalloc(vdata_size)) == NULL)
              HGOTO_ERROR(DFE_NOSPACE, FAIL);

          /* for each record read it in and put into TBBT tree 
             NOTE: Should change this to a single VSread() but then
             would have to store all the v_data rec's somewhere
             before inserting them into the TBBT tree...
             ....for somone to do later if performance of VSread()
             is bad.......*/
          for (j = 0; j < num_recs; j++)
            {
                uint8 *pntr = NULL;

                /* read single record */
                if(VSread(info->aid,v_data,1,FULL_INTERLACE)==FAIL)
                    HGOTO_ERROR(DFE_VSREAD,FAIL);
        
                pntr = v_data; /* set pointer to vdata record */

                /* Allocate space for a chunk record */
                if ((chkptr = (CHUNK_REC *) HDmalloc(sizeof(CHUNK_REC))) == NULL)
                    HGOTO_ERROR(DFE_NOSPACE, FAIL);

                /* Allocate space for a origin in chunk record */
                if ((chkptr->origin = (int32 *) HDmalloc(info->ndims*sizeof(int32))) == NULL)
                    HGOTO_ERROR(DFE_NOSPACE, FAIL);

                /* allocate space for key */
                if ((chk_key = (int32 *)HDmalloc(sizeof(int32))) == NULL)
                    HGOTO_ERROR(DFE_NOSPACE, FAIL);

                /* Copy origin first */
                for (k = 0; k < info->ndims; k++)
                  {
                      HDmemcpy(&chkptr->origin[k],pntr,sizeof(int32));
                      pntr += sizeof(int32);
#ifdef CHK_DEBUG
                      printf(" chktpr->origin[%d]=%d, ",k,chkptr->origin[k]);
#endif
                  }

                /* Copy tag next */
                HDmemcpy(&chkptr->chk_tag,pntr,sizeof(uint16));
                pntr += sizeof(uint16);
#ifdef CHK_DEBUG
                printf(" chktpr->chk_tag=%d, ",chkptr->chk_tag);
#endif
                /* Copy ref last */
                HDmemcpy(&chkptr->chk_ref,pntr,sizeof(uint16));
#ifdef CHK_DEBUG
                printf(" chktpr->chk_ref=%d, ",chkptr->chk_ref);
                printf("\n");
#endif
                /* now compute chunk number from origin */
                calculate_chunk_num(chk_key, info->ndims, chkptr->origin, 
                                    info->ddims);

                chkptr->chunk_number = *chk_key; 

                /* set chunk number to record number */
                chkptr->chk_vnum = info->num_recs++;

                /* add to TBBT tree based on chunk number as the key */
                tbbtdins(info->chk_tree, (VOIDP) chkptr , chk_key);   
            }

          /* set return value */
          access_aid = HAregister_atom(AIDGROUP,access_rec);

          /* create chunk cache with 'maxcache' set to the number of chunks
             in the last dimension */
          if ((info->chk_cache = mcache_open(&access_rec->file_id, access_aid, 
                       info->chunk_size, info->ddims[0].num_chunks, 
                       npages, 0)) == NULL)
              HE_REPORT_GOTO("failed to find initialize chunk cache", FAIL);

          /* set up chunk read/write routines */
          mcache_filter(info->chk_cache, HMCPchunkread, HMCPchunkwrite, 
                        access_rec);

          /* update chunk info data and file record info */
          info->attached = 1;
          file_rec->attach++;
          access_rec->special_info = (chunkinfo_t *) info;
      } /* end else need to get special info */

    /* access to data elments is done on a per chunk basis whihc
       can only be done in the read/write routines */    

    ret_value = access_aid;

#if 0
    /* set return value */
    ret_value = HAregister_atom(AIDGROUP,access_rec);
#endif
  done:
    if(ret_value == FAIL)   
      { /* Error condition cleanup */

          /* free info struct */
          if (info != NULL)
            {
                if (info->chk_cache != NULL)
                  {
                      /* Sync chunk cache */
                      mcache_sync(info->chk_cache);

                      /* close/free chunk cache */
                      mcache_close(info->chk_cache);
                  }

                if (info->aid != FAIL)
                    VSdetach(info->aid);

                /* free chunk tree */
                if (info->chk_tree != NULL)
                    tbbtdfree(info->chk_tree, chkdestroynode, chkfreekey);

                /* free up stuff in special info */
                if (info->ddims != NULL)
                    HDfree(info->ddims);
                if (info->seek_chunk_indices != NULL)
                    HDfree(info->seek_chunk_indices);
                if (info->seek_pos_chunk != NULL)
                    HDfree(info->seek_pos_chunk);
                if (info->fill_val != NULL)
                    HDfree(info->fill_val);

                HDfree(info);

                access_rec->special_info = NULL;
            }

      } /* end if */

    /* Normal function cleanup */
    /* free specail element header */
    if (sp_header != NULL)
        HDfree(sp_header);

    /* free allocated space for vdata record */
    if (v_data != NULL)
        HDfree(v_data);

    return ret_value;
}   /* HMCIstaccess */

/* ------------------------------------------------------------------------
NAME
   HMCcreate -- create a chunked element

DESCRIPTION
   This routine takes an HDF element and promotes it into a 
   chunked element.  Basically, the element becomes a chunked element
   allowing easy appending where the chunk records are storead in
   a Vdata.  If the element already exists, 
   this is an error currently otherwise a new element is created.

   All of the pieces of the chunked element are the same size from
   the stand point of the element. If compression is used then
   each chunk is compressed and the compression layer takes
   care it as the chunk layers sees each chunks as a seperate
   HDF object(DFTAG_CHUNK). The proper compression special header
   needs to be passed to the compression layer.

   The Vdata(chunk table) is made appendable with linked-block
   table size of 128.

RETURNS
   The AID of newly created chunked element, FAIL on error.

 --------------------------------------------------------------------------- */
int32
HMCcreate(int32 file_id,       /* IN: file to put chunked element in */
          uint16 tag,          /* IN: tag of element */
          uint16 ref,          /* IN: ref of element */
          uint8 nlevels,       /* IN: number of levels of chunks */
          int32 data_len,      /* IN: length of element */
          int32 fill_val_len,  /* IN: fill value length in bytes */
          VOID *fill_val,      /* IN: fill value */
          CHUNK_DEF *chk_array /* IN: structure describing chunk distribution
                                  can be an array? but we only handle 1 level */ )
{
    CONSTR(FUNC, "HMCcreate");     /* for HERROR */
    filerec_t  *file_rec   = NULL; /* file record */
    accrec_t   *access_rec = NULL; /* access record */
    int32       dd_aid     = FAIL; /* AID for writing the special info */
    chunkinfo_t *info      = NULL; /* information for the chunked elt */
    uint8       *sp_header = NULL; /* special element header */
    int32       npages     = 1;    /* i.e. number of chunks in element */
    int32       access_aid = FAIL; /* access id */
    uint16      chktbl_ref;        /* the ref of the link structure
                                      chunk table i.e. Vdata */
    uint16      special_tag;       /* special version of this tag */
    atom_t      data_id;           /* dd ID of existing regular element */
    int32       sp_tag_header_len = 0; /* length of special header */
    int32       ret_value  = SUCCEED;
    char        v_name[30] = "";   /* name of vdata i.e. chunk table */
    intn        i;                 /* loop index */

#ifdef HAVE_PABLO
    TRACE_ON(H_mask, ID_HMCcreate);
#endif /* HAVE_PABLO */

    /* clear error stack and validate file record id */
    HEclear();
    file_rec = HAatom_object(file_id);

    /* validate args */
    if (BADFREC(file_rec) || chk_array == NULL)
        HGOTO_ERROR(DFE_ARGS, FAIL);

    /* check file access for write */
    if (!(file_rec->access & DFACC_WRITE))
        HGOTO_ERROR(DFE_DENIED, FAIL);

    /* check if we are accidently passwed a special tag already */
    if(SPECIALTAG(tag)
       || (special_tag = MKSPECIALTAG(tag)) == DFTAG_NULL)
        HGOTO_ERROR(DFE_ARGS, FAIL);

    /* get empty slot in access records */
    access_rec = HIget_access_rec();
    if (access_rec == NULL)
        HGOTO_ERROR(DFE_TOOMANY, FAIL);

    /* search for identical dd */
    if ((data_id = HTPselect(file_rec,tag,ref))!=FAIL)
      {
          /*  this is where if a tag was already special i.e. compressed
              we would have to note it and promote it maybe? */
          /* Check if the element is already special */
          if (HTPis_special(data_id)==TRUE)
            {
                HTPendaccess(data_id);
                HGOTO_ERROR(DFE_CANTMOD, FAIL);
            }   /* end if */

      } /* end if */

    /* In theory we can have more than one level of chunks so 
       we need to repeat the following steps. This would
       allow subchunking but currently haven't decided how
       the user would pass this info to routine to create the
       proper chunk tables...etc.

       Do we need to create special chunk table to handle the
       special chunks i.e. ghost chunks.-> Pass on this for now  */

#if 0
    /* Okay we need to get a new ref for CHUNK table tag for first level */
    chktbl_ref = Htagnewref(file_id,DFTAG_CHUNKED);
#endif

    /* allocate and fill in special chunk info struct for CHUNKs */
    if (( info = (chunkinfo_t *) HDmalloc((uint32) sizeof(chunkinfo_t)))==NULL)
        HGOTO_ERROR(DFE_NOSPACE, FAIL);

    info->attached     = 1;
    info->aid          = FAIL;
    info->version      = _HDF_CHK_HDR_VER ;     /* verson 1 for now */
    info->flag         = chk_array->chunk_flag; 
    info->length       = data_len;              /* size of element */
    info->chunk_size   = chk_array->chunk_size; /* chunk size */
    info->ndims        = chk_array->num_dims;   /* number of dimensions */
    info->sp_tag       = DFTAG_NULL;            /* not used currently */
    info->sp_ref       = 0;                     /* not used currently */
    info->seek_chunk_indices = NULL;
    info->seek_pos_chunk = NULL;
    info->ddims          = NULL;
    info->chk_tree       = NULL;
    info->chk_cache      = NULL;
    info->num_recs       = 0;                   /* zero Vdata records to start */
    info->fill_val_len   = fill_val_len;        /* length of fill value */
    /* allocate space for fill value */
    if (( info->fill_val = (VOID *) HDmalloc(fill_val_len))==NULL)
        HGOTO_ERROR(DFE_NOSPACE, FAIL);
    /* copy fill value over */
    HDmemcpy(info->fill_val, fill_val, info->fill_val_len); /* fill_val_len bytes */

    /* Use Vxxx interface to create new Vdata to hold Chunk table */
    /* create/intialize chunk table (Vdata ) */

    /* Start access on Vdata */
    if(Vstart(file_id) == FAIL)
        HGOTO_ERROR(DFE_INTERNAL, FAIL);

    /* Create Vdata */
    if((info->aid = VSattach(file_id, -1, "w")) == FAIL)
        HGOTO_ERROR(DFE_CANTATTACH, FAIL);

    /* Make it appendable with linked block table size of 128 */
    if (VSappendable(info->aid, 128) == FAIL)
        HGOTO_ERROR(DFE_INTERNAL, FAIL);

    /* get ref of Vdata */
    chktbl_ref = VSQueryref(info->aid);

    info->chktbl_ref      = chktbl_ref; /* ref of chunk table */

    /* get tag of Vdata */
    info->chktbl_tag = VSQuerytag(info->aid);

    /* Define fields of chunk table i.e. Vdata */

    /* Define origin - order based on number of dims */
    if(VSfdefine(info->aid,_HDF_CHK_FIELD_1, DFNT_INT32,info->ndims) == FAIL)
        HGOTO_ERROR(DFE_INTERNAL, FAIL);

    /* Define tag of chunk 
       Note that the tag could be another Chunk table to
       represent another level. useful for quadtrees...etc. */
    if(VSfdefine(info->aid,_HDF_CHK_FIELD_2, DFNT_UINT16,1) == FAIL)
        HGOTO_ERROR(DFE_INTERNAL, FAIL);

    /* Define ref of chunk 
       Note that the ref could be that of another Chunk table to
       represent another level. useful for quadtrees...etc. */
    if(VSfdefine(info->aid,_HDF_CHK_FIELD_3, DFNT_UINT16,1) == FAIL)
        HGOTO_ERROR(DFE_INTERNAL, FAIL);

    /* Set Vdata name based on tag and ref of element.
       ...sort of a back pointer...*/
    sprintf(v_name,"%s%d_%d",_HDF_CHK_TBL_NAME,tag, ref);
    if(VSsetname(info->aid,v_name) == FAIL)
        HGOTO_ERROR(DFE_INTERNAL, FAIL);

    /* Set Vdata class */
    if(VSsetclass(info->aid,_HDF_CHK_TBL_CLASS) == FAIL)
        HGOTO_ERROR(DFE_INTERNAL, FAIL);

    /* Set the fields to write */
    if(VSsetfields(info->aid,_HDF_CHK_FIELD_NAMES)==FAIL)
        HGOTO_ERROR(DFE_BADFIELDS,FAIL);

    /* create dimension, seek_block and seek_pos arrays given number of dims */
    if (create_dim_recs(&(info->ddims),&(info->seek_chunk_indices),
                        &(info->seek_pos_chunk),info->ndims) == FAIL)
        HGOTO_ERROR(DFE_INTERNAL, FAIL);

    /* Copy info from input to dimension arrays */
    for (i = 0; i < info->ndims; i++)
      {
          info->ddims[i].distrib_type = chk_array->pdims[i].distrib_type;
          info->ddims[i].dim_length   = chk_array->pdims[i].dim_length;
          info->ddims[i].chunk_length = chk_array->pdims[i].chunk_length;
          info->ddims[i].num_chunks = info->ddims[i].dim_length /
              info->ddims[i].chunk_length;
          /* check to see if need to increase # of chunks along this dim */
          if (info->ddims[i].dim_length % info->ddims[i].chunk_length)
              info->ddims[i].num_chunks++; /* increase by one */
          npages = npages * info->ddims[i].num_chunks;
#ifdef CHK_DEBUG
          printf("HMCcreate: dim[%d].dim_length=%d,",i,info->ddims[i].dim_length);
          printf("HMCcreate: dim[%d].chunk_length=%d,",i,info->ddims[i].chunk_length);
          printf("HMCcreate: dim[%d].num_chunks=%d,",i,info->ddims[i].num_chunks);
#endif
      }                                               
#ifdef CHK_DEBUG
    printf("\n");
#endif
    /* Calculate total length of this special element header
       including the fields for 'sp_tag_desc' and 'sp_tag_head_len'.
       See description of format header at top of file for more
       info on fields */
    sp_tag_header_len = 6 + 9 + 8 + 5 +(12*info->ndims) + 4 + info->fill_val_len;

    /* Allocate buffer space for header */
    if (( sp_header = (uint8 *) HDcalloc(sp_tag_header_len,1))==NULL)
        HGOTO_ERROR(DFE_NOSPACE, FAIL);

    /* Calculate length of this special element header itself.
       Note the value of 'sp_tag_head_len' in the file is the 
       total length of this special object header - 6 bytes.
       beacuse the length of the fields 'sp_tag_desc'(2 bytes) and 
       'sp_tag_head_len' (4 bytes) which are not included */
    info->sp_tag_header_len = sp_tag_header_len - 6;

    /* encode info into chunked descripton record */
    {
        uint8      *p = sp_header;
        intn        j;

        UINT16ENCODE(p, SPECIAL_CHUNKED);        /* 2 bytes */
        INT32ENCODE(p, info->sp_tag_header_len); /* 4 bytes */
        HDmemcpy(p, &info->version,1);           /* 1 byte  */
        p = p + 1;
        INT32ENCODE(p, info->flag);         /* 4 bytes */
        INT32ENCODE(p, info->length);       /* 4 bytes */
        INT32ENCODE(p, info->chunk_size);   /* 4 bytes */
        UINT16ENCODE(p, info->chktbl_tag);  /* 2 bytes */
        UINT16ENCODE(p, info->chktbl_ref);  /* 2 bytes */
        UINT16ENCODE(p, info->sp_tag);      /* 2 bytes */
        UINT16ENCODE(p, info->sp_ref);      /* 2 bytes */
        HDmemcpy(p, &info->ndims,1);        /* 1 bytes */
        p = p + 1;
                                            /* = 28 bytes*/
        for (j = 0; j < info->ndims; j++)
          {
              INT32ENCODE(p,(info->ddims[j].distrib_type)); /* 4 bytes */
              INT32ENCODE(p,(info->ddims[j].dim_length));   /* 4 bytes */
              INT32ENCODE(p,(info->ddims[j].chunk_length)); /* 4 bytes */
          }                                               /* = 12 x ndims bytes */

        /* now for fill value */
        INT32ENCODE(p,(info->fill_val_len)); /* 4 bytes */
        HDmemcpy(p,info->fill_val,info->fill_val_len); /* fill_val_len bytes */
        p = p + fill_val_len;

        /* Future to encode multiply specialness stuff
           header lenghts, header,..etc*/
    }

    /* write the special info structure to fill */
    if((dd_aid = Hstartaccess(file_id,special_tag,ref,DFACC_ALL))==FAIL)
        HGOTO_ERROR(DFE_CANTACCESS, FAIL);

    /* write only the base 28 bytes ( 6 + 9 + 8 + 5)
       plus what is needed for each dimension which is (12 x ndims) bytes.
       plus for fill value 4 bytes + fill_val_len 
       plus in future multiply special headers  = sp_tag_header_len */
    if (Hwrite(dd_aid, sp_tag_header_len, sp_header) == FAIL)
        HGOTO_ERROR(DFE_WRITEERROR, FAIL);

    /* end access to special info stuff in file */
    if(Hendaccess(dd_aid)==FAIL)
        HGOTO_ERROR(DFE_CANTENDACCESS, FAIL);

    /* intialize TBBT tree of CHUNK records*/
    info->chk_tree = tbbtdmake(chkcompare, sizeof(int32));

    /* Detach from the data DD ID */
    if(data_id != FAIL)
      {
        if(HTPendaccess(data_id)==FAIL)
            HGOTO_ERROR(DFE_INTERNAL, FAIL);
      }

    /* update access record and file record */
    if((access_rec->ddid = HTPselect(file_rec,special_tag,ref))==FAIL)
        HGOTO_ERROR(DFE_INTERNAL, FAIL);

    access_rec->special      = SPECIAL_CHUNKED;
    access_rec->special_func = &chunked_funcs;
    access_rec->special_info = (VOIDP)info;
    access_rec->posn         = 0;
    access_rec->access       = DFACC_RDWR;
    access_rec->file_id      = file_id;
    access_rec->appendable   = FALSE;     /* start data as non-appendable */

    file_rec->attach++;

    /* register this valid access record for the chunked element */
    access_aid = HAregister_atom(AIDGROUP,access_rec);

    /* create chunk cache */
    if ((info->chk_cache = mcache_open(&access_rec->file_id, access_aid, 
                      info->chunk_size, info->ddims[0].num_chunks, 
                      npages,0)) == NULL)
        HE_REPORT_GOTO("failed to find initialize chunk cache", FAIL);

    /* set up chunk read/write routines */
    mcache_filter(info->chk_cache, HMCPchunkread, HMCPchunkwrite, 
                  access_rec);

    ret_value = access_aid;

  done:
    if(ret_value == FAIL)   
      { /* Error condition cleanup */
          /* free info struct */
          if (info != NULL)
            {
                if (info->chk_cache != NULL)
                  {   /* Sync chunk cache */
                      mcache_sync(info->chk_cache);

                      /* close chunk cache */
                      mcache_close(info->chk_cache);
                  }

                if (info->aid != FAIL)
                    VSdetach(info->aid); /* detach from chunk table */

                /* free chunk tree */
                if (info->chk_tree != NULL)
                    tbbtdfree(info->chk_tree, chkdestroynode, chkfreekey);

                /* free up stuff in special info */
                if (info->ddims != NULL)
                    HDfree(info->ddims);
                if (info->seek_chunk_indices != NULL)
                    HDfree(info->seek_chunk_indices);
                if (info->seek_pos_chunk != NULL)
                    HDfree(info->seek_pos_chunk);
                if (info->fill_val != NULL)
                    HDfree(info->fill_val);

                HDfree(info); /* free spcial info last */
            }

          /* free access record */
          if(access_rec != NULL)
              HDfree(access_rec);
      } /* end if */

    /* Normal function cleanup */
    /* free special element header */
    if (sp_header != NULL)
        HDfree(sp_header);

#ifdef HAVE_PABLO
    TRACE_OFF(H_mask, ID_HMCcreate);
#endif /* HAVE_PABLO */

    return ret_value;
} /* HMCcreate() */

/* ------------------------------ HMCPstread -------------------------------
NAME
   HMCPstread -- open an access record of chunked element for reading

DESCRIPTION
   Calls to HMCIstaccess to fill in the access rec for
   reading

RETURNS
   The AID of the access record on success FAIL on error.
---------------------------------------------------------------------------*/
int32
HMCPstread(accrec_t * access_rec /* IN: access record to fill in */)
{
    int32 ret_value;

#ifdef HAVE_PABLO
    TRACE_ON(H_mask, ID_HMCPstread);
#endif /* HAVE_PABLO */

    ret_value = HMCIstaccess(access_rec, DFACC_READ);

#ifdef HAVE_PABLO
    TRACE_OFF(H_mask, ID_HMCPstread);
#endif /* HAVE_PABLO */

    return ret_value;
}   /* HMCPstread */

/* ------------------------------ HMCPstwrite ------------------------------
NAME
   HMCPstwrite -- open an access record of a chunked elmenent for writing

DESCRIPTION
   Calls to HMCIstaccess to fill in the access rec for
   writing

RETURNS
   The AID of the access record on success FAIL on error.
---------------------------------------------------------------------------*/
int32
HMCPstwrite(accrec_t * access_rec /* IN: access record to fill in */)
{
    int32  ret_value;

#ifdef HAVE_PABLO
    TRACE_ON(H_mask, ID_HMCPstwrite);
#endif /* HAVE_PABLO */

    ret_value = HMCIstaccess(access_rec, DFACC_WRITE);

#ifdef HAVE_PABLO
    TRACE_OFF(H_mask, ID_HMCPstwrite);
#endif /* HAVE_PABLO */

    return ret_value;
}   /* HMCPstwrite */


/* ------------------------------- HMCPseek --------------------------------
NAME
   HMCPseek -- set the seek posn in the chunked elemnent

DESCRIPTION
   Set the seek posn in the given chunked element

RETURNS
   SUCCEED / FAIL
---------------------------------------------------------------------------*/
int32
HMCPseek(accrec_t * access_rec,  /* IN: access record to mess with */
         int32 offset,           /* IN: seek offset */
         int origin              /* IN: where we should calc the offset from */)
{
    CONSTR(FUNC, "HMCPseek");    /* for HERROR */
    chunkinfo_t *info = NULL;    /* information for the chunked elt */
    int32   ret_value = SUCCEED;

#ifdef HAVE_PABLO
    TRACE_ON(H_mask, ID_HMCPseek);
#endif /* HAVE_PABLO */

    /* Check args */
    if (access_rec == NULL)
        HGOTO_ERROR(DFE_ARGS, FAIL);

    /* validate access record */
    if (access_rec->special != SPECIAL_CHUNKED)
        HGOTO_ERROR(DFE_INTERNAL, FAIL);

    /* adjust the offset according to origin and validate */
    /* there is no upper bound to posn */
    if (origin == DF_CURRENT)
        offset += access_rec->posn;
    if (origin == DF_END)
        offset += ((chunkinfo_t *) (access_rec->special_info))->length;
    if (offset < 0)
        HGOTO_ERROR(DFE_RANGE, FAIL);

    info = (chunkinfo_t *) (access_rec->special_info);

    /* Seek to given location(bytes) for reading/writing */
    /* i.e calculate chunk indices given seek location 
       this will update the proper arrays in the special info struct */
    calculate_chunk_indices(offset,info->ndims,info->seek_chunk_indices,
                            info->seek_pos_chunk,info->ddims);

    /* set position in access record */
    access_rec->posn = offset;

  done:
    if(ret_value == FAIL)   
      { /* Error condition cleanup */

      } /* end if */

    /* Normal function cleanup */
#ifdef HAVE_PABLO
    TRACE_OFF(H_mask, ID_HMCPseek);
#endif /* HAVE_PABLO */

    return ret_value;
}   /* HMCPseek */

/* ------------------------------- HMCPchunkread --------------------------------
NAME
   HMCPchunkread - read a chunk

DESCRIPTION
   Read in some data from a chunked element.  If length
   is zero read until the end of the element.  It is assumed
   that the data buffer is big enough to store the data.
   If length would take us off the end of the element only
   read what has been written.

RETURNS
   The number of bytes read or FAIL on error
--------------------------------------------------------------------------- */
int32
HMCPchunkread(VOID  *cookie,    /* IN: access record to mess with */
              int32 chunk_num,  /* IN: chunk to read */
              VOID  *datap      /* OUT: buffer for data */)
{
    CONSTR(FUNC, "HMCPchunkread");    /* for HERROR */
    accrec_t * access_rec = (accrec_t *)cookie; /* access record */
    chunkinfo_t *info    = NULL; /* information record for this special data elt */
    CHUNK_REC   *chk_rec = NULL; /* chunk record */
    TBBT_NODE   *entry   = NULL; /* chunk node from TBBT */
    uint8       *bptr    = NULL; /* pointer to data buffer */
    int32       chk_id   = FAIL; /* chunk id */
    int32       relative_posn = 0; /* relative position in chunk of data elt */
    int32       bytes_read = 0;    /* total # bytes read for this call of HMCIread */
    int32       read_len = 0;      /* length of bytes to read */
    int32       nitems = 1;        /* used in HDmemfill(), */
    int32       ret_value = SUCCEED;

#ifdef HAVE_PABLO
    TRACE_ON(H_mask, ID_HMCPread);
#endif /* HAVE_PABLO */

    /* Check args */
    if (access_rec == NULL)
        HGOTO_ERROR(DFE_ARGS, FAIL);

    /* set inputs */
    bptr = (uint8 *) datap;
    info = (chunkinfo_t *) (access_rec->special_info);
    relative_posn = access_rec->posn;
    bytes_read    = 0;
    read_len      = info->chunk_size; 

#ifdef CHK_DEBUG
    printf("HMCPchunkread called with chunk %d \n",chunk_num);
#endif
    /* find chunk record in TBBT */
    if ((entry = (tbbtdfind(info->chk_tree, &chunk_num, NULL))) == NULL)
      { /* does not exist */
          /* calculate number of fill value items to fill buffer with */
          nitems = info->chunk_size / info->fill_val_len;

          /* copy fill values into buffer and return */
          if (HDmemfill(datap,info->fill_val, info->fill_val_len,nitems) == NULL)
              HE_REPORT_GOTO("HDmemfill failed to fill read chunk", FAIL);
      }
    else /* exists in TBBT */
      {
        /* get chunk record from node */
        chk_rec = (CHUNK_REC *) entry->data; 

        /* check to see if has been written to */
        if (chk_rec->chk_tag != DFTAG_NULL)
          { /* valid chunk in file */
              /* Start read on chunk */
              if ((chk_id = Hstartread(access_rec->file_id, chk_rec->chk_tag,
                                       chk_rec->chk_ref)) == FAIL)
                {
                    Hendaccess(chk_id);
                    HE_REPORT_GOTO("Hstartread failed to read chunk", FAIL);
                }

              /* read data from chunk */
              if (Hread(chk_id, read_len, bptr) == FAIL)
                  HGOTO_ERROR(DFE_READERROR, FAIL);

              bytes_read = read_len;

              /* end access to chunk */
              if (Hendaccess(chk_id) == FAIL)
                  HE_REPORT_GOTO("Hendaccess failed to end access to chunk", FAIL);
              
          }
        else /* chunk has not been written, so return fill value buffer */
          {  /* calculate number of fill value items to fill buffer with */
              nitems = info->chunk_size / info->fill_val_len;

               /* copy fill values into buffer and return */
              if (HDmemfill(datap,info->fill_val, info->fill_val_len,nitems) == NULL)
                  HE_REPORT_GOTO("HDmemfill failed to fill read chunk", FAIL);
          }
      } /* end else exists in TBBT tree */

    ret_value = bytes_read; /* number of bytes read */

  done:
    if(ret_value == FAIL)   
      { /* Error condition cleanup */
          if (chk_id != FAIL)
              Hendaccess(chk_id);
       } /* end if */

    /* Normal function cleanup */
#ifdef HAVE_PABLO
    TRACE_OFF(H_mask, ID_HMCPread);
#endif /* HAVE_PABLO */

    return ret_value;
} /* HMCPreadchunk() */

/* ------------------------------- HMCPread --------------------------------
NAME
   HMCPread - read data from a chunked element

DESCRIPTION
   Read in some data from a chunked element.  If length
   is zero read until the end of the element.  It is assumed
   that the data buffer is big enough to store the data.
   If length would take us off the end of the element only
   read what has been written.

RETURNS
   The number of bytes read or FAIL on error
--------------------------------------------------------------------------- */
int32
HMCPread(accrec_t * access_rec, /* IN: access record to mess with */
         int32 length,          /* IN: number of bytes to read */
         VOIDP datap            /* OUT: buffer for data */)
{
    CONSTR(FUNC, "HMCPread");    /* for HERROR */
    uint8       *data = NULL;    /* data buffer */
    chunkinfo_t *info = NULL;    /* information record for this special data elt */
    int32       relative_posn = 0; /* relative position in chunk of data elt */
    int32       bytes_read = 0;  /* total # bytes read for this call of HMCIread */
    uint8       *bptr = NULL;    /* data buffer pointer */
    int32       read_len = 0;    /* amount of data to copy */
    int32       read_seek = 0;   /* next read seek position */
    int32       chunk_size = 0;  /* size of data to read from chunk */
    int32       chunk_num = 0;   /* next chunk number */
    VOID        *chk_data = NULL; /* chunk data */
    uint8       *chk_dptr = NULL; /* pointer to chunk data */
#ifdef CHK_DEBUG
    int         i;
#endif
    int32       ret_value = SUCCEED;

#ifdef HAVE_PABLO
    TRACE_ON(H_mask, ID_HMCPread);
#endif /* HAVE_PABLO */

    /* Check args */
    if (access_rec == NULL)
        HGOTO_ERROR(DFE_ARGS, FAIL);

    /* set inputs */
    data = (uint8 *) datap;
    info = (chunkinfo_t *) (access_rec->special_info);
    relative_posn = access_rec->posn;

    /* validate length and set proper length */
    if (length == 0)
        length = info->length - access_rec->posn;
    else if (length < 0)
        HGOTO_ERROR(DFE_RANGE, FAIL);

    if (access_rec->posn + length > info->length)
        length = info->length - access_rec->posn;

    /* enter translating length to proper filling of buffer from chunks */
    bptr = datap;
    bytes_read = 0;
    read_len = length; 
    while (bytes_read < read_len) 
      {
          /* for debuging */
#ifdef CHK_DEBUG
          printf("Seek start(in chunk array):(");
          for (i = 0; i < info->ndims; i++)
              printf("%d%s", info->seek_chunk_indices[i], i!= info->ndims-1 ? ",":NULL);
          printf(")\n");
          printf("Seek start(within the chunk):(");
          for (i = 0; i < info->ndims; i++)
              printf("%d%s", info->seek_pos_chunk[i], i!= info->ndims-1 ? ",":NULL);
          printf(")\n");
#endif

          /* calculate chunk to retrieve */
          calculate_chunk_num(&chunk_num,info->ndims,info->seek_chunk_indices,
                              info->ddims);

          /* calculate contiguouschunk size that we can read from this chunk */
          calculate_chunk_for_chunk(&chunk_size,read_len,bytes_read,
                                    info->seek_pos_chunk,info->ddims);
#ifdef CHK_DEBUG
          printf("reading chunk of %d bytes\n", chunk_size);
#endif

          /* would be nice to get Chunk record from TBBT based on chunk number 
             and then get chunk data base on chunk vdata number but
             currently the chunk calculations return chunk 
             numbers and not Vdata record numbers. 
             This would reduce some overhead in the number of chunks
             dealt with in the cache */

          /* currently get chunk data from cache based on chunk number 
             Note the cache deals with objects starting from 1 not 0 */
          if ((chk_data = (VOID *) mcache_get(info->chk_cache, chunk_num+1, 0)) == NULL)
              HE_REPORT_GOTO("failed to find chunk record", FAIL);

          chk_dptr = chk_data; /* set chunk data ptr */

          /* calculate position in chunk */
          calculate_seek_in_chunk(&read_seek,info->ndims,info->seek_pos_chunk,
                                  info->ddims);

          chk_dptr += read_seek; /* move to correct position in chunk */

          /* copy data from chunk to users buffer */
          HDmemcpy(bptr, chk_dptr, chunk_size);        

          /* put chunk back to cache */
          if (mcache_put(info->chk_cache, chk_data, 0) == FAIL)
              HE_REPORT_GOTO("HMCPread failed to put chunk back in cache", FAIL);

#if 0     
          for (i=0; i < chunk_size; i++)
            {
                printf("%x(%d)",*(bptr+i),*(bptr+i));
                if (i != chunk_size - 1)
                    printf(",");
            }
          printf("\n");
#endif

          /* increment buffer pointer */
          bptr += chunk_size;

          /* adjust number of bytes already read */
          bytes_read += chunk_size; 

#ifdef CHK_DEBUG
          printf("read %d bytes already\n", bytes_read);
#endif
#if 0
          for (i=0; i < bytes_read; i++)
            {
                printf("%x(%d)",datap[i],datap[i]);
                if (i != bytes_read - 1)
                    printf(",");
            }
#endif
#ifdef CHK_DEBUG
          printf("\n");
#endif
          /* increment our starting seek postion i
           * i.e. find new chunk to start writing to */
          update_chunk_indices(chunk_size,info->seek_chunk_indices,
                               info->seek_pos_chunk,info->ndims,info->ddims);
      } /* end while "bytes_read" */

    /* update access record postion with bytes read */
    access_rec->posn += bytes_read;

    ret_value = bytes_read;

  done:
    if(ret_value == FAIL)   
      { /* Error condition cleanup */

      } /* end if */

    /* Normal function cleanup */
#ifdef HAVE_PABLO
    TRACE_OFF(H_mask, ID_HMCPread);
#endif /* HAVE_PABLO */

    return ret_value;
}   /* HMCPread  */

/* ------------------------------- HMCPchunkwrite -------------------------------
NAME
   HMCPchunkwrite -- write out chunk

DESCRIPTION
   Write out some data from a chunked element.  If we write
   passed the end of the existing element new chunks are created
   as needed.

RETURNS
   The number of bytes written or FAIL on error
---------------------------------------------------------------------------*/
int32
HMCPchunkwrite(VOID  *cookie,    /* IN: access record to mess with */
               int32 chunk_num,  /* IN: chunk number */
               const VOID *datap /* IN: buffer for data */)
{
    CONSTR(FUNC, "HMCPchunkwrite");   /* for HERROR */
    accrec_t * access_rec = (accrec_t *)cookie; /* access record */
    uint8      *data     = NULL;  /* data buffer */
    filerec_t  *file_rec = NULL;  /* file record */
    chunkinfo_t *info    = NULL;  /* chunked element information record */
    CHUNK_REC   *chk_rec = NULL;  /* current chunk */
    TBBT_NODE   *entry   = NULL;  /* node off of  chunk tree */
    uint8       *v_data  = NULL;  /* chunk table record i.e Vdata record */
    CHUNK_REC   *chkptr  = NULL;  /* Chunk record to inserted in TBBT  */
    VOID        *bptr    = NULL;  /* data buffer pointer */
    int32       chk_id   = FAIL ; /* chunkd accces id */
    int32       relative_posn;     /* relative position in chunked element */
    int32       bytes_written = 0; /* total #bytes written by HMCIwrite */
    int32       write_len = 0;     /* nbytes to write next */
    int32       ret_value = SUCCEED;
    intn        k;                 /* loop index */

#ifdef HAVE_PABLO
    TRACE_ON(H_mask, ID_HMCPwrite);
#endif /* HAVE_PABLO */

    /* Check args */
    if (access_rec == NULL)
        HGOTO_ERROR(DFE_ARGS, FAIL);

    /* Set inputs */
    data     = (uint8 *) datap;
    file_rec =  HAatom_object(access_rec->file_id);
    info     = (chunkinfo_t *) (access_rec->special_info);
    relative_posn = access_rec->posn;
    write_len     = info->chunk_size;
    bytes_written = 0;
    bptr          = (VOID *)datap;

#ifdef CHK_DEBUG
    printf("HMCPchunkwrite called with chunk %d \n",chunk_num);
#endif
    /* find chunk record in TBBT */
    if ((entry = (tbbtdfind(info->chk_tree, &chunk_num, NULL))) == NULL)
        HE_REPORT_GOTO("failed to find chunk record", FAIL);

    chk_rec = (CHUNK_REC *) entry->data; /* get file entry from node */

    /* Check to see if already created in chunk table */
    if (chk_rec->chk_tag == DFTAG_NULL)
      { /* does not exists in Vdata table and in file but does in TBBT */
          uint8 *pntr = NULL;

          chkptr = chk_rec;
          /* so create a new Vdata record */
          /* Allocate space for a single Chunk record in Vdata */
          if (v_data == NULL)
            {
                if ((v_data = (VOIDP) HDmalloc((info->ndims*sizeof(int32))
                                               + (2*sizeof(uint16)))) == NULL)
                    HGOTO_ERROR(DFE_NOSPACE, FAIL);
            }

          /* Initialize chunk record */
          chkptr->chk_tag = DFTAG_CHUNK;
          chkptr->chk_ref = Htagnewref(access_rec->file_id, DFTAG_CHUNK);
#ifdef CHK_DEBUG
          printf(" chktpr->chk_tag=%d, ",chkptr->chk_tag);
          printf(" chktpr->chk_ref=%d, ",chkptr->chk_ref);
#endif
          for (k = 0; k < info->ndims; k++)
            {
#ifdef CHK_DEBUG
                printf(" chktpr->origin[%d]=%d, ",k,chkptr->origin[k]);
#endif
            }
#ifdef CHK_DEBUG
          printf("\n");
#endif

          /* Copy origin first to vdata record*/
          pntr = v_data;
          for (k = 0; k < info->ndims; k++)
            {
                HDmemcpy(pntr, &chkptr->origin[k],sizeof(int32));
                pntr += sizeof(int32);
            }

          /* Copy tag next */
          HDmemcpy(pntr, &chkptr->chk_tag,sizeof(uint16));
          pntr += sizeof(uint16);

          /* Copy ref last */
          HDmemcpy(pntr, &chkptr->chk_ref,sizeof(uint16));

          /* Add to Vdata i.e. chunk table */
          if(VSwrite(info->aid,v_data,1,FULL_INTERLACE)==FAIL)
              HGOTO_ERROR(DFE_VSWRITE,FAIL);
      } /* not already in Vdata table */

    /* Start write on chunk */
    if ((chk_id = Hstartwrite(access_rec->file_id, chk_rec->chk_tag,
                              chk_rec->chk_ref,info->chunk_size)) == FAIL)
        HE_REPORT_GOTO("Hstartwrite failed to read chunk", FAIL);

    /* write data to chunk */
    if (Hwrite(chk_id, write_len, bptr) == FAIL)
        HGOTO_ERROR(DFE_WRITEERROR, FAIL);

    bytes_written = write_len;

    /* end access to chunk */
    if (Hendaccess(chk_id) == FAIL)
        HE_REPORT_GOTO("Hendaccess failed to end access to chunk", FAIL);

    ret_value = bytes_written;

  done:
    if(ret_value == FAIL)   
      { /* Error condition cleanup */
          if (chk_id != FAIL)
              Hendaccess(chk_id);
      } /* end if */

    /* Normal function cleanup */
    if (v_data != NULL)
        HDfree(v_data);

#ifdef HAVE_PABLO
    TRACE_OFF(H_mask, ID_HMCPwrite);
#endif /* HAVE_PABLO */

    return ret_value;
} /* HMCPwritechunk() */

/******************************************************************************
NAME
     HMCsetMaxcache - maximum number of chunks to cache 

DESCRIPTION
     Set the maximum number of chunks to cache.

     The values set here affects the current object's caching behaviour.
     The cache only only grow up if the cache is max'd out and otherwise
     it will be set the value 'maxcache' if the current number of chunks
     cached is less than 'maxcache'.

     Use flags arguement of 'HMC_PAGEALL' if the whole object is to be cached 
     in memory otherwise passs in zero.

RETURNS
     Returns number of 'maxcache' if successful and FAIL otherwise

NOTE
     This calls the real routine mcache_set_maxcache().
     Currently 'maxcache' has to be greater than 1. 

******************************************************************************/
int32
HMCsetMaxcache(int32 access_id, /* IN: access aid to mess with */
               int32 maxcache,  /* IN: max number of pages to cache */
               int32 flags      /* IN: flags = 0, HMC_PAGEALL */)
{
    CONSTR(FUNC, "HMCsetMaxcache");   /* for HERROR */
    accrec_t    *access_rec = NULL;   /* access record */
    chunkinfo_t *info       = NULL;   /* chunked element information record */
    int32 ret_value = SUCCEED;

    /* Check args */
    access_rec = HAatom_object(access_id);
    if (access_rec == NULL || maxcache < 1)
        HGOTO_ERROR(DFE_ARGS, FAIL);

    /* since this routine can be called by the user,
       need to check if this access id is special CHUNKED */
    if (access_rec->special == SPECIAL_CHUNKED)
      {
          info     = (chunkinfo_t *) (access_rec->special_info);

          if (info != NULL)
              ret_value =  mcache_set_maxcache(info->chk_cache,maxcache);
          else 
              ret_value = FAIL;
      }
    else /* not special */
        ret_value = FAIL;

done:
  if(ret_value == FAIL)   
    { /* Error condition cleanup */

    } /* end if */

  /* Normal function cleanup */

  return ret_value;
} /* HMCsetMaxcache() */

/* ------------------------------- HMCPwrite -------------------------------
NAME
   HMCwriteChunk -- write out a whole chunk

DESCRIPTION
   Write out some data from a chunked element.  
   This can be used by users to write whole chunks to the file
   based on chunk origin for now.

RETURNS
   The number of bytes written or FAIL on error
---------------------------------------------------------------------------*/
int32
HMCwriteChunk(int32 access_id,  /* IN: access aid to mess with */
              int32 *origin,    /* IN: origin of chunk to write */
              const VOID *datap /* IN: buffer for data */)
{
    CONSTR(FUNC, "HMCwritechunk");  /* for HERROR */
    accrec_t    *access_rec = NULL; /* access record */
    uint8       *data       = NULL; /* data buffer */
    filerec_t   *file_rec   = NULL; /* file record */
    chunkinfo_t *info       = NULL; /* chunked element information record */
    CHUNK_REC   *chk_rec    = NULL; /* current chunk */
    TBBT_NODE   *entry      = NULL; /* node off of  chunk tree */
    CHUNK_REC   *chkptr     = NULL; /* Chunk record to inserted in TBBT  */
    int32       *chk_key    = NULL; /* Chunk recored key for insertion in TBBT */
    uint8       *bptr       = NULL; /* data buffer pointer */
    VOID        *chk_data   = NULL; /* chunk data */
    uint8       *chk_dptr   = NULL; /* chunk data pointer */
    int32       relative_posn;      /* relative position in chunked element */
    int32       bytes_written = 0;  /* total #bytes written by HMCIwrite */
    int32       write_len = 0;      /* bytes to write next */
    int32       chunk_num = -1;     /* chunk number */
    int32       ret_value = SUCCEED;
    intn        k;                  /* loop index */


#ifdef HAVE_PABLO
    TRACE_ON(H_mask, ID_HMCPwrite);
#endif /* HAVE_PABLO */

    /* Check args */
    access_rec = HAatom_object(access_id);
    if (access_rec == NULL)
        HGOTO_ERROR(DFE_ARGS, FAIL);

    if (origin == NULL || datap == NULL)
        HGOTO_ERROR(DFE_ARGS, FAIL);

    /* validate file records */
    file_rec =  HAatom_object(access_rec->file_id);
    if (BADFREC(file_rec))
        HGOTO_ERROR(DFE_INTERNAL, FAIL);

    /* can write in this file? */
    if (!(file_rec->access & DFACC_WRITE))
        HGOTO_ERROR(DFE_DENIED, FAIL);

    /* since this routine can be called by the user,
       need to check if this access id is special CHUNKED */
    if (access_rec->special == SPECIAL_CHUNKED)
      {
          /* Set inputs */
          data     = (uint8 *) datap;
          info     = (chunkinfo_t *) (access_rec->special_info);
          relative_posn = access_rec->posn;
          write_len     = info->chunk_size;
          bytes_written = 0;
          bptr          = (VOID *)datap;

          /* calculate chunk number from origin */
          calculate_chunk_num(&chunk_num, info->ndims, origin, 
                              info->ddims);

          /* find chunk record in TBBT */
          if ((entry = (tbbtdfind(info->chk_tree, &chunk_num, NULL))) == NULL)
            { /* not in tree */
                
                /* so create a new chunk record */
                /* Allocate space for a chunk record */
                if ((chkptr = (CHUNK_REC *) HDmalloc(sizeof(CHUNK_REC))) == NULL)
                    HGOTO_ERROR(DFE_NOSPACE, FAIL);

                /* Allocate space for a origin in chunk record */
                if ((chkptr->origin = (int32 *) HDmalloc(info->ndims*sizeof(int32))) == NULL)
                    HGOTO_ERROR(DFE_NOSPACE, FAIL);

                /* allocate space for key */
                if ((chk_key = (int32 *)HDmalloc(sizeof(int32))) == NULL)
                    HGOTO_ERROR(DFE_NOSPACE, FAIL);

                /* Initialize chunk record */
                chkptr->chk_tag = DFTAG_NULL;
                chkptr->chk_ref = 0;
#ifdef CHK_DEBUG
                printf(" chktpr->chk_tag=%d, ",chkptr->chk_tag);
                printf(" chktpr->chk_ref=%d, ",chkptr->chk_ref);
#endif
                /* Intialize chunk origins */
                for (k = 0; k < info->ndims; k++)
                  {
                      chkptr->origin[k] = origin[k];
#ifdef CHK_DEBUG
                      printf(" chktpr->origin[%d]=%d, ",k,chkptr->origin[k]);
#endif
                  }
#ifdef CHK_DEBUG
                printf("\n");
#endif
                /* set chunk record number to next Vdata record number */
                chkptr->chk_vnum = info->num_recs++;

                /* set key to chunk number */
                chkptr->chunk_number = *chk_key = chunk_num;

                /* add to TBBT tree based on chunk number as the key */
                tbbtdins(info->chk_tree, (VOIDP) chkptr , chk_key);   

                /* assign over new chk */
                chk_rec = chkptr;

                /* re-intialize ptrs to allow for error-failure check */
                chkptr = NULL;
                chk_key = NULL;
            }
          else /* already in TBBT tree */
              chk_rec = (CHUNK_REC *) entry->data; /* get file entry from node */

          /* would be nice to get Chunk record from TBBT based on chunk number 
             and then get chunk data base on chunk vdata number but
             currently the chunk calculations return chunk 
             numbers and not Vdata record numbers. 
             This would reduce some overhead in the number of chunks
             dealt with in the cache */

          /* get chunk data from cache based on chunk number */
          if ((chk_data = (VOID *) mcache_get(info->chk_cache, chunk_num+1, 0)) == NULL)
              HE_REPORT_GOTO("failed to find chunk record", FAIL);

          chk_dptr = chk_data; /* set chunk data ptr */

          /* copy data from users buffer to chunk */
          HDmemcpy(chk_dptr, bptr, write_len);        

          /* put chunk back to cache */
          if (mcache_put(info->chk_cache, chk_data, MCACHE_DIRTY) == FAIL)
              HE_REPORT_GOTO("HMCPwrite: failed to put chunk back in cache", FAIL);

          bytes_written = write_len;

          /* update access record with bytes written */
          access_rec->posn += bytes_written; 

          ret_value = bytes_written;
      }
    else /* not special chunked element */
        ret_value = FAIL;

  done:
    if(ret_value == FAIL)   
      { /* Error condition cleanup */
          /* check chunk ptrs */
          if (chkptr != NULL)
            {
                if (chkptr->origin != NULL)
                    HDfree(chkptr->origin);
                HDfree(chkptr);
            }
          if (chk_key != NULL)
              HDfree(chk_key);
      } /* end if */

    /* Normal function cleanup */

#ifdef HAVE_PABLO
    TRACE_OFF(H_mask, ID_HMCPwrite);
#endif /* HAVE_PABLO */

    return ret_value;
}   /* HMCwritechunk */

/* ------------------------------- HMCPwrite -------------------------------
NAME
   HMCPwrite -- write out some data to a chunked element

DESCRIPTION
   Write out some data from a chunked element.  If we write
   passed the end of the existing element new chunks are created
   as needed.

RETURNS
   The number of bytes written or FAIL on error
---------------------------------------------------------------------------*/
int32
HMCPwrite(accrec_t * access_rec, /* IN: access record to mess with */
          int32 length,          /* IN: number of bytes to write */
          const VOIDP datap      /* IN: buffer for data */)
{
    CONSTR(FUNC, "HMCPwrite");    /* for HERROR */
    filerec_t   *file_rec = NULL; /* file record */
    chunkinfo_t *info     = NULL; /* chunked element information record */
    CHUNK_REC   *chk_rec  = NULL; /* current chunk */
    TBBT_NODE   *entry    = NULL; /* node off of  chunk tree */
    CHUNK_REC   *chkptr   = NULL; /* Chunk record to inserted in TBBT  */
    int32       *chk_key  = NULL; /* Chunk recored key for insertion in TBBT */
    uint8       *data     = NULL; /* data buffer */
    uint8       *bptr     = NULL; /* data buffer pointer */
    VOID        *chk_data = NULL; /* chunk data */
    uint8       *chk_dptr = NULL; /* chunk data pointer */
    int32       relative_posn;    /* relative position in chunked element */
    int32       bytes_written = 0;/* total #bytes written by HMCIwrite */
    int32       write_len = 0;    /* next write size */
    int32       write_seek = 0;   /* next write seek */
    int32       chunk_size = 0;   /* chunk size */
    int32       chunk_num = 0;    /* chunk number */
    int32       ret_value = SUCCEED;
    intn        k;                  /* loop index */
#ifdef CHK_DEBUG
    intn         i;
#endif

#ifdef HAVE_PABLO
    TRACE_ON(H_mask, ID_HMCPwrite);
#endif /* HAVE_PABLO */

    /* Check args */
    if (access_rec == NULL)
        HGOTO_ERROR(DFE_ARGS, FAIL);

    /* Set inputs */
    data     = (uint8 *) datap;
    file_rec =  HAatom_object(access_rec->file_id);
    info     = (chunkinfo_t *) (access_rec->special_info);
    relative_posn = access_rec->posn;
    write_len     = length;

    /* validate length and file records */
    if (length <= 0)
        HGOTO_ERROR(DFE_RANGE, FAIL);
    if (BADFREC(file_rec))
        HGOTO_ERROR(DFE_INTERNAL, FAIL);

    bytes_written = 0;
    bptr = datap;
    while (bytes_written < write_len) 
      {
          /* for debuging */
#ifdef CHK_DEBUG
          printf("Seek start(in chunk array):(");
          for (i = 0; i < info->ndims; i++)
              printf("%d%s", info->seek_chunk_indices[i], i!= info->ndims-1 ? ",":NULL);
          printf(")\n");
          printf(" Seek start(within the chunk):(");
          for (i = 0; i < info->ndims; i++)
              printf("%d%s", info->seek_pos_chunk[i], i!= info->ndims-1 ? ",":NULL);
          printf(")\n");
#endif

          /* calculate chunk to retrieve */
          calculate_chunk_num(&chunk_num,info->ndims,info->seek_chunk_indices,
                              info->ddims);

          /* calculate contiguous chunk size that we can write to this chunk */
          calculate_chunk_for_chunk(&chunk_size,write_len,bytes_written,
                                    info->seek_pos_chunk,info->ddims);

#ifdef CHK_DEBUG
          printf("    writing chunk(%d) of %d bytes ->\n", chunk_num, chunk_size);
#endif

          /* find chunk record in TBBT */
          if ((entry = (tbbtdfind(info->chk_tree, &chunk_num, NULL))) == NULL)
            { /* not in tree */
                
                /* so create a new chunk record */
                /* Allocate space for a chunk record */
                if ((chkptr = (CHUNK_REC *) HDmalloc(sizeof(CHUNK_REC))) == NULL)
                    HGOTO_ERROR(DFE_NOSPACE, FAIL);

                /* Allocate space for a origin in chunk record */
                if ((chkptr->origin = (int32 *) HDmalloc(info->ndims*sizeof(int32))) == NULL)
                    HGOTO_ERROR(DFE_NOSPACE, FAIL);

                /* allocate space for key */
                if ((chk_key = (int32 *)HDmalloc(sizeof(int32))) == NULL)
                    HGOTO_ERROR(DFE_NOSPACE, FAIL);

                /* Initialize chunk record */
                chkptr->chk_tag = DFTAG_NULL;
                chkptr->chk_ref = 0;
#ifdef CHK_DEBUG
                printf(" chktpr->chk_tag=%d, ",chkptr->chk_tag);
                printf(" chktpr->chk_ref=%d, ",chkptr->chk_ref);
#endif
                /* Intialize chunk origins */
                for (k = 0; k < info->ndims; k++)
                  {
                      chkptr->origin[k] = info->seek_chunk_indices[k];
#ifdef CHK_DEBUG
                      printf(" chktpr->origin[%d]=%d, ",k,chkptr->origin[k]);
#endif
                  }
#ifdef CHK_DEBUG
                printf("\n");
#endif
                /* set chunk record number to next Vdata record number */
                chkptr->chk_vnum = info->num_recs++;

                /* set key to chunk number */
                chkptr->chunk_number = *chk_key = chunk_num;

                /* add to TBBT tree based on chunk number as the key */
                tbbtdins(info->chk_tree, (VOIDP) chkptr , chk_key);   

                /* assign over new chk */
                chk_rec = chkptr;

                /* re-intialize ptrs to allow for error-failure check */
                chkptr = NULL;
                chk_key = NULL;
            }
          else /* already in TBBT tree */
              chk_rec = (CHUNK_REC *) entry->data; /* get file entry from node */

          /* would be nice to get Chunk record from TBBT based on chunk number 
             and then get chunk data base on chunk vdata number but
             currently the chunk calculations return chunk 
             numbers and not Vdata record numbers. 
             This would reduce some overhead in the number of chunks
             dealt with in the cache */
#ifdef CHK_DEBUG
         printf("getting chunk %d from cache\n",chunk_num);
#endif
          /* get chunk data from cache based on chunk number */
          if ((chk_data = (VOID *) mcache_get(info->chk_cache, chunk_num+1, 0)) == NULL)
              HE_REPORT_GOTO("failed to find chunk record", FAIL);

          chk_dptr = chk_data; /* set chunk data ptr */

          /* calculate position in chunk */
          calculate_seek_in_chunk(&write_seek,info->ndims,info->seek_pos_chunk,
                                  info->ddims);

#ifdef CHK_DEBUG
          fprintf(stderr,"HMCPwrite: write_seek = %d\n", write_seek);
#endif
          chk_dptr += write_seek; /* move to correct position in chunk */

          /* copy data from users buffer to chunk */
          HDmemcpy(chk_dptr, bptr, chunk_size);        

          /* put chunk back to cache */
          if (mcache_put(info->chk_cache, chk_data, MCACHE_DIRTY) == FAIL)
              HE_REPORT_GOTO("HMCPwrite: failed to put chunk back in cache", FAIL);

#if 0
          for (i=0; i < chunk_size; i++)
            {
                printf("%x(%d)",*(bptr+i),*(bptr+i));
                if (i != chunk_size - 1)
                    printf(",");
            }
          printf("\n");
          if (!strncat(orig_buf,bptr,chunk_size))
              printf("error: couldn't concatenat buffer to orig_buf \n");
#endif

          /* increment buffer pointer */
          bptr += chunk_size;

         /* adjust number of bytes already written */
          bytes_written += chunk_size; 

#ifdef CHK_DEBUG
          printf("     written %d bytes already ->", bytes_written);
          printf("\n");
#endif
          /* increment our starting seek postion i
           * i.e. find new chunk to start writing to */
          update_chunk_indices(chunk_size,info->seek_chunk_indices,
                               info->seek_pos_chunk,info->ndims,info->ddims);
      } /* end while "bytes_written" */

    /* update access record with bytes written */
    access_rec->posn += bytes_written; 

    ret_value = bytes_written;

  done:
    if(ret_value == FAIL)   
      { /* Error condition cleanup */
          /* check chunk ptrs */
          if (chkptr != NULL)
            {
                if (chkptr->origin != NULL)
                    HDfree(chkptr->origin);
                HDfree(chkptr);
            }
          if (chk_key != NULL)
              HDfree(chk_key);
      } /* end if */

    /* Normal function cleanup */

#ifdef HAVE_PABLO
    TRACE_OFF(H_mask, ID_HMCPwrite);
#endif /* HAVE_PABLO */

    return ret_value;
}   /* HMCPwrite */


/* ---------------------------------------------------------------------
NAME
   HMCPcloseAID -- close file but keep AID active

DESCRIPTION
   close the file currently being pointed to by this AID but 
   do *NOT* free the AID.

   This is called by Hnextread() which reuses an AID to point to
   the 'next' object as requested.  If the current object was an
   chunked object, the chunked information needs to be closed before all
   reference to it is lost.

   NOTE: Hnextread() is a bad fcn to use since it relies on previous state
         information. 

RETURNS
   SUCCEED / FAIL
---------------------------------------------------------------------------*/
int32
HMCPcloseAID(accrec_t *access_rec /* IN:  access record of file to close */)
{
    CONSTR(FUNC, "HMCPcloseAID");    /* for HERROR */
    chunkinfo_t *info     = NULL;    /* special information record */
    int32       ret_value = SUCCEED;

    /* check args */
    info =  (chunkinfo_t *) access_rec->special_info;
    if (info == NULL)
        HGOTO_ERROR(DFE_ARGS, FAIL);

    /* detach the special information record.
       If no more references to that, free the record */
    if (--(info->attached) == 0)
      {
          if (info->chk_cache != NULL)
            {
                /* Sync chunk cache */
                mcache_sync(info->chk_cache);
/*
#define STATISTICS 
*/
#ifdef STATISTICS
                mcache_stat(info->chk_cache);
#endif
                /* close chunk cache */
                mcache_close(info->chk_cache);
            }

          /* clean up chunk table lists and info record here */
          /* Use Vxxx interface to end access to Vdata info */
          if (info->aid != FAIL)
            {
                if (VSdetach(info->aid) == FAIL)
                    HGOTO_ERROR(DFE_CANTFLUSH, FAIL);
            }
          else
              HGOTO_ERROR(DFE_BADAID, FAIL);

          if (Vend(access_rec->file_id) == FAIL)
              HGOTO_ERROR(DFE_CANTFLUSH, FAIL);

          /* clean up chunk tree 
             note that currently the 'chkdestroynode' fcn
             also does Hendaccess() on each open chunk id.
             This is probably not a good place to do this as
             any errors would be ignored. */
          tbbtdfree(info->chk_tree, chkdestroynode, chkfreekey);

          /* free up stuff in special info */
          if (info->ddims != NULL)
              HDfree(info->ddims);
          if (info->seek_chunk_indices != NULL)
              HDfree(info->seek_chunk_indices);
          if (info->seek_pos_chunk != NULL)
              HDfree(info->seek_pos_chunk);
          if (info->fill_val != NULL)
              HDfree(info->fill_val);
          /* finally free up info */
          HDfree(info);
          access_rec->special_info = NULL;
      } /* attached to info */

done:
  if(ret_value == FAIL)   
    { /* Error condition cleanup */

    } /* end if */

  /* Normal function cleanup */

    return ret_value;
}   /* HMCPcloseAID */

/* ----------------------------- HPendaccess -----------------------------
NAME
   HMCPendacess -- close a chunk element AID

DESCRIPTION
   Free up all of the space used to store information about a
   chunked element.  Information is flushed to disk as
   it is created so this routine does NOT have to write anything
   out.

RETURNS
   SUCCEED / FAIL
--------------------------------------------------------------------------- */
intn
HMCPendaccess(accrec_t * access_rec /* IN:  access record to close */)
{
    CONSTR(FUNC, "HMCPendaccess");   /* for HERROR */
    filerec_t   *file_rec = NULL;    /* file record */
    intn        ret_value = SUCCEED;

#ifdef HAVE_PABLO
    TRACE_ON(H_mask, ID_HMCPendaccess);
#endif /* HAVE_PABLO */

    /* validate arguments first */
    if (access_rec == NULL)
        HGOTO_ERROR(DFE_ARGS, FAIL);        

    /* get file rec and special info */
    file_rec = HAatom_object(access_rec->file_id);

    /* validate file record */
    if (BADFREC(file_rec))
        HGOTO_ERROR(DFE_INTERNAL, FAIL);

    /* detach the special information record.
       If no more references to that, free the record */
    if (HMCPcloseAID(access_rec) == FAIL)
        HGOTO_ERROR(DFE_INTERNAL, FAIL);

    /* update file and access records */
    if (HTPendaccess(access_rec->ddid) == FAIL)
        HGOTO_ERROR(DFE_CANTFLUSH, FAIL);

    /* detach from the file */
    file_rec->attach--;

    /* free the access record */
    HDfree(access_rec);

  done:
    if(ret_value == FAIL)   
      { /* Error condition cleanup */

      } /* end if */

    /* Normal function cleanup */
#ifdef HAVE_PABLO
    TRACE_OFF(H_mask, ID_HMCPendaccess);
#endif /* HAVE_PABLO */

    return ret_value;
}   /* HMCPendaccess */

/* ------------------------------- HMCPinfo --------------------------------
NAME
   HMCPinfo -- return info about a chunk element

DESCRIPTION
   Return information about the given chunk element.  Info_chunk is
   assumed to be non-NULL.
   
RETURNS
   SUCCEED / FAIL
--------------------------------------------------------------------------- */
int32
HMCPinfo(accrec_t * access_rec,       /* IN: access record of access elemement */
         sp_info_block_t * info_block /* OUT: information about the special element */)
{
    CONSTR(FUNC, "HMCPinfo");       /* for HERROR */
    chunkinfo_t *info     = NULL;   /* special information record */
    int32       ret_value = SUCCEED;

#ifdef HAVE_PABLO
    TRACE_ON(H_mask, ID_HMCPinfo);
#endif /* HAVE_PABLO */

    /* Check args */
    if (access_rec == NULL)
        HGOTO_ERROR(DFE_ARGS, FAIL);

    /* validate access record */
    if (access_rec->special != SPECIAL_CHUNKED)
        HGOTO_ERROR(DFE_INTERNAL, FAIL);

    /* fill in the info_chunk */
    info =  (chunkinfo_t *) access_rec->special_info;
    info_block->key = SPECIAL_CHUNKED;
    info_block->chunk_size = info->chunk_size;

  done:
    if(ret_value == FAIL)   
      { /* Error condition cleanup */

      } /* end if */

    /* Normal function cleanup */

#ifdef HAVE_PABLO
    TRACE_OFF(H_mask, ID_HMCPinfo);
#endif /* HAVE_PABLO */

    return ret_value;
}   /* HMCPinfo */

/* ------------------------------ HMCPinquire ----------------------------- 
NAME
   HMCPinquire -- Hinquire for chunks

DESCRIPTION
   Return interesting information about a chunk element.
   NULL can be passed for any of the OUT parameters if their
   value is not needed.

RETURNS
   SUCCEED
--------------------------------------------------------------------------- */
int32
HMCPinquire(accrec_t *access_rec,  /* IN:  access record to return info about */
            int32 *pfile_id,       /* OUT: file ID; */
            uint16 *ptag,          /* OUT: tag of info record; */
            uint16 *pref,          /* OUT: ref of info record; */
            int32 *plength,        /* OUT: length of element; */
            int32 *poffset,        /* OUT: offset of element -- meaningless */
            int32 *pposn,          /* OUT: current position in element; */
            int16 *paccess,        /* OUT: access mode; */
            int16 *pspecial        /* OUT: special code; */)
{
    CONSTR(FUNC, "HMCPinquire");    /* for HERROR */
    uint16      data_tag, data_ref; /* Tag/ref of the data in the file */
    chunkinfo_t *info = NULL;       /* special information record */
    int32       ret_value = SUCCEED;

#ifdef HAVE_PABLO
    TRACE_ON(H_mask, ID_HMCPinquire);
#endif /* HAVE_PABLO */

    /* Check args */
    if (access_rec == NULL)
        HGOTO_ERROR(DFE_ARGS, FAIL);

    /* get special info */
    info =   (chunkinfo_t *) access_rec->special_info;

    /* get latest info for the dataset */
    if(HTPinquire(access_rec->ddid,&data_tag,&data_ref,NULL,NULL)==FAIL)
        HGOTO_ERROR(DFE_INTERNAL, FAIL);

    /* fill in the variables if they are present */
    if (pfile_id)
        *pfile_id = access_rec->file_id;
    if (ptag)
        *ptag = data_tag;
    if (pref)
        *pref = data_ref;
    if (plength)
        *plength = info->length;
    if (poffset)
        *poffset = 0;   /* meaningless */
    if (pposn)
        *pposn = access_rec->posn;
    if (paccess)
        *paccess = (int16)access_rec->access;
    if (pspecial)
        *pspecial = access_rec->special;

  done:
    if(ret_value == FAIL)   
      { /* Error condition cleanup */

      } /* end if */

    /* Normal function cleanup */
#ifdef HAVE_PABLO
    TRACE_OFF(H_mask, ID_HMCPinquire);
#endif /* HAVE_PABLO */

    return ret_value;
}   /* HMCPinquire */

