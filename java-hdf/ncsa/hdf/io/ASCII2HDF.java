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

import java.io.RandomAccessFile;
import java.io.IOException;
import java.util.StringTokenizer;
import java.lang.reflect.Array;
import java.awt.Dimension;
import ncsa.hdf.util.Queue;
import ncsa.hdf.hdflib.HDFConstants;
import ncsa.hdf.hdflib.HDFNativeData;
import ncsa.hdf.hdflib.HDFException;
import ncsa.hdf.hdflib.HDFLibrary;

/**
 *  ASCII2HDF converts an ASCII file into an HDF file

 *  @version   1.1.3 May 6, 1998
 *  @author    Peter X. Cao
 */
public class ASCII2HDF
{
    /** new line feed plus carrage return */
    public static char[] nl = {13, 10};

    /** the new line charactor */
    public static final String NL = new String(nl);

    /** the RandomAccessFile to read an ASCII file */
    private RandomAccessFile reader = null;
 
    /** the HDFWriter to write an HDF file */
    private HDFWriter writer = null;

    public ASCII2HDF(String asciiFile, String hdfFile) 
        throws IOException, HDFException
    {
        reader = new RandomAccessFile(asciiFile, "r");
        writer = new HDFWriter(hdfFile);

        if (writer!=null) writer.createHDF();
    }

    /** converts the ascii file into hdf file */
    public void convert()
    {
        long fileSize = 0;
        long position = 0;
        try { fileSize = reader.length(); }
        catch (IOException e) {}
        String line = "";

        while (position < fileSize)
        {
            try { line = reader.readLine().trim(); }
            catch (IOException e) {}

            // add annotation
            if (line.equalsIgnoreCase(HDFConstants.ANNOTATION))
                writeAnnotation();
            else if ( line.equalsIgnoreCase(HDFConstants.RI8) ||
                line.equalsIgnoreCase(HDFConstants.RI24) ||
                line.equalsIgnoreCase(HDFConstants.GR) )
                writeGR();
            else if (line.equalsIgnoreCase(HDFConstants.SDS))
                writeSDS();
            else if (line.equalsIgnoreCase(HDFConstants.VDATA))
                writeVdata();

            try { position = reader.getFilePointer(); }
            catch (IOException e) {}
        }


        try { reader.close(); }
        catch (IOException e) {}
    }

    /** reads the file annotation from ascii file and write it into an HDF file */
    private void writeAnnotation()
    {
        String line = "";
        String annotation = "";
        long position = 0;
        boolean proceed = true;

        while (proceed)
        {
            try
            {
                position = reader.getFilePointer();
                line = reader.readLine().trim();
            }
            catch (Exception e) {}
            if (line.equalsIgnoreCase(HDFConstants.RI8)  ||
                line.equalsIgnoreCase(HDFConstants.RI24) ||
                line.equalsIgnoreCase(HDFConstants.GR)   ||
                line.equalsIgnoreCase(HDFConstants.SDS)  ||
                line.equalsIgnoreCase(HDFConstants.VDATA) )
                proceed = false;
            else annotation += line + NL;;
        }

        try
        {
           reader.seek(position);   // set back the position
           writer.writeAnnotation(annotation, HDFConstants.DFACC_WRITE);
        }
        catch (Exception e) {;}
    }

    /** reads the GR data from ascii file and write it into an HDF file */
    private void writeGR()
    {
        //System.out.println("ASCII2HDF.writeGR()");

        String line = "";
        String name = "";
        String typeStr = "";
        int type = -1;
        int w = 0;
        int h = 0;
        Object data = null;

        try
        {
            name = reader.readLine().trim();
            line = reader.readLine().trim();
            h = Integer.parseInt(line);
            line = reader.readLine().trim();
            w = Integer.parseInt(line);
            typeStr = reader.readLine().trim();
        }
        catch (Exception e) {System.out.println(e);}
        type = HDFNativeData.getType(typeStr);

        data = readData(h, w, type);
        int dims[] = {w, h};
        int[] start = {0, 0};
        int[] stride = {1, 1};
        int interlace = HDFConstants.MFGR_INTERLACE_PIXEL;
        int mode = HDFConstants.DFACC_WRITE;
        try {
            writer.writeGR(mode, name, 1, type, interlace, dims,
                start, stride, 0, data, null);
        }
        catch (Exception e) { }
    }

    /** reads the SDS data from ascii file and write it into an HDF file */
    private void writeSDS()
    {
        String line = "";
        String name = "";
        String typeStr = "";
        int type = -1;
        int cols = 0;  // number of data columns
        int rows = 1;  // number of data rows
        int rank = 0;
        int[] dims = null;
        Object data = null;

        int tmpInt = 0;
        try
        {
            name = reader.readLine().trim();
            line = reader.readLine().trim();
            rank = Integer.parseInt(line);
            dims = new int[rank];

            if (rank < 1)
                return;
            else if (rank == 1)
            {
                cols = 1;
                line = reader.readLine().trim();
                tmpInt = Integer.parseInt(line);
                dims[0] = tmpInt;
            }
            else
            {
                for (int i=0; i<(rank-1); i++)
                {
                    line = reader.readLine().trim();
                    tmpInt = Integer.parseInt(line);
                    rows *= tmpInt;
                    dims[i] = tmpInt;
                }
                line = reader.readLine().trim();
                cols = Integer.parseInt(line);
                dims[rank-1] = cols;
            }
            typeStr = reader.readLine().trim();
        }
        catch (Exception e) {System.out.println(e);}
        type = HDFNativeData.getType(typeStr);

        data = readData(rows, cols, type);
        int[] start = {0, 0};
        int[] stride = {1, 1};
        int mode = HDFConstants.DFACC_WRITE;
        try { writer.writeSDS(mode, name, rank, type, dims, start, null, data); }
        catch (Exception e) { System.out.println(e); }
    }

    /** reads the Vdata from ascii file and write it into an HDF file */
    private void writeVdata()
    {
        String line = "";
        String name = "";
        int numberOfRecords = 0;
        int numberOfFields = 0;
        int[] fieldTypes = null;
        int[] fieldOrders = null;
        String[] fieldNames = null;
        byte[] data = null;
        StringTokenizer tokenizer = null;
        String theToken = "";


        try
        {
            name = reader.readLine().trim();
            line = reader.readLine().trim();
            numberOfRecords = Integer.parseInt(line);
            line = reader.readLine().trim();
            numberOfFields = Integer.parseInt(line);

            fieldTypes = new int[numberOfFields];
            fieldOrders = new int[numberOfFields];
            fieldNames = new String[numberOfFields];

            line = reader.readLine().trim();
            tokenizer = new StringTokenizer(line);
            for (int i=0; i<numberOfFields; i++)
            {
                theToken = tokenizer.nextToken();
                fieldTypes[i] = HDFNativeData.getType(theToken);
            }

            line = reader.readLine().trim();
            tokenizer = new StringTokenizer(line);
            for (int i=0; i<numberOfFields; i++)
            {
                fieldNames[i] = tokenizer.nextToken();
            }

            line = reader.readLine().trim();
            tokenizer = new StringTokenizer(line);
            for (int i=0; i<numberOfFields; i++)
            {
                theToken = tokenizer.nextToken();
                fieldOrders[i] = Integer.parseInt(theToken);
            }
        }
        catch (Exception e) {System.out.println(e);}

        int[] typeSize = new int[numberOfFields];
        int recordSize = 0;
        for (int i=0; i<numberOfFields; i++)
        {
            int bytes = 0;
            try { bytes = HDFLibrary.DFKNTsize(fieldTypes[i]); }
            catch (Exception ex) {}
            typeSize[i] = bytes;
            recordSize += fieldOrders[i]*bytes;
        }

        data = new byte[recordSize*numberOfRecords];
        int position = 0;
        int cellSize = 0;
        int rowPosition = 0;
        byte[] theCell = null;
        int order = 0;
        int dataType = 0;

        for (int row=0; row<numberOfRecords; row++)
        {
            try 
            { 
                line = reader.readLine().trim();
                tokenizer = new StringTokenizer(line);
            }
            catch (Exception e) {}

            for (int col=0; col<numberOfFields; col++)
            {
                order = fieldOrders[col];
                dataType = fieldTypes[col];
                cellSize = order*typeSize[col];
                theCell = new byte[cellSize];
                
                switch(dataType)
                {
                    case HDFConstants.DFNT_CHAR:
                    case HDFConstants.DFNT_INT8:
                    case HDFConstants.DFNT_UCHAR8:
                    case HDFConstants.DFNT_UINT8:
                        theToken = tokenizer.nextToken();
                        theCell = theToken.getBytes();
                        break;
                    case HDFConstants.DFNT_INT16:
                    case HDFConstants.DFNT_UINT16:
                        short[] sdata = new short[order];
                        for (int i=0; i<order; i++)
                        {
                            theToken = tokenizer.nextToken();
                            sdata[i] = Short.parseShort(theToken);
                        }
                        theCell = HDFNativeData.shortToByte(0, order, sdata);
                        break;
                    case HDFConstants.DFNT_INT32:
                    case HDFConstants.DFNT_UINT32:
                        int[] idata = new int[order];
                        for (int i=0; i<order; i++)
                        {
                            theToken = tokenizer.nextToken();
                            idata[i] = Integer.parseInt(theToken);
                        }
                        theCell = HDFNativeData.intToByte(0, order, idata);
                        break;
                    case HDFConstants.DFNT_INT64:
                    //case HDFConstants.DFNT_UINT64:
                        long[] ldata = new long[order];
                        for (int i=0; i<order; i++)
                        {
                            theToken = tokenizer.nextToken();
                            ldata[i] = Long.parseLong(theToken);
                        }
                        theCell = HDFNativeData.longToByte(0, order, ldata);
                        break;
                    case HDFConstants.DFNT_FLOAT:
                    //case HDFConstants.DFNT_FLOAT32:
                        float[] fdata = new float[order];
                        for (int i=0; i<order; i++)
                        {
                            theToken = tokenizer.nextToken();
                            fdata[i] = (Float.valueOf(theToken)).floatValue();
                        }
                        theCell = HDFNativeData.floatToByte(0, order, fdata);
                        break;
                    case HDFConstants.DFNT_DOUBLE:
                    //case HDFConstants.DFNT_FLOAT64:
                        double[] ddata = new double[order];
                        for (int i=0; i<order; i++)
                        {
                            theToken = tokenizer.nextToken();
                            ddata[i] = (Double.valueOf(theToken)).doubleValue();
                        }
                        theCell = HDFNativeData.doubleToByte(0, order, ddata);
                        break;
                    default:
                        theCell = new byte[cellSize];

                } //switch(dataType)
        
                System.arraycopy(theCell, 0, data, position, cellSize);
                position +=cellSize;

            } //for (int col=0; col<numberOfFields; col++)
        } // for (int row=0; row<numberOfRecords; row++)

        int mode = HDFConstants.DFACC_WRITE;
        try
        {
            writer.writeVdata(mode, name, -1,
                fieldTypes, fieldNames,
                fieldOrders, data,
                numberOfRecords, HDFConstants.FULL_INTERLACE);
         } catch (Exception ex) {}
    }

    /**
     *  reads data from ascii file
     *  @param w     number of columns
     *  @param h     nubmer of rows
     *  @param p     number of planes
     *  @return      the data array
     */
    private Object readData(int rows, int cols, int dataType)
    {
        String line = "";
        StringTokenizer tokenizer = null;
        String theToken = "";
        Object data = HDFNativeData.defineDataObject(dataType, rows*cols);

        // the data object is not defined
        if (data == null ) return null;

        for (int i=0; i<rows; i++)
        {
            try {line = reader.readLine().trim();}
            catch (IOException e) {}
            tokenizer = new StringTokenizer(line);

            if (tokenizer == null || tokenizer.countTokens() < cols)
                continue;

            for (int j=0; j<cols; j++)
            {
                theToken = tokenizer.nextToken();
                
                switch(dataType)
                {
                    case HDFConstants.DFNT_CHAR:
                    case HDFConstants.DFNT_INT8:
                        Array.setByte(data, i*cols+j, Byte.parseByte(theToken));
                        break;
                    case HDFConstants.DFNT_UCHAR8:
                    case HDFConstants.DFNT_UINT8:
                    case HDFConstants.DFNT_INT16:
                        Array.setShort(data, i*cols+j, Short.parseShort(theToken));
                        break;
                    case HDFConstants.DFNT_UINT16:
                    case HDFConstants.DFNT_INT32:
                        Array.setInt(data, i*cols+j, Integer.parseInt(theToken));
                        break;
                    case HDFConstants.DFNT_INT64:
                    case HDFConstants.DFNT_UINT32:
                        Array.setLong(data, i*cols+j, Long.parseLong(theToken));
                        break;
                    case HDFConstants.DFNT_FLOAT32:
                        Array.setFloat(data, i*cols+j, (Float.valueOf(theToken)).floatValue());
                        break;
                    case HDFConstants.DFNT_DOUBLE:
                        Array.setDouble(data, i*cols+j, (Double.valueOf(theToken)).doubleValue());
                        break;
                    default:
                } // switch(dataType)
            } // for (int j=0; j<cols; j++)
        } // for (int i=0; i<rows; i++)

        return data;
    }

}
