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


package ncsa.hdf.awt.plots;

import  java.awt.*;
import  java.lang.*;
import  java.util.Vector;

import  java.awt.event.*;

// XY-plot implementation
public class XYPlotPane extends Canvas implements MouseListener {

	// control class
	XYPlot 		plotCtr;

	// data being ploted (same as what plotCtr has)
	double  data[][];
	
	// annotation for each xy-plot
	String  annotation[] ;
	
	// Axis tag
	String  xTag = "X-Axis";
	String  yTag = "Y-Axis";

	// plot dataset range
	double xMin, xMax;
	double yMin, yMax;

	// plots number
	int  plotsNumber;

	// plots coordinates
	Vector  plots[];

	// color representation for each plot
	// Maybe there are serval line to be ploted
	Color   plotsColor[];
	
	// colors for each point to one plot
	Color	pColors[];   // special use for Histogram plot style

	// some of parameter to make a graphic
	public static final int  borderLen = 40;
	public static final int  extraLen    = 10;
	public static final int  arrowLen    = 05;
	public static final int  longScaleSize    = 04;
	public static final int  shortScaleSize   = 02;

	//  plot mode parameter constants
	public static final int  LINE 	   = 0;
	public static final int  POINT 	   = LINE+1;
	public static final int  HISTOGRAM = LINE+2;

	// plot mode (default)
	int     plotMode = LINE;

	// the boundary of the rectangle(plot)
	Rectangle  boundary = new Rectangle();
 
    	// Flicker-free update with translation by scrollbars
    	Image 		offScreenImage = null;
    	Dimension 	offscreensize;		
    	Graphics 	offGraphics;

	//  an indicator of the plot frame
    	boolean frameDisplayed = false;

	// set font
	// Create the new font for displaying the  data correctly 
    	// in the specified canvas within the scrollbar
    	Font dataFont        = new Font("Fixed", Font.PLAIN, 14);

	int  fontWidth = 14;
	int  fontHeight= 15;

	// zooming factor keep the drawable data range
	int  zoomFactor = 0;

	// current mouse coordinates
	int xPos=0, yPos=0;

	int oldCursor = Cursor.DEFAULT_CURSOR;

	// need to redraw the plot
	boolean  drawPlot = false;

    public XYPlotPane() {

	// If we plan to draw serval lines, each line should has own color 
	plotsColor = new Color[8];

	plotsColor[0] = Color.red;
	plotsColor[1] = Color.blue;
	plotsColor[2] = Color.orange.darker();
	plotsColor[3] = Color.cyan;
	plotsColor[4] = Color.green;
	plotsColor[5] = Color.magenta.darker();
	plotsColor[6] = Color.black;

	// set font
	setFont(dataFont);

	addMouseListener(this);
    }

    public XYPlotPane(XYPlot xyPlot, double dat[][]) {

	// default constructor
	this();
	
	// plot control
	this.plotCtr = xyPlot;

	// dataset of plot
	setData(dat);

    }

 

  // Set minimum size at SxS for current canvas  
  public Dimension getMinimumSize(){
    return new Dimension(360,360);
  }

 

   //  Set preferred  size at SxS for current canvas
   public Dimension getPreferredSize() {
	return getMinimumSize();
    }

  /** 
   * Sets the font of the component.
   * @param f the font
   */
  public synchronized void setFont(Font f) {
 
    // set Font for component
    super.setFont(f);
    
    // get new FontMetrics
    FontMetrics fontMetrics = getFontMetrics(getFont());
    
    // set font width & height
    fontHeight = fontMetrics.getHeight();
    fontWidth  = fontMetrics.charWidth('A');
    
  }

   // Set x-axis data range by a specified  data range  
   public void setXAxisRange(double data[]) {
	
	xMin = xMax = data[0];
	for (int i=1; i<data.length; i++) {
	    double tmp = data[i];

	    xMin = Math.min(tmp, xMin);
	    xMax = Math.max(tmp, xMax);
	}
	
	repaint();
    }

   // Reset axis data range for x-axis  
    public void setXAxisRange(double xmin, double xmax) {

	xMin = xmin;
	xMax = xmax;

	repaint();

    }

   // Set y-axis data range by a specified  data range  
   public void setYAxisRange(double data[]) {
		
	for (int i=0; i<data.length; i++) {
	    double tmp = data[i];

	    yMin = Math.min(tmp, yMin);
	    yMax = Math.max(tmp, yMax);
	}
	
	repaint();
    }

    // Reset axis data range for y-axis  
    public void setYAxisRange(double ymin, double ymax) {

	yMin = ymin;
	yMax = ymax;

	repaint();
    }

    /** Set annotation
     * @param strs the annotation for each xy-plot
     */
    public void setAnnotation(String[] strs) {
	annotation = strs;	
    }

    // Set X-Axis annotation(tag)
    public void setXAxisTag(String xTag) {
	this.xTag = xTag;	
    }

    // Set Y-Axis annotation(tag)
    public void setYAxisTag(String yTag) {
	this.yTag = yTag;	
    }

   /** set new plot data 
     * @param data the plot data
     */
    public void setData(double data[][]) {
     if (data != null) {
	/*  origanize data .... 
	x-axis: (data[0])  [x-axis scale ...]
	y1-axis:(data[1])  [ first plot  data ] 
	     11.1 23.2 ...
	y2-axis:(data[2])  [ second plot data ] 
	     -12.32 -34.1 ... */
	
	this.data = data;

	// set x-axis
	setXAxisRange(data[0]);

	repaint();
      }	
    }

    /** Get point refer to the graphic device
     * @param xVal the x  value(physical)
     * @param yVal the y  value(physical)
     * @return the point refer to the current graphic device 
     */
    public Point getDevicePoint(double xVal, double yVal) {
	
	// device value
	int x,y;
		
	x = (int)((xVal-xMin)/(xMax-xMin)*( boundary.width +1 )+boundary.x   );
	y = (int)((yMax-yVal)/(yMax-yMin)*( boundary.height+1 )+boundary.y   );

	return (new Point(x,y));
    }

   /** Get data point from the graphic device
     * @param x the x value(device)
     * @return the double value from the current graphic device 
     */
    public double  getDataPointx(int x) {
	
	// data value
	double xVal;
		
	xVal = (xMax-xMin) * (double)(x - boundary.x)/ ((double)(boundary.width+1)) + xMin;
	
	return xVal;
	
    }

   /** Get data point from the graphic device
     * @param y the y value(device)
     * @return the double value from the current graphic device 
     */
    public double  getDataPointy(int y) {
	
	// data value
	double yVal;
		
	yVal = yMax - ((y - boundary.y)/(boundary.height+1)*(yMax-yMin)) ;

	return yVal;
    }

    // Append plots  
    public void appendPlots() {

	// default
	appendPlots(data);

    }

   /**
     * Append draw point for the whole dataset
     * @param data the whole ploted dataset
     */
   public void appendPlots(double data[][]) {

    if (data!=null) {

     // how many line will be ploted ?
     plotsNumber = data.length - 1;

     if (plotsNumber > 0) {  // At least one line plots

	plots = null;
	plots = new Vector[plotsNumber];

	for (int kk=1; kk<=plotsNumber; kk++) {

	    // specify the new vector to hold the drawable points for each plot
	    plots[kk-1] = new Vector();
    
	    // how many points?
	    int  size = data[0].length;
	    
	    for (int i=0; i<size; i++) {

	    	// x value
	    	double   x = data[0][i];   // x-axis scale 

	    	// y value
	    	double   y = data[kk][i];  // plots value

	        Point pp = getDevicePoint(x,y);
	 
	        // append point
	        plots[kk-1].addElement(pp);

	    }
	}
      }
     }
    }

    //  Make annotation  
    public void makeAnnotation(Graphics gc) {

      // Panel size
      Dimension d = getSize();

      if (annotation != null) {

	// current color
	Color oldColor = gc.getColor();

	int h = d.height;
	int w = d.width;
	int x = borderLen + 8;
	int y = h - fontHeight/2;

	// draw each line
	for (int kk=0; kk<plotsNumber; kk++) { 
	
	    // set default color
	    gc.setColor(plotsColor[kk]);

	   if (annotation[kk] != null) { // anotation for kk-th plot
		
		int strLen = (annotation[kk].length() + 1) * fontWidth + 18  ;
		if ((x+strLen) > w) { // move to next line
		   
		    // adjust y
		    y -= (fontHeight - 1);
		
		    // adjust x
		    x = borderLen + 8;
		}

		// symbol
		gc.fillRect(x,y-8,12,8);		
		gc.setColor(oldColor);

		// annotation
		gc.drawString( annotation[kk], x+18 ,y);		
		x += strLen ;
	
	    } 
		
	} 

	// reset the color
	gc.setColor(oldColor);
      }
    }

   //  Draw xy-plot by drawable points
    public void drawPlotPoints(Graphics gc) {

        if ((plotMode==HISTOGRAM) && (pColors==null))
	    return;

	// current color
	Color oldColor = gc.getColor();

	setBackground(Color.white);

	// draw each line
	for (int kk=0; kk<plotsNumber; kk++) { 
	
	    // set default color
	    gc.setColor(plotsColor[kk]);

	   // get size of drawable point
	   int size = plots[kk].size();

	   if (size >1) { // one more drawable plots

		Point prePoint = (Point)plots[kk].elementAt(0);

		for (int i=1; i<size; i++) {

		     // current point		    
		     Point cPoint = (Point)plots[kk].elementAt(i);
	
		     switch(plotMode) {
	     		
		     case HISTOGRAM:
			// need to assign different palettes
			int pos = 255;
			if (size<=255) pos = i;
			gc.setColor(pColors[i]);
 
		     	// draw histogram
			Point xpt = getDevicePoint(data[0][i], yMin);

		     	gc.drawLine(cPoint.x, cPoint.y , cPoint.x, xpt.y);
			break;
	
		     case LINE:
		     	// draw line
		     	gc.drawLine(prePoint.x, prePoint.y, cPoint.x, cPoint.y);
			break;
		     case POINT:
			// draw plot(mark)
			gc.drawOval(prePoint.x-2, prePoint.y-2, 4, 4);
			break;
		     }

		     // move to next point
		     prePoint = cPoint;
		}		
	    }  // if (size > 1)
	} //for (int kk=0 ...)

	// reset the color
	gc.setColor(oldColor);

    }

    // Make an axis by current canvas size & drawable data 
    public void makeAxis(Graphics gc) {

	Dimension d = getSize();
	// get current canvas size
	int width = d.width;
	int height= d.height;

	// get new boundary
	int lx = 3*borderLen;
	int rx = width - 2*borderLen;
	if (rx<0)
	   rx = lx;

	int uy = 2*borderLen;
	int dy = height - 2*borderLen;
	if (dy<0)
	   dy = uy;

	// reshape the boundary
	boundary.setBounds(lx,     // x
			 uy,     // y
			 rx-lx,  // width
			 dy-uy); // height 

	// draw axis
	gc.drawLine (lx,dy,rx+extraLen,dy);
        gc.drawLine (lx,dy,lx,uy-extraLen);

	// draw label of value
	gc.drawString(yTag, lx-3*fontWidth, uy-extraLen-fontHeight);

	// draw arrow   ^
	gc.drawLine (lx,uy-extraLen,lx+arrowLen,uy-extraLen+arrowLen);
	gc.drawLine (lx,uy-extraLen,lx-arrowLen,uy-extraLen+arrowLen);

	// draw arrow  >
	gc.drawLine (rx+extraLen,dy,rx+extraLen-arrowLen,dy+arrowLen);
	gc.drawLine (rx+extraLen,dy,rx+extraLen-arrowLen,dy-arrowLen);

	// draw label of row number
	gc.drawString(xTag, (width-3*borderLen)/2, dy+3*fontHeight);

	// draw scale (5 points)
	for (int i=0;i<4;i++) {

   	    // long size
    	    gc.drawLine(lx+i*(rx-lx+1)/3,dy-longScaleSize,
	                lx+i*(rx-lx+1)/3,dy+longScaleSize);

	   // compute scale label by specify data value
    	   double yVal = yMin + (yMax - yMin)*i/3.0;   
    	   double xVal = xMin + (xMax - xMin)*i/3.0;

    	   String ylab = Double.toString(yVal);  // vdata value fof selected field
	   int pos;
	   String yLabel;
	   if ((pos=ylab.indexOf(".")) >= 0)
	      yLabel = new String(ylab.substring(0,pos+2));
	   else
	      yLabel = new String(ylab);

    	   if (yLabel.length() > 12) yLabel = yLabel.substring(0,12);
      
	   String xLabel = Integer.toString((int)xVal);  // colnum
           if (xLabel.length() > 5) xLabel = xLabel.substring(0,5);
  
   	   int xPos;
	   if  (xLabel.length() > 3)
 		xPos = lx+i*(rx-lx+1)/3-8 ;
	   else
		xPos = lx+i*(rx-lx+1)/3   ;

	   // colnum label
    	   gc.drawString(xLabel,  // string
		 	 xPos, dy+20);

    	   gc.drawLine(lx-longScaleSize,dy+i*(uy-dy+1)/3,
	       	       lx+longScaleSize,dy+i*(uy-dy+1)/3);  // y-axis

	   // vdata label (20) 30-yLabel.trim().length()
   	   gc.drawString(yLabel,
		 	 lx-(yLabel.trim().length()*fontWidth)-2*fontWidth,dy+i*(uy-dy+1)/3 );
  


  	   // short scale
    	   if (i != 3) {
      		gc.drawLine(lx+(int)((i+0.5)*(rx-lx+1)/3),dy-shortScaleSize,
		            lx+(int)((i+0.5)*(rx-lx+1)/3),dy+shortScaleSize);

      		gc.drawLine(lx-shortScaleSize,dy+(int)((i+0.5)*(uy-dy+1)/3),
		            lx+shortScaleSize,dy+(int)((i+0.5)*(uy-dy+1)/3));
   	   }
  	}  // for (int i=0; ...)
    }

    /**
     * Paints the component.
     * @param g the specified Graphics window
     * @see java.awt.Component#paint
     */
    public void paint(Graphics g) {

	Dimension d = getSize();

	// make axis
	makeAxis(g);

	// prepare drawable points
	appendPlots();

	// plot the point;
	drawPlotPoints(g);

	// make annotation
	makeAnnotation(g);

    	// get current graphics color 
    	Color gColor = g.getColor();
 
 	// set color
    	g.setColor(Color.lightGray);
  
	// separate line
	g.draw3DRect(0,d.height-1, d.width, 1, true);

 	// reset the color
    	g.setColor(gColor);
  
    }

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

 public int  getZoomFactor() {
	return zoomFactor;
    }

   // set plot mode (line or plot)
    public void setPlotMode(int mode) {

	this.plotMode = mode;
	
	repaint();
    }

  /* If a size of the plot pane is appropriate 
   * xy-plot should be painted
   */
  public synchronized void checkSize() {
    
    // set canvas size
    setSize(500,400);
    
    if (!frameDisplayed) {

      // popup frame
      plotCtr.popup();
      frameDisplayed = true;
    }
    
  }

    /**
     * java.awt.event.MouseListener stuff
     */
    public void mouseClicked(MouseEvent e)  {
    }

    /**
     * java.awt.event.MouseListener stuff
     */
    public void mousePressed(MouseEvent evt)  {     
    }

    /**
     * java.awt.event.MouseListener stuff
     */
    public void mouseReleased(MouseEvent evt)  {
    }

 

    /**
     * java.awt.event.MouseListener stuff
     */
    public void mouseEntered(MouseEvent evt)  {
    }

    /**
     * java.awt.event.MouseListener stuff
     */
    public void mouseExited(MouseEvent evt)  {
    }
}
