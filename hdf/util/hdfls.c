/*****************************************************************************
* 
*			  NCSA HDF version 3.10r5
*			     October 24, 1991
*
* NCSA HDF Version 3.10r5 source code and documentation are in the public
* domain.  Specifically, we give to the public domain all rights for future
* licensing of the source code, all resale rights, and all publishing rights.
* 
* We ask, but do not require, that the following message be included in all
* derived works:
* 
* Portions developed at the National Center for Supercomputing Applications at
* the University of Illinois at Urbana-Champaign.
* 
* THE UNIVERSITY OF ILLINOIS GIVES NO WARRANTY, EXPRESSED OR IMPLIED, FOR THE
* SOFTWARE AND/OR DOCUMENTATION PROVIDED, INCLUDING, WITHOUT LIMITATION,
* WARRANTY OF MERCHANTABILITY AND WARRANTY OF FITNESS FOR A PARTICULAR PURPOSE
* 
*****************************************************************************/
#ifdef RCSID
static char RcsId[] = "@(#)$Revision$";
#endif
/*
$Header$

$Log$
Revision 1.1  1992/02/11 17:15:23  chouck
Initial revision

 * Revision 3.3  1991/10/22  17:56:10  dilg
 * 5
 * HDF3.1r5
 *
 * New machine types added:
 *
 *         PC      - IBM PC (DOS)
 *         WIN     - IBM PC (Microsoft Windows 3.0)
 *         IBM6000 - IBM RS/6000 (AIX)
 *         CONVEX  - Convex C-2 (Unix)
 *
 * Bugs fixed in:
 *
 *         scup32.f
 *         cspck32.f
 *         dfpFf.f
 *         dfpF.c
 *         dfsd.c
 *
 * New utility added:
 *
 *         ristosds.c - convert raster images to sds.
 *
 * Also:
 *         All code for the library was modified to conform to the
 *         ANSI C standard.
 *
 * Revision 3.2  1990/07/02  10:11:46  clow
 * some cosmetic modifications
 *
*/
#include "df.h"

#define MAXBUFF 2000

DFdesc desc[MAXBUFF];

int
    debug=0,        /* Debugging is off by default */
    sort=1,         /* Sorting is on by default */
    longout=0;      /* short output by default */

struct des {
    int num;
    char name[50];
}   tagdes[]={
        {DFTAG_NULL, "Empty               (Utility)   "},
        {DFTAG_FID,  "File Identifier     (Utility)   "},
        {DFTAG_FD,   "File Description    (Utility)   "},
        {DFTAG_TID,  "Tag Identifier      (Utility)   "},
        {DFTAG_TD,   "Tag Description     (Utility)   "},
        {DFTAG_DIL,  "Data Id Label       (Utility)   "},
        {DFTAG_DIA,  "Data Id Annotation  (Utility)   "},
        {DFTAG_NT,   "Number type         (Utility)   "},
        {DFTAG_MT,   "Machine type        (Utility)   "},
        {DFTAG_ID8,  "Image Dimensions-8  (Raster-8)  "},
        {DFTAG_IP8,  "Image Palette-8     (Raster-8)  "},
        {DFTAG_RI8,  "Raster Image-8      (Raster-8)  "},
        {DFTAG_CI8,  "Compressed Image-8  (Raster-8)  "},
        {DFTAG_II8,  "Imcomp Image-8      (Raster-8)  "},
        {DFTAG_ID,   "Image Dimensions    (Raster)    "},
        {DFTAG_LUT,  "Image Palette       (Raster)    "},
        {DFTAG_RI,   "Raster Image        (Raster)    "},
        {DFTAG_CI,   "Compressed Image    (Raster)    "},
        {DFTAG_RIG,  "Raster Image Group  (Raster)    "},
        {DFTAG_LD,   "Palette Dimension   (Raster)    "},
        {DFTAG_MD,   "Matte Dimension     (Raster)    "},
        {DFTAG_MA,   "Matte Data          (Raster)    "},
        {DFTAG_CCN,  "Color Correction    (Raster)    "},
        {DFTAG_CFM,  "Color Format        (Raster)    "},
        {DFTAG_AR,   "Aspect Ratio        (Raster)    "},
        {DFTAG_DRAW, "Draw                (Composite) "},
        {DFTAG_RUN,  "Run                 (Composite) "},
        {DFTAG_XYP,  "X-Y position        (Composite) "},
        {DFTAG_MTO,  "M/c-Type override   (Composite)"},
        {DFTAG_T14,  "TEK 4014 Data       (Tektronix) "},
        {DFTAG_T105, "TEK 4105 data       (Tektronix) "},
        {DFTAG_RLE,  "Run Length Encoding (Raster)    "},
        {DFTAG_IMC,  "IMCOMP Encoding     (Raster)    "},
        {DFTAG_SDG,  "Scientific Data Group (SciData) "},
        {DFTAG_SD,   "Scientific Data     (SciData)   "},
        {DFTAG_SDD,  "SciData description (SciData)   "},
        {DFTAG_SDL,  "SciData labels      (SciData)   "},
        {DFTAG_SDU,  "SciData units       (SciData)   "},
        {DFTAG_SDF,  "SciData formats     (SciData)   "},
        {DFTAG_SDS,  "SciData scales      (SciData)   "},
        {DFTAG_SDM,  "SciData max/min     (SciData)   "},
        {DFTAG_SDC,  "SciData coordsys    (SciData)   "},
        {-1,         "Unknown Tag                     "}
    };

#if defined __STDC__ || defined PC
int compare(struct DFdd *, struct DFdd *);
int main(int, char **);
int lprint(struct DFdd *, int);
#else
int compare();
int main();
int lprint();
#endif /* __STDC__ || PC */

#if defined __STDC__ || defined PC
int compare(DFdesc *a, DFdesc *b)
#else
int compare(a, b)
DFdesc *a,*b;
#endif /* __STDC__ || PC */
{
    if (a->tag>b->tag) return(1);
    if (a->tag<b->tag) return(-1);
    if (a->ref>b->ref) return(1);
    if (a->ref<b->ref) return(-1);
    return(0);
}

#if defined __STDC__ || defined PC
main(int argc, char *argv[])
#else
main(argc, argv)
int argc;
char *argv[];
#endif /* __STDC__ || PC */
{
    DF *df;
    int i=1, j, n;

    if (argc <2) {
        printf("%s,  version: 1.0   date: December 1, 1988\n",argv[0]);
        printf("hdfls [-o][-l] fn ....\n");
        printf("        This program displays information about the");
        printf(" data elements in\n");
        printf("        HDF file.\n");
        printf("    -o: Ordered - display in reference number order\n");
        printf("    -l: Long format - display more information\n");
        exit (1);
        }
    
    if (argv[1][0]=='-') {
        switch( argv[1][1]) {
            case 'o':
                sort=0;
                break;
            case 'd':
                debug=1;
                break;
            case 'l':
                longout = 1;
                break;
            default:    
                printf("Unknown option :%c\n", argv[1][1]);
                break;
            }
        i++;
        }
    while (i<argc) {
        df=DFopen( argv[i], DFACC_READ, -1);
        printf( "%s:", argv[i]);
        if (!df) {
            if (DFerror==DFE_NOTDFFILE) 
                    printf(" not an NCSA Format Data File.\n");
            else printf(" Error opening file.\n");
            i++;
            continue;
            }

        n = DFdescriptors(df, desc, 0, MAXBUFF);

        for (j=0; j<n; j++) {
            if (debug) printf("%d) %d[%d]",j,desc[j].tag,desc[j].ref);
            if (debug) printf(" off %ld len %ld\n",
                    desc[j].offset,desc[j].length);
            }

        if (sort) qsort( (char *) desc, n, sizeof(DFdesc), compare);

        lprint(desc, n);

        if (DFclose(df)<0) printf("Error %d @ close\n", DFerror);
        i++;
        printf("\n");
    }
    return(0);              /* success */
}

#if defined __STDC__ || defined PC
lprint(DFdesc *desc, int num)
#else
lprint(desc, num)
DFdesc  *desc;
int num;
#endif /* __STDC__ || PC */
{
    
    int i=0,j=0;
    int prev=0, empty=0;

    while (j <num) {
        if (desc[j].tag==DFTAG_NULL) {
            empty++;
            j++;
            continue;               /* don't print anything now */
        }
        i=0;
        while ((uint16)tagdes[i].num!=desc[j].tag  && tagdes[i].num>0) i++;
        printf("\n%s:", tagdes[i].name);
        printf(" (tag %d)\n",desc[j].tag);
        if (!longout) printf("\tRef nos:");
        prev=desc[j].tag;
        while (desc[j].tag==(uint16)prev && j<num) {
            if (longout)
                printf("\t\tRef no %6d   %8ld bytes\n", desc[j].ref,
                                                            desc[j].length);
            else {
                printf(" %d",desc[j].ref);
            }
            j++;
        }
    }
    printf("\nEmpty (tag %d): %d slots\n",DFTAG_NULL, empty);
    return(0);
}
