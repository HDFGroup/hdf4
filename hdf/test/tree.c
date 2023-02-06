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
   tree.c
   Test HDF Threaded-Balanced-Binary Tree (tbbt) routines.

   REMARKS

   DESIGN

   BUGS/LIMITATIONS

   EXPORTED ROUTINES

   AUTHOR
   Quincey Koziol

   MODIFICATION HISTORY
   10/21/93 - Started coding.
 */

#include <time.h>
#include "tproto.h"
#include "tbbt.h"

#define MAX_TEST_SIZE 31  /* maximum number of elements to insert */
#define NUM_TEST_RUNS 100 /* number of times to insert & remove each size */

#define SEED(s)       (srand(s))
#define RandInt(a, b) ((rand() % (((b) - (a)) + 1)) + (a))

static void swap_arr(int32 *arr, intn a, intn b);

intn tcompare(void *k1, void *k2, intn cmparg);

static void
swap_arr(int32 *arr, intn a, intn b)
{
    int32 t;

    if (a != b) {
        t      = arr[a];
        arr[a] = arr[b];
        arr[b] = t;
    } /* end if */
} /* end swap_arr() */

intn
tcompare(void *k1, void *k2, intn cmparg)
{
    (void)cmparg;
    return ((intn)((*(int32 *)k1) - (*(int32 *)k2)));
}

void
test_tbbt(void)
{
    intn       test_size;
    intn       i, j;
    int32      ins_arr[MAX_TEST_SIZE];
    int32      rem_arr[MAX_TEST_SIZE];
    intn       t;
    TBBT_TREE *tree;
    void     **r;

    t = (intn)time(NULL);
    SEED((uintn)t);

    for (test_size = 3; test_size <= MAX_TEST_SIZE; test_size++) {
        MESSAGE(7, printf("\nTesting trees with %d elements\n", test_size););
        MESSAGE(8, printf("Testing tree #:"););
        for (j = 0; j < NUM_TEST_RUNS; j++) {
            MESSAGE(8, printf(" %d", j););
            for (i = 0; i < test_size; i++) { /* initialize the arrays */
                ins_arr[i] = i;
                rem_arr[i] = i;
            }                                 /* end for */
            for (i = 0; i < test_size; i++) { /* shuffle the arrays */
                t = RandInt(i, test_size - 1);
                swap_arr(ins_arr, i, t);
                t = RandInt(i, test_size - 1);
                swap_arr(rem_arr, i, t);
            } /* end for */

            if (Verbosity > 9) {
                printf("ins_arr: \n");
                for (i = 0; i < test_size; i++) /* print the arrays */
                    printf("%d \n", (int)ins_arr[i]);
                printf("\nrem_arr: \n");
                for (i = 0; i < test_size; i++) /* print the arrays */
                    printf("%d \n", (int)rem_arr[i]);
                printf("\n");
            } /* end if */

            tree = tbbtdmake(tcompare, sizeof(int32), 0);
            for (i = 0; i < test_size; i++) {
                MESSAGE(9, printf("inserting %d\n", (int)ins_arr[i]););
                tbbtdins(tree, (void *)&ins_arr[i], NULL);
                MESSAGE(9, tbbtdump(tree, -1););
            }
            MESSAGE(9, tbbtdump(tree, -1););
            for (i = 0; i < test_size; i++) {
                int32 key;

                key = rem_arr[i];
                r   = (void **)tbbtdfind(tree, (void *)&key, NULL);
                MESSAGE(9, printf("removing %d\n", (int)key););
                tbbtrem((TBBT_NODE **)tree, (TBBT_NODE *)r, NULL);
                MESSAGE(9, tbbtdump(tree, -1););
            } /* end for */
            tbbtdfree(tree, NULL, NULL);
        } /* end for */
    }     /* end for */
} /* end test_tbbt() */
