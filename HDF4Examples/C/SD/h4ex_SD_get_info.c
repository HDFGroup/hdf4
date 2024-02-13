#include <stdio.h>

#include "mfhdf.h"

#define FILE_NAME "SDS.hdf"

int
main()
{
    /************************* Variable declaration **************************/

    int32 sd_id, sds_id;
    int32 n_datasets, n_file_attrs, index;
    int32 dim_sizes[H4_MAX_VAR_DIMS];
    int32 rank, data_type, n_attrs;
    char  name[H4_MAX_NC_NAME];
    int   i;

    /********************* End of variable declaration ***********************/

    /*
     * Open the file and initialize the SD interface.
     */
    if ((sd_id = SDstart(FILE_NAME, DFACC_READ)) == FAIL)
        printf("*** ERROR from SDstart\n");

    /*
     * Determine the number of data sets in the file and the number
     * of file attributes.
     */
    if (SDfileinfo(sd_id, &n_datasets, &n_file_attrs) == FAIL)
        printf("*** ERROR from SDfileinfo\n");

    /*
     * Access every data set and print its name, rank, dimension sizes,
     * data type, and number of attributes.
     * The following information should be displayed:
     *
     *               name = SDStemplate
     *               rank = 2
     *               dimension sizes are : 16  5
     *               data type is  24
     *               number of attributes is  0
     */
    for (index = 0; index < n_datasets; index++) {
        sds_id = SDselect(sd_id, index);
        if (SDgetinfo(sds_id, name, &rank, dim_sizes, &data_type, &n_attrs) == FAIL)
            printf("*** ERROR from SDgetinfo\n");

        printf("name = %s\n", name);
        printf("rank = %d\n", rank);
        printf("dimension sizes are : ");
        for (i = 0; i < rank; i++)
            printf("%d  ", dim_sizes[i]);
        printf("\n");
        printf("data type is  %d\n", data_type);
        printf("number of attributes is  %d\n", n_attrs);

        /*
         * Terminate access to the data set.
         */
        if (SDendaccess(sds_id) == FAIL)
            printf("*** ERROR from SDendaccess\n");
    }

    /*
     * Terminate access to the SD interface and close the file.
     */
    if (SDend(sd_id) == FAIL)
        printf("*** ERROR from SDend\n");

    return 0;
}
