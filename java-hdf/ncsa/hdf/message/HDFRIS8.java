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
 *  HDFRIS8 holds HDF RIS8 information
 
 *  @version   1.1.3 September 22 1997
 *  @author    Peter X. Cao
 */
public class HDFRIS8 extends HDFGR
{
    /**
     *  Constructs an HDFRIS8 with default parameters
     */
    public HDFRIS8 ()
    {
        super();
    }

    /**
     *  Construct an HDFRIS8 with specified HDFObjectNode and filename
     *
     *  @param node     the HDFObjectNode to be transfered to the server
     *  @param filename the requested HDF file
     */
    public HDFRIS8 (HDFObjectNode node, String filename)
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
        //System.out.println("HDFRIS8::service()");
        this.hdf =  new HDFLibrary();
        try { information = getRis8 (filename, nodeObject); }
        catch (Exception e) { information = "ERROR: exception in HDFRIS8.getRis8()"; }
    }

    /**
     *  get HDF RIS8 image information for the selected HDF file
     *
     *  @prarm hdf        the HDFLibrary
     *  @param filename   the string of the hdf file name
     *  @param node       the HDFObjectNode
     *  @return           the string containing the 8-bit raster image information
     *
     *  @exception ncsa.hdf.hdflib.HDFException 
     *             should be thrown for errors in the
     *             HDF library call, but is not yet implemented.
     *
     */
    public static String readInfo (HDFLibrary hdf, String filename,
        HDFObjectNode node) throws HDFException
    {
        int     argv[]        = new int[2];
        boolean argB[]        = new boolean[1];
        int     numberOfImage = hdf.DFR8nimages(filename);
        String  info          = new String("");
    
        // image number in HDF file
        info = info.concat("\nNumber of images   : " + Integer.toString(numberOfImage));
    
        // for each image object, list image name & dimension
        hdf.DFR8restart();
    
        while (hdf.DFR8getdims(filename,argv,argB))
        {
            int     w          = argv[0];
            int     h          = argv[1];
            boolean hasPalette = argB[0];
            int     ref        = hdf.DFR8lastref();
        
            info = info.concat("\nReference number   : " + Integer.toString(ref));
            info = info.concat("\nDimension          : " + Integer.toString(w) + " X " +Integer.toString(h));
            if (hasPalette)
                info = info.concat("\nAssociated palette : TRUE");
            else
                info = info.concat("\nAssociated palette : FALSE\n");
        }

        return info;
    }

    /**
     *  get the selected HDF RIS8 for the selected HDF file
     *
     *  @param filename   the string of the hdf file name
     *  @param node       the HDFObjectNode
     *  @return           the error message or HDFRIS8 information
     *
     *  @exception ncsa.hdf.hdflib.HDFException 
     *             should be thrown for errors in the
     *             HDF library call, but is not yet implemented.
     *
     */
    private String getRis8 (String filename, HDFObjectNode node)
        throws HDFException
    {
        //System.out.println("HDFRIS8::getRis8()");

        int     request      = node.type;
        int     ref          = node.ref;
        int     argv[]       = new int[2];
        boolean argB[]       = new boolean[1];
        boolean hasPalette   = false;
        int     imageWidth   = 0;
        int     imageHeight  = 0;
        byte[]  data    = null;
        byte[]  imagePalette = null;

        if ( !hdf.DFR8readref(filename, ref) )
            return ("ERROR: native call to HDFLibrary.DFR8readref() failed.");

        // get HDF image information
        if ( !hdf.DFR8getdims(filename,argv,argB) )
            return ("ERROR: native call to HDFLibrary.DFR8getdims() failed.");
        hasPalette   = argB[0];
        imageWidth   = argv[0];
        imageHeight  = argv[1];
        data    = new byte[imageWidth*imageHeight];
        imagePalette = new byte[256*3];

        // read the image
        if ( !hdf.DFR8getimage(filename, data,imageWidth,imageHeight,imagePalette) )
        {
            return ("ERROR: native call to HDFLibrary.DFR8getimage() failed.");
        }
        this.originalImageSize = new Dimension(imageWidth, imageHeight);
        this.selectedImageSize = new Dimension(imageWidth, imageHeight);
        this.data = data;
        this.imagePalette = imagePalette;
        int temp[] = {1, HDFConstants.DFNT_UINT8};
        this.imageArgv = temp;

        return "";
    }

}
