/*********************************************************************
 *   Copyright 1993, University Corporation for Atmospheric Research
 *   See netcdf/README file for copying and redistribution conditions.
 *   $Header$
 *********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#include <netcdf.h>
#include "ncdump.h"
#include "dumplib.h"
#include "vardata.h"

char *progname;

static void
usage()
{
#define USAGE   "\
  [-c]             Coordinate variable data and header information\n\
  [-h]             Header information only, no data\n\
  [-v var1[,...]]  Data for variable(s) <var1>,... only\n\
  [-b [c|f]]       Brief annotations for C or Fortran indices in data\n\
  [-f [c|f]]       Full annotations for C or Fortran indices in data\n\
  [-l len]         Line length maximum in data section (default 80)\n\
  [-n name]        Name for netCDF (default derived from file name)\n\
  [-d n[,n]]       Approximate floating-point values with less precision\n\
  file             File name of input netCDF file\n"

    (void) fprintf(stderr,
		   "%s [-c|-h] [-v ...] [[-b|-f] [c|f]] [-l len] [-n name] [-d n[,n]] file\n%s",
		   progname,
		   USAGE);
    exit(EXIT_FAILURE);
}


/* 
 * convert pathname of netcdf file into name for cdl unit, by taking 
 * last component of path and stripping off any extension.
 */
static char *
name_path(path)
     char *path;
{
    char *cp, *new;

#ifdef vms
#define FILE_DELIMITER ']'
#endif    
#ifdef MSDOS
#define FILE_DELIMITER '\\'
#endif    
#ifndef FILE_DELIMITER /* default to unix */
#define FILE_DELIMITER '/'
#endif
    cp = strrchr(path, FILE_DELIMITER);
    if (cp == 0)		/* no delimiter */
      cp = path;
    else			/* skip delimeter */
      cp++;
    new = (char *) malloc((unsigned) (strlen(cp)+1));
    if (new == 0) {
	error("out of memory!");
	exit(EXIT_FAILURE);
    }
    (void) strcpy(new, cp);	/* copy last component of path */
    if ((cp = strrchr(new, '.')) != NULL)
      *cp = '\0';		/* strip off any extension */
    return new;
}


static char *
type_name(type)
     nc_type type;
{
    switch (type) {
      case NC_BYTE:
	return "byte";
      case NC_CHAR:
	return "char";
      case NC_SHORT:
	return "short";
      case NC_LONG:
	return "long";
      case NC_FLOAT:
	return "float";
      case NC_DOUBLE:
	return "double";
      default:
	error("type_name: bad type %d", type);
	return "bogus";
    }
}

/*
 * Remove trailing zeros (after decimal point) but not trailing decimal
 * point from ss, a string representation of a floating-point number that
 * might include an exponent part.
 */
static void
tztrim(ss)
     char *ss;			/* returned string representing dd */
{
    char *cp, *ep;
    
    cp = ss;
    if (*cp == '-')
        cp++;
    while(isdigit((int)*cp) || *cp == '.')
        cp++;
    if (*--cp == '.')
        return;
    ep = cp+1;
    while (*cp == '0')
        cp--;
    cp++;
    if (cp == ep)
        return;
    while (*ep)
        *cp++ = *ep++;
    *cp = '\0';
    return;
}
 
 
/*
 * Print list of attribute values.  Attribute values must be printed with
 * explicit type tags, because their types are not declared.
 */
static void
pr_att_vals(type, len, vals)
     nc_type type;
     int len;
     void *vals;
{
    int iel;
    union {
	char *cp;
	short *sp;
	nclong *lp;
	float *fp;
	double *dp;
    } gp;
    char *sp;
    unsigned char uc;
    char gps[30];		/* for ascii of a float or double precision */
    char *f_fmt = "%#.8g";
    char *d_fmt = "%#.16g";

    switch (type) {
      case NC_BYTE:
	gp.cp = (char *) vals;
	for (iel = 0; iel < len; iel++)
	  if (isprint(uc = *gp.cp++ & 0377))
	    Printf ("'%c'%s", uc, iel<len-1 ? ", " : "");
	  else
	    Printf ("'\\%o'%s", uc, iel<len-1 ? ", " : "");
	break;
      case NC_CHAR:
	gp.cp = (char *) vals;
	Printf ("\"");
	/* adjust len so trailing nulls don't get printed */
	sp = gp.cp + len - 1;
	while (*sp-- == '\0' && len > 0)
	    len--;
	for (iel = 0; iel < len; iel++)
	  switch (uc = *gp.cp++ & 0377) {
	    case '\b':
	      Printf ("\\b");
	      break;
	    case '\f':
	      Printf ("\\f");
	      break;
	    case '\n':		/* generate linebreaks after new-lines */
	      Printf ("\\n\",\n    \"");
	      break;
	    case '\r':
	      Printf ("\\r");
	      break;
	    case '\t':
	      Printf ("\\t");
	      break;
	    case '\v':
	      Printf ("\\v");
	      break;
	    case '\\':
	      Printf ("\\\\");
	      break;
	    case '\'':
	      Printf ("\\'");
	      break;
	    case '\"':
	      Printf ("\\\"");
	      break;
	    default:
	      Printf ("%c",uc);
	      break;
	  }
	Printf ("\"");
	break;
      case NC_SHORT:
	gp.sp = (short *) vals;
	for (iel = 0; iel < len; iel++)
	  Printf ("%ds%s",*gp.sp++,iel<len-1 ? ", " : "");
	break;
      case NC_LONG:
	gp.lp = (nclong *) vals;
	for (iel = 0; iel < len; iel++)
	  Printf ("%ld%s",*gp.lp++,iel<len-1 ? ", " : "");
	break;
      case NC_FLOAT:
	gp.fp = (float *) vals;
	for (iel = 0; iel < len; iel++) {
	    int ll;
	    (void) sprintf(gps, f_fmt, * gp.fp++);
	    /* append a trailing "f" for floating-point attributes */
	    ll = strlen(gps);
	    gps[ll + 1] = '\0';
	    gps[ll] = 'f';
            tztrim(gps);	/* trim trailing 0's after '.' */
	    Printf ("%s%s", gps, iel<len-1 ? ", " : "");
	}
	break;
      case NC_DOUBLE:
	gp.dp = (double *) vals;
	for (iel = 0; iel < len; iel++) {
	    (void) sprintf(gps, d_fmt, *gp.dp++);
	    tztrim(gps);	/* trim trailing 0's after '.' */
	    Printf ("%s%s", gps, iel<len-1 ? ", " : "");
	}
	break;
      default:
	error("pr_att_vals: bad type");
    }
}


static void
do_ncdump(path, specp)
     char *path;
     struct fspec* specp;
{
    int ndims;			/* number of dimensions */
    int nvars;			/* number of variables */
    int ngatts;			/* number of global attributes */
    int xdimid;			/* id of unlimited dimension */
    int dimid;			/* dimension id */
    int varid;			/* variable id */
    struct ncdim dims[MAX_NC_DIMS]; /* dimensions */
    long vdims[MAX_NC_DIMS];	/* dimension sizes for a single variable */
    struct ncvar var;		/* variable */
    struct ncatt att;		/* attribute */
    int id;			/* dimension number per variable */
    int ia;			/* attribute number */
    int iv;			/* variable number */
    int is_coord;		/* true if variable is a coordinate variable */
    int ncid = ncopen(path, NC_NOWRITE); /* netCDF id */
    vnode* vlist = newvlist();	/* list for vars specified with -v option */

    /* don't crash on error */
    ncopts = 0;

    if (ncid == -1) { 
        error("ncopen failed on %s", path);
        return;
    }
    /*
     * If any vars were specified with -v option, get list of associated
     * variable ids
     */
    for (iv=0; iv < specp->nlvars; iv++) {
	varid = ncvarid(ncid, specp->lvars[iv]);
	varadd(vlist, varid);
    }

    /* if name not specified, derive it from path */
    if (specp->name == (char *)0) {
	specp->name = name_path (path);
    }

    Printf ("netcdf %s {\n", specp->name);
    /*
     * get number of dimensions, number of variables, number of global
     * atts, and dimension id of unlimited dimension, if any
     */
    (void) ncinquire(ncid, &ndims, &nvars, &ngatts, &xdimid);

    /* get dimension info */
    if (ndims > 0)
      Printf ("dimensions:\n");
    for (dimid = 0; dimid < ndims; dimid++) {
	(void) ncdiminq(ncid, dimid, dims[dimid].name, &dims[dimid].size);
	if (dimid == xdimid)
	  Printf ("\t%s = %s ; // (%d currently)\n",dims[dimid].name,
		  "UNLIMITED", dims[dimid].size);
	else
	  Printf ("\t%s = %ld ;\n",dims[dimid].name, dims[dimid].size);
    }

    Printf ("\nvariables:\n");
    /* get variable info, with variable attributes */
    for (varid = 0; varid < nvars; varid++) {
	(void) ncvarinq(ncid, varid, var.name, &var.type, &var.ndims, var.dims,
			&var.natts);
	Printf ("\t%s %s", type_name(var.type), var.name);
	if (var.ndims > 0)
	  Printf ("(");
	for (id = 0; id < var.ndims; id++) {
	    Printf ("%s%s",
		    dims[var.dims[id]].name,
		    id < var.ndims-1 ? ", " : ")");
	}
	Printf (" ;\n");
	/* get variable attributes */
	for (ia = 0; ia < var.natts; ia++) {
	    (void) ncattname(ncid, varid, ia, att.name);
	    
	    Printf ("\t\t%s:%s = ", var.name, att.name);

	    (void) ncattinq(ncid, varid, att.name, &att.type, &att.len);

	    att.val = (void *) malloc((unsigned)att.len*nctypelen(att.type));
	    if (!att.val) {
		error("Out of memory!");
		(void) ncclose(ncid);
		return;
	    }
	    (void) ncattget(ncid, varid, att.name, att.val);
	    pr_att_vals(att.type, att.len, att.val);
	    Printf (" ;\n");
	    free ((char *) att.val);
	}
    }

    /* get global attributes */
    if (ngatts > 0)
      Printf ("\n// global attributes:\n");
    for (ia = 0; ia < ngatts; ia++) {
	(void) ncattname(ncid, NC_GLOBAL, ia, att.name);
	
	Printf ("\t\t:%s = ", att.name);
	
	(void) ncattinq(ncid, NC_GLOBAL, att.name, &att.type, &att.len);
	att.val = (void *) malloc((unsigned) (att.len*nctypelen(att.type)));
	if (!att.val) {
	    error("Out of memory!");
	    (void) ncclose(ncid);
	    return;
	}
	(void) ncattget(ncid, NC_GLOBAL, att.name, att.val);

	pr_att_vals(att.type, att.len, att.val);
	Printf (" ;\n");
	free ((char *) att.val);
    }
    
    if (! specp->header_only) {
	if (nvars > 0)
	  Printf ("\ndata:\n");
	/* output variable data */
	for (varid = 0; varid < nvars; varid++) {
	    /* if var list specified, test for membership */
	    if (specp->nlvars > 0 && ! varmember(vlist, varid))
	      continue;
	    (void) ncvarinq(ncid, varid, var.name, &var.type, &var.ndims,
			    var.dims, &var.natts);
	    if (specp->coord_vals) {
		/* Find out if this is a coordinate variable */
		is_coord = 0;
		for (dimid = 0; dimid < ndims; dimid++) {
		    if (strcmp(dims[dimid].name, var.name) == 0 &&
			var.ndims == 1) {
			is_coord = 1;
			break;
		    }
		}
		if (! is_coord)	/* don't get data for non-coordinate vars */
		  continue;
	    }
	    /*
	     * Only get data for variable if it is not a record variable,
	     * or if it is a record variable and at least one record has
	     * been written.
	     */
	    if (var.ndims == 0
		|| var.dims[0] != xdimid
		|| dims[xdimid].size != 0) {
		/* Collect variable's dim sizes */
		for (id = 0; id < var.ndims; id++)
		  vdims[id] = dims[var.dims[id]].size;
		if (vardata(&var, vdims, ncid, varid, specp) == -1) {
		    error("can't output data for variable %s", var.name);
		    (void) ncclose(ncid); return;
		}
	    }
	}
    }

    Printf ("}\n");
    (void) ncclose(ncid);
}


static void
make_lvars(optarg, fspecp)
     char *optarg;
     struct fspec* fspecp;
{
    char *cp = optarg;
    int nvars = 1;
    char ** cpp;

    /* compute number of variable names in comma-delimited list */
    fspecp->nlvars = 1;
    while (*cp++)
      if (*cp == ',')
 	nvars++;

    fspecp->lvars = (char **) malloc(nvars * sizeof(char*));
    if (!fspecp->lvars) {
	error("out of memory");
	exit(EXIT_FAILURE);
    }

    cpp = fspecp->lvars;
    /* copy variable names into list */
    for (cp = strtok(optarg, ",");
	 cp != NULL;
	 cp = strtok((char *) NULL, ",")) {
	
	*cpp = (char *) malloc(strlen(cp) + 1);
	if (!*cpp) {
	    error("out of memory");
	    exit(EXIT_FAILURE);
	}
	strcpy(*cpp, cp);
	cpp++;
    }
    fspecp->nlvars = nvars;
}


/*
 * Extract the significant-digits specifiers from the -d argument on the
 * command-line and update the default data formats appropriately.
 */
static void
set_sigdigs(optarg)
     char *optarg;
{
    char *ptr = optarg;
    char *ptr2 = 0;
    long flt_digits = 7;	/* default floating-point digits */
    long dbl_digits = 15;	/* default double-precision digits */
    char flt_fmt[6];
    char dbl_fmt[6];

    if (optarg != 0 && strlen(optarg) > 0 && optarg[0] != ',')
        flt_digits=strtol(optarg, &ptr, 10);

    if (flt_digits < 1 || flt_digits > 10) {
	error("unreasonable value for float significant digits: %d",
	      flt_digits);
	exit(EXIT_FAILURE);
    }
    if (*ptr == ',')
      dbl_digits = strtol(ptr+1, &ptr2, 10);
    if (ptr2 == ptr+1 || dbl_digits < 1 || dbl_digits > 20) {
	error("unreasonable value for double significant digits: %d",
	      dbl_digits);
	exit(EXIT_FAILURE);
    }
    (void) sprintf(flt_fmt, "%%.%dg", flt_digits);
    (void) sprintf(dbl_fmt, "%%.%dg", dbl_digits);
    set_formats(flt_fmt, dbl_fmt);
}


int
main(argc, argv)
int argc;
char *argv[];
{
    extern int optind;
    extern int opterr;
    extern char *optarg;
    static struct fspec fspec =	/* defaults, overridden on command line */
      {
	  0,			/* construct netcdf name from file name */
	  false,		/* print header info only, no data? */
	  false,		/* just print coord vars? */
	  false,		/* brief  comments in data section? */
	  false,		/* full annotations in data section?  */
	  LANG_NONE,		/* language conventions for indices */
	  0,			/* if -v specified, number of variables */
	  0			/* if -v specified, list of variable names */
	  };
    int c;
    int i;
    int max_len = 80;		/* default maximum line length */

    opterr = 1;
    progname = argv[0];

    while ((c = getopt(argc, argv, "b:cf:hl:n:v:d:")) != EOF)
      switch(c) {
	case 'h':		/* dump header only, no data */
	  fspec.header_only = true;
	  break;
	case 'c':		/* header, data only for coordinate dims */
	  fspec.coord_vals = true;
	  break;
	case 'n':		/*
				 * provide different name than derived from
				 * file name
				 */
	  fspec.name = optarg;
	  break;
	case 'b':		/* brief comments in data section */
	  fspec.brief_data_cmnts = true;
	  switch (tolower(optarg[0])) {
	    case 'c':
	      fspec.data_lang = LANG_C;
	      break;
	    case 'f':
	      fspec.data_lang = LANG_F;
	      break;
	    default:
	      error("invalid value for -b option: %s", optarg);
	      exit(EXIT_FAILURE);
	  }
	  break;
	case 'f':		/* full comments in data section */
	  fspec.full_data_cmnts = true;
	  switch (tolower(optarg[0])) {
	    case 'c':
	      fspec.data_lang = LANG_C;
	      break;
	    case 'f':
	      fspec.data_lang = LANG_F;
	      break;
	    default:
	      error("invalid value for -b option: %s", optarg);
	      exit(EXIT_FAILURE);
	  }
	  break;
	case 'l':		/* maximum line length */
	  max_len = strtol(optarg, 0, 0);
	  if (max_len < 10) {
	      error("unreasonably small line length specified: %d", max_len);
	      exit(EXIT_FAILURE);
	  }
	  break;
	case 'v':		/* variable names */
	  /* make list of names of variables specified */
	  make_lvars (optarg, &fspec);
	  break;
	case 'd':		/* specify precision for floats */
	  set_sigdigs(optarg, &fspec);
	  break;
	case '?':
	  usage();
	  break;
      }

    set_max_len(max_len);
    
    argc -= optind;
    argv += optind;

    i = 0;

    do {		
	if (argc > 0)
	  do_ncdump(argv[i], &fspec);
    } while (++i < argc);
    return EXIT_SUCCESS;
}
