#include <stdio.h>

#include "hdf.h"

#define FILE_NAME      "General_Vdatas3.hdf"
#define N_RECORDS      10 /* number of records the vdata contains */
#define ORDER_1        3  /* order of first field */
#define ORDER_2        1  /* order of second field */
#define ORDER_3        2  /* order of third field */
#define CLASS_NAME     "Particle Data"
#define VDATA_NAME     "Solid Particle"
#define FIELD1_NAME    "Position"                  /* contains x, y, z values */
#define FIELD2_NAME    "Mass"                      /* contains weight values */
#define FIELD3_NAME    "Temperature"               /* contains min and max values */
#define FIELDNAME_LIST "Position,Mass,Temperature" /* No spaces b/w names */

/* number of values per record */
#define N_VALS_PER_REC (ORDER_1 + ORDER_2 + ORDER_3)

int
main()
{
    /************************* Variable declaration **************************/

    int32   file_id, vdata_id;
    int32   vdata_ref = -1;                      /* ref number of a vdata, set to -1 to create  */
    int16   rec_num;                             /* current record number */
    float32 data_buf[N_RECORDS][N_VALS_PER_REC]; /* buffer for vdata values */

    /********************** End of variable declaration **********************/

    /*
     * Open the HDF file for writing.
     */
    if ((file_id = Hopen(FILE_NAME, DFACC_WRITE, 0)) == FAIL)
        printf("*** ERROR from Hopen\n");

    /*
     * Initialize the VS interface.
     */
    if (Vstart(file_id) == FAIL)
        printf("*** ERROR from Vstart\n");

    /*
     * Create a new vdata.
     */
    vdata_id = VSattach(file_id, vdata_ref, "w");

    /*
     * Set name and class name of the vdata.
     */
    if (VSsetname(vdata_id, VDATA_NAME) == FAIL)
        printf("*** ERROR from VSsetname\n");
    if (VSsetclass(vdata_id, CLASS_NAME) == FAIL)
        printf("*** ERROR from VSsetclass\n");

    /*
     * Introduce each field's name, data type, and order.  This is the first
     * part in defining a field.
     */
    if (VSfdefine(vdata_id, FIELD1_NAME, DFNT_FLOAT32, ORDER_1) == FAIL)
        printf("*** ERROR from VSfdefine\n");
    if (VSfdefine(vdata_id, FIELD2_NAME, DFNT_FLOAT32, ORDER_2) == FAIL)
        printf("*** ERROR from VSfdefine\n");
    if (VSfdefine(vdata_id, FIELD3_NAME, DFNT_FLOAT32, ORDER_3) == FAIL)
        printf("*** ERROR from VSfdefine\n");

    /*
     * Finalize the definition of the fields.
     */
    if (VSsetfields(vdata_id, FIELDNAME_LIST) == FAIL)
        printf("*** ERROR from VSsetfields\n");

    /*
     * Buffer the data by the record for fully interlaced mode.  Note that the
     * first three elements contain the three values of the first field, the
     * fourth element contains the value of the second field, and the last two
     * elements contain the two values of the third field.
     */
    for (rec_num = 0; rec_num < N_RECORDS; rec_num++) {
        data_buf[rec_num][0] = 1.0 * rec_num;
        data_buf[rec_num][1] = 2.0 * rec_num;
        data_buf[rec_num][2] = 3.0 * rec_num;
        data_buf[rec_num][3] = 0.1 + rec_num;
        data_buf[rec_num][4] = 0.0;
        data_buf[rec_num][5] = 65.0;
    }

    /*
     * Write the data from data_buf to the vdata with full interlacing mode.
     */
    VSwrite(vdata_id, (uint8 *)data_buf, N_RECORDS, FULL_INTERLACE);

    /*
     * Terminate access to the vdata and to the VS interface, then close
     * the HDF file.
     */
    if (VSdetach(vdata_id) == FAIL)
        printf("*** ERROR from VSdetach\n");
    if (Vend(file_id) == FAIL)
        printf("*** ERROR from Vend\n");
    if (Hclose(file_id) == FAIL)
        printf("*** ERROR from Hclose\n");

    return 0;
}
