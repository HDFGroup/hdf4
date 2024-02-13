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
    dynarray.c - Internal storage routines for handling "dynamic arrays"

REMARKS
    Dynamic arrays are "standard" arrays which store objects in a straight-
    forward linear array of objects (void *'s currently), but the "dynarray"
    allows the size of the array to vary without excessive overhead.

DESIGN
    Each dynarray is managed as a small structure to store the current size
    of the dynarray and a pointer to the array of pointers to the objects
    stored within the array.

BUGS/LIMITATIONS
    No hashing or any other "folding" of the storage space is done, so the size
    of the dynarray can get quite excessive in proportion to the # of elements
    used if the elements used are "sparse" in the dynarray space.

    These are strictly usable from C, adding a FORTRAN calling facility would
    require some re-working of the routines.

LOCAL ROUTINES
  None
EXPORTED ROUTINES
 Element Functions:
     DAget_elem - Get an element from a dynarray
     DAset_elem - Set an element pointer for a dynarray
     DAdel_elem - Delete an element from a dynarray
 Dynarray Functions:
     DAcreate_array - Create a dynarray
     DAdestroy_array - Destroy a dynarray
     DAsize_array    - Get the current dynarray size
*/

#include "hdfi.h"
#include "dynarray.h"

typedef struct dynarray_tag {
    intn   num_elems; /* Number of elements in the array currently */
    intn   incr_mult; /* Multiple to increment the array size by */
    void **arr;       /* Pointer to the actual array of void *'s */
} dynarr_t;

/******************************************************************************
 NAME
     DAcreate_array - Create a dynarray

 DESCRIPTION
    Create a dynarray for later use.  This routine allocates the dynarray
    structure and creates a dynarray with the specified minimum size.

 RETURNS
    Returns pointer to the dynarray created if successful and NULL otherwise

*******************************************************************************/
dynarr_p
DAcreate_array(intn start_size, /* IN: Initial array size */
               intn incr_mult   /* IN: multiple to create additional elements in */
)
{
    dynarr_t *new_arr   = NULL; /* ptr to the new dynarray */
    dynarr_p  ret_value = NULL;

    HEclear();
    if (start_size < 0 || incr_mult <= 0)
        HGOTO_ERROR(DFE_ARGS, NULL);

    new_arr = (dynarr_t *)calloc(1, sizeof(dynarr_t));
    if (new_arr == NULL)
        HGOTO_ERROR(DFE_NOSPACE, NULL);

    new_arr->num_elems = start_size;
    new_arr->incr_mult = incr_mult;
    if (start_size > 0) { /* only allocate space if the initial size is positive */
        new_arr->arr = (void **)calloc(start_size, sizeof(void *));
        if (new_arr->arr == NULL)
            HGOTO_ERROR(DFE_NOSPACE, NULL);
    } /* end if */

    ret_value = (dynarr_p)new_arr;

done:
    if (ret_value == NULL) { /* Error condition cleanup */
        if (new_arr != NULL) {
            free(new_arr->arr);
            free(new_arr);
        }
    }

    return ret_value;
} /* end DAcreate_array() */

/******************************************************************************
 NAME
     DAdestroy_array - Destroy a dynarray

 DESCRIPTION
    Destroy an existing dynarray from use.  This routine de-allocates the
    dynarray structure and deletes the current dynarray.

 RETURNS
    Returns SUCCEED if successful and FAIL otherwise

*******************************************************************************/
intn
DAdestroy_array(dynarr_p arr,      /* IN: Array to destroy */
                intn     free_elem /* IN: whether to free each element */
)
{
    dynarr_t *dest_arr; /* ptr to the dynarray destroy*/
    intn      i;
    intn      ret_value = SUCCEED;

    HEclear();
    dest_arr = (dynarr_t *)arr;
    if (dest_arr == NULL)
        HGOTO_ERROR(DFE_ARGS, FAIL);

    /* Chuck all the items stored in the array */
    if (free_elem != 0)
        for (i = 0; i < arr->num_elems; i++) {
            free(arr->arr[i]);
        }

    free(dest_arr->arr);
    free(dest_arr);

done:
    return ret_value;
} /* end DAdestroy_array() */

/******************************************************************************
 NAME
     DAdestroy_array - Get the current size of a dynarray

 DESCRIPTION
    Get the number of elements in use currently.

 RETURNS
    Returns # of dynarray elements if successful and FAIL otherwise

*******************************************************************************/
intn
DAsize_array(dynarr_p arr /* IN: Array to get size of */
)
{
    dynarr_t *arr_ptr; /* ptr to the dynarray destroy*/
    intn      ret_value = SUCCEED;

    HEclear();
    arr_ptr = (dynarr_t *)arr;
    if (arr_ptr == NULL)
        HGOTO_ERROR(DFE_ARGS, FAIL);

    ret_value = arr_ptr->num_elems;

done:
    return ret_value;
} /* end DAsize_array() */

/******************************************************************************
 NAME
     DAget_elem - Get an element from a dynarray

 DESCRIPTION
    Retrieve an element from a dynarray.  If the element to be retrieved is
    beyond the end of the currently allocated array elements, the array is
    not extended, a NULL pointer is merely returned.

 RETURNS
    Returns object ptr if successful and NULL otherwise

*******************************************************************************/
void *
DAget_elem(dynarr_p arr_ptr, /* IN: Array to access */
           intn     elem     /* IN: Array element to retrieve */
)
{
    dynarr_t *arr; /* ptr to the dynarray */
    void     *ret_value = NULL;

    HEclear();
    arr = (dynarr_t *)arr_ptr;
    if (elem < 0 || arr == NULL)
        HGOTO_ERROR(DFE_ARGS, NULL);

    if (elem >= arr->num_elems)
        ret_value = NULL;
    else
        ret_value = arr->arr[elem];

done:
    return ret_value;
} /* end DAget_elem() */

/******************************************************************************
 NAME
     DAset_elem - Set an element pointer for a dynarray

 DESCRIPTION
    Set an element pointer for a dynarray.  If the element to be set is
    beyond the end of the currently allocated array elements, the array is
    extended by whatever multiple of the incr_mult is needed to expand the
    # of array elements to include the array element to set.

 RETURNS
    Returns SUCCEED if successful and NULL otherwise

*******************************************************************************/
intn
DAset_elem(dynarr_p arr_ptr, /* IN: Array to access */
           intn     elem,    /* IN: Array element to set */
           void    *obj      /* IN: Pointer to the object to store */
)
{
    dynarr_t *arr; /* ptr to the dynarray */
    intn      ret_value = SUCCEED;

    HEclear();
    arr = (dynarr_t *)arr_ptr;
    if (elem < 0 || arr == NULL)
        HGOTO_ERROR(DFE_ARGS, FAIL);

    if (elem >= arr->num_elems) {
        intn new_size; /* new number of elements in the array */

        new_size = ((elem / arr->incr_mult) + 1) * arr->incr_mult;
        if (arr->num_elems == 0) { /* array not currently allocated */
            if ((arr->arr = (void **)calloc(new_size, sizeof(void *))) == NULL)
                HGOTO_ERROR(DFE_NOSPACE, FAIL);
        }                   /* end if */
        else {              /* extend the existing array */
            void **new_arr; /* storage for the new array of ptrs */

            if ((new_arr = (void **)realloc(arr->arr, new_size * sizeof(void *))) == NULL)
                HGOTO_ERROR(DFE_NOSPACE, FAIL);
            memset(&new_arr[arr->num_elems], 0, sizeof(void *) * (uintn)(new_size - arr->num_elems));
            arr->arr = new_arr;
        } /* end else */
        arr->num_elems = new_size;
    } /* end if */

    /* Set the element value */
    arr->arr[elem] = obj;

done:
    return ret_value;
} /* end DAset_elem() */

/*****************************************************************************
 NAME
     DAdel_elem - Delete an element from a dynarray

 DESCRIPTION
    Retrieve an element from a dynarray & delete it from the dynarray.  If the
    element to be retrieved is beyond the end of the currently allocated array
    elements, the array is not extended, a NULL pointer is merely returned.

 RETURNS
    Returns object ptr if successful and NULL otherwise

*******************************************************************************/
void *
DAdel_elem(dynarr_p arr_ptr, /* IN: Array to access */
           intn     elem     /* IN: Array element to retrieve */
)
{
    dynarr_t *arr; /* ptr to the dynarray */
    void     *ret_value = NULL;

    HEclear();
    arr = (dynarr_t *)arr_ptr;
    if (elem < 0 || arr == NULL)
        HGOTO_ERROR(DFE_ARGS, NULL);

    if (elem >= arr->num_elems)
        ret_value = NULL;
    else {
        ret_value      = arr->arr[elem];
        arr->arr[elem] = NULL;
    } /* end else */

done:
    return ret_value;
} /* end DAdel_elem() */
