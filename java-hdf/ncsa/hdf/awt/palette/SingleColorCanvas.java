
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

package ncsa.hdf.awt.palette;

import java.awt.*;
import java.awt.event.*;

// Object that will be used to indicate the drawable point
class ColorPoint {
	
	boolean 	drawable = false;
	
	Point 		point;

    public ColorPoint() {
    }

    public void setDrawable() {
	this.drawable = true;
    }

    public void setPoint(Point pt) {
	this.point = pt;
     }

    public Point getPoint() {
	return(this.point);
     }

    public void unSetDrawable() {
	this.drawable = false;
    }

    public boolean isDrawable() {
	return this.drawable;
    }
}


// Class for red palette 
public class SingleColorCanvas extends Canvas implements MouseListener,
  MouseMotionListener {

        // Palette Canvas
        PaletteCanvas paletteCanvas;
    
	// single color states
	ColorPoint[] cPoints;

	// define the drawing mode
	static final int POINT = 0;
	static final int LINE  = 1;

	// Define the Canvas type
	static final int RED   = 0;
	static final int GREEN = 1;
	static final int BLUE  = 2;

        // define the current drawable mode
        int  drawableMode = POINT;
	
	// canvas type
	int canvasType = RED;  
  
	// the red palette value
	byte[] colorData;
	
	// transform data
	byte[] data  = new byte[256];

	// default width and height of canvas
	int	cWidth = 256;
	int  	cHeight= 64;

	/** variables for duble-buffer  */
  	Image 		offScreenImage = null;
  	Graphics 	offGraphics;
 
	/** translated variable for scroll panel */
  	int tx=0,  ty=0;       // offset translate

	// current runtime version
	String jversion = System.getProperty("java.version");

	// keep track of the mouse points
	int  xpos, ypos;

	// line scope
	Rectangle lineRect = new Rectangle();
	
	// line indicator
	boolean drawLine = false;

    // default construct
    public SingleColorCanvas() {
	
	// resize the canvas
	this.setSize(cWidth, cHeight);
	addMouseListener(this);
	addMouseMotionListener(this);
    }

    // default construct
    public SingleColorCanvas(PaletteCanvas pCanvas) {
	this();
      this.paletteCanvas = pCanvas;
    }

    // constructor
    public SingleColorCanvas(PaletteCanvas pCanvas, byte[] reds) {
	
	this(pCanvas);
	
	// initialize the cPoints
	cPoints = new ColorPoint[256];
	
	// set color value
	colorData = reds;
	
	// do something here for initialization
	data = null;
	data = reds;
	
	if (colorData != null) {
	    
	    // data size
	    int size = colorData.length;
	    
	    // for each point
	    for (int i=0; i<size; i++) {
		
		ColorPoint cPoint = new ColorPoint();
		
		// get device point from color data
		// then assign to the cPoint
		cPoint.setPoint(dataToDevice(colorData[i],i));
		
		cPoints[i] = cPoint;
		
	    }

	}
	
    }

     // set drawable point data, to device point from palette value
     public void setData(byte[] reds) {

        if (cPoints == null)
	    cPoints = new ColorPoint[256];
	    
        // set color value
	colorData = reds;
	
	// do something here for initialization
	data = null;
	data = reds;
	
	if (colorData != null) {
	    
	    // data size
	    int size = colorData.length;
	    
	    // for each point
	    for (int i=0; i<size; i++) {
		
		ColorPoint cPoint = new ColorPoint();
		
		// get device point from color data
		// then assign to the cPoint
		cPoint.setPoint(dataToDevice(colorData[i],i));
		
		cPoints[i] = cPoint;
	    }
	} 
	
	// repaint
	repaint();
    }

    // get new data by current drawable points
    public byte[] getData() {
    
       byte[] dat = new byte[256];
    
       for (int i=0; i<256; i++) {
	    Point pt = cPoints[i].getPoint();
	    dat[i] = (byte)deviceToData(pt);
       }
	    
       return dat;
    }

    // Return the device value based on the color value  
    public Point dataToDevice(byte value, int ind) {
	
	// value should between -128 ~~~ 127
	// ind should be X value
	// canvas size is  256 * 64
	short dat = value;
	if (value<0)
	   dat = (short)(256 + value);
	
	float f = 64.0f/256.0f;
	
	int dPoint ;
	dPoint = (int)(f*dat);
	dPoint = cHeight - dPoint;
	
	return (new Point(ind, dPoint));
	
    }

    // Return the color value based on the device point
    public byte deviceToData(Point point) {
	// transform factor
	float f = 256f/64f;
	
	// get the device point
	int y = point.y;

	// compute the palette value
	int val = (int)f*(cHeight - y);

	return (byte)val;
	
    }

    // drawable model
    public void setDrawMode(int mode) {
	this.drawableMode = mode;
	repaint();
    }

    // which kind of canvas type?
    public void setCanvasType(int type) {
	this.canvasType = type;
	repaint();
    }

    // Confirm the point is in the drawable  canvas
    public boolean atBoundary(int x, int y) {

	if ((x<cWidth) && (y<cHeight) && (x>0) && (y>0))
	    return true;
	else
	    return false;

    }

    // set to desired kind of palette value
    public void resetPalette() {
   
	if (canvasType == RED)
	   paletteCanvas.setRedValues(getData());
	else {
	   if (canvasType == GREEN)
	      paletteCanvas.setGreenValues(getData());
	   else
	      paletteCanvas.setBlueValues(getData());
	}

    }

    // adjust the  drawable points array
    public void adjustPoints(int x1,int y1, int x2, int y2) {

	int dx = x2 - x1;
	int dy = y2 - y1;

	if (dx != 0) {
	if (dx>0) {  // draw from left to right

		for (int i=1; i<dx; i++) {

		    float factor = (float)dy / (float)dx;

		    int   deta = (int) ( i * factor);

		    cPoints[x1+i].setPoint(new Point(x1+i, y1+deta));
		}
	}
	else {  // draw from right to left

		dx = Math.abs(dx);
		for (int i=1; i<dx; i++) {

		    float factor = (float)dy / (float)dx;

		    int   deta = (int) ( i * factor);

		    cPoints[x1-i].setPoint(new Point(x1-i, y1+deta));
		}
	}
 
	} // if (dx != 0)

    }

    /**
     * Paints the component.
     * @param g the specified Graphics window
     * @see java.awt.Component#paint
     */
    public void paint(Graphics g) {

     Color bgColor = g.getColor();

     // drawbaground
     for (int j=0; j<64; j++) {
	if (canvasType == RED)
	   g.setColor(new Color(255-j*2,0,0)); // red
	if (canvasType == GREEN)
	   g.setColor(new Color(0,255-j*2,0)); // green
	if (canvasType == BLUE)
	   g.setColor(new Color(0,0,255-j*2)); // blue
	g.drawLine(0,j,255,j);
     }

     // recover the color
     // g.setColor(bgColor);
     g.setColor(Color.white);

     if (cPoints != null) { 
	
	  if ((drawableMode == POINT) ||(drawableMode == LINE) ) {
	    ColorPoint prev = cPoints[0];
	    for (int i=1; i<256; i++) {
		
		ColorPoint cp = cPoints[i];
		if ((prev != null) && (cp != null)) {
		   Point prevPoint = prev.getPoint();
		   Point cpPoint   = cp.getPoint();
		
		  if ((prevPoint != null) && 
		      (cpPoint != null))
		      g.drawLine(prevPoint.x,prevPoint.y,cpPoint.x,cpPoint.y);
		}
 
		prev = cp;
	    }
	  } 
    }
      
     
    if (drawLine) {
	g.drawLine(lineRect.x, lineRect.y, lineRect.width, lineRect.height);
        drawLine = false;
     }
  
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
    offGraphics.fillRect(0,0,d.width,d.height);    
    offGraphics.setColor(getForeground());
    
    
    // draw the current frame to the off-screen 
    paint(offGraphics);
    
    //then draw the image to the on-screen 
    g.drawImage(offScreenImage, 0, 0, null);
    
  }

    /**
     * java.awt.event.MouseListener stuff
     */
    public void mouseClicked(MouseEvent e)  {
    }

    /**
     * java.awt.event.MouseListener stuff
     */
    public void mousePressed( MouseEvent evt )  {

	int x = evt.getX();
	int y = evt.getY();

	// keep old rgb value
	paletteCanvas.pushPaletteValue();

	if (atBoundary(x,y)) {
	   // keep this point
	   xpos = x;
	   ypos = y;

	   cPoints[x].setPoint(new Point(x,y));
	   repaint();
	}
    }

    /**
     * java.awt.event.MouseListener stuff
     */
    public void mouseReleased(MouseEvent evt)  {

	int x = evt.getX();
	int y = evt.getY();

	// for  LINE drawable model
	if (atBoundary(x,y)) {

	   if (drawableMode == LINE) { // keep the point moving
		// add this point
        	cPoints[x].setPoint(new Point(x,y));
	
		// adjust the drawable point
		adjustPoints(xpos,ypos,x,y);		
	       
                repaint();
	   }
	   
	   // reset palette
	   resetPalette();

	}

	// make it effective	
	if (paletteCanvas != null)
	   paletteCanvas.mouseReleased(evt);
    }

    /**
     * java.awt.event.MouseListener stuff
     */
    public void mouseEntered(MouseEvent e)  {
    }

    /**
     * java.awt.event.MouseListener stuff
     */
    public void mouseExited(MouseEvent e)  {
    }

    /**
     * java.awt.event.MouseMotionListener stuff
     */
    public void mouseDragged(MouseEvent evt)  {
	
	int x = evt.getX();
	int y = evt.getY();

	// set to default
	drawLine = false;

	// for  POINT drawable model
	if (atBoundary(x,y)) {

	   if (drawableMode == POINT) { // keep the point moving
		// add this point
        		cPoints[x].setPoint(new Point(x,y));
	
		// adjust the drawable point
		adjustPoints(xpos,ypos,x,y);		
	
		xpos = x;
		ypos = y;
	       
                	repaint();

		// reset palette	
		resetPalette();
	
	   }
	   else {	
		drawLine = true;
		lineRect.setBounds(xpos,ypos,x,y);
		repaint();
	   }

	}
	else
	   repaint();
    }

    /**
     * java.awt.event.MouseMotionListener stuff
     */
    public void mouseMoved(MouseEvent e)  {
    }
}
