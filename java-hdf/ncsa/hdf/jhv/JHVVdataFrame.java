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

/** This class will create the spreadsheet to display Vdata value of an HDF file 
 */
public class JHVVdataFrame extends Frame implements AdjustmentListener  {

  /** JHV */
  JHV   jhv= null;

  /** the canvas of the  displayed data */
  JHVVdataCanvas	  vdataCanvas;
  
  /** the canvas to display the record number and vdata field name */
  JHVVdataRangeCanvas	rowInfoCanvas,  colnumInfoCanvas;

  /** the scrollbar associated with the spreadsheet */
  Scrollbar	hScrollbar, vScrollbar;
 
  /** selected vdata information */
  int[] selectedVdataRecords;
  int[] selectedVdataFields;
  int   selectedVdataFieldsNumber;
  int   selectedVdataRecordsNumber;
    
  /** common information of a Vdata */
  int      numberOfVdata;
  int      numberOfFields;
  String[] vdataFieldName;
  int[]    vdataFieldDatatype;
  String   vdataName;
  int[]    vdataFieldOrder;

    /**
     *  setup vdata
     *  @param jhv         The JHV viewer
     *  @param vdataObject The HDFVdata object containing information and data
     *  @auther            Peter Cao (xcao@ncsa.uiuc.edu)
     */
    public void setup(JHV jhv, ncsa.hdf.message.HDFVdata vdataObject)
    {

        this.jhv  = jhv;
   
        // display vdata records
        selectedVdataRecords = vdataObject.getRecords();
    
        // selected vdata fields
        selectedVdataFields = vdataObject.getFields();
    
        selectedVdataFieldsNumber = selectedVdataFields.length;
        selectedVdataRecordsNumber= selectedVdataRecords[1] - selectedVdataRecords[0] + 1;
   
        // vdata name
        vdataName = vdataObject.getVdataName();

        // vdata record number
        numberOfVdata = selectedVdataRecordsNumber;
   
        // vdata fields number
        numberOfFields = selectedVdataFieldsNumber;
    
        // specified vdata fields list
        vdataFieldName       = vdataObject.getFieldNames();
        vdataFieldOrder      = vdataObject.getFieldOrders();
        vdataFieldDatatype   = vdataObject.getFieldTypes();
   
        setTitle("HDF Vdata: " + vdataName);
    
        // canvas setup
        if (vdataCanvas != null)
            vdataCanvas.update(vdataObject);
    }


  /** setup the frame tittle
   * @param title a frame title
   */
  public void setTitle(String title) {
   
    // set title	
    super.setTitle(title);
  }

 /** create Spreadsheet */
  public void createSpreadsheetGUI() {

    // set Layout Manager
    setLayout(new BorderLayout());

    // spreadsheet panel
    Panel sPanel = new Panel();
    sPanel.setLayout(new BorderLayout());	
	
    // Horizontal & vertical Scrollbar 
    hScrollbar = new Scrollbar(Scrollbar.HORIZONTAL);
    hScrollbar.addAdjustmentListener(this);
    vScrollbar = new Scrollbar();
    vScrollbar.addAdjustmentListener(this);

    sPanel.add("North", colnumInfoCanvas);	
    sPanel.add("West",  rowInfoCanvas);
    sPanel.add("Center",vdataCanvas);
    sPanel.add("East",  vScrollbar);
    sPanel.add("South", hScrollbar);

    // set Spreadsheet
    add("Center", sPanel);
  
  }

  /** popup the new frame actually */
  public void popup() {

    // default frame size
    setSize(700, 500);

    // paint dataspread sheet
    vdataCanvas.repaint();
    
    // show component of the frame
    show();
    
    // compute the cell position
    vdataCanvas.computeCellPosition();

  }

  /**
   * Handles the event. Returns true if the event is handled and
   * should not be passed to the parent of this component. The default
   * event handler calls some helper methods to make life easier
   * on the programmer.
   * @param evt the event
   * @see java.awt.Component#handleEvent
   */
 
  public void adjustmentValueChanged(AdjustmentEvent e)
  {
    int type = e.getAdjustmentType();
    Scrollbar target = (Scrollbar)e.getAdjustable();

    switch (type) {
	
    case AdjustmentEvent.UNIT_INCREMENT :
    case AdjustmentEvent.UNIT_DECREMENT :
    case AdjustmentEvent.BLOCK_INCREMENT:
    case AdjustmentEvent.BLOCK_DECREMENT:
    case AdjustmentEvent.TRACK :
      // detect the vertical scrollbar
      if (target ==  vScrollbar) {

	// set draw flag
	vdataCanvas.drawFlag = false;
	
	// get offset
	vdataCanvas.ty = vScrollbar.getValue();
	
	// set scrollbar offset to display the correct data
	vdataCanvas.setVoffset();		
	
	// repaint the graphics
	vdataCanvas.repaint();
      }
	    
      // detect the horizontal scrollbar
      if (target ==  hScrollbar) {
	    
	// set draw flag
	vdataCanvas.drawFlag = false;
   
	// get offset
	vdataCanvas.tx = hScrollbar.getValue();
	
	// set horizontal scrollbar offset
	vdataCanvas.setHoffset();
	    
	// repaint the graphics
	vdataCanvas.repaint();
      }
    } // switch(evt.id)

  }

    /**
     *  Constructor with given JHV viewer and HDFObject
     *
     *  @param jhv         The JHV viewer
     *  @param vdataObject The HDFVdata object containing information and data
     *  @auther            Peter Cao (xcao@ncsa.uiuc.edu)
     */
    public JHVVdataFrame(JHV jhv, ncsa.hdf.message.HDFVdata vdataObject)
    {
        vdataCanvas = null;
        setup(jhv, vdataObject);

        // create new JHVVdataCanvas(actual spreadsheet panel)
        vdataCanvas = new JHVVdataCanvas(this, vdataObject);

        vdataCanvas.setCanvasSize(2*256,256);
        
        // create the canvas to display the title
        rowInfoCanvas = new JHVVdataRangeCanvas(this, JHVVdataRangeCanvas.VERTICAL);

        // another canvas to hold the dimension scale  information(colnum)
        colnumInfoCanvas=new JHVVdataRangeCanvas(this,JHVVdataRangeCanvas.HORIZONTAL);

        // create spreadsheet and control panel(Graphical User Interface)
        createSpreadsheetGUI();

        // check the spreadsheet size
        vdataCanvas.checkSize();
        this.addWindowListener(new JHVWindowAdapter(jhv));
    }


}
