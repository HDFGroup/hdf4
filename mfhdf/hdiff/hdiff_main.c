/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Copyright by The HDF Group.                                               *
 * Copyright by the Board of Trustees of the University of Illinois.         *
 * All rights reserved.                                                      *
 *                                                                           *
 * This file is part of HDF.  The full HDF copyright notice, including       *
 * terms governing use, modification, and redistribution, is contained in    *
 * the COPYING file, which can be found at the root of the source code       *
 * distribution tree, or in https://support.hdfgroup.org/ftp/HDF/releases/.  *
 * If you do not have access to either file, you may request a copy from     *
 * help@hdfgroup.org.                                                        *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "hdf.h"
#include "hfile.h"
#include "mfhdf.h"
#include "hdiff.h"

/*-------------------------------------------------------------------------
 * Function: main
 *
 * Purpose: hdiff main program
 *
 * Programmer: Pedro Vicente Nunes, pvn@ncsa.uiuc.edu
 *
 * Date:  August 27, 2003
 *  Modifications: 
 *  March 8, 2006. Added percent (relative) option
 *
 * Comments:
 *
 *-------------------------------------------------------------------------
 */


char *progname;

 
static void usage()
{

 (void) fprintf(stdout,
  "hdiff [-V] [-b] [-g] [-s] [-d] [-D] [-S] [-v var1[,...]] [-u var1[,...]] [-e count] [-t limit] [-p relative] file1 file2\n");
 fprintf(stdout,"  [-V]              Display version of the HDF4 library and exit\n");
 fprintf(stdout,"  [-b]              Verbose mode\n");
 fprintf(stdout,"  [-g]              Compare global attributes only\n");
 fprintf(stdout,"  [-s]              Compare SD local attributes only\n");
 fprintf(stdout,"  [-d]              Compare SD data only\n");
 fprintf(stdout,"  [-D]              Compare Vdata data only\n");
 fprintf(stdout,"  [-S]              Print statistics\n");
 fprintf(stdout,"  [-v var1[,...]]   Compare SD data on variable(s) <var1>,... only\n");
 fprintf(stdout,"  [-u var1[,...]]   Compare vdata on variable(s) <var1>,... only\n");
 fprintf(stdout,"  [-e count]        Print difference up to count number for each variable\n");
 fprintf(stdout,"  [-t limit]        Print difference when it is greater than limit\n");
 fprintf(stdout,"  [-p relative]     Print difference when it is greater than a relative limit\n");
 fprintf(stdout,"  file1             File name of the first HDF file\n");
 fprintf(stdout,"  file2             File name of the second HDF file\n");
 fprintf(stdout,"\n");
 fprintf(stdout,"The 'count' value must be a positive integer\n");
 fprintf(stdout,"The 'limit' and 'relative' values must be positive numbers\n");
 fprintf(stdout,"The -t compare criteria is |a - b| > limit\n");
 fprintf(stdout,"The -p compare criteria is |(b-a)/a| > relative\n");
 fprintf(stdout,"Return codes: 0 (no differences found), 1 (differences found)\n");
 exit(EXIT_FAILURE);
}


int
main(int argc, char *argv[])
{
    extern int optind;
    extern int opterr;
    extern char *optarg;
 
    static diff_opt_t opt = /* defaults, overridden on command line */
    {
            0,    /* verbose mode */
            1,    /* compare global attributes */
            1,    /* compare SD local attributes */
            1,    /* compare SD data */
            1,    /* compare GR data */
            1,    /* compare Vdata */
            MAX_DIFF,   /* no limit on the difference to be printed */
            0.0,  /* exact equal */
            0,    /* if -v specified, number of variables */
            0,    /* if -v specified, list of variable names */
            0,    /* if -u specified, number of variables */
            0,    /* if -u specified, list of variable names */
            0,    /* if -S specified print statistics */
            0,    /* -p err_rel */
            0,    /* error status */
    };
    int    c;
    uint32 nfound;
    int    ret;
    
    opterr = 1;
    progname = argv[0];
    
    if (argc < 2)
        usage();
    
    while ((c = getopt(argc, argv, "VbgsdSDe:t:v:u:p:")) != EOF)
    {
        switch(c) 
        {
        case 'V':  /* display version of the library */
            printf("%s, %s\n\n", argv[0], LIBVER_STRING ); 
            exit(0);
        case 'b':  /* verbose mode */
            opt.verbose =1;
            break;
        case 'g':  /* global attributes only */
            opt.ga = 1;
            opt.sa = 0;
            opt.sd = 0;
            opt.vd = 0;
            break;
        case 's':  /* SD local attributes only */
            opt.ga = 0;
            opt.sa = 1;
            opt.sd = 0;
            opt.vd = 0;
            break;
        case 'd':  /* SD data only */
            opt.ga = 0;
            opt.sa = 0;
            opt.sd = 1;
            opt.vd = 0;
            break;
        case 'D':  /* SD data only */
            opt.ga = 0;
            opt.sa = 0;
            opt.sd = 0;
            opt.vd = 1;
            break;
        case 'e':  /* max no. of difference to be printed */
            opt.max_err_cnt = atoi(optarg);
            break;
        case 't':  /* range of difference to be printed */
            opt.err_limit = (float32)atof(optarg);
            break;
        case 'v':  /* variable names */
            /* make list of names of variables specified */
            make_vars (optarg, &opt, 1);
            break;
        case 'u':  /* variable names */
            /* make list of names of variables specified */
            make_vars (optarg, &opt, 2);
            break;
        case 'S':
            opt.statistics = 1;
            break;
        case 'p':
            opt.err_rel = (float32)atof(optarg);
            break;
       }
    }
    
    argv = argv + optind;
    
    nfound = hdiff(argv[0],argv[1],&opt);
    
   /*-------------------------------------------------------------------------
    * exit code
    *   >0 if differences, 0 if no differences, <0 if error
    *-------------------------------------------------------------------------
    */
    
    ret= (nfound==0 ? 0 : 1 );
    if (opt.err_stat)
        ret=-1;
    return ret;
    
}
