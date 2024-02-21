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
 * File:    herrf.c
 * Purpose: C stubs for error-handling Fortran routines
 * Invokes: herr.c.
 * Contents:
 *     heprnt_:    Call HEprint to print error message
 * Remarks: none
 *---------------------------------------------------------------------------*/

#include "hdf_priv.h"
#include "hproto_fortran.h"

/*-----------------------------------------------------------------------------
 * Name:    heprnt
 * Purpose: call HEprint to print error messages, starting from top of stack
 * Inputs:  print_levels: number of levels to print
 * Returns: 0 on success, FAIL on failure
 * Users:   Fortran stub routine
 * Invokes: HEprint
 * Remarks: This routine has one less parameter than HEprint, because it
 *          doesn't allow the user to specify the stream to print to.
 *          Instead it prints automatically to stdout.
 *---------------------------------------------------------------------------*/

void
nheprnt(intf *print_levels)
{
    HEprint(stderr, *print_levels);
}
/*-----------------------------------------------------------------------------
 * Name:    heprntc
 * Purpose: call HEprint to print error messages, starting from top of stack
 * Inputs:  print_levels: number of levels to print
 * Returns: 0 on success, FAIL on failure
 * Users:   Fortran stub routine
 * Invokes: HEprint
 * Remarks: This routine has one less parameter than HEprint, because it
 *          doesn't allow the user to specify the stream to print to.
 *          Instead it prints automatically to stdout.
 *---------------------------------------------------------------------------*/

intf
nheprntc(_fcd filename, intf *print_levels, intf *namelen)
{
    FILE *err_file;
    char *c_name;
    intn  c_len;
    int   ret = 0;

    c_len = *namelen;
    if (c_len == 0) {
        HEprint(stderr, *print_levels);
        return ret;
    }
    c_name = HDf2cstring(filename, c_len);
    if (!c_name)
        return FAIL;
    err_file = fopen(c_name, "a");
    if (!err_file)
        return FAIL;
    HEprint(err_file, *print_levels);
    fclose(err_file);
    return ret;
}
/*-----------------------------------------------------------------------------
 * Name: hestringc
 * Purpose:  Calls HEstring
 * Inputs:   error_code - HDF error code
 * Outputs: error_message - error message assocoated with the error code
 * Returns: SUCCEED (0) if successful and FAIL(-1) otherwise
 *----------------------------------------------------------------------------*/
intf
nhestringc(intf *error_code, _fcd error_message, intf *len)
{
    char *cstring = NULL;
    intn  status;

    status  = -1;
    cstring = (char *)HEstring((hdf_err_code_t)*error_code);
    if (cstring) {
        status = 0;
        HDpackFstring(cstring, _fcdtocp(error_message), *len);
    }
    return status;
}
