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
    return(0);
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

