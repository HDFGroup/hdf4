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

package ncsa.hdf.message;

import java.io.Serializable;
import java.io.File;
import java.util.Vector;

import ncsa.hdf.hdflib.HDFLibrary;
import ncsa.hdf.hdflib.HDFNativeData;
import ncsa.hdf.hdflib.HDFConstants;
import ncsa.hdf.hdflib.HDFException;
import ncsa.hdf.util.Tools;

/**
 *  HDFObject holds HDF Message to/from the server
 
 *  @version   1.1.3 September 2 1997
 *  @author    Peter X. Cao
 */
public abstract class HDFObject implements Messageable, Serializable
{
    //The max length of data names
    public final static int MAX_NAME_LENGTH = 25;

    /** the HDFObjectNode to be transfered between the server and client */
    protected HDFObjectNode nodeObject;

    /** the name of the requested HDF file */
    protected String filename;

    /** the HDF library */
    protected HDFLibrary hdf;

    /** the text information about the object */
    protected String information;

    /** the data object which holds the HDF data content */
    protected Object data;

    /**
     *  Constructs an HDFObject with default parameters
     */
    public HDFObject () {this(null, "");}

    /**
     *  Construct an HDFObject with specified HDFObjectNode and filename
     *
     *  @param node     the HDFObjectNode to be transfered to the server
     *  @param filename the requested HDF file
     */
    public HDFObject (HDFObjectNode node, String filename)
    {
        this.filename = filename;
        this.information = "";
        this.data = null;
        this.nodeObject = node;
    }

    /**
     *  check if the object is me
     *
     *  @param message   the incomming HDFMessage
     */
    public boolean isMe(HDFMessage message)
    {
        return getClass().getName().equals(message.getOwner());
    }

    /**
     *  create a message sending to the server
     *
     *  @return  the HDFMessage created by this object
     */
    public HDFMessage toServer()
    {
        HDFMessage message = new HDFMessage(getClass().getName());
        if (nodeObject != null) message.put("nodeObject", nodeObject);
        if (filename != null) message.put("filename", filename);
        else message.put("filename", new String(""));
        return message;
    }

    /**
     *  process a message received from the client with default document root.
     * 
     *  @param  message  the message received from the client
     */
    public void fromClient(HDFMessage message)
    {
        this.fromClient(message, "");
    }

    /**
     *  process a message received from the client.
     * 
     *  @param  message  The message received from the client
     *  @param  docRoot  The server document root;
     */
    public void fromClient(HDFMessage message, String docRoot)
    {
        filename = (String) message.get("filename");
        if ( filename == null ) filename = "";
        filename = docRoot+File.separator + filename;
        filename = filename.replace('/', File.separatorChar);
        filename = filename.replace('\\', File.separatorChar);
        nodeObject = (HDFObjectNode) message.get("nodeObject");
        int index = -1;
        String doubleSeparator = File.separator+File.separator;
        while ( (index = filename.indexOf(doubleSeparator)) >= 0)
            filename = filename.substring(0, index)+filename.substring(index+1);
        service();
    }

    /** return the information */
    public String getInformation() { return information.trim(); }

    /** return the data content */
    public Object getData() { return data; }

    /**
     *  retrieve information about the object
     *  subclass needs to overwrite this method
     *
     *  @exception ncsa.hdf.hdflib.HDFException 
     *             should be thrown for errors in the
     *             HDF library call, but is not yet implemented.
     *
     */
    public static String readInfo (HDFLibrary hdf, String filename,
        HDFObjectNode node) throws HDFException { return "";}

    /** check if the file is an hdf file */
    public static boolean isHDF(String filename)
    {
        int fid = -1;
        HDFLibrary hdf = null;
        try
        {
            hdf = new HDFLibrary();
            fid = hdf.Hopen(filename);
            hdf.Hclose(fid);
        }
        catch (Exception e) { return false; }

        return (fid>0);
    }

    /**
     *  get an HDF object annotation.
     *
     *  @param   fid     the file identifer
     *  param    anntype the type of annontation.
     *  @param   node    the HDF Object node
     *  @return          the string conatainig the object annotation
     *
     *  @exception ncsa.hdf.hdflib.HDFException 
     *             should be thrown for errors in the
     *             HDF library call, but is not yet implemented.
     *
     */
    protected String getObjectAnnotation(int fid, int anntype,
        HDFObjectNode node) throws HDFException
    {
        int ref = node.ref;
        int tag = node.tag;
        String annotation = "\nHDF Object Annotation:\n";
        int[] fileInfo = new int[4];
        int an_id =  hdf.ANstart(fid);

        if ( !hdf.ANfileinfo(an_id, fileInfo) )
        {
            hdf.ANend(an_id);
            return  "ERROR: native call to HDFLibrary.ANfileinfo() failed.";
        }

        // get the number of the object label
        int numObject = hdf.ANnumann(an_id, anntype, (short)tag, (short)ref);
        
        if (numObject == HDFConstants.FAIL)
        {
            hdf.ANend(an_id);
            return "ERROR: native call to HDFLibrary.ANnumann() failed.";
        }

        // alocate space 
        int ann_id_list[] = new int[numObject];

        // get the list of the object annotation id
        int numAnnid = hdf.ANannlist(an_id, anntype, tag, ref, ann_id_list);

        if (numAnnid == HDFConstants.FAIL)
        {
            hdf.ANend(an_id);
            return  "ERROR: native call to HDFLibrary.ANannlist() failed.";
        }
        
        for (int i=0; i<numObject; i++)
        {
            // get ann_id;
            int annid = ann_id_list[i];

            // get the object label length
            int objectLen = hdf.ANannlen(annid);

            if (objectLen >0)
            {
                String annotationBuf[] = new String[1];
                annotationBuf[0] = new String("");

                // get object label
                if (hdf.ANreadann(annid, annotationBuf, objectLen))
                {
                    annotation = annotation + annotationBuf[0].trim();
                }        
            }
        }
        
        hdf.ANend(an_id);
        return annotation;
    }

    /**
     *  convert object data into image data without data range.
     *
     *  @param input    The input object
     *  @param type     The data type
     *  @param output   The output byte array
     *  @return         true if the convertion is successful
     */
    public static boolean object2byte(Object input, int type, byte[] output)
    {
        return object2byte(input, type, null, output);
    }

    /**
     *  convert object data into image data with specified data range.
     *
     *  @param input    The input object
     *  @param type     The data type
     *  @param minmax   The minimum and maximum value os the array
     *  @param output   The output byte array
     *  @return         true if the convertion is successful
     */
    public static boolean object2byte(Object input, int type, double[] minmax,
        byte[] output)
    {
        return object2byte(input, type, minmax, 0, output);
    }

    /**
     *  convert object data into image data with specified range and position.
     *
     *  @param input    The input object
     *  @param type     The data type
     *  @param minmax   The minimum and maximum value os the array
     *  @param position The first converted scientific data position
     *  @param output   The output byte array
     *  @return         true if the convertion is successful
     */
    public static boolean object2byte(Object input, int type, double[] minmax,
        int position, byte[] output)
    {
        // empty data set
        if ( (input == null) || (output == null) )
            return false;

        double spread = 1.0;
        int size = output.length;
        switch(type)
        {
            // one byte data
            case HDFConstants.DFNT_CHAR:
            case HDFConstants.DFNT_UCHAR8:
            case HDFConstants.DFNT_UINT8:
            case HDFConstants.DFNT_INT8:
            {
                byte data[] = (byte[]) input;
                if (minmax == null || minmax[0]==minmax[1])
                {
                    if (minmax == null) minmax = new  double[2];
                    byte range[] = (byte[])Tools.getRange(data, position, position+size-1);
                    minmax[0] = (double)range[0];
                    minmax[1] = (double)range[1];
                }

                for (int i=0; i<size; i++)
                {
                    if (data[i+position]<0) data[i+position] += 256;
                    if (data[i+position]>minmax[1])  output[i] = (byte)minmax[1];
                    else if (data[i+position]<minmax[0]) output[i] = (byte)minmax[0];
                    else output[i] = (byte)data[i+position];
                }
                break;
            }
            // short
            case HDFConstants.DFNT_INT16:
            case HDFConstants.DFNT_UINT16:
            {
                short data[] = (short[]) input;
                if (minmax == null || minmax[0]==minmax[1])
                {
                    if (minmax == null) minmax = new  double[2];
                    short range[] = (short[])Tools.getRange(data, position, position+size-1);
                    minmax[0] = (double)range[0];
                    minmax[1] = (double)range[1];
                }
                short delta = (short) (minmax[1]-minmax[0]);
                spread = (minmax[1] == minmax[0]) ? 1.00d : (double)(255.00/delta);
                if (spread > 255.00/Integer.MAX_VALUE)
                {
                    for (int i=0; i<size; i++)
                    {
                        output[i] = (byte)((data[i+position]-minmax[0])*spread);
                        if (output[i] == 0) output[i] = 1;
                    }
                }
                else
                {
                    for (int i=0; i<size; i++)
                        output[i] = 1;
                }
                break;
            }
            case HDFConstants.DFNT_INT32:
            case HDFConstants.DFNT_UINT32:
            {
                int data[] = (int[]) input;
                if (minmax == null || minmax[0]==minmax[1])
                {
                    if (minmax == null) minmax = new  double[2];
                    int range[] = (int[])Tools.getRange(data, position, position+size-1);
                    minmax[0] = (double)range[0];
                    minmax[1] = (double)range[1];
                }
                int delta = (int) (minmax[1]-minmax[0]);
                spread = (minmax[1] == minmax[0]) ? 1.00d : (double)(255.00/delta);
                if (spread > 255.00/Integer.MAX_VALUE)
                {
                    for (int i=0; i<size; i++)
                    {
                        output[i] = (byte)((data[i+position]-minmax[0])*spread);
                        if (output[i] == 0) output[i] = 1;
                    }
                }
                else
                {
                    for (int i=0; i<size; i++)
                        output[i] = 1;
                }
                break;
            }
            case HDFConstants.DFNT_INT64:
            case HDFConstants.DFNT_UINT64:
            {
                long data[] = (long[]) input;
                if (minmax == null || minmax[0]==minmax[1])
                {
                    if (minmax == null) minmax = new  double[2];
                    long range[] = (long[])Tools.getRange(data, position, position+size-1);
                    minmax[0] = (double)range[0];
                    minmax[1] = (double)range[1];
                }
                long delta = (long) (minmax[1]-minmax[0]);
                spread = (minmax[1] == minmax[0]) ? 1.00d : (double)(255.00/delta);
                if (spread > 255.00/Integer.MAX_VALUE)
                {  
                    for (int i=0; i<size; i++)
                    {
                        output[i] = (byte)((data[i+position]-minmax[0])*spread); 
                        if (output[i] == 0) output[i] = 1;
                    }
                } 
                else
                {
                    for (int i=0; i<size; i++)  
                        output[i] = 1;
                }
                break;
            }
            case HDFConstants.DFNT_FLOAT32:
            {
                float data[] = (float[]) input;
                if (minmax == null || minmax[0]==minmax[1])
                {
                    if (minmax == null) minmax = new  double[2];
                    float range[] = (float[])Tools.getRange(data, position, position+size-1);
                    minmax[0] = (double)range[0];
                    minmax[1] = (double)range[1];
                }
                float delta = (float) (minmax[1]-minmax[0]);
                spread = (minmax[1] == minmax[0]) ? 1.00d : (double)(255.00/delta);
                if (spread > 255.00/Integer.MAX_VALUE)
                {
                    for (int i=0; i<size; i++)
                    {
                        output[i] = (byte)((data[i+position]-minmax[0])*spread);
                        if (output[i] == 0) output[i] = 1;
                    }
                }
                else
                {
                    for (int i=0; i<size; i++)
                        output[i] = 1;
                }
                break;
            }
            case HDFConstants.DFNT_FLOAT64:
            {
                double data[] = (double[]) input;
                if (minmax == null || minmax[0]==minmax[1])
                {
                    if (minmax == null) minmax = new  double[2];
                    double range[] = (double[])Tools.getRange(data, position, position+size-1);
                    minmax[0] = (double)minmax[0];
                    minmax[1] = (double)minmax[1];
                }
                double delta = minmax[1]-minmax[0];
                spread = (minmax[1] == minmax[0]) ? 1.00d : (double)(255.00/delta);
                if (spread > 255.00/Integer.MAX_VALUE)
                {
                    for (int i=0; i<size; i++)
                    {
                        output[i] = (byte)((data[i+position]-minmax[0])*spread);
                        if (output[i] == 0) output[i] = 1;
                    }
                }
                else
                {
                    for (int i=0; i<size; i++)
                        output[i] = 1;
                }
                break;
            }
            default:
                return false;
        }

        return true;
    }

    /**
     *  Process numeric attributes or strings, converting from bytes to Java types.
     *  This calls native routines to do the numeric conversions.
     *
     *  @exception ncsa.hdf.hdflib.HDFException 
     *             should be thrown for errors in the
     *             HDF library call, but is not yet implemented.
     *
     */
    protected  String doattrs( byte[] buf, int buffsize, int NT, int count )
        throws HDFException
    {

        HDFNativeData convert = new HDFNativeData();
        String lr = " ";
        String separator = "; ";
        String str = new String("");

        int incr = HDFLibrary.DFKNTsize(NT);
        switch(NT)
        {
            case HDFConstants.DFNT_CHAR:
                // take it as char
                str = str.concat(lr);
                for (int jj=0; jj<count; jj++)
                {
                    String sval = new String(buf,jj,1);
                    str = str.concat(sval);
                }
                str = str.concat("  "); // need 'separator' to be removed below
                break;
            case HDFConstants.DFNT_UCHAR8:
            //case HDFConstants.DFNT_UCHAR:
            case HDFConstants.DFNT_UINT8:
                str = str.concat(lr);
                for (int jj=0; jj<count; jj++)
                {
                    Byte bval = new Byte(buf[jj]);
                    Short shval;
                    if (bval.shortValue() < 0)
                        shval = new Short((short)(bval.intValue() + 256));
                    else
                        shval = new Short(bval.shortValue());
                    str = str.concat(shval.toString());
                    str = str.concat(separator);
                }
                break;
            case HDFConstants.DFNT_INT8:
                str = str.concat(lr);
                for (int jj=0; jj<count; jj++)
                {
                    int pos = jj*incr;
                    // convert to integer
                    Byte bval = new Byte(buf[pos]);
                    str = str.concat(bval.toString());
                    str = str.concat(separator);
                }
                break;
            case HDFConstants.DFNT_INT16:
                str.concat(lr);
                for (int jj=0; jj<count; jj++)
                {
                    int pos = jj*incr;
                    // convert to integer
                    Short shval = new Short(convert.byteToShort(buf,pos));
                    str = str.concat(shval.toString());
                    str = str.concat(separator);
                }
                break;
            case HDFConstants.DFNT_UINT16:
                str = str.concat(lr);
                for (int jj=0; jj<count; jj+=incr)
                {
                    int pos = jj*incr;
                    Short shval = new Short(convert.byteToShort(buf,pos));
                    Integer ival;
                    if (shval.shortValue() < 0)  
                        ival = new Integer((shval.intValue() + 65536));
                    else  
                        ival = new Integer(shval.intValue());
                    str = str.concat(ival.toString());
                    str = str.concat(separator);
                }
                break;
            case HDFConstants.DFNT_INT32:
                str = str.concat(lr);
                for (int jj=0; jj<count; jj++)
                {
                    int pos = jj*incr;
                    // convert to integer
                    Integer ival = new Integer(convert.byteToInt(buf,pos));
                    str = str.concat(ival.toString());
                    str = str.concat(separator);
                }
                break;
            case HDFConstants.DFNT_UINT32:
                str = str.concat(lr);
                for (int jj=0; jj<count; jj+=incr)
                {
                    int pos = jj*incr;
                    Integer ival = new Integer(convert.byteToInt(buf,pos));
                    Float fval;
                    if (ival.intValue() < 0)
                        fval = new Float((ival.floatValue() + (float)4294967295.0));
                    else
                        fval = new Float(ival.intValue());
                    str = str.concat(ival.toString());
                    str = str.concat(separator);
                }
                break;
            //case HDFConstants.DFNT_FLOAT:
            case HDFConstants.DFNT_FLOAT32:
                str = str.concat(lr);
                for (int jj=0; jj<count; jj++)
                {
                    int pos = jj*incr;
                    // convert to float
                    Float fval = new Float(convert.byteToFloat(buf,pos));
                    str = str.concat(fval.toString());
                    str = str.concat(separator);
                }
                break;
            //case HDFConstants.DFNT_FLOAT64:
            case HDFConstants.DFNT_DOUBLE:
                str = str.concat(lr);
                for (int jj=0; jj<count; jj++)
                {
                    int pos = jj*incr;
                    // convert to integer
                    Double dval = new Double(convert.byteToDouble(buf,pos));
                    str = str.concat(dval.toString());
                    str = str.concat(separator);
                }
                break;
        }

        str = str.substring(0, str.length()-separator.length());
        return(str);
    }

    /**
     *  Return the RAINBOW palette
     *  rgb rgb rgb rgb rgb ....
     */
    public static byte[] createRainbowPalette()
    {
        int[] rainbowValues = {
        0x00, 0x00, 0x00, 0x7c, 0x00, 0xff, 0x78, 0x00, 0xfe, 0x73, 0x00, 0xff,
        0x6f, 0x00, 0xfe, 0x6a, 0x00, 0xff, 0x66, 0x00, 0xfe, 0x61, 0x00, 0xff,
        0x5d, 0x00, 0xfe, 0x58, 0x00, 0xff, 0x54, 0x00, 0xfe, 0x4f, 0x00, 0xff,
        0x4b, 0x00, 0xfe, 0x46, 0x00, 0xff, 0x42, 0x00, 0xfe, 0x3d, 0x00, 0xff,
        0x39, 0x00, 0xfe, 0x34, 0x00, 0xff, 0x30, 0x00, 0xfe, 0x2b, 0x00, 0xff,
        0x27, 0x00, 0xfe, 0x22, 0x00, 0xff, 0x1e, 0x00, 0xfe, 0x19, 0x00, 0xff,
        0x15, 0x00, 0xfe, 0x10, 0x00, 0xff, 0x0c, 0x00, 0xfe, 0x07, 0x00, 0xff,
        0x03, 0x00, 0xfe, 0x00, 0x02, 0xff, 0x00, 0x06, 0xfe, 0x00, 0x0b, 0xff,
        0x00, 0x0f, 0xfe, 0x00, 0x14, 0xff, 0x00, 0x18, 0xfe, 0x00, 0x1d, 0xff,
        0x00, 0x21, 0xfe, 0x00, 0x26, 0xff, 0x00, 0x2a, 0xfe, 0x00, 0x2f, 0xff,
        0x00, 0x33, 0xfe, 0x00, 0x38, 0xff, 0x00, 0x3c, 0xfe, 0x00, 0x41, 0xff,
        0x00, 0x45, 0xfe, 0x00, 0x4a, 0xff, 0x00, 0x4e, 0xfe, 0x00, 0x53, 0xff,
        0x00, 0x57, 0xfe, 0x00, 0x5c, 0xff, 0x00, 0x60, 0xfe, 0x00, 0x65, 0xff,
        0x00, 0x69, 0xfe, 0x00, 0x6e, 0xff, 0x00, 0x72, 0xfe, 0x00, 0x77, 0xff,
        0x00, 0x7a, 0xfe, 0x00, 0x80, 0xff, 0x00, 0x83, 0xfe, 0x00, 0x89, 0xff,
        0x00, 0x8c, 0xfe, 0x00, 0x92, 0xff, 0x00, 0x95, 0xfe, 0x00, 0x9b, 0xff,
        0x00, 0x9e, 0xfe, 0x00, 0xa4, 0xff, 0x00, 0xa7, 0xfe, 0x00, 0xad, 0xff,
        0x00, 0xb0, 0xfe, 0x00, 0xb6, 0xff, 0x00, 0xb9, 0xfe, 0x00, 0xbf, 0xff,
        0x00, 0xc2, 0xfe, 0x00, 0xc8, 0xff, 0x00, 0xcb, 0xfe, 0x00, 0xd1, 0xff,
        0x00, 0xd4, 0xfe, 0x00, 0xda, 0xff, 0x00, 0xdd, 0xfe, 0x00, 0xe3, 0xff,
        0x00, 0xe6, 0xfe, 0x00, 0xec, 0xff, 0x00, 0xf0, 0xfe, 0x00, 0xf5, 0xff,
        0x00, 0xf9, 0xfe, 0x00, 0xfe, 0xff, 0x00, 0xfe, 0xfa, 0x00, 0xff, 0xf7,
        0x00, 0xfe, 0xf1, 0x00, 0xff, 0xee, 0x00, 0xfe, 0xe8, 0x00, 0xff, 0xe5,
        0x00, 0xfe, 0xdf, 0x00, 0xff, 0xdc, 0x00, 0xfe, 0xd6, 0x00, 0xff, 0xd3,
        0x00, 0xfe, 0xcd, 0x00, 0xff, 0xca, 0x00, 0xfe, 0xc4, 0x00, 0xff, 0xc1,
        0x00, 0xfe, 0xbb, 0x00, 0xff, 0xb8, 0x00, 0xfe, 0xb2, 0x00, 0xff, 0xaf,
        0x00, 0xfe, 0xa9, 0x00, 0xff, 0xa6, 0x00, 0xfe, 0xa0, 0x00, 0xff, 0x9d,
        0x00, 0xfe, 0x97, 0x00, 0xff, 0x94, 0x00, 0xfe, 0x8e, 0x00, 0xff, 0x8b,
        0x00, 0xfe, 0x85, 0x00, 0xff, 0x82, 0x00, 0xfe, 0x7d, 0x00, 0xff, 0x79,
        0x00, 0xfe, 0x74, 0x00, 0xff, 0x70, 0x00, 0xfe, 0x6b, 0x00, 0xff, 0x67,
        0x00, 0xfe, 0x62, 0x00, 0xff, 0x5e, 0x00, 0xfe, 0x59, 0x00, 0xff, 0x55,
        0x00, 0xfe, 0x50, 0x00, 0xff, 0x4c, 0x00, 0xfe, 0x47, 0x00, 0xff, 0x43,
        0x00, 0xfe, 0x3e, 0x00, 0xff, 0x3a, 0x00, 0xfe, 0x35, 0x00, 0xff, 0x31,
        0x00, 0xfe, 0x2c, 0x00, 0xff, 0x28, 0x00, 0xfe, 0x23, 0x00, 0xff, 0x1f,
        0x00, 0xfe, 0x1a, 0x00, 0xff, 0x16, 0x00, 0xfe, 0x11, 0x00, 0xff, 0x0d,
        0x00, 0xfe, 0x08, 0x00, 0xff, 0x04, 0x01, 0xfe, 0x00, 0x05, 0xff, 0x00,
        0x0a, 0xfe, 0x00, 0x0e, 0xff, 0x00, 0x13, 0xfe, 0x00, 0x17, 0xff, 0x00,
        0x1c, 0xfe, 0x00, 0x20, 0xff, 0x00, 0x25, 0xfe, 0x00, 0x29, 0xff, 0x00,
        0x2e, 0xfe, 0x00, 0x32, 0xff, 0x00, 0x37, 0xfe, 0x00, 0x3b, 0xff, 0x00,
        0x40, 0xfe, 0x00, 0x44, 0xff, 0x00, 0x49, 0xfe, 0x00, 0x4d, 0xff, 0x00,
        0x52, 0xfe, 0x00, 0x56, 0xff, 0x00, 0x5b, 0xfe, 0x00, 0x5f, 0xff, 0x00,
        0x64, 0xfe, 0x00, 0x68, 0xff, 0x00, 0x6d, 0xfe, 0x00, 0x71, 0xff, 0x00,
        0x76, 0xfe, 0x00, 0x7b, 0xff, 0x00, 0x7e, 0xfe, 0x00, 0x84, 0xff, 0x00,
        0x87, 0xfe, 0x00, 0x8d, 0xff, 0x00, 0x90, 0xfe, 0x00, 0x96, 0xff, 0x00,
        0x99, 0xfe, 0x00, 0x9f, 0xff, 0x00, 0xa2, 0xfe, 0x00, 0xa8, 0xff, 0x00,
        0xab, 0xfe, 0x00, 0xb1, 0xff, 0x00, 0xb4, 0xfe, 0x00, 0xba, 0xff, 0x00,
        0xbd, 0xfe, 0x00, 0xc3, 0xff, 0x00, 0xc6, 0xfe, 0x00, 0xcc, 0xff, 0x00,
        0xcf, 0xfe, 0x00, 0xd5, 0xff, 0x00, 0xd8, 0xfe, 0x00, 0xde, 0xff, 0x00,
        0xe1, 0xfe, 0x00, 0xe7, 0xff, 0x00, 0xea, 0xfe, 0x00, 0xf0, 0xff, 0x00,
        0xf3, 0xfe, 0x00, 0xf9, 0xff, 0x00, 0xfc, 0xfe, 0x00, 0xff, 0xfc, 0x00,
        0xfe, 0xf7, 0x00, 0xff, 0xf3, 0x00, 0xfe, 0xee, 0x00, 0xff, 0xea, 0x00,
        0xfe, 0xe5, 0x00, 0xff, 0xe1, 0x00, 0xfe, 0xdc, 0x00, 0xff, 0xd8, 0x00,
        0xfe, 0xd3, 0x00, 0xff, 0xcf, 0x00, 0xfe, 0xca, 0x00, 0xff, 0xc6, 0x00,
        0xfe, 0xc1, 0x00, 0xff, 0xbd, 0x00, 0xfe, 0xb8, 0x00, 0xff, 0xb4, 0x00,
        0xfe, 0xaf, 0x00, 0xff, 0xab, 0x00, 0xfe, 0xa6, 0x00, 0xff, 0xa2, 0x00,
        0xfe, 0x9d, 0x00, 0xff, 0x99, 0x00, 0xfe, 0x94, 0x00, 0xff, 0x90, 0x00,
        0xfe, 0x8b, 0x00, 0xff, 0x87, 0x00, 0xfe, 0x83, 0x00, 0xff, 0x7e, 0x00,
        0xfe, 0x7a, 0x00, 0xff, 0x75, 0x00, 0xfe, 0x71, 0x00, 0xff, 0x6c, 0x00,
        0xfe, 0x68, 0x00, 0xff, 0x63, 0x00, 0xfe, 0x5f, 0x00, 0xff, 0x5a, 0x00,
        0xfe, 0x56, 0x00, 0xff, 0x51, 0x00, 0xfe, 0x4d, 0x00, 0xff, 0x48, 0x00,
        0xfe, 0x44, 0x00, 0xff, 0x3f, 0x00, 0xfe, 0x3b, 0x00, 0xff, 0x36, 0x00,
        0xfe, 0x32, 0x00, 0xff, 0x2d, 0x00, 0xfe, 0x29, 0x00, 0xff, 0x24, 0x00,
        0xfe, 0x20, 0x00, 0xff, 0x1b, 0x00, 0xfe, 0x17, 0x00, 0xff, 0x12, 0x00,
        0xfe, 0x0e, 0x00, 0xff, 0x09, 0x00, 0xff, 0x05, 0x00, 0xff, 0xff, 0xff };

        byte[] retVal = new byte[768];
        for (int kk=0; kk<768; kk++)
            retVal[kk] = (byte)rainbowValues[kk];
     	return retVal;
    }

}
