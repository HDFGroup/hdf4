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



#include <assert.h>
#include "hdf.h"
#include "mfhdf.h"


#include "hrepack_sdutil.h"
#include "hrepack_parse.h"
#include "hrepack_opttable.h"




/*-------------------------------------------------------------------------
 * Function: options_get_info
 *
 * Purpose: get COMP and CHUNK info from options
 *
 * Return: 0 if no info for this PATH, 1 otherwise
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
                      int            *info,        /* compression info OUT */
                      comp_coder_t   *comp_type,   /* compression type OUT  */
                      int            rank,         /* rank of object IN */
                      char           *path,        /* path of object IN */
                      int32          *dimsizes,    /* dimensions (for SZIP), IN */
                      int32          dtype         /* numeric type (for SZIP), IN */
                      )
{

 pack_info_t *obj=NULL; /* check if we have info for this object */
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
   
   /* chunk and compress */
   if (*chunk_flags == HDF_CHUNK && *comp_type>0 )
   {
    /* assign the object CHUNK info   */
    *chunk_flags              = HDF_CHUNK | HDF_COMP;
    chunk_def->comp.comp_type = obj->comp.type;
    switch (obj->comp.type)
    {
    case COMP_CODE_NONE:
     break;
     
    case COMP_CODE_SZIP:
     if (set_szip (rank,dimsizes,dtype,&c_info)==FAIL)
     {
      printf( "Error: Failed to get SZIP compression for <%s>\n", path);
      return -1;
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
   /* check if we have CHUNK info inserted for this one  */
   else if (obj->chunk.rank>0)
   {
    /*check if the input rank is correct (just here, better later than never) */
    if (obj->chunk.rank!=rank)
    {
     printf("Error: chunk rank does not match for <%s>\n",path);
     exit(1);
    }
    *chunk_flags = HDF_CHUNK;
    for (i = 0; i < rank; i++) 
     chunk_def->chunk_lengths[i] = obj->chunk.chunk_lengths[i];
    
   }
   /* check if we have COMP info; 0 is the NONE option */
   if (obj->comp.type>=0)
   {
    *comp_type   = obj->comp.type;
    *info        = obj->comp.info;
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
      if (set_szip (rank,dimsizes,dtype,&c_info)==FAIL)
      {
       printf( "Error: Failed to get SZIP compression for <%s>\n", path);
       return -1;
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
   
   /* check if we have CHUNK info inserted for this one  */
   else if (obj->chunk.rank>0)
   {
    /*check if the input rank is correct (just here, better later than never) */
    if (obj->chunk.rank!=rank)
    {
     printf("Error: chunk rank does not match for <%s>\n",path);
     exit(1);
    }
    *chunk_flags = HDF_CHUNK;
    for (i = 0; i < rank; i++) 
     chunk_def->chunk_lengths[i] = obj->chunk.chunk_lengths[i];
   }
  } /* obj */
  
  /* we must have COMP info */
  
  *comp_type   = options->comp_g.type;
  *info        = options->comp_g.info;
  /* check if we have also CHUNK info  */
  if (*chunk_flags==HDF_CHUNK)
  {
   *chunk_flags              = HDF_CHUNK | HDF_COMP;
   chunk_def->comp.comp_type = *comp_type;
   switch (*comp_type)
   {
   case COMP_CODE_NONE:
    break;
    
   case COMP_CODE_SZIP:
    if (set_szip (rank,dimsizes,dtype,&c_info)==FAIL)
    {
     printf( "Error: Failed to get SZIP compression for <%s>\n", path);
     return -1;
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
  
  /* we must have COMP info */
  *comp_type   = options->comp_g.type;
  *info        = options->comp_g.info;
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
    if (set_szip (rank,dimsizes,dtype,&c_info)==FAIL)
    {
     printf( "Error: Failed to get SZIP compression for <%s>\n", path);
     return -1;
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
 * Return: 0 for OK, 1 otherwise
 *
 * Programmer: Pedro Vicente, pvn@ncsa.uiuc.edu
 *
 * Date: August 11, 2003
 *
 *-------------------------------------------------------------------------
 */

int set_szip(int32 rank, int32 *dim_sizes, int32 dtype, comp_info *c_info)
{
 int32	pixels_per_scanline;
 int   i;

 pixels_per_scanline               = dim_sizes[rank-1];
 c_info->szip.pixels               = 1;
 for ( i = 0; i < rank; i++)
 {
  c_info->szip.pixels             *= dim_sizes[i];
 }
 c_info->szip.pixels_per_block     = 2;
 if(pixels_per_scanline >=2048)
  c_info->szip.pixels_per_scanline = 512;
 else
  c_info->szip.pixels_per_scanline = dim_sizes[rank-1];
 
 c_info->szip.options_mask = NN_OPTION_MASK;
 c_info->szip.options_mask |= RAW_OPTION_MASK;
 
 switch(dtype) 
 {
 case DFNT_INT8:
 case DFNT_UINT8:
 case DFNT_UCHAR8:
  c_info->szip.bits_per_pixel = 8;
  break;
 case DFNT_INT16:
 case DFNT_UINT16:
  c_info->szip.bits_per_pixel = 16;
  break;
 case DFNT_INT32:
 case DFNT_UINT32:
  c_info->szip.bits_per_pixel = 32;
  break;
 case DFNT_FLOAT:
  c_info->szip.bits_per_pixel = 32;
  break;
 case DFNT_DOUBLE:
  c_info->szip.bits_per_pixel = 64;
  break;
 default:
  printf("Error: Bad numeric type <%d> in SZIP\n",dtype);
  return -1;
 }

 return 0;
}




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
	printf("total chunks is %d\n",chunkcnt);
 return 0;
}


