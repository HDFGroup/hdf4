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

/* $Id: mfdatainfo.c 5399 2010-04-22 01:49:49Z bmribler $ */

/* NOTE: this file and other "datainfo" related files will be configured so
that this feature will not be built by default. -BMR */

/******************************************************************************
FILE
  mfdatainfo.c

  This file contains the HDF/netCDF based multi-file interface for functions
  that involve information about location and size of raw data.  This
  type of information allows applications to extract raw data from an HDF
  file without the use of HDF4 library.

  As with the rest of the SD functions, these functions have names beginning
  with SD.  Routines beginning with SDI are internal routines and
  should not be used outside of this module.

  Defining SDDEBUG will print status messages to stderr

EXPORTED ROUTINES
------------------

	--- retrieve location and size of data blocks
status = SDgetdatainfo(sdsid, ...);

	--- retrieve location and size of data blocks - not included yet
status = SDgetattdatainfo(id, ...);
status = SDgetanndatainfo(id, ...);

******************************************************************************/

#include "local_nc.h"
#ifdef VMS
#include <stat.h>
#endif

#ifdef HDF
#include "mfhdf.h"
 /* #include "hfile.h"
#include "vgint.h"
 */ 

#define DATAINFO_MASTER
#include "hdatainfo.h"
#include "mfdatainfo.h"

#ifdef H4_HAVE_LIBSZ          /* we have the szip library */
#include "szlib.h"
#endif

#ifndef MFSD_INTERNAL
#define MFSD_INTERNAL
#endif

#ifdef MFSD_INTERNAL
/* Local function prototypes */
#include "mfprivate.h"
#endif

PRIVATE intn get_attr_tag(char *attr_name, uint16* attr_tag);

/******************************************************************************
 NAME
    SDgetdatainfo -- Retrieves location and size of data blocks.
 USAGE
    intn SDgetdatainfo(sdsid, start_block, info_count, offsetarray, lengtharray)
	int32 sdsid		IN: dataset ID
	int32 start_block	IN: indicating where to start reading offsets
	uintn info_count	IN: number of data blocks the arrays can hold
	int32 *offsetarray	OUT: array for offsets
	int32 *lengtharray	OUT: array for lengths
 RETURNS
    The actual number of data blocks retrieved if successful and FAIL,
    otherwise.

 DESCRIPTION
    SDgetdatainfo retrieves two lists, one containing offsets to sdsid's data
    blocks and the other containing the lengths of those blocks.

    If start_block is 0, SDgetdatainfo will start getting data info from the
    beginning of the data set's data.  If start_block is greater than the
    number of blocks in the data set's data, SDgetdatainfo will return FAIL.

    When both offsetarray and lengtharray lists are NULL, SDgetdatainfo will
    call HDgetdatainfo passing in NULLs to return the actual number of blocks in the
    data set's data.  Otherwise, it will pass in non-NULL arrays to retrieve the
    offset and length of data blocks in the specified data set.  The non-NULL
    lists are assumed to be allocated sufficiently for holding info_count
    values.

    If info_count is larger than the actual number of offsets/lengths starting
    at start_block to the end of the data, then only the actual number
    offsets/lengths will be retrieved.

 MODIFICATION
    BMR - 2010/07/14: Revised to combine SDgetdatainfo and SDgetdatainfo_count

******************************************************************************/
intn
SDgetdatainfo(int32 sdsid,		/* IN: dataset ID */
	      int32 *chk_coord,		/* IN: chunk coord array or NULL for non-chunk SDS */
              uintn start_block,	/* IN: indicating where to start */
              uintn info_count,		/* IN: number of data blocks */
	      int32 *offsetarray,	/* OUT: array for offsets */
	      int32 *lengtharray)	/* OUT: array for lengths */
{
    CONSTR(FUNC, "SDgetdatainfo");    /* for HGOTO_ERROR */
    NC     *handle;
    NC_var *var;
    intn    count = FAIL; /* number of data blocks */
    intn    ret_value = 0;

    /* clear error stack */
    HEclear();

    /* get NC_var record */
    handle = SDIhandle_from_id(sdsid, SDSTYPE);
    if(handle == NULL || handle->file_type != HDF_FILE)
	HGOTO_ERROR(DFE_ARGS, FAIL);
    if(handle->vars == NULL)
	HGOTO_ERROR(DFE_ARGS, FAIL);

    var = SDIget_var(handle, sdsid);
    if(var == NULL)
        HGOTO_ERROR(DFE_ARGS, FAIL);

    /* if the data ref# of the SDS is 0, there is no storage is created yet */
    if (var->data_ref == 0)
    {
	count = 0; /* no blocks */
    }
    else
    {
	/* if both arrays are NULL, get the number of data blocks and return */
	if ((offsetarray == NULL && lengtharray == NULL) && (info_count == 0))
	{
	    count = HDgetdatainfo(handle->hdf_file, var->data_tag, var->data_ref, chk_coord, start_block, info_count, NULL, NULL);
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
	    count = HDgetdatainfo(handle->hdf_file, var->data_tag, var->data_ref, chk_coord, start_block, info_count, offsetarray, lengtharray);
	    if (count == FAIL)
		HGOTO_ERROR(DFE_INTERNAL, FAIL);
	}
    }
    ret_value = count;
done:
    if (ret_value == FAIL)
      { /* Failure cleanup */
      }
    /* Normal cleanup */
    return ret_value;
} /* SDgetdatainfo */


/******************************************************************************
 NAME
    SDgetattdatainfo -- Retrieves location and size of attribute's data.
 USAGE
    intn SDgetattdatainfo(id, attrindex, offset, length)
	int32 id		IN: file ID, SDS ID, or dimension ID
	int32 attrindex		IN: index of the attribute being inquired
	int32 *offset		OUT: offset of attribute's data
	int32 *length		OUT: length of attribute's data
 RETURNS
    The number of data blocks retrieved, which should be 1, if successful
    and FAIL, otherwise.

 DESCRIPTION
    SDgetattdatainfo retrieves the location of the attribute's data and its
    length.

 MODIFICATION
    2010/10/14: Revised to remove the parameter attrname because, for hmap
	project, it makes sense to just provide the attribute index. -BMR

******************************************************************************/
intn
SDgetattdatainfo(int32 id,	/* IN: dataset ID, dimension ID, or file ID */
		 int32 attrindex,/* IN: index of attribute to be inquired */
		 int32 *offset, /* OUT: buffer for offset */
		 int32 *length) /* OUT: buffer for length */
{
    CONSTR(FUNC, "SDgetattdatainfo");
    NC     *handle;
    NC_var *var;
    NC_dim *dim;
    int32   vg_ref=0,	/* ref# of the VG representing a file, an SDS, or a dimension */
	    n_elements,	/* number of elements in the file/SDS/dimension vgroup */
	    vs_id,	/* VS id, attached to a VS while looking for _HDF_ATTRIBUTE */
	    vg_id,	/* id of the file/SDS/dimension vgroup */
	    var_idx;	/* index of the variable representing the given dimension */
    char    vsclass[H4_MAX_NC_CLASS] = "", /* vs class to see if it is _HDF_ATTRIBUTE */
	    attrname[H4_MAX_NC_CLASS] = "",/* the given attribute's name */
	    vsname[H4_MAX_NC_CLASS] = "";  /* vs name to see if it's the inquired attr's name*/
    int32   ntype=0,	/* need these for SDattrinfo until fixing the function to... */
	    count=0;	/* ...check for NULL pointers */
    int     ii;
    intn    status,	/* returned value */
	    found,	/* TRUE when attribute is found */
	    ret_value = SUCCEED;

    /* Clear error stack */
    HEclear();

    /* Get the attribute's name */
    status = SDattrinfo(id, attrindex, attrname, &ntype, &count);

    /* Check if the id given is a file id, or SDS id, or dimension id, and get appropriate
       info structure */
    handle = SDIhandle_from_id(id, CDFTYPE);
    /* not a file id */
    if(handle == NULL)
    {
	/* then check if it's SDS id */
	handle = SDIhandle_from_id(id, SDSTYPE);
	/* not an SDS id */
	if(handle == NULL)
	{
	    /* then check if it's a dimension id */
	    handle = SDIhandle_from_id(id, DIMTYPE);

	    /* not a dimension id either, return with an error */
	    if(handle == NULL)
		HGOTO_ERROR(DFE_ARGS, FAIL);

	    /* dimension id is given, get the dimension info */
	    dim = SDIget_dim(handle, id);
	    if(dim == NULL)
		HGOTO_ERROR(DFE_ARGS, FAIL);

	    /* look for the variable representing this dimension,
		   ie. coordinate variable */
	    var_idx = SDIgetcoordvar(handle, dim, id, (int32)0);
	    if(var_idx == FAIL)
		HGOTO_ERROR(DFE_ARGS, FAIL);

	    /* get the variable object */
	    var = NC_hlookupvar(handle, var_idx);
	    if(var == NULL)
		HGOTO_ERROR(DFE_ARGS, FAIL);

	    /* get the ref number of the vgroup representing this dim */
	    vg_ref = var->vgid;

            /* If this dimension is not represented by a vgroup (old dimension,
               HDF 3.2, released in 1993) return to caller with error code
               DFE_DIMNOVG so caller can call SDgetdimattdatainfo to get
               to its attributes manually */
		if (vg_ref == 0)
		    HGOTO_DONE(DFE_DIMNOVG);
	}
	/* SDS id is given */
	else
	{
	    /* get the variable record */
	    if(handle->vars == NULL)
		HGOTO_ERROR(DFE_ARGS, FAIL);
	    var = SDIget_var(handle, id);
	    if(var == NULL)
		HGOTO_ERROR(DFE_ARGS, FAIL);

	    /* get the reference number of the vgroup representing this SDS */
	    vg_ref = var->vgid;
	}
    }
    /* File id is given */
    else
    {
	/* make sure it is an HDF file */
	if(handle->file_type != HDF_FILE)
	    HGOTO_ERROR(DFE_ARGS, FAIL);

	/* get the reference number of the vgroup representing the file */
	vg_ref = handle->vgid;
    }

    /* Validate the vgroup ref# */
    if (vg_ref == 0) HGOTO_ERROR(DFE_ARGS, FAIL);

    /* Get access to the vgroup and get the number of elements belong to it */
    vg_id = Vattach(handle->hdf_file, vg_ref, "r");
    if(vg_id == FAIL) 
	 HGOTO_ERROR(DFE_CANTATTACH, FAIL);
    n_elements = Vntagrefs(vg_id);
    if (n_elements == FAIL)
	HGOTO_ERROR(DFE_INTERNAL, FAIL);

    /* Look through the vgroup for vdatas of class _HDF_ATTRIBUTE.  If one is found, check
	to see if its name matches the searched attribute.  If it matches, use VSgetdatainfo
	to get offset and length of the attribute's data, then set flag to terminate the
	search */
    found = FALSE;
    for (ii = 0; ii < n_elements && !found; ii++) 
    {
	int32 elem_tag, elem_ref;

	/* get current tag/ref */
	if (Vgettagref(vg_id, ii, &elem_tag, &elem_ref) == FAIL)
	    HGOTO_ERROR(DFE_INTERNAL, FAIL);
      
	/* current element is a vdata */
	if(elem_tag == DFTAG_VH) 
	{
	    vs_id = VSattach(handle->hdf_file, elem_ref, "r");
	    if(vs_id == FAIL) 
	 	HGOTO_ERROR(DFE_CANTATTACH, FAIL);
          
	    if (VSgetclass(vs_id, vsclass) == FAIL)
	 	HGOTO_ERROR(DFE_INTERNAL, FAIL);

	    /* current vdata represents an attribute */
	    if(!HDstrcmp(vsclass, _HDF_ATTRIBUTE)) 
	    {
		if (VSgetname(vs_id, vsname) == FAIL)
		    HGOTO_ERROR(DFE_INTERNAL, FAIL);

		/* the searched attribute if found */
		if (!HDstrcmp(attrname, vsname))
		{
		    intn info_count=0;

		    /* get offset/length of attribute's data */
		    info_count = VSgetdatainfo(vs_id, 0, 1, offset, length);
		    if (info_count != 1) /* attribute data should only be in 1 block */
			HGOTO_ERROR(DFE_INTERNAL, FAIL);

		    /* set flag to terminate the search, and return info count */
		    found = TRUE;
		    ret_value = info_count;
		}
	    }
	    /* Close access to vdata */
	    if (VSdetach(vs_id) == FAIL)
		HGOTO_ERROR(DFE_CANTDETACH, FAIL);
	}
    }
done:
    if (ret_value == FAIL)
      { /* Failure cleanup */
      }
     /* Normal cleanup */

    return ret_value;
} /* SDgetattdatainfo */


/******************************************************************************
 NAME
    get_attr_tag -- Convert the name of attribute label, unit, or format to its
			   associated hdf tag (Private)
 USAGE
    intn get_attr_tag(attr_name, *attr_tag)
	char *attr_name		IN: name of the luf attributes
	uint16 *attr_tag	OUT: associated tag of luf
 RETURNS
    SUCCEED/FAIL

 DESCRIPTION
    In some older files, when dimensions were not stored as vgroup yet, the
    pre-defined attributes that belong to the dimensions were written following
    the same pre-defined attributes of the dataset.
    This function gives the associated tag of one of the luf attributes so that
    application can use tag/ref to read the luf string.
 
******************************************************************************/
PRIVATE intn 
get_attr_tag(char *attr_name, uint16* attr_tag)
{
    intn ret_value = SUCCEED;

    if (HDstrcmp(_HDF_LongName, attr_name) == 0)
        *attr_tag = DFTAG_SDL;
    else if (HDstrcmp(_HDF_Units, attr_name) == 0)
        *attr_tag = DFTAG_SDU;
    else if (HDstrcmp(_HDF_Format, attr_name) == 0)
        *attr_tag = DFTAG_SDF;
    else
    {
        ret_value = FAIL;
        fprintf(stderr, "get_attr_tag: attr_tag= %d, attr_name = %s\n", *attr_tag, attr_name);
    }
    return ret_value;
} /* get_attr_tag */

/******************************************************************************
 NAME
    SDgetdimattdatainfo -- Retrieves location and size of dimension attribute's
			   data.
 USAGE
    intn SDgetdimattdatainfo(id, sdsid, attr_name, offset, length)
	int32 dim_id		IN: dimension ID
	int32 sdsid		IN: SDS ID that dim_id belongs
	char *attr_name		IN: name of the attribute being inquired
	int32 *offset		OUT: offset of attribute's data
	int32 *length		OUT: length of attribute's data
 RETURNS
    The number of data blocks retrieved, which should be 1, if successful
    and FAIL, otherwise.

 DESCRIPTION
    In some older files, when dimensions were not stored as vgroup yet, the
    pre-defined attributes that belong to the dimensions were written following
    the same pre-defined attributes of the dataset.
 
 SDgetdimattdatainfo retrieves the location of the attribute's data and its
    length.

 MODIFICATION
    2010/10/14: Revised to remove the parameter attrname because, for hmap
	project, it makes sense to just provide the attribute index. -BMR

******************************************************************************/
intn
SDgetdimattdatainfo(int32 dim_id,	/* IN: dimension ID */
		    int32 sdsid,	/* IN: ID of dataset the dim belongs to */
		    char  *attr_name,    /* IN: name of attribute to be inquired */
		    int32 *offset,      /* OUT: buffer for offset */
		    int32 *length)      /* OUT: buffer for length */
{
    CONSTR(FUNC, "SDgetdimattdatainfo");
    NC     *handle;
    NC_var *var;
    NC_dim *dim;
    int32   off, len,
            dim_att_len=0,
            sdsluf_len=0,
            offp = 0;
    char   *lufbuf=NULL, *lufp=NULL;
    uint16  att_tag, att_ref;
    intn    dimidx_infile=0, dimidx_invar=0;
    int     ii;
    intn    status,	/* returned value */
	    ret_value = 0;

    /* Clear error stack */
    HEclear();

    /* Check if the given dimension id really is a dimension id */
    handle = SDIhandle_from_id(dim_id, DIMTYPE);
    if(handle == NULL)
	HGOTO_ERROR(DFE_ARGS, FAIL);

    /* Check if the given dataset id really is a dataset id */
    handle = SDIhandle_from_id(sdsid, SDSTYPE);
    if(handle == NULL)
	HGOTO_ERROR(DFE_ARGS, FAIL);

    /* Get the variable object of the dataset */
    var = SDIget_var(handle, sdsid);
    if(var == NULL)
	HGOTO_ERROR(DFE_ARGS, FAIL);

    /* If this variable doesn't have a valid NDG ref, we cannot proceed */
    if (var->ndg_ref == 0)
	HGOTO_ERROR(DFE_ARGS, FAIL);

    att_tag = 0;
    att_ref = var->ndg_ref; /* all elements of this var have the same ref# */

    /* Convert a luf name into its corresponding HDF tag */
    status = get_attr_tag(attr_name, &att_tag);
    if (status == FAIL)
    {
        fprintf(stderr, "attribute name is invalid for old dimension: %s\n", attr_name);
        HGOTO_ERROR(DFE_ARGS, FAIL);
    }

    if (att_tag != 0 && att_ref != 0)
    {
        /* Get offset/length of the luf string, which includes luf of the dataset
           and its dimensions all together */
        off = Hoffset(handle->hdf_file, att_tag, att_ref);
        if (off == FAIL)
            HGOTO_ERROR(DFE_BADOFFSET, FAIL);

        len = Hlength(handle->hdf_file, att_tag, att_ref);
        if (len == FAIL)
            HGOTO_ERROR(DFE_BADLEN, FAIL);

        if (len == 0)
        {
            HGOTO_DONE(0);
        }

        /* Read the luf string */
        lufbuf = (char *)HDmalloc(len+1);
        if (lufbuf == NULL)
            HGOTO_ERROR(DFE_NOSPACE, FAIL);
        Hgetelement(handle->hdf_file, att_tag, att_ref, lufbuf);

        /*
         * Parse the luf string to obtain the offset/length of the requested
         * dimension's luf
         */
        /* Start at the beginning of the dataset's luf */
        lufp = lufbuf;

        /* Skip dataset's luf using the first null character in the string */
        sdsluf_len = HDstrlen(lufbuf);

        /* Move cursor forward to the first dimension's attribute */
        lufp += sdsluf_len + 1;

        /* Convert dimension id back to index */
        dimidx_infile = dim_id & 0xffff;

        /* Find the index of dimension within the variable */
        dimidx_invar = -1;
        for (ii = 0; ii < var->assoc->count && dimidx_invar != ii; ii++)
            if (var->assoc->values[ii] == dimidx_infile)
                dimidx_invar = ii;

        /* Walk through each dimension to find requested luf */
        offp = 0;
        for (ii = 0; ii <= dimidx_invar; ii++)
        {
            char dim_att[100];

            /* Extract dimension 1 label */
            HDstrcpy(dim_att, (char *) lufp);
            dim_att_len = HDstrlen(dim_att);

            /* Move forward if this is not the dimension we're looking for */
            if (ii != dimidx_invar)
	    {
                lufp += dim_att_len + 1;
	        offp += dim_att_len + 1;
	    }
        }

        /* Calculate offset and length of the requested dimension's luf   */
        /* - off: offset where luf string starts, returned by Hoffset     */
        /* - sdsluf_len: length of the dataset luf + 1 for the null space */
        /* - offp: offset accummulated of all dimension lufs before the   */
        /*   requested one */
        *offset = off + sdsluf_len + 1 + offp;
        *length = dim_att_len;
        ret_value = 1;
    }
    if (lufbuf) HDfree(lufbuf);

done:
    if (ret_value == FAIL)
      { /* Failure cleanup */
        if (lufbuf) HDfree(lufbuf);
      }
     /* Normal cleanup */

    return ret_value;
} /* SDgetdimattdatainfo */


/******************************************************************************
 NAME
    SDgetanndatainfo -- Retrieves location and size of annotations' data.
 USAGE
    intn SDgetanndatainfo(sdsid, annot_type, size, offsetarray, lengtharray)
	int32 sdsid		IN: SDS ID
	ann_type annot_type	IN: type of annotations to retrieve data
	uintn size		IN: size of offsetarray and lengtharray
	int32 *offsetarray	OUT: offsets of annotations' data
	int32 *lengtharray	OUT: lengths of annotations' data
 RETURNS
    The number of annotation data info retrieved, if successful
    and FAIL, otherwise.

 DESCRIPTION
    SDgetanndatainfo retrieves the location and size specifying data of the
    annotations of the specified type from the dataset sdsid.

 IMPORTANT NOTE
    If caller provides buffers that are smaller than the number of annotations
    then SDgetanndatainfo only fills the buffers up to its size, and returns
    the number of annotations retrieved.  That means, the rest of the
    annotations are not retrievable.  However, this function is designed for
    hmap writer at this point, and such insufficient buffers situation is not
    anticipated beside the fact that we're running out of time.  In the future,
    or when such need arises, the function should be modified to include
    another parameter to allow retrieving partial annotations.

 MODIFICATION

******************************************************************************/
intn SDgetanndatainfo(int32 sdsid, ann_type annot_type, uintn size, int32* offsetarray, int32* lengtharray)
{
    CONSTR(FUNC, "SDgetanndatainfo");
    int32   file_id=FAIL, an_id=FAIL; /* file ID and AN API ID */
    NC     *handle = NULL;	/* file structure */
    NC_var *var = NULL;		/* variable structure of sds, to get NDG ref */
    int32  *dannots = NULL;	/* list of data annotation IDs */
    uint16  elem_tag, elem_ref; /* tag/ref of dataset's NDG */
    intn    num_annots,		/* number of annotation of requested type */
            ii,
            ret_value = 0;

    /* Annotation type must only be data label or description */
    if (annot_type != AN_DATA_LABEL && annot_type != AN_DATA_DESC)
	HGOTO_ERROR(DFE_ARGS, FAIL);

    /* Check if the given dataset id really is a dataset id */
    handle = SDIhandle_from_id(sdsid, SDSTYPE);
    if(handle == NULL)
        HGOTO_ERROR(DFE_ARGS, FAIL);

    /* Get the NDG ref of this dataset */
    elem_ref = SDidtoref(sdsid);
    elem_tag = DFTAG_NDG;

    /* If this variable doesn't have a valid NDG ref, we cannot proceed */
    if (elem_ref <= 0)
	HGOTO_ERROR(DFE_ARGS, FAIL);

    /* Open file to start Annotation inteface */
    if ((file_id = Hopen(handle->path, DFACC_READ, 0)) == FAIL)
        HGOTO_ERROR(DFE_BADOPEN, FAIL);
    if ((an_id = ANstart(file_id)) == FAIL)
        HGOTO_ERROR(DFE_CANTINIT, FAIL);

    /* Get number of data descs or labels with this tag/ref */
    num_annots = ANnumann(an_id, annot_type, elem_tag, elem_ref);
    if (num_annots == FAIL)
        HGOTO_ERROR(DFE_INTERNAL, FAIL);

    /* If offsets and lengths are not desired, return the number of annots */
    if (offsetarray == NULL || lengtharray == NULL)
	HGOTO_DONE(num_annots);

    /* If more annotations than space in user's buffers, only fill up buffers */
    if (num_annots > size)
        num_annots = size;

    /* Retrieve annotations */
    if (num_annots != 0)
    {
        /* Allocate space for list of annotation IDs on this tag/ref */
        if ((dannots = (int32 *)HDmalloc(num_annots * sizeof(int32))) == NULL)
	    HGOTO_ERROR(DFE_NOSPACE, FAIL);

        /* Get list of annotations IDs on this tag/ref */
        if (ANannlist(an_id, annot_type, elem_tag, elem_ref, dannots) == FAIL)
            HGOTO_ERROR(DFE_INTERNAL, FAIL);

        /* Loop through the annotation list and get their offsets/lengths */
        for (ii = 0; ii < num_annots; ii++)
        {
            /* Get annotation's offset and length */
            ret_value = ANgetdatainfo(dannots[ii], &offsetarray[ii], &lengtharray[ii]);
	    if (ret_value == FAIL)
                HGOTO_ERROR(DFE_INTERNAL, FAIL);
        }

        /* Return the number of annotations retrieved */
        ret_value = num_annots;

        /* Terminate access to the AN API and close the file */
        if(an_id != FAIL)
            ANend(an_id);
        if(file_id != FAIL)
            Hclose(file_id);
    } /* if there are annotations */

done:
    if (ret_value == FAIL)
      { /* Failure cleanup */
        if (dannots) HDfree(dannots);
      }
     /* Normal cleanup */
    return ret_value;
} /* SDgetanndatainfo */

#endif /* HDF */
