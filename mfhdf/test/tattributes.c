/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Copyright by The HDF Group.                                               *
 * All rights reserved.                                                      *
 *                                                                           *
 * This file is part of HDF.  The full HDF copyright notice, including       *
 * terms governing use, modification, and redistribution, is contained in    *
 * the COPYING file, which can be found at the root of the source code       *
 * distribution tree, or in https://support.hdfgroup.org/ftp/HDF/releases/.  *
 * If you do not have access to either file, you may request a copy from     *
 * help@hdfgroup.org.                                                        *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/****************************************************************************
 * tattributes.c - tests attribute features
 *	(currently only SDsetattr with count = 0)
 * Structure of the file:
 *    test_attributes - test driver
 *	  test_count - tests that SDsetattr fails when the parameter
 *		"count" is set to 0.  (HDFFD-989 and 227: SDsetattr didn't
 *		fail but, eventually, SDend did)
 *
 ****************************************************************************/

#include <string.h>
#include <stdlib.h>

#include "mfhdf.h"

#include "hdftest.h"

/********************************************************************
   Name: test_count() - tests that SDsetattr fails when the parameter
                        "count" is passed into SDsetattr as 0 and the
                        requested attribute is not created.

   Description:
        In the past, when the parameter "count" was set to 0, SDsetattr
        didn't fail but, eventually, SDend failed when the vdata storing
        the attribute was being created.  With the knowledge that existing
        applications would have failed eventually at SDend if such calls
        existed, and if SDend failure is not detected, the file will be
        corrupted, we decided to change so that SDsetattr now fails when
        "count" is 0.

        The main contents of the test are listed below.
        - create a one-dim SDS, named VAR1_NAME
        - name its dimension VAR1_NAME
        - get file information and verify that there is only 1 variable,
          dataset VAR1_NAME
        - set attribute to dimension "Variable 1" (SDsetattr)
        - set attribute to SDS "Variable 1" (SDsetattr)
        - get file information and verify that there are 2 variable,
          dataset VAR1_NAME and coordinate variable VAR1_NAME
        - write data to the SDS
        - close all and reopen the file
        - open dataset "Variable 1" (SDnametoindex)
        - verify that this variable is not a coordinate variable (SDiscoordvar)
        - read and verify its attribute information and values
        - get access to the dataset's first dimension
        - read and verify its attribute information and values
        - read data and verify that the data is not corrupted

   Return value:
        The number of errors occurred in this routine.

*********************************************************************/

#define FILE_SATTR    "tattributes.hdf"
#define VAR1_NAME     "Variable 1"
#define DIM1_NAME     "Dimension 1"
#define ATTR1_NAME    "Attribute Dimension 1"
#define ATTR2_NAME    "Attribute SDS 1"
#define ATTR3_NAME    "Attribute Zero"
#define ATTR1_VAL     "This is a good attr"
#define ATTR2_VAL     "This is a another attr"
#define ATTR1_LEN     20
#define ATTR2_LEN     23
#define ATTR3_LEN     5
#define ATTR_ZERO     "ZERO"
#define ATTR_LEN_ZERO 0

static int
test_count(void)
{
    char    sds_name[20], dim_name[20];
    float32 sds1_data[] = {0.1F, 2.3F, 4.5F, 6.7F, 8.9F};
    int32   dimsize[1], size;
    int32   sds_id, file_id, dim_id, index;
    int32   start = 0, stride = 1;
    int32   ntype, rank, count;
    int32   nattrs = 0;
    int     status = 0;
    char    attr_name[H4_MAX_NC_NAME], attr_values[80];
    int     num_errs = 0; /* number of errors so far */

    file_id = SDstart(FILE_SATTR, DFACC_CREATE);
    CHECK(file_id, FAIL, "SDstart");

    /* Create a one-dim dataset named VAR1_NAME, of type DFNT_FLOAT32. */
    dimsize[0] = 5;
    sds_id     = SDcreate(file_id, VAR1_NAME, DFNT_FLOAT32, 1, dimsize);
    CHECK(sds_id, FAIL, "SDcreate");

    /* Set the dimension name. */
    dim_id = SDgetdimid(sds_id, 0);
    CHECK(dim_id, FAIL, "SDgetdimid");
    status = SDsetdimname(dim_id, DIM1_NAME);
    CHECK(status, FAIL, "SDsetdimname");

    /* Attempt to set a string attr to dim passing in count=0, should fail. */
    status = SDsetattr(dim_id, ATTR_ZERO, DFNT_CHAR8, ATTR_LEN_ZERO, ATTR1_VAL);
    VERIFY(status, FAIL, "SDsetattr");

    /* To verify that this attribute is not created, SDend must be called and
       the file is opened again. */

    /* Close dataset and file. */
    status = SDendaccess(sds_id);
    CHECK(status, FAIL, "SDendaccess");
    status = SDend(file_id);
    CHECK(status, FAIL, "SDend");

    /* Open the file again to check attributes */
    file_id = SDstart(FILE_SATTR, DFACC_RDWR);
    CHECK(file_id, FAIL, "SDstart");

    /* Get index of dataset VAR1_NAME */
    index = SDnametoindex(file_id, VAR1_NAME);
    CHECK(index, FAIL, "SDnametoindex");

    sds_id = SDselect(file_id, index);
    CHECK(sds_id, FAIL, "SDselect");

    /* Get access to the SDS' first dimension. */
    dim_id = SDgetdimid(sds_id, 0);
    CHECK(dim_id, FAIL, "SDgetdimid");

    /* Get dimension information to verify there are no attrs set */
    status = SDdiminfo(dim_id, dim_name, &size, &ntype, &nattrs);
    VERIFY(nattrs, 0, "SDdiminfo");

    /* Set an attribute to dimension DIM1_NAME. */
    status = SDsetattr(dim_id, ATTR1_NAME, DFNT_CHAR8, ATTR1_LEN, ATTR1_VAL);
    CHECK(status, FAIL, "SDsetattr");

    /* Attempt to set a string attr to SDS passing in count=0, should fail. */
    status = SDsetattr(sds_id, ATTR_ZERO, DFNT_CHAR8, ATTR_LEN_ZERO, ATTR2_VAL);
    VERIFY(status, FAIL, "SDsetattr");

    /* Attempt to set a float attr to SDS passing in count=0, should fail. */
    status = SDsetattr(sds_id, ATTR_ZERO, DFNT_FLOAT32, ATTR_LEN_ZERO, ATTR2_VAL);
    VERIFY(status, FAIL, "SDsetattr");

    /* Close the file and reopen it to verify the number of attributes for SDS
       and for the dimension. */

    /* Close dataset and file. */
    status = SDendaccess(sds_id);
    CHECK(status, FAIL, "SDendaccess");
    status = SDend(file_id);
    CHECK(status, FAIL, "SDend");

    /* Open the file again to check attributes */
    file_id = SDstart(FILE_SATTR, DFACC_RDWR);
    CHECK(file_id, FAIL, "SDstart");

    /* Get index of dataset VAR1_NAME */
    index = SDnametoindex(file_id, VAR1_NAME);
    CHECK(index, FAIL, "SDnametoindex");

    sds_id = SDselect(file_id, index);
    CHECK(sds_id, FAIL, "SDselect");

    /* Get access to the SDS' first dimension. */
    dim_id = SDgetdimid(sds_id, 0);
    CHECK(dim_id, FAIL, "SDgetdimid");

    /* Get dimension information to verify there is one attribute set */
    status = SDdiminfo(dim_id, dim_name, &size, &ntype, &nattrs);
    VERIFY(nattrs, 1, "SDdiminfo");

    /* Get SDS info to verify there is no SDS attribute yet. */
    status = SDgetinfo(sds_id, sds_name, &rank, dimsize, &ntype, &nattrs);
    VERIFY(nattrs, 0, "SDsetattr");

    /* Set an attribute to dataset VAR1_NAME. */
    status = SDsetattr(sds_id, ATTR2_NAME, DFNT_CHAR8, ATTR2_LEN, ATTR2_VAL);
    CHECK(status, FAIL, "SDsetattr");

    /* Write data to the SDS */
    status = SDwritedata(sds_id, &start, &stride, dimsize, (void *)sds1_data);
    CHECK(status, FAIL, "SDwritedata");

    /* Close dataset and file. */
    status = SDendaccess(sds_id);
    CHECK(status, FAIL, "SDendaccess");
    status = SDend(file_id);
    CHECK(status, FAIL, "SDend");

    /* Open the file again to check attributes */
    file_id = SDstart(FILE_SATTR, DFACC_RDWR);
    CHECK(file_id, FAIL, "SDstart");

    /* Get index of dataset VAR1_NAME */
    index = SDnametoindex(file_id, VAR1_NAME);
    CHECK(index, FAIL, "SDnametoindex");

    sds_id = SDselect(file_id, index);
    CHECK(sds_id, FAIL, "SDselect");

    /* Get dataset information to verify that there is one attribute set. */
    status = SDgetinfo(sds_id, sds_name, &rank, dimsize, &ntype, &nattrs);
    VERIFY(nattrs, 1, "SDsetattr");

    /* Read and verify the information of the SDS' first attribute. */
    status = SDattrinfo(sds_id, 0, attr_name, &ntype, &count);
    CHECK(status, FAIL, "SDattrinfo");
    VERIFY(count, ATTR2_LEN, "SDattrinfo");
    VERIFY(strncmp(attr_name, ATTR2_NAME, 14), 0, "SDattrinfo");

    /* Read and verify the values of the SDS' first attribute. */
    status = SDreadattr(sds_id, 0, attr_values);
    CHECK(status, FAIL, "SDreadattr");

    if (strncmp(attr_values, ATTR2_VAL, ATTR2_LEN) != 0) {
        fprintf(stderr, "Unmatched attribute values for SDS %s: is <%s>, should be <%s>\n", VAR1_NAME,
                attr_values, ATTR2_VAL);
        num_errs++;
    }

    /* Get access to the SDS' first dimension. */
    dim_id = SDgetdimid(sds_id, 0);
    CHECK(dim_id, FAIL, "SDgetdimid");

    /* Get dimension information to verify number of attrs */
    status = SDdiminfo(dim_id, dim_name, &size, &ntype, &nattrs);
    VERIFY(nattrs, 1, "SDattrinfo");

    /* Read and verify the information of the dimension's first attribute. */
    status = SDattrinfo(dim_id, 0, attr_name, &ntype, &count);
    CHECK(status, FAIL, "SDattrinfo");
    VERIFY(count, ATTR1_LEN, "SDattrinfo");
    VERIFY(strncmp(attr_name, ATTR1_NAME, ATTR1_LEN), 0, "SDattrinfo");

    /* Read and verify the values of the dimension's first attribute. */
    status = SDreadattr(dim_id, 0, attr_values);
    CHECK(status, FAIL, "SDreadattr");

    if (strncmp(attr_values, ATTR1_VAL, ATTR1_LEN) != 0) {
        fprintf(stderr, "Unmatched attribute values for dimension %s: is <%s>, should be <%s>\n", VAR1_NAME,
                attr_values, ATTR1_VAL);
        num_errs++;
    }

    /* Close dataset and file. */
    status = SDendaccess(sds_id);
    CHECK(status, FAIL, "SDendaccess");

    status = SDend(file_id);
    CHECK(status, FAIL, "SDend");

    /* Return the number of errors that's been kept track of so far */
    return num_errs;
}

#define FILE1        "test1.hdf"
#define FILE2        "test2.hdf"
#define DSALPHA      "DataSetAlpha"
#define DSBETA       "DataSetBeta"
#define DSGAMMA      "DataSetGamma"
#define DSBOGUS      "BogusDataSet"
#define MYDIM        "MyDim"
#define DIMATTRCHAR  "DimensionAttribute Char"
#define DIMATTRFLOAT "DimensionAttribute Float"
#define DIMATTRINT   "DimensionAttribute Integer"
#define DIMATTRUINT  "DimensionAttribute Unsigned Integer"
#define DIMLABEL     "TheLabel"
#define DIMUNIT      "TheUnits"
#define DIMFORMAT    "TheFormat"
#define DIMCOORD     "TheCordsys"

static int
test_attribute_ops()
{
    int32    fid1, fid2;               /* File handles */
    int32    nt;                       /* Number type */
    int32    dimsize[10];              /* dimension sizes */
    int32    newsds, newsds2, newsds3; /* SDS handles */
    int32    dimid, dimid2;            /* Dimension handles */
    int32    num_sds;                  /* number of SDS in file */
    int32    num_gattr;                /* Number of global attributes */
    int32    index;                    /* Index of dataset in file */
    int32    ival;
    int      status; /* status flag */
    char     name[90];
    char     text[256];
    int32    start[10], end[10], stride[10]; /* start, end, stride arrays */
    int32    scale[10];
    char     l[80], u[80], fmt[80], c[80];
    int32    count = 0;
    int32    idata[100];
    int16    sdata[100], outdata[100];
    uint8    iuval;
    float32 *data = NULL;
    float32  max, min, imax, imin;
    float64  cal, cale, ioff, ioffe;
    int      num_errs = 0; /* number of errors so far */

    ncopts = NC_VERBOSE;

    data = (float32 *)calloc(1000, sizeof(float32));
    CHECK_ALLOC(data, "data", "main");

    /* Create two files */
    fid1 = SDstart(FILE1, DFACC_CREATE);
    CHECK(fid1, FAIL, "SDstart");

    fid2 = SDstart(FILE2, DFACC_CREATE);
    CHECK(fid2, FAIL, "SDstart");

    /* Verify that the file doesn't contain any data sets or any file attributes */
    status = SDfileinfo(fid1, &num_sds, &num_gattr);
    CHECK(status, FAIL, "SDfileinfo");

    if (num_sds != 0 || num_gattr != 0) {
        fprintf(stderr, "File %s should be empty but is not\n", FILE1);
        num_errs++;
    }

    /* Create dataset DSALPHA in file FILE1 */
    dimsize[0] = 4;
    dimsize[1] = 8;
    newsds     = SDcreate(fid1, DSALPHA, DFNT_FLOAT32, 2, dimsize);
    CHECK(newsds, FAIL, "SDcreate: Failed to create data set DSALPHA");

    /* Create data set DSGAMMA in file FILE1 */
    newsds3 = SDcreate(fid1, DSGAMMA, DFNT_FLOAT64, 1, dimsize);
    CHECK(newsds3, FAIL, "SDcreate: Failed to create data set DSGAMMA");

    /* Get info on number of data sets and global attributes in file */
    status = SDfileinfo(fid1, &num_sds, &num_gattr);
    CHECK(status, FAIL, "SDfileinfo");

    if (num_sds != 2) {
        fprintf(stderr, "Wrong number of data sets in FILE1\n");
        num_errs++;
    }
    if (num_gattr != 0) {
        fprintf(stderr, "Wrong number of file attributes in FILE1\n");
        num_errs++;
    }

    /* Get the first dimension of data set DSGAMMA */
    dimid = SDgetdimid(newsds3, 0);
    CHECK(dimid, FAIL, "SDgetdimid: Failed to get dimension id");

    /* Reset the dimension name to MYDIM */
    status = SDsetdimname(dimid, MYDIM);
    CHECK(status, FAIL, "SDsetdimname: Failed to change a dimension name");

    /* Add attribute DIMATTRCHAR to this dimension */
    status = SDsetattr(dimid, DIMATTRCHAR, DFNT_CHAR8, 4, "TRUE");
    CHECK(status, FAIL, "SDsetattr: Failed to set dimension attribute");

    /* Look up the attribute DIMATTRCHAR */
    status = SDfindattr(dimid, DIMATTRCHAR);
    if (status != 0) {
        fprintf(stderr, "SDfindattr: Unable to find attribute DIMATTRCHAR\n");
        num_errs++;
    }

    /* Get the info about the first attribute of this dimension  */
    status = SDattrinfo(dimid, (int32)0, name, &nt, &count);
    CHECK(status, FAIL, "SDattrinfo");

    /* Read first attribute in, assume CHAR here. */
    status = SDreadattr(dimid, 0, text);
    CHECK(status, FAIL, "SDreadattr");

    /* Compare value retrieved to what was written */
    if (strncmp(text, "TRUE", count)) {
        fprintf(stderr, "SDreadattr: Invalid dimension attribute read <%s>\n", text);
        num_errs++;
    }

    /* Get the first dimension of data sets DSALPHA */
    dimid = SDgetdimid(newsds, 0);
    CHECK(dimid, FAIL, "SDgetdimid: Failed to get dimension id");

    /* Set the name of this dimension to MYDIM */
    status = SDsetdimname(dimid, MYDIM);
    CHECK(status, FAIL, "SDsetdimname: Failed to change a dimension name");

    /* Set the scales for this dimension also */
    scale[0] = 1;
    scale[1] = 5;
    scale[2] = 7;
    scale[3] = 24;
    status   = SDsetdimscale(dimid, 4, DFNT_INT32, (void *)scale);
    CHECK(status, FAIL, "SDsetdimscale");

    /* Set the dimension strings for the dimension also */
    status = SDsetdimstrs(dimid, DIMLABEL, NULL, DIMFORMAT);
    CHECK(status, FAIL, "SDsetdimstrs");

    /* Verify that we can read the dimension's values with SDreaddata */
    start[0] = 0;
    end[0]   = 4;
    status   = SDreaddata(dimid, start, NULL, end, (void *)idata);
    CHECK(status, FAIL, "SDreaddata");

    /* Compare retrieved values for scale */
    for (int i = 0; i < 4; i++) {
        if (idata[i] != scale[i]) {
            fprintf(stderr, "SDreaddata() returned %ld, not %ld in location %d\n", (long)idata[i],
                    (long)scale[i], i);
            num_errs++;
        }
    }

    /* Add attribute DIMATTRFLOAT to this dimension */
    max    = 3.1415F;
    status = SDsetattr(dimid, DIMATTRFLOAT, DFNT_FLOAT32, 1, (void *)&max);
    CHECK(status, FAIL, "SDsetattr");

    /* Get the info about this attribute.  Note that there are four
       attributes so far: DIMATTRCHAR, DIMLABEL, DIMFORMAT, and DIMATTRFLOAT */
    status = SDattrinfo(dimid, 3, name, &nt, &count);
    CHECK(status, FAIL, "SDattrinfo");

    if (nt != DFNT_FLOAT32) {
        fprintf(stderr, "Wrong number type for SDattrinfo(dim)\n");
        num_errs++;
    }

    if (count != 1) {
        fprintf(stderr, "Wrong count for SDattrinfo(dim)\n");
        num_errs++;
    }

    if (strcmp(name, DIMATTRFLOAT)) {
        fprintf(stderr, "Wrong name for SDattrinfo(dim)\n");
        num_errs++;
    }

    /* Get the second dimension of data set DSALPHA */
    dimid2 = SDgetdimid(newsds, 1);
    CHECK(dimid2, FAIL, "SDgetdimid: Failed to get second dimension id");

    /* Lets store an attribute for the dimension without explicitly
       creating the coord var first */
    ival   = -256;
    status = SDsetattr(dimid2, DIMATTRINT, DFNT_INT32, 1, (void *)&ival);
    CHECK(status, FAIL, "SDsetattr");

    /* Get the info about this attribute */
    status = SDattrinfo(dimid2, 0, name, &nt, &count);
    CHECK(status, FAIL, "SDattrinfo");

    if (nt != DFNT_INT32) {
        fprintf(stderr, "Wrong number type for SDattrinfo(dim)\n");
        num_errs++;
    }

    if (count != 1) {
        fprintf(stderr, "Wrong count for SDattrinfo(dim)\n");
        num_errs++;
    }

    if (strcmp(name, DIMATTRINT)) {
        fprintf(stderr, "Wrong name for SDattrinfo(dim)\n");
        num_errs++;
    }

    /* Read dimension attribute back in */
    ival   = 0;
    status = SDreadattr(dimid2, 0, (void *)&ival);
    CHECK(status, FAIL, "SDreatattr");

    if (ival != -256) {
        fprintf(stderr, "Wrong value for SDreadattr(dim)\n");
        num_errs++;
    }

    /* Add an unsigned integer as an dimension attribute */
    iuval  = 253;
    status = SDsetattr(dimid2, DIMATTRUINT, DFNT_UINT8, 1, (void *)&iuval);
    CHECK(status, FAIL, "SDsetattr");

    /* Get the info about this attribute */
    status = SDattrinfo(dimid2, 1, name, &nt, &count);
    CHECK(status, FAIL, "SDattrinfo");

    if (nt != DFNT_UINT8) {
        fprintf(stderr, "Wrong number type for SDattrinfo(dim)\n");
        num_errs++;
    }

    if (count != 1) {
        fprintf(stderr, "Wrong count for SDattrinfo(dim)\n");
        num_errs++;
    }

    if (strcmp(name, DIMATTRUINT)) {
        fprintf(stderr, "Wrong name for SDattrinfo(dim)\n");
        num_errs++;
    }

    /* Read second dimension attribute back in */
    iuval  = 0;
    status = SDreadattr(dimid2, 1, (void *)&iuval);
    CHECK(status, FAIL, "SDreatattr");

    if (iuval != 253) {
        fprintf(stderr, "Wrong value for SDreadattr(dim)\n");
        num_errs++;
    }

    /* Find the index of data set DSALPHA in file FILE1 */
    index = SDnametoindex(fid1, DSALPHA);
    if (index != 0) {
        fprintf(stderr, "Couldn't find the first data set DSALPHA in FILE1\n");
        num_errs++;
    }

    /* Try finding data set in FILE2, should fail */
    index = SDnametoindex(fid2, DSALPHA);
    if (index != FAIL) {
        fprintf(stderr, "Found data set in wrong file\n");
        num_errs++;
    }

    /* Try finding non-existent data sets in file, should fail */
    index = SDnametoindex(fid1, DSBOGUS);
    if (index != FAIL) {
        fprintf(stderr, "Found bogus data set in file 1\n");
        num_errs++;
    }

    /* Set fill value for data set DSALPHA */
    max    = -17.5;
    status = SDsetfillvalue(newsds, (void *)&max);
    CHECK(status, FAIL, "SDsetfillvalue");

    /* Initialize array to write out */
    for (int i = 0; i < 10; i++)
        data[i] = (float32)i;

    /* write out (1,1)->(3,3) array out */
    start[0] = start[1] = 1;
    end[0] = end[1] = 3;
    status          = SDwritedata(newsds, start, NULL, end, (void *)data);
    CHECK(status, FAIL, "SDwritedata");

    /* Set the range for data set DSALPHA */
    max    = 10.0F;
    min    = 4.6F;
    status = SDsetrange(newsds, (void *)&max, (void *)&min);
    CHECK(status, FAIL, "SDsetrange");

    /* Brilliant...., retrieve it right back....*/
    status = SDgetrange(newsds, (void *)&imax, (void *)&imin);
    CHECK(status, FAIL, "SDsetrange");

    /* Set a character attribute for data set DSALPHA */
    status = SDsetattr(newsds, "spam", DFNT_CHAR8, 6, "Hi mom");
    CHECK(status, FAIL, "SDsetattr");

    /* Set the data strings for data set DSALPHA */
    status = SDsetdatastrs(newsds, DIMLABEL, DIMUNIT, NULL, DIMCOORD);
    CHECK(status, FAIL, "SDsetdatastrs");

    /* Brilliant.....retrieve them right back */
    status = SDgetdatastrs(newsds, l, u, fmt, c, 80);
    CHECK(status, FAIL, "SDgetdatastrs");

    if (strcmp(l, DIMLABEL)) {
        fprintf(stderr, "Bogus label returned (%s)\n", l);
        num_errs++;
    }
    if (strcmp(u, DIMUNIT)) {
        fprintf(stderr, "Bogus units returned (%s)\n", u);
        num_errs++;
    }
    if (strcmp(fmt, "")) {
        fprintf(stderr, "Bogus format returned\n");
        num_errs++;
    }
    if (strcmp(c, DIMCOORD)) {
        fprintf(stderr, "Bogus cordsys returned\n");
        num_errs++;
    }

    /* Retrieve CHAR attribute for DSALPHA */
    status = SDfindattr(newsds, "spam");
    if (status != 2) {
        fprintf(stderr, "Bad index for SDfindattr\n");
        num_errs++;
    }

    /* Retrieve non-existent CHAR attribute for DSALPHA.
       Should fail. */
    status = SDfindattr(newsds, "blarf");
    if (status != FAIL) {
        fprintf(stderr, "SDfindattr found non-existent attribute\n");
        num_errs++;
    }

    /* Set global attributes for file FILE1 */
    status = SDsetattr(fid1, "F-attr", DFNT_CHAR8, 10, "globulator");
    CHECK(status, FAIL, "SDsetattr");

    /* Get info about the global attribute just created */
    status = SDattrinfo(fid1, (int32)0, name, &nt, &count);
    CHECK(status, FAIL, "SDattrinfo");

    /* Read this global attribute back in and verify the values */
    status = SDreadattr(fid1, 0, text);
    CHECK(status, FAIL, "SDreadattr");

    if (strncmp(text, "globulator", count)) {
        fprintf(stderr, "Invalid global attribute read <%s>\n", text);
        num_errs++;
    }

    /* Get number of SDS and global attributes in file FILE2.
       The file should be empty */
    status = SDfileinfo(fid2, &num_sds, &num_gattr);
    if (num_sds != 0) {
        fprintf(stderr, "File2 should be empty but contains %d data sets\n", num_sds);
        num_errs++;
    }
    if (num_gattr != 0) {
        fprintf(stderr, "File2 should be empty but contains %d global attributes\n", num_gattr);
        num_errs++;
    }

    /* Set calibration info for dataset DSGAMMA in file FILE1 */
    cal    = 1.0;
    cale   = 5.0;
    ioff   = 3.0;
    ioffe  = 2.5;
    nt     = DFNT_INT8;
    status = SDsetcal(newsds3, cal, cale, ioff, ioffe, nt);
    CHECK(status, FAIL, "SDsetcal");

    /* Create a record variable in file FILE2 */
    dimsize[0] = SD_UNLIMITED;
    dimsize[1] = 6;
    newsds2    = SDcreate(fid2, DSBETA, DFNT_INT16, 2, dimsize);
    CHECK(newsds2, FAIL, "SDcreate: Failed to create new data set DSBETA");

    /* Get info on number of SDSs and global attributes in file FILE2.
       There should be only 1 SDS */
    status = SDfileinfo(fid2, &num_sds, &num_gattr);
    if (num_sds != 1) {
        fprintf(stderr, "Wrong number of datasets in file 2\n");
        num_errs++;
    }
    if (num_gattr != 0) {
        fprintf(stderr, "There should be no global attributes but it is %d\n", num_gattr);
        num_errs++;
    }

    for (int16 i = 0; i < 50; i++)
        sdata[i] = i;

    /* Write data to dataset DSBETA in file FILE2 */
    start[0] = start[1] = 0;
    end[0]              = 8;
    end[1]              = 6;
    status              = SDwritedata(newsds2, start, NULL, end, (void *)sdata);
    CHECK(status, FAIL, "SDwritedata");

    /* Now read part of an earlier dataset, DSALPHA, back in from file FILE1 */
    start[0] = start[1] = 0;
    end[0] = end[1] = 3;
    status          = SDreaddata(newsds, start, NULL, end, (void *)data);
    CHECK(status, FAIL, "SDreaddata");

    /* Verify the data values retrieved */
    if (!H4_FLT_ABS_EQUAL(data[0], -17.5F)) {
        fprintf(stderr, "Wrong value returned loc 0: %f\n", (double)data[0]);
        num_errs++;
    }
    if (!H4_FLT_ABS_EQUAL(data[3], -17.5F)) {
        fprintf(stderr, "Wrong value returned loc 3: %f\n", (double)data[3]);
        num_errs++;
    }
    if (!H4_FLT_ABS_EQUAL(data[5], 1.0F)) {
        fprintf(stderr, "Wrong value returned loc 5: %f\n", (double)data[5]);
        num_errs++;
    }
    if (!H4_FLT_ABS_EQUAL(data[6], -17.5F)) {
        fprintf(stderr, "Wrong value returned loc 6: %f\n", (double)data[6]);
        num_errs++;
    }
    if (!H4_FLT_ABS_EQUAL(data[8], 4.0F)) {
        fprintf(stderr, "Wrong value returned loc 8: %f\n", (double)data[8]);
        num_errs++;
    }

    for (int i = 0; i < 50; i++)
        outdata[i] = 0;

    /* Read data back in from DSBETA in file FILE2 */
    start[0] = start[1] = 1;
    end[0]              = 3;
    end[1]              = 3;
    stride[0]           = 2;
    stride[1]           = 2;
    status              = SDreaddata(newsds2, start, stride, end, (void *)outdata);
    CHECK(status, FAIL, "SDreaddata");

    {              /* verify read values; should be
                 7 9 11 19 21 23 31 33 35 */
        int i, j;  /* indexing the two dimensions */
        int k, m;  /* counters = number of elements read on each dimension */
        int n = 0; /* indexing the outdata array */
        for (i = 1, m = 0; m < 3; i = i + 2, m++)
            for (j = (i * 6) + 1, k = 0; k < 3; j = j + 2, k++, n++) {
                if (n < 10) /* number of elements read is 9 */
                    if (outdata[n] != sdata[j]) {
                        fprintf(stderr, "line %d, wrong value: should be %d, got %d\n", __LINE__, sdata[j],
                                outdata[n]);
                        num_errs++;
                    }
            }
    }

    /* why do we set calibration info and then use SDgetcal()
       on dataset DSGAMMA ? */
    cal    = 1.0;
    cale   = 5.0;
    ioff   = 3.0;
    ioffe  = 2.5;
    nt     = DFNT_INT8;
    status = SDgetcal(newsds3, &cal, &cale, &ioff, &ioffe, &nt);
    CHECK(status, FAIL, "SDgetcal");

    /* Verify calibration data for data set DSGAMMA */
    if (!H4_DBL_ABS_EQUAL(cal, 1.0)) {
        fprintf(stderr, "Wrong calibration info\n");
        num_errs++;
    }

    if (!H4_DBL_ABS_EQUAL(cale, 5.0)) {
        fprintf(stderr, "Wrong calibration info\n");
        num_errs++;
    }

    if (!H4_DBL_ABS_EQUAL(ioff, 3.0)) {
        fprintf(stderr, "Wrong calibration info\n");
        num_errs++;
    }

    if (!H4_DBL_ABS_EQUAL(ioffe, 2.5)) {
        fprintf(stderr, "Wrong calibration info\n");
        num_errs++;
    }

    if (nt != DFNT_INT8) {
        fprintf(stderr, "Wrong calibration info\n");
        num_errs++;
    }

    /* end access to data set DSALPHA */
    status = SDendaccess(newsds);
    CHECK(status, FAIL, "SDendaccess");

    /* end access to data set DSBETA */
    status = SDendaccess(newsds2);
    CHECK(status, FAIL, "SDendaccess");

    /* end access to data set DSGAMMA */
    status = SDendaccess(newsds3);
    CHECK(status, FAIL, "SDendaccess");

    /* Close access to file FILE1 */
    status = SDend(fid1);
    CHECK(status, FAIL, "SDend");

    /* Close access to file 'test2.hdf' */
    status = SDend(fid2);
    CHECK(status, FAIL, "SDend");

    /* Return the number of errors that's been kept track of so far */
    return num_errs;
}

/* Test driver for testing SD attributes. */
extern int
test_attributes()
{
    int num_errs = 0; /* number of errors */

    /* Output message about test being performed */
    TESTING("various setting attribute features (tattributes.c)");

    /* test when count is passed into SDsetattr as 0 */
    num_errs = num_errs + test_count();

    /* test various operations on attributes */
    num_errs = num_errs + test_attribute_ops();

    if (num_errs == 0)
        PASSED();
    else
        H4_FAILED();

    /* Return the number of errors that's been kept track of so far */
    return num_errs;
}
