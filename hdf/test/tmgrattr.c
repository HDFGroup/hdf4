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
 * tmgrattr.c - tests the attribute functions for the multi-file GR interface
 * Structure of the file:
 *    test_mgr_attr - test driver
 *        test_mgr_fillvalues - tests with fill value attributes
 *        test_mgr_userattr - tests with user-defined attributes
 *
****************************************************************************/

#include "hdf.h"
#include <stdio.h>
#include "tutils.h"

#include "tproto.h"
#define TESTFILE "tmgratt.hdf"

#define	F_ATT1_VAL	"Contents of First FILE Attribute"
#define	IMAGE1_NAME	"Image 1"
#define	F_ATT1_NAME	"File Attribute 1"
#define	F_ATT2_NAME	"File Attribute 2"
#define	RI_ATT1_NAME	"Image Attribute 1"
#define	RI_ATT2_NAME	"Image Attribute 2"
#define	F_ATT1_N_VALUES	32
#define	F_ATT2_N_VALUES	5
#define N_COMPS		4
#define	RI_ATT1_VAL	"Contents of IMAGE1's First Attribute"
#define	RI_ATT_N_VALUES		4
#define	RI_ATT1_N_VALUES	36
#define	RI_ATT2_N_VALUES	6

/* Pixel with fill values */
static float32 fill_pixel[RI_ATT_N_VALUES]={1.3,-2.4,1000.3,.25};
static uint8 file_attr_2[F_ATT2_N_VALUES] = {1, 2, 3, 4, 5};

/********************************************************************
   Name: test_mgr_fillvalues()

   Description:
	This test routine sets fill values to an RI with GRsetattr
	then reads the values back with GRgetattr and verifies them.

   Return value:
	The number of errors occurred in this routine.

*********************************************************************/
static int test_mgr_fillvalues()
{
    int32 fid;		/* HDF file ID */
    int32 grid;		/* ID for the GR interface */
    int32 riid;		/* ID for the RI image */
    int32 attr_index;	/* attribute index */
    int32 dims[2]={5,7};    /* dimensions used on all images */
    uint16 ref;		/* RI reference number */
    int32 ri_index;	/* RI index */
    float32 image[7][5][N_COMPS]; /* space for the image data */
    VOIDP read_fill_vals=NULL;   /* space for fill values read from attr */
    float32 image0[7][5][N_COMPS]; /* space for the image data */
    int32 start[2];	/* start of image data to grab */
    int32 stride[2];	/* stride of image data to grab */
    char attr_name[H4_MAX_GR_NAME];
    int32 ntype, n_values;
    hdf_ntinfo_t nt_info;  /* struct containing name and byte order of a num type */
    int32 ret;		/* generic return value */
    int   num_errs = 0; /* number of errors so far */

    MESSAGE(8, printf("Reading fill-value attribute\n"););

    fid = Hopen(TESTFILE, DFACC_CREATE, 0);
    CHECK(fid, FAIL, "Hopen");

    /* Initialize the GR interface */
    grid = GRstart(fid);
    CHECK(grid, FAIL, "GRstart");

    {
        /* Create empty image with default fill-values */
        riid = GRcreate(grid, IMAGE1_NAME, N_COMPS, DFNT_FLOAT32, MFGR_INTERLACE_PIXEL, dims);
        CHECK(riid, FAIL, "GRcreate");

        /* Set the fill-value */
        ret = GRsetattr(riid, FILL_ATTR, DFNT_FLOAT32, RI_ATT_N_VALUES, fill_pixel);
        CHECK(ret, FAIL, "GRsetattr");

        /* Save the ref. # for later access */
        ref = GRidtoref(riid);
        CHECK(ref, (uint16)FAIL, "GRidtoref");

        /* Close the empty image */
        ret = GRendaccess(riid);
        CHECK(ret, FAIL, "GRendaccess");

        /* Get the index of the newly created image */
        ri_index = GRreftoindex(grid, ref);
        CHECK(ri_index, FAIL, "GRreftoindex");

        /* Select the newly created image */
        riid = GRselect(grid, ri_index);
        CHECK(riid, FAIL, "GRselect");

        /* Buffer to read image's data in */
        HDmemset(image, 0, (size_t)(dims[0]*dims[1]* N_COMPS)*sizeof(float32));

        /* Fill the memory-only with the default pixel fill-value */
        HDmemfill(image0, fill_pixel, sizeof(fill_pixel), sizeof(image0) / sizeof(fill_pixel));

        /* Read and verify image's data, should only be fill-values */
        start[0] = start[1] = 0;
        stride[0] = stride[1] = 1;
        ret = GRreadimage(riid, start, stride, dims, image);
        CHECK(ret, FAIL, "GRreadimage");

        if (HDmemcmp(image, image0, sizeof(image0)) != 0) {
            MESSAGE(3, printf("Error reading data for image with user defined fill-values\n"););
            num_errs++;
        }

        /* Find the image's attribute named FILL_ATTR */
        attr_index = GRfindattr(riid, FILL_ATTR);
        VERIFY(attr_index, 0, "GRfindattr");

        /* Get information about the current attribute. */
        ret = GRattrinfo(riid, attr_index, attr_name, &ntype, &n_values);
        CHECK(ret, FAIL, "GRattrinfo");
        VERIFY(attr_index, 0, "GRattrinfo");
        VERIFY(ntype, DFNT_FLOAT32, "GRattrinfo");
        VERIFY(n_values, RI_ATT_N_VALUES, "GRattrinfo");
        VERIFY_CHAR(attr_name, FILL_ATTR, "GRattrinfo");

        /* Allocate a buffer to hold the attribute data. */
        read_fill_vals = HDmalloc (n_values * sizeof (float32));
        if (read_fill_vals == NULL) {
            fprintf(stderr, "Unable to allocate space for attribute data.\n");
            exit(1);
        }

        /* Piggy-back a test for Hgetntinfo */
        ret = Hgetntinfo(ntype, &nt_info);
        CHECK(ret, FAIL, "Hgetntinfo");
        VERIFY_CHAR(nt_info.type_name, "float32", "Hgetntinfo");

        /* Read and verify the attribute's data */
        ret = GRgetattr(riid, attr_index, (VOIDP) read_fill_vals);
        CHECK(ret, FAIL, "GRgetattr");

        if (HDmemcmp(fill_pixel, read_fill_vals, RI_ATT_N_VALUES) != 0) {
            MESSAGE(3, printf("Error reading values of attribute FILL_ATTR\n"););
            num_errs++;
        } /* end if */
    
        if (read_fill_vals != NULL)
            HDfree(read_fill_vals);

        /* Close the empty image */
        ret = GRendaccess(riid);
        CHECK(ret, FAIL, "GRendaccess");
    }

    /* Shut down the GR interface */
    ret = GRend(grid);
    CHECK(ret, FAIL, "GRend");

    /* Close the file */
    ret = Hclose(fid);
    CHECK(ret, FAIL, "Hclose");

    /* Return the number of errors that's been kept track of so far */
    return num_errs;
} /* end test_mgr_fillvalues() */


/********************************************************************
   Name: test_mgr_userattr()

   Description:
	This test routine creates user-define attributes to the file
	and to an RI with GRsetattr then reads the values back with
	GRgetattr and verifies them.

   Return value:
	The number of errors occurred in this routine.

*********************************************************************/
static int test_mgr_userattr()
{
    int32 grid, riid, fid,
	  ri_index,
          f_att_index,     /* index of file attributes */
          ri_att_index,    /* index of raster image attributes */
          n_values,        /* number of values in an attribute */
          n_rimages,       /* number of raster images in the file */
          n_file_attrs;    /* number of file attributes */
    char  attr_name[H4_MAX_GR_NAME];  /* buffer to hold the attribute name */
    char  ri_name[H4_MAX_GR_NAME];  /* buffer to hold the image name */
    int32 ncomp;           /* number of components */
    int32 ntype;           /* number type of the components */
    int32 il;              /* interlace of the image data */
    int32 dims[2];	   /* dimension sizes of the image */
    int32 n_attrs;         /* number of attributes with each image */
    int16 ri_attr_2[RI_ATT2_N_VALUES] = {1, 2, 3, 4, 5, 6};
    VOIDP data_buf;        /* buffer to hold the attribute values */
    hdf_ntinfo_t nt_info;  /* struct containing name and byte order of a num type */
    intn  status;          /* status for functions returning an intn */
    int   num_errs = 0;    /* number of errors so far */

    MESSAGE(8, printf("Reading user-defined attribute\n"););

    fid = Hopen(TESTFILE, DFACC_RDWR, 0);
    CHECK(fid, FAIL, "Hopen");

    /* Initialize the GR interface */
    grid = GRstart(fid);
    CHECK(grid, FAIL, "GRstart");

    /* Set two file attributes. */
    status = GRsetattr(grid, F_ATT1_NAME, DFNT_CHAR8, F_ATT1_N_VALUES, F_ATT1_VAL); 
    CHECK(status, FAIL, "GRsetattr");

    status = GRsetattr(grid, F_ATT2_NAME, DFNT_UINT8, F_ATT2_N_VALUES, (VOIDP)file_attr_2);
    CHECK(status, FAIL, "GRsetattr");

    /* Obtain the index of the image named IMAGE1_NAME. */
    ri_index = GRnametoindex(grid, IMAGE1_NAME);
    CHECK(ri_index, FAIL, "GRnametoindex");

    /* Obtain the identifier of this image. */
    riid = GRselect(grid, ri_index);
    CHECK(riid, FAIL, "GRselect");

    /* Set two attributes to the image. */
    status = GRsetattr(riid, RI_ATT1_NAME, DFNT_CHAR8, RI_ATT1_N_VALUES, RI_ATT1_VAL);
    CHECK(status, FAIL, "GRsetattr");

    status = GRsetattr(riid, RI_ATT2_NAME, DFNT_INT16, RI_ATT2_N_VALUES, (VOIDP)ri_attr_2);
    CHECK(status, FAIL, "GRsetattr");

    /* Terminate accesses, and close the HDF file. */
    status = GRendaccess(riid);
    CHECK(status, FAIL, "GRendaccess");
    status = GRend(grid);
    CHECK(status, FAIL, "GRend");
    status = Hclose(fid);
    CHECK(status, FAIL, "Hclose");

    /* Reopen the file again to read the attributes and verify their values.*/
    fid = Hopen(TESTFILE, DFACC_RDWR, 0);
    CHECK(fid, FAIL, "Hopen");

    /* Initialize the GR interface */
    grid = GRstart(fid);
    CHECK(grid, FAIL, "GRstart");

    /* Verify the number of file attributes */
    status = GRfileinfo (grid, &n_rimages, &n_file_attrs);
    CHECK(status, FAIL, "GRfileinfo");

    /* Read each file attribute and verify its values */
    if (status != FAIL && n_file_attrs > 0)
    {
	for (f_att_index = 0; f_att_index < n_file_attrs; f_att_index++)
	{
	    /* Get information about the current file attribute. */
	    status = GRattrinfo (grid, f_att_index, attr_name, &ntype,
                              &n_values);
	    CHECK(status, FAIL, "GRattrinfo");

	    /* Allocate a buffer to hold the file attribute data.  Knowledge
	     * about the data type is assumed to be available from the previous
	     * part of the test where the correspondent GRsetattr was called. */
	    switch (ntype) {
	      case DFNT_CHAR8:
		data_buf = malloc (n_values * sizeof (char8));
		if (data_buf == NULL)
		{
		    fprintf(stderr, "test_mgr_userattr: Failed to allocate memory for data buffer for char8.\n");
		    exit (1);
		}
		break;
	      case DFNT_UINT8:
		data_buf = malloc (n_values * sizeof (uint8));
		if (data_buf == NULL)
		{
		    fprintf(stderr, "test_mgr_userattr: Failed to allocate memory for data buffer for uint8.\n");
		    exit (1);
		}
		break;
	      default:
		{
		    fprintf (stderr, "test_mgr_userattr: Unable to determine data type to allocate data buffer.\n");
		    exit (1);
		}
	    } /* switch */

	    /* Read and verify the attribute values. */
	    status = GRgetattr (grid, f_att_index, (VOIDP)data_buf);
	    CHECK(status, FAIL, "GRgetattr");

	    switch (ntype) {
	      case DFNT_CHAR8:
		if (HDmemcmp(data_buf, F_ATT1_VAL, n_values) != 0)
		{
		    MESSAGE(3, printf("Error reading values of attribute %s\n",
				attr_name););
		    num_errs++;
		} /* end if */

		/* Piggy-back a test for Hgetntinfo */
		status = Hgetntinfo(ntype, &nt_info);
		CHECK(status, FAIL, "Hgetntinfo");
		VERIFY_CHAR(nt_info.type_name, "char8", "Hgetntinfo");
		break;
	      case DFNT_UINT8:
		if (HDmemcmp(data_buf, file_attr_2, n_values) != 0)
		{
		    MESSAGE(3, printf("Error reading values of attribute %s\n",
				attr_name););
		    num_errs++;
		} /* end if */

		/* Piggy-back a test for Hgetntinfo */
		status = Hgetntinfo(ntype, &nt_info);
		CHECK(status, FAIL, "Hgetntinfo");
		VERIFY_CHAR(nt_info.type_name, "uint8", "Hgetntinfo");
		break;
	      default:
		{
		    fprintf (stderr, "test_mgr_userattr: Unable to determine data type to verify data.\n");
		    exit (1);
		}
	    } /* switch */

	    /* Free the space allocated for the data buffer. */
	    HDfree(data_buf);

	} /* for */
    } /* if */

    /* Obtain the index of the image named IMAGE1_NAME. */
    ri_index = GRnametoindex(grid, IMAGE1_NAME);
    CHECK(ri_index, FAIL, "GRnametoindex");

    /* Obtain the identifier of this image. */
    riid = GRselect(grid, ri_index);
    CHECK(riid, FAIL, "GRselect");

    /* Get the image information */
    status = GRgetiminfo(riid, ri_name, &ncomp, &ntype, &il, dims, &n_attrs);
    CHECK(status, FAIL, "GRgetiminfo");

    /* Verify each attribute's values */
    if (status != FAIL && n_attrs > 0)
    {
	for (ri_att_index = 0; ri_att_index < n_attrs; ri_att_index++)
	{
	    /* Get information about the current attribute. */
	    status = GRattrinfo (riid, ri_att_index, attr_name, &ntype,
                              &n_values);
	    CHECK(status, FAIL, "GRattrinfo");

	    /* Allocate a buffer to hold the attribute data.  Knowledge about
	     * the data type is assumed to be available from the previous
	     * part of the test where the correspondent GRsetattr was called. */
	    switch (ntype) {
	      case DFNT_FLOAT32:
		data_buf = malloc (n_values * sizeof (float32));
		if (data_buf == NULL)
		{
		    fprintf(stderr, "test_mgr_userattr: Failed to allocate memory for data buffer for float32.\n");
		    exit (1);
		}
		break;
	      case DFNT_CHAR8:
		data_buf = malloc (n_values * sizeof (char8));
		if (data_buf == NULL)
		{
		    fprintf(stderr, "test_mgr_userattr: Failed to allocate memory for data buffer for char8.\n");
		    exit (1);
		}
		break;
	      case DFNT_INT16:
		data_buf = malloc (n_values * sizeof (int16));
		if (data_buf == NULL)
		{
		    fprintf(stderr, "test_mgr_userattr: Failed to allocate memory for data buffer for int16.\n");
		    exit (1);
		}
		break;
	      default:
		{
		    fprintf (stderr, "test_mgr_userattr: Unable to determine data type to allocate data buffer.\n");
		    exit (1);
		}
	    } /* switch */

	    /* Read and verify the attribute values. */
	    status = GRgetattr (riid, ri_att_index, (VOIDP)data_buf);
	    CHECK(status, FAIL, "GRgetattr");

	    /* Verify values from the attribute.
	     * Knowledge about the data type is assumed to be available from
	     * the previous part of the test. */
	    switch (ntype) {
	      case DFNT_FLOAT32:
		if (HDmemcmp(fill_pixel, data_buf, RI_ATT_N_VALUES) != 0)
		{
		    MESSAGE(3, printf("Error reading values of attribute FILL_ATTR\n"););
		    num_errs++;
		} /* end if */
		break;
	      case DFNT_CHAR8:
		if (HDmemcmp(RI_ATT1_VAL, data_buf, RI_ATT1_N_VALUES) != 0)
		{
		    MESSAGE(3, printf("Error reading values of attribute FILL_ATTR\n"););
		    num_errs++;
		} /* end if */
		break;
	      case DFNT_INT16:
		if (HDmemcmp(ri_attr_2, data_buf, RI_ATT2_N_VALUES) != 0)
		{
		    MESSAGE(3, printf("Error reading values of attribute FILL_ATTR\n"););
		    num_errs++;
		} /* end if */
		break;
	      default:
		{
		    fprintf (stderr, "test_mgr_userattr: Unable to determine data type to read attribute data.\n");
		    exit (1);
		}
	    } /* switch */

	    /* Free the space allocated for the data buffer. */
	    HDfree(data_buf);

	} /* for */
    } /* if */
    /* Terminate accesses, and close the HDF file. */
    status = GRendaccess(riid);
    CHECK(status, FAIL, "GRendaccess");
    status = GRend(grid);
    CHECK(status, FAIL, "GRend");
    status = Hclose(fid);
    CHECK(status, FAIL, "Hclose");

    /* Return the number of errors that's been kept track of so far */
    return(num_errs);
} /* test_mgr_userattr */

/****************************************************************
**
**  test_mgr_attr(): Main multi-file raster attribute test routine
**
**  VII. Atribute Functions
**      A. GRattrinfo
**      B. Read/Write Attributes
**          1. GRsetattr
**          2. GRgetattr
**      C. GRfindattr
**
**  test_mgr_attr is invoked by test_mgr in mgr.c.
**
****************************************************************/
/* Test driver for testing attributes in multi-file GR interface */
extern void test_mgr_attr()
{
    int num_errs = 0;

    /* Output message about test being performed */
    MESSAGE(5, printf("Testing Multi-file Raster Attribute routines\n"););

    /* Test attribute functions with fill-values attributes */
    num_errs = num_errs + test_mgr_fillvalues();

    /* Test attribute functions with user-defined attributes */
    num_errs = num_errs + test_mgr_userattr();

    if (num_errs != 0) { H4_FAILED(); }
}   /* test_mgr_attr() */
