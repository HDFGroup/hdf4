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


static void usage();

/*
Example:
-v -i hziptst.hdf -o hziptst_out.hdf -c "dataset:2x2" -t "dataset:GZIP 6"
-v -i hziptst.hdf -o hziptst_out.hdf -f info.txt
*/


int main(int argc, char **argv)
{
 char        *infile  = NULL;
 char        *outfile = NULL;
 options_t   options;            /*the global options */
 int         i;


 /* initialize options, -t table and -c table  */
 hzip_init (&options,0);

 for ( i = 1; i < argc; i++) 
 {
  if (strcmp(argv[i], "-i") == 0) {
   infile = argv[++i];
  }
  else if (strcmp(argv[i], "-o") == 0) {       
   outfile = argv[++i]; 
  }
  else if (strcmp(argv[i], "-v") == 0) {       
   options.verbose = 1;
  }
  else if (strcmp(argv[i], "-t") == 0) {  
   
   /* add the -t option */
   hzip_addcomp(argv[i+1],&options);

   /* jump to next */
   ++i;
  }
  else if (strcmp(argv[i], "-c") == 0) {       
   
   /* parse the -c option */
   hzip_addchunk(argv[i+1],&options);
   
   /* jump to next */
   ++i;
  }

  else if (strcmp(argv[i], "-m") == 0) {       
   
   options.threshold = parse_number(argv[i+1]);
   if (options.threshold==-1) {
    printf("Error: Invalid treshold size <%s>\n",argv[i+1]);
    exit(1);
   }
   ++i;
  }
  
  else if (strcmp(argv[i], "-f") == 0) {       
   read_info(argv[++i],&options);
  }
  
  else if (argv[i][0] == '-') {
   usage();
  }
 }

 if (infile == NULL || outfile == NULL) 
  usage();
 

/*-------------------------------------------------------------------------
 * print objects to compress/uncompress
 *-------------------------------------------------------------------------
 */

#if 0
 
 if (options.verbose)
 printf("Objects to compress are...\n");
 for ( i = 0; i < options.cp_tbl->nelems; i++) 
 {
  for ( j = 0; j < options.cp_tbl->objs[i].n_objs; j++) 
  {
   char* obj_name=options.cp_tbl->objs[i].obj_list[j].obj;
   if (options.verbose)
   printf("\t%s     \t %s compression, parameter %d\n",
    obj_name,
    get_scomp(options.cp_tbl->objs[i].comp.type),
    options.cp_tbl->objs[i].comp.info);
   
   /* check for duplicate names in input */
   for ( ii = 0; ii < options.cp_tbl->nelems; ii++) 
   {
    if (ii==i) continue;
    for ( jj = 0; jj < options.cp_tbl->objs[ii].n_objs; jj++) 
    {
     if (strcmp(obj_name,options.cp_tbl->objs[ii].obj_list[jj].obj)==0)
     {
      printf("\t%s     \t %s compression, parameter %d\n",
       obj_name,
       get_scomp(options.cp_tbl->objs[ii].comp.type),
       options.cp_tbl->objs[ii].comp.info);
      
      printf("Error: %s is already present in input. Exiting...\n",obj_name);
      comp_list_free(options.cp_tbl);
      exit(1);
     }
    }
    
   }
  }
 }
 

 if (options.compress==ALL && options.cp_tbl->nelems>1) 
 {
  printf("Error: '*' wildcard is present with other objects. Exiting...\n");
  comp_list_free(options.cp_tbl);
  exit(1);
 }


/*-------------------------------------------------------------------------
 * print objects to chunk
 *-------------------------------------------------------------------------
 */

 if (options.verbose)
 printf("Objects to chunk are...\n");
 for ( i = 0; i < options.ck_tbl->nelems; i++) 
 {
  for ( j = 0; j < options.ck_tbl->objs[i].n_objs; j++) 
  {
   char* obj_name=options.ck_tbl->objs[i].obj_list[j].obj;
   if (options.verbose) {
    printf("\t%s ",obj_name); 
    for ( k = 0; k < options.ck_tbl->objs[i].rank; k++) 
     printf("%d ",options.ck_tbl->objs[i].chunk_lengths[k]);
    printf("\n");
   }
   
   /* check for duplicate names in input */
   for ( ii = 0; ii < options.ck_tbl->nelems; ii++) 
   {
    if (ii==i) continue;
    for ( jj = 0; jj < options.ck_tbl->objs[ii].n_objs; jj++) 
    {
     if (strcmp(obj_name,options.ck_tbl->objs[ii].obj_list[jj].obj)==0)
     {
      printf("\t%s     \n",obj_name); 
      
      printf("Error: %s is already present in input. Exiting...\n",obj_name);
      chunk_list_free(options.ck_tbl);
      exit(1);
     }
    }
    
   }
  }
 }
 

 if (options.chunk==ALL && options.ck_tbl->nelems>1) 
 {
  printf("Error: '*' wildcard is present with other objects. Exiting...\n");
  chunk_list_free(options.ck_tbl);
  exit(1);
 }


#endif

 /* zip it */
 hzip(infile,outfile,&options);
 
 /* free tables */
 hzip_end(&options);
 return 0;
}


/*-------------------------------------------------------------------------
 * Function: usage
 *
 * Purpose: print usage
 *
 * Return: void
 *
 *-------------------------------------------------------------------------
 */

static 
void usage()
{

 printf(
  "hzip -i input -o output [-h] [-v] [-t 'comp_info'] [-c 'chunk_info']\n");
 printf("  -i input          Input HDF File\n");
 printf("  -o output         Output HDF File\n");
 printf("  [-h]              Print usage message\n");
 printf("  [-t 'comp_info']  Compression type: 'comp_info' is a string with the format\n");
 printf("\t\t     <object list>:<type of compression><parameters>\n");
 printf("\t\t     <object list> is a comma separated list of object names\n");
 printf("\t\t        meaning apply compression only to those objects\n");
 printf("\t\t        '*' means all objects\n");
 printf("\t\t     <type of compression> can be:\n");
 printf("\t\t       RLE, for RLE compression\n");
 printf("\t\t       HUFF, for Huffman\n");
 printf("\t\t       GZIP, for gzip\n");
 printf("\t\t       JPEG, for JPEG\n");
 printf("\t\t       SZIP, for szip\n");
 printf("\t\t       NONE, to uncompress\n");
 printf("\t\t     <parameters> is optional compression info\n");
 printf("\t\t       RLE, no parameter\n");
 printf("\t\t       HUFF, the skip-size\n");
 printf("\t\t       GZIP, the deflation level\n");
 printf("\t\t       JPEG, the quality factor\n");
 printf("\t\t       SZIP \n");
 printf("  [-c 'chunk_info'] Apply chunking. 'chunk_info' is a string with the format\n");
 printf("\t\t     <object list>:<chunk information>\n");
 printf("\t\t       <object list> is a comma separated list of object names\n");
 printf("\t\t         meaning apply chunking only to those objects\n");
 printf("\t\t         '*' means all objects\n");
 printf("\t\t       <chunk information> is the chunk size of each dimension:\n");
 printf("\t\t        <dim_1 x dim_2 x ... dim_n> or\n");
 printf("\t\t        NONE, to unchunk a previous chunked object\n");
 printf("  -f comp_file      File with compression info in it (instead of the two above options)\n");
 printf("\n");
 printf("Examples:\n");
 printf("\n");
 printf("1)$hzip -v -i file1.hdf -o file2.hdf -t '*:RLE'\n");
 printf("  compresses all objects in the file file1.hdf, using RLE compression\n");
 printf("\n");
 printf("2)$hzip -v -i file1.hdf -o file2.hdf -t 'A,B,C:HUFF 1' -t 'D,E:RLE' -c 'D,E:10x10'\n");
 printf("  applies Skipping Huffman compression with skip factor of 1, for objects A, B and C\n");
 printf("  applies RLE compression for objects D and E.\n");
 printf("  applies chunking to objects D and E using a chunk size of 10 for the 2 dimensions\n");
 printf("\n");
 printf("3)$hzip -v -i file1.hdf -o file2.hdf -t 'A:NONE'\n");
 printf("  uncompresses object A\n");
 printf("\n");
 printf("Note: the use of the verbose option -v is recommended\n");
 exit(1);
}


