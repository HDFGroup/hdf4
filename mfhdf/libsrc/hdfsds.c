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
PRIVATE uint16     *sdgTable=NULL;
PRIVATE uint8      *ptbuf = NULL;

PRIVATE intn hdf_query_seen_sdg
    PROTO((uint16 ndgRef));

PRIVATE VOID hdf_register_seen_sdg
    PROTO((uint16 ndgRef));

PRIVATE intn hdf_read_ndgs
    PROTO((NC *handle));

/* --------------------------- hdf_query_seen_sdg --------------------------- */
/*
 *  The SDG with the given ref number might be part of an SDG-NDG combo
 *    if so, we return TRUE else FALSE.
 *
 *  Attempt to discover if we've seen it in the most inefficient manner
 *    possible
 */
#ifdef PROTOTYPE
PRIVATE intn hdf_query_seen_sdg(uint16 ndgRef)
#else
PRIVATE intn hdf_query_seen_sdg(ndgRef)
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
PRIVATE VOID hdf_register_seen_sdg(uint16 sdgRef)
#else
PRIVATE VOID hdf_register_seen_sdg(sdgRef)
     uint16 sdgRef;
#endif
{

    if(!sdgTable) {
        sdgMax = 100;
        sdgTable = (uint16 *) HDmalloc(sdgMax * sizeof(uint16));
        sdgCurrent = 0;
    }
    
    sdgTable[sdgCurrent++] = sdgRef;
    
    if(sdgCurrent == sdgMax) {
        sdgMax *= 2;
        sdgTable = (uint16 *) HDrealloc((VOIDP) sdgTable, sdgMax * sizeof(uint16));
    }

} /* hdf_register_seen_sdg */


/* -------------------------- hdf_read_ndgs ------------------------------- */
/*
 * Loop through all of the NDGs in the file and create data structures for 
 *  them
 *
 */
#ifdef PROTOTYPE
PRIVATE intn hdf_read_ndgs(NC *handle)
#else
PRIVATE intn hdf_read_ndgs(handle)
     NC *handle;
#endif
{
    static char *FUNC = "hdf_read_ndg_dims";
    char   tmpname[80];
    uint8  ntstring[4];
    intn   dimcount;
    intn   dimattrcnt;

    /* info about NDG structure */
    int32  GroupID, aid, aid1;
    uint16 ndgTag, ndgRef;
    uint16 sddRef, lRef, uRef, fRef, sRef, sdRef;
    uint16 tmpTag, tmpRef;
    int16  rank, type;
    int32  *dimsizes, *scaletypes, HDFtype;
    intn   dim, max_thangs, current_dim, current_var, current_attr;
    intn   *vardims;

    /* info about netCDF structures */
    NC_dim  **dims;   /* hold list of dimensions as we create it */
    NC_var  **vars;   /* hold variable list as we create it      */
    NC_attr **attrs;  /* hold attribute list as we create it     */
    NC_attr *dimattrs[10];  /* for LUF and anno_label, anno_desc, 10 is enough */
    uint8  *labelbuf; /* label should not be used as var name due to non-uniqueness  */
    /* uint8  *namebuf;  */ /* buffer to store label info   */
    uint8  *scalebuf; /* buffer to store scale info */
    uint8  *unitbuf;  /* buffer to store unit info */
    uint8  *formatbuf; /* buffer to store format info */
    uint8  *coordbuf; /* buffer to store coord system info */
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
    dims = (NC_dim **) HDmalloc(sizeof(NC_dim *) * max_thangs);
    if(!dims) {
        HERROR(DFE_NOSPACE);
        return FALSE;
    }
    
    vars = (NC_var **) HDmalloc(sizeof(NC_var *) * max_thangs);
    if(!vars) {
        HERROR(DFE_NOSPACE);
        return FALSE;
    }

    attrs = (NC_attr **) HDmalloc(sizeof(NC_attr *) * max_thangs);
    if(!attrs) {
        HERROR(DFE_NOSPACE);
        return FALSE;
    }

    /* Check if temproray buffer has been allocated */
    if (ptbuf == NULL)
      {
        ptbuf = (uint8 *)HDmalloc(TBUF_SZ * sizeof(uint8));
        if (ptbuf == NULL)
          HRETURN_ERROR(DFE_NOSPACE, FALSE);
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
#ifdef DEBUG
                fprintf(stderr, "Call to Hinquire failed\n");
#endif
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
            
            sddRef = lRef = uRef = fRef = sRef = sdRef = 0;
            
            /* default number type is Float32 */
            type    = NC_FLOAT; 
            HDFtype = DFNT_FLOAT32;
            
            /* no attributes found yet */
            current_attr = 0;
            
            /* no meta-data seen yet */
            labelbuf = unitbuf = scalebuf = formatbuf = NULL;
            
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
                    if (Hread(aid1, (int32) 2, ptbuf) == FAIL) return FALSE;
                    p = ptbuf;
                    INT16DECODE(p, rank);
                    
                    /* get space for dimensions */
                    dimsizes = (int32 *) HDmalloc((uint32) rank * sizeof(int32));
                    if (dimsizes == NULL) return FALSE;
                    vardims = (intn *) HDmalloc((uint32) rank * sizeof(intn));
                    if (vardims == NULL) return FALSE;
                    scaletypes = (int32 *) HDmalloc((uint32) rank * sizeof(int32));
                    if (scaletypes == NULL) return FALSE;
                    
                    /* read dimension record */
                    if (Hread(aid1, (int32) 4 * rank, ptbuf) == FAIL) return FALSE;
                    p = ptbuf;
                    for (i = 0; i < rank; i++)
                        INT32DECODE(p, dimsizes[i]);
                    
                    /* read tag/ref of NT */
                    if (Hread(aid1,(int32) 4,  ptbuf) == FAIL) return FALSE;
                    p = ptbuf;
                    UINT16DECODE(p, ntTag);
                    UINT16DECODE(p, ntRef);
                    
                    /* read actual NT */
                    if (Hgetelement(handle->hdf_file, ntTag, ntRef, ntstring) == FAIL)
                        return FALSE;
                    
                    HDFtype = ntstring[1];
                    type = hdf_unmap_type(HDFtype);
                    
                    /* read in scale NTs */
                    for(i = 0; i < rank; i++) {
                        if (Hread(aid1,(int32) 4,  ptbuf) == FAIL) return FALSE;
                        p = ptbuf;
                        UINT16DECODE(p, ntTag);
                        UINT16DECODE(p, ntRef);
                        
                        /* read NT of this scale (dimension) */
                        if (Hgetelement(handle->hdf_file, ntTag, ntRef, ntstring) == FAIL)
                            return FALSE;
                        
                        scaletypes[i] = ntstring[1];
                    }
                    
                    sddRef = tmpRef;    /* prepare for a new dim var */
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

                case DFTAG_SDC:       /* coord  */
                     /*
                      * DFTAG_SDC => 'cordsys'
                      */
                     { 
                          int len;

                          len = Hlength(handle->hdf_file, DFTAG_SDC, tmpRef);
                          if (len == FAIL) return FALSE;
                          coordbuf = (uint8 *) HDgetspace((uint32) len + 1);
                          if (!coordbuf) HRETURN_ERROR(DFE_NOSPACE, FALSE);
                          if (Hgetelement(handle->hdf_file, DFTAG_SDC, tmpRef, coordbuf) == FAIL)
                              return FALSE;
                          coordbuf[len] = '\0';
                          if (coordbuf[0] != '\0')  {
                              attrs[current_attr] =
                                  (NC_attr *) NC_new_attr("cordsys",
                                      NC_CHAR, HDstrlen(coordbuf), coordbuf);
                              attrs[current_attr++]->HDFtype = DFNT_CHAR;
                          }
                          HDfreespace((VOIDP)coordbuf);
                     }
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
                    if (Hgetelement(handle->hdf_file, tmpTag, tmpRef, ptbuf) == FAIL)
                        return FALSE;
                    
                    if (Hlength(handle->hdf_file, tmpTag, tmpRef) == 36) {
                        /* DFNT_FLOAT64 based calibration */
                        
                        DFKconvert((VOIDP)ptbuf, 
                                   (VOIDP) tBuf, 
                                   DFNT_FLOAT64, 4, DFACC_READ, 0, 0);
                        
                        attrs[current_attr] = 
                            (NC_attr *) NC_new_attr("scale_factor", 
                                                    NC_DOUBLE, 
                                                    1, 
                                                    (Void *) &(tBuf[0]));
                        attrs[current_attr++]->HDFtype = DFNT_FLOAT64; 

                        attrs[current_attr] = 
                            (NC_attr *) NC_new_attr("scale_factor_err", 
                                                    NC_DOUBLE, 
                                                    1, 
                                                    (Void *) &(tBuf[8]));    
                        attrs[current_attr++]->HDFtype = DFNT_FLOAT64;
 
                        attrs[current_attr] = 
                            (NC_attr *) NC_new_attr("add_offset", 
                                                    NC_DOUBLE, 
                                                    1, 
                                                    (Void *) &(tBuf[16]));
                        attrs[current_attr++]->HDFtype = DFNT_FLOAT64;

                        attrs[current_attr] = 
                            (NC_attr *) NC_new_attr("add_offset_err", 
                                                    NC_DOUBLE, 
                                                    1, 
                                                    (Void *) &(tBuf[24]));
                        attrs[current_attr++]->HDFtype = DFNT_FLOAT64;

                        /* don't forget number_type  */
                        DFKconvert((VOIDP)(ptbuf + 32),
                                   (VOIDP) tBuf,
                                   DFNT_INT32, 1, DFACC_READ, 0,0);

                        attrs[current_attr] =
                            (NC_attr *) NC_new_attr("calibrated_nt",
                                                    NC_LONG,
                                                    1,
                                                    (Void *) &(tBuf[0]));
                        attrs[current_attr++]->HDFtype = DFNT_INT32;

                    } else {
                        /* DFNT_FLOAT32 based calibration */

                        DFKconvert((VOIDP)ptbuf, 
                                   (VOIDP)tBuf, 
                                   DFNT_FLOAT32, 4, DFACC_READ, 0, 0);
                        
                        attrs[current_attr] = 
                            (NC_attr *) NC_new_attr("scale_factor", 
                                                    NC_FLOAT, 
                                                    1, 
                                                    (Void *) &(tBuf[0]));
                        attrs[current_attr++]->HDFtype = DFNT_FLOAT32;
 
                        attrs[current_attr] = 
                            (NC_attr *) NC_new_attr("scale_factor_err", 
                                                    NC_FLOAT, 
                                                    1, 
                                                    (Void *) &(tBuf[4]));    
                        attrs[current_attr++]->HDFtype = DFNT_FLOAT32;

                        attrs[current_attr] = 
                            (NC_attr *) NC_new_attr("add_offset", 
                                                    NC_FLOAT, 
                                                    1, 
                                                    (Void *) &(tBuf[8]));
                        attrs[current_attr++]->HDFtype = DFNT_FLOAT32;

                        attrs[current_attr] = 
                            (NC_attr *) NC_new_attr("add_offset_err", 
                                                    NC_FLOAT, 
                                                    1, 
                                                    (Void *) &(tBuf[12]));
                        attrs[current_attr++]->HDFtype = DFNT_FLOAT32;

                       /* don't forget number_type  */
                        DFKconvert((VOIDP)(ptbuf + 16),
                                   (VOIDP) tBuf,
                                   DFNT_INT16, 1, DFACC_READ, 0,0);

                        attrs[current_attr] =
                            (NC_attr *) NC_new_attr("calibrated_nt",
                                                    NC_SHORT,
                                                    1,
                                                    (Void *) &(tBuf[0]));
                        attrs[current_attr++]->HDFtype = DFNT_INT16;
                    }
                    
                    break;
                    
                case DFTAG_SDM:        /* valid range info */
                    
                    if (Hgetelement(handle->hdf_file, tmpTag, tmpRef, ptbuf) == FAIL)
                        return FALSE;
                    
                    DFKconvert((VOIDP)ptbuf, 
                               (VOIDP)tBuf, 
                               HDFtype, 2, DFACC_READ, 0, 0);
                    
                    attrs[current_attr] = 
                        (NC_attr *) NC_new_attr("valid_max", 
                                                type, 
                                                1, 
                                                (Void *) tBuf);
                    attrs[current_attr++]->HDFtype = HDFtype;
                    
                    attrs[current_attr] = 
                        (NC_attr *) NC_new_attr("valid_min", 
                                                type, 
                                                1, 
                                                (Void *) &(tBuf[DFKNTsize(HDFtype)]));
                    attrs[current_attr++]->HDFtype = HDFtype;
                    break;

                case DFTAG_SDLNK:
                    if(ndgTag == DFTAG_SDG) continue;

                    if (Hgetelement(handle->hdf_file, tmpTag, tmpRef, ptbuf) == FAIL) {
                        return FALSE;
                    } else {
                        uint16 sdgTag, sdgRef;

                        p = ptbuf;
                        
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
                
/*                namebuf = (uint8 *) HDgetspace((uint32) len + 3);    */
                labelbuf = (uint8 *) HDmalloc((uint32) len + 3);
                if(!labelbuf) HRETURN_ERROR(DFE_NOSPACE, FALSE);
                
                if(Hgetelement(handle->hdf_file, DFTAG_SDL, lRef, labelbuf) == FAIL)
                    return FALSE;
                
                labelbuf[len + 2] = '\0';
                labelbuf[len + 1] = '\0';
                labelbuf[len + 0] = '\0';
                
            } else {
                /* namebuf = NULL;    */
                labelbuf = NULL;
            }
            
            if(uRef) {
                int len;
                
                len = Hlength(handle->hdf_file, DFTAG_SDU, uRef);
                if(len == FAIL) return FALSE;
                
                unitbuf = (uint8 *) HDmalloc((uint32) len+3);
                if(!unitbuf) HRETURN_ERROR(DFE_NOSPACE, FALSE);
                
                if(Hgetelement(handle->hdf_file, DFTAG_SDU, uRef, unitbuf) == FAIL)
                    return FALSE;

                unitbuf[len + 2] = '\0';
                unitbuf[len + 1] = '\0';
                unitbuf[len + 0] = '\0';

                
            } else {
                unitbuf = NULL;
            }
           
            if(fRef) {
                int len;

                len = Hlength(handle->hdf_file, DFTAG_SDF, fRef);
                if(len == FAIL) return FALSE;

                formatbuf = (uint8 *) HDgetspace((uint32) len+3);
                if(!formatbuf) HRETURN_ERROR(DFE_NOSPACE, FALSE);

                if(Hgetelement(handle->hdf_file, DFTAG_SDF, fRef, formatbuf) == FAIL)
                    return FALSE;

                formatbuf[len + 2] = '\0';
                formatbuf[len + 1] = '\0';
                formatbuf[len + 0] = '\0';


            } else {
                formatbuf = NULL;
            }

 
            if(sRef) {
                int len;
                
                len = Hlength(handle->hdf_file, DFTAG_SDS, sRef);
                if(len == FAIL) return FALSE;
                
                scalebuf = (uint8 *) HDmalloc((uint32) len);
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
                char *labelvalue  = NULL, *unitvalue = NULL, *formatvalue = NULL;

                /* now loop though each dimension
                   - get the size from dimsize[i]
                   - lref will give the ref of the label descriptor to see if 
                   has a real name else fake one based on the label of the NDG
                   - look at uref for units information and fref for formating 
                     info store both of these as attributes of the coordinate 
                     variable
                   - Promote the dimension to a variable if any of the LUF or
                     scales is assigned to this dimentsion. 
                   - The dimension variable name is the same as the dimension 
                     record name, fakeDim<dim count>.
                   */
               
                new_dim = FALSE; 
                tmpname[0] = '\0';
                
                if(lRef) {
                    labelvalue = (char *) labelbuf;
                    for(i = 0; i < dim + 1; i++) labelvalue += HDstrlen(labelvalue) + 1;
                    if (labelvalue[0] != '\0') new_dim = TRUE;
                }
                
                if(uRef) {
                    unitvalue = (char *) unitbuf;
                    for(i = 0; i < dim + 1; i++) unitvalue += HDstrlen(unitvalue) + 1;
                    if (unitvalue[0] != '\0') new_dim = TRUE;
                }

                if(fRef) {
                    formatvalue = (char *) formatbuf;
                    for(i = 0; i < dim + 1; i++) formatvalue += HDstrlen(formatvalue) + 1;
                    if (formatvalue[0] != '\0') new_dim = TRUE;
                }

                sprintf(tmpname, "fakeDim%d", dimcount++);
                
                this_dim = current_dim++;
                if(current_dim == max_thangs) {
                     /* need to allocate more space */    
                     max_thangs *= 2;
                     dims = (NC_dim **) HDrealloc((VOIDP) dims, sizeof(NC_dim *) * max_thangs);
                     if(!dims) HRETURN_ERROR(DFE_NOSPACE, FALSE);
                     vars = (NC_var **) HDrealloc((VOIDP) vars, sizeof(NC_var *) * max_thangs);
                     if(!vars) HRETURN_ERROR(DFE_NOSPACE, FALSE);
                }

                /*
                 * Create a new dimension record for each dimension. 
                 * Each NC_dim takes 8 bytes in memory. 
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
                if(new_dim || (scalebuf && scalebuf[dim])) {
                    vars[current_var] = NC_new_var(tmpname, 
                                                   hdf_unmap_type(scaletypes[dim]),
                                                   1, 
                                                   &this_dim);
                    vars[current_var]->data_tag = DFTAG_SDS;  /* not normal data */
                    vars[current_var]->data_ref = sRef;
                    vars[current_var]->HDFtype  = scaletypes[dim];
                    vars[current_var]->ndg_ref  = Htagnewref(handle->hdf_file,DFTAG_NDG);

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
                    /*
                     * Convert dimstrs into attributes  
                     * label -- "long_name" (cuz SDsetdimstrs() assigns "long_name" to label)
                     * unit  -- "units"
                     * format -- "format"
                     */

                    /* label => "long_name"  */
                    dimattrcnt = 0;
                    if (labelvalue && HDstrlen((char *)labelvalue)>0) {
                        dimattrs[dimattrcnt] =
                           (NC_attr *) NC_new_attr("long_name", NC_CHAR,
                                                    HDstrlen((char *)labelvalue),
                                                      (Void *) labelvalue);
                        dimattrs[dimattrcnt++]->HDFtype = DFNT_CHAR;
                    }

                    /* Units => 'units' */
                    if(unitvalue && HDstrlen((char *)unitvalue)>0) {
                         dimattrs[dimattrcnt] =
                             (NC_attr *) NC_new_attr("units", NC_CHAR,
                                           HDstrlen((char *)unitvalue),
                                           (Void *) unitvalue);
                         dimattrs[dimattrcnt++]->HDFtype = DFNT_CHAR;
                    }

                    /* Fomrat => 'format' */
                    if(formatvalue && HDstrlen((char *)formatvalue)>0) {
                         dimattrs[dimattrcnt] =
                             (NC_attr *) NC_new_attr("format", NC_CHAR,
                                           HDstrlen((char *)formatvalue),
                                           (Void *) formatvalue);
                         dimattrs[dimattrcnt++]->HDFtype = DFNT_CHAR;
                    }

                    /*
                     * Add the attributes to the variable
                     */
                    if(dimattrcnt)
                        vars[current_var]->attrs = NC_new_array(NC_ATTRIBUTE,
                                                      dimattrcnt,
                                                      (Void *) dimattrs);
                    else
                        vars[current_var]->attrs = NULL;

                    current_var++;  
                    if(current_var == max_thangs) {
                        /* need to allocate more space */    
                        max_thangs *= 2;

                        dims = (NC_dim **) HDrealloc((VOIDP)dims, sizeof(NC_dim *) * max_thangs);
                        if(!dims) HRETURN_ERROR(DFE_NOSPACE, FALSE);
                        
                        vars = (NC_var **) HDrealloc((VOIDP)vars, sizeof(NC_var *) * max_thangs);
                        if(!vars) HRETURN_ERROR(DFE_NOSPACE, FALSE);
                    }
                }
            }
            
            /*
             * Should the LUF-label be mapped as attr of "longname", to be consistent
             *   with the dim vars? 8/18/94
             * Should the annotation-label mapped to attr "anno-label", if "longname"
             *   has been taken by LUF-label?  8/18/94.
             *   
             * (If there is a data label use that as the variable name else) 
             * Use the reference number of the NDG as part of
             *    a made up name (Label is mapped as attr "longname" 9/2/94).
             *
             * Convert spaces in the name to underscores (yuck) otherwise
             *    ncgen will barf on ncdumped files)
             */
            /* if(labelbuf && (labelbuf[0] != '\0')) {
                char *c;
                for(c = (char *)labelbuf; *c; c++)
                    if((*c) == ' ') (*c) = '_';

                vars[current_var] = NC_new_var((char *) labelbuf, type, (int) rank, vardims);
            } else {
            */
                sprintf(tmpname, "Data-Set-%d", ndgRef); 
                vars[current_var] = NC_new_var(tmpname, type, (int) rank, vardims);
            /*  }
             */
 
            /*
             * Fill in extra information so it will be easier to backtrack
             *    when the user wants to lift data out
             */
            vars[current_var]->data_tag = DATA_TAG;
            vars[current_var]->data_ref = sdRef;
            vars[current_var]->HDFtype  = HDFtype;

             /*
              * NOTE:  If the user changes the file and saves setting this
              *   to ndgRef will blow away the old ndgs (but they will get
              *   rewritten).  Otherwise calls to SDidtoref() will return
              *   bogus values
              */
            vars[current_var]->ndg_ref  = ndgRef; 

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
                    
                    tBuf = (char *)HDmalloc(sizeof(char) * (len + 1));
                    if(tBuf != NULL) {
                        
                        status = DFANgetdesc(handle->path, ndgTag, ndgRef, tBuf, len);
                        if((status != FAIL) && (tBuf[0] != '\0')){
                            
                            attrs[current_attr] = 
                                (NC_attr *) NC_new_attr("remarks", 
                                                        NC_CHAR, 
                                                        len, 
                                                        tBuf);
                            attrs[current_attr++]->HDFtype = DFNT_CHAR;
                            
                        }
                        
                        HDfree((VOIDP)tBuf);
                        
                    }
                }
            }
            
            /*
             * If there is a label put in attr 'anno_label' (note: NOT 'long_name' 9/2/94)
             */
            {
                char label[256];
                
                status = DFANgetlabel(handle->path, ndgTag, ndgRef, label, 250);
                if((status != FAIL) && (label[0] != '\0')){
                    
                    attrs[current_attr] = 
                        (NC_attr *) NC_new_attr("anno_label", 
                                                NC_CHAR, 
                                                HDstrlen(label), 
                                                label);
                    attrs[current_attr++]->HDFtype = DFNT_CHAR;
                }
            }
            
            /* 
             * Label => 'long_name'
             */
            if(labelbuf && (labelbuf[0] != '\0')) {
                attrs[current_attr] =
                    (NC_attr *) NC_new_attr("long_name",
                                            NC_CHAR,
                                            HDstrlen((char *)labelbuf),
                                            (Void *) labelbuf);
                attrs[current_attr++]->HDFtype = DFNT_CHAR;
            }
 
            /*
             * Units => 'units'
             */
            if(unitbuf && (unitbuf[0] != '\0')) {
                attrs[current_attr] = 
                    (NC_attr *) NC_new_attr("units", 
                                            NC_CHAR, 
                                            HDstrlen((char *)unitbuf), 
                                            (Void *) unitbuf);
                attrs[current_attr++]->HDFtype = DFNT_CHAR;
            }
            
            /*
             * (Don't do format cuz HDF doesn't distinguish between C and Fortran
             * Actually, it seems HDF Format == netCDF Fortran Format)
             * Don't use 'C_format' or 'FORTRAN_format'
             * Format => 'format'
             */
            if(formatbuf && (formatbuf[0] != '\0')) {
                attrs[current_attr] =
                    (NC_attr *) NC_new_attr("format",
                                            NC_CHAR,
                                            HDstrlen((char *)formatbuf),
                                            (Void *) formatbuf);
                 attrs[current_attr++]->HDFtype = DFNT_CHAR;
            }

            
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
                
                dims = (NC_dim **) HDrealloc((VOIDP) dims, sizeof(NC_dim *) * max_thangs);
                if(!dims) HRETURN_ERROR(DFE_NOSPACE, FALSE);
                
                vars = (NC_var **) HDrealloc((VOIDP) vars, sizeof(NC_var *) * max_thangs);
                if(!vars) HRETURN_ERROR(DFE_NOSPACE, FALSE);
                
            }
            
            /*
             * De-allocate temporary storage
             */
            /* if(namebuf)  HDfreespace((VOIDP)namebuf);  */
            if(labelbuf)  HDfree((VOIDP)labelbuf);
            if(scalebuf) HDfree((VOIDP)scalebuf);
            if(unitbuf)  HDfree((VOIDP)unitbuf);
            if(formatbuf)  HDfree((VOIDP)formatbuf);
            HDfree((VOIDP)dimsizes);
            HDfree((VOIDP)vardims);
            HDfree((VOIDP)scaletypes);
            
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
    
    HDfree((VOIDP)dims);
    HDfree((VOIDP)vars);
    HDfree((VOIDP)attrs);

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
    HDfreenclear(sdgTable);

    handle = (*handlep);
    if(!handle) return FALSE;

    status = hdf_read_ndgs(handle);
    if(!status) return FALSE;

    /* deallocate SDG-NDG space */
    if(sdgTable) HDfree((VOIDP)sdgTable);
    sdgTable = NULL;

    return TRUE;

} /* hdf_read_sds_cdf */

#endif /* HDF */
