#ifdef RCSID
static char RcsId[] = "@(#)$Revision$";
#endif
/*
$Header$

$Log$
Revision 1.3  1993/01/26 19:42:27  koziol
Added support for reading and writing Little-Endian data on all
platforms.  This has been tested on: Cray, Sun, and PCs so far.

 * Revision 1.2  1993/01/19  05:54:41  koziol
 * Merged Hyperslab and JPEG routines with beginning of DEC ALPHA
 * port.  Lots of minor annoyances fixed.
 *
 * Revision 1.1  1992/09/04  21:25:52  chouck
 * Initial revision
 *
*/
/*-----------------------------------------------------------------------------
 * Updated with dfsd.c:
 * Revision 1.37

 File:  dfnd.c

 Purpose:
    Routines for input and output of numeric data group

 Invokes:
    df.c dfgroup.c dfkit.c dfi.h df.h dfsd.h

 Public functions:
    DFNDgetdims - get rank and dim sizes
    DFNDgetdatastrs - get label, unit, format and coord system of data
    DFNDgetdimstrs - get label, unit and format for a dimension
    DFNDgetdatalen - 
    DFNDgetdimlen -
    DFNDgetdimscale - get scale for a dimension
    DFNDgetrange - get max and min of data
    DFNDgetdata - get data values
    DFNDgetNT - get file number type for reading
    DFNDsetlengths - set lengths of label, unit, format strings on gets
    DFNDsetdims - set rank and dim sizes
    DFNDsetdatastrs - set data label, unit, format and coord system
    DFNDsetdimstrs - set dim labels, units and formats
    DFNDsetdimscale - set scale for a dimension
    DFNDsetrange - set max and min of data
    DFNDsetNT - set number type to be written out
    DFNDadddata - write out a complete SD
    DFNDrestart - forget info about last file accessed - restart from beginning
    DFNDnumber - return number of SDGs in file
    DFNDclear - forget all info set
    DFNDlastref - get reference number of last SDG read or written
    DFNDreadref - set the reference number of the next SDG to be read
    DFNDsettype - set output data type, m/c format, number type and array order
    DFNDgetslice - get part of the data, specified as a slice
    DFNDstartslice - set up to write SD
    DFNDputslice - write specified number of data items to file
    DFNDendslice - end of series of writes, write out SDG
    DFNDopen - open or create a file in preparation for reading or writing SDGs
    DFNDclose - close a file after reading or writing SDGs

 Lower level functions:
    DFNDgetsdg - read SDG into struct
    DFNDputsdg - write SDG to file

 Private functions:
    DFNDIsdginfo - find next sdg in file
    DFNDIisndg - was currently read sdg written by HDF3.2
    DFNDIgetrrank - get rank of the currently read sdg
    DFNDIgetwrank - get rank of the sdg to be written
    DFNDIclear - clear sdg data structure of all info
    DFNDIgetdata - read data from file
    DFNDIgetslice - get slice
    DFNDIputslice - put slice
    DFNDIendslice -
    DFNDIsetnsdg_t - set up nsdg table
    DFNDInextnsdg - get next nsdg from nsdg table
    DFNDIgetndg - read NDG into struct
    DFNDIputndg - write NDG to file

 *---------------------------------------------------------------------------*/

#include "hdf.h"
#include "herr.h"
#include "dfnd.h"
#include "hfile.h"
#include "dfgroup.h"
#ifdef WIN3
#include "c:\windev\include\malloc.h"
#endif

/* MMM: make this definition correct and move to hfile.h, or wherever. */
#define DF_NOFILE -1

#define LABEL   0
#define UNIT    1
#define FORMAT  2
#define COORDSYS 3

#define NFGSDG_TYPE_SDG	0	/* a pure SDG  */
#define NFGSDG_TYPE_NFG	1	/* a pure NDG  */
#define NFGSDG_TYPE_SDGNDG 2	/* an SDG in NDG */

/* In ver. 3.2 numbertype and file number format (subclass) are included  */
/* in DFSsdg, and  fileNTsize is local to functions . 		*/ 
/* static int fileNT=DFNTF_IEEE,	 default: all IEEE       */
/*           fileNTsize=4,			 */
/*           outNT=DFNTF_IEEE,		 default output: IEEE */
/*           outNTsize=4, 			 */
/*           userNT=DFNTF_IEEE ;	 default */

/* Global NDG/SDG information */
typedef struct {
    int32 Hid;             /* H-layer id for this file */
    DFnsdg_t_hdr *nsdghdr; /* NSDG table headers */
    int num_accs;          /* number of DFNDopen's to this file */
} NDGFILE;

typedef struct {
    DFSsdg Readsdg;       /* struct for reading */
    DFSsdg Writesdg;      /* struct for writing */
    Stat Ref;             /* status of write information: -1, 0, 1 */
    uint16 Writeref;  /* ref of next SDG/NDG to write to file */
    int Newdata;	/* Values in Readsdg fresh? */
				/* -1 : no descriptor read */
				/* 1 : descriptor read */
    int Nextsdg;   /* Signal if DFNDgetdata should get the next SDG/NDG */
    int Maxstrlen[4];
    int Ismaxmin;          /* is there a max/min value on read? */
    int FileTranspose;     /* is the data in column major order? */
    int Fortorder;         /* should data be written col major? */
    uint16 Readref;
    uint16 Lastref;
    DFdi lastnsdg;	   /* last read nsdg in nsdg_t */
} NDGCONTEXT;

typedef struct {
    NDGFILE *file;
    NDGCONTEXT *context;
} NDFILEREC;

static NDGID Sfile_id = (NDGID) DF_NOFILE;  /* file for slice writes */
static int32 *Sddims;        /* dims written so far in slice write */
static char *Lastfile = "";    /* last file opened by DFSD call */

static NDFILEREC *ndrec[MAX_FILE + 1];

/*-----------------------------------------------------------------------------
 * Name:    DFNDgetdims
 * Purpose: Get dimensions of data in next SDG
 * Inputs:  slot: pre-calculated array slot to use
 *          prank: pointer to integer for returning rank (no of dimensions)
 *          sizes: array of integers for returning size of each dimension
 *          maxrank: size of array for returning dimensions
 * Returns: 0 on success, FAIL on failure with ERROR set
 * Outputs: rank in prank, size of each dimension in sizes
 *          If rank > maxrank, rank is set, and -1 is returned
 * Users:   HDF users, utilities, other routines
 * Invokes: HERROR, DFNDIsdginfo
 * Method:  Opens file, calls DFNDIsdginfo to get SDG, copies rank etc, closes
 *          file, returns
 * Remarks: Always sequences to next SDG in file
 *          User specifies maxrank, and allocates sizes as an array of integers
 *          with dimension maxrank
 *---------------------------------------------------------------------------*/

#ifdef PROTOTYPE
int DFNDgetdims(NDGID id, intn *prank, int32 sizes[], intn maxrank)
#else
int DFNDgetdims(id, prank, sizes, maxrank)
NDGID id;
intn *prank;
int32 sizes[];
intn maxrank;
#endif /* PROTOTYPE */
{
    int i;
    NDFILEREC *rec = NDGID2REC(id);
    char *FUNC = "DFNDgetdims";

    HEclear();

    if (!VALIDNDGID(id)) {
        HERROR(DFE_BADCALL); return FAIL;
    }

    if (DFNDIsdginfo(id) < 0) {      /* reads next SDG from file */
        return FAIL;    /* on error, return */
    }

    if (!prank) {                        /* check if ptr is valid */
        HERROR(DFE_BADPTR); return FAIL;
    }

    *prank = rec->context->Readsdg.rank;	/* copy rank, dimensions */
    if (maxrank < *prank) {	/* if not all dimensions copied */
        HERROR(DFE_NOTENOUGH); return FAIL;
    }
    for (i = 0; i < *prank; i++)
        sizes[i] = rec->context->Readsdg.dimsizes[i];
    rec->context->Nextsdg = 0;
    return(0);
}

/*-----------------------------------------------------------------------------
 * Name:    DFNDgetdatastrs
 * Purpose: Get information about data: label, units, format
 * Inputs:  id: id of open HDF file
 *          label: string to return label in, length Maxstrlen[LABEL]
 *          unit: string to return unit in, length Maxstrlen[UNIT]
 *          format: string to return format in, length Maxstrlen[FORMAT]
 *          coordsys: string to return coord system, length Maxstrlen[COORDSYS]
 * Returns: 0 on success, FAIL on failure with ERROR set
 * Outputs: label, unit, format, coord system in the appropriate arguments
 * Users:   HDF users, utilities, other routines
 * Invokes: none
 * Method:  get values from struct Readsdg
 * Remarks: none
 *---------------------------------------------------------------------------*/
#ifdef PROTOTYPE
int DFNDgetdatastrs(NDGID id, char *label, char *unit, char *format,
                    char *coordsys)
#else
int DFNDgetdatastrs(id, label, unit, format, coordsys)
NDGID id;
char *label, *unit, *format, *coordsys;
#endif /* PROTOTYPE */
{
    char *FUNC = "DFNDgetdatastrs";

    HEclear();

    if (!VALIDNDGID(id)) {
        HERROR(DFE_BADCALL); return FAIL;
    }
    return(DFNDIgetdatastrs(NDGID2SLOT(id), label, unit, format, coordsys));
}
 
#ifdef PROTOTYPE
int DFNDIgetdatastrs(int slot, char *label, char *unit, char *format,
                    char *coordsys)
#else
int DFNDIgetdatastrs(slot, label, unit, format, coordsys)
int slot;
char *label, *unit, *format, *coordsys;
#endif /* PROTOTYPE */
{
    int32 luf;
    NDFILEREC *rec = NDGSLOT2REC(slot);
    char *lufp;
    char *FUNC="DFNDIgetdatastrs";

    HEclear();
    
    if (rec->context->Newdata < 0) {
        HERROR(DFE_BADCALL); return FAIL;
    }

/* NOTE: Once DFNDsetdatastrs is changed to always write all three (label,
         unit and format) whenever it is called, this routine should be
         changed so that it returns all three, if any exist.  This means
         that it also should be changed to return -1 if none exist.
         (Currently it returns FAIL only if the SDS doesn't exist.)
*/

    /* copy label, unit, format */
    for (luf=LABEL; luf<=FORMAT; luf++) {
        lufp = (luf==LABEL) ? label : (luf==UNIT) ? unit : format;
        if (lufp)
            if (rec->context->Readsdg.dataluf[luf])
                HDstrncpy(lufp, rec->context->Readsdg.dataluf[luf],
                          rec->context->Maxstrlen[luf]);
    }
    /* copy coordsys */
    if (coordsys)
       if (rec->context->Readsdg.coordsys)
          HDstrncpy(coordsys, rec->context->Readsdg.coordsys, rec->context->Maxstrlen[COORDSYS]);
       else coordsys[0] = '\0';
    return(0);
}

/*-----------------------------------------------------------------------------
 * Name:    DFNDgetdimstrs
 * Purpose: Get information about a dimension: label, units, format
 * Inputs:  slot: array slot 
 *          dim: no of dimension to get information about
 *          label: string to return label in, max length Maxstrlen[LABEL]
 *          unit: string to return unit in, max length Maxstrlen[UNIT]
 *          format: string to return format in, max length Maxstrlen[FORMAT]
 * Returns: 0 on success, FAIL on failure with ERROR set
 * Outputs: label, unit, format in the appropriate arguments
 *          NULL string if no value for the arguments
 * Users:   HDF users, utilities, other routines
 * Invokes: none
 * Method:  get values from struct Readsdg
 * Remarks: none
 *---------------------------------------------------------------------------*/

#ifdef PROTOTYPE
int DFNDgetdimstrs(NDGID id, int dim, char *label, char *unit, char *format)
#else
int DFNDgetdimstrs(id, dim, label, unit, format)
NDGID id;
int dim;
char *label, *unit, *format;
#endif /* PROTOTYPE */
{
    char *FUNC = "DFNDgetdimstrs";

    HEclear();

    if (!VALIDNDGID(id)) {
        HERROR(DFE_BADCALL); return FAIL;
    }

    return(DFNDIgetdimstrs(NDGID2SLOT(id), dim, label, unit, format));
}

#ifdef PROTOTYPE
int DFNDIgetdimstrs(int slot, int dim, char *label, char *unit, char *format)
#else
int DFNDIgetdimstrs(slot, dim, label, unit, format)
int32 slot;
int dim;
char *label, *unit, *format;
#endif /* PROTOTYPE */
{
    int luf, rdim;
    NDFILEREC *rec = NDGSLOT2REC(slot);
    char *lufp;
    char *FUNC = "DFNDIgetdimstrs";

    HEclear();
    
    if (rec->context->Newdata < 0) {
        HERROR(DFE_BADCALL); return FAIL;
    }

/* NOTE: Once DFNDsetdimstrs is changed to always write all three (label,
         unit and format) whenever it is called, this routine should be
         changed so that it returns all three, if any exist.  This means
         that it also should be changed to return -1 if none exist. 
         (Currently it returns FAIL only if the SDS doesn't exist.)
*/

    rdim = dim - 1;        /* translate dim to zero origin */
    if ((rdim >= rec->context->Readsdg.rank) || (rdim < 0)) {
        HERROR(DFE_BADDIM); return FAIL;
    }

    /* copy labels etc */
    for (luf = LABEL; luf <= FORMAT; luf++) {
        lufp = (luf == LABEL) ? label : (luf == UNIT) ? unit : format;
        if (lufp) {
            if (!rec->context->Readsdg.dimluf) { /* no labels etc */
                *lufp = '\0';
                continue;
            }
            if (rec->context->Readsdg.dimluf[luf])
                HDstrncpy(lufp, rec->context->Readsdg.dimluf[luf][rdim],
                          rec->context->Maxstrlen[luf]);
        }
    }
    return(0);
}
                
/*-----------------------------------------------------------------------------
 * Name:    DFNDgetdatalen
 * Purpose: Get actual length of label, unit, format, coordsys strings
 *          Called from FORTRAN
 * Inputs:  id: id of open HDF file
 *          llabel, lunit, lformat, lcoordsys - for returning lengths
 * Globals: Readsdg
 * Returns: 0 on success, FAIL on error with error set
 * Users:   HDF users, utilities, other routines
 * Invokes: none
 * Method:  get lengths from Readsdg
 *---------------------------------------------------------------------------*/

#ifdef PROTOTYPE
int DFNDgetdatalen(NDGID id, int *llabel, int *lunit, int *lformat,
                   int *lcoordsys)
#else
int DFNDgetdatalen(id, llabel, lunit, lformat, lcoordsys)
NDGID id;
int *llabel, *lunit, *lformat, *lcoordsys;
#endif /* PROTOTYPE */
{
    char *FUNC = "DFNDgetdatalen";

    HEclear();

    if(!VALIDNDGID(id)) {
        HERROR(DFE_BADCALL);  return FAIL;
    }

    return(DFNDIgetdatalen(NDGID2SLOT(id), llabel, lunit, lformat, lcoordsys));
}

#ifdef PROTOTYPE
int DFNDIgetdatalen(int slot, int *llabel, int *lunit, int *lformat,
                   int *lcoordsys)
#else
int DFNDIgetdatalen(slot, llabel, lunit, lformat, lcoordsys)
int slot;
int *llabel, *lunit, *lformat, *lcoordsys;
#endif /* PROTOTYPE */
{
    NDFILEREC *rec = NDGSLOT2REC(slot);
    char *FUNC = "DFNDIgetdatalen";

    HEclear();

    if (rec->context->Newdata < 0) { 
        HERROR(DFE_BADCALL); return FAIL; 
    }

    *llabel = rec->context->Readsdg.dataluf[LABEL]
               ? DFIstrlen(rec->context->Readsdg.dataluf[LABEL]) : 0;
    *lunit = rec->context->Readsdg.dataluf[UNIT]
               ? DFIstrlen(rec->context->Readsdg.dataluf[UNIT]) : 0;
    *lformat = rec->context->Readsdg.dataluf[FORMAT]
               ? DFIstrlen(rec->context->Readsdg.dataluf[FORMAT]) : 0;
    *lcoordsys = rec->context->Readsdg.coordsys
               ? DFIstrlen(rec->context->Readsdg.coordsys) : 0;
    return(0);
}

/*-----------------------------------------------------------------------------
 * Name:    DFNDgetdimlen
 * Purpose: Get actual length of label, unit, format strings
 *          Called from FORTRAN
 * Inputs:  id: id of open HDF file
 *          dim, llabel, lunit, lformat - for returning lengths
 * Globals: Readsdg
 * Returns: 0 on success, FAIL on error with error set
 * Users:   HDF users, utilities, other routines
 * Invokes: none
 * Method:  get lengths from Readsdg
 *---------------------------------------------------------------------------*/
#ifdef PROTOTYPE
int DFNDgetdimlen(NDGID id, int dim, int *llabel, int *lunit, int *lformat)
#else
int DFNDgetdimlen(id, dim, llabel, lunit, lformat)
NDGID id;
int dim, *llabel, *lunit, *lformat;
#endif /* PROTOTYPE */
{
    char *FUNC = "DFNDgetdimlen";

    HEclear();

    if (!VALIDNDGID(id)) {
        HERROR(DFE_BADCALL); return FAIL;
    }

    return(DFNDIgetdimlen(NDGID2SLOT(id), dim, llabel, lunit, lformat));
}

#ifdef PROTOTYPE
int DFNDIgetdimlen(int slot, int dim, int *llabel, int *lunit, int *lformat)
#else
int DFNDIgetdimlen(slot, dim, llabel, lunit, lformat)
int slot;
int dim;
int *llabel, *lunit, *lformat;
#endif /* PROTOTYPE */
{
    NDFILEREC *rec = NDGSLOT2REC(slot);
    char *FUNC = "DFNDIgetdimlen";

    HEclear();

    if (rec->context->Newdata < 0) { 
        HERROR(DFE_BADCALL); return FAIL; 
    }

    if (dim > rec->context->Readsdg.rank) { 
        HERROR(DFE_BADDIM); return FAIL; 
    }

    *llabel = rec->context->Readsdg.dimluf[LABEL][dim-1]
               ? DFIstrlen(rec->context->Readsdg.dimluf[LABEL][dim-1]) : 0;
    *lunit = rec->context->Readsdg.dimluf[UNIT][dim-1]
               ? DFIstrlen(rec->context->Readsdg.dimluf[UNIT][dim-1]) : 0;
    *lformat = rec->context->Readsdg.dimluf[FORMAT][dim-1]
               ? DFIstrlen(rec->context->Readsdg.dimluf[FORMAT][dim-1]) : 0;
    return(0);
}

/*-----------------------------------------------------------------------------
 * Name:    DFNDgetdimscale
 * Purpose: Get dimension scale
 * Inputs:  id: id of open HDF file
 *          dim: no of dimension to get scale for
 *          size: size of scale array
 *          scale: array to return scale in
 * Returns: 0 on success, FAIL on failure with error set
 * Outputs: scale if present, else -1
 * Users:   HDF users, utilities, other routines
 * Invokes: none
 * Method:  get values from struct Readsdg
 * Remarks: none
 *---------------------------------------------------------------------------*/
#ifdef PROTOTYPE
int DFNDgetdimscale(NDGID id, int dim, int32 maxsize, void *scale)
#else
int DFNDgetdimscale(id, dim, maxsize, scale)
NDGID id, maxsize;
int dim;
void *scale;
#endif /* PROTOTYPE */
{
    char *FUNC = "DFNDgetdimscale";

    HEclear();

    if (!VALIDNDGID(id)) {
        HERROR(DFE_BADCALL); return FAIL;
    }

    return(DFNDIgetdimscale(NDGID2SLOT(id), dim, maxsize, scale));
}

#ifdef PROTOTYPE
intn DFNDIgetdimscale(int slot, int dim, int32 maxsize, void *scale)
#else
intn DFNDIgetdimscale(slot, dim, maxsize, scale)
int slot, dim;
int32 maxsize;
void *scale;
#endif /* PROTOTYPE */
{
    int32 dimsize;
    int32 numtype, localNTsize;
    int rdim;
    uint8 *p1, *p2;
    NDFILEREC *rec = NDGSLOT2REC(slot);
    char *FUNC = "DFNDIgetdimscale";

    HEclear();
    
    if (rec->context->Newdata < 0) {
        HERROR(DFE_BADCALL); return FAIL;
    }
    rdim = dim-1;		/* translate dim to zero origin */
    if ((rdim >= rec->context->Readsdg.rank) || (rdim<0)) {
        HERROR(DFE_BADDIM); return FAIL;
    }
    if (maxsize < rec->context->Readsdg.dimsizes[rdim]) {
        HERROR(DFE_NOSPACE); return FAIL;
    }
    if (!scale) {
        HERROR(DFE_BADPTR); return FAIL;
    }
    if (!rec->context->Readsdg.dimscales || !rec->context->Readsdg.dimscales[rdim]) {
        /* no scale */  HERROR(DFE_NOVALS); return FAIL;
    }

    /* get number type and copy data from Readsdg to scale */
    if (rec->context->Readsdg.numbertype == DFNT_NONE)
        rec->context->Readsdg.numbertype = DFNT_FLOAT32;
    numtype = rec->context->Readsdg.numbertype;
    localNTsize = DFKNTsize(numtype | DFNT_NATIVE);
    dimsize = localNTsize * rec->context->Readsdg.dimsizes[rdim]; /* in bytes */

    p1 = (uint8 *)scale;
    p2 = (uint8 *)(rec->context->Readsdg.dimscales[rdim]);
    if (HDmemcopy(p2, p1, dimsize) != 0)     {
      return(-1);
    }
    return(0);
}

/*-----------------------------------------------------------------------------
 * Name:    DFNDgetrange
 * Purpose: Get maximum and minimum data values
 * Inputs:  id: id of open HDF file
 *          pmax: pointer to int8 to return maximum value in
 *          pmin: pointer to int8 to return minimum value in
 * Globals: Ismaxmin
 * Returns: 0 on success, -1 if no maxmin values or if error, with error set
 * Users:   HDF users, utilities, other routines
 * Invokes: none
 * Method:  Retrieves values from Readsdg
 * Remarks: none
 *---------------------------------------------------------------------------*/
#ifdef PROTOTYPE
int DFNDgetrange(NDGID id, void *pmax, void *pmin)
#else
int DFNDgetrange(id, pmax, pmin)
NDGID id;
void *pmax, *pmin;
#endif /* PROTOTYPE */
{
    char *FUNC = "DFNDgetrange";

    HEclear();

    if (!VALIDNDGID(id)) {
        HERROR(DFE_BADCALL); return FAIL;
    }

    return(DFNDIgetrange(NDGID2SLOT(id), pmax, pmin));
}

#ifdef PROTOTYPE
int DFNDIgetrange(int slot, void *pmax, void *pmin)
#else
int DFNDIgetrange(slot, pmax, pmin)
int slot;
void *pmax, *pmin;
#endif /* PROTOTYPE */
{
    int32 numtype, localNTsize;
    uint8 *p1, *p2;
    NDFILEREC *rec = NDGSLOT2REC(slot);
    char *FUNC = "DFNDIgetrange";

    HEclear();
    
    if (rec->context->Newdata < 0) {
        HERROR(DFE_BADCALL); return FAIL;
    }
    /* get number type and copy data  */
    if (rec->context->Readsdg.numbertype == DFNT_NONE)
        rec->context->Readsdg.numbertype = DFNT_FLOAT32;
    numtype = rec->context->Readsdg.numbertype;
    localNTsize = DFKNTsize(numtype | DFNT_NATIVE);

    if (rec->context->Ismaxmin ) {
        p1 = (uint8 *)pmax;
        p2 = (uint8 *)&(rec->context->Readsdg.max_min[0]);
        if (HDmemcopy(p2, p1, localNTsize) != 0)
            return(-1);
        p1 = (uint8 *)pmin;
        p2 = &(rec->context->Readsdg.max_min[localNTsize]);
        if (HDmemcopy(p2, p1, localNTsize) != 0)
            return(-1);
        return(0);
    } else {
        HERROR(DFE_NOVALS); return FAIL;
    }
}

/*-----------------------------------------------------------------------------
 * Name:    DFNDgetdata
 * Purpose: Get data from SDG.  Will sequence to next SDG if DFNDgetdims not
 *          called.
 * Inputs:  fileid: id of open HDF file to use
 *          rank: no of dimensions of array "data"
 *          maxsizes: actual dimensions of array "data"
 *          data: data for returning scientific data
 * Returns: 0 on success, FAIL on failure with error set
 * Outputs: actual scientific data in array
 * Users:   HDF users, utilities, other routines
 * Invokes: DFNDIgetdata
 * Method:  call DFNDIgetdata
 * Remarks: maxsizes may be larger than actual size.  In that event, the actual
 *          data may not be contiguous in the array "data"
 *          User sets maxsizes before call.
 *          It is not necessary to call DFNDgetdata first if the dimensions
 *          are correct
 *---------------------------------------------------------------------------*/

#ifdef PROTOTYPE
int DFNDgetdata(NDGID id, intn rank, int32 maxsizes[], void *data)
#else
int DFNDgetdata(id, rank, maxsizes, data)
NDGID id;
intn rank;
int32 maxsizes[];
void *data;
#endif /* PROTOTYPE */
{
    return(DFNDIgetdata(id, rank, maxsizes, data, 0));    /* 0 == C */
}
                
/*-----------------------------------------------------------------------------
 * Name:    DFNDsetlengths
 * Purpose: Set maximum length for label, unit, format, coordsys strings
 * Inputs:  id: id of open HDF file
 *          maxlen_label, maxlen_format, maxlen_unit, maxlen_coordsys:
 *              maximum length of each string.
 * Globals: Maxstrlen
 * Returns: 0 on success, FAIL on error with error set
 * Users:   HDF users, utilities, other routines
 * Invokes: none
 * Method:  Stores values in global Maxstrlen
 * Remarks: The get routines assume the array passed in by user is of this len
 *          If this routine is not called, the lengths default to DFS_MAXLEN
 *          The length includes the string terminator NULL byte
 *---------------------------------------------------------------------------*/
#ifdef PROTOTYPE
int DFNDsetlengths(NDGID id, int maxlen_label, int maxlen_unit,
                   int maxlen_format, int maxlen_coordsys)
#else
int DFNDsetlengths(id, maxlen_label, maxlen_unit, maxlen_format,
                   maxlen_coordsys)
NDGID id;
int maxlen_label, maxlen_unit, maxlen_format, maxlen_coordsys;
#endif /* PROTOTYPE */
{
    char *FUNC = "DFNDsetlengths";

    if(!VALIDNDGID(id)) {
        HERROR(DFE_BADCALL);  return FAIL;
    }

    return(DFNDIsetlengths(NDGID2SLOT(id), maxlen_label, maxlen_unit,
                           maxlen_format, maxlen_coordsys, 0));
}

#ifdef PROTOTYPE
int DFNDIsetlengths(int slot, int maxlen_label, int maxlen_unit,
                   int maxlen_format, int maxlen_coordsys, int flags)
#else
int DFNDIsetlengths(slot, maxlen_label, maxlen_unit, maxlen_format,
                   maxlen_coordsys, flags)
int slot, flags;
int maxlen_label, maxlen_unit, maxlen_format, maxlen_coordsys;
#endif /* PROTOTYPE */
{
    NDFILEREC *rec;

    if(flags & DFSDCHECKSET)
        if(DFNDIinitsd())
            return FAIL;
    rec = NDGSLOT2REC(slot);  /* after init sd, or it may not be there yet */

    if (maxlen_label > 0) rec->context->Maxstrlen[LABEL] = maxlen_label;
    if (maxlen_unit > 0) rec->context->Maxstrlen[UNIT] = maxlen_unit;
    if (maxlen_format > 0) rec->context->Maxstrlen[FORMAT] = maxlen_format;
    if (maxlen_coordsys > 0) rec->context->Maxstrlen[COORDSYS] = maxlen_coordsys;
    return(0);
}
    
/*-----------------------------------------------------------------------------
 * Name:    DFNDsetdims
 * Purpose: Set rank and sizes for subsequent SDGs
 * Inputs:  id: id of open HDF file
 *          rank: rank of array that holds the raw data
 *          dimsizes: sizes of all of the dimensions
 * Globals: Writesdg, Ref
 * Returns: 0 on success, FAIL on error with error set
 * Users:   HDF users, utilities, other routines
 * Invokes: DFNDIsetdims
 * Method:  Stores values in global structure Writesdg
 * Remarks: If dimensions change, all previous "set"s are cleared
 *          This routine must be called before DFNDsetdimstrs and
 *          DFNDsetdimscales.  It need not be called if these routines are
 *          not called, and the correct dimensions are supplied to DFNDadddata
 *---------------------------------------------------------------------------*/
#ifdef PROTOTYPE
int DFNDsetdims(NDGID id, intn rank, int32 dimsizes[])
#else
int DFNDsetdims(id, rank, dimsizes)
NDGID id, dimsizes[];
intn rank;
#endif /* PROTOTYPE */
{
    char *FUNC = "DFNDsetdims";

    HEclear();

    if (!VALIDNDGID(id)) {
        HERROR(DFE_BADCALL); return FAIL;
    }
    return(DFNDIsetdims(NDGID2SLOT(id), rank, dimsizes, 0));
}

#ifdef PROTOTYPE
int DFNDIsetdims(int slot, intn rank, int32 dimsizes[], int flags)
#else
int DFNDIsetdims(slot, rank, dimsizes, flags)
int slot, flags;
intn rank;
int32  dimsizes[];
#endif /* PROTOTYPE */
{
    int i;
    NDFILEREC *rec;
    char *FUNC = "DFNDIsetdims";

    HEclear();

    if (Sfile_id != DF_NOFILE) { 
        HERROR(DFE_BADCALL); return FAIL;
    }

    if(flags & DFSDCHECKSET)
        if(DFNDIinitsd())
            return FAIL;
    rec = NDGSLOT2REC(slot);  /* after init sd, or it may not be there yet */

    if (rec->context->Writesdg.rank == rank)	/* check if dimensions same */
        if (rec->context->Writesdg.dimsizes) {
            for (i = 0; i < rank; i++)
                if (rec->context->Writesdg.dimsizes[i] != dimsizes[i]) break;
            if (i == rank) return(0); /* Dimensions same as before */
        }   
    
    /* forget all attributes set previously */
    if (DFNDIclear(slot, &rec->context->Writesdg) < 0) return FAIL;

    /* allocate dimensions */
    rec->context->Writesdg.dimsizes = (int32 *)
                          HDgetspace((uint32)(rank * sizeof(int32)));
    if (rec->context->Writesdg.dimsizes == NULL) return FAIL;

    /* copy dimensions */
    rec->context->Writesdg.rank = rank;
    for (i=0; i<rank; i++)
        rec->context->Writesdg.dimsizes[i] = dimsizes[i];

    /* Note dimensions modified */
    rec->context->Ref.dims = 0;
    return(0);
}

/*-----------------------------------------------------------------------------
 * Name:    DFNDsetdatastrs
 * Purpose: Set label, unit and format for displaying subsequent SDGs
 * Inputs:  id: id of open HDF file
 *          label: label to be used to describe data
 *          unit: unit corresponding to data values
 *          format: format to be used in displaying data values
 *          coordsys: type of coordinate system
 * Globals: Writesdg, Ref
 * Returns: 0 on success, FAIL on failure with error set
 * Users:   HDF users, utilities, other routines
 * Invokes: none
 * Method:  Stores values in global structure Writesdg
 * Remarks: should we validate coordsys? proposed strings: "cartesian",
 *          "polar" (="spherical") and "cylindrical".  Do "spherical" and
 *          "cylindrical" make sense for 2D?
 *---------------------------------------------------------------------------*/
#ifdef PROTOTYPE
int DFNDsetdatastrs(NDGID id, char *label, char *unit, char *format,
                    char *coordsys)
#else
int DFNDsetdatastrs(id, label, unit, format, coordsys)
NDGID id;
char *label, *unit, *format, *coordsys;
#endif /* PROTOTYPE */
{
    char *FUNC = "DFNDsetdatastrs";

    HEclear();

    if (!VALIDNDGID(id)) {
        HERROR(DFE_BADCALL); return FAIL;
    }

    return(DFNDIsetdatastrs(NDGID2SLOT(id), label, unit, format, coordsys, 0));
}

#ifdef PROTOTYPE
int DFNDIsetdatastrs(int slot, char *label, char *unit, char *format,
                    char *coordsys, int flags)
#else
int DFNDIsetdatastrs(slot, label, unit, format, coordsys, flags)
int slot, flags;
char *label, *unit, *format, *coordsys;
#endif /* PROTOTYPE */
{
    NDFILEREC *rec;
    int luf;	/* takes values LABEL, UNIT, FORMAT in succession */
    char *lufp;			/* points to label, unit, format */
				/* in succession */

/* NOTE: The following code should be changed to write all three, even if
         one or more is an empty string.  Then, when DFNDgetdatastrs is called
         values will be returned for all three also, even though some might
         be empty strings.
*/

    if(flags & DFSDCHECKSET)
        if(DFNDIinitsd())
            return FAIL;
    rec = NDGSLOT2REC(slot);  /* after init sd, or it may not be there yet */

    for (luf = LABEL; luf <= FORMAT; luf++) {
	/* set lufp to point to label etc. as apppropriate */
        lufp = (luf == LABEL) ? label : (luf == UNIT) ? unit : format;

	/* free space if allocated */
        rec->context->Writesdg.dataluf[luf] =
          HDfreespace(rec->context->Writesdg.dataluf[luf]);

	/* copy string */
        if (lufp) {
            rec->context->Writesdg.dataluf[luf] = HDgetspace((uint32)DFIstrlen(lufp)+1);
            if (rec->context->Writesdg.dataluf[luf] == NULL) return FAIL;
            DFIstrcpy(rec->context->Writesdg.dataluf[luf], lufp);
        }
    }

    rec->context->Writesdg.coordsys =
        HDfreespace(rec->context->Writesdg.coordsys);

    if (coordsys) {
        rec->context->Writesdg.coordsys = HDgetspace((uint32) DFIstrlen(coordsys)+1);
        if (rec->context->Writesdg.coordsys == NULL) return FAIL;
        DFIstrcpy(rec->context->Writesdg.coordsys, coordsys);
    }

    /* indicate that label, unit, format and coordsys info modified */
    rec->context->Ref.luf[LABEL] = rec->context->Ref.luf[UNIT] = rec->context->Ref.luf[FORMAT] =
        rec->context->Ref.coordsys = 0;

    return(0);
}


/*-----------------------------------------------------------------------------
 * Name:    DFNDsetdimstrs
 * Purpose: For the given dimension, set label, unit, format
 *          This routine needs to be called once for each dimension whose 
 *          values the user wants to set.
 * Inputs:  id: id of open HDF file
 *          dim: the dimension that this info applies to
 *          label: label to be used to describe this dimension
 *          unit: units for dimension
 *          format: format to be used in displaying
 * Globals: Writesdg, Ref
 * Returns: 0 on success, FAIL on failure with error set
 * Users:   HDF users, utilities, other routines
 * Invokes: none
 * Method:  Stores values in global structure Writesdg
 *---------------------------------------------------------------------------*/

#ifdef PROTOTYPE
int DFNDsetdimstrs(NDGID id, int dim, char *label, char *unit, char *format)
#else
int DFNDsetdimstrs(id, dim, label, unit, format)
NDGID id;
int dim;
char *label, *unit, *format;
#endif /* PROTOTYPE */
{
    char *FUNC = "DFNDsetdimstrs";

    HEclear();

    if (!VALIDNDGID(id)) {
        HERROR(DFE_BADCALL); return FAIL;
    }

    return(DFNDIsetdimstrs(NDGID2SLOT(id), dim, label, unit, format, 0));
}

#ifdef PROTOTYPE
int DFNDIsetdimstrs(int slot, int dim, char *label, char *unit, char *format,
                    int flags)
#else
int DFNDIsetdimstrs(slot, dim, label, unit, format, flags)
int slot, flags;
int dim;
char *label, *unit, *format;
#endif /* PROTOTYPE */
{
    NDFILEREC *rec;
    int i, rdim, luf;		/* takes values LABEL, UNIT, FORMAT */
				/* in succession */
    char *lufp,			/* points to label, unit, format */
				/* in succession */
         *FUNC = "DFNDIsetdimstrs";
    HEclear();

    if(flags & DFSDCHECKSET)
        if(DFNDIinitsd())
            return FAIL;
    rec = NDGSLOT2REC(slot);  /* after init sd, or it may not be there yet */

    rdim = dim - 1;		/* translate from 1 to 0 origin */

    if ((rdim >= rec->context->Writesdg.rank) || (rdim < 0)) {
        HERROR(DFE_BADDIM); return FAIL;
    }

    for (luf = LABEL; luf <= FORMAT; luf++) {
	/* set lufp to point to label etc. as apppropriate */
        lufp = (luf == LABEL) ? label : (luf == UNIT) ? unit : format;

	/* allocate space if necessary */
        if(!rec->context->Writesdg.dimluf[luf]) {
            rec->context->Writesdg.dimluf[luf] = (char **)
                 HDgetspace((uint32) rec->context->Writesdg.rank * sizeof(char *));
            if (rec->context->Writesdg.dimluf[luf] == NULL) return FAIL;
            /* set allocated pointers to NULL*/
            for(i = 0; i < rec->context->Writesdg.rank; i++)
                rec->context->Writesdg.dimluf[luf][i] = NULL;
        }

	/* free string space if allocated */
        rec->context->Writesdg.dimluf[luf][rdim] =
            HDfreespace(rec->context->Writesdg.dimluf[luf][rdim]);

/* NOTE: The following code should be changed to write all three, even if
         one or more is an empty string.  Then, when DFNDgetdimstrs is called
         values will be returned for all three also, even though some might
         be empty strings.
*/

	/* copy string */
        if (lufp) {
            rec->context->Writesdg.dimluf[luf][rdim] =
                      HDgetspace((uint32) DFIstrlen(lufp) + 1);
            if (rec->context->Writesdg.dimluf[luf][rdim] == NULL) return FAIL;
            DFIstrcpy(rec->context->Writesdg.dimluf[luf][rdim], lufp);
        }
    }
    /* Indicate that this info has not been written to file */
    rec->context->Ref.luf[LABEL] = rec->context->Ref.luf[UNIT] = rec->context->Ref.luf[FORMAT] = 0;

    return(0);
}


/*-----------------------------------------------------------------------------
 * Name:    DFNDsetdimscale
 * Purpose: For the given dimension, set scale values
 *          This routine needs to be called once for each dimension whose 
 *          values the user wants to set.
 * Inputs:  id: id of open HDF file
 *          dim: the dimension that this info applies to
 *          dimsize: number of points in the scale
 *          scale: array of numbers that will make up the scale
 * Globals: Writesdg, Ref
 * Returns: 0 on success, FAIL on failure with error set
 * Users:   HDF users, utilities, other routines
 * Invokes: none
 * Method:  Stores values in global structure Writesdg
 *---------------------------------------------------------------------------*/
#ifdef PROTOTYPE
int DFNDsetdimscale(NDGID id, int dim, int32 dimsize, void *scale)
#else
int DFNDsetdimscale(id, dim, dimsize, scale)
NDGID id, dimsize;
int dim;
void *scale;
#endif /* PROTOTYPE */
{
    char *FUNC = "DFNDsetdimscale";

    HEclear();

    if (!VALIDNDGID(id)) {
        HERROR(DFE_BADCALL); return FAIL;
    }
    return(DFNDIsetdimscale(NDGID2SLOT(id), dim, dimsize, scale, 0));
}

#ifdef PROTOTYPE
int DFNDIsetdimscale(int slot, int dim, int32 dimsize, void *scale, int flags)
#else
int DFNDIsetdimscale(slot, dim, dimsize, scale, flags)
int slot, flags;
int dim;
int32   dimsize;
void *scale;
#endif /* PROTOTYPE */
{
    int rdim;
    int32 i, numtype, bytesize, localNTsize;
    NDFILEREC *rec;
    uint8 *p1, *p2;
    char *FUNC = "DFNDIsetdimscale";

    HEclear();

    if(flags & DFSDCHECKSET)
        if(DFNDIinitsd())
            return FAIL;
    rec = NDGSLOT2REC(slot);  /* after init sd, or it may not be there yet */

    rdim = dim - 1;		/* translate from 1 to 0 origin */

    if (!rec->context->Writesdg.dimsizes) {
        HERROR(DFE_BADCALL); return FAIL;
    }
    if (rec->context->Writesdg.numbertype == DFNT_NONE)
	if (DFNDIsetNT(slot, DFNT_FLOAT32, 0) < 0) return FAIL;
    numtype = rec->context->Writesdg.numbertype;
    localNTsize = DFKNTsize(numtype | DFNT_NATIVE);

    if ((rdim >= rec->context->Writesdg.rank) || (rdim < 0) /* check dimensions */
	|| (dimsize != rec->context->Writesdg.dimsizes[rdim])) {
        HERROR(DFE_BADDIM); return FAIL;
    }

    if (!scale) {		/* No scale for this dimension */
        if(rec->context->Writesdg.dimscales)
            rec->context->Writesdg.dimscales[rdim] =
		(uint8 *) HDfreespace((char*)rec->context->Writesdg.dimscales[rdim]);
        rec->context->Ref.scales = 0;
        return(0);
    }
    /* get number type and size of this type in this machine  */
    if (rec->context->Writesdg.numbertype == DFNT_NONE)
	if (DFNDIsetNT(slot, DFNT_FLOAT32, 0) <0) return FAIL;
    numtype = rec->context->Writesdg.numbertype;
    localNTsize = DFKNTsize(numtype | DFNT_NATIVE);
    bytesize = dimsize * localNTsize;
        
    /* allocate space for dimscales if necessary */
    if (!rec->context->Writesdg.dimscales) {
        rec->context->Writesdg.dimscales =
        (uint8 **) HDgetspace((uint32)rec->context->Writesdg.rank
                              * sizeof(int8 *));
        if (rec->context->Writesdg.dimscales == NULL) return FAIL;
        /* set allocated pointers to NULL */
        for (i = 0; i < rec->context->Writesdg.rank; i++)
            rec->context->Writesdg.dimscales[i] = NULL;
    }

    if (!rec->context->Writesdg.dimscales[rdim]) {
	/* allocate dimension scale space if necessary */
        rec->context->Writesdg.dimscales[rdim] =
                  (uint8 *) HDgetspace((uint32) bytesize);
        if (rec->context->Writesdg.dimscales[rdim] == NULL) return FAIL;
    }

             /* copy scale */
    p1 = (uint8 *)scale;
    p2 = (uint8 *)rec->context->Writesdg.dimscales[rdim];
    if (HDmemcopy(p1, p2, bytesize) != 0)
        return FAIL;

    /* Indicate scales modified */
    rec->context->Ref.scales = 0;

    return(0);
}

/*-----------------------------------------------------------------------------
 * Name:    DFNDsetrange
 * Purpose: Set maximum and minimum data values
 * Inputs:  id: id of open HDF file
 *          maxi: maximum value
 *          mini: minimum value
 * Globals: Ref
 * Returns: 0 on success, -1 if no maxmin values or if error, with error set
 * Users:   HDF users, utilities, other routines
 * Invokes: none
 * Method:  Modify Writesdg, set Ref
 * Remarks: Automatically cleared after call to DFNDadddata
 *---------------------------------------------------------------------------*/
#ifdef PROTOTYPE
int DFNDsetrange(NDGID id, void *maxi, void *mini)
#else
int DFNDsetrange(id, maxi, mini)
NDGID id;
void *maxi, *mini;
#endif /* PROTOTYPE */
{
    char *FUNC = "DFNDsetrange";

    HEclear();

    if (!VALIDNDGID(id)) {
        HERROR(DFE_BADCALL); return FAIL;
    }
    return(DFNDIsetrange(NDGID2SLOT(id), maxi, mini, 0));
}

#ifdef PROTOTYPE
int DFNDIsetrange(int slot, void *maxi, void *mini, int flags)
#else
int DFNDIsetrange(slot, maxi, mini, flags)
int slot, flags;
void *maxi, *mini;
#endif /* PROTOTYPE */
{
    int32 numtype, localNTsize;
    int i;
    NDFILEREC *rec;
    uint8 *p1, *p2;

    HEclear();

    if(flags & DFSDCHECKSET)
        if(DFNDIinitsd())
            return FAIL;
    rec = NDGSLOT2REC(slot);  /* after init sd, or it may not be there yet */

    p1 = &(rec->context->Writesdg.max_min[0]);
    for (i=0; i<16; i++)
        *p1++ = 0;       /* clear max_min   */

    /* get number type and copy the values to Writesdg   */
    if (rec->context->Writesdg.numbertype == DFNT_NONE)
        DFNDIsetNT(slot, DFNT_FLOAT32, 0);

    numtype = rec->context->Writesdg.numbertype;
    localNTsize = DFKNTsize(numtype | DFNT_NATIVE);
    p1 = (uint8 *)maxi;
    p2 = (uint8 *)mini;
    
    if (HDmemcopy(p1, (uint8 *) &(rec->context->Writesdg.max_min[0]),
                  localNTsize) == FAIL) 
        return FAIL;
    if (HDmemcopy(p2, (uint8 *)&(rec->context->Writesdg.max_min[localNTsize]),
                                                        localNTsize) == FAIL) 
        return FAIL;

    rec->context->Ref.maxmin = 0;

    return(0);
}

/*-----------------------------------------------------------------------------
 * Name:    DFNDadddata
 * Purpose: Calls DFNDIputslice to append data and SDG to file
 * Inputs:  fileid: id of open HDF to use
 *          rank: rank of data array
 *          dimsizes: sizes of the dimensions of data array
 *          data: array that holds data
 * Globals: Writesdg, Ref
 * Returns: 0 on success, FAIL on failure with error set
 * Users:   HDF users, utilities, other routines
 * Invokes: DFNDsetdims, DFNDstartslice, DFNDIputslice, DFNDIendslice
 * Method:  Invoke DFNDIputdata
 *---------------------------------------------------------------------------*/

#ifdef PROTOTYPE
int DFNDadddata(NDGID id, intn rank, int32 dimsizes[], void *data)
#else
int DFNDadddata(id, rank, dimsizes, data)
NDGID id;
intn rank;
int32 dimsizes[];
void *data;
#endif /* PROTOTYPE */
{
    /* 0 specifies C style array (row major) */
    return(DFNDIputdata(id, rank, dimsizes, data, 0));
}

/*-----------------------------------------------------------------------------
 * Name:    DFNDIrestart
 * Purpose: Do not remember info about file - get again from first data set
 * Inputs:  fileid: id of open HDF file
 * Returns: 0 on success
 * Users:   HDF programmers
 * Remarks: Subsequent gets will starts from first image
 *          Next put will write all "set" info to file
 *---------------------------------------------------------------------------*/
#ifdef PROTOTYPE
int DFNDrestart(NDGID id)
#else
int DFNDrestart(id)
NDGID id;
#endif /* PROTOTYPE */
{
    char *FUNC = "DFNDrestart";

    HEclear();

    if (!VALIDNDGID(id)) {
        HERROR(DFE_BADCALL); return FAIL;
    }

    return(DFNDIrestart(NDGID2SLOT(id), 0));
}

#ifdef PROTOTYPE
int DFNDIrestart(int slot, int flags)
#else
int DFNDIrestart(slot, flags)
int slot, flags;
#endif /* PROTOTYPE */
{
    NDFILEREC *rec = NDGSLOT2REC(slot);

    if(flags & DFSDCALL)
        Lastfile[0] = '\0';
    rec->context->Readref = 0;
    return(0);
}
    

/*-----------------------------------------------------------------------------
 * Name:    DFNDnumber
 * Purpose: Return number of NSDGs in file
 * Inputs:  fileid: id of open HDF file
 * Globals: nsdghdr
 * Returns: number of NSDGs on success, FAIL on error with error set
 * Users:   HDF users, utilities, other routines
 * Invokes: none
 * Method:  read nsdghdr->size
 * Remarks: none
 *---------------------------------------------------------------------------*/

#ifdef PROTOTYPE
int32 DFNDnumber(NDGID id)
#else
int32 DFNDnumber(id)
NDGID id;
#endif /* PROTOTYPE */
{
    int32 nsdgs = 0;
    char *FUNC = "DFNDnumber";

    HEclear();

    if (!VALIDNDGID(id)) {
        HERROR(DFE_BADCALL); return FAIL;
    }

    return(ndrec[NDGID2SLOT(id)]->file->nsdghdr->size);
}


/*-----------------------------------------------------------------------------
 * Name:    DFNDclear
 * Purpose: Clear all "set" values
 * Inputs:  id: id of open HDF file
 * Globals: Writesdg, Ref
 * Returns: 0 on success, FAIL on error with error set
 * Users:   HDF users, utilities, other routines
 * Invokes: DFNDIclear
 * Method:  Invoke DFNDIstartclear
 * Remarks: none
 *---------------------------------------------------------------------------*/
#ifdef PROTOTYPE
int DFNDclear(NDGID id)
#else
int DFNDclear(id)
NDGID id;
#endif /* PROTOTYPE */
{
    char *FUNC = "DFNDclear";

    if (!VALIDNDGID(id)) {
        HERROR(DFE_BADCALL); return FAIL;
    }
    return(DFNDIstartclear(NDGID2SLOT(id)));
}

/*-----------------------------------------------------------------------------
 * Name:    DFNDIstartclear
 * Users:   DFNDclear, DFSDclear
 * Invokes: DFNDIclear, DFNDIclearNT
 *---------------------------------------------------------------------------*/
#ifdef PROTOTYPE
int DFNDIstartclear(int slot)
#else
int DFNDIstartclear(slot)
int slot;
#endif /* PROTOTYPE */
{
    NDFILEREC *rec = NDGSLOT2REC(slot);

    rec->context->lastnsdg.tag = DFTAG_NULL;
    rec->context->lastnsdg.ref = 0;
    if(DFNDIclearNT(slot, &rec->context->Writesdg) < 0)
        return FAIL;
    if(DFNDIclear(slot, &rec->context->Writesdg) < 0)
        return FAIL;
    return(0);
}

/*-----------------------------------------------------------------------------
 * Name:    DFNDlastref
 * Purpose: Return last ref written or read
 * Inputs:  id: id of open HDF file
 * Globals: Lastref
 * Returns: ref on success, FAIL on error with error set
 * Users:   HDF users, utilities, other routines
 * Invokes: none
 * Method:  return Lastref
 * Remarks: none
 *---------------------------------------------------------------------------*/

#ifdef PROTOTYPE
int DFNDlastref(NDGID id)
#else
int DFNDlastref(id)
NDGID id;
#endif /* PROTOTYPE */
{
    char *FUNC = "DFNDlastref";

    if (!VALIDNDGID(id)) {
        HERROR(DFE_BADCALL); return FAIL;
    }

    return(DFNDIlastref(NDGID2SLOT(id)));
}

#ifdef PROTOTYPE
int DFNDIlastref(int slot)
#else
int DFNDIlastref(slot)
int slot;
#endif /* PROTOTYPE */
{
  return((int) ndrec[slot]->context->Lastref);
}

/*-----------------------------------------------------------------------------
 * Name:    DFNDreadref
 * Purpose: Set ref of SDS to get next
 * Inputs:  id: id of open file to which this applies
 *          ref: reference number of next get
 * Returns: 0 on success, FAIL on failure
 * Users:   HDF programmers, other routines and utilities
 * Invokes: DFIfind
 * Remarks: checks if image with this ref exists
 *---------------------------------------------------------------------------*/

#ifdef PROTOTYPE
int DFNDreadref(NDGID id, uint16 ref)
#else
int DFNDreadref(id, ref)
    NDGID id;
    uint16 ref;
#endif /* PROTOTYPE */
{
    NDFILEREC *rec = NDGID2REC(id);
    char *FUNC = "DFNDreadref";
    int32 aid;

    HEclear();

    if (!VALIDNDGID(id)) {
        HERROR(DFE_BADCALL); return FAIL;
    }

/* MMM Is this DFIfind needed now.  I.e. should we put an Hstartread
   here, or just have the Hstartread invoked later where the 
   actual read is performed?  The latter seems cleaner.
*/
/*
    if (DFIfind(id, DFTAG_SDG, ref,1,0,0, &dlep, &cdd) < 0) {
        Hclose(rec->file->Hid); return FAIL;
    }
*/

    if ((aid = Hstartread(rec->file->Hid, DFTAG_SDG, ref)) == FAIL
     && (aid=Hstartread(rec->file->Hid, DFTAG_NDG, ref)) == FAIL)      {
        HERROR(DFE_NOMATCH);
        return(HDerr(rec->file->Hid));
    }

    Hendaccess(aid);
    rec->context->Readref = ref;
    rec->context->Newdata = -1;
    return(0);
}

/*-----------------------------------------------------------------------------
 * Name:    DFNDgetslice
 * Purpose: Get slice of data from SDG.  Will sequence to next SDG if
 *          DFNDgetdims, DFNDgetdata or DFNDgetslice not called earlier.
 * Inputs:  filename: name of HDF file to use
 *          winst: array of size = rank of data, containing start of slice
 *          windims: array of size rank, containing end of slice
 *          data: array for returning slice
 *          dims: dimensions of array data
 * Returns: 0 on success, FAIL on failure with error set
 * Outputs: slice of data in data
 * Users:   DFNDIgetdata
 * Invokes: DFNDIgetslice
 * Method:  call DFNDIgetslice
 * Remarks: dims may be larger than size of slice.  In that event, the actual
 *          data may not be contiguous in the array "data".
 *          User sets dims before call.
 *---------------------------------------------------------------------------*/

#ifdef PROTOTYPE
int DFNDgetslice(NDGID id, int32 winst[], int32 windims[], void *data,
		 int32 dims[])
#else
int DFNDgetslice(id, winst, windims, data, dims)
NDGID id;
int32 winst[], windims[];
int32 dims[];
void *data;
#endif /* PROTOTYPE */
{
    return(DFNDIgetslice(id, winst, windims, data, dims, 0));
}

/*-----------------------------------------------------------------------------
 * Name:    DFNDstartslice
 * Purpose: Set up to write slice of data to SDG.
 * Inputs:  id: id of open HDF file to write to
 * Returns: 0 on success, FAIL on failure with error set
 * Users:   DFNDadddata
 * Invokes: DFnewref, DFaccess
 * Method:  call DFNDIputslice
 * Remarks: DFNDsetdims must have been called first
 *          No call which needs a file open may be made after this
 *          till DFNDendslice is called
 *---------------------------------------------------------------------------*/
#ifdef PROTOTYPE
int DFNDstartslice(NDGID id)
#else
int DFNDstartslice(id)
NDGID id;
#endif /* PROTOTYPE */
{
    int32 i, size;
    NDFILEREC *rec = NDGID2REC(id);
    char *FUNC = "DFNDstartslice";

    HEclear();

    if (!VALIDNDGID(id)) {
        HERROR(DFE_BADCALL); return FAIL;
    }

    if(Sfile_id != DF_NOFILE)  {  /* already in midst of slice write */
        HERROR(DFE_BADCALL);  return FAIL;
    }

    if (!rec->context->Writesdg.rank) {	/* dimensions not set */
        HERROR(DFE_BADDIM); return FAIL;
    }

    Sfile_id = id;

    if (!rec->context->Writeref)
        rec->context->Writeref = Hnewref(rec->file->Hid);
    if (!rec->context->Writeref) return FAIL;

    rec->context->Writesdg.data.tag = DFTAG_SD;
    rec->context->Writesdg.data.ref = rec->context->Writeref;

    /* if NT not set, default to float32 */
    if (rec->context->Writesdg.numbertype == DFNT_NONE)
        DFNDsetNT(id, DFNT_FLOAT32);

        /* set up to write data */
    size = DFKNTsize(rec->context->Writesdg.numbertype);
    for (i=0; i<rec->context->Writesdg.rank; i++)
        size *= rec->context->Writesdg.dimsizes[i];

    rec->context->Writesdg.aid = Hstartwrite(rec->file->Hid, DFTAG_SD,
                                             rec->context->Writeref, size);
    if (rec->context->Writesdg.aid == FAIL) {
        return FAIL;
    }

    /* allocate array for keeping track of dims written */
    Sddims = (int32 *) HDgetspace((uint32) rec->context->Writesdg.rank * sizeof(int32));
    if (Sddims == NULL) {
        return FAIL;
    }
    
    for (i=0; i < rec->context->Writesdg.rank; i++)
        Sddims[i] = 0;		/* nothing written so far */

    return(0);
}


/*-----------------------------------------------------------------------------
 * Name:    DFNDputslice
 * Purpose: Put slice of data to SDG.
 * Inputs:  winend: array of size rank, containing end of slice
 *          data: array containing slice
 *          dims: dimensions of array data
 * Returns: 0 on success, FAIL on failure with error set
 * Users:   DFNDadddata
 * Invokes: DFNDIputslice
 * Method:  call DFNDIputslice
 * Remarks: dims may be larger than size of slice.  In that event, the actual
 *          data may not be contiguous in the array "data".
 *          DFNDstartslice must have been called first
 *---------------------------------------------------------------------------*/

#ifdef PROTOTYPE
int DFNDputslice(NDGID id, int32 winend[], void *data, int32 dims[])
#else
int DFNDputslice(id, winend, data, dims)
NDGID id, winend[];
void *data;
int32 dims[];
#endif /* PROTOTYPE */
{

    return(DFNDIputslice(id, winend, data, dims, 0));
}


/*-----------------------------------------------------------------------------
 * Name:    DFNDendslice
 * Purpose: Write of data to SDG completed, write SDG and close file
 * Inputs:  id: id of open HDF file
 * Returns: 0 on success, FAIL on failure with error set
 * Users:   
 * Invokes: DFNDIendslice 
 * Method:  call DFNDIendslice
 *---------------------------------------------------------------------------*/

#ifdef PROTOTYPE
int DFNDendslice(NDGID id)
#else
int DFNDendslice(id)
NDGID id;
#endif /* PROTOTYPE */
{
    return(DFNDIendslice(id, 0));
}


/*---------------------------------------------------------------------
 *
 * Name: 	DFNDsetNT
 * Purpose:	Set number type for writing out to the hdf file
 * Inputs:	id: id of open HDF file
 *              numbertype
 * Return: 	0 on success, FAIL on failure with error set
 * Outputs:	none
 * Method: 	Set Writesdg.numbertype to numbertype 
 *              Calls DFKsetNT, which keeps current NT and also initializes
 *		conversion routines. 
 * Remarks:
 *--------------------------------------------------------------------- */
#ifdef PROTOTYPE
int DFNDsetNT(NDGID id, int32 numbertype)
#else
int DFNDsetNT(id, numbertype)
NDGID id, numbertype;
#endif /* PROTOTYPE */
{
    char *FUNC = "DFNDsetNT";

    HEclear();

    if (!VALIDNDGID(id)) {
        HERROR(DFE_BADCALL); return FAIL;
    }

    return(DFNDIsetNT(NDGID2SLOT(id), numbertype, 0));
}

#ifdef PROTOTYPE
int DFNDIsetNT(int slot, int32 numbertype, int flags)
#else
int DFNDIsetNT(slot, numbertype, flags)
int slot, flags;
int32 numbertype;
#endif /* PROTOTYPE */
{
    uint8 outNT;
    NDFILEREC *rec;

    if(flags & DFSDCHECKSET)
        if(DFNDIinitsd())
            return FAIL;
    rec = NDGSLOT2REC(slot);  /* after init sd, or it may not be there yet */

    outNT = (int8)(DFKisnativeNT(numbertype)? DFKgetPNSC(numbertype, DF_MT) :
				DFNTF_HDFDEFAULT);
    if ((numbertype == rec->context->Writesdg.numbertype) &&
            (outNT == rec->context->Writesdg.filenumsubclass))
        return(0);

	/* Forget previous numbertype  */
    if (DFNDIclearNT(slot, &rec->context->Writesdg) < 0) return FAIL;
    rec->context->Writesdg.numbertype = numbertype;
    rec->context->Writesdg.filenumsubclass = outNT;
    rec->context->Ref.nt = 0;
    rec->context->Ref.dims =
        (rec->context->Ref.dims >= 0 ? 0 : rec->context->Ref.dims);
    return(DFKsetNT(numbertype));
}

/*-------------------------------------------------------------------
* Name: 	DFNDIclearNT
* Purpose:	Reset all "set" values related to number types
* Inputs;	id: id of file we are working on - needed for Ref[]
*               sdg: pointer to sdg struct to clear
* Globals:	Ref
* Returns:	0 on success, FAIL on error with error set
* Users:	DFNDsetNT, HDF users
* Invokes:	none
* Remarks:
*--------------------------------------------------------------------*/

#ifdef PROTOTYPE
int DFNDIclearNT(int slot, DFSsdg *sdg)
#else
int DFNDIclearNT(slot, sdg)
int slot;
DFSsdg *sdg;
#endif
{
    int i;
    NDFILEREC *rec = NDGSLOT2REC(slot);

    HEclear();
    sdg->numbertype = DFNT_NONE;
    sdg->filenumsubclass = DFNTF_NONE;

    /* free scale pointers. Note: scale pointer array is not freed   */
    /* sdg->dimscales will be freed only when rank is changed        */
    if (sdg->dimscales)
        for (i = 0; i < sdg->rank; i++)
            sdg->dimscales[i] = (uint8 *)
                HDfreespace((char *) sdg->dimscales[i]);

    rec->context->Ref.nt = -1;
    rec->context->Ref.maxmin = -1;    /* maxmin and scales should be changed to */
    rec->context->Ref.scales = -1;    /* new number type              */
    return(0);
}

/*--------------------------------------------------------------------
* Name:	DFNDgetNT
* Purpose:	Get number type to be read from the hdf file
* Inputs: 	id: id of file
*               pnumbertype: pointer to int32 to return number type in
* Return:	0 on success, FAIL on error with DFerror set 
* Method:	return numbertype in Readsdg
*------------------------------------------------------------------- */

#ifdef PROTOTYPE
int DFNDgetNT(NDGID id, int32 *pnumbertype)
#else
int DFNDgetNT(id, pnumbertype)
NDGID id, *pnumbertype;
#endif
{
    char *FUNC="DFNDgetNT";

    HEclear();
    if (!VALIDNDGID(id)) {
        HERROR(DFE_BADCALL); return FAIL;
    }

    return(DFNDIgetNT(NDGID2SLOT(id), pnumbertype));
}

#ifdef PROTOTYPE
int DFNDIgetNT(int slot, int32 *pnumbertype)
#else
int DFNDIgetNT(slot, pnumbertype)
int slot;
int32 *pnumbertype;
#endif /* PROTOTYPE */
{
    NDFILEREC *rec = NDGSLOT2REC(slot);
    char *FUNC = "DFNDIgetNT";

    *(pnumbertype) = rec->context->Readsdg.numbertype;
    if (*(pnumbertype) == DFNT_NONE) 	{
        HERROR(DFE_BADNUMTYPE); return FAIL;
    }
    return(0);
}
    
/******************************************************************************/
/*--------------------- Lower level routines --------------------------------*/
/******************************************************************************/

/* Functions for NDG and SDG stuff 			 */

/*--------------------------------------------------------------------------
 * Name:    DFNDIsetnsdg_t
 * Purpose: Set up the NDG/SDG table. Each node has two
 *          fields: the 1st field is NDG or SDG, the 2nd
 *          field has value only when it is a special
 *          NDG, i.e. the data set is float32 and not
 *   	    compressed.
 * Inputs:  id: id of HDF file containing SDG
 * Returns: 0 on success, FAIL on failure with error set
 * Users:   DFNDopen for READ, DFNDIgetdata
 *--------------------------------------------------------------------------*/
#ifdef PROTOTYPE
int DFNDIsetnsdg_t(NDGID id, DFnsdg_t_hdr *nsdghdr)
#else
int DFNDIsetnsdg_t(id, nsdghdr)
NDGID id;
DFnsdg_t_hdr *nsdghdr;
#endif
{
    uint32 sz_DFnsdgle = (uint32)sizeof(struct DFnsdgle);
    int32 aid,         /* access id */
          ndgs, sdgs;	/* number of ndg's and sdg's */
    uint16 intag, inref;
    bool moretags, found;
    DFnsdgle *ntb, *stb, *new, *nf, *nr, *sf, *sr;
    DFdi di, lnkdd[2];
    uint8 *bufp;
    NDFILEREC *rec = NDGID2REC(id);
    char *FUNC="DFNDsetnsdg_t";
  
    if (!VALIDNDGID(id)) {
        HERROR(DFE_BADCALL); return FAIL;
    }

/* MMM:  Talk to Shiming and make sure the change made to the way ndgs
         and sdgs are handled is ok.
*/
    ndgs = Hnumber(rec->file->Hid, DFTAG_NDG);
    sdgs = Hnumber(rec->file->Hid, DFTAG_SDG);
    if ( (ndgs==FAIL) || (sdgs==FAIL) )
        return FAIL;
    
    if ( (ndgs+sdgs) == 0 ) {       /* no sdgs or ndgs in file */
        nsdghdr->size = 0;
        nsdghdr->nsdg_t = NULL;
        return(0);
    }
    if ( (ntb = (DFnsdgle *)HDgetspace(sz_DFnsdgle) ) == NULL)   {
        HERROR(DFE_NOSPACE); return FAIL;
    }		
    /* the first node in each table is a dummy node  */
    ntb->nsdg.tag = DFTAG_NULL;	/* set up and init an ndg table  */
    ntb->nsdg.ref = 0;
    ntb->sdg.tag = DFTAG_NULL;
    ntb->sdg.ref = 0;
    ntb->next = NULL;
    if ((stb = (DFnsdgle *)HDgetspace(sz_DFnsdgle)) == NULL)   {
        HERROR(DFE_NOSPACE); return FAIL;
    }
    stb->nsdg.tag = DFTAG_NULL;	/* set up and init an sdg table  */
    stb->nsdg.ref = 0;
    stb->sdg.tag = DFTAG_NULL; /* this field should be named as */
    stb->sdg.ref = 0;          /* stb->ndg.tag, the ndg to which this */
    stb->next = NULL;		/* sdg belongs. 		*/

    aid = Hstartread(rec->file->Hid, DFTAG_WILDCARD, DFREF_WILDCARD);
    moretags = (aid != FAIL);
    while (moretags)	{     /* read dd's and put each dd in ntb or stb */
        HQuerytagref(aid, &intag, &inref);
        	/* put NDG or SDG on ntb or stb	*/
	if (intag == DFTAG_NDG)	{
	    nr = ntb;
	    nf = ntb;
	    while ((inref>nf->nsdg.ref) && (nf->next!=NULL))   {
	       nr = nf;
	       nf = nf->next;
	    }
/* MMM:  Talk to Shiming and make sure the way this part was rearranged is ok.
*/
	    if (inref == nf->nsdg.ref)	{       /* check for duplicate nsdg */
            HERROR(DFE_BADNDG); return FAIL;
	    }

            /* add a node to the table */
            if ((new = (DFnsdgle *)HDgetspace(sz_DFnsdgle)) == NULL) {
                HERROR(DFE_NOSPACE); return FAIL;
            }
            new->nsdg.tag = DFTAG_NDG;
            new->nsdg.ref = inref;
            new->sdg.tag = DFTAG_NULL;
            new->sdg.ref = 0;

            if (inref < nf->nsdg.ref) {  /* does it go before current node? */
                new->next = nf;
                nr->next = new;
            } else {                     /* or at the end? */
                new->next = nf->next;
                nf->next = new;
            }

	    /* Does this NDG have an SDG? 	*/
	    if (DFdiread(rec->file->Hid, DFTAG_NDG, inref) < 0)
            return FAIL;
	    found = 0;
	    di.tag = DFTAG_NULL;
	    di.ref = 0;
	    while ((found == 0) && (DFdiget(&di.tag, &di.ref) == 0))  {
 		if (di.tag == DFTAG_SDLNK)
			found = 1;
	    }
	    if (found)	{    /* read in the tag/refs in the link element */
            if ((int32)FAIL == Hgetelement(rec->file->Hid, di.tag, di.ref, DFtbuf) )
                return FAIL;
            bufp = DFtbuf;
            UINT16DECODE(bufp, lnkdd[0].tag);
            UINT16DECODE(bufp, lnkdd[0].ref);
            UINT16DECODE(bufp, lnkdd[1].tag);
            UINT16DECODE(bufp, lnkdd[1].ref);
            new->sdg.tag = lnkdd[1].tag;
            new->sdg.ref = lnkdd[1].ref;
	    }
	}	/* end of NDG    */
	if (intag == DFTAG_SDG)	{
	    sr = stb;
	    sf = stb;
	    while ((inref>sf->nsdg.ref) && (sf->next!=NULL))   {
	       sr = sf;
	       sf = sf->next;
	    }
	    if (inref == sf->nsdg.ref)	{
            HERROR(DFE_BADNDG); return FAIL;
	    }
            /* insert a new node */
	    if ((new = (DFnsdgle *)HDgetspace(sz_DFnsdgle)) == NULL) {
	        HERROR(DFE_NOSPACE); return FAIL;
	    }
	    new->nsdg.tag = DFTAG_SDG;
	    new->nsdg.ref = inref;
            new->sdg.tag = DFTAG_NULL;
	    new->sdg.ref = 0;

	    if (inref < sf->nsdg.ref)  {  /* does it go before current node? */
            new->next = sf;
            sr->next = new;
	    } else {                      /* or at the end? */
            new->next = sf->next;
            sf->next = new;
	    }
	    /* Does it belong to  an NDG?    */
	    if (DFdiread(rec->file->Hid, DFTAG_SDG, inref) < 0) return FAIL;
	    found = 0;
        di.tag = DFTAG_NULL;
	    di.ref = 0;        	
        while ((found == 0) && (DFdiget(&di.tag, &di.ref) == 0)) {
            if (di.tag == DFTAG_SDLNK)
                found = 1;
        }
	    if (found)	{   /* read in the tag/refs in the link element */
            if ((int32)FAIL == Hgetelement(rec->file->Hid,  di.tag, di.ref, DFtbuf))
                return FAIL;
            bufp = DFtbuf;
            UINT16DECODE(bufp, lnkdd[0].tag);
            UINT16DECODE(bufp, lnkdd[0].ref);
            UINT16DECODE(bufp, lnkdd[1].tag);
            UINT16DECODE(bufp, lnkdd[1].ref);
            new->sdg.tag = lnkdd[0].tag;
            new->sdg.ref = lnkdd[0].ref;
	    }
	}	/* end of SDG    */

	/*   get next dd   */
        moretags = ( SUCCEED == 
                  Hnextread(aid, DFTAG_WILDCARD, DFREF_WILDCARD, DF_CURRENT) );
    }	/* gone through the dd blocks   */
    Hendaccess(aid);

    /* merge stb and ntb		*/
    	/* remove SDGNDG from stb       */
    nf = ntb->next;
    while (nf != NULL)	{
        inref = nf->sdg.ref;
        if (inref != 0) {   /* it has an SDG   */
            sr = stb;
            sf = stb;
            while ((sf->nsdg.ref < inref) && (sf->next != NULL))  {
                sr = sf;
                sf = sf->next;
            }
            if (sf->nsdg.ref == inref)      {
                if (sf->sdg.ref != nf->nsdg.ref)    {
                    HERROR(DFE_BADNDG); return FAIL;
                }
                else    {
                    sr->next = sf->next;
                    if ((sf = (DFnsdgle *)HDfreespace((char *) sf)) != NULL)
                    return FAIL;
                    sdgs--;
                }
            }
        }
        nf = nf->next;
    }
	/* check all SDGNDGs were removed   */
    sf = stb->next;
    while (sf != NULL)	{
        if (sf->sdg.ref != 0)   {
            HERROR(DFE_BADSDG); return FAIL;
        }
        sf = sf->next;
    }
    	/* merge the two tables into one */
    nf = ntb;			/* looking for the end of ntb   */
    while (nf->next != NULL) 
        nf = nf->next;
    nf->next = stb->next;	/* the first node in stb is a dummy */
    nsdghdr->size = ndgs + sdgs;
    nsdghdr->nsdg_t = ntb->next;
    /* Release the first nodes in stb and ntb  */
    if ((stb=(DFnsdgle *)HDfreespace((char *)stb)) != NULL)
        return FAIL;
    if ((ntb=(DFnsdgle *)HDfreespace((char *)ntb)) != NULL)
        return FAIL;
    return(0);
}   /* end of DFNDsdtnsdg_t   */


/*-----------------------------------------------------------------------------
 * Name	DFNDInextnsdg
 * Purpose: Returns next ndg or sdg in the file
 * Inputs:  id: id of the file in question
 *          nsdghdr: point to the nsdg table 
 *	   nsdg: the structure holds the di of next sdg or ndg
 * Returns: 0 on succeeds, FAIL on failure
 *---------------------------------------------------------------------------*/
#ifdef PROTOTYPE
int DFNDInextnsdg(NDGID id, DFnsdg_t_hdr *nsdghdr, DFdi *nsdg)
#else
int DFNDInextnsdg(id, nsdghdr, nsdg)
NDGID id;
DFnsdg_t_hdr *nsdghdr;
DFdi *nsdg;
#endif /* PROTOTYPE*/
{
    uint32 num;
    int16 found=0;
    NDFILEREC *rec = NDGID2REC(id);
    DFnsdgle *ptr;
    char *FUNC="DFNDInextnsdg";
    
    nsdg->tag= DFTAG_NULL;
    nsdg->ref = (uint16) 0;
    ptr = nsdghdr->nsdg_t;
    num = nsdghdr->size;

    if ((ptr == NULL)||(num == 0)) 	{
        return(0); 	/* empty table  */
    }
    if ((rec->context->lastnsdg.tag == DFTAG_NULL) &&
        (rec->context->lastnsdg.ref == 0)) {
        found = 1;
    }
    else {
        while ((num>0) && (ptr !=NULL) && (found == 0))	{
            if ((ptr->nsdg.tag == rec->context->lastnsdg.tag) && 
    	                (ptr->nsdg.ref == rec->context->lastnsdg.ref))  {
    	        if ((ptr = ptr->next) != NULL)    found = 1;
            }
            else 	{
    	        ptr=ptr->next;
                num--;
    	    }
        }	/* while  */
        if (((num == 0)&&(ptr!=NULL)) || ((num!=0) && (ptr == NULL))
                                                  || (found == 0))  {
            HERROR(DFE_BADTABLE); return FAIL;
        }
    }	/* else   */
    if (found)	{
        nsdg->tag = ptr->nsdg.tag;
        nsdg->ref = ptr->nsdg.ref;
    }
    return(0);
}   /* end of DFNDInextnsdg   */

/*-----------------------------------------------------------------------------
 * Name:    DFNDIgetndg
 * Purpose: Reads in NDG
 * Inputs:  id: pointer to HDF file containing NDG
 *          ref: ref of NDG to read
 *          sdg: pointer to DFSsdg struct to read NDG into
 * Returns: 0 on success, FAIL on failure with error set
 * Users:   HDF programmers, DFNDgetdims, DFNDgetdata
 * Invokes: DFgetelement, DFdiread, DFdiget, DFaccess, DFread
	    DFNDgetsdg
 * Method:  Reads in NDG using DFdiread.  Gets each tag/ref using DFdiget.
 *          Reads in dimensions using DFgetelement.
 * 	    Call DFNDgetsdg to read in the rest info.
 *          Mallocs space for these, freeing
 *          previously allocated space.
 * Remarks: This accepts non-float32 data
 *---------------------------------------------------------------------------*/

#ifdef PROTOTYPE
int DFNDIgetndg(NDGID id, uint16 ref, DFSsdg *sdg)
#else
int DFNDIgetndg(id, ref, sdg)
NDGID id;
uint16 ref;
DFSsdg *sdg;
#endif /* PROTOTYPE */
{
    int32 i;
    int luf, slot = NDGID2SLOT(id);
    NDFILEREC *rec = NDGID2REC(id);
    int32 length;
    DFdi elmt, nt;
    uint8 ntstring[4];
    int32 numtype,	/* current number type */
          fileNTsize,	/* size of this NT as it is in the file */
	  localNTsize,	/* size of this NT as it is in this machine */
          ret,
          aid;
    int8  fileNT,	/* file number subclass */
	  platnumsubclass;   /* platform number subclass */
    uint8 *isscales,
          *buf, 
          *p;           /* temporary pointer for moving things to buffer */
    char *FUNC="DFNDIgetndg";

    HEclear();

    if (!VALIDNDGID(id)) {  
        HERROR(DFE_BADCALL); return FAIL; 
    }
    if (!ref)  { 
        HERROR(DFE_BADREF); return FAIL; 
    }
    if (DFdiread(rec->file->Hid, DFTAG_NDG, ref)<0) /* read NDG into memory */
        return FAIL;

    DFNDIclear(slot, sdg);
    DFNDIclearNT(slot, sdg);
    rec->context->Ismaxmin = 0;

    while (!DFdiget(&elmt.tag, &elmt.ref)) {  /* get next tag/ref from NDG */
        luf = -1;		              /* flag value for label/unit/ */
        switch (elmt.tag) {	              /* format gets process tag/ref */

            case DFTAG_SD:	/* data tag/ref */
                    sdg->data.tag = elmt.tag; /* put tag/ref in struct */
                    sdg->data.ref = elmt.ref;
                break;

            case DFTAG_SDD:	/* dimension */
                aid = Hstartread(rec->file->Hid, elmt.tag, elmt.ref);
                if (aid == FAIL)
                    return FAIL;

		/* read rank */
                if (Hread(aid, (int32) 2, DFtbuf) == FAIL) return FAIL;
                p = DFtbuf;
                INT16DECODE(p, sdg->rank);

		/* get space for dimensions */
                sdg->dimsizes = (int32 *) HDgetspace((uint32) sdg->rank *
						      sizeof(int32));
                if (sdg->dimsizes == NULL) return FAIL;

		/* read dimension record */
                if (Hread(aid, (int32) 4*sdg->rank,DFtbuf) ==FAIL) return FAIL;
                p = DFtbuf;
                for (i=0; i<sdg->rank; i++)
                    INT32DECODE(p, sdg->dimsizes[i]);

                            /* read tag/ref of NT */
                if (Hread(aid,(int32) 4,  DFtbuf) == FAIL) return FAIL;
                p = DFtbuf;
                UINT16DECODE(p, nt.tag);
                UINT16DECODE(p, nt.ref);
                
                            /* read actual NT */
                if (Hgetelement(rec->file->Hid, nt.tag, nt.ref, ntstring) == FAIL)
                    return FAIL;

               /* BBB: following should be changed to check for any valid NT */
                if (ntstring[1] == DFNT_NONE) {
                    HERROR(DFE_BADCALL); return FAIL;
                }

                            /* set NT info */
                numtype = ntstring[1];
                fileNT = ntstring[3];
                platnumsubclass = DFKgetPNSC(numtype, DF_MT);
                if ((fileNT != DFNTF_HDFDEFAULT) &&
                    (fileNT != platnumsubclass))    {
                    HERROR(DFE_BADCALL); return FAIL;
                }
                if (fileNT != DFNTF_HDFDEFAULT) /* if native */
                    numtype = numtype | DFNT_NATIVE;
                        sdg->filenumsubclass = ntstring[3];
                sdg->numbertype = numtype;
                    /* set size of NT    */
                fileNTsize = DFKNTsize(numtype);
                localNTsize = DFKNTsize(numtype | DFNT_NATIVE);

                        /* read and check all scale NTs */
                for (i=0; i<sdg->rank; i++) {
                    if (Hread(aid, (int32) 4, DFtbuf) == FAIL) return FAIL;
                    p = DFtbuf;
                    UINT16DECODE(p, nt.tag);
                    UINT16DECODE(p, nt.ref);
                    
                            /* read NT itself */
                    if (Hgetelement(rec->file->Hid, nt.tag,nt.ref, ntstring) == FAIL)
                        return FAIL;

                    /* BBB: see above comment about same four lines */
                    if (ntstring[1] == DFNT_NONE) {
                        HERROR(DFE_BADCALL); return FAIL;
                    	}
                    }
                Hendaccess(aid);
	        break;

            case DFTAG_SDLNK:     /* SDG NDG link */
            break;      /* do nothing in 3.2  */

            case DFTAG_SDL:     /* labels */
                if (luf==(-1)) luf = LABEL;

            case DFTAG_SDU:     /* units */
                if (luf==(-1)) luf = UNIT;

            case DFTAG_SDF:     /* formats */
                if (luf==(-1)) luf = FORMAT;

                if (!sdg->dimsizes) {           /* internal error */
                    HERROR(DFE_CORRUPT); return FAIL;
                }

                        /* get needed size of buffer, allocate */
                length =  Hlength(rec->file->Hid, elmt.tag, elmt.ref);
                if (length == FAIL) return FAIL;
                buf = HDgetspace((uint32) length);
                if (buf == NULL) return FAIL;

                        /* read in luf */
                if (Hgetelement(rec->file->Hid, elmt.tag, elmt.ref, buf) == FAIL) {
                    buf = HDfreespace(buf);
                    return FAIL;
                }
                p = buf;
                        /* allocate data luf space */
                sdg->dataluf[luf] = HDgetspace((uint32) DFIstrlen((char*)p)+1);

                if (sdg->dataluf[luf] == NULL) {
                    buf = HDfreespace(buf);
                    return FAIL;
                }
                       /* extract data luf */
                DFIstrcpy(sdg->dataluf[luf], (char*)p);
                p += DFIstrlen(sdg->dataluf[luf])+1;

		      /* get space for dimluf array */
                sdg->dimluf[luf] =
                    (char **) HDgetspace((uint32) sdg->rank * sizeof(char *));
                if (sdg->dimluf[luf] == NULL) {
                    buf = HDfreespace(buf);
                    return FAIL;
                }

                      /* extract dimension lufs */
                for (i=0; i<sdg->rank; i++) {
                    sdg->dimluf[luf][i] = 
		      HDgetspace((uint32) DFIstrlen((char*)p)+1);
                    if (sdg->dimluf[luf][i] == NULL) {
                        buf = HDfreespace(buf);
                        return FAIL;
                    }
                    DFIstrcpy(sdg->dimluf[luf][i], (char*)p);
                    p += DFIstrlen(sdg->dimluf[luf][i])+1;
                }
                buf = HDfreespace(buf);
                break;

            case DFTAG_SDS:     /* scales */
                if (!sdg->dimsizes) {           /* internal error */
                    HERROR(DFE_CORRUPT); return FAIL;
                }

		/* set up to read scale */
                aid = Hstartread(rec->file->Hid, elmt.tag, elmt.ref);
                if (aid == FAIL)
                    return FAIL;

		/* read isscales */
                isscales = HDgetspace((uint32) sdg->rank);
                if (isscales == NULL) return FAIL;
                if (Hread(aid, (int32) sdg->rank, isscales) == FAIL) 
                    return FAIL;

		/* allocate scale pointers */
                sdg->dimscales =
                    (uint8 **) HDgetspace((uint32) sdg->rank * sizeof(int8 *));
                if (sdg->dimscales == NULL) {
                    isscales = HDfreespace(isscales);
                    return FAIL;
                }
                        /* read scales */
                for (i=0; i<sdg->rank; i++) {
                    sdg->dimscales[i] = NULL;       /* default */
                    if (!isscales[i]) continue;

                            /* space for scale */
                    sdg->dimscales[i] = (uint8 *)
                        HDgetspace((uint32) sdg->dimsizes[i] * localNTsize);
                    if (sdg->dimscales[i] == NULL) {
                        isscales = HDfreespace(isscales);
                        return FAIL;
                    }

                    if (platnumsubclass == fileNT) { /* no conversion needed */
                        ret = Hread(aid, (int32)sdg->dimsizes[i]*fileNTsize, 
                                                  (uint8 *) sdg->dimscales[i]);
                        if (ret == FAIL) { 
                            isscales = HDfreespace(isscales);
                            return FAIL;
                        }
                    }

                    else {                      /* conversion necessary */
                                /* allocate conversion buffer */
                        buf = HDgetspace((uint32)sdg->dimsizes[i] *
                                            fileNTsize);
                        if (buf == NULL) {
                            isscales = HDfreespace(isscales);
                            return FAIL;
                        }
                                /* read scale from file */
                        ret = Hread(aid, 
                                   (int32) (sdg->dimsizes[i]*fileNTsize), buf);
                        if (ret == FAIL) {
                            buf = HDfreespace(buf);
                            isscales = HDfreespace(isscales);
                            return FAIL;
                        }
                            
                        p = buf;
				/* convert, all at once */
                        DFKconvert(p, sdg->dimscales[i], numtype,
                                 sdg->dimsizes[i], DFACC_READ, 0, 0);

                        buf = HDfreespace(buf);
                    }
                }
                isscales = HDfreespace(isscales);
                Hendaccess(aid);
                break;

            case DFTAG_SDC:	/* coordsys */
		/* find and allocate necessary space */
                length =  Hlength(rec->file->Hid, elmt.tag, elmt.ref);
                if (length == FAIL) return FAIL;

                sdg->coordsys = HDgetspace((uint32) length);
                if (sdg->coordsys == NULL) return FAIL;

		/* read coordsys */
                if ( Hgetelement(rec->file->Hid, elmt.tag, elmt.ref, 
                                          (uint8 *) sdg->coordsys) == FAIL)
                    return FAIL;
                break;

            case DFTAG_SDM:	/* max/min */
                if (fileNT==platnumsubclass) {       /* no conversion */
                    if (Hgetelement(rec->file->Hid, elmt.tag, elmt.ref,
				     (uint8 *) &(sdg->max_min[0])) == FAIL)
                        return FAIL;
                }
                else {
                        /* allocate buffer */
#ifndef CVT_BUG
		    uint8 mm[16];
#endif
                    buf = HDgetspace((uint32) 2 * fileNTsize);
                    if (buf == NULL) return FAIL;

                        /* read and convert max/min */
                    if (Hgetelement(rec->file->Hid, elmt.tag, elmt.ref, buf) == FAIL)
                        return FAIL;
#ifdef CVT_BUG
                    DFKconvert(buf, &(sdg->max_min[0]), numtype, 1,
                            DFACC_READ, 0, 0);
                    DFKconvert(buf+fileNTsize, &(sdg->max_min[localNTsize]),
                            numtype, 1, DFACC_READ, 0, 0);
#else
                    DFKconvert(buf, mm, numtype, 2, DFACC_READ, 0, 0);
                    for (i=0; i<localNTsize; i++)
                        sdg->max_min[i] = mm[i];
                    for (i=0; i<localNTsize; i++)
                        sdg->max_min[i+localNTsize] = mm[i+localNTsize];
#endif
                    buf = HDfreespace(buf);
                }
                rec->context->Ismaxmin = 1;
                break;
                    
            case DFTAG_SDT:
                rec->context->FileTranspose = 1;
                break;
            default:            /* ignore unknown tags */
                break;
        }
    }
    return(0);
}


 /*---------------------------------------------------------------------------*
 * Name:    DFNDIputndg
 * Purpose: Write NDG out to HDF file
 * Inputs:  id: HDF file id
 *          ref: ref to put NDG with
 *          sdg: struct containing NDG info to put
 * Returns: 0 on success, FAIL on failure with error set
 * Users:   HDF programmers, utilities, other routines
 * Invokes: DFIcheck, DFdistart, DFdiadd, DFdiend, DFputelement, DFaccess,
 *          DFwrite
 * Remarks: Writes out NTs
 *---------------------------------------------------------------------------*/

#ifdef PROTOTYPE
int DFNDIputndg(NDGID id, uint16 ref, DFSsdg *sdg)
#else
int DFNDIputndg(id, ref, sdg)
NDGID id;
uint16 ref;
DFSsdg *sdg;
#endif /* PROTOTYPE */
{
    int32 i;
    int j, luf, issdg = 0;    /* issdg=1 if it is NDG SDG  */
    uint16 luftag;
    uint8 *buf, *Isscales=NULL, *bufp;
    uint8 ntstring[4],
	 platnumsubclass, 
	 outNT;	        /* file number type subclass	*/
    DFdi nt ;	
    int32 numtype,	/* current number type	*/
    	  localNTsize,  /* size of this NT on as it is on this machine */
          fileNTsize,	/* size of this NT as it will be in the file */
          scaleNTsize,	/* size of scale NT as it will be in the file */
          ret, aid;
    intn len;
    NDFILEREC *rec = NDGID2REC(id);
    char *FUNC="DFNDIputndg";
     
    HEclear();

    if (!VALIDNDGID(id)) {
        HERROR(DFE_BADCALL); return FAIL;
    }
    if (!ref)  {
        HERROR(DFE_BADREF); return FAIL;
    }

    /* set number type and subclass	*/
    if (sdg->numbertype == DFNT_NONE)
        DFNDsetNT(id, DFNT_FLOAT32);     /* default is float32  */
    numtype = sdg->numbertype;
    fileNTsize = DFKNTsize(numtype);
    scaleNTsize = fileNTsize;        /* for now, assume same. MAY CHANGE */
    outNT = sdg->filenumsubclass;
    localNTsize = DFKNTsize(numtype | DFNT_NATIVE);
    platnumsubclass = DFKgetPNSC(numtype, DF_MT);
    
    /* prepare to start writing ndg   */
    if (DFdisetup(10) < 0) return FAIL;

	/* put ND and ref       */
    if (DFdiput(sdg->data.tag, sdg->data.ref) < 0) return FAIL;

    if(rec->context->Ref.nt <= 0) {
        /* will not execute if has been written in putsdg */
            /* construct and write out NT */
        ntstring[0] = DFNT_VERSION;             /* version */
        ntstring[1] = (uint8)(numtype & 0xff);  /* type */
        ntstring[2] = (uint8)(fileNTsize*8); /* width of number type in bits */
        ntstring[3] = outNT;          /* class: IEEE or machine class */
        if (Hputelement(rec->file->Hid, DFTAG_NT, ref, ntstring, (int32) 4) == FAIL)
            return FAIL;
        rec->context->Ref.nt = ref;
    }

        /* write out NDD (dimension record) */
    /* new NDD; write rank, dims, data NT and scale NTs */
    if (rec->context->Ref.dims<=0) {
            
                /* put rank & dimensions in buffer */
        bufp = DFtbuf;     
        UINT16ENCODE(bufp, sdg->rank);
        for (i=0; i<sdg->rank; i++)
            INT32ENCODE(bufp, sdg->dimsizes[i]);

                /* put data NT and scale NTs  in buffer */
        nt.tag = DFTAG_NT;
        nt.ref = rec->context->Ref.nt;           /* same NT for scales too */

            /* "<=" used to put 1 data NT + rank scale NTs in buffer */
        for (i=0; i<=sdg->rank; i++) { /* scale NTs written even if no scale!*/
            UINT16ENCODE(bufp, nt.tag);
            UINT16ENCODE(bufp, nt.ref);
        }   
            /* write out NDD record */
        ret = Hputelement(rec->file->Hid, DFTAG_SDD, ref, DFtbuf,
                          (int32) (bufp-DFtbuf));
        if (ret == FAIL)
            return FAIL;
        rec->context->Ref.dims = ref;
    }
    /* write dimension record tag/ref */
        if(DFdiput(DFTAG_SDD, (uint16) rec->context->Ref.dims) < 0) return FAIL;
        
            /* write out label/unit/format */
    for (luf=LABEL; luf<=FORMAT; luf++) {
        luftag = (luf==LABEL) ? DFTAG_SDL : (luf==UNIT)
                                            ? DFTAG_SDU : DFTAG_SDF;
        bufp = DFtbuf;
            /* this block of code checks if luf is NULL, else writes it */
        if (!rec->context->Ref.luf[luf]) {            /* if luf was set */
            rec->context->Ref.luf[luf] = -1;          /* assume it is NULL */

                 /* if dataluf non-NULL, set up to write */
            if (sdg->dataluf[luf] && sdg->dataluf[luf][0]) { 
                len = DFIstrlen(sdg->dataluf[luf])+1;
                HDstrncpy( (char *)bufp, sdg->dataluf[luf], len);
                bufp += len;
            }	                               
            else {                       /* dataluf NULL */
                HDstrncpy( (char *)bufp, "", (int32) 1 );
                bufp ++;
            }

                /* for each dimluf, if non-NULL, set up to write */
            if (sdg->dimluf[luf]) {
                for (i=0; i<sdg->rank; i++) {
                    if ( sdg->dimluf[luf][i] &&     
                         sdg->dimluf[luf][i][0] ) {   /* dimluf not NULL */
                            len = DFIstrlen(sdg->dimluf[luf][i])+1;
                            HDstrncpy((char *)bufp, sdg->dimluf[luf][i], len);
                            bufp += len;
                    }
                    else {                        /* dimluf NULL */
                        HDstrncpy( (char *)bufp, "", (int32) 1 );
                        bufp ++;
                    }
                  /*  break; */  /* this shouldnt be here */
                }	/* i loop 	*/
            }	/* dimluf is non-NULL */
            rec->context->Ref.luf[luf] = ref; /* remember ref */
            ret = Hputelement(rec->file->Hid, luftag, 
                              (uint16) rec->context->Ref.luf[luf],
                              DFtbuf, (int32) (bufp-DFtbuf));
            if (ret == FAIL) return FAIL;
        }  /* luf was set */

	/* write luf tag/ref */
        if (rec->context->Ref.luf[luf]>0)
            if (DFdiput(luftag, (uint16) rec->context->Ref.luf[luf]) < 0)
                return FAIL;
    }	/* luf loop	*/

    /* check if there is a scale and write it out */
    if (!rec->context->Ref.scales) {		/* if scale set */
        Isscales = HDgetspace((uint32) sdg->rank);
        if (Isscales == NULL) return FAIL;
        rec->context->Ref.scales = (-1);        /* assume there is no scale */

                    /* set up Isscales array */
        for (i=0; i<sdg->rank; i++) {
            if (sdg->dimscales && sdg->dimscales[i]) {  /* a scale exists */
                Isscales[i] = 1;
                rec->context->Ref.scales = 0;    /* flag: write out scales */
            }
            else Isscales[i] = 0;
        }
    }

    if (!rec->context->Ref.scales) {      /* write out scales */
                /* compute space needed for scales */
        len = 0;
        for (i=0; i<sdg->rank; i++)  
            if (Isscales[i] == 1)
                len += sdg->dimsizes[i] * scaleNTsize;
        len += sdg->rank;

        aid = Hstartwrite(rec->file->Hid, DFTAG_SDS, ref, len);
        if (aid == FAIL ) {
            Isscales = HDfreespace(Isscales); 
            return FAIL;
        }
                            /* write Isscales */
        if (Hwrite(aid, (int32) sdg->rank, Isscales) == FAIL) {
            Isscales = HDfreespace(Isscales); 
            return FAIL;
        }
                            /* Write scales */
        for (j=0; j<sdg->rank; j++) {
            if (!Isscales[j]) continue;
            if (platnumsubclass ==outNT) {       /* no conversion needed */
                if (Hwrite(aid, (int32) (fileNTsize * sdg->dimsizes[j]),
                                   (uint8 *) sdg->dimscales[j]) == FAIL) {
                    Isscales = HDfreespace(Isscales);
                    return FAIL;
                }
            }
            else {              /* convert and write */
                        /* allocate buffer */
                buf = HDgetspace((uint32) (fileNTsize * sdg->dimsizes[j]));
                if (buf == NULL) {
                    Isscales = HDfreespace(Isscales); 
                    return FAIL;
                }
                        /* convert, all at once */
                DFKconvert(sdg->dimscales[j], buf, numtype,
			  sdg->dimsizes[j], DFACC_WRITE, 0, 0);
                        /* write it all out */
                if (Hwrite(aid, (int32) (fileNTsize * sdg->dimsizes[j]),
                                                              buf) == FAIL) {
                    Isscales = HDfreespace(Isscales);
                    buf = HDfreespace(buf);
                    return FAIL;
                }
                buf = HDfreespace(buf);
            }
        }
        rec->context->Ref.scales = ref;
        Hendaccess(aid);
    }
    Isscales = HDfreespace(Isscales);
    if (rec->context->Ref.scales>0)
        if (DFdiput(DFTAG_SDS, (uint16) rec->context->Ref.scales) < 0)
            return FAIL;

    /* write coordsys */
    if (!sdg->coordsys || !sdg->coordsys[0]) rec->context->Ref.coordsys = (-1);
    if (!rec->context->Ref.coordsys) {
        ret = Hputelement(rec->file->Hid, DFTAG_SDC, ref, (uint8 *)sdg->coordsys,
                                (int32) (DFIstrlen(sdg->coordsys)+1));
        if (ret == FAIL) return FAIL;
        rec->context->Ref.coordsys = ref;
    }
    if (rec->context->Ref.coordsys>0)
        if (DFdiput(DFTAG_SDC, (uint16) rec->context->Ref.coordsys) < 0)
	return FAIL;
        
    /* write max/min */
    if (!rec->context->Ref.maxmin) {
        if (platnumsubclass == outNT) {     /* no conversion */
            ret = Hputelement(rec->file->Hid, DFTAG_SDM, ref, 
                     (uint8 *) &(sdg->max_min[0]), (int32) (2 * fileNTsize));
            if (ret == FAIL) return FAIL;
            rec->context->Ref.maxmin = ref;
        }
        else {
	    /* allocate buffer */
#ifndef CVT_BUG
            uint8 mm[16];

            for (i=0; i<localNTsize; i++)
                mm[i] = sdg->max_min[i];
            for (i=0; i<localNTsize; i++)
                mm[localNTsize + i] = sdg->max_min[localNTsize + i];
#endif
            buf = HDgetspace((uint32) 2*fileNTsize); /* max/min is 8 bytes */
            if (buf == NULL) return FAIL;

	    /* convert */
#ifndef CVT_BUG
            DFKconvert(mm, buf, numtype, 2, DFACC_WRITE, 0, 0);
#else
            DFKconvert(&sdg->max_min, buf, numtype, 1, DFACC_WRITE, 0, 0);
            DFKconvert(&sdg->max_min + localNTsize, buf+outNTsize, 
			numtype, 1,DFACC_WRITE, 0, 0);
#endif
	    /* write */
            ret = Hputelement(rec->file->Hid, DFTAG_SDM, ref, buf,
	                  		     (int32) (2*fileNTsize));
            if (ret == FAIL) { 
                buf = HDfreespace(buf); return FAIL;
            }
            rec->context->Ref.maxmin = ref;
            buf = HDfreespace(buf);
        }
    }
    if (rec->context->Ref.maxmin>0)
        if (DFdiput(DFTAG_SDM, (uint16) rec->context->Ref.maxmin) < 0)
            return FAIL;
    /* max/min should be reset for each data set */
    rec->context->Ref.maxmin = (-1);

    if (!rec->context->Ref.transpose) { /* if transposed, add transpose tag */
        if (Hdupdd(rec->file->Hid, DFTAG_SDT, ref, DFTAG_SDD, ref) == FAIL)
            return FAIL;
        rec->context->Ref.transpose = ref;
    }
    if (rec->context->Ref.transpose > 0)
        if (DFdiput(DFTAG_SDT, (uint16) rec->context->Ref.transpose) < 0)
            return FAIL;

    if (numtype == DFNT_FLOAT32) {  /* if float32, add a DFTAG_SDLNK   */
        DFdi lnkdd[2];

        issdg = 1;
        lnkdd[0].tag=DFTAG_NDG;
        lnkdd[0].ref = ref;
        lnkdd[1].tag=DFTAG_SDG;
        lnkdd[1].ref = ref;
        bufp = DFtbuf;
        for (i=0; i<2; i++)	{
            UINT16ENCODE(bufp, lnkdd[i].tag);
            UINT16ENCODE(bufp, lnkdd[i].ref);
        }
        ret = Hputelement(rec->file->Hid, DFTAG_SDLNK, ref,
                                    DFtbuf,(int32) (bufp-DFtbuf));
        if (ret == FAIL) return FAIL;

	/* write DFTAG_SDLNK  */
        if (DFdiput(DFTAG_SDLNK, ref)<0) return FAIL;
    }
    
    /* write out NDG */
    if (DFdiwrite(rec->file->Hid, DFTAG_NDG, ref) < 0) return FAIL;
    /* write an SDG point to the dataset if it is an NDG SDG  */
    if (issdg)	{
        if (Hdupdd(rec->file->Hid, DFTAG_SDG, ref, DFTAG_NDG, ref) < 0) {
            Hclose(rec->file->Hid); return FAIL;
        }
    }
    return(0);
}

/*-----------------------------------------------------------------------------
 * Name:    DFNDIendslice
 * Purpose: Write of data to SDG completed, write SDG and close file
 * Inputs:  id: id of open HDF file
 *          isfortran: true if called from Fortran
 * Returns: 0 on success, FAIL on failure with error set
 * Users:   DFNDadddata
 * Invokes: DFNDputsdg, Hclose, HERROR
 * Method:  call DFNDputsdg
 * Remarks: checks that slice writes were completed.
 *---------------------------------------------------------------------------*/

#ifdef PROTOTYPE
int DFNDIendslice(NDGID id, int isfortran)
#else
int DFNDIendslice(id, isfortran)
NDGID id;
int isfortran;
#endif /* PROTOTYPE */
{
    int i, found;
    NDFILEREC *rec = NDGID2REC(id);
    DFnsdgle *new, *last, *cur;
    char *FUNC = "DFNDIendslice";

    HEclear();

    if (Sfile_id == DF_NOFILE) {
        HERROR(DFE_BADCALL); return FAIL;
    }

    /* check if slice writes complete */
    for (i=0; i < rec->context->Writesdg.rank; i++) {
        if (!rec->context->Fortorder && (i == 0) &&
            (Sddims[i] == rec->context->Writesdg.dimsizes[i]))  continue;
        if (  (isfortran || rec->context->Fortorder) &&
              (i == rec->context->Writesdg.rank - 1) &&
              (Sddims[i] == rec->context->Writesdg.dimsizes[i]))
            continue;
        if((isfortran || rec->context->Fortorder || i > 0) &&
           (!rec->context->Fortorder || i < rec->context->Writesdg.rank-1) &&
           (Sddims[i] == 0)) continue;
        HERROR(DFE_BADCALL);
        return(FAIL);
    }


    if (DFNDIputndg(id, rec->context->Writeref, &rec->context->Writesdg) < 0) {
        return FAIL;
    }

    Hendaccess(rec->context->Writesdg.aid);
    Sfile_id = DF_NOFILE;       /* partial write complete */
    Sddims = (int32 *) HDfreespace((char*) Sddims);

    /* update nsdg table */
    if((new = (DFnsdgle *) HDgetspace(sizeof(DFnsdgle))) == NULL) {
        HERROR(DFE_NOSPACE);  return FAIL;
    }
    new->nsdg.tag = DFTAG_NDG;
    new->nsdg.ref = rec->context->Writeref;
    if(rec->context->Writesdg.numbertype == DFNT_FLOAT32) {
        new->sdg.tag = DFTAG_SDG;
        new->sdg.ref = rec->context->Writeref;
    }
    else {
        new->sdg.tag = DFTAG_NULL;
        new->sdg.ref = (uint16) 0;
    }
    new->next = NULL;

    /* it is not possible to insert something in the front, and though it
       should never be necessary to do so, since ref numbers always go up,
       this is BAD */
    last = rec->file->nsdghdr->nsdg_t;
    if(last == NULL) {
        rec->file->nsdghdr->nsdg_t = new;
        rec->file->nsdghdr->size = 1;
    }
    else {
        for(cur = last->next; cur != NULL &&
               cur->nsdg.tag == DFTAG_NDG && cur->nsdg.ref < new->nsdg.ref;
            cur = cur->next, last = last->next)
            /*EMPTY*/;

        if(cur != NULL) /* see if we stopped b/c refs are equal */
            /* this really shouldn't happen, but just in case ... */
            if(new->nsdg.ref == cur->nsdg.ref) {
                HERROR(DFE_BADNDG); /* same error as DFNDIsetnsdg_t */
                rec->context->Lastref = rec->context->Writeref;
                rec->context->Writeref = 0;
                return FAIL;
            }

        /* now NDG goes after 'last' - use cur to search for SDG with ref */
        for(found = FALSE; cur != NULL && !found; cur = cur->next)
            if(cur->nsdg.tag == DFTAG_SDG && cur->nsdg.ref == new->sdg.ref)
                found = TRUE;

        if(!found) {
            /* now insert 'new' after 'last' */
            new->next = last->next;
            last->next = new;
            rec->file->nsdghdr->size++;
        }
    }
	
    rec->context->Lastref = rec->context->Writeref;  /* remember ref written */
    rec->context->Writeref = 0;		    /* don't know ref to write next */

    return(0);
}


/*-----------------------------------------------------------------------------
 * Name:    DFNDgetsdg
 * Purpose: Reads in SDG
 * Inputs:  id: id of HDF file containing SDG
 *          ref: ref of SDG to read
 *          sdg: pointer to DFSsdg struct to read SDG into
 * Returns: 0 on success, FAIL on failure with error set
 * Users:   HDF programmers, DFNDgetdims, DFNDgetdata
 * Invokes: DFgetelement, DFdiread, DFdiget, DFaccess, DFread
 * Method:  Reads in SDG using DFdiread.  Gets each tag/ref using DFdiget.
 *          Reads in dimensions, labels, units, formats, scales, coordinate
 *          system using DFgetelement.  Mallocs space for these, freeing
 *          previously allocated space.
 * Remarks: This is specific to floating point data
 *---------------------------------------------------------------------------*/

#ifdef PROTOTYPE
int DFNDgetsdg(NDGID id, uint16 ref, DFSsdg *sdg)
#else
int DFNDgetsdg(id, ref, sdg)
NDGID id;
uint16 ref;
DFSsdg *sdg;
#endif /* PROTOTYPE */
{
    int i, luf, slot = NDGID2SLOT(id);
    NDFILEREC *rec = NDGID2REC(id);
    int32 length, aid, ret;
    DFdi elmt, nt;
    uint8 *isscales;
    uint8 *buf, *p;
    uint8 ntstring[4];
    int userNT=DFNTF_HDFDEFAULT,
        fileNT=DFNTF_HDFDEFAULT,
        fileNTsize=4,
        numtype = DFNT_FLOAT32,
        localNTsize = 4;

    char *FUNC="DFNDIgetsdg";

    HEclear();

    if (!VALIDNDGID(id)) {
        HERROR(DFE_BADCALL); return FAIL;
    }
    if (!ref)  {
        HERROR(DFE_BADREF); return FAIL;
    }

    if (DFdiread(rec->file->Hid, DFTAG_SDG, ref)<0) /* read RIG into memory */
        return FAIL;

    userNT = (DF_MT>>8) & 0x0f;	/* get third nibble from right */

    DFNDIclear(slot, sdg);
    rec->context->Ismaxmin = 0;

    while (!DFdiget(&elmt.tag, &elmt.ref)) {  /* get next tag/ref from SDG */
        luf = -1;		              /* flag value for label/unit */
        switch (elmt.tag) {	              /* format gets process tag/ref */

            case DFTAG_SD:	/* data tag/ref */
                sdg->data.tag = elmt.tag; /* put tag/ref in struct */
                sdg->data.ref = elmt.ref;
                break;

            case DFTAG_SDD:	/* dimension */
                aid = Hstartread(rec->file->Hid, elmt.tag, elmt.ref);
                if (aid == FAIL)
                    return FAIL;

                /* read rank */
                if (Hread(aid, (int32) 2, DFtbuf) == FAIL) return FAIL;
                p = DFtbuf;
                INT16DECODE(p, sdg->rank);

                /* get space for dimensions */
                sdg->dimsizes = (int32 *) HDgetspace((uint32) sdg->rank *
                                                      sizeof(int32));
                if (sdg->dimsizes == NULL) return FAIL;

                /* read dimension record */
                if (Hread(aid, (int32) 4*sdg->rank,DFtbuf) ==FAIL) return FAIL;
                p = DFtbuf;
                for (i=0; i<sdg->rank; i++)
                    INT32DECODE(p, sdg->dimsizes[i]);

                            /* read tag/ref of NT */
                if (Hread(aid,(int32) 4,  DFtbuf) == FAIL) return FAIL;
                p = DFtbuf;
                UINT16DECODE(p, nt.tag);
                UINT16DECODE(p, nt.ref);

                            /* read actual NT */
                if (Hgetelement(rec->file->Hid, nt.tag, nt.ref, ntstring) == FAIL)
                    return FAIL;

                            /* check float data */
                if (ntstring[1] != DFNT_FLOAT) {
                    HERROR(DFE_BADCALL); return FAIL;
                }
                            /* set NT info */
                numtype = ntstring[1];
                fileNT = ntstring[3];
                if ((fileNT != DFNTF_HDFDEFAULT) &&
                    (fileNT != userNT)) {
                    HERROR(DFE_BADCALL); return FAIL;
                }
                if (fileNT != DFNTF_HDFDEFAULT) /* if native */
                    numtype = numtype | DFNT_NATIVE;
                sdg->filenumsubclass = ntstring[3];
                sdg->numbertype = numtype;
                    /* set size of NT    */
                fileNTsize = DFKNTsize(numtype);
                localNTsize = DFKNTsize(numtype | DFNT_NATIVE);

                        /* read and check all scale NTs */
                for (i=0; i<sdg->rank; i++) {
                    if (Hread(aid, (int32) 4, DFtbuf) == FAIL) return FAIL;
                    p = DFtbuf;
                    UINT16DECODE(p, nt.tag);
                    UINT16DECODE(p, nt.ref);

                            /* read NT itself */
                    if (Hgetelement(rec->file->Hid, nt.tag,nt.ref, ntstring) == FAIL)
                        return FAIL;

                            /* check float data */
                    if (ntstring[1] != DFNT_FLOAT) {
                        HERROR(DFE_BADCALL); return FAIL;
                    }
                }
                Hendaccess(aid);
                break;

            case DFTAG_SDL:     /* labels */
                if (luf==(-1)) luf = LABEL;

            case DFTAG_SDU:     /* units */
                if (luf==(-1)) luf = UNIT;

            case DFTAG_SDF:     /* formats */
                if (luf==(-1)) luf = FORMAT;

                if (!sdg->dimsizes) {           /* internal error */
                    HERROR(DFE_CORRUPT); return FAIL;
                }
                        /* get needed size of buffer, allocate */
                length =  Hlength(rec->file->Hid, elmt.tag, elmt.ref);
                if (length == FAIL) return FAIL;
                buf = HDgetspace((uint32) length);
                if (buf == NULL) return FAIL;

                        /* read in luf */
                if (Hgetelement(rec->file->Hid, elmt.tag, elmt.ref, buf) == FAIL) {
                    buf = HDfreespace(buf);
                    return FAIL;
                }
                p = buf;

                        /* allocate data luf space */
                sdg->dataluf[luf] = HDgetspace((uint32) DFIstrlen((char*)p)+1);

                if (sdg->dataluf[luf] == NULL) {
                    buf = HDfreespace(buf);
                    return FAIL;
                }
                     /* extract data luf */
                DFIstrcpy(sdg->dataluf[luf], (char*)p);
                p += DFIstrlen(sdg->dataluf[luf])+1;

		/* get space for dimluf array */
                sdg->dimluf[luf] =
                    (char **) HDgetspace((uint32) sdg->rank * sizeof(char *));
                if (sdg->dimluf[luf] == NULL) {
                    buf = HDfreespace(buf);
                    return FAIL;
                }
                     /* extract dimension lufs */
                for (i=0; i<sdg->rank; i++) {
                    sdg->dimluf[luf][i] = 
		      HDgetspace((uint32)DFIstrlen((char*)p)+1);
                    if (sdg->dimluf[luf][i] == NULL) {
                        buf = HDfreespace(buf);
                        return FAIL;
                    }
                    DFIstrcpy(sdg->dimluf[luf][i], (char*)p);
                    p += DFIstrlen(sdg->dimluf[luf][i])+1;
                }
                buf = HDfreespace(buf);
                break;

            case DFTAG_SDS:     /* scales */
                if (!sdg->dimsizes) {           /* internal error */
                    HERROR(DFE_CORRUPT); return FAIL;
                }
		/* set up to read scale */
                aid = Hstartread(rec->file->Hid, elmt.tag, elmt.ref);
                if (aid == FAIL)
                    return FAIL;

		/* read isscales */
                isscales = HDgetspace((uint32) sdg->rank);
                if (isscales == NULL) return FAIL;
                if (Hread(aid, (int32) sdg->rank, isscales) == FAIL)
                    return FAIL;

		/* allocate scale pointers */
                sdg->dimscales =
                    (uint8 **) HDgetspace((uint32) sdg->rank * sizeof(int8 *));
                if (sdg->dimscales == NULL) {
                    isscales = HDfreespace(isscales);
                    return FAIL;
                }

                /* read scales */
                for (i=0; i<sdg->rank; i++) {
                    sdg->dimscales[i] = NULL;       /* default */
                    if (!isscales[i]) continue;

                            /* space for scale */
                    sdg->dimscales[i] = (uint8 *)
                        HDgetspace((uint32) sdg->dimsizes[i] * localNTsize);
                    if (sdg->dimscales[i] == NULL) {
                        isscales = HDfreespace(isscales);
                        return FAIL;
                    }
                    if (userNT == fileNT) {     /* no conversion needed */
                        ret = Hread(aid, (int32)sdg->dimsizes[i]*fileNTsize,
                                                (uint8 *) sdg->dimscales[i]);
                        if (ret == FAIL) {
                            isscales = HDfreespace(isscales);
                            return FAIL;
                        }
                    } else {                      /* conversion necessary */
                                /* allocate conversion buffer */
                        buf = HDgetspace((uint32)sdg->dimsizes[i] *
                                            fileNTsize);
                        if (buf == NULL) {
                            isscales = HDfreespace(isscales);
                            return FAIL;
                        }

                                /* read scale from file */
                        ret = Hread(aid,
                                   (int32) (sdg->dimsizes[i]*fileNTsize), buf);
                        if (ret == FAIL) {
                            buf = HDfreespace(buf);
                            isscales = HDfreespace(isscales);
                            return FAIL;
                        }
                        p = buf;
				/* convert, all at once */
                        DFKconvert(p, (uint8 *) sdg->dimscales[i], 
				numtype, sdg->dimsizes[i], DFACC_READ, 0, 0);
                        buf = HDfreespace(buf);
                    }
                }
                isscales = HDfreespace(isscales);
                Hendaccess(aid);
                break;

            case DFTAG_SDC:	/* coordsys */
		/* find and allocate necessary space */
                length =  Hlength(rec->file->Hid, elmt.tag, elmt.ref);
                if (length == FAIL) return FAIL;

                sdg->coordsys = HDgetspace((uint32) length);
                if (sdg->coordsys == NULL) return FAIL;

		/* read coordsys */
                if ( Hgetelement(rec->file->Hid, elmt.tag, elmt.ref,
                                         (uint8 *)sdg->coordsys) == FAIL)
                    return FAIL;
                break;

            case DFTAG_SDM:	/* max/min */
                if (fileNT == userNT) {       /* no conversion */
                    if (Hgetelement(rec->file->Hid, elmt.tag, elmt.ref,
                                     (uint8 *) &(sdg->max_min[0])) == FAIL)
                        return FAIL;
                }
                else {	/* conversion */
#ifndef CVT_BUG
                    int8 mm[16];
#endif 
                        /* allocate buffer */
                    buf = HDgetspace((uint32) 2 * fileNTsize);
                    if (buf == NULL) return FAIL;
		    
                        /* read and convert max/min */
                    if (Hgetelement(rec->file->Hid, elmt.tag, elmt.ref, buf) == FAIL)
                        return FAIL;
#ifdef CVT_BUG
                    DFKconvert(buf, &sdg->max_min[0], numtype,
                        1, DFACC_READ, 0, 0);
                    DFKconvert(buf+fileNTsize, &sdg->max_min[localNTsize],
                        numtype, 1, DFACC_READ, 0, 0);
#else
                    DFKconvert(buf, (uint8*)mm, numtype, 2, DFACC_READ, 0, 0);
                    for (i = 0; i < localNTsize; i++)
                        sdg->max_min[i] = mm[i];
                    for (i = 0; i < localNTsize; i++)
                    sdg->max_min[i + localNTsize] = mm[i + localNTsize];
#endif
                    buf = HDfreespace(buf);
                }
                rec->context->Ismaxmin = 1;
                break;
                    
            case DFTAG_SDT:
                rec->context->FileTranspose = 1;
                break;
            default:            /* ignore unknown tags */
                if ((elmt.tag <= DFTAG_BREQ) && (elmt.tag >= DFTAG_EREQ)) {
                    /* is it a required tag for later versions?  */
                    HERROR(DFE_BADNDG); return FAIL;
                }
		break;
        }
    }
    return(0);
}

/*-----------------------------------------------------------------------------
 * Name:    DFNDopen
 * Purpose: open a file for reading/writing SDSs
 * Inputs:  filename: name of file to open
 *          access:   access mode
 *          num_dds:  number of DDs to put in a DD block; default is 32
 * Returns: NDGID reference on success, -1 (FAIL) on failure with error set
 * Users:   HDF users
 * Invokes: Hopen, DFNDIsetnsdg_t
 * Remarks: calls Hopen, then does some initialization
 *---------------------------------------------------------------------------*/

#ifdef PROTOTYPE
NDGID DFNDopen(char *filename, int access, int16 num_dds)
#else
NDGID DFNDopen(filename, access, num_dds)
char *filename;
int access;
int16 num_dds;
#endif
{
    return(DFNDIopen(filename, access, num_dds, 0));
}

#ifdef PROTOTYPE
NDGID DFNDIopen(char *filename, int access, int16 num_dds, int flags)
#else
NDGID DFNDIopen(filename, access, num_dds, flags)
char *filename;
int access, flags;
int16 num_dds;
#endif
{
    int32 Hid;
    NDGID NDGid;
    int slot, i;
    NDFILEREC *rec;
    char *FUNC = "DFNDopen";
    DFSsdg *p;
    Stat *q;

    if((Hid = (NDGID) Hopen(filename, access, num_dds)) == FAIL)
      {
        HERROR(DFE_BADOPEN);
        return(FAIL);
      }

    if((slot = DFNDIget_rec_slot(Hid)) == FAIL) { /* there are no slots */
        Hclose(Hid);  return FAIL;
    }

    NDGid = NDGSLOT2ID(slot);

    /* this is impossible for now, and eventually we may want to permit this */
    if (NDGid == Sfile_id) { /* in the middle of a partial write */
        HERROR(DFE_ALROPEN);
        return(FAIL);
    }

    if(flags & DFSDCALL) {
        ndrec[slot] = ndrec[MAX_FILE];
        ndrec[MAX_FILE] = NULL;
        if(!strcmp(Lastfile, "")) {
            if((Lastfile = (char *) HDgetspace(strlen(filename) + 1)) == NULL){
                Hclose(Hid);
                HERROR(DFE_NOSPACE);
                return FAIL;
            }
            *Lastfile = '\0';
        }
        if(strcmp(Lastfile, filename)) { /* strcmp != 0  =>  different file */
            if((Lastfile = (char *) HDfreespace(Lastfile)) != NULL) {
                Hclose(Hid);
                return FAIL;
            }
            if((Lastfile = (char *) HDgetspace(strlen(filename) + 1)) == NULL){
                Hclose(Hid);
                HERROR(DFE_NOSPACE);
                return FAIL;
            }
            flags |= DFSDBUILDNSDGT;
            HDstrncpy(Lastfile, filename, strlen(filename) + 1);
        }
    }

    if((rec = ndrec[slot]) == NULL) {
        if((rec = ndrec[slot] = HDgetspace(sizeof(NDFILEREC))) == NULL) {
            Hclose(Hid);
            HERROR(DFE_NOSPACE);
            return FAIL;
        }

        rec->file = (NDGFILE *) NULL;
        rec->context = (NDGCONTEXT *) NULL;

        for(i = 0; i < MAX_FILE; i++)
            if(ndrec[i] != NULL) /* if there is a file ... */
                if(i != slot) /* and we aren't in the empty slot we found ...*/
                    if(ndrec[i]->file->Hid == Hid) { /* same file as us */
#ifdef ALLOW_MULTIPLE_DFND_CONTEXTS_USE_THESE_LINES
                        rec->file = ndrec[i]->file;
                        ndrec[i]->file->num_accs++;
                        break;
#else /* not currently allowing multiple opens (contexts) to a file */
                        HDfreespace(ndrec[slot]);
                        HERROR(DFE_ALROPEN);
                        return FAIL;
#endif /* ALLOW_MULTIPLE_DFND_CONTEXTS_USE_THESE_LINES */
                    }
    }

    if(rec->file == NULL) { /* no other opens to this file */
        if((rec->file = (NDGFILE *) HDgetspace(sizeof(NDGFILE))) == NULL) {
            Hclose(Hid);  HERROR(DFE_NOSPACE);  return FAIL;
        }
        rec->file->Hid = Hid;
        rec->file->num_accs = 1;
        rec->file->nsdghdr = NULL;
    }

    if(rec->context == NULL) {
        /* user DFNDopen or user DFSD write with no DFSDset... */
        if((rec->context =
            (NDGCONTEXT *) HDgetspace(sizeof(NDGCONTEXT))) == NULL) {
            Hclose(Hid); HERROR(DFE_NOSPACE); return FAIL;
        }

        p = &rec->context->Readsdg;
        p->data.tag = (uint16) DFTAG_NULL;  p->data.ref = (uint16) 0;
        p->rank = (intn) 0;
        p->dimsizes = NULL;
        p->coordsys = NULL;
        for(i = 0; i < 3; i++) {
            p->dataluf[i] = NULL;
            p->dimluf[i] = NULL;
        }
        p->dimscales = NULL;
        for(i = 0; i < 16; i++) p->max_min[i] = (uint8) 0;
        p->numbertype = (int32) DFNT_NONE;
        p->filenumsubclass = DFNTF_NONE;
        p->aid = (int32) -1;
        p->compression = (int32) 0;
        p->isndg = 0;

        p = &rec->context->Writesdg;
        p->data.tag = (uint16) DFTAG_NULL;  p->data.ref = (uint16) 0;
        p->rank = (intn) 0;
        p->dimsizes = NULL;
        p->coordsys = NULL;
        for(i = 0; i < 3; i++) {
            p->dataluf[i] = NULL;
            p->dimluf[i] = NULL;
        }
        p->dimscales = NULL;
        for(i = 0; i < 16; i++) p->max_min[i] = (uint8) 0;
        p->numbertype = (int32) DFNT_NONE;
        p->filenumsubclass = DFNTF_NONE;
        p->aid = (int32) -1;
        p->compression = (int32) 0;
        p->isndg = 0;

        q = &rec->context->Ref;
        q->dims = q->nt = q->coordsys = q->scales = q->maxmin =
            q->transpose = -1;
        for(i = 0; i < 3; i++) q->luf[i] = -1;

        rec->context->Writeref = 0;
        rec->context->Readref = 0;
        rec->context->Lastref = 0;
        rec->context->Newdata = -1;
        rec->context->Nextsdg = 1;
        rec->context->Ismaxmin = 0;
        rec->context->FileTranspose = 0;
        rec->context->Fortorder = 0;

        for(i = 0; i < 4; i++)
            rec->context->Maxstrlen[i] = DFS_MAXLEN;

        rec->context->lastnsdg.tag = DFTAG_NULL;
        rec->context->lastnsdg.ref = 0;
    }

    if(rec->file->nsdghdr == NULL) {
        rec->file->nsdghdr = (DFnsdg_t_hdr*)HDgetspace(sizeof(DFnsdg_t_hdr));
        if(rec->file->nsdghdr == NULL) {
            Hclose(Hid);
            HERROR(DFE_NOSPACE);
            return FAIL;
        }
        rec->file->nsdghdr->size = 0;
        rec->file->nsdghdr->nsdg_t = NULL;
    }

    /*
     *  The nsdg table is always needed, for read or write (implied read).
     *  If it is non-null, then this is DFSDgetdata or DFSDgetslice opening
     *  the file, and we only want to rebuild the table if this is a different
     *  file than the last time.  If it is the same, the second and subsequent
     *  consecutive gets will already have this in place, and will fail if they
     *  rebuild this, since the read will start from the beginning of the file.
     *  If the name comparison above in if (flags & DFSDCALL) indicated that
     *  this is NOT the same file, then it will have set DFSDBUILDNSDGT on.
     */
    if(((rec->file->nsdghdr->nsdg_t == NULL)) || (flags & DFSDBUILDNSDGT))
      {
        if(DFNDIsetnsdg_t(NDGid, rec->file->nsdghdr) == FAIL)
          return FAIL;
        rec->context->lastnsdg.tag = DFTAG_NULL;
        rec->context->lastnsdg.ref = 0;
      }

    return NDGid;
}

/*-----------------------------------------------------------------------------
 * Name:    DFNDclose
 * Purpose: close a file after reading/writing SDSs
 * Inputs:  fileid: fileid returned by DFNDopen
 * Returns: 0 on success, -1 (FAIL) on failure with error set
 * Users:   HDF users
 * Invokes: Hclose
 * Remarks: 
 *---------------------------------------------------------------------------*/

#ifdef PROTOTYPE
int DFNDclose(NDGID id)
#else
int DFNDclose(id)
NDGID id;
#endif
{
    return(DFNDIclose(id, 0));
}

#ifdef PROTOTYPE
int DFNDIclose(NDGID id, int flags)
#else
int DFNDIclose(id, flags)
NDGID id;
int flags;
#endif
{
    int slot = NDGID2SLOT(id), ret = 0, i;
    NDFILEREC *rec = NDGID2REC(id);
    DFnsdg_t_hdr *q;
    DFnsdgle *cur, *new;

    if(flags & DFSDCALL) {
        /* called from DFSD interface - copy info to defined location */
        ndrec[MAX_FILE] = ndrec[slot];
        ndrec[slot] = NULL;
    }
    else {
        /* remove this context */
        if((rec->context = (NDGCONTEXT *) HDfreespace(rec->context)) != NULL)
            ret = FAIL;

        /* Take away one file access.  If no one else has an access, free */
        rec->file->num_accs--;
        if(rec->file->num_accs == 0) {
            if((q = ndrec[slot]->file->nsdghdr) != NULL) {
                for(cur = q->nsdg_t, new = q->nsdg_t; cur != NULL; cur = new) {
                    new = cur->next;
                    if((cur = (DFnsdgle *) HDfreespace(cur)) != NULL)
                        ret = FAIL;
                }
                if((q = (DFnsdg_t_hdr *) HDfreespace(q)) != NULL)
                    ret = FAIL;
                ndrec[slot]->file->nsdghdr = NULL;
            } /* if nsdghdr != NULL */

            if((ndrec[slot] = (NDFILEREC *) HDfreespace(ndrec[slot])) != NULL)
                ret = FAIL;
        } /* if num_accs == 0 */
    }


    if(ret == FAIL) {
        Hclose(rec->file->Hid);
        return FAIL;
    }
    else return(Hclose(rec->file->Hid));
}

/*****************************************************************************/
/*----------------------- Internal routines ---------------------------------*/
/*****************************************************************************/
/*-----------------------------------------------------------------------------
 * Name:    DFNDIinitsd
 * Purpose: Insure structures exist for actions from DFSD interface
 * Inputs:  none
 * Returns: 0 on success, FAIL on failure with error set
 * Users:   DFSDset...
 *---------------------------------------------------------------------------*/
#ifdef PROTOTYPE
int DFNDIinitsd(void)
#else
int DFNDIinitsd()
#endif /* PROTOTYPE */
{
    int i;
    NDFILEREC *p;
    DFSsdg *w;
    Stat *r;
    char *FUNC = "DFNDIinitsd";

    if(ndrec[MAX_FILE] == NULL) {
        if((p = ndrec[MAX_FILE] = HDgetspace(sizeof(NDFILEREC))) == NULL) {
            HERROR(DFE_NOSPACE);  return FAIL;
        }

        p->file = NULL;
        if((p->context = (NDGCONTEXT *)HDgetspace(sizeof(NDGCONTEXT)))==NULL) {
            HERROR(DFE_NOSPACE);  return FAIL;
        }

        w = &p->context->Writesdg;
        w->data.tag = DFTAG_NULL;
        w->data.ref = (uint16) 0;
        w->rank = (intn) 0;
        if((w->dimsizes = (int32 *) HDgetspace(sizeof(int32))) == NULL) {
          HERROR(DFE_NOSPACE);  return FAIL;
        }
        if((w->coordsys = (char *) HDgetspace(sizeof(char))) == NULL) {
          HERROR(DFE_NOSPACE);  return FAIL;
        }
        for(i = 0; i < 3; i++)
          if((w->dataluf[i] = (char *) HDgetspace(sizeof(char))) == NULL) {
            HERROR(DFE_NOSPACE);  return FAIL;
          }
        for(i = 0; i < 3; i++)
          if((w->dimluf[i] = (char **) HDgetspace(sizeof(char *))) == NULL) {
            HERROR(DFE_NOSPACE);  return FAIL;
          }
        if((w->dimscales = (uint8 **) HDgetspace(sizeof(uint8 *))) == NULL) {
          HERROR(DFE_NOSPACE);  return FAIL;
        }
        for(i = 0; i < 16; i++) w->max_min[i] = (uint8) 0;
        w->numbertype = DFNT_NONE;
        w->filenumsubclass = DFNTF_NONE;
        w->aid = (int32) -1;
        w->compression = (int32) 0;
        w->isndg = 0;

        w = &p->context->Readsdg;
        w->data.tag = DFTAG_NULL;
        w->data.ref = (uint16) 0;
        w->rank = (intn) 0;
        if((w->dimsizes = (int32 *) HDgetspace(sizeof(int32))) == NULL) {
          HERROR(DFE_NOSPACE);  return FAIL;
        }
        if((w->coordsys = (char *) HDgetspace(sizeof(char))) == NULL) {
          HERROR(DFE_NOSPACE);  return FAIL;
        }
        for(i = 0; i < 3; i++)
          if((w->dataluf[i] = (char *) HDgetspace(sizeof(char))) == NULL) {
            HERROR(DFE_NOSPACE);  return FAIL;
          }
        for(i = 0; i < 3; i++)
          if((w->dimluf[i] = (char **) HDgetspace(sizeof(char *))) == NULL) {
            HERROR(DFE_NOSPACE);  return FAIL;
          }
        if((w->dimscales = (uint8 **) HDgetspace(sizeof(uint8 *))) == NULL) {
          HERROR(DFE_NOSPACE);  return FAIL;
        }
        for(i = 0; i < 16; i++) w->max_min[i] = (uint8) 0;
        w->numbertype = DFNT_NONE;
        w->filenumsubclass = DFNTF_NONE;
        w->aid = (int32) -1;
        w->compression = (int32) 0;
        w->isndg = 0;

        r = &p->context->Ref;
        r->dims = r->nt = r->coordsys = r->scales = r->maxmin =
            r->transpose = -1;
        for(i = 0; i < 3; i++)
            r->luf[i] = -1;

        for(i = 0; i < 4; i++)
          p->context->Maxstrlen[i] = DFS_MAXLEN;

        p->context->Writeref = 0;
        p->context->Readref = 0;
        p->context->Lastref = 0;
        p->context->Newdata = -1;
        p->context->Nextsdg = 1;
        p->context->Ismaxmin = 0;
        p->context->FileTranspose = 0;
        p->context->Fortorder = 0;
        p->context->lastnsdg.tag = DFTAG_NULL;
        p->context->lastnsdg.ref = 0;
    }

    return(0);
}

/*-----------------------------------------------------------------------------
 * Name:    DFNDIget_rec_slot
 * Purpose: Pick out slot in array for the record associated with this file
 * Inputs:  Hid: id returned by Hopen
 * Returns: slot, range 0 to MAX_FILE, on success; FAIL on failure
 * Method:  linear search for unused slot
 *---------------------------------------------------------------------------*/
#ifdef PROTOTYPE
int DFNDIget_rec_slot()
#else
int DFNDIget_rec_slot()
#endif /* PROTOTYPE */
{
    int i;
    char *FUNC = "DFNDIget_rec_slot";

    for(i = 0; i < MAX_FILE; i++)
        if(ndrec[i] == NULL)
            return i;

    /* couldn't find an open slot */
    HERROR(DFE_TOOMANY);
    return FAIL;
}

/*-----------------------------------------------------------------------------
 * Name:    DFNDIsdginfo
 * Purpose: Locates next sdg in file
 * Inputs:  id: id of HDF file
 * Returns: 0 on success, FAIL on failure with error set
 * Users:   HDF systems programmers, DFNDgetdims, DFNDgetdata
 * Invokes: DFIfind, DFNDgetsdg
 * Method:  Call DFIfind to find SDG, then DFNDgetsdg to read it in to Readsdg
 * Remarks: none
 *---------------------------------------------------------------------------*/

#ifdef PROTOTYPE
int DFNDIsdginfo(NDGID id)
#else
int DFNDIsdginfo(id)
NDGID id;
#endif /* PROTOTYPE */
{
    NDFILEREC *rec = NDGID2REC(id);
    DFdi *ptr;
    char *FUNC="DFNDIsdginfo";
    int32 aid;

    HEclear();

    if (!VALIDNDGID(id)) {
        HERROR(DFE_BADCALL); return FAIL;
    }
    ptr = (DFdi *)HDgetspace(sizeof(DFdi));
    if (rec->context->Readref != 0) {
        aid = Hstartread(rec->file->Hid, DFTAG_NDG, rec->context->Readref);
        if (aid != FAIL) {
            ptr->ref = rec->context->Readref;
            ptr->tag = DFTAG_NDG;
            Hendaccess(aid);
        }
        else {
            aid = Hstartread(rec->file->Hid, DFTAG_SDG, rec->context->Readref);
            if (aid != FAIL)    {
                ptr->ref = rec->context->Readref;
                ptr->tag = DFTAG_SDG;
                Hendaccess(aid);
            }
            else     return FAIL;
        }
    }
    else        {
        if (DFNDInextnsdg(id, rec->file->nsdghdr, ptr) < 0) return FAIL;
        if ((ptr->tag != DFTAG_NDG) && (ptr->tag != DFTAG_SDG)) {
            HERROR(DFE_BADTAG); return FAIL;
        }
        if (ptr->ref <=0)       {
            HERROR(DFE_BADREF); return FAIL;
        }
        rec->context->Readref = ptr->ref;
    }

    /* find next sdg */
    if (ptr->tag == DFTAG_NDG)  {
	if (DFNDIgetndg(id, ptr->ref, &rec->context->Readsdg) <0) 
	    return FAIL;
        rec->context->Readsdg.isndg = 1;
    }
    else {
    	if (DFNDgetsdg(id, ptr->ref, &rec->context->Readsdg) < 0)
            return FAIL;
        rec->context->Readsdg.isndg = 0;
   }

    rec->context->Lastref = ptr->ref;           /* remember ref read */
    rec->context->lastnsdg.tag = ptr->tag;
    rec->context->lastnsdg.ref = ptr->ref;

    rec->context->Newdata = 1;      /* now Readsdg is fresh */
    rec->context->Readref = 0;
    if ((ptr = (DFdi *) HDfreespace((char *)ptr)) != NULL)
        return FAIL;

    return(0);
}

/*-----------------------------------------------------------------------------
 * Name:    DFNDIisndg
 * Purpose: is the current read sds an sdg or nsdg/ndg
 * Inputs:  fileid: id of open HDF file
 *          isndg: 0 -- pure sdg( written by 3.1); 1 -- nsdg/ndg 
 * Returns: 0 on success, FAIL on failure with error set
 * Users:   HDF systems programmers, functions in DFNDF.c
 * Invokes: none
 * Method:  Assigns Readsdg.isndg to isndg.
 * Remarks: none
 *---------------------------------------------------------------------------*/

#ifdef PROTOTYPE
int DFNDIisndg(int slot, int32 *isndg)
#else
int DFNDIisndg(slot, isndg)
int slot;
int32 *isndg;
#endif /* PROTOTYPE */
{
    NDFILEREC *rec = NDGSLOT2REC(slot);
    *isndg = rec->context->Readsdg.isndg;
    return(0);
}

/*-----------------------------------------------------------------------------
 * Name:    DFNDIgetrrank
 * Purpose: get rank of the current sdg, to transpose dims for Fortran
 * Inputs:  fileid: id of open HDF file
 *          rank: address to return the rank
 * Returns: 0 on success, FAIL on failure with error set
 * Users:   HDF systems programmers, functions in DFNDF.c
 * Invokes: none
 * Method:  Assigns Readsdg.rank to rank.
 * Remarks: none
 *---------------------------------------------------------------------------*/

#ifdef PROTOTYPE
int DFNDIgetrrank(int slot, intn *rank)
#else
int DFNDIgetrrank(slot, rank)
int slot;
intn *rank;
#endif /* PROTOTYPE */
{
    *rank = ndrec[slot]->context->Readsdg.rank;
    return(0);
}

/*-----------------------------------------------------------------------------
 * Name:    DFNDIgetwrank
 * Purpose: get rank of the current sdg, to transpose dims for Fortran
 * Inputs:  fileid: id of open HDF file
 *          rank: address to return the rank
 * Returns: 0 on success, FAIL on failure with error set
 * Users:   HDF systems programmers, functions in DFNDF.c
 * Invokes: none
 * Method:  Assigns Writesdg.rank to rank.
 * Remarks: none
 *---------------------------------------------------------------------------*/

#ifdef PROTOTYPE
int DFNDIgetwrank(int slot, intn *rank)
#else
int DFNDIgetwrank(slot, rank)
int slot;
intn *rank;
#endif /* PROTOTYPE */
{
    NDFILEREC *rec = NDGSLOT2REC(slot);
    *rank = rec->context->Writesdg.rank;
    return(0);
}

/*-----------------------------------------------------------------------------
 * Name:    DFNDIclear
 * Purpose: Reset all "set" values, free allocated space
 * Inputs:  id: id of file in question - needed for some global []'s
 *          sdg: pointer to sdg struct to clear
 * Globals: Ref
 * Returns: 0 on success, FAIL on error with error set
 * Users:   HDF users, utilities, other routines
 * Invokes: none
 * Method:  Release space in sdg
 * Remarks: none
 *---------------------------------------------------------------------------*/

#ifdef PROTOTYPE
int DFNDIclear(int slot, DFSsdg *sdg)
#else
int DFNDIclear(slot, sdg)
int slot;
DFSsdg *sdg;
#endif /* PROTOTYPE */
{
    int luf, i;
    NDFILEREC *rec = NDGSLOT2REC(slot);
    char *FUNC="DFNDIclear";

    HEclear();

    if (Sfile_id !=DF_NOFILE) {        /* cannot clear during slice writes */
        HERROR(DFE_BADCALL); return FAIL;
    }
    sdg->dimsizes = (int32 *) HDfreespace((char*) sdg->dimsizes);

    sdg->coordsys = HDfreespace(sdg->coordsys);

        /* free label/unit/format pointers */
    for (luf = LABEL; luf <= FORMAT; luf++) {
        if (sdg->dimluf[luf])       /* free strings */
            for (i = 0; i < sdg->rank; i++)
                sdg->dimluf[luf][i] = HDfreespace(sdg->dimluf[luf][i]);

	/* free string pointers */
        sdg->dimluf[luf] = (char **) HDfreespace((char*) sdg->dimluf[luf]);

	/* free data string */
        sdg->dataluf[luf] = HDfreespace(sdg->dataluf[luf]);
    }

    /* free scale pointers */
    if (sdg->dimscales)
        for (i = 0; i < sdg->rank; i++)
            sdg->dimscales[i] = (uint8 *)
		HDfreespace((char*) sdg->dimscales[i]);

    /* free array of scale pointers */
    sdg->dimscales = (uint8 **) HDfreespace((int8 *)sdg->dimscales);

    sdg->rank = 0;
    /* number type is independant to dimsizes   4/7/92  sxu
    sdg->numbertype = DFNT_NONE;
    sdg->filenumsubclass = DFNTF_NONE;
    */
    sdg->aid = (int32) -1;
    sdg->compression = (int32) 0;
    rec->context->FileTranspose = 0;

    rec->context->Ref.dims = rec->context->Ref.scales = rec->context->Ref.luf[LABEL] = rec->context->Ref.luf[UNIT]
        = rec->context->Ref.luf[FORMAT] =  rec->context->Ref.coordsys = rec->context->Ref.maxmin = (-1);
    return(0);
}


/*-----------------------------------------------------------------------------
 * Name:    DFNDIgetdata
 * Purpose: Get data from SDG.  Will sequence to next SDG if DFNDgetdims not
 *          called.
 * Inputs:  id: id of HDF file to use
 *          rank: no of dimensions of array "data"
 *          maxsizes: actual dimensions of array "data"
 *          data: data for returning scientific data
 *          isfortran : 0 if called from C, 1 when called from FORTRAN
 * Returns: 0 on success, FAIL on failure with error set
 * Outputs: actual scientific data in array
 * Users:   DFNDgetdata
 * Invokes: DFNDIgetslice, HDgetspace, HDfreespace, HERROR, DFNDIsdginfo
 * Method:  Call DFNDIsdginfo to read sdg if necessary, set up
 *          window start and end arrays, call DFNDIgetslice.
 * Remarks: maxsizes may be larger than actual size.  In that event, the actual
 *          data may not be contiguous in the array "data"
 *          User sets maxsizes before call.
 *---------------------------------------------------------------------------*/

#ifdef PROTOTYPE
int DFNDIgetdata(NDGID id, intn rank, int32 maxsizes[], void *data,
		 int isfortran)
#else
int DFNDIgetdata(id, rank, maxsizes, data, isfortran)
NDGID id;
intn rank;
int32 maxsizes[];
void *data;
int isfortran;
#endif /* PROTOTYPE */
{
    int32 *winst, *windims;
    int ret, i;
    NDFILEREC *rec = NDGID2REC(id);
    char *FUNC = "DFNDIgetdata";

    HEclear();

    if (!VALIDNDGID(id)) {
        HERROR(DFE_BADCALL); return FAIL;
    }

    /* if Readsdg not fresh */
    if (rec->context->Newdata != 1 || rec->context->Nextsdg ) {
        /* check that table is built */
        if(rec->file->nsdghdr->nsdg_t == NULL)
            if(DFNDIsetnsdg_t(id, rec->file->nsdghdr) < 0) {
                return FAIL;
            }
        if (DFNDIsdginfo(id) < 0)  {   /* reads next SDG from file */
            return FAIL;
        }
    }

    winst = (int32 *)HDgetspace((uint32) rec->context->Readsdg.rank *
                                sizeof(int32));
    if (winst == NULL) return FAIL;
    windims = (int32 *) HDgetspace((uint32) rec->context->Readsdg.rank
                                   * sizeof(int32));
    if (windims == NULL) {
        HDfreespace((char*) winst);
        return FAIL;
    }

    for (i = 0; i < rank; i++) {
        winst[i] = 1;
        windims[i] = rec->context->Readsdg.dimsizes[i];
    }

    ret = DFNDIgetslice(id, winst, windims, data, maxsizes, isfortran);
    winst = (int32 *) HDfreespace((char*) winst);
    windims = (int32 *) HDfreespace((char*) windims);
    rec->context->Nextsdg = 1;
    return(ret);
}


/*-----------------------------------------------------------------------------
 * Name:    DFNDIputdata
 * Purpose: Writes entire SDG to file
 * Inputs:  filename: name of HDF file to use
 *          rank: rank of data array
 *          dimsizes: sizes of the dimensions of data array
 *          data: array that holds data
 *          accmode: 0 if write to new file, 1 if append to file
 *          isfortran: 0 if C, 1 if FORTRAN
 * Globals: Writeref
 * Returns: 0 on success, FAIL on failure with error set
 * Users:   HDF users, utilities, other routines
 * Invokes: HDgetspace, HDfreespace, DFNDIputslice,
 *          DFNDstartslice, DFNDIendslice
 * Method:  Create file if necessary, allocate arrays, call slice routines
 *---------------------------------------------------------------------------*/

#ifdef PROTOTYPE
int DFNDIputdata(NDGID id, intn rank, int32 *dimsizes, void *data,
                 int isfortran)
#else
int DFNDIputdata(id, rank, dimsizes, data, isfortran)
NDGID id;
intn  rank;
int32   *dimsizes;
void *data;
int isfortran;
#endif /* PROTOTYPE */
{
    int ret;
    NDFILEREC *rec = NDGID2REC(id);
    char *FUNC="DFNDIputdata";

    HEclear();

    if (!VALIDNDGID(id)) {
        HERROR(DFE_BADCALL); return FAIL;
    }

    if (rec->context->Ref.dims)       /* don't call setdims if already called */
        if (DFNDsetdims(id, rank, dimsizes)<0) return FAIL;

    if (DFNDstartslice(id)<0) return FAIL;

    if((ret=DFNDIputslice(id, rec->context->Writesdg.dimsizes,
                          data, dimsizes, isfortran)) < 0)
        return ret;

    return DFNDIendslice(id, isfortran);
}

/*----------------------------------------------------------------------------
 * Name:    DFNDIgetslice
 * Purpose: Get slice of data from SDG.  Will sequence to next SDG if
 *          DFNDgetdims, DFNDgetdata or DFNDgetslice not called earlier.
 * Inputs:  id: id of open HDF file to use
 *          winst: array of size = rank of data, containing start of slice
 *          windims: array of size rank, containing size of slice
 *          data: array for returning slice
 *          dims: dimensions of array data
 *          isfortran : 0 if called from C, 1 when called from FORTRAN
 * Returns: 0 on success, FAIL on failure with error set
 * Outputs: slice of data in data
 * Users:   DFNDIgetdata
 * Invokes: HERROR, DFNDIsdginfo, DFaccess, DFread
 * Method:  Open file, call DFNDIsdginfo to read sdg if necessary, read the
 *          data, convert types if necessary, place in data as appropriate
 *          data is assumed column major for FORTRAN, row major for C
 * Remarks: dims may be larger than size of slice.  In that event, the actual
 *          data may not be contiguous in the array "data".
 *          User sets dims before call.
 *--------------------------------------------------------------------------*/

/*****************************************************************************/
/* DESIGN DECISIONS                                                          */
/*****************************************************************************/
/* 
   A. All stride/index/offset value will, when this is done -- refer to
      element counts rather than byte counts in the name of consistency.

   B. The conversion buffers/allcated areas... will all be char buffers --
      providing that the Cray-2 is cooperative.
									     */	
/*****************************************************************************/

/*****************************************************************************/
/* CHANGE LOG                                                                */
/*****************************************************************************/
/*
  A.
									     */
/*****************************************************************************/

#ifdef PROTOTYPE
int DFNDIgetslice(NDGID id, int32 winst[], int32 windims[],
		  void *data, int32 dims[], int isfortran)
#else
int DFNDIgetslice(id, winst, windims, data, dims, isfortran)
    NDGID  id;         /* HDF file containing the dataset */
    int32   winst[];    /* array containing the coordinates of the start */
                            /* of the slice in the HDF file */
    int32	windims[];	/* array containing the size of the slice */
    int32	dims[];		/* array containing the dimensions of data[] */
    void	*data;		/* array to hold the floating point data read*/
    int		isfortran;	/* true if called from Fortran */
#endif /* PROTOTYPE */

{
    intn
        rank;           /* number of dimensions in data[] */
    int32
        leastsig,       /* fastest varying subscript in the array */
        error,          /* flag if an error occurred, */
                            /* used by DFconvert macro */
        convert,        /* true if machine NT != NT to be read */
        transposed,     /* true if we must transpose the data before writing */
        done,           /* true if we are at the end of the slice */
        aid; 
    int32
        i, j,           /* temporary loop index */
        issdg,          /* 1 -- pure sdg. do what HDF3.1 does   */
        *wstart,        /* tmp array containing starting slice dims */
        *wdims,         /* tmp array containing the slice size */
        *adims,         /* tmp array containing the dimensions of data[] */
        *fdims,         /* tmp array containing the dimensions */
                            /* of the dataset in the file */
        numtype,    /* current number type  */
        fileNTsize, /* size of this NT in the file  */
        localNTsize,    /* size of this NT as it occurs in this machine */
        numelements,    /* number of floats to read at once */
        readsize,       /* number of bytes to read at once */
        datastride,         /* number of floats in one row of data[] */
        *offset,        /* array for accessing the next element in data[] */
        *foffset,       /* array for accessing the next element in the file */
        *dimsleft,      /* array for tracking the current position in data[] */
        isnative, 
        fileoffset,     /* offset into the current dataset in the file */
        machinetype;    /* assigned DF_MT.  used for debugging */
    uint8
        platnumsubclass, /* class of this NT for this platform */
     	fileNT, 	/* file number subclass	*/
        *scatterbuf,    /* buffer to hold the current row contiguously */
        *sp,		/* ptr into scatterbuf 		*/
        *datap,         /* ptr into data[] at starting offset */
                            /* of current block */
        *dp,            /* ptr into data[] at an element of the current row */
    	*buf;           /* buffer containing the converted current row */
    NDFILEREC *rec = NDGID2REC(id);
    char *FUNC="DFNDIgetslice";

    HEclear();

    if (!VALIDNDGID(id)) {
        HERROR(DFE_BADCALL); return FAIL;
    }

    if (!data) {
        HERROR(DFE_BADPTR); return FAIL;
    }

    if (rec->context->Newdata != 1) {   /* if ->context->Readsdg not fresh */
        if(rec->file->nsdghdr->nsdg_t == NULL)/* check that table is built */
            if(DFNDIsetnsdg_t(id, rec->file->nsdghdr) < 0) {
                return FAIL;
            }
        if (DFNDIsdginfo(id)<0)  { /* reads next SDG from file */
            return FAIL;
        }
    }
    rank = rec->context->Readsdg.rank;
    numtype = rec->context->Readsdg.numbertype;
    isnative = DFNT_NATIVE;
    machinetype = DF_MT;
    platnumsubclass = DFKgetPNSC(numtype, DF_MT);
    localNTsize = DFKNTsize(numtype | isnative);
    fileNTsize = DFKNTsize(numtype);
    fileNT = rec->context->Readsdg.filenumsubclass;
    issdg = rec->context->Readsdg.isndg ? 0 : 1;

    /* get dimensions of slice to extract, set nwindims. also err check */
    for (i=0; i < (int32)rank; i++) {
    	/* check validity for the dimension ranges */
        if ((windims[i] < 1) || (winst[i] < 1)
            || (winst[i]+windims[i]-1 > rec->context->Readsdg.dimsizes[i])) {
            HERROR(DFE_BADDIM);
            return FAIL;
        }
        /* check if space allocated is sufficient */
        if (dims[i] < windims[i]) {
            HERROR(DFE_NOTENOUGH);
            return FAIL;
        }
    }
    /* allocate buffers */
    wstart = (int32 *) HDgetspace((uint32) 4 * rank * sizeof(int32));
    if (wstart == NULL) {
        HERROR(DFE_NOSPACE);
        return FAIL;
    }
    wdims = wstart + rank;
    adims = wdims + rank;
    fdims = adims + rank;

    /* copy arrays to private workspace (so that they are in row major order)*/
    for (i=0; i < (int32)rank; i++) {
        int32 ii = (issdg && isfortran) ? (int32)rank - i - 1  : i;

        adims[i] = dims[ii];
        ii = (issdg && rec->context->FileTranspose) ? (int32)rank - i - 1 : i;
        wstart[i] = winst[ii]-1; /* translate to 0 origin */
        wdims[i] = windims[ii];
        fdims[i] = rec->context->Readsdg.dimsizes[ii];
    }

    convert = (fileNT != platnumsubclass); /* is conversion necessary */

    /* is transposition needed */
    transposed = issdg && (isfortran ^ rec->context->FileTranspose);

    /*
     * Note that if the data is transposed we must work on a row by row
     * basis and cannot collapse dimensions.
     */
    if (!transposed) {
        /* collapse dimensions if contiguous both in the file and in memory */
        for (i=(int32)rank-1; i>0; i--) { /* stop before most sig dim */
            if (adims[i] > wdims[i] /* not all of data[] will be filled */
                || wstart[i] != 0 /* reading only part of the dataset */
                || wdims[i] < fdims[i]) break;
            wstart[i-1] *= fdims[i];
            wdims[i-1] *= wdims[i];
            adims[i-1] *= adims[i];
            fdims[i-1] *= fdims[i];
            rank--;
        }
    }
    leastsig = (int32)rank - 1;  /* which is least sig dim */

    /* position at start of data set */
    aid = Hstartread(rec->file->Hid, rec->context->Readsdg.data.tag,
                     rec->context->Readsdg.data.ref);
    if (aid == FAIL) {
        HDfreespace((char *)wstart);
        return FAIL;
    }

    error = 0;
    if (rank==1 && !convert) {
        /* all data is contiguous with no conversions */
        readsize = adims[0] * fileNTsize;
        if ( (Hseek(aid, wstart[0] * fileNTsize, 0) == FAIL) ||
            (readsize != Hread(aid, readsize, (uint8 *)data)) ) error = 1;
    } else {
	/*
	 * The data must be further manipulated.
	 * It may be transposed, may need conversion, may not be contiguous, or
	 * any combination of these.
	 */
        numelements  = wdims[leastsig];
        readsize = numelements * fileNTsize;

        /* allocate 1 row buffers */
        if (convert) {
            if ((buf = HDgetspace((uint32) readsize)) == NULL) {
                HDfreespace((char *)wstart);
                HERROR(DFE_NOSPACE);
                Hendaccess(aid);
                return FAIL;
            }
        } else buf = NULL;

        if (transposed) {
            scatterbuf = (uint8 *)HDgetspace((uint32) numelements *
                            localNTsize);
            if (scatterbuf == NULL) {
                HDfreespace((char *)wstart);
                HDfreespace(buf);
                HERROR(DFE_NOSPACE);
                Hendaccess(aid);
                return FAIL;
            }
        } else scatterbuf = NULL;

        offset = (int32 *) HDgetspace((uint32)3 * rank * sizeof(int32));
        if (offset == NULL) {
            HDfreespace((char *)wstart);
            HDfreespace(buf);
            HDfreespace((char *)scatterbuf);
            HERROR(DFE_NOSPACE);
            Hendaccess(aid);
            return FAIL;
        }
        foffset = offset + rank;
        dimsleft = foffset + rank;

        /* compute initial position in the data */
        for (i = leastsig; i >= 0; i--)
            dimsleft[i] = wdims[i];

        /* compute offsets in the source array */
        if (transposed) {
            offset[0] == 1 * localNTsize;
            for (i = 0;  i <leastsig; i++)
            offset[i + 1] = offset[i] * adims[leastsig - i];
        } else {
            offset[leastsig] = 1*localNTsize;
            for (i = leastsig; i > 0; i--)
            offset[i-1] = offset[i] * adims[i];
        }
        datastride = offset[leastsig];

        /* compute offsets in the file */
        for (i=leastsig, foffset[i] = 1 * fileNTsize; i > 0; i--)
            foffset[i-1] = foffset[i] * fdims[i];

        /*
         * Compute starting position in file
         * All file reads are done relative to this starting offset.
         * Cumulative offset is from most sig to next to least sig dim.
         */
        for (i = 0, fileoffset = 0; i < leastsig; i++)
            fileoffset = (fileoffset + wstart[i]) * fdims[i + 1];
        fileoffset += wstart[leastsig]; /* adjust for last dim */
        fileoffset *= fileNTsize; /* convert to bytes */

        datap = (uint8 *)data;
        done = 0;

        /* -- now read in the data */
        do {
            /* move to the next data element in the file */
            if (Hseek(aid, fileoffset, 0) == FAIL) {
                error = 1;
                break;
            }

            /* read and convert one contiguous block of data */
            if (convert) {
                if (readsize != Hread(aid, readsize, buf)) {
                    error = 1;
                    break;
                }
                DFKconvert(buf, transposed ? (uint8 *)scatterbuf :
                    (uint8 *)datap, numtype, numelements, DFACC_READ, 0, 0);
            }
            else {
                if (readsize != Hread(aid, readsize,
                        transposed ? scatterbuf : datap)) {
                    error=1;
                    break;
                }
            }
            if (transposed) {
            /* scatter out the elements of one row */
#ifdef UNICOS
#pragma ivdep
#endif
                for (dp = datap, sp = scatterbuf, i=0; i < numelements; i++) {
                    for (j = 0; j < localNTsize; j++)
                        *(dp +j) = *(sp +j);
                    sp += localNTsize;
                    dp += datastride;
                }
            }

            /*
             * Find starting place of the next row/block.
             * Note that all moves are relative:
             *   this preserves the starting offsets of each dimension
             */
            for (i = leastsig - 1; i >= 0; i--) {
                if (--dimsleft[i] > 0) {
                    /* move to next element in the current dimension */
                    datap += offset[i];
                    fileoffset += foffset[i];
                    break;
                } else {
                    dimsleft[i] = wdims[i];
                    /*
                     * Note that we are still positioned at the beginning of
                     * the last element in the current dimension
                     */
                    /* move back to the beginning of dimension i */
                    datap -= offset[i] * (wdims[i]-1);
                    /* move back to beginning read position of dimension i */
                    fileoffset -= foffset[i] * (wdims[i]-1);
                    if (i == 0) done = 1;
                }
            }
        } while (!done && leastsig > 0);

        HDfreespace(buf);
        HDfreespace((char *)scatterbuf);
        HDfreespace((char *)offset);
    }

    Hendaccess(aid);
    HDfreespace((char *)wstart);
    if (error)  {
        return FAIL;
    }
    else return (0);

}

/*----------------------------------------------------------------------------
 * Name:    DFNDIputslice
 * Purpose: Put slice of data to SDG.
 * Inputs:  id: id of open HDF file to write to
 *          windims: array of size rank, containing size of slice
 *          data: array containing slice
 *          dims: dimensions of array data
 *	    	isfortran: 0 for C, 1 for Fortran
 * Returns: 0 on success, FAIL on failure with error set
 * Users:   DFNDputslice, DFNDadddata
 * Invokes: DFwrite, HDgetspace, HDfreespace,DFKnumout(if conversion
	    required)
 * Method:  check dimensions for contiguity, convert types if necessary
 *          write to file
 * Remarks: dims may be larger than size of slice.  In that event, the actual
 *          data may not be contiguous in the array "data".
 *          DFNDstartslice must have been called first
 *          If DFKnumout is called, DFNDsetNT may need to have been
 *	    called
 *	    Note, writes must be contiguous - successive calls to putslice
 *          must write out array consecutively, according to the setting
 *          of the Fortorder variable - row major if 0, column major if 1
 *--------------------------------------------------------------------------*/

#ifdef PROTOTYPE
int DFNDIputslice(NDGID id, int32 windims[], void *data, int32 dims[],
                  int isfortran)
#else
int DFNDIputslice(id, windims, data, dims, isfortran)
NDGID id;                /* file id returned by DFNDopen */
int32	windims[];	/* array containing dimensions of the slice */
int32	dims[];		/* array containing the dimensions of data[] */
void	*data;		/* array of the floating point data to write */
int	isfortran;	/* true if called from Fortran */
#endif /* PROTOTYPE */
{
    intn
        rank;          /* number of dimensions in data[] */
    int32
        leastsig,      /* fastest varying subscript in the array */
        convert,       /* true if machine NT = NT to be written */
        contiguous,    /* true if there are no gaps in the data to be written*/
    	numtype,       /* current number type */
        platnumsubclass, /* class of this NT for this platform */
        fileNTsize,    /* size of this NT as it will be in the file */
        isnative,
        localNTsize;   /* size of this NT as it occurs in theis machine */
    int32
        ret,           /* return code from DFwrite */
        i, j,          /* temporaries */
        numelements,   /* number of elements to write out per row */
        writesize,     /* number of bytes to write out per row */
        datastride;    /* number of bytes in one row of data[] */
    uint8
        *datap,        /* pointer into data[] at */
                          /* the start of the current row */
        *buf;          /* buffer containing converted current row */
    NDFILEREC *rec = NDGID2REC(id);
    char *FUNC="DFNDIputslice";

    HEclear();

    if (!VALIDNDGID(id)) {
        HERROR(DFE_BADCALL); return FAIL;
    }

    if (!data) {
        HERROR(DFE_BADPTR); return FAIL;
    }
    if (Sfile_id == DF_NOFILE) {
        HERROR(DFE_BADCALL); return FAIL;
    }

    rank = rec->context->Writesdg.rank;

    for (i=0; i < (int32)rank; i++) {
    	/* check validity for the dimension ranges */
        if ((windims[i]<=0) || (windims[i] > rec->context->Writesdg.dimsizes[i])) {
            HERROR(DFE_BADDIM); return FAIL;
        }
	/* check if space allocated is sufficient */
	if (dims[i] < windims[i]) {
	    HERROR(DFE_NOTENOUGH); return FAIL;
	}
    }

    /* check to see if the slices fit together */
    /* Same for Fortran or C    */
    /* find the first significant dimension */
    for (i = 0; windims[i] == 1 && i < (int32)rank - 1; i++)
        /* empty */;
    /* check that all 'lesser' dims match */
    for (j = i+1; j < (int32)rank; j++)
        if (rec->context->Writesdg.dimsizes[j] != windims[j]) {
            HERROR(DFE_BADDIM); return FAIL;
        }
    /* update Sddims to reflect new write */
    Sddims[i] += windims[i];
    for(; i > 0 && Sddims[i] >= rec->context->Writesdg.dimsizes[i]; i--) {
        Sddims[i-1] += Sddims[i] / rec->context->Writesdg.dimsizes[i]; 
                 /* promote the unit */
        Sddims[i] %= rec->context->Writesdg.dimsizes[i];
    }

    leastsig = (int32) rank - 1;    /* which is least sig dim */
    numtype = rec->context->Writesdg.numbertype;
    platnumsubclass = DFKgetPNSC(numtype, DF_MT);
		/* get class of this num type for this platform */
    fileNTsize = DFKNTsize(numtype);
    isnative = DFNT_NATIVE;
    localNTsize = DFKNTsize(numtype | isnative);
    convert = ( (platnumsubclass != DFNTF_HDFDEFAULT) && /* need conversion? */
		(!DFKisnativeNT(numtype)) );

    contiguous = 1;
    for (i=0; contiguous && i < (int32)rank; i++) {
	/* check if data at the end of the users array will be contiguous */
        if (dims[i] > rec->context->Writesdg.dimsizes[i])
            contiguous = 0;
	/* Note: if a winstart[] array is ever added, will also need */
	/*	to check if data at start of users array will be */
	/*	contig			*/
    }

    /*
     *	2 Factors that determine how we write (in order of importance)
     *	conversion and contiguous
     */
    datap = (uint8 *)data;
    if (!convert && contiguous) {
        /* compute total number of elements to write */
        for (i = 0, numelements = 1; i < (int32)rank; i++) 
            numelements *= windims[i];
        writesize = numelements * fileNTsize;
        ret = Hwrite(rec->context->Writesdg.aid, writesize, (uint8 *)data); /* done */
        if (ret == FAIL)  {
            return FAIL;
        } 
    }
    else {          /* must step through the data */
        /* compute number of occurrences of the least sig dim */
        if (rec->context->Fortorder) 
            for (i = (int32)rank-1, j = 1; i > 0; i--) j *= windims[i];
        else for (i = 0, j = 1; i < (int32)rank - 1; i++) j *= windims[i];
        numelements = windims[leastsig];
        writesize = numelements * fileNTsize;
        datastride = dims[leastsig] * localNTsize;
        if (convert) {
            buf = (uint8 *)HDgetspace((uint32) writesize);
            if (buf == NULL)  {
                return FAIL;
            }
            for (i = 0; i < j; i++, datap += datastride) {
                DFKconvert(datap, buf, numtype,
                           numelements, DFACC_WRITE, 0, 0);
                ret = Hwrite(rec->context->Writesdg.aid, writesize, buf);
                /* done */
                if (ret == FAIL) {
                    HDfreespace(buf);
                    return FAIL;
                }
            }
            HDfreespace(buf);
        }
        else {          /* !contiguous  */
            for (i = 0; i < j; i++, datap += datastride)
                ret = Hwrite(rec->context->Writesdg.aid, writesize, datap);
            if (ret == FAIL) {
                return FAIL;
            }
        }
    }

    return(ret >= 0 ? 0 : FAIL);
}
