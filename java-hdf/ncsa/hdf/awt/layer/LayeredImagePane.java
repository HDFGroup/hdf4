
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


package ncsa.hdf.awt.layer;

import java.awt.*;
import java.awt.event.*;
import java.awt.image.*;
import java.lang.String;
import java.lang.Thread;
import java.util.*;

import ncsa.hdf.awt.image.*;

// Display layered image
public class LayeredImagePane extends Canvas implements MouseListener{

	private Hashtable imageLayers;
        int topImage;
	Dimension panelSize;
  	Image offScreenImage;
  	Graphics offGraphics;
        Color imageBackground;
        LayeredImage layeredImage;

    public LayeredImagePane(LayeredImage layeredImage) {
        this.layeredImage = layeredImage;
        imageBackground = layeredImage.getBackground();
	imageLayers = new Hashtable();
        topImage = 0;
        panelSize = new Dimension(300,200);
	setSize(panelSize);
        addMouseListener(this);
    }

    // Add an image with specified layer
    protected void addImage(LayeredImageInfo imageInfo, int index) {

        topImage = index;

	if (imageInfo != null) {
	   int layer = imageLayers.size();  // set current image layer

	   imageInfo.setLayer(layer);
	   imageLayers.put(Integer.toString(index), imageInfo);

	   // resize panel
           int w = Math.max(panelSize.width, imageInfo.getWidth());
           int h = Math.max(panelSize.height, imageInfo.getHeight());
           panelSize.setSize(w, h);
           setSize(panelSize);
	}
    }

    // Remove an image with specified layer
    protected void removeImage(int index) {
 		
	int layer = getLayer(index);

	if (layer == -1) 
	   return;

	imageLayers.remove(Integer.toString(index));
	
	if (!imageLayers.isEmpty()) { // not empty
	   // should adjust layer
	   for ( Enumeration keys = imageLayers.keys();
	  	 keys.hasMoreElements(); ) {
		String ind = (String)keys.nextElement();
		updateLayer(Integer.parseInt(ind),layer);		
	   }
	}
	
    }

    public int getLayer(LayeredImageInfo imageInfo) {
	return (imageInfo.getLayer());
    }

    /** 
     * Returns the preferred size of panel.
     */
    public Dimension getPreferredSize() {
	return getMinimumSize();
    }

    /**
     * Returns the mininimum size of this component.
     */
    public Dimension getMinimumSize() {
	return (panelSize);
    }

    public int getLayer(int index) {

	LayeredImageInfo info;
	// Tests if the specified object is a key in iamgeLayers
	if (imageLayers.containsKey(Integer.toString(index))) {
	   info = (LayeredImageInfo)imageLayers.get(Integer.toString(index));
	   return (info.getLayer());
	}
	return -1;
    }

    public LayeredImageInfo getLayeredImageInfo(int index) {

	LayeredImageInfo info;
	// Tests if the specified object is a key in iamgeLayers
	if (imageLayers.containsKey(Integer.toString(index))) {
	   info = (LayeredImageInfo)imageLayers.get(Integer.toString(index));
	   return (info);
	}
	return null;
    }

    protected void setLayer(LayeredImageInfo imageInfo, int layer) {
	if (imageInfo != null) {
	   imageInfo.setLayer(layer);
	}
    } 

    protected void setLayer(int index, int layer) {
	if (imageLayers.containsKey(Integer.toString(index))) {
	   LayeredImageInfo info = 
		(LayeredImageInfo)(imageLayers.get(Integer.toString(index)));
	   setLayer(info,layer);
	}
    } 

    protected void updateLayer(int index, int layer) {

	if (imageLayers.containsKey(Integer.toString(index))) {
	   LayeredImageInfo info = 
		(LayeredImageInfo)(imageLayers.get(Integer.toString(index)));

	   if (info.getLayer()>layer) 
		info.setLayer(info.getLayer()-1);	   
	}
    } 

    protected void updateLayerFilter(int index, ImageFilter filter) {

	if (imageLayers.containsKey(Integer.toString(index))) {

	   LayeredImageInfo info = 
		(LayeredImageInfo)(imageLayers.get(Integer.toString(index)));

	   info.setFilter(filter);	   
	}

	repaint();
    } 

    /**
     * Paints the component.
     */
    public void paint(Graphics g) {

	int layerCount = imageLayers.size();
	Image images[] = new Image[layerCount];

	for ( Enumeration imageIndexs = imageLayers.keys();
			  imageIndexs.hasMoreElements(); ) {

		// image index
		int index = Integer.parseInt((String)(imageIndexs.nextElement()));

		LayeredImageInfo info = 
			(LayeredImageInfo)getLayeredImageInfo(index);
		if (info != null) {
		    int layer     = info.getLayer();
		    images[layer] = info.getImage();
		}
	}

	// draw layered image
	for (int kk=0; kk<layerCount; kk++) 
	    g.drawImage(images[kk], 0, 0, this);

    }

 /**
  * Updates the component.
  */
 public void update(Graphics g) { 
	
    Dimension d = getSize();
   
    if (offScreenImage == null) {
      // offScreenImage not created; create it.
      offScreenImage = createImage(d.width , d.height );	
      
      // get the off-screen graphics context    
      offGraphics    = offScreenImage.getGraphics();
      
      // set the font for offGraphics
      offGraphics.setFont(getFont());	 
    }
    
    // paint the background on the off-screen graphics context
    offGraphics.setColor(getBackground());
    offGraphics.fillRect(0,0,d.width,d.height);    
    offGraphics.setColor(getForeground());

    // draw the current frame to the off-screen
    paint(offGraphics);
    
    //then draw the image to the on-screen 
    g.drawImage(offScreenImage, 0, 0, null);
 }

    public Color grabColor(Image image, int x, int y)
    {
        int[] pixels = new int[1];
        PixelGrabber pg = new PixelGrabber(image, x, y, 1, 1, pixels, 0, 1);
        try { pg.grabPixels(); } 
        catch (InterruptedException e) { return null; }

        if ((pg.getStatus() & ImageObserver.ABORT) != 0) { return null; }

        int pixel = pixels[0];
        int alpha = (pixel >> 24) & 0xff;
        int red   = (pixel >> 16) & 0xff;
        int green = (pixel >>  8) & 0xff;
        int blue  = (pixel      ) & 0xff;

        return (new Color(red, green, blue));
    }

    public Color getImageBackground() { return imageBackground; }

    public void setTopImageIndex(int top) { topImage = top; }

    public void mouseClicked(MouseEvent e)
    {
        Point p = e.getPoint();
        Color theColor = null;
        LayeredImageInfo info = getLayeredImageInfo(topImage);
        if (info == null) return;

        Image image = info.getImage();
        if (image == null) return;

        int w = image.getWidth(this);
        int h = image.getHeight(this);

        if (p.x < w && p.y < h)
            theColor = grabColor(image, p.x, p.y);

        if (theColor != null)
            this.imageBackground = theColor;

        layeredImage.bgArea.setBackground (this.imageBackground);
    }

    public void mouseEntered(MouseEvent e){}
    public void mouseExited(MouseEvent e){}
    public void mousePressed(MouseEvent e){}
    public void mouseReleased(MouseEvent e){}

}
