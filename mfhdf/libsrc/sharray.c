/*
 *	Copyright 1993, University Corporation for Atmospheric Research
 *      See netcdf/COPYRIGHT file for copying and redistribution conditions.
 */
/*	$Id$ */

#include	"local_nc.h"
#include	"alloc.h"


/*
 * Translate an array of cnt short integers at sp.
 *
 * N.B.:
 * Alignment constraints may recommend alternative solutions
 *
 * Example implementation of ntohs() and htons() can be found in
 *	 ../xdr/byteorder.c
 */
bool_t
xdr_shorts(xdrs, sp, cnt)
	XDR *xdrs;
	short *sp;
	u_int cnt ;
{
#ifndef BIG_SHORTS
	short *bp ;
	u_char buf[4] ;
	int rem = cnt % 2 ;
	bool_t stat = TRUE ;

	bp = (short *)buf ;
	cnt /= 2 ;

	if( xdrs->x_op == XDR_DECODE)
	{
		for( ; (cnt > 0) && stat ; cnt--)
		{
			stat = XDR_GETBYTES(xdrs, buf, 4) ;
#ifdef SWAP
			*sp++ = ntohs(*bp++) ;
			*sp++ = ntohs(*bp--) ;
#else /* SWAP */
			*sp++ = *bp++ ;
			*sp++ = *bp-- ;
#endif /* SWAP */
		}
		if(stat && rem) /* remainder */
		{
			stat = XDR_GETBYTES(xdrs, buf, 4) ;
#ifdef SWAP
			*sp = ntohs(*bp) ;
#else /* SWAP */
			*sp = *bp ;
#endif /* SWAP */
		}
		return(stat) ;
	}
	if( xdrs->x_op == XDR_ENCODE)
	{
		for(  ; (cnt > 0) && stat ; cnt--)
		{
#ifdef SWAP
			*bp++ = htons(*sp++) ;
			*bp-- = htons(*sp++) ;
#else /* SWAP */
			*bp++ = *sp++ ;
			*bp-- = *sp++ ;
#endif /* SWAP */
			stat = XDR_PUTBYTES(xdrs, buf, 4) ;
		}
		if(stat && rem) /* remainder */
		{
#ifdef SWAP
			*bp++ = htons(*sp) ;
#else /* SWAP */
			*bp++ = *sp ;
#endif /* SWAP */
			*bp = 0 ;
			stat = XDR_PUTBYTES(xdrs, buf, 4) ;
		}
		return(stat) ;
	}
	return(FALSE) ;
#else
	/* BIG_SHORTS */
	struct {
		unsigned pad : 32 ;
		unsigned hs: 1 ;
		unsigned hi: 15 ;
		unsigned ls: 1 ;
		unsigned lo: 15 ;
	} buf;
	int rem = cnt % 2 ;
	bool_t stat = TRUE ;

	cnt /= 2 ;

	if( xdrs->x_op == XDR_DECODE)
	{
		for( ; (cnt > 0) && stat ; cnt--)
		{
			stat = XDR_GETLONG(xdrs, &buf) ;
			*sp++ = buf.hs ? -32768 + buf.hi : buf.hi ;
#if DEBUGC
			fprintf(stderr, "\t%d\n", *(sp-1)) ;
#endif
			*sp++ = buf.ls ? -32768 + buf.lo : buf.lo ;
#if DEBUGC
			fprintf(stderr, "\t%d\n", *(sp-1)) ;
#endif
		}
		if(stat && rem) /* remainder */
		{
			stat = XDR_GETLONG(xdrs, &buf) ;
			*sp = buf.hs ? -32768 + buf.hi : buf.hi ;
#if DEBUGC
			fprintf(stderr, "\t%d\n", *sp) ;
#endif
		}
		return(stat) ;
	}
	if( xdrs->x_op == XDR_ENCODE)
	{
#ifdef DEBUGC
	{ int ii; for(ii = 0 ; ii< 2*cnt ; ii++)
		fprintf(stderr, "%d: %d\n", ii, sp[ii]) ; }
#endif
		for(  ; (cnt > 0) && stat ; cnt--)
		{
			if( *sp < 0 )
			{
				buf.hs = 1 ;
				buf.hi = *sp + 32768 ;
			}  else {
				buf.hs = 0 ;
				buf.hi = *sp ;
			}
			sp++ ;
			if( *sp < 0 )
			{
				buf.ls = 1 ;
				buf.lo = *sp + 32768 ;
			}  else {
				buf.ls = 0 ;
				buf.lo = *sp ;
			}
			sp++ ;
#ifdef DEBUGC
		fprintf(stderr, "%d %d : %d\n", 
			 buf.hi, buf.lo, sizeof(buf)) ;
#endif
			stat = XDR_PUTLONG(xdrs, &buf) ;
		}
		if(stat && rem) /* remainder */
		{
			if( *sp < 0 )
			{
				buf.hs = 1 ;
				buf.hi = *sp + 32768 ;
			}  else {
				buf.hs = 0 ;
				buf.hi = *sp ;
			}
			stat = XDR_PUTLONG(xdrs, &buf) ;
		}
		return(stat) ;
	}
	return(FALSE) ;
#endif /* !BIG_SHORTS */
}
