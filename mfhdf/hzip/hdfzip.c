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
#include "hzip.h"
#include "parse.h"

void print_options(options_t *options);


void hzip_init (options_t *options)
{
 memset(options,0,sizeof(options_t));
 
#if defined (ONE_TABLE)
 options_table_init(&(options->op_tbl));

#else

 comp_list_init(&(options->cp_tbl));
 chunk_list_init(&(options->ck_tbl));

#endif
}

void hzip_end  (options_t *options)
{
 
#if defined (ONE_TABLE)
 options_table_free(options->op_tbl);

#else

 comp_list_free(options->cp_tbl);
 chunk_list_free(options->ck_tbl);

#endif
}


/*-------------------------------------------------------------------------
 * Function: hzip
 *
 * Purpose: locate all high-level HDF objects in the file 
 *  and compress/chunk them using options
 *
 * Return: void
 *
 * Programmer: Pedro Vicente, pvn@ncsa.uiuc.edu
 *
 * Date: July 10, 2003
 *
 *-------------------------------------------------------------------------
 */
void hzip(char* infile, char* outfile, options_t *options)
{
 options->trip=0;

 if (options->verbose)
  print_options(options);

 /* first check for objects in input that are in the file */
 if (list(infile,outfile,options)==0)
 {
  /* the real deal now */
  options->trip=1;
  
  if (options->verbose)
   printf("Making new file %s...\n",outfile);
  
  list(infile,outfile,options);
 }
}


/*-------------------------------------------------------------------------
 * Function: hzip_addcomp
 *
 * Purpose: add a compression -t option to table 
 *   Example: -t "*:GZIP 6" , STR = "*:GZIP 6"
 *
 * Return: void
 *
 * Programmer: Pedro Vicente, pvn@ncsa.uiuc.edu
 *
 * Date: July 23, 2003
 *
 *-------------------------------------------------------------------------
 */

void hzip_addcomp(char* str, options_t *options)
{
 
 obj_list_t      *obj_list=NULL;     /*one object list for the -t and -c option entry */
 comp_info_t     comp;               /*compression info for the current -t option entry */
 int             n_objs;             /*number of objects in the current -t or -c option entry */
 int             ret, i;


 /* parse the -t option */
 obj_list=parse_comp(str,&n_objs,&comp);


#if !defined (ONE_TABLE)
 
 /* 
 add it to compress options table 
 return compress behaviour; 0 for no(default), 1 for selectd, 2 for all 
 */
 ret=comp_list_add(obj_list,n_objs,comp,options->cp_tbl);
 if (ret>options->compress)
  options->compress=ret;
 
 /* if we are compressing all set the global compression type */
 if (options->compress==ALL)
  options->comp=comp;


#else

  /* searh for the "*" all objects character */
 for (i = 0; i < n_objs; i++) 
 {
  if (strcmp("*",obj_list[i].obj)==0)
  {
   /* if we are compressing all set the global comp type */
   options->all_comp=1;
   options->comp_g=comp;
  }
 }

 if (i>1)
 {
  printf("\nError: '*' cannot be with other objects, <%s>. Exiting...\n",str);
  free(obj_list);
  options_table_free(options->op_tbl);
  exit(1);
 }

 if (options->all_comp==0)
  options_add_comp(obj_list,n_objs,comp,options->op_tbl);

#endif


 
}


/*-------------------------------------------------------------------------
 * Function: hzip_addchunk
 *
 * Purpose: add a chunk -c option to table 
 *   Example: -c "*:2x2" , STR = "*:2x2"
 *
 * Return: void
 *
 * Programmer: Pedro Vicente, pvn@ncsa.uiuc.edu
 *
 * Date: July 23, 2003
 *
 *-------------------------------------------------------------------------
 */


void hzip_addchunk(char* str, options_t *options)
{
 
 obj_list_t      *obj_list=NULL;     /*one object list for the -t and -c option entry */
 int             n_objs;             /*number of objects in the current -t or -c option entry */
 int32           chunk_lengths[MAX_VAR_DIMS]; /* chunk lengths along each dimension */
 int             chunk_rank;         /*global rank for chunks */
 int             ret, i, j;

 
 /* parse the -c option */
 obj_list=parse_chunk(str,&n_objs,chunk_lengths,&chunk_rank);

#if !defined (ONE_TABLE)
 
 /* 
 add it to chunk options table 
 return chunk behaviour; 0 for no(default), 1 for selectd, 2 for all 
 */
 ret=chunk_list_add(obj_list,n_objs,chunk_lengths,chunk_rank,options->ck_tbl);
 if (ret>options->chunk)
  options->chunk=ret;
 
 /* if we are chunking all set the global chunking type */
 if (options->chunk==ALL)
 {
  options->chunk_rank=chunk_rank;
  options->chunk_flags=HDF_CHUNK;   /* set at least for chunk; it can further be comp */
  for (j = 0; j < chunk_rank; j++) 
   options->chunk_def.chunk_lengths[j] = chunk_lengths[j];
 }

#else

  /* searh for the "*" all objects character */
 for (i = 0; i < n_objs; i++) 
 {
  if (strcmp("*",obj_list[i].obj)==0)
  {
   /* if we are chunking all set the global chunking type */
   options->all_chunk=1;
   options->chunk_g.rank=chunk_rank;
   for (j = 0; j < chunk_rank; j++) 
    options->chunk_g.chunk_lengths[j] = chunk_lengths[j];
  }
 }

 if (i>1)
 {
  printf("\nError: '*' cannot be with other objects, <%s>. Exiting...\n",str);
  free(obj_list);
  options_table_free(options->op_tbl);
  exit(1);
 }

 if (options->all_chunk==0)
  options_add_chunk(obj_list,n_objs,chunk_lengths,chunk_rank,options->op_tbl);

#endif

 free(obj_list);
  
}


/*-------------------------------------------------------------------------
 * Function: print_options
 *
 * Purpose: 
 *
 * Return: void
 *
 * Programmer: Pedro Vicente, pvn@ncsa.uiuc.edu
 *
 * Date: July 25, 2003
 *
 *-------------------------------------------------------------------------
 */
void print_options(options_t *options)
{
 int   i, k;
 char* path;
 int   comp_type, rank;

 
 
#if 1
 
/*-------------------------------------------------------------------------
 * objects to chunk
 *-------------------------------------------------------------------------
 */
 
 printf("Objects to chunk are...\n");
 if (options->all_chunk==1)
  printf("\tChunk all\n");
 for ( i = 0; i < options->op_tbl->nelems; i++) 
 {
  char* obj_name=options->op_tbl->objs[i].path;

  if (options->op_tbl->objs[i].chunk.rank>0)
  {
   printf("\t%s ",obj_name); 
   for ( k = 0; k < options->op_tbl->objs[i].chunk.rank; k++) 
    printf("%d ",options->op_tbl->objs[i].chunk.chunk_lengths[k]);
   printf("\n");
  }
  else if (options->op_tbl->objs[i].chunk.rank==-2)
  {
   printf("\t%s %s\n",obj_name,"NONE"); 
  }
 }
 

/*-------------------------------------------------------------------------
 * objects to compress/uncompress
 *-------------------------------------------------------------------------
 */
 
 printf("Objects to compress are...\n");
 if (options->all_comp==1)
  printf("\tCompress all\n");
 for ( i = 0; i < options->op_tbl->nelems; i++) 
 {
  if (options->op_tbl->objs[i].comp.type>=0)
  {
   char* obj_name=options->op_tbl->objs[i].path;
   printf("\t%s     \t %s compression, parameter %d\n",
    obj_name,
    get_scomp(options->op_tbl->objs[i].comp.type),
    options->op_tbl->objs[i].comp.info);
   
  }
 }
 
#else

 printf("Objects to modify are...\n");
 if (options->all_chunk==1)
  printf("   Chunk all\n");
 if (options->all_comp==1)
  printf("   Compress all\n");
 for ( i = 0; i < options->op_tbl->nelems; i++) 
 {
  path      = options->op_tbl->objs[i].path;
  comp_type = options->op_tbl->objs[i].comp.type;
  rank      = options->op_tbl->objs[i].chunk.rank;
  
  printf(PFORMAT,
   (rank>0)?"chunk":"",                    /*chunk info*/
   (comp_type>0)?get_scomp(comp_type):"",  /*compression info*/
   path);                                  /*name*/

 }




#endif
 
}
