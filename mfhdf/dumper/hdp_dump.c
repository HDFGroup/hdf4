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

/* --------------------------------------------------------- 
   * printing functions copied from vshow.c and used by sdsdumpfull(). 
 */

intn 
fmtbyte(unsigned char *x, file_type_t ft, FILE * ofp)
{
    return (fprintf(ofp, "%02x ", (unsigned) *x));
}

intn 
fmtint8(VOIDP x, file_type_t ft, FILE * ofp)
{
    int s,k;
    if(ft==DASCII)
    return (fprintf(ofp, "%d", (int) *((signed char *) x)));
    else
      {s= (int) *((signed char *) x);
       fwrite(&s, sizeof(int), 1, ofp);
      }
}

intn 
fmtuint8(VOIDP x, file_type_t ft, FILE * ofp)
{
    unsigned s;
    if(ft==DASCII)
    return (fprintf(ofp, "%u", (unsigned) *((unsigned char *) x)));
    else
      { s= (unsigned) *((unsigned char *) x);
        fwrite(&s, sizeof(unsigned), 1, ofp);
      } 
}

intn 
fmtint16(VOIDP x, file_type_t ft, FILE * ofp)
{
    int16       s;

    HDmemcpy(&s, x, sizeof(int16));
    if(ft==DASCII)
    return (fprintf(ofp, "%d", (int) s));
    else
    return(fwrite(&s, sizeof(int), 1, ofp));
}

intn 
fmtuint16(VOIDP x, file_type_t ft, FILE * ofp)
{
    uint16      s;

    HDmemcpy(&s, x, sizeof(uint16));
    if(ft==DASCII)
    return (fprintf(ofp, "%u", (unsigned) s));
    else
    return(fwrite(&s, sizeof(unsigned), 1, ofp));
}

intn 
fmtchar(VOIDP x, file_type_t ft, FILE * ofp)
{
    if (isprint(*(unsigned char *) x))
      {
          putc(*((char *) x), ofp);
          return (1);
      }		/* end if */
    else
      {
          putc('\\', ofp);
          return (1 + fprintf(ofp, "%03o", *((uchar8 *) x)));
      }		/* end else */
}

intn 
fmtuchar8(VOIDP x, file_type_t ft, FILE * ofp)
{
    return (1 + fprintf(ofp, "%o", *((uchar8 *) x)));
}

intn 
fmtint(VOIDP x, file_type_t ft, FILE * ofp)
{
    intn        i;

    HDmemcpy(&i, x, sizeof(intn));
    if(ft==DASCII) 
    return (fprintf(ofp, "%d", (int) i));
    else
    return(fwrite(&i, sizeof(int), 1, ofp));
}

#define FLOAT32_EPSILON ((float32)1.0e-20)
intn 
fmtfloat32(VOIDP x, file_type_t ft, FILE * ofp)
{
    float32     fdata;
    int k;

    HDmemcpy(&fdata, x, sizeof(float32));
    if (fabs(fdata - FILL_FLOAT) <= FLOAT32_EPSILON)
        return (fprintf(ofp, "FloatInf"));
    else
     {if(ft==DASCII)
       return (fprintf(ofp, "%f", fdata));
      else
       {
       return(fwrite(&fdata, sizeof(float32), 1, ofp));
       }
     }
}

intn 
fmtint32(VOIDP x, file_type_t ft, FILE * ofp)
{
    int32       l;

    HDmemcpy(&l, x, sizeof(int32));
    if(ft==DASCII)
    return (fprintf(ofp, "%ld", (long) l));
    else
    return(fwrite(&l, sizeof(int32), 1, ofp));
}

intn 
fmtuint32(VOIDP x, file_type_t ft, FILE * ofp)
{
    uint32      l;

    HDmemcpy(&l, x, sizeof(uint32));
    if(ft==DASCII)
    return (fprintf(ofp, "%lu", (unsigned long) l));
    else
    return(fwrite(&l, sizeof(unsigned long), 1, ofp));
}

intn 
fmtshort(VOIDP x, file_type_t ft, FILE * ofp)
{
    short       s;

    HDmemcpy(&s, x, sizeof(short));
    if(ft==DASCII)
    return (fprintf(ofp, "%d", (int) s));
    else
    return(fwrite(&s, sizeof(short), 1, ofp));
}

#define FLOAT64_EPSILON ((float64)1.0e-20)
intn 
fmtfloat64(VOIDP x, file_type_t ft, FILE * ofp)
{
    float64     d;

    HDmemcpy(&d, x, sizeof(float64));
    if (fabs(d - FILL_DOUBLE) <= FLOAT64_EPSILON)
        return (fprintf(ofp, "DoubleInf"));
    else
     {if(ft==DASCII)
        return (fprintf(ofp, "%f", d));
      else
        return(fwrite(&d, sizeof(float64), 1, ofp));
     }
}

int32 
dumpfull(int32 nt, file_type_t ft, int32 cnt, VOIDP databuf, intn indent, FILE * ofp)
{
    intn        i;
    VOIDP       b;
    intn        (*fmtfunct) (VOIDP, file_type_t, FILE *) = NULL;
    int32       off;
    intn        cn;

    switch (nt & 0xff )
      {
      case DFNT_CHAR:
        if(ft==DASCII)
          fmtfunct = fmtchar;
          break;
      case DFNT_UCHAR:
        if(ft==DASCII)
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
          break;
      }		/* end switch */
    b = databuf;
    off = DFKNTsize(nt | DFNT_NATIVE);
    cn = indent;

    if(ft==DASCII)
     {
    if (nt != DFNT_CHAR)
      {
          for (i = 0; i < cnt; i++)
            {
                cn += fmtfunct(b, ft, ofp);
                b = (char *) b + off;
                putc(' ', ofp);
                cn++;
                if (cn > 65)
                  {
                      putc('\n', ofp);
                      for (cn = 0; cn < indent; cn++)
                          putc(' ', ofp);
                  }		/* end if */
            }	/* end for */
      }		/* end if */
    else
            /* DFNT_CHAR */
      {
          for (i = 0; i < cnt; i++)
            {
                cn += fmtfunct(b, ft, ofp);
                b = (char *) b + off;
                if (cn > 65)
                  {
                      putc('\n', ofp);
                      for (cn = 0; cn < indent; cn++)
                          putc(' ', ofp);
                  }		/* end if */
            }	/* end for */
      }		/* end else */
    putc('\n', ofp);
     }       /* end DASCII  */
    else         /*  binary   */
     {
         for (i = 0; i < cnt; i++)
            {
                cn += fmtfunct(b, ft, ofp);
                b = (char *) b + off;
                cn++;
            }
     }

    return (0);
}
