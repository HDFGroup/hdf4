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

/* $Id$ */

/*+ hkit.h
***  private header file for hkit routines
+*/

#ifndef __HKIT_H

/* tag_messages is the list of tag descriptions in the system, kept as
   tag-description pairs.  To look up a description, a linear search is
   required but efficiency should be okay. */
typedef struct tag_descript_t {
    uint16 tag;    /* tag for description ? */
    char   *desc;  /* tag description ? */  
} tag_descript_t;

/*  NOTE: 
 *        Please keep tag names <= 30 characters - a 
 *        lot of pretty-printing code depends on it.
*/
PRIVATE const tag_descript_t tag_descriptions[] =
{
{ DFTAG_NULL  , "No Data"},
{ DFTAG_VERSION , "Version Descriptor"},
{ DFTAG_LINKED , "Linked Blocks Indicator"},
{ DFTAG_FID   , "File Identifier"},
{ DFTAG_FD    , "File Description"},
{ DFTAG_TID   , "Tag Identifier"},
{ DFTAG_TD    , "Tag Description"},
{ DFTAG_DIL   , "Data Id Label"},
{ DFTAG_DIA   , "Data Id Annotation"},
{ DFTAG_NT    , "Number type"},
{ DFTAG_MT    , "Machine type"},
{ DFTAG_COMPRESSED, "Compressed Data Indicator"},
      
      /* raster-8 Tags */
{ DFTAG_ID8   , "Image Dimensions-8"},
{ DFTAG_IP8   , "Image Palette-8"},
{ DFTAG_RI8   , "Raster Image-8"},
{ DFTAG_CI8   , "RLE Compressed Image-8"},
{ DFTAG_II8   , "Imcomp Image-8"},
      
      /* Raster Image Tags */
{ DFTAG_ID    , "Image Dimensions"},
{ DFTAG_LUT   , "Image Palette"},
{ DFTAG_RI    , "Raster Image Data"},
{ DFTAG_CI    , "Compressed Image"},
{ DFTAG_RIG   , "Raster Image Group"},
{ DFTAG_LD    , "Palette Dimension"},
{ DFTAG_MD    , "Matte Dimension"},
{ DFTAG_MA    , "Matte Data"},
{ DFTAG_CCN   , "Color Correction"},
{ DFTAG_CFM   , "Color Format"},
{ DFTAG_AR    , "Aspect Ratio"},
{ DFTAG_DRAW  , "Sequenced images"},
{ DFTAG_RUN   , "Runable program / script"},
{ DFTAG_XYP   , "X-Y position"},
{ DFTAG_MTO   , "M/c-Type override"},
      
      /* Tektronix */
{ DFTAG_T14   , "TEK 4014 Data"},
{ DFTAG_T105  , "TEK 4105 data"},
      
      /* Compression Schemes */
{ DFTAG_RLE   , "Run Length Encoding"},
{ DFTAG_IMCOMP , "IMCOMP Encoding"},
{ DFTAG_JPEG , "24-bit JPEG Encoding"},
{ DFTAG_GREYJPEG , "8-bit JPEG Encoding"},
      
      /* Scientific / Numeric Data Sets */
{ DFTAG_SDG   , "Scientific Data Group"},
{ DFTAG_NDG   , "Numeric Data Group"},
{ DFTAG_SD    , "Scientific Data"},
{ DFTAG_SDD   , "SciData dimension record"},
{ DFTAG_SDL   , "SciData labels"},
{ DFTAG_SDU   , "SciData units"},
{ DFTAG_SDF   , "SciData formats"},
{ DFTAG_SDS   , "SciData scales"},
{ DFTAG_SDM   , "SciData max/min"},
{ DFTAG_SDC   , "SciData coordsys"},
{ DFTAG_SDT   , "Transpose"},
{ DFTAG_SDLNK , "Links related to the dataset"},
{ DFTAG_CAL   , "Calibration information"},
      
      /* V Group Tags */
{ DFTAG_VG   , "Vgroup"},
{ DFTAG_VH   , "Vdata"},
{ DFTAG_VS   , "Vdata Storage"}
};

#endif /* __HKIT_H */

