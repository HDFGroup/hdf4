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
#include "copy.h"
#include "parse.h"
#include "utils.h"



int  copy_vdata_attribute(int32 in, int32 out, int32 findex, intn attrindex);
int  options_get_info(options_t      *options,     /* global options */
                      int32          *chunk_flags, /* chunk flags OUT */
                      HDF_CHUNK_DEF  *chunk_def,   /* chunk definition OUT */
                      int            *info,        /* compression info OUT */
                      comp_coder_t   *comp_type,   /* compression type OUT  */
                      int            rank,         /* rank of object IN */
                      char           *path,        /* path of object IN */
                      int32          *dimsizes,    /* dimensions (for SZIP), IN */
                      int32          dtype         /* numeric type (for SZIP), IN */
                      );

int  set_szip(int32 rank, int32 *dimsizes, int32 dtype, comp_info *c_info);

/*-------------------------------------------------------------------------
 * Function: copy_sds
 *
 * Purpose: copy an SDS from input file to output file and compress it using options
 *
 * Return: 0, -1 for error 
 *
 * Programmer: Pedro Vicente, pvn@ncsa.uiuc.edu
 *
 * Date: July 10, 2003
 *
 *-------------------------------------------------------------------------
 */

int copy_sds(int32 sd_in,
             int32 sd_out,
             int32 tag,               /* tag of input SDS */
             int32 ref,               /* ref of input SDS */
             int32 vgroup_id_out_par, /* output parent group ID */
             char*path_name,          /* absolute path for input group name */
             options_t *options,
             table_t *table,
             int32 infile_id,
             int32 outfile_id)
{
 intn  status_n;              /* returned status_n for functions returning an intn  */
 int32 status_32,             /* returned status_n for functions returning an int32 */
       sds_id,                /* data set identifier */
       sds_out,               /* data set identifier */
       sds_index,             /* index number of the data set */
       dtype,                 /* SDS data type */
       dimsizes[MAX_VAR_DIMS],/* dimensional size of SDS */
       start[MAX_VAR_DIMS],   /* read start */
       edges[MAX_VAR_DIMS],   /* read edges */
       nattrs,                /* number of SDS attributes */
       rank,                  /* rank of SDS */
       sds_ref,               /* reference number of the output data set */
       numtype,               /* number type */
       eltsz,                 /* element size */
       nelms,                 /* number of elements */
       dim_size,              /* dimension size */
       dim_id,                /* dimension ID */
       dim_out;               /* dimension ID */
 char             sds_name[MAX_NC_NAME]; 
 char             dim_name[MAX_NC_NAME];
 char             *path=NULL;
 VOIDP            buf=NULL;
 VOIDP            dim_buf=NULL;
 int              i, j, ret=1;
 comp_info_t      *comp=NULL;     /* compression info got from table */
 int              info;           /* temporary int compression info */
 comp_coder_t     comp_type;      /* compression type requested  */
 comp_coder_t     comp_type_in;   /* compression type original  */
 comp_info        c_info;         /* compression info requested  */
 comp_info        c_info_in;      /* compression info original  */
 HDF_CHUNK_DEF    chunk_def;      /* chunk definition */
 HDF_CHUNK_DEF    chunk_def_in;   /* chunk definition original */
 int32            chunk_flags;    /* chunk flags */ 
 int32            chunk_flags_in; /* chunk flags original*/ 
 intn             empty_sds;
 int              have_info;

 sds_index = SDreftoindex(sd_in,ref);
 sds_id    = SDselect(sd_in,sds_index);
 
 /*obtain name,rank,dimsizes,datatype and num of attributes of sds */
 status_n=SDgetinfo(sds_id,sds_name,&rank,dimsizes,&dtype,&nattrs);

 /* check if the given SDS is a dimension scale, return 0 for no table add */
 if ( SDiscoordvar(sds_id) ) {
  SDendaccess(sds_id);
  return 0;
 }
 
 /* initialize path */
 path=get_path(path_name,sds_name);
 
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
 status_n = SDgetcompress(sds_id, &comp_type_in, &c_info_in);

 /* get chunk lengths */
 status_n = SDgetchunkinfo(sds_id, &chunk_def_in, &chunk_flags_in);
 /* retrieve the compress info if so */
 if ( (HDF_CHUNK | HDF_COMP) == chunk_flags_in )
 {
  chunk_def_in.comp.comp_type=comp_type_in;
  switch (comp_type_in)
  {
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
  case COMP_CODE_SZIP:
   chunk_def_in.comp.comp_type              = COMP_CODE_SZIP;
   chunk_def_in.comp.cinfo.szip             = c_info_in.szip;
   break;
  default:
   printf("Error: Unrecognized compression code in %d <%s>\n",comp_type_in,path);
  };
 }

/*-------------------------------------------------------------------------
 * set the default values to the ones read from the object
 *-------------------------------------------------------------------------
 */

 comp_type   = comp_type_in;
 switch (comp_type_in)
  {
  case COMP_CODE_NONE:
   break;
  case COMP_CODE_RLE:
   break;
  case COMP_CODE_SZIP:
   break;
  case COMP_CODE_SKPHUFF:
   info  = c_info_in.skphuff.skp_size;
   break;
  case COMP_CODE_DEFLATE:
   info  = c_info_in.deflate.level;
   break;
  default:
   printf("Error: Unrecognized compression code in %d <%s>\n",comp_type,path);
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
  case COMP_CODE_RLE:
   chunk_def.comp.comp_type              = COMP_CODE_RLE;
   break;
  case COMP_CODE_SKPHUFF:
   chunk_def.comp.comp_type              = COMP_CODE_SKPHUFF;
   chunk_def.comp.cinfo.skphuff          = c_info_in.skphuff;
   break;
  case COMP_CODE_DEFLATE:
   chunk_def.comp.comp_type              = COMP_CODE_DEFLATE;
   chunk_def.comp.cinfo.deflate         = c_info_in.deflate;
   break;
  case COMP_CODE_SZIP:
   chunk_def.comp.comp_type              = COMP_CODE_SZIP;
   chunk_def.comp.cinfo.szip             = c_info_in.szip;
   break;
  default:
   printf("Error: Unrecognized compression code in %d <%s>\n",comp_type_in,path);
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
 * get size before printing
 *-------------------------------------------------------------------------
 */

 /* compute the number of the bytes for each value. */
 numtype = dtype & DFNT_MASK;
 eltsz = DFKNTsize(numtype | DFNT_NATIVE);

 /* set edges of SDS */
 nelms=1;
 for (j = 0; j < rank; j++) {
  nelms   *= dimsizes[j];
  edges[j] = dimsizes[j];
  start[j] = 0;
 }

/*-------------------------------------------------------------------------
 * check for maximum number of chunks treshold
 *-------------------------------------------------------------------------
 */
 if ( options->trip>0 ) 
 {
  int count=1, nchunks;
  int maxchunk=USHRT_MAX;
  if ( (chunk_flags == HDF_CHUNK) || (chunk_flags == (HDF_CHUNK | HDF_COMP)) )
  {
   for (j = 0; j < rank; j++) {
    count   *= chunk_def.chunk_lengths[j];
   }
   nchunks=nelms/count;
   if (nchunks>maxchunk){
    printf("Warning: number of chunks is %d (greater than %d). Not chunking <%s>\n",
    nchunks,maxchunk,path);
    chunk_flags=HDF_NONE;
   }
  }
 }


/*-------------------------------------------------------------------------
 * check for objects too small
 *-------------------------------------------------------------------------
 */
 if ( have_info==1 && options->trip>0  && nelms*eltsz<options->threshold )
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
  SDendaccess(sds_id);
  if (path) free(path);
  return 0;
 }

/*-------------------------------------------------------------------------
 * create new SDS
 *-------------------------------------------------------------------------
 */

 if ((sds_out = SDcreate(sd_out,sds_name,dtype,rank,dimsizes)) == FAIL) {
  printf( "Failed to create new SDS <%s>\n", path);
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
  if (SDsetchunk (sds_out, chunk_def, chunk_flags)==FAIL)
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
   c_info.skphuff.skp_size = info;
   break;
  case COMP_CODE_DEFLATE:
   c_info.deflate.level = info;
   break;
  default:
   printf( "Error: Unrecognized compression code %d\n", comp_type);
  }

  if (SDsetcompress (sds_out, comp_type, &c_info)==FAIL)
  {
   printf( "Error: Failed to set compression for <%s>\n", path);
   ret=-1;
   goto out;
  }
 }


/*-------------------------------------------------------------------------
 * check if the input SDS is empty. if so , do not read its data and write to new one
 *-------------------------------------------------------------------------
 */ 
 if (SDcheckempty( sds_id, &empty_sds ) == FAIL) {
  printf( "Failed to check empty SDS <%s>\n", path);
  ret=-1;
  goto out;
 }
/*-------------------------------------------------------------------------
 * read sds and write new one
 *-------------------------------------------------------------------------
 */
 if (empty_sds==0 )
 {
  /* alloc */
  if ((buf = (VOIDP) HDmalloc(nelms * eltsz)) == NULL) {
   printf( "Failed to allocate %d elements of size %d\n", nelms, eltsz);
   ret=-1;
   goto out;
  }
  
  /* read data */
  if (SDreaddata (sds_id, start, NULL, edges, buf) == FAIL) {
   printf( "Could not read SDS <%s>\n", path);
   ret=-1;
   goto out;
  }
  
  /* write the data */
  if (SDwritedata(sds_out, start, NULL, edges, buf) == FAIL) {
   printf( "Failed to write to new SDS <%s>\n", path);
   ret=-1;
   goto out;
  }
 } /* empty_sds */

/*-------------------------------------------------------------------------
 * copy attributes
 *-------------------------------------------------------------------------
 */ 
 
 if( copy_sds_attrs(sds_id,sds_out,nattrs,options)==FAIL) {
  ret=-1;
  goto out;
 }
 
/*-------------------------------------------------------------------------
 * copy dimension scales
 *-------------------------------------------------------------------------
 */ 
 
 /* loop through each dimension up to rank of SDS */
 for (i = 0; i < rank; i++) 
 {
  /* get dimension handle for input dimension */
  if ((dim_id = SDgetdimid(sds_id, i)) == FAIL) {
   printf( "Failed to get dimension %d of SDS <%s>\n", i, path);
   ret=-1;
   goto out;
  }
  /* get dimension handle for output dimension */
  if ((dim_out = SDgetdimid(sds_out, i)) == FAIL) {
   printf( "Failed to get dim_id for dimension %d of SDS <%s>\n", i, path);
   ret=-1;
   goto out;
  }
  /* get dimension information for input dimension */
  if (SDdiminfo(dim_id, dim_name, &dim_size, &dtype, &nattrs) == FAIL) {
   printf( "Failed to get info for dimension %d of SDS <%s>\n", i, path);
   ret=-1;
   goto out;
  }
  /* set output dimension name */
  if (SDsetdimname(dim_out, dim_name) == FAIL) {
   printf( "Failed to set dimension name %d of SDS <%s>\n", i, path);
   ret=-1;
   goto out;
  }
  /* copy attributes */
  if (nattrs && copy_sds_attrs(dim_id, dim_out, nattrs, options) == FAIL) {
   printf( "Failed to copy attributes for dimension %d of of SDS <%s>\n", i, path);
   ret=-1;
   goto out;
  }
  /* copy scale information over */
  if (dtype != 0) 
  {
   /* compute the number of the bytes for each value. */
   numtype = dtype & DFNT_MASK;
   eltsz = DFKNTsize(numtype | DFNT_NATIVE);

   if ((dim_buf = (VOIDP) HDmalloc(dim_size * eltsz)) == NULL) {
    printf( "Failed to alloc %d for dimension scale\n", dim_size);
    ret=-1;
    goto out;
   }
   if (SDgetdimscale(dim_id, dim_buf) == FAIL) {
    printf( "Failed to get scale info for %s\n", dim_name);
    ret=-1;
    goto out;
   }
   if (SDsetdimscale(dim_out, dim_size, dtype, dim_buf) == FAIL) {
    printf( "Failed to set scale info for %s\n", dim_name);
    ret=-1;
    goto out;
   }
  }
 }

 /* obtain the reference number of the new SDS using its identifier */
 if ((sds_ref = SDidtoref (sds_out)) == FAIL) {
  printf( "Failed to get new SDS reference in <%s>\n", path);
  ret=-1;
  goto out;
 }

/*-------------------------------------------------------------------------
 * add SDS to group
 *-------------------------------------------------------------------------
 */ 
 
 /* add it to group, if present */
 if (vgroup_id_out_par) 
 {
  /* add the SDS to the vgroup. the tag DFTAG_NDG is used */
  if ((status_32 = Vaddtagref (vgroup_id_out_par, TAG_GRP_DSET, sds_ref)) == FAIL) {
   printf( "Failed to add new SDS to group <%s>\n", path);
   ret=-1;
   goto out;
  }
 }

/*-------------------------------------------------------------------------
 * copy ANs
 *-------------------------------------------------------------------------
 */ 
 
 copy_an(infile_id,outfile_id,
         ref,tag,sds_ref,tag, 
         path,options);

out:
 /* terminate access to the SDSs */
 status_n = SDendaccess(sds_id);
 status_n = SDendaccess (sds_out);
   
 if (path)
  free(path);
 if (buf)
  free(buf);

 return ret;
 
}


/*-------------------------------------------------------------------------
 * Function: copy_sds_attrs
 *
 * Purpose: copy SD attributes from input file to output file 
 *   used for global, dataset and dimension attributes
 *
 * Return: 1, for success, -1 for error 
 *
 * Programmer: Pedro Vicente, pvn@ncsa.uiuc.edu
 *
 * Date: July 30, 2003
 *
 *-------------------------------------------------------------------------
 */

int copy_sds_attrs(int32 id_in,
                   int32 id_out,
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

 /* loop through attributes in input SDS */
 for (i = 0; i < nattrs; i++) 
 {
  if (SDattrinfo (id_in, i, attr_name, &dtype, &nelms) == FAIL) {
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
  /* read attributes from input SDS */
  if (SDreadattr(id_in, i, attr_buf) == FAIL) {
   printf( "Cannot read attribute %s\n", attr_name);
   return-1;
  }
  /* put attributes into output SDS */
  if (SDsetattr(id_out, attr_name, dtype, nelms, attr_buf) == FAIL) {
   printf( "Cannot write attribute %s\n", attr_name);
   return-1;
  }

  if (attr_buf)
   free(attr_buf);
 }

 return 1;
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

/*-------------------------------------------------------------------------
 * Function: copy_vdata_attribute
 *
 * Purpose: copy VS attributes from input file to output file 
 *
 * Return: 1, for success, -1 for error 
 *
 * Programmer: Pedro Vicente, pvn@ncsa.uiuc.edu
 *
 * Date: July 28, 2003
 *
 *-------------------------------------------------------------------------
 */

int copy_vdata_attribute(int32 in, int32 out, int32 findex, intn attrindex)
{
 char   attr_name[MAX_NC_NAME];
 int32  n_values, attr_size, attr_type;
 VOIDP  *values=NULL;

 /* Get attribute information */
 VSattrinfo(in, findex, attrindex, attr_name, &attr_type, &n_values, &attr_size);

 /* Allocate space for attribute values */
 if ((values = (VOIDP)malloc(attr_size * n_values)) == NULL) {
  printf( "Cannot allocate %d values of size %d for attribute %s",
   n_values, attr_size, attr_name);
  return-1;
 }

 /* Read attribute from input object */
 if (VSgetattr(in, findex, attrindex, values) == FAIL) {
  printf( "Cannot read attribute %s\n", attr_name);
  if (values) free(values);
  return-1;
 }

 /* Write attribute to output object */
 if (VSsetattr(out, findex, attr_name, attr_type, n_values, values) == FAIL) {
  printf( "Cannot write attribute %s\n", attr_name);
  if (values) free(values);
  return-1;
 }

 if (values) free(values);

 return 1;
}


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

 /* set the interlace for reading by component interlacing scheme */
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
 * Function: copy_vs
 *
 * Purpose: copy a VS from input file to output file
 *
 * Return: 0, -1 for error 
 *
 * Programmer: Pedro Vicente, pvn@ncsa.uiuc.edu
 *
 * Date: July 11, 2003
 *
 *-------------------------------------------------------------------------
 */

int copy_vs( int32 infile_id,
             int32 outfile_id,
             int32 tag,               /* tag of input VS */
             int32 ref,               /* ref of input VS */
             int32 vgroup_id_out_par, /* output parent group ID */
             char*path_name,          /* absolute path for input group name */
             options_t *options,
             table_t *table,
             int   is_lone)
{
 intn  status_n;              /* returned status_n for functions returning an intn  */
 int32 status_32,             /* returned status_n for functions returning an int32 */
       vdata_id,              /* vdata identifier */
       vdata_out,             /* vdata identifier */
       vdata_ref,             /* reference number of the vdata */
       n_records,             /* number of records */
       vdata_size, 
       interlace_mode,
       field_type, 
       field_order;
 int   n_fields, n_attrs;
 char  vdata_name [VSNAMELENMAX], vdata_class[VSNAMELENMAX];
 char  fieldname_list[VSFIELDMAX*FIELDNAMELENMAX];
 char  *path=NULL;
 char  *field_name;
 uint8 *buf=NULL;
 int   i, j, ret=1;

/*-------------------------------------------------------------------------
 * attach the vdata, gets its name and class
 *-------------------------------------------------------------------------
 */ 

 if ((vdata_id  = VSattach (infile_id, ref, "r")) == FAIL ){
  printf( "Failed to attach vdata ref %d\n", ref);
  return-1;
 }
 if ((status_32 = VSgetname  (vdata_id, vdata_name)) == FAIL ){
  printf( "Failed to name for vdata ref %d\n", ref);
  return-1;
 }
 if ((status_32 = VSgetclass (vdata_id, vdata_class)) == FAIL ){
  printf( "Failed to name for vdata ref %d\n", ref);
  return-1;
 }
 
 /* ignore reserved HDF groups/vdatas; they are lone ones */
 if( is_lone==1 && vdata_class != NULL) {
  if( is_reserved(vdata_class)){
   if ((status_32 = VSdetach (vdata_id)) == FAIL )
    printf( "Failed to detach vdata <%s>\n", path_name);
   return 0;
  }
 }
 
 /* initialize path */
 path=get_path(path_name,vdata_name);
 
 /* add object to table */
 table_add(table,tag,ref,path);
 
#if defined(HZIP_DEBUG)
 printf ("\t%s %d\n", path, ref); 
#endif
 
 if (options->verbose)
 {
  printf(PFORMAT,"","",path);    
 }
 
 /* check inspection mode */
 if ( options->trip==0 ) {
  if ((status_32 = VSdetach (vdata_id)) == FAIL )
   printf( "Failed to detach vdata <%s>\n", path_name);
  if (path) free(path);
  return 0;
 }
 
 
/*-------------------------------------------------------------------------
 * get vdata info
 *-------------------------------------------------------------------------
 */ 
 
 if (VSinquire(vdata_id, &n_records, &interlace_mode, fieldname_list, 
  &vdata_size, vdata_name) == FAIL) {
  printf( "Failed to get info for vdata ref %d\n", ref);
  if (path) free(path);
  return-1;
 }
 
#if defined( HZIP_DEBUG)
 printf( 
  "Transferring vdata %s: class=%s, %d recs, interlace=%d, size=%d\n\tfields='%s'\n",
  vdata_name, vdata_class, n_records, interlace_mode, vdata_size, 
  fieldname_list);
#endif
 
 
 /*-------------------------------------------------------------------------
  * create the VS in the output file.  the vdata reference number is set
  * to -1 for creating and the access mode is "w" for writing 
  *-------------------------------------------------------------------------
  */ 
 
 if ((vdata_out  = VSattach (outfile_id, -1, "w")) == FAIL) {
  printf( "Failed to create new VS <%s>\n", path);
  status_32 = VSdetach (vdata_id);
  if (path) free(path);
  return -1;
 }
 if ((status_32 = VSsetname (vdata_out, vdata_name)) == FAIL) {
  printf( "Failed to set name for new VS <%s>\n", path);
  ret=-1;
  goto out;
 }
 if ((status_32 = VSsetclass(vdata_out, vdata_class)) == FAIL) {
  printf( "Failed to set class for new VS <%s>\n", path);
  ret=-1;
  goto out;
 }
 if (VSsetinterlace(vdata_out, interlace_mode) == FAIL) {
  printf( "Failed to set interlace mode for output vdata\n");
  ret=-1;
  goto out;
 }


/*-------------------------------------------------------------------------
 * define the fields for vdata_out
 *-------------------------------------------------------------------------
 */ 
 
 if ((n_fields = VFnfields(vdata_id)) == FAIL ){
  printf( "Failed getting fields for VS <%s>\n", path);
  ret=-1;
  goto out;
 }
 
 for (i = 0; i < n_fields; i++) {
  field_name = VFfieldname(vdata_id, i);
  field_type = VFfieldtype(vdata_id, i);
  field_order = VFfieldorder(vdata_id, i);
  if (VSfdefine(vdata_out, field_name, field_type, field_order) == FAIL) {
   printf( "Error: cannot define fields for VS <%s>\n", path);
   ret=-1;
   goto out;
  }
 }
 
 /* Set fields */
 if ((status_n = VSsetfields(vdata_out, fieldname_list)) == FAIL) {
  printf( "Error: cannot define fields for VS <%s>\n", path);
  ret=-1;
  goto out;
 }

 
/*-------------------------------------------------------------------------
 * read, write vdata
 *-------------------------------------------------------------------------
 */ 

 /* Set fields for reading */
 if ((status_n = VSsetfields(vdata_id, fieldname_list)) == FAIL) {
   printf( "Error: cannot define fields for VS <%s>\n", path);
   ret=-1;
   goto out;
  }
 if (n_records>0)
 {
  if ((buf = (uint8 *)malloc(n_records * vdata_size)) == NULL ){
   printf( "Failed to get memory for new VS <%s>\n", path);
   ret=-1;
   goto out;
  }
  if (VSread(vdata_id, buf, n_records, interlace_mode) == FAIL) {
   printf( "Error reading vdata <%s>\n", path);
   ret=-1;
   goto out;
  }
  if (VSwrite(vdata_out, buf, n_records, interlace_mode) == FAIL) {
   printf( "Error writing vdata <%s>\n", path);
   ret=-1;
   goto out;
  }
 }
 

/*-------------------------------------------------------------------------
 * read, write attributes
 *-------------------------------------------------------------------------
 */ 
 
 if ((n_attrs = VSfnattrs( vdata_id, -1 )) == FAIL ){
  printf( "Failed getting attributes for VS <%s>\n", path);
  ret=-1;
  goto out;
 }
 for (i = 0; i < n_attrs; i++) {
  copy_vdata_attribute(vdata_id, vdata_out, -1, i);
 }
 
/*-------------------------------------------------------------------------
 * read, write field attributes
 *-------------------------------------------------------------------------
 */ 
  
 for (i = 0; i < n_fields; i++) {
  if ((n_attrs = VSfnattrs(vdata_id, i)) == FAIL ){
   printf( "Failed getting fields for VS <%s>\n", path);
   ret=-1;
   goto out;
  }
  for (j = 0; j < n_attrs; j++) {
   copy_vdata_attribute(vdata_id, vdata_out, i, j);
  }
 }
  
/*-------------------------------------------------------------------------
 * add VS to group, if needed
 *-------------------------------------------------------------------------
 */ 
 if (vgroup_id_out_par) 
 {
  /* obtain the reference number of the new VS  */
  if ((vdata_ref=VSQueryref(vdata_out)) == 0) {
   printf( "Failed to get new VS reference in <%s>\n", path);
  }
  
  /* add the VS to the vgroup. the INPUT TAG is used */
  if ((status_32 = Vaddtagref (vgroup_id_out_par, tag, vdata_ref)) == FAIL) {
   printf( "Failed to add new VS to group <%s>\n", path);
  }
 }

/*-------------------------------------------------------------------------
 * copy ANs
 *-------------------------------------------------------------------------
 */ 

 copy_vs_an(infile_id,outfile_id,vdata_id,vdata_out,path,options);
 
out:
 /* terminate access to the VSs */
 status_32 = VSdetach (vdata_id);
 status_32 = VSdetach (vdata_out);
 
 if (path)
  free(path);
 if (buf)
  free(buf);
 
 return ret;
}





/*-------------------------------------------------------------------------
 * Function: copy_vg
 *
 * Purpose: copy a VG from input file to output file
 *
 * Return: void
 *
 * Programmer: Pedro Vicente, pvn@ncsa.uiuc.edu
 *
 * Date: July 11, 2003
 *
 *-------------------------------------------------------------------------
 */

void copy_vg(char* infname,
             char* outfname,
             int32 infile_id,
             int32 outfile_id,
             int32 tag,               /* tag of input VS */
             int32 ref,               /* ref of input VS */
             int32 vgroup_id_out_par, /* output parent group ID */
             char*path_name,          /* absolute path for input group name */
             options_t *options,
             table_t *table)
{
 int32 status_32,             /* returned status_n for functions returning an int32 */
       vgroup_id,             /* vg identifier */
       vgroup_id_out,         /* vg identifier */
       ntagrefs,              /* number of tag/ref pairs in a vgroup */
       *tags,                 /* buffer to hold the tag numbers of vgroups   */
       *refs,                 /* buffer to hold the ref numbers of vgroups   */
       vg_index;              /* position of a vgroup in the vgroup  */
 char  vgroup_name [VGNAMELENMAX], vgroup_class[VGNAMELENMAX];
 char  *path=NULL;
 
/*
 * attach to the current vgroup then get its
 * name and class. note: the current vgroup must be detached before
 * moving to the next.
 */

 vgroup_id = Vattach (infile_id, ref, "r");
 status_32 = Vgetname (vgroup_id, vgroup_name);
 status_32 = Vgetclass (vgroup_id, vgroup_class);
 
 /* ignore reserved HDF groups/vdatas */
 if( is_reserved(vgroup_class)){
  status_32 = Vdetach (vgroup_id);
  return;
 }
 if(vgroup_name != NULL) 
  if(strcmp(vgroup_name,GR_NAME)==0) {
   status_32 = Vdetach (vgroup_id);
   return;
 }
  
 /* initialize path */
 path=get_path(path_name,vgroup_name);
 
 /* add object to table */
 table_add(table,tag,ref,path);
  
#if defined(HZIP_DEBUG)
 printf ("\t%s %d\n", path, ref); 
#endif
  
 if ( options->trip==0 ) 
 {
  /*we must go to other groups always */
 }
 
/* 
 * create the group in the output file.  the vgroup reference number is set
 * to -1 for creating and the access mode is "w" for writing 
 */
 vgroup_id_out = Vattach (outfile_id, -1, "w");
 status_32     = Vsetname (vgroup_id_out, vgroup_name);
 status_32     = Vsetclass (vgroup_id_out, vgroup_class);
 
 /* insert the created vgroup into its parent */
 vg_index = Vinsert (vgroup_id_out_par, vgroup_id_out);
 
 /* insert objects for this group */
 ntagrefs  = Vntagrefs(vgroup_id);
 if ( ntagrefs > 0 )
 {
  tags = (int32 *) malloc(sizeof(int32) * ntagrefs);
  refs = (int32 *) malloc(sizeof(int32) * ntagrefs);
  Vgettagrefs(vgroup_id, tags, refs, ntagrefs);
  /* recurse */
  vgroup_insert(infname,outfname,infile_id,outfile_id,vgroup_id_out,
   path,tags,refs,ntagrefs,table,options);
  free (tags);
  free (refs);
 }
 status_32 = Vdetach (vgroup_id);
 status_32 = Vdetach (vgroup_id_out);
 
 if (path)
  free(path);
  
 return;
}




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


