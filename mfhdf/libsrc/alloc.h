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


#ifdef HDF
#define Alloc(theNum, theType) \
	(theType *)HDgetspace(sizeof(theType) * (theNum))
#else
#define Alloc(theNum, theType) \
	(theType *)malloc(sizeof(theType) * (theNum))
#endif


#ifndef NO_STDLIB
#ifdef HDF
#define Free(ptr)		HDfreespace((VOIDP)ptr)
#else
#define Free(ptr)		free(ptr)
#endif
#else
/* old style free */
#ifdef HDF
#define Free(ptr)		(void)HDfreespace((char *)ptr)
#else
#define Free(ptr)		(void)free((char *)ptr)
#endif
#endif /* !NO_STDLIB */

#define ARRAYLEN(arr) (sizeof(arr)/sizeof(arr[0]))

#endif /* !_ALLOC_H_ */
