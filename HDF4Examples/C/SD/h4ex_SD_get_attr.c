#include <stdio.h>
#include <stdlib.h>

#include "mfhdf.h"

#define FILE_NAME      "SDS.hdf"
#define FILE_ATTR_NAME "File_contents"
#define SDS_ATTR_NAME  "Valid_range"
#define DIM_ATTR_NAME  "Dim_metric"

int
main()
{
    /************************* Variable declaration **************************/

    int32 sd_id, sds_id, dim_id;
    int32 attr_index, data_type, n_values;
    char  attr_name[H4_MAX_NC_NAME];
    int   i;

    /********************* End of variable declaration ***********************/

    /*
     * Open the file and initialize SD interface.
     */
    if ((sd_id = SDstart(FILE_NAME, DFACC_READ)) == FAIL)
        printf("*** ERROR from SDstart\n");

    /*
     * Find the file attribute defined by FILE_ATTR_NAME.
     */
    attr_index = SDfindattr(sd_id, FILE_ATTR_NAME);

    /*
     * Get information about the file attribute. Note that the first
     * parameter is an SD interface identifier.
     */
    if (SDattrinfo(sd_id, attr_index, attr_name, &data_type, &n_values) == FAIL)
        printf("*** ERROR from SDattrinfo\n");

    /* The data type should be DFNT_CHAR, from SD_set_attr.c */
    if (data_type == DFNT_CHAR) {
        char *fileattr_data;

        /*
         * Allocate a buffer to hold the attribute data.
         */
        fileattr_data = (char *)malloc((n_values + 1) * DFKNTsize(data_type));

        /*
         * Read the file attribute data.
         */
        if (SDreadattr(sd_id, attr_index, fileattr_data) == FAIL)
            printf("*** ERROR from SDreadattr\n");
        fileattr_data[n_values] = '\0';

        /*
         * Print out file attribute value and free buffer.
         */
        printf("File attribute value is : %s\n", fileattr_data);
        free(fileattr_data);
    }

    /*
     * Select the first data set.
     */
    sds_id = SDselect(sd_id, 0);

    /*
     * Find the data set attribute defined by SDS_ATTR_NAME. Note that the
     * first parameter is a data set identifier.
     */
    attr_index = SDfindattr(sds_id, SDS_ATTR_NAME);

    /*
     * Get information about the data set attribute.
     */
    if (SDattrinfo(sds_id, attr_index, attr_name, &data_type, &n_values) == FAIL)
        printf("*** ERROR from SDattrinfo\n");

    /*
     * The data type should be DFNT_FLOAT32, from SD_set_attr.c.
     */
    if (data_type == DFNT_FLOAT32) {
        float32 *sds_data;

        /*
         * Allocate a buffer to hold the data set attribute data.
         */
        sds_data = (float32 *)malloc(n_values * DFKNTsize(data_type));

        /*
         * Read the SDS attribute data.
         */
        if (SDreadattr(sds_id, attr_index, sds_data) == FAIL)
            printf("*** ERROR from SDreadattr\n");

        /*
         * Print out SDS attribute data type and values and free buffer.
         */
        printf("SDS attribute data type is : float32\n");
        printf("SDS attribute values are :  ");
        for (i = 0; i < n_values; i++)
            printf(" %f", sds_data[i]);
        printf("\n");
        free(sds_data);
    }

    /*
     * Get the identifier for the second dimension of the SDS.
     */
    dim_id = SDgetdimid(sds_id, 1);

    /*
     * Find dimension attribute defined by DIM_ATTR_NAME.
     */
    attr_index = SDfindattr(dim_id, DIM_ATTR_NAME);

    /*
     * Get information about the dimension attribute.
     */
    if (SDattrinfo(dim_id, attr_index, attr_name, &data_type, &n_values) == FAIL)
        printf("*** ERROR from SDattrinfo\n");

    /*
     * The data type should be DFNT_CHAR, from SD_set_attr.c.
     */
    if (data_type == DFNT_CHAR) {
        char *dimattr_data;

        /*
         * Allocate a buffer to hold the dimension attribute data.
         */
        dimattr_data = (char *)malloc((n_values + 1) * DFKNTsize(data_type));

        /*
         * Read the dimension attribute data.
         */
        if (SDreadattr(dim_id, attr_index, dimattr_data) == FAIL)
            printf("*** ERROR from SDreadattr\n");
        dimattr_data[n_values] = '\0';

        /*
         * Print out dimension attribute value and free buffer.
         */
        printf("Dimensional attribute values is : %s\n", dimattr_data);
        free(dimattr_data);
    }

    /*
     * Terminate access to the data set and to the SD interface and
     * close the file.
     */
    if (SDendaccess(sds_id) == FAIL)
        printf("*** ERROR from SDendaccess\n");
    if (SDend(sd_id) == FAIL)
        printf("*** ERROR from SDend\n");

    /*   Output of this program is :
     *
     *    File attribute value is : Storm_track_data
     *    SDS attribute data type is : float32
     *    SDS attribute values are :   2.000000 10.000000
     *    Dimensional attribute values is : Seconds
     */

    return 0;
}
