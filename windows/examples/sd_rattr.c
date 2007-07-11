/*******************************************************************/
/*                                                                 */
/*  sd_rattr.c                                                     */
/*     This program reads the three attributes added to the HDF    */
/*     file, sd.hdf, in the previous tutorial.                     */
/*                                                                 */
/*******************************************************************/

#include "mfhdf.h"

main( ) 
{

    int32 sd_id, sds_id, sds_idx, dim_id;
    int32 attr_index, num_type, count, status; 
    char attr_name[64], buffer[64];
    int32 range_buffer[2];
    intn i,j;	

    /* Open the file. */
    sd_id = SDstart("sd.hdf", DFACC_READ);
    if (sd_id == FAIL) {
       printf ("SDstart failed.\n");
       HEprint (stdout, 0);
       exit (-1);
    }
    else
       printf ("\n... sd.hdf opened for READ ONLY access\n");

    /* Find the file attribute named "file_contents". */
    attr_index = SDfindattr(sd_id, "file_contents");
    if (attr_index == FAIL) {
       printf ("SDfindattr failed.\n");
       HEprint (stdout, 0);
       exit (-1);
    }

    /* Get information about the file attribute. */
    status = SDattrinfo(sd_id, attr_index, attr_name, &num_type, &count);
    if (status == FAIL) {
       printf ("SDattrinfo failed.\n");
       HEprint (stdout, 0);
       exit (-1);
    }

    /* Read the attribute data. */
    status = SDreadattr(sd_id, attr_index, buffer);
    if (status == FAIL) {
       printf ("SDreadattr failed.\n");
       HEprint (stdout, 0);
       exit (-1);
    }
    else {
       printf("\n%s: ", attr_name);
       for (i=0; i < count; i++)
       printf ("%c", buffer[i]);
       printf ("\n");
    }

    /* Get the identifier for the first data set. */
    sds_idx = SDnametoindex(sd_id, "data1");
    if (sds_idx == FAIL) {
       printf ("SDnametoindex failed.\n");
       HEprint (stdout, 0);
       exit (-1);
    }
    
    sds_id = SDselect(sd_id, sds_idx);
    if (sds_id == FAIL) {
       printf ("SDselect failed.\n");
       HEprint (stdout, 0);
       exit (-1);
    }

    /* Find the data set attribute named "valid_range". */
    attr_index = SDfindattr(sds_id, "valid_range");
    if (attr_index == FAIL) {
       printf ("SDfindattr failed.\n");
       HEprint (stdout, 0);
       exit (-1);
    }

    /* Read the attribute data. */
    status = SDreadattr(sds_id, attr_index, range_buffer);
    if (status == FAIL) {
       printf ("SDreadattr failed.\n");
       HEprint (stdout, 0);
       exit (-1);
    }
    else
       printf("valid range: %i %i\n",range_buffer[0],range_buffer[1]);

    dim_id = SDgetdimid (sds_id, 0);
    if (dim_id == FAIL) {
       printf ("SDgetdimid failed.\n");
       HEprint (stdout, 0);
       exit (-1);
    }

    attr_index = SDfindattr (dim_id, "dim_metric");
    if (attr_index == FAIL) {
       printf ("SDfindattr failed.\n");
       HEprint (stdout, 0);
       exit (-1);
    }

    status = SDreadattr(dim_id, attr_index, buffer);
    if (status == FAIL) {
       printf ("SDreadattr failed.\n");
       HEprint (stdout, 0);
       exit (-1);
    }
    else {
       printf ("dim_metric: ");
       for (i=0; i < 9; i++)
          printf ("%c", buffer[i]);
       printf ("\n");
    }

    /* Terminate access to the array */
    status = SDendaccess(sds_id);
    if (status == FAIL) {
       printf ("SDendaccess failed.\n");
       HEprint (stdout, 0);
       exit (-1);
    }

    /* Terminate access to the SD interface and close the file */
    status = SDend(sd_id);
    if (status == FAIL) {
       printf ("SDend failed.\n");
       HEprint (stdout, 0);
       exit (-1);
    }
    else
       printf ("\n... closing file\n\n");

}


