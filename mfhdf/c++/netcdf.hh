/*********************************************************************
 *   Copyright 1992, University Corporation for Atmospheric Research
 *   See netcdf/README file for copying and redistribution conditions.
 *
 *   Purpose:	C++ class interface for netCDF
 *
 *   $Header$
 *********************************************************************/

#ifndef Netcdf_def
#define Netcdf_def

#include "netcdf.h"		// the C interface
#include "ncvalues.hh"		// arrays that know the type of their elements

typedef const char* NcToken;	// names for netCDF components
typedef unsigned int bool;	// many members return 0 on failure

class NcFile;
class NcOldFile;
class NcDim;

class NcAtt;
class NcVar;

class NcFile			// Base class for NcOldFile and NcNewFile
{
  public:
    enum FillMode {
	Fill = NC_FILL,		// prefill with fill values (default)
	NoFill = NC_NOFILL,	// don't prefill, avoids some duplicate writes
	Bad			// invalid fill mode
      };
    virtual ~NcFile( void );
    int close( void );

    bool is_valid( void ) const;

    int num_dims( void ) const;		   // number of dimensions
    int num_vars( void ) const;		   // number of variables
    int num_atts( void ) const;		   // number of attributes

    NcDim* get_dim( NcToken ) const;	   // dimension by name
    NcVar* get_var( NcToken ) const;	   // variable by name
    NcAtt* get_att( NcToken ) const;	   // global attribute by name
    NcDim* get_dim( int ) const;	   // n-th dimension
    NcVar* get_var( int ) const;	   // n-th variable
    NcAtt* get_att( int ) const;	   // n-th global attribute
    NcDim* rec_dim( void ) const;	   // unlimited dimension, if any

    bool sync( void );			   // synchronize to disk
    bool abort( void );			   // back out of bad defines
    bool set_fill( FillMode = Fill );	   // set fill-mode
    FillMode get_fill( void );		   // get fill-mode
    
    // Add new dimensions, variables, global attributes.
    // These put the file in "define" mode, so could be expensive.
    NcDim* add_dim( NcToken dimname ); // unlimited
    NcDim* add_dim( NcToken dimname, long dimsize );
    NcVar* add_var( NcToken varname, NcType type,	// scalar
		    const NcDim* dim0=0,		// 1-dim
		    const NcDim* dim1=0,		// 2-dim
		    const NcDim* dim2=0,		// 3-dim
		    const NcDim* dim3=0,		// 4-dim
		    const NcDim* dim4=0 );		// 5-dim
    NcVar*	 add_var( NcToken varname, NcType type,	// n-dim
			  int ndims,
			  const NcDim* dims );

    bool add_att( NcToken, char );	   // add scalar attributes
    bool add_att( NcToken, short );	   // ... needs templates!
    bool add_att( NcToken, int );
    bool add_att( NcToken, long );
    bool add_att( NcToken, float );
    bool add_att( NcToken, double );

    bool add_att(NcToken, const char*);	   // add string attribute

    bool add_att( NcToken, int, const char* ); // add vector attributes
    bool add_att( NcToken, int, const short* );
    bool add_att( NcToken, int, const int* );
    bool add_att( NcToken, int, const long* );
    bool add_att( NcToken, int, const float* );
    bool add_att( NcToken, int, const double* );
    
    // These are needed by other Nc classes, but users shouldn't need them
    bool define_mode( void );	// leaves in define mode, if possible
    bool data_mode( void );	// leaves in data mode, if possible
    int id( void ) const;	// id used by C interface

    enum OpenMode {
	ReadOnly = NC_NOWRITE,	// open for read only (default for NcOldFile)
	Write = NC_WRITE	// open for write, must exist
      };

  protected:
    NcFile( void );
    int the_id;
    int in_define_mode;
    NcDim** dimensions;
    NcVar** variables;
    NcVar* globalv;	// global attributes attached to this
    NcToken attname(int num) const;
};

// For existing netCDF files.
class NcOldFile : public NcFile
{
  public:
    NcOldFile( const char * path, NcFile::OpenMode = ReadOnly );
};

// For new netCDF files.
class NcNewFile : public NcFile
{
  public:
    enum CreateMode {
	NoClobber = NC_NOCLOBBER, // create only if doesn't exist (default)
	Clobber = NC_CLOBBER	// create new file, even if exists
      };
    NcNewFile( const char * path, CreateMode = NoClobber );
};

class NcDim
{
  public:
    NcToken name( void ) const;
    long size( void ) const;
    bool is_valid( void ) const;
    bool is_unlimited( void ) const;
    bool rename( NcToken newname );
    int id( void ) const;

  protected:
    NcDim(const NcFile*, NcToken name);	// for existing dimensions
    NcDim(const NcFile*, int num);
    NcDim(NcFile*, NcToken name, long sz); // defines a new dim
    virtual ~NcDim( void );		// only NcFiles delete dimensions

  private:
    const NcFile *the_file;
    int the_id;
    
  friend NcOldFile::NcOldFile( const char *, NcFile::OpenMode );
  friend NcDim* NcFile::add_dim( NcToken, long );
  friend NcFile::close( void );
};

// Abstract base class for NcVar and NcAtt
class NcTypedComponent
{
  public:
    virtual NcToken name( void ) const = 0;
    virtual NcType type( void ) const = 0;
    virtual bool is_valid( void ) const = 0;
    virtual long num_vals( void ) const = 0; 
    virtual bool rename( NcToken newname ) = 0;
    virtual NcValues* values( void ) const = 0;	// block of all values

    // The following member functions provide conversions from the value
    // type to a desired basic type.  If the value is out of range,
    // the default "fill-value" for the appropriate type is returned.

    virtual byte as_byte( int n ) const;   	// nth value as an unsgnd char
    virtual char as_char( int n ) const;	// nth value as char
    virtual short as_short( int n ) const;	// nth value as short
    virtual long as_long( int n ) const;	// nth value as long
    virtual float as_float( int n ) const;	// nth value as floating-point
    virtual double as_double( int n ) const;	// nth value as double
    virtual char* as_string( int n ) const;	// nth value as string

  protected:
    NcFile *the_file;
    NcTypedComponent( NcFile* );
    virtual NcValues* get_space( void ) const; // empty, holds all values
};

class NcVar : public NcTypedComponent
{
  public:
    ~NcVar( void );
    NcToken name( void ) const;
    NcType type( void ) const;
    bool is_valid( void ) const;
    int num_dims( void ) const;		// dimensionality of variable
    NcDim* get_dim( int ) const;	// n-th dimension
    long* edges( void ) const;		// dimension sizes
    int num_atts( void ) const;		// number of attributes
    NcAtt* get_att( NcToken ) const; // attribute by name
    NcAtt* get_att( int ) const;	// n-th attribute
    long num_vals( void ) const;	// product of dimension sizes
    NcValues* values( void ) const; // all values

    // Put scalar or 1, ..., 5 dimensional arrays by providing enough
    // arguments.  The arguments are edge lengths, and their number must not
    // exceed variable's dimensionality.  The start corner is [0,0,..., 0] by
    // default, but may be reset using the set_cur() member.  FALSE is
    // returned if type of values does not match type for variable.
    bool put( const char* vals,
		      long c0=0, long c1=0, long c2=0, long c3=0, long c4=0 );
    bool put( const short* vals,
		      long c0=0, long c1=0, long c2=0, long c3=0, long c4=0 );
    bool put( const long* vals,
		      long c0=0, long c1=0, long c2=0, long c3=0, long c4=0 );
    bool put( const int* vals,
		      long c0=0, long c1=0, long c2=0, long c3=0, long c4=0 );
    bool put( const float* vals,
		      long c0=0, long c1=0, long c2=0, long c3=0, long c4=0 );
    bool put( const double* vals,
		      long c0=0, long c1=0, long c2=0, long c3=0, long c4=0 );

    // Put n-dimensional arrays, starting at [0, 0, ..., 0] by default,
    // may be reset with set_cur().
    bool put( const char* vals, const long* counts );
    bool put( const short* vals, const long* counts );
    bool put( const long* vals, const long* counts );
    bool put( const int* vals, const long* counts );
    bool put( const float* vals, const long* counts );
    bool put( const double* vals, const long* counts );

    // Get scalar or 1, ..., 5 dimensional arrays by providing enough
    // arguments.  The arguments are edge lengths, and their number must not
    // exceed variable's dimensionality.  The start corner is [0,0,..., 0] by
    // default, but may be reset using the set_cur() member.
    bool get( char* vals, long c0=0, long c1=0,
			long c2=0, long c3=0, long c4=0 ) const;
    bool get( short* vals, long c0=0, long c1=0,
			long c2=0, long c3=0, long c4=0 ) const;
    bool get( long* vals, long c0=0, long c1=0,
			long c2=0, long c3=0, long c4=0 ) const;
    bool get( int* vals, long c0=0, long c1=0,
			long c2=0, long c3=0, long c4=0 ) const;
    bool get( float* vals, long c0=0, long c1=0,
			long c2=0, long c3=0, long c4=0 ) const;
    bool get( double* vals, long c0=0, long c1=0,
			 long c2=0, long c3=0, long c4=0 ) const; 
    // Get n-dimensional arrays, starting at [0, 0, ..., 0] by default,
    // may be reset with set_cur().
    bool get( char* vals, const long* counts ) const;
    bool get( short* vals, const long* counts ) const;
    bool get( long* vals, const long* counts ) const;
    bool get( int* vals, const long* counts ) const;
    bool get( float* vals, const long* counts ) const;
    bool get( double* vals, const long* counts ) const;

    bool set_cur(long c0=-1, long c1=-1, long c2=-1,
			 long c3=-1, long c4=-1);
    bool set_cur(long* cur);

    // these put file in define mode, so could be expensive
    bool add_att( NcToken, char ); // add scalar attributes
    bool add_att( NcToken, short );
    bool add_att( NcToken, int );
    bool add_att( NcToken, long );
    bool add_att( NcToken, float );
    bool add_att( NcToken, double );

    bool add_att( NcToken, const char* ); // add string attribute

    bool add_att( NcToken, int, const char* ); // add vector attributes
    bool add_att( NcToken, int, const short* );
    bool add_att( NcToken, int, const int* );
    bool add_att( NcToken, int, const long* );
    bool add_att( NcToken, int, const float* );
    bool add_att( NcToken, int, const double* );

    bool rename( NcToken newname );

    int id( void ) const;		// rarely needed, C interface id
    
  protected:
    int the_id;
    long* the_cur;

    // protected constructors because only an NcFile creates these
    NcVar( void );
    NcVar(NcFile*, int);
    NcVar(NcFile*, NcToken);

    int attnum( NcToken attname ) const;
    NcToken attname( int attnum ) const;
    void init_cur( void );

    friend NcOldFile::NcOldFile( const char *, OpenMode );
    friend NcNewFile::NcNewFile( const char *, CreateMode );
    friend NcVar* NcFile::add_var( NcToken varname, NcType type,
					const NcDim*,
					const NcDim*,
					const NcDim*,
					const NcDim*,
					const NcDim*);
    friend NcVar* NcFile::add_var( NcToken, NcType, int,
					const NcDim* );
    
};

class NcAtt : public NcTypedComponent
{
public:	   
    ~NcAtt( void );
    NcToken name( void ) const;
    NcType type( void ) const;
    bool is_valid( void ) const;
    long num_vals( void ) const; 
    NcValues* values( void ) const;
    bool rename( NcToken newname );
    bool remove( void );
protected:
    const NcVar *the_variable;
    NcToken the_name;
    // protected constructors because only NcVars and NcFiles create
    // attributes
    NcAtt( NcFile*, const NcVar*, NcToken);
    NcAtt( NcFile*, NcToken); // global attribute

    friend NcOldFile::NcOldFile( const char *, OpenMode );
    friend NcNewFile::NcNewFile( const char *, CreateMode );
    friend NcAtt* NcVar::get_att( NcToken ) const;
};

// For controlling error handling
class NcError {
  public:
    enum Behavior {
	silent_nonfatal = 0,
	verbose_nonfatal = NC_VERBOSE,
	silent_fatal = NC_FATAL,
	verbose_fatal = NC_FATAL | NC_VERBOSE	   
      };

    // constructor saves previous error state, sets new state
    NcError( Behavior b = verbose_fatal );

    // destructor restores previous error state
    ~NcError( void );

    int get_err( void );			 // returns most recent error

  private:
    int the_old_state;
    int the_old_err;
};

#endif
