package ncsa.hdf.message;

import java.awt.Dimension;
import ncsa.hdf.hdflib.HDFException;
import ncsa.hdf.hdflib.HDFLibrary;
import ncsa.hdf.hdflib.HDFConstants;
import ncsa.hdf.hdflib.HDFNativeData;

/**
 *  HDFSDS holds HDF SDS information

 *  @version   1.1.3 September 22 1997
 *  @author    Peter X. Cao
 */
public class HDFSDS extends HDFObject
{
    /** the original image size */
    protected  Dimension originalImageSize;

    /** the requested image size */
    protected  Dimension selectedImageSize;

    /** the image data */
    protected  Object    imageData;

    /** the iamge palette data */
    protected  byte      imagePalette[];

    /**
     *   hold image arguments.
     *  <pre>
     *   imageArgv[0] = number of images
     *   imageArgv[1] = image data type
     *  </pre>
     */
    protected  int imageArgv[];

    /** byte array containing data for animation images */
    protected  byte      images[][];

    /**
     *   hold data range in the image
     *  <pre>
     *   dataRange[0] = the minimum the the image data
     *   dataRange[2] = the maxnimum the the image data
     *  </pre>
     */
    protected  double dataRange[];

    /**
     *  Constructs an HDFSDS with default parameters
     */
    public HDFSDS ()
    {
        super();
        originalImageSize = null;
        selectedImageSize = null;
        imageData = null;
        imagePalette = null;
        imageArgv = null;
        dataRange = null;
        images = null;
    }

    /**
     *  Construct an HDFSDS with specified HDFObjectNode and filename
     *
     *  @param node     the HDFObjectNode to be transfered to the server
     *  @param filename the requested HDF file
     */
    public HDFSDS (HDFObjectNode node, String filename)
    {
        super(node, filename);
        originalImageSize = null;
        selectedImageSize = null;
        imageData = null;
        imagePalette = null;
        imageArgv = null;
        dataRange = null;
        images = null;
    }

    /**
     *  create a message for sending to the client
     *
     *  @return  the HDFMessage created by this object
     */
    public HDFMessage toClient()
    {
        HDFMessage message  = new HDFMessage(getClass().getName());

        message.put("information", information);
        if (originalImageSize != null) message.put("originalImageSize", originalImageSize);
        if (selectedImageSize != null) message.put("selectedImageSize", selectedImageSize);
        if (imageData != null) message.put("imageData", imageData);
        if (imagePalette != null ) message.put("imagePalette", imagePalette);
        if (imageArgv != null ) message.put("imageArgv", imageArgv);
        if (dataRange != null ) message.put("dataRange", dataRange);
        if (images != null ) message.put("images", images);
        return message;
    }

    /**
     *  overwrite HDFGR
     */
    public void service()
    {
        try { information = getSDS (hdfFilename, nodeObject); }
        catch (Exception e) { information = "ERROR: exception in HDFSDS.getSDS()"; }
    }

    /**
     *  process the message received from the server
     *
     *  @param message  the HDFMessage received from the server
     */
    public void fromServer(HDFMessage message)
    {
        information = (String) message.get("information");
        originalImageSize = (Dimension) message.get("originalImageSize");
        selectedImageSize = (Dimension) message.get("selectedImageSize");
        imageData = message.get("imageData");
        imagePalette = (byte[]) message.get("imagePalette");
        imageArgv = (int[]) message.get("imageArgv");
        dataRange = (double[]) message.get("dataRange");
        images = (byte[][]) message.get("images");
    }

    /**
     *  get HDF SDS image information for the selected HDF file
     *
     *  @prarm hdf        the HDFLibrary
     *  @param filename   the string of the hdf file name
     *  @param node       the HDFObjectNode
     *  @return           the string the HDF SDS image information
     */
    public static String readInfo (HDFLibrary hdf, String filename,
        HDFObjectNode node) throws HDFException
    {
        int    request    = node.type;
        int    requestRef = node.ref;
        int sdid  = -1 ;
        int sdsid = -1;
        int[] fileInfo = new int[2];
        String   info = new String();

        sdid = hdf.SDstart(filename, HDFConstants.DFACC_RDONLY);
        if (sdid == HDFConstants.FAIL)
        {
            hdf.SDend(sdid);
            return ("ERROR: native call to HDFLibrary.SDstart() failed");
        }

        if ( !hdf.SDfileinfo(sdid, fileInfo) )
        {
            hdf.SDend(sdid);
            return ("ERROR: native call to HDFLibrary.SDfileinfo() failed");
        }

        // sds number in HDF file
        if (request == HDFObjectNode.SDS)
        {
            info = info.concat("\nNumber of Dataset           : " + fileInfo[0]);
            info = info.concat("\nNumber of Global Attributes : " + fileInfo[1]);
        }
        else
        {
            int ref = requestRef;
            int index = hdf.SDreftoindex(sdid, ref);
            if (index == HDFConstants.FAIL)
            {   
                 info = info.concat("\nERROR: native call to HDFLibrary.SDreftoindex() failed"); 
            }
            else
            {
                if ((sdsid = hdf.SDselect(sdid,index)) == HDFConstants.FAIL)
                {
                    hdf.SDendaccess(sdsid);
                    info = info.concat("\nERROR: native call to HDFLibrary.SDselect() failed"); 
                }
                else
                { 
                    int[] SDInfo = new int[3];
                    String datasetname = new String("");
                    String[] ss = new String[1];
                    ss[0] = datasetname;
                    int  dimsize[]     = new int[16];
                    if (hdf.SDgetinfo(sdsid, ss, dimsize, SDInfo))
                    {
                        datasetname = ss[0];
                        info = info.concat("\nDataset name         : " + datasetname);
                        String rankInfo = Integer.toString(SDInfo[0]) + ", ";
                        for (int i=(SDInfo[0] - 1); i>0; i--)
                        {
                            rankInfo += Integer.toString(dimsize[i]);
                            rankInfo += " X ";
                        }
                        rankInfo += Integer.toString(dimsize[0]);
                        info = info.concat("\nRank                 : " + rankInfo);
                        info = info.concat("\nData number type     : " + hdf.HDgetNTdesc(SDInfo[1]));
                        info = info.concat("\nNumber of Attributes : " + SDInfo[2]);
                    } 
                    else 
                    {
                        info = info.concat("\nERROR: native call to HDFLibrary.SDgetinfo() failed");
                    }
                } 
                hdf.SDendaccess(sdsid);
            } 
        }   
 
        hdf.SDend(sdid);
        return (info);
    }

    /**
     *  get the selected HDF Generic raster image for the selected HDF file
     *
     *  @param filename   the string of the hdf file name
     *  @param node       the HDFObjectNode
     *  @return           the error message / SDS attributes
     */
    private String getSDS (String filename, HDFObjectNode node)
        throws HDFException
    {
        int request   = node.type;
        int ref       = node.ref;
        int attrIndex = node.index;
        int plane     = node.plane;
        int sdid  = -1;
        int sdsid = -1;
        int[] fileInfo = new int[2];
        String info = new String();        
    
        sdid = hdf.SDstart(filename, HDFConstants.DFACC_RDONLY);
        if (sdid == HDFConstants.FAIL)
        {
            hdf.SDend(sdid);
            return ("ERROR: native call to HDFLibrary.SDstart() failed.");
        }
    
        // get HDF SDS information
        if ( !hdf.SDfileinfo(sdid, fileInfo ) )
        {
            hdf.SDend(sdid);
            return ("ERROR: native call to HDFLibrary.SDfileinfo() failed.");
        }
    
    
        if (request == HDFObjectNode.SDSGLOBALATTR)
        {
            // SDS Global Attributes
            info = info.concat("\nSDS Global Attributes"+"\n");
            info = info.concat(getHdfSDSgattr(sdid, attrIndex)+"\n");
        }
        else if (request == HDFObjectNode.SDSDATASETATTR)
        {
            // SDS dataset attribute
            info = info.concat("\nSDS Image Attributes"+"\n");
            info = info.concat(getHdfSDSattr(sdid, attrIndex, ref)+"\n");
        }
        else
        { 
            info = getSDSImage(sdid, attrIndex, ref, plane);
        }

        hdf.SDend(sdid);
        return info;
    }

    /**
     *  get the HDF file SDS image
     *
     *  @param sdid       the SDS identifier
     *  @param ref        the reference number of the selected HDF object node
     *  @param attrIndex  the index of HDF file attributes
     *  @return           the error message
     */
    private String getSDSImage(int sdid, int attrIndex, int ref, int plane) throws HDFException
    {
        int sdsid = -1;
        int index = -1;
        int w = 256;
        int h = 256;
        String retStr = null;

        index = hdf.SDreftoindex(sdid, ref);
        if (index == HDFConstants.FAIL)
        {
            retStr = "ERROR: native call to HDFLibrary.SDreftoindex() failed";
        }
        else if ((sdsid = hdf.SDselect(sdid,index)) == HDFConstants.FAIL)
        {
            hdf.SDendaccess(sdsid);
            retStr = "ERROR: native call to HDFLibrary.SDselect() failed";
        }
        else
        {
            // get sds info.
            int [] SDInfo = new int[3];
            String datasetname = new String("");
            String[] ss = new String[1];
            ss[0] = datasetname;
            int  dimsize[] = new int[16];

            if (hdf.SDgetinfo(sdsid, ss, dimsize, SDInfo))
            {
                datasetname = ss[0];
                int rank = SDInfo[0];
                if ((rank ==2) || (rank ==3))
                {
                    w = dimsize[rank-1];
                    h = dimsize[rank-2];
                    retStr = getSDSImageData(sdsid, rank, SDInfo[1], dimsize, plane);
                }
            }
        }

        hdf.SDendaccess(sdsid);
        return retStr;
    } 

    /**
     *  Get the HDF  converted image by specified plane number
     *  @param sdsid   the SDS dataset identifier 
     *  @param sdsInfo the sds info. array
     *  @param dims    the sds dimension size
     *  @param plane   the plane number to be retrived.
     *  @return        the error message
     */
    private String getSDSImageData(int sdsid, int rank, int nt, int[] dims, int plane) throws HDFException
    {
        boolean hasPalette = false;
        int orientation = 0;

        if (nodeObject.sdsSelection != null)
            orientation = nodeObject.sdsSelection[0];
    
        // Is the rank valid?
        if ( (rank <= 1) || (rank >= 4) )
            return ("ERROR: rank "+rank+"is not supported.");

        int w = dims[rank-1];
        int h = dims[rank-2];
        if (orientation > 0 && rank >2)
        {
             h = dims[0];
             if (orientation == 2) w = dims[1];
        }

        Dimension originalImageSize = new Dimension(w, h);
        double maxmin[] = new double[2];
        double dataRange[] = null;
        if ( hdf.SDgetrange(sdsid, maxmin) )
        {
            dataRange = new double[2];
            dataRange[0] = maxmin[1];
            dataRange[1] = maxmin[0];
        }


        if ( (w < 0) || (h < 0) )
            return ("ERROR: invalid dimension");
    
        byte[] imagePalette = new byte[3*256];

        if (!hasPalette)
            imagePalette = createRainbowPalette();
   
        // read sds data        
        int start[] = {0, 0, 0};
        int stride[]= {1, 1, 1};
        int count[] = {1, 1, 1};

        int max = (h>w?h:w);
        int skip = max / nodeObject.preferedImageSize;
        if ( (skip>0) && ((w/skip)>0) && ((h/skip)>0) )
            skip++;
        else
            skip = 1;

        if ( nodeObject.isPreview )
        {
            if ( (rank == 3) && (nodeObject.sdsSelection != null) )
            {
                for (int kk=0; kk<rank; kk++)
                {
		    if (kk != orientation)
                    {
		        start[kk]  = 0;
		        stride[kk] = skip;
		        count[kk]  = dims[kk]/skip;
		    }
		    else
                    {
		        start[kk]  = nodeObject.sdsSelection[1];
		        stride[kk] = 1;
                        count[kk]  = 1;
		    }
	        }
            }
            else
            {
                stride[rank-2] = skip;
                stride[rank-1] = skip;
                count[rank-2] = dims[rank-2]/skip;
                count[rank-1] = dims[rank-1]/skip;
            }
            w = w/skip;
            h = h/skip;
        }
        else
        {
            w = Math.max(1, nodeObject.selectedRange.width);
            h = Math.max(1, nodeObject.selectedRange.height);
            switch (rank)
            {
                case 2:
                    // two dimension array
                    start[0] = nodeObject.selectedRange.y;
                    start[1] = nodeObject.selectedRange.x;
                    stride[0] = 1;
                    stride[1] = 1;
                    count[0] = h  ;  // y
                    count[1] = w  ;  //x
                    break;
                case 3:
                    // three dimesion
                    switch (orientation) 
                    {
                        case 0:
                            // z-axis
                            start[0] = plane-1;
                            start[1] = nodeObject.selectedRange.y;
                            start[2] = nodeObject.selectedRange.x;
                            count[1] = h; // y
                            count[2] = w; // x
                            break;
                        case 1: 
                            // y-axis
                            start[0] = nodeObject.selectedRange.y;
                            start[1] = plane-1;
                            start[2] = nodeObject.selectedRange.x;
                            count[0] = h; // z
                            count[2] = w; // x
                            break;
                        case 2:
                            // x-axis
                            start[0] = nodeObject.selectedRange.y;
                            start[1] = nodeObject.selectedRange.x;
                            start[2] = plane-1;
                            count[0] = h; // z
                            count[1] = w; // y
                            break;
                        default:
                            System.out.println("No Active Selected");
                    }
                    break;
                default:
                    System.out.println("Not support");
            }
        }

        int dataSize = w*h;
        Object imageData = defineDataObject(nt, dataSize);

        // load all images for aninmation
        if ( (rank==3) && (plane > dims[orientation]) )
        {
            int numberOfImages = dims[orientation];
            this.images = new byte[numberOfImages-1][w*h];
            boolean cvFlag = false;

            for (int i=1; i<numberOfImages; i++)
            {
                start[orientation] = i;
                if ( !hdf.SDreaddata(sdsid,start,stride,count,imageData) )
                    return ("ERROR: native call to HDFLibrary.SDreaddata() failed.");
                HDFObject.object2byte(imageData, nt, dataRange, this.images[i-1]);
            }
            return "";
        }
        else
        {
            // read flag
            if ( !hdf.SDreaddata(sdsid,start,stride,count,imageData) )
                return ("ERROR: native call to HDFLibrary.SDreaddata() failed.");
        }

        // set up HDFSDS variables
        this.originalImageSize = originalImageSize;
        this.selectedImageSize = new Dimension(w, h);
        this.imageData = imageData;
        this.imagePalette = imagePalette;
        int temp[] = {1, nt, rank};
        this.imageArgv = temp;
        if ( rank==3 ) this.imageArgv[0] = dims[orientation];
        this.dataRange = dataRange;

        return "";
    }


    /**
     * display the HDF file SDS global attributes
     *
     * @param sdid       the SDS identifier
     * @param attrIndex  the index of HDF file attributes
     * @return           the string of the global attribute.
     */  
    private String getHdfSDSgattr ( int sdid, int attrIndex ) throws HDFException
    {
        int kk = attrIndex;
        String attr = new String();        
        String lr = new String("");
        String semicolon = new String("; ");
    
        // for each of the global attribute 
        String name = new String("");
        String[] ss = new String[1];
        ss[0] = name;
        int[] attrInfo = new int[2];

        if ( hdf.SDattrinfo(sdid, kk, ss, attrInfo) )
        {
            name = ss[0];
            attr = attr.concat("\nName  : " + name);

            // read the attributes as bytes, use doattr() to convert to java types
            int attrSize = hdf.DFKNTsize(attrInfo[0])*attrInfo[1] + 1;
            byte[] buf = new byte[attrSize];

            if ( !hdf.SDreadattr(sdid, kk, buf) )
            {
                return (attr);
            }

            buf[attrSize-1] = '\0';
            attr = attr.concat("\nType  : " + hdf.HDgetNTdesc(attrInfo[0]));
            attr = attr.concat("\nCount : " + attrInfo[1]);
            attr = attr.concat("\nValue : " + doattrs( buf, attrSize, attrInfo[0], attrInfo[1]));
        }
        return attr;
    }

    /**
     *  display the HDF file SDS dataset attributes
     *
     *  @param sdid       the SDS identifier
     *  @param attrIndex  the index of HDF file attributes
     *  @param ref        the reference number of the HDF object
     *  @return           the string of the image attribute.
     */
    private String getHdfSDSattr(int sdid, int attrIndex, int ref) throws HDFException
    {
        int riid = -1;
        String attr = new String();        
        String lr = new String("");
        String semicolon = new String("; ");

        int index = hdf.SDreftoindex(sdid, ref);
        if (index == HDFConstants.FAIL)
            return ("ERROR: native call to HDFLibrary.SDreftoindex() failed");
    
        if ( (riid = hdf.SDselect(sdid,index)) == HDFConstants.FAIL )
        {
            hdf.SDendaccess(riid);
            return("ERROR: native call to HDFLibrary.SDselect() failed");
        }
    
        // for each of the global attribute 
        int kk = attrIndex; 
        String name = new String("");
        String[] ss = new String[1];
        ss[0] = name;
        int attrInfo[] = new int[2];

        if ( hdf.SDattrinfo(riid, kk, ss, attrInfo) )
        {
            name = ss[0];
            attr = attr.concat("\nName  : " + name);
            attr = attr.concat("\nType  : " + hdf.HDgetNTdesc( attrInfo[0]) );
            attr = attr.concat("\nCount : " + attrInfo[1]);
    
            int attrSize = hdf.DFKNTsize(attrInfo[0])*attrInfo[1] + 1;
            byte[] buf = new byte[attrSize];

            if ( !hdf.SDreadattr(riid, kk, buf) )
            {
                hdf.SDendaccess(riid);
                return(attr);
            }

            attr = attr.concat("\nValue : " + doattrs( buf, attrSize, attrInfo[0], attrInfo[1] ));
        }
        hdf.SDendaccess(riid);
        return attr;
    }

    /**
     *  define the data object
     *
     *  @param dataType  the type of the iamge data
     *  @param dataSize  the size of the image data array
     */
    private Object defineDataObject(int dataType, int dataSize)
    {
        Object imageData = null;

        switch(dataType)
        {
            // one byte data
            case HDFConstants.DFNT_CHAR:
            case HDFConstants.DFNT_UCHAR8:
            case HDFConstants.DFNT_UINT8:
            case HDFConstants.DFNT_INT8:
                imageData = new byte[dataSize];
                break;
            // short
            case HDFConstants.DFNT_INT16:
            case HDFConstants.DFNT_UINT16:
                imageData = new short[dataSize];
                break;
            case HDFConstants.DFNT_INT32:
            case HDFConstants.DFNT_UINT32:
                imageData = new int[dataSize];
                break;
            case HDFConstants.DFNT_INT64:
            case HDFConstants.DFNT_UINT64:
                imageData = new long[dataSize];
                break;
            case HDFConstants.DFNT_FLOAT32:
                imageData = new float[dataSize];
                break;
            case HDFConstants.DFNT_FLOAT64:
                imageData = new double[dataSize];
                break;
        }
        return imageData;
    }

    /**
     *  Process numeric attributes or strings, converting from bytes to Java types.
     *  This calls native routines to do the numeric conversions.
     */
    String doattrs( byte[] buf, int buffsize, int NT, int count ) throws HDFException
    {

        HDFNativeData convert = new HDFNativeData();
        String lr         = new String("\t");
        String semicolon  = new String("; ");
        String str = new String(" ");

        int incr = hdf.DFKNTsize(NT);
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
                    str = str.concat(semicolon);
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
                    str = str.concat(semicolon);
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
                    str = str.concat(semicolon);
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
                    str = str.concat(semicolon);
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
                    str = str.concat(semicolon);
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
                    str = str.concat(semicolon);
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
                    str = str.concat(semicolon);
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
                    str = str.concat(semicolon);
                }
                break;
        }
        return(str);
    }

    /**
     *  Return the RAINBOW palette
     *  rgb rgb rgb rgb rgb ....
     */
    protected byte[] createRainbowPalette()
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

    /** return the original image size */
    public Dimension getOriginalImageSize() { return originalImageSize; }

    /** return the requested image size */
    public Dimension getCurrentImageSize() { return selectedImageSize; }

    /** return the image data */
    public Object getImageData() { return imageData; }

    /** return the image palette data */
    public byte[] getImagePalette() { return imagePalette; }

    /** return the image arguments */
    public int[] getImageArgv() { return imageArgv; }

    /** return the iamge data range */
    public double[] getDataRange() { return dataRange; }

    /** return data for animation images */
    public byte[][] getImages() { return images; }

}
