#ifdef RCSID
static char RcsId[] = "@(#)$Revision$";
#endif
/*
$Header$

$Log$
Revision 1.3  1992/07/15 21:48:48  sxu
No change.

 * Revision 1.2  1992/07/14  17:50:30  mlivin
 * removed Sun pixrect stuff
 *
 * Revision 1.1  1992/07/06  22:54:16  mlivin
 * Initial revision
 *
*/
/* display.c -- contains code for displaying an image using ICR 
 * this code is plucked from hdfrseq.c
 */
#include "he.h"

#define SCRX 1152
#define SCRY 900

/* HEdisplay -- stub function for displaying an image using ICR */
#ifdef PROTOTYPE
int HEdisplay(HE_CMD *cmd)
#else
int HEdisplay(cmd)
    HE_CMD *cmd;
#endif
{
#ifndef IBM6000
    register int i;
    int center = 1;
    int xwhere = 0;
    int ywhere = 0;
    int factor = 1;
    int large = 0;

    for (i = 1; i < cmd->argc; i++)
	if (cmd->argv[i][0] == '-')
	    switch (findOpt(cmd->argv[i] + 1)) 
	    {
	    case HE_HELP:
		printf("display [-position <xpos> <ypos>] [-expansion <exp>] [-large]\n");
		printf("\t-position\tImage position on console screen\n");
		printf("\t-expansion\tImage expansion factor\n");
		printf("\t-large\t\tMake image as large as posible\n");
		return HE_OK;
	    case HE_POSITION:
		center = 0;
		xwhere = atoi(cmd->argv[++i]);
		ywhere = atoi(cmd->argv[++i]);
		if ((xwhere < 0) || (xwhere > SCRX) ||
		    (ywhere < 0) || (ywhere > SCRY))
		{
		    fprintf(stderr, "Invalid position.\n");
		    return HE_FAIL;
		}
		break;

	    case HE_EXPANSION:
		factor = atoi(cmd->argv[++i]);
		if (factor < 1) factor = 1;
		break;

	    case HE_LARGE:
		large = 1;
		break;
	    case HE_NOTFOUND:
		unkOpt(cmd->argv[i]);
		return HE_FAIL;
	    case HE_AMBIG:
		ambigOpt(cmd->argv[i]);
		return HE_FAIL;
	    default:
		irrOpt(cmd->argv[i]);
		return HE_FAIL;
	    }
	else
	{
	    unkArg(cmd->argv[i]);
	    return HE_FAIL;
	}
    return display(center, xwhere, ywhere, factor, large);
#else
    printf("Display routines do not work on this platform.\n");
    return 1;
#endif
}

#ifdef PROTOTYPE
void goTo(int desc)
#else
void goTo(desc)
    int desc;
#endif
{
    /* goto element of he_desc[desc] */
    /* ask swami */

    /* right now this only works for r8 */
    DFR8readref(he_file, he_desc[desc].ref);
}


#ifndef IBM6000  /* Skip it all */

int oldcf=0;			/* old value of compression flag */
int oldx=0,oldy=0;		/* old values of xdim and ydim */
int coldx=0,coldy=0;		/* old values of xdim and ydim for CI8s */
int xdim=0,ydim=0;  		/* size of image on disk */
int xwhere,ywhere;		/* where to put it on the screen */
int ispal;
int large;			/* should make images as large as possible */
int center;			/* should center the images */
int oldxs=0,oldys=0;		/* old sizes */
int xsize=0,ysize=0;		/* what final size on screen, after blow-up */
int factor;

unsigned char rgb[768];		/* storage for a palette */
char *wherebig=NULL;	/* where to store small image */
char *wheresmall=NULL;	/* where to store image-related stuff */

#ifdef PROTOTYPE
int getSpace(void)
#else
int getSpace()
#endif
{
    /*  
     *  Don't allocate anything if the image is the same size as before.
     */
    if (oldx != xdim || oldy != ydim) {
	oldx = xdim; oldy = ydim; 
	
	if (wheresmall)
	    free(wheresmall);
	
	if (NULL == (wheresmall = (char *) HDgetspace(xdim*ydim))) {
	    printf(" Cannot allocate memory, fatal error\n");
	    exit(1);
	}
	
    }
    return(0);
}

/*************************************************************************/
/*  largeset
 *  Set up the xfact, yfact, xsize and ysize for expanding the image
 *  locally.  
 *
 */
#ifdef PROTOTYPE
int largeSet(void)
#else
int largeSet()
#endif
{
    int
	tmp;

    if (large) {
	factor = SCRX/xdim;	/* how much blow-up can we do? */
				/* calculate expansion factor  */
	tmp = SCRY/ydim;

	/* take minimum expansion factor */
	if (factor > tmp) factor = tmp;
    }
    
    xsize = factor*xdim;	/* re-calculate actual pixel dimensions */
    ysize = factor*ydim;
    
    return(factor > 1);		/* is expansion necessary? */
}

#ifdef PROTOTYPE
int display(int c, int x, int y, int f, int l)
#else
int display(c, x, y, f, l)
    int c, x, y, f, l;
#endif
{

    center = c;
    xwhere = x;
    ywhere = y;
    factor = f;
    large = l;

    if (!isRig(he_desc[he_currDesc].tag)) {

	fprintf(stderr, "Current element not a image group.\n");
	return HE_FAIL;
    }

    goTo(he_currDesc);

    if (DFR8getdims(he_file, &xdim, &ydim, &ispal)<0) {
	fprintf(stderr, "Error getting image group.\n");
	return HE_FAIL;
    }

    if (he_remote)
	getSpace();		/* get space for image in mem */


/*
*  Try to successfully load the palette and image from the file
*/
    if (DFR8getimage(he_file, wheresmall, xdim, ydim, rgb) < 0) {
	fprintf(stderr, "Error getting image group.\n");
	return HE_FAIL;
    }

    if (he_remote) 
	rImage(ispal);	/* display remote image with [palette] */

    return HE_OK;

}


/*****************************************************************************/
/*  rimage
*  Remote display of the image using the ICR.
*  Just print the codes to stdout using the protocol.
*/
#ifdef PROTOTYPE
int rImage(int usepal)
#else
int rImage(usepal)
    int usepal;
#endif
{
    int i,j,newxsize;
    unsigned char *space,*thisline;
    unsigned char *thischar;
    register unsigned char c;

/*
*  Open the window with the W command
*/

    printf("\033^W;%d;%d;%d;%d;0;rseq^",xwhere,ywhere,xdim*factor,ydim*factor);

/*
*  If a palette should be used, send it with the M command.
*/
    if (usepal) {
	(void)printf("\033^M;0;256;768;rseq^");  /* start map */

	thischar = rgb;
	for (j=0; j<768; j++) {
	    c = *thischar++;
	    if (c > 31 && c < 123) {
		putchar(c);
	    }
	    else {
		putchar((c>>6)+123);
		putchar((c & 0x3f) + 32);
	    }
	}
    }

/*
*  Send the data for the image with RLE encoding for efficiency.
*  Encode each line and send it.
*/
    space = (unsigned char *) malloc(ydim+128);
    thisline = (unsigned char *) wheresmall;

    for (i = 0; i < ydim; i++) {
        newxsize = rleIt(thisline,space,xdim);
	thisline += xdim;	/* increment to next line */

        (void)printf("\033^R;0;%d;%d;%d;rseq^",i*factor,factor,newxsize); 

        thischar = space;
        for (j = 0; j < newxsize; j++) {

/***********************************************************************/
/*  Encoding of bytes:
*
*  123 precedes #'s 0-63
*  124 precedes #'s 64-127
*  125 precedes #'s 128-191
*  126 precedes #'s 192-255
*  overall:  realchar = (specialchar - 123)*64 + (char-32)
*            specialchar = r div 64 + 123
*            char = r mod 64 + 32
*/
/***********************************************************************/

	    c = *thischar++;      /* get byte to send */

	    if (c > 31 && c < 123) {
		putchar(c);
	    }
	    else {
		putchar((c>>6)+123);
		putchar((c & 0x3f) + 32);
	    }
        }
    }

/*
*  pause for the user
*/

    free(space);
    return HE_OK;
}

/*****************************************************************************/
/* expandimg
*  copy an image memory to memory, expanding byte by byte to get a larger image.
*  no aliasing, just byte replication
*/
#ifdef PROTOTYPE
int bigImg(unsigned char *targ, unsigned char *src)
#else
int bigImg(targ, src)
    unsigned char *targ,*src;
#endif
{
    register i,j,line;
    register unsigned char *p,*q,*oldq;

    for (line = 0; line < ydim; line++) {
	p = src+line*xdim;
	oldq = q = targ+line*xsize*factor;

	for (i=0; i<xdim; i++,p++)
	    for (j=0; j<factor; j++)
		*q++ = *p;
		
	for (i=1; i<factor ; i++) {
	    DFmovmem(oldq,q,xsize); /* make one copy of the line */
	    q += xsize;
	}

    }
    return HE_OK;
}


/********************************************************************/
/*  rleit
*  compress the data to go out with a simple run-length encoded scheme.
*
*/
#ifdef PROTOTYPE
int rleIt(char *buf, char *bufto, int len)
#else
int rleIt(buf, bufto, len)
    int len;
    char *buf,*bufto;
#endif
{
    register char *p,*q,*cfoll,*clead;
    char *begp;
    int i;

    p = buf;
    cfoll = bufto;		/* place to copy to */
    clead = cfoll + 1;
	
    begp = p;
    while (len > 0) {		/* encode stuff until gone */

	q = p + 1;
	i = len-1;
	while (*p == *q && i+120 > len && i) {
	    q++;
	    i--;
	}
		
	if (q > p + 2) {	/* three in a row */
	    if (p > begp) {
		*cfoll = p - begp;
		cfoll = clead;
	    }
	    *cfoll++ = 128 | (q-p); /* len of seq */
	    *cfoll++ = *p;	/* char of seq */
	    len -= q-p;		/* subtract len of seq */
	    p = q;
	    clead = cfoll+1;
	    begp = p;
	}
	else {
	    *clead++ = *p++;	/* copy one char */
	    len--;
	    if (p > begp + 120) {
		*cfoll = p - begp;
		cfoll = clead++;
		begp = p;
	    }
	}
	
    }
/*
*  fill in last bytecount
*/
    if (p > begp) 
	*cfoll = (p - begp);
    else
	clead--;		/* don't need count position */
	
    return((int)(clead - bufto)); /* how many stored as encoded */
}

#endif /* IBM6000 */

/* end of display.c */
