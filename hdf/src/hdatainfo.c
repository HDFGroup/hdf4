/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Copyright by The HDF Group.                                               *
 * Copyright by the Board of Trustees of the University of Illinois.         *
 * All rights reserved.                                                      *
 *                                                                           *
 * This file is part of HDF.  The full HDF copyright notice, including       *
 * terms governing use, modification, and redistribution, is contained in    *
 * the files COPYING and Copyright.html.  COPYING can be found at the root   *
 * of the source code distribution tree; Copyright.html can be found at      *
 * http://hdfgroup.org/products/hdf4/doc/Copyright.html.  If you do not have *
 * access to either file, you may request a copy from help@hdfgroup.org.     *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifdef RCSID
static char RcsId[] = "@(#)$Revision: 5393 $";
#endif

/* $Id: hdatainfo.c 5393 2010-03-25 18:38:17Z bmribler $ */

/*
FILE
   hdatainfo.c
   HDF data information routines

REMARKS
   These functions are the low-level functions of the "datainfo" functions,
   such as SDgetdatainfo, VSgetdatainfo, VSgetattrdatainfo...

*/

#include "atom.h"
#include "hfile.h"

#ifdef H4_HAVE_LIBSZ
#include "szlib.h"
#endif

/*--------------------------------------------------------------------------
 NAME
    HDgetdatainfo_count -- Gets the number of data blocks in an element
 USAGE
    int32 HDgetdatainfo(file_id, data_tag, data_ref)
		int32  file_id;		IN: file id
		uint16 data_tag;	IN: tag of the element
		uint16 data_ref;	IN: ref of element
 RETURNS
    The number of data blocks in the element, if successful, and FAIL, otherwise

 DESCRIPTION
--------------------------------------------------------------------------*/
intn HDgetdatainfo_count(
	int32 file_id,
	uint16 data_tag, uint16 data_ref) /* IN: tag/ref of element */
{
    CONSTR(FUNC, "HDgetdatainfo_count");	/* for HGOTO_ERROR */
    int32	data_len;	/* offset of the data we are checking */
    uint16	sp_tag;		/* special tag */
    atom_t      data_id = FAIL;	/* dd ID of existing regular element */
    uint8      *drec_buf=NULL, *p;
    filerec_t  *file_rec;	/* file record */
    uintn	info_count = 0;
    int32	length; /* offset and length of a data block */
    intn        ret_value=SUCCEED;

    /* clear error stack */
    HEclear();

    /* convert file id to file rec and check for validity */
    file_rec = HAatom_object(file_id);
    if (BADFREC(file_rec))
	HGOTO_ERROR(DFE_ARGS, FAIL);

    /* get access element from dataset's tag/ref */
    if ((data_id=HTPselect(file_rec, data_tag, data_ref))!=FAIL)
    {
	/* if the element is not special, that means dataset's tag/ref 
	   specifies the actual data that was written to the dataset in
	   one block, so we don't need to check further */
	if (HTPis_special(data_id)==FALSE)
        {
	    info_count = 1;
        }

	/* if the element is special, get the special info header and decode
	   for special tag to detect compression/chunking/linked blocks */
	else
	{
            /* get the info for the dataset (description record) */
            data_len = HPread_drec(file_id, data_id, &drec_buf);
	    if (data_len <= 0)
		HGOTO_ERROR(DFE_READERROR, FAIL);

	    /* get special tag */
	    p = drec_buf;
	    INT16DECODE(p, sp_tag);

	    /* if this is a compressed element, then it should only be none or
	       one block of data */
	    if (sp_tag == SPECIAL_COMP)
	    {
		/* skip 2byte header_version */
		p = p + 2;
		INT32DECODE(p, length);	/* get _uncompressed_ data length */

		/* No data written */
		if (length == 0)
		    info_count = 0;

		/* Data has been written to one block of data */
		else
		    info_count = 1;
	    } /* element is compressed */

	    /* if this is a chunked element, hand the task over to the chunking
		layer. */
	    else if (sp_tag == SPECIAL_CHUNKED)
	    {
		if (HMCgetdatainfo_count(file_id, p, &info_count)==FAIL)
		    HGOTO_ERROR(DFE_INTERNAL, FAIL);
	    }

	    /* if this is a linked block element, get the number of blocks */
	    else if (sp_tag == SPECIAL_LINKED) /* unlimited dimension here */
	    {
		int32 num_blocks;
		uint16 link_ref;

		p = p + 4 + 4; /* skip length, block_size */
		INT32DECODE(p, num_blocks);
		UINT16DECODE(p, link_ref);

		/* get data information from the linked blocks */
		info_count = HLgetdatainfo(file_id, link_ref, num_blocks, NULL, NULL);
    } /* element is SPECIAL_LINKED */
	} /* else, data_id is special */

	/* end access to the aid */
	if (HTPendaccess(data_id) == FAIL)
	    HGOTO_ERROR(DFE_CANTENDACCESS, FAIL);
    }  /* end if data_id != FAIL */

    else /* HTPselect failed */
        HGOTO_ERROR(DFE_CANTACCESS, FAIL);

    ret_value = info_count; 
done:
    if(ret_value == FAIL)   
    { /* Error condition cleanup */
    } /* end if */

    /* Normal function cleanup */
    if (drec_buf != NULL) 
	HDfree(drec_buf);

    return ret_value;
} /* HDgetdatainfo_count */


/*--------------------------------------------------------------------------
 NAME
    HDgetdatainfo -- Retrieves the offset(s) and length(s) of the data in
		      the data element.
 USAGE
    int32 HDgetdatainfo(file_id, data_tag, data_ref, start_block, info_count,
			 *offsetarray, *lengtharray)	
		int32  file_id;		IN: file id
		uint16 data_tag;	IN: tag of the element
		uint16 data_ref;	IN: ref of element
		uintn  start_block;	IN: data block to start at, 0 base
		uintn  info_count;	IN: number of info records
		int32 *offsetarray;	OUT: array to hold offsets
		int32 *lengtharray;	OUT: array to hold lengths
 RETURNS
    SUCCEED/FAIL
 DESCRIPTION
    
--------------------------------------------------------------------------*/
intn
HDgetdatainfo(int32 file_id,
		uint16 data_tag, uint16 data_ref, /* IN: tag/ref of element */
		uintn start_block,	/* IN: data block to start at, 0 base */
		uintn info_count,	/* IN: number of info records */
		int32 *offsetarray,	/* OUT: array to hold offsets */
		int32 *lengtharray)	/* OUT: array to hold lengths */
{
    CONSTR(FUNC, "HDgetdatainfo");	/* for HGOTO_ERROR */
    int32	data_len;	/* offset of the data we are checking */
    uint16	sp_tag;		/* special tag */
    uint16	comp_ref = 0;
    uint8      *drec_buf=NULL, *p;	/* description record buffer */
    atom_t      data_id = FAIL;	/* dd ID of existing element */
    int32	offset, length; /* offset and length of a data block */
    filerec_t  *file_rec;	/* file record */
    intn	ret_value=SUCCEED;

    /* clear error stack */
    HEclear();

    /* validate arguments */
    if (start_block < 0)
        HGOTO_ERROR(DFE_ARGS, FAIL);
    if (info_count <= 0)
        HGOTO_ERROR(DFE_ARGS, FAIL);
    if (offsetarray == NULL && lengtharray == NULL) /* one can be NULL */
        HGOTO_ERROR(DFE_ARGS, FAIL);

    /* convert file id to file rec and check for validity */
    file_rec = HAatom_object(file_id);
    if (BADFREC(file_rec))
	HGOTO_ERROR(DFE_ARGS, FAIL);

    /* get access element from dataset's tag/ref */
    if ((data_id=HTPselect(file_rec, data_tag, data_ref))!=FAIL)
    {
	/* if the element is not special, that means dataset's tag/ref 
	   specifies the actual data that was written to the dataset, get
	   the offset and length of the data */
	if (HTPis_special(data_id)==FALSE)
        {
	    if ((offset = Hoffset(file_id, data_tag, data_ref)) == FAIL)
		HGOTO_ERROR(DFE_BADOFFSET, FAIL);
	    if ((length = Hlength(file_id, data_tag, data_ref)) == FAIL)
		HGOTO_ERROR(DFE_BADLEN, FAIL);

	    /* Only one data block here, starting offset cannot be > 1 */
	    if (start_block > 1)
		HGOTO_ERROR(DFE_ARGS, FAIL);

	    offsetarray[0] = offset;
	    lengtharray[0] = length;
	    ret_value = 1;
        }

	/* if the element is special, get the special info header and decode
	   for special tag to detect compression/chunking/linked blocks */
	else
	{
	    /* get the info for the dataset (description record) */
            data_len = HPread_drec(file_id, data_id, &drec_buf);
	    if (data_len <= 0)
		HGOTO_ERROR(DFE_READERROR, FAIL);

	    /* get special tag */
	    p = drec_buf;
	    INT16DECODE(p, sp_tag);

	    /* if this is a compressed element, then it should only be none or
	       one block of data */
	    if (sp_tag == SPECIAL_COMP)
	    {
		/* skip 2byte header_version */
		p = p + 2;
		INT32DECODE(p, length);	/* get _uncompressed_ data length */

		/* No data written */
		if (length == 0)
		{
		    ret_value = 0;
		}
		/* Data has been written, get its offset and length */
		else
		{
		    /* Only one data block for compressed data, starting offset
			cannot be > 1 */
		    if (start_block > 1)
			HGOTO_ERROR(DFE_ARGS, FAIL);

		    /* get offset and length of the compressed element */
		    UINT16DECODE(p, comp_ref);
		    if ((offset = Hoffset(file_id, DFTAG_COMPRESSED, comp_ref)) == FAIL)
			HGOTO_ERROR(DFE_BADLEN, FAIL);

		    if ((length = Hlength(file_id, DFTAG_COMPRESSED, comp_ref)) == FAIL)
			HGOTO_ERROR(DFE_BADLEN, FAIL);

		    offsetarray[0] = offset;
		    lengtharray[0] = length;
		    ret_value = 1;
		} /* compressed data written */
	    } /* element is compressed */

	    /* if it is a chunked element, hand the task over to the chunking
		layer. */
	    else if (sp_tag == SPECIAL_CHUNKED)
	    {
	     if (HMCgetdatainfo(file_id, p, start_block, info_count, offsetarray, lengtharray)==FAIL)
		  HGOTO_ERROR(DFE_INTERNAL, FAIL);
	    }

	    /* unlimited dimension; extract the number of blocks and the ref #
		of the link table then hand over to linked block layer */
	    else if (sp_tag == SPECIAL_LINKED)
	    {
		int32 num_blocks;
		uint16 link_ref;

		p = p + 4 + 4; /* skip length, block_size */
		INT32DECODE(p, num_blocks);
		UINT16DECODE(p, link_ref);

		/* get data information from the linked blocks */
		info_count = HLgetdatainfo(file_id, link_ref, num_blocks,
						offsetarray, lengtharray);

	    } /* element is SPECIAL_LINKED */
	} /* else, data_id is special */

	/* end access to the element */
	if (HTPendaccess(data_id) == FAIL)
	    HGOTO_ERROR(DFE_CANTENDACCESS, FAIL);
    }  /* end if data_id != FAIL */

    else /* HTPselect failed */
        HGOTO_ERROR(DFE_CANTACCESS, FAIL);
done:
    if(ret_value == FAIL)   
    { /* Error condition cleanup */
    } /* end if */

    /* Normal function cleanup */
    if (drec_buf != NULL) 
	HDfree(drec_buf);

    return ret_value;
} /* HDgetdatainfo */
