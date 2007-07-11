#include "hdf.h"

#define LENGTH0 15
#define LENGTH1 10

main( ) 
{
	int32 sd_id, sds_id, status, dimid, sds_idx;
	int32 dims[2], start[2], edges[2], rank;
	int16 array_data[LENGTH0][LENGTH1];
	int32 dim_sizes[MAX_VAR_DIMS];
	intn i, j;
	int32 array_rank, num_type, attributes;
	char name[MAX_NC_NAME];
	int16 scale0 [] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14};
	int16 scale1 [] = {0,1,2,3,4,5,6,7,8,9};

	printf ("\nCreate a new HDF file called dim.hdf...\n");
	/* Create and open the file and initiate the SD interface. */
	sd_id = SDstart("dim.hdf", DFACC_CREATE);
	printf (" SDstart returns sd_id: %i\n", sd_id);

	/* Define the rank and dimensions of the data set to be created. */
	rank = 2;
	dims[0] = LENGTH0;
	dims[1] = LENGTH1;

	printf ("Add a data array called 'HDF Data' to it...\n");
	/* Create the array data set. */
	sds_id = SDcreate(sd_id, "HDF Data", DFNT_INT16, rank, dims);
	printf (" SDcreate returns sds_id: %i\n", sds_id);

	/* Fill the stored-data array with values. */
	for (j = 0; j < LENGTH0; j++) {
		for (i = 0; i < LENGTH1; i++)
			array_data[j][i] = (i + j) + 1;
	}

	/* Define the location, pattern, and size of the data set */
	for (i = 0; i < rank; i++) {
		start[i] = 0;
		edges[i] = dims[i];
	}

	printf ("Set the dimension names to 'Dimension 0' and 'Dimension 1' ...\n");
	dimid = SDgetdimid (sds_id, 0);
	printf (" SDgetdimid returns Dimension 0 ID: %i\n", dimid);

	status = SDsetdimname (dimid, " Dimension 0");
	printf (" SDsetdimname returns: %i\n", status);

	dimid = SDgetdimid (sds_id, 1);
	printf (" SDgetdimid returns Dimension 1 ID: %i\n", dimid);

	status = SDsetdimname (dimid, " Dimension 1");
	printf (" SDsetdimname returns: %i\n", status);

	printf ("Write data to the data array 'HDF Data' ...\n");
	/* Write the stored data to the "HDF Data" data set. The fifth  \
	argument must be explicitly cast to a generic pointer to conform \
	to the HDF API definition for SDwritedata. */
	status = SDwritedata(sds_id, start, NULL, edges, (VOIDP)array_data); 
	printf (" SDwritedata returns: %i\n", status);

	printf ("Close array and dim.hdf file ...\n");
	/* Terminate access to the array. */
	status = SDendaccess(sds_id);
	printf (" SDendaccess returns: %i\n", status);

	/* Terminate access to the SD interface and close the file. */
	status = SDend(sd_id);
	printf (" SDend returns: %i\n", status);

	printf ("Re-open dim.hdf in RW mode and select array 'HDF Data' ...\n");
	/* Open the file and initiate the SD interface. */
	sd_id = SDstart("dim.hdf", DFACC_RDWR);
	printf (" SDstart returns sd_id: %i\n", sd_id);
	sds_idx = SDnametoindex (sd_id, "HDF Data");
	printf (" SDnametoindex returns 'HDF Data' array index: %i\n", sds_idx);
	sds_id = SDselect (sd_id, sds_idx);
	printf (" SDselect returns sds_id: %i\n", sds_id);

	printf ("Set the dimension scales ...\n");
	status = SDgetinfo(sds_id, name, &array_rank, dim_sizes,  \
					&num_type, &attributes);
	printf (" SDgetinfo returns: %i\n", status);

	printf (" Rank: %i\n", array_rank);
	printf (" Dimension 0 Size: %i\n", dim_sizes[0]);
	printf (" Dimension 1 Size: %i\n", dim_sizes[1]);

	dimid = SDgetdimid (sds_id, 0);
	printf (" SDgetdimid returns Dimension 0 ID: %i\n", dimid);
    
	status = SDsetdimscale (dimid, dim_sizes[0], num_type, scale0);
	printf (" SDsetdimscale returns: %i\n", status);
 
	dimid = SDgetdimid (sds_id, 1);
	printf (" SDgetdimid returns Dimension 1 ID: %i\n", dimid);
    
	status = SDsetdimscale (dimid, dim_sizes[1], num_type, scale1);
	printf (" SDsetdimscale returns: %i\n", status);
 
	printf ("Close data array and dim.hdf ... \n");
	status = SDendaccess(sds_id);
	printf (" SDendaccess returns: %i\n", status);

	/* Terminate access to the SD interface and close the file. */
	status = SDend(sd_id);
	printf (" SDend returns: %i\n", status);

}


