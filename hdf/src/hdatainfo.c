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
    - Gets the offset(s) and length(s) of the data in the data element.
HDgetdatainfo(file_id, tag, ref, *chk_coord, start_block, info_count, *offsetarray, *lengtharray)

EXPORTED ROUTINES
	--- Gets offset(s)/length(s) of a vdata's data.
VSgetdatainfo(vsid, start_block, info_count, *offsetarray, *lengtharray)

	--- Gets offset(s)/length(s) of the data of an image
GRgetdatainfo(riid, start_block, info_count, *offsetarray, *lengtharray)

	--- Gets offset/length of the data of a vdata's attribute
VSgetattdatainfo(vsid, findex, attrindex, *offset, *length)

	--- Gets offset/length of the data of a vgroup's attribute
Vgetattdatainfo(vgid, attrindex, *offset, *length)

	--- Gets offset/length of the data of an image's attribute
GRgetattdatainfo(id, attrindex, *offset, *length)

	--- Gets offset/length of the data of an annotation
ANgetdatainfo(ann_id, *offset, *length)
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
    HDgetdatainfo -- Gets the offset(s) and length(s) of the data in
		      the data element.
 USAGE
    int32 HDgetdatainfo(file_id, tag, ref, start_block, info_count,
			 *offsetarray, *lengtharray)	
	int32  file_id;		IN: file id
	uint16 tag;		IN: tag of the element
	uint16 ref;		IN: ref of element
	int32 *chk_coord;	IN: coordinate array of the inquired chunk
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
    Oct 5, 2010: Modified to handle compressed/linked-block element -BMR
--------------------------------------------------------------------------*/
intn
HDgetdatainfo(int32 file_id,
	uint16 tag, uint16 ref, /* IN: tag/ref of element */
	int32 *chk_coord,
	uintn start_block,	/* IN: data block to start at, 0 base */
	uintn info_count,	/* IN: number of info records */
	int32 *offsetarray,	/* OUT: array to hold offsets */
	int32 *lengtharray)	/* OUT: array to hold lengths */
{
    CONSTR(FUNC, "HDgetdatainfo");	/* for HGOTO_ERROR */
    filerec_t  *file_rec;	/* file record */
    uint16	sp_tag;		/* special tag */
    uint16	comp_ref = 0;	/* ref for compressed data or compression header */
    uint16	dtag, dref;	/* description record tag/ref */
    int32	dlen=0, doff=0;	/* offset/length of the description record */
    uint8	lbuf[COMP_HEADER_LENGTH],
		*p=NULL;	/* desc record buffer and a pointer to it */
    atom_t	data_id = FAIL;	/* dd ID of existing element */
    int32	length; /* uncomp data len to check if data had been written */
    uintn	count=0;/* num of data blocks returned by getdatainfo funcs */
    uint16	spec_code=0;/* special code: SPECIAL_LINKED, SPECIAL_COMP,... */
    int32	comp_aid=-1;/* compressed element access id */
    intn	ret_value=SUCCEED;

    /* Clear error stack */
    HEclear();

    /* validate arguments */
    if (info_count == 0 && offsetarray != NULL && lengtharray != NULL)
        HGOTO_ERROR(DFE_ARGS, FAIL);

    /* convert file id to file rec and check for validity */
    file_rec = HAatom_object(file_id);
    if (BADFREC(file_rec))
	HGOTO_ERROR(DFE_ARGS, FAIL);

    /* get access element from dataset's tag/ref */
    if ((data_id=HTPselect(file_rec, tag, ref))!=FAIL)
    {
	/* get the info pointed to by this dd, which could point to data or
	   description record */
	if (HTPinquire(data_id, &dtag, &dref, &doff, &dlen) == FAIL)
            HGOTO_ERROR(DFE_INTERNAL, FAIL);

        /* return 0 if no data had been written */
        if (doff == INVALID_OFFSET && dlen == INVALID_LENGTH)
	{
	    /* end access to the element */
	    if (HTPendaccess(data_id) == FAIL)
	        HGOTO_ERROR(DFE_CANTENDACCESS, FAIL);

	    HGOTO_DONE(0);
	}

	/* if the element is not special, that means dataset's tag/ref 
	   specifies the actual data that was written to the dataset, get
	   the offset and length of the data if they were requested */
	if (HTPis_special(data_id)==FALSE)
        {
	    /* Only one data block here, starting offset cannot be > 1 */
	    if (start_block > 1)
		HGOTO_ERROR(DFE_ARGS, FAIL);

	    if (offsetarray != NULL && lengtharray != NULL)
	    {
		offsetarray[0] = doff;
		lengtharray[0] = dlen;
	    }
	    count = 1;
        }

	/* if the element is special, get the special info header and decode
	   for special tag to detect compression/chunking/linked blocks */
	else
	{
	    if (HPseek(file_rec, doff) == FAIL)
		HGOTO_ERROR(DFE_SEEKERROR, FAIL);
	    if (HP_read(file_rec, lbuf, (int)2) == FAIL)
		HGOTO_ERROR(DFE_READERROR, FAIL);

	    /* Decode the special tag */
	    p = &lbuf[0];
	    INT16DECODE(p, sp_tag);

	    /* if this is a compressed element, then it should only be none or
	       one block of data */
	    if (sp_tag == SPECIAL_COMP)
	    {
		if (HP_read(file_rec, lbuf, (int)COMP_HEADER_LENGTH) == FAIL)
		HGOTO_ERROR(DFE_READERROR, FAIL);

		p = &lbuf[0];
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
		    UINT16DECODE(p, comp_ref);

		if ((comp_aid = HTPselect(file_rec, DFTAG_COMPRESSED, comp_ref)) == FAIL)
		    HE_REPORT_GOTO("HTPselect failed ", FAIL);

		if (HTPis_special(comp_aid)!=TRUE)
		{
		    /* this element is not further special, only compressed,
		       get its offset and length */
		    if (offsetarray != NULL && lengtharray != NULL)
		    {
			int32 off=0, len=0;
			off = Hoffset(file_id, DFTAG_COMPRESSED, comp_ref);
			if (off == FAIL)
			    HGOTO_ERROR(DFE_BADOFFSET, FAIL);
			len = Hlength(file_id, DFTAG_COMPRESSED, comp_ref);
			if (len == FAIL)
			    HGOTO_ERROR(DFE_BADLEN, FAIL);
			offsetarray[0] = off;
			lengtharray[0] = len;
		    }
		    count = 1;
		}   /* end if */
		else
		{ /* this element is further special, read in the special code to see what
		     the specialness is then process appropriately */
		    if(HTPinquire(comp_aid, NULL, NULL, &doff, NULL)==FAIL)
		    {
			HTPendaccess(comp_aid);
			HGOTO_ERROR(DFE_INTERNAL, FAIL);
		    } /* end if */
		    if (HPseek(file_rec, doff) == FAIL)
			HGOTO_ERROR(DFE_SEEKERROR, FAIL);
		    if (HP_read(file_rec, lbuf, (int)2) == FAIL)
			HGOTO_ERROR(DFE_READERROR, FAIL);

		    /* use special code to determine how to retrieve off/len */
		    p = &lbuf[0];
		    INT16DECODE(p, spec_code);

		    if (spec_code == SPECIAL_LINKED)
		    {
			if (HP_read(file_rec, lbuf, (int)14) == FAIL)
			    HGOTO_ERROR(DFE_READERROR, FAIL);

			/* pass the special header info to the linked-block API
			   to get the data info if they are requested or the
			   info count only, otherwise */ 
			p = &lbuf[0];
			if (offsetarray != NULL && lengtharray != NULL)
			    count = HLgetdatainfo(file_id, p, start_block, info_count, offsetarray, lengtharray);
			else  /* get data information from the linked blocks */
			    count = HLgetdatainfo(file_id, p, start_block, 0, NULL, NULL);
		    } /* this element is also stored in linked blocks */
		} /* this element is further special */

		if(HTPendaccess(comp_aid)==FAIL)
		    HGOTO_ERROR(DFE_CANTENDACCESS, FAIL);

		} /* compressed data written */
	    } /* element is compressed */

	    /* if it is a chunked element, hand the task over to the chunking
		layer. */
	    else if (sp_tag == SPECIAL_CHUNKED)
	    {
		if (chk_coord != NULL)
		    count = HMCgetdatainfo(file_id, tag, ref, chk_coord,
			    start_block, info_count, offsetarray, lengtharray);
		else /* BMR: check to see what should be done here */
		{
/* commented this out so subsequent tests can be run - 1/2011 */
		    /*  fprintf(stderr, "\nERROR>>> Element with tag/ref %d/%d is a chunked element, the chunk's coordinates must be specified\n", tag, ref);
		    exit(0);
		or
		    HGOTO_ERROR(DFE_ARGS, FAIL);
 */ 
		}
	    }

	    /* unlimited dimension; extract the number of blocks and the ref #
		of the link table then hand over to linked block layer */
	    else if (sp_tag == SPECIAL_LINKED)
	    {
		if (HP_read(file_rec, lbuf, (int)14) == FAIL)
		    HGOTO_ERROR(DFE_READERROR, FAIL);

		/* pass the special header info to the linked-block API to get the data
		   info if they are requested or the info count only, otherwise */ 
		p = &lbuf[0];
		if (offsetarray != NULL && lengtharray != NULL)
		    count = HLgetdatainfo(file_id, p, start_block, info_count, offsetarray, lengtharray);
		else  /* get data information from the linked blocks */
		    count = HLgetdatainfo(file_id, p, start_block, 0, NULL, NULL);
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

    /* Clear error stack */
    HEclear();

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
	    count = HDgetdatainfo(vs->f, VSDATATAG, vs->oref, NULL, start_block, info_count, NULL, NULL);
	}
	else
	{
	    count = HDgetdatainfo(vs->f, VSDATATAG, vs->oref, NULL, start_block, info_count, offsetarray, lengtharray);
	}
	if (count == FAIL)
	    HGOTO_ERROR(DFE_ARGS, FAIL);
    }
    else
    {
	if (offsetarray != NULL && lengtharray != NULL)
	{
	    int32 off=0, len=0;
	    if ((off = Hoffset(vs->f, VSDATATAG, vs->oref)) == FAIL)
		HGOTO_ERROR(DFE_BADOFFSET, FAIL);
	    if ((len = Hlength(vs->f, VSDATATAG, vs->oref)) == FAIL)
		HGOTO_ERROR(DFE_BADLEN, FAIL);
	    *offsetarray = off;
	    *lengtharray = len;
	}
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
    int32 attr_vsid;
    intn status;
    intn ret_value = SUCCEED;

    /* Clear error stack */
    HEclear();

    /* Validate Vgroup ID */
    if (HAatom_group(vgid) != VGIDGROUP)
       HGOTO_ERROR(DFE_ARGS, FAIL);

    /* Locate vg's index in vgtab */
    if (NULL == (vg_inst = (vginstance_t *)HAatom_object(vgid)))
       HGOTO_ERROR(DFE_VTAB, FAIL);
    if (NULL == (vg = vg_inst->vg))
        HGOTO_ERROR(DFE_NOVS, FAIL);

    /* Validate arguments */

    if (attrindex < 0)
        HGOTO_ERROR(DFE_BADATTR, FAIL); /* invalid attribute index given */

    if (attrindex < vg->noldattrs) /* index of old-style attribute */
        vg_alist = vg->old_alist;  /* use old-attr list */
    else if (attrindex >= vg->noldattrs &&
             attrindex < vg->nattrs) /* index of new-style attributes */
        vg_alist = vg->alist;        /* use new-attr list */
    else /* not that many attrs */
        HGOTO_ERROR(DFE_BADATTR, FAIL);

    if (vg_alist == NULL)
        /* Bad attr list */
        HGOTO_ERROR(DFE_BADATTR, FAIL);

    /* Get vdata */
    if (FAIL == (attr_vsid = VSattach(vg->f, (int32)vg_alist[attrindex].aref, "r")))
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
    intn nattrs, idx, a_index, found;
    intn status;
    intn ret_value = SUCCEED;

    /* Clear error stack */
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

    /* no attrs or bad attr list */
    if (nattrs == 0 || vs_alist == NULL)
	HGOTO_ERROR(DFE_ARGS, FAIL);

    found = 0;
    a_index = -1;
    for (idx=0; idx<nattrs && found==0; idx++)
    {
	if (vs_alist->findex == findex)
	{
	    a_index++;
	    if (a_index == attrindex)
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
	char *attrname,		/* IN: attribute name if index = -1 */
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
    group_t id_group=BADGROUP;
    intn       status; 
    intn       ret_value = SUCCEED;

    /* Clear error stack */
    HEclear();

    /* Use index to search but if index is -1 then attr name must be non-NULL */
    if (attrindex < 0 && attrname == NULL)
        HGOTO_ERROR(DFE_ARGS, FAIL);

    /* check the validity of the ID, the index is checked below */
    id_group = HAatom_group(id);
    if (id_group != RIIDGROUP && id_group != GRIDGROUP)
        HGOTO_ERROR(DFE_ARGS, FAIL);
    
    if (id_group == GRIDGROUP)
      {
          /* locate GR's object in hash table */
          if (NULL == (gr_ptr = (gr_info_t *) HAatom_object(id)))
              HGOTO_ERROR(DFE_GRNOTFOUND, FAIL);

          if(attrindex >= gr_ptr->gattr_count)
              HGOTO_ERROR(DFE_ARGS, FAIL);

          search_tree = gr_ptr->gattree;
	  hdf_file_id = gr_ptr->hdf_file_id;
      } /* end if */
    else if (id_group == RIIDGROUP)
      {
          /* locate RI's object in hash table */
          if (NULL == (ri_ptr = (ri_info_t *) HAatom_object(id)))
              HGOTO_ERROR(DFE_RINOTFOUND, FAIL);

          if(attrindex >= ri_ptr->lattr_count)
              HGOTO_ERROR(DFE_ARGS, FAIL); 
          search_tree = ri_ptr->lattree;
	  hdf_file_id = ri_ptr->gr_ptr->hdf_file_id;
      } /* end if */
    else
        HGOTO_ERROR(DFE_ARGS, FAIL);

    /* Search for an attribute with the same index */
    aentry = (void **)tbbtfirst((TBBT_NODE *)*search_tree);
    found = FALSE;
    while (!found && (aentry != NULL))
    {
	at_ptr = (at_info_t *)*aentry;
	if (at_ptr == NULL)
	{
	    HGOTO_ERROR(DFE_ARGS, FAIL); 
	}

	/* Search by attribute index */
	if (attrindex >= 0)
	{
	    /* If index is found, set flag */
	    if (at_ptr->index == attrindex)
		found = TRUE;
	}
	/* Search by attribute name */
	else if (HDstrcmp(attrname, at_ptr->name) == 0)
	    found = TRUE;

	/* Not found, go to the next entry */
	if (!found)
	    aentry = (void **)tbbtnext((TBBT_NODE *)aentry);
    } /* end while */

    if (found)
    {
	/* Get access to the vdata that stores the attribute */
	attr_vsid = VSattach(hdf_file_id, (int32)at_ptr->ref, "r");
	if (attr_vsid == FAIL)
	    HGOTO_ERROR(DFE_CANTATTACH, FAIL);

	/* Get offset and length of attribute's data.  Note that start_block
	   is 0 and info_count is 1 because attribute's data is only stored
	   in 1 block */
	status = VSgetdatainfo(attr_vsid, 0, 1, offset, length);
	if (status == FAIL)
	    HGOTO_ERROR(DFE_GENAPP, FAIL);

	if (FAIL == VSdetach(attr_vsid))
	    HGOTO_ERROR(DFE_CANTDETACH, FAIL);
    }

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

    /* Clear error stack */
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
	if (length == FAIL)
	    HGOTO_DONE(0);

        /* if both arrays are NULL, get the number of data blocks and return */
        if ((offsetarray == NULL && lengtharray == NULL) && (info_count == 0))
        {
            count = HDgetdatainfo(hdf_file_id, ri_ptr->img_tag, ri_ptr->img_ref, NULL, start_block, 0, NULL, NULL);
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
            count = HDgetdatainfo(hdf_file_id, ri_ptr->img_tag, ri_ptr->img_ref, NULL, start_block, info_count, offsetarray, lengtharray);
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
	if (offset != NULL && length != NULL)
	{
	    off = Hoffset(file_id, ann_tag, ann_ref);
	    if (off == FAIL)
                HGOTO_ERROR(DFE_INTERNAL, FAIL);
	    len = Hlength(file_id, ann_tag, ann_ref);
	    if (len == FAIL)
                HGOTO_ERROR(DFE_INTERNAL, FAIL);
	    *offset = off;
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
