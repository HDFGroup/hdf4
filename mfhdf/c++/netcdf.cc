/*********************************************************************
 *   Copyright 1992, University Corporation for Atmospheric Research
 *   See netcdf/README file for copying and redistribution conditions.
 *
 *   Purpose:	Implements class interface for netCDF over C interface
 *
 *   $Header$
 *********************************************************************/

#include <string.h>
#include <malloc.h>
#include "netcdf.hh"

#ifndef TRUE
#define TRUE 1
#define FALSE 0
#endif

static const int ncGlobal = NC_GLOBAL; // psuedo-variable for global attributes

static const int ncBad = -1;	// failure return for netCDF C interface 

NcDim::NcDim(const NcFile* nc, NcToken name)
	: the_file(nc), the_id(ncdimid(nc->id(), name))
{}

NcDim::NcDim(const NcFile* nc, int id)
	: the_file(nc), the_id(id)
{}

NcDim::NcDim(NcFile* nc, NcToken name, long sz)
	: the_file(nc), the_id(nc
			       ? ncdimdef(nc->id(), name, sz)
			       : ncBad)
{}

NcDim::~NcDim( void )
{
}

int NcDim::id( void ) const
{
    return the_id;
}

NcToken NcDim::name( void ) const	// caller should free
{
    char nam[MAX_NC_NAME];
    if (the_file)
      ncdiminq(the_file->id(), the_id, nam, 0);
    else
      return 0;
    return strdup(nam);
}

long NcDim::size( void ) const
{
    long sz = 0;
    if (the_file)
      ncdiminq(the_file->id(), the_id, (char *)0, &sz);
    return sz;
}

bool NcDim::is_valid( void ) const
{
    return the_file->is_valid() && the_id != ncBad;
}

bool NcDim::is_unlimited( void ) const
{
    if (!the_file)
      return FALSE;
    int recdim;
    ncinquire(the_file->id(), 0, 0, 0, &recdim);
    return the_id == recdim;
}

bool NcDim::rename(NcToken newname)
{
    return ncdimrename(the_file->id(), the_id, newname) != ncBad;
}

NcFile::NcFile( void )
{
    the_id = ncBad;		// until the file is actually open
    dimensions = new NcDim*[MAX_NC_DIMS];
    variables = new NcVar*[MAX_NC_VARS];
}

NcOldFile::NcOldFile( const char * path, NcFile::OpenMode mode)
{
    NcError err(NcError::silent_nonfatal); // because constructor must not fail
    the_id = ncopen(path, mode);
    in_define_mode = 0;
    for (int i = 0; i < num_dims(); i++)
      dimensions[i] = new NcDim(this, i);
    for (i = 0; i < num_vars(); i++)
      variables[i] = new NcVar(this, i);
    globalv = new NcVar(this, ncGlobal);
}

NcNewFile::NcNewFile( const char * path, CreateMode mode)
{
    NcError err(NcError::silent_nonfatal); // because constructor must not fail
    the_id = nccreate(path, mode);
    in_define_mode = 1;
    globalv = new NcVar(this, ncGlobal);
}

NcFile::~NcFile( void )
{
    (void) close();
}

bool NcFile::is_valid( void ) const
{
    return the_id != ncBad;
}

int NcFile::close( void )
{
    for (int i = 0; i < num_dims(); i++)
      delete dimensions[i];
    for (i = 0; i < num_vars(); i++)
      delete variables[i];
    delete [] dimensions;
    delete [] variables;
    delete globalv;
    int old_id = the_id;
    the_id = ncBad;
    return ncclose(old_id) != ncBad;
}

bool NcFile::define_mode( void )
{
    if (! is_valid())
      return FALSE;
    if (in_define_mode)
      return TRUE;
    if (ncredef(the_id) == ncBad)
      return FALSE;
    in_define_mode = 1;
    return TRUE;
}

bool NcFile::data_mode( void )
{
    if (! is_valid())
      return FALSE;
    if (! in_define_mode)
      return TRUE;
    if (ncendef(the_id) == ncBad)
      return FALSE;
    in_define_mode = 0;
    return TRUE;
}

int NcFile::id( void ) const
{
    return the_id;
}

int NcFile::num_dims( void ) const
{
    int num;
    ncinquire(the_id, &num, 0, 0, 0);
    return num;
}

int NcFile::num_vars( void ) const
{
    int num;
    ncinquire(the_id, 0, &num, 0, 0);
    return num;
}

int NcFile::num_atts( void ) const
{
    int num = 0;
    if (is_valid())
      ncinquire(the_id, 0, 0, &num, 0);
    return num;
}

NcDim* NcFile::add_dim(NcToken name)
{
    return add_dim(name, NC_UNLIMITED);
}

NcDim* NcFile::add_dim(NcToken name, long size)
{
    if (!define_mode())
      return 0;
    int n = num_dims();
    NcDim* dimp = new NcDim(this, name, size);
    dimensions[n] = dimp;	// for garbage collection on close()
    return dimp;
}

NcDim* NcFile::get_dim( int i ) const
{
    if (i < 0 || i >= num_dims())
      return 0;
    return dimensions[i];
}

NcDim* NcFile::get_dim( NcToken name ) const
{
    int dimid = ncdimid(the_id, name);
    if (dimid == ncBad)
      return 0;
    return dimensions[dimid];
}

NcVar * NcFile::get_var( int i ) const
{
    if (i < 0 || i >= num_vars())
      return 0;
    return variables[i];
}

NcVar * NcFile::get_var( NcToken name ) const
{
    int varid = ncvarid(the_id, name);
    if (varid == ncBad)
      return 0;
    return variables[varid];
}

// To create scalar, 1-dimensional, ..., 5-dimensional variables, just supply
// as many dimension arguments as necessary

NcVar* NcFile::add_var(NcToken name, NcType type, // scalar to 5D var
			    const NcDim* dim0,
			    const NcDim* dim1,
			    const NcDim* dim2,
			    const NcDim* dim3,
			    const NcDim* dim4)
{
    if (! define_mode())
      return 0;
    int dims[5];
    int ndims = 0;
    if (dim0) {
	ndims++;
	dims[0] = dim0->id();
	if (dim1) {
	    ndims++;
	    dims[1] = dim1->id();
	    if (dim2) {
		ndims++;
		dims[2] = dim2->id();
		if (dim3) {
		    ndims++;
		    dims[3] = dim3->id();
		    if (dim4) {
			ndims++;
			dims[4] = dim4->id();
		    }
		}
	    }
	}
    }
    int n = num_vars();
    NcVar* varp =
      new NcVar(this,
		     ncvardef(the_id, name, (nc_type) type, ndims, dims));
    variables[n] = varp;
    return varp;
}

// For variables with more than 5 dimensions, use n-dimensional interface
// with vector of dimensions.

NcVar* NcFile::add_var(NcToken name, NcType type, int ndims,
			    const NcDim* dims)
{
    if (! define_mode())
      return 0;
    int *dimids = new int[ndims];
    for (int i=0; i < ndims; i++)
      dimids[i] = dims[i].id();
    int n = num_vars();
    NcVar* varp =
      new NcVar(this,
		     ncvardef(the_id, name, (nc_type) type, ndims, dimids));
    variables[n] = varp;
    return varp;
}

NcToken NcFile::attname(int attnum) const
{
    if (attnum < 0 || attnum >= num_atts())
      return 0;
    char aname[MAX_NC_NAME];
    if (ncattname(the_id, ncGlobal, attnum, aname) == ncBad)
      return 0;
    return strdup(aname);
}

NcAtt::NcAtt(NcFile* nc, const NcVar* var, NcToken name)
   : NcTypedComponent(nc), the_variable(var), the_name(strdup(name))
{}

NcAtt::NcAtt(NcFile* nc, NcToken name)
   : NcTypedComponent(nc), the_variable(NULL), the_name(strdup(name))
{
	int i=0;
}

NcAtt::~NcAtt( void )
{
    free((char *) the_name);
}

NcToken NcAtt::name( void ) const
{
    return strdup(the_name);
}

NcType NcAtt::type( void ) const
{
    nc_type typ;
    ncattinq(the_file->id(), the_variable->id(), the_name, &typ, 0);
    return (NcType) typ;
}

long NcAtt::num_vals( void ) const
{
    int len;
    ncattinq(the_file->id(), the_variable->id(), the_name, 0, &len);
    return len;
}

bool NcAtt::is_valid( void ) const
{
    return the_file->is_valid() &&
      the_variable->is_valid() &&
	ncattinq(the_file->id(), the_variable->id(), the_name, 0, 0) != ncBad;
}

NcValues* NcAtt::values( void ) const
{
    NcValues* valp = get_space();
    if (ncattget(the_file->id(),
		 the_variable->id(),
		 the_name,
		 valp->base()) == ncBad) {
	delete valp;
	return 0;
    }
	
    return valp;
}

bool NcAtt::rename(NcToken newname)
{
    return ncattrename(the_file->id(), the_variable->id(),
		       the_name, newname) != ncBad;
}

bool NcAtt::remove( void )
{
    return ncattdel(the_file->id(), the_variable->id(), the_name) != ncBad;
}

void NcVar::init_cur( void )
{
    the_cur = new long[MAX_NC_DIMS]; // *** don't know num_dims() yet?
    for(int i = 0; i < MAX_NC_DIMS; i++)
      the_cur[i] = 0;
}

// If no args, set cursor to all zeros.	 Else set initial elements of cursor
// to args provided, rest to zeros.
bool NcVar::set_cur(long c0, long c1, long c2, long c3, long c4)
{
    long t[5];
    t[0] = c0;
    t[1] = c1;
    t[2] = c2;
    t[3] = c3;
    t[4] = c4;
    for(int j = 0; j < 5; j++) {
	if (t[j] == -1) {
	    if (num_dims() < j)
	      return FALSE;	// too many for variable's dimensionality
	    for (int i = 0; i < j; i++) {
		if (t[i] >= get_dim(i)->size())
		  return FALSE;	// too big for dimension
		the_cur[i] = t[i];
	    }
	    for(i = j; i < num_dims(); i++)
	      the_cur[i] = 0;
	    return TRUE;
	}
    }
}

bool NcVar::set_cur(long *cur)
{
    for(int i = 0; i < num_dims(); i++) {
	if (cur[i] >= get_dim(i)->size())
	  return FALSE;
	the_cur[i] = cur[i];
    }
    return TRUE;
}

NcVar::NcVar(NcFile* nc, int id)
   : NcTypedComponent(nc), the_id(id)
{
    init_cur();
}

NcVar::NcVar( NcFile* nc, NcToken name )
	: NcTypedComponent(nc), the_id(ncvarid(nc->id(), name))
{
    init_cur();
}

NcVar::~NcVar( void )
{
    delete[] the_cur;
}

NcToken NcVar::name( void ) const	// caller should free
{
    char nam[MAX_NC_NAME];
    ncvarinq(the_file->id(), the_id, nam, 0, 0, 0, 0);
    return strdup(nam);
}

NcType NcVar::type( void ) const
{
    nc_type typ;
    ncvarinq(the_file->id(), the_id, 0, &typ, 0, 0, 0);
    return (NcType) typ;
}

int NcVar::num_dims( void ) const
{
    int ndim;
    ncvarinq(the_file->id(), the_id, 0, 0, &ndim, 0, 0);
    return ndim;
}

int NcVar::num_atts( void ) const // handles variable and global atts
{
    int natt = 0;
    if (the_file->is_valid())
      if (the_id == ncGlobal)
	natt = the_file->num_atts();
      else
	ncvarinq(the_file->id(), the_id, 0, 0, 0, 0, &natt);
    return natt;
}

// The i-th dimension for this variable
NcDim* NcVar::get_dim( int i ) const
{
    int ndim;
    int dims[MAX_NC_DIMS];
    if(ncvarinq(the_file->id(), the_id, 0, 0, &ndim, dims, 0) == ncBad ||
       i < 0 || i >= ndim)
      return 0;
    return the_file->get_dim(dims[i]);
}

int NcVar::attnum( NcToken attrname ) const
{
    for(int num=0; num < num_atts(); num++) {
	char aname[MAX_NC_NAME];
	ncattname(the_file->id(), the_id, num, aname);
	if (strcmp(aname, attrname) == 0)
	  break;
    }
    return num;			// num_atts() if no such attribute
}

long* NcVar::edges( void ) const	// edge lengths (dimension sizes)
{
    long* evec = new long[num_dims()];
    for(int i=0; i < num_dims(); i++)
      evec[i] = get_dim(i)->size();
    return evec;
}

NcToken NcVar::attname( int attnum ) const
{
    if (attnum < 0 || attnum >= num_atts())
      return 0;
    char aname[MAX_NC_NAME];
    if (ncattname(the_file->id(), the_id, attnum, aname) == ncBad)
      return 0;
    return strdup(aname);
}

NcAtt * NcVar::get_att( NcToken aname ) const
{
    NcAtt* att = new NcAtt(the_file, this, aname );
    return att;
}

NcAtt * NcVar::get_att( int n ) const
{
    if (n < 0 || n >= num_atts())
      return 0;
    return get_att(attname(n));
}

NcAtt * NcFile::get_att( NcToken aname ) const
{
    return globalv->get_att(aname);
}

NcAtt * NcFile::get_att( int n ) const
{
    return globalv->get_att(n);
}

NcDim* NcFile::rec_dim( ) const
{
    int recdim;
    ncinquire(the_id, 0, 0, 0, &recdim);
    if (recdim == -1)
      return 0;
    return get_dim(recdim);
}

#define NcFile_add_scalar_att(TYPE)					      \
bool NcFile::add_att(NcToken aname, TYPE val)				      \
{									      \
    return globalv->add_att(aname, val);				      \
}
NcFile_add_scalar_att(char)
NcFile_add_scalar_att(short)
NcFile_add_scalar_att(int)
NcFile_add_scalar_att(long)
NcFile_add_scalar_att(float)
NcFile_add_scalar_att(double)

bool NcFile::add_att(NcToken aname, const char* val)
{
    return globalv->add_att(aname, val);
}

#define NcFile_add_vector_att(TYPE)					      \
bool NcFile::add_att(NcToken aname, int n, const TYPE* val)		      \
{									      \
    return globalv->add_att(aname, n, val);				      \
}
NcFile_add_vector_att(char)
NcFile_add_vector_att(short)
NcFile_add_vector_att(int)
NcFile_add_vector_att(long)
NcFile_add_vector_att(float)
NcFile_add_vector_att(double)

bool NcFile::sync( void )
{
    return ncsync(the_id) != ncBad;
}

bool NcFile::abort( void )
{
    return ncabort(the_id) != ncBad;
}

bool NcFile::set_fill( FillMode a_mode )
{
    return ncsetfill(the_id, a_mode) != ncBad;
}

enum NcFile::FillMode NcFile::get_fill( void )
{
    int mode = ncsetfill(the_id, Fill);
    if (mode == NC_FILL)
      return NcFile::Fill;
    if (mode == NC_NOFILL) {
	ncsetfill(the_id, NoFill); // reset it
	return NcFile::NoFill;
    }
    return NcFile::Bad;
}

bool NcVar::is_valid( void ) const
{
    return the_file->is_valid() && the_id != ncBad;
}

long NcVar::num_vals( void ) const
{
    long prod = 1;
    NcDim* dim;
    for (int d = 0; dim = get_dim(d); d++)
	prod *= dim->size();
    return  prod;
}

NcTypedComponent::NcTypedComponent ( NcFile* nc )
	: the_file(nc)
{}

NcValues* NcTypedComponent::get_space( void ) const
{
    NcValues* valp;
    switch (type()) {
      case ncFloat:
	valp = new NcValues_float(num_vals());
	break;
      case ncDouble:
	valp = new NcValues_double(num_vals());
	break;
      case ncLong:
	valp = new NcValues_long(num_vals());
	break;
      case ncInt:
	valp = new NcValues_int(num_vals());
	break;
      case ncShort:
	valp = new NcValues_short(num_vals());
	break;
      case ncByte:
      case ncChar:
	valp = new NcValues_char(num_vals());
	break;
      case ncNoType:
      default:
	valp = 0;
    }
    return valp;
}

NcValues* NcVar::values( void ) const
{
    int ndims = num_dims();
    long crnr[MAX_NC_DIMS];
    long edgs[MAX_NC_DIMS];
    for (int i = 0; i < ndims; i++) {
	crnr[i] = 0;
	edgs[i] = get_dim(i)->size();
    }
    NcValues* valp = get_space();
    if (ncvarget(the_file->id(), the_id, crnr, edgs, valp->base()) == ncBad)
	return 0;
    return valp;
}

#define NcVar_add_scalar_att(TYPE)					      \
bool NcVar::add_att(NcToken aname, TYPE val)			      \
{									      \
    if (! the_file->define_mode())					      \
      return FALSE;							      \
    if (ncattput(the_file->id(), the_id, aname, (nc_type) NcTypeEnum(TYPE),   \
		 1, &val) == ncBad)					      \
      return FALSE;							      \
    return TRUE;							      \
}									      \

NcVar_add_scalar_att(char)
NcVar_add_scalar_att(short)
NcVar_add_scalar_att(int)
NcVar_add_scalar_att(long)
NcVar_add_scalar_att(double)

bool NcVar::add_att(NcToken aname, float val)
{
    if (! the_file->define_mode())
      return FALSE;
    float fval = (float) val;	// workaround for bug, val passed as double??
    if (ncattput(the_file->id(), the_id, aname, (nc_type) ncFloat,
		 1, &fval) == ncBad)
      return FALSE;
    return TRUE;
}

bool NcVar::add_att(NcToken aname, const char* val)
{
    if (! the_file->define_mode())
      return FALSE;
    if (ncattput(the_file->id(), the_id, aname, (nc_type) ncChar,
		 strlen(val) + 1, val) == ncBad)
      return FALSE;
    return TRUE;
}

#define NcVar_add_vector_att(TYPE)					      \
bool NcVar::add_att(NcToken aname, int len, const TYPE* vals)		      \
{									      \
    if (! the_file->define_mode())					      \
      return FALSE;							      \
    if (ncattput(the_file->id(), the_id, aname, (nc_type) NcTypeEnum(TYPE),   \
		 len, vals) == ncBad)					      \
      return FALSE;							      \
    return TRUE;							      \
}
NcVar_add_vector_att(char)
NcVar_add_vector_att(short)
NcVar_add_vector_att(long)
NcVar_add_vector_att(int)
NcVar_add_vector_att(float)
NcVar_add_vector_att(double)

#define NcVar_put_array(TYPE)						      \
bool NcVar::put( const TYPE* vals,					      \
		     long edge0,					      \
		     long edge1,					      \
		     long edge2,					      \
		     long edge3,					      \
		     long edge4)					      \
{									      \
    if (type() != NcTypeEnum(TYPE))					      \
      return FALSE;							      \
    if (! the_file->data_mode())					      \
      return FALSE;							      \
    long count[5];							      \
    count[0] = edge0;							      \
    count[1] = edge1;							      \
    count[2] = edge2;							      \
    count[3] = edge3;							      \
    count[4] = edge4;							      \
    for (int i = 0; i < 5; i++) {					      \
	if (count[i]) {							      \
	    if (num_dims() < i)						      \
	      return FALSE;						      \
	} else								      \
	  break;							      \
    }									      \
    static long start[5] = {0, 0, 0, 0, 0};				      \
    for (int j = 0; j < 5; j++) {					      \
     start[j] = the_cur[j];						      \
    }									      \
    return ncvarput(the_file->id(), the_id, start, count, vals) != ncBad;     \
}

NcVar_put_array(char)
NcVar_put_array(short)
NcVar_put_array(long)
NcVar_put_array(int)
NcVar_put_array(float)
NcVar_put_array(double)

#define NcVar_put_nd_array(TYPE)					      \
bool NcVar::put( const TYPE* vals, const long* count )			      \
{									      \
    if (type() != NcTypeEnum(TYPE))					      \
      return FALSE;							      \
    if (! the_file->data_mode())					      \
      return FALSE;							      \
    long start[MAX_NC_DIMS];						      \
    for (int i = 0; i < num_dims(); i++)				      \
      start[i] = the_cur[i];						      \
    return ncvarput(the_file->id(), the_id, start, count, vals) != ncBad;     \
}

NcVar_put_nd_array(char)
NcVar_put_nd_array(short)
NcVar_put_nd_array(long)
NcVar_put_nd_array(int)
NcVar_put_nd_array(float)
NcVar_put_nd_array(double)

#define NcVar_get_array(TYPE)						      \
bool NcVar::get( TYPE* vals,						      \
		     long edge0,					      \
		     long edge1,					      \
		     long edge2,					      \
		     long edge3,					      \
		     long edge4) const					      \
{									      \
    if (type() != NcTypeEnum(TYPE))					      \
      return FALSE;							      \
    if (! the_file->data_mode())					      \
      return FALSE;							      \
    long count[5];							      \
    count[0] = edge0;							      \
    count[1] = edge1;							      \
    count[2] = edge2;							      \
    count[3] = edge3;							      \
    count[4] = edge4;							      \
    for (int i = 0; i < 5; i++) {					      \
	if (count[i]) {							      \
	    if (num_dims() < i)						      \
	      return FALSE;						      \
	} else								      \
	  break;							      \
    }									      \
    static long start[5] = {0, 0, 0, 0, 0};				      \
    for (int j = 0; j < 5; j++) {					      \
     start[j] = the_cur[j];						      \
    }									      \
    return ncvarget(the_file->id(), the_id, start, count, vals) != ncBad;     \
}

NcVar_get_array(char)
NcVar_get_array(short)
NcVar_get_array(long)
NcVar_get_array(int)
NcVar_get_array(float)
NcVar_get_array(double)

#define NcVar_get_nd_array(TYPE)					      \
bool NcVar::get( TYPE* vals, const long* count ) const		      	      \
{									      \
    if (type() != NcTypeEnum(TYPE))					      \
      return FALSE;							      \
    if (! the_file->data_mode())					      \
      return FALSE;							      \
    long start[MAX_NC_DIMS];						      \
    for (int i = 0; i < num_dims(); i++)				      \
	start[i] = the_cur[i];						      \
    return ncvarget(the_file->id(), the_id, start, count, vals) != ncBad;     \
}

NcVar_get_nd_array(char)
NcVar_get_nd_array(short)
NcVar_get_nd_array(long)
NcVar_get_nd_array(int)
NcVar_get_nd_array(float)
NcVar_get_nd_array(double)

bool NcVar::rename(NcToken newname)
{
    return ncvarrename(the_file->id(), the_id, newname) != ncBad;
}

int NcVar::id( void ) const
{
    return the_id;
}

#define Nc_as(TYPE) name2(as_,TYPE)
#define NcTypedComponent_as(TYPE)					      \
TYPE NcTypedComponent::Nc_as(TYPE)( int n ) const			      \
{									      \
    return values()->Nc_as(TYPE)(n);					      \
}
NcTypedComponent_as(byte)
NcTypedComponent_as(char)
NcTypedComponent_as(short)
NcTypedComponent_as(long)
NcTypedComponent_as(float)
NcTypedComponent_as(double)

char* NcTypedComponent::as_string( int n ) const
{
    return values()->as_string(n);
}

NcError::NcError( Behavior b )
{
    the_old_state = ncopts;	// global variable in C interface
    the_old_err = ncerr;	// global variable in C interface
    ncopts = (int) b;
}

NcError::~NcError( void )
{
    ncopts = the_old_state;
    ncerr = the_old_err;
}

int NcError::get_err( void )	// returns most recent error
{
    return ncerr;
}
