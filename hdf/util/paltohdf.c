#ifdef RCSID
static char RcsId[] = "@(#) $Revision$"
#endif
/*
$Header$

$Log$
Revision 1.1  1992/07/01 20:50:03  mlivin
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
 * Revision 3.1  1990/07/02  10:58:31  clow
 * some cosmetic modifications
 *
*/
/*
*  paltohdf.c
*       Version: 1.0   date: August 1, 1989
*       This utility converts a raw palette to hdf format 
*       The incoming palette is assumed to have 768 bytes:
*          256 red values, 256 greens, and 256 blues.
*          The palette in the HDF file will have the RGB values
*          interlaced: RGB RGB ... (This is standard HDF format.)
*
*  by Mike Folk
*  first version of paltohdf:   8/01/89
*
*  This program is in the public domain
*/

#include "hdf.h"

#ifdef PROTOTYPE
int main(int argc, char *argv[]);
int palconv(char *palfile, char *outfile);
#else
int main();
int palconv();
#endif /* PROTOTYPE */

#ifdef PROTOTYPE
main(int argc, char *argv[]) 
#else
main(argc, argv) 
int argc;
char *argv[];
#endif /* PROTOTYPE */
{
	if (argc != 3) { 
	    printf("Usage:\n");
	    printf("   %s rawpalfile hdffile \n\n", argv[0]);
            printf("%s,  version: 1.1   date: July 1, 1992\n\n", argv[0]);
            printf("\tThis utility converts a raw palette to hdf format \n\n");
            printf("\tThe incoming palette is assumed to have 768 bytes:\n");
            printf("\t256 red values, 256 greens, and 256 blues.\n\n");
            printf("\tThe palette in the HDF file will have the RGB values\n");
            printf("\tinterlaced: RGB RGB ... (standard HDF format).\n\n");
		exit(1);
	}

	palconv(argv[1], argv[2]);
}

/*
 *	palconv(palfile, outfile) sets the palette
 */

#ifdef PROTOTYPE
palconv(char *palfile, char *outfile)
#else
palconv(palfile, outfile)
char *palfile, *outfile;
#endif /* PROTOTYPE */
{
    unsigned char palspace[1024], reds[256], greens[256], blues[256], *p;
    FILE *fp;
    int j, ret;

    fp = fopen(palfile, "r");
    if (fp == NULL) {
        printf(" Error opening palette file %s\n", palfile);
        exit(1);
    }
    fread(reds, 1, 256, fp);
    fread(greens, 1, 256, fp);
    fread(blues, 1, 256, fp);
    fclose(fp);

    p = palspace;
    for (j = 0; j < 256; j++) {
        *p++ = reds[j];
        *p++ = greens[j];
        *p++ = blues[j];
    }

    ret = DFPaddpal(outfile, palspace);
    if (ret < 0) {
        printf(" Error: %d, in writing palette %s\n", ret, palfile);
        exit(1);
    }
    return(0);
}

