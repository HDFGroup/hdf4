#ifdef RCSID
static char RcsId[] = "@(#)$Revision$";
#endif
/*
$Header$

$Log$
Revision 1.4  1993/01/19 05:54:16  koziol
Merged Hyperslab and JPEG routines with beginning of DEC ALPHA
port.  Lots of minor annoyances fixed.

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
    DFKnb1b - -  Native mode for 8 bit integers
    DFKnb2b -  Native mode for 16 bit integers
    DFKnb4b -  Native mode for 32 bit integers and floats 
    DFKnb8b -  Native mode for 64 bit floats 
    DFKsb2b -  Byte swapping for 16 bit integers
    DFKsb4b -  Byte swapping for 32 bit integers
    DFKsb8b -  Byte swapping for 64 bit floats 
    DFKui2i -  Unicos routine for importing 16 bit unsigned integers
    DFKui2s -  Unicos routine for importing 16 bit signed integers
    DFKuo2i -  Unicos routine for exporting 16 bit integers
    DFKui4i -  Unicos routine for importing unsigned 32bit integers
    DFKui4s -  Unicos routine for importing signed 32bit integers
    DFKuo4i -  Unicos routine for exporting 4 byte integers (both)
    DFKui4f -  Unicos routine for importing 32 bit floats   
    DFKuo4f -  Unicos routine for exporting 32 bit floats  
    DFKui8f -  Unicos routine for importing 64 bit floats 
    DFKuo64f - Unicos routine for exporting 64 bit floats
    DFKvi4f -  VMS routine for importing 32 bit floats
    DFKvo4f -  VMS routine for exporting 32 bit floats
    DFKvi8f -  VMS routine for importing 64 bit floats
    DFKvo8f -  VMS routine for exporting 64 bit floats

 Other PUBLIC functions:
    DFKmachineNTsize - Determine size in machine, given number type
    DFKhdfNTsize     - Determine size in HDF format, given number type
    DFKsetNT         - Set number type for future conversion calls
    DFKsetcustom    - Template for user to setup custom conversion 
                      routines
    DFKisnative     - Checks whether number type is native mode
    DFconvert       - provide compatibility with 3.0 routines

 Private functions:
    DFKInoset    - Indicate that DFKsetNT hasn't been called

 Remarks:

 *------------------------------------------------------------------*/

#if defined(__STDC__) && !defined(PROTOTYPE)
#define PROTOTYPE
#endif /* Assert prototype defined */

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

/*****************************************************************************/
/* CONSTANT DEFINITIONS                                                      */
/* These will eventually be incorporated into dfi.h                          */
/*****************************************************************************/
#if !defined(VMS) && !defined(PC) && !defined(MIPSEL) && !defined(CONVEXNATIVE)
#     if !defined(UNICOS)
#          define UI8_IN     DFKnb1b    /* Unsigned Integer, 8 bits */
#          define UI8_OUT    DFKnb1b
#          define SI16_IN    DFKnb2b    /* S = Signed */
#          define SI16_OUT   DFKnb2b  
#          define UI16_IN    DFKnb2b
#          define UI16_OUT   DFKnb2b
#          define SI32_IN    DFKnb4b
#          define SI32_OUT   DFKnb4b
#          define UI32_IN    DFKnb4b
#          define UI32_OUT   DFKnb4b
#          define F32_IN     DFKnb4b    /* Float, 32 bits */
#          define F32_OUT    DFKnb4b
#          define F64_IN     DFKnb8b
#          define F64_OUT    DFKnb8b
#          define NUI8_IN    DFKnb1b    /* Native Unsigned Integer, 8 bits */
#          define NUI8_OUT   DFKnb1b
#          define NSI16_IN   DFKnb2b
#          define NSI16_OUT  DFKnb2b
#          define NUI16_IN   DFKnb2b
#          define NUI16_OUT  DFKnb2b
#          define NSI32_IN   DFKnb4b
#          define NSI32_OUT  DFKnb4b
#          define NUI32_IN   DFKnb4b
#          define NUI32_OUT  DFKnb4b
#          define NF32_IN    DFKnb4b
#          define NF32_OUT   DFKnb4b
#          define NF64_IN    DFKnb8b
#          define NF64_OUT   DFKnb8b
#     else
#          define UI8_IN     DFKnb1b
#          define UI8_OUT    DFKnb1b
#          define SI16_IN    DFKui2s
#          define SI16_OUT   DFKuo2i
#          define UI16_IN    DFKui2i
#          define UI16_OUT   DFKuo2i
#          define SI32_IN    DFKui4s
#          define SI32_OUT   DFKuo4i
#          define UI32_IN    DFKui4i
#          define UI32_OUT   DFKuo4i
#          define F32_IN     DFKui4f
#          define F32_OUT    DFKuo4f
#          define F64_IN     DFKui8f
#          define F64_OUT    DFKuo8f
#          define NUI8_IN    DFKnb1b
#          define NUI8_OUT   DFKnb1b
#          define NSI16_OUT  DFKnb8b
#          define NSI16_IN   DFKnb8b
#          define NUI16_IN   DFKnb8b
#          define NUI16_OUT  DFKnb8b
#          define NSI32_IN   DFKnb8b
#          define NSI32_OUT  DFKnb8b
#          define NUI32_IN   DFKnb8b
#          define NUI32_OUT  DFKnb8b
#          define NF32_IN    DFKnb8b
#          define NF32_OUT   DFKnb8b
#          define NF64_IN    DFKnb8b
#          define NF64_OUT   DFKnb8b
#     endif /* !UNICOS */
#else /* must be VMS || PC || MIPSEL || CONVEXNATIVE */
#     define UI8_IN     DFKnb1b
#     define UI8_OUT    DFKnb1b
#   if !defined(CONVEXNATIVE)
#     define SI16_IN    DFKsb2b  /* The s in DFKsb2b is for swap */
#     define SI16_OUT   DFKsb2b
#     define UI16_IN    DFKsb2b
#     define UI16_OUT   DFKsb2b
#     define SI32_IN    DFKsb4b
#     define SI32_OUT   DFKsb4b
#     define UI32_IN    DFKsb4b
#     define UI32_OUT   DFKsb4b
#     if defined(VMS)
#          define F32_IN     DFKvi4f
#          define F32_OUT    DFKvo4f
#          define F64_IN     DFKvi8f
#          define F64_OUT    DFKvo8f
#     else  /* !VMS */
#          define F32_IN     DFKsb4b
#          define F32_OUT    DFKsb4b
#          define F64_IN     DFKsb8b
#          define F64_OUT    DFKsb8b
#     endif /* VMS */
#  else /* CONVEXNATIVE */
#     define SI16_IN	DFKnb2b
#     define SI16_OUT   DFKnb2b
#     define UI16_IN    DFKnb2b
#     define UI16_OUT   DFKnb2b
#     define SI32_IN    DFKnb4b
#     define SI32_OUT   DFKnb4b
#     define UI32_IN    DFKnb4b
#     define UI32_OUT   DFKnb4b
#     define F32_IN     DFKci4f	/* CONVEX stuff */
#     define F32_OUT    DFKco4f
#     define F64_IN     DFKci8f
#     define F64_OUT    DFKco8f
#  endif /* !CONVEXNATIVE */
#     define NUI8_IN    DFKnb1b
#     define NUI8_OUT   DFKnb1b
#     define NSI16_IN   DFKnb2b
#     define NSI16_OUT  DFKnb2b
#     define NUI16_IN   DFKnb2b
#     define NUI16_OUT  DFKnb2b
#     define NSI32_IN   DFKnb4b
#     define NSI32_OUT  DFKnb4b
#     define NUI32_IN   DFKnb4b
#     define NUI32_OUT  DFKnb4b
#     define NF32_IN    DFKnb4b
#     define NF32_OUT   DFKnb4b
#     define NF64_IN    DFKnb8b
#     define NF64_OUT   DFKnb8b
#     define BYTE_SWAPPER
#endif /* !VMS && !PC && !MIPS && !CONVEXNATIVE */


/*****************************************************************************/
/* STRUCTURE DEFINTIONS                                                      */
/*****************************************************************************/
union fpx
{
     float f;
     long  l;
};

union float_uint_uchar {
    float32 f;
    int32 i;
    unsigned char c[4];
};

/*
**  Static function prototypes
*/
static int DFKInoset
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

#ifdef BYTE_SWAPPER

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
        dest[0] = source[3];
        dest[1] = source[2];
        dest[2] = source[1];
        dest[3] = source[0];
        dest += 4;
        source += 4;
      }
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

#endif /* BYTE_SWAPPER */

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

/************************************************************/
/* DFKui2i()                                                */
/* -->Unicos routine for importing 2 byte data items        */ 
/* (**) This routine converts two byte IEEE to eight byte   */
/*      Cray little endian integer.                         */
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
    for(i = 0; i < num_elm; i++) {
      lptr_dest[0] = 0x0000000000000000;
      dest[6] = source[0];
      dest[7] = source[1];
      source += 2;
      lptr_dest++;
      dest = (uint8*)lptr_dest;
    }
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
  register uint32 i;
  int fast_processing=0;
#ifdef PROTOTYPE
  uint8 * source = (uint8*)s;
  uint8 * dest = (uint8*)d;
#endif /* PROTOTYPE */
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
    for(i = 0; i < num_elm; i++) {
      dest[0] = source[6];
      dest[1] = source[7];
      dest += 2;
      source += 8;
    }
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
  char *FUNC="DFKui4i";

  HEclear();

  if(source == dest || num_elm == 0) {  /* Inplace conversions  not permitted */
    HERROR(DFE_BADCONV);                /* No elements to convert is an error */
    return FAIL;
  }

  if(source_stride == 0 && dest_stride == 0) {
    fast_processing = 1;
  }
  
  if(fast_processing)
    for(i = 0; i < num_elm; i++) {
      lptr_dest[0] = 0;
      dest[4] = source[0];
      dest[5] = source[1];
      dest[6] = source[2];
      dest[7] = source[3];
      source += 4;
      lptr_dest ++;
      dest = (uint8 *)lptr_dest;
    }
  else 
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
    }
  return 0;
}

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
  char *FUNC="DFKui4s";

  HEclear();

  if(source == dest || num_elm == 0) {  /* Inplace conversions  not permitted */
    HERROR(DFE_BADCONV);                /* No elements to convert is an error */
    return FAIL;
  }

  if(source_stride == 0 && dest_stride == 0) {
    fast_processing = 1;
  }
  
  if(fast_processing)
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
  else 
    for(i = 0; i < num_elm; i++) {
      if((source[0] & 0x80)) {          /* Can't forget to sign extend */
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
  register uint32 i;
#ifdef PROTOTYPE
  uint8 * source = (uint8*)s;
  uint8 * dest = (uint8*)d;
#endif /* PROTOTYPE */
  char *FUNC="DFKuo4i";

  HEclear();

  if(source == dest || num_elm == 0) {  /* Inplace conversions  not permitted */
    HERROR(DFE_BADCONV);                /* No elements to convert is an error */
    return FAIL;
  }

  if(source_stride == 0 && dest_stride == 0) {
    fast_processing = 1;
  }
  
  if(fast_processing)
    for(i = 0; i < num_elm; i++) {
      dest[0] = source[4];
      dest[1] = source[5];
      dest[2] = source[6];
      dest[3] = source[7];
      dest += 4;
      source += 8;
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

#define UI4_MASKA 0x8000000000000000
#define UI4_MASKB 0x7f80000000000000
#define UI4_MASKC 0x007fffff00000000
#define UI4_MASKD 0x0000800000000000

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
  register int i,j;
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
  if(source_stride == 0 && dest_stride == 0) {
      fast_processing = 1;            
    if((num_elm % 2))                   /* If this is true, we have odd num */
      odd_man_out = 1;
  }

  if(fast_processing) {
    num_elm = num_elm / 2;
    for(i = 0; i < num_elm; i++) {
      if(*(float*)lptr_src != 0) {
	*lptr_dest = (((*lptr_src & UI4_MASKA) |
		       ((*lptr_src & UI4_MASKB) >> 7) +
		       (16258 << 48)) |     
		      (((*lptr_src & UI4_MASKC) >> 8) | (UI4_MASKD)));
	if(((*lptr_src & 0xffffffff00000000) << 1)== 0)
	  *lptr_dest = 0;
      }
      else
	*lptr_dest = *lptr_src;
      lptr_dest++;
      if(*(float*)lptr_src != 0) {
	*lptr_dest = ((((((*lptr_src << 32) & UI4_MASKA)) |
			(((*lptr_src <<32) & UI4_MASKB) >> 7) +
			(16258 << 48)) |     
		       ((((*lptr_src << 32) & UI4_MASKC) >> 8) |(UI4_MASKD))));
	if(((*lptr_src & 0x00000000ffffffff) << 33)== 0)
	  *lptr_dest = 0;
      }
      else
	*lptr_dest = *lptr_src;
      lptr_src++;
      lptr_dest++;
    }
    if(odd_man_out){
      if(((float*)lptr_src)[0] != 0) {
	lptr_dest[0] = ((((lptr_src[0]) & UI4_MASKA) |
		       (((lptr_src[0]) & UI4_MASKB) >> 7) + (16258 << 48)) |
		      ((((lptr_src[0]) & UI4_MASKC) >> 8) | (UI4_MASKD)));
	if(((lptr_src[0] & 0xffffffff00000000) << 1)== 0)
	  lptr_dest[0] = 0;
      }
      else
	*lptr_dest = *lptr_src;
    }
  }
  else { /* We end up here if we are doing stride based processing */
    buf1 = 0;
    for(i = 0; i < num_elm; i++) {
      dud1[0] = source[0];               /* Loop would be less efficient */
      dud1[1] = source[1];
      dud1[2] = source[2];
      dud1[3] = source[3];

      if((float)buf1 != 0) {
	buf2 = (((buf1 & UI4_MASKA) |
		       ((buf1 & UI4_MASKB) >> 7) +
		       (16258 << 48)) |     
		      (((buf1 & UI4_MASKC) >> 8) | (UI4_MASKD)));
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


#define UO4_MASKA (0x8000000000000000)
#define UO4_MASKB (0x7fff000000000000)
#define UO4_MASKC (0x00007fffff000000)
#define UO4_MASKD (0x0000000000800000)

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
  register int i,j;
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
  if(source_stride == 0 && dest_stride == 0) {
      fast_processing = 1;            
    if((num_elm % 2))                   /* If this is true, we have odd num */
      odd_man_out = 1;
  }

  if(fast_processing) {
    for(i = 0; i < (int)(num_elm/2); i++) {
      buf1 = lptr_src[0];
      buf2 = lptr_src[1];
      if(buf1 != 0) 
	buf1 = (((buf1 & UO4_MASKA) | 
		 ((((buf1 & UO4_MASKB) >> 48) - 16258) << 55)) +
		(((buf1 & UO4_MASKC) + 
		  ((buf1 & UO4_MASKD) << 1)) << 8));
      else
	buf1 = 0;
      if(buf2 != 0) 
	buf2 = ((((buf2 & UO4_MASKA) | 
		  ((((buf2 & UO4_MASKB) >> 48) - 16258) << 55)) +
		 (((buf2 & UO4_MASKC) + 
		   ((buf2 & UO4_MASKD) << 1)) << 8)) >> 32);
      else
        buf2 = 0;
      lptr_dest[0] = ((buf1 & 0xffffffff00000000) | 
		      (buf2 & 0x00000000ffffffff));
      lptr_src ++;
      lptr_src ++;
      lptr_dest ++;
    }
    if(odd_man_out){
      if(lptr_src[0] != 0)
    lptr_dest[0] = (((lptr_src[0] & UO4_MASKA) | 
		       ((((lptr_src[0] & UO4_MASKB) >> 48) - 16258) << 55)) +
		      (((lptr_src[0] & UO4_MASKC) + 
			((lptr_src[0] & UO4_MASKD) << 1)) << 8));
      else
	lptr_dest[0] = 0;
    }
  }
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
	buf2 = (((buf1 & UO4_MASKA) | 
		 ((((buf1 & UO4_MASKB) >> 48) -16258) << 55)) +
		(((buf1 & UO4_MASKC) + ((buf1 & UO4_MASKD) << 1)) << 8));
      else
	buf2 = buf1;

      dest[0] = dud2[0];            /* Loop would be less efficient */
      dest[1] = dud2[1];
      dest[2] = dud2[2];
      dest[3] = dud2[3];

      source += source_stride;
      dest += dest_stride;
    }
  }
  return;
}

#define UI8_MASKA 0x8000000000000000
#define UI8_MASKB 0x7ff0000000000000
#define UI8_MASKC 0x000fffffffffffff
#define UI8_MASKD 0x0000000000000008
#define UI8_MASKE 0x0000800000000000

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
  register int i,j;
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

  if(fast_processing)
    for(i = 0; i < num_elm; i ++) {
      if (lptr_src[0] != 0) {
	lptr_dest[0] = (((lptr_src[0] & UI8_MASKA) |
			 ((lptr_src[0] & UI8_MASKB) >> 4) + (15362 << 48)) |
			((((lptr_src[0] & UI8_MASKC) + 
			   ((lptr_src[0] & UI8_MASKD) << 1)) >> 5) | 
			 (UI8_MASKE)) );
	if ((lptr_dest[0] << 1) == 0) 
	  lptr_dest[0] = 0;
      }
      else 
	lptr_dest[0] = 0;
      lptr_src++;
      lptr_dest++;
    }
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
	buf = (((buf & UI8_MASKA) | ((buf & UI8_MASKB) >> 4) + (15362 << 48)) |
	       ((((buf & UI8_MASKC) + ((buf & UI8_MASKD) << 1)) >> 5) | 
		(UI8_MASKE)) );
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

#define UO8_MASKA 0x8000000000000000
#define UO8_MASKB 0x7fff000000000000
#define UO8_MASKC 0x00007fffffffffff

/************************************************************/
/* DFKuo64f()                                               */
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
  register int i,j;
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

  if(fast_processing)
    for(i = 0; i < num_elm; i ++) {
      if (lptr_src[0] != 0) 
    lptr_dest[0] = (((lptr_src[0] & UO8_MASKA) |
             (((((lptr_src[0] & UO8_MASKB) >> 48) - 15362) << 53) >> 1)) +
            ((lptr_src[0] & UO8_MASKC) << 5));
      else 
	lptr_dest[0] = 0;
      lptr_src++;
      lptr_dest++;
    }
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
    buf = (((buf & UO8_MASKA) |                             /* sign bit */
        (((((buf & UO8_MASKB) >> 48) - 15362) << 53) >> 1)) |   /* exp */
           ((buf & UO8_MASKC) << 5));                       /* mantissa */
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
#endif  /* CONVEXNATIVE */

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

    	    case DFNT_UCHAR:	return(SIZE_UCHAR);
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
int32 DFKconvert(uint8 *source, uint8 *dest, int32 ntype, int32 num_elm,
	 	 int16 access, int32 source_stride, int32 dest_stride)
#else
int32 DFKconvert(source, dest, ntype, num_elm, access, source_stride,
		 dest_stride)
uint8 *source, *dest;
int32 ntype, num_elm, source_stride, dest_stride;
int16 access;
#endif

{
    int ret;

    DFKsetNT(ntype);
    if (access == DFACC_READ) 
        ret = DFKnumin((VOIDP)source, (VOIDP)dest, num_elm, source_stride,
		       dest_stride);
    else ret = DFKnumout((VOIDP)source, (VOIDP)dest, num_elm, source_stride, 
			 dest_stride);
    return(ret);
}

/*****************************************************************************
 * Miscellaneous Other Conversion Routines
 *****************************************************************************/


