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

/* $Id$ */

/*+ herr.h
***  header file for using error routines
*** to be included by all ".c" files
+*/

#ifndef __HERR_H
#define __HERR_H

/* if these symbols are not provided by the compiler, we'll have to
   fake them.  These are used in HERROR for recording location of
   error in code. */

#ifndef __FILE__
#   define __FILE__ "File name not supported"
#endif
#ifndef __LINE__
#   define __LINE__ 0
#endif

/* HERROR macro, used to facilitate error reporting.  Assumes that
   there's a variable called FUNC which holds the function name.
   Assume that func and file are both stored in static space, or at
   least be not corrupted in the meanwhile. */

#define HERROR(e) HEpush(e, FUNC, __FILE__, __LINE__)

/* HRETURN_ERROR macro, used to facilitate error reporting.  Makes
   same assumptions as HERROR.  IN ADDITION, this macro causes
   a return from the calling routine */

#define HRETURN_ERROR(err, ret_val) {HERROR(err); return(ret_val);}

/* HCLOSE_RETURN_ERROR macro, used to facilitate error reporting.  Makes
   same assumptions as HRETURN_ERROR.  IN ADDITION, this macro causes
   the file specified by the id "fid" to be closed */

#define HCLOSE_RETURN_ERROR(hfid, err, ret_val) {HERROR(err); Hclose(hfid); \
                                                return(ret_val);}

/*
======================================================================
   Error codes

   NOTE: Remember to update the error_messages[] structure in herr.c
   whenever errors are added/deleted from this list.
======================================================================
*/
/* Declare an enumerated type which holds all the valid HDF error codes */
typedef enum {
    DFE_NONE=0,         /* special zero, no error */
    DFE_FNF,            /* File not found */
    DFE_DENIED,         /* Access to file denied */
    DFE_ALROPEN,        /* File already open */
    DFE_TOOMANY,        /* Too Many AID's or files open */
    DFE_BADNAME,        /* Bad file name on open */
    DFE_BADACC,         /* Bad file access mode */
    DFE_BADOPEN,        /* Other open error */
    DFE_NOTOPEN,        /* File can't be closed 'cause it isn't open */
    DFE_CANTCLOSE,      /* fclose wouldn't work! */
    DFE_DFNULL,         /* DF is a null pointer */
    DFE_ILLTYPE,        /* DF has an illegal type: internal error */
    DFE_UNSUPPORTED,    /* Feature not currently supported */
    DFE_BADDDLIST,      /* The DD list is non-existent: internal error */
    DFE_NOTDFFILE,      /* This is not a DF file and it is not 0 length */
    DFE_SEEDTWICE,      /* The DD list already seeded: internal error */
    DFE_NOSPACE,        /* Malloc failed */
    DFE_NOSUCHTAG,      /* No such tag in the file: search failed */
    DFE_READERROR,      /* There was a read error */
    DFE_WRITEERROR,     /* There was a write error */
    DFE_SEEKERROR,      /* There was a seek error */
    DFE_NOFREEDD,       /* There are no free DD's left: internal error */
    DFE_BADTAG,         /* illegal WILDCARD tag */
    DFE_BADREF,         /* illegal WILDCARD reference # */
    DFE_RDONLY,         /* The DF is read only */
    DFE_BADCALL,        /* Calls in wrong order */
    DFE_BADPTR,         /* NULL ptr argument */
    DFE_BADLEN,         /* Invalid len specified */
    DFE_BADSEEK,        /* Attempt to seek past end of element */
    DFE_NOMATCH,        /* No (more) DDs which match specified tag/ref */
    DFE_NOTINSET,       /* Warning: Set contained unknown tag: ignored */
    DFE_BADDIM,         /* negative or zero dimensions specified */
    DFE_BADOFFSET,      /* Illegal offset specified */
    DFE_BADSCHEME,      /* Unknown compression scheme specified */
    DFE_NODIM,          /* No dimension record associated with image */
    DFE_NOTENOUGH,      /* space provided insufficient for size of data */
    DFE_NOVALS,         /* Values not available */
    DFE_CORRUPT,        /* File is corrupted */
    DFE_BADFP,          /* File contained an illegal floating point num */
    DFE_NOREF,          /* no more reference numbers are available */
    DFE_BADDATATYPE,    /* unknown or unavailable data type specified */
    DFE_BADMCTYPE,      /* unknown or unavailable machine type specified */
    DFE_BADNUMTYPE,     /* unknown or unavailable number type specified */
    DFE_BADORDER,       /* unknown or illegal array order specified */
    DFE_ARGS,           /* bad arguments to routine */
    DFE_INTERNAL,       /* serious internal error */
    DFE_DUPDD,          /* the new tag/ref is already used */
    DFE_CANTMOD,        /* old element not exist, cannot modify */
    DFE_RANGE,          /* improper range for attempted acess */
    DFE_BADTABLE,       /* the nsdg table is wrong   */
    DFE_BADSDG,         /* error processing an sdg    */
    DFE_BADNDG,         /* error processing an ndg     */
    DFE_BADFIELDS,      /* Bad fields string passed to Vset routine */
    DFE_NORESET,        /* Too late to modify this value */
    DFE_NOVS,           /* Counldn't find VS in file */
    DFE_VGSIZE,         /* Too many elements in VGroup */
    DFE_DIFFFILES,      /* Attempt to merge objs in diff files */
    DFE_VTAB,           /* Elmt not in vtab[] */
    DFE_BADAID,         /* Got a bogus aid */
    DFE_OPENAID,        /* There are still active AIDs */
    DFE_BADCONV,        /* Don't know how to convert data type */
    DFE_GENAPP,         /* Generic application,level error */
    DFE_CANTFLUSH,      /* Can't flush DD back to file */
    DFE_BADTYPE,        /* Incompatible types specified */
    DFE_SYMSIZE,        /* Too many symbols in users table */
    DFE_BADATTACH,      /* Cannot write to a previously attached VData */
    DFE_CANTATTACH,     /* Cannot attach to a VData */
    DFE_CANTDETACH,     /* Cannot detach a VData with access 'w' */
    DFE_CANTUPDATE,     /* Cannot update the DD block */
    DFE_CANTHASH,       /* Cannot add a DD to the hash table */
    DFE_CANTDELDD,      /* Cannot delete a DD in the file */
    DFE_CANTDELHASH,    /* Cannot delete a DD from the hash table */
    DFE_BADMODEL,       /* Invalid compression model specified */
    DFE_BADCODER,       /* Invalid compression encoder specified */
    DFE_MODEL,          /* Error in modeling layer of compression */
    DFE_CODER,          /* Error in encoding layer of compression */
    DFE_CINIT,          /* Error in encoding initialization */
    DFE_CDECODE,        /* Error in decoding compressed data */
    DFE_CENCODE,        /* Error in encoding compressed data */
    DFE_CTERM,          /* Error in encoding termination */
    DFE_CSEEK,          /* Error seekging in encoded dataset */
    DFE_MINIT,          /* Error in modeling initialization */
    DFE_COMPINFO,       /* Invalid compression header */
    DFE_BADRIG,         /* Error processing a RIG */
    DFE_PUTELEM,        /* Hputelement failed in some way */
    DFE_GETELEM,        /* Hgetelement failed in some way */
    DFE_BADGROUP,       /* Error from DFdiread in opening a group */
    DFE_GROUPSETUP,     /* Error from DFdisetup in opening a group */
    DFE_PUTGROUP,       /* Error when putting a tag/ref into a group */
    DFE_CANTCOMP,       /* Can't compress an object */
    DFE_CANTDECOMP,     /* Can't de-compress an object */
    DFE_UNINIT,         /* Interface was not initialized correctly */
    DFE_CANTINIT,       /* Can't initialize an interface we depend on */
    DFE_CANTSHUTDOWN,   /* Can't shut down an interface we depend on */
    DFE_BITREAD,        /* There was a bit-read error */
    DFE_BITWRITE,       /* There was a bit-write error */
    DFE_BITSEEK,        /* There was a bit-seek error */
    DFE_CANTLINK,       /* Can't initialize link information */
    DFE_CANTSYNC        /*Cannot syncronize memory with file */
} hdf_err_code_t;

#ifdef _H_ERR_MASTER_

/* error_messages is the list of error messages in the system, kept as
   error_code-message pairs.  To look up a message, a linear search is
   required but efficiency should be okay. */

typedef struct error_messages_t {
    hdf_err_code_t error_code;
    const char *str;
} error_messages_t;

PRIVATE const struct error_messages_t error_messages[] =
{
{ DFE_NONE,         "No error"},
{ DFE_FNF,          "File not found"},
{ DFE_DENIED,       "Access to file denied"},
{ DFE_ALROPEN,      "File already open"},
{ DFE_TOOMANY,      "Too Many AID's or files open"},
{ DFE_BADNAME,      "Bad file name on open"},
{ DFE_BADACC,       "Bad file access mode"},
{ DFE_BADOPEN,      "Error opening file"},
{ DFE_NOTOPEN,      "File can't be closed; It isn't open"},
{ DFE_CANTCLOSE,    "Unable to close file"},
{ DFE_DFNULL,       "DF has a null pointer"},
{ DFE_ILLTYPE,      "Internal error: DF has an illegal type"},
{ DFE_UNSUPPORTED,  "Feature not currently supported"},
{ DFE_BADDDLIST,    "Internal error: The DD list is non-existent"},
{ DFE_NOTDFFILE,    "This is not an HDF file"},
{ DFE_SEEDTWICE,    "Internal error: The DD list is already seeded"},
{ DFE_NOSPACE,      "Internal error: Out of space"},
{ DFE_READERROR,    "Read error"},
{ DFE_WRITEERROR,   "Write error"},
{ DFE_SEEKERROR,    "Error performing seek operation"},
{ DFE_NOFREEDD,     "There are no free DD's left"},
{ DFE_BADTAG,       "Illegal WILDCARD tag"},
{ DFE_BADREF,       "Illegal WILDCARD reference"},
{ DFE_RDONLY,       "Attempt to write to read-only HDF file"},
{ DFE_BADCALL,      "Calls in wrong order"},
{ DFE_BADPTR,       "NULL ptr argument"},
{ DFE_BADLEN,       "Invalid length specified"},
{ DFE_BADSEEK,      "Attempt to seek past end of element"},
{ DFE_NOMATCH,      "No (more) DDs which match specified tag/ref"},
{ DFE_NOTINSET,     "Set contained unknown tag: ignored"},
{ DFE_BADDIM,       "Negative or zero dimensions specified"},
{ DFE_BADOFFSET,    "Illegal offset specified"},
{ DFE_BADSCHEME,    "Unknown compression scheme specified"},
{ DFE_NODIM,        "No dimension record associated with image"},
{ DFE_NOTENOUGH,    "Space provided insufficient for size of data"},
{ DFE_NOVALS,       "Values not available"},
{ DFE_CORRUPT,      "File is corrupted"},
{ DFE_BADFP,        "File contained an illegal floating point number"},
{ DFE_NOREF,        "No more reference numbers are available"},
{ DFE_BADDATATYPE,  "Unknown or unavailable data type specified"},
{ DFE_BADMCTYPE,    "Unknown or unavailable machine type specified"},
{ DFE_BADNUMTYPE,   "Unknown or unavailable number type specified"},
{ DFE_BADORDER,     "Unknown or illegal array order specified"},
{ DFE_ARGS,         "Invalid arguments to routine"},
{ DFE_INTERNAL,     "HDF Internal error"},
{ DFE_DUPDD,        "Tag/ref is already used"},
{ DFE_CANTMOD,      "Old element does not exist, cannot modify"},
{ DFE_RANGE,        "Improper range for attempted access"},
{ DFE_BADTABLE,     "The nsdg table is wrong"},
{ DFE_BADSDG,       "Error processing an sdg"},
{ DFE_BADNDG,       "Error processing an ndg"},
{ DFE_BADFIELDS,    "Unable to parse fields string correctly"},
{ DFE_NORESET,      "Can not reset this value"},
{ DFE_NOVS,         "Could not find specified VS or VG in file"},
{ DFE_VGSIZE,       "No more elements will fit in this VGroup"},
{ DFE_DIFFFILES,    "Attempt to merge objects in different files"},
{ DFE_VTAB,         "Element is not in VSet tables"},
{ DFE_BADAID,       "Unable to create a new AID"},
{ DFE_OPENAID,      "There are still active AIDs"},
{ DFE_BADCONV,      "Don't know how to convert data type"},
{ DFE_GENAPP,       "Generic application-level error"},
{ DFE_CANTFLUSH,    "Cannot flush the changed DD back to the file"},
{ DFE_BADTYPE,      "Incompatible type specified"},
{ DFE_SYMSIZE,      "Too many symbols in table"},
{ DFE_BADATTACH,    "Cannot write to a previously attached VData"},
{ DFE_CANTATTACH,   "Cannot attach to a VData"},
{ DFE_CANTDETACH,   "Cannot detach a VData with access 'w'"},
{ DFE_CANTUPDATE,   "Cannot update the DD block"},
{ DFE_CANTHASH,     "Cannot add a DD to the hash table"},
{ DFE_CANTDELDD,    "Cannot delete a DD in the file"},
{ DFE_CANTDELHASH,  "Cannot delete a DD from the hash table"},
{ DFE_BADMODEL,     "Invalid compression model specified"},
{ DFE_BADCODER,     "Invalid compression coder specified"},
{ DFE_MODEL,        "Error in modeling layer of compression"},
{ DFE_CODER,        "Error in encoding layer of compression"},
{ DFE_CINIT,        "Error in encoding initialization"},
{ DFE_CDECODE,      "Error in decoding compressed data"},
{ DFE_CENCODE,      "Error in encoding compressed data"},
{ DFE_CTERM,        "Error in encoding termination"},
{ DFE_CSEEK,        "Error seeking in encoded dataset"},
{ DFE_MINIT,        "Error in modeling initialization"},
{ DFE_COMPINFO,     "Invalid compression header"},
{ DFE_BADRIG,       "Error processing a RIG"},
{ DFE_PUTELEM,      "Hputelement failed in some way"},
{ DFE_GETELEM,      "Hgetelement failed in some way"},
{ DFE_BADGROUP,     "Error from DFdiread in opening a group"},
{ DFE_GROUPSETUP,   "Error from DFdisetup in opening a group"},
{ DFE_PUTGROUP,     "Error when putting a tag/ref into a group"},
{ DFE_CANTCOMP,     "Can't compress an object"},
{ DFE_CANTDECOMP,   "Can't de-compress an object"},
{ DFE_UNINIT,       "Interface was not initialized correctly"},
{ DFE_CANTINIT,     "Can't initialize an interface we depend on"},
{ DFE_CANTSHUTDOWN, "Can't shut down an interface we depend on"},
{ DFE_BITREAD,      "There was a bit-read error"},
{ DFE_BITWRITE,     "There was a bit-write error"},
{ DFE_BITSEEK,      "There was a bit-seek error"},
{ DFE_CANTLINK,     "Can't initialize link information"},
{ DFE_CANTSYNC,     "Cannot syncronize memory with file"}
};
#endif /* _H_ERR_MASTER_ */

#endif /* __HERR_H */
