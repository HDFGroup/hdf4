#ifdef RCSID
static char RcsId[] = "@(#)$Revision$";
#endif
/*
$Header$

$Log$
Revision 1.1  1992/06/09 17:35:18  mfolk
Initial revision

*/
/*****************************************************************************
*
*                         NCSA HDF version 3.2beta
*                            February 29, 1992
*
* NCSA HDF Version 3.2 source code and documentation are in the public
* domain.  Specifically, we give to the public domain all rights for future
* licensing of the source code, all resale rights, and all publishing rights.
*
* We ask, but do not require, that the following message be included in all
* derived works:
*
* Portions developed at the National Center for Supercomputing Applications at
* the University of Illinois at Urbana-Champaign.
*
* THE UNIVERSITY OF ILLINOIS GIVES NO WARRANTY, EXPRESSED OR IMPLIED, FOR THE
* SOFTWARE AND/OR DOCUMENTATION PROVIDED, INCLUDING, WITHOUT LIMITATION,
* WARRANTY OF MERCHANTABILITY AND WARRANTY OF FITNESS FOR A PARTICULAR PURPOSE
*
*****************************************************************************/

/**************************************************************************
* hdf24hdf8	Quantizes a HDF RGB 24 bit "pixel" image into a 8 bit image
*               with RGB palette and stores it as a HDF 8-bit raster image
*               file.
*
*  usage:        hdf24hdf8 r24_file hdf8_file
*
*               On Input:
*               --------
*
*               hdf24_file	- File containing the HDF RGB 24-bit
*               		      raster image.
*
*               On Output:
*               ---------
*
*               hdf8_file	- HDF file with one 8-bit raster image set,
*               		      i.e. 8-bit image, dimensions, and 
*               		      RGB palette.
*
* by:        	NCSA
* date(s):        May 89, Jun 89, Aug 89, May 90
*
****************************************************************************/

typedef	unsigned char	UCHAR;
typedef unsigned int	UINT;

#define		NCOLORS		256
#define		PALSIZE		3 * NCOLORS
#define		COMPRESSION	0		/* no compression */

#include <stdio.h>

#ifdef UNIX
#include <sys/file.h>
#endif
#ifdef MAC
#include <FCntl.h>
#include <StdLib.h>
#endif

#include "hdf.h"

#define USAGE	fprintf (stderr, "usage: hdf24hdf8 hdf24_file hdf8_file\n")

int main
  PROTO((int, char **));
int r24r8
  PROTO((int, int, unsigned char *, unsigned char *, int, unsigned char *));

#ifdef PROTOTYPE
main (int argc, char *argv[])
#else
main (argc, argv)
int argc;
char *argv[];
#endif /* PROTOTYPE */ 
{
	int i, nc;
	int x_dim, y_dim, size;
	int interlace;
	char c;
	char *ptr;
	UCHAR *r24, *r8, *pal;
	UCHAR hdfpal[PALSIZE], *p;
#ifdef UNIX
	char *malloc ();
#endif

	if (argc != 3)
	{
		USAGE;
		exit (1);
	}

	/* Get the HDF R24 image */

	if (DF24getdims(argv[1], &x_dim, &y_dim, &interlace) < 0) {
		fprintf(stderr,"error: %s is not an HDF file or ",
			argv[1]);
		fprintf(stderr,"it does not contain a R24 image\n");
		exit (-1);
	}

	size = x_dim * y_dim;

	if ((r24 = (UCHAR *) malloc (size * 3)) == NULL)
	{
		fprintf (stderr, "error: malloc to hold r24 image failed\n");
		exit (-1);
	}

	if (DF24getimage(argv[1], r24, x_dim, y_dim) < 0) {
		fprintf(stderr,"error: DF24getimage failed\n");
		exit (-1);
	}

	if ((r8 = (UCHAR *) malloc (size)) == NULL)
	{
		fprintf (stderr, "error: malloc to hold r8 image failed\n");
		exit (-1);
	}
	if ((pal = (UCHAR *) malloc (PALSIZE)) == NULL)
	{
		fprintf (stderr, "error: malloc to hold palette failed\n");
		exit (-1);
	}

	if (r24r8 (x_dim, y_dim, r24, r8, NCOLORS, pal) == -1)
	{
		fprintf (stderr, "error: quantization failed\n");
		exit (-1);
	}

		/* rearrange palette to conform to HDF requirements */
	p = hdfpal;
	for (i = 0; i < NCOLORS; i++)
	{
		*p++ = pal[i];
		*p++ = pal[i + NCOLORS];
		*p++ = pal[i + NCOLORS * 2];
	}
	if (DFR8setpalette (hdfpal) == -1)
	{
        HEprint(stderr,0);
		exit (-1);
	}
	if (DFR8putimage (argv[2], r8, x_dim, y_dim, COMPRESSION) == -1)
	{
        HEprint(stderr,0);
		exit (-1);
	}

	free ((char *) r24);
	free ((char *) r8);
	free ((char *) pal);

	return 0;
}

#ifdef PROTOTYPE
r24r8 (int xres, int yres, UCHAR *dat24, UCHAR *dat8, int cres, UCHAR *cdat)
#else
r24r8 (xres, yres, dat24, dat8, cres, cdat)
int      xres;		/* x dimension - horizontal size */
int      yres;		/* y dimension - vertical size */
UCHAR   *dat24;		/* pointer to 24 bit image in "pixel" format */
UCHAR   *dat8;		/* pointer to 8 bit image */
int      cres;		/* number of colors in the palette - use 256 */
UCHAR   *cdat;		/* pointer to palette - should be 3 * 256 bytes long */
#endif /* PROTOTYPE */ 
{
    int      ct,xct,yct;
    int      rres,rd,rr,rn,rct;
    int      gres,gd,gr,gn,gct;
    int      bres,bd,br,bn,bct;
    int      coff;
    UINT    *idat[2];
    UINT    *cp,*np;
    UCHAR   *dip,*dop,*rp,*gp,*bp;
#ifdef UNIX
    char    *malloc();
#endif

    if ((idat[0] = (UINT *)malloc(6*xres*sizeof(UINT))) == NULL)
    {   fprintf(stderr,"error: Memory allocation fault\n");
	return -1;
    }
    idat[1] = idat[0] + (3 * xres);

    rres = 6;
    gres = 7;
    bres = 6;
    coff = 2;

    rr = gr = br = 255;
    rn = rres - 1;
    gn = gres - 1;
    bn = bres - 1;

    rp = cdat + coff;
    gp = rp + cres;
    bp = gp + cres;

    for (rct=0; rct<rres; rct++)
    {   for (gct=0; gct<gres; gct++)
	{   for (bct=0; bct<bres; bct++)
	    {   *rp++ = (UCHAR)(rr * rct / rn);
		*gp++ = (UCHAR)(gr * gct / gn);
		*bp++ = (UCHAR)(br * bct / bn);
	    }
	}
    }

    rp = cdat;
    gp = rp + cres;
    bp = gp + cres;
    cp = idat[0];
    np = idat[1];
    dip = dat24;
    dop = dat8;

    for (xct=3*xres; --xct>=0; )
	*cp++ = *dip++;

    for (yct=0; yct<(yres-1); yct++)
    {
	np = idat[(yct+1)%2];
	for (xct=3*xres; --xct>=0; )
	    *np++ = *dip++;

	cp = idat[yct%2];
	np = idat[(yct+1)%2];

	if ((rct = (cp[0] * rn / rr)) > rn) rct = rn;
	if ((gct = (cp[1] * gn / gr)) > gn) gct = gn;
	if ((bct = (cp[2] * bn / br)) > bn) bct = bn;

	*dop++ = ct = (rct * gres + gct) * bres + bct + coff;

	rd = cp[0] - rp[ct];
	gd = cp[1] - gp[ct];
	bd = cp[2] - bp[ct];

	cp += 3;
	np += 3;

	cp[0]  += rd * 7 / 16;
	cp[1]  += gd * 7 / 16;
	cp[2]  += bd * 7 / 16;
	np[-3] += rd * 5 / 16;
	np[-2] += gd * 5 / 16;
	np[-1] += bd * 5 / 16;
	np[0]  += rd / 16;
	np[1]  += gd / 16;
	np[2]  += bd / 16;

	for (xct=2; xct<xres; xct++)
	{
	    if ((rct = (cp[0] * rn / rr)) > rn) rct = rn;
	    if ((gct = (cp[1] * gn / gr)) > gn) gct = gn;
	    if ((bct = (cp[2] * bn / br)) > bn) bct = bn;

	    *dop++ = ct = (rct * gres + gct) * bres + bct + coff;

	    rd = cp[0] - rp[ct];
	    gd = cp[1] - gp[ct];
	    bd = cp[2] - bp[ct];

	    cp += 3;
	    np += 3;

	    cp[0]  += rd * 7 / 16;
	    cp[1]  += gd * 7 / 16;
	    cp[2]  += bd * 7 / 16;
	    np[-6] += rd * 3 / 16;
	    np[-5] += gd * 3 / 16;
	    np[-4] += bd * 3 / 16;
	    np[-3] += rd * 5 / 16;
	    np[-2] += gd * 5 / 16;
	    np[-1] += bd * 5 / 16;
	    np[0]  += rd / 16;
	    np[1]  += gd / 16;
	    np[2]  += bd / 16;

	}

	if ((rct = (cp[0] * rn / rr)) > rn) rct = rn;
	if ((gct = (cp[1] * gn / gr)) > gn) gct = gn;
	if ((bct = (cp[2] * bn / br)) > bn) bct = bn;

	*dop++ = ct = (rct * gres + gct) * bres + bct + coff;

	rd = cp[0] - rp[ct];
	gd = cp[1] - gp[ct];
	bd = cp[2] - bp[ct];

	cp += 3;
	np += 3;

	np[-6] += rd * 3 / 16;
	np[-5] += gd * 3 / 16;
	np[-4] += bd * 3 / 16;
	np[-3] += rd * 5 / 16;
	np[-2] += gd * 5 / 16;
	np[-1] += bd * 5 / 16;
    }

    cp = idat[yct%2];

    if ((rct = (cp[0] * rn / rr)) > rn) rct = rn;
    if ((gct = (cp[1] * gn / gr)) > gn) gct = gn;
    if ((bct = (cp[2] * bn / br)) > bn) bct = bn;

    *dop++ = ct = (rct * gres + gct) * bres + bct + coff;

    rd = cp[0] - rp[ct];
    gd = cp[1] - gp[ct];
    bd = cp[2] - bp[ct];

    cp += 3;

    cp[0]  += rd * 7 / 16;
    cp[1]  += gd * 7 / 16;
    cp[2]  += bd * 7 / 16;

    for (xct=2; xct<xres; xct++)
    {
	if ((rct = (cp[0] * rn / rr)) > rn) rct = rn;
	if ((gct = (cp[1] * gn / gr)) > gn) gct = gn;
	if ((bct = (cp[2] * bn / br)) > bn) bct = bn;

	*dop++ = ct = (rct * gres + gct) * bres + bct + coff;

	rd = cp[0] - rp[ct];
	gd = cp[1] - gp[ct];
	bd = cp[2] - bp[ct];

	cp += 3;

	cp[0]  += rd * 7 / 16;
	cp[1]  += gd * 7 / 16;
	cp[2]  += bd * 7 / 16;
    }

    if ((rct = (cp[0] * rn / rr)) > rn) rct = rn;
    if ((gct = (cp[1] * gn / gr)) > gn) gct = gn;
    if ((bct = (cp[2] * bn / br)) > bn) bct = bn;

    *dop++ = (rct * gres + gct) * bres + bct + coff;

    free(idat[0]);
    return 0;
}



