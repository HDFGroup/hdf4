#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#include "hdf.h"
#include "mfhdf.h"

#include "hdiff.h"
#include "dumplib.h"

char *progname;


static void
usage()
{
 (void) fprintf(stderr,
  "hdiff [-b] [-g] [-s] [-d] [-S] [-v ...] [-e counnt] [-t limit] file1 file2\n");
 fprintf(stderr,"  [-b]              Verbose mode\n");
 fprintf(stderr,"  [-g]              Compare global attributes only\n");
 fprintf(stderr,"  [-s]              Compare SD local attributes only\n");
 fprintf(stderr,"  [-d]              Compare SD data only\n");
 fprintf(stderr,"  [-D]              Compare Vdata data only\n");
 fprintf(stderr,"  [-S]              Print statistics\n");
 fprintf(stderr,"  [-v var1[,...]]   Compare SD data on variable(s) <var1>,... only\n");
 fprintf(stderr,"  [-u var1[,...]]   Compare vdata on variable(s) <var1>,... only\n");
 fprintf(stderr,"  [-e count]        Print difference up to count number for each variable\n");
 fprintf(stderr,"  [-t limit]        Print difference when it is greater than limit\n");
 fprintf(stderr,"  file1             File name of the first input HDF file\n");
 fprintf(stderr,"  file2             File name of the second input HDF file\n");
 exit(EXIT_FAILURE);
}


int
main(int argc, char *argv[])
{
 extern int optind;
 extern int opterr;
 extern char *optarg;
 static struct fspec fspec = /* defaults, overridden on command line */
 {
  0,    /* verbose mode */
  1,    /* compare global attributes */
  1,    /* compare SD local attributes */
  1,    /* compare SD data */
  1,    /* compare GR data */
  1,    /* compare Vdata */
 -1,    /* no limit on the difference to be printed */
  0.0,  /* exact equal */
  0,    /* if -v specified, number of variables */
  0,    /* if -v specified, list of variable names */
  0,    /* if -u specified, number of variables */
  0,    /* if -u specified, list of variable names */
  0     /* if -S specified print statistics */
 };
 int   c;
 int   i, ret;
 
 opterr = 1;
 progname = argv[0];
 
 if (argc == 1)
  usage();
 
 while ((c = getopt(argc, argv, "bgsdSDe:t:v:u:")) != EOF)
  switch(c) {
 case 'b':  /* verbose mode */
  fspec.verbose =1;
  break;
 case 'g':  /* global attributes only */
  fspec.ga = 1;
  fspec.sa = 0;
  fspec.sd = 0;
  fspec.vd = 0;
  break;
 case 's':  /* SD local attributes only */
  fspec.ga = 0;
  fspec.sa = 1;
  fspec.sd = 0;
  fspec.vd = 0;
  break;
 case 'd':  /* SD data only */
  fspec.ga = 0;
  fspec.sa = 0;
  fspec.sd = 1;
  fspec.vd = 0;
  break;
 case 'D':  /* SD data only */
  fspec.ga = 0;
  fspec.sa = 0;
  fspec.sd = 0;
  fspec.vd = 1;
  break;
 case 'e':  /* max no. of difference to be printed */
  fspec.max_err_cnt = atoi(optarg);
  break;
 case 't':  /* range of difference to be printed */
  fspec.err_limit = (float)atof(optarg);
  break;
 case 'v':  /* variable names */
  /* make list of names of variables specified */
  make_vars (optarg, &fspec, 1);
  break;
 case 'u':  /* variable names */
  /* make list of names of variables specified */
  make_vars (optarg, &fspec, 2);
  break;
 case 'S':
  fspec.statistics = 1;
  break;
 case '?':
  usage();
  break;
 }
 
/*-------------------------------------------------------------------------
 * print the command line options
 *-------------------------------------------------------------------------
 */
 
 printf("$hdiff");
 for (i=1; i<argc ; i++) 
 {
  printf(" %s", argv[i] );
 }
 printf("\n");
 
 argv = argv + optind;

 ret=hdiff(argv[0],argv[1],fspec);
  
 return EXIT_SUCCESS;
}
