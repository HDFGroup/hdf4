#ifdef RCSID
static char RcsId[] = "@(#)$Revision$";
#endif
/*
$Header$

$Log$
Revision 1.1  1992/07/01 20:14:53  mlivin
Initial revision

 * Revision 3.2  1991/10/22  17:56:10  dilg
 * 5
 * HDF3.1r5
 *
 * New machine types added:
 *
 *         PC      - IBM PC (DOS)
 *         WIN     - IBM PC (Microsoft Windows 3.0)
 *         IBM6000 - IBM RS/6000 (AIX)
 *         CONVEX  - Convex C-2 (Unix)
 *
 * Bugs fixed in:
 *
 *         scup32.f
 *         cspck32.f
 *         dfpFf.f
 *         dfpF.c
 *         dfsd.c
 *
 * New utility added:
 *
 *         ristosds.c - convert raster images to sds.
 *
 * Also:
 *         All code for the library was modified to conform to the
 *         ANSI C standard.
 *
 * Revision 3.1  1990/07/02  10:11:49  clow
 * some cosmetic modifications
 *
*/

/*
*  hdfcomp.c
*  Re-compress Raster-8 HDF file
*/

#include "hdf.h"

uint8 *space;
char palette[768];
int32 xdim, ydim;
int ispal;

#ifdef PROTOTYPE
int main(int argc, char *argv[]);
#else
int main();
#endif /* PROTOTYPE */

#ifdef PROTOTYPE
main(int argc, char *argv[]) 
#else
main(argc,argv) 
    int argc;
    char *argv[];
#endif /* PROTOTYPE */
    {
    int i, ret;
    char *outfile;
    int image = 1;
    uint16 prevref, writeref = 200, compress = (uint16) 0;

    if (argc < 3) { 
        printf("%s,  version: 1.1   date: July 1, 1992\n", argv[0]);
        printf("  This utility will read in raster-8 images from an\n");
        printf("  HDF file and create a new HDF containing the\n");
        printf("  images in a compressed format.  Images will be\n");
        printf("  appended to outfile, if it exists.\n\n");
        printf("Usage:\n");
        printf(" hdfcomp outfile {[-c],[-r],[-i]} imagefile ...\n");
        printf("                 {[-c],[-r],[-i]} imagefile\n");
        printf("         -r: Store without compression (default)\n");
        printf("         -c: Store using RLE compression\n");
        printf("         -i: Store using IMCOMP compression (requires a");
        printf(" palette in the HDF file)\n");
        exit(1);
    }

    outfile = argv[1];

    for (i = 2; i < argc; i++) {
        if (*argv[i] == '-') {
            switch (argv[i][1]) {
                case 'r':               /* raster */
                    image = 1;
                    compress = (uint16) 0;
                    break;
                case 'c':               /* RLE */
                    image = 1;
                    compress = DFTAG_RLE;
                    break;
                case 'i':               /* IMCOMP */
                    image = 1;
                    compress = DFTAG_IMC;
                    break;
                default:
                    printf("Illegal option: %s, skipping....\n", argv[i]);   
                    break;
            }
        }
        else { /* file name */
            while (DFR8getdims(argv[i], &xdim, &ydim, &ispal) >= 0) {
                prevref = DFR8lastref();
                if ((space = (uint8 *) HDgetspace(xdim * ydim)) == NULL) {
                    printf("Not enough memory to convert image");
                    exit(1);
                }

                if (DFR8getimage(argv[i], space, xdim, ydim, palette) < 0) {
                    printf("Error reading image from file %s\n", argv[i]);
                    exit(1);
                }
                if (ispal) DFR8setpalette(palette);
                else if (compress == DFTAG_IMC) {
                    printf("Couldn't find palette for IMCOMP compression\n");
                    exit(1);
                }
                ret = DFR8writeref(outfile, writeref++);

                if (DFR8addimage(outfile, space, xdim, ydim, compress)<0) {
                    printf("Error writing image to file %s\n", outfile);
                    exit(1);
                }

                /* sequence past this image */
                ret = DFR8readref(argv[i], prevref);
                ret = DFR8getdims(argv[i], &xdim, &ydim, &ispal);

		HDfreespace(space);
            }
        }
    }

    return(0);
}
