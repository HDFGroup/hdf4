/* $Id$ */
#ifndef _ALLOC_H_
#define _ALLOC_H_

#ifndef NO_STDLIB
#include <stdlib.h>
#else
extern char *malloc();
extern char *realloc();
#ifndef NULL
#define NULL  0
#endif /* !NULL */
#endif /* !NO_STDLIB */


#define Alloc(theNum, theType) \
	(theType *)malloc(sizeof(theType) * (theNum)) 


#ifndef NO_STDLIB
#define Free(ptr)		free(ptr)
#else
/* old style free */
#define Free(ptr)		(void)free((char *)ptr)
#endif /* !NO_STDLIB */

#define ARRAYLEN(arr) (sizeof(arr)/sizeof(arr[0]))

#endif /* !_ALLOC_H_ */
