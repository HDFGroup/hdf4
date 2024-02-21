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

/*  herr_priv.h
 *
 *  Error routines (private)
 */

#ifndef H4_HERR_PRIV_H
#define H4_HERR_PRIV_H

#include "hproto.h"

/* HERROR macro, used to facilitate error reporting */
#define HERROR(e) HEpush(e, __func__, __FILE__, __LINE__)

/* HRETURN_ERROR macro, used to facilitate error reporting.  Makes
 * same assumptions as HERROR.  IN ADDITION, this macro causes
 * a return from the calling routine
 */

#define HRETURN_ERROR(err, ret_val)                                                                          \
    do {                                                                                                     \
        HERROR(err);                                                                                         \
        return (ret_val);                                                                                    \
    } while (0)

/* HGOTO_ERROR macro, used to facilitate error reporting.  Makes
 * same assumptions as HERROR.  IN ADDITION, this macro causes
 * a jump to the label 'done' which should be in every function
 * Also there is an assumption of a variable 'ret_value'
 */

#define HGOTO_ERROR(err, ret_val)                                                                            \
    do {                                                                                                     \
        HERROR(err);                                                                                         \
        ret_value = ret_val;                                                                                 \
        goto done;                                                                                           \
    } while (0)

/* HGOTO_FAIL macro, used to facilitate error reporting.  This one
 * is added in 2019 to tidy the code.  It is similar to HGOTO_ERROR,
 * except it does not call HERROR.  This is to preserve the exact
 * behavior as the original code.
 */

#define HGOTO_FAIL(ret_val)                                                                                  \
    do {                                                                                                     \
        ret_value = ret_val;                                                                                 \
        goto done;                                                                                           \
    } while (0)

/* HCLOSE_GOTO_ERROR macro, used to facilitate error reporting.  Makes
 * same assumptions as HRETURN_ERROR.  IN ADDITION, this macro causes
 * the file specified by the id "fid" to be closed
 * Also , this macro causes a jump to the label 'done' which should
 * be in every function. There is an assumption of a variable 'ret_value'
 */

/* TODO: Refactor this out of library and simply close hfid in the `done` target */

#define HCLOSE_GOTO_ERROR(hfid, err, ret_val)                                                                \
    do {                                                                                                     \
        HERROR(err);                                                                                         \
        Hclose(hfid);                                                                                        \
        ret_value = ret_val;                                                                                 \
        goto done;                                                                                           \
    } while (0)

/* HGOTO_DONE macro, used to facilitate the new error reporting model.
 * This macro is just a wrapper to set the return value and jump to the 'done'
 * label.  Also assumption of a variable 'ret_value'
 */

#define HGOTO_DONE(ret_val)                                                                                  \
    do {                                                                                                     \
        ret_value = ret_val;                                                                                 \
        goto done;                                                                                           \
    } while (0)

/*******************************/
/* For further error reporting */
/*******************************/

#define HE_REPORT(msg) HEreport(msg)

#define HE_REPORT_RETURN(msg, ret_val)                                                                       \
    do {                                                                                                     \
        HEreport(msg);                                                                                       \
        return (ret_val);                                                                                    \
    } while (0)

#define HE_CLOSE_REPORT_RETURN(hfid, msg, ret_val)                                                           \
    do {                                                                                                     \
        HEreport(msg);                                                                                       \
        Hclose(hfid);                                                                                        \
        return (ret_val);                                                                                    \
    } while (0)

#define HE_REPORT_GOTO(msg, ret_val)                                                                         \
    do {                                                                                                     \
        HEreport(msg);                                                                                       \
        ret_value = ret_val;                                                                                 \
        goto done;                                                                                           \
    } while (0)

#define HE_CLOSE_REPORT_GOTO(hfid, msg, ret_val)                                                             \
    do {                                                                                                     \
        HEreport(msg);                                                                                       \
        Hclose(hfid);                                                                                        \
        ret_value = ret_val;                                                                                 \
        goto done;                                                                                           \
    } while (0)

#endif /* H4_HERR_PRIV_H */
