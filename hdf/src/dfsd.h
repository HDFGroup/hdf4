/*
$Header$

$Log$
Revision 1.8  1993/01/26 19:42:40  koziol
Added support for reading and writing Little-Endian data on all
platforms.  This has been tested on: Cray, Sun, and PCs so far.

 * Revision 1.7  1993/01/19  05:55:12  koziol
 * Merged Hyperslab and JPEG routines with beginning of DEC ALPHA
 * port.  Lots of minor annoyances fixed.
 *
 * Revision 1.6  1993/01/04  18:03:39  sxu
 * change dfsdpre32 to dfsdpre32sdg
 *
 * Revision 1.5  1992/11/10  20:21:32  georgev
 * Added fill value to DFSsdg struct, Added prototypes for
 *  hyperslab routines
 *
 * Revision 1.4  1992/11/02  16:35:41  koziol
 * Updates from 3.2r2 -> 3.3
 *
 * Revision 1.3  1992/08/31  16:14:52  chouck
 * Added Fortran support for calibration tags
 *
 * Revision 1.2  1992/08/27  22:18:42  chouck
 * Added support for calibration tag reading and writing
 *
 * Revision 1.1  1992/08/25  21:40:44  koziol
 * Initial revision
 *
*/
/*-----------------------------------------------------------------------------
 * File:    dfsd.h
 * Purpose: header file for the Scientific Data set
 * Invokes: dfrig.h
 * Contents: 
 *  Structure definitions: DFSsdg
 *  Constant definitions: DFS_MAXLEN
 * Remarks: This is included with user programs which use SDG
 *          Currently defined to be 2-D.  Will later be increased to
 *          multiple dimensions
 *---------------------------------------------------------------------------*/

#ifndef DFSDG                       /* avoid re-inclusion */
#define DFSDG

#include "hdf.h"

#define DFS_MAXLEN 255		/*  Max length of label/unit/format strings */

/* include numbertype and aid for 3.2   S. Xu   */
/* structure to hold SDG info */
typedef struct DFSsdg {
    DFdi    data;               /* tag/ref of data in file */
    intn    rank;               /* number of dimensions */
    int32   _HUGE *dimsizes;     /* dimensions of data */
    char    _HUGE *coordsys;
    char    _HUGE *dataluf[3];   /* label/unit/format of data */
    char    _HUGE **dimluf[3];   /* label/unit/format for each dim */
    uint8   _HUGE **dimscales;   /* scales for each dimension */
    uint8   max_min[16];        /* max, min values of data, */
	    		       /* currently atmost 8 bytes each	*/
    int32   numbertype;         /* default is float32      */
    uint8   filenumsubclass;    /* number format in the file, default is IEEE */
    int32   aid;                /* access id     */
    int32   compression;        /* 0 -- not compressed  */
    int32   isndg;              /* 0 -- pure sdg, written by 3.1 else ndg */
    float64 cal, cal_err;       /* calibration multiplier stuff          */
    float64 ioff, ioff_err;     /* calibration offset stuff              */
    int32   cal_type;           /* number type of data after calibration */
    uint8   *fill_value;        /* fill value if any specified  */
} DFSsdg;

/* DFnsdgle is the internal structure which stores SDG or NDS and   */
/* related SDG in an HDF file.					    */
/* It is a linked list.						    */

typedef struct DFnsdgle   {
    DFdi nsdg;			/* NDG from 3.2 or SDG from 3.1  */
    DFdi sdg;			/* Only special NDF has values in this field */
    struct DFnsdgle *next;
}  DFnsdgle;

typedef struct DFnsdg_t_hdr	{
    uint32 size;
    DFnsdgle *nsdg_t;
}  DFnsdg_t_hdr;

extern int DFSDgetdims
    PROTO((char _HUGE *filename, intn _HUGE *prank, int32 _HUGE sizes[], intn maxrank));
extern int DFSDgetdatastrs
    PROTO((char _HUGE *label, char _HUGE *unit, char _HUGE *format,char _HUGE *coordsys));
extern int DFSDgetdimstrs
    PROTO((int dim, char _HUGE *label, char _HUGE *unit, char _HUGE *format));
extern int DFSDgetdatalen
    PROTO((int _HUGE *llabel, int _HUGE *lunit, int _HUGE *lformat,int _HUGE *lcoordsys));
extern int DFSDgetdimlen
    PROTO((int dim, int _HUGE *llabel, int _HUGE *lunit, int _HUGE *lformat));
extern int DFSDgetdimscale
    PROTO((int dim, int32 maxsize, void _HUGE *scale));
extern int DFSDgetrange
    PROTO((void _HUGE *pmax, void _HUGE *pmin));
extern int DFSDgetdata
    PROTO((char _HUGE *filename, intn rank, int32 _HUGE maxsizes[], void _HUGE *data));
extern int DFSDsetlengths
    PROTO((int maxlen_label, int maxlen_unit, int maxlen_format,
                int maxlen_coordsys));
extern int DFSDsetdims
    PROTO((intn rank, int32 _HUGE dimsizes[]));
extern int DFSDsetdatastrs
    PROTO((char _HUGE *label, char _HUGE *unit, char _HUGE *format,char _HUGE *coordsys));
extern int DFSDsetdimstrs
    PROTO((int dim, char _HUGE *label, char _HUGE *unit, char _HUGE *format));
extern int DFSDsetdimscale
    PROTO((int dim, int32 dimsize, void _HUGE *scale));
extern int DFSDsetrange
    PROTO((void _HUGE *maxi, void _HUGE *mini));
extern int DFSDputdata
    PROTO((char _HUGE *filename, intn rank, int32 _HUGE dimsizes[], void _HUGE *data));
extern int DFSDadddata
    PROTO((char _HUGE *filename, intn rank, int32 _HUGE dimsizes[], void _HUGE *data));
extern int DFSDrestart
    PROTO((void));
extern int32 DFSDnumber
    PROTO((char _HUGE *filename));
extern int DFSDclear
    PROTO((void));
extern int DFSDlastref
    PROTO((void));
extern int DFSDreadref
    PROTO((char _HUGE *filename, uint16 ref));
extern int DFSDgetslice
    PROTO((char _HUGE *filename, int32 _HUGE winst[], int32 _HUGE windims[],void _HUGE *data,
                int32 _HUGE dims[]));
extern int DFSDstartslice
    PROTO((char _HUGE *filename));
extern int DFSDputslice
    PROTO((int32 _HUGE winend[], void _HUGE *data, int32 _HUGE dims[]));
extern int DFSDendslice
    PROTO((void));
extern int DFSDsetNT
    PROTO((int32 numbertype));
extern int DFSDsetorder
    PROTO((int arrayorder));
extern int DFSDgetNT
    PROTO((int32 _HUGE *pnumbertype));
extern intn DFSDpre32sdg
    PROTO((char _HUGE *filename,uint16 ref, intn _HUGE *ispre32));
extern int DFSDgetsdg
    PROTO((int32 file_id, uint16 ref, DFSsdg _HUGE *sdg));
extern int DFSDputsdg
    PROTO((int32 file_id, uint16 ref, DFSsdg _HUGE *sdg));
extern int32 DFSDIopen
    PROTO((char _HUGE *filename, int access));
extern int DFSDIsdginfo
    PROTO((int32 file_id));
extern int DFSDIclear
    PROTO((DFSsdg _HUGE *sdg));
extern int DFSDIclearNT
    PROTO((DFSsdg _HUGE *sdg));
extern int DFSDIgetdata
    PROTO((char _HUGE *filename, intn rank, int32 _HUGE maxsizes[], void _HUGE *data,
                int isfortran));
extern int DFSDIputdata
    PROTO((char _HUGE *filename, intn rank, int32 _HUGE *dimsizes, void _HUGE *data,
                int accmode, int isfortran));
extern int DFSDIgetslice
    PROTO((char _HUGE *filename, int32 _HUGE winst[], int32 _HUGE windims[], void _HUGE *data,
                int32 _HUGE dims[], int isfortran));
extern int DFSDIputslice
    PROTO((int32 _HUGE windims[], void _HUGE *data, int32 _HUGE dims[], int isfortran));
extern int DFSDIendslice
    PROTO((int isfortran));
extern intn DFSDIrefresh
    PROTO((char _HUGE *filename));
extern int DFSDIisndg
    PROTO((intn _HUGE *isndg));
extern int DFSDIgetrrank
    PROTO((intn _HUGE *rank));
extern int DFSDIgetwrank
    PROTO((intn _HUGE *rank));
extern int DFSDIsetdimstrs
    PROTO((int dim, char _HUGE *label, char _HUGE *unit, char _HUGE *format));
extern int DFSDIsetdatastrs
    PROTO((char _HUGE *label, char _HUGE *unit, char _HUGE *format,
            char _HUGE *coordsys));

extern int DFSDsetcal
    PROTO((float64 cal, float64 cal_err, float64 ioff, 
           float64 ioff_err, int32 cal_nt));
extern int DFSDgetcal
    PROTO((float64 _HUGE *pcal, float64 _HUGE *pcal_err, float64 _HUGE *pioff,
               float64 _HUGE *pioff_err, int32 _HUGE *cal_nt));

extern int DFSDwriteref
    PROTO((char _HUGE *filename, uint16 ref));
extern int DFSDsetfillvalue
    PROTO((void _HUGE *fill_value));
extern int DFSDgetfillvalue
    PROTO((void _HUGE *fill_value));
extern int DFSDwritefillvalue
    PROTO((char _HUGE *filename, void _HUGE *fill_value));
extern int DFSDwriteslab
    PROTO((char _HUGE *filename, int32 _HUGE start[],int32 _HUGE stride[],
           int32 _HUGE count[], void _HUGE *data));

#endif /*DFSDG*/
