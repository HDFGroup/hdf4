/*********************************************************************
 *   Copyright 1993, UCAR/Unidata
 *   See netcdf/COPYRIGHT file for copying and redistribution conditions.
 *********************************************************************/

#ifndef H4NC_GENLIB_H
#define H4NC_GENLIB_H

extern char *progname; /* for error messages */
extern char *cdlname;  /* for error messages */

#ifdef __cplusplus
extern "C" {
#endif

extern void  derror(const char *fmt, ...);
extern void *emalloc(size_t size);
extern void *erealloc(void *ptr, size_t size);
extern void  usage(void);

extern void yyerror(char *);
extern int  yyparse(void);

extern void put_variable(void *);

/* generate.c */
void        cline(const char *stmnt);
void        fline(const char *stmnt);
const char *nctype(nc_type);
const char *ncctype(nc_type);
char       *cstrstr(char *, size_t);
char       *fstrstr(char *, size_t);
char       *fstring(nc_type, void *, int);
void        define_netcdf(char *netcdfname);

/* load.c */
void load_netcdf(void *rec_start);

/* getfill.c */
void nc_fill(nc_type, long, void *, union generic);
void nc_getfill(nc_type, union generic *);
void nc_putfill(nc_type, void *, union generic *);

/* init.c */
void init_netcdf(void);

/* close.c */
void close_netcdf(void);

#ifdef __cplusplus
}
#endif

#endif /* H4NC_GENLIB_H */
