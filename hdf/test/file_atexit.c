/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Copyright by The HDF Group.                                               *
 *                                                                           *
 * This file is part of HDF.  The full HDF copyright notice, including       *
 * terms governing use, modification, and redistribution, is contained in    *
 * the COPYING file, which can be found at the root of the source code       *
 * distribution tree, or in https://support.hdfgroup.org/ftp/HDF/releases/.  *
 * If you do not have access to either file, you may request a copy from     *
 * help@hdfgroup.org.                                                        *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "testhdf.h"
#include "hfile_atexit_priv.h"

/* Tests the functionality of the hfile_atexit.c code. This is used to
 * store function pointers that will be invoked when the atexit() handler
 * runs.
 */

static size_t count_g = 0;

int
add_one(void)
{
    count_g += 1;

    return SUCCEED;
}

int
add_two(void)
{
    count_g += 2;

    return SUCCEED;
}

void
test_hfile_atexit(void)
{
    size_t          n_functions  = 0; /* number of functions added */
    size_t          actual_count = 0; /* What count_g should be after execute all */
    hfile_atexit_t *ha           = NULL;
    int             ret          = FAIL;

    /*****************************************/
    /* Call each API call with NULL pointers */
    /*****************************************/

    /* These should fail */
    ret = hfile_atexit_create(NULL);
    CHECK_VOID(ret, SUCCEED, "hfile_atexit_create");

    ret = hfile_atexit_add(NULL, add_one);
    CHECK_VOID(ret, SUCCEED, "hfile_atexit_add");
    ret = hfile_atexit_execute_all(NULL);
    CHECK_VOID(ret, SUCCEED, "hfile_atexit_execute_all");

    ret = hfile_atexit_add(ha, add_one);
    CHECK_VOID(ret, SUCCEED, "hfile_atexit_add");
    ret = hfile_atexit_execute_all(ha);
    CHECK_VOID(ret, SUCCEED, "hfile_atexit_execute_all");

    /* These should succeed */
    ret = hfile_atexit_destroy(NULL);
    CHECK_VOID(ret, FAIL, "hfile_atexit_destroy");
    ret = hfile_atexit_destroy(&ha);
    CHECK_VOID(ret, FAIL, "hfile_atexit_destroy");

    /****************************/
    /* Make sure creation works */
    /****************************/

    ret = hfile_atexit_create(&ha);
    CHECK_VOID(ret, FAIL, "hfile_atexit_create");
    CHECK_ALLOC(ha, "ha", "hfile_atexit_create");
    VERIFY_VOID(ha->n_functions, 0, "hfile_atexit_create");
    VERIFY_VOID(ha->max_functions, ATEXIT_ARRAY_INCR, "hfile_atexit_create");

    /******************/
    /* Add a function */
    /******************/

    ret = hfile_atexit_add(ha, add_one);
    CHECK_VOID(ret, FAIL, "hfile_atexit_add");
    n_functions += 1;
    actual_count += 1;

    VERIFY_VOID(ha->n_functions, n_functions, "hfile_atexit_add");
    VERIFY_VOID(ha->max_functions, ATEXIT_ARRAY_INCR, "hfile_atexit_add");

    /*******************************************/
    /* Add enough functions to force a realloc */
    /*******************************************/

    for (size_t i = 0; i < ATEXIT_ARRAY_INCR; i++) {
        ret = hfile_atexit_add(ha, add_one);
        CHECK_VOID(ret, FAIL, "hfile_atexit_add");

        n_functions += 1;
        actual_count += 1;
        VERIFY_VOID(ha->n_functions, n_functions, "hfile_atexit_add");
    }
    VERIFY_VOID(ha->max_functions, (2 * ATEXIT_ARRAY_INCR), "hfile_atexit_add");

    /***********************************/
    /* Make sure functions are invoked */
    /***********************************/

    ret = hfile_atexit_execute_all(ha);
    CHECK_VOID(ret, FAIL, "hfile_atexit_execute_all");

    /* Each function adds one to count_g */
    VERIFY_VOID(count_g, actual_count, "hfile_atexit_execute_all");

    /* The functions don't get cleared out after use */
    VERIFY_VOID(ha->n_functions, n_functions, "hfile_atexit_execute_all");
    VERIFY_VOID(ha->max_functions, (2 * ATEXIT_ARRAY_INCR), "hfile_atexit_execute_all");

    /* reset count */
    count_g = 0;

    /* Execute the functions again */
    ret = hfile_atexit_execute_all(ha);
    CHECK_VOID(ret, FAIL, "hfile_atexit_execute_all");

    /* Each function adds one to count_g */
    VERIFY_VOID(count_g, actual_count, "hfile_atexit_execute_all");

    /*************************************************/
    /* Add enough functions to force ANOTHER realloc */
    /*************************************************/

    for (size_t i = 0; i < ATEXIT_ARRAY_INCR; i++) {
        ret = hfile_atexit_add(ha, add_two);
        CHECK_VOID(ret, FAIL, "hfile_atexit_add");

        /* Each function adds two to count_g */
        actual_count += 2;
        n_functions += 1;
        VERIFY_VOID(ha->n_functions, n_functions, "hfile_atexit_add");
    }
    VERIFY_VOID(ha->max_functions, (3 * ATEXIT_ARRAY_INCR), "hfile_atexit_add");

    /***********************************/
    /* Make sure functions are invoked */
    /***********************************/

    /* reset count */
    count_g = 0;

    ret = hfile_atexit_execute_all(ha);
    CHECK_VOID(ret, FAIL, "hfile_atexit_execute_all");

    /* Check count_g */
    VERIFY_VOID(count_g, actual_count, "hfile_atexit_execute_all");

    /* The functions don't get cleared out after use */
    VERIFY_VOID(ha->n_functions, n_functions, "hfile_atexit_execute_all");
    VERIFY_VOID(ha->max_functions, (3 * ATEXIT_ARRAY_INCR), "hfile_atexit_execute_all");

    /****************************/
    /* Make sure shutdown works */
    /****************************/

    ret = hfile_atexit_destroy(&ha);
    CHECK_VOID(ret, FAIL, "hfile_atexit_destroy");

    /* Destroy sets the passed-in pointer to NULL */
    VERIFY_VOID(ha, NULL, "hfile_atexit_execute_all");
}
