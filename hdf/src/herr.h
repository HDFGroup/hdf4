/*
$Header$

$Log$
Revision 1.11  1993/10/06 20:27:40  koziol
More compression fixed, and folded Doug's suggested change into VSappendable.

 * Revision 1.10  1993/10/04  20:02:49  koziol
 * Updated error reporting in H-Layer routines, and added more error codes and
 * compression stuff.
 *
 * Revision 1.9  1993/10/01  20:01:05  koziol
 * Put "extern C" block around function prototypes for C++ compatibility.
 *
 * Revision 1.8  1993/09/30  19:05:10  koziol
 * Added basic compressing functionality for special tags.
 *
 * Revision 1.7  1993/09/28  18:04:29  koziol
 * Removed OLD_WAY & QAK ifdef's.  Removed oldspecial ifdef's for special
 * tag handling.  Added new compression special tag type.
 *
 * Revision 1.6  1993/07/13  20:45:02  chouck
 * Fixed a few memory leaks
 *
 * Revision 1.5  1993/04/06  17:23:37  chouck
 * Added Vset macros
 *
 * Revision 1.4  1993/03/29  16:47:57  koziol
 * Updated JPEG code to new JPEG 4 code.
 * Changed VSets to use Threaded-Balanced-Binary Tree for internal
 * 	(in memory) representation.
 * Changed VGROUP * and VDATA * returns/parameters for all VSet functions
 * 	to use 32-bit integer keys instead of pointers.
 * Backed out speedups for Cray, until I get the time to fix them.
 * Fixed a bunch of bugs in the little-endian support in DFSD.
 *
 * Revision 1.3  1993/02/18  04:23:21  georgev
 * Added new HCLOSE_RETURN_ERROR macro.
 *
 * Revision 1.2  1993/01/19  05:55:44  koziol
 * Merged Hyperslab and JPEG routines with beginning of DEC ALPHA
 * port.  Lots of minor annoyances fixed.
 *
 * Revision 1.1  1992/08/25  21:40:44  koziol
 * Initial revision
 *
*/
/*+ herr.h
***  header file for using error routines
*** to be included by all ".c" files
+*/

#ifndef __HERR_H

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

#define HERROR(e) HEpush((int16)(e), FUNC, __FILE__, __LINE__)

/* HRETURN_ERROR macro, used to facilitate error reporting.  Makes
   same assumptions as HERROR.  IN ADDITION, this macro causes
   a return from the calling routine */

#define HRETURN_ERROR(err, ret_val) {HERROR(err); return(ret_val);} 

/* HCLOSE_RETURN_ERROR macro, used to facilitate error reporting.  Makes
   same assumptions as HRETURN_ERROR.  IN ADDITION, this macro causes
   the file specified by the id "fid" to be closed */

#define HCLOSE_RETURN_ERROR(hfid, err, ret_val) {HERROR(err); Hclose(hfid); return(ret_val);} 

#if 0
/* Clear the error stack */
extern int32 error_top;
#define HEclear() { error_top = (int32)0; }
#endif

/*
======================================================================
   Error codes

   NOTE: Remember to update the error_messages[] structure in herr.c
   whenever errors are added/deleted from this list.
======================================================================
*/
#define DFE_NONE        0   /* special zero, no error */
#define DFE_FNF         -1  /* File not found */
#define DFE_DENIED      -2  /* Access to file denied */
#define DFE_ALROPEN     -3  /* File already open */
#define DFE_TOOMANY     -4  /* Too Many AID's or files open */
#define DFE_BADNAME     -5  /* Bad file name on open */
#define DFE_BADACC      -6  /* Bad file access mode */
#define DFE_BADOPEN     -7  /* Other open error */
#define DFE_NOTOPEN     -8  /* File can't be closed 'cause it isn't open */
#define DFE_CANTCLOSE   -9  /* fclose wouldn't work! */
#define DFE_DFNULL      -10 /* DF is a null pointer */
#define DFE_ILLTYPE     -11 /* DF has an illegal type: internal error */
#define DFE_UNSUPPORTED -12 /* Feature not currently supported */
#define DFE_BADDDLIST   -13 /* The DD list is non-existent: internal error */
#define DFE_NOTDFFILE   -14 /* This is not a DF file and it is not 0 length */
#define DFE_SEEDTWICE   -15 /* The DD list already seeded: internal error */
#define DFE_NOSPACE     -16 /* Malloc failed */
#define DFE_NOSUCHTAG   -17 /* No such tag in the file: search failed */
#define DFE_READERROR   -18 /* There was a read error */
#define DFE_WRITEERROR  -19 /* There was a write error */
#define DFE_SEEKERROR   -20 /* There was a seek error */
#define DFE_NOFREEDD    -21 /* There are no free DD's left: internal error */
#define DFE_BADTAG      -22 /* illegal WILDCARD tag */
#define DFE_BADREF      -23 /* illegal WILDCARD reference # */
#define DFE_RDONLY      -24 /* The DF is read only */
#define DFE_BADCALL     -25 /* Calls in wrong order */
#define DFE_BADPTR      -26 /* NULL ptr argument */
#define DFE_BADLEN      -27 /* Invalid len specified */
#define DFE_BADSEEK     -28 /* Attempt to seek past end of element */
#define DFE_NOMATCH     -29 /* No (more) DDs which match specified tag/ref */
#define DFE_NOTINSET    -30 /* Warning: Set contained unknown tag: ignored */
#define DFE_BADDIM      -31 /* negative or zero dimensions specified */
#define DFE_BADOFFSET   -32 /* Illegal offset specified */
#define DFE_BADSCHEME   -33 /* Unknown compression scheme specified */
#define DFE_NODIM       -34 /* No dimension record associated with image */
#define DFE_NOTENOUGH   -35 /* space provided insufficient for size of data */
#define DFE_NOVALS      -36 /* Values not available */
#define DFE_CORRUPT     -37 /* File is corrupted */
#define DFE_BADFP       -38 /* File contained an illegal floating point num */
#define DFE_NOREF       -39 /* no more reference numbers are available */
#define DFE_BADDATATYPE -40 /* unknown or unavailable data type specified */
#define DFE_BADMCTYPE   -41 /* unknown or unavailable machine type specified */
#define DFE_BADNUMTYPE  -42 /* unknown or unavailable number type specified */
#define DFE_BADORDER    -43 /* unknown or illegal array order specified */
#define DFE_ARGS        -44 /* bad arguments to routine */
#define DFE_INTERNAL    -45 /* serious internal error */
#define DFE_DUPDD       -46 /* the new tag/ref is already used */
#define DFE_CANTMOD     -47 /* old element not exist, cannot modify */
#define DFE_RANGE       -48 /* improper range for attempted acess */
#define DFE_BADTABLE    -49 /* the nsdg table is wrong   */
#define DFE_BADSDG      -50 /* error processing an sdg    */
#define DFE_BADNDG      -51 /* error processing an ndg     */
#define DFE_BADFIELDS   -52 /* Bad fields string passed to Vset routine */
#define DFE_NORESET     -53 /* Too late to modify this value */
#define DFE_NOVS        -54 /* Counldn't find VS in file */
#define DFE_VGSIZE      -55 /* Too many elements in VGroup */
#define DFE_DIFFFILES   -56 /* Attempt to merge objs in diff files */
#define DFE_VTAB        -57 /* Elmt not in vtab[] */
#define DFE_BADAID      -58 /* Got a bogus aid */
#define DFE_OPENAID     -59 /* There are still active AIDs */
#define DFE_BADCONV     -60 /* Don't know how to convert data type */
#define DFE_GENAPP      -61 /* Generic application-level error */
#define DFE_CANTFLUSH   -62 /* Can't flush DD back to file */
#define DFE_BADTYPE     -63 /* Incompatible types specified */
#define DFE_SYMSIZE     -64 /* Too many symbols in users table */
#define DFE_BADATTACH   -65 /* Cannot write to a previously attached VData */
#define DFE_CANTDETACH  -66 /* Cannot detach a VData with access 'w' */
#define DFE_CANTUPDATE  -67 /* Cannot update the DD block */
#define DFE_CANTHASH    -68 /* Cannot add a DD to the hash table */
#define DFE_CANTDELDD   -69 /* Cannot delete a DD in the file */
#define DFE_CANTDELHASH -70 /* Cannot delete a DD from the hash table */
#define DFE_BADMODEL    -71 /* Invalid compression model specified */
#define DFE_BADCODER    -72 /* Invalid compression encoder specified */
#define DFE_MODEL       -73 /* Error in modeling layer of compression */
#define DFE_CODER       -74 /* Error in encoding layer of compression */
#define DFE_CINIT       -75 /* Error in encoding initialization */
#define DFE_CDECODE     -76 /* Error in decoding compressed data */

#ifdef _H_ERR_MASTER_

/* error_messages is the list of error messages in the system, kept as
   error_code-message pairs.  To look up a message, a linear search is
   required but efficiency should be okay. */

typedef struct error_messages_t {
    int16 error_code;
    char *str;
} error_messages_t;

PRIVATE const error_messages_t error_messages[] =
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
{ DFE_CDECODE,      "Error in decoding compressed data"}
};
#endif /* _H_ERR_MASTER_ */

#endif /* __HERR_H */
