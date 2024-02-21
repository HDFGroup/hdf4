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

/*
FILE
    atom.c - Internal storage routines for handling "atoms"

REMARKS
    Atoms are just ID's which allow objects (void *'s currently) to be
    bundled into "groups" for more general storage.

DESIGN
    The groups are stored in an array of pointers to store each group in an
    element. Each "atomic group" node contains a link to a hash table to
    manage the atoms in each group.  The allowed "atomic groups" are stored
    in an enum (called group_t) in atom_priv.h.

BUGS/LIMITATIONS
    Can't iterate over the atoms in a group.

LOCAL ROUTINES
  HAIfind_atom      - Returns a pointer to an atom_info_t from a atom ID
  HAIget_atom_node  - Gets an atom node (uses the atom free list)
  HAIrelease_atom_node - Releases an atom node (uses the atom free list)
EXPORTED ROUTINES
 Atom Functions:
  HAregister_atom   - Register an object in a group and get an atom for it
  HAatom_object     - Get the object for an atom
  HAatom_group      - Get the group for an atom
  HAremove_atom     - Remove an atom from a group
  HAsearch_atom     - Search a group for a particular object
 Atom Group Functions:
  HAinit_group      - Initialize a group to store atoms in
  HAdestroy_group   - Destroy an atomic group
 Atom Group Cleanup:
  HAshutdown        - Terminate various static buffers.
*/

#include "hdf_priv.h"
#include "atom_priv.h"

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
    uintn         count;     /* # of times this group has been initialized */
    intn          hash_size; /* size of the hash table to store the atoms in */
    uintn         atoms;     /* current number of atoms held */
    uintn         nextid;    /* atom ID to use for the next atom */
    atom_info_t **atom_list; /* pointer to an array of ptrs to atoms */
} atom_group_t;

/* Array of pointers to atomic groups */
static atom_group_t *atom_group_list[MAXGROUP] = {NULL};

/* Pointer to the atom node free list */
static atom_info_t *atom_free_list = NULL;

/* Array of pointers to atomic groups */
atom_t atom_id_cache[ATOM_CACHE_SIZE]  = {-1, -1, -1, -1};
void  *atom_obj_cache[ATOM_CACHE_SIZE] = {NULL};

/* Private function prototypes */
static atom_info_t *HAIfind_atom(atom_t atm);

static atom_info_t *HAIget_atom_node(void);

static void HAIrelease_atom_node(atom_info_t *atm);

/******************************************************************************
 NAME
     HAinit_group - Initialize an atomic group

 DESCRIPTION
    Creates a global atomic group to store atoms in.  If the group has already
    been initialized, this routine just increments the count of # of
    initializations and returns without trying to change the size of the hash
    table.

    NOTE: The hash size MUST be a power of 2 (checked in code)

 RETURNS
    Returns SUCCEED if successful and FAIL otherwise

*******************************************************************************/
intn
HAinit_group(group_t grp,      /* IN: Group to initialize */
             intn    hash_size /* IN: Minimum hash table size to use for group */
)
{
    atom_group_t *grp_ptr   = NULL; /* ptr to the atomic group */
    intn          ret_value = SUCCEED;

    HEclear();
    if ((grp <= BADGROUP || grp >= MAXGROUP) && hash_size > 0)
        HGOTO_ERROR(DFE_ARGS, FAIL);

    /* Assertion necessary for faster pointer swapping */
    assert(sizeof(hdf_pint_t) == sizeof(void *));

    /* Ensure hash_size is not zero and a power of two */
    if (hash_size == 0)
        HGOTO_ERROR(DFE_ARGS, FAIL);
    if (hash_size & (hash_size - 1))
        HGOTO_ERROR(DFE_ARGS, FAIL);

    if (atom_group_list[grp] == NULL) { /* Allocate the group information */
        grp_ptr = (atom_group_t *)calloc(1, sizeof(atom_group_t));
        if (grp_ptr == NULL)
            HGOTO_ERROR(DFE_NOSPACE, FAIL);
        atom_group_list[grp] = grp_ptr;
    }    /* end if */
    else /* Get the pointer to the existing group */
        grp_ptr = atom_group_list[grp];

    if (grp_ptr->count == 0) { /* Initialize the atom group structure */
        grp_ptr->hash_size = hash_size;
        grp_ptr->atoms     = 0;
        grp_ptr->nextid    = 0;
        if ((grp_ptr->atom_list = (atom_info_t **)calloc(hash_size, sizeof(atom_info_t *))) == NULL)
            HGOTO_ERROR(DFE_NOSPACE, FAIL);
    } /* end if */

    /* Increment the count of the times this group has been initialized */
    grp_ptr->count++;

done:
    if (ret_value == FAIL) { /* Error condition cleanup */
        if (grp_ptr != NULL) {
            free(grp_ptr->atom_list);
            free(grp_ptr);
        }
    }

    return ret_value;
} /* end HAinit_group() */

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
intn
HAdestroy_group(group_t grp /* IN: Group to destroy */
)
{
    atom_group_t *grp_ptr   = NULL; /* ptr to the atomic group */
    intn          ret_value = SUCCEED;

    HEclear();
    if (grp <= BADGROUP || grp >= MAXGROUP)
        HGOTO_ERROR(DFE_ARGS, FAIL);

    grp_ptr = atom_group_list[grp];
    if (grp_ptr == NULL || grp_ptr->count <= 0)
        HGOTO_ERROR(DFE_INTERNAL, FAIL);

    /* Decrement the number of users of the atomic group */
    if ((--(grp_ptr->count)) == 0) {
        uintn i;

        for (i = 0; i < ATOM_CACHE_SIZE; i++)
            if (ATOM_TO_GROUP(atom_id_cache[i]) == grp) {
                atom_id_cache[i]  = (-1);
                atom_obj_cache[i] = NULL;
            } /* end if */
        free(grp_ptr->atom_list);
        grp_ptr->atom_list = NULL;
    } /* end if */

done:
    return ret_value;
} /* end HAdestroy_group() */

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
atom_t
HAregister_atom(group_t grp,   /* IN: Group to register the object in */
                void   *object /* IN: Object to attach to atom */
)
{
    atom_group_t *grp_ptr = NULL; /* ptr to the atomic group */
    atom_info_t  *atm_ptr = NULL; /* ptr to the new atom */
    atom_t        atm_id;         /* new atom ID */
    uintn         hash_loc;       /* new item's hash table location */
    atom_t        ret_value = SUCCEED;

    HEclear();
    if (grp <= BADGROUP || grp >= MAXGROUP)
        HGOTO_ERROR(DFE_ARGS, FAIL);

    grp_ptr = atom_group_list[grp];
    if (grp_ptr == NULL || grp_ptr->count <= 0)
        HGOTO_ERROR(DFE_INTERNAL, FAIL);

    if ((atm_ptr = HAIget_atom_node()) == NULL)
        HGOTO_ERROR(DFE_NOSPACE, FAIL);

    /* Create the atom & it's ID */
    atm_id           = MAKE_ATOM(grp, grp_ptr->nextid);
    atm_ptr->id      = atm_id;
    atm_ptr->obj_ptr = object;
    atm_ptr->next    = NULL;

    /* hash bucket already full, prepend to front of chain */
    hash_loc = grp_ptr->nextid % (uintn)grp_ptr->hash_size;
    if (grp_ptr->atom_list[hash_loc] != NULL)
        atm_ptr->next = grp_ptr->atom_list[hash_loc];

    /* Insert into the group */
    grp_ptr->atom_list[hash_loc] = atm_ptr;
    grp_ptr->atoms++;
    grp_ptr->nextid++;

    ret_value = atm_id;

done:
    return ret_value;
} /* end HAregister_atom() */

/******************************************************************************
 NAME
     HAatom_object - Returns to the object ptr for the atom

 DESCRIPTION
    Retrieves the object ptr which is associated with the atom.

 RETURNS
    Returns object ptr if successful and NULL otherwise

*******************************************************************************/
void *
HAPatom_object(atom_t atm /* IN: Atom to retrieve object for */
)
{
    atom_info_t *atm_ptr   = NULL; /* ptr to the new atom */
    void        *ret_value = NULL;

    HEclear();

    /* General lookup of the atom */
    if ((atm_ptr = HAIfind_atom(atm)) == NULL)
        HGOTO_ERROR(DFE_INTERNAL, NULL);

    /* Check if we've found the correct atom */
    if (atm_ptr != NULL)
        ret_value = atm_ptr->obj_ptr;

done:
    return ret_value;
} /* end HAatom_object() */

/******************************************************************************
 NAME
     HAatom_group - Returns to the group for the atom

 DESCRIPTION
    Retrieves the group which is associated with the atom.

 RETURNS
    Returns group if successful and BADGROUP otherwise

*******************************************************************************/
group_t
HAatom_group(atom_t atm /* IN: Atom to retrieve group for */
)
{
    group_t ret_value = BADGROUP;

    HEclear();
    ret_value = ATOM_TO_GROUP(atm);
    if (ret_value <= BADGROUP || ret_value >= MAXGROUP)
        HGOTO_ERROR(DFE_ARGS, BADGROUP);

done:
    return ret_value;
} /* end HAatom_group() */

/******************************************************************************
 NAME
     HAremove_atom - Removes an atom from a group

 DESCRIPTION
    Removes an atom from a group.

 RETURNS
    Returns atom's object if successful and NULL otherwise

*******************************************************************************/
void *
HAremove_atom(atom_t atm /* IN: Atom to remove */
)
{
    atom_group_t *grp_ptr = NULL; /* ptr to the atomic group */
    atom_info_t  *curr_atm,       /* ptr to the current atom */
        *last_atm;                /* ptr to the last atom */
    group_t grp;                  /* atom's atomic group */
    uintn   hash_loc;             /* atom's hash table location */
    uintn   i;                    /* local counting variable */
    void   *ret_value = NULL;

    HEclear();
    grp = ATOM_TO_GROUP(atm);
    if (grp <= BADGROUP || grp >= MAXGROUP)
        HGOTO_ERROR(DFE_ARGS, NULL);

    grp_ptr = atom_group_list[grp];
    if (grp_ptr == NULL || grp_ptr->count <= 0)
        HGOTO_ERROR(DFE_INTERNAL, NULL);

    /* Get the location in which the atom is located */
    hash_loc = (uintn)ATOM_TO_LOC(atm, grp_ptr->hash_size);
    curr_atm = grp_ptr->atom_list[hash_loc];
    if (curr_atm == NULL)
        HGOTO_ERROR(DFE_INTERNAL, NULL);

    last_atm = NULL;
    while (curr_atm != NULL) {
        if (curr_atm->id == atm)
            break;
        last_atm = curr_atm;
        curr_atm = curr_atm->next;
    } /* end while */

    if (curr_atm != NULL) {
        if (last_atm == NULL) /* atom is the first the chain */
            grp_ptr->atom_list[hash_loc] = curr_atm->next;
        else
            last_atm->next = curr_atm->next;
        ret_value = curr_atm->obj_ptr;
        HAIrelease_atom_node(curr_atm);
    }    /* end if */
    else /* couldn't find the atom in the proper place */
        HGOTO_ERROR(DFE_INTERNAL, NULL);

    /* Delete object from cache */
    for (i = 0; i < ATOM_CACHE_SIZE; i++)
        if (atom_id_cache[i] == atm) {
            atom_id_cache[i]  = (-1);
            atom_obj_cache[i] = NULL;
            break; /* we assume there is only one instance in the cache */
        }

    /* Decrement the number of atoms in the group */
    (grp_ptr->atoms)--;

done:
    return ret_value;
} /* end HAremove_atom() */

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
void *
HAsearch_atom(group_t         grp,  /* IN: Group to search for the object in */
              HAsearch_func_t func, /* IN: Ptr to the comparison function */
              const void     *key   /* IN: pointer to key to compare against */
)
{
    atom_group_t *grp_ptr = NULL; /* ptr to the atomic group */
    atom_info_t  *atm_ptr = NULL; /* ptr to the new atom */
    intn          i;              /* local counting variable */
    void         *ret_value = NULL;

    HEclear();
    if (grp <= BADGROUP || grp >= MAXGROUP)
        HGOTO_ERROR(DFE_ARGS, NULL);

    grp_ptr = atom_group_list[grp];
    if (grp_ptr == NULL || grp_ptr->count <= 0)
        HGOTO_ERROR(DFE_INTERNAL, NULL);

    /* Start at the beginning of the array */
    for (i = 0; i < grp_ptr->hash_size; i++) {
        atm_ptr = grp_ptr->atom_list[i];
        while (atm_ptr != NULL) {
            if ((*func)(atm_ptr->obj_ptr, key))
                HGOTO_DONE(atm_ptr->obj_ptr); /* found the item we are looking for */
            atm_ptr = atm_ptr->next;
        } /* end while */
    }     /* end for */

done:
    return ret_value;
} /* end HAsearch_atom() */

/******************************************************************************
 NAME
     HAIfind_atom - Finds a atom in a group

 DESCRIPTION
    Retrieves the atom ptr which is associated with the atom.

 RETURNS
    Returns atom ptr if successful and NULL otherwise

*******************************************************************************/
static atom_info_t *
HAIfind_atom(atom_t atm /* IN: Atom to retrieve atom for */
)
{
    atom_group_t *grp_ptr = NULL; /* ptr to the atomic group */
    atom_info_t  *atm_ptr = NULL; /* ptr to the new atom */
    group_t       grp;            /* atom's atomic group */
    uintn         hash_loc;       /* atom's hash table location */
    atom_info_t  *ret_value = NULL;

    HEclear();
    grp = ATOM_TO_GROUP(atm);
    if (grp <= BADGROUP || grp >= MAXGROUP)
        HGOTO_ERROR(DFE_ARGS, NULL);

    grp_ptr = atom_group_list[grp];
    if (grp_ptr == NULL || grp_ptr->count <= 0)
        HGOTO_ERROR(DFE_INTERNAL, NULL);

    /* Get the location in which the atom is located */
    hash_loc = (uintn)ATOM_TO_LOC(atm, grp_ptr->hash_size);
    atm_ptr  = grp_ptr->atom_list[hash_loc];
    if (atm_ptr == NULL)
        HGOTO_ERROR(DFE_INTERNAL, NULL);

    while (atm_ptr != NULL) {
        if (atm_ptr->id == atm)
            break;
        atm_ptr = atm_ptr->next;
    } /* end while */

    if (atm_ptr) {
        /* if found, add it to the end of the cached list */
        atom_id_cache[ATOM_CACHE_SIZE - 1]  = atm;
        atom_obj_cache[ATOM_CACHE_SIZE - 1] = atm_ptr->obj_ptr;
    }

    ret_value = atm_ptr;

done:
    return ret_value;
} /* end HAIfind_atom() */

/******************************************************************************
 NAME
     HAIget_atom_node - Gets an atom node

 DESCRIPTION
    Either gets an atom node from the free list (if there is one available)
    or allocate a node.

 RETURNS
    Returns atom ptr if successful and NULL otherwise

*******************************************************************************/
static atom_info_t *
HAIget_atom_node(void)
{
    atom_info_t *ret_value = NULL;

    HEclear();
    if (atom_free_list != NULL) {
        ret_value      = atom_free_list;
        atom_free_list = atom_free_list->next;
    } /* end if */
    else {
        if ((ret_value = (atom_info_t *)malloc(sizeof(atom_info_t))) == NULL)
            HGOTO_ERROR(DFE_NOSPACE, NULL);
    } /* end else */

done:
    return ret_value;
} /* end HAIget_atom_node() */

/******************************************************************************
 NAME
     HAIrelease_atom_node - Releases an atom node

 DESCRIPTION
    Puts an atom node into the free list

 RETURNS
    No return value

*******************************************************************************/
static void
HAIrelease_atom_node(atom_info_t *atm)
{
    /* Insert the atom at the beginning of the free list */
    atm->next      = atom_free_list;
    atom_free_list = atm;
} /* end HAIrelease_atom_node() */

/*--------------------------------------------------------------------------
 NAME
    HAshutdown
 PURPOSE
    Terminate various static buffers.
 USAGE
    intn HAshutdown()
 RETURNS
    Returns SUCCEED/FAIL
 DESCRIPTION
    Free various buffers allocated in the HA routines.
 GLOBAL VARIABLES
 COMMENTS, BUGS, ASSUMPTIONS
    Should only ever be called by the "atexit" function HDFend
 EXAMPLES
 REVISION LOG
--------------------------------------------------------------------------*/
intn
HAshutdown(void)
{
    atom_info_t *curr;
    intn         i;

    /* Release the free-list if it exists */
    if (atom_free_list != NULL) {
        while (atom_free_list != NULL) {
            curr           = atom_free_list;
            atom_free_list = atom_free_list->next;
            free(curr);
        } /* end while */
    }     /* end if */

    for (i = 0; i < (intn)MAXGROUP; i++)
        if (atom_group_list[i] != NULL) {
            free(atom_group_list[i]->atom_list);
            free(atom_group_list[i]);
            atom_group_list[i] = NULL;
        } /* end if */
    return SUCCEED;
} /* end HAshutdown() */
