/*
$Header$

$Log$
Revision 1.3  1992/08/31 16:14:52  chouck
Added Fortran support for calibration tags

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

typedef struct {                /* DFdi = data identifier = tag/ref */
    uint16 tag, ref;
} DFdi;

#define DFS_MAXLEN 255		/*  Max length of label/unit/format strings */

/* include numbertype and aid for 3.2   S. Xu   */
/* structure to hold SDG info */
typedef struct DFSsdg {
    DFdi    data;              /* tag/ref of data in file */
    intn    rank;              /* number of dimensions */
    int32   *dimsizes;	       /* dimensions of data */
    char    *coordsys;
    char    *dataluf[3];       /* label/unit/format of data */
    char    **dimluf[3];       /* label/unit/format for each dim */
    uint8   **dimscales;       /* scales for each dimension */
    uint8   max_min[16];       /* max, min values of data, */
	    		       /* currently atmost 8 bytes each	*/
    int32   numbertype;	       /* default is float32      */
    uint8   filenumsubclass;   /* number format in the file, default is IEEE */
    int32   aid;               /* access id		*/
    int32   compression;       /* 0 -- not compressed  */
    int32   isndg;      /* 0 -- pure sdg, written by 3.1 else ndg */
    float64 cal, cal_err;      /* calibration multiplier stuff          */
    float64 ioff, ioff_err;    /* calibration offset stuff              */
    int32   cal_type;          /* number type of data after calibration */
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
    PROTO((char *filename, intn *prank, int32 sizes[], intn maxrank));
extern int DFSDgetdatastrs
    PROTO((char *label, char *unit, char *format,char *coordsys));
extern int DFSDgetdimstrs
    PROTO((int dim, char *label, char *unit, char *format));
extern int DFSDgetdatalen
    PROTO((int *llabel, int *lunit, int *lformat,int *lcoordsys));
extern int DFSDgetdimlen
    PROTO((int dim, int *llabel, int *lunit, int *lformat));
extern int DFSDgetdimscale
    PROTO((int dim, int32 maxsize, void *scale));
extern int DFSDgetrange
    PROTO((void *pmax, void *pmin));
extern int DFSDgetdata
    PROTO((char *filename, intn rank, int32 maxsizes[], void *data));
extern int DFSDsetlengths
    PROTO((int maxlen_label, int maxlen_unit, int maxlen_format,
                int maxlen_coordsys));
extern int DFSDsetdims
    PROTO((intn rank, int32 dimsizes[]));
extern int DFSDsetdatastrs
    PROTO((char *label, char *unit, char *format,char *coordsys));
extern int DFSDsetdimstrs
    PROTO((int dim, char *label, char *unit, char *format));
extern int DFSDsetdimscale
    PROTO((int dim, int32 dimsize, void *scale));
extern int DFSDsetrange
    PROTO((void *maxi, void *mini));
extern int DFSDputdata
    PROTO((char *filename, intn rank, int32 dimsizes[], void *data));
extern int DFSDadddata
    PROTO((char *filename, intn rank, int32 dimsizes[], void *data));
extern int DFSDrestart
    PROTO((void));
extern int32 DFSDnumber
    PROTO((char *filename));
extern int DFSDclear
    PROTO((void));
extern int DFSDlastref
    PROTO((void));
extern int DFSDreadref
    PROTO((char *filename, uint16 ref));
extern int DFSDgetslice
    PROTO((char *filename, int32 winst[], int32 windims[],void *data,
                int32 dims[]));
extern int DFSDstartslice
    PROTO((char *filename));
extern int DFSDputslice
    PROTO((int32 winend[], void *data, int32 dims[]));
extern int DFSDendslice
    PROTO((void));
extern int DFSDsetNT
    PROTO((int32 numbertype));
extern int DFSDsetorder
    PROTO((int arrayorder));
extern int DFSDgetNT
    PROTO((int32 *pnumbertype));
extern int DFSDgetsdg
    PROTO((int32 file_id, uint16 ref, DFSsdg *sdg));
extern int DFSDputsdg
    PROTO((int32 file_id, uint16 ref, DFSsdg *sdg));
extern int32 DFSDIopen
    PROTO((char *filename, int access));
extern int DFSDIsdginfo
    PROTO((int32 file_id));
extern int DFSDIclear
    PROTO((DFSsdg *sdg));
extern int DFSDIclearNT
    PROTO((DFSsdg *sdg));
extern int DFSDIgetdata
    PROTO((char *filename, intn rank, int32 maxsizes[], void *data,
                int isfortran));
extern int DFSDIputdata
    PROTO((char *filename, intn rank, int32 *dimsizes, void *data,
                int accmode, int isfortran));
extern int DFSDIgetslice
    PROTO((char *filename, int32 winst[], int32 windims[], void *data,
                int32 dims[], int isfortran));
extern int DFSDIputslice
    PROTO((int32 windims[], void *data, int32 dims[], int isfortran));
extern int DFSDIendslice
    PROTO((int isfortran));
extern intn DFSDIrefresh
    PROTO((char *filename));
extern int DFSDIisndg
    PROTO((int32 *isndg));
extern int DFSDIgetrrank
    PROTO((intn *rank));
extern int DFSDIgetwrank
    PROTO((intn *rank));

extern int DFSDsetcal
    PROTO((float64 cal, float64 cal_err, float64 ioff, 
           float64 ioff_err, int32 cal_nt));
extern int DFSDgetcal
    PROTO((float64 *pcal, float64 *pcal_err, float64 *pioff, 
               float64 *pioff_err, int32 *cal_nt));

#endif /*DFSDG*/
