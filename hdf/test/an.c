#ifdef RCSID
static char RcsId[] = "@(#)$Revision$";
#endif
/*
$Header$

$Log$
Revision 1.1  1993/04/15 20:00:05  koziol
Re-named the new tests for MS-DOS compatibility

 * Revision 1.3  1993/04/08  20:44:01  koziol
 * Minor Cray tweaks and made the tests in testhdf.c dynamic, so the only thing
 * you have to do to add a test is to add the InitTest line.  Also added the
 * nt_lend.c test for little-endian testing.
 *
 * Revision 1.2  1993/01/27  22:41:22  briand
 * Fixed problem with compiling on RS6000.
 *
 * Revision 1.1  1993/01/27  22:04:25  briand
 * Converted test files to work with master test program: testhdf
 *
 * Revision 1.6  1992/05/31  19:05:44  mfolk
 * Added int32 casts to line 127 for Convex.
 *
 * Revision 1.5  1992/05/29  19:19:08  mfolk
 * Changed pal declaration from char to uint8 for Convex.
 *
 * Revision 1.4  1992/05/27  21:46:34  chouck
 * Added a cast
 *
 * Revision 1.3  1992/03/23  16:38:33  mfolk
 * fixed minor bug in reporting success/failure of a DFR8 routine.
 *
 * Revision 1.2  1992/02/28  15:22:47  mfolk
 * *** empty log message ***
 *
 * Revision 1.1  1992/02/28  14:33:49  mfolk
 * Initial revision
 *
*/

/***********************************************************
*
* Test program: Stores annotations in a file
* Writes several SDSs and corresponding RISs to a file.
* Writes labels and descriptions for all but the first three SDSs.
* Writes labels and descriptions for all RISs.
*
*************************************************************/

#define TESTFILE "tdfan.hdf"

#include "tproto.h"

extern int num_errs;
extern int Verbocity;

#define ISFIRST    (int)1
#define NOTFIRST   (int)0
#define MAXLEN_LAB     50
#define MAXLEN_DESC  1000
#define ROWS           10
#define COLS           10
#define REPS            2   /* number of data sets to write to file */

void test_an()
{
    char labsds[MAXLEN_LAB], labris[MAXLEN_LAB],
         descsds[MAXLEN_DESC], descris[MAXLEN_DESC];
    uint8 pal[768];
    uint8 *image, *newimage;
    uint16 refnum;
    int32 ret, rank;
    int j, dimsizes[2];
    float *data;

/* set up object labels and descriptions */

    strcpy(labsds, "Object label #1: sds");
    strcpy(labris, "Object label #2: image");
    strcpy(descsds,"Object Descr #1: 1  2  3  4  5  6  7  8  9 10 11 12 \n" );
    strcat(descsds,"             13 14 15 16 17 18 19 20 **END SDS DESCR**\n");
    strcpy(descris,"Object Descr #2: A B C D E F G H I J K L \n");
    strcat(descris, "                M N O **END IMAGE DESCR **\n");

/***** generate float array and image *****/

    data = (float *) malloc(ROWS*COLS*sizeof(float));
    image = (uint8 *) malloc(ROWS*COLS*sizeof(char));
    newimage = (uint8 *) malloc(ROWS*COLS*sizeof(char));

    dimsizes[0]=ROWS; 
    dimsizes[1]=COLS;

    gen2Dfloat(ROWS, COLS, data);
    genimage(ROWS, COLS, data, image);

    ret = DFSDsetdims(2,dimsizes);
    RESULT("DFSDsetdims");

/********  Write labels and descriptions *********/
    MESSAGE(5,printf("***  Writing labels and descriptions with SDS and RIS ***\n"););
    for (j=0; j<REPS; j++) {

        /* write out scientific data set */
        ret = DFSDadddata(TESTFILE, (uint16)2,dimsizes, (void *)data);
        RESULT("DFSDadddata");

        if ((j%3) != 0) {      /* write out annotations for 2 out of every 3 */
            refnum = DFSDlastref();
            ret = DFANputlabel(TESTFILE, DFTAG_SDG, refnum, labsds);
            RESULT("DFANputlabel");
            ret = DFANputdesc(TESTFILE, DFTAG_SDG, refnum, 
                                                   descsds, strlen(descsds));
            RESULT("DFANputdesc");
        }

        ret = DFR8addimage(TESTFILE, (char *)image, COLS, ROWS, (int) NULL);
        RESULT("DFR8addimage");
        refnum = DFR8lastref();
        ret = DFANputlabel(TESTFILE, DFTAG_RIG, refnum, labris);
        RESULT("DFANputlabel");
        ret = DFANputdesc(TESTFILE,DFTAG_RIG,refnum, descris, strlen(descris));
        RESULT("DFANputdesc");
    }


/********  Read labels and descriptions *********/

    MESSAGE(5,printf("*** Reading labels and descriptions for SDS and RIS ***\n"););

    for (j=0; j<REPS; j++) {

        ret = DFSDgetdims(TESTFILE,&rank,dimsizes,3);
        RESULT("DFSDgetdims");
        refnum = DFSDlastref();

        if ((j%3) != 0)       /* read in annotations for 2 out of every 3 */
            check_lab_desc(TESTFILE, DFTAG_SDG, refnum, labsds, descsds);

        ret = DFR8getimage(TESTFILE, newimage, (int32) COLS, (int32) ROWS, pal);
        RESULT("DFR8getimage");
        refnum = DFR8lastref();
        check_lab_desc(TESTFILE, DFTAG_RIG, refnum, labris, descris);
    }

}


/****************************************************************
**
**  gen2Dfloat:  generate 2-D data array 
**
****************************************************************/
        int
gen2Dfloat(height, width, data)
int   height, width;
float *data;
{
    int i, j;
        float *pdata;

    /* store one value per row, increasing by one for each row */
        pdata = data;
    for (i=0; i< height; i++)
       for (j=0; j< width; j++)
           *pdata++ = (float) i+1;

}

/****************************************************************
**
**  genimage:  generate image from 2-D float array
**
****************************************************************/
        int
genimage(height, width, data, image)
    int   height, width;
    float *data;
    uint8 *image;
{
    int i, limit;
    float *pdata, max, min, multiplier;
    uint8  *pimage;

    limit = height*width;
    pdata = data;
    max = min = *pdata;
    for (i=0; i<limit; i++, pdata++) {
       max = (max > *pdata) ? max : *pdata;
       min = (min < *pdata) ? min : *pdata;
    }
    /* store one value per row, increasing by one for each row */
    pdata = data;
    pimage = image;
    multiplier = 255.0 /(max-min);
    for (i=0; i< limit; i++)
           *image++ = (uint8) ((*pdata++)-min)*multiplier; 

}


/****************************************************************
**
**  check_lab_desc:  read and compare label and description
**                   with expected ones
**
****************************************************************/
    int
check_lab_desc(filename, tag, ref, label, desc)
    char *filename, *label, *desc;
    uint16 tag, ref;
{
    int inlablen, indesclen, ret; 
    char inlabel[MAXLEN_LAB], *indesc;

    inlablen = ret = DFANgetlablen(TESTFILE, tag, ref);
    RESULT("DFANgetlablen");
    if (inlablen != strlen(label)) {
        printf("\t>>>BAD LABEL LENGTH.\n\t       IS: %d\n\tSHOULD BE: %d<<<\n",
                                                inlablen, strlen(label) );
        num_errs++;
    }
    ret = DFANgetlabel(TESTFILE, tag, ref, inlabel, MAXLEN_LAB);
    RESULT("DFANgetlabel");
    if (strcmp(inlabel, label)!=0) {
        printf("\t>>>BAD LABEL. \n\t       IS: %s; \n\tSHOULD BE: %s<<<\n", 
                                                            inlabel, label );
        num_errs++;
    }

    indesclen = ret = DFANgetdesclen(TESTFILE, tag, ref);
    RESULT("DFANgetdesclen");
    if (indesclen != strlen(desc)) {
        printf("\t>>>BAD DESCRIPTION LENGTH. \n\t       IS: %d", indesclen);
        printf("\n\tSHOULD BE: %d<<<\n", strlen(desc) );
        num_errs++;
    }
    else {
        indesc = (char *)malloc( indesclen+1);
        ret = DFANgetdesc(TESTFILE, tag, ref, indesc, MAXLEN_DESC);
        RESULT("DFANgetdesc");
        indesc[indesclen] = '\0';
        if (strcmp(indesc, desc)!=0) {
            printf("\t>>>BAD DESCRIPTION.\n\t      IS: %s", indesc);
            printf("\n\tSHOULD BE: %s<<<\n", desc);
            num_errs++;
        }
    }
}
