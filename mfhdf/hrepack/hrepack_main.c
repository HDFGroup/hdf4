/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Copyright by The HDF Group.                                               *
 * Copyright by the Board of Trustees of the University of Illinois.         *
 * All rights reserved.                                                      *
 *                                                                           *
 * This file is part of HDF.  The full HDF copyright notice, including       *
 * terms governing use, modification, and redistribution, is contained in    *
 * the COPYING file, which can be found at the root of the source code       *
 * distribution tree, or in https://support.hdfgroup.org/ftp/HDF/releases/.  *
 * If you do not have access to either file, you may request a copy from     *
 * help@hdfgroup.org.                                                        *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "hdf.h"
#include "hfile.h"
#include "mfhdf.h"
#include "hrepack.h"
#include "hrepack_parse.h"
#include "hrepack_opttable.h"

static void usage(void);

/*-------------------------------------------------------------------------
 * Function: main
 *
 * Purpose: hrepack main program
 *
 * Programmer: Pedro Vicente Nunes, pvn@ncsa.uiuc.edu
 *
 * Date: July 10, 2003
 *
 *-------------------------------------------------------------------------
 */


int main(int argc, char **argv)
{
    char        *infile  = NULL;
    char        *outfile = NULL;
    options_t   options;            /*the global options */
    int         i;
    int         ret;
    
    
    /* initialize options  */
    hrepack_init (&options,0);
    
    for ( i = 1; i < argc; i++) 
    {
        if (strcmp(argv[i], "-i") == 0) {
            infile = argv[++i];
        }
        else if (strcmp(argv[i], "-o") == 0) {       
            outfile = argv[++i]; 
        }
        else if (strcmp(argv[i], "-h") == 0) {       
	    goto out;
        }
        else if (strcmp(argv[i], "-v") == 0) {       
            options.verbose = 1;
        }
        else if (strcmp(argv[i], "-V") == 0) {       
            printf("%s, %s\n\n", argv[0], LIBVER_STRING );
            exit(0);
        }
        else if (strcmp(argv[i], "-t") == 0) {  
            
            /* add the -t option */
            if (hrepack_addcomp(argv[i+1],&options)<0)
                goto out;
            
            /* jump to next */
            ++i;
        }
        else if (strcmp(argv[i], "-c") == 0) {       
            
            /* parse the -c option */
            if (hrepack_addchunk(argv[i+1],&options)<0)
                goto out;
            
            /* jump to next */
            ++i;
        }
        
        else if (strcmp(argv[i], "-m") == 0) {       
            
            options.threshold = parse_number(argv[i+1]);
            if (options.threshold==-1) {
                printf("Error: Invalid treshold size <%s>\n",argv[i+1]);
                goto out;
            }
            ++i;
        }
        
        else if (strcmp(argv[i], "-f") == 0) {       
            if (read_info(argv[++i],&options)<0)
                goto out;
        }
        
        else if (argv[i][0] == '-') {
            goto out;
        }
    }
    
    if (infile == NULL || outfile == NULL) 
        goto out;
    
    /* zip it */
    ret=hrepack_main(infile,outfile,&options);
    
    /* free tables */
    hrepack_end(&options);
    
    /* unix error return code */
    if (ret==-1)
        return 1;
    else
        return 0;
    
out:

    usage();
    
    /* free tables */
    hrepack_end(&options);
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
void usage(void)
{

 printf("usage: hrepack -i input -o output [-V] [-h] [-v] [-t 'comp_info'] [-c 'chunk_info'] [-f cfile] [-m size]\n");
 printf("  -i input          input HDF File\n");
 printf("  -o output         output HDF File\n");
 printf("  [-V]              prints version of the HDF4 library and exits\n");
 printf("  [-h]              prints this usage\n");
 printf("  [-v]              verbose mode\n");
 printf("  [-t 'comp_info']  compression type: 'comp_info' is a string with the format\n");
 printf("\t\t     <object list>:<type of compression><parameters>\n");
 printf("\t\t     <object list> is a comma separated list of object names\n");
 printf("\t\t        meaning apply compression only to those objects\n");
 printf("\t\t        '*' means all objects\n");
 printf("\t\t     <type of compression> can be:\n");
 printf("\t\t       RLE, for RLE compression\n");
 printf("\t\t       HUFF, for Huffman\n");
 printf("\t\t       GZIP, for gzip\n");
 printf("\t\t       JPEG, for JPEG (for images only)\n");
 printf("\t\t       SZIP, for szip\n");
 printf("\t\t       NONE, to uncompress\n");
 printf("\t\t     <parameters> is optional compression info\n");
 printf("\t\t       RLE, no parameter\n");
 printf("\t\t       HUFF, the skip-size\n");
 printf("\t\t       GZIP, the deflation level\n");
 printf("\t\t       JPEG, the quality factor\n");
 printf("\t\t       SZIP, pixels per block, compression mode (NN or EC)\n");
 printf("  [-c 'chunk_info'] apply chunking. 'chunk_info' is a string with the format\n");
 printf("\t\t     <object list>:<chunk information>\n");
 printf("\t\t       <object list> is a comma separated list of object names\n");
 printf("\t\t         meaning apply chunking only to those objects\n");
 printf("\t\t         '*' means all objects\n");
 printf("\t\t       <chunk information> is the chunk size of each dimension:\n");
 printf("\t\t        <dim_1 x dim_2 x ... dim_n> or\n");
 printf("\t\t        NONE, to unchunk a previous chunked object\n");
 printf("  [-f cfile]      file with compression information -t and -c\n");
 printf("  [-m size]       do not compress objects smaller than size (bytes)\n");
 printf("\n");
 printf("Examples:\n");
 printf("\n");
 printf("1) hrepack -v -i file1.hdf -o file2.hdf -t '*:RLE'\n");
 printf("   compresses all objects in the file file1.hdf, using RLE compression\n");
 printf("\n");
 printf("2) hrepack -v -i file1.hdf -o file2.hdf -t '/group1/A,/group2/B,C:HUFF 1'\n");
 printf("   applies Skipping Huffman compression with skip factor of 1,\n");
 printf("   for objects /group1/A, /group2/B and C\n");
 printf("\n");
 printf("2) hrepack -v -i file1.hdf -o file2.hdf -t '/group1/D:RLE' -c 'D,E:10x10'\n");
 printf("   applies RLE compression for object /group1/D\n");
 printf("   applies chunking to objects D and E using a chunk size of 10 for the 2 dimensions\n");
 printf("\n");
 printf("3) hrepack -v -i file1.hdf -o file2.hdf -t 'A:NONE'\n");
 printf("   uncompresses object A\n");
 printf("\n");
 printf("4) hrepack -v -i file1.hdf -o file2.hdf -t 'A:SZIP 8,NN'\n");
 printf("   applies SZIP compression to object A, with parameters 8 and NN\n");
 printf("\n");
 printf("Note: the use of the verbose option -v is recommended\n");
}

