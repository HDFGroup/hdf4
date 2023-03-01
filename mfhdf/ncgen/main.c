/*********************************************************************
 *   Copyright 1993, UCAR/Unidata
 *   See netcdf/COPYRIGHT file for copying and redistribution conditions.
 *********************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "h4getopt.h"

#include "ncgen.h"
#include "genlib.h"

char *progname; /* for error messages */
char *cdlname;

int   c_flag;
int   fortran_flag;
int   netcdf_flag;
char *netcdf_name = NULL; /* name of output netCDF file to write */

extern FILE *yyin;
extern FILE *yyout;

void
usage()
{
    fprintf(stderr, "Usage: %s [-V] [ -b ] [ -c ] [ -f ] [ -o outfile] [ file ... ]\n", progname);
    fprintf(stderr, "  [-V]\t\t   Display version of the HDF4 library and exit\n");
    fprintf(stderr, "  [-b]\t\t   For binary netCDF output, '.nc' extension\n");
    fprintf(stderr, "  [-c]\t\t   For C output\n");
    fprintf(stderr, "  [-f]\t\t   For Fortran output\n");
    fprintf(stderr, "  [-o <outfile>]   Explicitly specify output file name\n");
    exit(8);
}

int
main(int argc, char *argv[])
{
    int   c;
    FILE *fp;

    yyin  = stdin;
    yyout = stdout;

    h4opterr = 1; /* print error message if bad option */
    progname = argv[0];
    cdlname  = "-";

    c_flag       = 0;
    fortran_flag = 0;
    netcdf_flag  = 0;

    while ((c = h4getopt(argc, argv, "Vbcfno:")) != EOF)
        switch (c) {
            case 'V': /* for c output */
                printf("%s, %s\n\n", argv[0], LIBVER_STRING);
                ;
                exit(EXIT_SUCCESS);
            case 'c': /* for c output */
                c_flag = 1;
                break;
            case 'f': /* for fortran output */
                fortran_flag = 1;
                break;
            case 'b': /* for binary netcdf output, ".nc" extension */
                netcdf_flag = 1;
                break;
            case 'n': /* old version of -b, uses ".cdf" extension */
                netcdf_flag = -1;
                break;
            case 'o': /* to explicitly specify output name */
                netcdf_flag = 1;
                netcdf_name = (char *)emalloc(strlen(h4optarg) + 1);
                if (!netcdf_name) {
                    derror("%s: out of memory", progname);
                    exit(1);
                }
                strcpy(netcdf_name, h4optarg);
                break;
            case '?':
                usage();
                break;
        }

    if (fortran_flag && c_flag) {
        derror("Only one of -c or -f may be specified");
        exit(8);
    }

    argc -= h4optind;
    argv += h4optind;

    if (argc > 1) {
        derror("%s: only one input file argument permitted", progname);
        exit(6);
    }

    fp = stdin;
    if (argc > 0 && strcmp(argv[0], "-") != 0) {
        if ((fp = fopen(argv[0], "r")) == NULL) {
            derror("can't open file %s for reading: ", argv[0]);
            perror("");
            exit(7);
        }
        cdlname = (char *)emalloc(1 + strlen(argv[0]));
        strcpy(cdlname, argv[0]);
    }
    yyin = fp;
    return (yyparse());
}
