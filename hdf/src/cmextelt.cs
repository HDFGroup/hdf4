#include "hdf.h"

/* CM5 stuff */
#include <cm/cmfs.h>
#include <cm/cm_file.h>
#define parallel_write	CMFS_write_file_always
#define parallel_read	CMFS_read_file_always

int CM_DEBUG = 0;
int32 CM_HDFtype;

PRIVATE int32
CMgetboolsize()
{

  switch(CM_HDFtype) {
    case DFNT_CHAR8:
    case DFNT_UCHAR8:
    case DFNT_INT8:    return boolsizeof(int8);

    case DFNT_INT16:   
    case DFNT_UINT16:  return boolsizeof(int16);

    case DFNT_INT32:   
    case DFNT_UINT32:  return boolsizeof(int32);

    case DFNT_FLOAT32: return boolsizeof(float32);

    case DFNT_FLOAT64: return boolsizeof(float64);

    default:           return FAIL;
  }
}

#ifdef NOT
int32 CMSDcreate(fid, datasetname, datafilename, nt, rank, dimsizes)
int32 fid;
char  *datasetname, *datafilename;
int32 nt, rank, *dimsizes;
{
    int32 sdsid;

    sdsid = SDcreate(fid, datasetname, nt, rank, dimsizes);
    if (sdsid == FAIL)
	return(FAIL);
    if (SDsetexternalfile(sdsid, datafilename, 0) == FAIL)
	return(FAIL);
    return(sdsid);
}
#endif

int32 CM_OPEN(datafname, openmode)
char *datafname;
int	openmode;
{
    int32 fd;
    /* Parallel data file process */
    fd = CMFS_open (datafname, CMFS_O_RDWR);
    if ( fd < 0 ) {
	    perror(datafname);
	    return (FAIL);
    }
    return(fd);
}

int32 CM_CREATE(datafname)
char *datafname;
{
    int32 fd;
    /* Parallel data file process */
    fd = CMFS_open (datafname, CMFS_O_RDWR | CMFS_O_CREAT | CMFS_O_TRUNC,
	    0644);
    if ( fd < 0 ) {
	    perror(datafname);
	    return (FAIL);
    }
    return(fd);
}

intn CM_WRITE(fd, data, length)
int fd, length;
void:void *data;
{
    int retval;
    int32 Boolsize;

    Boolsize = CMgetboolsize();
    if (Boolsize != FAIL){
	retval = parallel_write(fd, data, Boolsize);
	if (retval == Boolsize)
	    return(SUCCEED);
    }
    CMFS_perror("write_file");
    return (FAIL);
}

intn CM_READ(fd, data, length)
int fd, length;
int:void *data;
{
    int retval;
    int32 Boolsize;

    Boolsize = CMgetboolsize();
    if (Boolsize != FAIL){
	retval = parallel_read(fd, data, Boolsize);
	if (retval == Boolsize)
	    return(SUCCEED);
    }
    CMFS_perror("read_file");
    return (FAIL);

}

intn CM_SEEK(fd, offset)
int fd, offset;
{
    return (CMFS_serial_lseek(fd, offset, CMFS_L_SET) != -1 ? SUCCEED : FAIL);
}

#include <csfort.h>

/*-----------------------------------------------------------------------------
 * Name:    CMreaddata
 * Purpose: read a section of data
 * Inputs:  id: dataset id
 *          start: start location
 *          stride: stride along each dimension
 *          end: number of values along each dim to read
 *          values: data 
 * Returns: 0 on success, -1 on failure with error set
 *---------------------------------------------------------------------------*/

   FRETVAL(intf)
CMreaddata(int32 id, int32 *start, int32 *stride, int32 *end, VOIDP values)
{
    intf    ret;
    shape   S;
    
    S = CMC_allocate_shape_from_desc(values);
    Kopyptr(CMC_unwrap_pvar(values, S), &values);
    ret = (intf) SDreaddata(id, start, stride, end, values);
    deallocate_shape(&S);
    return(ret);
}

/*-----------------------------------------------------------------------------
 * Name:    CMwritedata
 * Purpose: write a section of data
 * Inputs:  id: dataset id
 *          start: start location
 *          stride: stride along each dimension
 *          end: number of values along each dim to write
 *          values: data 
 * Returns: 0 on success, -1 on failure with error set
 *---------------------------------------------------------------------------*/

   FRETVAL(intf)
CMwritedata(int32 id, int32 *start, int32 *stride, int32 *end, void * values)
{
    intf    ret;
    shape   S;
    
    S = CMC_allocate_shape_from_desc(values);
    Kopyptr(CMC_unwrap_pvar(values, S), &values);
    ret = (intf) SDwritedata(id, start, stride, end, values);
    deallocate_shape(&S);
    return(ret);
}

/* temporary hack to deceive C* to take the pointer type. */
Kopyptr(void * pt, void ** pt2)
{
    *pt2 = pt;
}

