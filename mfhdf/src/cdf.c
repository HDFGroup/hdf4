/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Copyright 1993, University Corporation for Atmospheric Research           *
 * See netcdf/COPYRIGHT file for copying and redistribution conditions.      *
 *                                                                           *
 * Copyright by The HDF Group.                                               *
 * Copyright by the Board of Trustees of the University of Illinois.         *
 * All rights reserved.                                                      *
 *                                                                           *
 * This file is part of HDF.  The full HDF copyright notice, including       *
 * terms governing use, modification, and redistribution, is contained in    *
 * the COPYING file, which can be found at the root of the source code       *
 * distribution tree, or in https://support.hdfgroup.org/ftp/HDF/releases/.  *
 * If you do not have access to either file, you may request a copy from     *
 * help@hdfgroup.org.                                                        *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "nc_priv.h"
#include "herr_priv.h"

#include "hfile_priv.h"

int32 hdf_get_magicnum(const char *filename);

static int hdf_num_attrs(NC   *handle, /* IN: handle to SDS */
                         int32 vg /* IN: ref of top Vgroup */);

static bool_t NC_xdr_cdf(XDR *xdrs, NC **handlep);

static int NC_free_xcdf(NC *);

/* hmm we write the NDG out always for now */
#define WRITE_NDG 1

/*
 * Free the resources that xdr_cdf allocates
 */
static int
NC_free_xcdf(NC *handle)
{
    int ret_value = SUCCEED;

    if (handle != NULL) {
        if (NC_free_array(handle->dims) == FAIL)
            HGOTO_FAIL(FAIL);
        if (NC_free_array(handle->attrs) == FAIL)
            HGOTO_FAIL(FAIL);
        if (NC_free_array(handle->vars) == FAIL)
            HGOTO_FAIL(FAIL);
    }

done:
    return ret_value;
}

int
NC_free_cdf(NC *handle)
{
    int ret_value = SUCCEED;

    if (handle != NULL) {
        if (NC_free_xcdf(handle) == FAIL)
            HGOTO_FAIL(FAIL);

        /* destroy xdr struct */
        h4_xdr_destroy(handle->xdrs);
        free(handle->xdrs);
        handle->xdrs = NULL;

        if (handle->file_type == HDF_FILE) {
            if (Vend(handle->hdf_file) == FAIL)
                HGOTO_FAIL(FAIL);

            if (Hclose(handle->hdf_file) == FAIL)
                HGOTO_FAIL(FAIL);
        }

        free(handle);
        handle = NULL;
    }

done:
    return ret_value;
}

/* --------------------------- hdf_get_magicnum ---------------------------- */
/*
  Return the file's format version number, i.e., magic number.  This number
  can be used to determine the format type of a file, such as HDF, CDF, or
  netCDF/64-bit.
*/
int32
hdf_get_magicnum(const char *filename)
{
    hdf_file_t fp;
    uint8      buf[4];
    uint8     *pbuf = NULL;
    int32      magic_num;
    int32      ret_value = 0;

    fp = (hdf_file_t)HI_OPEN(filename, DFACC_READ);
    if (OPENERR(fp)) {
        HGOTO_ERROR(DFE_BADNAME, FAIL);
    }

    /* Make sure it is at the beginning of the file */
    if (HI_SEEK(fp, MAGICOFFSET) == FAIL) {
        HGOTO_ERROR(DFE_SEEKERROR, FAIL);
    }

    /* Read the first 4 bytes in the file, where the format version number
       is stored. */
    if (HI_READ(fp, buf, MAGICLEN) == FAIL) {
        HI_CLOSE(fp);
        HGOTO_ERROR(DFE_READERROR, FAIL);
    }

    /* Obtain the file format version number then close the file*/
    pbuf = &buf[0];
    INT32DECODE(pbuf, magic_num);
    HI_CLOSE(fp);

    /* If magic_num is a valid file format version number, then return it */
    if (magic_num == HDFXMAGIC || magic_num == CDFMAGIC || magic_num == NCMAGIC || magic_num == NCMAGIC64)
        ret_value = magic_num;
    else
        HGOTO_ERROR(DFE_INVFILE, FAIL);
done:
    return ret_value;
} /* hdf_get_magicnum */

/* -------------------------------- HDiscdf -------------------------------- */
/*
  Return TRUE/FALSE depending on if the given file is a NASA CDF file
*/
int
HDiscdf(const char *filename)
{
    int32 magic_num = 0;
    int   ret_value = FALSE;

    /* Use internal function to open the file and get a magic number if the
       file has one */
    magic_num = hdf_get_magicnum(filename);

    /* A CDF file would have CDFMAGIC at the beginning */
    if (magic_num == CDFMAGIC)
        ret_value = TRUE;
    else
        ret_value = FALSE;

    return ret_value;
}

/* -------------------------------- HDisnetcdf --------------------------------

  Return TRUE if the given file is a netCDF file, FALSE otherwise.
*/
int
HDisnetcdf(const char *filename)
{
    int32 magic_num = 0;
    int   ret_value = FALSE;

    /* Use internal function to open the file and get a magic number if the
       file has one */
    magic_num = hdf_get_magicnum(filename);

    /* A classic netCDF file would have NCMAGIC at the beginning */
    if (magic_num == NCMAGIC)
        ret_value = TRUE;
    else
        ret_value = FALSE;

    return ret_value;
} /* HDisnetcdf */

/* ------------------------------ HDisnetcdf64 --------------------------------

  Return TRUE if the given file is a netCDF 64-bit file, FALSE otherwise.
*/
int
HDisnetcdf64(const char *filename)
{
    int32 magic_num = 0;
    int   ret_value = FALSE;

    /* Use internal function to open the file and get a magic number if the
       file has one */
    magic_num = hdf_get_magicnum(filename);

    /* A 64-bit netCDF file would have NCMAGIC64 at the beginning */
    if (magic_num == NCMAGIC64)
        ret_value = TRUE;
    else
        ret_value = FALSE;

    return ret_value;
} /* HDisnetcdf64 */

/******************************************************************************/

NC *
NC_new_cdf(const char *name, int mode)
{
    int32 hdf_mode  = DFACC_RDWR; /* default */
    NC   *cdf       = NULL;
    NC   *ret_value = NULL;

    /* allocate an NC struct */
    cdf = calloc(1, sizeof(NC));
    if (cdf == NULL) {
        nc_serror("NC_new_cdf");
        HGOTO_FAIL(NULL);
    }

    cdf->flags = mode;

    /*
     * See what type of file we are looking at.
     * If we are creating a new file it will be an HDF file
     */
    if (mode & NC_CREAT) {
        cdf->file_type = HDF_FILE;
    }
    else {
        if (Hishdf(name))
            cdf->file_type = HDF_FILE;
        else if (HDiscdf(name))
            cdf->file_type = CDF_FILE;
        else if (HDisnetcdf(name))
            cdf->file_type = netCDF_FILE;
        else
            HGOTO_FAIL(NULL);
    }

    /* Delay allocating xdr struct until it is needed */
    cdf->xdrs = calloc(1, sizeof(XDR));
    if (cdf->xdrs == NULL) {
        nc_serror("NC_new_cdf: xdrs");
        HGOTO_FAIL(NULL);
    }

    /*
     * Set up the XDR functions that some of the netCDF old code uses
     */
    switch (cdf->file_type) {
        case HDF_FILE:
            h4_xdr_setup_nofile(cdf->xdrs, mode); /* return type is 'void' */
            break;
        case netCDF_FILE:
            if (NCxdrfile_create(cdf->xdrs, name, mode) < 0)
                HGOTO_FAIL(NULL);
            break;
        case CDF_FILE:
            /* CDF_xdrfile_create(); */
            /* try this, I bet it will be sufficient */
            h4_xdr_setup_nofile(cdf->xdrs, mode);
            break;
    }

    cdf->dims      = NULL;
    cdf->attrs     = NULL;
    cdf->vars      = NULL;
    cdf->begin_rec = 0;
    cdf->recsize   = 0;
    cdf->numrecs   = 0;
    cdf->redefid   = -1;

    /*
     * determine the HDF access mode
     */
    switch (mode) {
        case NC_CLOBBER:
            hdf_mode = DFACC_CLOBBER;
            break;
        case NC_NOCLOBBER:
            /* will handle below */
            break;
        case NC_WRITE:
            hdf_mode = DFACC_RDWR;
            break;
        case NC_NOWRITE:
            hdf_mode = DFACC_RDONLY;
            break;
        default:
            hdf_mode = DFACC_RDWR;
    }

    /*
     * Do file type specific setup
     */
    switch (cdf->file_type) {
        case HDF_FILE: /* HDF stuff */

            /* see if the file exists */
            if (mode == NC_NOCLOBBER) {
                if ((int)Hishdf(name)) { /* Need to free allocated structures.
                                            This will happen on failure cleanup. */
                    HGOTO_FAIL(NULL);
                }
                hdf_mode = DFACC_RDWR;
            }

            /* open the file */
            cdf->hdf_file = (int32)Hopen(name, hdf_mode, 200);
            if (cdf->hdf_file == FAIL)
                HGOTO_FAIL(NULL);

            /* start Vxx access */
            if (Vstart(cdf->hdf_file) == FAIL)
                HGOTO_FAIL(NULL);

            cdf->hdf_mode = hdf_mode;
            cdf->vgid     = 0; /* invalid ref */

            /* copy filename only up to its length instead of FILENAME_MAX as
               used to be */
            strncpy(cdf->path, name, strlen(name) + 1);
            cdf->path[strlen(name)] = '\0';
            break;
        case netCDF_FILE:
            /* Nothing */
            strncpy(cdf->path, name, strlen(name) + 1);
            cdf->path[strlen(name)] = '\0';
            break;
        case CDF_FILE:
            cdf->cdf_fp = (hdf_file_t)HI_OPEN(name, hdf_mode);
            if (OPENERR(cdf->cdf_fp))
                HRETURN_ERROR(DFE_DENIED, NULL);
            break;
    }

    /*
     * Read in the contents
     */
    if (cdf->xdrs->x_op == XDR_DECODE) /* Not NC_CREAT */
    {
        if (!xdr_cdf(cdf->xdrs, &cdf)) { /* free cdf struct. This cleanup is different than
                                            NC_free_xcdf(). */
            NC_free_cdf(cdf);            /* free memory, close structures,files etc*/
            cdf = NULL;
            HGOTO_FAIL(NULL);
        }

        if (NC_computeshapes(cdf) == -1)
            HGOTO_FAIL(NULL);
    }

    ret_value = cdf;

done:
    if (ret_value == NULL) {

        /* Handles case other than one for NC_free_cdf().
         * These routines only free up allocated memory.
         */
        if (cdf != NULL) {
            NC_free_xcdf(cdf); /* no point in catching error here */
            if (cdf->xdrs != NULL) {
                h4_xdr_destroy(cdf->xdrs);
                free(cdf->xdrs);
            }
            free(cdf);
        }
    }

    return ret_value;
} /* NC_new_cdf */

/*
 * Duplicate a description structure.
 * Can only be called for 'old' extant on disk, eg, old in DATA mode.
 */
NC *
NC_dup_cdf(const char *name, int mode, NC *old)
{
    NC *cdf       = NULL;
    NC *ret_value = NULL;

    cdf = malloc(sizeof(NC));
    if (cdf == NULL) {
        nc_serror("NC_dup_cdf");
        HGOTO_FAIL(NULL);
    }

    cdf->flags = old->flags | NC_INDEF;

    cdf->xdrs = malloc(sizeof(XDR));
    if (cdf->xdrs == NULL) {
        nc_serror("NC_dup_cdf: xdrs");
        HGOTO_FAIL(NULL);
    }

    cdf->dims      = NULL;
    cdf->attrs     = NULL;
    cdf->vars      = NULL;
    cdf->begin_rec = 0;
    cdf->recsize   = 0;
    cdf->numrecs   = 0;

    cdf->file_type = old->file_type;

    if (NCxdrfile_create(cdf->xdrs, name, mode) < 0)
        HGOTO_FAIL(NULL);

    old->xdrs->x_op = XDR_DECODE;
    if (!xdr_cdf(old->xdrs, &cdf))
        HGOTO_FAIL(NULL);
    if (NC_computeshapes(cdf) == -1)
        HGOTO_FAIL(NULL);

    ret_value = cdf;

done:
    if (ret_value == NULL) {
        if (cdf != NULL) { /* free up allocated structures */
            free(cdf->xdrs);

            NC_free_xcdf(cdf); /* don't catch error here */
            free(cdf);
        }
    }

    return ret_value;
}

int
ncinquire(int cdfid, int *ndimsp, int *nvarsp, int *nattrsp, int *xtendimp)
{
    NC *handle;

    cdf_routine_name = "ncinquire";

    handle = NC_check_id(cdfid);
    if (handle == NULL)
        return -1;

    if (nvarsp != NULL)
        *nvarsp = (handle->vars != NULL) ? handle->vars->count : 0;
    if (nattrsp != NULL)
        *nattrsp = (handle->attrs != NULL) ? handle->attrs->count : 0;
    if (handle->dims != NULL) {
        NC_dim **dp;

        if (ndimsp != NULL)
            *ndimsp = handle->dims->count;
        if (xtendimp != NULL) {
            *xtendimp = -1;

            dp = (NC_dim **)handle->dims->values;
            for (unsigned ii = 0; ii < handle->dims->count; ii++, dp++) {
                if ((*dp)->size == NC_UNLIMITED) {
                    *xtendimp = ii;
                }
            }
        }
    }
    else {
        if (ndimsp != NULL)
            *ndimsp = 0;
        if (xtendimp != NULL)
            *xtendimp = -1;
    }

    return cdfid;
}

bool_t
xdr_cdf(XDR *xdrs, NC **handlep)
{
    bool_t ret_value = TRUE;

    switch ((*handlep)->file_type) {
        case HDF_FILE:
            if (hdf_xdr_cdf(xdrs, handlep) == FAIL)
                ret_value = FALSE;
            break;
        case netCDF_FILE:
            ret_value = NC_xdr_cdf(xdrs, handlep);
            break;
        case CDF_FILE:
            ret_value = nssdc_xdr_cdf(xdrs, handlep);
            break;
        default:
            ret_value = FALSE;
            break;
    }

    return ret_value;
}

static bool_t
NC_xdr_cdf(XDR *xdrs, NC **handlep)
{
    unsigned magic;

    if (xdrs->x_op == XDR_FREE) {
        NC_free_xcdf(*handlep);
        return TRUE;
    }

    if (h4_xdr_getpos(xdrs) != 0) {
        if (!h4_xdr_setpos(xdrs, 0)) {
            nc_serror("Can't set position to begin");
            return FALSE;
        }
    }

    /* magic number */
    if (!h4_xdr_u_int(xdrs, &magic)) {
        if (xdrs->x_op == XDR_DECODE) {
            NCadvise(NC_ENOTNC, "Not a netcdf file (Can't read magic number)");
        }
        else {
            /* write error */
            nc_serror("xdr_cdf: h4_xdr_u_int");
        }
        return FALSE;
    }

    if (xdrs->x_op == XDR_DECODE && magic != NCMAGIC) {
        if (magic == NCLINKMAGIC) {
            NCadvise(NC_NOERR, "link file not handled yet");
            return FALSE;
        }
        NCadvise(NC_ENOTNC, "Not a netcdf file");
        return FALSE;
    }

    if (!xdr_numrecs(xdrs, *handlep)) {
        NCadvise(NC_EXDR, "xdr_numrecs");
        return FALSE;
    }
    if (!xdr_NC_array(xdrs, &((*handlep)->dims))) {
        NCadvise(NC_EXDR, "xdr_cdf:dims");
        return FALSE;
    }
    if (!xdr_NC_array(xdrs, &((*handlep)->attrs))) {
        NCadvise(NC_EXDR, "xdr_cdf:attrs");
        return FALSE;
    }
    if (!xdr_NC_array(xdrs, &((*handlep)->vars))) {
        NCadvise(NC_EXDR, "xdr_cdf:vars");
        return FALSE;
    }
    return TRUE;
}

/*****************************************************************************
 *
 *            NCSA HDF / netCDF Project
 *                    May, 1993
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
 * Please report all bugs / comments to help@hdfgroup.org
 *
 *****************************************************************************/

/* ----------------------------------------------------------------
** Map an NC_<type> to an HDF type
*/
int
hdf_map_type(nc_type type)
{

    switch (type) {
        case NC_UNSPECIFIED: /* be like netCDF */
        case NC_CHAR:
            return DFNT_CHAR;
        case NC_BYTE:
            return DFNT_INT8;
        case NC_SHORT:
            return DFNT_INT16;
        case NC_LONG:
            return DFNT_INT32;
        case NC_FLOAT:
            return DFNT_FLOAT32;
        case NC_DOUBLE:
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
nc_type
hdf_unmap_type(int type)
{
    switch (type & 0xff) {
        case DFNT_CHAR:
        case DFNT_UCHAR:
            return NC_CHAR;
        case DFNT_INT8:
        case DFNT_UINT8:
            return NC_BYTE;
        case DFNT_INT16:
        case DFNT_UINT16:
            return NC_SHORT;
        case DFNT_INT32:
        case DFNT_UINT32:
            return NC_LONG;
        case DFNT_FLOAT32:
            return NC_FLOAT;
        case DFNT_FLOAT64:
            return NC_DOUBLE;
        default:
            return (nc_type)FAIL; /* need a better legal nc_type value */
    }

} /* hdf_unmap_type */

/* -----------------------------------------------------------------
** Given a dimension id number return its hdf_ref number (Vgroup id)
*/
int
hdf_get_ref(NC *handle, int i)
{
    NC_array *tmp  = NULL;
    NC_dim  **d    = NULL;
    uint8_t  *dims = NULL;

    tmp  = handle->dims;
    dims = handle->dims->values;
    dims += i * tmp->szof;

    d = (NC_dim **)dims;
    return (*d)->vgid; /* return ref of vgroup */
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
**
** NOTE2: Someone should update the comments here. They no longer
**        seem valid -GV 9/19/97
*/
int
hdf_create_dim_vdata(XDR *xdrs, NC *handle, NC_dim *dim)
{
    int   ref;
    int32 val;
    long  dsize;
    int   ret_value = FAIL;

    (void)xdrs;

    dsize = 1;
    val   = (dim->size != NC_UNLIMITED) ? dim->size : (int32)handle->numrecs;
    ref   = VHstoredata(handle->hdf_file, "Values", (const uint8 *)&val, dsize, DFNT_INT32, dim->name->values,
                        DIM_VALS01);

    if (ref == FAIL) {
        HGOTO_FAIL(FAIL);
    }

    ret_value = ref;

done:
    return ret_value;
} /* hdf_create_dim_vdata */

/* ----------------------------------------------------------------
** Given a dimension pointer return the ref of an older version
**   dim Vdata which was newly created to represent the values
**   the dimension takes on, for backward compatibility.
**
**   For DIM_VALS, the values the variable takes on are the values
**     for the 'steps' in the dimension.
**
*/
int
hdf_create_compat_dim_vdata(XDR *xdrs, NC *handle, NC_dim *dim, int32 dimval_ver)
{
    int    i;
    int    ref;
    long   dsize;
    int32 *val       = NULL;
    int    ret_value = FAIL;

    (void)xdrs;

    if (dimval_ver != DIMVAL_VERSION00)
        HGOTO_FAIL(FAIL);

    dsize = (dim->size == NC_UNLIMITED) ? 1 : dim->size;
    if (dsize < 0)
        HGOTO_FAIL(FAIL);
    /* create a fake one */
    /* allocate space */
    val = malloc((size_t)dsize * sizeof(int32));
    if (!val)
        HGOTO_ERROR(DFE_NOSPACE, FAIL);

    /* Shouldn't a check for netCDF file be here before using handle->numrecs?
       If the file is an HDF file, vp->numrecs should be used, right?
       -BMR (2013-6/24) */
    if (dim->size == NC_UNLIMITED) {
        *val = handle->numrecs;
    }
    else {
        for (i = 0; i < dsize; i++)
            val[i] = i;
    }

    ref = VHstoredata(handle->hdf_file, "Values", (const uint8 *)val, dsize, DFNT_INT32, dim->name->values,
                      DIM_VALS);
    if (ref == FAIL) {
        HGOTO_FAIL(FAIL);
    }

    ret_value = ref;

done:
    free(val);

    return ret_value;
} /* hdf_create_compat_dim_vdata */

/* ----------------------------------------------------------------
** Write out a vdata representing an attribute
*/
int
hdf_write_attr(XDR *xdrs, NC *handle, NC_attr **attr)
{
    char    *name   = NULL;
    uint8_t *values = NULL;
    int      size;
    int      type;
    int      order;
    int      ret_value = SUCCEED;

    (void)xdrs;

    name   = (*attr)->name->values;
    values = (*attr)->data->values;
    size   = (*attr)->data->count;
    type   = (*attr)->HDFtype;

    if (type == DFNT_CHAR) {
        order = size;
        size  = 1;
    }
    else {
        order = 1;
    }

    ret_value = VHstoredatam(handle->hdf_file, ATTR_FIELD_NAME, (unsigned char *)values, size, type, name,
                             _HDF_ATTRIBUTE, order);

    return ret_value;
} /* hdf_write_attr */

/* ----------------------------------------------------------------
** Write out a group representing a dimension
*/
int32
hdf_write_dim(XDR *xdrs, NC *handle, NC_dim **dim, int32 cnt)
{
    int32 tags[100];
    int32 refs[100];
    int32 count;
    const char *class          = NULL;
    char  name[H4_MAX_NC_NAME] = "";
    int32 ret_value            = SUCCEED;

    /*
     * Look up to see if there is a variable of the same name
     *  giving values
     */
    count       = 0;
    tags[count] = DFTAG_VH;
    refs[count] = hdf_create_dim_vdata(xdrs, handle, (*dim));
    if (refs[count] == FAIL)
        HGOTO_FAIL(FAIL);
    count++;

    /* do we need to create compatible dimension? */
    if ((*dim)->dim00_compat) {
        tags[count] = DFTAG_VH;
        refs[count] = hdf_create_compat_dim_vdata(xdrs, handle, (*dim), DIMVAL_VERSION00);
        if (refs[count] == FAIL)
            HGOTO_FAIL(FAIL);
        count++;
    }

    /* check if UNLIMITED dimension */
    if ((*dim)->size == NC_UNLIMITED)
        class = _HDF_UDIMENSION;
    else
        class = _HDF_DIMENSION;

    if (strncmp((*dim)->name->values, "fakeDim", 7) == 0)
        sprintf(name, "fakeDim%d", (int)cnt);
    else
        strcpy(name, (*dim)->name->values);

    /* write out the dimension group? */
    (*dim)->vgid = VHmakegroup(handle->hdf_file, tags, refs, count, name, class);

    ret_value = (*dim)->vgid; /* ref of vgroup of dimension */

done:

    return ret_value;
} /* hdf_write_dim */

/* ----------------------------------------------------------------
** Write out a group representing a variable
** If successful, return the id of the created Vgroup else
**  return NULL
*/
int32
hdf_write_var(XDR *xdrs, NC *handle, NC_var **var)
{
    NC_array  *attrs = NULL;
    NC_iarray *assoc = NULL;
    uint8      ntstring[4];
    uint16     ref;
    int8       outNT;
    uint8      tbuf[2 + ((H4_MAX_VAR_DIMS + 1) * 8)]; /* temporary buffer */
    int32      tags[H4_MAX_NC_ATTRS + H4_MAX_VAR_DIMS + 2];
    int32      refs[H4_MAX_NC_ATTRS + H4_MAX_VAR_DIMS + 10];
    uint16     nt_ref, rank;
    int32      GroupID, val, vs_id;
    uint8     *bufp      = NULL;
    int32      ret_value = SUCCEED;
    unsigned   i, count;
    uint8_t   *attribute = NULL;

    count = 0;
    assoc = (*var)->assoc;
    attrs = (*var)->attrs;

    /*
     *  Get the dimension information
     */
    for (i = 0; i < assoc->count; i++) {
        tags[count] = DIM_TAG;
        refs[count] = hdf_get_ref(handle, assoc->values[i]);
        count++;
    }

    /*
     * Add info for the attributes
     */
    if (attrs) {
        attribute = attrs->values;
        for (i = 0; i < attrs->count; i++) {
            tags[count] = ATTR_TAG;
            refs[count] = hdf_write_attr(xdrs, handle, (NC_attr **)attribute);
            if (refs[count] == FAIL)
                HGOTO_FAIL(FAIL);
            attribute += attrs->szof;
            count++;
        }
    }

    /*
     *  Add info about the type of this variable
     */
    if ((*var)->var_type == IS_SDSVAR || (*var)->var_type == IS_CRDVAR) {
        char fields[FIELDNAMELENMAX];
        char vsclass[H4_MAX_NC_NAME];

        if ((*var)->var_type == IS_SDSVAR) {
            strncpy(fields, "SDS variable", 13);
            strncpy(vsclass, _HDF_SDSVAR, 7);
        }
        else if ((*var)->var_type == IS_CRDVAR) {
            strncpy(fields, "Coordinate variable", 20);
            strncpy(vsclass, _HDF_CRDVAR, 9);
        }

        if ((vs_id = VSattach(handle->hdf_file, -1, "w")) == FAIL)
            HGOTO_FAIL(FAIL);
        if (VSsetclass(vs_id, vsclass) == FAIL)
            HGOTO_FAIL(FAIL);

        if (VSfdefine(vs_id, fields, DFNT_FLOAT32, 1) == FAIL)
            HGOTO_FAIL(FAIL);
        if (VSsetfields(vs_id, fields) == FAIL)
            HGOTO_FAIL(FAIL);

        ref = VSQueryref(vs_id);

        if (VSdetach(vs_id) == FAIL)
            HGOTO_FAIL(FAIL);

        tags[count] = DFTAG_VH;
        refs[count] = ref;
        count++;
    }

    /*
     * If we already have data written out include that too
     *   (this might happen after a redef() cuz we will leave
     *   the data sitting on the disk but clear out all the
     *   meta-data)
     */
    if ((*var)->data_ref) {
        tags[count] = (int32)DFTAG_SD;
        refs[count] = (*var)->data_ref;
        count++;
    }

    /*
     * Write out a number type tag so that we can recover this
     *   variable's type later on
     *
     * by default numbers are converted to IEEE otherwise we need to save the
     *   machine type in the NT object
     */

    /* someone unwrap this statement....*/
    outNT = ((*var)->HDFtype & DFNT_NATIVE)   ? DFKgetPNSC((*var)->HDFtype, DF_MT)
            : ((*var)->HDFtype & DFNT_LITEND) ? DFNTF_PC
                                              : DFNTF_IEEE;

    ref         = Hnewref(handle->hdf_file);
    ntstring[0] = DFNT_VERSION;                    /* version */
    ntstring[1] = (uint8)((*var)->HDFtype & 0xff); /* type */
    ntstring[2] = (uint8)((*var)->HDFsize * 8);    /* width (in bits) */
    ntstring[3] = outNT;                           /* class: IEEE or machine class */
    if (Hputelement(handle->hdf_file, DFTAG_NT, ref, ntstring, (int32)4) == FAIL)
        HGOTO_FAIL(FAIL);
    tags[count] = DFTAG_NT;
    refs[count] = ref;
    nt_ref      = (uint16)ref;
    count++;

#ifdef WRITE_NDG
    /* prepare to start writing ndg   */
    if ((GroupID = DFdisetup(10)) < 0)
        HGOTO_FAIL(FAIL);

    /* write SD record */
    if ((*var)->data_ref) {
        if (DFdiput(GroupID, DFTAG_SD, (uint16)(*var)->data_ref) == FAIL)
            HGOTO_FAIL(FAIL);
    }

    /* write NT tag/ref */
    if (DFdiput(GroupID, DFTAG_NT, (uint16)ref) == FAIL)
        HGOTO_FAIL(FAIL);

    /* put rank & dimensions in buffer */
    bufp = tbuf;
    rank = assoc->count;
    UINT16ENCODE(bufp, rank);
    for (i = 0; i < (int)rank; i++) {
        val = (int32)(*var)->shape[i];

        /* need to fake the size of the record dimension */
        if (val == NC_UNLIMITED) {
            if (handle->file_type == HDF_FILE)
                val = (*var)->numrecs;
            else
                val = handle->numrecs;
        }

        INT32ENCODE(bufp, val);
    }

    /* "<=" used to put 1 data NT + rank scale NTs in buffer */
    for (i = 0; i <= (int)rank; i++) { /* scale NTs written even if no scale!*/
        UINT16ENCODE(bufp, DFTAG_NT);
        UINT16ENCODE(bufp, nt_ref);
    }

    /* write out SDD record */
    if (Hputelement(handle->hdf_file, DFTAG_SDD, ref, tbuf, (int32)(bufp - tbuf)) == FAIL)
        HGOTO_FAIL(FAIL);

    /* write dimension record tag/ref */
    if (DFdiput(GroupID, DFTAG_SDD, (uint16)ref) == FAIL)
        HGOTO_FAIL(FAIL);

    tags[count] = DFTAG_SDD;
    refs[count] = ref;
    count++;

    /* Add a bogus tag so we know this NDG is really a variable */
    if (DFdiput(GroupID, BOGUS_TAG, (uint16)ref) == FAIL)
        HGOTO_FAIL(FAIL);

    /* write out NDG */
    if (DFdiwrite(handle->hdf_file, GroupID, DFTAG_NDG, (*var)->ndg_ref) < 0)
        HGOTO_FAIL(FAIL);

    tags[count] = DFTAG_NDG;
    refs[count] = (*var)->ndg_ref;
    count++;

#endif /* WRITE_NDG */

    /* write the vgroup for the coordinate variable */
    (*var)->vgid = VHmakegroup(handle->hdf_file, tags, refs, count, (*var)->name->values, _HDF_VARIABLE);

    ret_value = (*var)->vgid; /* ref of vgroup of variable */

done:
    return ret_value;
} /* hdf_write_var */

/* ----------------------------------------------------------------
** Write out a cdf structure
*/
int
hdf_write_xdr_cdf(XDR *xdrs, NC **handlep)
{
    int32     count;
    int       status, done;
    unsigned  sz, i, j;
    int32    *tags           = NULL;
    int32    *refs           = NULL;
    NC_dim  **dims           = NULL;
    NC_dim  **dims1          = NULL;
    NC_array *tmp            = NULL;
    long     *dim_size_array = NULL;
    long     *tsizeptr       = NULL;
    long      tsize;
    uint32   *dim_hash_array = NULL;
    uint32   *thashptr       = NULL;
    uint32    thash;
    uint8_t  *vars      = NULL;
    uint8_t  *attrs     = NULL;
    int       ret_value = SUCCEED;

    /* Convert old scales into coordinate var values before writing
       out any header info */
    status = hdf_conv_scales(handlep);
    if (status == FAIL)
        HGOTO_FAIL(FAIL);

    /* count size of tag / ref arrays */
    sz = 0;
    if ((*handlep)->dims)
        sz += (*handlep)->dims->count;
    if ((*handlep)->vars)
        sz += (*handlep)->vars->count;
    if ((*handlep)->attrs)
        sz += (*handlep)->attrs->count;

    /* allocate tag / ref arrays */
    tags = malloc(sz * sizeof(int32) + 1);
    refs = malloc(sz * sizeof(int32) + 1);
    if (NULL == tags || NULL == refs) {
        HGOTO_FAIL(FAIL);
    }

    /*
    ** write out dimension arrays
    */
    count = 0;
    if ((*handlep)->dims) {
        tmp  = (*handlep)->dims;
        dims = (NC_dim **)(*handlep)->dims->values;

        tsizeptr = dim_size_array = malloc(sizeof(long) * (size_t)tmp->count);
        thashptr = dim_hash_array = malloc(sizeof(uint32) * (size_t)tmp->count);

        if (NULL == dim_size_array || NULL == dim_hash_array) {
            HGOTO_FAIL(FAIL);
        }

        for (i = 0; i < tmp->count; i++, dims++) {
            *tsizeptr++ = (*dims)->size;
            *thashptr++ = (*dims)->name->hash;
        } /* end for */

        dims = (NC_dim **)(*handlep)->dims->values;
        for (i = 0; i < tmp->count; i++) {

            /* this is really ugly and should be handled another way */
            /* make sure we don't duplicate dimensions */
            done     = FALSE;
            dims1    = (NC_dim **)(*handlep)->dims->values;
            tsize    = dim_size_array[i];
            thash    = dim_hash_array[i];
            tsizeptr = dim_size_array;
            thashptr = dim_hash_array;

            for (j = 0; j < i; j++) {
                /* this order on the test is faster -QAK */
                if (thash == *thashptr && tsize == *tsizeptr &&
                    NC_compare_string((*dims)->name, (*dims1)->name) == 0) {
                    done = TRUE;
                    break;
                }
                tsizeptr++;
                thashptr++;
                dims1++;
            }

            if (!done) {
                tags[count] = (int32)DIM_TAG;
                refs[count] = (int32)hdf_write_dim(xdrs, (*handlep), dims, count);
                if (refs[count] == FAIL)
                    HGOTO_FAIL(FAIL);
                count++;
            }
            dims++;
        }
    } /* end if handle->dims */

    /*
    ** write out variable info
    */
    if ((*handlep)->vars) {
        tmp  = (*handlep)->vars;
        vars = (*handlep)->vars->values;
        for (i = 0; i < tmp->count; i++) {
            tags[count] = (int32)VAR_TAG;
            refs[count] = (int32)hdf_write_var(xdrs, (*handlep), (NC_var **)vars);
            if (refs[count] == FAIL)
                HGOTO_FAIL(FAIL);
            vars += tmp->szof;
            count++;
        }
    }

    /*
     * write global attribute information
     */
    if ((*handlep)->attrs) {
        tmp   = (*handlep)->attrs;
        attrs = (*handlep)->attrs->values;
        for (i = 0; i < tmp->count; i++) {
            tags[count] = (int32)ATTR_TAG;
            refs[count] = (int32)hdf_write_attr(xdrs, (*handlep), (NC_attr **)attrs);
            if (refs[count] == FAIL)
                HGOTO_FAIL(FAIL);
            attrs += tmp->szof;
            count++;
        }
    }

    /* write out final VGroup thang */
    /* set the top level CDF VGroup pointer */
    (*handlep)->vgid = VHmakegroup((*handlep)->hdf_file, tags, refs, count, (*handlep)->path, _HDF_CDF);

    ret_value = (*handlep)->vgid; /* ref of final vgroup  */

done:
    free(dim_size_array);
    free(dim_hash_array);
    free(tags);
    free(refs);

    return ret_value;
} /* hdf_write_xdr_cdf */

/* --------------------------------------------------------------
** hdf_conv_scales converts old scale values into coord var values.
** Searches through var list for DFTAG_SDS, reads in the scale data,
** change the ref to ndg_ref and the tag to DATA_TAG, writes the
** data out.
*/
int
hdf_conv_scales(NC **handlep)
{
    int       status, scaleref, scaletag, scalelen;
    unsigned  i;
    NC_var  **vars      = NULL;
    NC_array *tmp       = NULL;
    uint8    *scalebuf  = NULL;
    uint8    *datap     = NULL;
    int       ret_value = SUCCEED;

    if ((*handlep)->vars) {
        tmp  = (*handlep)->vars;
        vars = (NC_var **)tmp->values;
        for (i = 0; i < tmp->count; i++) {
            if (((*vars)->data_tag == DFTAG_SDS) && ((*vars)->data_ref != (*vars)->ndg_ref)) {
                /* read in scale values */
                scaleref = (*vars)->data_ref;
                scaletag = (*vars)->data_tag;
                scalelen = Hlength((*handlep)->hdf_file, scaletag, scaleref);
                if (scalelen == FAIL)
                    HGOTO_FAIL(FAIL);
                if ((*vars)->data_offset == -1) { /* this dim has no scale values */
                    (*vars)->data_ref = 0;
                    (*vars)->data_tag = DATA_TAG;
                }
                else { /* has scale values */
                    scalebuf = malloc((uint32)scalelen);
                    if (scalebuf == NULL)
                        HGOTO_FAIL(FAIL);

                    status = Hgetelement((*handlep)->hdf_file, scaletag, scaleref, scalebuf);
                    if (status == FAIL)
                        HGOTO_FAIL(FAIL);

                    (*vars)->data_tag = DATA_TAG;
                    (*vars)->data_ref =
                        (*vars)->ndg_ref; /* Try to stick
                                             with the current way. If this ref conflicts with
                                             existing SDS, call Hnewref to get a new one. 3/25/97 */
                    datap = scalebuf + (*vars)->data_offset;
                    status =
                        Hputelement((*handlep)->hdf_file, DATA_TAG, (*vars)->data_ref, datap, (*vars)->len);
                    if (status == FAIL) {
                        (*vars)->data_tag = scaletag;
                        (*vars)->data_ref = scaleref;
                        HGOTO_FAIL(FAIL);
                    }
                } /* has scale values */
            }     /* DFTAG_SDS */
            vars++;
        }
    }

done:
    free(scalebuf);

    return ret_value;
}

/* ----------------------------------------------------------------
** Read in the dimensions out of a cdf structure
** Return FAIL if something goes wrong
*/
int
hdf_read_dims(XDR *xdrs, NC *handle, int32 vg)
{
    char     vgname[H4_MAX_NC_NAME]   = "";
    char     vsclass[H4_MAX_NC_CLASS] = "";
    char     vgclass[H4_MAX_NC_CLASS] = "";
    int      id, count, i, found;
    int      sub_id;
    int32    dim_size;
    NC_dim **dimension = NULL;
    int32    dim, entries;
    int32    vs;
    int      ret_value = SUCCEED;

    (void)xdrs;

    found = FALSE;
    count = 0;
    id    = -1;

    /*
     * Allocate enough space in case everything is a dimension
     */
    count     = 0;
    dimension = malloc(sizeof(NC_dim *) * (size_t)Vntagrefs(vg) + 1);
    if (NULL == dimension) {
        /* replace it with NCadvice or HERROR?? */
        HGOTO_FAIL(FAIL);
    }

    /*
     * Look through for a Vgroup of class _HDF_DIMENSION
     */
    while ((id = Vgetnext(vg, id)) != FAIL) {
        if (Visvg(vg, id)) {
            dim = Vattach(handle->hdf_file, id, "r");
            if (dim == FAIL)
                continue; /* why do we continue? does this failure here
                                not matter? -GV */
            if (Vgetclass(dim, vgclass) == FAIL)
                HGOTO_FAIL(FAIL);

            if (!strcmp(vgclass, _HDF_DIMENSION) || !strcmp(vgclass, _HDF_UDIMENSION)) {
                int is_dimval, is_dimval01;

                /* init both flags to FALSE  */
                is_dimval   = FALSE;
                is_dimval01 = FALSE;

                if (Vinquire(dim, &entries, vgname) == FAIL)
                    HGOTO_FAIL(FAIL);

                /*
                 * look through for a Vdata of class DIM_VALS01 and/or DIM_VALS
                 * to get size
                 */
                sub_id = -1;
                while (((sub_id = Vgetnext(dim, sub_id)) != FAIL)) {
                    if (Visvs(dim, sub_id)) {
                        vs = VSattach(handle->hdf_file, sub_id, "r");
                        if (vs == FAIL)
                            HGOTO_FAIL(FAIL);

                        if (VSgetclass(vs, vsclass) == FAIL)
                            HGOTO_FAIL(FAIL);

                        if (!strcmp(vsclass, DIM_VALS)) {
                            is_dimval = TRUE;
                            if (strcmp(vgclass, _HDF_UDIMENSION)) /* not unlimited dim */
                            {
                                if (VSQuerycount(vs, &dim_size) == FAIL)
                                    HGOTO_FAIL(FAIL);
                            }
                        }
                        if ((!strcmp(vsclass, DIM_VALS01)) ||
                            (!strcmp(vgclass, _HDF_UDIMENSION))) { /* DIM_VALS && _HDF_UDIMENSION */
                            int32 val; /* needs a temp var since handle->numrecs */
                                       /* may not be an int32 */
                                       /*
                                          The call to VSsetfields fails for the files created with the library
                                          version 3.3r1. This call is not necessary since handle vs is
                                          obtained by specifying class name.
                                          Elena Pourmal 2/17/99
           
                                                            if (VSsetfields(vs, "Values") == FAIL)
                                                            {
                                                                ret_value = FAIL;
                                                                goto done;
                                                            }
                                        */

                            if (VSseek(vs, 0) == FAIL)
                                HGOTO_FAIL(FAIL);

                            /*
                             * This is highly dangerous since there might be multiple
                             * unlimited dimensions
                             */
                            if (VSread(vs, (uint8 *)&val, 1, FULL_INTERLACE) != 1)
                                HGOTO_FAIL(FAIL);
                            if (!strcmp(vgclass, _HDF_UDIMENSION)) {
                                dim_size        = NC_UNLIMITED;
                                handle->numrecs = val;
                            }
                            else
                                dim_size = val;
                        } /* DIM_VALS && _HDF_UDIMENSION */

                        if (!strcmp(vsclass, DIM_VALS01)) /* dimval01  */
                            is_dimval01 = TRUE;

                        if (VSdetach(vs) == FAIL)
                            HGOTO_FAIL(FAIL);

                        /* Is it the second dim vs of a compatible dim? */
                        found = FALSE;
                        for (i = count - 1; ((i >= 0) && (!found)); i--) {
                            if (!strcmp(vgname, dimension[i]->name->values) &&
                                (dim_size == dimension[i]->size)) {
                                /* vgname is the dim name and may be diff from vsname */
                                if (is_dimval01 == TRUE && is_dimval == TRUE)
                                    dimension[i]->dim00_compat = 1;
                                found = TRUE; /* the second vs */
                            }
                        } /* for */

                        if (!found) {
                            dimension[count] = NC_new_dim(vgname, dim_size);
                            if (NULL == dimension[count]) {
                                HGOTO_FAIL(FAIL);
                            }                                 /*  dimension[count]  */
                            if (!strcmp(vsclass, DIM_VALS01)) /* dimvals01 only  */
                                dimension[count]->dim00_compat = 0;

                            /* record vgroup id here so we can use later */
                            /* Note: this is only for later file -BMR */
                            dimension[count]->vgid = id;

                            count++;
                        } /* found */
                    }     /* is vs  */
                }         /* while in dimension vg  */
            }             /* is vg  */

            if (Vdetach(dim) == FAIL)
                HGOTO_FAIL(FAIL);
        } /* while */
    }

    if (count) {
        handle->dims = NC_new_array(NC_DIMENSION, count, (uint8_t *)dimension);
        if (handle->dims == NULL)
            HGOTO_FAIL(FAIL);
    }
    else
        handle->dims = NULL;

done:
    if (ret_value == FAIL) {
        if (handle->dims != NULL) {
            NC_free_array(handle->dims);
            handle->dims = NULL;
        }
    }

    free(dimension);

    return ret_value;
} /* hdf_read_dims */

/******************************************************************************
 NAME
   hdf_num_attrs

 DESCRIPTION
   Determine number of attributes in vgroup i.e. of the SDS

 RETURNS
   returns number of attributes in vgroup if successful and FAIL
   otherwise.
*******************************************************************************/
static int
hdf_num_attrs(NC   *handle, /* IN: handle to SDS */
              int32 vg /* IN: ref of top Vgroup */)
{
    int   count = 0;
    int   t, n;
    int32 vs, tag;
    int32 id                    = -1;
    char class[H4_MAX_NC_CLASS] = "";
    int ret_value               = FAIL;

    n = Vntagrefs(vg);
    if (n == FAIL) {
        HGOTO_FAIL(FAIL);
    }

    /*
     * look through for a Vdata of class _HDF_ATTRIBUTE
     */
    for (t = 0; t < n; t++) {
        if (Vgettagref(vg, t, &tag, &id) == FAIL)
            HGOTO_FAIL(FAIL);

        if (tag == DFTAG_VH) {
            vs = VSattach(handle->hdf_file, id, "r");
            if (vs == FAIL)
                HGOTO_FAIL(FAIL);

            if (VSgetclass(vs, class) == FAIL)
                HGOTO_FAIL(FAIL);

            if (!strcmp(class, _HDF_ATTRIBUTE))
                count++;

            if (VSdetach(vs) == FAIL)
                HGOTO_FAIL(FAIL);
        }
    }

    ret_value = count;

done:
    return ret_value;
} /* hdf_num_attrs */

/* ----------------------------------------------------------------
** Read in all attributes of the current vg
** Return NULL if something goes wrong
** Return a pointer to the array of attributes if all goes well
*/
NC_array *
hdf_read_attrs(XDR *xdrs, NC *handle, int32 vg)
{
    int     count, t, n;
    int32   vs, tag, id, vsize, attr_size, nt;
    nc_type type;
    char    vsname[H4_MAX_NC_NAME] = "";
    char    fields[100]            = "";
    char class[H4_MAX_NC_CLASS]    = "";
    char     *values               = NULL;
    NC_attr **attributes           = NULL;
    NC_array *Array                = NULL;
    NC_array *ret_value            = NULL;

    (void)xdrs;

    count = 0;
    id    = -1;

    n = Vntagrefs(vg);
    if (n == FAIL) {
        HGOTO_FAIL(NULL);
    }

    /*
     * Allocate enough space in case everything is an attribute
     */
    count      = 0;
    attributes = malloc(sizeof(NC_attr *) * (size_t)n + 1);
    if (NULL == attributes) {
        /* replace it with NCAdvice or HERROR? */
        HGOTO_FAIL(NULL);
    }

    /*
     * look through for a Vdata of class _HDF_ATTRIBUTE
     */
    for (t = 0; t < n; t++) {
        if (Vgettagref(vg, t, &tag, &id) == FAIL)
            HGOTO_FAIL(NULL);

        if (tag == DFTAG_VH) {
            vs = VSattach(handle->hdf_file, id, "r");
            if (vs == FAIL)
                HGOTO_FAIL(NULL);

            if (VSgetclass(vs, class) == FAIL)
                HGOTO_FAIL(NULL);

            if (!strcmp(class, _HDF_ATTRIBUTE)) {
                if (VSinquire(vs, &attr_size, NULL, fields, &vsize, vsname) == FAIL)
                    HGOTO_FAIL(NULL);

                if ((nt = VFfieldtype(vs, 0)) == FAIL)
                    HGOTO_FAIL(NULL);

                if ((type = hdf_unmap_type(nt)) == FAIL)
                    HGOTO_FAIL(NULL);

                values = malloc((size_t)(vsize * attr_size + 1));
                if (NULL == values)
                    HGOTO_FAIL(NULL);

                if (VSsetfields(vs, fields) == FAIL)
                    HGOTO_FAIL(NULL);

                if (VSread(vs, (uint8 *)values, attr_size, FULL_INTERLACE) == FAIL)
                    HGOTO_FAIL(NULL);

                if (type == NC_CHAR) {
                    if ((attr_size = VFfieldorder(vs, 0)) == FAIL)
                        HGOTO_FAIL(NULL);

                    ((char *)values)[attr_size] = '\0';
                }

                attributes[count] = (NC_attr *)NC_new_attr(vsname, type, attr_size, values);
                if (NULL == attributes[count]) {
                    /* replace it with NCadvice or HERROR? */
                    HGOTO_FAIL(NULL);
                }
                attributes[count]->HDFtype = nt;

                /* free values and reset to NULL */
                free(values);
                values = NULL;
                count++;
            } /* end if attribute ? */

            if (VSdetach(vs) == FAIL)
                HGOTO_FAIL(NULL);
        } /* end if DFTAG_VH */
    }     /* end for */

    /* create array of attributes */
    if (count)
        Array = NC_new_array(NC_ATTRIBUTE, count, (uint8_t *)attributes);

    ret_value = Array; /* return array of attributes */

done:
    if (ret_value == NULL) {
        if (Array != NULL)
            NC_free_array(Array);
    }

    free(values);
    free(attributes);

    return ret_value;
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
hdf_read_vars(XDR *xdrs, NC *handle, int32 vg)
{
    char vgname[H4_MAX_NC_NAME]  = "";
    char subname[H4_MAX_NC_NAME] = "";
    char class[H4_MAX_NC_CLASS]  = "";
    NC_var      **variables      = NULL;
    NC_var       *vp             = NULL;
    int           ndims, *dims = NULL;
    uint8         ntstring[4];
    int           data_ref, is_rec_var, vg_size, count;
    int32         data_count;
    int32         HDFtype = FAIL;
    int32         tag;
    int32         id;
    int32         n;
    int32         sub_id;
    int32         entries;
    int32         ndg_ref = 0;
    int32         rag_ref = 0;
    int           nattrs;
    hdf_vartype_t var_type = UNKNOWN;
    int           t, i;
    nc_type       type;
    int32         var, sub;
    int           ret_value = SUCCEED;

    count = 0;
    id    = -1;

    /*
     * Allocate enough space in case everything is a variable
     */
    count     = 0;
    variables = malloc(sizeof(NC_var *) * (size_t)Vntagrefs(vg) + 1);
    if (NULL == variables) {
        HGOTO_FAIL(FAIL);
    }

    /*
     * Allocate enough space in case lots of dimensions
     */
    dims = malloc(sizeof(int) * (size_t)Vntagrefs(vg) + 1);
    if (NULL == dims) {
        HGOTO_FAIL(FAIL);
    }

    /*
     * Look through for a Vgroup of class _HDF_VARIABLE
     */
    if ((vg_size = Vntagrefs(vg)) == FAIL)
        HGOTO_FAIL(FAIL);

    for (i = 0; i < vg_size; i++) {
        if (Vgettagref(vg, i, &tag, &id) == FAIL) {
            HGOTO_FAIL(FAIL);
        }

        if (tag == DFTAG_VG) {
            var = Vattach(handle->hdf_file, id, "r");
            if (var == FAIL)
                continue; /* isn't this bad? -GV */

            if (Vgetclass(var, class) == FAIL) {
                HGOTO_FAIL(FAIL);
            }

            /* Process as below if this VGroup represents a Variable or
            a Coordinate Variable */
            if (!strcmp(class, _HDF_VARIABLE)) {

                /*
                 * We have found a VGroup representing a Variable or a
                 * a Coordinate Variable
                 */
                ndims      = 0;
                type       = NC_UNSPECIFIED;
                data_ref   = 0;
                data_count = 0;
                rag_ref    = 0;
                is_rec_var = FALSE;

                if (Vinquire(var, &n, vgname) == FAIL) {
                    HGOTO_FAIL(FAIL);
                }

                /*
                 * Loop through contents looking for dimensions
                 */
                for (t = 0; t < n; t++) {
                    char dimclass[H4_MAX_NC_CLASS] = "";
                    char vsclass[H4_MAX_NC_CLASS]  = "";
                    if (Vgettagref(var, t, &tag, &sub_id) == FAIL) {
                        HGOTO_FAIL(FAIL);
                    }

                    switch (tag) {
                        case DFTAG_VG: /* ------ V G R O U P ---------- */
                            sub = Vattach(handle->hdf_file, sub_id, "r");
                            if (FAIL == sub) {
                                HGOTO_FAIL(FAIL);
                            }

                            if (FAIL == Vgetclass(sub, dimclass)) {
                                HGOTO_FAIL(FAIL);
                            }

                            if (!strcmp(dimclass, _HDF_DIMENSION) || !strcmp(dimclass, _HDF_UDIMENSION)) {

                                if (!strcmp(dimclass, _HDF_UDIMENSION))
                                    is_rec_var = TRUE;

                                if (FAIL == Vinquire(sub, &entries, subname)) {
                                    HGOTO_FAIL(FAIL);
                                }

                                dims[ndims] = (int)NC_dimid(handle, subname);
                                if (-1 == dims[ndims]) /* should change to FAIL */
                                {
                                    HGOTO_FAIL(FAIL);
                                }

                                ndims++;
                            }
                            if (FAIL == Vdetach(sub)) {
                                HGOTO_FAIL(FAIL);
                            }

                            break;
                        case DFTAG_VH: /* ----- V D A T A ----- */
                            sub = VSattach(handle->hdf_file, sub_id, "r");
                            if (FAIL == sub)
                                HGOTO_FAIL(FAIL);

                            if (FAIL == VSgetclass(sub, vsclass))
                                HGOTO_FAIL(FAIL);

                            if (!strcmp(vsclass, _HDF_SDSVAR))
                                var_type = IS_SDSVAR;
                            else if (!strcmp(vsclass, _HDF_CRDVAR))
                                var_type = IS_CRDVAR;
                            else
                                var_type = UNKNOWN;

                            if (FAIL == VSdetach(sub))
                                HGOTO_FAIL(FAIL);
                            break;
                        case DFTAG_NDG: /* ----- NDG Tag for HDF 3.2 ----- */
                            ndg_ref = sub_id;
                            break;
                        case DFTAG_SD: /* ------- Data Storage ------ */
                            data_ref = sub_id;
                            /* Note: apparently Hlength will fail in certain cases, but
                                     but this okay since I believe this is because
                                     the data does not exist yet in the file?
                                     So we can't catch this error -GV*/
                            data_count = Hlength(handle->hdf_file, DATA_TAG, sub_id);

                            break;
                        case DFTAG_SDRAG: /* ----- Ragged Array index ----- */
                            rag_ref = sub_id;
                            break;
                        case DFTAG_NT: /* ------- Number type ------- */
                            if (Hgetelement(handle->hdf_file, tag, sub_id, ntstring) == FAIL) {
                                HGOTO_FAIL(FAIL);
                            }

                            HDFtype = ntstring[1];
                            if ((type = hdf_unmap_type(HDFtype)) == FAIL) {
                                HGOTO_FAIL(FAIL);
                            }

                            /*
                             * Check if data was stored in native format
                             * And make sure the numbertype version numbers are the same
                             */
                            if ((ntstring[0] != DFNT_VERSION) ||
                                ((ntstring[3] != DFNTF_NONE) && (ntstring[3] != DFNTF_IEEE))) {

                                /* check if in native mode for a different type of machine  or external data
                                 * file is LITEND */
                                if (ntstring[3] == DFNTF_PC)
                                    HDFtype |= DFNT_LITEND;
                                else {
                                    if (ntstring[3] != (uint8)DFKgetPNSC(HDFtype, DF_MT)) {
                                        /*
                                         * OK, we have a problem here --- is in native mode
                                         * for a different machine.  PUNT
                                         */
                                        goto bad_number_type; /* GOTO */
                                    }
                                    else {
                                        /*
                                         * Is in native mode but its OK --- same machine type
                                         */
                                        HDFtype |= DFNT_NATIVE;
                                    }
                                }
                            }

                            break;
                        default:
                            /* Do nothing */
                            break;
                    }
                }

                variables[count] = NC_new_var(vgname, type, ndims, dims);
                /* BMR: put back hdf type that was set wrong by
                NC_new_var; please refer to the cvs history of
                bug #172 for reason on this statement - 4/17/2001
                */
                variables[count]->HDFtype = HDFtype;

                vp = variables[count];
                if (NULL == vp) {
                    HGOTO_FAIL(FAIL);
                }

                /* Read in the attributes if any */
                if ((nattrs = hdf_num_attrs(handle, var)) > 0)
                    vp->attrs = hdf_read_attrs(xdrs, handle, var);
                else
                    vp->attrs = NULL;

                /* set up for easy access later */
                vp->vgid     = id;
                vp->data_ref = data_ref;
                vp->data_tag = DATA_TAG;
                vp->HDFtype  = HDFtype;
                vp->ndg_ref  = (uint16)ndg_ref;
                vp->cdf      = handle; /* for NC_var_shape */
                vp->var_type = var_type;

                /* need to process the ragged array info here */
                /* QUESTION:  Load the whole rag_fill list in now??????? */
                if (rag_ref) {
                    vp->is_ragged = TRUE;
                }

                if (vp->data_ref) {
                    /*
                     * We have already seen data for this variable so now
                     *  we need to worry about its numrecs field
                     */

                    if (is_rec_var) {
                        /*
                         * Call NC_var_shape() so we can figure out how many
                         *  records have been written.  This is horribly
                         *  inefficient, but the separation-of-powers gets
                         *  really mucked up if we wait till later...
                         */

                        if (NC_var_shape(vp, handle->dims) == -1) {
                            HGOTO_FAIL(FAIL);
                        }

                        /*
                         * Now figure out how many recs have been written
                         * For a while there was a -1 at the end of this
                         *   equation.  I don't remember why its there
                         *   (4-Nov-93)
                         */
                        vp->numrecs = data_count / vp->dsizes[0];

                        /*
                         * Deallocate the shape info as it will be recomputed
                         *  at a higher level later
                         */
                        free(vp->shape);
                        free(vp->dsizes);
                        /* Reset these two pointers to NULL after
                            freeing.  BMR 4/11/01 */
                        vp->shape  = NULL;
                        vp->dsizes = NULL;
                    }
                    else {
                        /* Not a rec var, don't worry about it */
                        vp->numrecs = 1;
                    }
                } /* end vp->data_ref */
                count++;
            } /* end if vgroup class is variable */

bad_number_type: /* ? */

            if (FAIL == Vdetach(var))
                HGOTO_FAIL(FAIL);

        } /* end if DTAG_VG */
    }     /* end for vg_size */

    /* create array of variables */
    if (count) {
        handle->vars = NC_new_array(NC_VARIABLE, count, (uint8_t *)variables);
        if (NULL == handle->vars)
            HGOTO_FAIL(FAIL);
    }
    else
        handle->vars = NULL;

done:
    if (ret_value == FAIL) {
        if (handle->vars != NULL)
            NC_free_array(handle->vars);
    }

    free(variables);
    free(dims);

    return ret_value;
} /* hdf_read_vars */

/* ----------------------------------------------------------------
** Read in a cdf structure
*/
int
hdf_read_xdr_cdf(XDR *xdrs, NC **handlep)
{
    int32 cdf_vg = FAIL;
    int   vgid   = 0;
    int   status;
    int   ret_value = SUCCEED;

    if ((vgid = Vfindclass((*handlep)->hdf_file, _HDF_CDF)) != FAIL) {
        cdf_vg = Vattach((*handlep)->hdf_file, vgid, "r");
        if (cdf_vg == FAIL)
            HGOTO_ERROR(DFE_CANTATTACH, FAIL);
    } /* end if */
    else
        HGOTO_FAIL(FAIL);

    (*handlep)->vgid = vgid; /* ref of vgroup */

    /* read in dimensions */
    status = hdf_read_dims(xdrs, (*handlep), cdf_vg);
    if (status == FAIL)
        HGOTO_FAIL(FAIL);

    /* read in variables */
    status = hdf_read_vars(xdrs, (*handlep), cdf_vg);
    if (status == FAIL)
        HGOTO_FAIL(FAIL);

    /* read in attributes */
    if (hdf_num_attrs((*handlep), cdf_vg) > 0)
        (*handlep)->attrs = hdf_read_attrs(xdrs, (*handlep), cdf_vg);
    else
        (*handlep)->attrs = NULL;

    /* detach from cdf vgroup */
    if (FAIL == Vdetach(cdf_vg))
        HGOTO_FAIL(FAIL);

done:
    if (ret_value == FAIL) {
        if (cdf_vg != FAIL)
            Vdetach(cdf_vg);
    }

    return ret_value;
} /* hdf_read_xdr_cdf */

/* -------------------------------------------------------------------
** Read or write a CDF structure
**
** If we are reading, first try to read the information out of netCDF
**    object stored explicitly in HDF files as netCDF objects.  If
**    that fails try to read SDSs out of the HDF file and interpret
**    them as netCDF information.
*/
int
hdf_xdr_cdf(XDR *xdrs, NC **handlep)
{
    int status;
    int ret_value = SUCCEED;

    switch (xdrs->x_op) {
        case XDR_ENCODE:
            if ((*handlep)->vgid) {
                if (FAIL == hdf_cdf_clobber((*handlep)))
                    HGOTO_FAIL(FAIL);
            }
            status = hdf_write_xdr_cdf(xdrs, handlep);
            if (FAIL == status) {
                HGOTO_FAIL(FAIL);
            }
            break;
        case XDR_DECODE:
            if (FAIL == (status = hdf_read_xdr_cdf(xdrs, handlep))) {
                status = hdf_read_sds_cdf(xdrs, handlep);
                if (FAIL == status) {
                    HGOTO_ERROR(DFE_BADNDG, FAIL);
                }
            } /* end if */
            break;
        case XDR_FREE:
            if (FAIL == NC_free_cdf((*handlep)))
                ret_value = FAIL;
            else
                ret_value = SUCCEED;
            break;
        default:
            ret_value = FAIL;
    }

done:
    return ret_value;
} /* hdf_xdr_cdf */

/* ---------------------- hdf_vg_clobber --------------- */
/*
  Delete a VGroup that is on the disk.  Basically, we will want to
  just trash everything inside of it, making sure that any VDatas
  with class == DATA are saved
*/
int
hdf_vg_clobber(NC *handle, int id)
{
    int   t, n;
    int32 vg, tag, ref;
    int32 status;
    int   ret_value = SUCCEED;

    /* loop through and Clobber all top level VGroups */

    /* attach to top level vgroup with read access */
    vg = Vattach(handle->hdf_file, id, "r");
    if (FAIL == vg) {
        HGOTO_FAIL(FAIL);
    }

    /* get number of members in vgroup */
    n = Vntagrefs(vg);
    if (FAIL == n) {
        HGOTO_FAIL(FAIL);
    }

    /* Loop though and kill stuff */
    for (t = 0; t < n; t++) { /* get tag/ref of element in vgroup */
        if (FAIL == Vgettagref(vg, t, &tag, &ref)) {
            HGOTO_FAIL(FAIL);
        }

        /* switch on the type of element: vgroup, vdata, data,
           everything else */
        switch (tag) {
            case DFTAG_VG: /* recursive call */
                /* check if vgroup exists in file before trying to delete
                   it's members */
                if (vexistvg(handle->hdf_file, ref) != FAIL) {
                    if (FAIL == hdf_vg_clobber(handle, ref)) {
                        HGOTO_FAIL(FAIL);
                    }
                }
                break;
            case DFTAG_VH:
                /* check if vdata exists in file before trying to delete it */
                if (vexistvs(handle->hdf_file, ref) != FAIL) {
                    status = VSdelete(handle->hdf_file, (int32)ref);
                    if (FAIL == status) {
                        HGOTO_FAIL(FAIL);
                    }
                }
                break;
            case DFTAG_SD:
                /*
                 * Don't delete actual numeric data
                 * I guess this means we save it? -GV
                 */
                break;
            default: /* delete other objects given tag/ref in file */
                if (FAIL == Hdeldd(handle->hdf_file, (uint16)tag, (uint16)ref)) {
                    HGOTO_FAIL(FAIL);
                }

                break;
        }
    }

    ret_value = Vdetach(vg);

done:
    return ret_value;
} /* hdf_vg_clobber */

/* --------------------------- hdf_cdf_clobber ---------------------------- */
/*
  Delete a netCDF structure that has been already written to disk
*/
int
hdf_cdf_clobber(NC *handle)
{
    int32 vg, tag, ref;
    int   n, t, status;
    int   ret_value = SUCCEED;

    if (!handle->vgid) {     /* okay right? */
        ret_value = SUCCEED; /* hmm...since ref of vgroup is zero? */
        goto done;
    }

    /* Close open VData pointers */
    if (FAIL == hdf_close(handle)) {
        HGOTO_FAIL(FAIL);
    }

    /* loop through and Clobber all top level VGroups */
    vg = Vattach(handle->hdf_file, handle->vgid, "r");
    if (vg == FAIL) {
        HGOTO_FAIL(FAIL);
    }

    /* get number of members of Vgroup */
    n = Vntagrefs(vg);
    if (FAIL == n) {
        HGOTO_FAIL(FAIL);
    }

    /* Loop though and just kill everyone */
    for (t = 0; t < n; t++) {
        if (FAIL == Vgettagref(vg, t, &tag, &ref)) {
            HGOTO_FAIL(FAIL);
        }

        /* if this member is a vgroup destroy everything in it */
        if (tag == DFTAG_VG) {
            /* check if vgroup exists in file */
            if (vexistvg(handle->hdf_file, ref) != FAIL) {
                hdf_vg_clobber(handle, ref);
            }
        }

        switch (tag) {
            case DFTAG_VG:
                status = Vdelete(handle->hdf_file, (int32)ref);
                if (FAIL == status) {
                    HGOTO_FAIL(FAIL);
                }

                break;
            case DFTAG_VH:
                status = VSdelete(handle->hdf_file, (int32)ref);
                if (FAIL == status) {
                    HGOTO_FAIL(FAIL);
                }

                break;
            default:
                status = Hdeldd(handle->hdf_file, (uint16)tag, (uint16)ref);
                if (FAIL == status) {
                    HGOTO_FAIL(FAIL);
                }

                break;
        } /* end switch tag */
    }     /* end for every member in vgroup */

    if (FAIL == Vdetach(vg)) {
        HGOTO_FAIL(FAIL);
    }

    status = Vdelete(handle->hdf_file, (int32)handle->vgid);
    if (FAIL == status) {
        HGOTO_FAIL(FAIL);
    }

    handle->vgid = 0; /* reset ref of SDS vgroup to invalid ref */

done:
    return ret_value;
} /* hdf_cdf_clobber */

/* -------------------------- hdf_close --------------------- */
/*
  We're about to close the file, do last minute HDF cleanup
  Also dump the number of records currently instantiated into the
  unlimited dimensions.

  BUG:  All unlimited dimensions will have the same size

BMR: handle->numrecs is used to write out the dim value for all unlimited
     dimensions without checking for netCDF file as in other places where
     vp->numrecs is used for HDF file and handle->numrecs is used for netCDF
     file.  I believe this is what the "BUG:" comment above means.  6/24/2013
*/
int
hdf_close(NC *handle)
{
    NC_array *tmp  = NULL;
    NC_var  **vp   = NULL;
    uint8_t  *vars = NULL;
    int       i;
    int       id, sub_id;
    int32     vg, dim;
    int32     vs;
    char class[H4_MAX_NC_CLASS] = "";
    int ret_value               = SUCCEED;

    /* loop through and detach from variable data VDatas */
    if (handle->vars) {
        tmp  = handle->vars;
        vars = handle->vars->values;

        for (i = 0; i < tmp->count; i++) {
            vp = (NC_var **)vars;

            if ((*vp)->aid != FAIL) {
                if (FAIL == Hendaccess((*vp)->aid)) {
                    HGOTO_FAIL(FAIL);
                }
            }

            (*vp)->aid = FAIL; /* reset access id */
            vars += tmp->szof;
        } /* end for each variable */
    }

    /* loop through top level looking for unlimited dimensions.
       we write them out? -GV */
    if (handle->flags & NC_NDIRTY) {
        id = -1;
        vg = Vattach(handle->hdf_file, handle->vgid, "r");
        if (FAIL == vg) {
            HGOTO_FAIL(FAIL);
        }

        /* go through vgroup hierarchy */
        while ((id = Vgetnext(vg, id)) != FAIL) {
            if (Visvg(vg, id)) {
                dim = Vattach(handle->hdf_file, id, "r");
                if (FAIL == dim) {
                    HGOTO_FAIL(FAIL);
                }

                if (FAIL == Vgetclass(dim, class)) {
                    HGOTO_FAIL(FAIL);
                }

                /* look for proper vgroup */
                if (!strcmp(class, _HDF_UDIMENSION)) {
                    sub_id = -1;
                    /* look for vdata in vgroup */
                    while ((sub_id = Vgetnext(dim, sub_id)) != FAIL) {
                        if (Visvs(dim, sub_id)) { /* yes, attach to vdata */
                            vs = VSattach(handle->hdf_file, sub_id, "w");
                            if (vs == FAIL) {
                                HGOTO_FAIL(FAIL);
                                /* HEprint(stdout, 0); */
                            }
                            /* get class of vdata */
                            if (FAIL == VSgetclass(vs, class)) {
                                HGOTO_FAIL(FAIL);
                            }

                            /* are these dimension vdatas? */
                            if (!strcmp(class, DIM_VALS) || !strcmp(class, DIM_VALS01)) { /* yes */
                                int32 val = handle->numrecs;

                                if (FAIL == VSsetfields(vs, "Values")) {
                                    HGOTO_FAIL(FAIL);
                                }

                                if (FAIL == VSseek(vs, 0)) {
                                    HGOTO_FAIL(FAIL);
                                }

                                /* write out dimension vdatas? */
                                if (VSwrite(vs, (uint8 *)&val, 1, FULL_INTERLACE) != 1) {
                                    HGOTO_FAIL(FAIL);
                                }
                            }

                            /* detach from vdata */
                            if (FAIL == VSdetach(vs)) {
                                HGOTO_FAIL(FAIL);
                            }

                        } /* end if vdata */
                    }     /* end while looking for vdata in vgroup */
                }         /* end if UNLIMITED dimension vgroup */

                if (FAIL == Vdetach(dim)) {
                    fprintf(stderr, "hdf_close: Vdetach failed for vgroup ref %d\n", id);
                    HGOTO_FAIL(FAIL);
                }

            } /* end if vgroup */
        }     /* end if looking through toplevel vgroup hierarchy */

        if (FAIL == Vdetach(vg)) {
            HGOTO_FAIL(FAIL);
        }

    } /* end if we need to flush out unlimited dimensions? */

done:
    return ret_value;
} /* hdf_close */

/*******************************************************************************/

/*
 * How much space will the xdr'd NC description take.
 *
 */
int
NC_xlen_cdf(NC *cdf)
{
    int len = 8;

    if (cdf == NULL)
        return 0;

    len += NC_xlen_array(cdf->dims);
    len += NC_xlen_array(cdf->attrs);
    len += NC_xlen_array(cdf->vars);

    return len;
}

#define RECPOS 4L /* seek index of numrecs value */
bool_t
xdr_numrecs(XDR *xdrs, NC *handle)
{

    if (handle->file_type == HDF_FILE)
        return TRUE; /* hmm...why? */

    if ((handle->flags & NC_NOFILL) && xdrs->x_op == XDR_ENCODE && handle->begin_rec > 0) {
        /*
         * we need to write something just beyond the last
         * record so we can successfully read back the
         * entire last record.
         */
        if (!h4_xdr_setpos(xdrs, handle->begin_rec + handle->numrecs * handle->recsize)) {
            nc_serror("Can't set position to EOF");
            return FALSE;
        }

        if (!h4_xdr_u_int(xdrs, &(handle->numrecs)))
            return FALSE;
    }

    if (!h4_xdr_setpos(xdrs, RECPOS)) {
        nc_serror("Can't set position to RECPOS");
        return FALSE;
    }
    return h4_xdr_u_int(xdrs, &(handle->numrecs));
}

static bool_t
xdr_2shorts(XDR *xdrs, short *sp)
/* sp - at least 2 valid shorts */
{
    return xdr_shorts(xdrs, sp, 2);
}

bool_t
xdr_NC_fill(XDR *xdrs, NC_var *vp)
{
    char   fillp[2 * sizeof(double)];
    bool_t stat;
    bool_t (*xdr_NC_fnct)();
    unsigned long alen = vp->len;
    NC_attr     **attr = NULL;

    /*
     * set up fill value
     */
    /* start with the default */
    NC_arrayfill((uint8_t *)fillp, (size_t)2 * sizeof(double), vp->type);

    /*
     * if there is a valid user defined value, use it instead
     */
    attr = NC_findattr(&vp->attrs, _FillValue);
    if (attr != NULL) {
        if ((*attr)->data->type != vp->type || (*attr)->data->count != 1)
            NCadvise(NC_EBADTYPE, "var %s: _FillValue type mismatch", vp->name->values);
        else {
            int   len = NC_typelen(vp->type);
            char *cp  = fillp;
            while (cp < &fillp[sizeof(fillp) - 1]) {
                NC_copy_arrayvals(cp, (*attr)->data);
                cp += len;
            }
        }
    }

    switch (vp->type) {
        case NC_BYTE:
        case NC_CHAR:
            alen /= 4;
            xdr_NC_fnct = h4_xdr_bytes;
            break;
        case NC_SHORT:
            alen /= 4;
            xdr_NC_fnct = xdr_2shorts;
            break;
        case NC_LONG:
            alen /= 4;
            xdr_NC_fnct = h4_xdr_int;
            break;
        case NC_FLOAT:
            alen /= 4;
            xdr_NC_fnct = h4_xdr_float;
            break;
        case NC_DOUBLE:
            alen /= 8;
            xdr_NC_fnct = h4_xdr_double;
            break;
        default:
            NCadvise(NC_EBADTYPE, "bad type %d", vp->type);
            return FALSE;
    }

    /* write out fill values */
    for (stat = TRUE; stat && (alen > 0); alen--) {
        stat = (*xdr_NC_fnct)(xdrs, fillp);
    }

    if (!stat) {
        NCadvise(NC_EXDR, "xdr_NC_fill");
        return FALSE;
    }

    return TRUE;
}
