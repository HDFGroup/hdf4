/*****************************************************************************
* 
*			NCSA HDF / netCDF --- PROTOTYPE
*			        June 15, 1992
*
* NCSA HDF / netCDF source code and documentation are in the public domain.  
* Specifically, we give to the public domain all rights for future
* licensing of the source code, all resale rights, and all publishing rights.
* 
* We ask, but do not require, that the following message be included in all
* derived works:
* 
* Portions developed at the National Center for Supercomputing Applications at
* the University of Illinois at Urbana-Champaign.  Funding for this project 
* has come primarily from the National Science Foundation.
* 
* THE UNIVERSITY OF ILLINOIS GIVES NO WARRANTY, EXPRESSED OR IMPLIED, FOR THE
* SOFTWARE AND/OR DOCUMENTATION PROVIDED, INCLUDING, WITHOUT LIMITATION,
* WARRANTY OF MERCHANTABILITY AND WARRANTY OF FITNESS FOR A PARTICULAR PURPOSE
* 
******************************************************************************
*
* WARNING:  This is experimental software, while all of the netCDF software
*   tests pass, it is a safe bet that there are still bugs in the code.  
*
* Please report all bugs / comments to chouck@ncsa.uiuc.edu
*
* As this is a prototype implementation we can not guarantee that future
*   releases of NCSA HDF / netCDF software will be backward compatible 
*   with data created with this release.
*
*****************************************************************************/

/* A couple of local prototypes */

void hdf_cdf_clobber
  PROTO((NC *handle));

void hdf_close
  PROTO((NC *handle));

/* ----------------------------------------------------------------
** Map an NC_<type> to an HDF type
*/
int
  hdf_map_type(type)
int type;
{

  switch(type) {
  case NC_CHAR   :
    return DFNT_CHAR;
  case NC_BYTE   :
    return DFNT_INT8;
  case NC_SHORT   :
    return DFNT_INT16;
  case NC_LONG   :
    return DFNT_INT32;
  case NC_FLOAT   :
    return DFNT_FLOAT32;
  case NC_DOUBLE   :
    return DFNT_FLOAT64;
  default:
    return DFNT_NONE;
  }

} /* hdf_map_type */


/* ----------------------------------------------------------------
** UnMap a data type.  I.e. go from an HDF type to an NC_<type>
*/
int
  hdf_unmap_type(type)
int type;
{

  switch(type) {
  case DFNT_CHAR        :
    return NC_CHAR;
  case DFNT_INT8        :
  case DFNT_UINT8        :
    return NC_BYTE;
  case DFNT_INT16       :
  case DFNT_UINT16       :
    return NC_SHORT;
  case DFNT_INT32       :
  case DFNT_UINT32       :
    return NC_LONG;
  case DFNT_FLOAT32     :
    return NC_FLOAT; 
  case DFNT_FLOAT64     :
    return NC_DOUBLE;
  default:
    return FAIL;
  }

} /* hdf_unmap_type */

/* -----------------------------------------------------------------
** Given a dimension id number return its hdf_ref number (Vgroup id)
*/
int
  hdf_get_ref(handle, i)
NC *handle;
int i;
{
  NC_array *tmp;
  NC_dim **d;
  Void *dims;
  
  tmp = handle->dims;
  dims = handle->dims->values;
  dims += i * tmp->szof;
  
  d = (NC_dim **) dims;
  return (*d)->vgid;

} /* get_hdf_ref */

/* ----------------------------------------------------------------
** Given a dimension pointer return the ref of a Vdata which was
**   newly created to represent the values the dimension takes on
** If there is a variable with the same name as our dimension then
**   the values the variable takes on are the values for the
**   'steps' in the dimension.
** Otherwise, the dimension takes on the values 0..(size -1)
**
** NOTE:  This may cause conflicts cuz we may get called before
**   the variable's values are set???
** BUG:  Assume dimension values are always integers for now.
*/
int 
  hdf_create_dim_vdata(xdrs, handle, dim)
XDR *xdrs;
NC *handle;
NC_dim *dim;
{
  char *FUNC = "hdf_create_dim_vdata";
  int found = FALSE;
  int i;
  int ref;
  long dsize;

#if DEBUG
 fprintf(stderr, "hdf_create_dim_vdata I've been called\n");
 fprintf(stderr, "handle->hdf_file = %d\n", handle->hdf_file);
#endif

  dsize = dim->size;
  if(dsize == NC_UNLIMITED) {
#ifdef DEBUG
      fprintf(stderr, "Fudging unlimited dimension.\n");
#endif
      dsize = 1;
  }

#if 0
  /* look for variable with the given name */
  if(handle->vars) {
    dp = (NC_var **) handle->vars->values;
    for(ii = 0; ii < handle->vars->count; ii++, dp++)
      if(HDstrcmp(dim->name->values, (*dp)->name->values) == 0) {
	/* found it */
	found = TRUE;
	break;
      }
  }

#endif

  if(found) {
    /* load in the variable's values */
#if DEBUG
   fprintf(stderr, "Found real values for dimension %s\n", dim->name->values);
#endif

  } else {
      int *val;
      /* create a fake one */
#if DEBUG
      fprintf(stderr, "Creating fake dim  ::::%s::: (%d)\n", dim->name->values, dsize);
#endif
      
      /* allocate space */
      val = (int *) malloc(dsize * sizeof(int));
      if(!val) {
          HERROR(DFE_NOSPACE);
          return FAIL;
      }
      
      if(dim->size != NC_UNLIMITED) {
          for(i = 0; i < dsize; i++)
              val[i] = i;
      } else {
          val[0] = handle->numrecs;
      }
      
      ref = VHstoredata(handle->hdf_file, "Values", (void *)val, dsize,
                        DFNT_INT32, dim->name->values, DIM_VALS);
      
      if(ref == FAIL) {
          fprintf(stderr, "FAILed creating Vdata %s\n", dim->name->values);
          return FAIL;
      }
      
      free(val);

  }
  
#if DEBUG
  fprintf(stderr, "Returning vdata pointer %d\n", ref);
#endif
  
  return ref;
  
} /* hdf_create_dim_vdata */

/* ----------------------------------------------------------------
** Write out a vdata representing an attribute
*/
int
  hdf_write_attr(xdrs, handle, attr)
XDR *xdrs;
NC *handle;
NC_attr **attr;
{
  int status;
  char *name;
  Void *values;
  int size;
  int type;
  int order;

  name = (*attr)->name->values;
  values = (*attr)->data->values;
  size = (*attr)->data->count;
  type = hdf_map_type((*attr)->data->type);

#if DEBUG
 fprintf(stderr, "hdf_write_attr I've been called\n");
 fprintf(stderr, "The attribute is called %s\n", name);
 fprintf(stderr, "Type = %d (%d)  size  %d\n", type, (*attr)->data->type, size);
 fprintf(stderr, "Value: ");
  switch(type) {
  case DFNT_CHAR :fprintf(stderr, " (char) %s\n", (char *) values); break;
  case DFNT_INT32 :fprintf(stderr, " (long) %d\n", (int *) values); break;
  case DFNT_FLOAT32 :fprintf(stderr, " (float) %f\n", (float *) values); break;
  case DFNT_FLOAT64 :fprintf(stderr, " (double) %f\n", (double *) values); break;
  default:fprintf(stderr, "???\n");
  }
#endif

  if(type == DFNT_CHAR) {
      order = size;
      size = 1;
  } else { 
      order = 1;
  }
      
  status = VHstoredatam(handle->hdf_file, "Values", 
                        (unsigned char *) values, size, 
                        type, name, ATTRIBUTE, order);

#if DEBUG
 fprintf(stderr, "hdf_write_attr returning %d\n", status);
#endif

  return status;

} /* hdf_write_attr */

/* ----------------------------------------------------------------
** Write out a group representing a dimension
*/
int
  hdf_write_dim(xdrs, handle, dim)
XDR *xdrs;
NC *handle;
NC_dim **dim;
{
  int status;
  int tags[100], refs[100];
  int count;
  char *class;

#if DEBUG
 fprintf(stderr, "hdf_write_dim I've been called\n");
 fprintf(stderr, "The name is -- %s -- \n", (*dim)->name->values);
#endif

  /*
   * Look up to see if there is a variable of the same name
   *  giving values
   */
  count = 0;
  tags[count] = DFTAG_VH;
  refs[count] = hdf_create_dim_vdata(xdrs, handle, (*dim));
  count++;
  
  if((*dim)->size == NC_UNLIMITED) 
      class = UDIMENSION;
  else
      class = DIMENSION;
  
  status = VHmakegroup(handle->hdf_file, tags, refs, count, 
		       (*dim)->name->values, class);

  if(status == FAIL)
      HEprint(stdout, 0);

  (*dim)->vgid = status;

  return status;

} /* hdf_write_dim */


/* ----------------------------------------------------------------
** Write out a group representing a variable
** If successfull, return the id of the created Vgroup else
**  return NULL
*/
int
  hdf_write_var(xdrs, handle, var)
XDR *xdrs;
NC *handle;
NC_var **var;
{
  NC_array *attrs;
  NC_iarray *assoc;
  uint8 ntstring[4];
  uint16 ref;
  int tags[100], refs[100];

  register int  i, count;
  register Void *attribute;

  count = 0;
  assoc = (*var)-> assoc;
  attrs = (*var)-> attrs;

#if DEBUG
 fprintf(stderr, "hdf_write_var I've been called\n");
 fprintf(stderr, "handle->hdf_file = %d\n", handle->hdf_file);
 fprintf(stderr, "The name is -- %s -- \n", (*var)->name->values);

  if(assoc && assoc->count) {
    fprintf(stderr, "value of assoc %d\n", assoc);
    fprintf(stderr, " assoc->count %d\n", assoc->count);
    fprintf(stderr, " asc[0] %d asc[1] %d\n", assoc->values[0], assoc->values[1]);
  }
#endif

  /*
   *  Get the dimension information
   */
  for(i = 0; i < assoc->count; i++) {
    tags[count] = DIM_TAG;
    refs[count] = hdf_get_ref(handle, assoc->values[i]);
    count++;
  }

  /* 
   * Add info for the attributes
   */
  if(attrs) {
    attribute = attrs->values;
    for(i = 0; i < attrs->count; i++) {
      tags[count] = ATTR_TAG;
      refs[count] = hdf_write_attr(xdrs, handle, attribute);
      attribute += attrs->szof;
      count++;
    }
  }
  
  /*
   * If we already have data written out include that too
   *   (this might happen after a redef() cuz we will leave
   *   the data sitting on the disk but clear out all the 
   *   meta-data)
   */
  if((*var)->data_ref) {
      tags[count] = DFTAG_SD;
      refs[count] = (*var)->data_ref;
#if DEBUG
      fprintf(stderr, " ---- Carrying forward data with ref %d ---- \n", (*var)->data_ref);
#endif 
      count++;
  } 
  
  
  /*
   * Write out a number type tag so that we can recover this 
   *   variable's type later on
   */
  ref = Hnewref(handle->hdf_file);
  ntstring[0] = DFNT_VERSION;                    /* version */
  ntstring[1] = (uint8)((*var)->HDFtype & 0xff); /* type */
  ntstring[2] = (uint8)((*var)->HDFsize * 8);    /* width (in bits) */
  ntstring[3] = DFNTF_NONE;                      /* class: IEEE or machine class */
  if(Hputelement(handle->hdf_file, DFTAG_NT, ref, ntstring, (int32) 4) == FAIL)
      return FAIL;
  tags[count] = DFTAG_NT;
  refs[count] = ref;
  count++;
  
  (*var)->vgid = VHmakegroup(handle->hdf_file, tags, refs, count, 
                             (*var)->name->values, VARIABLE);
  if((*var)->vgid == FAIL)
      HEprint(stdout, 0);
  
  return (*var)->vgid;
  
} /* hdf_write_var */


/* ----------------------------------------------------------------
** Write out a cdf structure
*/
bool_t
  hdf_write_xdr_cdf(xdrs, handlep)
XDR *xdrs;
NC **handlep;
{
  int32 *tags, *refs, count;
  int sz, i, status;
  NC_array *tmp;
  Void *dims;
  Void *vars;
  Void *attrs;

#if DEBUG
 fprintf(stderr, "hdf_write_xdr_cdf i've been called op = %d \n", xdrs->x_op);
#endif

  /* count size of tag / ref arrays */
  sz = 0;
  if((*handlep)->dims)  sz += (*handlep)->dims->count;
  if((*handlep)->vars)  sz += (*handlep)->vars->count;
  if((*handlep)->attrs) sz += (*handlep)->attrs->count;

#if DEBUG
 fprintf(stderr, "sz = %d\n", sz);
#endif

  /* allocate tag / ref arrays */
  tags = (int32 *) malloc(sz * sizeof(int32) + 1);
  refs = (int32 *) malloc(sz * sizeof(int32) + 1);
  if(!tags || !refs) {
    fprintf(stderr, "Out of memory line %d file %s\n", __LINE__, __FILE__);
    return FALSE;
  }

  /* 
  ** write out dimension arrays 
  */
  count = 0;
  if((*handlep)->dims) {
    tmp = (*handlep)->dims; 
    dims = (*handlep)->dims->values;
    for(i = 0; i < tmp->count; i++) {
      tags[count] = (int32) DIM_TAG;
      refs[count] = (int32) hdf_write_dim(xdrs, (*handlep), dims);
      dims += tmp->szof;
      count++;
    }
  }

  /* 
  ** write out variable info 
  */
  if((*handlep)->vars) {
      tmp = (*handlep)->vars; 
      vars = (*handlep)->vars->values;
      for(i = 0; i < tmp->count; i++) {
          tags[count] = (int32) VAR_TAG;
          refs[count] = (int32) hdf_write_var(xdrs, (*handlep), vars);
          vars += tmp->szof;
          count++;
      }
  }

  /*
   * write global attribute information
   */
  if((*handlep)->attrs) {
      tmp = (*handlep)->attrs; 
      attrs = (*handlep)->attrs->values;
      for(i = 0; i < tmp->count; i++) {
          tags[count] = (int32) ATTR_TAG;
          refs[count] = (int32) hdf_write_attr(xdrs, (*handlep), attrs);
          attrs += tmp->szof;
          count++;
      }
  }

#if DEBUG
  fprintf(stderr, "About to write top level VG with %d elements\n", count);
#endif

  /* write out final VGroup thang */
  status = VHmakegroup((*handlep)->hdf_file, tags, refs, count, 
		       (*handlep)->path, CDF);
  if(status == FAIL)
      HEprint(stdout, 0);

#ifdef DEBUG
  fprintf(stderr, "======= Have finished writing top level VGroup #%d\n", status);
#endif
  
  /* set the top level CDF VGroup pointer */
  (*handlep)->vgid = status;
  
  return TRUE;

} /* hdf_write_xdr_cdf */


/* ----------------------------------------------------------------
** Read in the dimensions out of a cdf structure
** Return FAIL if something goes wrong
*/
int 
  hdf_read_dims(xdrs, handle, vg)
XDR    *xdrs;
NC     *handle;
int32  vg;
{

  char vgname[100], vsclass[128], vgclass[128];
  int id, count, found, entries;
  int sub_id;
  int32 dim_size;
  NC_dim **dimension;
  int32 dim;
  int32 vs;

  found = FALSE;
  count = 0;
  id = -1;

#if DEBUG
 fprintf(stderr, "hdf_read_dims I've been called, handle->hdf_file = %d\n", handle->hdf_file);
#endif

  /*
   * Allocate enough space in case everything is a dimension
   */
  count = 0;
  dimension = (NC_dim **) malloc(sizeof(NC_dim *) * Vntagrefs(vg) + 1);
  if(!dimension) {
    fprintf(stderr, "Out of memory line %d file %s\n", __LINE__, __FILE__);
    return FAIL;
  }

  /*
   * Look through for a Vgroup of class DIMENSION
   */
  while((id = Vgetnext(vg, id)) != FAIL) {
    if(Visvg(vg, id)) {
      dim = Vattach(handle->hdf_file, id, "r");
      if(dim == FAIL) continue;
      Vgetclass(dim, vgclass);
      if(!HDstrcmp(vgclass, DIMENSION) || !HDstrcmp(vgclass, UDIMENSION)) {
	Vinquire(dim, &entries, vgname);      
	
	/* 
	 * look through for a Vdata of class DIM_VALS to get size 
	 */
	sub_id = -1;
	while(((sub_id = Vgetnext(dim, sub_id)) != FAIL) && !found) {
	  if(Visvs(dim, sub_id)) {
	    vs = VSattach(handle->hdf_file, sub_id, "r");
	    if(vs == FAIL) HEprint(stdout, 0);
	    
            VSgetclass(vs, vsclass);
	    if(!HDstrcmp(vsclass, DIM_VALS)) {
              VSQuerycount(vs, &dim_size);

              if(!HDstrcmp(vgclass, UDIMENSION)) {
                  dim_size = NC_UNLIMITED;
                  VSsetfields(vs, "Values");
                  VSseek(vs, 0);
                  if(VSread(vs, (VOIDP) &(handle->numrecs), 1, FULL_INTERLACE) != 1)
                      HEprint(stderr, 0);
              }

	      dimension[count] = NC_new_dim(vgname, dim_size);
	      if(!dimension[count]) {
		fprintf(stderr, "Can't create new dimension #%d\n", count);
		return FAIL;
	      }
	      
#if DEBUG
	     fprintf(stderr, "Dimension <%s> has size %d\n", vgname, dim_size);
#endif
	      count++;
	      
	    }
	    VSdetach(vs);
	  }
	}
      }
      Vdetach(dim);
    }
  }

  if(count)
    handle->dims = NC_new_array(NC_DIMENSION, count, (Void *) dimension);
  else
    handle->dims = NULL;

  free(dimension);

#if DEBUG
 fprintf(stderr, "Created dimension array %d \n", handle->dims);
#endif

  return SUCCEED;

} /* hdf_read_dims */

/* ----------------------------------------------------------------
** Read in all attributes of the current vg
** Return NULL if something goes wrong
** Return a pointer to the array of attributes if all goes well
*/
NC_array *
  hdf_read_attrs(xdrs, handle, vg)
XDR    *xdrs;
NC     *handle;
VGROUP *vg;
{

  char vsname[100], fields[100], *values, class[128];
  int id, tag, count, attr_size, type, vsize, t, n;
  NC_attr **attributes;
  NC_array *Array = NULL;
  int32 vs;

  count = 0;
  id = -1;

#if DEBUG
 fprintf(stderr, "hdf_read_attrs I've been called, handle->hdf_file = %d\n", handle->hdf_file);
#endif

  /*
   * Allocate enough space in case everything is an attribute
   */
  count = 0;
  attributes = (NC_attr **) malloc(sizeof(NC_attr *) * Vntagrefs(vg) + 1);
  if(!attributes) {
    fprintf(stderr, "Out of memory line %d file %s\n", __LINE__, __FILE__);
    return NULL;
  }

  /*
   * look through for a Vdata of class ATTRIBUTE
   */
  n = Vntagrefs(vg);
  for (t = 0; t < n; t++) {
      Vgettagref(vg, t, &tag, &id);
      
      if(tag == DFTAG_VH) {
          vs = VSattach(handle->hdf_file, id, "r");
          if(vs == FAIL) 
              HEprint(stdout, 0);
          
          VSgetclass(vs, class);
          if(!HDstrcmp(class, ATTRIBUTE)) {
              VSinquire(vs, &attr_size, NULL, fields, &vsize, vsname);
              type = hdf_unmap_type(VFfieldtype(vs, 0));
              values = (char *) malloc(vsize * attr_size + 1);
              VSsetfields(vs, fields);
              VSread(vs, (uint8 *) values, attr_size, FULL_INTERLACE);
              
              if(type == NC_CHAR) {
                  attr_size = VFfieldorder(vs, 0);
                  ((char *) values)[attr_size] = '\0';
              }
              attributes[count] = 
                  (NC_attr *) NC_new_attr(vsname, type, attr_size, values);
              if(!attributes[count]) {
                  fprintf(stderr, "Can't create new attribute #%d\n", count);
                  return NULL;
              }
              
#if DEBUG
              fprintf(stderr, "Attribute <%s> has type %d and size %d\n", 
                      vsname, type, attr_size);
#endif
              count++;
              
          }
          VSdetach(vs);
      }
  }
  
  if(count) Array = NC_new_array(NC_ATTRIBUTE, count, (Void *) attributes);

  free(attributes);

#if DEBUG
  fprintf(stderr, "Created attribute array %d \n", Array);
#endif

  return Array;

} /* hdf_read_attrs */



/* ----------------------------------------------------------------
** Read in the variables out of a cdf structure
** Return FAIL if something goes wrong
**
** Important:  We must already assume that handle->dims is set
**   so that we can do a call to NC_var_shape() so that we can
**   set the numrecs fields of variables (so we can fill record
**   variables intelligently)
*/
int 
  hdf_read_vars(xdrs, handle, vg)
XDR    *xdrs;
NC     *handle;
int32  vg;
{

  char vgname[100], subname[100], class[128];
  int  count, entries;
  NC_var **variables, *vp;
  int    ndims, *dims, n;
  uint8 ntstring[4];
  int tag, sub_id, data_ref, is_rec_var, vg_size, id;
  int32 data_count, HDFtype;

  register int     type, t, i;
  register int32   var, sub;

  count = 0;
  id = -1;

#if DEBUG
  fprintf(stderr, "hdf_read_vars I've been called, handle->hdf_file = %d\n", handle->hdf_file);
#endif

  /*
   * Allocate enough space in case everything is a variable
   */
  count = 0;
  variables = (NC_var **) malloc(sizeof(NC_var *) * Vntagrefs(vg) + 1);
  if(!variables) {
    fprintf(stderr, "Out of memory line %d file %s\n", __LINE__, __FILE__);
    return FAIL;
  }

  /*
   * Allocate enough space in case lots of dimensions
   */
  dims = (int *) malloc(sizeof(int) * Vntagrefs(vg) + 1);
  if(!dims) {
    fprintf(stderr, "Out of memory line %d file %s\n", __LINE__, __FILE__);
    return FAIL;
  }

  /*
   * Look through for a Vgroup of class VARIABLE
   */
  vg_size = Vntagrefs(vg);
  for(i = 0; i < vg_size; i++) {
      Vgettagref(vg, i, &tag, &id);
      if(tag == DFTAG_VG) {
          var = Vattach(handle->hdf_file, id, "r");
          if(var == FAIL) continue;
          
          Vgetclass(var, class);
          if(!HDstrcmp(class, VARIABLE)) {
              
              /*
               * We have found a VGroup representing a Variable
               */
              ndims = 0;
              type = 0;
              data_ref = 0;
              data_count = 0;
              is_rec_var = FALSE;
              Vinquire(var, &n, vgname);      
          
              /*
               * Loop through contents looking for dimensions
               */
              for (t = 0; t < n; t++) {
                  Vgettagref(var, t, &tag, &sub_id);
                  
                  switch(tag) {
                  case DFTAG_VG :   /* ------ V G R O U P ---------- */
                      sub = Vattach(handle->hdf_file, sub_id, "r");

                      Vgetclass(sub, class);
                      if(!HDstrcmp(class, DIMENSION) || 
                         !HDstrcmp(class, UDIMENSION)) {
                          
                          if(!HDstrcmp(class, UDIMENSION)) {
#ifdef DEBUG
                              fprintf(stderr, "Found a rec-var %s\n", vgname);
#endif
                              is_rec_var = TRUE;
                          }
                          
                          Vinquire(sub, &entries, subname);      
                          
                          dims[ndims] = (int) NC_dimid( handle, subname);
#if DEBUG
                          fprintf(stderr, "Var <%s> has dimension <%s> #%d\n", 
                                  vgname, subname, dims[ndims]);
#endif
                          ndims++;
                      }
                      Vdetach(sub);
                      break;
                  case DFTAG_VH :   /* ----- V D A T A --------- */

#ifdef OLD_DATA
                      vs = VSattach(handle->hdf_file, sub_id, "r");
                      if(!HDstrcmp(VSCLASS(vs), DATA)) {
                          type = hdf_unmap_type(VFfieldtype(vs, 0));
                          data_ref = sub_id; 
                          VSQuerycount(vs, &data_count);
#if DEBUG
                          fprintf(stderr, "Var <%s> has type %d\n", vgname, type);
#endif
                      }
                      VSdetach(vs);
#endif
                      break;

                  case DFTAG_SD :   /* ------- Data Storage ------ */
                      data_ref = sub_id;
                      data_count = Hlength(handle->hdf_file, DATA_TAG, sub_id);
                      break;
                  case DFTAG_NT :   /* ------- Number type ------- */
                      if(Hgetelement(handle->hdf_file, tag, sub_id, ntstring) == FAIL)
                          return FAIL;
                      HDFtype = ntstring[1];
                      type = hdf_unmap_type(HDFtype);
                      break;
                  default:
                      /* Do nothing */
                      break;
                  }
              }
          
              variables[count] = NC_new_var(vgname, type, ndims, dims);
              vp = variables[count];
              if(!vp) {
                  fprintf(stderr, "Can't read new variable %s\n", vgname);
                  return FAIL;
              }
              
#if DEBUG
              printf("Created a variable called %s   (id %d) \n", vgname, id);
#endif
              /* Read in the attributes */
              vp->attrs = hdf_read_attrs(xdrs, handle, var);
              
              /* set up for easy access later */
              vp->vgid     = id;
              vp->data_ref = data_ref;
              vp->data_tag = DATA_TAG;
              vp->HDFtype  = HDFtype;
              
              if(vp->data_ref) {
                  /*
                   * We have already seen data for this variable so now
                   *  we need to worry about its numrecs field
                   */
                  
                  if(is_rec_var) {
                      /*
                       * Call NC_var_shape() so we can figure out how many
                       *  records have been written.  This is horribly 
                       *  inefficient, but the separation-of-powers gets
                       *  really mucked up if we wait till later...
                       */
                      
                      if(NC_var_shape(vp, handle->dims) == -1)
                          return FAIL;
                      
                      /*
                       * Now figure out how many recs have been written
                       */
/*                      vp->numrecs = data_count / (vp->dsizes[0] / NC_typelen(vp->type)); */
                      vp->numrecs = data_count / vp->dsizes[0] - 1; 
                      
#ifdef DEBUG
                      fprintf(stderr, "I have set numrecs to %d\n", vp->numrecs);
#endif                  
                      /*
                       * Deallocate the shape info as it will be recomputed
                       *  at a higher level later
                       */
                      Free(vp->shape);
                      Free(vp->dsizes);
                      
                  } else {
                      /* Not a rec var, don't worry about it */
                      vp->numrecs = 1;
                  }
              } 
              
              count++;
              
          }
          Vdetach(var);
      }
  }
  
  if(count)
      handle->vars = NC_new_array(NC_VARIABLE, count, (Void *) variables);
  else
      handle->vars = NULL;

  free(variables);
  
#if DEBUG
  fprintf(stderr, "Created variable array %d \n", handle->vars);
#endif
  
  return SUCCEED;

} /* hdf_read_vars */


/* ----------------------------------------------------------------
** Read in a cdf structure
*/
bool_t
  hdf_read_xdr_cdf(xdrs, handlep)
XDR *xdrs;
NC **handlep;
{
  char            vgname[100], class[128];
  register int32  cdf_vg;
  register int    vgid, found, status;
  int             entries;

#if DEBUG
 fprintf(stderr, "hdf_read_xdr_cdf i've been called %d\n", (*handlep)->hdf_file);
#endif

  /* find first thing of type CDF */
  vgid = -1;
  found = FALSE;
  while(!found && ((vgid = Vgetid((*handlep)->hdf_file, vgid)) != FAIL)) {
    cdf_vg = Vattach((*handlep)->hdf_file, vgid, "r");
    if(cdf_vg == FAIL) {
        fprintf(stderr, "Oops\n");
        return FALSE;
    }
    Vgetclass(cdf_vg, class);
    if(!HDstrcmp(class, CDF)) {
        found = TRUE;
    } else {
        Vdetach(cdf_vg);
    }
  }

  if(!found)
    return FALSE;

  (*handlep)->vgid = vgid;

  Vinquire(cdf_vg, &entries, vgname);

#if DEBUG
 fprintf(stderr, "Found CDF : %s  (%d entries)\n", vgname, entries);
#endif

  /* read in dimensions */
  status = hdf_read_dims(xdrs, (*handlep), cdf_vg);
  if(status == FAIL)
    return FALSE;

  /* read in variables */
  status = hdf_read_vars(xdrs, (*handlep), cdf_vg);
  if(status == FAIL)
    return FALSE;

  /* read in attributes */
  (*handlep)->attrs = hdf_read_attrs(xdrs, (*handlep), cdf_vg);

  Vdetach(cdf_vg);

  return TRUE;

} /* hdf_read_xdr_cdf */

/* -------------------------------------------------------------------
** Read or write a CDF structure
**
** If we are reading, first try to read the information out of netCDF
**    object stored explicitly in HDF files as netCDF objects.  If
**    that fails try to read SDSs out of the HDF file and interpret
**    them as netCDF information.
*/
bool_t
  hdf_xdr_cdf(xdrs, handlep)
XDR *xdrs;
NC **handlep;
{

  int status;

#if DEBUG
  fprintf(stderr, "xdr_cdf i've been called op = %d \n", xdrs->x_op);
  setjj();
#endif

  switch(xdrs->x_op) {
  case XDR_ENCODE :
      if((*handlep)->vgid) hdf_cdf_clobber((*handlep));
      status = hdf_write_xdr_cdf(xdrs, handlep);
      break;
  case XDR_DECODE :
      if((status = hdf_read_xdr_cdf(xdrs, handlep)) == FALSE)
          status = hdf_read_sds_cdf(xdrs, handlep);
      break;
  default:
      status = TRUE;
  }
  
  return status;
  
} /* hdf_xdr_cdf */


/* ---------------------- hdf_vg_clobber --------------- */
/*
  Delete a VGroup that is on the disk.  Basically, we will want to
  just trash everything inside of it, making sure that any VDatas
  with class == DATA are saved
*/
void hdf_vg_clobber(handle, id)
NC *handle;
int id;
{
  int tag, ref, t, n;
  int32 vg;
  int32 vs, status;

  /* loop through and Clobber all top level VGroups */
  vg = Vattach(handle->hdf_file, id, "r");
  if(vg == FAIL) return;

  /* Loop though and kill stuff */
  n = Vntagrefs(vg);
  for (t = 0; t < n; t++) {
    Vgettagref(vg, t, &tag, &ref);

#ifdef DEBUG
    fprintf(stderr, " Looking at <%d, %d>\n", tag, ref);
    fflush(stderr);
#endif
    switch(tag) {
    case DFTAG_VG :
        hdf_vg_clobber(handle, ref);
        break;
    case DFTAG_VH :
        vs = VSattach(handle->hdf_file, ref, "r");
        if(vs == FAIL) continue;
        
        VSdetach(vs);
        status = VSdelete(handle->hdf_file, (int32) ref);
/*
        Hdeldd(handle->hdf_file, DFTAG_VH, ref);
        Hdeldd(handle->hdf_file, DFTAG_VS, ref);
*/ 
        if(status == FAIL) printf("Yikes failed deleting Vdata %d\n",ref);

        break;
    case DFTAG_SD :
        /*
         * Don't delete actual numeric data
         */
        break;
    default:
        Hdeldd(handle->hdf_file, (uint16) tag, (uint16) ref);
        break;
    }
  }

  Vdetach(vg);

} /* hdf_vg_clobber */


/* --------------------------- hdf_cdf_clobber ---------------------------- */
/*
  Delete a netCDF structure that has been already written to disk
*/
void hdf_cdf_clobber(handle)
NC *handle;
{
  int32  vg;
  int n, t;
  int tag, ref, status;

  if(!handle->vgid) return;

  /* Close open VData pointers */
  hdf_close(handle);

  /* loop through and Clobber all top level VGroups */
  vg = Vattach(handle->hdf_file, handle->vgid, "r");
  if(vg == FAIL) return;

  /* Loop though and just kill everyone */
  n = Vntagrefs(vg);
  for (t = 0; t < n; t++) {
    Vgettagref(vg, t, &tag, &ref);
    if(tag == DFTAG_VG) hdf_vg_clobber(handle, ref);

    switch(tag) {
    case DFTAG_VG:
        status = Vdelete(handle->hdf_file, (int32) ref);
        break;
    case DFTAG_VH:
        status = VSdelete(handle->hdf_file, (int32) ref);
        break;
    default:
        status = Hdeldd(handle->hdf_file, (uint16) tag, (uint16) ref);
    }

    if(status == FAIL) printf("Yikes failed deleting %d\n", tag);

  }

#ifdef DEBUG
  fprintf(stderr,"Clobbering VGroup %d\n", handle->vgid);
#endif
  
  Vdetach(vg);
  status = Vdelete(handle->hdf_file, (int32) handle->vgid);
  if(status == FAIL) printf("Yikes failed deleting VGroup\n");
/*   Hdeldd(handle->hdf_file, DFTAG_VG, handle->vgid); */
  handle->vgid = 0;

#ifdef DEBUG
  fprintf(stderr, "    ALL done with the cdf_clobber()\n");
#endif

} /* hdf_cdf_clobber */

/* -------------------------- hdf_close --------------------- */
/*
  We're about to close the file, do last minute HDF cleanup
  Also dump the number of records currently instatiated into the
  unlimited dimensions.  BUG:  All unlimited dimensions will have
  the same size
*/
void hdf_close(handle)
     NC *handle;
{
    NC_array *tmp;
    NC_var **vp;
    Void *vars;
    register int i;
    int id, sub_id;
    int32 vg, dim;
    int32 vs;
    char class[128];
    
#ifdef DEBUG
    fprintf(stderr,"hdf_close, I've been called\n");
#endif
    
    /* loop through and detach from variable data VDatas */
    if(handle->vars) {
        tmp = handle->vars; 
        vars = handle->vars->values;
        
        for(i = 0; i < tmp->count; i++) {
            vp = (NC_var **) vars;

#ifdef OLD_DATA
            if((*vp)->vs)
                VSdetach((*vp)->vs);
            (*vp)->vs = NULL;
#else
            if((*vp)->aid)
                Hendaccess((*vp)->aid);
            (*vp)->aid = NULL;
#endif

            vars += tmp->szof;
        }
    }
    
    /* loop through top level looking for unlimited dimensions */
    if(handle->flags & NC_NDIRTY) {
        id = -1;
        vg = Vattach(handle->hdf_file, handle->vgid, "r");
        while((id = Vgetnext(vg, id)) != FAIL) {
            if(Visvg(vg, id)) {
                dim = Vattach(handle->hdf_file, id, "r");
                Vgetclass(dim, class);
                if(!HDstrcmp(class, UDIMENSION)) {
                    
                    sub_id = -1;
                    while((sub_id = Vgetnext(dim, sub_id)) != FAIL) {
                        if(Visvs(dim, sub_id)) {
                            vs = VSattach(handle->hdf_file, sub_id, "w");
                            if(vs == FAIL) HEprint(stdout, 0);
                            
                            VSgetclass(vs, class);
                            if(!HDstrcmp(class, DIM_VALS)) {
                                VSsetfields(vs, "Values");
                                VSseek(vs, 0);
                                if(VSwrite(vs, (VOIDP) &(handle->numrecs), 1, FULL_INTERLACE) != 1) 
                                    HEprint(stderr, 0);  

                            }
                            VSdetach(vs);
                        }
                    }
                }
                Vdetach(dim);
            }
        }
        Vdetach(vg);
    }

} /* hdf_close */
                
