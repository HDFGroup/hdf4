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

/**
 *  This is a class for handling multidimensional arrays for
 *  HDF.
 *  <p>
 *  The purpose is to allow the storage and retrieval of
 *  arbitrary array types containing scientific data.
 *  <p>
 *  The methods support the conversion of an array to and
 *  from Java to a one-dimensional array of bytes suitable
 *  for I/O by the C library.
 *  <p>
 *  This class heavily uses the <a href="./ncsa.hdf.hdflib.HDFNativeData.html">HDFNativeData</a>
 *  class to convert between Java and C representations.
 */

public class HDFArray {

Object _theArray = null;
ArrayDescriptor _desc = null;
byte [] _barray = null;

public HDFArray() {
}

public HDFArray(Object anArray) throws HDFException {

	Class tc = anArray.getClass();
        if (tc.isArray() == false) {
                /* exception: not an array */
                System.out.println("Error:  HDFArray requires array");
 HDFException ex =
		(HDFException)new HDFJavaException("HDFArray: not an array?: ");
ex.printStackTrace();
                        throw(ex);
                //return;
        }
	_theArray = anArray;
	_desc = new ArrayDescriptor( _theArray );
}

/**
 *  allocate a one-dimensional array of bytes sufficient to store
 *  the array.
 */

public byte[] emptyBytes(){
	return (new byte[_desc.totalSize]);
}

/**
 *  Given a Java array of numbers, convert it to a one-dimensional
 *  array of bytes in correct native order.
 */
public byte[] byteify() throws HDFException{

	if (_barray != null) return _barray;
	if (_theArray == null) {
                /* exception: not an array */
                System.out.println("Error:  byteify requires array");
 HDFException ex =
		(HDFException)new HDFJavaException("HDFArray: byteify not an array?: ");
ex.printStackTrace();
                        throw(ex);

                //return null;
        }

	if (_desc.dims == 1) {
		/* special case */
		if (_desc.NT == 'B') {
			/* really special case! */
			_barray = (byte [])_theArray;
			return _barray;
		} else {
			_barray = new byte[_desc.totalSize];

			byte [] therow;
			if (_desc.NT == 'I') {
				therow = ncsa.hdf.hdflib.HDFNativeData.intToByte(0,_desc.dimlen[1],(int [])_theArray);
			} else if (_desc.NT == 'S') {
				therow = ncsa.hdf.hdflib.HDFNativeData.shortToByte(0,_desc.dimlen[1],(short [])_theArray);
			} else if (_desc.NT == 'F') {
				therow = ncsa.hdf.hdflib.HDFNativeData.floatToByte(0,_desc.dimlen[1],(float [])_theArray);
			} else if (_desc.NT == 'J') {
				therow = ncsa.hdf.hdflib.HDFNativeData.longToByte(0,_desc.dimlen[1],(long [])_theArray);
			} else if (_desc.NT == 'D') {
				therow = ncsa.hdf.hdflib.HDFNativeData.doubleToByte(0,_desc.dimlen[1],(double [])_theArray);
			} else {
				System.out.println("Exception: unknown type or not implemented yet...");
				return null;
			}
			System.arraycopy(therow,0,_barray,0,(_desc.dimlen[1] * _desc.NTsize));
			return _barray;
		}
	}

	_barray = new byte[_desc.totalSize];

	Object oo = _theArray;
	int n = 0;  /* the current byte */
	int index = 0;
	int i;
	while ( n < _desc.totalSize ) {
		oo = _desc.objs[0];
		index = n / _desc.bytetoindex[0];
                index %= _desc.dimlen[0];
		for (i = 0 ; i < (_desc.dims); i++) {
			index = n / _desc.bytetoindex[i];
			index %= _desc.dimlen[i];

			if (index == _desc.currentindex[i]) {
				/* then use cached copy */
				oo = _desc.objs[i];
			} else {
				/* check range of index */		
				if (index > (_desc.dimlen[i] - 1)) {
					System.out.println("out of bounds?");
					return null;
				}
				oo = java.lang.reflect.Array.get((Object) oo,index);
				_desc.currentindex[i] = index;
				_desc.objs[i] = oo;
			}
		}

		/* byte-ify */
		byte arow[];
		if (_desc.NT == 'J') {
			arow = ncsa.hdf.hdflib.HDFNativeData.longToByte(0,_desc.dimlen[_desc.dims],(long [])_desc.objs[_desc.dims - 1]);
		} else if (_desc.NT == 'I') {
			arow = ncsa.hdf.hdflib.HDFNativeData.intToByte(0,_desc.dimlen[_desc.dims],(int [])_desc.objs[_desc.dims - 1]);
		} else if (_desc.NT == 'S') {
			arow = ncsa.hdf.hdflib.HDFNativeData.shortToByte(0,_desc.dimlen[_desc.dims],(short [])_desc.objs[_desc.dims - 1]);
		} else if (_desc.NT == 'B') {
			arow = (byte [])_desc.objs[_desc.dims - 1];
		} else if (_desc.NT == 'F') {
			/* 32 bit float */
			arow = ncsa.hdf.hdflib.HDFNativeData.floatToByte(0,_desc.dimlen[_desc.dims],(float [])_desc.objs[_desc.dims - 1]);
		} else if (_desc.NT == 'D') {
			/* 64 bit float */
			arow = ncsa.hdf.hdflib.HDFNativeData.doubleToByte(0,_desc.dimlen[_desc.dims],(double [])_desc.objs[_desc.dims - 1]);
		} else {
			System.out.println("Exception:  unknown or unsupported type: "+_desc.NT);
			return null;
		}
		System.arraycopy(arow,0,_barray,n,(_desc.dimlen[_desc.dims] * _desc.NTsize));
		n += _desc.bytetoindex[_desc.dims - 1];
	}
	/* assert:  the whole array is completed--currentindex should == len - 1 */

	/* error checks */

	if (n < _desc.totalSize) {
		System.out.println("Panic didn't complete all input data: n=  "+n+" size = "+_desc.totalSize);
	}
	for (i = 0;i < _desc.dims; i++) {
		if (_desc.currentindex[i] != _desc.dimlen[i] - 1) {
			System.out.println("Panic didn't complete all data: currentindex["+i+"] = "+_desc.currentindex[i]+" (should be "+(_desc.dimlen[i] - 1)+" ?)");
		}
	}
	return _barray;
}



/**
 *  Given a one-dimensional array of numbers, convert it to a java
 *  array of the shape and size passed to the constructor.
 */
public Object arrayify(byte[] bytes) throws HDFException {

	if (_theArray == null) {
                /* exception: not an array */
                System.out.println("Error:  arrayify requires array");
 HDFException ex =
		(HDFException)new HDFJavaException("arrayify: not an array?: ");
ex.printStackTrace();
                        throw(ex);
                //return null;
        }

	if (java.lang.reflect.Array.getLength((Object) bytes) != _desc.totalSize) {
                /* exception: array not right size */
                System.out.println("Error:  wrong size array");
                return null;
        }
	_barray = bytes; /* hope that the bytes are correct.... */

	if (_desc.dims == 1) {
		/* special case */
		/* 2 data copies here! */
		if (_desc.NT == 'I') {
			int [] x = (int [])ncsa.hdf.hdflib.HDFNativeData.byteToInt(0,_desc.dimlen[1],_barray);
			System.arraycopy(x,0,_theArray,0,_desc.dimlen[1]);
			return _theArray;
		} else if (_desc.NT == 'S') {
			short [] x = ncsa.hdf.hdflib.HDFNativeData.byteToShort(0,_desc.dimlen[1],_barray);
			System.arraycopy(x,0,_theArray,0,_desc.dimlen[1]);
			return _theArray;
		} else if (_desc.NT == 'F') {
			float x[] = ncsa.hdf.hdflib.HDFNativeData.byteToFloat(0,_desc.dimlen[1],_barray);
			System.arraycopy(x,0,_theArray,0,_desc.dimlen[1]);
			return _theArray;
		} else if (_desc.NT == 'J') {
			long x[] = ncsa.hdf.hdflib.HDFNativeData.byteToLong(0,_desc.dimlen[1],_barray);
			System.arraycopy(x,0,_theArray,0,_desc.dimlen[1]);
			return _theArray;
		} else if (_desc.NT == 'D') {
			double x[] = ncsa.hdf.hdflib.HDFNativeData.byteToDouble(0,_desc.dimlen[1],_barray);
			System.arraycopy(x,0,_theArray,0,_desc.dimlen[1]);
			return _theArray;
		} else if (_desc.NT == 'B') {
			System.arraycopy(_barray,0,_theArray,0,_desc.dimlen[1]);
			return _theArray;
		} else {
			System.out.println("Exception: unknown type or not implemented yet...");
			//_theArray = null;
			return null;
		}
	}
	/* Assert dims >= 2 */

	Object oo = _theArray;
	int n = 0;  /* the current byte */
	int index = 0;
	int i;
	while ( n < _desc.totalSize ) {
		oo = _desc.objs[0];
		index = n / _desc.bytetoindex[0];
		index %= _desc.dimlen[0];
		for (i = 0 ; i < (_desc.dims); i++) {
			index = n / _desc.bytetoindex[i];
			index %= _desc.dimlen[i];

			if (index == _desc.currentindex[i]) {
				/* then use cached copy */
				oo = _desc.objs[i];
			} else {
				/* check range of index */		
				if (index > (_desc.dimlen[i] - 1)) {
					System.out.println("out of bounds?");
					return null;
				}
				oo = java.lang.reflect.Array.get((Object) oo,index);
				_desc.currentindex[i] = index;
				_desc.objs[i] = oo;
			}
		}

		/* byte-ify */
		if (_desc.NT == 'J') {
			long [] arow = ncsa.hdf.hdflib.HDFNativeData.byteToLong(n,_desc.dimlen[_desc.dims],_barray);
			java.lang.reflect.Array.set(_desc.objs[_desc.dims - 2] ,
				(_desc.currentindex[_desc.dims - 1]), (Object)arow);
			n += _desc.bytetoindex[_desc.dims - 1];
			_desc.currentindex[_desc.dims - 1]++;
		} else if (_desc.NT == 'I') {
			int [] arow = ncsa.hdf.hdflib.HDFNativeData.byteToInt(n,_desc.dimlen[_desc.dims],_barray);
			java.lang.reflect.Array.set(_desc.objs[_desc.dims - 2] ,
				(_desc.currentindex[_desc.dims - 1]), (Object)arow);

			n += _desc.bytetoindex[_desc.dims - 1];
			_desc.currentindex[_desc.dims - 1]++;
		} else if (_desc.NT == 'S') {
			short [] arow = ncsa.hdf.hdflib.HDFNativeData.byteToShort(n,_desc.dimlen[_desc.dims],_barray);
			java.lang.reflect.Array.set(_desc.objs[_desc.dims - 2] ,
				(_desc.currentindex[_desc.dims - 1]), (Object)arow);

			n += _desc.bytetoindex[_desc.dims - 1];
			_desc.currentindex[_desc.dims - 1]++;
		} else if (_desc.NT == 'B') {
			System.arraycopy( _barray, n, _desc.objs[_desc.dims - 1], 0, _desc.dimlen[_desc.dims]);
			n += _desc.bytetoindex[_desc.dims - 1];
		} else if (_desc.NT == 'F') {
			float arow[] = ncsa.hdf.hdflib.HDFNativeData.byteToFloat(n,_desc.dimlen[_desc.dims],_barray);
			java.lang.reflect.Array.set(_desc.objs[_desc.dims - 2] ,
				(_desc.currentindex[_desc.dims - 1]), (Object)arow);

			n += _desc.bytetoindex[_desc.dims - 1];
			_desc.currentindex[_desc.dims - 1]++;
		} else if (_desc.NT == 'D') {
			double [] arow = ncsa.hdf.hdflib.HDFNativeData.byteToDouble(n,_desc.dimlen[_desc.dims],_barray);
			java.lang.reflect.Array.set(_desc.objs[_desc.dims - 2] ,
				(_desc.currentindex[_desc.dims - 1]), (Object)arow);

			n += _desc.bytetoindex[_desc.dims - 1];
			_desc.currentindex[_desc.dims - 1]++;
		} else {
			System.out.println("Exception:  unknown or unsupported type: "+_desc.NT);
		}

	}

	/* assert:  the whole array is completed--currentindex should == len - 1 */

	/* error checks */

	if (n < _desc.totalSize) {
		System.out.println("Panic didn't complete all input data: n=  "+n+" size = "+_desc.totalSize);
	}
	for (i = 0;i <= _desc.dims-2; i++) {
		if (_desc.currentindex[i] != _desc.dimlen[i] - 1) {
			System.out.println("Panic didn't complete all data: currentindex["+i+"] = "+_desc.currentindex[i]+" (should be "+(_desc.dimlen[i] - 1)+"?");
		}
	}
	if (_desc.NT != 'B') {
	if (_desc.currentindex[_desc.dims - 1] != _desc.dimlen[_desc.dims - 1]) {
		System.out.println("Panic didn't complete all data: currentindex["+i+"] = "+_desc.currentindex[i]+" (should be "+(_desc.dimlen[i])+"?");
	}
	} else {
	if (_desc.currentindex[_desc.dims - 1] != (_desc.dimlen[_desc.dims - 1] - 1)) {
		System.out.println("Panic didn't complete all data: currentindex["+i+"] = "+_desc.currentindex[i]+" (should be "+(_desc.dimlen[i] - 1)+"?");
	}
	}

	return _theArray;
}

}


/**
  * This class is used by HDFArray to discover the shape and type of an
  * arbitrary array.
  */

class ArrayDescriptor {

	static String theType = "";
	static Class theClass = null;
        static int [] dimlen = null;
        static int [] dimstart = null;
        static int [] currentindex = null;
        static int [] bytetoindex = null;
	static int totalSize = 0;
        static Object [] objs = null;
	static char NT = ' ';  /*  must be B,S,I,L,F,D, else error */
        static int NTsize = 0;
	static int dims = 0;

	public ArrayDescriptor ( Object anArray ) throws HDFException {

		Class tc = anArray.getClass();
		if (tc.isArray() == false) {
			/* exception: not an array */
			System.out.println("Error:  arraydescriptor requires array");
 HDFException ex =
		(HDFException)new HDFJavaException("ArrayDescriptor: not an array?: ");
ex.printStackTrace();
                        throw(ex);
			//return;
		}

		theClass = tc;

		/* parse the type descriptor to discover the
			shape of the array */
		String ss = tc.toString();
		theType = ss;
		int n = 6;
		dims = 0;
		char c = ' ';
		while (n < ss.length()) {
			c = ss.charAt(n);
			n++;
			if (c == '[') {
				dims++;
			}
		}

		/* To do:  extend to deal with Integer, Short, etc. */
		NT = c;  /*  must be B,S,I,L,F,D, else error */
		if (NT == 'B') {
			NTsize = 1;
		} else if (NT == 'S') {
			NTsize = 2;
		} else if ((NT == 'I') || (NT == 'F')) {
			NTsize = 4;
		} else if ((NT == 'J') || (NT == 'D')){
			NTsize = 8;
		} else {
			/* exception:  not a numeric type */
			System.out.println("Error:  array is not numeric?");
			return;
		}

		/* fill in the table */
		dimlen = new int [dims+1];
		dimstart = new int [dims+1];
		currentindex = new int [dims+1];
		bytetoindex = new int [dims+1];
		objs = new Object [dims+1];

		Object o = anArray;
		objs[0] = o;
		dimlen[0]= 1;
		dimstart[0] = 0;
		currentindex[0] = 0;
		int i;
		for ( i = 1; i <= dims; i++) {
			dimlen[i]= java.lang.reflect.Array.getLength((Object) o);
			o = java.lang.reflect.Array.get((Object) o,0);
			objs [i] = o;
			dimstart[i] = 0;
			currentindex[i] = 0;
		}

		int j;
		int dd;
		bytetoindex[dims] = NTsize;
		for ( i = dims; i >= 0; i--) {
			dd = NTsize;
			for (j = i; j < dims; j++) {
				dd *= dimlen[j + 1];
			}
			bytetoindex[i] = dd;
		}

		totalSize = bytetoindex[0];
	}

	public void dumpInfo()
	{
		System.out.println("Type: "+theType);
		System.out.println("Class: "+theClass);
		System.out.println("NT: "+NT+" NTsize: "+NTsize);
		System.out.println("Array has "+dims+" dimensions ("+totalSize+" bytes)");
		int i;
		for (i = 0; i <= dims; i++) {
			Class tc = objs[i].getClass();
			String ss = tc.toString();
			System.out.println(i+":  start "+dimstart[i]+": len "+dimlen[i]+" current "+currentindex[i]+" bytetoindex "+bytetoindex[i]+" object "+objs[i]+" otype "+ss);
		}
	}


}
