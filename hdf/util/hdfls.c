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
Revision 1.3  1992/02/21 21:06:29  mfolk
no change

 * Revision 1.2  1992/02/11  17:16:24  chouck
 * Cosmetic changes.
 * Added new tag values
 *
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
#include "hdf.h"
#include "hfile.h"

#define MAXBUFF 2000

dd_t desc[MAXBUFF];

int
    debug=0,        /* Debugging is off by default */
    sort=1,         /* Sorting is on by default */
    longout=0;      /* short output by default */

struct des {
    int num;
    char name[50];
}   tagdes[]={
        {DFTAG_NULL, "Empty               (Utility)   "},

        {DFTAG_LINKED,  "Link to other block (Utility)"},
        {DFTAG_VERSION, "File Version        (Utility)   "},

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
        {DFTAG_SDD,  "SciData description (SciData)   "},
        {DFTAG_SD,   "Scientific Data     (SciData)   "},
        {DFTAG_SDS,  "SciData Scales      (SciData)   "},
        {DFTAG_SDL,  "SciData labels      (SciData)   "},
        {DFTAG_SDU,  "SciData units       (SciData)   "},
        {DFTAG_SDF,  "SciData formats     (SciData)   "},
        {DFTAG_SDM,  "SciData max/min     (SciData)   "}, 
        {DFTAG_SDC,  "SciData coordsys    (SciData)   "},
        {DFTAG_SDT,  "SciData transpose   (SciData)   "},

        {DFTAG_RLE,  "Run Length Encoding (Compression)"},
        {DFTAG_IMC,     "IMCOMP compression (Compression)"},
        {DFTAG_IMCOMP, "IMCOMP compression (Compression)"},

#ifdef VSDATATAG
        {VGDESCTAG,  "Vset Group Desc     (Vset)"},
        {VSDESCTAG,  "Vset Set Desc       (Vset)"},
        {VSDATATAG,  "Vset Data           (Vset)"},
#endif

	{-1,         "Unknown Tag                     "}
    };

int compare
  PROTO((struct DFdd *, struct DFdd *));
int main
  PROTO((int, char **));
int lprint
  PROTO((struct DFdd *, int));

#ifdef PROTOTYPE
int compare(dd_t *a, DFdesc *b)
#else
int compare(a, b)
dd_t *a,*b;
#endif /* PROTOTYPE */
{
    if (a->tag>b->tag) return(1);
    if (a->tag<b->tag) return(-1);
    if (a->ref>b->ref) return(1);
    if (a->ref<b->ref) return(-1);
    return(0);
}

#ifdef PROTOTYPE
main(int argc, char *argv[])
#else
main(argc, argv)
int argc;
char *argv[];
#endif /* PROTOTYPE */
{
    int32 fid;
    int i=1, j, n;
    filerec_t *file_rec;    /* file record */
    dd_t *desc;

    if (argc <2) {
        printf("%s,  version: 1.1   date: February 10, 1992\n",argv[0]);
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
        fid=Hopen( argv[i], DFACC_READ, -1);
        printf( "%s:  ", argv[i]);
        if (fid == FAIL) {
	  printf("Not an HDF file.\n");
	  i++;
	  continue;
	}

	file_rec = FID2REC(fid);
	
	/*
	  while{desc = file_rec->ddhead->ddlist) {
	  */
	desc = file_rec->ddhead->ddlist;
	n = file_rec->ddhead->ndds;
	
	if(debug) {
	  for (j=0; j<n; j++) {
	    printf("%d) %d[%d]",j,desc[j].tag,desc[j].ref);
	    printf(" off %ld len %ld\n", desc[j].offset,desc[j].length);
	  }
	}
	
	if (sort) qsort( (char *) desc, n, sizeof(dd_t), compare);
	
	lprint(desc, n);
	/*	
	}
	*/

        if (Hclose(fid) == FAIL) 
	  HEprint();

        i++;
        printf("\n");
    }
    return(0);              /* success */
}

#ifdef PROTOTYPE
int lprint(dd_t *desc, int num)
#else
int lprint(desc, num)
dd_t  *desc;
int num;
#endif /* PROTOTYPE */
{
  
  int i=0,j=0;
  int prev=0, empty=0;
  
  while (j <num) {
    if (desc[j].tag==DFTAG_NULL) {
      empty++;
      j++;
      continue;               /* don't print anything now */
    }

    /*
    ** Find and print text description of this tag
    */
    i=0;
    while ((uint16)tagdes[i].num!=desc[j].tag  && tagdes[i].num>0) i++;
    printf("\n%s:", tagdes[i].name);
    printf(" (tag %d)\n",desc[j].tag);

    /*
    ** Print out reference number information
    */
    prev=desc[j].tag;
    if(longout) {
      while (desc[j].tag==(uint16)prev && j<num) {
	printf("\t\tRef no %6d   %8ld bytes\n", desc[j].ref, desc[j].length);
	j++;
      }
    } else {
      printf("\tRef nos:");
      while (desc[j].tag==(uint16)prev && j<num) {
	printf(" %d",desc[j].ref);
	j++;
      }
    }
  }
  printf("\nEmpty (tag %d): %d slots\n", DFTAG_NULL, empty);
  return 0;

}
