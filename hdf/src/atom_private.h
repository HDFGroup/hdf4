/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Copyright by The HDF Group.                                               *
 * Copyright by the Board of Trustees of the University of Illinois.         *
 * All rights reserved.                                                      *
 *                                                                           *
 * This file is part of HDF.  The full HDF copyright notice, including       *
 * terms governing use, modification, and redistribution, is contained in    *
 * the COPYING file, which can be found at the root of the source code       *
 * distribution tree, or in https://support.hdfgroup.org/ftp/HDF/releases/.  *
 * If you do not have access to either file, you may request a copy from     *
 * help@hdfgroup.org.                                                        *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/*-----------------------------------------------------------------------------
 * File:    atom_private.h
 * Purpose: private header file for atom API
 *---------------------------------------------------------------------------*/

#ifndef H4_ATOM_PRIVATE_H
#define H4_ATOM_PRIVATE_H

/* # of bits to use for Group ID in each atom (change if MAXGROUP>16) */
#define GROUP_BITS 4
#define GROUP_MASK 0x0F

/* # of bits to use for the Atom index in each atom (change if MAXGROUP>16) */
#define ATOM_BITS 28
#define ATOM_MASK 0x0FFFFFFF

/* # of previous atoms cached, change inline caching macros (HAatom_object & HAIswap_cache) if this changes */
#define ATOM_CACHE_SIZE 4

/* Map an atom to a Group number */
#define ATOM_TO_GROUP(a) ((group_t)((((atom_t)(a)) >> ((sizeof(atom_t) * 8) - GROUP_BITS)) & GROUP_MASK))

#ifdef HASH_SIZE_POWER_2
/* Map an atom to a hash location (assumes s is a power of 2 and smaller than the ATOM_MASK constant) */
#define ATOM_TO_LOC(a, s) ((atom_t)(a) & ((s)-1))
#else
/* Map an atom to a hash location */
#define ATOM_TO_LOC(a, s) (((atom_t)(a)&ATOM_MASK) % (s))
#endif

/* Combine a Group number and an atom index into an atom */
#define MAKE_ATOM(g, i)                                                                                      \
    ((((atom_t)(g)&GROUP_MASK) << ((sizeof(atom_t) * 8) - GROUP_BITS)) | ((atom_t)(i)&ATOM_MASK))

/* Atom information structure used */
typedef struct atom_info_struct_tag {
    atom_t                       id;      /* atom ID for this info */
    void                       **obj_ptr; /* pointer associated with the atom */
    struct atom_info_struct_tag *next;    /* link to next atom (in case of hash-clash) */
} atom_info_t;

/* Atom group structure used */
typedef struct atom_group_struct_tag {
    uintn         count;     /* # of times this group has been initialized */
    intn          hash_size; /* size of the hash table to store the atoms in */
    uintn         atoms;     /* current number of atoms held */
    uintn         nextid;    /* atom ID to use for the next atom */
    atom_info_t **atom_list; /* pointer to an array of ptrs to atoms */
} atom_group_t;

#endif /* H4_ATOM_PRIVATE_H */
