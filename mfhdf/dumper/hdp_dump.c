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
fmtbyte(unsigned char *x, FILE * ofp)
{
	return (fprintf(ofp, "%02x ", (unsigned) *x));
}

intn 
fmtint8(VOIDP x, FILE * ofp)
{
	return (fprintf(ofp, "%d", (int) *((signed char *) x)));
}

intn 
fmtuint8(VOIDP x, FILE * ofp)
{
	return (fprintf(ofp, "%u", (unsigned) *((unsigned char *) x)));
}

intn 
fmtint16(VOIDP x, FILE * ofp)
{
	int16       s;

	HDmemcpy(&s, x, sizeof(int16));
	return (fprintf(ofp, "%d", (int) s));
}

intn 
fmtuint16(VOIDP x, FILE * ofp)
{
	uint16      s;

	HDmemcpy(&s, x, sizeof(uint16));
	return (fprintf(ofp, "%u", (unsigned) s));
}

intn 
fmtchar(VOIDP x, FILE * ofp)
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
fmtuchar8(VOIDP x, FILE * ofp)
{
	return (1 + fprintf(ofp, "%o", *((uchar8 *) x)));
}

intn 
fmtint(VOIDP x, FILE * ofp)
{
	intn        i;

	HDmemcpy(&i, x, sizeof(intn));
	return (fprintf(ofp, "%d", (int) i));
}

#define FLOAT32_EPSILON ((float32)1.0e-20)
intn 
fmtfloat32(VOIDP x, FILE * ofp)
{
	float32     f;

	HDmemcpy(&f, x, sizeof(float32));
	if (fabs(f - FILL_FLOAT) <= FLOAT32_EPSILON)
		return (fprintf(ofp, "FloatInf"));
	else
		return (fprintf(ofp, "%f", f));
}

intn 
fmtint32(VOIDP x, FILE * ofp)
{
	int32       l;

	HDmemcpy(&l, x, sizeof(int32));
	return (fprintf(ofp, "%ld", (long) l));
}

intn 
fmtuint32(VOIDP x, FILE * ofp)
{
	uint32      l;

	HDmemcpy(&l, x, sizeof(uint32));
	return (fprintf(ofp, "%lu", (unsigned long) l));
}

intn 
fmtshort(VOIDP x, FILE * ofp)
{
	short       s;

	HDmemcpy(&s, x, sizeof(short));
	return (fprintf(ofp, "%d", (int) s));
}

#define FLOAT64_EPSILON ((float64)1.0e-20)
intn 
fmtfloat64(VOIDP x, FILE * ofp)
{
	float64     d;

	HDmemcpy(&d, x, sizeof(float64));
	if (fabs(d - FILL_DOUBLE) <= FLOAT64_EPSILON)
		return (fprintf(ofp, "DoubleInf"));
	else
		return (fprintf(ofp, "%f", d));
}

int32 
dumpfull(int32 nt, int32 cnt, VOIDP databuf, intn indent, FILE * ofp)
{
	intn        i;
	VOIDP       b;
	intn        (*fmtfunct) (VOIDP, FILE *) = NULL;
	int32       off;
	intn        cn;

	switch (nt)
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
			  break;
	  }		/* end switch */
	b = databuf;
	off = DFKNTsize(nt | DFNT_NATIVE);
	cn = indent;
	if (nt != DFNT_CHAR)
	  {
		  for (i = 0; i < cnt; i++)
			{
				cn += fmtfunct(b, ofp);
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
				cn += fmtfunct(b, ofp);
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
	return (0);
}
