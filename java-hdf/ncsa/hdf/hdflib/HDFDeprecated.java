/****************************************************************************
 * NCSA HDF                                                                 *
 * National Comptational Science Alliance                                   *
 * University of Illinois at Urbana-Champaign                               *
 * 605 E. Springfield, Champaign IL 61820                                   *
 *                                                                          *
 * For conditions of distribution and use, see the accompanying             *
 * hdf/COPYING file.                                                        *
 *                                                                          *
 ****************************************************************************/

package ncsa.hdf.hdflib;

import ncsa.hdf.hdflib.*;

import java.io.*;
import java.net.*;
import java.lang.*;
import java.util.*;

public class HDFDeprecated extends HDFLibrary {


	static {
		System.loadLibrary("hdf");
	}

    public HDFDeprecated()  {
    }

 
     public native boolean  DFANaddfds(int file_id, String description, int desc_len) throws HDFException;
 
     public native boolean DFANaddfid(int file_id, String label) throws HDFException;
 
     public native boolean DFANclear( ) throws HDFException;
 
     public native boolean DFANgetdesc(String filename, short tag, short ref,
 	String[] desc_buf, int buf_len) throws HDFException;
 
     public native int DFANgetdesclen(String filename, short tag, short ref) throws HDFException;
 
     public native int DFANgetfds(int file_id, String[] desc_buf, int buf_len, int isfirst) throws HDFException;
 
     public int DFANgetfds(int file_id, String[] desc_buf, int buf_len, boolean isfirst)  throws HDFException{
 		if (isfirst) {
 		   return DFANgetfds(file_id, desc_buf, buf_len, 1);	
 		} else {
 		   return DFANgetfds(file_id, desc_buf, buf_len, 0);	
 		}
 	}
 
 
     public native int DFANgetfdslen(int file_id, int isfirst) throws HDFException;
 
     public int DFANgetfdslen(int file_id, boolean isfirst)  throws HDFException{
 		if (isfirst) {
 		   return DFANgetfdslen(file_id, 1);	
 		} else {
 		   return DFANgetfdslen(file_id, 0);	
 		}
 	}
 
     public native int DFANgetfid(int file_id, String[] desc_buf, int buf_len, int isfirst) throws HDFException;
 
     public int DFANgetfid(int file_id, String[] desc_buf, int buf_len, boolean isfirst) throws HDFException{
 		if (isfirst) {
 		   return DFANgetfid(file_id, desc_buf, buf_len, 1);	
 		} else {
 		   return DFANgetfid(file_id, desc_buf, buf_len, 0);	
 		}
 	}
 
     public native int DFANgetfidlen(int file_id, int isfirst) throws HDFException;
 
     public int DFANgetfidlen(int file_id, boolean isfirst)  throws HDFException {
 		if (isfirst) {
                    return DFANgetfidlen(file_id, 1);
                 } else {
                    return DFANgetfidlen(file_id, 0);
                 }
         }
 
 
     public native boolean DFANgetlabel(String filename, short tag, short ref,
 	String[] label_buf, int buf_len) throws HDFException;
 
     public native int DFANgetlablen(String filename, short tag, short ref) throws HDFException;
 
 
     public native int DFANlablist(String filename, short tag, short[] ref_list, 
 		String[] label_list, int list_len, int label_len, int start_pos) throws HDFException;
 
     public native short DFANlastref( ) throws HDFException;
 
     public native boolean DFANputdesc(String filename, short tag, short ref,
 	 String description, int desc_len) throws HDFException;
 
     public native boolean DFANputlabel(String filename, short tag, short ref,
 	String label) throws HDFException;
 
     public native boolean DFSDadddata(String filename, int rank, int[] dimsizes, byte[] data) throws HDFException;
 
     public boolean DFSDadddata(String filename, int rank, int[] dimsizes, Object theData)  throws HDFException{
 		byte[] data;
 		int rval;
 
 		HDFArray theArray = new HDFArray(theData);
 		data = theArray.byteify();
 		return DFSDadddata(filename, rank, dimsizes, data);
 	}
 
     public native boolean DFSDclear( ) throws HDFException;
     public native boolean DFSDendslab( ) throws HDFException;
     public native boolean DFSDendslice( ) throws HDFException;
 
 
     public native boolean DFSDgetcal(double[] calInfo) throws HDFException;
 
     public native boolean DFSDgetdata(String filename, int[] rank, int[] dimsizes, byte[] data) throws HDFException;
 
     public boolean DFSDgetdata(String filename, int[] rank, int[] dimsizes, Object theData) throws HDFException {
 		byte[] data;
 		boolean rval;
 
 		HDFArray theArray = new HDFArray(theData);
 		data = theArray.emptyBytes();
 	        rval = DFSDgetdata(filename, rank, dimsizes,  data);
 		theData = theArray.arrayify( data );
 		return rval;
 	}
 
 
     public native boolean DFSDgetdatalen(int[] info) throws HDFException;
 
     public native boolean DFSDgetdatastrs(String[] datastrs) throws HDFException;
 
     public native boolean DFSDgetdimlen(int dim, int[] label_len) throws HDFException;
 
     public native boolean DFSDgetdims(String filename, int[] rank, int[] dimsizes, int maxrank) throws HDFException;
 
     public native boolean DFSDgetdimscale(int dim, int size, byte[] scale) throws HDFException;
 
     public boolean DFSDgetdimscale(int dim, int size, Object theScale)  throws HDFException{
 		byte[] scale;
                 boolean rval;
 
                 HDFArray theArray = new HDFArray(theScale);
                 scale = theArray.emptyBytes();
 		rval = DFSDgetdimscale(dim, size, scale);
                 theScale = theArray.arrayify( scale );
                 return rval;
         }
 
     
     public native boolean DFSDgetdimstrs(int dim, String[] dimstrs) throws HDFException;
 
     public native boolean DFSDgetfillvalue(byte[] fill_value) throws HDFException;

     public boolean  DFSDgetfillvalue( Object [] theFillValue) throws HDFException {
	int NT;
        int[] nt = new int[1];
        DFSDgetNT(nt);
	NT = nt[0];
		HDFNativeData convert = new HDFNativeData();
		byte[] d1 = new byte[8];
		boolean rval;
		rval = DFSDgetfillvalue( d1 );
		if ((NT == HDFConstants.DFNT_INT8 ) 
		 || (NT == HDFConstants.DFNT_CHAR8 )
		 || (NT == HDFConstants.DFNT_CHAR )
			) {
			theFillValue[0] = (Object)new Byte(d1[0]);
		} else if ((NT == HDFConstants.DFNT_UINT8 )
		 || (NT == HDFConstants.DFNT_UCHAR8 )
		 || (NT == HDFConstants.DFNT_UCHAR8 )
			) {
			Byte f = new Byte(d1[0]);
			Short fmx;
			if (f.shortValue() < 0) {
				theFillValue[0] = (Object)new Short((short)(f.intValue() + 256));
			} else {
			       theFillValue[0] = (Object)new Short(f.shortValue());
			}
		} else if ((NT == HDFConstants.DFNT_INT16 ) 
		 || (NT == HDFConstants.DFNT_CHAR16 )
			) {
			short [] fx = convert.byteToShort(0,1,d1);
			theFillValue[0] = (Object)new Short(fx[0]);
		 } else if ( (NT == HDFConstants.DFNT_UINT16 )
		 || (NT == HDFConstants.DFNT_UCHAR16 )
			) {
			short[] fmx = convert.byteToShort(0,1,d1);
			Short f = new Short(fmx[0]);
			Integer i;
			if (f.intValue() < 0) {
				theFillValue[0] = (Object)new Integer(f.intValue() + 165536);
			} else {
				theFillValue[0] = (Object)new Integer(f.intValue());
			}
		} else if ((NT == HDFConstants.DFNT_INT32 ) 
			) {
			int [] fx = convert.byteToInt(0,1,d1);
			theFillValue[0] = (Object)new Integer(fx[0]);
		} else if ((NT == HDFConstants.DFNT_UINT32 ) 
			) {
			int[] fmx = convert.byteToInt(0,1,d1);
			Integer i = new Integer(fmx[0]);
			Float f;
		if (i.floatValue() < 0) {
				theFillValue[0] = (Object)new Float((float)(i.floatValue() + 4294967295.0));
			} else {
				theFillValue[0] = (Object)new Float(i.floatValue());
			}
		} else if (NT == HDFConstants.DFNT_FLOAT32 ) {
			float [] fx = convert.byteToFloat(0,1,d1);
			theFillValue[0] = (Object)new Float(fx[0]);
		} else if (NT == HDFConstants.DFNT_FLOAT64 ) {
			double [] fx = convert.byteToDouble(0,1,d1);
			theFillValue[0] = (Object)new Double(fx[0]);
		} else {
			System.out.println("Error: DFSDgetfillvalue not converting, type "+NT);
		}
		return rval;
	}
 
     public native boolean DFSDgetNT(int[] data_type) throws HDFException;
 
     public native boolean DFSDgetrange(byte[] max, byte[] min) throws HDFException;

     public boolean  DFSDgetrange( double maxmin[]) throws HDFException {
	int NT;
        int[] nt = new int[1];
        DFSDgetNT(nt);
	NT = nt[0];
		HDFNativeData convert = new HDFNativeData();
		byte[] d1 = new byte[8];
		byte[] d2 = new byte[8];
		boolean rval;
		rval = DFSDgetrange( d1, d2);
		if ((NT == HDFConstants.DFNT_INT8 ) 
                 || (NT == HDFConstants.DFNT_CHAR8 )
                 || (NT == HDFConstants.DFNT_CHAR )
                        ) {
                        Byte f = new Byte(d1[0]);
                        maxmin[0] = (f.doubleValue());
                        f = new Byte(d2[0]);
                        maxmin[1] = (f.doubleValue());
                } else if ((NT == HDFConstants.DFNT_UINT8 ) 
                 || (NT == HDFConstants.DFNT_UCHAR8 )
                 || (NT == HDFConstants.DFNT_UCHAR8 )
                        ) {
                        Byte f = new Byte(d1[0]);
                        Short fmx;
                        if (f.shortValue() < 0) {
                                fmx = new Short((short)(f.intValue() + 256));
                        } else {
                                fmx = new Short(f.shortValue());
                        }
                        maxmin[0] = (fmx.doubleValue());
                        f = new Byte(d2[0]);
                        fmx = new Short(f.shortValue());
                        maxmin[1] = (fmx.doubleValue());
                } else if ((NT == HDFConstants.DFNT_INT16 ) 
                 || (NT == HDFConstants.DFNT_CHAR16 )
                        ) {
                        short [] fmx = convert.byteToShort(0,1,d1);
                        short [] fmn = convert.byteToShort(0,1,d2);
                        Short f = new Short(fmx[0]);
                        maxmin[0] = (f.doubleValue());
                        f = new Short(fmn[0]);
                        maxmin[1] = (f.doubleValue());
                } else if ((NT == HDFConstants.DFNT_UINT16 ) 
                 || (NT == HDFConstants.DFNT_UINT16 )
                        ) {
                        short[] fmx = convert.byteToShort(0,1,d1);
                        Short f = new Short(fmx[0]);
                        Integer i;
                        if (f.intValue() < 0) {
                                i = new Integer(f.intValue() + 165536);
                        } else {
                                i = new Integer(f.intValue());
                        }
                        maxmin[0] = (i.doubleValue());
                        fmx = convert.byteToShort(0,1,d2);
                        f = new Short(fmx[0]);
                        if (f.intValue() < 0) {
                                i = new Integer(f.intValue() + 165536);
                        } else {
                                i = new Integer(f.intValue()); 
			}
                        maxmin[0] = (i.doubleValue());
                        fmx = convert.byteToShort(0,1,d2);
                        f = new Short(fmx[0]);
                        if (f.intValue() < 0) {
                                i = new Integer(f.intValue() + 165536);
                        } else {
                                i = new Integer(f.intValue());
                        }
                        maxmin[1] = (i.doubleValue());
                } else if ((NT == HDFConstants.DFNT_INT32 ) ) {
                        int [] fmx = convert.byteToInt(0,1,d1);
                        int [] fmn = convert.byteToInt(0,1,d2);
                        Integer f = new Integer(fmx[0]);
                        maxmin[0] = (f.doubleValue());
                        f = new Integer(fmn[0]);
                        maxmin[1] = (f.doubleValue());
                } else if ( (NT == HDFConstants.DFNT_UINT32 )) {
                        int[] fmx = convert.byteToInt(0,1,d1);
                        Integer i = new Integer(fmx[0]);
                        Float f;
                        if (i.floatValue() < 0) {
                                f = new Float((float)(i.floatValue() + 4294967295.0));
                        } else {
                                f = new Float(i.floatValue());
                        }
                        maxmin[0] = (f.doubleValue());
                        fmx = convert.byteToInt(0,1,d1);
                        i = new Integer(fmx[0]);
                        if (i.floatValue() < 0) {
                                f = new Float((float)(i.floatValue() + 4294967295.0));
                        } else {
                                f = new Float(i.floatValue());
                        }
                        maxmin[1] = (f.doubleValue());
                } else if (NT == HDFConstants.DFNT_FLOAT32 ) {
                        float [] fmx = convert.byteToFloat(0,1,d1);
                        float [] fmn = convert.byteToFloat(0,1,d2);
                        Float f = new Float(fmx[0]);
                        maxmin[0] = (f.doubleValue());
                        f = new Float(fmn[0]);
                        maxmin[1] = (f.doubleValue());
                } else if (NT == HDFConstants.DFNT_FLOAT64 ) {
                        double [] fmx = convert.byteToDouble(0,1,d1);
                        double [] fmn = convert.byteToDouble(0,1,d2);
                        Double f = new Double(fmx[0]);
                        maxmin[0] = (f.doubleValue());
			f = new Double(fmn[0]);
                        maxmin[1] = (f.doubleValue());

		} else {
			System.out.println("Error: DFSDgetrange not converting, type "+NT);
		}
			return rval;
	}
 
     public native boolean DFSDgetslice(String filename, int [] winst, int [] windims, 
 	byte [] data, int [] dims) throws HDFException;
 
     public boolean DFSDgetslice(String filename, int [] winst, int [] windims, 
 	Object theData, int [] dims) throws HDFException{
 		byte[] data;
                 boolean rval;
 
                 HDFArray theArray = new HDFArray(theData);
                 data = theArray.emptyBytes();
 		rval = DFSDgetslice(filename, winst, windims, data, dims);
                 theData = theArray.arrayify( data );
                 return rval;
         }
 
     public native int DFSDlastref( ) throws HDFException;
 
 
     public native int DFSDndatasets(String filename) throws HDFException;
 
 
     public boolean DFSDpre32sdg(String filename, short ref, boolean ispre32)
 	throws HDFException {
		int []iv = new int[1];
 		boolean rval;
 		rval = DFSDpre32sdg(filename, ref, iv);
 		if (iv[0] == 0) {
 			ispre32 = false;
 		} else {
 			ispre32 = true;
 		}
 		return rval;
 	}
 
     public native boolean DFSDpre32sdg(String filename, short ref, int[] ispre32) throws HDFException;
 
     public native boolean DFSDputdata(String filename, int rank, int [] dimsizes, byte [] data) throws HDFException;
 
     public boolean DFSDputdata(String filename, int rank, int [] dimsizes, Object theData)  throws HDFException{
 		byte[] data;
                 int rval;
 
                 HDFArray theArray = new HDFArray(theData);
                 data = theArray.byteify();
                 return DFSDputdata(filename, rank, dimsizes, data);
         }
 
 
     public native boolean DFSDputslice(int [] windims, byte [] source, int [] dims) throws HDFException;
 
     public boolean DFSDputslice(int [] windims, Object source, int [] dims)  throws HDFException{
 		byte[] data;
                 int rval;
 
                 HDFArray theArray = new HDFArray(source);
                 data = theArray.byteify();
                 return DFSDputslice(windims, data, dims);
         }
 
 
     public native boolean DFSDreadref(String filename, short ref) throws HDFException;
 
     public native boolean DFSDreadslab(String filename, int [] start, int [] slab_size, 
 		int [] stride, byte [] buffer, int [] buffer_size) throws HDFException;
 
     public boolean DFSDreadslab(String filename, int [] start, int [] slab_size, 
 		int [] stride, Object theData, int [] buffer_size)
 	throws HDFException {
 		byte[] data;
                 int rval;
 
                 HDFArray theArray = new HDFArray(theData);
                 data = theArray.byteify();
                 return DFSDreadslab(filename, start, slab_size, stride, data, buffer_size);
         }
 
 
     public native boolean DFSDrestart( ) throws HDFException;
 
     public native boolean DFSDsetcal(double cal, double cal_err, double offset, double offset_err, 
 		int data_type) throws HDFException ;
 
     public native boolean DFSDsetdatastrs(String label, String unit, String format, String coordsys) throws HDFException;
 
 
     public native boolean DFSDsetdims (int rank, int [] dimsizes) throws HDFException;
 
     public native boolean DFSDsetdimscale (int dim, int dimsize, byte[] scale) throws HDFException;
 
     public boolean DFSDsetdimscale (int dim, int dimsize, Object theScale) throws HDFException {
 		byte[] data;
                 int rval;
 
                 HDFArray theArray = new HDFArray(theScale);
                 data = theArray.byteify();
                 return DFSDsetdimscale (dim, dimsize, data);
 	}
 
     public native boolean DFSDsetdimstrs(int dim, String label, String unit, String format) throws HDFException;
 
     public native boolean DFSDsetfillvalue(Object fill_value) throws HDFException;
 
     public native boolean DFSDsetlengths(int label_len, int unit_len, int format_len, 
 		int coords_len) throws HDFException ;
 
     public native boolean DFSDsetNT(int data_type) throws HDFException;

	public native boolean  DFSDsetrange( byte[] max, byte[] min) throws HDFException;

    public boolean  DFSDsetrange( Object max, Object min) throws HDFException {
                byte[] d1 = null;
                byte[] d2 = null;
                boolean rval;
		Class mincl = min.getClass();
		String mncn = mincl.getName();
		Class nc = mincl.getSuperclass();
		String s = nc.getName();
		if ( s.equals("java.lang.Number") == false) {
			/* exception:  bad argument */
			return(false);
		}
		Class maxcl = max.getClass();
		String mxcn = maxcl.getName();
		nc = maxcl.getSuperclass();
		s = nc.getName();
		if ( s.equals("java.lang.Number") == false) {
			/* exception:  bad argument */
			return(false);
		}
		if (mncn.equals(mxcn) == false) {
			/* exception:   argument don't agree */
			return(false);
		}
		System.out.println("DFSDsetrange: min,max are sub-class of Number ");
		if (mncn.equals("java.lang.Integer")) {
			Integer fmx = (Integer)max;
			Integer fmn = (Integer)min;
			d1 = ncsa.hdf.hdflib.HDFNativeData.intToByte(fmx.intValue());
			d2 = ncsa.hdf.hdflib.HDFNativeData.intToByte(fmn.intValue());
		} else if (mncn.equals("java.lang.Float")) {
			Float fmx = (Float)max;
			Float fmn = (Float)min;
			d1 = ncsa.hdf.hdflib.HDFNativeData.floatToByte(fmx.floatValue());
			d2 = ncsa.hdf.hdflib.HDFNativeData.floatToByte(fmn.floatValue());
		} else if (mncn.equals("java.lang.Double")) {
			Double fmx = (Double)max;
			Double fmn = (Double)min;
			d1 = ncsa.hdf.hdflib.HDFNativeData.doubleToByte(fmx.doubleValue());
			d2 = ncsa.hdf.hdflib.HDFNativeData.doubleToByte(fmn.doubleValue());
		} else if (mncn.equals("java.lang.Short")) {
			Short fmx = (Short)max;
			Short fmn = (Short)min;
			d1 = ncsa.hdf.hdflib.HDFNativeData.shortToByte(fmx.shortValue());
			d2 = ncsa.hdf.hdflib.HDFNativeData.shortToByte(fmn.shortValue());
		} else if (mncn.equals("java.lang.Byte")) {
			Byte fmx = (Byte)max;
			Byte fmn = (Byte)min;
			d1 = ncsa.hdf.hdflib.HDFNativeData.byteToByte(fmx.byteValue());
			d2 = ncsa.hdf.hdflib.HDFNativeData.byteToByte(fmn.byteValue());
		} else {
			/* exception:  unsupported type */
			return(false);
		}
			
                return  DFSDsetrange( d1, d2);
        }

 
     public native boolean DFSDstartslab(String filename) throws HDFException;
 
     public native boolean DFSDstartslice(String filename) throws HDFException;
 
     public native boolean DFSDwriteref(String filename, short ref) throws HDFException;
 
     public native boolean DFSDwriteslab(int [] start, int [] stride, int[] count, 
 	byte[] data) throws HDFException;
 
     public boolean DFSDwriteslab(int [] start, int [] stride, int[] count, 
 	Object theData)  throws HDFException{
 		byte[] data;
                 int rval;
 
                 HDFArray theArray = new HDFArray(theData);
                 data = theArray.byteify();
                 return DFSDwriteslab(start, stride, count, data);
         }
 
 
	public native boolean DFUfptoimage(int hdim, int vdim,
                float max, float min, float[] hscale, float[] vscale,
                float[] data, byte[] palette,
                String outfile,
                int ct_method, int hres, int vres, int compress) throws HDFException;

    public native boolean Vclose(int file_id) throws HDFException;

    public native int Vopen(String filename, int access, short ndds) throws HDFException;

    public native void VSdump(int vkey) throws HDFException;

  /* not implemented */
//VOID VSfpack(int32 vdata_id, intn action, char
//*fields_in_buf, VOIDP buf, intn buf_size, intn
//n_records, char *fields, VOIDP bufptrs[])

    public native void HEprint(OutputStream stream, int level) throws HDFException;
        /* not implemented */


}

