#include <stdio.h>

#include "hdf.h"

#define FILE_NAME      "h4ex_VD_write_mixed_vdata.hdf"
#define VDATA_NAME     "Mixed Data Vdata"
#define CLASS_NAME     "General Data Class"
#define FIELD1_NAME    "Temp"
#define FIELD2_NAME    "Height"
#define FIELD3_NAME    "Speed"
#define FIELD4_NAME    "Ident"
#define ORDER          1                         /* number of values in the field         */
#define N_RECORDS      20                        /* number of records the vdata contains  */
#define N_FIELDS       4                         /* number of fields in the vdata         */
#define FIELDNAME_LIST "Temp,Height,Speed,Ident" /* No spaces b/w names */

/* number of bytes of the data to be written, i.e., the size of all the
   field values combined times the number of records */
#define BUF_SIZE (2 * sizeof(float32) + sizeof(int16) + sizeof(char)) * N_RECORDS

int
main()
{
    /************************* Variable declaration **************************/

    int32   file_id, vdata_id;
    int32   vdata_ref = -1;       /* vdata's reference number, set to -1 to create */
    float32 temp[N_RECORDS];      /* buffer to hold values of first field   */
    int16   height[N_RECORDS];    /* buffer to hold values of second field  */
    float32 speed[N_RECORDS];     /* buffer to hold values of third field   */
    char8   ident[N_RECORDS];     /* buffer to hold values of fourth field  */
    void   *fldbufptrs[N_FIELDS]; /*pointers to be pointing to the field buffers*/
    uint16  databuf[BUF_SIZE];    /* buffer to hold the data after being packed*/
    int     i;

    /********************** End of variable declaration **********************/

    /*
     * Create an HDF file.
     */
    if ((file_id = Hopen(FILE_NAME, DFACC_CREATE, 0)) == FAIL)
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
     * part in defining a vdata field.
     */
    if (VSfdefine(vdata_id, FIELD1_NAME, DFNT_FLOAT32, ORDER) == FAIL)
        printf("*** ERROR from VSfdefine\n");
    if (VSfdefine(vdata_id, FIELD2_NAME, DFNT_INT16, ORDER) == FAIL)
        printf("*** ERROR from VSfdefine\n");
    if (VSfdefine(vdata_id, FIELD3_NAME, DFNT_FLOAT32, ORDER) == FAIL)
        printf("*** ERROR from VSfdefine\n");
    if (VSfdefine(vdata_id, FIELD4_NAME, DFNT_CHAR8, ORDER) == FAIL)
        printf("*** ERROR from VSfdefine\n");

    /*
     * Finalize the definition of the fields of the vdata.
     */
    if (VSsetfields(vdata_id, FIELDNAME_LIST) == FAIL)
        printf("*** ERROR from VSsetfields\n");

    /*
     * Enter data values into the field buffers by the records.
     */
    for (i = 0; i < N_RECORDS; i++) {
        temp[i]   = 1.11 * (i + 1);
        height[i] = i;
        speed[i]  = 1.11 * (i + 1);
        ident[i]  = 'A' + i;
    }

    /*
     * Build an array of pointers each of which points to a field buffer that
     * holds all values of the field.
     */
    fldbufptrs[0] = &temp[0];
    fldbufptrs[1] = &height[0];
    fldbufptrs[2] = &speed[0];
    fldbufptrs[3] = &ident[0];

    /*
     * Pack all data in the field buffers that are pointed to by the set of
     * pointers fldbufptrs, and store the packed data into the buffer
     * databuf.  Note that the second parameter is _HDF_VSPACK for packing.
     */
    if (VSfpack(vdata_id, _HDF_VSPACK, NULL, (void *)databuf, BUF_SIZE, N_RECORDS, NULL,
                (void *)fldbufptrs) == FAIL)
        printf("*** ERROR from VSfpack\n");

    /*
     * Write all records of the packed data to the vdata.
     */
    VSwrite(vdata_id, (uint8 *)databuf, N_RECORDS, FULL_INTERLACE);

    /*
     * Terminate access to the vdata and the VS interface, then close
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
