/*****************************************************************************
* 
*			NCSA HDF / netCDF --- PROTOTYPE
*			        June 15, 1992
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
* WARNING:  This is experimental software, while all of the netCDF software
*   tests pass, it is a safe bet that there are still bugs in the code.  
*
* Please report all bugs / comments to chouck@ncsa.uiuc.edu
*
* As this is a prototype implementation we can not guarantee that future
*   releases of NCSA HDF / netCDF software will be backward compatible 
*   with data created with this release.
*
*****************************************************************************/


PRIVATE int32 tBuf_size = 0;
PRIVATE int8  *tBuf = NULL;

/* ---------------------------- hdf_fill_array ---------------------------- */
/*
  Fill the array pointed to by storage with the value pointed to
  by value.  It is assumed that the filling is being done on
  behalf of variable vp which is used to compute the number of 
  fill values to generate.
*/
int 
    hdf_fill_array(vp, storage, value)
NC_var *vp;
Void *storage, *value;
{
    register Void *lo, *hi;
    
    lo = storage;
    hi = storage + vp->len;

    switch(vp->type) {
    case NC_BYTE    :
    case NC_CHAR    :
        { 
            char t = *((char *) value);
            while(lo < hi) *lo++ = t;
        }
        break;
    case NC_SHORT    :
        {
            short t = *((short *) value);
            while(lo < hi) {
                *((short *) lo) = t;
                lo += sizeof(short);
            }
        }
        break;
    case NC_LONG     :
        {
            long t =  *((long *) value);
            while(lo < hi) {
                *((long *) lo) = t;
                lo += sizeof(long);
            }
        }
        break;
    case NC_FLOAT    :
        {
            float t = *((float *) value);
            while(lo < hi) {
                *((float *) lo) = t;
                lo += sizeof(long);
            }
        }
        break;
    case NC_DOUBLE   :
        {
            double t = *((double *) value);
            while(lo < hi) {
                *((double *) lo) = t;
                lo += sizeof(double);
            }
        }
        break;
    }
    
} /* hdf_fill_array */

/* ------------------------- hdf_get_data ------------------- */
/*
 * Given a variable vgid return the id of a valid data storage
 * Create and fill in the VS as a side effect if it doesn't
 *  exist yet
 */
int 
  hdf_get_data(handle, vp, type)
NC *handle;
NC_var *vp;
nc_type type;
{
    NC_attr **attr;
    int32 vg;
    int vsid, nvalues, status, tag, t, n;
    register Void *values;
    int byte_count;
    
#if DEBUG 
    fprintf(stderr, "hdf_get_data I've been called\n");
#endif
    
    if(!handle) return NULL;
    if(!vp) return NULL;

    /* 
     * if it is stored as NDGs we can't do any better than what was
     *    originally stored in vp->data_ref
     */
    if(vp->data_tag == DFTAG_SDS) return vp->data_ref;
    
    if(vp->vgid) {
        /* attach to the variable's Vgroup */
        vg = Vattach(handle->hdf_file, vp->vgid, "w");
        if(vg == FAIL) {
            HEprint(stderr, 0);
            return NULL;
        }
        
        /* loop through looking for a data storage object */
        n = Vntagrefs(vg);
        for(t = 0; t < n; t++) {
            Vgettagref(vg, t, &tag, &vsid);
            if(tag == DATA_TAG) return vsid;
        }
    }
    
    if(handle->hdf_mode == DFACC_RDONLY)
        return NULL;
  
    /* 
     * create a new data storage object
     */
#ifdef DEBUG
    fprintf(stderr, "--- Creating new data storage (len = %d) --- \n", vp->len);
    fprintf(stderr, "shape[0]= %d shape[1]= %d\n", vp->shape[0], vp->shape[1]);
    fprintf(stderr, "dsize[0]= %d dsize[1]= %d\n", vp->dsizes[0], vp->dsizes[1]);
#endif  
    
    /* look up fill value (if it exists) */
    values = (Void *) HDgetspace(vp->len);
    attr = NC_findattr(&(vp->attrs), _FillValue);
    nvalues = vp->len / NC_typelen(vp->type);
    
    if(!attr) {
#ifdef DEBUG
        fprintf(stderr, "No fill value fill with default \n");
#endif
        NC_arrayfill(values, vp->len, vp->type);
    } else {
#ifdef DEBUG
        fprintf(stderr, "Found a fill value (need %d of em)\n", nvalues);
#endif
        hdf_fill_array(vp, values, (*attr)->data->values);
    }
    
    /* --------------------------------------
     *
     * allocate new data storage 
     *
     * --------------------------------------
     */

    byte_count = nvalues * vp->HDFsize;
    vsid = Hnewref(handle->hdf_file);
    vp->aid = Hstartwrite(handle->hdf_file, DATA_TAG, vsid, byte_count);

    if(vp->aid == FAIL) return NULL;

    /* make sure our tmp buffer is big enough to hold everything */
    if(tBuf_size < byte_count) {
        if(tBuf) HDfreespace(tBuf);
        tBuf_size = byte_count;
        tBuf = (int8 *) HDgetspace(tBuf_size);
    }

    /*
     * Do numerical conversions
     */
    DFKsetNT(vp->HDFtype);
    DFKnumout((uint8 *) values, tBuf, (uint32) nvalues, 0, 0);

    status = Hwrite(vp->aid, byte_count, (uint8 *) tBuf);
    if(status != byte_count) return NULL;
    if(Hendaccess(vp->aid) == FAIL) return NULL;

    /* if it is a record var might as well make it linked blocks now */
    if(IS_RECVAR(vp)) {
        vp->aid = HLcreate(handle->hdf_file, DATA_TAG, vsid, 
                           byte_count * 64, 128);
        if(vp->aid == FAIL) return NULL;
        if(Hendaccess(vp->aid) == FAIL) return NULL;
    }

    if(vp->vgid) {
        /* add new Vdata to existing Vgroup */
        Vaddtagref(vg, (int32) DATA_TAG, (int32) vsid);
        
        /* detach from the variable's VGroup --- will no longer need it */
        Vdetach(vg);
    }
        
#if DEBUG 
    fprintf(stderr, "Done with the DATA Vdata returning id %d\n", vsid);
#endif

    vp->aid = NULL;
            
    return vsid;

} /* hdf_get_data */

/* ------------------------- xdr_NCvdata ------------------- */
/*
 *  Read / write 'count' items of contiguous data of type 'type' at 'where'
 *
 * If we are not already attached to the VData representing the
 *  data attach to it now.  Since attaching / detaching is so
 *  slow, stay attached for future reads / writes.  As a result,
 *  we must always attach with write access.
 */
static bool_t
hdf_xdr_NCvdata(handle, vp, where, type, count, values)
NC *handle;
NC_var *vp;
u_long where;
nc_type type;
Void *values;
int count;
{
    int status;
    int byte_count;

#if DEBUG
    fprintf(stderr, "hdf_xdr_NCvdata I've been called : %s\n", vp->name->values);
#endif

    /* map to the correct location */
    where -= vp->begin;

#if DEBUG 
    fprintf(stderr, "Where = %d  count = %d\n", where, count);
#endif
    
    if(!vp->aid) {
        /* attach to proper data storage*/
        /* 
         * BUG: this is going to cause problems if we ever put record
         * data into separate VDatas
         */
        if(!vp->data_ref)
            vp->data_ref = hdf_get_data(handle, vp, type);
        
        /*
         * Fail if there is no data *AND* we were trying to read...
         * Otherwise, we should probabally blank out the storage...
         */
        if(vp->data_ref == NULL) 
            if(handle->hdf_mode == DFACC_RDONLY) {
                if(vp->data_tag == DFTAG_SDS) 
                    HDmemset(values, (char) 0, count * vp->szof);
                return TRUE;
            } else {
                return FALSE;
            }

        if((vp->data_offset < 0) && (handle->hdf_mode == DFACC_RDONLY)) {
            /* blank the memory */
            HDmemset(values, (char) 0, count * vp->szof);
            return TRUE;
        }
        
        if(handle->hdf_mode == DFACC_RDONLY)
            vp->aid = Hstartread(handle->hdf_file, vp->data_tag, vp->data_ref);
        else
            vp->aid = Hstartwrite(handle->hdf_file, vp->data_tag, vp->data_ref, 0);
        
        if(vp->aid == FAIL) {
            HEprint(stderr, 0);
            return(FALSE);
        }
    }
    
    /* 
     * It may be the case that the current does NOT begin at the start of the
     *   data-object which is storing it.  In that case compute the correct 
     *   location.
     */
    if(vp->data_offset > 0) where += vp->data_offset;
    
    /* position ourselves correctly */
    status = Hseek(vp->aid, where, DF_START);
    if(status == FAIL) {
        HEprint(stderr, 0);
        return(FALSE);
    }
    
    /* make sure our tmp buffer is big enough to hold everything */
    byte_count = count * vp->HDFsize;
    if(tBuf_size < byte_count) {
        if(tBuf) HDfreespace(tBuf);
        tBuf_size = byte_count;
        tBuf = (int8 *) HDgetspace(tBuf_size);
        if(tBuf == NULL) return FALSE;
    }
    

    /* Read or write the data into / from values */
    DFKsetNT(vp->HDFtype);
    
    if(handle->xdrs->x_op == XDR_DECODE) {
        status = Hread(vp->aid, byte_count, (uint8 *) tBuf);
        if(status != byte_count) return FALSE;
        
        /* convert tBuf into values */
        DFKnumin((uint8 *) tBuf, (uint8 *) values, (uint32) count, 0, 0);
        
    } else {
        
        /*  convert values into tBuf */
        DFKnumout((uint8 *) values, tBuf, (uint32) count, 0, 0);
        
        status = Hwrite(vp->aid, byte_count, (uint8 *) tBuf);
        if(status != byte_count) return FALSE;
    }
    
#if 0

    /*
     * Just do the I/O let the level above us worry about converting
     *  the data
     */
    if(handle->xdrs->x_op == XDR_DECODE) {
        status = Hread(vp->aid, byte_count, (uint8 *) values);
        if(status != byte_count) return FALSE;
    } else {
        status = Hwrite(vp->aid, byte_count, (uint8 *) values);
        if(status != byte_count) return FALSE;
    }
 
#endif

#ifdef DEBUG
    fprintf(stderr, " * * * Done with call to xdr_NCvdata * * *\n");
#endif
    
    return(TRUE);
    
} /* xdr_NCvdata */


/* ------------------------- xdr_NCv1data ------------------- */
/*
 * read / write a single datum of type 'type' at 'where'
 * This is designed to replace the xdr based routine of the
 *  similar name
 * Return TRUE if everything worked, else FALSE
 */
static bool_t
  hdf_xdr_NCv1data(handle, vp, where, type, values)
NC *handle;
NC_var *vp;
u_long where;
nc_type type;
Void *values;
{

#if 0

    intn status;
    uint8 buf[100];
    
    /* Read or write the data into / from values */
    DFKsetNT(vp->HDFtype);
    
    if(handle->xdrs->x_op == XDR_DECODE) {

        if(!(hdf_xdr_NCvdata(handle, vp, where, type, 1, buf)))
            return FALSE;

        /* convert buf into values */
        DFKnumin((uint8 *) buf, (uint8 *) values, 1, 0, 0);
        
    } else {
        
        /*  convert values into tBuf */
        DFKnumout((uint8 *) values, buf, 1, 0, 0);

        if(!(hdf_xdr_NCvdata(handle, vp, where, type, 1, buf)))
            return FALSE;

    }
    
    return TRUE;

#endif
    
    return (hdf_xdr_NCvdata(handle, vp, where, type, 1, values)); 

} /* xdr_NCv1data */



