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
 *  HDFGR holds HDF Generic Raster image information.
 *  It also serves as a base class of other image types
 
 *  @version   1.1.3 September 22 1997
 *  @author    Peter X. Cao
 */
public class HDFGR extends HDFObject
{
    /** the original image size */
    protected  Dimension originalImageSize;

    /** the requested image area */
    protected  Dimension selectedImageSize;

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

    /**
     *  Construct an HDFGR with specified HDFObjectNode and filename
     *
     *  @param node     the HDFObjectNode to be transfered to the server
     *  @param filename the requested HDF file
     */
    public HDFGR (HDFObjectNode node, String filename)
    {
        super(node, filename);
        originalImageSize = null;
        selectedImageSize = null;
        imagePalette = null;
        imageArgv = null;
    }

    public HDFGR ()
    { this(null, ""); }

    /**
     *  create a message for sending to the client
     *
     *  @return  The HDFMessage created by this object
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

        return message;
    }

    /**
     *  serve the client request on the server
     */
    public void service()
    {
        this.hdf =  new HDFLibrary();
        try { information = getGR (filename, nodeObject); }
        catch (Exception e) { information = "ERROR: exception in HDFGR.getGR()"; }
    }

    /**
     *  process the  message received from the server
     */
    public void fromServer(HDFMessage message)
    {
        information = (String) message.get("information");
        originalImageSize = (Dimension) message.get("originalImageSize");
        selectedImageSize = (Dimension) message.get("selectedImageSize");
        data = message.get("data");
        imagePalette = (byte[]) message.get("imagePalette");
        imageArgv = (int[]) message.get("imageArgv");
    }

    /**
     *  get HDF GR image information for the selected HDF file
     *
     *  @prarm hdf        the HDFLibrary
     *  @param filename   the string of the hdf file name
     *  @param node       the HDFObjectNode
     *  @return           the string containing the GR image information
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
        int    fid        = -1;
        int    grid       = -1;
        int    riid       = -1;
        int[]  argv       = new int[2];
        String attrBuf    = new String();
        String info       = new String();

        if ((fid = hdf.Hopen(filename)) <= 0)
        {
            return ("ERROR: Open " + filename + " failed.");
        }
   
        grid = hdf.GRstart(fid);
        if (grid == HDFConstants.FAIL)
        {
            hdf.GRend(grid);
            hdf.Hclose(fid);
            return ("ERROR: native call to HDFLibrary.GRstart() failed.");
        }

        // get HDF GR information
        if ( !hdf.GRfileinfo(grid, argv) )
        {
            hdf.GRend(grid);
            hdf.Hclose(fid);
            return ("ERROR: native call to HDFLibrary.GRfileinfo() failed.");
        }

        // GR number in HDF file
        if (request == HDFObjectNode.GR)
        { 
            info = info.concat("\nNumber of Dataset           : " + argv[0]);
            info = info.concat("\nNumber of Global Attributes : " + argv[1]);
        }
        else
        {   
            int ref   = requestRef;
            int index = hdf.GRreftoindex(grid, (short)ref);
            if (index == HDFConstants.FAIL)
            {
                 info = info.concat("\nERROR: native call to HDFLibrary.GRreftoindex() failed"); 
            }
            else
            {
                if ( (riid = hdf.GRselect(grid,index)) == HDFConstants.FAIL )
                {
                    hdf.GRendaccess(riid);
                    info = info.concat("\nERROR: native call to HDFLibrary.GRselect() failed"); 
                }
                else
                {                
                    String   gr_name   = new String(" "); 
                    String[] n         = new String[1];
                    int[]    imInfo    = new int[4];
                    int[]    dim_sizes = new int[2];  /* ??? */

                    n[0] = gr_name;
                    if ( hdf.GRgetiminfo( riid, n, imInfo, dim_sizes) )
                    {
                        gr_name = n[0];
        
                        // GR dataset name
                        info = info.concat("\nImage Name                  : " + gr_name);
                        info = info.concat("\nImage Index                 : " + Integer.toString(index));

                        // rank
                        String rankInfo = Integer.toString(dim_sizes[0]) + " X " + Integer.toString(dim_sizes[1]);
                        info = info.concat("\nImage Size                  : " + rankInfo);
                                
                        // nt
                        info = info.concat("\nData Number Type            : " + hdf.HDgetNTdesc(imInfo[1]));
                        info = info.concat("\nNum of Components           : " + imInfo[0]);
                        info = info.concat("\nInterlace                   : " + imInfo[2]);

                        // attributes
                        info = info.concat("\nNum of Attributes           : " + imInfo[3]);

                    } 
                    else
                    {
                        info = info.concat("\nERROR: native call to HDFLibrary.GRgetinfo() failed");
                    }
                } 
                hdf.GRendaccess(riid);
            } 
        }
  
        hdf.GRend(grid);
        hdf.Hclose(fid);
        return info;
    }

    /**
     *  get the selected HDF Generic raster image for the selected HDF file
     *
     *  @param filename   the string of the hdf file name
     *  @param node       the HDFObjectNode
     *  @return           the error message / GR attributes
     *
     *  @exception ncsa.hdf.hdflib.HDFException 
     *             should be thrown for errors in the
     *             HDF library call, but is not yet implemented.
     *
     */
    private String getGR (String filename, HDFObjectNode node)
        throws HDFException
    {
        int request = node.type;
        int ref = node.ref;
        int index = node.index;
        int grid = -1;
        int riid = -1;
        int fid  = -1;
        int[] fileInfo = new int[2];
        String info = new String();        

        if ((fid = hdf.Hopen(filename)) <= 0)
            return ("ERROR: Open " + filename + " failed.");

        grid = hdf.GRstart(fid);
        if (grid == HDFConstants.FAIL)
        {
            hdf.GRend(grid);
            hdf.Hclose(fid);
            return ("ERROR: native call to HDFLibrary.GRstart() failed.");
        }

        // get HDF GR information
        if (hdf.GRfileinfo(grid, fileInfo) == false)
        {
            hdf.GRend(grid);
            hdf.Hclose(fid);
            return ("ERROR: native call to HDFLibrary.GRfileinfo() failed.");
        }

        if (request == HDFObjectNode.GRGLOBALATTR)
            info = getHdfGRgattr(grid, fileInfo);
        else if (request == HDFObjectNode.GRDATASETATTR)
            info = getHdfGRattr(grid, fileInfo, index);
        else if (request == HDFObjectNode.GRDATASETAN)
            info = getObjectAnnotation(fid, HDFConstants.AN_DATA_DESC, node);
        else
            info = getGRImage(grid, index, ref);

        hdf.GRend(grid);
        hdf.Hclose(fid);
        return info;
    }

    /**
     *  get the HDF file generic raster image
     *
     *  @param grid       the GR identifier
     *  @param ref        the reference number of the selected HDF object node
     *  @param attrIndex  the index of HDF file attributes
     *  @return           the error message
     *
     *  @exception ncsa.hdf.hdflib.HDFException 
     *             should be thrown for errors in the
     *             HDF library call, but is not yet implemented.
     *
     */
    private String getGRImage(int grid, int attrIndex, int ref) throws HDFException
    {
        int riid = -1;
        String retStr = new String("");
        int w = 256;
        int h = 256;
        int index = hdf.GRreftoindex(grid, (short)ref);

        if (index == HDFConstants.FAIL)
        {
            retStr = "ERROR: native call to HDFLibrary.GRreftoindex() failed";
        }
        else if ((riid = hdf.GRselect(grid,index)) == HDFConstants.FAIL)
        {
            hdf.GRendaccess(riid);
            retStr = "ERROR: native call to HDFLibrary.GRselect() failed";
        }
        else
        { 
            String gr_name = new String("");
            String[] n = new String[1];
            n[0] = gr_name;
            int[]dim_sizes = new int[2];  /* ??? */
            int[] argv = new int[4];
  
            if (hdf.GRgetiminfo(riid, n, argv, dim_sizes) )
            {
                gr_name = n[0];
                w = dim_sizes[0];
                h = dim_sizes[1];
                int plane = 1;
                retStr = getGRImageData(riid, argv[0], argv[1], argv[2], w, h, plane);
            }
        } 
    
        hdf.GRendaccess(riid);
        return retStr;
    }

    /**
     *  Get the HDF  generic raster image by specified plane number
     *  @param riid    the raster image identifier 
     *  @param imgInfo the image info. array
     *  @param plane   the plane number to be retrived.
     *  @return        the error message
     *
     *  @exception ncsa.hdf.hdflib.HDFException 
     *             should be thrown for errors in the
     *             HDF library call, but is not yet implemented.
     *
     */
    private String getGRImageData(int riid, int ncomp, int nt, int interlace,
        int original_w, int original_h, int plane) throws HDFException
    {

        int w = original_w;
        int h = original_h;
        boolean hasPalette  = false;
        byte[] imagePalette = new byte[3*256];

        // set the interlace for reading by component interlacing scheme
        if ( !hdf.GRreqimageil(riid, HDFConstants.MFGR_INTERLACE_COMPONENT) )
        {
            return ("ERROR: native call to HDFLibrary.GRreqimageil() failed.");
        }
    
        // get palette info.
        int lutid = hdf.GRgetlutid(riid, 0);
        if (lutid == HDFConstants.FAIL)
           hasPalette = false;
        else
           hasPalette = true;

        // specify palette info.
        int[] lutInfo = new int[4];

        hdf.GRreqlutil(riid, HDFConstants.MFGR_INTERLACE_PIXEL);

        if ( !( hasPalette && hdf.GRreadlut(lutid, imagePalette) ) )
            imagePalette = createRainbowPalette();

        // read image data        
        int start[] = new int[2];
        int stride[]= new int[2];
        int count[] = new int[2];

        start[0] = 0;
        start[1] = 0;
        stride[0] = 1;
        stride[1] = 1;

        int max = (h>w?h:w);
        int skip = max / nodeObject.preferedImageSize;
        if ( !nodeObject.isPreview )
        {
            start[0] = nodeObject.selectedRange.x;
            start[1] = nodeObject.selectedRange.y;
            w = Math.max(1, nodeObject.selectedRange.width);
            h = Math.max(1, nodeObject.selectedRange.height);
        }
        else if ( (skip>0) && ((w/skip)>0) && ((h/skip)>0) )
        {
            skip++;
            stride[0] = skip;
            stride[1] = skip;
            w = w/skip;
            h = h/skip;
        }
        count[0] = w;
        count[1] = h;

        int dataSize = w*h*ncomp;
        Object data = defineDataObject(nt, dataSize);
        if ( !hdf.GRreadimage(riid,start,stride,count,data) )
            return ("ERROR: native call to HDFLibrary.GRreadimage() failed.");

        this.originalImageSize = new Dimension(original_w, original_h);
        this.selectedImageSize = new Dimension(w, h);
        this.data = data;
        this.imagePalette = imagePalette;
        this.imageArgv = new int[2];
        this.imageArgv[0] = ncomp;
        this.imageArgv[1] = nt;
        return "";
    }
  
    /**
     *  get the global attributes for GR image
     *
     * @param sdid       the GR identifier
     * @param fileInfo   the GR data information
     * @return           the string of the global attributes.
     *
     *  @exception ncsa.hdf.hdflib.HDFException 
     *             should be thrown for errors in the
     *             HDF library call, but is not yet implemented.
     *
     */
    private String getHdfGRgattr(int grid, int[] fileInfo )
        throws HDFException
    {
        String textInfo = "";
        int globalAttrNumber = fileInfo[1];

        textInfo = "\nGlobal Attributes";
        textInfo += "\n\nName";
        for (int j=0; j<MAX_NAME_LENGTH-4; j++) textInfo += " ";
        //textInfo += " Type";
        //for (int j=0; j<MAX_NAME_LENGTH-5; j++) textInfo += " ";
        textInfo += " Value\n";
        for (int j=0; j<80; j++) textInfo += "_";

        // for each global attribute
        for (int i=0; i<globalAttrNumber; i++)
        {
            // get attribute info.
            textInfo += "\n";
            String attrname = "";
            String attrtype = "";
            String[] ss = new String[1];
            ss[0] = attrname;
            int[] attrInfo = new int[2];
            if (hdf.GRattrinfo(grid, i, ss, attrInfo))
            {
                attrname = ss[0];
                attrtype = hdf.HDgetNTdesc(attrInfo[0]);

                int l = MAX_NAME_LENGTH - attrname.length();
                if (l > 0) for(int j=0; j<l; j++) attrname += " ";
                else attrname = attrname.substring(0, MAX_NAME_LENGTH);
                textInfo += attrname;

                //l = MAX_NAME_LENGTH - attrtype.length();
                //if (l > 0) for(int j=0; j<l; j++) attrtype += " ";
                //else attrtype = attrtype.substring(0, MAX_NAME_LENGTH);
                //textInfo += attrtype;

                int attrSize = hdf.DFKNTsize(attrInfo[0])*attrInfo[1] + 1;
                byte[] buf = new byte[attrSize];
                if ( !hdf.SDreadattr(grid, i, buf) ) continue;
                textInfo += doattrs( buf, attrSize, attrInfo[0], attrInfo[1]);
            }
        } // end of for (int i=0; i<globalAttrNumber; i++)

        return textInfo;
    }

    /**
     *  get the attributes for GR image
     *
     *  @param grid       the GR identifier
     *  @param fileInfo   the GR data information
     *  @param index      the index of the HDF data object
     *  @return           the string of the data attribute.
     *
     *  @exception ncsa.hdf.hdflib.HDFException 
     *             should be thrown for errors in the
     *             HDF library call, but is not yet implemented.
     *
     */
    private String getHdfGRattr(int grid, int[] fileInfo, int index)
        throws HDFException
    {
        String textInfo = "";

        int griid = hdf.GRselect(grid,index);
        if (griid == HDFConstants.FAIL) return textInfo;

        // get datsset info.
        int [] GRInfo = new int[4];
        String datasetname = "";
        String[] ss = new String[1];
        ss[0] = datasetname;
        int  dimsize[] = new int[32];
        if (hdf.GRgetiminfo(griid, ss, GRInfo, dimsize))
        {
            int datasetAttrNumber = GRInfo[3];
            int [] GRattrInfo = new int[2];

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
                if (hdf.GRattrinfo( griid, kk, ss, GRattrInfo))
                {
                    attrname = ss[0];
                    attrtype = hdf.HDgetNTdesc(GRattrInfo[0]);

                    int l = MAX_NAME_LENGTH - attrname.length();
                    if (l > 0) for(int j=0; j<l; j++) attrname += " ";
                    else attrname = attrname.substring(0, MAX_NAME_LENGTH);
                    textInfo += attrname;

                    //l = MAX_NAME_LENGTH - attrtype.length();
                    //if (l > 0) for(int j=0; j<l; j++) attrtype += " ";
                    //else attrtype = attrtype.substring(0, MAX_NAME_LENGTH);
                    //textInfo += attrtype;

                    int attrSize = hdf.DFKNTsize(GRattrInfo[0])*GRattrInfo[1] + 1;
                    byte[] buf = new byte[attrSize];
                    if ( !hdf.GRgetattr(griid, kk, buf) ) continue;
                    textInfo += doattrs( buf, attrSize, GRattrInfo[0], GRattrInfo[1]);
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
     *
     *  @exception ncsa.hdf.hdflib.HDFException 
     *             should be thrown for errors in the
     *             HDF library call, but is not yet implemented.
     *
     */
    private Object defineDataObject(int dataType, int dataSize)
    {
        Object data = null;

        switch(dataType)
        {
            // one byte data
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
            default:
            case HDFConstants.DFNT_CHAR:
            case HDFConstants.DFNT_UCHAR8:
            case HDFConstants.DFNT_UINT8:
            case HDFConstants.DFNT_INT8:
                data = new byte[dataSize];
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

}
