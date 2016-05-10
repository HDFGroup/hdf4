/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Copyright by The HDF Group.                                               *
 * Copyright by the Board of Trustees of the University of Illinois.         *
 * All rights reserved.                                                      *
 *                                                                           *
 * This file is part of HDF.  The full HDF copyright notice, including       *
 * terms governing use, modification, and redistribution, is contained in    *
 * the files COPYING and Copyright.html.  COPYING can be found at the root   *
 * of the source code distribution tree; Copyright.html can be found at      *
 * http://hdfgroup.org/products/hdf4/doc/Copyright.html.  If you do not have *
 * access to either file, you may request a copy from help@hdfgroup.org.     *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <hdf.h>
#include "gif.h"
#include <string.h>
int
main(argv , argc)
int argv;
char *argc[];
{
	
	GIFTOMEM GifMemoryStruct;
	GIFIMAGEDESC gifImageDesc;

	FILE *fpGif;
	int32 i,ImageCount;
	int32 filesize;
	BYTE *MemGif;
	BYTE *StartPos;
	char GIFFileName[256];
	char HDFFileName[256];

	/* Initialize all GifMemoryStruct pointers to null
	** to prevent hassles later on
	*/
	GifMemoryStruct.GifHeader                  = NULL;
	GifMemoryStruct.GifImageDesc               = NULL;
	GifMemoryStruct.GifGraphicControlExtension = NULL;
	GifMemoryStruct.GifPlainTextExtension      = NULL;
	GifMemoryStruct.GifApplicationExtension    = NULL;
	GifMemoryStruct.GifCommentExtension        = NULL;
	
	
	if (argv<3)
	{
		printf("\n\nWrong Usage. Use:\ngif2hdf <GIFFILE> <HDFFILE>\n\n");
		return(-1);
	}
	
	

    HDstrncpy(GIFFileName , argc[1] , VSNAMELENMAX - 1);
    HDstrncpy(HDFFileName , argc[2] , VSNAMELENMAX - 1);
	GIFFileName[VSNAMELENMAX - 1] = '\0';
	HDFFileName[VSNAMELENMAX - 1] = '\0';

	if (!(fpGif = fopen(GIFFileName,"rb"))) {
		printf("Unable to open GIF file for reading.\n");
		printf("Filename (including path) must be less than %d charachters in length\n",VSNAMELENMAX);
		exit(-1);
	}

	/* Get the whole file into memory. Mem's much faster than I/O */
	fseek(fpGif, 0L , 2);
	filesize = ftell(fpGif);
	fseek(fpGif, 0L , 0);
	if (filesize == 0) printf("File Size Zero");
	if (!(MemGif = StartPos = (BYTE *)malloc(filesize))) {
		printf("Out of memory");
		exit (-1);
	}
	if (fread(MemGif,filesize,1,fpGif) != 1) {
		printf("Corrupted Input File");
		exit(-1);
	}

	fseek(fpGif,0L,0);

	/*
	** Call Gif2Mem and break the whole file into parts.
	** Gif2Mem also calls decompresses the images so we don't 
	** have to worry about that
	*/
	GifMemoryStruct = Gif2Mem(MemGif);
	if (ferror(fpGif)) {
		printf("File Stream Error\n\n");
		exit(-1);
	}
	fclose(fpGif);
	
	/* Call WriteHDF from here. Go ahead and change WriteHDF to write 
	** whatever format you want
	*/
	if (WriteHDF(GifMemoryStruct , argc[2] , argc[1]))
		printf("HDF Write Error\n\n");
	
	/* Free all buffers */
	ImageCount = (int32)(GifMemoryStruct.GifHeader)->ImageCount;

	for(i = 0 ; i < ImageCount ; i++)
	{
		gifImageDesc = *(GifMemoryStruct.GifImageDesc[i]);
		if (gifImageDesc.Image != NULL)
            HDfree(gifImageDesc.Image);
	
		if (GifMemoryStruct.GifGraphicControlExtension[i] != NULL)
            HDfree(GifMemoryStruct.GifGraphicControlExtension[i]);
		
	}
    HDfree(StartPos);

    HDfree (GifMemoryStruct.GifHeader);
	
	if (GifMemoryStruct.GifApplicationExtension != NULL)
        HDfree (GifMemoryStruct.GifApplicationExtension);

	return (0);
}
