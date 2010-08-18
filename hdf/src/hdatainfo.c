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

#define MFGR_MASTER
#include "hdf.h"
#include "hlimits.h"
#include "vgint.h"

#ifdef H4_HAVE_LIBSZ          /* we have the library */
#include "szlib.h"
#endif

/*
#define MFGR_MASTER
#ifdef H4_HAVE_LIBSZ
#include "szlib.h"
#endif

#include "atom.h"
#include "mfgr.h"
*/

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
    uint16	drec_tag, drec_ref;	/* description record tag/ref */
    int32	drec_aid;	/* description record access id */
    atom_t      data_id = FAIL;	/* dd ID of existing regular element */
    uint8      *drec_buf=NULL, *p;
    filerec_t  *file_rec;	/* file record */
    uintn	info_count = 0;
    int32	offset, length; /* offset and length of a data block */
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
 NOTES
    Aug 17, 2010: Tested with SDgetdatainfo and VSgetdatainfo -BMR
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
    uint16	drec_tag, drec_ref;	/* description record tag/ref */
    uint8      *drec_buf=NULL, *p;	/* description record buffer */
    int32	drec_aid;	/* description record access id */
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
	    if ((offset = Hoffset(file_id, data_tag, data_ref)) == FAIL)
		HGOTO_ERROR(DFE_BADOFFSET, FAIL);
	    if ((length = Hlength(file_id, data_tag, data_ref)) == FAIL)
		HGOTO_ERROR(DFE_BADLEN, FAIL);

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
   VSgetdatainfo - gets the offset/length of a vdata's data

DESCRIPTION

RETURNS
   SUCCEED/FAIL
TODO
    - start_block and n_blocks
    - additional comments/header
 NOTES
    Aug 17, 2010: Tested in hdf/test/tdatainfo.c -BMR
----------------------------------------------------------*/
intn 
VSgetdatainfo(int32 vsid,	/* IN: vdata key */
	uintn start_block,	/* IN: indicating where to start */
	uintn n_blocks,		/* IN: number of blocks to be retrieved */
	int32 *offsetarray,	/* OUT: buffer for offset(s) */
	int32 *lengtharray)	/* OUT: buffer for length(s) */
{
    CONSTR(FUNC, "VSgetdatainfo");
    vsinstance_t *w = NULL;
    VDATA        *vs = NULL;
    accrec_t     *access_rec;
    intn	  count;
    intn          ret_value = SUCCEED;

    /* check key is valid vdata */
    if (HAatom_group(vsid) != VSIDGROUP)
        HGOTO_ERROR(DFE_ARGS, FAIL);

    /* get vdata instance */
    if (NULL == (w = (vsinstance_t *) HAatom_object(vsid)))
        HGOTO_ERROR(DFE_NOVS, FAIL);

    /* get vdata itself and check it */
    vs = w->vs;
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
	    count = HDgetdatainfo(vs->f, VSDATATAG, vs->oref, start_block, n_blocks, offsetarray, lengtharray);
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
   Vgetattdatainfo - gets the offset/length of the data
		      of a vgroup's attribute

DESCRIPTION

RETURNS
   SUCCEED/FAIL
TODO
    - not tested
    - not documented
----------------------------------------------------------*/
intn 
Vgetattdatainfo(int32 vgid,	/* IN: vdata key */
	intn attrindex,
	char *attrname,		/* IN: attribute name */
	int32 *offsetarray,	/* OUT: buffer for offset(s) */
	int32 *lengtharray)	/* OUT: buffer for length(s) */
{
    CONSTR(FUNC, "Vgetattdatainfo");
    VGROUP *vg, *attr_vg;
    vg_attr_t *vg_alist;
    vginstance_t *vg_inst, *attr_inst;
    int32 attr_vgid, nattrs;
    intn i, a_index, found;
    int32 offset, length;
    intn status;
    char *fldname;
    intn ret_value = SUCCEED;

    HEclear();
    if (HAatom_group(vgid) != VGIDGROUP)
       HGOTO_ERROR(DFE_ARGS, FAIL);
    /* locate vg's index in vgtab */
    if (NULL == (vg_inst = (vginstance_t *)HAatom_object(vgid)))
       HGOTO_ERROR(DFE_VTAB, FAIL);
    if (NULL == (vg = vg_inst->vg))
        HGOTO_ERROR(DFE_NOVS, FAIL);

    nattrs = vg->nattrs;
    if (nattrs <= attrindex || vg->alist == NULL)
        /* not that many attrs or bad attr list */
        HGOTO_ERROR(DFE_ARGS, FAIL);

     vg_alist = vg->alist;
     if (nattrs == 0 || vg_alist == NULL)
          /* no attrs or bad attr list */
            HGOTO_ERROR(DFE_ARGS, FAIL);

    found = 0;
    a_index = -1; 
    for (i=0; i<nattrs; i++)  {
           a_index++; 
           if (a_index == attrindex) {
              found = 1;
              break;
           }
        vg_alist++;
    }
    if (!found)
        HGOTO_ERROR(DFE_ARGS, FAIL);

    /* found. get attr info */
    if (FAIL == (attr_vgid = VSattach(vg->f, (int32)vg_alist->aref, "r")))
        HGOTO_ERROR(DFE_CANTATTACH, FAIL);

    status = VSgetdatainfo(attr_vgid, 0, 1, &offset, &length);
    if (status == FAIL)
        HGOTO_ERROR(DFE_GENAPP, FAIL);

    if (FAIL == VSdetach(attr_vgid))
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
   VSgetattdatainfo - gets the offset/length of the data
		      of a vdata's attribute

DESCRIPTION

RETURNS
   SUCCEED/FAIL
TODO
    - not tested
    - not documented
----------------------------------------------------------*/
intn 
VSgetattdatainfo(int32 vsid,	/* IN: vdata key */
	int32 findex, intn attrindex,
	char *attrname,		/* IN: attribute name */
	int32 *offsetarray,	/* OUT: buffer for offset(s) */
	int32 *lengtharray)	/* OUT: buffer for length(s) */
{
    CONSTR(FUNC, "VSgetattdatainfo");
    intn          ret_value = SUCCEED;


     VDATA *vs, *attr_vs;
     vs_attr_t *vs_alist;
     vsinstance_t *vs_inst, *attr_inst;
     int32 attr_vsid;
     intn i, nattrs, a_index, found;
    int32 offset, length;
    intn status;
     DYN_VWRITELIST *w;
     char *fldname;

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
    a_index = -1; 
    for (i=0; i<nattrs; i++)  {
        if (vs_alist->findex == findex)  {
           a_index++; 
           if (a_index == attrindex) {
              found = 1;
              break;
           }
        }
        vs_alist++;
    }
    if (!found)
        HGOTO_ERROR(DFE_ARGS, FAIL);
    /* found. get attr info */
    if (FAIL == (attr_vsid = VSattach(vs->f, (int32)vs_alist->aref, "r")))
        HGOTO_ERROR(DFE_CANTATTACH, FAIL);

    status = VSgetdatainfo(attr_vsid, 0, 1, &offset, &length);
    if (status == FAIL)
        HGOTO_ERROR(DFE_GENAPP, FAIL);

    if (FAIL == VSdetach(attr_vsid))
        HGOTO_ERROR(DFE_CANTDETACH, FAIL);

done:
  if(ret_value == FAIL)   
    { /* Error condition cleanup */

    } /* end if */

  /* Normal function cleanup */
  return ret_value;
}   /* VSgetattdatainfo */


/*------------------------------------------------------ 
NAME
   GRgetattdatainfo - gets the offset/length of the data
		      of a vgroup's attribute

DESCRIPTION

RETURNS
   SUCCEED/FAIL
TODO
    - not tested
    - not documented
----------------------------------------------------------*/
intn 
GRgetattdatainfo(int32 id,	/* IN: either GR ID or RI ID */
	intn attrindex,
	char *attrname,		/* IN: attribute name */
	int32 *offset,	/* OUT: buffer for attribute's data's offset */
	int32 *length)	/* OUT: buffer for attribute's data's length */
{
    CONSTR(FUNC, "GRgetattdatainfo");
    gr_info_t *gr_ptr;          /* ptr to the GR information for this grid */
    ri_info_t *ri_ptr;          /* ptr to the image to work with */
    int32 hdf_file_id;
    int32 attr_vgid;
    void * *t;                   /* temp. ptr to the image found */
    int found = FALSE;
    TBBT_TREE *search_tree;     /* attribute tree to search through */
    at_info_t *at_ptr;          /* ptr to the attribute to work with */
    intn status; 
    intn   ret_value = SUCCEED;

    /* clear error stack and check validity of args */
    HEclear();

    /* check the validity of the ID, the index is checked below */
    if (HAatom_group(id) != RIIDGROUP && HAatom_group(id) != GRIDGROUP)
        HGOTO_ERROR(DFE_ARGS, FAIL);
    
    if (HAatom_group(id) == GRIDGROUP)
      {
          /* locate GR's object in hash table */
          if (NULL == (gr_ptr = (gr_info_t *) HAatom_object(id)))
              HGOTO_ERROR(DFE_NOVS, FAIL);

          if(attrindex < 0 || attrindex >= gr_ptr->gattr_count)
              HGOTO_ERROR(DFE_ARGS, FAIL);

          search_tree = gr_ptr->gattree;
	  hdf_file_id = gr_ptr->hdf_file_id;
      } /* end if */
    else if (HAatom_group(id) == RIIDGROUP)
      {
          /* locate RI's object in hash table */
          if (NULL == (ri_ptr = (ri_info_t *) HAatom_object(id)))
              HGOTO_ERROR(DFE_NOVS, FAIL);

          if(attrindex < 0 || attrindex >= ri_ptr->lattr_count)
              HGOTO_ERROR(DFE_ARGS, FAIL); 
          search_tree = ri_ptr->lattree;
	  hdf_file_id = ri_ptr->gr_ptr->hdf_file_id;
      } /* end if */
    else
        HGOTO_ERROR(DFE_ARGS, FAIL);

    /* Search for an attribute with the same name */
    if((t = (void **)tbbtfirst((TBBT_NODE *)*search_tree)) != NULL)
      {
          do {
              at_ptr = (at_info_t *)*t;
              if (at_ptr != NULL && HDstrcmp(at_ptr->name, attrname) == 0)  /* ie. the name matches */
                {
                    found = TRUE;
                    break;
                } /* end if */
          } while ((t = (void **)tbbtnext((TBBT_NODE *)t)) != NULL);
      } /* end if */

    if ((attr_vgid = VSattach(hdf_file_id, (int32)at_ptr->ref, "w")) == FAIL)
                    HGOTO_ERROR(DFE_CANTATTACH, FAIL);

/* Can RI have linked block?  Need to verify */
    status = VSgetdatainfo(attr_vgid, 0, 1, offset, length);
    if (status == FAIL)
        HGOTO_ERROR(DFE_GENAPP, FAIL);

    if (FAIL == VSdetach(attr_vgid))
        HGOTO_ERROR(DFE_CANTDETACH, FAIL);

done:
  if(ret_value == 0)   
    { /* Error condition cleanup */

    } /* end if */

  /* Normal function cleanup */
  return ret_value;
}   /* GRgetattdatainfo */


/*------------------------------------------------------ 
NAME
   GRgetdatainfo - gets the offset/length of the data
		      of a vgroup's attribute

DESCRIPTION

RETURNS
   SUCCEED/FAIL
TODO
    - not tested
    - not documented
----------------------------------------------------------*/
intn 
GRgetdatainfo(int32 riid,	/* IN: raster image ID */
	uintn start_block,	/* IN: start retrieving data at */
	uintn n_blocks,		/* IN: number of data blocks to retrieve */
	int32 *offsetarray,	/* OUT: buffer for offset(s) */
	int32 *lengtharray)	/* OUT: buffer for length(s) */
{
    CONSTR(FUNC, "GRgetdatainfo");
    gr_info_t *gr_ptr;          /* ptr to the GR information for this grid */
    ri_info_t *ri_ptr;          /* ptr to the image to work with */
    at_info_t *at_ptr;          /* ptr to the attribute to work with */
    uint16 ref;
    intn   ret_value = SUCCEED;

    /* clear error stack and check validity of args */
    HEclear();

    /* check the validity of the ID, the index is checked below */
    if (HAatom_group(riid)==RIIDGROUP)
      {
          /* locate RI's object in hash table */
          ri_ptr = (ri_info_t *) HAatom_object(riid);
          if (NULL == ri_ptr)
              HGOTO_ERROR(DFE_NOVS, FAIL);

          ref = ri_ptr->ri_ref;
      } /* end if */

done:
  if(ret_value == 0)   
    { /* Error condition cleanup */

    } /* end if */

  /* Normal function cleanup */
  return ret_value;
}   /* GRgetdatainfo */
