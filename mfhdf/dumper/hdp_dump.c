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

#include <stdio.h>
#include "mfhdf.h"
#include "hdp.h"
#include <ctype.h>
#ifndef MIPSEL
#include <math.h>
#endif /* MIPSEL */

/* 
 * printing functions copied from vshow.c and used by sdsdumpfull(). 
 *
 * Please pay attention to the data types used in the print/output routines.
 * Make sure the data type being dumped matches arguments in 'fwrite()' .etc.
 *
 */

intn 
fmtbyte(unsigned char *x, /* assumption: byte is the same as unsigned char */
        file_type_t    ft, 
        FILE          *ofp)
{
    unsigned char s;

    if(ft == DASCII)
      return (fprintf(ofp, "%02x ", (unsigned) *x));
    else
      { 
          s = (unsigned char) *x;
          return(fwrite(&s, sizeof(unsigned char),1,ofp));
      }
}

intn 
fmtint8(VOIDP       x,  /* assumption: int8 is same as signed char */
        file_type_t ft, 
        FILE       *ofp)
{
    int8 s;

    if(ft == DASCII)
        return (fprintf(ofp, "%d", (int) *((signed char *) x)));
    else
      {
          s = (int8) *((signed char *) x);
          return(fwrite(&s, sizeof(int8), 1, ofp));
      }
}

intn 
fmtuint8(VOIDP       x, /* assumption: uint8 is same as unsigned char */
         file_type_t ft, 
         FILE       *ofp)
{
    uint8 s;

    if(ft == DASCII)
        return (fprintf(ofp, "%u", (unsigned) *((unsigned char *) x)));
    else
      { 
          s = (uint8) *((unsigned char *) x);
          return(fwrite(&s, sizeof(uint8), 1, ofp));
      } 
}

intn 
fmtint16(VOIDP       x, 
         file_type_t ft, 
         FILE       *ofp)
{
    int16 s;

    HDmemcpy(&s, x, sizeof(int16));

    if(ft == DASCII)
        return (fprintf(ofp, "%d", (int) s));
    else
        return(fwrite(&s, sizeof(int16), 1, ofp));
}

intn 
fmtuint16(VOIDP       x, 
          file_type_t ft, 
          FILE       *ofp)
{
    uint16      s;

    HDmemcpy(&s, x, sizeof(uint16));

    if(ft == DASCII)
        return (fprintf(ofp, "%u", (unsigned) s));
    else
        return(fwrite(&s, sizeof(uint16), 1, ofp));
}

intn 
fmtchar(VOIDP       x, 
        file_type_t ft, 
        FILE       *ofp)
{
    if (isprint(*(unsigned char *) x))
      {
          putc(*((char *) x), ofp);
          return (1);
      }		
    else
      {
          putc('\\', ofp);
          return (1 + fprintf(ofp, "%03o", *((uchar8 *) x)));
      }		
}

intn 
fmtuchar8(VOIDP       x, /* assumption: uchar8 is same as unsigned char */
          file_type_t ft, 
          FILE       *ofp)
{   
    uchar8 s;

    if(ft == DASCII) 
        return (1 + fprintf(ofp, "%o", *((uchar8 *) x)));
    else
      { 
          s = (uchar8) *((unsigned char *)x);
          return(fwrite(&s, sizeof(uchar8),1, ofp));
      }
}

intn 
fmtint(VOIDP       x, /* assumption: int is same as 'intn' */
       file_type_t ft, 
       FILE       *ofp)
{
    intn        i;

    HDmemcpy(&i, x, sizeof(intn));

    if(ft == DASCII) 
        return (fprintf(ofp, "%d", (int) i));
    else
        return(fwrite(&i, sizeof(intn), 1, ofp));
}

#define FLOAT32_EPSILON ((float32)1.0e-20)
intn 
fmtfloat32(VOIDP       x, 
           file_type_t ft, 
           FILE       *ofp)
{
    float32     fdata;

    HDmemcpy(&fdata, x, sizeof(float32));

    if(ft == DASCII)
      {
          if (fabs(fdata - FILL_FLOAT) <= FLOAT32_EPSILON)
              return (fprintf(ofp, "FloatInf"));
          else
              return (fprintf(ofp, "%f", fdata));
      }
    else
      {
          return(fwrite(&fdata, sizeof(float32), 1, ofp));
      }
}

intn 
fmtint32(VOIDP       x, 
         file_type_t ft, 
         FILE       *ofp)
{
    int32       l;

    HDmemcpy(&l, x, sizeof(int32));

    if(ft == DASCII)
        return (fprintf(ofp, "%ld", (long) l));
    else
        return(fwrite(&l, sizeof(int32), 1, ofp));
}

intn 
fmtuint32(VOIDP       x, 
          file_type_t ft, 
          FILE       *ofp)
{
    uint32      l;

    HDmemcpy(&l, x, sizeof(uint32));

    if(ft == DASCII)
        return (fprintf(ofp, "%lu", (unsigned long) l));
    else
        return(fwrite(&l, sizeof(uint32), 1, ofp));
}

intn 
fmtshort(VOIDP       x, 
         file_type_t ft, 
         FILE       *ofp)
{
    short s;

    HDmemcpy(&s, x, sizeof(short));

    if(ft == DASCII)
        return (fprintf(ofp, "%d", (int) s));
    else
        return(fwrite(&s, sizeof(short), 1, ofp));
}

#define FLOAT64_EPSILON ((float64)1.0e-20)
intn 
fmtfloat64(VOIDP       x, 
           file_type_t ft, 
           FILE       *ofp)
{
    float64     d;

    HDmemcpy(&d, x, sizeof(float64));

    if(ft == DASCII)
      {
          if (fabs(d - FILL_DOUBLE) <= FLOAT64_EPSILON)
              return (fprintf(ofp, "DoubleInf"));
          else
             return (fprintf(ofp, "%f", d));
      }
    else
      {
          return(fwrite(&d, sizeof(float64), 1, ofp));
      }
}

int32 
dumpfull(int32       nt, 
         file_type_t ft, 
         int32       cnt,     /* number of items in 'databuf' ? */
         VOIDP       databuf, 
         intn        indent, 
         FILE       *ofp)
{
    intn    i;
    VOIDP   b = NULL;
    intn    (*fmtfunct) (VOIDP, file_type_t, FILE *) = NULL;
    int32   off;
    intn    cn;
    intn    ret_value = SUCCEED;

    /* check inputs */
    if (NULL == databuf || NULL == ofp)
      {
          ret_value = FAIL;
          goto done;
      }
    
    switch (nt & 0xff )
      {
      case DFNT_CHAR:
          fmtfunct = fmtchar;
          break;
      case DFNT_UCHAR:
          fmtfunct = fmtuchar8;
          break;
      case DFNT_UINT8:
          fmtfunct = fmtuint8;
          break;
      case DFNT_INT8:
          fmtfunct = fmtint8;
          break;
      case DFNT_UINT16:
          fmtfunct = fmtuint16;
          break;
      case DFNT_INT16:
          fmtfunct = fmtint16;
          break;
      case DFNT_UINT32:
          fmtfunct = fmtuint32;
          break;
      case DFNT_INT32:
          fmtfunct = fmtint32;
          break;
      case DFNT_FLOAT32:
          fmtfunct = fmtfloat32;
          break;
      case DFNT_FLOAT64:
          fmtfunct = fmtfloat64;
          break;
      default:
          fprintf(ofp, "HDP does not support type [%d] \n", (int) nt);
          ret_value = FAIL;
          goto done;
      }		/* end switch */

    /* assign to variables used in loop below */
    b = databuf;
    off = DFKNTsize(nt | DFNT_NATIVE); /* what is offset for data type */
    if (off == FAIL){
	fprintf(ofp, "Failed to find native size of type [%d] \n", (int)nt);
        ret_value = FAIL;
        goto done;
    }
    cn = indent;

    /* check if were are dumping data i.e. items in buffer in 
       ASCII or Binary mode. */
    if(ft == DASCII)
      { /* ASCII */
          if (nt != DFNT_CHAR)
            {
                for (i = 0; i < cnt && b != NULL; i++)
                  {
                      cn += fmtfunct(b, ft, ofp); /* dump item to file */
                      b = (char *) b + off;
                      putc(' ', ofp);
                      cn++;
                      /* temporary fix bad alignment algo in dumpfull by
                         adding i < cnt-1 to remove extra line - BMR */
                      if (cn > 65 && i < cnt-1 )
                        {
                            putc('\n', ofp);

                            for (cn = 0; cn < indent; cn++)
                                putc(' ', ofp);
                        }		/* end if */
                  }	/* end for every item in buffer */
            }		
          else /* DFNT_CHAR */
            {
                for (i = 0; i < cnt && b != NULL; i++)
                  {
                      cn += fmtfunct(b, ft, ofp); /* dump item to file */
                      b = (char *) b + off;
                      if (cn > 65)
                        {
                            putc('\n', ofp);
                            for (cn = 0; cn < indent; cn++)
                                putc(' ', ofp);
                        }		/* end if */
                  }	/* end for every item in buffer */
            }		/* end else DFNT_CHAR */

          putc('\n', ofp); /* newline */

      } /* end DASCII  */
    else /*  Binary   */
      {
          for (i = 0; i < cnt && b != NULL; i++)
            {
                cn += fmtfunct(b, ft, ofp); /* dump item to file */
                b = (char *) b + off; /* increment by offset? */
                cn++;
            } /* end for all items in buffer */
      }

done:
    if (ret_value == FAIL)
      { /* Failure cleanup */
      }
    /* Normal cleanup */

    return ret_value;
} /* dumpfull */
