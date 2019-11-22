/* @(#)types.h    2.3 88/08/15 4.0 RPCSRC */
/*      @(#)types.h 1.18 87/07/24 SMI      */
/*
 * Rpc additions to <sys/types.h>
 */
#ifndef __TYPES_RPC_HEADER__
#define __TYPES_RPC_HEADER__

#ifdef H4_HAVE_STDINT_H
#include <stdint.h>
#endif

#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif

#define    bool_t    int
#define    enum_t    int

#define __dontcare__    -1

#ifndef NULL
#    define NULL 0
#endif

#ifdef H4_HAVE_STDDEF_H
#include <stddef.h>
#endif
#ifdef H4_HAVE_STDLIB_H
#include <stdlib.h>
#else
extern char *malloc();
#endif

#define mem_alloc(bsize)    malloc(bsize)
#define mem_free(ptr, bsize)    free(ptr)

#ifdef H4_HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif

#ifndef u_char
typedef unsigned char u_char;
#endif
#ifndef u_short
typedef uint16_t      u_short;
#endif
#ifndef u_int
typedef uint32_t      u_int;
#endif
#ifndef u_long
typedef unsigned long u_long;
#endif
#ifndef u_quad_t
typedef uint64_t      u_quad_t;
#endif
#ifndef quad_t
typedef int64_t       quad_t;
#endif
#ifndef caddr_t
typedef char *        caddr_t;
#endif

#ifdef __CYGWIN32__
#include <time.h>
#endif
#ifdef H4_HAVE_SYS_TIME_H
#include <sys/time.h>
#endif

#ifndef INADDR_LOOPBACK
#define       INADDR_LOOPBACK         (u_long)0x7F000001
#endif
#ifndef MAXHOSTNAMELEN
#define        MAXHOSTNAMELEN  64
#endif

#endif /* ndef __TYPES_RPC_HEADER__ */
