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

import  java.awt.*;
import java.awt.event.*;

/** This class will display the image  in the seprate  window */

class ImageCanvas extends Canvas {
  
  /** the current displayed image */
  Image 	  	currentImage;
     
  /** image size */
  int imageWidth  = 256;
  int imageHeight = 256;
	
  /** variables for duble-buffer  */
  Image 	offScreenImage = null;
  Graphics 	offGraphics;
 
  /** display string */
  String infoStr = "None image created";
    
  /** frame displayed ? */
  boolean frameDisplayed = false;
    
  /** translated variable for scroll panel */
  int tx=0,  ty=0;       // offset translate 
    
  /** Scrollbar of the image canvas */
  Scrollbar      hScrollbar,      vScrollbar;

  /** new constructor */
  public ImageCanvas() {	
   
    super();
    
  }

  /** set the current   displayed image
   * @param img the current image
   */ 
  public void setImage(Image img) {
    this.currentImage = img;	
    repaint();
  }

  /** set the current image  size
   * @param width  the width of the image
   * @param height the height of the image
   */ 
  public void setImageSize(int width, int height) {
    
    imageWidth = width;
    imageHeight= height;
  }

  /** resize the canvas by the provided ratio
    * @param factor the factor to origanize the frame
    */
  public void setCanvasSize(float factor){
   
    int w = getSize().width;
    int h = getSize().height;
    
    w = (int)(w*factor);
    h = (int)(h*factor);
    
    // set image size
    setImageSize((int)(getImageWidth()*factor), (int)(getImageHeight()*factor));
    
    // resize the canvas
    setSize(w,h);
    
  } 

  /** Reshapes the Component to the specified bounding box. */
  public synchronized  void setBounds(int x, int y, int w, int h) {
    
    super.setBounds(x, y, w, h);
    
    // resize horizontal scrollbar
    setHScrollValue();
    
    // resize the vertical scrollbar
    setVScrollValue();
    
  }

  /**
   *  set Scrollbar
   * @param h the horizontal scrollbar
   * @param v the vertical scrollbar
   */
  public  void  setScrollbar(Scrollbar h, Scrollbar v) {
   
    // set scrollbar value
    this.hScrollbar = h;
    this.vScrollbar = v;
  }

  /** Adjust the Horizontal Scrollbar value by the specifyed  width. */
  void setHScrollValue() {

    // get current canvas size
    int canvasWidth = getSize().width;
    
    // canvas is valid?
    if (canvasWidth <= 0) {
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
int p = (int) (canvasWidth * 0.9); 
int m = (int)(imageWidth - (canvasWidth - p)); 
    hScrollbar.setValues(//draw the part of the image that starts at this x:
			 tx, 
			 //amount to scroll for a "page":
			 p, //(int)(canvasWidth * 0.9), 
			 //minimum image x to specify:
			 0,
			 //maximum image x to specify:
			 m); //imageWidth - canvasWidth);
    
    //"visible" arg to setValues() has no effect after scrollbar is visible.
    hScrollbar.setBlockIncrement(p);//(int)(canvasWidth * 0.9));

    /*
    if ((imageWidth - canvasWidth) <= 0)
       hScrollbar.hide();
    else
       hScrollbar.show();
    */
    return;
  }
  

 /** Adjust the Vertical Scrollbar value by the specifyed  width. */
  void setVScrollValue() {
    
    // get current canvas size
    int canvasHeight = getSize().height;
    
    // canvas is valid?
    if (canvasHeight <= 0) {
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
    
int p = (int) (canvasHeight * 0.9); 
int m = (int)(imageHeight - (canvasHeight - p)); 
    vScrollbar.setValues(//draw the part of the image that starts at this y:
			 ty, 
			 //amount to scroll for a "page":
			 p, //(int)(canvasHeight * 0.9), 
			 //minimum image y to specify:
			 0,
			 //maximum image y to specify:
			 m); //imageHeight - canvasHeight);
    
    //"visible" arg to setValues() has no effect after scrollbar is visible.
    vScrollbar.setBlockIncrement(p);//(int)(canvasHeight * 0.9));
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
   
    
    // If the image size is greater than the Canvas make the image shrink
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
      // frame
      g.setColor(Color.red);
      g.drawRect(0,0,getSize().width, getSize().height);
      
      // info
      g.setColor(Color.black);
      FontMetrics fm = g.getFontMetrics();
      int x = (w - fm.stringWidth(infoStr))/2;
      int y = h/2;
      g.drawString(infoStr, x, y);
      
    } else {
		
      // paint image
      g.drawImage(currentImage, startx, starty,width, height, this);
      
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
  public void update(Graphics g) {
  
    Dimension d = getSize();
    
    if (offScreenImage == null) {
      // offScreenImage not created; create it.
      // make the off-screen image big enough
      if ((d.width*d.height) < 500 * 500)
	  offScreenImage = createImage(500, 500);
      else
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
}

/** This class define the image frame */
public class ImageFrame extends Frame 
    implements WindowListener, AdjustmentListener, ActionListener {

  /** the canvas that the image will  be painted. */
  ImageCanvas       imageCanvas = null;
    
  /** Scrollbar of the image canvas */
  Scrollbar      hScrollbar,      vScrollbar;

  /** the frame from Spreadsheet */
  JHVDataFrame   dataFrame = null;

  /** Create the seprate frame based on the  image canvas.
   *  This frame will display the image within the frame
   */
  public ImageFrame(Image image) {

    // set the frame tittle
    setTitle();
    
    // image canvas
    imageCanvas = new ImageCanvas();

    // set image size
    int w = image.getWidth(this);
    int h = image.getHeight(this);
  
    // repaint the image
    imageCanvas.setImage(image);

    imageCanvas.setImageSize(w,h);
   
    // set layout manager
    setLayout(new BorderLayout());
            
    // create the interface for this frame
    Panel panel = new Panel();
    panel.setLayout(new BorderLayout());
    
    // create the scrollbar
    vScrollbar = new Scrollbar();
    vScrollbar.addAdjustmentListener(this);
    hScrollbar = new Scrollbar(Scrollbar.HORIZONTAL);
    hScrollbar.addAdjustmentListener(this);

    panel.add("Center", imageCanvas );
    panel.add("East",   vScrollbar);
    panel.add("South",  hScrollbar);
    
    add("Center", panel);
        
    // add control panel
    Button dismiss = new Button("Dismiss");
    dismiss.addActionListener(this);
    add("South", dismiss);
   
    // set scrollbar
    imageCanvas.setScrollbar(hScrollbar, vScrollbar);
    
    popup();
    addWindowListener(this);
  }

  /** setup the frame tittle */
  public void setTitle() {
    String winString = " Image ";
    super.setTitle(winString);
  }

  /** popup the new frame actually */
  public void popup() {
         
    // adjust the size of the frame
    int frameWidth = Math.min(imageCanvas.imageWidth , 1000) + 50;
    int frameHeight= Math.min(imageCanvas.imageHeight, 1000) + 100;
    
    if ((frameWidth >400) || (frameHeight > 400))
        setSize(400,400);
    else
        setSize(frameWidth, frameHeight);
    
    // show the whole components
    setVisible(true);
  
    // set scrollbar value
    imageCanvas.setHScrollValue();
    imageCanvas.setVScrollValue();
  }
  

  /** resize the frame 
    * @param factor the factor to origanize the frame
    */
  public void setFrameSize(float factor){
   
    imageCanvas.setCanvasSize(factor);
    
    int w = getSize().width;
    int h = getSize().height;
    
    w = (int)(w*factor);
    h = (int)(h*factor);
     
    if ((w*h) > 400 * 400)
       super.setSize(400,400);
    else
       if ((w*h) < 200*200)
          super.setSize(200,200);
       else
          super.setSize(w,h);
   
    // validate
    validate();
 
    // layout
    doLayout();
    
  }
  

  /**
   * Handles the event. Returns true if the event is handled and
   * should not be passed to the parent of this component. The default
   * event handler calls some helper methods to make life easier
   * on the programmer.
   * @param evt the event
   * @see java.awt.Component#handleEvent
   */

  public void windowActivated(WindowEvent we) {}

  public void windowClosed(WindowEvent we) {}

  public void windowDeactivated(WindowEvent we) {}

  public void windowDeiconified(WindowEvent we) {}

  public void windowIconified(WindowEvent we) {}

  public void windowOpened(WindowEvent we) {}

  public void windowClosing(WindowEvent we)
  {
    if (dataFrame != null) {
      dataFrame.dataCanvas.imageFrame = null;
      dataFrame.dataCanvas.defaultImage = null;
      dataFrame.dataCanvas.interpolateImage = null;
    }
    
    // release frame
    dataFrame = null;
    dispose();	 
    
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
    
      if (target ==  vScrollbar) {
		
	// get translated value
	//imageCanvas.ty = ((Integer) evt.arg).intValue();	
	imageCanvas.ty = vScrollbar.getValue();
	
	// adjust the horizontal scrollbar (not necessary)
	//imageCanvas.setVScrollValue();
	
	// repaint the graphics
	imageCanvas.repaint();
	
      }
      // detect the horizontal scrollbar
      if (target ==  hScrollbar) {
	
	// get translated value
	// imageCanvas.tx = ((Integer) evt.arg).intValue();	
	imageCanvas.tx = hScrollbar.getValue();
	
	// adjust the horizontal scrollbar
	//imageCanvas.setHScrollValue();
	
	// repaint the graphics
	imageCanvas.repaint();
      }
    } 
    
    
    
  }
  
  

  public void actionPerformed(ActionEvent e)
  {
    
    String arg = e.getActionCommand();

    //"dimiss" pressed, then close the frame
    if ("Dismiss".equals(arg)) {
      // close the window
      if (dataFrame != null) {
	dataFrame.dataCanvas.imageFrame = null;
	dataFrame.dataCanvas.defaultImage = null;
	dataFrame.dataCanvas.interpolateImage = null;
      }
      
      dataFrame = null;
      dispose();
      
    }	
    
    
  }
  
}
