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
import java.io.*;
import java.util.Vector;
import java.lang.*;

import ncsa.hdf.hdflib.*;

/** Display of HDF Vdata */
class JHVVdataCanvas extends Canvas implements MouseListener{
    
    // the frame of the spreadsheet
    JHVVdataFrame	vdataFrame;
   
    // An HDF vdata value  
    String[][]       vdata = null;

    // variables related to HDF Vdata
    int vdata_id;
 
    // vdata fields list
    String[]   selectedVdataFieldName = null;
    
    // the current canvas size
    int           canvasWidth ;
    int           canvasHeight;
    
    // the cell size of the spreadsheet
    public  static  Rectangle CellRect = null ;

    // dynamic cell size of each field
    public  Rectangle[]  vdataCellRect = null;
   
    // spreadsheet width totally
    public   int  spreadsheetWidth = 0;
    
    // how many lines?
    public  static  int cellNumberByRow;
    
    // how many colnums?
    public  static  int cellNumberByColnum;
    
    // 10 digits display for each cell 
    public  static  int NumberLength = 18;
    
    // create the new font with the specified font size
    public static   int defaultFontSize = 14;
    
    // Create the new font for displaying the  data correctly 
    // in the specified canvas within the scrollbar
    Font dataFont        = new Font("Times", Font.PLAIN, defaultFontSize);
    
    // default font width & height
    public static   int fontWidth = 14;
    public static   int fontHeight= 15;
    
    // Flicker-free update with translation by scrollbars
    Image 		offScreenImage = null;
    Dimension 	offscreensize;		
    Graphics 	offGraphics;
     
    //  the indicator of the spreadsheet frame
    boolean frameDisplayed = false;
    
    // translated value
    // All subsequent operations on this graphics context will be relative
    // to this origin.
    int tx = 0;
    int ty = 0;
    
    // offset value of the scrollbar
    int hOffset = 0;
    int vOffset = 0;
       
    // the position and size of each cell in the spreadsheet
    Rectangle [][] cells = new Rectangle[120][60];
     
    // the indicator that the spreadsheet has been selected 
    boolean drawFlag = false;
    
    // draw area
    Rectangle drawRectangle = null;

    /* The following is for displaying the multiple  colnum selection */

    /* ===========================================
    // draw by colnum
    boolean   drawColnumFlag = false ;
    Rectangle drawRectangleByColnum = null ;

    // selectedColnum
    int selectedColnum = -1 ;
    =============================================== */

    /* the data object */
    JHVVdataPlot      plotData[];

    /* selected colnum vector */
    Vector   selectedColnumVector = new Vector();

    /** xy-plot frame indicator */
    boolean  plotFrameDisplayed = false;
    
    /** frame  for xy-plot */
    JHVVdataPlotFrame  plotFrame = null;

    // flag
    boolean findDataRangeFlag = true;

    /** Calculate each cell size by row of a spreadsheet based on provided
     *  HDF vdata.  That is metedata associated with different order.
     */
    public void calculateCellSize() {

	// compute spreadsheet width
	spreadsheetWidth  = 0;

	// set cell size based on the current selected font.
	CellRect = new Rectangle(NumberLength*(fontWidth+1), fontHeight + 4);

	int   size = Math.max(vdataFrame.numberOfFields,
	                       vdataFrame.selectedVdataFieldsNumber);
	
	// big enough ?
	vdataCellRect = new Rectangle[size];
    
	// for each field
	for (int i=0; i<vdataFrame.selectedVdataFieldsNumber; i++) {
	
	    // get selected field index
	    int index = vdataFrame.selectedVdataFields[i];
	    
	    int datatype = vdataFrame.vdataFieldDatatype[index];
	    if ((datatype & HDFConstants.DFNT_LITEND) != 0) {
		datatype -= HDFConstants.DFNT_LITEND;
	    }
	    int order    = vdataFrame.vdataFieldOrder[index];
	    String fldName = vdataFrame.vdataFieldName[index];
	
    	    // assume repeat number is less than 3
	    if ((order>3) && (datatype != HDFConstants.DFNT_CHAR8)) // &&(dataType != HDFConstants.DFNT_UINT8) )	                      
	       order = 3;

	    if ((datatype != HDFConstants.DFNT_CHAR8) && 
	        (datatype != HDFConstants.DFNT_UCHAR8)) {
	    
		// set cell size based on the current selected font.
		int numLength = NumberLength; // default
		if ((datatype == HDFConstants.DFNT_INT8)  ||
		    (datatype == HDFConstants.DFNT_UINT8)  ||
		    (datatype == HDFConstants.DFNT_INT16)  ||
		    (datatype == HDFConstants.DFNT_UINT16) )
		   numLength /= 2;

		else if ((datatype == HDFConstants.DFNT_FLOAT32)  ||
		        (datatype == HDFConstants.DFNT_FLOAT64) )		   
		      numLength += 2;

		int len = Math.max(order * numLength, fldName.length());
		//int len = Math.max(order * NumberLength, fldName.length());
	     
	        //if (order > 2) 
	        //   vdataCellRect[i] = new Rectangle(len*(fontWidth+1)/2, fontHeight + 4);
	        // else
		   vdataCellRect[i] = new Rectangle(len*(fontWidth+1), fontHeight + 4);
	    }
	    else {
		// char
		int len = Math.max(order, fldName.length());	
		vdataCellRect[i] = new Rectangle( len * (fontWidth+1), fontHeight + 4);
	    }
	    // compute spreadsheet width
	    spreadsheetWidth += vdataCellRect[i].width;
	}
	
    }


    public void destroyXYPlot() {

	// remove all elements
	selectedColnumVector.removeAllElements();

	// set frame null
	plotFrame.dispose();

	plotFrame = null;
	plotFrameDisplayed = false;

    }

  /** set size for current canvas 
   * @param w the width
   * @param h the height
   */
  public void setCanvasSize(int w,int h) {
    // set canvas size
    canvasWidth = w;
    canvasHeight= h;
  }

  // Set minimum size at SxS for current canvas  
  public Dimension getMinimumSize(){
    return new Dimension(canvasWidth, canvasHeight);
  }

 //  Set preferred  size at SxS for current canvas  
  public Dimension getPreferredSize() {
    return getMinimumSize();
  }

   /** get cell number for each row or colnum */
   public void getCellNumber(int w, int h) {

      cellNumberByRow    = h / CellRect.height  ;

      int colNum = 0;
      int width = 0;

      for (int i=hOffset; i<vdataFrame.selectedVdataFieldsNumber; i++, colNum++)  {

	width += vdataCellRect[i].width;
	if (width > w)
	   break;
      }

      cellNumberByColnum  = colNum;

    }

 /** draw the spreadsheet */
  public synchronized void paint(Graphics g) {

    // get background color
    Color bColor = getBackground();

    // get current graphics color
    Color gColor = g.getColor();

    // get current canvas size
    int w = getSize().width;
    int h = getSize().height;
    
    /**************************************************************
    // compute the current lines & colnums based on the canvas size
    cellNumberByColnum = w / CellRect.width  ;
    cellNumberByRow    = h / CellRect.height  ;
    ***************************************************************/

    computeCellPosition();

    // repaint the info panel
    vdataFrame.rowInfoCanvas.init(h-2);
    vdataFrame.colnumInfoCanvas.init(w-2);
    
    int startx = 1;
    int starty = 1;
	
    // draw the rectangle
    g.drawRect(startx,starty,w-2,h-2);

    // click on cell
    if (drawFlag) {
      // set background color
      g.setColor(Color.cyan);

      // draw cell
      if (drawRectangle != null)
	g.fillRect(drawRectangle.x, drawRectangle.y, 
		   drawRectangle.width-1, drawRectangle.height-1);
    }

    // set background color
    g.setColor(Color.cyan);

    // if selected on multipal colnum, then draw it.
    drawSelectedColnum(g, selectedColnumVector);    

    // set color
    g.setColor(Color.lightGray);
    
    // draw grid
    startx = 1;
    starty = 1;
   	
    // draw the grid (Horizontal)
    for (int i=1; i<= cellNumberByRow; i++) {
      starty += CellRect.height;
      g.draw3DRect(startx+1, starty+1, w-3, 1, true);
    }
	
    startx = 1;
    starty = 1;
    // draw the grid (Vertical)
    for (int i=1; i<= cellNumberByColnum; i++) {
      //startx += CellRect.width;
      if (vdataCellRect[hOffset+i-1] != null)
	startx += vdataCellRect[hOffset+i-1].width;
	
      g.draw3DRect(startx+1, starty+1, 1, h-3, true);
    }  
    
    // reset the color
    g.setColor(gColor);
  	
    for (int i =0; i<=cellNumberByRow; i++) {
    
      startx = 1;
      for (int j=0; j<=cellNumberByColnum; j++) {
	// display position
	if (j>0) {
	    if (vdataCellRect[hOffset+j-1] != null)
	      	startx += vdataCellRect[hOffset+j-1].width ;
	    else 
		break;
	}
	starty = CellRect.height * (i+1) ;
	
	// display float number
	String dispStr = "";
	if (((vOffset+i) < (vdataFrame.selectedVdataRecordsNumber)) &&
	    ((hOffset+j) < (vdataFrame.selectedVdataFieldsNumber)))    {
		
	  // assign value
	  dispStr = vdata[vOffset+i][hOffset+j];
   	  g.drawString(dispStr, startx + 4, starty);
		
	}
	else
	  continue; 
      }
    } 

  }

    // draw multipal colnum
    public void drawSelectedColnum(Graphics gc, Vector mColnum) {

	// for each selected colnum
	for (int kk=0; kk<mColnum.size(); kk++) {

	  // specify the drawable scope
      	  Rectangle rect = null;

	  int  col = ((Integer)(mColnum.elementAt(kk))).intValue();
	  int selectedColnum = col;

	  if ((plotData[selectedColnum] != null) && 
	      (plotData[selectedColnum].drawColnumFlag)) {

	    // adjust selected colnum to be viewable
	    col -= hOffset;

    	    // confirm cell
    	    if ((col<= cellNumberByColnum) && (col >=0) ) {
	
    	   	if (cells[0][col] != null) {
      	      	   rect = cells[0][col];

  	   	}
	   
   	   	// reset drawRectangleByColnum
	   	if (rect != null)
	      	   rect.setSize(rect.width,
			       rect.height*cellNumberByRow);

            } 

	   if (rect != null)
	      gc.fillRect(rect.x, rect.y, 
		   rect.width-1, rect.height-1);
	  } // need to draw
	} // for ...

    }

 /**
   * Updates the component. This method is called in
   * response to a call to repaint. You can assume that
   * the background is not cleared.
   * @param g the specified Graphics window
   * @see java.awt.Component#update
   */
  public   void update(Graphics g) {

    Dimension d = getSize();
    setCanvasSize(d.width, d.height);
    
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

  /**
   * If the size of the spreadsheet is appropriate 
   * the spreadsheet will be displayed
   */
  public synchronized void checkSize() {
    
    // set canvas size
    setSize(canvasWidth, canvasHeight);

    if (!frameDisplayed) {
      // popup frame
      vdataFrame.popup();
      frameDisplayed = true;
    }
    
  }

   /** Reshapes the Component to the specified bounding box. 
   * @param x the x coordinate
   * @param y the y coordinate
   * @param width the width of the component
   * @param height the height of the component
   */
  public synchronized  void setBounds(int x, int y, int w, int h) {
  
    super.setBounds(x, y, w, h);
  
    // resize horizontal scrollbar
    setHScrollbarValue();
    
    // resize the vertical scrollbar
    setVScrollbarValue();
    
    // recompute the cell position
    computeCellPosition();
  }
  

   /** Compute the each cell position after the canvas changed */
  public void computeCellPosition() {

    // current canvas size
    int w = getSize().width;
    int h = getSize().height;
    
    /*****************************************
    // spreadsheet lines & colnums
    cellNumberByColnum = w / CellRect.width;
    cellNumberByRow    = h / CellRect.height;
    ******************************************/
    getCellNumber(w,h);

    // specify the dimension for the cells
    cells = null;
    cells = new Rectangle[cellNumberByRow][cellNumberByColnum+1];
    
    int starty = 2;
    for (int i=0; i<cellNumberByRow; i++ ) {

      starty = CellRect.height*i + 3;
      
      int startx = 2;
      for (int j=0; j<cellNumberByColnum; j++) {
	 
	// x position
	//startx = CellRect.width*j + 3;
        if (j>0)
	    startx += vdataCellRect[hOffset+j-1].width;
	    
	// set rectangle
        
	if (cells[i][j] == null)
	  cells[i][j] = new Rectangle(startx+3, starty, 
				      vdataCellRect[hOffset+j].width, CellRect.height);
	else
	  cells[i][j].setBounds(startx+3, starty, 
	                      vdataCellRect[hOffset+j].width, CellRect.height);
	    
      }
    }
  }
    

 /** Adjust the Scrollbar value by the specifyed dataset range	 */
  void setHScrollbarValue() {

    // get current canvas size
    int canvasWidth = getSize().width;
    int canvasHeight= getSize().height;

    // canvas is valid?
    if ((canvasWidth <= 0)||(canvasHeight<=0)) { 
      return;
    }
    
    //Shift everything to the right if we're displaying empty space
    //on the right side.   
    int width = spreadsheetWidth;
    
    if ((tx + canvasWidth) > width) {
      int newtx = width - canvasWidth;
      if (newtx < 0) {
	newtx = 0;
      }
      tx = newtx;
    }
    
    int p = (int) (canvasWidth * 0.9);
    int m = (int)(width - (canvasWidth - p));
    vdataFrame.hScrollbar.setValues(
	     //draw the part of the dataset that starts at this x:
	     tx, 
	     //amount to scroll for a "page":
	     p,
	     //minimum image x to specify:
	     0,
	     //maximum image x to specify:
	     m);

    //"visible" arg to setValues() has no effect after scrollbar is visible.
    vdataFrame.hScrollbar.setBlockIncrement(p);

    return;
  }

 /** Adjust the Scrollbar value by the specifyed dataset range	 */
  void setVScrollbarValue() {

    // get current canvas size
    int canvasWidth = getSize().width;
    int canvasHeight= getSize().height;
    
    // canvas is valid?
    if ((canvasWidth <= 0)||(canvasHeight<=0)) { 
      return;
    }
        
    //Shift everything to the right if we're displaying empty space
    //on the right side.
    
    int height = vdataFrame.selectedVdataRecordsNumber * CellRect.height + 5;
 
    if ((ty + canvasHeight) > height) {
      int newty = height - canvasHeight;
      if (newty < 0) {
	newty = 0;
      }
      ty  = newty;
    }
    
int p = (int) (canvasHeight * 0.9);
int m = (int)(height - (canvasHeight - p));
    vdataFrame.vScrollbar.setValues(
             //draw the part of the dataset that starts at this x:
             ty, 
	     //amount to scroll for a "page":
	     p, //(int)(canvasHeight*0.9),
	     //minimum image y to specify:
	     0,
	     //maximum image y to specify:
	     m); //height - canvasHeight);

    //"visible" arg to setValues() has no effect after scrollbar is visible.
    vdataFrame.hScrollbar.setBlockIncrement(p); //(int)(canvasHeight*0.9));

    return;
  }
  

  /** set scrollbar offset(vertical) */
  public void setVoffset() {
    
    vOffset = ty/CellRect.height ;
	
    // adjust vOffset so we can see the last row clearly
    if (vOffset>1)
	++vOffset; 

    // set find data range
    findDataRangeFlag = false;

    if (selectedColnumVector.size() != 0)
    	// for xy-plot
    	drawXYPlot(getPlotZoomFactor());

  }

  /** set horizontal scrollbar offset value */
  public void setHoffset() {
    
    int offset = 0;
    int width  = 0;
    int i      = 0;
    for (i=0; i<vdataFrame.selectedVdataFieldsNumber; i++) {

	width += vdataCellRect[i].width;
	if (width > tx) {
	   break;
	} else {
	   ++offset;
	}
    }

    hOffset = offset;

    // more than one cell
    if (tx>vdataCellRect[0].width)
        ++hOffset;
  }

  public void mouseReleased(MouseEvent e) {}

  public void mouseEntered(MouseEvent e) {}

  public void mouseExited(MouseEvent e) {}

  public void mouseClicked(MouseEvent e) {}

  public void mousePressed(MouseEvent e)
  {
    int x = e.getX();
    int y = e.getY();
    drawFlag = false;
    
    // when mouse down , paint the cell to deep gray(darkGray)
    // first using the mouse position to get the cell position
    // int col = x / CellRect.width  ;
    int row = y / CellRect.height ;
    
    int col = 0;
    int len = 0;
    for (int i=hOffset; i<cellNumberByColnum+hOffset; i++, col++) {
      len += vdataCellRect[i].width;
	if (len>x)
	  break;
    }
    
    // confirm cell
    if ((col<= cellNumberByColnum) && (row<cellNumberByRow)) {
      
      Rectangle rect = null;
      
      if (cells[row][col] != null)
	rect = cells[row][col];
      
      if (rect != null) {
	// confirm this cell will be redraw
	if (rect.contains(x,y)) {  
	  // the specified point lies inside a rectangle
	  drawFlag = true;
	  drawRectangle = rect;	
	}
      }
      repaint();
    }
  }

 /** Draw by colnum
   * @param x the x coordinate
   * @param y the y coordinate
   */
  public void drawByColnum( int x, int y) {

    // set draw flag
    drawFlag = false;

    // define plotData
    if (plotData == null)
	plotData = new JHVVdataPlot[vdataFrame.selectedVdataFieldsNumber];

    // invalid selection
    int selectedColnum = getSelectedColnum(x,y) ;

    // set find data range
    findDataRangeFlag  =  true;
    
    // set draw flag
    boolean  drawColnumFlag = true;

    if ( selectedColnum > -1) { // valid selection
   
      // forget the field which contains of more then one data number
      // int dt = vdataFrame.vdataFieldDatatype[selectedColnum];
      int order = vdataFrame.vdataFieldOrder[selectedColnum];
      if (order>=2) return;

      if (plotData[selectedColnum] != null) {	// basic information   
	   drawColnumFlag = plotData[selectedColnum].drawColnumFlag;
	   plotData[selectedColnum].drawColnumFlag = !drawColnumFlag;

      }
      else
	   plotData[selectedColnum] = new JHVVdataPlot();

      //  add or remove the current selection
      Integer obj = new Integer(selectedColnum);

      if ((drawColnumFlag) && (!selectedColnumVector.contains(obj))) {

	  // current support 6 ploted object
	  if (selectedColnumVector.size()<6)
	     // add
	     selectedColnumVector.addElement(obj);
	}
	else
	  // remove
	  selectedColnumVector.removeElement(obj);

      // specify the drawable scope
      Rectangle drawRectangleByColnum = null;
   
      if (drawColnumFlag) {

        // set plot object
	plotData[selectedColnum].drawColnumFlag = drawColnumFlag;
	plotData[selectedColnum].setDrawableRectangle(drawRectangleByColnum);
	plotData[selectedColnum].setSelectedColnum(selectedColnum);

        repaint();

	drawXYPlot(getPlotZoomFactor());
      } 
    } 
    else {
    	repaint();
    }
  }

    public int getPlotZoomFactor() {
	
	if (plotFrame != null) {
	   if (plotFrame.plotCanvas != null)
	      return  plotFrame.plotCanvas.getZoomFactor();
	   else
	      return 0;
	} else return 0;

    }

 /** Draw xy-plot 
   */
  public void drawXYPlot( ) {

	// default
	drawXYPlot(0); 
  }

 /** Draw xy-plot 
   */
  public void drawXYPlot( int zoomFactor ) {
  
	// current selected colnum
        Vector mColnum = selectedColnumVector;

 	//  vdata xy-plot
	int  dataNumber = cellNumberByRow;
	
	// adjust data number by zoom factor
	if (zoomFactor > 0) {
	   for (int kk=0; kk<zoomFactor; kk++)
		dataNumber /= 2;
	}
	else {
	   if (zoomFactor < 0)
	      for (int kk=0; kk<(-zoomFactor); kk++)
		  dataNumber *= 2;
	}
	// adjust data number
	if ((vOffset+dataNumber) > vdataFrame.selectedVdataRecordsNumber) 
	    dataNumber = vdataFrame.selectedVdataRecordsNumber - vOffset ;

	// At least one data is exist.
	if (dataNumber<=0) dataNumber = -dataNumber + 1;

	double tmpData[][] = null;
	tmpData = new double[mColnum.size()+1][dataNumber];

	String tmpStr[]    = new String[8];

	// organize data
	for (int i=0; i<dataNumber; i++) {  // for x & y
	    tmpData[0][i] =  (double) (vOffset + i + 1);

	    // for each selected colnum
	    for (int kk=0; kk<mColnum.size(); kk++) {

	      try {
	        // selectedColnum ?
	    	int selectedColnum = ((Integer)(mColnum.elementAt(kk))).intValue();
		String valStr = (String)vdata[vOffset+i][selectedColnum].trim();
	    	tmpData[kk+1][i]   = Double.valueOf(valStr).doubleValue();
	      } 
	      catch (Exception e) { tmpData[kk+1][i] = 0.0; };
	    }
	}
	if (!plotFrameDisplayed) {

	   // set frame displayed
	   plotFrameDisplayed  = true;

	   // display  plot
	   plotFrame = new JHVVdataPlotFrame(this,tmpData);
	}
	else { // reset data 
	   
	   // set data
	   plotFrame.plotCanvas.setData(tmpData);
	}
	
	if (findDataRangeFlag) { 
	   resetDataRange();

          // for each selected colnum
	  for (int kk=0; kk<mColnum.size(); kk++) {

	    // selectedColnum ?
	    int selectedColnum = ((Integer)(mColnum.elementAt(kk))).intValue();
	    tmpStr[kk]         =  vdataFrame.vdataFieldName[selectedColnum];	
	  }

          // set annotation
	  plotFrame.plotCanvas.setAnnotation(tmpStr);

	} // if (findDataRangeFlag)
  }

 /** Check zoom factor
   */
  public boolean canMakeZoom( int zoomFactor ) {
  
	boolean retValue = true;

 	//  vdata xy-plot
	int  dataNumber = cellNumberByRow;
	
	// adjust data number by zoom factor
	if (zoomFactor > 0) {  // zoom-in
	   for (int kk=0; kk<zoomFactor; kk++)
		dataNumber /= 2;
	}
	else {
	   if (zoomFactor < 0)  // zoom-out
	      for (int kk=0; kk<(-zoomFactor); kk++)
		  dataNumber *= 2;
		
	}	   
 
	// adjust data number
	if  ((vOffset+dataNumber) > 2*vdataFrame.selectedVdataRecordsNumber) 
	    retValue = false;

	// make sure it can be zoomed in.
	if  ((dataNumber < 5) && (plotFrame.zoomState == JHVVdataPlotFrame.ZOOM_IN))
	    retValue = false;

	return retValue;
  }

    /** Get data range by specify field number */
    public double[] getDataRange(int selectedColnum) {

	double range[] = new double[2];
	
  	// selected colnum data range for whole dataset
	double min  = Double.MAX_VALUE;
	double max  = -min;
	if ( selectedColnum < 0) {
	   range[0] = range[1] = min;
	   return range;
	}
	for (int kk=0; kk<vdataFrame.selectedVdataRecordsNumber; kk++) {
	  double tmp = 0;
	  try {
	    String valStr = (String)vdata[kk][selectedColnum].trim();
	    tmp = Double.valueOf(valStr).doubleValue();
	  } catch (Exception e) { tmp  = 0; };
	  min = Math.min(tmp,min);
	  max = Math.max(tmp,max);	  
	}
	
	range[0] = min;
	range[1] = max;

	return range;
     }

    /** Return the selected colnum by clicked point (x,y)
     * @param x the x coordinate
     * @param y the y coordinate
     * @ return the selected colnum number , otherwise -1
     */
    public int getSelectedColnum(int x, int y) {

	int seleNum = -1;

	int col = 0;
    	int len = 0;
    	for (int i=hOffset; i<cellNumberByColnum+hOffset; i++, col++) {
		len += vdataCellRect[i].width;
		if (len>x)
	    	    break;
    	}

 	// confirm cell
    	if (col<= cellNumberByColnum)
	   // set which colnum has been selected, after that we'll make a xy-plot.
	   seleNum = col + hOffset;

	return seleNum;
    }

  void setFindDataRange(boolean flag) {

	this.findDataRangeFlag = flag;
  }

     public void resetDataRange() {

	  // current selected colnum
          Vector mColnum = selectedColnumVector;

	  // selected colnum data range for whole dataset
	  double min=Double.MAX_VALUE;
	  double max=-min;

	  int pos = 0;
	  if (plotFrame.plotCanvas.plotMode == plotFrame.plotCanvas.XYPLOT)
	     pos = 1;

          // for each selected colnum
          for (int kk=pos; kk<mColnum.size(); kk++) {

	    int selectedColnum = ((Integer)(mColnum.elementAt(kk))).intValue();

	    double tmpRange[] = null ;
	    tmpRange = getDataRange(selectedColnum);

	    min = Math.min(min,tmpRange[0]);
	    max = Math.max(max,tmpRange[1]);
         }

	 // reset y-axis range
	 plotFrame.plotCanvas.setYAxisRange(min,max);
     }

    /**
     *  Constructor with given JHVVdataFrame and HDFObject
     *
     *  @param frame       The JHVVdataFrame
     *  @param vdataObject The HDFVdata object containing information and data
     *  @auther            Peter Cao (xcao@ncsa.uiuc.edu)
     */
    public JHVVdataCanvas(JHVVdataFrame frame, ncsa.hdf.message.HDFVdata vdataObject)
    {
        // set spreadsheet frame 
        vdataFrame = frame;
    
        // do initalization
        setup(vdataObject);
        addMouseListener(this);
    }
    /**
     *  setup the JHVVdataCanvas
     *
     *  @param vdataObject The HDFVdata object containing information and data
     *  @auther            Peter Cao (xcao@ncsa.uiuc.edu)
     */
    public void setup(ncsa.hdf.message.HDFVdata vdataObject)
    {
        frameDisplayed = false;
    
        // set font
        setFont(dataFont);
        
        // cell size
        calculateCellSize();
        
        selectedVdataFieldName = vdataObject.getFieldNames();
        vdata =  vdataObject.getVdata();

        // set draw flag
        drawFlag       = false;
   }

    /**
     *  update the JHVVdataCanvas
     *
     *  @param vdataObject The HDFVdata object containing information and data
     *  @auther            Peter Cao (xcao@ncsa.uiuc.edu)
     */
    public void update(ncsa.hdf.message.HDFVdata vdataObject)
    {

        setup (vdataObject);

        // reset
        hOffset = 0;
        vOffset = 0;
        
        // set scrollbar value
        setVScrollbarValue();
        setHScrollbarValue();
        
        // re-calculate the cell position
        computeCellPosition();

        // reset scrollbar values
        vdataFrame.vScrollbar.setValue(0);
        vdataFrame.hScrollbar.setValue(0);

        // repaint
        repaint();

        // for xy-plot
        if (plotFrame != null)
           destroyXYPlot();

    }     
}
