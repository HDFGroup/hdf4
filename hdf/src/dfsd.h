/***************************************************************************
*
*
*                         NCSA HDF version 3.2r3
*                            December 1, 1992
*
* NCSA HDF Version 3.2 source code and documentation are in the public
* domain.  Specifically, we give to the public domain all rights for future
* licensing of the source code, all resale rights, and all publishing rights.
*
* We ask, but do not require, that the following message be included in all
* derived works:
*
* Portions developed at the National Center for Supercomputing Applications at
* the University of Illinois at Urbana-Champaign, in collaboration with the
* Information Technology Institute of Singapore.
*
* THE UNIVERSITY OF ILLINOIS GIVES NO WARRANTY, EXPRESSED OR IMPLIED, FOR THE
* SOFTWARE AND/OR DOCUMENTATION PROVIDED, INCLUDING, WITHOUT LIMITATION,
* WARRANTY OF MERCHANTABILITY AND WARRANTY OF FITNESS FOR A PARTICULAR PURPOSE
*
****************************************************************************
*/
/*
$Header$

$Log$
Revision 1.6  1993/01/04 18:03:39  sxu
change dfsdpre32 to dfsdpre32sdg

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
    int32   HUGE *dimsizes;     /* dimensions of data */
    char    HUGE *coordsys;
    char    HUGE *dataluf[3];   /* label/unit/format of data */
    char    HUGE **dimluf[3];   /* label/unit/format for each dim */
    uint8   HUGE **dimscales;   /* scales for each dimension */
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
    uint8   *fill_value;       /* fill value if any specified  */
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
    PROTO((char HUGE *filename, intn HUGE *prank, int32 HUGE sizes[], intn maxrank));
extern int DFSDgetdatastrs
    PROTO((char HUGE *label, char HUGE *unit, char HUGE *format,char HUGE *coordsys));
extern int DFSDgetdimstrs
    PROTO((int dim, char HUGE *label, char HUGE *unit, char HUGE *format));
extern int DFSDgetdatalen
    PROTO((int HUGE *llabel, int HUGE *lunit, int HUGE *lformat,int HUGE *lcoordsys));
extern int DFSDgetdimlen
    PROTO((int dim, int HUGE *llabel, int HUGE *lunit, int HUGE *lformat));
extern int DFSDgetdimscale
    PROTO((int dim, int32 maxsize, void HUGE *scale));
extern int DFSDgetrange
    PROTO((void HUGE *pmax, void HUGE *pmin));
extern int DFSDgetdata
    PROTO((char HUGE *filename, intn rank, int32 HUGE maxsizes[], void HUGE *data));
extern int DFSDsetlengths
    PROTO((int maxlen_label, int maxlen_unit, int maxlen_format,
                int maxlen_coordsys));
extern int DFSDsetdims
    PROTO((intn rank, int32 HUGE dimsizes[]));
extern int DFSDsetdatastrs
    PROTO((char HUGE *label, char HUGE *unit, char HUGE *format,char HUGE *coordsys));
extern int DFSDsetdimstrs
    PROTO((int dim, char HUGE *label, char HUGE *unit, char HUGE *format));
extern int DFSDsetdimscale
    PROTO((int dim, int32 dimsize, void HUGE *scale));
extern int DFSDsetrange
    PROTO((void HUGE *maxi, void HUGE *mini));
extern int DFSDputdata
    PROTO((char HUGE *filename, intn rank, int32 HUGE dimsizes[], void HUGE *data));
extern int DFSDadddata
    PROTO((char HUGE *filename, intn rank, int32 HUGE dimsizes[], void HUGE *data));
extern int DFSDrestart
    PROTO((void));
extern int32 DFSDnumber
    PROTO((char HUGE *filename));
extern int DFSDclear
    PROTO((void));
extern int DFSDlastref
    PROTO((void));
extern int DFSDreadref
    PROTO((char HUGE *filename, uint16 ref));
extern int DFSDgetslice
    PROTO((char HUGE *filename, int32 HUGE winst[], int32 HUGE windims[],void HUGE *data,
                int32 HUGE dims[]));
extern int DFSDstartslice
    PROTO((char HUGE *filename));
extern int DFSDputslice
    PROTO((int32 HUGE winend[], void HUGE *data, int32 HUGE dims[]));
extern int DFSDendslice
    PROTO((void));
extern int DFSDsetNT
    PROTO((int32 numbertype));
extern int DFSDsetorder
    PROTO((int arrayorder));
extern int DFSDgetNT
    PROTO((int32 HUGE *pnumbertype));
extern intn DFSDpre32sdg
    PROTO((char HUGE *filename,uint16 ref, intn HUGE *ispre32));
extern int DFSDgetsdg
    PROTO((int32 file_id, uint16 ref, DFSsdg HUGE *sdg));
extern int DFSDputsdg
    PROTO((int32 file_id, uint16 ref, DFSsdg HUGE *sdg));
extern int32 DFSDIopen
    PROTO((char HUGE *filename, int access));
extern int DFSDIsdginfo
    PROTO((int32 file_id));
extern int DFSDIclear
    PROTO((DFSsdg HUGE *sdg));
extern int DFSDIclearNT
    PROTO((DFSsdg HUGE *sdg));
extern int DFSDIgetdata
    PROTO((char HUGE *filename, intn rank, int32 HUGE maxsizes[], void HUGE *data,
                int isfortran));
extern int DFSDIputdata
    PROTO((char HUGE *filename, intn rank, int32 HUGE *dimsizes, void HUGE *data,
                int accmode, int isfortran));
extern int DFSDIgetslice
    PROTO((char HUGE *filename, int32 HUGE winst[], int32 HUGE windims[], void HUGE *data,
                int32 HUGE dims[], int isfortran));
extern int DFSDIputslice
    PROTO((int32 HUGE windims[], void HUGE *data, int32 HUGE dims[], int isfortran));
extern int DFSDIendslice
    PROTO((int isfortran));
extern intn DFSDIrefresh
    PROTO((char HUGE *filename));
extern int DFSDIisndg
    PROTO((intn HUGE *isndg));
extern int DFSDIgetrrank
    PROTO((intn HUGE *rank));
extern int DFSDIgetwrank
    PROTO((intn HUGE *rank));

extern int DFSDsetcal
    PROTO((float64 cal, float64 cal_err, float64 ioff, 
           float64 ioff_err, int32 cal_nt));
extern int DFSDgetcal
    PROTO((float64 HUGE *pcal, float64 HUGE *pcal_err, float64 HUGE *pioff,
               float64 HUGE *pioff_err, int32 HUGE *cal_nt));

extern int DFSDwriteref
    PROTO((char *filename, uint16 ref));
extern int DFSDsetfillvalue
    PROTO((void *fill_value));
extern int DFSDgetfillvalue
    PROTO((void *fill_value));
extern int DFSDwritefillvalue
    PROTO((char *filename, uint16 ref));
extern int DFSDwriteslab
    PROTO((char *filename, int32 start[], stride[],
           int32count[], void *data));

#endif /*DFSDG*/
