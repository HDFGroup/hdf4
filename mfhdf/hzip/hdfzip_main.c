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
Examples:
-v -i hziptst.hdf -o hziptst_out.hdf -c "*:2x2" -t "*:GZIP 6"

*/


int main(int argc, char **argv)
{
	char        *infile  = NULL;
 char        *outfile = NULL;
	options_t   options;            /*the global options */
	int         i, j, ii, jj, k;


 /* initialize options, -t table and -c table  */
	hzip_init (&options);

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


 exit(1);
}







#if 0

hzip -i input -o output [-h] [-v] [-t "comp_info"] [-c "chunk_info"]
  -i input          Input HDF File
  -o output         Output HDF File
  [-h]              Print usage message
  [-t "comp_info"]  Compression type: "comp_info" is a string with the format
                       "<list of objects> : <type of compression> <compression parameters>"
                       <list of objects> is a comma separated list of object names, 
                          meaning apply compression only to those objects. "*" means all objects
                       <type of compression> can be:
                         RLE, for RLE compression
                         HUFF, for Huffman
                         GZIP, for gzip
                         JPEG, for JPEG
                         SZIP, for szip
                         NONE, for no compression
                       <compression parameters> is optional compression info 
                         RLE, no parameter
                         HUFF, defaults to 1 skip-size
                         GZIP, defaults to deflation level 1
                         JPEG, 
                         SZIP, 
  [-c "chunk_info"] Apply chunking. "chunk_info" is a string with the format
                       "<list of objects> : <chunk information>"
                       <list of objects> is a comma separated list of object names, 
                          meaning apply chunking only to those objects. "*" means all objects
                       <chunk information> is the chunk size of each dimension:
                         <dim_1 x dim_2 x ... dim_n> or
                         NONE, for no chunking
  -f comp_file      File with compression info in it (instead of the two above options)

		$hzip -i file1.hdf -o file2.hdf -t "*:RLE"
compresses all objects in the file file1.hdf, using RLE compression

$hzip -i file1.hdf -o file2.hdf 
-t "A,B,C:HUFF 1" -t "D,E:RLE"
-c "D,E:10x10"
applies Skipping Huffman compression with skip factor of 1, for objects A, B and C. 
applies RLE compression for objects D and E.
applies chunking to objects D and E using a chunk size of 10 for the 2 dimensions.
$hzip -i file1.hdf -o file2.hdf -t "A:NONE"
applies no compression to object A. this can be used to uncompress the object, if it is compressed.


#endif


