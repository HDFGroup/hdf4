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

package ncsa.hdf.io;

import java.awt.Dimension;
import java.io.FileWriter;
import java.io.PrintWriter;
import java.io.IOException;
import ncsa.hdf.hdflib.HDFLibrary;
import ncsa.hdf.hdflib.HDFConstants;
import ncsa.hdf.hdflib.HDFException;
import ncsa.hdf.hdflib.HDFTable;
import ncsa.hdf.hdflib.HDFNativeData;

/**
 *  HDFWriter writes HDF Objects into an ASCII file

 *  @version   1.1.3 January 8, 1998
 *  @author    Peter X. Cao
 *  @see       java.io.PrintWriter
 */
public class ASCIIWriter extends PrintWriter
{
    /** array of carriage return and LF NL linefeed */
    public static char[] nl = {13, 10};

    /** the new charactor */
    public static String NL = new String(nl);

    /** delimiter */
    String delimiter = " ";

    public ASCIIWriter(String fileName) throws IOException
    {
        super(new FileWriter(fileName));
    }

    public ASCIIWriter(String fileName, boolean append) throws IOException
    {
        super(new FileWriter(fileName, append));
    }

    /** set delimiter */
    public void setDelimiter(String delimiter)
    { this.delimiter = delimiter; }

    /**
     *  write annotation into the ASCII file
     *
     *  @annotation  the file annotation
     */
    public void writeAnnotation(String annotation)
        throws IOException
    {
        println();
        println(HDFConstants.ANNOTATION);
        println(annotation);
        flush();
    }

    /**
     *  write 2D data
     *
     *  @info     text information about the data
     *  @d        the dimension of the data
     *  @dataType the type of data
     *  @data     the 2D data presented by 1D array
     */
    public void write2DData(String format, String name, int rank,
        Dimension d, int dataType, Object data)
        throws IOException
    {
        int w = d.width;
        int h = d.height;
        int maxLength = 3;

        // write data information
        println();
        println(format);
        println(name);
        if (format.equalsIgnoreCase(HDFConstants.SDS)) println(rank);
        println(h);
        println(w);
        println(HDFNativeData.getType(dataType));

        // write data
        String theStr = "";
        int padding = 0;
        for (int i=0; i<h; i++)
        {
            for (int j=0; j<w; j++)
            {
                switch(dataType)
                {
                    case HDFConstants.DFNT_CHAR:
                    case HDFConstants.DFNT_UCHAR8:
                    case HDFConstants.DFNT_UINT8:
                    case HDFConstants.DFNT_INT8:
                        theStr = String.valueOf(java.lang.reflect.Array.getByte(data,i*w+j));
                        break;
                    case HDFConstants.DFNT_INT16:
                    case HDFConstants.DFNT_UINT16:
                        maxLength = 6;
                        theStr = String.valueOf(java.lang.reflect.Array.getShort(data,i*w+j));
                        break;
                    case HDFConstants.DFNT_INT32:
                    case HDFConstants.DFNT_UINT32:
                        maxLength = 12;
                        theStr = String.valueOf(java.lang.reflect.Array.getInt(data,i*w+j));
                        break;
                    case HDFConstants.DFNT_INT64:
                    case HDFConstants.DFNT_UINT64:
                        maxLength = 24;
                        theStr = String.valueOf(java.lang.reflect.Array.getLong(data,i*w+j));
                        break;
                    case HDFConstants.DFNT_FLOAT32:
                        maxLength = 15;
                        theStr = String.valueOf(java.lang.reflect.Array.getFloat(data,i*w+j));
                        break;
                    case HDFConstants.DFNT_FLOAT64:
                        maxLength = 24;
                        theStr = String.valueOf(java.lang.reflect.Array.getDouble(data,i*w+j));
                        break;
                }
                padding = maxLength-theStr.length();
                for (int k=0; k<padding; k++) theStr += " ";
                theStr += delimiter;
                print(theStr);
            }
            println();
        }
        flush();
    }

    /**
     *  write HDF Vdata data
     *
     *  @info    text information about the data
     *  @fileds  the array of data fields
     *  @records the range of data records
     *  @data    the vdata
     */
    public void writeVdata(String name, int[] field_types, String[] field_names,
        int[] field_orders, int n_records, String[][] data) throws IOException
    {
        if (field_types  == null  ||
            field_names  == null ||
            field_orders == null ||
            data         == null )
            return;

        int n_fields = field_types.length;

        // find the max length for each column
        int[] w_columns = new int[n_fields];
        for (int i=0; i<n_fields; i++)
        {
            w_columns[i] = field_names[i].length();
            for (int j=0; j<n_records; j++)
                w_columns[i] = Math.max(w_columns[i], data[j][i].length());
        }

        int padding = 0;
        String names = "";
        String types = "";
        String orders = "";
        String theStr = "";

        // write the head information
        for (int i=0; i<n_fields; i++)
        {
            theStr = field_names[i];
            theStr = theStr.replace(' ', '_');
            padding = w_columns[i]-theStr.length();
            for (int k=0; k<padding; k++) theStr += " ";
            names += theStr+delimiter;

            theStr = HDFNativeData.getType(field_types[i]);
            padding = w_columns[i]-theStr.length();
            for (int k=0; k<padding; k++) theStr += " ";
            types += theStr+delimiter;

            theStr = String.valueOf(field_orders[i]);
            padding = w_columns[i]-theStr.length();
            for (int k=0; k<padding; k++) theStr += " ";
            orders += theStr+delimiter;
        }
        println();
        println(HDFConstants.VDATA);
        println(name);
        println(String.valueOf(n_records));
        println(String.valueOf(n_fields));
        println(types);
        println(names);
        println(orders);

        // write data
        for (int i=0; i<n_records; i++)
        {
            for (int j=0; j<n_fields; j++)
            {
                theStr = String.valueOf(data[i][j]);
                padding = w_columns[j]-theStr.length();
                for (int k=0; k<padding; k++) theStr += " ";
                theStr += delimiter;
                print(theStr);
            }
            println();
        }
        flush();

    }
}
