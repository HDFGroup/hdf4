#include "tproto.h"

#define XSIZE 10
#define YSIZE 10
#define TESTFILE "tdf24.hdf"
 
extern int num_errs;
extern int Verbocity;


/* ------------------------------- test_r24 ------------------------------- */

void test_r24()
{
    int32 xd, yd;
    int il;
    int Error;
    char buf[XSIZE][YSIZE][3];
    char buf1[XSIZE][3][YSIZE];
    char buf2[3][XSIZE][YSIZE];
    char in[XSIZE][YSIZE][3];
    char in1[XSIZE][3][YSIZE];
    char in2[3][XSIZE][YSIZE];
    int i,j,ret;
    int ref0, ref1, ref2;

    for (i=0; i < XSIZE; i++)
       for (j=0; j < YSIZE; j++) {
           buf[i][j][0] = buf[i][j][1] = buf[i][j][2] = i + j;
           buf1[i][0][j] = buf1[i][1][j] = buf1[i][2][j] = i | j;
           buf2[0][i][j] = buf2[1][i][j] = buf2[2][i][j] = i ^ j;
       }

    MESSAGE(5,printf("Writing 24bit images with differing interlacing\n"););

    ret = DF24setil(0);
    RESULT("DF24setil");
    ret = DF24putimage(TESTFILE, &(buf[0][0][0]), XSIZE, YSIZE);
    RESULT("DF24putimage");
    ref0 = DF24lastref();

    ret = DF24setil(1);
    RESULT("DF24setil");
    ret = DF24addimage(TESTFILE, &(buf1[0][0][0]), XSIZE, YSIZE);
    RESULT("DF24addimage");
    ref1 = DF24lastref();

    ret = DF24setil(2);
    RESULT("DF24setil");
    ret = DF24addimage(TESTFILE, &(buf2[0][0][0]), XSIZE, YSIZE);
    RESULT("DF24addimage");
    ref2 = DF24lastref();

    /* read image 0 */
 
    MESSAGE(5,printf("Reading and verifying 24bit images\n"););
  
    ret = DF24restart();
    RESULT("DF24restart");
    ret = DF24reqil(0);
    RESULT("DF24reqil");
    ret = DF24getdims(TESTFILE, &xd, &yd, &il);
    RESULT("DF24getdims");

    if((xd != XSIZE) || (yd != YSIZE) || il != 0) {
        fprintf(stderr, "Returned meta-data is wrong for image 0\n");
        num_errs++;
    }

    ret = DF24getimage(TESTFILE,(VOIDP)in, XSIZE, YSIZE);
    RESULT("DF24getimage");
    Error = FALSE;
     for (i=0; i< XSIZE; i++)
       for (j=0; j< YSIZE; j++)
           if (in[i][j][0] != buf[i][j][0]
               || in[i][j][1] != buf[i][j][1]
               || in[i][j][2] != buf[i][j][2]) {
               Error = TRUE;
       }

    if(Error) {
        fprintf(stderr, "Image 0 was incorrect\n");
        num_errs++;
    }

    if(ref0 != DF24lastref()) {
        fprintf(stderr, "Bogus lastref for image 0\n");
        num_errs++;
    }

    /* read image 1 */

    ret = DF24getdims(TESTFILE, &xd, &yd, &il);
    RESULT("DF24getdims");

    if((xd != XSIZE) || (yd != YSIZE) || il != 1) {
        fprintf(stderr, "Returned meta-data is wrong for image 1\n");
        num_errs++;
    }

    ret = DF24getimage(TESTFILE,(VOIDP)in, XSIZE, YSIZE);
    RESULT("DF24getimage");
    Error = FALSE;
     for (i=0; i< XSIZE; i++)
       for (j=0; j< YSIZE; j++)
           if (in[i][j][0] != buf1[i][0][j]
               || in[i][j][1] != buf1[i][1][j]
               || in[i][j][2] != buf1[i][2][j]) {
               Error = TRUE;
       }
    
    if(Error) {
        fprintf(stderr, "Image 1 was incorrect\n");
        num_errs++;
    }
    
    if(ref1 != DF24lastref()) {
        fprintf(stderr, "Bogus lastref for image 1\n");
        num_errs++;
    }


    /* read image 2 */
      
    ret = DF24getdims(TESTFILE, &xd, &yd, &il);
    RESULT("DF24getdims");

    if((xd != XSIZE) || (yd != YSIZE) || il != 2) {
        fprintf(stderr, "Returned meta-data is wrong for image 2\n");
        num_errs++;
    }

    ret = DF24getimage(TESTFILE,(VOIDP)in, XSIZE, YSIZE);
    RESULT("DF24getimage");
    Error = FALSE;
     for (i=0; i< XSIZE; i++)
       for (j=0; j< YSIZE; j++)
           if (in[i][j][0] != buf2[0][i][j]
               || in[i][j][1] != buf2[1][i][j]
               || in[i][j][2] != buf2[2][i][j]) {
               Error = TRUE;
       }
 
 
    if(Error) {
        fprintf(stderr, "Image 2 was incorrect\n");
        num_errs++;
    }
    
    if(ref2 != DF24lastref()) {
        fprintf(stderr, "Bogus lastref for image 2\n");
        num_errs++;
    }

    ret = DF24restart();
    RESULT("DF24restart");
  

    /* read image 3 */

    ret = DF24reqil(1);
    RESULT("DF24reqil");
    ret = DF24getdims(TESTFILE, &xd, &yd, &il);    
    RESULT("DF24getdims");
    
    if((xd != XSIZE) || (yd != YSIZE) || il != 0) {
        fprintf(stderr, "Returned meta-data is wrong for image 3\n");
        num_errs++;
    }

    ret = DF24getimage(TESTFILE,(VOIDP)in1, XSIZE, YSIZE);
    RESULT("DF24getimage");
    Error = FALSE;
     for (i=0; i< XSIZE; i++)
       for (j=0; j< YSIZE; j++)
           if (in1[i][0][j] != buf[i][j][0]
               || in1[i][1][j] != buf[i][j][1]
               || in1[i][2][j] != buf[i][j][2]) {
               Error = TRUE;
       }

    if(Error) {
        fprintf(stderr, "Image 3 was incorrect\n");
        num_errs++;
    }
  
    /* read image 4 */

    ret = DF24getdims(TESTFILE, &xd, &yd, &il);
    RESULT("DF24getdims");

    if((xd != XSIZE) || (yd != YSIZE) || il != 1) {
        fprintf(stderr, "Returned meta-data is wrong for image 4\n");
        num_errs++;
    }

    ret = DF24getimage(TESTFILE,(VOIDP)in1, XSIZE, YSIZE);
    RESULT("DF24getimage");
    Error = FALSE;
     for (i=0; i< XSIZE; i++)
       for (j=0; j< YSIZE; j++)
           if (in1[i][0][j] != buf1[i][0][j]
               || in1[i][1][j] != buf1[i][1][j]
               || in1[i][2][j] != buf1[i][2][j]) {
               Error = TRUE;
       }  

    if(Error) {
        fprintf(stderr, "Image 4 was incorrect\n");
        num_errs++;
    }

    /* read image 5 */
  
    ret = DF24getdims(TESTFILE, &xd, &yd, &il);
    RESULT("DF24getdims");

    if((xd != XSIZE) || (yd != YSIZE) || il != 2) {
        fprintf(stderr, "Returned meta-data is wrong for image 5\n");
        num_errs++;
    }

    ret = DF24getimage(TESTFILE,(VOIDP)in1, XSIZE, YSIZE);
    RESULT("DF24getimage");
    Error = FALSE;
     for (i=0; i< XSIZE; i++)
       for (j=0; j< YSIZE; j++)
           if (in1[i][0][j] != buf2[0][i][j]
               || in1[i][1][j] != buf2[1][i][j]
               || in1[i][2][j] != buf2[2][i][j]) {
               Error = TRUE;
       }

    if(Error) {
        fprintf(stderr, "Image 5 was incorrect\n");
        num_errs++;
    }
  

    /* read image 6 */

    ret = DF24restart();
    RESULT("DF24restart");

    ret = DF24reqil(2);
    RESULT("DF24reqil");

    ret = DF24getdims(TESTFILE, &xd, &yd, &il);
    RESULT("DF24getdims");

    if((xd != XSIZE) || (yd != YSIZE) || il != 0) {
        fprintf(stderr, "Returned meta-data is wrong for image 6\n");
        num_errs++;
    }

    ret = DF24getimage(TESTFILE,(VOIDP)in2, XSIZE, YSIZE);
    RESULT("DF24getimage");
    Error = FALSE;
     for (i=0; i< XSIZE; i++)
       for (j=0; j< YSIZE; j++)
           if (in2[0][i][j] != buf[i][j][0]
               || in2[1][i][j] != buf[i][j][1]
               || in2[2][i][j] != buf[i][j][2]) {
               Error = TRUE;
       }

    if(Error) {
        fprintf(stderr, "Image 6 was incorrect\n");
        num_errs++;
    }
    
    /* read image 7 */
    ret = DF24getdims(TESTFILE, &xd, &yd, &il);
    RESULT("DF24getdims");

    if((xd != XSIZE) || (yd != YSIZE) || il != 1) {
        fprintf(stderr, "Returned meta-data is wrong for image 7\n");
        num_errs++;
    }

    ret = DF24getimage(TESTFILE,(VOIDP)in2, XSIZE, YSIZE);
    RESULT("DF24getimage");
     for (i=0; i< XSIZE; i++)
       for (j=0; j< YSIZE; j++)
           if (in2[0][i][j] != buf1[i][0][j]
               || in2[1][i][j] != buf1[i][1][j]
               || in2[2][i][j] != buf1[i][2][j]) {
               Error = TRUE;
       }

    if(Error) {
        fprintf(stderr, "Image 7 was incorrect\n");
        num_errs++;
    }

    /* read image 8 */

    ret = DF24getdims(TESTFILE, &xd, &yd, &il);
    RESULT("DF24getdims");

    if((xd != XSIZE) || (yd != YSIZE) || il != 2) {
        fprintf(stderr, "Returned meta-data is wrong for image 8\n");
        num_errs++;
    }

    ret = DF24getimage(TESTFILE,(VOIDP)in2, XSIZE, YSIZE);
    RESULT("DF24getimage");
    Error = FALSE;
     for (i=0; i< XSIZE; i++)
       for (j=0; j< YSIZE; j++)
           if (in2[0][i][j] != buf2[0][i][j]
               || in2[1][i][j] != buf2[1][i][j]
               || in2[2][i][j] != buf2[2][i][j]) {
               Error = TRUE;
       }
 
    if(Error) {
        fprintf(stderr, "Image 8 was incorrect\n");
        num_errs++;
    }
  
}



/* ------------------------------- test_r8 -------------------------------- */

#define XD1 10
#define YD1 10
#define XD2 7
#define YD2 11

void test_r8()
{
    uint8 *im2, *ii2;
	uint8 *im1, *ii1;
    uint8 *pal1, *pal2, *ipal;

    int x,y;
    int ret, num_images=0;
    uint16 ref1, ref2, ref3;
    int32 xd, yd;
    int ispal;
    int error;

    im1 = (uint8 *) malloc(XD1 * YD1 * sizeof(uint8));
	ii1 = (uint8 *) malloc(XD1 * YD1 * sizeof(uint8));
	if(!im1 || !ii1) {
		fprintf(stderr, "Out of memory!\n");
		exit(1);
	}
	
	im2 = (uint8 *) HDgetspace(XD2 * YD2 * sizeof(uint8));
	ii2 = (uint8 *) HDgetspace(XD2 * YD2 * sizeof(uint8));
	if(!im2 || !ii2) {
		fprintf(stderr, "Out of memory!\n");
		exit(1);
	}
	
    pal1 = (uint8 *) malloc(768 * sizeof(char));
    pal2 = (uint8 *) malloc(768 * sizeof(char));
    ipal = (uint8 *) malloc(768 * sizeof(char));
	if(!ipal || !pal1 || !pal2) {
		fprintf(stderr, "Out of memory!\n");
		exit(1);	
	}

    for(x=0;x<XD1;x++)
       for (y=0;y<YD1;y++)
           im1[x * XD1 + y] = x+y;
    for(x=0;x<XD2;x++)
       for(y=0;y<YD2;y++)
           im2[x * XD2 + y] = (2*x+y)-256*((2*x+y)/256);
    for (x=0;x<256;x++) {
       pal1[3*x] = x;
       pal1[3*x + 1] = x;
       pal1[3*x + 2] = x;
       pal2[x] = x;
       pal2[x+256] = x;
       pal2[x+512] = x;
    }

    /* start here */

    MESSAGE(5,printf("Storing 8bit raster images\n"););
    ret = DFR8setpalette(pal1);
    RESULT("DFR8setpalette");

    MESSAGE(5,printf("Putting image with no compression\n"););
    ret = DFR8putimage(TESTFILE, (VOIDP)im1, XD1, YD1, 0);
    RESULT("DFR8putimage");
    num_images++;
    ref1 = DFR8lastref();

    MESSAGE(5,printf("Putting image RLE compression\n"););
    ret = DFR8addimage(TESTFILE, (VOIDP)im2, XD2, YD2, DFTAG_RLE);
    RESULT("DFR8addimage");
    num_images++;
    ref2 = DFR8lastref();

    ret = DFR8setpalette(pal2);
    RESULT("DFR8setpalette");
    MESSAGE(5,printf("Putting image IMCOMP compression\n"););
    ret = DFR8addimage(TESTFILE, (VOIDP)im2, XD2, YD2, DFTAG_IMCOMP);
    RESULT("DFR8addimage");
    num_images++;
    ref3 = DFR8lastref();

    ret = DFR8nimages(TESTFILE);
    if (ret != num_images) {
        fprintf(stderr, "        >>> WRONG NUMBER OF IMAGES <<<\n");
        num_errs++;
    }

    MESSAGE(5,printf("Reading and verifying 8bit raster images\n"););

    ret = DFR8restart();
    RESULT("DFR8restart");

    MESSAGE(5,printf("Verifying uncompressed image\n"););

    ret = DFR8getdims(TESTFILE, &xd, &yd, &ispal);
    RESULT("DFR8getdims");
    ret = DFR8getimage(TESTFILE, (uint8 *) ii1, (int32) XD1, (int32) YD1, ipal);
    RESULT("DFR8getimage");
    error = FALSE;
    check_im_pal(XD1, YD1, xd, yd, (uint8 **)im1, (uint8 **)ii1, pal1, ipal);

    MESSAGE(5,printf("Verifying RLE compressed image\n"););

    ret = DFR8getdims(TESTFILE, &xd, &yd, &ispal);
    RESULT("DFR8getdims");

    MESSAGE(5,printf("Verifying IMCOMP compressed image\n"););

    ret = DFR8getdims(TESTFILE, &xd, &yd, &ispal);
    RESULT("DFR8getdims");
    ret = DFR8getimage(TESTFILE, (uint8 *) ii2, (int32) XD2, (int32) YD2, ipal);
    RESULT("DFR8getimage");

    MESSAGE(5,printf("Rechecking RLE image\n"););

    ret = DFR8readref(TESTFILE, ref2);
    RESULT("DFR8readref");
    ret = DFR8getimage(TESTFILE, (uint8 *) ii2, (int32) XD2, (int32) YD2, ipal);
    RESULT("DFR8getimage");
    check_im_pal(XD2, YD2, XD2, YD2, (uint8 **)im2, (uint8 **)ii2, pal1, ipal);

    MESSAGE(5,printf("Changing palette of first image\n"););

    ret = DFR8writeref(TESTFILE, ref1);
    RESULT("DFR8writeref");
    ret = DFR8setpalette(pal2);
    RESULT("DFR8setpalette");
    ret = DFR8addimage(TESTFILE, (VOIDP)im1, XD1, YD1, DFTAG_RLE);
    RESULT("DFR8addimage");


    MESSAGE(5,printf("Verifying palette change\n"););

    ret = DFR8readref(TESTFILE, ref1);
    RESULT("DFR8readref");
    ret = DFR8getdims(TESTFILE, &xd, &yd, &ispal);
    RESULT("DFR8getdims");
    ret = DFR8getimage(TESTFILE, (uint8 *) ii1, (int32) XD1, (int32) YD1, ipal);
    RESULT("DFR8getimage");
    check_im_pal(XD1, YD1, xd, yd, (uint8 **)im1, (uint8 **)ii1, pal2, ipal);

}

check_im_pal(oldx, oldy, newx, newy, oldim, newim, oldpal, newpal)
    int oldx, oldy, newx, newy;
    uint8 **oldim, **newim, *oldpal, *newpal;    
{
    int error;
    int x, y;
    uint8 *op, *np;

    MESSAGE(5,printf("Checking image and palette.\n"););

    op = (uint8 *)oldim;
    np = (uint8 *)newim;
    if ( newx!=oldx || newy != oldy)
        printf("        >>> DIMENSIONS WRONG <<<\n\n");
    error = FALSE;
    for (x=0;x<oldx;x++)
       for (y=0;y<oldy;y++) {
           if ( *op != *np) { 
               printf("\t>>>Error at %d %d, orig image=%d; new image=%d<<<\n",
                                                               x, y, *op, *np);
               error = TRUE;
           }
           op++;
           np++;
    }
    if (error) {
        printf("    >>> Image is wrong. <<<\n");
        num_errs++;
    }

    error = FALSE;
    for (x=0;x<768;x++)
       if (oldpal[x] != newpal[x]) {
           printf("\t>>>Pal error at %d, orig pal %u new pal %u<<<\n",
                                                 x, oldpal[x], newpal[x]);
               error = TRUE;
       }
    if (error) {
        printf("    >>> Palette is wrong. <<<\n");
        num_errs++;
    }
}

void test_pal()
{
    int i;
    int ret;
    uint16 ref1, ref2;

    char *pal1, *pal2, *ipal;
   
    pal1 = (char *) malloc(768 * sizeof(char));
	pal2 = (char *) malloc(768 * sizeof(char));
	ipal = (char *) malloc(768 * sizeof(char));
	if(!ipal || !pal1 || !pal2) {
		fprintf(stderr, "Out of memory!\n");
		exit(1);	
	}

    for (i=0;i<256;i++) {
       pal1[3*i] = i;
       pal1[3*i + 1] = i;
       pal1[3*i + 2] = i;
       pal2[i] = i;
       pal2[i + 256] = i;
       pal2[i + 512] = i;
    }
    
    MESSAGE(5,printf("Adding palettes to a file\n"););

    ret = DFPputpal(TESTFILE, pal1, 0, "w");
    RESULT("DFPputpal");
    ref1 = DFPlastref();


    ret = DFPaddpal(TESTFILE, pal2);
    RESULT("DFPaddpal");
    ref2 = DFPlastref();

    MESSAGE(5,printf("Reading and verifying palettes\n"););

    ret = DFPrestart();
    RESULT("DFPrestart");


    /* read and check palette 1 */
    ret = DFPgetpal(TESTFILE, ipal);
    RESULT("DFPgetpal");
    for (i=0;i<768;i++)
       if (ipal[i] != pal1[i])
           printf("Error at %d, ipal %d pal1 %d\n", i, ipal[i], pal1[i]);

    if(ref1 != DFPlastref()) {
        fprintf(stderr,"  >>> DFPlastref() bad for palette 1 <<<\n");
        num_errs++;
    }


    /* read and check palette 2 */
    ret = DFPgetpal(TESTFILE, ipal);
    RESULT("DFPgetpal");
    for (i=0;i<768;i++)
       if (ipal[i] != pal2[i])
           printf("Error at %d, ipal %d pal2 %d\n", i, ipal[i], pal2[i]);

    if(ref2 != DFPlastref()) {
        fprintf(stderr,"  >>> DFPlastref() bad for palette 2 <<<\n");
        num_errs++;
    }

    /*
     * this is 4 instead of 2 since they are 8-bit pals and thus
     * list twice and generic pals 
     */

    if(DFPnpals(TESTFILE) != 4) {
        fprintf(stderr,"  >>> DFPnpals() gives wrong number <<<\n");
        num_errs++;
    }

    /* recheck palette number 2 */
    ret = DFPreadref(TESTFILE, ref2);
    RESULT("DFPreadref");
    ret = DFPgetpal(TESTFILE, ipal);
    RESULT("DFPgetpal");
    for (i=0;i<768;i++)
       if (ipal[i] != pal2[i])
           printf("Error at %d, ipal %d pal2 %d\n", i, ipal[i], pal2[i]);


    /* recheck palette number 1 */
    ret = DFPreadref(TESTFILE, ref1);
    RESULT("DFPreadref");
    ret = DFPgetpal(TESTFILE, ipal);
    RESULT("DFPgetpal");
    for (i=0;i<768;i++)
       if (ipal[i] != pal1[i])
           printf("Error at %d, ipal %d pal1 %d\n", i, ipal[i], pal1[i]);

    MESSAGE(5,printf("Modifying first palette\n"););
    for (i=0;i<256;i++)
       pal1[i+256] = 255 - i;
    ret = DFPwriteref(TESTFILE, ref1);
    RESULT("DFPwriteref");

    ret = DFPputpal(TESTFILE, pal1, 1, "a");
    RESULT("DFPputpal");

    ret = DFPreadref(TESTFILE, ref1);
    RESULT("DFPreafref");

    ret = DFPgetpal(TESTFILE, ipal);
    RESULT("DFPgetpal");
    for (i=0;i<768;i++)
       if (ipal[i] != pal1[i])
           printf("Error at %d, ipal %d pal1 %d\n", i, ipal[i], pal1[i]);
}

