/*******************************************************************/
/*                                                                 */
/*  sd_create.c                                                    */
/*        This program creates an HDF file, sd.hdf, which          */
/*        contains an SDS of size 10 x 5.                          */
/*                                                                 */
/*******************************************************************/

#include "mfhdf.h"

#define DIM0 10
#define DIM1 5

main( ) 
{
	int32 sd_id, sds_id, istat;
	int32 dims[2], start[2], edges[2], rank;
	int16 array_data[DIM0][DIM1];
	intn i, j;

	/* Create and open the file and initiate the SD interface. */
	sd_id = SDstart("sd.hdf", DFACC_CREATE);
	if (sd_id == FAIL) {
		printf ("SDstart failed.\n");
		HEprint(stdout,0);
		exit (-1);
	}
	else
		printf ("\n... sd.hdf created\n");

	/* Define the rank and dimensions of the data set to be created. */
	rank = 2;
	dims[0] = DIM0;
	dims[1] = DIM1;

	/* Create the array data set. */
	sds_id = SDcreate(sd_id, "data1", DFNT_INT16, rank, dims);
	if (sds_id == FAIL) {
		printf ("SDcreate failed.\n");
		HEprint(stdout,0);
		exit (-1);
	}	
	else
		printf ("... data1 created\n");

	/* Fill the array with values. */
	for (j = 0; j < DIM0; j++) {
		for (i = 0; i < DIM1; i++)
			array_data[j][i] = (i + j) + 1;
 	}

	/* Define the location, pattern, and size of the data set */
	for (i = 0; i < rank; i++) {
		start[i] = 0;
		edges[i] = dims[i];
	}

	istat = SDwritedata(sds_id, start, NULL, edges, (VOIDP)array_data); 
	if (istat == FAIL) {
        printf ("SDwritedata failed.\n");
		HEprint(stdout,0);
		exit (-1);
	}	
	else
		printf ("... data written out\n");

	/* Terminate access to the array. */
	istat = SDendaccess(sds_id);
	if (istat == FAIL) {
        printf ("SDendaccess failed.\n");
		HEprint(stdout,0);
		exit (-1);
	}	

	/* Terminate access to the SD interface and close the file. */
	istat = SDend(sd_id);
	if (istat == FAIL) {
        printf ("SDend failed.\n");
		HEprint(stdout,0);
		exit (-1);
	}	
	else
		printf("... file closed\n\n");
}


