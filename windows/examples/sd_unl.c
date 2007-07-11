/******************************************************************/
/*                                                                */
/*  sd_unl.c                                                      */
/*    This program creates an hdf file called "sdunl.hdf"         */
/*    and makes use of the unlimited dimension feature in         */
/*    HDF.                                                        */
/*                                                                */
/******************************************************************/

#include "mfhdf.h"

#define DIM0 10
#define DIM1 5

main( ) 
{
	int32 sd_id, sds_id, istat, sds_idx;
	int32 dims[2], start[2], edges[2], rank;
	int16 array_data[DIM0][DIM1], append_data[DIM1];
	intn i, j;

	/* Create and open the file and initiate the SD interface. */
	sd_id = SDstart("sdunl.hdf", DFACC_CREATE);
	if (sd_id != FAIL)
		printf ("\n... sdunl.hdf created\n");

	/* Define the rank and dimensions of the data set to be created. */
	rank = 2;
	dims[0] = SD_UNLIMITED;
	dims[1] = DIM1;

	/* Create the array data set. */
	sds_id = SDcreate(sd_id, "data1", DFNT_INT16, rank, dims);
	if (sds_id != FAIL)
		printf ("... data1 created\n");

	/* Fill the stored-data array with values. */
	for (j = 0; j < DIM0; j++) {
		for (i = 0; i < DIM1; i++)
			array_data[j][i] = (i + j) + 1;
 	}

	/* Define the location, pattern, and size of the data set */
	for (i = 0; i < rank; i++) {
		start[i] = 0;
	}

	edges[0] = DIM0;
	edges[1] = DIM1;

	istat = SDwritedata(sds_id, start, NULL, edges, (VOIDP)array_data); 
	if (istat != FAIL)
		printf ("... data written out\n");

	/* Terminate access to the array. */
	istat = SDendaccess(sds_id);

	/* Terminate access to the SD interface and close the file. */
	istat = SDend(sd_id);
	if (istat != FAIL)
		printf("... file closed\n");

	sd_id = SDstart("sdunl.hdf", DFACC_WRITE);
	if (sd_id != FAIL)
		printf ("... sdunl.hdf opened in Read/Write mode\n");

	sds_idx = SDnametoindex (sd_id, "data1");

	sds_id = SDselect (sd_id, sds_idx);
	if (sds_id != FAIL)
		printf ("... data1 selected\n");

	/* Store array values to be appended */
	for (i = 0; i <DIM1; i++)
		append_data[i] = i + 1;

	/* Define the location of the append */
	start[0] = DIM0;
	start[1] = 0;
	edges[0] = 1;
	edges[1] = DIM1;

	/* Append data to file */
	istat = SDwritedata (sds_id, start, NULL, edges, (VOIDP) append_data);
	if (istat != FAIL)
		printf ("... data appended\n");

	/* Terminate access to the array. */
	istat = SDendaccess (sds_id);

	/* Terminate access to the SD interface and close the file. */
	istat = SDend (sd_id);
	if (istat != FAIL)
		printf("... file closed\n\n");
}
