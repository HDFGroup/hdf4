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
#define TESTFILE2 "tmgrchk.hdf"
#define DATAFILE "tmgr.dat"

#define MFGR_TESTER
#include "tproto.h"
#include "mfgr.h"

/* Local pre-processor macros */
#define XDIM    0
#define YDIM    1
#define MAX_IMG_NAME    64  /* Maximum length of image names for this test */

/* Local Data to verify image information in datafile */
const struct {
    const char *name;
    int32 ncomp;
    int32 nt;
    int32 il;
    int32 dimsizes[2];
    int32 n_attr;
} datafile_info[]=
  { /* This information applies to the "tmgr.dat" file */
    {"Raster Image #0", 3, DFNT_UCHAR8, MFGR_INTERLACE_PIXEL, {13,15}, 2},
    {"Raster Image #1", 3, DFNT_UCHAR8, MFGR_INTERLACE_LINE, {13,15}, 2},
    {"Raster Image #2", 3, DFNT_UCHAR8, MFGR_INTERLACE_COMPONENT, {13,15}, 2},
    {"Test Image #1", 4, DFNT_UINT16, MFGR_INTERLACE_PIXEL, {21,23}, 3},
    {"Test Image #2", 2, DFNT_FLOAT64, MFGR_INTERLACE_PIXEL, {17,19}, 3}
  };

const uint8 image00[15][13][3]={
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

#ifdef QAK
static void dump_image(void *data, int32 xdim, int32 ydim, int32 ncomp, int32 nt);
#endif /* QAK */
static void test_mgr_init(void);
static void test_mgr_image_b1a(int flag);
static void test_mgr_image_b1b(int flag);
static void test_mgr_image_b2a1aa(int flag);
static void test_mgr_image_b2a1bb1(int flag);
static void test_mgr_image_b2a1bb2(int flag);
static void test_mgr_image_b2a1cc1(int flag);
static void test_mgr_image_b2a1cc2(int flag);
static void test_mgr_image_b2a2aa(int flag);
static void test_mgr_image_b2a2bb(int flag);
static void test_mgr_image_b2a2cc(int flag);
static void test_mgr_image_b2b1(int flag);
static void test_mgr_image_b2b2(int flag);
static void test_mgr_image_b2b3(int flag);
static void test_mgr_image(int flag);
static void test_mgr_index(int flag);
static void test_mgr_interlace(int flag);
static void test_mgr_lut(int flag);
static void test_mgr_special(int flag);
static void test_mgr_attr(int flag);

#ifdef QAK
static void dump_image(void *data, int32 xdim, int32 ydim, int32 ncomp, int32 nt)
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
                if(ncomp>1)
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
                                printf("%c",*ptr);
                            }
                            break;
#endif /* QAK */

                          case DFNT_UINT8:
                            {
                                unsigned char *ptr=(unsigned char *)data;
                                printf("%u",(unsigned)*ptr);
                            }
                            break;

                          case DFNT_INT8:
                            {
                                char *ptr=(char *)data;
                                printf("%d",(int)*ptr);
                            }
                            break;

                          case DFNT_UINT16:
                            {
                                uint16 *ptr=(uint16 *)data;
                                printf("%u",(unsigned)*ptr);
                            }
                            break;

                          case DFNT_INT16:
                            {
                                int16 *ptr=(int16 *)data;
                                printf("%d",(int)*ptr);
                            }
                            break;

                          case DFNT_UINT32:
                            {
                                uint32 *ptr=(uint32 *)data;
                                printf("%lu",(unsigned long)*ptr);
                            }
                            break;

                          case DFNT_INT32:
                            {
                                int32 *ptr=(int32 *)data;
                                printf("%ld",(long)*ptr);
                            }
                            break;

                          case DFNT_FLOAT32:
                            {
                                float32 *ptr=(float32 *)data;
                                printf("%f",(double)*ptr);
                            }
                            break;

                          case DFNT_FLOAT64:
                            {
                                float64 *ptr=(float64 *)data;
                                printf("%f",(double)*ptr);
                            }
                            break;

                          default:
                            printf("unknown NT: %ld\n",(long)nt);
                            break;

                      } /* end switch */
                    if(k<(ncomp-1))
                        printf(", ");
                    data=(void *)((char *)data+nt_size);
                  } /* end for */
                if(ncomp>1)
                    printf("}, ");
                else
                    printf(", ");

            } /* end for */
          printf("\n");
      } /* end for */
}   /* dump_image() */
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
        C. GRluttoref
    VI. Special Element Functions [Need to be implemented]
        A. GRsetexternalfile
        B. GRsetaccesstype
    VII. Atribute Functions
        A. GRattrinfo
        B. Read/Write Attributes
            1. GRsetattr
            2. GRgetattr
        C. GRfindattr
    VIII. Old-Style Raster Image Access
    IX. Compressed image Functions
        
*/

/****************************************************************
**
**  test_mgr_init(): Multi-file Raster Initialization Test Routine
** 
**  I. Interface Initialization
**      A. GRstart
**      B. GRend
**      C. GRfileinfo
** 
****************************************************************/
static void
test_mgr_init(void)
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
          MESSAGE(3, printf("Error! Number of datasets/attributes in new file incorrect\n"););
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
          MESSAGE(3, printf("Error! Number of datasets/attributes in existing file incorrect\n"););
          num_errs++;
      } /* end if */

    /* Shut down the GR interface */
    ret=GRend(grid);
    CHECK(ret,FAIL,"GRend");

    /* Close the file */
    ret=Hclose(fid);
    CHECK(ret,FAIL,"Hclose");
}   /* end test_mgr_init() */

/* Sub-tests for test_mgr_image() */
static void test_mgr_image_b1a(int flag)
{
    int32 fid;              /* HDF file ID */
    int32 grid;             /* GRID for the interface */
    int32 ret;              /* generic return value */
    int32   cdims[2] = {1,1};    /* chunk dims */
    int32   *rcdims;             /* for SDgetchunkinfo() */
    HDF_CHUNK_DEF chunk_def;     /* Chunk defintion set */ 
    HDF_CHUNK_DEF rchunk_def;    /* Chunk defintion read */ 
    int32     cflags;            /* chunk flags */

/* B1a - Read/Write images - with no Data - Default Fill Value */
    MESSAGE(8, printf("Check out I/O on image with no data, using the default fill value\n"););

    /* Open up the existing datafile and get the image information from it */
    if (flag)
        fid=Hopen(TESTFILE2,DFACC_RDWR,0);
    else
        fid=Hopen(TESTFILE,DFACC_RDWR,0);
    CHECK(fid,FAIL,"Hopen");

    /* Initialize the GR interface */
    grid=GRstart(fid);
    CHECK(grid,FAIL,"GRstart");

    {
        int32 riid;     /* RI ID for the new image */
        int32 dims[2]={4,5};    /* dimensions for the empty image */
        uint16 ref;     /* RI ref #. */
        int32 index;    /* RI index # */
        float32 image[5][4][3]; /* space for the image data */
        float32 image0[5][4][3]; /* space for the image data */
        int32 start[2]; /* start of image data to grab */
        int32 stride[2];/* stride of image data to grab */

        /* Create empty image with default fill value */
        riid=GRcreate(grid,"Empty Image",3,DFNT_FLOAT32,MFGR_INTERLACE_PIXEL,dims);
        CHECK(riid,FAIL,"GRcreate");

        /* Check if creating chunked GR */
        if (flag)
          {
              /* Create chunked GR 
                 chunk is 2x2 which will create 6 chunks */
              cdims[0] = chunk_def.chunk_lengths[0] = 2;
              cdims[1] = chunk_def.chunk_lengths[1] = 2;
              ret = GRsetchunk(riid, chunk_def, HDF_CHUNK);
              CHECK(ret,FAIL,"GRsetchunk");

              /* Set Chunk cache to hold 3 chunks */
              ret = GRsetchunkcache(riid, 3, 0);
              CHECK(ret,FAIL,"GRsetchunkcache");
          }

        /* Save the ref. # for later access */
        ref=GRidtoref(riid);
        CHECK(ref,(uint16)FAIL,"GRidtoref");

        /* Close the empty image */
        ret=GRendaccess(riid);
        CHECK(ret,FAIL,"GRendaccess");

        /* Get the index of the newly created image */
        index=GRreftoindex(grid,ref);
        CHECK(index,FAIL,"GRreftoindex");

        /* Select the newly created image */
        riid=GRselect(grid,index);
        CHECK(riid,FAIL,"GRselect");

        HDmemset(image,255,(size_t)(dims[0]*dims[1]*3)*sizeof(float32));
        /* '0' is the default fill value */
        HDmemset(image0,0,(size_t)(dims[0]*dims[1]*3)*sizeof(float32));

        start[0]=start[1]=0;
        stride[0]=stride[1]=1;
        ret=GRreadimage(riid,start,stride,dims,image);
        CHECK(ret,FAIL,"GRreadimage");

        if(0!=HDmemcmp(image,image0,sizeof(image0)))
          {
              MESSAGE(3, printf("Error reading data for image with default fill value\n"););
              num_errs++;
          } /* end if */

        /* check if we are doing chunked tests */
        if (flag)
          {
              /* Get chunk lengths */
              ret = GRgetchunkinfo(riid, &rchunk_def, &cflags);
              CHECK(ret,FAIL,"GRgetchunkinfo");

              rcdims = rchunk_def.chunk_lengths;

              /* check chunk lengths and to see if GR is chunked */
              if (cdims[0] != rcdims[0] || cdims[1] != rcdims[1] || cflags != HDF_CHUNK)
                {
                    fprintf(stderr, "Chunk Test. GRgetchunkinfo returned wrong values\n");
                    fprintf(stderr, "cdims[0]=%d,cdims[1]=%d \n",(int)cdims[0],(int)cdims[1]);
                    fprintf(stderr, "rcdims[0]=%d,rcdims[1]=%d \n",(int)rcdims[0],(int)rcdims[1]);
                    fprintf(stderr, "cflags =%d \n", (int)cflags );
                }

          }

        /* Close the empty image */
        ret=GRendaccess(riid);
        CHECK(ret,FAIL,"GRendaccess");
    }
    
    /* Shut down the GR interface */
    ret=GRend(grid);
    CHECK(ret,FAIL,"GRend");

    /* Close the file */
    ret=Hclose(fid);
    CHECK(ret,FAIL,"Hclose");
} /* end test_mgr_image_b1a() */

static void test_mgr_image_b1b(int flag)
{
    int32 fid;              /* HDF file ID */
    int32 grid;             /* GRID for the interface */
    int32 ret;              /* generic return value */
    int32   cdims[2] = {1,1};    /* chunk dims */
    int32   *rcdims;             /* for SDgetchunkinfo() */
    HDF_CHUNK_DEF chunk_def;     /* Chunk defintion set */ 
    HDF_CHUNK_DEF rchunk_def;    /* Chunk defintion read */ 
    int32     cflags;            /* chunk flags */

/* B1b - Read/Write images - with no Data - User-defined Fill Value */
    MESSAGE(8, printf("Check out I/O on image with no data, using User Defined fill-value\n"););

    /* Open up the existing datafile and get the image information from it */
    if (flag)
        fid=Hopen(TESTFILE2,DFACC_RDWR,0);
    else
        fid=Hopen(TESTFILE,DFACC_RDWR,0);
    CHECK(fid,FAIL,"Hopen");

    /* Initialize the GR interface */
    grid=GRstart(fid);
    CHECK(grid,FAIL,"GRstart");

    {
        int32 riid;     /* RI ID for the new image */
        int32 dims[2]={5,7};    /* dimensions for the empty image */
        uint16 ref;     /* RI ref #. */
        int32 index;    /* RI index # */
        float64 image[7][5][4]; /* space for the image data */
        float64 fill_pixel[4]={1.3,-2.4,1000.3,.25};   /* pixel with fill values */
        float64 image0[7][5][4]; /* space for the image data */
        int32 start[2]; /* start of image data to grab */
        int32 stride[2];/* stride of image data to grab */

        /* Create empty image with default fill value */
        riid=GRcreate(grid,"Empty Image2",4,DFNT_FLOAT64,MFGR_INTERLACE_PIXEL,dims);
        CHECK(riid,FAIL,"GRcreate");

        /* Set the fill-value */
        ret=GRsetattr(riid,FILL_ATTR,DFNT_FLOAT64,sizeof(fill_pixel)/sizeof(float64),fill_pixel);
        CHECK(ret,FAIL,"GRsetattr");

        /* Check if creating chunked GR */
        if (flag)
          {
              /* Create chunked GR 
                 chunk is 2x2 which will create 6 chunks */
              cdims[0] = chunk_def.chunk_lengths[0] = 2;
              cdims[1] = chunk_def.chunk_lengths[1] = 2;
              ret = GRsetchunk(riid, chunk_def, HDF_CHUNK);
              CHECK(ret,FAIL,"GRsetchunk");
          }

        /* Save the ref. # for later access */
        ref=GRidtoref(riid);
        CHECK(ref,(uint16)FAIL,"GRidtoref");

        /* Close the empty image */
        ret=GRendaccess(riid);
        CHECK(ret,FAIL,"GRendaccess");

        /* Get the index of the newly created image */
        index=GRreftoindex(grid,ref);
        CHECK(index,FAIL,"GRreftoindex");

        /* Select the newly created image */
        riid=GRselect(grid,index);
        CHECK(riid,FAIL,"GRselect");

        HDmemset(image,0,(size_t)(dims[0]*dims[1]*4)*sizeof(float64));
        /* fill the memory-only with the default pixel fill-value */
        HDmemfill(image0,fill_pixel,sizeof(fill_pixel),sizeof(image0)/sizeof(fill_pixel));

        start[0]=start[1]=0;
        stride[0]=stride[1]=1;
        ret=GRreadimage(riid,start,stride,dims,image);
        CHECK(ret,FAIL,"GRreadimage");

        if(0!=HDmemcmp(image,image0,sizeof(image0)))
          {
              MESSAGE(3, printf("Error reading data for image with user defined fill-value\n"););
              num_errs++;
          } /* end if */

        /* check if we are doing chunked tests */
        if (flag)
          {
              /* Get chunk lengths */
              ret = GRgetchunkinfo(riid, &rchunk_def, &cflags);
              CHECK(ret,FAIL,"GRgetchunkinfo");

              rcdims = rchunk_def.chunk_lengths;

              /* check chunk lengths and to see if GR is chunked */
              if (cdims[0] != rcdims[0] || cdims[1] != rcdims[1] || cflags != HDF_CHUNK)
                {
                    fprintf(stderr, "Chunk Test. GRgetchunkinfo returned wrong values\n");
                    fprintf(stderr, "cdims[0]=%d,cdims[1]=%d \n",(int)cdims[0],(int)cdims[1]);
                    fprintf(stderr, "rcdims[0]=%d,rcdims[1]=%d \n",(int)rcdims[0],(int)rcdims[1]);
                    fprintf(stderr, "cflags =%d \n", (int)cflags );
                }

          }

        /* Close the empty image */
        ret=GRendaccess(riid);
        CHECK(ret,FAIL,"GRendaccess");
    }
    
    /* Shut down the GR interface */
    ret=GRend(grid);
    CHECK(ret,FAIL,"GRend");

    /* Close the file */
    ret=Hclose(fid);
    CHECK(ret,FAIL,"Hclose");
} /* end test_mgr_image_b1b() */

static void test_mgr_image_b2a1aa(int flag)
{
    int32 fid;              /* HDF file ID */
    int32 grid;             /* GRID for the interface */
    int32 ret;              /* generic return value */
    int32   cdims[2] = {1,1};    /* chunk dims */
    int32   *rcdims;             /* for SDgetchunkinfo() */
    HDF_CHUNK_DEF chunk_def;     /* Chunk defintion set */ 
    HDF_CHUNK_DEF rchunk_def;    /* Chunk defintion read */ 
    int32     cflags;            /* chunk flags */

/* B2a1aa - Read/Write images - with real Data - New Image - with Default Fill Value - Whole Image */
    MESSAGE(8, printf("Check out I/O on new image with real data, with Default fill-value, Whole Image\n"););

    /* Open up the existing datafile and get the image information from it */
    if (flag)
        fid=Hopen(TESTFILE2,DFACC_RDWR,0);
    else
        fid=Hopen(TESTFILE,DFACC_RDWR,0);
    CHECK(fid,FAIL,"Hopen");

    /* Initialize the GR interface */
    grid=GRstart(fid);
    CHECK(grid,FAIL,"GRstart");

    {
#ifdef TEST_XDIM
#undef TEST_XDIM
#endif /* TEST_XDIM */
#define TEST_XDIM    3
#ifdef TEST_YDIM
#undef TEST_YDIM
#endif /* TEST_YDIM */
#define TEST_YDIM    8
#ifdef TEST_NCOMP
#undef TEST_NCOMP
#endif /* TEST_NCOMP */
#define TEST_NCOMP   2
#ifdef TEST_VARTYPE
#undef TEST_VARTYPE
#endif /* TEST_VARTYPE */
#define TEST_VARTYPE int32
#ifdef TEST_NT
#undef TEST_NT
#endif /* TEST_NT */
#define TEST_NT      DFNT_INT32

        int32 riid;     /* RI ID for the new image */
        int32 dims[2]={TEST_XDIM,TEST_YDIM};    /* dimensions for the empty image */
        uint16 ref;     /* RI ref #. */
        int32 index;    /* RI index # */
        TEST_VARTYPE image[TEST_YDIM][TEST_XDIM][TEST_NCOMP]; /* space for the image data */
        TEST_VARTYPE fill_pixel[TEST_NCOMP]={1,-2};   /* pixel with fill values */
        TEST_VARTYPE fill_pixel2[TEST_NCOMP]={-2,1};   /* pixel with fill values */
        TEST_VARTYPE image0[TEST_YDIM][TEST_XDIM][TEST_NCOMP]; /* space for the image data */
        int32 start[2]; /* start of image data to grab */
        int32 stride[2];/* stride of image data to grab */
        intn i,j;       /* local counting variables */

        /* fill the memory-only with the default pixel fill-value */
        for(i=0; i<TEST_YDIM; i++)
          {
              for(j=0; j<TEST_XDIM; j++)
                {
                    if((j%2)==0)
                        HDmemcpy(&image0[i][j][0],fill_pixel,sizeof(fill_pixel));
                    else
                        HDmemcpy(&image0[i][j][0],fill_pixel2,sizeof(fill_pixel2));
                } /* end for */
          } /* end for */
        HDmemcpy(image,image0,sizeof(image0));

        /* Create empty image with default fill value */
        riid=GRcreate(grid,"Test Image B2a1aa",TEST_NCOMP,TEST_NT,MFGR_INTERLACE_PIXEL,dims);
        CHECK(riid,FAIL,"GRcreate");

        /* Check if creating chunked GR */
        if (flag)
          {
              /* Create chunked GR 
                 chunk is 2x2 which will create 6 chunks */
              cdims[0] = chunk_def.chunk_lengths[0] = 2;
              cdims[1] = chunk_def.chunk_lengths[1] = 2;
              ret = GRsetchunk(riid, chunk_def, HDF_CHUNK);
              CHECK(ret,FAIL,"GRsetchunk");
          }

        /* Save the ref. # for later access */
        ref=GRidtoref(riid);
        CHECK(ref,(uint16)FAIL,"GRidtoref");

        start[0]=start[1]=0;
        stride[0]=stride[1]=1;
        ret=GRwriteimage(riid,start,stride,dims,image);
        CHECK(ret,FAIL,"GRwriteimage");

        /* Close the empty image */
        ret=GRendaccess(riid);
        CHECK(ret,FAIL,"GRendaccess");

        /* Get the index of the newly created image */
        index=GRreftoindex(grid,ref);
        CHECK(index,FAIL,"GRreftoindex");

        /* Select the newly created image */
        riid=GRselect(grid,index);
        CHECK(riid,FAIL,"GRselect");

        start[0]=start[1]=0;
        stride[0]=stride[1]=1;
        ret=GRreadimage(riid,start,stride,dims,image);
        CHECK(ret,FAIL,"GRreadimage");

        if(0!=HDmemcmp(image,image0,sizeof(image0)))
          {
              MESSAGE(3, printf("%d:Error reading data for new image with default fill-value, whole image\n",__LINE__););
              num_errs++;
          } /* end if */

        /* check if we are doing chunked tests */
        if (flag)
          {
              /* Get chunk lengths */
              ret = GRgetchunkinfo(riid, &rchunk_def, &cflags);
              CHECK(ret,FAIL,"GRgetchunkinfo");

              rcdims = rchunk_def.chunk_lengths;

              /* check chunk lengths and to see if GR is chunked */
              if (cdims[0] != rcdims[0] || cdims[1] != rcdims[1] || cflags != HDF_CHUNK)
                {
                    fprintf(stderr, "Chunk Test. GRgetchunkinfo returned wrong values\n");
                    fprintf(stderr, "cdims[0]=%d,cdims[1]=%d \n",(int)cdims[0],(int)cdims[1]);
                    fprintf(stderr, "rcdims[0]=%d,rcdims[1]=%d \n",(int)rcdims[0],(int)rcdims[1]);
                    fprintf(stderr, "cflags =%d \n", (int)cflags );
                }

          }

        /* Close the empty image */
        ret=GRendaccess(riid);
        CHECK(ret,FAIL,"GRendaccess");
    }
    
    /* Shut down the GR interface */
    ret=GRend(grid);
    CHECK(ret,FAIL,"GRend");

    /* Close the file */
    ret=Hclose(fid);
    CHECK(ret,FAIL,"Hclose");
} /* end test_mgr_image_b2a1aa() */

static void test_mgr_image_b2a1bb1(int flag)
{
    int32 fid;              /* HDF file ID */
    int32 grid;             /* GRID for the interface */
    int32 ret;              /* generic return value */
    int32   cdims[2] = {1,1};    /* chunk dims */
    int32   *rcdims;             /* for SDgetchunkinfo() */
    HDF_CHUNK_DEF chunk_def;     /* Chunk defintion set */ 
    HDF_CHUNK_DEF rchunk_def;    /* Chunk defintion read */ 
    int32     cflags;            /* chunk flags */

/* B2a1bb - Read/Write images - with real Data - New Image - with Default Fill Value - Sub-setted Image */
    MESSAGE(8, printf("Check out I/O on new image with real data, with Default fill-value, Writing Sub-setted Image\n"););

    /* Open up the existing datafile and get the image information from it */
    if (flag)
        fid=Hopen(TESTFILE2,DFACC_RDWR,0);
    else
        fid=Hopen(TESTFILE,DFACC_RDWR,0);
    CHECK(fid,FAIL,"Hopen");

    /* Initialize the GR interface */
    grid=GRstart(fid);
    CHECK(grid,FAIL,"GRstart");

    {
#ifdef TEST_XDIM
#undef TEST_XDIM
#endif /* TEST_XDIM */
#define TEST_XDIM    19
#ifdef TEST_YDIM
#undef TEST_YDIM
#endif /* TEST_YDIM */
#define TEST_YDIM    23
#ifdef TEST_NCOMP
#undef TEST_NCOMP
#endif /* TEST_NCOMP */
#define TEST_NCOMP   4
#ifdef TEST_VARTYPE
#undef TEST_VARTYPE
#endif /* TEST_VARTYPE */
#define TEST_VARTYPE uint16
#ifdef TEST_NT
#undef TEST_NT
#endif /* TEST_NT */
#define TEST_NT      DFNT_UINT16

        int32 riid;     /* RI ID for the new image */
        int32 dims[2]={TEST_XDIM,TEST_YDIM};    /* dimensions for the empty image */
        uint16 ref;     /* RI ref #. */
        int32 index;    /* RI index # */
        TEST_VARTYPE image[TEST_YDIM][TEST_XDIM][TEST_NCOMP]; /* space for the image data */
        TEST_VARTYPE fill_pixel[TEST_NCOMP]={40000,4800,3,1000};   /* pixel with fill values */
        TEST_VARTYPE fill_pixel2[TEST_NCOMP]={1230,1,65000,35000};   /* pixel with fill values */
        TEST_VARTYPE image0[TEST_YDIM][TEST_XDIM][TEST_NCOMP]; /* space for the image data */
        TEST_VARTYPE sub_image[TEST_YDIM][TEST_XDIM][TEST_NCOMP]; /* space for the image data */
        TEST_VARTYPE *sub_ptr;
        int32 start[2]; /* start of image data to use */
        int32 stride[2];/* stride of image data to use */
        int32 count[2]; /* # of pixels of image data to use */
        intn i,j,k;     /* local counting variables */

        /* fill the memory-only with the default pixel fill-values */
        HDmemset(image0,0,sizeof(image0));
        sub_ptr=(TEST_VARTYPE *)sub_image;
        for(i=0; i<TEST_YDIM; i++)
          {
              for(j=0; j<TEST_XDIM; j++)
                {
                    if(((i>(TEST_YDIM/3)) && (i<(2*TEST_YDIM/3)))
                        && ((j>(TEST_XDIM/4)) && (j<(3*TEST_XDIM/4))))
                      {
                          if((j%2)==0)
                              HDmemcpy(&image0[i][j][0],fill_pixel,sizeof(fill_pixel));
                          else
                              HDmemcpy(&image0[i][j][0],fill_pixel2,sizeof(fill_pixel2));
                          HDmemcpy(sub_ptr,&image0[i][j][0],TEST_NCOMP*sizeof(TEST_VARTYPE));
                          sub_ptr+=TEST_NCOMP;
                      } /* end if */
                } /* end for */
          } /* end for */

        /* initialize the disk buffer */
        HDmemset(image,255,sizeof(image));

        /* Create empty image with default fill value */
        riid=GRcreate(grid,"Test Image B2a1bb",TEST_NCOMP,TEST_NT,MFGR_INTERLACE_PIXEL,dims);
        CHECK(riid,FAIL,"GRcreate");

        /* Check if creating chunked GR */
        if (flag)
          {
              /* Create chunked GR 
                 chunk is 2x2 which will create 6 chunks */
              cdims[0] = chunk_def.chunk_lengths[0] = 2;
              cdims[1] = chunk_def.chunk_lengths[1] = 2;
              ret = GRsetchunk(riid, chunk_def, HDF_CHUNK);
              CHECK(ret,FAIL,"GRsetchunk");
          }

        /* Save the ref. # for later access */
        ref=GRidtoref(riid);
        CHECK(ref,(uint16)FAIL,"GRidtoref");

        /* Create sub-setted window with only the filled pixels in it */
        start[XDIM]=(TEST_XDIM/4)+1;
        start[YDIM]=(TEST_YDIM/3)+1;
        count[XDIM]=((3*TEST_XDIM/4)-(TEST_XDIM/4))-1;
        count[YDIM]=((2*TEST_YDIM/3)-(TEST_YDIM/3))-1;
        stride[XDIM]=stride[YDIM]=1;
        ret=GRwriteimage(riid,start,stride,count,sub_image);
        CHECK(ret,FAIL,"GRwriteimage");

        /* Close the empty image */
        ret=GRendaccess(riid);
        CHECK(ret,FAIL,"GRendaccess");

        /* Get the index of the newly created image */
        index=GRreftoindex(grid,ref);
        CHECK(index,FAIL,"GRreftoindex");

        /* Select the newly created image */
        riid=GRselect(grid,index);
        CHECK(riid,FAIL,"GRselect");

        /* Get the whole image back */
        start[XDIM]=start[YDIM]=0;
        stride[XDIM]=stride[YDIM]=1;
        ret=GRreadimage(riid,start,stride,dims,image);
        CHECK(ret,FAIL,"GRreadimage");

        if(0!=HDmemcmp(image,image0,sizeof(image0)))
          {
              MESSAGE(3, printf("%d:Error reading data for new image with default fill-value, sub-setted image\n",__LINE__););

              MESSAGE(8, for(i=0; i<TEST_YDIM; i++) \
                      for(j=0; j<TEST_XDIM; j++) \
                          for(k=0; k<TEST_NCOMP; k++) \
                              if(image[i][j][k]!=image0[i][j][k]) \
                                  printf("Location: [%d][%d][%d] image=%u, image0=%u \n",i,j,k,(unsigned)image[i][j][k],(unsigned)image0[i][j][k]); );
              num_errs++;
          } /* end if */

        /* check if we are doing chunked tests */
        if (flag)
          {
              /* Get chunk lengths */
              ret = GRgetchunkinfo(riid, &rchunk_def, &cflags);
              CHECK(ret,FAIL,"GRgetchunkinfo");

              rcdims = rchunk_def.chunk_lengths;

              /* check chunk lengths and to see if GR is chunked */
              if (cdims[0] != rcdims[0] || cdims[1] != rcdims[1] || cflags != HDF_CHUNK)
                {
                    fprintf(stderr, "Chunk Test. GRgetchunkinfo returned wrong values\n");
                    fprintf(stderr, "cdims[0]=%d,cdims[1]=%d \n",(int)cdims[0],(int)cdims[1]);
                    fprintf(stderr, "rcdims[0]=%d,rcdims[1]=%d \n",(int)rcdims[0],(int)rcdims[1]);
                    fprintf(stderr, "cflags =%d \n", (int)cflags );
                }

          }

        /* Close the empty image */
        ret=GRendaccess(riid);
        CHECK(ret,FAIL,"GRendaccess");
    }
    
    /* Shut down the GR interface */
    ret=GRend(grid);
    CHECK(ret,FAIL,"GRend");

    /* Close the file */
    ret=Hclose(fid);
    CHECK(ret,FAIL,"Hclose");

} /* end test_mgr_image_b2a1bb1() */

static void test_mgr_image_b2a1bb2(int flag)
{
    int32 fid;              /* HDF file ID */
    int32 grid;             /* GRID for the interface */
    int32 ret;              /* generic return value */
    int32   cdims[2] = {1,1};    /* chunk dims */
    int32   *rcdims;             /* for SDgetchunkinfo() */
    HDF_CHUNK_DEF chunk_def;     /* Chunk defintion set */ 
    HDF_CHUNK_DEF rchunk_def;    /* Chunk defintion read */ 
    int32     cflags;            /* chunk flags */

    MESSAGE(8, printf("Check out I/O on new image with real data, with Default fill-value, Reading Sub-setted Image\n"););
    /* Open up the existing datafile and get the image information from it */
    if (flag)
        fid=Hopen(TESTFILE2,DFACC_RDWR,0);
    else
        fid=Hopen(TESTFILE,DFACC_RDWR,0);
    CHECK(fid,FAIL,"Hopen");

    /* Initialize the GR interface */
    grid=GRstart(fid);
    CHECK(grid,FAIL,"GRstart");

    {
#ifdef TEST_XDIM
#undef TEST_XDIM
#endif /* TEST_XDIM */
#define TEST_XDIM    19
#ifdef TEST_YDIM
#undef TEST_YDIM
#endif /* TEST_YDIM */
#define TEST_YDIM    23
#ifdef TEST_NCOMP
#undef TEST_NCOMP
#endif /* TEST_NCOMP */
#define TEST_NCOMP   4
#ifdef TEST_VARTYPE
#undef TEST_VARTYPE
#endif /* TEST_VARTYPE */
#define TEST_VARTYPE uint16
#ifdef TEST_NT
#undef TEST_NT
#endif /* TEST_NT */
#define TEST_NT      DFNT_UINT16

        int32 riid;     /* RI ID for the new image */
        int32 dims[2]={TEST_XDIM,TEST_YDIM};    /* dimensions for the empty image */
        uint16 ref;     /* RI ref #. */
        int32 index;    /* RI index # */
        TEST_VARTYPE image[TEST_YDIM][TEST_XDIM][TEST_NCOMP]; /* space for the image data */
        TEST_VARTYPE fill_pixel[TEST_NCOMP]={40000,4800,3,1000};   /* pixel with fill values */
        TEST_VARTYPE fill_pixel2[TEST_NCOMP]={1230,1,65000,35000};   /* pixel with fill values */
        TEST_VARTYPE image0[TEST_YDIM][TEST_XDIM][TEST_NCOMP]; /* space for the image data */
        TEST_VARTYPE sub_image[TEST_YDIM][TEST_XDIM][TEST_NCOMP]; /* space for the image data */
        TEST_VARTYPE *sub_ptr;
        int32 start[2]; /* start of image data to use */
        int32 stride[2];/* stride of image data to use */
        int32 count[2]; /* # of pixels of image data to use */
        intn i,j;       /* local counting variables */

        /* fill the memory-only with the default pixel fill-values */
        HDmemset(image0,0,sizeof(image0));
        sub_ptr=(TEST_VARTYPE *)sub_image;
        for(i=0; i<TEST_YDIM; i++)
          {
              for(j=0; j<TEST_XDIM; j++)
                {
                    if((j%2)==0)
                        HDmemcpy(&image0[i][j][0],fill_pixel,sizeof(fill_pixel));
                    else
                        HDmemcpy(&image0[i][j][0],fill_pixel2,sizeof(fill_pixel2));
                    if(((i>(TEST_YDIM/3)) && (i<(2*TEST_YDIM/3)))
                        && ((j>(TEST_XDIM/4)) && (j<(3*TEST_XDIM/4))))
                      {
                          HDmemcpy(sub_ptr,&image0[i][j][0],TEST_NCOMP*sizeof(TEST_VARTYPE));
                          sub_ptr+=TEST_NCOMP;
                      } /* end if */
                } /* end for */
          } /* end for */

        /* initialize the disk buffer */
        HDmemset(image,255,sizeof(image));

        /* Create empty image with default fill value */
        riid=GRcreate(grid,"Test Image B2a1bb2",TEST_NCOMP,TEST_NT,MFGR_INTERLACE_PIXEL,dims);
        CHECK(riid,FAIL,"GRcreate");

        /* Check if creating chunked GR */
        if (flag)
          {
              /* Create chunked GR 
                 chunk is 2x2 which will create 6 chunks */
              cdims[0] = chunk_def.chunk_lengths[0] = 2;
              cdims[1] = chunk_def.chunk_lengths[1] = 2;
              ret = GRsetchunk(riid, chunk_def, HDF_CHUNK);
              CHECK(ret,FAIL,"GRsetchunk");

              /* Set Chunk cache to hold 3 chunks */
              ret = GRsetchunkcache(riid, 3, 0);
              CHECK(ret,FAIL,"GRsetchunkcache");
          }

        /* Save the ref. # for later access */
        ref=GRidtoref(riid);
        CHECK(ref,(uint16)FAIL,"GRidtoref");

        /* Create whole image */
        start[XDIM]=start[YDIM]=0;
        stride[XDIM]=stride[YDIM]=1;
        ret=GRwriteimage(riid,start,stride,dims,image0);
        CHECK(ret,FAIL,"GRwriteimage");

        /* Close the empty image */
        ret=GRendaccess(riid);
        CHECK(ret,FAIL,"GRendaccess");

        /* Get the index of the newly created image */
        index=GRreftoindex(grid,ref);
        CHECK(index,FAIL,"GRreftoindex");

        /* Select the newly created image */
        riid=GRselect(grid,index);
        CHECK(riid,FAIL,"GRselect");

        /* Get the sub-set image back */
        start[XDIM]=(TEST_XDIM/4)+1;
        start[YDIM]=(TEST_YDIM/3)+1;
        count[XDIM]=((3*TEST_XDIM/4)-(TEST_XDIM/4))-1;
        count[YDIM]=((2*TEST_YDIM/3)-(TEST_YDIM/3))-1;
        stride[XDIM]=stride[YDIM]=1;
        ret=GRreadimage(riid,start,stride,count,image);
        CHECK(ret,FAIL,"GRreadimage");

        if(0!=HDmemcmp(image,sub_image,(size_t)(count[XDIM]*count[YDIM])*sizeof(fill_pixel)))
          {
              MESSAGE(3, printf("%d:Error reading data for new image with default fill-value, sub-setted image\n",__LINE__););
              num_errs++;
          } /* end if */

        /* check if we are doing chunked tests */
        if (flag)
          {
              /* Get chunk lengths */
              ret = GRgetchunkinfo(riid, &rchunk_def, &cflags);
              CHECK(ret,FAIL,"GRgetchunkinfo");

              rcdims = rchunk_def.chunk_lengths;

              /* check chunk lengths and to see if GR is chunked */
              if (cdims[0] != rcdims[0] || cdims[1] != rcdims[1] || cflags != HDF_CHUNK)
                {
                    fprintf(stderr, "Chunk Test. GRgetchunkinfo returned wrong values\n");
                    fprintf(stderr, "cdims[0]=%d,cdims[1]=%d \n",(int)cdims[0],(int)cdims[1]);
                    fprintf(stderr, "rcdims[0]=%d,rcdims[1]=%d \n",(int)rcdims[0],(int)rcdims[1]);
                    fprintf(stderr, "cflags =%d \n", (int)cflags );
                }

          }

        /* Close the empty image */
        ret=GRendaccess(riid);
        CHECK(ret,FAIL,"GRendaccess");
    }
    
    /* Shut down the GR interface */
    ret=GRend(grid);
    CHECK(ret,FAIL,"GRend");

    /* Close the file */
    ret=Hclose(fid);
    CHECK(ret,FAIL,"Hclose");

} /* end test_mgr_image_b2a1bb2() */

static void test_mgr_image_b2a1cc1(int flag)
{
    int32 fid;              /* HDF file ID */
    int32 grid;             /* GRID for the interface */
    int32 ret;              /* generic return value */
    int32   cdims[2] = {1,1};    /* chunk dims */
    int32   *rcdims;             /* for SDgetchunkinfo() */
    HDF_CHUNK_DEF chunk_def;     /* Chunk defintion set */ 
    HDF_CHUNK_DEF rchunk_def;    /* Chunk defintion read */ 
    int32     cflags;            /* chunk flags */

/* B2a1cc - Read/Write images - with real Data - New Image - with Default Fill Value - Sub-sampled Image */
    MESSAGE(8, printf("Check out I/O on new image with real data, with Default fill-value, Writing Sub-sampled Image\n"););

    /* Open up the existing datafile and get the image information from it */
    if (flag)
        fid=Hopen(TESTFILE2,DFACC_RDWR,0);
    else
        fid=Hopen(TESTFILE,DFACC_RDWR,0);
    CHECK(fid,FAIL,"Hopen");

    /* Initialize the GR interface */
    grid=GRstart(fid);
    CHECK(grid,FAIL,"GRstart");

    {
#ifdef TEST_XDIM
#undef TEST_XDIM
#endif /* TEST_XDIM */
#define TEST_XDIM    19
#ifdef TEST_YDIM
#undef TEST_YDIM
#endif /* TEST_YDIM */
#define TEST_YDIM    23
#ifdef TEST_NCOMP
#undef TEST_NCOMP
#endif /* TEST_NCOMP */
#define TEST_NCOMP   5
#ifdef TEST_VARTYPE
#undef TEST_VARTYPE
#endif /* TEST_VARTYPE */
#define TEST_VARTYPE int16
#ifdef TEST_NT
#undef TEST_NT
#endif /* TEST_NT */
#define TEST_NT      DFNT_INT16

        int32 riid;     /* RI ID for the new image */
        int32 dims[2]={TEST_XDIM,TEST_YDIM};    /* dimensions for the empty image */
        uint16 ref;     /* RI ref #. */
        int32 index;    /* RI index # */
        TEST_VARTYPE image[TEST_YDIM][TEST_XDIM][TEST_NCOMP]; /* space for the image data */
        TEST_VARTYPE fill_pixel[TEST_NCOMP]={-20000,-1,4800,3,1000};   /* pixel with fill values */
        TEST_VARTYPE fill_pixel2[TEST_NCOMP]={45,1230,1,32000,-32000};   /* pixel with fill values */
        TEST_VARTYPE image0[TEST_YDIM][TEST_XDIM][TEST_NCOMP]; /* space for the image data */
        TEST_VARTYPE sub_image[TEST_YDIM][TEST_XDIM][TEST_NCOMP]; /* space for the image data */
        TEST_VARTYPE *sub_ptr;
        int32 start[2]; /* start of image data to use */
        int32 stride[2];/* stride of image data to use */
        int32 count[2]; /* # of pixels of image data to use */
        intn i,j,k;     /* local counting variables */

        /* fill the memory-only with the default pixel fill-values */
        HDmemset(image0,0,sizeof(TEST_VARTYPE)*(size_t)(TEST_YDIM*TEST_XDIM*TEST_NCOMP));
        sub_ptr=(TEST_VARTYPE *)sub_image;
        for(i=0; i<TEST_YDIM; i++)
          {
              for(j=0; j<TEST_XDIM; j++)
                {
                    if((i%2)!=0 && (j%2)!=0)
                      {
                          if((j%3)==0)
                              HDmemcpy(&image0[i][j][0],fill_pixel,sizeof(TEST_VARTYPE)*TEST_NCOMP);
                          else
                              HDmemcpy(&image0[i][j][0],fill_pixel2,sizeof(TEST_VARTYPE)*TEST_NCOMP);
                          HDmemcpy(sub_ptr,&image0[i][j][0],TEST_NCOMP*sizeof(TEST_VARTYPE));
                          sub_ptr+=TEST_NCOMP;
                      } /* end if */
                } /* end for */
          } /* end for */

        /* initialize the disk buffer */
        HDmemset(image,255,sizeof(TEST_VARTYPE)*(size_t)(TEST_YDIM*TEST_XDIM*TEST_NCOMP));

        /* Create empty image with default fill value */
        riid=GRcreate(grid,"Test Image B2a1cc",TEST_NCOMP,TEST_NT,MFGR_INTERLACE_PIXEL,dims);
        CHECK(riid,FAIL,"GRcreate");

        /* Check if creating chunked GR */
        if (flag)
          {
              /* Create chunked GR 
                 chunk is 2x2 which will create 6 chunks */
              cdims[0] = chunk_def.chunk_lengths[0] = 2;
              cdims[1] = chunk_def.chunk_lengths[1] = 2;
              ret = GRsetchunk(riid, chunk_def, HDF_CHUNK);
              CHECK(ret,FAIL,"GRsetchunk");
          }

        /* Save the ref. # for later access */
        ref=GRidtoref(riid);
        CHECK(ref,(uint16)FAIL,"GRidtoref");

        /* Create sub-sampled window with only the filled pixels in it */
        start[XDIM]=1;
        start[YDIM]=1;
        count[XDIM]=TEST_XDIM/2;
        count[YDIM]=TEST_YDIM/2;
        stride[XDIM]=stride[YDIM]=2;
        ret=GRwriteimage(riid,start,stride,count,sub_image);
        CHECK(ret,FAIL,"GRwriteimage");

        /* Close the empty image */
        ret=GRendaccess(riid);
        CHECK(ret,FAIL,"GRendaccess");

        /* Get the index of the newly created image */
        index=GRreftoindex(grid,ref);
        CHECK(index,FAIL,"GRreftoindex");

        /* Select the newly created image */
        riid=GRselect(grid,index);
        CHECK(riid,FAIL,"GRselect");

        /* Get the whole image back */
        start[XDIM]=start[YDIM]=0;
        stride[XDIM]=stride[YDIM]=1;
        ret=GRreadimage(riid,start,stride,dims,image);
        CHECK(ret,FAIL,"GRreadimage");

        if(0!=HDmemcmp(image,image0,sizeof(TEST_VARTYPE)*TEST_YDIM*TEST_XDIM*TEST_NCOMP))
          {
              MESSAGE(3, printf("%d:Error reading data for new image with default fill-value, sub-sampled image\n",__LINE__););

              MESSAGE(8, for(i=0; i<TEST_YDIM; i++) \
                      for(j=0; j<TEST_XDIM; j++) \
                          for(k=0; k<TEST_NCOMP; k++) \
                            if(image[i][j][k]!=image0[i][j][k]) \
                                  printf("Location: [%d][%d][%d] image=%d, image0=%d \n",(int)i,(int)j,(int)k,(int)image[i][j][k],(int)image0[i][j][k]); );
              num_errs++;
          } /* end if */

        /* check if we are doing chunked tests */
        if (flag)
          {
              /* Get chunk lengths */
              ret = GRgetchunkinfo(riid, &rchunk_def, &cflags);
              CHECK(ret,FAIL,"GRgetchunkinfo");

              rcdims = rchunk_def.chunk_lengths;

              /* check chunk lengths and to see if GR is chunked */
              if (cdims[0] != rcdims[0] || cdims[1] != rcdims[1] || cflags != HDF_CHUNK)
                {
                    fprintf(stderr, "Chunk Test. GRgetchunkinfo returned wrong values\n");
                    fprintf(stderr, "cdims[0]=%d,cdims[1]=%d \n",(int)cdims[0],(int)cdims[1]);
                    fprintf(stderr, "rcdims[0]=%d,rcdims[1]=%d \n",(int)rcdims[0],(int)rcdims[1]);
                    fprintf(stderr, "cflags =%d \n", (int)cflags );
                }

          }

        /* Close the empty image */
        ret=GRendaccess(riid);
        CHECK(ret,FAIL,"GRendaccess");
    }
    
    /* Shut down the GR interface */
    ret=GRend(grid);
    CHECK(ret,FAIL,"GRend");

    /* Close the file */
    ret=Hclose(fid);
    CHECK(ret,FAIL,"Hclose");

}

static void test_mgr_image_b2a1cc2(int flag)
{
    int32 fid;              /* HDF file ID */
    int32 grid;             /* GRID for the interface */
    int32 ret;              /* generic return value */
    int32   cdims[2] = {1,1};    /* chunk dims */
    int32   *rcdims;             /* for SDgetchunkinfo() */
    HDF_CHUNK_DEF chunk_def;     /* Chunk defintion set */ 
    HDF_CHUNK_DEF rchunk_def;    /* Chunk defintion read */ 
    int32     cflags;            /* chunk flags */

    MESSAGE(8, printf("Check out I/O on new image with real data, with Default fill-value, Reading Sub-sampled Image\n"););
    /* Open up the existing datafile and get the image information from it */
    if (flag)
        fid=Hopen(TESTFILE2,DFACC_RDWR,0);
    else
        fid=Hopen(TESTFILE,DFACC_RDWR,0);
    CHECK(fid,FAIL,"Hopen");

    /* Initialize the GR interface */
    grid=GRstart(fid);
    CHECK(grid,FAIL,"GRstart");

    {
#ifdef TEST_XDIM
#undef TEST_XDIM
#endif /* TEST_XDIM */
#define TEST_XDIM    19
#ifdef TEST_YDIM
#undef TEST_YDIM
#endif /* TEST_YDIM */
#define TEST_YDIM    23
#ifdef TEST_NCOMP
#undef TEST_NCOMP
#endif /* TEST_NCOMP */
#define TEST_NCOMP   4
#ifdef TEST_VARTYPE
#undef TEST_VARTYPE
#endif /* TEST_VARTYPE */
#define TEST_VARTYPE uint32
#ifdef TEST_NT
#undef TEST_NT
#endif /* TEST_NT */
#define TEST_NT      DFNT_UINT32

        int32 riid;     /* RI ID for the new image */
        int32 dims[2]={TEST_XDIM,TEST_YDIM};    /* dimensions for the empty image */
        uint16 ref;     /* RI ref #. */
        int32 index;    /* RI index # */
        TEST_VARTYPE image[TEST_YDIM][TEST_XDIM][TEST_NCOMP]; /* space for the image data */
        TEST_VARTYPE fill_pixel[TEST_NCOMP]={4000000,4800,3,1000};   /* pixel with fill values */
        TEST_VARTYPE fill_pixel2[TEST_NCOMP]={1230,1,65000,350000};   /* pixel with fill values */
        TEST_VARTYPE image0[TEST_YDIM][TEST_XDIM][TEST_NCOMP]; /* space for the image data */
        TEST_VARTYPE sub_image[TEST_YDIM][TEST_XDIM][TEST_NCOMP]; /* space for the image data */
        TEST_VARTYPE *sub_ptr;
        int32 start[2]; /* start of image data to use */
        int32 stride[2];/* stride of image data to use */
        int32 count[2]; /* # of pixels of image data to use */
        intn i,j;       /* local counting variables */

        /* fill the memory-only with the default pixel fill-values */
        HDmemset(image0,0,sizeof(image0));
        sub_ptr=(TEST_VARTYPE *)sub_image;
        for(i=0; i<TEST_YDIM; i++)
          {
              for(j=0; j<TEST_XDIM; j++)
                {
                    if((j%2)==0)
                        HDmemcpy(&image0[i][j][0],fill_pixel,sizeof(fill_pixel));
                    else
                        HDmemcpy(&image0[i][j][0],fill_pixel2,sizeof(fill_pixel2));
                    if((i%2) && (j%2))
                      {
                          HDmemcpy(sub_ptr,&image0[i][j][0],TEST_NCOMP*sizeof(TEST_VARTYPE));
                          sub_ptr+=TEST_NCOMP;
                      } /* end if */
                } /* end for */
          } /* end for */

        /* initialize the disk buffer */
        HDmemset(image,255,sizeof(image));

        /* Create empty image with default fill value */
        riid=GRcreate(grid,"Test Image B2a1cc2",TEST_NCOMP,TEST_NT,MFGR_INTERLACE_PIXEL,dims);
        CHECK(riid,FAIL,"GRcreate");

        /* Check if creating chunked GR */
        if (flag)
          {
              /* Create chunked GR 
                 chunk is 2x2 which will create 6 chunks */
              cdims[0] = chunk_def.chunk_lengths[0] = 2;
              cdims[1] = chunk_def.chunk_lengths[1] = 2;
              ret = GRsetchunk(riid, chunk_def, HDF_CHUNK);
              CHECK(ret,FAIL,"GRsetchunk");
          }

        /* Save the ref. # for later access */
        ref=GRidtoref(riid);
        CHECK(ref,(uint16)FAIL,"GRidtoref");

        /* Create whole image */
        start[XDIM]=start[YDIM]=0;
        stride[XDIM]=stride[YDIM]=1;
        ret=GRwriteimage(riid,start,stride,dims,image0);
        CHECK(ret,FAIL,"GRwriteimage");

        /* Close the empty image */
        ret=GRendaccess(riid);
        CHECK(ret,FAIL,"GRendaccess");

        /* Get the index of the newly created image */
        index=GRreftoindex(grid,ref);
        CHECK(index,FAIL,"GRreftoindex");

        /* Select the newly created image */
        riid=GRselect(grid,index);
        CHECK(riid,FAIL,"GRselect");

        /* Get the sub-sample image back */
        start[XDIM]=1;
        start[YDIM]=1;
        count[XDIM]=TEST_XDIM/2;
        count[YDIM]=TEST_YDIM/2;
        stride[XDIM]=stride[YDIM]=2;
        ret=GRreadimage(riid,start,stride,count,image);
        CHECK(ret,FAIL,"GRreadimage");

        if(0!=HDmemcmp(image,sub_image,(size_t)(count[XDIM]*count[YDIM])*sizeof(fill_pixel)))
          {
              MESSAGE(3, printf("%d:Error reading data for new image with default fill-value, sub-sampled image\n",__LINE__););
              num_errs++;
          } /* end if */

        /* check if we are doing chunked tests */
        if (flag)
          {
              /* Get chunk lengths */
              ret = GRgetchunkinfo(riid, &rchunk_def, &cflags);
              CHECK(ret,FAIL,"GRgetchunkinfo");

              rcdims = rchunk_def.chunk_lengths;

              /* check chunk lengths and to see if GR is chunked */
              if (cdims[0] != rcdims[0] || cdims[1] != rcdims[1] || cflags != HDF_CHUNK)
                {
                    fprintf(stderr, "Chunk Test. GRgetchunkinfo returned wrong values\n");
                    fprintf(stderr, "cdims[0]=%d,cdims[1]=%d \n",(int)cdims[0],(int)cdims[1]);
                    fprintf(stderr, "rcdims[0]=%d,rcdims[1]=%d \n",(int)rcdims[0],(int)rcdims[1]);
                    fprintf(stderr, "cflags =%d \n", (int)cflags );
                }

          }

        /* Close the empty image */
        ret=GRendaccess(riid);
        CHECK(ret,FAIL,"GRendaccess");
    }
    
    /* Shut down the GR interface */
    ret=GRend(grid);
    CHECK(ret,FAIL,"GRend");

    /* Close the file */
    ret=Hclose(fid);
    CHECK(ret,FAIL,"Hclose");

} /* end test_mgr_image_b2a1cc() */

static void test_mgr_image_b2a2aa(int flag)
{
#ifdef QAK
    int32 fid;              /* HDF file ID */
    int32 grid;             /* GRID for the interface */
    int32 ret;              /* generic return value */

/* B2a2aa - Read/Write images - with real Data - New Image - with User-Defined Fill Value - Whole Image */
/* The following test is unnecessary, fill-values only are important when writing out partial images */
    MESSAGE(8, printf("Check out I/O on new image with real data, with User-Defined fill-value, Whole Image\n"););

    /* Open up the existing datafile and get the image information from it */
    fid=Hopen(TESTFILE,DFACC_RDWR,0);
    CHECK(fid,FAIL,"Hopen");

    /* Initialize the GR interface */
    grid=GRstart(fid);
    CHECK(grid,FAIL,"GRstart");

    {
#ifdef TEST_XDIM
#undef TEST_XDIM
#endif /* TEST_XDIM */
#define TEST_XDIM    3
#ifdef TEST_YDIM
#undef TEST_YDIM
#endif /* TEST_YDIM */
#define TEST_YDIM    8
#ifdef TEST_NCOMP
#undef TEST_NCOMP
#endif /* TEST_NCOMP */
#define TEST_NCOMP   2
#ifdef TEST_VARTYPE
#undef TEST_VARTYPE
#endif /* TEST_VARTYPE */
#define TEST_VARTYPE int32
#ifdef TEST_NT
#undef TEST_NT
#endif /* TEST_NT */
#define TEST_NT      DFNT_INT32

        int32 riid;     /* RI ID for the new image */
        int32 dims[2]={TEST_XDIM,TEST_YDIM};    /* dimensions for the empty image */
        uint16 ref;     /* RI ref #. */
        int32 index;    /* RI index # */
        TEST_VARTYPE image[TEST_YDIM][TEST_XDIM][TEST_NCOMP]; /* space for the image data */
        TEST_VARTYPE fill_pixel[TEST_NCOMP]={1,-2};   /* pixel with fill values */
        TEST_VARTYPE fill_pixel2[TEST_NCOMP]={-2,1};   /* pixel with fill values */
        TEST_VARTYPE image0[TEST_YDIM][TEST_XDIM][TEST_NCOMP]; /* space for the image data */
        int32 start[2]; /* start of image data to grab */
        int32 stride[2];/* stride of image data to grab */
        intn i,j,k;     /* local counting variables */

        /* fill the memory-only with the default pixel fill-value */
        for(i=0; i<TEST_YDIM; i++)
          {
              for(j=0; j<TEST_XDIM; j++)
                {
                    if((j%2)==0)
                        HDmemcpy(&image0[i][j][0],fill_pixel,sizeof(fill_pixel));
                    else
                        HDmemcpy(&image0[i][j][0],fill_pixel2,sizeof(fill_pixel2));
                } /* end for */
          } /* end for */
        HDmemcpy(image,image0,sizeof(image0));

        /* Create empty image with default fill value */
        riid=GRcreate(grid,"Test Image B2a1aa",TEST_NCOMP,TEST_NT,MFGR_INTERLACE_PIXEL,dims);
        CHECK(riid,FAIL,"GRcreate");

        /* Save the ref. # for later access */
        ref=GRidtoref(riid);
        CHECK(ref,(uint16)FAIL,"GRidtoref");

        start[0]=start[1]=0;
        stride[0]=stride[1]=1;
        ret=GRwriteimage(riid,start,stride,dims,image);
        CHECK(ret,FAIL,"GRwriteimage");

        /* Close the empty image */
        ret=GRendaccess(riid);
        CHECK(ret,FAIL,"GRendaccess");

        /* Get the index of the newly created image */
        index=GRreftoindex(grid,ref);
        CHECK(index,FAIL,"GRreftoindex");

        /* Select the newly created image */
        riid=GRselect(grid,index);
        CHECK(riid,FAIL,"GRselect");

        start[0]=start[1]=0;
        stride[0]=stride[1]=1;
        ret=GRreadimage(riid,start,stride,dims,image);
        CHECK(ret,FAIL,"GRreadimage");

        if(0!=HDmemcmp(image,image0,sizeof(image0)))
          {
              MESSAGE(3, printf("%d:Error reading data for new image with default fill-value, whole image\n",__LINE__););
              num_errs++;
          } /* end if */

        /* Close the empty image */
        ret=GRendaccess(riid);
        CHECK(ret,FAIL,"GRendaccess");
    }
    
    /* Shut down the GR interface */
    ret=GRend(grid);
    CHECK(ret,FAIL,"GRend");

    /* Close the file */
    ret=Hclose(fid);
    CHECK(ret,FAIL,"Hclose");
#endif /* QAK */

} /* end test_mgr_image_b2a2aa() */

static void test_mgr_image_b2a2bb(int flag)
{
    int32 fid;              /* HDF file ID */
    int32 grid;             /* GRID for the interface */
    int32 ret;              /* generic return value */
    int32   cdims[2] = {1,1};    /* chunk dims */
    int32   *rcdims;             /* for SDgetchunkinfo() */
    HDF_CHUNK_DEF chunk_def;     /* Chunk defintion set */ 
    HDF_CHUNK_DEF rchunk_def;    /* Chunk defintion read */ 
    int32     cflags;            /* chunk flags */

/* B2a2bb - Read/Write images - with real Data - New Image - with User-Defined Fill Value - Sub-setted Image */
    MESSAGE(8, printf("Check out I/O on new image with real data, with User-Defined fill-value, Writing Sub-setted Image\n"););

    /* Open up the existing datafile and get the image information from it */
    if (flag)
        fid=Hopen(TESTFILE2,DFACC_RDWR,0);
    else
        fid=Hopen(TESTFILE,DFACC_RDWR,0);
    CHECK(fid,FAIL,"Hopen");

    /* Initialize the GR interface */
    grid=GRstart(fid);
    CHECK(grid,FAIL,"GRstart");

    {
#ifdef TEST_XDIM
#undef TEST_XDIM
#endif /* TEST_XDIM */
#define TEST_XDIM    19
#ifdef TEST_YDIM
#undef TEST_YDIM
#endif /* TEST_YDIM */
#define TEST_YDIM    23
#ifdef TEST_NCOMP
#undef TEST_NCOMP
#endif /* TEST_NCOMP */
#define TEST_NCOMP   4
#ifdef TEST_VARTYPE
#undef TEST_VARTYPE
#endif /* TEST_VARTYPE */
#define TEST_VARTYPE float32
#ifdef TEST_NT
#undef TEST_NT
#endif /* TEST_NT */
#define TEST_NT      DFNT_FLOAT32

        int32 riid;     /* RI ID for the new image */
        int32 dims[2]={TEST_XDIM,TEST_YDIM};    /* dimensions for the empty image */
        uint16 ref;     /* RI ref #. */
        int32 index;    /* RI index # */
        TEST_VARTYPE image[TEST_YDIM][TEST_XDIM][TEST_NCOMP]; /* space for the image data */
        TEST_VARTYPE fill_pixel[TEST_NCOMP]={(TEST_VARTYPE)-3.75,(TEST_VARTYPE)4.5,(TEST_VARTYPE)-0.375,(TEST_VARTYPE)100.125};   /* pixel with fill values */
        TEST_VARTYPE pixel[TEST_NCOMP]={(TEST_VARTYPE)-20.00,(TEST_VARTYPE)4.875,(TEST_VARTYPE)0.125,(TEST_VARTYPE)1.0};   /* pixel with fill values */
        TEST_VARTYPE pixel2[TEST_NCOMP]={(TEST_VARTYPE)1.25,(TEST_VARTYPE)1.0,(TEST_VARTYPE)-6500.0,(TEST_VARTYPE)350.0};   /* pixel with fill values */
        TEST_VARTYPE image0[TEST_YDIM][TEST_XDIM][TEST_NCOMP]; /* space for the image data */
        TEST_VARTYPE sub_image[TEST_YDIM][TEST_XDIM][TEST_NCOMP]; /* space for the image data */
        TEST_VARTYPE *sub_ptr;
        int32 start[2]; /* start of image data to use */
        int32 stride[2];/* stride of image data to use */
        int32 count[2]; /* # of pixels of image data to use */
        intn i,j,k;     /* local counting variables */

        /* fill the memory-only with the default pixel fill-values */
        HDmemfill(image0,fill_pixel,sizeof(fill_pixel),sizeof(image0)/sizeof(fill_pixel));
        sub_ptr=(TEST_VARTYPE *)sub_image;
        for(i=0; i<TEST_YDIM; i++)
          {
              for(j=0; j<TEST_XDIM; j++)
                {
                    if(((i>(TEST_YDIM/3)) && (i<(2*TEST_YDIM/3)))
                        && ((j>(TEST_XDIM/4)) && (j<(3*TEST_XDIM/4))))
                      {
                          if((j%2)==0)
                              HDmemcpy(&image0[i][j][0],pixel,sizeof(pixel));
                          else
                              HDmemcpy(&image0[i][j][0],pixel2,sizeof(pixel2));
                          HDmemcpy(sub_ptr,&image0[i][j][0],TEST_NCOMP*sizeof(TEST_VARTYPE));
                          sub_ptr+=TEST_NCOMP;
                      } /* end if */
                } /* end for */
          } /* end for */

        /* initialize the disk buffer */
        HDmemset(image,255,sizeof(image));

        /* Create empty image with default fill value */
        riid=GRcreate(grid,"Test Image B2a2bb",TEST_NCOMP,TEST_NT,MFGR_INTERLACE_PIXEL,dims);
        CHECK(riid,FAIL,"GRcreate");

        /* Set the fill-value */
        ret=GRsetattr(riid,FILL_ATTR,TEST_NT,TEST_NCOMP,fill_pixel);
        CHECK(ret,FAIL,"GRsetattr");

        /* Check if creating chunked GR */
        if (flag)
          {
              /* Create chunked GR 
                 chunk is 2x2 which will create 6 chunks */
              cdims[0] = chunk_def.chunk_lengths[0] = 2;
              cdims[1] = chunk_def.chunk_lengths[1] = 2;
              ret = GRsetchunk(riid, chunk_def, HDF_CHUNK);
              CHECK(ret,FAIL,"GRsetchunk");
          }

        /* Save the ref. # for later access */
        ref=GRidtoref(riid);
        CHECK(ref,(uint16)FAIL,"GRidtoref");


        /* Create sub-setted window with only the filled pixels in it */
        start[XDIM]=(TEST_XDIM/4)+1;
        start[YDIM]=(TEST_YDIM/3)+1;
        count[XDIM]=((3*TEST_XDIM/4)-(TEST_XDIM/4))-1;
        count[YDIM]=((2*TEST_YDIM/3)-(TEST_YDIM/3))-1;
        stride[XDIM]=stride[YDIM]=1;
        ret=GRwriteimage(riid,start,stride,count,sub_image);
        CHECK(ret,FAIL,"GRwriteimage");

        /* Close the empty image */
        ret=GRendaccess(riid);
        CHECK(ret,FAIL,"GRendaccess");

        /* Get the index of the newly created image */
        index=GRreftoindex(grid,ref);
        CHECK(index,FAIL,"GRreftoindex");

        /* Select the newly created image */
        riid=GRselect(grid,index);
        CHECK(riid,FAIL,"GRselect");

        /* Get the whole image back */
        start[XDIM]=start[YDIM]=0;
        stride[XDIM]=stride[YDIM]=1;
        ret=GRreadimage(riid,start,stride,dims,image);
        CHECK(ret,FAIL,"GRreadimage");

        if(0!=HDmemcmp(image,image0,sizeof(image0)))
          {
              MESSAGE(3, printf("Error reading data for new image with user-defined fill-value, sub-setted image\n"););

              MESSAGE(8, for(i=0; i<TEST_YDIM; i++) \
                      for(j=0; j<TEST_XDIM; j++) \
                          for(k=0; k<TEST_NCOMP; k++) \
                              if(image[i][j][k]!=image0[i][j][k]) \
                                  printf("Location: [%d][%d][%d] image=%f, image0=%f \n",i,j,k,(double)image[i][j][k],(double)image0[i][j][k]); );
              num_errs++;
          } /* end if */

        /* check if we are doing chunked tests */
        if (flag)
          {
              /* Get chunk lengths */
              ret = GRgetchunkinfo(riid, &rchunk_def, &cflags);
              CHECK(ret,FAIL,"GRgetchunkinfo");

              rcdims = rchunk_def.chunk_lengths;

              /* check chunk lengths and to see if GR is chunked */
              if (cdims[0] != rcdims[0] || cdims[1] != rcdims[1] || cflags != HDF_CHUNK)
                {
                    fprintf(stderr, "Chunk Test. GRgetchunkinfo returned wrong values\n");
                    fprintf(stderr, "cdims[0]=%d,cdims[1]=%d \n",(int)cdims[0],(int)cdims[1]);
                    fprintf(stderr, "rcdims[0]=%d,rcdims[1]=%d \n",(int)rcdims[0],(int)rcdims[1]);
                    fprintf(stderr, "cflags =%d \n", (int)cflags );
                }

          }

        /* Close the empty image */
        ret=GRendaccess(riid);
        CHECK(ret,FAIL,"GRendaccess");
    }
    
    /* Shut down the GR interface */
    ret=GRend(grid);
    CHECK(ret,FAIL,"GRend");

    /* Close the file */
    ret=Hclose(fid);
    CHECK(ret,FAIL,"Hclose");

/* The following test is unnecessary, fill-values only make a difference when writing out data -QAK */
#ifdef QAK
    MESSAGE(8, printf("Check out I/O on new image with real data, with User-Defined fill-value, Reading Sub-setted Image\n"););
    /* Open up the existing datafile and get the image information from it */
    fid=Hopen(TESTFILE,DFACC_RDWR,0);
    CHECK(fid,FAIL,"Hopen");

    /* Initialize the GR interface */
    grid=GRstart(fid);
    CHECK(grid,FAIL,"GRstart");

    {
#ifdef TEST_XDIM
#undef TEST_XDIM
#endif /* TEST_XDIM */
#define TEST_XDIM    19
#ifdef TEST_YDIM
#undef TEST_YDIM
#endif /* TEST_YDIM */
#define TEST_YDIM    23
#ifdef TEST_NCOMP
#undef TEST_NCOMP
#endif /* TEST_NCOMP */
#define TEST_NCOMP   4
#ifdef TEST_VARTYPE
#undef TEST_VARTYPE
#endif /* TEST_VARTYPE */
#define TEST_VARTYPE uint16
#ifdef TEST_NT
#undef TEST_NT
#endif /* TEST_NT */
#define TEST_NT      DFNT_UINT16

        int32 riid;     /* RI ID for the new image */
        int32 dims[2]={TEST_XDIM,TEST_YDIM};    /* dimensions for the empty image */
        uint16 ref;     /* RI ref #. */
        int32 index;    /* RI index # */
        TEST_VARTYPE image[TEST_YDIM][TEST_XDIM][TEST_NCOMP]; /* space for the image data */
        TEST_VARTYPE fill_pixel[TEST_NCOMP]={-3.4,4.5,-0.03,100.4};   /* pixel with fill values */
        TEST_VARTYPE pixel[TEST_NCOMP]={-20.00,4.8,0.3,1.0};   /* pixel with fill values */
        TEST_VARTYPE pixel2[TEST_NCOMP]={1.23,1.0,-6500.0,350.0};   /* pixel with fill values */
        TEST_VARTYPE image0[TEST_YDIM][TEST_XDIM][TEST_NCOMP]; /* space for the image data */
        TEST_VARTYPE sub_image[TEST_YDIM][TEST_XDIM][TEST_NCOMP]; /* space for the image data */
        TEST_VARTYPE *sub_ptr;
        int32 start[2]; /* start of image data to use */
        int32 stride[2];/* stride of image data to use */
        int32 count[2]; /* # of pixels of image data to use */
        intn i,j,k;     /* local counting variables */

        /* fill the memory-only with the default pixel fill-values */
        HDmemset(image0,0,sizeof(image0));
        sub_ptr=(TEST_VARTYPE *)sub_image;
        for(i=0; i<TEST_YDIM; i++)
          {
              for(j=0; j<TEST_XDIM; j++)
                {
                    if((j%2)==0)
                        HDmemcpy(&image0[i][j][0],pixel,sizeof(pixel));
                    else
                        HDmemcpy(&image0[i][j][0],pixel2,sizeof(pixel2));
                    if(((i>(TEST_YDIM/3)) && (i<(2*TEST_YDIM/3)))
                        && ((j>(TEST_XDIM/4)) && (j<(3*TEST_XDIM/4))))
                      {
                          HDmemcpy(sub_ptr,&image0[i][j][0],TEST_NCOMP*sizeof(TEST_VARTYPE));
                          sub_ptr+=TEST_NCOMP;
                      } /* end if */
                } /* end for */
          } /* end for */

        /* initialize the disk buffer */
        HDmemset(image,255,sizeof(image));

        /* Create empty image with default fill value */
        riid=GRcreate(grid,"Test Image B2a2bb2",TEST_NCOMP,TEST_NT,MFGR_INTERLACE_PIXEL,dims);
        CHECK(riid,FAIL,"GRcreate");

        /* Save the ref. # for later access */
        ref=GRidtoref(riid);
        CHECK(ref,(uint16)FAIL,"GRidtoref");

        /* Set the fill-value */
        ret=GRsetattr(riid,FILL_ATTR,TEST_NT,TEST_NCOMP,fill_pixel);
        CHECK(ret,FAIL,"GRsetattr");

        /* Create whole image */
        start[XDIM]=start[YDIM]=0;
        stride[XDIM]=stride[YDIM]=1;
        ret=GRwriteimage(riid,start,stride,dims,image0);
        CHECK(ret,FAIL,"GRwriteimage");

        /* Close the empty image */
        ret=GRendaccess(riid);
        CHECK(ret,FAIL,"GRendaccess");

        /* Get the index of the newly created image */
        index=GRreftoindex(grid,ref);
        CHECK(index,FAIL,"GRreftoindex");

        /* Select the newly created image */
        riid=GRselect(grid,index);
        CHECK(riid,FAIL,"GRselect");

        /* Get the sub-set image back */
        start[XDIM]=(TEST_XDIM/4)+1;
        start[YDIM]=(TEST_YDIM/3)+1;
        count[XDIM]=((3*TEST_XDIM/4)-(TEST_XDIM/4))-1;
        count[YDIM]=((2*TEST_YDIM/3)-(TEST_YDIM/3))-1;
        stride[XDIM]=stride[YDIM]=1;
        ret=GRreadimage(riid,start,stride,count,image);
        CHECK(ret,FAIL,"GRreadimage");

        if(0!=HDmemcmp(image,sub_image,count[XDIM]*count[YDIM]*sizeof(pixel)))
          {
              MESSAGE(3, printf("%d:Error reading data for new image with default fill-value, sub-setted image\n",__LINE__););
              num_errs++;
          } /* end if */

        /* Close the empty image */
        ret=GRendaccess(riid);
        CHECK(ret,FAIL,"GRendaccess");
    }
    
    /* Shut down the GR interface */
    ret=GRend(grid);
    CHECK(ret,FAIL,"GRend");

    /* Close the file */
    ret=Hclose(fid);
    CHECK(ret,FAIL,"Hclose");
#endif /* QAK */

} /* end test_mgr_image_b2a2bb() */

static void test_mgr_image_b2a2cc(int flag)
{
    int32 fid;              /* HDF file ID */
    int32 grid;             /* GRID for the interface */
    int32 ret;              /* generic return value */
    int32   cdims[2] = {1,1};    /* chunk dims */
    int32   *rcdims;             /* for SDgetchunkinfo() */
    HDF_CHUNK_DEF chunk_def;     /* Chunk defintion set */ 
    HDF_CHUNK_DEF rchunk_def;    /* Chunk defintion read */ 
    int32     cflags;            /* chunk flags */

/* B2a2cc - Read/Write images - with real Data - New Image - with User-Defined Fill Value - Sub-sampled Image */
    MESSAGE(8, printf("Check out I/O on new image with real data, with User-Defined fill-value, Writing Sub-sampled Image\n"););

    /* Open up the existing datafile and get the image information from it */
    if (flag)
        fid=Hopen(TESTFILE2,DFACC_RDWR,0);
    else
        fid=Hopen(TESTFILE,DFACC_RDWR,0);
    CHECK(fid,FAIL,"Hopen");

    /* Initialize the GR interface */
    grid=GRstart(fid);
    CHECK(grid,FAIL,"GRstart");

    {
#ifdef TEST_XDIM
#undef TEST_XDIM
#endif /* TEST_XDIM */
#define TEST_XDIM    19
#ifdef TEST_YDIM
#undef TEST_YDIM
#endif /* TEST_YDIM */
#define TEST_YDIM    23
#ifdef TEST_NCOMP
#undef TEST_NCOMP
#endif /* TEST_NCOMP */
#define TEST_NCOMP   5
#ifdef TEST_VARTYPE
#undef TEST_VARTYPE
#endif /* TEST_VARTYPE */
#define TEST_VARTYPE int16
#ifdef TEST_NT
#undef TEST_NT
#endif /* TEST_NT */
#define TEST_NT      DFNT_INT16

        int32 riid;     /* RI ID for the new image */
        int32 dims[2]={TEST_XDIM,TEST_YDIM};    /* dimensions for the empty image */
        uint16 ref;     /* RI ref #. */
        int32 index;    /* RI index # */
        TEST_VARTYPE image[TEST_YDIM][TEST_XDIM][TEST_NCOMP]; /* space for the image data */
        TEST_VARTYPE fill_pixel[TEST_NCOMP]={-3,4,-13,100,1200};   /* pixel with fill values */
        TEST_VARTYPE pixel[TEST_NCOMP]={-20,4,0,1,-367};   /* pixel with fill values */
        TEST_VARTYPE pixel2[TEST_NCOMP]={1,-11,-6500,350,20};   /* pixel with fill values */
        TEST_VARTYPE image0[TEST_YDIM][TEST_XDIM][TEST_NCOMP]; /* space for the image data */
        TEST_VARTYPE sub_image[TEST_YDIM][TEST_XDIM][TEST_NCOMP]; /* space for the image data */
        TEST_VARTYPE *sub_ptr;
        int32 start[2]; /* start of image data to use */
        int32 stride[2];/* stride of image data to use */
        int32 count[2]; /* # of pixels of image data to use */
        intn i,j,k;     /* local counting variables */

        /* fill the memory-only with the default pixel fill-values */
        HDmemfill(image0,fill_pixel,sizeof(fill_pixel),sizeof(image0)/sizeof(fill_pixel));
        sub_ptr=(TEST_VARTYPE *)sub_image;
        for(i=0; i<TEST_YDIM; i++)
          {
              for(j=0; j<TEST_XDIM; j++)
                {
                    if((i%2)!=0 && (j%2)!=0)
                      {
                          if((j%3)==0)
                              HDmemcpy(&image0[i][j][0],pixel,sizeof(TEST_VARTYPE)*TEST_NCOMP);
                          else
                              HDmemcpy(&image0[i][j][0],pixel2,sizeof(TEST_VARTYPE)*TEST_NCOMP);
                          HDmemcpy(sub_ptr,&image0[i][j][0],TEST_NCOMP*sizeof(TEST_VARTYPE));
                          sub_ptr+=TEST_NCOMP;
                      } /* end if */
                } /* end for */
          } /* end for */

        /* initialize the disk buffer */
        HDmemset(image,255,sizeof(TEST_VARTYPE)*(size_t)(TEST_YDIM*TEST_XDIM*TEST_NCOMP));

        /* Create empty image with default fill value */
        riid=GRcreate(grid,"Test Image B2a2cc",TEST_NCOMP,TEST_NT,MFGR_INTERLACE_PIXEL,dims);
        CHECK(riid,FAIL,"GRcreate");

        /* Set the fill-value */
        ret=GRsetattr(riid,FILL_ATTR,TEST_NT,TEST_NCOMP,fill_pixel);
        CHECK(ret,FAIL,"GRsetattr");

        /* Check if creating chunked GR */
        if (flag)
          {
              /* Create chunked GR 
                 chunk is 2x2 which will create 6 chunks */
              cdims[0] = chunk_def.chunk_lengths[0] = 2;
              cdims[1] = chunk_def.chunk_lengths[1] = 2;
              ret = GRsetchunk(riid, chunk_def, HDF_CHUNK);
              CHECK(ret,FAIL,"GRsetchunk");
          }

        /* Save the ref. # for later access */
        ref=GRidtoref(riid);
        CHECK(ref,(uint16)FAIL,"GRidtoref");

        /* Create sub-sampled window with only the filled pixels in it */
        start[XDIM]=1;
        start[YDIM]=1;
        count[XDIM]=TEST_XDIM/2;
        count[YDIM]=TEST_YDIM/2;
        stride[XDIM]=stride[YDIM]=2;
        ret=GRwriteimage(riid,start,stride,count,sub_image);
        CHECK(ret,FAIL,"GRwriteimage");

        /* Close the empty image */
        ret=GRendaccess(riid);
        CHECK(ret,FAIL,"GRendaccess");

        /* Get the index of the newly created image */
        index=GRreftoindex(grid,ref);
        CHECK(index,FAIL,"GRreftoindex");

        /* Select the newly created image */
        riid=GRselect(grid,index);
        CHECK(riid,FAIL,"GRselect");

        /* Get the whole image back */
        start[XDIM]=start[YDIM]=0;
        stride[XDIM]=stride[YDIM]=1;
        ret=GRreadimage(riid,start,stride,dims,image);
        CHECK(ret,FAIL,"GRreadimage");

        if(0!=HDmemcmp(image,image0,sizeof(TEST_VARTYPE)*TEST_YDIM*TEST_XDIM*TEST_NCOMP))
          {
              MESSAGE(3, printf("Error reading data for new image with user-defined fill-value, sub-sampled image\n"););

              MESSAGE(8, for(i=0; i<TEST_YDIM; i++) \
                      for(j=0; j<TEST_XDIM; j++) \
                          for(k=0; k<TEST_NCOMP; k++) \
                            if(image[i][j][k]!=image0[i][j][k]) \
                                  printf("Location: [%d][%d][%d] image=%d, image0=%d \n",(int)i,(int)j,(int)k,(int)image[i][j][k],(int)image0[i][j][k]); );
              num_errs++;
          } /* end if */

        /* check if we are doing chunked tests */
        if (flag)
          {
              /* Get chunk lengths */
              ret = GRgetchunkinfo(riid, &rchunk_def, &cflags);
              CHECK(ret,FAIL,"GRgetchunkinfo");

              rcdims = rchunk_def.chunk_lengths;

              /* check chunk lengths and to see if GR is chunked */
              if (cdims[0] != rcdims[0] || cdims[1] != rcdims[1] || cflags != HDF_CHUNK)
                {
                    fprintf(stderr, "Chunk Test. GRgetchunkinfo returned wrong values\n");
                    fprintf(stderr, "cdims[0]=%d,cdims[1]=%d \n",(int)cdims[0],(int)cdims[1]);
                    fprintf(stderr, "rcdims[0]=%d,rcdims[1]=%d \n",(int)rcdims[0],(int)rcdims[1]);
                    fprintf(stderr, "cflags =%d \n", (int)cflags );
                }

          }

        /* Close the empty image */
        ret=GRendaccess(riid);
        CHECK(ret,FAIL,"GRendaccess");
    }
    
    /* Shut down the GR interface */
    ret=GRend(grid);
    CHECK(ret,FAIL,"GRend");

    /* Close the file */
    ret=Hclose(fid);
    CHECK(ret,FAIL,"Hclose");

/* The following test is unnecessary, fill-values only make a difference when writing out data -QAK */
#ifdef QAK
    MESSAGE(8, printf("Check out I/O on new image with real data, with Default fill-value, Reading Sub-sampled Image\n"););
    /* Open up the existing datafile and get the image information from it */
    fid=Hopen(TESTFILE,DFACC_RDWR,0);
    CHECK(fid,FAIL,"Hopen");

    /* Initialize the GR interface */
    grid=GRstart(fid);
    CHECK(grid,FAIL,"GRstart");

    {
#ifdef TEST_XDIM
#undef TEST_XDIM
#endif /* TEST_XDIM */
#define TEST_XDIM    19
#ifdef TEST_YDIM
#undef TEST_YDIM
#endif /* TEST_YDIM */
#define TEST_YDIM    23
#ifdef TEST_NCOMP
#undef TEST_NCOMP
#endif /* TEST_NCOMP */
#define TEST_NCOMP   4
#ifdef TEST_VARTYPE
#undef TEST_VARTYPE
#endif /* TEST_VARTYPE */
#define TEST_VARTYPE uint32
#ifdef TEST_NT
#undef TEST_NT
#endif /* TEST_NT */
#define TEST_NT      DFNT_UINT32

        int32 riid;     /* RI ID for the new image */
        int32 dims[2]={TEST_XDIM,TEST_YDIM};    /* dimensions for the empty image */
        uint16 ref;     /* RI ref #. */
        int32 index;    /* RI index # */
        TEST_VARTYPE image[TEST_YDIM][TEST_XDIM][TEST_NCOMP]; /* space for the image data */
        TEST_VARTYPE fill_pixel[TEST_NCOMP]={4000000,4800,3,1000};   /* pixel with fill values */
        TEST_VARTYPE fill_pixel2[TEST_NCOMP]={1230,1,65000,350000};   /* pixel with fill values */
        TEST_VARTYPE image0[TEST_YDIM][TEST_XDIM][TEST_NCOMP]; /* space for the image data */
        TEST_VARTYPE sub_image[TEST_YDIM][TEST_XDIM][TEST_NCOMP]; /* space for the image data */
        TEST_VARTYPE *sub_ptr;
        int32 start[2]; /* start of image data to use */
        int32 stride[2];/* stride of image data to use */
        int32 count[2]; /* # of pixels of image data to use */
        intn i,j,k;     /* local counting variables */

        /* fill the memory-only with the default pixel fill-values */
        HDmemset(image0,0,sizeof(image0));
        sub_ptr=(TEST_VARTYPE *)sub_image;
        for(i=0; i<TEST_YDIM; i++)
          {
              for(j=0; j<TEST_XDIM; j++)
                {
                    if((j%2)==0)
                        HDmemcpy(&image0[i][j][0],fill_pixel,sizeof(fill_pixel));
                    else
                        HDmemcpy(&image0[i][j][0],fill_pixel2,sizeof(fill_pixel2));
                    if((i%2) && (j%2))
                      {
                          HDmemcpy(sub_ptr,&image0[i][j][0],TEST_NCOMP*sizeof(TEST_VARTYPE));
                          sub_ptr+=TEST_NCOMP;
                      } /* end if */
                } /* end for */
          } /* end for */

        /* initialize the disk buffer */
        HDmemset(image,255,sizeof(image));

        /* Create empty image with default fill value */
        riid=GRcreate(grid,"Test Image B2a2cc2",TEST_NCOMP,TEST_NT,MFGR_INTERLACE_PIXEL,dims);
        CHECK(riid,FAIL,"GRcreate");

        /* Save the ref. # for later access */
        ref=GRidtoref(riid);
        CHECK(ref,(uint16)FAIL,"GRidtoref");

        /* Create whole image */
        start[XDIM]=start[YDIM]=0;
        stride[XDIM]=stride[YDIM]=1;
        ret=GRwriteimage(riid,start,stride,dims,image0);
        CHECK(ret,FAIL,"GRwriteimage");

        /* Close the empty image */
        ret=GRendaccess(riid);
        CHECK(ret,FAIL,"GRendaccess");

        /* Get the index of the newly created image */
        index=GRreftoindex(grid,ref);
        CHECK(index,FAIL,"GRreftoindex");

        /* Select the newly created image */
        riid=GRselect(grid,index);
        CHECK(riid,FAIL,"GRselect");

        /* Get the sub-sample image back */
        start[XDIM]=1;
        start[YDIM]=1;
        count[XDIM]=TEST_XDIM/2;
        count[YDIM]=TEST_YDIM/2;
        stride[XDIM]=stride[YDIM]=2;
        ret=GRreadimage(riid,start,stride,count,image);
        CHECK(ret,FAIL,"GRreadimage");

        if(0!=HDmemcmp(image,sub_image,count[XDIM]*count[YDIM]*sizeof(fill_pixel)))
          {
              MESSAGE(3, printf("%d:Error reading data for new image with default fill-value, sub-sampled image\n",__LINE__););
              num_errs++;
          } /* end if */

        /* Close the empty image */
        ret=GRendaccess(riid);
        CHECK(ret,FAIL,"GRendaccess");
    }
    
    /* Shut down the GR interface */
    ret=GRend(grid);
    CHECK(ret,FAIL,"GRend");

    /* Close the file */
    ret=Hclose(fid);
    CHECK(ret,FAIL,"Hclose");
#endif /* QAK */

} /* end test_mgr_image_b2a2cc() */

static void test_mgr_image_b2b1(int flag)
{
    int32 fid;              /* HDF file ID */
    int32 grid;             /* GRID for the interface */
    int32 ret;              /* generic return value */

/* B2b1 - Read/Write images - with real Data - Existing Image - Whole Image */
    MESSAGE(8, printf("Check out I/O from Existing Image - Whole Image\n"););

    /* Open up the existing datafile and get the image information from it */
    fid=Hopen(DATAFILE,DFACC_READ,0);
    CHECK(fid,FAIL,"Hopen");

    /* Try initializing the GR interface */
    grid=GRstart(fid);
    CHECK(grid,FAIL,"GRstart");

    {
        int32 n_datasets;       /* number of datasets */
        int32 n_attrs;          /* number of attributes */
        intn i;     /* local counting variables */
        
        ret=(intn)GRfileinfo(grid,&n_datasets,&n_attrs);
        CHECK(ret,FAIL,"GRfileinfo");

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
                    MESSAGE(3, printf("Error! Name for image %d is: %s, should be %s\n",i,name,datafile_info[i].name););
                    num_errs++;
                } /* end if */

              /* Check the # of components */
              if(ncomp!=datafile_info[i].ncomp)
                {
                    MESSAGE(3, printf("Error! Number of components for image %d is: %ld, should be %ld\n",i,(long)ncomp,(long)datafile_info[i].ncomp););
                    num_errs++;
                } /* end if */

              /* Check the NT of components */
              if(nt!=datafile_info[i].nt)
                {
                    MESSAGE(3, printf("Error! NT of components for image %d is: %ld, should be %ld\n",i,(long)nt,(long)datafile_info[i].nt););
                    num_errs++;
                } /* end if */

              /* Check the interlace of components */
              if(il!=datafile_info[i].il)
                {
                    MESSAGE(3, printf("Error! Interlace of components for image %d is: %ld, should be %ld\n",i,(long)il,(long)datafile_info[i].il););
                    num_errs++;
                } /* end if */

              /* Check the x-dimension of the image */
              if(dimsizes[XDIM]!=datafile_info[i].dimsizes[XDIM])
                {
                    MESSAGE(3, printf("Error! X-dim of image %d is: %ld, should be %ld\n",i,(long)dimsizes[XDIM],(long)datafile_info[i].dimsizes[XDIM]););
                    num_errs++;
                } /* end if */

              /* Check the y-dimension of the image */
              if(dimsizes[YDIM]!=datafile_info[i].dimsizes[YDIM])
                {
                    MESSAGE(3, printf("Error! Y-dim of image %d is: %ld, should be %ld\n",i,(long)dimsizes[YDIM],(long)datafile_info[i].dimsizes[YDIM]););
                    num_errs++;
                } /* end if */

              /* Check the # of attributes of the image */
              if(n_attr!=datafile_info[i].n_attr)
                {
                    MESSAGE(3, printf("Error! # of attributes for image %d is: %ld, should be %ld\n",i,(long)n_attr,(long)datafile_info[i].n_attr););
                    num_errs++;
                } /* end if */

              /* Check the image data itself */
              {
                  int32 start[2];
                  int32 stride[2];

                    img_data=HDmalloc((size_t)(dimsizes[0]*dimsizes[1]*ncomp*DFKNTsize(nt|DFNT_NATIVE)));
                    CHECK(img_data,NULL,"HDmalloc");

                    HDmemset(img_data,0,(size_t)(dimsizes[0]*dimsizes[1]*ncomp*DFKNTsize(nt|DFNT_NATIVE)));

                    start[0]=start[1]=0;
                    stride[0]=stride[1]=1;
                    ret=GRreadimage(riid,start,stride,dimsizes,img_data);
                    CHECK(ret,FAIL,"GRreadimage");

                    switch(i)
                      {
                          case 0:
                              if(0!=HDmemcmp(img_data,image00,sizeof(image00)))
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
      } /* end block */

    /* Shut down the GR interface */
    ret=GRend(grid);
    CHECK(ret,FAIL,"GRend");

    /* Close the file */
    ret=Hclose(fid);
    CHECK(ret,FAIL,"Hclose");

} /* end test_mgr_image_b2b1() */

static void test_mgr_image_b2b2(int flag)
{
/* B2b2 - Read/Write images - with real Data - Existing Image - Sub-setted Image */
    /* This test is unnecessary, I think this case has been adequately covered above -QAK */
} /* end test_mgr_image_b2b2() */

static void test_mgr_image_b2b3(int flag)
{
/* B2b3 - Read/Write images - with real Data - Existing Image - Sub-sampled Image */
    /* This test is unnecessary, I think this case has been adequately covered above -QAK */
} /* end test_mgr_image_b2b3() */

static void test_mgr_image_chunk(int flag)
{
    int32   cdims[2] = {1,1};    /* chunk dims */
    int32   *rcdims;             /* for SDgetchunkinfo() */
    HDF_CHUNK_DEF chunk_def;     /* Chunk defintion set */ 
    HDF_CHUNK_DEF rchunk_def;    /* Chunk defintion read */ 
    int32     cflags;            /* chunk flags */
    int32 fid;              /* HDF file ID */
    int32 grid;             /* GRID for the interface */
    int32 ret;              /* generic return value */

    /* Open up the existing datafile and get the image information from it */
    fid=Hopen(TESTFILE2,DFACC_RDWR,0);
    CHECK(fid,FAIL,"Hopen");

    /* Initialize the GR interface */
    grid=GRstart(fid);
    CHECK(grid,FAIL,"GRstart");

    {
#ifdef TEST_XDIM
#undef TEST_XDIM
#endif /* TEST_XDIM */
#define TEST_XDIM    3
#ifdef TEST_YDIM
#undef TEST_YDIM
#endif /* TEST_YDIM */
#define TEST_YDIM    8
#ifdef TEST_NCOMP
#undef TEST_NCOMP
#endif /* TEST_NCOMP */
#define TEST_NCOMP   2
#ifdef TEST_VARTYPE
#undef TEST_VARTYPE
#endif /* TEST_VARTYPE */
#define TEST_VARTYPE int32
#ifdef TEST_NT
#undef TEST_NT
#endif /* TEST_NT */
#define TEST_NT      DFNT_INT32
        int32 riid;     /* RI ID for the new image */
        int32 dims[2]={TEST_XDIM,TEST_YDIM};    /* dimensions for the empty image */
        uint16 ref;     /* RI ref #. */
        int32 index;    /* RI index # */
        TEST_VARTYPE image[TEST_YDIM][TEST_XDIM][TEST_NCOMP]; /* space for the image data */
        TEST_VARTYPE fill_pixel[TEST_NCOMP]={1,-2};   /* pixel with fill values */
        TEST_VARTYPE fill_pixel2[TEST_NCOMP]={-2,1};   /* pixel with fill values */
        TEST_VARTYPE image0[TEST_YDIM][TEST_XDIM][TEST_NCOMP]; /* space for the image data */
        int32 start[2]; /* start of image data to grab */
        int32 stride[2];/* stride of image data to grab */
        intn i,j;       /* local counting variables */


        /* fill the memory-only with the default pixel fill-value */
        for(i=0; i<TEST_YDIM; i++)
          {
              for(j=0; j<TEST_XDIM; j++)
                {
                    if((j%2)==0)
                        HDmemcpy(&image0[i][j][0],fill_pixel,sizeof(fill_pixel));
                    else
                        HDmemcpy(&image0[i][j][0],fill_pixel2,sizeof(fill_pixel2));
                } /* end for */
          } /* end for */
        HDmemcpy(image,image0,sizeof(image0));

        /* Create empty image with default fill value */
        riid=GRcreate(grid,"Test Chunk Image B2a1aa",TEST_NCOMP,TEST_NT,MFGR_INTERLACE_PIXEL,dims);
        CHECK(riid,FAIL,"GRcreate");

        /* Create chunked GR 
           chunk is 2x2 which will create 6 chunks */
        cdims[0] = chunk_def.chunk_lengths[0] = 2;
        cdims[1] = chunk_def.chunk_lengths[1] = 2;
        ret = GRsetchunk(riid, chunk_def, HDF_CHUNK);
        CHECK(ret,FAIL,"GRsetchunk");

        /* Set Chunk cache to hold 2 chunks */
        ret = GRsetchunkcache(riid, 2, 0);
        CHECK(ret,FAIL,"GRsetchunkcache");

        /* Save the ref. # for later access */
        ref=GRidtoref(riid);
        CHECK(ref,(uint16)FAIL,"GRidtoref");

        start[0]=start[1]=0;
        stride[0]=stride[1]=1;
        ret=GRwriteimage(riid,start,stride,dims,image);
        CHECK(ret,FAIL,"GRwriteimage");

        /* Close the empty image */
        ret=GRendaccess(riid);
        CHECK(ret,FAIL,"GRendaccess");

        /* Shut down the GR interface */
        ret=GRend(grid);
        CHECK(ret,FAIL,"GRend");

        /* Initialize the GR interface again */
        grid=GRstart(fid);
        CHECK(grid,FAIL,"GRstart");

#if 0
        /* Create empty image with default fill value */
        riid=GRcreate(grid,"Test Image B2a1aa",TEST_NCOMP,TEST_NT,MFGR_INTERLACE_PIXEL,dims);
        CHECK(riid,FAIL,"GRcreate");

#endif

        /* Get the index of the newly created image */
        index=GRreftoindex(grid,ref);
        CHECK(index,FAIL,"GRreftoindex");

        /* Select the newly created image */
        riid=GRselect(grid,index);
        CHECK(riid,FAIL,"GRselect");

        start[0]=start[1]=0;
        stride[0]=stride[1]=1;
        ret=GRreadimage(riid,start,stride,dims,image);
        CHECK(ret,FAIL,"GRreadimage");

        if(0!=HDmemcmp(image,image0,sizeof(image0)))
          {
              MESSAGE(3, printf("%d:Error reading data for new image with default fill-value, whole image\n",__LINE__););
              num_errs++;
          } /* end if */

        /* Get chunk lengths */
        ret = GRgetchunkinfo(riid, &rchunk_def, &cflags);
        CHECK(ret,FAIL,"GRgetchunkinfo");

        rcdims = rchunk_def.chunk_lengths;

        /* check chunk lengths and to see if GR is chunked */
        if (cdims[0] != rcdims[0] || cdims[1] != rcdims[1] || cflags != HDF_CHUNK)
          {
              fprintf(stderr, "Chunk Test 1. GRgetchunkinfo returned wrong values\n");
              fprintf(stderr, "cdims[0]=%d,cdims[1]=%d \n",(int)cdims[0],(int)cdims[1]);
              fprintf(stderr, "rcdims[0]=%d,rcdims[1]=%d \n",(int)rcdims[0],(int)rcdims[1]);
              fprintf(stderr, "cflags =%d \n", (int)cflags );
          }

        /* Close the empty image */
        ret=GRendaccess(riid);
        CHECK(ret,FAIL,"GRendaccess");
    }
    
    /* Shut down the GR interface */
    ret=GRend(grid);
    CHECK(ret,FAIL,"GRend");

    /* Close the file */
    ret=Hclose(fid);
    CHECK(ret,FAIL,"Hclose");


#if 0

    /* Create chunked GR 
       chunk is 3x2 which will create 6 chunks.
       Use GZIP compression */
    cdims[0] = chunk_def.comp.chunk_lengths[0] = 3;
    cdims[1] = chunk_def.comp.chunk_lengths[1] = 2;
#if 0
    chunk_def.comp.comp_type = COMP_CODE_RLE;   /* RLE */

    chunk_def.comp.comp_type = COMP_CODE_SKPHUFF; /* Skipping Huffman */
    chunk_def.comp.cinfo.skphuff.skp_size = sizeof(uint16);
#endif
    chunk_def.comp.comp_type = COMP_CODE_DEFLATE; /* GZIP */
    chunk_def.comp.cinfo.deflate.level = 6;

    status = GRsetchunk(riid8, chunk_def, HDF_CHUNK | HDF_COMP);
    if(status == FAIL) 
      {
        fprintf(stderr, "Error! Chunk Test 7. Failed to create new chunked, GZIP Compressed data set\n");
        num_err++;
        goto test8;
      }
#endif

} /* end test_mgr_image_chunk() */

/****************************************************************
**
**  test_mgr_image(): Multi-file Raster Image I/O Test Routine
** 
**      A. GRcreate/GRselect/GRendaccess w/GRgetiminfo
**      B. Write/Read images
**          1. With no Data
**              a. Default fill value
**              b. user defined fill value
**          2. With real Data
**              a. New Image
**                  1. With default fill value
**                      aa. Whole image
**                      bb. Sub-setted image
**                      cc. Sub-sampled image
**                  2. With user defined vill value
**                      aa. Whole image
**                      bb. Sub-setted image
**                      cc. Sub-sampled image
**              b. Existing Image
**                  1. Whole image
**                  2. Sub-setted image
**                  3. Sub-sampled image
** 
****************************************************************/
static void
test_mgr_image(int flag)
{
    /* Output message about test being performed */
    MESSAGE(6, printf("Testing Multi-file Raster Image I/O routines\n"););
    test_mgr_image_b1a(flag);
    test_mgr_image_b1b(flag);
    test_mgr_image_b2a1aa(flag);
    test_mgr_image_b2a1bb1(flag);
    test_mgr_image_b2a1bb2(flag);
    test_mgr_image_b2a1cc1(flag);
    test_mgr_image_b2a1cc2(flag);
    test_mgr_image_b2a2aa(flag);
    test_mgr_image_b2a2bb(flag);
    test_mgr_image_b2a2cc(flag);
    test_mgr_image_b2b1(flag);
    test_mgr_image_b2b2(flag);
    test_mgr_image_b2b3(flag);
#if 0
    test_mgr_image_chunk(flag);
#endif
}   /* end test_mgr_image() */

/****************************************************************
**
**  test_mgr_index(): Multi-file Raster ID/Ref/Index Test Routine
** 
**  III. ID/Ref/Index Functions
**      A. GRidtoref
**      B. GRreftoindex
** 
****************************************************************/
static void
test_mgr_index(int flag)
{
    /* output message about test being performed */
    MESSAGE(6, printf("Testing Multi-File Raster id/ref/index routines\n"););

/* I believe that these are adequately tested in the test_mgr_image routine -QAK */
}   /* end test_mgr_index() */

/****************************************************************
**
**  test_mgr_interlace(): Multi-file Raster Interlace Test Routine
** 
**  IV. Interlace Functions [Need to be implemented]
**      A. GRreqlutil - tested in the palette test below.
**      B. GRreqimageil
** 
****************************************************************/
static void
test_mgr_interlace(int flag)
{
    int32 fid;              /* hdf file id */
    int32 grid;             /* grid for the interface */
    int32 n_datasets;       /* number of datasets */
    int32 n_attrs;          /* number of attributes */
    int32 ret;              /* generic return value */
    VOIDP image;            /* image to retrieve */

    /* Output message about test being performed */
    MESSAGE(6, printf("Testing Multi-file Raster Interlace routines\n"););

    /* open up the existing datafile and get the image information from it */
    if (flag)
        fid=Hopen(TESTFILE2,DFACC_RDWR,0);
    else
        fid=Hopen(TESTFILE,DFACC_RDWR,0);
    CHECK(fid,FAIL,"Hopen");

    /* initialize the gr interface */
    grid=GRstart(fid);
    CHECK(grid,FAIL,"GRstart");

    {
        intn i,j;     /* local counting variables */
        
        ret=(intn)GRfileinfo(grid,&n_datasets,&n_attrs);
        CHECK(ret,FAIL,"GRfileinfo");

        for(i=0; i<n_datasets; i++)
          {
              int32 riid;               /* RI ID for an image */
              char name[MAX_IMG_NAME];  /* storage for the image's name */
              int32 ncomp;              /* number of components */
              int32 nt;                 /* NT of the components */
              int32 il;                 /* interlace of the image data */
              int32 start[2];
              int32 stride[2];
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

              image=HDmalloc((size_t)(dimsizes[XDIM]*dimsizes[YDIM]*ncomp*DFKNTsize(nt|DFNT_NATIVE)));
              CHECK(image,NULL,"HDmalloc");

              start[0]=start[1]=0;
              stride[0]=stride[1]=1;
              ret=GRreadimage(riid,start,stride,dimsizes,image);

              /* Check the image data itself */
              for(j=(intn)MFGR_INTERLACE_PIXEL; j<=(intn)MFGR_INTERLACE_COMPONENT; j++)
                {
                    VOIDP pixel_buf;

                    img_data=HDmalloc((size_t)(dimsizes[0]*dimsizes[1]*ncomp*DFKNTsize(nt|DFNT_NATIVE)));
                    CHECK(img_data,NULL,"HDmalloc");

                    pixel_buf=HDmalloc((size_t)(dimsizes[0]*dimsizes[1]*ncomp*DFKNTsize(nt|DFNT_NATIVE)));
                    CHECK(pixel_buf,NULL,"HDmalloc");

                    HDmemset(img_data,0,(size_t)(dimsizes[0]*dimsizes[1]*ncomp*DFKNTsize(nt|DFNT_NATIVE)));

                    ret=GRreqimageil(riid,j);
                    CHECK(ret,FAIL,"GRreqimageil");

                    start[0]=start[1]=0;
                    stride[0]=stride[1]=1;
                    ret=GRreadimage(riid,start,stride,dimsizes,img_data);
                    CHECK(ret,FAIL,"GRreadimage");

                    GRIil_convert(image,MFGR_INTERLACE_PIXEL,pixel_buf,(gr_interlace_t)j,dimsizes,ncomp,nt);
                    if(0!=HDmemcmp(img_data,pixel_buf,
                          (size_t)(dimsizes[XDIM]*dimsizes[YDIM]*ncomp*DFKNTsize(nt|DFNT_NATIVE))))
                      {
                          MESSAGE(3, printf("Error reading data for image %d, j=%d\n",i,j););
                          num_errs++;
                      } /* end if */
                    HDfree(img_data);
                    HDfree(pixel_buf);
                } /* end for */

              HDfree(image);

              /* End access to the image */
              ret=GRendaccess(riid);
              CHECK(ret,FAIL,"GRendaccess");
          } /* end for */
      } /* end block */
    
    /* shut down the gr interface */
    ret=GRend(grid);
    CHECK(ret,FAIL,"GRend");

    /* close the file */
    ret=Hclose(fid);
    CHECK(ret,FAIL,"Hclose");
}   /* end test_mgr_interlace() */

/****************************************************************
**
**  test_mgr_lut(): Multi-file Raster LUT/Palette Test Routine
** 
**  V. Palette Functions
**      A. GRgetlutid w/GRgetlutinfo
**      B. Read/Write Palettes
**          1. GRwritelut
**          2. GRreadlut
**	C. GRluttoref
** 
****************************************************************/
static void
test_mgr_lut(int flag)
{
    int32 fid;              /* hdf file id */
    int32 grid;             /* grid for the interface */
    int32 ret;              /* generic return value */

    /* Output message about test being performed */
    MESSAGE(6, printf("Testing Multi-file Raster Palette routines\n"););

    /* open up the existing datafile and get the image information from it */
    fid=Hopen(TESTFILE,DFACC_RDWR,0);
    CHECK(fid,FAIL,"Hopen");

    /* initialize the gr interface */
    grid=GRstart(fid);
    CHECK(grid,FAIL,"GRstart");

/* pick up here -QAK2 */
    {
        intn i,j;     /* local counting variables */
        int32 riid;               /* RI ID for an image */
        int32 lutid;              /* RI ID for an image */
        char name[MAX_IMG_NAME];  /* storage for the image's name */
        int32 ncomp;              /* number of components */
        int32 pal_ncomp;          /* number of palette components */
        int32 nt;                 /* NT of the components */
        int32 pal_nt;             /* NT of the palette components */
        int32 il;                 /* interlace of the image data */
        int32 pal_il;             /* interlace of the palette data */
        int32 dimsizes[2];        /* dimension sizes of the image */
        int32 pal_entries;        /* number of entries in the palette */
        int32 n_attr;             /* number of attributes with each image */
        uint8 *tmp_data;          /* temporary buffer pointer */
        VOIDP pal_data;           /* buffer for the palette data */
	uint16 pal_ref;		  /* reference number of the palette */

        /* Attach to the image */
        riid=GRselect(grid,0);
        CHECK(riid,FAIL,"GRselect");

        /* Get the Image information */
        *name='\0';
        ret=GRgetiminfo(riid,name,&ncomp,&nt,&il,dimsizes,&n_attr);
        CHECK(ret,FAIL,"GRgetiminfo");

        lutid=GRgetlutid(riid,0);
	CHECK(lutid,FAIL,"GRgetlutid");
        
        /* Get the Palette information */
        ret=GRgetlutinfo(lutid,&pal_ncomp,&pal_nt,&pal_il,&pal_entries);
        CHECK(ret,FAIL,"GRgetlutinfo");

        /* Check the palette values, they should all be "nil" values */
        if(pal_ncomp!=0)
          {
              MESSAGE(3, printf("Error! Incorrect palette components\n"););
              num_errs++;
          } /* end if */
        if(pal_nt!=DFNT_NONE)
          {
              MESSAGE(3, printf("Error! Incorrect palette number-type\n"););
              num_errs++;
          } /* end if */
        if(pal_il!=(-1))
          {
              MESSAGE(3, printf("Error! Incorrect palette interlace, pal_il=%d\n",(int)pal_il););
              num_errs++;
          } /* end if */
        if(pal_entries!=0)
          {
              MESSAGE(3, printf("Error! Incorrect palette # of entries\n"););
              num_errs++;
          } /* end if */

        /* Set the palette components */
        pal_ncomp=3;
        pal_nt=DFNT_UINT8;
        pal_il=(int32)MFGR_INTERLACE_PIXEL;
        pal_entries=256;

        pal_data=HDmalloc((size_t)(pal_entries*pal_ncomp*DFKNTsize(pal_nt|DFNT_NATIVE)));
        CHECK(pal_data,NULL,"HDmalloc");

        /* Initialize the palette data, in 'pixel' interlace */
        tmp_data=(uint8 *)pal_data;
        for(j=0; j<pal_entries; j++)
            for(i=0; i<pal_ncomp; i++)
                *tmp_data++=(uint8)(j*i);

        /* Write the palette out */
        ret=GRwritelut(lutid,pal_ncomp,pal_nt,pal_il,pal_entries,pal_data);
        CHECK(ret,FAIL,"GRwritelut");

        /* Check the image data itself */
        for(j=(intn)MFGR_INTERLACE_PIXEL; j<=(intn)MFGR_INTERLACE_COMPONENT; j++)
          {
              VOIDP pixel_buf;
              int32 dimsizes2[2];

              tmp_data=HDmalloc((size_t)(pal_entries*pal_ncomp*DFKNTsize(pal_nt|DFNT_NATIVE)));
              CHECK(tmp_data,NULL,"HDmalloc");

              pixel_buf=HDmalloc((size_t)(pal_entries*pal_ncomp*DFKNTsize(pal_nt|DFNT_NATIVE)));
              CHECK(pixel_buf,NULL,"HDmalloc");

              HDmemset(tmp_data,0,(size_t)(pal_entries*pal_ncomp*DFKNTsize(pal_nt|DFNT_NATIVE)));

              ret=GRreqlutil(lutid,j);
              CHECK(ret,FAIL,"GRreqlutil");

              ret=GRreadlut(lutid,tmp_data);

              dimsizes2[XDIM]=1;
              dimsizes2[YDIM]=pal_entries;
              GRIil_convert(pal_data,MFGR_INTERLACE_PIXEL,pixel_buf,(gr_interlace_t)j,dimsizes2,pal_ncomp,pal_nt);
              if(0!=HDmemcmp(tmp_data,pixel_buf,
                    (size_t)(pal_entries*pal_ncomp*DFKNTsize(pal_nt|DFNT_NATIVE))))
                {
                    MESSAGE(3, printf("Error reading data for palette j=%d\n",j););
                    num_errs++;
                } /* end if */
              HDfree(tmp_data);
              HDfree(pixel_buf);
          } /* end for */

        HDfree(pal_data);

	/* This lutid should yield a valid reference number, which is not 0 - BMR */ 
        pal_ref=GRluttoref(lutid);
	CHECK(pal_ref,0,"GRluttoref");

	/* Now, this bogus lutid should cause GRluttoref to return a 0 - BMR */
        pal_ref=GRluttoref(0);
	VERIFY(pal_ref,0,"GRluttoref");
        
        /* End access to the image */
        ret=GRendaccess(riid);
        CHECK(ret,FAIL,"GRendaccess");
      } /* end block */
    
    /* shut down the gr interface */
    ret=GRend(grid);
    CHECK(ret,FAIL,"GRend");

    /* close the file */
    ret=Hclose(fid);
    CHECK(ret,FAIL,"Hclose");
}   /* end test_mgr_lut() */

/****************************************************************
**
**  test_mgr_special(): Multi-file Raster Special Element Test Routine
** 
**  VI. Special Element Functions [Need to be implemented]
**      A. GRsetexternalfile
**      B. GRsetaccesstype
** 
****************************************************************/
static void
test_mgr_special(int flag)
{
    /* Output message about test being performed */
    MESSAGE(6, printf("Testing Multi-file Raster Special Element routines\n"););
}   /* end test_mgr_special() */

/****************************************************************
**
**  test_mgr_attr(): Multi-file Raster Attribute Test Routine
** 
**  VII. Atribute Functions
**      A. GRattrinfo
**      B. Read/Write Attributes
**          1. GRsetattr
**          2. GRgetattr
**      C. GRfindattr
** 
****************************************************************/
static void
test_mgr_attr(int flag)
{
    /* Output message about test being performed */
    MESSAGE(6, printf("Testing Multi-file Raster Attribute routines\n"););

/* I believe that these are adequately tested in the test_mgr_image routine -QAK */
}   /* end test_mgr_attr() */

#define OLDRLEFILE  "8bit.dat"
#define OLDGREYJPEGFILE  "greyjpeg.dat"
#define OLDJPEGFILE  "jpeg.dat"
#define JPEGX   46
#define JPEGY   23

static const uint8  jpeg_8bit_j80[JPEGY][JPEGX] =
{
    {200, 200, 200, 200, 200, 200, 200, 200, 202, 202, 201, 201, 201, 200, 200, 200, 201, 201, 200, 200, 200, 201, 202, 202, 202, 202, 201, 200, 200, 200, 200, 201, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200}, 
    {200, 200, 200, 200, 200, 200, 200, 200, 199, 199, 199, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 199, 199, 199, 199, 200, 200, 201, 201, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200}, 
    {200, 200, 200, 200, 200, 200, 200, 200, 199, 199, 199, 200, 200, 201, 201, 201, 200, 200, 201, 201, 201, 200, 199, 199, 199, 199, 200, 201, 201, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200}, 
    {200, 200, 200, 200, 200, 200, 200, 200, 201, 201, 201, 201, 201, 201, 201, 201, 201, 201, 202, 202, 202, 201, 200, 199, 202, 202, 202, 202, 202, 200, 199, 198, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200}, 
    {200, 200, 200, 200, 200, 200, 200, 200, 201, 201, 201, 200, 200, 200, 199, 199, 200, 200, 200, 201, 201, 200, 200, 200, 200, 200, 201, 201, 201, 200, 200, 199, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200}, 
    {200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 199, 199, 199, 198, 198, 198, 199, 199, 198, 198, 198, 199, 200, 201, 197, 197, 197, 198, 198, 200, 201, 202, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200}, 
    {200, 200, 200, 200, 200, 200, 200, 200, 199, 199, 200, 200, 201, 202, 202, 203, 201, 200, 199, 198, 199, 201, 204, 205, 203, 202, 200, 199, 198, 200, 202, 203, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200}, 
    {200, 200, 200, 200, 200, 200, 200, 200, 200, 201, 202, 203, 205, 207, 208, 209, 205, 203, 201, 200, 202, 205, 208, 211, 214, 211, 206, 202, 200, 200, 201, 202, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200}, 
    {199, 199, 198, 199, 200, 202, 204, 205, 202, 200, 197, 198, 201, 204, 205, 205, 220, 204, 207, 222, 225, 201, 206, 213, 205, 218, 219, 224, 222, 198, 222, 208, 222, 208, 235, 201, 189, 204, 198, 202, 199, 201, 209, 192, 192, 217}, 
    {200, 200, 199, 199, 200, 201, 202, 203, 210, 208, 205, 203, 201, 197, 191, 186, 170, 212, 216, 191, 222, 217, 218, 185, 216, 226, 200, 193, 207, 232, 193, 221, 210, 124, 127, 218, 223, 192, 197, 207, 207, 209, 210, 220, 209, 184}, 
    {202, 201, 201, 200, 200, 200, 201, 201, 196, 197, 200, 203, 203, 200, 195, 191, 196, 180, 168, 123, 34, 66, 56, 74, 57, 83, 73, 81, 94, 221, 72, 64, 120, 119, 83, 100, 207, 190, 198, 198, 94, 44, 57, 50, 213, 215}, 
    {54, 54, 54, 53, 53, 54, 54, 54, 49, 51, 55, 59, 62, 63, 63, 62, 54, 110, 65, 43, 69, 40, 54, 36, 53, 67, 57, 65, 66, 61, 94, 83, 83, 69, 102, 99, 64, 214, 195, 90, 28, 46, 63, 51, 43, 57}, 
    {52, 52, 53, 54, 54, 55, 55, 55, 56, 54, 51, 48, 47, 46, 48, 49, 56, 63, 48, 72, 67, 55, 51, 65, 50, 48, 46, 70, 49, 67, 63, 86, 59, 107, 60, 73, 40, 44, 102, 36, 40, 218, 192, 165, 101, 44}, 
    {198, 199, 200, 201, 202, 202, 202, 202, 199, 197, 194, 190, 188, 190, 194, 198, 196, 151, 88, 202, 61, 71, 148, 169, 165, 188, 139, 77, 86, 70, 81, 84, 163, 226, 206, 184, 52, 46, 48, 165, 204, 189, 179, 213, 208, 79}, 
    {199, 200, 201, 202, 202, 201, 200, 199, 201, 203, 204, 205, 206, 209, 214, 218, 218, 198, 195, 199, 182, 201, 202, 206, 201, 228, 240, 191, 131, 121, 110, 221, 211, 208, 207, 198, 213, 231, 204, 220, 197, 211, 207, 194, 207, 213}, 
    {201, 202, 202, 203, 202, 200, 199, 197, 199, 202, 206, 206, 204, 202, 202, 203, 205, 195, 210, 199, 211, 220, 210, 224, 224, 204, 215, 205, 227, 218, 229, 220, 203, 201, 199, 202, 206, 206, 195, 205, 203, 194, 202, 197, 210, 190}, 
    {200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 203, 206, 210, 213, 213, 210, 206, 203, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200}, 
    {200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 198, 199, 201, 202, 202, 201, 199, 198, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200}, 
    {200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 199, 199, 198, 197, 197, 198, 199, 199, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200}, 
    {200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 202, 201, 200, 199, 199, 200, 201, 202, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200}, 
    {200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 199, 199, 199, 199, 199, 199, 199, 199, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200}, 
    {200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 198, 199, 200, 201, 201, 200, 199, 198, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200}, 
    {200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 201, 201, 202, 202, 202, 202, 201, 201, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200} 
};

static const uint8  jpeg_24bit_j30[JPEGY][JPEGX][3] =
{
    235, 112, 16, 235, 112, 16, 235, 112, 16, 235, 112, 16, 235, 112, 16, 235, 112, 16, 
    235, 112, 16, 235, 112, 16, 230, 107, 11, 231, 108, 12, 232, 109, 13, 234, 111, 15, 
    236, 113, 17, 238, 115, 19, 239, 116, 20, 247, 113, 24, 255, 100, 25, 255, 95, 30, 
    255, 96, 32, 255, 96, 35, 255, 95, 41, 255, 95, 42, 255, 95, 46, 255, 95, 46, 252, 88, 37, 
    254, 88, 36, 255, 90, 32, 255, 90, 28, 255, 91, 25, 255, 92, 21, 255, 93, 17, 255, 98, 14, 
    250, 106, 10, 247, 107, 12, 255, 100, 19, 255, 96, 25, 255, 93, 30, 255, 95, 30, 
    255, 99, 28, 242, 108, 23, 228, 116, 16, 215, 124, 10, 210, 127, 9, 210, 127, 9, 
    217, 122, 16, 221, 119, 18, 

    245, 110, 3, 245, 110, 3, 245, 110, 3, 245, 110, 3, 245, 110, 3, 245, 110, 3, 245, 110, 3, 
    245, 110, 3, 241, 106, 0, 241, 106, 0, 242, 107, 0, 244, 109, 2, 245, 110, 3, 247, 112, 5, 
    248, 113, 6, 251, 112, 9, 250, 102, 6, 253, 99, 9, 251, 100, 11, 251, 99, 15, 250, 99, 20, 
    249, 99, 23, 249, 98, 25, 249, 98, 25, 249, 98, 25, 250, 98, 22, 251, 98, 18, 255, 99, 14, 
    255, 99, 8, 255, 100, 4, 255, 100, 1, 255, 100, 0, 255, 102, 0, 255, 101, 3, 255, 99, 7, 
    255, 98, 12, 255, 99, 14, 255, 101, 16, 252, 104, 14, 247, 107, 10, 242, 111, 5, 
    238, 114, 2, 235, 116, 0, 233, 116, 2, 235, 115, 5, 236, 113, 7, 

    255, 104, 0, 255, 104, 0, 255, 104, 0, 255, 104, 0, 255, 104, 0, 255, 104, 0, 255, 104, 0, 
    255, 104, 0, 255, 100, 0, 255, 100, 0, 255, 101, 0, 255, 102, 0, 255, 103, 0, 255, 104, 0, 
    255, 105, 0, 255, 109, 0, 243, 110, 0, 238, 112, 0, 236, 113, 0, 236, 112, 0, 235, 112, 0, 
    235, 111, 0, 235, 111, 0, 235, 111, 0, 241, 117, 0, 242, 116, 0, 243, 115, 0, 244, 116, 0, 
    245, 115, 0, 245, 114, 0, 246, 114, 0, 255, 107, 0, 255, 94, 0, 255, 90, 0, 255, 97, 0, 
    255, 104, 0, 247, 112, 0, 242, 115, 0, 246, 113, 0, 255, 108, 0, 255, 99, 0, 255, 94, 0, 
    255, 92, 0, 255, 94, 0, 255, 100, 0, 255, 103, 0, 

    255, 103, 0, 255, 104, 0, 255, 103, 0, 255, 104, 0, 255, 103, 0, 255, 104, 0, 255, 103, 0, 
    255, 104, 0, 255, 99, 0, 255, 101, 0, 255, 100, 0, 255, 101, 0, 255, 101, 0, 255, 102, 0, 
    255, 101, 0, 255, 105, 0, 251, 113, 0, 244, 116, 0, 244, 116, 0, 244, 115, 0, 242, 115, 0, 
    242, 114, 0, 242, 114, 0, 244, 113, 0, 249, 118, 0, 249, 118, 0, 249, 117, 0, 248, 117, 0, 
    248, 116, 0, 247, 116, 0, 248, 115, 0, 255, 108, 0, 255, 97, 0, 255, 93, 0, 255, 99, 0, 
    255, 105, 0, 252, 113, 0, 249, 114, 0, 253, 111, 0, 255, 106, 0, 255, 97, 0, 255, 93, 0, 
    255, 90, 0, 255, 92, 0, 255, 97, 0, 255, 100, 0, 

    255, 106, 0, 254, 106, 0, 255, 106, 0, 254, 106, 0, 255, 106, 0, 254, 106, 0, 255, 106, 0, 
    254, 106, 0, 253, 103, 0, 251, 103, 0, 253, 103, 0, 250, 102, 0, 252, 102, 0, 250, 102, 0, 
    251, 101, 0, 252, 101, 0, 255, 107, 0, 255, 106, 0, 255, 105, 0, 255, 105, 0, 255, 104, 0, 
    255, 103, 0, 255, 102, 0, 255, 101, 0, 255, 100, 0, 255, 100, 0, 255, 101, 0, 255, 102, 0, 
    255, 103, 0, 255, 102, 0, 255, 103, 0, 255, 106, 0, 236, 111, 21, 233, 111, 26, 250, 107, 5, 
    255, 104, 0, 255, 101, 0, 255, 100, 0, 255, 101, 0, 255, 103, 0, 249, 106, 14, 238, 108, 30, 
    232, 109, 39, 235, 109, 35, 246, 106, 19, 252, 105, 10, 

    250, 107, 5, 250, 107, 5, 250, 107, 5, 250, 107, 5, 250, 107, 5, 250, 107, 5, 250, 107, 5, 
    250, 107, 5, 248, 105, 3, 247, 104, 2, 246, 103, 1, 245, 102, 0, 244, 101, 0, 243, 100, 0, 
    242, 99, 0, 246, 98, 0, 255, 103, 0, 255, 101, 0, 255, 99, 0, 255, 98, 0, 255, 97, 0, 
    255, 96, 3, 255, 94, 7, 255, 94, 9, 255, 87, 2, 255, 88, 0, 255, 90, 0, 255, 94, 0, 
    255, 95, 0, 255, 98, 0, 255, 101, 0, 252, 107, 0, 222, 119, 18, 217, 120, 25, 235, 112, 16, 
    253, 105, 7, 255, 98, 0, 255, 96, 0, 255, 97, 3, 255, 102, 9, 242, 108, 19, 229, 113, 26, 
    224, 115, 30, 226, 114, 28, 240, 109, 21, 247, 106, 18, 

    252, 106, 7, 252, 106, 7, 252, 106, 7, 252, 106, 7, 252, 106, 7, 252, 106, 7, 252, 106, 7, 
    252, 106, 7, 250, 104, 5, 249, 103, 4, 248, 102, 3, 247, 101, 2, 245, 99, 0, 244, 98, 0, 
    242, 96, 0, 243, 96, 0, 247, 100, 0, 249, 99, 0, 250, 98, 0, 252, 96, 0, 253, 94, 0, 
    254, 93, 5, 255, 91, 9, 255, 90, 12, 244, 81, 2, 244, 83, 3, 246, 87, 3, 246, 92, 2, 
    247, 99, 3, 248, 105, 3, 249, 108, 2, 245, 115, 0, 233, 120, 0, 229, 121, 0, 231, 115, 14, 
    232, 110, 37, 235, 104, 58, 239, 101, 64, 245, 101, 51, 250, 102, 32, 254, 105, 2, 
    255, 108, 0, 255, 109, 0, 255, 108, 0, 255, 104, 0, 255, 102, 5, 

    255, 103, 5, 255, 104, 5, 255, 103, 5, 255, 104, 5, 255, 103, 5, 255, 104, 5, 255, 103, 5, 
    255, 104, 5, 255, 101, 3, 253, 101, 2, 253, 99, 1, 250, 98, 0, 249, 95, 0, 247, 95, 0, 
    246, 92, 0, 247, 91, 0, 247, 88, 0, 250, 86, 0, 252, 84, 0, 253, 83, 0, 255, 80, 4, 
    255, 79, 9, 255, 77, 14, 255, 77, 18, 248, 67, 10, 249, 70, 12, 248, 76, 12, 248, 83, 15, 
    251, 91, 17, 251, 98, 18, 252, 104, 18, 249, 110, 5, 242, 116, 0, 236, 118, 0, 228, 117, 12, 
    222, 114, 39, 219, 111, 62, 224, 108, 67, 235, 105, 55, 249, 102, 32, 255, 101, 0, 
    255, 99, 0, 255, 99, 0, 255, 99, 0, 255, 98, 0, 255, 99, 5, 

    255, 97, 0, 255, 97, 0, 255, 97, 0, 255, 97, 0, 255, 97, 0, 255, 97, 0, 255, 97, 0, 
    255, 97, 0, 245, 82, 0, 255, 96, 0, 255, 111, 10, 255, 114, 13, 255, 106, 5, 255, 98, 0, 
    255, 96, 0, 255, 95, 1, 255, 84, 4, 255, 62, 0, 245, 31, 0, 234, 18, 0, 255, 36, 0, 
    255, 66, 26, 255, 71, 38, 255, 56, 29, 255, 80, 55, 255, 35, 10, 225, 6, 0, 231, 21, 0, 
    254, 53, 11, 255, 64, 16, 244, 57, 4, 224, 55, 0, 204, 62, 0, 204, 82, 0, 164, 52, 0, 
    208, 103, 0, 255, 160, 52, 219, 108, 3, 217, 87, 0, 255, 128, 37, 255, 100, 20, 255, 93, 18, 
    255, 88, 19, 255, 91, 20, 255, 97, 19, 255, 99, 18, 

    233, 101, 29, 233, 101, 29, 233, 101, 29, 233, 101, 29, 233, 101, 29, 233, 101, 29, 
    233, 101, 29, 233, 101, 29, 225, 93, 21, 232, 100, 28, 239, 107, 35, 239, 107, 35, 
    236, 104, 32, 238, 106, 34, 248, 116, 44, 255, 124, 59, 235, 101, 50, 255, 122, 80, 
    255, 121, 87, 233, 85, 59, 202, 47, 29, 200, 42, 33, 221, 58, 59, 236, 73, 78, 182, 21, 29, 
    210, 53, 60, 243, 94, 96, 243, 105, 102, 207, 79, 70, 173, 56, 39, 186, 75, 55, 215, 117, 90, 
    197, 116, 86, 234, 162, 114, 248, 174, 103, 124, 45, 0, 139, 53, 0, 251, 155, 43, 
    217, 104, 10, 203, 76, 5, 205, 63, 25, 204, 54, 39, 207, 54, 49, 217, 66, 57, 227, 87, 54, 
    233, 96, 52, 

    186, 122, 110, 186, 122, 110, 186, 122, 110, 186, 122, 110, 186, 122, 110, 186, 122, 110, 
    186, 122, 110, 186, 122, 110, 180, 116, 104, 180, 116, 104, 177, 113, 101, 173, 109, 97, 
    175, 111, 99, 188, 124, 112, 207, 143, 131, 202, 169, 154, 150, 174, 158, 163, 213, 201, 
    177, 219, 215, 137, 175, 178, 88, 117, 131, 70, 92, 115, 80, 99, 131, 93, 111, 149, 
    95, 114, 156, 103, 131, 170, 120, 156, 192, 122, 171, 201, 94, 156, 179, 59, 132, 149, 
    49, 131, 143, 68, 144, 158, 133, 189, 216, 174, 205, 225, 149, 151, 140, 234, 208, 171, 
    226, 170, 113, 165, 93, 35, 241, 163, 124, 195, 123, 111, 155, 97, 122, 123, 79, 130, 
    96, 60, 124, 92, 57, 115, 118, 73, 102, 146, 98, 114, 

    91, 68, 94, 89, 69, 94, 91, 68, 94, 89, 69, 94, 91, 68, 94, 89, 69, 94, 91, 68, 94, 
    89, 69, 94, 84, 61, 87, 78, 58, 83, 74, 51, 77, 69, 49, 74, 75, 52, 78, 85, 65, 90, 
    103, 80, 106, 86, 104, 124, 53, 149, 161, 34, 166, 178, 27, 152, 172, 5, 123, 151, 
    0, 99, 138, 0, 88, 138, 0, 75, 136, 0, 60, 127, 0, 96, 165, 0, 87, 155, 0, 94, 158, 
    0, 124, 182, 9, 154, 207, 17, 175, 220, 30, 198, 237, 60, 212, 249, 38, 146, 182, 
    39, 114, 145, 184, 230, 254, 118, 136, 158, 83, 71, 91, 193, 165, 187, 150, 117, 148, 
    135, 107, 147, 74, 60, 111, 72, 68, 127, 66, 74, 136, 63, 72, 131, 61, 65, 113, 61, 62, 106, 


    78, 71, 113, 77, 72, 113, 78, 71, 113, 77, 72, 113, 78, 71, 113, 77, 72, 113, 78, 71, 113, 
    77, 72, 113, 79, 72, 114, 74, 69, 110, 72, 65, 107, 70, 65, 106, 74, 67, 109, 76, 71, 112, 
    80, 73, 115, 60, 84, 122, 38, 121, 153, 28, 137, 168, 27, 129, 169, 16, 108, 159, 
    16, 97, 160, 29, 103, 176, 36, 104, 187, 30, 97, 186, 19, 89, 184, 19, 100, 192, 
    20, 114, 202, 8, 117, 199, 0, 105, 182, 0, 108, 177, 0, 148, 212, 64, 192, 229, 90, 170, 159, 
    128, 184, 171, 49, 108, 142, 74, 129, 209, 7, 56, 175, 0, 20, 152, 142, 161, 255, 
    106, 108, 195, 51, 36, 77, 113, 90, 96, 176, 152, 140, 187, 172, 165, 140, 145, 167, 
    86, 100, 137, 

    172, 124, 136, 171, 125, 136, 172, 124, 136, 171, 125, 136, 172, 124, 136, 171, 125, 136, 
    172, 124, 136, 171, 125, 136, 185, 137, 149, 181, 135, 146, 179, 131, 143, 179, 133, 144, 
    182, 134, 146, 179, 133, 144, 172, 124, 136, 149, 123, 132, 108, 126, 128, 135, 170, 172, 
    162, 190, 201, 147, 165, 187, 126, 132, 166, 135, 134, 178, 169, 161, 218, 195, 186, 249, 
    176, 171, 238, 150, 155, 221, 131, 151, 212, 128, 163, 219, 114, 166, 214, 84, 151, 193, 
    60, 138, 174, 69, 133, 142, 206, 228, 189, 64, 71, 30, 118, 131, 139, 135, 151, 202, 
    90, 105, 196, 58, 61, 164, 49, 33, 122, 225, 187, 246, 150, 90, 102, 200, 129, 107, 
    241, 170, 128, 233, 173, 136, 183, 149, 140, 145, 124, 129, 

    241, 96, 31, 240, 97, 31, 241, 96, 31, 240, 97, 31, 241, 96, 31, 240, 97, 31, 241, 96, 31, 
    240, 97, 31, 255, 111, 46, 247, 104, 38, 242, 97, 32, 241, 98, 32, 246, 101, 36, 
    244, 101, 35, 237, 92, 27, 217, 88, 20, 184, 87, 8, 212, 125, 46, 240, 143, 74, 229, 123, 65, 
    212, 94, 48, 216, 89, 54, 232, 100, 77, 243, 110, 93, 233, 103, 90, 192, 72, 58, 
    177, 71, 55, 215, 127, 105, 255, 191, 160, 255, 205, 167, 217, 174, 131, 183, 139, 92, 
    99, 39, 0, 208, 135, 80, 180, 96, 34, 179, 81, 16, 193, 78, 13, 193, 65, 2, 237, 98, 43, 
    195, 48, 4, 242, 95, 61, 236, 93, 63, 219, 84, 55, 193, 69, 35, 174, 61, 17, 178, 72, 20, 


    255, 91, 0, 255, 91, 0, 255, 91, 0, 255, 91, 0, 255, 91, 0, 255, 91, 0, 255, 91, 0, 
    255, 91, 0, 255, 97, 0, 255, 86, 0, 255, 74, 0, 255, 73, 0, 255, 80, 0, 255, 83, 0, 
    255, 79, 0, 248, 76, 0, 254, 98, 0, 249, 100, 0, 247, 90, 0, 250, 87, 0, 255, 92, 3, 
    255, 91, 9, 242, 58, 0, 204, 19, 0, 217, 35, 0, 214, 41, 0, 220, 57, 0, 223, 72, 1, 
    202, 66, 0, 173, 48, 0, 162, 46, 0, 169, 58, 0, 227, 114, 46, 244, 122, 49, 242, 104, 6, 
    230, 75, 0, 255, 96, 0, 255, 78, 0, 236, 47, 0, 255, 117, 18, 255, 99, 30, 255, 94, 44, 
    255, 94, 53, 255, 100, 51, 255, 107, 34, 254, 104, 19, 

    255, 102, 0, 255, 102, 0, 255, 102, 0, 255, 102, 0, 255, 102, 0, 255, 102, 0, 255, 102, 0, 
    255, 102, 0, 255, 102, 0, 255, 102, 0, 255, 102, 0, 255, 102, 0, 255, 102, 0, 255, 102, 0, 
    255, 102, 0, 255, 102, 0, 253, 100, 0, 244, 93, 0, 234, 81, 0, 228, 72, 0, 229, 71, 0, 
    238, 78, 0, 251, 88, 0, 255, 96, 5, 255, 100, 10, 255, 101, 10, 255, 102, 10, 254, 104, 10, 
    252, 105, 10, 249, 106, 10, 247, 107, 10, 246, 107, 12, 246, 107, 16, 247, 106, 14, 
    253, 105, 5, 255, 105, 0, 255, 104, 0, 255, 103, 0, 255, 101, 0, 255, 101, 0, 255, 100, 9, 
    255, 100, 16, 255, 101, 19, 254, 102, 18, 254, 104, 10, 254, 104, 7, 

    255, 105, 2, 255, 105, 2, 255, 105, 2, 255, 105, 2, 255, 105, 2, 255, 105, 2, 255, 105, 2, 
    255, 105, 2, 255, 105, 2, 255, 105, 2, 255, 105, 2, 255, 105, 2, 255, 105, 2, 255, 105, 2, 
    255, 105, 2, 255, 105, 2, 255, 111, 8, 255, 106, 3, 249, 98, 0, 243, 92, 0, 243, 92, 0, 
    249, 98, 0, 255, 106, 3, 255, 111, 8, 255, 105, 2, 255, 105, 2, 255, 105, 2, 255, 105, 2, 
    255, 105, 2, 255, 105, 2, 255, 105, 2, 255, 105, 2, 255, 105, 2, 255, 105, 2, 255, 105, 2, 
    255, 105, 2, 255, 105, 2, 255, 105, 2, 255, 105, 2, 255, 105, 2, 255, 105, 2, 255, 105, 2, 
    255, 105, 2, 255, 105, 2, 255, 105, 2, 255, 105, 2, 

    255, 105, 2, 255, 105, 2, 255, 105, 2, 255, 105, 2, 255, 105, 2, 255, 105, 2, 255, 105, 2, 
    255, 105, 2, 255, 105, 2, 255, 105, 2, 255, 105, 2, 255, 105, 2, 255, 105, 2, 255, 105, 2, 
    255, 105, 2, 255, 105, 2, 255, 112, 9, 255, 110, 7, 255, 108, 5, 255, 106, 3, 255, 106, 3, 
    255, 108, 5, 255, 110, 7, 255, 112, 9, 255, 105, 2, 255, 105, 2, 255, 105, 2, 255, 105, 2, 
    255, 105, 2, 255, 105, 2, 255, 105, 2, 255, 105, 2, 255, 105, 2, 255, 105, 2, 255, 105, 2, 
    255, 105, 2, 255, 105, 2, 255, 105, 2, 255, 105, 2, 255, 105, 2, 255, 105, 2, 255, 105, 2, 
    255, 105, 2, 255, 105, 2, 255, 105, 2, 255, 105, 2, 

    255, 105, 2, 255, 105, 2, 255, 105, 2, 255, 105, 2, 255, 105, 2, 255, 105, 2, 255, 105, 2, 
    255, 105, 2, 255, 105, 2, 255, 105, 2, 255, 105, 2, 255, 105, 2, 255, 105, 2, 255, 105, 2, 
    255, 105, 2, 255, 105, 2, 252, 101, 0, 254, 103, 0, 255, 104, 1, 255, 105, 2, 255, 105, 2, 
    255, 104, 1, 254, 103, 0, 252, 101, 0, 255, 105, 2, 255, 105, 2, 255, 105, 2, 255, 105, 2, 
    255, 105, 2, 255, 105, 2, 255, 105, 2, 255, 105, 2, 255, 105, 2, 255, 105, 2, 255, 105, 2, 
    255, 105, 2, 255, 105, 2, 255, 105, 2, 255, 105, 2, 255, 105, 2, 255, 105, 2, 255, 105, 2, 
    255, 105, 2, 255, 105, 2, 255, 105, 2, 255, 105, 2, 

    255, 105, 2, 255, 105, 2, 255, 105, 2, 255, 105, 2, 255, 105, 2, 255, 105, 2, 255, 105, 2, 
    255, 105, 2, 255, 105, 2, 255, 105, 2, 255, 105, 2, 255, 105, 2, 255, 105, 2, 255, 105, 2, 
    255, 105, 2, 255, 105, 2, 247, 96, 0, 250, 99, 0, 253, 102, 0, 255, 104, 1, 255, 104, 1, 
    253, 102, 0, 250, 99, 0, 247, 96, 0, 255, 105, 2, 255, 105, 2, 255, 105, 2, 255, 105, 2, 
    255, 105, 2, 255, 105, 2, 255, 105, 2, 255, 105, 2, 255, 105, 2, 255, 105, 2, 255, 105, 2, 
    255, 105, 2, 255, 105, 2, 255, 105, 2, 255, 105, 2, 255, 105, 2, 255, 105, 2, 255, 105, 2, 
    255, 105, 2, 255, 105, 2, 255, 105, 2, 255, 105, 2, 

    255, 105, 2, 255, 105, 2, 255, 105, 2, 255, 105, 2, 255, 105, 2, 255, 105, 2, 255, 105, 2, 
    255, 105, 2, 255, 105, 2, 255, 105, 2, 255, 105, 2, 255, 105, 2, 255, 105, 2, 255, 105, 2, 
    255, 105, 2, 255, 105, 2, 253, 102, 0, 255, 104, 1, 255, 106, 3, 255, 108, 5, 255, 108, 5, 
    255, 106, 3, 255, 104, 1, 253, 102, 0, 255, 105, 2, 255, 105, 2, 255, 105, 2, 255, 105, 2, 
    255, 105, 2, 255, 105, 2, 255, 105, 2, 255, 105, 2, 255, 105, 2, 255, 105, 2, 255, 105, 2, 
    255, 105, 2, 255, 105, 2, 255, 105, 2, 255, 105, 2, 255, 105, 2, 255, 105, 2, 255, 105, 2, 
    255, 105, 2, 255, 105, 2, 255, 105, 2, 255, 105, 2, 

    255, 105, 2, 255, 105, 2, 255, 105, 2, 255, 105, 2, 255, 105, 2, 255, 105, 2, 255, 105, 2, 
    255, 105, 2, 255, 105, 2, 255, 105, 2, 255, 105, 2, 255, 105, 2, 255, 105, 2, 255, 105, 2, 
    255, 105, 2, 255, 105, 2, 255, 107, 4, 255, 106, 3, 255, 106, 3, 255, 105, 2, 255, 105, 2, 
    255, 106, 3, 255, 106, 3, 255, 107, 4, 255, 105, 2, 255, 105, 2, 255, 105, 2, 255, 105, 2, 
    255, 105, 2, 255, 105, 2, 255, 105, 2, 255, 105, 2, 255, 105, 2, 255, 105, 2, 255, 105, 2, 
    255, 105, 2, 255, 105, 2, 255, 105, 2, 255, 105, 2, 255, 105, 2, 255, 105, 2, 255, 105, 2, 
    255, 105, 2, 255, 105, 2, 255, 105, 2, 255, 105, 2 
};

/* Sub-tests for test_mgr_old() */
static void test_mgr_old_a(int flag)
{
    int32 fid;              /* HDF file ID */
    int32 grid;             /* GRID for the interface */
    int32 ret;              /* generic return value */

/* A - Read RLE compressed data from old raster image file */
    MESSAGE(8, printf("Read RLE compressed image\n"););

    /* Open up the existing datafile and get the image information from it */
    fid=Hopen(OLDRLEFILE,DFACC_RDWR,0);
    CHECK(fid,FAIL,"Hopen");

    /* Initialize the GR interface */
    grid=GRstart(fid);
    CHECK(grid,FAIL,"GRstart");

    {
        int32 riid;     /* RI ID for the new image */
        int32 dims[2]={10,10};    /* dimensions for the empty image */
        uint8 image[10][10]; /* space for the image data */
        uint8 image0[10][10]; /* space for the image data */
        int32 start[2]; /* start of image data to grab */
        int32 stride[2];/* stride of image data to grab */
        intn i,j;       /* indices */

        /* Initialize data we are looking for in image */
        for (i = 0; i < 10; i++)
            for (j = 0; j < 10; j++)
                image0[i][j] = (uint8) (i + j);

        /* Get the first image in this file */
        riid=GRselect(grid,0);
        CHECK(riid,FAIL,"GRselect");

        /* Read the whole image in */
        start[0]=start[1]=0;
        stride[0]=stride[1]=1;
        ret=GRreadimage(riid,start,stride,dims,image);
        CHECK(ret,FAIL,"GRreadimage");

        /* Verify correct image contents */
        if(HDmemcmp(image,image0,10*10)!=0) {
            MESSAGE(3, printf("Error reading data for RLE compressed image\n"););
            num_errs++;
        } /* end if */

        /* Close the empty image */
        ret=GRendaccess(riid);
        CHECK(ret,FAIL,"GRendaccess");
    }
    
    /* Shut down the GR interface */
    ret=GRend(grid);
    CHECK(ret,FAIL,"GRend");

    /* Close the file */
    ret=Hclose(fid);
    CHECK(ret,FAIL,"Hclose");
} /* end test_mgr_old_a() */

static void test_mgr_old_c(int flag)
{
    int32 fid;              /* HDF file ID */
    int32 grid;             /* GRID for the interface */
    int32 ret;              /* generic return value */

/* C - Read 8-bit JPEG compressed data from old raster image file */
    MESSAGE(8, printf("Read 8-bit JPEG compressed image\n"););

    /* Open up the existing datafile and get the image information from it */
    fid=Hopen(OLDGREYJPEGFILE,DFACC_READ,0);
    CHECK(fid,FAIL,"Hopen");

    /* Initialize the GR interface */
    grid=GRstart(fid);
    CHECK(grid,FAIL,"GRstart");

    {
        int32 riid;     /* RI ID for the new image */
        int32 dims[2]={JPEGX,JPEGY};    /* dimensions for the empty image */
        uint8 image[JPEGY][JPEGX]; /* space for the image data */
        int32 start[2]; /* start of image data to grab */
        int32 stride[2];/* stride of image data to grab */

        /* Get the first image in this file */
        riid=GRselect(grid,0);
        CHECK(riid,FAIL,"GRselect");

        /* Read the whole image in */
        start[0]=start[1]=0;
        stride[0]=stride[1]=1;
        ret=GRreadimage(riid,start,stride,dims,image);
        CHECK(ret,FAIL,"GRreadimage");

        /* Verify correct image contents */
        if(HDmemcmp(image,jpeg_8bit_j80,JPEGY*JPEGX)!=0) {
            MESSAGE(3, printf("Error reading data for 8-bit JPEG compressed image\n"););
            num_errs++;
        } /* end if */

        /* Close the empty image */
        ret=GRendaccess(riid);
        CHECK(ret,FAIL,"GRendaccess");
    }
    
    /* Shut down the GR interface */
    ret=GRend(grid);
    CHECK(ret,FAIL,"GRend");

    /* Close the file */
    ret=Hclose(fid);
    CHECK(ret,FAIL,"Hclose");
} /* end test_mgr_old_c() */

static void test_mgr_old_e(int flag)
{
    int32 fid;              /* HDF file ID */
    int32 grid;             /* GRID for the interface */
    int32 ret;              /* generic return value */

/* E - Read 24-bit JPEG compressed data from old raster image file */
    MESSAGE(8, printf("Read 24-bit JPEG compressed image\n"););

    /* Open up the existing datafile and get the image information from it */
    fid=Hopen(OLDJPEGFILE,DFACC_READ,0);
    CHECK(fid,FAIL,"Hopen");

    /* Initialize the GR interface */
    grid=GRstart(fid);
    CHECK(grid,FAIL,"GRstart");

    {
        int32 riid;     /* RI ID for the new image */
        int32 dims[2]={JPEGX,JPEGY};    /* dimensions for the empty image */
        uint8 image[JPEGY][JPEGX][3]; /* space for the image data */
        int32 start[2]; /* start of image data to grab */
        int32 stride[2];/* stride of image data to grab */

        /* Get the first image in this file */
        riid=GRselect(grid,0);
        CHECK(riid,FAIL,"GRselect");

        /* Read the whole image in */
        start[0]=start[1]=0;
        stride[0]=stride[1]=1;
        ret=GRreadimage(riid,start,stride,dims,image);
        CHECK(ret,FAIL,"GRreadimage");

        /* Verify correct image contents */
        if(HDmemcmp(image,jpeg_24bit_j30,JPEGY*JPEGX*3)!=0) {
            MESSAGE(3, printf("Error reading data for 8-bit JPEG compressed image\n"););
            num_errs++;
        } /* end if */

        /* Close the empty image */
        ret=GRendaccess(riid);
        CHECK(ret,FAIL,"GRendaccess");
    }
    
    /* Shut down the GR interface */
    ret=GRend(grid);
    CHECK(ret,FAIL,"GRend");

    /* Close the file */
    ret=Hclose(fid);
    CHECK(ret,FAIL,"Hclose");
} /* end test_mgr_old_c() */

/****************************************************************
**
**  test_mgr_old(): Multi-file Raster Old-style Image Access tests
** 
**  VIII. Old-style raster image tests
**      A. Read data from RLE compressed image
**      B. Create RLE compressed image & write to it (not implemented)
**      C. Read data from 8-bit JPEG compressed image
**      D. Create 8-bit JPEG compressed image & write to it
**      E. Read data from 24-bit JPEG compressed image
**      F. Create 24-bit JPEG compressed image & write to it
** 
****************************************************************/
static void
test_mgr_old(int flag)
{
    /* Output message about test being performed */
    MESSAGE(6, printf("Testing Multi-file Raster Old-Style Access\n"););
    test_mgr_old_a(flag);
#ifdef NOT_IMPLEMENTED
    test_mgr_old_b(flag);
#endif
    test_mgr_old_c(flag);
#ifdef NOT_IMPLEMENTED
    test_mgr_old_d(flag);
#endif
    test_mgr_old_e(flag);
#ifdef NOT_IMPLEMENTED
    test_mgr_old_f(flag);
#endif /* NOT_YET */

}   /* end test_mgr_old() */

/****************************************************************
**
**  test_mgr_compress(): Multi-file Raster Compression tests
** 
**  VIII. Compressed image tests
**      A. Create Image
**      B. Write data to RLE compressed image
**      C. Read data from JPEG compressed image
**      D. Write data to JPEG compressed image
** 
****************************************************************/
static void
test_mgr_compress(int flag)
{
    /* Output message about test being performed */
    MESSAGE(6, printf("Testing Multi-file Raster Compression Testing\n"););
#ifdef NOT_YET
    test_mgr_compress_a(flag);
    test_mgr_compress_b(flag);
    test_mgr_compress_c(flag);
    test_mgr_compress_d(flag);
#endif /* NOT_YET */

}   /* end test_mgr_compress() */

/****************************************************************
**
**  test_mgr(): Main multi-file raster image test routine
** 
****************************************************************/
void
test_mgr(void)
{
    /*
        Each major outline portion has it's own main function:
        I. Interface Initialization     - test_mgr_init
        II. Create Images               - test_mgr_image
        III. ID/Ref/Index Functions     - test_mgr_index
        IV. Interlace Functions         - test_mgr_interlace
        V. Palette Functions            - test_mgr_lut
        VI. Special Element Functions   - test_mgr_special
        VII. Atribute Functions         - test_mgr_attr
        VIII. Access to old-style images - test_mgr_old
        VIII. Compressed images         - test_mgr_compress
    */

    /* Output message about test being performed */
    MESSAGE(5, printf("Testing Multi-file Raster routines\n"););
    test_mgr_init();
    test_mgr_image(0); /* normal GR */
    test_mgr_image(1); /* chunked GR */
    test_mgr_index(0);
    test_mgr_interlace(0); /* read from normal GR */
    test_mgr_interlace(1); /* read from chunked GR */
    test_mgr_lut(0);
    test_mgr_special(0);
    test_mgr_attr(0);
    test_mgr_old(0);
    test_mgr_compress(0);
}   /* test_mgr() */

