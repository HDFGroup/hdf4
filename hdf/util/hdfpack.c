/****************************************************************************
 * NCSA HDF                                                                 *
 * Software Development Group                                               *
 * National Center for Supercomputing Applications                          *
 * University of Illinois at Urbana-Champaign                               *
 * 605 E. Springfield, Champaign IL 61820                                   *
 *                                                                          *
 * For conditions of distribution and use, see the accompanying             *
 * hdf/COPYING file.                                                        *
 *                                                                          *
 ****************************************************************************/

#ifdef RCSID
static char RcsId[] = "@(#)$Revision$";
#endif

/* $Id$ */

/*
** FILE
**	hdfpack.c
** USAGE
**	hdfpack [options] <infile> <outfile>
** DESCRIPTION
**	This program will compact an HDF file by reading in all the data
**	elements in the file and writing them out to a new file.
**      Options are:
**          -b  Don't coalesce linked-block elements
**          -i  Interactive mode; prompt for each linked-block element
**          -d# Force the output file to have # DDs per DD block
**          -t# Force output file to have # linked blocks per table entry
**      Only one of options b and i can be specified.
** COMMENTS, BUGS, ASSUMPTIONS
**	Both arguments must be supplied to the program and they cannot be
**	identical.
**	You must have enough additional disk space for the compacted file.
** AUTHOR
**	Doug Ilg
*/

#include <stdio.h>
#include <string.h>
#include "hdf.h"
#include "herr.h"

typedef struct mydd_t {
    uint16 tag;
    uint16 ref;
    int32 offset;
    int32 length;
    int16 special;
}mydd_t;

/* Static function prototypes */
int promptblocks
    PROTO((mydd_t *dd));

VOID copy_blocks
    PROTO((mydd_t *dd, int32 infile, int32 outfile));

VOID merge_blocks
    PROTO((mydd_t *dd, int32 infile, int32 outfile));

int main
    PROTO((int, char **));
static VOID usage
    PROTO((char *));
static VOID hdferror
    PROTO((void));
static VOID error
    PROTO((char *));
int desc_comp
    PROTO((const void *d1, const void *d2));

unsigned char *data;
char invoke[81];
int32 data_size;
int32 nblk = 0;

#ifdef PROTOTYPE
main(int argc, char *argv[])
#else
main(argc, argv)
int argc;
char *argv[];
#endif /* PROTOTYPE */
{
    int i, num_desc, ret, fnum, merge;
    int32 infile, outfile, aid, stat;
    mydd_t *dlist;
    int32 oldoff, oldlen;
    int blocks = 1;
    int intr = 0;
    int16 ndds = 0;
    int optset = 0;
    char *tmp, fname[2][80];
    char *FUNC="main";

/*
**   Get invocation name of program
*/
    tmp = (char *)NULL;
    strcpy(invoke, strtok(argv[0], "/]\\\0"));
    for (;;) {
	if (tmp != NULL)
	    strcpy(invoke, tmp);
	if ((tmp = strtok((char *)NULL, "/]\\\0")) == NULL)
	    break;
    }

    if (argc < 3) {
        usage(invoke);
        exit(1);
    }

/*
**   Get option flags
*/
    fnum = 0;
    i = 1;
    while(i < argc){
	if (argv[i][0]=='-') {
        switch(argv[i][1]) {
            case 'b':
                if (optset == 0) {
                    blocks = 0;
                    optset = 1;
                } else {
                    error("incompatible options: -i and -b");
                }
                break;
            case 'i':
                if (optset == 0) {
                    intr = 1;
                    blocks = 0;
                    optset = 1;
                } else {
                    error("incompatible options: -i and -b");
                }
                break;
            case 'd':
                ndds = (int16)atoi(&argv[i][2]);
                break;
            case 't':
                nblk = atoi(&argv[i][2]);
                break;
            default:
                fprintf(stderr, "Unknown option -%c ignored\n", argv[i][1]);
                break;
        }
	} else {
	    if (fnum < 2) {
	        strcpy(fname[fnum], argv[i]);
	        fnum++;
	    }
	}
        i++;
    }

/*
**   Enough [unique] file arguments?
*/
    if ((fnum != 2) || (strcmp(fname[0], fname[1]) == 0)) {
	error("need 2 unique file names");
    }

/*
**   Check to make sure input file is HDF
*/
    ret = (int)Hishdf(fname[0]);
    if (ret == FALSE)
        hdferror();

/*
**   Open input and output files
*/
    infile = Hopen(fname[0], DFACC_READ, 0);

    if (infile == FAIL)
	hdferror();

    outfile = Hopen(fname[1], DFACC_CREATE, ndds);

    if (outfile == FAIL)
	hdferror();

/*
**   See how many data elements there
**   are and make room for their DD's
*/
    num_desc = Hnumber(infile, DFTAG_WILDCARD);
    if (num_desc == FAIL)
	hdferror();

    dlist = (mydd_t *)HDgetspace(num_desc * sizeof(*dlist));
    if (dlist == NULL)
	error("\tWow!  That file must be HUGE!\n\tThere isn't enough memory to hold the DD's.\n");

/*
**   Allocate data buffer - try 1 Meg first, work down
*/
    data_size = 1048576; /* 1 MB */
    data = NULL;
    while ((data = (unsigned char *)HDgetspace(data_size)) == NULL)
	data_size /= 2; /* okay then, cut request by half */

/*
**   Get all DD's for data elements
*/
    aid = Hstartread(infile, DFTAG_WILDCARD, DFREF_WILDCARD);
    if (aid == FAIL){
	printf("MAJOR PROBLEM: Hstartread for DD's; line %d\n", __LINE__);
	hdferror();
    }
    for(i = 0; i < num_desc; i++) {

        /*
         * Get data about the current one
         */ 
	Hinquire(aid, NULL, &dlist[i].tag, &dlist[i].ref, &dlist[i].length,
		 &dlist[i].offset, NULL, NULL, &dlist[i].special);

        /*
         * Move to the next one
         */
	stat = Hnextread(aid, DFTAG_WILDCARD, DFREF_WILDCARD, DF_CURRENT);

        /*
         * Fail if there are none left and we expect more
         */
	if ((stat == FAIL) && (i + 1 < num_desc)) {
	    printf("MAJOR PROBLEM: DDs; only got %d of %d; line %d\n",i,num_desc,__LINE__);
	    hdferror();
        }
    }

    /*
     * Close the access element
     */
    stat = Hendaccess(aid);
    if (stat == FAIL)
        hdferror();

/*
**   Sort DD's by offset to make it easy to
**   find DD's that point to the same data
*/
    qsort((char *)dlist, num_desc, sizeof(*dlist), desc_comp);

    oldoff = -1;
    oldlen = -1;
    for (i=0; i<num_desc; i++) {
	if (((dlist[i].tag != DFTAG_NULL) && (dlist[i].tag != DFTAG_VERSION))
	    && (dlist[i].tag != DFTAG_LINKED)) {
	    if ((dlist[i].offset != oldoff) || (dlist[i].length != oldlen)) {
/*
**   if this DD points to different data, read the data
**   from the old file and write it to the new file
*/
	        switch (dlist[i].special){
		    case SPECIAL_LINKED:
			if (intr == 1)
			    merge = promptblocks(&dlist[i]);
			else
			    merge = blocks;
			if (merge == 0) {
			    copy_blocks(&dlist[i], infile, outfile);
			} else {
			    merge_blocks(&dlist[i], infile, outfile);
			}
			break;
		    default:
			merge_blocks(&dlist[i], infile, outfile);
			break;
		} /* switch (special) */
	    } else {
/*
**   otherwise, just make a new DD for same data
*/
		ret = Hdupdd(outfile, dlist[i].tag, dlist[i].ref,
			     dlist[i-1].tag, dlist[i-1].ref);
                if (ret == FAIL) {
		    HERROR(DFE_GENAPP);
	            hdferror();
		}
	    }
	}
/*
**   save offset of data to check against next DD
*/
    oldoff = dlist[i].offset;
    }

/*
**   done; close files
*/
    Hclose(infile);
    Hclose(outfile);

    return(0);
}


/*
** NAME
**      promptblocks
*/
#ifdef PROTOTYPE
int promptblocks(mydd_t *dd)
#else
int promptblocks(dd)
mydd_t *dd;
#endif /* PROTOTYPE */
{
    char ans[80];

    printf("Would you like to coalesce the following linked-block element:\n");
    printf("\ttag = %d\n\tref = %d\n(y/n): ", dd->tag, dd->ref);
    gets(ans);
    if ((ans[0] == 'y') || (ans[0] == 'Y'))
	return(1);
    else
	return(0);
}


/*
** NAME
**      copy_blocks -- move a linked-block element; preserve blocking
*/
#ifdef PROTOTYPE
VOID copy_blocks(mydd_t *dd, int32 infile, int32 outfile)
#else
VOID copy_blocks(dd, infile, outfile)
mydd_t *dd;
int32 infile, outfile;
#endif /* PROTOTYPE */
{
    int32 inaid, ret, rdret, len, first_len, block_len, nblocks, outaid;
    char *FUNC="copy_blocks";


    inaid = Hstartread(infile, dd->tag, dd->ref);
    ret = HDinqblockinfo(inaid, &len, &first_len, &block_len, &nblocks);
    if (ret != SUCCEED) {
        HERROR(DFE_GENAPP);
        hdferror();
    }
/*
**  copy first block
*/
    outaid = Hstartwrite(outfile, HDbase_tag(dd->tag), dd->ref, first_len);
    if (outaid == FAIL) {
        HERROR(DFE_GENAPP);
        hdferror();
    }
    rdret = 0;
    while (rdret < first_len) {
        ret = Hread(inaid, (data_size<first_len) ? data_size : first_len, data);
	if (ret == FAIL) {
	    HERROR(DFE_GENAPP);
	    hdferror();
	}
	rdret += ret;
	ret = Hwrite(outaid, ret, data);
	if (ret == FAIL) {
	    HERROR(DFE_GENAPP);
	    hdferror();
	}
    }
    ret = Hendaccess(outaid);
/*
**  promote to linked-block element
*/
    if (nblk > 0)
        nblocks = nblk;

    outaid = HLcreate(outfile,HDbase_tag(dd->tag), dd->ref, block_len, nblocks);
    if (outaid == FAIL) {
	HERROR(DFE_GENAPP);
	hdferror();
    }
/*
**  copy remaining blocks
*/
    rdret = data_size;
    while (rdret == data_size) {
	rdret = Hread(inaid, data_size, data);
	if (ret == FAIL) {
	    HERROR(DFE_GENAPP);
	    hdferror();
	}
	ret = Hwrite(outaid, rdret, data);
	if (ret == FAIL) {
	    HERROR(DFE_GENAPP);
	    hdferror();
	}
    }
    Hendaccess(outaid);
}


/*
** NAME
**      merge_blocks
*/
#ifdef PROTOTYPE
VOID merge_blocks(mydd_t *dd, int32 infile, int32 outfile)
#else
VOID merge_blocks(dd, infile, outfile)
mydd_t *dd;
int32 infile, outfile;
#endif /* PROTOTYPE */
{
    int32 inaid, outaid, ret, len;
    char *FUNC="merge_blocks";


    inaid = Hstartread(infile, dd->tag, dd->ref);
    if (inaid == FAIL) {
	HERROR(DFE_GENAPP);
        hdferror();
    }
    outaid = Hstartwrite(outfile, HDbase_tag(dd->tag), dd->ref, dd->length);
    if (outaid == FAIL) {
	HERROR(DFE_GENAPP);
	hdferror();
    }

    while (dd->length>0) {
	dd->length -= (len= Hread(inaid, data_size, data));
        if (len == FAIL) {
	    HERROR(DFE_GENAPP);
	    hdferror();
	}
        ret = Hwrite(outaid, len, data);
        if (len == FAIL) {
	    HERROR(DFE_GENAPP);
            hdferror();
	}
    }

    ret = Hendaccess(inaid);
    if (ret == FAIL) {
	HERROR(DFE_GENAPP);
	hdferror();
    }

    ret = Hendaccess(outaid);
    if (ret == FAIL) {
	HERROR(DFE_GENAPP);
        hdferror();
    }
}


/*
** NAME
**	usage -- print out usage template
** USAGE
**	int usage()
** RETURNS
**	none
** DESCRIPTION
**	Print hdfpack's usage template to stdout.
** GLOBAL VARIABLES
** COMMENTS, BUGS, ASSUMPTIONS
** EXAMPLES
*/
#ifdef PROTOTYPE
static VOID usage(char *name)
#else
static VOID usage(name)
char *name;
#endif /* PROTOTYPE */
{
    fprintf(stderr, "Usage:  %s [-i | -b] [-d#] [-t#] <infile> <outfile>\n", name);
}

/*
** NAME
**	hdferror -- print out HDF error number
** USAGE
**	int hdferror()
** RETURNS
**	none
** DESCRIPTION
**	Print an HDF error number to stderr.
** GLOBAL VARIABLES
** COMMENTS, BUGS, ASSUMPTIONS
**	This routine terminates the program with code 1.
** EXAMPLES
*/
#ifdef PROTOTYPE
static VOID hdferror(void)
#else
static VOID hdferror()
#endif /* PROTOTYPE */
{
    HEprint(stderr, 0);
    exit(1);
}

/*
** NAME
**      error -- print error to stderr
** USAGE
**      int error(string);
**      char *string;           IN: pointer to error description string
** RETURNS
**	none
** DESCRIPTION
**	Print an HDF error number to stderr.
** GLOBAL VARIABLES
** COMMENTS, BUGS, ASSUMPTIONS
**	This routine terminates the program with code 1.
** EXAMPLES
*/
#ifdef PROTOTYPE
static VOID error(char *string)
#else
static VOID error(string)
char *string;
#endif
{
    fprintf(stderr, "%s: %s\n", invoke, string);
    exit(1);
}

/*
** NAME
**	desc_comp -- compare two DD's by offset
** USAGE
**	int desc_cmp(d1, d2)
**	mydd_t *d1		IN: pointer to a DD
**	mydd_t *d2		IN: pointer to a DD
** RETURNS
**	A number less than, greater than, or equal to 0, according to
**	whether d1's offset is less than, greater than, or equal to
**	d2's offset.
** DESCRIPTION
**	A pointer to this routine is passed to qsort() as the comparison
**	function for the sort.
** GLOBAL VARIABLES
** COMMENTS, BUGS, ASSUMPTIONS
** EXAMPLES
*/
#ifdef PROTOTYPE
int desc_comp(const void *d1, const void *d2)
#else
int desc_comp(d1, d2)
const VOID *d1, *d2;
#endif /* PROTOTYPE */
{
    return((int)(((mydd_t *)d1)->offset - ((mydd_t *)d2)->offset));
}
