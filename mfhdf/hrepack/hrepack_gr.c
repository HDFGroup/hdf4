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

#include "hrepack_gr.h"
#include "hrepack_utils.h"
#include "hrepack_sdutil.h"
#include "hrepack_an.h"
#include "hrepack_parse.h"
#include "hrepack_opttable.h"


/*-------------------------------------------------------------------------
 * Function: copy_gr
 *
 * Purpose: copy a GR from input file to output file and compress it using options
 *
 * Return: void
 *
 * Programmer: Pedro Vicente, pvn@ncsa.uiuc.edu
 *
 * Date: July 11, 2003
 *
 *-------------------------------------------------------------------------
 */

int  copy_gr(int32 infile_id,
             int32 outfile_id,
             int32 gr_in,
             int32 gr_out,
             int32 tag,               /* tag of input GR */
             int32 ref,               /* ref of input GR */
             int32 vgroup_id_out_par, /* output parent group ID */
             char*path_name,          /* absolute path for input group name */
             options_t *options,
             table_t *table)
{
 intn          status_n;      /* returned status_n for functions returning an intn  */
 int32         status_32,     /* returned status_n for functions returning an int32 */
               ri_id,         /* raster image identifier */
               ri_out,        /* raster image identifier */
               ri_index,      /* index of a image */
               dimsizes[2],   /* dimensions of an image */
               n_comps,       /* number of components an image contains */
               interlace_mode,/* interlace mode of an image */ 
               dtype,         /* number type of an image */
               n_attrs,       /* number of attributes belong to an image */
               gr_ref;        /* reference number of the output data set */
               
 int32         pal_id,        /* palette identifier */
               pal_out,       /* palette identifier */
               pal_ref,
               r_num_entries, 
               r_data_type, 
               r_ncomp, 
               r_interlace_mode; 
 char          gr_name[MAX_GR_NAME]; 
 char          *path=NULL;
 comp_info_t   *comp=NULL;     /* compression info got from table */
 int           info;           /* temporary int compression info */
 comp_coder_t  comp_type;      /* compression type requested  */
 comp_coder_t  comp_type_in;   /* compression type original  */
 comp_info     c_info;         /* compression info requested  */
 comp_info     c_info_in;      /* compression info original  */
 HDF_CHUNK_DEF chunk_def;      /* chunk definition */
 HDF_CHUNK_DEF chunk_def_in;   /* chunk definition original */
 int32         chunk_flags;    /* chunk flags */ 
 int32         chunk_flags_in; /* chunk flags original*/ 
 int           i, j, ret, rank=2, have_info;
 int           has_pal = 0;
 int32         start[2],       /* read start */
               edges[2],       /* read edges */
               numtype,        /* number type */
               eltsz,          /* element size */
               nelms,          /* number of elements */
               data_size;
 VOIDP         buf=NULL;
 uint8         pal_data[256*3];


 ri_index = GRreftoindex(gr_in,(uint16)ref);
 ri_id    = GRselect(gr_in,ri_index);
   
 status_n = GRgetiminfo(ri_id,gr_name,&n_comps,&dtype,&interlace_mode,dimsizes,&n_attrs);
 
 /* initialize path */
 path=get_path(path_name,gr_name);

 /* add object to table */
 table_add(table,tag,ref,path);
 
#if defined(HZIP_DEBUG)
 printf ("\t%s %d\n", path, ref); 
#endif

/*-------------------------------------------------------------------------
 * get the original compression/chunk info from the object 
 *-------------------------------------------------------------------------
 */
 
 comp_type_in = COMP_CODE_NONE;  /* reset variables before retrieving info */
 HDmemset(&c_info_in, 0, sizeof(comp_info)) ;
 status_n = GRgetcompress(ri_id, &comp_type_in, &c_info_in);

 /* get chunk lengths */
 status_n = GRgetchunkinfo(ri_id, &chunk_def_in, &chunk_flags_in);
 /* retrieve the compress info if so */
 if ( (HDF_CHUNK | HDF_COMP) == chunk_flags_in )
 {
  chunk_def_in.comp.comp_type=comp_type_in;
  switch (comp_type_in)
  {
   case COMP_CODE_NONE:
   break;
  case COMP_CODE_SZIP:
   chunk_def_in.comp.comp_type              = COMP_CODE_SZIP;
   chunk_def_in.comp.cinfo.szip             = c_info_in.szip;
   break;
  case COMP_CODE_RLE:
   chunk_def_in.comp.comp_type              = COMP_CODE_RLE;
   break;
  case COMP_CODE_SKPHUFF:
   chunk_def_in.comp.comp_type              = COMP_CODE_SKPHUFF;
   chunk_def_in.comp.cinfo.skphuff          = c_info_in.skphuff;
   break;
  case COMP_CODE_DEFLATE:
   chunk_def_in.comp.comp_type              = COMP_CODE_DEFLATE;
   chunk_def_in.comp.cinfo.deflate          = c_info_in.deflate;
   break;
  case COMP_CODE_JPEG:
   chunk_def_in.comp.comp_type              = COMP_CODE_JPEG;
   chunk_def_in.comp.cinfo.jpeg             = c_info_in.jpeg;
   break;
  default:
   printf("Error: Unrecognized compression code <%d> in <%s>\n",comp_type_in,path);
   break;
  };
 }

/*-------------------------------------------------------------------------
 * set the default values to the ones read from the object
 *-------------------------------------------------------------------------
 */

 comp_type   = comp_type_in;
 switch (comp_type_in)
  {
  case COMP_CODE_SZIP:
   break;
  case COMP_CODE_RLE:
   break;
  case COMP_CODE_SKPHUFF:
   info  = c_info_in.skphuff.skp_size;
   break;
  case COMP_CODE_DEFLATE:
   info  = c_info_in.deflate.level;
   break;
  case COMP_CODE_JPEG:
   info  = c_info_in.jpeg.quality;
   break;
  };
 chunk_flags = chunk_flags_in;
 if ( (HDF_CHUNK) == chunk_flags )
 {
  for (i = 0; i < rank; i++) 
   chunk_def.chunk_lengths[i]      = chunk_def_in.chunk_lengths[i];
 }
 else if ( (HDF_CHUNK | HDF_COMP) == chunk_flags )
 {
  for (i = 0; i < rank; i++) {
   chunk_def.chunk_lengths[i]      = chunk_def_in.chunk_lengths[i];
   chunk_def.comp.chunk_lengths[i] = chunk_def_in.chunk_lengths[i];
  }
  chunk_def.comp.comp_type=comp_type_in;
  switch (comp_type_in)
  {
   case COMP_CODE_NONE:
   break;
  case COMP_CODE_SZIP:
   chunk_def.comp.comp_type              = COMP_CODE_SZIP;
   chunk_def.comp.cinfo.szip             = c_info_in.szip;
   break;
  case COMP_CODE_RLE:
   chunk_def.comp.comp_type              = COMP_CODE_RLE;
   break;
  case COMP_CODE_SKPHUFF:
   chunk_def.comp.comp_type              = COMP_CODE_SKPHUFF;
   chunk_def.comp.cinfo.skphuff          = c_info_in.skphuff;
   break;
  case COMP_CODE_DEFLATE:
   chunk_def.comp.comp_type              = COMP_CODE_DEFLATE;
   chunk_def.comp.cinfo.deflate          = c_info_in.deflate;
   break;
  case COMP_CODE_JPEG:
   chunk_def.comp.comp_type              = COMP_CODE_JPEG;
   chunk_def.comp.cinfo.jpeg             = c_info_in.jpeg;
   break;
  default:
   printf("Error: Unrecognized compression code <%d> in <%s>\n",comp_type_in,path);
   break;
  };
 }


/*-------------------------------------------------------------------------
 * get the compression/chunk info of this object from the table
 * translate to usable info
 * this is done ONLY for the second trip inspection 
 *-------------------------------------------------------------------------
 */
 
 /* check inspection mode */
 if ( options->trip>0 ) 
 {
  have_info = 
  options_get_info(options,      /* global options */
                   &chunk_flags, /* chunk flags OUT */
                   &chunk_def,   /* chunk definition OUT */
                   &info,        /* compression info OUT */
                   &comp_type,   /* compression type OUT  */
                   rank,         /* rank of object IN */
                   path,         /* path of object IN */
                   dimsizes,     /* dimensions (for SZIP), IN */
                   dtype         /* numeric type ( for SZIP), IN */
                   );
 } /* check inspection mode */


/*-------------------------------------------------------------------------
 * check for data size before printing
 *-------------------------------------------------------------------------
 */

 /* compute the number of the bytes for each value. */
 numtype = dtype & DFNT_MASK;
 eltsz = DFKNTsize(numtype | DFNT_NATIVE);

 /* set edges of GR */
 nelms=1;
 for (j = 0; j < rank; j++) {
  nelms   *= dimsizes[j];
  edges[j] = dimsizes[j];
  start[j] = 0;
 }

 data_size = dimsizes[0]*dimsizes[1]*n_comps*eltsz;

/*-------------------------------------------------------------------------
 * check for objects too small
 *-------------------------------------------------------------------------
 */
 if ( have_info && options->trip>0  && nelms*eltsz<options->threshold )
 {
  /* reset to the original values . we don't want to uncompress if it was */
  chunk_flags=chunk_flags_in;
  comp_type=comp_type_in;
  if (options->verbose) {
   printf("Warning: object size smaller than %d bytes. Not compressing <%s>\n",
    options->threshold,path);
  }
 }
 
/*-------------------------------------------------------------------------
 * print the PATH, COMP and CHUNK info
 *-------------------------------------------------------------------------
 */ 
 
 if (options->verbose)
 {
  int pr_comp_type=0;
  if (comp_type>0)
  {
   pr_comp_type=comp_type;
  }
  else
  {
   if (chunk_flags== (HDF_CHUNK | HDF_COMP))
   {
    pr_comp_type=chunk_def.comp.comp_type;
   }
  }
  printf(PFORMAT,
   (chunk_flags>0)?"chunk":"",                    /*chunk info*/
   (pr_comp_type>0)?get_scomp(pr_comp_type):"",   /*compression info*/
   path);                                         /*name*/
 }

/*-------------------------------------------------------------------------
 * if we are in first trip inspection mode, exit, after printing the info
 *-------------------------------------------------------------------------
 */ 
 
 /* check inspection mode */
 if ( options->trip==0 ) {
  status_n=GRendaccess(ri_id);
  if (path) free(path);
  return 0;
 }
/*-------------------------------------------------------------------------
 * read gr and create new one
 *-------------------------------------------------------------------------
 */

 /* alloc */
 if ((buf = (VOIDP) HDmalloc(data_size)) == NULL) {
  printf( "Failed to allocate %d elements of size %d\n", nelms, eltsz);
  GRendaccess(ri_id);
  if (path) free(path);
  return-1;
 }


 /* set the interlace for reading  */
 if ( GRreqimageil(ri_id, interlace_mode) == FAIL ){
  printf( "Could not set interlace for GR <%s>\n", path);
  GRendaccess(ri_id);
  if (path) free(path);
  return-1;
 }
 
 /* read data */
 if (GRreadimage (ri_id, start, NULL, edges, buf) == FAIL) {
  printf( "Could not read GR <%s>\n", path);
  GRendaccess(ri_id);
  if (path) free(path);
  return-1;
 }
 
 /* create output GR */
 if ((ri_out = GRcreate(gr_out,gr_name,n_comps,dtype,interlace_mode,dimsizes)) == FAIL) {
  printf( "Failed to create new GR <%s>\n", path);
  ret=-1;
  goto out;
 }

/*-------------------------------------------------------------------------
 * set chunk 
 *
 * Chunked                  -> flags = HDF_CHUNK
 * Chunked and compressed   -> flags = HDF_CHUNK | HDF_COMP 
 * Non-chunked              -> flags = HDF_NONE
 *-------------------------------------------------------------------------
 */

 /* set chunk */
 if ( (chunk_flags == HDF_CHUNK) || (chunk_flags == (HDF_CHUNK | HDF_COMP)) )
 {
  if (GRsetchunk (ri_out, chunk_def, chunk_flags)==FAIL)
  {
   printf( "Error: Failed to set chunk dimensions for <%s>\n", path);
   ret=-1;
   goto out;
  }
 }

/*-------------------------------------------------------------------------
 * set compression
 *
 * COMP_CODE_RLE       -> simple RLE encoding
 * COMP_CODE_SKPHUFF   -> Skipping huffman encoding
 * COMP_CODE_DEFLATE   -> gzip 'deflate' encoding
 *-------------------------------------------------------------------------
 */
   
 /* use compress without chunk-in */
 else if ( chunk_flags==HDF_NONE && comp_type>COMP_CODE_NONE)  
 {
  /* setup compression factors */
  switch(comp_type) 
  {
   case COMP_CODE_SZIP:
   if (set_szip (rank,dimsizes,dtype,&c_info)==FAIL)
   {
    printf( "Error: Failed to set SZIP compression for <%s>\n", path);
    ret=-1;
    goto out;
   }
   break;
  case COMP_CODE_RLE:         
   break;
  case COMP_CODE_SKPHUFF:     
   c_info.skphuff.skp_size    = info;
   break;
  case COMP_CODE_DEFLATE:
   c_info.deflate.level       = info;
   break;
  case COMP_CODE_JPEG:
   c_info.jpeg.quality        = info;
   c_info.jpeg.force_baseline = 1;
   break;
  default:
   printf( "Error: Unrecognized compression code %d\n", comp_type);
  }

  if (GRsetcompress (ri_out, comp_type, &c_info)==FAIL)
  {
   printf( "Error: Failed to set compression for <%s>\n", path);
   ret=-1;
   goto out;
  }
 }
 
 /* write the data */
 if (GRwriteimage(ri_out, start, NULL, edges, buf) == FAIL) {
  printf( "Failed to write to new GR <%s>\n", path);
  ret=-1;
  goto out;
 }

/*-------------------------------------------------------------------------
 * copy attributes
 *-------------------------------------------------------------------------
 */ 
 
 if( copy_gr_attrs(ri_id,ri_out,n_attrs,options)==FAIL) {
  ret=-1;
  goto out;
 }

/*-------------------------------------------------------------------------
 * check for palette
 *-------------------------------------------------------------------------
 */ 

 pal_id = GRgetlutid(ri_id, 0);
 GRgetlutinfo(pal_id,&r_ncomp,&r_data_type,&r_interlace_mode,&r_num_entries);

 /*check if there is palette data */
 has_pal=((r_ncomp == 0) || (r_interlace_mode < 0) || (r_num_entries == 0))?0:1;

 if ( has_pal==1 )
 {
  GRreqlutil(ri_id, r_interlace_mode);    
  if ((status_n = GRreadlut(pal_id, pal_data)) == FAIL) {
   printf( "Failed to get palette data for <%s>\n", path);
  }

  if ((pal_ref=GRluttoref(pal_id))== FAIL) {
   printf( "Failed to get palette ref for <%s>\n", path);
  }
  /* add palette to table; we want to later check for lone palettes */
  table_add(table,DFTAG_IP8,pal_ref,"palette");
  
  if (status_n==SUCCEED)
  {
   /* Get the id for the new palette */
   if ((pal_out = GRgetlutid(ri_out, 0)) == FAIL) {
    printf( "Failed to get palette ID for <%s>\n", path);
   }
   
   /* Write the palette to file. */
   if ((status_n = GRwritelut(pal_out,r_ncomp,r_data_type,r_interlace_mode,r_num_entries, 
    (VOIDP)pal_data)) == FAIL) {
    printf( "Failed to write palette for <%s>\n", path);
   }
  } /* SUCCEED */
 } /* has_pal==1 */

 
 /* obtain the reference number of the new SDS using its identifier */
 if ((gr_ref = GRidtoref (ri_out)) == FAIL) {
  printf( "Failed to get new GR reference in <%s>\n", path);
 }

/*-------------------------------------------------------------------------
 * add GR to group, if needed
 *-------------------------------------------------------------------------
 */ 
 if (vgroup_id_out_par) 
 {
  /* add the GR to the vgroup. the tag DFTAG_RIG is used */
  if ((status_32 = Vaddtagref (vgroup_id_out_par, TAG_GRP_IMAGE, gr_ref)) == FAIL) {
   printf( "Failed to add new GR to group <%s>\n", path);
  }
 }

/*-------------------------------------------------------------------------
 * copy ANs
 *-------------------------------------------------------------------------
 */ 
 
 copy_an(infile_id,outfile_id,
         ref,DFTAG_RIG,gr_ref,DFTAG_RIG, 
         path,options);
 copy_an(infile_id,outfile_id,
         ref,DFTAG_RI,gr_ref,DFTAG_RI,
         path,options);

#if 1
 {
  VOIDP buf1=NULL;
  int32 interlace_mode2;
  int   cmp;

  status_n = GRgetiminfo(ri_out,gr_name,&n_comps,&dtype,&interlace_mode2,dimsizes,&n_attrs);

  
  /* alloc */
  if ((buf1 = (VOIDP) HDmalloc(data_size)) == NULL) {
   printf( "Failed to allocate %d elements of size %d\n", nelms, eltsz);
   GRendaccess(ri_id);
   if (path) free(path);
   return-1;
  }
  
  
  /* set the interlace for reading  */
  if ( GRreqimageil(ri_out, interlace_mode2) == FAIL ){
   printf( "Could not set interlace for GR <%s>\n", path);
   GRendaccess(ri_id);
   if (path) free(path);
   return-1;
  }
  
  /* read data */
  if (GRreadimage (ri_out, start, NULL, edges, buf1) == FAIL) {
   printf( "Could not read GR <%s>\n", path);
   GRendaccess(ri_id);
   if (path) free(path);
   return-1;
  }
  
   cmp = HDmemcmp(buf1,buf,data_size);
   free(buf1);
 }
 
#endif
 

out:
 
 /* terminate access to the GRs */
 status_n = GRendaccess(ri_id);
 status_n = GRendaccess(ri_out);
    
 if (path)
  free(path);
 if (buf)
  free(buf);

 return ret;
 
}


/*-------------------------------------------------------------------------
 * Function: copy_gr_attrs
 *
 * Purpose: copy GR attributes from input file to output file 
 *
 * Return: 1, for success, -1 for error 
 *
 * Programmer: Pedro Vicente, pvn@ncsa.uiuc.edu
 *
 * Date: July 28, 2003
 *
 *-------------------------------------------------------------------------
 */

int copy_gr_attrs(int32 ri_id,
                  int32 ri_out,
                  int32 nattrs,          
                  options_t *options)
{
 int32 dtype,                 /* SDS data type */
       numtype,               /* number type */
       eltsz,                 /* element size */
       nelms;                 /* number of elements */
 char  attr_name[MAX_NC_NAME];
 VOIDP attr_buf=NULL;
 int   i;

 /* loop through attributes in input GR */
 for (i = 0; i < nattrs; i++) 
 {
  if (GRattrinfo (ri_id, i, attr_name, &dtype, &nelms) == FAIL) {
   printf( "Cannot get info for attribute number %d\n", i);
   return-1;
  }
  /* compute the number of the bytes for each value. */
  numtype = dtype & DFNT_MASK;
  eltsz   = DFKNTsize(numtype | DFNT_NATIVE);
  if ((attr_buf = (VOIDP) HDmalloc(nelms * eltsz)) == NULL) {
   printf( "Error allocating %d values of size %d for attribute %s",
    nelms, numtype, attr_name);
   return-1;
  }
  /* read attributes from input GR */
  if (GRgetattr(ri_id, i, attr_buf) == FAIL) {
   printf( "Cannot read attribute %s\n", attr_name);
   return-1;
  }
  /* put attributes into output GR */
  if (GRsetattr(ri_out, attr_name, dtype, nelms, attr_buf) == FAIL) {
   printf( "Cannot write attribute %s\n", attr_name);
   return-1;
  }

  if (attr_buf)
   free(attr_buf);
 }

 return 1;
}



