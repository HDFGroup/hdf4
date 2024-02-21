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

/*  herr.h
 *
 *  Error routines
 */

#ifndef H4_HERR_H
#define H4_HERR_H

#include "hdf.h"

/*
 * ======================================================================
 * Error codes
 *
 * NOTE: Remember to update the error_messages[] structure in herr.c
 * whenever errors are added/deleted from this list.
 * ======================================================================
 */

/* Declare an enumerated type which holds all the valid HDF error codes */
typedef enum {
    DFE_NONE = 0,   /* special zero, no error */
                    /* Low-level I/O errors */
    DFE_FNF,        /* File not found */
    DFE_DENIED,     /* Access to file denied */
    DFE_ALROPEN,    /* File already open */
    DFE_TOOMANY,    /* Too Many AID's or files open */
    DFE_BADNAME,    /* Bad file name on open */
    DFE_BADACC,     /* Bad file access mode */
    DFE_BADOPEN,    /* Other open error */
    DFE_NOTOPEN,    /* File can't be closed 'cause it isn't open */
    DFE_CANTCLOSE,  /* fclose wouldn't work! */
    DFE_READERROR,  /* There was a read error */
    DFE_WRITEERROR, /* There was a write error */
    DFE_SEEKERROR,  /* There was a seek error */
    DFE_RDONLY,     /* The DF is read only */
    DFE_BADSEEK,    /* Attempt to seek past end of element */
    DFE_INVFILE,    /* File is neither hdf, cdf, netcdf */

    /* Low-level HDF I/O errors */
    DFE_PUTELEM,  /* Hputelement failed in some way */
    DFE_GETELEM,  /* Hgetelement failed in some way */
    DFE_CANTLINK, /* Can't initialize link information */
    DFE_CANTSYNC, /* Cannot synchronize memory with file */

    /* Old group interface errors */
    DFE_BADGROUP,   /* Error from DFdiread in opening a group */
    DFE_GROUPSETUP, /* Error from DFdisetup in opening a group */
    DFE_PUTGROUP,   /* Error when putting a tag/ref into a group */
    DFE_GROUPWRITE, /* Error when writing out a group */

    /* Internal HDF errors */
    DFE_DFNULL,        /* DF is a null pointer */
    DFE_ILLTYPE,       /* DF has an illegal type: internal error */
    DFE_BADDDLIST,     /* The DD list is non-existent: internal error */
    DFE_NOTDFFILE,     /* This is not a DF file and it is not 0 length */
    DFE_SEEDTWICE,     /* The DD list already seeded: internal error */
    DFE_NOSUCHTAG,     /* No such tag in the file: search failed */
    DFE_NOFREEDD,      /* There are no free DD's left: internal error */
    DFE_BADTAG,        /* illegal WILDCARD tag */
    DFE_BADREF,        /* illegal WILDCARD reference # */
    DFE_NOMATCH,       /* No (more) DDs which match specified tag/ref */
    DFE_NOTINSET,      /* Warning: Set contained unknown tag: ignored */
    DFE_BADOFFSET,     /* Illegal offset specified */
    DFE_CORRUPT,       /* File is corrupted */
    DFE_NOREF,         /* no more reference numbers are available */
    DFE_DUPDD,         /* the new tag/ref is already used */
    DFE_CANTMOD,       /* old element not exist, cannot modify */
    DFE_DIFFFILES,     /* Attempt to merge objs in diff files */
    DFE_BADAID,        /* Got a bogus aid */
    DFE_OPENAID,       /* There are still active AIDs */
    DFE_CANTFLUSH,     /* Can't flush DD back to file */
    DFE_CANTUPDATE,    /* Cannot update the DD block */
    DFE_CANTHASH,      /* Cannot add a DD to the hash table */
    DFE_CANTDELDD,     /* Cannot delete a DD in the file */
    DFE_CANTDELHASH,   /* Cannot delete a DD from the hash table */
    DFE_CANTACCESS,    /* Cannot access specified tag/ref */
    DFE_CANTENDACCESS, /* Cannot end access to data element */
    DFE_TABLEFULL,     /* Access table is full */
    DFE_NOTINTABLE,    /* Cannot find element in table */

    /* Generic errors */
    DFE_UNSUPPORTED, /* Feature not currently supported */
    DFE_NOSPACE,     /* Malloc failed */
    DFE_BADCALL,     /* Calls in wrong order */
    DFE_BADPTR,      /* NULL ptr argument */
    DFE_BADLEN,      /* Invalid len specified */
    DFE_NOTENOUGH,   /* space provided insufficient for size of data */
    DFE_NOVALS,      /* Values not available */
    DFE_ARGS,        /* bad arguments to routine */
    DFE_INTERNAL,    /* serious internal error */
    DFE_NORESET,     /* Too late to modify this value */
    DFE_EXCEEDMAX,   /* Value exceeds max allowed */
    DFE_GENAPP,      /* Generic application,level error */

    /* Generic interface errors */
    DFE_UNINIT,       /* Interface was not initialized correctly */
    DFE_CANTINIT,     /* Can't initialize an interface we depend on */
    DFE_CANTSHUTDOWN, /* Can't shut down an interface we depend on */

    /* General Dataset errors */
    DFE_BADDIM,      /* negative or zero dimensions specified */
    DFE_BADFP,       /* File contained an illegal floating point num */
    DFE_BADDATATYPE, /* unknown or unavailable data type specified */
    DFE_BADMCTYPE,   /* unknown or unavailable machine type specified */
    DFE_BADNUMTYPE,  /* unknown or unavailable number type specified */
    DFE_BADORDER,    /* unknown or illegal array order specified */
    DFE_RANGE,       /* improper range for attempted access */
    DFE_BADCONV,     /* Don't know how to convert data type */
    DFE_BADTYPE,     /* Incompatible types specified */
    DFE_BADDIMNAME,  /* Dimension name not valid or already taken */
    DFE_NOVGREP,     /* No Vgroup representation for SDS and dim */

    /* Compression errors */
    DFE_BADSCHEME,   /* Unknown compression scheme specified */
    DFE_BADMODEL,    /* Invalid compression model specified */
    DFE_BADCODER,    /* Invalid compression encoder specified */
    DFE_MODEL,       /* Error in modeling layer of compression */
    DFE_CODER,       /* Error in encoding layer of compression */
    DFE_CINIT,       /* Error in encoding initialization */
    DFE_CDECODE,     /* Error in decoding compressed data */
    DFE_CENCODE,     /* Error in encoding compressed data */
    DFE_CTERM,       /* Error in encoding termination */
    DFE_CSEEK,       /* Error seekging in encoded dataset */
    DFE_MINIT,       /* Error in modeling initialization */
    DFE_COMPINFO,    /* Invalid compression header */
    DFE_CANTCOMP,    /* Can't compress an object */
    DFE_CANTDECOMP,  /* Can't de-compress an object */
    DFE_NOENCODER,   /* Encoder not available */
    DFE_NOSZLIB,     /* SZIP library not available */
    DFE_COMPVERSION, /* Z_VERSION_ERROR (-6) returned from zlib */
    DFE_READCOMP,    /* Error in reading compressed data; this
                        error occurs when one of the following
                        error codes is returned from zlib:
                             Z_ERRNO         (-1)
                             Z_STREAM_ERROR  (-2)
                             Z_DATA_ERROR    (-3)
                             Z_MEM_ERROR     (-4)
                             Z_BUF_ERROR     (-5) */

    /* Raster errors */
    DFE_NODIM,       /* No dimension record associated with image */
    DFE_BADRIG,      /* Error processing a RIG */
    DFE_RINOTFOUND,  /* Can't find raster image */
    DFE_BADATTR,     /* Bad Attribute */
    DFE_LUTNOTFOUND, /* No palette information for RIG */
    DFE_GRNOTFOUND,  /* Can't find specified GR */

    /* SDG/NDG errors */
    DFE_BADTABLE, /* the nsdg table is wrong */
    DFE_BADSDG,   /* error processing an sdg */
    DFE_BADNDG,   /* error processing an ndg */

    /* Vset errors */
    DFE_VGSIZE,      /* Too many elements in VGroup */
    DFE_VTAB,        /* Elmt not in vtab[] */
    DFE_CANTADDELEM, /* Cannot add tag/ref to VGroup */
    DFE_BADVGNAME,   /* Cannot set VGroup name */
    DFE_BADVGCLASS,  /* Cannot set VGroup class */

    /* Vdata errors */
    DFE_BADFIELDS,  /* Bad fields string passed to Vset routine */
    DFE_NOVS,       /* Counldn't find VS in file */
    DFE_SYMSIZE,    /* Too many symbols in users table */
    DFE_BADATTACH,  /* Cannot write to a previously attached VData */
    DFE_BADVSNAME,  /* Cannot set VData name */
    DFE_BADVSCLASS, /* Cannot set VData class */
    DFE_VSWRITE,    /* Error writing to VData */
    DFE_VSREAD,     /* Error reading from VData */
    DFE_BADVH,      /* Error in VData Header */
    DFE_FIELDSSET,  /* Fields already set for vdata */

    /* High-level Vdata/Vset errors */
    DFE_VSCANTCREATE, /* Cannot create VData */
    DFE_VGCANTCREATE, /* Cannot create VGroup */

    /* Generic Vdata/Vset errors */
    DFE_CANTATTACH, /* Cannot attach to a VData/Vset */
    DFE_CANTDETACH, /* Cannot detach a VData/Vset with access 'w' */

    /* XDR level errors */
    DFE_XDRERROR, /* Error occur in XDR and/or CDF level */

    /* bit I/O errors */
    DFE_BITREAD,  /* There was a bit-read error */
    DFE_BITWRITE, /* There was a bit-write error */
    DFE_BITSEEK,  /* There was a bit-seek error */

    /* tbbt interface errors */
    DFE_TBBTINS, /* Failed to insert element into tree */

    /* bit-vector interface errors */
    DFE_BVNEW,  /* Failed to create a bit-vector */
    DFE_BVSET,  /* Failed when setting a bit in a bit-vector */
    DFE_BVGET,  /* Failed when getting a bit in a bit-vector */
    DFE_BVFIND, /* Failed when finding a bit in a bit-vector */

    /* General to all interfaces */
    DFE_CANTSETATTR, /* Failed to add an attribute */
    DFE_CANTGETATTR, /* Failed to find or get an attribute */

    /* Annotation interface errors */
    DFE_ANAPIERROR /* Failed in annotation interface */
} hdf_err_code_t;

#endif /* H4_HERR_H */
