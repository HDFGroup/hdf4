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
    /** the HDFObjectNode to be transfered between the server and client */
    protected HDFObjectNode nodeObject;

    /** the name of the requested HDF file */
    protected String hdfFilename;

    /** the HDF library */
    protected HDFLibrary hdf;

    /** the text information about the object */
    protected String information;

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
        this.hdf =  new HDFLibrary();
        this.hdfFilename = filename;
        this.information = "";
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
        if (hdfFilename != null) message.put("hdfFilename", hdfFilename);
        else message.put("hdfFilename", new String(""));
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
        String filename = (String) message.get("hdfFilename");
        if ( filename != null ) hdfFilename = filename;
        hdfFilename = docRoot+File.separator + hdfFilename;
        hdfFilename = hdfFilename.replace('/', File.separatorChar);
        hdfFilename = hdfFilename.replace('\\', File.separatorChar);
        nodeObject = (HDFObjectNode) message.get("nodeObject");
        int index = -1;
        String doubleSeparator = File.separator+File.separator;
        while ( (index = hdfFilename.indexOf(doubleSeparator)) >= 0)
            hdfFilename = hdfFilename.substring(0, index)+hdfFilename.substring(index+1);
        service();
    }

    /** return the information */
    public String getInformation() { return information; }

    /**
     *  retrieve information about the object
     *  subclass needs to overwrite this method
     */
    public static String readInfo (HDFLibrary hdf, String filename,
        HDFObjectNode node) throws HDFException { return "";}

    /**
     *  get an HDF object annotation.
     *
     *  @param   fid     the file identifer
     *  param    anntype the type of annontation.
     *  @param   node    the HDF Object node
     *  @return          the string conatainig the object annotation
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
                if (minmax == null)
                {
                    minmax = new double[2];
                    byte range[] = Tools.getRange(data, position, position+size-1);
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
                if (minmax == null)
                {
                    minmax = new double[2];
                    short range[] = Tools.getRange(data, position, position+size-1);
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
                if (minmax == null)
                {
                    minmax = new double[2];
                    int range[] = Tools.getRange(data, position, position+size-1);
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
                if (minmax == null)
                {
                    minmax = new double[2];
                    long range[] = Tools.getRange(data, position, position+size-1);
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
                if (minmax == null)
                {
                    minmax = new double[2];
                    float range[] = Tools.getRange(data, position, position+size-1);
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
                if (minmax == null)
                {
                    minmax = new double[2];
                    double range[] = Tools.getRange(data, position, position+size-1);
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

}
