/* @(#)types.h  1.2 87/11/07 3.9 RPCSRC */
/*      @(#)types.h 1.18 87/07/24 SMI      */
/*
 * Rpc additions to <sys/types.h>
 */
#ifndef __TYPES_RPC_HEADER__
#define __TYPES_RPC_HEADER__

#define bool_t  int
#define enum_t  int
#ifdef FALSE
#undef FALSE
#endif /* FALSE */
#define FALSE   (0)
#ifdef TRUE
#undef TRUE
#endif /* TRUE */
#define TRUE    (1)
#define __dontcare__    -1
#ifndef NULL
#   define NULL 0
#endif

#if defined __STDC__ || defined PC || defined _WINDOWS || defined _WIN32
#define STDC_INCLUDES
#endif

#ifdef STDC_INCLUDES
#include <stddef.h>
#include <stdlib.h>
#else
extern char *malloc();
#endif

#define mem_alloc(bsize)    malloc(bsize)
#define mem_free(ptr, bsize)    free(ptr)

#if defined unix | defined __unix
#ifndef makedev /* ie, we haven't already included it */
#include <sys/types.h>
#endif
#else /* !unix */
typedef unsigned char   u_char;
typedef unsigned short  u_short;
typedef unsigned int    u_int;
typedef unsigned long   u_long;
typedef char *  caddr_t;
#endif /* !unix */

#endif /* !__TYPES_RPC_HEADER__ */
