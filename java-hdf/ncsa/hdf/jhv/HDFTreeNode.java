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

package ncsa.hdf.jhv;

import java.awt.*;
import java.awt.event.*;
import java.awt.image.*;
import java.net.*;
import java.applet.*;
import java.io.File;
import java.io.InputStream;
import java.io.IOException;
import java.util.*;
import java.net.*;

import ncsa.hdf.hdflib.*;
import ncsa.hdf.util.*;
import ncsa.hdf.awt.image.*;
import ncsa.hdf.message.*;


/** This class is actual HDF tree node derived from the HDFTreeNode and
 *  used by JHV. This class also define a bunch of methods to
 *  do something like the HDF operation
 *
 *  Date: 3-26-97
 *  For attribute of an HDF objects, add "Number type" & "Count" info.
 *  Need changed to the following methods:
 *    getHdfSDSgattr(), getHdfSDSattr(),
 *    getHdfGRgattr(),  getHdfGRattr(),
 *  Date: 6-5-97
 *    Take UINT8 same as INT8.
 *
 *  Modifications:
 *     1. Separate hdf file access from the UI. 01/22/98, Peter Cao
 *     2. Support remote file access. 01/22/98, Peter Cao
 *     3. Add documentation. 01/22/98, Peter Cao
 */

public class HDFTreeNode extends TreeNode
{
    public static final int PREFEREDIMAGESIZE = 256;

    JHV applet_;
    HDFLibrary hdf;
    HDFObjectNode node;
    Image image = null;
    Image paletteImage = null;

    // skip value when read a large HDF file (subsample)
    int skip=1;
    SliceSelection sliceSelection;
    SDSDimInfo sliceInfo;

    /**
     * create the new HDF Tree node
     * @param obj  the generic object
     * @param img  the default icon for this node
     * @param app  the applet of this node that belongs to
     */
    public HDFTreeNode(Object obj, Image img, JHV app)
    {
        super(obj, img);
        applet_ = app;
    }

    /**
     * create the new HDF Tree node for the node who has the child
     * @param obj          the generic object
     * @param img          the default icon for this node
     * @paramimgCollapsed  the icon for opened file folder
     * @param app          the applet of this node that belongs to
     */
    public HDFTreeNode(Object obj, Image img, Image imgCollapsed, JHV app)
    {
        super(obj, img, imgCollapsed);
        applet_ = app;
    }


    /** clean up  the image panel */
    public void eraseImage() {
        applet_.hdfCanvas.setImage(null);
        applet_.hdfCanvas.repaint();
    }

    /**
     *  creates a raster image
     *
     *  @param imgData   the byte array of image data
     *  @param w         the image width
     *  @param h         the image height
     *  @param imgPal    the byte array of the image palette
     *  @return          the raster image
    */
    public Image createRasterImage(byte[]  imgData, int w, int h,byte[] imgPal)
    {
        ImageDataConverter convert = new ImageDataConverter();
        return(convert.createRasterImage(imgData,w,h,imgPal));
    }

    /**
     *  create the palette image
     *  @param pal the palette value
     *  @return the image , otherwise null
     */
    public Image createPaletteImage(byte[] pal)
    {
        byte[] palImageData = new byte[256*18];

        for (int i=0; i<256; i++) {
            for (int j=0; j<6; j++ ) {
                palImageData[i*12+j]    = (byte)i;
                palImageData[i*12+j+6] = (byte)i;
                palImageData[i*12+j+12] = (byte)i;
            }
        }

        return(  createRasterImage(palImageData,12,256,pal));
    }

    /**
     *  select one HDF Tree node, this node is expandable(has one child at least)
     *  @param tree the HDF Tree
     */
    public void expandCollapse(HDFTree tree)
    {
        super.expandCollapse(tree);
        eraseImage();
        HDFObjectNode  node = (HDFObjectNode)(tree.selectedNode.userObject);
        applet_.jhvFrame.setCursor(new Cursor(Cursor.WAIT_CURSOR));
        dispMessage(node.information);

        // display 8bit raster images in animated format
        if (node.getObjType() == node.RIS8)
        {
            try { dispHdfRis8Images(applet_.dispMode, node); }
            catch (Exception ex) {}
        }
        applet_.jhvFrame.setCursor(new Cursor(Cursor.DEFAULT_CURSOR));

    }

    /**
     * Display the text message
     * @param msg the message to be displayed
     */
    public void dispMessage(String msg)
    {
        if (msg == null) return;

        applet_.infoText.setText(msg);
        int pos = applet_.infoText.getText().length() - 1;
        if (pos<0) pos = 0;
        applet_.infoText.select(pos,pos);
    }

    /**
     *  get the HDF message from server
     *
     *  @return            The HDFObject containing the requested data
     *  @auther            Peter Cao (xcao@ncsa.uiuc.edu), 10/2/97
     */
    public HDFObject getHDFObject()
    {
        HDFTree tree = applet_.getTree();
        return applet_.getHDFObject((HDFObjectNode)(tree.selectedNode.userObject));
    }

    /**
     *  display an HDFObject received from a remote machine
     *
     *  @param tree        The HDFTree of the HDF hierarchy
     *  @param modifier    The display modifier
     *  @param jhv         The JHV viewer
     *  @auther            Peter Cao (xcao@ncsa.uiuc.edu), 10/2/97
     */
    public void select(HDFTree tree)
    {
        JHV jhv = this.applet_;
        eraseImage();

        jhv.jhvFrame.setCursor(new Cursor(Cursor.WAIT_CURSOR));
        node = (HDFObjectNode)(tree.selectedNode.userObject);
        node.preferedImageSize = PREFEREDIMAGESIZE;
        jhv.hdfCanvas.setObject(node);
        HDFObject hdfObject = jhv.getHDFObject(node);

        String info = hdfObject.getInformation();
        if (info!=null)
        {
            info = info.trim();
            if (info.length()>0) jhv.infoText.setText(info);
        }

        try
        {
            switch(node.type)
            {
                case HDFObjectNode.Annotation:
                    dispHdfAnnotation(hdfObject);
                    break;
                case HDFObjectNode.RIS8:
                    dispHdfRis8(hdfObject);
                    break;
                case HDFObjectNode.RIS24:
                    dispHdfRis24(hdfObject);
                    break;
                case HDFObjectNode.GRDATASET:
                    dispHdfGR(hdfObject);
                    break;
                case HDFObjectNode.GRGLOBALATTR:
                case HDFObjectNode.GRDATASETATTR:
                case HDFObjectNode.GRDATASETAN:
                    jhv.infoText.setText(((HDFGR) hdfObject).getInformation());
                    jhv.hdfCanvas.setImage(null);
                    jhv.hdfCanvas.repaint();
                    break;
                case HDFObjectNode.SDSDATASET:
                    dispHdfSDS(hdfObject);
                    break;
                case HDFObjectNode.SDSGLOBALATTR:
                case HDFObjectNode.SDSDATASETATTR:
                case HDFObjectNode.SDSDATASETAN:
                    jhv.infoText.setText(((HDFSDS) hdfObject).getInformation());
                    jhv.hdfCanvas.setImage(null);
                    jhv.hdfCanvas.repaint();
                    break;
                case HDFObjectNode.Vdata:
                    HDFVdata vdata = (HDFVdata) hdfObject;
                    if (jhv.vdataFrameDisplayed && jhv.vdataFrame != null)
                        jhv.vdataFrame.setup(jhv, vdata);
                    else
                    {
                        jhv.vdataFrameDisplayed = true;
                        jhv.vdataFrame = new JHVVdataFrame(jhv, vdata);
                    }
                    break;
                default:
                    break;
            }
        }
        catch (Exception ex) { jhv.jhvFrame.setCursor(new Cursor(Cursor.DEFAULT_CURSOR));}
        jhv.jhvFrame.setCursor(new Cursor(Cursor.DEFAULT_CURSOR));
        
        //System.gc();
    }

    /**
     *  display annontation received from a remote machine
     *
     *  @param hdfObject the HDFObject received from the remote machine
     *  @auther          Peter Cao (xcao@ncsa.uiuc.edu), 10/2/97
     */
    public void dispHdfAnnotation(HDFObject hdfObject)
    {
        HDFAnnotation annotation = (HDFAnnotation) hdfObject;
        applet_.infoText.setText(annotation.getAnnotation());
    }

    /**
     *  display 8-bit raster image
     *
     *  @param hdfObject the HDFObject received from the remote machine
     *  @auther          Peter Cao (xcao@ncsa.uiuc.edu), 10/2/97
     */
    public void dispHdfRis8(HDFObject hdfObject)
    {
        //System.out.println("HDFTreeNode::dispHdfRis8()");
        HDFRIS8 ris8 = (HDFRIS8) hdfObject;
        byte[] imagePalette = ris8.getImagePalette();
        Object ris8Data = ris8.getData();
        int imageArgv[] = ris8.getImageArgv();
        int dataType = imageArgv[1];
        Dimension imageSize = ris8.getCurrentImageSize();
        int w = imageSize.width;
        int h = imageSize.height;
        byte[] imageData = new byte[w*h];
        boolean cvFlag = HDFObject.object2byte(ris8Data, dataType, null, 0, imageData);
        Image palImage = createPaletteImage(imagePalette);
        Image img = createRasterImage(imageData,w,h,imagePalette);

        if (img != null)
        {
            applet_.hdfCanvas.setOriginalImageSize(w,h);
            applet_.hdfCanvas.setImage(img, palImage);
            applet_.hdfCanvas.repaint();
        }
    }

    /**
     *  display 24-bit raster image received from a remote machine
     *
     *  @param hdfObject the HDFObject received from the remote machine
     *  @auther          Peter Cao (xcao@ncsa.uiuc.edu), 10/2/97
     */
    public void dispHdfRis24(HDFObject hdfObject)
    {
        HDFRIS24 ris24 = (HDFRIS24) hdfObject;
        byte[] imagePalette = ris24.getImagePalette();

        Object ris24Data = ris24.getData();
        int imageArgv[] = ris24.getImageArgv();
        int dataType = imageArgv[1];
        Dimension imageSize = ris24.getCurrentImageSize();
        int w = imageSize.width;
        int h = imageSize.height;
        //byte[] imageData = new byte[w*h*3];
        byte[] imageData = new byte[w*h];
        boolean cvFlag = HDFObject.object2byte(ris24Data, dataType, null, 0, imageData);
        Image palImage = createPaletteImage(imagePalette);
        Image img = createRasterImage(imageData,w,h,imagePalette);
        if (img != null)
        {
            applet_.hdfCanvas.setOriginalImageSize(w,h);
            applet_.hdfCanvas.setImage(img,palImage);
            applet_.hdfCanvas.repaint();
        }
    }

    /**
     *  display generic raster image received from a remote machine
     *
     *  @param hdfObject the HDFObject received from the remote machine
     *  @auther          Peter Cao (xcao@ncsa.uiuc.edu), 10/2/97
     */
    public void dispHdfGR(HDFObject hdfObject)
    {
        HDFGR gr = (HDFGR) hdfObject;
        byte[] imagePalette = gr.getImagePalette();
        Object grData = gr.getData();
        int imageArgv[] = gr.getImageArgv();
        int dataType = imageArgv[1];
        Dimension imageSize = gr.getCurrentImageSize();
        int w = imageSize.width;
        int h = imageSize.height;
        byte[] imageData = new byte[w*h];
        double range[] = null;
        boolean cvFlag = HDFObject.object2byte(grData, dataType, range, 0, imageData);
        Image palImage = createPaletteImage(imagePalette);
        Image img = createRasterImage(imageData,w,h,imagePalette);
        if (img != null)
        {
            imageSize = gr.getOriginalImageSize();
            w = imageSize.width;
            h = imageSize.height;
            applet_.hdfCanvas.setDataRange(range);
            applet_.hdfCanvas.setOriginalImageSize(w,h);
            applet_.hdfCanvas.setImage(img, palImage);
            int skip = ((w>h)?w:h)/PREFEREDIMAGESIZE+1;
            applet_.hdfCanvas.setSubsampleFactor(skip);
            applet_.hdfCanvas.repaint();
        }
    }

    /**
     *  display SDS received from a remote machine
     *
     *  @param hdfObject the HDFObject received from the remote machine
     *  @auther          Peter Cao (xcao@ncsa.uiuc.edu), 10/2/97
     */
    public void dispHdfSDS(HDFObject hdfObject)
    {
        //System.out.println("HDFTreeNode.dispHdfSDS()");

        HDFSDS sds = (HDFSDS) hdfObject;
        byte[] imagePalette = sds.getImagePalette();
        Object sdsData = sds.getData();
        int imageArgv[] = sds.getImageArgv();
        int dataType = imageArgv[1];
        Dimension imageSize = sds.getCurrentImageSize();
        int w = imageSize.width;
        int h = imageSize.height;
        double range[] = sds.getDataRange();
        byte[] imageData = new byte[w*h];
        boolean cvFlag = HDFObject.object2byte(sdsData, dataType, range, 0, imageData);
        Image palImage = createPaletteImage(imagePalette);
        Image img = createRasterImage(imageData,w,h,imagePalette);
        imageSize = sds.getOriginalImageSize();
        w = imageSize.width;
        h = imageSize.height;

        if (img != null)
        {
           applet_.hdfCanvas.setDataRange(range);
           applet_.hdfCanvas.setOriginalImageSize(w,h);
           applet_.hdfCanvas.setImage(img, palImage);
           int skip = ((w>h)?w:h)/PREFEREDIMAGESIZE+1;
           applet_.hdfCanvas.setSubsampleFactor(skip);
           applet_.hdfCanvas.repaint();
        }

        if (sliceSelection != null && sliceSelection.isVisible()) return;

        int rank = imageArgv[2];
        if (rank > 1)
        {
            int numberOfImages = imageArgv[2];
            int dimsize[] = {imageArgv[0], h ,w};
            if (sliceSelection == null)
            {
                // construct slice info.
                boolean states[]  = {true, false, false};
                int selects[] = {0, 0, 0};
                sliceInfo = new SDSDimInfo(rank,dimsize,states,selects);
                if (rank > 2)
                {
                    sliceSelection = new SliceSelection(this,sliceInfo);
                    sliceSelection.setVisible();
                }
            }
            else sliceSelection.setVisible();
            applet_.hdfCanvas.setSliceWindow(sliceSelection);

        }
    }

    /**
     *  get the selected SDS image
     *
     *  @param sliceInfo  the SDSDimInfo for the selected slice of SDS image
     *  @auther           Peter Cao (xcao@ncsa.uiuc.edu), 10/2/97
     */
    public void getHdfSDSimage(SDSDimInfo sliceInfo)
    {
        int orientation = sliceInfo.getActiveSlice();
        int slice = sliceInfo.getSelectedValue(orientation);
        int selection[] = {orientation, slice};
        node.sdsSelection = selection;
        node.isPreview = true;
        applet_.hdfCanvas.setObject(node);
        //node.hdfObject = applet_.getHDFObject(node);
        //dispHdfSDS(node.hdfObject);
        dispHdfSDS(applet_.getHDFObject(node));
    }

    /**
     *  displays all 8-bit raster images in anmiated or layered format
     *  @param viewType  the view type: animated or layered or default
     *  @param node      the current selected node
     */
    public void dispHdfRis8Images(int viewType, HDFObjectNode node)
    {
        if (viewType == JHV.DEFAULT) return;

        Queue children = node.child;
        int imageNumber = children.size();
        Image images[] = new Image[imageNumber];
        HDFObjectNode theNode = null;
        HDFRIS8 ris8 = null;
        byte[] imagePalette = null;
        Object ris8Data = null;
        int imageArgv[] = null;
        int dataType = -1;
        Dimension imageSize = null;
        int w = 0;
        int h = 0;
        byte[] imageData = null;
        Image palImage = null;
        Image theImage = null;
        Color[][] colors = null;
        Color[] color = null;
	byte[] reds = null;
	byte[] greens = null;
	byte[] blues = null;
        if (viewType == JHV.LAYER)
        {
            colors = new Color[imageNumber][];
            reds = new byte[256];
            greens = new byte[256];
            blues = new byte[256];
        }
        int pixel=0, r=0, g=0, b=0;
        IndexColorModel cm = null;

        for (int i=0; i<imageNumber; i++)
        {
            theNode = (HDFObjectNode)children.elementAt(i);
            ris8 = (HDFRIS8)applet_.getHDFObject(theNode);
            imagePalette = ris8.getImagePalette();
            ris8Data = ris8.getData();
            imageArgv = ris8.getImageArgv();
            dataType = imageArgv[1];
            imageSize = ris8.getCurrentImageSize();
            w = imageSize.width;
            h = imageSize.height;
            imageData = new byte[w*h];
            HDFObject.object2byte(ris8Data, dataType, null, 0, imageData);
            palImage = createPaletteImage(imagePalette);
            theImage = createRasterImage(imageData,w,h,imagePalette);
            images[i] = theImage;
        }

        if (viewType == JHV.ANIMATION)
            applet_.animatedImages = new JHVImageAnimation(images);
        else if (viewType == JHV.LAYER)
            applet_.layeredImages = new JHVLayeredImage(images);
    }


}
