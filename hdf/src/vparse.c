#ifdef RCSID
static char RcsId[] = "@(#)$Revision$";
#endif
/*
$Header$

$Log$
Revision 1.7  1993/08/18 16:00:52  chouck
Restored changes from version 1.5 that had gotten blown away (grumble grumble)

 * Revision 1.6  1993/08/16  21:46:45  koziol
 * Wrapped in changes for final, working version on the PC.
 *
 * Revision 1.5  1993/08/03  15:49:22  chouck
 * Cleaned up a bunch of Vset code
 *
 * Revision 1.4  1993/03/29  16:50:46  koziol
 * Updated JPEG code to new JPEG 4 code.
 * Changed VSets to use Threaded-Balanced-Binary Tree for internal
 * 	(in memory) representation.
 * Changed VGROUP * and VDATA * returns/parameters for all VSet functions
 * 	to use 32-bit integer keys instead of pointers.
 * Backed out speedups for Cray, until I get the time to fix them.
 * Fixed a bunch of bugs in the little-endian support in DFSD.
 *
 * Revision 1.3  1993/01/19  05:56:29  koziol
 * Merged Hyperslab and JPEG routines with beginning of DEC ALPHA
 * port.  Lots of minor annoyances fixed.
 *
 * Revision 1.2  1992/11/02  16:35:41  koziol
 * Updates from 3.2r2 -> 3.3
 *
 * Revision 1.1  1992/08/25  21:40:44  koziol
 * Initial revision
 *
*/
/*****************************************************************************
*
* vparse.c
* Part of the HDF VSet interface.
*
************************************************************************/

#include "vg.h"

#define ISCOMMA(c) ( (c==',') ? 1:0 )

/* ------------------------------------------------------------------ */

/*
** Given a string (attrs) , the routine parses it into token strings,
** and returns a ptr (attrv) to an array of ptrs where the tokens 
** are stored.  The number of tokens are returned in attrc.
**
** Currently used only by routines that manipulate field names.
** As such each field string is truncated to a max length of
** FIELDNAMELENMAX (as defined in hdf.h). For most cases, this
** truncation doesn't happen because FIELDNAMELENMAX is a big number.
**
** RETURN FAIL if error.
** RETURN SUCCEED if ok.
**
** Current implementation: all strings inputs converted to uppercase.    
** tokens must be separated by COMMAs.
**
** Tokens are stored in static area sym , and pointers are returned
** to calling routine. Hence, tokens must be used before next call 
** to scanattrs.
**
*/

PRIVATE char* 	symptr[50];
PRIVATE char    sym[50][FIELDNAMELENMAX+1];
PRIVATE	intn 	nsym;

#ifdef PROTOTYPE
int32 scanattrs (char *attrs, int32 *attrc, char ***attrv)
#else
int32 scanattrs (attrs,attrc,attrv)

	char	*attrs;		/* field string (input) */
	int32	*attrc;		/* # of fields (output) */
	char	***attrv;	/* array of char ptrs to fields (output) */
#endif

{
  register char   *s, *s0, *ss;
  register intn   i, slen, len;
  char * FUNC = "scanattrs";
  char * saved_string = HDstrdup(attrs);
  
  s = saved_string;
  slen = HDstrlen(s);
  nsym = 0;
  
  s0 = s;
  while(*s) {
      if (*s >= 'a' && *s <= 'z') *s=(char)toupper(*s);
      if ( ISCOMMA(*s) ) {

          /* make sure we've got a legitimate length */
          len = (intn)(s - s0);
          if (len <= 0) return(FAIL);
          
          /* save that token */
          ss = symptr[nsym] = sym[nsym]; 
          nsym++;
          
          /* shove the string into our static buffer.  YUCK! */
          if ( len > FIELDNAMELENMAX) len = FIELDNAMELENMAX;
          HIstrncpy(ss, s0, len + 1);

          /* skip over the comma */
          s++;

          /* skip over white space before the next field name */
          while(*s && *s == ' ') s++;
          
          /* keep track of the first character of the next token */
          s0 = s;

      } else {

          /* move along --- nothing to see here */
          s++;
      }
  }  
  
  /* save the last token */
  len = (intn)(s - s0);
  if (len <= 0) return(FAIL);
  ss = symptr[nsym] = sym[nsym]; 
  nsym++;
  
  if ( len > FIELDNAMELENMAX) len = FIELDNAMELENMAX;
  HIstrncpy(ss, s0, len + 1);
  
  symptr[nsym] = NULL;
  *attrc = nsym;
  *attrv = (char**) symptr;
  
  HDfreespace(saved_string);

  return(SUCCEED); /* ok */
  
} /* scanattrs */

/* ------------------------------------------------------------------ */
