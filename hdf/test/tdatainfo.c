/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Copyright by The HDF Group.                                               *
 *                                                                           *
 * This file is part of HDF.  The full HDF copyright notice, including       *
 * terms governing use, modification, and redistribution, is contained in    *
 * the files COPYING and Copyright.html.  COPYING can be found at the root   *
 * of the source code distribution tree; Copyright.html can be found at      *
 * http://hdfgroup.org/products/hdf4/doc/Copyright.html.  If you do not have *
 * access to either file, you may request a copy from help@hdfgroup.org.     *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/* $Id: tdatainfo.c 5334 2010-01-28 06:10:50Z bmribler $ */

#include "hdf.h"

#define DATAINFO_MASTER
#include "hdatainfo.h"
#include "tdatainfo.h"
#include "tproto.h"

/************ Draft - still need a little more of clean up ***********/

/***********************************************************************
  NOTE: At this time, many of the offsets in these tests are verified
	by hard-coding.  When adding new data to the files in exiting
	tests, please either add data at the bottom of the files or
	change the hard-coded offsets appropriately, using the values
	from the error messages.  However, you'll need to verify the
	correctness of these values first using the command 'od' on
	the hdf file.
*************************************************************************/

/*********** Functions to access t_hdfdatainfo_t **********************/

/* alloc_info is a utility function that allocates hdf_datainfo_t's members*/
intn alloc_info(t_hdf_datainfo_t *info, uintn info_count)
{
    info->offsets = (int32 *) HDmalloc(info_count * sizeof(int32));
    if (info->offsets == NULL)
	return -1;
    info->lengths = (int32 *) HDmalloc(info_count * sizeof(int32));
    if (info->lengths == NULL)
	return -1;
    return 0;
}

void free_info(t_hdf_datainfo_t *info)
{
    if (info != NULL)
    {
	if (info->offsets != NULL)
	    HDfree(info->offsets);
	if (info->lengths != NULL)
	    HDfree(info->lengths);
    }
}

/****************************************************************************
   Name: test_simple_vs() - tests non-special Vdatas

   Description:
	This routine creates and writes contiguous data to vdatas and verifies
        some data and data information with VSgetdatainfo.  The tests include
        the following vdatas:
	- a single field vdata with int32 values
	- a multiple field/multiple order vdata with all int32 values
	- a single field vdata with character values
	Only the character vdata's values are verified against the original
	buffer.  The other two were verified by examining the hdf file using
	od. (Todo 1)
   BMR - Jul 2010
 ****************************************************************************/
#define SIMPLE_FILE	"datainfo_simple.hdf"   /* data file */
#define NONSPECIAL_VS	"Non-Special Vdata"
#define CONTCLASS_NAME	"Contiguous Vdata"
#define LINKED_BLOCK	"Linked-block Vdata"
#define FIELD1_NAME	"Field1"	/* contains three integers */
#define FIELD2_NAME	"Field2"	/* contains one integer */
#define FIELD3_NAME	"Field3"	/* contains two integers */
#define	FIELD_NAME_LIST	"Field1,Field2,Field3"
#define ORDER_1 	3	/* order of first field of 1st vdata */
#define ORDER_2 	1	/* order of second field of 1st vdata */
#define ORDER_3 	2	/* order of third field of 1st vdata */
#define	BLOCK_SIZE	80	/* arbitrary number for block size */
#define	NUM_BLOCKS	8	/* arbitrary number for number of blocks */
#define N_VALS_PER_REC_1   1    /* # of values per record in single vdata */
#define N_VALS_PER_REC_2 (ORDER_1+ORDER_2+ORDER_3) /* # of vals/rec. in multiple vdata */
#define N_RECORDS	5	/* number of records to be written to the
				   vdatas at every write */
static void
test_simple_vs(void)
{
    int32 fid,		/* File ID */
	  vsid,		/* Vdata ID */
	  vs_ref,	/* Vdata ref# */
	  n_records,	/* number of records actually written to vdata */
	  data_buf0[N_RECORDS][N_VALS_PER_REC_1], /* for single vdata's data */
	  data_buf1[N_RECORDS][N_VALS_PER_REC_2]; /* for multiple vdata's data*/
    char  data_buf2[N_RECORDS] = {'H', '4', 'M', 'A', 'P'};
    int16 rec_num;	/* current record number */
    int32 offset, length; /* offset/length buffers for single block of data */
    intn  n_blocks;	/* number of blocks a vdata has */
    t_hdf_datainfo_t vs_info; /* data structure to hold offset/length arrays and
	some other information about the data */
    int32 status;	/* Status values from routines */
    intn status_n;	/* Status values from routines */

    /* Open the HDF file and initialize the interface. */
    fid = Hopen(SIMPLE_FILE, DFACC_CREATE, 0);
    CHECK_VOID(fid,FAIL,"Hopen");
    status = Vstart(fid);
    CHECK_VOID(status,FAIL,"Vstart");

    /* Create and write to the first vdata using high-level func VHstoredata */
    for (rec_num = 0; rec_num < N_RECORDS; rec_num++)
	data_buf0[rec_num][0] = 5 + rec_num;

    vs_ref = VHstoredata(fid, "Only field", (const uint8 *)data_buf0, N_RECORDS, DFNT_INT32, "One Field One Order", CONTCLASS_NAME);

    /*
     * Get access to the vdata just created then get and verify offset/length
     * of its data
     */

    vsid = VSattach (fid, vs_ref, "w");
    CHECK_VOID(vsid, FAIL, "VSattach");

    /* Get the number of data blocks first, should be 1 */
    n_blocks = VSgetdatainfo(vsid, 0, 0, NULL, NULL);
    CHECK_VOID(n_blocks, FAIL, "VSgetdatainfo");
    VERIFY_VOID(n_blocks, 1, "VSgetdatainfo");

    /* Get the offset/length of the data, should be 294/20 */
    n_blocks = VSgetdatainfo(vsid, 0, n_blocks, &offset, &length);
    CHECK_VOID(n_blocks, FAIL, "VSgetdatainfo");

    /* Verify offset/length */
    VERIFY_VOID(offset, 294, "VSgetdatainfo offset from 'One Field One Order'");
    VERIFY_VOID(length, 20, "VSgetdatainfo length from 'One Field One Order'");

    status = VSdetach (vsid);
    CHECK_VOID(status, FAIL, "Vdetach");

    /*
     * Create a multi-field/multi-order vdata, named NONSPECIAL_VS, in class
     * CONTCLASS_NAME, write data to it, then get and verify its data's
     * offsets/lengths
     */
    vsid = VSattach (fid, -1, "w");
    CHECK_VOID(vsid, FAIL, "VSattach");

    /* Set name and class name of the vdata. */
    status = VSsetname (vsid, NONSPECIAL_VS);
    CHECK_VOID(status, FAIL, "VSsetname");
    status = VSsetclass (vsid, CONTCLASS_NAME);
    CHECK_VOID(status, FAIL, "VSsetclass");

    /* Record its reference number for later access before detaching it */
    vs_ref = VSQueryref(vsid);
    CHECK_VOID(vs_ref, FAIL, "VSQueryref:vsid");
    status = VSdetach (vsid);
    CHECK_VOID(status, FAIL, "Vdetach");

    /* Attach to vdata NONSPECIAL_VS to write data, but first verify that
	number of data blocks is 0 */
    vsid = VSattach (fid, vs_ref, "w");
    CHECK_VOID(vsid, FAIL, "VSattach");

    n_blocks = VSgetdatainfo(vsid, 0, 0, NULL, NULL);
    CHECK_VOID(n_blocks, FAIL, "VSgetdatainfo");
    VERIFY_VOID(n_blocks, 0, "VSgetdatainfo");

    /* Define the fields in the vdata */
    status_n = VSfdefine (vsid, FIELD1_NAME, DFNT_INT32, ORDER_1);
    CHECK_VOID(status_n, FAIL, "VSfdefine");
    status_n = VSfdefine (vsid, FIELD2_NAME, DFNT_INT32, ORDER_2);
    CHECK_VOID(status_n, FAIL, "VSfdefine");
    status_n = VSfdefine (vsid, FIELD3_NAME, DFNT_INT32, ORDER_3);
    CHECK_VOID(status_n, FAIL, "VSfdefine");
    status_n = VSsetfields (vsid, FIELD_NAME_LIST);
    CHECK_VOID(status_n, FAIL, "VSsetfields");

    /* 
     * Buffer the data by the record for fully interlaced mode.  Note that the
     * first three elements contain the three values of the first field, the
     * fourth element contains the value of the second field, and the last two
     * elements contain the two values of the third field.
     */
    for (rec_num = 0; rec_num < N_RECORDS; rec_num++)
    {
        data_buf1[rec_num][0] = 2;	/* easier to inspect the bin file */
        data_buf1[rec_num][1] = 2;
        data_buf1[rec_num][2] = 2;
        data_buf1[rec_num][3] = 2;
        data_buf1[rec_num][4] = 2;
        data_buf1[rec_num][5] = 2;
        /*  data_buf1[rec_num][0] = 1 + rec_num;
        data_buf1[rec_num][1] = 2 + rec_num;
        data_buf1[rec_num][2] = 3 + rec_num;
        data_buf1[rec_num][3] = 10 + rec_num;
        data_buf1[rec_num][4] = 10;
        data_buf1[rec_num][5] = 65;
 */ 
    }

    /* Set the block size and the number of blocks */
    status_n = VSsetblocksize(vsid, BLOCK_SIZE);
    CHECK_VOID(status_n, FAIL, "VSsetblocksize");
    status_n = VSsetnumblocks(vsid, NUM_BLOCKS);
    CHECK_VOID(status_n, FAIL, "VSsetnumblocks");

    /* Write the data from data_buf1 to the non special vdata */
    n_records = VSwrite(vsid, (uint8 *)data_buf1, N_RECORDS, FULL_INTERLACE);
    VERIFY_VOID(n_records, N_RECORDS, "VSwrite:vsid");

    n_blocks = VSgetdatainfo(vsid, 0, 0, NULL, NULL);
    CHECK_VOID(n_blocks, FAIL, "VSgetdatainfo");
    VERIFY_VOID(n_blocks, 1, "VSgetdatainfo:vsid");

    n_blocks = VSgetdatainfo(vsid, 0, n_blocks, &offset, &length);
    CHECK_VOID(n_blocks, FAIL, "VSgetdatainfo");

    /* Verify offset/length */
    VERIFY_VOID(offset, 456, "VSgetdatainfo offset from NONSPECIAL_VS");
    VERIFY_VOID(length, 120, "VSgetdatainfo length from NONSPECIAL_VS");

    status_n = VSdetach (vsid);
    CHECK_VOID(status, FAIL, "Vdetach");

    /* Create and write to another simple vdata, named 'Characters Only', in
	class CONTCLASS_NAME */
    vs_ref = VHstoredata(fid, "Only field", (const uint8 *)data_buf2,
		 N_RECORDS, DFNT_CHAR, "Characters Only", CONTCLASS_NAME); 


    /* Attach to vdata 'Characters Only' and get offset and length of its data */
    vsid = VSattach(fid, vs_ref, "r");

    n_blocks = VSgetdatainfo(vsid, 0, 0, NULL, NULL);
    CHECK_VOID(n_blocks, FAIL, "VSgetdatainfo");
    VERIFY_VOID(n_blocks, 1, "VSgetdatainfo NONSPECIAL_VS");

    /* Allocate space to record the vdata's data info */
    alloc_info(&vs_info, n_blocks);

    /* Record various info */
    vs_info.n_values = 5;
    vs_info.numtype = DFNT_CHAR;

    /* Get offset/length */
    n_blocks = VSgetdatainfo(vsid, 0, n_blocks, vs_info.offsets, vs_info.lengths);
    CHECK_VOID(n_blocks, FAIL, "VSgetdatainfo");

    /* Close everything */
    status = VSdetach (vsid);
    CHECK_VOID(status, FAIL, "Vdetach");

    status_n = Vend(fid);
    CHECK_VOID(status,FAIL,"Vend");

    status_n = Hclose(fid);
    CHECK_VOID(status,FAIL,"Hclose");

    /* Open the file with fopen, read data at the offset obtained and verify
       the values */
    status_n = readnoHDF_char(SIMPLE_FILE, &vs_info, data_buf2);
    if (status_n == FAIL)
	fprintf(stderr, "Attempt reading data without HDF4 library failed at line %d\n", __LINE__);
} /* test_simple_vs() */

#define APPEND_FILE	"datainfo_linkblock.hdf"   /* data file */
#define APPENDABLE_VD	"Appendable Vdata"
static void
test_append_vs()
{
    int32 fid;       /* file ID */
    int32 apvsid;    /* vdata IDs */
    int32 vs1_ref,   /* vdata ref# */
   	 n_records,  /* number of records written to vdata */
         data_buf0[N_RECORDS][N_VALS_PER_REC_1], /* for "Very Simple Vdata" */
         data_buf1[N_RECORDS][N_VALS_PER_REC_2]; /* for first vdata's data */
    int16 rec_num;      /* current record number */
    intn n_blocks;
    t_hdf_datainfo_t vs_info;
    int32 status;       /* Status values from routines */
    intn status_n;       /* Status values from routines */

    /* Open the HDF file. */
    fid = Hopen(APPEND_FILE, DFACC_CREATE, 0);
    CHECK_VOID(fid,FAIL,"Hopen");

    /* Initialize HDF for subsequent vgroup/vdata access. */
    status_n = Vstart(fid);
    CHECK_VOID(status_n,FAIL,"Vstart");

    /* Create the first vdata */
    apvsid = VSattach (fid, -1, "w");
    CHECK_VOID(apvsid, FAIL, "VSattach");

    vs1_ref = VSQueryref(apvsid);
    CHECK_VOID(vs1_ref, FAIL, "VSQueryref:apvsid");

    status = VSdetach (apvsid);
    CHECK_VOID(status, FAIL, "Vdetach");

    apvsid = VSattach (fid, vs1_ref, "w");
    CHECK_VOID(apvsid, FAIL, "VSattach");

    /* Set name and class name of the vdata. */
    status = VSsetname (apvsid, APPENDABLE_VD);
    CHECK_VOID(status, FAIL, "VSsetname");
    status = VSsetclass (apvsid, LINKED_BLOCK);
    CHECK_VOID(status, FAIL, "VSsetclass");

    n_blocks = VSgetdatainfo(apvsid, 0, 0, NULL, NULL);
    CHECK_VOID(n_blocks, FAIL, "VSgetdatainfo");
    VERIFY_VOID(n_blocks, 0, "VSgetdatainfo");

    /* Introduce each field's name, data type, and order.  This is the first
      part in defining a field.  */
    status_n = VSfdefine (apvsid, FIELD1_NAME, DFNT_INT32, ORDER_1);
    CHECK_VOID(status_n, FAIL, "VSfdefine");
    status_n = VSfdefine (apvsid, FIELD2_NAME, DFNT_INT32, ORDER_2);
    CHECK_VOID(status_n, FAIL, "VSfdefine");
    status_n = VSfdefine (apvsid, FIELD3_NAME, DFNT_INT32, ORDER_3);
    CHECK_VOID(status_n, FAIL, "VSfdefine");

    /* Finalize the definition of the fields. */
    status_n = VSsetfields (apvsid, FIELD_NAME_LIST);
    CHECK_VOID(status_n, FAIL, "VSsetfields");

    /* 
     * Buffer the data by the record for fully interlaced mode.  Note that the
     * first three elements contain the three values of the first field, the
     * fourth element contains the value of the second field, and the last two
     * elements contain the two values of the third field.
     */
    for (rec_num = 0; rec_num < N_RECORDS; rec_num++)
    {
        data_buf1[rec_num][0] = 5;	/* easier to inspect the bin file */
        data_buf1[rec_num][1] = 5;
        data_buf1[rec_num][2] = 5;
        data_buf1[rec_num][3] = 5;
        data_buf1[rec_num][4] = 5;
        data_buf1[rec_num][5] = 5;

	 /* data_buf1[rec_num][0] = 1 + rec_num;
        data_buf1[rec_num][1] = 2 + rec_num;
        data_buf1[rec_num][2] = 3 + rec_num;
        data_buf1[rec_num][3] = 10 + rec_num;
        data_buf1[rec_num][4] = 10;
        data_buf1[rec_num][5] = 65;
 */ 
    }

    /* Set the block size and the number of blocks the first vdata */
    status_n = VSsetblocksize(apvsid, BLOCK_SIZE);
    CHECK_VOID(status_n, FAIL, "VSsetblocksize");
    status_n = VSsetnumblocks(apvsid, NUM_BLOCKS);
    CHECK_VOID(status_n, FAIL, "VSsetnumblocks");

    /* Write the data from data_buf1 to vdata APPENDABLE_VD the first time */
    n_records = VSwrite(apvsid, (uint8 *)data_buf1, N_RECORDS, FULL_INTERLACE);
    VERIFY_VOID(n_records, N_RECORDS, "VSwrite:apvsid");

    n_blocks = VSgetdatainfo(apvsid, 0, 0, NULL, NULL);
    CHECK_VOID(n_blocks, FAIL, "VSgetdatainfo");

    /* Allocate space to record the vdata's data info */
    alloc_info(&vs_info, n_blocks);

    n_blocks = VSgetdatainfo(apvsid, 0, n_blocks, vs_info.offsets, vs_info.lengths);
    CHECK_VOID(n_blocks, FAIL, "VSgetdatainfo");

    vs1_ref = VSQueryref(apvsid);
    CHECK_VOID(vs1_ref, FAIL, "VSQueryref:apvsid");

    {
	int32 vd2_ref;

	for (rec_num = 0; rec_num < N_RECORDS; rec_num++)
	    data_buf0[rec_num][0] = 10 + rec_num;

	/* Create and write to another very simple vdata */
	vd2_ref = VHstoredata(fid, "Field 1", (const uint8 *)data_buf0, N_RECORDS, DFNT_INT32, "Another One Field One Order", "Very Simple Vdata");
    }

    for (rec_num = 0; rec_num < N_RECORDS; rec_num++)
    {
        data_buf1[rec_num][0] = 2;	/* easier to inspect the bin file */
        data_buf1[rec_num][1] = 2;
        data_buf1[rec_num][2] = 2;
        data_buf1[rec_num][3] = 2;
        data_buf1[rec_num][4] = 2;
        data_buf1[rec_num][5] = 2;
         /* data_buf1[rec_num][0] = 100 + rec_num;
        data_buf1[rec_num][1] = 200 + rec_num;
        data_buf1[rec_num][2] = 300 + rec_num;
        data_buf1[rec_num][3] = 1000 + rec_num;
        data_buf1[rec_num][4] = 1000;
        data_buf1[rec_num][5] = 6500;
 */ 
    }
    /* Write the data to vdata APPENDABLE_VD the second time */
    n_records = VSwrite(apvsid, (uint8 *)data_buf1, N_RECORDS, FULL_INTERLACE);
    VERIFY_VOID(n_records, N_RECORDS, "VSwrite:apvsid");

    status = VSdetach (apvsid);
    CHECK_VOID(status, FAIL, "Vdetach");

    apvsid = VSattach (fid, vs1_ref, "w");
    CHECK_VOID(apvsid, FAIL, "VSattach");

    n_blocks = VSgetdatainfo(apvsid, 0, 0, NULL, NULL);
    CHECK_VOID(n_blocks, FAIL, "VSgetdatainfo");

    /* Allocate space to record the vdata's data info */
    alloc_info(&vs_info, n_blocks);

    /* Record various info */
    vs_info.n_values = 30;
    vs_info.numtype = DFNT_INT32;

    n_blocks = VSgetdatainfo(apvsid, 0, n_blocks, vs_info.offsets, vs_info.lengths);
    CHECK_VOID(n_blocks, FAIL, "VSgetdatainfo");

    {
	int ii;
	int32 check_offsets[] = {294, 556, 636};
	int32 check_lengths[] = {120, 80, 80};
	for (ii = 0; ii < n_blocks; ii++)
	{
	    VERIFY_VOID(vs_info.offsets[ii], check_offsets[ii], "VSgetdatainfo offset");
	    VERIFY_VOID(vs_info.lengths[ii], check_lengths[ii], "VSgetdatainfo length");
	}
    }

    /* Close everything. */
    status = VSdetach (apvsid);
    CHECK_VOID(status, FAIL, "Vdetach");
    status = Vend(fid);
    CHECK_VOID(status,FAIL,"Vend");
    status = Hclose(fid);
    CHECK_VOID(status,FAIL,"Hclose");
} /* test_append_vs */


/*******************************************************************
 Read and verify char data using previously obtained data info,
 without HDF4 library
********************************************************************/
intn readnoHDF_char(const char *filename, const t_hdf_datainfo_t *info, const char *orig_buf)
{
    /* Open file and read in data without using VS API */
    FILE  *fd;		/* file descriptor */
    size_t readlen=0;	/* number of bytes actually read */
    char *readcbuf;
    int ii;

    /* Open the file for reading without SD API */
    fd = fopen(filename, "r");
    if (fd == NULL)
    {
	fprintf(stderr, "readnoHDF_char: unable to open file %s", filename);
        return FAIL;
    }

    /* Forward to the position of the first block of data */
    if (fseek(fd, (off_t)info->offsets[0], SEEK_SET) == -1)
    {
        fprintf(stderr, "readnoHDF_char: unable to seek offset %d\n",
                (int)info->offsets[0]);
        return FAIL;
    }

    /* Allocate buffers for SDS' data */
    readcbuf = (char *) HDmalloc(info->n_values * sizeof(char));
    if (readcbuf == NULL)
	fprintf(stderr, "readnoHDF_char: allocation readcbuf failed\n");

    /* Read in this block of data */
    readlen = fread((void*)readcbuf, 1, info->n_values, fd);
    if (readlen > 0)
    {
        /* Compare data read without VS API against the original buffer */
        for (ii = 0; ii < readlen; ii++)
        {
	    if (readcbuf[ii] != orig_buf[ii])
	    fprintf(stderr, "At value# %d: written = %c read = %c\n",
					 ii, orig_buf[ii], readcbuf[ii]);
        }
    }
    /* Close the file */
    if (fclose(fd) == -1)
    {
	fprintf(stderr, "readnoHDF_char: unable to close file %s", SIMPLE_FILE);
        return FAIL;
    }
    return SUCCEED;
}

/* Test driver for testing the public functions VSgetdatainfo,
   VSgetattdatainfo, Vgetattdatainfo. */
void
test_datainfo(void)
{
    /* Test Vdatas where data is stored in one contiguous block */
    test_simple_vs();

    /* Test Vdatas where data might be stored in linked blocks */
    test_append_vs();
}
