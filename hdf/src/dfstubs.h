/*
$Header$

$Log$
Revision 1.3  1993/10/01 20:00:48  koziol
Put "extern C" block around function prototypes for C++ compatibility.

 * Revision 1.2  1993/01/19  05:55:22  koziol
 * Merged Hyperslab and JPEG routines with beginning of DEC ALPHA
 * port.  Lots of minor annoyances fixed.
 *
 * Revision 1.1  1992/08/25  21:40:44  koziol
 * Initial revision
 *
*/
/*
** FILE
**	dfstubs.h
** PURPOSE
**	Header file for "dfstubs.c" HDF 3.1 emulation using new routines
**	from "hfile.c".
** AUTHOR
**	Doug Ilg
*/

#ifndef DFSTUBS_H  /* avoid re-inclusion */
#define DFSTUBS_H
/* This is the master HDF driver (taking the place of df.c), so... */
#define DFMASTER
#undef PERM_OUT  /* used to "comment out" code */

#include "df.h"
#undef DFMASTER

#if !defined(__GNUC__) & !defined(CONVEX) & !defined(VMS) & !defined(PC)
#include <memory.h>
#endif  /* !__GNUC__ & !CONVEX & !VMS & !PC */

#define DFACC_APPEND	8
#define DFEL_ABSENT	0
#define DFEL_RESIDENT	1
#define DFSRCH_OLD	0
#define DFSRCH_NEW	1

PRIVATE int32	DFid;
PRIVATE int32	DFaid;
PRIVATE int	DFaccmode;
PRIVATE int	DFelaccmode;
PRIVATE uint16	search_tag;
PRIVATE uint16	search_ref;
PRIVATE int	search_stat	= DFSRCH_NEW;
PRIVATE int32	search_aid	= 0;
PRIVATE int	DFelstat	= DFEL_ABSENT;
PRIVATE int32	DFelsize	= 0;
PRIVATE int32	DFelseekpos	= 0;
PRIVATE uint16	acc_tag		= 0;
PRIVATE uint16	acc_ref		= 0;
PRIVATE char	*DFelement	= NULL;

extern DF *makedf();

#if defined c_plusplus || defined __cplusplus
extern "C" {
#endif /* c_plusplus || __cplusplus */

/* prototypes for internal routines */
PRIVATE int DFIclearacc
   PROTO((void));

PRIVATE int DFIcheck
   PROTO((DF *dfile));

#if defined c_plusplus || defined __cplusplus
}
#endif /* c_plusplus || __cplusplus */

#endif /* DFSTUBS_H */

