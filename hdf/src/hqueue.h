/*
 * Copyright (c) 1991, 1993
 *	The Regents of the University of California.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 *	@(#)queue.h	8.3 (Berkeley) 12/13/93
 */

/*****************************************************************************
 * File: hqueue.h
 *
 * This is a modffied version of the original Berkeley code for
 * manipulating a memory pool. This version however is not
 * compatible with the original Berkeley version.
 *
 * Author: George V.- 9/3/96
 *
 *****************************************************************************/

#ifndef H4_HQUEUE_H
#define H4_HQUEUE_H

/*
 * This file defines three types of data structures: lists, tail queues,
 * and circular queues.
 *
 * A list is headed by a single forward pointer (or an array of forward
 * pointers for a hash table header). The elements are doubly linked
 * so that an arbitrary element can be removed without a need to
 * traverse the list. New elements can be added to the list after
 * an existing element or at the head of the list. A list may only be
 * traversed in the forward direction.
 *
 * A tail queue is headed by a pair of pointers, one to the head of the
 * list and the other to the tail of the list. The elements are doubly
 * linked so that an arbitrary element can be removed without a need to
 * traverse the list. New elements can be added to the list after
 * an existing element, at the head of the list, or at the end of the
 * list. A tail queue may only be traversed in the forward direction.
 *
 * A circle queue is headed by a pair of pointers, one to the head of the
 * list and the other to the tail of the list. The elements are doubly
 * linked so that an arbitrary element can be removed without a need to
 * traverse the list. New elements can be added to the list before or after
 * an existing element, at the head of the list, or at the end of the list.
 * A circle queue may be traversed in either direction, but has a more
 * complex end of list detection.
 *
 * For details on the use of these macros, see the queue(3) manual page.
 * (BSD4.3 manual set - GeorgeV)
 *
 */

/* NOTE: LIST and TAILQ implementations were removed */

/*
 * Circular queue definitions.
 */
#define H4_CIRCLEQ_HEAD(name, type)                                                                          \
    struct name {                                                                                            \
        struct type *cqh_first; /* first element */                                                          \
        struct type *cqh_last;  /* last element */                                                           \
    }

#define H4_CIRCLEQ_ENTRY(type)                                                                               \
    struct {                                                                                                 \
        struct type *cqe_next; /* next element */                                                            \
        struct type *cqe_prev; /* previous element */                                                        \
    }

/*
 * Circular queue functions.
 */
#define H4_CIRCLEQ_INIT(head)                                                                                \
    {                                                                                                        \
        (head)->cqh_first = (void *)(head);                                                                  \
        (head)->cqh_last  = (void *)(head);                                                                  \
    }

#define H4_CIRCLEQ_INSERT_AFTER(head, listelm, elm, field)                                                   \
    {                                                                                                        \
        (elm)->field.cqe_next = (listelm)->field.cqe_next;                                                   \
        (elm)->field.cqe_prev = (listelm);                                                                   \
        if ((listelm)->field.cqe_next == (void *)(head))                                                     \
            (head)->cqh_last = (elm);                                                                        \
        else                                                                                                 \
            (listelm)->field.cqe_next->field.cqe_prev = (elm);                                               \
        (listelm)->field.cqe_next = (elm);                                                                   \
    }

#define H4_CIRCLEQ_INSERT_BEFORE(head, listelm, elm, field)                                                  \
    {                                                                                                        \
        (elm)->field.cqe_next = (listelm);                                                                   \
        (elm)->field.cqe_prev = (listelm)->field.cqe_prev;                                                   \
        if ((listelm)->field.cqe_prev == (void *)(head))                                                     \
            (head)->cqh_first = (elm);                                                                       \
        else                                                                                                 \
            (listelm)->field.cqe_prev->field.cqe_next = (elm);                                               \
        (listelm)->field.cqe_prev = (elm);                                                                   \
    }

#define H4_CIRCLEQ_INSERT_HEAD(head, elm, field)                                                             \
    {                                                                                                        \
        (elm)->field.cqe_next = (head)->cqh_first;                                                           \
        (elm)->field.cqe_prev = (void *)(head);                                                              \
        if ((head)->cqh_last == (void *)(head))                                                              \
            (head)->cqh_last = (elm);                                                                        \
        else                                                                                                 \
            (head)->cqh_first->field.cqe_prev = (elm);                                                       \
        (head)->cqh_first = (elm);                                                                           \
    }

#define H4_CIRCLEQ_INSERT_TAIL(head, elm, field)                                                             \
    {                                                                                                        \
        (elm)->field.cqe_next = (void *)(head);                                                              \
        (elm)->field.cqe_prev = (head)->cqh_last;                                                            \
        if ((head)->cqh_first == (void *)(head))                                                             \
            (head)->cqh_first = (elm);                                                                       \
        else                                                                                                 \
            (head)->cqh_last->field.cqe_next = (elm);                                                        \
        (head)->cqh_last = (elm);                                                                            \
    }

#define H4_CIRCLEQ_REMOVE(head, elm, field)                                                                  \
    {                                                                                                        \
        if ((elm)->field.cqe_next == (void *)(head))                                                         \
            (head)->cqh_last = (elm)->field.cqe_prev;                                                        \
        else                                                                                                 \
            (elm)->field.cqe_next->field.cqe_prev = (elm)->field.cqe_prev;                                   \
        if ((elm)->field.cqe_prev == (void *)(head))                                                         \
            (head)->cqh_first = (elm)->field.cqe_next;                                                       \
        else                                                                                                 \
            (elm)->field.cqe_prev->field.cqe_next = (elm)->field.cqe_next;                                   \
    }
#endif /* H4_HQUEUE_H */
