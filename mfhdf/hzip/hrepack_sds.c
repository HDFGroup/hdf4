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
#include "hrepack_sds.h"
#include "hrepack_an.h"
#include "hrepack_utils.h"
#include "hrepack_parse.h"
#include "hrepack_opttable.h"



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

