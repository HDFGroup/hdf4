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

#include <stdlib.h>

#include "hdf_priv.h"
#include "hfile_atexit_priv.h"

/**
 * \internal
 * \brief Resizes an atexit context so it can hold more functions
 *
 * \param[in] ha    The atexit context
 *
 * \return SUCCEED/FAIL
 *
 * \details Resizes the functions array by ATEXIT_ARRAY_INCR. Does not
 *          modify the array if the resize fails.
 */
static int hfile_atexit_resize(hfile_atexit_t *ha);

int
hfile_atexit_create(hfile_atexit_t **ha)
{
    hfile_atexit_t *new_ha    = NULL;
    int             ret_value = SUCCEED;

    if (ha == NULL)
        HGOTO_ERROR(DFE_INTERNAL, FAIL);

    new_ha = (hfile_atexit_t *)calloc(1, sizeof(hfile_atexit_t));
    if (new_ha == NULL)
        HGOTO_ERROR(DFE_INTERNAL, FAIL);

    new_ha->functions = (hdf_termfunc_t *)calloc(ATEXIT_ARRAY_INCR, sizeof(hdf_termfunc_t));
    if (new_ha->functions == NULL)
        HGOTO_ERROR(DFE_INTERNAL, FAIL);

    new_ha->n_functions   = 0;
    new_ha->max_functions = ATEXIT_ARRAY_INCR;

    *ha = new_ha;

    return ret_value;
done:
    if (new_ha)
        free(new_ha->functions);
    free(new_ha);

    return ret_value;
}

static int
hfile_atexit_resize(hfile_atexit_t *ha)
{
    size_t          new_size  = 0;
    hdf_termfunc_t *temp      = NULL;
    int             ret_value = SUCCEED;

    new_size = (ha->max_functions + ATEXIT_ARRAY_INCR) * sizeof(hdf_termfunc_t);

    temp = (hdf_termfunc_t *)realloc(ha->functions, new_size);
    if (NULL == temp)
        HGOTO_ERROR(DFE_CANTINIT, FAIL);

    ha->functions = temp;
    ha->max_functions += ATEXIT_ARRAY_INCR;

    return ret_value;
done:
    return ret_value;
}

int
hfile_atexit_add(hfile_atexit_t *ha, hdf_termfunc_t func)
{
    int ret_value = SUCCEED;

    if (ha == NULL)
        HGOTO_ERROR(DFE_INTERNAL, FAIL);

    /* Resize, if necessary */
    if (ha->n_functions == ha->max_functions)
        if (hfile_atexit_resize(ha) < 0)
            HGOTO_ERROR(DFE_INTERNAL, FAIL);

    ha->functions[ha->n_functions] = func;
    ha->n_functions += 1;

    return ret_value;
done:
    return ret_value;
}

int
hfile_atexit_execute_all(hfile_atexit_t *ha)
{
    int ret_value = SUCCEED;

    if (ha == NULL)
        HGOTO_ERROR(DFE_INTERNAL, FAIL);

    for (size_t i = 0; i < ha->n_functions; i++)
        ha->functions[i]();

    return ret_value;
done:
    return ret_value;
}

int
hfile_atexit_destroy(hfile_atexit_t **ha)
{
    /* Passing NULL to cleanup code is okay */
    if (ha == NULL || *ha == NULL)
        return SUCCEED;

    free((*ha)->functions);
    free(*ha);
    *ha = NULL;

    return SUCCEED;
}
