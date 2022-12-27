/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
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



#include "hdf.h"
#include "mfhdf.h"
#include "hrepack_utils.h"
#include "hrepack_opttable.h"


/*-------------------------------------------------------------------------
 * Function: is_reserved
 *
 * Purpose: check for reserved Vgroup/Vdata class/names
 *
 * Return: 1 if reserved, 0 if not
 *
 * Programmer: Pedro Vicente, pvn@ncsa.uiuc.edu
 *
 * Date: August 22, 2003
 *
 *-------------------------------------------------------------------------
 */

int is_reserved(char*vgroup_class)
{
    int ret=0;
    
    /* ignore reserved HDF groups/vdatas */
    if(vgroup_class != NULL) {
        if( (strcmp(vgroup_class,_HDF_ATTRIBUTE)==0) ||
            (strcmp(vgroup_class,_HDF_VARIABLE) ==0) || 
            (strcmp(vgroup_class,_HDF_DIMENSION)==0) ||
            (strcmp(vgroup_class,_HDF_UDIMENSION)==0) ||
            (strcmp(vgroup_class,DIM_VALS)==0) ||
            (strcmp(vgroup_class,DIM_VALS01)==0) ||
            (strcmp(vgroup_class,_HDF_CDF)==0) ||
            (strcmp(vgroup_class,GR_NAME)==0) ||
            (strcmp(vgroup_class,RI_NAME)==0) || 
            (strcmp(vgroup_class,RIGATTRNAME)==0) ||
            (strcmp(vgroup_class,RIGATTRCLASS)==0) ){
            ret=1;
        }
        
        /* class and name(partial) for chunk table i.e. Vdata */
        if( (strncmp(vgroup_class,"_HDF_CHK_TBL_",13)==0)){
            ret=1;
        }
        
    }
    
    return ret;
}



/*-------------------------------------------------------------------------
 * Function: get_path
 *
 * Purpose: return absolute path for an object
 *
 * Return: path
 *
 * Programmer: Pedro Vicente, pvn@ncsa.uiuc.edu
 *
 * Date: July 11, 2003
 *
 *-------------------------------------------------------------------------
 */

char *get_path(char*path_name, char*obj_name) 
{
    char *path=NULL;
    /* initialize path */
    if (path_name!=NULL) 
    {
        path = (char*) malloc(strlen(path_name) + strlen(obj_name) + 2);
        strcpy( path, path_name );
        strcat( path, "/" );
        strcat( path, obj_name ); 
    }
    else
    {
        path = (char*) malloc(strlen(obj_name) + 1);
        strcpy( path, obj_name ); 
    }
    return path;
}

/*-------------------------------------------------------------------------
 * Function: options_get_info
 *
 * Purpose: get COMP and CHUNK information from options
 *
 * Return: 0 if no information for this PATH, 1 otherwise
 *
 * Programmer: Pedro Vicente, pvn@ncsa.uiuc.edu
 *
 * Date: July 28, 2003
 *
 *-------------------------------------------------------------------------
 */


int  options_get_info(options_t      *options,     /* global options */
                      int32          *chunk_flags, /* chunk flags OUT */
                      HDF_CHUNK_DEF  *chunk_def,   /* chunk definition OUT */
                      int            *info,        /* compression information OUT */
                      int            *szip_mode,   /* compression information OUT */
                      comp_coder_t   *comp_type,   /* compression type OUT  */
                      int            rank,         /* rank of object IN */
                      char           *path,        /* path of object IN */
                      int            ncomps,       /* number of GR image planes (for SZIP), IN */
                      int32          *dimsizes,    /* dimensions (for SZIP), IN */
                      int32          dtype         /* numeric type (for SZIP), IN */
                      )
{
    
    pack_info_t *obj=NULL; /* check if we have information for this object */
    int         i;
    comp_info   c_info; /* for SZIP default values */
    
                        /*-------------------------------------------------------------------------
                        * CASE 1: chunk==ALL comp==SELECTED 
                        *-------------------------------------------------------------------------
    */
    
    if (options->all_chunk==1 && options->all_comp==0)
    {
        /* NONE option */
        if (options->chunk_g.rank==-2)
        {
            chunk_flags = HDF_NONE;
        }
        
        /*check if the input rank is correct (warn this one cannot be chunked) */
        else if (options->chunk_g.rank!=rank)
        {
            if ( options->verbose ) 
                printf("Warning: chunk rank does not apply to <%s>\n",path);
        }
        else
        {
            *chunk_flags = HDF_CHUNK;
            for (i = 0; i < rank; i++) 
                chunk_def->chunk_lengths[i] = options->chunk_g.chunk_lengths[i];
        }
        
        obj = options_get_object(path,options->op_tbl);
        
        if (obj!=NULL )
        {
            
            /* 0 is the NONE option */
            *comp_type   = obj->comp.type;
            *info        = obj->comp.info;
            *szip_mode   = obj->comp.szip_mode;
            
            /* chunk and compress */
            if (*chunk_flags == HDF_CHUNK && *comp_type>0 )
            {
                /* assign the object CHUNK information   */
                *chunk_flags              = HDF_CHUNK | HDF_COMP;
                chunk_def->comp.comp_type = obj->comp.type;
                switch (obj->comp.type)
                {
                case COMP_CODE_NONE:
                    break;
                    
                case COMP_CODE_SZIP:
                    if (set_szip (obj->comp.info,obj->comp.szip_mode,&c_info)==FAIL)
                    {
                        return FAIL;
                    }
                    chunk_def->comp.cinfo = c_info;
                    
                    break;
                case COMP_CODE_RLE:
                    break;
                case COMP_CODE_SKPHUFF:
                    chunk_def->comp.cinfo.skphuff.skp_size = obj->comp.info;
                    break;
                case COMP_CODE_DEFLATE:
                    chunk_def->comp.cinfo.deflate.level    = obj->comp.info;
                    break;
                case COMP_CODE_JPEG:
                    chunk_def->comp.cinfo.jpeg.quality        = obj->comp.info;
                    chunk_def->comp.cinfo.jpeg.force_baseline = 1;
                    break;
                default:
                    printf("Error: Unrecognized compression code in %d <%s>\n",obj->comp.type,path);
                    break;
                }; /*switch */
                for (i = 0; i < rank; i++) 
                {
                    /* To use chunking with RLE, Skipping Huffman, and GZIP compression */
                    chunk_def->comp.chunk_lengths[i] = options->chunk_g.chunk_lengths[i];
                }
            } /* chunk_flags */
        } /* obj */
    }
    
    /*-------------------------------------------------------------------------
    * CASE 2: chunk==SELECTED comp==SELECTED 
    *-------------------------------------------------------------------------
    */
    else if (options->all_chunk==0 && options->all_comp==0)
    {
        obj = options_get_object(path,options->op_tbl);
        
        if (obj!=NULL)
        {
            /* NONE option */
            if (obj->chunk.rank==-2)
            {
                *chunk_flags = HDF_NONE;
            }
            /* check if we have CHUNK information inserted for this one  */
            else if (obj->chunk.rank>0)
            {
                /*check if the input rank is correct (just here, better later than never) */
                if (obj->chunk.rank!=rank)
                {
                    printf("Error: chunk rank does not match for <%s>\n",path);
                    return FAIL;
                }
                *chunk_flags = HDF_CHUNK;
                for (i = 0; i < rank; i++) 
                    chunk_def->chunk_lengths[i] = obj->chunk.chunk_lengths[i];
                
            }
            /* check if we have COMP information; 0 is the NONE option */
            if (obj->comp.type>=0)
            {
                *comp_type   = obj->comp.type;
                *info        = obj->comp.info;
                *szip_mode   = obj->comp.szip_mode;
                /* check if we have also CHUNK info  */
                if (obj->chunk.rank>0)
                {
                    *chunk_flags              = HDF_CHUNK | HDF_COMP;
                    chunk_def->comp.comp_type = *comp_type;
                    switch (*comp_type)
                    {
                    case COMP_CODE_NONE:
                        break;
                        
                    case COMP_CODE_SZIP:
                        if (set_szip (obj->comp.info,obj->comp.szip_mode,&c_info)==FAIL)
                        {
                            return FAIL;
                        }
                        chunk_def->comp.cinfo = c_info;
                        
                        break;
                    case COMP_CODE_RLE:
                        break;
                    case COMP_CODE_SKPHUFF:
                        chunk_def->comp.cinfo.skphuff.skp_size = obj->comp.info;
                        break;
                    case COMP_CODE_DEFLATE:
                        chunk_def->comp.cinfo.deflate.level    = obj->comp.info;
                        break; 
                    case COMP_CODE_JPEG:
                        chunk_def->comp.cinfo.jpeg.quality        = obj->comp.info;
                        chunk_def->comp.cinfo.jpeg.force_baseline = 1;
                        break;
                    default:
                        printf("Error: Unrecognized compression code in %d <%s>\n",*comp_type,path);
                        return FAIL;
                        break;
                    };
                }
            } /* comp.type */
        } /* obj */
    } /* else if */
    
      /*-------------------------------------------------------------------------
      * CASE 3: chunk==SELECTED comp==ALL 
      *-------------------------------------------------------------------------
    */
    else if (options->all_chunk==0 && options->all_comp==1)
    {
        obj = options_get_object(path,options->op_tbl);
        
        if (obj!=NULL)
        {
            
            /* NONE option */
            if (obj->chunk.rank==-2)
            {
                *chunk_flags = HDF_NONE;
            }
            
            /* check if we have CHUNK information inserted for this one  */
            else if (obj->chunk.rank>0)
            {
                /*check if the input rank is correct (just here, better later than never) */
                if (obj->chunk.rank!=rank)
                {
                    printf("Error: chunk rank does not match for <%s>\n",path);
                    return FAIL;
                }
                *chunk_flags = HDF_CHUNK;
                for (i = 0; i < rank; i++) 
                    chunk_def->chunk_lengths[i] = obj->chunk.chunk_lengths[i];
            }
        } /* obj */
        
        /* we must have COMP information */
        
        *comp_type   = options->comp_g.type;
        *info        = options->comp_g.info;
        *szip_mode   = options->comp_g.szip_mode;
        /* check if we have also CHUNK information  */
        if ( (*chunk_flags==HDF_CHUNK) || (*chunk_flags==(HDF_CHUNK|HDF_COMP)))
        {
            *chunk_flags              = HDF_CHUNK | HDF_COMP;
            chunk_def->comp.comp_type = *comp_type;
            switch (*comp_type)
            {
            case COMP_CODE_NONE:
                break;
                
            case COMP_CODE_SZIP:
                if (set_szip (options->comp_g.info,options->comp_g.szip_mode,&c_info)==FAIL)
                {
                    return FAIL;
                }
                chunk_def->comp.cinfo = c_info;
                
            case COMP_CODE_RLE:
                break;
            case COMP_CODE_SKPHUFF:
                chunk_def->comp.cinfo.skphuff.skp_size = *info;
                break;
            case COMP_CODE_DEFLATE:
                chunk_def->comp.cinfo.deflate.level    = *info;
                break;
            case COMP_CODE_JPEG:
                chunk_def->comp.cinfo.jpeg.quality        = *info;;
                chunk_def->comp.cinfo.jpeg.force_baseline = 1;
                break;
            default:
                printf("Error: Unrecognized compression code in %d <%s>\n",*comp_type,path);
                return FAIL;
                break;
            };
        }
    } /* else if */
    
      /*-------------------------------------------------------------------------
      * CASE 4: chunk==ALL comp==ALL 
      *-------------------------------------------------------------------------
    */
    else if (options->all_chunk==1 && options->all_comp==1)
    {
        /* NONE option */
        if (options->chunk_g.rank==-2)
        {
            *chunk_flags = HDF_NONE;
        }
        
        /*check if this object rank is the same as input (warn this one cannot be chunked) */
        else if (options->chunk_g.rank!=rank)
        {
            if ( options->verbose ) 
                printf("Warning: chunk rank does not apply to <%s>\n",path);
        }
        else
        {
            *chunk_flags = HDF_CHUNK;
            for (i = 0; i < rank; i++) 
                chunk_def->chunk_lengths[i] = options->chunk_g.chunk_lengths[i];
        }
        
        /* we must have COMP information */
        *comp_type   = options->comp_g.type;
        *info        = options->comp_g.info;
        *szip_mode   = options->comp_g.szip_mode;
        /* check if we can aplly CHUNK */
        if (options->chunk_g.rank==rank)
        {
            *chunk_flags              = HDF_CHUNK | HDF_COMP;
            chunk_def->comp.comp_type = *comp_type;
            switch (*comp_type)
            {
            case COMP_CODE_NONE:
                break;
                
            case COMP_CODE_SZIP:
                if (set_szip (options->comp_g.info,options->comp_g.szip_mode,&c_info)==FAIL)
                {
                    return FAIL;
                }
                chunk_def->comp.cinfo = c_info;
                
            case COMP_CODE_RLE:
                break;
            case COMP_CODE_SKPHUFF:
                chunk_def->comp.cinfo.skphuff.skp_size = *info;
                break;
            case COMP_CODE_DEFLATE:
                chunk_def->comp.cinfo.deflate.level    = *info;
                break;
            case COMP_CODE_JPEG:
                chunk_def->comp.cinfo.jpeg.quality        = *info;;
                chunk_def->comp.cinfo.jpeg.force_baseline = 1;
                break;
            default:
                printf("Error: Unrecognized compression code in %d <%s>\n",*comp_type,path);
                return FAIL;
                break;
            };
        }
    } /* else if */
    
    return (obj==NULL) ? 0 : 1;
    
    
}



/*-------------------------------------------------------------------------
 * Function: set_szip
 *
 * Purpose: utility to set SZIP parameters
 *
 * SZIP compresses data block by block, with a user-tunable block size. 
 * This block size is passed in the parameter pixels_per_block and must be even, 
 * with typical values being 8, 10, 16, and 32. The more pixel values vary, 
 * the smaller this number should be. For optimal performance, the number of 
 * pixels per scan line (i.e., the size of the fastest-changing dimension in the chunk) 
 * should be an even multiple of the number of pixels per block. 
 *
 * Return: 0 for OK, -1 otherwise
 *
 * Programmer: Pedro Vicente, pvn@ncsa.uiuc.edu
 *
 * Date: August 11, 2003
 *
 *-------------------------------------------------------------------------
 */

int set_szip( int   pixels_per_block, /*in */
              int   compression_mode, /* in */
              comp_info *c_info/*out*/)
{
#ifdef H4_HAVE_LIBSZ
    
    int   ppb=pixels_per_block;
    
    if (SZ_encoder_enabled() == 0) {
        printf("Warning: SZIP encoder is not enabled\n");
        return -1;
    }
    
    if ( (compression_mode!=NN_MODE) && (compression_mode!=EC_MODE))
    {
        printf("SZIP compression mode must be NN_MODE or EC_MODE");
        return -1;
    }
    
    /* 
    pixels_per_block must be an even number, and <= pixels_per_scanline 
    and <= SZ_MAX_PIXELS_PER_BLOCK
    */
    
    if (pixels_per_block & 1)
    {
        printf("Pixels per block must be even.\n");
        return -1;
    }
    if (ppb < 2 || ppb > 32) {
        printf("Pixels per block must be 2-32.\n");
        return -1;
    }
    c_info->szip.pixels_per_block = ppb;
    
    /* set according to input value */
    c_info->szip.options_mask = SZ_EC_OPTION_MASK;
    if (compression_mode == EC_MODE) {
        c_info->szip.options_mask = SZ_EC_OPTION_MASK;
    } else if (compression_mode == NN_MODE) {
        c_info->szip.options_mask = SZ_NN_OPTION_MASK;
    }
    c_info->szip.options_mask |= SZ_RAW_OPTION_MASK;
    
    return 0;
#else
    printf("Warning: SZIP compression is not available\n");
    return -1;
#endif
    
}

/*-------------------------------------------------------------------------
 * Function: cache
 *
 * Purpose: Checks chunk size
 *
 *-------------------------------------------------------------------------
 */

int cache(
HDF_CHUNK_DEF    chunk_def,
int32 eltsz,
int32 rank,
int32 *dimsize)
{
    int32 targetbytes;
    int32 chunkrow;
    int32 chunkcnt;
    int32 chunksizes[32];
    int i;
    int32 cntr;
    
    for (i = 0; i < rank; i++) {
        chunkcnt = 1;
        targetbytes = dimsize[i] * eltsz;
        chunkrow = eltsz * chunk_def.chunk_lengths[i];
        cntr = chunkrow;
        while( cntr < targetbytes) {
            cntr += chunkrow;
            chunkcnt++;
        }
        chunksizes[i] = chunkcnt;
    }
    chunkcnt = 1;
    for (i = 0; i < rank; i++) {
        chunkcnt *= chunksizes[i];
    }
    printf("total chunks is %ld\n",chunkcnt);
    return 0;
}



