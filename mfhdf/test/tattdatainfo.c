/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Copyright by The HDF Group.                                               *
 * All rights reserved.                                                      *
 *                                                                           *
 * This file is part of HDF.  The full HDF copyright notice, including       *
 * terms governing use, modification, and redistribution, is contained in    *
 * the files COPYING and Copyright.html.  COPYING can be found at the root   *
 * of the source code distribution tree; Copyright.html can be found at      *
 * http://hdfgroup.org/products/hdf4/doc/Copyright.html.  If you do not have *
 * access to either file, you may request a copy from help@hdfgroup.org.     *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/****************************************************************************
 * tdatainfo.c - tests the function SDgetattdatainfo.
 * Structure of the file:
 *    test_attdatainfo - test driver
 *	  test_nonspecial_SDSs - tests nonspecial SDSs
****************************************************************************/

 /* #include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <math.h>

#if WIN32
#define snprintf sprintf_s
#define ssize_t int32
#endif 
 */ 

#include "mfhdf.h"

#ifdef HDF

#include "hdftest.h"

#define ATTR_FILE     "attdatainfo.hdf"	/* data file */
#define X_LENGTH      10
#define Y_LENGTH      10
#define RANK          2

/* Calculates the number of values in an SDS using the dimensions and rank */
int32 comp_n_values(int32 rank, int32 *dimsizes)
{
    int ii;
    int32 n_values = 1;
    for (ii = 0; ii < rank; ii++)
	n_values = n_values * dimsizes[ii];
    return(n_values);
}

/****************************************************************************
   Name: test_attrs() - tests getting attribute's data info

   Description:
	This routine creates and writes data to non-special SDSs and verifies
	data and data information with SDgetattdatainfo.  The tests include
	the following SDSs:
	- a 2-dim 5x8 element SDS, float32, with no data
	- a 1-dim 10-element SDS, int32, with 10 values
	- a 2-dim 5x8 element SDS, float32, with 5x8 values
	- a 1-dim 20-element SDS, char, with 20 values
	SDgetattdatainfo will retrieve the number of blocks in the datasets
	and the offsets and lengths of the blocks.  Then the data will be read
	back from the file at the previously retrieved offsets/lengths, without
	the use of the HDF4 library, and will be verified against the original
	data buffers.

   BMR - Jul 2010
 ****************************************************************************/
#define SDS1_NAME	"First SDS"
#define SDS2_NAME	"Second SDS"
#define RANK1		1
#define RANK2		2
#define LENGTH1_X	10
#define LENGTH2_X	5
#define LENGTH2_Y	8
#define LENGTH3_X	21
#define FILE_ATTR_NAME "File_contents"
#define SDS_ATTR_NAME1  "Valid_range 1"
#define SDS_ATTR_NAME2  "Valid_range 2"
#define DIM_ATTR_NAME  "Dim_metric"

static intn test_attrs()
{
    int32 sd_id, sds_id, dim_id, dim_idx, att_idx;
    int32 dimsizes[2], starts[2], edges[2], rank=0;
    int32 data1[LENGTH1_X];
    float data2[LENGTH2_X][LENGTH2_Y];
    char  data3[LENGTH3_X], outdata3[LENGTH3_X];
    char  sds_name[20];
    uintn info_count = 0;
    int32 n_values, nattrs;
    int32 offset=0, length=0;
   char8   file_values[] = "Storm_track_data";
   float32 sds_values[2] = {2., 10.};
   float32 sds_values_out[2];
   char8   dim_values[]  = "Seconds";
    intn  status;
    int   ii, jj;
    intn  num_errs = 0;		/* number of errors so far */

    /* Create the file and initialize the SD interface */
    sd_id = SDstart(ATTR_FILE, DFACC_CREATE);
    CHECK(sd_id, FAIL, "test_attrs: SDstart");

    /* Set an attribute that describes the file contents. */
    n_values = 16;
    status = SDsetattr (sd_id, FILE_ATTR_NAME, DFNT_CHAR8, n_values,
                       (VOIDP)file_values);
    CHECK(status, FAIL, "SDsetattr")

    /***************************************************************
     Create and write non-special SDSs
     ***************************************************************/

    /* Create a 2x2 dataset called "EmptyDataset" */
    dimsizes[0] = LENGTH2_X;
    dimsizes[1] = LENGTH2_Y;

    /*
     * Create a 1-dim 10-element SDS, type int32, then write 10 values
     * to it
     */
    dimsizes[0] = LENGTH1_X;
    sds_id = SDcreate (sd_id, SDS1_NAME, DFNT_INT32, RANK1, dimsizes);
    CHECK(sds_id, FAIL, "SDcreate");

    for (ii=0; ii < LENGTH1_X; ii++)
	data1[ii] = 1000 * ii;

    starts[0] = 0;
    edges[0] = LENGTH1_X;
    status = SDwritedata(sds_id, starts, NULL, edges, (VOIDP)data1);
    CHECK(status, FAIL, "SDwritedata");

    status = SDendaccess(sds_id);
    CHECK(status, FAIL, "SDendaccess");

    /* Assign two attributes to the first SDS. */
    n_values  = 2;
  fprintf(stderr, "set_attr.c: SDsetattr %s \n", SDS_ATTR_NAME1);
    status = SDsetattr (sds_id, SDS_ATTR_NAME1, DFNT_FLOAT32, n_values,
                       (VOIDP)sds_values);
    CHECK(status, FAIL, "SDsetattr")
 fprintf(stderr, "done SDsetattr %s \n", SDS_ATTR_NAME1);
    n_values = 7;
    status = SDsetattr (sds_id, SDS_ATTR_NAME2, DFNT_CHAR8, n_values,
                       (VOIDP)dim_values);
    CHECK(status, FAIL, "SDsetattr")

    /* 
     * Create a 2-dim 5x8 element SDS, type float32, then write 5x8 values
     * to it
     */
    dimsizes[0] = LENGTH2_X;
    dimsizes[1] = LENGTH2_Y;
    sds_id = SDcreate (sd_id, SDS2_NAME, DFNT_FLOAT32, RANK2, dimsizes);
    CHECK(sds_id, FAIL, "SDcreate");

    for (ii=0; ii < LENGTH2_X; ii++)
	for (jj=0; jj < LENGTH2_Y; jj++)
	    data2[ii][jj] = 500.50 * (ii+jj);

    starts[0] = 0;
    starts[1] = 0;
    edges[0] = LENGTH2_X;
    edges[1] = LENGTH2_Y;
    status = SDwritedata(sds_id, starts, NULL, edges, (VOIDP)data2);
    CHECK(status, FAIL, "SDwritedata");

    /* Get the the second dimension identifier of the SDS. */
    dim_idx = 1;
    dim_id = SDgetdimid (sds_id, dim_idx);
    CHECK(dim_id, FAIL, "SDgetdimid")

    /* Set an attribute of the dimension that specifies the dimension metric. */
    n_values = 7;
    status = SDsetattr (dim_id, DIM_ATTR_NAME, DFNT_CHAR8, n_values,
                       (VOIDP)dim_values);
    CHECK(status, FAIL, "SDsetattr")

    status = SDendaccess(sds_id);
    CHECK(status, FAIL, "SDendaccess");

    /***********************************************************************
     Read data info for later accessing data without the use of HDF4 library
     ***********************************************************************/

    /* Open the second dataset, verify that number of data block is 1, then
	retrieve and record the offset/length */
    sds_id = SDselect(sd_id, 0);
    CHECK(sds_id, FAIL, "test_attrs: SDselect SDS index 1");

    /* Get SDS' rank */
    status = SDgetinfo(sds_id, sds_name, NULL, NULL, NULL, &nattrs);
    CHECK(status, FAIL, "test_attrs: SDgetinfo SDS index 0");
    VERIFY(nattrs, 2, "test_attrs: SDgetinfo SDS index 0");

     /* check_offsets[0] = ;
 */ 
    for (att_idx=0; att_idx < nattrs; att_idx++)
    {
	status = SDgetattdatainfo(sds_id, att_idx, &offset, &length);
	CHECK(status, FAIL, "test_attrs: SDgetattdatainfo");
 fprintf(stderr, "sds '%s', attribute #%d: offset = %d, length = %d\n", sds_name, att_idx, offset, length);
    }

    status = SDendaccess(sds_id);
    CHECK(status, FAIL, "SDendaccess");

    status = SDend(sd_id);
    CHECK(status, FAIL, "SDend");

    /******************************************************************
     Read data using previously obtained data info without HDF4 library
     ******************************************************************/

#if 0
    /* Open file and read in data without using SD API */
    {
	int   fd;		/* for open */
	int32 ret32;		/* for DFKconvert */
	int   ret;		/* for fabs */
        ssize_t readlen=0;	/* for read */
        int32 *readibuf, *readibuf_swapped;
        float *readfbuf, *readfbuf_swapped;
        char *readcbuf, *readcbuf_swapped;
	char readfbuf_str[12], data2_str[12]; /* for comparing readfbuf values */
        uint32 n_values;
	int ii, jj, kk;

	/* Open the file for reading without SD API */
	fd = open(ATTR_FILE, O_RDONLY);
	if (fd == -1)
	{
	    fprintf(stderr, "test_attrs: unable to open file %s", ATTR_FILE);
	    num_errs++;
            return num_errs;
	}

	/* Forward to the position of the data of SDS at index 1 */
        if (lseek(fd, (off_t)sds1_info.offsets[0], SEEK_SET) == -1)
        {
            fprintf(stderr, "test_attrs: unable to seek offset %d\n",
                (int)sds1_info.offsets[0]);
	    num_errs++;
            return num_errs;
        }

	/* Allocate buffers for SDS' data */
        readibuf = (int32 *) HDmalloc(sds1_info.n_values * sizeof(int32));
        readibuf_swapped = (int32 *) HDmalloc(sds1_info.n_values * sizeof(int32));
        /* Read in this block of data */
        readlen = read(fd, (VOIDP)readibuf, (size_t)sds1_info.lengths[0]);
	CHECK(readlen, FAIL, "DFKconvert");

	ret32 = DFKconvert(readibuf, readibuf_swapped, sds1_info.numtype,
				 (uint32)sds1_info.n_values, DFACC_WRITE, 0, 0);
	CHECK(ret32, FAIL, "DFKconvert");

	if (ret32 > 0)
	{
            /* Compare data read without SD API against the original buffer */
            for (ii = 0; ii < sds1_info.n_values; ii++)
            {
		if (readibuf_swapped[ii] != data1[ii])
		    fprintf(stderr, "At value# %d: written = %d read = %d\n",
					 ii, data1[ii], readibuf_swapped[ii]);
            }
	}

	/* Forward to the position of the data of SDS at index 2 */
        if (lseek(fd, (off_t)sds2_info.offsets[0], SEEK_SET) == -1)
        {
            fprintf(stderr, "test_attrs: unable to seek offset %d\n",
                (int)sds2_info.offsets[0]);
	    num_errs++;
            return num_errs;
        }

	/* Allocate buffers for SDS' data */
        readfbuf = (float32 *) HDmalloc(sds2_info.n_values * sizeof(float32));
        readfbuf_swapped = (float32 *) HDmalloc(sds2_info.n_values * sizeof(float32));
        /* Read in this block of data */
        readlen = read(fd, (VOIDP)readfbuf, (size_t)sds2_info.lengths[0]);
	CHECK(readlen, FAIL, "DFKconvert");

	ret32 = DFKconvert(readfbuf, readfbuf_swapped, sds2_info.numtype,
				 (uint32)sds2_info.n_values, DFACC_WRITE, 0, 0);
	CHECK(ret32, FAIL, "DFKconvert");

        /* Compare data read without SD API against the original buffer */
	kk = 0;
	for (jj = 0; jj < sds2_info.dimsizes[0]; jj++)
            for (ii = 0; ii < sds2_info.dimsizes[1]; ii++)
            {
		/* Flag if the two numbers are not close enough */
		if (fabs(readfbuf_swapped[kk] - data2[jj][ii]) > 0.00001)
		    fprintf(stderr, "At value# %d: written = %f read = %f\n",
				 ii, data2[jj][ii], readfbuf_swapped[kk]);
		if (kk < sds2_info.n_values) kk++;
            }

	/* Forward to the position of the data of SDS at index 3 */
        if (lseek(fd, (off_t)sds3_info.offsets[0], SEEK_SET) == -1)
        {
            fprintf(stderr, "test_attrs: unable to seek offset %d\n",
                (int)sds3_info.offsets[0]);
	    num_errs++;
            return num_errs;
        }

	/* Allocate buffers for SDS' data */
        readibuf = (int32 *) HDmalloc(sds3_info.n_values * sizeof(int32));
        readibuf_swapped = (int32 *) HDmalloc(sds3_info.n_values * sizeof(int32));
        /* Read in this block of data */
        readlen = read(fd, (VOIDP)readibuf, (size_t)sds3_info.lengths[0]);
	CHECK(readlen, FAIL, "DFKconvert");

	ret32 = DFKconvert(readibuf, readibuf_swapped, sds3_info.numtype,
				 (uint32)sds3_info.n_values, DFACC_WRITE, 0, 0);
	CHECK(ret32, FAIL, "DFKconvert");

	if (ret32 > 0)
	{
            /* Compare data read without SD API against the original buffer */
            for (ii = 0; ii < sds3_info.n_values; ii++)
            {
		if (readibuf_swapped[ii] != data3[ii])
		    fprintf(stderr, "At value# %d: written = %d read = %d\n",
					 ii, data3[ii], readibuf_swapped[ii]);
            }
	}

	if (close(fd) == -1)
	{
	    fprintf(stderr, "test_attrs: unable to close file %s", ATTR_FILE);
	    num_errs++;
            return num_errs;
	}
    }
    free_info(&sds1_info);
    free_info(&sds2_info);
    free_info(&sds3_info);
#endif

    /* Return the number of errors that's been kept track of so far */
    return num_errs;
} /* test_attrs */


/* Test driver for testing the public function SDgetattdatainfo. */
 /* extern int test_datainfo()
 */ 
int main()
{
    intn status;
    int num_errs = 0;

    /* Output message about test being performed */
    TESTING("getting location info of data (tdatainfo.c)");

    /* Test nonspecial SDSs */
    num_errs = num_errs + test_attrs();

    if (num_errs == 0) PASSED();
    return num_errs;
}
#endif /* HDF */
