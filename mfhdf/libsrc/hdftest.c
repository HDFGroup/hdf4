/****************************************************************************
 * NCSA HDF                                                                 *
 * Software Development Group                                               *
 * National Center for Supercomputing Applications                          *
 * University of Illinois at Urbana-Champaign                               *
 * 605 E. Springfield, Champaign IL 61820                                   *
 *                                                                          *
 * For conditions of distribution and use, see the accompanying             *
 * hdf/COPYING file.                                                        *
 *                                                                          *
 ****************************************************************************/

#ifdef RCSID
static char RcsId[] = "@(#)$Revision$";
#endif

/* $Id$ */

#include "mfhdf.h"

#ifdef HDF

#define CHECK(status, name) {if(status == FAIL) { printf("*** Routine %s FAILED at line %d ***\n", name, __LINE__); num_err++;}}

#define FILE1   "test1.hdf"
#define FILE2   "test2.hdf"
#define FILE3   "test3.hdf"
#define EXTFILE "exttest.hdf"

#define EXTERNAL_TEST
#define NBIT_TEST
#define CHUNK_TEST

#if defined __MWERKS__
#include <console.h>
#endif

#ifdef CHUNK_TEST
/* Dimensions of slab */
static int32  edge_dims[3]  = {2, 3, 4};  /* size of slab dims */
static int32  start_dims[3] = {0, 0, 0};  /* starting dims  */
static int32  d_dims[3]     = {2, 3, 4};
static int32  cdims[3]      = {1, 2, 3};  /* chunk lengths */

/* Chunk teseting - arrays for chunk writes */
/* float32 arrays */
static float32  chunk1_f32[4] =
        {(float32) 0.0, (float32) 1.0, (float32) 2.0, (float32) 3.0};

static float32  chunk2_f32[4] =
        {(float32) 10.0, (float32) 11.0, (float32) 12.0, (float32) 13.0};

static float32  chunk3_f32[4] =
        {(float32) 20.0, (float32) 21.0, (float32) 22.0, (float32) 23.0};

static float32  chunk4_f32[4] =
        {(float32) 100.0, (float32) 101.0, (float32) 102.0, (float32) 103.0};

static float32  chunk5_f32[4] =
        {(float32) 110.0, (float32) 111.0, (float32) 112.0, (float32) 113.0};

static float32  chunk6_f32[4] =
        {(float32) 120.0, (float32) 121.0, (float32) 122.0, (float32) 123.0};

/* uint16 arrays */
static uint16  chunk1_u16[4] =        { 0, 1, 2, 3};

static uint16  chunk2_u16[4] =        { 10, 11, 12, 13};

static uint16  chunk3_u16[4] =        { 20, 21, 22, 23};

static uint16  chunk4_u16[4] =        { 100, 101, 102, 103};

static uint16  chunk5_u16[4] =        { 110, 111, 112, 113};

static uint16  chunk6_u16[4] =        { 120, 121, 122, 123};

/* uint16 chunk arrays used in example 1 */
static uint16  chunk1_2u16[6] = {11, 21, 
                                 12, 22, 
                                 13, 23};

static uint16  chunk2_2u16[6] = {31, 41, 
                                 32, 42, 
                                 33, 43};

static uint16  chunk3_2u16[6] = {14, 24, 
                                 15, 25, 
                                 16, 26};

static uint16  chunk4_2u16[6] = {34, 44, 
                                 35, 45, 
                                 36, 46};

static uint16  chunk5_2u16[6] = {17, 27, 
                                 18, 28, 
                                 19, 29};

static uint16  chunk6_2u16[6] = {37, 47, 
                                 38, 48, 
                                 39, 49};

/* for visual layout in Example 1*/
static uint16  u16_2data[9][4] =
{ 
   {11, 21, 31, 41},
   {12, 22, 32, 42},
   {13, 23, 33, 43},
   {14, 24, 34, 44},
   {15, 25, 35, 45},
   {16, 26, 36, 46},
   {17, 27, 37, 47},
   {18, 28, 38, 48},
   {19, 29, 39, 49},
};

/* for comparison in example 1 */
static uint16 u16_2cdata[5][2] =
{
  {23, 33},
  {24, 34},
  {25, 35},
  {26, 36},
  {27, 37}
};

/* uint8 arrays */
static uint8  chunk1_u8[4] = { 0, 1, 2, 3};

static uint8  chunk2_u8[4] = { 10, 11, 12, 13};

static uint8  chunk3_u8[4] = { 20, 21, 22, 23};

static uint8  chunk4_u8[4] = { 100, 101, 102, 103};

static uint8  chunk5_u8[4] = { 110, 111, 112, 113};

static uint8  chunk6_u8[4] = { 120, 121, 122, 123};

/* data arrays layed out in memory  */
/* for comparison */
static float32  f32_data[2][3][4] =
{
    {
        {(float32) 0.0, (float32) 1.0, (float32) 2.0, (float32) 3.0},
        {(float32) 10.0, (float32) 11.0, (float32) 12.0, (float32) 13.0},
        {(float32) 20.0, (float32) 21.0, (float32) 22.0, (float32) 23.0}},
    {
        {(float32) 100.0, (float32) 101.0, (float32) 102.0, (float32) 103.0},
        {(float32) 110.0, (float32) 111.0, (float32) 112.0, (float32) 113.0},
        {(float32) 120.0, (float32) 121.0, (float32) 122.0, (float32) 123.0}}};

static uint16  u16_data[2][3][4] =
{
    {
        { 0, 1, 2, 3},
        { 10, 11, 12, 13},
        { 20, 21, 22, 23}},
    {
        { 100, 101, 102, 103},
        { 110, 111, 112, 113},
        { 120, 121, 122, 123}}};

static uint8  u8_data[2][3][4] =
{
    {
        { 0, 1, 2, 3},
        { 10, 11, 12, 13},
        { 20, 21, 22, 23}},
    {
        { 100, 101, 102, 103},
        { 110, 111, 112, 113},
        { 120, 121, 122, 123}}};


#endif /* CHUNK_TEST */

#ifdef PROTOTYPE
int main(int argc, char *argv[])
#else
int main(argc, argv)
int argc;
char *argv[];
#endif
{
    int32 f1, f2, f3, nt, dimsize[10], nattr;
    int32 newsds, newsds2, newsds3, dimid, dimid1, dimid2, number, offset;
    int32   sdsid;                /* SDS handle */
#ifdef CHUNK_TEST
    int32   newsds4, newsds5, newsds6, newsds7, newsds8;   /* Chunked SDS ids */
    float32 inbuf_f32[2][3][4];  /* float32 Data array read from from file */
    uint16  inbuf_u16[2][3][4];  /* uint16 Data array read from from file */
    uint16  inbuf1_2u16[9][4];    /* Data array read for Example 1 */
    uint16  inbuf_2u16[5][2];    /* Data array read for Example 1 */
    uint8   inbuf_u8[2][3][4];   /* uint8 Data array read from from file */
    uint8   inbuf[32];      /* input buffer */
    uint8   ru8_data[4];    /* chunk input buffer */
    int32   rcdims[3];      /* for SDgetChunkInfo() */
    uint16  fill_u16 = 0;  /* fill value */
    SD_CHUNK_DEF chunk_def; /* Chunk defintion */
    comp_info cinfo;       /* compression info */
#if 0
    float32 *wf32_data = f32_data;
    uint16  *wu16_data = u16_data;
    uint8   *wu8_data  = u8_data;
#endif
#endif /* CHUNK_TEST */
    int32 index, ival, sdid;
    int32 rank;
    intn status, i,j,k, nattrs;
    char name[90], text[256];
    int32   start[10], end[10], scale[10], stride[10];
    char    l[80], u[80], fmt[80], c[80];
    int32   count;
    int     num_err = 0;
    int32   idata[100], rdata[100];
    int16   sdata[100];
    int32  ndg_saved_ref;

    uint8  iuval;
    float32 data[1000], max, min, imax, imin;
    float64 cal, cale, ioff, ioffe;

#if defined __MWERKS__
    argc = ccommand(&argv);
#endif

    ncopts = NC_VERBOSE;

    f1 = SDstart(FILE1, DFACC_CREATE);
    CHECK(f1, "SDstart");

    f2 = SDstart(FILE2, DFACC_CREATE);
    CHECK(f2, "SDstart");

    status = SDfileinfo(f1, &number, &nattr);
    if((status != SUCCEED) && (number != 0)) {
        fprintf(stderr, "File1 still has stuff in it\n");
        num_err++;
    }
    /* create a 4 by 8 dataset */
    dimsize[0] = 4;
    dimsize[1] = 8;
    newsds = SDcreate(f1, "DataSetAlpha", DFNT_FLOAT32, 2, dimsize);
    if(newsds == FAIL) {
        fprintf(stderr, "Failed to create a new data set alpha\n");
        num_err++;
    }

    /* save the ref number for the first dataset --- will check at very end */
    ndg_saved_ref = SDidtoref(newsds);
    if(ndg_saved_ref == 0) {
        fprintf(stderr, "Failed to get NDG ref for DataSetAlpha\n");
        num_err++;
    }

    newsds3 = SDcreate(f1, "DataSetGamma", DFNT_FLOAT64, 1, dimsize);
    if(newsds3 == FAIL) {
        fprintf(stderr, "Failed to create a new data set gamma\n");
        num_err++;
    }

    status = SDfileinfo(f1, &number, &nattr);
    if(number != 2) {
        fprintf(stderr, "Wrong number of datasets in file 1\n");
        num_err++;
    }



    dimid = SDgetdimid(newsds3, 0);
    if(dimid == FAIL) {
        fprintf(stderr, "Failed to get dimension id\n");
        num_err++;
    }

    status = SDsetdimname(dimid, "MyDim");
    CHECK(status, "SDsetdimname");


    status = SDsetattr(dimid, "DimensionAttribute", DFNT_CHAR8, 
                       4, "TRUE");
    CHECK(status, "SDsetattr");
    
    status = SDfindattr(dimid, "DimensionAttribute");
    if(status != 0) {
        fprintf(stderr, "Bad index for SDfindattr on Dimension Attribute %d\n",
                status);
        num_err++;
    }

    status = SDattrinfo(dimid, (int32) 0, name, &nt, &count);
    CHECK(status, "SDattrinfo");

    status = SDreadattr(dimid, 0, text);
    CHECK(status, "SDreadattr");
    
    if(HDstrncmp(text, "TRUE", count)) {
        fprintf(stderr, "Invalid dimension attribute read <%s>\n", text);
        num_err++;
    }

    dimid = SDgetdimid(newsds, 0);
    if(dimid == FAIL) {
        fprintf(stderr, "Failed to get dimension id\n");
        num_err++;
    }

    status = SDsetdimname(dimid, "MyDim");
    CHECK(status, "SDsetdimname");

    scale[0] = 1;
    scale[1] = 5;
    scale[2] = 7;
    scale[3] = 24;
    status = SDsetdimscale(dimid, 4, DFNT_INT32, (VOIDP) scale);
    CHECK(status, "SDsetdimscale");

    status = SDsetdimstrs(dimid, "DimLabel", NULL, "TheFormat");
    CHECK(status, "SDsetdimstrs");

    /* verify that we can read the dimensions values with SDreaddata */
    start[0] = 0;
    end[0]   = 4;
    status = SDreaddata(dimid, start, NULL, end, (VOIDP) idata);
    CHECK(status, "SDreaddata");

    for(i = 0; i < 4; i++) {
        if(idata[i] != scale[i]) {
            fprintf(stderr, "SDreaddata() returned %ld not %ld in location %d\n", 
                    (long)idata[i], (long)scale[i], i);
            num_err++;
        }
    }

    /* lets store an attribute here */
    max = 3.1415;
    status = SDsetattr(dimid, "DimAttr", DFNT_FLOAT32, 1, (VOIDP) &max);
    CHECK(status, "SDsetattr");

    /* lets make sure we can read it too */
    status = SDattrinfo(dimid, 3, name, &nt, &count);
    CHECK(status, "SDattrinfo");

    if(nt != DFNT_FLOAT32) {
        fprintf(stderr, "Wrong number type for SDattrinfo(dim)\n");
        num_err++;
    }

    if(count != 1) {
        fprintf(stderr, "Wrong count for SDattrinfo(dim)\n");
        num_err++;
    }

    if(strcmp(name, "DimAttr")) {
        fprintf(stderr, "Wrong name for SDattrinfo(dim)\n");
        num_err++;
    }

    dimid2 = SDgetdimid(newsds, 1);
    if(dimid2 == FAIL) {
        fprintf(stderr, "Failed to get second dimension id\n");
        num_err++;
    }

    /* lets store an attribute without explicitly creating the coord var first */
    ival = -256;
    status = SDsetattr(dimid2, "Integer", DFNT_INT32, 1, (VOIDP) &ival);
    CHECK(status, "SDsetattr");

    /* lets make sure we can read it too */
    status = SDattrinfo(dimid2, 0, name, &nt, &count);
    CHECK(status, "SDattrinfo");

    if(nt != DFNT_INT32) {
        fprintf(stderr, "Wrong number type for SDattrinfo(dim)\n");
        num_err++;
    }

    if(count != 1) {
        fprintf(stderr, "Wrong count for SDattrinfo(dim)\n");
        num_err++;
    }

    if(strcmp(name, "Integer")) {
        fprintf(stderr, "Wrong name for SDattrinfo(dim)\n");
        num_err++;
    }
    ival = 0;
    status = SDreadattr(dimid2, 0, (VOIDP) &ival);
    CHECK(status, "SDreatattr");
    
    if(ival != -256) {
        fprintf(stderr, "Wrong value for SDreadattr(dim)\n");
        num_err++;
    }

    /* add an unsigned integer */
    iuval = 253;
    status = SDsetattr(dimid2, "UnsignedInteger", DFNT_UINT8, 1, (VOIDP) &iuval);
    CHECK(status, "SDsetattr");

    /* lets make sure we can read it too */
    status = SDattrinfo(dimid2, 1, name, &nt, &count);
    CHECK(status, "SDattrinfo");

    if(nt != DFNT_UINT8) {
        fprintf(stderr, "Wrong number type for SDattrinfo(dim)\n");
        num_err++;
    }
    if(count != 1) {
        fprintf(stderr, "Wrong count for SDattrinfo(dim)\n");
        num_err++;
    }
    if(strcmp(name, "UnsignedInteger")) {
        fprintf(stderr, "Wrong name for SDattrinfo(dim)\n");
        num_err++;
    }
    iuval = 0;
    status = SDreadattr(dimid2, 1, (VOIDP) &iuval);
    CHECK(status, "SDreatattr");
    
    if(iuval != 253) {
        fprintf(stderr, "Wrong value for SDreadattr(dim)\n");
        num_err++;
    }


    status = SDnametoindex(f1, "DataSetAlpha");
    if(status != 0) {
        fprintf(stderr, "Couldn't find data set in file 1\n");
        num_err++;
    }

    status = SDnametoindex(f2, "DataSetAlpha");
    if(status != FAIL) {
        fprintf(stderr, "Found data set in wrong file 2\n");
        num_err++;
    }

    status = SDnametoindex(f1, "BogusDataSet");
    if(status != FAIL) {
        fprintf(stderr, "Found bogus data set in file 1\n");
        num_err++;
    }

    max = -17.5;
    status = SDsetfillvalue(newsds, (VOIDP) &max);
    CHECK(status, "SDsetfillvalue");

    for(i = 0; i < 10; i++)
        data[i] = (float32) i;

    start[0] = start[1] = 1;
    end[0]   = end[1]   = 3;
    status = SDwritedata(newsds, start, NULL, end, (VOIDP) data);
    CHECK(status, "SDwritedata");

    max = 10.0;
    min = 4.6;
    status = SDsetrange(newsds, (VOIDP) &max, (VOIDP) &min);
    CHECK(status, "SDsetrange");

    status = SDgetrange(newsds, (VOIDP) &imax, (VOIDP) &imin);
    CHECK(status, "SDsetrange");

    status = SDsetattr(newsds, "spam", DFNT_CHAR8, 6, "Hi mom");
    CHECK(status, "SDsetattr");

    status = SDsetdatastrs(newsds, "TheLabel", "TheUnits", NULL, "TheCordsys");
    CHECK(status, "SDsetdatastrs");

    status = SDgetdatastrs(newsds, l, u, fmt, c, 80);
    CHECK(status, "SDgetdatastrs");

    if(HDstrcmp(l, "TheLabel")) {
        fprintf(stderr, "Bogus label returned (%s)\n", l);
        num_err++;
    }
    if(HDstrcmp(u, "TheUnits")) {
        fprintf(stderr, "Bogus units returned (%s)\n", u);
        num_err++;
    }
    if(HDstrcmp(fmt, "")) {
        fprintf(stderr, "Bogus format returned\n");
        num_err++;
    }
    if(HDstrcmp(c, "TheCordsys")) {
        fprintf(stderr, "Bogus cordsys returned\n");
        num_err++;
    }

    status = SDfindattr(newsds, "spam");
    if(status != 2) {
        fprintf(stderr, "Bad index for SDfindattr\n");
        num_err++;
    }

    status = SDfindattr(newsds, "blarf");
    if(status != -1) {
        fprintf(stderr, "SDfindattr found non-existant attribute\n");
        num_err++;
    }

    status = SDsetattr(f1, "F-attr", DFNT_CHAR8, 10, "globulator");
    CHECK(status, "SDsetattr");

    status = SDattrinfo(f1, (int32) 0, name, &nt, &count);
    CHECK(status, "SDattrinfo");

    status = SDreadattr(f1, 0, text);
    CHECK(status, "SDreadattr");
    
    if(HDstrncmp(text, "globulator", count)) {
        fprintf(stderr, "Invalid global attribute read <%s>\n", text);
        num_err++;
    }

    status = SDfileinfo(f2, &number, &nattr);
    if(number != 0) {
        fprintf(stderr, "File2 still has stuff in it\n");
        num_err++;
    }

    cal   = 1.0;
    cale  = 5.0;
    ioff  = 3.0;
    ioffe = 2.5;
    nt    = DFNT_INT8;
    status = SDsetcal(newsds3, cal, cale, ioff, ioffe, nt);
    CHECK(status, "SDsetcal");

    /* create a record variable */
    dimsize[0] = SD_UNLIMITED;
    dimsize[1] = 6;
    newsds2 = SDcreate(f2, "DataSetBeta", DFNT_INT16, 2, dimsize);
    if(newsds2 == FAIL) {
        fprintf(stderr, "Failed to create a new data set\n");
        num_err++;
    }

    status = SDfileinfo(f2, &number, &nattr);
    if(number != 1) {
        fprintf(stderr, "Wrong number of datasets in file 2\n");
        num_err++;
    }

    for(i = 0; i < 50; i++)
        sdata[i] = i;

    start[0] = start[1] = 0;
    end[0]   = 8;
    end[1]   = 6;
    status = SDwritedata(newsds2, start, NULL, end, (VOIDP) sdata);
    CHECK(status, "SDwritedata");


    start[0] = start[1] = 0;
    end[0]   = end[1]   = 3;
    status = SDreaddata(newsds, start, NULL, end, (VOIDP) data);
    CHECK(status, "SDreaddata");

    if(data[0] != -17.5) {
        fprintf(stderr, "Wrong value returned loc 0\n");
        num_err++;
    }
    if(data[3] != -17.5) {
        fprintf(stderr, "Wrong value returned loc 3\n");
        num_err++;
    }
    if(data[5] != 1.0) {
        fprintf(stderr, "Wrong value returned loc 5\n");
        num_err++;
    }
    if(data[6] != -17.5) {
        fprintf(stderr, "Wrong value returned loc 6\n");
        num_err++;
    }
    if(data[8] != 4.0) {
        fprintf(stderr, "Wrong value returned loc 8\n");
        num_err++;
    }

    for(i = 0; i < 50; i++)
        sdata[i] = 0;

    start[0] = start[1] = 1;
    end[0]   = 3;
    end[1]   = 3;
    stride[0] = 2;
    stride[1] = 2;
    status = SDreaddata(newsds2, start, stride, end, (VOIDP) sdata);
    CHECK(status, "SDreaddata");

    for(i = 0; i < 10; i++)
        printf("%d := %d\n", i, sdata[i]);
    
    cal   = 1.0;
    cale  = 5.0;
    ioff  = 3.0;
    ioffe = 2.5;
    nt    = DFNT_INT8;
    status = SDgetcal(newsds3, &cal, &cale, &ioff, &ioffe, &nt);
    CHECK(status, "SDgetcal");

    if(cal != 1.0) {
        fprintf(stderr, "Wrong calibration info\n");
        num_err++;
    }

    if(cale != 5.0) {
        fprintf(stderr, "Wrong calibration info\n");
        num_err++;
    }

    if(ioff != 3.0) {
        fprintf(stderr, "Wrong calibration info\n");
        num_err++;
    }

    if(ioffe != 2.5) {
        fprintf(stderr, "Wrong calibration info\n");
        num_err++;
    }

    if(nt != DFNT_INT8) {
        fprintf(stderr, "Wrong calibration info\n");
        num_err++;
    }

    status = SDendaccess(newsds);
    CHECK(status, "SDendaccess");

    status = SDendaccess(newsds2);
    CHECK(status, "SDendaccess");

    status = SDendaccess(newsds3);
    CHECK(status, "SDendaccess");

#ifdef EXTERNAL_TEST

    /*
     * Test the External File storage stuff
     */

    nt = DFNT_INT32 | DFNT_NATIVE;
    dimsize[0] = 5;
    dimsize[1] = 5;
    newsds = SDcreate(f1, "ExternalDataSet", nt, 2, dimsize);
    if(newsds == FAIL) {
        fprintf(stderr, "Failed to create a new data set for external promotion\n");
        num_err++;
    }

    for(i = 0; i < 25; i++)
        idata[i] = i;

    start[0] = start[1] = 0;
    end[0]   = end[1]   = 5;
    status = SDwritedata(newsds, start, NULL, end, (VOIDP) idata);
    CHECK(status, "SDwritedata");

    status = SDsetexternalfile(newsds, EXTFILE, 0);
    CHECK(status, "SDsetexternalfile");

    for(i = 0; i < 10; i++)
        idata[i] = i * 10;

    start[0] = start[1] = 0;
    end[0]   = 2;
    end[1]   = 5;
    status = SDwritedata(newsds, start, NULL, end, (VOIDP) idata);
    CHECK(status, "SDwritedata");

    status = SDendaccess(newsds);
    CHECK(status, "SDendaccess");

    /* need to close to flush external info to file */
    status = SDend(f1);
    CHECK(status, "SDend");

    f1 = SDstart(FILE1, DFACC_RDWR);
    CHECK(f1, "SDstart (again)");

    dimsize[0] = 3;
    dimsize[1] = 3;
    newsds2 = SDcreate(f1, "WrapperDataSet", nt, 2, dimsize);
    if(newsds == FAIL) {
        fprintf(stderr, "Failed to create a new data set for external wrapping\n");
        num_err++;
    }

    offset = DFKNTsize(nt) * 2;
    status = SDsetexternalfile(newsds2, EXTFILE, offset);
    CHECK(status, "SDsetexternalfile");
 
    start[0] = start[1] = 0;
    end[0]   = end[1]   = 3;
    status = SDreaddata(newsds2, start, NULL, end, (VOIDP) idata);
    CHECK(status, "SDreaddata");

    for(i = 0; i < 8; i++)
        if(idata[i] != (i + 2) * 10) {
            fprintf(stderr, "Bogus val in loc %d in wrapper dset want %d  got %ld\n", 
		    i, (i + 2) * 10, (long)idata[i]);
            num_err++;
        }

    if(idata[8] != 10) {
        fprintf(stderr, "Bogus val in last loc in wrapper dset want 10  got %ld\n",
		(long)idata[8]);
        num_err++;
    }

    status = SDendaccess(newsds2);
    CHECK(status, "SDendaccess");

#endif /* EXTERNAL_TEST */

#ifdef NBIT_TEST

    /*
     * Test the N-Bit storage stuff
     */

    f3 = SDstart(FILE3, DFACC_CREATE);
    CHECK(f1, "SDstart");

    nt = DFNT_INT32;
    dimsize[0] = 5;
    dimsize[1] = 5;
    newsds = SDcreate(f3, "NBitDataSet", nt, 2, dimsize);
    if(newsds == FAIL) {
        fprintf(stderr, "Failed to create a new data set for external promotion\n");
        num_err++;
    }

    for(i = 0; i < 25; i++)
        idata[i] = i*10;

    status = SDsetnbitdataset(newsds,6,7,FALSE,FALSE);
    CHECK(status, "SDsetnbitdataset");

    start[0] = start[1] = 0;
    end[0]   = end[1]   = 5;
    status = SDwritedata(newsds, start, NULL, end, (VOIDP) idata);
    CHECK(status, "SDwritedata");

    status = SDendaccess(newsds);
    CHECK(status, "SDendaccess");

    /* need to close to flush n-bit info to file */
    status = SDend(f3);
    CHECK(status, "SDend");

    /* read the n-bit data back in */
    f3 = SDstart(FILE3, DFACC_RDWR);
    CHECK(f1, "SDstart (again)");

    newsds2 = SDselect(f3, 0);
    if(newsds == FAIL) {
        fprintf(stderr, "Failed to select a data set for n-bit access\n");
        num_err++;
    }

    start[0] = start[1] = 0;
    end[0]   = end[1]   = 5;
    status = SDreaddata(newsds2, start, NULL, end, (VOIDP) rdata);
    CHECK(status, "SDreaddata");

    for(i = 0; i < 25; i++)
        if((idata[i]&0x7f) != rdata[i]) {
            fprintf(stderr,"Bogus val in loc %d in n-bit dset want %ld got %ld\n",
		    i, (long)idata[i], (long)rdata[i]);
            num_err++;
        }


    status = SDendaccess(newsds2);
    CHECK(status, "SDendaccess");

    status = SDend(f3);
    CHECK(status, "SDend");

#endif /* NBIT_TEST */
/* test SDsetdimval_incomp */
    dimsize[0]=SD_UNLIMITED;
    dimsize[1]=6;
    sdid = SDcreate(f1, "dimval_non_compat", DFNT_INT32, 2, dimsize);
    if (sdid == FAIL) {
       fprintf(stderr, "Fail to create dimval_non_compat in FILE1\n");
       num_err++;
    }
    dimid=SDgetdimid(sdid, 0);
/*    status = SDsetdimval_comp(dimid, SD_DIMVAL_BW_INCOMP);
    CHECK(status, "SDsetdimval_comp");
*/
    dimid1=SDgetdimid(sdid, 1);
    status = SDsetdimval_comp(dimid1, SD_DIMVAL_BW_INCOMP);
    CHECK(status, "SDsetdimval_comp");
    for (i=0; i<6; i++)
        scale[i]=i*5;
    status = SDsetdimscale(dimid1, 6, DFNT_INT32, scale);
    CHECK(status, "SDsetdimscale");
    start[0]=0;
    start[1]=0;
    end[0]=4;
    end[1]=6;
    for (i=0; i<24; i++)
        idata[i] = i;
    status = SDwritedata(sdid, start, NULL, end, (VOIDP)idata);
    CHECK(status, "SDwritedata");
    status = SDendaccess(sdid);
    CHECK(status, "SDendaccess");
    /* end of write "dimval_non_compat"  */ 
    status = SDend(f1);
    CHECK(status, "SDend");
    /* read back and change dimval compatibility  */
    f1 = SDstart(FILE1, DFACC_RDWR);
    CHECK(f1, "SDstart (again2)");
    /* read back dimval_non_compat  */
    index = SDnametoindex(f1, "dimval_non_compat");
    if(index == FAIL) {
        fprintf(stderr, "Failed on SDreftoindex call\n");
        num_err++;
    }
    sdid = SDselect(f1, index);
    if(sdid == FAIL) {
        fprintf(stderr, "Failed on SDselect call\n");
          num_err++;
    }
    status = SDgetinfo(sdid, name, (int32 *)&rank, dimsize, &nt, (int32 *)&nattrs);
    CHECK(status, "SDgetinfo");
    if (rank!=2 || dimsize[0]!=4 || dimsize[1]!=6 || nt!=DFNT_INT32) {
        fprintf(stderr, "Failed on SDgetinfo call\n");
          num_err++;
    }
    dimid=SDgetdimid(sdid,0);
    status = SDdiminfo(dimid, name, (int32 *)&dimsize[0], &nt, (int32 *)&nattrs);
    if (dimsize[0]!=SD_UNLIMITED || nt!= 0 )  {
          fprintf(stderr, "Failed on SDgetinfo call\n");
          num_err++;
    }
    dimid1=SDgetdimid(sdid,1);
    status = SDdiminfo(dimid1, name, (int32 *)&dimsize[1], &nt, (int32 *)&nattrs);
    if (dimsize[1]!=6 || nt!= DFNT_INT32 )  {
          fprintf(stderr, "Failed on SDgetinfo call\n");
          num_err++;
    }
    status = SDreaddata(sdid, start, NULL, end, (VOIDP)idata);
    CHECK(status, "SDwritedata");
    for (i=0; i<24; i++)  {
        if (idata[i] != i) {
           fprintf(stderr, "wrong value: should be %d, got %d\n",
                           i, idata[i]);
           num_err++;
        }
    }
    status = SDisdimval_bwcomp(dimid1);
    if (status != SD_DIMVAL_BW_INCOMP)  {
          fprintf(stderr, "Failed on SDisdimvalcomp call\n");
          num_err++;
    }
    status = SDsetdimval_comp(dimid1, SD_DIMVAL_BW_COMP);
    status = SDendaccess(sdid);
    CHECK(status, "SDendaccess");
    status = SDend(f1);
    CHECK(status, "SDend");
    /* open one last time to check that NDG ref has been constant */
    /* check SDsetdimval_compat */
    f1 = SDstart(FILE1, DFACC_RDWR);
    CHECK(f1, "SDstart (again3)");
    /* read back dimval_non_compat  */
    index = SDnametoindex(f1, "dimval_non_compat");
    if(index == FAIL) {
        fprintf(stderr, "Failed on SDreftoindex call\n");
        num_err++;
    }
    sdid = SDselect(f1, index);
    if(sdid == FAIL) {
        fprintf(stderr, "Failed on SDselect call\n");
          num_err++;
    }
    status = SDgetinfo(sdid, name, (int32 *)&rank, dimsize, &nt, (int32 *)&nattrs);
    CHECK(status, "SDgetinfo");
    if (rank!=2 || dimsize[0]!=4 || dimsize[1]!=6 || nt!=DFNT_INT32) {
        fprintf(stderr, "Failed on SDgetinfo call\n");
          num_err++;
    }
    dimid1=SDgetdimid(sdid,1);
    status = SDdiminfo(dimid1, name, (int32 *)&dimsize[1], &nt, (int32 *)&nattrs);
    if (dimsize[1]!=6 || nt!= DFNT_INT32 )  {
          fprintf(stderr, "Failed on SDgetinfo call\n");
          num_err++;
    }
    status = SDisdimval_bwcomp(dimid1);
    if (status != SD_DIMVAL_BW_COMP)  {
          fprintf(stderr, "Failed on SDisdimvalcomp call\n");
          num_err++;
    }

    index = SDreftoindex(f1, ndg_saved_ref);
    if(index == FAIL) {
        fprintf(stderr, "Failed on SDreftoindex call\n");
        num_err++;
    }

    sdsid = SDselect(f1, index);
    if(index == FAIL) {
        fprintf(stderr, "Failed on SDselect on index from SDreftoindex\n");
        num_err++;
    }
    
    if(ndg_saved_ref != SDidtoref(sdsid)) {
        fprintf(stderr, "Saved NDG ref != to SDindextoref of same\n");
        num_err++;
    }

    /* Close down */    
    status = SDendaccess(sdsid);
    CHECK(status, "SDendaccess");


#ifdef CHUNK_TEST

    /* create a  9x4 SDS of uint16 in file 1 */
    d_dims[0] = 9;
    d_dims[1] = 4;
    newsds8 = SDcreate(f1, "DataSetChunked_1", DFNT_UINT16, 2, d_dims);
    if(newsds8 == FAIL) 
      {
        fprintf(stderr, "Failed to create a new data set \n");
        num_err++;
      }

    /* set fill value */
    fill_u16 = 0;
    status = SDsetfillvalue(newsds8, (VOIDP) &fill_u16);
    CHECK(status, "SDsetfillvalue");

    /* Create chunked SDS 
       chunk is 3x2 which will create 6 chunks */
    cdims[0] = 3;
    cdims[1] = 2;
    status = SDsetChunk(newsds8, cdims, SD_CHUNK_LENGTHS);
    if(status == FAIL) 
      {
        fprintf(stderr, "Failed to create new chunked data set\n");
        num_err++;
      }

    /* Set Chunk cache to hold 2 chunks */
    status = SDsetChunkCache(newsds8, 2, 0);
    if(status == FAIL) 
      {
        fprintf(stderr, "SDsetChunkCache failed\n");
        num_err++;
      }


    /* Write data */
    start_dims[0] = 0;
    start_dims[1] = 0;
    edge_dims[0] = 9;
    edge_dims[1] = 4;
    status = SDwritedata(newsds8, start_dims, NULL, edge_dims, (VOIDP) u16_2data);
    if(status == FAIL) 
      {
        fprintf(stderr, "Failed to write slabw1 to new chunked data set\n");
        num_err++;
      }

    /* read a portion of data back in using SDreaddata*/
    start_dims[0] = 0;
    start_dims[1] = 0;
    edge_dims[0] = 9;
    edge_dims[1] = 4;
    status = SDreaddata(newsds8, start_dims, NULL, edge_dims, (VOIDP) inbuf1_2u16);
    CHECK(status, "SDreaddata");
    for (i = 0; i < 9; i++)
      {
        for (j = 0; j < 4; j++)
          {
              if (inbuf1_2u16[i][j] != u16_2data[i][j])
                {
                    fprintf(stderr,"inbuf1_2u16[%d][%d]=%d,",
                            i,j,inbuf1_2u16[i][j]);
                    fprintf(stderr,"u16_cdata[%d][%d]=%d,",
                            i,j,u16_2data[i][j]);
                    fprintf(stderr,"\n");
                    num_err++;
                }
          }
      }
    /* Get chunk lengths */
    status = SDgetChunkInfo(newsds8, rcdims, SD_CHUNK_LENGTHS);
    if(status == FAIL) 
      {
        fprintf(stderr, "SDgetChunkInfo failed \n");
        num_err++;
      }

    if (cdims[0] != rcdims[0] || cdims[1] != rcdims[1])
      {
        fprintf(stderr, "SDgetChunkInfo returned wrong values\n");
        num_err++;
      }

    /* check to see if SDS is chunked */
    status = SDisChunked(newsds8);
    if (status != 1)
      {
          fprintf(stderr,"SDisChunked does think SDS is chunked\n");
      }

    /* Close down this SDS*/    
    status = SDendaccess(newsds8);
    CHECK(status, "SDendaccess");

    /* 
      Example 1. 2-D 9x4 SDS of uint16 with 3x2 chunks
                 Write data using SDwriteChunk().
                 Read data using SDreaddata().
    */

    /* create a  9x4 SDS of uint16 in file 1 */
    d_dims[0] = 9;
    d_dims[1] = 4;
    newsds7 = SDcreate(f1, "DataSetChunked_2", DFNT_UINT16, 2, d_dims);
    if(newsds7 == FAIL) 
      {
        fprintf(stderr, "Failed to create a new data set \n");
        num_err++;
      }

    /* set fill value */
    fill_u16 = 0;
    status = SDsetfillvalue(newsds7, (VOIDP) &fill_u16);
    CHECK(status, "SDsetfillvalue");

    /* Create chunked SDS 
       chunk is 3x2 which will create 6 chunks */
    cdims[0] = 3;
    cdims[1] = 2;
    status = SDsetChunk(newsds7, cdims, SD_CHUNK_LENGTHS);
    if(status == FAIL) 
      {
        fprintf(stderr, "Failed to create new chunked data set\n");
        num_err++;
      }

    /* Set Chunk cache to hold 2 chunks */
    status = SDsetChunkCache(newsds7, 2, 0);
    if(status == FAIL) 
      {
        fprintf(stderr, "SDsetChunkCache failed\n");
        num_err++;
      }

    /* Write data use SDwriteChunk */

    /* Write chunk 1 */
    start_dims[0] = 0;
    start_dims[1] = 0;
    status = SDwriteChunk(newsds7, start_dims, (VOIDP) chunk1_2u16);

    /* Write chunk 4 */
    start_dims[0] = 1;
    start_dims[1] = 1;
    status = SDwriteChunk(newsds7, start_dims, (VOIDP) chunk4_2u16);

    /* Write chunk 2 */
    start_dims[0] = 0;
    start_dims[1] = 1;
    status = SDwriteChunk(newsds7, start_dims, (VOIDP) chunk2_2u16);

    /* Write chunk 5 */
    start_dims[0] = 2;
    start_dims[1] = 0;
    status = SDwriteChunk(newsds7, start_dims, (VOIDP) chunk5_2u16);

    /* Write chunk 3 */
    start_dims[0] = 1;
    start_dims[1] = 0;
    status = SDwriteChunk(newsds7, start_dims, (VOIDP) chunk3_2u16);

    /* Write chunk 6 */
    start_dims[0] = 2;
    start_dims[1] = 1;
    status = SDwriteChunk(newsds7, start_dims, (VOIDP) chunk6_2u16);
 
    /* read a portion of data back in using SDreaddata
       i.e  5x2 subset of the whole array */
    start_dims[0] = 2;
    start_dims[1] = 1;
    edge_dims[0] = 5;
    edge_dims[1] = 2;
    status = SDreaddata(newsds7, start_dims, NULL, edge_dims, (VOIDP) inbuf_2u16);
    CHECK(status, "SDreaddata");
   /* This 5x2 array should look somethink like this
         {{23, 24, 25, 26, 27},
          {33, 34, 35, 36, 37}}    
    */
    for (i = 0; i < 5; i++)
      {
        for (j = 0; j < 2; j++)
          {
              if (inbuf_2u16[i][j] != u16_2cdata[i][j])
                {
                    fprintf(stderr,"inbuf_2u16[%d][%d]=%d,",
                            i,j,inbuf_2u16[i][j]);
                    fprintf(stderr,"u16_2cdata[%d][%d]=%d,",
                            i,j,u16_2cdata[i][j]);
                    fprintf(stderr,"\n");
                    num_err++;
                }
          }
      }
    /* Get chunk lengths */
    status = SDgetChunkInfo(newsds7, rcdims, SD_CHUNK_LENGTHS);
    if(status == FAIL) 
      {
        fprintf(stderr, "SDgetChunkInfo failed \n");
        num_err++;
      }

    if (cdims[0] != rcdims[0] || cdims[1] != rcdims[1])
      {
        fprintf(stderr, "SDgetChunkInfo returned wrong values\n");
        num_err++;
      }

    /* check to see if SDS is chunked */
    status = SDisChunked(newsds7);
    if (status != 1)
      {
          fprintf(stderr,"SDisChunked does think SDS is chunked\n");
      }

    /* Close down this SDS*/    
    status = SDendaccess(newsds7);
    CHECK(status, "SDendaccess");

    /* 
      So far 3 differnet number types are tested with 3-D arrays
      */

    /* create a new chunked SDS of float32 in file 1 */
    d_dims[0] = 2;
    d_dims[1] = 3;
    d_dims[2] = 4;
    newsds4 = SDcreate(f1, "DataSetChunk1", DFNT_FLOAT32, 3, d_dims);
    if(newsds4 == FAIL) 
      {
        fprintf(stderr, "Failed to create a new data set \n");
        num_err++;
      }

    max = 0.0;
    status = SDsetfillvalue(newsds4, (VOIDP) &max);
    CHECK(status, "SDsetfillvalue");

    /* Set chunking */
    cdims[0] = 1;
    cdims[1] = 2;
    cdims[2] = 3;
    status = SDsetChunk(newsds4, cdims, SD_CHUNK_LENGTHS);
    if(status == FAIL) 
      {
        fprintf(stderr, "Failed to create new chunked data set\n");
        num_err++;
      }

    /* Write data out */
    start_dims[0] = 0;
    start_dims[1] = 0;
    start_dims[2] = 0;
    edge_dims[0] = 2;
    edge_dims[1] = 3;
    edge_dims[2] = 4;
    status = SDwritedata(newsds4, start_dims, NULL, edge_dims, (VOIDP) f32_data);
    if(status == FAIL) 
      {
        fprintf(stderr, "Failed to write slabw1 to new chunked data set\n");
        num_err++;
      }

    /* read data back in */
    start_dims[0] = 0;
    start_dims[1] = 0;
    start_dims[2] = 0;
    edge_dims[0] = 2;
    edge_dims[1] = 3;
    edge_dims[2] = 4;
    status = SDreaddata(newsds4, start_dims, NULL, edge_dims, (VOIDP) inbuf_f32);
    CHECK(status, "SDreaddata");

    for (i = 0; i < d_dims[0]; i++)
      {
        for (j = 0; j < d_dims[1]; j++)
          {
            for (k = 0; k < d_dims[2]; k++)
              {
                  if (inbuf_f32[i][j][k] != f32_data[i][j][k])
                    {
                        fprintf(stderr,"inbuf_f32[%d][%d][%d]=%f,",
                                i,j,k,inbuf_f32[i][j][k]);
                        fprintf(stderr,"f32_data[%d][%d][%d]=%f,",
                                i,j,k,f32_data[i][j][k]);
                        fprintf(stderr,"\n");
                        num_err++;
                    }
              }
          }
      }
    /* check to see if SDS is chunked */
    status = SDisChunked(newsds4);
    if (status != 1)
      {
          fprintf(stderr,"SDisChunked does think SDS is chunked\n");
      }

    /* Close down SDS*/    
    status = SDendaccess(newsds4);
    CHECK(status, "SDendaccess");


    /* create a new chunked SDS of uint16 in file 1 */
    d_dims[0] = 2;
    d_dims[1] = 3;
    d_dims[2] = 4;
    newsds5 = SDcreate(f1, "DataSetChunk2", DFNT_UINT16, 3, d_dims);
    if(newsds5 == FAIL) 
      {
        fprintf(stderr, "Failed to set a new data set chunked\n");
        num_err++;
      }

    /* set fill value */
    fill_u16 = 0;
    status = SDsetfillvalue(newsds5, (VOIDP) &fill_u16);
    CHECK(status, "SDsetfillvalue");

    /* Set chunking, chunk is 1x2x3 */
    cdims[0] = 1;
    cdims[1] = 2;
    cdims[2] = 3;
    status = SDsetChunk(newsds5, cdims, SD_CHUNK_LENGTHS);
    if(status == FAIL) 
      {
        fprintf(stderr, "Failed to create new chunked data set\n");
        num_err++;
      }

    /* Set Chunk cache */
    status = SDsetChunkCache(newsds5, 4, 0);
    if(status == FAIL) 
      {
        fprintf(stderr, "SDsetChunkCache failed\n");
        num_err++;
      }

    /* Write data */
    start_dims[0] = 0;
    start_dims[1] = 0;
    start_dims[2] = 0;
    edge_dims[0] = 2;
    edge_dims[1] = 3;
    edge_dims[2] = 4;
    status = SDwritedata(newsds5, start_dims, NULL, edge_dims, (VOIDP) u16_data);
    if(status == FAIL) 
      {
        fprintf(stderr, "Failed to write slabw1 to new chunked data set\n");
        num_err++;
      }

    /* read data back in */
    start_dims[0] = 0;
    start_dims[1] = 0;
    start_dims[2] = 0;
    edge_dims[0] = 2;
    edge_dims[1] = 3;
    edge_dims[2] = 4;
    status = SDreaddata(newsds5, start_dims, NULL, edge_dims, (VOIDP) inbuf_u16);
    CHECK(status, "SDreaddata");
    for (i = 0; i < d_dims[0]; i++)
      {
        for (j = 0; j < d_dims[1]; j++)
          {
            for (k = 0; k < d_dims[2]; k++)
              {
                  if (inbuf_u16[i][j][k] != u16_data[i][j][k])
                    {
                        fprintf(stderr,"inbuf_u16[%d][%d][%d]=%d,",
                                i,j,k,inbuf_u16[i][j][k]);
                        fprintf(stderr,"u16_data[%d][%d][%d]=%d,",
                                i,j,k,u16_data[i][j][k]);
                        fprintf(stderr,"\n");
                        num_err++;
                    }
              }
          }
      }
    /* Check chunk info */
    status = SDgetChunkInfo(newsds5, rcdims, SD_CHUNK_LENGTHS);
    if(status == FAIL) 
      {
        fprintf(stderr, "SDgetChunkInfo failed \n");
        num_err++;
      }

    if (cdims[0] != rcdims[0] || cdims[1] != rcdims[1] || cdims[2] != rcdims[2])
      {
        fprintf(stderr, "SDgetChunkInfo returned wrong values\n");
        num_err++;
      }

    /* Close down SDS*/    
    status = SDendaccess(newsds5);
    CHECK(status, "SDendaccess");


    /* create a new chunked SDS of uint8 in file 1 */
    d_dims[0] = 2;
    d_dims[1] = 3;
    d_dims[2] = 4;
    newsds6 = SDcreate(f1, "DataSetChunk3", DFNT_UINT8, 3, d_dims);
    if(newsds6 == FAIL) 
      {
        fprintf(stderr, "Failed to set a new data set chunked\n");
        num_err++;
      }

    /* Set chunking, chunk is 1x1x4 */
    cdims[0] = 1;
    cdims[1] = 1;
    cdims[2] = 4;
    status = SDsetChunk(newsds6, cdims, SD_CHUNK_LENGTHS);
    if(status == FAIL) 
      {
        fprintf(stderr, "Failed to create new chunked data set\n");
        num_err++;
      }

#if 0
    /* Write data using SDwritedata() */
    start_dims[0] = 0;
    start_dims[1] = 0;
    start_dims[2] = 0;
    edge_dims[0] = 2;
    edge_dims[1] = 3;
    edge_dims[2] = 4;
    status = SDwritedata(newsds6, start_dims, NULL, edge_dims, (VOIDP) wu8_data);
    if(status == FAIL) 
      {
        fprintf(stderr, "Failed to write wu8_data to new chunked data set\n");
        num_err++;
      }
#endif

    /* Write data use SDwriteChunk */
    start_dims[0] = 0;
    start_dims[1] = 0;
    start_dims[2] = 0;
    status = SDwriteChunk(newsds6, start_dims, (VOIDP) chunk1_u8);
    if(status == FAIL) 
      {
        fprintf(stderr, "SDwriteChunk failed to write chunk 1\n");
        num_err++;
      }

    start_dims[0] = 1;
    start_dims[1] = 0;
    start_dims[2] = 0;
    status = SDwriteChunk(newsds6, start_dims, (VOIDP) chunk4_u8);
    if(status == FAIL) 
      {
        fprintf(stderr, "SDwriteChunk failed to write chunk 4\n");
        num_err++;
      }

    start_dims[0] = 0;
    start_dims[1] = 1;
    start_dims[2] = 0;
    status = SDwriteChunk(newsds6, start_dims, (VOIDP) chunk2_u8);
    if(status == FAIL) 
      {
        fprintf(stderr, "SDwriteChunk failed to write chunk 2\n");
        num_err++;
      }

    start_dims[0] = 1;
    start_dims[1] = 1;
    start_dims[2] = 0;
    status = SDwriteChunk(newsds6, start_dims, (VOIDP) chunk5_u8);
    if(status == FAIL) 
      {
        fprintf(stderr, "SDwriteChunk failed to write chunk 5\n");
        num_err++;
      }

    start_dims[0] = 0;
    start_dims[1] = 2;
    start_dims[2] = 0;
    status = SDwriteChunk(newsds6, start_dims, (VOIDP) chunk3_u8);
    if(status == FAIL) 
      {
        fprintf(stderr, "SDwriteChunk failed to write chunk 3\n");
        num_err++;
      }

    start_dims[0] = 1;
    start_dims[1] = 2;
    start_dims[2] = 0;
    status = SDwriteChunk(newsds6, start_dims, (VOIDP) chunk6_u8);
    if(status == FAIL) 
      {
        fprintf(stderr, "SDwriteChunk failed to write chunk 6\n");
        num_err++;
      }

    /* read data back in */
    start_dims[0] = 0;
    start_dims[1] = 0;
    start_dims[2] = 0;
    edge_dims[0] = 2;
    edge_dims[1] = 3;
    edge_dims[2] = 4;
    status = SDreaddata(newsds6, start_dims, NULL, edge_dims, (VOIDP) inbuf_u8);
    CHECK(status, "SDreaddata");
    for (i = 0; i < d_dims[0]; i++)
      {
        for (j = 0; j < d_dims[1]; j++)
          {
            for (k = 0; k < d_dims[2]; k++)
              {
                  if (inbuf_u8[i][j][k] != u8_data[i][j][k])
                    {
                        fprintf(stderr,"inbuf_u8[%d][%d][%d]=%d,",
                                i,j,k,inbuf_u8[i][j][k]);
                        fprintf(stderr,"u8_data[%d][%d][%d]=%d,",
                                i,j,k,u8_data[i][j][k]);
                        fprintf(stderr,"\n");
                        num_err++;
                    }
              }
          }
      }

    start_dims[0] = 0;
    start_dims[1] = 0;
    start_dims[2] = 0;
    status = SDreadChunk(newsds6, start_dims, (VOIDP) ru8_data);
    if(status == FAIL) 
      {
        fprintf(stderr, "SDreadChunk failed to read chunk 1\n");
        num_err++;
      }

    /* Verify chunk */
    for (i = 0; i < 4; i++)
       {
          if (ru8_data[i] != chunk1_u8[i])
            {
                printf("chunk1_u8: Wrong data at %d, out %d in %d\n", 
                 i, chunk1_u8[i], ru8_data[i]);
                num_err++;
            }
       }

    start_dims[0] = 0;
    start_dims[1] = 1;
    start_dims[2] = 0;
    status = SDreadChunk(newsds6, start_dims, (VOIDP) ru8_data);
    if(status == FAIL) 
      {
        fprintf(stderr, "SDreadChunk failed to read chunk 2\n");
        num_err++;
      }

    /* Verify chunk */
    for (i = 0; i < 4; i++)
       {
          if (ru8_data[i] != chunk2_u8[i])
            {
                printf("chunk2_u8: Wrong data at %d, out %d in %d\n", 
                 i, chunk2_u8[i], ru8_data[i]);
                num_err++;
            }
       }
    start_dims[0] = 0;
    start_dims[1] = 2;
    start_dims[2] = 0;
    status = SDreadChunk(newsds6, start_dims, (VOIDP) ru8_data);
    if(status == FAIL) 
      {
        fprintf(stderr, "SDreadChunk failed to read chunk 3\n");
        num_err++;
      }

    /* Verify chunk */
    for (i = 0; i < 4; i++)
       {
          if (ru8_data[i] != chunk3_u8[i])
            {
                printf("chunk3_u8: Wrong data at %d, out %d in %d\n", 
                 i, chunk3_u8[i], ru8_data[i]);
                num_err++;
            }
       }

    start_dims[0] = 1;
    start_dims[1] = 0;
    start_dims[2] = 0;
    status = SDreadChunk(newsds6, start_dims, (VOIDP) ru8_data);
    if(status == FAIL) 
      {
        fprintf(stderr, "SDreadChunk failed to read chunk 4\n");
        num_err++;
      }

    /* Verify chunk */
    for (i = 0; i < 4; i++)
       {
          if (ru8_data[i] != chunk4_u8[i])
            {
                printf("chunk4_u8: Wrong data at %d, out %d in %d\n", 
                 i, chunk4_u8[i], ru8_data[i]);
                num_err++;
            }
       }

    start_dims[0] = 1;
    start_dims[1] = 1;
    start_dims[2] = 0;
    status = SDreadChunk(newsds6, start_dims, (VOIDP) ru8_data);
    if(status == FAIL) 
      {
        fprintf(stderr, "SDreadChunk failed to read chunk 5\n");
        num_err++;
      }

    /* Verify chunk */
    for (i = 0; i < 4; i++)
       {
          if (ru8_data[i] != chunk5_u8[i])
            {
                printf("chunk5_u8: Wrong data at %d, out %d in %d\n", 
                 i, chunk5_u8[i], ru8_data[i]);
                num_err++;
            }
       }

    start_dims[0] = 1;
    start_dims[1] = 2;
    start_dims[2] = 0;
    status = SDreadChunk(newsds6, start_dims, (VOIDP) ru8_data);
    if(status == FAIL) 
      {
        fprintf(stderr, "SDreadChunk failed to read chunk 6\n");
        num_err++;
      }

    /* Verify chunk */
    for (i = 0; i < 4; i++)
       {
          if (ru8_data[i] != chunk6_u8[i])
            {
                printf("chunk6_u8: Wrong data at %d, out %d in %d\n", 
                 i, chunk6_u8[i], ru8_data[i]);
                num_err++;
            }
       }

    /* Close down SDS*/    
    status = SDendaccess(newsds6);
    CHECK(status, "SDendaccess");


    /* ---------------------------------------------------------------
     *  Chunking with Compression 
     ----------------------------------------------------------------*/

    /* create a  9x4 SDS of uint16 in file 1 */
    d_dims[0] = 9;
    d_dims[1] = 4;
    newsds8 = SDcreate(f1, "DataSetChunked_GZIP_5", DFNT_UINT16, 2, d_dims);
    if(newsds8 == FAIL) 
      {
        fprintf(stderr, "Failed to create a new data set \n");
        num_err++;
      }

    /* set fill value */
    fill_u16 = 0;
    status = SDsetfillvalue(newsds8, (VOIDP) &fill_u16);
    CHECK(status, "SDsetfillvalue");

    /* Create chunked SDS 
       chunk is 3x2 which will create 6 chunks.
       Use GZIP compression */
    cdims[0] = 3;
    cdims[1] = 2;
    chunk_def.chunk_lengths = cdims;
    chunk_def.comp_type = COMP_CODE_RLE;
    chunk_def.cinfo = &cinfo;
    status = SDsetChunk(newsds8, &chunk_def, SD_CHUNK_COMP);
    if(status == FAIL) 
      {
        fprintf(stderr, "Failed to create new chunked, Compressed data set\n");
        num_err++;
      }

    /* Set Chunk cache to hold 2 chunks */
    status = SDsetChunkCache(newsds8, 2, 0);
    if(status == FAIL) 
      {
        fprintf(stderr, "SDsetChunkCache failed\n");
        num_err++;
      }


    /* Write data */
    start_dims[0] = 0;
    start_dims[1] = 0;
    edge_dims[0] = 9;
    edge_dims[1] = 4;
    status = SDwritedata(newsds8, start_dims, NULL, edge_dims, (VOIDP) u16_2data);
    if(status == FAIL) 
      {
        fprintf(stderr, "Failed to write slabw1 to new chunked data set\n");
        num_err++;
      }

    /* read a portion of data back in using SDreaddata*/
    start_dims[0] = 0;
    start_dims[1] = 0;
    edge_dims[0] = 9;
    edge_dims[1] = 4;
    status = SDreaddata(newsds8, start_dims, NULL, edge_dims, (VOIDP) inbuf1_2u16);
    CHECK(status, "SDreaddata");
    for (i = 0; i < 9; i++)
      {
        for (j = 0; j < 4; j++)
          {
              if (inbuf1_2u16[i][j] != u16_2data[i][j])
                {
                    fprintf(stderr,"inbuf1_2u16[%d][%d]=%d,",
                            i,j,inbuf1_2u16[i][j]);
                    fprintf(stderr,"u16_cdata[%d][%d]=%d,",
                            i,j,u16_2data[i][j]);
                    fprintf(stderr,"\n");
                    num_err++;
                }
          }
      }
    /* Get chunk lengths */
    status = SDgetChunkInfo(newsds8, rcdims, SD_CHUNK_LENGTHS);
    if(status == FAIL) 
      {
        fprintf(stderr, "SDgetChunkInfo failed \n");
        num_err++;
      }

    if (cdims[0] != rcdims[0] || cdims[1] != rcdims[1])
      {
        fprintf(stderr, "SDgetChunkInfo returned wrong values\n");
        num_err++;
      }

    /* check to see if SDS is chunked */
    status = SDisChunked(newsds8);
    if (status != 1)
      {
          fprintf(stderr,"SDisChunked does think SDS is chunked\n");
      }

    /* Close down this SDS*/    
    status = SDendaccess(newsds8);
    CHECK(status, "SDendaccess");

    /*
     * Chunking with NBIT
     */
#if 0 /* NOT YET */
    f3 = SDstart(FILE3, DFACC_RDWR);
    CHECK(f1, "SDstart");

    nt = DFNT_INT32;
    dimsize[0] = 5;
    dimsize[1] = 5;
    newsds = SDcreate(f3, "Chunked_NBitDataSet", nt, 2, dimsize);
    if(newsds == FAIL) {
        fprintf(stderr, "Failed to create a new chunked, nbit data set \n");
        num_err++;
    }

    for(i = 0; i < 25; i++)
        idata[i] = i*10;

    /* Create chunked SDS 
       chunk is 2x2 which will create 9 chunks.
       Use NBIT compression */
    cdims[0] = 2;
    cdims[1] = 2;
    chunk_def.chunk_lengths = cdims;
    chunk_def.start_bit = 6;
    chunk_def.bit_len   = 7;
    chunk_def.sign_ext  = FALSE;
    chunk_def.fill_one  = FALSE;
    status = SDsetChunk(newsds, &chunk_def, SD_CHUNK_NBIT);
    if(status == FAIL) 
      {
        fprintf(stderr, "Failed to create new chunked, NBIT data set\n");
        num_err++;
      }
#if 0
    status = SDsetnbitdataset(newsds,6,7,FALSE,FALSE);
    CHECK(status, "SDsetnbitdataset");
#endif
    start[0] = start[1] = 0;
    end[0]   = end[1]   = 5;
    status = SDwritedata(newsds, start, NULL, end, (VOIDP) idata);
    CHECK(status, "SDwritedata");

    status = SDendaccess(newsds);
    CHECK(status, "SDendaccess");

    /* need to close to flush n-bit info to file */
    status = SDend(f3);
    CHECK(status, "SDend");

    /* read the n-bit data back in */
    f3 = SDstart(FILE3, DFACC_RDWR);
    CHECK(f1, "SDstart (again)");

    newsds2 = SDselect(f3, 0);
    if(newsds == FAIL) {
        fprintf(stderr, "Failed to select a data set for n-bit access\n");
        num_err++;
    }

    start[0] = start[1] = 0;
    end[0]   = end[1]   = 5;
    status = SDreaddata(newsds2, start, NULL, end, (VOIDP) rdata);
    CHECK(status, "SDreaddata");

    for(i = 0; i < 25; i++)
        if((idata[i]&0x7f) != rdata[i]) {
            fprintf(stderr,"Bogus val in loc %d in n-bit dset want %ld got %ld\n",
		    i, (long)idata[i], (long)rdata[i]);
            num_err++;
        }


    status = SDendaccess(newsds2);
    CHECK(status, "SDendaccess");

    status = SDend(f3);
    CHECK(status, "SDend");
#endif /* CHUNKING with NBIT */

#endif /* CHUNK_TEST */

    status = SDend(f1);
    CHECK(status, "SDend");

    status = SDend(f2);
    CHECK(status, "SDend");

    printf("num_err == %d\n", num_err);

    exit(0);
}

#endif /* HDF */


