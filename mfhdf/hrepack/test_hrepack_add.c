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


#include "hdf.h"
#include "mfhdf.h"

#include "hrepack.h"
#include "pal_rgb.h"
#include "test_hrepack_add.h"



/* globals for read image data, used in gr, r8 and r24 add */
int      X_LENGTH;
int      Y_LENGTH;
int      N_COMPS;
unsigned char *image_data = 0;   



/*-------------------------------------------------------------------------
 * Function: add_gr_ffile
 *
 * Purpose: utility function to read an image data file and save the image with the
 *  GR - Multifile General Raster Image Interface,
 *  optionally inserting the image into the group VGROUP_ID
 *
 * Return: void
 *
 * Programmer: Pedro Vicente, pvn@ncsa.uiuc.edu
 *
 * Date: July 3, 2003
 *
 *-------------------------------------------------------------------------
 */

void add_gr_ffile(char* name_file,
                  char* gr_name,
                  int32 interlace_mode,
                  int32 file_id,
                  int32 vgroup_id)
{
 int32  gr_id,          /* GR interface identifier */
        ri_id,          /* raster image identifier */
        gr_ref,         /* reference number of the GR image */
        start[2],       /* start position to write for each dimension */
        edges[2],       /* number of elements to be written along each dimension */
        dim_gr[2],      /* dimension sizes of the image array */
        data_type;      /* data type of the image data */
 char   *srcdir = getenv("srcdir"); /* the source directory */
 char   data_file[512]="";          /* buffer to hold name of existing data file */
 uint8  attr_values[2]={1,2};
 int    n_values;
 
 /* compose the name of the file to open, using the srcdir, if appropriate */
 if ( srcdir )
 {
  strcpy(data_file, srcdir);
  strcat(data_file, "/");
 }
 strcat( data_file, name_file);
 if ( read_data(data_file)>0)
 {
  /* set the data type, interlace mode, and dimensions of the image */
  data_type = DFNT_UINT8;
  dim_gr[0] = X_LENGTH;
  dim_gr[1] = Y_LENGTH;
  
  
  /* initialize the GR interface */
  if ((gr_id = GRstart (file_id))== FAIL)
  {
   printf("Error: Could not start GR interface\n");
  }
  
  /* create the raster image array */
  if ((ri_id = GRcreate (gr_id, gr_name, N_COMPS, data_type, interlace_mode, dim_gr))== FAIL)
  {
   printf("Error: Could not create GR <%s>\n", gr_name);
  }
  
  /* define the size of the data to be written */
  start[0] = start[1] = 0;
  edges[0] = X_LENGTH;
  edges[1] = Y_LENGTH;
  
  
  /* write the data in the buffer into the image array */
  if (GRwriteimage(ri_id, start, NULL, edges, (VOIDP)image_data)==FAIL)
  {
   printf("Error: Could not write GR <%s>\n", gr_name);
  }

  /* assign an attribute to the SDS */
  n_values = 2;
  if(GRsetattr(ri_id, "Myattr", DFNT_UINT8, n_values, (VOIDP)attr_values)==FAIL)
  {
   printf("Error: Could not write attributes for GR <%s>\n", gr_name);
  }
  
  /* obtain the reference number of the GR using its identifier */
  gr_ref = GRidtoref (ri_id);


#if defined( HZIP_DEBUG)
  printf("add_gr %d\n",gr_ref); 
#endif
  
  /* add the GR to the vgroup. the tag DFTAG_RIG is used */
  if (vgroup_id) {
   if (Vaddtagref (vgroup_id, TAG_GRP_IMAGE, gr_ref)==FAIL)
   {
    printf("Error: Could not add GR <%s> to group\n", gr_name);
   }
  }
   
   /* terminate access to the raster image */
   if (GRendaccess (ri_id)==FAIL)
   {
    printf("Error: Could not close GR <%s>\n", gr_name);
   }
   
   /* terminate access to the GR interface */
   if (GRend (gr_id)==FAIL)
   {
    printf("Error: Could not close GR interface\n");
   }
   
   
   /* add an annotation and label to the object */
   add_an(file_id, DFTAG_RI, gr_ref);

 }  /* read data */

 if ( image_data )
 {
  free( image_data );
  image_data=NULL;
 }


}


/*-------------------------------------------------------------------------
 * Function: add_gr
 *
 * Purpose: utility function to write images with the
 *  GR - Multifile General Raster Image Interface,
 *  optionally inserting the image into the group VGROUP_ID
 *
 * Return: void
 *
 * Programmer: Pedro Vicente, pvn@ncsa.uiuc.edu
 *
 * Date: August 18, 2003
 *
 *-------------------------------------------------------------------------
 */

/* dimensions of image */
#define X_DIM_GR     6
#define Y_DIM_GR     4

void add_gr(char* gr_name,           /* gr name */
            int32 file_id,           /* file ID */
            int32 vgroup_id,         /* group ID */
            int32 chunk_flags,       /* chunk flags */
            int32 comp_type,         /* compression flag */
            comp_info *comp_info     /* compression structure */ )
{
 int32  gr_id,          /* GR interface identifier */
        ri_id,          /* raster image identifier */
        gr_ref,         /* reference number of the GR image */
        start[2],       /* start position to write for each dimension */
        edges[2],       /* number of elements to be written along each dimension */
        dim_gr[2],      /* dimension sizes of the image array */
        interlace_mode, /* interlace mode of the image */
        data_type,      /* data type of the image data */
        data[Y_DIM_GR][X_DIM_GR];
 int    i, j, n=0, ncomps=1;
 HDF_CHUNK_DEF chunk_def;           /* Chunking definitions */ 
 int32         pixels_per_scanline;
 
 /* set the data type, interlace mode, and dimensions of the image */
 data_type = DFNT_UINT32;
 interlace_mode = MFGR_INTERLACE_PIXEL;
 dim_gr[0] = Y_DIM_GR;
 dim_gr[1] = X_DIM_GR;

 /* data set data initialization */
 for (j = 0; j < Y_DIM_GR; j++) {
  for (i = 0; i < X_DIM_GR; i++)
   data[j][i] = n++;
 }

 /*define some compression specific parameters */
 switch(comp_type)
 {
 case COMP_CODE_RLE:
  break;

 case COMP_CODE_SKPHUFF:
  comp_info->skphuff.skp_size = 1;
  break;

 case COMP_CODE_DEFLATE:
  comp_info->deflate.level = 6;
  break;
  
 case COMP_CODE_SZIP:
  pixels_per_scanline = dim_gr[1];
  comp_info->szip.pixels = dim_gr[0]*dim_gr[1];
  comp_info->szip.pixels_per_block = 2;
  if(pixels_per_scanline >=2048)
   comp_info->szip.pixels_per_scanline = 512;
  else
   comp_info->szip.pixels_per_scanline = dim_gr[1];
  comp_info->szip.options_mask = NN_OPTION_MASK;
  comp_info->szip.options_mask |= RAW_OPTION_MASK;
  comp_info->szip.bits_per_pixel = 32;
  break;
 }
 
 /* initialize the GR interface */
 if ((gr_id = GRstart (file_id))== FAIL)
 {
  printf("Error: Could not start GR interface\n");
 }
 
 /* create the raster image array */
 if ((ri_id = GRcreate (gr_id, gr_name, ncomps, data_type, interlace_mode, dim_gr))== FAIL)
 {
  printf("Error: Could not create GR <%s>\n", gr_name);
 }

 /* set chunk */
 if ( (chunk_flags == HDF_CHUNK) || (chunk_flags == (HDF_CHUNK | HDF_COMP)) )
 {
  /* Define chunk's dimensions */
  chunk_def.chunk_lengths[0] = Y_DIM_GR/2;
  chunk_def.chunk_lengths[1] = X_DIM_GR/2;
  /* To use chunking with RLE, Skipping Huffman, and GZIP compression */
  chunk_def.comp.chunk_lengths[0] = Y_DIM_GR/2;
  chunk_def.comp.chunk_lengths[1] = X_DIM_GR/2;

  /*define some compression specific parameters */
  switch(comp_type)
  {
  case COMP_CODE_RLE:
   chunk_def.comp.comp_type = COMP_CODE_RLE;
   break;
   
  case COMP_CODE_SKPHUFF:
   chunk_def.comp.comp_type = COMP_CODE_SKPHUFF;
   chunk_def.comp.cinfo.skphuff.skp_size = 1;
   break;
   
  case COMP_CODE_DEFLATE:
   /* GZIP compression, set compression type, flag and deflate level*/
   chunk_def.comp.comp_type = COMP_CODE_DEFLATE;
   chunk_def.comp.cinfo.deflate.level = 6;
   break;
   
  case COMP_CODE_SZIP:
   pixels_per_scanline = dim_gr[1];
   chunk_def.comp.cinfo.szip.pixels = dim_gr[0]*dim_gr[1];
   chunk_def.comp.cinfo.szip.pixels_per_block = 2;
   if(pixels_per_scanline >=2048)
    chunk_def.comp.cinfo.szip.pixels_per_scanline = 512;
   else
    chunk_def.comp.cinfo.szip.pixels_per_scanline = dim_gr[1];
   chunk_def.comp.cinfo.szip.options_mask = NN_OPTION_MASK;
   chunk_def.comp.cinfo.szip.options_mask |= RAW_OPTION_MASK;
   chunk_def.comp.cinfo.szip.bits_per_pixel = 32;
   break;
  }
  if(GRsetchunk (ri_id, chunk_def, chunk_flags)==FAIL)
  {
   printf("Error: Could not set chunk for GR <%s>\n", gr_name);
  }
 }
 
 /* use compress without chunk-in */
 else if ( (chunk_flags==HDF_NONE || chunk_flags==HDF_CHUNK) && 
  comp_type>COMP_CODE_NONE && comp_type<COMP_CODE_INVALID)
 {
  if(GRsetcompress (ri_id, comp_type, comp_info)==FAIL)
  {
   printf("Error: Could not set compress for GR <%s>\n", gr_name);
  }
 }

 
 /* define the size of the data to be written */
 start[0] = start[1] = 0;
 edges[0] = Y_DIM_GR;
 edges[1] = X_DIM_GR;
 
 /* write the data in the buffer into the image array */
 if (GRwriteimage(ri_id, start, NULL, edges, (VOIDP)data)==FAIL)
 {
  printf("Error: Could not set write GR <%s>\n", gr_name);
 }
   
 /* obtain the reference number of the GR using its identifier */
 gr_ref = GRidtoref (ri_id);
 
#if defined( HZIP_DEBUG)
 printf("add_gr %d\n",gr_ref); 
#endif
 
 /* add the GR to the vgroup. the tag DFTAG_RIG is used */
 if (vgroup_id)
  if (Vaddtagref (vgroup_id, TAG_GRP_IMAGE, gr_ref)==FAIL)
  {
   printf("Error: Could not add GR <%s> to group\n", gr_name);
  }
 
 /* terminate access to the raster image */
 if (GRendaccess (ri_id)==FAIL)
 {
  printf("Error: Could not close GR <%s>\n", gr_name);
 }
 
 /* terminate access to the GR interface */
 if (GRend (gr_id)==FAIL)
 {
  printf("Error: Could not close GR interface\n");
 }

 /* add an annotation and label to the object */
 add_an(file_id, DFTAG_RI, gr_ref);
 
}

/*-------------------------------------------------------------------------
 * Function: add_glb_attrs
 *
 * Purpose: utility function to write global attributes
 *
 * Return: void
 *
 * Programmer: Pedro Vicente, pvn@ncsa.uiuc.edu
 *
 * Date: July 30, 2003
 *
 *-------------------------------------------------------------------------
 */

void add_glb_attrs(char *fname,
                   int32 file_id)
{
 intn  status;                 /* status for functions returning an intn */
 int32 sd_id,                  /* SD interface identifier */
       gr_id;                  /* GR interface identifier */
 uint8 attr_values[2]={1,2};
 int   n_values=2;
     
/*-------------------------------------------------------------------------
 * make SDS global attributes
 *-------------------------------------------------------------------------
 */ 
 
 /* initialize the SD interface */
 sd_id  = SDstart (fname, DFACC_WRITE);

 /* assign an attribute to the SD */
 status = SDsetattr(sd_id, "MySDgattr", DFNT_UINT8, n_values, (VOIDP)attr_values);

 /* terminate access to the SD interface */
 status = SDend (sd_id);

/*-------------------------------------------------------------------------
 * make GR global attributes
 *-------------------------------------------------------------------------
 */ 

 gr_id  = GRstart(file_id);
 
 /* assign an attribute to the GR */
 status = GRsetattr(gr_id, "MyGRgattr", DFNT_UINT8, n_values, (VOIDP)attr_values);

 /* terminate access to the GR interface */
 status = GRend (gr_id);
}


/*-------------------------------------------------------------------------
 * Function: add_r8
 *
 * Purpose: utility function to read an image data file and save the image with the
 *  DFR8 - Single-file 8-Bit Raster Image Interface,
 *  optionally inserting the image into the group VGROUP_ID
 *
 * Return: void
 *
 * Programmer: Pedro Vicente, pvn@ncsa.uiuc.edu
 *
 * Date: July 3, 2003
 *
 *-------------------------------------------------------------------------
 */

void add_r8(char* image_file,char *fname,int32 file_id,int32 vgroup_id)
{
 intn   status_n;       /* returned status_n for functions returning an intn  */
 int32  status_32,      /* returned status_n for functions returning an int32 */
        ri_ref;         /* reference number of the GR image */
 char   *srcdir = getenv("srcdir"); /* the source directory */
 char   data_file[512]="";          /* buffer to hold name of existing data file */
 
 /* compose the name of the file to open, using the srcdir, if appropriate */
 if ( srcdir )
 {
  strcpy(data_file, srcdir);
  strcat(data_file, "/");
 }
 strcat( data_file, image_file);
 if ( read_data(data_file)>0)
 {
  /* add a palette */
  status_n = DFR8setpalette(pal_rgb);

  /* write the image */
  status_n = DFR8addimage(fname, image_data, X_LENGTH, Y_LENGTH, 0);
  
  /* obtain the reference number of the RIS8 */
  ri_ref = DFR8lastref();

#if defined( HZIP_DEBUG)
  printf("add_r8 %d\n",ri_ref); 
#endif
  
  /* add the image to the vgroup. the tag DFTAG_RIG is used */
  if (vgroup_id)
   status_32 = Vaddtagref (vgroup_id, TAG_GRP_IMAGE, ri_ref);

  /* add an annotation and label to the object */
  add_an(file_id, TAG_GRP_IMAGE, ri_ref);
 }

 if ( image_data )
 {
  free( image_data );
  image_data=NULL;
 }

}


/*-------------------------------------------------------------------------
 * Function: add_r24
 *
 * Purpose: utility function to read an image data file and save the image with the
 *  DF24 - Single-file 24-Bit Raster Image Interface,
 *  optionally inserting the image into the group VGROUP_ID
 *
 * Return: void
 *
 * Programmer: Pedro Vicente, pvn@ncsa.uiuc.edu
 *
 * Date: July 3, 2003
 *
 *-------------------------------------------------------------------------
 */

void add_r24(char* image_file,char *fname,int32 file_id,intn il,int32 vgroup_id)
{
 intn   status_n;       /* returned status_n for functions returning an intn  */
 int32  status_32,      /* returned status_n for functions returning an int32 */
        ri_ref;         /* reference number of the GR image */
 char   *srcdir = getenv("srcdir"); /* the source directory */
 char   data_file[512]="";          /* buffer to hold name of existing data file */
 
 /* compose the name of the file to open, using the srcdir, if appropriate */
 if ( srcdir )
 {
  strcpy(data_file, srcdir);
  strcat(data_file, "/");
 }
 strcat( data_file, image_file);
 if ( read_data(data_file)>0)
 {
  /* set pixel interlace */
  status_n = DF24setil(il);

  /* write the image */
  status_n = DF24addimage(fname, image_data, X_LENGTH, Y_LENGTH);
  
  /* obtain the reference number of the RIS24 */
  ri_ref = DF24lastref();

#if defined( HZIP_DEBUG)
  printf("add_r24 %d\n",ri_ref); 
#endif
  
  /* add the image to the vgroup. the tag DFTAG_RIG is used */
  if (vgroup_id)
   status_32 = Vaddtagref (vgroup_id, TAG_GRP_IMAGE, ri_ref);

  /* add an annotation and label to the object */
  add_an(file_id, TAG_GRP_IMAGE, ri_ref);
 }

 if ( image_data )
 {
  free( image_data );
  image_data=NULL;
 }

}



/*-------------------------------------------------------------------------
 * Function: add_sd
 *
 * Purpose: utility function to write with
 *  SD - Multifile Scientific Data Interface,
 *  optionally :
 *   1)inserting the SD into the group VGROUP_ID
 *   2)making the dataset chunked and/or compressed
 *
 * Return: void
 *
 * Programmer: Pedro Vicente, pvn@ncsa.uiuc.edu
 *
 * Date: July 3, 2003
 *
 *-------------------------------------------------------------------------
 */


/* dimensions of dataset */
#define X_DIM      20
#define Y_DIM      80
#define Z_DIM      2


void add_sd(char *fname,             /* file name */
            int32 file_id,           /* file ID */
            char* sds_name,          /* sds name */
            int32 vgroup_id,         /* group ID */
            int32 chunk_flags,       /* chunk flags */
            int32 comp_type,         /* compression flag */
            comp_info *comp_info     /* compression structure */ )

{
 intn   status_n;     /* returned status_n for functions returning an intn  */
 int32  status_32,    /* returned status_n for functions returning an int32 */
        sd_id,        /* SD interface identifier */
        sds_id,       /* data set identifier */
        sds_ref,      /* reference number of the data set */
        dim_sds[2],   /* dimension of the data set */
        rank = 2,     /* rank of the data set array */
        n_values,     /* number of values of attribute */
        dim_index,    /* dimension index */
        dim_id,       /* dimension ID */
        start[2],     /* write start */
        edges[2],     /* write edges */
        fill_value=2, /* fill value */
        data[Y_DIM][X_DIM];
 float32 sds_values[2] = {2., 10.}; /* values of the SDS attribute  */
 int16   data_X[X_DIM];             /* X dimension dimension scale */
 float64 data_Y[Y_DIM];             /* Y dimension dimension scale */
 int     i, j;
 HDF_CHUNK_DEF chunk_def;           /* Chunking definitions */ 
 int32         pixels_per_scanline;

 /* set the size of the SDS's dimension */
 dim_sds[0] = Y_DIM;
 dim_sds[1] = X_DIM;

 /*define some compression specific parameters */
 switch(comp_type)
 {
 case COMP_CODE_RLE:
  break;

 case COMP_CODE_SKPHUFF:
  comp_info->skphuff.skp_size = 1;
  break;

 case COMP_CODE_DEFLATE:
  comp_info->deflate.level = 6;
  break;
  
 case COMP_CODE_SZIP:
  pixels_per_scanline = dim_sds[1];
  comp_info->szip.pixels = dim_sds[0]*dim_sds[1];
  comp_info->szip.pixels_per_block = 2;
  if(pixels_per_scanline >=2048)
   comp_info->szip.pixels_per_scanline = 512;
  else
   comp_info->szip.pixels_per_scanline = dim_sds[1];
  comp_info->szip.options_mask = NN_OPTION_MASK;
  comp_info->szip.options_mask |= RAW_OPTION_MASK;
  comp_info->szip.bits_per_pixel = 32;
  break;
 }
 
 /* Define chunk's dimensions */
 chunk_def.chunk_lengths[0] = Y_DIM/2;
 chunk_def.chunk_lengths[1] = X_DIM/2;
 /* To use chunking with RLE, Skipping Huffman, and GZIP compression */
 chunk_def.comp.chunk_lengths[0] = Y_DIM/2;
 chunk_def.comp.chunk_lengths[1] = X_DIM/2;
 
 /* GZIP compression, set compression type, flag and deflate level*/
 chunk_def.comp.comp_type = COMP_CODE_DEFLATE;
 chunk_def.comp.cinfo.deflate.level = 6;             

 /* data set data initialization */
 for (j = 0; j < Y_DIM; j++) {
  for (i = 0; i < X_DIM; i++)
   data[j][i] = (i + j) + 1;
 }
/* initialize dimension scales */
 for (i=0; i < X_DIM; i++) data_X[i] = i;
 for (i=0; i < Y_DIM; i++) data_Y[i] = 0.1 * i;

 /* initialize the SD interface */
 sd_id = SDstart (fname, DFACC_WRITE);
 
 /* create the SDS */
 sds_id = SDcreate (sd_id, sds_name, DFNT_INT32, rank, dim_sds);

 /* set chunk */
 if ( (chunk_flags == HDF_CHUNK) || (chunk_flags == (HDF_CHUNK | HDF_COMP)) )
  status_n = SDsetchunk (sds_id, chunk_def, chunk_flags);
 
 /* use compress without chunk-in */
 else if ( (chunk_flags==HDF_NONE || chunk_flags==HDF_CHUNK) && 
            comp_type>COMP_CODE_NONE && comp_type<COMP_CODE_INVALID)
  status_n = SDsetcompress (sds_id, comp_type, comp_info); 

 /* set a fill value */
 status_n = SDsetfillvalue (sds_id, (VOIDP)&fill_value);
  
 /* define the location and size of the data to be written to the data set */
 start[0] = 0;
 start[1] = 0;
 edges[0] = Y_DIM;
 edges[1] = X_DIM;
 
 /* write the stored data to the data set */
 status_n = SDwritedata (sds_id, start, NULL, edges, (VOIDP)data);

/* assign an attribute to the SDS */
 n_values = 2;
 status_n = SDsetattr (sds_id, "Valid_range", DFNT_FLOAT32, n_values, (VOIDP)sds_values);
   
/*  For each dimension of the data set specified in SDS_NAME,
 *  get its dimension identifier and set dimension name
 *  and dimension scale. Note that data type of dimension scale 
 *  can be different between dimensions and can be different from 
 *  SDS data type.
 */
 for (dim_index = 0; dim_index < rank; dim_index++) 
 {
 /* select the dimension at position dim_index */
  dim_id = SDgetdimid (sds_id, dim_index);
  
 /* assign name and dimension scale to selected dimension */
  switch (dim_index)
  {
  case 0: 
   status_n = SDsetdimname (dim_id, "Y_Axis");
   n_values = Y_DIM;
   status_n = SDsetdimscale (dim_id,n_values,DFNT_FLOAT64,(VOIDP)data_Y);  
   status_n = SDsetattr (dim_id, "info", DFNT_CHAR8, 7,"meters");
   break;
  case 1: 
   status_n = SDsetdimname (dim_id, "X_Axis");
   n_values = X_DIM; 
   status_n = SDsetdimscale (dim_id,n_values,DFNT_INT16,(VOIDP)data_X);
   status_n = SDsetattr (dim_id, "info", DFNT_CHAR8, 5,"feet");
   break;
  default: 
   break;
  }
 }

 /* obtain the reference number of the SDS using its identifier */
 sds_ref = SDidtoref (sds_id);
 
#if defined( HZIP_DEBUG)
 printf("add_sd %d\n",sds_ref); 
#endif
 
 /* add the SDS to the vgroup. the tag DFTAG_NDG is used */
 if (vgroup_id)
  status_32 = Vaddtagref (vgroup_id, TAG_GRP_DSET, sds_ref);
 
 /* add an annotation and label to the object */
 add_an(file_id, TAG_GRP_DSET, sds_ref);
 
 /* terminate access to the SDS */
 status_n = SDendaccess (sds_id);
 
 /* terminate access to the SD interface */
 status_n = SDend (sd_id);
}


/*-------------------------------------------------------------------------
 * Function: add_sd3d
 *
 * Purpose: utility function to write with
 *  SD - Multifile Scientific Data Interface,
 *  optionally :
 *   1)inserting the SD into the group VGROUP_ID
 *   2)making the dataset chunked and/or compressed
 *
 * Return: void
 *
 * Programmer: Pedro Vicente, pvn@ncsa.uiuc.edu
 *
 * Date: July 3, 2003
 *
 *-------------------------------------------------------------------------
 */

void add_sd3d(char *fname,             /* file name */
              int32 file_id,           /* file ID */
              char* sds_name,          /* sds name */
              int32 vgroup_id,         /* group ID */
              int32 chunk_flags,       /* chunk flags */
              int32 comp_type,         /* compression flag */
              comp_info *comp_info     /* compression structure */ )

{
 intn   status_n;     /* returned status_n for functions returning an intn  */
 int32  status_32,    /* returned status_n for functions returning an int32 */
        sd_id,        /* SD interface identifier */
        sds_id,       /* data set identifier */
        sds_ref,      /* reference number of the data set */
        dim_sds[3],   /* dimension of the data set */
        rank = 3,     /* rank of the data set array */
        start[3],     /* write start */
        fill_value=2, /* fill value */
        data[Z_DIM][Y_DIM][X_DIM];
 int    i, j, k;
 HDF_CHUNK_DEF chunk_def;           /* Chunking definitions */ 
 
 
 /* Define chunk's dimensions */
 chunk_def.chunk_lengths[0] = Z_DIM/2;
 chunk_def.chunk_lengths[1] = Y_DIM/2;
 chunk_def.chunk_lengths[2] = X_DIM/2;
 /* To use chunking with RLE, Skipping Huffman, and GZIP compression */
 chunk_def.comp.chunk_lengths[0] = Y_DIM/2;
 chunk_def.comp.chunk_lengths[1] = Y_DIM/2;
 chunk_def.comp.chunk_lengths[2] = X_DIM/2;
 
 /* GZIP compression, set compression type, flag and deflate level*/
 chunk_def.comp.comp_type = COMP_CODE_DEFLATE;
 chunk_def.comp.cinfo.deflate.level = 6;             

 /* data set data initialization */
 for (k = 0; k < Z_DIM; k++){
  for (j = 0; j < Y_DIM; j++) 
   for (i = 0; i < X_DIM; i++)
    data[k][j][i] = (i + j) + 1;
 }
 
 /* initialize the SD interface */
 sd_id = SDstart (fname, DFACC_WRITE);
 
 /* set the size of the SDS's dimension */
 dim_sds[0] = Z_DIM;
 dim_sds[1] = Y_DIM;
 dim_sds[2] = X_DIM;
 
 /* create the SDS */
 sds_id = SDcreate (sd_id, sds_name, DFNT_INT32, rank, dim_sds);

 /* set chunk */
 if ( (chunk_flags == HDF_CHUNK) || (chunk_flags == (HDF_CHUNK | HDF_COMP)) )
  status_n = SDsetchunk (sds_id, chunk_def, chunk_flags);
 
 /* use compress without chunk-in */
 else if ( (chunk_flags==HDF_NONE || chunk_flags==HDF_CHUNK) && 
            comp_type>COMP_CODE_NONE && comp_type<COMP_CODE_INVALID)
  status_n = SDsetcompress (sds_id, comp_type, comp_info); 

 /* set a fill value */
 status_n = SDsetfillvalue (sds_id, (VOIDP)&fill_value);
  
 /* define the location and size of the data to be written to the data set */
 start[0] = 0;
 start[1] = 0;
 start[2] = 0;
 
 /* write the stored data to the data set */
 status_n = SDwritedata (sds_id, start, NULL, dim_sds, (VOIDP)data);

 /* obtain the reference number of the SDS using its identifier */
 sds_ref = SDidtoref (sds_id);
 
#if defined( HZIP_DEBUG)
 printf("add_sd %d\n",sds_ref); 
#endif
 
 /* add the SDS to the vgroup. the tag DFTAG_NDG is used */
 if (vgroup_id)
  status_32 = Vaddtagref (vgroup_id, TAG_GRP_DSET, sds_ref);

 /* add an annotation and label to the object */
 add_an(file_id, TAG_GRP_DSET, sds_ref);
 
 /* terminate access to the SDS */
 status_n = SDendaccess (sds_id);
 
 /* terminate access to the SD interface */
 status_n = SDend (sd_id);
}


/*-------------------------------------------------------------------------
 * Function: add_vs
 *
 * Purpose: utility function to write with
 *  VS - Vdata Interface,
 *  optionally inserting the VS into the group VGROUP_ID
 *
 * Return: void
 *
 * Programmer: Pedro Vicente, pvn@ncsa.uiuc.edu
 *
 * Date: July 3, 2003
 *
 *-------------------------------------------------------------------------
 */


#define  N_RECORDS        3         /* number of records the vdata contains */
#define  ORDER_1          3         /* order of first field */
#define  ORDER_2          1         /* order of second field */
#define  ORDER_3          2         /* order of third field */
#define  CLASS_NAME       "Particle"
#define  FIELD1_NAME      "Position"      /* contains x, y, z values */
#define  FIELD2_NAME      "Mass"          /* contains weight values */
#define  FIELD3_NAME      "Temperature"   /* contains min and max values */
#define  FIELDNAME_LIST   "Position,Mass,Temperature" /* No spaces b/w names */
#define  N_VALS_PER_REC   (ORDER_1 + ORDER_2 + ORDER_3)  /* number of values per record */

void add_vs(char* vs_name,int32 file_id,int32 vgroup_id)
{
 intn    status_n;       /* returned status_n for functions returning an intn  */
 int32   status_32,      /* returned status_n for functions returning an int32 */
         vdata_ref,      /* reference number of the vdata */
         vdata_tag,      /* tag number of the vdata */
         vdata_id,       /* vdata id */
         num_of_records; /* number of records actually written to vdata */
 int32   attr_n_values   = 3; /* number of values in the vdata attribute */
 int32   field_n_values  = 4; /* number of values in the field attribute */
 char    vd_attr[3]      = {'A', 'B', 'C'};/* vdata attribute values*/
 int32   fld_attr[4]     = {2, 4, 6, 8};   /* field attribute values*/
 float32 data_buf[N_RECORDS][N_VALS_PER_REC]; /* buffer for vdata values */
 int     i;

 /* Initialize the VS interface */
 status_n = Vstart (file_id);
 
 /* Create a new vdata, set to -1 to create  */
 vdata_ref = -1,    
 vdata_id = VSattach (file_id, vdata_ref, "w");
 
 /* Set name and class name of the vdata */
 status_32 = VSsetname (vdata_id, vs_name);
 status_32 = VSsetclass (vdata_id, CLASS_NAME);
 
 /* Introduce each field's name, data type, and order */
 status_n = VSfdefine (vdata_id, FIELD1_NAME, DFNT_FLOAT32, ORDER_1 );
 status_n = VSfdefine (vdata_id, FIELD2_NAME, DFNT_FLOAT32, ORDER_2 );
 status_n = VSfdefine (vdata_id, FIELD3_NAME, DFNT_FLOAT32, ORDER_3 );
 
 /* Finalize the definition of the fields */
 status_n = VSsetfields (vdata_id, FIELDNAME_LIST);
 
/* 
 * Buffer the data by the record for fully interlaced mode.  Note that the
 * first three elements contain the three values of the first field, the
 * fourth element contains the value of the second field, and the last two
 * elements contain the two values of the third field.
 */
 for (i = 0; i < N_RECORDS; i++)
 {
  data_buf[i][0] = (float32)1.0 * i;
  data_buf[i][1] = (float32)2.0 * i;
  data_buf[i][2] = (float32)3.0 * i;
  data_buf[i][3] = (float32)0.1 + i;
  data_buf[i][4] = 0.0;
  data_buf[i][5] = 65.0;
 }
 
/* Write the data from data_buf to the vdata with full interlacing mode */
 num_of_records = VSwrite (vdata_id, (uint8 *)data_buf, N_RECORDS, 
  FULL_INTERLACE);
  
 /* Attach an attribute to the vdata, i.e., indicated by the second parameter */
 status_n = VSsetattr (vdata_id,_HDF_VDATA,"Myattr",DFNT_CHAR,
  attr_n_values, vd_attr);
 
 /* Attach an attribute to the field 0 */
 status_n = VSsetattr (vdata_id, 0, "Myfattr", DFNT_INT32, 
  field_n_values, fld_attr);
 
 /* Obtain the tag and ref number of the vdata */
 vdata_tag = VSQuerytag (vdata_id);
 vdata_ref = VSQueryref (vdata_id);
 
#if defined( HZIP_DEBUG)
 printf("add_vs %d\n",vdata_ref); 
#endif
 
 /* add the VS to the vgroup*/
 if (vgroup_id)
  status_32 = Vaddtagref (vgroup_id, vdata_tag, vdata_ref);
 
 /* terminate access to the VSs */
 status_32 = VSdetach (vdata_id);
 
 /* Terminate access to the VS interface */
 status_n = Vend (file_id);

 /* add an annotation and label to the vdata */
 add_an(file_id, vdata_tag, vdata_ref);
}


/*-------------------------------------------------------------------------
 * Function: add_file_an
 *
 * Purpose: utility function to write a file AN
 *
 * Return: void
 *
 * Programmer: Pedro Vicente, pvn@ncsa.uiuc.edu
 *
 * Date: July 30, 2003
 *
 *-------------------------------------------------------------------------
 */

#define  FILE_LABEL_TXT "This is a file label"
#define  FILE_DESC_TXT  "This is a file description"
#define  DATA_LABEL_TXT "This is a data label"
#define  DATA_DESC_TXT  "This is a data annotation"


void add_file_an(int32 file_id)
{
 intn  status_n;     /* returned status for functions returning an intn  */
 int32 status_32,    /* returned status for functions returning an int32 */
       an_id,        /* AN interface identifier */
       file_label_id,/* file label identifier */
       file_desc_id, /* file description identifier */
       data_label_id,  /* data label identifier */
       data_desc_id,   /* data description identifier */
       vgroup_id;
 uint16 vgroup_tag, vgroup_ref;
         
 /* Initialize the AN interface */
 an_id = ANstart (file_id);

/*-------------------------------------------------------------------------
 * file labels and annotations
 *-------------------------------------------------------------------------
 */ 

 /* Create the file label */
 file_label_id = ANcreatef (an_id, AN_FILE_LABEL);

 /* Write the annotations to the file label */
 status_32 = ANwriteann (file_label_id,FILE_LABEL_TXT,strlen (FILE_LABEL_TXT));

 /* Create file description */
 file_desc_id = ANcreatef (an_id, AN_FILE_DESC);

 /* Write the annotation to the file description */
 status_32 = ANwriteann (file_desc_id, FILE_DESC_TXT, strlen (FILE_DESC_TXT));


/*-------------------------------------------------------------------------
 * data labels and annotations
 *-------------------------------------------------------------------------
 */ 
 
 /* Create a vgroup in the V interface*/
 status_n = Vstart (file_id);
 vgroup_id = Vattach (file_id, -1, "w");
 status_32 = Vsetname (vgroup_id, "an_group");
 
 /* Obtain the tag and ref number of the vgroup */
 vgroup_tag = (uint16) VQuerytag (vgroup_id);
 vgroup_ref = (uint16) VQueryref (vgroup_id);
 
 /* Create the data label for the vgroup identified by its tag and ref number */
 data_label_id = ANcreate (an_id, vgroup_tag, vgroup_ref, AN_DATA_LABEL);
 
 /* Write the annotation text to the data label */
 status_32 = ANwriteann (data_label_id, DATA_LABEL_TXT, strlen (DATA_LABEL_TXT));
 
 /* Create the data description for the vgroup identified by its tag and ref number */
 data_desc_id = ANcreate (an_id, vgroup_tag, vgroup_ref, AN_DATA_DESC);
 
 /* Write the annotation text to the data description */
 status_32 = ANwriteann (data_desc_id, DATA_DESC_TXT, strlen (DATA_DESC_TXT));
 
 /* Teminate access to the vgroup and to the V interface */
 status_32 = Vdetach (vgroup_id);
 status_n = Vend (file_id);
 
 /* Terminate access to each annotation explicitly */
 status_n = ANendaccess (file_label_id);
 status_n = ANendaccess (file_desc_id);
 status_n = ANendaccess (data_label_id);
 status_n = ANendaccess (data_desc_id);
 
 /* Terminate access to the AN interface */
 status_32 = ANend (an_id);
}



/*-------------------------------------------------------------------------
 * Function: add_an
 *
 * Purpose: utility function to write a AN
 *
 * Return: void
 *
 * Programmer: Pedro Vicente, pvn@ncsa.uiuc.edu
 *
 * Date: August 19, 2003
 *
 *-------------------------------------------------------------------------
 */


void add_an(int32 file_id, int32 tag, int32 ref)
{
 intn  status_n;     /* returned status for functions returning an intn  */
 int32 status_32,    /* returned status for functions returning an int32 */
       an_id,        /* AN interface identifier */
       data_label_id,  /* data label identifier */
       data_desc_id;   /* data description identifier */
         
 /* Initialize the AN interface */
 an_id = ANstart (file_id);

/*-------------------------------------------------------------------------
 * data labels and annotations
 *-------------------------------------------------------------------------
 */ 
   
 /* Create the data label for the object identified by its tag and ref number */
 data_label_id = ANcreate (an_id, (uint16)tag, (uint16)ref, AN_DATA_LABEL);
 
 /* Write the annotation text to the data label */
 if (ANwriteann (data_label_id, DATA_LABEL_TXT, strlen (DATA_LABEL_TXT))==FAIL){
  printf("Error: writing data label in tag %d ref %d\n", tag, ref);
 }
 
 /* Create the data description for the object identified by its tag and ref number */
 data_desc_id = ANcreate (an_id, (uint16)tag, (uint16)ref, AN_DATA_DESC);
 
 /* Write the annotation text to the data description */
 if (ANwriteann (data_desc_id, DATA_DESC_TXT, strlen (DATA_DESC_TXT))==FAIL){
  printf("Error: writing data label in tag %d ref %d\n", tag, ref);
 }
 
 /* Terminate access to each annotation explicitly */
 status_n = ANendaccess (data_label_id);
 status_n = ANendaccess (data_desc_id);
 
 /* Terminate access to the AN interface */
 status_32 = ANend (an_id);
}




/*-------------------------------------------------------------------------
 * Function: add_pal
 *
 * Purpose: utility function to write a palette
 *
 * Return: void
 *
 * Programmer: Pedro Vicente, pvn@ncsa.uiuc.edu
 *
 * Date: August 19, 2003
 *
 *-------------------------------------------------------------------------
 */


void add_pal(char* fname)
{
 uint8  palette_data[256*3];
 
 if (DFPaddpal(fname,palette_data)==FAIL){
  printf( "Failed to write palette in <%s>\n", fname);
 }
 
}









/*-------------------------------------------------------------------------
 * read_data
 * utility function to read ASCII image data
 * the files have a header of the type
 *
 *   components
 *   n
 *   height
 *   n
 *   width
 *   n
 * 
 * followed by the image data
 *
 *-------------------------------------------------------------------------
 */

int read_data(char* file_name)
{
 int    i, n;
 int    color_planes;
 char   str[20];
 FILE   *f;
 int    w, h;

 f = fopen( file_name, "r");

 if ( f == NULL )
 {
  printf( "Could not open file <%s>\n", file_name );
  return -1;
 }

 fscanf( f, "%s", str );
 fscanf( f, "%d", &color_planes );
 fscanf( f, "%s", str );
 fscanf( f, "%d", &h); 
 fscanf( f, "%s", str );
 fscanf( f, "%d", &w); 

 /* globals */
 N_COMPS=color_planes;
 Y_LENGTH=h;
 X_LENGTH=w;

 if ( image_data )
 {
  free( image_data );
  image_data=NULL;
 }

 image_data = (unsigned char*)malloc(w*h*color_planes*sizeof(unsigned char));

 for (i = 0; i < h*w*color_planes ; i++)
 {
  fscanf( f, "%d",&n );
  image_data[i] = (unsigned char)n;
 }
 fclose(f);

 return 1;

}

