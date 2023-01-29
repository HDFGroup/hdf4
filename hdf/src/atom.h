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
 * File:    atom.h
 * Purpose: header file for atom API
 *---------------------------------------------------------------------------*/

#ifndef H4_ATOM_H
#define H4_ATOM_H

#include "hdf.h"

#include "H4api_adpt.h"

/* Atom Features control */

/* Do swap using XOR operator. Ugly but fast... */
#define HAIswap_cache(i, j)                                                                                  \
    atom_id_cache[i] ^= atom_id_cache[j],                                                                    \
        atom_obj_cache[i] = (void *)((hdf_pint_t)atom_obj_cache[j] ^ (hdf_pint_t)atom_obj_cache[i]),         \
        atom_id_cache[j] ^= atom_id_cache[i],                                                                \
        atom_obj_cache[j] = (void *)((hdf_pint_t)atom_obj_cache[i] ^ (hdf_pint_t)atom_obj_cache[j]),         \
        atom_id_cache[i] ^= atom_id_cache[j],                                                                \
        atom_obj_cache[i] = (void *)((hdf_pint_t)atom_obj_cache[i] ^ (hdf_pint_t)atom_obj_cache[j])

/* Note! This is hardwired to the atom cache value being 4 */
#define HAatom_object(atm)                                                                                   \
    (atom_id_cache[0] == atm   ? atom_obj_cache[0]                                                           \
     : atom_id_cache[1] == atm ? (HAIswap_cache(0, 1), atom_obj_cache[0])                                    \
     : atom_id_cache[2] == atm ? (HAIswap_cache(1, 2), atom_obj_cache[1])                                    \
     : atom_id_cache[3] == atm ? (HAIswap_cache(2, 3), atom_obj_cache[2])                                    \
                               : HAPatom_object(atm))

/* Group values allowed */
typedef enum {
    BADGROUP   = (-1), /* Invalid Group */
    DDGROUP    = 0,    /* Group ID for DD objects */
    AIDGROUP   = 1,    /* Group ID for access ID objects */
    FIDGROUP   = 2,    /* Group ID for file ID objects */
    VGIDGROUP  = 3,    /* Group ID for Vgroup ID objects */
    VSIDGROUP  = 4,    /* Group ID for Vdata ID objects */
    GRIDGROUP  = 5,    /* Group ID for GR ID objects */
    RIIDGROUP  = 6,    /* Group ID for RI ID objects */
    BITIDGROUP = 7,    /* Group ID for Bitfile ID objects */
    ANIDGROUP  = 8,    /* Group ID for Annotation ID objects */
    MAXGROUP           /* Highest group in group_t (Invalid as true group) */
} group_t;

/* Type of atoms to return to users */
typedef int32 atom_t;

/* Type of the function to compare objects & keys */
typedef int (*HAsearch_func_t)(const void *obj, const void *key);

#if defined ATOM_MASTER | defined ATOM_TESTER

/* # of bits to use for Group ID in each atom (change if MAXGROUP > 16) */
#define GROUP_BITS 4
#define GROUP_MASK 0x0F

/* # of bits to use for the Atom index in each atom (change if MAXGROUP > 16) */
#define ATOM_BITS 28
#define ATOM_MASK 0x0FFFFFFF

/* # of previous atoms cached, change inline caching macros (HAatom_object & HAIswap_cache) if this changes */
#define ATOM_CACHE_SIZE 4

/* Map an atom to a Group number */
#define ATOM_TO_GROUP(a) ((group_t)((((atom_t)(a)) >> ((sizeof(atom_t) * 8) - GROUP_BITS)) & GROUP_MASK))

/* Map an atom to a hash location (assumes s is a power of 2 and smaller than the ATOM_MASK constant) */
#define ATOM_TO_LOC(a, s) ((atom_t)(a) & ((s)-1))

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
    unsigned      count;     /* # of times this group has been initialized */
    int           hash_size; /* size of the hash table to store the atoms in */
    unsigned      atoms;     /* current number of atoms held */
    unsigned      nextid;    /* atom ID to use for the next atom */
    atom_info_t **atom_list; /* pointer to an array of ptrs to atoms */
} atom_group_t;

/* Define this in only one place */
#ifdef ATOM_MASTER

/* Array of pointers to atomic groups */
static atom_group_t *atom_group_list[MAXGROUP] = {NULL};

/* Pointer to the atom node free list */
static atom_info_t *atom_free_list = NULL;

/* Array of pointers to atomic groups */
HDFPUBLIC atom_t atom_id_cache[ATOM_CACHE_SIZE]  = {-1, -1, -1, -1};
HDFPUBLIC void  *atom_obj_cache[ATOM_CACHE_SIZE] = {NULL};
#endif /* ATOM_MASTER */

/* Useful routines for generally private use */

#endif /* ATOM_MASTER | ATOM_TESTER */

#ifndef ATOM_MASTER
HDFLIBAPI atom_t atom_id_cache[];
HDFLIBAPI void  *atom_obj_cache[];
#endif

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************
 NAME
     HAinit_group - Initialize an atomic group

 DESCRIPTION
    Creates an atomic group to store atoms in.  If the group has already been
    initialized, this routine just increments the count of # of initializations
    and returns without trying to change the size of the hash table.

 RETURNS
    Returns SUCCEED if successful and FAIL otherwise

*******************************************************************************/
HDFLIBAPI int HAinit_group(group_t grp,     /* IN: Group to initialize */
                           int    hash_size /* IN: Minimum hash table size to use for group */
);

/******************************************************************************
 NAME
     HAdestroy_group - Destroy an atomic group

 DESCRIPTION
    Destroys an atomic group which atoms are stored in.  If the group still
    has atoms which are registered, this routine fails.  If there have been
    multiple initializations of the group, this routine just decrements the
    count of initializations and does not check the atoms out-standing.

 RETURNS
    Returns SUCCEED if successful and FAIL otherwise

*******************************************************************************/
HDFLIBAPI int HAdestroy_group(group_t grp /* IN: Group to destroy */
);

/******************************************************************************
 NAME
     HAregister_atom - Register an object in a group and get an atom for it.

 DESCRIPTION
    Registers an object in a group and returns an atom for it.  This routine
    does _not_ check for unique-ness of the objects, if you register an object
    twice, you will get two different atoms for it.  This routine does make
    certain that each atom in a group is unique.  Atoms are created by getting
    a unique number for the group the atom is in and incorporating the group
    into the atom which is returned to the user.

 RETURNS
    Returns atom if successful and FAIL otherwise

*******************************************************************************/
HDFLIBAPI atom_t HAregister_atom(group_t grp,   /* IN: Group to register the object in */
                                 void   *object /* IN: Object to attach to atom */
);

/******************************************************************************
 NAME
     HAatom_object - Returns to the object ptr for the atom

 DESCRIPTION
    Retrieves the object ptr which is associated with the atom.

 RETURNS
    Returns object ptr if successful and NULL otherwise

*******************************************************************************/
HDFLIBAPI void *HAPatom_object(atom_t atm /* IN: Atom to retrieve object for */
);

/******************************************************************************
 NAME
     HAatom_group - Returns to the group for the atom

 DESCRIPTION
    Retrieves the group which is associated with the atom.

 RETURNS
    Returns group if successful and FAIL otherwise

*******************************************************************************/
HDFLIBAPI group_t HAatom_group(atom_t atm /* IN: Atom to retrieve group for */
);

/******************************************************************************
 NAME
     HAremove_atom - Removes an atom from a group

 DESCRIPTION
    Removes an atom from a group.

 RETURNS
    Returns atom's object if successful and FAIL otherwise

*******************************************************************************/
HDFLIBAPI void *HAremove_atom(atom_t atm /* IN: Atom to remove */
);

/******************************************************************************
 NAME
     HAsearch_atom - Search for an object in a group and get it's pointer.

 DESCRIPTION
    Searches for an object in a group and returns the pointer to it.
    This routine calls the function pointer passed in for each object in the
    group until it finds a match.  Currently there is no way to resume a
    search.

 RETURNS
    Returns pointer an atom's object if successful and NULL otherwise

*******************************************************************************/
HDFLIBAPI void *HAsearch_atom(group_t         grp,  /* IN: Group to search for the object in */
                              HAsearch_func_t func, /* IN: Ptr to the comparison function */
                              const void     *key   /* IN: pointer to key to compare against */
);

/******************************************************************************
 NAME
     HAshutdown - Terminate various static buffers.

 DESCRIPTION
    Free various buffers allocated in the HA routines.

 RETURNS
    Returns SUCCEED/FAIL

*******************************************************************************/
HDFLIBAPI int HAshutdown(void);

#ifdef __cplusplus
}
#endif

#endif /* H4_ATOM_H */
