/* Copyright (c) 1998, 1999, 2000, 2004, 2005 Free Software Foundation, Inc.
   This file is part of the GNU C Library.
   Contributed by Thorsten Kukuk <kukuk@vt.uni-paderborn.de>, 1998.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, write to the Free
   Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
   02111-1307 USA.  */

#include "H4api_adpt.h"

#include "types.h"
#include "xdr.h"

/* XDR 64bit integers */
bool_t
xdr_int64_t (XDR *xdrs, int64_t *ip)
{
  long t1, t2;

  switch (xdrs->x_op)
    {
    case XDR_ENCODE:
      t1 = (int32_t) ((*ip) >> 32);
      t2 = (int32_t) (*ip);
      return (XDR_PUTINT32(xdrs, &t1) && XDR_PUTINT32(xdrs, &t2));
    case XDR_DECODE:
      if (!XDR_GETINT32(xdrs, &t1) || !XDR_GETINT32(xdrs, &t2))
        return FALSE;
      *ip = ((int64_t) t1) << 32;
      *ip |= (uint32_t) t2;	/* Avoid sign extension.  */
      return TRUE;
    case XDR_FREE:
      return TRUE;
    default:
      return FALSE;
    }
}

/* XDR 64bit unsigned integers */
bool_t
xdr_uint64_t (XDR *xdrs, uint64_t *uip)
{
  long t1;
  long t2;

  switch (xdrs->x_op)
    {
    case XDR_ENCODE:
      t1 = (uint32_t) ((*uip) >> 32);
      t2 = (uint32_t) (*uip);
      return (XDR_PUTINT32 (xdrs, &t1) &&
	      XDR_PUTINT32(xdrs, &t2));
    case XDR_DECODE:
      if (!XDR_GETINT32(xdrs, &t1) ||
	  !XDR_GETINT32(xdrs, &t2))
        return FALSE;
      *uip = ((uint64_t) t1) << 32;
      *uip |= t2;
      return TRUE;
    case XDR_FREE:
      return TRUE;
    default:
      return FALSE;
    }
}

/* XDR 32bit integers */
bool_t
xdr_int32_t (XDR *xdrs, int32_t *lp)
{
  long t;

  switch (xdrs->x_op)
    {
    case XDR_ENCODE:
      t = *lp;
      return XDR_PUTINT32 (xdrs, &t);
    case XDR_DECODE:
      if (!XDR_GETINT32 (xdrs, &t))
        return FALSE;
      *lp = t;
      return TRUE;
    case XDR_FREE:
      return TRUE;
    default:
      return FALSE;
    }
}

/* XDR 32bit unsigned integers */
bool_t
xdr_uint32_t (XDR *xdrs, uint32_t *ulp)
{
  long t;

  switch (xdrs->x_op)
    {
    case XDR_ENCODE:
      t = *ulp;
      return XDR_PUTINT32 (xdrs, &t);
    case XDR_DECODE:
      if (!XDR_GETINT32 (xdrs, &t))
        return FALSE;
      *ulp = t;
    case XDR_FREE:
      return TRUE;
    default:
      return FALSE;
    }
}

/* XDR 16bit integers */
bool_t
xdr_int16_t (XDR *xdrs, int16_t *ip)
{
  long t;

  switch (xdrs->x_op)
    {
    case XDR_ENCODE:
      t = (int32_t) *ip;
      return XDR_PUTINT32 (xdrs, &t);
    case XDR_DECODE:
      if (!XDR_GETINT32 (xdrs, &t))
	return FALSE;
      *ip = (int16_t) t;
      return TRUE;
    case XDR_FREE:
      return TRUE;
    default:
      return FALSE;
    }
}

/* XDR 16bit unsigned integers */
bool_t
xdr_uint16_t (XDR *xdrs, uint16_t *uip)
{
  long ut;

  switch (xdrs->x_op)
    {
    case XDR_ENCODE:
      ut = (uint32_t) *uip;
      return XDR_PUTINT32 (xdrs, &ut);
    case XDR_DECODE:
      if (!XDR_GETINT32 (xdrs, &ut))
	return FALSE;
      *uip = (uint16_t) ut;
      return TRUE;
    case XDR_FREE:
      return TRUE;
    default:
      return FALSE;
    }
}

/* XDR 8bit integers */
bool_t
xdr_int8_t (XDR *xdrs, int8_t *ip)
{
  long t;

  switch (xdrs->x_op)
    {
    case XDR_ENCODE:
      t = (int32_t) *ip;
      return XDR_PUTINT32 (xdrs, &t);
    case XDR_DECODE:
      if (!XDR_GETINT32 (xdrs, &t))
	return FALSE;
      *ip = (int8_t) t;
      return TRUE;
    case XDR_FREE:
      return TRUE;
    default:
      return FALSE;
    }
}

/* XDR 8bit unsigned integers */
bool_t
xdr_uint8_t (XDR *xdrs, uint8_t *uip)
{
  long ut;

  switch (xdrs->x_op)
    {
    case XDR_ENCODE:
      ut = (uint32_t) *uip;
      return XDR_PUTINT32 (xdrs, &ut);
    case XDR_DECODE:
      if (!XDR_GETINT32 (xdrs, &ut))
	return FALSE;
      *uip = (uint8_t) ut;
      return TRUE;
    case XDR_FREE:
      return TRUE;
    default:
      return FALSE;
    }
}
