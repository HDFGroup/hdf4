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
static char RcsId[] = "$Revision$";
#endif

/* $Id$ */

/***********************************************************
*
* Test program:  mgr
*
* Test the multi-file raster image interface
*
*************************************************************/

#define TESTFILE "tmgr.hdf"
#define DATAFILE "tmgr.dat"

#include "tproto.h"
#include "mfgr.h"

/* Local pre-processor macros */
#define XDIM    0
#define YDIM    1
#define MAX_IMG_NAME    64  /* Maximum length of image names for this test */

/* Local Data to verify image information in datafile */
const struct {
    char *name;
    int32 ncomp;
    int32 nt;
    int32 il;
    int32 dimsizes[2];
    int32 n_attr;
} datafile_info[]=
  { /* This information applies to the tmgr.dat" file */
    {"Raster Image #0", 3, DFNT_UCHAR8, MFGR_INTERLACE_PIXEL, {13,15}, 2},
    {"Raster Image #1", 3, DFNT_UCHAR8, MFGR_INTERLACE_LINE, {13,15}, 2},
    {"Raster Image #2", 3, DFNT_UCHAR8, MFGR_INTERLACE_COMPONENT, {13,15}, 2},
    {"Test Image #1", 4, DFNT_UINT16, MFGR_INTERLACE_PIXEL, {21,23}, 3},
    {"Test Image #2", 2, DFNT_FLOAT64, MFGR_INTERLACE_PIXEL, {17,19}, 3}
  };

const uint8 image0[15][13][3]={
{{0 ,0 ,0 },{1 ,1 ,1 },{2 ,2 ,2 },{3 ,3 ,3 },{4 ,4 ,4 },{5 ,5 ,5 },{6 ,6 ,6 },{7 ,7 ,7 },{8 ,8 ,8 },{9 ,9 ,9 },{10 ,10 ,10 },{11 ,11 ,11 },{12 ,12 ,12 }},
{{1 ,1 ,1 },{2 ,2 ,2 },{3 ,3 ,3 },{4 ,4 ,4 },{5 ,5 ,5 },{6 ,6 ,6 },{7 ,7 ,7 },{8 ,8 ,8 },{9 ,9 ,9 },{10 ,10 ,10 },{11 ,11 ,11 },{12 ,12 ,12 },{13 ,13 ,13 }},
{{2 ,2 ,2 },{3 ,3 ,3 },{4 ,4 ,4 },{5 ,5 ,5 },{6 ,6 ,6 },{7 ,7 ,7 },{8 ,8 ,8 },{9 ,9 ,9 },{10 ,10 ,10 },{11 ,11 ,11 },{12 ,12 ,12 },{13 ,13 ,13 },{14 ,14 ,14 }},
{{3 ,3 ,3 },{4 ,4 ,4 },{5 ,5 ,5 },{6 ,6 ,6 },{7 ,7 ,7 },{8 ,8 ,8 },{9 ,9 ,9 },{10 ,10 ,10 },{11 ,11 ,11 },{12 ,12 ,12 },{13 ,13 ,13 },{14 ,14 ,14 },{15 ,15 ,15 }},
{{4 ,4 ,4 },{5 ,5 ,5 },{6 ,6 ,6 },{7 ,7 ,7 },{8 ,8 ,8 },{9 ,9 ,9 },{10 ,10 ,10 },{11 ,11 ,11 },{12 ,12 ,12 },{13 ,13 ,13 },{14 ,14 ,14 },{15 ,15 ,15 },{16 ,16 ,16 }},
{{5 ,5 ,5 },{6 ,6 ,6 },{7 ,7 ,7 },{8 ,8 ,8 },{9 ,9 ,9 },{10 ,10 ,10 },{11 ,11 ,11 },{12 ,12 ,12 },{13 ,13 ,13 },{14 ,14 ,14 },{15 ,15 ,15 },{16 ,16 ,16 },{17 ,17 ,17 }},
{{6 ,6 ,6 },{7 ,7 ,7 },{8 ,8 ,8 },{9 ,9 ,9 },{10 ,10 ,10 },{11 ,11 ,11 },{12 ,12 ,12 },{13 ,13 ,13 },{14 ,14 ,14 },{15 ,15 ,15 },{16 ,16 ,16 },{17 ,17 ,17 },{18 ,18 ,18 }},
{{7 ,7 ,7 },{8 ,8 ,8 },{9 ,9 ,9 },{10 ,10 ,10 },{11 ,11 ,11 },{12 ,12 ,12 },{13 ,13 ,13 },{14 ,14 ,14 },{15 ,15 ,15 },{16 ,16 ,16 },{17 ,17 ,17 },{18 ,18 ,18 },{19 ,19 ,19 }},
{{8 ,8 ,8 },{9 ,9 ,9 },{10 ,10 ,10 },{11 ,11 ,11 },{12 ,12 ,12 },{13 ,13 ,13 },{14 ,14 ,14 },{15 ,15 ,15 },{16 ,16 ,16 },{17 ,17 ,17 },{18 ,18 ,18 },{19 ,19 ,19 },{20 ,20 ,20 }},
{{9 ,9 ,9 },{10 ,10 ,10 },{11 ,11 ,11 },{12 ,12 ,12 },{13 ,13 ,13 },{14 ,14 ,14 },{15 ,15 ,15 },{16 ,16 ,16 },{17 ,17 ,17 },{18 ,18 ,18 },{19 ,19 ,19 },{20 ,20 ,20 },{21 ,21 ,21 }},
{{10 ,10 ,10 },{11 ,11 ,11 },{12 ,12 ,12 },{13 ,13 ,13 },{14 ,14 ,14 },{15 ,15 ,15 },{16 ,16 ,16 },{17 ,17 ,17 },{18 ,18 ,18 },{19 ,19 ,19 },{20 ,20 ,20 },{21 ,21 ,21 },{22 ,22 ,22 }},
{{11 ,11 ,11 },{12 ,12 ,12 },{13 ,13 ,13 },{14 ,14 ,14 },{15 ,15 ,15 },{16 ,16 ,16 },{17 ,17 ,17 },{18 ,18 ,18 },{19 ,19 ,19 },{20 ,20 ,20 },{21 ,21 ,21 },{22 ,22 ,22 },{23 ,23 ,23 }},
{{12 ,12 ,12 },{13 ,13 ,13 },{14 ,14 ,14 },{15 ,15 ,15 },{16 ,16 ,16 },{17 ,17 ,17 },{18 ,18 ,18 },{19 ,19 ,19 },{20 ,20 ,20 },{21 ,21 ,21 },{22 ,22 ,22 },{23 ,23 ,23 },{24 ,24 ,24 }},
{{13 ,13 ,13 },{14 ,14 ,14 },{15 ,15 ,15 },{16 ,16 ,16 },{17 ,17 ,17 },{18 ,18 ,18 },{19 ,19 ,19 },{20 ,20 ,20 },{21 ,21 ,21 },{22 ,22 ,22 },{23 ,23 ,23 },{24 ,24 ,24 },{25 ,25 ,25 }},
{{14 ,14 ,14 },{15 ,15 ,15 },{16 ,16 ,16 },{17 ,17 ,17 },{18 ,18 ,18 },{19 ,19 ,19 },{20 ,20 ,20 },{21 ,21 ,21 },{22 ,22 ,22 },{23 ,23 ,23 },{24 ,24 ,24 },{25 ,25 ,25 },{26 ,26 ,26 }}
};
const uint8 image1[15][13][3]={
{{0 ,1 ,2 },{3 ,4 ,5 },{6 ,7 ,8 },{9 ,10 ,11 },{12 ,0 ,1 },{2 ,3 ,4 },{5 ,6 ,7 },{8 ,9 ,10 },{11 ,12 ,0 },{1 ,2 ,3 },{4 ,5 ,6 },{7 ,8 ,9 },{10 ,11 ,12 }},
{{1 ,1 ,3 },{3 ,5 ,5 },{7 ,7 ,9 },{9 ,11 ,11 },{13 ,1 ,1 },{3 ,3 ,5 },{5 ,7 ,7 },{9 ,9 ,11 },{11 ,13 ,1 },{1 ,3 ,3 },{5 ,5 ,7 },{7 ,9 ,9 },{11 ,11 ,13 }},
{{2 ,3 ,2 },{3 ,6 ,7 },{6 ,7 ,10 },{11 ,10 ,11 },{14 ,2 ,3 },{2 ,3 ,6 },{7 ,6 ,7 },{10 ,11 ,10 },{11 ,14 ,2 },{3 ,2 ,3 },{6 ,7 ,6 },{7 ,10 ,11 },{10 ,11 ,14 }},
{{3 ,3 ,3 },{3 ,7 ,7 },{7 ,7 ,11 },{11 ,11 ,11 },{15 ,3 ,3 },{3 ,3 ,7 },{7 ,7 ,7 },{11 ,11 ,11 },{11 ,15 ,3 },{3 ,3 ,3 },{7 ,7 ,7 },{7 ,11 ,11 },{11 ,11 ,15 }},
{{4 ,5 ,6 },{7 ,4 ,5 },{6 ,7 ,12 },{13 ,14 ,15 },{12 ,4 ,5 },{6 ,7 ,4 },{5 ,6 ,7 },{12 ,13 ,14 },{15 ,12 ,4 },{5 ,6 ,7 },{4 ,5 ,6 },{7 ,12 ,13 },{14 ,15 ,12 }},
{{5 ,5 ,7 },{7 ,5 ,5 },{7 ,7 ,13 },{13 ,15 ,15 },{13 ,5 ,5 },{7 ,7 ,5 },{5 ,7 ,7 },{13 ,13 ,15 },{15 ,13 ,5 },{5 ,7 ,7 },{5 ,5 ,7 },{7 ,13 ,13 },{15 ,15 ,13 }},
{{6 ,7 ,6 },{7 ,6 ,7 },{6 ,7 ,14 },{15 ,14 ,15 },{14 ,6 ,7 },{6 ,7 ,6 },{7 ,6 ,7 },{14 ,15 ,14 },{15 ,14 ,6 },{7 ,6 ,7 },{6 ,7 ,6 },{7 ,14 ,15 },{14 ,15 ,14 }},
{{7 ,7 ,7 },{7 ,7 ,7 },{7 ,7 ,15 },{15 ,15 ,15 },{15 ,7 ,7 },{7 ,7 ,7 },{7 ,7 ,7 },{15 ,15 ,15 },{15 ,15 ,7 },{7 ,7 ,7 },{7 ,7 ,7 },{7 ,15 ,15 },{15 ,15 ,15 }},
{{8 ,9 ,10 },{11 ,12 ,13 },{14 ,15 ,8 },{9 ,10 ,11 },{12 ,8 ,9 },{10 ,11 ,12 },{13 ,14 ,15 },{8 ,9 ,10 },{11 ,12 ,8 },{9 ,10 ,11 },{12 ,13 ,14 },{15 ,8 ,9 },{10 ,11 ,12 }},
{{9 ,9 ,11 },{11 ,13 ,13 },{15 ,15 ,9 },{9 ,11 ,11 },{13 ,9 ,9 },{11 ,11 ,13 },{13 ,15 ,15 },{9 ,9 ,11 },{11 ,13 ,9 },{9 ,11 ,11 },{13 ,13 ,15 },{15 ,9 ,9 },{11 ,11 ,13 }},
{{10 ,11 ,10 },{11 ,14 ,15 },{14 ,15 ,10 },{11 ,10 ,11 },{14 ,10 ,11 },{10 ,11 ,14 },{15 ,14 ,15 },{10 ,11 ,10 },{11 ,14 ,10 },{11 ,10 ,11 },{14 ,15 ,14 },{15 ,10 ,11 },{10 ,11 ,14 }},
{{11 ,11 ,11 },{11 ,15 ,15 },{15 ,15 ,11 },{11 ,11 ,11 },{15 ,11 ,11 },{11 ,11 ,15 },{15 ,15 ,15 },{11 ,11 ,11 },{11 ,15 ,11 },{11 ,11 ,11 },{15 ,15 ,15 },{15 ,11 ,11 },{11 ,11 ,15 }},
{{12 ,13 ,14 },{15 ,12 ,13 },{14 ,15 ,12 },{13 ,14 ,15 },{12 ,12 ,13 },{14 ,15 ,12 },{13 ,14 ,15 },{12 ,13 ,14 },{15 ,12 ,12 },{13 ,14 ,15 },{12 ,13 ,14 },{15 ,12 ,13 },{14 ,15 ,12 }},
{{13 ,13 ,15 },{15 ,13 ,13 },{15 ,15 ,13 },{13 ,15 ,15 },{13 ,13 ,13 },{15 ,15 ,13 },{13 ,15 ,15 },{13 ,13 ,15 },{15 ,13 ,13 },{13 ,15 ,15 },{13 ,13 ,15 },{15 ,13 ,13 },{15 ,15 ,13 }},
{{14 ,15 ,14 },{15 ,14 ,15 },{14 ,15 ,14 },{15 ,14 ,15 },{14 ,14 ,15 },{14 ,15 ,14 },{15 ,14 ,15 },{14 ,15 ,14 },{15 ,14 ,14 },{15 ,14 ,15 },{14 ,15 ,14 },{15 ,14 ,15 },{14 ,15 ,14 }}
};
const uint8 image2[15][13][3]={
{{0 ,1 ,2 },{3 ,4 ,5 },{6 ,7 ,8 },{9 ,10 ,11 },{12 ,1 ,0 },{3 ,2 ,5 },{4 ,7 ,6 },{9 ,8 ,11 },{10 ,13 ,2 },{3 ,0 ,1 },{6 ,7 ,4 },{5 ,10 ,11 },{8 ,9 ,14 }},
{{3 ,2 ,1 },{0 ,7 ,6 },{5 ,4 ,11 },{10 ,9 ,8 },{15 ,4 ,5 },{6 ,7 ,0 },{1 ,2 ,3 },{12 ,13 ,14 },{15 ,8 ,5 },{4 ,7 ,6 },{1 ,0 ,3 },{2 ,13 ,12 },{15 ,14 ,9 }},
{{6 ,7 ,4 },{5 ,2 ,3 },{0 ,1 ,14 },{15 ,12 ,13 },{10 ,7 ,6 },{5 ,4 ,3 },{2 ,1 ,0 },{15 ,14 ,13 },{12 ,11 ,8 },{9 ,10 ,11 },{12 ,13 ,14 },{15 ,0 ,1 },{2 ,3 ,4 }},
{{9 ,8 ,11 },{10 ,13 ,12 },{15 ,14 ,1 },{0 ,3 ,2 },{5 ,10 ,11 },{8 ,9 ,14 },{15 ,12 ,13 },{2 ,3 ,0 },{1 ,6 ,11 },{10 ,9 ,8 },{15 ,14 ,13 },{12 ,3 ,2 },{1 ,0 ,7 }},
{{12 ,13 ,14 },{15 ,8 ,9 },{10 ,11 ,4 },{5 ,6 ,7 },{0 ,13 ,12 },{15 ,14 ,9 },{8 ,11 ,10 },{5 ,4 ,7 },{6 ,1 ,14 },{15 ,12 ,13 },{10 ,11 ,8 },{9 ,6 ,7 },{4 ,5 ,2 }},
{{0 ,1 ,2 },{3 ,4 ,5 },{6 ,7 ,8 },{9 ,10 ,11 },{12 ,1 ,0 },{3 ,2 ,5 },{4 ,7 ,6 },{9 ,8 ,11 },{10 ,13 ,2 },{3 ,0 ,1 },{6 ,7 ,4 },{5 ,10 ,11 },{8 ,9 ,14 }},
{{3 ,2 ,1 },{0 ,7 ,6 },{5 ,4 ,11 },{10 ,9 ,8 },{15 ,4 ,5 },{6 ,7 ,0 },{1 ,2 ,3 },{12 ,13 ,14 },{15 ,8 ,5 },{4 ,7 ,6 },{1 ,0 ,3 },{2 ,13 ,12 },{15 ,14 ,9 }},
{{6 ,7 ,4 },{5 ,2 ,3 },{0 ,1 ,14 },{15 ,12 ,13 },{10 ,7 ,6 },{5 ,4 ,3 },{2 ,1 ,0 },{15 ,14 ,13 },{12 ,11 ,8 },{9 ,10 ,11 },{12 ,13 ,14 },{15 ,0 ,1 },{2 ,3 ,4 }},
{{9 ,8 ,11 },{10 ,13 ,12 },{15 ,14 ,1 },{0 ,3 ,2 },{5 ,10 ,11 },{8 ,9 ,14 },{15 ,12 ,13 },{2 ,3 ,0 },{1 ,6 ,11 },{10 ,9 ,8 },{15 ,14 ,13 },{12 ,3 ,2 },{1 ,0 ,7 }},
{{12 ,13 ,14 },{15 ,8 ,9 },{10 ,11 ,4 },{5 ,6 ,7 },{0 ,13 ,12 },{15 ,14 ,9 },{8 ,11 ,10 },{5 ,4 ,7 },{6 ,1 ,14 },{15 ,12 ,13 },{10 ,11 ,8 },{9 ,6 ,7 },{4 ,5 ,2 }},
{{0 ,1 ,2 },{3 ,4 ,5 },{6 ,7 ,8 },{9 ,10 ,11 },{12 ,1 ,0 },{3 ,2 ,5 },{4 ,7 ,6 },{9 ,8 ,11 },{10 ,13 ,2 },{3 ,0 ,1 },{6 ,7 ,4 },{5 ,10 ,11 },{8 ,9 ,14 }},
{{3 ,2 ,1 },{0 ,7 ,6 },{5 ,4 ,11 },{10 ,9 ,8 },{15 ,4 ,5 },{6 ,7 ,0 },{1 ,2 ,3 },{12 ,13 ,14 },{15 ,8 ,5 },{4 ,7 ,6 },{1 ,0 ,3 },{2 ,13 ,12 },{15 ,14 ,9 }},
{{6 ,7 ,4 },{5 ,2 ,3 },{0 ,1 ,14 },{15 ,12 ,13 },{10 ,7 ,6 },{5 ,4 ,3 },{2 ,1 ,0 },{15 ,14 ,13 },{12 ,11 ,8 },{9 ,10 ,11 },{12 ,13 ,14 },{15 ,0 ,1 },{2 ,3 ,4 }},
{{9 ,8 ,11 },{10 ,13 ,12 },{15 ,14 ,1 },{0 ,3 ,2 },{5 ,10 ,11 },{8 ,9 ,14 },{15 ,12 ,13 },{2 ,3 ,0 },{1 ,6 ,11 },{10 ,9 ,8 },{15 ,14 ,13 },{12 ,3 ,2 },{1 ,0 ,7 }},
{{12 ,13 ,14 },{15 ,8 ,9 },{10 ,11 ,4 },{5 ,6 ,7 },{0 ,13 ,12 },{15 ,14 ,9 },{8 ,11 ,10 },{5 ,4 ,7 },{6 ,1 ,14 },{15 ,12 ,13 },{10 ,11 ,8 },{9 ,6 ,7 },{4 ,5 ,2 }}
};
const uint16 image3[23][21][4]={
{{0 ,1 ,2 ,3 },{1 ,2 ,3 ,4 },{2 ,3 ,4 ,5 },{3 ,4 ,5 ,6 },{4 ,5 ,6 ,7 },{5 ,6 ,7 ,8 },{6 ,7 ,8 ,9 },{7 ,8 ,9 ,10 },{8 ,9 ,10 ,11 },{9 ,10 ,11 ,12 },{0 ,0 ,0 ,0 },{0 ,0 ,0 ,0 },{0 ,0 ,0 ,0 },{0 ,0 ,0 ,0 },{0 ,0 ,0 ,0 },{0 ,0 ,0 ,0 },{0 ,0 ,0 ,0 },{0 ,0 ,0 ,0 },{0 ,0 ,0 ,0 },{0 ,0 ,0 ,0 },{0 ,0 ,0 ,0 }},
{{0 ,0 ,0 ,0 },{2 ,2 ,2 ,2 },{0 ,0 ,0 ,0 },{2 ,2 ,2 ,2 },{0 ,0 ,0 ,0 },{2 ,2 ,2 ,2 },{0 ,0 ,0 ,0 },{2 ,2 ,2 ,2 },{0 ,0 ,0 ,0 },{2 ,2 ,2 ,2 },{0 ,0 ,0 ,0 },{2 ,2 ,2 ,2 },{0 ,0 ,0 ,0 },{2 ,2 ,2 ,2 },{0 ,0 ,0 ,0 },{2 ,2 ,2 ,2 },{0 ,0 ,0 ,0 },{2 ,2 ,2 ,2 },{0 ,0 ,0 ,0 },{2 ,2 ,2 ,2 },{0 ,0 ,0 ,0 }},
{{10 ,11 ,12 ,13 },{2 ,2 ,2 ,2 },{12 ,13 ,14 ,15 },{2 ,2 ,2 ,2 },{14 ,15 ,16 ,17 },{2 ,2 ,2 ,2 },{16 ,17 ,18 ,19 },{2 ,2 ,2 ,2 },{18 ,19 ,20 ,21 },{2 ,2 ,2 ,2 },{0 ,0 ,0 ,0 },{2 ,2 ,2 ,2 },{0 ,0 ,0 ,0 },{2 ,2 ,2 ,2 },{0 ,0 ,0 ,0 },{2 ,2 ,2 ,2 },{0 ,0 ,0 ,0 },{2 ,2 ,2 ,2 },{0 ,0 ,0 ,0 },{2 ,2 ,2 ,2 },{0 ,0 ,0 ,0 }},
{{0 ,0 ,0 ,0 },{2 ,2 ,2 ,2 },{0 ,0 ,0 ,0 },{2 ,2 ,2 ,2 },{0 ,0 ,0 ,0 },{2 ,2 ,2 ,2 },{0 ,0 ,0 ,0 },{2 ,2 ,2 ,2 },{0 ,0 ,0 ,0 },{2 ,2 ,2 ,2 },{0 ,0 ,0 ,0 },{2 ,2 ,2 ,2 },{0 ,0 ,0 ,0 },{2 ,2 ,2 ,2 },{0 ,0 ,0 ,0 },{2 ,2 ,2 ,2 },{0 ,0 ,0 ,0 },{2 ,2 ,2 ,2 },{0 ,0 ,0 ,0 },{2 ,2 ,2 ,2 },{0 ,0 ,0 ,0 }},
{{20 ,21 ,22 ,23 },{2 ,2 ,2 ,2 },{1 ,2 ,3 ,4 },{2 ,2 ,2 ,2 },{3 ,4 ,5 ,6 },{2 ,2 ,2 ,2 },{5 ,6 ,7 ,8 },{2 ,2 ,2 ,2 },{7 ,8 ,9 ,10 },{2 ,2 ,2 ,2 },{0 ,0 ,0 ,0 },{2 ,2 ,2 ,2 },{0 ,0 ,0 ,0 },{2 ,2 ,2 ,2 },{0 ,0 ,0 ,0 },{2 ,2 ,2 ,2 },{0 ,0 ,0 ,0 },{2 ,2 ,2 ,2 },{0 ,0 ,0 ,0 },{2 ,2 ,2 ,2 },{0 ,0 ,0 ,0 }},
{{0 ,0 ,0 ,0 },{2 ,2 ,2 ,2 },{0 ,0 ,0 ,0 },{2 ,2 ,2 ,2 },{0 ,0 ,0 ,0 },{2 ,2 ,2 ,2 },{0 ,0 ,0 ,0 },{2 ,2 ,2 ,2 },{0 ,0 ,0 ,0 },{2 ,2 ,2 ,2 },{0 ,0 ,0 ,0 },{2 ,2 ,2 ,2 },{0 ,0 ,0 ,0 },{2 ,2 ,2 ,2 },{0 ,0 ,0 ,0 },{2 ,2 ,2 ,2 },{0 ,0 ,0 ,0 },{2 ,2 ,2 ,2 },{0 ,0 ,0 ,0 },{2 ,2 ,2 ,2 },{0 ,0 ,0 ,0 }},
{{9 ,10 ,11 ,12 },{2 ,2 ,2 ,2 },{11 ,12 ,13 ,14 },{2 ,2 ,2 ,2 },{13 ,14 ,15 ,16 },{2 ,2 ,2 ,2 },{15 ,16 ,17 ,18 },{2 ,2 ,2 ,2 },{17 ,18 ,19 ,20 },{2 ,2 ,2 ,2 },{0 ,0 ,0 ,0 },{2 ,2 ,2 ,2 },{0 ,0 ,0 ,0 },{2 ,2 ,2 ,2 },{0 ,0 ,0 ,0 },{2 ,2 ,2 ,2 },{0 ,0 ,0 ,0 },{2 ,2 ,2 ,2 },{0 ,0 ,0 ,0 },{2 ,2 ,2 ,2 },{0 ,0 ,0 ,0 }},
{{0 ,0 ,0 ,0 },{2 ,2 ,2 ,2 },{0 ,0 ,0 ,0 },{2 ,2 ,2 ,2 },{0 ,0 ,0 ,0 },{2 ,2 ,2 ,2 },{0 ,0 ,0 ,0 },{2 ,2 ,2 ,2 },{0 ,0 ,0 ,0 },{2 ,2 ,2 ,2 },{0 ,0 ,0 ,0 },{2 ,2 ,2 ,2 },{0 ,0 ,0 ,0 },{2 ,2 ,2 ,2 },{0 ,0 ,0 ,0 },{2 ,2 ,2 ,2 },{0 ,0 ,0 ,0 },{2 ,2 ,2 ,2 },{0 ,0 ,0 ,0 },{2 ,2 ,2 ,2 },{0 ,0 ,0 ,0 }},
{{19 ,20 ,21 ,22 },{2 ,2 ,2 ,2 },{0 ,1 ,2 ,3 },{2 ,2 ,2 ,2 },{2 ,3 ,4 ,5 },{2 ,2 ,2 ,2 },{4 ,5 ,6 ,7 },{2 ,2 ,2 ,2 },{6 ,7 ,8 ,9 },{2 ,2 ,2 ,2 },{0 ,0 ,0 ,0 },{2 ,2 ,2 ,2 },{0 ,0 ,0 ,0 },{2 ,2 ,2 ,2 },{0 ,0 ,0 ,0 },{2 ,2 ,2 ,2 },{0 ,0 ,0 ,0 },{2 ,2 ,2 ,2 },{0 ,0 ,0 ,0 },{2 ,2 ,2 ,2 },{0 ,0 ,0 ,0 }},
{{0 ,0 ,0 ,0 },{2 ,2 ,2 ,2 },{0 ,0 ,0 ,0 },{2 ,2 ,2 ,2 },{0 ,0 ,0 ,0 },{2 ,2 ,2 ,2 },{0 ,0 ,0 ,0 },{2 ,2 ,2 ,2 },{0 ,0 ,0 ,0 },{2 ,2 ,2 ,2 },{0 ,0 ,0 ,0 },{2 ,2 ,2 ,2 },{0 ,0 ,0 ,0 },{2 ,2 ,2 ,2 },{0 ,0 ,0 ,0 },{2 ,2 ,2 ,2 },{0 ,0 ,0 ,0 },{2 ,2 ,2 ,2 },{0 ,0 ,0 ,0 },{2 ,2 ,2 ,2 },{0 ,0 ,0 ,0 }},
{{8 ,9 ,10 ,11 },{2 ,2 ,2 ,2 },{10 ,11 ,12 ,13 },{2 ,2 ,2 ,2 },{12 ,13 ,14 ,15 },{2 ,2 ,2 ,2 },{14 ,15 ,16 ,17 },{2 ,2 ,2 ,2 },{16 ,17 ,18 ,19 },{2 ,2 ,2 ,2 },{0 ,0 ,0 ,0 },{2 ,2 ,2 ,2 },{0 ,0 ,0 ,0 },{2 ,2 ,2 ,2 },{0 ,0 ,0 ,0 },{2 ,2 ,2 ,2 },{0 ,0 ,0 ,0 },{2 ,2 ,2 ,2 },{0 ,0 ,0 ,0 },{2 ,2 ,2 ,2 },{0 ,0 ,0 ,0 }},
{{0 ,0 ,0 ,0 },{2 ,2 ,2 ,2 },{0 ,0 ,0 ,0 },{2 ,2 ,2 ,2 },{0 ,0 ,0 ,0 },{2 ,2 ,2 ,2 },{0 ,0 ,0 ,0 },{2 ,2 ,2 ,2 },{0 ,0 ,0 ,0 },{2 ,2 ,2 ,2 },{0 ,0 ,0 ,0 },{2 ,2 ,2 ,2 },{0 ,0 ,0 ,0 },{2 ,2 ,2 ,2 },{0 ,0 ,0 ,0 },{2 ,2 ,2 ,2 },{0 ,0 ,0 ,0 },{2 ,2 ,2 ,2 },{0 ,0 ,0 ,0 },{2 ,2 ,2 ,2 },{0 ,0 ,0 ,0 }},
{{18 ,19 ,20 ,21 },{19 ,20 ,21 ,22 },{20 ,21 ,22 ,23 },{0 ,1 ,2 ,3 },{1 ,2 ,3 ,4 },{2 ,3 ,4 ,5 },{3 ,4 ,5 ,6 },{4 ,5 ,6 ,7 },{5 ,6 ,7 ,8 },{6 ,7 ,8 ,9 },{0 ,0 ,0 ,0 },{0 ,0 ,0 ,0 },{0 ,0 ,0 ,0 },{0 ,0 ,0 ,0 },{0 ,0 ,0 ,0 },{0 ,0 ,0 ,0 },{0 ,0 ,0 ,0 },{0 ,0 ,0 ,0 },{0 ,0 ,0 ,0 },{0 ,0 ,0 ,0 },{0 ,0 ,0 ,0 }},
{{0 ,0 ,0 ,0 },{0 ,0 ,0 ,0 },{0 ,0 ,0 ,0 },{0 ,0 ,0 ,0 },{0 ,0 ,0 ,0 },{0 ,0 ,0 ,0 },{0 ,0 ,0 ,0 },{0 ,0 ,0 ,0 },{0 ,0 ,0 ,0 },{0 ,0 ,0 ,0 },{0 ,0 ,0 ,0 },{0 ,0 ,0 ,0 },{0 ,0 ,0 ,0 },{0 ,0 ,0 ,0 },{0 ,0 ,0 ,0 },{0 ,0 ,0 ,0 },{0 ,0 ,0 ,0 },{0 ,0 ,0 ,0 },{0 ,0 ,0 ,0 },{0 ,0 ,0 ,0 },{0 ,0 ,0 ,0 }},
{{7 ,8 ,9 ,10 },{8 ,9 ,10 ,11 },{9 ,10 ,11 ,12 },{10 ,11 ,12 ,13 },{11 ,12 ,13 ,14 },{12 ,13 ,14 ,15 },{13 ,14 ,15 ,16 },{14 ,15 ,16 ,17 },{15 ,16 ,17 ,18 },{16 ,17 ,18 ,19 },{0 ,0 ,0 ,0 },{0 ,0 ,0 ,0 },{0 ,0 ,0 ,0 },{0 ,0 ,0 ,0 },{0 ,0 ,0 ,0 },{0 ,0 ,0 ,0 },{0 ,0 ,0 ,0 },{0 ,0 ,0 ,0 },{0 ,0 ,0 ,0 },{0 ,0 ,0 ,0 },{0 ,0 ,0 ,0 }},
{{0 ,0 ,0 ,0 },{0 ,0 ,0 ,0 },{0 ,0 ,0 ,0 },{0 ,0 ,0 ,0 },{0 ,0 ,0 ,0 },{0 ,0 ,0 ,0 },{0 ,0 ,0 ,0 },{0 ,0 ,0 ,0 },{0 ,0 ,0 ,0 },{0 ,0 ,0 ,0 },{0 ,0 ,0 ,0 },{0 ,0 ,0 ,0 },{0 ,0 ,0 ,0 },{0 ,0 ,0 ,0 },{0 ,0 ,0 ,0 },{0 ,0 ,0 ,0 },{0 ,0 ,0 ,0 },{0 ,0 ,0 ,0 },{0 ,0 ,0 ,0 },{0 ,0 ,0 ,0 },{0 ,0 ,0 ,0 }},
{{17 ,18 ,19 ,20 },{18 ,19 ,20 ,21 },{19 ,20 ,21 ,22 },{20 ,21 ,22 ,23 },{0 ,1 ,2 ,3 },{1 ,2 ,3 ,4 },{2 ,3 ,4 ,5 },{3 ,4 ,5 ,6 },{4 ,5 ,6 ,7 },{5 ,6 ,7 ,8 },{0 ,0 ,0 ,0 },{0 ,0 ,0 ,0 },{0 ,0 ,0 ,0 },{0 ,0 ,0 ,0 },{0 ,0 ,0 ,0 },{0 ,0 ,0 ,0 },{0 ,0 ,0 ,0 },{0 ,0 ,0 ,0 },{0 ,0 ,0 ,0 },{0 ,0 ,0 ,0 },{0 ,0 ,0 ,0 }},
{{0 ,0 ,0 ,0 },{0 ,0 ,0 ,0 },{0 ,0 ,0 ,0 },{0 ,0 ,0 ,0 },{0 ,0 ,0 ,0 },{0 ,0 ,0 ,0 },{0 ,0 ,0 ,0 },{0 ,0 ,0 ,0 },{0 ,0 ,0 ,0 },{0 ,0 ,0 ,0 },{0 ,0 ,0 ,0 },{0 ,0 ,0 ,0 },{0 ,0 ,0 ,0 },{0 ,0 ,0 ,0 },{0 ,0 ,0 ,0 },{0 ,0 ,0 ,0 },{0 ,0 ,0 ,0 },{0 ,0 ,0 ,0 },{0 ,0 ,0 ,0 },{0 ,0 ,0 ,0 },{0 ,0 ,0 ,0 }},
{{6 ,7 ,8 ,9 },{7 ,8 ,9 ,10 },{8 ,9 ,10 ,11 },{9 ,10 ,11 ,12 },{10 ,11 ,12 ,13 },{11 ,12 ,13 ,14 },{12 ,13 ,14 ,15 },{13 ,14 ,15 ,16 },{14 ,15 ,16 ,17 },{15 ,16 ,17 ,18 },{0 ,0 ,0 ,0 },{0 ,0 ,0 ,0 },{0 ,0 ,0 ,0 },{0 ,0 ,0 ,0 },{0 ,0 ,0 ,0 },{0 ,0 ,0 ,0 },{0 ,0 ,0 ,0 },{0 ,0 ,0 ,0 },{0 ,0 ,0 ,0 },{0 ,0 ,0 ,0 },{0 ,0 ,0 ,0 }},
{{0 ,0 ,0 ,0 },{0 ,0 ,0 ,0 },{0 ,0 ,0 ,0 },{0 ,0 ,0 ,0 },{0 ,0 ,0 ,0 },{0 ,0 ,0 ,0 },{0 ,0 ,0 ,0 },{0 ,0 ,0 ,0 },{0 ,0 ,0 ,0 },{0 ,0 ,0 ,0 },{0 ,0 ,0 ,0 },{0 ,0 ,0 ,0 },{0 ,0 ,0 ,0 },{0 ,0 ,0 ,0 },{0 ,0 ,0 ,0 },{0 ,0 ,0 ,0 },{0 ,0 ,0 ,0 },{0 ,0 ,0 ,0 },{0 ,0 ,0 ,0 },{0 ,0 ,0 ,0 },{0 ,0 ,0 ,0 }},
{{16 ,17 ,18 ,19 },{17 ,18 ,19 ,20 },{18 ,19 ,20 ,21 },{19 ,20 ,21 ,22 },{20 ,21 ,22 ,23 },{0 ,1 ,2 ,3 },{1 ,2 ,3 ,4 },{2 ,3 ,4 ,5 },{3 ,4 ,5 ,6 },{4 ,5 ,6 ,7 },{0 ,0 ,0 ,0 },{0 ,0 ,0 ,0 },{0 ,0 ,0 ,0 },{0 ,0 ,0 ,0 },{0 ,0 ,0 ,0 },{0 ,0 ,0 ,0 },{0 ,0 ,0 ,0 },{0 ,0 ,0 ,0 },{0 ,0 ,0 ,0 },{0 ,0 ,0 ,0 },{0 ,0 ,0 ,0 }},
{{0 ,0 ,0 ,0 },{0 ,0 ,0 ,0 },{0 ,0 ,0 ,0 },{0 ,0 ,0 ,0 },{0 ,0 ,0 ,0 },{0 ,0 ,0 ,0 },{0 ,0 ,0 ,0 },{0 ,0 ,0 ,0 },{0 ,0 ,0 ,0 },{0 ,0 ,0 ,0 },{0 ,0 ,0 ,0 },{0 ,0 ,0 ,0 },{0 ,0 ,0 ,0 },{0 ,0 ,0 ,0 },{0 ,0 ,0 ,0 },{0 ,0 ,0 ,0 },{0 ,0 ,0 ,0 },{0 ,0 ,0 ,0 },{0 ,0 ,0 ,0 },{0 ,0 ,0 ,0 },{0 ,0 ,0 ,0 }},
{{0 ,0 ,0 ,0 },{0 ,0 ,0 ,0 },{0 ,0 ,0 ,0 },{0 ,0 ,0 ,0 },{0 ,0 ,0 ,0 },{0 ,0 ,0 ,0 },{0 ,0 ,0 ,0 },{0 ,0 ,0 ,0 },{0 ,0 ,0 ,0 },{0 ,0 ,0 ,0 },{0 ,0 ,0 ,0 },{0 ,0 ,0 ,0 },{0 ,0 ,0 ,0 },{0 ,0 ,0 ,0 },{0 ,0 ,0 ,0 },{0 ,0 ,0 ,0 },{0 ,0 ,0 ,0 },{0 ,0 ,0 ,0 },{0 ,0 ,0 ,0 },{0 ,0 ,0 ,0 },{0 ,0 ,0 ,0 }}
};
const float64 image4[19][17][2]={
{{6.0,6.0},{6.0,6.0},{6.0,6.0},{6.0,6.0},{6.0,6.0},{6.0,6.0},{6.0,6.0},{6.0,6.0},{0.0,0.0},{0.0,0.0},{0.0,0.0},{0.0,0.0},{0.0,0.0},{0.0,0.0},{0.0,0.0},{0.0,0.0},{0.0,0.0}},
{{6.0,6.0},{6.0,6.0},{6.0,6.0},{6.0,6.0},{6.0,6.0},{6.0,6.0},{6.0,6.0},{6.0,6.0},{0.0,0.0},{0.0,0.0},{0.0,0.0},{0.0,0.0},{0.0,0.0},{0.0,0.0},{0.0,0.0},{0.0,0.0},{0.0,0.0}},
{{6.0,6.0},{6.0,6.0},{6.0,6.0},{6.0,6.0},{6.0,6.0},{6.0,6.0},{6.0,6.0},{6.0,6.0},{0.0,0.0},{0.0,0.0},{0.0,0.0},{0.0,0.0},{0.0,0.0},{0.0,0.0},{0.0,0.0},{0.0,0.0},{0.0,0.0}},
{{6.0,6.0},{6.0,6.0},{6.0,6.0},{6.0,6.0},{6.0,6.0},{6.0,6.0},{6.0,6.0},{6.0,6.0},{0.0,0.0},{0.0,0.0},{0.0,0.0},{0.0,0.0},{0.0,0.0},{0.0,0.0},{0.0,0.0},{0.0,0.0},{0.0,0.0}},
{{6.0,6.0},{6.0,6.0},{6.0,6.0},{6.0,6.0},{6.0,6.0},{6.0,6.0},{6.0,6.0},{6.0,6.0},{0.0,0.0},{0.0,0.0},{0.0,0.0},{0.0,0.0},{0.0,0.0},{0.0,0.0},{0.0,0.0},{0.0,0.0},{0.0,0.0}},
{{6.0,6.0},{6.0,6.0},{6.0,6.0},{6.0,6.0},{6.0,6.0},{6.0,6.0},{6.0,6.0},{6.0,6.0},{0.0,0.0},{0.0,0.0},{0.0,0.0},{0.0,0.0},{0.0,0.0},{0.0,0.0},{0.0,0.0},{0.0,0.0},{0.0,0.0}},
{{6.0,6.0},{6.0,6.0},{6.0,6.0},{6.0,6.0},{6.0,6.0},{6.0,6.0},{6.0,6.0},{6.0,6.0},{0.0,0.0},{0.0,0.0},{0.0,0.0},{0.0,0.0},{0.0,0.0},{0.0,0.0},{0.0,0.0},{0.0,0.0},{0.0,0.0}},
{{6.0,6.0},{6.0,6.0},{6.0,6.0},{6.0,6.0},{6.0,6.0},{6.0,6.0},{6.0,6.0},{6.0,6.0},{0.0,0.0},{0.0,0.0},{0.0,0.0},{0.0,0.0},{0.0,0.0},{0.0,0.0},{0.0,0.0},{0.0,0.0},{0.0,0.0}},
{{6.0,6.0},{6.0,6.0},{6.0,6.0},{6.0,6.0},{6.0,6.0},{6.0,6.0},{6.0,6.0},{6.0,6.0},{0.0,0.0},{0.0,0.0},{0.0,0.0},{0.0,0.0},{0.0,0.0},{0.0,0.0},{0.0,0.0},{0.0,0.0},{0.0,0.0}},
{{0.0,0.0},{0.0,0.0},{0.0,0.0},{0.0,0.0},{0.0,0.0},{0.0,0.0},{0.0,0.0},{0.0,0.0},{9.0,9.0},{9.0,9.0},{9.0,9.0},{9.0,9.0},{9.0,9.0},{9.0,9.0},{9.0,9.0},{9.0,9.0},{0.0,0.0}},
{{0.0,0.0},{0.0,0.0},{0.0,0.0},{0.0,0.0},{0.0,0.0},{0.0,0.0},{0.0,0.0},{0.0,0.0},{9.0,9.0},{9.0,9.0},{9.0,9.0},{9.0,9.0},{9.0,9.0},{9.0,9.0},{9.0,9.0},{9.0,9.0},{0.0,0.0}},
{{0.0,0.0},{0.0,0.0},{0.0,0.0},{0.0,0.0},{0.0,0.0},{0.0,0.0},{0.0,0.0},{0.0,0.0},{9.0,9.0},{9.0,9.0},{9.0,9.0},{9.0,9.0},{9.0,9.0},{9.0,9.0},{9.0,9.0},{9.0,9.0},{0.0,0.0}},
{{0.0,0.0},{0.0,0.0},{0.0,0.0},{0.0,0.0},{0.0,0.0},{0.0,0.0},{0.0,0.0},{0.0,0.0},{9.0,9.0},{9.0,9.0},{9.0,9.0},{9.0,9.0},{9.0,9.0},{9.0,9.0},{9.0,9.0},{9.0,9.0},{0.0,0.0}},
{{0.0,0.0},{0.0,0.0},{0.0,0.0},{0.0,0.0},{0.0,0.0},{0.0,0.0},{0.0,0.0},{0.0,0.0},{9.0,9.0},{9.0,9.0},{9.0,9.0},{9.0,9.0},{9.0,9.0},{9.0,9.0},{9.0,9.0},{9.0,9.0},{0.0,0.0}},
{{0.0,0.0},{0.0,0.0},{0.0,0.0},{0.0,0.0},{0.0,0.0},{0.0,0.0},{0.0,0.0},{0.0,0.0},{9.0,9.0},{9.0,9.0},{9.0,9.0},{9.0,9.0},{9.0,9.0},{9.0,9.0},{9.0,9.0},{9.0,9.0},{0.0,0.0}},
{{0.0,0.0},{0.0,0.0},{0.0,0.0},{0.0,0.0},{0.0,0.0},{0.0,0.0},{0.0,0.0},{0.0,0.0},{9.0,9.0},{9.0,9.0},{9.0,9.0},{9.0,9.0},{9.0,9.0},{9.0,9.0},{9.0,9.0},{9.0,9.0},{0.0,0.0}},
{{0.0,0.0},{0.0,0.0},{0.0,0.0},{0.0,0.0},{0.0,0.0},{0.0,0.0},{0.0,0.0},{0.0,0.0},{9.0,9.0},{9.0,9.0},{9.0,9.0},{9.0,9.0},{9.0,9.0},{9.0,9.0},{9.0,9.0},{9.0,9.0},{0.0,0.0}},
{{0.0,0.0},{0.0,0.0},{0.0,0.0},{0.0,0.0},{0.0,0.0},{0.0,0.0},{0.0,0.0},{0.0,0.0},{9.0,9.0},{9.0,9.0},{9.0,9.0},{9.0,9.0},{9.0,9.0},{9.0,9.0},{9.0,9.0},{9.0,9.0},{0.0,0.0}},
{{0.0,0.0},{0.0,0.0},{0.0,0.0},{0.0,0.0},{0.0,0.0},{0.0,0.0},{0.0,0.0},{0.0,0.0},{0.0,0.0},{0.0,0.0},{0.0,0.0},{0.0,0.0},{0.0,0.0},{0.0,0.0},{0.0,0.0},{0.0,0.0},{0.0,0.0}}
};

void dump_image(void *data, int32 xdim, int32 ydim, int32 ncomp, int32 nt)
{
    int32 nt_size=DFKNTsize(nt);
    int32 i,j,k;

    for(i=0; i<ydim; i++)
      {
#ifdef QAK
          printf("%ld:",(long)i);
#endif /* QAK */
          for(j=0; j<xdim; j++)
            {
                printf("{");
                for(k=0; k<ncomp; k++)
                  {
                    switch(nt)
                      {
                          case DFNT_CHAR8:
                          case DFNT_UCHAR8:
#ifdef QAK
                            {
                                char *ptr=(char *)data;
                                printf("%c ",*ptr);
                            }
                            break;
#endif /* QAK */

                          case DFNT_UINT8:
                            {
                                unsigned char *ptr=(unsigned char *)data;
                                printf("%u ",(unsigned)*ptr);
                            }
                            break;

                          case DFNT_INT8:
                            {
                                char *ptr=(char *)data;
                                printf("%d ",(int)*ptr);
                            }
                            break;

                          case DFNT_UINT16:
                            {
                                uint16 *ptr=(uint16 *)data;
                                printf("%u ",(unsigned)*ptr);
                            }
                            break;

                          case DFNT_INT16:
                            {
                                int16 *ptr=(int16 *)data;
                                printf("%d ",(int)*ptr);
                            }
                            break;

                          case DFNT_UINT32:
                            {
                                uint32 *ptr=(uint32 *)data;
                                printf("%lu ",(unsigned long)*ptr);
                            }
                            break;

                          case DFNT_INT32:
                            {
                                int32 *ptr=(int32 *)data;
                                printf("%ld ",(long)*ptr);
                            }
                            break;

                          case DFNT_FLOAT32:
                            {
                                float32 *ptr=(float32 *)data;
                                printf("%f ",(double)*ptr);
                            }
                            break;

                          case DFNT_FLOAT64:
                            {
                                float64 *ptr=(float64 *)data;
                                printf("%f ",(double)*ptr);
                            }
                            break;

                          default:
                            printf("unknown NT: %ld\n",(long)nt);
                            break;

                      } /* end switch */
            if(k<(ncomp-1))
                printf(",");
                    data=(void *)((char *)data+nt_size);
                  } /* end for */
                printf("},");

            } /* end for */
          printf("\n");
      } /* end for */
}   /* dump_image() */

#ifdef QAK
/****************************************************************
**
**  test_mgr(): Main multi-file raster image test routine
** 
****************************************************************/
void
test_mgr_old()
{
    intn i,j,k;             /* local counting variable */
    int32 hdf_file_id;      /* HDF file ID */
    int32 grid;             /* GR interface ID */
    int32 riid;             /* RI interface ID */
    int32 ret;              /* generic return value */
    int32 n_datasets;       /* number of datasets */
    int32 n_attrs;          /* number of attributes */
    int32 ncomp;            /* number of components */
    int32 nt;               /* number-type */
    int32 il;               /* interlace */
    int32 dimsizes[2];      /* dimension sizes */
    char attr_name[32];     /* name of the attribute */
    char u8_attr[64];       /* uint8 attribute */
    uint16 *img1_data;      /* uint16 image pointer */
    int32 start[2];         /* starting location for I/O */
    int32 stride[2];        /* stride of I/O */
    int32 count[2];         /* count of I/O */

    /* Create the test file */
#ifdef QAK
    hdf_file_id=Hopen(TESTFILE,DFACC_ALL,0);
#else /* QAK */
    hdf_file_id=Hopen(TESTFILE,DFACC_RDWR,0);
#endif /* QAK */
    CHECK(hdf_file_id,FAIL,"Hopen");

    /* Try initializing the GR interface */
    grid=GRstart(hdf_file_id);
    CHECK(grid,FAIL,"GRstart");

    ret=(intn)GRfileinfo(grid,&n_datasets,&n_attrs);
    CHECK(ret,FAIL,"GRfileinfo");

printf("hdf_file_id=%ld\n",(long)hdf_file_id);
printf("grid=%ld: n_datasets=%ld, n_attrs=%ld\n",(long)grid,(long)n_datasets,(long)n_attrs);
    if(n_attrs>0)
      {
          for(i=0; i<n_attrs; i++)
            {
                ret=(intn)GRattrinfo(grid,i,attr_name,&nt,&ncomp);
                CHECK(ret,FAIL,"GRattrinfo");
if(ret==FAIL)
    HEprint(stderr,0);

                ret=(intn)GRgetattr(grid,i,u8_attr);
                CHECK(ret,FAIL,"GRgetattr");
                if(ret!=FAIL)
                    printf("Global Attribute #%d: Name=%s, Value=%s\n",i,attr_name,u8_attr);

            } /* end for */
      } /* end if */

    /* Shut down the GR interface */
    ret=GRend(grid);
    CHECK(ret,FAIL,"GRend");
if(ret==FAIL)
    HEprint(stderr,0);

    /* Close the file */
    ret=Hclose(hdf_file_id);
    CHECK(ret,FAIL,"Hclose");
if(ret==FAIL)
    HEprint(stderr,0);


    /* Ok, let's try playing with the images some */
    hdf_file_id=Hopen(TESTFILE,DFACC_RDWR,0);
    CHECK(hdf_file_id,FAIL,"Hopen");

    /* Try initializing the GR interface */
    grid=GRstart(hdf_file_id);
    CHECK(grid,FAIL,"GRstart");

    ret=(intn)GRfileinfo(grid,&n_datasets,&n_attrs);
    CHECK(ret,FAIL,"GRfileinfo");

    /* Attach to each image */
    for(i=0; i<n_datasets; i++)
      {
          riid=GRselect(grid,i);
          CHECK(riid,FAIL,"GRselect");
          if(riid==FAIL)
              HEprint(stderr,0);
          else
            {
                ret=(intn)GRgetiminfo(riid,NULL,&ncomp,&nt,&il,dimsizes,&n_attrs);
                CHECK(ret,FAIL,"GRfileinfo");

printf("%d: riid=%ld: ncomp=%ld, nt=%ld, il=%ld, dim[0]=%ld, dim[1]=%ld, n_attrs=%ld\n",i,(long)riid,(long)ncomp,(long)nt,(long)il,(long)dimsizes[0],(long)dimsizes[1],(long)n_attrs);

              /* Detach from the first image */
              ret=GRendaccess(riid);
              CHECK(ret,FAIL,"GRselect");
            } /* end else */
      } /* end for */

    /* Shut down the GR interface */
    ret=GRend(grid);
    CHECK(ret,FAIL,"GRend");
if(ret==FAIL)
    HEprint(stderr,0);

    /* Close the file */
    ret=Hclose(hdf_file_id);
    CHECK(ret,FAIL,"Hclose");
if(ret==FAIL)
    HEprint(stderr,0);


    /* Ok, now create some attributes in the images */
    hdf_file_id=Hopen(TESTFILE,DFACC_RDWR,0);
    CHECK(hdf_file_id,FAIL,"Hopen");

    /* Try initializing the GR interface */
    grid=GRstart(hdf_file_id);
    CHECK(grid,FAIL,"GRstart");

    /* Create an attribute for the file */
    HDstrcpy(attr_name,"Test1");
    HDstrcpy(u8_attr,"Attribute value 1");
    ret=GRsetattr(grid,attr_name,DFNT_UINT8,HDstrlen(u8_attr)+1,u8_attr);
    CHECK(ret,FAIL,"GRsetattr");

    /* Create another attribute for the file */
    HDstrcpy(attr_name,"Test2");
    HDstrcpy(u8_attr,"Attribute value 2");
    ret=GRsetattr(grid,attr_name,DFNT_UINT8,HDstrlen(u8_attr)+1,u8_attr);
    CHECK(ret,FAIL,"GRsetattr");

    ret=(intn)GRfileinfo(grid,&n_datasets,&n_attrs);
    CHECK(ret,FAIL,"GRfileinfo");

    /* Attach to each image */
    for(i=0; i<n_datasets; i++)
      {
          riid=GRselect(grid,i);
          CHECK(riid,FAIL,"GRselect");
          if(riid==FAIL)
              HEprint(stderr,0);
          else
            {
                /* Create an attribute for the image */
                HDstrcpy(attr_name,"Image1");
                HDstrcpy(u8_attr,"Attribute value 1");
                ret=GRsetattr(riid,attr_name,DFNT_UINT8,HDstrlen(u8_attr)+1,u8_attr);
                CHECK(ret,FAIL,"GRsetattr");

                /* Create another attribute for the image */
                HDstrcpy(attr_name,"Image2");
                HDstrcpy(u8_attr,"Attribute value 2");
                ret=GRsetattr(riid,attr_name,DFNT_UINT8,HDstrlen(u8_attr)+1,u8_attr);
                CHECK(ret,FAIL,"GRsetattr");

                ret=(intn)GRgetiminfo(riid,NULL,&ncomp,&nt,&il,dimsizes,&n_attrs);
                CHECK(ret,FAIL,"GRfileinfo");

printf("%d: riid=%ld: ncomp=%ld, nt=%ld, il=%ld, dim[0]=%ld, dim[1]=%ld, n_attrs=%ld\n",i,(long)riid,(long)ncomp,(long)nt,(long)il,(long)dimsizes[0],(long)dimsizes[1],(long)n_attrs);

                for(j=0; j<n_attrs; j++)
                  {
                    ret=(intn)GRattrinfo(riid,j,attr_name,&nt,&ncomp);
                    CHECK(ret,FAIL,"GRattrinfo");
if(ret==FAIL)
    HEprint(stderr,0);

                    ret=(intn)GRgetattr(riid,j,u8_attr);
                    CHECK(ret,FAIL,"GRgetattr");
                    if(ret!=FAIL)
                        printf("Image #%d Attribute #%d: Name=%s, Value=%s\n",i,j,attr_name,u8_attr);
                  } /* end for */

                /* Detach from the image */
                ret=GRendaccess(riid);
                CHECK(ret,FAIL,"GRendaccess");
            } /* end else */
      } /* end for */

    /* Shut down the GR interface */
    ret=GRend(grid);
    CHECK(ret,FAIL,"GRend");
if(ret==FAIL)
    HEprint(stderr,0);

    /* Close the file */
    ret=Hclose(hdf_file_id);
    CHECK(ret,FAIL,"Hclose");
if(ret==FAIL)
    HEprint(stderr,0);

    /* Ok, now create some new images in the file */
    hdf_file_id=Hopen(TESTFILE,DFACC_RDWR,0);
    CHECK(hdf_file_id,FAIL,"Hopen");

    /* Try initializing the GR interface */
    grid=GRstart(hdf_file_id);
    CHECK(grid,FAIL,"GRstart");

    dimsizes[0]=21;
    dimsizes[1]=23;
    ncomp=4;
    nt=DFNT_UINT16;
    riid=(intn)GRcreate(grid,"Test Image #1",ncomp,nt,0,dimsizes);
    CHECK(riid,FAIL,"GRcreate");

    img1_data=HDmalloc(dimsizes[0]*dimsizes[1]*DFKNTsize(nt)*ncomp);
    CHECK(img1_data,NULL,"HDmalloc");

    {
        uint16 *tptr=(uint16 *)img1_data;

        for(i=0; i<dimsizes[1]; i++)
            for(j=0; j<dimsizes[0]; j++)
                for(k=0; k<ncomp; k++)
                    *tptr++=j+k;
    }
            
    start[0]=start[1]=0;
    stride[0]=1;
    stride[1]=2;
    count[0]=dimsizes[0]/2;
    count[1]=dimsizes[1]/2;
    ret=GRwriteimage(riid,start,stride,count,img1_data);
    CHECK(ret,FAIL,"GRwriteimage");
if(ret==FAIL)
    HEprint(stderr,0);
    
    {
        uint16 *tptr=(uint16 *)img1_data;

        for(i=0; i<dimsizes[1]; i++)
            for(j=0; j<dimsizes[0]; j++)
                for(k=0; k<ncomp; k++)
                    *tptr++=2;
    }

    start[0]=start[1]=1;
    stride[0]=2;
    stride[1]=1;
    count[0]=dimsizes[0]/2;
    count[1]=dimsizes[1]/2;
    ret=GRwriteimage(riid,start,stride,count,img1_data);
    CHECK(ret,FAIL,"GRwriteimage");
if(ret==FAIL)
    HEprint(stderr,0);
    
    /* Get rid of the image data */
    HDfree(img1_data);

    /* Detach from the image */
    ret=GRendaccess(riid);
    CHECK(ret,FAIL,"GRendaccess");
if(ret==FAIL)
    HEprint(stderr,0);

    dimsizes[0]=17;
    dimsizes[1]=19;
    ncomp=2;
    nt=DFNT_FLOAT64;
    riid=(intn)GRcreate(grid,"Test Image #2",ncomp,nt,0,dimsizes);
    CHECK(riid,FAIL,"GRcreate");

    img1_data=HDmalloc(dimsizes[0]*dimsizes[1]*DFKNTsize(nt)*ncomp);
    CHECK(img1_data,NULL,"HDmalloc");

    {
        float64 *tptr=(float64 *)img1_data;

        for(i=0; i<dimsizes[1]; i++)
            for(j=0; j<dimsizes[0]; j++)
                for(k=0; k<ncomp; k++)
                    *tptr++=6.0;
    }
            
    start[0]=start[1]=0;
    stride[0]=stride[1]=1;
    count[0]=dimsizes[0]/2;
    count[1]=dimsizes[1]/2;
    ret=GRwriteimage(riid,start,stride,count,img1_data);
    CHECK(ret,FAIL,"GRwriteimage");
if(ret==FAIL)
    HEprint(stderr,0);
    
    {
        float64 *tptr=(float64 *)img1_data;

        for(i=0; i<dimsizes[1]; i++)
            for(j=0; j<dimsizes[0]; j++)
                for(k=0; k<ncomp; k++)
                    *tptr++=9.0;
    }
            
    start[0]=dimsizes[0]/2;
    start[1]=dimsizes[1]/2;
    stride[0]=stride[1]=1;
    count[0]=dimsizes[0]/2;
    count[1]=dimsizes[1]/2;
    ret=GRwriteimage(riid,start,stride,count,img1_data);
    CHECK(ret,FAIL,"GRwriteimage");
if(ret==FAIL)
    HEprint(stderr,0);
    
    /* Get rid of the image data */
    HDfree(img1_data);

    /* Detach from the image */
    ret=GRendaccess(riid);
    CHECK(ret,FAIL,"GRendaccess");
if(ret==FAIL)
    HEprint(stderr,0);

    ret=(intn)GRfileinfo(grid,&n_datasets,&n_attrs);
    CHECK(ret,FAIL,"GRfileinfo");

    /* Attach to each image */
    for(i=0; i<n_datasets; i++)
      {
          riid=GRselect(grid,i);
          CHECK(riid,FAIL,"GRselect");
          if(riid==FAIL)
              HEprint(stderr,0);
          else
            {
                /* Create an attribute for the image */
                HDstrcpy(attr_name,"Image1");
                HDstrcpy(u8_attr,"Attribute value 1");
                ret=GRsetattr(riid,attr_name,DFNT_UINT8,HDstrlen(u8_attr)+1,u8_attr);
                CHECK(ret,FAIL,"GRsetattr");

                /* Create another attribute for the image */
                HDstrcpy(attr_name,"Image2");
                HDstrcpy(u8_attr,"Attribute value 2");
                ret=GRsetattr(riid,attr_name,DFNT_UINT8,HDstrlen(u8_attr)+1,u8_attr);
                CHECK(ret,FAIL,"GRsetattr");

                ret=(intn)GRgetiminfo(riid,NULL,&ncomp,&nt,&il,dimsizes,&n_attrs);
                CHECK(ret,FAIL,"GRfileinfo");

printf("%d: riid=%ld: ncomp=%ld, nt=%ld, il=%ld, dim[0]=%ld, dim[1]=%ld, n_attrs=%ld\n",i,(long)riid,(long)ncomp,(long)nt,(long)il,(long)dimsizes[0],(long)dimsizes[1],(long)n_attrs);

                img1_data=HDmalloc(dimsizes[0]*dimsizes[1]*ncomp*DFKNTsize(nt));
                CHECK(img1_data,NULL,"HDmalloc");

                HDmemset(img1_data,0,dimsizes[0]*dimsizes[1]*ncomp*DFKNTsize(nt));

                start[0]=start[1]=0;
                stride[0]=stride[1]=1;
                ret=GRreadimage(riid,start,stride,dimsizes,img1_data);

                /* Dump the image out */
                dump_image(img1_data,dimsizes[0],dimsizes[1],ncomp,nt);

                HDfree(img1_data);

                for(j=0; j<n_attrs; j++)
                  {
                    ret=(intn)GRattrinfo(riid,j,attr_name,&nt,&ncomp);
                    CHECK(ret,FAIL,"GRattrinfo");
if(ret==FAIL)
    HEprint(stderr,0);

                    ret=(intn)GRgetattr(riid,j,u8_attr);
                    CHECK(ret,FAIL,"GRgetattr");
                    if(ret!=FAIL)
                        printf("Image #%d Attribute #%d: Name=%s, Value=%s\n",i,j,attr_name,u8_attr);
                  } /* end for */

                /* Detach from the image */
                ret=GRendaccess(riid);
                CHECK(ret,FAIL,"GRendaccess");
            } /* end else */
      } /* end for */

    /* Shut down the GR interface */
    ret=GRend(grid);
    CHECK(ret,FAIL,"GRend");
if(ret==FAIL)
    HEprint(stderr,0);

    /* Close the file */
    ret=Hclose(hdf_file_id);
    CHECK(ret,FAIL,"Hclose");
if(ret==FAIL)
    HEprint(stderr,0);
} /* test_mgr_old() */
#endif /* QAK */

/* Test outline:
    I. Interface Initialization
        A. GRstart
        B. GRend
        C. GRfileinfo
    II. Create Images
        A. GRcreate/GRselect/GRendaccess w/GRgetiminfo
        B. Write/Read images
            1. With no Data
                a. Default fill value
                b. user defined fill value
            2. With real Data
                a. New Image
                    1. With default fill value
                        aa. Whole image
                        bb. Sub-setted image
                        cc. Sub-sampled image
                    2. With user defined vill value
                        aa. Whole image
                        bb. Sub-setted image
                        cc. Sub-sampled image
                b. Existing Image
                    1. Whole image
                    2. Sub-setted image
                    3. Sub-sampled image
    III. ID/Ref/Index Functions
        A. GRidtoref
        B. GRreftoindex
    IV. Interlace Functions [Need to be implemented]
        A. GRreqlutil
        B. GRreqimageil
    V. Palette Functions
        A. GRgetlutid w/GRgetlutinfo
        B. Read/Write Palettes
            1. GRwritelut
            2. GRreadlut
    VI. Special Element Functions [Need to be implemented]
        A. GRsetexternalfile
        B. GRsetaccesstype
        C. GRsetcompress
    VII. Atribute Functions
        A. GRattrinfo
        B. Read/Write Attributes
            1. GRsetattr
            2. GRgetattr
        C. GRfindattr
        
*/

/****************************************************************
**
**  test_mgr_init(): Multi-file Raster Initialization Test Routine
** 
****************************************************************/
void
test_mgr_init()
{
    int32 fid;              /* HDF file ID */
    int32 grid;             /* GRID for the interface */
    int32 n_datasets;       /* number of datasets */
    int32 n_attrs;          /* number of attributes */
    int32 ret;              /* generic return value */

    /* Output message about test being performed */
    MESSAGE(6, printf("Testing Multi-file Raster Initialization routines\n"););

    MESSAGE(8, printf("Try creating a new file and checking it out\n"););

    /* Ok, now create a new file */
    fid=Hopen(TESTFILE,DFACC_RDWR,0);
    CHECK(fid,FAIL,"Hopen");

    /* Try initializing the GR interface */
    grid=GRstart(fid);
    CHECK(grid,FAIL,"GRstart");

    ret=(intn)GRfileinfo(grid,&n_datasets,&n_attrs);
    CHECK(ret,FAIL,"GRfileinfo");

    if(n_datasets!=0 || n_attrs!=0)
      {
          MESSAGE(3, printf("Number of datasets/attributes in new file incorrect\n"););
          num_errs++;
      } /* end if */

    /* Shut down the GR interface */
    ret=GRend(grid);
    CHECK(ret,FAIL,"GRend");

    /* Close the file */
    ret=Hclose(fid);
    CHECK(ret,FAIL,"Hclose");


    MESSAGE(8, printf("Try checking out an existing file\n"););

    /* Ok, now check an existing file */

    fid=Hopen(DATAFILE,DFACC_READ,0);
    CHECK(fid,FAIL,"Hopen");

    /* Try initializing the GR interface */
    grid=GRstart(fid);
    CHECK(grid,FAIL,"GRstart");

    ret=(intn)GRfileinfo(grid,&n_datasets,&n_attrs);
    CHECK(ret,FAIL,"GRfileinfo");

    if(n_datasets!=5 || n_attrs!=2)
      {
          MESSAGE(3, printf("Number of datasets/attributes in existing file incorrect\n"););
          num_errs++;
      } /* end if */

    /* Shut down the GR interface */
    ret=GRend(grid);
    CHECK(ret,FAIL,"GRend");

    /* Close the file */
    ret=Hclose(fid);
    CHECK(ret,FAIL,"Hclose");
}   /* end test_mgr_init() */

/****************************************************************
**
**  test_mgr_image(): Multi-file Raster Image I/O Test Routine
** 
        A. GRcreate/GRselect/GRendaccess w/GRgetiminfo
        B. Write/Read images
            1. With no Data
                a. Default fill value
                b. user defined fill value
            2. With real Data
                a. New Image
                    1. With default fill value
                        aa. Whole image
                        bb. Sub-setted image
                        cc. Sub-sampled image
                    2. With user defined vill value
                        aa. Whole image
                        bb. Sub-setted image
                        cc. Sub-sampled image
                b. Existing Image
                    1. Whole image
                    2. Sub-setted image
                    3. Sub-sampled image
****************************************************************/
void
test_mgr_image()
{
    int32 fid;              /* HDF file ID */
    int32 grid;             /* GRID for the interface */
    int32 n_datasets;       /* number of datasets */
    int32 n_attrs;          /* number of attributes */
    int32 ret;              /* generic return value */
    intn i;                 /* local counting variables */

    /* Output message about test being performed */
    MESSAGE(6, printf("Testing Multi-file Raster Image I/O routines\n"););

    MESSAGE(8, printf("Try checking out the images in an existing file\n"););

    /* Open up the existing datafile and get the image information from it */
    fid=Hopen(DATAFILE,DFACC_READ,0);
    CHECK(fid,FAIL,"Hopen");

    /* Try initializing the GR interface */
    grid=GRstart(fid);
    CHECK(grid,FAIL,"GRstart");

    ret=(intn)GRfileinfo(grid,&n_datasets,&n_attrs);
    CHECK(ret,FAIL,"GRfileinfo");

    /* Walk through each existing dataset and verify it's contents */
    for(i=0; i<n_datasets; i++)
      {
          int32 riid;               /* RI ID for an image */
          char name[MAX_IMG_NAME];  /* storage for the image's name */
          int32 ncomp;              /* number of components */
          int32 nt;                 /* NT of the components */
          int32 il;                 /* interlace of the image data */
          int32 dimsizes[2];        /* dimension sizes of the image */
          int32 n_attr;             /* number of attributes with each image */
          VOIDP img_data;           /* buffer for the image data */

          /* Attach to the image */
          riid=GRselect(grid,i);
          CHECK(riid,FAIL,"GRselect");

          /* Get the Image information */
          *name='\0';
          ret=GRgetiminfo(riid,name,&ncomp,&nt,&il,dimsizes,&n_attr);
          CHECK(ret,FAIL,"GRgetiminfo");

          /* Check the name for correctness */
          if(HDstrcmp(name,datafile_info[i].name))
            {
                MESSAGE(3, printf("Name for image %d is: %s, should be %s\n",i,name,datafile_info[i].name););
                num_errs++;
            } /* end if */

          /* Check the # of components */
          if(ncomp!=datafile_info[i].ncomp)
            {
                MESSAGE(3, printf("Number of components for image %d is: %ld, should be %ld\n",i,(long)ncomp,(long)datafile_info[i].ncomp););
                num_errs++;
            } /* end if */

          /* Check the NT of components */
          if(nt!=datafile_info[i].nt)
            {
                MESSAGE(3, printf("NT of components for image %d is: %ld, should be %ld\n",i,(long)nt,(long)datafile_info[i].nt););
                num_errs++;
            } /* end if */

          /* Check the interlace of components */
          if(il!=datafile_info[i].il)
            {
                MESSAGE(3, printf("Interlace of components for image %d is: %ld, should be %ld\n",i,(long)il,(long)datafile_info[i].il););
                num_errs++;
            } /* end if */

          /* Check the x-dimension of the image */
          if(dimsizes[XDIM]!=datafile_info[i].dimsizes[XDIM])
            {
                MESSAGE(3, printf("X-dim of image %d is: %ld, should be %ld\n",i,(long)dimsizes[XDIM],(long)datafile_info[i].dimsizes[XDIM]););
                num_errs++;
            } /* end if */

          /* Check the y-dimension of the image */
          if(dimsizes[YDIM]!=datafile_info[i].dimsizes[YDIM])
            {
                MESSAGE(3, printf("Y-dim of image %d is: %ld, should be %ld\n",i,(long)dimsizes[YDIM],(long)datafile_info[i].dimsizes[YDIM]););
                num_errs++;
            } /* end if */

          /* Check the # of attributes of the image */
          if(n_attr!=datafile_info[i].n_attr)
            {
                MESSAGE(3, printf("# of attributes for image %d is: %ld, should be %ld\n",i,(long)n_attr,(long)datafile_info[i].n_attr););
                num_errs++;
            } /* end if */

          /* Check the image data itself */
          {
              int32 start[2];
              int32 stride[2];

                img_data=HDmalloc(dimsizes[0]*dimsizes[1]*ncomp*DFKNTsize(nt));
                CHECK(img_data,NULL,"HDmalloc");

                HDmemset(img_data,0,dimsizes[0]*dimsizes[1]*ncomp*DFKNTsize(nt));

                start[0]=start[1]=0;
                stride[0]=stride[1]=1;
                ret=GRreadimage(riid,start,stride,dimsizes,img_data);

                switch(i)
                  {
                      case 0:
                          if(0!=HDmemcmp(img_data,image0,sizeof(image0)))
                            {
                                MESSAGE(3, printf("Error reading data for image %d\n",i););
                                num_errs++;
                            } /* end if */
                          break;

                      case 1:
                          if(0!=HDmemcmp(img_data,image1,sizeof(image1)))
                            {
                                MESSAGE(3, printf("Error reading data for image %d\n",i););
                                num_errs++;
                            } /* end if */
                          break;

                      case 2:
                          if(0!=HDmemcmp(img_data,image2,sizeof(image2)))
                            {
                                MESSAGE(3, printf("Error reading data for image %d\n",i););
                                num_errs++;
                            } /* end if */
                          break;

                      case 3:
                          if(0!=HDmemcmp(img_data,image3,sizeof(image3)))
                            {
                                MESSAGE(3, printf("Error reading data for image %d\n",i););
                                num_errs++;
                            } /* end if */
                          break;

                      case 4:
                          if(0!=HDmemcmp(img_data,image4,sizeof(image4)))
                            {
                                MESSAGE(3, printf("Error reading data for image %d\n",i););
                                num_errs++;
                            } /* end if */
                          break;

                  } /* end switch */

                HDfree(img_data);
          } /* end block */

          /* End access to the image */
          ret=GRendaccess(riid);
          CHECK(ret,FAIL,"GRendaccess");
      } /* end for */

    /* Shut down the GR interface */
    ret=GRend(grid);
    CHECK(ret,FAIL,"GRend");

    /* Close the file */
    ret=Hclose(fid);
    CHECK(ret,FAIL,"Hclose");
}   /* end test_mgr_image() */

/****************************************************************
**
**  test_mgr_index(): Multi-file Raster ID/Ref/Index Test Routine
** 
****************************************************************/
void
test_mgr_index()
{
    /* Output message about test being performed */
    MESSAGE(6, printf("Testing Multi-file Raster ID/Ref/Index routines\n"););
}   /* end test_mgr_index() */

/****************************************************************
**
**  test_mgr_lut(): Multi-file Raster LUT/Palette Test Routine
** 
****************************************************************/
void
test_mgr_lut()
{
    /* Output message about test being performed */
    MESSAGE(6, printf("Testing Multi-file Raster Palette routines\n"););
}   /* end test_mgr_lut() */

/****************************************************************
**
**  test_mgr_attr(): Multi-file Raster Attribute Test Routine
** 
****************************************************************/
void
test_mgr_attr()
{
    /* Output message about test being performed */
    MESSAGE(6, printf("Testing Multi-file Raster Attribute routines\n"););
}   /* end test_mgr_attr() */

/****************************************************************
**
**  test_mgr(): Main multi-file raster image test routine
** 
****************************************************************/
void
test_mgr()
{
    /*
        Each major outline portion has it's own main function:
        I. Interface Initialization - test_mgr_init
        II. Create Images           - test_mgr_image
        III. ID/Ref/Index Functions - test_mgr_index
        IV. Interlace Functions [Need to be implemented]
        V. Palette Functions        - test_mgr_lut
        VI. Special Element Functions [Need to be implemented]
        VII. Atribute Functions     - test_mgr_attr
    */

    /* Output message about test being performed */
    MESSAGE(5, printf("Testing Multi-file Raster routines\n"););

    test_mgr_init();
    test_mgr_image();
    test_mgr_index();
    test_mgr_lut();
    test_mgr_attr();
}   /* test_mgr() */

