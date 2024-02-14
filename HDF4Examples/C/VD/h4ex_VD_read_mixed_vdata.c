#include <stdio.h>

#include "hdf.h"

#define N_RECORDS      20 /* number of records to be read */
#define N_FIELDS       2  /* number of fields to be read */
#define FILE_NAME      "Packed_Vdata.hdf"
#define VDATA_NAME     "Mixed Data Vdata"
#define FIELDNAME_LIST "Temp,Ident"

/* number of bytes of the data to be read */
#define BUFFER_SIZE (sizeof(float32) + sizeof(char)) * N_RECORDS

int
main()
{
    /************************* Variable declaration **************************/

    int32   file_id, vdata_id;
    int32   num_of_records;       /* number of records actually read */
    int32   vdata_ref;            /* reference number of the vdata to be read */
    float32 itemp[N_RECORDS];     /* buffer to hold values of first field     */
    char    idents[N_RECORDS];    /* buffer to hold values of fourth field    */
    uint8   databuf[BUFFER_SIZE]; /* buffer to hold read data, still packed   */
    void   *fldbufptrs[N_FIELDS]; /*pointers to be pointing to the field buffers*/
    int     i;

    /********************** End of variable declaration **********************/

    /*
     * Open the HDF file for reading.
     */
    if ((file_id = Hopen(FILE_NAME, DFACC_READ, 0)) == FAIL)
        printf("*** ERROR from Hopen\n");

    /*
     * Initialize the VS interface.
     */
    if (Vstart(file_id) == FAIL)
        printf("*** ERROR from Vstart\n");

    /*
     * Get the reference number of the vdata, whose name is specified in
     * VDATA_NAME, using VSfind, which will be discussed in Section 4.7.3.
     */
    vdata_ref = VSfind(file_id, VDATA_NAME);

    /*
     * Attach to the vdata for reading.
     */
    vdata_id = VSattach(file_id, vdata_ref, "r");

    /*
     * Specify the fields that will be read.
     */
    if (VSsetfields(vdata_id, FIELDNAME_LIST) == FAIL)
        printf("*** ERROR from VSsetfields\n");

    /*
     * Read N_RECORDS records of the vdata and store the values into the
     * buffer databuf.
     */
    num_of_records = VSread(vdata_id, (uint8 *)databuf, N_RECORDS, FULL_INTERLACE);

    /*
     * Build an array of pointers each of which points to an array that
     * will hold all values of a field after being unpacked.
     */
    fldbufptrs[0] = &itemp[0];
    fldbufptrs[1] = &idents[0];

    /*
     * Unpack the data from the buffer databuf and store the values into the
     * appropriate field buffers pointed to by the set of pointers fldbufptrs.
     * Note that the second parameter is _HDF_VSUNPACK for unpacking and the
     * number of records is the one returned by VSread.
     */
    if (VSfpack(vdata_id, _HDF_VSUNPACK, FIELDNAME_LIST, (void *)databuf, BUFFER_SIZE, num_of_records, NULL,
                (void *)fldbufptrs) == FAIL)
        printf("*** ERROR from VSfpack\n");

    /*
     * Display the read data being stored in the field buffers.
     */
    printf("\n     Temp      Ident\n");
    for (i = 0; i < num_of_records; i++)
        printf("   %6.2f        %c\n", itemp[i], idents[i]);

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
