#ifdef RCSID
static char RcsId[] = "@(#)$Revision$";
#endif
/*
$Header$

$Log$
Revision 1.3  1993/01/19 05:56:29  koziol
Merged Hyperslab and JPEG routines with beginning of DEC ALPHA
port.  Lots of minor annoyances fixed.

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
** are stored.  The no of tokens are returned in attrc.
**
** Currently used only by routines that manipulate field names.
** As such each field string is truncated to a max length of
** FIELDNAMELENMAX (as defined in vg.h). For most cases, this
** truncation doesn't happen because FIELDNAMELENMAX is a big number.
**
** RETURN -1 if error.
** RETURN 1 if ok.
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
  
  s = attrs;
  slen = HDstrlen(s);
  nsym = 0;
  
  s0 = s;
  for (i = 0; i < slen; i++, s++)
    if ( ISCOMMA(*s) ) {
      len = s - s0; 
      if (len <= 0) return(FAIL);

      /* save that token */
      ss = symptr[nsym] = sym[nsym]; 
      nsym++;
      
      if ( len > FIELDNAMELENMAX) len = FIELDNAMELENMAX;
      HIstrncpy(ss, s0, len + 1);
      s0 = s+1;
    }
  
  /* save the last token */
  len = s - s0; 
  if (len <= 0) return(FAIL);
  ss = symptr[nsym] = sym[nsym]; 
  nsym++;
  
  if ( len > FIELDNAMELENMAX) len = FIELDNAMELENMAX;
  HIstrncpy(ss, s0, len + 1);
  
  /* convert all fields tokens to uppercase */
  for (i = 0; i < nsym; i++) {
    s = symptr[i];
    while(*s != '\0') {
      if (*s >= 'a' && *s <= 'z') *s=(char)toupper(*s);
      s++;
    }
  }
  
  symptr[nsym] = NULL;
  *attrc = nsym;
  *attrv = (char**) symptr;
  
  return(SUCCEED); /* ok */
  
} /* scanattrs */

/* ------------------------------------------------------------------ */
