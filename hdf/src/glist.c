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

#include <stdio.h>
#include "glist.h"
#include "hdf.h"
#include "hdfi.h"
#include "herr.h"

int
HDGLinitialize_list(Generic_list *list)
{
    CONSTR(FUNC, "HDGLinitialize_list");	/* for HERROR */

    /* Allocate an initialize info struct */
    list->info = (Generic_list_info *)HDmalloc(sizeof(Generic_list_info));

    if (list->info == NULL)
      {
        HERROR(DFE_NOSPACE);
        return FAIL;
      }

    list->info->pre_element.pointer  = NULL;
    list->info->pre_element.previous = &list->info->pre_element;
    list->info->pre_element.next     = &list->info->post_element;
    list->info->post_element.pointer = NULL;
    list->info->post_element.previous = &list->info->pre_element;
    list->info->post_element.next     = &list->info->post_element;

    list->info->current = &list->info->pre_element;
    list->info->deleted_element.pointer = NULL;
    list->info->lt      = NULL;

    return SUCCEED;
}

void
HDGLdestroy_list(Generic_list *list)
{
    HDGLremove_all(*list);
    HDfree(list->info);
}

int
HDGLadd_to_end(Generic_list list, VOIDP pointer)
{
    CONSTR(FUNC, "HDGLadd_to_end");	/* for HERROR */

    if (pointer == NULL)
      {
        HERROR(DFE_ARGS);
        return FAIL;
      }

    /* Allocate and add to end of list */
    Generic_list_element *element =
        (Generic_list_element *)HDmalloc(sizeof(Generic_list_element));
    if (element == NULL)
      {
        HERROR(DFE_NOSPACE);
        return FAIL;
      }

    element->next     = &list.info->post_element;
    element->previous = list.info->post_element.previous;
    element->pointer  = pointer;

    list.info->post_element.previous->next = element;
    list.info->post_element.previous = element;

    return SUCCEED;
}

int
HDGLadd_to_list(Generic_list list, VOIDP pointer)
{
    CONSTR(FUNC, "HDGLadd_to_list");	/* for HERROR */

    /* Check to see if there is a sort fcn */
    if (!list.info->lt)
      {
        /* no sort fcn so add to end of list */
        return HDGLadd_to_end(list, pointer);
      }

    if (pointer == NULL)
      {
        HERROR(DFE_ARGS);
        return FAIL;
      }

    Generic_list_element *element = list.info->pre_element.next;
    while (element != &list.info->post_element &&
            (*list.info->lt)(element->pointer, pointer))
        element = element->next;

    /* Allocate and add to list */
    Generic_list_element *new_element =
        (Generic_list_element *)HDmalloc(sizeof(Generic_list_element));
    if (new_element == NULL)
      {
        HERROR(DFE_NOSPACE);
        return FAIL;
      }

    new_element->next     = element;
    new_element->previous = element->previous;
    new_element->pointer  = pointer;

    element->previous->next = new_element;
    element->previous = new_element;

    return SUCCEED;
}


void
HDGLremove_all(Generic_list list)
{
    Generic_list_element *element = list.info->pre_element.next;
    while (element != &list.info->post_element)
      {
        element = element->next;
        HDfree(element->previous);
      }

    list.info->pre_element.next = &list.info->post_element;
    list.info->post_element.previous = &list.info->pre_element;
}

VOIDP
HDGLfirst_in_list(Generic_list list)
{
    list.info->current = list.info->pre_element.next->next->previous;
    return list.info->current->pointer;
}

VOIDP
HDGLnext_in_list(Generic_list list)
{
    list.info->current = list.info->current->next;
    return list.info->current->pointer;
}
