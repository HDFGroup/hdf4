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
import ncsa.hdf.io.*;
import ncsa.hdf.awt.InfoDialog;

/** This class will create the spreadsheet to display Vdata value of an HDF file
 */
public class JHVVdataFrame
    extends Frame
    implements ActionListener, AdjustmentListener
{

  /** JHV */
  JHV   jhv= null;

  /** the canvas of the  displayed data */
  JHVVdataCanvas vdataCanvas;
  
  /** the canvas to display the record number and vdata field name */
  JHVVdataRangeCanvas	rowInfoCanvas,  colnumInfoCanvas;

  /** the scrollbar associated with the spreadsheet */
  Scrollbar hScrollbar, vScrollbar;
 
  /** selected vdata information */
  int[] selectedVdataRecords;
  int[] selectedVdataFields;
  int   selectedVdataFieldsNumber;
  int   selectedVdataRecordsNumber;
    
  /** common information of a Vdata */
  int numberOfVdata;
  int numberOfFields;
  String[] vdataFieldName;
  int[] vdataFieldDatatype;
  String vdataName;
  int[] vdataFieldOrder;

  TextField statusBar;

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
    add("South", statusBar=new TextField());
    statusBar.setBackground(Color.lightGray);
    statusBar.setEditable(false);

  }

  /** popup the new frame actually */
  public void popup() {

    // paint dataspread sheet
    vdataCanvas.repaint();

    //pack();
    setVisible(true);
    setSize(700, 500);
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
        setMenuBar(this.createMenuBar());

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

    /**
     * Handles Action Events
     */
    public void actionPerformed(ActionEvent e)
    {
        statusBar.setText("");

        Object target = e.getSource();
        String arg = e.getActionCommand();

        if (target instanceof MenuItem)
            handleMenuEvent(arg);
    }

    public void handleMenuEvent(String arg)
    {
        setCursor(new Cursor(Cursor.WAIT_CURSOR));

        if  ("Close".equals(arg))
        {
            dispose();
            jhv.vdataFrame = null;
        }
        else if ("Save As HDF".equals(arg))
        {
            try { save(true); }
            catch (Exception ex) { jhv.infoText.setText(ex.getMessage()); }
        }
        else if ("Save As Text".equals(arg))
        {
            try { save(false); }
            catch (Exception ex) { jhv.infoText.setText(ex.getMessage()); }
        }
        setCursor(new Cursor(Cursor.DEFAULT_CURSOR));

    }

    public MenuBar createMenuBar()
    {
        MenuBar  menuBar = new MenuBar();
        menuBar.add(createFileMenu("File"));
        return  menuBar;
    }

   /** create a file menu by provoded title
    * @param menuTitle the menu title
    */
    public Menu createFileMenu(String  menuTitle)
    {
        // new menu by specified menuTitle
        Menu fileMenu = new Menu(menuTitle);

	MenuItem save = new MenuItem("Save As HDF");
	save.addActionListener(this);
        fileMenu.add(save);

	save = new MenuItem("Save As Text");
	save.addActionListener(this);
        fileMenu.add(save);

        fileMenu.addSeparator();

	MenuItem close = new MenuItem("Close");
	close.addActionListener(this);
        fileMenu.add(close);

        return fileMenu;
    }

    /** save the data into HDF file or ASCII file */
    public void save(boolean isHDF)
    {
        FileDialog fd = new FileDialog(this, "Save HDF Data", FileDialog.SAVE);
        fd.setDirectory(jhv.cDir);
        fd.show();
        String theFile = fd.getFile();
        if (theFile == null) return;
        theFile = theFile.trim();
        String dir = fd.getDirectory();
        String fullname = dir + theFile;

        String warning = null;
        InfoDialog id = null;
        File outFile = new File(fullname);

        if (outFile.exists()) {
            if (!outFile.canWrite())
            {
                warning = fullname+"\n"+ "This file is read only.\n"+ "File save failed.";
                statusBar.setText(warning);
                id = new InfoDialog(jhv.jhvFrame, fd.getTitle(), warning, jhv.warningIcon, false);
                id.show();
                return;
            }
        }

        String workingFile = jhv.asciiFile;
        if (isHDF) workingFile = jhv.hdfFile;
        boolean isWorkingFile = false;
        if (System.getProperty("os.name").toLowerCase().startsWith("win"))
            isWorkingFile = fullname.equalsIgnoreCase(workingFile);
        else
            isWorkingFile = fullname.equals(workingFile);

        if (isWorkingFile)
        {
            warning = fullname+"\n"+
                "This file is currently in use.\n"+
                "Overwriting the file causes inconsistency.\n"+
                "The JHV may not work until the file is reloaded.\n" +
                "  \n"+
                "Replace working file ?";
            statusBar.setText(warning);

            id = new InfoDialog(jhv.jhvFrame, fd.getTitle(),
                warning, jhv.warningIcon, true);
            id.setlabels("  Yes  ", "   No   ");
            id.show();
            if (!id.clickedOkButton()) return;
        }
        else if (!jhv.isMac() && !jhv.isWin() && outFile.exists())
        {
            warning = fullname+"\n"+ "This file already exists.\n"+ "  \n"+ "Replace existing file ?";
            statusBar.setText(warning);

            id = new InfoDialog(jhv.jhvFrame, fd.getTitle(), warning, jhv.warningIcon, true);
            id.setlabels("  Yes  ", "   No   ");
            id.show();
            if (!id.clickedOkButton()) return;
        }

        // reset the current working directory in JHV
        jhv.cDir = dir;

        // set annotation
        workingFile = jhv.asciiFile;
        if (jhv.isHDF()) workingFile = jhv.hdfFile;
        String nl = JHV.NL;
        String annotation = "";
        annotation += "File Name   : "+theFile;
        annotation += nl+"Data Object : "+vdataName.trim();
        annotation += nl+"Source File : "+workingFile;
        annotation += nl+"Created on  : "+(new java.util.Date()).toString();
        annotation += nl+"Created from: Java HDF Viewer";

        if (isHDF)
        {
            // write HDF file
            HDFWriter writer = new HDFWriter(fullname);

            try
            {
                writer.writeAnnotation(annotation, HDFConstants.DFACC_CREATE);
                writer.writeVdata(HDFConstants.DFACC_WRITE, vdataName, -1,
                    vdataFieldDatatype, vdataFieldName,
                    vdataFieldOrder, (Object)Object2Byte(vdataCanvas.hdfData),
                    selectedVdataRecordsNumber, HDFConstants.FULL_INTERLACE);
            } catch (Exception ex) {
                warning = fullname+"\n"+ "The write failed.\n"+ "  \n"+ "Exception: "+ex.getMessage();
                statusBar.setText(warning);
                id = new InfoDialog(jhv.jhvFrame, fd.getTitle(), warning, jhv.warningIcon, false);
                id.setlabels("  OK  ","");
                id.show();
                if (id.clickedOkButton()) return;
            }

        }
        else
        {
            // write ASCII file
            ASCIIWriter writer = null;

            String vdata[][] = new String[selectedVdataRecordsNumber][selectedVdataFieldsNumber];
            for (int row=0; row<selectedVdataRecordsNumber; row++)
                for (int col=0; col<selectedVdataFieldsNumber; col++)
                    vdata[row][col]= object2String(row,col);

            try
            {
                writer = new ASCIIWriter(fullname);
                writer.writeAnnotation(annotation);
                writer.setDelimiter(jhv.delimiter);
                writer.writeVdata(vdataName, vdataFieldDatatype, vdataFieldName,
                    vdataFieldOrder, selectedVdataRecordsNumber, vdata);
                writer.close();
            } catch (Exception ex) {
                warning = fullname+"\n"+ "The write failed.\n"+ "  \n"+ "Exception: "+ex.getMessage();
                statusBar.setText(warning);
                id = new InfoDialog(jhv.jhvFrame, fd.getTitle(), warning, jhv.warningIcon, false);
                id.setlabels("  OK  ","");
                id.show();
                if (id.clickedOkButton()) return;
            }
        }
        statusBar.setText("Save data successful at "+fullname);

    }

    /**
     *  converts raw vdata into byte data
     *
     *  @param data  the raw vdata
     *  @return  the byte array of the vdata
     */
    private byte[] Object2Byte(Object[] data)
    {
        int order = 0;
        int dataType = -1;
        int recordSize = 0;
        byte[] theData = null;
        byte[] theCell = null;
        int[] typeSize = new int[selectedVdataFieldsNumber];

        for (int col=0; col<selectedVdataFieldsNumber; col++)
        {
            int bytes = 0;
            try { bytes = HDFLibrary.DFKNTsize(vdataFieldDatatype[col]); }
            catch (Exception ex) {}
            typeSize[col] = bytes;
            recordSize += vdataFieldOrder[col]*bytes;
        }
        theData = new byte[recordSize*selectedVdataRecordsNumber];

        int position = 0;
        int cellSize = 0;
        int rowPosition = 0;
        for (int row=0; row<selectedVdataRecordsNumber; row++)
        {
            for (int col=0; col<selectedVdataFieldsNumber; col++)
            {
                order = vdataFieldOrder[col];
                dataType = vdataFieldDatatype[col];
                cellSize = order*typeSize[col];
                theCell = new byte[cellSize];

                switch(dataType)
                {
                    case HDFConstants.DFNT_CHAR:
                    case HDFConstants.DFNT_UCHAR8:
                    case HDFConstants.DFNT_UINT8:
                    case HDFConstants.DFNT_INT8:
                        byte[] bdata = (byte[])data[col];
                        String tmpStr = new String(bdata,order*row,order);
                        theCell = tmpStr.getBytes();
                        //System.arraycopy(theCell, 0, bdata, row*order, order);
                        break;
                    case HDFConstants.DFNT_INT16:
                    case HDFConstants.DFNT_UINT16:
                        short[] sdata = (short[])data[col];
                        theCell = HDFNativeData.shortToByte(row*order, order, sdata);
                        break;
                    case HDFConstants.DFNT_INT32:
                    case HDFConstants.DFNT_UINT32:
                        int[] idata = (int[])data[col];
                        theCell = HDFNativeData.intToByte(row*order, order, idata);
                        break;
                    case HDFConstants.DFNT_INT64:
                    //case HDFConstants.DFNT_UINT64:
                        long[] ldata = (long[])data[col];
                        theCell = HDFNativeData.longToByte(row*order, order, ldata);
                        break;
                    case HDFConstants.DFNT_FLOAT:
                    //case HDFConstants.DFNT_FLOAT32:
                        float[] fdata = (float[])data[col];
                        theCell = HDFNativeData.floatToByte(row*order, order, fdata);
                        break;
                    case HDFConstants.DFNT_DOUBLE:
                    //case HDFConstants.DFNT_FLOAT64:
                        double[] ddata = (double[])data[col];
                        theCell = HDFNativeData.doubleToByte(row*order, order, ddata);
                        break;
                    default:
                        theCell = new byte[cellSize];

                } //switch(dataType)
        
                System.arraycopy(theCell, 0, theData, position, cellSize);
                position +=cellSize;

            } //for (int col=0; col<selectedVdataFieldsNumber; col++)
        } // for (int row=0; row<selectedVdataRecordsNumber; row++)

        return theData;
    }

    /**
     *   converts the data object of a vadata cell into a string
     *
     *  @param row  the row index of the vdata table
     *  @param col  the column index of the vdata table
     *  @return the string of the data
     */
    private String object2String(int row, int col)
    {
        String theString = "";
        int order = vdataFieldOrder[col];
        int dataType = vdataFieldDatatype[col];

        switch(dataType)
        {
            // string
            case HDFConstants.DFNT_CHAR:
            case HDFConstants.DFNT_UCHAR8:
                byte[] bdata = (byte[])vdataCanvas.hdfData[col];
                theString = new String(bdata,order*row,order);
                break;

            case HDFConstants.DFNT_UINT8:
            case HDFConstants.DFNT_INT8:
                byte[] sbdata = (byte[])vdataCanvas.hdfData[col];
                theString="";
                for (int j=0; j<order; j++)
                {
                    int tmpVal = (int)((byte)sbdata[j+row*order]);
                    if ((dataType == HDFConstants.DFNT_UINT8) && (tmpVal <0))
                        tmpVal += 256;
                    theString += Integer.toString(tmpVal) +" ";
                }
                break;

            case HDFConstants.DFNT_INT16:
            case HDFConstants.DFNT_UINT16:
                short[] sdata = (short[])vdataCanvas.hdfData[col];
                theString="";
                for (int j=0; j<order; j++)
                {
                    int tmpVal = (int)sdata[j+order*row];
                    if ((dataType == HDFConstants.DFNT_UINT16) && (tmpVal <0))
                        tmpVal += 65536;
                    theString += Integer.toString(tmpVal) +" ";
                }
                break;

            case HDFConstants.DFNT_INT32:
            case HDFConstants.DFNT_UINT32:
                int[] idata = (int[])vdataCanvas.hdfData[col];
                theString="";
                for (int j=0; j<order; j++)
                {
                    int tmpVal = idata[j+order*row];
                    if ((dataType == HDFConstants.DFNT_UINT32) && (tmpVal <0))
                        tmpVal += 4294967296L;
                    theString += Integer.toString(tmpVal) +" ";
                }
                break;

            case HDFConstants.DFNT_INT64:
            //case HDFConstants.DFNT_UINT64:
                long[] ldata = (long[])vdataCanvas.hdfData[col];
                theString="";
                for (int j=0; j<order; j++)
                {
                    long tmpVal = ldata[j+order*row];
                    theString += Long.toString(tmpVal) +" ";
                }
                break;

            case HDFConstants.DFNT_FLOAT:
            //case HDFConstants.DFNT_FLOAT32:
                float[] fdata = (float[])vdataCanvas.hdfData[col];
                theString="";
                for (int j=0; j<order; j++)
                {
                    float tmpVal = fdata[j+order*row];
                    theString += Float.toString(tmpVal) +" ";
                }
                break;

            case HDFConstants.DFNT_DOUBLE:
            //case HDFConstants.DFNT_FLOAT64:
                double[] ddata = (double[])vdataCanvas.hdfData[col];
                theString="";
                for (int j=0; j<order; j++)
                {
                    double tmpVal =ddata[j+order*row];
                    theString += Double.toString(tmpVal) +" ";
                }
                break;

            default:
                theString = "";
        }

        return theString;
    }

}
