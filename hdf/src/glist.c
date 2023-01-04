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

/* TODO(schwehr): Check that num_of_elements never overflows */

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
    list->info->num_of_elements = 0;

    return SUCCEED;
}

#if 0
int
HDGLinitialize_sorted_list(Generic_list *list,
                       int (*lt)(VOIDP /* a */, VOIDP /* b */))
{
    const int ret_value = HDGLinitialize_list(list);

    if (ret_value == SUCCEED)
        list->info->lt = lt; /* Set sort fcn */

    return ret_value;
} /* HDGLinitialize_sorted_list() */
#endif

void
HDGLdestroy_list(Generic_list *list)
{
    HDGLremove_all(*list);
    HDfree(list->info);
}

#if 0
int
HDGLadd_to_beginning(Generic_list list, VOIDP pointer)
{
    CONSTR(FUNC, "HDGLadd_to_beginning");	/* for HERROR */

    if (pointer == NULL)
      {
        HERROR(DFE_ARGS);
        return FAIL;
      }

    /* Allocate and add to beginning of list */
    Generic_list_element *element =
        (Generic_list_element *)HDmalloc(sizeof(Generic_list_element));
    if (element == NULL)
      {
        HERROR(DFE_NOSPACE);
        return FAIL;
      }

    element->next     = list.info->pre_element.next;
    element->previous = &list.info->pre_element;
    element->pointer  = pointer;

    list.info->pre_element.next->previous = element;
    list.info->pre_element.next = element;

    list.info->num_of_elements++;

    return SUCCEED;
}
#endif

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

    list.info->num_of_elements++;

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

    list.info->num_of_elements++;

    return SUCCEED;
}

#if 0
VOIDP
HDGLremove_from_list(Generic_list list, VOIDP pointer)
{
    /* Find element in list */
    Generic_list_element *element = list.info->post_element.previous;
    while (element != &list.info->pre_element && element->pointer != pointer)
        element = element->previous;

    if (element == &list.info->pre_element)
      { /* No such element was found. */
        return NULL;
      }

    /* Have found element */
    if (element == list.info->current)
      {
        list.info->deleted_element.previous = element->previous;
        list.info->deleted_element.next     = element->next;
        list.info->current                  = &list.info->deleted_element;
      }

    element->previous->next = element->next;
    element->next->previous = element->previous;

    HDfree(element);
    list.info->num_of_elements--;

    return pointer; /* return ptr to original element */
}
#endif

#if 0
VOIDP
HDGLremove_from_beginning(Generic_list list)
{
    if(list.info->num_of_elements == 0)
      { /* Empty list */
        return NULL;
      }

    /* Remove first element */
    Generic_list_element *element = list.info->pre_element.next;
    if (element == list.info->current)
        list.info->current = &list.info->pre_element;

    VOIDP pointer = element->pointer;
    list.info->pre_element.next = element->next;
    element->next->previous = &list.info->pre_element;

    HDfree(element);
    list.info->num_of_elements--;

    return pointer; /* return the fist element */
}
#endif

#if 0
VOIDP
HDGLremove_from_end(Generic_list list)
{
    if(list.info->num_of_elements == 0)
      { /* Empty list */
        return NULL;
      }

    Generic_list_element *element = list.info->post_element.previous;
    if (element == list.info->current)
        list.info->current = &list.info->post_element;

    VOIDP pointer = element->pointer;
    list.info->post_element.previous = element->previous;
    element->previous->next = &list.info->post_element;

    HDfree(element);
    list.info->num_of_elements--;

    return pointer; /* return last element */
}
#endif

#if 0
VOIDP
HDGLremove_current(Generic_list list)
{
    Generic_list_element *element = list.info->current;
    if (element->pointer == NULL)
      { /* current is empty */
        return NULL;
      }

    list.info->deleted_element.previous = element->previous;
    list.info->deleted_element.next     = element->next;
    list.info->current                  = &list.info->deleted_element;

    VOIDP pointer = element->pointer;
    element->next->previous = element->previous;
    element->previous->next = element->next;

    HDfree(element);
    list.info->num_of_elements--;

    return pointer; /* return current element */
}
#endif

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
    list.info->num_of_elements = 0;
}

#if 0
VOIDP
HDGLpeek_at_beginning(Generic_list list)
{
    return list.info->pre_element.next->pointer;
}
#endif

#if 0
VOIDP
HDGLpeek_at_end(Generic_list list)
{
    return list.info->post_element.previous->pointer;
}
#endif

VOIDP
HDGLfirst_in_list(Generic_list list)
{
    list.info->current = list.info->pre_element.next->next->previous;
    return list.info->current->pointer;
}

#if 0
VOIDP
HDGLcurrent_in_list(Generic_list list)
{
    return list.info->current->pointer;
}
#endif

#if 0
VOIDP
HDGLlast_in_list(Generic_list list)
{
    list.info->current = list.info->post_element.previous->previous->next;
    return list.info->current->pointer;
}
#endif

VOIDP
HDGLnext_in_list(Generic_list list)
{
    list.info->current = list.info->current->next;
    return list.info->current->pointer;
}

#if 0
VOIDP
HDGLprevious_in_list(Generic_list list)
{
    list.info->current = list.info->current->previous;
    return list.info->current->pointer;
}
#endif

#if 0
void
HDGLreset_to_beginning(Generic_list list)
{
    list.info->current = &list.info->pre_element;
}
#endif

#if 0
void
HDGLreset_to_end(Generic_list list)
{
    list.info->current = &list.info->post_element;
}
#endif

#if 0
int
HDGLnum_of_objects(Generic_list list)
{
    return list.info->num_of_elements;
}
#endif

#if 0
int
HDGLis_empty(Generic_list list)
{
    return list.info->num_of_elements == 0;
}
#endif

#if 0
int
HDGLis_in_list(Generic_list list, VOIDP pointer)
{
    Generic_list_element *element = list.info->pre_element.next;

    while (element != &list.info->post_element && element->pointer != pointer)
        element = element->next;

    return element != &list.info->post_element;
}
#endif

#if 0
Generic_list
HDGLcopy_list(Generic_list list)
{
    Generic_list list_copy;
    list_copy.info = NULL; /* initialize info to NULL */

    /* initialize new list */
    int ret_value = HDGLinitialize_sorted_list(&list_copy, list.info->lt);

    /* copy over every element to new list */
    Generic_list_element *element = ret_value ? list.info->pre_element.next : NULL;
    while (element && element != &list.info->post_element)
      {
        if (HDGLadd_to_end(list_copy, element->pointer) == FAIL)
          {
            ret_value = FAIL;
            break;
          }
        element = element->next;
      }

    if (ret_value == FAIL)
      { /* need to remove all elements from copy */
          if (list_copy.info != NULL)
            {
                HDGLremove_all(list_copy);
            }

          list_copy.info = NULL; /* set to NULL */
      }

    return list_copy;
}
#endif

#if 0
void
HDGLperform_on_list(Generic_list list,
                    void (*fn)(VOIDP /* pointer */, VOIDP /* args */),
                    VOIDP args)
{
    Generic_list_element *element = list.info->pre_element.next;
    while (element != &list.info->post_element)
      { /* call fcn on each element */
        (*fn)(element->pointer, args);
        element = element->next;
      }
}
#endif

#if 0
VOIDP
HDGLfirst_that(Generic_list list,
               int (*fn)(VOIDP /* pointer */, VOIDP /* args */),
               VOIDP args)
{
    Generic_list_element *element = list.info->pre_element.next;
    while (element != &list.info->post_element &&
                            !(*fn)(element->pointer, args))
      {
        element = element->next;
      }

    if (element->pointer)
        list.info->current = element;

    return element->pointer;
}
#endif

#if 0
VOIDP
HDGLnext_that(Generic_list list,
              int (*fn)(VOIDP /* pointer */, VOIDP /* args */),
              VOIDP args)
{
    Generic_list_element *element = list.info->current->next;
    while (element != &list.info->post_element &&
                            !(*fn)(element->pointer, args))
      {
        element = element->next;
      }

    if (element->pointer)
        list.info->current = element;

    return element->pointer;
}
#endif

#if 0
VOIDP
HDGLprevious_that(Generic_list list,
                  int (*fn)(VOIDP /* pointer */, VOIDP /* args */),
                  VOIDP args)
{
    Generic_list_element *element = list.info->current->previous;
    while (element != &list.info->pre_element &&
                            !(*fn)(element->pointer, args))
      {
        element = element->previous;
      }

    if (element->pointer)
        list.info->current = element;

    return element->pointer;
}
#endif

#if 0
VOIDP
HDGLlast_that(Generic_list list,
              int (*fn)(VOIDP /* pointer */, VOIDP /* args */),
              VOIDP args)
{
    Generic_list_element *element = list.info->post_element.previous;
    while (element != &list.info->pre_element &&
                            !(*fn)(element->pointer, args))
      {
        element = element->previous;
      }

    if (element->pointer)
        list.info->current = element;

    return element->pointer;
}
#endif

#if 0
Generic_list
HDGLall_such_that(Generic_list list,
              int (*fn)(VOIDP /* pointer */, VOIDP /* args */),
              VOIDP args)
{
    /* initialize copy of list */
    Generic_list list_copy;
    int ret_value = HDGLinitialize_sorted_list(&list_copy, list.info->lt);

    /* copy over elements that satisfy the fcn */
    Generic_list_element *element = ret_value ? list.info->pre_element.next : NULL;
    while (element && element != &list.info->post_element)
      {
        if ((*fn)(element->pointer, args))
          {
            if (HDGLadd_to_end(list_copy, element->pointer) == FAIL)
              {
                  ret_value = FAIL;
                  break;
              }
          }
        element = element->next;
      }

    if (ret_value == FAIL)
      {
          if (list_copy.info != NULL)
            {
                HDGLremove_all(list_copy);
            }

          list_copy.info = NULL; /* set to NULL */
      }

    return list_copy;
} /* HDGLall_such_that() */
#endif

#if 0
HDGLremove_all_such_that(Generic_list list,
                         int (*fn)(VOIDP /* pointer */, VOIDP /* args */),
                         VOIDP args)
{
    HDGLreset_to_beginning(list);

    VOIDP obj;
    while ((obj = HDGLnext_in_list(list)))
      {
        if ((*fn)(obj, args))
            HDGLremove_current(list);
      }
}
#endif
