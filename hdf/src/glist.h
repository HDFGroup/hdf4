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

/************************************************************************
  Credits:
         Original code is part of the public domain 'Generic List Library'
         by Keith Pomakis(kppomaki@jeeves.uwaterloo.ca)-Spring, 1994
 ************************************************************************/

#ifndef GLIST_H
#define GLIST_H

#include "hdfi.h"

/* Structure for each element in the list */
typedef struct GLE_struct {
    VOIDP                pointer;   /* data element itself */
    struct GLE_struct   *previous; /* previous element */
    struct GLE_struct   *next;     /* next element */
} Generic_list_element;

/* List info structure */
typedef struct GLI_struct {
    Generic_list_element   *current;               /* current element */
    Generic_list_element   pre_element;            /* pre element */
    Generic_list_element   post_element;           /* post element */
    Generic_list_element   deleted_element;        /* deleted element */
    int                  (*lt)(VOIDP a, VOIDP b); /* sort fcn */
} Generic_list_info;

/* Top most List structure, handle to the list */
typedef struct GL_struct {
    Generic_list_info *info;
} Generic_list;

/******************************************************************************
 NAME
     HDGLinitialize_list
 DESCRIPTION
     Every list must be initialized before it is used.  The only time it is
     valid to re-initialize a list is after it has been destroyed.
 RETURNS
     SUCCEED/FAIL
*******************************************************************************/
int HDGLinitialize_list(Generic_list *list /* IN: list */);

/******************************************************************************
 NAME
     HDGLadd_to_end
 DESCRIPTION
    This function will add the specified object to the end of the
    list.  The pointer must not be NULL.
 RETURNS
    SUCCEED/FAIL
*******************************************************************************/
int HDGLadd_to_end(Generic_list list, /*IN: list */
                    VOIDP pointer /*IN: data element */);

/******************************************************************************
 NAME
     HDGLadd_to_list
 DESCRIPTION
    This function will add the specified object to the list.  The pointer
    must not be NULL.
 RETURNS
     SUCCEED/FAIL
*******************************************************************************/
// Keep
int HDGLadd_to_list(Generic_list list, /*IN: list */
                     VOIDP pointer /*IN: data element */);

/******************************************************************************
 NAME
     HDGLremove_all
 DESCRIPTION
    This function will remove all objects from the list.  Note that the
    memory for these objects will not be reclaimed, so if the objects have
    no other references, it is best to avoid this function and remove the
    objects one by one, freeing them when necessary.
 RETURNS
    Nothing
*******************************************************************************/
void HDGLremove_all(Generic_list list /*IN: list */);

/******************************************************************************
 NAME
     HDGLfirst_in_list
 DESCRIPTION
    This function will return the first object in the list and mark it as
    the current object.  If the list is empty, NULL is returned.
 RETURNS
    First element in list if non-empty, otherwise NULL.
*******************************************************************************/
VOIDP HDGLfirst_in_list(Generic_list list /*IN: list */);

/******************************************************************************
 NAME
     HDGLnext_in_list
 DESCRIPTION
    This function will return the next object in the list and mark it as
    the current object.  If the end of the list is reached, or if the list
    is empty, NULL is returned.
 RETURNS
    Next element in list if non-empty, otherwise NULL.
*******************************************************************************/
VOIDP HDGLnext_in_list(Generic_list list /*IN: list */);


#endif /* GLIST_H */

