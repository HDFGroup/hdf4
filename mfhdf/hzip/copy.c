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



#if !defined (ONE_TABLE)

comp_info_t* get_comp(options_t *options, char* path);
int          get_chunk(options_t *options, char* path, int32 *chunk_lengths);
#endif

int copy_sds_attrs(int32 sds_id,
                       int32 sds_out,
                       int32 nattrs,          
                       options_t *options);

int copy_gr_attrs(int32 ri_id,
                  int32 ri_out,
                  int32 nattrs,          
                  options_t *options);

int copy_vdata_attribute(int32 in, int32 out, int field, int attr);


void options_get_info(options_t      *options,     /* global options */
                      int32          *chunk_flags, /* chunk flags OUT */
                      HDF_CHUNK_DEF  *chunk_def,   /* chunk definition OUT */
                      int            *info,        /* compression info OUT */
                      comp_coder_t   *comp_type,   /* compression type OUT  */
                      int            rank,         /* rank of object IN */
                      char           *path         /* path of object IN */
                      );


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
             table_t *table)
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

#if !defined (ONE_TABLE)
 int              chunk_rank;     /* rank got from chunk table  */
 int32            chunk_lengths[MAX_VAR_DIMS]; /* chunk lengths along each dimension */
#endif


 
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
 table_add(table,tag,ref,path,options->verbose&&options->trip==0);

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
   chunk_def_in.comp.cinfo.skphuff.skp_size = c_info_in.skphuff.skp_size;
   break;
  case COMP_CODE_DEFLATE:
   chunk_def_in.comp.comp_type              = COMP_CODE_DEFLATE;
   chunk_def_in.comp.cinfo.deflate.level    = c_info_in.deflate.level;
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
  case COMP_CODE_RLE:
   break;
  case COMP_CODE_SKPHUFF:
   info  = c_info_in.skphuff.skp_size;
   break;
  case COMP_CODE_DEFLATE:
   info  = c_info_in.deflate.level;
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
   chunk_def.comp.cinfo.skphuff.skp_size = c_info_in.skphuff.skp_size;
   break;
  case COMP_CODE_DEFLATE:
   chunk_def.comp.comp_type              = COMP_CODE_DEFLATE;
   chunk_def.comp.cinfo.deflate.level    = c_info_in.deflate.level;
   break;
  };
 }


#if !defined (ONE_TABLE)


/*-------------------------------------------------------------------------
 * get the compression/chunk info of this object from the table
 * translate to usable info
 * this is done ONLY for the second trip inspection 
 *-------------------------------------------------------------------------
 */
 
 /* check inspection mode */
 if ( options->trip>0 ) 
 {

 if (options->compress==ALL && options->chunk==NONE)
 {
  comp_type   = options->comp.type;
  info        = options->comp.info;
 }
 else if (options->compress==SELECTED && options->chunk==NONE)
 {
  comp        = get_comp(options,path);
  if (comp!=NULL)
  {
   comp_type   = comp->type;
   info        = comp->info;
  }
 }
 else if (options->compress==NONE && options->chunk==SELECTED)
 {
  comp_type   = COMP_CODE_NONE;
  info        = 1; 
  chunk_rank  = get_chunk(options,path,chunk_lengths);
  if (chunk_rank!=-1 )    
  {
   assert(chunk_rank==rank);
   chunk_flags = HDF_CHUNK;
   /* Define chunk's dimensions */
   for (i = 0; i < rank; i++) 
    chunk_def.chunk_lengths[i] = chunk_lengths[i];
  }
 }
 else if (options->compress==NONE && options->chunk==ALL)
 {
  comp_type   = COMP_CODE_NONE;
  info        = 1; 
  chunk_rank  = options->chunk_rank;
  /* 
    this dataset is not possible to chunk with the global parameters
    do not chunk it
   */
  if (chunk_rank!=rank)
  {
   chunk_flags = HDF_NONE;
  }
  else
  {
   chunk_flags = HDF_CHUNK;
   /* Define chunk's dimensions */
   for (i = 0; i < rank; i++) 
    chunk_def.chunk_lengths[i] = options->chunk_def.chunk_lengths[i];
  }
 }
 else if (options->compress==SELECTED && options->chunk==SELECTED)
 {
  comp        = get_comp(options,path);
  if (comp!=NULL)
  {
   comp_type   = comp->type;
   info        = comp->info;
  }
  chunk_rank  = get_chunk(options,path,chunk_lengths);
  if (chunk_rank!=-1 )    
  {
   assert(chunk_rank==rank);
   chunk_flags = HDF_CHUNK;
   if (comp) 
   {
    chunk_flags              = HDF_CHUNK | HDF_COMP;
    chunk_def.comp.comp_type = comp->type;
    switch (comp->type)
    {
     case COMP_CODE_RLE:
      break;
     case COMP_CODE_SKPHUFF:
      chunk_def.comp.cinfo.skphuff.skp_size = comp->info;
      break;
     case COMP_CODE_DEFLATE:
      chunk_def.comp.cinfo.deflate.level    = comp->info;
      break;
    };
    
   }
   /* Define chunk's dimensions */
   for (i = 0; i < rank; i++) 
   {
    chunk_def.chunk_lengths[i] = chunk_lengths[i];
    /* To use chunking with RLE, Skipping Huffman, and GZIP compression */
    chunk_def.comp.chunk_lengths[i] = chunk_lengths[i];
   }
  }
 }
 else if (options->compress==SELECTED && options->chunk==ALL)
 {
  comp        = get_comp(options,path);
  chunk_rank  = options->chunk_rank;
  /* 
    this dataset is not possible to chunk with the global parameters
    do not chunk it
   */
  if (chunk_rank!=rank)
  {
   chunk_flags = HDF_NONE;
  }
  else
  {
   if (comp!=NULL) /* we have compression for this one */
   {
    /* Define chunk */
    chunk_flags = HDF_CHUNK | HDF_COMP;
    for (i = 0; i < rank; i++) 
     chunk_def.chunk_lengths[i] = options->chunk_def.chunk_lengths[i];
    chunk_def.comp.comp_type = comp->type;
    switch (comp->type)
    {
    case COMP_CODE_RLE:
     break;
    case COMP_CODE_SKPHUFF:
     chunk_def.comp.cinfo.skphuff.skp_size = comp->info;
     break;
    case COMP_CODE_DEFLATE:
     chunk_def.comp.cinfo.deflate.level    = comp->info;
     break;
    };
   }
   else  /* we do not have compression for this one */
   {
    /* Define chunk */
    chunk_flags = HDF_CHUNK;
    for (i = 0; i < rank; i++) 
     chunk_def.chunk_lengths[i] = options->chunk_def.chunk_lengths[i];;
   }
  }
 }
 else if (options->compress==ALL && options->chunk==SELECTED)
 {
  chunk_rank  = get_chunk(options,path,chunk_lengths);
  if (chunk_rank!=-1 )    
  {
   assert(chunk_rank==rank);
   /* Define chunk's dimensions */
   for (i = 0; i < rank; i++) 
    chunk_def.chunk_lengths[i] = chunk_lengths[i];
   chunk_flags = HDF_CHUNK | HDF_COMP;
   chunk_def.comp.comp_type = options->comp.type;
   switch (options->comp.type)
   {
   case COMP_CODE_RLE:
    break;
   case COMP_CODE_SKPHUFF:
    chunk_def.comp.cinfo.skphuff.skp_size = options->comp.info;
    break;
   case COMP_CODE_DEFLATE:
    chunk_def.comp.cinfo.deflate.level    = options->comp.info;
    break;
   };
   for (i = 0; i < rank; i++) 
   {
    /* To use chunking with RLE, Skipping Huffman, and GZIP compression */
    chunk_def.comp.chunk_lengths[i] = chunk_lengths[i];
   }
  }
  else /*compress these with SDsetcompress */
  {
   comp_type   = options->comp.type;
   info        = options->comp.info;
   chunk_flags = HDF_NONE;
  }
 }
 else if (options->compress==ALL && options->chunk==ALL)
 {
  chunk_rank  = options->chunk_rank;
  /* 
  this dataset is not possible to chunk with the global parameters
  do not chunk it
  */
  if (chunk_rank!=rank)
  {
   chunk_flags = HDF_NONE;
  }
  else
  {
   /* Define chunk */
   chunk_flags = HDF_CHUNK | HDF_COMP;
   for (i = 0; i < rank; i++) 
    chunk_def.chunk_lengths[i] = options->chunk_def.chunk_lengths[i];
   chunk_def.comp.comp_type = options->comp.type;
   switch (options->comp.type)
   {
   case COMP_CODE_RLE:
    break;
   case COMP_CODE_SKPHUFF:
    chunk_def.comp.cinfo.skphuff.skp_size = options->comp.info;
    break;
   case COMP_CODE_DEFLATE:
    chunk_def.comp.cinfo.deflate.level    = options->comp.info;
    break;
   };
   for (i = 0; i < rank; i++) 
   {
    /* To use chunking with RLE, Skipping Huffman, and GZIP compression */
    chunk_def.comp.chunk_lengths[i] = options->chunk_def.chunk_lengths[i];
   }
  }
 }
 } /* check inspection mode */

#else

/*-------------------------------------------------------------------------
 * get the compression/chunk info of this object from the table
 * translate to usable info
 * this is done ONLY for the second trip inspection 
 *-------------------------------------------------------------------------
 */
 
 /* check inspection mode */
 if ( options->trip>0 ) 
 {
  options_get_info(options,      /* global options */
                   &chunk_flags, /* chunk flags OUT */
                   &chunk_def,   /* chunk definition OUT */
                   &info,        /* compression info OUT */
                   &comp_type,   /* compression type OUT  */
                   rank,         /* rank of object IN */
                   path          /* path of object IN */
                   );
 } /* check inspection mode */



#endif /* ONE_TABLE */



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
 * read sds and create new one
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

 /* alloc */
 if ((buf = (VOIDP) HDmalloc(nelms * eltsz)) == NULL) {
  fprintf(stderr, "Failed to allocate %d elements of size %d\n", nelms, eltsz);
  SDendaccess(sds_id);
  if (path) free(path);
  return-1;
 }
 
 /* read data */
 if (SDreaddata (sds_id, start, NULL, edges, buf) == FAIL) {
  fprintf(stderr, "Could not read SDS <%s>\n", path);
  SDendaccess(sds_id);
  if (path) free(path);
  return-1;
 }

 /* create output SDS */
 if ((sds_out = SDcreate(sd_out,sds_name,dtype,rank,dimsizes)) == FAIL) {
  fprintf(stderr, "Failed to create new SDS <%s>\n", path);
  goto out;
  ret=-1;
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
   fprintf(stderr, "Error: Failed to set chunk dimensions for <%s>\n", path);
   goto out;
   ret=-1;
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
  case COMP_CODE_RLE:         
   break;
  case COMP_CODE_SKPHUFF:     
   c_info.skphuff.skp_size = info;
   break;
  case COMP_CODE_DEFLATE:
   c_info.deflate.level = info;
   break;
  default:
   fprintf(stderr, "Error: Unrecognized compression code %d\n", comp_type);
  }

  if (SDsetcompress (sds_out, comp_type, &c_info)==FAIL)
  {
   fprintf(stderr, "Error: Failed to set compression for <%s>\n", path);
   goto out;
   ret=-1;
  }
 }
 
 
 /* write the data */
 if (SDwritedata(sds_out, start, NULL, edges, buf) == FAIL) {
  fprintf(stderr, "Failed to write to new SDS <%s>\n", path);
  goto out;
  ret=-1;
 }

/*-------------------------------------------------------------------------
 * copy attributes
 *-------------------------------------------------------------------------
 */ 
 
 if( copy_sds_attrs(sds_id,sds_out,nattrs,options)==FAIL) {
  goto out;
  ret=-1;
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
   fprintf(stderr, "Failed to get dimension %d of SDS <%s>\n", i, path);
   goto out;
   ret=-1;
  }
  /* get dimension handle for output dimension */
  if ((dim_out = SDgetdimid(sds_out, i)) == FAIL) {
   fprintf(stderr, "Failed to get dim_id for dimension %d of SDS <%s>\n", i, path);
   goto out;
   ret=-1;
  }
  /* get dimension information for input dimension */
  if (SDdiminfo(dim_id, dim_name, &dim_size, &dtype, &nattrs) == FAIL) {
   fprintf(stderr, "Failed to get info for dimension %d of SDS <%s>\n", i, path);
   goto out;
   ret=-1;
  }
  /* set output dimension name */
  if (SDsetdimname(dim_out, dim_name) == FAIL) {
   fprintf(stderr, "Failed to set dimension name %d of SDS <%s>\n", i, path);
   goto out;
   ret=-1;
  }
  /* copy attributes */
  if (nattrs && copy_sds_attrs(dim_id, dim_out, nattrs, options) == FAIL) {
   fprintf(stderr, "Failed to copy attributes for dimension %d of of SDS <%s>\n", i, path);
   goto out;
   ret=-1;
  }
  /* copy scale information over */
  if (dtype != 0) 
  {
   /* compute the number of the bytes for each value. */
   numtype = dtype & DFNT_MASK;
   eltsz = DFKNTsize(numtype | DFNT_NATIVE);

   if ((dim_buf = (VOIDP) HDmalloc(dim_size * eltsz)) == NULL) {
    fprintf(stderr, "Failed to alloc %d for dimension scale\n", dim_size);
    goto out;
    ret=-1;
   }
   if (SDgetdimscale(dim_id, dim_buf) == FAIL) {
    fprintf(stderr, "Failed to get scale info for %s\n", dim_name);
    goto out;
    ret=-1;
   }
   if (SDsetdimscale(dim_out, dim_size, dtype, dim_buf) == FAIL) {
    fprintf(stderr, "Failed to set scale info for %s\n", dim_name);
    goto out;
    ret=-1;
   }
  }
 }


/*-------------------------------------------------------------------------
 * add SDS to group
 *-------------------------------------------------------------------------
 */ 
 
 /* add it to group, if present */
 if (vgroup_id_out_par) 
 {
  /* obtain the reference number of the new SDS using its identifier */
  if ((sds_ref = SDidtoref (sds_out)) == FAIL) {
   fprintf(stderr, "Failed to get new SDS reference in <%s>\n", path);
   goto out;
   ret=-1;
  }

  /* add the SDS to the vgroup. the tag DFTAG_NDG is used */
  if ((status_32 = Vaddtagref (vgroup_id_out_par, TAG_GRP_DSET, sds_ref)) == FAIL) {
   fprintf(stderr, "Failed to add new SDS to group <%s>\n", path);
   goto out;
   ret=-1;
  }
 }

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
 * Purpose: copy SDS attributes from input file to output file 
 *
 * Return: 1, for success, -1 for error 
 *
 * Programmer: Pedro Vicente, pvn@ncsa.uiuc.edu
 *
 * Date: July 10, 2003
 *
 *-------------------------------------------------------------------------
 */

int copy_sds_attrs(int32 sds_id,
                   int32 sds_out,
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
  if (SDattrinfo (sds_id, i, attr_name, &dtype, &nelms) == FAIL) {
   fprintf(stderr, "Cannot get info for attribute number %d\n", i);
   return-1;
  }
  /* compute the number of the bytes for each value. */
  numtype = dtype & DFNT_MASK;
  eltsz   = DFKNTsize(numtype | DFNT_NATIVE);
  if ((attr_buf = (VOIDP) HDmalloc(nelms * eltsz)) == NULL) {
   fprintf(stderr, "Error allocating %d values of size %d for attribute %s",
    nelms, numtype, attr_name);
   return-1;
  }
  /* read attributes from input SDS */
  if (SDreadattr(sds_id, i, attr_buf) == FAIL) {
   fprintf(stderr, "Cannot read attribute %s\n", attr_name);
   return-1;
  }
  /* put attributes into output SDS */
  if (SDsetattr(sds_out, attr_name, dtype, nelms, attr_buf) == FAIL) {
   fprintf(stderr, "Cannot write attribute %s\n", attr_name);
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
   fprintf(stderr, "Cannot get info for attribute number %d\n", i);
   return-1;
  }
  /* compute the number of the bytes for each value. */
  numtype = dtype & DFNT_MASK;
  eltsz   = DFKNTsize(numtype | DFNT_NATIVE);
  if ((attr_buf = (VOIDP) HDmalloc(nelms * eltsz)) == NULL) {
   fprintf(stderr, "Error allocating %d values of size %d for attribute %s",
    nelms, numtype, attr_name);
   return-1;
  }
  /* read attributes from input GR */
  if (GRgetattr(ri_id, i, attr_buf) == FAIL) {
   fprintf(stderr, "Cannot read attribute %s\n", attr_name);
   return-1;
  }
  /* put attributes into output GR */
  if (GRsetattr(ri_out, attr_name, dtype, nelms, attr_buf) == FAIL) {
   fprintf(stderr, "Cannot write attribute %s\n", attr_name);
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

int copy_vdata_attribute(int32 in, int32 out, int field, int attr)
{
 char   attr_name[MAX_NC_NAME];
 int32  n_values, attr_size, attr_type;
 VOIDP  *values=NULL;

 /* Get attribute information */
 VSattrinfo(in, field, 0, attr_name, &attr_type, &n_values, &attr_size);

 /* Allocate space for attribute values */
 if ((values = (VOIDP)malloc(attr_size * n_values)) == NULL) {
  fprintf(stderr, "Cannot allocate %d values of size %d for attribute %s",
   n_values, attr_size, attr_name);
  return-1;
 }

 /* Read attribute from input object */
 if (VSgetattr(in, field, 0, values) == FAIL) {
  fprintf(stderr, "Cannot read attribute %s\n", attr_name);
  return-1;
 }

 /* Write attribute to output object */
 if (VSsetattr(out, field, attr_name, attr_type, n_values, values) == FAIL) {
  fprintf(stderr, "Cannot write attribute %s\n", attr_name);
  return-1;
 }

 if (values)
  free(values);

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

int  copy_gr(int32 gr_in,
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
 int           i, j, ret, rank=2;
 int32         start[2],       /* read start */
               edges[2],       /* read edges */
               numtype,        /* number type */
               eltsz,          /* element size */
               nelms,          /* number of elements */
               data_size;
 VOIDP         buf=NULL;


#if !defined (ONE_TABLE)
 int           chunk_rank;     /* rank got from chunk table  */
 int32         chunk_lengths[MAX_VAR_DIMS]; /* chunk lengths along each dimension */
#endif

 ri_index = GRreftoindex(gr_in,(uint16)ref);
 ri_id    = GRselect(gr_in,ri_index);
   
 status_n = GRgetiminfo(ri_id,gr_name,&n_comps,&dtype,&interlace_mode,dimsizes,&n_attrs);
 
 /* initialize path */
 path=get_path(path_name,gr_name);

 /* add object to table */
 table_add(table,tag,ref,path,options->verbose&&options->trip==0);
 
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
  case COMP_CODE_RLE:
   chunk_def_in.comp.comp_type              = COMP_CODE_RLE;
   break;
  case COMP_CODE_SKPHUFF:
   chunk_def_in.comp.comp_type              = COMP_CODE_SKPHUFF;
   chunk_def_in.comp.cinfo.skphuff.skp_size = c_info_in.skphuff.skp_size;
   break;
  case COMP_CODE_DEFLATE:
   chunk_def_in.comp.comp_type              = COMP_CODE_DEFLATE;
   chunk_def_in.comp.cinfo.deflate.level    = c_info_in.deflate.level;
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
  case COMP_CODE_RLE:
   break;
  case COMP_CODE_SKPHUFF:
   info  = c_info_in.skphuff.skp_size;
   break;
  case COMP_CODE_DEFLATE:
   info  = c_info_in.deflate.level;
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
   chunk_def.comp.cinfo.skphuff.skp_size = c_info_in.skphuff.skp_size;
   break;
  case COMP_CODE_DEFLATE:
   chunk_def.comp.comp_type              = COMP_CODE_DEFLATE;
   chunk_def.comp.cinfo.deflate.level    = c_info_in.deflate.level;
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
  options_get_info(options,      /* global options */
                   &chunk_flags, /* chunk flags OUT */
                   &chunk_def,   /* chunk definition OUT */
                   &info,        /* compression info OUT */
                   &comp_type,   /* compression type OUT  */
                   rank,         /* rank of object IN */
                   path          /* path of object IN */
                   );
 } /* check inspection mode */


 
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

 /* alloc */
 if ((buf = (VOIDP) HDmalloc(data_size)) == NULL) {
  fprintf(stderr, "Failed to allocate %d elements of size %d\n", nelms, eltsz);
  GRendaccess(ri_id);
  if (path) free(path);
  return-1;
 }

 /* set the interlace for reading by component interlacing scheme */
 if ( GRreqimageil(ri_id, interlace_mode) == FAIL ){
  fprintf(stderr, "Could not read GR <%s>\n", path);
  GRendaccess(ri_id);
  if (path) free(path);
  return-1;
 }
 
 /* read data */
 if (GRreadimage (ri_id, start, NULL, edges, buf) == FAIL) {
  fprintf(stderr, "Could not read GR <%s>\n", path);
  GRendaccess(ri_id);
  if (path) free(path);
  return-1;
 }

 
 /* create output GR */
 if ((ri_out = GRcreate(gr_out,gr_name,n_comps,dtype,interlace_mode,dimsizes)) == FAIL) {
  fprintf(stderr, "Failed to create new GR <%s>\n", path);
  goto out;
  ret=-1;
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
   fprintf(stderr, "Error: Failed to set chunk dimensions for <%s>\n", path);
   goto out;
   ret=-1;
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
  case COMP_CODE_RLE:         
   break;
  case COMP_CODE_SKPHUFF:     
   c_info.skphuff.skp_size = info;
   break;
  case COMP_CODE_DEFLATE:
   c_info.deflate.level = info;
   break;
  default:
   fprintf(stderr, "Error: Unrecognized compression code %d\n", comp_type);
  }

  if (GRsetcompress (ri_out, comp_type, &c_info)==FAIL)
  {
   fprintf(stderr, "Error: Failed to set compression for <%s>\n", path);
   goto out;
   ret=-1;
  }
 }
 
 
 /* write the data */
 if (GRwriteimage(ri_out, start, NULL, edges, buf) == FAIL) {
  fprintf(stderr, "Failed to write to new GR <%s>\n", path);
  goto out;
  ret=-1;
 }

/*-------------------------------------------------------------------------
 * copy attributes
 *-------------------------------------------------------------------------
 */ 
 
 if( copy_gr_attrs(ri_id,ri_out,n_attrs,options)==FAIL) {
  goto out;
  ret=-1;
 }
 
/*-------------------------------------------------------------------------
 * add GR to group, if needed
 *-------------------------------------------------------------------------
 */ 
 if (vgroup_id_out_par) 
 {
  /* obtain the reference number of the new SDS using its identifier */
  if ((gr_ref = GRidtoref (ri_out)) == FAIL) {
   fprintf(stderr, "Failed to get new GR reference in <%s>\n", path);
  }

  /* add the GR to the vgroup. the tag DFTAG_RIG is used */
  if ((status_32 = Vaddtagref (vgroup_id_out_par, TAG_GRP_IMAGE, gr_ref)) == FAIL) {
   fprintf(stderr, "Failed to add new GR to group <%s>\n", path);
  }
 }


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
             table_t *table)
{
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



 vdata_id  = VSattach (infile_id, ref, "r");
 status_32 = VSgetname  (vdata_id, vdata_name);
 status_32 = VSgetclass (vdata_id, vdata_class);
 
 /* ignore reserved HDF groups/vdatas */
 if(vdata_class != NULL) {
  if( is_reserved(vdata_class)){
   status_32 = Vdetach (vdata_id);
   return 0;
  }
 }
 
 /* initialize path */
 path=get_path(path_name,vdata_name);
 
 /* add object to table */
 table_add(table,tag,ref,path,options->verbose&&options->trip==0);
 
#if defined(HZIP_DEBUG)
   printf ("\t%s %d\n", path, ref); 
#endif

 if (options->verbose)
 {
  printf(PFORMAT,"","",path);    
 }

  /* check inspection mode */
 if ( options->trip==0 ) {
  status_32 = Vdetach (vdata_id);
  if (path) free(path);
  return 0;
 }


/*-------------------------------------------------------------------------
 * get vdata
 *-------------------------------------------------------------------------
 */ 
 
 if (VSinquire(vdata_id, &n_records, &interlace_mode, fieldname_list, 
  &vdata_size, vdata_name) == FAIL) {
  fprintf(stderr, "Failed to get info for vdata ref %d\n", ref);
  return-1;
 }

#if defined( HZIP_DEBUG)
  fprintf(stderr, 
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
  fprintf(stderr, "Failed to create new VS <%s>\n", path);
  status_32 = VSdetach (vdata_id);
  if (path) free(path);
  return -1;
 }

 status_32  = VSsetname (vdata_out, vdata_name);
 status_32  = VSsetclass(vdata_out, vdata_class);
 
 if (VSsetinterlace(vdata_out, interlace_mode) == FAIL) {
  fprintf(stderr, "Failed to set interlace mode for output vdata\n");
  if (path) free(path);
  return -1;
 }

  
/*-------------------------------------------------------------------------
 * read, write
 *-------------------------------------------------------------------------
 */ 
 
 n_fields = VFnfields(vdata_id);
#if defined( HZIP_DEBUG)
  fprintf(stderr, "Number of fields = %d\n", n_fields);
#endif
 buf = (uint8 *)malloc(n_records * vdata_size);
 for (i = 0; i < n_fields; i++) {
  field_name = VFfieldname(vdata_id, i);
  field_type = VFfieldtype(vdata_id, i);
  field_order = VFfieldorder(vdata_id, i);
  VSfdefine(vdata_out, field_name, field_type, field_order);
  n_attrs = VSfnattrs(vdata_id, i);
  for (j = 0; j < n_attrs; j++) {
   copy_vdata_attribute(vdata_id, vdata_out, i, j);
  }
 }
 /* Set fields for reading */
 VSsetfields(vdata_id, fieldname_list);
 VSsetfields(vdata_out, fieldname_list);
 n_attrs = VSnattrs(vdata_id);
 for (i = 0; i < n_attrs; i++) {
  copy_vdata_attribute(vdata_id, vdata_out, -1, i);
 }
 
 /* Read into buffer, then write */
 if (VSread(vdata_id, buf, n_records, interlace_mode) == FAIL) {
  fprintf(stderr, "Error reading vdata %s\n", vdata_name);
  ret=-1;
  goto out;
 }

 if (VSwrite(vdata_out, buf, n_records, interlace_mode) == FAIL) {
  fprintf(stderr, "Error writing vdata %s\n", vdata_name);
  ret=-1;
  goto out;
 }
 
 
/*-------------------------------------------------------------------------
 * add VS to group, if needed
 *-------------------------------------------------------------------------
 */ 

 if (vgroup_id_out_par) 
 {
  /* obtain the reference number of the new VS using its identifier */
  if ((vdata_ref = VSfind (outfile_id,vdata_name)) == 0) {
   fprintf(stderr, "Failed to get new VS reference in <%s>\n", path);
  }

  /* add the VS to the vgroup. the tag DFTAG_VS is used */
  if ((status_32 = Vaddtagref (vgroup_id_out_par, DFTAG_VS, vdata_ref)) == FAIL) {
   fprintf(stderr, "Failed to add new VS to group <%s>\n", path);
  }
 }


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

char * get_path(char*path_name, char*obj_name) 
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
 table_add(table,tag,ref,path,options->verbose&&options->trip==0);
  
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
 * Return: void
 *
 * Programmer: Pedro Vicente, pvn@ncsa.uiuc.edu
 *
 * Date: July 28, 2003
 *
 *-------------------------------------------------------------------------
 */


void options_get_info(options_t      *options,     /* global options */
                      int32          *chunk_flags, /* chunk flags OUT */
                      HDF_CHUNK_DEF  *chunk_def,   /* chunk definition OUT */
                      int            *info,        /* compression info OUT */
                      comp_coder_t   *comp_type,   /* compression type OUT  */
                      int            rank,         /* rank of object IN */
                      char           *path         /* path of object IN */
                      )
{

 int i;
 
/*-------------------------------------------------------------------------
 * CASE 1: chunk==ALL comp==SELECTED 
 *-------------------------------------------------------------------------
 */
 
 if (options->all_chunk==1 && options->all_comp==0)
 {
  obj_info_t *obj;
  
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
    case COMP_CODE_RLE:
     break;
    case COMP_CODE_SKPHUFF:
     chunk_def->comp.cinfo.skphuff.skp_size = obj->comp.info;
     break;
    case COMP_CODE_DEFLATE:
     chunk_def->comp.cinfo.deflate.level    = obj->comp.info;
     break;
    };
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
  obj_info_t *obj = options_get_object(path,options->op_tbl);
  
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
     case COMP_CODE_RLE:
      break;
     case COMP_CODE_SKPHUFF:
      chunk_def->comp.cinfo.skphuff.skp_size = obj->comp.info;
      break;
     case COMP_CODE_DEFLATE:
      chunk_def->comp.cinfo.deflate.level    = obj->comp.info;
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
  obj_info_t *obj = options_get_object(path,options->op_tbl);
  
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
   case COMP_CODE_RLE:
    break;
   case COMP_CODE_SKPHUFF:
    chunk_def->comp.cinfo.skphuff.skp_size = *info;
    break;
   case COMP_CODE_DEFLATE:
    chunk_def->comp.cinfo.deflate.level    = *info;
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
   case COMP_CODE_RLE:
    break;
   case COMP_CODE_SKPHUFF:
    chunk_def->comp.cinfo.skphuff.skp_size = *info;
    break;
   case COMP_CODE_DEFLATE:
    chunk_def->comp.cinfo.deflate.level    = *info;
    break;
   };
  }
 } /* else if */
 
}









#if !defined (ONE_TABLE)


/*-------------------------------------------------------------------------
 * Function: get_comp
 *
 * Purpose: get the compression info from table 
 *
 * Return: comp_info_t
 *
 * Programmer: Pedro Vicente, pvn@ncsa.uiuc.edu
 *
 * Date: July 16, 2003
 *
 *-------------------------------------------------------------------------
 */

comp_info_t* get_comp(options_t *options, char* path)
{
 int i, j;

 if (options->cp_tbl) {
  for ( i = 0; i < options->cp_tbl->nelems; i++)
  {
   for ( j = 0; j < options->cp_tbl->objs[i].n_objs; j++) 
   {
    char* obj_name=options->cp_tbl->objs[i].obj_list[j].obj;
    if (strcmp(obj_name,path)==0)
    {
     return (&options->cp_tbl->objs[i].comp);
    }
   }
  }
 }

 return NULL;
}


/*-------------------------------------------------------------------------
 * Function: get_chunk
 *
 * Purpose: get the chunk info from table 
 *
 * Return: 
 *
 * Programmer: Pedro Vicente, pvn@ncsa.uiuc.edu
 *
 * Date: July 21, 2003
 *
 *-------------------------------------------------------------------------
 */

int get_chunk(options_t *options, char* path, int32 *chunk_lengths)
{
 int i, j, k;

 if (options->ck_tbl) {
  for ( i = 0; i < options->ck_tbl->nelems; i++)
  {
   for ( j = 0; j < options->ck_tbl->objs[i].n_objs; j++) 
   {
    char* obj_name=options->ck_tbl->objs[i].obj_list[j].obj;
    if (strcmp(obj_name,path)==0)
    {
     for ( k = 0; k < options->ck_tbl->objs[i].rank; k++) 
      chunk_lengths[k]=options->ck_tbl->objs[i].chunk_lengths[k];
     return (options->ck_tbl->objs[i].rank);
    }
   }
  }
 }

 return -1;
}


#endif




