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


/*------------------------------------------------------------------
 File:  dfkcray.c

 Purpose:
    Routines to support Cray conversion to and from HDF format

 Invokes:

 PRIVATE conversion functions:
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

 Remarks:
    These files used to be in dfconv.c, but it got a little too huge,
    so I broke them out into seperate files. - Q

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

#include "hdf.h"
#include "herr.h"
#include "hconv.h"

/*****************************************************************************/
/* NUMBER CONVERSION ROUTINES FOR THE UNICOS OPERATING SYSTEM                */
/* Parameter dest_stride is used because source buffer and dest buffer will  */
/* be different sizes for all data types except char.                        */
/*****************************************************************************/

#if defined(UNICOS)

#define NOINTCRAY2IEG
/* #define NOFLOATCRAY2IEG */

PRIVATE VOID DFKswap
    PROTO((VOIDP s, VOIDP d, uintn elem_size, uintn num_elem));

#define SWAP_MASKA  0xffffffff00000000
#define SWAP_MASKB  0x00000000ffffffff
#define SWAP_MASKC  0xffff0000ffff0000
#define SWAP_MASKD  0x0000ffff0000ffff
#define SWAP_MASKE  0xff00ff00ff00ff00
#define SWAP_MASKF  0x00ff00ff00ff00ff

/************************************************************/
/* DFKswap()                                                */
/* -->Swap groups of 'n' bytes                              */
/*  !NOTE!: This routine does not handle overlapping memory */
/*              blocks properly, but that condition should  */
/*              be caught by the converion routines...      */
/*          Also, there in no provision for source or       */
/*          destination strides other than 1.               */
/************************************************************/
#ifdef PROTOTYPE
PRIVATE VOID DFKswap(VOIDP s, VOIDP d, uintn elem_size, uintn num_elem)
#else
PRIVATE VOID DFKswap(source, dest, elem_size, num_elem)
uint8 * source, * dest;
uintn elem_size, num_elem;
#endif /* PROTOTYPE */
{
    register uintn i;
#ifdef PROTOTYPE
    uint8 * source = (uint8*)s;
    uint8 * dest = (uint8*)d;
#endif /* PROTOTYPE */
    unsigned long *lp_dest,*lp_src;
    intn odd_man_out;
    uintn n;
    char *FUNC="DFKswap";

    switch(elem_size) {
        case 2:     /* 2 byte elements */
            odd_man_out=num_elem%4;         /* check for odd number of elem. */
            n=num_elem/4;
            for(i=0,lp_src=(unsigned long *)source,lp_dest=(unsigned long *)dest; i<n; i++,lp_dest++,lp_src++) {
                *lp_dest=((*lp_src&SWAP_MASKE)>>8)|
                        ((*lp_src&SWAP_MASKF)<<8);
              } /* end for */
            if(odd_man_out) {
                source=(uint8 *)lp_src;
                dest=(uint8 *)lp_dest;
                switch(odd_man_out) {
                    case 3:
                        dest[0]=source[5];
                        dest[1]=source[4];
                        dest[2]=source[3];
                        dest[3]=source[2];
                        dest[4]=source[1];
                        dest[5]=source[0];
                        break;

                    case 2:
                        dest[0]=source[3];
                        dest[1]=source[2];
                        dest[2]=source[1];
                        dest[3]=source[0];
                        break;

                    case 1:
                        dest[0]=source[1];
                        dest[1]=source[0];
                        break;

                  } /* end switch */
              } /* end if */
            break;

        case 4:     /* 4 byte elements */
            odd_man_out=num_elem%2;         /* check for odd number of elem. */
            n=num_elem/2;
            for(i=0,lp_src=(unsigned long *)source,lp_dest=(unsigned long *)dest; i<n; i++,lp_dest++,lp_src++) {
                *lp_dest=((*lp_src&SWAP_MASKC)>>16)|
                        ((*lp_src&SWAP_MASKD)<<16);
                *lp_dest=((*lp_dest&SWAP_MASKE)>>8)|
                        ((*lp_dest&SWAP_MASKF)<<8);
              } /* end for */
            if(odd_man_out) {
                source=(uint8 *)lp_src;
                dest=(uint8 *)lp_dest;
                dest[0]=source[3];
                dest[1]=source[2];
                dest[2]=source[1];
                dest[3]=source[0];
              } /* end if */
            break;

        case 8:     /* 8 byte elements */
            for(i=0,lp_src=(unsigned long *)source,lp_dest=(unsigned long *)dest; i<num_elem; i++,lp_dest++,lp_src++) {
                *lp_dest=((*lp_src&SWAP_MASKA)>>32)|
                        ((*lp_src&SWAP_MASKB)<<32);
                *lp_dest=((*lp_dest&SWAP_MASKC)>>16)|
                        ((*lp_dest&SWAP_MASKD)<<16);
                *lp_dest=((*lp_dest&SWAP_MASKE)>>8)|
                        ((*lp_dest&SWAP_MASKF)<<8);
              } /* end for */
            break;
      } /* end switch */
}   /* end DFKswap() */

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
int DFKui2i(VOIDP s, VOIDP d, uint32 num_elm, uint32 source_stride,
		   uint32 dest_stride)
#else
int DFKui2i(source, dest, num_elm, source_stride, dest_stride)
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
	int ierr;       /* error from IEG2CRAY */
	int type=7;     /* type of conversion to perform 7=short integer */
	int bitoff=0;   /* bit offset in the IEEE stream */
    intn n_elem=1;  /* the number of elements for stride-based processing */

    HEclear();

    if(source == dest || num_elm == 0)   /* Inplace conversions not permitted */
        HRETURN_ERROR(DFE_BADCONV,FAIL); /* No elements is an error */

    /* Find out if it is OK to use faster array processing */
    if(source_stride == 0 && dest_stride == 0)
        fast_processing = 1;

    if(fast_processing) {
        ierr=IEG2CRAY(&type,&num_elm,source,&bitoff,dest);
	    if(ierr!=0)
            HRETURN_ERROR(DFE_BADCONV,FAIL);  /* error in Cray conversion */
      } /* end if */
    else { /* Generic stride processing */
        for(i = 0; i < num_elm; i++) {
#ifdef NOINTCRAY2IEG
            dest[0] = 0x00;
            dest[1] = 0x00;
            dest[2] = 0x00;
            dest[3] = 0x00;
            dest[4] = 0x00;
            dest[5] = 0x00;
            dest[6] = source[0];
            dest[7] = source[1];
#else
            ierr=IEG2CRAY(&type,&n_elem,source,&bitoff,dest);
    	    if(ierr!=0)
                HRETURN_ERROR(DFE_BADCONV,FAIL);  /* error in Cray conversion */
#endif
            source += source_stride;
            dest += dest_stride;
        }
      } /* end if */
    return(SUCCEED);
}

#define UI2S_MASKA  0xffff000000000000
#define UI2S_MASKB  0x0000ffff00000000
#define UI2S_MASKC  0x00000000ffff0000
#define UI2S_MASKD  0x000000000000ffff
#define UI2S_MASKE  0x8000000000000000
#define UI2S_MASKF  0x0000800000000000
#define UI2S_MASKG  0x0000000080000000
#define UI2S_MASKH  0x0000000000008000
#define UI2S_MASKI  0xffffffffffff0000

/************************************************************/
/* DFKui2s()                                                */
/* -->Unicos routine for importing 2 byte signed ints       */
/* (**) This routine converts two byte IEEE to eight byte   */
/*      Cray.                                               */
/************************************************************/
#ifdef PROTOTYPE
int DFKui2s(VOIDP s, VOIDP d, uint32 num_elm, uint32 source_stride,
		   uint32 dest_stride)
#else
int DFKui2s(source, dest, num_elm, source_stride, dest_stride)
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
    int ierr;       /* error from IEG2CRAY */
    int type=7;     /* type of conversion to perform 7=short integer */
    int bitoff=0;   /* bit offset in the IEEE stream */
    intn n_elem=1;  /* the number of elements for stride-based processing */

    HEclear();

    if(source == dest || num_elm == 0)   /* Inplace conversions  not permitted */
        HRETURN_ERROR(DFE_BADCONV,FAIL);    /* No elements to convert is an error */

    /* Find out if it is OK to use faster array processing */
    if(source_stride == 0 && dest_stride == 0)
        fast_processing = 1;

    if(fast_processing) {
        ierr=IEG2CRAY(&type,&num_elm,source,&bitoff,dest);
    	if(ierr!=0)
            HRETURN_ERROR(DFE_BADCONV,FAIL);  /* error in Cray conversion */
      } /* end if */
    else { /* Generic stride processing */
        for(i = 0; i < num_elm; i++) {
#ifdef NOINTCRAY2IEG
            if((source[0] & 0x80)) {          /* Can't forget to extend sign */
    	        dest[0] = 0xff;
    	        dest[1] = 0xff;
    	        dest[2] = 0xff;
    	        dest[3] = 0xff;
    	        dest[4] = 0xff;
    	        dest[5] = 0xff;
               } /* end if */
            else {
            	dest[0] = 0x00;
            	dest[1] = 0x00;
            	dest[2] = 0x00;
            	dest[3] = 0x00;
            	dest[4] = 0x00;
        	    dest[5] = 0x00;
              } /* end else */
            dest[6] = source[0];
            dest[7] = source[1];
#else
            ierr=IEG2CRAY(&type,&n_elem,source,&bitoff,dest);
        	if(ierr!=0)
                HRETURN_ERROR(DFE_BADCONV,FAIL);  /* error in Cray conversion */
#endif
            source += source_stride;
            dest += dest_stride;
          } /* end for */
      } /* end if */
    return(SUCCEED);
}

#define UO2I_MASK 0x000000000000ffff

/************************************************************/
/* DFKuo2i()                                                */
/* -->Unicos routine for exporting 2 byte data items        */
/************************************************************/
#ifdef PROTOTYPE
int DFKuo2i(VOIDP s, VOIDP d, uint32 num_elm, uint32 source_stride,
		   uint32 dest_stride)
#else
int DFKuo2i(source, dest, num_elm, source_stride, dest_stride)
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
	int ierr;       /* error from IEG2CRAY */
	int type=7;     /* type of conversion to perform 7=short integer */
	int bitoff=0;   /* bit offset in the IEEE stream */
    intn n_elem=1;  /* the number of elements for stride-based processing */

    HEclear();

    if(source == dest || num_elm == 0)  /* Inplace conversions  not permitted */
        HRETURN_ERROR(DFE_BADCONV,FAIL);  /* No elements to convert is an error */

    /* Find out if it is OK to use faster array processing */
    if(source_stride == 0 && dest_stride == 0)
        fast_processing = 1;

    if(fast_processing) {
        ierr=CRAY2IEG(&type,&num_elm,dest,&bitoff,source);
	    if(ierr!=0)
            HRETURN_ERROR(DFE_BADCONV,FAIL);  /* error in Cray conversion */
      } /* end if */
    else { /* Generic Stride processing */
        for(i = 0; i < num_elm; i++) {
#ifdef NOINTCRAY2IEG
            dest[0] = source[6];
            dest[1] = source[7];
#else
            ierr=CRAY2IEG(&type,&n_elem,dest,&bitoff,source);
    	    if(ierr!=0)
                HRETURN_ERROR(DFE_BADCONV,FAIL);  /* error in Cray conversion */
#endif
            source += source_stride;
            dest += dest_stride;
          } /* end for */
      } /* end else */
  return(SUCCEED);
}

#define UI4I_MASKA 0xffffffff00000000
#define UI4I_MASKB 0x00000000ffffffff

/************************************************************/
/* DFKui4i()                                                */
/* -->Unicos routine for importing 4 byte unsigned ints     */
/************************************************************/
#ifdef PROTOTYPE
int DFKui4i(VOIDP s, VOIDP d, uint32 num_elm, uint32 source_stride,
		   uint32 dest_stride)
#else
int DFKui4i(source, dest, num_elm, source_stride, dest_stride)
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
    int ierr;       /* error from IEG2CRAY */
    int type=1;     /* type of conversion to perform 1=short integer */
    int bitoff=0;   /* bit offset in the IEEE stream */
    intn n_elem=1;  /* the number of elements for stride-based processing */

    HEclear();

    if(source == dest || num_elm == 0)  /* Inplace conversions  not permitted */
        HRETURN_ERROR(DFE_BADCONV,FAIL);    /* No elements to convert is an error */

    if(source_stride == 0 && dest_stride == 0)
        fast_processing = 1;

    if(fast_processing) {
        ierr=IEG2CRAY(&type,&num_elm,source,&bitoff,dest);
	    if(ierr!=0)
            HRETURN_ERROR(DFE_BADCONV,FAIL);  /* error in Cray conversion */
      } /* end if */
    else {
        for(i = 0; i < num_elm; i++) {
#ifdef NOINTCRAY2IEG
            dest[0] = 0;
            dest[1] = 0;
            dest[2] = 0;
            dest[3] = 0;
            dest[4] = source[0];
            dest[5] = source[1];
            dest[6] = source[2];
            dest[7] = source[3];
#else
            ierr=IEG2CRAY(&type,&n_elem,source,&bitoff,dest);
    	    if(ierr!=0)
                HRETURN_ERROR(DFE_BADCONV,FAIL);  /* error in Cray conversion */
#endif
            dest += dest_stride;
            source += source_stride;
          } /* end for */
      } /* end else */
  return(SUCCEED);
}

#define UI4S_MASKA 0xffffffff00000000
#define UI4S_MASKB 0x00000000ffffffff
#define UI4S_MASKC 0x8000000000000000
#define UI4S_MASKD 0x0000000080000000
#define UI4S_MASKE 0xffffffff00000000

/************************************************************/
/* DFKui4s()                                                */
/* -->Unicos routine for importing 4 signed ints            */
/************************************************************/
#ifdef PROTOTYPE
int DFKui4s(VOIDP s, VOIDP d, uint32 num_elm, uint32 source_stride,
		   uint32 dest_stride)
#else
int DFKui4s(source, dest, num_elm, source_stride, dest_stride)
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
    int ierr;       /* error from IEG2CRAY */
    int type=1;     /* type of conversion to perform 1=short integer */
	int bitoff=0;   /* bit offset in the IEEE stream */
    intn n_elem=1;  /* the number of elements for stride-based processing */

    HEclear();

    if(source == dest || num_elm == 0)  /* Inplace conversions  not permitted */
        HRETURN_ERROR(DFE_BADCONV,FAIL);    /* No elements to convert is an error */

    if(source_stride == 0 && dest_stride == 0)
        fast_processing = 1;

    if(fast_processing) {
        ierr=IEG2CRAY(&type,&num_elm,source,&bitoff,dest);
	    if(ierr!=0)
            HRETURN_ERROR(DFE_BADCONV,FAIL);  /* error in Cray conversion */
      } /* end if */
    else
        for(i = 0; i < num_elm; i++) {
#ifdef NOINTCRAY2IEG
            if(source[0] & 0x80) {          /* Can't forget to sign extend */
                dest[0] = 0xff;
                dest[1] = 0xff;
                dest[2] = 0xff;
                dest[3] = 0xff;
              } /* end if */
            else {
                dest[0] = 0;
                dest[1] = 0;
                dest[2] = 0;
                dest[3] = 0;
              } /* end else */
            dest[4] = source[0];
            dest[5] = source[1];
            dest[6] = source[2];
            dest[7] = source[3];
#else
            ierr=IEG2CRAY(&type,&n_elem,source,&bitoff,dest);
    	    if(ierr!=0)
                HRETURN_ERROR(DFE_BADCONV,FAIL);  /* error in Cray conversion */
#endif
            dest += dest_stride;
            source += source_stride;
          } /* end for */
  return(SUCCEED);
}

#define UO4I_MASK 0x00000000ffffffff

/************************************************************/
/* DFKuo4i()                                                */
/* -->Unicos routine for exporting 4 byte data items        */
/************************************************************/

#ifdef PROTOTYPE
int DFKuo4i(VOIDP s, VOIDP d, uint32 num_elm, uint32 source_stride,
		   uint32 dest_stride)
#else
int DFKuo4i(source, dest, num_elm, source_stride, dest_stride)
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
    int ierr;       /* error from IEG2CRAY */
    int type=1;     /* type of conversion to perform 7=short integer */
    int bitoff=0;   /* bit offset in the IEEE stream */
    intn n_elem=1;  /* the number of elements for stride-based processing */

    HEclear();

    if(source == dest || num_elm == 0)   /* Inplace conversions  not permitted */
        HRETURN_ERROR(DFE_BADCONV,FAIL);    /* No elements to convert is an error */

    if(source_stride == 0 && dest_stride == 0)
        fast_processing = 1;

    if(fast_processing) {
        ierr=CRAY2IEG(&type,&num_elm,dest,&bitoff,source);
	    if(ierr!=0)
            HRETURN_ERROR(DFE_BADCONV,FAIL);  /* error in Cray conversion */
      } /* end if */
    else
        for(i = 0; i < num_elm; i++) {
#ifdef NOINTCRAY2IEG
            dest[0] = source[4];
            dest[1] = source[5];
            dest[2] = source[6];
            dest[3] = source[7];
#else
            ierr=CRAY2IEG(&type,&n_elem,dest,&bitoff,source);
    	    if(ierr!=0)
                HRETURN_ERROR(DFE_BADCONV,FAIL);  /* error in Cray conversion */
#endif
            dest += dest_stride;
            source += source_stride;
          } /* end for */
  return(SUCCEED);
}

#define UI4F_MASKA 0x8000000000000000
#define UI4F_MASKB 0x7f80000000000000
#define UI4F_MASKC 0x007fffff00000000
#define UI4F_MASKD 0x0000800000000000
#define UI4F_MASKE 0x0000000080000000
#define UI4F_MASKF 0x000000007f800000
#define UI4F_MASKG 0x00000000007fffff
#define UI4F_MASKH 0x0000000000008000
#define UI4F_MASKI 0x7fffffff00000000
#define UI4F_MASKJ 0x000000007fffffff

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
int DFKui4f(VOIDP s, VOIDP d, uint32 num_elm, uint32 source_stride,
		   uint32 dest_stride)
#else
int DFKui4f(source, dest, num_elm, source_stride, dest_stride)
uint8 * source;
uint8 * dest;
uint32 num_elm;
uint32 source_stride;
uint32 dest_stride;
#endif /* PROTOTYPE */
{
    int fast_processing = 0;            /* By default not array processed */
    int odd_man_out = 0;                /* By default there are even num_elm */
    int i,j,n;
    long buf1;                          /* This is a temporary stride buf */
    long buf2;                          /* This is a temporary stride buf */
    uint8 * dud1 = (uint8*)&buf1;       /* Dummy pointer to buf1 for strides */
    uint8 * dud2 = (uint8*)&buf2;       /* Dummy pointer to buf2 for strides */
#ifdef PROTOTYPE
    uint8 * source = (uint8*)s;         /* Cray does not like certain   */
    uint8 * dest = (uint8*)d;           /* void and void* constructions */
#endif /* PROTOTYPE */
    long * lptr_src = (long*)source;
    long * lptr_dest = (long*)dest;
    char *FUNC="DFKui4f";
	int ierr;       /* error from IEG2CRAY */
	int type=2;     /* type of conversion to perform 2=32-bit float */
	int bitoff=0;   /* bit offset in the IEEE stream */
    intn n_elem=1;  /* the number of elements for stride-based processing */

    HEclear();

    /* Check for conversion errors */
    if(source == dest || num_elm == 0)  /* Inplace conversions not permitted */
        HRETURN_ERROR(DFE_BADCONV,FAIL);    /* under UNICOS */

    /* Find out if it is OK to use faster array processing */
    if(source_stride == 0 && dest_stride == 0)
        fast_processing = 1;

    if(fast_processing) {
        ierr=IEG2CRAY(&type,&num_elm,source,&bitoff,dest);
    	if(ierr!=0)
            HRETURN_ERROR(DFE_BADCONV,FAIL);  /* error in Cray conversion */
      } /* end if */
    else { /* We end up here if we are doing stride based processing */
        buf1 = 0;
        for(i = 0; i < num_elm; i++) {
#ifdef NOFLOATCRAY2IEG
            dud1[0] = source[0];               /* Loop would be less efficient */
            dud1[1] = source[1];
            dud1[2] = source[2];
            dud1[3] = source[3];

            if((float)buf1 != 0) {
                buf2 = (((buf1 & UI4F_MASKA) | ((buf1 & UI4F_MASKB) >> 7) +
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

#else
            ierr=IEG2CRAY(&type,&n_elem,source,&bitoff,dest);
        	if(ierr!=0)
                HRETURN_ERROR(DFE_BADCONV,FAIL);  /* error in Cray conversion */
#endif
            source += source_stride;
            dest += dest_stride;
          } /* end for */
      } /* end else */
  return(SUCCEED);
}

#define UO4F_MASKA 0x8000000000000000
#define UO4F_MASKB 0x7fff000000000000
#define UO4F_MASKC 0x00007fffff000000
#define UO4F_MASKD 0x0000000000800000
#define UO4F_MASKE 0xffffffff00000000

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
int DFKuo4f(VOIDP s, VOIDP d, uint32 num_elm, uint32 source_stride,
		   uint32 dest_stride)
#else
int DFKuo4f(source, dest, num_elm, source_stride, dest_stride)
uint8 * source, * dest;
uint32 num_elm, source_stride, dest_stride;
#endif /* PROTOTYPE */
{
    int fast_processing = 0;        /* By default not array processed */
    int odd_man_out = 0;            /* By default there are even num_elm */
    int i,j,n;
    long buf1;                      /* This is a temporary stride buf */
    long buf2;                      /* This is a temporary stride buf */
    uint8 * dud1 = (uint8*)&buf1;   /* Dummy pointer to buf1 for strides */
    uint8 * dud2 = (uint8*)&buf2;   /* Dummy pointer to buf2 for strides */
#ifdef PROTOTYPE
    uint8 * source = (uint8*)s;     /* Cray does not like certain   */
    uint8 * dest = (uint8*)d;       /* void and void* constructions */
#endif /* PROTOTYPE */
    long * lptr_src = (long*)source;
    long * lptr_dest = (long*)dest;
    char *FUNC="DFKuo4f";
    int ierr;       /* error from IEG2CRAY */
    int type=2;     /* type of conversion to perform 2=32-bit float */
    int bitoff=0;   /* bit offset in the IEEE stream */
    intn n_elem=1;  /* the number of elements for stride-based processing */

    HEclear();

    /* Check for conversion errors */
    if(source == dest || num_elm == 0)  /* Inplace conversions not permitted */
        HRETURN_ERROR(DFE_BADCONV,FAIL);    /* under UNICOS */

    /* Find out if it is OK to use faster array processing */
    if(source_stride == 0 && dest_stride == 0)
        fast_processing = 1;

    if(fast_processing) {
        ierr=CRAY2IEG(&type,&num_elm,dest,&bitoff,source);
	    if(ierr!=0)
            HRETURN_ERROR(DFE_BADCONV,FAIL);  /* error in Cray conversion */
      } /* end if */
    else { /* We end up here if we are doing stride based processing */
        buf1 = 0;
        for(i = 0; i < num_elm; i++) {
#ifdef NOFLOATCRAY2IEG
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
                        (((buf1 & UO4F_MASKC) +
                        ((buf1 & UO4F_MASKD) << 1)) << 8));
            else
                buf2 = buf1;

            dest[0] = dud2[0];            /* Loop would be less efficient */
            dest[1] = dud2[1];
            dest[2] = dud2[2];
            dest[3] = dud2[3];

#else
            ierr=CRAY2IEG(&type,&n_elem,dest,&bitoff,source);
    	    if(ierr!=0)
                HRETURN_ERROR(DFE_BADCONV,FAIL);  /* error in Cray conversion */
#endif
            source += source_stride;
            dest += dest_stride;
          } /* end for */
      } /* end else */
  return(SUCCEED);
}

#define UI8F_MASKA 0x8000000000000000
#define UI8F_MASKB 0x7ff0000000000000
#define UI8F_MASKC 0x000fffffffffffff
#define UI8F_MASKD 0x0000000000000008
#define UI8F_MASKE 0x0000800000000000
#define UI8F_MASKG 0x7fffffffffffffff

/************************************************************/
/* DFKui8f()                                                */
/* -->Unicos routine for importing 64 bit floats            */
/************************************************************/

#ifdef PROTOTYPE
int DFKui8f(VOIDP s, VOIDP d, uint32 num_elm, uint32 source_stride,
		   uint32 dest_stride)
#else
int DFKui8f(source, dest, num_elm, source_stride, dest_stride)
uint8 * source, * dest;
uint32 num_elm, source_stride, dest_stride;
#endif /* PROTOTYPE */
{
    int fast_processing = 0;        /* By default not array processed */
    int i,j,n;
    long buf;                       /* This is a temporary stride buf */
    uint8 * dud = (uint8*)&buf;     /* Dummy pointer to buf1 for strides */
#ifdef PROTOTYPE
    uint8 * source = (uint8*)s;     /* Cray does not like certain   */
    uint8 * dest = (uint8*)d;       /* void and void* constructions */
#endif /* PROTOTYPE*/
    long * lptr_src = (long*)source;
    long * lptr_dest = (long*)dest;
    char *FUNC="DFKui8f";
	int ierr;       /* error from IEG2CRAY */
	int type=8;     /* type of conversion to perform 8=64-bit float */
	int bitoff=0;   /* bit offset in the IEEE stream */
    intn n_elem=1;  /* the number of elements for stride-based processing */

    HEclear();

    /* Check for conversion errors */
    if(source == dest || num_elm == 0) /* Inplace conversions not permitted */
        HRETURN_ERROR(DFE_BADCONV,FAIL);    /* under UNICOS */

    /* Find out if it is OK to use faster array processing */
    if(source_stride == 0 && dest_stride == 0)
        fast_processing = 1;

    if(fast_processing) {
        ierr=IEG2CRAY(&type,&num_elm,source,&bitoff,dest);
    	if(ierr!=0)
            HRETURN_ERROR(DFE_BADCONV,FAIL);  /* error in Cray conversion */
      } /* end if */
    else
        for(i = 0; i < num_elm; i ++) {
#ifdef NOFLOATCRAY2IEG
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

#else
            ierr=IEG2CRAY(&type,&n_elem,source,&bitoff,dest);
        	if(ierr!=0)
                HRETURN_ERROR(DFE_BADCONV,FAIL);  /* error in Cray conversion */
#endif
            source += source_stride;
            dest += dest_stride;
          } /* end for */
    return(SUCCEED);
}

#define UO8F_MASKA 0x8000000000000000
#define UO8F_MASKB 0x7fff000000000000
#define UO8F_MASKC 0x00007fffffffffff

/************************************************************/
/* DFKuo8f()                                                */
/* -->Unicos routine for exporting 64 bit floats            */
/************************************************************/

#ifdef PROTOTYPE
int DFKuo8f(VOIDP s, VOIDP d, uint32 num_elm, uint32 source_stride,
		   uint32 dest_stride)
#else
int DFKuo8f(source, dest, num_elm, source_stride, dest_stride)
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
	int ierr;       /* error from IEG2CRAY */
	int type=8;     /* type of conversion to perform 8=64-bit float */
	int bitoff=0;   /* bit offset in the IEEE stream */
    intn n_elem=1;  /* the number of elements for stride-based processing */

    HEclear();

    /* Check for conversion errors */
    if(source == dest || num_elm == 0) /* Inplace conversions not permitted */
        HRETURN_ERROR(DFE_BADCONV,FAIL);    /* under UNICOS */

    /* Find out if it is OK to use faster array processing */
    if(source_stride == 0 && dest_stride == 0)
        fast_processing = 1;

    if(fast_processing) {
        ierr=CRAY2IEG(&type,&num_elm,dest,&bitoff,source);
    	if(ierr!=0)
            HRETURN_ERROR(DFE_BADCONV,FAIL);  /* error in Cray conversion */
      } /* end if */
    else
        for(i = 0; i < num_elm; i ++) {
#ifdef NOFLOATCRAY2IEG
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

#else
            ierr=CRAY2IEG(&type,&n_elem,dest,&bitoff,source);
        	if(ierr!=0)
                HRETURN_ERROR(DFE_BADCONV,FAIL);  /* error in Cray conversion */
#endif
            source += source_stride;
            dest += dest_stride;
          } /* end for */
    return(SUCCEED);
}

#define LUI2I_MASKA1 0xff00000000000000
#define LUI2I_MASKA2 0x00ff000000000000
#define LUI2I_MASKB1 0x0000ff0000000000
#define LUI2I_MASKB2 0x000000ff00000000
#define LUI2I_MASKC1 0x00000000ff000000
#define LUI2I_MASKC2 0x0000000000ff0000
#define LUI2I_MASKD1 0x000000000000ff00
#define LUI2I_MASKD2 0x00000000000000ff

/************************************************************/
/* DFKlui2i()                                               */
/* -->Unicos routine for importing 2 byte data items        */
/* (**) This routine converts two byte little-endian IEEE   */
/*      to eight byte Cray big endian integer.              */
/************************************************************/
#ifdef PROTOTYPE
int DFKlui2i(VOIDP s, VOIDP d, uint32 num_elm, uint32 source_stride,
		   uint32 dest_stride)
#else
int DFKlui2i(source, dest, num_elm, source_stride, dest_stride)
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
    int ierr;       /* error from IEG2CRAY */
    int type=7;     /* type of conversion to perform 7=short integer */
    int bitoff=0;   /* bit offset in the IEEE stream */
    uint8 *tmp_dst; /* temporary buffer to hold byte swapped values */
    intn n_elem=1;  /* the number of elements for stride-based processing */

    HEclear();

    if(source == dest || num_elm == 0)  /* Inplace conversions not permitted */
        HRETURN_ERROR(DFE_BADCONV,FAIL);    /* No elements is an error */

    /* Find out if it is OK to use faster array processing */
    if(source_stride == 0 && dest_stride == 0)
        fast_processing = 1;

    if(fast_processing) {
        tmp_dst=(uint8 *)HDgetspace(2*num_elm);
        DFKswap(source,tmp_dst,2,num_elm);
        ierr=IEG2CRAY(&type,&num_elm,tmp_dst,&bitoff,dest);
        HDfreespace(tmp_dst);
    	if(ierr!=0)
            HRETURN_ERROR(DFE_BADCONV,FAIL);  /* error in Cray conversion */
      } /* end for */
    else { /* Generic stride processing */
#ifdef NOINTCRAY2IEG
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
          } /* end for */
#else
        tmp_dst=(uint8 *)HDgetspace(2);
        for(i = 0; i < num_elm; i++) {
            DFKswap(source,tmp_dst,2,1);
            ierr=IEG2CRAY(&type,&n_elem,tmp_dst,&bitoff,dest);
        	if(ierr!=0) {
                HDfreespace(tmp_dst);
                HRETURN_ERROR(DFE_BADCONV,FAIL);  /* error in Cray conversion */
              } /* end if */
            source += source_stride;
            dest += dest_stride;
          } /* end for */
        HDfreespace(tmp_dst);
#endif
      } /* end else */
  return(SUCCEED);
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
#define LUI2S_MASKI  0xffffffffffff0000

/************************************************************/
/* DFKlui2s()                                                */
/* -->Unicos routine for importing 2 byte signed ints       */
/* (**) This routine converts two byte IEEE to eight byte   */
/*      Cray.                                               */
/************************************************************/
#ifdef PROTOTYPE
int DFKlui2s(VOIDP s, VOIDP d, uint32 num_elm, uint32 source_stride,
		   uint32 dest_stride)
#else
int DFKlui2s(source, dest, num_elm, source_stride, dest_stride)
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
	int ierr;       /* error from IEG2CRAY */
	int type=7;     /* type of conversion to perform 7=short integer */
	int bitoff=0;   /* bit offset in the IEEE stream */
    uint8 *tmp_dst; /* temporary buffer to hold byte swapped values */
    intn n_elem=1;  /* the number of elements for stride-based processing */

    HEclear();

    if(source == dest || num_elm == 0)  /* Inplace conversions  not permitted */
        HRETURN_ERROR(DFE_BADCONV,FAIL);    /* No elements to convert is an error */

    /* Find out if it is OK to use faster array processing */
    if(source_stride == 0 && dest_stride == 0)
        fast_processing = 1;

    if(fast_processing) {
        tmp_dst=(uint8 *)HDgetspace(2*num_elm);
        DFKswap(source,tmp_dst,2,num_elm);
        ierr=IEG2CRAY(&type,&num_elm,tmp_dst,&bitoff,dest);
        HDfreespace(tmp_dst);
    	if(ierr!=0)
            HRETURN_ERROR(DFE_BADCONV,FAIL);  /* error in Cray conversion */
      } /* end for */
    else { /* Generic stride processing */
#ifdef NOINTCRAY2IEG
        for(i = 0; i < num_elm; i++) {
            if((source[1] & 0x80)) {          /* Can't forget to extend sign */
    	        dest[0] = 0xff;
            	dest[1] = 0xff;
            	dest[2] = 0xff;
            	dest[3] = 0xff;
            	dest[4] = 0xff;
            	dest[5] = 0xff;
              } /* end if */
            else {
            	dest[0] = 0x00;
            	dest[1] = 0x00;
            	dest[2] = 0x00;
            	dest[3] = 0x00;
            	dest[4] = 0x00;
            	dest[5] = 0x00;
              } /* end else */
            dest[6] = source[1];
            dest[7] = source[0];
            source += source_stride;
            dest += dest_stride;
          } /* end for */
#else
        tmp_dst=(uint8 *)HDgetspace(2);
        for(i = 0; i < num_elm; i++) {
            DFKswap(source,tmp_dst,2,1);
            ierr=IEG2CRAY(&type,&n_elem,tmp_dst,&bitoff,dest);
        	if(ierr!=0) {
                HDfreespace(tmp_dst);
                HRETURN_ERROR(DFE_BADCONV,FAIL);  /* error in Cray conversion */
              } /* end if */
            source += source_stride;
            dest += dest_stride;
          } /* end for */
        HDfreespace(tmp_dst);
#endif
      } /* end else */
    return(SUCCEED);
}

#define LUO2I_MASKA 0x00000000000000ff
#define LUO2I_MASKB 0x000000000000ff00

/************************************************************/
/* DFKluo2i()                                               */
/* -->Unicos routine for exporting 2 byte little-endian     */
/*      data items                                          */
/************************************************************/
#ifdef PROTOTYPE
int DFKluo2i(VOIDP s, VOIDP d, uint32 num_elm, uint32 source_stride,
		   uint32 dest_stride)
#else
int DFKluo2i(source, dest, num_elm, source_stride, dest_stride)
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
	int ierr;       /* error from IEG2CRAY */
	int type=7;     /* type of conversion to perform 7=short integer */
	int bitoff=0;   /* bit offset in the IEEE stream */
    uint8 *tmp_dst; /* temporary buffer to hold byte swapped values */
    intn n_elem=1;  /* the number of elements for stride-based processing */

    HEclear();

    if(source == dest || num_elm == 0)  /* Inplace conversions  not permitted */
        HRETURN_ERROR(DFE_BADCONV,FAIL);/* No elements to convert is an error */

    /* Find out if it is OK to use faster array processing */
    if(source_stride == 0 && dest_stride == 0)
        fast_processing = 1;

    if(fast_processing) {
        tmp_dst=(uint8 *)HDgetspace(2*num_elm);
        ierr=CRAY2IEG(&type,&num_elm,tmp_dst,&bitoff,source);
    	if(ierr!=0) {
            HDfreespace(tmp_dst);
            HRETURN_ERROR(DFE_BADCONV,FAIL);  /* error in Cray conversion */
          } /* end if */
        DFKswap(tmp_dst,dest,2,num_elm);
        HDfreespace(tmp_dst);
      } /* end if */
    else { /* Generic Stride processing */
#ifdef NOINTCRAY2IEG
        for(i = 0; i < num_elm; i++){
            dest[0] = source[7];
            dest[1] = source[6];
            source += source_stride;
            dest += dest_stride;
          } /* end for */
#else
        tmp_dst=(uint8 *)HDgetspace(2);
        for(i = 0; i < num_elm; i++) {
            ierr=CRAY2IEG(&type,&n_elem,tmp_dst,&bitoff,source);
        	if(ierr!=0) {
                HDfreespace(tmp_dst);
                HRETURN_ERROR(DFE_BADCONV,FAIL);  /* error in Cray conversion */
              } /* end if */
            DFKswap(tmp_dst,dest,2,1);
            source += source_stride;
            dest += dest_stride;
          } /* end for */
        HDfreespace(tmp_dst);
#endif
      } /* end else */
  return(SUCCEED);
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
int DFKlui4i(VOIDP s, VOIDP d, uint32 num_elm, uint32 source_stride,
		   uint32 dest_stride)
#else
int DFKlui4i(source, dest, num_elm, source_stride, dest_stride)
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
	int ierr;       /* error from IEG2CRAY */
	int type=1;     /* type of conversion to perform 1=integer */
	int bitoff=0;   /* bit offset in the IEEE stream */
    uint8 *tmp_dst; /* temporary buffer to hold byte swapped values */
    intn n_elem=1;  /* the number of elements for stride-based processing */

    HEclear();

    if(source == dest || num_elm == 0) /* Inplace conversions  not permitted */
        HRETURN_ERROR(DFE_BADCONV,FAIL);/* No elements to convert is an error */

    if(source_stride == 0 && dest_stride == 0)
        fast_processing = 1;

    if(fast_processing) {
        tmp_dst=(uint8 *)HDgetspace(4*num_elm);
        DFKswap(source,tmp_dst,4,num_elm);
        ierr=IEG2CRAY(&type,&num_elm,tmp_dst,&bitoff,dest);
        HDfreespace(tmp_dst);
    	if(ierr!=0)
            HRETURN_ERROR(DFE_BADCONV,FAIL);  /* error in Cray conversion */
      } /* end if */
    else {
#ifdef NOINTCRAY2IEG
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
          } /* end for */
#else
        tmp_dst=(uint8 *)HDgetspace(4);
        for(i = 0; i < num_elm; i++) {
            DFKswap(source,tmp_dst,4,1);
            ierr=IEG2CRAY(&type,&n_elem,tmp_dst,&bitoff,dest);
        	if(ierr!=0) {
                HDfreespace(tmp_dst);
                HRETURN_ERROR(DFE_BADCONV,FAIL);  /* error in Cray conversion */
              } /* end if */
            dest += dest_stride;
            source += source_stride;
          } /* end for */
        HDfreespace(tmp_dst);
#endif
      } /* end else */
    return(SUCCEED);
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
#define LUI4S_MASKK 0xffffffff00000000

/************************************************************/
/* DFKlui4s()                                                */
/* -->Unicos routine for importing 4 signed ints            */
/************************************************************/
#ifdef PROTOTYPE
int DFKlui4s(VOIDP s, VOIDP d, uint32 num_elm, uint32 source_stride,
		   uint32 dest_stride)
#else
int DFKlui4s(source, dest, num_elm, source_stride, dest_stride)
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
	int ierr;       /* error from IEG2CRAY */
	int type=1;     /* type of conversion to perform 1=integer */
	int bitoff=0;   /* bit offset in the IEEE stream */
    uint8 *tmp_dst; /* temporary buffer to hold byte swapped values */
    intn n_elem=1;  /* the number of elements for stride-based processing */

    HEclear();

    if(source == dest || num_elm == 0) /* Inplace conversions  not permitted */
        HRETURN_ERROR(DFE_BADCONV,FAIL);/* No elements to convert is an error */

    if(source_stride == 0 && dest_stride == 0)
        fast_processing = 1;

    if(fast_processing) {
        tmp_dst=(uint8 *)HDgetspace(4*num_elm);
        DFKswap(source,tmp_dst,4,num_elm);
        ierr=IEG2CRAY(&type,&num_elm,tmp_dst,&bitoff,dest);
        HDfreespace(tmp_dst);
    	if(ierr!=0)
            HRETURN_ERROR(DFE_BADCONV,FAIL);  /* error in Cray conversion */
      } /* end if */
    else {
#ifdef NOINTCRAY2IEG
        for(i = 0; i < num_elm; i++) {
            if((source[3] & 0x80)) {    /* Can't forget to sign extend */
            	dest[0] = 0xff;
            	dest[1] = 0xff;
            	dest[2] = 0xff;
            	dest[3] = 0xff;
              } /* end if */
            else {
    	        dest[0] = 0;
    	        dest[1] = 0;
    	        dest[2] = 0;
    	        dest[3] = 0;
              } /* end else */
            dest[4] = source[3];
            dest[5] = source[2];
            dest[6] = source[1];
            dest[7] = source[0];
            dest += dest_stride;
            source += source_stride;
          } /* end for */
#else
        tmp_dst=(uint8 *)HDgetspace(4);
        for(i = 0; i < num_elm; i++) {
            DFKswap(source,tmp_dst,4,1);
            ierr=IEG2CRAY(&type,&n_elem,tmp_dst,&bitoff,dest);
        	if(ierr!=0) {
                HDfreespace(tmp_dst);
                HRETURN_ERROR(DFE_BADCONV,FAIL);  /* error in Cray conversion */
              } /* end if */
            dest += dest_stride;
            source += source_stride;
          } /* end for */
        HDfreespace(tmp_dst);
#endif
      } /* end else */
    return(SUCCEED);
}

#define LUO4I_MASKA 0x00000000ff00ff00
#define LUO4I_MASKB 0x0000000000ff00ff
#define LUO4I_MASKC 0xffff0000ffff0000
#define LUO4I_MASKD 0x0000ffff0000ffff

/************************************************************/
/* DFKluo4i()                                                */
/* -->Unicos routine for exporting 4 byte data items        */
/************************************************************/
#ifdef PROTOTYPE
int DFKluo4i(VOIDP s, VOIDP d, uint32 num_elm, uint32 source_stride,
		   uint32 dest_stride)
#else
int DFKluo4i(source, dest, num_elm, source_stride, dest_stride)
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
    unsigned long *lp_dest;
    unsigned long *lp_src;
    char *FUNC="DFKuo4i";
	int ierr;       /* error from IEG2CRAY */
	int type=1;     /* type of conversion to perform 1=integer */
	int bitoff=0;   /* bit offset in the IEEE stream */
    uint8 *tmp_dst; /* temporary buffer to hold byte swapped values */
    intn n_elem=1;  /* the number of elements for stride-based processing */

    HEclear();

    if(source == dest || num_elm == 0) /* Inplace conversions  not permitted */
        HRETURN_ERROR(DFE_BADCONV,FAIL); /* No elements to convert is an error */

    if(source_stride == 0 && dest_stride == 0)
        fast_processing = 1;

    if(fast_processing) {
        tmp_dst=(uint8 *)HDgetspace(4*num_elm);
        ierr=CRAY2IEG(&type,&num_elm,tmp_dst,&bitoff,source);
    	if(ierr!=0) {
            HDfreespace(tmp_dst);
            HRETURN_ERROR(DFE_BADCONV,FAIL);  /* error in Cray conversion */
          } /* end if */
        DFKswap(tmp_dst,dest,4,num_elm);
        HDfreespace(tmp_dst);
      } /* end if */
    else {
#ifdef NOINTCRAY2IEG
        for(i = 0; i < num_elm; i++) {
            dest[0] = source[7];
            dest[1] = source[6];
            dest[2] = source[5];
            dest[3] = source[4];
            dest += dest_stride;
            source += source_stride;
          } /* end for */
#else
        tmp_dst=(uint8 *)HDgetspace(4);
        for(i = 0; i < num_elm; i++) {
            ierr=CRAY2IEG(&type,&n_elem,tmp_dst,&bitoff,source);
        	if(ierr!=0) {
                HDfreespace(tmp_dst);
                HRETURN_ERROR(DFE_BADCONV,FAIL);  /* error in Cray conversion */
              } /* end if */
            DFKswap(tmp_dst,dest,4,num_elm);
            dest += dest_stride;
            source += source_stride;
          } /* end for */
        HDfreespace(tmp_dst);
#endif
      } /* end else */
    return(SUCCEED);
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

#define LUI4F_MASKH  0xffffff7f00000000
#define LUI4F_MASKI  0x00000000ffffff7f

/***************************************************************/
/* DFKlui4f()                                                  */
/* -->Unicos routine for importing little-endian 32 bit floats */
/***************************************************************/

/************************************************************

                     <<<< WARNING >>>>

    The nature of converting between 64 bit floating point
  numbers and 32 bit floating point numbers LOSES PRECISION.
  Taking a number in 64bit cray format, converting to IEEE
  (internal HDF format) and back will round the number at
  about the 7th decimal place.

 ************************************************************/

#ifdef PROTOTYPE
int DFKlui4f(VOIDP s, VOIDP d, uint32 num_elm, uint32 source_stride,
		   uint32 dest_stride)
#else
int DFKlui4f(source, dest, num_elm, source_stride, dest_stride)
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
	int ierr;       /* error from IEG2CRAY */
	int type=2;     /* type of conversion to perform 2=32-bit float */
	int bitoff=0;   /* bit offset in the IEEE stream */
    uint8 *tmp_dst; /* temporary buffer to hold byte swapped values */
    intn n_elem=1;  /* the number of elements for stride-based processing */

    HEclear();

    /* Check for conversion errors */
    if(source == dest || num_elm == 0)  /* Inplace conversions not permitted */
        HRETURN_ERROR(DFE_BADCONV,FAIL);    /* under UNICOS */

    /* Find out if it is OK to use faster array processing */
    if(source_stride == 0 && dest_stride == 0)
        fast_processing = 1;

    if(fast_processing) {
        tmp_dst=(uint8 *)HDgetspace(4*num_elm);
        DFKswap(source,tmp_dst,4,num_elm);
        ierr=IEG2CRAY(&type,&num_elm,tmp_dst,&bitoff,dest);
        HDfreespace(tmp_dst);
    	if(ierr!=0)
            HRETURN_ERROR(DFE_BADCONV,FAIL);  /* error in Cray conversion */
      } /* end if */
    else { /* We end up here if we are doing stride based processing */
#ifdef NOFLOATCRAY2IEG
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
          } /* end for */
#else
        tmp_dst=(uint8 *)HDgetspace(4);
        for(i = 0; i < num_elm; i++) {
            DFKswap(source,tmp_dst,4,1);
            ierr=IEG2CRAY(&type,&n_elem,tmp_dst,&bitoff,dest);
        	if(ierr!=0) {
                HDfreespace(tmp_dst);
                HRETURN_ERROR(DFE_BADCONV,FAIL);  /* error in Cray conversion */
              } /* end if */
            source += source_stride;
            dest += dest_stride;
          } /* end for */
        HDfreespace(tmp_dst);
#endif
      } /* end for */
    return(SUCCEED);
}

#define LUO4F_MASKA 0x8000000000000000
#define LUO4F_MASKB 0x7fff000000000000
#define LUO4F_MASKC 0x00007fffff000000
#define LUO4F_MASKD 0x0000000000800000
#define LUO4F_MASKE 0xff00ff00ff00ff00
#define LUO4F_MASKF 0x00ff00ff00ff00ff
#define LUO4F_MASKG 0xffff0000ffff0000
#define LUO4F_MASKH 0x0000ffff0000ffff

/***************************************************************/
/* DFKluo4f()                                                  */
/* -->Unicos routine for exporting little-endian 32 bit floats */
/***************************************************************/

/************************************************************

                     <<<< WARNING >>>>

    The nature of converting between 64 bit floating point
  numbers and 32 bit floating point numbers LOSES PRECISION.
  Taking a number in 64bit cray format, converting to IEEE
  (internal HDF format) and back will round the number at
  about the 7th decimal place.

 ************************************************************/

#ifdef PROTOTYPE
int DFKluo4f(VOIDP s, VOIDP d, uint32 num_elm, uint32 source_stride,
		   uint32 dest_stride)
#else
int DFKluo4f(source, dest, num_elm, source_stride, dest_stride)
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
	int ierr;       /* error from IEG2CRAY */
	int type=2;     /* type of conversion to perform 2=32-bit float */
	int bitoff=0;   /* bit offset in the IEEE stream */
    uint8 *tmp_dst; /* temporary buffer to hold byte swapped values */
    intn n_elem=1;  /* the number of elements for stride-based processing */

    HEclear();

    /* Check for conversion errors */
    if(source == dest || num_elm == 0) /* Inplace conversions not permitted */
        HRETURN_ERROR(DFE_BADCONV,FAIL);    /* under UNICOS */

    /* Find out if it is OK to use faster array processing */
    if(source_stride == 0 && dest_stride == 0)
        fast_processing = 1;

    if(fast_processing) {
        tmp_dst=(uint8 *)HDgetspace(4*num_elm);
        ierr=CRAY2IEG(&type,&num_elm,tmp_dst,&bitoff,source);
    	if(ierr!=0) {
            HDfreespace(tmp_dst);
            HRETURN_ERROR(DFE_BADCONV,FAIL);  /* error in Cray conversion */
          } /* end if */
        DFKswap(tmp_dst,dest,4,num_elm);
        HDfreespace(tmp_dst);
      } /* end if */
    else { /* We end up here if we are doing stride based processing */
#ifdef NOFLOATCRAY2IEG
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
          } /* end for */
#else
        tmp_dst=(uint8 *)HDgetspace(4);
        for(i = 0; i < num_elm; i++) {
            ierr=CRAY2IEG(&type,&n_elem,tmp_dst,&bitoff,source);
        	if(ierr!=0) {
                HDfreespace(tmp_dst);
                HRETURN_ERROR(DFE_BADCONV,FAIL);  /* error in Cray conversion */
              } /* end if */
            DFKswap(tmp_dst,dest,4,1);
            source += source_stride;
            dest += dest_stride;
          } /* end for */
        HDfreespace(tmp_dst);
#endif
      } /* end else */
    return(SUCCEED);
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
#define LUI8F_MASKL 0xffffffffffffff7f

/************************************************************/
/* DFKlui8f()                                               */
/* -->Unicos routine for importing 64 bit floats            */
/************************************************************/
#ifdef PROTOTYPE
int DFKlui8f(VOIDP s, VOIDP d, uint32 num_elm, uint32 source_stride,
		   uint32 dest_stride)
#else
int DFKlui8f(source, dest, num_elm, source_stride, dest_stride)
uint8 * source, * dest;
uint32 num_elm, source_stride, dest_stride;
#endif /* PROTOTYPE */
{
    int fast_processing = 0;            /* By default not array processed */
    int i,j,n;
    long buf;                           /* This is a temporary stride buf */
    uint8 * dud = (uint8*)&buf;         /* Dummy pointer to buf1 for strides */
#ifdef PROTOTYPE
    uint8 * source = (uint8*)s;         /* Cray does not like certain   */
    uint8 * dest = (uint8*)d;           /* void and void* constructions */
#endif /* PROTOTYPE*/
    long * lptr_src = (long*)source;
    long * lptr_dest = (long*)dest;
    char *FUNC="DFKui8f";
	int ierr;       /* error from IEG2CRAY */
	int type=8;     /* type of conversion to perform 8=64-bit float */
	int bitoff=0;   /* bit offset in the IEEE stream */
    uint8 *tmp_dst; /* temporary buffer to hold byte swapped values */
    intn n_elem=1;  /* the number of elements for stride-based processing */

    HEclear();

    /* Check for conversion errors */
    if(source == dest || num_elm == 0) /* Inplace conversions not permitted */
        HRETURN_ERROR(DFE_BADCONV,FAIL);    /* under UNICOS */

    /* Find out if it is OK to use faster array processing */
    if(source_stride == 0 && dest_stride == 0)
        fast_processing = 1;

    if(fast_processing) {
        tmp_dst=(uint8 *)HDgetspace(8*num_elm);
        DFKswap(source,tmp_dst,8,num_elm);
        ierr=IEG2CRAY(&type,&num_elm,tmp_dst,&bitoff,dest);
        HDfreespace(tmp_dst);
    	if(ierr!=0)
            HRETURN_ERROR(DFE_BADCONV,FAIL);  /* error in Cray conversion */
      } /* end if */
    else {
#ifdef NOFLOATCRAY2IEG
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
          } /* end for */
#else
        tmp_dst=(uint8 *)HDgetspace(8);
        for(i = 0; i < num_elm; i ++) {
            DFKswap(source,tmp_dst,8,1);
            ierr=IEG2CRAY(&type,&n_elem,tmp_dst,&bitoff,dest);
        	if(ierr!=0) {
                HDfreespace(tmp_dst);
                HRETURN_ERROR(DFE_BADCONV,FAIL);  /* error in Cray conversion */
              } /* end if */
            source += source_stride;
            dest += dest_stride;
          } /* end for */
        HDfreespace(tmp_dst);
#endif
      } /* end else */
    return(SUCCEED);
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
int DFKluo8f(VOIDP s, VOIDP d, uint32 num_elm, uint32 source_stride,
		   uint32 dest_stride)
#else
int DFKluo8f(source, dest, num_elm, source_stride, dest_stride)
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
	int ierr;       /* error from IEG2CRAY */
	int type=8;     /* type of conversion to perform 8=64-bit float */
	int bitoff=0;   /* bit offset in the IEEE stream */
    uint8 *tmp_dst; /* temporary buffer to hold byte swapped values */
    intn n_elem=1;  /* the number of elements for stride-based processing */

    HEclear();

    /* Check for conversion errors */
    if(source == dest || num_elm == 0) /* Inplace conversions not permitted */
        HRETURN_ERROR(DFE_BADCONV,FAIL);    /* under UNICOS */

    /* Find out if it is OK to use faster array processing */
    if(source_stride == 0 && dest_stride == 0)
        fast_processing = 1;

    if(fast_processing) {
        tmp_dst=(uint8 *)HDgetspace(8*num_elm);
        ierr=CRAY2IEG(&type,&num_elm,tmp_dst,&bitoff,source);
    	if(ierr!=0) {
            HDfreespace(tmp_dst);
            HRETURN_ERROR(DFE_BADCONV,FAIL);  /* error in Cray conversion */
          } /* end if */
        DFKswap(tmp_dst,dest,8,num_elm);
        HDfreespace(tmp_dst);
      } /* end if */
    else {
#ifdef NOFLOATCRAY2IEG
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
          } /* end for */
#else
        tmp_dst=(uint8 *)HDgetspace(8);
        for(i = 0; i < num_elm; i ++) {
            ierr=CRAY2IEG(&type,&n_elem,tmp_dst,&bitoff,source);
        	if(ierr!=0) {
                HDfreespace(tmp_dst);
                HRETURN_ERROR(DFE_BADCONV,FAIL);  /* error in Cray conversion */
              } /* end if */
            DFKswap(tmp_dst,dest,8,1);
            source += source_stride;
            dest += dest_stride;
          } /* end for */
        HDfreespace(tmp_dst);
#endif
      } /* end else */
    return(SUCCEED);
}

#else /* i.e. not on a cray */

int cray_dummy; /* prevent empty symbol table messages */

#endif /* UNICOS */
