#ifdef RCSID
static char RcsId[] = "@(#)$Revision$";
#endif
/*
$Header$

$Log$
Revision 1.3  1993/01/19 06:00:21  koziol
Merged Hyperslab and JPEG routines with beginning of DEC ALPHA
port.  Lots of minor annoyances fixed.

 * Revision 1.2  1992/07/15  21:48:48  sxu
 *  Added changes for CONVEX
 *
 * Revision 1.1  1992/07/01  20:50:03  mlivin
 * Initial revision
 *
 * Revision 3.1  1991/10/22  17:56:10  dilg
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
 * Revision 3.0  1990/02/02  20:31:12  clow
 * *** empty log message ***
 *
*/
/*
*  hdftopal.c
*       Version: 1.0   date: August 1, 1989
*       This utility converts a palette from an HDF file
*       to a raw palette in a raw palette file.
*       The outgoing palette will have 768 bytes: First
*       256 red values, then 256 greens, then 256 blues.
*
*  by Mike Folk
*  first version of hdftopal:   8/01/89
*
*  This program is in the public domain
*
*/

#include "hdf.h"

#ifdef PROTOTYPE
int main(int argc, char *argv[]);
int rawpalconv(char *hdffile, char *rawpalfile);
#else
int main();
int rawpalconv();
#endif /* PROTOTYPE */

#ifdef PROTOTYPE
main(int argc, char *argv[]) 
#else
main(argc,argv) 
int argc;
char *argv[];
#endif /* PROTOTYPE */
{
    if (argc != 3) { 
        printf("Usage:\n");
        printf("   %s hdffile rawpalfile \n\n", argv[0]);
        printf("%s,  version: 1.1   date: July 1, 1992\n\n",argv[0]);
        printf("   This utility converts a palette from an HDF file \n");
        printf("   to a raw palette in a raw palette file.\n\n");
        printf("   The outgoing palette will have 768 bytes: First \n");
        printf("   256 red values, then 256 greens, then 256 blues.\n\n");
        exit(1);
    }

    rawpalconv(argv[1], argv[2]);
}

/*
 *      rawpalconv(palfile, outfile) sets the palette
 */

#ifdef PROTOTYPE
rawpalconv(char *hdffile, char *rawpalfile)
#else
rawpalconv(hdffile, rawpalfile)
char *hdffile, *rawpalfile;
#endif /* PROTOTYPE */
{
    uint8 palspace[1024], reds[256], greens[256], blues[256], *p;
    FILE *fp;
    int j, ret;

    ret = DFPgetpal(hdffile, (char *) palspace);
    if (ret < 0) {
        printf("Error in reading file %s\n", hdffile);
        exit(1);
    }

    p = palspace;
    for (j = 0; j < 256; j++) {
        reds[j]   = *p++;
        greens[j] = *p++;
        blues[j]  = *p++;
    }

#ifdef PC
    fp = fopen(rawpalfile, "wb");    /* open in binary mode on PC */
#else
    fp = fopen(rawpalfile, "w");
#endif
    if (fp == NULL) {
        printf("Error opening raw palette file %s\n", rawpalfile);
        exit(1);
    }

    fwrite(reds, 1, 256, fp);
    fwrite(greens, 1, 256, fp);
    fwrite(blues, 1, 256, fp);
    fclose(fp);
    return(0);
}

