/*
$Header$

$Log$
Revision 1.1  1992/08/25 21:40:44  koziol
Initial revision

*/
/*-----------------------------------------------------------------------------
 * File:    dfrig.h
 * Purpose: header file for the Raster Image set
 * Invokes: df.h
 * Contents:
 *  Structure definitions: DFRdr, DFRrig
 * Remarks: This is included with user programs which use RIG
 *---------------------------------------------------------------------------*/


#ifndef DFRIG                  /* avoid re-inclusion */
#define DFRIG

typedef struct {
    uint16 tag, ref;
} DFdi;

/* description record: used to describe image data, palette data etc. */
typedef struct {
    int32 xdim, ydim;          /* dimensions of data */
    DFdi nt;                   /* number type of data */
    int16 ncomponents, interlace; /* data ordering: chunky / planar etc */
    DFdi compr;                        /* compression */
    /* ### Note: compression is currently uniquely described with a tag.
       No data is attached to this tag/ref.  But this capability is
       provided for future expansion, when this tag/ref might point to
       some data needed for decompression, such as the actual encodings */
} DFRdr;

/* structure to hold RIG info */
typedef struct {
    DFdi image;                        /* image */
    DFRdr descimage;           /* image data description */
    DFdi lut;                  /* color look-up table (palette) */
    DFRdr desclut;             /* look-up table description */
    DFdi mattechannel;
    DFRdr descmattechannel;
    int32 xpos, ypos;          /* X-Y position of image on screen */
    float32 aspectratio;         /* ratio of pixel height to width */
    float32 ccngamma, ccnred[3], ccngrren[3], ccnblue[3], ccnwhite[3];
                               /* color correction parameters */
    char *cf;                  /* color format */
} DFRrig;

#endif /*DFRIG*/
