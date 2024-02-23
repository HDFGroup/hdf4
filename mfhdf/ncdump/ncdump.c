/*********************************************************************
 *   Copyright 1993, University Corporation for Atmospheric Research
 *   See netcdf/README file for copying and redistribution conditions.
 *********************************************************************/

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "nc_priv.h"
#include "ncdump.h"

#include "h4getopt.h"
#include "dumplib.h"
#include "vardata.h"

static const char *type_name(nc_type type);

char         *progname;
struct ncdim *dims  = NULL; /* dimensions */
long         *vdims = NULL; /* dimension sizes for a single variable */

static void
usage()
{
    printf("ncdump [-V|-c|-h|-u] [-v ...] [[-b|-f] [c|f]] [-l len] [-n name] [-d n[,n]] file\n");
    printf("\t [-V]             Display version of the HDF4 library and exit\n");
    printf("\t [-c]             Coordinate variable data and header information\n");
    printf("\t [-h]             Header information only, no data\n");
    printf("\t [-u]             Replace nonalpha-numerics in names with underscores\n");
    printf("\t [-v var1[,...]]  Data for variable(s) <var1>,... only\n");
    printf("\t [-b [c|f]]       Brief annotations for C or Fortran indices in data\n");
    printf("\t [-f [c|f]]       Full annotations for C or Fortran indices in data\n");
    printf("\t [-l len]         Line length maximum in data section (default 80)\n");
    printf("\t [-n name]        Name for netCDF (default derived from file name)\n");
    printf("\t [-d n[,n]]       Approximate floating-point values with less precision\n");
    printf("\t file             File name of input netCDF file\n");

    exit(EXIT_FAILURE);
}

/*
 * convert pathname of netcdf file into name for cdl unit, by taking
 * last component of path and stripping off any extension.
 */
static char *
name_path(char *path)
{
    char *cp, *new;

#ifdef MSDOS
#define FILE_DELIMITER '\\'
#endif
#ifndef FILE_DELIMITER /* default to unix */
#define FILE_DELIMITER '/'
#endif
    cp = strrchr(path, FILE_DELIMITER);
    if (cp == 0) /* no delimiter */
        cp = path;
    else /* skip delimeter */
        cp++;
    new = (char *)malloc((unsigned)(strlen(cp) + 1));
    if (new == 0) {
        error("out of memory!");
        exit(EXIT_FAILURE);
    }
    (void)strcpy(new, cp); /* copy last component of path */
    if ((cp = strrchr(new, '.')) != NULL)
        *cp = '\0'; /* strip off any extension */
    return new;
}

static const char *
type_name(nc_type type)
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
/* ss - returned string representing dd */
static void
tztrim(char *ss)
{
    char *cp, *ep;

    cp = ss;
    if (*cp == '-')
        cp++;
    while (isdigit((int)*cp) || *cp == '.')
        cp++;
    if (*--cp == '.')
        return;
    ep = cp + 1;
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
pr_att_vals(nc_type type, int len, void *vals)
{
    int iel;
    union {
        char   *cp;
        short  *sp;
        nclong *lp;
        float  *fp;
        double *dp;
    } gp;
    char         *sp;
    unsigned char uc;
    char          gps[30]; /* for ascii of a float or double precision */
    const char   *f_fmt = "%#.8g";
    const char   *d_fmt = "%#.16g";

    switch (type) {
        case NC_BYTE:
            gp.cp = (char *)vals;
            for (iel = 0; iel < len; iel++)
                if (isprint(uc = (unsigned char)(*gp.cp++ & 0377)))
                    printf("'%c'%s", uc, iel < len - 1 ? ", " : "");
                else
                    printf("'\\%o'%s", uc, iel < len - 1 ? ", " : "");
            break;
        case NC_CHAR:
            gp.cp = (char *)vals;
            printf("\"");
            /* adjust len so trailing nulls don't get printed */
            sp = gp.cp + len - 1;
            while (*sp-- == '\0' && len > 0)
                len--;
            for (iel = 0; iel < len; iel++)
                switch (uc = (unsigned char)(*gp.cp++ & 0377)) {
                    case '\b':
                        printf("\\b");
                        break;
                    case '\f':
                        printf("\\f");
                        break;
                    case '\n': /* generate linebreaks after new-lines */
                        printf("\\n\",\n    \"");
                        break;
                    case '\r':
                        printf("\\r");
                        break;
                    case '\t':
                        printf("\\t");
                        break;
                    case '\v':
                        printf("\\v");
                        break;
                    case '\\':
                        printf("\\\\");
                        break;
                    case '\'':
                        printf("\\'");
                        break;
                    case '\"':
                        printf("\\\"");
                        break;
                    default:
                        printf("%c", uc);
                        break;
                }
            printf("\"");
            break;
        case NC_SHORT:
            gp.sp = (short *)vals;
            for (iel = 0; iel < len; iel++)
                printf("%ds%s", *gp.sp++, iel < len - 1 ? ", " : "");
            break;
        case NC_LONG:
            gp.lp = (nclong *)vals;
            for (iel = 0; iel < len; iel++)
                printf("%d%s", (int)*gp.lp++, iel < len - 1 ? ", " : "");
            break;
        case NC_FLOAT:
            gp.fp = (float *)vals;
            for (iel = 0; iel < len; iel++) {
                size_t ll;
                (void)sprintf(gps, f_fmt, (double)*gp.fp++);
                /* append a trailing "f" for floating-point attributes */
                ll          = strlen(gps);
                gps[ll + 1] = '\0';
                gps[ll]     = 'f';
                tztrim(gps); /* trim trailing 0's after '.' */
                printf("%s%s", gps, iel < len - 1 ? ", " : "");
            }
            break;
        case NC_DOUBLE:
            gp.dp = (double *)vals;
            for (iel = 0; iel < len; iel++) {
                (void)sprintf(gps, d_fmt, *gp.dp++);
                tztrim(gps); /* trim trailing 0's after '.' */
                printf("%s%s", gps, iel < len - 1 ? ", " : "");
            }
            break;
        default:
            error("pr_att_vals: bad type");
    }
}

/*
 * sanitize_string
 *
 * 	If the string contains characters other than alpha-numerics,
 * 	an underscore, or a hyphen, convert it to an underscore.
 */
char *
sanitize_string(char *str, bool sanitize)
{
    char *new_str = NULL;

    if (!str)
        return NULL;

    new_str = strdup(str);
    if (NULL == new_str) {
        error("Out of memory!");
        return NULL;
    }

    if (sanitize) {
        char *ptr = new_str;

        /* Convert all non-alpha, non-numeric, non-hyphen, non-underscore
         * characters to underscores
         */
        for (; *ptr; ptr++)
            if (!isalnum(*ptr) && *ptr != '_' && *ptr != '-')
                *ptr = '_';
    }

    return new_str;
}

static void
do_ncdump(char *path, struct fspec *specp)
{
    int          ndims;       /* number of dimensions */
    int          nvars;       /* number of variables */
    int          ngatts;      /* number of global attributes */
    int          xdimid;      /* id of unlimited dimension */
    int          dimid;       /* dimension id */
    int          varid;       /* variable id */
    struct ncvar var;         /* variable */
    struct ncatt att;         /* attribute */
    int          id;          /* dimension number per variable */
    int          ia;          /* attribute number */
    int          iv;          /* variable number */
    int          is_coord;    /* true if variable is a coordinate variable */
    int          isempty = 0; /* true if an old hdf dim has no scale values */

    int    ncid  = ncopen(path, NC_NOWRITE); /* netCDF id */
    vnode *vlist = newvlist();               /* list for vars specified with -v option */

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
    for (iv = 0; iv < specp->nlvars; iv++) {
        varid = ncvarid(ncid, specp->lvars[iv]);
        varadd(vlist, varid);
    }

    /* if name not specified, derive it from path */
    if (specp->name == NULL)
        specp->name = name_path(path);

    printf("netcdf %s {\n", specp->name);

    /*
     * get number of dimensions, number of variables, number of global
     * atts, and dimension id of unlimited dimension, if any
     */
    (void)ncinquire(ncid, &ndims, &nvars, &ngatts, &xdimid);

    /* get dimension info */
    if (ndims > 0) {
        printf("dimensions:\n");

        for (dimid = 0; dimid < ndims; dimid++) {
            char *fixed_str = NULL;

            if (ncdiminq(ncid, dimid, dims[dimid].name, &dims[dimid].size) < 0)
                fprintf(stderr, "Error calling ncdiminq on dimid = %d\n", dimid);

            fixed_str = sanitize_string(dims[dimid].name, specp->fix_str);
            if (fixed_str == NULL) {
                (void)ncclose(ncid);
                return;
            }

            if (dimid == xdimid)
                printf("\t%s = %s ; // (%d currently)\n", fixed_str, "UNLIMITED", (int)dims[dimid].size);
            else
                printf("\t%s = %ld ;\n", fixed_str, dims[dimid].size);

            free(fixed_str);
        }
    }

    printf("\nvariables:\n");

    /* get variable info, with variable attributes */
    for (varid = 0; varid < nvars; varid++) {
        char *fixed_var;

        (void)ncvarinq(ncid, varid, var.name, &var.type, &var.ndims, var.dims, &var.natts);
        fixed_var = sanitize_string(var.name, specp->fix_str);

        if (fixed_var == NULL) {
            (void)ncclose(ncid);
            return;
        }

        printf("\t%s %s", type_name(var.type), fixed_var);

        if (var.ndims > 0)
            printf("(");

        for (id = 0; id < var.ndims; id++) {
            char *fixed_dim = sanitize_string(dims[var.dims[id]].name, specp->fix_str);

            if (fixed_dim == NULL) {
                (void)ncclose(ncid);
                free(fixed_var);
                return;
            }

            printf("%s%s", fixed_dim, id < var.ndims - 1 ? ", " : ")");
            free(fixed_dim);
        }

        printf(" ;\n");

        /* get variable attributes */
        for (ia = 0; ia < var.natts; ia++) {
            char *fixed_att;

            (void)ncattname(ncid, varid, ia, att.name);
            fixed_att = sanitize_string(att.name, specp->fix_str);

            if (!fixed_att) {
                (void)ncclose(ncid);
                free(fixed_var);
                return;
            }

            printf("\t\t%s:%s = ", fixed_var, fixed_att);
            (void)ncattinq(ncid, varid, att.name, &att.type, &att.len);
            att.val = (void *)malloc((size_t)(att.len * nctypelen(att.type)));

            if (!att.val) {
                error("Out of memory!");
                (void)ncclose(ncid);
                free(fixed_att);
                free(fixed_var);
                return;
            }

            (void)ncattget(ncid, varid, att.name, att.val);
            pr_att_vals(att.type, att.len, att.val);
            printf(" ;\n");
            free(att.val);
            free(fixed_att);
        }

        free(fixed_var);
    }

    /* get global attributes */
    if (ngatts > 0)
        printf("\n// global attributes:\n");

    for (ia = 0; ia < ngatts; ia++) {
        char *fixed_att;

        (void)ncattname(ncid, NC_GLOBAL, ia, att.name);
        fixed_att = sanitize_string(att.name, specp->fix_str);

        if (!fixed_att) {
            (void)ncclose(ncid);
            return;
        }

        printf("\t\t:%s = ", fixed_att);

        (void)ncattinq(ncid, NC_GLOBAL, att.name, &att.type, &att.len);
        att.val = malloc((unsigned)(att.len * nctypelen(att.type)));

        if (!att.val) {
            error("Out of memory!");
            (void)ncclose(ncid);
            free(fixed_att);
            return;
        }

        (void)ncattget(ncid, NC_GLOBAL, att.name, att.val);
        pr_att_vals(att.type, att.len, att.val);
        printf(" ;\n");
        free(att.val);
        free(fixed_att);
    }

    if (!specp->header_only) {
        if (nvars > 0)
            printf("\ndata:\n");

        /* output variable data */
        for (varid = 0; varid < nvars; varid++) {
            /* if var list specified, test for membership */
            if (specp->nlvars > 0 && !varmember(vlist, varid))
                continue;

            (void)ncvarinq(ncid, varid, var.name, &var.type, &var.ndims, var.dims, &var.natts);

            if (specp->coord_vals) {
                /* Find out if this is a coordinate variable */
                is_coord = 0;

                for (dimid = 0; dimid < ndims; dimid++) {
                    if (strcmp(dims[dimid].name, var.name) == 0 && var.ndims == 1) {
                        is_coord = 1;
                        break;
                    }
                }

                if (!is_coord)
                    /* don't get data for non-coordinate vars */
                    continue;
            }

            /*
             * Only get data for variable if it is not a record variable,
             * or if it is a record variable and at least one record has
             * been written.
             */
            /* skip the dimension vars which have dim strings only.  */
            {
                NC     *handle;
                NC_var *vp;

                isempty = 0;
                handle  = NC_check_id(ncid);

                if (handle->file_type == HDF_FILE) {
                    vp = NC_hlookupvar(handle, varid);
                    /* This is set up to take care of
                     * cases where an array has been
                     * defined but no data */
                    /* has yet been added. */
                    if ((vp->data_tag == DFTAG_SDS || vp->data_tag == DFTAG_SD) && (vp->data_ref == 0))
                        isempty = 1;
                }
            }

            if (isempty)
                continue;

            if (var.ndims == 0 || var.dims[0] != xdimid || dims[xdimid].size != 0) {
                int ret = 0;

                /* Collect variable's dim sizes */
                for (id = 0; id < var.ndims; id++)
                    vdims[id] = dims[var.dims[id]].size;

                /* if (vardata(&var, vdims, ncid, varid, specp) == -1)
                 */
                ret = vardata(&var, vdims, ncid, varid, specp);
                /* Keep the original failure detection until further improvement */
                if (ret == -1) {
                    error("can't output data for variable %s", var.name);
                    (void)ncclose(ncid);
                    return;
                }
                /* This failure indicates that vardata failed
                   because ncvarget fails.  (HDFFR-1468)
                   -BMR, 2015/01/20) */
                if (ret == ERR_READFAIL) {
                    error("Reading failed for variable %s, possibly the data is corrupted.", var.name);
                    (void)ncclose(ncid);
                    return;
                }
            }
        }
    }

    free(vlist);

    printf("}\n");
    (void)ncclose(ncid);
}

static void
make_lvars(char *arg, struct fspec *fspecp)
{
    char  *cp    = arg;
    int    nvars = 1;
    char **cpp;

    /* compute number of variable names in comma-delimited list */
    fspecp->nlvars = 1;
    while (*cp++)
        if (*cp == ',')
            nvars++;

    fspecp->lvars = (char **)malloc((size_t)nvars * sizeof(char *));
    if (!fspecp->lvars) {
        error("out of memory");
        exit(EXIT_FAILURE);
    }

    cpp = fspecp->lvars;
    /* copy variable names into list */
    for (cp = strtok(arg, ","); cp != NULL; cp = strtok((char *)NULL, ",")) {

        *cpp = (char *)malloc(strlen(cp) + 1);
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
set_sigdigs(char *arg)
{
    char *ptr        = arg;
    char *ptr2       = 0;
    long  flt_digits = 7;  /* default floating-point digits */
    long  dbl_digits = 15; /* default double-precision digits */
    char  flt_fmt[16];
    char  dbl_fmt[16];

    if (arg != 0 && strlen(arg) > 0 && arg[0] != ',')
        flt_digits = strtol(arg, &ptr, 10);

    if (flt_digits < 1 || flt_digits > 10) {
        error("unreasonable value for float significant digits: %d", flt_digits);
        exit(EXIT_FAILURE);
    }
    if (*ptr == ',')
        dbl_digits = strtol(ptr + 1, &ptr2, 10);
    if (ptr2 == ptr + 1 || dbl_digits < 1 || dbl_digits > 20) {
        error("unreasonable value for double significant digits: %d", dbl_digits);
        exit(EXIT_FAILURE);
    }
    (void)sprintf(flt_fmt, "%%.%dg", (int)flt_digits);
    (void)sprintf(dbl_fmt, "%%.%dg", (int)dbl_digits);
    set_formats(flt_fmt, dbl_fmt);
}

int
main(int argc, char *argv[])
{
    static struct fspec fspec = /* defaults, overridden on command line */
        {
            0,         /* construct netcdf name from file name */
            false,     /* print header info only, no data? */
            false,     /* just print coord vars? */
            false,     /* brief  comments in data section? */
            false,     /* full annotations in data section?  */
            false,     /* replace nonalpha-numeric with underscore?  */
            LANG_NONE, /* language conventions for indices */
            0,         /* if -v specified, number of variables */
            0          /* if -v specified, list of variable names */
        };
    int c;
    int i;
    int max_len = 80; /* default maximum line length */

    h4opterr = 1;
    progname = argv[0];

    if (1 == argc) /* if no arguments given, print help and exit */
        usage();

    dims  = (struct ncdim *)calloc(H4_MAX_NC_DIMS, sizeof(struct ncdim)); /* dimensions */
    vdims = (long *)calloc(H4_MAX_NC_DIMS, sizeof(long)); /* dimension sizes for a single variable */

    if ((dims == NULL) || (vdims == NULL)) {
        error("Unable to allocate memory!\n");
        exit(EXIT_FAILURE);
    }

    while ((c = h4getopt(argc, argv, "b:cf:hul:n:v:d:V")) != EOF)
        switch (c) {
            case 'V': /* display version of the library */
                printf("%s, %s\n\n", argv[0], LIBVER_STRING);
                exit(EXIT_SUCCESS);
            case 'h': /* dump header only, no data */
                fspec.header_only = true;
                break;
            case 'c': /* header, data only for coordinate dims */
                fspec.coord_vals = true;
                break;
            case 'n': /*
                       * provide different name than derived from
                       * file name
                       */
                fspec.name = h4optarg;
                break;
            case 'u': /* replace nonalpha-numerics with underscores */
                fspec.fix_str = true;
                break;
            case 'b': /* brief comments in data section */
                fspec.brief_data_cmnts = true;
                switch (tolower(h4optarg[0])) {
                    case 'c':
                        fspec.data_lang = LANG_C;
                        break;
                    case 'f':
                        fspec.data_lang = LANG_F;
                        break;
                    default:
                        error("invalid value for -b option: %s", h4optarg);
                        exit(EXIT_FAILURE);
                }
                break;
            case 'f': /* full comments in data section */
                fspec.full_data_cmnts = true;
                switch (tolower(h4optarg[0])) {
                    case 'c':
                        fspec.data_lang = LANG_C;
                        break;
                    case 'f':
                        fspec.data_lang = LANG_F;
                        break;
                    default:
                        error("invalid value for -b option: %s", h4optarg);
                        exit(EXIT_FAILURE);
                }
                break;
            case 'l': /* maximum line length */
                max_len = (int)strtol(h4optarg, 0, 0);
                if (max_len < 10) {
                    error("unreasonably small line length specified: %d", max_len);
                    exit(EXIT_FAILURE);
                }
                break;
            case 'v': /* variable names */
                /* make list of names of variables specified */
                make_lvars(h4optarg, &fspec);
                break;
            case 'd': /* specify precision for floats */
                set_sigdigs(h4optarg);
                break;
            case '?':
                usage();
                break;
        }

    set_max_len(max_len);

    argc -= h4optind;
    argv += h4optind;

    i = 0;
    do {
        if (argc > 0)
            do_ncdump(argv[i], &fspec);
    } while (++i < argc);

    free(dims);
    free(vdims);

    return EXIT_SUCCESS;
}
