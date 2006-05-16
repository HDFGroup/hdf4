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
#include "hrepack_mattbl.h"
#include "hrepack_sdutil.h"
#include "hrepack_sds.h"
#include "hrepack_dim.h"

static 
int gen_dim(char* name,              /* name of SDS */
            int32 ref,               /* ref of SDS */
            int32 sd_in,
            int32 sd_out,
            table_t *td1,
            table_t *td2,
            options_t *options);


/*-------------------------------------------------------------------------
 * Function: match_dim
 *
 * Purpose: generate "lone" dimensions. 
 *  Find common dimension names; the algorithm used for this search is the 
 *  cosequential match algorithm and is described in 
 *  Folk, Michael; Zoellick, Bill. (1992). File Structures. Addison-Wesley.
 *
 * Return: void
 *
 * Programmer: Pedro Vicente Nunes, pvn@ncsa.uiuc.edu
 *
 * Date: May 10, 2006
 *
 *-------------------------------------------------------------------------
 */
void match_dim(int32 sd_in,
               int32 sd_out,
               table_t *td1,
               table_t *td2,
               options_t *options)
{
 int   cmp;
 int   more_names_exist = (td1->nobjs>0 && td2->nobjs>0) ? 1 : 0;
 int   curr1=0;
 int   curr2=0;
 int   nfound=0;
 /*build a common list */
 match_table_t *mattbl=NULL;
 unsigned infile[2]; 
 int      i;
 

/*-------------------------------------------------------------------------
 * build the list
 *-------------------------------------------------------------------------
 */
 match_dim_table_init( &mattbl );

 while ( more_names_exist )
 {
  cmp = strcmp( td1->objs[curr1].obj_name, td2->objs[curr2].obj_name );
  if ( cmp == 0 )
  {
   infile[0]=1; infile[1]=1;
   match_dim_table_add(mattbl,infile,
    td1->objs[curr1].obj_name,
    td1->objs[curr1].tag,
    td1->objs[curr1].ref,
    td2->objs[curr2].tag,
    td2->objs[curr2].ref);

   curr1++;
   curr2++;
  }
  else if ( cmp < 0 )
  {
   infile[0]=1; infile[1]=0;
   match_dim_table_add(mattbl,infile,
    td1->objs[curr1].obj_name,
    td1->objs[curr1].tag,
    td1->objs[curr1].ref,
    -1,
    -1);
   curr1++;
  }
  else 
  {
   infile[0]=0; infile[1]=1;
   match_dim_table_add(mattbl,infile,
    td2->objs[curr2].obj_name,
    -1,
    -1,
    td2->objs[curr2].tag,
    td2->objs[curr2].ref);
   curr2++;
  }

  more_names_exist = (curr1<td1->nobjs && curr2<td1->nobjs) ? 1 : 0;

 
 } /* end while */

 /* td1 did not end */
 if (curr1<td1->nobjs)
 {
  while ( curr1<td1->nobjs )
  {
   infile[0]=1; infile[1]=0;
   match_dim_table_add(mattbl,infile,
    td1->objs[curr1].obj_name,
    td1->objs[curr1].tag,
    td1->objs[curr1].ref,
    -1,
    -1);
   curr1++;
  }
 }

 /* td2 did not end */
 if (curr2<td2->nobjs)
 {
  while ( curr2<td2->nobjs )
  {
   infile[0]=0; infile[1]=1;
   match_dim_table_add(mattbl,infile,
    td2->objs[curr2].obj_name,
    -1,
    -1,
    td2->objs[curr2].tag,
    td2->objs[curr2].ref);
   curr2++;
  }
 }

/*-------------------------------------------------------------------------
 * print the list
 *-------------------------------------------------------------------------
 */
#if 0
 {
  char     c1, c2;
  if (options->verbose) {
   printf("---------------------------------------\n");
   printf("file1     file2\n");
   printf("---------------------------------------\n");
   for (i = 0; i < mattbl->nobjs; i++)
   {
    c1 = (char)((mattbl->objs[i].flags[0]) ? 'x' : ' ');
    c2 = (char)((mattbl->objs[i].flags[1]) ? 'x' : ' ');
    printf("%5c %6c    %-15s\n", c1, c2, mattbl->objs[i].obj_name);
   }
   printf("\n");
  }
 }
#endif


/*-------------------------------------------------------------------------
 * get objects from list1 not in list2
 *-------------------------------------------------------------------------
 */

 for (i = 0; i < mattbl->nobjs; i++)
 {
  if ( mattbl->objs[i].flags[0] && ( ! mattbl->objs[i].flags[1] ) )
  {
   gen_dim(mattbl->objs[i].obj_name,
           mattbl->objs[i].ref1,
           sd_in,
           sd_out,
           td1,
           td2,
           options);
     
  }
 }


 /* free table */
 match_dim_table_free(mattbl);
}


/*-------------------------------------------------------------------------
 * Function: gen_dim
 *
 * Purpose: generate "lone" dimensions. 
 *
 * Return: -1 error, 1 ok
 *
 * Programmer: Pedro Vicente Nunes, pvn@ncsa.uiuc.edu
 *
 * Date: May 10, 2006
 *
 *-------------------------------------------------------------------------
 */
static 
int gen_dim(char* name,              /* name of SDS */
            int32 ref,               /* ref of SDS */
            int32 sd_in,
            int32 sd_out,
            table_t *td1,
            table_t *td2,
            options_t *options)
{
 int32 sds_id,                /* data set identifier */
       sds_out,               /* data set identifier */
       dim_id,                /* dimension identifier */
       sds_index,             /* index number of the data set */
       dtype,                 /* SDS data type */
       dimsizes[MAX_VAR_DIMS],/* dimensions of SDS */
       start[MAX_VAR_DIMS],   /* read start */
       edges[MAX_VAR_DIMS],   /* read edges */
       nattrs,                /* number of SDS attributes */
       rank,                  /* rank of SDS */
       numtype,               /* number type */
       eltsz,                 /* element size */
       nelms;                 /* number of elements */
 char             sds_name[MAX_NC_NAME]; 
 VOIDP            buf=NULL;
 VOIDP            dim_buf=NULL;
 int              i, j, ret=1,stat;
 int              info;           /* temporary int compression information */
 comp_coder_t     comp_type;      /* compression type requested  */
 comp_coder_t     comp_type_in;   /* compression type original  */
 comp_info        c_info;         /* compression information requested  */
 comp_info        c_info_in;      /* compression information original  */
 HDF_CHUNK_DEF    chunk_def;      /* chunk definition */
 HDF_CHUNK_DEF    chunk_def_in;   /* chunk definition original */
 int32            chunk_flags;    /* chunk flags */ 
 int32            chunk_flags_in; /* chunk flags original*/ 
 int              szip_mode;      /* szip mode, EC, NN */
 intn             empty_sds;
 int              have_info=0;
 
 sds_index = SDreftoindex(sd_in,ref);
 sds_id    = SDselect(sd_in,sds_index);

 /*obtain name,rank,dimsizes,datatype and num of attributes of sds */
 if (SDgetinfo(sds_id,sds_name,&rank,dimsizes,&dtype,&nattrs)==FAIL){
  printf( "Could not get information for SDS\n");
  SDendaccess(sds_id);
  return -1;
 }

/*-------------------------------------------------------------------------
 * check if the input SDS is empty. if so , avoid some operations (mainly read, write)
 *-------------------------------------------------------------------------
 */ 
 if (SDcheckempty( sds_id, &empty_sds ) == FAIL) {
  printf( "Failed to check empty SDS <%s>\n", sds_name);
  ret=-1;
  goto out;
 }
 
/*-------------------------------------------------------------------------
 * get the original compression/chunk information from the object 
 *-------------------------------------------------------------------------
 */

 if (empty_sds==0 )
 {
  comp_type_in = COMP_CODE_NONE;  /* reset variables before retrieving information */
  HDmemset(&c_info_in, 0, sizeof(comp_info)) ;
  stat=SDgetcompress(sds_id, &comp_type_in, &c_info_in);
  if (stat==FAIL && comp_type_in>0){
   printf( "Could not get compression information for SDS <%s>\n",sds_name);
   SDendaccess(sds_id);
   return -1;
  }
  
  /* get chunk lengths */
  stat=SDgetchunkinfo(sds_id, &chunk_def_in, &chunk_flags_in);
  if (stat==FAIL){
   printf( "Could not get chunking information for SDS <%s>\n",sds_name);
   SDendaccess(sds_id);
   return -1;
  }
  
  /* retrieve the compress information if so */
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
#ifdef H4_HAVE_LIBSZ
    chunk_def_in.comp.comp_type              = COMP_CODE_SZIP;
    chunk_def_in.comp.cinfo.szip             = c_info_in.szip;
#else
    printf("Error: SZIP compression is not available <%s>\n",sds_name);
    SDendaccess(sds_id);
    return -1;
#endif
    break;
   default:
    printf("Error: Unrecognized compression code in %d <%s>\n",comp_type_in,sds_name);
   };
  }
  
  /*-------------------------------------------------------------------------
  * set the default values to the ones read from the object
  *-------------------------------------------------------------------------
  */
  
  comp_type   = comp_type_in;
  switch (comp_type_in)
  {
  case COMP_CODE_NBIT:
   printf("Nbit compression not supported in this version <%s>\n",sds_name);
   break;
  case COMP_CODE_NONE:
   break;
  case COMP_CODE_RLE:
   break;
  case COMP_CODE_SZIP:
#ifdef H4_HAVE_LIBSZ
   info      = c_info_in.szip.pixels_per_block;
   if (c_info_in.szip.options_mask & SZ_EC_OPTION_MASK) {
    szip_mode = EC_MODE;
   } else if (c_info_in.szip.options_mask & SZ_NN_OPTION_MASK) {
    szip_mode = NN_MODE;
   }
#else
   printf("SZIP compression not supported in this version <%s>\n",sds_name);
#endif
   break;
  case COMP_CODE_SKPHUFF:
   info  = c_info_in.skphuff.skp_size;
   break;
  case COMP_CODE_DEFLATE:
   info  = c_info_in.deflate.level;
   break;
  default:
   printf("Error: Unrecognized compression code in %d <%s>\n",comp_type,sds_name);
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
    chunk_def.comp.cinfo.deflate          = c_info_in.deflate;
    break;
   case COMP_CODE_SZIP:
#ifdef H4_HAVE_LIBSZ
    chunk_def.comp.comp_type              = COMP_CODE_SZIP;
    chunk_def.comp.cinfo.szip             = c_info_in.szip;
#else
    printf("Error: SZIP compression not available in %d <%s>\n",comp_type_in,sds_name);
#endif
    break;
   default:
    printf("Error: Unrecognized compression code in %d <%s>\n",comp_type_in,sds_name);
   };
  }

  
  /*-------------------------------------------------------------------------
  * get the compression/chunk information of this object from the table
  * translate to usable information
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
    &info,        /* compression information OUT */
    &szip_mode,   /* compression information OUT */
    &comp_type,   /* compression type OUT  */
    rank,         /* rank of object IN */
    sds_name,         /* path of object IN */
    1,            /* number of GR image planes (for SZIP), IN */
    dimsizes,     /* dimensions (for SZIP), IN */
    dtype         /* numeric type ( for SZIP), IN */
    );
   if (have_info==FAIL)
    comp_type=COMP_CODE_NONE;
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
   int maxchunk=INT_MAX;
   if ( (chunk_flags == HDF_CHUNK) || (chunk_flags == (HDF_CHUNK | HDF_COMP)) )
   {
    for (j = 0; j < rank; j++) {
     count   *= chunk_def.chunk_lengths[j];
    }
    nchunks=nelms/count;
    if (nchunks>maxchunk){
     printf("Warning: number of chunks is %d (greater than %d). Not chunking <%s>\n",
      nchunks,maxchunk,sds_name);
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
     options->threshold,sds_name);
   }
  }
  

 } /* empty_sds */

/*-------------------------------------------------------------------------
 * print the PATH, COMP and CHUNK information
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
   (chunk_flags>0)?"chunk":"",                    /*chunk information*/
   (pr_comp_type>0)?get_scomp(pr_comp_type):"",   /*compression information*/
   sds_name);                                         /*name*/
 }

/*-------------------------------------------------------------------------
 * check if the requested compression is valid
 * SDSs do not support JPEG
 *-------------------------------------------------------------------------
 */
 
 /* check inspection mode */
 if ( options->trip>0 && empty_sds==0) 
 {
  switch(comp_type)
  {
  case COMP_CODE_NONE:
  case COMP_CODE_RLE:
  case COMP_CODE_SKPHUFF:
  case COMP_CODE_DEFLATE:
  case COMP_CODE_SZIP:
  case COMP_CODE_NBIT:
   break;
  case COMP_CODE_JPEG:
   printf("Error: JPEG compression is not available for <%s>\n",sds_name);
   ret=FAIL;
   goto out;
   break;
  default:
   printf("Error: Unrecognized compression code %d in <%s>\n",comp_type_in,sds_name);
   ret=FAIL;
   goto out;
  }
 } /* check inspection mode */



/*-------------------------------------------------------------------------
 * create new SDS
 *-------------------------------------------------------------------------
 */

 if ((sds_out = SDcreate(sd_out,sds_name,dtype,rank,dimsizes)) == FAIL) {
  printf( "Failed to create new SDS <%s>\n", sds_name);
  ret=-1;
  goto out;
 }

/*-------------------------------------------------------------------------
 * make it a "dimension"
 *-------------------------------------------------------------------------
 */

 if ((dim_id = SDgetdimid(sds_out, 0)) == FAIL) {
  printf( "Failed to get dimension ID for SDS <%s>\n", sds_name);
  ret=-1;
  goto out;
 }

 if (SDsetdimname(dim_id, sds_name) == FAIL) {
  printf( "Failed to set dimension name for SDS <%s>\n", sds_name);
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

 if (empty_sds==0 )
 {
  
  /* set chunk */
  if ( (chunk_flags == HDF_CHUNK) || (chunk_flags == (HDF_CHUNK | HDF_COMP)) )
  {
   if (SDsetchunk (sds_out, chunk_def, chunk_flags)==FAIL)
   {
    printf( "Error: Failed to set chunk dimensions for <%s>\n", sds_name);
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
   if ( nelms*eltsz<options->threshold )
   {
    /* reset to the original values . we don't want to uncompress if it was */
    comp_type=COMP_CODE_NONE;
    if (options->verbose) {
     printf("Warning: object size smaller than %d bytes. Not compressing <%s>\n",
      options->threshold,sds_name);
    } 
   } else  {
    
    /* setup compression factors */
    switch(comp_type) 
    {
    case COMP_CODE_SZIP:
     if (set_szip (info,szip_mode,&c_info)==FAIL)
     {
      comp_type=COMP_CODE_NONE;
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
    case COMP_CODE_NBIT:
     comp_type = COMP_CODE_NONE;  /* not supported in this version */
     break;
    default:
     printf( "Error: Unrecognized compression code %d\n", comp_type);
    }
    
    if (SDsetcompress (sds_out, comp_type, &c_info)==FAIL)
    {
     printf( "Error: Failed to set compression for <%s>\n", sds_name);
     ret=-1;
     goto out;
    }
   }
  }
  


/*-------------------------------------------------------------------------
 * read sds and write new one
 *-------------------------------------------------------------------------
 */

  /* alloc */
  if ((buf = (VOIDP) HDmalloc(nelms * eltsz)) == NULL) {
   printf( "Failed to allocate %d elements of size %d\n", nelms, eltsz);
   ret=-1;
   goto out;
  }
  
  /* read data */
  if (SDreaddata (sds_id, start, NULL, edges, buf) == FAIL) {
   printf( "Could not read SDS <%s>\n", sds_name);
   ret=-1;
   goto out;
  }
  
  /* write the data */
  if (SDwritedata(sds_out, start, NULL, edges, buf) == FAIL) {
   printf( "Failed to write to new SDS <%s>\n", sds_name);
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
 


out:
 /* terminate access to the SDSs */
 if (SDendaccess(sds_id)== FAIL )
  printf( "Failed to close SDS <%s>\n", sds_name);
 if (sds_out!=FAIL) {
  if (SDendaccess (sds_out)== FAIL )
   printf( "Failed to close SDS <%s>\n", sds_name);
 }

 if (buf)
  free(buf);

 return ret;
 
}


