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

import java.awt.*;
import java.awt.event.*;
import java.awt.image.*;
import java.applet.*;
import java.util.*;
import java.net.*;



//-------------------------------------------------------------------------------------
//
//  Upgraded to the JDK 1.1.1b Event Model.
//    - Apu Kapadia, May 21st 1997.
//
//-------------------------------------------------------------------------------------


/** This class will create a new canvas to display an HDF field name & records */
public class JHVVdataRangeCanvas extends Canvas implements MouseListener {
 
 /** the spreadsheet frame */
  JHVVdataFrame	vdataFrame;
  
  /** the indicator of the canvas attributes */
  int             orientation;
   
  /** The horizontal Canvas variable. */
  public static final int	HORIZONTAL = 0;
   
  /** The vertical Canvas variable.  */    
  public static final int	VERTICAL   = 1;

  // default value for data spreadsheet row or colnum number
  public int rectLen 	= 30;	
  public int xLen	= 256*2;
  public int yLen	= 256*1;

  Font		font;
  FontMetrics	fontMetrics;
  
  int		fontWidth, fontHeight;
    
  // double buffering
  // variables for double-buffer
  Image 	offScreenImage = null;
  Graphics 	offGraphics;

 /** new class constructor 
   * @param frame the spreadsheet frame
   * @param direction the variable to specify the canvas associated with spreadsheet
   */
  public JHVVdataRangeCanvas(JHVVdataFrame frame, int direction) {
    
    // set spreadsheet frame
    vdataFrame    = frame;

    // set canvas type
    orientation = direction;

    // set the current used font
    font 	    = frame.vdataCanvas.dataFont;

    // Sets the font of the component.
    setFont(font);

    init();

    addMouseListener(this);    

  }

  /** Initialize the canvas 
   * @param len the canvas width or length
   */
  public void init(int len) {
	
    // set the canvas size
    if (orientation == HORIZONTAL) {
      // Assume scrollbar width to 10;
      xLen = len + rectLen + 20 ;
      setSize(xLen, rectLen);
    }
    else {
      yLen = len;
      setSize(rectLen, yLen);
    }
    repaint();
  }

 /** Initialize the canvas by default */
  public void init() {
	
    // set the canvas size
    if (orientation == HORIZONTAL) 
      setSize(xLen, rectLen);	
    else 
      setSize(rectLen, yLen);  
  }

  /** 
   * Sets the font of the component.
   * @param f the font
   */
  public synchronized void setFont(Font f) {
    // set Font for component
    super.setFont(f);

    // get new FontMetrics
    fontMetrics = getFontMetrics(getFont());

    // set font width & height
    fontHeight = fontMetrics.getHeight();
    fontWidth  = fontMetrics.charWidth('A');
    
  }

 /** draw anHDF title responding to the spreadsheet */
  public synchronized void paint(Graphics g) {

    // cell size of the spreadsheet
    Rectangle rect = vdataFrame.vdataCanvas.CellRect;
  
    // the row and colnum of the current spreadsheet
    int rowNumber  = vdataFrame.vdataCanvas.cellNumberByRow;
    int colNumber  = vdataFrame.vdataCanvas.cellNumberByColnum;

    if (orientation == HORIZONTAL)  {
      g.setColor(Color.pink);
      
      g.fill3DRect(rectLen,1, xLen , rectLen, false);
      g.setColor(Color.white);
  
      // drawable position
      int x = rectLen;
      int y = 1;
    
      int pos = 0;

      int hVal = vdataFrame.vdataCanvas.hOffset	;

      for (int i=0; i<=colNumber; i++)  
	if ((hVal + i) <
	    vdataFrame.selectedVdataFieldsNumber)  {
	    
	  //x = rectLen + rect.width*i + 4;
	  int offset = 0;
	  if (i>0)
	     pos +=   vdataFrame.vdataCanvas.vdataCellRect[hVal+i-1].width;
	    
	  // width for one character
	  int w = vdataFrame.vdataCanvas.fontWidth + 1;
	  if (i<colNumber)
	    // width of cell
	    offset = vdataFrame.vdataCanvas.vdataCellRect[hVal+i].width/w;
	    	    
	  x = rectLen + 4 + pos;
	    
	  // find index of selected field  
	  int idx  = vdataFrame.selectedVdataFields[hVal + i];    
	  String kk = vdataFrame.vdataFieldName[idx];
	  
          // offset to display title within the center
	  if (offset > kk.length())
	     offset = (offset - kk.length()) * w / 2;
	  else
	     offset = 6;
	    
	  // adjust display postion
	  x += offset;
    
	  // display title in the cnter of cell  
	  g.drawString(kk,x, rectLen-10 );
	}
      
    } 
    else {
      // set color 
      g.setColor(Color.magenta);

      // draw the graphics
      g.fill3DRect(1,1, rectLen, yLen, true);
      g.setColor(Color.white);
      
      int y = 1;  
      // first value
      int kk = vdataFrame.vdataCanvas.vOffset + 
	       vdataFrame.selectedVdataRecords[0];
    
      // selected vdata records number
      int selectedVdataNumber = vdataFrame.selectedVdataRecordsNumber;
	   
      for (int i=1; i<=rowNumber; i++)  
	if ((vdataFrame.vdataCanvas.vOffset + i) <=
	    selectedVdataNumber)  {
	  y = rect.height*i;
	  g.drawString(Integer.toString(kk+i-1), 1, y);
	}
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

  public void mousePressed(MouseEvent me) {}
  public void mouseEntered(MouseEvent me) {}
  public void mouseExited(MouseEvent me) {}
  public void mouseClicked(MouseEvent me) {}

  public void mouseReleased(MouseEvent me)
  {
    int x = me.getX();
    int y = me.getY();
    vdataFrame.vdataCanvas.drawByColnum(x-rectLen,y);
  }
}
