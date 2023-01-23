/*********************************************************************
 *   Copyright 1993, UCAR/Unidata
 *   See netcdf/COPYRIGHT file for copying and redistribution conditions.
 *   /hdf/src/master/mfhdf/ncdump/ncdump.h,v 1.1 1993/04/21 21:51:19 chouck Exp
 *********************************************************************/

#ifndef HDIFF_H
#define HDIFF_H

#include "hdf.h"
#include "mfhdf.h"
#include "hdiff_table.h"

#define Printf (void)printf

/* Maximum value for max_err_cnt */
#define MAX_DIFF 0x7FFFFFFF

struct ncdim { /* dimension */
    char  name[H4_MAX_NC_NAME];
    int32 size;
};

struct ncvar { /* variable */
    char  name[H4_MAX_NC_NAME];
    int32 type;
    int32 ndims;
    int32 dims[H4_MAX_VAR_DIMS];
    int32 natts;
};

struct ncatt { /* attribute */
    int32 var;
    char  name[H4_MAX_NC_NAME];
    int32 type;
    int32 len;
    void *val;
};

typedef enum { LANG_NONE, LANG_C, LANG_F } Nclang;

typedef struct {        /* selection for comparison  */
    int verbose;        /*
                         * if true, print cuurent interface comparison
                         */
    int ga;             /*
                         * if true, compare global attributes only
                         */
    int sa;             /*
                         * if true, compare SD local attributes only
                         */
    int sd;             /*
                         * if true, compare SD data only
                         */
    int gr;             /*
                         * if true, compare GR data only
                         */
    int vd;             /*
                         * if true, compare Vdata only
                         */
    uint32 max_err_cnt; /*
                         * max. no of difference to be printed
                         */
    float32 err_limit;  /*
                         * limit of difference for the comparison
                         */
    int nlvars;         /*
                         * Number of variables specified with -v option
                         * on command line
                         */
    char **lvars;       /*
                         * list of variable names specified with -v
                         * option on command line
                         */
    int nuvars;         /*
                         * Number of variables specified with -u option
                         * on command line
                         */
    char **uvars;       /*
                         * list of variable names specified with -u
                         * option on command line
                         */
    int     statistics;
    float32 err_rel; /*
                      * relative diff for the comparison
                      */

    int err_stat;
    /* an error occurred (1, error, 0, no error) */

} diff_opt_t;

/*-------------------------------------------------------------------------
 * public functions
 *-------------------------------------------------------------------------
 */

#ifdef __cplusplus
extern "C" {
#endif

uint32 hdiff(const char *fname1, const char *fname2, diff_opt_t *opt);
void   make_vars(char *optarg, diff_opt_t *opt, int option);

#ifdef __cplusplus
}
#endif

/*-------------------------------------------------------------------------
 * private functions
 *-------------------------------------------------------------------------
 */

uint32 gattr_diff(int32 sdid1, int32 sdid2, diff_opt_t *opt);
void   pr_att_vals(nc_type type, int len, void *vals);

uint32 array_diff(void *buf1, void *buf2, uint32 tot_cnt, const char *name1, const char *name2, int rank,
                  int32 *dims, int32 type, float32 err_limit, float32 err_rel, uint32 max_err_cnt,
                  int32 statistics, void *fill1, void *fill2);

uint32 match(uint32 nobjects1, dtable_t *list1, uint32 nobjects2, dtable_t *list2, int32 sd1_id, int32 gr1_id,
             int32 file1_id, int32 sd2_id, int32 gr2_id, int32 file2_id, diff_opt_t *opt);

uint32 diff(int32 file1_id, int32 file2_id, int32 sd1_id, int32 sd2_id, int32 gr1_id, int32 gr2_id,
            char *obj1_name, char *obj2_name, int32 tag1, int32 ref1, int32 tag2, int32 ref2,
            diff_opt_t *opt);

void print_dims(int r, int32 *d);

uint32 diff_vs(int32 file1_id, int32 file2_id, int32 ref1, int32 ref2, diff_opt_t *opt);

uint32 diff_gr(int32 gr1_id, int32 gr2_id, int32 ref1, int32 ref2, diff_opt_t *opt);

uint32 diff_sds(int32 sd1_id, int32 sd2_id, int32 ref1, int32 ref2, diff_opt_t *opt);

#endif /* HDIFF_H */
