/*********************************************************************
 *   Copyright 1993, UCAR/Unidata
 *   See netcdf/COPYRIGHT file for copying and redistribution conditions.
 *   /hdf/src/master/mfhdf/ncdump/ncdump.h,v 1.1 1993/04/21 21:51:19 chouck Exp
 *********************************************************************/

#define  Printf  (void) printf



struct ncdim {			/* dimension */
    char name[MAX_NC_NAME];
    long size;
};

struct ncvar {			/* variable */
    char name[MAX_NC_NAME];
    int32 type;
    int32 ndims;
    int32 dims[MAX_VAR_DIMS];
    int32 natts;
};

struct ncatt {			/* attribute */
    int32 var;
    char name[MAX_NC_NAME];
    int32 type;
    int32 len;
    void *val;
};

typedef
enum {LANG_NONE, LANG_C, LANG_F} Nclang; 

struct fspec {			/* selection for comparison  */
    int verbose;			/*
				 * if true, print cuurent interface comparison
				 */
				int ga;			/*
				 * if true, compare global attributes only 
				 */
    int sa;			/*
				 * if true, compare SD local attributes only 
				 */
    int sd;			/*
				 * if true, compare SD data only 
				 */
    int vd;			/*
				 * if true, compare Vdata only 
				 */
    int max_err_cnt;            /*
                                 * max. no of difference to be printed
                                 */
    float err_limit;		/*
				 * limit of difference for the comparison
				 */
    int nlvars;			/*
				 * Number of variables specified with -v option
				 * on command line
				 */
    char** lvars;		/*
				 * list of variable names specified with -v
				 * option on command line
				 */
    int nuvars;			/*
				 * Number of variables specified with -u option
				 * on command line
				 */
    char** uvars;		/*
				 * list of variable names specified with -u
				 * option on command line
				 */
				int statistics;

};




#ifdef __cplusplus
extern "C" {
#endif

int  hdiff(char *fname1, char *fname2, struct fspec fspec);
int  gattr_diff(int sdid1, int sdid2, struct fspec specp);
int  sdattr_diff(int sdid1, int sdid2, struct fspec specp);
int  sddata_diff(int sdid1, int sdid2, struct fspec specp);
int  vdata_diff(int32 fid1, int32 fid2, struct fspec specp, int32 cmp_flag, int32 f_flag);
void pr_att_vals(nc_type type, int len, void *vals);
int  vdata_cmp(int32 vs1, int32 vs2, char *gname, char*cname, int32 max_err_cnt);
void fmt_print(uint8 *x, int32 type);
void make_vars(char *optarg, struct fspec* fspecp, int option);
int  is_readable( char *vgclass );


#ifdef __cplusplus
}
#endif


