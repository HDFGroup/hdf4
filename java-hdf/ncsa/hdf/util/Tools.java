
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


/**
 *  A class contains static methods.
 *  For example,
 *  <pre>
 *    Toolkit.getKey() will return an unique string key.
 *  </pre>
 *
 *  @see                 java.lang.Object
 *  @version             1.0 31 March 1997
 *  @author              Peter X. Cao
 */

package ncsa.hdf.util;

import java.util.Date;
import java.lang.reflect.Array;

public class Tools
{

    public static boolean debug = false;

    /**
     *  return a unique string key. This method is synchronized.
     *  Only one thread calls it a time to make the key unique.
     *
     *  @return         The string key
     */
    public static synchronized String getKey()
    {
        /**
         *  the time will make the key unique for different running time.
         *  the hashCode will make the key unique at the same running time.
         *  the key is the string(time)+string(hashCode).
         */
        try { Thread.currentThread().sleep(5); }
        catch (InterruptedException ex) { System.out.println("ERROR in Toolkit::getKey(): "+ex); }

        long   time     = (new Date()).getTime();
        int    hashCode = (new Object()).hashCode();
        String timePart = Long.toString(time);
        String codePart = Integer.toString(hashCode);

        if (debug)
        {
            System.out.println("Toolkit::getKey(): timePart = "+timePart);
            System.out.println("Toolkit::getKey(): codePart = "+codePart);
        }

        return timePart+codePart;
    }

    static public void notFalse(boolean b) throws IllegalArgumentException
    {
        if(b == false) throw new IllegalArgumentException( "boolean expression false");
    }

    static public void notNull(Object obj) throws IllegalArgumentException
    {
        if(obj == null) throw new IllegalArgumentException("null argument");
    }

    static public void notFalse(boolean b, String s) throws IllegalArgumentException
    {
        if(b == false) throw new IllegalArgumentException(s);
    }

    static public void notNull(Object obj, String s) throws IllegalArgumentException
    {
        if(obj == null) throw new IllegalArgumentException(s);
    }

    /**
     *  Sort an array in ascending order with insert sort
     *  @param array the data array to be sorted
     */
    public static void sort(Object array)
    {
        if (!array.getClass().isArray())
            return;

        int length = Array.getLength(array);
        double d0=0, d1=0;
        Object theObject = null;

        for (int i = 1; i < length; i++)
        {
            int j = i;
            d0 = (double) Array.getDouble(array, i-1);
            d1 = (double) Array.getDouble(array, i);
            theObject = Array.get(array, i);
            while ((j > 0) && (d0 > d1))
            {
                Array.set(array, j, Array.get(array, j-1));
                j--;
                d0 = (double) Array.getDouble(array, j-1);
            }
            Array.set(array, j, theObject);
        }
    }


    /**
     *  Sort an array of strings with insert sort
     */
    public static void sort(String a[])
    {
        for (int i = 1; i < a.length; i++)
        {
            int j = i;
            String B = a[i];
            while ((j > 0) && (a[j-1].compareTo(B) > 0))
            {
                a[j] = a[j-1];
                j--;
            }
            a[j] = B;
        }
    }

    /**
     *  convert a double to a byte array
     *  @param data         The input double array
     *  @return             The byte array
     */
    public static byte[] double2byte(double data[])
    {
        if (data == null) return null;

        byte output[] = new byte[data.length];
        for (int i = 0; i < data.length; i++)
            output[i] = (byte) data[i];

        return output;
    }

    /**
     *  calculate the mean value of an array
     *
     *  @param data    The data array
     *  @return        The mean value
     */
    public static double mean(Object data)
    {
        if (data==null || !data.getClass().isArray())
            return Double.NaN;

        return mean(data, 0, Array.getLength(data)-1);
    }

    /**
     *  calculate the mean value of an array
     *
     *  @param data    The data array
     *  @param low     The starting index
     *  @param high    The ending index
     *  @return        The mean value
     */
    public static double mean(Object data, int low, int high)
    {
        if (data==null || !data.getClass().isArray())
            return Double.NaN;

        double sum = 0;

        for (int i=low; i<=high; i++)
            sum += Array.getDouble(data,i);

        return sum/(high-low+1);
    }

    /**
     *  calculate the variance of an array
     *
     *  @param data    The data array
     *  @return        The variance
     */
    public static double var(Object data)
    {
        if (data==null || !data.getClass().isArray())
            return Double.NaN;

        return var(data, 0, Array.getLength(data)-1);
    }

    /**
     *  calculate the variance of an array
     *
     *  @param data    The data array
     *  @param low     The starting index
     *  @param high    The ending index
     *  @return        The variance
     */
    public static double var(Object data, int low, int high)
    {
        if (data==null || !data.getClass().isArray())
            return Double.NaN;

        double mean = mean(data, low, high);
        double sum = 0;
        double tmp = 0;

        for (int i=low; i<=high; i++)
        {
            tmp = Array.getDouble(data,i);
            sum += (tmp-mean)*(tmp-mean);
        }

        return sum/(high-low);
    }

    /** for testing purpose only */
    public static void main(String[] argv)
    {
        /*
        // test sorting
        int length = 6;
        int[]   intArray = {11, 62, 43, 34, 75, 26};
        double[] floatArray = {1.1, 6.2, 4.3, 3.4, 7.5, 2.6};
        sort((Object) intArray);
        sort((Object)floatArray);
        System.out.print("\n\nintArray = ");
        for (int i=0; i<length; i++ )
        {
            System.out.print(intArray[i]+"  ");
        }
        System.out.print("\n\nfloatArray = ");
        for (int i=0; i<length; i++ )
        {
            System.out.print(floatArray[i]+"  ");
        }
        */

        /*
        // test getRange()
        byte[] bdata = {1, 2, 3, 4, 5};
        short[] sdata = {1, 2, 3, 4, 5};
        int[] idata = {1, 2, 3, 4, 5};
        long[] ldata = {1, 2, 3, 4, 5};
        float[] fdata = {(float)1.1, (float)1.2, (float)1.3, (float)1.4, (float)1.5};
        double[] ddata = {1.1, 1.2, 1.3, 1.4, 1.5};
        getRange(bdata);
        getRange(sdata);
        getRange(idata);
        getRange(ldata);
        getRange(fdata);
        getRange(ddata);
        */
    }

    /**
     *  find the maximum and the minimum in a double array
     *
     *  @param data    The input double array
     *  @return        The max and min data
     */
    public static double[] getRange(double data[])
    {
        if (data == null) return null;
        else return getRange(data, 0, data.length-1);
    }

    /**
     *  find the maximum and the minimum in a float array
     *
     *  @param data    The input float array
     *  @return        The max and min data
     */
    public static float[] getRange(float data[])
    {
        if (data == null) return null;
        else return getRange(data, 0, data.length-1);
    }

    /**
     *  find the maximum and the minimum in a long array
     *
     *  @param data    The input long array
     *  @return        The max and min data
     */
    public static long[] getRange(long data[])
    {
        if (data == null) return null;
        else return getRange(data, 0, data.length-1);
    }

    /**
     *  find the maximum and the minimum in a int array
     *
     *  @param data    The input int array
     *  @return        The max and min data
     */
    public static int[] getRange(int data[])
    {
        if (data == null) return null;
        else return getRange(data, 0, data.length-1);
    }

    /**
     *  find the maximum and the minimum in a byte array
     *
     *  @param data    The input byte array
     *  @return        The max and min data
     */
    public static byte[] getRange(byte data[])
    {
        if (data == null) return null;
        else return getRange(data, 0, data.length-1);
    }

    /**
     *  find the maximum and the minimum in a short array
     *
     *  @param data    The input byte array
     *  @return        The max and min data
     */
    public static short[] getRange(short data[])
    {
        if (data == null) return null;
        else return getRange(data, 0, data.length-1);
    }
     
    /**
     *  find the maximum and the minimum in a double array
     *
     *  @param data    The input double array
     *  @param low     The lower index
     *  @param high    The higher index
     *  @return        The max and min data
     */
    public static double[] getRange(double data[], int low, int high)
    {
        if (data == null) return null;

        double max = Double.MIN_VALUE;
        double min = Double.MAX_VALUE;
        double range[] = new double[2];
    
        for (int i=low; i<=high; i++)
        {
            if (data[i] > max) max = data[i];
            if (data[i] < min) min = data[i];
        }
        range[0] = min;
        range[1] = max;
        return range;
    }

    /**
     *  find the maximum and the minimum in a float array
     *
     *  @param data    The input float array
     *  @param low     The lower index
     *  @param high    The higher index
     *  @return        The max and min data
     */
    public static float[] getRange(float data[], int low, int high)
    {
        if (data == null) return null;

        float max = Float.MIN_VALUE;
        float min = Float.MAX_VALUE;
        float range[] = new float[2];
    
        for (int i=low; i<=high; i++)
        {
            if (data[i] > max) max = data[i];
            if (data[i] < min) min = data[i];
        }
        range[0] = min;
        range[1] = max;
        return range;
    }

    /**
     *  find the maximum and the minimum in a long array
     *
     *  @param data    The input long array
     *  @param low     The lower index
     *  @param high    The higher index
     *  @return        The max and min data
     */
    public static long[] getRange(long data[], int low, int high)
    {
        if (data == null) return null;

        long max = Long.MIN_VALUE;
        long min = Long.MAX_VALUE;
        long range[] = new long[2];
    
        for (int i=low; i<=high; i++)
        {
            if (data[i] > max) max = data[i];
            if (data[i] < min) min = data[i];
        }
        range[0] = min;
        range[1] = max;
        return range;
    }

    /**
     *  find the maximum and the minimum in a int array
     *
     *  @param data    The input int array
     *  @param low     The lower index
     *  @param high    The higher index
     *  @return        The max and min data
     */
    public static int[] getRange(int data[], int low, int high)
    {
        if (data == null) return null;

        int max = Integer.MIN_VALUE;
        int min = Integer.MAX_VALUE;
        int range[] = new int[2];
    
        for (int i=low; i<=high; i++)
        {
            if (data[i] > max) max = data[i];
            if (data[i] < min) min = data[i];
        }
        range[0] = min;
        range[1] = max;
        return range;
    }

    /**
     *  find the maximum and the minimum in a byte array
     *
     *  @param data    The input byte array
     *  @param low     The lower index
     *  @param high    The higher index
     *  @return        The max and min data
     */
    public static byte[] getRange(byte data[], int low, int high)
    {
        if (data == null) return null;

        byte max = Byte.MIN_VALUE;
        byte min = Byte.MAX_VALUE;
        byte range[] = new byte[2];
    
        for (int i=low; i<=high; i++)
        {
            if (data[i] > max) max = data[i];
            if (data[i] < min) min = data[i];
        }
        range[0] = min;
        range[1] = max;
        return range;
    }

    /**
     *  find the maximum and the minimum in a short array
     *
     *  @param data    The input byte array
     *  @param low     The lower index
     *  @param high    The higher index
     *  @return        The max and min data
     */
    public static short[] getRange(short data[], int low, int high)
    {
        if (data == null) return null;

        short max = Short.MIN_VALUE;
        short min = Short.MAX_VALUE;
        short range[] = new short[2];
    
        for (int i=low; i<=high; i++)
        {
            if (data[i] > max) max = data[i];
            if (data[i] < min) min = data[i];
        }
        range[0] = min;
        range[1] = max;
        return range;
    }

    /**
     *  find the maximum and the minimum of an array
     *
     *  @param data    The data array
     *  @return        The max and min data
     */
    public static Object getRange(Object data)
    {
        if (data==null || !data.getClass().isArray())
            return null;

        return getRange(data, 0, Array.getLength(data)-1);
    }

    /**
     *  find the maximum and the minimum of an array
     *  // does not work correctly for irix machines
     *
     *  @param data    The data array
     *  @param low     The starting index
     *  @param high    The ending index
     *  @return        The max and min data
     */
    public static Object getRange(Object data, int low, int high)
    {
        //System.out.println("Tools.getRange()");

        if (data==null || !data.getClass().isArray())
            return null;

        double max = Double.MIN_VALUE;
        double min = Double.MAX_VALUE;
        double tmpDouble = 0;
        Object range = null;

        for (int i=low; i<=high; i++)
        {
            tmpDouble = Array.getDouble(data,i);
            if (tmpDouble > max) max = tmpDouble;
            if (tmpDouble < min) min = tmpDouble;
        }

        String s = data.getClass().toString();
        int index = s.indexOf("[")+1;
        char type = s.charAt(index);

        switch(type)
        {
            case 'B':
                byte[] bdata = {(byte)min, (byte)max};
                range = bdata;
                break;
            case 'S':
                short[] sdata = {(short)min, (short)max};
                range = sdata;
                break;
            case 'I':
                int[] idata = {(int)min, (int)max};
                range = idata;
                break;
            case 'F':
                float[] fdata = {(float)min, (float)max};
                range = fdata;
                break;
            case 'J':
                long[] ldata = {(long)min, (long)max};
                range = ldata;
                break;
            case 'D':
                double[] ddata = {min, max};
                range = ddata;
                break;
            default:
        }
        return range;
    }

}
