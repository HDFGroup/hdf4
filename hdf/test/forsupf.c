/****************************************************************************
 * NCSA HDF                                                                 *
 * Software Development Group                                               *
 * National Center for Supercomputing Applications                          *
 * University of Illinois at Urbana-Champaign                               *
 * 605 E. Springfield, Champaign IL 61820                                   *
 *                                                                          *
 * For conditions of distribution and use, see the accompanying             *
 * hdf/COPYING file.                                                        *
 *                                                                          *
 ****************************************************************************/

#ifdef RCSID
static char RcsId[] = "$Revision$";
#endif

/* $Id$ */

#include "hdf.h"
#include "fortest.h"

/*-----------------------------------------------------------------------------
 * Name:    getverb
 * Purpose: Get the verbosity from the "HDF_VERBOSITY" environment variable
 *          and return it to the FORTRAN calling routine.
 * Inputs:  NONE
 * Returns: verbosity level on success, FAIL on failure
 * Users:   HDF Fortran programmers
 * Invokes: HDgetenv
 *---------------------------------------------------------------------------*/

FRETVAL(intf)
ngetverb(void)
{
    char *verb_str;
    intn verb_level=FAIL;
#ifdef vms
    return(4);
#else
    verb_str=HDgetenv(FOR_VERB);

    if(verb_str!=NULL)
        verb_level=(intn)HDstrtol(verb_str,NULL,0); /* convert whole string using base 10 */
    return((intf)verb_level);
#endif
}   /* end getverb() */

/*-----------------------------------------------------------------------------
 * Name:    hisystem
 * Purpose: Invoke the system call to execute cmd
 * Inputs:  cmd -- the command to execute
 * Returns: verbosity level on success, FAIL on failure
 * Users:   HDF Fortran programmers
 * Invokes: HDgetenv
 *---------------------------------------------------------------------------*/

FRETVAL(intf)
nhisystem(_fcd cmd, intf *cmdlen)
{
    char       *fn;
    intf        ret;

    fn = HDf2cstring(cmd, (intn) *cmdlen);
    if (!fn) return(FAIL);
    ret = (intf) system(fn);
    HDfree(fn);
    return (ret);
}   /* end nhisystem() */

