/* sd_sh_dim.c 
 * Based on sd_dim.c.
 * All SDS's share the dimensions. The file size is
 * smaller and execution time is shorter. Performance
 * is improved. 
 */

#include "mfhdf.h"

#define HEIGHT 10
#define LAT 10
#define LON 10

void main(int argc,char *argv[])
{
    int32 f1, dimsize[3]; 
    int32 newsds, dimid0, dimid1, dimid2;
    intn status, i,j,k,l, city, dataset;
    intn city_num=20, dataset_num=25;
    int32   start[3], end[3];
    int32 data[1000];
    char sdsname[20], nm_suf[10];
    char *basenm="sds";

    /* Start SD */

    f1 = SDstart("sd_sh_dim.hdf", DFACC_CREATE);
    dimsize[0] = HEIGHT;
    dimsize[1] = LAT;
    dimsize[2] = LON;
    /* initialize data[], start[] and end[] */
    l = 0;
    for (i=0; i<HEIGHT; i++)
        for (j=0; j<LAT; j++)
            for (k=0; k<LON; k++) {
                data[l] = l;
                l++;
            }
    start[0] = start[1] = start[2] = 0;
    end[0]   = dimsize[0];
    end[1]   = dimsize[1];
    end[2]   = dimsize[2];
    for (city=0; city<city_num; city++)
        for (dataset = 0; dataset<dataset_num; dataset++) {
            /* create a name for each dataset */
            strcpy(sdsname, basenm);
            sprintf(nm_suf, "_%d_%d",city,dataset);
            strcat(sdsname, nm_suf);
            newsds = SDcreate(f1, sdsname, DFNT_INT32, 3, dimsize);
            dimid0 = SDgetdimid(newsds, 0);
            status = SDsetdimname(dimid0, "DIM0");
            dimid1 = SDgetdimid(newsds, 1);
            status = SDsetdimname(dimid1, "DIM1");
            dimid2 = SDgetdimid(newsds, 2);
            status = SDsetdimname(dimid2, "DIM2");
            status = SDwritedata(newsds,start,NULL,end,(VOIDP)data);
            status = SDendaccess(newsds);
    }
    status = SDend(f1);
}
