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
#include "hfile.h"
#include "vgint.h"

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
    call HDgetdatainfo_count to return the actual number of blocks in the
    data set's data.  Otherwise, it will call HDgetdatainfo to retrieve the
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
	    count = HDgetdatainfo_count(handle->hdf_file, var->data_tag, var->data_ref);
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
	    count = HDgetdatainfo(handle->hdf_file, var->data_tag, var->data_ref, start_block, info_count, offsetarray, lengtharray);
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

intn
SDgetattdatainfo(int32 sdsid,	/* IN: dataset ID */
        	 int32 findex,
		 intn attrindex,
		 char *attrname,/* IN: attribute name */
		 int32 *offset, /* OUT: buffer for offset */
		 int32 *length) /* OUT: buffer for length */
{
    CONSTR(FUNC, "SDgetattdatainfo");
    NC     *handle;
    NC_var *var;
    int32   n_elements;
    int32   vsid, elem_tag, elem_ref;
    char    vsclass[H4_MAX_NC_CLASS] = "", vsname[H4_MAX_NC_CLASS] = "";
    int     ii;
    intn    status;
    intn    ret_value = FAIL;

    /* get NC_var record */
    handle = SDIhandle_from_id(sdsid, SDSTYPE);
    if(handle == NULL || handle->file_type != HDF_FILE)
	HGOTO_ERROR(DFE_ARGS, FAIL);
    if(handle->vars == NULL)
	HGOTO_ERROR(DFE_ARGS, FAIL);

    var = SDIget_var(handle, sdsid);
    if(var == NULL) HGOTO_ERROR(DFE_ARGS, FAIL);

    if (var->vgid == 0) HGOTO_ERROR(DFE_ARGS, FAIL);

    n_elements = Vntagrefs(var->vgid);
    if (n_elements == FAIL)
      {
          ret_value = FAIL;
          goto done;
      }

    /*
     * look through for a Vdata of class _HDF_ATTRIBUTE
     */
    for (ii = 0; ii < n_elements; ii++) 
      {
          if (Vgettagref(var->vgid, ii, &elem_tag, &elem_ref) == FAIL)
            {
                ret_value = FAIL;
                goto done;
            }
      
          if(elem_tag == DFTAG_VH) 
            {
                vsid = VSattach(handle->hdf_file, elem_ref, "r");
                if(vsid == FAIL) 
                  {
                      ret_value = FAIL;
                      goto done;
                  }
          
                if (VSgetclass(vsid, vsclass) == FAIL)
                  {
                      ret_value = FAIL;
                      goto done;
                  }

/* This might include field's attribute as well so maybe no need for
field case - need verify */
                if(!HDstrcmp(vsclass, _HDF_ATTRIBUTE)) 
		{
		    status = VSgetname(vsid, vsname);
		    if (!HDstrcmp(attrname, vsname))
		    {
			status = VSgetdatainfo(vsid, 0, 1, offset, length);
			if (status == FAIL)
			    HGOTO_ERROR(DFE_GENAPP, FAIL);
		    }
		}

                if (VSdetach(vsid) == FAIL)
                  {
                      ret_value = FAIL;
                      goto done;
                  }
            }
      }
  
done:
    if (ret_value == FAIL)
      { /* Failure cleanup */
      }
     /* Normal cleanup */

    return ret_value;
} /* hdf_num_attrs */
#endif /* HDF */
