
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
   *  hdfcomp.c
   *  Re-compress Raster-8 HDF file
 */

#include "hdf.h"

uint8      *space;
uint8       palette[768];
int32       xdim, ydim;
int         ispal;

int         main(int argc, char *argv[]);

int
main(int argc, char *argv[])
{
    int         i, ret;
    char       *outfile;
    int         image = 1;
    intn        jpeg_qual = 75; /* JPEG quality factor */
    uint16      prevref, writeref = 200, compress = (uint16) 0;
    comp_info   cinfo;          /* compression structure */

    if (argc < 3)
      {
          printf("%s,  version: 1.2   date: December 21, 1992\n", argv[0]);
          printf("  This utility will read in raster-8 images from an\n");
          printf("  HDF file and create a new HDF containing the\n");
          printf("  images in a compressed format.  Images will be\n");
          printf("  appended to outfile, if it exists.\n\n");
          printf("Usage:\n");
          printf(" hdfcomp outfile {[-c],[-r],[-i],[-j<quality>]} imagefile ...\n");
          printf("                 {[-c],[-r],[-i], [-j<quality>]} imagefile\n");
          printf("         -r: Store without compression (default)\n");
          printf("         -c: Store using RLE compression\n");
          printf("         -i: Store using IMCOMP compression (requires a");
          printf(" palette in the HDF file)\n");
          printf("         -j<quality>: Store using JPEG compression\n");
          printf("            with a quality factor from 1-100, 75 default\n");
          exit(1);
      }

    outfile = argv[1];

    for (i = 2; i < argc; i++)
      {
          if (*argv[i] == '-')
            {
                switch (argv[i][1])
                  {
                      case 'r': /* raster */
                          image = 1;
                          compress = (uint16) 0;
                          break;
                      case 'c': /* RLE */
                          image = 1;
                          compress = COMP_RLE;
                          break;
                      case 'i': /* IMCOMP */
                          image = 1;
                          compress = COMP_IMCOMP;
                          break;
                      case 'j': /* JPEG */
                          image = 1;
                          compress = COMP_JPEG;
                          if ((jpeg_qual = atoi(&argv[i][2])) <= 0 || jpeg_qual > 100)
                            {
                                printf("Bad JPEG quality setting, should be between\n");
                                printf("1 and 100, using default value of 75\n");
                                jpeg_qual = 75;
                            }   /* end if */
                          cinfo.jpeg.quality = jpeg_qual;   /* set JPEG parameters */
                          cinfo.jpeg.force_baseline = TRUE;
                          break;
                      default:
                          printf("Illegal option: %s, skipping....\n", argv[i]);
                          break;
                  }
            }
          else
            {   /* file name */
                while (DFR8getdims(argv[i], &xdim, &ydim, &ispal) >= 0)
                  {
                      prevref = DFR8lastref();
                      if ((space = (uint8 *) HDgetspace(xdim * ydim)) == NULL)
                        {
                            printf("Not enough memory to convert image");
                            exit(1);
                        }

                      if (DFR8getimage(argv[i], space, xdim, ydim, palette) < 0)
                        {
                            printf("Error reading image from file %s\n", argv[i]);
                            exit(1);
                        }
                      if (ispal)
                          DFR8setpalette((uint8 *) palette);
                      else if (compress == DFTAG_IMC)
                        {
                            printf("Couldn't find palette for IMCOMP compression\n");
                            exit(1);
                        }
                      ret = DFR8writeref(outfile, writeref++);

                      if (compress)
                          DFR8setcompress(compress, &cinfo);
                      if (DFR8addimage(outfile, (VOIDP) space,
                                       xdim, ydim, compress) < 0)
                        {
                            printf("Error writing image to file %s\n", outfile);
                            exit(1);
                        }

                      /* sequence past this image */
                      ret = DFR8readref(argv[i], prevref);
                      ret = DFR8getdims(argv[i], &xdim, &ydim, &ispal);

                      HDfreespace((VOIDP) space);
                  }
            }
      }

    return (0);
}
