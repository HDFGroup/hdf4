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

#ifndef H4_HFILE_ATEXIT_PRIV_H
#define H4_HFILE_ATEXIT_PRIV_H

#include "hdf.h"

/* HDF4 allows external registration of atexit() functions via the
 * HPregister_term_func() call. Several internal source files will also
 * register their own shutdown handlers via this mechanism.
 *
 * Since any numbrer of shutdown handlers can be called, they must
 * be stored in a dynamic structure. The library currently registers
 * 7 handlers.
 */

/** The initial size of the handler array, in number of exit functions,
 *  and the size by which the array is incremented when full. It is assumed
 *  that there will not be a lot of error handlers.
 */
#define ATEXIT_ARRAY_INCR 16

/** Functions that execute when the library shuts down */
typedef struct hfile_atexit_t {
    hdf_termfunc_t *functions;     /** Array of exit functions */
    size_t          n_functions;   /** Number of exit functions registered */
    size_t          max_functions; /** Allocated size of the exit function array */
} hfile_atexit_t;

HDFLIBAPI int hfile_atexit_create(hfile_atexit_t **ha);
HDFLIBAPI int hfile_atexit_add(hfile_atexit_t *ha, hdf_termfunc_t func);
HDFLIBAPI int hfile_atexit_execute_all(hfile_atexit_t *ha);
HDFLIBAPI int hfile_atexit_destroy(hfile_atexit_t **ha);

#endif /* H4_HFILE_ATEXIT_PRIV_H */
