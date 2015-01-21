/*********************************************************************
 *   Copyright 1993, UCAR/Unidata
 *   See netcdf/COPYRIGHT file for copying and redistribution conditions.
 *   $Id$
 *********************************************************************/

#define  Printf  (void) printf
#define  ERR_READFAIL	-2	/* Used to return by vardata() to indicate that
				   ncvarget() fails to read the data */

typedef enum {false=0, true=1} bool;

struct ncdim {			/* dimension */
    char name[H4_MAX_NC_NAME];
    long size;
};

struct ncvar {			/* variable */
    char name[H4_MAX_NC_NAME];
    nc_type type;
    int ndims;
    int dims[H4_MAX_VAR_DIMS];
    int natts;
};

struct ncatt {			/* attribute */
    int var;
    char name[H4_MAX_NC_NAME];
    nc_type type;
    int len;
    void *val;
};

typedef
enum {LANG_NONE, LANG_C, LANG_F} Nclang; 

struct fspec {			/* specification for how to format dump */
    char *name;			/*
				 * name specified with -n or derived from file
				 * name
				 */
    bool header_only;		/*
				 * if true, don't print any variable data
				 */
    bool coord_vals;		/*
				 * if true, print header and coordinate
				 * dimension values (values of variables that
				 * are also dimensions), but no other variable
				 * data
				 */
    bool brief_data_cmnts;	/*
				 * if true, put // comments in data section
				 * identifying variable and indices, useful for
				 * navigating through large multi-dimensional
				 * data lists.
				 */
    bool full_data_cmnts;	/*
				 * if true, put // comments in data section
				 * identifying every value, useful for
				 * navigating through large multi-dimensional
				 * data lists.
				 */
    bool fix_str;		/*
				 * if true, replace nonalpha-numeric 
				 * characters in a name with underscores
				 */
    Nclang data_lang;		/*
				 * Specifies index conventions used in data
				 * comments, either LANG_C (C, 0-based, column
				 * major) or LANG_F (Fortran, 1-based, row
				 * major)
				 */
    int nlvars;			/*
				 * Number of variables specified with -v option
				 * on command line
				 */
    char** lvars;		/*
				 * list of variable names specified with -v
				 * option on command line
				 */
};

#ifdef OLD_WAY
extern int getopt               PROTO((
                                       int argc,
                                       char **argv,
                                       char *opts
                                ));

#endif /* HP9000 */

