#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>



#include "hdf.h"
#include "mfhdf.h"

#include "hdiff.h"
#include "dumplib.h"
/* 
* convert pathname of netcdf file into name for cdl unit, by taking 
* last component of path and stripping off any extension.
*/
char *
name_path(char *path)
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
 if (cp == 0)  /* no delimiter */
  cp = path;
 else   /* skip delimeter */
  cp++;
 new = (char *) malloc((unsigned) (strlen(cp)+1));
 if (new == 0) {
  error("out of memory!");
  exit(EXIT_FAILURE);
 }
 (void) strcpy(new, cp); /* copy last component of path */
 if ((cp = strrchr(new, '.')) != NULL)
  *cp = '\0';  /* strip off any extension */
 return new;
}


char *
type_name(nc_type type)
{
 switch (type) {
 case DFNT_INT8:
  return "byte";
 case DFNT_CHAR:
  return "char";
 case DFNT_INT16:
  return "short";
 case DFNT_INT32:
  return "long";
 case DFNT_FLOAT:
  return "float";
 case DFNT_DOUBLE:
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
void
tztrim(char *ss)
  /* returned string representing dd */
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
void
pr_att_vals(nc_type type, int len, void *vals)
{
 int iel;
 union {
  char *cp;
  short *sp;
  long *lp;
  float *fp;
  double *dp;
 } gp;
 char *sp;
 unsigned char uc;
 char gps[30];  /* for ascii of a float or double precision */
 char *f_fmt = "%#.8g";
 char *d_fmt = "%#.16g";
 
 switch (type) {
 case DFNT_INT8:
  gp.cp = (char *) vals;
  for (iel = 0; iel < len; iel++)
   if (isprint(uc = *gp.cp++ & 0377))
    Printf ("'%c'%s", uc, iel<len-1 ? ", " : "");
   else
    Printf ("'\\%o'%s", uc, iel<len-1 ? ", " : "");
   break;
 case DFNT_CHAR:
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
 case '\n':  /* generate linebreaks after new-lines */
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
 case DFNT_INT16:
  gp.sp = (short *) vals;
  for (iel = 0; iel < len; iel++)
   Printf ("%ds%s",*gp.sp++,iel<len-1 ? ", " : "");
  break;
 case DFNT_INT32:
  gp.lp = (long *) vals;
  for (iel = 0; iel < len; iel++)
   Printf ("%ld%s",*gp.lp++,iel<len-1 ? ", " : "");
  break;
 case DFNT_FLOAT:
  gp.fp = (float *) vals;
  for (iel = 0; iel < len; iel++) {
   int ll;
   (void) sprintf(gps, f_fmt, * gp.fp++);
   /* append a trailing "f" for floating-point attributes */
   ll = strlen(gps);
   gps[ll + 1] = '\0';
   gps[ll] = 'f';
   tztrim(gps); /* trim trailing 0's after '.' */
   Printf ("%s%s", gps, iel<len-1 ? ", " : "");
  }
  break;
 case DFNT_DOUBLE:
  gp.dp = (double *) vals;
  for (iel = 0; iel < len; iel++) {
   (void) sprintf(gps, d_fmt, *gp.dp++);
   tztrim(gps); /* trim trailing 0's after '.' */
   Printf ("%s%s", gps, iel<len-1 ? ", " : "");
  }
  break;
 default:
  error("pr_att_vals: bad type - %d", type);
 }
}



void
make_vars(char *optarg, struct fspec* fspecp, int option)
{
 char *cp = optarg;
 int nvars = 1;
 char ** cpp;
 
 /* compute number of variable names in comma-delimited list */
 if (option == 1)
  fspecp->nlvars = 1;
 else
  fspecp->nuvars = 1;
 
 while (*cp++)
  if (*cp == ',')
   nvars++;
  
  if (option == 1)
  {
   fspecp->lvars = (char **) malloc(nvars * sizeof(char*));
   if (!fspecp->lvars) {
    error("out of memory");
    exit(EXIT_FAILURE);
   }
   cpp = fspecp->lvars;
  }
  else
  {
   fspecp->uvars = (char **) malloc(nvars * sizeof(char*));
   if (!fspecp->uvars) {
    error("out of memory");
    exit(EXIT_FAILURE);
   }
   cpp = fspecp->uvars;
  }
  
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
  if (option == 1)
   fspecp->nlvars = nvars;
  else
   fspecp->nuvars = nvars;
}


/*
* Extract the significant-digits specifiers from the -d argument on the
* command-line and update the default data formats appropriately.
*/
void
set_sigdigs(char *optarg)
{
 char *ptr = optarg;
 char *ptr2 = 0;
 long flt_digits = 7; /* default floating-point digits */
 long dbl_digits = 15; /* default double-precision digits */
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


