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
    // The maximum value of the rank parameter
    public final static int MAX_VAR_DIMS = 32;

    /** the original image size */
    protected  Dimension originalImageSize;

    /** the requested image size */
    protected  Dimension selectedImageSize;

    /** the iamge palette data */
    protected  byte imagePalette[];

    /** dimension units */
    protected String[] dimUnits;

    /** dimension scales */
    protected Object[] dimScales;

    /** dimension types */
    protected int[] dimTypes;

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
        imagePalette = null;
        imageArgv = null;
        dataRange = null;
        images = null;
        dimUnits = null;
        dimScales = null;
        dimTypes = null;
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
        data = null;
        imagePalette = null;
        imageArgv = null;
        dataRange = null;
        images = null;
        dimUnits = null;
        dimScales = null;
        dimTypes = null;
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
        if (data != null) message.put("data", data);
        if (imagePalette != null ) message.put("imagePalette", imagePalette);
        if (imageArgv != null ) message.put("imageArgv", imageArgv);
        if (dataRange != null ) message.put("dataRange", dataRange);
        if (images != null ) message.put("images", images);
        if (dimUnits != null ) message.put("dimUnits", dimUnits);
        if (dimScales != null ) message.put("dimScales", dimScales);
        if (dimTypes != null ) message.put("dimTypes", dimTypes);
        return message;
    }

    /**
     *  overwrite HDFGR
     */
    public void service()
    {
        //System.out.println("HDFSDS.service()");

        this.hdf = new HDFLibrary();
        try { information = getSDS (filename, nodeObject); }
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
        data = message.get("data");
        imagePalette = (byte[]) message.get("imagePalette");
        imageArgv = (int[]) message.get("imageArgv");
        dataRange = (double[]) message.get("dataRange");
        images = (byte[][]) message.get("images");
        dimUnits = (String[]) message.get("dimUnits");
        dimScales = (Object[]) message.get("dimScales");
        dimTypes = (int[]) message.get("dimTypes");
    }

    /**
     *  get HDF SDS image information for the selected HDF file
     *
     *  @prarm hdf        the HDFLibrary
     *  @param filename   the string of the hdf file name
     *  @param node       the HDFObjectNode
     *  @return           the string the HDF SDS image information
     *
     *  @exception ncsa.hdf.hdflib.HDFException 
     *             should be thrown for errors in the
     *             HDF library call, but is not yet implemented.
     *
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
                    int  dimsize[] = new int[MAX_VAR_DIMS];
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
     *
     *  @exception ncsa.hdf.hdflib.HDFException 
     *             should be thrown for errors in the
     *             HDF library call, but is not yet implemented.
     *
     */
    private String getSDS (String filename, HDFObjectNode node)
        throws HDFException
    {
        //System.out.println("HDFSDS.getSDS()");

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
            info = getHdfSDSgattr(sdid, fileInfo);
        else if (request == HDFObjectNode.SDSDATASETATTR)
            info = getHdfSDSattr(sdid, fileInfo, node.index);
        else
            info = getSDSData(sdid, attrIndex, ref, plane);

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
     *
     *  @exception ncsa.hdf.hdflib.HDFException 
     *             should be thrown for errors in the
     *             HDF library call, but is not yet implemented.
     *
     */
    private String getSDSData(int sdid, int attrIndex, int ref, int plane)
        throws HDFException
    {
        //System.out.println("HDFSDS.getSDSImag()");

        int sdsid = -1;
        int index = -1;
        String retStr = "";

        int orientation = 0;
        if (nodeObject.sdsSelection != null)
            orientation = nodeObject.sdsSelection[0];

        index = hdf.SDreftoindex(sdid, ref);
        if (index == HDFConstants.FAIL)
            return "ERROR: native call to HDFLibrary.SDreftoindex() failed";

        if ((sdsid = hdf.SDselect(sdid,index)) == HDFConstants.FAIL)
        {
            hdf.SDendaccess(sdsid);
            return "ERROR: native call to HDFLibrary.SDselect() failed";
        }

        // get sds info.
        int [] SDInfo = new int[3];
        String datasetname = new String("");
        String[] ss = new String[1];
        ss[0] = datasetname;
        int dimsize[] = new int[16];

        if (!hdf.SDgetinfo(sdsid, ss, dimsize, SDInfo))
        {
            hdf.SDendaccess(sdsid);
            return "ERROR: native call to HDFLibrary.SDgetinfo() failed";
        }

        //  type rank of the data set
        int nt = SDInfo[1];
        int rank = SDInfo[0];

        // only support rank=2 0r 3
        if ( (rank <= 1) || (rank >= 4) )
        {
            hdf.SDendaccess(sdsid);
            return ("ERROR: the viewer does not support SDS with rank "+rank);
        }

        // get the dimension information
        String[] dimStr = {"", "", ""};
        String dimName[] = {""};
        int[] dimInfo = new int[3];
        boolean ok = false;
        String[] dimUnits = new String[rank];
        Object[] dimScales = new Object[rank];
        int[] dimTypes = new int[rank];
        int dimid = -1;

        // do not load dimension information for preview images
        if (plane <= dimsize[orientation])
        {
            for (int i=0; i<rank; i++)
            {
                dimTypes[i] = nt;
                dimid = hdf.SDgetdimid(sdsid, i);
                ok = hdf.SDgetdimstrs(dimid, dimStr,20);
                if (ok) dimUnits[i] = dimStr[1];
                ok = hdf.SDdiminfo( dimid, dimName, dimInfo);
                if (ok && dimInfo[1]>0)
                {
                    dimTypes[i] = dimInfo[1];
                    dimScales[i] = defineDataObject(dimTypes[i], dimInfo[0]);
                    ok = hdf.SDgetdimscale(dimid, dimScales[i]);
                }
                else dimScales[i] = null;
            }
        }

        datasetname = ss[0];
        boolean hasPalette = false;

        int w = dimsize[rank-1];
        int h = dimsize[rank-2];

        if (orientation > 0 && rank >2)
        {
             h = dimsize[0];
             if (orientation == 2) w = dimsize[1];
        }

        if ( (w < 0) || (h < 0) )
        {
            hdf.SDendaccess(sdsid);
            return ("ERROR: width/height is less than zero");
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
        else skip = 1;

        if ( nodeObject.isPreview )
        {
            if ( (rank >= 3) && (nodeObject.sdsSelection != null) )
            {
               // 3 or more dimensions
                for (int kk=0; kk<rank; kk++)
                {
		    if (kk != orientation)
                    {
		        start[kk]  = 0;
		        stride[kk] = skip;
		        count[kk]  = dimsize[kk]/skip;
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
                // two dimension
                stride[rank-2] = skip;
                stride[rank-1] = skip;
                count[rank-2] = dimsize[rank-2]/skip;
                count[rank-1] = dimsize[rank-1]/skip;
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
            }
        }

        int dataSize = w*h;
        Object data = defineDataObject(nt, dataSize);

        // load all images for aninmation
        if ( (rank==3) && (plane > dimsize[orientation]) )
        {
            int numberOfImages = dimsize[orientation];
            this.images = new byte[numberOfImages-1][w*h];
            boolean cvFlag = false;

            for (int i=1; i<numberOfImages; i++)
            {
                start[orientation] = i;
                if ( !hdf.SDreaddata(sdsid,start,stride,count,data) )
                {
                    hdf.SDendaccess(sdsid);
                    return ("ERROR: native call to HDFLibrary.SDreaddata() failed.");
                }
                HDFObject.object2byte(data, nt, dataRange, this.images[i-1]);
            }
        }
        else
        {
            // read data for the chosen plane

            if ( !hdf.SDreaddata(sdsid,start,stride,count,data) )
            {
                hdf.SDendaccess(sdsid);
                return ("ERROR: native call to HDFLibrary.SDreaddata() failed.");
            }

            this.originalImageSize = originalImageSize;
            this.selectedImageSize = new Dimension(w, h);
            this.data = data;
            this.imagePalette = imagePalette;
            int temp[] = {1, nt, rank};
            this.imageArgv = temp;
            if ( rank==3 ) this.imageArgv[0] = dimsize[orientation];
            this.dataRange = dataRange;

            // dimension information
            this.dimUnits = dimUnits;
            this.dimScales = dimScales;
            this.dimTypes = dimTypes;
        }

        hdf.SDendaccess(sdsid);
        return "";
    }


    /**
     * display the HDF file SDS global attributes
     *
     * @param sdid       the SDS identifier
     * @param fileInfo   the SDS data information
     * @return           the string of the global attributes.
     *
     *  @exception ncsa.hdf.hdflib.HDFException 
     *             should be thrown for errors in the
     *             HDF library call, but is not yet implemented.
     *
     */
    private String getHdfSDSgattr ( int sdid, int[] fileInfo)
        throws HDFException
    {
        String textInfo = "";
        int sdsGlobalAttrNumber = fileInfo[1];

        textInfo = "\nDataset Global Attributes";
        textInfo += "\n\nName";
        for (int j=0; j<MAX_NAME_LENGTH-4; j++) textInfo += " ";
        textInfo += " Value\n";
        for (int j=0; j<80; j++) textInfo += "_";

        // for each global attribute
        for (int i=0; i<sdsGlobalAttrNumber; i++)
        {
            // get attribute info.
            textInfo += "\n";
            String attrname = "";
            String attrtype = "";
            String[] ss = new String[1];
            ss[0] = attrname;
            int[] SDattrInfo = new int[2];
            if (hdf.SDattrinfo(sdid, i, ss, SDattrInfo))
            {
                attrname = ss[0];
                attrtype = hdf.HDgetNTdesc(SDattrInfo[0]);

                int l = MAX_NAME_LENGTH - attrname.length();
                if (l > 0) for(int j=0; j<l; j++) attrname += " ";
                else attrname = attrname.substring(0, MAX_NAME_LENGTH);
                textInfo += attrname;

                //l = MAX_NAME_LENGTH - attrtype.length();
                //if (l > 0) for(int j=0; j<l; j++) attrtype += " ";
                //else attrtype = attrtype.substring(0, MAX_NAME_LENGTH);
                //textInfo += attrtype;

                int attrSize = hdf.DFKNTsize(SDattrInfo[0])*SDattrInfo[1] + 1;
                byte[] buf = new byte[attrSize];
                if ( !hdf.SDreadattr(sdid, i, buf) ) continue;
                textInfo += doattrs( buf, attrSize, SDattrInfo[0], SDattrInfo[1]);
            }
        } // end of for (int i=0; i<sdsGlobalAttrNumber; i++)

        return textInfo;
    }

    /**
     *  display the HDF file SDS dataset attributes
     *
     *  @param sdid       the SDS identifier
     *  @param fileInfo   the SDS data information
     *  @param index      the index of the HDF data object
     *  @return           the string of the data attribute.
     *
     *  @exception ncsa.hdf.hdflib.HDFException 
     *             should be thrown for errors in the
     *             HDF library call, but is not yet implemented.
     *
     */
    private String getHdfSDSattr(int sdid, int[] fileInfo, int index)
        throws HDFException
    {
        String textInfo = "";

        int sdsid = hdf.SDselect(sdid,index);
        if (sdsid == HDFConstants.FAIL) return textInfo;

        // check if the given SDS is the dimension scale
        if (hdf.SDiscoordvar(sdsid)) {
            hdf.SDendaccess(sdsid);
            return textInfo;
        }

        // get datsset info.
        int [] SDInfo = new int[3];
        String datasetname = new String(" ");
        String[] ss = new String[1];
        ss[0] = datasetname;
        int  dimsize[] = new int[10];
        if (hdf.SDgetinfo(sdsid, ss, dimsize, SDInfo))
        {
            int datasetAttrNumber = SDInfo[2];
            int long_name_idx = hdf.SDfindattr(sdsid, "long_name");
            int [] SDattrInfo = new int[2];
            hdf.SDattrinfo(sdsid, long_name_idx, ss,SDattrInfo);

            textInfo = "\nDataset Attributes";
            textInfo += "\n\nName";
            for (int j=0; j<MAX_NAME_LENGTH-4; j++) textInfo += " ";
            //textInfo += " Type";
            //for (int j=0; j<MAX_NAME_LENGTH-5; j++) textInfo += " ";
            textInfo += " Value\n";
            for (int j=0; j<80; j++) textInfo += "_";

            // for each dataset attribute
            for (int kk=0; kk<datasetAttrNumber; kk++)
            {
                // get attribute info.
                textInfo += "\n";
                String attrname = "";
                String attrtype = "";
                ss[0] = attrname;
                if (hdf.SDattrinfo( sdsid, kk, ss, SDattrInfo))
                {
                    attrname = ss[0];
                    attrtype = hdf.HDgetNTdesc(SDattrInfo[0]);

                    int l = MAX_NAME_LENGTH - attrname.length();
                    if (l > 0) for(int j=0; j<l; j++) attrname += " ";
                    else attrname = attrname.substring(0, MAX_NAME_LENGTH);
                    textInfo += attrname;

                    //l = MAX_NAME_LENGTH - attrtype.length();
                    //if (l > 0) for(int j=0; j<l; j++) attrtype += " ";
                    //else attrtype = attrtype.substring(0, MAX_NAME_LENGTH);
                    //textInfo += attrtype;

                    int attrSize = hdf.DFKNTsize(SDattrInfo[0])*SDattrInfo[1] + 1;
                    byte[] buf = new byte[attrSize];
                    if ( !hdf.SDreadattr(sdsid, kk, buf) ) continue;
                    textInfo += doattrs( buf, attrSize, SDattrInfo[0], SDattrInfo[1]);
                }
            } // end of for (int kk=0; kk<datasetAttrNumber; kk++)
        }
        return textInfo;
    }

    /**
     *  define the data object
     *
     *  @param dataType  the type of the iamge data
     *  @param dataSize  the size of the image data array
     */
    private Object defineDataObject(int dataType, int dataSize)
    {
        Object data = null;

        switch(dataType)
        {
            // one byte data
            case HDFConstants.DFNT_CHAR:
            case HDFConstants.DFNT_UCHAR8:
            case HDFConstants.DFNT_UINT8:
            case HDFConstants.DFNT_INT8:
                data = new byte[dataSize];
                break;
            // short
            case HDFConstants.DFNT_INT16:
            case HDFConstants.DFNT_UINT16:
                data = new short[dataSize];
                break;
            case HDFConstants.DFNT_INT32:
            case HDFConstants.DFNT_UINT32:
                data = new int[dataSize];
                break;
            case HDFConstants.DFNT_INT64:
            case HDFConstants.DFNT_UINT64:
                data = new long[dataSize];
                break;
            case HDFConstants.DFNT_FLOAT32:
                data = new float[dataSize];
                break;
            case HDFConstants.DFNT_FLOAT64:
                data = new double[dataSize];
                break;
        }
        return data;
    }

    /** return the original image size */
    public Dimension getOriginalImageSize() { return originalImageSize; }

    /** return the requested image size */
    public Dimension getCurrentImageSize() { return selectedImageSize; }

    /** return the image palette data */
    public byte[] getImagePalette() { return imagePalette; }

    /** return the image arguments */
    public int[] getImageArgv() { return imageArgv; }

    /** return the iamge data range */
    public double[] getDataRange() { return dataRange; }

    /** return data for animation images */
    public byte[][] getImages() { return images; }

    /** return dimension units */
    public String[] getDimUnits() { return dimUnits; }

    /** return dimension types */
    public int[] getDimTypes() { return dimTypes; }

    /** return dimension scales */
    public Object[] getDimScales() { return dimScales; }

}
