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

/*
 FILE
       hcomp.c
       HDF compressed data I/O routines

 REMARKS
        These functions read and write compressed data to HDF data objects.
        The compressed data objects are implemented as "special tags"
        in the HDF file and the "H" layer I/O routines break out to the
        functions in this module to deal with them.

 DESIGN
            The compression I/O functions are designed as state machines.
        There are two seperate state machines implemented, as layers
        on top of one another.
            The top layer is the modeling layer, whose purpose is to
        send/receive uncompressed bytes between the higher layer
        (the "H" layer routines) and the lower layer, the coding layer.
        The modeling layer is constrained to look like Standard C
        I/O functions to the upper layer, while sending data in
        potentially unusual orders to the coding layer.  [An unusual
        order of bytes would be a Peano or Hilbert curve instead
        of the raster order more normally used].
            The lower layer is the coding layer, whose purpose is to
        send/receive bytes of data to the higher layer (the modeling
        layer) and to send/receive bits/bytes of data to the bit I/O
        functions after encoding them with some compression scheme.
            Both of these layers are designed as independent state
        machines whose state contains all the information to restart
        at a given point.  The purpose of this is to "cache" the
        state of each layer at certain [convenient] times in order
        to increase performance during random I/O.

 BUGS/LIMITATIONS
        Currently the following design limitations are still in place:
            1 - Cannot compress an existing data element (will be fixed
                    before release)  [ I think this is done, but it needs
                    testing]

            2 - Statistic gathering from several types of compression
                    is not implemented (should be fixed before release)
            3 - "State caching" for improved performance in not implemented,
                    although some data-structures allow for it. (should be
                    fixed before release)
            4 - Random writing in compressed data is not supported (unlikely
                    to _ever_ be fixed)

 EXPORTED ROUTINES
        HCcreate - create or modify an existing data element to be compressed

 AUTHOR
       Quincey Koziol

 MODIFICATION HISTORY
    9/21/93 - Starting writing specs & coding prototype
   10/09/93 - Finished initial testing.  First version with only stdio
                modeling and RLE coding done.
*/

/* General HDF includes */
#define COMPRESS_MASTER
#include "hdf.h"
#include "hfile.h"

/* HDF compression includes */
#include "hcompi.h"         /* Internal definitions for compression */

/* Local defines */
#define COMP_HEADER_VERSION 0
#define COMP_HEADER_LENGTH  14
#define COMP_START_BLOCK    1

/* declaration of the functions provided in this module */
PRIVATE int32 HCIstaccess
    PROTO((accrec_t *access_rec, int16 access));

PRIVATE int32 HCIinit_coder
    PROTO((comp_coder_info_t *cinfo,comp_coder_t coder_type,
            comp_info *coder_info));

PRIVATE int32 HCIread_header
    PROTO((filerec_t *file_rec,accrec_t *access_rec,
        compinfo_t *info,dd_t *info_dd,comp_info *c_info,
        model_info *m_info));

PRIVATE int32 HCIwrite_header
    PROTO((filerec_t *file_rec,accrec_t *access_rec,compinfo_t *info,dd_t *dd,
            uint16 special_tag,uint16 ref));

PRIVATE int32 HCIinit_model
    PROTO((comp_model_info_t *minfo,comp_model_t model_type,
            model_info *m_info));

/* Private buffer */
PRIVATE uint8 *ptbuf = NULL;

/* comp_funcs -- struct of accessing functions for the compressed
   data element function modules.  The position of each function in
   the table is standard */

funclist_t comp_funcs={
    HCPstread,
    HCPstwrite,
    HCPseek,
    HCPinquire,
    HCPread,
    HCPwrite,
    HCPendaccess
};

/*--------------------------------------------------------------------------
 NAME
    HCIinit_coder -- Set the coder function pointers

 USAGE
    int32 HCIinit_coder(cinfo,coder_type,coder_info)
        comp_coder_info_t *cinfo;   IN/OUT: pointer to coder information to modify
        comp_coder_t coder_type;    IN: the type of encoding to use
        comp_info *coder_info;      IN: setup information for some encoding types

 RETURNS
    Return SUCCEED or FAIL

 DESCRIPTION
    Sets the encoder function pointers and the encoder type for a given
    coder type.

 GLOBAL VARIABLES
 COMMENTS, BUGS, ASSUMPTIONS
 EXAMPLES
 REVISION LOG
--------------------------------------------------------------------------*/
#ifdef PROTOTYPE
PRIVATE int32 HCIinit_coder(comp_coder_info_t *cinfo,comp_coder_t coder_type,
        comp_info *c_info)
#else
PRIVATE int32 HCIinit_coder(cinfo, coder_type, c_info)
    comp_coder_info_t *cinfo;   /* encoding information to modify */
    comp_coder_t coder_type;    /* type of encoding to use */
    comp_info *c_info;      /* encoding information */
#endif
{
    char *FUNC="HCIinit_coder";  /* for HERROR */

    switch(coder_type) {    /* determin the type of encoding */
        case COMP_CODE_RLE:           /* Run-length encoding */
            cinfo->coder_type=COMP_CODE_RLE;    /* set coding type */
            cinfo->coder_funcs=crle_funcs;      /* set the RLE func. ptrs */
            break;

        case COMP_CODE_NBIT:        /* N-bit encoding */
            cinfo->coder_type=COMP_CODE_NBIT;   /* set the coding type */
            cinfo->coder_funcs=cnbit_funcs;     /* set the N-bit func. ptrs */

            /* copy coding info */
            cinfo->coder_info.nbit_info.nt=c_info->nbit.nt;
            cinfo->coder_info.nbit_info.sign_ext=c_info->nbit.sign_ext;
            cinfo->coder_info.nbit_info.fill_one=c_info->nbit.fill_one;
            cinfo->coder_info.nbit_info.mask_off=c_info->nbit.start_bit;
            cinfo->coder_info.nbit_info.mask_len=c_info->nbit.bit_len;
#ifdef TESTING
printf("HCIinit_coder(): nt=%d, sign_ext=%d, fill_one=%d, start_bit=%d, bit_len=%d\n",c_info->nbit.nt,c_info->nbit.sign_ext,c_info->nbit.fill_one,c_info->nbit.start_bit,c_info->nbit.bit_len);
printf("HCIinit_coder(): coder_funcs.write=%p\n",cinfo->coder_funcs.write);
#endif
            if((cinfo->coder_info.nbit_info.nt_size
                    =DFKNTsize(cinfo->coder_info.nbit_info.nt))==FAIL)
                HRETURN_ERROR(DFE_BADNUMTYPE,FAIL);
            break;

        default:
           HRETURN_ERROR(DFE_BADCODER,FAIL);
      } /* end switch */
    return(SUCCEED);
}   /* end HCIinit_coder() */

/*--------------------------------------------------------------------------
 NAME
    HCIinit_model -- Set the model function pointers

 USAGE
    int32 HCIinit_model(minfo,model_type,m_info)
        comp_model_info_t *minfo;   IN/OUT: pointer to model information to modify
        comp_model_t model_type;    IN: the type of encoding to use
        model_info *m_info;         IN: modeling information

 RETURNS
    Return SUCCEED or FAIL

 DESCRIPTION
    Sets the modeling function pointers and the model type for a given
    model type.

 GLOBAL VARIABLES
 COMMENTS, BUGS, ASSUMPTIONS
 EXAMPLES
 REVISION LOG
--------------------------------------------------------------------------*/
#ifdef PROTOTYPE
PRIVATE int32 HCIinit_model(comp_model_info_t *minfo,comp_model_t model_type,
        model_info *m_info)
#else
PRIVATE int32 HCIinit_model(minfo, model_type, m_info)
    comp_model_info_t *minfo;   /* modeling information to modify */
    comp_model_t model_type;    /* type of modeling to use */
    model_info *m_info;     /* modeling information */
#endif
{
    char *FUNC="HCIinit_model";  /* for HERROR */

    switch(model_type) {    /* determine the type of modeling */
        case COMP_MODEL_STDIO:        /* standard C stdio modeling */
            minfo->model_type=COMP_MODEL_STDIO;    /* set model type */
            minfo->model_funcs=mstdio_funcs;    /* set the stdio func. ptrs */
            break;

        default:
           HRETURN_ERROR(DFE_BADMODEL,FAIL);
      } /* end switch */

    return(SUCCEED);
}   /* end HCIinit_model() */

/*--------------------------------------------------------------------------
 NAME
    HCIwrite_header -- Write the compression header info to a file

 USAGE
    int32 HCIwrite_header(file_rec,access_rec,info,dd)
        filerec_t *file_rec;    IN: ptr to the file record for the HDF file
        accrec_t *access_rec;   IN: ptr to the access element record
        compinfo_t *info;       IN: ptr the compression information
        dd_t *dd;               IN: ptr to the DD info for the element
        uint16 special_tag,ref; IN: the tag/ref of the compressed element

 RETURNS
    Return SUCCEED or FAIL

 DESCRIPTION
    Writes the compression information to a new block in the HDF file.

 GLOBAL VARIABLES
 COMMENTS, BUGS, ASSUMPTIONS
 EXAMPLES
 REVISION LOG
--------------------------------------------------------------------------*/
#ifdef PROTOTYPE
PRIVATE int32 HCIwrite_header(filerec_t *file_rec,accrec_t *access_rec,
        compinfo_t *info,dd_t *dd,uint16 special_tag,uint16 ref)
#else
PRIVATE int32 HCIwrite_header(file_rec,access_rec,info,dd,special_tag,ref)
    filerec_t *file_rec;    /* file record */
    accrec_t *access_rec;   /* access element record */
    compinfo_t *info;       /* special element information */
    dd_t *dd;               /* the DD of the new element */
    uint16 special_tag,ref; /* the tag/ref pair of the compressed element */
#endif
{
    char *FUNC="HCIwrite_header";  /* for HERROR */
    uint8 *p;       /* pointer to the temporary buffer */

    /* Check if temproray buffer has been allocated */
    if (ptbuf == NULL)
      {
        ptbuf = (uint8 *)HDgetspace(TBUF_SZ * sizeof(uint8));
        if (ptbuf == NULL)
          HRETURN_ERROR(DFE_NOSPACE, FAIL);
      }

    /* write special element info to the file */
    p = ptbuf;
    INT16ENCODE(p, SPECIAL_COMP);           /* specify special tag type */
    UINT16ENCODE(p, COMP_HEADER_VERSION);   /* specify header version */
    INT32ENCODE(p, info->length);           /* write length of un-comp. data */
    UINT16ENCODE(p, (uint16)info->comp_ref);/* specify ref # of comp. data */
    UINT16ENCODE(p, (uint16)info->minfo.model_type);    /* specify model type */
    UINT16ENCODE(p, (uint16)info->cinfo.coder_type);    /* specify coder type */

    /* write any additional information needed for modeling type */
    switch(info->minfo.model_type) {
        default:    /* no additional information needed */
            break;
      } /* end switch */

    /* write any additional information needed for coding type */
    switch(info->cinfo.coder_type) {
        case COMP_CODE_NBIT:    /* N-bit coding needs info */
#ifdef TESTING
printf("HCIwrite_header(): nt=%d, sign_ext=%d, fill_one=%d, start_bit=%d, bit_len=%d\n",info->cinfo.coder_info.nbit_info.nt,info->cinfo.coder_info.nbit_info.sign_ext,info->cinfo.coder_info.nbit_info.fill_one,info->cinfo.coder_info.nbit_info.mask_off,info->cinfo.coder_info.nbit_info.mask_len);
#endif
            /* specify number-type of N-bit data */
            INT32ENCODE(p, info->cinfo.coder_info.nbit_info.nt);
            /* next is the flag to indicate whether to sign extend */
            UINT16ENCODE(p, (uint16)info->cinfo.coder_info.nbit_info.sign_ext);
            /* flag to fill with 1's or 0's */
            UINT16ENCODE(p, (uint16)info->cinfo.coder_info.nbit_info.fill_one);
            /* the offset of the bits extracted */
            INT32ENCODE(p, (int32)info->cinfo.coder_info.nbit_info.mask_off);
            /* the number of bits extracted */
            INT32ENCODE(p, (int32)info->cinfo.coder_info.nbit_info.mask_len);
            break;

        default:    /* no additional information needed */
            break;
      } /* end switch */

    dd->tag=special_tag;
    dd->ref=ref;
    dd->length=p - ptbuf;      /* not a constant length */
#ifdef OLD_WAY
    if(HI_SEEKEND(file_rec->file)==FAIL) {
       access_rec->used=FALSE;
       HRETURN_ERROR(DFE_SEEKERROR,FAIL);
      } /* end if */

    /* write compressed special element data to the file */
    dd->offset=HI_TELL(file_rec->file);
#else
    if((dd->offset=HPgetdiskblock(file_rec,dd->length,TRUE))==FAIL) {
       access_rec->used=FALSE;
       HRETURN_ERROR(DFE_INTERNAL,FAIL);
      } /* end if */
#endif
    if(HI_WRITE(file_rec->file, ptbuf, dd->length)==FAIL) {
        access_rec->used=FALSE;
        HRETURN_ERROR(DFE_WRITEERROR,FAIL);
      } /* end if */

    /* update DD block in the file */
    if(FAIL==HIupdate_dd(file_rec,access_rec->block,access_rec->idx,FUNC)) {
        access_rec->used=FALSE;
        HRETURN_ERROR(DFE_CANTUPDATE,FAIL);
      } /* end if */

    /* add new DD to hash table */
    if(FAIL==HIadd_hash_dd(file_rec,dd->tag,dd->ref,access_rec->block,access_rec->idx)) {
        access_rec->used=FALSE;
        HRETURN_ERROR(DFE_CANTHASH,FAIL);
      } /* end if */
    return(SUCCEED);
}   /* end HCIwrite_header() */

/*--------------------------------------------------------------------------
 NAME
    HCIread_header -- Read the compression header info from a file

 USAGE
    int32 HCIread_header(file_rec,access_rec,info,info_dd,comp_info,model_info)
        filerec_t *file_rec;    IN: ptr to the file record for the HDF file
        accrec_t *access_rec;   IN: ptr to the access element record
        compinfo_t *info;       IN: ptr the compression information
        dd_t *info_dd;          IN: ptr to the DD info for the element
        comp_info *comp_info;   IN/OUT: ptr to encoding info
        model_info *model_info; IN/OUT: ptr to modeling info

 RETURNS
    Return SUCCEED or FAIL

 DESCRIPTION
    Parses the compression header from a data element in an HDF file.

 GLOBAL VARIABLES
 COMMENTS, BUGS, ASSUMPTIONS
 EXAMPLES
 REVISION LOG
--------------------------------------------------------------------------*/
#ifdef PROTOTYPE
PRIVATE int32 HCIread_header(filerec_t *file_rec,accrec_t *access_rec,
        compinfo_t *info,dd_t *info_dd,comp_info *c_info,
        model_info *m_info)
#else
PRIVATE int32 HCIread_header(file_rec,access_rec,info,info_dd,c_info,m_info)
    filerec_t *file_rec;    /* file record */
    accrec_t *access_rec;   /* access element record */
    compinfo_t *info;       /* special element information */
    dd_t *info_dd;          /* the DD of the new element */
    comp_info *c_info;   /* ptr to compression information to fill in */
    model_info *m_info; /* ptr to modeling information to fill in */
#endif
{
    char *FUNC="HCIread_header";    /* for HERROR */
    uint16 header_version;      /* version of the compression header */
    uint16 mtype,ctype;         /* temporary variables for model and coder type */
    uint8 *p;       /* pointer to the temporary buffer */


    /* Check if temproray buffer has been allocated */
    if (ptbuf == NULL) {
        ptbuf = (uint8 *)HDgetspace(TBUF_SZ * sizeof(uint8));
        if (ptbuf == NULL)
          HRETURN_ERROR(DFE_NOSPACE, FAIL);
      }

    if(HI_SEEK(file_rec->file, info_dd->offset+2)==FAIL)
        HRETURN_ERROR(DFE_SEEKERROR,FAIL);
    if(HI_READ(file_rec->file, ptbuf, (COMP_HEADER_LENGTH-2))==FAIL)
        HRETURN_ERROR(DFE_READERROR,FAIL);

    p = ptbuf;
    UINT16DECODE(p, header_version);    /* get header length */
    INT32DECODE(p, info->length);   /* get _uncompressed_ data length */
    UINT16DECODE(p, info->comp_ref);    /* get ref # of comp. data */
    UINT16DECODE(p, mtype);             /* get model type */
    info->minfo.model_type=(comp_model_t)mtype;
    UINT16DECODE(p, ctype);            /* get encoding type */
    info->cinfo.coder_type=(comp_coder_t)ctype;

    /* read any additional information needed for modeling type */
    switch(info->minfo.model_type) {
        default:    /* no additional information needed */
            break;
      } /* end switch */

    /* read any additional information needed for coding type */
    switch(info->cinfo.coder_type) {
        case COMP_CODE_NBIT:    /* N-bit coding needs info */
            {
                uint16 s_ext;               /* temp. var for sign extend */
                uint16 f_one;               /* temp. var for fill one */
                int32 m_off,m_len;          /* temp. var for mask offset and len */

                if(HI_READ(file_rec->file, p, 16)==FAIL)
                    HRETURN_ERROR(DFE_READERROR,FAIL);
                /* specify number-type of N-bit data */
                INT32DECODE(p, c_info->nbit.nt);
                /* next is the flag to indicate whether to sign extend */
                UINT16DECODE(p, s_ext);
                c_info->nbit.sign_ext=(bool)s_ext;
                /* the flag to indicate whether to fill with 1's or 0's */
                UINT16DECODE(p, f_one);
                c_info->nbit.fill_one=(bool)f_one;
                /* the offset of the bits extracted */
                INT32DECODE(p, m_off);
                c_info->nbit.start_bit=(intn)m_off;
                /* the number of bits extracted */
                INT32DECODE(p, m_len);
                c_info->nbit.bit_len=(intn)m_len;
#ifdef TESTING
printf("HCIread_header(): nt=%d, sign_ext=%d, fill_one=%d, start_bit=%d, bit_len=%d\n",c_info->nbit.nt,c_info->nbit.sign_ext,c_info->nbit.fill_one,c_info->nbit.start_bit,c_info->nbit.bit_len);
#endif
              } /* end case */
            break;

        default:    /* no additional information needed */
            break;
      } /* end switch */

    return(SUCCEED);
}   /* end HCIread_header() */

/*--------------------------------------------------------------------------
 NAME
    HCcreate -- Create a compressed data element

 USAGE
    int32 HCcreate(id,tag,ref,model_type,coder_type)
        int32 id;            IN: the file id to create the data in
        uint16 tag,ref;      IN: the tag/ref pair which is to be compressed
        intn model_type;     IN: the type of modeling to use
        intn coder_type;     IN: the type of encoding to use

 RETURNS
    Return an AID to the newly created compressed element, FAIL on error.

 DESCRIPTION
    Create a compressed data element.  If that data element already
    exists, we will compress that data element if it is currently un-compresed,
    or return FAIL if it is already compressed.

 GLOBAL VARIABLES
 COMMENTS, BUGS, ASSUMPTIONS
 EXAMPLES
 REVISION LOG
--------------------------------------------------------------------------*/
#ifdef PROTOTYPE
int32 HCcreate(int32 file_id, uint16 tag, uint16 ref, comp_model_t model_type,
        model_info *m_info, comp_coder_t coder_type,
        comp_info *c_info)
#else
int32 HCcreate(file_id, tag, ref, model_type, m_info, coder_type, c_info)
    int32 file_id;      /* file record id */
    uint16 tag, ref;    /* tag/ref of the special data element to create */
    comp_model_t model_type;    /* type of modeling to use */
    model_info *m_info;     /* modeling information */
    comp_coder_t coder_type;    /* type of encoding to use */
    comp_info *c_info;      /* coding information */
#endif
{
    char *FUNC="HCcreate";  /* for HERROR */
    filerec_t *file_rec;    /* file record */
    accrec_t *access_rec;   /* access element record */
    int slot;
    dd_t *dd;
    ddblock_t *data_block;  /* dd block ptr to exist data element */
    int32 data_idx;         /* dd list index to existing data element */
    compinfo_t *info;       /* special element information */
    dd_t *data_dd;          /* dd of existing regular element */
    uint16 special_tag;     /* special version of tag */
#ifdef OLD_WAY
    uint8 *p;               /* pointer into the temp. buffer */
#endif
    int32 ret;
#ifdef OLD_WAY
    uint16 ctype,mtype;     /* temp. storage for coder and model types */
#endif

#ifdef TESTING
printf("HCcreate(): entering\n");
#endif
    /* clear error stack and validate args */
    HEclear();
    file_rec=FID2REC(file_id);
    if(file_rec==NULL || file_rec->refcount==0 || SPECIALTAG(tag)
            || (special_tag=MKSPECIALTAG(tag))==DFTAG_NULL)
       HRETURN_ERROR(DFE_ARGS,FAIL);


    /* Check if temproray buffer has been allocated */
    if (ptbuf == NULL) {
        ptbuf = (uint8 *)HDgetspace(TBUF_SZ * sizeof(uint8));
        if (ptbuf == NULL)
          HRETURN_ERROR(DFE_NOSPACE, FAIL);
      }

    /* chech for access permission */
    if(!(file_rec->access & DFACC_WRITE))
       HRETURN_ERROR(DFE_DENIED,FAIL);

    /* get a slot in the access records table */
    if(FAIL==(slot=HIget_access_slot()))
       HRETURN_ERROR(DFE_TOOMANY,FAIL);

    access_rec=&access_records[slot];

#ifdef TESTING
printf("HCcreate(): check 1\n");
#endif
    /* look for existing data element of the same tag/ref */
    if(FAIL!=HIlookup_dd(file_rec, tag, ref, &data_block, &data_idx)) {
        data_dd=&(data_block->ddlist[data_idx]);
        if(SPECIALTAG(data_dd->tag)) {
           /* abort since we cannot convert the data element to a compressed
              data element */
           access_rec->used=FALSE;
           HRETURN_ERROR(DFE_CANTMOD,FAIL);
          } /* end if */
      } /* end if */
    else
       data_dd=NULL;

    /* look for empty dd to use */
    if(FAIL==HIlookup_dd(file_rec, DFTAG_NULL, DFREF_WILDCARD,
                         &file_rec->null_block, &file_rec->null_idx)) {
        if(FAIL==HInew_dd_block(file_rec, FILE_NDDS(file_rec), FUNC)) {
            access_rec->used=FALSE;
            HRETURN_ERROR(DFE_NOFREEDD,FAIL);
          }  /* end if */
        else {
           access_rec->block=file_rec->ddlast;
           access_rec->idx  =0;
          } /* end else */
      } /* end if */
    else {
        access_rec->block=file_rec->null_block;
        access_rec->idx  =file_rec->null_idx;
      } /* end else */
    dd=&access_rec->block->ddlist[access_rec->idx];

#ifdef TESTING
printf("HCcreate(): check 2\n");
#endif
    /* set up the special element information and write it to file */
    info=(compinfo_t *)HDgetspace(sizeof(compinfo_t));
    access_rec->special_info=(VOIDP)info;
    if(info==NULL) {
        access_rec->used=FALSE;
        HRETURN_ERROR(DFE_NOSPACE,FAIL);
      } /* end if */

    if(data_dd!=NULL)       /* compress existing data */
        info->length=data_dd->length;
    else    /* start new compressed data element */
        info->length=COMP_START_BLOCK;

    /* set up compressed special info structure */
    info->attached=1;
    info->comp_ref=Hnewref(file_id);         /* get the new reference # */
#ifdef TESTING
printf("HCcreate(): check 3\n");
#endif
    HCIinit_model(&(info->minfo),model_type,m_info);
#ifdef TESTING
printf("HCcreate(): check 4\n");
#endif
    HCIinit_coder(&(info->cinfo),coder_type,c_info);
#ifdef TESTING
printf("HCcreate(): check 5, coder_funcs.write=%p\n",info->cinfo.coder_funcs.write);
#endif

#ifdef OLD_WAY
    /* write special element info to the file */
    p = ptbuf;
    INT16ENCODE(p, SPECIAL_COMP);           /* specify special tag type */
    UINT16ENCODE(p, COMP_HEADER_VERSION);   /* specify header version */
    INT32ENCODE(p, info->length);           /* write length of un-comp. data */
    UINT16ENCODE(p, (uint16)info->comp_ref);/* specify ref # of comp. data */
    mtype=(uint16)model_type;
    UINT16ENCODE(p, (uint16)mtype);    /* specify model type stored */
    ctype=(uint16)coder_type;
    UINT16ENCODE(p, (uint16)ctype);    /* specify coder type stored */
    dd->tag=special_tag;
    dd->ref=ref;
    dd->length=COMP_HEADER_LENGTH;
#ifdef OLD_WAY
    if(HI_SEEKEND(file_rec->file)==FAIL) {
       access_rec->used=FALSE;
       HRETURN_ERROR(DFE_SEEKERROR,FAIL);
      } /* end if */
    dd->offset=HI_TELL(file_rec->file);
#else
    if((dd->offset=HPgetdiskblock(file_rec,dd->length,TRUE))==FAIL) {
       access_rec->used=FALSE;
       HRETURN_ERROR(DFE_SEEKERROR,FAIL);
      } /* end if */
#endif

    /* write compressed special element data to the file */
    if(HI_WRITE(file_rec->file, ptbuf, dd->length)==FAIL) {
        access_rec->used=FALSE;
        HRETURN_ERROR(DFE_WRITEERROR,FAIL);
      } /* end if */

    /* update DD block in the file */
    if(FAIL==HIupdate_dd(file_rec,access_rec->block,access_rec->idx,FUNC)) {
        access_rec->used=FALSE;
        HRETURN_ERROR(DFE_CANTUPDATE,FAIL);
      } /* end if */

    /* add new DD to hash table */
    if(FAIL==HIadd_hash_dd(file_rec,dd->tag,dd->ref,access_rec->block,access_rec->idx)) {
        access_rec->used=FALSE;
        HRETURN_ERROR(DFE_CANTHASH,FAIL);
      } /* end if */
#else
    if(HCIwrite_header(file_rec,access_rec,info,dd,special_tag,ref)==FAIL) {
        access_rec->used=FALSE;
        HRETURN_ERROR(DFE_WRITEERROR,FAIL);
      } /* end if */
#endif

#ifdef TESTING
printf("HCcreate(): check 6, coder_funcs.write=%p\n",info->cinfo.coder_funcs.write);
#endif
    /* update access record and file record */
    access_rec->special_func=&comp_funcs;
    access_rec->special=SPECIAL_COMP;
    access_rec->posn=0;
    access_rec->access=DFACC_WRITE;
    access_rec->file_id=file_id;
    access_rec->appendable=FALSE;   /* start data as non-appendable */
    access_rec->flush=FALSE;        /* start data as not needing flushing */

    file_rec->attach++;
    if(ref>file_rec->maxref)
        file_rec->maxref=ref;

#ifdef TESTING
printf("HCcreate(): check 6.5, coder_funcs.write=%p\n",info->cinfo.coder_funcs.write);
#endif
    /* propagate the initialization down to the modeling layer */
    if((ret=(*(info->minfo.model_funcs.stwrite))(access_rec))==FAIL) {
        access_rec->used=FALSE;
        HRETURN_ERROR(DFE_MODEL,FAIL);
      } /* end if */

#ifdef TESTING
printf("HCcreate(): check 6.6, coder_funcs.write=%p\n",info->cinfo.coder_funcs.write);
#endif
    /* compress the old DD and get rid of it, if there was one */
    if(data_dd!=NULL) {
        VOIDP buf;              /* temporary buffer */

        buf=HDgetspace(data_dd->length);
        if(buf==NULL) {
            access_rec->used=FALSE;
            HDfreespace((VOIDP)info);
            HRETURN_ERROR(DFE_NOSPACE,FAIL);
          } /* end if */
        if(HI_SEEK(file_rec->file, data_dd->offset)==FAIL) {
            access_rec->used=FALSE;
            HDfreespace((VOIDP)info);
            HDfreespace((VOIDP)buf);
            HRETURN_ERROR(DFE_SEEKERROR,FAIL);
          } /* end if */
        if(HI_READ(file_rec->file, buf, data_dd->length)==FAIL) {
            access_rec->used=FALSE;
            HDfreespace((VOIDP)info);
            HDfreespace((VOIDP)buf);
            HRETURN_ERROR(DFE_READERROR,FAIL);
          } /* end if */

        /* write the data through to the compression layer */
        if(HCPwrite(access_rec,data_dd->length,buf)==FAIL) {
            access_rec->used=FALSE;
            HRETURN_ERROR(DFE_MODEL,FAIL);
          } /* end if */

        /* seek back to the beginning of the data through to the compression layer */
        if(HCPseek(access_rec,0,DF_START)==FAIL) {
            access_rec->used=FALSE;
            HRETURN_ERROR(DFE_MODEL,FAIL);
          } /* end if */

        HDfreespace((VOIDP)buf);

        /* Delete the old DD from the file and memory hash table */
        if(FAIL==Hdeldd(file_id, data_dd->tag, data_dd->ref)) {
            access_rec->used=FALSE;
            HRETURN_ERROR(DFE_CANTDELDD,FAIL);
          } /* end if */
        if(FAIL==HIdel_hash_dd(file_rec, data_dd->tag, data_dd->ref)) {
            access_rec->used=FALSE;
            HRETURN_ERROR(DFE_CANTDELHASH,FAIL);
          } /* end if */
      } /* end if */

#ifdef TESTING
printf("HCcreate(): check 7, coder_funcs.write=%p\n",info->cinfo.coder_funcs.write);
#endif
    return(ASLOT2ID(slot));
}   /* end HCcreate() */

/*--------------------------------------------------------------------------
 NAME
    HCIstaccess -- Start accessing a compressed data element.

 USAGE
    int32 HCIstaccess(access_rec, access)
        accrec_t *access_rec;   IN: the access record of the data element
        int16 access;           IN: the type of access wanted

 RETURNS
    Returns an AID or FAIL

 DESCRIPTION
    Common code called by HCIstread and HCIstwrite

 GLOBAL VARIABLES
 COMMENTS, BUGS, ASSUMPTIONS
 EXAMPLES
 REVISION LOG
--------------------------------------------------------------------------*/
#ifdef PROTOTYPE
PRIVATE int32 HCIstaccess(accrec_t *access_rec, int16 access)
#else
PRIVATE int32 HCIstaccess(access_rec, access)
    accrec_t *access_rec;   /* access record */
    int16 access;           /* access mode */
#endif
{
    char *FUNC="HCIstaccess";   /* for HERROR */
    dd_t *info_dd;              /* dd of the special information element */
    compinfo_t *info;           /* special element information */
    filerec_t *file_rec;        /* file record */
#ifdef OLD_WAY
    uint16 header_version;      /* version of the compression header */
    uint16 mtype,ctype;         /* temporary variables for model and coder type */
#endif
    comp_info c_info;       /* encoding information from the header */
    model_info m_info;      /* modeling information from the header */

    /* get file record and validate */
    file_rec=FID2REC(access_rec->file_id);
    if(!file_rec || file_rec->refcount==0 || !(file_rec->access & access))
        HRETURN_ERROR(DFE_ARGS,FAIL);

    /* Check if temproray buffer has been allocated */
    if (ptbuf == NULL) {
        ptbuf = (uint8 *)HDgetspace(TBUF_SZ * sizeof(uint8));
        if (ptbuf == NULL)
          HRETURN_ERROR(DFE_NOSPACE, FAIL);
      }

    /* intialize the access record */
    access_rec->special=SPECIAL_COMP;
    access_rec->posn=0;
    access_rec->access=access;

    /* get the dd for information */
    info_dd=&access_rec->block->ddlist[access_rec->idx];

    /* get the special info record */
#ifdef OLD_WAY
/* this won't work because the compression state will get screwed up */
    access_rec->special_info=HIgetspinfo(access_rec,info_dd->tag,info_dd->ref);
    if(access_rec->special_info) {  /* found it from other access records */
        info=(compinfo_t *)access_rec->special_info;
        info->attached++;
      } /* end if */
    else {  /* look for information in the file */
#endif
#ifdef OLD_WAY
        if(HI_SEEK(file_rec->file, info_dd->offset+2)==FAIL) {
            access_rec->used=FALSE;
            HRETURN_ERROR(DFE_SEEKERROR,FAIL);
        }
        if((ret=HI_READ(file_rec->file, ptbuf, (COMP_HEADER_LENGTH-2)))==FAIL) {
            access_rec->used=FALSE;
            HRETURN_ERROR(DFE_READERROR,FAIL);
        }
        access_rec->special_info=(VOIDP)HDgetspace(sizeof(compinfo_t));
        info=(compinfo_t *)access_rec->special_info;
        if(info==NULL) {
           access_rec->used=FALSE;
           HRETURN_ERROR(DFE_NOSPACE,FAIL);
        }
        {
            uint8 *p = ptbuf;
            UINT16DECODE(p, header_version);    /* get header length */
            INT32DECODE(p, info->length);   /* get _uncompressed_ data length */
            UINT16DECODE(p, info->comp_ref);    /* get ref # of comp. data */
            UINT16DECODE(p, mtype);             /* get model type */
            model_type=(comp_model_t)mtype;
            UINT16DECODE(p, ctype);            /* get encoding type */
            coder_type=(comp_coder_t)ctype;
        }
        info->attached=1;
        if(HCIinit_model(&(info->minfo),model_type)==FAIL)
            HRETURN_ERROR(DFE_MINIT,FAIL);
        if(HCIinit_coder(&(info->cinfo),coder_type)==FAIL)
            HRETURN_ERROR(DFE_CINIT,FAIL);
#else
        access_rec->special_info=(VOIDP)HDgetspace(sizeof(compinfo_t));
        info=(compinfo_t *)access_rec->special_info;
        if(info==NULL)
            HRETURN_ERROR(DFE_NOSPACE,FAIL);

        if(HCIread_header(file_rec,access_rec,info,info_dd,&c_info,
                    &m_info)==FAIL) {
            HDfreespace(info);
            HRETURN_ERROR(DFE_COMPINFO,FAIL);
          } /* end if */
        info->attached=1;
        if(HCIinit_model(&(info->minfo),info->minfo.model_type,&m_info)==FAIL) {
            HDfreespace(info);
            HRETURN_ERROR(DFE_MINIT,FAIL);
          } /* end if */
        if(HCIinit_coder(&(info->cinfo),info->cinfo.coder_type,&c_info)==FAIL) {
            HDfreespace(info);
            HRETURN_ERROR(DFE_CINIT,FAIL);
          } /* end if */
#endif
#ifdef OLD_WAY
      } /* end else */
#endif

    file_rec->attach++;

    return(ASLOT2ID(access_rec-access_records));
}   /* end HCIstaccess() */

/*--------------------------------------------------------------------------
 NAME
    HCPstread -- Start read access on a compressed data element.

 USAGE
    int32 HCPstread(access_rec)
        accrec_t *access_rec;   IN: the access record of the data element

 RETURNS
    Returns an AID or FAIL

 DESCRIPTION
    Start read access on a compressed data element.

 GLOBAL VARIABLES
 COMMENTS, BUGS, ASSUMPTIONS
 EXAMPLES
 REVISION LOG
--------------------------------------------------------------------------*/
#ifdef PROTOTYPE
int32 HCPstread(accrec_t *access_rec)
#else
int32 HCPstread(access_rec)
    accrec_t *access_rec;
#endif
{
    char *FUNC="HCPstread";     /* for HERROR */
    compinfo_t *info;           /* information on the special element */
    int32 aid;                  /* AID to return */
    int32 ret;

    if((aid=HCIstaccess(access_rec, DFACC_READ))==FAIL) {
        access_rec->used=FALSE;
        HRETURN_ERROR(DFE_DENIED,FAIL);
      } /* end if */
    info=(compinfo_t *)access_rec->special_info;
    if((ret=(*(info->minfo.model_funcs.stread))(access_rec))==FAIL) {
        access_rec->used=FALSE;
        HRETURN_ERROR(DFE_MODEL,FAIL);
      } /* end if */
    return(aid);
}   /* end HCPstread() */

/*--------------------------------------------------------------------------
 NAME
    HCPstwrite -- Start write access on a compressed data element.

 USAGE
    int32 HCPstwrite(access_rec)
        accrec_t *access_rec;   IN: the access record of the data element

 RETURNS
    Returns an AID or FAIL

 DESCRIPTION
    Start write access on a compressed data element.

 GLOBAL VARIABLES
 COMMENTS, BUGS, ASSUMPTIONS
 EXAMPLES
 REVISION LOG
--------------------------------------------------------------------------*/
#ifdef PROTOTYPE
int32 HCPstwrite(accrec_t *access_rec)
#else
int32 HCPstwrite(access_rec)
    accrec_t *access_rec;
#endif
{
    char *FUNC="HCPstwrite";    /* for HERROR */
    compinfo_t *info;           /* information on the special element */
    int32 aid;                  /* AID to return */
    int32 ret;

    if((aid=HCIstaccess(access_rec, DFACC_WRITE))==FAIL) {
        access_rec->used=FALSE;
        HRETURN_ERROR(DFE_DENIED,FAIL);
      } /* end if */
    info=(compinfo_t *)access_rec->special_info;
    if((ret=(*(info->minfo.model_funcs.stwrite))(access_rec))==FAIL) {
        access_rec->used=FALSE;
        HRETURN_ERROR(DFE_MODEL,FAIL);
      } /* end if */
    return(aid);
}   /* end HCPstwrite() */

/*--------------------------------------------------------------------------
 NAME
    HCPseek -- Seek to offset within the data element

 USAGE
    int32 HCPseek(access_rec,offset,origin)
        accrec_t *access_rec;   IN: the access record of the data element
        int32 offset;       IN: the offset in bytes from the origin specified
        intn origin;        IN: the origin to seek from

 RETURNS
    Returns SUCCEED or FAIL

 DESCRIPTION
    Seek to a position with a compressed data element.

 GLOBAL VARIABLES
 COMMENTS, BUGS, ASSUMPTIONS
 EXAMPLES
 REVISION LOG
--------------------------------------------------------------------------*/
#ifdef PROTOTYPE
int32 HCPseek(accrec_t *access_rec, int32 offset, intn origin)
#else
int32 HCPseek(access_rec, offset, origin)
    accrec_t *access_rec;
    int32 offset;
    intn origin;
#endif
{
    char *FUNC="HCPseek";   /* for HERROR */
    compinfo_t *info;       /* information on the special element */
    int32 ret;

    /* Adjust offset according to origin.  There is no upper bound to posn */
    if(origin==DF_CURRENT)
        offset+=access_rec->posn;
    if(origin==DF_END)
        offset+=((compinfo_t *)(access_rec->special_info))->length;
    if(offset<0)
       HRETURN_ERROR(DFE_RANGE,FAIL);

    info=(compinfo_t *)access_rec->special_info;
    if((ret=(*(info->minfo.model_funcs.seek))(access_rec,offset,origin))==FAIL)
        HRETURN_ERROR(DFE_MODEL,FAIL);

    /* set the offset */
    access_rec->posn=offset;

    return(ret);
}   /* end HCPseek() */

/*--------------------------------------------------------------------------
 NAME
    HCPread -- Read in a portion of data from a compressed data element.

 USAGE
    int32 HCPread(access_rec,length,data)
        accrec_t *access_rec;   IN: the access record of the data element
        int32 length;           IN: the number of bytes to read
        VOIDP data;             OUT: the buffer to place the bytes read

 RETURNS
    Returns the number of bytes read or FAIL

 DESCRIPTION
    Read in a number of bytes from a compressed data element.

 GLOBAL VARIABLES
 COMMENTS, BUGS, ASSUMPTIONS
 EXAMPLES
 REVISION LOG
--------------------------------------------------------------------------*/
#ifdef PROTOTYPE
int32 HCPread(accrec_t *access_rec, int32 length, VOIDP data)
#else
int32 HCPread(access_rec, length, data)
    accrec_t *access_rec;   /* access record */
    int32 length;           /* length of data to read in */
    VOIDP data;             /* data buffer */
#endif
{
    char *FUNC="HCPread";   /* for HERROR */
    compinfo_t *info;       /* information on the special element */
    int32 ret;

    /* validate length */
    if(length<0)
       HRETURN_ERROR(DFE_RANGE,FAIL);

    info=(compinfo_t *)access_rec->special_info;

    /* adjust length if it falls off the end of the element */
    if(length==0)
        length=info->length-access_rec->posn;
    else
        if(length<0 || access_rec->posn+length>info->length)
           HRETURN_ERROR(DFE_RANGE,FAIL);

    if((ret=(*(info->minfo.model_funcs.read))(access_rec,length,data))==FAIL)
        HRETURN_ERROR(DFE_MODEL,FAIL);

    /* adjust access position */
    access_rec->posn+=length;

    return(length);
}   /* end HCPread() */

/*--------------------------------------------------------------------------
 NAME
    HCPwrite -- Write out a portion of data from a compressed data element.

 USAGE
    int32 HCPwrite(access_rec,length,data)
        accrec_t *access_rec;   IN: the access record of the data element
        int32 length;           IN: the number of bytes to write
        VOIDP data;             IN: the buffer to retrieve the bytes written

 RETURNS
    Returns the number of bytes written or FAIL

 DESCRIPTION
    Write out a number of bytes to a compressed data element.

 GLOBAL VARIABLES
 COMMENTS, BUGS, ASSUMPTIONS
 EXAMPLES
 REVISION LOG
--------------------------------------------------------------------------*/
#ifdef PROTOTYPE
int32 HCPwrite(accrec_t *access_rec, int32 length, VOIDP data)
#else
int32 HCPwrite(access_rec, length, data)
    accrec_t *access_rec;      /* access record */
    int32 length;              /* length of data to write */
    VOIDP data;                        /* data buffer */
#endif
{
    char *FUNC="HCPwrite";     /* for HERROR */
    compinfo_t *info;       /* information on the special element */
    int32 ret;

#ifdef TESTING
printf("HCPwrite(): entering\n");
#endif
    /* validate length */
    if(length<0)
       HRETURN_ERROR(DFE_RANGE,FAIL);


    /* Check if temproray buffer has been allocated */
    if (ptbuf == NULL) {
        ptbuf = (uint8 *)HDgetspace(TBUF_SZ * sizeof(uint8));
        if (ptbuf == NULL)
          HRETURN_ERROR(DFE_NOSPACE, FAIL);
      }

#ifdef TESTING
printf("HCPwrite(): before func ptr call\n");
#endif
    info=(compinfo_t *)access_rec->special_info;
    if((ret=(*(info->minfo.model_funcs.write))(access_rec,length,data))==FAIL)
        HRETURN_ERROR(DFE_MODEL,FAIL);
#ifdef TESTING
printf("HCPwrite(): after func ptr call\n");
#endif

    /* update access record, and information about special element */
    access_rec->posn+=length;
    if(access_rec->posn>info->length) {
        uint8 *p = ptbuf;           /* temp buffer ptr */
        dd_t *info_dd=           /* dd of infromation element */
            &access_rec->block->ddlist[access_rec->idx];
        filerec_t *file_rec=FID2REC(access_rec->file_id);    /* file record */

        info->length=access_rec->posn;
        INT32ENCODE(p, info->length);
        if(HI_SEEK(file_rec->file, info_dd->offset+4)==FAIL)
            HRETURN_ERROR(DFE_SEEKERROR,FAIL);
        if(HI_WRITE(file_rec->file, ptbuf, 4)==FAIL) /* re-write un-comp. len */
            HRETURN_ERROR(DFE_WRITEERROR,FAIL);
      } /* end if */

    return(length);
}   /* end HCPwrite() */

/*--------------------------------------------------------------------------
 NAME
    HCPinquire -- Inquire information about the access record and data element.

 USAGE
    int32 HCPinquire(access_rec,pfile_id,ptag,pref,plength,poffset,pposn,
            paccess,pspecial)
        accrec_t *access_rec;   IN: the access record of the data element
        int32 *pfile_id;        OUT: ptr to file id
        uint16 *ptag;           OUT: ptr to tag of information
        uint16 *pref;           OUT: ptr to ref of information
        int32 *plength;         OUT: ptr to length of data element
        int32 *poffset;         OUT: ptr to offset of data element
        int32 *pposn;           OUT: ptr to position of access in element
        int16 *paccess;         OUT: ptr to access mode
        int16 *pspecial;        OUT: ptr to special code

 RETURNS
    Returns SUCCEED or FAIL

 DESCRIPTION
    Inquire information about the access record and data element.

 GLOBAL VARIABLES
 COMMENTS, BUGS, ASSUMPTIONS
 EXAMPLES
 REVISION LOG
--------------------------------------------------------------------------*/
#ifdef PROTOTYPE
int32 HCPinquire(accrec_t *access_rec, int32 *pfile_id, uint16 *ptag,
    uint16 *pref, int32 *plength, int32 *poffset,int32 *pposn,int16 *paccess,
    int16 *pspecial)
#else
int32 HCPinquire(access_rec, pfile_id, ptag, pref, plength, poffset, pposn,
            paccess, pspecial)
    accrec_t *access_rec;   /* access record */
    int32 *pfile_id;        /* ptr to file id, OUT */
    uint16 *ptag;           /* ptr to tag of information, OUT */
    uint16 *pref;           /* ptr to ref of information, OUT */
    int32 *plength;         /* ptr to length of data element, OUT */
    int32 *poffset;         /* ptr to offset of data element, OUT */
    int32 *pposn;           /* ptr to position of access in element, OUT */
    int16 *paccess;         /* ptr to access mode, OUT */
    int16 *pspecial;        /* ptr to special code, OUT */
#endif
{
    dd_t *info_dd=           /* dd of special information */
       &(access_rec->block->ddlist[access_rec->idx]);
    compinfo_t *info=         /* special information record */
       (compinfo_t *)access_rec->special_info;

    /* fill in the variables if they are present */
    if(pfile_id!=NULL)
        *pfile_id=access_rec->file_id;
    if(ptag!=NULL)
        *ptag=info_dd->tag;
    if(pref!=NULL)
        *pref=info_dd->ref;
    if(plength!=NULL)
        *plength=info->length;
    if(poffset!=NULL)
        *poffset=info_dd->offset;
    if(pposn!=NULL)
        *pposn=access_rec->posn;
    if(paccess!=NULL)
        *paccess=access_rec->access;
    if(pspecial!=NULL)
        *pspecial=access_rec->special;

    return(SUCCEED);
}   /* end HCPinquire() */

/*--------------------------------------------------------------------------
 NAME
    HCPendaccess -- Close the compressed data element and free the AID

 USAGE
    int32 HCPendaccess(access_rec)
        accrec_t *access_rec;   IN: the access record of the data element

 RETURNS
    Returns SUCCEED or FAIL

 DESCRIPTION
    Close the compressed data element and free the AID.

 GLOBAL VARIABLES
 COMMENTS, BUGS, ASSUMPTIONS
 EXAMPLES
 REVISION LOG
--------------------------------------------------------------------------*/
#ifdef PROTOTYPE
int32 HCPendaccess(accrec_t *access_rec)
#else
int32 HCPendaccess(access_rec)
    accrec_t *access_rec;      /* access record to dispose of */
#endif
{
    char *FUNC="HCPendaccess";      /* for HERROR */
    filerec_t *file_rec=FID2REC(access_rec->file_id);   /* file record */

    /* validate file record */
    if(file_rec==(filerec_t *) NULL || file_rec->refcount==0)
       HRETURN_ERROR(DFE_INTERNAL,FAIL);

    /* close the file pointed to by this access rec */
    HCPcloseAID(access_rec);

    /* detach from the file */
    file_rec->attach--;

    /* free the access record */
    access_rec->used=FALSE;

    return(SUCCEED);
}   /* end HCPendaccess() */

/*--------------------------------------------------------------------------
 NAME
    HCPcloseAID -- Get rid of the compressed data element data structures

 USAGE
    int32 HCPcloseAID(access_rec)
        accrec_t *access_rec;   IN: the access record of the data element

 RETURNS
    Returns SUCCEED or FAIL

 DESCRIPTION
    Get rid of the compressed data element internal data structures

 GLOBAL VARIABLES
 COMMENTS, BUGS, ASSUMPTIONS
 EXAMPLES
 REVISION LOG
--------------------------------------------------------------------------*/
#ifdef PROTOTYPE
int32 HCPcloseAID(accrec_t *access_rec)
#else
int32 HCPcloseAID(access_rec)
    accrec_t *access_rec;      /* access record to dispose of */
#endif
{
    char *FUNC="HCPcloseAID"; /* for HERROR */
    compinfo_t *info;         /* special information record */
    int32 ret;

    info=(compinfo_t *)access_rec->special_info;
    if((ret=(*(info->minfo.model_funcs.endaccess))(access_rec))==FAIL)
        HRETURN_ERROR(DFE_MODEL,FAIL);

    /* Free the compression information */
    if(--(info->attached)==0)
        HDfreespace(info);

    return(SUCCEED);
}   /* end HCPcloseAID() */
