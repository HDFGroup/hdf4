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

package ncsa.hdf.awt;

import java.awt.*;
import java.awt.event.*;
import  java.awt.image.ImageObserver;

/**
* Handle scrolling image display.
* @version 1.00 9/2/97
* @auther  XInjian Lu
*/
class ScrollerImageCanvas extends Canvas {

   /** the currently displayed image */
   Image                 currentImage;

   /** image size */
   int imageWidth  = 16;
   int imageHeight = 16;

   /** variables for double buffering  */
   Image         offScreenImage = null;
   Graphics      offGraphics;

   /** display string */
   String infoStr = "No image";

   /** image offset on panel */
   int tx=0,  ty=0;       // offset translate

   /** scrollbar of the image canvas */
   Scrollbar      hScroll,      vScroll;

   /** new constructor */
   public ScrollerImageCanvas() {
      super();
   }

   /** Constructor with image
     * @param image the image
     */
    public ScrollerImageCanvas(Image image) {

        this();
    
	// set image
	setImage(image);	

	// set the default image size
	setImageSize(imageWidth, imageHeight);
	
	// resize the canvas
	setSize(imageWidth, imageHeight);

    }

   /** Constructor with image
     * @param image the image
     * @param v the vertical scrollbar
     * @param h the horizontal scrollbar
     */
    public ScrollerImageCanvas(Image image, Scrollbar v, Scrollbar h) {

	this(image);

	// append the scrollbar
	setScrollbars(h,v);
	
    }

   /** set the current displayed image
   * @param img the current image
   */
   public void setImage ( Image img ) {
      this.currentImage = img;
     
      // recalculate bounds, etc.
      setHScrollValue();
      setVScrollValue();
      repaint();
   }

   /** set the current image size
   * @param width  the width of the image
   * @param height the height of the image
   */
   public void setImageSize ( int width, int height) {
      imageWidth = width;
      imageHeight = height;

      if (currentImage != null) {
	
         setHScrollValue();
         setVScrollValue();
         repaint();
      }
   }

   /** set the current image size by specified zoom factor
   * @param factor a zoom factor
   */
   public void setImageSize ( float factor) {

      // set image size
      imageWidth = (int)(imageWidth * factor);
      imageHeight =(int)(imageHeight* factor);

      // prepare image
      if (currentImage != null) {
    	
	 setHScrollValue();
         setVScrollValue();
         repaint();
      }
   }

   /** set the current image width
   * @param width  the width of the image
   */
   public void setImageWidth ( int width ) {
      imageWidth = width;
      if (currentImage != null) setHScrollValue();
   }

   /** set the current image height
   * @param height the height of the image
   */
   public void setImageHeight ( int height) {
      imageHeight = height;
      if (currentImage != null) setVScrollValue();
   }

   /** Reshapes the Component to the specified bounding box. */
   public synchronized void setBounds ( int x, int y, int w, int h ) {

      super.setBounds( x, y, w, h );

      // resize scrollbars
      setHScrollValue();
      setVScrollValue();
   }

  /**
   * Set scrollbars for the canvas
   * @param h the horizontal scrollbar
   * @param v the vertical scrollbar
   */
   public void setScrollbars ( Scrollbar h, Scrollbar v ) {
      // set scrollbar values
      this.hScroll = h;
      this.vScroll = v;
   }

   /** change the horizontal scrollbar value to the sn appropriate width. */
   void setHScrollValue() {
      // get current canvas size
      int canvasWidth = getSize().width;

      // canvas is valid?
      if (canvasWidth <= 0) {
         return;
      }

      if (currentImage == null) {
         return;
      }

      //Shift everything to the right if we're displaying empty space
      //on the right side.
      if ((tx + canvasWidth) > imageWidth) {
         int newtx = imageWidth - canvasWidth;

         if (newtx < 0) {
           newtx = 0;
         }
         tx = newtx;
      }

      int p = (int)(canvasWidth * 0.9);
      int m = imageWidth - (canvasWidth - p);
      hScroll.setValues(//draw the part of the image that starts at this x:
                         tx,
                         //amount to scroll for a "page":
                         p,
                         //minimum image x to specify:
                         0,
                         //maximum image x to specify:
                         m);

      //"visible" arg to setValues() has no effect after scrollbar is visible.
      hScroll.setBlockIncrement( p);

      if ((imageWidth - canvasWidth) <= 0)
         hScroll.setEnabled(false);
      else
         hScroll.setEnabled(true);

      return;
   }

 /** Adjust the Vertical Scrollbar value by the specified width. */
   void setVScrollValue() {

      // get current canvas size
      int canvasHeight = getSize().height;

      // canvas is valid?
      if (canvasHeight <= 0) {
         return;
      }

      if (currentImage == null) {
         return;
      }

      //Shift everything to the right if we're displaying empty space
      //on the right side.
      if ((ty + canvasHeight) > imageHeight) {
         int newty = imageHeight - canvasHeight;

         if (newty < 0) {
            newty = 0;
         }
         ty = newty;
      }

      int p = (int)(canvasHeight * 0.9);
      int m = imageHeight - (canvasHeight - p);
      vScroll.setValues(//draw the part of the image that starts at this y:
                         ty,
                         //amount to scroll for a "page":
                         p,
                         //minimum image y to specify:
                         0,
                         //maximum image y to specify:
                         m);

      //"visible" arg to setValues() has no effect after scrollbar is visible.
      vScroll.setBlockIncrement(p);

      if ((imageHeight - canvasHeight) <= 0)
         vScroll.setEnabled(false);
      else
         vScroll.setEnabled(true);

      return;
   }

   /** return the current image in the canvas */
   public Image getImage() {
      return currentImage;
   }

   /**
   * Get the dimensions of an image.
   * @return the image's dimensions.
   */
   Dimension getImageDimensions(Image im) {
     return new Dimension(im.getWidth(null), im.getHeight(null));
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
    int startx=0, starty=0;
 
    // set the specified translated parameters 
    // and the subcomponents will be relative to this origin.
    g.translate(-tx, -ty);
   
    // If the image size is greater than the Canvas 
    int width;
    int height;
    width = getImageWidth();
    height= getImageHeight();
      
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
	
	// draw a box around the empty canvas
	g.setColor( Color.cyan );
	g.drawRect( 0, 0, getSize().width, getSize().height );
   
	// info
	g.setColor( Color.black );
	FontMetrics fm = g.getFontMetrics();
	int x = (w - fm.stringWidth(infoStr))/2;
	int y = h/2;
	g.drawString( "No image", x, y );
	
    } else {
	
	/*
	// draw the frame
	g.setColor(Color.red);
	g.drawRect(0,0,getSize().width, getSize().height);
	*/
	
	// paint image
	if (prepareImage(currentImage, this)) 
	    g.drawImage(currentImage, startx, starty,width, height, this);
	else
	    g.drawImage(currentImage,startx,starty, this);
	
    }
    // set the specified translated parameters 
    // and the subcomponents will be relative to this origin.
    g.translate(tx, ty);
}

   /**
   * Updates the component. This method is called in
   * response to a call to repaint. You can assume that
   * the background is not cleared.
   * @param g the specified Graphics window
   * @see java.awt.Component#update
   */
   public void update ( Graphics g ) {

      Dimension d = getSize();

      if (offScreenImage == null) {
         // offScreenImage not created; create offscreen image of proper size
         if ((d.width*d.height) < 800 * 500)
            offScreenImage = createImage(800, 500);
         else
            offScreenImage = createImage(d.width, d.height);

         // get the off-screen graphics context
         offGraphics    = offScreenImage.getGraphics();

         // set the font for offGraphics
         offGraphics.setFont(getFont());
      }

      // paint the background on the off-screen graphics context
      offGraphics.setColor( getBackground() );
      offGraphics.fillRect( 0,0, d.width, d.height);
      offGraphics.setColor( getForeground() );

      // draw the current frame to the offscreen image
      paint( offGraphics );

      // draw the (offscreen) image to the current (on-screen) graphics context
      g.drawImage( offScreenImage, 0, 0, null );
   }
}

/**
* A panel containing a scrolling image canvas and scrollbars.
*/
public class ScrollingPanel extends Panel implements AdjustmentListener {

   /** the canvas upon which the image will be painted. */
   ScrollerImageCanvas       imageCanvas = null;
    
   Image             view;
   Dimension         preferred = null;

   /** Scrollbar of the image canvas */
   Scrollbar         hScroll, vScroll;

   /** Create a separate panel based on the image canvas.
   *  This panel will contain the image canvas and scrollbars
   */
   public ScrollingPanel ( Image image ) {
    
      super();

      // image canvas
      imageCanvas = new ScrollerImageCanvas();
      setImage( image );

      // set layout manager
      setLayout( new BorderLayout() );

      // create the scrollbar
      vScroll = new Scrollbar();
      vScroll.addAdjustmentListener(this);
      hScroll = new Scrollbar( Scrollbar.HORIZONTAL );
      hScroll.addAdjustmentListener(this);

      this.add("Center", imageCanvas );
      this.add("East",   vScroll);
      this.add("South",  hScroll);

      // set scrollbars
      imageCanvas.setScrollbars( hScroll, vScroll );
   }

   /** Create a separate panel based on the image canvas.
   *  This panel will contain the image canvas and scrollbars
   */
   public ScrollingPanel () {
    
      super();

      // image canvas
      imageCanvas = new ScrollerImageCanvas();
      //setImage( null );

      // set layout manager
      setLayout( new BorderLayout() );

      // create the scrollbar
      vScroll = new Scrollbar();
      vScroll.addAdjustmentListener(this);
      hScroll = new Scrollbar( Scrollbar.HORIZONTAL );
      hScroll.addAdjustmentListener(this);
      this.add("Center", imageCanvas );
      this.add("East",   vScroll);
      this.add("South",  hScroll);

      // set scrollbars
      imageCanvas.setScrollbars( hScroll, vScroll );
   }

   /** Create a separate panel based on the image canvas.
   *  This panel will contain the image canvas and scrollbars
   */
   public ScrollingPanel ( int width, int height ) {
      super();

      preferred = new Dimension( width, height );

      // image canvas
      imageCanvas = new ScrollerImageCanvas();

      // set layout manager
      setLayout( new BorderLayout() );

      // create the scrollbar
      vScroll = new Scrollbar();
      vScroll.addAdjustmentListener(this);
      hScroll = new Scrollbar( Scrollbar.HORIZONTAL );
      hScroll.addAdjustmentListener(this);

      this.add("Center", imageCanvas );
      this.add("East",   vScroll);
      this.add("South",  hScroll);

      // set scrollbars
      imageCanvas.setScrollbars( hScroll, vScroll );

      this.setSize( width, height );
   }

  /**
   * Set the image to be displayed on the panel.
   * <p>
   * @param java.awt.Image	The image object to display
   */
   public void setImage ( Image image ) {

   if (image != null) {
	
      // set image specifications, or load the image and defer specs
      if (prepareImage( image, this )) {
         int w = image.getWidth(this);
         int h = image.getHeight(this);

         imageCanvas.setImageSize( w, h );
         imageCanvas.setImage( image );

      }
      else {
         image.getWidth( this );
         image.getHeight( this );
      }
      view = image;
    }
  }

  /**
   * Set the image to be displayed on the panel by provided image size
   * <p>
   * @param java.awt.Image	The image object to display
   */
   public void setImage ( Image image , int w, int h) {

   if (image != null) {
	
	prepareImage( image,w,h,this );
	
	imageCanvas.setImageSize( w, h );
	imageCanvas.setImage( image );

	view = image;
    }
  }

  /**
   * Set the image size
   */
   public void setImageSize(float factor ) {
    
    imageCanvas.setImageSize(factor);

  }

   public boolean imageUpdate ( Image img, int infoflags, int x, int y,
         int width, int height ) {

      // process image event notification
      if ((infoflags & ImageObserver.ERROR) != 0) {
         view = null;
         
         // System.out.println( "(Panel.imageUpdate) image acquisition failed" );

         repaint();
         return false;
      }

      if ((infoflags & ImageObserver.ABORT) != 0) {
         view = null;
         
         // System.out.println( "(Panel.imageUpdate) image acquisition aborted" );

         repaint();
         return false;
      }

      if ((infoflags & ImageObserver.ALLBITS) != 0) {
         imageCanvas.setImage( img );
         view = img;
         
         repaint();
         return true;
      }

      if ((infoflags & ImageObserver.WIDTH) != 0) {
         imageCanvas.setImageWidth( width );
      }

      if ((infoflags & ImageObserver.HEIGHT) != 0) {
         imageCanvas.setImageHeight( height );
      }

      return true;
   }

  public void adjustmentValueChanged(AdjustmentEvent e)
  {
    int type = e.getAdjustmentType();
    Scrollbar target = (Scrollbar) e.getAdjustable();

    switch (type) {
    case AdjustmentEvent.UNIT_INCREMENT:
    case AdjustmentEvent.UNIT_DECREMENT:
    case AdjustmentEvent.BLOCK_INCREMENT:
    case AdjustmentEvent.BLOCK_DECREMENT:
    case AdjustmentEvent.TRACK:
    
      // detect the vertical scrollbar
      if (target ==  vScroll) {
	// get translated value
	imageCanvas.ty = vScroll.getValue();
	
	// repaint the graphics
	imageCanvas.repaint();
      }
      // detect the horizontal scrollbar
      if (target ==  hScroll) {
	// get translated value
               imageCanvas.tx = hScroll.getValue();
	       
               // repaint the graphics
               imageCanvas.repaint();
      }
    } // switch(evt.id)
    
    
    
  }

   /**
   * All sizing methods are overridden as debugging aids.
   */
   public void setSize ( int width, int height ) {
      super.setSize( width, height );
   }

   public void setSize ( Dimension d ) {
      super.setSize( d );
   }

   public void setBounds ( int x, int y, int width, int height ) {
      super.setBounds( x, y, width, height );
   }

   public Dimension getMinimumSize ( ) {
      Dimension   d = super.getPreferredSize();
      return d;
   }

   public Dimension getPreferredSize ( ) {
      Dimension   d = super.getPreferredSize();

      if (preferred != null) {
         return preferred;
      }

      return d;
   }
}
