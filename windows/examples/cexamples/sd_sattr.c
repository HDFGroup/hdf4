/*******************************************************************/
/*                                                                 */
/*  sd_sattr.c                                                     */
/*     This program adds three attributes to the HDF file, sd.hdf, */
/*     which was created using sd_create.c:                        */         
/*          file_contents   - A file attribute, with a value of    */
/*                            "storm_track_data"                   */
/*          valid_range     - A data set attribute with range      */ 
/*                            values of 2 and 10                   */
/*          dim_metric      - A dimension attribute for dimension  */
/*                            zero, with a value of "millibars"    */
/*                                                                 */
/*******************************************************************/
 

#include "hdf.h"

main( ) 
{

	int32 sd_id, sds_id, dim_id, dim_index, istat;
	int32 num_values[2];

	/* Open the file and get the identifier for the file. */
	sd_id = SDstart("sd.hdf", DFACC_WRITE);
	if (sd_id == FAIL) {
		printf ("SDstart failed.\n"); 
		HEprint(stdout,0);
 		exit (-1);
	}
	else
		printf ("\n... sd.hdf opened for WRITE access\n");

	/* Set an attribute that describes the file contents. */
	istat = SDsetattr(sd_id, "file_contents", DFNT_CHAR8, 16, (VOIDP)"storm_track_data");
	if (istat == FAIL) {
		printf ("SDsetattr failed.\n"); 
		HEprint(stdout,0);
		exit (-1);
	}
	else
		printf ("... file attribute written\n");

	/* Get the identifier for the first data set. */
	sds_id = SDselect(sd_id, 0);
	if (sds_id == FAIL) {
		printf ("SDselect failed.\n"); 
		HEprint(stdout,0);
		exit (-1);
	}
	/* Set an attribute the specifies a valid range of values. */
	num_values[0] = 2;
	num_values[1] = 10;
	istat = SDsetattr(sds_id, "valid_range", DFNT_INT32, 2, (VOIDP)num_values);
	if (istat == FAIL) {
		printf ("SDsetattr failed.\n"); 
		HEprint(stdout,0);
		exit (-1);
	} 
	else
		printf ("... data attribute written\n");

	/* Get the identifier for the first dimension. */
	dim_id = SDgetdimid(sds_id, 0);
	if (dim_id == FAIL) {
		printf ("SDgetdimid failed.\n"); 
		HEprint(stdout,0);
		exit (-1);
	} 

	/* Set an attribute that specifies the dimension metric. */
	istat = SDsetattr(dim_id, "dim_metric", DFNT_CHAR8, 9,  (VOIDP)"millibars");
	if (istat == FAIL) {
		printf ("SDsetattr failed.\n"); 
		HEprint(stdout,0);
		exit (-1);
	} 
	else
		printf ("... dimension attribute written\n");

	/* Terminate access to the array */
	istat = SDendaccess(sds_id);
	if (istat == FAIL) {
		printf ("SDendaccess failed.\n"); 
		HEprint(stdout,0);
		exit (-1);
	} 

	/* Terminate access to the SD interface and close the file */
	istat = SDend(sd_id);
	if (istat == FAIL) {
		printf ("SDend failed.\n"); 
		HEprint(stdout,0);
		exit (-1);
	} 
	else
		printf("... file closed\n\n");

}


