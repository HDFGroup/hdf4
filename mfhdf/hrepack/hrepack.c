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
#include "hrepack_parse.h"
#include "hrepack_opttable.h"


void print_options(options_t *options);

/*-------------------------------------------------------------------------
 * Function: hrepack
 *
 * Purpose: locate all high-level HDF objects in the file 
 *  and compress/chunk them using options
 *
 * Algorythm: 2 traversals are made to the file; the 1st builds a list of
 *  the high-level objects, the 2nd makes a copy of them, using the options;
 *  the reason for the 1st traversal is to check for invalid object name requests
 *
 * Return: void
 *
 * Programmer: Pedro Vicente, pvn@ncsa.uiuc.edu
 *
 * Date: July 10, 2003
 *
 *-------------------------------------------------------------------------
 */
void hrepack(const char* infile, 
             const char* outfile, 
             options_t *options)
{
 options->trip=0;

 /* also checks input */
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
 * Function: hrepack_addcomp
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

void hrepack_addcomp(const char* str, options_t *options)
{
 
 obj_list_t      *obj_list=NULL; /*one object list for the -t and -c option entry */
 comp_info_t     comp;           /*compression info for the current -t option entry */
 int             n_objs;         /*number of objects in the current -t or -c option entry */
 int             i;

 if (options->all_comp==1){
  printf("Error: Invalid compression input: '*' is present with other objects <%s>\n",str);
  exit(1);
 }

 /* parse the -t option */
 obj_list=parse_comp(str,&n_objs,&comp);

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

}


/*-------------------------------------------------------------------------
 * Function: hrepack_addchunk
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


void hrepack_addchunk(const char* str, options_t *options)
{
 
 obj_list_t  *obj_list=NULL;     /*one object list for the -t and -c option entry */
 int         n_objs;             /*number of objects in the current -t or -c option entry */
 int32       chunk_lengths[MAX_VAR_DIMS]; /* chunk lengths along each dimension */
 int         chunk_rank;         /*global rank for chunks */
 int         i, j;

 if (options->all_chunk==1){
  printf("Error: Invalid chunking input: '*' is present with other objects <%s>\n",str);
  exit(1);
 }
 
 /* parse the -c option */
 obj_list=parse_chunk(str,&n_objs,chunk_lengths,&chunk_rank);

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

 free(obj_list);
  
}


/*-------------------------------------------------------------------------
 * Function: hrepack_init
 *
 * Purpose: initialize options
 *
 *-------------------------------------------------------------------------
 */

void hrepack_init (options_t *options, int verbose)
{
 memset(options,0,sizeof(options_t));
 options->threshold = 1024;
 options->verbose   = verbose;
 options_table_init(&(options->op_tbl));
}

/*-------------------------------------------------------------------------
 * Function: hrepack_end
 *
 * Purpose: free options table
 *
 *-------------------------------------------------------------------------
 */

void hrepack_end  (options_t *options)
{
 options_table_free(options->op_tbl);
}

/*-------------------------------------------------------------------------
 * Function: print_options
 *
 * Purpose: print options
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
 int   i, k, j, has_cp=0, has_ck=0;

/*-------------------------------------------------------------------------
 * objects to chunk
 *-------------------------------------------------------------------------
 */
 if (options->verbose) 
 {
  printf("Objects to chunk are...\n");
  if (options->all_chunk==1)  {
   printf("\tChunk all with dimension [");
   for ( j = 0; j < options->chunk_g.rank; j++)  
    printf("%d ", options->chunk_g.chunk_lengths[j]);
   printf("]\n");
  }
 }/* verbose */

 for ( i = 0; i < options->op_tbl->nelems; i++) 
 {
  char* obj_name=options->op_tbl->objs[i].path;
  
  if (options->op_tbl->objs[i].chunk.rank>0)
  {
   if (options->verbose){
    printf("\t%s ",obj_name); 
    for ( k = 0; k < options->op_tbl->objs[i].chunk.rank; k++) 
     printf("%d ",options->op_tbl->objs[i].chunk.chunk_lengths[k]);
    printf("\n");
   }
   has_ck=1;
  }
  else if (options->op_tbl->objs[i].chunk.rank==-2)
  {
   if (options->verbose)
    printf("\t%s %s\n",obj_name,"NONE"); 
   has_ck=1;
  }
 }
 
 if (options->all_chunk==1 && has_ck){
  printf("Error: Invalid chunking input: '*' is present with other objects\n");
  exit(1);
 }
 
/*-------------------------------------------------------------------------
 * objects to compress/uncompress
 *-------------------------------------------------------------------------
 */
 
 if (options->verbose) 
 {
  printf("Objects to compress are...\n");
  if (options->all_comp==1) 
  {
   switch (options->comp_g.type)
   {
   default:
    break;
   case COMP_CODE_RLE:
   case COMP_CODE_SZIP:
     printf("\tCompress all with %s compression\n",
      get_scomp(options->comp_g.type));
    break;
   case COMP_CODE_SKPHUFF:
   case COMP_CODE_DEFLATE:
   case COMP_CODE_JPEG:
     printf("\tCompress all with %s compression, parameter %d\n",
      get_scomp(options->comp_g.type),
      options->comp_g.info);
    break;
   };
  }
 } /* verbose */

 for ( i = 0; i < options->op_tbl->nelems; i++) 
 {
  pack_info_t obj=options->op_tbl->objs[i];
  if (obj.comp.type>0)
  {
   char* obj_name=obj.path;
   if (options->verbose) {
    printf("\t%s     \t %s compression, parameter %d\n",
     obj_name,
     get_scomp(obj.comp.type),
     obj.comp.info);
   }
   has_cp=1;
  }
 }
 
 if (options->all_comp==1 && has_cp){
  printf("Error: Invalid compression input: * is present with other objects\n");
  exit(1);
 }
}

/*-------------------------------------------------------------------------
 * Function: read_info
 *
 * Purpose: read comp and chunk options from file
 *
 * Return: void
 *
 * Programmer: Pedro Vicente, pvn@ncsa.uiuc.edu
 *
 * Date: July 30, 2003
 *
 *-------------------------------------------------------------------------
 */

void read_info(const char *filename,options_t *options) 
{
 char stype[10];
 char comp_info[1024];
 FILE *fp;
 char c;
 int  i, rc=1;

 
 if ((fp = fopen(filename, "r")) == (FILE *)NULL) {
  printf( "Cannot open options file %s", filename);
  exit(1);
 }

 /* Cycle until end of file reached */
 while( 1 )
 {
  rc=fscanf(fp, "%s", stype);
  if (rc==-1)
   break;
   
 /*-------------------------------------------------------------------------
  * comp
  *-------------------------------------------------------------------------
  */
  if (strcmp(stype,"-t") == 0) { 

   /* find begining of info */
   i=0; c='0';
   while( c!='"' )
   {
    fscanf(fp, "%c", &c);
   }
   c='0';
   /* go until end */
   while( c!='"' )
   {
    fscanf(fp, "%c", &c);
    comp_info[i]=c;
    i++;
   }
   comp_info[i-1]='\0'; /*cut the last " */    

   hrepack_addcomp(comp_info,options);
  }
 /*-------------------------------------------------------------------------
  * chunk
  *-------------------------------------------------------------------------
  */
  else if (strcmp(stype,"-c") == 0) { 
   
   /* find begining of info */
   i=0; c='0';
   while( c!='"' )
   {
    fscanf(fp, "%c", &c);
   }
   c='0';
   /* go until end */
   while( c!='"' )
   {
    fscanf(fp, "%c", &c);
    comp_info[i]=c;
    i++;
   }
   comp_info[i-1]='\0'; /*cut the last " */    

   hrepack_addchunk(comp_info,options);
  }
 /*-------------------------------------------------------------------------
  * not valid
  *-------------------------------------------------------------------------
  */
  else {
   printf( "Bad file format for %s", filename);
   exit(1);
  }
 }

 fclose(fp);
 return;
}

