/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Copyright 1993, University Corporation for Atmospheric Research           *
 * See netcdf/COPYRIGHT file for copying and redistribution conditions.      *
 *                                                                           *
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

#ifndef MFH4_NC_PRIV_H
#define MFH4_NC_PRIV_H

#include <inttypes.h>

#include "hdf_priv.h"
#include "H4api_adpt.h"

/* NetCDF functionality used in HDF4
 *
 * The multi-file interface was built on NetCDF 2.3.2 and HDF4 originally
 * exported a NetCDF interface. The public NetCDF interface has been removed
 * and all NetCDF API calls, types, etc. are now assembled in this private
 * header file.
 */

/* Prepend 'H4_' to all NetCDF function names to avoid name clash when the
 * HDF and NetCDF headers are included in the same application.
 *
 * These could be manually renamed and the HNAME macro eliminated, but that
 * would just make the code harder to read.
 */
#define HNAME(x) H4_##x

/* Variables */
#define ncerr  HNAME(ncerr)
#define ncopts HNAME(ncopts)

/* Former public API calls */
#define nccreate HNAME(nccreate) /* used in src - mfsd.c */
#define ncopen   HNAME(ncopen)   /* used in src - mfsd.c */
#define ncendef  HNAME(ncendef)
#define ncclose  HNAME(ncclose) /* used in src - mfsd.c */
#define ncabort  HNAME(ncabort) /* used in src - file.c */
#define ncdimdef HNAME(ncdimdef)
#define ncdiminq HNAME(ncdiminq)

#define ncvardef  HNAME(ncvardef)
#define ncvarid   HNAME(ncvarid)
#define ncvarinq  HNAME(ncvarinq)
#define ncvarput1 HNAME(ncvarput1)
#define ncvarget1 HNAME(ncvarget1)
#define ncvarput  HNAME(ncvarput)
#define ncvarget  HNAME(ncvarget) /* used in src - array.c */
#define ncvarputs HNAME(ncvarputs)
#define ncvargets HNAME(ncvargets)
#define ncvarputg HNAME(ncvarputg)
#define ncvargetg HNAME(ncvargetg)

#define ncattput  HNAME(ncattput)
#define nctypelen HNAME(nctypelen) /* used in src - array.c, nssdc.c, putget.c */
#define ncsetfill HNAME(ncsetfill) /* used in src - mfsd.c */

/* Internal API calls */
#define nc_serror         HNAME(nc_serror)
#define NCadvise          HNAME(NCadvise)
#define NC_computeshapes  HNAME(NC_computeshapes)
#define NC_xtypelen       HNAME(NC_xtypelen)
#define NC_xlen_array     HNAME(NC_xlen_array)
#define NC_xlen_attr      HNAME(NC_xlen_attr)
#define NC_xlen_cdf       HNAME(NC_xlen_cdf)
#define NC_xlen_dim       HNAME(NC_xlen_dim)
#define NC_xlen_iarray    HNAME(NC_xlen_iarray)
#define NC_xlen_string    HNAME(NC_xlen_string)
#define NC_xlen_var       HNAME(NC_xlen_var)
#define NC_arrayfill      HNAME(NC_arrayfill)
#define NC_copy_arrayvals HNAME(NC_copy_arrayvals)
#define NC_free_array     HNAME(NC_free_array)
#define NC_free_attr      HNAME(NC_free_attr)
#define NC_free_cdf       HNAME(NC_free_cdf)
#define NC_free_dim       HNAME(NC_free_dim)
#define NC_free_iarray    HNAME(NC_free_iarray)
#define NC_free_string    HNAME(NC_free_string)
#define NC_free_var       HNAME(NC_free_var)
#define NC_incr_array     HNAME(NC_incr_array)
#define NC_dimid          HNAME(NC_dimid)
#define NCcktype          HNAME(NCcktype)
#define NC_indefine       HNAME(NC_indefine)
#define xdr_cdf           HNAME(xdr_cdf)
#define xdr_numrecs       HNAME(xdr_numrecs)
#define xdr_shorts        HNAME(xdr_shorts)
#define xdr_NC_array      HNAME(xdr_NC_array)
#define xdr_NC_attr       HNAME(xdr_NC_attr)
#define xdr_NC_dim        HNAME(xdr_NC_dim)
#define xdr_NC_fill       HNAME(xdr_NC_fill)
#define xdr_NC_iarray     HNAME(xdr_NC_iarray)
#define xdr_NC_string     HNAME(xdr_NC_string)
#define xdr_NC_var        HNAME(xdr_NC_var)
#define NC_typelen        HNAME(NC_typelen)
#define NC_check_id       HNAME(NC_check_id)
#define NC_new_cdf        HNAME(NC_new_cdf)
#define NC_new_array      HNAME(NC_new_array)
#define NC_re_array       HNAME(NC_re_array)
#define NC_new_attr       HNAME(NC_new_attr)
#define NC_findattr       HNAME(NC_findattr)
#define NC_new_dim        HNAME(NC_new_dim)
#define NC_new_iarray     HNAME(NC_new_iarray)
#define NC_new_string     HNAME(NC_new_string)
#define NC_re_string      HNAME(NC_re_string)
#define NC_hlookupvar     HNAME(NC_hlookupvar)
#define NC_new_var        HNAME(NC_new_var)
#define NCvario           HNAME(NCvario)
#define NCcoordck         HNAME(NCcoordck)
#define xdr_NCvshort      HNAME(xdr_NCvshort)
#define NC_dcpy           HNAME(NC_dcpy)
#define NCxdrfile_create  HNAME(NCxdrfile_create)
#define NCgenio           HNAME(NCgenio)      /* from putgetg.c */
#define NC_var_shape      HNAME(NC_var_shape) /* from var.c */

#ifndef FILENAME_MAX
#define FILENAME_MAX 255
#endif

/* HDF4's stripped-down XDR implementation */
#include "hdf_xdr_priv.h"

/* Constants for sizes of NC types */
#define NC_BYTE_SIZE        1
#define NC_CHAR_SIZE        1
#define NC_SHORT_SIZE       2
#define NC_LONG_SIZE        4
#define NC_FLOAT_SIZE       4
#define NC_DOUBLE_SIZE      8
#define NC_UNSPECIFIED_SIZE 0

/*
 * Masks for the struct NC flags field; passed in as 'mode' arg to
 * nccreate and ncopen.
 */
#define NC_RDWR   1      /* read/write, 0 => readonly */
#define NC_CREAT  2      /* in create phase, cleared by ncendef */
#define NC_EXCL   4      /* on create, don't destroy existing file */
#define NC_INDEF  8      /* in define mode, cleared by ncendef */
#define NC_NSYNC  0x10   /* synchronise numrecs on change */
#define NC_HSYNC  0x20   /* synchronise whole header on change */
#define NC_NDIRTY 0x40   /* numrecs has changed */
#define NC_HDIRTY 0x80   /* header info has changed */
#define NC_NOFILL 0x100  /* Don't fill vars on endef and increase of record */
#define NC_LINK   0x8000 /* isa link */

#define NC_FILL 0 /* argument to ncsetfill to clear NC_NOFILL */

/*
 * 'mode' arguments for nccreate and ncopen
 */
#define NC_NOWRITE   0
#define NC_WRITE     NC_RDWR
#define NC_CLOBBER   (NC_INDEF | NC_CREAT | NC_RDWR)
#define NC_NOCLOBBER (NC_INDEF | NC_EXCL | NC_CREAT | NC_RDWR)

/*
 * 'size' argument to ncdimdef for an unlimited dimension
 */
#define NC_UNLIMITED 0L

/*
 * Attribute id to put/get a global attribute
 */
#define NC_GLOBAL -1

#include "hlimits.h"

/* This type used to be defined as an enum, but the C standard is a bit
 * vague as to which integer type you actually get to represent your enum
 * and passing that through a pointer caused failures on MacOS.
 */
typedef int nc_type;
#define NC_UNSPECIFIED 0 /* private */
#define NC_BYTE        1
#define NC_CHAR        2
#define NC_SHORT       3
#define NC_LONG        4 /* Maps to a 32-bit signed integer (int32_t) */
#define NC_FLOAT       5
#define NC_DOUBLE      6
/* private */
#define NC_BITFIELD  7
#define NC_STRING    8
#define NC_IARRAY    9
#define NC_DIMENSION 10
#define NC_VARIABLE  11
#define NC_ATTRIBUTE 12

/*
 * Global NetCDF error status variable
 * Initialized in error.c
 */
#define NC_NOERR        0           /* No Error */
#define NC_EBADID       1           /* Not a netcdf id */
#define NC_ENFILE       2           /* Too many netcdfs open */
#define NC_EEXIST       3           /* netcdf file exists && NC_NOCLOBBER */
#define NC_EINVAL       4           /* Invalid Argument */
#define NC_EPERM        5           /* Write to read only */
#define NC_ENOTINDEFINE 6           /* Operation not allowed in data mode */
#define NC_EINDEFINE    7           /* Operation not allowed in define mode */
#define NC_EINVALCOORDS 8           /* Coordinates out of Domain */
#define NC_EMAXDIMS     9           /* MAX_NC_DIMS exceeded */
#define NC_ENAMEINUSE   10          /* String match to name in use */
#define NC_ENOTATT      11          /* Attribute not found */
#define NC_EMAXATTS     12          /* MAX_NC_ATTRS exceeded */
#define NC_EBADTYPE     13          /* Not a netcdf data type */
#define NC_EBADDIM      14          /* Invalid dimension id */
#define NC_EUNLIMPOS    15          /* NC_UNLIMITED in the wrong index */
#define NC_EMAXVARS     16          /* MAX_NC_VARS exceeded */
#define NC_ENOTVAR      17          /* Variable not found */
#define NC_EGLOBAL      18          /* Action prohibited on NC_GLOBAL varid */
#define NC_ENOTNC       19          /* Not a netcdf file */
#define NC_ESTS         20          /* In Fortran, string too short */
#define NC_EMAXNAME     21          /* MAX_NC_NAME exceeded */
#define NC_ENTOOL       NC_EMAXNAME /* Backward compatibility */
#define NC_EUNLIMIT     22          /* NC_UNLIMITED size already in use */

#define NC_EXDR   32 /* */
#define NC_SYSERR -1

HDFLIBAPI int ncerr;

/*
 * Global options variable. Used to determine behavior of error handler.
 * Initialized in error.c
 */
#define NC_FATAL   1
#define NC_VERBOSE 2

HDFLIBAPI int ncopts; /* default is (NC_FATAL | NC_VERBOSE) */

/*
 * HDF headers
 */

#include "vg.h"
#include "hfile_priv.h"
#include "mfhdf.h"

#define ATTR_TAG  DFTAG_VH
#define DIM_TAG   DFTAG_VG
#define VAR_TAG   DFTAG_VG
#define DATA_TAG  DFTAG_SD
#define BOGUS_TAG ((uint16)721)

#define DIMVAL_VERSION00 0     /* <dimsize> fake values */
#define DIMVAL_VERSION01 1     /* 1 elt with value of <dimsize>  */
#define BLOCK_MULT       64    /* multiplier for bytes in linked blocks */
#define MAX_BLOCK_SIZE   65536 /* maximum size of block in linked blocks */
#define BLOCK_COUNT      128   /* size of linked block pointer objects  */

/* from cdflib.h CDF 2.3 */
#ifndef MAX_VXR_ENTRIES
#define MAX_VXR_ENTRIES 10
#endif /* MAX_VXR_ENTRIES */

/* VIX record for CDF variable data storage */
typedef struct vix_t_def {
    int32             nEntries;                  /* number of entries in this vix */
    int32             nUsed;                     /* number of entries containing valid data */
    int32             firstRec[MAX_VXR_ENTRIES]; /* number of first records */
    int32             lastRec[MAX_VXR_ENTRIES];  /* number of last records */
    int32             offset[MAX_VXR_ENTRIES];   /* file offset of records */
    struct vix_t_def *next;                      /* next one in line */
} vix_t;

/* NetCDF array type */
typedef struct {
    nc_type  type;   /* the discriminant */
    size_t   len;    /* the total length originally allocated */
    size_t   szof;   /* sizeof each value */
    unsigned count;  /* length of the array */
    uint8_t *values; /* the actual data */
} NC_array;

/* Counted string for names and such
 *
 * count is the actual size of the buffer for the string
 * len is the length of the string in the buffer
 *
 * count != len when a string is resized to something smaller
 */

#define NC_compare_string(s1, s2) ((s1)->hash != (s2)->hash ? 1 : strcmp((s1)->values, (s2)->values))

typedef struct {
    unsigned count;
    unsigned len;
    uint32   hash; /* [non-perfect] hash value for faster comparisons */
    char    *values;
} NC_string;

/* Counted array of ints for assoc list */
typedef struct {
    unsigned count;
    int     *values;
} NC_iarray;

/* NC dimension structure */
typedef struct {
    NC_string *name;
    int32      size;
    int32      dim00_compat; /* compatible with Dim0.0 */
    int32      vgid;         /* id of the Vgroup representing this dimension */
    int32      count;        /* Number of pointers to this dimension */
} NC_dim;

/* NC attribute */
typedef struct {
    NC_string *name;
    NC_array  *data;
    int32      HDFtype; /* it should be in NC_array *data. However, */
                        /* NC.dims and NC.vars are NC_array too. */
} NC_attr;

typedef struct {
    char          path[FILENAME_MAX + 1];
    unsigned      flags;
    XDR          *xdrs;
    long          begin_rec; /* (off_t) position of the first 'record' */
    unsigned long recsize;   /* length of 'record' */
    int           redefid;
    /* below gets xdr'd */
    unsigned   numrecs; /* number of 'records' allocated */
    NC_array  *dims;
    NC_array  *attrs;
    NC_array  *vars;
    int32      hdf_file;
    int        file_type;
    int32      vgid;
    int        hdf_mode; /* mode we are attached for */
    hdf_file_t cdf_fp;   /* file pointer used for CDF files */
} NC;

/* NC variable: description and data */
typedef struct {
    NC_string     *name;    /* name->values shows data set's name */
    NC_iarray     *assoc;   /* user definition */
    unsigned long *shape;   /* compiled info (Each holds a dimension size. -BMR) */
    unsigned long *dsizes;  /* compiled info (Each element holds the amount of space
         needed to hold values in that dimension, e.g., first dimension
         size is 10, value type is int32=4, then dsizes[0]=4*10=40. -BMR) */
    NC_array     *attrs;    /* list of attribute structures */
    nc_type       type;     /* the discriminant */
    unsigned long len;      /* the total length originally allocated */
    size_t        szof;     /* sizeof each value */
    long          begin;    /* seek index, often an off_t */
    NC           *cdf;      /* handle of the file where this var belongs to  */
    int32         vgid;     /* id of the variable's Vgroup */
    uint16        data_ref; /* ref of the variable's data storage (if exists), default 0 */
    uint16        data_tag; /* tag of the variable's data storage (if exists), default DATA_TAG */
    uint16        ndg_ref;  /* ref of ndg for this dataset */
    hdf_vartype_t var_type; /* type of this variable, default UNKNOWN
            IS_SDSVAR == this var is an SDS variable
            IS_CRDVAR == this var is a coordinate variable
            UNKNOWN == because the var was created prior to this distinction.
    This is to distinguish b/w a one-dim data set and a coord var of the same name.
    It's less riskier than using a flag and change the file format, I think. -BMR */
    int   data_offset;      /* non-traditional data may not begin at 0 */
    int32 block_size;       /* size of the blocks for unlimited dim. datasets, default -1 */
    int   numrecs;          /* number of records this has been filled up to, for unlimited dim */
    int32 aid;              /* aid for DFTAG_SD data */
    int32 HDFtype;          /* type of this variable as HDF thinks */
    int32 HDFsize;          /* size of this variable as HDF thinks */
    /* These next two flags control when space in the file is allocated
        for a new dataset.  They are used (currently) in SDwritedata() and
        hdf_get_vp_aid() to allocate the full length of a new fixed-size dataset
        which is not writing fill values, instead of letting them get created
        as an "appendable" dataset and probably get converted into a linked-
        block special element when they don't need to be one */
    int32  created;    /* BOOLEAN == is newly created */
    int32  set_length; /* BOOLEAN == needs length set */
    int32  is_ragged;  /* BOOLEAN == is a ragged array */
    int32 *rag_list;   /* size of ragged array lines */
    int32  rag_fill;   /* last line in rag_list to be set */
    vix_t *vixHead;    /* list of VXR records for CDF data storage */
} NC_var;

#define IS_RECVAR(vp) ((vp)->shape != NULL ? (*(vp)->shape == NC_UNLIMITED) : 0)

#define netCDF_FILE 0
#define HDF_FILE    1
#define CDF_FILE    2

HDFLIBAPI const char *cdf_routine_name; /* defined in lerror.c */

#define MAGICOFFSET 0 /* Offset where format version number is written */

/* Format version number for CDF file */
/* Written twice at the beginning of pre-2.6 CDF file */
#define CDFMAGIC 0x0000FFFF

/* Format version number for HDF file */
#define HDFXMAGIC 0x0e031301 /* ^N^C^S^A */

/* Format version number for netCDF classic file */
#define NCMAGIC 0x43444601 /*  C D F 1 */

/* Format version number for 64-bit offset file */
#define NCMAGIC64 0x43444602 /*  C D F 2 */

/* Format version number for link file */
#define NCLINKMAGIC 0x43444c01 /*  C D L 1 */

#ifdef __cplusplus
extern "C" {
#endif

/* Former public API calls */
HDFLIBAPI int nccreate(const char *path, int cmode);
HDFLIBAPI int ncopen(const char *path, int mode);
HDFLIBAPI int ncendef(int cdfid);
HDFLIBAPI int ncclose(int cdfid);
HDFLIBAPI int ncabort(int cdfid);
HDFLIBAPI int ncdimdef(int cdfid, const char *name, long length);
HDFLIBAPI int ncdiminq(int cdfid, int dimid, char *name, long *length);
HDFLIBAPI int ncvardef(int cdfid, const char *name, nc_type datatype, int ndims, const int *dim);
HDFLIBAPI int ncvarid(int cdfid, const char *name);
HDFLIBAPI int ncvarinq(int cdfid, int varid, char *name, nc_type *datatype, int *ndims, int *dim, int *natts);
HDFLIBAPI int ncvarput1(int cdfid, int varid, const long *coords, const void *value);
HDFLIBAPI int ncvarget1(int cdfid, int varid, const long *coords, void *value);
HDFLIBAPI int ncvarput(int cdfid, int varid, const long *start, const long *count, void *value);
HDFLIBAPI int ncvarget(int cdfid, int varid, const long *start, const long *count, void *value);
HDFLIBAPI int ncvarputs(int cdfid, int varid, const long *start, const long *count, const long *stride,
                        void *values);
HDFLIBAPI int ncvargets(int cdfid, int varid, const long *start, const long *count, const long *stride,
                        void *values);
HDFLIBAPI int ncvarputg(int cdfid, int varid, const long *start, const long *count, const long *stride,
                        const long *imap, void *values);
HDFLIBAPI int ncvargetg(int cdfid, int varid, const long *start, const long *count, const long *stride,
                        const long *imap, void *values);
HDFLIBAPI int ncattput(int cdfid, int varid, const char *name, nc_type datatype, int len, const void *value);
HDFLIBAPI int nctypelen(nc_type datatype);
HDFLIBAPI int ncsetfill(int cdfid, int fillmode);

/* Internal API calls */
HDFLIBAPI void nc_serror(const char *fmt, ...);
HDFLIBAPI void NCadvise(int err, const char *fmt, ...);

HDFLIBAPI int NC_computeshapes(NC *handle);
HDFLIBAPI int NC_xtypelen(nc_type type);
HDFLIBAPI int NC_xlen_array(NC_array *array);
HDFLIBAPI int NC_xlen_attr(NC_attr **app);
HDFLIBAPI int NC_xlen_cdf(NC *cdf);
HDFLIBAPI int NC_xlen_dim(NC_dim **dpp);
HDFLIBAPI int NC_xlen_iarray(NC_iarray *iarray);
HDFLIBAPI int NC_xlen_string(NC_string *cdfstr);
HDFLIBAPI int NC_xlen_var(NC_var **vpp);

HDFLIBAPI void NC_arrayfill(void *lo, size_t len, nc_type type);
HDFLIBAPI void NC_copy_arrayvals(char *target, NC_array *array);
HDFLIBAPI int  NC_free_array(NC_array *array);
HDFLIBAPI int  NC_free_attr(NC_attr *attr);
HDFLIBAPI int  NC_free_cdf(NC *handle);
HDFLIBAPI int  NC_free_dim(NC_dim *dim);
HDFLIBAPI int  NC_free_iarray(NC_iarray *iarray);
HDFLIBAPI int  NC_free_string(NC_string *cdfstr);
HDFLIBAPI int  NC_free_var(NC_var *var);

HDFLIBAPI uint8_t *NC_incr_array(NC_array *array, uint8_t *tail);

HDFLIBAPI int    NC_dimid(NC *handle, char *name);
HDFLIBAPI bool_t NCcktype(nc_type datatype);
HDFLIBAPI bool_t NC_indefine(int cdfid, bool_t iserr);
HDFLIBAPI bool_t xdr_cdf(XDR *xdrs, NC **handlep);
HDFLIBAPI bool_t xdr_numrecs(XDR *xdrs, NC *handle);
HDFLIBAPI bool_t xdr_shorts(XDR *xdrs, short *sp, unsigned cnt);
HDFLIBAPI bool_t xdr_NC_array(XDR *xdrs, NC_array **app);
HDFLIBAPI bool_t xdr_NC_attr(XDR *xdrs, NC_attr **app);
HDFLIBAPI bool_t xdr_NC_dim(XDR *xdrs, NC_dim **dpp);
HDFLIBAPI bool_t xdr_NC_fill(XDR *xdrs, NC_var *vp);
HDFLIBAPI bool_t xdr_NC_iarray(XDR *xdrs, NC_iarray **ipp);
HDFLIBAPI bool_t xdr_NC_string(XDR *xdrs, NC_string **spp);
HDFLIBAPI bool_t xdr_NC_var(XDR *xdrs, NC_var **vpp);

HDFLIBAPI size_t NC_typelen(nc_type type);

HDFLIBAPI NC        *NC_check_id(int cdfid);
HDFLIBAPI NC        *NC_new_cdf(const char *name, int mode);
HDFLIBAPI NC_array  *NC_new_array(nc_type type, unsigned count, const void *values);
HDFLIBAPI NC_array  *NC_re_array(NC_array *old, nc_type type, unsigned count, const void *values);
HDFLIBAPI NC_attr   *NC_new_attr(const char *name, nc_type type, unsigned count, const void *values);
HDFLIBAPI NC_attr  **NC_findattr(NC_array **ap, const char *name);
HDFLIBAPI NC_dim    *NC_new_dim(const char *name, long size);
HDFLIBAPI NC_iarray *NC_new_iarray(unsigned count, const int values[]);
HDFLIBAPI NC_string *NC_new_string(unsigned count, const char *str);
HDFLIBAPI NC_string *NC_re_string(NC_string *old, unsigned count, const char *str);
HDFLIBAPI NC_var    *NC_hlookupvar(NC *handle, int varid);
HDFLIBAPI NC_var    *NC_new_var(const char *name, nc_type type, int ndims, const int *dims);
HDFLIBAPI int        NCvario(NC *handle, int varid, const long *start, const long *edges, void *values);
HDFLIBAPI bool_t     NCcoordck(NC *handle, NC_var *vp, const long *coords);
HDFLIBAPI bool_t     xdr_NCvshort(XDR *xdrs, unsigned which, short *values);
HDFLIBAPI bool_t     NC_dcpy(XDR *target, XDR *source, long nbytes);
HDFLIBAPI int        NCxdrfile_create(XDR *xdrs, const char *path, int ncmode);

HDFLIBAPI int       hdf_fill_array(uint8_t *storage, int32 len, uint8_t *value, int32 type);
HDFLIBAPI int       hdf_get_data(NC *handle, NC_var *vp);
HDFLIBAPI int32     hdf_get_vp_aid(NC *handle, NC_var *vp);
HDFLIBAPI int       hdf_map_type(nc_type);
HDFLIBAPI nc_type   hdf_unmap_type(int);
HDFLIBAPI int       hdf_get_ref(NC *, int);
HDFLIBAPI int       hdf_create_dim_vdata(XDR *, NC *, NC_dim *);
HDFLIBAPI int       hdf_create_compat_dim_vdata(XDR *xdrs, NC *handle, NC_dim *dim, int32 dimval_ver);
HDFLIBAPI int       hdf_write_attr(XDR *, NC *, NC_attr **);
HDFLIBAPI int32     hdf_write_dim(XDR *, NC *, NC_dim **, int32);
HDFLIBAPI int32     hdf_write_var(XDR *, NC *, NC_var **);
HDFLIBAPI int       hdf_write_xdr_cdf(XDR *, NC **);
HDFLIBAPI int       hdf_conv_scales(NC **);
HDFLIBAPI int       hdf_read_dims(XDR *, NC *, int32);
HDFLIBAPI NC_array *hdf_read_attrs(XDR *, NC *, int32);
HDFLIBAPI int       hdf_read_vars(XDR *, NC *, int32);
HDFLIBAPI int       hdf_read_xdr_cdf(XDR *, NC **);
HDFLIBAPI int       hdf_xdr_cdf(XDR *, NC **);
HDFLIBAPI int       hdf_vg_clobber(NC *, int);
HDFLIBAPI int       hdf_cdf_clobber(NC *);
HDFLIBAPI int       hdf_close(NC *);
HDFLIBAPI int       hdf_read_sds_dims(NC *);
HDFLIBAPI int       hdf_read_sds_cdf(XDR *, NC **);

HDFLIBAPI int SDPfreebuf(void);

HDFLIBAPI int NCgenio(NC *handle, int varid, const long *start, const long *count, const long *stride,
                      const long *imap, void *values);
HDFLIBAPI int NC_var_shape(NC_var *var, NC_array *dims);
HDFLIBAPI int NC_reset_maxopenfiles(int req_max);
HDFLIBAPI int NC_get_maxopenfiles(void);
HDFLIBAPI int NC_get_systemlimit(void);
HDFLIBAPI int NC_get_numopencdfs(void);

HDFLIBAPI nc_type cdf_unmap_type(int type);

HDFLIBAPI bool_t nssdc_read_cdf(XDR *xdrs, NC **handlep);
HDFLIBAPI bool_t nssdc_write_cdf(XDR *xdrs, NC **handlep);
HDFLIBAPI bool_t nssdc_xdr_cdf(XDR *xdrs, NC **handlep);

HDFLIBAPI int HDiscdf(const char *filename);
HDFLIBAPI int HDisnetcdf(const char *filename);
HDFLIBAPI int HDisnetcdf64(const char *filename);

#ifdef __cplusplus
}
#endif

#endif /* MFH4_NC_PRIV_H */
