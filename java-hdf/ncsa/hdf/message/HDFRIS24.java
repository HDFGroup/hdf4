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

/**
 *  HDFRIS24 holds HDF RIS24 information
 
 *  @version   1.1.3 September 22 1997
 *  @author    Peter X. Cao
 */
public class HDFRIS24 extends HDFGR
{
    /**
     *  Constructs an HDFRIS24 with default parameters
     */
    public HDFRIS24 ()
    {
        super();
    }

    /**
     *  Construct an HDFRIS24 with specified HDFObjectNode and filename
     *
     *  @param node     the HDFObjectNode to be transfered to the server
     *  @param filename the requested HDF file
     */
    public HDFRIS24 (HDFObjectNode node, String filename)
    {
        super(node, filename);
    }

    /**
     *  create a message for sending to the client
     *
     *  @return  the HDFMessage created by this object
     */
    public HDFMessage toClient()
    {
        HDFMessage message = super.toClient();
        message.setOwner(getClass().getName());
        return message;
    }

    /**
     *  overwrites HDFGR
     */
    public void service()
    {
        this.hdf =  new HDFLibrary();
        try { information = getRis24 (filename, nodeObject); }
        catch (Exception e) { information = "ERROR: exception in HDFRIS24.getRis24()"; }
    }

    /**
     *  get HDF 24-bit raster image information for the selected HDF file
     *
     *  @prarm hdf        the HDFLibrary
     *  @param filename   the string of the hdf file name
     *  @param node       the HDFObjectNode
     *  @return           the string the HDF 24-bit raster image information
     *
     *  @exception ncsa.hdf.hdflib.HDFException 
     *             should be thrown for errors in the
     *             HDF library call, but is not yet implemented.
     *
     */
    public static String readInfo (HDFLibrary hdf, String filename,
        HDFObjectNode node) throws HDFException
    {
        int argv[] = new int[3];
        int numberOfImage = hdf.DF24nimages(filename);
        String  info = new String("");

        // number of images in the HDF file
        info = info.concat("\nNumber of images : " + Integer.toString(numberOfImage));
    
        // for each image object, list image name & dimension
        hdf.DF24restart();
    
        while (hdf.DF24getdims(filename,argv))
        {
            int w         = argv[0];
            int h         = argv[1];
            int interlace = argv[2];
            int ref       = hdf.DF24lastref();

            info = info.concat("\nReference number : " + Integer.toString(ref));
            info = info.concat("\nDimension        : " + Integer.toString(w) + " X " +Integer.toString(h));
            info = info.concat("\nInterlace        : " + Integer.toString(interlace));
        }

        return info;
    }

    /**
     *  get the selected HDF 24-bit raster image for the selected HDF file
     *
     *  @param filename   the string of the hdf file name
     *  @param node       the HDFObjectNode
     *  @return           the error message or HDFRIS24 information
     *
     *  @exception ncsa.hdf.hdflib.HDFException 
     *             should be thrown for errors in the
     *             HDF library call, but is not yet implemented.
     *
     */
    private String getRis24 (String filename, HDFObjectNode node)
        throws HDFException
    {
        int        request     = node.type;
        int        ref         = node.ref;
        int        attrIndex   = node.index;
        int       argv[]  = new int[3];

        // set the reference number
        if ( !hdf.DF24readref(filename, ref) )
        {
            return ("ERROR: native call to HDFLibrary.DF24readref() failed.");
        }

        // get HDF image information
        if ( !hdf.DF24getdims(filename,argv) )
        {
            return ("ERROR: native call to HDFLibrary.DF24getdims() failed.");
        }

        int imageWidth      = argv[0];
        int imageHeight     = argv[1];
        int  interlace      = argv[2];
        byte[] data    = new byte[imageWidth*imageHeight*3];
        /*
        byte[] imagePalette = new byte[256*3];
        for (int i=0; i<256; i++)
	    for (int j=0; j<3; j++)
	        imagePalette[i*3+j] = (byte)i;
        */


        // set the interlace for reading by component interlacing scheme
        hdf.DF24reqil(HDFConstants.MFGR_INTERLACE_COMPONENT);

        // read the image
        if ( !hdf.DF24getimage(filename, data,imageWidth, imageHeight) )
        {
            return ("ERROR: native call to HDFLibrary.DF24getimage() failed.");
        }

        this.originalImageSize = new Dimension(imageWidth, imageHeight);
        this.selectedImageSize = this.originalImageSize;
        this.imagePalette = new byte[256*3];
        this.data = byte24to8(data, imageWidth, imageHeight, this.imagePalette);
        //this.data = data;
        int temp[] = {1, HDFConstants.DFNT_UINT8};
        this.imageArgv = temp;
        return "";
    }

    /**
     *  convert 24-bit image data to 8-bit image data
     *
     *  @param p24     the byte array of the 24-bit image data
     *  @param w       the image width
     *  @param h       the image height
     *  @param palette the image palette
     *
     *  @return the byte array of the 8-bit image data
     */
    public byte[] byte24to8 (byte[] p24, int w, int h, byte[] palette)
    {
        /* up to 255 colors: 3 bits for red, 3 bits for green and 2 bits for blue*/
        short rmask = 0xe0;
        short rshift = 0;
        short gmask = 0xe0;
        short gshift = 3;
        short bmask = 0xc0;
        short bshift = 6;

        byte[] p8;
        if (p24 == null || w<=0 || h<=0) return null;

        p8 = new byte[w*h];
        if (palette == null || palette.length < 768)
            palette = new byte[256*3];

        for (short i=0; i<256; i++)
        {
            palette[i*3]   = (byte) ((((i<<rshift)&rmask)*255+rmask/2)/rmask);
            palette[i*3+1] = (byte) ((((i<<gshift)&gmask)*255+gmask/2)/gmask);
            palette[i*3+2] = (byte) ((((i<<bshift)&bmask)*255+bmask/2)/bmask);
        }

        int size = w*h;
        byte r, g, b;
        for (int i=0; i < size; i++)
        {
            r = p24[i];
            g = p24[size+i];
            b = p24[2*size+i];

            p8[i] = (byte)( (r&(rmask>>rshift))|
                            (g&(gmask>>gshift))|
                            (b&(bmask>>bshift)) );
            //p8[i] = (byte)((((r&rmask)>>rshift)|
            //                ((g&gmask)>>gshift)|
            //                ((b&bmask)>>bshift)));

//System.out.println(r +" "+g+" "+b);

        }


        return p8;
    }

}
