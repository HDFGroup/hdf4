#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#include "hdf.h"
#include "mfhdf.h"

#include "hdiff.h"
#include "dumplib.h"

int hdiff(char *fname1, char *fname2, struct fspec fspec)
{
 intn  status_n;            
 int32 sdid1, sdid2;
	int32 grid1, grid2;
 int32 fid1, fid2;
 int   ret=0;
 
 sdid1 = SDstart(fname1, DFACC_RDONLY); 
 if (sdid1 == -1)
 {
  error("SDstart failed on %s", fname1);
  exit(1);
 }
 
 sdid2 = SDstart(fname2, DFACC_RDONLY);
 if (sdid2 == -1)
 {
  SDend(sdid1);
  error("SDstart failed on %s", fname2);
  exit(1);
 }

 
 if (fspec.ga == 1) 
  ret=gattr_diff(sdid1, sdid2, fspec);
 
 if (fspec.sa == 1)
  ret=sdattr_diff(sdid1, sdid2, fspec);
 
 if (fspec.sd == 1)
  ret=sddata_diff(sdid1, sdid2, fspec);
 
 SDend(sdid1);
 SDend(sdid2);
 
 
 fid1 = Hopen(fname1, DFACC_READ, 0); 
 if (fid1 == -1)
 {
  error("Hopen failed on %s", fname1);
  exit(1);
 }
 
 fid2 = Hopen(fname2, DFACC_READ, 0); 
 if (fid2 == -1)
 {
  error("Hopen failed on %s", fname2);
  exit(1);
 }
 
 if (fspec.vd == 1)
 {
  ret=vdata_diff(fid2, fid1, fspec, 0, 1);
  ret=vdata_diff(fid1, fid2, fspec, 1, 0);
 }

	
	grid1 = GRstart(fid1); 
 if (grid1 == -1)
 {
  error("GRstart failed on %s", fname1);
  exit(1);
 }
 
 grid2 = GRstart(fid2);
 if (grid2 == -1)
 {
  GRend(grid1);
  error("GRstart failed on %s", fname2);
  exit(1);
 }

	if (fspec.gr == 1)
  ret=grdata_diff(grid1, grid2, fspec);


	GRend(grid1);
	GRend(grid2);
 
 status_n=Hclose(fid1);
 status_n=Hclose(fid2);
 
 return ret;
}
