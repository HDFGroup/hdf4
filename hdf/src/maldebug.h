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

/* $Id$ */

/*----------------------------------------------------------------------
 *
 *  maldebug.h -- Dynamic memory handler interface
 *  Description: memdebug.h provides the interface definitions for the dynamic
 *  memory handler.
 *  See memdebug.c for complete documentation.
 *
 */

#ifndef _MALDEBUG_H
#define _MALDEBUG_H

/* Compilation options */
#define MEM_LIST		/* Build internal list */
#define MEM_WHERE		/* Keep track of memory block source */
#define MEM_HEADER		/* Keep headers and footers around for each block */
#define MEM_COMP_FREE	/* Complement the space free'd */

/* Interface functions */
unsigned long	Mem_Used(void);
void		Mem_Display(FILE *fp);

/* Interface functions to access only through macros */
#if defined(MEM_WHERE)
void	*mem_HDgetspace(size_t size, char *fil, int lin);
void	*mem_HDregetspace(void *old_ptr, size_t size, char *fil, int lin);
void	*mem_HDfreespace(void *ptr, char *fil, int lin);
#else
void	*mem_HDgetspace(size_t size);
void	*mem_HDregetspace(void *old_ptr, size_t size);
void	*mem_HDfreespace(void *ptr);
#endif

/* Interface macros */
#if !defined(__MALDEBUG__)
#if defined(MEM_WHERE)
#define HDgetspace(a)	    mem_HDgetspace((a),__FILE__,__LINE__)
#define HDregetspace(a,b)	mem_HDregetspace((a),(b),__FILE__,__LINE__)
#define HDfreespace(a)      mem_HDfreespace((a),__FILE__,__LINE__)
#else
#define HDgetspace(a)	    mem_HDgetspace(a)
#define HDregetspace(a,b)	mem_HDregetspace((a),(b))
#define HDfreespace(a)      mem_HDfreespace(a)
#endif
#endif

#endif /* _MALDEBUG_H */
