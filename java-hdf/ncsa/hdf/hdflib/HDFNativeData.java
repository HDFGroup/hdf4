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

public class HDFNativeData
{

    static { System.loadLibrary("hdf"); }

    public HDFNativeData() {}
    public static native int[] byteToInt( byte[] data );
    public static native float[] byteToFloat( byte[] data );
    public static native short[] byteToShort( byte[] data );
    public static native long[] byteToLong( byte[] data );
    public static native double[] byteToDouble( byte[] data );

    public static native int[] byteToInt( int start, int len, byte[] data );
    public static int byteToInt( byte[] data, int start)
    {
        int []ival = new int[1];
        ival = byteToInt(start,1,data);
        return(ival[0]);
    }

    public static native short[] byteToShort( int start, int len, byte[] data );
    public static short byteToShort( byte[] data, int start)
    {
        short []sval = new short[1];
        sval = byteToShort(start,1,data);
        return(sval[0]);
    }

    public static native float[] byteToFloat( int start, int len, byte[] data );
    public static float byteToFloat( byte[] data, int start)
    {
        float []fval = new float[1];
        fval = byteToFloat(start,1,data);
        return(fval[0]);
    }

    public static native long[] byteToLong( int start, int len, byte[] data );
    public static long byteToLong( byte[] data, int start)
    {
        long []lval = new long[1];
        lval = byteToLong(start,1,data);
        return(lval[0]);
    }

    public static native double[] byteToDouble( int start, int len, byte[] data );
    public static double byteToDouble( byte[] data, int start)
    {
        double []dval = new double[1];
        dval = byteToDouble(start,1,data);
        return(dval[0]);
    }

    public static native byte[] intToByte( int start, int len, int[] data);
    public static native byte[] shortToByte( int start, int len, short[] data);
    public static native byte[] floatToByte( int start, int len, float[] data);
    public static native byte[] longToByte( int start, int len, long[] data);
    public static native byte[] doubleToByte( int start, int len, double[] data);

    public static native byte[] byteToByte( byte data);
    static byte[] byteToByte( Byte data){return byteToByte(data.byteValue());}
    public static native byte[] intToByte( int data);
    static byte[] intToByte( Integer data){return intToByte(data.intValue());}
    public static native byte[] shortToByte(short data);
    static byte[] shortToByte( Short data){return shortToByte(data.shortValue());}
    public static native byte[] floatToByte( float data);
    static byte[] floatToByte( Float data){return floatToByte(data.floatValue());};
    public static native byte[] longToByte( long data);
    static byte[] longToByte(Long data){ return longToByte(data.longValue());}
    public static native byte[] doubleToByte( double data);
    static byte[] doubleToByte( Double data){return doubleToByte(data.doubleValue());}

    public Object byteToNumber( byte[] barray, Object obj)
        throws HDFException
    {
        Class theClass = obj.getClass();
        String type = theClass.getName();
        Object retobj = null;

        if (type.equals("java.lang.Integer")) {
            int[] i = ncsa.hdf.hdflib.HDFNativeData.byteToInt(0,1,barray);
            retobj = new Integer(i[0]);
        } else  if (type.equals("java.lang.Byte")) {
            retobj = new Byte(barray[0]);
        } else  if (type.equals("java.lang.Short")) {
            short[] f = ncsa.hdf.hdflib.HDFNativeData.byteToShort(0,1,barray);
            retobj = new Short(f[0]) ;
        } else  if (type.equals("java.lang.Float")) {
            float[] f = ncsa.hdf.hdflib.HDFNativeData.byteToFloat(0,1,barray);
            retobj = new Float(f[0]) ;
        } else  if (type.equals("java.lang.Long")) {
            long[] f = ncsa.hdf.hdflib.HDFNativeData.byteToLong(0,1,barray);
            retobj = new Long(f[0]) ;
        } else  if (type.equals("java.lang.Double")) {
            double[] f = ncsa.hdf.hdflib.HDFNativeData.byteToDouble(0,1,barray);
            retobj = new Double(f[0] );
        } else {
            /* exception: unsupprted type */
            HDFException ex =
            (HDFException)new HDFJavaException("byteToNumber: setfield bad type: "+obj+" "+type);
            throw(ex);
        }
        return(retobj);
    }

    /**
     *  convert number type to string type
     *  params type  the number representing the data type
     *  return the string representing the data type
     */
    public static String getType(int type)
    {
        if   (type == HDFConstants.DFNT_UCHAR8) return HDFConstants.UCHAR8;
        else if (type == HDFConstants.DFNT_CHAR8) return HDFConstants.CHAR8;
        else if (type == HDFConstants.DFNT_UCHAR16) return HDFConstants.UCHAR16;
        else if (type == HDFConstants.DFNT_CHAR16) return HDFConstants.CHAR16;
        else if (type == HDFConstants.DFNT_FLOAT32) return HDFConstants.FLOAT32;
        else if (type == HDFConstants.DFNT_FLOAT64) return HDFConstants.FLOAT64;
        else if (type == HDFConstants.DFNT_FLOAT128) return HDFConstants.FLOAT128;
        else if (type == HDFConstants.DFNT_INT8) return HDFConstants.INT8;
        else if (type == HDFConstants. DFNT_UINT8) return HDFConstants.UINT8;
        else if (type == HDFConstants.DFNT_INT16) return HDFConstants.INT16;
        else if (type == HDFConstants.DFNT_UINT16) return HDFConstants.UINT16;
        else if (type == HDFConstants.DFNT_INT32) return HDFConstants.INT32;
        else if (type == HDFConstants.DFNT_UINT32) return HDFConstants.UINT32;
        else if (type == HDFConstants.DFNT_INT64) return HDFConstants.INT64;
        else if (type == HDFConstants.DFNT_UINT64) return HDFConstants.UINT64;
        else if (type == HDFConstants.DFNT_INT128) return HDFConstants.INT128;
        else if (type == HDFConstants.DFNT_UINT128) return HDFConstants.UINT128;
        else return "Undefined Data Type";
    }

    /**
     *  convert string type to number type
     *  params type  the string representing the data type
     *  return the integer representing the data type
     */
    public static int getType(String type)
    {
        if   (type.equalsIgnoreCase(HDFConstants.UCHAR8)) return HDFConstants.DFNT_UCHAR8;
        else if (type.equalsIgnoreCase(HDFConstants.CHAR8)) return HDFConstants.DFNT_CHAR8;
        else if (type.equalsIgnoreCase(HDFConstants.UCHAR16)) return HDFConstants.DFNT_UCHAR16;
        else if (type.equalsIgnoreCase(HDFConstants.CHAR16)) return HDFConstants.DFNT_CHAR16;
        else if (type.equalsIgnoreCase(HDFConstants.FLOAT32)) return HDFConstants.DFNT_FLOAT32;
        else if (type.equalsIgnoreCase(HDFConstants.FLOAT64)) return HDFConstants.DFNT_FLOAT64;
        else if (type.equalsIgnoreCase(HDFConstants.FLOAT128)) return HDFConstants.DFNT_FLOAT128;
        else if (type.equalsIgnoreCase(HDFConstants.INT8)) return HDFConstants.DFNT_INT8;
        else if (type.equalsIgnoreCase(HDFConstants. UINT8)) return HDFConstants.DFNT_UINT8;
        else if (type.equalsIgnoreCase(HDFConstants.INT16)) return HDFConstants.DFNT_INT16;
        else if (type.equalsIgnoreCase(HDFConstants.UINT16)) return HDFConstants.DFNT_UINT16;
        else if (type.equalsIgnoreCase(HDFConstants.INT32)) return HDFConstants.DFNT_INT32;
        else if (type.equalsIgnoreCase(HDFConstants.UINT32)) return HDFConstants.DFNT_UINT32;
        else if (type.equalsIgnoreCase(HDFConstants.INT64)) return HDFConstants.DFNT_INT64;
        else if (type.equalsIgnoreCase(HDFConstants.UINT64)) return HDFConstants.DFNT_UINT64;
        else if (type.equalsIgnoreCase(HDFConstants.INT128)) return HDFConstants.DFNT_INT128;
        else if (type.equalsIgnoreCase(HDFConstants.UINT128)) return HDFConstants.DFNT_UINT128;
        else return -1;
    }

    /**
     *  define the data object
     *
     *  @param dataType  the type of the iamge data
     *  @param dataSize  the size of the image data array
     */
    public static Object defineDataObject(int dataType, int dataSize)
    {
        Object data = null;

        switch(dataType)
        {
            case HDFConstants.DFNT_CHAR:
            case HDFConstants.DFNT_INT8:
                data = new byte[dataSize];
                break;
            case HDFConstants.DFNT_UCHAR8:
            case HDFConstants.DFNT_UINT8:
            case HDFConstants.DFNT_INT16:
                data = new short[dataSize];
                break;
            case HDFConstants.DFNT_UINT16:
            case HDFConstants.DFNT_INT32:
                data = new int[dataSize];
                 break;
            case HDFConstants.DFNT_INT64:
            case HDFConstants.DFNT_UINT32:
                data = new long[dataSize];
                 break;
            case HDFConstants.DFNT_FLOAT32:
                data = new float[dataSize];
                 break;
            case HDFConstants.DFNT_DOUBLE:
                data = new double[dataSize];
                 break;
            default:
        }
        return data;
    }

}
