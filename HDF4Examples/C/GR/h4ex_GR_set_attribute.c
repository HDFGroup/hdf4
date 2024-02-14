#include <stdio.h>

#include "hdf.h"

#define FILE_NAME        "General_RImages4.hdf"
#define IMAGE_NAME       "Image Array 2"
#define F_ATT1_NAME      "File Attribute 1"
#define F_ATT2_NAME      "File Attribute 2"
#define RI_ATT1_NAME     "Image Attribute 1"
#define RI_ATT2_NAME     "Image Attribute 2"
#define F_ATT1_VAL       "Contents of First FILE Attribute"
#define F_ATT2_VAL       "Contents of Second FILE Attribute"
#define F_ATT1_N_VALUES  32
#define F_ATT2_N_VALUES  33
#define RI_ATT1_VAL      "Contents of IMAGE's First Attribute"
#define RI_ATT1_N_VALUES 35
#define RI_ATT2_N_VALUES 6

int
main()
{
    /************************* Variable declaration **************************/

    int32 gr_id, ri_id, file_id, ri_index;
    int16 ri_attr_2[RI_ATT2_N_VALUES] = {1, 2, 3, 4, 5, 6};

    /********************** End of variable declaration **********************/

    /*
     * Open the HDF file.
     */
    if ((file_id = Hopen(FILE_NAME, DFACC_WRITE, 0)) == FAIL)
        printf("*** ERROR from Hopen\n");

    /*
     * Initialize the GR interface.
     */
    gr_id = GRstart(file_id);

    /*
     * Set two file attributes to the file with names, data types, numbers of
     * values, and values of the attributes specified.
     */
    if (GRsetattr(gr_id, F_ATT1_NAME, DFNT_CHAR8, F_ATT1_N_VALUES, (void *)F_ATT1_VAL) == FAIL)
        printf("*** ERROR from GRsetattr\n");

    if (GRsetattr(gr_id, F_ATT2_NAME, DFNT_CHAR8, F_ATT2_N_VALUES, (void *)F_ATT2_VAL) == FAIL)
        printf("*** ERROR from GRsetattr\n");

    /*
     * Obtain the index of the image named IMAGE_NAME.
     */
    ri_index = GRnametoindex(gr_id, IMAGE_NAME);

    /*
     * Obtain the identifier of this image.
     */
    ri_id = GRselect(gr_id, ri_index);

    /*
     * Set two attributes to the image with names, data types, numbers of
     * values, and values of the attributes specified.
     */
    if (GRsetattr(ri_id, RI_ATT1_NAME, DFNT_CHAR8, RI_ATT1_N_VALUES, (void *)RI_ATT1_VAL) == FAIL)
        printf("*** ERROR from GRsetattr\n");

    if (GRsetattr(ri_id, RI_ATT2_NAME, DFNT_INT16, RI_ATT2_N_VALUES, (void *)ri_attr_2) == FAIL)
        printf("*** ERROR from GRsetattr\n");

    /*
     * Terminate access to the image and to the GR interface, and close the
     * HDF file.
     */
    if (GRendaccess(ri_id) == FAIL)
        printf("*** ERROR from GRendaccess\n");
    if (GRend(gr_id) == FAIL)
        printf("*** ERROR from GRend\n");
    if (Hclose(file_id) == FAIL)
        printf("*** ERROR from Hclose\n");

    return 0;
}
