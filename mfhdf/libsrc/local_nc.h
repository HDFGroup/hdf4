/*
 *	Copyright 1993, University Corporation for Atmospheric Research
 *      See netcdf/COPYRIGHT file for copying and redistribution conditions.
 */
/* $Id$ */
#ifndef _LOCAL_NC_
#define _LOCAL_NC_

/*
 *	netcdf library 'private' data structures, objects and interfaces
 */

#include	<stddef.h> /* size_t */
#include	<stdio.h> /* FILENAME_MAX */
#ifndef FILENAME_MAX
#define FILENAME_MAX  255
#endif

#ifndef       NO_SYS_XDR_INC
#ifdef VMS
#    define  STDC_INCLUDES
#endif   /* VMS */
#include	<rpc/types.h>
#include	<rpc/xdr.h>
#else
#if defined(macintosh) | defined (THINK_C)
     /* For the mac renamed "types.h" to "rtypes.h" 
        because we don't want it to pick up the system one */
#include      "rtypes.h" /* "../xdr/rtypes.h" */
#include      "xdr.h"    /* "../xdr/xdr.h" */
#else /* !macintosh */
#include      <types.h>  /* "../xdr/types.h" */
#include      <xdr.h>    /* "../xdr/xdr.h" */
#endif /* !macintosh */
#endif /* !NO_SYSTEM_XDR_INCLUDES */

#include	"netcdf.h" /* needed for defs of nc_type, ncvoid, ... */

/* ptr argument type in internal functions */
#define Void    char

/*
** Include HDF stuff
*/
#ifdef HDF

#include "hdf.h"
#include "vg.h"

#define ATTR_TAG  DFTAG_VH
#define DIM_TAG   DFTAG_VG
#define VAR_TAG   DFTAG_VG
#define DATA_TAG  DFTAG_SD
#define BOGUS_TAG ((uint16) 721)

#define ATTRIBUTE         "Attr0.0"
#define VARIABLE          "Var0.0"
#define DIMENSION         "Dim0.0"
#define UDIMENSION        "UDim0.0"
#define DIM_VALS          "DimVal0.0" 
#define CDF               "CDF0.0"
#define DATA              "Data0.0"
#define ATTR_FIELD_NAME   "Values"

#define BLOCK_SIZE  64    /* multiplier for bytes in linked blocks */
#define BLOCK_COUNT 128   /* size of linked block pointer objects  */

#endif

/* like, a discriminated union in the sense of xdr */
typedef struct {
	nc_type type ;		/* the discriminant */
	size_t len ;		/* the total length originally allocated */
	size_t szof ;		/* sizeof each value */
	unsigned count ;	/* length of the array */
	Void *values ;		/* the actual data */
} NC_array ;

/* Counted string for names and such */
/* 

  count is the actual size of the buffer for the string
  len is the length of the string in the buffer
  
  count != len when a string is resized to something smaller

*/
typedef struct {
	unsigned count ;
        unsigned len ; 
	char *values ;
} NC_string ;

/* Counted array of ints for assoc list */
typedef struct {
	unsigned count ;
	int *values ;
} NC_iarray ;

/* NC dimension stucture */
typedef struct {
	NC_string *name ;
        long size ;
#ifdef HDF
	int32 vgid;   /* id of the Vgroup representing this dimension */
        int32 count;  /* Number of pointers to this dimension */
#endif
} NC_dim ;

/* NC attribute */
typedef struct {
	NC_string	*name ;
	NC_array	*data ;
} NC_attr ;

/* NC variable: description and data */
typedef struct {
	NC_string *name ;
	NC_iarray *assoc ; /* user definition */
	unsigned long *shape ; /* compiled info */
	unsigned long *dsizes ; /* compiled info */
	NC_array *attrs;
	nc_type type ;		/* the discriminant */
	unsigned long len ;		/* the total length originally allocated */
	size_t szof ;		/* sizeof each value */
	long begin ;  /* seek index, often an off_t */
#ifdef HDF
	int32 vgid;     /* id of the variable's Vgroup */
        uint16 data_ref;  /* ref of the variable's data storage (if exists) */
        uint16 data_tag;  /* tag of the variable's data storage (if exists) */
        uint16 ndg_ref;   /* ref of ndg for this dataset */
        intn   data_offset; /* non-traditional data may not begin at 0 */
        int numrecs;  /* number of records this has been filled to */
        int32 aid;    /* aid for DFTAG_SD data */
        int32 HDFtype; /* type of this variable as HDF thinks */
        int32 HDFsize; /* size of this variable as HDF thinks */
        int32   is_ragged; /* BOOLEAN == is a ragged array */
        int32 * rag_list;  /* size of ragged array lines */
        int32   rag_fill;  /* last line in rag_list to be set */
#endif
} NC_var ;

#define IS_RECVAR(vp) \
	((vp)->shape != NULL ? (*(vp)->shape == NC_UNLIMITED) : 0 )

#define netCDF_FILE  0
#define HDF_FILE     1
#define CDF_FILE     2

typedef struct {
	char path[FILENAME_MAX + 1] ;
	unsigned flags ;
	XDR *xdrs ;
	long begin_rec ; /* (off_t) postion of the first 'record' */
	unsigned long recsize ; /* length of 'record' */
	int redefid ;
	/* below gets xdr'd */
	unsigned long numrecs ; /* number of 'records' allocated */
	NC_array *dims ;
	NC_array *attrs ;
	NC_array *vars ;
#ifdef HDF
	int32 hdf_file;
        int file_type;
        int32 vgid;
        int hdf_mode; /* mode we are attached for */
        FILE * cdf_fp; /* file pointer used for CDF files */
#endif
} NC ;

extern char *cdf_routine_name ; /* defined in lerror.c */

               /*  C D F 1 */
#define	NCMAGIC	0x43444601
                       /*  C D L 1 */
#define	NCLINKMAGIC	0x43444c01

#ifndef HDF /* HDF has already worked out if we have prototypes */
#undef PROTO
#ifndef NO_HAVE_PROTOTYPES 
#   define	PROTO(x)	x
#else
#   define	PROTO(x)	()
#endif
#endif /* HDF */

#ifdef __cplusplus
extern "C" {
#endif

extern void		nc_serror			PROTO((
	char *fmt,
	...
)) ;
extern void		NCadvise			PROTO((
	int err,
	char *fmt,
	...
)) ;

extern int        NC_computeshapes	PROTO((
    NC		*handle
));
extern int        NC_xtypelen		PROTO((
    nc_type	type
));
extern int        NC_xlen_array		PROTO((
    NC_array	*array
));
extern int        NC_xlen_attr		PROTO((
    NC_attr	**app
));
extern int        NC_xlen_cdf		PROTO((
    NC		*cdf
));
extern int        NC_xlen_dim		PROTO((
    NC_dim	**dpp
));
extern int        NC_xlen_iarray	PROTO((
    NC_iarray	*iarray
));
extern int        NC_xlen_string	PROTO((
    NC_string	*cdfstr
));
extern int        NC_xlen_var		PROTO((
    NC_var	**vpp
));

extern char       *NCmemset		PROTO((
    char	*s,
    int		c,
    int		n
));

extern void       NC_arrayfill		PROTO((
    Void	*lo,
    size_t	len,
    nc_type	type
));
extern void       NC_copy_arrayvals	PROTO((
    char	*target,
    NC_array	*array
));
extern void       NC_free_array		PROTO((
    NC_array	*array
));
extern void       NC_free_attr		PROTO((
    NC_attr	*attr
));
extern void       NC_free_cdf		PROTO((
    NC		*handle
));
extern void       NC_free_dim		PROTO((
    NC_dim	*dim
));
extern void       NC_free_iarray	PROTO((
    NC_iarray	*iarray
));
extern void       NC_free_string	PROTO((
    NC_string	*cdfstr
));
extern void       NC_free_var		PROTO((
    NC_var	*var
));

extern Void      *NC_incr_array		PROTO((
    NC_array	*array,
    Void	*tail
));

extern bool_t     NCcktype		PROTO((
    nc_type	datatype
));
extern bool_t     NC_indefine		PROTO((
    int		cdfid,
    bool_t	iserr
));
extern bool_t     xdr_cdf		PROTO((
    XDR		*xdrs,
    NC		**handlep
));
extern bool_t     xdr_numrecs		PROTO((
    XDR		*xdrs,
    NC		*handle
));
extern bool_t     xdr_shorts		PROTO((
    XDR		*xdrs,
    short	*sp,
    u_int	cnt
));
extern bool_t     xdr_NC_array		PROTO((
    XDR		*xdrs,
    NC_array	**app
));
extern bool_t     xdr_NC_attr		PROTO((
    XDR		*xdrs,
    NC_attr	**app
));
extern bool_t     xdr_NC_dim		PROTO((
    XDR		*xdrs,
    NC_dim	**dpp
));
extern bool_t     xdr_NC_fill		PROTO((
    XDR		*xdrs,
    NC_var	*vp
));
extern bool_t     xdr_NC_iarray		PROTO((
    XDR		*xdrs,
    NC_iarray	**ipp
));
extern bool_t     xdr_NC_string		PROTO((
    XDR		*xdrs,
    NC_string	**spp
));
extern bool_t     xdr_NC_var		PROTO((
    XDR		*xdrs,
    NC_var	**vpp
));

extern size_t     NC_typelen		PROTO((
    nc_type	type
));

extern NC        *NC_check_id		PROTO((
    int		cdfid
));
extern NC        *NC_dup_cdf		PROTO((
    const char *name,
	int     mode,
    NC		*old
));
extern NC        *NC_new_cdf		PROTO((
    const char *name,
    int		mode
));
extern NC_array  *NC_new_array		PROTO((
    nc_type	type,
    unsigned	count,
    const void	*values
));
extern NC_array  *NC_re_array		PROTO((
    NC_array	*old,
    nc_type	type,
    unsigned	count,
    const void	*values
));
extern NC_attr  *NC_new_attr        PROTO((
    const char *name,
    nc_type type,
    unsigned count ,
    const void *values
));
extern NC_attr  **NC_findattr		PROTO((
    NC_array	**ap,
    const char	*name
));
extern NC_dim    *NC_new_dim		PROTO((
    const char	*name,
    long	size
));
extern NC_iarray *NC_new_iarray		PROTO((
    unsigned	count,
    const int		values[]
));
extern NC_string *NC_new_string		PROTO((
    unsigned	count,
    const char	*str
));
extern NC_string *NC_re_string		PROTO((
    NC_string	*old,
    unsigned	count,
    const char	*str
));
extern NC_var    *NC_hlookupvar		PROTO((
    NC		*handle,
    int		varid
));
extern NC_var    *NC_new_var		PROTO((
    const char	*name,
    nc_type	type,
    int		ndims,
    const int		*dims
));
extern int	NCvario			PROTO((
	NC *handle,
	int varid,
	const long *start,
	const long *edges,
	Void *values
));
extern bool_t	NCcoordck	PROTO((
	NC *handle,
	NC_var *vp, 
	const long *coords
));
extern bool_t xdr_NCvshort    PROTO((
        XDR *xdrs,
        unsigned which,
        short *values
));
extern bool_t	NC_dcpy			PROTO((
	XDR *target,
	XDR *source,
	long nbytes
));
extern int NCxdrfile_sync
    PROTO((XDR *xdrs));

extern int NCxdrfile_create
    PROTO((XDR *xdrs,const char *path,int ncmode));

#ifdef HDF
extern int hdf_fill_array
    PROTO((Void  * storage,int32 len,Void  * value,int32 type));

extern int hdf_get_data
    PROTO((NC *handle,NC_var *vp));

extern int hdf_map_type
    PROTO((int ));

extern int hdf_unmap_type
    PROTO((int ));

extern int hdf_get_ref
    PROTO((NC *,int ));

extern int hdf_create_dim_vdata
    PROTO((XDR *,NC *,NC_dim *));

extern int hdf_write_attr
    PROTO((XDR *,NC *,NC_attr **));

extern int32 hdf_write_dim
    PROTO((XDR *,NC *,NC_dim **,int32));

extern int32 hdf_write_var
    PROTO((XDR *,NC *,NC_var **));

extern int hdf_write_xdr_cdf
    PROTO((XDR *,NC **));

extern int hdf_read_dims
    PROTO((XDR *,NC *,int32 ));

extern NC_array *hdf_read_attrs
    PROTO((XDR *,NC *,int32 ));

extern int hdf_read_vars
    PROTO((XDR *,NC *,int32 ));

extern int hdf_read_xdr_cdf
    PROTO((XDR *,NC **));

extern int hdf_xdr_cdf
    PROTO((XDR *,NC **));

extern void hdf_vg_clobber
    PROTO((NC *,int ));

extern void hdf_cdf_clobber
    PROTO((NC *));

extern void hdf_close
    PROTO((NC *));

extern intn hdf_read_sds_dims
    PROTO((NC *));

extern int hdf_read_sds_cdf
    PROTO((XDR *,NC **));

extern int NCgenio
    PROTO((NC *handle, int varid, const long *start, const long *count,
        const long *stride, const long *imap,Void *values));

extern int NC_var_shape
    PROTO((NC_var *var,NC_array *dims));

#endif /* HDF */

#ifdef __cplusplus
}
#endif

#endif /* _LOCAL_NC_ */
