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
 * File:    dynarray_priv.h
 * Purpose: header file for dynamic array API
 *---------------------------------------------------------------------------*/

#ifndef H4_DYNARRAY_PRIV_H
#define H4_DYNARRAY_PRIV_H

#include "hdf_priv.h"

/*
    Define the pointer to the dynarray without giving outside routines access
    to the internal workings of the structure.
*/
typedef struct dynarray_tag *dynarr_p;

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************
 NAME
     DAcreate_array - Create a dynarray

 DESCRIPTION
    Create a dynarray for later use.  This routine allocates the dynarray
    structure and creates a dynarray with the specified minimum size.

 RETURNS
    Returns pointer to the dynarray created if successful and NULL otherwise

*******************************************************************************/
dynarr_p DAcreate_array(int start_size, /* IN: Initial array size */
                        int incr_mult   /* IN: multiple to create additional elements in */
);

/******************************************************************************
 NAME
     DAdestroy_array - Destroy a dynarray

 DESCRIPTION
    Destroy an existing dynarray from use.  This routine de-allocates the
    dynarray structure and deletes the current dynarray.

 RETURNS
    Returns SUCCEED if successful and FAIL otherwise

*******************************************************************************/
int DAdestroy_array(dynarr_p arr,      /* IN: Array to destroy */
                    int      free_elem /* IN: whether to free each element */
);

/******************************************************************************
 NAME
     DAdestroy_array - Get the current size of a dynarray

 DESCRIPTION
    Get the number of elements in use currently.

 RETURNS
    Returns # of dynarray elements if successful and FAIL otherwise

*******************************************************************************/
int DAsize_array(dynarr_p arr /* IN: Array to get size of */
);

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
void *DAget_elem(dynarr_p arr_ptr, /* IN: Array to access */
                 int      elem     /* IN: Array element to retrieve */
);

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
int DAset_elem(dynarr_p arr_ptr, /* IN: Array to access */
               int      elem,    /* IN: Array element to set */
               void    *obj      /* IN: Pointer to the object to store */
);

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
void *DAdel_elem(dynarr_p arr_ptr, /* IN: Array to access */
                 int      elem     /* IN: Array element to retrieve */
);

#ifdef __cplusplus
}
#endif

#endif /* H4_DYNARRAY_PRIV_H */
