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

/* -------------------------------- herr.c -------------------------------- */
/*
   HDF error handling / reporting routines

LOCAL ROUTINES
  None
EXPORTED ROUTINES
  HEstring -- return error description
  HEclear  -- clear the error stack
  HEpush   -- push an error onto the stack
  HEreport -- give a more detailed error description
  HEprint  -- print values from the error stack
  HEvalue  -- return a error off of the error stack
 */

#include "hdf_priv.h"

/*
 ** Include files for variable argument processing for HEreport
 */
#include <stdarg.h>

/* always points to the next available slot; the last error record is in slot (top-1) */
static int32 error_top = 0;

/* We use a stack to hold the errors plus we keep track of the function,
   file and line where the error occurs. */

/* the structure of the error stack element */
typedef struct error_t {
    hdf_err_code_t error_code;                   /* Error number */
    char           function_name[FUNC_NAME_LEN]; /* function where error occur */
    const char    *file_name;                    /* file where error occur */
    int            line;                         /* line in file where error occurs */
    int            system;                       /* for system or HDF error */
    char          *desc;                         /* optional supplied description */
} error_t;

/* error_messages is the list of error messages in the system, kept as
   error_code-message pairs.  To look up a message, a linear search is
   required but efficiency should be okay. */

typedef struct error_messages_t {
    hdf_err_code_t error_code;
    const char    *str;
} error_messages_t;

static const struct error_messages_t error_messages[] = {
    {DFE_NONE, "No error"},
    /* Low-level I/O errors */
    {DFE_FNF, "File not found"},
    {DFE_DENIED, "Access to file denied"},
    {DFE_ALROPEN, "File already open"},
    {DFE_TOOMANY, "Too Many AID's or files open"},
    {DFE_BADNAME, "Bad file name on open"},
    {DFE_BADACC, "Bad file access mode"},
    {DFE_BADOPEN, "Error opening file"},
    {DFE_NOTOPEN, "File can't be closed; It isn't open"},
    {DFE_CANTCLOSE, "Unable to close file"},
    {DFE_READERROR, "Read error"},
    {DFE_WRITEERROR, "Write error"},
    {DFE_SEEKERROR, "Error performing seek operation"},
    {DFE_RDONLY, "Attempt to write to read-only HDF file"},
    {DFE_BADSEEK, "Attempt to seek past end of element"},
    {DFE_INVFILE, "File is not supported, must be either hdf, cdf, netcdf"},

    /* Low-level HDF I/O errors */
    {DFE_PUTELEM, "Hputelement failed in some way"},
    {DFE_GETELEM, "Hgetelement failed in some way"},
    {DFE_CANTLINK, "Can't initialize link information"},
    {DFE_CANTSYNC, "Cannot synchronize memory with file"},

    /* Old group interface errors */
    {DFE_BADGROUP, "Error from DFdiread in opening a group"},
    {DFE_GROUPSETUP, "Error from DFdisetup in opening a group"},
    {DFE_PUTGROUP, "Error when putting a tag/ref into a group"},
    {DFE_GROUPWRITE, "Error when writing out a group"},

    /* Internal HDF errors */
    {DFE_DFNULL, "DF has a null pointer"},
    {DFE_ILLTYPE, "Internal error: DF has an illegal type"},
    {DFE_BADDDLIST, "Internal error: The DD list is non-existent"},
    {DFE_NOTDFFILE, "This is not an HDF file"},
    {DFE_SEEDTWICE, "Internal error: The DD list is already seeded"},
    {DFE_NOSUCHTAG, "No such tag in the file: search failed"},
    {DFE_NOFREEDD, "There are no free DD's left"},
    {DFE_BADTAG, "Illegal WILDCARD tag"},
    {DFE_BADREF, "Illegal WILDCARD reference"},
    {DFE_NOMATCH, "No (more) DDs which match specified tag/ref"},
    {DFE_NOTINSET, "Set contained unknown tag: ignored"},
    {DFE_BADOFFSET, "Illegal offset specified"},
    {DFE_CORRUPT, "File is corrupted"},
    {DFE_NOREF, "No more reference numbers are available"},
    {DFE_DUPDD, "Tag/ref is already used"},
    {DFE_CANTMOD, "Old element does not exist, cannot modify"},
    {DFE_DIFFFILES, "Attempt to merge objects in different files"},
    {DFE_BADAID, "Unable to create a new AID"},
    {DFE_OPENAID, "There are still active AIDs"},
    {DFE_CANTFLUSH, "Cannot flush the changed DD back to the file"},
    {DFE_CANTUPDATE, "Cannot update the DD block"},
    {DFE_CANTHASH, "Cannot add a DD to the hash table"},
    {DFE_CANTDELDD, "Cannot delete a DD in the file"},
    {DFE_CANTDELHASH, "Cannot delete a DD from the hash table"},
    {DFE_CANTACCESS, "Cannot access specified tag/ref"},
    {DFE_CANTENDACCESS, "Cannot end access to data element"},
    {DFE_TABLEFULL, "Access table is full"},
    {DFE_NOTINTABLE, "Cannot find element in table"},

    /* Generic errors */
    {DFE_UNSUPPORTED, "Feature not currently supported"},
    {DFE_NOSPACE, "Internal error: Out of space"},
    {DFE_BADCALL, "Calls in wrong order"},
    {DFE_BADPTR, "NULL ptr argument"},
    {DFE_BADLEN, "Invalid length specified"},
    {DFE_NOTENOUGH, "Space provided insufficient for size of data"},
    {DFE_NOVALS, "Values not available"},
    {DFE_ARGS, "Invalid arguments to routine"},
    {DFE_INTERNAL, "HDF Internal error"},
    {DFE_NORESET, "Can not reset this value"},
    {DFE_EXCEEDMAX, "Value exceeds max allowed"},
    {DFE_GENAPP, "Generic application-level error"},

    /* Generic interface errors */
    {DFE_UNINIT, "Interface was not initialized correctly"},
    {DFE_CANTINIT, "Can't initialize an interface we depend on"},
    {DFE_CANTSHUTDOWN, "Can't shut down an interface we depend on"},

    /* Dataset errors */
    {DFE_BADDIM, "Negative or zero dimensions specified"},
    {DFE_BADFP, "File contained an illegal floating point number"},
    {DFE_BADDATATYPE, "Unknown or unavailable data type specified"},
    {DFE_BADMCTYPE, "Unknown or unavailable machine type specified"},
    {DFE_BADNUMTYPE, "Unknown or unavailable number type specified"},
    {DFE_BADORDER, "Unknown or illegal array order specified"},
    {DFE_RANGE, "Improper range for attempted access"},
    {DFE_BADCONV, "Don't know how to convert data type"},
    {DFE_BADTYPE, "Incompatible type specified"},
    {DFE_BADDIMNAME, "Dimension name not valid or already taken"},
    {DFE_NOVGREP, "No Vgroup representation for SDS and dim"},

    /* Compression errors */
    {DFE_BADSCHEME, "Unknown compression scheme specified"},
    {DFE_BADMODEL, "Invalid compression model specified"},
    {DFE_BADCODER, "Invalid compression coder specified"},
    {DFE_MODEL, "Error in modeling layer of compression"},
    {DFE_CODER, "Error in encoding layer of compression"},
    {DFE_CINIT, "Error in encoding initialization"},
    {DFE_CDECODE, "Error in decoding compressed data"},
    {DFE_CENCODE, "Error in encoding compressed data"},
    {DFE_CTERM, "Error in encoding termination"},
    {DFE_CSEEK, "Error seeking in encoded dataset"},
    {DFE_MINIT, "Error in modeling initialization"},
    {DFE_COMPINFO, "Invalid compression header"},
    {DFE_CANTCOMP, "Can't compress an object"},
    {DFE_CANTDECOMP, "Can't de-compress an object"},
    {DFE_NOENCODER, "Encoder not available"},
    {DFE_NOSZLIB, "SZIP library not available"},
    {DFE_COMPVERSION, "Z_VERSION_ERROR (-6) returned from zlib"},
    {DFE_READCOMP, "Error in reading compressed data"},

    /* Raster errors */
    {DFE_NODIM, "No dimension record associated with image or data set"},
    {DFE_BADRIG, "Error processing a RIG"},
    {DFE_RINOTFOUND, "Can't find raster image"},
    {DFE_BADATTR, "Bad Attribute"},
    {DFE_LUTNOTFOUND, "No palette information for RIG"},
    {DFE_GRNOTFOUND, "Can't find specified GR"},

    /* SDG/NDG errors */
    {DFE_BADTABLE, "The nsdg table is wrong"},
    {DFE_BADSDG, "Error processing an sdg"},
    {DFE_BADNDG, "Error processing an ndg"},

    /* Vset errors */
    {DFE_VGSIZE, "No more elements will fit in this VGroup"},
    {DFE_VTAB, "Element is not in VSet tables"},
    {DFE_CANTADDELEM, "Cannot add tag/ref to VGroup"},
    {DFE_BADVGNAME, "Cannot set VGroup name"},
    {DFE_BADVGCLASS, "Cannot set VGroup class"},

    /* Vdata errors */
    {DFE_BADFIELDS, "Unable to parse fields string correctly"},
    {DFE_NOVS, "Could not find specified VS or VG in file"},
    {DFE_SYMSIZE, "Too many symbols in table"},
    {DFE_BADATTACH, "Cannot write to a previously attached VData"},
    {DFE_BADVSNAME, "Cannot set VData name"},
    {DFE_BADVSCLASS, "Cannot set VData class"},
    {DFE_VSWRITE, "Error writing to VData"},
    {DFE_VSREAD, "Error reading from VData"},
    {DFE_BADVH, "Error in VData Header"},
    {DFE_FIELDSSET, "Fields already set for vdata"},

    /* High-level Vdata/Vset errors */
    {DFE_VSCANTCREATE, "Cannot create VData"},
    {DFE_VGCANTCREATE, "Cannot create VGroup"},

    /* Generic Vdata/Vset errors */
    {DFE_CANTATTACH, "Cannot attach to a VData"},
    {DFE_CANTDETACH, "Cannot detach a VData with access 'w'"},

    /* XDR level errors */
    {DFE_XDRERROR, "Error from XDR and/or CDF level"},

    /* bit I/O errors */
    {DFE_BITREAD, "There was a bit-read error"},
    {DFE_BITWRITE, "There was a bit-write error"},
    {DFE_BITSEEK, "There was a bit-seek error"},

    /* tbbt interface errors */
    {DFE_TBBTINS, "Failed to insert element into tree"},

    /* bit-vector interface errors */
    {DFE_BVNEW, "Failed to create a bit-vector"},
    {DFE_BVSET, "Failed when setting a bit in a bit-vector"},
    {DFE_BVGET, "Failed when getting a bit in a bit-vector"},
    {DFE_BVFIND, "Failed when finding a bit in a bit-vector"},

    /* General to all interfaces */
    {DFE_CANTSETATTR, "Cannot set an attribute"},
    {DFE_CANTGETATTR, "Cannot find or get an attribute"},

    /* Annotation interface errors */
    {DFE_ANAPIERROR, "Failed in annotation interface"}

};

/* pointer to the structure to hold error messages */
static error_t *error_stack = NULL;

#ifndef DEFAULT_MESG
#define DEFAULT_MESG "Unknown error"
#endif

/* size of error message table */
#define ERRMESG_SZ (sizeof(error_messages) / sizeof(error_messages[0]))

/*------------------------------------------------------------------------
NAME
   HEstring -- return error description
USAGE
   char * HEstring(error_code)
   int16  error_code;      IN: the numerical value of this error
RETURNS
   An error description string
DESCRIPTION
   Return a textual description of the given error.  These strings
   are statically declared and should not be free()ed by the user.
   If no string can be found to describe this error a generic
   default message is returned.

---------------------------------------------------------------------------*/
const char *
HEstring(hdf_err_code_t error_code)
{
    int i; /* temp int index */

    /* look for the error_code in error message table */
    for (i = 0; i < (int)ERRMESG_SZ; i++)
        if (error_messages[i].error_code == error_code) {
            return error_messages[i].str;
        }

    /* otherwise, return default message */
    return DEFAULT_MESG;
} /* HEstring */

/*--------------------------------------------------------------------------
NAME
   HEclear -- clear the error stack
USAGE
   void HEclear(void)
RETURNS
   NONE
DESCRIPTION
   Remove all currently reported errors from the error stack

---------------------------------------------------------------------------*/
void
HEclear(void)
{
    if (!error_top)
        goto done;

    /* error_top == 0 means no error in stack */
    /* clean out old descriptions if they exist */
    for (; error_top > 0; error_top--) {
        free(error_stack[error_top - 1].desc);
        error_stack[error_top - 1].desc = NULL;
    }

done:
    return;
} /* HEclear */

/*-------------------------------------------------------------------------
NAME
   HEpush -- push an error onto the stack
USAGE
   void HEpush(error_code, func_name, file_name, line)
   int16  error_code;      IN: the numerical value of this error
   char * func_name;       IN: function where the error happened
   char * file_name;       IN: file name of offending function
   int    line;            IN: line number of the reporting statement
RETURNS
   NONE
DESCRIPTION
   push a new error onto stack.  If stack is full, error
   is ignored.  assumes that the character strings
   (function_name and file_name) referred are in some
   semi-permanent storage, so it just saves the pointer
   to the strings.  blank out the description field so
   that a description is reported  only if REreport is called

---------------------------------------------------------------------------*/
void
HEpush(hdf_err_code_t error_code, const char *function_name, const char *file_name, int line)
{
    int i;

    /* if the stack is not allocated, then do it */
    if (!error_stack) {
        error_stack = (error_t *)malloc((uint32)sizeof(error_t) * ERR_STACK_SZ);
        if (!error_stack) {
            puts("HEpush cannot allocate space.  Unable to continue!!");
            exit(8);
        }
        for (i = 0; i < ERR_STACK_SZ; i++)
            error_stack[i].desc = NULL;
    }

    /* if stack is full, discard error */
    /* otherwise, push error details onto stack */

    if (error_top < ERR_STACK_SZ) {
        strcpy(error_stack[error_top].function_name, function_name);
        error_stack[error_top].file_name  = file_name;
        error_stack[error_top].line       = line;
        error_stack[error_top].error_code = error_code;
        free(error_stack[error_top].desc);
        error_stack[error_top].desc = NULL;
        error_top++;
    }
} /* HEpush */

/*-------------------------------------------------------------------------
NAME
   HEreport -- give a more detailed error description
USAGE
   void HEreport(format, ....)
   char * format;           IN: printf style print statement
RETURNS
   NONE
DESCRIPTION
   Using printf and the variable number of args facility allow the
   library to specify a more detailed description of a given
   error condition

---------------------------------------------------------------------------*/
void
HEreport(const char *format, ...)
{
    va_list arg_ptr;
    char   *tmp;

    va_start(arg_ptr, format);

    if ((error_top < ERR_STACK_SZ + 1) && (error_top > 0)) {
        tmp = (char *)malloc(ERR_STRING_SIZE);
        if (!tmp) {
            HERROR(DFE_NOSPACE);
            goto done;
        }
        vsprintf(tmp, format, arg_ptr);
        free(error_stack[error_top - 1].desc);
        error_stack[error_top - 1].desc = tmp;
    }

    va_end(arg_ptr);

done:
    return;
} /* HEreport */

/*-------------------------------------------------------------------------
NAME
   HEprint -- print values from the error stack
USAGE
   void HEprint(stream, levels)
   FILE * stream;      IN: file to print error message to
   int32  level;       IN: level at which to start printing
RETURNS
   NONE
DESCRIPTION
   Print part of the error stack to a given file.  If level == 0
   the entire stack is printed.  If an extra description has been
   added (via HEreport) it is printed too.

---------------------------------------------------------------------------*/
void
HEprint(FILE *stream, int32 print_levels)
{
    if (print_levels == 0 || print_levels > error_top) /* print all errors */
        print_levels = error_top;

    /* print the errors starting from most recent */
    for (print_levels--; print_levels >= 0; print_levels--) {
        fprintf(stream, "HDF error: (%d) <%s>\n\tDetected in %s() [%s line %d]\n",
                error_stack[print_levels].error_code, HEstring(error_stack[print_levels].error_code),
                error_stack[print_levels].function_name, error_stack[print_levels].file_name,
                error_stack[print_levels].line);
        if (error_stack[print_levels].desc)
            fprintf(stream, "\t%s\n", error_stack[print_levels].desc);
    }
} /* HEprint */

/* ------------------------------- HEvalue -------------------------------- */
/*

   NAME
   HEvalue -- return a error off of the error stack
   USAGE
   int16 HEvalue(level)
   int32 level;           IN: level of the error stack to return
   RETURNS
   Error code or DFE_NONE if no error
   DESCRIPTION
   Return the error code of a single error out of the error stack

   --------------------------------------------------------------------------- */
int16
HEvalue(int32 level)
{
    int16 ret_value = DFE_NONE;

    if (level > 0 && level <= error_top)
        ret_value = (int16)error_stack[error_top - level].error_code;
    else
        ret_value = DFE_NONE;

    return ret_value;
} /* HEvalue */

/*--------------------------------------------------------------------------
 NAME
    HEshutdown
 PURPOSE
    Terminate various static buffers.
 USAGE
    int HEshutdown()
 RETURNS
    Returns SUCCEED/FAIL
 DESCRIPTION
    Free various buffers allocated in the HE routines.
 GLOBAL VARIABLES
 COMMENTS, BUGS, ASSUMPTIONS
    Should only ever be called by the "atexit" function HDFend
 EXAMPLES
 REVISION LOG
--------------------------------------------------------------------------*/
int
HEshutdown(void)
{
    if (error_stack != NULL) {
        free(error_stack);
        error_stack = NULL;
        error_top   = 0;
    }
    return SUCCEED;
} /* end HEshutdown() */
