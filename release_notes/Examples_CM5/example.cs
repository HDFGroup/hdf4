#include <hdf.h>
#include <netcdf.h>
#include <mfhdf.h>

#define xsize 256
#define ysize 256
#define zsize 256

int32	dims[3] = {xsize, ysize, zsize};
int32	begin[3] = {0, 0, 0};
int32	edges[3] = {xsize, ysize, zsize};
int32	cosmo_cube[xsize][ysize][zsize];
int32	cosmo_cube2[xsize][ysize][zsize];
int32	rank, nt, nattr;

/* 
* ==============================================================
*  1. Declare the shape and the parallel variables.
*/

shape [xsize][ysize][zsize]Datashape;
int:Datashape para_cosmo_cube;
int:Datashape para_cosmo_cube2;

/* default hdf and data filename */
/* To test this, set up a symbolic link, sda, to point at your directory */
/* in either /sda1 or /sda2. */

char	*filename = "Cosmo.hdf";
char	*datasetname = "Cosmo.Quarant001";
char	*datafile = "sda/Cosmo.Quarant001.data";
char    sdname[MAX_NC_NAME];     

/* function declarations */

main(ac, av)
int ac;
char **av;
{
	int	fid, sdsid, dimid, offset;
	int	index;
	char	sdname[MAX_NC_NAME];

	/* process any command option */
	if (ac == 3){
		filename = *++av;
		datafile = *++av;
	}

	printf("Using these names:\nHDF file:%s\nDataset: %s\nDatafile: %s\n",
		filename, datasetname, datafile);
	unlink(datafile);


	/* creating a new CM SDS */
	/* Define a new data set */
	fid = SDstart(filename, DFACC_CREATE);
	sdsid = SDcreate(fid, datasetname, DFNT_INT32, 3, (int32 *) dims);
	offset = 0;
	if (SDsetexternalfile(sdsid, datafile, offset) == FAIL){
	    printf("SDsetexternalfile failed\n");
	    errorhandle();
	}
	if (SDsetaccesstype(sdsid, DFACC_PARALLEL) == FAIL){
	    printf("SDsetaccesstype failed\n");
	    errorhandle();
	}

	/* calculate the cosmo cube */
	with (Datashape){
		para_cosmo_cube = (pcoord(2)<<16) | (pcoord(1)<<8) | pcoord(0);
	}

	/* Store the cube */
	/* First the attributes */
	dimid = SDgetdimid(sdsid, 0);
	SDsetdimname(dimid, "X-axis");
	SDsetdimstrs(dimid, "DimLabel", NULL, "TheFormat");
	/* ...				*/

	/* Store the data */
	SDwritedata(sdsid, begin, NULL, edges, &para_cosmo_cube);

	/* All done.  Close the HDF file. */
	SDendaccess(sdsid);
	SDend(fid);


	/* Read in a CM SDS */
	fid = SDstart(filename, DFACC_RDWR);
	index = SDnametoindex(fid, datasetname);
	sdsid = SDselect(fid, index);
	if (SDsetaccesstype(sdsid, DFACC_PARALLEL) == FAIL){
	    printf("can't use parallel access\n");
	    errorhandle();
	}

	/* Read in its attributes */
	SDgetinfo(sdsid, sdname, &rank, dims, &nt, &nattr);
	/* ...				*/


	/* Read in the data */
	SDreaddata(sdsid, begin, NULL, edges, &para_cosmo_cube2);

	/* update the cube */
	with (Datashape)
	    para_cosmo_cube2 *= 2;

	/* Replace the old data with new */
	SDwritedata(sdsid, begin, NULL, edges, &para_cosmo_cube2);

	/* All done.  Close the HDF file. */
	SDendaccess(sdsid);
	SDend(fid);
}

/* Encounter some error.  Ask user if to continue? */
errorhandle()
{
    char ans[BUFSIZ];
    printf("Go on?(y/n)[y]");
    gets(ans);
    if (*ans == 'y' || *ans == '\0')
	return;
    printf("Quit\n");
    exit(1);
}
