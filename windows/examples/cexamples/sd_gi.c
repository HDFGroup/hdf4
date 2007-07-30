/*******************************************************************/
/*                                                                 */
/*  sd_gi.c                                                        */
/*        This program opens the HDF file, sd.hdf, in READ         */
/*        access, and uses SDfileinfo and SDgetinfo to determine   */
/*        the number of data sets and get the name, rank, and      */
/*        number of attributes in each data set.                   */
/*                                                                 */
/*******************************************************************/

#include "hdf.h"

main( ) 
{
	int32 sd_id, sds_id, n_datasets, n_file_attrs, index;
	int32 dim_sizes[MAX_VAR_DIMS];
	int32 rank, num_type, attributes, istat;
	char name[64];

	/* Open the file and initiate the SD interface. */
	sd_id = SDstart("sd.hdf", DFACC_READ);
	if (sd_id != FAIL)
		printf ("\n... sd.hdf opened for READ access\n");

	/* Determine the contents of the file. */
	istat = SDfileinfo(sd_id, &n_datasets, &n_file_attrs);

	/* Access and print the name of every data set in the file. */
	for (index = 0; index < n_datasets; index++) {
		sds_id = SDselect(sd_id, index);

		istat = SDgetinfo(sds_id, name, &rank, dim_sizes,  \
		 			&num_type, &attributes);

		printf("name = %s\n", name);
		printf("rank = %i\n", rank);
		printf("number of attributes = %i\n", attributes);

		istat = SDendaccess(sds_id);
	}

	/* Terminate access to the SD interface and close the file. */
	istat = SDend(sd_id);
	if (istat != FAIL)
		printf("... file closed\n\n");
}


