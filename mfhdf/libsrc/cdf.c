/*
 *	Copyright 1993, University Corporation for Atmospheric Research
 *      See netcdf/COPYRIGHT file for copying and redistribution conditions.
 */
/*	$Id$ */

#include	"local_nc.h"
#include	"alloc.h"
#include        <hfile.h>

static bool_t NC_xdr_cdf
    PROTO((XDR *xdrs,NC **handlep));

#define WRITE_NDG 1

/* Private buffer */
PRIVATE uint8 *ptbuf = NULL;

/*
 * free the stuff that xdr_cdf allocates
 */
void
NC_free_xcdf(handle)
NC *handle ;
{
    if(handle == NULL)
        return ;
    NC_free_array(handle->dims) ;
    NC_free_array(handle->attrs) ;
    NC_free_array(handle->vars) ;
}

void
NC_free_cdf(handle)
NC *handle ;
{
      if(handle == NULL)
              return ;
      NC_free_xcdf(handle) ;
#ifndef macintosh /* We don't handle xdr files yet */
      xdr_destroy(handle->xdrs) ;
#endif /* !macintosh */
      Free(handle->xdrs) ;

      if(handle->file_type == HDF_FILE) {
          Vend(handle->hdf_file);
          Hclose(handle->hdf_file);
      }

      Free(handle) ;
}

/*
  
  From NASA CDF Source

*/
#define V2_MAGIC_NUMBER  0x0000FFFF   /* Written twice at the beginning of file */
#define V2_MAGIC_OFFSET  0


/* -------------------------------- Hiscdf -------------------------------- */
/*

  Return TRUE/FALSE depending on if the given file is a NASA CDF file

*/
intn
Hiscdf(filename)
const char * filename;
{
    
    char *FUNC = "Hiscdf";
    intn       ret;
    hdf_file_t fp;
    uint8      b[4];
    uint8    * bb;
    int32      magic_num;
  
    fp = HI_OPEN(filename, DFACC_READ);
    if (OPENERR(fp)) {
        return(FALSE);
    } else {
        if(HI_SEEK(fp, V2_MAGIC_OFFSET) == FAIL) {
            HERROR(DFE_SEEKERROR);
            return FALSE;
        }
        
        if(HI_READ(fp, b, 4) == FAIL) {
            HERROR(DFE_READERROR);
            return FALSE;
        }

        bb = &b[0];

        INT32DECODE(bb, magic_num); 

        if(magic_num == V2_MAGIC_NUMBER) 
            ret = TRUE;
        else 
            ret = FALSE;

        HI_CLOSE(fp);
        return(ret);
    }
}



NC *
NC_new_cdf(name, mode)
const char *name ;
int mode ;
{
	NC *cdf ;
        int32 hdf_mode;
        char * FUNC = "NC_new_cdf";

	cdf = (NC *)HDmalloc(sizeof(NC)) ;
	if( cdf == NULL )
	{
		nc_serror("NC_new_cdf") ;
		return(NULL) ;
	}

	cdf->flags = mode ;

	cdf->xdrs = (XDR *)HDmalloc(sizeof(XDR)) ;
	if( cdf->xdrs == NULL)
	{
		nc_serror("NC_new_cdf: xdrs") ;
		Free(cdf) ;
		return(NULL) ;
	} /* else */
	
        /*
         * See what type of file we are looking at.
         * If we are creating a new file it will be an HDF file
         */
        if(mode & NC_CREAT) {
            cdf->file_type = HDF_FILE;
        } else {
            if(Hishdf((char *) name))
                cdf->file_type = HDF_FILE;
            else if(Hiscdf((char *) name))
                cdf->file_type = CDF_FILE;
            else
                cdf->file_type = netCDF_FILE;
            
            if(cdf->file_type == CDF_FILE)
                printf("Yow!  found a CDF file\n");
        }

        /*
         * Set up the XDR functions that some of the netCDF old code uses
         */
        switch(cdf->file_type) {
        case HDF_FILE:
            hdf_xdrfile_create(cdf->xdrs, mode);
            break;
        case netCDF_FILE:
            if( NCxdrfile_create( cdf->xdrs, name, mode ) < 0) {
                Free(cdf->xdrs) ;
                Free(cdf) ;
                return(NULL) ;
            } 
            break;
        case CDF_FILE:
/*             CDF_xdrfile_create(); */
            /* try this, I bet it will be sufficient */
            hdf_xdrfile_create(cdf->xdrs, mode);
            break;
        }
        
	cdf->dims = NULL ;
	cdf->attrs = NULL ;
	cdf->vars = NULL ;
	cdf->begin_rec = 0 ;
	cdf->recsize = 0 ;
	cdf->numrecs = 0 ;
	cdf->redefid = -1 ;

        /* 
         * determine the HDF access mode 
         */
        switch(mode) {
        case NC_CLOBBER   :
            hdf_mode = DFACC_CLOBBER; break;
        case NC_NOCLOBBER :
            /* will handle below */
            break;
        case NC_WRITE     :
		hdf_mode = DFACC_RDWR;    break;
        case NC_NOWRITE   :
            hdf_mode = DFACC_RDONLY;  break;
        default:
            hdf_mode = DFACC_RDWR;
        }
        
        
        /*
         * Do file type specific setup
         */
        switch(cdf->file_type) {
        case HDF_FILE: 
            
            /* see if the file exists */
            if(mode == NC_NOCLOBBER) {
                if((int) Hishdf((char *) name))
                  { /* Need to free allocated structures */
                    NC_free_xcdf(cdf) ;
#ifndef macintosh /* We don't handle xdr files yet */
                    xdr_destroy(cdf->xdrs) ;
#endif /* !macintosh */
                    Free(cdf->xdrs) ;
                    Free(cdf) ;
                    return(NULL);
                  }
                hdf_mode = DFACC_RDWR;
            }
                
            cdf->hdf_file = (int32) Hopen((char *) name, hdf_mode, 200);
            Vstart(cdf->hdf_file);
            
            if(cdf->hdf_file == FAIL) {
                return (NULL);
            }
            
            cdf->hdf_mode = hdf_mode;
            cdf->vgid = 0;
            
            HDstrncpy(cdf->path, name, FILENAME_MAX);
            
#ifdef DEBUG
            printf("value returned from Hopen() : %d\n", cdf->hdf_file);
#endif
            break;
        case netCDF_FILE:
#ifdef macintosh
            /* for mac we don't handle XDR files */
            return (NULL);
#endif /* macintosh */
            break;
        case CDF_FILE:
            fprintf(stderr, "About to do CDF file set up\n");
            cdf->cdf_fp =  HI_OPEN(name, hdf_mode);
            if (OPENERR(cdf->cdf_fp)) 
                HRETURN_ERROR(DFE_DENIED,NULL);
            break;
        }

        /*
         * Read in the contents
         */
	if(cdf->xdrs->x_op == XDR_DECODE) /* Not NC_CREAT */
	{
		if(!xdr_cdf(cdf->xdrs, &cdf) )
		{
			NC_free_cdf(cdf) ;
			return (NULL) ;
		}
		if( NC_computeshapes(cdf) == -1)
			return (NULL) ;
	}

	return (cdf) ;
}


/* 
 * Duplicate a description structure.
 * Can only be called for 'old' extant on disk, eg, old in DATA mode.
 */
NC *
NC_dup_cdf(name, mode, old)
const char *name ;
int mode ;
NC *old ;
{
	NC *cdf ;

	cdf = (NC *)HDmalloc(sizeof(NC)) ;
	if( cdf == NULL )
	{
		nc_serror("NC_dup_cdf") ;
		return(NULL) ;
	}

	cdf->flags = old->flags | NC_INDEF ;

	cdf->xdrs = (XDR *)HDmalloc(sizeof(XDR)) ;
	if( cdf->xdrs == NULL)
	{
		nc_serror("NC_dup_cdf: xdrs") ;
		Free(cdf) ;
		return(NULL) ;
	} /* else */	

	cdf->dims = NULL ;
	cdf->attrs = NULL ;
	cdf->vars = NULL ;
	cdf->begin_rec = 0 ;
	cdf->recsize = 0 ;
	cdf->numrecs = 0 ;

/*
#ifdef HDF
        if(old->file_type == HDF_FILE) {
          cdf->hdf_file = old->hdf_file;
          cdf->file_type = old->file_type;
          cdf->vgid = old->vgid;
        }
#endif
*/

#ifdef HDF
        cdf->file_type = old->file_type;
#endif

	if(NCxdrfile_create( cdf->xdrs, name, mode) < 0)
	{
		Free(cdf) ;
		return(NULL) ;
	} /* else */	

	old->xdrs->x_op = XDR_DECODE ;
	if(!xdr_cdf(old->xdrs, &cdf) )
	{
		NC_free_xcdf(cdf) ;
		return(NULL) ;
	}
	if( NC_computeshapes(cdf) == -1)
		return(NULL) ;

	return(cdf) ;
}


ncinquire(cdfid, ndimsp, nvarsp, nattrsp, xtendimp)
int cdfid ;
int *ndimsp ;
int *nvarsp ;
int *nattrsp ;
int *xtendimp ;
{
	NC *handle ;

	cdf_routine_name = "ncinquire" ;

	handle = NC_check_id(cdfid) ;
	if(handle == NULL)
		return(-1) ;
	
	if(nvarsp != NULL)
		*nvarsp = (handle->vars != NULL) ? handle->vars->count : 0 ;
	if(nattrsp != NULL)
		*nattrsp = (handle->attrs != NULL) ? handle->attrs->count : 0 ;
	if(handle->dims != NULL)
	{
		NC_dim **dp ;
		int ii ;

		if(ndimsp != NULL)
			*ndimsp = handle->dims->count ;
		if(xtendimp != NULL) {
			*xtendimp = -1 ;

			dp = (NC_dim**)handle->dims->values ;
			for(ii = 0 ; ii < handle->dims->count ; ii++, dp++)
			{
				if((*dp)->size == NC_UNLIMITED)
				{
					*xtendimp = ii ;
				}
			}
		}
	} else {
		if(ndimsp != NULL)
			*ndimsp = 0 ;
		if(xtendimp != NULL)
			*xtendimp = -1 ;
	}

	return(cdfid) ;
}

bool_t
xdr_cdf(xdrs, handlep)
	XDR *xdrs;
	NC **handlep;
{
    
    switch((*handlep)->file_type) {
    case HDF_FILE:
        return(hdf_xdr_cdf(xdrs, handlep));
        break;
    case netCDF_FILE:
        return(NC_xdr_cdf(xdrs, handlep));
        break;
    case CDF_FILE:
        return(nssdc_xdr_cdf(xdrs, handlep));
        break;
    default:
        return FALSE;
    }
}


static bool_t
NC_xdr_cdf(xdrs, handlep)
	XDR *xdrs;
	NC **handlep;
{

	u_long	magic = NCMAGIC ;

	if( xdrs->x_op == XDR_FREE)
	{
		NC_free_xcdf(*handlep) ;
		return(TRUE) ;
	}
	
	if( xdr_getpos(xdrs) != 0)
	{
		if( !xdr_setpos(xdrs, 0L) )
		{
			nc_serror("Can't set position to begin") ;
			return(FALSE) ;
		}
	}

	/* magic number */
	if( !xdr_u_long(xdrs, &magic) )
	{
		if( xdrs->x_op == XDR_DECODE)
		{
			NCadvise(NC_ENOTNC,
				"Not a netcdf file (Can't read magic number)") ;
		}
		else
		{
			/* write error */
			nc_serror("xdr_cdf: xdr_u_long") ;
		}
		return(FALSE) ;
	}

	if( xdrs->x_op == XDR_DECODE && magic != NCMAGIC )
	{
		if(magic == NCLINKMAGIC)
		{
			NCadvise(NC_NOERR, "link file not handled yet") ;
			return(FALSE) ;
		} /* else */
		NCadvise(NC_ENOTNC, "Not a netcdf file") ;
		return(FALSE) ;
	}

	if( !xdr_numrecs(xdrs, *handlep))
	{
		NCadvise(NC_EXDR, "xdr_numrecs") ;
		return(FALSE) ;
	}
	if( !xdr_NC_array(xdrs, &((*handlep)->dims)))
	{
		NCadvise(NC_EXDR, "xdr_cdf:dims") ;
		return(FALSE) ;
	}
	if( !xdr_NC_array(xdrs, &((*handlep)->attrs)))
	{
		NCadvise(NC_EXDR, "xdr_cdf:attrs") ;
		return(FALSE) ;
	}
	if( !xdr_NC_array(xdrs, &((*handlep)->vars)))
	{
		NCadvise(NC_EXDR, "xdr_cdf:vars") ;
		return(FALSE) ;
	}	
	return(TRUE) ;
}

#ifdef HDF

/*****************************************************************************
* 
*			NCSA HDF / netCDF Project
*			        May, 1993
*
* NCSA HDF / netCDF source code and documentation are in the public domain.  
* Specifically, we give to the public domain all rights for future
* licensing of the source code, all resale rights, and all publishing rights.
* 
* We ask, but do not require, that the following message be included in all
* derived works:
* 
* Portions developed at the National Center for Supercomputing Applications at
* the University of Illinois at Urbana-Champaign.  Funding for this project 
* has come primarily from the National Science Foundation.
* 
* THE UNIVERSITY OF ILLINOIS GIVES NO WARRANTY, EXPRESSED OR IMPLIED, FOR THE
* SOFTWARE AND/OR DOCUMENTATION PROVIDED, INCLUDING, WITHOUT LIMITATION,
* WARRANTY OF MERCHANTABILITY AND WARRANTY OF FITNESS FOR A PARTICULAR PURPOSE
* 
******************************************************************************
*
* Please report all bugs / comments to chouck@ncsa.uiuc.edu
*
*****************************************************************************/

/* A couple of local prototypes */

void hdf_cdf_clobber
  PROTO((NC *handle));

void hdf_close
  PROTO((NC *handle));

/* ----------------------------------------------------------------
** Map an NC_<type> to an HDF type
*/
int
  hdf_map_type(type)
int type;
{

  switch(type) {
  case NC_CHAR   :
    return DFNT_CHAR;
  case NC_BYTE   :
    return DFNT_INT8;
  case NC_SHORT   :
    return DFNT_INT16;
  case NC_LONG   :
    return DFNT_INT32;
  case NC_FLOAT   :
    return DFNT_FLOAT32;
  case NC_DOUBLE   :
    return DFNT_FLOAT64;
  default:
    return DFNT_NONE;
  }

} /* hdf_map_type */


/* ----------------------------------------------------------------
**  UnMap a data type.  I.e. go from an HDF type to an NC_<type>
**  The HDF type may be in DFNT_NATIVE mode, so only look at the 
**    bottom bits
*/
int
  hdf_unmap_type(type)
int type;
{

  switch(type & 0xff) {
  case DFNT_CHAR        :
    return NC_CHAR;
  case DFNT_INT8        :
  case DFNT_UINT8       :
    return NC_BYTE;
  case DFNT_INT16       :
  case DFNT_UINT16      :
    return NC_SHORT;
  case DFNT_INT32       :
  case DFNT_UINT32      :
    return NC_LONG;
  case DFNT_FLOAT32     :
    return NC_FLOAT; 
  case DFNT_FLOAT64     :
    return NC_DOUBLE;
  default:
    return FAIL;
  }

} /* hdf_unmap_type */

/* -----------------------------------------------------------------
** Given a dimension id number return its hdf_ref number (Vgroup id)
*/
int
  hdf_get_ref(handle, i)
NC *handle;
int i;
{
  NC_array *tmp;
  NC_dim **d;
  Void *dims;
  
  tmp = handle->dims;
  dims = handle->dims->values;
  dims += i * tmp->szof;
  
  d = (NC_dim **) dims;
  return (*d)->vgid;

} /* get_hdf_ref */


/* ----------------------------------------------------------------
** Given a dimension pointer return the ref of a Vdata which was
**   newly created to represent the values the dimension takes on
** If there is a variable with the same name as our dimension then
**   the values the variable takes on are the values for the
**   'steps' in the dimension.
** Otherwise, the dimension takes on the values 0..(size -1)
**
** NOTE:  This may cause conflicts cuz we may get called before
**   the variable's values are set???
*/
int 
  hdf_create_dim_vdata(xdrs, handle, dim)
XDR *xdrs;
NC *handle;
NC_dim *dim;
{
  char *FUNC = "hdf_create_dim_vdata";
  int found = FALSE;
  int i;
  int ref;
  long dsize;

#if DEBUG
 fprintf(stderr, "hdf_create_dim_vdata I've been called\n");
 fprintf(stderr, "handle->hdf_file = %d\n", handle->hdf_file);
#endif

  dsize = dim->size;
  if(dsize == NC_UNLIMITED) {
#ifdef DEBUG
      fprintf(stderr, "Fudging unlimited dimension.\n");
#endif
      dsize = 1;
  }

#if 0
  /* look for variable with the given name */
  if(handle->vars) {
    dp = (NC_var **) handle->vars->values;
    for(ii = 0; ii < handle->vars->count; ii++, dp++)
      if(HDstrcmp(dim->name->values, (*dp)->name->values) == 0) {
	/* found it */
	found = TRUE;
	break;
      }
  }

#endif

  if(found) {
    /* load in the variable's values */
#if DEBUG
   fprintf(stderr, "Found real values for dimension %s\n", dim->name->values);
#endif

  } else {
      int *val;
      /* create a fake one */
#if DEBUG
      fprintf(stderr, "Creating fake dim  ::::%s::: (%d)\n", dim->name->values, dsize);
#endif
      
      /* allocate space */
      val = (int *) HDmalloc(dsize * sizeof(int));
      if(!val) {
          HERROR(DFE_NOSPACE);
          return FAIL;
      }
      
      if(dim->size != NC_UNLIMITED) {
          for(i = 0; i < dsize; i++)
              val[i] = i;
      } else {
          val[0] = handle->numrecs;
      }
      
      ref = VHstoredata(handle->hdf_file, "Values", (void *)val, dsize,
                        DFNT_INT32, dim->name->values, DIM_VALS);
      
      if(ref == FAIL) {
          fprintf(stderr, "FAILed creating Vdata %s\n", dim->name->values);
          return FAIL;
      }
      
      HDfree((VOIDP)val);

  }
  
#if DEBUG
  fprintf(stderr, "Returning vdata pointer %d\n", ref);
#endif
  
  return ref;
  
} /* hdf_create_dim_vdata */

/* ----------------------------------------------------------------
** Write out a vdata representing an attribute
*/
int
  hdf_write_attr(xdrs, handle, attr)
XDR *xdrs;
NC *handle;
NC_attr **attr;
{
  int status;
  char *name;
  Void *values;
  int size;
  int type;
  int order;

  name = (*attr)->name->values;
  values = (*attr)->data->values;
  size = (*attr)->data->count;
  type = (*attr)->HDFtype;

#if DEBUG
 fprintf(stderr, "hdf_write_attr I've been called\n");
 fprintf(stderr, "The attribute is called %s\n", name);
 fprintf(stderr, "Type = %d (%d)  size  %d\n", type, (*attr)->HDFtype, size);
 fprintf(stderr, "Value: ");
  switch(type) {
  case DFNT_CHAR :fprintf(stderr, " (char) %s\n", (char *) values); break;
  case DFNT_UINT8 :fprintf(stderr, " (uint8) %d\n", (char *) values); break;
  case DFNT_INT8 :fprintf(stderr, " (int8) %d\n", (char *) values); break;
  case DFNT_UINT16 :fprintf(stderr, " (uint16) %d\n", (int *) values); break;
  case DFNT_INT16 :fprintf(stderr, " (int16) %d\n", (int *) values); break;
  case DFNT_UINT32 :fprintf(stderr, " (uint32) %d\n", (int *) values); break;
  case DFNT_INT32 :fprintf(stderr, " (int32) %d\n", (int *) values); break;
  case DFNT_FLOAT32 :fprintf(stderr, " (float32) %f\n", (float *) values); break;
  case DFNT_FLOAT64 :fprintf(stderr, " (float64) %f\n", (double *) values); break;
  default:fprintf(stderr, "???\n");
  }
#endif

  if(type == DFNT_CHAR) {
      order = size;
      size = 1;
  } else { 
      order = 1;
  }
      
  status = VHstoredatam(handle->hdf_file, ATTR_FIELD_NAME, 
                        (unsigned char *) values, size, 
                        type, name, ATTRIBUTE, order);

#if DEBUG
 fprintf(stderr, "hdf_write_attr returning %d\n", status);
#endif

  return status;

} /* hdf_write_attr */

/* ----------------------------------------------------------------
** Write out a group representing a dimension
*/
int32
  hdf_write_dim(xdrs, handle, dim, cnt)
XDR *xdrs;
NC *handle;
NC_dim **dim;
int32 cnt;
{
  int32 status;
  int32 tags[100], refs[100];
  int32 count;
  char  *class;
  char  name[MAX_NC_NAME];

#if DEBUG
 fprintf(stderr, "hdf_write_dim I've been called\n");
 fprintf(stderr, "The name is -- %s -- \n", (*dim)->name->values);
#endif

  /*
   * Look up to see if there is a variable of the same name
   *  giving values
   */
  count = 0;
  tags[count] = DFTAG_VH;
  refs[count] = hdf_create_dim_vdata(xdrs, handle, (*dim));
  if(refs[count] == FAIL) return FAIL;
  count++;
  
  if((*dim)->size == NC_UNLIMITED) 
      class = UDIMENSION;
  else
      class = DIMENSION;
  
  if(HDstrncmp((*dim)->name->values, "fakeDim", 7) == 0)
      sprintf(name, "fakeDim%d", cnt);
  else
      HDstrcpy(name, (*dim)->name->values);

  status = VHmakegroup(handle->hdf_file, tags, refs, count, 
		       name, class);

  if(status == FAIL)
      HEprint(stdout, 0);

  (*dim)->vgid = status;

  return status;

} /* hdf_write_dim */


/* ----------------------------------------------------------------
** Write out a group representing a variable
** If successfull, return the id of the created Vgroup else
**  return NULL
*/
int32
  hdf_write_var(xdrs, handle, var)
XDR *xdrs;
NC *handle;
NC_var **var;
{
  NC_array  *  attrs;
  NC_iarray *  assoc;
  NC_dim    ** dp;
  uint8        ntstring[4];
  uint16       ref;
  int8         outNT;
  int32 tags[MAX_NC_ATTRS + MAX_VAR_DIMS + 2], refs[MAX_NC_ATTRS + MAX_VAR_DIMS + 10];
  uint16       nt_ref, rank;
  int32     *  ip, GroupID, val;
  uint8     *  bufp;
  char      *FUNC = "hdf_write_var";

  register int  i, count;
  register Void *attribute;

  count = 0;
  assoc = (*var)-> assoc;
  attrs = (*var)-> attrs;

#if DEBUG
 fprintf(stderr, "hdf_write_var I've been called\n");
 fprintf(stderr, "handle->hdf_file = %d\n", handle->hdf_file);
 fprintf(stderr, "The name is -- %s -- \n", (*var)->name->values);

  if(assoc && assoc->count) {
    fprintf(stderr, "value of assoc %d\n", assoc);
    fprintf(stderr, " assoc->count %d\n", assoc->count);
    fprintf(stderr, " asc[0] %d asc[1] %d\n", assoc->values[0], assoc->values[1]);
  }
#endif

  /*
   *  Get the dimension information
   */
  for(i = 0; i < assoc->count; i++) {
    tags[count] = DIM_TAG;
    refs[count] = hdf_get_ref(handle, assoc->values[i]);
    count++;
  }

  /* 
   * Add info for the attributes
   */
  if(attrs) {
    attribute = attrs->values;
    for(i = 0; i < attrs->count; i++) {
      tags[count] = ATTR_TAG;
      refs[count] = hdf_write_attr(xdrs, handle, (NC_attr **)attribute);
      attribute += attrs->szof;
      count++;
    }
  }
  
  /*
   * If we already have data written out include that too
   *   (this might happen after a redef() cuz we will leave
   *   the data sitting on the disk but clear out all the 
   *   meta-data)
   */
  if((*var)->data_ref) {
      tags[count] = (int32) DFTAG_SD;
      refs[count] = (*var)->data_ref;
#if DEBUG
      fprintf(stderr, " ---- Carrying forward data with ref %d ---- \n", (*var)->data_ref);
#endif 
      count++;
  } 
  
  
  /*
   * Write out a number type tag so that we can recover this 
   *   variable's type later on
   *
   * by default numbers are converted to IEEE otherwise we need to save the 
   *   machine type in the NT object
   */
  if((*var)->HDFtype & DFNT_NATIVE)
      outNT = DFKgetPNSC((*var)->HDFtype, DF_MT);
  else
      outNT = DFNTF_IEEE;

  ref = Hnewref(handle->hdf_file);
  ntstring[0] = DFNT_VERSION;                    /* version */
  ntstring[1] = (uint8)((*var)->HDFtype & 0xff); /* type */
  ntstring[2] = (uint8)((*var)->HDFsize * 8);    /* width (in bits) */
  ntstring[3] = outNT;                           /* class: IEEE or machine class */
  if(Hputelement(handle->hdf_file, DFTAG_NT, ref, ntstring, (int32) 4) == FAIL)
      return FAIL;
  tags[count] = DFTAG_NT;
  refs[count] = ref;
  nt_ref = (uint16) ref;
  count++;
  
#ifdef WRITE_NDG
    /* Check if temproray buffer has been allocated */
    if (ptbuf == NULL)
      {
        ptbuf = (uint8 *)HDmalloc(TBUF_SZ * sizeof(uint8));
        if (ptbuf == NULL)
          HRETURN_ERROR(DFE_NOSPACE, FAIL);
      }

  /* prepare to start writing ndg   */
  if ((GroupID = DFdisetup(10)) < 0)
      return FAIL;
  
  /* write SD record */
  if((*var)->data_ref)
      if (DFdiput(GroupID, DFTAG_SD, (uint16) (*var)->data_ref) == FAIL)
          return FAIL;
  
  /* write NT tag/ref */
  if (DFdiput(GroupID, DFTAG_NT, (uint16) ref) == FAIL)
      return FAIL;

  /* put rank & dimensions in buffer */
  bufp = ptbuf;
  rank = assoc->count;
  UINT16ENCODE(bufp, rank);
  for(i = 0; i < (int)rank; i++) {

      val = (int32) (*var)->shape[i];

      /* need to fake the size of the record dimension */
      if(val == NC_UNLIMITED) {
          if(handle->file_type == HDF_FILE)
              val = (*var)->numrecs;
          else
              val = handle->numrecs;
      }

      INT32ENCODE(bufp, val);
  }
  
  /* "<=" used to put 1 data NT + rank scale NTs in buffer */
  for (i = 0; i <= (int)rank; i++) 
      {  /* scale NTs written even if no scale!*/
          UINT16ENCODE(bufp, DFTAG_NT);
          UINT16ENCODE(bufp, nt_ref);
      }   
  /* write out SDD record */
  if(Hputelement(handle->hdf_file, DFTAG_SDD, ref, ptbuf, (int32) (bufp-ptbuf)) == FAIL)
      return FAIL;
  
  /* write dimension record tag/ref */
  if (DFdiput(GroupID, DFTAG_SDD,(uint16) ref) == FAIL)
      return FAIL;

  tags[count] = DFTAG_SDD;
  refs[count] = ref;
  count++;

  /* Add a bogus tag so we know this NDG is really a variable */
  if (DFdiput(GroupID, BOGUS_TAG,(uint16) ref) == FAIL)
      return FAIL;

  /* write out NDG */
  if (DFdiwrite(handle->hdf_file, GroupID, DFTAG_NDG, (*var)->ndg_ref) < 0) 
      return FAIL;
  
  tags[count] = DFTAG_NDG;
  refs[count] = (*var)->ndg_ref;
  count++;

#endif /* WRITE_NDG */

  (*var)->vgid = VHmakegroup(handle->hdf_file, tags, refs, count, 
                             (*var)->name->values, VARIABLE);

  if((*var)->vgid == FAIL) {
      fprintf(stderr, "Failed to write variable %s\n", (*var)->name->values);
      fprintf(stderr, "count = %d\n", count);
      for(i = 0; i < count; i++)
          fprintf(stderr, "i = %d   tag = %d ref = %d\n", i, tags[i], refs[i]);
          
      HEprint(stdout, 0);
  }  

  return (*var)->vgid;
  
} /* hdf_write_var */


/* ----------------------------------------------------------------
** Write out a cdf structure
*/
bool_t
  hdf_write_xdr_cdf(xdrs, handlep)
XDR *xdrs;
NC **handlep;
{
  int32 *tags, *refs, count;
  int sz, i, j, status, done;
  NC_dim **dims, **dims1;
  NC_array *tmp;
  Void *vars;
  Void *attrs;

#if DEBUG
 fprintf(stderr, "hdf_write_xdr_cdf i've been called op = %d \n", xdrs->x_op);
#endif

  /* count size of tag / ref arrays */
  sz = 0;
  if((*handlep)->dims)  sz += (*handlep)->dims->count;
  if((*handlep)->vars)  sz += (*handlep)->vars->count;
  if((*handlep)->attrs) sz += (*handlep)->attrs->count;

#if DEBUG
 fprintf(stderr, "sz = %d\n", sz);
#endif

  /* allocate tag / ref arrays */
  tags = (int32 *) HDmalloc(sz * sizeof(int32) + 1);
  refs = (int32 *) HDmalloc(sz * sizeof(int32) + 1);
  if(!tags || !refs) {
      fprintf(stderr, "Out of memory line %d file %s\n", __LINE__, __FILE__);
      return FALSE;
  }

  /* 
  ** write out dimension arrays 
  */
  count = 0;
  if((*handlep)->dims) {
      tmp = (*handlep)->dims; 
      dims = (NC_dim **) (*handlep)->dims->values;
      for(i = 0; i < tmp->count; i++) {

          /* this is really ugly and should be handled another way */
          /* make sure we don't duplicate dimensions */
          done = FALSE;
          dims1 = (NC_dim **) (*handlep)->dims->values;
          for(j = 0; j < i; j++) {
              if((*dims1)->size == (*dims)->size &&
                 HDstrcmp((*dims)->name->values, 
                          (*dims1)->name->values) == 0)
                  done = TRUE;
              dims1++;
          }

          if(!done) {
              tags[count] = (int32) DIM_TAG;
              refs[count] = (int32) hdf_write_dim(xdrs, (*handlep), dims, count);
              if(refs[count] == FAIL) return FALSE;
              count++;
          }
          dims++;
      }
  }
  
  /* 
  ** write out variable info 
  */
  if((*handlep)->vars) {
      tmp = (*handlep)->vars; 
      vars = (*handlep)->vars->values;
      for(i = 0; i < tmp->count; i++) {
          tags[count] = (int32) VAR_TAG;
          refs[count] = (int32) hdf_write_var(xdrs, (*handlep), (NC_var **)vars);
          if(refs[count] == FAIL) return FALSE;
          vars += tmp->szof;
          count++;
      }
  }

  /*
   * write global attribute information
   */
  if((*handlep)->attrs) {
      tmp = (*handlep)->attrs; 
      attrs = (*handlep)->attrs->values;
      for(i = 0; i < tmp->count; i++) {
          tags[count] = (int32) ATTR_TAG;
          refs[count] = (int32) hdf_write_attr(xdrs, (*handlep), (NC_attr **)attrs);
          if(refs[count] == FAIL) return FALSE;
          attrs += tmp->szof;
          count++;
      }
  }

#if DEBUG
  fprintf(stderr, "About to write top level VG with %d elements\n", count);
  {
      int i; 
      for(i = 0; i < count; i++) fprintf(stderr, "%d :=> %d %d\n", i, tags[i], refs[i]);
  }
#endif

  /* write out final VGroup thang */
  status = VHmakegroup((*handlep)->hdf_file, tags, refs, count, 
		       (*handlep)->path, CDF);
  if(status == FAIL)
      HEprint(stdout, 0);

  HDfree((VOIDP)tags);
  HDfree((VOIDP)refs);

#ifdef DEBUG
  fprintf(stderr, "======= Have finished writing top level VGroup #%d\n", status);
#endif
  
  /* set the top level CDF VGroup pointer */
  (*handlep)->vgid = status;
  
  return TRUE;

} /* hdf_write_xdr_cdf */


/* ----------------------------------------------------------------
** Read in the dimensions out of a cdf structure
** Return FAIL if something goes wrong
*/
int 
  hdf_read_dims(xdrs, handle, vg)
XDR    *xdrs;
NC     *handle;
int32  vg;
{

  char vgname[100], vsclass[128], vgclass[128];
  int id, count, found;
  int sub_id;
  int32 dim_size;
  NC_dim **dimension;
  int32 dim, entries;
  int32 vs;

  found = FALSE;
  count = 0;
  id = -1;

#if DEBUG
 fprintf(stderr, "hdf_read_dims I've been called, handle->hdf_file = %d\n", handle->hdf_file);
#endif

  /*
   * Allocate enough space in case everything is a dimension
   */
  count = 0;
  dimension = (NC_dim **) HDmalloc(sizeof(NC_dim *) * Vntagrefs(vg) + 1);
  if(!dimension) {
    fprintf(stderr, "Out of memory line %d file %s\n", __LINE__, __FILE__);
    return FAIL;
  }

  /*
   * Look through for a Vgroup of class DIMENSION
   */
  while((id = Vgetnext(vg, id)) != FAIL) {
    if(Visvg(vg, id)) {
      dim = Vattach(handle->hdf_file, id, "r");
      if(dim == FAIL) continue;
      Vgetclass(dim, vgclass);
      if(!HDstrcmp(vgclass, DIMENSION) || !HDstrcmp(vgclass, UDIMENSION)) {
	Vinquire(dim, &entries, vgname);      
	
	/* 
	 * look through for a Vdata of class DIM_VALS to get size 
	 */
	sub_id = -1;
	while(((sub_id = Vgetnext(dim, sub_id)) != FAIL) && !found) {
	  if(Visvs(dim, sub_id)) {
	    vs = VSattach(handle->hdf_file, sub_id, "r");
	    if(vs == FAIL) HEprint(stdout, 0);
	    
            VSgetclass(vs, vsclass);
	    if(!HDstrcmp(vsclass, DIM_VALS)) {
              VSQuerycount(vs, &dim_size);

              if(!HDstrcmp(vgclass, UDIMENSION)) {
                  dim_size = NC_UNLIMITED;
                  VSsetfields(vs, "Values");
                  VSseek(vs, 0);
                  
                  /*
                   * This is highly dangerous since there might be multiple
                   * unlimited dimensions
                   */
                  if(VSread(vs, (uint8 *) &(handle->numrecs), 1, FULL_INTERLACE) != 1)
                      HEprint(stderr, 0);
              }

	      dimension[count] = NC_new_dim(vgname, dim_size);
	      if(!dimension[count]) {
		fprintf(stderr, "Can't create new dimension #%d\n", count);
		return FAIL;
	      }
	      
#if DEBUG
	     fprintf(stderr, "Dimension <%s> has size %d\n", vgname, dim_size);
#endif
	      count++;
	      
	    }
	    VSdetach(vs);
	  }
	}
      }
      Vdetach(dim);
    }
  }

  if(count)
    handle->dims = NC_new_array(NC_DIMENSION, count, (Void *) dimension);
  else
    handle->dims = NULL;

  HDfree((VOIDP)dimension);

#if DEBUG
 fprintf(stderr, "Created dimension array %d \n", handle->dims);
#endif

  return SUCCEED;

} /* hdf_read_dims */

/* ----------------------------------------------------------------
** Read in all attributes of the current vg
** Return NULL if something goes wrong
** Return a pointer to the array of attributes if all goes well
*/
NC_array *
  hdf_read_attrs(xdrs, handle, vg)
XDR    *xdrs;
NC     *handle;
int32   vg;
{

  char vsname[100], fields[100], *values, class[128];
  int count, type, t, n;
  NC_attr **attributes;
  NC_array *Array = NULL;
  int32 vs, tag, id, vsize, attr_size, nt;

  count = 0;
  id = -1;

#if DEBUG
 fprintf(stderr, "hdf_read_attrs I've been called, handle->hdf_file = %d\n", handle->hdf_file);
#endif

  /*
   * Allocate enough space in case everything is an attribute
   */
  count = 0;
  attributes = (NC_attr **) HDmalloc(sizeof(NC_attr *) * Vntagrefs(vg) + 1);
  if(!attributes) {
    fprintf(stderr, "Out of memory line %d file %s\n", __LINE__, __FILE__);
    return NULL;
  }

  /*
   * look through for a Vdata of class ATTRIBUTE
   */
  n = Vntagrefs(vg);
  for (t = 0; t < n; t++) {
      Vgettagref(vg, t, &tag, &id);
      
      if(tag == DFTAG_VH) {
          vs = VSattach(handle->hdf_file, id, "r");
          if(vs == FAIL) 
              HEprint(stdout, 0);
          
          VSgetclass(vs, class);
          if(!HDstrcmp(class, ATTRIBUTE)) {
              VSinquire(vs, &attr_size, NULL, fields, &vsize, vsname);
              nt = VFfieldtype(vs, 0);
              type = hdf_unmap_type(nt);
              values = (char *) HDmalloc(vsize * attr_size + 1);
              VSsetfields(vs, fields);
              VSread(vs, (uint8 *) values, attr_size, FULL_INTERLACE);
              
              if(type == NC_CHAR) {
                  attr_size = VFfieldorder(vs, 0);
                  ((char *) values)[attr_size] = '\0';
              }
              attributes[count] = 
                  (NC_attr *) NC_new_attr(vsname, type, attr_size, values);
              if(!attributes[count]) {
                  fprintf(stderr, "Can't create new attribute #%d\n", count);
                  return NULL;
              }
              attributes[count]->HDFtype = nt;
              
#if DEBUG
              fprintf(stderr, "Attribute <%s> has type %d and size %d\n", 
                      vsname, type, attr_size);
#endif
              HDfree((VOIDP)values);
              count++;
              
          }
          VSdetach(vs);
      }
  }
  
  if(count) Array = NC_new_array(NC_ATTRIBUTE, count, (Void *) attributes);

  HDfree((VOIDP)attributes);

#if DEBUG
  fprintf(stderr, "Created attribute array %d \n", Array);
#endif

  return Array;

} /* hdf_read_attrs */



/* ----------------------------------------------------------------
** Read in the variables out of a cdf structure
** Return FAIL if something goes wrong
**
** Important:  We must already assume that handle->dims is set
**   so that we can do a call to NC_var_shape() so that we can
**   set the numrecs fields of variables (so we can fill record
**   variables intelligently)
*/
int 
  hdf_read_vars(xdrs, handle, vg)
XDR    *xdrs;
NC     *handle;
int32  vg;
{

  char vgname[100], subname[100], class[128];
  NC_var **variables, *vp;
  int    ndims, *dims;
  uint8 ntstring[4];
  int   data_ref, is_rec_var, vg_size, count;
  int32 data_count, HDFtype, tag, id;
  int32 n, sub_id, entries, ndg_ref, rag_ref;

  register int     type, t, i;
  register int32   var, sub;

  count = 0;
  id = -1;

#if DEBUG
  fprintf(stderr, "hdf_read_vars I've been called, handle->hdf_file = %d\n", handle->hdf_file);
#endif

  /*
   * Allocate enough space in case everything is a variable
   */
  count = 0;
  variables = (NC_var **) HDmalloc(sizeof(NC_var *) * Vntagrefs(vg) + 1);
  if(!variables) {
    fprintf(stderr, "Out of memory line %d file %s\n", __LINE__, __FILE__);
    return FAIL;
  }

  /*
   * Allocate enough space in case lots of dimensions
   */
  dims = (int *) HDmalloc(sizeof(int) * Vntagrefs(vg) + 1);
  if(!dims) {
    fprintf(stderr, "Out of memory line %d file %s\n", __LINE__, __FILE__);
    return FAIL;
  }

  /*
   * Look through for a Vgroup of class VARIABLE
   */
  vg_size = Vntagrefs(vg);
  for(i = 0; i < vg_size; i++) {
      Vgettagref(vg, i, &tag, &id);
      if(tag == DFTAG_VG) {
          var = Vattach(handle->hdf_file, id, "r");
          if(var == FAIL) continue;
          
         Vgetclass(var, class);
          if(!HDstrcmp(class, VARIABLE)) {
              
              /*
               * We have found a VGroup representing a Variable
               */
              ndims = 0;
              type = 0;
              data_ref = 0;
              data_count = 0;
              rag_ref = 0;
              is_rec_var = FALSE;
              Vinquire(var, &n, vgname);      
          
              /*
               * Loop through contents looking for dimensions
               */
              for (t = 0; t < n; t++) {
                  Vgettagref(var, t, &tag, &sub_id);
                  
                  switch(tag) {
                  case DFTAG_VG :   /* ------ V G R O U P ---------- */
                      sub = Vattach(handle->hdf_file, sub_id, "r");

                      Vgetclass(sub, class);
                      if(!HDstrcmp(class, DIMENSION) || 
                         !HDstrcmp(class, UDIMENSION)) {
                          
                          if(!HDstrcmp(class, UDIMENSION))
                              is_rec_var = TRUE;
                          
                          Vinquire(sub, &entries, subname);      
                          dims[ndims] = (int) NC_dimid( handle, subname);
                          ndims++;
                      }
                      Vdetach(sub);
                      break;
                  case DFTAG_VH :   /* ----- V D A T A ----- */
                      break;
                  case DFTAG_NDG :  /* ----- NDG Tag for HDF 3.2 ----- */
                      ndg_ref = sub_id;
                      break;
                  case DFTAG_SD :   /* ------- Data Storage ------ */
                      data_ref = sub_id;
                      data_count = Hlength(handle->hdf_file, DATA_TAG, sub_id);
                      break;
                  case DFTAG_SDRAG : /* ----- Ragged Array index ----- */
                      rag_ref = sub_id;
                      printf("Lookout!  Found a ragged array element\n");
                      break;
                  case DFTAG_NT :   /* ------- Number type ------- */
                      if(Hgetelement(handle->hdf_file, tag, sub_id, ntstring) == FAIL)
                          return FAIL;
                      HDFtype = ntstring[1];
                      type = hdf_unmap_type(HDFtype);

                      /*
                       * Check if data was stored in native format
                       * And make sure the numbertype version numbers are the same
                       */
                      if((ntstring[0] != DFNT_VERSION) ||
                         ((ntstring[3] != DFNTF_NONE) &&
                          (ntstring[3] != DFNTF_IEEE))) {
                          
                          /* check if in native mode for a different type of machine */
                          if(ntstring[3] != DFKgetPNSC(HDFtype, DF_MT)) {
                              /* 
                               * OK, we have a problem here --- is in native mode
                               * for a different machine.  PUNT
                               */
                              
                              goto bad_number_type;

                          } else {
                              /*
                               * Is in native mode but its OK --- same machine type
                               */
                              
                              HDFtype |= DFNT_NATIVE;

                          }

                      }
                      
                      break;
                  default:
                      /* Do nothing */
                      break;
                  }
              }
          
              variables[count] = NC_new_var(vgname, type, ndims, dims);
              vp = variables[count];
              if(!vp) {
                  fprintf(stderr, "Can't read new variable %s\n", vgname);
                  return FAIL;
              }
              
#if DEBUG
              printf("Created a variable called %s   (id %d) \n", vgname, id);
#endif
              /* Read in the attributes */
              vp->attrs = hdf_read_attrs(xdrs, handle, var);
              
              /* set up for easy access later */
              vp->vgid     = id;
              vp->data_ref = data_ref;
              vp->data_tag = DATA_TAG;
              vp->HDFtype  = HDFtype;
              vp->ndg_ref  = (uint16) ndg_ref;
              vp->cdf = handle; /* for NC_var_shape */

              /* need to process the ragged array info here */
              /* QUESTION:  Load the whole rag_fill list in now??????? */
              if(rag_ref) {
                  vp->is_ragged = TRUE;
              }
              
              if(vp->data_ref) {
                  /*
                   * We have already seen data for this variable so now
                   *  we need to worry about its numrecs field
                   */
                  
                  if(is_rec_var) {
                      /*
                       * Call NC_var_shape() so we can figure out how many
                       *  records have been written.  This is horribly 
                       *  inefficient, but the separation-of-powers gets
                       *  really mucked up if we wait till later...
                       */
                      
                      if(NC_var_shape(vp, handle->dims) == -1)
                          return FAIL;
                      
                      /*
                       * Now figure out how many recs have been written
                       * For a while there was a -1 at the end of this
                       *   equation.  I don't remember why its there
                       *   (4-Nov-93)
                       */
                      vp->numrecs = data_count / vp->dsizes[0];
                      
#ifdef DEBUG
                      fprintf(stderr, "I have set numrecs to %d\n", vp->numrecs);
#endif                  
                      /*
                       * Deallocate the shape info as it will be recomputed
                       *  at a higher level later
                       */
                      HDfree((VOIDP)vp->shape);
                      HDfree((VOIDP)vp->dsizes);
                      
                  } else {
                      /* Not a rec var, don't worry about it */
                      vp->numrecs = 1;
                  }
              } 
              
              count++;
              
          }

bad_number_type:
          
          Vdetach(var);
      }
  }
  
  if(count)
      handle->vars = NC_new_array(NC_VARIABLE, count, (Void *) variables);
  else
      handle->vars = NULL;

  HDfree((VOIDP)variables);
  HDfree((VOIDP)dims);
  
#if DEBUG
  fprintf(stderr, "Created variable array %d \n", handle->vars);
#endif
  
  return SUCCEED;

} /* hdf_read_vars */



/* ----------------------------------------------------------------
** Read in a cdf structure
*/
bool_t
  hdf_read_xdr_cdf(xdrs, handlep)
XDR *xdrs;
NC **handlep;
{
#if DEBUG
  char            vgname[100];
  int32           entries;
#endif
  register int32  cdf_vg;
  register int    vgid, status;
#ifdef OLD_WAY
  register int    found;
  char            class[128];
#endif /* OLD_WAY */
  CONSTR(FUNC,"hdf_read_xdr_cdf");

#if DEBUG
 fprintf(stderr, "hdf_read_xdr_cdf i've been called %d\n", (*handlep)->hdf_file);
#endif

#ifdef OLD_WAY
  /* find first thing of type CDF */
  vgid = -1;
  found = FALSE;
  while(!found && ((vgid = Vgetid((*handlep)->hdf_file, vgid)) != FAIL)) {
    cdf_vg = Vattach((*handlep)->hdf_file, vgid, "r");
    if(cdf_vg == FAIL) 
        HRETURN_ERROR(DFE_CANTATTACH,FALSE);
    Vgetclass(cdf_vg, class);
    if(!HDstrcmp(class, CDF)) 
        found = TRUE;
    else 
        Vdetach(cdf_vg);
  }
  if(!found)
    return FALSE;

#else
    if((vgid=Vfindclass((*handlep)->hdf_file,CDF))!=FAIL) {
        cdf_vg = Vattach((*handlep)->hdf_file, vgid, "r");
        if(cdf_vg == FAIL) 
            HRETURN_ERROR(DFE_CANTATTACH,FALSE);
      } /* end if */
    else
        return(FALSE);
#endif /* OLD_WAY */

  (*handlep)->vgid = vgid;

#if DEBUG
  Vinquire(cdf_vg, &entries, vgname);

 fprintf(stderr, "Found CDF : %s  (%d entries)\n", vgname, entries);
#endif

  /* read in dimensions */
  status = hdf_read_dims(xdrs, (*handlep), cdf_vg);
  if(status == FAIL)
    return FALSE;

  /* read in variables */
  status = hdf_read_vars(xdrs, (*handlep), cdf_vg);
  if(status == FAIL)
    return FALSE;

  /* read in attributes */
  (*handlep)->attrs = hdf_read_attrs(xdrs, (*handlep), cdf_vg);

  Vdetach(cdf_vg);

  return TRUE;

} /* hdf_read_xdr_cdf */

/* -------------------------------------------------------------------
** Read or write a CDF structure
**
** If we are reading, first try to read the information out of netCDF
**    object stored explicitly in HDF files as netCDF objects.  If
**    that fails try to read SDSs out of the HDF file and interpret
**    them as netCDF information.
*/
bool_t
  hdf_xdr_cdf(xdrs, handlep)
XDR *xdrs;
NC **handlep;
{
  CONSTR(FUNC,"hdf_xdr_cdf"); /* for HERROR */

  int status;

#if DEBUG
  fprintf(stderr, "hdf_xdr_cdf i've been called op = %d \n", xdrs->x_op);
#endif

  switch(xdrs->x_op) {
  case XDR_ENCODE :
      if((*handlep)->vgid) hdf_cdf_clobber((*handlep));
      status = hdf_write_xdr_cdf(xdrs, handlep);
      break;
  case XDR_DECODE :
      if((status = hdf_read_xdr_cdf(xdrs, handlep)) == FALSE) {
          status = hdf_read_sds_cdf(xdrs, handlep);
	  if(status==FALSE)
	      HRETURN_ERROR(DFE_BADNDG,FALSE);
	} /* end if */
      break;
  case XDR_FREE   :
      NC_free_cdf((*handlep));
      status = TRUE;
      break;
  default:
      status = TRUE;
  }
  
  return status;
  
} /* hdf_xdr_cdf */


/* ---------------------- hdf_vg_clobber --------------- */
/*
  Delete a VGroup that is on the disk.  Basically, we will want to
  just trash everything inside of it, making sure that any VDatas
  with class == DATA are saved
*/
void hdf_vg_clobber(handle, id)
NC *handle;
int id;
{
  int t, n;
  int32 vg, tag, ref;
  int32 vs, status;

  /* loop through and Clobber all top level VGroups */
  vg = Vattach(handle->hdf_file, id, "r");
  if(vg == FAIL) return;

  /* Loop though and kill stuff */
  n = Vntagrefs(vg);
  for (t = 0; t < n; t++) {
    Vgettagref(vg, t, &tag, &ref);

#ifdef DEBUG
    fprintf(stderr, " Looking at <%d, %d>\n", tag, ref);
    fflush(stderr);
#endif
    switch(tag) {
    case DFTAG_VG :
        hdf_vg_clobber(handle, ref);
        break;
    case DFTAG_VH :
        vs = VSattach(handle->hdf_file, ref, "r");
        if(vs == FAIL) continue;
        
        VSdetach(vs);
        status = VSdelete(handle->hdf_file, (int32) ref);
/*
        Hdeldd(handle->hdf_file, DFTAG_VH, ref);
        Hdeldd(handle->hdf_file, DFTAG_VS, ref);
*/ 
        if(status == FAIL) printf("Yikes failed deleting Vdata %d\n",ref);

        break;
    case DFTAG_SD :
        /*
         * Don't delete actual numeric data
         */
        break;
    default:
        Hdeldd(handle->hdf_file, (uint16) tag, (uint16) ref);
        break;
    }
  }

  Vdetach(vg);

} /* hdf_vg_clobber */


/* --------------------------- hdf_cdf_clobber ---------------------------- */
/*
  Delete a netCDF structure that has been already written to disk
*/
void hdf_cdf_clobber(handle)
NC *handle;
{
  int32  vg, tag, ref;
  int    n, t, status;

  if(!handle->vgid) return;

  /* Close open VData pointers */
  hdf_close(handle);

  /* loop through and Clobber all top level VGroups */
  vg = Vattach(handle->hdf_file, handle->vgid, "r");
  if(vg == FAIL) return;

  /* Loop though and just kill everyone */
  n = Vntagrefs(vg);
  for (t = 0; t < n; t++) {
    Vgettagref(vg, t, &tag, &ref);
    if(tag == DFTAG_VG) hdf_vg_clobber(handle, ref);

    switch(tag) {
    case DFTAG_VG:
        status = Vdelete(handle->hdf_file, (int32) ref);
        break;
    case DFTAG_VH:
        status = VSdelete(handle->hdf_file, (int32) ref);
        break;
    default:
        status = Hdeldd(handle->hdf_file, (uint16) tag, (uint16) ref);
    }

    if(status == FAIL) printf("Yikes failed deleting %d\n", tag);

  }

#ifdef DEBUG
  fprintf(stderr,"Clobbering VGroup %d\n", handle->vgid);
#endif
  
  Vdetach(vg);
  status = Vdelete(handle->hdf_file, (int32) handle->vgid);
  if(status == FAIL) printf("Yikes failed deleting VGroup\n");
/*   Hdeldd(handle->hdf_file, DFTAG_VG, handle->vgid); */
  handle->vgid = 0;

#ifdef DEBUG
  fprintf(stderr, "    ALL done with the cdf_clobber()\n");
#endif

} /* hdf_cdf_clobber */

/* -------------------------- hdf_close --------------------- */
/*
  We're about to close the file, do last minute HDF cleanup
  Also dump the number of records currently instatiated into the
  unlimited dimensions.  BUG:  All unlimited dimensions will have
  the same size
*/
void hdf_close(handle)
     NC *handle;
{
    NC_array *tmp;
    NC_var **vp;
    Void *vars;
    register int i;
    int id, sub_id;
    int32 vg, dim;
    int32 vs;
    char class[128];
    
#ifdef DEBUG
    fprintf(stderr,"hdf_close, I've been called\n");
#endif
    
    /* loop through and detach from variable data VDatas */
    if(handle->vars) {
        tmp = handle->vars; 
        vars = handle->vars->values;
        
        for(i = 0; i < tmp->count; i++) {
            vp = (NC_var **) vars;

            if((*vp)->aid != FAIL)
                Hendaccess((*vp)->aid);
            (*vp)->aid = FAIL;

            vars += tmp->szof;
        }
    }
    
    /* loop through top level looking for unlimited dimensions */
    if(handle->flags & NC_NDIRTY) {
        id = -1;
        vg = Vattach(handle->hdf_file, handle->vgid, "r");
        while((id = Vgetnext(vg, id)) != FAIL) {
            if(Visvg(vg, id)) {
                dim = Vattach(handle->hdf_file, id, "r");
                Vgetclass(dim, class);
                if(!HDstrcmp(class, UDIMENSION)) {
                    
                    sub_id = -1;
                    while((sub_id = Vgetnext(dim, sub_id)) != FAIL) {
                        if(Visvs(dim, sub_id)) {
                            vs = VSattach(handle->hdf_file, sub_id, "w");
                            if(vs == FAIL) HEprint(stdout, 0);
                            
                            VSgetclass(vs, class);
                            if(!HDstrcmp(class, DIM_VALS)) {
                                VSsetfields(vs, "Values");
                                VSseek(vs, 0);
                                if(VSwrite(vs, (uint8 *) &(handle->numrecs), 1, FULL_INTERLACE) != 1) 
                                    HEprint(stderr, 0);  

                            }
                            VSdetach(vs);
                        }
                    }
                }
                Vdetach(dim);
            }
        }
        Vdetach(vg);
    }

#ifdef QAK
    /* Free dimension array if necessary */
    if(handle->dims) 
        NC_free_array(handle->dims); /* Might need to close Vdatas? -QAK */
#endif /* QAK */
    
} /* hdf_close */
                
#endif /* HDF */

/*
 * How much space will the xdr'd NC description take.
 *
 */
NC_xlen_cdf(cdf)
NC *cdf ;
{
	int len = 8 ;

	if(cdf == NULL)
		return(0) ;

	len += NC_xlen_array(cdf->dims) ;
	len += NC_xlen_array(cdf->attrs) ;
	len += NC_xlen_array(cdf->vars) ;

	return(len) ;
}


#define RECPOS	4L 	/* seek index of numrecs value */
bool_t
xdr_numrecs(xdrs, handle)
	XDR *xdrs;
	NC *handle;
{

#ifdef HDF
    if(handle->file_type == HDF_FILE) return TRUE;
#endif

	if( (handle->flags & NC_NOFILL)
		&& xdrs->x_op == XDR_ENCODE
		&& handle->begin_rec > 0)
	{
		/*
		 * we need to write something just beyond the last 
		 * record so we can successfully read back the 
		 * entire last record.
		 */
		if( !xdr_setpos(xdrs, handle->begin_rec
				+  handle->numrecs * handle->recsize) )
		{
			nc_serror("Can't set position to EOF") ;
			return(FALSE) ;
		}
#ifdef RDEBUG
		fprintf(stderr,"\txdr_numrecs %ld = %d + %ld * %d\n",
			xdr_getpos(xdrs), 
			handle->begin_rec, handle->numrecs, handle->recsize) ;
#endif /*  RDEBUG */
		if( !xdr_u_long(xdrs, &(handle->numrecs)) )
			return(FALSE) ;
	}

	if( !xdr_setpos(xdrs, RECPOS) )
	{
		nc_serror("Can't set position to RECPOS") ;
		return(FALSE) ;
	}
	return( xdr_u_long(xdrs, &(handle->numrecs)) ) ;
}

static bool_t
xdr_4bytes(xdrs, cp)
XDR *xdrs ;
char *cp ; /* at least 4 valid bytes */
{
      return xdr_opaque(xdrs, cp, 4) ;
}

static bool_t
xdr_2shorts(xdrs, sp)
XDR *xdrs ;
short *sp ; /* at least 2 valid shorts */
{
      return xdr_shorts(xdrs, sp, 2) ;
}

bool_t
xdr_NC_fill(xdrs, vp)
XDR *xdrs ;
NC_var *vp ;
{
	char fillp[2*sizeof(double)] ;
	bool_t stat ;
	bool_t (*xdr_NC_fnct)() ;
	u_long alen = vp->len ;
	NC_attr **attr = NULL ;


	/*
	 * set up fill value
	 */
	/* start with the default */
	NC_arrayfill((Void *)fillp, (size_t)2*sizeof(double),
		vp->type) ;

	/* 
	 * if there is a valid user defined value, use it instead
	 */
	attr = NC_findattr(&vp->attrs, _FillValue) ;
	if( attr != NULL )
	{
		if( (*attr)->data->type != vp->type || (*attr)->data->count != 1 )
			NCadvise(NC_EBADTYPE, "var %s: _FillValue type mismatch",
				vp->name->values) ;
		else
		{
			int len = NC_typelen(vp->type) ;
			char *cp = fillp ;
			while(cp < &fillp[sizeof(fillp) -1])
			{
				NC_copy_arrayvals(cp, (*attr)->data) ;
				cp += len ;
			}
		}
	}

		switch(vp->type){
		case NC_BYTE :
		case NC_CHAR :
			alen /= 4 ;
			xdr_NC_fnct = xdr_4bytes ;
			break ;
		case NC_SHORT :
			alen /= 4 ;
			xdr_NC_fnct = xdr_2shorts ;
			break ;
		case NC_LONG :
			alen /= 4 ;
#ifdef __alpha
			xdr_NC_fnct = xdr_int ;
#else
			xdr_NC_fnct = xdr_long ;
#endif
			break ;	
		case NC_FLOAT :
			alen /= 4 ;
			xdr_NC_fnct = xdr_float ;
			break ;
		case NC_DOUBLE : 
			alen /= 8 ;
			xdr_NC_fnct = xdr_double ;
			break ;
		default :
			NCadvise(NC_EBADTYPE, "bad type %d", vp->type) ;
			return(FALSE) ;
		}

		/* write out fill values */
		for(stat = TRUE ; stat && (alen > 0) ; alen--)
		{
			stat = (*xdr_NC_fnct)(xdrs,fillp) ;	
		}

		if(!stat)
		{
			NCadvise(NC_EXDR, "xdr_NC_fill") ;
			return(FALSE) ;
		}
		
		return(TRUE) ;
}

