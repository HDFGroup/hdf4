/*******************************************************************/
/*                                                                 */
/*  sd_rd.c                                                        */
/*        This program creates an HDF file, sd.hdf, which          */
/*        contains an SDS of size 10 x 5.  It then re-opens        */
/*        the HDF file in Read Only mode, reads the data back      */
/*        in, and prints it to the screen.                         */
/*                                                                 */
/*******************************************************************/

#include "hdf.h"

#define DIM1 5
#define DIM0 10

main( ) 
{
	int32 sd_id, sds_id, istat, sd_index;
	int32 dims[2], start[2], edges[2], rank;
	int16 array_data[DIM0][DIM1];
	int16 new_array[DIM0][DIM1]; 
	intn i, j;

	/* Create and open the file and initiate the SD interface. */
	sd_id = SDstart("sd.hdf", DFACC_CREATE);
	if (sd_id != FAIL)
		printf ("\n... sd.hdf created\n");

	/* Define the rank and dimensions of the data set to be created. */
	rank = 2;
	dims[0] = DIM0;
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
		edges[i] = dims[i];
	}

	istat = SDwritedata(sds_id, start, NULL, edges, (VOIDP)array_data); 
	if (istat != FAIL)
		printf ("... data written out\n");


	/* Terminate access to the array. */
	istat = SDendaccess(sds_id);

	/* Terminate access to the SD interface and close the file. */
	istat = SDend(sd_id);
	if (istat != FAIL)
		printf("... file closed\n");

	/* Read data back and write to a new array */

	sd_id = SDstart("sd.hdf", DFACC_READ);
	if (sd_id != FAIL)
		printf ("... sd.hdf opened with READ access\n");

	sd_index = 0;
	sds_id = SDselect (sd_id, sd_index); 

	istat = SDreaddata (sds_id, start, NULL, edges, (VOIDP) new_array);

	printf ("\nData: \n");
	for (j = 0; j < DIM0; j++) 
	{
		for (i = 0; i < DIM1; i++)
			printf ("  %i ", new_array[j][i]);
		printf ("\n");
	}
	printf ("\n");

	/* Terminate access to the array. */
	istat = SDendaccess(sds_id);

	/* Terminate access to the SD interface and close the file. */
	istat = SDend(sd_id);
	if (istat != FAIL)
		printf("... file closed\n\n");

}


