#include <stdio.h>

#include "hdf.h"

#define FILE_NAME  "Image_Chunked.hdf"
#define IMAGE_NAME "Image with Chunks"
#define X_LENGTH   10 /* number of rows in the image */
#define Y_LENGTH   6  /* number of columns in the image */
#define NCOMPS     3  /* number of components in the image */

int
main()
{
    /************************* Variable declaration **************************/

    int32 file_id, /* HDF file identifier */
        gr_id,     /* GR interface identifier */
        ri_id,     /* raster image identifier */
        dims[2],   /* dimension sizes of the image array */
        start[2],  /* start position to read the image array */
        edges[2];  /* edges of read array */
    int32 image_data[X_LENGTH][Y_LENGTH][NCOMPS];
    int   ii, jj;

    /********************** End of variable declaration **********************/

    /*
     * Open the file for reading.
     */
    if ((file_id = Hopen(FILE_NAME, DFACC_RDONLY, 0)) == FAIL)
        printf("*** ERROR from Hopen\n");

    /*
     * Initialize the GR interface.
     */
    if ((gr_id = GRstart(file_id)) == FAIL)
        printf("*** ERROR from GRstart\n");

    /*
     * Open the raster image array.
     */
    ri_id = GRselect(gr_id, 0);

    /*
     * Set dimensions of the image.
     */
    dims[0]  = X_LENGTH;
    dims[1]  = Y_LENGTH;
    start[0] = start[1] = 0;
    edges[0]            = dims[0];
    edges[1]            = dims[1];

    /* Read the data in the image array. */
    if (GRreadimage(ri_id, start, NULL, edges, (void *)image_data) == FAIL)
        printf("*** ERROR from GRreadimage\n");

    printf("Image Data:\n");
    printf("Component 1:\n  ");
    for (ii = 0; ii < X_LENGTH; ii++) {
        for (jj = 0; jj < Y_LENGTH; jj++)
            printf("%i ", image_data[ii][jj][0]);
        printf("\n  ");
    }
    printf("\nComponent 2:\n  ");
    for (ii = 0; ii < X_LENGTH; ii++) {
        for (jj = 0; jj < Y_LENGTH; jj++)
            printf("%i ", image_data[ii][jj][1]);
        printf("\n  ");
    }
    printf("\nComponent 3:\n  ");
    for (ii = 0; ii < X_LENGTH; ii++) {
        for (jj = 0; jj < Y_LENGTH; jj++)
            printf("%i ", image_data[ii][jj][2]);
        printf("\n  ");
    }

    printf("\n");

    /*
     * Terminate access to the raster image and to the GR interface and,
     * close the HDF file.
     */
    if (GRendaccess(ri_id) == FAIL)
        printf("*** ERROR from GRendaccess\n");
    if (GRend(gr_id) == FAIL)
        printf("*** ERROR from GRend\n");
    if (Hclose(file_id) == FAIL)
        printf("*** ERROR from Hclose\n");

    return 0;
}
