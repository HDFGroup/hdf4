/*
$Header$

$Log$
Revision 1.1  1992/09/04 21:25:52  chouck
Initial revision

*/
/*-----------------------------------------------------------------------------
 * File:    dfnd.h
 * Purpose: header file for the Scientific Data set
 * Updated with:
 * Revision 1.11
 * Invokes: hdf.h hfile.h
 * Contents: 
 *  Structure definitions: DFSsdg, DFdi, Stat, DFnsdgle, DFnsdg_t_hdr
 *  Constant definitions: DFS_MAXLEN
 *  Prototypes for: dfnd.c dfndF.c dfndFf.f
 *  Included in: dfnd.c dfndF.c dfsd.c dfsdF.c
 *---------------------------------------------------------------------------*/


#ifndef DFNDG                       /* avoid re-inclusion */
#define DFNDG

#include "hdf.h"
#include "hfile.h"

typedef struct {                /* DFdi = data identifier = tag/ref */
    uint16 tag, ref;
} DFdi;

#define DFS_MAXLEN 255          /*  Max length of label/unit/format strings */
#define DF_NOFILE -1

/* include numbertype and aid for 3.2   S. Xu   */
/* structure to hold SDG info */
typedef struct DFSsdg {
    DFdi    data;             /* tag/ref of data in file */
    intn    rank;             /* number of dimensions */
    int32   *dimsizes;        /* dimensions of data */
    char    *coordsys;
    char    *dataluf[3];      /* label/unit/format of data */
    char    **dimluf[3];      /* label/unit/format for each dim */
    uint8   **dimscales;      /* scales for each dimension */
    uint8   max_min[16];      /* max, min values of data, */
                              /* currently atmost 8 bytes each  */
    int32   numbertype;       /* default is float32      */
    uint8   filenumsubclass;  /* number format in the file, default is IEEE */
    int32   aid;              /* access id              */
    int32   compression;      /* 0 -- not compressed  */
    int32   isndg;            /* 0 -- pure sdg, written by 3.1   */
                              /* 1 -- ndg or nsdg, written by 3.2   */
} DFSsdg;

/* DFnsdgle is the internal structure which stores SDG or NDS and   */
/* related SDG in an HDF file.                                      */
/* It is a linked list.                                             */

typedef struct DFnsdgle   {
    DFdi nsdg;                  /* NDG from 3.2 or SDG from 3.1  */
    DFdi sdg;                   /* Only special NDF has values in this field */
    struct DFnsdgle *next;
}  DFnsdgle;

typedef struct DFnsdg_t_hdr     {
    uint32 size;
    DFnsdgle *nsdg_t;
}  DFnsdg_t_hdr;

typedef struct status_rec {	/* Indicators of status (s) of info:    */
    int dims;			/* s = -1: there is no info in this category */
    int nt;             /* s = 0: info was set, but not yet written */
    int coordsys;       /* s > 0: info was set and written with ref no.s*/
    int luf[3];
    int scales;
    int maxmin;
    int transpose;	/* It should be taken out!!!		!!!  */
} Stat;

typedef int32 NDGID;  /* what DFNDopen returns */

/* These really shouldn't be so public, but I can't think of any way to get
   around it.  The problem is that the Fortran stubs for things dealing with
   dimensions need the rank, so they call DFNDIget[wr]rank, and whether this
   is an NDG/SDG or a pure SDG, or whatever, so they call DFNDIisndg.

   All three of these routines work on slots, not file ids, because sometimes
   they are called from dfsdF.c, and there is no current id, the slot is
   MAX_FILE, the reserved slot for the DFSD interface.  Trouble is that means
   that those Fortran stubs who DO have an id must translate it with the macro
   NDGIS2SLOT, so I put all the macros here.
*/
/* macros for creating and managing NDG IDs */
#define VALIDNDGID(i) (((((uint32)(i) >> 16) & 0xffff) == NDGIDTYPE) && \
                       (((uint32)(i) & 0xffff) < MAX_FILE))
#define NDGSLOT2ID(s) (( (uint32) NDGIDTYPE & 0xffff) << 16 | ((s) & 0xffff) )
#define NDGID2SLOT(i) ( VALIDNDGID(i) ? (uint32) (i) & 0xffff : -1)
#define NDGID2REC(id) ndrec[NDGID2SLOT(id)]
#define NDGSLOT2REC(s) ndrec[(s)]

/* constants for the status flags from DFSD -> DFND tracking of info */
#define DFSDCALL 0x01  /* Is this a DFSD call? */
#define DFSDCHECKSET 0x02 /* check whether the Writesdg slot is allocated? */
#define DFSDBUILDNSDGT 0x04 /* force rebuilding of nsdg table? */

#ifdef OLD_WAY
/*----------------------------------------------------------------------------*/
/*                           Function Prototypes                              */

#ifdef __STDC__

#if defined c_plusplus || defined __cplusplus
extern "C" {
#endif /* c_plusplus || __cplusplus */

/* prototypes for dfndF.c */

extern int FCALLKEYW dngdast_(NDGID *id, char *label, char **unit,
                              char **format, char **coordsys);
extern int FCALLKEYW dngdist_(NDGID *id, int *dim, char *label, char **unit,
                              char **format);
extern int FCALLKEYW dngdisc_(NDGID *id, int *dim, int *maxsize, void *scale);
extern int FCALLKEYW dngrang_(NDGID *id, void *pmax, void *pmin);
extern int FCALLKEYW dnsdims_(NDGID *id, int32 *rank, int dimsizes[]);
extern int FCALLKEYW dnsdisc_(NDGID *id, int *dim, int *dimsize, void *scale);
extern int FCALLKEYW dnsrang_(NDGID *id, void *max, void *min);
extern int FCALLKEYW dnclear_(NDGID *id);
extern int FCALLKEYW dnslens_(NDGID *id, int *maxlen_label, int *maxlen_unit,
                              int *maxlen_format, int *maxlen_coordsys);
extern int FCALLKEYW dngdiln_(NDGID *id, int *dim, int *llabel, int *lunit,
                              int *lformat);
extern int FCALLKEYW dngdaln_(NDGID *id, int *llabel, int *lunit, int *lformat,
                              int *lcoordsys);
extern int FCALLKEYW dnfirst_(NDGID *id);
extern int FCALLKEYW dnpslc_(NDGID *id, int32 windims[], void *data,
                             int32 dims[]);
extern int FCALLKEYW dneslc_(NDGID *id);
extern int FCALLKEYW dnsnt_(NDGID *id, int32 *numbertype);
extern int FCALLKEYW dngnt_(NDGID *id, int32 *pnumbertype);
extern int FCALLKEYW dnigdim_(NDGID *id, int32 *prank, int sizes[],
                              int *maxrank);
extern int FCALLKEYW dnigdat_(NDGID *id, int32 *rank, int maxsizes[],
                              void *data);
extern int FCALLKEYW dniadat_(NDGID *id, int32 *rank, int dimsizes[],
                              void *data);
extern int FCALLKEYW dnigslc_(NDGID *id, int32 winst[], int32 windims[],
                              void *data, int32 dims[]);
extern int FCALLKEYW dnisslc_(NDGID *id);
extern int FCALLKEYW dnirref_(NDGID *id, int *ref);
extern int FCALLKEYW dnlref_(NDGID *id);
extern int FCALLKEYW dnnum_(NDGID *id);
extern int FCALLKEYW dniopen_(char *filename, int32 access, int32 num_dds,
                              int32 *fnlen);
extern int FCALLKEYW dnisdas_(NDGID *id, char *flabel, char *funit,
                    char *fformat, char *fcoordsys, int *isfortran,
                    int *llabel, int *lunit, int *lformat, int *lcoordsys);
extern int FCALLKEYW dnisdis_(NDGID *id, int *dim, char *flabel, char *funit,
                              char *fformat,
                    int *isfortran, int *llabel, int *lunit, int *lformat);
extern int FCALLKEYW dnclose_(NDGID *id, int32 fileid);
extern int FCALLKEYW dfndgetdatastrs_(NDGID *id, _fcd *label, _fcd *unit,
                                      _fcd *format, _fcd *coordsys);
extern int FCALLKEYW dfndgetdimstrs_(NDGID *id, int *dim, _fcd *label,
                                     _fcd *unit, _fcd *format);
extern int FCALLKEYW dfndgetdimscale_(NDGID *id, int *dim, int *maxsize,
                                      void *scale);
extern int FCALLKEYW dfndgetrange_(NDGID *id, void *pmax, void *pmin);
extern int FCALLKEYW dfndsetdims_(NDGID *id, int32 *rank, int dimsizes[]);
extern int FCALLKEYW dfndsetdimscale_(NDGID *id, int *dim, int *dimsize,
                                      void *scale);
extern int FCALLKEYW dfndsetrange_(NDGID *id, void *max, void *min);
extern int FCALLKEYW dfndclear_(NDGID *id);
extern int FCALLKEYW dfndsetlengths_(NDGID *id, int *maxlen_label,
                  int *maxlen_unit, int *maxlen_format, int *maxlen_coordsys);
extern int FCALLKEYW dfndgetdimlen_(NDGID *id, int *dim, int *llabel,
                                    int *lunit, int *lformat);
extern int FCALLKEYW dfndgetdatalen_(int *llabel, int *lunit, int *lformat,
                                     int *lcoordsys);
extern int FCALLKEYW dfndrestart_(NDGID *id);
extern int FCALLKEYW dfndputslice_(NDGID *id, int32 windims[], void *data,
                                   int32 dims[]);
extern int FCALLKEYW dfndendslice_(NDGID *id);
extern int FCALLKEYW dfndsetnt_(NDGID *id, int32 *numbertype);
extern int FCALLKEYW dfndgetnt_(NDGID *id, int32 *pnumbertype);
extern int FCALLKEYW dfndlastref_(NDGID *id);
extern int FCALLKEYW dfndnumber_(NDGID *id);
extern int FCALLKEYW dfndclose(int32 id);

#if defined c_plusplus || defined __cplusplus
}
#endif /* c_plusplus || __cplusplus */

#else /* !__STDC__ */

/* prototypes for dfndF.c */

extern int dnisdas_();
extern int dnisdis_();
extern int dngdast_();
extern int dngdist_();
extern int dngdisc_();
extern int dngrang_();
extern int dnsdims_();
extern int dnsdisc_();
extern int dnsrang_();
extern int dnclear_();
extern int dnslens_();
extern int dngdiln_();
extern int dngdaln_();
extern int dnfirst_();
extern int dnpslc_();
extern int dneslc_();
extern int dnsnt_();
extern int32 dngnt_();
extern int dnigdim_();
extern int dnigdat_();
extern int dnipdat_();
extern int dniadat_();
extern int dnigslc_();
extern int dnisslc_();
extern int dnirref_();
extern int dnlref_();
extern int dniopen_();
extern int dnclose_();
extern int dfndgetdatastrs_();
extern int dfndgetdimstrs_();
extern int dfndgetdimscale_();
extern int dfndgetrange_();
extern int dfndsetdims_();
extern int dfndsetdimscale_();
extern int dfndsetrange_();
extern int dfndclear_();
extern int dfndsetlengths_();
extern int dfndgetdimlen_();
extern int dfndgetdatalen_();
extern int dfndrestart_();
extern int dfndputslice_();
extern int dfndendslice_();
extern int dfndsetnt_();
extern int32 dfndgetnt_();
extern int dfndlastref_();
extern int dfndclose_();

#endif /* __STDC__ */
#else   /* !OLD_WAY */

extern int DFNDgetdims
    PROTO((NDGID id, intn *prank, int32 sizes[], intn maxrank));
extern int DFNDgetdatastrs
    PROTO((NDGID id, char *label, char *unit, char *format, char *coordsys));
extern int DFNDIgetdatastrs
    PROTO((int slot, char *label, char *unit, char *format, char *coordsys));
extern int DFNDgetdimstrs
    PROTO((NDGID id, int dim, char *label, char *unit, char *format));
extern int DFNDIgetdimstrs
    PROTO((int slot, int dim, char *label, char *unit, char *format));
extern int DFNDgetdatalen
    PROTO((NDGID id, int *llabel, int *lunit, int *lformat, int *lcoordsys));
extern int DFNDIgetdatalen
    PROTO((int slot, int *llabel, int *lunit, int *lformat, int *lcoordsys));
extern int DFNDgetdimlen
    PROTO((NDGID id, int dim, int *llabel, int *lunit, int *lformat));
extern int DFNDIgetdimlen
    PROTO((int slot, int dim, int *llabel, int *lunit, int *lformat));
extern int DFNDgetdimscale
    PROTO((NDGID id, int dim, int32 maxsize, void *scale));
extern int DFNDIgetdimscale
    PROTO((int slot, int dim, int32 maxsize, void *scale));
extern int DFNDgetrange
    PROTO((NDGID id, void *pmax, void *pmin));
extern int DFNDIgetrange
    PROTO((int slot, void *pmax, void *pmin));
extern int DFNDgetdata
    PROTO((NDGID id, intn rank, int32 maxsizes[], void *data));
extern int DFNDsetlengths
    PROTO((NDGID id, int maxlen_label, int maxlen_unit, int maxlen_format,
                int maxlen_coordsys));
extern int DFNDIsetlengths
    PROTO((int slot, int maxlen_label, int maxlen_unit, int maxlen_format,
           int maxlen_coordsys, int flags));
extern int DFNDsetdims
    PROTO((NDGID id, intn rank, int32 dimsizes[]));
extern int DFNDIsetdims
    PROTO((int slot, intn rank, int32 dimsizes[], int flags));
extern int DFNDsetdatastrs
    PROTO((NDGID id, char *label, char *unit, char *format, char *coordsys));
extern int DFNDIsetdatastrs
    PROTO((int slot, char *label, char *unit, char *format, char *coordsys,
           int flags));
extern int DFNDsetdimstrs
    PROTO((NDGID id, int dim, char *label, char *unit, char *format));
extern int DFNDIsetdimstrs
    PROTO((int slot, int dim, char *label, char *unit, char *format,
           int flags));
extern int DFNDsetdimscale
    PROTO((NDGID id, int dim, int32 dimsize, void *scale));
extern int DFNDIsetdimscale
    PROTO((int slot, int dim, int32 dimsize, void *scale, int flags));
extern int DFNDsetrange
    PROTO((NDGID id, void *maxi, void *mini));
extern int DFNDIsetrange
    PROTO((int slot, void *maxi, void *mini, int flags));
extern int DFNDadddata
    PROTO((NDGID id, intn rank, int32 dimsizes[], void *data));
extern int DFNDrestart
    PROTO((NDGID id));
extern int DFNDIrestart
    PROTO((int slot, int flags));
extern int32 DFNDnumber
    PROTO((NDGID id));
extern int DFNDclear
    PROTO((NDGID id));
extern int DFNDIstartclear
    PROTO((int slot));
extern int DFNDlastref
    PROTO((NDGID id));
extern int DFNDIlastref
    PROTO((int slot));
extern int DFNDreadref
    PROTO((NDGID id, uint16 ref));
extern int DFNDgetslice
    PROTO((NDGID id, int32 winst[], int32 windims[],void *data,int32 dims[]));
extern int DFNDstartslice
    PROTO((NDGID id));
extern int DFNDputslice
    PROTO((NDGID id, int32 winend[], void *data, int32 dims[]));
extern int DFNDendslice
    PROTO((NDGID id));
extern int DFNDsetNT
    PROTO((NDGID id, int32 numbertype));
extern int DFNDIsetNT
    PROTO((int slot, int32 numbertype, int flags));
extern int DFNDgetNT
    PROTO((NDGID id, int32 *pnumbertype));
extern int DFNDIgetNT
    PROTO((int slot, int32 *pnumbertype));
extern int DFNDgetsdg
    PROTO((NDGID id, uint16 ref, DFSsdg *sdg));
extern NDGID DFNDopen
    PROTO((char *filename, int access, int16 num_dds));
extern NDGID DFNDIopen
    PROTO((char *filename, int access, int16 num_dds, int flags));
extern int DFNDclose
    PROTO((NDGID id));
extern int DFNDIinit
    PROTO((int slot, int access));
extern int DFNDinitsd
    PROTO((void));
extern int DFNDputsdg
    PROTO((NDGID id, uint16 ref, DFSsdg *sdg));
extern int DFNDIsdginfo
    PROTO((NDGID id));
extern int DFNDIclear
    PROTO((int slot, DFSsdg *sdg));
extern int DFNDIclearNT
    PROTO((int slot, DFSsdg *sdg));
extern int DFNDIgetdata
    PROTO((NDGID id, intn rank, int32 maxsizes[], void *data, int isfortran));
extern int DFNDIputdata
    PROTO((NDGID id, intn rank, int32 *dimsizes, void *data, int isfortran));
extern int DFNDIgetslice
    PROTO((NDGID id, int32 winst[], int32 windims[], void *data,
           int32 dims[], int isfortran));
extern int DFNDIputslice
    PROTO((NDGID id, int32 windims[], void *data, int32 dims[],
           int isfortran));
extern int DFNDIendslice
    PROTO((NDGID id, int isfortran));
extern int DFNDIisndg
    PROTO((int slot, int32 *isndg));
extern int DFNDIgetrrank
    PROTO((int slot, intn *rank));
extern int DFNDIgetwrank
    PROTO((int slot, intn *rank));

#endif  /* OLD_WAY */

#endif /* DFNDG */
