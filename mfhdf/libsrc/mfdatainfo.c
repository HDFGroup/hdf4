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
status = SDgetattdatainfo(sdsid, ...);

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
    if(var == NULL) HGOTO_ERROR(DFE_ARGS, FAIL);

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
    The number of data blocks retrieved, which should be 1, if successful and FAIL,
    otherwise.

 DESCRIPTION
    SDgetattdatainfo retrieves the location of the attribute's data and its length.

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
    int32   vg_ref=0,	/* ref# of the vgroup that represents a file, an SDS, or a dimension */
	    n_elements,	/* number of elements in the file/SDS/dimension vgroup */
	    vs_id,	/* vdata id, attached to a vdata while looking for _HDF_ATTRIBUTE */
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

	    /* Note: there is a vgid in NC_dim, unfortunately, the vgroup id wasn't recorded
		there during the reading of the file; remove this block later when that bug
		is fixed -BMR 2010/10/17 */
	    /* look for the variable representing this dimension, ie. coordinate variable */
	    var_idx = SDIgetcoordvar(handle, dim, id, (int32)0);
	    if(var_idx == FAIL)
		HGOTO_ERROR(DFE_ARGS, FAIL);

	    /* get the variable object */
	    var = NC_hlookupvar(handle, var_idx);
	    if(var == NULL)
		HGOTO_ERROR(DFE_ARGS, FAIL);

	    /* get the reference number of the vgroup representing this dimension */
	    vg_ref = var->vgid;
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

#endif /* HDF */
