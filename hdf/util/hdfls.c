#ifdef RCSID
static char RcsId[] = "@(#)$Revision$";
#endif
/*
$Header$

$Log$
Revision 1.29  1993/08/16 21:49:01  koziol
Wrapped in changes for final, working version on the PC.

 * Revision 1.28  1993/05/17  22:54:41  sxu
 * Wrapped in changes made in 3.2 and upgrade *.COM files for 3.3.
 *
 * Revision 1.27  1993/04/19  23:04:30  koziol
 * General Code Cleanup to reduce/remove compilation warnings on PC
 *
 * Revision 1.26  1993/04/13  21:40:09  georgev
 * Fixed cast problem for qsort on SGI's.
 *
 * Revision 1.25  1993/04/08  20:44:27  koziol
 * Minor Cray tweaks.
 *
 * Revision 1.24  1993/04/06  17:24:39  chouck
 * Added some fixes for the Vset stuff
 *
 * Revision 1.23  1993/04/05  22:38:23  koziol
 * Fixed goofups made in haste when patching code.
 *
 * Revision 1.22  1993/03/29  16:52:48  koziol
 * Finished  DEC ALPHA port.
 * Updated JPEG code to new JPEG 4 code.
 * Changed VSets to use Threaded-Balanced-Binary Tree for internal
 * 	(in memory) representation.
 * Changed VGROUP * and VDATA * returns/parameters for all VSet functions
 * 	to use 32-bit integer keys instead of pointers.
 * Backed out speedups for Cray, until I get the time to fix them.
 * Fixed a bunch of bugs in the little-endian support in DFSD.
 *
 * Revision 1.20  1993/01/19  06:00:14  koziol
 * Merged Hyperslab and JPEG routines with beginning of DEC ALPHA
 * port.  Lots of minor annoyances fixed.
 *
 * Revision 1.19  1992/07/15  21:48:48  sxu
 * No change
 *
 * Revision 1.18  1992/07/09  01:18:57  koziol
 * More PC fixes
 *
 * Revision 1.17  1992/07/08  15:39:42  chouck
 * Removed return(0) at end of main().  Took out some debugging
 * info from the -t option
 *
 * Revision 1.16  1992/06/19  16:45:58  chouck
 * More descriptive error messages when Hopen() fails
 *
 * Revision 1.15  1992/06/08  22:25:22  chouck
 * Minor fix with program name
 *
 * Revision 1.14  1992/06/08  21:59:41  chouck
 * Added 'verbose' option for labels/descriptions (-v) and
 * option (-t #) to only list info about a given tag
 *
 * Revision 1.13  1992/05/31  19:16:26  mfolk
 * No change.  But Convex doesn't like match of arg types in call to
 * qsort on line 220.
 *
 * Revision 1.12  1992/05/26  21:00:08  koziol
 * Folded Jason's Mac port and Linted code into the main version
 *
 * Revision 1.11  1992/04/24  15:38:25  koziol
 * PC port
 *
 * Revision 1.10  1992/03/27  15:39:28  chouck
 * Can now handle multiple command line options
 *
 * Revision 1.9  1992/03/25  22:51:35  chouck
 * Fixed stupid bugs relating to unknown tags
 *
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
#include "herr.h"

#ifdef MAC
/* this isn't worth putting in hdfi.h */
/* otherwise, need to include stdlib.h, which */
/* in turn requires other includes - very messy, trust me */ 
void qsort(void *base, size_t nmemb, size_t size, 
        int (*compar) (const void*, const void *));
#endif

#ifdef PC
#define MAXBUFF 2048
#else
#define MAXBUFF 8192
#endif

dd_t desc[MAXBUFF];

intn
    debug    = FALSE,        /* Debugging is off by default */
    sort     = TRUE,         /* Sorting is on by default */
    longout  = FALSE,        /* short output by default */
    labels   = FALSE;        /* no label info by default */
uint16  only_tag = DFTAG_NULL;  /* by default print info about all tags */

char * file_name;    /* name of current file being listed */

int compare
  PROTO((const VOID *, const VOID *));
int main
  PROTO((int, char **));
int lprint
  PROTO((dd_t *, int));

#ifdef PROTOTYPE
int compare(const VOID * aa, const VOID * bb)
#else
int compare(aa, bb)
const VOIDP aa, bb;
#endif /* PROTOTYPE */
{
    dd_t *a = (dd_t *) aa;
    dd_t *b = (dd_t *) bb;
    
    if(a->tag > b->tag) return(1);
    if(a->tag < b->tag) return(-1);
    if(a->ref > b->ref) return(1);
    if(a->ref < b->ref) return(-1);
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
    
    while((i < argc) && (argv[i][0]=='-')){
        switch(argv[i][1]) {
        case 'o':
            sort = 0;
            break;
        case 'd':
            debug = TRUE;
            break;
        case 'v':
            labels = TRUE;
            /* fall through... */
        case 'l':
            longout = TRUE;
            break;
        case 't' :
            if(argv[i][2] != '\0') 
                only_tag = (uint16)atoi(&(argv[i][2]));
            else 
                only_tag = (uint16)atoi(&(argv[++i][0]));
            break;
        default:    
            printf("Unknown option : -%c\n", argv[1][1]);
            break;
        }
        i++;
    }
    
    /*
     * If a file name has not been supplied print the usage message
     */
    if(i == argc) {
        printf("%s,  version: 1.2   date: June 8, 1992\n",argv[0]);
        printf("%s [-o] [-l] [-d] [-v] [-t #] fn ....\n", argv[0]);
        printf("        This program displays information about the");
        printf(" data elements in\n");
        printf("        HDF file.\n");
        printf("    -d: offset & length info of each element in the file\n");
        printf("    -o: Ordered - display in reference number order\n");
        printf("    -l: Long format - display more information\n");
        printf("    -v: Verbose format - display text of annotations and labels.\n");
        printf("        (Verbose format automatically puts you in Long format).\n");
        printf("    -t #: List only information about a specific type of tag.\n");
        printf("          For example '%s -t 700 foo.hdf' \n", argv[0]);
        printf("          will list information only about Scientific Data\n");
        printf("          Groups.\n");
        exit (1);
    }
    
    while(i < argc) {
        file_name = argv[i];
        fid = Hopen(file_name, DFACC_READ, -1);
        printf( "%s:  ", argv[i]);
        if(fid == FAIL) {
            if(HEvalue(1) == DFE_NOTDFFILE) {
                printf("\n\tNot an HDF file.\n");
                i++;
                continue;
            } else {
                printf("\n");
                fflush(stdout);
                HEprint(stderr, 0);
            }
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
	
	if (sort) qsort( (char *)desc, n, sizeof(dd_t), compare);
	
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
    return(0);
}

#ifdef PROTOTYPE
int lprint(dd_t *desc, int num)
#else
int lprint(desc, num)
dd_t  *desc;
int num;
#endif /* PROTOTYPE */
{
  
  intn j = 0, empty = 0, status;
  uint16 prev = 0;
  int32 len;
  char *name, *label_str;
  
  while (j <num) {
    if (desc[j].tag == DFTAG_NULL) {
      empty++;
      j++;
      continue;               /* don't print anything now */
    }

    /* 
     * skip this tag if the user only wants to see some tags and
     *  this is not one of them 
     */
    if(only_tag != DFTAG_NULL && only_tag != desc[j].tag) {
        j++;
        continue;
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
    prev = desc[j].tag;
    if(longout) {
      while (desc[j].tag == prev && j < num) {
	printf("\tRef no %6d\t%8ld bytes\n", desc[j].ref, desc[j].length);
        if(labels) {
            /* read in all of the labels */
            len = DFANgetlablen(file_name, prev, desc[j].ref);
            if(len != FAIL) {
                label_str = (char *) HDgetspace((uint32) len + 1);
                status = DFANgetlabel(file_name, prev, desc[j].ref, label_str, len + 1);
                label_str[len] = '\0';
                if(status == FAIL) 
                    printf("\t  Unable to read label\n");
                else
                    printf("\t  Label: %s\n", label_str);
                HDfreespace(label_str);
            }

            /* read in all of the annotations */
            len = DFANgetdesclen(file_name, prev, desc[j].ref);
            if(len != FAIL) {
                label_str = (char *) HDgetspace((uint32) len + 1);
                status = DFANgetdesc(file_name, prev, desc[j].ref, label_str, len + 1);
                label_str[len] = '\0';
                if(status == FAIL) 
                    printf("\t  Unable to read description\n");
                else
                    printf("\t  Description: %s\n", label_str);
                HDfreespace(label_str);
            }
        }
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
