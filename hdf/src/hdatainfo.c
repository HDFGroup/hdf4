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

LOW-LEVEL ROUTINES
    - Gets the number of data blocks in an element.
HDgetdatainfo_count(file_id, data_tag, data_ref)

    - Gets the offset(s) and length(s) of the data in the data element.
HDgetdatainfo(file_id, data_tag, data_ref, start_block, info_count, *offsetarray, *lengtharray)

EXPORTED ROUTINES
    - Gets the offset(s)/length(s) of a vdata's data.
VSgetdatainfo(vsid, start_block, info_count, *offsetarray, *lengtharray)

    - Gets the offset(s)/length(s) of the data of an image
GRgetdatainfo(riid, start_block, info_count, *offsetarray, *lengtharray)

    - Gets the offset/length of the data of a vdata's attribute
VSgetattdatainfo(vsid, findex, attrindex, *offset, *length)

    - Gets the offset/length of the data of a vgroup's attribute
Vgetattdatainfo(vgid, attrindex, *offset, *length)

    - Gets the offset/length of the data of an image's attribute
GRgetattdatainfo(id, attrindex, *attrname, *offset, *length)
*/

#ifndef MFGR_MASTER
#define MFGR_MASTER	/* for GRgetdatainfo and GRgetattdatainfo */
#endif			/* mfgr.h is included in hdf.h */

#ifndef MFAN_MASTER
#define MFAN_MASTER	/* for ANgetdatainfo */
#endif			/* mfan.h is included here */

#include "hdf.h"
#include "hlimits.h"
#include "vgint.h"
#include "mfan.h"

#define DATAINFO_MASTER
#include "hdatainfo.h"

#ifdef H4_HAVE_LIBSZ          /* we have the library */
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
 NOTES
    Aug 17, 2010: Tested with SDgetdatainfo and VSgetdatainfo -BMR
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
    HDgetdatainfo -- Gets the offset(s) and length(s) of the data in
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
 NOTES
    Aug 17, 2010: Tested with SDgetdatainfo and VSgetdatainfo -BMR
    Sep 7, 2010: Tested with GRgetdatainfo, but not linked-block yet -BMR
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
    uintn	count=0;
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
	    uint16 find_tag = 0;
	    uint16 find_ref = 0;

	    if (Hfind(file_id, data_tag, data_ref, &find_tag, &find_ref, &offset, &length, DF_FORWARD) == FAIL)
	    {
		HGOTO_ERROR(DFE_NOMATCH, FAIL);
	    }

	    /* Only one data block here, starting offset cannot be > 1 */
	    if (start_block > 1)
		HGOTO_ERROR(DFE_ARGS, FAIL);

	    offsetarray[0] = offset;
	    lengtharray[0] = length;
	    count = 1;
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
		    count = 0;
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
		    count = 1;
		} /* compressed data written */
	    } /* element is compressed */

	    /* if it is a chunked element, hand the task over to the chunking
		layer. */
	    else if (sp_tag == SPECIAL_CHUNKED)
	    {
	     if (count=HMCgetdatainfo(file_id, p, start_block, info_count, offsetarray, lengtharray)==FAIL)
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
		count = HLgetdatainfo(file_id, link_ref, num_blocks,
						offsetarray, lengtharray);

	    } /* element is SPECIAL_LINKED */
	} /* else, data_id is special */

	/* end access to the element */
	if (HTPendaccess(data_id) == FAIL)
	    HGOTO_ERROR(DFE_CANTENDACCESS, FAIL);
    }  /* end if data_id != FAIL */

    else /* HTPselect failed */
        HGOTO_ERROR(DFE_CANTACCESS, FAIL);

    ret_value = count;
done:
    if(ret_value == FAIL)   
    { /* Error condition cleanup */
    } /* end if */

    /* Normal function cleanup */
    if (drec_buf != NULL) 
	HDfree(drec_buf);

    return ret_value;
} /* HDgetdatainfo */


/*------------------------------------------------------ 
NAME
   VSgetdatainfo - Gets the offset/length of a vdata's data

DESCRIPTION

RETURNS
   SUCCEED/FAIL
TODO
    - not tested with start_block and info_count
    - additional comments/header
 NOTES
    Aug 17, 2010: Tested some in hdf/test/tdatainfo.c -BMR
----------------------------------------------------------*/
intn 
VSgetdatainfo(int32 vsid,	/* IN: vdata key */
	uintn start_block,	/* IN: indicating where to start */
	uintn info_count,	/* IN: number of blocks to be retrieved */
	int32 *offsetarray,	/* OUT: buffer for offset(s) */
	int32 *lengtharray)	/* OUT: buffer for length(s) */
{
    CONSTR(FUNC, "VSgetdatainfo");
    vsinstance_t *vs_inst = NULL;
    VDATA        *vs = NULL;
    accrec_t     *access_rec;
    intn	  count;
    intn          ret_value = SUCCEED;

    /* check key is valid vdata */
    if (HAatom_group(vsid) != VSIDGROUP)
        HGOTO_ERROR(DFE_ARGS, FAIL);

    /* get vdata instance */
    if (NULL == (vs_inst = (vsinstance_t *) HAatom_object(vsid)))
        HGOTO_ERROR(DFE_NOVS, FAIL);

    /* get vdata itself and check it */
    vs = vs_inst->vs;
    if (vs == NULL)
        HGOTO_ERROR(DFE_BADPTR, FAIL);

    access_rec = HAatom_object(vs->aid);
    if (access_rec == (accrec_t *) NULL)
	HGOTO_ERROR(DFE_ARGS, FAIL);

    /* If the vdata is empty, return 0 block */
    if (vs->nvertices <= 0)
	HGOTO_DONE(0);

    if (access_rec->special == SPECIAL_LINKED)
    {
	if (offsetarray == NULL && lengtharray == NULL)
	{
	    count = HDgetdatainfo_count(vs->f, VSDATATAG, vs->oref);
	}
	else
	{
	    count = HDgetdatainfo(vs->f, VSDATATAG, vs->oref, start_block, info_count, offsetarray, lengtharray);
	}
	if (count == FAIL)
	    HGOTO_ERROR(DFE_ARGS, FAIL);
    }
    else
    {
	if (offsetarray != NULL)
	    if ((*offsetarray = Hoffset(vs->f, VSDATATAG, vs->oref)) == FAIL)
		HGOTO_ERROR(DFE_BADOFFSET, FAIL);
	if (lengtharray != NULL)
	    if ((*lengtharray = Hlength(vs->f, VSDATATAG, vs->oref)) == FAIL)
		HGOTO_ERROR(DFE_BADLEN, FAIL);
	count = 1;
    }
    ret_value = count;

done:
  if(ret_value == FAIL)   
    { /* Error condition cleanup */
    } /* end if */
  /* Normal function cleanup */
  return ret_value;
}   /* VSgetdatainfo */

/*------------------------------------------------------ 
NAME
   Vgetattdatainfo - Gets the offset/length of the data
		      of a vgroup's attribute

DESCRIPTION

RETURNS
   SUCCEED/FAIL
TODO
    - additional tests for types other than char, maybe
    - not throughly documented
----------------------------------------------------------*/
intn 
Vgetattdatainfo(int32 vgid,	/* IN: vdata key */
	intn attrindex,		/* IN: attribute index */
	int32 *offset,		/* OUT: buffer for offset */
	int32 *length)		/* OUT: buffer for length */
{
    CONSTR(FUNC, "Vgetattdatainfo");
    VGROUP *vg;
    vg_attr_t *vg_alist;
    vginstance_t *vg_inst;
    int32 attr_vsid, nattrs;
    intn attr_index, found;
    intn status;
    intn ret_value = SUCCEED;

    HEclear();
    if (HAatom_group(vgid) != VGIDGROUP)
       HGOTO_ERROR(DFE_ARGS, FAIL);
    /* Locate vg's index in vgtab */
    if (NULL == (vg_inst = (vginstance_t *)HAatom_object(vgid)))
       HGOTO_ERROR(DFE_VTAB, FAIL);
    if (NULL == (vg = vg_inst->vg))
        HGOTO_ERROR(DFE_NOVS, FAIL);

    /* Validate arguments */
    nattrs = vg->nattrs;
    if (attrindex <0 || attrindex >= nattrs)
        /* not that many attrs or bad attr list */
        HGOTO_ERROR(DFE_ARGS, FAIL);

     vg_alist = vg->alist;
     if (nattrs == 0 || vg_alist == NULL)
          /* no attrs or bad attr list */
            HGOTO_ERROR(DFE_ARGS, FAIL);

    /* Search for the attribute index given by caller */
    found = 0;
    for (attr_index=0; attr_index<nattrs && found==0; attr_index++)
    {
	if (attr_index == attrindex)
	    found = 1;
	if (!found) vg_alist++;
    }
    /* If this happened, it would have been detected by the check for range
       of attrindex above already, but check it anyway */
    if (!found)
        HGOTO_ERROR(DFE_ARGS, FAIL);

    /* Attribute is found.  Get access to the vdata that stores the attribute's
       data, retrieve the offset and length of the data, then close access. */

    /* Get vdata */
    if (FAIL == (attr_vsid = VSattach(vg->f, (int32)vg_alist->aref, "r")))
        HGOTO_ERROR(DFE_CANTATTACH, FAIL);

    /* Get offset and length of attribute's data.  Note that start_block is 0
       and info_count is 1 because attribute's data is only stored in 1 block */
    status = VSgetdatainfo(attr_vsid, 0, 1, offset, length);
    if (status == FAIL)
        HGOTO_ERROR(DFE_GENAPP, FAIL);

    /* Close vdata */
    if (FAIL == VSdetach(attr_vsid))
        HGOTO_ERROR(DFE_CANTDETACH, FAIL);
done:
  if(ret_value == FAIL)   
    { /* Error condition cleanup */

    } /* end if */

  /* Normal function cleanup */
  return ret_value;
}   /* Vgetattdatainfo */

/*------------------------------------------------------ 
NAME
    VSgetattdatainfo - Gets the offset/length of the data
		      of a vdata's attribute

DESCRIPTION
    VSgetattdatainfo retrieves the offset and length of the data that belongs
    to an attribute.  If findex is _HDF_VDATA (or -1), then the attribute is
    associated with the vdata vsid.  If findex is an index of the vdata field,
    then the attribute is one that is associated with the vdata field.  The
    parameter attrindex specifies the attribute's index within the vdata's
    or the field's attribute list.  Thus, its valid value must be within
    [0-nattrs of the associated list].

    VSgetattdatainfo uses VSgetdatainfo once it locates the vdata that stores
    the attribute.

RETURNS
   SUCCEED/FAIL
TODO
    - additional tests for types other than char, maybe
    - not throughly documented
----------------------------------------------------------*/
intn 
VSgetattdatainfo(int32 vsid,	/* IN: vdata key */
	int32 findex, intn attrindex,
	int32 *offset,	/* OUT: buffer for offset */
	int32 *length)	/* OUT: buffer for length */
{
    CONSTR(FUNC, "VSgetattdatainfo");
    VDATA *vs;
    vs_attr_t *vs_alist;
    vsinstance_t *vs_inst;
    int32 attr_vsid;
    intn nattrs, attr_index, found;
    intn status;
    intn ret_value = SUCCEED;

    HEclear();

    if (HAatom_group(vsid) != VSIDGROUP)
        HGOTO_ERROR(DFE_ARGS, FAIL);
    /* locate vs' index in vstab */
    if (NULL == (vs_inst = (vsinstance_t *)HAatom_object(vsid)))
        HGOTO_ERROR(DFE_NOVS, FAIL);
    if (NULL == (vs = vs_inst->vs))
        HGOTO_ERROR(DFE_NOVS, FAIL);
    if ((findex >= vs->wlist.n || findex < 0) && (findex != _HDF_VDATA))
        HGOTO_ERROR(DFE_BADFIELDS, FAIL);
    nattrs = vs->nattrs;
    if (attrindex <0 || attrindex >= nattrs)
        HGOTO_ERROR(DFE_ARGS, FAIL);
    vs_alist = vs->alist;
    if (nattrs == 0 || vs_alist == NULL)
          /* no attrs or bad attr list */
            HGOTO_ERROR(DFE_ARGS, FAIL);
    found = 0;
    for (attr_index=0; attr_index<nattrs && found==0; attr_index++)
    {
	if (vs_alist->findex == findex)
	{
	    if (attr_index == attrindex)
		found = 1;
        }
	if (!found) vs_alist++;
    }
    /* If this happened, it would have been detected by the check for range
       of attrindex above already, but check it anyway */
    if (!found)
        HGOTO_ERROR(DFE_ARGS, FAIL);

    /* Attribute is found.  Get access to the vdata that stores the attribute's
       data, retrieve the offset and length of the data, then close access. */

    /* Get vdata */
    if (FAIL == (attr_vsid = VSattach(vs->f, (int32)vs_alist->aref, "r")))
        HGOTO_ERROR(DFE_CANTATTACH, FAIL);

    /* Get offset and length of attribute's data.  Note that start_block is 0
       and info_count is 1 because attribute's data is only stored in 1 block */
    status = VSgetdatainfo(attr_vsid, 0, 1, offset, length);
    if (status == FAIL)
        HGOTO_ERROR(DFE_GENAPP, FAIL);

    /* Close vdata */
    if (FAIL == VSdetach(attr_vsid))
        HGOTO_ERROR(DFE_CANTDETACH, FAIL);

done:
  if(ret_value == FAIL)   
    { /* Error condition cleanup */

    } /* end if */

  /* Normal function cleanup */
  return ret_value;
}   /* VSgetattdatainfo */


/*------------------------------------------------------------ 
NAME
   GRgetattdatainfo - Gets the offset/length of the data of a
			GR file's or an image's attribute

DESCRIPTION

RETURNS
   SUCCEED/FAIL
TODO
    - not tested
    - not documented
--------------------------------------------------------------*/
intn 
GRgetattdatainfo(int32 id,	/* IN: either GR ID or RI ID */
	intn attrindex,		/* IN: attribute index */
	char *attrname,		/* IN: attribute name */
	int32 *offset,		/* OUT: buffer for attribute's data's offset */
	int32 *length)		/* OUT: buffer for attribute's data's length */
{
    CONSTR(FUNC, "GRgetattdatainfo");
    gr_info_t *gr_ptr;          /* ptr to the GR information for this grid */
    ri_info_t *ri_ptr;          /* ptr to the image to work with */
    int32      hdf_file_id;	/* file id */
    int32      attr_vsid;	/* id of vdata that stores the attribute */
    void     **aentry;		/* temp. ptr to the image found */
    int        found = FALSE;	/* TRUE when the searched attribute is found */
    TBBT_TREE *search_tree;	/* attribute tree to search through */
    at_info_t *at_ptr=NULL;	/* ptr to the attribute to work with */
    intn       status; 
    intn       ret_value = SUCCEED;

    /* clear error stack and check validity of args */
    HEclear();

    /* check the validity of the ID, the index is checked below */
    if (HAatom_group(id) != RIIDGROUP && HAatom_group(id) != GRIDGROUP)
        HGOTO_ERROR(DFE_ARGS, FAIL);
    
    if (HAatom_group(id) == GRIDGROUP)
      {
          /* locate GR's object in hash table */
          if (NULL == (gr_ptr = (gr_info_t *) HAatom_object(id)))
              HGOTO_ERROR(DFE_GRNOTFOUND, FAIL);

          if(attrindex < 0 || attrindex >= gr_ptr->gattr_count)
              HGOTO_ERROR(DFE_ARGS, FAIL);

          search_tree = gr_ptr->gattree;
	  hdf_file_id = gr_ptr->hdf_file_id;
      } /* end if */
    else if (HAatom_group(id) == RIIDGROUP)
      {
          /* locate RI's object in hash table */
          if (NULL == (ri_ptr = (ri_info_t *) HAatom_object(id)))
              HGOTO_ERROR(DFE_RINOTFOUND, FAIL);

          if(attrindex < 0 || attrindex >= ri_ptr->lattr_count)
              HGOTO_ERROR(DFE_ARGS, FAIL); 
          search_tree = ri_ptr->lattree;
	  hdf_file_id = ri_ptr->gr_ptr->hdf_file_id;
      } /* end if */
    else
        HGOTO_ERROR(DFE_ARGS, FAIL);

    /* Search for an attribute with the same name */
    aentry = (void **)tbbtfirst((TBBT_NODE *)*search_tree);
    found = FALSE;
    while (!found && (aentry != NULL))
    {
	at_ptr = (at_info_t *)*aentry;
	/* If index is found */
	if ((at_ptr != NULL) && (at_ptr->index == attrindex))
	    found = TRUE;
	else
	    aentry = (void **)tbbtnext((TBBT_NODE *)aentry);
    } /* end while */

    attr_vsid = VSattach(hdf_file_id, (int32)at_ptr->ref, "r");
    if (attr_vsid == FAIL)
	HGOTO_ERROR(DFE_CANTATTACH, FAIL);

    /* Get offset and length of attribute's data.  Note that start_block is 0
       and info_count is 1 because attribute's data is only stored in 1 block */
    status = VSgetdatainfo(attr_vsid, 0, 1, offset, length);
 fprintf(stderr, "offset = %d, length = %d \n", *offset, *length);
    if (status == FAIL)
        HGOTO_ERROR(DFE_GENAPP, FAIL);

    if (FAIL == VSdetach(attr_vsid))
        HGOTO_ERROR(DFE_CANTDETACH, FAIL);

done:
  if(ret_value == 0)   
    { /* Error condition cleanup */

    } /* end if */

  /* Normal function cleanup */
  return ret_value;
}   /* GRgetattdatainfo */


/*--------------------------------------------------------------- 
NAME
   GRgetdatainfo - Gets the offsets/lengths of the data of an image

DESCRIPTION

RETURNS
   SUCCEED/FAIL
TODO
    - not tested with linked-block element yet
    - not documented
----------------------------------------------------------------*/
intn 
GRgetdatainfo(int32 riid,	/* IN: raster image ID */
	uintn start_block,	/* IN: start retrieving data at */
	uintn info_count,	/* IN: number of data blocks to retrieve */
	int32 *offsetarray,	/* OUT: buffer for offset(s) */
	int32 *lengtharray)	/* OUT: buffer for length(s) */
{
    CONSTR(FUNC, "GRgetdatainfo");
    ri_info_t *ri_ptr;          /* ptr to the image to work with */
    int32 hdf_file_id;		/* short cut for file id */
    int32 length = 0;
    uintn count;
    intn   ret_value = SUCCEED;

    /* clear error stack and check validity of args */
    HEclear();

    /* check the validity of the ID */
    if (HAatom_group(riid) != RIIDGROUP)
        HGOTO_ERROR(DFE_ARGS, FAIL);

    /* locate RI's object in hash table */
    ri_ptr = (ri_info_t *) HAatom_object(riid);
    if (NULL == ri_ptr)
	HGOTO_ERROR(DFE_RINOTFOUND, FAIL);

    hdf_file_id = ri_ptr->gr_ptr->hdf_file_id;	/* alias for file id */

    /* Check for no data in the image */

    /* If the image has no tag/ref pair assigned to it yet, return 0 for
       info count */
    if(ri_ptr->img_tag==DFTAG_NULL || ri_ptr->img_tag==DFREF_WILDCARD
	|| ri_ptr->img_ref==DFREF_WILDCARD)
    {
	HGOTO_DONE(0);
    }
    /* If the image already had a tag/ref pair, make sure it has actual data,
       if not, return 0 for info count */
    else
    {
	length = Hlength(hdf_file_id, ri_ptr->img_tag, ri_ptr->img_ref);
	if (length <= 0)
	    HGOTO_DONE(0);

        /* if both arrays are NULL, get the number of data blocks and return */
        if ((offsetarray == NULL && lengtharray == NULL) && (info_count == 0))
        {
            count = HDgetdatainfo_count(hdf_file_id, ri_ptr->img_tag, ri_ptr->img_ref);
            if (count == FAIL)
                HGOTO_ERROR(DFE_INTERNAL, FAIL);
        }

        /* just in case user forgets to allocate space for arrays */
        else if ((offsetarray == NULL && lengtharray == NULL) && (info_count > 0))
        {
            HGOTO_ERROR(DFE_NOTENOUGH, FAIL);
        }

        /* application requests actual offsets/lengths */
        else
        {
            count = HDgetdatainfo(hdf_file_id, ri_ptr->img_tag, ri_ptr->img_ref, start_block, info_count, offsetarray, lengtharray);
            if (count == FAIL)
                HGOTO_ERROR(DFE_INTERNAL, FAIL);
        }
    } /* end else */

    ret_value = count;

done:
  if(ret_value == 0)   
    { /* Error condition cleanup */
    } /* end if */
  /* Normal function cleanup */
  return ret_value;
}   /* GRgetdatainfo */


/*--------------------------------------------------------------------------
 NAME
    ANgetdatainfo -- Gets the offset(s) and length(s) locating the data of
		      the annotation.
 USAGE
    int32 ANgetdatainfo(ann_id, *offset, *length)	
	int32  ann_id;	IN: annotation ID
	int32 *offset;	OUT: buffer for offset
	int32 *length;	OUT: buffer for length
 RETURNS
    SUCCEED/FAIL
 DESCRIPTION
    
 NOTES
    Aug 25, 2010: Tested in tdatainfo.c/test_annotation -BMR
--------------------------------------------------------------------------*/
intn
ANgetdatainfo(int32 ann_id,    /* IN: annotation id */
	int32 *offset,	/* OUT: buffer for offset */
	int32 *length)	/* OUT: buffer for length */
{
    CONSTR(FUNC, "ANgetdatainfo");
    filerec_t  *file_rec = NULL;		/* file record pointer */
    ANnode     *ann_node   = NULL;
    int32       file_id = FAIL;
    int32       type;
    int32       ann_key;
    int         newflag = 0;
    uint16      ann_tag;
    uint16      ann_ref;
    intn        ret_value = SUCCEED;

    /* Clear error stack */
    HEclear();

    /* Get annotation record */
    ann_node = HAatom_object(ann_id);
    if (NULL == ann_node)
        HGOTO_ERROR(DFE_ARGS, FAIL);

    /* Convert file_id to file rec and check for validity */
    file_id = ann_node->file_id;
    file_rec = HAatom_object(file_id);
    if (BADFREC(file_rec))
        HGOTO_ERROR(DFE_INTERNAL, FAIL);

    /* Get annotation key, type, and ref# */
    ann_key = ann_node->ann_key;
    type    = AN_KEY2TYPE(ann_key);
    ann_ref = AN_KEY2REF(ann_key);

    /* Set type tag */
    switch((int32)type)
      {
      case AN_DATA_LABEL:
          ann_tag = DFTAG_DIL;
          break;
      case AN_DATA_DESC:
          ann_tag = DFTAG_DIA;
          break;
      case AN_FILE_LABEL:
          ann_tag = DFTAG_FID;
          break;
      case AN_FILE_DESC:
          ann_tag = DFTAG_FD;
          break;
      default:
          HE_REPORT_GOTO("Bad annotation type for this call",FAIL);
      }

#ifdef NEED_ELEMENT_TAG_REF
/* Keep these here just in case we end up need the object's tag/ref -BMR */
    /* Get annotation entry so that we can get object's tag/ref later */
    if ((entry = tbbtdfind(file_rec->an_tree[type], &ann_key, NULL)) == NULL)
	HE_REPORT_GOTO("failed to retrieve annotation of 'type' tree", FAIL);

    ann_entry = (ANentry *) entry->data;

    elem_tag = ann_entry->elmtag;
    elem_ref = ann_entry->elmref;
#endif

    /* If annotation exists, try to get offset/length */
    newflag  = ann_node->new_ann;
    if (newflag == 0)
    {
	int32 off=0, len=0;

	off = Hoffset(file_id, ann_tag, ann_ref);
	if (off == FAIL)
            HGOTO_ERROR(DFE_INTERNAL, FAIL)
	else
	{
	    if (offset != NULL)
	    *offset = off;
	}

	len = Hlength(file_id, ann_tag, ann_ref);
	if (len == FAIL)
            HGOTO_ERROR(DFE_INTERNAL, FAIL)
	else
	{
	    if (length != NULL)
	    *length = len;
	}

	/* Because for Data label/description, the object's tag/ref were
	written to the file before the annotation data, 4 bytes must be
	taken into account for them */
	if (ann_tag == DFTAG_DIL || ann_tag == DFTAG_DIA)
	{
	    *offset = *offset + 4;
	    *length = *length - 4;
	}
    }

  done:
    if(ret_value == FAIL)   
      { /* Error condition cleanup */
      } /* end if */
    /* Normal function cleanup */
    return ret_value;
} /* ANgetdatainfo */
