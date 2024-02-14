/*********************************************************************
 *   Copyright 1993, University Corporation for Atmospheric Research
 *   See netcdf/COPYRIGHT file for copying and redistribution conditions.
 *********************************************************************/

#ifndef NCDUMP_DUMPLIB_H
#define NCDUMP_DUMPLIB_H

extern char *progname; /* for error messages */

#ifdef __cplusplus
extern "C" {
#endif

/* Print error message to stderr, don't exit */
extern void error(const char *fmt, ...);

/* set position in line before lput() calls */
extern void set_indent(int indent);

/* set maximum line length */
extern void set_max_len(int len);

/* splits lines to keep them short */
extern void lput(const char *string);

/* In case different formats specified with -d option, set them here. */
extern void set_formats(char *flt_fmt, char *dbl_fmt);

/* Determine print format to use for each value for this variable. */
const char *get_fmt(int ncid, int varid, nc_type type);

/* structure for list of variables specified with -v option */
struct vnode {
    struct vnode *next;
    int           id;
};
typedef struct vnode vnode;

/* Get new variable list */
extern vnode *newvlist(void);

/* Add a variable id to variable list */
extern void varadd(vnode *vlist, int varid);

/* Test if a variable id is in variable list */
extern int varmember(vnode *vlist, int varid);

#ifdef __cplusplus
}
#endif

#endif /* NCDUMP_DUMPLIB_H */
