#include "hdf.h"

#define FILE_NAME    "General_RImages.hdf"
#define RI_ATTR_NAME "Image Attribute 2"

int
main()
{
    /************************* Variable declaration **************************/

    intn   status; /* status for functions returning an intn */
    int32  gr_id, ri_id, file_id;
    int32  f_att_index;               /* index of file attributes */
    int32  ri_att_index;              /* index of raster image attributes */
    int32  data_type;                 /* image data type */
    int32  n_values;                  /* number of values in an attribute */
    int32  value_index;               /* index of values in an attribute */
    int32  n_rimages;                 /* number of raster images in the file */
    int32  n_file_attrs;              /* number of file attributes */
    char   attr_name[H4_MAX_GR_NAME]; /* buffer to hold the attribute name     */
    VOIDP  data_buf = NULL;           /* buffer to hold the attribute values   */
    int16 *int_ptr;                   /* int16 pointer to point to a void data buffer     */
    char8 *char_ptr;                  /* char8 pointer to point to a void data buffer     */

    /********************** End of variable declaration **********************/

    /*
     * Open the HDF file.
     */
    file_id = Hopen(FILE_NAME, DFACC_READ, 0);

    /*
     * Initialize the GR interface.
     */
    gr_id = GRstart(file_id);

    /*
     * Determine the number of attributes in the file.
     */
    status = GRfileinfo(gr_id, &n_rimages, &n_file_attrs);
    CHECK_NOT_VAL(status, FAIL, "GRfileinfo");
    if (status != FAIL && n_file_attrs > 0) {
        for (f_att_index = 0; f_att_index < n_file_attrs; f_att_index++) {
            /*
             * Get information about the current file attribute.
             */
            status = GRattrinfo(gr_id, f_att_index, attr_name, &data_type, &n_values);
            CHECK_NOT_VAL(status, FAIL, "GRattrinfo");

            /*
             * Allocate a buffer to hold the file attribute data.  In this example,
             * knowledge about the data type is assumed to be available from
             * the previous example for simplicity.  In reality, the size
             * of the type must be determined based on the machine where the
             * program resides.
             */
            if (data_type == DFNT_CHAR8) {
                data_buf = malloc(n_values * sizeof(char8));
                if (data_buf == NULL) {
                    printf("Unable to allocate space for attribute data.\n");
                    exit(1);
                }
            }
            else {
                printf("Unable to determine data type to allocate data buffer.\n");
                exit(1);
            }

            /*
             * Read and display the attribute values.
             */
            status = GRgetattr(gr_id, f_att_index, (VOIDP)data_buf);
            CHECK_NOT_VAL(status, FAIL, "GRgetattr");

            char_ptr = (char8 *)data_buf;
            printf("Attribute %s: ", attr_name);
            for (value_index = 0; value_index < n_values; value_index++)
                printf("%c", char_ptr[value_index]);
            printf("\n");

            /*
             * Free the space allocated for the data buffer.
             */

            free(data_buf);

        } /* for */
    }     /* if */

    /*
     * Select the second image in the file.
     */
    ri_id = GRselect(gr_id, 1);

    /*
     * Find the image attribute named RI_ATTR_NAME.
     */
    ri_att_index = GRfindattr(ri_id, RI_ATTR_NAME);

    /*
     * Get information about the attribute.
     */
    status = GRattrinfo(ri_id, ri_att_index, attr_name, &data_type, &n_values);
    CHECK_NOT_VAL(status, FAIL, "GRattrinfo");

    /*
     * Allocate a buffer to hold the file attribute data.  As mentioned above,
     * knowledge about the data type is assumed to be available from
     * the previous example for simplicity.  In reality, the size of the
     * type must be determined based on the machine where the program resides.
     */
    if (data_type == DFNT_INT16)
        data_buf = malloc(n_values * sizeof(int16));

    /*
     * Read and display the attribute values.
     */
    status = GRgetattr(ri_id, ri_att_index, (VOIDP)data_buf);
    CHECK_NOT_VAL(status, FAIL, "GRgetattr");

    printf("\nAttribute %s: ", RI_ATTR_NAME);
    int_ptr = (int16 *)data_buf;
    for (value_index = 0; value_index < n_values; value_index++)
        printf("%d ", int_ptr[value_index]);
    printf("\n");

    /*
     * Free the space allocated for the data buffer.
     */
    free(data_buf);

    /*
     * Terminate access to the raster image and to the GR interface, and
     * close the file.
     */

    status = GRendaccess(ri_id);
    CHECK_NOT_VAL(status, FAIL, "GRendaccess");
    status = GRend(gr_id);
    CHECK_NOT_VAL(status, FAIL, "GRend");
    status = Hclose(file_id);
    CHECK_NOT_VAL(status, FAIL, "Hclose");

    return 0;
}
