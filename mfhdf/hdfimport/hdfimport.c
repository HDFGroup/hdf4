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

/*
 * Name:
 *      hdfimport (previously fp2hdf)
 *
 * Purpose:
 *      To convert floating point and/or integer data to HDF Scientific Data Set (SDS)
 *      and/or 8-bit Raster Image Set (RIS8) format, storing the results
 *      in an HDF file.  The image data can be scaled about the mean value.
 *
 *                                  -------------
 *      floating point data        |             | ----------> RIS8
 *      (SDS, ASCII text, or  ---> | hdfimport   |   and/or
 *      native floating point)     |             | ----------> SDS
 *                                  -------------
 *                       AND / OR
 *                               ---------------
 *      integer data               |              |
 *      (ASCII text, or       ---> |  hdfimport   |  ----------> SDS
 *      binary integer)            |              |
 *                                  --------------
 * Synopsis:
 *      hdfimport -h[elp], OR
 *      hdfimport <infile> [ [-t[ype] <output-type> | -n] [<infile> [-t[ype] <output-type> | -n ]]...]
 *                            -o[utfile] <outfile> [-r[aster] [ras_opts ...]] [-f[loat]]
 *
 *      -h[elp]:
 *              Print this summary of usage, and exit.
 *
 *      <infile(s)>:
 *              Name of the input file(s), containing a single
 *        two-dimensional or three-dimensional floating point array
 *        in either ASCII text, native floating point, native integer
 *        or HDF SDS format.  If an HDF file is used for input, it
 *        must contain an SDS. The SDS need only contain a dimension
 *        record and the data, but if it also contains maximum and
 *        minimum values and/or scales for each axis, these will
 *        be used.  If the input format is ASCII text or native
 *        floating point or native integer, see "Notes" below on
 *        how it must be organized.
 *
 *      -t[ype] <output_type>:
 *              Optionally used for every input ASCII file to specify the
 *            data type of the data-set to be written. If not specified
 *              default data type is 32-bit floating point. <output-type>
 *              can be any of the following: FP32 (default), FP64, INT32
 *             INT16, INT8. It can be used only with ASCII files.
 *
 *      -n:
 *        This option is to be used only if the binary input file
 *        contains 64-bit floating point data and the default
 *        behaviour (default behaviour is to write it to a 32-bit
 *        floating point data-set) should be overridden to write
 *        it to a 64-bit floating point data-set.
 *
 *      -o[utfile] <outfile>:
 *              Data from one or more input files are stored as one or
 *              more data sets and/or images in one HDF output file,
 *              "outfile".
 *
 *      -r[aster]:
 *              Store output as a raster image set in the output file.
 *
 *      -f[loat]:
 *              Store output as a scientific data set in the output file.
 *              This is the default if the "-r" option is not specified.
 *
 *      ras_opts ...
 *
 *      -e[xpand] <horiz> <vert> [<depth>]:
 *              Expand float data via pixel replication to produce the
 *              image(s).  "horiz" and "vert" give the horizontal and
 *              vertical resolution of the image(s) to be produced; and
 *              optionally, "depth" gives the number of images or depth
 *              planes (for 3D input data).
 *
 *      -i[nterp] <horiz> <vert> [<depth>]:
 *              Apply bilinear, or trilinear, interpolation to the float
 *              data to produce the image(s).  "horiz", "vert", and "depth"
 *              must be greater than or equal to the dimensions of the
 *              original dataset.
 *      If max and min are supplied in input file, this option clips
 *      values that are greater than max or less then min, setting
 *      them to the max and min, respectively.
 *
 *      -p[alfile] <palfile>:
 *              Store the palette with the image.  Get the palette from
 *              "palfile"; which may be an HDF file containing a palette,
 *              or a file containing a raw palette.
 *
 *      -m[ean] <mean>:
 *              If a floating point mean value is given, the image will be
 *              scaled about the mean.  The new extremes (newmax and newmin),
 *              as given by:
 *
 *                 newmax = mean + max(abs(max-mean), abs(mean-min))
 *                 newmin = mean - max(abs(max-mean), abs(mean-min))
 *
 *              will be equidistant from the mean value.  If no mean value
 *              is given, then the mean will be:  0.5 * (max + min)
 *
 * Notes:
 *      If the input file format is ASCII text or native floating point or native integer(32-bit,
 *      16-bit, 8-bit), it
 *      must have the following input fields:
 *
 *              format
 *              nplanes
 *              nrows
 *              ncols
 *              max_value
 *              min_value
 *              [plane1 plane2 plane3 ...]
 *              row1 row2 row3 ...
 *              col1 col2 col3 ...
 *              data1 data2 data3 ...
 *              ...
 *
 *      Where:
 *              format:
 *                      Format designator ("TEXT", "FP32", "FP64", "IN32", "IN16", "IN08").
 *                      nplanes, nrows, ncols:
 *                      Dimensions are specified in the order slowest changing dimension first.
 *            ncols is dimension of the fastest changing dimension. (horizontal axis
 *            or X-axis in a 3D scale)
 *            nrows corresponds to dimension of the vertical axis or Y-axis in a 3D
 *            scale.
 *            nplanes corresponds to the slowest changing dimension i.e. dimension of
 *            the depth axis or the Z-axis in a 3D scale ("1" for 2D input).
 *              max_value:
 *                      Maximum data value.
 *              min_value:
 *                      Minimum data value.
 *              plane1, plane2, plane3, ...:
 *                      Scales for depth axis.
 *              row1, row2, row3, ...:
 *                      Scales for the vertical axis.
 *              col1, col2, col3, ...:
 *                      Scales for the horizontal axis.
 *              data1, data2, data3, ...:
 *                      The data ordered by rows, left to right and top
 *                      to bottom; then optionally, ordered by planes,
 *                      front to back.
 *
 *      For FP32 and FP64 input format, "format", "nplanes", "nrows", "ncols",
 *      and "nplanes" are native integers; where "format" is the integer
 *      representation of the appropriate 4-character string (0x46503332 for
 *      "FP32" and 0x46503634 for "FP64").  The remaining input fields are
 *      composed of native 32-bit floating point values for FP32 input format,
 *      or native 64-bit floating point values for FP64 input format.
 *
 *      For IN32, IN16 and IN08 input format, "format", "nplanes", "nrows", "ncols",
 *      and "nplanes" are native integers; where "format" is the integer
 *      representation of the appropriate 4-character string. The remaining input
 *      fields are composed of native 32-bit integer values for IN32 input format,
 *      or native 16-bit integer values for IN16 input format or native 8-bit
 *      integer values for IN08 input format.
 *
 */

#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hdf.h"
#include "hfile.h"
#include "mfhdf.h"

#ifdef H4_HAVE_SYS_STAT_H
#include <sys/stat.h>
#endif

#ifdef H4_HAVE_FCNTL_H
#include <fcntl.h>
#endif

/*
 * global macros
 */
#define EXPAND   1 /* -e: expand image with pixel replication */
#define INTERP   2 /* -i: expand image with interpolation */
#define NAME_LEN 255

/*
 * structure definition to associate input files with the output data types
 */
struct infilesformat {
    char  filename[NAME_LEN];
    int   outtype; /* if the value is "" output type will be FP32. Applicable only to TEXT Files*/
    int32 handle;  /* added to facilitate the use of SD interface -BMR 2006/08/18 */
};
/*
 * structure definition for command line options
 */
struct Options {
    struct infilesformat infiles[30]; /* structure to hold the list of input file names. Limited to 30*/
    char                 outfile[32]; /* output file name */
    char                 palfile[32]; /* palette file name, if any */
    int                  fcount;      /* number of input files */
    int                  to_float;    /* float output is desired */
    int                  to_image;    /* image output is desired */
    int                  to_int;
    int                  pal;     /* output palette with image */
    int                  ctm;     /* color transform method: EXPAND or INTERP */
    int                  exh;     /* horizontal expansion factor */
    int                  exv;     /* vertical expansion factor */
    int                  exd;     /* depth expansion factor */
    int                  hres;    /* horizontal resolution of output image */
    int                  vres;    /* vertical resolution of output image */
    int                  dres;    /* depth resolution of output image */
    int                  mean;    /* scale image around a mean */
    float32              meanval; /* mean value to scale the image around */
};

/* Additional Structures to handle different data types */
struct int16set /* variables for an INT 16 data set */
{
    int16  max;
    int16  min;
    int16 *hscale;
    int16 *vscale;
    int16 *dscale;
};

struct int32set /* variables for an INT 32 data set */
{
    int32  max;
    int32  min;
    int32 *hscale;
    int32 *vscale;
    int32 *dscale;
};

struct fp64set /* variables for a FLOAT 64 data set */
{
    float64  max;
    float64  min;
    float64 *hscale;
    float64 *vscale;
    float64 *dscale;
};

struct int8set /* variables for an INT 8 data set */
{
    int8  max;
    int8  min;
    int8 *hscale;
    int8 *vscale;
    int8 *dscale;
};

/*
 * structure definition for the input data
 */
struct Input {
    int             is_hdf;    /* HDF file format flag */
    int             is_text;   /* ASCII text format flag */
    int             is_fp32;   /* 32-bit native floating point format flag */
    int             is_fp64;   /* 64-bit native floating point format flag */
    int             is_int32;  /* 32-bit int */
    int             is_int16;  /* 16-bit int */
    int32           rank;      /* number of input data dimensions */
    int32           dims[3];   /* input dimensions - ncols, nrows, nplanes */
    int             is_vscale; /* vertical axis scales in the input */
    int             is_hscale; /* horizontal axis scales in the input */
    int             is_dscale; /* depth axis scales in the input */
    float32         max;       /* maximum value of the data */
    float32         min;       /* minimum value of the data */
    float32        *hscale;    /* horizontal scales for fp32*/
    float32        *vscale;    /* vertical scales for fp32*/
    float32        *dscale;    /* depth scales for fp32*/
    struct int32set in32s;
    struct int16set in16s;
    struct int8set  in8s;
    struct fp64set  fp64s;
    void           *data; /* input data */
    int             outtype;
};

/*
 * structure definition for the output raster images
 */
struct Raster {
    int            hres; /* horizontal resolution of the image */
    int            vres; /* vertical resolution of the image */
    int            dres; /* depth resolution of the image */
    unsigned char *image;
};

/*
 *  constants to represent data types
 */

#define FP_32  0
#define FP_64  1
#define INT_32 2
#define INT_16 3
#define INT_8  4
#define NO_NE  5

/*
 * state table tokens
 */
#define FILENAME 0  /* filename */
#define OPT_o    1  /* output filename */
#define OPT_r    2  /* convert to image */
#define OPT_e    3  /* expand image via pixel replication */
#define OPT_i    4  /* make interpolated image */
#define OPT_num  5  /* resolution of enlarged image */
#define OPT_p    6  /* palette filename */
#define OPT_f    7  /* convert to float (default) */
#define OPT_h    8  /* request for explanation */
#define OPT_m    9  /* mean to scale around */
#define OPT_t    10 /* datatype of the SDS to be written */
#define OPT_n                                                                                                \
    11 /* for  a FLOAT 64 binary input file to be accepted as FLOAT 64 SDS (default behaviour is writing it  \
          as FLOAT 32 SDS */
#define ERR 20 /* invalid token */

/*
 * state table for parsing the command line.
 */
static int state_table[19][12] = {

    /* token ordering:
       FILENAME     OPT_o   OPT_r   OPT_e   OPT_i   OPT_num   OPT_p   OPT_f
       OPT_h        OPT_m   OPT_z */

    /* state 0: start */
    {1, ERR, ERR, ERR, ERR, ERR, ERR, ERR, 14, ERR, ERR, ERR},

    /* state 1: input files */
    {1, 2, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR, 17, 18},

    /* state 2: -o[utfile] */
    {3, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR},

    /* state 3: outfile */
    {ERR, ERR, 4, ERR, ERR, ERR, ERR, 13, ERR, ERR, ERR, ERR},

    /* state 4: -r[aster] */
    {ERR, ERR, ERR, 5, 9, ERR, 10, 12, ERR, 15, ERR, ERR},

    /* state 5: -e[xpand] */
    {ERR, ERR, ERR, ERR, ERR, 6, ERR, ERR, ERR, ERR, ERR, ERR},

    /* state 6: -e[xpand] or -i[nterp] option argument */
    {ERR, ERR, ERR, ERR, ERR, 7, ERR, ERR, ERR, ERR, ERR, ERR},

    /* state 7: -e[xpand] or -i[nterp] option argument */
    {ERR, ERR, ERR, ERR, ERR, 8, 10, 12, ERR, 15, ERR, ERR},

    /* state 8: -e[xpand] or -i[nterp] option argument */
    {ERR, ERR, ERR, ERR, ERR, ERR, 10, 12, ERR, 15, ERR, ERR},

    /* state 9: -i[nterp] */
    {ERR, ERR, ERR, ERR, ERR, 6, ERR, ERR, ERR, ERR, ERR, ERR},

    /* state 10: -p[alfile] */
    {11, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR},

    /* state 11: palfile */
    {ERR, ERR, ERR, 5, 9, ERR, ERR, 12, ERR, 15, ERR, ERR},

    /* state 12: -f[loat] (after -r[aster]) */
    {ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR},

    /* state 13: -f[loat] */
    {ERR, ERR, 4, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR},

    /* state 14: -h[elp] */
    {ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR},

    /* state 15: -m[ean] */
    {ERR, ERR, ERR, ERR, ERR, 16, ERR, ERR, ERR, ERR, ERR, ERR},

    /* state 16: mean */
    {ERR, ERR, ERR, 5, 9, ERR, 10, 12, ERR, ERR, ERR, ERR},

    /* state 17: output type for data set */
    {1, 2, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR},

    /* state 18: override default behaviour for FP 64 */
    {1, 2, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR}

};

/* static local functions */
static int gtoken(char *s);
static int process(struct Options *opt);
static int gfloat(char *infile, FILE *strm, float32 *fp32, struct Input *in);
static int gint(char *infile, FILE *strm, int32 *ival, struct Input *in);
static int isnum(char *s);
static int gdata(struct infilesformat infile_info, struct Input *in, FILE *strm, int *is_maxmin);
static int gdimen(struct infilesformat infile_info, struct Input *in, FILE *strm);
static int gmaxmin(struct infilesformat infile_info, struct Input *in, FILE *strm, int *is_maxmin);
static int gscale(struct infilesformat infile_info, struct Input *in, FILE *strm, int *is_scale);
static int gtype(char *infile, struct Input *in, FILE **strm);
static int indexes(float32 *scale, int dim, int *idx, int res);
static int interp(struct Input *in, struct Raster *im);
static int palette(char *palfile);
static int pixrep(struct Input *in, struct Raster *im);

/*
 * functions with non-integer return types
 */
void help(char *);
void mean(struct Input *, struct Options *);
void usage(char *);

/*
 * Additional functions defined to incorporate the revisions (pkamat)
 */
static int gfloat64(char *infile, FILE *strm, float64 *fp64, struct Input *in);
static int gint32(char *infile, FILE *strm, int32 *ival, struct Input *in);
static int gint16(char *infile, FILE *strm, int16 *ival, struct Input *in);
static int gint8(char *infile, FILE *strm, int8 *ival, struct Input *in);
static int init_scales(struct Input *in);
void       fpdeallocate(struct Input *in, struct Raster *im, struct Options *opt);

/*
 * Name:
 *      main
 *
 * Purpose:
 *      The driver for "hdfimport".
 *
 *  Revision (pkamat):
 *        Changes to the state table to handle -t option and the -n option.
 *      Also, a different structure used for holding input files.
 */
int
main(int argc, char *argv[])
{
    struct Options *opt = NULL;
    int             i, k;
    int             outfile_named = FALSE;
    int             token;
    int             state       = 0;
    int             flag        = 0;
    char            types[5][6] = {"FP32", "FP64", "INT32", "INT16", "INT8"};

    const char *err1 = "Invalid number of arguments:  %d.\n";
    const char *err2 = "Error in state table.\n";
    const char *err3 = "No output file given.\n";
    const char *err4 = "Program aborted.\n";
    const char *err5 = "Cannot allooacte memory.\n";

    if (NULL == (opt = (struct Options *)calloc(1, sizeof(struct Options)))) {
        fprintf(stderr, "%s", err5);
        goto err;
    }

    /* set 'stdout' and 'stderr' to line-buffering mode */
    (void)HDsetvbuf(stderr, (char *)NULL, _IOLBF, 0);
    (void)HDsetvbuf(stdout, (char *)NULL, _IOLBF, 0);

    /*
     * validate the number of command line arguments
     */
    if (argc < 2) {
        fprintf(stderr, err1, argc);
        usage(argv[0]);
        goto err;
    }

    opt->to_image = FALSE; /* default: no image */
    opt->to_float = FALSE; /* default: make float if no image */
                           /* Set FALSE here.  Will be set TRUE */
                           /* after confirming image option is not set.  */
    opt->ctm    = EXPAND;  /* default: pixel replication */
    opt->hres   = 0;       /* default: no expansion values */
    opt->vres   = 0;
    opt->dres   = 0;
    opt->pal    = FALSE; /* default: no palette */
    opt->mean   = FALSE; /* default: no mean given */
    opt->fcount = 0;     /* to count number of input files */

    /*
     * parse the command line
     */
    for (i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-V") == 0) {
            printf("%s, %s\n\n", argv[0], LIBVER_STRING);
            exit(0);
        }

        if ((token = gtoken(argv[i])) == ERR) {
            usage(argv[0]);
            goto err;
        }

        state = state_table[state][token];

        switch (state) {
            case 1: /* counting input files */
                (void)strcpy(opt->infiles[opt->fcount].filename, argv[i]);
                opt->infiles[opt->fcount].outtype = NO_NE;
                opt->fcount++;
                break;
            case 2: /* -o found; look for outfile */
                break;
            case 3: /* get outfile name */
                (void)strcpy(opt->outfile, argv[i]);
                outfile_named = TRUE;
                break;
            case 4: /* -r found */
                opt->to_image = TRUE;
                break;
            case 5: /* -e found */
                opt->ctm = EXPAND;
                break;
            case 6: /* horizontal resolution */
                opt->hres = atoi(argv[i]);
                break;
            case 7: /* vertical resolution */
                opt->vres = atoi(argv[i]);
                break;
            case 8: /* depth resolution */
                opt->dres = atoi(argv[i]);
                break;
            case 9: /* -i found */
                opt->ctm = INTERP;
                break;
            case 10: /* -p found */
                opt->pal = TRUE;
                break;
            case 11: /* get pal filename */
                (void)strcpy(opt->palfile, argv[i]);
                break;
            case 12: /* -f found (after a -r) */
            case 13: /* -f found (no -r yet) */
                opt->to_float = TRUE;
                break;
            case 14: /* -h found; help, then exit */
                help(argv[0]);
                exit(0);
            case 15: /* -m found */
                opt->mean = TRUE;
                break;
            case 16: /* mean value */
                opt->meanval = (float32)atof(argv[i]);
                break;
            case 17: /* -t found */
                i++;
                flag = 0;
                for (k = 0; ((k <= 4) && (!flag)); k++)
                    if (!strcmp(argv[i], types[k]))
                        flag = 1;
                if (flag)
                    opt->infiles[opt->fcount - 1].outtype = k - 1;
                else {
                    usage(argv[0]);
                    goto err;
                }
                break;
            case 18: /* -n found */
                opt->infiles[opt->fcount - 1].outtype = FP_64;
                break;
            case ERR: /* command syntax error */
            default:
                fprintf(stderr, "%s", err2);
                usage(argv[0]);
                goto err;
        }
    }

    /*
     * make sure an output file was specified
     */
    if (!outfile_named) {
        fprintf(stderr, "%s", err3);
        usage(argv[0]);
        goto err;
    }

    if (!opt->to_image)
        opt->to_float = TRUE;

    /*
     * process the input files
     */
    if (process(opt))
        goto err;

    free(opt);

    return EXIT_SUCCESS;

err:
    free(opt);
    fprintf(stderr, "%s", err4);
    return EXIT_FAILURE;
}

/*
 * Name:
 *      gdata
 *
 * Purpose:
 *      Get the input data.
 *
 * Revision(pkamat):
 *      Modified to read in data of type INT 32, INT 16, INT 8
 *      in addition to FP 32 and FP 64.
 * Revision: (bmribler - 2006/8/18)
 *    Replaced first parameter with 'struct infilesformat' to use both
 *    the file name and the SD identifier (handle.)
 */
static int
gdata(struct infilesformat infile_info, struct Input *in, FILE *strm, int *is_maxmin)
{
    int32       i, j, k;
    float32    *fp32;
    int32      *in32;
    int16      *in16;
    float64    *fp64;
    int8       *in8;
    int32       hdfdims[3], start[3]; /* order: ZYX or YX */
    int32       sd_id, sds_id, sd_index;
    int32       len = in->dims[0] * in->dims[1] * in->dims[2];
    char        infile[NAME_LEN];
    intn        status;
    const char *err1 = "Unable to get input data from file: %s.\n";

    /*
     * extract the input data from the input file
     */
    if (in->is_hdf == TRUE) {
        sd_id = infile_info.handle;
        strcpy(infile, infile_info.filename);
        sd_index = 0;
        sds_id   = SDselect(sd_id, sd_index);

        /*
         * hdfdims is ordered: ZYX or YX
         * in->dims is ordered: XYZ
         */
        if (in->rank == 2) {
            hdfdims[0] = in->dims[1];
            hdfdims[1] = in->dims[0];
            start[0] = start[1] = 0;
        }
        else {
            hdfdims[0] = in->dims[2];
            hdfdims[1] = in->dims[1];
            hdfdims[2] = in->dims[0];
            start[0] = start[1] = start[2] = 0;
        }

        status = SDreaddata(sds_id, start, NULL, hdfdims, in->data);
        if (status == FAIL) {
            fprintf(stderr, err1, infile);
            goto err;
        }
    }
    else {
        if (in->outtype == FP_32) {
            for (k = 0, fp32 = (float32 *)in->data; k < in->dims[2]; k++) {
                for (j = 0; j < in->dims[1]; j++) {
                    for (i = 0; i < in->dims[0]; i++, fp32++) {
                        if (gfloat(infile, strm, fp32, in)) {
                            fprintf(stderr, err1, infile);
                            goto err;
                        }
                    }
                }
            }
            if (*is_maxmin == FALSE) {
                in->min = in->max = *(float32 *)in->data;
                for (i = 1; i < len; i++) {
                    if (((float32 *)in->data)[i] > in->max)
                        in->max = ((float32 *)in->data)[i];
                    if (((float32 *)in->data)[i] < in->min)
                        in->min = ((float32 *)in->data)[i];
                }
                *is_maxmin = TRUE;
            }
        }
        if (in->outtype == INT_32) {
            for (k = 0, in32 = (int32 *)in->data; k < in->dims[2]; k++) {
                for (j = 0; j < in->dims[1]; j++) {
                    for (i = 0; i < in->dims[0]; i++, in32++) {
                        if (gint32(infile, strm, in32, in)) {
                            fprintf(stderr, err1, infile);
                            goto err;
                        }
                    }
                }
            }
            if (*is_maxmin == FALSE) {
                in->in32s.min = in->in32s.max = *(int32 *)in->data;
                for (i = 1; i < len; i++) {
                    if (((int32 *)in->data)[i] > in->in32s.max)
                        in->in32s.max = ((int32 *)in->data)[i];
                    if (((int32 *)in->data)[i] < in->in32s.min)
                        in->in32s.min = ((int32 *)in->data)[i];
                }
                *is_maxmin = TRUE;
            }
        }
        if (in->outtype == INT_16) {
            for (k = 0, in16 = (int16 *)in->data; k < in->dims[2]; k++) {
                for (j = 0; j < in->dims[1]; j++) {
                    for (i = 0; i < in->dims[0]; i++, in16++) {
                        if (gint16(infile, strm, in16, in)) {
                            fprintf(stderr, err1, infile);
                            goto err;
                        }
                    }
                }
            }
            if (*is_maxmin == FALSE) {
                in->in16s.min = in->in16s.max = *(int16 *)in->data;
                for (i = 1; i < len; i++) {
                    if (((int16 *)in->data)[i] > in->in16s.max)
                        in->in16s.max = ((int16 *)in->data)[i];
                    if (((int16 *)in->data)[i] < in->in16s.min)
                        in->in16s.min = ((int16 *)in->data)[i];
                }
                *is_maxmin = TRUE;
            }
        }

        if (in->outtype == INT_8) {
            for (k = 0, in8 = (int8 *)in->data; k < in->dims[2]; k++) {
                for (j = 0; j < in->dims[1]; j++) {
                    for (i = 0; i < in->dims[0]; i++, in8++) {
                        if (gint8(infile, strm, in8, in)) {
                            fprintf(stderr, err1, infile);
                            goto err;
                        }
                    }
                }
            }
            if (*is_maxmin == FALSE) {
                in->in8s.min = in->in8s.max = *(int8 *)in->data;
                for (i = 1; i < len; i++) {
                    if (((int8 *)in->data)[i] > in->in8s.max)
                        in->in8s.max = ((int8 *)in->data)[i];
                    if (((int8 *)in->data)[i] < in->in8s.min)
                        in->in8s.min = ((int8 *)in->data)[i];
                }
                *is_maxmin = TRUE;
            }
        }

        if (in->outtype == FP_64) {
            for (k = 0, fp64 = (float64 *)in->data; k < in->dims[2]; k++) {
                for (j = 0; j < in->dims[1]; j++) {
                    for (i = 0; i < in->dims[0]; i++, fp64++) {
                        if (gfloat64(infile, strm, fp64, in)) {
                            fprintf(stderr, err1, infile);
                            goto err;
                        }
                    }
                }
            }
            if (*is_maxmin == FALSE) {
                in->fp64s.min = in->fp64s.max = *(float64 *)in->data;
                for (i = 1; i < len; i++) {
                    if (((float64 *)in->data)[i] > in->fp64s.max)
                        in->fp64s.max = ((float64 *)in->data)[i];
                    if (((float64 *)in->data)[i] < in->fp64s.min)
                        in->fp64s.min = ((float64 *)in->data)[i];
                }
                *is_maxmin = TRUE;
            }
        }

        /* } */
        (void)fclose(strm);
    }

#ifdef DEBUG
    printf("\tdata:");
    for (k = 0, fp32 = in->data; k < in->dims[2]; k++) {
        printf("\n");
        for (j = 0; j < in->dims[1]; j++) {
            printf("\n\t");
            for (i = 0; i < in->dims[0]; i++, fp32++)
                printf("%E ", *fp32);
        }
    }
    printf("\n\n\n");
#endif /* DEBUG */

    return (0);

err:
    return (1);
}

/*
 * Name:
 *      gdimen
 *
 * Purpose:
 *      Determine the input data dimensions.
 * Revision: (bmribler - 2006/8/18)
 *    Used the SD interface instead of DFSD.
 *    Replaced first parameter with 'struct infilesformat' to use both
 *    the file name and the SD identifier (handle.)
 */

static int
gdimen(struct infilesformat infile_info, struct Input *in, FILE *strm)
{
    int32 hdfdims[3];                      /* order: ZYX or YX */
    char *infile   = infile_info.filename; /* shortcut for input filename */
    char *sds_name = NULL;
    int32 rank, nattrs, dtype; /* rank, num of attrs, data type */

    const char *err1 = "Unable to get data dimensions from file: %s.\n";
    const char *err2 = "Invalid data rank of %d in file: %s.\n";
    const char *err3 = "Dimension(s) is less than '2' in file: %s.\n";
    const char *err4 = "Unexpected number type from file: %s.\n";
    const char *err5 = "Unable to get the length of the SDS' name: index %d.\n";
    const char *err6 = "Unable to allocate dynamic memory.\n";
    const char *err7 = "Failed to open the SDS.\n";

    /*
     * extract the rank and dimensions of the HDF input file
     */
    if (in->is_hdf == TRUE) {
        int32  sds_id, sd_index;
        int32  sd_id    = infile_info.handle; /* shortcut for handle from SDstart */
        uint16 name_len = 0;
        intn   status   = FAIL;

        /* get the dimension information of the only SDS in the file */
        sd_index = 0;
        sds_id   = SDselect(sd_id, sd_index);
        if (sds_id == FAIL) {
            fprintf(stderr, "%s", err7);
            goto err;
        }

        /* get the SDS name's length and allocate sufficient space for
        the name's buffer */
        status = SDgetnamelen(sds_id, &name_len);
        if (status == FAIL) {
            fprintf(stderr, err5, sd_index);
            goto err;
        }
        sds_name = (char *)malloc(name_len + 1);
        if (sds_name == NULL) {
            fprintf(stderr, "%s", err6);
            goto err;
        }

        /* obtain the SDS' information */
        status = SDgetinfo(sds_id, sds_name, &rank, hdfdims, &dtype, &nattrs);
        if (status == FAIL) {
            fprintf(stderr, err1, infile);
            goto err;
        }
        in->rank = (int)rank;

        /* don't know how to deal with other numbers yet */
        if (dtype != DFNT_FLOAT32) {
            fprintf(stderr, err4, infile);
            goto err;
        }

        /*
         * hdfdims is ordered: ZYX or YX
         * in->dims is ordered: XYZ
         */
        if (in->rank == 2) {
            in->dims[0] = hdfdims[1];
            in->dims[1] = hdfdims[0];
            in->dims[2] = 1;
        }
        else if (in->rank == 3) {
            in->dims[0] = hdfdims[2];
            in->dims[1] = hdfdims[1];
            in->dims[2] = hdfdims[0];
        }
        else {
            fprintf(stderr, err2, in->rank, infile);
            goto err;
        }

        /*
         * get the rank and dimensions from files of other input formats
         *
         */
    }
    else {
        if (gint(infile, strm, &in->dims[2], in)) {
            fprintf(stderr, err1, infile);
            goto err;
        }
        if (in->dims[2] > 1)
            in->rank = 3;
        else
            in->rank = 2;
        if (gint(infile, strm, &in->dims[1], in)) {
            fprintf(stderr, err1, infile);
            goto err;
        }
        if (gint(infile, strm, &in->dims[0], in)) {
            fprintf(stderr, err1, infile);
            goto err;
        }
    }

    /*
     * validate dimension sizes
     */
    if ((in->dims[0] < 2) || (in->dims[1] < 2)) {
        fprintf(stderr, err3, infile);
        goto err;
    }

#ifdef DEBUG
    printf("\nInput Information ...\n\n");
    printf("\trank:\n\n\t%d\n\n", in->rank);
    printf("\tdimensions (nplanes,nrows,ncols):\n\n");
    printf("\t%d %d %d\n\n", in->dims[2], in->dims[1], in->dims[0]);
#endif /* DEBUG */

    free(sds_name);
    return (0);

err:
    free(sds_name);
    return (1);
}

/*
 * Name:
 *      gfloat
 *
 * Purpose:
 *      Read in a single floating point value from the input stream.  The
 *      input format may either be ASCII text , 32-bit native floating point,
 *      or 64-bit native floating point.
 */
static int
gfloat(char *infile, FILE *strm, float32 *fp32, struct Input *in)
{
    float64 fp64 = 0.0;

    const char *err1 = "Unable to get 'float' value from file: %s.\n";

    if (in->is_text == TRUE) {
        if (fscanf(strm, "%e", fp32) != 1) {
            fprintf(stderr, err1, infile);
            goto err;
        }
    }
    else if (in->is_fp32 == TRUE) {
        if (fread((char *)fp32, sizeof(float32), 1, strm) != 1) {
            fprintf(stderr, err1, infile);
            goto err;
        }
    }
    else {
        if (fread((char *)&fp64, sizeof(float64), 1, strm) != 1) {
            fprintf(stderr, err1, infile);
            goto err;
        }
        *fp32 = (float32)fp64;
    }

    return (0);

err:
    return (1);
}

/*
 * Name: (pkamat - New function)
 *      gfloat64
 *
 * Purpose:
 *      Read in a double floating point value from the input stream.  The
 *      input format may either be ASCII text ,
 *      or 64-bit native floating point.
 */

static int
gfloat64(char *infile, FILE *strm, float64 *fp64, struct Input *in)
{
    const char *err1 = "Unable to get 'float' value from file: %s.\n";

    if (in->is_text == TRUE) {
        if (fscanf(strm, "%le", fp64) != 1) {
            fprintf(stderr, err1, infile);
            goto err;
        }
    }
    else {
        if (fread((char *)fp64, sizeof(float64), 1, strm) != 1) {
            fprintf(stderr, err1, infile);
            goto err;
        }
    }

    return (0);

err:
    return (1);
}

/*
 * Name:
 *      gint
 *
 * Purpose:
 *      Read in a single integer value from the input stream.  The input
 *      format may either be ASCII text or a native BCD of type integer.
 */
static int
gint(char *infile, FILE *strm, int32 *ival, struct Input *in)
{
    const char *err1 = "Unable to get 'int' value from file: %s.\n";
    /*
     * process TEXT-formatted input
     */
    if (in->is_text == TRUE) {
        if (fscanf(strm, "%d", ival) != 1) {
            fprintf(stderr, err1, infile);
            goto err;
        }

        /*
         * process BCD-formatted input
         */
    }
    else {
        if (fread((char *)ival, sizeof(int), 1, strm) != 1) {
            fprintf(stderr, err1, infile);
            goto err;
        }
    }

    return (0);

err:
    return (1);
}

/*
 * Name: (pkamat - New function)
 *      gint32
 *
 * Purpose:
 *      Read in a single 32-bit integer value from the input stream.  The input
 *      format may either be ASCII text or a native BCD of type integer.
 */

static int
gint32(char *infile, FILE *strm, int32 *ival, struct Input *in)
{
    const char *err1 = "Unable to get 'int32' value from file: %s.\n";
    /*
     * process TEXT-formatted input
     */
    if (in->is_text == TRUE) {
        if (fscanf(strm, "%d", ival) != 1) {
            fprintf(stderr, err1, infile);
            goto err;
        }

        /*
         * process BCD-formatted input
         */
    }
    else {
        if (fread((char *)ival, sizeof(int32), 1, strm) != 1) {
            fprintf(stderr, err1, infile);
            goto err;
        }
    }

    return (0);

err:
    return (1);
}

/*
 * Name: (pkamat - New function)
 *      gint16
 *
 * Purpose:
 *      Read in a single 16-bit integer value from the input stream.  The input
 *      format may either be ASCII text or a native BCD of type 16-bit integer.
 */

static int
gint16(char *infile, FILE *strm, int16 *ival, struct Input *in)
{
    const char *err1 = "Unable to get 'int16' value from file: %s.\n";

    if (in->is_text == TRUE) {
        if (fscanf(strm, "%hd", ival) != 1) {
            fprintf(stderr, err1, infile);
            goto err;
        }
    }

    else {
        if (fread((char *)ival, sizeof(int16), 1, strm) != 1) {
            fprintf(stderr, err1, infile);
            goto err;
        }
    }

    return (0);

err:
    return (1);
}

/*
 * Name: (pkamat - New function)
 *      gint8
 *
 * Purpose:
 *      Read in a single 8-bit integer value from the input stream.  The input
 *      format may either be ASCII text or a native BCD of type 8-bit integer.
 */

static int
gint8(char *infile, FILE *strm, int8 *ival, struct Input *in)
{
    const char *err1 = "Unable to get 'int8' value from file: %s.\n";
    int16       temp;

    if (in->is_text == TRUE) {
        if (fscanf(strm, "%hd", &temp) != 1) {
            fprintf(stderr, err1, infile);
            goto err;
        }
        *ival = (int8)temp;
    }
    else {
        if (fread((char *)ival, sizeof(int8), 1, strm) != 1) {
            fprintf(stderr, err1, infile);
            goto err;
        }
    }
    return (0);

err:
    return (1);
}
/*
 * Name:
 *      gmaxmin
 *
 * Purpose:
 *      Extract the maximum and minimum data values from the input file.
 *      Supports 32-bit integer, 16-bit integer, 8-bit integer, 32-bit float, 64-bit float
 * Revision: (pvn) March 14, 2006
 *      Used the SD interface instead of DFSD
 * Revision: (bmribler - 2006/8/18)
 *    Removed SDstart call here, used passed-in SD id from process() instead.
 *    Replaced first parameter with 'struct infilesformat' to use both
 *    the file name and the SD identifier (handle.)
 */
static int
gmaxmin(struct infilesformat infile_info, struct Input *in, FILE *strm, int *is_maxmin)
{
    const char *err1 = "Unable to get max/min values from file: %s.\n";

    /*
     * extract the max/min values from the input file
     */
    if (in->is_hdf == TRUE) {
        int32 sds_id, sd_index = 0;
        intn  status;

        sds_id = SDselect(infile_info.handle, sd_index);
        status = SDgetrange(sds_id, &in->max, &in->min);
        if (status != FAIL) {
            if (in->max > in->min)
                *is_maxmin = TRUE;
        }

        /* terminate access to the array. */
        if (SDendaccess(sds_id) == FAIL)
            goto err;
    }
    else /* input file is not an HDF file */
    {
        char *infile = infile_info.filename;
        if (in->outtype == FP_32) {
            if (gfloat(infile, strm, &in->max, in)) {
                fprintf(stderr, err1, infile);
                goto err;
            }
            if (gfloat(infile, strm, &in->min, in)) {
                fprintf(stderr, err1, infile);
                goto err;
            }
            if (in->max > in->min)
                *is_maxmin = TRUE;
        }
        if (in->outtype == FP_64) {
            if (gfloat64(infile, strm, &in->fp64s.max, in)) {
                fprintf(stderr, err1, infile);
                goto err;
            }
            if (gfloat64(infile, strm, &in->fp64s.min, in)) {
                fprintf(stderr, err1, infile);
                goto err;
            }
            if (in->fp64s.max > in->fp64s.min)
                *is_maxmin = TRUE;
        }
        if (in->outtype == INT_32) {
            if (gint32(infile, strm, &in->in32s.max, in)) {
                fprintf(stderr, err1, infile);
                goto err;
            }
            if (gint32(infile, strm, &in->in32s.min, in)) {
                fprintf(stderr, err1, infile);
                goto err;
            }
            if (in->in32s.max > in->in32s.min)
                *is_maxmin = TRUE;
        }

        if (in->outtype == INT_16) {
            if (gint16(infile, strm, &in->in16s.max, in)) {
                fprintf(stderr, err1, infile);
                goto err;
            }
            if (gint16(infile, strm, &in->in16s.min, in)) {
                fprintf(stderr, err1, infile);
                goto err;
            }
            if (in->in16s.max > in->in16s.min)
                *is_maxmin = TRUE;
        }

        if (in->outtype == INT_8) {
            if (gint8(infile, strm, &in->in8s.max, in)) {
                fprintf(stderr, err1, infile);
                goto err;
            }
            if (gint8(infile, strm, &in->in8s.min, in)) {
                fprintf(stderr, err1, infile);
                goto err;
            }
            if (in->in8s.max > in->in8s.min)
                *is_maxmin = TRUE;
        }
    }

#ifdef DEBUG
    printf("\tinput maximum/minimum values:\n\n");
    printf("\t%E %E\n\n", in->max, in->min);
#endif /* DEBUG */

    return (0);

err:
    return (1);
}

/*
 * Name:
 *      gscale
 *
 * Purpose:
 *      Determine the scale for each axis.
 *
 * Revision: (pkamat)
 *      Modified to support 32-bit integer, 16-bit integer, 8-bit integer in
 *        addition to 32-bit float and 64-bit float
 * Revision: (pvn) March 14, 2006
 *      Used the SD interface instead of DFSD
 * Revision: (bmribler - 2006/8/18)
 *    Removed SDstart call here, used passed-in SD id from process() instead.
 *    Replaced first parameter with 'struct infilesformat' to use both
 *    the file name and the SD identifier (handle.)
 */
static int
gscale(struct infilesformat infile_info, struct Input *in, FILE *strm, int *is_scale)
{
    int   i;
    int32 hdfdims[3]; /* order: ZYX or YX */

    const char *err1 = "Unable to get axis scale from file: %s.\n";

    *is_scale = TRUE;

    /*
     * hdfdims is ordered: ZYX or YX
     * in->dims is ordered: XYZ
     */
    if (in->rank == 2) {
        hdfdims[0] = in->dims[1];
        hdfdims[1] = in->dims[0];
    }
    else {
        hdfdims[0] = in->dims[2];
        hdfdims[1] = in->dims[1];
        hdfdims[2] = in->dims[0];
    }

    /*
     * extract the scale values from the input file
     */
    if (in->is_hdf == TRUE) {
        int32 sds_id, dim_id, sd_index = 0;
        int32 sd_id = infile_info.handle; /* shortcut for handle from SDstart */

        /* select the SDS */
        sds_id = SDselect(sd_id, sd_index);

        /* if the SDS is two-dimensional... */
        if (in->rank == 2) {
            /* select the dimension */
            dim_id = SDgetdimid(sds_id, 0);
            if (SDgetdimscale(dim_id, in->vscale) == FAIL)
                goto err;

            dim_id = SDgetdimid(sds_id, 1);
            if (SDgetdimscale(dim_id, in->hscale) == FAIL)
                goto err;
        }
        else /* ...three-dimensional... */
        {
            dim_id = SDgetdimid(sds_id, 0);
            if (SDgetdimscale(dim_id, in->dscale) == FAIL)
                goto err;

            dim_id = SDgetdimid(sds_id, 1);
            if (SDgetdimscale(dim_id, in->vscale) == FAIL)
                goto err;

            dim_id = SDgetdimid(sds_id, 2);
            if (SDgetdimscale(dim_id, in->hscale) == FAIL)
                goto err;
        }

        /* terminate access to the array. */
        if (SDendaccess(sds_id) == FAIL)
            goto err;
    }
    else /* input file is not an HDF file */
    {
        char infile[NAME_LEN];
        strcpy(infile, infile_info.filename);
        switch (in->outtype) {
            case 0: /* 32-bit float */
                if (in->rank == 2) {
                    for (i = 0; i < hdfdims[0]; i++) {
                        if (gfloat(infile, strm, &in->vscale[i], in)) {
                            fprintf(stderr, err1, infile);
                            goto err;
                        }
                    }
                    in->vscale[i] = in->vscale[i - 1];
                    for (i = 0; i < hdfdims[1]; i++) {
                        if (gfloat(infile, strm, &in->hscale[i], in)) {
                            fprintf(stderr, err1, infile);
                            goto err;
                        }
                    }

                    in->hscale[i] = in->hscale[i - 1];
                }
                else {
                    for (i = 0; i < hdfdims[0]; i++) {
                        if (gfloat(infile, strm, &in->dscale[i], in)) {
                            fprintf(stderr, err1, infile);
                            goto err;
                        }
                    }
                    in->dscale[i] = in->dscale[i - 1];
                    for (i = 0; i < hdfdims[1]; i++) {
                        if (gfloat(infile, strm, &in->vscale[i], in)) {
                            fprintf(stderr, err1, infile);
                            goto err;
                        }
                    }
                    in->vscale[i] = in->vscale[i - 1];
                    for (i = 0; i < hdfdims[2]; i++) {
                        if (gfloat(infile, strm, &in->hscale[i], in)) {
                            fprintf(stderr, err1, infile);
                            goto err;
                        }
                    }
                    in->hscale[i] = in->hscale[i - 1];
                }
                break;

            case 1: /* 64-bit float */
                if (in->rank == 2) {
                    for (i = 0; i < hdfdims[0]; i++) {
                        if (gfloat64(infile, strm, &in->fp64s.vscale[i], in)) {
                            fprintf(stderr, err1, infile);
                            goto err;
                        }
                    }
                    in->fp64s.vscale[i] = in->fp64s.vscale[i - 1];
                    for (i = 0; i < hdfdims[1]; i++) {
                        if (gfloat64(infile, strm, &in->fp64s.hscale[i], in)) {
                            fprintf(stderr, err1, infile);
                            goto err;
                        }
                    }
                    in->fp64s.hscale[i] = in->fp64s.hscale[i - 1];
                }
                else {
                    for (i = 0; i < hdfdims[0]; i++) {
                        if (gfloat64(infile, strm, &in->fp64s.dscale[i], in)) {
                            fprintf(stderr, err1, infile);
                            goto err;
                        }
                    }
                    in->fp64s.dscale[i] = in->fp64s.dscale[i - 1];

                    for (i = 0; i < hdfdims[1]; i++) {
                        if (gfloat64(infile, strm, &in->fp64s.vscale[i], in)) {
                            fprintf(stderr, err1, infile);
                            goto err;
                        }
                    }
                    in->fp64s.vscale[i] = in->fp64s.vscale[i - 1];

                    for (i = 0; i < hdfdims[2]; i++) {
                        if (gfloat64(infile, strm, &in->fp64s.hscale[i], in)) {
                            fprintf(stderr, err1, infile);
                            goto err;
                        }
                    }
                    in->fp64s.hscale[i] = in->fp64s.hscale[i - 1];
                }
                break;

            case 2: /* 32-bit integer */
                if (in->rank == 2) {
                    for (i = 0; i < hdfdims[0]; i++) {
                        if (gint32(infile, strm, &in->in32s.vscale[i], in)) {
                            fprintf(stderr, err1, infile);
                            goto err;
                        }
                    }
                    in->in32s.vscale[i] = in->in32s.vscale[i - 1];
                    for (i = 0; i < hdfdims[1]; i++) {
                        if (gint32(infile, strm, &in->in32s.hscale[i], in)) {
                            fprintf(stderr, err1, infile);
                            goto err;
                        }
                    }
                    in->in32s.hscale[i] = in->in32s.hscale[i - 1];
                }

                else {
                    for (i = 0; i < hdfdims[0]; i++) {
                        if (gint32(infile, strm, &in->in32s.dscale[i], in)) {
                            fprintf(stderr, err1, infile);
                            goto err;
                        }
                    }
                    in->in32s.dscale[i] = in->in32s.dscale[i - 1];
                    for (i = 0; i < hdfdims[1]; i++) {
                        if (gint32(infile, strm, &in->in32s.vscale[i], in)) {
                            fprintf(stderr, err1, infile);
                            goto err;
                        }
                    }
                    in->in32s.vscale[i] = in->in32s.vscale[i - 1];
                    for (i = 0; i < hdfdims[2]; i++) {
                        if (gint32(infile, strm, &in->in32s.hscale[i], in)) {
                            fprintf(stderr, err1, infile);
                            goto err;
                        }
                    }
                    in->in32s.hscale[i] = in->in32s.hscale[i - 1];
                }
                break;

            case 3: /* 16-bit integer */
                if (in->rank == 2) {
                    for (i = 0; i < hdfdims[0]; i++) {
                        if (gint16(infile, strm, &in->in16s.vscale[i], in)) {
                            fprintf(stderr, err1, infile);
                            goto err;
                        }
                    }
                    in->in16s.vscale[i] = in->in16s.vscale[i - 1];
                    for (i = 0; i < hdfdims[1]; i++) {
                        if (gint16(infile, strm, &in->in16s.hscale[i], in)) {
                            fprintf(stderr, err1, infile);
                            goto err;
                        }
                    }
                    in->in16s.hscale[i] = in->in16s.hscale[i - 1];
                }

                else {
                    for (i = 0; i < hdfdims[0]; i++) {
                        if (gint16(infile, strm, &in->in16s.dscale[i], in)) {
                            fprintf(stderr, err1, infile);
                            goto err;
                        }
                    }
                    in->in16s.dscale[i] = in->in16s.dscale[i - 1];
                    for (i = 0; i < hdfdims[1]; i++) {
                        if (gint16(infile, strm, &in->in16s.vscale[i], in)) {
                            fprintf(stderr, err1, infile);
                            goto err;
                        }
                    }
                    in->in16s.vscale[i] = in->in16s.vscale[i - 1];
                    for (i = 0; i < hdfdims[2]; i++) {
                        if (gint16(infile, strm, &in->in16s.hscale[i], in)) {
                            fprintf(stderr, err1, infile);
                            goto err;
                        }
                    }
                    in->in16s.hscale[i] = in->in16s.hscale[i - 1];
                }
                break;

            case 4: /* 8-bit integer */
                if (in->rank == 2) {
                    for (i = 0; i < hdfdims[0]; i++) {
                        if (gint8(infile, strm, &in->in8s.vscale[i], in)) {
                            fprintf(stderr, err1, infile);
                            goto err;
                        }
                    }
                    in->in8s.vscale[i] = in->in8s.vscale[i - 1];
                    for (i = 0; i < hdfdims[1]; i++) {
                        if (gint8(infile, strm, &in->in8s.hscale[i], in)) {
                            fprintf(stderr, err1, infile);
                            goto err;
                        }
                    }
                    in->in8s.hscale[i] = in->in8s.hscale[i - 1];
                }

                else {
                    for (i = 0; i < hdfdims[0]; i++) {
                        if (gint8(infile, strm, &in->in8s.dscale[i], in)) {
                            fprintf(stderr, err1, infile);
                            goto err;
                        }
                    }
                    in->in8s.dscale[i] = in->in8s.dscale[i - 1];
                    for (i = 0; i < hdfdims[1]; i++) {
                        if (gint8(infile, strm, &in->in8s.vscale[i], in)) {
                            fprintf(stderr, err1, infile);
                            goto err;
                        }
                    }
                    in->in8s.vscale[i] = in->in8s.vscale[i - 1];
                    for (i = 0; i < hdfdims[2]; i++) {
                        if (gint8(infile, strm, &in->in8s.hscale[i], in)) {
                            fprintf(stderr, err1, infile);
                            goto err;
                        }
                    }
                    in->in8s.hscale[i] = in->in8s.hscale[i - 1];
                }
                break;
        }
    }

#ifdef DEBUG
    if (in->rank == 2) {
        printf("\tscales of the axes (vert,horiz):\n\n\t");
        for (i = 0; i < hdfdims[0]; i++)
            printf("%E ", in->vscale[i]);
        printf("\n\t");
        for (i = 0; i < hdfdims[1]; i++)
            printf("%E ", in->hscale[i]);
    }
    else {
        printf("\tscales of the axes (depth,vert,horiz):\n\n\t");
        for (i = 0; i < hdfdims[0]; i++)
            printf("%E ", in->dscale[i]);
        printf("\n\t");
        for (i = 0; i < hdfdims[1]; i++)
            printf("%E ", in->vscale[i]);
        printf("\n\t");
        for (i = 0; i < hdfdims[2]; i++)
            printf("%E ", in->hscale[i]);
    }
    printf("\n\n\n");
#endif /* DEBUG */

    return (0);

err:
    return (1);
}

/*
 * Name:
 *      gtoken
 *
 * Purpose:
 *      Return the token identifier associated with the command line
 *      argument.
 */
static int
gtoken(char *s)
{
    size_t len;
    int    token;

    const char *err1 = "Illegal argument: %s.\n";

    /*
     * identify the token type
     */
    if (s[0] == '-') { /* option name (or negative number) */
        token = ERR;
        len   = strlen(&s[1]);
        switch (s[1]) {
            case 'o':
                if (!strncmp("outfile", &s[1], len))
                    token = OPT_o;
                break;
            case 'r':
                if (!strncmp("raster", &s[1], len))
                    token = OPT_r;
                break;
            case 'e':
                if (!strncmp("expand", &s[1], len))
                    token = OPT_e;
                break;
            case 'i':
                if (!strncmp("interp", &s[1], len))
                    token = OPT_i;
                break;
            case 'p':
                if (!strncmp("palfile", &s[1], len))
                    token = OPT_p;
                break;
            case 'f':
                if (!strncmp("float", &s[1], len))
                    token = OPT_f;
                break;
            case 'h':
                if (!strncmp("help", &s[1], len))
                    token = OPT_h;
                break;
            case 'm':
                if (!strncmp("mean", &s[1], len))
                    token = OPT_m;
                break;
            case 'n':
                token = OPT_n;
                break;
            case 't':
                token = OPT_t;
                break;
            default:
                if (isnum(s)) /* negative number? */
                    token = OPT_num;
        }
        if (token == ERR)
            fprintf(stderr, err1, s);
    }
    else if (isnum(s)) /* positive number */
        token = OPT_num;
    else /* filename */
        token = FILENAME;

    return (token);
}

/*
 * Name:
 *      gtype
 *
 * Purpose:
 *      Determine the type of the input file (HDF, TEXT, FP32, FP64)
 *
 * Revision: (pkamat)
 *        Modified to support INT32, INT16, INT8 formats.
 *      Also determines and validates the outtype type of the data-set
 */
static int
gtype(char *infile, struct Input *in, FILE **strm)
{
    char buf[8];

    const char *err1 = "Unable to open file: %s.\n";
    const char *err2 = "Unable to get format tag from file: %s.\n";
    const char *err3 = "Invalid file format in file: %s.\n";
    const char *err4 =
        "Invalid use of -t or -n options. Can be used only for TEXT files or for FP64 binary files\n";

    /*
     * determine the input file format
     */
    if (Hishdf(infile))
        in->is_hdf = TRUE;
    else {
        if ((*strm = fopen(infile, "r")) == NULL) {
            fprintf(stderr, err1, infile);
            goto err;
        }
        if (fread(buf, 4, 1, *strm) != 1) {
            fprintf(stderr, err2, infile);
            goto err;
        }
        if (!memcmp("TEXT", buf, 4) || !memcmp("text", buf, 4)) {
            in->is_text = TRUE;
            if (in->outtype == NO_NE)
                in->outtype = FP_32;
        }
        else {
            if (!memcmp("FP64", buf, 4) || !memcmp("fp64", buf, 4)) {
                in->is_fp64 = TRUE;
                if (in->outtype != FP_64) {
                    if (in->outtype != NO_NE) {
                        fprintf(stderr, err4, infile);
                        goto err;
                    }
                    else {
                        in->outtype = FP_32;
                    }
                }
            }
            else {
                if (in->outtype != NO_NE) {
                    fprintf(stderr, err4, infile);
                    goto err;
                }
                if (!memcmp("FP32", buf, 4) || !memcmp("fp32", buf, 4)) {
                    in->is_fp32 = TRUE;
                    in->outtype = FP_32;
                }

                else if (!memcmp("IN32", buf, 4) || !memcmp("in32", buf, 4))
                    in->outtype = INT_32;
                else if (!memcmp("IN16", buf, 4) || !memcmp("in16", buf, 4))
                    in->outtype = INT_16;
                else if (!memcmp("IN08", buf, 4) || !memcmp("in08", buf, 4))
                    in->outtype = INT_8;
                else {
                    fprintf(stderr, err3, infile);
                    goto err;
                }
                if (in->outtype == NO_NE) {
                    fprintf(stderr, err4, infile);
                    goto err;
                }
            }
        }
    }

    return (0);

err:
    return (1);
}

/*
 * Name:
 *      help
 *
 * Purpose:
 *      Print a helpful summary of command usage and features.
 */
void
help(char *name)
{
    printf("Name:\n");
    printf("\t%s (previously fp2hdf)\n\n", name);
    printf("Purpose:\n");
    printf("\tTo convert floating point data to HDF Scientific ");
    printf("Data Set (SDS)\n");
    printf("\tand/or 8-bit Raster Image Set (RIS8) format, ");
    printf("storing the results\n");
    printf("\tin an HDF file.  The image data can be scaled ");
    printf("about a mean value.\n\n");

    fprintf(stderr, "Synopsis:");
    fprintf(stderr, "\n\t%s  -h[elp]", name);
    fprintf(stderr, "\n\t\t Print this summary of usage and exit.");
    fprintf(stderr, "\n\t\t ");
    fprintf(stderr, "\n\t%s  -V", name);
    fprintf(stderr, "\n\t\t Print version of the HDF4 library and exit.");
    fprintf(stderr, "\n\t\t ");
    fprintf(stderr,
            "\n\t%s  <infile> [ [-t[ype] <output-type> | -n] [<infile> [-t[ype] <output-type> | -n]...]",
            name);
    fprintf(stderr, "\n\t\t\t\t\t-o[utfile] <outfile> [-r[aster] [ras_opts ...]] [-f[loat]]");

    fprintf(stderr, "\n\n\t <infile(s)>:");
    fprintf(stderr, "\n\t\t Name of the input file(s), containing a single ");
    fprintf(stderr, "\n\t\t two-dimensional or three-dimensional floating point array ");
    fprintf(stderr, "\n\t\t in either ASCII text, native floating point, native integer ");
    fprintf(stderr, "\n\t\t or HDF SDS format.  If an HDF file is used for input, it ");
    fprintf(stderr, "\n\t\t must contain an SDS. The SDS need only contain a dimension ");
    fprintf(stderr, "\n\t\t record and the data, but if it also contains maximum and ");
    fprintf(stderr, "\n\t\t minimum values and/or scales for each axis, these will ");
    fprintf(stderr, "\n\t\t be used.  If the input format is ASCII text or native ");
    fprintf(stderr, "\n\t\t floating point or native integer, see \"Notes\" below on ");
    fprintf(stderr, "\n\t\t how it must be organized.");

    fprintf(stderr, "\n\n\t -t[ype] <output_type>: ");
    fprintf(stderr, "\n\t\t Optionally used for every input ASCII file to specify the ");
    fprintf(stderr, "\n\t\t data type of the data-set to be written. If not specified               ");
    fprintf(stderr, "\n\t\t default data type is 32-bit floating point. <output-type>");
    fprintf(stderr, "\n\t\t can be any of the following: FP32 (default), FP64, INT32");
    fprintf(stderr, "\n\t\t INT16, INT8. It can be used only with ASCII files.");

    fprintf(stderr, "\n\n\t -n:  ");
    fprintf(stderr, "\n\t\t This option is to be used only if the binary input file ");
    fprintf(stderr, "\n\t\t contains 64-bit floating point data and the default");
    fprintf(stderr, "\n\t\t behaviour (default behaviour is to write it to a 32-bit");
    fprintf(stderr, "\n\t\t floating point data-set) should be overridden to write ");
    fprintf(stderr, "\n\t\t it to a 64-bit floating point data-set.");

    fprintf(stderr, "\n\n\t -o[utfile] <outfile>:");
    fprintf(stderr, "\n\t\t Data from one or more input files are stored as one or");
    fprintf(stderr, "\n\t\t more data sets and/or images in one HDF output file,");
    fprintf(stderr, "\n\t\t \"outfile\".");

    fprintf(stderr, "\n\n\t -r[aster]:");
    fprintf(stderr, "\n\t\t Store output as a raster image set in the output file.");

    fprintf(stderr, "\n\n\t -f[loat]:");
    fprintf(stderr, "\n\t Store output as a scientific data set in the output file.");
    fprintf(stderr, "\n\t This is the default if the \"-r\" option is not specified.");

    fprintf(stderr, "\n\n\t ras_opts ...");
    fprintf(stderr, "\n\n\t -e[xpand] <horiz> <vert> [<depth>]:");
    fprintf(stderr, "\n\t Expand float data via pixel replication to produce the");
    fprintf(stderr, "\n\t image(s).  \"horiz\" and \"vert\" give the horizontal and");
    fprintf(stderr, "\n\t vertical resolution of the image(s) to be produced; and");
    fprintf(stderr, "\n\t optionally, \"depth\" gives the number of images or depth");
    fprintf(stderr, "\n\t planes (for 3D input data).");

    fprintf(stderr, "\n\n\t -i[nterp] <horiz> <vert> [<depth>]:");
    fprintf(stderr, "\n\t\t Apply bilinear, or trilinear, interpolation to the float");
    fprintf(stderr, "\n\t\t data to produce the image(s).  \"horiz\", \"vert\", and \"depth\"");
    fprintf(stderr, "\n\t\t must be greater than or equal to the dimensions of the");
    fprintf(stderr, "\n\t\t original dataset.");
    fprintf(stderr, "\n\t\t If max and min are supplied in input file, this option clips");
    fprintf(stderr, "\n\t\t values that are greater than max or less then min, setting");
    fprintf(stderr, "\n\t\t them to the max and min, respectively.");

    fprintf(stderr, "\n\n\t -p[alfile] <palfile>:");
    fprintf(stderr, "\n\t\t Store the palette with the image.  Get the palette from");
    fprintf(stderr, "\n\t\t \"palfile\"; which may be an HDF file containing a palette,");
    fprintf(stderr, "\n\t\t or a file containing a raw palette.");

    fprintf(stderr, "\n\n\t -m[ean] <mean>:");
    fprintf(stderr, "\n\t\t If a floating point mean value is given, the image will be");
    fprintf(stderr, "\n\t\t scaled about the mean.  The new extremes (newmax and newmin),");
    fprintf(stderr, "\n\t\t as given by:");

    fprintf(stderr, "\n\n\t\t\t newmax = mean + max(abs(max-mean), abs(mean-min))");
    fprintf(stderr, "\n\t\t\t newmin = mean - max(abs(max-mean), abs(mean-min))");

    fprintf(stderr, "\n\n\t\t will be equidistant from the mean value.  If no mean value");
    fprintf(stderr, "\n\t\t is given, then the mean will be:  0.5   (max + min)");

    fprintf(stderr, "\n\n\t Notes:");
    fprintf(
        stderr,
        "\n\t\t If the input file format is ASCII text or native floating point or native integer(32-bit,");
    fprintf(stderr, "\n\t\t 16-bit, 8-bit), it");
    fprintf(stderr, "\n\t\t must have the following input fields:");

    fprintf(stderr, "\n\t\t format");
    fprintf(stderr, "\n\t\t nplanes");
    fprintf(stderr, "\n\t\t nrows");
    fprintf(stderr, "\n\t\t cols");
    fprintf(stderr, "\n\t\t max_value");
    fprintf(stderr, "\n\t\t min_value");
    fprintf(stderr, "\n\t\t [plane1 plane2 plane3 ...]");
    fprintf(stderr, "\n\t\t row1 row2 row3 ...");
    fprintf(stderr, "\n\t\t col1 col2 col3 ...");
    fprintf(stderr, "\n\t\t data1 data2 data3 ...");

    fprintf(stderr, "\n\n\t\t Where:");
    fprintf(stderr, "\n\n\t\t format:");
    fprintf(stderr,
            "\n\t\t\t Format designator (\"TEXT\", \"FP32\", \"FP64\", \"IN32\", \"IN16\", \"IN08\").");
    fprintf(stderr, "\n\t\t\t nplanes, nrows, ncols:");
    fprintf(stderr, "\n\t\t\t Dimensions are specified in the order slowest changing dimension first.");
    fprintf(stderr, "\n\t\t\t ncols is dimension of the fastest changing dimension. (horizontal axis");
    fprintf(stderr, "\n\t\t\t or X-axis in a 3D scale)");
    fprintf(stderr, "\n\t\t\t nrows corresponds to dimension of the vertical axis or Y-axis in a 3D ");
    fprintf(stderr, "\n\t\t\t scale.");
    fprintf(stderr, "\n\t\t\t nplanes corresponds to the slowest changing dimension i.e. dimension of ");
    fprintf(stderr, "\n\t\t\t the depth axis or the Z-axis in a 3D scale (\"1\" for 2D input).");
    fprintf(stderr, "\n\t\t max_value:");
    fprintf(stderr, "\n\t\t\t Maximum data value.");
    fprintf(stderr, "\n\t\t min_value:");
    fprintf(stderr, "\n\t\t\t Minimum data value.");
    fprintf(stderr, "\n\t\t plane1, plane2, plane3, ...:");
    fprintf(stderr, "\n\t\t\t Scales for depth axis.");
    fprintf(stderr, "\n\t\t row1, row2, row3, ...:");
    fprintf(stderr, "\n\t\t\t Scales for the vertical axis.");
    fprintf(stderr, "\n\t\t col1, col2, col3, ...:");
    fprintf(stderr, "\n\t\t\t Scales for the horizontal axis.");
    fprintf(stderr, "\n\t\t data1, data2, data3, ...:");
    fprintf(stderr, "\n\t\t\t The data ordered by rows, left to right and top");
    fprintf(stderr, "\n\t\t\t to bottom; then optionally, ordered by planes,");
    fprintf(stderr, "\n\t\t\t front to back.");

    fprintf(stderr,
            "\n\n\t\t For FP32 and FP64 input format, \"format\", \"nplanes\", \"nrows\", \"ncols\",");
    fprintf(stderr, "\n\t\t and \"nplanes\" are native integers; where \"format\" is the integer");
    fprintf(stderr, "\n\t\t representation of the appropriate 4-character string (0x46503332 for");
    fprintf(stderr, "\n\t\t \"FP32\" and 0x46503634 for \"FP64\").  The remaining input fields are");
    fprintf(stderr, "\n\t\t composed of native 32-bit floating point values for FP32 input format,");
    fprintf(stderr, "\n\t\t or native 64-bit floating point values for FP64 input format.");

    fprintf(stderr,
            "\n\n\t For IN32, IN16 and IN08 input format, \"format\", \"nplanes\", \"nrows\", \"ncols\",");
    fprintf(stderr, "\n\t\t and \"nplanes\" are native integers; where \"format\" is the integer");
    fprintf(stderr, "\n\t\t representation of the appropriate 4-character string. The remaining input ");
    fprintf(stderr, "\n\t\t fields are composed of native 32-bit integer values for IN32 input format,");
    fprintf(stderr, "\n\t\t or native 16-bit integer values for IN16 input format or native 8-bit ");
    fprintf(stderr, "\n\t\t integer values for IN08 input format.");

    printf("\nExamples:\n");
    printf("\tConvert floating point data in \"f1.txt\" to SDS ");
    printf("format, and store it\n");
    printf("\tas an SDS in HDF file \"o1\":\n\n");
    printf("\t\t%s f1.txt -o o1\n\n", name);
    printf("\tConvert floating point data in \"f2.hdf\" to ");
    printf("8-bit raster format, and\n");
    printf("\tstore it as an RIS8 in HDF file \"o2\":\n\n");
    printf("\t\t%s f2.hdf -o o2 -r\n\n", name);
    printf("\tConvert floating point data in \"f3.bin\" to ");
    printf("8-bit raster format and\n");
    printf("\tSDS format, and store both the RIS8 and the SDS ");
    printf("in HDF file \"o3\":\n\n");
    printf("\t\t%s f3.bin -o o3 -r -f\n\n", name);
    printf("\tConvert floating point data in \"f4\" to a ");
    printf("500x600 raster image, and\n");
    printf("\tstore the RIS8 in HDF file \"o4\".  Also store a ");
    printf("palette from \"palfile\"\n");
    printf("\twith the image:\n\n");
    printf("\t\t%s f4 -o o4 -r -e 500 600 -p palfile\n\n", name);
    printf("\tConvert floating point data in \"f5\" to 200 ");
    printf("planes of 500x600 raster\n");
    printf("\timages, and store the RIS8 in HDF file \"o5\".  ");
    printf("Also scale the image\n");
    printf("\tdata so that it is centered about a mean value ");
    printf("of 10.0:\n\n");
    printf("\t\t%s f5 -o o5 -r -i 500 600 200 -m 10.0\n", name);

    return;
}

/*
 * Name:
 *      indexes
 *
 * Purpose:
 *      For each pixel location along an axis, determine the nearest
 *      scale value neighbor.  Return a list of indexes into the scale
 *      array.
 */
static int
indexes(float32 *scale, int dim, int *idx, int res)
{
    int      i, j;
    float32 *midpt;
    float32  loc;
    float32  delta;

    const char *err1 = "Unable to allocate dynamic memory.\n";
    /*
     * determine the midpoints between scale values
     */
    if ((midpt = (float32 *)malloc((size_t)dim * sizeof(float32))) == NULL) {
        fprintf(stderr, "%s", err1);
        goto err;
    }
    for (i = 0; i < dim - 1; i++)
        midpt[i] = (scale[i] + scale[i + 1]) * (float32)0.5;
    midpt[dim - 1] = scale[dim - 1] + (scale[dim - 1] - midpt[dim - 2]);

    /*
     * determine the distance between pixel locations
     */
    delta = (scale[dim - 1] - scale[0]) / (float32)(res - 1);

    /*
     * compute indexes, keeping the index the same until the location
     * extends beyond the midpoint
     */
    for (i = 1, j = 0, idx[0] = 0, loc = scale[0]; i < res; i++) {
        loc += delta;
        idx[i] = idx[i - 1];
        while (loc >= midpt[j]) {
            idx[i] += 1;
            j += 1;
        }
    }

    /*
     * free dynamically allocated memory
     */
    free(midpt);

    return (0);

err:
    return (1);
}

/*
 * Name:
 *      interp
 *
 * Purpose:
 *      Use a bilinear, or trilinear, interpolation scheme to construct
 *      the raster image(s).
 *
 *  Bug revision:  the line that previously read:
 *
 *      hratio[i] = ((hrange > 0) ? 1.0 : -1.0) * (in->hscale[j+1] -
 *                    loc) / (in->hscale[j+1] - in->hscale[j]);
 *    has been changed to read:
 *      hratio[i] = (in->hscale[j+1] - loc) / (in->hscale[j+1] - in->hscale[j]);
 *
 *    Similar changes were made to the corresponding lines for
 *    computing vratio and dratio.
 *
 *  Bug revision: If values occur that are outside the ranges of the
 *    max and min values provided, these values are now "clipped" to
 *    be the same as the max and min, respectively.
 */

static int
interp(struct Input *in, struct Raster *im)
{
    int            i, j, k, m;
    int           *hinc, *voff, *doff = NULL;
    float32        pix;
    float32        loc;
    float32        range;
    float32        ratio;
    float32        hrange, vrange, drange = (float32)0.0;
    float32        hdelta, vdelta, ddelta = (float32)0.0;
    float32        t1, t2, t3, t4, t5, t6;
    float32       *hratio, *vratio, *dratio = NULL;
    float32       *pt[8];
    unsigned char *ip = im->image;

    const char *err1 = "Unable to allocate dynamic memory.\n";

    /*
     * determine the range of pixel locations
     */
    range  = in->max - in->min;
    ratio  = (float32)237.9 / range;
    hrange = in->hscale[in->dims[0] - 1] - in->hscale[0];
    vrange = in->vscale[in->dims[1] - 1] - in->vscale[0];
    if (in->rank == 3)
        drange = in->dscale[in->dims[2] - 1] - in->dscale[0];

    /*
     * determine the distance between pixel locations
     */
    hdelta = hrange / (float32)(im->hres - 1);
    vdelta = vrange / (float32)(im->vres - 1);
    if (in->rank == 3)
        ddelta = drange / (float32)(im->dres - 1);

    /*
     * allocate dynamic memory for the interpolation ratio buffers
     */
    if ((hratio = (float32 *)malloc((size_t)im->hres * sizeof(float32))) == NULL) {
        fprintf(stderr, "%s", err1);
        goto err;
    }
    if ((vratio = (float32 *)malloc((unsigned int)im->vres * sizeof(float32))) == NULL) {
        fprintf(stderr, "%s", err1);
        goto err;
    }
    if (in->rank == 3) {
        if ((dratio = (float32 *)malloc((unsigned int)im->dres * sizeof(float32))) == NULL) {
            fprintf(stderr, "%s", err1);
            goto err;
        }
    }

    /*
     * allocate dynamic memory for the pixel location offset/increment
     * buffers
     */
    if ((hinc = (int *)malloc((unsigned int)im->hres * sizeof(int))) == NULL) {
        fprintf(stderr, "%s", err1);
        goto err;
    }
    if ((voff = (int *)malloc((unsigned int)(im->vres + 1) * sizeof(int))) == NULL) {
        fprintf(stderr, "%s", err1);
        goto err;
    }
    if (in->rank == 3) {
        if ((doff = (int *)malloc((unsigned int)(im->dres + 1) * sizeof(int))) == NULL) {
            fprintf(stderr, "%s", err1);
            goto err;
        }
    }

    /*
     * compute the interpolation ratios and pixel location
     * offsets/increments for each axis
     */
    for (i = 0, j = 0; i < im->hres; i++) {
        loc     = hdelta * (float)i + in->hscale[0];
        hinc[i] = 0;
        while ((j < (in->dims[0] - 2)) &&
               ((hrange > (float32)0.0) ? (in->hscale[j + 1] < loc) : (in->hscale[j + 1] > loc))) {
            hinc[i] += 1;
            j += 1;
        }
        hratio[i] = (in->hscale[j + 1] - loc) / (in->hscale[j + 1] - in->hscale[j]);
    }
    for (i = 0, j = 0, voff[0] = 0; i < im->vres; i++) {
        loc = vdelta * (float)i + in->vscale[0];
        while ((j < (in->dims[1] - 2)) &&
               ((vrange > (float32)0.0) ? (in->vscale[j + 1] < loc) : (in->vscale[j + 1] > loc))) {
            voff[i] += 1;
            j += 1;
        }
        vratio[i]   = (in->vscale[j + 1] - loc) / (in->vscale[j + 1] - in->vscale[j]);
        voff[i + 1] = voff[i];
    }
    if (in->rank == 3) {
        for (i = 0, j = 0, doff[0] = 0; i < im->dres; i++) {
            loc = ddelta * (float)i + in->dscale[0];
            while ((j < (in->dims[2] - 2)) &&
                   ((drange > (float32)0.0) ? (in->dscale[j + 1] < loc) : (in->dscale[j + 1] > loc))) {
                doff[i] += 1;
                j += 1;
            }
            dratio[i]   = (in->dscale[j + 1] - loc) / (in->dscale[j + 1] - in->dscale[j]);
            doff[i + 1] = doff[i];
        }
    }

    /*
     * do the interpolation for each point in the target image, taking
     * advantage of the fact that the target is evenly spaced along each
     * axis
     */
    if (in->rank == 2) {
        for (i = 0; i < im->vres; i++) {
            pt[0] = (float32 *)in->data + (in->dims[0] * voff[i]);
            pt[1] = pt[0] + 1;
            pt[2] = pt[0] + in->dims[0];
            pt[3] = pt[2] + 1;
            for (j = 0; j < im->hres; j++) {
                for (m = 0; m < 4; m++)
                    pt[m] += hinc[j];
                t1  = *pt[2] - ((*pt[2] - *pt[0]) * vratio[i]);
                t2  = *pt[3] - ((*pt[3] - *pt[1]) * vratio[i]);
                pix = t2 - ((t2 - t1) * hratio[j]);
                if (pix > in->max)
                    pix = in->max; /* clip (bug fix) */
                if (pix < in->min)
                    pix = in->min; /* ditto */
                *ip++ = (unsigned char)((ratio * (pix - in->min)) + (float32)1.5);
            }
        }
    }
    else { /* rank == 3 */
        for (i = 0; i < im->dres; i++) {
            for (j = 0; j < im->vres; j++) {
                pt[0] = (float32 *)in->data + (in->dims[0] * voff[j]) + (in->dims[0] * in->dims[1] * doff[i]);
                pt[1] = pt[0] + 1;
                pt[2] = pt[0] + in->dims[0];
                pt[3] = pt[2] + 1;
                pt[4] = pt[0] + (in->dims[0] * in->dims[1]);
                pt[5] = pt[4] + 1;
                pt[6] = pt[4] + in->dims[0];
                pt[7] = pt[6] + 1;
                for (k = 0; k < im->hres; k++) {
                    for (m = 0; m < 8; m++)
                        pt[m] += hinc[k];
                    t1  = *pt[4] - ((*pt[4] - *pt[0]) * dratio[i]);
                    t2  = *pt[6] - ((*pt[6] - *pt[2]) * dratio[i]);
                    t3  = *pt[5] - ((*pt[5] - *pt[1]) * dratio[i]);
                    t4  = *pt[7] - ((*pt[7] - *pt[3]) * dratio[i]);
                    t5  = t2 - ((t2 - t1) * vratio[j]);
                    t6  = t4 - ((t4 - t3) * vratio[j]);
                    pix = t6 - ((t6 - t5) * hratio[k]);
                    if (pix > in->max)
                        pix = in->max; /* clip (bug fix) */
                    if (pix < in->min)
                        pix = in->min; /* ditto */
                    *ip++ = (unsigned char)((ratio * (pix - in->min)) + (float32)1.5);
                }
            }
        }
    }

    /*
     * free dynamically allocated memory
     */
    free(hratio);
    free(vratio);
    if (in->rank == 3)
        free(dratio);
    free(hinc);
    free(voff);
    if (in->rank == 3)
        free(doff);

    return (0);

err:
    return (1);
}

/*
 * Name:
 *      isnum
 *
 * Purpose:
 *      Determine whether or not the string is representative of an
 *      integer or floating point number.  If it is, a non-zero value
 *      is returned.  A leading (-) to denote sign is acceptable.
 */
static int
isnum(char *s)
{
    char *cp;
    int   rval = FALSE;

    /*
     * check to see if its a floating point number
     */
    cp = s;
    (void)strtod(s, &cp);
    if ((*cp == '\0') && (cp != s))
        rval = TRUE;

    /*
     * check to see if its an integer number (radix 8, 10, or 16)
     */
    else {
        cp = s;
        (void)strtol(s, &cp, 0);
        if ((*cp == '\0') && (cp != s))
            rval = TRUE;
    }

    return (rval);
}

/*
 * Name:
 *      mean
 *
 * Purpose:
 *      Reset the maximum and minimum data values to be symmetric about
 *      the user-specified mean value.
 */
void
mean(struct Input *in, struct Options *opt)
{
    float32 delta, delta_max, delta_min;

    delta_max = (float32)fabs((double)(in->max - opt->meanval));
    delta_min = (float32)fabs((double)(opt->meanval - in->min));
    delta     = (delta_max > delta_min) ? delta_max : delta_min;

    in->max = opt->meanval + delta;
    in->min = opt->meanval - delta;

    return;
}

/*
 * Name:
 *      palette
 *
 * Purpose:
 *      Process the (user specified) palette input file.
 */
static int
palette(char *palfile)
{
    unsigned char *color;
    unsigned char  pal[1024], red[256], green[256], blue[256];
    FILE          *strm;
    int            i;

    const char *err1 = "Unable to get palette from file: %s.\n";
    const char *err2 = "Unable to open palette file: %s.\n";
    const char *err3 = "Unable to set default palette.\n";

    /*
     * extract a palette from an HDF file
     */
    if (Hishdf(palfile)) {
        if (DFPgetpal(palfile, pal)) {
            fprintf(stderr, err1, palfile);
            goto err;
        }

        /*
         * read in a raw palette file
         */
    }
    else {
        if ((strm = fopen(palfile, "r")) == NULL) {
            fprintf(stderr, err2, palfile);
            goto err;
        }
        if (fread((char *)red, 1, 256, strm) != 256) {
            fprintf(stderr, err1, palfile);
            goto err;
        }
        else if (fread((char *)green, 1, 256, strm) != 256) {
            fprintf(stderr, err1, palfile);
            goto err;
        }
        else if (fread((char *)blue, 1, 256, strm) != 256) {
            fprintf(stderr, err1, palfile);
            goto err;
        }
        (void)fclose(strm);

        /*
         * interleave the R,G,B values
         */
        color = pal;
        for (i = 0; i < 256; i++) {
            *color++ = red[i];
            *color++ = green[i];
            *color++ = blue[i];
        }
    }

    /*
     * set up the palette as the default for subsequent images
     */
    if (DFR8setpalette(pal)) {
        fprintf(stderr, "%s", err3);
        goto err;
    }

    return (0);

err:
    return (1);
}

/*
 * Name:
 *      pixrep
 *
 * Purpose:
 *      Expand the image(s) to the desired resolution using pixel
 *      replication.
 */
static int
pixrep(struct Input *in, struct Raster *im)
{
    int           *hidx, *vidx, *didx;
    int            ovidx, odidx;
    int            dummy;
    int32          i, j, k;
    float32       *dp;
    float32        range;
    float32        ratio;
    unsigned char *ip, *plane, *row, *pix;

    const char *err1 = "Unable to dynamically allocate memory.\n";
    dp               = (float32 *)in->data;
    ip               = im->image;
    range            = in->max - in->min;
    ratio            = (float32)237.9 / range;

    /*
     * determine the scale indexes of the horizontal pixel locations
     */
    if ((hidx = (int *)malloc((unsigned int)(im->hres + 1) * sizeof(int))) == NULL) {
        fprintf(stderr, "%s", err1);
        goto err;
    }

    if (indexes(in->hscale, in->dims[0], hidx, im->hres))
        goto err;

    /*
     * determine the scale indexes of the vertical pixel locations
     */
    if ((vidx = (int *)malloc((unsigned int)(im->vres + 1) * sizeof(int))) == NULL) {
        fprintf(stderr, "%s", err1);
        goto err;
    }

    if (indexes(in->vscale, in->dims[1], vidx, im->vres))
        goto err;

    /*
     * determine the scale indexes of the depth plane locations
     */
    dummy = 0;
    didx  = &dummy;
    if (in->rank == 3) {
        if ((didx = (int *)malloc((unsigned int)(im->dres + 1) * sizeof(int))) == NULL) {
            fprintf(stderr, "%s", err1);
            goto err;
        }

        if (indexes(in->dscale, in->dims[2], didx, im->dres))
            goto err;
    }

    /*
     * compute the expanded image
     */
    if ((pix = (unsigned char *)malloc((unsigned int)(in->dims[0] + 1))) == NULL) {
        fprintf(stderr, "%s", err1);
        goto err;
    }
    for (k = 0, odidx = didx[0] - 1; k < im->dres; k++) {
        /*
         * construct a new depth plane
         */
        if (didx[k] > odidx) {
            for (j = 0, ovidx = vidx[0] - 1; j < im->vres; j++) {
                /*
                 * construct a new row
                 */
                if (vidx[j] > ovidx) {
                    for (i = 0; i < in->dims[0]; i++)
                        pix[i] = (unsigned char)((ratio * (*dp++ - in->min)) + (float32)1.5);
                    for (i = 0; i < im->hres; i++)
                        *ip++ = pix[hidx[i]];
                    /*
                     * repeat the previous row
                     */
                }
                else {
                    row = ip - im->hres;
                    for (i = 0; i < im->hres; i++)
                        *ip++ = *row++;
                }
                ovidx = vidx[j];
            }
            /*
             * repeat the previous depth plane
             */
        }
        else {
            plane = ip - (im->hres * im->vres);
            for (j = 0; j < im->vres; j++)
                for (i = 0; i < im->hres; i++)
                    *ip++ = plane[(j * im->hres) + i];
        }
        odidx = didx[k];
    }

    /*
     * free dynamically allocated space
     */
    free(hidx);
    free(vidx);
    if (in->rank == 3)
        free(didx);
    free(pix);

    return (0);

err:
    return (1);
}

/*
 * Name:
 *      create_SDS
 *
 * Purpose:
 *      This function contains common code that creates a two- or
 *    three-dimensional dataset, used in function 'process.'  It
 *    was factored out to reduce the length of 'process.'
 *    Returns the new SDS identifier, if success, and FAIL,
 *    otherwise. (bmribler - 2006/8/18)
 */
static int32
create_SDS(int32 sd_id, int32 nt, struct Input *in)
{
    int32 sds_id = FAIL;

    if (in->rank == 2) {
        int32 edges[2];
        edges[0] = in->dims[1];
        edges[1] = in->dims[0];
        sds_id   = SDcreate(sd_id, NULL, nt, in->rank, edges);
    }
    else {
        int32 edges[3];
        edges[0] = in->dims[2];
        edges[1] = in->dims[1];
        edges[2] = in->dims[0];
        sds_id   = SDcreate(sd_id, NULL, nt, in->rank, edges);
    }
    return (sds_id);
}

/*
 * Name:
 *      alloc_data
 *
 * Purpose:
 *      This function contains common code that allocates memory for
 *    the data buffer to hold different types of data.  It
 *    was factored out to reduce the length of 'process.'
 *    Returns SUCCEED or FAIL. (bmribler - 2006/8/18)
 */
static intn
alloc_data(void **data, int32 len, int outtype)
{
    const char *alloc_err = "Unable to dynamically allocate memory.\n";

    switch (outtype) {
        case 0: /* 32-bit float */
        case 5: /* NO_NE */
            if ((*data = (void *)malloc((size_t)len * sizeof(float32))) == NULL) {
                fprintf(stderr, "%s", alloc_err);
                return FAIL;
            }
            break;
        case 1: /* 64-bit float */
            if ((*data = (void *)malloc((size_t)len * sizeof(float64))) == NULL) {
                fprintf(stderr, "%s", alloc_err);
                return FAIL;
            }
            break;
        case 2: /* 32-bit integer */
            if ((*data = (void *)malloc((size_t)len * sizeof(int32))) == NULL) {
                fprintf(stderr, "%s", alloc_err);
                return FAIL;
            }
            break;
        case 3: /* 16-bit integer */
            if ((*data = (void *)malloc((size_t)len * sizeof(int16))) == NULL) {
                fprintf(stderr, "%s", alloc_err);
                return FAIL;
            }
            break;
        case 4: /* 8-bit integer */
            if ((*data = (void *)malloc((size_t)len * sizeof(int8))) == NULL) {
                fprintf(stderr, "%s", alloc_err);
                return FAIL;
            }
            break;
    } /* end switch */
    return SUCCEED;
} /* alloc_data */

/*
 * Name:
 *      write_SDS
 *
 * Purpose:
 *      This function contains common code, that writes a two- or
 *    three-dimensional dataset, used in function 'process.'  It
 *    was factored out to reduce the length of 'process.'
 *    Returns SUCCEED or FAIL. (bmribler - 2006/8/18)
 */
static intn
write_SDS(int32 sds_id, struct Input *in)
{
    const char *write_err = "Unable to write an SDS to the HDF output file\n";
    if (in->rank == 2) {
        int32 edges[2], start[2];
        edges[0] = in->dims[1];
        edges[1] = in->dims[0];
        start[0] = 0;
        start[1] = 0;
        if (SDwritedata(sds_id, start, NULL, edges, (void *)in->data) != 0) {
            fprintf(stderr, "%s", write_err);
            return FAIL;
        }
    }
    else {
        int32 edges[3], start[3];
        edges[0] = in->dims[2];
        edges[1] = in->dims[1];
        edges[2] = in->dims[0];
        start[0] = 0;
        start[1] = 0;
        start[2] = 0;
        if (SDwritedata(sds_id, start, NULL, edges, (void *)in->data) != 0) {
            fprintf(stderr, "%s", write_err);
            return FAIL;
        }
    }
    return SUCCEED;
} /* write_SDS */

/*
 * Name:
 *      set_dimensions
 *
 * Purpose:
 *      This function contains the common code, that sets dimension scale
 *    for a two- or three-dimensional dataset, used in function 'process.'
 *    It was factored out to reduce the length of 'process.'
 *    Returns SUCCEED or FAIL. (bmribler - 2006/8/18)
 */
static intn
set_dimensions(int32 sds_id, struct Input *in, int32 nt, void *dscale, void *vscale, void *hscale)
{
    int32       dim_id, dim_index;
    const char *dim_err = "Unable to set dimension scales\n";

    if (in->rank == 2) {
        int32 edges[2];

        edges[0] = in->dims[1];
        edges[1] = in->dims[0];

        dim_index = 0;
        dim_id    = SDgetdimid(sds_id, dim_index);

        if (SDsetdimscale(dim_id, edges[0], nt, (void *)vscale) == FAIL) {
            fprintf(stderr, "%s, dim index %d\n", dim_err, dim_index);
            return FAIL;
        }

        dim_index = 1;
        dim_id    = SDgetdimid(sds_id, dim_index);

        if (SDsetdimscale(dim_id, edges[1], nt, hscale) != 0) {
            fprintf(stderr, "%s, dim index %d\n", dim_err, dim_index);
            return FAIL;
        }
    }
    else {
        int32 edges[3];
        edges[0] = in->dims[2];
        edges[1] = in->dims[1];
        edges[2] = in->dims[0];

        dim_index = 0;
        dim_id    = SDgetdimid(sds_id, dim_index);

        if (SDsetdimscale(dim_id, edges[0], nt, dscale) != 0) {
            fprintf(stderr, "%s, dim index %d\n", dim_err, dim_index);
            return FAIL;
        }
        dim_index = 1;
        dim_id    = SDgetdimid(sds_id, dim_index);

        if (SDsetdimscale(dim_id, edges[1], nt, vscale) != 0) {
            fprintf(stderr, "%s, dim index %d\n", dim_err, dim_index);
            return FAIL;
        }
        dim_index = 2;
        dim_id    = SDgetdimid(sds_id, dim_index);

        if (SDsetdimscale(dim_id, edges[2], nt, hscale) != 0) {
            fprintf(stderr, "%s, dim index %d\n", dim_err, dim_index);
            return FAIL;
        }
    }
    return SUCCEED;
} /* set_dimensions */

/*
 * Name:
 *      process
 *
 * Purpose:
 *      Process each input file.
 *
 * Revision: (pkamat)
 *    Modified to support the writing of the data set in any of the
 *    following types: INT32, INT16, INT8 and FP64
 * Modification: pvn: March, 3, 2006
 *    handled the case of in->outtype == 5 (NO_NE), for hdf input type
 *    current version assumes that datum is DFNT_FLOAT32 for this case
 * Revision: (bmribler - 2006/8/18)
 *    - Modified to store the input SD identifier in 'struct infilesformat'
 *      so the id can be passed into various functions, instead of
 *      repeatedly calling SDstart in these functions.
 *    - Factored out common codes to make this ~900-line function become
 *      more readable and maintainable.
 */
static int
process(struct Options *opt)
{
    struct Input   in;
    struct Raster  im;
    unsigned char *ip;
    int            i, j;
    int            is_maxmin;
    int            is_scale;
    int32          len;
    FILE          *strm = NULL;
    int32          hdf;
    int32          sd_id  = FAIL;
    int32          sds_id = FAIL;

#ifdef DEBUG
    int h, v, d;
#endif /* DEBUG */

    const char *err1  = "Error creating HDF output file: %s.\n";
    const char *err1a = "Error opening the created HDF output file for writing, file %s.\n";
    const char *err2  = "Unable to dynamically allocate memory.\n";
    const char *err3a = "Warning: cannot make image smaller using -e ";
    const char *err3b = "option.\n\t %s resolution will be made the ";
    const char *err3c = "same as %s dimension of the\n\t dataset, ";
    const char *err3d = "which is: %d.\n\n";
    const char *err4  = "Unable to write an RIS8 to the HDF output file\n";
    const char *err5a = "Unable to set range to an SDS\n";
    const char *err6a = "Unable to close the SDS\n";
    const char *err6  = "Unable to close the HDF output file\n";
    /*
     * process the palette file (if one was specified)
     */
    if (opt->pal == TRUE)
        if (palette(opt->palfile))
            goto err;

    /*
     * create the HDF output file
     */
    if ((hdf = Hopen(opt->outfile, DFACC_CREATE, 0)) == FAIL) {
        fprintf(stderr, err1, opt->outfile);
        goto err;
    }
    (void)Hclose(hdf);

    /* new interface */
    if ((sd_id = SDstart(opt->outfile, DFACC_WRITE)) == FAIL) {
        fprintf(stderr, err1a, opt->outfile);
        goto err;
    }

    /*
     * main loop: process input files, one per pass
     */

    for (i = 0; i < opt->fcount; i++) {
        /*
         * initialize key parameters
         */
        in.is_hdf  = FALSE;
        in.is_text = FALSE;
        in.is_fp32 = FALSE;
        in.is_fp64 = FALSE;
        is_maxmin  = FALSE;
        is_scale   = FALSE;
        in.outtype = opt->infiles[i].outtype;

        if (Hishdf(opt->infiles[i].filename)) {
            in.is_hdf              = TRUE;
            opt->infiles[i].handle = SDstart(opt->infiles[i].filename, DFACC_RDONLY);
            if (opt->infiles[i].handle == FAIL) {
                fprintf(stderr, err1a, opt->infiles[i].filename);
                goto err;
            }
        }
        /*
         * get the file type, input data dimensions, and input data
         * max/min values
         */

        if (gtype(opt->infiles[i].filename, &in, &strm))
            goto err;

        if (gdimen(opt->infiles[i], &in, strm))
            goto err;

        if (gmaxmin(opt->infiles[i], &in, strm, &is_maxmin))
            goto err;

        /*
         * Initialize the scale variables according to the output type
         * of data set
         */
        if (init_scales(&in))
            goto err;

        /*
         * get the scale for each axis
         */
        if (gscale(opt->infiles[i], &in, strm, &is_scale))
            goto err;

        /*
         * get the input data
         */
        len = in.dims[0] * in.dims[1] * in.dims[2];

        /* allocate memory for in.data depending on in.outtype value */
        if (alloc_data(&(in.data), len, in.outtype) == FAIL)
            goto err;

        if (gdata(opt->infiles[i], &in, strm, &is_maxmin))
            goto err;

        /*
         * put the input data in the HDF output file, in SDS format
         */
        if (opt->to_float == TRUE) {
            intn status;
            switch (in.outtype) {

                case 0: /* 32-bit float */
                case 5: /* NO_NE */
                    /* create data-set */
                    sds_id = create_SDS(sd_id, DFNT_FLOAT32, &in);
                    if (sds_id == FAIL)
                        goto err;

                    if (is_scale == TRUE) {
                        /* set range */
                        if (SDsetrange(sds_id, &in.max, &in.min) != 0) {
                            fprintf(stderr, "%s", err5a);
                            goto err;
                        }

                        /* set dimension scale */
                        status = set_dimensions(sds_id, &in, DFNT_FLOAT32, (void *)in.dscale,
                                                (void *)in.vscale, (void *)in.hscale);
                        if (status == FAIL)
                            goto err;
                    }

                    /* write data to the data set */
                    if (write_SDS(sds_id, &in) == FAIL)
                        goto err;
                    break;

                case 1: /* 64-bit float */

                    /* create data-set */
                    sds_id = create_SDS(sd_id, DFNT_FLOAT64, &in);
                    if (sds_id == FAIL)
                        goto err;

                    if (is_scale == TRUE) {
                        /* set range */
                        if (SDsetrange(sds_id, &in.fp64s.max, &in.fp64s.min) != 0) {
                            fprintf(stderr, "%s", err5a);
                            goto err;
                        }

                        /* set dimension scale */
                        status = set_dimensions(sds_id, &in, DFNT_FLOAT64, (void *)in.fp64s.dscale,
                                                (void *)in.fp64s.vscale, (void *)in.fp64s.hscale);
                        if (status == FAIL)
                            goto err;
                    }

                    /* write data to the data set */
                    if (write_SDS(sds_id, &in) == FAIL)
                        goto err;
                    break;

                case 2: /* 32-bit integer */

                    /* create data-set */
                    sds_id = create_SDS(sd_id, DFNT_INT32, &in);
                    if (sds_id == FAIL)
                        goto err;

                    if (is_scale == TRUE) {
                        /* set range */
                        if (SDsetrange(sds_id, &in.in32s.max, &in.in32s.min) != 0) {
                            fprintf(stderr, "%s", err5a);
                            goto err;
                        }

                        /* set dimension scale */
                        status = set_dimensions(sds_id, &in, DFNT_INT32, (void *)in.in32s.dscale,
                                                (void *)in.in32s.vscale, (void *)in.in32s.hscale);
                        if (status == FAIL)
                            goto err;
                    }

                    /* write data to the data set */
                    if (write_SDS(sds_id, &in) == FAIL)
                        goto err;
                    break;

                case 3: /* 16-bit integer */
                    /* create data-set */
                    sds_id = create_SDS(sd_id, DFNT_INT16, &in);
                    if (sds_id == FAIL)
                        goto err;

                    if (is_scale == TRUE) {
                        /* set range */
                        if (SDsetrange(sds_id, &in.in16s.max, &in.in16s.min) != 0) {
                            fprintf(stderr, "%s", err5a);
                            goto err;
                        }

                        /* set dimension scale */
                        status = set_dimensions(sds_id, &in, DFNT_INT16, (void *)in.in16s.dscale,
                                                (void *)in.in16s.vscale, (void *)in.in16s.hscale);
                        if (status == FAIL)
                            goto err;
                    }

                    /* write data to the data set */
                    if (write_SDS(sds_id, &in) == FAIL)
                        goto err;
                    break;

                case 4: /* 8-bit integer */
                    /* create data-set */
                    sds_id = create_SDS(sd_id, DFNT_INT8, &in);
                    if (sds_id == FAIL)
                        goto err;

                    if (is_scale == TRUE) {
                        /* set range */
                        if (SDsetrange(sds_id, &in.in8s.max, &in.in8s.min) != 0) {
                            fprintf(stderr, "%s", err5a);
                            goto err;
                        }

                        /* set dimension scale */
                        status = set_dimensions(sds_id, &in, DFNT_INT8, (void *)in.in8s.dscale,
                                                (void *)in.in8s.vscale, (void *)in.in8s.hscale);
                        if (status == FAIL)
                            goto err;
                    }

                    /* write data to the data set */
                    if (write_SDS(sds_id, &in) == FAIL)
                        goto err;
                    break;
            }
            /* close data set */
            if (SDendaccess(sds_id) == FAIL) {
                fprintf(stderr, "%s", err6a);
                goto err;
            }

            /* close the input file */
            if (in.is_hdf == TRUE) {
                if (SDend(opt->infiles[i].handle) == FAIL) {
                    fprintf(stderr, "SDend failed");
                    goto err;
                }
            }
        } /* if opt->to_float == TRUE */

        /*
         * put the input data in the HDF output file, in RIS8 format
         */
        if (opt->to_image == TRUE) {
            /*
             * allocate a buffer for the output image
             */
            im.hres = (opt->hres == 0) ? in.dims[0] : opt->hres;
            if ((im.hres < in.dims[0]) && (opt->ctm == EXPAND)) {
                fprintf(stderr, "%s", err3a);
                fprintf(stderr, err3b, "Horiz.");
                fprintf(stderr, err3c, "horiz.");
                fprintf(stderr, err3d, in.dims[0]);
                im.hres   = in.dims[0];
                opt->hres = in.dims[0];
            }
            im.vres = (opt->vres == 0) ? in.dims[1] : opt->vres;
            if ((im.vres < in.dims[1]) && (opt->ctm == EXPAND)) {
                fprintf(stderr, "%s", err3a);
                fprintf(stderr, err3b, "Vert.");
                fprintf(stderr, err3c, "vert.");
                fprintf(stderr, err3d, in.dims[1]);
                im.vres   = in.dims[1];
                opt->vres = in.dims[1];
            }
            im.dres = 1;
            if (in.rank == 3) {
                im.dres = (opt->dres == 0) ? in.dims[2] : opt->dres;
                if ((im.dres < in.dims[2]) && (opt->ctm == EXPAND)) {
                    fprintf(stderr, "%s", err3a);
                    fprintf(stderr, err3b, "Depth");
                    fprintf(stderr, err3c, "depth");
                    fprintf(stderr, err3d, in.dims[2]);
                    im.dres   = in.dims[2];
                    opt->dres = in.dims[2];
                }
            }
            len = im.hres * im.vres * im.dres;
            if ((im.image = (unsigned char *)malloc((unsigned int)len)) == NULL) {
                fprintf(stderr, "%s", err2);
                goto err;
            }

            /*
             * reset max/min symmetrically about the mean value
             */
            if (opt->mean == TRUE)
                mean(&in, opt);

            /*
             * perform pixel replication or interpolation
             */
            if (opt->ctm == EXPAND) {
                if (pixrep(&in, &im))
                    goto err;
            }
            else { /* INTERP */
                if (interp(&in, &im))
                    goto err;
            }

            len = im.hres * im.vres;
            for (j = 0, ip = im.image; j < im.dres; j++, ip += len) {
                if (DFR8addimage(opt->outfile, ip, im.hres, im.vres, DFTAG_RLE)) {
                    fprintf(stderr, "%s", err4);
                    goto err;
                }
            }

#ifdef DEBUG
            printf("Output Raster Information ...\n\n");
            printf("\tresolution (horiz,vert,[depth]):\n\n");
            if (in.rank == 2)
                printf("\t%d %d\n\n", im.hres, im.vres);
            else
                printf("\t%d %d %d\n\n", im.hres, im.vres, im.dres);
            if (opt->mean == TRUE) {
                printf("\tadjusted max/min values:\n\n");
                printf("\t%f %f\n\n", in.max, in.min);
            }
            printf("\tcolor index values:");
            for (d = 0, ip = im.image; d < im.dres; d++) {
                printf("\n");
                for (v = 0; v < im.vres; v++) {
                    printf("\n");
                    for (h = 0; h < im.hres; h++, ip++)
                        printf("\t%d", *ip);
                }
            }
            printf("\n");
#endif /* DEBUG */
        }
        /*
         * free dynamically allocated space
         */
        fpdeallocate(&in, &im, opt);
    } /* end of for loop */

    /* close the output file */
    if (SDend(sd_id) != 0) {
        fprintf(stderr, "%s", err6);
        goto err;
    }

    return (0);

err:
    return (1);
}

/*
 * Name: (pkamat - New function)
 *      fpdeallocate
 *
 * Purpose:
 *      Deallocate memory of all data structures
 */

void
fpdeallocate(struct Input *in, struct Raster *im, struct Options *opt)
{
    switch (in->outtype) {
        case 0: /* 32-bit float */
        case 5: /* NO_NE */
            free(in->hscale);
            free(in->vscale);
            if (in->rank == 3)
                free(in->dscale);

            if (opt->to_image == TRUE)
                free(im->image);
            break;

        case 1: /* 64-bit float */
            free(in->fp64s.hscale);
            free(in->fp64s.vscale);
            if (in->rank == 3)
                free(in->fp64s.dscale);

            if (opt->to_image == TRUE)
                free(im->image);
            break;

        case 2: /* 32-bit integer */
            free(in->in32s.hscale);
            free(in->in32s.vscale);
            if (in->rank == 3)
                free(in->in32s.dscale);
            break;

        case 3: /* 16-bit integer */
            free(in->in16s.hscale);
            free(in->in16s.vscale);
            if (in->rank == 3)
                free(in->in16s.dscale);
            break;

        case 4: /* 8-bit integer */
            free(in->in8s.hscale);
            free(in->in8s.vscale);
            if (in->rank == 3)
                free(in->in8s.dscale);
            break;
    }
    free(in->data);
}

/*
 * Name: (pkamat - New function)
 *      init_scales
 *
 * Purpose:
 *      Initialise the data-structures to hold scale information
 * Modification: pvn: March, 3, 2006
 *  handled the case of in->outtype == 5 (NO_NE), for hdf input type
 *  current version assumes that datum is DFNT_FLOAT32 for this case
 */
static int
init_scales(struct Input *in)
{
    const char *err1 = "Unable to dynamically allocate memory.\n";
    switch (in->outtype) {
        case 0: /* 32-bit float */
        case 5: /* NO_NE */
            if ((in->hscale = (float32 *)malloc((size_t)(in->dims[0] + 1) * sizeof(float32))) == NULL) {
                fprintf(stderr, "%s", err1);
                goto err;
            }
            if ((in->vscale = (float32 *)malloc((size_t)(in->dims[1] + 1) * sizeof(float32))) == NULL) {
                fprintf(stderr, "%s", err1);
                goto err;
            }
            if (in->rank == 3) {
                if ((in->dscale = (float32 *)malloc((size_t)(in->dims[2] + 1) * sizeof(float32))) == NULL) {
                    fprintf(stderr, "%s", err1);
                    goto err;
                }
            }
            break;

        case 1: /* 64-bit float */

            if ((in->fp64s.hscale = (float64 *)malloc((size_t)(in->dims[0] + 1) * sizeof(float64))) == NULL) {
                fprintf(stderr, "%s", err1);
                goto err;
            }
            if ((in->fp64s.vscale = (float64 *)malloc((size_t)(in->dims[1] + 1) * sizeof(float64))) == NULL) {
                fprintf(stderr, "%s", err1);
                goto err;
            }
            if (in->rank == 3) {
                if ((in->fp64s.dscale = (float64 *)malloc((size_t)(in->dims[2] + 1) * sizeof(float64))) ==
                    NULL) {
                    fprintf(stderr, "%s", err1);
                    goto err;
                }
            }
            break;
        case 2: /* 32-bit integer */
            if ((in->in32s.hscale = (int32 *)malloc((size_t)(in->dims[0] + 1) * sizeof(int32))) == NULL) {
                fprintf(stderr, "%s", err1);
                goto err;
            }
            if ((in->in32s.vscale = (int32 *)malloc((size_t)(in->dims[1] + 1) * sizeof(int32))) == NULL) {
                fprintf(stderr, "%s", err1);
                goto err;
            }
            if (in->rank == 3) {
                if ((in->in32s.dscale = (int32 *)malloc((size_t)(in->dims[2] + 1) * sizeof(int32))) == NULL) {
                    fprintf(stderr, "%s", err1);
                    goto err;
                }
            }
            break;

        case 3: /* 16-bit integer */
            if ((in->in16s.hscale = (int16 *)malloc((size_t)(in->dims[0] + 1) * sizeof(int16))) == NULL) {
                fprintf(stderr, "%s", err1);
                goto err;
            }
            if ((in->in16s.vscale = (int16 *)malloc((size_t)(in->dims[1] + 1) * sizeof(int16))) == NULL) {
                fprintf(stderr, "%s", err1);
                goto err;
            }
            if (in->rank == 3) {
                if ((in->in16s.dscale = (int16 *)malloc((size_t)(in->dims[2] + 1) * sizeof(int16))) == NULL) {
                    fprintf(stderr, "%s", err1);
                    goto err;
                }
            }
            break;

        case 4: /* 8-bit integer */
            if ((in->in8s.hscale = (int8 *)malloc((size_t)(in->dims[0] + 1) * sizeof(int8))) == NULL) {
                fprintf(stderr, "%s", err1);
                goto err;
            }
            if ((in->in8s.vscale = (int8 *)malloc((size_t)(in->dims[1] + 1) * sizeof(int8))) == NULL) {
                fprintf(stderr, "%s", err1);
                goto err;
            }
            if (in->rank == 3) {
                if ((in->in8s.dscale = (int8 *)malloc((size_t)(in->dims[2] + 1) * sizeof(int8))) == NULL) {
                    fprintf(stderr, "%s", err1);
                    goto err;
                }
            }
            break;
    }
    return (0);
err:
    return (1);
}

/*
 * Name:
 *      usage
 *
 * Purpose:
 *      Print a summary of command usage.
 */
void
usage(char *name)
{
    fprintf(stderr, "\nUsage:\t%s -h[elp], OR\n", name);
    fprintf(stderr, "\t%s -V, OR\n", name);
    fprintf(stderr, "\t%s <infile> [ [-t[ype] <output-type> | -n] ", name);
    fprintf(stderr, "[<infile> [-t[ype] <output-type> | -n ]]...]\n");
    fprintf(stderr, "\t\t\t\t\t-o[utfile] <outfile> [options..]\n");
    fprintf(stderr, "\n\t-t[ype] <output_type>");

    fprintf(stderr, "\n\t\tOptionally used for every input ASCII file to specify the");
    fprintf(stderr, "\n\t\tdata type of the data-set to be written. If not specified");
    fprintf(stderr, "\n\t\tdefault data type is 32-bit floating point. <output-type>");
    fprintf(stderr, "\n\t\tcan be any of the following: FP32 (default), FP64, INT32");
    fprintf(stderr, "\n\t\tINT16, INT8. It can be used only with ASCII files.");

    fprintf(stderr, "\n\t-n");
    fprintf(stderr, "\n\t\tThis option is to be used only if the binary input file ");
    fprintf(stderr, "\n\t\tcontains 64-bit floating point data and the default");
    fprintf(stderr, "\n\t\tbehaviour (default behaviour is to write it to a 32-bit");
    fprintf(stderr, "\n\t\tfloating point data-set) should be overridden to write ");
    fprintf(stderr, "\n\t\tit to a 64-bit floating point data-set.");
    fprintf(stderr, "\n\n\toptions...\n");
    fprintf(stderr, "\n\t-r[aster]:\n");
    fprintf(stderr, "\t\tproduce an image.  Could be ");
    fprintf(stderr, "followed by:\n");
    fprintf(stderr, "\t\t-e[xpand] <horiz> <vert> ");
    fprintf(stderr, "[<depth>]:\n");
    fprintf(stderr, "\t\t\t resolution with pixel ");
    fprintf(stderr, "replication\n");
    fprintf(stderr, "\t\t-i[nterp] <horiz> <vert> ");
    fprintf(stderr, "[<depth>]:\n");
    fprintf(stderr, "\t\t\tresolution with interpolation\n");
    fprintf(stderr, "\t\t-p[alfile] <palfile>:\n");
    fprintf(stderr, "\t\t\tinclude palette from palfile\n");
    fprintf(stderr, "\t\t-m[ean] <meanval>:\n");
    fprintf(stderr, "\t\t\tmean value to scale image ");
    fprintf(stderr, "around\n");
    fprintf(stderr, "\t-f[loat]:\n");
    fprintf(stderr, "\t\tproduce floating point data\n\n");
}
