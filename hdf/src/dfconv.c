#ifdef RCSID
static char RcsId[] = "@(#)$Revision$";
#endif
/*
$Header$

$Log$
Revision 1.7  1993/02/17 21:19:33  koziol
Enabled Cray speedups, but left testing code in still

 * Revision 1.6  1993/02/16  18:47:26  koziol
 * Cray Speedups
 *
 * Revision 1.5  1993/01/26  19:42:18  koziol
 * Added support for reading and writing Little-Endian data on all
 * platforms.  This has been tested on: Cray, Sun, and PCs so far.
 *
 * Revision 1.4  1993/01/19  05:54:16  koziol
 * Merged Hyperslab and JPEG routines with beginning of DEC ALPHA
 * port.  Lots of minor annoyances fixed.
 *
 * Revision 1.3  1993/01/15  23:47:37  sxu
 * bug fixed in DFKvo8f and DFKvi8f
 *
 * Revision 1.2  1992/11/02  16:35:41  koziol
 * Updates from 3.2r2 -> 3.3
 *
 * Revision 1.1  1992/08/25  21:40:44  koziol
 * Initial revision
 *
*/
/************************************************************************/
/* DFCONV.C                                                             */
/************************************************************************/

/*------------------------------------------------------------------
 File:  dfconv.c

 Purpose:
    Routines to support conversion to and from HDF format

 Invokes:
    
 PRIVATE conversion functions:
    DFKnb1b -  Native mode for 8 bit integers
    DFKnb2b -  Native mode for 16 bit integers
    DFKnb4b -  Native mode for 32 bit integers and floats 
    DFKnb8b -  Native mode for 64 bit floats 
    DFKsb2b -  Byte swapping for 16 bit integers
    DFKsb4b -  Byte swapping for 32 bit integers
    DFKsb8b -  Byte swapping for 64 bit floats 
    DFKui2i -  Unicos routine for importing 16 bit unsigned integers
    DFKui2s -  Unicos routine for importing 16 bit signed integers
    DFKuo2i -  Unicos routine for exporting 16 bit integers (both)
    DFKui4i -  Unicos routine for importing unsigned 32bit integers
    DFKui4s -  Unicos routine for importing signed 32bit integers
    DFKuo4i -  Unicos routine for exporting 4 byte integers (both)
    DFKui4f -  Unicos routine for importing 32 bit floats   
    DFKuo4f -  Unicos routine for exporting 32 bit floats  
    DFKui8f -  Unicos routine for importing 64 bit floats 
    DFKuo8f -  Unicos routine for exporting 64 bit floats
    DFKlui2i-  Unicos routine for importing little-endian 16 bit unsigned ints
    DFKlui2s-  Unicos routine for importing little-endian 16 bit signed ints
    DFKluo2i-  Unicos routine for exporting little-endian 16 bit ints (both)
    DFKlui4i-  Unicos routine for importing little-endian unsigned 32bit ints
    DFKlui4s-  Unicos routine for importing little-endian signed 32bit ints
    DFKluo4i-  Unicos routine for exporting little-endian 4 byte ints (both)
    DFKlui4f-  Unicos routine for importing little-endian 32 bit floats
    DFKluo4f-  Unicos routine for exporting little-endian 32 bit floats
    DFKlui8f-  Unicos routine for importing little-endian 64 bit floats
    DFKluo8f-  Unicos routine for exporting little-endian 64 bit floats
    DFKvi4f -  VMS routine for importing 32 bit floats
    DFKvo4f -  VMS routine for exporting 32 bit floats
    DFKvi8f -  VMS routine for importing 64 bit floats
    DFKvo8f -  VMS routine for exporting 64 bit floats
    DFKlvi4f-  VMS routine for importing little-endian 32 bit floats
    DFKlvo4f-  VMS routine for exporting little-endian 32 bit floats
    DFKlvi8f-  VMS routine for importing little-endian 64 bit floats
    DFKlvo8f-  VMS routine for exporting little-endian 64 bit floats
    DFKci4f -  Convex routine for importing 32 bit floats
    DFKco4f -  Convex routine for exporting 32 bit floats
    DFKci8f -  Convex routine for importing 64 bit floats
    DFKco8f -  Convex routine for exporting 64 bit floats
    DFKlci4f-  Convex routine for importing little-endian 32 bit floats
    DFKlco4f-  Convex routine for exporting little-endian 32 bit floats
    DFKlci8f-  Convex routine for importing little-endian 64 bit floats
    DFKlco8f-  Convex routine for exporting little-endian 64 bit floats
    DFKpi4f -  VP routine for importing 32 bit floats
    DFKpo4f -  VP routine for exporting 32 bit floats
    DFKpi8f -  VP routine for importing 64 bit floats
    DFKpo8f -  VP routine for exporting 64 bit floats

 Other PUBLIC functions:
    DFKmachineNTsize - Determine size in machine, given number type
    DFKhdfNTsize     - Determine size in HDF format, given number type
    DFKsetNT         - Set number type for future conversion calls
    DFKsetcustom    - Template for user to setup custom conversion 
                      routines
    DFKisnative     - Checks whether number type is native mode
    DFKislitend     - Checks whether number type is little-endian mode
    DFconvert       - provide compatibility with 3.0 routines

 Private functions:
    DFKInoset    - Indicate that DFKsetNT hasn't been called

 Remarks:

 *------------------------------------------------------------------*/

/*****************************************************************************/
/*                                                                           */
/*    All the routines in this file marked as PRIVATE have been marked so    */
/*  for a reason.  *ANY* of these routines may or may nor be supported in    */
/*  the next version of HDF (4.00).  Furthurmore, the names, paramters, or   */
/*  functionality is *NOT* guaranteed to remain the same.                    */
/*    The *ONLY* guarantee possible is that DFKnumin(), and DFKnumout()      */
/*  will not change.  They are *NOT* guaranteed to be implemented in the     */
/*  next version of HDF as function pointers.  They are guaranteed to take   */
/*  the same arguments and produce the same results.                         */
/*    If your programs call any routines in this file except for             */
/*  DFKnumin(), DFKnumout, and/or DFKsetntype(), your code may not work      */
/*  with future versions of HDF and your code will *NOT* be portable.        */
/*                                                                           */
/*****************************************************************************/


#include <ctype.h>
#define DFKMASTER  /* Define this for the prototypes for DFKnumin/out */
#include "hdf.h"
#include "herr.h"
#include "hconv.h"


/* #define TEST */
#ifdef TEST
#define TEST3_sb4b

#define TEST2_ui2i
#define TEST2_ui2s
#define TEST1_uo2i
#define TEST2_ui4i
#define TEST2_ui4s
#define TEST1_uo4i
#define TEST1_ui4f
#define TEST1_uo4f
#define TEST1_ui8f
#define TEST1_uo8f

#define TEST2_lui2i
#define TEST2_lui2s
#define TEST1_luo2i
#define TEST2_lui4i
#define TEST2_lui4s
#define TEST2_luo4i
#define TEST1_lui4f
#define TEST1_luo4f
#define TEST1_lui8f
#define TEST1_luo8f
#endif

#define DUFF 
#ifdef DUFF
#define DUFF_sb4b

#define DUFF_ui2i
#define DUFF_ui2s
#define DUFF_uo2i
#define DUFF_ui4i
#define DUFF_ui4s
#define DUFF_uo4i
#define DUFF_ui4f
#define DUFF_uo4f
#define DUFF_ui8f
#define DUFF_uo8f

#define DUFF_lui2i
#define DUFF_lui2s
#define DUFF_luo2i
#define DUFF_lui4i
#define DUFF_lui4s
#define DUFF_luo4i
#define DUFF_lui4f
#define DUFF_luo4f
#define DUFF_lui8f
#define DUFF_luo8f
#endif

/*
**  Static function prototypes
*/
PRIVATE int DFKInoset
    PROTO((VOIDP source, VOIDP dest, uint32 num_elm,uint32 source_stride,
            uint32 dest_stride));

/*
**  Conversion Routine Pointer Definitions
*/
int (*DFKnumin)
        PROTO((VOIDP source, VOIDP dest, uint32 num_elm,
                uint32 source_stride, uint32 dest_stride)) = DFKInoset;
int (*DFKnumout)
        PROTO((VOIDP source, VOIDP dest, uint32 num_elm,
                uint32 source_stride, uint32 dest_stride)) = DFKInoset;

/*****************************************************************************/
/* NATIVE MODE NUMBER "CONVERSION" ROUTINES                                  */
/*****************************************************************************/

/************************************************************/
/* DFKnb1b()                                                */
/*   Native mode for 1 byte data items                      */
/************************************************************/
#ifdef PROTOTYPE
PRIVATE int DFKnb1b(VOIDP s, VOIDP d, uint32 num_elm, uint32 source_stride,
		   uint32 dest_stride)
#else
PRIVATE int DFKnb1b(s, d, num_elm, source_stride, dest_stride)
VOIDP s, d;
uint32 num_elm, source_stride, dest_stride;
#endif /* PROTOTYPE */
{
  int fast_processing = 0;
  int in_place = 0;
  register uint32 i;
  uint8 * source = (uint8*)s;
  uint8 * dest = (uint8*)d;
  char *FUNC="DFKnb1b";

  HEclear();

  if(num_elm == 0){
    HERROR(DFE_BADCONV);
    return FAIL;
  }

  /* Determine if faster array processing is appropriate */
  if((source_stride == 0 && dest_stride == 0) ||
     (source_stride == 1 && dest_stride == 1))
    fast_processing = 1;

  /* Determine if the conversion should be inplace */
  if(source == dest)
    in_place = 1;

  if(fast_processing) {
    if(!in_place) {
      HDmemcpy(dest, source, num_elm);
      return 0;
    }
    else
      return 0;                         /* Nothing to do */
  } 
  else
    for(i = 0; i < num_elm; i++) {
      dest[0] = source[0];
      dest += dest_stride;
      source += source_stride;
    }
  
  return 0;
}

#if !defined(UNICOS)  /* UNICOS does not need these routines */

/************************************************************/
/* DFKnb2b()                                                */
/* -->Native mode for 2 byte data items                     */
/************************************************************/
#ifdef PROTOTYPE
PRIVATE int DFKnb2b(VOIDP s, VOIDP d, uint32 num_elm, uint32 source_stride,
		   uint32 dest_stride)
#else
PRIVATE int DFKnb2b(s, d, num_elm, source_stride, dest_stride)
VOIDP s, d;
uint32 num_elm, source_stride, dest_stride;
#endif /* PROTOTYPE */
{
  int fast_processing = 0;              /* Default is not fast processing */
  int in_place = 0;                     /* Inplace must be detected */
  register uint32 i;            
  uint8 buf[2];                          /* Inplace processing buffer */
  uint8 * source = (uint8*)s;
  uint8 * dest = (uint8*)d;
  char *FUNC="DFKnb2b";

  HEclear();

  if(num_elm == 0){
    HERROR(DFE_BADCONV);
    return FAIL;
  }

  /* Determine if faster array processing is appropriate */
  if((source_stride == 0 && dest_stride == 0) ||
     (source_stride == 2 && dest_stride == 2))
    fast_processing = 1;

  /* Determine if the conversion should be inplace */
  if(source == dest)
    in_place = 1;

  if(fast_processing) 
    if(!in_place) {
      HDmemcpy(dest, source, num_elm*2);
      return 0;
    }
    else {    /* Nothing to do */
      return 0;
    }
  
  /* Generic stride processing */
  if(!in_place)
    for(i = 0; i < num_elm; i++) {
      dest[0] = source[0];
      dest[1] = source[1];
      dest += dest_stride;
      source += source_stride;
    }
  else
    for(i = 0; i < num_elm; i++) {
      buf[0] = source[0];
      buf[1] = source[1];
      dest[0] = buf[0];
      dest[1] = buf[1];
      dest += dest_stride;
      source += source_stride;
    }
    
  return 0;
}

/************************************************************/
/* DFKnb4b()                                                */
/* -->Native mode for 4 byte items                          */
/************************************************************/
#ifdef PROTOTYPE
PRIVATE int DFKnb4b(VOIDP s, VOIDP d, uint32 num_elm,
		   uint32 source_stride, uint32 dest_stride)
#else
PRIVATE int DFKnb4b(s, d, num_elm, source_stride, dest_stride)
uint8 * s, * d;
uint32 num_elm, source_stride, dest_stride;
#endif /* PROTOTYPE */
{
  int fast_processing = 0;              /* Default is not fast processing */
  int in_place = 0;                     /* Inplace must be detected */
  register uint32 i;            
  uint8 buf[4];                          /* Inplace processing buffer */
  uint8 * source = (uint8*)s;
  uint8 * dest = (uint8*)d;
  char *FUNC="DFKnb4b";

  HEclear();

  if(num_elm == 0){
    HERROR(DFE_BADCONV);
    return FAIL;
  }

  /* Determine if faster array processing is appropriate */
  if((source_stride == 0 && dest_stride == 0) ||
     (source_stride == 4 && dest_stride == 4))
    fast_processing = 1;

  /* Determine if the conversion should be inplace */
  if(source == dest)
    in_place = 1;

  if(fast_processing) 
    if(!in_place) {
      HDmemcpy(dest, source, num_elm*4);
      return 0;
    }
    else {  /* Nothing to do */
      return 0;
    }
 
  /* Generic stride processing */
  if(!in_place)
    for(i = 0; i < num_elm; i++) {
      dest[0] = source[0];
      dest[1] = source[1];
      dest[2] = source[2];
      dest[3] = source[3];
      dest += dest_stride;
      source += source_stride;
    }
  else
    for(i = 0; i < num_elm; i++) {
      buf[0] = source[0];
      buf[1] = source[1];
      buf[2] = source[2];
      buf[3] = source[3];
      dest[0] = buf[0];
      dest[1] = buf[1];
      dest[2] = buf[2];
      dest[3] = buf[3];
      dest += dest_stride;
      source += source_stride;
    }
    
  return 0;
}

#endif /* UNICOS */

/************************************************************/
/* DFKnb8b()                                                */
/* -->Native mode for 8 byte items                          */
/************************************************************/
#ifdef PROTOTYPE
PRIVATE int DFKnb8b(VOIDP s, VOIDP d, uint32 num_elm,
		   uint32 source_stride, uint32 dest_stride)
#else
PRIVATE int DFKnb8b(source, dest, num_elm, source_stride, dest_stride)
uint8 * source, * dest;
uint32 num_elm, source_stride, dest_stride;
#endif /* PROTOTYPE */
{
  int fast_processing = 0;              /* Default is not fast processing */
  int in_place = 0;                     /* Inplace must be detected */
  register uint32 i;            
  uint8 buf[8];                          /* Inplace processing buffer */
#ifdef PROTOTYPE
  uint8 * source = (uint8*)s;
  uint8 * dest = (uint8*)d;
#endif /* PROTOTYPE */

    char *FUNC="DFKnb8b";

    HEclear();

  if(num_elm == 0){
    HERROR(DFE_BADCONV);
    return FAIL;
  }

  /* Determine if faster array processing is appropriate */
  if((source_stride == 0 && dest_stride == 0) ||
     (source_stride == 8 && dest_stride == 8))
    fast_processing = 1;

  /* Determine if the conversion should be inplace */
  if(source == dest)
    in_place = 1;

  if(fast_processing) 
    if(!in_place) {
      HDmemcpy(dest, source, num_elm*8);
      return 0;
    }
    else {  
      return 0;     /* No work to do ! */
      }
 
  /* Generic stride processing */
  if(!in_place)
    for(i = 0; i < num_elm; i++) {
      dest[0] = source[0];
      dest[1] = source[1];
      dest[2] = source[2];
      dest[3] = source[3];
      dest[4] = source[4];
      dest[5] = source[5];
      dest[6] = source[6];
      dest[7] = source[7];
      dest += dest_stride;
      source += source_stride;
    }
  else
    for(i = 0; i < num_elm; i++) {
      buf[0] = source[0];
      buf[1] = source[1];
      buf[2] = source[2];
      buf[3] = source[3];
      buf[4] = source[4];
      buf[5] = source[5];
      buf[6] = source[6];
      buf[7] = source[7];
      dest[0] = buf[0];
      dest[1] = buf[1];
      dest[2] = buf[2];
      dest[3] = buf[3];
      dest[4] = buf[4];
      dest[5] = buf[5];
      dest[6] = buf[6];
      dest[7] = buf[7];
      dest += dest_stride;
      source += source_stride;
    }
    
  return 0;
}


/*****************************************************************************/
/* NUMBER CONVERSION ROUTINES FOR BYTE SWAPPING                              */
/*****************************************************************************/

/************************************************************/
/* DFKsb2b()                                                */
/* -->Byte swapping for 2 byte data items                   */
/************************************************************/
#ifdef PROTOTYPE
PRIVATE int DFKsb2b(VOIDP s, VOIDP d, uint32 num_elm, uint32 source_stride,
		   uint32 dest_stride)
#else
PRIVATE int DFKsb2b(source, dest, num_elm, source_stride, dest_stride)
uint8 * source, * dest;
uint32 num_elm, source_stride, dest_stride;
#endif /* PROTOTYPE */
{
  int fast_processing = 0;              /* Default is not fast processing */
  int in_place = 0;                     /* Inplace must be detected */
  register uint32 i;            
  uint8 buf[2];                          /* Inplace processing buffer */
#ifdef PROTOTYPE
  uint8 * source = (uint8*)s;
  uint8 * dest = (uint8*)d;
#endif /* PROTOTYPE */
  char *FUNC="DFKsb2b";

  HEclear();

  if(num_elm == 0){                          /* No elements is an error. */
    HERROR(DFE_BADCONV);
    return FAIL;
  }

  /* Determine if faster array processing is appropriate */
  if(source_stride == 0 && dest_stride == 0)
    fast_processing = 1;

  /* Determine if the conversion should be inplace */
  if(source == dest)
    in_place = 1;

  if(fast_processing) 
    if(!in_place) {
      for(i = 0; i < num_elm; i++) {
        dest[0] = source[1];
        dest[1] = source[0];
        dest += 2;
        source += 2;
      }
      return 0;
    }
    else { 
      for(i = 0; i < num_elm; i++) {
        buf[0] = source[1];
        buf[1] = source[0];
        dest[0] = buf[0];
        dest[1] = buf[1];
        dest += 2;
        source += 2;
      }
      return 0;
    }
  
  /* Generic stride processing */
  if(!in_place)
    for(i = 0; i < num_elm; i++) {
      dest[0] = source[1];
      dest[1] = source[0];
      dest += dest_stride;
      source += source_stride;
    }
  else
    for(i = 0; i < num_elm; i++) {
      buf[0] = source[1];
      buf[1] = source[0];
      dest[0] = buf[0];
      dest[1] = buf[1];
      dest += dest_stride;
      source += source_stride;
    }
  return 0;
}

/************************************************************/
/* DFKsb4b()                                                */
/* -->Byte swapping for 4 byte data items                   */
/************************************************************/
#ifdef PROTOTYPE
PRIVATE int DFKsb4b(VOIDP s, VOIDP d, uint32 num_elm, uint32 source_stride,
		   uint32 dest_stride)
#else
PRIVATE int DFKsb4b(source, dest, num_elm, source_stride, dest_stride)
uint8 * source, * dest;
uint32 num_elm, source_stride, dest_stride;
#endif /* PROTOTYPE */
{
  int fast_processing = 0;              /* Default is not fast processing */
  int in_place = 0;                     /* Inplace must be detected */
  register uint32 i;            
  uint8 buf[4];                          /* Inplace processing buffer */
#ifdef PROTOTYPE
  uint8 * source = (uint8*)s;
  uint8 * dest = (uint8*)d;
#endif /* PROTOTYPE */
  char *FUNC="DFKsb4b";
  uint32 *lp_dest;
  uint32 *lp_src;

  HEclear();

  if(num_elm == 0){                         /* No elements is an error. */
    HERROR(DFE_BADCONV);
    return FAIL;
  }

  /* Determine if faster array processing is appropriate */
  if(source_stride == 0 && dest_stride == 0)
    fast_processing = 1;

  /* Determine if the conversion should be inplace */
  if(source == dest)
    in_place = 1;

  if(fast_processing) 
    if(!in_place) {
#ifndef DUFF_sb4b
#ifdef TEST1_sb4b
    source+=3;
#endif
#ifdef TEST3_sb4b
    lp_dest=(uint32 *)dest;
    lp_src=(uint32 *)source;
#endif
      for(i = 0; i < num_elm; i++) {
#if defined TEST3_sb4b
        *lp_dest++ = ((lp_src[0]&0x000000ff)<<24) |
                    ((lp_src[0]&0x0000ff00)<<8) |
                    ((lp_src[0]&0x00ff0000)>>8) |
                    ((lp_src[0]&0xff000000)>>24);
        lp_src++;
#elif defined TEST2_sb4b
        *dest++ = source[3];
        *dest++ = source[2];
        *dest++ = source[1];
        *dest++ = source[0];
        source += 4;
#elif defined TEST1_sb4b
        *dest++ = *source--;
        *dest++ = *source--;
        *dest++ = *source--;
        *dest++ = *source;
        source += 7;
#else
        dest[0] = source[3];
        dest[1] = source[2];
        dest[2] = source[1];
        dest[3] = source[0];
        dest += 4;
        source += 4;
#endif
      }
#else   /* DUFF_sb4b */
        register uint32 n=(num_elm+7)/8;

        switch(num_elm%8) {
            case 0:
                do{
                    dest[0] = source[3];
                    dest[1] = source[2];
                    dest[2] = source[1];
                    dest[3] = source[0];
                    dest += 4;
                    source += 4;
            case 7:
                    dest[0] = source[3];
                    dest[1] = source[2];
                    dest[2] = source[1];
                    dest[3] = source[0];
                    dest += 4;
                    source += 4;
            case 6:
                    dest[0] = source[3];
                    dest[1] = source[2];
                    dest[2] = source[1];
                    dest[3] = source[0];
                    dest += 4;
                    source += 4;
            case 5:
                    dest[0] = source[3];
                    dest[1] = source[2];
                    dest[2] = source[1];
                    dest[3] = source[0];
                    dest += 4;
                    source += 4;
            case 4:
                    dest[0] = source[3];
                    dest[1] = source[2];
                    dest[2] = source[1];
                    dest[3] = source[0];
                    dest += 4;
                    source += 4;
            case 3:
                    dest[0] = source[3];
                    dest[1] = source[2];
                    dest[2] = source[1];
                    dest[3] = source[0];
                    dest += 4;
                    source += 4;
            case 2:
                    dest[0] = source[3];
                    dest[1] = source[2];
                    dest[2] = source[1];
                    dest[3] = source[0];
                    dest += 4;
                    source += 4;
            case 1:
                    dest[0] = source[3];
                    dest[1] = source[2];
                    dest[2] = source[1];
                    dest[3] = source[0];
                    dest += 4;
                    source += 4;
                } while(--n>0);
		}
#endif  /* DUFF_sb4b */
      return 0;
    }
    else {
      for(i = 0; i < num_elm; i++) {
        buf[0] = source[3];
        buf[1] = source[2];
        buf[2] = source[1];
        buf[3] = source[0];
        dest[0] = buf[0];
        dest[1] = buf[1];
        dest[2] = buf[2];
        dest[3] = buf[3];
        dest += 4;
        source += 4;
      }
      return 0;
    }
  
  /* Generic stride processing */
  if(!in_place)
    for(i = 0; i < num_elm; i++) {
      dest[0] = source[3];
      dest[1] = source[2];
      dest[2] = source[1];
      dest[3] = source[0];
      dest += dest_stride;
      source += source_stride;
    }
  else
    for(i = 0; i < num_elm; i++) {
      buf[0] = source[3];
      buf[1] = source[2];
      buf[2] = source[1];
      buf[3] = source[0];
      dest[0] = buf[0];
      dest[1] = buf[1];
      dest[2] = buf[2];
      dest[3] = buf[3];
      dest += dest_stride;
      source += source_stride;
    }
  return 0;
}

/************************************************************/
/* DFKsb8b()                                                */
/* -->Byte swapping for 8 byte data items                   */
/************************************************************/
#ifdef PROTOTYPE
PRIVATE int DFKsb8b(VOIDP s, VOIDP d, uint32 num_elm, uint32 source_stride,
		   uint32 dest_stride)
#else
PRIVATE int DFKsb8b(source, dest, num_elm, source_stride, dest_stride)
uint8 * source, * dest;
uint32 num_elm, source_stride, dest_stride;
#endif /* PROTOTYPE */
{
  int fast_processing = 0;              /* Default is not fast processing */
  int in_place = 0;                     /* Inplace must be detected */
  register uint32 i;            
  uint8 buf[8];                          /* Inplace processing buffer */
#ifdef PROTOTYPE
  uint8 * source = (uint8*)s;
  uint8 * dest = (uint8*)d;
#endif /* PROTOTYPE */
    char *FUNC="DFKsb8b";

    HEclear();

  if(num_elm == 0){                         /* No elements is an error. */
    HERROR(DFE_BADCONV);
    return FAIL;
  }

  /* Determine if faster array processing is appropriate */
  if(source_stride == 0 && dest_stride == 0)
    fast_processing = 1;

  /* Determine if the conversion should be inplace */
  if(source == dest)
    in_place = 1;

  if(fast_processing) 
    if(!in_place) {
      for(i = 0; i < num_elm; i++) {
        dest[0] = source[7];
        dest[1] = source[6];
        dest[2] = source[5];
        dest[3] = source[4];
        dest[4] = source[3];
        dest[5] = source[2];
        dest[6] = source[1];
        dest[7] = source[0];
        dest += 8;
        source += 8;
      }
      return 0;
    }
    else {
      for(i = 0; i < num_elm; i++) {
        buf[0] = source[7];
        buf[1] = source[6];
        buf[2] = source[5];
        buf[3] = source[4];
        buf[4] = source[3];
        buf[5] = source[2];
        buf[6] = source[1];
        buf[7] = source[0];
        dest[0] = buf[0];
        dest[1] = buf[1];
        dest[2] = buf[2];
        dest[3] = buf[3];
        dest[4] = buf[4];
        dest[5] = buf[5];
        dest[6] = buf[6];
        dest[7] = buf[7];
        dest += 8;
        source += 8;
      }
      return 0;
    }
  
  /* Generic stride processing */
  if(!in_place)
    for(i = 0; i < num_elm; i++) {
      dest[0] = source[7];
      dest[1] = source[6];
      dest[2] = source[5];
      dest[3] = source[4];
      dest[4] = source[3];
      dest[5] = source[2];
      dest[6] = source[1];
      dest[7] = source[0];
      dest += dest_stride;
      source += source_stride;
    }
  else
    for(i = 0; i < num_elm; i++) {
      buf[0] = source[7];
      buf[1] = source[6];
      buf[2] = source[5];
      buf[3] = source[4];
      buf[4] = source[3];
      buf[5] = source[2];
      buf[6] = source[1];
      buf[7] = source[0];
      dest[0] = buf[0];
      dest[1] = buf[1];
      dest[2] = buf[2];
      dest[3] = buf[3];
      dest[4] = buf[4];
      dest[5] = buf[5];
      dest[6] = buf[6];
      dest[7] = buf[7];
      dest += dest_stride;
      source += source_stride;
    }

  return 0;
}

/*****************************************************************************/
/* NUMBER CONVERSION ROUTINES FOR THE UNICOS OPERATING SYSTEM                */
/* Parameter dest_stride is used because source buffer and dest buffer will  */
/* be different sizes for all data types except char.                        */
/*****************************************************************************/

/*****************************************************************************/
/*                                                                           */
/*   For all you CRAY lovers out there, I just thought I would tell you      */
/* how my routines work.  If you know of a better, or more efficient         */
/* method for converting any of these, please tell me.                       */
/*                                                                           */
/*   Mark Straka provided the routine to convert the 32 bit and 64 bit       */
/* Cray floats into the IEEE format and back.  In the previous release of    */
/* HDF, his routines were incorporated from Fortran source.  In this         */
/* release, the original C source has been incorporated.  Please note,       */
/* the Public Domain notice included at the top of this file pertains to     */
/* these routines as well.                                                   */
/*                                                                           */
/*****************************************************************************/

#if defined(UNICOS)

#define UI2I_MASKA  0xffff000000000000
#define UI2I_MASKB  0x0000ffff00000000
#define UI2I_MASKC  0x00000000ffff0000
#define UI2I_MASKD  0x000000000000ffff

/************************************************************/
/* DFKui2i()                                                */
/* -->Unicos routine for importing 2 byte data items        */ 
/* (**) This routine converts two byte IEEE to eight byte   */
/*      Cray big endian integer.                            */
/************************************************************/
#ifdef PROTOTYPE
PRIVATE int DFKui2i(VOIDP s, VOIDP d, uint32 num_elm, uint32 source_stride,
		   uint32 dest_stride)
#else
PRIVATE int DFKui2i(source, dest, num_elm, source_stride, dest_stride)
uint8 * source, * dest;
uint32 num_elm, source_stride, dest_stride;
#endif /* PROTOTYPE */
{
  register uint32 i;
  int fast_processing=0;
#ifdef PROTOTYPE
  uint8 * source = (uint8*)s;
  uint8 * dest = (uint8*)d;
#endif /* PROTOTYPE */
  long * lptr_dest = (long*)dest;
    long *lp_dest;
    unsigned long *lp_src;
  char *FUNC="DFKui2i";

  HEclear();

  if(source == dest || num_elm == 0) {  /* Inplace conversions not permitted */
    HERROR(DFE_BADCONV);                /* No elements is an error */
    return FAIL;
  }

  /* Find out if it is OK to use faster array processing */
  if(source_stride == 0 && dest_stride == 0) 
      fast_processing = 1;            

    if(fast_processing) {
#ifndef DUFF_ui2i
#if defined TEST2_ui2i
        int odd_man_out;        /* By default there are even num_elm */
        intn n;

        odd_man_out = num_elm%4;

        n=num_elm/4;
        lp_dest=(long *)dest;
        lp_src=(unsigned long *)source;
        HDmemset(lp_dest,0,num_elm*sizeof(long));
        for(i = 0; i < n; i++) {
            lp_dest[0]=(lp_src[0]&UI2I_MASKA)>>48;
            lp_dest[1]=(lp_src[0]&UI2I_MASKB)>>32;
            lp_dest[2]=(lp_src[0]&UI2I_MASKC)>>16;
            lp_dest[3]=lp_src[0]&UI2I_MASKD;
            lp_dest+=4;
            lp_src++;
          } /* end for */
        switch(odd_man_out) {
            case 3:
                lp_dest[0]=(lp_src[0]&UI2I_MASKA)>>48;
                lp_dest[1]=(lp_src[0]&UI2I_MASKB)>>32;
                lp_dest[2]=(lp_src[0]&UI2I_MASKC)>>16;
                break;

            case 2:
                lp_dest[0]=(lp_src[0]&UI2I_MASKA)>>48;
                lp_dest[1]=(lp_src[0]&UI2I_MASKB)>>32;
                break;

            case 1:
                lp_dest[0]=(lp_src[0]&UI2I_MASKA)>>48;
                break;

            default:
                break;
          } /* end switch */
#elif defined TEST1_ui2i
        int odd_man_out;        /* By default there are even num_elm */
        intn n;

        odd_man_out = num_elm%4;

        n=num_elm/4;
        lp_dest=(long *)dest;
        lp_src=(unsigned long *)source;
        HDmemset(lp_dest,0,num_elm*sizeof(long));
        for(i = 0; i < n; i++) {
            *lp_dest++=(lp_src[0]&UI2I_MASKA)>>48;
            *lp_dest++=(lp_src[0]&UI2I_MASKB)>>32;
            *lp_dest++=(lp_src[0]&UI2I_MASKC)>>16;
            *lp_dest++=lp_src[0]&UI2I_MASKD;
            lp_src++;
          } /* end for */
        switch(odd_man_out) {
            case 3:
                *lp_dest++=(lp_src[0]&UI2I_MASKA)>>48;
                *lp_dest++=(lp_src[0]&UI2I_MASKB)>>32;
                *lp_dest++=(lp_src[0]&UI2I_MASKC)>>16;
                break;

            case 2:
                *lp_dest++=(lp_src[0]&UI2I_MASKA)>>48;
                *lp_dest++=(lp_src[0]&UI2I_MASKB)>>32;
                break;

            case 1:
                *lp_dest++=(lp_src[0]&UI2I_MASKA)>>48;
                break;

            default:
                break;
          } /* end switch */
#else
    for(i = 0; i < num_elm; i++) {
      lptr_dest[0] = 0x0000000000000000;
      dest[6] = source[0];
      dest[7] = source[1];
      source += 2;
      lptr_dest++;
      dest = (uint8*)lptr_dest;
    }
#endif
#else   /* DUFF_ui2i */
        uintn n;
        int odd_man_out;        /* By default there are even num_elm */

        lp_dest=(long *)dest;
        lp_src=(unsigned long *)source;
        HDmemset(lp_dest,0,num_elm*sizeof(long));

        odd_man_out = num_elm%4;

        num_elm/=4;
        n=(num_elm+7)/8;
        switch(num_elm%8) {
            case 0:
                do{
                    lp_dest[0]=(lp_src[0]&UI2I_MASKA)>>48;
                    lp_dest[1]=(lp_src[0]&UI2I_MASKB)>>32;
                    lp_dest[2]=(lp_src[0]&UI2I_MASKC)>>16;
                    lp_dest[3]=lp_src[0]&UI2I_MASKD;
                    lp_dest+=4;
                    lp_src++;
#ifdef QAK
            case 15:
                    lp_dest[0]=(lp_src[0]&UI2I_MASKA)>>48;
                    lp_dest[1]=(lp_src[0]&UI2I_MASKB)>>32;
                    lp_dest[2]=(lp_src[0]&UI2I_MASKC)>>16;
                    lp_dest[3]=lp_src[0]&UI2I_MASKD;
                    lp_dest+=4;
                    lp_src++;
            case 14:
                    lp_dest[0]=(lp_src[0]&UI2I_MASKA)>>48;
                    lp_dest[1]=(lp_src[0]&UI2I_MASKB)>>32;
                    lp_dest[2]=(lp_src[0]&UI2I_MASKC)>>16;
                    lp_dest[3]=lp_src[0]&UI2I_MASKD;
                    lp_dest+=4;
                    lp_src++;
            case 13:
                    lp_dest[0]=(lp_src[0]&UI2I_MASKA)>>48;
                    lp_dest[1]=(lp_src[0]&UI2I_MASKB)>>32;
                    lp_dest[2]=(lp_src[0]&UI2I_MASKC)>>16;
                    lp_dest[3]=lp_src[0]&UI2I_MASKD;
                    lp_dest+=4;
                    lp_src++;
            case 12:
                    lp_dest[0]=(lp_src[0]&UI2I_MASKA)>>48;
                    lp_dest[1]=(lp_src[0]&UI2I_MASKB)>>32;
                    lp_dest[2]=(lp_src[0]&UI2I_MASKC)>>16;
                    lp_dest[3]=lp_src[0]&UI2I_MASKD;
                    lp_dest+=4;
                    lp_src++;
            case 11:
                    lp_dest[0]=(lp_src[0]&UI2I_MASKA)>>48;
                    lp_dest[1]=(lp_src[0]&UI2I_MASKB)>>32;
                    lp_dest[2]=(lp_src[0]&UI2I_MASKC)>>16;
                    lp_dest[3]=lp_src[0]&UI2I_MASKD;
                    lp_dest+=4;
                    lp_src++;
            case 10:
                    lp_dest[0]=(lp_src[0]&UI2I_MASKA)>>48;
                    lp_dest[1]=(lp_src[0]&UI2I_MASKB)>>32;
                    lp_dest[2]=(lp_src[0]&UI2I_MASKC)>>16;
                    lp_dest[3]=lp_src[0]&UI2I_MASKD;
                    lp_dest+=4;
                    lp_src++;
            case 9:
                    lp_dest[0]=(lp_src[0]&UI2I_MASKA)>>48;
                    lp_dest[1]=(lp_src[0]&UI2I_MASKB)>>32;
                    lp_dest[2]=(lp_src[0]&UI2I_MASKC)>>16;
                    lp_dest[3]=lp_src[0]&UI2I_MASKD;
                    lp_dest+=4;
                    lp_src++;
            case 8:
                    lp_dest[0]=(lp_src[0]&UI2I_MASKA)>>48;
                    lp_dest[1]=(lp_src[0]&UI2I_MASKB)>>32;
                    lp_dest[2]=(lp_src[0]&UI2I_MASKC)>>16;
                    lp_dest[3]=lp_src[0]&UI2I_MASKD;
                    lp_dest+=4;
                    lp_src++;
#endif
            case 7:
                    lp_dest[0]=(lp_src[0]&UI2I_MASKA)>>48;
                    lp_dest[1]=(lp_src[0]&UI2I_MASKB)>>32;
                    lp_dest[2]=(lp_src[0]&UI2I_MASKC)>>16;
                    lp_dest[3]=lp_src[0]&UI2I_MASKD;
                    lp_dest+=4;
                    lp_src++;
            case 6:
                    lp_dest[0]=(lp_src[0]&UI2I_MASKA)>>48;
                    lp_dest[1]=(lp_src[0]&UI2I_MASKB)>>32;
                    lp_dest[2]=(lp_src[0]&UI2I_MASKC)>>16;
                    lp_dest[3]=lp_src[0]&UI2I_MASKD;
                    lp_dest+=4;
                    lp_src++;
            case 5:
                    lp_dest[0]=(lp_src[0]&UI2I_MASKA)>>48;
                    lp_dest[1]=(lp_src[0]&UI2I_MASKB)>>32;
                    lp_dest[2]=(lp_src[0]&UI2I_MASKC)>>16;
                    lp_dest[3]=lp_src[0]&UI2I_MASKD;
                    lp_dest+=4;
                    lp_src++;
            case 4:
                    lp_dest[0]=(lp_src[0]&UI2I_MASKA)>>48;
                    lp_dest[1]=(lp_src[0]&UI2I_MASKB)>>32;
                    lp_dest[2]=(lp_src[0]&UI2I_MASKC)>>16;
                    lp_dest[3]=lp_src[0]&UI2I_MASKD;
                    lp_dest+=4;
                    lp_src++;
            case 3:
                    lp_dest[0]=(lp_src[0]&UI2I_MASKA)>>48;
                    lp_dest[1]=(lp_src[0]&UI2I_MASKB)>>32;
                    lp_dest[2]=(lp_src[0]&UI2I_MASKC)>>16;
                    lp_dest[3]=lp_src[0]&UI2I_MASKD;
                    lp_dest+=4;
                    lp_src++;
            case 2:
                    lp_dest[0]=(lp_src[0]&UI2I_MASKA)>>48;
                    lp_dest[1]=(lp_src[0]&UI2I_MASKB)>>32;
                    lp_dest[2]=(lp_src[0]&UI2I_MASKC)>>16;
                    lp_dest[3]=lp_src[0]&UI2I_MASKD;
                    lp_dest+=4;
                    lp_src++;
            case 1:
                    lp_dest[0]=(lp_src[0]&UI2I_MASKA)>>48;
                    lp_dest[1]=(lp_src[0]&UI2I_MASKB)>>32;
                    lp_dest[2]=(lp_src[0]&UI2I_MASKC)>>16;
                    lp_dest[3]=lp_src[0]&UI2I_MASKD;
                    lp_dest+=4;
                    lp_src++;
                } while(--n>0);
		}
        switch(odd_man_out) {
            case 3:
                lp_dest[0]=(lp_src[0]&UI2I_MASKA)>>48;
                lp_dest[1]=(lp_src[0]&UI2I_MASKB)>>32;
                lp_dest[2]=(lp_src[0]&UI2I_MASKC)>>16;
                break;

            case 2:
                lp_dest[0]=(lp_src[0]&UI2I_MASKA)>>48;
                lp_dest[1]=(lp_src[0]&UI2I_MASKB)>>32;
                break;

            case 1:
                lp_dest[0]=(lp_src[0]&UI2I_MASKA)>>48;
                break;

            default:
                break;
          } /* end switch */
#endif  /* DUFF_ui2i */
  }
  else { /* Generic stride processing */
    for(i = 0; i < num_elm; i++) {
      dest[0] = 0x00;
      dest[1] = 0x00;
      dest[2] = 0x00;
      dest[3] = 0x00;
      dest[4] = 0x00;
      dest[5] = 0x00;
      dest[6] = source[0];
      dest[7] = source[1];
      source += source_stride;
      dest += dest_stride;
    }
  }
  return 0;
}

#define UI2S_MASKA  0xffff000000000000
#define UI2S_MASKB  0x0000ffff00000000
#define UI2S_MASKC  0x00000000ffff0000
#define UI2S_MASKD  0x000000000000ffff
#define UI2S_MASKE  0x8000000000000000
#define UI2S_MASKF  0x0000800000000000
#define UI2S_MASKG  0x0000000080000000
#define UI2S_MASKH  0x0000000000008000

/************************************************************/
/* DFKui2s()                                                */
/* -->Unicos routine for importing 2 byte signed ints       */
/* (**) This routine converts two byte IEEE to eight byte   */
/*      Cray.                                               */
/************************************************************/
#ifdef PROTOTYPE
PRIVATE int DFKui2s(VOIDP s, VOIDP d, uint32 num_elm, uint32 source_stride,
		   uint32 dest_stride)
#else
PRIVATE int DFKui2s(source, dest, num_elm, source_stride, dest_stride)
uint8 * source, * dest;
uint32 num_elm, source_stride, dest_stride;
#endif /* PROTOTYPE */
{
  register uint32 i;
  int fast_processing=0;
#ifdef PROTOTYPE
  uint8 * source = (uint8*)s;
  uint8 * dest = (uint8*)d;
#endif /* PROTOTYPE */
  long * lptr_dest = (long*)dest;
    long *lp_dest;
    unsigned long *lp_src;
  char *FUNC="DFKui2s";

  HEclear();

  if(source == dest || num_elm == 0) {  /* Inplace conversions  not permitted */
    HERROR(DFE_BADCONV);                /* No elements to convert is an error */
    return FAIL;
  }

  /* Find out if it is OK to use faster array processing */
  if(source_stride == 0 && dest_stride == 0) 
      fast_processing = 1;            

  if(fast_processing) {
#ifndef DUFF_ui2s
#if defined TEST2_ui2s
        int odd_man_out;        /* By default there are even num_elm */
        intn n;

        odd_man_out = num_elm%4;

        n=num_elm/4;
        lp_dest=(long *)dest;
        lp_src=(unsigned long *)source;
        HDmemset(lp_dest,0,num_elm*sizeof(long));
        for(i = 0; i < n; i++) {
            if(lp_src[0] & UI2S_MASKE)      /* Can't forget to extend sign */
                lp_dest[0] = 0xffffffffffffffff;
            lp_dest[0]=(lp_src[0]&UI2I_MASKA)>>48;
            if(lp_src[0] & UI2S_MASKF)      /* Can't forget to extend sign */
                lp_dest[1] = 0xffffffffffffffff;
            lp_dest[1]=(lp_src[0]&UI2I_MASKB)>>32;
            if(lp_src[0] & UI2S_MASKG)      /* Can't forget to extend sign */
                lp_dest[2] = 0xffffffffffffffff;
            lp_dest[2]=(lp_src[0]&UI2I_MASKC)>>16;
            if(lp_src[0] & UI2S_MASKH)      /* Can't forget to extend sign */
                lp_dest[3] = 0xffffffffffffffff;
            lp_dest[3]=lp_src[0]&UI2I_MASKD;
            lp_dest+=4;
            lp_src++;
          } /* end for */
        switch(odd_man_out) {
            case 3:
                if(lp_src[0] & UI2S_MASKE)      /* Can't forget to extend sign */
                    lp_dest[0] = 0xffffffffffffffff;
                lp_dest[0]=(lp_src[0]&UI2I_MASKA)>>48;
                if(lp_src[0] & UI2S_MASKF)      /* Can't forget to extend sign */
                    lp_dest[1] = 0xffffffffffffffff;
                lp_dest[1]=(lp_src[0]&UI2I_MASKB)>>32;
                if(lp_src[0] & UI2S_MASKG)      /* Can't forget to extend sign */
                    lp_dest[2] = 0xffffffffffffffff;
                lp_dest[2]=(lp_src[0]&UI2I_MASKC)>>16;
                break;

            case 2:
                if(lp_src[0] & UI2S_MASKE)      /* Can't forget to extend sign */
                    lp_dest[0] = 0xffffffffffffffff;
                lp_dest[0]=(lp_src[0]&UI2I_MASKA)>>48;
                if(lp_src[0] & UI2S_MASKF)      /* Can't forget to extend sign */
                    lp_dest[1] = 0xffffffffffffffff;
                lp_dest[1]=(lp_src[0]&UI2I_MASKB)>>32;
                break;

            case 1:
                if(lp_src[0] & UI2S_MASKE)      /* Can't forget to extend sign */
                    lp_dest[0] = 0xffffffffffffffff;
                lp_dest[0]=(lp_src[0]&UI2I_MASKA)>>48;
                break;

            case 0:
                break;
          } /* end switch */
#elif defined TEST1_ui2s
        int odd_man_out;        /* By default there are even num_elm */
        intn n;

        odd_man_out = num_elm%4;

        n=num_elm/4;
        lp_dest=(long *)dest;
        lp_src=(unsigned long *)source;
        HDmemset(lp_dest,0,num_elm*sizeof(long));
        for(i = 0; i < n; i++) {
            if(lp_src[0] & UI2S_MASKE)      /* Can't forget to extend sign */
                *lp_dest = 0xffffffffffffffff;
            *lp_dest++=(lp_src[0]&UI2I_MASKA)>>48;
            if(lp_src[0] & UI2S_MASKF)      /* Can't forget to extend sign */
                *lp_dest = 0xffffffffffffffff;
            *lp_dest++=(lp_src[0]&UI2I_MASKB)>>32;
            if(lp_src[0] & UI2S_MASKG)      /* Can't forget to extend sign */
                *lp_dest = 0xffffffffffffffff;
            *lp_dest++=(lp_src[0]&UI2I_MASKC)>>16;
            if(lp_src[0] & UI2S_MASKH)      /* Can't forget to extend sign */
                *lp_dest = 0xffffffffffffffff;
            *lp_dest++=lp_src[0]&UI2I_MASKD;
            lp_src++;
          } /* end for */
        switch(odd_man_out) {
            case 3:
                if(lp_src[0] & UI2S_MASKE)      /* Can't forget to extend sign */
                    *lp_dest = 0xffffffffffffffff;
                *lp_dest++=(lp_src[0]&UI2I_MASKA)>>48;
                if(lp_src[0] & UI2S_MASKF)      /* Can't forget to extend sign */
                    *lp_dest = 0xffffffffffffffff;
                *lp_dest++=(lp_src[0]&UI2I_MASKB)>>32;
                if(lp_src[0] & UI2S_MASKG)      /* Can't forget to extend sign */
                    *lp_dest = 0xffffffffffffffff;
                *lp_dest++=(lp_src[0]&UI2I_MASKC)>>16;
                break;

            case 2:
                if(lp_src[0] & UI2S_MASKE)      /* Can't forget to extend sign */
                    *lp_dest = 0xffffffffffffffff;
                *lp_dest++=(lp_src[0]&UI2I_MASKA)>>48;
                if(lp_src[0] & UI2S_MASKF)      /* Can't forget to extend sign */
                    *lp_dest = 0xffffffffffffffff;
                *lp_dest++=(lp_src[0]&UI2I_MASKB)>>32;
                break;

            case 1:
                if(lp_src[0] & UI2S_MASKE)      /* Can't forget to extend sign */
                    *lp_dest = 0xffffffffffffffff;
                *lp_dest++=(lp_src[0]&UI2I_MASKA)>>48;
                break;

            case 0:
                break;
          } /* end switch */
#else
    for(i = 0; i < num_elm; i++) {
      if((source[0] & 0x80))           /* Can't forget to extend sign */
	lptr_dest[0] = 0xffffffffffffffff;
      else
	lptr_dest[0] = 0x0000000000000000;
      dest[6] = source[0];
      dest[7] = source[1];
      source += 2;
      lptr_dest++;
      dest = (uint8*)lptr_dest;
    }
#endif
#else   /* DUFF_ui2s */
        uintn n;
        int odd_man_out;        /* By default there are even num_elm */

        lp_dest=(long *)dest;
        lp_src=(unsigned long *)source;
        HDmemset(lp_dest,0,num_elm*sizeof(long));

        odd_man_out = num_elm%4;

        num_elm/=4;
        n=(num_elm+7)/8;
        switch(num_elm%8) {
            case 0:
                do{
                    if(lp_src[0] & UI2S_MASKE)      /* Can't forget to extend sign */
                        lp_dest[0] = 0xffffffffffffffff;
                    lp_dest[0]=(lp_src[0]&UI2I_MASKA)>>48;
                    if(lp_src[0] & UI2S_MASKF)      /* Can't forget to extend sign */
                        lp_dest[1] = 0xffffffffffffffff;
                    lp_dest[1]=(lp_src[0]&UI2I_MASKB)>>32;
                    if(lp_src[0] & UI2S_MASKG)      /* Can't forget to extend sign */
                        lp_dest[2] = 0xffffffffffffffff;
                    lp_dest[2]=(lp_src[0]&UI2I_MASKC)>>16;
                    if(lp_src[0] & UI2S_MASKH)      /* Can't forget to extend sign */
                        lp_dest[3] = 0xffffffffffffffff;
                    lp_dest[3]=lp_src[0]&UI2I_MASKD;
                    lp_dest+=4;
                    lp_src++;
#ifdef QAK
            case 15:
                    if(lp_src[0] & UI2S_MASKE)      /* Can't forget to extend sign */
                        lp_dest[0] = 0xffffffffffffffff;
                    lp_dest[0]=(lp_src[0]&UI2I_MASKA)>>48;
                    if(lp_src[0] & UI2S_MASKF)      /* Can't forget to extend sign */
                        lp_dest[1] = 0xffffffffffffffff;
                    lp_dest[1]=(lp_src[0]&UI2I_MASKB)>>32;
                    if(lp_src[0] & UI2S_MASKG)      /* Can't forget to extend sign */
                        lp_dest[2] = 0xffffffffffffffff;
                    lp_dest[2]=(lp_src[0]&UI2I_MASKC)>>16;
                    if(lp_src[0] & UI2S_MASKH)      /* Can't forget to extend sign */
                        lp_dest[3] = 0xffffffffffffffff;
                    lp_dest[3]=lp_src[0]&UI2I_MASKD;
                    lp_dest+=4;
                    lp_src++;
            case 14:
                    if(lp_src[0] & UI2S_MASKE)      /* Can't forget to extend sign */
                        lp_dest[0] = 0xffffffffffffffff;
                    lp_dest[0]=(lp_src[0]&UI2I_MASKA)>>48;
                    if(lp_src[0] & UI2S_MASKF)      /* Can't forget to extend sign */
                        lp_dest[1] = 0xffffffffffffffff;
                    lp_dest[1]=(lp_src[0]&UI2I_MASKB)>>32;
                    if(lp_src[0] & UI2S_MASKG)      /* Can't forget to extend sign */
                        lp_dest[2] = 0xffffffffffffffff;
                    lp_dest[2]=(lp_src[0]&UI2I_MASKC)>>16;
                    if(lp_src[0] & UI2S_MASKH)      /* Can't forget to extend sign */
                        lp_dest[3] = 0xffffffffffffffff;
                    lp_dest[3]=lp_src[0]&UI2I_MASKD;
                    lp_dest+=4;
                    lp_src++;
            case 13:
                    if(lp_src[0] & UI2S_MASKE)      /* Can't forget to extend sign */
                        lp_dest[0] = 0xffffffffffffffff;
                    lp_dest[0]=(lp_src[0]&UI2I_MASKA)>>48;
                    if(lp_src[0] & UI2S_MASKF)      /* Can't forget to extend sign */
                        lp_dest[1] = 0xffffffffffffffff;
                    lp_dest[1]=(lp_src[0]&UI2I_MASKB)>>32;
                    if(lp_src[0] & UI2S_MASKG)      /* Can't forget to extend sign */
                        lp_dest[2] = 0xffffffffffffffff;
                    lp_dest[2]=(lp_src[0]&UI2I_MASKC)>>16;
                    if(lp_src[0] & UI2S_MASKH)      /* Can't forget to extend sign */
                        lp_dest[3] = 0xffffffffffffffff;
                    lp_dest[3]=lp_src[0]&UI2I_MASKD;
                    lp_dest+=4;
                    lp_src++;
            case 12:
                    if(lp_src[0] & UI2S_MASKE)      /* Can't forget to extend sign */
                        lp_dest[0] = 0xffffffffffffffff;
                    lp_dest[0]=(lp_src[0]&UI2I_MASKA)>>48;
                    if(lp_src[0] & UI2S_MASKF)      /* Can't forget to extend sign */
                        lp_dest[1] = 0xffffffffffffffff;
                    lp_dest[1]=(lp_src[0]&UI2I_MASKB)>>32;
                    if(lp_src[0] & UI2S_MASKG)      /* Can't forget to extend sign */
                        lp_dest[2] = 0xffffffffffffffff;
                    lp_dest[2]=(lp_src[0]&UI2I_MASKC)>>16;
                    if(lp_src[0] & UI2S_MASKH)      /* Can't forget to extend sign */
                        lp_dest[3] = 0xffffffffffffffff;
                    lp_dest[3]=lp_src[0]&UI2I_MASKD;
                    lp_dest+=4;
                    lp_src++;
            case 11:
                    if(lp_src[0] & UI2S_MASKE)      /* Can't forget to extend sign */
                        lp_dest[0] = 0xffffffffffffffff;
                    lp_dest[0]=(lp_src[0]&UI2I_MASKA)>>48;
                    if(lp_src[0] & UI2S_MASKF)      /* Can't forget to extend sign */
                        lp_dest[1] = 0xffffffffffffffff;
                    lp_dest[1]=(lp_src[0]&UI2I_MASKB)>>32;
                    if(lp_src[0] & UI2S_MASKG)      /* Can't forget to extend sign */
                        lp_dest[2] = 0xffffffffffffffff;
                    lp_dest[2]=(lp_src[0]&UI2I_MASKC)>>16;
                    if(lp_src[0] & UI2S_MASKH)      /* Can't forget to extend sign */
                        lp_dest[3] = 0xffffffffffffffff;
                    lp_dest[3]=lp_src[0]&UI2I_MASKD;
                    lp_dest+=4;
                    lp_src++;
            case 10:
                    if(lp_src[0] & UI2S_MASKE)      /* Can't forget to extend sign */
                        lp_dest[0] = 0xffffffffffffffff;
                    lp_dest[0]=(lp_src[0]&UI2I_MASKA)>>48;
                    if(lp_src[0] & UI2S_MASKF)      /* Can't forget to extend sign */
                        lp_dest[1] = 0xffffffffffffffff;
                    lp_dest[1]=(lp_src[0]&UI2I_MASKB)>>32;
                    if(lp_src[0] & UI2S_MASKG)      /* Can't forget to extend sign */
                        lp_dest[2] = 0xffffffffffffffff;
                    lp_dest[2]=(lp_src[0]&UI2I_MASKC)>>16;
                    if(lp_src[0] & UI2S_MASKH)      /* Can't forget to extend sign */
                        lp_dest[3] = 0xffffffffffffffff;
                    lp_dest[3]=lp_src[0]&UI2I_MASKD;
                    lp_dest+=4;
                    lp_src++;
            case 9:
                    if(lp_src[0] & UI2S_MASKE)      /* Can't forget to extend sign */
                        lp_dest[0] = 0xffffffffffffffff;
                    lp_dest[0]=(lp_src[0]&UI2I_MASKA)>>48;
                    if(lp_src[0] & UI2S_MASKF)      /* Can't forget to extend sign */
                        lp_dest[1] = 0xffffffffffffffff;
                    lp_dest[1]=(lp_src[0]&UI2I_MASKB)>>32;
                    if(lp_src[0] & UI2S_MASKG)      /* Can't forget to extend sign */
                        lp_dest[2] = 0xffffffffffffffff;
                    lp_dest[2]=(lp_src[0]&UI2I_MASKC)>>16;
                    if(lp_src[0] & UI2S_MASKH)      /* Can't forget to extend sign */
                        lp_dest[3] = 0xffffffffffffffff;
                    lp_dest[3]=lp_src[0]&UI2I_MASKD;
                    lp_dest+=4;
                    lp_src++;
            case 8:
                    if(lp_src[0] & UI2S_MASKE)      /* Can't forget to extend sign */
                        lp_dest[0] = 0xffffffffffffffff;
                    lp_dest[0]=(lp_src[0]&UI2I_MASKA)>>48;
                    if(lp_src[0] & UI2S_MASKF)      /* Can't forget to extend sign */
                        lp_dest[1] = 0xffffffffffffffff;
                    lp_dest[1]=(lp_src[0]&UI2I_MASKB)>>32;
                    if(lp_src[0] & UI2S_MASKG)      /* Can't forget to extend sign */
                        lp_dest[2] = 0xffffffffffffffff;
                    lp_dest[2]=(lp_src[0]&UI2I_MASKC)>>16;
                    if(lp_src[0] & UI2S_MASKH)      /* Can't forget to extend sign */
                        lp_dest[3] = 0xffffffffffffffff;
                    lp_dest[3]=lp_src[0]&UI2I_MASKD;
                    lp_dest+=4;
                    lp_src++;
#endif
            case 7:
                    if(lp_src[0] & UI2S_MASKE)      /* Can't forget to extend sign */
                        lp_dest[0] = 0xffffffffffffffff;
                    lp_dest[0]=(lp_src[0]&UI2I_MASKA)>>48;
                    if(lp_src[0] & UI2S_MASKF)      /* Can't forget to extend sign */
                        lp_dest[1] = 0xffffffffffffffff;
                    lp_dest[1]=(lp_src[0]&UI2I_MASKB)>>32;
                    if(lp_src[0] & UI2S_MASKG)      /* Can't forget to extend sign */
                        lp_dest[2] = 0xffffffffffffffff;
                    lp_dest[2]=(lp_src[0]&UI2I_MASKC)>>16;
                    if(lp_src[0] & UI2S_MASKH)      /* Can't forget to extend sign */
                        lp_dest[3] = 0xffffffffffffffff;
                    lp_dest[3]=lp_src[0]&UI2I_MASKD;
                    lp_dest+=4;
                    lp_src++;
            case 6:
                    if(lp_src[0] & UI2S_MASKE)      /* Can't forget to extend sign */
                        lp_dest[0] = 0xffffffffffffffff;
                    lp_dest[0]=(lp_src[0]&UI2I_MASKA)>>48;
                    if(lp_src[0] & UI2S_MASKF)      /* Can't forget to extend sign */
                        lp_dest[1] = 0xffffffffffffffff;
                    lp_dest[1]=(lp_src[0]&UI2I_MASKB)>>32;
                    if(lp_src[0] & UI2S_MASKG)      /* Can't forget to extend sign */
                        lp_dest[2] = 0xffffffffffffffff;
                    lp_dest[2]=(lp_src[0]&UI2I_MASKC)>>16;
                    if(lp_src[0] & UI2S_MASKH)      /* Can't forget to extend sign */
                        lp_dest[3] = 0xffffffffffffffff;
                    lp_dest[3]=lp_src[0]&UI2I_MASKD;
                    lp_dest+=4;
                    lp_src++;
            case 5:
                    if(lp_src[0] & UI2S_MASKE)      /* Can't forget to extend sign */
                        lp_dest[0] = 0xffffffffffffffff;
                    lp_dest[0]=(lp_src[0]&UI2I_MASKA)>>48;
                    if(lp_src[0] & UI2S_MASKF)      /* Can't forget to extend sign */
                        lp_dest[1] = 0xffffffffffffffff;
                    lp_dest[1]=(lp_src[0]&UI2I_MASKB)>>32;
                    if(lp_src[0] & UI2S_MASKG)      /* Can't forget to extend sign */
                        lp_dest[2] = 0xffffffffffffffff;
                    lp_dest[2]=(lp_src[0]&UI2I_MASKC)>>16;
                    if(lp_src[0] & UI2S_MASKH)      /* Can't forget to extend sign */
                        lp_dest[3] = 0xffffffffffffffff;
                    lp_dest[3]=lp_src[0]&UI2I_MASKD;
                    lp_dest+=4;
                    lp_src++;
            case 4:
                    if(lp_src[0] & UI2S_MASKE)      /* Can't forget to extend sign */
                        lp_dest[0] = 0xffffffffffffffff;
                    lp_dest[0]=(lp_src[0]&UI2I_MASKA)>>48;
                    if(lp_src[0] & UI2S_MASKF)      /* Can't forget to extend sign */
                        lp_dest[1] = 0xffffffffffffffff;
                    lp_dest[1]=(lp_src[0]&UI2I_MASKB)>>32;
                    if(lp_src[0] & UI2S_MASKG)      /* Can't forget to extend sign */
                        lp_dest[2] = 0xffffffffffffffff;
                    lp_dest[2]=(lp_src[0]&UI2I_MASKC)>>16;
                    if(lp_src[0] & UI2S_MASKH)      /* Can't forget to extend sign */
                        lp_dest[3] = 0xffffffffffffffff;
                    lp_dest[3]=lp_src[0]&UI2I_MASKD;
                    lp_dest+=4;
                    lp_src++;
            case 3:
                    if(lp_src[0] & UI2S_MASKE)      /* Can't forget to extend sign */
                        lp_dest[0] = 0xffffffffffffffff;
                    lp_dest[0]=(lp_src[0]&UI2I_MASKA)>>48;
                    if(lp_src[0] & UI2S_MASKF)      /* Can't forget to extend sign */
                        lp_dest[1] = 0xffffffffffffffff;
                    lp_dest[1]=(lp_src[0]&UI2I_MASKB)>>32;
                    if(lp_src[0] & UI2S_MASKG)      /* Can't forget to extend sign */
                        lp_dest[2] = 0xffffffffffffffff;
                    lp_dest[2]=(lp_src[0]&UI2I_MASKC)>>16;
                    if(lp_src[0] & UI2S_MASKH)      /* Can't forget to extend sign */
                        lp_dest[3] = 0xffffffffffffffff;
                    lp_dest[3]=lp_src[0]&UI2I_MASKD;
                    lp_dest+=4;
                    lp_src++;
            case 2:
                    if(lp_src[0] & UI2S_MASKE)      /* Can't forget to extend sign */
                        lp_dest[0] = 0xffffffffffffffff;
                    lp_dest[0]=(lp_src[0]&UI2I_MASKA)>>48;
                    if(lp_src[0] & UI2S_MASKF)      /* Can't forget to extend sign */
                        lp_dest[1] = 0xffffffffffffffff;
                    lp_dest[1]=(lp_src[0]&UI2I_MASKB)>>32;
                    if(lp_src[0] & UI2S_MASKG)      /* Can't forget to extend sign */
                        lp_dest[2] = 0xffffffffffffffff;
                    lp_dest[2]=(lp_src[0]&UI2I_MASKC)>>16;
                    if(lp_src[0] & UI2S_MASKH)      /* Can't forget to extend sign */
                        lp_dest[3] = 0xffffffffffffffff;
                    lp_dest[3]=lp_src[0]&UI2I_MASKD;
                    lp_dest+=4;
                    lp_src++;
            case 1:
                    if(lp_src[0] & UI2S_MASKE)      /* Can't forget to extend sign */
                        lp_dest[0] = 0xffffffffffffffff;
                    lp_dest[0]=(lp_src[0]&UI2I_MASKA)>>48;
                    if(lp_src[0] & UI2S_MASKF)      /* Can't forget to extend sign */
                        lp_dest[1] = 0xffffffffffffffff;
                    lp_dest[1]=(lp_src[0]&UI2I_MASKB)>>32;
                    if(lp_src[0] & UI2S_MASKG)      /* Can't forget to extend sign */
                        lp_dest[2] = 0xffffffffffffffff;
                    lp_dest[2]=(lp_src[0]&UI2I_MASKC)>>16;
                    if(lp_src[0] & UI2S_MASKH)      /* Can't forget to extend sign */
                        lp_dest[3] = 0xffffffffffffffff;
                    lp_dest[3]=lp_src[0]&UI2I_MASKD;
                    lp_dest+=4;
                    lp_src++;
                } while(--n>0);
		}
        switch(odd_man_out) {
            case 3:
                if(lp_src[0] & UI2S_MASKE)      /* Can't forget to extend sign */
                    lp_dest[0] = 0xffffffffffffffff;
                lp_dest[0]=(lp_src[0]&UI2I_MASKA)>>48;
                if(lp_src[0] & UI2S_MASKF)      /* Can't forget to extend sign */
                    lp_dest[1] = 0xffffffffffffffff;
                lp_dest[1]=(lp_src[0]&UI2I_MASKB)>>32;
                if(lp_src[0] & UI2S_MASKG)      /* Can't forget to extend sign */
                    lp_dest[2] = 0xffffffffffffffff;
                lp_dest[2]=(lp_src[0]&UI2I_MASKC)>>16;
                break;

            case 2:
                if(lp_src[0] & UI2S_MASKE)      /* Can't forget to extend sign */
                    lp_dest[0] = 0xffffffffffffffff;
                lp_dest[0]=(lp_src[0]&UI2I_MASKA)>>48;
                if(lp_src[0] & UI2S_MASKF)      /* Can't forget to extend sign */
                    lp_dest[1] = 0xffffffffffffffff;
                lp_dest[1]=(lp_src[0]&UI2I_MASKB)>>32;
                break;

            case 1:
                if(lp_src[0] & UI2S_MASKE)      /* Can't forget to extend sign */
                    lp_dest[0] = 0xffffffffffffffff;
                lp_dest[0]=(lp_src[0]&UI2I_MASKA)>>48;
                break;

            case 0:
                break;
          } /* end switch */
#endif  /* DUFF_ui2s */
  }
  else { /* Generic stride processing */
    for(i = 0; i < num_elm; i++) {
      if((source[0] & 0x80)) {          /* Can't forget to extend sign */
	dest[0] = 0xff;
	dest[1] = 0xff;
	dest[2] = 0xff;
	dest[3] = 0xff;
	dest[4] = 0xff;
	dest[5] = 0xff;
      }
      else {
	dest[0] = 0x00;
	dest[1] = 0x00;
	dest[2] = 0x00;
	dest[3] = 0x00;
	dest[4] = 0x00;
	dest[5] = 0x00;
      }
      dest[6] = source[0];
      dest[7] = source[1];
      source += source_stride;
      dest += dest_stride;
    }
  }
  return 0;
}

#define UO2I_MASK 0x000000000000ffff

/************************************************************/
/* DFKuo2i()                                                */
/* -->Unicos routine for exporting 2 byte data items        */ 
/************************************************************/
#ifdef PROTOTYPE
PRIVATE int DFKuo2i(VOIDP s, VOIDP d, uint32 num_elm, uint32 source_stride,
		   uint32 dest_stride)
#else
PRIVATE int DFKuo2i(source, dest, num_elm, source_stride, dest_stride)
uint8 * source, * dest;
uint32 num_elm, source_stride, dest_stride;
#endif /* PROTOTYPE */
{
  register uintn i;
  int fast_processing=0;
#ifdef PROTOTYPE
  uint8 * source = (uint8*)s;
  uint8 * dest = (uint8*)d;
#endif /* PROTOTYPE */
    long *lp_dest;
    long *lp_src;
    char *FUNC="DFKuo2i";

    HEclear();

  if(source == dest || num_elm == 0) {  /* Inplace conversions  not permitted */
    HERROR(DFE_BADCONV);                /* No elements to convert is an error */
    return FAIL;
  }

  /* Find out if it is OK to use faster array processing */
  if(source_stride == 0 && dest_stride == 0)
      fast_processing = 1;            

  if(fast_processing) {
#ifndef DUFF_uo2i
#if defined TEST1_uo2i
    int odd_man_out;        /* By default there are even num_elm */
    intn n;

    odd_man_out = num_elm%4;

    n=num_elm/4;
    lp_dest=(long *)dest;
    lp_src=(long *)source;
    for(i = 0; i < n; i++) {
        *lp_dest++=((lp_src[0]&UO2I_MASK)<<48) |
                    ((lp_src[1]&UO2I_MASK)<<32) |
                    ((lp_src[2]&UO2I_MASK)<<16) |
                    (lp_src[3]&UO2I_MASK);
        lp_src+=4;
    }
    switch(odd_man_out) {   /* clean up leftovers */
        case 3:
            *lp_dest=((lp_src[0]&UO2I_MASK)<<48) |
                        ((lp_src[1]&UO2I_MASK)<<32) |
                        ((lp_src[2]&UO2I_MASK)<<16);
            break;

        case 2:
            *lp_dest=((lp_src[0]&UO2I_MASK)<<48) |
                        ((lp_src[1]&UO2I_MASK)<<32);
            break;

        case 1:
            *lp_dest=(lp_src[0]&UO2I_MASK)<<48;
            break;

        default:
            break;
      } /* end switch */
#else
    for(i = 0; i < num_elm; i++) {
      dest[0] = source[6];
      dest[1] = source[7];
      dest += 2;
      source += 8;
    }
#endif
#else   /* DUFF_uo2i */
        uintn n;
        int odd_man_out;        /* By default there are even num_elm */

        odd_man_out = num_elm%4;

        num_elm/=4;
        n=(num_elm+7)/8;
        lp_dest=(long *)dest;
        lp_src=(long *)source;
        switch(num_elm%8) {
            case 0:
                do{
                    *lp_dest++=((lp_src[0]&UO2I_MASK)<<48) |
                                ((lp_src[1]&UO2I_MASK)<<32) |
                                ((lp_src[2]&UO2I_MASK)<<16) |
                                (lp_src[3]&UO2I_MASK);
                    lp_src+=4;
#ifdef QAK
            case 15:
                    *lp_dest++=((lp_src[0]&UO2I_MASK)<<48) |
                                ((lp_src[1]&UO2I_MASK)<<32) |
                                ((lp_src[2]&UO2I_MASK)<<16) |
                                (lp_src[3]&UO2I_MASK);
                    lp_src+=4;
            case 14:
                    *lp_dest++=((lp_src[0]&UO2I_MASK)<<48) |
                                ((lp_src[1]&UO2I_MASK)<<32) |
                                ((lp_src[2]&UO2I_MASK)<<16) |
                                (lp_src[3]&UO2I_MASK);
                    lp_src+=4;
            case 13:
                    *lp_dest++=((lp_src[0]&UO2I_MASK)<<48) |
                                ((lp_src[1]&UO2I_MASK)<<32) |
                                ((lp_src[2]&UO2I_MASK)<<16) |
                                (lp_src[3]&UO2I_MASK);
                    lp_src+=4;
            case 12:
                    *lp_dest++=((lp_src[0]&UO2I_MASK)<<48) |
                                ((lp_src[1]&UO2I_MASK)<<32) |
                                ((lp_src[2]&UO2I_MASK)<<16) |
                                (lp_src[3]&UO2I_MASK);
                    lp_src+=4;
            case 11:
                    *lp_dest++=((lp_src[0]&UO2I_MASK)<<48) |
                                ((lp_src[1]&UO2I_MASK)<<32) |
                                ((lp_src[2]&UO2I_MASK)<<16) |
                                (lp_src[3]&UO2I_MASK);
                    lp_src+=4;
            case 10:
                    *lp_dest++=((lp_src[0]&UO2I_MASK)<<48) |
                                ((lp_src[1]&UO2I_MASK)<<32) |
                                ((lp_src[2]&UO2I_MASK)<<16) |
                                (lp_src[3]&UO2I_MASK);
                    lp_src+=4;
            case 9:
                    *lp_dest++=((lp_src[0]&UO2I_MASK)<<48) |
                                ((lp_src[1]&UO2I_MASK)<<32) |
                                ((lp_src[2]&UO2I_MASK)<<16) |
                                (lp_src[3]&UO2I_MASK);
                    lp_src+=4;
            case 8:
                    *lp_dest++=((lp_src[0]&UO2I_MASK)<<48) |
                                ((lp_src[1]&UO2I_MASK)<<32) |
                                ((lp_src[2]&UO2I_MASK)<<16) |
                                (lp_src[3]&UO2I_MASK);
                    lp_src+=4;
#endif
            case 7:
                    *lp_dest++=((lp_src[0]&UO2I_MASK)<<48) |
                                ((lp_src[1]&UO2I_MASK)<<32) |
                                ((lp_src[2]&UO2I_MASK)<<16) |
                                (lp_src[3]&UO2I_MASK);
                    lp_src+=4;
            case 6:
                    *lp_dest++=((lp_src[0]&UO2I_MASK)<<48) |
                                ((lp_src[1]&UO2I_MASK)<<32) |
                                ((lp_src[2]&UO2I_MASK)<<16) |
                                (lp_src[3]&UO2I_MASK);
                    lp_src+=4;
            case 5:
                    *lp_dest++=((lp_src[0]&UO2I_MASK)<<48) |
                                ((lp_src[1]&UO2I_MASK)<<32) |
                                ((lp_src[2]&UO2I_MASK)<<16) |
                                (lp_src[3]&UO2I_MASK);
                    lp_src+=4;
            case 4:
                    *lp_dest++=((lp_src[0]&UO2I_MASK)<<48) |
                                ((lp_src[1]&UO2I_MASK)<<32) |
                                ((lp_src[2]&UO2I_MASK)<<16) |
                                (lp_src[3]&UO2I_MASK);
                    lp_src+=4;
            case 3:
                    *lp_dest++=((lp_src[0]&UO2I_MASK)<<48) |
                                ((lp_src[1]&UO2I_MASK)<<32) |
                                ((lp_src[2]&UO2I_MASK)<<16) |
                                (lp_src[3]&UO2I_MASK);
                    lp_src+=4;
            case 2:
                    *lp_dest++=((lp_src[0]&UO2I_MASK)<<48) |
                                ((lp_src[1]&UO2I_MASK)<<32) |
                                ((lp_src[2]&UO2I_MASK)<<16) |
                                (lp_src[3]&UO2I_MASK);
                    lp_src+=4;
            case 1:
                    *lp_dest++=((lp_src[0]&UO2I_MASK)<<48) |
                                ((lp_src[1]&UO2I_MASK)<<32) |
                                ((lp_src[2]&UO2I_MASK)<<16) |
                                (lp_src[3]&UO2I_MASK);
                    lp_src+=4;
                } while(--n>0);
		}

        switch(odd_man_out) {   /* clean up leftovers */
            case 3:
                *lp_dest=((lp_src[0]&UO2I_MASK)<<48) |
                            ((lp_src[1]&UO2I_MASK)<<32) |
                            ((lp_src[2]&UO2I_MASK)<<16);
                break;

            case 2:
                *lp_dest=((lp_src[0]&UO2I_MASK)<<48) |
                            ((lp_src[1]&UO2I_MASK)<<32);
                break;

            case 1:
                *lp_dest=(lp_src[0]&UO2I_MASK)<<48;
                break;

            default:
                break;
          } /* end switch */
#endif  /* DUFF_uo2i */
  }
  else { /* Generic Stride processing */
    for(i = 0; i < num_elm; i++){
      dest[0] = source[6];
      dest[1] = source[7];
      source += source_stride;
      dest += dest_stride;
    }
  }
  return 0;
}

#define UI4I_MASKA 0xffffffff00000000
#define UI4I_MASKB 0x00000000ffffffff

/************************************************************/
/* DFKui4i()                                                */
/* -->Unicos routine for importing 4 byte unsigned ints     */
/************************************************************/
#ifdef PROTOTYPE
PRIVATE int DFKui4i(VOIDP s, VOIDP d, uint32 num_elm, uint32 source_stride,
		   uint32 dest_stride)
#else
PRIVATE int DFKui4i(source, dest, num_elm, source_stride, dest_stride)
uint8 * source, * dest;
uint32 num_elm, source_stride, dest_stride;
#endif /* PROTOTYPE */
{
  int fast_processing=0;
  register uint32 i;
#ifdef PROTOTYPE
  uint8 * source = (uint8*)s;
  uint8 * dest = (uint8*)d;
#endif /* PROTOTYPE */
  long * lptr_dest = (long*)dest;
    long *lp_dest;
    unsigned long *lp_src;
  char *FUNC="DFKui4i";

  HEclear();

  if(source == dest || num_elm == 0) {  /* Inplace conversions  not permitted */
    HERROR(DFE_BADCONV);                /* No elements to convert is an error */
    return FAIL;
  }

    if(source_stride == 0 && dest_stride == 0)
        fast_processing = 1;
  
    if(fast_processing) {
#ifndef DUFF_ui4i
#if defined TEST2_ui4i
        int odd_man_out;        /* By default there are even num_elm */
        intn n;

        odd_man_out = num_elm % 2;

        n=num_elm/2;
        lp_dest=(long *)dest;
        lp_src=(unsigned long *)source;
        HDmemset(lp_dest,0,num_elm*sizeof(long)); /* initialize to zeros */
        for(i = 0; i < n; i++) {
            lp_dest[0]=(lp_src[0]&UI4I_MASKA)>>32;
            lp_dest[1]=lp_src[0]&UI4I_MASKB;
            lp_dest+=2;
            lp_src++;
          } /* end for */
        if(odd_man_out)
            *lp_dest=(lp_src[0]&UI4I_MASKA)>>32;
#elif defined TEST1_ui4i
        int odd_man_out;        /* By default there are even num_elm */
        intn n;

        odd_man_out = num_elm % 2;

        n=num_elm/2;
        lp_dest=(long *)dest;
        lp_src=(unsigned long *)source;
        HDmemset(lp_dest,0,num_elm*sizeof(long)); /* initialize to zeros */
        for(i = 0; i < n; i++) {
            *lp_dest++=(lp_src[0]&UI4I_MASKA)>>32;
            *lp_dest++=lp_src[0]&UI4I_MASKB;
            lp_src++;
          } /* end for */
        if(odd_man_out)
            *lp_dest++=(lp_src[0]&UI4I_MASKA)>>32;
#else
        for(i = 0; i < num_elm; i++) {
            lptr_dest[0]=0;
            dest[4] = source[0];
            dest[5] = source[1];
            dest[6] = source[2];
            dest[7] = source[3];
            source += 4;
            lptr_dest ++;
            dest = (uint8 *)lptr_dest;
          } /* end for */
#endif
#else   /* DUFF_ui4i */
        uintn n;
        int odd_man_out;        /* By default there are even num_elm */

        lp_dest=(long *)dest;
        lp_src=(unsigned long *)source;
        HDmemset(lp_dest,0,num_elm*sizeof(long)); /* initialize to zeros */

        odd_man_out = num_elm % 2;

        num_elm/=2;
        n=(num_elm+7)/8;
        switch(num_elm%8) {
            case 0:
                do{
                    lp_dest[0]=(lp_src[0]&UI4I_MASKA)>>32;
                    lp_dest[1]=lp_src[0]&UI4I_MASKB;
                    lp_dest+=2;
                    lp_src++;
#ifdef QAK
            case 15:
                    lp_dest[0]=(lp_src[0]&UI4I_MASKA)>>32;
                    lp_dest[1]=lp_src[0]&UI4I_MASKB;
                    lp_dest+=2;
                    lp_src++;
            case 14:
                    lp_dest[0]=(lp_src[0]&UI4I_MASKA)>>32;
                    lp_dest[1]=lp_src[0]&UI4I_MASKB;
                    lp_dest+=2;
                    lp_src++;
            case 13:
                    lp_dest[0]=(lp_src[0]&UI4I_MASKA)>>32;
                    lp_dest[1]=lp_src[0]&UI4I_MASKB;
                    lp_dest+=2;
                    lp_src++;
            case 12:
                    lp_dest[0]=(lp_src[0]&UI4I_MASKA)>>32;
                    lp_dest[1]=lp_src[0]&UI4I_MASKB;
                    lp_dest+=2;
                    lp_src++;
            case 11:
                    lp_dest[0]=(lp_src[0]&UI4I_MASKA)>>32;
                    lp_dest[1]=lp_src[0]&UI4I_MASKB;
                    lp_dest+=2;
                    lp_src++;
            case 10:
                    lp_dest[0]=(lp_src[0]&UI4I_MASKA)>>32;
                    lp_dest[1]=lp_src[0]&UI4I_MASKB;
                    lp_dest+=2;
                    lp_src++;
            case 9:
                    lp_dest[0]=(lp_src[0]&UI4I_MASKA)>>32;
                    lp_dest[1]=lp_src[0]&UI4I_MASKB;
                    lp_dest+=2;
                    lp_src++;
            case 8:
                    lp_dest[0]=(lp_src[0]&UI4I_MASKA)>>32;
                    lp_dest[1]=lp_src[0]&UI4I_MASKB;
                    lp_dest+=2;
                    lp_src++;
#endif
            case 7:
                    lp_dest[0]=(lp_src[0]&UI4I_MASKA)>>32;
                    lp_dest[1]=lp_src[0]&UI4I_MASKB;
                    lp_dest+=2;
                    lp_src++;
            case 6:
                    lp_dest[0]=(lp_src[0]&UI4I_MASKA)>>32;
                    lp_dest[1]=lp_src[0]&UI4I_MASKB;
                    lp_dest+=2;
                    lp_src++;
            case 5:
                    lp_dest[0]=(lp_src[0]&UI4I_MASKA)>>32;
                    lp_dest[1]=lp_src[0]&UI4I_MASKB;
                    lp_dest+=2;
                    lp_src++;
            case 4:
                    lp_dest[0]=(lp_src[0]&UI4I_MASKA)>>32;
                    lp_dest[1]=lp_src[0]&UI4I_MASKB;
                    lp_dest+=2;
                    lp_src++;
            case 3:
                    lp_dest[0]=(lp_src[0]&UI4I_MASKA)>>32;
                    lp_dest[1]=lp_src[0]&UI4I_MASKB;
                    lp_dest+=2;
                    lp_src++;
            case 2:
                    lp_dest[0]=(lp_src[0]&UI4I_MASKA)>>32;
                    lp_dest[1]=lp_src[0]&UI4I_MASKB;
                    lp_dest+=2;
                    lp_src++;
            case 1:
                    lp_dest[0]=(lp_src[0]&UI4I_MASKA)>>32;
                    lp_dest[1]=lp_src[0]&UI4I_MASKB;
                    lp_dest+=2;
                    lp_src++;
                } while(--n>0);
		}
        if(odd_man_out)
            *lp_dest=(lp_src[0]&UI4I_MASKA)>>32;
#endif  /* DUFF_ui4i */
      } /* end if */
    else {
        for(i = 0; i < num_elm; i++) {
            dest[0] = 0;
            dest[1] = 0;
            dest[2] = 0;
            dest[3] = 0;
            dest[4] = source[0];
            dest[5] = source[1];
            dest[6] = source[2];
            dest[7] = source[3];
            dest += dest_stride;
            source += source_stride;
          } /* end for */
      } /* end else */
  return 0;
}

#define UI4S_MASKA 0xffffffff00000000
#define UI4S_MASKB 0x00000000ffffffff
#define UI4S_MASKC 0x8000000000000000
#define UI4S_MASKD 0x0000000080000000

/************************************************************/
/* DFKui4s()                                                */
/* -->Unicos routine for importing 4 signed ints            */ 
/************************************************************/
#ifdef PROTOTYPE
PRIVATE int DFKui4s(VOIDP s, VOIDP d, uint32 num_elm, uint32 source_stride,
		   uint32 dest_stride)
#else
PRIVATE int DFKui4s(source, dest, num_elm, source_stride, dest_stride)
uint8 * source, * dest;
uint32 num_elm, source_stride, dest_stride;
#endif /* PROTOTYPE */
{
  int fast_processing=0;
  register uint32 i;
#ifdef PROTOTYPE
  uint8 * source = (uint8*)s;
  uint8 * dest = (uint8*)d;
#endif /* PROTOTYPE */
  long * lptr_dest = (long*)dest;
    long *lp_dest;
    long *lp_src;
  char *FUNC="DFKui4s";

  HEclear();

  if(source == dest || num_elm == 0) {  /* Inplace conversions  not permitted */
    HERROR(DFE_BADCONV);                /* No elements to convert is an error */
    return FAIL;
  }

  if(source_stride == 0 && dest_stride == 0)
    fast_processing = 1;
  
    if(fast_processing) {
#ifndef DUFF_ui4s
#if defined TEST2_ui4s
        int odd_man_out;        /* By default there are even num_elm */
        intn n;

        odd_man_out = num_elm % 2;

        n=num_elm/2;
        lp_dest=(long *)dest;
        lp_src=(long *)source;
        HDmemset(lp_dest,0,num_elm*sizeof(long)); /* initialize to zeros */
        for(i = 0; i < n; i++) {
            if(lp_src[0] & UI4S_MASKC)            /* Can't forget to sign extend */
                lp_dest[0] = 0xffffffffffffffff;
            lp_dest[0]=(lp_src[0]&UI4S_MASKA)>>32;
            if(lp_src[0] & UI4S_MASKD)            /* Can't forget to sign extend */
                lp_dest[1] = 0xffffffffffffffff;
            lp_dest[1]=lp_src[0]&UI4S_MASKB;
            lp_dest+=2;
            lp_src++;
          } /* end for */
        if(odd_man_out) {
            if(lp_src[0] & UI4S_MASKC)            /* Can't forget to sign extend */
                *lp_dest = 0xffffffffffffffff;
            *lp_dest=(lp_src[0]&UI4S_MASKA)>>32;
          } /* end if */
#elif defined TEST1_ui4s
        int odd_man_out;        /* By default there are even num_elm */
        intn n;

        odd_man_out = num_elm % 2;

        n=num_elm/2;
        lp_dest=(long *)dest;
        lp_src=(long *)source;
        HDmemset(lp_dest,0,num_elm*sizeof(long)); /* initialize to zeros */
        for(i = 0; i < n; i++) {
            if(lp_src[0] & UI4S_MASKC)            /* Can't forget to sign extend */
                *lp_dest = 0xffffffffffffffff;
            *lp_dest++=(lp_src[0]&UI4S_MASKA)>>32;
            if(lp_src[0] & UI4S_MASKD)            /* Can't forget to sign extend */
                *lp_dest = 0xffffffffffffffff;
            *lp_dest++=lp_src[0]&UI4S_MASKB;
            lp_src++;
          } /* end for */
        if(odd_man_out) {
            if(lp_src[0] & UI4S_MASKC)            /* Can't forget to sign extend */
                *lp_dest = 0xffffffffffffffff;
            *lp_dest++=(lp_src[0]&UI4S_MASKA)>>32;
          } /* end if */
#else
        for(i = 0; i < num_elm; i++) {
            if((source[0] & 0x80))            /* Can't forget to sign extend */
                lptr_dest[0] = 0xffffffffffffffff;
            else
                lptr_dest[0] = 0x0000000000000000;
            dest[4] = source[0];
            dest[5] = source[1];
            dest[6] = source[2];
            dest[7] = source[3];
            source += 4;
            lptr_dest ++;
            dest = (uint8 *)lptr_dest;
        }
#endif
#else   /* DUFF_ui4s */
        uintn n;
        int odd_man_out;        /* By default there are even num_elm */

        lp_dest=(long *)dest;
        lp_src=(long *)source;
        HDmemset(lp_dest,0,num_elm*sizeof(long)); /* initialize to zeros */

        odd_man_out = num_elm % 2;

        num_elm/=2;
        n=(num_elm+7)/8;
        switch(num_elm%8) {
            case 0:
                do{
                    if(lp_src[0] & UI4S_MASKC)            /* Can't forget to sign extend */
                        lp_dest[0] = 0xffffffffffffffff;
                    lp_dest[0]=(lp_src[0]&UI4S_MASKA)>>32;
                    if(lp_src[0] & UI4S_MASKD)            /* Can't forget to sign extend */
                        lp_dest[1] = 0xffffffffffffffff;
                    lp_dest[1]=lp_src[0]&UI4S_MASKB;
                    lp_dest+=2;
                    lp_src++;
#ifdef QAK
            case 15:
                    if(lp_src[0] & UI4S_MASKC)            /* Can't forget to sign extend */
                        lp_dest[0] = 0xffffffffffffffff;
                    lp_dest[0]=(lp_src[0]&UI4S_MASKA)>>32;
                    if(lp_src[0] & UI4S_MASKD)            /* Can't forget to sign extend */
                        lp_dest[1] = 0xffffffffffffffff;
                    lp_dest[1]=lp_src[0]&UI4S_MASKB;
                    lp_dest+=2;
                    lp_src++;
            case 14:
                    if(lp_src[0] & UI4S_MASKC)            /* Can't forget to sign extend */
                        lp_dest[0] = 0xffffffffffffffff;
                    lp_dest[0]=(lp_src[0]&UI4S_MASKA)>>32;
                    if(lp_src[0] & UI4S_MASKD)            /* Can't forget to sign extend */
                        lp_dest[1] = 0xffffffffffffffff;
                    lp_dest[1]=lp_src[0]&UI4S_MASKB;
                    lp_dest+=2;
                    lp_src++;
            case 13:
                    if(lp_src[0] & UI4S_MASKC)            /* Can't forget to sign extend */
                        lp_dest[0] = 0xffffffffffffffff;
                    lp_dest[0]=(lp_src[0]&UI4S_MASKA)>>32;
                    if(lp_src[0] & UI4S_MASKD)            /* Can't forget to sign extend */
                        lp_dest[1] = 0xffffffffffffffff;
                    lp_dest[1]=lp_src[0]&UI4S_MASKB;
                    lp_dest+=2;
                    lp_src++;
            case 12:
                    if(lp_src[0] & UI4S_MASKC)            /* Can't forget to sign extend */
                        lp_dest[0] = 0xffffffffffffffff;
                    lp_dest[0]=(lp_src[0]&UI4S_MASKA)>>32;
                    if(lp_src[0] & UI4S_MASKD)            /* Can't forget to sign extend */
                        lp_dest[1] = 0xffffffffffffffff;
                    lp_dest[1]=lp_src[0]&UI4S_MASKB;
                    lp_dest+=2;
                    lp_src++;
            case 11:
                    if(lp_src[0] & UI4S_MASKC)            /* Can't forget to sign extend */
                        lp_dest[0] = 0xffffffffffffffff;
                    lp_dest[0]=(lp_src[0]&UI4S_MASKA)>>32;
                    if(lp_src[0] & UI4S_MASKD)            /* Can't forget to sign extend */
                        lp_dest[1] = 0xffffffffffffffff;
                    lp_dest[1]=lp_src[0]&UI4S_MASKB;
                    lp_dest+=2;
                    lp_src++;
            case 10:
                    if(lp_src[0] & UI4S_MASKC)            /* Can't forget to sign extend */
                        lp_dest[0] = 0xffffffffffffffff;
                    lp_dest[0]=(lp_src[0]&UI4S_MASKA)>>32;
                    if(lp_src[0] & UI4S_MASKD)            /* Can't forget to sign extend */
                        lp_dest[1] = 0xffffffffffffffff;
                    lp_dest[1]=lp_src[0]&UI4S_MASKB;
                    lp_dest+=2;
                    lp_src++;
            case 9:
                    if(lp_src[0] & UI4S_MASKC)            /* Can't forget to sign extend */
                        lp_dest[0] = 0xffffffffffffffff;
                    lp_dest[0]=(lp_src[0]&UI4S_MASKA)>>32;
                    if(lp_src[0] & UI4S_MASKD)            /* Can't forget to sign extend */
                        lp_dest[1] = 0xffffffffffffffff;
                    lp_dest[1]=lp_src[0]&UI4S_MASKB;
                    lp_dest+=2;
                    lp_src++;
            case 8:
                    if(lp_src[0] & UI4S_MASKC)            /* Can't forget to sign extend */
                        lp_dest[0] = 0xffffffffffffffff;
                    lp_dest[0]=(lp_src[0]&UI4S_MASKA)>>32;
                    if(lp_src[0] & UI4S_MASKD)            /* Can't forget to sign extend */
                        lp_dest[1] = 0xffffffffffffffff;
                    lp_dest[1]=lp_src[0]&UI4S_MASKB;
                    lp_dest+=2;
                    lp_src++;
#endif
            case 7:
                    if(lp_src[0] & UI4S_MASKC)            /* Can't forget to sign extend */
                        lp_dest[0] = 0xffffffffffffffff;
                    lp_dest[0]=(lp_src[0]&UI4S_MASKA)>>32;
                    if(lp_src[0] & UI4S_MASKD)            /* Can't forget to sign extend */
                        lp_dest[1] = 0xffffffffffffffff;
                    lp_dest[1]=lp_src[0]&UI4S_MASKB;
                    lp_dest+=2;
                    lp_src++;
            case 6:
                    if(lp_src[0] & UI4S_MASKC)            /* Can't forget to sign extend */
                        lp_dest[0] = 0xffffffffffffffff;
                    lp_dest[0]=(lp_src[0]&UI4S_MASKA)>>32;
                    if(lp_src[0] & UI4S_MASKD)            /* Can't forget to sign extend */
                        lp_dest[1] = 0xffffffffffffffff;
                    lp_dest[1]=lp_src[0]&UI4S_MASKB;
                    lp_dest+=2;
                    lp_src++;
            case 5:
                    if(lp_src[0] & UI4S_MASKC)            /* Can't forget to sign extend */
                        lp_dest[0] = 0xffffffffffffffff;
                    lp_dest[0]=(lp_src[0]&UI4S_MASKA)>>32;
                    if(lp_src[0] & UI4S_MASKD)            /* Can't forget to sign extend */
                        lp_dest[1] = 0xffffffffffffffff;
                    lp_dest[1]=lp_src[0]&UI4S_MASKB;
                    lp_dest+=2;
                    lp_src++;
            case 4:
                    if(lp_src[0] & UI4S_MASKC)            /* Can't forget to sign extend */
                        lp_dest[0] = 0xffffffffffffffff;
                    lp_dest[0]=(lp_src[0]&UI4S_MASKA)>>32;
                    if(lp_src[0] & UI4S_MASKD)            /* Can't forget to sign extend */
                        lp_dest[1] = 0xffffffffffffffff;
                    lp_dest[1]=lp_src[0]&UI4S_MASKB;
                    lp_dest+=2;
                    lp_src++;
            case 3:
                    if(lp_src[0] & UI4S_MASKC)            /* Can't forget to sign extend */
                        lp_dest[0] = 0xffffffffffffffff;
                    lp_dest[0]=(lp_src[0]&UI4S_MASKA)>>32;
                    if(lp_src[0] & UI4S_MASKD)            /* Can't forget to sign extend */
                        lp_dest[1] = 0xffffffffffffffff;
                    lp_dest[1]=lp_src[0]&UI4S_MASKB;
                    lp_dest+=2;
                    lp_src++;
            case 2:
                    if(lp_src[0] & UI4S_MASKC)            /* Can't forget to sign extend */
                        lp_dest[0] = 0xffffffffffffffff;
                    lp_dest[0]=(lp_src[0]&UI4S_MASKA)>>32;
                    if(lp_src[0] & UI4S_MASKD)            /* Can't forget to sign extend */
                        lp_dest[1] = 0xffffffffffffffff;
                    lp_dest[1]=lp_src[0]&UI4S_MASKB;
                    lp_dest+=2;
                    lp_src++;
            case 1:
                    if(lp_src[0] & UI4S_MASKC)            /* Can't forget to sign extend */
                        lp_dest[0] = 0xffffffffffffffff;
                    lp_dest[0]=(lp_src[0]&UI4S_MASKA)>>32;
                    if(lp_src[0] & UI4S_MASKD)            /* Can't forget to sign extend */
                        lp_dest[1] = 0xffffffffffffffff;
                    lp_dest[1]=lp_src[0]&UI4S_MASKB;
                    lp_dest+=2;
                    lp_src++;
                } while(--n>0);
		}
        if(odd_man_out) {
            if(lp_src[0] & UI4S_MASKC)            /* Can't forget to sign extend */
                *lp_dest = 0xffffffffffffffff;
            *lp_dest=(lp_src[0]&UI4S_MASKA)>>32;
          } /* end if */
#endif  /* DUFF_ui4s */
      } /* end if */
  else 
    for(i = 0; i < num_elm; i++) {
      if(source[0] & 0x80) {          /* Can't forget to sign extend */
        dest[0] = 0xff;
        dest[1] = 0xff;
        dest[2] = 0xff;
        dest[3] = 0xff;
      }
      else {
        dest[0] = 0;
        dest[1] = 0;
        dest[2] = 0;
        dest[3] = 0;
      }
      dest[4] = source[0];
      dest[5] = source[1];
      dest[6] = source[2];
      dest[7] = source[3];
      dest += dest_stride;
      source += source_stride;
    }
  return 0;
}

#define UO4I_MASK 0x00000000ffffffff

/************************************************************/
/* DFKuo4i()                                                */
/* -->Unicos routine for exporting 4 byte data items        */ 
/************************************************************/

#ifdef PROTOTYPE
PRIVATE int DFKuo4i(VOIDP s, VOIDP d, uint32 num_elm, uint32 source_stride,
		   uint32 dest_stride)
#else
PRIVATE int DFKuo4i(source, dest, num_elm, source_stride, dest_stride)
uint8 * source, * dest;
uint32 num_elm, source_stride, dest_stride;
#endif /* PROTOTYPE */
{
  int fast_processing=0;
  register uintn i;
#ifdef PROTOTYPE
  uint8 * source = (uint8*)s;
  uint8 * dest = (uint8*)d;
#endif /* PROTOTYPE */
    long *lp_dest;
    long *lp_src;
  char *FUNC="DFKuo4i";

  HEclear();

  if(source == dest || num_elm == 0) {  /* Inplace conversions  not permitted */
    HERROR(DFE_BADCONV);                /* No elements to convert is an error */
    return FAIL;
  }

  if(source_stride == 0 && dest_stride == 0) {
    fast_processing = 1;
  }
  
  if(fast_processing) {

#ifndef DUFF_uo4i
#if defined TEST1_uo4i
    int odd_man_out;        /* By default there are even num_elm */
    intn n;

    odd_man_out = num_elm % 2;

    n=num_elm/2;
    lp_dest=(long *)dest;
    lp_src=(long *)source;
    for(i = 0; i < n; i++) {
        *lp_dest++=((lp_src[0]&UO4I_MASK)<<32)|(lp_src[1]&UO4I_MASK);
        lp_src+=2;
    }
    if(odd_man_out)
        *lp_dest=(lp_src[0]&UO4I_MASK)<<32;
#else
    for(i = 0; i < num_elm; i++) {
      dest[0] = source[4];
      dest[1] = source[5];
      dest[2] = source[6];
      dest[3] = source[7];
      dest += 4;
      source += 8;
    }
#endif
#else   /* DUFF_uo4i */
        uintn n;
        int odd_man_out;        /* By default there are even num_elm */

        odd_man_out = num_elm % 2;

        num_elm/=2;
        n=(num_elm+7)/8;
        lp_dest=(long *)dest;
        lp_src=(long *)source;
        switch(num_elm%8) {
            case 0:
                do{
                    *lp_dest++=((lp_src[0]&UO4I_MASK)<<32)|(lp_src[1]&UO4I_MASK);
                    lp_src+=2;
#ifdef QAK
            case 15:
                    *lp_dest++=((lp_src[0]&UO4I_MASK)<<32)|(lp_src[1]&UO4I_MASK);
                    lp_src+=2;
            case 14:
                    *lp_dest++=((lp_src[0]&UO4I_MASK)<<32)|(lp_src[1]&UO4I_MASK);
                    lp_src+=2;
            case 13:
                    *lp_dest++=((lp_src[0]&UO4I_MASK)<<32)|(lp_src[1]&UO4I_MASK);
                    lp_src+=2;
            case 12:
                    *lp_dest++=((lp_src[0]&UO4I_MASK)<<32)|(lp_src[1]&UO4I_MASK);
                    lp_src+=2;
            case 11:
                    *lp_dest++=((lp_src[0]&UO4I_MASK)<<32)|(lp_src[1]&UO4I_MASK);
                    lp_src+=2;
            case 10:
                    *lp_dest++=((lp_src[0]&UO4I_MASK)<<32)|(lp_src[1]&UO4I_MASK);
                    lp_src+=2;
            case 9:
                    *lp_dest++=((lp_src[0]&UO4I_MASK)<<32)|(lp_src[1]&UO4I_MASK);
                    lp_src+=2;
            case 8:
                    *lp_dest++=((lp_src[0]&UO4I_MASK)<<32)|(lp_src[1]&UO4I_MASK);
                    lp_src+=2;
#endif
            case 7:
                    *lp_dest++=((lp_src[0]&UO4I_MASK)<<32)|(lp_src[1]&UO4I_MASK);
                    lp_src+=2;
            case 6:
                    *lp_dest++=((lp_src[0]&UO4I_MASK)<<32)|(lp_src[1]&UO4I_MASK);
                    lp_src+=2;
            case 5:
                    *lp_dest++=((lp_src[0]&UO4I_MASK)<<32)|(lp_src[1]&UO4I_MASK);
                    lp_src+=2;
            case 4:
                    *lp_dest++=((lp_src[0]&UO4I_MASK)<<32)|(lp_src[1]&UO4I_MASK);
                    lp_src+=2;
            case 3:
                    *lp_dest++=((lp_src[0]&UO4I_MASK)<<32)|(lp_src[1]&UO4I_MASK);
                    lp_src+=2;
            case 2:
                    *lp_dest++=((lp_src[0]&UO4I_MASK)<<32)|(lp_src[1]&UO4I_MASK);
                    lp_src+=2;
            case 1:
                    *lp_dest++=((lp_src[0]&UO4I_MASK)<<32)|(lp_src[1]&UO4I_MASK);
                    lp_src+=2;
                } while(--n>0);
		}
        if(odd_man_out)
            *lp_dest++=(lp_src[0]&UO4I_MASK)<<32;

#endif  /* DUFF_uo4i */
  }
  else 
    for(i = 0; i < num_elm; i++) {
      dest[0] = source[4];
      dest[1] = source[5];
      dest[2] = source[6];
      dest[3] = source[7];
      dest += dest_stride;
      source += source_stride;
    }
  return 0;
}

#define UI4F_MASKA 0x8000000000000000
#define UI4F_MASKB 0x7f80000000000000
#define UI4F_MASKC 0x007fffff00000000
#define UI4F_MASKD 0x0000800000000000
#define UI4F_MASKE 0x0000000080000000
#define UI4F_MASKF 0x000000007f800000
#define UI4F_MASKG 0x00000000007fffff
#define UI4F_MASKH 0x0000000000008000

/************************************************************/
/* DFKui4f()                                                */
/* -->Unicos routine for importing 32 bit floats            */
/************************************************************/

/************************************************************

                     <<<< WARNING >>>>

    The nature of converting between 64 bit floating point
  numbers and 32 bit floating point numbers LOSES PRECISION.
  Taking a number in 64bit cray format, converting to IEEE
  (internal HDF format) and back will round the number at
  about the 7th decimal place.

 ************************************************************/

#ifdef PROTOTYPE
PRIVATE int DFKui4f(VOIDP s, VOIDP d, uint32 num_elm, uint32 source_stride,
		   uint32 dest_stride)
#else
PRIVATE int DFKui4f(source, dest, num_elm, source_stride, dest_stride)
uint8 * source;
uint8 * dest;
uint32 num_elm;
uint32 source_stride;
uint32 dest_stride;
#endif /* PROTOTYPE */
{
  int fast_processing = 0;              /* By default not array processed */
  int odd_man_out = 0;                  /* By default there are even num_elm */
  int i,j,n;
  long buf1;                            /* This is a temporary stride buf */
  long buf2;                            /* This is a temporary stride buf */
  uint8 * dud1 = (uint8*)&buf1;         /* Dummy pointer to buf1 for strides */
  uint8 * dud2 = (uint8*)&buf2;         /* Dummy pointer to buf2 for strides */
#ifdef PROTOTYPE
  uint8 * source = (uint8*)s;            /* Cray does not like certain   */
  uint8 * dest = (uint8*)d;              /* void and void* constructions */
#endif /* PROTOTYPE */
  long * lptr_src = (long*)source;
  long * lptr_dest = (long*)dest;
  char *FUNC="DFKui4f";

  HEclear();

  /* Check for conversion errors */
  if(source == dest || num_elm == 0) { /* Inplace conversions not permitted */
    HERROR(DFE_BADCONV);               /* under UNICOS */
    return FAIL;                       /* No elements convert is an error   */
  }

  /* Find out if it is OK to use faster array processing */
  if(source_stride == 0 && dest_stride == 0)
      fast_processing = 1;

    if(fast_processing) {
#ifndef DUFF_ui4f
#if defined TEST1_ui4f
        odd_man_out = num_elm%2 ;
        n = num_elm / 2;
        for(i = 0; i < n; i++) {
            lptr_dest[0] = ((*lptr_src & UI4F_MASKA) |
                    ((*lptr_src & UI4F_MASKB) >> 7) + (16258 << 48)) |
                    (((*lptr_src & UI4F_MASKC) >> 8) | (UI4F_MASKD));
            lptr_dest[1] = ((((*lptr_src & UI4F_MASKE)) |
                    ((*lptr_src & UI4F_MASKF) >> 7) + (16258 << 16)) |
                    (((*lptr_src & UI4F_MASKG) >> 8) |(UI4F_MASKH))) << 32;
            lptr_dest+=2;
            lptr_src++;
          } /* end for */
        if(odd_man_out) {
            lptr_dest[0] = (((lptr_src[0]) & UI4F_MASKA) |
                (((lptr_src[0]) & UI4F_MASKB) >> 7) + (16258 << 48)) |
                ((((lptr_src[0]) & UI4F_MASKC) >> 8) | (UI4F_MASKD));
          } /* end if */
#else
        if((num_elm % 2))                   /* If this is true, we have odd num */
          odd_man_out = 1;
        num_elm = num_elm / 2;
        for(i = 0; i < num_elm; i++) {
            if(*(float*)lptr_src != 0) {
                *lptr_dest = (((*lptr_src & UI4F_MASKA) |
                        ((*lptr_src & UI4F_MASKB) >> 7) +
                        (16258 << 48)) |
                        (((*lptr_src & UI4F_MASKC) >> 8) | (UI4F_MASKD)));
                if(((*lptr_src & 0xffffffff00000000) << 1)== 0)
                    *lptr_dest = 0;
              } /* end if */
            else
                *lptr_dest = *lptr_src;
            lptr_dest++;
            if(*(float*)lptr_src != 0) {
                *lptr_dest = ((((((*lptr_src << 32) & UI4F_MASKA)) |
                        (((*lptr_src <<32) & UI4F_MASKB) >> 7) +
                        (16258 << 48)) |
                        ((((*lptr_src << 32) & UI4F_MASKC) >> 8) |(UI4F_MASKD))));
                if(((*lptr_src & 0x00000000ffffffff) << 33)== 0)
                    *lptr_dest = 0;
              } /* end if */
            else
                *lptr_dest = *lptr_src;
            lptr_src++;
            lptr_dest++;
          } /* end for */
        if(odd_man_out) {
            if(((float*)lptr_src)[0] != 0) {
                lptr_dest[0] = ((((lptr_src[0]) & UI4F_MASKA) |
                    (((lptr_src[0]) & UI4F_MASKB) >> 7) + (16258 << 48)) |
                    ((((lptr_src[0]) & UI4F_MASKC) >> 8) | (UI4F_MASKD)));
                if(((lptr_src[0] & 0xffffffff00000000) << 1)== 0)
                    lptr_dest[0] = 0;
              } /* end if */
            else
                *lptr_dest = *lptr_src;
          } /* end if */
#endif
#else   /* DUFF_ui4f */
        odd_man_out = num_elm % 2;

        num_elm/=2;
        n=(num_elm+7)/8;
        switch(num_elm%8) {
            case 0:
                do{
                    lptr_dest[0] = ((*lptr_src & UI4F_MASKA) |
                            ((*lptr_src & UI4F_MASKB) >> 7) + (16258 << 48)) |
                            (((*lptr_src & UI4F_MASKC) >> 8) | (UI4F_MASKD));
                    lptr_dest[1] = ((((*lptr_src & UI4F_MASKE)) |
                            ((*lptr_src & UI4F_MASKF) >> 7) + (16258 << 16)) |
                            (((*lptr_src & UI4F_MASKG) >> 8) |(UI4F_MASKH))) << 32;
                    lptr_dest+=2;
                    lptr_src++;
#ifdef QAK
            case 15:
                    lptr_dest[0] = ((*lptr_src & UI4F_MASKA) |
                            ((*lptr_src & UI4F_MASKB) >> 7) + (16258 << 48)) |
                            (((*lptr_src & UI4F_MASKC) >> 8) | (UI4F_MASKD));
                    lptr_dest[1] = ((((*lptr_src & UI4F_MASKE)) |
                            ((*lptr_src & UI4F_MASKF) >> 7) + (16258 << 16)) |
                            (((*lptr_src & UI4F_MASKG) >> 8) |(UI4F_MASKH))) << 32;
                    lptr_dest+=2;
                    lptr_src++;
            case 14:
                    lptr_dest[0] = ((*lptr_src & UI4F_MASKA) |
                            ((*lptr_src & UI4F_MASKB) >> 7) + (16258 << 48)) |
                            (((*lptr_src & UI4F_MASKC) >> 8) | (UI4F_MASKD));
                    lptr_dest[1] = ((((*lptr_src & UI4F_MASKE)) |
                            ((*lptr_src & UI4F_MASKF) >> 7) + (16258 << 16)) |
                            (((*lptr_src & UI4F_MASKG) >> 8) |(UI4F_MASKH))) << 32;
                    lptr_dest+=2;
                    lptr_src++;
            case 13:
                    lptr_dest[0] = ((*lptr_src & UI4F_MASKA) |
                            ((*lptr_src & UI4F_MASKB) >> 7) + (16258 << 48)) |
                            (((*lptr_src & UI4F_MASKC) >> 8) | (UI4F_MASKD));
                    lptr_dest[1] = ((((*lptr_src & UI4F_MASKE)) |
                            ((*lptr_src & UI4F_MASKF) >> 7) + (16258 << 16)) |
                            (((*lptr_src & UI4F_MASKG) >> 8) |(UI4F_MASKH))) << 32;
                    lptr_dest+=2;
                    lptr_src++;
            case 12:
                    lptr_dest[0] = ((*lptr_src & UI4F_MASKA) |
                            ((*lptr_src & UI4F_MASKB) >> 7) + (16258 << 48)) |
                            (((*lptr_src & UI4F_MASKC) >> 8) | (UI4F_MASKD));
                    lptr_dest[1] = ((((*lptr_src & UI4F_MASKE)) |
                            ((*lptr_src & UI4F_MASKF) >> 7) + (16258 << 16)) |
                            (((*lptr_src & UI4F_MASKG) >> 8) |(UI4F_MASKH))) << 32;
                    lptr_dest+=2;
                    lptr_src++;
            case 11:
                    lptr_dest[0] = ((*lptr_src & UI4F_MASKA) |
                            ((*lptr_src & UI4F_MASKB) >> 7) + (16258 << 48)) |
                            (((*lptr_src & UI4F_MASKC) >> 8) | (UI4F_MASKD));
                    lptr_dest[1] = ((((*lptr_src & UI4F_MASKE)) |
                            ((*lptr_src & UI4F_MASKF) >> 7) + (16258 << 16)) |
                            (((*lptr_src & UI4F_MASKG) >> 8) |(UI4F_MASKH))) << 32;
                    lptr_dest+=2;
                    lptr_src++;
            case 10:
                    lptr_dest[0] = ((*lptr_src & UI4F_MASKA) |
                            ((*lptr_src & UI4F_MASKB) >> 7) + (16258 << 48)) |
                            (((*lptr_src & UI4F_MASKC) >> 8) | (UI4F_MASKD));
                    lptr_dest[1] = ((((*lptr_src & UI4F_MASKE)) |
                            ((*lptr_src & UI4F_MASKF) >> 7) + (16258 << 16)) |
                            (((*lptr_src & UI4F_MASKG) >> 8) |(UI4F_MASKH))) << 32;
                    lptr_dest+=2;
                    lptr_src++;
            case 9:
                    lptr_dest[0] = ((*lptr_src & UI4F_MASKA) |
                            ((*lptr_src & UI4F_MASKB) >> 7) + (16258 << 48)) |
                            (((*lptr_src & UI4F_MASKC) >> 8) | (UI4F_MASKD));
                    lptr_dest[1] = ((((*lptr_src & UI4F_MASKE)) |
                            ((*lptr_src & UI4F_MASKF) >> 7) + (16258 << 16)) |
                            (((*lptr_src & UI4F_MASKG) >> 8) |(UI4F_MASKH))) << 32;
                    lptr_dest+=2;
                    lptr_src++;
            case 8:
                    lptr_dest[0] = ((*lptr_src & UI4F_MASKA) |
                            ((*lptr_src & UI4F_MASKB) >> 7) + (16258 << 48)) |
                            (((*lptr_src & UI4F_MASKC) >> 8) | (UI4F_MASKD));
                    lptr_dest[1] = ((((*lptr_src & UI4F_MASKE)) |
                            ((*lptr_src & UI4F_MASKF) >> 7) + (16258 << 16)) |
                            (((*lptr_src & UI4F_MASKG) >> 8) |(UI4F_MASKH))) << 32;
                    lptr_dest+=2;
                    lptr_src++;
#endif
            case 7:
                    lptr_dest[0] = ((*lptr_src & UI4F_MASKA) |
                            ((*lptr_src & UI4F_MASKB) >> 7) + (16258 << 48)) |
                            (((*lptr_src & UI4F_MASKC) >> 8) | (UI4F_MASKD));
                    lptr_dest[1] = ((((*lptr_src & UI4F_MASKE)) |
                            ((*lptr_src & UI4F_MASKF) >> 7) + (16258 << 16)) |
                            (((*lptr_src & UI4F_MASKG) >> 8) |(UI4F_MASKH))) << 32;
                    lptr_dest+=2;
                    lptr_src++;
            case 6:
                    lptr_dest[0] = ((*lptr_src & UI4F_MASKA) |
                            ((*lptr_src & UI4F_MASKB) >> 7) + (16258 << 48)) |
                            (((*lptr_src & UI4F_MASKC) >> 8) | (UI4F_MASKD));
                    lptr_dest[1] = ((((*lptr_src & UI4F_MASKE)) |
                            ((*lptr_src & UI4F_MASKF) >> 7) + (16258 << 16)) |
                            (((*lptr_src & UI4F_MASKG) >> 8) |(UI4F_MASKH))) << 32;
                    lptr_dest+=2;
                    lptr_src++;
            case 5:
                    lptr_dest[0] = ((*lptr_src & UI4F_MASKA) |
                            ((*lptr_src & UI4F_MASKB) >> 7) + (16258 << 48)) |
                            (((*lptr_src & UI4F_MASKC) >> 8) | (UI4F_MASKD));
                    lptr_dest[1] = ((((*lptr_src & UI4F_MASKE)) |
                            ((*lptr_src & UI4F_MASKF) >> 7) + (16258 << 16)) |
                            (((*lptr_src & UI4F_MASKG) >> 8) |(UI4F_MASKH))) << 32;
                    lptr_dest+=2;
                    lptr_src++;
            case 4:
                    lptr_dest[0] = ((*lptr_src & UI4F_MASKA) |
                            ((*lptr_src & UI4F_MASKB) >> 7) + (16258 << 48)) |
                            (((*lptr_src & UI4F_MASKC) >> 8) | (UI4F_MASKD));
                    lptr_dest[1] = ((((*lptr_src & UI4F_MASKE)) |
                            ((*lptr_src & UI4F_MASKF) >> 7) + (16258 << 16)) |
                            (((*lptr_src & UI4F_MASKG) >> 8) |(UI4F_MASKH))) << 32;
                    lptr_dest+=2;
                    lptr_src++;
            case 3:
                    lptr_dest[0] = ((*lptr_src & UI4F_MASKA) |
                            ((*lptr_src & UI4F_MASKB) >> 7) + (16258 << 48)) |
                            (((*lptr_src & UI4F_MASKC) >> 8) | (UI4F_MASKD));
                    lptr_dest[1] = ((((*lptr_src & UI4F_MASKE)) |
                            ((*lptr_src & UI4F_MASKF) >> 7) + (16258 << 16)) |
                            (((*lptr_src & UI4F_MASKG) >> 8) |(UI4F_MASKH))) << 32;
                    lptr_dest+=2;
                    lptr_src++;
            case 2:
                    lptr_dest[0] = ((*lptr_src & UI4F_MASKA) |
                            ((*lptr_src & UI4F_MASKB) >> 7) + (16258 << 48)) |
                            (((*lptr_src & UI4F_MASKC) >> 8) | (UI4F_MASKD));
                    lptr_dest[1] = ((((*lptr_src & UI4F_MASKE)) |
                            ((*lptr_src & UI4F_MASKF) >> 7) + (16258 << 16)) |
                            (((*lptr_src & UI4F_MASKG) >> 8) |(UI4F_MASKH))) << 32;
                    lptr_dest+=2;
                    lptr_src++;
            case 1:
                    lptr_dest[0] = ((*lptr_src & UI4F_MASKA) |
                            ((*lptr_src & UI4F_MASKB) >> 7) + (16258 << 48)) |
                            (((*lptr_src & UI4F_MASKC) >> 8) | (UI4F_MASKD));
                    lptr_dest[1] = ((((*lptr_src & UI4F_MASKE)) |
                            ((*lptr_src & UI4F_MASKF) >> 7) + (16258 << 16)) |
                            (((*lptr_src & UI4F_MASKG) >> 8) |(UI4F_MASKH))) << 32;
                    lptr_dest+=2;
                    lptr_src++;
                } while(--n>0);
		}
        if(odd_man_out) {
            lptr_dest[0] = (((lptr_src[0]) & UI4F_MASKA) |
                (((lptr_src[0]) & UI4F_MASKB) >> 7) + (16258 << 48)) |
                ((((lptr_src[0]) & UI4F_MASKC) >> 8) | (UI4F_MASKD));
          } /* end if */
#endif  /* DUFF_ui4f */
      } /* end if */
    else { /* We end up here if we are doing stride based processing */
        buf1 = 0;
        for(i = 0; i < num_elm; i++) {
            dud1[0] = source[0];               /* Loop would be less efficient */
            dud1[1] = source[1];
            dud1[2] = source[2];
            dud1[3] = source[3];

            if((float)buf1 != 0) {
                buf2 = (((buf1 & UI4F_MASKA) |
                        ((buf1 & UI4F_MASKB) >> 7) +
                        (16258 << 48)) |
                        (((buf1 & UI4F_MASKC) >> 8) | (UI4F_MASKD)));
                if((buf1 << 1)== 0)
                    buf2 = 0;
              } /* end if */
            else
                buf2 = buf1;

            dest[0] = dud2[0];            /* Loop would be less efficient */
            dest[1] = dud2[1];
            dest[2] = dud2[2];
            dest[3] = dud2[3];
            dest[4] = dud2[4];
            dest[5] = dud2[5];
            dest[6] = dud2[6];
            dest[7] = dud2[7];

            source += source_stride;
            dest += dest_stride;
          } /* end for */
      } /* end else */
  return;
}


#define UO4F_MASKA (0x8000000000000000)
#define UO4F_MASKB (0x7fff000000000000)
#define UO4F_MASKC (0x00007fffff000000)
#define UO4F_MASKD (0x0000000000800000)
#define UO4F_MASKE (0xffffffff00000000)

/************************************************************/
/* DFKuo4f()                                                */
/* -->Unicos routine for exporting 32 bit floats            */
/************************************************************/

/************************************************************

                     <<<< WARNING >>>>

    The nature of converting between 64 bit floating point
  numbers and 32 bit floating point numbers LOSES PRECISION.
  Taking a number in 64bit cray format, converting to IEEE
  (internal HDF format) and back will round the number at
  about the 7th decimal place.

 ************************************************************/

#ifdef PROTOTYPE
PRIVATE int DFKuo4f(VOIDP s, VOIDP d, uint32 num_elm, uint32 source_stride,
		   uint32 dest_stride)
#else
PRIVATE int DFKuo4f(source, dest, num_elm, source_stride, dest_stride)
uint8 * source, * dest;
uint32 num_elm, source_stride, dest_stride;
#endif /* PROTOTYPE */
{
  int fast_processing = 0;              /* By default not array processed */
  int odd_man_out = 0;                  /* By default there are even num_elm */
  int i,j,n;
  long buf1;                            /* This is a temporary stride buf */
  long buf2;                            /* This is a temporary stride buf */
  uint8 * dud1 = (uint8*)&buf1;         /* Dummy pointer to buf1 for strides */
  uint8 * dud2 = (uint8*)&buf2;         /* Dummy pointer to buf2 for strides */
#ifdef PROTOTYPE
  uint8 * source = (uint8*)s;           /* Cray does not like certain   */
  uint8 * dest = (uint8*)d;             /* void and void* constructions */
#endif /* PROTOTYPE */
  long * lptr_src = (long*)source;
  long * lptr_dest = (long*)dest;
  char *FUNC="DFKuo4f";

  HEclear();

  /* Check for conversion errors */
  if(source == dest || num_elm == 0) { /* Inplace conversions not permitted */
    HERROR(DFE_BADCONV);               /* under UNICOS */
    return FAIL;                       /* No elements convert is an error   */
  }

  /* Find out if it is OK to use faster array processing */
  if(source_stride == 0 && dest_stride == 0)
      fast_processing = 1;            

    if(fast_processing) {
#ifndef DUFF_uo4f
#if defined TEST1_uo4f
        odd_man_out = num_elm%2;
        n=num_elm/2;
        for(i = 0; i < n; i++) {
            lptr_dest[0] = ((((lptr_src[0] & UO4F_MASKA) |
                    ((((lptr_src[0] & UO4F_MASKB) >> 48) - 16258) << 55)) +
                    (((lptr_src[0] & UO4F_MASKC) +
                    ((lptr_src[0] & UO4F_MASKD) << 1)) << 8)) & UO4F_MASKE) |
                    (((((lptr_src[1] & UO4F_MASKA) |
                    ((((lptr_src[1] & UO4F_MASKB) >> 48) - 16258) << 55)) +
                    (((lptr_src[1] & UO4F_MASKC) +
                    ((lptr_src[1] & UO4F_MASKD) << 1)) << 8))&UO4F_MASKE)>>32);
            lptr_src+=2;
            lptr_dest++;
          } /* end for */
        if(odd_man_out)
            lptr_dest[0] = (((lptr_src[0] & UO4F_MASKA) |
                    ((((lptr_src[0] & UO4F_MASKB) >> 48) - 16258) << 55)) +
                    (((lptr_src[0] & UO4F_MASKC) +
                    ((lptr_src[0] & UO4F_MASKD) << 1)) << 8));
#else
        if((num_elm % 2))                   /* If this is true, we have odd num */
          odd_man_out = 1;
        for(i = 0; i < (int)(num_elm/2); i++) {
            buf1 = lptr_src[0];
            buf2 = lptr_src[1];
            if(buf1 != 0)
                buf1 = (((buf1 & UO4F_MASKA) |
                        ((((buf1 & UO4F_MASKB) >> 48) - 16258) << 55)) +
                        (((buf1 & UO4F_MASKC) +
                        ((buf1 & UO4F_MASKD) << 1)) << 8));
            else
                buf1 = 0;
            if(buf2 != 0)
                buf2 = ((((buf2 & UO4F_MASKA) |
                        ((((buf2 & UO4F_MASKB) >> 48) - 16258) << 55)) +
                        (((buf2 & UO4F_MASKC) +
                        ((buf2 & UO4F_MASKD) << 1)) << 8)) >> 32);
            else
                buf2 = 0;
            lptr_dest[0]=((buf1&0xffffffff00000000)|(buf2&0x00000000ffffffff));
            lptr_src ++;
            lptr_src ++;
            lptr_dest ++;
          } /* end for */
        if(odd_man_out) {
            if(lptr_src[0] != 0)
                lptr_dest[0] = (((lptr_src[0] & UO4F_MASKA) |
                        ((((lptr_src[0] & UO4F_MASKB) >> 48) - 16258) << 55)) +
                        (((lptr_src[0] & UO4F_MASKC) +
                        ((lptr_src[0] & UO4F_MASKD) << 1)) << 8));
            else
                lptr_dest[0] = 0;
          } /* end if */
#endif
#else   /* DUFF_uo4f */
        odd_man_out = num_elm % 2;

        num_elm/=2;
        n=(num_elm+7)/8;
        switch(num_elm%8) {
            case 0:
                do{
                    lptr_dest[0] = ((((lptr_src[0] & UO4F_MASKA) |
                            ((((lptr_src[0] & UO4F_MASKB) >> 48) - 16258) << 55)) +
                            (((lptr_src[0] & UO4F_MASKC) +
                            ((lptr_src[0] & UO4F_MASKD) << 1)) << 8)) & UO4F_MASKE) |
                            (((((lptr_src[1] & UO4F_MASKA) |
                            ((((lptr_src[1] & UO4F_MASKB) >> 48) - 16258) << 55)) +
                            (((lptr_src[1] & UO4F_MASKC) +
                            ((lptr_src[1] & UO4F_MASKD) << 1)) << 8))&UO4F_MASKE)>>32);
                    lptr_src+=2;
                    lptr_dest++;
#ifdef QAK
            case 15:
                    lptr_dest[0] = ((((lptr_src[0] & UO4F_MASKA) |
                            ((((lptr_src[0] & UO4F_MASKB) >> 48) - 16258) << 55)) +
                            (((lptr_src[0] & UO4F_MASKC) +
                            ((lptr_src[0] & UO4F_MASKD) << 1)) << 8)) & UO4F_MASKE) |
                            (((((lptr_src[1] & UO4F_MASKA) |
                            ((((lptr_src[1] & UO4F_MASKB) >> 48) - 16258) << 55)) +
                            (((lptr_src[1] & UO4F_MASKC) +
                            ((lptr_src[1] & UO4F_MASKD) << 1)) << 8))&UO4F_MASKE)>>32);
                    lptr_src+=2;
                    lptr_dest++;
            case 14:
                    lptr_dest[0] = ((((lptr_src[0] & UO4F_MASKA) |
                            ((((lptr_src[0] & UO4F_MASKB) >> 48) - 16258) << 55)) +
                            (((lptr_src[0] & UO4F_MASKC) +
                            ((lptr_src[0] & UO4F_MASKD) << 1)) << 8)) & UO4F_MASKE) |
                            (((((lptr_src[1] & UO4F_MASKA) |
                            ((((lptr_src[1] & UO4F_MASKB) >> 48) - 16258) << 55)) +
                            (((lptr_src[1] & UO4F_MASKC) +
                            ((lptr_src[1] & UO4F_MASKD) << 1)) << 8))&UO4F_MASKE)>>32);
                    lptr_src+=2;
                    lptr_dest++;
            case 13:
                    lptr_dest[0] = ((((lptr_src[0] & UO4F_MASKA) |
                            ((((lptr_src[0] & UO4F_MASKB) >> 48) - 16258) << 55)) +
                            (((lptr_src[0] & UO4F_MASKC) +
                            ((lptr_src[0] & UO4F_MASKD) << 1)) << 8)) & UO4F_MASKE) |
                            (((((lptr_src[1] & UO4F_MASKA) |
                            ((((lptr_src[1] & UO4F_MASKB) >> 48) - 16258) << 55)) +
                            (((lptr_src[1] & UO4F_MASKC) +
                            ((lptr_src[1] & UO4F_MASKD) << 1)) << 8))&UO4F_MASKE)>>32);
                    lptr_src+=2;
                    lptr_dest++;
            case 12:
                    lptr_dest[0] = ((((lptr_src[0] & UO4F_MASKA) |
                            ((((lptr_src[0] & UO4F_MASKB) >> 48) - 16258) << 55)) +
                            (((lptr_src[0] & UO4F_MASKC) +
                            ((lptr_src[0] & UO4F_MASKD) << 1)) << 8)) & UO4F_MASKE) |
                            (((((lptr_src[1] & UO4F_MASKA) |
                            ((((lptr_src[1] & UO4F_MASKB) >> 48) - 16258) << 55)) +
                            (((lptr_src[1] & UO4F_MASKC) +
                            ((lptr_src[1] & UO4F_MASKD) << 1)) << 8))&UO4F_MASKE)>>32);
                    lptr_src+=2;
                    lptr_dest++;
            case 11:
                    lptr_dest[0] = ((((lptr_src[0] & UO4F_MASKA) |
                            ((((lptr_src[0] & UO4F_MASKB) >> 48) - 16258) << 55)) +
                            (((lptr_src[0] & UO4F_MASKC) +
                            ((lptr_src[0] & UO4F_MASKD) << 1)) << 8)) & UO4F_MASKE) |
                            (((((lptr_src[1] & UO4F_MASKA) |
                            ((((lptr_src[1] & UO4F_MASKB) >> 48) - 16258) << 55)) +
                            (((lptr_src[1] & UO4F_MASKC) +
                            ((lptr_src[1] & UO4F_MASKD) << 1)) << 8))&UO4F_MASKE)>>32);
                    lptr_src+=2;
                    lptr_dest++;
            case 10:
                    lptr_dest[0] = ((((lptr_src[0] & UO4F_MASKA) |
                            ((((lptr_src[0] & UO4F_MASKB) >> 48) - 16258) << 55)) +
                            (((lptr_src[0] & UO4F_MASKC) +
                            ((lptr_src[0] & UO4F_MASKD) << 1)) << 8)) & UO4F_MASKE) |
                            (((((lptr_src[1] & UO4F_MASKA) |
                            ((((lptr_src[1] & UO4F_MASKB) >> 48) - 16258) << 55)) +
                            (((lptr_src[1] & UO4F_MASKC) +
                            ((lptr_src[1] & UO4F_MASKD) << 1)) << 8))&UO4F_MASKE)>>32);
                    lptr_src+=2;
                    lptr_dest++;
            case 9:
                    lptr_dest[0] = ((((lptr_src[0] & UO4F_MASKA) |
                            ((((lptr_src[0] & UO4F_MASKB) >> 48) - 16258) << 55)) +
                            (((lptr_src[0] & UO4F_MASKC) +
                            ((lptr_src[0] & UO4F_MASKD) << 1)) << 8)) & UO4F_MASKE) |
                            (((((lptr_src[1] & UO4F_MASKA) |
                            ((((lptr_src[1] & UO4F_MASKB) >> 48) - 16258) << 55)) +
                            (((lptr_src[1] & UO4F_MASKC) +
                            ((lptr_src[1] & UO4F_MASKD) << 1)) << 8))&UO4F_MASKE)>>32);
                    lptr_src+=2;
                    lptr_dest++;
            case 8:
                    lptr_dest[0] = ((((lptr_src[0] & UO4F_MASKA) |
                            ((((lptr_src[0] & UO4F_MASKB) >> 48) - 16258) << 55)) +
                            (((lptr_src[0] & UO4F_MASKC) +
                            ((lptr_src[0] & UO4F_MASKD) << 1)) << 8)) & UO4F_MASKE) |
                            (((((lptr_src[1] & UO4F_MASKA) |
                            ((((lptr_src[1] & UO4F_MASKB) >> 48) - 16258) << 55)) +
                            (((lptr_src[1] & UO4F_MASKC) +
                            ((lptr_src[1] & UO4F_MASKD) << 1)) << 8))&UO4F_MASKE)>>32);
                    lptr_src+=2;
                    lptr_dest++;
#endif
            case 7:
                    lptr_dest[0] = ((((lptr_src[0] & UO4F_MASKA) |
                            ((((lptr_src[0] & UO4F_MASKB) >> 48) - 16258) << 55)) +
                            (((lptr_src[0] & UO4F_MASKC) +
                            ((lptr_src[0] & UO4F_MASKD) << 1)) << 8)) & UO4F_MASKE) |
                            (((((lptr_src[1] & UO4F_MASKA) |
                            ((((lptr_src[1] & UO4F_MASKB) >> 48) - 16258) << 55)) +
                            (((lptr_src[1] & UO4F_MASKC) +
                            ((lptr_src[1] & UO4F_MASKD) << 1)) << 8))&UO4F_MASKE)>>32);
                    lptr_src+=2;
                    lptr_dest++;
            case 6:
                    lptr_dest[0] = ((((lptr_src[0] & UO4F_MASKA) |
                            ((((lptr_src[0] & UO4F_MASKB) >> 48) - 16258) << 55)) +
                            (((lptr_src[0] & UO4F_MASKC) +
                            ((lptr_src[0] & UO4F_MASKD) << 1)) << 8)) & UO4F_MASKE) |
                            (((((lptr_src[1] & UO4F_MASKA) |
                            ((((lptr_src[1] & UO4F_MASKB) >> 48) - 16258) << 55)) +
                            (((lptr_src[1] & UO4F_MASKC) +
                            ((lptr_src[1] & UO4F_MASKD) << 1)) << 8))&UO4F_MASKE)>>32);
                    lptr_src+=2;
                    lptr_dest++;
            case 5:
                    lptr_dest[0] = ((((lptr_src[0] & UO4F_MASKA) |
                            ((((lptr_src[0] & UO4F_MASKB) >> 48) - 16258) << 55)) +
                            (((lptr_src[0] & UO4F_MASKC) +
                            ((lptr_src[0] & UO4F_MASKD) << 1)) << 8)) & UO4F_MASKE) |
                            (((((lptr_src[1] & UO4F_MASKA) |
                            ((((lptr_src[1] & UO4F_MASKB) >> 48) - 16258) << 55)) +
                            (((lptr_src[1] & UO4F_MASKC) +
                            ((lptr_src[1] & UO4F_MASKD) << 1)) << 8))&UO4F_MASKE)>>32);
                    lptr_src+=2;
                    lptr_dest++;
            case 4:
                    lptr_dest[0] = ((((lptr_src[0] & UO4F_MASKA) |
                            ((((lptr_src[0] & UO4F_MASKB) >> 48) - 16258) << 55)) +
                            (((lptr_src[0] & UO4F_MASKC) +
                            ((lptr_src[0] & UO4F_MASKD) << 1)) << 8)) & UO4F_MASKE) |
                            (((((lptr_src[1] & UO4F_MASKA) |
                            ((((lptr_src[1] & UO4F_MASKB) >> 48) - 16258) << 55)) +
                            (((lptr_src[1] & UO4F_MASKC) +
                            ((lptr_src[1] & UO4F_MASKD) << 1)) << 8))&UO4F_MASKE)>>32);
                    lptr_src+=2;
                    lptr_dest++;
            case 3:
                    lptr_dest[0] = ((((lptr_src[0] & UO4F_MASKA) |
                            ((((lptr_src[0] & UO4F_MASKB) >> 48) - 16258) << 55)) +
                            (((lptr_src[0] & UO4F_MASKC) +
                            ((lptr_src[0] & UO4F_MASKD) << 1)) << 8)) & UO4F_MASKE) |
                            (((((lptr_src[1] & UO4F_MASKA) |
                            ((((lptr_src[1] & UO4F_MASKB) >> 48) - 16258) << 55)) +
                            (((lptr_src[1] & UO4F_MASKC) +
                            ((lptr_src[1] & UO4F_MASKD) << 1)) << 8))&UO4F_MASKE)>>32);
                    lptr_src+=2;
                    lptr_dest++;
            case 2:
                    lptr_dest[0] = ((((lptr_src[0] & UO4F_MASKA) |
                            ((((lptr_src[0] & UO4F_MASKB) >> 48) - 16258) << 55)) +
                            (((lptr_src[0] & UO4F_MASKC) +
                            ((lptr_src[0] & UO4F_MASKD) << 1)) << 8)) & UO4F_MASKE) |
                            (((((lptr_src[1] & UO4F_MASKA) |
                            ((((lptr_src[1] & UO4F_MASKB) >> 48) - 16258) << 55)) +
                            (((lptr_src[1] & UO4F_MASKC) +
                            ((lptr_src[1] & UO4F_MASKD) << 1)) << 8))&UO4F_MASKE)>>32);
                    lptr_src+=2;
                    lptr_dest++;
            case 1:
                    lptr_dest[0] = ((((lptr_src[0] & UO4F_MASKA) |
                            ((((lptr_src[0] & UO4F_MASKB) >> 48) - 16258) << 55)) +
                            (((lptr_src[0] & UO4F_MASKC) +
                            ((lptr_src[0] & UO4F_MASKD) << 1)) << 8)) & UO4F_MASKE) |
                            (((((lptr_src[1] & UO4F_MASKA) |
                            ((((lptr_src[1] & UO4F_MASKB) >> 48) - 16258) << 55)) +
                            (((lptr_src[1] & UO4F_MASKC) +
                            ((lptr_src[1] & UO4F_MASKD) << 1)) << 8))&UO4F_MASKE)>>32);
                    lptr_src+=2;
                    lptr_dest++;
                } while(--n>0);
		}
        if(odd_man_out)
            lptr_dest[0] = (((lptr_src[0] & UO4F_MASKA) |
                    ((((lptr_src[0] & UO4F_MASKB) >> 48) - 16258) << 55)) +
                    (((lptr_src[0] & UO4F_MASKC) +
                    ((lptr_src[0] & UO4F_MASKD) << 1)) << 8));
#endif  /* DUFF_uo4f */
      } /* end if */
    else { /* We end up here if we are doing stride based processing */
        buf1 = 0;
        for(i = 0; i < num_elm; i++) {
            dud1[0] = source[0];               /* Loop would be less efficient */
            dud1[1] = source[1];
            dud1[2] = source[2];
            dud1[3] = source[3];
            dud1[4] = source[4];
            dud1[5] = source[5];
            dud1[6] = source[6];
            dud1[7] = source[7];

            if((float)buf1 != 0)
                buf2 = (((buf1 & UO4F_MASKA) |
                        ((((buf1 & UO4F_MASKB) >> 48) -16258) << 55)) +
                        (((buf1 & UO4F_MASKC) + ((buf1 & UO4F_MASKD) << 1)) << 8));
            else
                buf2 = buf1;

            dest[0] = dud2[0];            /* Loop would be less efficient */
            dest[1] = dud2[1];
            dest[2] = dud2[2];
            dest[3] = dud2[3];

            source += source_stride;
            dest += dest_stride;
          } /* end for */
      } /* end else */
  return;
}

#define UI8F_MASKA 0x8000000000000000
#define UI8F_MASKB 0x7ff0000000000000
#define UI8F_MASKC 0x000fffffffffffff
#define UI8F_MASKD 0x0000000000000008
#define UI8F_MASKE 0x0000800000000000

/************************************************************/
/* DFKui8f()                                                */
/* -->Unicos routine for importing 64 bit floats            */
/************************************************************/

#ifdef PROTOTYPE
PRIVATE int DFKui8f(VOIDP s, VOIDP d, uint32 num_elm, uint32 source_stride,
		   uint32 dest_stride)
#else
PRIVATE int DFKui8f(source, dest, num_elm, source_stride, dest_stride)
uint8 * source, * dest;
uint32 num_elm, source_stride, dest_stride;
#endif /* PROTOTYPE */
{
  int fast_processing = 0;              /* By default not array processed */
  int i,j,n;
  long buf;                             /* This is a temporary stride buf */
  uint8 * dud = (uint8*)&buf;           /* Dummy pointer to buf1 for strides */
#ifdef PROTOTYPE
  uint8 * source = (uint8*)s;           /* Cray does not like certain   */
  uint8 * dest = (uint8*)d;             /* void and void* constructions */
#endif /* PROTOTYPE*/
  long * lptr_src = (long*)source;
  long * lptr_dest = (long*)dest;
  char *FUNC="DFKui8f";

  HEclear();

  /* Check for conversion errors */
  if(source == dest || num_elm == 0) { /* Inplace conversions not permitted */
    HERROR(DFE_BADCONV);               /* under UNICOS */
    return FAIL;                       /* No elements convert is an error   */
  }

  /* Find out if it is OK to use faster array processing */
  if(source_stride == 0 && dest_stride == 0) 
    fast_processing = 1;            

    if(fast_processing) {
#ifndef DUFF_ui8f
#if defined TEST2_ui8f
        n=num_elm;
        for(i = 0; i < n; i ++) {
            buf=lptr_src[0];
            lptr_dest[0] = ((buf & UI8F_MASKA) |
                    ((buf & UI8F_MASKB) >> 4) + (15362 << 48)) |
                    ((((buf & UI8F_MASKC) +
                    ((buf & UI8F_MASKD) << 1)) >> 5) | UI8F_MASKE);
            lptr_src++;
            lptr_dest++;
          } /* end for */
#elif defined TEST1_ui8f
        n=num_elm;
        for(i = 0; i < n; i ++) {
            lptr_dest[0] = ((lptr_src[0] & UI8F_MASKA) |
                    ((lptr_src[0] & UI8F_MASKB) >> 4) + (15362 << 48)) |
                    ((((lptr_src[0] & UI8F_MASKC) +
                    ((lptr_src[0] & UI8F_MASKD) << 1)) >> 5) | UI8F_MASKE);
            lptr_src++;
            lptr_dest++;
          } /* end for */
#else
        for(i = 0; i < num_elm; i ++) {
            if (lptr_src[0] != 0) {
                lptr_dest[0] = (((lptr_src[0] & UI8F_MASKA) |
                        ((lptr_src[0] & UI8F_MASKB) >> 4) + (15362 << 48)) |
                        ((((lptr_src[0] & UI8F_MASKC) +
                        ((lptr_src[0] & UI8F_MASKD) << 1)) >> 5) |
                        (UI8F_MASKE)) );
                if((lptr_dest[0] << 1) == 0)
                    lptr_dest[0] = 0;
              } /* end if */
            else
                lptr_dest[0] = 0;
            lptr_src++;
            lptr_dest++;
          } /* end for */
#endif
#else   /* DUFF_ui8f */
        n=(num_elm+7)/8;
        switch(num_elm%8) {
            case 0:
                do{
                    lptr_dest[0] = ((lptr_src[0] & UI8F_MASKA) |
                            ((lptr_src[0] & UI8F_MASKB) >> 4) + (15362 << 48)) |
                            ((((lptr_src[0] & UI8F_MASKC) +
                            ((lptr_src[0] & UI8F_MASKD) << 1)) >> 5) | UI8F_MASKE);
                    lptr_src++;
                    lptr_dest++;
#ifdef QAK
            case 15:
                    lptr_dest[0] = ((lptr_src[0] & UI8F_MASKA) |
                            ((lptr_src[0] & UI8F_MASKB) >> 4) + (15362 << 48)) |
                            ((((lptr_src[0] & UI8F_MASKC) +
                            ((lptr_src[0] & UI8F_MASKD) << 1)) >> 5) | UI8F_MASKE);
                    lptr_src++;
                    lptr_dest++;
            case 14:
                    lptr_dest[0] = ((lptr_src[0] & UI8F_MASKA) |
                            ((lptr_src[0] & UI8F_MASKB) >> 4) + (15362 << 48)) |
                            ((((lptr_src[0] & UI8F_MASKC) +
                            ((lptr_src[0] & UI8F_MASKD) << 1)) >> 5) | UI8F_MASKE);
                    lptr_src++;
                    lptr_dest++;
            case 13:
                    lptr_dest[0] = ((lptr_src[0] & UI8F_MASKA) |
                            ((lptr_src[0] & UI8F_MASKB) >> 4) + (15362 << 48)) |
                            ((((lptr_src[0] & UI8F_MASKC) +
                            ((lptr_src[0] & UI8F_MASKD) << 1)) >> 5) | UI8F_MASKE);
                    lptr_src++;
                    lptr_dest++;
            case 12:
                    lptr_dest[0] = ((lptr_src[0] & UI8F_MASKA) |
                            ((lptr_src[0] & UI8F_MASKB) >> 4) + (15362 << 48)) |
                            ((((lptr_src[0] & UI8F_MASKC) +
                            ((lptr_src[0] & UI8F_MASKD) << 1)) >> 5) | UI8F_MASKE);
                    lptr_src++;
                    lptr_dest++;
            case 11:
                    lptr_dest[0] = ((lptr_src[0] & UI8F_MASKA) |
                            ((lptr_src[0] & UI8F_MASKB) >> 4) + (15362 << 48)) |
                            ((((lptr_src[0] & UI8F_MASKC) +
                            ((lptr_src[0] & UI8F_MASKD) << 1)) >> 5) | UI8F_MASKE);
                    lptr_src++;
                    lptr_dest++;
            case 10:
                    lptr_dest[0] = ((lptr_src[0] & UI8F_MASKA) |
                            ((lptr_src[0] & UI8F_MASKB) >> 4) + (15362 << 48)) |
                            ((((lptr_src[0] & UI8F_MASKC) +
                            ((lptr_src[0] & UI8F_MASKD) << 1)) >> 5) | UI8F_MASKE);
                    lptr_src++;
                    lptr_dest++;
            case 9:
                    lptr_dest[0] = ((lptr_src[0] & UI8F_MASKA) |
                            ((lptr_src[0] & UI8F_MASKB) >> 4) + (15362 << 48)) |
                            ((((lptr_src[0] & UI8F_MASKC) +
                            ((lptr_src[0] & UI8F_MASKD) << 1)) >> 5) | UI8F_MASKE);
                    lptr_src++;
                    lptr_dest++;
            case 8:
                    lptr_dest[0] = ((lptr_src[0] & UI8F_MASKA) |
                            ((lptr_src[0] & UI8F_MASKB) >> 4) + (15362 << 48)) |
                            ((((lptr_src[0] & UI8F_MASKC) +
                            ((lptr_src[0] & UI8F_MASKD) << 1)) >> 5) | UI8F_MASKE);
                    lptr_src++;
                    lptr_dest++;
#endif
            case 7:
                    lptr_dest[0] = ((lptr_src[0] & UI8F_MASKA) |
                            ((lptr_src[0] & UI8F_MASKB) >> 4) + (15362 << 48)) |
                            ((((lptr_src[0] & UI8F_MASKC) +
                            ((lptr_src[0] & UI8F_MASKD) << 1)) >> 5) | UI8F_MASKE);
                    lptr_src++;
                    lptr_dest++;
            case 6:
                    lptr_dest[0] = ((lptr_src[0] & UI8F_MASKA) |
                            ((lptr_src[0] & UI8F_MASKB) >> 4) + (15362 << 48)) |
                            ((((lptr_src[0] & UI8F_MASKC) +
                            ((lptr_src[0] & UI8F_MASKD) << 1)) >> 5) | UI8F_MASKE);
                    lptr_src++;
                    lptr_dest++;
            case 5:
                    lptr_dest[0] = ((lptr_src[0] & UI8F_MASKA) |
                            ((lptr_src[0] & UI8F_MASKB) >> 4) + (15362 << 48)) |
                            ((((lptr_src[0] & UI8F_MASKC) +
                            ((lptr_src[0] & UI8F_MASKD) << 1)) >> 5) | UI8F_MASKE);
                    lptr_src++;
                    lptr_dest++;
            case 4:
                    lptr_dest[0] = ((lptr_src[0] & UI8F_MASKA) |
                            ((lptr_src[0] & UI8F_MASKB) >> 4) + (15362 << 48)) |
                            ((((lptr_src[0] & UI8F_MASKC) +
                            ((lptr_src[0] & UI8F_MASKD) << 1)) >> 5) | UI8F_MASKE);
                    lptr_src++;
                    lptr_dest++;
            case 3:
                    lptr_dest[0] = ((lptr_src[0] & UI8F_MASKA) |
                            ((lptr_src[0] & UI8F_MASKB) >> 4) + (15362 << 48)) |
                            ((((lptr_src[0] & UI8F_MASKC) +
                            ((lptr_src[0] & UI8F_MASKD) << 1)) >> 5) | UI8F_MASKE);
                    lptr_src++;
                    lptr_dest++;
            case 2:
                    lptr_dest[0] = ((lptr_src[0] & UI8F_MASKA) |
                            ((lptr_src[0] & UI8F_MASKB) >> 4) + (15362 << 48)) |
                            ((((lptr_src[0] & UI8F_MASKC) +
                            ((lptr_src[0] & UI8F_MASKD) << 1)) >> 5) | UI8F_MASKE);
                    lptr_src++;
                    lptr_dest++;
            case 1:
                    lptr_dest[0] = ((lptr_src[0] & UI8F_MASKA) |
                            ((lptr_src[0] & UI8F_MASKB) >> 4) + (15362 << 48)) |
                            ((((lptr_src[0] & UI8F_MASKC) +
                            ((lptr_src[0] & UI8F_MASKD) << 1)) >> 5) | UI8F_MASKE);
                    lptr_src++;
                    lptr_dest++;
                } while(--n>0);
		}
#endif  /* DUFF_ui8f */
      } /* end if */
    else
        for(i = 0; i < num_elm; i ++) {
            dud[0] = source[0];
            dud[1] = source[1];
            dud[2] = source[2];
            dud[3] = source[3];
            dud[4] = source[4];
            dud[5] = source[5];
            dud[6] = source[6];
            dud[7] = source[7];

            if (buf != 0) {
                buf = (((buf & UI8F_MASKA) |
                        ((buf & UI8F_MASKB) >> 4) + (15362 << 48)) |
                        ((((buf & UI8F_MASKC) + ((buf & UI8F_MASKD) << 1)) >> 5) |
                        (UI8F_MASKE)) );
                if ((buf << 1) == 0)
                    buf = 0;
              } /* end if */
            else
                buf = 0;

            dest[0] = dud[0];
            dest[1] = dud[1];
            dest[2] = dud[2];
            dest[3] = dud[3];
            dest[4] = dud[4];
            dest[5] = dud[5];
            dest[6] = dud[6];
            dest[7] = dud[7];

            source += source_stride;
            dest += dest_stride;
          } /* end for */
  return;
}

#define UO8F_MASKA 0x8000000000000000
#define UO8F_MASKB 0x7fff000000000000
#define UO8F_MASKC 0x00007fffffffffff

/************************************************************/
/* DFKuo8f()                                                */
/* -->Unicos routine for exporting 64 bit floats            */
/************************************************************/

#ifdef PROTOTYPE
PRIVATE int DFKuo8f(VOIDP s, VOIDP d, uint32 num_elm, uint32 source_stride,
		   uint32 dest_stride)
#else
PRIVATE int DFKuo8f(source, dest, num_elm, source_stride, dest_stride)
uint8 * source, * dest;
uint32 num_elm, source_stride, dest_stride;
#endif /* PROTOTYPE */
{
  int fast_processing = 0;              /* By default not array processed */
  int odd_man_out = 0;                  /* By default there are even num_elm */
  int i,j,n;
  long buf;                             /* This is a temporary stride buf */
  uint8 * dud = (uint8*)&buf;           /* Dummy pointer to buf1 for strides */
#ifdef PROTOTYPE
  uint8 * source = (uint8*)s;           /* Cray does not like certain   */
  uint8 * dest = (uint8*)d;             /* void and void* constructions */
#endif /* PROTOTYPE */
  long * lptr_src = (long*)source;
  long * lptr_dest = (long*)dest;
  char *FUNC="DFKuo8f";

  HEclear();

  /* Check for conversion errors */
  if(source == dest || num_elm == 0) { /* Inplace conversions not permitted */
    HERROR(DFE_BADCONV);               /* under UNICOS */
    return FAIL;                       /* No elements convert is an error   */
  }

  /* Find out if it is OK to use faster array processing */
  if(source_stride == 0 && dest_stride == 0)
      fast_processing = 1;            

    if(fast_processing) {
#ifndef DUFF_uo8f
#if defined TEST1_uo8f
        n=num_elm;
        for(i = 0; i < n; i ++) {
            lptr_dest[0] = ((lptr_src[0] & UO8F_MASKA) |
                    (((((lptr_src[0] & UO8F_MASKB) >> 48) - 15362) << 53) >> 1)) +
                    ((lptr_src[0] & UO8F_MASKC) << 5);
            lptr_src++;
            lptr_dest++;
          } /* end for */
#else
        for(i = 0; i < num_elm; i ++) {
            if (lptr_src[0] != 0)
                lptr_dest[0] = (((lptr_src[0] & UO8F_MASKA) |
                        (((((lptr_src[0] & UO8F_MASKB) >> 48) - 15362) << 53) >> 1)) +
                        ((lptr_src[0] & UO8F_MASKC) << 5));
            else
                lptr_dest[0] = 0;
            lptr_src++;
            lptr_dest++;
          } /* end for */
#endif
#else   /* DUFF_uo8f */
        n=(num_elm+7)/8;
        switch(num_elm%8) {
            case 0:
                do{
                    lptr_dest[0] = ((lptr_src[0] & UO8F_MASKA) |
                            (((((lptr_src[0] & UO8F_MASKB) >> 48) - 15362) << 53) >> 1)) +
                            ((lptr_src[0] & UO8F_MASKC) << 5);
                    lptr_src++;
                    lptr_dest++;
#ifdef QAK
            case 15:
                    lptr_dest[0] = ((lptr_src[0] & UO8F_MASKA) |
                            (((((lptr_src[0] & UO8F_MASKB) >> 48) - 15362) << 53) >> 1)) +
                            ((lptr_src[0] & UO8F_MASKC) << 5);
                    lptr_src++;
                    lptr_dest++;
            case 14:
                    lptr_dest[0] = ((lptr_src[0] & UO8F_MASKA) |
                            (((((lptr_src[0] & UO8F_MASKB) >> 48) - 15362) << 53) >> 1)) +
                            ((lptr_src[0] & UO8F_MASKC) << 5);
                    lptr_src++;
                    lptr_dest++;
            case 13:
                    lptr_dest[0] = ((lptr_src[0] & UO8F_MASKA) |
                            (((((lptr_src[0] & UO8F_MASKB) >> 48) - 15362) << 53) >> 1)) +
                            ((lptr_src[0] & UO8F_MASKC) << 5);
                    lptr_src++;
                    lptr_dest++;
            case 12:
                    lptr_dest[0] = ((lptr_src[0] & UO8F_MASKA) |
                            (((((lptr_src[0] & UO8F_MASKB) >> 48) - 15362) << 53) >> 1)) +
                            ((lptr_src[0] & UO8F_MASKC) << 5);
                    lptr_src++;
                    lptr_dest++;
            case 11:
                    lptr_dest[0] = ((lptr_src[0] & UO8F_MASKA) |
                            (((((lptr_src[0] & UO8F_MASKB) >> 48) - 15362) << 53) >> 1)) +
                            ((lptr_src[0] & UO8F_MASKC) << 5);
                    lptr_src++;
                    lptr_dest++;
            case 10:
                    lptr_dest[0] = ((lptr_src[0] & UO8F_MASKA) |
                            (((((lptr_src[0] & UO8F_MASKB) >> 48) - 15362) << 53) >> 1)) +
                            ((lptr_src[0] & UO8F_MASKC) << 5);
                    lptr_src++;
                    lptr_dest++;
            case 9:
                    lptr_dest[0] = ((lptr_src[0] & UO8F_MASKA) |
                            (((((lptr_src[0] & UO8F_MASKB) >> 48) - 15362) << 53) >> 1)) +
                            ((lptr_src[0] & UO8F_MASKC) << 5);
                    lptr_src++;
                    lptr_dest++;
            case 8:
                    lptr_dest[0] = ((lptr_src[0] & UO8F_MASKA) |
                            (((((lptr_src[0] & UO8F_MASKB) >> 48) - 15362) << 53) >> 1)) +
                            ((lptr_src[0] & UO8F_MASKC) << 5);
                    lptr_src++;
                    lptr_dest++;
#endif
            case 7:
                    lptr_dest[0] = ((lptr_src[0] & UO8F_MASKA) |
                            (((((lptr_src[0] & UO8F_MASKB) >> 48) - 15362) << 53) >> 1)) +
                            ((lptr_src[0] & UO8F_MASKC) << 5);
                    lptr_src++;
                    lptr_dest++;
            case 6:
                    lptr_dest[0] = ((lptr_src[0] & UO8F_MASKA) |
                            (((((lptr_src[0] & UO8F_MASKB) >> 48) - 15362) << 53) >> 1)) +
                            ((lptr_src[0] & UO8F_MASKC) << 5);
                    lptr_src++;
                    lptr_dest++;
            case 5:
                    lptr_dest[0] = ((lptr_src[0] & UO8F_MASKA) |
                            (((((lptr_src[0] & UO8F_MASKB) >> 48) - 15362) << 53) >> 1)) +
                            ((lptr_src[0] & UO8F_MASKC) << 5);
                    lptr_src++;
                    lptr_dest++;
            case 4:
                    lptr_dest[0] = ((lptr_src[0] & UO8F_MASKA) |
                            (((((lptr_src[0] & UO8F_MASKB) >> 48) - 15362) << 53) >> 1)) +
                            ((lptr_src[0] & UO8F_MASKC) << 5);
                    lptr_src++;
                    lptr_dest++;
            case 3:
                    lptr_dest[0] = ((lptr_src[0] & UO8F_MASKA) |
                            (((((lptr_src[0] & UO8F_MASKB) >> 48) - 15362) << 53) >> 1)) +
                            ((lptr_src[0] & UO8F_MASKC) << 5);
                    lptr_src++;
                    lptr_dest++;
            case 2:
                    lptr_dest[0] = ((lptr_src[0] & UO8F_MASKA) |
                            (((((lptr_src[0] & UO8F_MASKB) >> 48) - 15362) << 53) >> 1)) +
                            ((lptr_src[0] & UO8F_MASKC) << 5);
                    lptr_src++;
                    lptr_dest++;
            case 1:
                    lptr_dest[0] = ((lptr_src[0] & UO8F_MASKA) |
                            (((((lptr_src[0] & UO8F_MASKB) >> 48) - 15362) << 53) >> 1)) +
                            ((lptr_src[0] & UO8F_MASKC) << 5);
                    lptr_src++;
                    lptr_dest++;
                } while(--n>0);
		}
#endif  /* DUFF_uo8f */
      } /* end if */
    else
        for(i = 0; i < num_elm; i ++) {
            dud[0] = source[0];
            dud[1] = source[1];
            dud[2] = source[2];
            dud[3] = source[3];
            dud[4] = source[4];
            dud[5] = source[5];
            dud[6] = source[6];
            dud[7] = source[7];

            if (buf != 0) {
                buf = (((buf & UO8F_MASKA) |                             /* sign bit */
                        (((((buf & UO8F_MASKB) >> 48) - 15362) << 53) >> 1)) |   /* exp */
                        ((buf & UO8F_MASKC) << 5));                       /* mantissa */
              } /* end if */
            else
                buf = 0;

            dest[0] = dud[0];
            dest[1] = dud[1];
            dest[2] = dud[2];
            dest[3] = dud[3];
            dest[4] = dud[4];
            dest[5] = dud[5];
            dest[6] = dud[6];
            dest[7] = dud[7];

            source += source_stride;
            dest += dest_stride;
          } /* end for */
  return;
}

#define LUI2I_MASKA1 0xff00000000000000
#define LUI2I_MASKA2 0x00ff000000000000
#define LUI2I_MASKB1 0x0000ff0000000000
#define LUI2I_MASKB2 0x000000ff00000000
#define LUI2I_MASKC1 0x00000000ff000000
#define LUI2I_MASKC2 0x0000000000ff0000
#define LUI2I_MASKD1 0x000000000000ff00
#define LUI2I_MASKD2 0x00000000000000ff

/* QAK */
/************************************************************/
/* DFKlui2i()                                               */
/* -->Unicos routine for importing 2 byte data items        */ 
/* (**) This routine converts two byte little-endian IEEE   */
/*      to eight byte Cray big endian integer.              */
/************************************************************/
#ifdef PROTOTYPE
PRIVATE int DFKlui2i(VOIDP s, VOIDP d, uint32 num_elm, uint32 source_stride,
		   uint32 dest_stride)
#else
PRIVATE int DFKlui2i(source, dest, num_elm, source_stride, dest_stride)
uint8 * source, * dest;
uint32 num_elm, source_stride, dest_stride;
#endif /* PROTOTYPE */
{
  register uint32 i;
  int fast_processing=0;
#ifdef PROTOTYPE
  uint8 * source = (uint8*)s;
  uint8 * dest = (uint8*)d;
#endif /* PROTOTYPE */
  long * lptr_dest = (long*)dest;
    long *lp_dest;
    long *lp_src;
  char *FUNC="DFKui2i";

  HEclear();

  if(source == dest || num_elm == 0) {  /* Inplace conversions not permitted */
    HERROR(DFE_BADCONV);                /* No elements is an error */
    return FAIL;
  }

  /* Find out if it is OK to use faster array processing */
  if(source_stride == 0 && dest_stride == 0) 
      fast_processing = 1;            

  if(fast_processing) {
#ifndef DUFF_lui2i
#if defined TEST2_lui2i
        int odd_man_out;        /* By default there are even num_elm */
        intn n;

        odd_man_out = num_elm%4;

        n=num_elm/4;
        lp_dest=(long *)dest;
        lp_src=(long *)source;
        HDmemset(lp_dest,0,num_elm*sizeof(long));
        for(i = 0; i < n; i++) {
            lp_dest[0]=((lp_src[0]&LUI2I_MASKA1)>>56) |
                    ((lp_src[0]&LUI2I_MASKA2)>>40);
            lp_dest[1]=((lp_src[0]&LUI2I_MASKB1)>>40) |
                    ((lp_src[0]&LUI2I_MASKB2)>>24);
            lp_dest[2]=((lp_src[0]&LUI2I_MASKC1)>>24) |
                    ((lp_src[0]&LUI2I_MASKC2)>>8);
            lp_dest[3]=((lp_src[0]&LUI2I_MASKD1)>>8) |
                    ((lp_src[0]&LUI2I_MASKD2)<<8);
            lp_dest+=4;
            lp_src++;
          } /* end for */
        switch(odd_man_out) {
            case 3:
                lp_dest[0]=((lp_src[0]&LUI2I_MASKA1)>>56) |
                        ((lp_src[0]&LUI2I_MASKA2)>>40);
                lp_dest[1]=((lp_src[0]&LUI2I_MASKB1)>>40) |
                        ((lp_src[0]&LUI2I_MASKB2)>>24);
                lp_dest[2]=((lp_src[0]&LUI2I_MASKC1)>>24) |
                        ((lp_src[0]&LUI2I_MASKC2)>>8);
                break;

            case 2:
                lp_dest[0]=((lp_src[0]&LUI2I_MASKA1)>>56) |
                        ((lp_src[0]&LUI2I_MASKA2)>>40);
                lp_dest[1]=((lp_src[0]&LUI2I_MASKB1)>>40) |
                        ((lp_src[0]&LUI2I_MASKB2)>>24);
                break;

            case 1:
                lp_dest[0]=((lp_src[0]&LUI2I_MASKA1)>>56) |
                        ((lp_src[0]&LUI2I_MASKA2)>>40);
                break;

            default:
                break;
          } /* end switch */
#elif defined TEST1_lui2i
        int odd_man_out;        /* By default there are even num_elm */
        intn n;

        odd_man_out = num_elm%4;

        n=num_elm/4;
        lp_dest=(long *)dest;
        lp_src=(long *)source;
        HDmemset(lp_dest,0,num_elm*sizeof(long));
        for(i = 0; i < n; i++) {
            *lp_dest++=((lp_src[0]&LUI2I_MASKA1)>>56) |
                    ((lp_src[0]&LUI2I_MASKA2)>>40);
            *lp_dest++=((lp_src[0]&LUI2I_MASKB1)>>40) |
                    ((lp_src[0]&LUI2I_MASKB2)>>24);
            *lp_dest++=((lp_src[0]&LUI2I_MASKC1)>>24) |
                    ((lp_src[0]&LUI2I_MASKC2)>>8);
            *lp_dest++=((lp_src[0]&LUI2I_MASKD1)>>8) |
                    ((lp_src[0]&LUI2I_MASKD2)<<8);
            lp_src++;
          } /* end for */
        switch(odd_man_out) {
            case 3:
                *lp_dest++=((lp_src[0]&LUI2I_MASKA1)>>56) |
                        ((lp_src[0]&LUI2I_MASKA2)>>40);
                *lp_dest++=((lp_src[0]&LUI2I_MASKB1)>>40) |
                        ((lp_src[0]&LUI2I_MASKB2)>>24);
                *lp_dest++=((lp_src[0]&LUI2I_MASKC1)>>24) |
                        ((lp_src[0]&LUI2I_MASKC2)>>8);
                break;

            case 2:
                *lp_dest++=((lp_src[0]&LUI2I_MASKA1)>>56) |
                        ((lp_src[0]&LUI2I_MASKA2)>>40);
                *lp_dest++=((lp_src[0]&LUI2I_MASKB1)>>40) |
                        ((lp_src[0]&LUI2I_MASKB2)>>24);
                break;

            case 1:
                *lp_dest++=((lp_src[0]&LUI2I_MASKA1)>>56) |
                        ((lp_src[0]&LUI2I_MASKA2)>>40);
                break;

            default:
                break;
          } /* end switch */
#else
    for(i = 0; i < num_elm; i++) {
      lptr_dest[0] = 0x0000000000000000;
      dest[6] = source[1];
      dest[7] = source[0];
      source += 2;
      lptr_dest++;
      dest = (uint8*)lptr_dest;
    }
#endif
#else   /* DUFF_lui2i */
        uintn n;
        int odd_man_out;        /* By default there are even num_elm */

        lp_dest=(long *)dest;
        lp_src=(long *)source;
        HDmemset(lp_dest,0,num_elm*sizeof(long));

        odd_man_out = num_elm % 4;

        num_elm/=4;
        n=(num_elm+7)/8;
        switch(num_elm%8) {
            case 0:
                do{
                    lp_dest[0]=((lp_src[0]&LUI2I_MASKA1)>>56) |
                            ((lp_src[0]&LUI2I_MASKA2)>>40);
                    lp_dest[1]=((lp_src[0]&LUI2I_MASKB1)>>40) |
                            ((lp_src[0]&LUI2I_MASKB2)>>24);
                    lp_dest[2]=((lp_src[0]&LUI2I_MASKC1)>>24) |
                            ((lp_src[0]&LUI2I_MASKC2)>>8);
                    lp_dest[3]=((lp_src[0]&LUI2I_MASKD1)>>8) |
                            ((lp_src[0]&LUI2I_MASKD2)<<8);
                    lp_dest+=4;
                    lp_src++;
#ifdef QAK
            case 15:
                    lp_dest[0]=((lp_src[0]&LUI2I_MASKA1)>>56) |
                            ((lp_src[0]&LUI2I_MASKA2)>>40);
                    lp_dest[1]=((lp_src[0]&LUI2I_MASKB1)>>40) |
                            ((lp_src[0]&LUI2I_MASKB2)>>24);
                    lp_dest[2]=((lp_src[0]&LUI2I_MASKC1)>>24) |
                            ((lp_src[0]&LUI2I_MASKC2)>>8);
                    lp_dest[3]=((lp_src[0]&LUI2I_MASKD1)>>8) |
                            ((lp_src[0]&LUI2I_MASKD2)<<8);
                    lp_dest+=4;
                    lp_src++;
            case 14:
                    lp_dest[0]=((lp_src[0]&LUI2I_MASKA1)>>56) |
                            ((lp_src[0]&LUI2I_MASKA2)>>40);
                    lp_dest[1]=((lp_src[0]&LUI2I_MASKB1)>>40) |
                            ((lp_src[0]&LUI2I_MASKB2)>>24);
                    lp_dest[2]=((lp_src[0]&LUI2I_MASKC1)>>24) |
                            ((lp_src[0]&LUI2I_MASKC2)>>8);
                    lp_dest[3]=((lp_src[0]&LUI2I_MASKD1)>>8) |
                            ((lp_src[0]&LUI2I_MASKD2)<<8);
                    lp_dest+=4;
                    lp_src++;
            case 13:
                    lp_dest[0]=((lp_src[0]&LUI2I_MASKA1)>>56) |
                            ((lp_src[0]&LUI2I_MASKA2)>>40);
                    lp_dest[1]=((lp_src[0]&LUI2I_MASKB1)>>40) |
                            ((lp_src[0]&LUI2I_MASKB2)>>24);
                    lp_dest[2]=((lp_src[0]&LUI2I_MASKC1)>>24) |
                            ((lp_src[0]&LUI2I_MASKC2)>>8);
                    lp_dest[3]=((lp_src[0]&LUI2I_MASKD1)>>8) |
                            ((lp_src[0]&LUI2I_MASKD2)<<8);
                    lp_dest+=4;
                    lp_src++;
            case 12:
                    lp_dest[0]=((lp_src[0]&LUI2I_MASKA1)>>56) |
                            ((lp_src[0]&LUI2I_MASKA2)>>40);
                    lp_dest[1]=((lp_src[0]&LUI2I_MASKB1)>>40) |
                            ((lp_src[0]&LUI2I_MASKB2)>>24);
                    lp_dest[2]=((lp_src[0]&LUI2I_MASKC1)>>24) |
                            ((lp_src[0]&LUI2I_MASKC2)>>8);
                    lp_dest[3]=((lp_src[0]&LUI2I_MASKD1)>>8) |
                            ((lp_src[0]&LUI2I_MASKD2)<<8);
                    lp_dest+=4;
                    lp_src++;
            case 11:
                    lp_dest[0]=((lp_src[0]&LUI2I_MASKA1)>>56) |
                            ((lp_src[0]&LUI2I_MASKA2)>>40);
                    lp_dest[1]=((lp_src[0]&LUI2I_MASKB1)>>40) |
                            ((lp_src[0]&LUI2I_MASKB2)>>24);
                    lp_dest[2]=((lp_src[0]&LUI2I_MASKC1)>>24) |
                            ((lp_src[0]&LUI2I_MASKC2)>>8);
                    lp_dest[3]=((lp_src[0]&LUI2I_MASKD1)>>8) |
                            ((lp_src[0]&LUI2I_MASKD2)<<8);
                    lp_dest+=4;
                    lp_src++;
            case 10:
                    lp_dest[0]=((lp_src[0]&LUI2I_MASKA1)>>56) |
                            ((lp_src[0]&LUI2I_MASKA2)>>40);
                    lp_dest[1]=((lp_src[0]&LUI2I_MASKB1)>>40) |
                            ((lp_src[0]&LUI2I_MASKB2)>>24);
                    lp_dest[2]=((lp_src[0]&LUI2I_MASKC1)>>24) |
                            ((lp_src[0]&LUI2I_MASKC2)>>8);
                    lp_dest[3]=((lp_src[0]&LUI2I_MASKD1)>>8) |
                            ((lp_src[0]&LUI2I_MASKD2)<<8);
                    lp_dest+=4;
                    lp_src++;
            case 9:
                    lp_dest[0]=((lp_src[0]&LUI2I_MASKA1)>>56) |
                            ((lp_src[0]&LUI2I_MASKA2)>>40);
                    lp_dest[1]=((lp_src[0]&LUI2I_MASKB1)>>40) |
                            ((lp_src[0]&LUI2I_MASKB2)>>24);
                    lp_dest[2]=((lp_src[0]&LUI2I_MASKC1)>>24) |
                            ((lp_src[0]&LUI2I_MASKC2)>>8);
                    lp_dest[3]=((lp_src[0]&LUI2I_MASKD1)>>8) |
                            ((lp_src[0]&LUI2I_MASKD2)<<8);
                    lp_dest+=4;
                    lp_src++;
            case 8:
                    lp_dest[0]=((lp_src[0]&LUI2I_MASKA1)>>56) |
                            ((lp_src[0]&LUI2I_MASKA2)>>40);
                    lp_dest[1]=((lp_src[0]&LUI2I_MASKB1)>>40) |
                            ((lp_src[0]&LUI2I_MASKB2)>>24);
                    lp_dest[2]=((lp_src[0]&LUI2I_MASKC1)>>24) |
                            ((lp_src[0]&LUI2I_MASKC2)>>8);
                    lp_dest[3]=((lp_src[0]&LUI2I_MASKD1)>>8) |
                            ((lp_src[0]&LUI2I_MASKD2)<<8);
                    lp_dest+=4;
                    lp_src++;
#endif
            case 7:
                    lp_dest[0]=((lp_src[0]&LUI2I_MASKA1)>>56) |
                            ((lp_src[0]&LUI2I_MASKA2)>>40);
                    lp_dest[1]=((lp_src[0]&LUI2I_MASKB1)>>40) |
                            ((lp_src[0]&LUI2I_MASKB2)>>24);
                    lp_dest[2]=((lp_src[0]&LUI2I_MASKC1)>>24) |
                            ((lp_src[0]&LUI2I_MASKC2)>>8);
                    lp_dest[3]=((lp_src[0]&LUI2I_MASKD1)>>8) |
                            ((lp_src[0]&LUI2I_MASKD2)<<8);
                    lp_dest+=4;
                    lp_src++;
            case 6:
                    lp_dest[0]=((lp_src[0]&LUI2I_MASKA1)>>56) |
                            ((lp_src[0]&LUI2I_MASKA2)>>40);
                    lp_dest[1]=((lp_src[0]&LUI2I_MASKB1)>>40) |
                            ((lp_src[0]&LUI2I_MASKB2)>>24);
                    lp_dest[2]=((lp_src[0]&LUI2I_MASKC1)>>24) |
                            ((lp_src[0]&LUI2I_MASKC2)>>8);
                    lp_dest[3]=((lp_src[0]&LUI2I_MASKD1)>>8) |
                            ((lp_src[0]&LUI2I_MASKD2)<<8);
                    lp_dest+=4;
                    lp_src++;
            case 5:
                    lp_dest[0]=((lp_src[0]&LUI2I_MASKA1)>>56) |
                            ((lp_src[0]&LUI2I_MASKA2)>>40);
                    lp_dest[1]=((lp_src[0]&LUI2I_MASKB1)>>40) |
                            ((lp_src[0]&LUI2I_MASKB2)>>24);
                    lp_dest[2]=((lp_src[0]&LUI2I_MASKC1)>>24) |
                            ((lp_src[0]&LUI2I_MASKC2)>>8);
                    lp_dest[3]=((lp_src[0]&LUI2I_MASKD1)>>8) |
                            ((lp_src[0]&LUI2I_MASKD2)<<8);
                    lp_dest+=4;
                    lp_src++;
            case 4:
                    lp_dest[0]=((lp_src[0]&LUI2I_MASKA1)>>56) |
                            ((lp_src[0]&LUI2I_MASKA2)>>40);
                    lp_dest[1]=((lp_src[0]&LUI2I_MASKB1)>>40) |
                            ((lp_src[0]&LUI2I_MASKB2)>>24);
                    lp_dest[2]=((lp_src[0]&LUI2I_MASKC1)>>24) |
                            ((lp_src[0]&LUI2I_MASKC2)>>8);
                    lp_dest[3]=((lp_src[0]&LUI2I_MASKD1)>>8) |
                            ((lp_src[0]&LUI2I_MASKD2)<<8);
                    lp_dest+=4;
                    lp_src++;
            case 3:
                    lp_dest[0]=((lp_src[0]&LUI2I_MASKA1)>>56) |
                            ((lp_src[0]&LUI2I_MASKA2)>>40);
                    lp_dest[1]=((lp_src[0]&LUI2I_MASKB1)>>40) |
                            ((lp_src[0]&LUI2I_MASKB2)>>24);
                    lp_dest[2]=((lp_src[0]&LUI2I_MASKC1)>>24) |
                            ((lp_src[0]&LUI2I_MASKC2)>>8);
                    lp_dest[3]=((lp_src[0]&LUI2I_MASKD1)>>8) |
                            ((lp_src[0]&LUI2I_MASKD2)<<8);
                    lp_dest+=4;
                    lp_src++;
            case 2:
                    lp_dest[0]=((lp_src[0]&LUI2I_MASKA1)>>56) |
                            ((lp_src[0]&LUI2I_MASKA2)>>40);
                    lp_dest[1]=((lp_src[0]&LUI2I_MASKB1)>>40) |
                            ((lp_src[0]&LUI2I_MASKB2)>>24);
                    lp_dest[2]=((lp_src[0]&LUI2I_MASKC1)>>24) |
                            ((lp_src[0]&LUI2I_MASKC2)>>8);
                    lp_dest[3]=((lp_src[0]&LUI2I_MASKD1)>>8) |
                            ((lp_src[0]&LUI2I_MASKD2)<<8);
                    lp_dest+=4;
                    lp_src++;
            case 1:
                    lp_dest[0]=((lp_src[0]&LUI2I_MASKA1)>>56) |
                            ((lp_src[0]&LUI2I_MASKA2)>>40);
                    lp_dest[1]=((lp_src[0]&LUI2I_MASKB1)>>40) |
                            ((lp_src[0]&LUI2I_MASKB2)>>24);
                    lp_dest[2]=((lp_src[0]&LUI2I_MASKC1)>>24) |
                            ((lp_src[0]&LUI2I_MASKC2)>>8);
                    lp_dest[3]=((lp_src[0]&LUI2I_MASKD1)>>8) |
                            ((lp_src[0]&LUI2I_MASKD2)<<8);
                    lp_dest+=4;
                    lp_src++;
                } while(--n>0);
		}
        switch(odd_man_out) {
            case 3:
                lp_dest[0]=((lp_src[0]&LUI2I_MASKA1)>>56) |
                        ((lp_src[0]&LUI2I_MASKA2)>>40);
                lp_dest[1]=((lp_src[0]&LUI2I_MASKB1)>>40) |
                        ((lp_src[0]&LUI2I_MASKB2)>>24);
                lp_dest[2]=((lp_src[0]&LUI2I_MASKC1)>>24) |
                        ((lp_src[0]&LUI2I_MASKC2)>>8);
                break;

            case 2:
                lp_dest[0]=((lp_src[0]&LUI2I_MASKA1)>>56) |
                        ((lp_src[0]&LUI2I_MASKA2)>>40);
                lp_dest[1]=((lp_src[0]&LUI2I_MASKB1)>>40) |
                        ((lp_src[0]&LUI2I_MASKB2)>>24);
                break;

            case 1:
                lp_dest[0]=((lp_src[0]&LUI2I_MASKA1)>>56) |
                        ((lp_src[0]&LUI2I_MASKA2)>>40);
                break;

            default:
                break;
          } /* end switch */
#endif  /* DUFF_lui2i */
  }
  else { /* Generic stride processing */
    for(i = 0; i < num_elm; i++) {
      dest[0] = 0x00;
      dest[1] = 0x00;
      dest[2] = 0x00;
      dest[3] = 0x00;
      dest[4] = 0x00;
      dest[5] = 0x00;
      dest[6] = source[1];
      dest[7] = source[0];
      source += source_stride;
      dest += dest_stride;
    }
  }
  return 0;
}

#define LUI2S_MASKA1 0xff00000000000000
#define LUI2S_MASKA2 0x00ff000000000000
#define LUI2S_MASKB1 0x0000ff0000000000
#define LUI2S_MASKB2 0x000000ff00000000
#define LUI2S_MASKC1 0x00000000ff000000
#define LUI2S_MASKC2 0x0000000000ff0000
#define LUI2S_MASKD1 0x000000000000ff00
#define LUI2S_MASKD2 0x00000000000000ff
#define LUI2S_MASKE  0x0080000000000000
#define LUI2S_MASKF  0x0000008000000000
#define LUI2S_MASKG  0x0000000000800000
#define LUI2S_MASKH  0x0000000000000080

/************************************************************/
/* DFKlui2s()                                                */
/* -->Unicos routine for importing 2 byte signed ints       */
/* (**) This routine converts two byte IEEE to eight byte   */
/*      Cray.                                               */
/************************************************************/
#ifdef PROTOTYPE
PRIVATE int DFKlui2s(VOIDP s, VOIDP d, uint32 num_elm, uint32 source_stride,
		   uint32 dest_stride)
#else
PRIVATE int DFKlui2s(source, dest, num_elm, source_stride, dest_stride)
uint8 * source, * dest;
uint32 num_elm, source_stride, dest_stride;
#endif /* PROTOTYPE */
{
  register uint32 i;
  int fast_processing=0;
#ifdef PROTOTYPE
  uint8 * source = (uint8*)s;
  uint8 * dest = (uint8*)d;
#endif /* PROTOTYPE */
  long * lptr_dest = (long*)dest;
    long *lp_dest;
    long *lp_src;
  char *FUNC="DFKui2s";

  HEclear();

  if(source == dest || num_elm == 0) {  /* Inplace conversions  not permitted */
    HERROR(DFE_BADCONV);                /* No elements to convert is an error */
    return FAIL;
  }

  /* Find out if it is OK to use faster array processing */
  if(source_stride == 0 && dest_stride == 0) 
      fast_processing = 1;            

  if(fast_processing) {
#ifndef DUFF_lui2s
#if defined TEST2_lui2s
        int odd_man_out;        /* By default there are even num_elm */
        intn n;

        odd_man_out = num_elm%4;

        n=num_elm/4;
        lp_dest=(long *)dest;
        lp_src=(long *)source;
        HDmemset(lp_dest,0,num_elm*sizeof(long));
        for(i = 0; i < n; i++) {
            if(lp_src[0] & LUI2S_MASKE)     /* Can't forget to extend sign */
                lp_dest[0] = 0xffffffffffffffff;
            lp_dest[0]=((lp_src[0]&LUI2S_MASKA1)>>56) |
                    ((lp_src[0]&LUI2S_MASKA2)>>40);
            if(lp_src[0] & LUI2S_MASKF)     /* Can't forget to extend sign */
                lp_dest[1] = 0xffffffffffffffff;
            lp_dest[1]=((lp_src[0]&LUI2S_MASKB1)>>40) |
                    ((lp_src[0]&LUI2S_MASKB2)>>24);
            if(lp_src[0] & LUI2S_MASKG)     /* Can't forget to extend sign */
                lp_dest[2] = 0xffffffffffffffff;
            lp_dest[2]=((lp_src[0]&LUI2S_MASKC1)>>24) |
                    ((lp_src[0]&LUI2S_MASKC2)>>8);
            if(lp_src[0] & LUI2S_MASKH)     /* Can't forget to extend sign */
                lp_dest[3] = 0xffffffffffffffff;
            lp_dest[3]=((lp_src[0]&LUI2S_MASKD1)>>8) |
                    ((lp_src[0]&LUI2S_MASKD2)<<8);
            lp_dest+=4;
            lp_src++;
          } /* end for */
        switch(odd_man_out) {
            case 3:
                if(lp_src[0] & LUI2S_MASKE)     /* Can't forget to extend sign */
                    lp_dest[0] = 0xffffffffffffffff;
                lp_dest[0]=((lp_src[0]&LUI2S_MASKA1)>>56) |
                        ((lp_src[0]&LUI2S_MASKA2)>>40);
                if(lp_src[0] & LUI2S_MASKF)     /* Can't forget to extend sign */
                    lp_dest[1] = 0xffffffffffffffff;
                lp_dest[1]=((lp_src[0]&LUI2S_MASKB1)>>40) |
                        ((lp_src[0]&LUI2S_MASKB2)>>24);
                if(lp_src[0] & LUI2S_MASKG)     /* Can't forget to extend sign */
                    lp_dest[2] = 0xffffffffffffffff;
                lp_dest[2]=((lp_src[0]&LUI2S_MASKC1)>>24) |
                        ((lp_src[0]&LUI2S_MASKC2)>>8);
                break;

            case 2:
                if(lp_src[0] & LUI2S_MASKE)     /* Can't forget to extend sign */
                    lp_dest[0] = 0xffffffffffffffff;
                lp_dest[0]=((lp_src[0]&LUI2S_MASKA1)>>56) |
                        ((lp_src[0]&LUI2S_MASKA2)>>40);
                if(lp_src[0] & LUI2S_MASKF)     /* Can't forget to extend sign */
                    lp_dest[1] = 0xffffffffffffffff;
                lp_dest[1]=((lp_src[0]&LUI2S_MASKB1)>>40) |
                        ((lp_src[0]&LUI2S_MASKB2)>>24);
                break;

            case 1:
                if(lp_src[0] & LUI2S_MASKE)     /* Can't forget to extend sign */
                    lp_dest[0] = 0xffffffffffffffff;
                lp_dest[0]=((lp_src[0]&LUI2S_MASKA1)>>56) |
                        ((lp_src[0]&LUI2S_MASKA2)>>40);
                break;

            default:
                break;
          } /* end switch */
#elif defined TEST1_lui2s
        int odd_man_out;        /* By default there are even num_elm */
        intn n;

        odd_man_out = num_elm%4;

        n=num_elm/4;
        lp_dest=(long *)dest;
        lp_src=(long *)source;
        HDmemset(lp_dest,0,num_elm*sizeof(long));
        for(i = 0; i < n; i++) {
            if(lp_src[0] & LUI2S_MASKE)     /* Can't forget to extend sign */
                *lp_dest = 0xffffffffffffffff;
            *lp_dest++=((lp_src[0]&LUI2S_MASKA1)>>56) |
                    ((lp_src[0]&LUI2S_MASKA2)>>40);
            if(lp_src[0] & LUI2S_MASKF)     /* Can't forget to extend sign */
                *lp_dest = 0xffffffffffffffff;
            *lp_dest++=((lp_src[0]&LUI2S_MASKB1)>>40) |
                    ((lp_src[0]&LUI2S_MASKB2)>>24);
            if(lp_src[0] & LUI2S_MASKG)     /* Can't forget to extend sign */
                *lp_dest = 0xffffffffffffffff;
            *lp_dest++=((lp_src[0]&LUI2S_MASKC1)>>24) |
                    ((lp_src[0]&LUI2S_MASKC2)>>8);
            if(lp_src[0] & LUI2S_MASKH)     /* Can't forget to extend sign */
                *lp_dest = 0xffffffffffffffff;
            *lp_dest++=((lp_src[0]&LUI2S_MASKD1)>>8) |
                    ((lp_src[0]&LUI2S_MASKD2)<<8);
            lp_src++;
          } /* end for */
        switch(odd_man_out) {
            case 3:
                if(lp_src[0] & LUI2S_MASKE)     /* Can't forget to extend sign */
                    *lp_dest = 0xffffffffffffffff;
                *lp_dest++=((lp_src[0]&LUI2S_MASKA1)>>56) |
                        ((lp_src[0]&LUI2S_MASKA2)>>40);
                if(lp_src[0] & LUI2S_MASKF)     /* Can't forget to extend sign */
                    *lp_dest = 0xffffffffffffffff;
                *lp_dest++=((lp_src[0]&LUI2S_MASKB1)>>40) |
                        ((lp_src[0]&LUI2S_MASKB2)>>24);
                if(lp_src[0] & LUI2S_MASKG)     /* Can't forget to extend sign */
                    *lp_dest = 0xffffffffffffffff;
                *lp_dest++=((lp_src[0]&LUI2S_MASKC1)>>24) |
                        ((lp_src[0]&LUI2S_MASKC2)>>8);
                break;

            case 2:
                if(lp_src[0] & LUI2S_MASKE)     /* Can't forget to extend sign */
                    *lp_dest = 0xffffffffffffffff;
                *lp_dest++=((lp_src[0]&LUI2S_MASKA1)>>56) |
                        ((lp_src[0]&LUI2S_MASKA2)>>40);
                if(lp_src[0] & LUI2S_MASKF)     /* Can't forget to extend sign */
                    *lp_dest = 0xffffffffffffffff;
                *lp_dest++=((lp_src[0]&LUI2S_MASKB1)>>40) |
                        ((lp_src[0]&LUI2S_MASKB2)>>24);
                break;

            case 1:
                if(lp_src[0] & LUI2S_MASKE)     /* Can't forget to extend sign */
                    *lp_dest = 0xffffffffffffffff;
                *lp_dest++=((lp_src[0]&LUI2S_MASKA1)>>56) |
                        ((lp_src[0]&LUI2S_MASKA2)>>40);
                break;

            default:
                break;
          } /* end switch */
#else
    for(i = 0; i < num_elm; i++) {
      if((source[1] & 0x80))           /* Can't forget to extend sign */
	lptr_dest[0] = 0xffffffffffffffff;
      else
	lptr_dest[0] = 0x0000000000000000;
      dest[6] = source[1];
      dest[7] = source[0];
      source += 2;
      lptr_dest++;
      dest = (uint8*)lptr_dest;
    }
#endif
#else   /* DUFF_lui2s */
        uintn n;
        int odd_man_out;        /* By default there are even num_elm */

        lp_dest=(long *)dest;
        lp_src=(long *)source;
        HDmemset(lp_dest,0,num_elm*sizeof(long));

        odd_man_out = num_elm % 4;

        num_elm/=4;
        n=(num_elm+7)/8;
        switch(num_elm%8) {
            case 0:
                do{
                    if(lp_src[0] & LUI2S_MASKE)     /* Can't forget to extend sign */
                        lp_dest[0] = 0xffffffffffffffff;
                    lp_dest[0]=((lp_src[0]&LUI2S_MASKA1)>>56) |
                            ((lp_src[0]&LUI2S_MASKA2)>>40);
                    if(lp_src[0] & LUI2S_MASKF)     /* Can't forget to extend sign */
                        lp_dest[1] = 0xffffffffffffffff;
                    lp_dest[1]=((lp_src[0]&LUI2S_MASKB1)>>40) |
                            ((lp_src[0]&LUI2S_MASKB2)>>24);
                    if(lp_src[0] & LUI2S_MASKG)     /* Can't forget to extend sign */
                        lp_dest[2] = 0xffffffffffffffff;
                    lp_dest[2]=((lp_src[0]&LUI2S_MASKC1)>>24) |
                            ((lp_src[0]&LUI2S_MASKC2)>>8);
                    if(lp_src[0] & LUI2S_MASKH)     /* Can't forget to extend sign */
                        lp_dest[3] = 0xffffffffffffffff;
                    lp_dest[3]=((lp_src[0]&LUI2S_MASKD1)>>8) |
                            ((lp_src[0]&LUI2S_MASKD2)<<8);
                    lp_dest+=4;
                    lp_src++;
#ifdef QAK
            case 15:
                    if(lp_src[0] & LUI2S_MASKE)     /* Can't forget to extend sign */
                        lp_dest[0] = 0xffffffffffffffff;
                    lp_dest[0]=((lp_src[0]&LUI2S_MASKA1)>>56) |
                            ((lp_src[0]&LUI2S_MASKA2)>>40);
                    if(lp_src[0] & LUI2S_MASKF)     /* Can't forget to extend sign */
                        lp_dest[1] = 0xffffffffffffffff;
                    lp_dest[1]=((lp_src[0]&LUI2S_MASKB1)>>40) |
                            ((lp_src[0]&LUI2S_MASKB2)>>24);
                    if(lp_src[0] & LUI2S_MASKG)     /* Can't forget to extend sign */
                        lp_dest[2] = 0xffffffffffffffff;
                    lp_dest[2]=((lp_src[0]&LUI2S_MASKC1)>>24) |
                            ((lp_src[0]&LUI2S_MASKC2)>>8);
                    if(lp_src[0] & LUI2S_MASKH)     /* Can't forget to extend sign */
                        lp_dest[3] = 0xffffffffffffffff;
                    lp_dest[3]=((lp_src[0]&LUI2S_MASKD1)>>8) |
                            ((lp_src[0]&LUI2S_MASKD2)<<8);
                    lp_dest+=4;
                    lp_src++;
            case 14:
                    if(lp_src[0] & LUI2S_MASKE)     /* Can't forget to extend sign */
                        lp_dest[0] = 0xffffffffffffffff;
                    lp_dest[0]=((lp_src[0]&LUI2S_MASKA1)>>56) |
                            ((lp_src[0]&LUI2S_MASKA2)>>40);
                    if(lp_src[0] & LUI2S_MASKF)     /* Can't forget to extend sign */
                        lp_dest[1] = 0xffffffffffffffff;
                    lp_dest[1]=((lp_src[0]&LUI2S_MASKB1)>>40) |
                            ((lp_src[0]&LUI2S_MASKB2)>>24);
                    if(lp_src[0] & LUI2S_MASKG)     /* Can't forget to extend sign */
                        lp_dest[2] = 0xffffffffffffffff;
                    lp_dest[2]=((lp_src[0]&LUI2S_MASKC1)>>24) |
                            ((lp_src[0]&LUI2S_MASKC2)>>8);
                    if(lp_src[0] & LUI2S_MASKH)     /* Can't forget to extend sign */
                        lp_dest[3] = 0xffffffffffffffff;
                    lp_dest[3]=((lp_src[0]&LUI2S_MASKD1)>>8) |
                            ((lp_src[0]&LUI2S_MASKD2)<<8);
                    lp_dest+=4;
                    lp_src++;
            case 13:
                    if(lp_src[0] & LUI2S_MASKE)     /* Can't forget to extend sign */
                        lp_dest[0] = 0xffffffffffffffff;
                    lp_dest[0]=((lp_src[0]&LUI2S_MASKA1)>>56) |
                            ((lp_src[0]&LUI2S_MASKA2)>>40);
                    if(lp_src[0] & LUI2S_MASKF)     /* Can't forget to extend sign */
                        lp_dest[1] = 0xffffffffffffffff;
                    lp_dest[1]=((lp_src[0]&LUI2S_MASKB1)>>40) |
                            ((lp_src[0]&LUI2S_MASKB2)>>24);
                    if(lp_src[0] & LUI2S_MASKG)     /* Can't forget to extend sign */
                        lp_dest[2] = 0xffffffffffffffff;
                    lp_dest[2]=((lp_src[0]&LUI2S_MASKC1)>>24) |
                            ((lp_src[0]&LUI2S_MASKC2)>>8);
                    if(lp_src[0] & LUI2S_MASKH)     /* Can't forget to extend sign */
                        lp_dest[3] = 0xffffffffffffffff;
                    lp_dest[3]=((lp_src[0]&LUI2S_MASKD1)>>8) |
                            ((lp_src[0]&LUI2S_MASKD2)<<8);
                    lp_dest+=4;
                    lp_src++;
            case 12:
                    if(lp_src[0] & LUI2S_MASKE)     /* Can't forget to extend sign */
                        lp_dest[0] = 0xffffffffffffffff;
                    lp_dest[0]=((lp_src[0]&LUI2S_MASKA1)>>56) |
                            ((lp_src[0]&LUI2S_MASKA2)>>40);
                    if(lp_src[0] & LUI2S_MASKF)     /* Can't forget to extend sign */
                        lp_dest[1] = 0xffffffffffffffff;
                    lp_dest[1]=((lp_src[0]&LUI2S_MASKB1)>>40) |
                            ((lp_src[0]&LUI2S_MASKB2)>>24);
                    if(lp_src[0] & LUI2S_MASKG)     /* Can't forget to extend sign */
                        lp_dest[2] = 0xffffffffffffffff;
                    lp_dest[2]=((lp_src[0]&LUI2S_MASKC1)>>24) |
                            ((lp_src[0]&LUI2S_MASKC2)>>8);
                    if(lp_src[0] & LUI2S_MASKH)     /* Can't forget to extend sign */
                        lp_dest[3] = 0xffffffffffffffff;
                    lp_dest[3]=((lp_src[0]&LUI2S_MASKD1)>>8) |
                            ((lp_src[0]&LUI2S_MASKD2)<<8);
                    lp_dest+=4;
                    lp_src++;
            case 11:
                    if(lp_src[0] & LUI2S_MASKE)     /* Can't forget to extend sign */
                        lp_dest[0] = 0xffffffffffffffff;
                    lp_dest[0]=((lp_src[0]&LUI2S_MASKA1)>>56) |
                            ((lp_src[0]&LUI2S_MASKA2)>>40);
                    if(lp_src[0] & LUI2S_MASKF)     /* Can't forget to extend sign */
                        lp_dest[1] = 0xffffffffffffffff;
                    lp_dest[1]=((lp_src[0]&LUI2S_MASKB1)>>40) |
                            ((lp_src[0]&LUI2S_MASKB2)>>24);
                    if(lp_src[0] & LUI2S_MASKG)     /* Can't forget to extend sign */
                        lp_dest[2] = 0xffffffffffffffff;
                    lp_dest[2]=((lp_src[0]&LUI2S_MASKC1)>>24) |
                            ((lp_src[0]&LUI2S_MASKC2)>>8);
                    if(lp_src[0] & LUI2S_MASKH)     /* Can't forget to extend sign */
                        lp_dest[3] = 0xffffffffffffffff;
                    lp_dest[3]=((lp_src[0]&LUI2S_MASKD1)>>8) |
                            ((lp_src[0]&LUI2S_MASKD2)<<8);
                    lp_dest+=4;
                    lp_src++;
            case 10:
                    if(lp_src[0] & LUI2S_MASKE)     /* Can't forget to extend sign */
                        lp_dest[0] = 0xffffffffffffffff;
                    lp_dest[0]=((lp_src[0]&LUI2S_MASKA1)>>56) |
                            ((lp_src[0]&LUI2S_MASKA2)>>40);
                    if(lp_src[0] & LUI2S_MASKF)     /* Can't forget to extend sign */
                        lp_dest[1] = 0xffffffffffffffff;
                    lp_dest[1]=((lp_src[0]&LUI2S_MASKB1)>>40) |
                            ((lp_src[0]&LUI2S_MASKB2)>>24);
                    if(lp_src[0] & LUI2S_MASKG)     /* Can't forget to extend sign */
                        lp_dest[2] = 0xffffffffffffffff;
                    lp_dest[2]=((lp_src[0]&LUI2S_MASKC1)>>24) |
                            ((lp_src[0]&LUI2S_MASKC2)>>8);
                    if(lp_src[0] & LUI2S_MASKH)     /* Can't forget to extend sign */
                        lp_dest[3] = 0xffffffffffffffff;
                    lp_dest[3]=((lp_src[0]&LUI2S_MASKD1)>>8) |
                            ((lp_src[0]&LUI2S_MASKD2)<<8);
                    lp_dest+=4;
                    lp_src++;
            case 9:
                    if(lp_src[0] & LUI2S_MASKE)     /* Can't forget to extend sign */
                        lp_dest[0] = 0xffffffffffffffff;
                    lp_dest[0]=((lp_src[0]&LUI2S_MASKA1)>>56) |
                            ((lp_src[0]&LUI2S_MASKA2)>>40);
                    if(lp_src[0] & LUI2S_MASKF)     /* Can't forget to extend sign */
                        lp_dest[1] = 0xffffffffffffffff;
                    lp_dest[1]=((lp_src[0]&LUI2S_MASKB1)>>40) |
                            ((lp_src[0]&LUI2S_MASKB2)>>24);
                    if(lp_src[0] & LUI2S_MASKG)     /* Can't forget to extend sign */
                        lp_dest[2] = 0xffffffffffffffff;
                    lp_dest[2]=((lp_src[0]&LUI2S_MASKC1)>>24) |
                            ((lp_src[0]&LUI2S_MASKC2)>>8);
                    if(lp_src[0] & LUI2S_MASKH)     /* Can't forget to extend sign */
                        lp_dest[3] = 0xffffffffffffffff;
                    lp_dest[3]=((lp_src[0]&LUI2S_MASKD1)>>8) |
                            ((lp_src[0]&LUI2S_MASKD2)<<8);
                    lp_dest+=4;
                    lp_src++;
            case 8:
                    if(lp_src[0] & LUI2S_MASKE)     /* Can't forget to extend sign */
                        lp_dest[0] = 0xffffffffffffffff;
                    lp_dest[0]=((lp_src[0]&LUI2S_MASKA1)>>56) |
                            ((lp_src[0]&LUI2S_MASKA2)>>40);
                    if(lp_src[0] & LUI2S_MASKF)     /* Can't forget to extend sign */
                        lp_dest[1] = 0xffffffffffffffff;
                    lp_dest[1]=((lp_src[0]&LUI2S_MASKB1)>>40) |
                            ((lp_src[0]&LUI2S_MASKB2)>>24);
                    if(lp_src[0] & LUI2S_MASKG)     /* Can't forget to extend sign */
                        lp_dest[2] = 0xffffffffffffffff;
                    lp_dest[2]=((lp_src[0]&LUI2S_MASKC1)>>24) |
                            ((lp_src[0]&LUI2S_MASKC2)>>8);
                    if(lp_src[0] & LUI2S_MASKH)     /* Can't forget to extend sign */
                        lp_dest[3] = 0xffffffffffffffff;
                    lp_dest[3]=((lp_src[0]&LUI2S_MASKD1)>>8) |
                            ((lp_src[0]&LUI2S_MASKD2)<<8);
                    lp_dest+=4;
                    lp_src++;
#endif
            case 7:
                    if(lp_src[0] & LUI2S_MASKE)     /* Can't forget to extend sign */
                        lp_dest[0] = 0xffffffffffffffff;
                    lp_dest[0]=((lp_src[0]&LUI2S_MASKA1)>>56) |
                            ((lp_src[0]&LUI2S_MASKA2)>>40);
                    if(lp_src[0] & LUI2S_MASKF)     /* Can't forget to extend sign */
                        lp_dest[1] = 0xffffffffffffffff;
                    lp_dest[1]=((lp_src[0]&LUI2S_MASKB1)>>40) |
                            ((lp_src[0]&LUI2S_MASKB2)>>24);
                    if(lp_src[0] & LUI2S_MASKG)     /* Can't forget to extend sign */
                        lp_dest[2] = 0xffffffffffffffff;
                    lp_dest[2]=((lp_src[0]&LUI2S_MASKC1)>>24) |
                            ((lp_src[0]&LUI2S_MASKC2)>>8);
                    if(lp_src[0] & LUI2S_MASKH)     /* Can't forget to extend sign */
                        lp_dest[3] = 0xffffffffffffffff;
                    lp_dest[3]=((lp_src[0]&LUI2S_MASKD1)>>8) |
                            ((lp_src[0]&LUI2S_MASKD2)<<8);
                    lp_dest+=4;
                    lp_src++;
            case 6:
                    if(lp_src[0] & LUI2S_MASKE)     /* Can't forget to extend sign */
                        lp_dest[0] = 0xffffffffffffffff;
                    lp_dest[0]=((lp_src[0]&LUI2S_MASKA1)>>56) |
                            ((lp_src[0]&LUI2S_MASKA2)>>40);
                    if(lp_src[0] & LUI2S_MASKF)     /* Can't forget to extend sign */
                        lp_dest[1] = 0xffffffffffffffff;
                    lp_dest[1]=((lp_src[0]&LUI2S_MASKB1)>>40) |
                            ((lp_src[0]&LUI2S_MASKB2)>>24);
                    if(lp_src[0] & LUI2S_MASKG)     /* Can't forget to extend sign */
                        lp_dest[2] = 0xffffffffffffffff;
                    lp_dest[2]=((lp_src[0]&LUI2S_MASKC1)>>24) |
                            ((lp_src[0]&LUI2S_MASKC2)>>8);
                    if(lp_src[0] & LUI2S_MASKH)     /* Can't forget to extend sign */
                        lp_dest[3] = 0xffffffffffffffff;
                    lp_dest[3]=((lp_src[0]&LUI2S_MASKD1)>>8) |
                            ((lp_src[0]&LUI2S_MASKD2)<<8);
                    lp_dest+=4;
                    lp_src++;
            case 5:
                    if(lp_src[0] & LUI2S_MASKE)     /* Can't forget to extend sign */
                        lp_dest[0] = 0xffffffffffffffff;
                    lp_dest[0]=((lp_src[0]&LUI2S_MASKA1)>>56) |
                            ((lp_src[0]&LUI2S_MASKA2)>>40);
                    if(lp_src[0] & LUI2S_MASKF)     /* Can't forget to extend sign */
                        lp_dest[1] = 0xffffffffffffffff;
                    lp_dest[1]=((lp_src[0]&LUI2S_MASKB1)>>40) |
                            ((lp_src[0]&LUI2S_MASKB2)>>24);
                    if(lp_src[0] & LUI2S_MASKG)     /* Can't forget to extend sign */
                        lp_dest[2] = 0xffffffffffffffff;
                    lp_dest[2]=((lp_src[0]&LUI2S_MASKC1)>>24) |
                            ((lp_src[0]&LUI2S_MASKC2)>>8);
                    if(lp_src[0] & LUI2S_MASKH)     /* Can't forget to extend sign */
                        lp_dest[3] = 0xffffffffffffffff;
                    lp_dest[3]=((lp_src[0]&LUI2S_MASKD1)>>8) |
                            ((lp_src[0]&LUI2S_MASKD2)<<8);
                    lp_dest+=4;
                    lp_src++;
            case 4:
                    if(lp_src[0] & LUI2S_MASKE)     /* Can't forget to extend sign */
                        lp_dest[0] = 0xffffffffffffffff;
                    lp_dest[0]=((lp_src[0]&LUI2S_MASKA1)>>56) |
                            ((lp_src[0]&LUI2S_MASKA2)>>40);
                    if(lp_src[0] & LUI2S_MASKF)     /* Can't forget to extend sign */
                        lp_dest[1] = 0xffffffffffffffff;
                    lp_dest[1]=((lp_src[0]&LUI2S_MASKB1)>>40) |
                            ((lp_src[0]&LUI2S_MASKB2)>>24);
                    if(lp_src[0] & LUI2S_MASKG)     /* Can't forget to extend sign */
                        lp_dest[2] = 0xffffffffffffffff;
                    lp_dest[2]=((lp_src[0]&LUI2S_MASKC1)>>24) |
                            ((lp_src[0]&LUI2S_MASKC2)>>8);
                    if(lp_src[0] & LUI2S_MASKH)     /* Can't forget to extend sign */
                        lp_dest[3] = 0xffffffffffffffff;
                    lp_dest[3]=((lp_src[0]&LUI2S_MASKD1)>>8) |
                            ((lp_src[0]&LUI2S_MASKD2)<<8);
                    lp_dest+=4;
                    lp_src++;
            case 3:
                    if(lp_src[0] & LUI2S_MASKE)     /* Can't forget to extend sign */
                        lp_dest[0] = 0xffffffffffffffff;
                    lp_dest[0]=((lp_src[0]&LUI2S_MASKA1)>>56) |
                            ((lp_src[0]&LUI2S_MASKA2)>>40);
                    if(lp_src[0] & LUI2S_MASKF)     /* Can't forget to extend sign */
                        lp_dest[1] = 0xffffffffffffffff;
                    lp_dest[1]=((lp_src[0]&LUI2S_MASKB1)>>40) |
                            ((lp_src[0]&LUI2S_MASKB2)>>24);
                    if(lp_src[0] & LUI2S_MASKG)     /* Can't forget to extend sign */
                        lp_dest[2] = 0xffffffffffffffff;
                    lp_dest[2]=((lp_src[0]&LUI2S_MASKC1)>>24) |
                            ((lp_src[0]&LUI2S_MASKC2)>>8);
                    if(lp_src[0] & LUI2S_MASKH)     /* Can't forget to extend sign */
                        lp_dest[3] = 0xffffffffffffffff;
                    lp_dest[3]=((lp_src[0]&LUI2S_MASKD1)>>8) |
                            ((lp_src[0]&LUI2S_MASKD2)<<8);
                    lp_dest+=4;
                    lp_src++;
            case 2:
                    if(lp_src[0] & LUI2S_MASKE)     /* Can't forget to extend sign */
                        lp_dest[0] = 0xffffffffffffffff;
                    lp_dest[0]=((lp_src[0]&LUI2S_MASKA1)>>56) |
                            ((lp_src[0]&LUI2S_MASKA2)>>40);
                    if(lp_src[0] & LUI2S_MASKF)     /* Can't forget to extend sign */
                        lp_dest[1] = 0xffffffffffffffff;
                    lp_dest[1]=((lp_src[0]&LUI2S_MASKB1)>>40) |
                            ((lp_src[0]&LUI2S_MASKB2)>>24);
                    if(lp_src[0] & LUI2S_MASKG)     /* Can't forget to extend sign */
                        lp_dest[2] = 0xffffffffffffffff;
                    lp_dest[2]=((lp_src[0]&LUI2S_MASKC1)>>24) |
                            ((lp_src[0]&LUI2S_MASKC2)>>8);
                    if(lp_src[0] & LUI2S_MASKH)     /* Can't forget to extend sign */
                        lp_dest[3] = 0xffffffffffffffff;
                    lp_dest[3]=((lp_src[0]&LUI2S_MASKD1)>>8) |
                            ((lp_src[0]&LUI2S_MASKD2)<<8);
                    lp_dest+=4;
                    lp_src++;
            case 1:
                    if(lp_src[0] & LUI2S_MASKE)     /* Can't forget to extend sign */
                        lp_dest[0] = 0xffffffffffffffff;
                    lp_dest[0]=((lp_src[0]&LUI2S_MASKA1)>>56) |
                            ((lp_src[0]&LUI2S_MASKA2)>>40);
                    if(lp_src[0] & LUI2S_MASKF)     /* Can't forget to extend sign */
                        lp_dest[1] = 0xffffffffffffffff;
                    lp_dest[1]=((lp_src[0]&LUI2S_MASKB1)>>40) |
                            ((lp_src[0]&LUI2S_MASKB2)>>24);
                    if(lp_src[0] & LUI2S_MASKG)     /* Can't forget to extend sign */
                        lp_dest[2] = 0xffffffffffffffff;
                    lp_dest[2]=((lp_src[0]&LUI2S_MASKC1)>>24) |
                            ((lp_src[0]&LUI2S_MASKC2)>>8);
                    if(lp_src[0] & LUI2S_MASKH)     /* Can't forget to extend sign */
                        lp_dest[3] = 0xffffffffffffffff;
                    lp_dest[3]=((lp_src[0]&LUI2S_MASKD1)>>8) |
                            ((lp_src[0]&LUI2S_MASKD2)<<8);
                    lp_dest+=4;
                    lp_src++;
                } while(--n>0);
		}
        switch(odd_man_out) {
            case 3:
                if(lp_src[0] & LUI2S_MASKE)     /* Can't forget to extend sign */
                    lp_dest[0] = 0xffffffffffffffff;
                lp_dest[0]=((lp_src[0]&LUI2S_MASKA1)>>56) |
                        ((lp_src[0]&LUI2S_MASKA2)>>40);
                if(lp_src[0] & LUI2S_MASKF)     /* Can't forget to extend sign */
                    lp_dest[1] = 0xffffffffffffffff;
                lp_dest[1]=((lp_src[0]&LUI2S_MASKB1)>>40) |
                        ((lp_src[0]&LUI2S_MASKB2)>>24);
                if(lp_src[0] & LUI2S_MASKG)     /* Can't forget to extend sign */
                    lp_dest[2] = 0xffffffffffffffff;
                lp_dest[2]=((lp_src[0]&LUI2S_MASKC1)>>24) |
                        ((lp_src[0]&LUI2S_MASKC2)>>8);
                break;

            case 2:
                if(lp_src[0] & LUI2S_MASKE)     /* Can't forget to extend sign */
                    lp_dest[0] = 0xffffffffffffffff;
                lp_dest[0]=((lp_src[0]&LUI2S_MASKA1)>>56) |
                        ((lp_src[0]&LUI2S_MASKA2)>>40);
                if(lp_src[0] & LUI2S_MASKF)     /* Can't forget to extend sign */
                    lp_dest[1] = 0xffffffffffffffff;
                lp_dest[1]=((lp_src[0]&LUI2S_MASKB1)>>40) |
                        ((lp_src[0]&LUI2S_MASKB2)>>24);
                break;

            case 1:
                if(lp_src[0] & LUI2S_MASKE)     /* Can't forget to extend sign */
                    lp_dest[0] = 0xffffffffffffffff;
                lp_dest[0]=((lp_src[0]&LUI2S_MASKA1)>>56) |
                        ((lp_src[0]&LUI2S_MASKA2)>>40);
                break;

            default:
                break;
          } /* end switch */
#endif  /* DUFF_lui2s */
  }
  else { /* Generic stride processing */
    for(i = 0; i < num_elm; i++) {
      if((source[1] & 0x80)) {          /* Can't forget to extend sign */
	dest[0] = 0xff;
	dest[1] = 0xff;
	dest[2] = 0xff;
	dest[3] = 0xff;
	dest[4] = 0xff;
	dest[5] = 0xff;
      }
      else {
	dest[0] = 0x00;
	dest[1] = 0x00;
	dest[2] = 0x00;
	dest[3] = 0x00;
	dest[4] = 0x00;
	dest[5] = 0x00;
      }
      dest[6] = source[1];
      dest[7] = source[0];
      source += source_stride;
      dest += dest_stride;
    }
  }
  return 0;
}

#define LUO2I_MASKA 0x00000000000000ff
#define LUO2I_MASKB 0x000000000000ff00

/************************************************************/
/* DFKluo2i()                                               */
/* -->Unicos routine for exporting 2 byte little-endian     */
/*      data items                                          */
/************************************************************/
#ifdef PROTOTYPE
PRIVATE int DFKluo2i(VOIDP s, VOIDP d, uint32 num_elm, uint32 source_stride,
		   uint32 dest_stride)
#else
PRIVATE int DFKluo2i(source, dest, num_elm, source_stride, dest_stride)
uint8 * source, * dest;
uint32 num_elm, source_stride, dest_stride;
#endif /* PROTOTYPE */
{
  register uint32 i;
  int fast_processing=0;
#ifdef PROTOTYPE
  uint8 * source = (uint8*)s;
  uint8 * dest = (uint8*)d;
#endif /* PROTOTYPE */
    long *lp_dest;
    long *lp_src;
    char *FUNC="DFKuo2i";

    HEclear();

  if(source == dest || num_elm == 0) {  /* Inplace conversions  not permitted */
    HERROR(DFE_BADCONV);                /* No elements to convert is an error */
    return FAIL;
  }

  /* Find out if it is OK to use faster array processing */
  if(source_stride == 0 && dest_stride == 0)
      fast_processing = 1;            

  if(fast_processing) {
#ifndef DUFF_luo2i
#if defined TEST1_luo2i
    int odd_man_out;        /* By default there are even num_elm */
    intn n;

    odd_man_out = num_elm%4;

    n=num_elm/4;
    lp_dest=(long *)dest;
    lp_src=(long *)source;
    for(i = 0; i < n; i++) {
        *lp_dest++=((lp_src[0]&LUO2I_MASKA)<<56) |
                    ((lp_src[0]&LUO2I_MASKB)<<40) |
                    ((lp_src[1]&LUO2I_MASKA)<<40) |
                    ((lp_src[1]&LUO2I_MASKB)<<24) |
                    ((lp_src[2]&LUO2I_MASKA)<<24) |
                    ((lp_src[2]&LUO2I_MASKB)<<8) |
                    ((lp_src[3]&LUO2I_MASKA)<<8) |
                    ((lp_src[3]&LUO2I_MASKB)>>8);
        lp_src+=4;
    }
    switch(odd_man_out) {   /* clean up leftovers */
        case 3:
            *lp_dest=((lp_src[0]&LUO2I_MASKA)<<56) |
                        ((lp_src[0]&LUO2I_MASKB)<<40) |
                        ((lp_src[1]&LUO2I_MASKA)<<40) |
                        ((lp_src[1]&LUO2I_MASKB)<<24) |
                        ((lp_src[2]&LUO2I_MASKA)<<24) |
                        ((lp_src[2]&LUO2I_MASKB)<<8);
            break;

        case 2:
            *lp_dest=((lp_src[0]&LUO2I_MASKA)<<56) |
                        ((lp_src[0]&LUO2I_MASKB)<<40) |
                        ((lp_src[1]&LUO2I_MASKA)<<40) |
                        ((lp_src[1]&LUO2I_MASKB)<<24);
            break;

        case 1:
            *lp_dest=((lp_src[0]&LUO2I_MASKA)<<56) |
                        ((lp_src[0]&LUO2I_MASKB)<<40);
            break;

        case 0:
            break;
      } /* end switch */
#else
    for(i = 0; i < num_elm; i++) {
      dest[0] = source[7];
      dest[1] = source[6];
      dest += 2;
      source += 8;
    }
#endif
#else   /* DUFF_luo2i */
        uintn n;
        int odd_man_out;        /* By default there are even num_elm */

        odd_man_out = num_elm % 4;

        num_elm/=4;
        n=(num_elm+7)/8;
        lp_dest=(long *)dest;
        lp_src=(long *)source;
        switch(num_elm%8) {
            case 0:
                do{
                    *lp_dest++=((lp_src[0]&LUO2I_MASKA)<<56) |
                                ((lp_src[0]&LUO2I_MASKB)<<40) |
                                ((lp_src[1]&LUO2I_MASKA)<<40) |
                                ((lp_src[1]&LUO2I_MASKB)<<24) |
                                ((lp_src[2]&LUO2I_MASKA)<<24) |
                                ((lp_src[2]&LUO2I_MASKB)<<8) |
                                ((lp_src[3]&LUO2I_MASKA)<<8) |
                                ((lp_src[3]&LUO2I_MASKB)>>8);
                    lp_src+=4;
#ifdef QAK
            case 15:
                    *lp_dest++=((lp_src[0]&LUO2I_MASKA)<<56) |
                                ((lp_src[0]&LUO2I_MASKB)<<40) |
                                ((lp_src[1]&LUO2I_MASKA)<<40) |
                                ((lp_src[1]&LUO2I_MASKB)<<24) |
                                ((lp_src[2]&LUO2I_MASKA)<<24) |
                                ((lp_src[2]&LUO2I_MASKB)<<8) |
                                ((lp_src[3]&LUO2I_MASKA)<<8) |
                                ((lp_src[3]&LUO2I_MASKB)>>8);
                    lp_src+=4;
            case 14:
                    *lp_dest++=((lp_src[0]&LUO2I_MASKA)<<56) |
                                ((lp_src[0]&LUO2I_MASKB)<<40) |
                                ((lp_src[1]&LUO2I_MASKA)<<40) |
                                ((lp_src[1]&LUO2I_MASKB)<<24) |
                                ((lp_src[2]&LUO2I_MASKA)<<24) |
                                ((lp_src[2]&LUO2I_MASKB)<<8) |
                                ((lp_src[3]&LUO2I_MASKA)<<8) |
                                ((lp_src[3]&LUO2I_MASKB)>>8);
                    lp_src+=4;
            case 13:
                    *lp_dest++=((lp_src[0]&LUO2I_MASKA)<<56) |
                                ((lp_src[0]&LUO2I_MASKB)<<40) |
                                ((lp_src[1]&LUO2I_MASKA)<<40) |
                                ((lp_src[1]&LUO2I_MASKB)<<24) |
                                ((lp_src[2]&LUO2I_MASKA)<<24) |
                                ((lp_src[2]&LUO2I_MASKB)<<8) |
                                ((lp_src[3]&LUO2I_MASKA)<<8) |
                                ((lp_src[3]&LUO2I_MASKB)>>8);
                    lp_src+=4;
            case 12:
                    *lp_dest++=((lp_src[0]&LUO2I_MASKA)<<56) |
                                ((lp_src[0]&LUO2I_MASKB)<<40) |
                                ((lp_src[1]&LUO2I_MASKA)<<40) |
                                ((lp_src[1]&LUO2I_MASKB)<<24) |
                                ((lp_src[2]&LUO2I_MASKA)<<24) |
                                ((lp_src[2]&LUO2I_MASKB)<<8) |
                                ((lp_src[3]&LUO2I_MASKA)<<8) |
                                ((lp_src[3]&LUO2I_MASKB)>>8);
                    lp_src+=4;
            case 11:
                    *lp_dest++=((lp_src[0]&LUO2I_MASKA)<<56) |
                                ((lp_src[0]&LUO2I_MASKB)<<40) |
                                ((lp_src[1]&LUO2I_MASKA)<<40) |
                                ((lp_src[1]&LUO2I_MASKB)<<24) |
                                ((lp_src[2]&LUO2I_MASKA)<<24) |
                                ((lp_src[2]&LUO2I_MASKB)<<8) |
                                ((lp_src[3]&LUO2I_MASKA)<<8) |
                                ((lp_src[3]&LUO2I_MASKB)>>8);
                    lp_src+=4;
            case 10:
                    *lp_dest++=((lp_src[0]&LUO2I_MASKA)<<56) |
                                ((lp_src[0]&LUO2I_MASKB)<<40) |
                                ((lp_src[1]&LUO2I_MASKA)<<40) |
                                ((lp_src[1]&LUO2I_MASKB)<<24) |
                                ((lp_src[2]&LUO2I_MASKA)<<24) |
                                ((lp_src[2]&LUO2I_MASKB)<<8) |
                                ((lp_src[3]&LUO2I_MASKA)<<8) |
                                ((lp_src[3]&LUO2I_MASKB)>>8);
                    lp_src+=4;
            case 9:
                    *lp_dest++=((lp_src[0]&LUO2I_MASKA)<<56) |
                                ((lp_src[0]&LUO2I_MASKB)<<40) |
                                ((lp_src[1]&LUO2I_MASKA)<<40) |
                                ((lp_src[1]&LUO2I_MASKB)<<24) |
                                ((lp_src[2]&LUO2I_MASKA)<<24) |
                                ((lp_src[2]&LUO2I_MASKB)<<8) |
                                ((lp_src[3]&LUO2I_MASKA)<<8) |
                                ((lp_src[3]&LUO2I_MASKB)>>8);
                    lp_src+=4;
            case 8:
                    *lp_dest++=((lp_src[0]&LUO2I_MASKA)<<56) |
                                ((lp_src[0]&LUO2I_MASKB)<<40) |
                                ((lp_src[1]&LUO2I_MASKA)<<40) |
                                ((lp_src[1]&LUO2I_MASKB)<<24) |
                                ((lp_src[2]&LUO2I_MASKA)<<24) |
                                ((lp_src[2]&LUO2I_MASKB)<<8) |
                                ((lp_src[3]&LUO2I_MASKA)<<8) |
                                ((lp_src[3]&LUO2I_MASKB)>>8);
                    lp_src+=4;
#endif
            case 7:
                    *lp_dest++=((lp_src[0]&LUO2I_MASKA)<<56) |
                                ((lp_src[0]&LUO2I_MASKB)<<40) |
                                ((lp_src[1]&LUO2I_MASKA)<<40) |
                                ((lp_src[1]&LUO2I_MASKB)<<24) |
                                ((lp_src[2]&LUO2I_MASKA)<<24) |
                                ((lp_src[2]&LUO2I_MASKB)<<8) |
                                ((lp_src[3]&LUO2I_MASKA)<<8) |
                                ((lp_src[3]&LUO2I_MASKB)>>8);
                    lp_src+=4;
            case 6:
                    *lp_dest++=((lp_src[0]&LUO2I_MASKA)<<56) |
                                ((lp_src[0]&LUO2I_MASKB)<<40) |
                                ((lp_src[1]&LUO2I_MASKA)<<40) |
                                ((lp_src[1]&LUO2I_MASKB)<<24) |
                                ((lp_src[2]&LUO2I_MASKA)<<24) |
                                ((lp_src[2]&LUO2I_MASKB)<<8) |
                                ((lp_src[3]&LUO2I_MASKA)<<8) |
                                ((lp_src[3]&LUO2I_MASKB)>>8);
                    lp_src+=4;
            case 5:
                    *lp_dest++=((lp_src[0]&LUO2I_MASKA)<<56) |
                                ((lp_src[0]&LUO2I_MASKB)<<40) |
                                ((lp_src[1]&LUO2I_MASKA)<<40) |
                                ((lp_src[1]&LUO2I_MASKB)<<24) |
                                ((lp_src[2]&LUO2I_MASKA)<<24) |
                                ((lp_src[2]&LUO2I_MASKB)<<8) |
                                ((lp_src[3]&LUO2I_MASKA)<<8) |
                                ((lp_src[3]&LUO2I_MASKB)>>8);
                    lp_src+=4;
            case 4:
                    *lp_dest++=((lp_src[0]&LUO2I_MASKA)<<56) |
                                ((lp_src[0]&LUO2I_MASKB)<<40) |
                                ((lp_src[1]&LUO2I_MASKA)<<40) |
                                ((lp_src[1]&LUO2I_MASKB)<<24) |
                                ((lp_src[2]&LUO2I_MASKA)<<24) |
                                ((lp_src[2]&LUO2I_MASKB)<<8) |
                                ((lp_src[3]&LUO2I_MASKA)<<8) |
                                ((lp_src[3]&LUO2I_MASKB)>>8);
                    lp_src+=4;
            case 3:
                    *lp_dest++=((lp_src[0]&LUO2I_MASKA)<<56) |
                                ((lp_src[0]&LUO2I_MASKB)<<40) |
                                ((lp_src[1]&LUO2I_MASKA)<<40) |
                                ((lp_src[1]&LUO2I_MASKB)<<24) |
                                ((lp_src[2]&LUO2I_MASKA)<<24) |
                                ((lp_src[2]&LUO2I_MASKB)<<8) |
                                ((lp_src[3]&LUO2I_MASKA)<<8) |
                                ((lp_src[3]&LUO2I_MASKB)>>8);
                    lp_src+=4;
            case 2:
                    *lp_dest++=((lp_src[0]&LUO2I_MASKA)<<56) |
                                ((lp_src[0]&LUO2I_MASKB)<<40) |
                                ((lp_src[1]&LUO2I_MASKA)<<40) |
                                ((lp_src[1]&LUO2I_MASKB)<<24) |
                                ((lp_src[2]&LUO2I_MASKA)<<24) |
                                ((lp_src[2]&LUO2I_MASKB)<<8) |
                                ((lp_src[3]&LUO2I_MASKA)<<8) |
                                ((lp_src[3]&LUO2I_MASKB)>>8);
                    lp_src+=4;
            case 1:
                    *lp_dest++=((lp_src[0]&LUO2I_MASKA)<<56) |
                                ((lp_src[0]&LUO2I_MASKB)<<40) |
                                ((lp_src[1]&LUO2I_MASKA)<<40) |
                                ((lp_src[1]&LUO2I_MASKB)<<24) |
                                ((lp_src[2]&LUO2I_MASKA)<<24) |
                                ((lp_src[2]&LUO2I_MASKB)<<8) |
                                ((lp_src[3]&LUO2I_MASKA)<<8) |
                                ((lp_src[3]&LUO2I_MASKB)>>8);
                    lp_src+=4;
                } while(--n>0);
		}
        switch(odd_man_out) {   /* clean up leftovers */
            case 3:
                *lp_dest=((lp_src[0]&LUO2I_MASKA)<<56) |
                            ((lp_src[0]&LUO2I_MASKB)<<40) |
                            ((lp_src[1]&LUO2I_MASKA)<<40) |
                            ((lp_src[1]&LUO2I_MASKB)<<24) |
                            ((lp_src[2]&LUO2I_MASKA)<<24) |
                            ((lp_src[2]&LUO2I_MASKB)<<8);
                break;

            case 2:
                *lp_dest=((lp_src[0]&LUO2I_MASKA)<<56) |
                            ((lp_src[0]&LUO2I_MASKB)<<40) |
                            ((lp_src[1]&LUO2I_MASKA)<<40) |
                            ((lp_src[1]&LUO2I_MASKB)<<24);
                break;

            case 1:
                *lp_dest=((lp_src[0]&LUO2I_MASKA)<<56) |
                            ((lp_src[0]&LUO2I_MASKB)<<40);
                break;

            default:
                break;
          } /* end switch */
#endif  /* DUFF_luo2i */
  }
  else { /* Generic Stride processing */
    for(i = 0; i < num_elm; i++){
      dest[0] = source[7];
      dest[1] = source[6];
      source += source_stride;
      dest += dest_stride;
    }
  }
  return 0;
}

#define LUI4I_MASKA 0xff00000000000000
#define LUI4I_MASKB 0x00ff000000000000
#define LUI4I_MASKC 0x0000ff0000000000
#define LUI4I_MASKD 0x000000ff00000000
#define LUI4I_MASKE 0x00000000ff000000
#define LUI4I_MASKF 0x0000000000ff0000
#define LUI4I_MASKG 0x000000000000ff00
#define LUI4I_MASKH 0x00000000000000ff

/************************************************************/
/* DFKlui4i()                                               */
/* -->Unicos routine for importing 4 byte little-endian     */
/*      unsigned ints                                       */
/************************************************************/
#ifdef PROTOTYPE
PRIVATE int DFKlui4i(VOIDP s, VOIDP d, uint32 num_elm, uint32 source_stride,
		   uint32 dest_stride)
#else
PRIVATE int DFKlui4i(source, dest, num_elm, source_stride, dest_stride)
uint8 * source, * dest;
uint32 num_elm, source_stride, dest_stride;
#endif /* PROTOTYPE */
{
  int fast_processing=0;
  register uint32 i;
#ifdef PROTOTYPE
  uint8 * source = (uint8*)s;
  uint8 * dest = (uint8*)d;
#endif /* PROTOTYPE */
  long * lptr_dest = (long*)dest;
  long *lp_dest;
  long *lp_src;
  char *FUNC="DFKui4i";

  HEclear();

  if(source == dest || num_elm == 0) {  /* Inplace conversions  not permitted */
    HERROR(DFE_BADCONV);                /* No elements to convert is an error */
    return FAIL;
  }

  if(source_stride == 0 && dest_stride == 0) {
    fast_processing = 1;
  }
  
    if(fast_processing) {
#ifndef DUFF_lui4i
#if defined TEST2_lui4i
        int odd_man_out;        /* By default there are even num_elm */
        intn n;

        odd_man_out = num_elm % 2;

        n=num_elm/2;
        lp_dest=(long *)dest;
        lp_src=(long *)source;
        HDmemset(lp_dest,0,num_elm*sizeof(long)); /* initialize to zeros */
        for(i = 0; i < n; i++) {
            lp_dest[0]=(lp_src[0]&LUI4I_MASKA)>>56 |
                        (lp_src[0]&LUI4I_MASKB)>>40 |
                        (lp_src[0]&LUI4I_MASKC)>>24 |
                        (lp_src[0]&LUI4I_MASKD)>>8;
            lp_dest[1]=(lp_src[0]&LUI4I_MASKE)>>24 |
                        (lp_src[0]&LUI4I_MASKF)>>8 |
                        (lp_src[0]&LUI4I_MASKG)<<8 |
                        (lp_src[0]&LUI4I_MASKH)<<24;
            lp_dest+=2;
            lp_src++;
          } /* end for */
        if(odd_man_out)
            *lp_dest=(lp_src[0]&LUI4I_MASKA)>>56 |
                        (lp_src[0]&LUI4I_MASKB)>>40 |
                        (lp_src[0]&LUI4I_MASKC)>>24 |
                        (lp_src[0]&LUI4I_MASKD)>>8;
#elif defined TEST1_lui4i
        int odd_man_out;        /* By default there are even num_elm */
        intn n;

        odd_man_out = num_elm % 2;

        n=num_elm/2;
        lp_dest=(long *)dest;
        lp_src=(long *)source;
        HDmemset(lp_dest,0,num_elm*sizeof(long)); /* initialize to zeros */
        for(i = 0; i < n; i++) {
            *lp_dest++=(lp_src[0]&LUI4I_MASKA)>>56 |
                        (lp_src[0]&LUI4I_MASKB)>>40 |
                        (lp_src[0]&LUI4I_MASKC)>>24 |
                        (lp_src[0]&LUI4I_MASKD)>>8;
            *lp_dest++=(lp_src[0]&LUI4I_MASKE)>>24 |
                        (lp_src[0]&LUI4I_MASKF)>>8 |
                        (lp_src[0]&LUI4I_MASKG)<<8 |
                        (lp_src[0]&LUI4I_MASKH)<<24;
            lp_src++;
          } /* end for */
        if(odd_man_out)
            *lp_dest++=(lp_src[0]&LUI4I_MASKA)>>56 |
                        (lp_src[0]&LUI4I_MASKB)>>40 |
                        (lp_src[0]&LUI4I_MASKC)>>24 |
                        (lp_src[0]&LUI4I_MASKD)>>8;
#else
    for(i = 0; i < num_elm; i++) {
      lptr_dest[0] = 0;
      dest[4] = source[3];
      dest[5] = source[2];
      dest[6] = source[1];
      dest[7] = source[0];
      source += 4;
      lptr_dest ++;
      dest = (uint8 *)lptr_dest;
    }
#endif
#else   /* DUFF_lui4i */
        uintn n;
        int odd_man_out;        /* By default there are even num_elm */

        lp_dest=(long *)dest;
        lp_src=(long *)source;
        HDmemset(lp_dest,0,num_elm*sizeof(long)); /* initialize to zeros */

        odd_man_out = num_elm % 2;

        num_elm/=2;
        n=(num_elm+7)/8;
        switch(num_elm%8) {
            case 0:
                do{
                    lp_dest[0]=(lp_src[0]&LUI4I_MASKA)>>56 |
                                (lp_src[0]&LUI4I_MASKB)>>40 |
                                (lp_src[0]&LUI4I_MASKC)>>24 |
                                (lp_src[0]&LUI4I_MASKD)>>8;
                    lp_dest[1]=(lp_src[0]&LUI4I_MASKE)>>24 |
                                (lp_src[0]&LUI4I_MASKF)>>8 |
                                (lp_src[0]&LUI4I_MASKG)<<8 |
                                (lp_src[0]&LUI4I_MASKH)<<24;
                    lp_dest+=2;
                    lp_src++;
#ifdef QAK
            case 15:
                    lp_dest[0]=(lp_src[0]&LUI4I_MASKA)>>56 |
                                (lp_src[0]&LUI4I_MASKB)>>40 |
                                (lp_src[0]&LUI4I_MASKC)>>24 |
                                (lp_src[0]&LUI4I_MASKD)>>8;
                    lp_dest[1]=(lp_src[0]&LUI4I_MASKE)>>24 |
                                (lp_src[0]&LUI4I_MASKF)>>8 |
                                (lp_src[0]&LUI4I_MASKG)<<8 |
                                (lp_src[0]&LUI4I_MASKH)<<24;
                    lp_dest+=2;
                    lp_src++;
            case 14:
                    lp_dest[0]=(lp_src[0]&LUI4I_MASKA)>>56 |
                                (lp_src[0]&LUI4I_MASKB)>>40 |
                                (lp_src[0]&LUI4I_MASKC)>>24 |
                                (lp_src[0]&LUI4I_MASKD)>>8;
                    lp_dest[1]=(lp_src[0]&LUI4I_MASKE)>>24 |
                                (lp_src[0]&LUI4I_MASKF)>>8 |
                                (lp_src[0]&LUI4I_MASKG)<<8 |
                                (lp_src[0]&LUI4I_MASKH)<<24;
                    lp_dest+=2;
                    lp_src++;
            case 13:
                    lp_dest[0]=(lp_src[0]&LUI4I_MASKA)>>56 |
                                (lp_src[0]&LUI4I_MASKB)>>40 |
                                (lp_src[0]&LUI4I_MASKC)>>24 |
                                (lp_src[0]&LUI4I_MASKD)>>8;
                    lp_dest[1]=(lp_src[0]&LUI4I_MASKE)>>24 |
                                (lp_src[0]&LUI4I_MASKF)>>8 |
                                (lp_src[0]&LUI4I_MASKG)<<8 |
                                (lp_src[0]&LUI4I_MASKH)<<24;
                    lp_dest+=2;
                    lp_src++;
            case 12:
                    lp_dest[0]=(lp_src[0]&LUI4I_MASKA)>>56 |
                                (lp_src[0]&LUI4I_MASKB)>>40 |
                                (lp_src[0]&LUI4I_MASKC)>>24 |
                                (lp_src[0]&LUI4I_MASKD)>>8;
                    lp_dest[1]=(lp_src[0]&LUI4I_MASKE)>>24 |
                                (lp_src[0]&LUI4I_MASKF)>>8 |
                                (lp_src[0]&LUI4I_MASKG)<<8 |
                                (lp_src[0]&LUI4I_MASKH)<<24;
                    lp_dest+=2;
                    lp_src++;
            case 11:
                    lp_dest[0]=(lp_src[0]&LUI4I_MASKA)>>56 |
                                (lp_src[0]&LUI4I_MASKB)>>40 |
                                (lp_src[0]&LUI4I_MASKC)>>24 |
                                (lp_src[0]&LUI4I_MASKD)>>8;
                    lp_dest[1]=(lp_src[0]&LUI4I_MASKE)>>24 |
                                (lp_src[0]&LUI4I_MASKF)>>8 |
                                (lp_src[0]&LUI4I_MASKG)<<8 |
                                (lp_src[0]&LUI4I_MASKH)<<24;
                    lp_dest+=2;
                    lp_src++;
            case 10:
                    lp_dest[0]=(lp_src[0]&LUI4I_MASKA)>>56 |
                                (lp_src[0]&LUI4I_MASKB)>>40 |
                                (lp_src[0]&LUI4I_MASKC)>>24 |
                                (lp_src[0]&LUI4I_MASKD)>>8;
                    lp_dest[1]=(lp_src[0]&LUI4I_MASKE)>>24 |
                                (lp_src[0]&LUI4I_MASKF)>>8 |
                                (lp_src[0]&LUI4I_MASKG)<<8 |
                                (lp_src[0]&LUI4I_MASKH)<<24;
                    lp_dest+=2;
                    lp_src++;
            case 9:
                    lp_dest[0]=(lp_src[0]&LUI4I_MASKA)>>56 |
                                (lp_src[0]&LUI4I_MASKB)>>40 |
                                (lp_src[0]&LUI4I_MASKC)>>24 |
                                (lp_src[0]&LUI4I_MASKD)>>8;
                    lp_dest[1]=(lp_src[0]&LUI4I_MASKE)>>24 |
                                (lp_src[0]&LUI4I_MASKF)>>8 |
                                (lp_src[0]&LUI4I_MASKG)<<8 |
                                (lp_src[0]&LUI4I_MASKH)<<24;
                    lp_dest+=2;
                    lp_src++;
            case 8:
                    lp_dest[0]=(lp_src[0]&LUI4I_MASKA)>>56 |
                                (lp_src[0]&LUI4I_MASKB)>>40 |
                                (lp_src[0]&LUI4I_MASKC)>>24 |
                                (lp_src[0]&LUI4I_MASKD)>>8;
                    lp_dest[1]=(lp_src[0]&LUI4I_MASKE)>>24 |
                                (lp_src[0]&LUI4I_MASKF)>>8 |
                                (lp_src[0]&LUI4I_MASKG)<<8 |
                                (lp_src[0]&LUI4I_MASKH)<<24;
                    lp_dest+=2;
                    lp_src++;
#endif
            case 7:
                    lp_dest[0]=(lp_src[0]&LUI4I_MASKA)>>56 |
                                (lp_src[0]&LUI4I_MASKB)>>40 |
                                (lp_src[0]&LUI4I_MASKC)>>24 |
                                (lp_src[0]&LUI4I_MASKD)>>8;
                    lp_dest[1]=(lp_src[0]&LUI4I_MASKE)>>24 |
                                (lp_src[0]&LUI4I_MASKF)>>8 |
                                (lp_src[0]&LUI4I_MASKG)<<8 |
                                (lp_src[0]&LUI4I_MASKH)<<24;
                    lp_dest+=2;
                    lp_src++;
            case 6:
                    lp_dest[0]=(lp_src[0]&LUI4I_MASKA)>>56 |
                                (lp_src[0]&LUI4I_MASKB)>>40 |
                                (lp_src[0]&LUI4I_MASKC)>>24 |
                                (lp_src[0]&LUI4I_MASKD)>>8;
                    lp_dest[1]=(lp_src[0]&LUI4I_MASKE)>>24 |
                                (lp_src[0]&LUI4I_MASKF)>>8 |
                                (lp_src[0]&LUI4I_MASKG)<<8 |
                                (lp_src[0]&LUI4I_MASKH)<<24;
                    lp_dest+=2;
                    lp_src++;
            case 5:
                    lp_dest[0]=(lp_src[0]&LUI4I_MASKA)>>56 |
                                (lp_src[0]&LUI4I_MASKB)>>40 |
                                (lp_src[0]&LUI4I_MASKC)>>24 |
                                (lp_src[0]&LUI4I_MASKD)>>8;
                    lp_dest[1]=(lp_src[0]&LUI4I_MASKE)>>24 |
                                (lp_src[0]&LUI4I_MASKF)>>8 |
                                (lp_src[0]&LUI4I_MASKG)<<8 |
                                (lp_src[0]&LUI4I_MASKH)<<24;
                    lp_dest+=2;
                    lp_src++;
            case 4:
                    lp_dest[0]=(lp_src[0]&LUI4I_MASKA)>>56 |
                                (lp_src[0]&LUI4I_MASKB)>>40 |
                                (lp_src[0]&LUI4I_MASKC)>>24 |
                                (lp_src[0]&LUI4I_MASKD)>>8;
                    lp_dest[1]=(lp_src[0]&LUI4I_MASKE)>>24 |
                                (lp_src[0]&LUI4I_MASKF)>>8 |
                                (lp_src[0]&LUI4I_MASKG)<<8 |
                                (lp_src[0]&LUI4I_MASKH)<<24;
                    lp_dest+=2;
                    lp_src++;
            case 3:
                    lp_dest[0]=(lp_src[0]&LUI4I_MASKA)>>56 |
                                (lp_src[0]&LUI4I_MASKB)>>40 |
                                (lp_src[0]&LUI4I_MASKC)>>24 |
                                (lp_src[0]&LUI4I_MASKD)>>8;
                    lp_dest[1]=(lp_src[0]&LUI4I_MASKE)>>24 |
                                (lp_src[0]&LUI4I_MASKF)>>8 |
                                (lp_src[0]&LUI4I_MASKG)<<8 |
                                (lp_src[0]&LUI4I_MASKH)<<24;
                    lp_dest+=2;
                    lp_src++;
            case 2:
                    lp_dest[0]=(lp_src[0]&LUI4I_MASKA)>>56 |
                                (lp_src[0]&LUI4I_MASKB)>>40 |
                                (lp_src[0]&LUI4I_MASKC)>>24 |
                                (lp_src[0]&LUI4I_MASKD)>>8;
                    lp_dest[1]=(lp_src[0]&LUI4I_MASKE)>>24 |
                                (lp_src[0]&LUI4I_MASKF)>>8 |
                                (lp_src[0]&LUI4I_MASKG)<<8 |
                                (lp_src[0]&LUI4I_MASKH)<<24;
                    lp_dest+=2;
                    lp_src++;
            case 1:
                    lp_dest[0]=(lp_src[0]&LUI4I_MASKA)>>56 |
                                (lp_src[0]&LUI4I_MASKB)>>40 |
                                (lp_src[0]&LUI4I_MASKC)>>24 |
                                (lp_src[0]&LUI4I_MASKD)>>8;
                    lp_dest[1]=(lp_src[0]&LUI4I_MASKE)>>24 |
                                (lp_src[0]&LUI4I_MASKF)>>8 |
                                (lp_src[0]&LUI4I_MASKG)<<8 |
                                (lp_src[0]&LUI4I_MASKH)<<24;
                    lp_dest+=2;
                    lp_src++;
                } while(--n>0);
		}
        if(odd_man_out)
            *lp_dest=(lp_src[0]&LUI4I_MASKA)>>56 |
                        (lp_src[0]&LUI4I_MASKB)>>40 |
                        (lp_src[0]&LUI4I_MASKC)>>24 |
                        (lp_src[0]&LUI4I_MASKD)>>8;
#endif  /* DUFF_lui4i */
      } /* end if */
  else 
    for(i = 0; i < num_elm; i++) {
      dest[0] = 0;
      dest[1] = 0;
      dest[2] = 0;
      dest[3] = 0;
      dest[4] = source[3];
      dest[5] = source[2];
      dest[6] = source[1];
      dest[7] = source[0];
      dest += dest_stride;
      source += source_stride;
    }
  return 0;
}

#define LUI4S_MASKA 0xff00000000000000
#define LUI4S_MASKB 0x00ff000000000000
#define LUI4S_MASKC 0x0000ff0000000000
#define LUI4S_MASKD 0x000000ff00000000
#define LUI4S_MASKE 0x00000000ff000000
#define LUI4S_MASKF 0x0000000000ff0000
#define LUI4S_MASKG 0x000000000000ff00
#define LUI4S_MASKH 0x00000000000000ff
#define LUI4S_MASKI 0x0000008000000000
#define LUI4S_MASKJ 0x0000000000000080

/************************************************************/
/* DFKlui4s()                                                */
/* -->Unicos routine for importing 4 signed ints            */ 
/************************************************************/
#ifdef PROTOTYPE
PRIVATE int DFKlui4s(VOIDP s, VOIDP d, uint32 num_elm, uint32 source_stride,
		   uint32 dest_stride)
#else
PRIVATE int DFKlui4s(source, dest, num_elm, source_stride, dest_stride)
uint8 * source, * dest;
uint32 num_elm, source_stride, dest_stride;
#endif /* PROTOTYPE */
{
  int fast_processing=0;
  register uint32 i;
#ifdef PROTOTYPE
  uint8 * source = (uint8*)s;
  uint8 * dest = (uint8*)d;
#endif /* PROTOTYPE */
  long * lptr_dest = (long*)dest;
  long *lp_dest;
  long *lp_src;
  char *FUNC="DFKui4s";

  HEclear();

  if(source == dest || num_elm == 0) {  /* Inplace conversions  not permitted */
    HERROR(DFE_BADCONV);                /* No elements to convert is an error */
    return FAIL;
  }

  if(source_stride == 0 && dest_stride == 0) {
    fast_processing = 1;
  }
  
  if(fast_processing) {
#ifndef DUFF_lui4s
#if defined TEST2_lui4s
        int odd_man_out;        /* By default there are even num_elm */
        intn n;

        odd_man_out = num_elm % 2;

        n=num_elm/2;
        lp_dest=(long *)dest;
        lp_src=(long *)source;
        HDmemset(lp_dest,0,num_elm*sizeof(long)); /* initialize to zeros */
        for(i = 0; i < n; i++) {
            if(lp_src[0] & LUI4S_MASKI)     /* Can't forget to sign extend */
                lp_dest[0] = 0xffffffffffffffff;
            lp_dest[0]=(lp_src[0]&LUI4S_MASKA)>>56 |
                        (lp_src[0]&LUI4S_MASKB)>>40 |
                        (lp_src[0]&LUI4S_MASKC)>>24 |
                        (lp_src[0]&LUI4S_MASKD)>>8;
            if(lp_src[0] & LUI4S_MASKJ)     /* Can't forget to sign extend */
                lp_dest[1] = 0xffffffffffffffff;
            lp_dest[1]=(lp_src[0]&LUI4S_MASKE)>>24 |
                        (lp_src[0]&LUI4S_MASKF)>>8 |
                        (lp_src[0]&LUI4S_MASKG)<<8 |
                        (lp_src[0]&LUI4S_MASKH)<<24;
            lp_dest+=2;
            lp_src++;
          } /* end for */
        if(odd_man_out) {
            if(lp_src[0] & LUI4S_MASKI)     /* Can't forget to sign extend */
                lp_dest[0] = 0xffffffffffffffff;
            lp_dest[0]=(lp_src[0]&LUI4S_MASKA)>>56 |
                        (lp_src[0]&LUI4S_MASKB)>>40 |
                        (lp_src[0]&LUI4S_MASKC)>>24 |
                        (lp_src[0]&LUI4S_MASKD)>>8;
          } /* end if */
#elif defined TEST1_lui4s
        int odd_man_out;        /* By default there are even num_elm */
        intn n;

        odd_man_out = num_elm % 2;

        n=num_elm/2;
        lp_dest=(long *)dest;
        lp_src=(long *)source;
        HDmemset(lp_dest,0,num_elm*sizeof(long)); /* initialize to zeros */
        for(i = 0; i < n; i++) {
            if(lp_src[0] & LUI4S_MASKI)     /* Can't forget to sign extend */
                lp_dest[0] = 0xffffffffffffffff;
            *lp_dest++=(lp_src[0]&LUI4S_MASKA)>>56 |
                        (lp_src[0]&LUI4S_MASKB)>>40 |
                        (lp_src[0]&LUI4S_MASKC)>>24 |
                        (lp_src[0]&LUI4S_MASKD)>>8;
            if(lp_src[0] & LUI4S_MASKJ)     /* Can't forget to sign extend */
                lp_dest[0] = 0xffffffffffffffff;
            *lp_dest++=(lp_src[0]&LUI4S_MASKE)>>24 |
                        (lp_src[0]&LUI4S_MASKF)>>8 |
                        (lp_src[0]&LUI4S_MASKG)<<8 |
                        (lp_src[0]&LUI4S_MASKH)<<24;
            lp_src++;
          } /* end for */
        if(odd_man_out) {
            if(lp_src[0] & LUI4S_MASKI)     /* Can't forget to sign extend */
                lp_dest[0] = 0xffffffffffffffff;
            *lp_dest++=(lp_src[0]&LUI4S_MASKA)>>56 |
                        (lp_src[0]&LUI4S_MASKB)>>40 |
                        (lp_src[0]&LUI4S_MASKC)>>24 |
                        (lp_src[0]&LUI4S_MASKD)>>8;
          } /* end if */
#else
    for(i = 0; i < num_elm; i++) {
      if((source[3] & 0x80))            /* Can't forget to sign extend */
	lptr_dest[0] = 0xffffffffffffffff;
      else
	lptr_dest[0] = 0x0000000000000000;
      dest[4] = source[3];
      dest[5] = source[2];
      dest[6] = source[1];
      dest[7] = source[0];
      source += 4;
      lptr_dest ++;
      dest = (uint8 *)lptr_dest;
    }
#endif
#else   /* DUFF_lui4s */
        uintn n;
        int odd_man_out;        /* By default there are even num_elm */

        lp_dest=(long *)dest;
        lp_src=(long *)source;
        HDmemset(lp_dest,0,num_elm*sizeof(long)); /* initialize to zeros */

        odd_man_out = num_elm % 2;

        num_elm/=2;
        n=(num_elm+7)/8;
        switch(num_elm%8) {
            case 0:
                do{
                    if(lp_src[0] & LUI4S_MASKI)     /* Can't forget to sign extend */
                        lp_dest[0] = 0xffffffffffffffff;
                    lp_dest[0]=(lp_src[0]&LUI4S_MASKA)>>56 |
                                (lp_src[0]&LUI4S_MASKB)>>40 |
                                (lp_src[0]&LUI4S_MASKC)>>24 |
                                (lp_src[0]&LUI4S_MASKD)>>8;
                    if(lp_src[0] & LUI4S_MASKJ)     /* Can't forget to sign extend */
                        lp_dest[1] = 0xffffffffffffffff;
                    lp_dest[1]=(lp_src[0]&LUI4S_MASKE)>>24 |
                                (lp_src[0]&LUI4S_MASKF)>>8 |
                                (lp_src[0]&LUI4S_MASKG)<<8 |
                                (lp_src[0]&LUI4S_MASKH)<<24;
                    lp_dest+=2;
                    lp_src++;
#ifdef QAK
            case 15:
                    if(lp_src[0] & LUI4S_MASKI)     /* Can't forget to sign extend */
                        lp_dest[0] = 0xffffffffffffffff;
                    lp_dest[0]=(lp_src[0]&LUI4S_MASKA)>>56 |
                                (lp_src[0]&LUI4S_MASKB)>>40 |
                                (lp_src[0]&LUI4S_MASKC)>>24 |
                                (lp_src[0]&LUI4S_MASKD)>>8;
                    if(lp_src[0] & LUI4S_MASKJ)     /* Can't forget to sign extend */
                        lp_dest[1] = 0xffffffffffffffff;
                    lp_dest[1]=(lp_src[0]&LUI4S_MASKE)>>24 |
                                (lp_src[0]&LUI4S_MASKF)>>8 |
                                (lp_src[0]&LUI4S_MASKG)<<8 |
                                (lp_src[0]&LUI4S_MASKH)<<24;
                    lp_dest+=2;
                    lp_src++;
            case 14:
                    if(lp_src[0] & LUI4S_MASKI)     /* Can't forget to sign extend */
                        lp_dest[0] = 0xffffffffffffffff;
                    lp_dest[0]=(lp_src[0]&LUI4S_MASKA)>>56 |
                                (lp_src[0]&LUI4S_MASKB)>>40 |
                                (lp_src[0]&LUI4S_MASKC)>>24 |
                                (lp_src[0]&LUI4S_MASKD)>>8;
                    if(lp_src[0] & LUI4S_MASKJ)     /* Can't forget to sign extend */
                        lp_dest[1] = 0xffffffffffffffff;
                    lp_dest[1]=(lp_src[0]&LUI4S_MASKE)>>24 |
                                (lp_src[0]&LUI4S_MASKF)>>8 |
                                (lp_src[0]&LUI4S_MASKG)<<8 |
                                (lp_src[0]&LUI4S_MASKH)<<24;
                    lp_dest+=2;
                    lp_src++;
            case 13:
                    if(lp_src[0] & LUI4S_MASKI)     /* Can't forget to sign extend */
                        lp_dest[0] = 0xffffffffffffffff;
                    lp_dest[0]=(lp_src[0]&LUI4S_MASKA)>>56 |
                                (lp_src[0]&LUI4S_MASKB)>>40 |
                                (lp_src[0]&LUI4S_MASKC)>>24 |
                                (lp_src[0]&LUI4S_MASKD)>>8;
                    if(lp_src[0] & LUI4S_MASKJ)     /* Can't forget to sign extend */
                        lp_dest[1] = 0xffffffffffffffff;
                    lp_dest[1]=(lp_src[0]&LUI4S_MASKE)>>24 |
                                (lp_src[0]&LUI4S_MASKF)>>8 |
                                (lp_src[0]&LUI4S_MASKG)<<8 |
                                (lp_src[0]&LUI4S_MASKH)<<24;
                    lp_dest+=2;
                    lp_src++;
            case 12:
                    if(lp_src[0] & LUI4S_MASKI)     /* Can't forget to sign extend */
                        lp_dest[0] = 0xffffffffffffffff;
                    lp_dest[0]=(lp_src[0]&LUI4S_MASKA)>>56 |
                                (lp_src[0]&LUI4S_MASKB)>>40 |
                                (lp_src[0]&LUI4S_MASKC)>>24 |
                                (lp_src[0]&LUI4S_MASKD)>>8;
                    if(lp_src[0] & LUI4S_MASKJ)     /* Can't forget to sign extend */
                        lp_dest[1] = 0xffffffffffffffff;
                    lp_dest[1]=(lp_src[0]&LUI4S_MASKE)>>24 |
                                (lp_src[0]&LUI4S_MASKF)>>8 |
                                (lp_src[0]&LUI4S_MASKG)<<8 |
                                (lp_src[0]&LUI4S_MASKH)<<24;
                    lp_dest+=2;
                    lp_src++;
            case 11:
                    if(lp_src[0] & LUI4S_MASKI)     /* Can't forget to sign extend */
                        lp_dest[0] = 0xffffffffffffffff;
                    lp_dest[0]=(lp_src[0]&LUI4S_MASKA)>>56 |
                                (lp_src[0]&LUI4S_MASKB)>>40 |
                                (lp_src[0]&LUI4S_MASKC)>>24 |
                                (lp_src[0]&LUI4S_MASKD)>>8;
                    if(lp_src[0] & LUI4S_MASKJ)     /* Can't forget to sign extend */
                        lp_dest[1] = 0xffffffffffffffff;
                    lp_dest[1]=(lp_src[0]&LUI4S_MASKE)>>24 |
                                (lp_src[0]&LUI4S_MASKF)>>8 |
                                (lp_src[0]&LUI4S_MASKG)<<8 |
                                (lp_src[0]&LUI4S_MASKH)<<24;
                    lp_dest+=2;
                    lp_src++;
            case 10:
                    if(lp_src[0] & LUI4S_MASKI)     /* Can't forget to sign extend */
                        lp_dest[0] = 0xffffffffffffffff;
                    lp_dest[0]=(lp_src[0]&LUI4S_MASKA)>>56 |
                                (lp_src[0]&LUI4S_MASKB)>>40 |
                                (lp_src[0]&LUI4S_MASKC)>>24 |
                                (lp_src[0]&LUI4S_MASKD)>>8;
                    if(lp_src[0] & LUI4S_MASKJ)     /* Can't forget to sign extend */
                        lp_dest[1] = 0xffffffffffffffff;
                    lp_dest[1]=(lp_src[0]&LUI4S_MASKE)>>24 |
                                (lp_src[0]&LUI4S_MASKF)>>8 |
                                (lp_src[0]&LUI4S_MASKG)<<8 |
                                (lp_src[0]&LUI4S_MASKH)<<24;
                    lp_dest+=2;
                    lp_src++;
            case 9:
                    if(lp_src[0] & LUI4S_MASKI)     /* Can't forget to sign extend */
                        lp_dest[0] = 0xffffffffffffffff;
                    lp_dest[0]=(lp_src[0]&LUI4S_MASKA)>>56 |
                                (lp_src[0]&LUI4S_MASKB)>>40 |
                                (lp_src[0]&LUI4S_MASKC)>>24 |
                                (lp_src[0]&LUI4S_MASKD)>>8;
                    if(lp_src[0] & LUI4S_MASKJ)     /* Can't forget to sign extend */
                        lp_dest[1] = 0xffffffffffffffff;
                    lp_dest[1]=(lp_src[0]&LUI4S_MASKE)>>24 |
                                (lp_src[0]&LUI4S_MASKF)>>8 |
                                (lp_src[0]&LUI4S_MASKG)<<8 |
                                (lp_src[0]&LUI4S_MASKH)<<24;
                    lp_dest+=2;
                    lp_src++;
            case 8:
                    if(lp_src[0] & LUI4S_MASKI)     /* Can't forget to sign extend */
                        lp_dest[0] = 0xffffffffffffffff;
                    lp_dest[0]=(lp_src[0]&LUI4S_MASKA)>>56 |
                                (lp_src[0]&LUI4S_MASKB)>>40 |
                                (lp_src[0]&LUI4S_MASKC)>>24 |
                                (lp_src[0]&LUI4S_MASKD)>>8;
                    if(lp_src[0] & LUI4S_MASKJ)     /* Can't forget to sign extend */
                        lp_dest[1] = 0xffffffffffffffff;
                    lp_dest[1]=(lp_src[0]&LUI4S_MASKE)>>24 |
                                (lp_src[0]&LUI4S_MASKF)>>8 |
                                (lp_src[0]&LUI4S_MASKG)<<8 |
                                (lp_src[0]&LUI4S_MASKH)<<24;
                    lp_dest+=2;
                    lp_src++;
#endif
            case 7:
                    if(lp_src[0] & LUI4S_MASKI)     /* Can't forget to sign extend */
                        lp_dest[0] = 0xffffffffffffffff;
                    lp_dest[0]=(lp_src[0]&LUI4S_MASKA)>>56 |
                                (lp_src[0]&LUI4S_MASKB)>>40 |
                                (lp_src[0]&LUI4S_MASKC)>>24 |
                                (lp_src[0]&LUI4S_MASKD)>>8;
                    if(lp_src[0] & LUI4S_MASKJ)     /* Can't forget to sign extend */
                        lp_dest[1] = 0xffffffffffffffff;
                    lp_dest[1]=(lp_src[0]&LUI4S_MASKE)>>24 |
                                (lp_src[0]&LUI4S_MASKF)>>8 |
                                (lp_src[0]&LUI4S_MASKG)<<8 |
                                (lp_src[0]&LUI4S_MASKH)<<24;
                    lp_dest+=2;
                    lp_src++;
            case 6:
                    if(lp_src[0] & LUI4S_MASKI)     /* Can't forget to sign extend */
                        lp_dest[0] = 0xffffffffffffffff;
                    lp_dest[0]=(lp_src[0]&LUI4S_MASKA)>>56 |
                                (lp_src[0]&LUI4S_MASKB)>>40 |
                                (lp_src[0]&LUI4S_MASKC)>>24 |
                                (lp_src[0]&LUI4S_MASKD)>>8;
                    if(lp_src[0] & LUI4S_MASKJ)     /* Can't forget to sign extend */
                        lp_dest[1] = 0xffffffffffffffff;
                    lp_dest[1]=(lp_src[0]&LUI4S_MASKE)>>24 |
                                (lp_src[0]&LUI4S_MASKF)>>8 |
                                (lp_src[0]&LUI4S_MASKG)<<8 |
                                (lp_src[0]&LUI4S_MASKH)<<24;
                    lp_dest+=2;
                    lp_src++;
            case 5:
                    if(lp_src[0] & LUI4S_MASKI)     /* Can't forget to sign extend */
                        lp_dest[0] = 0xffffffffffffffff;
                    lp_dest[0]=(lp_src[0]&LUI4S_MASKA)>>56 |
                                (lp_src[0]&LUI4S_MASKB)>>40 |
                                (lp_src[0]&LUI4S_MASKC)>>24 |
                                (lp_src[0]&LUI4S_MASKD)>>8;
                    if(lp_src[0] & LUI4S_MASKJ)     /* Can't forget to sign extend */
                        lp_dest[1] = 0xffffffffffffffff;
                    lp_dest[1]=(lp_src[0]&LUI4S_MASKE)>>24 |
                                (lp_src[0]&LUI4S_MASKF)>>8 |
                                (lp_src[0]&LUI4S_MASKG)<<8 |
                                (lp_src[0]&LUI4S_MASKH)<<24;
                    lp_dest+=2;
                    lp_src++;
            case 4:
                    if(lp_src[0] & LUI4S_MASKI)     /* Can't forget to sign extend */
                        lp_dest[0] = 0xffffffffffffffff;
                    lp_dest[0]=(lp_src[0]&LUI4S_MASKA)>>56 |
                                (lp_src[0]&LUI4S_MASKB)>>40 |
                                (lp_src[0]&LUI4S_MASKC)>>24 |
                                (lp_src[0]&LUI4S_MASKD)>>8;
                    if(lp_src[0] & LUI4S_MASKJ)     /* Can't forget to sign extend */
                        lp_dest[1] = 0xffffffffffffffff;
                    lp_dest[1]=(lp_src[0]&LUI4S_MASKE)>>24 |
                                (lp_src[0]&LUI4S_MASKF)>>8 |
                                (lp_src[0]&LUI4S_MASKG)<<8 |
                                (lp_src[0]&LUI4S_MASKH)<<24;
                    lp_dest+=2;
                    lp_src++;
            case 3:
                    if(lp_src[0] & LUI4S_MASKI)     /* Can't forget to sign extend */
                        lp_dest[0] = 0xffffffffffffffff;
                    lp_dest[0]=(lp_src[0]&LUI4S_MASKA)>>56 |
                                (lp_src[0]&LUI4S_MASKB)>>40 |
                                (lp_src[0]&LUI4S_MASKC)>>24 |
                                (lp_src[0]&LUI4S_MASKD)>>8;
                    if(lp_src[0] & LUI4S_MASKJ)     /* Can't forget to sign extend */
                        lp_dest[1] = 0xffffffffffffffff;
                    lp_dest[1]=(lp_src[0]&LUI4S_MASKE)>>24 |
                                (lp_src[0]&LUI4S_MASKF)>>8 |
                                (lp_src[0]&LUI4S_MASKG)<<8 |
                                (lp_src[0]&LUI4S_MASKH)<<24;
                    lp_dest+=2;
                    lp_src++;
            case 2:
                    if(lp_src[0] & LUI4S_MASKI)     /* Can't forget to sign extend */
                        lp_dest[0] = 0xffffffffffffffff;
                    lp_dest[0]=(lp_src[0]&LUI4S_MASKA)>>56 |
                                (lp_src[0]&LUI4S_MASKB)>>40 |
                                (lp_src[0]&LUI4S_MASKC)>>24 |
                                (lp_src[0]&LUI4S_MASKD)>>8;
                    if(lp_src[0] & LUI4S_MASKJ)     /* Can't forget to sign extend */
                        lp_dest[1] = 0xffffffffffffffff;
                    lp_dest[1]=(lp_src[0]&LUI4S_MASKE)>>24 |
                                (lp_src[0]&LUI4S_MASKF)>>8 |
                                (lp_src[0]&LUI4S_MASKG)<<8 |
                                (lp_src[0]&LUI4S_MASKH)<<24;
                    lp_dest+=2;
                    lp_src++;
            case 1:
                    if(lp_src[0] & LUI4S_MASKI)     /* Can't forget to sign extend */
                        lp_dest[0] = 0xffffffffffffffff;
                    lp_dest[0]=(lp_src[0]&LUI4S_MASKA)>>56 |
                                (lp_src[0]&LUI4S_MASKB)>>40 |
                                (lp_src[0]&LUI4S_MASKC)>>24 |
                                (lp_src[0]&LUI4S_MASKD)>>8;
                    if(lp_src[0] & LUI4S_MASKJ)     /* Can't forget to sign extend */
                        lp_dest[1] = 0xffffffffffffffff;
                    lp_dest[1]=(lp_src[0]&LUI4S_MASKE)>>24 |
                                (lp_src[0]&LUI4S_MASKF)>>8 |
                                (lp_src[0]&LUI4S_MASKG)<<8 |
                                (lp_src[0]&LUI4S_MASKH)<<24;
                    lp_dest+=2;
                    lp_src++;
                } while(--n>0);
		}
        if(odd_man_out) {
            if(lp_src[0] & LUI4S_MASKI)     /* Can't forget to sign extend */
                lp_dest[0] = 0xffffffffffffffff;
            lp_dest[0]=(lp_src[0]&LUI4S_MASKA)>>56 |
                        (lp_src[0]&LUI4S_MASKB)>>40 |
                        (lp_src[0]&LUI4S_MASKC)>>24 |
                        (lp_src[0]&LUI4S_MASKD)>>8;
          } /* end if */
#endif  /* DUFF_lui4s */
      } /* end if */
  else 
    for(i = 0; i < num_elm; i++) {
      if((source[3] & 0x80)) {          /* Can't forget to sign extend */
	dest[0] = 0xff;
	dest[1] = 0xff;
	dest[2] = 0xff;
	dest[3] = 0xff;
      }
      else {
	dest[0] = 0;
	dest[1] = 0;
	dest[2] = 0;
	dest[3] = 0;
      }
      dest[4] = source[3];
      dest[5] = source[2];
      dest[6] = source[1];
      dest[7] = source[0];
      dest += dest_stride;
      source += source_stride;
    }
  return 0;
}

#if defined TEST2_luo4i | defined (DUFF_luo4i)
#define LUO4I_MASKA 0x00000000ff00ff00
#define LUO4I_MASKB 0x0000000000ff00ff
#define LUO4I_MASKC 0xffff0000ffff0000
#define LUO4I_MASKD 0x0000ffff0000ffff
#elif defined TEST1_luo4i
#define LUO4I_MASKA 0x00000000ff000000
#define LUO4I_MASKB 0x0000000000ff0000
#define LUO4I_MASKC 0x000000000000ff00
#define LUO4I_MASKD 0x00000000000000ff
#endif

/************************************************************/
/* DFKluo4i()                                                */
/* -->Unicos routine for exporting 4 byte data items        */ 
/************************************************************/

#ifdef PROTOTYPE
PRIVATE int DFKluo4i(VOIDP s, VOIDP d, uint32 num_elm, uint32 source_stride,
		   uint32 dest_stride)
#else
PRIVATE int DFKluo4i(source, dest, num_elm, source_stride, dest_stride)
uint8 * source, * dest;
uint32 num_elm, source_stride, dest_stride;
#endif /* PROTOTYPE */
{
  int fast_processing=0;
  register uintn i;
#ifdef PROTOTYPE
  uint8 * source = (uint8*)s;
  uint8 * dest = (uint8*)d;
#endif /* PROTOTYPE */
  long *lp_dest;
  long *lp_src;
  char *FUNC="DFKuo4i";

  HEclear();

  if(source == dest || num_elm == 0) {  /* Inplace conversions  not permitted */
    HERROR(DFE_BADCONV);                /* No elements to convert is an error */
    return FAIL;
  }

  if(source_stride == 0 && dest_stride == 0) {
    fast_processing = 1;
  }
  
  if(fast_processing) {
#ifndef DUFF_luo4i
#if defined TEST2_luo4i
    int odd_man_out = 0;        /* By default there are even num_elm */
    intn n;

    if(num_elm % 2)             /* If this is true, we have odd num */
      odd_man_out = 1;

    n=num_elm/2;
    lp_dest=(long *)dest;
    lp_src=(long *)source;
    for(i = 0; i < n; i++) {
        *lp_dest=((lp_src[0]&LUO4I_MASKA)<<24) | ((lp_src[0]&LUO4I_MASKB)<<40) |
                   ((lp_src[1]&LUO4I_MASKA)>>8) | ((lp_src[1]&LUO4I_MASKB)>>8);
        *lp_dest++=((*lp_dest&LUO4I_MASKC)>>16) | ((*lp_dest&LUO4I_MASKD)<<16);
        lp_src+=2;
    }
    if(odd_man_out) {
        *lp_dest=((lp_src[0]&LUO4I_MASKA)<<24) | ((lp_src[0]&LUO4I_MASKB)<<40);
        *lp_dest++=((*lp_dest&LUO4I_MASKC)>>16) | ((*lp_dest&LUO4I_MASKD)<<16);
      } /* end if */
#elif defined TEST1_luo4i
    int odd_man_out = 0;        /* By default there are even num_elm */
    intn n;

    if(num_elm % 2)             /* If this is true, we have odd num */
      odd_man_out = 1;

    n=num_elm/2;
    lp_dest=(long *)dest;
    lp_src=(long *)source;
    for(i = 0; i < n; i++) {
    *lp_dest++=((lp_src[0]&LUO4I_MASKA)<<8) |
                   ((lp_src[0]&LUO4I_MASKB)<<24) |
                   ((lp_src[0]&LUO4I_MASKC)<<40) |
                   ((lp_src[0]&LUO4I_MASKD)<<56) |
                   ((lp_src[1]&LUO4I_MASKA)>>24) |
                   ((lp_src[1]&LUO4I_MASKB)>>8) |
                   ((lp_src[1]&LUO4I_MASKC)<<8) |
                   ((lp_src[1]&LUO4I_MASKD)<<24);
        lp_src+=2;
    }
    if(odd_man_out) {
        dest=(uint8 *)lp_dest;
        source=(uint8 *)lp_src;
        dest[0] = source[7];
        dest[1] = source[6];
        dest[2] = source[5];
        dest[3] = source[4];
      } /* end if */
#else
    for(i = 0; i < num_elm; i++) {
      dest[0] = source[7];
      dest[1] = source[6];
      dest[2] = source[5];
      dest[3] = source[4];
      dest += 4;
      source += 8;
    }
#endif
#else   /* DUFF_luo4i */
        register uintn n;
        int odd_man_out = 0;        /* By default there are even num_elm */

        if(num_elm % 2)             /* If this is true, we have odd num */
          odd_man_out = 1;

        num_elm/=2;
        n=(num_elm+7)/8;
        lp_dest=(long *)dest;
        lp_src=(long *)source;
        switch(num_elm%8) {
            case 0:
                do{
                    *lp_dest=((lp_src[0]&LUO4I_MASKA)<<24) |
                                ((lp_src[0]&LUO4I_MASKB)<<40) |
                                ((lp_src[1]&LUO4I_MASKA)>>8) |
                                ((lp_src[1]&LUO4I_MASKB)>>8);
                    *lp_dest++=((*lp_dest&LUO4I_MASKC)>>16) |
                                ((*lp_dest&LUO4I_MASKD)<<16);
                    lp_src+=2;
#ifdef QAK
            case 15:
                    *lp_dest=((lp_src[0]&LUO4I_MASKA)<<24) |
                                ((lp_src[0]&LUO4I_MASKB)<<40) |
                                ((lp_src[1]&LUO4I_MASKA)>>8) |
                                ((lp_src[1]&LUO4I_MASKB)>>8);
                    *lp_dest++=((*lp_dest&LUO4I_MASKC)>>16) |
                                ((*lp_dest&LUO4I_MASKD)<<16);
                    lp_src+=2;
            case 14:
                    *lp_dest=((lp_src[0]&LUO4I_MASKA)<<24) |
                                ((lp_src[0]&LUO4I_MASKB)<<40) |
                                ((lp_src[1]&LUO4I_MASKA)>>8) |
                                ((lp_src[1]&LUO4I_MASKB)>>8);
                    *lp_dest++=((*lp_dest&LUO4I_MASKC)>>16) |
                                ((*lp_dest&LUO4I_MASKD)<<16);
                    lp_src+=2;
            case 13:
                    *lp_dest=((lp_src[0]&LUO4I_MASKA)<<24) |
                                ((lp_src[0]&LUO4I_MASKB)<<40) |
                                ((lp_src[1]&LUO4I_MASKA)>>8) |
                                ((lp_src[1]&LUO4I_MASKB)>>8);
                    *lp_dest++=((*lp_dest&LUO4I_MASKC)>>16) |
                                ((*lp_dest&LUO4I_MASKD)<<16);
                    lp_src+=2;
            case 12:
                    *lp_dest=((lp_src[0]&LUO4I_MASKA)<<24) |
                                ((lp_src[0]&LUO4I_MASKB)<<40) |
                                ((lp_src[1]&LUO4I_MASKA)>>8) |
                                ((lp_src[1]&LUO4I_MASKB)>>8);
                    *lp_dest++=((*lp_dest&LUO4I_MASKC)>>16) |
                                ((*lp_dest&LUO4I_MASKD)<<16);
                    lp_src+=2;
            case 11:
                    *lp_dest=((lp_src[0]&LUO4I_MASKA)<<24) |
                                ((lp_src[0]&LUO4I_MASKB)<<40) |
                                ((lp_src[1]&LUO4I_MASKA)>>8) |
                                ((lp_src[1]&LUO4I_MASKB)>>8);
                    *lp_dest++=((*lp_dest&LUO4I_MASKC)>>16) |
                                ((*lp_dest&LUO4I_MASKD)<<16);
                    lp_src+=2;
            case 10:
                    *lp_dest=((lp_src[0]&LUO4I_MASKA)<<24) |
                                ((lp_src[0]&LUO4I_MASKB)<<40) |
                                ((lp_src[1]&LUO4I_MASKA)>>8) |
                                ((lp_src[1]&LUO4I_MASKB)>>8);
                    *lp_dest++=((*lp_dest&LUO4I_MASKC)>>16) |
                                ((*lp_dest&LUO4I_MASKD)<<16);
                    lp_src+=2;
            case 9:
                    *lp_dest=((lp_src[0]&LUO4I_MASKA)<<24) |
                                ((lp_src[0]&LUO4I_MASKB)<<40) |
                                ((lp_src[1]&LUO4I_MASKA)>>8) |
                                ((lp_src[1]&LUO4I_MASKB)>>8);
                    *lp_dest++=((*lp_dest&LUO4I_MASKC)>>16) |
                                ((*lp_dest&LUO4I_MASKD)<<16);
                    lp_src+=2;
            case 8:
                    *lp_dest=((lp_src[0]&LUO4I_MASKA)<<24) |
                                ((lp_src[0]&LUO4I_MASKB)<<40) |
                                ((lp_src[1]&LUO4I_MASKA)>>8) |
                                ((lp_src[1]&LUO4I_MASKB)>>8);
                    *lp_dest++=((*lp_dest&LUO4I_MASKC)>>16) |
                                ((*lp_dest&LUO4I_MASKD)<<16);
                    lp_src+=2;
#endif
            case 7:
                    *lp_dest=((lp_src[0]&LUO4I_MASKA)<<24) |
                                ((lp_src[0]&LUO4I_MASKB)<<40) |
                                ((lp_src[1]&LUO4I_MASKA)>>8) |
                                ((lp_src[1]&LUO4I_MASKB)>>8);
                    *lp_dest++=((*lp_dest&LUO4I_MASKC)>>16) |
                                ((*lp_dest&LUO4I_MASKD)<<16);
                    lp_src+=2;
            case 6:
                    *lp_dest=((lp_src[0]&LUO4I_MASKA)<<24) |
                                ((lp_src[0]&LUO4I_MASKB)<<40) |
                                ((lp_src[1]&LUO4I_MASKA)>>8) |
                                ((lp_src[1]&LUO4I_MASKB)>>8);
                    *lp_dest++=((*lp_dest&LUO4I_MASKC)>>16) |
                                ((*lp_dest&LUO4I_MASKD)<<16);
                    lp_src+=2;
            case 5:
                    *lp_dest=((lp_src[0]&LUO4I_MASKA)<<24) |
                                ((lp_src[0]&LUO4I_MASKB)<<40) |
                                ((lp_src[1]&LUO4I_MASKA)>>8) |
                                ((lp_src[1]&LUO4I_MASKB)>>8);
                    *lp_dest++=((*lp_dest&LUO4I_MASKC)>>16) |
                                ((*lp_dest&LUO4I_MASKD)<<16);
                    lp_src+=2;
            case 4:
                    *lp_dest=((lp_src[0]&LUO4I_MASKA)<<24) |
                                ((lp_src[0]&LUO4I_MASKB)<<40) |
                                ((lp_src[1]&LUO4I_MASKA)>>8) |
                                ((lp_src[1]&LUO4I_MASKB)>>8);
                    *lp_dest++=((*lp_dest&LUO4I_MASKC)>>16) |
                                ((*lp_dest&LUO4I_MASKD)<<16);
                    lp_src+=2;
            case 3:
                    *lp_dest=((lp_src[0]&LUO4I_MASKA)<<24) |
                                ((lp_src[0]&LUO4I_MASKB)<<40) |
                                ((lp_src[1]&LUO4I_MASKA)>>8) |
                                ((lp_src[1]&LUO4I_MASKB)>>8);
                    *lp_dest++=((*lp_dest&LUO4I_MASKC)>>16) |
                                ((*lp_dest&LUO4I_MASKD)<<16);
                    lp_src+=2;
            case 2:
                    *lp_dest=((lp_src[0]&LUO4I_MASKA)<<24) |
                                ((lp_src[0]&LUO4I_MASKB)<<40) |
                                ((lp_src[1]&LUO4I_MASKA)>>8) |
                                ((lp_src[1]&LUO4I_MASKB)>>8);
                    *lp_dest++=((*lp_dest&LUO4I_MASKC)>>16) |
                                ((*lp_dest&LUO4I_MASKD)<<16);
                    lp_src+=2;
            case 1:
                    *lp_dest=((lp_src[0]&LUO4I_MASKA)<<24) |
                                ((lp_src[0]&LUO4I_MASKB)<<40) |
                                ((lp_src[1]&LUO4I_MASKA)>>8) |
                                ((lp_src[1]&LUO4I_MASKB)>>8);
                    *lp_dest++=((*lp_dest&LUO4I_MASKC)>>16) |
                                ((*lp_dest&LUO4I_MASKD)<<16);
                    lp_src+=2;
                } while(--n>0);
		}
        if(odd_man_out) {
            *lp_dest=((lp_src[0]&LUO4I_MASKA)<<24) |
                    ((lp_src[0]&LUO4I_MASKB)<<40);
            *lp_dest++=((*lp_dest&LUO4I_MASKC)>>16) |
                    ((*lp_dest&LUO4I_MASKD)<<16);
          } /* end if */
#endif  /* DUFF_luo4i */
  }
  else 
    for(i = 0; i < num_elm; i++) {
      dest[0] = source[7];
      dest[1] = source[6];
      dest[2] = source[5];
      dest[3] = source[4];
      dest += dest_stride;
      source += source_stride;
    }
  return 0;
}

#define LUI4F_MASKA  0x0000008000000000
#define LUI4F_MASKB1 0x0000007f00000000
#define LUI4F_MASKB2 0x0000800000000000
#define LUI4F_MASKC1 0x00007f0000000000
#define LUI4F_MASKC2 0x00ff000000000000
#define LUI4F_MASKC3 0xff00000000000000
#define LUI4F_MASKD  0x0000800000000000
#define LUI4F_MASKE  0x0000000000000080
#define LUI4F_MASKF1 0x000000000000007f
#define LUI4F_MASKF2 0x0000000000008000
#define LUI4F_MASKG1 0x0000000000007f00
#define LUI4F_MASKG2 0x0000000000ff0000
#define LUI4F_MASKG3 0x00000000ff000000

/************************************************************/
/* DFKlui4f()                                                */
/* -->Unicos routine for importing 32 bit floats            */
/************************************************************/

/************************************************************

                     <<<< WARNING >>>>

    The nature of converting between 64 bit floating point
  numbers and 32 bit floating point numbers LOSES PRECISION.
  Taking a number in 64bit cray format, converting to IEEE
  (internal HDF format) and back will round the number at
  about the 7th decimal place.

 ************************************************************/

#ifdef PROTOTYPE
PRIVATE int DFKlui4f(VOIDP s, VOIDP d, uint32 num_elm, uint32 source_stride,
		   uint32 dest_stride)
#else
PRIVATE int DFKlui4f(source, dest, num_elm, source_stride, dest_stride)
uint8 * source;
uint8 * dest;
uint32 num_elm;
uint32 source_stride;
uint32 dest_stride;
#endif /* PROTOTYPE */
{
  int fast_processing = 0;              /* By default not array processed */
  int odd_man_out = 0;                  /* By default there are even num_elm */
  int i,j,n;
  long buf1;                            /* This is a temporary stride buf */
  long buf2;                            /* This is a temporary stride buf */
  uint8 * dud1 = (uint8*)&buf1;         /* Dummy pointer to buf1 for strides */
  uint8 * dud2 = (uint8*)&buf2;         /* Dummy pointer to buf2 for strides */
#ifdef PROTOTYPE
  uint8 * source = (uint8*)s;            /* Cray does not like certain   */
  uint8 * dest = (uint8*)d;              /* void and void* constructions */
#endif /* PROTOTYPE */
  long * lptr_src = (long*)source;
  long * lptr_dest = (long*)dest;
  char *FUNC="DFKui4f";

    HEclear();

    /* Check for conversion errors */
    if(source == dest || num_elm == 0) { /* Inplace conversions not permitted */
        HERROR(DFE_BADCONV);             /* under UNICOS */
        return FAIL;                     /* No elements convert is an error   */
    }

    /* Find out if it is OK to use faster array processing */
    if(source_stride == 0 && dest_stride == 0)
        fast_processing = 1;

    if(fast_processing) {
#ifndef DUFF_lui4f
#if defined TEST1_lui4f
        odd_man_out = num_elm%2;
        n = num_elm / 2;
        for(i = 0; i < n; i++) {
            lptr_dest[0] = (((*lptr_src & LUI4F_MASKA)<<24) |/* Sign bit */
                    (((*lptr_src & LUI4F_MASKB1) << 17) |  /* Exp. */
                    ((*lptr_src & LUI4F_MASKB2) << 1)) +
                    (16258 << 48)) |
                    (((*lptr_src & LUI4F_MASKC1) |         /* Mantissa */
                     ((*lptr_src & LUI4F_MASKC2) >>16) |
                     ((*lptr_src & LUI4F_MASKC3) >>32)) | LUI4F_MASKD);
            lptr_dest[1] = (((*lptr_src & LUI4F_MASKE) <<56) |/* Sign bit */
                    (((*lptr_src & LUI4F_MASKF1) << 49) |  /* Exp. */
                    ((*lptr_src  & LUI4F_MASKF2) << 33)) +
                    (16258 << 48)) |
                    ((((*lptr_src & LUI4F_MASKG1) <<32) |         /* Mantissa */
                     ((*lptr_src & LUI4F_MASKG2) <<16)|
                     (*lptr_src & LUI4F_MASKG3)) | LUI4F_MASKD);
            lptr_src++;
            lptr_dest+=2;
          } /* end for */
        if(odd_man_out)
            lptr_dest[0] = (((lptr_src[0] & LUI4F_MASKA)<<24) |/* Sign bit */
                    (((lptr_src[0] & LUI4F_MASKB1) << 17) |  /* Exp. */
                    ((lptr_src[0] & LUI4F_MASKB2) << 1)) +
                    (16258 << 48)) |
                    (((lptr_src[0] & LUI4F_MASKC1) |         /* Mantissa */
                     ((lptr_src[0] & LUI4F_MASKC2) >>16) |
                     ((lptr_src[0] & LUI4F_MASKC3) >>32)) | LUI4F_MASKD);
#else
        if((num_elm % 2))           /* If this is true, we have odd num */
            odd_man_out = 1;
        num_elm = num_elm / 2;
        for(i = 0; i < num_elm; i++) {
            if(*(float*)lptr_src != 0) {
                if(((*lptr_src & 0xffffffff00000000) << 1)== 0)
                    *lptr_dest = 0;
                else 
                    *lptr_dest = (((*lptr_src & LUI4F_MASKA)<<24) |/* Sign bit */
                            (((*lptr_src & LUI4F_MASKB1) << 17) |  /* Exp. */
                            ((*lptr_src & LUI4F_MASKB2) << 1)) +
                            (16258 << 48)) |
                            (((*lptr_src & LUI4F_MASKC1) |         /* Mantissa */
                             ((*lptr_src & LUI4F_MASKC2) >>16) |
                             ((*lptr_src & LUI4F_MASKC3) >>32)) | LUI4F_MASKD);
            }
            else
                *lptr_dest = *lptr_src;
            lptr_dest++;
            if(*(float*)lptr_src != 0) {
                if(((*lptr_src & 0x00000000ffffffff) << 33)== 0)
                    *lptr_dest = 0;
                else
                    *lptr_dest = ((((*lptr_src << 32) & LUI4F_MASKA)<<24) |/* Sign bit */
                            ((((*lptr_src << 32) & LUI4F_MASKB1) << 17) |  /* Exp. */
                            (((*lptr_src <<32 ) & LUI4F_MASKB2) << 1)) +
                            (16258 << 48)) |
                            ((((*lptr_src << 32) & LUI4F_MASKC1) |         /* Mantissa */
                             (((*lptr_src << 32) & LUI4F_MASKC2) >>16) |
                             (((*lptr_src << 32) & LUI4F_MASKC3) >>32)) | LUI4F_MASKD);
            }
            else
                *lptr_dest = *lptr_src;
            lptr_src++;
            lptr_dest++;
        }
        if(odd_man_out) {
            if(((float*)lptr_src)[0] != 0) {
                if(((lptr_src[0] & 0xffffffff00000000) << 1)== 0)
                    lptr_dest[0] = 0;
                else
                    lptr_dest[0] = (((lptr_src[0] & LUI4F_MASKA)<<24) |/* Sign bit */
                            (((lptr_src[0] & LUI4F_MASKB1) << 17) |  /* Exp. */
                            ((lptr_src[0] & LUI4F_MASKB2) << 1)) +
                            (16258 << 48)) |
                            (((lptr_src[0] & LUI4F_MASKC1) |         /* Mantissa */
                             ((lptr_src[0] & LUI4F_MASKC2) >>16) |
                             ((lptr_src[0] & LUI4F_MASKC3) >>32)) | LUI4F_MASKD);
            }
            else
                *lptr_dest = *lptr_src;
        }
#endif
#else   /* DUFF_lui4f */
        odd_man_out = num_elm % 2;

        num_elm/=2;
        n=(num_elm+7)/8;
        switch(num_elm%8) {
            case 0:
                do{
                    lptr_dest[0] = (((*lptr_src & LUI4F_MASKA)<<24) |/* Sign bit */
                            (((*lptr_src & LUI4F_MASKB1) << 17) |  /* Exp. */
                            ((*lptr_src & LUI4F_MASKB2) << 1)) +
                            (16258 << 48)) |
                            (((*lptr_src & LUI4F_MASKC1) |         /* Mantissa */
                             ((*lptr_src & LUI4F_MASKC2) >>16) |
                             ((*lptr_src & LUI4F_MASKC3) >>32)) | LUI4F_MASKD);
                    lptr_dest[1] = (((*lptr_src & LUI4F_MASKE) <<56) |/* Sign bit */
                            (((*lptr_src & LUI4F_MASKF1) << 49) |  /* Exp. */
                            ((*lptr_src  & LUI4F_MASKF2) << 33)) +
                            (16258 << 48)) |
                            ((((*lptr_src & LUI4F_MASKG1) <<32) |         /* Mantissa */
                             ((*lptr_src & LUI4F_MASKG2) <<16)|
                             (*lptr_src & LUI4F_MASKG3)) | LUI4F_MASKD);
                    lptr_src++;
                    lptr_dest+=2;
#ifdef QAK
            case 15:
                    lptr_dest[0] = (((*lptr_src & LUI4F_MASKA)<<24) |/* Sign bit */
                            (((*lptr_src & LUI4F_MASKB1) << 17) |  /* Exp. */
                            ((*lptr_src & LUI4F_MASKB2) << 1)) +
                            (16258 << 48)) |
                            (((*lptr_src & LUI4F_MASKC1) |         /* Mantissa */
                             ((*lptr_src & LUI4F_MASKC2) >>16) |
                             ((*lptr_src & LUI4F_MASKC3) >>32)) | LUI4F_MASKD);
                    lptr_dest[1] = (((*lptr_src & LUI4F_MASKE) <<56) |/* Sign bit */
                            (((*lptr_src & LUI4F_MASKF1) << 49) |  /* Exp. */
                            ((*lptr_src  & LUI4F_MASKF2) << 33)) +
                            (16258 << 48)) |
                            ((((*lptr_src & LUI4F_MASKG1) <<32) |         /* Mantissa */
                             ((*lptr_src & LUI4F_MASKG2) <<16)|
                             (*lptr_src & LUI4F_MASKG3)) | LUI4F_MASKD);
                    lptr_src++;
                    lptr_dest+=2;
            case 14:
                    lptr_dest[0] = (((*lptr_src & LUI4F_MASKA)<<24) |/* Sign bit */
                            (((*lptr_src & LUI4F_MASKB1) << 17) |  /* Exp. */
                            ((*lptr_src & LUI4F_MASKB2) << 1)) +
                            (16258 << 48)) |
                            (((*lptr_src & LUI4F_MASKC1) |         /* Mantissa */
                             ((*lptr_src & LUI4F_MASKC2) >>16) |
                             ((*lptr_src & LUI4F_MASKC3) >>32)) | LUI4F_MASKD);
                    lptr_dest[1] = (((*lptr_src & LUI4F_MASKE) <<56) |/* Sign bit */
                            (((*lptr_src & LUI4F_MASKF1) << 49) |  /* Exp. */
                            ((*lptr_src  & LUI4F_MASKF2) << 33)) +
                            (16258 << 48)) |
                            ((((*lptr_src & LUI4F_MASKG1) <<32) |         /* Mantissa */
                             ((*lptr_src & LUI4F_MASKG2) <<16)|
                             (*lptr_src & LUI4F_MASKG3)) | LUI4F_MASKD);
                    lptr_src++;
                    lptr_dest+=2;
            case 13:
                    lptr_dest[0] = (((*lptr_src & LUI4F_MASKA)<<24) |/* Sign bit */
                            (((*lptr_src & LUI4F_MASKB1) << 17) |  /* Exp. */
                            ((*lptr_src & LUI4F_MASKB2) << 1)) +
                            (16258 << 48)) |
                            (((*lptr_src & LUI4F_MASKC1) |         /* Mantissa */
                             ((*lptr_src & LUI4F_MASKC2) >>16) |
                             ((*lptr_src & LUI4F_MASKC3) >>32)) | LUI4F_MASKD);
                    lptr_dest[1] = (((*lptr_src & LUI4F_MASKE) <<56) |/* Sign bit */
                            (((*lptr_src & LUI4F_MASKF1) << 49) |  /* Exp. */
                            ((*lptr_src  & LUI4F_MASKF2) << 33)) +
                            (16258 << 48)) |
                            ((((*lptr_src & LUI4F_MASKG1) <<32) |         /* Mantissa */
                             ((*lptr_src & LUI4F_MASKG2) <<16)|
                             (*lptr_src & LUI4F_MASKG3)) | LUI4F_MASKD);
                    lptr_src++;
                    lptr_dest+=2;
            case 12:
                    lptr_dest[0] = (((*lptr_src & LUI4F_MASKA)<<24) |/* Sign bit */
                            (((*lptr_src & LUI4F_MASKB1) << 17) |  /* Exp. */
                            ((*lptr_src & LUI4F_MASKB2) << 1)) +
                            (16258 << 48)) |
                            (((*lptr_src & LUI4F_MASKC1) |         /* Mantissa */
                             ((*lptr_src & LUI4F_MASKC2) >>16) |
                             ((*lptr_src & LUI4F_MASKC3) >>32)) | LUI4F_MASKD);
                    lptr_dest[1] = (((*lptr_src & LUI4F_MASKE) <<56) |/* Sign bit */
                            (((*lptr_src & LUI4F_MASKF1) << 49) |  /* Exp. */
                            ((*lptr_src  & LUI4F_MASKF2) << 33)) +
                            (16258 << 48)) |
                            ((((*lptr_src & LUI4F_MASKG1) <<32) |         /* Mantissa */
                             ((*lptr_src & LUI4F_MASKG2) <<16)|
                             (*lptr_src & LUI4F_MASKG3)) | LUI4F_MASKD);
                    lptr_src++;
                    lptr_dest+=2;
            case 11:
                    lptr_dest[0] = (((*lptr_src & LUI4F_MASKA)<<24) |/* Sign bit */
                            (((*lptr_src & LUI4F_MASKB1) << 17) |  /* Exp. */
                            ((*lptr_src & LUI4F_MASKB2) << 1)) +
                            (16258 << 48)) |
                            (((*lptr_src & LUI4F_MASKC1) |         /* Mantissa */
                             ((*lptr_src & LUI4F_MASKC2) >>16) |
                             ((*lptr_src & LUI4F_MASKC3) >>32)) | LUI4F_MASKD);
                    lptr_dest[1] = (((*lptr_src & LUI4F_MASKE) <<56) |/* Sign bit */
                            (((*lptr_src & LUI4F_MASKF1) << 49) |  /* Exp. */
                            ((*lptr_src  & LUI4F_MASKF2) << 33)) +
                            (16258 << 48)) |
                            ((((*lptr_src & LUI4F_MASKG1) <<32) |         /* Mantissa */
                             ((*lptr_src & LUI4F_MASKG2) <<16)|
                             (*lptr_src & LUI4F_MASKG3)) | LUI4F_MASKD);
                    lptr_src++;
                    lptr_dest+=2;
            case 10:
                    lptr_dest[0] = (((*lptr_src & LUI4F_MASKA)<<24) |/* Sign bit */
                            (((*lptr_src & LUI4F_MASKB1) << 17) |  /* Exp. */
                            ((*lptr_src & LUI4F_MASKB2) << 1)) +
                            (16258 << 48)) |
                            (((*lptr_src & LUI4F_MASKC1) |         /* Mantissa */
                             ((*lptr_src & LUI4F_MASKC2) >>16) |
                             ((*lptr_src & LUI4F_MASKC3) >>32)) | LUI4F_MASKD);
                    lptr_dest[1] = (((*lptr_src & LUI4F_MASKE) <<56) |/* Sign bit */
                            (((*lptr_src & LUI4F_MASKF1) << 49) |  /* Exp. */
                            ((*lptr_src  & LUI4F_MASKF2) << 33)) +
                            (16258 << 48)) |
                            ((((*lptr_src & LUI4F_MASKG1) <<32) |         /* Mantissa */
                             ((*lptr_src & LUI4F_MASKG2) <<16)|
                             (*lptr_src & LUI4F_MASKG3)) | LUI4F_MASKD);
                    lptr_src++;
                    lptr_dest+=2;
            case 9:
                    lptr_dest[0] = (((*lptr_src & LUI4F_MASKA)<<24) |/* Sign bit */
                            (((*lptr_src & LUI4F_MASKB1) << 17) |  /* Exp. */
                            ((*lptr_src & LUI4F_MASKB2) << 1)) +
                            (16258 << 48)) |
                            (((*lptr_src & LUI4F_MASKC1) |         /* Mantissa */
                             ((*lptr_src & LUI4F_MASKC2) >>16) |
                             ((*lptr_src & LUI4F_MASKC3) >>32)) | LUI4F_MASKD);
                    lptr_dest[1] = (((*lptr_src & LUI4F_MASKE) <<56) |/* Sign bit */
                            (((*lptr_src & LUI4F_MASKF1) << 49) |  /* Exp. */
                            ((*lptr_src  & LUI4F_MASKF2) << 33)) +
                            (16258 << 48)) |
                            ((((*lptr_src & LUI4F_MASKG1) <<32) |         /* Mantissa */
                             ((*lptr_src & LUI4F_MASKG2) <<16)|
                             (*lptr_src & LUI4F_MASKG3)) | LUI4F_MASKD);
                    lptr_src++;
                    lptr_dest+=2;
            case 8:
                    lptr_dest[0] = (((*lptr_src & LUI4F_MASKA)<<24) |/* Sign bit */
                            (((*lptr_src & LUI4F_MASKB1) << 17) |  /* Exp. */
                            ((*lptr_src & LUI4F_MASKB2) << 1)) +
                            (16258 << 48)) |
                            (((*lptr_src & LUI4F_MASKC1) |         /* Mantissa */
                             ((*lptr_src & LUI4F_MASKC2) >>16) |
                             ((*lptr_src & LUI4F_MASKC3) >>32)) | LUI4F_MASKD);
                    lptr_dest[1] = (((*lptr_src & LUI4F_MASKE) <<56) |/* Sign bit */
                            (((*lptr_src & LUI4F_MASKF1) << 49) |  /* Exp. */
                            ((*lptr_src  & LUI4F_MASKF2) << 33)) +
                            (16258 << 48)) |
                            ((((*lptr_src & LUI4F_MASKG1) <<32) |         /* Mantissa */
                             ((*lptr_src & LUI4F_MASKG2) <<16)|
                             (*lptr_src & LUI4F_MASKG3)) | LUI4F_MASKD);
                    lptr_src++;
                    lptr_dest+=2;
#endif
            case 7:
                    lptr_dest[0] = (((*lptr_src & LUI4F_MASKA)<<24) |/* Sign bit */
                            (((*lptr_src & LUI4F_MASKB1) << 17) |  /* Exp. */
                            ((*lptr_src & LUI4F_MASKB2) << 1)) +
                            (16258 << 48)) |
                            (((*lptr_src & LUI4F_MASKC1) |         /* Mantissa */
                             ((*lptr_src & LUI4F_MASKC2) >>16) |
                             ((*lptr_src & LUI4F_MASKC3) >>32)) | LUI4F_MASKD);
                    lptr_dest[1] = (((*lptr_src & LUI4F_MASKE) <<56) |/* Sign bit */
                            (((*lptr_src & LUI4F_MASKF1) << 49) |  /* Exp. */
                            ((*lptr_src  & LUI4F_MASKF2) << 33)) +
                            (16258 << 48)) |
                            ((((*lptr_src & LUI4F_MASKG1) <<32) |         /* Mantissa */
                             ((*lptr_src & LUI4F_MASKG2) <<16)|
                             (*lptr_src & LUI4F_MASKG3)) | LUI4F_MASKD);
                    lptr_src++;
                    lptr_dest+=2;
            case 6:
                    lptr_dest[0] = (((*lptr_src & LUI4F_MASKA)<<24) |/* Sign bit */
                            (((*lptr_src & LUI4F_MASKB1) << 17) |  /* Exp. */
                            ((*lptr_src & LUI4F_MASKB2) << 1)) +
                            (16258 << 48)) |
                            (((*lptr_src & LUI4F_MASKC1) |         /* Mantissa */
                             ((*lptr_src & LUI4F_MASKC2) >>16) |
                             ((*lptr_src & LUI4F_MASKC3) >>32)) | LUI4F_MASKD);
                    lptr_dest[1] = (((*lptr_src & LUI4F_MASKE) <<56) |/* Sign bit */
                            (((*lptr_src & LUI4F_MASKF1) << 49) |  /* Exp. */
                            ((*lptr_src  & LUI4F_MASKF2) << 33)) +
                            (16258 << 48)) |
                            ((((*lptr_src & LUI4F_MASKG1) <<32) |         /* Mantissa */
                             ((*lptr_src & LUI4F_MASKG2) <<16)|
                             (*lptr_src & LUI4F_MASKG3)) | LUI4F_MASKD);
                    lptr_src++;
                    lptr_dest+=2;
            case 5:
                    lptr_dest[0] = (((*lptr_src & LUI4F_MASKA)<<24) |/* Sign bit */
                            (((*lptr_src & LUI4F_MASKB1) << 17) |  /* Exp. */
                            ((*lptr_src & LUI4F_MASKB2) << 1)) +
                            (16258 << 48)) |
                            (((*lptr_src & LUI4F_MASKC1) |         /* Mantissa */
                             ((*lptr_src & LUI4F_MASKC2) >>16) |
                             ((*lptr_src & LUI4F_MASKC3) >>32)) | LUI4F_MASKD);
                    lptr_dest[1] = (((*lptr_src & LUI4F_MASKE) <<56) |/* Sign bit */
                            (((*lptr_src & LUI4F_MASKF1) << 49) |  /* Exp. */
                            ((*lptr_src  & LUI4F_MASKF2) << 33)) +
                            (16258 << 48)) |
                            ((((*lptr_src & LUI4F_MASKG1) <<32) |         /* Mantissa */
                             ((*lptr_src & LUI4F_MASKG2) <<16)|
                             (*lptr_src & LUI4F_MASKG3)) | LUI4F_MASKD);
                    lptr_src++;
                    lptr_dest+=2;
            case 4:
                    lptr_dest[0] = (((*lptr_src & LUI4F_MASKA)<<24) |/* Sign bit */
                            (((*lptr_src & LUI4F_MASKB1) << 17) |  /* Exp. */
                            ((*lptr_src & LUI4F_MASKB2) << 1)) +
                            (16258 << 48)) |
                            (((*lptr_src & LUI4F_MASKC1) |         /* Mantissa */
                             ((*lptr_src & LUI4F_MASKC2) >>16) |
                             ((*lptr_src & LUI4F_MASKC3) >>32)) | LUI4F_MASKD);
                    lptr_dest[1] = (((*lptr_src & LUI4F_MASKE) <<56) |/* Sign bit */
                            (((*lptr_src & LUI4F_MASKF1) << 49) |  /* Exp. */
                            ((*lptr_src  & LUI4F_MASKF2) << 33)) +
                            (16258 << 48)) |
                            ((((*lptr_src & LUI4F_MASKG1) <<32) |         /* Mantissa */
                             ((*lptr_src & LUI4F_MASKG2) <<16)|
                             (*lptr_src & LUI4F_MASKG3)) | LUI4F_MASKD);
                    lptr_src++;
                    lptr_dest+=2;
            case 3:
                    lptr_dest[0] = (((*lptr_src & LUI4F_MASKA)<<24) |/* Sign bit */
                            (((*lptr_src & LUI4F_MASKB1) << 17) |  /* Exp. */
                            ((*lptr_src & LUI4F_MASKB2) << 1)) +
                            (16258 << 48)) |
                            (((*lptr_src & LUI4F_MASKC1) |         /* Mantissa */
                             ((*lptr_src & LUI4F_MASKC2) >>16) |
                             ((*lptr_src & LUI4F_MASKC3) >>32)) | LUI4F_MASKD);
                    lptr_dest[1] = (((*lptr_src & LUI4F_MASKE) <<56) |/* Sign bit */
                            (((*lptr_src & LUI4F_MASKF1) << 49) |  /* Exp. */
                            ((*lptr_src  & LUI4F_MASKF2) << 33)) +
                            (16258 << 48)) |
                            ((((*lptr_src & LUI4F_MASKG1) <<32) |         /* Mantissa */
                             ((*lptr_src & LUI4F_MASKG2) <<16)|
                             (*lptr_src & LUI4F_MASKG3)) | LUI4F_MASKD);
                    lptr_src++;
                    lptr_dest+=2;
            case 2:
                    lptr_dest[0] = (((*lptr_src & LUI4F_MASKA)<<24) |/* Sign bit */
                            (((*lptr_src & LUI4F_MASKB1) << 17) |  /* Exp. */
                            ((*lptr_src & LUI4F_MASKB2) << 1)) +
                            (16258 << 48)) |
                            (((*lptr_src & LUI4F_MASKC1) |         /* Mantissa */
                             ((*lptr_src & LUI4F_MASKC2) >>16) |
                             ((*lptr_src & LUI4F_MASKC3) >>32)) | LUI4F_MASKD);
                    lptr_dest[1] = (((*lptr_src & LUI4F_MASKE) <<56) |/* Sign bit */
                            (((*lptr_src & LUI4F_MASKF1) << 49) |  /* Exp. */
                            ((*lptr_src  & LUI4F_MASKF2) << 33)) +
                            (16258 << 48)) |
                            ((((*lptr_src & LUI4F_MASKG1) <<32) |         /* Mantissa */
                             ((*lptr_src & LUI4F_MASKG2) <<16)|
                             (*lptr_src & LUI4F_MASKG3)) | LUI4F_MASKD);
                    lptr_src++;
                    lptr_dest+=2;
            case 1:
                    lptr_dest[0] = (((*lptr_src & LUI4F_MASKA)<<24) |/* Sign bit */
                            (((*lptr_src & LUI4F_MASKB1) << 17) |  /* Exp. */
                            ((*lptr_src & LUI4F_MASKB2) << 1)) +
                            (16258 << 48)) |
                            (((*lptr_src & LUI4F_MASKC1) |         /* Mantissa */
                             ((*lptr_src & LUI4F_MASKC2) >>16) |
                             ((*lptr_src & LUI4F_MASKC3) >>32)) | LUI4F_MASKD);
                    lptr_dest[1] = (((*lptr_src & LUI4F_MASKE) <<56) |/* Sign bit */
                            (((*lptr_src & LUI4F_MASKF1) << 49) |  /* Exp. */
                            ((*lptr_src  & LUI4F_MASKF2) << 33)) +
                            (16258 << 48)) |
                            ((((*lptr_src & LUI4F_MASKG1) <<32) |         /* Mantissa */
                             ((*lptr_src & LUI4F_MASKG2) <<16)|
                             (*lptr_src & LUI4F_MASKG3)) | LUI4F_MASKD);
                    lptr_src++;
                    lptr_dest+=2;
                } while(--n>0);
		}
        if(odd_man_out)
            lptr_dest[0] = (((lptr_src[0] & LUI4F_MASKA)<<24) |/* Sign bit */
                    (((lptr_src[0] & LUI4F_MASKB1) << 17) |  /* Exp. */
                    ((lptr_src[0] & LUI4F_MASKB2) << 1)) +
                    (16258 << 48)) |
                    (((lptr_src[0] & LUI4F_MASKC1) |         /* Mantissa */
                     ((lptr_src[0] & LUI4F_MASKC2) >>16) |
                     ((lptr_src[0] & LUI4F_MASKC3) >>32)) | LUI4F_MASKD);
#endif  /* DUFF_lui4f */
    }
    else { /* We end up here if we are doing stride based processing */
        buf1 = 0;
        for(i = 0; i < num_elm; i++) {
            dud1[0] = source[3];        /* Loop would be less efficient */
            dud1[1] = source[2];
            dud1[2] = source[1];
            dud1[3] = source[0];

            if((float)buf1 != 0) {
                buf2 = (((buf1 & UI4F_MASKA) |
                        ((buf1 & UI4F_MASKB) >> 7) +
                        (16258 << 48)) |
                        (((buf1 & UI4F_MASKC) >> 8) | (UI4F_MASKD)));
                if((buf1 << 1)== 0)
                    buf2 = 0;
            }
            else
                buf2 = buf1;

            dest[0] = dud2[0];            /* Loop would be less efficient */
            dest[1] = dud2[1];
            dest[2] = dud2[2];
            dest[3] = dud2[3];
            dest[4] = dud2[4];
            dest[5] = dud2[5];
            dest[6] = dud2[6];
            dest[7] = dud2[7];

            source += source_stride;
            dest += dest_stride;
        }
    }
  return;
}


#define LUO4F_MASKA 0x8000000000000000
#define LUO4F_MASKB 0x7fff000000000000
#define LUO4F_MASKC 0x00007fffff000000
#define LUO4F_MASKD 0x0000000000800000
#define LUO4F_MASKE 0xff00ff00ff00ff00
#define LUO4F_MASKF 0x00ff00ff00ff00ff
#define LUO4F_MASKG 0xffff0000ffff0000
#define LUO4F_MASKH 0x0000ffff0000ffff

/************************************************************/
/* DFKluo4f()                                                */
/* -->Unicos routine for exporting 32 bit floats            */
/************************************************************/

/************************************************************

                     <<<< WARNING >>>>

    The nature of converting between 64 bit floating point
  numbers and 32 bit floating point numbers LOSES PRECISION.
  Taking a number in 64bit cray format, converting to IEEE
  (internal HDF format) and back will round the number at
  about the 7th decimal place.

 ************************************************************/

#ifdef PROTOTYPE
PRIVATE int DFKluo4f(VOIDP s, VOIDP d, uint32 num_elm, uint32 source_stride,
		   uint32 dest_stride)
#else
PRIVATE int DFKluo4f(source, dest, num_elm, source_stride, dest_stride)
uint8 * source, * dest;
uint32 num_elm, source_stride, dest_stride;
#endif /* PROTOTYPE */
{
  int fast_processing = 0;              /* By default not array processed */
  int odd_man_out = 0;                  /* By default there are even num_elm */
  int i,j,n;
  long buf1;                            /* This is a temporary stride buf */
  long buf2;                            /* This is a temporary stride buf */
  uint8 * dud1 = (uint8*)&buf1;         /* Dummy pointer to buf1 for strides */
  uint8 * dud2 = (uint8*)&buf2;         /* Dummy pointer to buf2 for strides */
#ifdef PROTOTYPE
  uint8 * source = (uint8*)s;           /* Cray does not like certain   */
  uint8 * dest = (uint8*)d;             /* void and void* constructions */
#endif /* PROTOTYPE */
  long * lptr_src = (long*)source;
  long * lptr_dest = (long*)dest;
  char *FUNC="DFKuo4f";

  HEclear();

  /* Check for conversion errors */
  if(source == dest || num_elm == 0) { /* Inplace conversions not permitted */
    HERROR(DFE_BADCONV);               /* under UNICOS */
    return FAIL;                       /* No elements convert is an error   */
  }

  /* Find out if it is OK to use faster array processing */
  if(source_stride == 0 && dest_stride == 0)
      fast_processing = 1;            

    if(fast_processing) {
#ifndef DUFF_luo4f
#if defined TEST1_luo4f
        odd_man_out = num_elm%2;
        n=num_elm/2;
        for(i = 0; i < n; i++) {
            buf1 = (((lptr_src[0] & LUO4F_MASKA) |
                    ((((lptr_src[0] & LUO4F_MASKB) >> 48) - 16258) << 55)) +
                    (((lptr_src[0] & LUO4F_MASKC) +
                    ((lptr_src[0] & LUO4F_MASKD) << 1)) << 8)) |
                    ((((lptr_src[1] & LUO4F_MASKA) |
                    ((((lptr_src[1] & LUO4F_MASKB) >> 48) - 16258) << 55)) +
                    (((lptr_src[1] & LUO4F_MASKC) +
                    ((lptr_src[1] & LUO4F_MASKD) << 1)) << 8)) >> 32);
            buf1 = ((buf1 & LUO4F_MASKE)>>8) | ((buf1 & LUO4F_MASKF)<<8);
            lptr_dest[0] = ((buf1 & LUO4F_MASKG)>>16) | ((buf1 & LUO4F_MASKH)<<16);
            lptr_src+=2;
            lptr_dest ++;
        }
        if(odd_man_out) {
            buf1 = ((lptr_src[0] & LUO4F_MASKA) |
                    ((((lptr_src[0]  & LUO4F_MASKB) >> 48) - 16258) << 55)) +
                    (((lptr_src[0]  & LUO4F_MASKC) +
                    ((lptr_src[0]  & LUO4F_MASKD) << 1)) << 8);
            buf1 = ((buf1 & LUO4F_MASKE)>>8) | ((buf1 & LUO4F_MASKF)<<8);
            lptr_dest[0] = ((buf1 & LUO4F_MASKG)>>16) | ((buf1 & LUO4F_MASKH)<<16);
        }
#else
        if((num_elm % 2))                   /* If this is true, we have odd num */
          odd_man_out = 1;
        for(i = 0; i < (int)(num_elm/2); i++) {
            buf1 = lptr_src[0];
            buf2 = lptr_src[1];
            if(buf1 != 0)
                buf1 = (((buf1 & LUO4F_MASKA) |
                        ((((buf1 & LUO4F_MASKB) >> 48) - 16258) << 55)) +
                        (((buf1 & LUO4F_MASKC) +
                        ((buf1 & LUO4F_MASKD) << 1)) << 8));
            else
                buf1 = 0;
            if(buf2 != 0)
                buf2 = ((((buf2 & LUO4F_MASKA) |
                        ((((buf2 & LUO4F_MASKB) >> 48) - 16258) << 55)) +
                        (((buf2 & LUO4F_MASKC) +
                        ((buf2 & LUO4F_MASKD) << 1)) << 8)) >> 32);
            else
                buf2 = 0;
            lptr_dest[0] = (((buf1 & 0xff00000000000000) >>24) |
                            ((buf1 & 0x00ff000000000000) >>8) |
                            ((buf1 & 0x0000ff0000000000) <<8) |
                            ((buf1 & 0x000000ff00000000) <<24) ) |
                           (((buf2 & 0x00000000000000ff) <<24) |
                            ((buf2 & 0x000000000000ff00) <<8) |
                            ((buf2 & 0x0000000000ff0000) >>8) |
                            ((buf2 & 0x00000000ff000000) >>24));
            lptr_src ++;
            lptr_src ++;
            lptr_dest ++;
        }
        if(odd_man_out) {
            buf1 = lptr_src[0];
            if(buf1 != 0) {
                buf1 = ((buf1 & LUO4F_MASKA) |
                        ((((buf1 & LUO4F_MASKB) >> 48) - 16258) << 55)) +
                        (((buf1 & LUO4F_MASKC) +
                        ((buf1 & LUO4F_MASKD) << 1)) << 8);
                lptr_dest[0] = ((buf1 & 0xff00000000000000) >>24) |
                               ((buf1 & 0x00ff000000000000) >>8) |
                               ((buf1 & 0x0000ff0000000000) <<8) |
                               ((buf1 & 0x000000ff00000000) <<24) ;
              } /* end if */
            else
                lptr_dest[0] = 0;
        }
#endif
#else   /* DUFF_luo4f */
        odd_man_out = num_elm % 2;

        num_elm/=2;
        n=(num_elm+7)/8;
        switch(num_elm%8) {
            case 0:
                do{
                    buf1 = (((lptr_src[0] & LUO4F_MASKA) |
                            ((((lptr_src[0] & LUO4F_MASKB) >> 48) - 16258) << 55)) +
                            (((lptr_src[0] & LUO4F_MASKC) +
                            ((lptr_src[0] & LUO4F_MASKD) << 1)) << 8)) |
                            ((((lptr_src[1] & LUO4F_MASKA) |
                            ((((lptr_src[1] & LUO4F_MASKB) >> 48) - 16258) << 55)) +
                            (((lptr_src[1] & LUO4F_MASKC) +
                            ((lptr_src[1] & LUO4F_MASKD) << 1)) << 8)) >> 32);
                    buf1 = ((buf1 & LUO4F_MASKE)>>8) | ((buf1 & LUO4F_MASKF)<<8);
                    lptr_dest[0] = ((buf1 & LUO4F_MASKG)>>16) | ((buf1 & LUO4F_MASKH)<<16);
                    lptr_src+=2;
                    lptr_dest ++;
#ifdef QAK
            case 15:
                    buf1 = (((lptr_src[0] & LUO4F_MASKA) |
                            ((((lptr_src[0] & LUO4F_MASKB) >> 48) - 16258) << 55)) +
                            (((lptr_src[0] & LUO4F_MASKC) +
                            ((lptr_src[0] & LUO4F_MASKD) << 1)) << 8)) |
                            ((((lptr_src[1] & LUO4F_MASKA) |
                            ((((lptr_src[1] & LUO4F_MASKB) >> 48) - 16258) << 55)) +
                            (((lptr_src[1] & LUO4F_MASKC) +
                            ((lptr_src[1] & LUO4F_MASKD) << 1)) << 8)) >> 32);
                    buf1 = ((buf1 & LUO4F_MASKE)>>8) | ((buf1 & LUO4F_MASKF)<<8);
                    lptr_dest[0] = ((buf1 & LUO4F_MASKG)>>16) | ((buf1 & LUO4F_MASKH)<<16);
                    lptr_src+=2;
                    lptr_dest ++;
            case 14:
                    buf1 = (((lptr_src[0] & LUO4F_MASKA) |
                            ((((lptr_src[0] & LUO4F_MASKB) >> 48) - 16258) << 55)) +
                            (((lptr_src[0] & LUO4F_MASKC) +
                            ((lptr_src[0] & LUO4F_MASKD) << 1)) << 8)) |
                            ((((lptr_src[1] & LUO4F_MASKA) |
                            ((((lptr_src[1] & LUO4F_MASKB) >> 48) - 16258) << 55)) +
                            (((lptr_src[1] & LUO4F_MASKC) +
                            ((lptr_src[1] & LUO4F_MASKD) << 1)) << 8)) >> 32);
                    buf1 = ((buf1 & LUO4F_MASKE)>>8) | ((buf1 & LUO4F_MASKF)<<8);
                    lptr_dest[0] = ((buf1 & LUO4F_MASKG)>>16) | ((buf1 & LUO4F_MASKH)<<16);
                    lptr_src+=2;
                    lptr_dest ++;
            case 13:
                    buf1 = (((lptr_src[0] & LUO4F_MASKA) |
                            ((((lptr_src[0] & LUO4F_MASKB) >> 48) - 16258) << 55)) +
                            (((lptr_src[0] & LUO4F_MASKC) +
                            ((lptr_src[0] & LUO4F_MASKD) << 1)) << 8)) |
                            ((((lptr_src[1] & LUO4F_MASKA) |
                            ((((lptr_src[1] & LUO4F_MASKB) >> 48) - 16258) << 55)) +
                            (((lptr_src[1] & LUO4F_MASKC) +
                            ((lptr_src[1] & LUO4F_MASKD) << 1)) << 8)) >> 32);
                    buf1 = ((buf1 & LUO4F_MASKE)>>8) | ((buf1 & LUO4F_MASKF)<<8);
                    lptr_dest[0] = ((buf1 & LUO4F_MASKG)>>16) | ((buf1 & LUO4F_MASKH)<<16);
                    lptr_src+=2;
                    lptr_dest ++;
            case 12:
                    buf1 = (((lptr_src[0] & LUO4F_MASKA) |
                            ((((lptr_src[0] & LUO4F_MASKB) >> 48) - 16258) << 55)) +
                            (((lptr_src[0] & LUO4F_MASKC) +
                            ((lptr_src[0] & LUO4F_MASKD) << 1)) << 8)) |
                            ((((lptr_src[1] & LUO4F_MASKA) |
                            ((((lptr_src[1] & LUO4F_MASKB) >> 48) - 16258) << 55)) +
                            (((lptr_src[1] & LUO4F_MASKC) +
                            ((lptr_src[1] & LUO4F_MASKD) << 1)) << 8)) >> 32);
                    buf1 = ((buf1 & LUO4F_MASKE)>>8) | ((buf1 & LUO4F_MASKF)<<8);
                    lptr_dest[0] = ((buf1 & LUO4F_MASKG)>>16) | ((buf1 & LUO4F_MASKH)<<16);
                    lptr_src+=2;
                    lptr_dest ++;
            case 11:
                    buf1 = (((lptr_src[0] & LUO4F_MASKA) |
                            ((((lptr_src[0] & LUO4F_MASKB) >> 48) - 16258) << 55)) +
                            (((lptr_src[0] & LUO4F_MASKC) +
                            ((lptr_src[0] & LUO4F_MASKD) << 1)) << 8)) |
                            ((((lptr_src[1] & LUO4F_MASKA) |
                            ((((lptr_src[1] & LUO4F_MASKB) >> 48) - 16258) << 55)) +
                            (((lptr_src[1] & LUO4F_MASKC) +
                            ((lptr_src[1] & LUO4F_MASKD) << 1)) << 8)) >> 32);
                    buf1 = ((buf1 & LUO4F_MASKE)>>8) | ((buf1 & LUO4F_MASKF)<<8);
                    lptr_dest[0] = ((buf1 & LUO4F_MASKG)>>16) | ((buf1 & LUO4F_MASKH)<<16);
                    lptr_src+=2;
                    lptr_dest ++;
            case 10:
                    buf1 = (((lptr_src[0] & LUO4F_MASKA) |
                            ((((lptr_src[0] & LUO4F_MASKB) >> 48) - 16258) << 55)) +
                            (((lptr_src[0] & LUO4F_MASKC) +
                            ((lptr_src[0] & LUO4F_MASKD) << 1)) << 8)) |
                            ((((lptr_src[1] & LUO4F_MASKA) |
                            ((((lptr_src[1] & LUO4F_MASKB) >> 48) - 16258) << 55)) +
                            (((lptr_src[1] & LUO4F_MASKC) +
                            ((lptr_src[1] & LUO4F_MASKD) << 1)) << 8)) >> 32);
                    buf1 = ((buf1 & LUO4F_MASKE)>>8) | ((buf1 & LUO4F_MASKF)<<8);
                    lptr_dest[0] = ((buf1 & LUO4F_MASKG)>>16) | ((buf1 & LUO4F_MASKH)<<16);
                    lptr_src+=2;
                    lptr_dest ++;
            case 9:
                    buf1 = (((lptr_src[0] & LUO4F_MASKA) |
                            ((((lptr_src[0] & LUO4F_MASKB) >> 48) - 16258) << 55)) +
                            (((lptr_src[0] & LUO4F_MASKC) +
                            ((lptr_src[0] & LUO4F_MASKD) << 1)) << 8)) |
                            ((((lptr_src[1] & LUO4F_MASKA) |
                            ((((lptr_src[1] & LUO4F_MASKB) >> 48) - 16258) << 55)) +
                            (((lptr_src[1] & LUO4F_MASKC) +
                            ((lptr_src[1] & LUO4F_MASKD) << 1)) << 8)) >> 32);
                    buf1 = ((buf1 & LUO4F_MASKE)>>8) | ((buf1 & LUO4F_MASKF)<<8);
                    lptr_dest[0] = ((buf1 & LUO4F_MASKG)>>16) | ((buf1 & LUO4F_MASKH)<<16);
                    lptr_src+=2;
                    lptr_dest ++;
            case 8:
                    buf1 = (((lptr_src[0] & LUO4F_MASKA) |
                            ((((lptr_src[0] & LUO4F_MASKB) >> 48) - 16258) << 55)) +
                            (((lptr_src[0] & LUO4F_MASKC) +
                            ((lptr_src[0] & LUO4F_MASKD) << 1)) << 8)) |
                            ((((lptr_src[1] & LUO4F_MASKA) |
                            ((((lptr_src[1] & LUO4F_MASKB) >> 48) - 16258) << 55)) +
                            (((lptr_src[1] & LUO4F_MASKC) +
                            ((lptr_src[1] & LUO4F_MASKD) << 1)) << 8)) >> 32);
                    buf1 = ((buf1 & LUO4F_MASKE)>>8) | ((buf1 & LUO4F_MASKF)<<8);
                    lptr_dest[0] = ((buf1 & LUO4F_MASKG)>>16) | ((buf1 & LUO4F_MASKH)<<16);
                    lptr_src+=2;
                    lptr_dest ++;
#endif
            case 7:
                    buf1 = (((lptr_src[0] & LUO4F_MASKA) |
                            ((((lptr_src[0] & LUO4F_MASKB) >> 48) - 16258) << 55)) +
                            (((lptr_src[0] & LUO4F_MASKC) +
                            ((lptr_src[0] & LUO4F_MASKD) << 1)) << 8)) |
                            ((((lptr_src[1] & LUO4F_MASKA) |
                            ((((lptr_src[1] & LUO4F_MASKB) >> 48) - 16258) << 55)) +
                            (((lptr_src[1] & LUO4F_MASKC) +
                            ((lptr_src[1] & LUO4F_MASKD) << 1)) << 8)) >> 32);
                    buf1 = ((buf1 & LUO4F_MASKE)>>8) | ((buf1 & LUO4F_MASKF)<<8);
                    lptr_dest[0] = ((buf1 & LUO4F_MASKG)>>16) | ((buf1 & LUO4F_MASKH)<<16);
                    lptr_src+=2;
                    lptr_dest ++;
            case 6:
                    buf1 = (((lptr_src[0] & LUO4F_MASKA) |
                            ((((lptr_src[0] & LUO4F_MASKB) >> 48) - 16258) << 55)) +
                            (((lptr_src[0] & LUO4F_MASKC) +
                            ((lptr_src[0] & LUO4F_MASKD) << 1)) << 8)) |
                            ((((lptr_src[1] & LUO4F_MASKA) |
                            ((((lptr_src[1] & LUO4F_MASKB) >> 48) - 16258) << 55)) +
                            (((lptr_src[1] & LUO4F_MASKC) +
                            ((lptr_src[1] & LUO4F_MASKD) << 1)) << 8)) >> 32);
                    buf1 = ((buf1 & LUO4F_MASKE)>>8) | ((buf1 & LUO4F_MASKF)<<8);
                    lptr_dest[0] = ((buf1 & LUO4F_MASKG)>>16) | ((buf1 & LUO4F_MASKH)<<16);
                    lptr_src+=2;
                    lptr_dest ++;
            case 5:
                    buf1 = (((lptr_src[0] & LUO4F_MASKA) |
                            ((((lptr_src[0] & LUO4F_MASKB) >> 48) - 16258) << 55)) +
                            (((lptr_src[0] & LUO4F_MASKC) +
                            ((lptr_src[0] & LUO4F_MASKD) << 1)) << 8)) |
                            ((((lptr_src[1] & LUO4F_MASKA) |
                            ((((lptr_src[1] & LUO4F_MASKB) >> 48) - 16258) << 55)) +
                            (((lptr_src[1] & LUO4F_MASKC) +
                            ((lptr_src[1] & LUO4F_MASKD) << 1)) << 8)) >> 32);
                    buf1 = ((buf1 & LUO4F_MASKE)>>8) | ((buf1 & LUO4F_MASKF)<<8);
                    lptr_dest[0] = ((buf1 & LUO4F_MASKG)>>16) | ((buf1 & LUO4F_MASKH)<<16);
                    lptr_src+=2;
                    lptr_dest ++;
            case 4:
                    buf1 = (((lptr_src[0] & LUO4F_MASKA) |
                            ((((lptr_src[0] & LUO4F_MASKB) >> 48) - 16258) << 55)) +
                            (((lptr_src[0] & LUO4F_MASKC) +
                            ((lptr_src[0] & LUO4F_MASKD) << 1)) << 8)) |
                            ((((lptr_src[1] & LUO4F_MASKA) |
                            ((((lptr_src[1] & LUO4F_MASKB) >> 48) - 16258) << 55)) +
                            (((lptr_src[1] & LUO4F_MASKC) +
                            ((lptr_src[1] & LUO4F_MASKD) << 1)) << 8)) >> 32);
                    buf1 = ((buf1 & LUO4F_MASKE)>>8) | ((buf1 & LUO4F_MASKF)<<8);
                    lptr_dest[0] = ((buf1 & LUO4F_MASKG)>>16) | ((buf1 & LUO4F_MASKH)<<16);
                    lptr_src+=2;
                    lptr_dest ++;
            case 3:
                    buf1 = (((lptr_src[0] & LUO4F_MASKA) |
                            ((((lptr_src[0] & LUO4F_MASKB) >> 48) - 16258) << 55)) +
                            (((lptr_src[0] & LUO4F_MASKC) +
                            ((lptr_src[0] & LUO4F_MASKD) << 1)) << 8)) |
                            ((((lptr_src[1] & LUO4F_MASKA) |
                            ((((lptr_src[1] & LUO4F_MASKB) >> 48) - 16258) << 55)) +
                            (((lptr_src[1] & LUO4F_MASKC) +
                            ((lptr_src[1] & LUO4F_MASKD) << 1)) << 8)) >> 32);
                    buf1 = ((buf1 & LUO4F_MASKE)>>8) | ((buf1 & LUO4F_MASKF)<<8);
                    lptr_dest[0] = ((buf1 & LUO4F_MASKG)>>16) | ((buf1 & LUO4F_MASKH)<<16);
                    lptr_src+=2;
                    lptr_dest ++;
            case 2:
                    buf1 = (((lptr_src[0] & LUO4F_MASKA) |
                            ((((lptr_src[0] & LUO4F_MASKB) >> 48) - 16258) << 55)) +
                            (((lptr_src[0] & LUO4F_MASKC) +
                            ((lptr_src[0] & LUO4F_MASKD) << 1)) << 8)) |
                            ((((lptr_src[1] & LUO4F_MASKA) |
                            ((((lptr_src[1] & LUO4F_MASKB) >> 48) - 16258) << 55)) +
                            (((lptr_src[1] & LUO4F_MASKC) +
                            ((lptr_src[1] & LUO4F_MASKD) << 1)) << 8)) >> 32);
                    buf1 = ((buf1 & LUO4F_MASKE)>>8) | ((buf1 & LUO4F_MASKF)<<8);
                    lptr_dest[0] = ((buf1 & LUO4F_MASKG)>>16) | ((buf1 & LUO4F_MASKH)<<16);
                    lptr_src+=2;
                    lptr_dest ++;
            case 1:
                    buf1 = (((lptr_src[0] & LUO4F_MASKA) |
                            ((((lptr_src[0] & LUO4F_MASKB) >> 48) - 16258) << 55)) +
                            (((lptr_src[0] & LUO4F_MASKC) +
                            ((lptr_src[0] & LUO4F_MASKD) << 1)) << 8)) |
                            ((((lptr_src[1] & LUO4F_MASKA) |
                            ((((lptr_src[1] & LUO4F_MASKB) >> 48) - 16258) << 55)) +
                            (((lptr_src[1] & LUO4F_MASKC) +
                            ((lptr_src[1] & LUO4F_MASKD) << 1)) << 8)) >> 32);
                    buf1 = ((buf1 & LUO4F_MASKE)>>8) | ((buf1 & LUO4F_MASKF)<<8);
                    lptr_dest[0] = ((buf1 & LUO4F_MASKG)>>16) | ((buf1 & LUO4F_MASKH)<<16);
                    lptr_src+=2;
                    lptr_dest ++;
                } while(--n>0);
		}
        if(odd_man_out) {
            buf1 = ((lptr_src[0] & LUO4F_MASKA) |
                    ((((lptr_src[0]  & LUO4F_MASKB) >> 48) - 16258) << 55)) +
                    (((lptr_src[0]  & LUO4F_MASKC) +
                    ((lptr_src[0]  & LUO4F_MASKD) << 1)) << 8);
            buf1 = ((buf1 & LUO4F_MASKE)>>8) | ((buf1 & LUO4F_MASKF)<<8);
            lptr_dest[0] = ((buf1 & LUO4F_MASKG)>>16) | ((buf1 & LUO4F_MASKH)<<16);
        }
#endif  /* DUFF_luo4f */
    }
    else { /* We end up here if we are doing stride based processing */
        buf1 = 0;
        for(i = 0; i < num_elm; i++) {
            dud1[0] = source[0];        /* Loop would be less efficient */
            dud1[1] = source[1];
            dud1[2] = source[2];
            dud1[3] = source[3];
            dud1[4] = source[4];
            dud1[5] = source[5];
            dud1[6] = source[6];
            dud1[7] = source[7];
      
            if((float)buf1 != 0)
                buf2 = (((buf1 & LUO4F_MASKA) |
                        ((((buf1 & LUO4F_MASKB) >> 48) -16258) << 55)) +
                        (((buf1 & LUO4F_MASKC) + ((buf1 & LUO4F_MASKD) << 1)) << 8));
            else
                buf2 = buf1;

            dest[3] = dud2[0];            /* Loop would be less efficient */
            dest[2] = dud2[1];
            dest[1] = dud2[2];
            dest[0] = dud2[3];

            source += source_stride;
            dest += dest_stride;
        }
    }
    return;
}

#define LUI8F_MASKA  0x0000000000000080
#define LUI8F_MASKB1 0x000000000000007f
#define LUI8F_MASKB2 0x000000000000f000
#define LUI8F_MASKC1 0x0000000000000f00
#define LUI8F_MASKC2 0x0000000000ff0000
#define LUI8F_MASKC3 0x00000000ff000000
#define LUI8F_MASKC4 0x000000ff00000000
#define LUI8F_MASKC5 0x0000ff0000000000
#define LUI8F_MASKC6 0x00ff000000000000
#define LUI8F_MASKC7 0xff00000000000000
#define LUI8F_MASKD  0x0800000000000000
#define LUI8F_MASKE  0x0000800000000000
#define LUI8F_MASKF 0xff00ff00ff00ff00
#define LUI8F_MASKG 0x00ff00ff00ff00ff
#define LUI8F_MASKH 0xffff0000ffff0000
#define LUI8F_MASKI 0x0000ffff0000ffff
#define LUI8F_MASKJ 0xffffffff00000000
#define LUI8F_MASKK 0x00000000ffffffff

/************************************************************/
/* DFKlui8f()                                                */
/* -->Unicos routine for importing 64 bit floats            */
/************************************************************/

#ifdef PROTOTYPE
PRIVATE int DFKlui8f(VOIDP s, VOIDP d, uint32 num_elm, uint32 source_stride,
		   uint32 dest_stride)
#else
PRIVATE int DFKlui8f(source, dest, num_elm, source_stride, dest_stride)
uint8 * source, * dest;
uint32 num_elm, source_stride, dest_stride;
#endif /* PROTOTYPE */
{
  int fast_processing = 0;              /* By default not array processed */
  int i,j,n;
  long buf;                             /* This is a temporary stride buf */
  uint8 * dud = (uint8*)&buf;           /* Dummy pointer to buf1 for strides */
#ifdef PROTOTYPE
  uint8 * source = (uint8*)s;           /* Cray does not like certain   */
  uint8 * dest = (uint8*)d;             /* void and void* constructions */
#endif /* PROTOTYPE*/
  long * lptr_src = (long*)source;
  long * lptr_dest = (long*)dest;
  char *FUNC="DFKui8f";

  HEclear();

  /* Check for conversion errors */
  if(source == dest || num_elm == 0) { /* Inplace conversions not permitted */
    HERROR(DFE_BADCONV);               /* under UNICOS */
    return FAIL;                       /* No elements convert is an error   */
  }

  /* Find out if it is OK to use faster array processing */
    if(source_stride == 0 && dest_stride == 0)
        fast_processing = 1;

    if(fast_processing) {
#ifndef DUFF_lui8f
#if defined TEST1_lui8f
        n=num_elm;
        for(i = 0; i < n; i ++) {
            lptr_dest[0] = (((lptr_src[0] & LUI8F_MASKA) << 56) | /* Sign */
                    (((lptr_src[0] & LUI8F_MASKB1) << 52) |   /* Exp. */
                    ((lptr_src[0] & LUI8F_MASKB2) << 36)) + (15362 << 48)) |
                    ((((((lptr_src[0] & LUI8F_MASKC1) << 40) | /* Mantissa */
                        ((lptr_src[0] & LUI8F_MASKC2) << 24) |
                        ((lptr_src[0] & LUI8F_MASKC3) << 8) |
                        ((lptr_src[0] & LUI8F_MASKC4) >> 8) |
                        ((lptr_src[0] & LUI8F_MASKC5) >> 24) |
                        ((lptr_src[0] & LUI8F_MASKC6) >> 40) |
                        ((lptr_src[0] & LUI8F_MASKC7) >> 56)) +
                    ((lptr_src[0] & LUI8F_MASKD) >> 55)) >>5) | LUI8F_MASKE);
            lptr_src++;
            lptr_dest++;
        }
#else
        for(i = 0; i < num_elm; i ++) {
            if (lptr_src[0] != 0) {
                if ((lptr_src[0] & (~LUI8F_MASKA)) == 0)
                    lptr_dest[0] = 0;
                else
                    lptr_dest[0] = (((lptr_src[0] & LUI8F_MASKA) << 56) | /* Sign */
                            (((lptr_src[0] & LUI8F_MASKB1) << 52) |   /* Exp. */
                            ((lptr_src[0] & LUI8F_MASKB2) << 36)) + (15362 << 48)) |
                            ((((((lptr_src[0] & LUI8F_MASKC1) << 40) | /* Mantissa */
                                ((lptr_src[0] & LUI8F_MASKC2) << 24) |
                                ((lptr_src[0] & LUI8F_MASKC3) << 8) |
                                ((lptr_src[0] & LUI8F_MASKC4) >> 8) |
                                ((lptr_src[0] & LUI8F_MASKC5) >> 24) |
                                ((lptr_src[0] & LUI8F_MASKC6) >> 40) |
                                ((lptr_src[0] & LUI8F_MASKC7) >> 56)) +
                            ((lptr_src[0] & LUI8F_MASKD) >> 55)) >>5) | LUI8F_MASKE);
            }
            else
                lptr_dest[0] = 0;
            lptr_src++;
            lptr_dest++;
        }
#endif
#else   /* DUFF_lui8f */
        n=(num_elm+7)/8;
        switch(num_elm%8) {
            case 0:
                do{
                    lptr_dest[0] = (((lptr_src[0] & LUI8F_MASKA) << 56) | /* Sign */
                            (((lptr_src[0] & LUI8F_MASKB1) << 52) |   /* Exp. */
                            ((lptr_src[0] & LUI8F_MASKB2) << 36)) + (15362 << 48)) |
                            ((((((lptr_src[0] & LUI8F_MASKC1) << 40) | /* Mantissa */
                                ((lptr_src[0] & LUI8F_MASKC2) << 24) |
                                ((lptr_src[0] & LUI8F_MASKC3) << 8) |
                                ((lptr_src[0] & LUI8F_MASKC4) >> 8) |
                                ((lptr_src[0] & LUI8F_MASKC5) >> 24) |
                                ((lptr_src[0] & LUI8F_MASKC6) >> 40) |
                                ((lptr_src[0] & LUI8F_MASKC7) >> 56)) +
                            ((lptr_src[0] & LUI8F_MASKD) >> 55)) >>5) | LUI8F_MASKE);
                    lptr_src++;
                    lptr_dest++;
#ifdef QAK
            case 15:
                    lptr_dest[0] = (((lptr_src[0] & LUI8F_MASKA) << 56) | /* Sign */
                            (((lptr_src[0] & LUI8F_MASKB1) << 52) |   /* Exp. */
                            ((lptr_src[0] & LUI8F_MASKB2) << 36)) + (15362 << 48)) |
                            ((((((lptr_src[0] & LUI8F_MASKC1) << 40) | /* Mantissa */
                                ((lptr_src[0] & LUI8F_MASKC2) << 24) |
                                ((lptr_src[0] & LUI8F_MASKC3) << 8) |
                                ((lptr_src[0] & LUI8F_MASKC4) >> 8) |
                                ((lptr_src[0] & LUI8F_MASKC5) >> 24) |
                                ((lptr_src[0] & LUI8F_MASKC6) >> 40) |
                                ((lptr_src[0] & LUI8F_MASKC7) >> 56)) +
                            ((lptr_src[0] & LUI8F_MASKD) >> 55)) >>5) | LUI8F_MASKE);
                    lptr_src++;
                    lptr_dest++;
            case 14:
                    lptr_dest[0] = (((lptr_src[0] & LUI8F_MASKA) << 56) | /* Sign */
                            (((lptr_src[0] & LUI8F_MASKB1) << 52) |   /* Exp. */
                            ((lptr_src[0] & LUI8F_MASKB2) << 36)) + (15362 << 48)) |
                            ((((((lptr_src[0] & LUI8F_MASKC1) << 40) | /* Mantissa */
                                ((lptr_src[0] & LUI8F_MASKC2) << 24) |
                                ((lptr_src[0] & LUI8F_MASKC3) << 8) |
                                ((lptr_src[0] & LUI8F_MASKC4) >> 8) |
                                ((lptr_src[0] & LUI8F_MASKC5) >> 24) |
                                ((lptr_src[0] & LUI8F_MASKC6) >> 40) |
                                ((lptr_src[0] & LUI8F_MASKC7) >> 56)) +
                            ((lptr_src[0] & LUI8F_MASKD) >> 55)) >>5) | LUI8F_MASKE);
                    lptr_src++;
                    lptr_dest++;
            case 13:
                    lptr_dest[0] = (((lptr_src[0] & LUI8F_MASKA) << 56) | /* Sign */
                            (((lptr_src[0] & LUI8F_MASKB1) << 52) |   /* Exp. */
                            ((lptr_src[0] & LUI8F_MASKB2) << 36)) + (15362 << 48)) |
                            ((((((lptr_src[0] & LUI8F_MASKC1) << 40) | /* Mantissa */
                                ((lptr_src[0] & LUI8F_MASKC2) << 24) |
                                ((lptr_src[0] & LUI8F_MASKC3) << 8) |
                                ((lptr_src[0] & LUI8F_MASKC4) >> 8) |
                                ((lptr_src[0] & LUI8F_MASKC5) >> 24) |
                                ((lptr_src[0] & LUI8F_MASKC6) >> 40) |
                                ((lptr_src[0] & LUI8F_MASKC7) >> 56)) +
                            ((lptr_src[0] & LUI8F_MASKD) >> 55)) >>5) | LUI8F_MASKE);
                    lptr_src++;
                    lptr_dest++;
            case 12:
                    lptr_dest[0] = (((lptr_src[0] & LUI8F_MASKA) << 56) | /* Sign */
                            (((lptr_src[0] & LUI8F_MASKB1) << 52) |   /* Exp. */
                            ((lptr_src[0] & LUI8F_MASKB2) << 36)) + (15362 << 48)) |
                            ((((((lptr_src[0] & LUI8F_MASKC1) << 40) | /* Mantissa */
                                ((lptr_src[0] & LUI8F_MASKC2) << 24) |
                                ((lptr_src[0] & LUI8F_MASKC3) << 8) |
                                ((lptr_src[0] & LUI8F_MASKC4) >> 8) |
                                ((lptr_src[0] & LUI8F_MASKC5) >> 24) |
                                ((lptr_src[0] & LUI8F_MASKC6) >> 40) |
                                ((lptr_src[0] & LUI8F_MASKC7) >> 56)) +
                            ((lptr_src[0] & LUI8F_MASKD) >> 55)) >>5) | LUI8F_MASKE);
                    lptr_src++;
                    lptr_dest++;
            case 11:
                    lptr_dest[0] = (((lptr_src[0] & LUI8F_MASKA) << 56) | /* Sign */
                            (((lptr_src[0] & LUI8F_MASKB1) << 52) |   /* Exp. */
                            ((lptr_src[0] & LUI8F_MASKB2) << 36)) + (15362 << 48)) |
                            ((((((lptr_src[0] & LUI8F_MASKC1) << 40) | /* Mantissa */
                                ((lptr_src[0] & LUI8F_MASKC2) << 24) |
                                ((lptr_src[0] & LUI8F_MASKC3) << 8) |
                                ((lptr_src[0] & LUI8F_MASKC4) >> 8) |
                                ((lptr_src[0] & LUI8F_MASKC5) >> 24) |
                                ((lptr_src[0] & LUI8F_MASKC6) >> 40) |
                                ((lptr_src[0] & LUI8F_MASKC7) >> 56)) +
                            ((lptr_src[0] & LUI8F_MASKD) >> 55)) >>5) | LUI8F_MASKE);
                    lptr_src++;
                    lptr_dest++;
            case 10:
                    lptr_dest[0] = (((lptr_src[0] & LUI8F_MASKA) << 56) | /* Sign */
                            (((lptr_src[0] & LUI8F_MASKB1) << 52) |   /* Exp. */
                            ((lptr_src[0] & LUI8F_MASKB2) << 36)) + (15362 << 48)) |
                            ((((((lptr_src[0] & LUI8F_MASKC1) << 40) | /* Mantissa */
                                ((lptr_src[0] & LUI8F_MASKC2) << 24) |
                                ((lptr_src[0] & LUI8F_MASKC3) << 8) |
                                ((lptr_src[0] & LUI8F_MASKC4) >> 8) |
                                ((lptr_src[0] & LUI8F_MASKC5) >> 24) |
                                ((lptr_src[0] & LUI8F_MASKC6) >> 40) |
                                ((lptr_src[0] & LUI8F_MASKC7) >> 56)) +
                            ((lptr_src[0] & LUI8F_MASKD) >> 55)) >>5) | LUI8F_MASKE);
                    lptr_src++;
                    lptr_dest++;
            case 9:
                    lptr_dest[0] = (((lptr_src[0] & LUI8F_MASKA) << 56) | /* Sign */
                            (((lptr_src[0] & LUI8F_MASKB1) << 52) |   /* Exp. */
                            ((lptr_src[0] & LUI8F_MASKB2) << 36)) + (15362 << 48)) |
                            ((((((lptr_src[0] & LUI8F_MASKC1) << 40) | /* Mantissa */
                                ((lptr_src[0] & LUI8F_MASKC2) << 24) |
                                ((lptr_src[0] & LUI8F_MASKC3) << 8) |
                                ((lptr_src[0] & LUI8F_MASKC4) >> 8) |
                                ((lptr_src[0] & LUI8F_MASKC5) >> 24) |
                                ((lptr_src[0] & LUI8F_MASKC6) >> 40) |
                                ((lptr_src[0] & LUI8F_MASKC7) >> 56)) +
                            ((lptr_src[0] & LUI8F_MASKD) >> 55)) >>5) | LUI8F_MASKE);
                    lptr_src++;
                    lptr_dest++;
            case 8:
                    lptr_dest[0] = (((lptr_src[0] & LUI8F_MASKA) << 56) | /* Sign */
                            (((lptr_src[0] & LUI8F_MASKB1) << 52) |   /* Exp. */
                            ((lptr_src[0] & LUI8F_MASKB2) << 36)) + (15362 << 48)) |
                            ((((((lptr_src[0] & LUI8F_MASKC1) << 40) | /* Mantissa */
                                ((lptr_src[0] & LUI8F_MASKC2) << 24) |
                                ((lptr_src[0] & LUI8F_MASKC3) << 8) |
                                ((lptr_src[0] & LUI8F_MASKC4) >> 8) |
                                ((lptr_src[0] & LUI8F_MASKC5) >> 24) |
                                ((lptr_src[0] & LUI8F_MASKC6) >> 40) |
                                ((lptr_src[0] & LUI8F_MASKC7) >> 56)) +
                            ((lptr_src[0] & LUI8F_MASKD) >> 55)) >>5) | LUI8F_MASKE);
                    lptr_src++;
                    lptr_dest++;
#endif
            case 7:
                    lptr_dest[0] = (((lptr_src[0] & LUI8F_MASKA) << 56) | /* Sign */
                            (((lptr_src[0] & LUI8F_MASKB1) << 52) |   /* Exp. */
                            ((lptr_src[0] & LUI8F_MASKB2) << 36)) + (15362 << 48)) |
                            ((((((lptr_src[0] & LUI8F_MASKC1) << 40) | /* Mantissa */
                                ((lptr_src[0] & LUI8F_MASKC2) << 24) |
                                ((lptr_src[0] & LUI8F_MASKC3) << 8) |
                                ((lptr_src[0] & LUI8F_MASKC4) >> 8) |
                                ((lptr_src[0] & LUI8F_MASKC5) >> 24) |
                                ((lptr_src[0] & LUI8F_MASKC6) >> 40) |
                                ((lptr_src[0] & LUI8F_MASKC7) >> 56)) +
                            ((lptr_src[0] & LUI8F_MASKD) >> 55)) >>5) | LUI8F_MASKE);
                    lptr_src++;
                    lptr_dest++;
            case 6:
                    lptr_dest[0] = (((lptr_src[0] & LUI8F_MASKA) << 56) | /* Sign */
                            (((lptr_src[0] & LUI8F_MASKB1) << 52) |   /* Exp. */
                            ((lptr_src[0] & LUI8F_MASKB2) << 36)) + (15362 << 48)) |
                            ((((((lptr_src[0] & LUI8F_MASKC1) << 40) | /* Mantissa */
                                ((lptr_src[0] & LUI8F_MASKC2) << 24) |
                                ((lptr_src[0] & LUI8F_MASKC3) << 8) |
                                ((lptr_src[0] & LUI8F_MASKC4) >> 8) |
                                ((lptr_src[0] & LUI8F_MASKC5) >> 24) |
                                ((lptr_src[0] & LUI8F_MASKC6) >> 40) |
                                ((lptr_src[0] & LUI8F_MASKC7) >> 56)) +
                            ((lptr_src[0] & LUI8F_MASKD) >> 55)) >>5) | LUI8F_MASKE);
                    lptr_src++;
                    lptr_dest++;
            case 5:
                    lptr_dest[0] = (((lptr_src[0] & LUI8F_MASKA) << 56) | /* Sign */
                            (((lptr_src[0] & LUI8F_MASKB1) << 52) |   /* Exp. */
                            ((lptr_src[0] & LUI8F_MASKB2) << 36)) + (15362 << 48)) |
                            ((((((lptr_src[0] & LUI8F_MASKC1) << 40) | /* Mantissa */
                                ((lptr_src[0] & LUI8F_MASKC2) << 24) |
                                ((lptr_src[0] & LUI8F_MASKC3) << 8) |
                                ((lptr_src[0] & LUI8F_MASKC4) >> 8) |
                                ((lptr_src[0] & LUI8F_MASKC5) >> 24) |
                                ((lptr_src[0] & LUI8F_MASKC6) >> 40) |
                                ((lptr_src[0] & LUI8F_MASKC7) >> 56)) +
                            ((lptr_src[0] & LUI8F_MASKD) >> 55)) >>5) | LUI8F_MASKE);
                    lptr_src++;
                    lptr_dest++;
            case 4:
                    lptr_dest[0] = (((lptr_src[0] & LUI8F_MASKA) << 56) | /* Sign */
                            (((lptr_src[0] & LUI8F_MASKB1) << 52) |   /* Exp. */
                            ((lptr_src[0] & LUI8F_MASKB2) << 36)) + (15362 << 48)) |
                            ((((((lptr_src[0] & LUI8F_MASKC1) << 40) | /* Mantissa */
                                ((lptr_src[0] & LUI8F_MASKC2) << 24) |
                                ((lptr_src[0] & LUI8F_MASKC3) << 8) |
                                ((lptr_src[0] & LUI8F_MASKC4) >> 8) |
                                ((lptr_src[0] & LUI8F_MASKC5) >> 24) |
                                ((lptr_src[0] & LUI8F_MASKC6) >> 40) |
                                ((lptr_src[0] & LUI8F_MASKC7) >> 56)) +
                            ((lptr_src[0] & LUI8F_MASKD) >> 55)) >>5) | LUI8F_MASKE);
                    lptr_src++;
                    lptr_dest++;
            case 3:
                    lptr_dest[0] = (((lptr_src[0] & LUI8F_MASKA) << 56) | /* Sign */
                            (((lptr_src[0] & LUI8F_MASKB1) << 52) |   /* Exp. */
                            ((lptr_src[0] & LUI8F_MASKB2) << 36)) + (15362 << 48)) |
                            ((((((lptr_src[0] & LUI8F_MASKC1) << 40) | /* Mantissa */
                                ((lptr_src[0] & LUI8F_MASKC2) << 24) |
                                ((lptr_src[0] & LUI8F_MASKC3) << 8) |
                                ((lptr_src[0] & LUI8F_MASKC4) >> 8) |
                                ((lptr_src[0] & LUI8F_MASKC5) >> 24) |
                                ((lptr_src[0] & LUI8F_MASKC6) >> 40) |
                                ((lptr_src[0] & LUI8F_MASKC7) >> 56)) +
                            ((lptr_src[0] & LUI8F_MASKD) >> 55)) >>5) | LUI8F_MASKE);
                    lptr_src++;
                    lptr_dest++;
            case 2:
                    lptr_dest[0] = (((lptr_src[0] & LUI8F_MASKA) << 56) | /* Sign */
                            (((lptr_src[0] & LUI8F_MASKB1) << 52) |   /* Exp. */
                            ((lptr_src[0] & LUI8F_MASKB2) << 36)) + (15362 << 48)) |
                            ((((((lptr_src[0] & LUI8F_MASKC1) << 40) | /* Mantissa */
                                ((lptr_src[0] & LUI8F_MASKC2) << 24) |
                                ((lptr_src[0] & LUI8F_MASKC3) << 8) |
                                ((lptr_src[0] & LUI8F_MASKC4) >> 8) |
                                ((lptr_src[0] & LUI8F_MASKC5) >> 24) |
                                ((lptr_src[0] & LUI8F_MASKC6) >> 40) |
                                ((lptr_src[0] & LUI8F_MASKC7) >> 56)) +
                            ((lptr_src[0] & LUI8F_MASKD) >> 55)) >>5) | LUI8F_MASKE);
                    lptr_src++;
                    lptr_dest++;
            case 1:
                    lptr_dest[0] = (((lptr_src[0] & LUI8F_MASKA) << 56) | /* Sign */
                            (((lptr_src[0] & LUI8F_MASKB1) << 52) |   /* Exp. */
                            ((lptr_src[0] & LUI8F_MASKB2) << 36)) + (15362 << 48)) |
                            ((((((lptr_src[0] & LUI8F_MASKC1) << 40) | /* Mantissa */
                                ((lptr_src[0] & LUI8F_MASKC2) << 24) |
                                ((lptr_src[0] & LUI8F_MASKC3) << 8) |
                                ((lptr_src[0] & LUI8F_MASKC4) >> 8) |
                                ((lptr_src[0] & LUI8F_MASKC5) >> 24) |
                                ((lptr_src[0] & LUI8F_MASKC6) >> 40) |
                                ((lptr_src[0] & LUI8F_MASKC7) >> 56)) +
                            ((lptr_src[0] & LUI8F_MASKD) >> 55)) >>5) | LUI8F_MASKE);
                    lptr_src++;
                    lptr_dest++;
                } while(--n>0);
        }
#endif  /* DUFF_lui8f */
      } /* end if */
    else
        for(i = 0; i < num_elm; i ++) {
            dud[0] = source[7];
            dud[1] = source[6];
            dud[2] = source[5];
            dud[3] = source[4];
            dud[4] = source[3];
            dud[5] = source[2];
            dud[6] = source[1];
            dud[7] = source[0];

            if (buf != 0) {
                buf = (((buf & UI8F_MASKA) | ((buf & UI8F_MASKB) >> 4) +
                        (15362 << 48)) |
                        ((((buf & UI8F_MASKC) + ((buf & UI8F_MASKD) << 1)) >> 5) |
                        (UI8F_MASKE)) );
                if ((buf << 1) == 0)
                    buf = 0;
            }
            else
                buf = 0;

            dest[0] = dud[0];
            dest[1] = dud[1];
            dest[2] = dud[2];
            dest[3] = dud[3];
            dest[4] = dud[4];
            dest[5] = dud[5];
            dest[6] = dud[6];
            dest[7] = dud[7];

            source += source_stride;
            dest += dest_stride;
        }
  return;
}

#define LUO8F_MASKA 0x8000000000000000
#define LUO8F_MASKB 0x7fff000000000000
#define LUO8F_MASKC 0x00007fffffffffff
#define LUO8F_MASKD 0xff00ff00ff00ff00
#define LUO8F_MASKE 0x00ff00ff00ff00ff
#define LUO8F_MASKF 0xffff0000ffff0000
#define LUO8F_MASKG 0x0000ffff0000ffff
#define LUO8F_MASKH 0xffffffff00000000
#define LUO8F_MASKI 0x00000000ffffffff

/************************************************************/
/* DFKluo8f()                                               */
/* -->Unicos routine for exporting 64 bit floats            */
/************************************************************/

#ifdef PROTOTYPE
PRIVATE int DFKluo8f(VOIDP s, VOIDP d, uint32 num_elm, uint32 source_stride,
		   uint32 dest_stride)
#else
PRIVATE int DFKluo8f(source, dest, num_elm, source_stride, dest_stride)
uint8 * source, * dest;
uint32 num_elm, source_stride, dest_stride;
#endif /* PROTOTYPE */
{
  int fast_processing = 0;              /* By default not array processed */
  int odd_man_out = 0;                  /* By default there are even num_elm */
  int i,j,n;
  long buf;                             /* This is a temporary stride buf */
  uint8 * dud = (uint8*)&buf;           /* Dummy pointer to buf1 for strides */
#ifdef PROTOTYPE
  uint8 * source = (uint8*)s;           /* Cray does not like certain   */
  uint8 * dest = (uint8*)d;             /* void and void* constructions */
#endif /* PROTOTYPE */
  long * lptr_src = (long*)source;
  long * lptr_dest = (long*)dest;
  char *FUNC="DFKuo8f";

  HEclear();

  /* Check for conversion errors */
  if(source == dest || num_elm == 0) { /* Inplace conversions not permitted */
    HERROR(DFE_BADCONV);               /* under UNICOS */
    return FAIL;                       /* No elements convert is an error   */
  }

  /* Find out if it is OK to use faster array processing */
    if(source_stride == 0 && dest_stride == 0)
        fast_processing = 1;

    if(fast_processing) {
#ifndef DUFF_luo8f
#if defined TEST1_luo8f
        n=num_elm;
        for(i = 0; i < n; i ++) {
            buf = (((lptr_src[0] & LUO8F_MASKA) |
                    (((((lptr_src[0] & LUO8F_MASKB) >> 48) - 15362) << 53) >> 1)) +
                    ((lptr_src[0] & LUO8F_MASKC) << 5));
            buf = ((buf & LUO8F_MASKD)>>8) | ((buf & LUO8F_MASKE)<<8);
            buf = ((buf & LUO8F_MASKF)>>16) | ((buf & LUO8F_MASKG)<<16);
            lptr_dest[0] = ((buf & LUO8F_MASKH)>>32) | ((buf & LUO8F_MASKI)<<32);
            lptr_src++;
            lptr_dest++;
        }
#else
        for(i = 0; i < num_elm; i ++) {
            if (lptr_src[0] != 0) {
                buf = (((lptr_src[0] & LUO8F_MASKA) |
                        (((((lptr_src[0] & LUO8F_MASKB) >> 48) - 15362) << 53) >> 1)) +
                        ((lptr_src[0] & LUO8F_MASKC) << 5));
                lptr_dest[0] = ((buf & 0xff00000000000000) >>56) |
                               ((buf & 0x00ff000000000000) >>40) |
                               ((buf & 0x0000ff0000000000) >>24) |
                               ((buf & 0x000000ff00000000) >>8) |
                               ((buf & 0x00000000ff000000) <<8) |
                               ((buf & 0x0000000000ff0000) <<24) |
                               ((buf & 0x000000000000ff00) <<40) |
                               ((buf & 0x00000000000000ff) <<56);
              } /* end if */
            else
                lptr_dest[0] = 0;
            lptr_src++;
            lptr_dest++;
        }
#endif
#else   /* DUFF_luo8f */
        n=(num_elm+7)/8;
        switch(num_elm%8) {
            case 0:
                do{
                    buf = (((lptr_src[0] & LUO8F_MASKA) |
                            (((((lptr_src[0] & LUO8F_MASKB) >> 48) - 15362) << 53) >> 1)) +
                            ((lptr_src[0] & LUO8F_MASKC) << 5));
                    buf = ((buf & LUO8F_MASKD)>>8) | ((buf & LUO8F_MASKE)<<8);
                    buf = ((buf & LUO8F_MASKF)>>16) | ((buf & LUO8F_MASKG)<<16);
                    lptr_dest[0] = ((buf & LUO8F_MASKH)>>32) | ((buf & LUO8F_MASKI)<<32);
                    lptr_src++;
                    lptr_dest++;
#ifdef QAK
            case 15:
                    buf = (((lptr_src[0] & LUO8F_MASKA) |
                            (((((lptr_src[0] & LUO8F_MASKB) >> 48) - 15362) << 53) >> 1)) +
                            ((lptr_src[0] & LUO8F_MASKC) << 5));
                    buf = ((buf & LUO8F_MASKD)>>8) | ((buf & LUO8F_MASKE)<<8);
                    buf = ((buf & LUO8F_MASKF)>>16) | ((buf & LUO8F_MASKG)<<16);
                    lptr_dest[0] = ((buf & LUO8F_MASKH)>>32) | ((buf & LUO8F_MASKI)<<32);
                    lptr_src++;
                    lptr_dest++;
            case 14:
                    buf = (((lptr_src[0] & LUO8F_MASKA) |
                            (((((lptr_src[0] & LUO8F_MASKB) >> 48) - 15362) << 53) >> 1)) +
                            ((lptr_src[0] & LUO8F_MASKC) << 5));
                    buf = ((buf & LUO8F_MASKD)>>8) | ((buf & LUO8F_MASKE)<<8);
                    buf = ((buf & LUO8F_MASKF)>>16) | ((buf & LUO8F_MASKG)<<16);
                    lptr_dest[0] = ((buf & LUO8F_MASKH)>>32) | ((buf & LUO8F_MASKI)<<32);
                    lptr_src++;
                    lptr_dest++;
            case 13:
                    buf = (((lptr_src[0] & LUO8F_MASKA) |
                            (((((lptr_src[0] & LUO8F_MASKB) >> 48) - 15362) << 53) >> 1)) +
                            ((lptr_src[0] & LUO8F_MASKC) << 5));
                    buf = ((buf & LUO8F_MASKD)>>8) | ((buf & LUO8F_MASKE)<<8);
                    buf = ((buf & LUO8F_MASKF)>>16) | ((buf & LUO8F_MASKG)<<16);
                    lptr_dest[0] = ((buf & LUO8F_MASKH)>>32) | ((buf & LUO8F_MASKI)<<32);
                    lptr_src++;
                    lptr_dest++;
            case 12:
                    buf = (((lptr_src[0] & LUO8F_MASKA) |
                            (((((lptr_src[0] & LUO8F_MASKB) >> 48) - 15362) << 53) >> 1)) +
                            ((lptr_src[0] & LUO8F_MASKC) << 5));
                    buf = ((buf & LUO8F_MASKD)>>8) | ((buf & LUO8F_MASKE)<<8);
                    buf = ((buf & LUO8F_MASKF)>>16) | ((buf & LUO8F_MASKG)<<16);
                    lptr_dest[0] = ((buf & LUO8F_MASKH)>>32) | ((buf & LUO8F_MASKI)<<32);
                    lptr_src++;
                    lptr_dest++;
            case 11:
                    buf = (((lptr_src[0] & LUO8F_MASKA) |
                            (((((lptr_src[0] & LUO8F_MASKB) >> 48) - 15362) << 53) >> 1)) +
                            ((lptr_src[0] & LUO8F_MASKC) << 5));
                    buf = ((buf & LUO8F_MASKD)>>8) | ((buf & LUO8F_MASKE)<<8);
                    buf = ((buf & LUO8F_MASKF)>>16) | ((buf & LUO8F_MASKG)<<16);
                    lptr_dest[0] = ((buf & LUO8F_MASKH)>>32) | ((buf & LUO8F_MASKI)<<32);
                    lptr_src++;
                    lptr_dest++;
            case 10:
                    buf = (((lptr_src[0] & LUO8F_MASKA) |
                            (((((lptr_src[0] & LUO8F_MASKB) >> 48) - 15362) << 53) >> 1)) +
                            ((lptr_src[0] & LUO8F_MASKC) << 5));
                    buf = ((buf & LUO8F_MASKD)>>8) | ((buf & LUO8F_MASKE)<<8);
                    buf = ((buf & LUO8F_MASKF)>>16) | ((buf & LUO8F_MASKG)<<16);
                    lptr_dest[0] = ((buf & LUO8F_MASKH)>>32) | ((buf & LUO8F_MASKI)<<32);
                    lptr_src++;
                    lptr_dest++;
            case 9:
                    buf = (((lptr_src[0] & LUO8F_MASKA) |
                            (((((lptr_src[0] & LUO8F_MASKB) >> 48) - 15362) << 53) >> 1)) +
                            ((lptr_src[0] & LUO8F_MASKC) << 5));
                    buf = ((buf & LUO8F_MASKD)>>8) | ((buf & LUO8F_MASKE)<<8);
                    buf = ((buf & LUO8F_MASKF)>>16) | ((buf & LUO8F_MASKG)<<16);
                    lptr_dest[0] = ((buf & LUO8F_MASKH)>>32) | ((buf & LUO8F_MASKI)<<32);
                    lptr_src++;
                    lptr_dest++;
            case 8:
                    buf = (((lptr_src[0] & LUO8F_MASKA) |
                            (((((lptr_src[0] & LUO8F_MASKB) >> 48) - 15362) << 53) >> 1)) +
                            ((lptr_src[0] & LUO8F_MASKC) << 5));
                    buf = ((buf & LUO8F_MASKD)>>8) | ((buf & LUO8F_MASKE)<<8);
                    buf = ((buf & LUO8F_MASKF)>>16) | ((buf & LUO8F_MASKG)<<16);
                    lptr_dest[0] = ((buf & LUO8F_MASKH)>>32) | ((buf & LUO8F_MASKI)<<32);
                    lptr_src++;
                    lptr_dest++;
#endif
            case 7:
                    buf = (((lptr_src[0] & LUO8F_MASKA) |
                            (((((lptr_src[0] & LUO8F_MASKB) >> 48) - 15362) << 53) >> 1)) +
                            ((lptr_src[0] & LUO8F_MASKC) << 5));
                    buf = ((buf & LUO8F_MASKD)>>8) | ((buf & LUO8F_MASKE)<<8);
                    buf = ((buf & LUO8F_MASKF)>>16) | ((buf & LUO8F_MASKG)<<16);
                    lptr_dest[0] = ((buf & LUO8F_MASKH)>>32) | ((buf & LUO8F_MASKI)<<32);
                    lptr_src++;
                    lptr_dest++;
            case 6:
                    buf = (((lptr_src[0] & LUO8F_MASKA) |
                            (((((lptr_src[0] & LUO8F_MASKB) >> 48) - 15362) << 53) >> 1)) +
                            ((lptr_src[0] & LUO8F_MASKC) << 5));
                    buf = ((buf & LUO8F_MASKD)>>8) | ((buf & LUO8F_MASKE)<<8);
                    buf = ((buf & LUO8F_MASKF)>>16) | ((buf & LUO8F_MASKG)<<16);
                    lptr_dest[0] = ((buf & LUO8F_MASKH)>>32) | ((buf & LUO8F_MASKI)<<32);
                    lptr_src++;
                    lptr_dest++;
            case 5:
                    buf = (((lptr_src[0] & LUO8F_MASKA) |
                            (((((lptr_src[0] & LUO8F_MASKB) >> 48) - 15362) << 53) >> 1)) +
                            ((lptr_src[0] & LUO8F_MASKC) << 5));
                    buf = ((buf & LUO8F_MASKD)>>8) | ((buf & LUO8F_MASKE)<<8);
                    buf = ((buf & LUO8F_MASKF)>>16) | ((buf & LUO8F_MASKG)<<16);
                    lptr_dest[0] = ((buf & LUO8F_MASKH)>>32) | ((buf & LUO8F_MASKI)<<32);
                    lptr_src++;
                    lptr_dest++;
            case 4:
                    buf = (((lptr_src[0] & LUO8F_MASKA) |
                            (((((lptr_src[0] & LUO8F_MASKB) >> 48) - 15362) << 53) >> 1)) +
                            ((lptr_src[0] & LUO8F_MASKC) << 5));
                    buf = ((buf & LUO8F_MASKD)>>8) | ((buf & LUO8F_MASKE)<<8);
                    buf = ((buf & LUO8F_MASKF)>>16) | ((buf & LUO8F_MASKG)<<16);
                    lptr_dest[0] = ((buf & LUO8F_MASKH)>>32) | ((buf & LUO8F_MASKI)<<32);
                    lptr_src++;
                    lptr_dest++;
            case 3:
                    buf = (((lptr_src[0] & LUO8F_MASKA) |
                            (((((lptr_src[0] & LUO8F_MASKB) >> 48) - 15362) << 53) >> 1)) +
                            ((lptr_src[0] & LUO8F_MASKC) << 5));
                    buf = ((buf & LUO8F_MASKD)>>8) | ((buf & LUO8F_MASKE)<<8);
                    buf = ((buf & LUO8F_MASKF)>>16) | ((buf & LUO8F_MASKG)<<16);
                    lptr_dest[0] = ((buf & LUO8F_MASKH)>>32) | ((buf & LUO8F_MASKI)<<32);
                    lptr_src++;
                    lptr_dest++;
            case 2:
                    buf = (((lptr_src[0] & LUO8F_MASKA) |
                            (((((lptr_src[0] & LUO8F_MASKB) >> 48) - 15362) << 53) >> 1)) +
                            ((lptr_src[0] & LUO8F_MASKC) << 5));
                    buf = ((buf & LUO8F_MASKD)>>8) | ((buf & LUO8F_MASKE)<<8);
                    buf = ((buf & LUO8F_MASKF)>>16) | ((buf & LUO8F_MASKG)<<16);
                    lptr_dest[0] = ((buf & LUO8F_MASKH)>>32) | ((buf & LUO8F_MASKI)<<32);
                    lptr_src++;
                    lptr_dest++;
            case 1:
                    buf = (((lptr_src[0] & LUO8F_MASKA) |
                            (((((lptr_src[0] & LUO8F_MASKB) >> 48) - 15362) << 53) >> 1)) +
                            ((lptr_src[0] & LUO8F_MASKC) << 5));
                    buf = ((buf & LUO8F_MASKD)>>8) | ((buf & LUO8F_MASKE)<<8);
                    buf = ((buf & LUO8F_MASKF)>>16) | ((buf & LUO8F_MASKG)<<16);
                    lptr_dest[0] = ((buf & LUO8F_MASKH)>>32) | ((buf & LUO8F_MASKI)<<32);
                    lptr_src++;
                    lptr_dest++;
                } while(--n>0);
		}
#endif  /* DUFF_luo8f */
      } /* end if */
    else
        for(i = 0; i < num_elm; i ++) {
            dud[0] = source[0];
            dud[1] = source[1];
            dud[2] = source[2];
            dud[3] = source[3];
            dud[4] = source[4];
            dud[5] = source[5];
            dud[6] = source[6];
            dud[7] = source[7];

            if (buf != 0) {
                buf = (((buf & LUO8F_MASKA) |             /* sign bit */
                        (((((buf & LUO8F_MASKB) >> 48) - 15362) << 53) >> 1)) |/* exp */
                        ((buf & LUO8F_MASKC) << 5));      /* mantissa */
            }
            else
                buf = 0;

            dest[7] = dud[0];
            dest[6] = dud[1];
            dest[5] = dud[2];
            dest[4] = dud[3];
            dest[3] = dud[4];
            dest[2] = dud[5];
            dest[1] = dud[6];
            dest[0] = dud[7];

            source += source_stride;
            dest += dest_stride;
        }
  return;
}

#endif /* UNICOS */

#if defined(VMS)

/************************************************************/
/* NUMBER CONVERSION ROUTINES FOR VAX ARCHITECTURES         */
/* Most of the VAX number types can be handled with the     */
/* generic byte swapping routines.  Only the floating point */
/* conversion routines need to be customized.               */
/************************************************************/

/************************************************************/
/* DFKvi4f()                                                */
/* --> Import routine for 4 byte VAX floats                 */
/************************************************************/
#ifdef PROTOTYPE
PRIVATE int DFKvi4f(VOIDP s, VOIDP d, uint32 num_elm, uint32 source_stride,
		   uint32 dest_stride)
#else
PRIVATE int DFKvi4f(source, dest, num_elm, source_stride, dest_stride)
uint8 * source, * dest;
uint32 num_elm, source_stride, dest_stride;
#endif /* PROTOTYPE */
{
  int in_place = 0;                     /* Inplace must be detected */
  register uint32 i;            
  uint8 buf[4];                          /* Inplace processing buffer */
#ifdef PROTOTYPE
  uint8 * source = (uint8*)s;
  uint8 * dest = (uint8*)d;
#endif /* PROTOTYPE */
  char *FUNC="DFKvi4f";
  uint8 exp;

  HEclear();

  if(source == dest)
      in_place = TRUE;

  if(num_elm == 0 || in_place){   /* No elements is an error as is in place. */
    HERROR(DFE_BADCONV);
    return FAIL;
  }

  if(source_stride == 0 && dest_stride == 0)
      source_stride = dest_stride = 4;

  for(i = 0; i < num_elm; i++) {

      /* extract exponent */
      exp = (source[0] << 1) | (source[1] >> 7); 
      if (exp) {       
          /* 
           * non-zero exponent 
           */

          /* copy mantissa, last bit of exponent */
          dest[0] = source[1];
          dest[2] = source[3];
          dest[3] = source[2];
          if (exp < 254) 
              /* normal value */
              dest[1] = source[0] + (uint8)1;   /* actually adds two to exp */
          else {                              
              /* infinity or NaN */
              if (exp == 254)                      /* unrepresentable - OFL */
                  /* set mant = 0 for overflow */
                  dest[0] = dest[1] = dest[2] = dest[3] = 0;
              dest[0] &= 0x7f;              /* set last bit of exp to 0 */
              dest[1] = 0x80;               /* sign=1 exp=0 -> OFL or NaN */
          }
      }
      else if (source[1] & 0x60) {               /* denormalized value */
          register int shft;
          
          shft = (source[1] & 0x40) ? 1 : 2;  /* shift needed to normalize */
          /* shift mantissa */
          /* note last bit of exp set to 1 implicitly */
          dest[0] = (uint8)(source[1] << shft) 
              | (uint8)(source[2] >> (8-shft));
          dest[3] = (uint8)(source[2] << shft) 
              | (uint8)(source[3] >> (8-shft));
          dest[2] = (uint8)(source[3] << shft);
          dest[1] = (uint8)(source[0] & 0x80);    /* sign */
          if (shft==1) {                          /* set exp to 2 */
            dest[1] |= 0x01;
            dest[0] &= 0x7f;                  /* set LSB of exp to 0 */
          }
      }
      else dest[0] = dest[1] = dest[2] = dest[3] = 0;
      
      source += source_stride;
      dest   += dest_stride;
  }


  return 0;

#ifdef DFKIT
    for (i=0; i<size; i++)
      {
          /* extract exponent */
          exp = (uint8)(in[i].c[0] << 1) | (uint8)(in[i].c[1] >> 7); 
          if (exp) {                                  /* non-zero exponent */
              /* copy mantissa, last bit of exponent */
              out[i].c[0] = in[i].c[1];
              out[i].c[2] = in[i].c[3];
              out[i].c[3] = in[i].c[2];
              if (exp<254)                        /* normal value */
                  out[i].c[1] = in[i].c[0] + (uint8)1;   /* adds two to exp */
              else {                           /* infinity or NaN */
                  if (exp==254)                /* unrepresentable - OFL */
                      out[i].i = 0;            /* set mant=0 for overflow */
                  out[i].c[0] &= 0x7f;         /* set last bit of exp to 0 */
                  out[i].c[1] = 0x80;          /* sign=1 exp=0 -> OFL or NaN */
              }
          }
          else if (in[i].c[1] & 0x60) {        /* denormalized value */
              register int shft;
    
              /* shift needed to normalize */
              shft = (in[i].c[1] & 0x40) ? 1 : 2;  

              /* shift mantissa */
              /* note last bit of exp set to 1 implicitly */
              out[i].c[0] = (uint8)(in[i].c[1] << shft) 
                  | (uint8)(in[i].c[2] >> (8-shft));
              out[i].c[3] = (uint8)(in[i].c[2] << shft) 
                  | (uint8)(in[i].c[3] >> (8-shft));
              out[i].c[2] = (uint8)(in[i].c[3] << shft);
              out[i].c[1] = (uint8)(in[i].c[0] & 0x80);          /* sign */
              if (shft==1) {                        /* set exp to 2 */
                  out[i].c[1] |= 0x01;
                  out[i].c[0] &= 0x7f;              /* set LSB of exp to 0 */
              }
          }
          else out[i].i = 0;                        /* zero */
      }
    return(0);
#endif /* DFKIT */

}

/************************************************************/
/* DFKvo4f()                                                */
/* --> Export routine for 4 byte VAX floats                 */
/************************************************************/
#ifdef PROTOTYPE 
PRIVATE int DFKvo4f(VOIDP s, VOIDP d, uint32 num_elm, uint32 source_stride,
		   uint32 dest_stride)
#else
PRIVATE int DFKvo4f(source, dest, num_elm, source_stride, dest_stride)
uint8 * source, * dest;
uint32 num_elm, source_stride, dest_stride;
#endif /* PROTOTYPE */
{
  int in_place = 0;                     /* Inplace must be detected */
  register uint32 i;            
  uint8 buf[4];                          /* Inplace processing buffer */
#ifdef PROTOTYPE
  uint8 * source = (uint8*)s;
  uint8 * dest = (uint8*)d;
#endif /* PROTOTYPE */
  char *FUNC="DFKvo4f";
  intn  exp;

  HEclear();

  if(source == dest) 
      in_place = TRUE;

  if(num_elm == 0 || in_place){  /* No elements is an error as is in place*/
    HERROR(DFE_BADCONV);
    return FAIL;
  }

  if(source_stride == 0 && dest_stride == 0)
      source_stride = dest_stride = 4;

  for(i = 0; i < num_elm; i++) {
      
      /* extract exponent */
      exp = (source[1] << 1) | (source[0] >> 7); 

      if(!exp && !source[1]) {
          /* 
           * zero value 
           */
          dest[0] = dest[1] = dest[2] = dest[3] = 0;
      }
      else if(exp > 2) {
          /*
           * Normal value
           */

          dest[0] = source[1] - (uint8)1; /* subtracts 2 from exponent */
          /* copy mantissa, LSB of exponent */
          dest[1] = source[0];
          dest[2] = source[3];
          dest[3] = source[2];

      }
      else if(exp) {
          register intn shft;
          /* 
           * denormalized number 
           */

          /* keep sign, zero exponent */
          dest[0] = source[1] & 0x80;

          shft = 3 - exp;

          /* shift original mant by 1 or 2 to get denormalized mant */
          /* prefix mantissa with '1'b or '01'b as appropriate */
          dest[1] = (uint8)((source[0] & 0x7f) >> shft) 
              | (uint8)(0x10 << exp);
          dest[2] = (uint8)(source[0] << (8-shft)) 
              | (uint8)(source[3] >> shft);
          dest[3] = (uint8)(source[3] << (8-shft)) 
              | (uint8)(source[2] >> shft);  
      }
      else {
          /* 
           * sign=1 -> infinity or NaN 
           */
          
          dest[0] = 0xff;                /* set exp to 255 */
          /* copy mantissa */
          dest[1] = source[0] | (uint8)0x80;  /* LSB of exp = 1 */
          dest[2] = source[3];
          dest[3] = source[2];
      }
      
      source += source_stride;
      dest   += dest_stride;
  }
  
  return 0;

#ifdef DFKIT
   uint8 exp;
    int i;

    for (i=0; i<size; i++) {
        /* extract exponent */
        exp = (uint8)(in[i].c[1] << 1) | (uint8)(in[i].c[0] >> 7);  
        if (!exp && !in[i].c[1]) out[i].i = 0;        /* zero value */
        else if (exp>2) {                               /* normal value */
            out[i].c[0] = in[i].c[1] - (uint8)1; /* subtracts 2 from expent */
            /* copy mantissa, LSB of exponent */
            out[i].c[1] = in[i].c[0];
            out[i].c[2] = in[i].c[3];
            out[i].c[3] = in[i].c[2];
        }
        else if (exp) {                          /* denormalized number */
            register int shft;

            /* keep sign, zero exponent */
            out[i].c[0] = in[i].c[1] & (uint8)0x80; 
            shft = 3 - exp;
            /* shift original mant by 1 or 2 to get denormalized mant */
            /* prefix mantissa with '1'b or '01'b as appropriate */
            out[i].c[1] = (uint8)((in[i].c[0] & 0x7f) >> shft) 
                | (uint8)(0x10 << exp);
            out[i].c[2] = (uint8)(in[i].c[0] << (8-shft)) 
                | (uint8)(in[i].c[3] >> shft);
            out[i].c[3] = (uint8)(in[i].c[3] << (8-shft)) 
                | (uint8)(in[i].c[2] >> shft);
        }
        else {                                 /* sign=1 -> infinity or NaN */
            out[i].c[0] = 0xff;                /* set exp to 255 */
                /* copy mantissa */
            out[i].c[1] = in[i].c[0] | (uint8)0x80;  /* LSB of exp = 1 */
            out[i].c[2] = in[i].c[3];
            out[i].c[3] = in[i].c[2];
        }
    }
    return(0);

#endif /* DFKIT */

}

/*
 * Define structures to encode and decode Vax numbers
 * The following code is based on the methods of reading / writing
 *  doubles from Vaxen developed by Sun Microsystems as part of
 *  the XDR library.
 */

/* How an IEEE double looks */
struct  ieee_double {
    unsigned int   mantissa1 : 20;
    unsigned int   exp       : 11;
    unsigned int   sign      : 1;
    unsigned int   mantissa2 : 32;
};

/* How a Vax double looks */
struct  vax_double {
    unsigned int    mantissa1 : 7;
    unsigned int    exp       : 8;
    unsigned int    sign      : 1;
    unsigned int    mantissa2 : 16;
    unsigned int    mantissa3 : 16;
    unsigned int    mantissa4 : 16;
};

#define VAX_DBL_BIAS   0x81
#define IEEE_DBL_BIAS  0x3ff
#define MASK(nbits)    ((1 << nbits) - 1)

struct dbl_limits {
    struct vax_double  vaxx;
    struct ieee_double ieee;
};

static struct dbl_limits dbl_lim[2] = {
    {{ 0x7f,  0xff, 0x0, 0xffff, 0xffff, 0xfff},  /* Max Vax */
     { 0x0,  0x7ff, 0x0, 0x0 }},                  /* Max IEEE */
    {{ 0x0, 0x0, 0x0, 0x0, 0x0, 0x0},             /* Min Vax */
     { 0x0, 0x0, 0x0, 0x0 }}                      /* Min IEEE */
};

/************************************************************/
/* DFKvi8f()                                                */
/* --> Import routine for 8 byte VAX floats                 */
/************************************************************/
#ifdef PROTOTYPE
PRIVATE int DFKvi8f(VOIDP s, VOIDP d, uint32 num_elm, uint32 source_stride,
		   uint32 dest_stride)
#else
PRIVATE int DFKvi8f(source, dest, num_elm, source_stride, dest_stride)
uint8 * source, * dest;
uint32 num_elm, source_stride, dest_stride;
#endif /* PROTOTYPE */
{
  int in_place = 0;                     /* Inplace must be detected */
  register uint32 i;            
  uint8 buf[4];                          /* Inplace processing buffer */
#ifdef PROTOTYPE
  uint8 * source = (uint8*)s;
  uint8 * dest = (uint8*)d;
#endif /* PROTOTYPE */
  char *FUNC="DFKvi8f";
  intn exp;

  struct dbl_limits *lim;
  struct ieee_double id;
  struct vax_double  *vd;
  intn found, j;

  HEclear();

  if(source == dest)
      in_place = TRUE;

  if(num_elm == 0 || in_place){   /* No elements is an error as is in place. */
    HERROR(DFE_BADCONV);
    return FAIL;
  }
  
  if(source_stride == 0 && dest_stride == 0)
      source_stride = dest_stride = 8;

  for(i = 0; i < num_elm; i++) {
shipit:  /* In VAX, bytes in a word are counted from right to left */
      {
         register int j,k;
         uint8 *bufi, *i3e;

         i3e = source;
         bufi = (uint8 *)&(id);
         for (j=0; j<2; j++)     {
             for (k=0; k<4; k++)
                 bufi[k] = i3e[3-k];
             bufi += 4;
             i3e +=4;
         }
      }   /* shipit   */

      vd = (struct vax_double *)  dest;
      
      found = FALSE;
      
      for(j = 0, lim = dbl_lim;
          j < sizeof(dbl_lim)/sizeof(struct dbl_limits);
          j++, lim++) {
          if((id.mantissa2 == lim->ieee.mantissa2) &&
             (id.mantissa1 == lim->ieee.mantissa1) &&
             (id.exp == lim->vaxx.exp)) {
              *vd = lim->vaxx;
              found = TRUE;
              break;
          }
      }

      if(!found) {
          vd->exp = id.exp - IEEE_DBL_BIAS + VAX_DBL_BIAS;
          vd->mantissa1 = id.mantissa1 >> 13;
          vd->mantissa2 = ((id.mantissa1 & MASK(13)) << 3) |
              (id.mantissa2 >> 29);
          vd->mantissa3 = id.mantissa2 >> 13;
          vd->mantissa4 = id.mantissa2 << 3;
      }

      vd->sign = id.sign;

      source += source_stride;
      dest   += dest_stride;
  }

  return 0;
}

/************************************************************/
/* DFKvo8f()                                                */
/* --> Export routine for 8 byte VAX floats                 */
/************************************************************/
#ifdef PROTOTYPE
PRIVATE int DFKvo8f(VOIDP s, VOIDP d, uint32 num_elm, uint32 source_stride,
		   uint32 dest_stride)
#else
PRIVATE int DFKvo8f(source, dest, num_elm, source_stride, dest_stride)
uint8 * source, * dest;
uint32 num_elm, source_stride, dest_stride;
#endif /* PROTOTYPE */
{
  int in_place = 0;                     /* Inplace must be detected */
  register intn i;
  uint8 buf[4];                          /* Inplace processing buffer */
#ifdef PROTOTYPE
  uint8 * source = (uint8*)s;
  uint8 * dest = (uint8*)d;
#endif /* PROTOTYPE */
  char *FUNC="DFKvo8f";
  intn exp;

  struct dbl_limits *lim;
  struct ieee_double id;
  struct vax_double  vd;
  intn found, j;

  HEclear();

  if(source == dest)
      in_place = TRUE;

  if(num_elm == 0 || in_place){   /* No elements is an error as is in place. */
    HERROR(DFE_BADCONV);
    return FAIL;
  }

  if(source_stride == 0 && dest_stride == 0)
      source_stride = dest_stride = 8;

  for(i = 0; i < num_elm; i++) {

      vd = *((struct vax_double *)  source);
      
      found = FALSE;

      for(j = 0, lim=dbl_lim;
          j< sizeof(dbl_lim)/sizeof(struct dbl_limits);
          j++, lim++) {
          if((vd.mantissa4 == lim->vaxx.mantissa4) &&
             (vd.mantissa3 == lim->vaxx.mantissa3) &&
             (vd.mantissa2 == lim->vaxx.mantissa2) &&
             (vd.mantissa1 == lim->vaxx.mantissa1) &&
             (vd.exp == lim->vaxx.exp)) {
              id = lim->ieee;
              found = TRUE;
              break;
          }
      }

      if(!found) {
          id.exp = vd.exp - VAX_DBL_BIAS + IEEE_DBL_BIAS;
          id.mantissa1 = (vd.mantissa1 << 13) | (vd.mantissa2 >> 3);
          id.mantissa2 = ((vd.mantissa2 & MASK(3)) << 29) |
              (vd.mantissa3 << 13) |
                  ((vd.mantissa4>> 3) & MASK(13));
      }
      
      id.sign = vd.sign;

shipit:  /* In VAX the bytes in a word are counted from right to left */
      {
         register int j,k;
         uint8 *i3e, *bufo;

         i3e = (uint8 *)&(id);
         bufo = dest;
         for (j=0;j<2;j++)   {
             for (k=0; k<4; k++)
                 bufo[k]=i3e[3-k];
             bufo += 4;
             i3e +=4;
         }
       }

      source += source_stride;
      dest   += dest_stride;
  }

  return 0;
}

/************************************************************/
/* DFKlvi4f()                                                */
/* --> Import routine for 4 byte VAX floats                 */
/************************************************************/
#ifdef PROTOTYPE
PRIVATE int DFKlvi4f(VOIDP s, VOIDP d, uint32 num_elm, uint32 source_stride,
		   uint32 dest_stride)
#else
PRIVATE int DFKlvi4f(source, dest, num_elm, source_stride, dest_stride)
uint8 * source, * dest;
uint32 num_elm, source_stride, dest_stride;
#endif /* PROTOTYPE */
{
  int in_place = 0;                     /* Inplace must be detected */
  register uint32 i;            
  uint8 buf[4];                          /* Inplace processing buffer */
#ifdef PROTOTYPE
  uint8 * source = (uint8*)s;
  uint8 * dest = (uint8*)d;
#endif /* PROTOTYPE */
  char *FUNC="DFKvi4f";
  uint8 exp;

  HEclear();

  if(source == dest)
      in_place = TRUE;

  if(num_elm == 0 || in_place){   /* No elements is an error as is in place. */
    HERROR(DFE_BADCONV);
    return FAIL;
  }

  if(source_stride == 0 && dest_stride == 0)
      source_stride = dest_stride = 4;

  for(i = 0; i < num_elm; i++) {

      /* extract exponent */
      exp = (source[3] << 1) | (source[2] >> 7);
      if (exp) {       
          /* 
           * non-zero exponent 
           */

          /* copy mantissa, last bit of exponent */
          dest[0] = source[2];
          dest[2] = source[0];
          dest[3] = source[1];
          if (exp < 254) 
              /* normal value */
              dest[1] = source[3] + (uint8)1;   /* actually adds two to exp */
          else {                              
              /* infinity or NaN */
              if (exp == 254)                      /* unrepresentable - OFL */
                  /* set mant = 0 for overflow */
                  dest[0] = dest[1] = dest[2] = dest[3] = 0;
              dest[0] &= 0x7f;              /* set last bit of exp to 0 */
              dest[1] = 0x80;               /* sign=1 exp=0 -> OFL or NaN */
          }
      }
      else if (source[2] & 0x60) {               /* denormalized value */
          register int shft;
          
          shft = (source[2] & 0x40) ? 1 : 2;  /* shift needed to normalize */
          /* shift mantissa */
          /* note last bit of exp set to 1 implicitly */
          dest[0] = (uint8)(source[2] << shft) | (uint8)(source[1] >> (8-shft));
          dest[3] = (uint8)(source[1] << shft) | (uint8)(source[0] >> (8-shft));
          dest[2] = (uint8)(source[0] << shft);
          dest[1] = (uint8)(source[3] & 0x80);    /* sign */
          if (shft==1) {                          /* set exp to 2 */
            dest[1] |= 0x01;
            dest[0] &= 0x7f;                  /* set LSB of exp to 0 */
          }
      }
      else dest[0] = dest[1] = dest[2] = dest[3] = 0;
      
      source += source_stride;
      dest   += dest_stride;
  }


  return 0;

#ifdef DFKIT
    for (i=0; i<size; i++)
      {
          /* extract exponent */
          exp = (uint8)(in[i].c[0] << 1) | (uint8)(in[i].c[1] >> 7); 
          if (exp) {                                  /* non-zero exponent */
              /* copy mantissa, last bit of exponent */
              out[i].c[0] = in[i].c[1];
              out[i].c[2] = in[i].c[3];
              out[i].c[3] = in[i].c[2];
              if (exp<254)                        /* normal value */
                  out[i].c[1] = in[i].c[0] + (uint8)1;   /* adds two to exp */
              else {                           /* infinity or NaN */
                  if (exp==254)                /* unrepresentable - OFL */
                      out[i].i = 0;            /* set mant=0 for overflow */
                  out[i].c[0] &= 0x7f;         /* set last bit of exp to 0 */
                  out[i].c[1] = 0x80;          /* sign=1 exp=0 -> OFL or NaN */
              }
          }
          else if (in[i].c[1] & 0x60) {        /* denormalized value */
              register int shft;
    
              /* shift needed to normalize */
              shft = (in[i].c[1] & 0x40) ? 1 : 2;  

              /* shift mantissa */
              /* note last bit of exp set to 1 implicitly */
              out[i].c[0] = (uint8)(in[i].c[1] << shft) 
                  | (uint8)(in[i].c[2] >> (8-shft));
              out[i].c[3] = (uint8)(in[i].c[2] << shft) 
                  | (uint8)(in[i].c[3] >> (8-shft));
              out[i].c[2] = (uint8)(in[i].c[3] << shft);
              out[i].c[1] = (uint8)(in[i].c[0] & 0x80);          /* sign */
              if (shft==1) {                        /* set exp to 2 */
                  out[i].c[1] |= 0x01;
                  out[i].c[0] &= 0x7f;              /* set LSB of exp to 0 */
              }
          }
          else out[i].i = 0;                        /* zero */
      }
    return(0);
#endif /* DFKIT */

}

/************************************************************/
/* DFKlvo4f()                                                */
/* --> Export routine for 4 byte VAX floats                 */
/************************************************************/
#ifdef PROTOTYPE 
PRIVATE int DFKlvo4f(VOIDP s, VOIDP d, uint32 num_elm, uint32 source_stride,
		   uint32 dest_stride)
#else
PRIVATE int DFKlvo4f(source, dest, num_elm, source_stride, dest_stride)
uint8 * source, * dest;
uint32 num_elm, source_stride, dest_stride;
#endif /* PROTOTYPE */
{
  int in_place = 0;                     /* Inplace must be detected */
  register uint32 i;            
  uint8 buf[4];                          /* Inplace processing buffer */
#ifdef PROTOTYPE
  uint8 * source = (uint8*)s;
  uint8 * dest = (uint8*)d;
#endif /* PROTOTYPE */
  char *FUNC="DFKvo4f";
  intn  exp;

  HEclear();

  if(source == dest) 
      in_place = TRUE;

  if(num_elm == 0 || in_place){  /* No elements is an error as is in place*/
    HERROR(DFE_BADCONV);
    return FAIL;
  }

  if(source_stride == 0 && dest_stride == 0)
      source_stride = dest_stride = 4;

  for(i = 0; i < num_elm; i++) {
      
      /* extract exponent */
      exp = (source[2] << 1) | (source[3] >> 7);

      if(!exp && !source[2]) {
          /* 
           * zero value 
           */
          dest[0] = dest[1] = dest[2] = dest[3] = 0;
      }
      else if(exp > 2) {
          /*
           * Normal value
           */

          dest[0] = source[2] - (uint8)1; /* subtracts 2 from exponent */
          /* copy mantissa, LSB of exponent */
          dest[1] = source[3];
          dest[2] = source[0];
          dest[3] = source[1];

      }
      else if(exp) {
          register intn shft;
          /* 
           * denormalized number 
           */

          /* keep sign, zero exponent */
          dest[0] = source[2] & 0x80;

          shft = 3 - exp;

          /* shift original mant by 1 or 2 to get denormalized mant */
          /* prefix mantissa with '1'b or '01'b as appropriate */
          dest[1] = (uint8)((source[3] & 0x7f) >> shft)
                    | (uint8)(0x10 << exp);
          dest[2] = (uint8)(source[3] << (8-shft))
                    | (uint8)(source[0] >> shft);
          dest[3] = (uint8)(source[0] << (8-shft))
                    | (uint8)(source[1] >> shft);
      }
      else {
          /* 
           * sign=1 -> infinity or NaN 
           */
          
          dest[0] = 0xff;                /* set exp to 255 */
          /* copy mantissa */
          dest[1] = source[3] | (uint8)0x80;  /* LSB of exp = 1 */
          dest[2] = source[0];
          dest[3] = source[1];
      }
      
      source += source_stride;
      dest   += dest_stride;
  }
  
  return 0;

#ifdef DFKIT
   uint8 exp;
    int i;

    for (i=0; i<size; i++) {
        /* extract exponent */
        exp = (uint8)(in[i].c[1] << 1) | (uint8)(in[i].c[0] >> 7);  
        if (!exp && !in[i].c[1]) out[i].i = 0;        /* zero value */
        else if (exp>2) {                               /* normal value */
            out[i].c[0] = in[i].c[1] - (uint8)1; /* subtracts 2 from expent */
            /* copy mantissa, LSB of exponent */
            out[i].c[1] = in[i].c[0];
            out[i].c[2] = in[i].c[3];
            out[i].c[3] = in[i].c[2];
        }
        else if (exp) {                          /* denormalized number */
            register int shft;

            /* keep sign, zero exponent */
            out[i].c[0] = in[i].c[1] & (uint8)0x80; 
            shft = 3 - exp;
            /* shift original mant by 1 or 2 to get denormalized mant */
            /* prefix mantissa with '1'b or '01'b as appropriate */
            out[i].c[1] = (uint8)((in[i].c[0] & 0x7f) >> shft) 
                | (uint8)(0x10 << exp);
            out[i].c[2] = (uint8)(in[i].c[0] << (8-shft)) 
                | (uint8)(in[i].c[3] >> shft);
            out[i].c[3] = (uint8)(in[i].c[3] << (8-shft)) 
                | (uint8)(in[i].c[2] >> shft);
        }
        else {                                 /* sign=1 -> infinity or NaN */
            out[i].c[0] = 0xff;                /* set exp to 255 */
                /* copy mantissa */
            out[i].c[1] = in[i].c[0] | (uint8)0x80;  /* LSB of exp = 1 */
            out[i].c[2] = in[i].c[3];
            out[i].c[3] = in[i].c[2];
        }
    }
    return(0);

#endif /* DFKIT */

}

/*
 * Define structures to encode and decode Vax numbers
 * The following code is based on the methods of reading / writing
 *  doubles from Vaxen developed by Sun Microsystems as part of
 *  the XDR library.
 */

/* How an IEEE double looks */
struct  ieee_double {
    unsigned int   mantissa1 : 20;
    unsigned int   exp       : 11;
    unsigned int   sign      : 1;
    unsigned int   mantissa2 : 32;
};

/* How a Vax double looks */
struct  vax_double {
    unsigned int    mantissa1 : 7;
    unsigned int    exp       : 8;
    unsigned int    sign      : 1;
    unsigned int    mantissa2 : 16;
    unsigned int    mantissa3 : 16;
    unsigned int    mantissa4 : 16;
};

#define VAX_DBL_BIAS   0x81
#define IEEE_DBL_BIAS  0x3ff
#define MASK(nbits)    ((1 << nbits) - 1)

struct dbl_limits {
    struct vax_double  vaxx;
    struct ieee_double ieee;
};

static struct dbl_limits dbl_lim[2] = {
    {{ 0x7f,  0xff, 0x0, 0xffff, 0xffff, 0xfff},  /* Max Vax */
     { 0x0,  0x7ff, 0x0, 0x0 }},                  /* Max IEEE */
    {{ 0x0, 0x0, 0x0, 0x0, 0x0, 0x0},             /* Min Vax */
     { 0x0, 0x0, 0x0, 0x0 }}                      /* Min IEEE */
};

/************************************************************/
/* DFKlvi8f()                                                */
/* --> Import routine for 8 byte VAX floats                 */
/************************************************************/
#ifdef PROTOTYPE
PRIVATE int DFKlvi8f(VOIDP s, VOIDP d, uint32 num_elm, uint32 source_stride,
		   uint32 dest_stride)
#else
PRIVATE int DFKlvi8f(source, dest, num_elm, source_stride, dest_stride)
uint8 * source, * dest;
uint32 num_elm, source_stride, dest_stride;
#endif /* PROTOTYPE */
{
  int in_place = 0;                     /* Inplace must be detected */
  register uint32 i;            
  uint8 buf[4];                          /* Inplace processing buffer */
#ifdef PROTOTYPE
  uint8 * source = (uint8*)s;
  uint8 * dest = (uint8*)d;
#endif /* PROTOTYPE */
  char *FUNC="DFKvi8f";
  intn exp;

  struct dbl_limits *lim;
  struct ieee_double id;
  struct vax_double  *vd;
  intn found, j;

  HEclear();

  if(source == dest)
      in_place = TRUE;

  if(num_elm == 0 || in_place){   /* No elements is an error as is in place. */
    HERROR(DFE_BADCONV);
    return FAIL;
  }
  
    if(source_stride == 0 && dest_stride == 0)
        source_stride = dest_stride = 8;

    for(i = 0; i < num_elm; i++) {
        HDmemcpy(&(id),&source[4],4);   /* swap the two 4-byte words */
        HDmemcpy(((uint8 *)&(id))+4,source,4);

        vd = (struct vax_double *)  dest;
      
        found = FALSE;
      
        for(j = 0, lim = dbl_lim;
                j < sizeof(dbl_lim)/sizeof(struct dbl_limits); j++, lim++) {
            if((id.mantissa2 == lim->ieee.mantissa2) &&
                    (id.mantissa1 == lim->ieee.mantissa1) &&
                    (id.exp == lim->vaxx.exp)) {
                *vd = lim->vaxx;
                found = TRUE;
                break;
            }
        }

        if(!found) {
            vd->exp = id.exp - IEEE_DBL_BIAS + VAX_DBL_BIAS;
            vd->mantissa1 = id.mantissa1 >> 13;
            vd->mantissa2 = ((id.mantissa1 & MASK(13)) << 3) |
                    (id.mantissa2 >> 29);
            vd->mantissa3 = id.mantissa2 >> 13;
            vd->mantissa4 = id.mantissa2 << 3;
        }

        vd->sign = id.sign;

        source += source_stride;
        dest   += dest_stride;
    }

  return 0;
}

/************************************************************/
/* DFKlvo8f()                                                */
/* --> Export routine for 8 byte VAX floats                 */
/************************************************************/
#ifdef PROTOTYPE
PRIVATE int DFKlvo8f(VOIDP s, VOIDP d, uint32 num_elm, uint32 source_stride,
		   uint32 dest_stride)
#else
PRIVATE int DFKlvo8f(source, dest, num_elm, source_stride, dest_stride)
uint8 * source, * dest;
uint32 num_elm, source_stride, dest_stride;
#endif /* PROTOTYPE */
{
  int in_place = 0;                     /* Inplace must be detected */
  register intn i;
  uint8 buf[4];                          /* Inplace processing buffer */
#ifdef PROTOTYPE
  uint8 * source = (uint8*)s;
  uint8 * dest = (uint8*)d;
#endif /* PROTOTYPE */
  char *FUNC="DFKvo8f";
  intn exp;

  struct dbl_limits *lim;
  struct ieee_double id;
  struct vax_double  vd;
  intn found, j;

    HEclear();

    if(source == dest)
        in_place = TRUE;

    if(num_elm == 0 || in_place) { /* No elements is an error as is in place. */
        HERROR(DFE_BADCONV);
        return FAIL;
    }

    if(source_stride == 0 && dest_stride == 0)
        source_stride = dest_stride = 8;

    for(i = 0; i < num_elm; i++) {

        vd = *((struct vax_double *)  source);
      
        found = FALSE;

        for(j = 0, lim=dbl_lim;
                j< sizeof(dbl_lim)/sizeof(struct dbl_limits);
                j++, lim++) {
            if((vd.mantissa4 == lim->vaxx.mantissa4) &&
                    (vd.mantissa3 == lim->vaxx.mantissa3) &&
                    (vd.mantissa2 == lim->vaxx.mantissa2) &&
                    (vd.mantissa1 == lim->vaxx.mantissa1) &&
                    (vd.exp == lim->vaxx.exp)) {
                id = lim->ieee;
                found = TRUE;
                break;
            }
        }

        if(!found) {
            id.exp = vd.exp - VAX_DBL_BIAS + IEEE_DBL_BIAS;
            id.mantissa1 = (vd.mantissa1 << 13) | (vd.mantissa2 >> 3);
            id.mantissa2 = ((vd.mantissa2 & MASK(3)) << 29) |
                    (vd.mantissa3 << 13) |
                    ((vd.mantissa4>> 3) & MASK(13));
        }
      
        id.sign = vd.sign;

        HDmemcpy(dest,((uint8 *)&(id))+4,4);   /* swap the two 4-byte words */
        HDmemcpy(&dest[4],&(id),4);

        source += source_stride;
        dest   += dest_stride;
    }

    return 0;
}
#endif /* VMS */

#ifdef CONVEXNATIVE

/****************************************************************
 * NUMBER CONVERSION ROUTINES FOR CONVEX ARCHITECTURES          *
 * Most of the CONVEX number types can be handled with the      *
 * generic routines.  Only the floating point                   *
 * conversion routines need to be customized.                   *
 * Routines written by J.W. de Bruijn,                          *
 * DELPHI project, Laboratory of Seismics and Acoustics,        *
 * Delft University of Technology.                              *
 * Based on the VAX <-> IEEE routines from NCSA.                *
 ****************************************************************/

/************************************************************/
/* DFKci4f()                                                */
/* --> Import routine for 4 byte CONVEX floats              */
/************************************************************/
#ifdef PROTOTYPE
PRIVATE int DFKci4f(VOIDP s, VOIDP d, uint32 num_elm, uint32 source_stride,
		   uint32 dest_stride)
#else
PRIVATE int DFKci4f(source, dest, num_elm, source_stride, dest_stride)
uint8 * source, * dest;
uint32 num_elm, source_stride, dest_stride;
#endif /* PROTOTYPE */
{
  int in_place = 0;                     /* Inplace must be detected */
  register uint32 i;            
  uint8 buf[4];                          /* Inplace processing buffer */
#ifdef PROTOTYPE
  uint8 * source = (uint8*)s;
  uint8 * dest = (uint8*)d;
#endif /* PROTOTYPE */
  char *FUNC="DFKci4f";
  uint8 exp;

  HEclear();

  if(source == dest)
      in_place = TRUE;

  if(num_elm == 0 || in_place){   /* No elements is an error as is in place. */
    HERROR(DFE_BADCONV);
    return FAIL;
  }

  if(source_stride == 0 && dest_stride == 0)
      source_stride = dest_stride = 4;

  for(i = 0; i < num_elm; i++) {

      /* extract exponent */
      exp = (uint8)(source[0] << 1) | (uint8)(source[1] >> 7); 
      if (exp) {       
          /* 
           * non-zero exponent 
           */

          /* copy mantissa, last bit of exponent */
          dest[1] = source[1];
          dest[3] = source[3];
          dest[2] = source[2];
          if (exp < 254) 
              /* normal value */
              dest[0] = source[0] + (uint8)1;   /* actually adds two to exp */
          else {                              
              /* infinity or NaN */
              if (exp == 254)                      /* unrepresentable - OFL */
                  /* set mant = 0 for overflow */
                  dest[0] = dest[1] = dest[2] = dest[3] = 0;
              dest[1] &= 0x7f;              /* set last bit of exp to 0 */
              dest[0] = 0x80;               /* sign=1 exp=0 -> OFL or NaN */
          }
      }
      else if (source[1] & 0x60) {               /* denormalized value */
          register int shft;
          
          shft = (source[1] & 0x40) ? 1 : 2;  /* shift needed to normalize */
          /* shift mantissa */
          /* note last bit of exp set to 1 implicitly */
          dest[1] = (uint8)(source[1] << shft) | (uint8)(source[2] >> (8-shft));
          dest[2] = (uint8)(source[2] << shft) | (uint8)(source[3] >> (8-shft));
          dest[3] = (uint8)(source[3] << shft);
          dest[0] = (uint8)(source[0] & 0x80);    /* sign */
          if (shft==1) {                          /* set exp to 2 */
            dest[0] |= 0x01;
            dest[1] &= 0x7f;                  /* set LSB of exp to 0 */
          }
      }
      else dest[0] = dest[1] = dest[2] = dest[3] = 0;
      
      source += source_stride;
      dest   += dest_stride;
  }

  return 0;

}

/************************************************************/
/* DFKco4f()                                                */
/* --> Export routine for 4 byte CONVEX floats              */
/************************************************************/
#ifdef PROTOTYPE 
PRIVATE int DFKco4f(VOIDP s, VOIDP d, uint32 num_elm, uint32 source_stride,
		   uint32 dest_stride)
#else
PRIVATE int DFKco4f(source, dest, num_elm, source_stride, dest_stride)
uint8 * source, * dest;
uint32 num_elm, source_stride, dest_stride;
#endif /* PROTOTYPE */
{
  int in_place = 0;                     /* Inplace must be detected */
  register uint32 i;            
  uint8 buf[4];                          /* Inplace processing buffer */
#ifdef PROTOTYPE
  uint8 * source = (uint8*)s;
  uint8 * dest = (uint8*)d;
#endif /* PROTOTYPE */
  char *FUNC="DFKco4f";
  intn  exp;

  HEclear();

  if(source == dest) 
      in_place = TRUE;

  if(num_elm == 0 || in_place){  /* No elements is an error as is in place*/
    HERROR(DFE_BADCONV);
    return FAIL;
  }

  if(source_stride == 0 && dest_stride == 0)
      source_stride = dest_stride = 4;

  for(i = 0; i < num_elm; i++) {
      
      /* extract exponent */
      exp = (source[0] << 1) | (source[1] >> 7); 

      if(!exp && !source[0]) {
          /* 
           * zero value 
           */
          dest[0] = dest[1] = dest[2] = dest[3] = 0;
      }
      else if(exp > 2) {
          /*
           * Normal value
           */

          dest[0] = source[0] - (uint8)1; /* subtracts 2 from exponent */
          /* copy mantissa, LSB of exponent */
          dest[1] = source[1];
          dest[2] = source[2];
          dest[3] = source[3];

      }
      else if(exp) {
          register intn shft;
          /* 
           * denormalized number 
           */

          /* keep sign, zero exponent */
          dest[0] = source[0] & 0x80;

          shft = 3 - exp;

          /* shift original mant by 1 or 2 to get denormalized mant */
          /* prefix mantissa with '1'b or '01'b as appropriate */
          dest[1] = (uint8)((source[1] & 0x7f) >> shft) | (uint8)(0x10 << exp);
          dest[2] = (uint8)(source[1] << (8-shft)) | (uint8)(source[2] >> shft);
          dest[3] = (uint8)(source[2] << (8-shft)) | (uint8)(source[3] >> shft);  
      }
      else {
          /* 
           * sign=1 -> infinity or NaN 
           */
          
          dest[0] = 0xff;                /* set exp to 255 */
          /* copy mantissa */
          dest[1] = source[1] | (uint8)0x80;  /* LSB of exp = 1 */
          dest[2] = source[2];
          dest[3] = source[3];
      }
      
      source += source_stride;
      dest   += dest_stride;
  }
  
  return 0;

}

/************************************************************/
/* DFKci8f()                                                */
/* --> Import routine for 8 byte CONVEX floats              */
/************************************************************/
#ifdef PROTOTYPE
PRIVATE int DFKci8f(VOIDP s, VOIDP d, uint32 num_elm, uint32 source_stride,
		   uint32 dest_stride)
#else
PRIVATE int DFKci8f(source, dest, num_elm, source_stride, dest_stride)
uint8 * source, * dest;
uint32 num_elm, source_stride, dest_stride;
#endif /* PROTOTYPE */
{
  int in_place = 0;                     /* Inplace must be detected */
  register uint32 i;            
  uint8 buf[8];                          /* Inplace processing buffer */
#ifdef PROTOTYPE
  uint8 * source = (uint8*)s;
  uint8 * dest = (uint8*)d;
#endif /* PROTOTYPE */
  char *FUNC="DFKci8f";
  intn exp;

  HEclear();

  if(source == dest)
      in_place = TRUE;

  if(num_elm == 0 || in_place){   /* No elements is an error as is in place. */
    HERROR(DFE_BADCONV);
    return FAIL;
  }

  if(source_stride == 0 && dest_stride == 0)
      source_stride = dest_stride = 8;

  for(i = 0; i < num_elm; i++) {

      /* extract exponent */
      exp = (source[0] << 1) | (source[1] >> 4); 
      if (exp) {       
          /* 
           * non-zero exponent 
           */

          /* copy mantissa, sign and first bits of exponent */
          dest[2] = source[2];
          dest[3] = source[3];
          dest[4] = source[4];
          dest[5] = source[5];
          dest[6] = source[6];
          dest[7] = source[7];
          dest[0] = source[0];
          if (exp < 2046) {
              /* normal value */
	      dest[1] = source[1] + 0x20;	/* add two to exp */
	      if (dest[1] < 0x20) dest[0] += 1;	/* carry */
	  }
          else {                              
              /* infinity or NaN */
              if (exp == 2046)                      /* unrepresentable - OFL */
                  /* set mant = 0 for overflow */
                  dest[0] = dest[1] = dest[2] = dest[3] = 0;
              dest[0] = 0x80;               /* sign=1 exp=0 -> OFL or NaN */
              dest[1] &= 0x0f;              /* set last bit of exp to 0 */
          }
      }
      else if (source[1] & 0x0C) {               /* denormalized value */
          register int shft;
          
          shft = (source[1] & 0x08) ? 1 : 2;  /* shift needed to normalize */
          /* shift mantissa */
          /* note last bit of exp set to 1 implicitly */
          dest[1] = (uint8)(source[1] << shft) | (uint8)(source[2] >> (8-shft));
          dest[2] = (uint8)(source[2] << shft) | (uint8)(source[3] >> (8-shft));
          dest[3] = (uint8)(source[3] << shft) | (uint8)(source[4] >> (8-shft));
          dest[4] = (uint8)(source[4] << shft) | (uint8)(source[5] >> (8-shft));
          dest[5] = (uint8)(source[5] << shft) | (uint8)(source[6] >> (8-shft));
          dest[6] = (uint8)(source[6] << shft) | (uint8)(source[7] >> (8-shft));
          dest[7] = (uint8)(source[7] << shft);
          dest[0] = (uint8)(source[0] & 0x80);    /* sign */
          if (shft==1) {                          /* set exp to 2 */
            dest[1] |= 0x20;                  /* set LSB of exp to 0 */
          }
      }
      else {
      	dest[0] = dest[1] = dest[2] = dest[3] = 0;
      	dest[4] = dest[5] = dest[6] = dest[7] = 0;
      }
      
      source += source_stride;
      dest   += dest_stride;
  }


  return 0;

}

/************************************************************/
/* DFKco8f()                                                */
/* --> Export routine for 8 byte CONVEX floats              */
/************************************************************/
#ifdef PROTOTYPE 
PRIVATE int DFKco8f(VOIDP s, VOIDP d, uint32 num_elm, uint32 source_stride,
		   uint32 dest_stride)
#else
PRIVATE int DFKco8f(source, dest, num_elm, source_stride, dest_stride)
uint8 * source, * dest;
uint32 num_elm, source_stride, dest_stride;
#endif /* PROTOTYPE */
{
  int in_place = 0;                     /* Inplace must be detected */
  register uint32 i;            
  uint8 buf[8];                          /* Inplace processing buffer */
#ifdef PROTOTYPE
  uint8 * source = (uint8*)s;
  uint8 * dest = (uint8*)d;
#endif /* PROTOTYPE */
  char *FUNC="DFKco8f";
  intn  exp;

  HEclear();

  if(source == dest) 
      in_place = TRUE;

  if(num_elm == 0 || in_place){  /* No elements is an error as is in place*/
    HERROR(DFE_BADCONV);
    return FAIL;
  }

  if(source_stride == 0 && dest_stride == 0)
      source_stride = dest_stride = 8;

  for(i = 0; i < num_elm; i++) {
      
      /* extract exponent */
      exp = (source[0] << 1) | (source[1] >> 4); 

      if(!exp && !source[0]) {
          /* 
           * zero value 
           */
          dest[0] = dest[1] = dest[2] = dest[3] = 0;
          dest[4] = dest[5] = dest[6] = dest[7] = 0;
      }
      else if(exp > 2) {
          /*
           * Normal value
           */
          /* copy sign, MSBs of exponent */
          dest[0] = source[0];
	  dest[1] = source[1] - 0x20;	/* subtracts 2 from exponent */
          /* copy mantissa */
          dest[2] = source[2];
          dest[3] = source[3];
          dest[4] = source[4];
          dest[5] = source[5];
          dest[6] = source[6];
          dest[7] = source[7];
      }
      else if(exp) {
          register intn shft;
          /* 
           * denormalized number 
           */

          /* keep sign, zero exponent */
          dest[0] = source[0] & 0x80;

          shft = 3 - exp;

          /* shift original mant by 1 or 2 to get denormalized mant */
          /* prefix mantissa with '1'b or '01'b as appropriate */
          dest[1] = (uint8)((source[1] & 0x0f) >> shft) | (uint8)(0x02 << exp);
          dest[2] = (uint8)(source[1] << (8-shft)) | (uint8)(source[2] >> shft);
          dest[3] = (uint8)(source[2] << (8-shft)) | (uint8)(source[3] >> shft);  
          dest[4] = (uint8)(source[3] << (8-shft)) | (uint8)(source[4] >> shft);  
          dest[5] = (uint8)(source[4] << (8-shft)) | (uint8)(source[5] >> shft);  
          dest[6] = (uint8)(source[5] << (8-shft)) | (uint8)(source[6] >> shft);  
          dest[7] = (uint8)(source[6] << (8-shft)) | (uint8)(source[7] >> shft);  
      }
      else {
          /* 
           * sign=1 -> infinity or NaN 
           */
          
          dest[0] = 0xff;                /* set exp to 255 */
          /* copy mantissa */
          dest[1] = source[1] | (uint8)0xF0;  /* LSBs of exp = 1 */
          dest[2] = source[2];
          dest[3] = source[3];
          dest[4] = source[4];
          dest[5] = source[5];
          dest[6] = source[6];
          dest[7] = source[7];
      }
      
      source += source_stride;
      dest   += dest_stride;
  }
  
  return 0;

}

/************************************************************/
/* DFKlci4f()                                                */
/* --> Import routine for 4 byte CONVEX floats              */
/************************************************************/
#ifdef PROTOTYPE
PRIVATE int DFKlci4f(VOIDP s, VOIDP d, uint32 num_elm, uint32 source_stride,
		   uint32 dest_stride)
#else
PRIVATE int DFKlci4f(source, dest, num_elm, source_stride, dest_stride)
uint8 * source, * dest;
uint32 num_elm, source_stride, dest_stride;
#endif /* PROTOTYPE */
{
  int in_place = 0;                     /* Inplace must be detected */
  register uint32 i;            
  uint8 buf[4];                          /* Inplace processing buffer */
#ifdef PROTOTYPE
  uint8 * source = (uint8*)s;
  uint8 * dest = (uint8*)d;
#endif /* PROTOTYPE */
  char *FUNC="DFKci4f";
  uint8 exp;

  HEclear();

  if(source == dest)
      in_place = TRUE;

  if(num_elm == 0 || in_place){   /* No elements is an error as is in place. */
    HERROR(DFE_BADCONV);
    return FAIL;
  }

  if(source_stride == 0 && dest_stride == 0)
      source_stride = dest_stride = 4;

  for(i = 0; i < num_elm; i++) {

      /* extract exponent */
      exp = (uint8)(source[4] << 1) | (uint8)(source[3] >> 7);
      if (exp) {       
          /* 
           * non-zero exponent 
           */

          /* copy mantissa, last bit of exponent */
          dest[1] = source[2];
          dest[3] = source[0];
          dest[2] = source[1];
          if (exp < 254) 
              /* normal value */
              dest[0] = source[3] + (uint8)1;   /* actually adds two to exp */
          else {                              
              /* infinity or NaN */
              if (exp == 254)                      /* unrepresentable - OFL */
                  /* set mant = 0 for overflow */
                  dest[0] = dest[1] = dest[2] = dest[3] = 0;
              dest[1] &= 0x7f;              /* set last bit of exp to 0 */
              dest[0] = 0x80;               /* sign=1 exp=0 -> OFL or NaN */
          }
      }
      else if (source[2] & 0x60) {               /* denormalized value */
          register int shft;
          
          shft = (source[2] & 0x40) ? 1 : 2;  /* shift needed to normalize */
          /* shift mantissa */
          /* note last bit of exp set to 1 implicitly */
          dest[1] = (uint8)(source[2] << shft) | (uint8)(source[1] >> (8-shft));
          dest[2] = (uint8)(source[1] << shft) | (uint8)(source[0] >> (8-shft));
          dest[3] = (uint8)(source[0] << shft);
          dest[0] = (uint8)(source[3] & 0x80);    /* sign */
          if (shft==1) {                          /* set exp to 2 */
            dest[0] |= 0x01;
            dest[1] &= 0x7f;                  /* set LSB of exp to 0 */
          }
      }
      else dest[0] = dest[1] = dest[2] = dest[3] = 0;
      
      source += source_stride;
      dest   += dest_stride;
  }

  return 0;

}

/************************************************************/
/* DFKlco4f()                                                */
/* --> Export routine for 4 byte CONVEX floats              */
/************************************************************/
#ifdef PROTOTYPE 
PRIVATE int DFKlco4f(VOIDP s, VOIDP d, uint32 num_elm, uint32 source_stride,
		   uint32 dest_stride)
#else
PRIVATE int DFKlco4f(source, dest, num_elm, source_stride, dest_stride)
uint8 * source, * dest;
uint32 num_elm, source_stride, dest_stride;
#endif /* PROTOTYPE */
{
  int in_place = 0;                     /* Inplace must be detected */
  register uint32 i;            
  uint8 buf[4];                          /* Inplace processing buffer */
#ifdef PROTOTYPE
  uint8 * source = (uint8*)s;
  uint8 * dest = (uint8*)d;
#endif /* PROTOTYPE */
  char *FUNC="DFKco4f";
  intn  exp;

  HEclear();

  if(source == dest) 
      in_place = TRUE;

  if(num_elm == 0 || in_place){  /* No elements is an error as is in place*/
    HERROR(DFE_BADCONV);
    return FAIL;
  }

  if(source_stride == 0 && dest_stride == 0)
      source_stride = dest_stride = 4;

  for(i = 0; i < num_elm; i++) {
      
      /* extract exponent */
      exp = (source[3] << 1) | (source[2] >> 7);

      if(!exp && !source[3]) {
          /* 
           * zero value 
           */
          dest[0] = dest[1] = dest[2] = dest[3] = 0;
      }
      else if(exp > 2) {
          /*
           * Normal value
           */

          dest[0] = source[3] - (uint8)1; /* subtracts 2 from exponent */
          /* copy mantissa, LSB of exponent */
          dest[1] = source[2];
          dest[2] = source[1];
          dest[3] = source[0];

      }
      else if(exp) {
          register intn shft;
          /* 
           * denormalized number 
           */

          /* keep sign, zero exponent */
          dest[0] = source[3] & 0x80;

          shft = 3 - exp;

          /* shift original mant by 1 or 2 to get denormalized mant */
          /* prefix mantissa with '1'b or '01'b as appropriate */
          dest[1] = (uint8)((source[2] & 0x7f) >> shft) | (uint8)(0x10 << exp);
          dest[2] = (uint8)(source[2] << (8-shft)) | (uint8)(source[1] >> shft);
          dest[3] = (uint8)(source[1] << (8-shft)) | (uint8)(source[0] >> shft);
      }
      else {
          /* 
           * sign=1 -> infinity or NaN 
           */
          
          dest[0] = 0xff;                /* set exp to 255 */
          /* copy mantissa */
          dest[1] = source[2] | (uint8)0x80;  /* LSB of exp = 1 */
          dest[2] = source[1];
          dest[3] = source[0];
      }
      
      source += source_stride;
      dest   += dest_stride;
  }
  
  return 0;

}

/************************************************************/
/* DFKlci8f()                                                */
/* --> Import routine for 8 byte CONVEX floats              */
/************************************************************/
#ifdef PROTOTYPE
PRIVATE int DFKlci8f(VOIDP s, VOIDP d, uint32 num_elm, uint32 source_stride,
		   uint32 dest_stride)
#else
PRIVATE int DFKlci8f(source, dest, num_elm, source_stride, dest_stride)
uint8 * source, * dest;
uint32 num_elm, source_stride, dest_stride;
#endif /* PROTOTYPE */
{
  int in_place = 0;                     /* Inplace must be detected */
  register uint32 i;            
  uint8 buf[8];                          /* Inplace processing buffer */
#ifdef PROTOTYPE
  uint8 * source = (uint8*)s;
  uint8 * dest = (uint8*)d;
#endif /* PROTOTYPE */
  char *FUNC="DFKci8f";
  intn exp;

  HEclear();

  if(source == dest)
      in_place = TRUE;

  if(num_elm == 0 || in_place){   /* No elements is an error as is in place. */
    HERROR(DFE_BADCONV);
    return FAIL;
  }

  if(source_stride == 0 && dest_stride == 0)
      source_stride = dest_stride = 8;

  for(i = 0; i < num_elm; i++) {

      /* extract exponent */
      exp = (source[7] << 1) | (source[6] >> 4);
      if (exp) {       
          /* 
           * non-zero exponent 
           */

          /* copy mantissa, sign and first bits of exponent */
          dest[2] = source[5];
          dest[3] = source[4];
          dest[4] = source[3];
          dest[5] = source[2];
          dest[6] = source[1];
          dest[7] = source[0];
          dest[0] = source[7];
          if (exp < 2046) {
              /* normal value */
          dest[1] = source[6] + 0x20;   /* add two to exp */
	      if (dest[1] < 0x20) dest[0] += 1;	/* carry */
	  }
          else {                              
              /* infinity or NaN */
              if (exp == 2046)                      /* unrepresentable - OFL */
                  /* set mant = 0 for overflow */
                  dest[0] = dest[1] = dest[2] = dest[3] = 0;
              dest[0] = 0x80;               /* sign=1 exp=0 -> OFL or NaN */
              dest[1] &= 0x0f;              /* set last bit of exp to 0 */
          }
      }
      else if (source[6] & 0x0C) {               /* denormalized value */
          register int shft;
          
          shft = (source[6] & 0x08) ? 1 : 2;  /* shift needed to normalize */
          /* shift mantissa */
          /* note last bit of exp set to 1 implicitly */
          dest[1] = (uint8)(source[6] << shft) | (uint8)(source[5] >> (8-shft));
          dest[2] = (uint8)(source[5] << shft) | (uint8)(source[4] >> (8-shft));
          dest[3] = (uint8)(source[4] << shft) | (uint8)(source[3] >> (8-shft));
          dest[4] = (uint8)(source[3] << shft) | (uint8)(source[2] >> (8-shft));
          dest[5] = (uint8)(source[2] << shft) | (uint8)(source[1] >> (8-shft));
          dest[6] = (uint8)(source[1] << shft) | (uint8)(source[0] >> (8-shft));
          dest[7] = (uint8)(source[0] << shft);
          dest[0] = (uint8)(source[7] & 0x80);    /* sign */
          if (shft==1) {                          /* set exp to 2 */
            dest[1] |= 0x20;                  /* set LSB of exp to 0 */
          }
      }
      else {
      	dest[0] = dest[1] = dest[2] = dest[3] = 0;
      	dest[4] = dest[5] = dest[6] = dest[7] = 0;
      }
      
      source += source_stride;
      dest   += dest_stride;
  }


  return 0;

}

/************************************************************/
/* DFKlco8f()                                                */
/* --> Export routine for 8 byte CONVEX floats              */
/************************************************************/
#ifdef PROTOTYPE 
PRIVATE int DFKlco8f(VOIDP s, VOIDP d, uint32 num_elm, uint32 source_stride,
		   uint32 dest_stride)
#else
PRIVATE int DFKlco8f(source, dest, num_elm, source_stride, dest_stride)
uint8 * source, * dest;
uint32 num_elm, source_stride, dest_stride;
#endif /* PROTOTYPE */
{
  int in_place = 0;                     /* Inplace must be detected */
  register uint32 i;            
  uint8 buf[8];                          /* Inplace processing buffer */
#ifdef PROTOTYPE
  uint8 * source = (uint8*)s;
  uint8 * dest = (uint8*)d;
#endif /* PROTOTYPE */
  char *FUNC="DFKco8f";
  intn  exp;

  HEclear();

  if(source == dest) 
      in_place = TRUE;

  if(num_elm == 0 || in_place){  /* No elements is an error as is in place*/
    HERROR(DFE_BADCONV);
    return FAIL;
  }

  if(source_stride == 0 && dest_stride == 0)
      source_stride = dest_stride = 8;

  for(i = 0; i < num_elm; i++) {
      
      /* extract exponent */
      exp = (source[7] << 1) | (source[6] >> 4);

      if(!exp && !source[7]) {
          /* 
           * zero value 
           */
          dest[0] = dest[1] = dest[2] = dest[3] = 0;
          dest[4] = dest[5] = dest[6] = dest[7] = 0;
      }
      else if(exp > 2) {
          /*
           * Normal value
           */
          /* copy sign, MSBs of exponent */
          dest[0] = source[7];
          dest[1] = source[6] - 0x20;   /* subtracts 2 from exponent */
          /* copy mantissa */
          dest[2] = source[5];
          dest[3] = source[4];
          dest[4] = source[3];
          dest[5] = source[2];
          dest[6] = source[1];
          dest[7] = source[0];
      }
      else if(exp) {
          register intn shft;
          /* 
           * denormalized number 
           */

          /* keep sign, zero exponent */
          dest[0] = source[7] & 0x80;

          shft = 3 - exp;

          /* shift original mant by 1 or 2 to get denormalized mant */
          /* prefix mantissa with '1'b or '01'b as appropriate */
          dest[1] = (uint8)((source[6] & 0x0f) >> shft) | (uint8)(0x02 << exp);
          dest[2] = (uint8)(source[6] << (8-shft)) | (uint8)(source[5] >> shft);
          dest[3] = (uint8)(source[5] << (8-shft)) | (uint8)(source[4] >> shft);
          dest[4] = (uint8)(source[4] << (8-shft)) | (uint8)(source[3] >> shft);
          dest[5] = (uint8)(source[3] << (8-shft)) | (uint8)(source[2] >> shft);
          dest[6] = (uint8)(source[2] << (8-shft)) | (uint8)(source[1] >> shft);
          dest[7] = (uint8)(source[1] << (8-shft)) | (uint8)(source[0] >> shft);
      }
      else {
          /* 
           * sign=1 -> infinity or NaN 
           */
          
          dest[0] = 0xff;                /* set exp to 255 */
          /* copy mantissa */
          dest[1] = source[6] | (uint8)0xF0;  /* LSBs of exp = 1 */
          dest[2] = source[5];
          dest[3] = source[4];
          dest[4] = source[3];
          dest[5] = source[2];
          dest[6] = source[1];
          dest[7] = source[0];
      }
      
      source += source_stride;
      dest   += dest_stride;
  }
  
  return 0;

}
#endif  /* CONVEXNATIVE */

#if defined(VP)

/* 
 * NUMBER CONVERSION ROUTINES FOR Fujitsu VP series         
 * (IBM floating point, BIG-ENDIAN integer types, ASCII     
 *  chars...)                                               
 *                                                          
 * Drew Whitehouse, 
 * Australian National University Supercomputer Facility. 
 * Dec 92.
 *
 * Routines used were adapted from routines provided for     
 * anon. ftp by the Cornell National Supercomputer        
 * Facility. The following copyright was attached.....      
 *                                                          
 */

/*---------------------------------------------------------------------*
 * Val I. Garger, Technology Integration                               *
 * Group, CNSF, Cornell University                                     *
 * vig@eagle.cnsf.cornell.edu                                          *
 *-------------------------------------------------------------------- *        
 *                                                                     *        
 *  COPYRIGHT -  VAL GARGER, CORNELL NATIONAL SUPERCOMPUTER FACILITY,  *        
 *               (JUNE 1990) CORNELL UNIVERSITY, ITHACA, NY.           *        
 *               CONTAINS RESTRICTED MATERIALS OF CORNELL UNIVERSITY,  *        
 *               (C) COPYRIGHT CORNELL UNIVERSITY 1990                 * 
 *---------------------------------------------------------------------*/

/************************************************************/
/* DFKpi4f()                                                */
/* --> Import routine for 4 byte IBM floats                 */
/* ie IEEE32->IBM32                                         */
/************************************************************/

#define pi4f_last 0x000000ff
#define pi4f_impl 0x00800000
#define pi4f_sign 0x80000000
#define pi4f_tiss 0x007fffff                                                         
#ifdef PROTOTYPE
PRIVATE int DFKpi4f(VOIDP s, VOIDP d, uint32 num_elm, uint32 source_stride,
		   uint32 dest_stride)
#else
PRIVATE int DFKpi4f(source, dest, num_elm, source_stride, dest_stride)
uint8 * source, * dest;
uint32 num_elm, source_stride, dest_stride;
#endif /* PROTOTYPE */
{
  int32 i,k, ibs, ibe, ibt;

#ifdef PROTOTYPE
  uint8 * source = (uint8*)s;
  uint8 * dest = (uint8*)d;
#endif /* PROTOTYPE */
  uint32 *buf;
  char *FUNC="DFKpi4f";

  HEclear();

  if(num_elm == 0){   /* No elements is an error */
    HERROR(DFE_BADCONV);
    return FAIL;
  }

  if(source_stride == 0 && dest_stride == 0)
      source_stride = dest_stride = 4;

  for(i = 0; i < num_elm; i++) {

    buf = (uint32*)source;
    ibt =   *buf;
    ibs =   *buf          &  pi4f_sign;
    ibe  = ( *buf >> 23 )  &  pi4f_last;

    if( ibe != 0 )  {
      if( ibe == 255 ) { 
	ibe = 378;
	ibt = pi4f_tiss;
      } 
      ibe = ibe - 127 + 256 +1;
      k = ibe%4;
      ibe = ibe/4;
      if( k != 0 ) 
	ibe = ibe + 1;
      ibe = ibe << 24 ;
      ibt = (ibt & pi4f_tiss) | pi4f_impl ;
      if (k != 0)
	ibt = ( ibt + (1 << (3-k) )  ) >> (4-k);
    }

    buf = (uint32*)dest;
    *buf = ibs | ibe | ibt;                            
    source += source_stride;
    dest   += dest_stride;
  }
  return 0;
}

/************************************************************/
/* DFKpo4f()                                                */
/* --> Export routine for 4 byte IBM floats                 */
/* IBM->IEEE                                                */
/************************************************************/

#define   po4f_exp   0x7F000000
#define   po4f_sign  0x80000000
#define   po4f_tis   0x00FFFFFF
#define   po4f_etis  0x007FFFFF

#ifdef PROTOTYPE 
PRIVATE int DFKpo4f(VOIDP s, VOIDP d, uint32 num_elm, uint32 source_stride,
		   uint32 dest_stride)
#else
PRIVATE int DFKpo4f(source, dest, num_elm, source_stride, dest_stride)
uint8 * source, * dest;
uint32 num_elm, source_stride, dest_stride;
#endif /* PROTOTYPE */
{

  int32 i, ibs, ibe, ibt, it, k;

#ifdef PROTOTYPE
  uint8 * source = (uint8*)s;
  uint8 * dest = (uint8*)d;
#endif /* PROTOTYPE */
  char *FUNC="DFKpi4f";
  uint32 *buf;
  uint8 exp;

  HEclear();

  if(num_elm == 0 ){		/* No elements is an error */
    HERROR(DFE_BADCONV);
    return FAIL;
  }

  if(source_stride == 0 && dest_stride == 0)
    source_stride = dest_stride = 4;

  for(i = 0; i < num_elm; i++) {

    buf = (uint32*)source;
    ibs = *buf  & po4f_sign;
    ibe = *buf  & po4f_exp;
    ibt = *buf  & po4f_tis;
    it = ibt << 8;

    for(k = 0;  (k < 5) && (it >= 0);  k++ ) 
      it = it << 1;

    if( k < 4 ) {
      ibt = (it >> 8) & po4f_etis;
      ibe = (ibe >> 22) - 256 + 127 - k - 1;
      if( ibe < 0 )
	ibe = ibt = 0;
      if( ibe >= 255 ) {
	ibe = 255;                                                
	ibt = 0;                                                 
      }                                                              
      ibe = ibe << 23;
    }

    /* put result into dest */
    buf = (uint32*)dest;
    *buf = ibs | ibe | ibt;                            
    source += source_stride;
    dest   += dest_stride;
  }
  return 0;

}

/************************************************************/
/* DFKpi8f()                                                */
/* --> Import routine for 8 byte VP floats                  */
/* ie IEEE64->IBM64                                         */
/************************************************************/

#define pi8f_impl 0x00100000 
#define pi8f_sign 0x80000000
#define pi8f_maxl 0x00ffffff 
#define pi8f_expn 0x7ff00000
#define pi8f_tiss 0x000fffff
#define pi8f_maxr 0xffffffff

#ifdef PROTOTYPE
PRIVATE int DFKpi8f(VOIDP s, VOIDP d, uint32 num_elm, uint32 source_stride,
		   uint32 dest_stride)
#else
PRIVATE int DFKpi8f(source, dest, num_elm, source_stride, dest_stride)
uint8 * source, * dest;
uint32 num_elm, source_stride, dest_stride;
#endif /* PROTOTYPE */
{
#ifdef PROTOTYPE
  uint8 * source = (uint8*)s;
  uint8 * dest   = (uint8*)d;
#endif /* PROTOTYPE */
  char *FUNC="DFKpi8f"; 
  int i;
  int32 ibs,ibe,ibt1,ibt2,isht,k;
  int32 *buf;

  HEclear();

  if(num_elm == 0) {		/* No elements is an error */ 
    HERROR(DFE_BADCONV);
    return FAIL;
  }

  if(source_stride == 0 && dest_stride == 0)
    source_stride = dest_stride = 8;

  for ( i = 0; i < num_elm ; i++) {

    buf  = (int32*)source;
    ibs  = *buf & pi8f_sign;
    ibe  = *buf & pi8f_expn;
    ibt1 = *buf & pi8f_tiss;
    ibt2 = *(buf+1);

    if (ibe != 0) {

      ibe = (ibe >> 20) - 1023 + 256 + 1;
      k = 0;                                                                    
      if (ibe > 508) k = 2;                                                     
      if (ibe <   0) k = 1; 

      switch (k) {

      case 1:
	ibe = ibt1 = ibt2 = 0;
	break;

      case 2:
	ibe = 127;
	ibt1 = pi8f_maxl;
	ibt2 = pi8f_maxr;
	break;

      default:
	isht = ibe % 4 -  1;
	ibe = ibe >> 2 ; 
	if ( isht != -1)
	  ibe += 1;
	else
	  isht = 3;

	ibt1 |=  pi8f_impl;
	if (isht != 0 ) {
	  ibt1 = ( ibt1 << isht ) |  (ibt2 >> 32-isht);
	  ibt2 = ibt2 << isht;
	}
	break;
      }
      ibe = ibe << 24;

    } /* ibe != 0 */

    /* put number into destination array */
    buf  = (int32*)dest;
    *buf = ibs | ibe | ibt1;
    *(buf+1) = ibt2;

    source += source_stride;
    dest   += dest_stride;
  }
  return 0;
}


/************************************************************/
/* DFKpo8f()                                                */
/* --> Export routine for 8 byte VP floats                  */
/* IBM64->IEEE64                                            */
/************************************************************/
#define  po8f_exp   0x7f000000
#define  po8f_sign  0x80000000
#define  po8f_tiss  0x00FFFFFF 
#define  po8f_nrm   0x00f00000 
static int32 take[4] = {0x007fffff, 0x003fffff, 0x001fffff, 0x000fffff};
static int32 look[4] = {0x00800000, 0x00400000, 0x00200000, 0x00100000};
#ifdef PROTOTYPE
PRIVATE int DFKpo8f(VOIDP s, VOIDP d, uint32 num_elm, uint32 source_stride,
		   uint32 dest_stride)
#else
PRIVATE int DFKpo8f(source, dest, num_elm, source_stride, dest_stride)
uint8 * source, * dest;
uint32 num_elm, source_stride, dest_stride;
#endif /* PROTOTYPE */
{
#ifdef PROTOTYPE
  uint8 * source = (uint8*)s;
  uint8 * dest = (uint8*)d;
#endif				/* PROTOTYPE */
  int32 ibs, ibe, ibt1, ibt2, it, isht;
  int i,k;
  int32 *buf;
  double *tmp;
  char *FUNC="DFKpo8f";
  union {
    struct { 
      int long i1;
      int long i2;
    } sti;
    struct { 
      double r;
    } str;
  } un;

  HEclear();

  if(num_elm == 0){		/* No elements is an error */
    HERROR(DFE_BADCONV);
    return FAIL;
  }

  if(source_stride == 0 && dest_stride == 0)
    source_stride = dest_stride = 8;

  for (i = 0; i < num_elm; i++) {

    buf = (int32*)source;
    ibs  = *buf & po8f_sign;
    ibe  = *buf & po8f_exp;
    ibt1 = *buf & po8f_tiss;
    ibt2 = *(buf+1);

    if  ( (ibt1 == 0) && (ibt2 == 0) )                                          
      ibe = 0 ;                                                                
    else {

      if ( (ibe != 0) && ( (ibt1 & po8f_nrm) == 0 ) ) {
	un.sti.i1 = *buf;
	un.sti.i2 = *(buf+1) ;
	un.str.r  = un.str.r + 0e0 ;
	ibe  = un.sti.i1 & po8f_exp ;
	ibt1 = un.sti.i1 & po8f_tiss ;
	ibt2 = un.sti.i2 ;
      }

      if      ((ibt1 & look[0]) != 0 )
	{ k = 3 ; ibt1 =  ibt1 & take[0] ; }
      else if ((ibt1 & look[1]) != 0 )
	{ k = 2 ; ibt1 =  ibt1 & take[1] ; }
      else if ((ibt1 & look[2]) != 0 )
	{ k = 1 ; ibt1 =  ibt1 & take[2] ; }
      else if ((ibt1 & look[3]) != 0 )
	{ k = 0 ;  ibt1 =  ibt1 & take[3] ; }
      else k = 4;

      /* mantissa */
      if  (k < 4) {
	if (k != 0) {
	  /* shift with rounding */
	    it   = (ibt2 & 65535) + (1 << (k-1)) ;
	    ibt2 = ( (ibt2 >> 16) & 65535 )  +  (it >> 16) ;
	    ibt1 = ibt1 + (ibt2 >> 16) ;
	    /* rounded */
	    ibt2 = ( (ibt2 & 65535) << (16-k) ) | ( (it & 65535) >> k) ;
	    it   = ibt1 << (32 - k) ;
	    ibt1 = ibt1 >> k ;
	    ibt2 = ibt2 | it;
	  }                                                                      

	/*  exponent */                                                       
	ibe = ( (ibe >> 22)  - 256 + 1023 + k - 4 ) << 20 ;                    
      } 
    }
    /* put number into destination array */
    buf = (int32*)dest;
    *buf     = ibs | ibe | ibt1;
    *(buf+1) = ibt2;
    source += source_stride;
    dest   += dest_stride;
  }
  return 0;
}


#endif /* VP */
/* End of Actual Conversion routines, start of Interface functions */

/************************************************************
 * If the programmer forgot to call DFKsetntype, then let
 * them know about it.
 * Should this be PUBLIC or PRIVATE
 ************************************************************/
#ifdef PROTOTYPE
PRIVATE int DFKInoset(VOIDP source, VOIDP dest, uint32 num_elm, 
	uint32 source_stride, uint32 dest_stride)
#else
PRIVATE int DFKInoset(source, dest, num_elm, source_stride, dest_stride)
uint8 * source, * dest;
uint32 num_elm, source_stride, dest_stride;
#endif /* PROTOTYPE */
{
  char *FUNC="DFKInoset";

  HEclear();

    /* shut the compiler up about not using the arguments */
    source=source;
    dest=dest;
    num_elm=num_elm;
    source_stride=source_stride;
    dest_stride=dest_stride;

  /* If this is causing a problem for you, call DFKsetntype */
  HERROR(DFE_BADCONV);
  return FAIL;
}


/*****************************************************************************
 * Routines that depend on the above information
 *****************************************************************************/

PRIVATE int32 g_ntype = DFNT_NONE;   /* Holds current number type. */
                                    /* Initially not set.         */

/************************************************************ 
* DFKqueryNT()
*   Determine the current conversion settings 
 ************************************************************/
PUBLIC
#ifdef PROTOTYPE
int32 DFKqueryNT(void) 
#else
int32 DFKqueryNT()
#endif /* PROTOTYPE */
{
  return g_ntype;
}

/************************************************************
 * DFKNTsize()
 *   Determine the size, given the number type
 ************************************************************/
PUBLIC
#ifdef PROTOTYPE
int DFKNTsize(int32 number_type)
#else
DFKNTsize(number_type)
int number_type;
#endif /* PROTOTYPE */
{
    char *FUNC="DFKNTsize";

	switch ((int32) number_type)  {
    	    case DFNT_NUCHAR:	return(SIZE_NUCHAR);
    	    case DFNT_NCHAR:	return(SIZE_NCHAR);
    	    case DFNT_NINT8:	return(SIZE_NINT8);
            case DFNT_NUINT8:   return(SIZE_NUINT8);

    	    case DFNT_NINT16:	return(SIZE_NINT16);
            case DFNT_NUINT16:  return(SIZE_NUINT16);

    	    case DFNT_NINT32:   return(SIZE_NINT32);
            case DFNT_NUINT32:  return(SIZE_NUINT32);

            case DFNT_NFLOAT32: return(SIZE_NFLOAT32);

            case DFNT_NFLOAT64: return(SIZE_NFLOAT64);

            case DFNT_LUCHAR:   return(SIZE_LUCHAR);
            case DFNT_LCHAR:    return(SIZE_LCHAR);
            case DFNT_LINT8:    return(SIZE_LINT8);
            case DFNT_LUINT8:   return(SIZE_LUINT8);

            case DFNT_LINT16:   return(SIZE_LINT16);
            case DFNT_LUINT16:  return(SIZE_LUINT16);

            case DFNT_LINT32:   return(SIZE_LINT32);
            case DFNT_LUINT32:  return(SIZE_LUINT32);

            case DFNT_LFLOAT32: return(SIZE_LFLOAT32);

            case DFNT_LFLOAT64: return(SIZE_LFLOAT64);

            case DFNT_UCHAR:    return(SIZE_UCHAR);
            case DFNT_CHAR:     return(SIZE_CHAR);
            case DFNT_INT8:     return(SIZE_INT8);
            case DFNT_UINT8:    return(SIZE_UINT8);

    	    case DFNT_INT16:	return(SIZE_INT16);
            case DFNT_UINT16:   return(SIZE_UINT16);

    	    case DFNT_INT32:	return(SIZE_INT32);
            case DFNT_UINT32:   return(SIZE_UINT32);

            case DFNT_FLOAT32:  return(SIZE_FLOAT32);

            case DFNT_FLOAT64:  return(SIZE_FLOAT64);

    	    default: break;
  	}	/* switch	*/
  		/* hdf default format   */
  return FAIL;
}

/************************************************************
 * DFKsetNT()
 *   Set the number type for future conversion calls
 ************************************************************/
PUBLIC
#ifdef PROTOTYPE
intn DFKsetNT(int32 ntype)
#else
intn DFKsetNT(ntype)
int32 ntype;
#endif /* PROTOTYPE */
{
  char *FUNC="DFKsetNT";

  HEclear();

  g_ntype = ntype;

  switch(ntype) {
    case DFNT_CHAR8:
    case DFNT_UCHAR8:
    case DFNT_INT8:    
    case DFNT_UINT8:   DFKnumin  = UI8_IN;
                       DFKnumout = UI8_OUT;
                       return 0;
    case DFNT_INT16:   DFKnumin  = SI16_IN;
                       DFKnumout = SI16_OUT;
                       return 0;
    case DFNT_UINT16:  DFKnumin  = UI16_IN;
                       DFKnumout = UI16_OUT;
                       return 0;
    case DFNT_INT32:   DFKnumin  = SI32_IN;
                       DFKnumout = SI32_OUT;
                       return 0;
    case DFNT_UINT32:  DFKnumin  = UI32_IN;
                       DFKnumout = UI32_OUT;
                       return 0;
    case DFNT_FLOAT32: DFKnumin  = F32_IN;
                       DFKnumout = F32_OUT;
                       return 0;
    case DFNT_FLOAT64: DFKnumin  = F64_IN;
                       DFKnumout = F64_OUT;
                       return 0;

    /* 
     * NATIVE MODE 'CONVERSIONS'
     */
    case DFNT_NCHAR: 
    case DFNT_NINT8:
    case DFNT_NUCHAR:
    case DFNT_NUINT8:   DFKnumin  = NUI8_IN;
                        DFKnumout = NUI8_OUT;
                        return 0;
    case DFNT_NINT16:   DFKnumin  = NSI16_IN;
                        DFKnumout = NSI16_OUT;
                        return 0;
    case DFNT_NUINT16:  DFKnumin  = NUI16_IN;
                        DFKnumout = NUI16_OUT;
                        return 0;
    case DFNT_NINT32:   DFKnumin  = NSI32_IN;
                        DFKnumout = NSI32_OUT;
                        return 0;
    case DFNT_NUINT32:  DFKnumin  = NUI32_IN;
                        DFKnumout = NUI32_OUT;
                        return 0;
    case DFNT_NFLOAT32: DFKnumin  = NF32_IN;
                        DFKnumout = NF32_OUT;
                        return 0;
    case DFNT_NFLOAT64: DFKnumin  = NF64_IN;
                        DFKnumout = NF64_OUT;
                        return 0;

    /*
     * Little Endian Conversions
     */
    case DFNT_LCHAR:
    case DFNT_LINT8:
    case DFNT_LUCHAR:
    case DFNT_LUINT8:   DFKnumin  = LUI8_IN;
                        DFKnumout = LUI8_OUT;
                        return 0;
    case DFNT_LINT16:   DFKnumin  = LSI16_IN;
                        DFKnumout = LSI16_OUT;
                        return 0;
    case DFNT_LUINT16:  DFKnumin  = LUI16_IN;
                        DFKnumout = LUI16_OUT;
                        return 0;
    case DFNT_LINT32:   DFKnumin  = LSI32_IN;
                        DFKnumout = LSI32_OUT;
                        return 0;
    case DFNT_LUINT32:  DFKnumin  = LUI32_IN;
                        DFKnumout = LUI32_OUT;
                        return 0;
    case DFNT_LFLOAT32: DFKnumin  = LF32_IN;
                        DFKnumout = LF32_OUT;
                        return 0;
    case DFNT_LFLOAT64: DFKnumin  = LF64_IN;
                        DFKnumout = LF64_OUT;
                        return 0;

/* No conversion routines are specified for DFNT_custom.  User must provide. */
/* Users should call DFCV_SetCustomIn() and DFCV_SetCustomOut() if they      */
/* choose to use DFNT_CUSTOM.  Users should provide their own method to      */
/* distinguish between multiple 'custom' conversion routines.  HDF only      */
/* knows such routines as type 'DFNT_CUSTOM'.                                */

    case DFNT_CUSTOM:  g_ntype = DFNT_CUSTOM;
                       return 0;
    default:           HERROR(DFE_BADCONV);
                       return FAIL;
  }
}


/*****************************************************************************
 * The following routine provides an easy method for the user to setup custom
 * conversion routines....
 *****************************************************************************/
PUBLIC
#ifdef PROTOTYPE
int DFKsetcustom(
  int (*DFKcustin)(VOIDP source, VOIDP dest, uint32 num_elm,
		   uint32 source_stride, uint32 dest_stride), 
  int (*DFKcustout)(VOIDP source, VOIDP dest, uint32 num_elm, 
		    uint32 source_stride, uint32 dest_stride)
)
#else
DFKsetcustom(DFKcustout, DFKcustin)
int (*DFKcustin)();
int (*DFKcustout)();
#endif /* PROTOTYPE */
{
  DFKnumin = DFKcustin;
  DFKnumout = DFKcustout;
  DFKsetNT(DFNT_CUSTOM);   /* Keep HDF from getting confused */
  return 0;
} 


/*------------------------------------------------------------------
 * Name:    DFKisnativeNT
 * Purpose: Determine whether number type is native mode
 * Inputs:  numbertype: number type
 * Returns: 1 if true, 0 if false
 * Users:   DFSDgetslice
 * Method:  Checks to see if the "native mode" bit is set
 * Remarks:
 *------------------------------------------------------------------*/

#if defined PROTOTYPE
int32 DFKisnativeNT(int32 numbertype)
#else
int32 DFKisnativeNT(numbertype)
int32 numbertype;
#endif /* PROTOTYPE */
{
    return ((DFNT_NATIVE & numbertype)>0 ? 1 : 0 );
}

/*------------------------------------------------------------------
 * Name:    DFKislitendNT
 * Purpose: Determine whether number type is little-endian mode
 * Inputs:  numbertype: number type
 * Returns: 1 if true, 0 if false
 * Users:   DFSDgetslice
 * Method:  Checks to see if the "native mode" bit is set
 * Remarks:
 *------------------------------------------------------------------*/

#if defined PROTOTYPE
int32 DFKislitendNT(int32 numbertype)
#else
int32 DFKislitendNT(numbertype)
int32 numbertype;
#endif /* PROTOTYPE */
{
    return ((DFNT_LITEND & numbertype)>0 ? 1 : 0 );
}

/************************************************************
 * DFconvert()
 *
 * This routine is called by HDF version 3.0 compatibility
 * routines.  It serves as a jump point to the new version 4.0
 * comversion functions.  DFconvert() CANNOT be used by Vdata
 * applications because it assumes a stride of 1 (for
 * compatibility). Vdata routines should call DFnum_in() and 
 * DFKnumout() (depending on which translation is needed)
 *
 * uint8 * source    location where the data is stored
 * uint8 * dest      location to put the converted data
 * int  * ntype       the overall number type of the data, ie DFNT_FLOAT...
 * int  * sourcetype  the specific type of the source data, ie DFNTF_IEEE...
 * int  * desttype    the specifid type of the converted data, ie DFNTF_VAX...
 * int  * size        the number (total) of BYTES to convert
 ************************************************************/
#if defined PROTOTYPE
int DFconvert(uint8 *source,uint8 *dest,int ntype,int sourcetype,int desttype,
        int32 size)
#else
int DFconvert(source, dest, ntype, sourcetype, desttype, size)
uint8 * source, * dest; 
int ntype, sourcetype, desttype;
int32 size;
#endif /* PROTOTYPE */
{ 
  register int32 num_elm;
  char *FUNC="DFconvert";

  HEclear();

  if(DFKsetNT(ntype) == FAIL) {
    HERROR(DFE_BADCONV);
    return FAIL;
  }

  if(sourcetype == desttype) { 
    HDmemcpy(dest, source, size);
    return 0;
  }

  num_elm = size / 4;

/* Check to see if they want to covert numbers in from the disk */
  if(sourcetype == DFNTF_IEEE && (desttype == DFNTF_VAX || 
				  desttype == DFNTF_CRAY || 
				  desttype == DFNTF_PC      ) )
    return (DFKnumin)((VOIDP)source, (VOIDP)dest, num_elm, 0, 0);

/* Check to see if they want to convert numbers out to disk */
  if(desttype == DFNTF_IEEE && (sourcetype == DFNTF_VAX || 
				sourcetype == DFNTF_CRAY || 
				sourcetype == DFNTF_PC     ) )
    return DFKnumout((VOIDP)source, (VOIDP)dest, num_elm, 0, 0);

/* Return an error because they did not specify valid translation codes */
  HERROR(DFE_BADCONV);
  return FAIL;
}

/*------------------------------------------------------------------
 * Name:    DFKgetPNSC
 * Purpose: Get PlatformNumberSubclass for a given number type
 * Inputs:  numtype: number type to get subclass for
 *          machinetype: machine-type code
 * Returns: PlatformNumberSubclass on success, FAIL on failure with
 *          error set
 * Users:   DFSDgetslice
 * Method:  Checks NT_TYPES to determine whether it is a char, int, or
 *          float, then looks in corresponding field of machine type
 *          (DFMT) to get the class.
 * Remarks:
 *------------------------------------------------------------------*/

#if defined(PROTOTYPE)
int8 DFKgetPNSC(int32 numbertype, int32 machinetype)
#else
int8 DFKgetPNSC(numbertype, machinetype)
int32 numbertype, machinetype;
#endif /* PROTOTYPE */
{
    switch (numbertype) {
        case DFNT_FLOAT32:
        case DFNT_FLOAT64:
        case DFNT_FLOAT128: return (int8)((machinetype>>8) & 0x0f);

        case DFNT_INT8:
        case DFNT_UINT8:
        case DFNT_INT16:
        case DFNT_UINT16:
        case DFNT_INT32:
        case DFNT_UINT32:
        case DFNT_INT64:
        case DFNT_UINT64:
        case DFNT_INT128:
        case DFNT_UINT128: return (int8)((machinetype>>4) & 0x0f);

        case DFNT_CHAR8:
        case DFNT_UCHAR8:
        case DFNT_CHAR16:
        case DFNT_UCHAR16: return (int8)(machinetype & 0x0f);


        case DFNT_NFLOAT32:
        case DFNT_NFLOAT64:
        case DFNT_NFLOAT128: return (int8)((machinetype>>8) & 0x0f);

        case DFNT_NINT8:
        case DFNT_NUINT8:
        case DFNT_NINT16:
        case DFNT_NUINT16:
        case DFNT_NINT32:
        case DFNT_NUINT32:
        case DFNT_NINT64:
        case DFNT_NUINT64:
        case DFNT_NINT128:
        case DFNT_NUINT128: return (int8)((machinetype>>4) & 0x0f);

        case DFNT_NCHAR8:
        case DFNT_NUCHAR8:
        case DFNT_NCHAR16:
        case DFNT_NUCHAR16: return (int8)(machinetype & 0x0f);
        default: return FAIL;
    }
}

/*----------------------------------------------------------------------------
* Name:	DFKconvert
* Purpose: set number type and do the convert
* Inputs:  source -- location where the data is stored
*	   dest -- location to put the converted data
*	   ntype -- the current number type
*	   num_elm -- number of elements to be converted
*	   access -- DFACC_READ for numin, DFACC_WRITE for numout
*	   source_stride, dest_stride -- strides in source and destination
* Returns: 0 -- succeed; FAIL -- failure
* Users:   DFSDgetsdg, DFSDputsdg, DFSDIgetslice, DFSDIgetslice
* Method:  Calls DFKsetNT, then call DFnumin or DFnumout
*---------------------------------------------------------------------------*/

#if defined PROTOTYPE
int32 DFKconvert(VOIDP source, VOIDP dest, int32 ntype, int32 num_elm,
	 	 int16 access, int32 source_stride, int32 dest_stride)
#else
int32 DFKconvert(source, dest, ntype, num_elm, access, source_stride,
		 dest_stride)
VOIDP source;
VOIDP dest;
int32 ntype, num_elm, source_stride, dest_stride;
int16 access;
#endif

{
    int ret;

    DFKsetNT(ntype);
    if (access == DFACC_READ) 
        ret = DFKnumin(source, dest, num_elm, source_stride, dest_stride);
    else
        ret = DFKnumout(source, dest, num_elm, source_stride, dest_stride);
    return(ret);
}

/*****************************************************************************
 * Miscellaneous Other Conversion Routines
 *****************************************************************************/


