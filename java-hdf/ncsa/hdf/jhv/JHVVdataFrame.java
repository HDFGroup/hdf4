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




//-----------------------------------------------------------------------------
//
//  Upgraded to the JDK 1.1.1b Event Model.
//    - Apu Kapadia, May 21st, 1997.
//
//-----------------------------------------------------------------------------

/** This class will create the spreadsheet to display Vdata value of an HDF file 
 */
public class JHVVdataFrame extends Frame implements AdjustmentListener  {

  /** JHV */
  JHV   jhv= null;

  /** an HDF vdata object */
  HDFLibrary   hdf;
  
  /** vdata identifier */
  int  vdata_id;
    
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

  /** Constructor of the class.
   * @param jhv      an JHV object
   * @param hdf anHDFVdata object
   * @param vdata_id an HDF vdata identiffier
   * @param records  an HDF vdata records range
   * @param fldnumber a selected HDF vdata fields
   */ 

  public JHVVdataFrame(JHV jhv, HDFLibrary hdf, int vdata_id, int[] records, int[] fldnumber) {

    this.jhv  = jhv;
    
    // set HDF object
    this.hdf = hdf;
   
    // set vdata identifier
    this.vdata_id = vdata_id;
    
    // display vdata records
    selectedVdataRecords = records;
    
    // selected vdata fields
    selectedVdataFields = fldnumber;
    
    selectedVdataFieldsNumber = fldnumber.length;
    selectedVdataRecordsNumber= records[1] - records[0] + 1;
 
    // retrieve vdata information
    try {
    getVdataInfo(vdata_id);
    } catch (HDFException e) {}
    
    // set frame title
    setTitle("HDF Vdata: " + vdataName);
	
    // create new JHVVdataCanvas(actual spreadsheet panel)
    vdataCanvas = new JHVVdataCanvas(this);

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

  /** setup the frame tittle
   * @param title a frame title
   */
  public void setTitle(String title) {
   
    // set title	
    super.setTitle(title);
  }

  /** Initialize vdata */
  public void setup( int vdata_id, int[] records, int[] fldnumber) {

    // set vdata identifier
    this.vdata_id = vdata_id;
    
    // display vdata records
    selectedVdataRecords = records;
    
    // selected vdata fields
    selectedVdataFields = fldnumber;
    selectedVdataFieldsNumber = fldnumber.length;
    selectedVdataRecordsNumber= records[1] - records[0] + 1;
   
    // retrieve vdata information
    try {
    getVdataInfo(vdata_id);
    } catch (HDFException e) {}
   
    setTitle("HDF Vdata: " + vdataName);
    
    // canvas setup
    vdataCanvas.update();

  }

 

 /** create Spreadsheet */
  public void createSpreadsheetGUI() {

    // set Layout Manager
    setLayout(new BorderLayout());

    // spreadsheet panel
    Panel	sPanel = new Panel();
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

 /** Retrieve HDF vdata common information */
 public boolean getVdataInfo(int vdata_id) throws HDFException {

    // vdata name
    vdataName = new String(" ");
	String[] s = new String[1];
	s[0] = vdataName;
	hdf.VSgetname(vdata_id,s/*vdataName*/);
	vdataName = s[0];

    // vdata record number
    numberOfVdata = hdf.VSelts(vdata_id);
    if (numberOfVdata == HDFConstants.FAIL)
       return false;
    
    // vdata fields number
    numberOfFields = hdf.VFnfields(vdata_id);
    if (numberOfFields == HDFConstants.FAIL)
       return false;
    
    // specified vdata fields list
    vdataFieldName       = new String[numberOfFields];
    vdataFieldOrder      = new int[numberOfFields];
    vdataFieldDatatype   = new int[numberOfFields];
    
    for (int i=0; i<numberOfFields; i++) {
	
	// get field name
	vdataFieldName[i] = new String(hdf.VFfieldname(vdata_id,i));

	// data type of a specified field
	vdataFieldDatatype[i] = hdf.VFfieldtype(vdata_id,i);
	
	// order of a specified field 
	vdataFieldOrder[i] = hdf.VFfieldorder(vdata_id,i);
	
	if ((vdataFieldDatatype[i] == HDFConstants.FAIL) ||
	    (vdataFieldOrder[i] == HDFConstants.FAIL)) 
	   return false;
    }
    
    return true;
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
}
