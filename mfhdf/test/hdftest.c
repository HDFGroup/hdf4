/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Copyright by The HDF Group.                                               *
 * Copyright by the Board of Trustees of the University of Illinois.         *
 * All rights reserved.                                                      *
 *                                                                           *
 * This file is part of HDF.  The full HDF copyright notice, including       *
 * terms governing use, modification, and redistribution, is contained in    *
 * the COPYING file, which can be found at the root of the source code       *
 * distribution tree, or in https://support.hdfgroup.org/ftp/HDF/releases/.  *
 * If you do not have access to either file, you may request a copy from     *
 * help@hdfgroup.org.                                                        *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/* $Id: hdftest.c 5218 2009-08-29 04:21:49Z bmribler $ */

#include "mfhdf.h"

#ifdef HDF

#include "hdftest.h"

#define UFOFILE   "file.UFO"  /* non-existing file */
#define FILE1     "test1.hdf"
#define FILE2     "test2.hdf"
#define EXTTST    "exttst.hdf"    /* main file for external file test */
#define EXTFILE   "SD_externals"  /* external file created in test */
#define NBITFILE  "nbit.hdf"
#define COMPFILE1 "comptst1.hdf"
#define COMPFILE2 "comptst2.hdf"
#define COMPFILE3 "comptst3.hdf"
#define COMPFILE4 "comptst4.hdf"
#define COMPFILE5 "comptst5.hdf"
#define COMPFILE6 "comptst6.hdf"
#define COMPFILE7 "comptst7.hdf"
#define CHKFILE   "chktst.hdf"    /* Chunking test file */
#define CNBITFILE "chknbit.hdf"   /* Chunking w/ NBIT compression */

/* Which tests to run? */
#define EXTERNAL_TEST 
#define NBIT_TEST 
#define COMP_TEST 
#define CHUNK_TEST
/*  commented out for now because of 'long' handling on 64-bit
    machines by this version of the netCDF library is broken. 
    The new version of the netCDF library(2.4.3?) has fixed 
    this I think. To fix it here requires merging in those fixes.*/

#define NETCDF_READ_TEST 

/* all test functions to be called in main */
extern int test_netcdf_reading();
extern int test_szip_compression();
extern int test_checkempty();
extern int test_idtest();
/* extern int test_sd(); - removed temporarily, see note in main(...) */
extern int test_mixed_apis();
extern int test_files();
extern int test_SDSprops();
extern int test_coordvar();
extern int test_chunk();
extern int test_compression();
extern int test_dimension();
extern int test_attributes();
extern int test_datasizes();
extern int test_datainfo();
extern int test_external();
extern int test_att_ann_datainfo();

int 
main(int argc, char *argv[])
{
    int32 f1, f2, fext, fnbit; /* File handles */
    int32 nt;                /* Number type */
    int32 dimsize[10];       /* dimension sizes */
    int32 newsds, newsds2, newsds3; /* SDS handles */
    int32 sdsid;                 /* SDS handle */
    int32 noextsds;              /* no external SDS id */
    int32 dimid, dimid1, dimid2; /* Dimension handles */
    int32 num_sds;               /* number of SDS in file */
    int32 num_gattr;             /* Number of global attributes */
    int32 offset;                /* offset for ? */
    int32 index;       /* Index of dataset in file */
    int32 ival;
    int32 sdid;        /* another SDS handle */
    int32 rank;        /* rank of SDS */
    intn  status;      /* status flag */
    intn  i;           /* loop variables */
    intn  nattrs;      /* Number of attributes again? */
    char name[90];
    char text[256];
    int32   start[10], end[10], stride[10]; /* start, end, stride arrays */
    int32   scale[10];
    char    l[80], u[80], fmt[80], c[80];
    int32   count;
    int32   fillval, readval;
    int32   idata[100];
    int32   rdata[100];
    int16   sdata[100], outdata[100];
    int32   ndg_saved_ref;  /* used to save a ref of an SDS in one of the test */
    uint8   iuval;
    float32 data[1000], max, min, imax, imin;
    float64 cal, cale, ioff, ioffe;
    int     num_errs = 0;    /* number of errors so far */

    ncopts = NC_VERBOSE;

    /* Testing SDstart */
    /* Try start non-existing file with RDONLY and RDWR. Both should fail. */
    f1 = SDstart(UFOFILE, DFACC_RDONLY);

    if (f1 != FAIL)
      {
          fprintf(stderr, "SDstart(..., RDONLY) should fail\n");
          num_errs++;
          SDend(f1);
      }

    f1 = SDstart(UFOFILE, DFACC_RDWR);
    if (f1 != FAIL)
      {
          fprintf(stderr, "SDstart(..., RDWR) should fail\n");
          num_errs++;
          SDend(f1);
      }

    /* -------hmm what are testing here?----------------- */

    /* Create two files */
    f1 = SDstart(FILE1, DFACC_CREATE);
    CHECK(f1, FAIL, "SDstart");

    f2 = SDstart(FILE2, DFACC_CREATE);
    CHECK(f2, FAIL, "SDstart");

    /* whats in these empty files */
    status = SDfileinfo(f1, &num_sds, &num_gattr);
    CHECK(status, FAIL, "SDfileinfo");

    if(num_gattr != 0) 
      {
          fprintf(stderr, "File %s still has stuff in it\n", FILE1);
          num_errs++;
      }

    /* create a 4 by 8 dataset called DataSetAlpha in file test1.hdf */
    dimsize[0] = 4;
    dimsize[1] = 8;
    newsds = SDcreate(f1, "DataSetAlpha", DFNT_FLOAT32, 2, dimsize);
    CHECK(newsds, FAIL, "SDcreate: Failed to create a new data set DataSetAlpha ");

    /* save the ref number for the first dataset --- will check at very end */
    ndg_saved_ref = SDidtoref(newsds);
    CHECK(ndg_saved_ref, 0, "SDidtoref: Failed to get NDG ref for DataSetAlpha ");

    /* create datatset DataSetGamma in file test1.hdf */
    newsds3 = SDcreate(f1, "DataSetGamma", DFNT_FLOAT64, 1, dimsize);
    CHECK(newsds3, FAIL, "SDcreate:Failed to create a new data set gamma");

    /* get info on number of datasets and global attributes in file */
    status = SDfileinfo(f1, &num_sds, &num_gattr);
    CHECK(status, FAIL, "SDfileinfo");

    if(num_sds != 2) 
      {
        fprintf(stderr, "Wrong number of datasets in file 1\n");
        num_errs++;
      }

    /* get dimension handle for first dimension? of DataSetGamma */
    dimid = SDgetdimid(newsds3, 0);
    CHECK(dimid, FAIL, "SDgetdimid:Failed to get dimension id");

    /* reset the dimension name to Mydim? */
    status = SDsetdimname(dimid, "MyDim");
    CHECK(status, FAIL, "SDsetdimname: Failed to set dimension name to 'MyDim'");

    /* Set dimension attribute to 'TRUE' */
    status = SDsetattr(dimid, "DimensionAttribute", DFNT_CHAR8, 4, "TRUE");
    CHECK(status, FAIL, "SDsetattr: Failed to set Dimension attribute");
    
    /* hmm. look it back up again. */
    status = SDfindattr(dimid, "DimensionAttribute");
    if(status != 0) 
      {
        fprintf(stderr, "SDfindattr: Bad index for finding 'DimensionAttribute' %d\n",
                status);
        num_errs++;
      }

    /* Find out info about first atribute for dimension  */
    status = SDattrinfo(dimid, (int32) 0, name, &nt, &count);
    CHECK(status, FAIL, "SDattrinfo");

    /* read first attribute in, assume CHAR here. */
    status = SDreadattr(dimid, 0, text);
    CHECK(status, FAIL, "SDreadattr");
    
    /* Compare value reterieved to what was written */
    if(HDstrncmp(text, "TRUE", count)) 
      {
        fprintf(stderr, "SDreadattr: Invalid dimension attribute read <%s>\n", text);
        num_errs++;
      }

    /* get First dimension of dataset 'DataSetAlpha' */
    dimid = SDgetdimid(newsds, 0);
    CHECK(dimid, FAIL, "SDgetdimid: Failed to get dimension id");

    /* Set this name of this dimension to 'Mydim' */
    status = SDsetdimname(dimid, "MyDim");
    CHECK(status, FAIL, "SDsetdimname");


    /* Set the scales for this dimension also */
    scale[0] = 1;
    scale[1] = 5;
    scale[2] = 7;
    scale[3] = 24;
    status = SDsetdimscale(dimid, 4, DFNT_INT32, (VOIDP) scale);
    CHECK(status, FAIL, "SDsetdimscale");

    /* Set the dimension strings for the dimension also */
    status = SDsetdimstrs(dimid, "DimLabel", NULL, "TheFormat");
    CHECK(status, FAIL, "SDsetdimstrs");

    /* verify that we can read the dimensions values with SDreaddata */
    start[0] = 0;
    end[0]   = 4;
    status = SDreaddata(dimid, start, NULL, end, (VOIDP) idata);
    CHECK(status, FAIL, "SDreaddata");

    /* compare retrieved values for scale */
    for(i = 0; i < 4; i++) 
      {
        if(idata[i] != scale[i]) 
          {
              fprintf(stderr, "SDreaddata() returned %ld not %ld in location %d\n", 
                      (long)idata[i], (long)scale[i], i);
              num_errs++;
          }
      }

    /* hmm...lets store an attribute here for the dimension */
    max = (float32)3.1415;
    status = SDsetattr(dimid, "DimAttr", DFNT_FLOAT32, 1, (VOIDP) &max);
    CHECK(status, FAIL, "SDsetattr");

    /* lets make sure we can read it too */
    status = SDattrinfo(dimid, 3, name, &nt, &count);
    CHECK(status, FAIL, "SDattrinfo");

    if(nt != DFNT_FLOAT32) 
      {
        fprintf(stderr, "Wrong number type for SDattrinfo(dim)\n");
        num_errs++;
      }

    if(count != 1) 
      {
        fprintf(stderr, "Wrong count for SDattrinfo(dim)\n");
        num_errs++;
      }

    if(strcmp(name, "DimAttr")) 
      {
        fprintf(stderr, "Wrong name for SDattrinfo(dim)\n");
        num_errs++;
      }

    /* get second dimension of data set 'DataSetAlpha' */
    dimid2 = SDgetdimid(newsds, 1);
    CHECK(dimid2, FAIL, "SDgetdimid: Failed to get second dimension id");

    /* lets store an attribute for the dimension without explicitly 
       creating the coord var first */
    ival = -256;
    status = SDsetattr(dimid2, "Integer", DFNT_INT32, 1, (VOIDP) &ival);
    CHECK(status, FAIL, "SDsetattr");

    /* lets make sure we can read it too */
    status = SDattrinfo(dimid2, 0, name, &nt, &count);
    CHECK(status, FAIL, "SDattrinfo");

    if(nt != DFNT_INT32) 
      {
        fprintf(stderr, "Wrong number type for SDattrinfo(dim)\n");
        num_errs++;
      }

    if(count != 1) 
      {
        fprintf(stderr, "Wrong count for SDattrinfo(dim)\n");
        num_errs++;
      }

    if(strcmp(name, "Integer")) 
      {
        fprintf(stderr, "Wrong name for SDattrinfo(dim)\n");
        num_errs++;
      }

    /* read dimension attribute back in */
    ival = 0;
    status = SDreadattr(dimid2, 0, (VOIDP) &ival);
    CHECK(status, FAIL, "SDreatattr");
    
    if(ival != -256) 
      {
        fprintf(stderr, "Wrong value for SDreadattr(dim)\n");
        num_errs++;
      }

    /* add an unsigned integer as an dimension attribute */
    iuval = 253;
    status = SDsetattr(dimid2, "UnsignedInteger", DFNT_UINT8, 1, (VOIDP) &iuval);
    CHECK(status, FAIL, "SDsetattr");

    /* lets make sure we can read it too */
    status = SDattrinfo(dimid2, 1, name, &nt, &count);
    CHECK(status, FAIL, "SDattrinfo");

    if(nt != DFNT_UINT8) 
      {
        fprintf(stderr, "Wrong number type for SDattrinfo(dim)\n");
        num_errs++;
      }

    if(count != 1) 
      {
        fprintf(stderr, "Wrong count for SDattrinfo(dim)\n");
        num_errs++;
      }

    if(strcmp(name, "UnsignedInteger")) 
      {
        fprintf(stderr, "Wrong name for SDattrinfo(dim)\n");
        num_errs++;
      }

    /* read second dimension attribute back in */
    iuval = 0;
    status = SDreadattr(dimid2, 1, (VOIDP) &iuval);
    CHECK(status, FAIL, "SDreatattr");
    
    if(iuval != 253) 
      {
        fprintf(stderr, "Wrong value for SDreadattr(dim)\n");
        num_errs++;
      }

    /* Find index of data set 'DataSetAlpha' in file test1.hdf */
    status = SDnametoindex(f1, "DataSetAlpha");
    if(status != 0) 
      {
        fprintf(stderr, "Couldn't find data set in file 1\n");
        num_errs++;
      }

    /* Try finding data set in test2.hdf, should fail? */
    status = SDnametoindex(f2, "DataSetAlpha");
    if(status != FAIL) 
      {
        fprintf(stderr, "Found data set in wrong file 2\n");
        num_errs++;
      }

    /* Try finding non-existent dataset in file, should fail */
    status = SDnametoindex(f1, "BogusDataSet");
    if(status != FAIL) 
      {
        fprintf(stderr, "Found bogus data set in file 1\n");
        num_errs++;
      }

    /* Set fill value for data set 'DataSetAlpha' assume we still have valid
       handle at this point...*/
    max = -17.5;
    status = SDsetfillvalue(newsds, (VOIDP) &max);
    CHECK(status, FAIL, "SDsetfillvalue");

    /* initialize array to write out */
    for(i = 0; i < 10; i++)
        data[i] = (float32) i;

    /* write out (1,1)->(3,3) array out */ 
    start[0] = start[1] = 1;
    end[0]   = end[1]   = 3;
    status = SDwritedata(newsds, start, NULL, end, (VOIDP) data);
    CHECK(status, FAIL, "SDwritedata");

    /* set the range for data set 'DataSetAlpha' */
    max = (float32)10.0;
    min = (float32)4.6;
    status = SDsetrange(newsds, (VOIDP) &max, (VOIDP) &min);
    CHECK(status, FAIL, "SDsetrange");

    /* Brillant...., retrieve it right back....*/
    status = SDgetrange(newsds, (VOIDP) &imax, (VOIDP) &imin);
    CHECK(status, FAIL, "SDsetrange");

    /* set a character attribute for data set 'DataSetAlpha' */
    status = SDsetattr(newsds, "spam", DFNT_CHAR8, 6, "Hi mom");
    CHECK(status, FAIL, "SDsetattr");

    /* Set the data strings for data set 'DataSetAlpha' */
    status = SDsetdatastrs(newsds, "TheLabel", "TheUnits", NULL, "TheCordsys");
    CHECK(status, FAIL, "SDsetdatastrs");

    /* Brilliant.....retrieve them right back */
    status = SDgetdatastrs(newsds, l, u, fmt, c, 80);
    CHECK(status, FAIL, "SDgetdatastrs");

    if(HDstrcmp(l, "TheLabel")) 
      {
        fprintf(stderr, "Bogus label returned (%s)\n", l);
        num_errs++;
      }
    if(HDstrcmp(u, "TheUnits")) 
      {
        fprintf(stderr, "Bogus units returned (%s)\n", u);
        num_errs++;
      }
    if(HDstrcmp(fmt, "")) 
      {
        fprintf(stderr, "Bogus format returned\n");
        num_errs++;
      }
    if(HDstrcmp(c, "TheCordsys")) 
      {
        fprintf(stderr, "Bogus cordsys returned\n");
        num_errs++;
      }

    /* retrieve CHAR attribute for 'DataSetAlpha' */
    status = SDfindattr(newsds, "spam");
    if(status != 2) 
      {
        fprintf(stderr, "Bad index for SDfindattr\n");
        num_errs++;
      }

    /* retrieve non-existent CHAR attribute for 'DataSetAlpha'. 
       Should fail. */
    status = SDfindattr(newsds, "blarf");
    if(status != FAIL) 
      {
        fprintf(stderr, "SDfindattr found non-existant attribute\n");
        num_errs++;
      }

    /* hmm....set global attributes for File 'test1.hdf' */
    status = SDsetattr(f1, "F-attr", DFNT_CHAR8, 10, "globulator");
    CHECK(status, FAIL, "SDsetattr");

    /* get info about the global attribute just created....*/
    status = SDattrinfo(f1, (int32) 0, name, &nt, &count);
    CHECK(status, FAIL, "SDattrinfo");

    /* read this global attribute back in ....*/
    status = SDreadattr(f1, 0, text);
    CHECK(status, FAIL, "SDreadattr");
    
    if(HDstrncmp(text, "globulator", count)) 
      {
        fprintf(stderr, "Invalid global attribute read <%s>\n", text);
        num_errs++;
      }

    /* Get number of SDS and global attributes in file 'test2.hdf'.
       It should be empty...*/
    status = SDfileinfo(f2, &num_sds, &num_gattr);
    if(num_sds != 0) 
      {
        fprintf(stderr, "File2 still has stuff in it\n");
        num_errs++;
      }

    /* Set calibration info for dataset 'DataSetGamma' in file 'test1.hdf' */
    cal   = 1.0;
    cale  = 5.0;
    ioff  = 3.0;
    ioffe = 2.5;
    nt    = DFNT_INT8;
    status = SDsetcal(newsds3, cal, cale, ioff, ioffe, nt);
    CHECK(status, FAIL, "SDsetcal");

    /* create a record variable in file 'test2.hdf' */
    dimsize[0] = SD_UNLIMITED;
    dimsize[1] = 6;
    newsds2 = SDcreate(f2, "DataSetBeta", DFNT_INT16, 2, dimsize);
    CHECK(newsds2, FAIL, "SDcreate: Failed to create new data set 'DataSetBeta'");

    /* get info on number of SDSs and global attributes in file 'test2.hdf'
       There should be only 1 SDS */
    status = SDfileinfo(f2, &num_sds, &num_gattr);
    if(num_sds != 1) 
      {
        fprintf(stderr, "Wrong number of datasets in file 2\n");
        num_errs++;
      }

    for(i = 0; i < 50; i++)
        sdata[i] = i;


    /* Write data to dataset 'DataSetBeta' in file 'test2.hdf' */
    start[0] = start[1] = 0;
    end[0]   = 8;
    end[1]   = 6;
    status = SDwritedata(newsds2, start, NULL, end, (VOIDP) sdata);
    CHECK(status, FAIL, "SDwritedata");

    /* Now read part of an earlier dataset,'DataSetAlpha', 
       back in from file 'test1.hdf' */
    start[0] = start[1] = 0;
    end[0]   = end[1]   = 3;
    status = SDreaddata(newsds, start, NULL, end, (VOIDP) data);
    CHECK(status, FAIL, "SDreaddata");

    /* verify the data values retrieved from 'DataSetAlpha' */
    if(data[0] != -17.5) 
      {
        fprintf(stderr, "Wrong value returned loc 0: %f\n",(float)data[0]);
        num_errs++;
      }
    if(data[3] != -17.5) 
      {
        fprintf(stderr, "Wrong value returned loc 3: %f\n",(float)data[3]);
        num_errs++;
      }
    if(data[5] != 1.0) 
      {
        fprintf(stderr, "Wrong value returned loc 5: %f\n",(float)data[5]);
        num_errs++;
      }
    if(data[6] != -17.5) 
      {
        fprintf(stderr, "Wrong value returned loc 6: %f\n",(float)data[6]);
        num_errs++;
      }
    if(data[8] != 4.0) 
      {
        fprintf(stderr, "Wrong value returned loc 8: %f\n",(float)data[8]);
        num_errs++;
      }

    for(i = 0; i < 50; i++)
        outdata[i] = 0;

    /* read data back in from 'DataSetBeta' from file 'test2.hdf' */
    start[0] = start[1] = 1;
    end[0]   = 3;
    end[1]   = 3;
    stride[0] = 2;
    stride[1] = 2;
    status = SDreaddata(newsds2, start, stride, end, (VOIDP) outdata);
    CHECK(status, FAIL, "SDreaddata");

    { /* verify read values; should be
    7 9 11 19 21 23 31 33 35 */
      int i,j; /* indexing the two dimensions */
      int k,l; /* counters = number of elements read on each dimension */
      int m=0; /* indexing the outdata array */
      for(i = 1,l=0; l<3; i=i+2,l++)
  for (j =(i*6)+1,k=0; k<3; j=j+2,k++,m++)
  {
      if (m < 10) /* number of elements read is 9 */
      if (outdata[m] != sdata[j])
      {
    fprintf(stderr, "line %d, wrong value: should be %d, got %d\n",
                           __LINE__, sdata[j], outdata[m]);
    num_errs++;
      }
  }
    }

    /* why do we set calibration info and then use SDgetcal() 
       on dataset 'DataSetGamma' ? */
    cal   = 1.0;
    cale  = 5.0;
    ioff  = 3.0;
    ioffe = 2.5;
    nt    = DFNT_INT8;
    status = SDgetcal(newsds3, &cal, &cale, &ioff, &ioffe, &nt);
    CHECK(status, FAIL, "SDgetcal");

    /* Verify calibration data for data set 'DataSetGamma' */
    if(cal != 1.0) 
      {
        fprintf(stderr, "Wrong calibration info\n");
        num_errs++;
      }

    if(cale != 5.0) 
      {
        fprintf(stderr, "Wrong calibration info\n");
        num_errs++;
      }

    if(ioff != 3.0) 
      {
        fprintf(stderr, "Wrong calibration info\n");
        num_errs++;
      }

    if(ioffe != 2.5) 
      {
        fprintf(stderr, "Wrong calibration info\n");
        num_errs++;
      }

    if(nt != DFNT_INT8) 
      {
        fprintf(stderr, "Wrong calibration info\n");
        num_errs++;
      }

    /* end access to data set 'DataSetAlpha' */
    status = SDendaccess(newsds);
    CHECK(status, FAIL, "SDendaccess");

    /* end access to data set 'DataSetBeta' */
    status = SDendaccess(newsds2);
    CHECK(status, FAIL, "SDendaccess");

    /* end access to data set 'DataSetGamma' */
    status = SDendaccess(newsds3);
    CHECK(status, FAIL, "SDendaccess");

    /* Close access to file 'test1.hdf' */
    status = SDend(f1);
    CHECK(status, FAIL, "SDend");

    /* Close access to file 'test2.hdf' */
    status = SDend(f2);
    CHECK(status, FAIL, "SDend");


    /*
     * New set of tests?.....
     */

    /* test SDsetfillmode   */
    /* test fixed size SDS   */
    /* create an empty SDS, set SD_NOFILL.
       Change the fill mode to SD_FILL, and write a slab of data */

    /* open file 'test1.hdf' */
    f1 = SDstart(FILE1, DFACC_RDWR);
    CHECK(f1, FAIL, "SDstart (again)");

    /* Set fill mode on file to not write out fill values */
    status = SDsetfillmode(f1, SD_NOFILL);
    CHECK(status, FAIL, "SDsetfillmode: (SD_NOFILL)");

    /* Create data set 'FIXED1' in file test1.hdf */
    dimsize[0]=5;
    dimsize[1]=6;
    sdid = SDcreate(f1, "FIXED1", DFNT_INT32, 2, dimsize);
    CHECK(sdid, FAIL, "SDcreate:Fail to create data set 'FIXED1' in 'test1.hdf'");

    for (i=0; i<30; i++)
        idata[i] = i+100;

    /* Set fill value attribute for data set 'FIXED1' using SDsetattr().
       Same affect as using SDsetfillvalue(). */
    fillval = -300;
    status = SDsetattr(sdid, "_FillValue", DFNT_INT32, 1,
               (VOIDP) &fillval); /* can use SDsetfillvalue */
    CHECK(status, FAIL, "SDsetattr");

    /* Test get compression info when the data set is empty and not set
       to be compressed */
    {
  comp_coder_t comp_type;  /* type of compression */
  comp_info cinfo;  /* compression information */
  status = SDgetcompinfo(sdid, &comp_type, &cinfo);
  CHECK(status, FAIL, "SDgetcompinfo");
  VERIFY(comp_type, COMP_CODE_NONE, "SDgetcompinfo");
    }

    /* end access to data set 'FIXED1' */
    status = SDendaccess(sdid);
    CHECK(status, FAIL, "SDendaccess");

    /* get index of dataset in file 'test1.hdf' called 'FIXED1' */
    index = SDnametoindex(f1, "FIXED1");
    CHECK(index, FAIL, "SDnametoindex");

    /* Select data set 'FIXED1' based on it's index */
    sdid = SDselect(f1, index);
    CHECK(sdid, FAIL, "SDselect");

    /* change the fill mode for the file back to writing out the fill
       values. */
    status = SDsetfillmode(f1, SD_FILL);
    CHECK(status, FAIL, "SDsetfillmode");

    /* Write data to data set 'FIXED1'.
       Note that SD_FILL mode is on. */
    start[0]=2;
    start[1]=0;
    end[0]=1;
    end[1]=6;
    status = SDwritedata(sdid, start, NULL, end, (VOIDP)idata);
    CHECK(status, FAIL, "SDwritedata: (SD_FILL)");

    /* Test get compression info when the data set is not empty and 
       compressed */
    {
  comp_coder_t comp_type;  /* type of compression */
  comp_info cinfo;  /* compression information */
  status = SDgetcompinfo(sdid, &comp_type, &cinfo);
  CHECK(status, FAIL, "SDgetcompinfo");
  VERIFY(comp_type, COMP_CODE_NONE, "SDgetcompinfo");
    }

    /* end access to data set 'FIXED1' */
    status = SDendaccess(sdid);
    CHECK(status, FAIL, "SDendaccess");

    /* set the fill mode for 'test1.hdf' to no-fill */
    status = SDsetfillmode(f1, SD_NOFILL);
    CHECK(status, FAIL, "SDsetfillmode (SD_NOFILL)");

    /* create a data set 'FIXED' in file 'test1.hdf' */
    sdid = SDcreate(f1, "FIXED", DFNT_INT32, 2, dimsize);
    CHECK(sdid,FAIL,"SDcreate:Failed to create data set 'FIXED' in file 'test1.hdf'");

    for (i=0; i<30; i++)
        idata[i] = i+100;

    /* Set fill value for data set 'FIXED' using SDsetfillvalue() */
    fillval = -300;
    status = SDsetfillvalue(sdid, (VOIDP) &fillval);
    CHECK(status, FAIL, "SDsetfillvalue");

    /* write out the first 2 records to data set 'FIXED' with SD_NOFILL mode */
    start[0]=2;
    start[1]=0;
    end[0]=1;
    end[1]=6;
    status = SDwritedata(sdid, start, NULL, end, (VOIDP)idata);
    CHECK(status, FAIL, "SDwritedata: (SD_NOFILL)");

    /* end access to data set 'FIXED' */
    status = SDendaccess(sdid);
    CHECK(status, FAIL, "SDendaccess");

    /* close file 'test1.hdf' */
    status = SDend(f1);
    CHECK(status, FAIL, "SDend");

    /* open again, write record 4 with SD_FILL mode */
    /* fill values already written out in the first SDwritedata,
       fillmode changes should not affect the fill values */

    /* open file 'test1.hdf' */
    f1 = SDstart(FILE1, DFACC_RDWR);
    CHECK(f1, FAIL, "SDstart: test1.hdf");

    /* Set fill mode to SD_FILL */
    status = SDsetfillmode(f1, SD_FILL);
    CHECK(status, FAIL, "SDsetfillmode: (SD_FILL)");

    /* get index of data set 'FIXED' */
    index = SDnametoindex(f1, "FIXED");
    CHECK(index, FAIL, "SDnametoindex: (FIXED)");

    /* Select the data set 'FIXED' based on its index */
    sdid = SDselect(f1, index);
    CHECK(sdid, FAIL, "SDselect: (FIXED)");

    /* Get its fill value */
    status = SDgetfillvalue(sdid, &readval);
    CHECK(status, FAIL, "SDgetfillvalue: (FIXED)");
    VERIFY(readval, -300, "SDgetfillvalue");

    /* Write record 4 */
    start[0]=4;
    start[1]=0;
    end[0]=1;
    end[1]=6;
    status = SDwritedata(sdid, start, NULL, end, (VOIDP)idata);
    CHECK(status, FAIL, "SDwritedata (SD_FILL)");

    /* end access to data set 'FIXED' */
    status = SDendaccess(sdid);
    CHECK(status, FAIL, "SDendaccess");

    /* close file 'test1.hdf' */
    status = SDend(f1);
    CHECK(status, FAIL, "SDend");

    /* read back and check fill values */

    /* open file 'test1.hdf' back up */
    f1 = SDstart(FILE1, DFACC_RDWR);
    CHECK(f1, FAIL, "SDstart: test1.hdf");

    /* get index of data set 'FIXED' */
    index = SDnametoindex(f1, "FIXED");
    CHECK(index, FAIL, "SDnametoindex (FIXED)");

    /* Select the data set 'FIXED' based on its index */
    sdid = SDselect(f1, index);
    CHECK(sdid, FAIL, "SDselect (FIXED)");

    /* read data back in from data set 'FIXED' */
    start[0]=0;
    start[1]=0;
    end[0]=5;
    end[1]=6;
    status = SDreaddata(sdid, start, NULL, end, (VOIDP)idata);
    CHECK(status, FAIL, "SDreaddata(FIXED)");

    /* verify the data */
    for (i=12; i<18; i++)  
      {
        if ((idata[i] != 100 + (i-12)) ||
            (idata[i+12] != 100 + (i-12))) 
          {
           fprintf(stderr, "line %d, wrong value: should be %d, got %d %d\n",
                           __LINE__,100 + i-12, (int)idata[i], (int)idata[i+12]);
           num_errs++;
          }
      }
    
    for (i=18; i<24; i++)  
      {
        if (idata[i] ==fillval) 
          {
           fprintf(stderr, "line %d, wrong value: should not be %d, got %d\n",
                           __LINE__,(int)fillval, (int)idata[i]);
           num_errs++;
          }
      }

    /* end access to data set 'FIXED' */
    status = SDendaccess(sdid);
    CHECK(status, FAIL, "SDendaccess");

    /* read back in data set 'FIXED1' , with fill values */

    /* get index of data set 'FIXED1' from file 'test1.hdf' */
    index = SDnametoindex(f1, "FIXED1");
    CHECK(index, FAIL, "SDnametoindex (FIXED1)");

    /* select dataset 'FIXED1' based on its index in the file */
    sdid = SDselect(f1, index);
    CHECK(sdid, FAIL, "SDselect (FIXED1)");

    /* read data from data set 'FIXED1' */
    start[0]=0;
    start[1]=0;
    end[0]=5;
    end[1]=6;
    status = SDreaddata(sdid, start, NULL, end, (VOIDP)idata);
    CHECK(status, FAIL, "SDreaddata(FIXED)");

    /* verify the data */
    for (i=12; i<18; i++)  
      {
        if (idata[i] != (100 + (i-12)))  
          {
           fprintf(stderr, "line %d, wrong value: should be %d, got %d \n",
                           __LINE__, 100 + i-12, (int)idata[i]);
           num_errs++;
          }
      }

    for (i=18; i<24; i++)  
      {
        if (idata[i] != fillval) 
          {
           fprintf(stderr, "line %d, wrong value: should be %d, got %d\n",
                           __LINE__, (int)fillval, (int)idata[i]);
           num_errs++;
          }
      }

    /* end access to data set 'FIXED1' in file 'test1.hdf' */
    status = SDendaccess(sdid);
    CHECK(status, FAIL, "SDendaccess");

    /* close file 'test1.hdf' */
    status = SDend(f1);
    CHECK(status, FAIL, "SDend");

   /* 
    * test UNLIMITED size SDS   
    */

    /* open file 'test1.hdf' */
    f1 = SDstart(FILE1, DFACC_RDWR);
    CHECK(f1, FAIL, "SDstart (file1)");

    /* set fill mode to no-fill */
    status = SDsetfillmode(f1, SD_NOFILL);
    CHECK(status, FAIL, "SDsetfillmode (SD_NOFILL)");

    /* Set first dimension to UNLIMITED.
       Create data set 'UNLIMITED_SDS' in file 'test1.hdf' */
    dimsize[0]=SD_UNLIMITED;
    dimsize[1]=6;
    sdid = SDcreate(f1, "UNLIMITED_SDS", DFNT_INT32, 2, dimsize);
    CHECK(sdid, FAIL, "SDcreate:Failed to create data set 'UNLIMITED_SDS' in file 'test1.hdf'");

    for (i=0; i<24; i++)
        idata[i] = i;

    /* Set fill value for data set 'UNLIMITED_SDS' */
    fillval = -300;
    status = SDsetfillvalue(sdid, (VOIDP) &fillval);
    CHECK(status, FAIL, "SDsetfillvalue");

    /* write out the third record with SD_NOFILL mode on */
    start[0]=2;
    start[1]=0;
    end[0]=1;
    end[1]=6;
    status = SDwritedata(sdid, start, NULL, end, (VOIDP)idata);
    CHECK(status, FAIL, "SDwritedata: (SD_NOFILL, UNLIMITED)");

    /* end access to data set 'UNLIMITED_SDS' in file 'test1.hdf' */
    status = SDendaccess(sdid);
    CHECK(status, FAIL, "SDendaccess");

    /* Close file 'test1.hdf' */
    status = SDend(f1);
    CHECK(status, FAIL, "SDend");

    /* open again, write record 4 with SD_FILL mode */

    /* open file 'test1.hdf' again */
    f1 = SDstart(FILE1, DFACC_RDWR);
    CHECK(f1, FAIL, "SDstart: test1.hdf");

    /* set fill mode to SD_FILL */
    status = SDsetfillmode(f1, SD_FILL);
    CHECK(status, FAIL, "SDsetfillmode: (SD_FILL)");

    /* get index of data set 'UNLIMITED_SDS' */
    index = SDnametoindex(f1, "UNLIMITED_SDS");
    CHECK(index, FAIL, "SDnametoindex: (UNLIMITED)");

    /* select data set 'UNLIMITED_SDS' based on its index in the file */
    sdid = SDselect(f1, index);
    CHECK(sdid, FAIL, "SDselect: (UNLIMITED)");

    /* write 4?th record to data set */
    start[0]=4;
    start[1]=0;
    end[0]=1;
    end[1]=6;
    status = SDwritedata(sdid, start, NULL, end, (VOIDP)idata);
    CHECK(status, FAIL, "SDwritedata: (SD_FILL)");

    /* end access to data set 'UNLIMITED_SDS' */
    status = SDendaccess(sdid);
    CHECK(status, FAIL, "SDendaccess");

    /* close file 'test1.hdf' */
    status = SDend(f1);
    CHECK(status, FAIL, "SDend");

    /* read back and check fill values */

    /* open file 'test1.hdf' again */
    f1 = SDstart(FILE1, DFACC_RDWR);
    CHECK(f1, FAIL, "SDstart: (test1.hdf)");

    /* get index of data set 'UNLIMITED_SDS' */
    index = SDnametoindex(f1, "UNLIMITED_SDS");
    CHECK(index, FAIL, "SDnametoindex: (UNLIMITED_SDS)");

    /* select data set 'UNLIMITED_SDS' based on it's index in the file */
    sdid = SDselect(f1, index);
    CHECK(sdid, FAIL, "SDselect: (UNLIMITED_SDS)");

    /* read data from data set 'UNLIMITED_SDS' */
    start[0]=0;
    start[1]=0;
    end[0]=5;
    end[1]=6;
    status = SDreaddata(sdid, start, NULL, end, (VOIDP)idata);
    CHECK(status, FAIL, "SDwritedata(NO_FILL)");

    /* verify the data */
    for (i=12; i<18; i++)  
      {
        if ((idata[i] != (i-12)) || (idata[i+12] != (i-12))) 
          {
           fprintf(stderr, "line %d, wrong value for %d: should be %d, got %d\n",
                           __LINE__, i-12, (int)idata[i], (int)idata[i+12]);
           num_errs++;
          }
      }

    for (i=18; i<24; i++)  
      {
        if (idata[i] !=fillval) 
          {
           fprintf(stderr, "line %d, wrong value: should be %d, got %d\n",
                           __LINE__, (int)fillval, (int)idata[i]);
           num_errs++;
          }
      }

    /* end access to data set 'UNLIMITED_SDS' */
    status = SDendaccess(sdid);
    CHECK(status, FAIL, "SDendaccess");

    /* close file 'test1.hdf' */
    status = SDend(f1);
    CHECK(status, FAIL, "SDend");

   /* 
    * test SDsetdimval_incomp() 
    */

    /* open file 'test1.hdf' */
    f1 = SDstart(FILE1, DFACC_RDWR);
    CHECK(f1, FAIL, "SDstart (file1)");

    /* set first dimension to be UNLIMITED.
       Create data set 'dimval_1_compat' */
    dimsize[0]=SD_UNLIMITED;
    dimsize[1]=6;
    sdid = SDcreate(f1, "dimval_1_compat", DFNT_INT32, 2, dimsize);
    CHECK(sdid,FAIL,"SDcreate:Failed to create data set 'dimval_1_compat' in file 'test1.hdf'");

    /* get handle for first dimension of data set 'dimval_1_compat' */
    dimid=SDgetdimid(sdid, 0);
    CHECK(dimid, FAIL, "SDgetdimid");

    /* get handle for second dimension of data set 'dimval_1_compat' */
    dimid1=SDgetdimid(sdid, 1);
    CHECK(dimid1, FAIL, "SDgetdimid");

    /* set second dimension as being backward compatible, default is
       non-compatible  */
    status = SDsetdimval_comp(dimid1, SD_DIMVAL_BW_COMP);
    CHECK(status, FAIL, "SDsetdimval_comp");

    for (i=0; i<6; i++)
        scale[i]=i*5;

    /* set the scale for the second dimension */
    status = SDsetdimscale(dimid1, 6, DFNT_INT32, scale);
    CHECK(status, FAIL, "SDsetdimscale");

    for (i=0; i<24; i++)
        idata[i] = i;

    /* write data to data set 'dimval_1_compat' in file 'test1.hdf' */
    start[0]=0;
    start[1]=0;
    end[0]=4;
    end[1]=6;
    status = SDwritedata(sdid, start, NULL, end, (VOIDP)idata);
    CHECK(status, FAIL, "SDwritedata");

    /* end access to data set 'dimval_1_compat' */
    status = SDendaccess(sdid);
    CHECK(status, FAIL, "SDendaccess");

    /* close file 'test1.hdf */
    status = SDend(f1);
    CHECK(status, FAIL, "SDend");

    /* read back and change dimval compatibility  */

    /* open file 'test1.hdf' again */
    f1 = SDstart(FILE1, DFACC_RDWR);
    CHECK(f1, FAIL, "SDstart: (again2)");

    /* get index of data set 'dimval_1_compat' in file 'test1.hdf' */
    index = SDnametoindex(f1, "dimval_1_compat");
    CHECK(index,FAIL,"SDnametoindex: failed to get index for data set 'dimval_non_compat' in file 'test1.hdf'");

    /* select data set 'dimval_1_compat' based on it's index in file */
    sdid = SDselect(f1, index);
    CHECK(sdid,FAIL,"SDselect:Failed to select data set 'dimval_1_compat' in file 'test1.hdf'");

    /* info on data set 'dimval_1_compat' */
    status = SDgetinfo(sdid, name, (int32 *)&rank, dimsize, &nt, (int32 *)&nattrs);
    CHECK(status, FAIL, "SDgetinfo");

    /* verify correctness of information */
    if (rank!=2 || dimsize[0]!=4 || dimsize[1]!=6 || nt!=DFNT_INT32) 
      {
        fprintf(stderr, "SDgetinfo returned wrong values\n");
          num_errs++;
      }

    /* get handle for first dimension of data set 'dimval_1_compat' */
    dimid=SDgetdimid(sdid,0);
    CHECK(dimid, FAIL, "SDgetdimid");

    /* get dimension info for first dimension */
    status = SDdiminfo(dimid, name, (int32 *)&dimsize[0], &nt, (int32 *)&nattrs);
    CHECK(status, FAIL, "SDdiminfo");

    /* verify correctness of information */
    if (dimsize[0]!=SD_UNLIMITED || nt!= 0 )  
      {
          fprintf(stderr, "SDdiminfo returned wrong values\n");
          num_errs++;
      }
    /* is it backward non-compatible? */
    status = SDisdimval_bwcomp(dimid);
    if (status != SD_DIMVAL_BW_INCOMP)
       { 
           fprintf(stderr, "SDisdimvalcomp returned wrong value for dimension.\n");
            num_errs++;
       }
    /* re-set first dimension as backward compatible */
    status = SDsetdimval_comp(dimid, SD_DIMVAL_BW_COMP);
    CHECK(status, FAIL, "SDsetdimval_comp");

    /* get handle for second dimension of data set 'dimval_1_compat' */
    dimid1=SDgetdimid(sdid,1);
    CHECK(dimid1, FAIL, "SDgetdimid");

    /* get dimension info for second dimension */
    status = SDdiminfo(dimid1, name, (int32 *)&dimsize[1], &nt, (int32 *)&nattrs);
    CHECK(status, FAIL, "SDdiminfo");

    /* verify correctness of information */
    if (dimsize[1]!=6 || nt!= DFNT_INT32 )  
      {
          fprintf(stderr, "Failed on SDgetinfo call\n");
          num_errs++;
      }

    /* read data back from data set 'dimval_1_compat' */
    status = SDreaddata(sdid, start, NULL, end, (VOIDP)idata);
    CHECK(status, FAIL, "SDwritedata");

    /* verify data */
    for (i=0; i<24; i++)  
      {
        if (idata[i] != i) 
          {
           fprintf(stderr, "line %d, wrong value: should be %d, got %d\n",
                           __LINE__, i, (int)idata[i]);
           num_errs++;
          }
      }

    /* see if second dimension is backward compatible. 
       should be compatible */
    status = SDisdimval_bwcomp(dimid1);
    if (status != SD_DIMVAL_BW_COMP)  
      {
          fprintf(stderr, "SDisdimvalcomp returned wrong value for dimension\n");
          num_errs++;
      }

    /* re-set second dimension as backward non-compatible */
    status = SDsetdimval_comp(dimid1, SD_DIMVAL_BW_INCOMP);
    CHECK(status, FAIL, "SDsetdimval_comp");

    /* end access to data set 'dimval_1_compat' */
    status = SDendaccess(sdid);
    CHECK(status, FAIL, "SDendaccess");

    /* close file 'test1.hdf' */
    status = SDend(f1);
    CHECK(status, FAIL, "SDend");

    /* open one last time to check that NDG ref has been constant */
    /* check SDsetdimval_compat */

    /* open file 'test1.hdf' again */
    f1 = SDstart(FILE1, DFACC_RDWR);
    CHECK(f1, FAIL, "SDstart (again3)");

    /* get index of data set 'dimval_1_compat' in file 'test1.hdf' */
    index = SDnametoindex(f1, "dimval_1_compat");
    CHECK(index,FAIL,"SDnametoindex: failed to get index for data set 'dimval_1_compat' in file 'test1.hdf'");

    /* select data set 'dimval_1_compat' based on it's index in file */
    sdid = SDselect(f1, index);
    CHECK(sdid,FAIL,"SDselect:Failed to select data set 'dimval_1_compat' in file 'test1.hdf'");

    /* info on data set 'dimval_1_compat' */
    status = SDgetinfo(sdid, name, (int32 *)&rank, dimsize, &nt, (int32 *)&nattrs);
    CHECK(status, FAIL, "SDgetinfo");

    /* verify correctness of information */
    if (rank!=2 || dimsize[0]!=4 || dimsize[1]!=6 || nt!=DFNT_INT32) 
      {
        fprintf(stderr, "SDgetinfo returned wrong values\n");
          num_errs++;
      }

    /* get handle for second dimension of data set 'dimval_1_compat' */
    dimid1=SDgetdimid(sdid,1);
    CHECK(dimid1, FAIL, "SDgetdimid");

    /* get dimension info for second dimension */
    status = SDdiminfo(dimid1, name, (int32 *)&dimsize[1], &nt, (int32 *)&nattrs);
    CHECK(status, FAIL, "SDdiminfo");

    /* verify correctness of information */
    if (dimsize[1]!=6 || nt!= DFNT_INT32 )  
      {
          fprintf(stderr, "Failed on SDgetinfo call\n");
          num_errs++;
      }

    /* see if second dimensionis backward compatible. 
       should be backward non-compatible */
    status = SDisdimval_bwcomp(dimid1);
    if (status != SD_DIMVAL_BW_INCOMP)  
      {
          fprintf(stderr, "SDisdimvalcomp returned wrong value\n");
          num_errs++;
      }
    /* re-set second dimension as backward compatible */
    status = SDsetdimval_comp(dimid1, SD_DIMVAL_BW_COMP);
    CHECK(status, FAIL, "SDsetdimval_comp");
    /* end access to data set 'dimval_1_compat' */
    status = SDendaccess(sdid);
    CHECK(status, FAIL, "SDendaccess");

    /*
     * used saved ref at the begining to retrieve the data set
     */
    
    /* get the index of the data set to which this 'ref' belongs to */
    index = SDreftoindex(f1, ndg_saved_ref);
    CHECK(index,FAIL,"SDreftoindex: failed to get index for 'ndg_saved_ref'");

    /* get handle for this data set (DataSetAlpha) */
    sdsid = SDselect(f1, index);
    CHECK(sdsid,FAIL,"SDselect: Failed to get handle for data set 'DataSetAlpha' ");

    /* check if ref of this is the same as the one saved earlier */
    if(ndg_saved_ref != SDidtoref(sdsid)) 
      {
        fprintf(stderr, "Saved NDG ref != to SDindextoref of same\n");
        num_errs++;
      }

    /* end access to data set 'DataSetAlpha' in file 'test1.hdf' */    
    status = SDendaccess(sdsid);
    CHECK(status, FAIL, "SDendaccess");

    /* close file 'test1.hdf' */
    status = SDend(f1);
    CHECK(status, FAIL, "SDend");

/****************************************************************
 * Moved testing of external storage stuff into texternal.c     *
 * -BMR, Nov 16, 2015                                           *
 ****************************************************************/

#ifdef NBIT_TEST

    /*
     * Test the N-Bit storage stuff
     */

    /* Create file 'nbit.hdf' */
    fnbit = SDstart(NBITFILE, DFACC_CREATE);
    CHECK(fnbit, FAIL, "SDstart");

    /* Create data set 'NBitDataSet' in file 'nbit.hdf' */
    nt = DFNT_INT32;
    dimsize[0] = 5;
    dimsize[1] = 5;
    newsds = SDcreate(fnbit, "NBitDataSet", nt, 2, dimsize);
    CHECK(newsds,FAIL,"SDcreate:Failed to create a new data set('NBitDataSet') for n-bit testing");

    /* Initialize data to write out */
    for(i = 0; i < 25; i++)
        idata[i] = i*10;

    /* Promote the data set 'NBitDataSet' to an NBIT data set */
    status = SDsetnbitdataset(newsds,6,7,FALSE,FALSE);
    CHECK(status, FAIL, "SDsetnbitdataset");

    /* Write data to the NBIT data set 'NBitDataSet' */
    start[0] = start[1] = 0;
    end[0]   = end[1]   = 5;
    status = SDwritedata(newsds, start, NULL, end, (VOIDP) idata);
    CHECK(status, FAIL, "SDwritedata");

    /* end access to NBIT data set 'NBitDataSet' */
    status = SDendaccess(newsds);
    CHECK(status, FAIL, "SDendaccess");

    /* need to close to flush n-bit info to file.
       hmm... */
    status = SDend(fnbit);
    CHECK(status, FAIL, "SDend");

    /* read the n-bit data back in */
    fnbit = SDstart(NBITFILE, DFACC_RDWR);
    CHECK(fnbit, FAIL, "SDstart (again)");

    /* Select the NBIT data set back in, assume it is the first one */
    newsds2 = SDselect(fnbit, 0);
    CHECK(newsds2, FAIL,"SDselect:Failed to select a data set for n-bit access");

    /* read data back in from the NBIT data set */
    start[0] = start[1] = 0;
    end[0]   = end[1]   = 5;
    status = SDreaddata(newsds2, start, NULL, end, (VOIDP) rdata);
    CHECK(status, FAIL, "SDreaddata");

    /* verify the data */
    for(i = 0; i < 25; i++)
      {
        if((idata[i]&0x7f) != rdata[i]) 
          {
            fprintf(stderr,"Bogus val in loc %d in n-bit dset want %ld got %ld\n",
        i, (long)idata[i], (long)rdata[i]);
            num_errs++;
          }
      }

    /* end access to NBIT data set */
    status = SDendaccess(newsds2);
    CHECK(status, FAIL, "SDendaccess");

    /* close file 'nbit.hdf' */
    status = SDend(fnbit);
    CHECK(status, FAIL, "SDend");

#endif /* NBIT_TEST */

#ifdef COMP_TEST
    /*
     * Test the compressed storage routines
     */
    status = test_compression();
    CHECK(status, FAIL, "test_compression");
    num_errs = num_errs + status;

#endif /* COMP_TEST */

#ifdef CHUNK_TEST
    status = test_chunk();
    CHECK(status, FAIL, "test_chunk");
    num_errs = num_errs + status;
#endif /* CHUNK_TEST */

#ifdef NETCDF_READ_TEST
    status = test_netcdf_reading();
    CHECK(status, FAIL, "test_netcdf_reading");
    num_errs = num_errs + status;
#endif /* NETCDF_READ_TEST */

    /* BMR: Added a test routine dedicated for testing dimensions.  A
       test on SDsetdimscale for an unsigned type was added while I was
       fixing bug #172.  I didn't want to add this test into the already
       very long main program so, instead, I added this routine, which can
       be appended with other dimension tests in the future.  Also, some
       day, the main program can be shortened and some of its dimension-related
       tests can be moved into this test routine (in tdims.c) - 04/18/01 */
    status = test_dimensions();
    num_errs = num_errs + status;

    /* BMR: Added a test routine dedicated for testing SDcheckempty. 09/17/04 */
    status = test_checkempty();
    num_errs = num_errs + status;

    /* BMR: Added a test routine dedicated for testing attributes (currently
  only SDsetattr with count=0, more will be moved here eventually
  (in tattributes.c) - 05/31/11 */
    status = test_attributes();
    num_errs = num_errs + status;

    /* BMR: Added a test routine dedicated for testing SDgetdatasize (in 
       tdatasizes.c) - 09/17/08 */
    status = test_datasizes();
    num_errs = num_errs + status;

    /* BMR: Added a test routine dedicated for testing SDgetdatainfo (in 
       tdatainfo.c) - 03/20/10 */
    status = test_datainfo();
    num_errs = num_errs + status;

    /* BMR: Added a test routine dedicated for testing SDgetattdatainfo
       and SDgetanndatainto (in tattdatainfo.c) - 1/7/10 */
    status = test_att_ann_datainfo();
    num_errs = num_errs + status;

    /* BMR: Added a test routine dedicated for testing SDidtype (in tidtypes.c)
       -01/21/05.  Changed file name to tmixed_apis.c and added tests for
       Vgetvgroups, VSgetvdatas, and Vgisinternal -2011 & 01/2012 */
    status = test_mixed_apis();
    num_errs = num_errs + status;

    /* BMR: Added a test routine dedicated for testing miscellaneous 
       file-related APIs (in tfiles.c) - 10/12/05 */
    status = test_files();
    num_errs = num_errs + status;

    /* BMR: Added a test routine dedicated for testing the behavior of
     * several functions when the SDS has rank=0. (in trank0.c) - 02/4/05 */
    /* BMR: SDcreate fails on Copper when rank=0.  EP decided to remove
     * this test until further study can be made on this feature.
    status = test_rank0();
    num_errs = num_errs + status; */

    /* BMR: Added a test routine dedicated for testing functionality 
       related to SDS' properties (in tsdsprops.c) - 09/12/06 */
    status = test_SDSprops();
    num_errs = num_errs + status;

    /* BMR: Added a test routine dedicated for testing functionality 
       related to coordinate variables (in tcoordvar.c) - 05/21/07 */
    status = test_coordvar();
    num_errs = num_errs + status;

    /* BMR: Added a test routine dedicated for testing functionality 
       related to external data (in textdata.c) - 10/29/15 */
    status = test_external();
    num_errs = num_errs + status;

    /* BMR: Verifies that some functions will not fail even though SZIP 
       library is not present or only decoder is available. */
    status = test_szip_compression();  /* in tszip.c */
    num_errs = num_errs + status;

    /* BMR: This test fails on some systems when the user are logged in
     * as root.  We decided to comment it out until further work can be
     * attempted. (in tsd.c) 11/04/05 */
    /* status = test_sd(); */
    /* num_errs = num_errs + status; */

    if (num_errs == 0)
        printf("*** HDF-SD test passes ***\n");
    else                                                          \
        printf("*** HDF-SD test fails ***\n");

    return num_errs;
}

#endif /* HDF */
