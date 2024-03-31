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

/***********************************************************
 *
 * Test program: Stores annotations in a file
 * Writes several SDSs and corresponding RISs to a file.
 * Writes labels and descriptions for all but the first three SDSs.
 * Writes labels and descriptions for all RISs.
 *
 *************************************************************/

#include "testhdf.h"

#define TESTFILE "tdfan.hdf"

#define ISFIRST     (int)1
#define NOTFIRST    (int)0
#define MAXLEN_LAB  50
#define MAXLEN_DESC 1000
#define ROWS        10
#define COLS        10
#define REPS        32 /*2*/ /* number of data sets to write to file */

static void gen2Dfloat(int height, int width, float *data);

static void genimage(int height, int width, float *data, uint8 *image);

static void check_lab_desc(uint16 tag, uint16 ref, char *label, char *desc);

void
test_an(void)
{
    char   labsds[MAXLEN_LAB], labris[MAXLEN_LAB], descsds[MAXLEN_DESC], descris[MAXLEN_DESC];
    uint8  pal[768];
    uint8 *image, *newimage;
    uint16 refnum;
    int32  ret;
    int    rank;
    int    j;
    int32  dimsizes[2];
    float *data;

    /* set up object labels and descriptions */

    strcpy(labsds, "Object label #1: sds");
    strcpy(labris, "Object label #2: image");
    strcpy(descsds, "Object Descr #1: 1  2  3  4  5  6  7  8  9 10 11 12 \n");
    strcat(descsds, "             13 14 15 16 17 18 19 20 **END SDS DESCR**\n");
    strcpy(descris, "Object Descr #2: A B C D E F G H I J K L \n");
    strcat(descris, "                M N O **END IMAGE DESCR **\n");

    /***** generate float array and image *****/

    data     = (float *)malloc(ROWS * COLS * sizeof(float));
    image    = (uint8 *)malloc(ROWS * COLS * sizeof(char));
    newimage = (uint8 *)malloc(ROWS * COLS * sizeof(char));

    dimsizes[0] = ROWS;
    dimsizes[1] = COLS;

    gen2Dfloat(ROWS, COLS, data);
    genimage(ROWS, COLS, data, image);

    ret = DFSDsetdims(2, dimsizes);
    RESULT("DFSDsetdims");

    /********  Write labels and descriptions *********/
    MESSAGE(5, printf("***  Writing labels and descriptions with SDS and RIS ***\n"););
    for (j = 0; j < REPS; j++) {

        /* write out scientific data set */
        ret = DFSDadddata(TESTFILE, 2, dimsizes, (void *)data);
        RESULT("DFSDadddata");

        if ((j % 3) != 0) { /* write out annotations for 2 out of every 3 */
            refnum = DFSDlastref();
            ret    = DFANputlabel(TESTFILE, DFTAG_SDG, refnum, labsds);
            RESULT("DFANputlabel");
            ret = DFANputdesc(TESTFILE, DFTAG_SDG, refnum, descsds, (int32)strlen(descsds));
            RESULT("DFANputdesc");
        }

        ret = DFR8addimage(TESTFILE, (void *)image, COLS, ROWS, 0);
        RESULT("DFR8addimage");
        refnum = DFR8lastref();
        ret    = DFANputlabel(TESTFILE, DFTAG_RIG, refnum, labris);
        RESULT("DFANputlabel");
        ret = DFANputdesc(TESTFILE, DFTAG_RIG, refnum, descris, (int32)strlen(descris));
        RESULT("DFANputdesc");
    }

    /********  Read labels and descriptions *********/

    MESSAGE(5, printf("*** Reading labels and descriptions for SDS and RIS ***\n"););

    for (j = 0; j < REPS; j++) {

        ret = DFSDgetdims(TESTFILE, &rank, dimsizes, 3);
        RESULT("DFSDgetdims");
        refnum = DFSDlastref();

        if ((j % 3) != 0) /* read in annotations for 2 out of every 3 */
            check_lab_desc(DFTAG_SDG, refnum, labsds, descsds);

        ret = DFR8getimage(TESTFILE, newimage, (int32)COLS, (int32)ROWS, pal);
        RESULT("DFR8getimage");
        refnum = DFR8lastref();
        check_lab_desc(DFTAG_RIG, refnum, labris, descris);
    }

    free(data);
    free(image);
    free(newimage);
}

/****************************************************************
**
**  gen2Dfloat:  generate 2-D data array
**
****************************************************************/
static void
gen2Dfloat(int height, int width, float *data)
{
    int    i, j;
    float *pdata;

    /* store one value per row, increasing by one for each row */
    pdata = data;
    for (i = 0; i < height; i++)
        for (j = 0; j < width; j++)
            *pdata++ = (float)(i + 1);
}

/****************************************************************
**
**  genimage:  generate image from 2-D float array
**
****************************************************************/
static void
genimage(int height, int width, float *data, uint8 *image)
{
    int    i, limit;
    float *pdata, max, min, multiplier;

    limit = height * width;
    pdata = data;
    max = min = *pdata;
    for (i = 0; i < limit; i++, pdata++) {
        max = (max > *pdata) ? max : *pdata;
        min = (min < *pdata) ? min : *pdata;
    }
    /* store one value per row, increasing by one for each row */
    pdata      = data;
    multiplier = 255.0F / (max - min);
    for (i = 0; i < limit; i++)
        *image++ = (uint8)(((*pdata++) - min) * multiplier);
}

/****************************************************************
**
**  check_lab_desc:  read and compare label and description
**                   with expected ones
**
****************************************************************/
static void
check_lab_desc(uint16 tag, uint16 ref, char *label, char *desc)
{
    int32 inlablen, indesclen, ret;
    char  inlabel[MAXLEN_LAB], *indesc;

    inlablen = ret = DFANgetlablen(TESTFILE, tag, ref);
    RESULT("DFANgetlablen");
    if (inlablen != (int32)strlen(label)) {
        printf("\t>>>BAD LABEL LENGTH.\n\t       IS: %d\n\tSHOULD BE: %d<<<\n", (int)inlablen,
               (int)strlen(label));
        num_errs++;
    }
    ret = DFANgetlabel(TESTFILE, tag, ref, inlabel, MAXLEN_LAB);
    RESULT("DFANgetlabel");
    if (strcmp(inlabel, label) != 0) {
        printf("\t>>>BAD LABEL. \n\t       IS: %s; \n\tSHOULD BE: %s<<<\n", inlabel, label);
        num_errs++;
    }

    indesclen = ret = DFANgetdesclen(TESTFILE, tag, ref);
    RESULT("DFANgetdesclen");
    if (indesclen != (int32)strlen(desc)) {
        printf("\t>>>BAD DESCRIPTION LENGTH. \n\t       IS: %d", (int)indesclen);
        printf("\n\tSHOULD BE: %d<<<\n", (int)strlen(desc));
        num_errs++;
    }
    else {
        indesc = (char *)malloc((size_t)indesclen + 1);
        ret    = DFANgetdesc(TESTFILE, tag, ref, indesc, MAXLEN_DESC);
        RESULT("DFANgetdesc");
        indesc[indesclen] = '\0';
        if (strcmp(indesc, desc) != 0) {
            printf("\t>>>BAD DESCRIPTION.\n\t      IS: %s", indesc);
            printf("\n\tSHOULD BE: %s<<<\n", desc);
            num_errs++;
        }
        free(indesc);
    }
}

void
test_an_2(void)
{
    char   labsds[MAXLEN_LAB], labris[MAXLEN_LAB], descsds[MAXLEN_DESC], descris[MAXLEN_DESC];
    uint8  pal[768];
    uint8 *image, *newimage;
    uint16 refnum;
    int32  ret;
    int    rank;
    int    j;
    int32  dimsizes[2];
    float *data;

    /* set up object labels and descriptions */

    strcpy(labsds, "Object label #1: sds");
    strcpy(labris, "Object label #2: image");
    strcpy(descsds, "Object Descr #1: 1  2  3  4  5  6  7  8  9 10 11 12 \n");
    strcat(descsds, "             13 14 15 16 17 18 19 20 **END SDS DESCR**\n");
    strcpy(descris, "Object Descr #2: A B C D E F G H I J K L \n");
    strcat(descris, "                M N O **END IMAGE DESCR **\n");

    /***** generate float array and image *****/

    data     = (float *)malloc(ROWS * COLS * sizeof(float));
    image    = (uint8 *)malloc(ROWS * COLS * sizeof(char));
    newimage = (uint8 *)malloc(ROWS * COLS * sizeof(char));

    dimsizes[0] = ROWS;
    dimsizes[1] = COLS;

    gen2Dfloat(ROWS, COLS, data);
    genimage(ROWS, COLS, data, image);

    ret = DFSDsetdims(2, dimsizes);
    RESULT("DFSDsetdims");

    /********  Write labels and descriptions *********/
    MESSAGE(5, printf("***  Writing labels and descriptions with SDS and RIS ***\n"););
    for (j = 0; j < REPS; j++) {

        /* write out scientific data set */
        ret = DFSDadddata(TESTFILE, 2, dimsizes, (void *)data);
        RESULT("DFSDadddata");

        if ((j % 3) != 0) { /* write out annotations for 2 out of every 3 */
            refnum = DFSDlastref();
            ret    = DFANputlabel(TESTFILE, DFTAG_SDG, refnum, labsds);
            RESULT("DFANputlabel");
            ret = DFANputdesc(TESTFILE, DFTAG_SDG, refnum, descsds, (int32)strlen(descsds));
            RESULT("DFANputdesc");
        }

        ret = DFR8addimage(TESTFILE, (void *)image, COLS, ROWS, 0);
        RESULT("DFR8addimage");
        refnum = DFR8lastref();
        ret    = DFANputlabel(TESTFILE, DFTAG_RIG, refnum, labris);
        RESULT("DFANputlabel");
        ret = DFANputdesc(TESTFILE, DFTAG_RIG, refnum, descris, (int32)strlen(descris));
        RESULT("DFANputdesc");
    }

    /********  Read labels and descriptions *********/

    MESSAGE(5, printf("*** Reading labels and descriptions for SDS and RIS ***\n"););

    for (j = 0; j < REPS; j++) {

        ret = DFSDgetdims(TESTFILE, &rank, dimsizes, 3);
        RESULT("DFSDgetdims");
        refnum = DFSDlastref();

        if ((j % 3) != 0) /* read in annotations for 2 out of every 3 */
            check_lab_desc(DFTAG_SDG, refnum, labsds, descsds);

        ret = DFR8getimage(TESTFILE, newimage, (int32)COLS, (int32)ROWS, pal);
        RESULT("DFR8getimage");
        refnum = DFR8lastref();
        check_lab_desc(DFTAG_RIG, refnum, labris, descris);
    }

    free(data);
    free(image);
    free(newimage);
}
