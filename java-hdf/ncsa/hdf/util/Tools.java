
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
     *  Sort an array of integers with insert sort
     */
    public static void sort(int a[])
    {
        for (int i = 1; i < a.length; i++)
        {
            int j = i;
            int B = a[i];
            while ((j > 0) && (a[j-1] > B))
            {
                a[j] = a[j-1];
                j--;
            }
            a[j] = B;
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

}
