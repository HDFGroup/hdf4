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


import  ncsa.hdf.hdflib.*;
import java.awt.event.*;
import java.awt.*;
import java.awt.image.*;
import java.net.*;
import java.applet.*;
import java.io.File;
import java.io.InputStream;
import java.io.IOException;
import java.util.*;
import java.net.*;


//---------------------------------------------------------------------
//
//  Upgraded to the JDK 1.1.1b Event Model.
//    - Apu Kapadia, May 21st, 1997.
//
//---------------------------------------------------------------------


/**
 * JHV  is to display the HDF tree structure
 *
 * @author HDF Group, NCSA
 */

/** This class define the image canvas. When click the hdf node, the image can
 *  be painted on this canvas */
public class JHVCanvas extends Canvas implements MouseListener, 
                                                 MouseMotionListener {

  JHV		app;
  
  Image 	  	currentImage;
  Image           currentPaletteImage;
  
  HDFObjectNode	node;
  
  String infoStr = "";
  
  // image size;
  int imageWidth  = 256;
  int imageHeight = 256;
  
  Rectangle 	imgRange = new Rectangle() ;
  
  // variables for double-buffer
  Image 	offScreenImage = null;
  Graphics 	offGraphics;
  
  boolean 	mouseOnImageFlag = false;
  
  // current mouse position on the image
  int mx, my;

  /** the constructor */
  JHVCanvas() { 
  }
  

  /** use the applet to construct the HDFCanvas */
  public JHVCanvas(JHV app) {
    this.app = app;  
    this.addMouseListener(this);
    this.addMouseMotionListener(this);
  }

  /** set the current image  displayed
   * @param img the current image
   */ 
  public void setImage(Image img) {

    this.currentImage = null;
    this.currentImage = img;
    this.currentPaletteImage = null;
    repaint();
    try {
      Thread.sleep(10);
    } catch (InterruptedException e) {}
  }

  /** set the current image  displayed
   * @param img the current image,
   * @param palImage the palette image
   */ 
  public void setImage(Image img, Image palImage){
    this.currentImage = img;
    this.currentPaletteImage = palImage;
    repaint();
    try {
      Thread.sleep(10);
    } catch (InterruptedException e) {}
  }

  /** set the current palette Palette image  displayed
   * @param img the current image
   */ 
  public void setPaletteImage(Image img) {
    this.currentPaletteImage = img;	
    repaint();
    try {
      Thread.sleep(10);
    } catch (InterruptedException e) {}
  }

  /** set the current HDF Object node
   * @param node  the HDF Object Node
   */ 
  public void setObject(HDFObjectNode node) {
    this.node = node;
  }

  /** set the current image  size
   * @param width  the width of the image
   * @param height the height of the image
   */ 
  public void setImageSize(int width, int height) {
    
    imageWidth = width;
    imageHeight= height;
  }

  /** return the current image in the canvas */
  public Image getImage() {
    return currentImage;
  }
  

  /** return the current image width  */
  public int getImageWidth() {
    return imageWidth;
  }

  /** return the current image height  */
  public int getImageHeight() {
    return imageHeight;
  }

  /**
   * Returns the minimum size of this component.
   * @see #preferredSize
   * @see LayoutManager
   */
  public Dimension getMinimumSize() {
    return new Dimension(20,20);
  }

  /**
   * Paints the component.
   * @param g the specified Graphics window
   * @see java.awt.Component#paint
   */
  public void paint(Graphics g) {
    
    // get current Canvas size
    int w = getSize().width;
    int h = getSize().height;
    
    // get the approciate position to display the image
    int startx=1, starty=1;
    
    // If the image size is greater than the Canvas make the image shrink
    int width;
    int height;
    width = getImageWidth();
    height= getImageHeight();
    
    int  factor_x = width / w;	 
    int  factor_y = height / h;
    
    int factor = Math.max(factor_x, factor_y) + 1;
    
    if (factor>1) {
      width /= factor;
      height/= factor;
    }
    
    if (width>w)
      startx = 1;
    else
      startx = (w-width)/2;
    
    if (height>h)
      starty = 1;
    else
      starty = (h-height)/2;
    
    if (currentImage == null) {
      super.paint(g);
      // frame
      g.setColor(Color.red);
      g.drawRect(1,1,getSize().width-2, getSize().height-2);
      
      // info
      g.setColor(Color.black);
      FontMetrics fm = g.getFontMetrics();
      int x = (w - fm.stringWidth(infoStr))/2;
      int y = h/2;
      g.drawString(infoStr, x, y);

    } else {
      // draw the frame
      g.setColor(Color.white);
      g.draw3DRect(1,1,getSize().width-2,getSize().height-2,false);
	
      // image
      g.drawImage(currentImage, startx, starty,width, height, this);
	
      // display palette    
      if (currentPaletteImage != null) {
	
	int palWidth;
	int palHeight;
	    
	if ((startx > 16))
	  palWidth = 12;
	else
	  palWidth = startx -2;

	if (h > 256)
	    palHeight = 255;
	else
	    palHeight = h - 4;
	    	    
	g.drawImage(currentPaletteImage,
		    2,
		    (h-palHeight)/2+1,
		    palWidth,
		    palHeight,
		    this);
      }
      
      // make annotation for image
      if ((mouseOnImageFlag) && (node != null)) {
	
	mouseOnImageFlag  = false;
	
	// write info. on the top of the image
	writeInfo(g, mx, my, "Click for more");
	
      }
    }
    
    // set the image range
    imgRange.setBounds(startx, starty, width, height);
    
  }

  /** Write the information on the top of the image
   * @param g the graphics
   * @param x the x   coordinate
   * @param y the y coordinate
   * @param info the string
   */
  public void writeInfo(Graphics g , int x , int y , String info) {

    // get width and height
    FontMetrics lFM = g.getFontMetrics();
    int w = lFM.stringWidth(info) + lFM.stringWidth("AB");
    int h = lFM.getHeight()*2;
    
    // get position to display the string
    int xpos = x + lFM.stringWidth("A");
    int ypos = y + lFM.getHeight() + lFM.getHeight()/2;
    
    // set the background
    g.setColor(Color.white);
    
    // draw the oval to display the string
    g.fillOval(x,y,w,h);
    
    // set the foreground
    g.setColor(Color.red);
    
    g.drawString(info, xpos,ypos );
    
  }
  

  /** reset the Canvas size
   * @param w the width of the Canvas
   * @param h the height of the Canvas
   */
  public void setCanvasSize(int w, int h) {
    
    Dimension d= getSize();
    
    // get the current canvas size
    int width=getSize().width;
    int height=getSize().height;
    
    if (w>width)
      width = w;
    if (h>height)
      height = h;
    
    // resize the canvas
    setSize(width, height);
    
  }
  

  /**
   * Updates the component. This method is called in
   * response to a call to repaint. You can assume that
   * the background is not cleared.
   * @param g the specified Graphics window
   * @see java.awt.Component#update
   */
  public void update(Graphics g) {

    Dimension d = getSize();
    
    if (offScreenImage == null) {
      // offScreenImage not created; create it.
      offScreenImage = createImage(d.width, d.height);	
      
      // get the off-screen graphics context    
      offGraphics    = offScreenImage.getGraphics();
      
      // set the font for offGraphics
      offGraphics.setFont(getFont());	 
    }
    
    // paint the background on the off-screen graphics context
    offGraphics.setColor(getBackground());
    offGraphics.fillRect(1,1,d.width-2,d.height-2);    
    offGraphics.setColor(getForeground());
    
    
    // draw the current frame to the off-screen 
    paint(offGraphics);
    
    //then draw the image to the on-screen 
    g.drawImage(offScreenImage, 0, 0, null);
    
  }
  

  /**
   * Called when the mouse enters the component.
   * @param evt the event
   * @param x the x coordinate
   * @param y the y coordinate
   * @see java.awt.Component#mouseEnter
   */

  public void mouseEntered(MouseEvent me)
  {
    int x = me.getX();
    int y = me.getY();

    if (imgRange.contains(x,y)) {
      
      // make annotation (Click to get the orignal image)
      mouseOnImageFlag = true;

      mx = x;
      my = y;
      repaint();
    }

  }


  /**
   * Called when the mouse exits the component.
   * @param evt the event
   * @param x the x coordinate
   * @param y the y coordinate
   * @see java.awt.Component#mouseExit
   */

  public void mouseExited(MouseEvent me)
  {
    mouseOnImageFlag = false;
    repaint();
  }


  public void mouseDragged(MouseEvent me) {}

  /**
   * Called if the mouse moves (the mouse button is up)
   * @param evt the event
   * @param x the x coordinate
   * @param y the y coordinate
   * @see java.awt.Component#mouseMove
   */

  public void mouseMoved(MouseEvent me)
  {

    int x = me.getX();
    int y = me.getY();

    if (imgRange.contains(x,y)) {
      
      // make annotation (Click to get the orignal image)
      mouseOnImageFlag = true;
      mx = x;
      my = y;
      repaint();
    }
  }
						   
  public void mouseClicked(MouseEvent me) {}
  public void mousePressed(MouseEvent me) {}
						   
   /**						      	   
    * Called if the mouse is up.
    * @param evt the event
    * @param x the x coordinate
    * @param y the y coordinate
    * @see java.awt.Component#mouseUp
    */
  public void mouseReleased(MouseEvent me)
  {
    if ((node != null) && (currentImage != null)) {
      
      // set cursor type to "WAIT_CURSOR"
      ((Component)app.jhvFrame).setCursor(new Cursor(Cursor.WAIT_CURSOR));
      
      // Create  new frame and display the orginal image use the ScrollPanel
      JHVImageFrame frame = new JHVImageFrame(this);
      
      // display frame
      frame.popup();
      
      // set cursor type to "DEFAULT_CURSOR"
      ((Component)app.jhvFrame).setCursor(new Cursor(Cursor.DEFAULT_CURSOR));
    }
  }

}
