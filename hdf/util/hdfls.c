/*****************************************************************************
* 
*			  NCSA HDF version 3.2beta
*			     February 29, 1992
*
* NCSA HDF Version 3.2 source code and documentation are in the public
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
Revision 1.9  1992/03/25 22:51:35  chouck
Fixed stupid bugs relating to unknown tags

 * Revision 1.8  1992/03/23  17:21:52  likkai
 * Put in description of "-d" option in "usage" message.
 * and redo the format for the output of that option.
 *
 * Revision 1.7  1992/03/18  17:49:26  chouck
 * Added Hendaccess() call so that Hclose() will not FAIL
 * ,
 *
 * Revision 1.6  1992/03/11  20:53:33  chouck
 * Use HDgettagname() to look up tag names.  Change the tag/ref look
 * up routines to use Hiquire() and Hnextread instead of DFdescriptors()
 *
 * Revision 1.5  1992/02/29  18:58:36  sxu
 * add header
 *
 * Revision 1.4  1992/02/26  17:46:31  likkai
 * added descriptions for Vset elements.
 *
 * Revision 1.3  1992/02/21  21:06:29  mfolk
 * no change
 *
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

int compare
  PROTO((dd_t *a, dd_t *));
int main
  PROTO((int, char **));
int lprint
  PROTO((dd_t *, int));

#ifdef PROTOTYPE
int compare(dd_t *a, dd_t *b)
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
    int32 fid, aid;
    int i=1, j, n, status;
    filerec_t *file_rec;    /* file record */
    dd_t desc[MAXBUFF];

    if (argc <2) {
        printf("%s,  version: 1.1   date: February 10, 1992\n",argv[0]);
        printf("hdfls [-o][-l] fn ....\n");
        printf("        This program displays information about the");
        printf(" data elements in\n");
        printf("        HDF file.\n");
        printf("    -d: offset & length info of each element in the file\n");
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
	  printf("\n\tNot an HDF file.\n");
	  i++;
	  continue;
	}

	aid = Hstartread(fid, DFTAG_WILDCARD, DFREF_WILDCARD);
	if(aid == FAIL) {
	  HEprint(stderr, 0);
	  i++;
	  continue;	  
	}

	status = SUCCEED;
	for(n = 0; (n < MAXBUFF) && (status != FAIL); n++) {
	  Hinquire(aid, NULL, &desc[n].tag, &desc[n].ref, &desc[n].length,
		   &desc[n].offset, NULL, NULL, NULL);
	  status = Hnextread(aid, DFTAG_WILDCARD, DFREF_WILDCARD, DF_CURRENT);
	}

	if(n == MAXBUFF) {
	  fprintf(stderr, 
		  "Warning:  File may have more DD's than hdfls can display\n");
	}

	if(debug) {
	  printf("\n");
	  for (j=0; j<n; j++) {
	    printf("%6d) tag %6d ref %6d ", j, desc[j].tag, desc[j].ref);
	    printf(" offset %10ld length %10ld\n", desc[j].offset, desc[j].length);
	  }
	}
	
	if (sort) qsort( (char *) desc, n, sizeof(dd_t), compare);
	
	lprint(desc, n);

	if(Hendaccess(aid) == FAIL) {
	  HEprint(stderr, 0);
	}

        if (Hclose(fid) == FAIL) {
	  HEprint(stderr, 0);
	}

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
  
  int j=0;
  int prev=0, empty=0;
  char *name;
  
  while (j <num) {
    if (desc[j].tag==DFTAG_NULL) {
      empty++;
      j++;
      continue;               /* don't print anything now */
    }

    /*
    ** Find and print text description of this tag
    */
    name = HDgettagname(desc[j].tag);
    if(!name) name = "Unknown Tag";
    printf("\n%-30s: (tag %d)\n", name, desc[j].tag);

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
  if(empty) printf("\nEmpty (tag %d): %d slots\n", DFTAG_NULL, empty);
  return 0;

}
