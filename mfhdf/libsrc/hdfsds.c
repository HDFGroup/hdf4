/****************************************************************************
 * NCSA HDF                                                                 *
 * Software Development Group                                               *
 * National Center for Supercomputing Applications                          *
 * University of Illinois at Urbana-Champaign                               *
 * 605 E. Springfield, Champaign IL 61820                                   *
 *                                                                          *
 * For conditions of distribution and use, see the accompanying             *
 * hdf/COPYING file.                                                        *
 *                                                                          *
 ****************************************************************************/

#ifdef RCSID
static char RcsId[] = "@(#)$Revision$";
#endif

/* $Id$ */

/***************************************************************************
 *
 * This file contains the code to read old SDS objects out of HDF files
 *  an pretend that they are netCDF / mutli-file SDS data objects
 *
 **************************************************************************/

/*

  General Thoughts


  Will need to add a 'type' field to the variable stucture so that we can
  how it was stored (basically will want to just store the tag of the object
  as well as the ref (which is already stored)).
  

  We can use the LUF record to get names of dimensions.  If no info stored
  int the LUF then we will need to fake it.

  Will prolly be easiest to define a coordinate variable for every dimension 
  even if it doesn't need any attributes or data values

  Scales are a problem.  In HDF scales, are used to define the values for
  the coordinate variables.  The problem is that all of the values for the
  scales in HDF are jammed into the same tag's storage.  So, we will need
  to add an 'offset' field to the variable record so that we know where to 
  start reading to get the data.  If the offset == 0 then we should start
  reading from the beginning.  If the offset == -1 then there is *NO* data
  (since a scale may not have data specified for it).

*/

#ifdef HDF

#include        "local_nc.h"

PRIVATE intn       sdgCurrent;
PRIVATE intn       sdgMax;
PRIVATE uint16     *sdgTable;

intn hdf_query_seen_sdg
    PROTO((uint16 ndgRef));

VOID hdf_register_seen_sdg
    PROTO((uint16 ndgRef));

/* --------------------------- hdf_query_seen_sdg --------------------------- */
/*
 *  The SDG with the given ref number might be part of an SDG-NDG combo
 *    if so, we return TRUE else FALSE.
 *
 *  Attempt to discover if we've seen it in the most inefficient manner
 *    possible
 */
#ifdef PROTOTYPE
intn hdf_query_seen_sdg(uint16 ndgRef)
#else
intn hdf_query_seen_sdg(ndgRef)
     uint16 ndgRef;
#endif
{
    intn i;

    if(!sdgTable) return FALSE;

    for(i = 0; i < sdgCurrent; i++)
        if(sdgTable[i] == ndgRef) return TRUE;

    return FALSE;

} /* hdf_cdf_query_sdg */


/* --------------------------- hdf_register_seen_sdg --------------------------- */
/*
 *  The SDG with the given ref is in an SDG-NDG combo.  Add it to the seen table
 *    so we don't read it twice
 */
#ifdef PROTOTYPE
VOID hdf_register_seen_sdg(uint16 sdgRef)
#else
VOID hdf_register_seen_sdg(sdgRef)
     uint16 sdgRef;
#endif
{

    if(!sdgTable) {
        sdgMax = 100;
        sdgTable = (uint16 *) HDgetspace(sdgMax * sizeof(uint16));
        sdgCurrent = 0;
    }
    
    sdgTable[sdgCurrent++] = sdgRef;
    
    if(sdgCurrent == sdgMax) {
        sdgMax *= 2;
        sdgTable = (uint16 *) HDregetspace((VOIDP) sdgTable, sdgMax * sizeof(uint16));
    }

} /* hdf_register_seen_sdg */


/* -------------------------- hdf_read_ndgs ------------------------------- */
/*
 * Loop through all of the NDGs in the file and create data structures for 
 *  them
 *
 * NOTE: DFtbuf is a global temporary buffer defined in hdfi.h
 */
#ifdef PROTOTYPE
intn hdf_read_ndgs(NC *handle)
#else
intn hdf_read_ndgs(handle)
     NC *handle;
#endif
{
    static char *FUNC = "hdf_read_ndg_dims";
    char   tmpname[80];
    uint8  ntstring[4];
    intn   dimcount;

    /* info about NDG structure */
    int32  GroupID, aid, aid1;
    uint16 ndgTag, ndgRef;
    uint16 lRef, uRef, fRef, sRef, sdRef;
    uint16 tmpTag, tmpRef;
    int16  rank, type;
    int32  *dimsizes, *scaletypes, HDFtype;
    intn   dim, max_thangs, current_dim, current_var, current_attr;
    intn   *vardims;

    /* info about netCDF structures */
    NC_dim  **dims;   /* hold list of dimensions as we create it */
    NC_var  **vars;   /* hold variable list as we create it      */
    NC_attr **attrs;  /* hold attribute list as we create it     */
    uint8  *namebuf;  /* buffer to store label info   */
    uint8  *scalebuf; /* buffer to store scale info */
    uint8  *unitbuf;  /* buffer to store unit info */

    intn   new_dim;   /* == new dim so create coord variable     */

    /* random book-keeping */
    intn   i, status, tag_index;
    uint8  *p, tBuf[128];
    char   *q;

    intn scale_offset;   /* current offset into the scales record for the
                            current dimension's values */

    /*
     *  Allocate the array to store the dimensions
     */
    max_thangs  = 100;
    dims = (NC_dim **) HDgetspace(sizeof(NC_dim *) * max_thangs);
    if(!dims) {
        HERROR(DFE_NOSPACE);
        return FALSE;
    }
    
    vars = (NC_var **) HDgetspace(sizeof(NC_var *) * max_thangs);
    if(!vars) {
        HERROR(DFE_NOSPACE);
        return FALSE;
    }

    attrs = (NC_attr **) HDgetspace(sizeof(NC_attr *) * max_thangs);
    if(!attrs) {
        HERROR(DFE_NOSPACE);
        return FALSE;
    }
    

    /* no dimensions or variables yet */
    current_dim = 0;
    current_var = 0;
    dimcount = 0;

    for(tag_index = 0; tag_index < 2; tag_index++) {

        if(tag_index == 0)
            ndgTag = DFTAG_NDG;
        else
            ndgTag = DFTAG_SDG;

        /*
         * Start from the beginning and look though
         */
        aid = Hstartread(handle->hdf_file, ndgTag, DFREF_WILDCARD);
        if(aid == FAIL) continue;
        
        /*
         * Loop through all of the NDGs in the file
         */
        status = SUCCEED;
        while (status == SUCCEED) {
            uint16 ntTag, ntRef;
            
            if(HQuerytagref(aid, &ndgTag, &ndgRef) == FAIL) {
                fprintf(stderr, "Call to Hinquire failed\n");
                return FALSE;
            }

            /* Test if its an SDG-NDG which we've processed already */
            if((ndgTag == DFTAG_SDG) && (hdf_query_seen_sdg(ndgRef))) {
                status = Hnextread(aid, ndgTag, DFREF_WILDCARD, DF_CURRENT);  
                continue;
            }
                
            /* OK, now we need to get the relevant dimension structure */
#ifdef DEBUG
            printf("Found NDG at %d %d\n", ndgTag, ndgRef);
#endif
            
            /* read the group into memory */
            if ((GroupID = DFdiread(handle->hdf_file, ndgTag, ndgRef)) < 0) 
                return FALSE;
            
            lRef = uRef = fRef = sRef = sdRef = 0;
            
            /* default number type is Float32 */
            type    = NC_FLOAT; 
            HDFtype = DFNT_FLOAT32;
            
            /* no attributes found yet */
            current_attr = 0;
            
            /* no meta-data seen yet */
            unitbuf = scalebuf = namebuf = NULL;
            
            /*
             * Loop through the members of the group looking for stuff
             *
             * NOTE:  Only generate attributes for meta-data which does
             *        not depend on the rank of the data since we can not
             *        be sure that we get the rank first.  
             *        If the meta-data depends on the rank, just remember the
             *        ref number and read the element once this while loop
             *        is finished.
             */
            while (!DFdiget(GroupID, &tmpTag, &tmpRef)) {
                
                switch(tmpTag) {
                case DFTAG_SDD:
                    aid1 = Hstartread(handle->hdf_file, tmpTag, tmpRef);
                    if (aid1 == FAIL)
                        return FALSE;
                    
                    /* read rank */
                    if (Hread(aid1, (int32) 2, DFtbuf) == FAIL) return FALSE;
                    p = DFtbuf;
                    INT16DECODE(p, rank);
                    
                    /* get space for dimensions */
                    dimsizes = (int32 *) HDgetspace((uint32) rank * sizeof(int32));
                    if (dimsizes == NULL) return FALSE;
                    vardims = (intn *) HDgetspace((uint32) rank * sizeof(intn));
                    if (vardims == NULL) return FALSE;
                    scaletypes = (int32 *) HDgetspace((uint32) rank * sizeof(int32));
                    if (scaletypes == NULL) return FALSE;
                    
                    /* read dimension record */
                    if (Hread(aid1, (int32) 4 * rank, DFtbuf) == FAIL) return FALSE;
                    p = DFtbuf;
                    for (i = 0; i < rank; i++)
                        INT32DECODE(p, dimsizes[i]);
                    
                    /* read tag/ref of NT */
                    if (Hread(aid1,(int32) 4,  DFtbuf) == FAIL) return FALSE;
                    p = DFtbuf;
                    UINT16DECODE(p, ntTag);
                    UINT16DECODE(p, ntRef);
                    
                    /* read actual NT */
                    if (Hgetelement(handle->hdf_file, ntTag, ntRef, ntstring) == FAIL)
                        return FALSE;
                    
                    HDFtype = ntstring[1];
                    type = hdf_unmap_type(HDFtype);
                    
                    /* read in scale NTs */
                    for(i = 0; i < rank; i++) {
                        if (Hread(aid1,(int32) 4,  DFtbuf) == FAIL) return FALSE;
                        p = DFtbuf;
                        UINT16DECODE(p, ntTag);
                        UINT16DECODE(p, ntRef);
                        
                        /* read NT of this scale (dimension) */
                        if (Hgetelement(handle->hdf_file, ntTag, ntRef, ntstring) == FAIL)
                            return FALSE;
                        
                        scaletypes[i] = ntstring[1];
                    }
                    
                    Hendaccess(aid1);
                    
                    break;
                    
                case DFTAG_SDL:       /* label */
                    lRef = tmpRef;
                    break;
                    
                case DFTAG_SDU:       /* unit */
                    uRef = tmpRef;
                    break;
                    
                case DFTAG_SDF:       /* format */
                    fRef = tmpRef;
                    break;
                    
                case DFTAG_SDS:       /* scales */
                    sRef = tmpRef;
                    break;
                    
                case DFTAG_SD:        /* actual data */
                    sdRef = tmpRef;
                    break;
                    
                case DFTAG_CAL:        /* calibration info */
                    /* 
                     * DFTAG_CAL => 'scale_factor', 'add_offset', 'scale_factor_err', 
                     *              'add_offset_err'
                     */
                    if (Hgetelement(handle->hdf_file, tmpTag, tmpRef, DFtbuf) == FAIL)
                        return FALSE;
                    
                    if (Hlength(handle->hdf_file, tmpTag, tmpRef) == 36) {
                        /* DFNT_FLOAT64 based calibration */
                        
                        DFKconvert((VOIDP)DFtbuf, 
                                   (VOIDP) tBuf, 
                                   DFNT_FLOAT64, 4, DFACC_READ, 0, 0);
                        
                        attrs[current_attr++] = 
                            (NC_attr *) NC_new_attr("scale_factor", 
                                                    NC_DOUBLE, 
                                                    1, 
                                                    (Void *) &(tBuf[0]));
                        
                        attrs[current_attr++] = 
                            (NC_attr *) NC_new_attr("scale_factor_err", 
                                                    NC_DOUBLE, 
                                                    1, 
                                                    (Void *) &(tBuf[8]));    
                        
                        attrs[current_attr++] = 
                            (NC_attr *) NC_new_attr("add_offset", 
                                                    NC_DOUBLE, 
                                                    1, 
                                                    (Void *) &(tBuf[16]));
                        
                        attrs[current_attr++] = 
                            (NC_attr *) NC_new_attr("add_offset_err", 
                                                    NC_DOUBLE, 
                                                    1, 
                                                    (Void *) &(tBuf[24]));
                    
                    } else {
                        /* DFNT_FLOAT32 based calibration */

                        DFKconvert((VOIDP)DFtbuf, 
                                   (VOIDP)tBuf, 
                                   DFNT_FLOAT32, 4, DFACC_READ, 0, 0);
                        
                        attrs[current_attr++] = 
                            (NC_attr *) NC_new_attr("scale_factor", 
                                                    NC_FLOAT, 
                                                    1, 
                                                    (Void *) &(tBuf[0]));
                        
                        attrs[current_attr++] = 
                            (NC_attr *) NC_new_attr("scale_factor_err", 
                                                    NC_FLOAT, 
                                                    1, 
                                                    (Void *) &(tBuf[4]));    
                        
                        attrs[current_attr++] = 
                            (NC_attr *) NC_new_attr("add_offset", 
                                                    NC_FLOAT, 
                                                    1, 
                                                    (Void *) &(tBuf[8]));
                        
                        attrs[current_attr++] = 
                            (NC_attr *) NC_new_attr("add_offset_err", 
                                                    NC_FLOAT, 
                                                    1, 
                                                    (Void *) &(tBuf[12]));
                        
                        
                    }
                    
                    break;
                    
                case DFTAG_SDM:        /* valid range info */
                    
                    if (Hgetelement(handle->hdf_file, tmpTag, tmpRef, DFtbuf) == FAIL)
                        return FALSE;
                    
                    DFKconvert((VOIDP)DFtbuf, 
                               (VOIDP)tBuf, 
                               HDFtype, 2, DFACC_READ, 0, 0);
                    
                    attrs[current_attr++] = 
                        (NC_attr *) NC_new_attr("valid_max", 
                                                type, 
                                                1, 
                                                (Void *) tBuf);
                    
                    attrs[current_attr++] = 
                        (NC_attr *) NC_new_attr("valid_min", 
                                                type, 
                                                1, 
                                                (Void *) &(tBuf[DFKNTsize(HDFtype)]));
                    
                    break;

                case DFTAG_SDLNK:
                    if(ndgTag == DFTAG_SDG) continue;

                    if (Hgetelement(handle->hdf_file, tmpTag, tmpRef, DFtbuf) == FAIL) {
                        return FALSE;
                    } else {
                        uint16 sdgTag, sdgRef;

                        p = DFtbuf;
                        
                        /* the first two are for the NDG tag/ref */
                        UINT16DECODE(p, sdgTag);
                        UINT16DECODE(p, sdgRef);
                        
                        /* now the ones we're interested in */
                        UINT16DECODE(p, sdgTag);
                        UINT16DECODE(p, sdgRef);

                        hdf_register_seen_sdg(sdgRef);
                        
                    }

                    break;
                    
                default:
                    break;
                }
            }    
            
            if(lRef) {
                int len;
                
                /*
                 *  Add three NULLS to the end to account for a bug in HDF 3.2r1-3
                 */

                len = Hlength(handle->hdf_file, DFTAG_SDL, lRef);
                if(len == FAIL) return FALSE;
                
                namebuf = (uint8 *) HDgetspace((uint32) len + 3);
                if(!namebuf) HRETURN_ERROR(DFE_NOSPACE, FALSE);
                
                if(Hgetelement(handle->hdf_file, DFTAG_SDL, lRef, namebuf) == FAIL)
                    return FALSE;
                
                namebuf[len + 2] = '\0';
                namebuf[len + 1] = '\0';
                namebuf[len + 0] = '\0';
                
            } else {
                namebuf = NULL;
            }
            
            if(uRef) {
                int len;
                
                len = Hlength(handle->hdf_file, DFTAG_SDU, uRef);
                if(len == FAIL) return FALSE;
                
                unitbuf = (uint8 *) HDgetspace((uint32) len);
                if(!unitbuf) HRETURN_ERROR(DFE_NOSPACE, FALSE);
                
                if(Hgetelement(handle->hdf_file, DFTAG_SDU, uRef, unitbuf) == FAIL)
                    return FALSE;
                
            } else {
                unitbuf = NULL;
            }
            
            if(sRef) {
                int len;
                
                len = Hlength(handle->hdf_file, DFTAG_SDS, sRef);
                if(len == FAIL) return FALSE;
                
                scalebuf = (uint8 *) HDgetspace((uint32) len);
                if(!scalebuf) HRETURN_ERROR(DFE_NOSPACE, FALSE);
                
                if(Hgetelement(handle->hdf_file, DFTAG_SDS, sRef, scalebuf) == FAIL)
                    return FALSE;
                
            } else {
                scalebuf = NULL;
            }
            
            /* skip over the garbage at the beginning */
            scale_offset = rank * sizeof(uint8);
            
            for (dim = 0; dim < rank; dim++) {
                intn this_dim     = FAIL;
                char *unitname    = NULL;
                
                /* now loop though each dimension
                   - get the size from dimsize[i]
                   - lref will give the ref of the label descriptor to see if 
                   has a real name else fake one based on the label of the NDG
                   - look at uref for units information and fref for formating info
                   store both of these as attributes of the coordinate variable
                   */
                
                tmpname[0] = '\0';
                
                if(lRef) {
                    q = (char *) namebuf;
                    for(i = 0; i < dim + 1; i++) q += HDstrlen(q) + 1;
                    sprintf(tmpname, "%s", q);
                }
                
                if(uRef) {
                    unitname = (char *) unitbuf;
                    for(i = 0; i < dim + 1; i++) unitname += HDstrlen(unitname) + 1;
                }
                
                if(tmpname[0] == '\0')
                    sprintf(tmpname, "Dimension-%d", dimcount++);
                
                /*
                 *  We should check to make sure that we have unique 
                 *    dimension names
                 */
                /*
                 *  if this dimension already exists, set this_dim to
                 *  its index else do nothing
                 */
                for(i = 0; i < current_dim; i++)
                    if(strcmp(dims[i]->name->values, tmpname) == 0) {
                        /* found it */
                        this_dim = i;
                    }             
                                
                new_dim = FALSE;
                if(this_dim == FAIL) {
                    this_dim = current_dim++;
                    if(current_dim == max_thangs) {
                        /* need to allocate more space */    
                        max_thangs *= 2;

                        dims = (NC_dim **) HDregetspace((VOIDP) dims, sizeof(NC_dim *) * max_thangs);
                        if(!dims) HRETURN_ERROR(DFE_NOSPACE, FALSE);
                        
                        vars = (NC_var **) HDregetspace((VOIDP) vars, sizeof(NC_var *) * max_thangs);
                        if(!vars) HRETURN_ERROR(DFE_NOSPACE, FALSE);
                        
                    }
                    new_dim  = TRUE;
                }

                /*
                 * hmmm, this will blow away the old dim record...
                 */
                dims[this_dim] = NC_new_dim(tmpname, dimsizes[dim]);
                
                /* 
                 * It looks like were gonna have to do the variable define
                 *    here too cuz we need to remember the indicies of where
                 *    we put the dimensions
                 */
                
                vardims[dim] = (intn) this_dim;
                
                
                /*
                 * Look at the scale NTs since the scales may have different number 
                 *   types
                 * Promote the dimension to a variable, but only if it has meta-data
                 *   stored with it.
                 */
                if(new_dim && scalebuf && scalebuf[dim]) {
                    vars[current_var] = NC_new_var(tmpname, 
                                                   hdf_unmap_type(scaletypes[dim]),
                                                   1, 
                                                   &this_dim);
                    vars[current_var]->data_tag = DFTAG_SDS;  /* not normal data */
                    vars[current_var]->data_ref = sRef;
                    vars[current_var]->HDFtype  = scaletypes[dim];
                    vars[current_var]->ndg_ref  = Hnewref(handle->hdf_file);

                    /*
                     * See if a scales record has been stored and if there have
                     *   been scale values provided
                     */
                    if((scalebuf) && (scalebuf[dim])) {
                        vars[current_var]->data_offset = scale_offset;
                        scale_offset += dimsizes[dim] * DFKNTsize(scaletypes[dim]);
                    } else {
                        vars[current_var]->data_offset = -1;  /* no values */
                    }
                    
                    current_var++;
                    if(current_var == max_thangs) {
                        /* need to allocate more space */    
                        max_thangs *= 2;

                        dims = (NC_dim **) HDregetspace((VOIDP)dims, sizeof(NC_dim *) * max_thangs);
                        if(!dims) HRETURN_ERROR(DFE_NOSPACE, FALSE);
                        
                        vars = (NC_var **) HDregetspace((VOIDP)vars, sizeof(NC_var *) * max_thangs);
                        if(!vars) HRETURN_ERROR(DFE_NOSPACE, FALSE);
                        
                    }

                }
                
            }
            
            /*
             * If there is a data label use that as the variable name
             *    else use the reference number of the NDG as part of
             *    a made up name
             *
             * Convert spaces in the name to underscores (yuck) otherwise
             *    ncgen will barf on ncdumped files
             */
            if(namebuf) {
                char *c;
                for(c = (char *)namebuf; *c; c++)
                    if((*c) == ' ') (*c) = '_';

                vars[current_var] = NC_new_var((char *) namebuf, type, (int) rank, vardims);
            } else {
                sprintf(tmpname, "Data-Set-%d", ndgRef); 
                vars[current_var] = NC_new_var(tmpname, type, (int) rank, vardims);
            }
            
            /*
             * Fill in extra information so it will be easier to backtrack
             *    when the user wants to lift data out
             */
            vars[current_var]->data_tag = DATA_TAG;
            vars[current_var]->data_ref = sdRef;
            vars[current_var]->HDFtype  = HDFtype;
            vars[current_var]->ndg_ref  = Hnewref(handle->hdf_file);

            /*
             * --------------------------------------------
             * Convert HDF meta-data into netCDF attributes
             * --------------------------------------------
             */
            
            /*
             * If there is an annotation put in 'remarks'
             */
            
            {
                intn len = DFANgetdesclen(handle->path, ndgTag, ndgRef);
                if(len != FAIL) {
                    char *tBuf;
                    
                    tBuf = HDgetspace(sizeof(char) * (len + 1));
                    if(tBuf != NULL) {
                        
                        status = DFANgetdesc(handle->path, ndgTag, ndgRef, tBuf, len);
                        if((status != FAIL) && (tBuf[0] != '\0')){
                            
                            attrs[current_attr++] = 
                                (NC_attr *) NC_new_attr("remarks", 
                                                        NC_CHAR, 
                                                        len, 
                                                        tBuf);
                            
                        }
                        
                        HDfreespace((VOIDP)tBuf);
                        
                    }
                }
            }
            
            /*
             * If there is a label put in 'long_name'
             */
            {
                char label[256];
                
                status = DFANgetlabel(handle->path, ndgTag, ndgRef, label, 250);
                if((status != FAIL) && (label[0] != '\0')){
                    
                    attrs[current_attr++] = 
                        (NC_attr *) NC_new_attr("long_name", 
                                                NC_CHAR, 
                                                HDstrlen(label), 
                                                label);
                    
                }
            }
            
            /*
             * Units => 'units'
             */
            if(unitbuf) {
                attrs[current_attr++] = 
                    (NC_attr *) NC_new_attr("units", 
                                            NC_CHAR, 
                                            HDstrlen((char *)unitbuf), 
                                            (Void *) unitbuf);
                
            }
            
            /*
             * Don't do format cuz HDF doesn't distinguish between C and Fortran
             * Actually, it seems HDF Format == netCDF Fortran Format
             */
            
            /*
             * Add the attributes to the variable
             */ 
            if(current_attr)
                vars[current_var]->attrs = NC_new_array(NC_ATTRIBUTE, 
                                                        current_attr,
                                                        (Void *) attrs);
            else
                vars[current_var]->attrs = NULL;
            
            
            current_var++;
            if(current_var == max_thangs) {
                /* need to allocate more space */    
                max_thangs *= 2;
                
                dims = (NC_dim **) HDregetspace((VOIDP) dims, sizeof(NC_dim *) * max_thangs);
                if(!dims) HRETURN_ERROR(DFE_NOSPACE, FALSE);
                
                vars = (NC_var **) HDregetspace((VOIDP) vars, sizeof(NC_var *) * max_thangs);
                if(!vars) HRETURN_ERROR(DFE_NOSPACE, FALSE);
                
            }
            
            /*
             * De-allocate temporary storage
             */
            if(namebuf)  HDfreespace((VOIDP)namebuf);
            if(scalebuf) HDfreespace((VOIDP)scalebuf);
            if(unitbuf)  HDfreespace((VOIDP)unitbuf);
            HDfreespace((VOIDP)dimsizes);
            HDfreespace((VOIDP)vardims);
            HDfreespace((VOIDP)scaletypes);
            
            /*
             * Look for the next DataSet
             */
            status = Hnextread(aid, ndgTag, DFREF_WILDCARD, DF_CURRENT);
            
        } /* while (more NDGs) */
        
        Hendaccess(aid);
        
        /*
         * Set up the structures in the proper form
         */
        if(current_dim)
            handle->dims = NC_new_array(NC_DIMENSION, current_dim, (Void *) dims);
        else
            handle->dims = NULL;
        
        if(current_var)
            handle->vars = NC_new_array(NC_VARIABLE, current_var, (Void *) vars);
        else
        handle->vars = NULL;
        
    } /* outermost for loop to loop between NDGs and SDGs */
    
    HDfreespace((VOIDP)dims);
    HDfreespace((VOIDP)vars);
    HDfreespace((VOIDP)attrs);

    return TRUE;

} /* hdf_read_ndgs */


/* --------------------------- hdf_read_sds_cdf --------------------------- */
/*
 * Read SDSs out of an HDF file an pretend like they are netCDF objects
 * We can already assume that the file is open and that it is in fact
 *   an HDF file
 */

bool_t
    hdf_read_sds_cdf(xdrs, handlep)
XDR *xdrs;
NC **handlep;
{

    int32  status;
    NC     *handle;
    
    /* 
     * go through and treat each SDS as a separate varibiable 
     */

    /* 
     * should we try to create an unlimited dimension somehow ???
     */
    
    /* we haven't seen any SDG-NDG combos yet */
    sdgTable = NULL;

    handle = (*handlep);
    if(!handle) return FALSE;

    status = hdf_read_ndgs(handle);
    if(!status) return FALSE;

    /* deallocate SDG-NDG space */
    if(sdgTable) HDfreespace((VOIDP)sdgTable);
    sdgTable = NULL;

    return TRUE;

} /* hdf_read_sds_cdf */

#endif /* HDF */
