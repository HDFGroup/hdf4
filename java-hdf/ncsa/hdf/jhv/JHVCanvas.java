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

/**
 * JHV  is to display the HDF tree structure
 * Modified: 
 * 	Handle large HDF file access. xlu 9/6/97
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

  int originalImageWidth  = 256;
  int originalImageHeight = 256;
 
  // image size;
  int imageWidth  = 256;
  int imageHeight = 256;
  
  // Rectangle 	imgRange = new Rectangle() ;
  
  // variables for double-buffer
  Image 	offScreenImage = null;
  Graphics 	offGraphics;
  
  boolean 	mouseOnImageFlag = false;
  
  // current mouse position on the image
  int mx, my;

  // top-left point of an image displayed     
  int startx, starty;

  Rectangle 	currentDragArea;

  // following two variables to be added to handle the large dataset access.
  // image area in this canvas
  Rectangle  imageArea = new Rectangle();
    
  // subset range of an image
  Rectangle  subsetRange;

  // indicator of a mouse gragging
  boolean 	dragFlag = false;
  Rectangle	dragArea;

  // subsample - expanded factor
  int	skip = 1;
  
  // sds data range
  double[] range;

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

    if (img==null) dragFlag = false;
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

    setImageSize(img.getWidth(this), img.getHeight(this));

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

 public void setOriginalImageSize(int width, int height) {
    
    originalImageWidth = width;
    originalImageHeight= height;
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
    
    //System.out.println("HDBCanvas: " + w + ", " + h);
    
    // get the approciate position to display the image
    startx=0;
    starty=0;
    
    // If the image size is greater than the Canvas make the image shrink
    int width;
    int height;
    width = getImageWidth();
    height= getImageHeight();
    
    // System.out.println ( "Image: " + width + " ," + height);
    
    int  factor_x = width / w;	 
    int  factor_y = height / h;
    
    int factor = Math.max(factor_x, factor_y) + 1;
    
    if (factor>1) {
      width /= factor;
      height/= factor;
    }
    
    if (width>w)
      startx = 0;
    else
      startx = (w-width)/2;
    
    if (height>h)
      starty = 0;
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
      // setBackground(Color.black);

      //g.setColor(Color.red);

      //g.drawRect(0,0,getSize().width, getSize().height);
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
	writeInfo(g, mx, my, "Highlight image");
	
      }
    }
  
    // set the image range
    imageArea.setBounds(startx, starty, width, height);
    
    g.setColor(Color.red);   
    if  (dragFlag)
	g.drawRect(dragArea.x, dragArea.y, dragArea.width, dragArea.height);

    //System.out.println("imageArea: " + imageArea);  
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
      offScreenImage = createImage(d.width*2, d.height*2);	
      
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
  
    // check mouse position, then do something.
    checkMousePosition(x,y);
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

  public void mouseDragged(MouseEvent me) {
	int x = me.getX();
    	int y = me.getY();

 	// set the mouse draged
    	dragFlag = true;
    
    	// resize the draged area
    	currentDragArea.setSize(x-currentDragArea.x , y-currentDragArea.y );
    
    	// get the rectangle that can be drawable(the value is valid);
    	// get current canvas size 
    	Dimension d = getSize();
    
    	// make sure that drag area can be drawable
    	dragArea = getDrawableRect(currentDragArea, d);

    	// repaint to display the info. or not. 		
    	repaint();
  }

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
 
    // check mouse position, then do something to that.
    checkMousePosition(x,y);
    repaint();
  }
						   

  public void mouseClicked(MouseEvent me) {}

  public void mousePressed(MouseEvent me) {
   
	int x = me.getX();
    	int y = me.getY();

	// keep the mouse position
    	currentDragArea = new Rectangle(x, y, 0,0);
  }
						   

   /**						      	   
    * Called if the mouse is up.
    */
  public void mouseReleased(MouseEvent me)   {
    
    // Is dragArea valid?
    // Checks if two rectangles intersect.
    if  (node != null) {
      if ((node.getObjType() != node.RIS8) && 
	  (node.getObjType() != node.RIS24) ) {
	
	if ( dragFlag) 
	  if ( ! dragArea.intersects(imageArea) ) return ;
	
	if (!dragFlag)
	  subsetRange = new Rectangle(0,0,originalImageWidth, originalImageHeight);
	else {
	  // compute the dataset range based on the draged area & scroll offset
	  // if the dragArea is inside the image area
	  // set datasetArea.
	  subsetRange = new Rectangle(dragArea.x - startx , 
				      dragArea.y - starty ,
				      dragArea.width,      dragArea.height);
	  
	  // make the draw area valid
	  int dx = dragArea.x - startx;
	  int dy = dragArea.y - starty;
	  int w  = dragArea.width;
	  int h  = dragArea.height;
	  
	  if (dx <0) {	
	    w += dx;
	    dx = 0;
	  }
	  
	  if (dy <0) {
	    h += dy;
	    dy = 0;
	  }
	  
	  if ( (dx+w) > imageArea.width)
	    w = imageArea.width - dx;
	  if ( (dy+h) > imageArea.height)
	    h = imageArea.height - dy;
	  
	  int f = getSubsampleFactor();
	  
	  int  factor_x = getImageWidth() / getSize().width  ;
	  int  factor_y = getImageHeight()/ getSize().height ;
	  
	  int factor = Math.max(factor_x, factor_y) + 1;
	  
	  if (factor>1) f *= factor;
	  
	  // reset the datasetRange
	  if ( (dragArea.intersection(imageArea)).equals(imageArea) )
	    // drag area is bigger enough
	    subsetRange.setBounds(0,0,originalImageWidth, originalImageHeight);
	  else
	    subsetRange.setBounds(dx*f,dy*f,w*f,h*f);
	  
	  if ((dragArea.x<imageArea.x)&&
	      ((dragArea.width+dragArea.x)>(imageArea.x+imageArea.height)))
	    subsetRange.setBounds(0,subsetRange.y,originalImageWidth,subsetRange.height);
	  
	  if ((dragArea.y<imageArea.y)&&
	      ((dragArea.height+dragArea.y)>(imageArea.y+imageArea.height)))
	    subsetRange.setBounds(subsetRange.x,0,subsetRange.width,originalImageHeight );
	  
	}
      }
      else
	subsetRange = new Rectangle(0,0,originalImageWidth, originalImageHeight);
      
      // System.out.println("HDBCanvas::mouseUp::Dataset: " + subsetRange);
    }
    
    if ((node != null) && (currentImage != null) && (subsetRange != null)) {
      
      // set cursor type to "WAIT_CURSOR"
      ((Component)app.jhvFrame).setCursor(new Cursor(Cursor.WAIT_CURSOR));
      
      // Create  new frame and display the orginal image use the ScrollPanel
      // JHVImageFrame frame = new JHVImageFrame(this);
      JHVImageFrame frame = new JHVImageFrame(this, subsetRange);
      
      // display frame
      frame.popup();
      
      // set cursor type to "DEFAULT_CURSOR"
      ((Component)app.jhvFrame).setCursor(new Cursor(Cursor.DEFAULT_CURSOR));
      
    }  
    
  }
  

  /** Return the new drawable rectangle based on the anonther rectangle &
   *  currnet canvas size.
   * @param originalRect the rectangle
   * @param drawingArea  the size can be painted.
   * @return the new rectangle
   */
  Rectangle getDrawableRect(Rectangle originalRect, Dimension drawingArea) {
    int x = originalRect.x;
    int y = originalRect.y;
    int width = originalRect.width ;
    int height = originalRect.height ;
    
    //Make sure rectangle width and height are positive.
    if (width < 0) {
      width = 0 - width;
      x = x - width + 1;
      if (x < 0) {
	width += x;
	x = 0;
      }
    }
    if (height < 0) {
      height = 0 - height;
      y = y - height + 1;
      if (y < 0) {
	height += y;
	y = 0;
      }
    }
    
    //The rectangle shouldn't extend past the drawing area.
    if ((x + width ) > drawingArea.width) {
      width = drawingArea.width - x ;
    }
    if ((y + height )> drawingArea.height) {
      height = drawingArea.height - y ;
    }  
    return new Rectangle(x, y, width, height);;
  }

  // check mouse position, then do something to that.
  public void checkMousePosition(int x, int y) {
	
    // check if the point(x,y) is inside the image area
    if (imageArea.contains(x,y)) {
  
      // make annotation (Click to get the orignal image)
      mouseOnImageFlag = true;
    } // if (imageArea.inside(x,y)) {
    else {
	mouseOnImageFlag = false;  
    }

    // keep current mouse position.
    mx=x; my=y;
  }
  

  void setSubsampleFactor(int skip) {
	this.skip = skip;
   }

   int getSubsampleFactor() {
	return this.skip;
   }

   

   public void setDataRange(double[] range) {
	this.range = range;
  }

 

 public double getMaxValue() {
	if (range != null)
	   return range[1];
	return 0;
  }

  public double getMinValue() {
	if (range != null)
	   return range[0]; 
	return 0;
  }
}
