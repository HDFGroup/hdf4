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

import java.applet.Applet;
import java.net.URL;
import java.util.Hashtable;
import java.awt.image.*;
import java.lang.Thread;
import java.awt.*;
import java.awt.event.*;
import java.io.*;
import java.lang.reflect.Array;

import ncsa.hdf.hdflib.*;
import ncsa.hdf.awt.image.*;
import ncsa.hdf.awt.animation.ImageAnimation;
import ncsa.hdf.awt.*;
import ncsa.hdf.awt.event.*;
import ncsa.hdf.message.*;
import ncsa.hdf.io.*;
import ncsa.hdf.util.*;


//  Upgraded to the JDK 1.1.1b Event Model by Apu
/**
 * This class displays an image in seprate window. More image processing is here.
 * @version 1.50  9/6/97
 * @auther Xinjian  Lu
 */
public class JHVImageFrame
    extends Frame implements
    ItemListener, ActionListener, AdjustmentListener
{

  /** the canvas that the image will  be painted. */
  JHVImageCanvas imageCanvas = null;

  /** zoom panel responded to JHVImageCanvas */
  ScrollingPanel zoomImagePanel = null;

  /** the canvas that the image will be drawed 
    responding to the HDF node selected */
  JHVCanvas jhvCanvas = null;

  /** the HDF tree browser applet */
  JHV applet_ = null;

  /** default frame size */
  int frameWidth = 710;
  int frameHeight = 420;

  /** the constants of the animation operation  */
  public  final static int NEXT     = 1;
  public  final static int PREVIOUS = 2;

  /** the current plane number of the sequence images  */
  int numPlane = 1;

  /** the current plane number of the sequence images  */
  /**  the total image number */
  int plane = 1;
    
  /** Scrollbar associated with the image panel */
  Scrollbar   hScrollbar = null;
  Scrollbar   vScrollbar = null;
    
  CheckboxGroup  zoomFactorGroup ;
  Checkbox       zoomFactor100   ;
  Checkbox       zoomFactor200   ;
  Checkbox       zoomFactor500   ;
  Checkbox       zoomFactor800   ;

  // spreedsheet button
  Button spButton;

  // animation
  Button anButton;

  // image number
  TextField imgField;
  Label imgLabel;

  // status bar
  TextField statusBar;

  // animated operation button
  Button nextButton;
  Button prevButton;

  JHVSDSImageControl sdsImageControl;
  JHVImageRGBControl imageRGBControl;
  JHVImageBCControl imageBCControl;

  MenuItem adjustMenuItem;

  public float   zoomFactor = 2.0f;

  // display pixel value or digital value?
  public static final int NONE_VALUE  = 0;
  public static final int PIXEL_VALUE = 1;
  public static final int DIGIT_VALUE = 2;

  public static final int NONE_PLOT = 3;
  public static final int RADIAL_PLOT = 4;
  public static final int HISTOGRAM_PLOT = 5;

  // view menu items  
  CheckboxMenuItem 	digitItem;
  CheckboxMenuItem	pixelItem;
  CheckboxMenuItem	noneItem;
  CheckboxMenuItem	radialItem;
  CheckboxMenuItem	histogramItem;
  CheckboxMenuItem	noPlotItem;

  // animation window opeded?
  boolean		animationWindowOpened = false;
  JHVImageAnimation	jhvImageAnimation;

  // Inner class for Window closing
  class JHVImageFrameClosedProcess extends WindowAdapter {
     public void windowClosing(WindowEvent evt) {
	// close all of my control
	close();
     }
  }

  /** Create the seprate frame based on the HDF browser's image canvas.
   *  This frame will display the HDF raster image with scroll panel
   * @param app the JHV applet name
   */
  public JHVImageFrame(JHVCanvas canvas_, SDSDimInfo slice, Rectangle subset) {

    // JHV applet
    applet_ = canvas_.app;

    // set HDF Browser image canvas
    jhvCanvas = canvas_;

    // set the frame tittle
    setTitle();

    // set menu bar
    setMenuBar(this.createJHVImageMenuBar());

    // image canvas
    imageCanvas = new JHVImageCanvas(this, subset);

    // set slice info
    setSliceInfo(slice);

    // set jhv canvas associated with image canvas
    imageCanvas.jhvCanvas = canvas_;

    imageCanvas.setImageSize(subset.width, subset.height);

    // set canvas size
    // imageCanvas.setCanvasSize(w,h);
    imageCanvas.setCanvasSize(200,200);

    // get HDF Object Node;
    imageCanvas.setHDFObjectNode( canvas_.node );

    // initialize the HDF based on the selected HDF node
    imageCanvas.initHDF();

    // set layout manager
    setLayout(new BorderLayout());

    // GUI
    add("Center", createDisplayItems());

    // set scrollbar
    imageCanvas.setScrollbar(hScrollbar, vScrollbar);

    //imagePanel.imageCanvas.checkSize();
    imageCanvas.checkSize();

    // speedsChoice.addItemListener(this);
    this.addWindowListener(new JHVImageFrameClosedProcess());

    plane = imageCanvas.numberOfImage;
    imgLabel.setText("1 of "+plane);

  }

  /** setup the frame tittle */
  public void setTitle() {
    String winString = " Image from HDF";
    super.setTitle(winString);
  }

    /** create menubar for JHV image viewer */
    public MenuBar createJHVImageMenuBar() {

        // new menubar
        MenuBar  jhvImageMenuBar = new MenuBar();
        jhvImageMenuBar.add(createFileMenu("File"));
  	jhvImageMenuBar.add(createEditMenu("Edit"));
	jhvImageMenuBar.add(createViewMenu("View"));
	// jhvImageMenuBar.add(createPlotMenu("Plot"));
    	jhvImageMenuBar.add(createPaletteMenu("Palette"));
	jhvImageMenuBar.add(createImageMenu("Image"));
	jhvImageMenuBar.add(createFilterMenu("Filter"));
	jhvImageMenuBar.add(createContourMenu("Contour"));
	jhvImageMenuBar.add(createDataMenu("Data"));

        return  jhvImageMenuBar;
    }

   /** create a file menu by provoded title
    * @param menuTitle the menu title
    */
    public Menu createFileMenu(String  menuTitle) {

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

  /** create a view menu by provoded title
    * @param menuTitle the menu title
    */
    public Menu createViewMenu(String  menuTitle) {

        // new menu by specified menuTitle
        Menu viewMenu = new Menu(menuTitle);

	noneItem = new CheckboxMenuItem("Default",true);
	noneItem.addItemListener(this);
        viewMenu.add(noneItem);

	// add menuItem
	digitItem = new CheckboxMenuItem("Digital Value");
	digitItem.addItemListener(this);
	viewMenu.add(digitItem); //default

	pixelItem = new CheckboxMenuItem("Pixel Value");
	pixelItem.addItemListener(this);
        viewMenu.add(pixelItem);

	viewMenu.addSeparator();

   	// histogram or arial plots
	noPlotItem = new CheckboxMenuItem("No Plot",true);
	noPlotItem.addItemListener(this);
        viewMenu.add(noPlotItem);

	// histogram or arial plots
	radialItem = new CheckboxMenuItem("X-Y Plot");
	radialItem.addItemListener(this);
        viewMenu.add(radialItem);

	histogramItem = new CheckboxMenuItem("Histogram");
	histogramItem.addItemListener(this);
        viewMenu.add(histogramItem);

        return viewMenu;
    }

  /** create a plot menu by provoded title
    * @param menuTitle the menu title
    */
    public Menu createPlotMenu(String  menuTitle) {

        // new menu by specified menuTitle
        Menu plotMenu = new Menu(menuTitle);

    	// histogram or arial plots
	noPlotItem = new CheckboxMenuItem("No Plot",true);
	noPlotItem.addItemListener(this);
        plotMenu.add(noPlotItem);

	// histogram or arial plots
	radialItem = new CheckboxMenuItem("X-Y Plot");
	radialItem.addItemListener(this); 
        plotMenu.add(radialItem);

	histogramItem = new CheckboxMenuItem("Histogram");
	histogramItem.addItemListener(this); 
        plotMenu.add(histogramItem);

        return plotMenu;    
    }

   /** create a edit  menu by provoded title such like bright/contrast ...
    * @param menuTitle the menu title
    */
    public Menu createEditMenu(String  menuTitle) {
    
        // new menu by specified menuTitle
        Menu editMenu = new Menu(menuTitle);
    
        // add menuItem 
	
	MenuItem UndoAllMItem = new MenuItem("Undo All");
	UndoAllMItem.addActionListener(this);
 	editMenu.add(UndoAllMItem);
	editMenu.addSeparator();
	MenuItem UndoMItem = new MenuItem("Undo");
	UndoMItem.addActionListener(this);
	editMenu.add(UndoMItem);	
        return editMenu;
    
    }

   /** create a palette menu by provoded title
    * @param menuTitle the menu title
    */
    public Menu createPaletteMenu(String  menuTitle) {

        // new menu by specified menuTitle
        Menu paletteMenu = new Menu(menuTitle);

        // add menuItem
	MenuItem NewPaletteMItem = new MenuItem("New Palette");
	NewPaletteMItem.addActionListener(this);
	paletteMenu.add(NewPaletteMItem);
	paletteMenu.addSeparator();
	MenuItem SetPaletteMItem = new MenuItem("Set Palette");
	SetPaletteMItem.addActionListener(this);
	paletteMenu.add(SetPaletteMItem);

        return paletteMenu;

    }

   /** create a image process menu by provoded title such like bright/contrast ...
    * @param menuTitle the menu title
    */
    public Menu createImageMenu(String  menuTitle) {
    
        // new menu by specified menuTitle
        Menu imageMenu = new Menu(menuTitle);
      
	// add menuItem 
        adjustMenuItem = new MenuItem("Range Modification");
 	adjustMenuItem.addActionListener(this);
	imageMenu.add(adjustMenuItem);

	imageMenu.addSeparator();

	MenuItem RGBBalanceMItem = new MenuItem("RGB Balance");
	RGBBalanceMItem.addActionListener(this);
	imageMenu.add(RGBBalanceMItem);
	
	MenuItem BCMItem = new MenuItem("Brightness/Contrast" );
	BCMItem.addActionListener(this);
	imageMenu.add(BCMItem);
	
	MenuItem NegativeMItem = new MenuItem("Negative");
	NegativeMItem.addActionListener(this);
	imageMenu.add( NegativeMItem );
 
        return imageMenu;
    
    }

   /** create a image filter menu by provoded title 
    * @param menuTitle the menu title
    */
    public Menu createFilterMenu(String  menuTitle) {
    
        // new menu by specified menuTitle
        Menu filterMenu = new Menu(menuTitle);
    
        // add menuItem 
	
	MenuItem smooth = new MenuItem("Smooth");
	smooth.addActionListener(this);
 	filterMenu.add(smooth);

	MenuItem sharpen = new MenuItem("Sharpen");
	sharpen.addActionListener(this);
	filterMenu.add(sharpen);

	MenuItem addNoise = new MenuItem("Add Noise");
	addNoise.addActionListener(this);
	filterMenu.add(addNoise);

	MenuItem findEdge = new MenuItem("Find Edge");
	findEdge.addActionListener(this);
	filterMenu.add(findEdge);

	MenuItem flipH = new MenuItem("Flip(Horizontal)" );
	flipH.addActionListener(this);
	filterMenu.add(flipH);

	MenuItem flipV = new MenuItem("Flip(Vertical)" );
	flipV.addActionListener(this);
	filterMenu.add(flipV);

	MenuItem emboss = new MenuItem("Emboss");
	emboss.addActionListener(this);
	filterMenu.add(emboss);

	MenuItem polarize = new MenuItem("Polarize");
	polarize.addActionListener(this);
	filterMenu.add(polarize);

	MenuItem rotate = new MenuItem("Rotate");
	rotate.addActionListener(this);
	filterMenu.add(rotate);
		
        return filterMenu;
    
    }

   /** create a image contour menu by provoded title
    * @param menuTitle the menu title
    */
    public Menu createContourMenu(String  menuTitle) {

        // new menu by specified menuTitle
        Menu contourMenu = new Menu(menuTitle);

        // add menuItem
	MenuItem level3 = new MenuItem("Level 3");
	level3.addActionListener(this);
 	contourMenu.add(level3);

	MenuItem level5 = new MenuItem("Level 5");
	level5.addActionListener(this);
	contourMenu.add(level5);

	MenuItem level9  = new MenuItem("Level 9");
	level9.addActionListener(this);
	contourMenu.add(level9);

        return contourMenu;

    }
   /** create a data menu
    * @param menuTitle the menu title
    */
    public Menu createDataMenu(String  menuTitle) {

        // new menu by specified menuTitle
        Menu menu = new Menu(menuTitle);

        // add menuItem for showing data statistic
	MenuItem item = new MenuItem("Show Statistics");
	item.addActionListener(this);
 	menu.add(item);

	menu.addSeparator();

        // add menuItem for converting data
	item = new MenuItem("Convert Data");
	item.addActionListener(this);
 	menu.add(item);

        return menu;
    }

/** create new GUI for ImageFrame */
public Panel createDisplayItems() {
 
    // return panel
    Panel retPanel = new Panel();
     
    // image panel
    Panel imgPanel = new Panel();
    imgPanel.setLayout(new BorderLayout());
    
    // create the scrollbar
    vScrollbar = new Scrollbar();
    vScrollbar.addAdjustmentListener(this);
    hScrollbar = new Scrollbar(Scrollbar.HORIZONTAL);
    hScrollbar.addAdjustmentListener(this);

    imgPanel.add("Center", imageCanvas );
    imgPanel.add("East",   vScrollbar);
    imgPanel.add("South",  hScrollbar);
     
    // control area
    Panel ctrPanel = new Panel();
    ctrPanel.setLayout(new GridLayout(1,0));

    // place buttons
    ctrPanel.add(spButton = new Button("Spreadsheet"));
    ctrPanel.add(anButton = new Button("Animation"));
    ctrPanel.add(nextButton =new Button("Next"));
    ctrPanel.add(prevButton =new Button("Previous"));
    spButton.addActionListener(this);
    anButton.addActionListener(this);
    nextButton.addActionListener(this);
    prevButton.addActionListener(this);

    // place image number field
    ctrPanel.add(imgField = new TextField("1"));
    ctrPanel.add(imgLabel = new Label("1 of "+plane));
    imgField.addActionListener(this);

    // place zoom factor checkboxes
    Panel zCtrPanel0 = new Panel();
    zCtrPanel0.setLayout(new GridLayout(1,0));
    Panel zCtrPanel1 = new Panel();
    zCtrPanel1.setLayout(new GridLayout(1,0));
    CheckboxGroup  zoomFactorGroup = new CheckboxGroup();
    Checkbox       zoomFactor100   = new Checkbox("1x", zoomFactorGroup, false);
    Checkbox       zoomFactor200   = new Checkbox("2x", zoomFactorGroup, true);
    Checkbox       zoomFactor500   = new Checkbox("5x", zoomFactorGroup, false);
    Checkbox       zoomFactor1000  = new Checkbox("10x", zoomFactorGroup, false);
    zoomFactor100.addItemListener(this);
    zoomFactor200.addItemListener(this);
    zoomFactor500.addItemListener(this);
    zoomFactor1000.addItemListener(this);
    zCtrPanel0.add(zoomFactor100);
    zCtrPanel0.add(zoomFactor200);
    zCtrPanel1.add(zoomFactor500);
    zCtrPanel1.add(zoomFactor1000);
    ctrPanel.add(zCtrPanel0);
    ctrPanel.add(zCtrPanel1);

     // place status bar
    Panel bottomPanel = new Panel();
    bottomPanel.setLayout(new GridLayout(0,1));
    bottomPanel.add(ctrPanel);
    bottomPanel.add(statusBar = new TextField());
    bottomPanel.setBackground(Color.lightGray);
    statusBar.setEditable(false);

    // zoom image panel
    zoomImagePanel = new ScrollingPanel(100,100);

    Panel lPanel = new Panel();
    lPanel.setLayout(new BorderLayout());

    lPanel.add("Center",zoomImagePanel);
    //lPanel.add("South", zCtrPanel);

    Panel uPanel = new Panel();
    uPanel.setLayout(new GridLayout(1,2));
    uPanel.add(imgPanel);
    uPanel.add(lPanel);

    retPanel.setLayout(new BorderLayout());
    retPanel.add("Center", uPanel);
    retPanel.add("South", bottomPanel);

    return retPanel;

}

 

  /** set the action flag for displaying the image
   * @param actionFlag the action flag value
   * @exception IOException If an I/O error has occurred.
   */ 
  public void setImage(int actionFlag) throws IOException {
    
    int   i;
    Image img_;

    // "Next" button pressed
    if (actionFlag == NEXT) {  // next...

      // next image plane number
      ++numPlane;
      if (numPlane > plane)
	numPlane = 1;
      
      //next image
      img_ = imageCanvas.getImage(numPlane);

      // set image for current canvas & repaint it
      imageCanvas.setImage(img_);

    }
    // "Previous" button pressed
    if (actionFlag == PREVIOUS) {  // previous

      // previous image plane number
      --numPlane;

      // adjust the value
      if (numPlane < 1)
	numPlane = plane;

      // get previous image
      img_ = imageCanvas.getImage(numPlane);

      // set image for current canvas
      imageCanvas.setImage(img_);

    }

    imgField.setText(String.valueOf(numPlane));
    imgLabel.setText(numPlane+" of "+plane);

  }

  /** popup the new frame actually */
  public void popup()
  {

    // resize the frame; (which default size is best?)
    setSize(frameWidth, frameHeight);

    //paint the canvas
    imageCanvas.repaint();
  
    // set scrollbar value
    imageCanvas.setHScrollValue();
    imageCanvas.setVScrollValue();   

    // default location
    setLocation(100,100);

    // show the whole components
    setVisible(true);

    //pack();
    setSize(frameWidth, frameHeight);
    show();

  }

    public void close() {

	if (imageCanvas.paletteEditor != null)
	    imageCanvas.paletteEditor.selectedExit();

	// close the window
	dispose();

	if (jhvImageAnimation != null) {
	    jhvImageAnimation.animatedImage.dispose();
	    jhvImageAnimation.animatedImage = null;
	    jhvImageAnimation = null;
	}

	// shut down all my control
	// for sds
	if (sdsImageControl != null) {
	   sdsImageControl.close();
	   sdsImageControl = null;
	}

	if (imageRGBControl != null) {
	   imageRGBControl.close();
	   imageRGBControl = null;
	}

	if (imageBCControl != null) {
	   imageBCControl.close();
	   imageBCControl = null;
	}
	if (imageCanvas.hPlot != null) {
	   (imageCanvas.hPlot).xyPlotExit();
	   imageCanvas.hPlot = null;
	}
	if (imageCanvas.aPlot != null) {
	   (imageCanvas.aPlot).xyPlotExit();
	   imageCanvas.aPlot = null;
	}

        jhvCanvas.imageFrame = null;
    }

    /** Enables annimated button */
    public void buttonEnable() {

	nextButton.setEnabled(true);
	prevButton.setEnabled(true);
	anButton.setEnabled(true);
	// stopButton.setEnabled(true);
	// foreButton.setEnabled(true);
	// backButton.setEnabled(true);	
    }

    /** Disables annimated button */
    public void buttonDisable() {
	
	nextButton.setEnabled(false);
	prevButton.setEnabled(false);
	anButton.setEnabled(false);
	// stopButton.setEnabled(false);
	// foreButton.setEnabled(false);
	// backButton.setEnabled(false);	
    }

    public void handleMenuEvent(String arg)
    {
        spButton.setEnabled(true);
        setCursor(new Cursor(Cursor.WAIT_CURSOR));

        if ("Close".equals(arg))
            close();
        else if ("Save As HDF".equals(arg))
        {
            //jhvCanvas.imageFrame.setCursor(new Cursor(WAIT_CURSOR));
            try { save(true); }
            catch (Exception ex) { applet_.infoText.setText(ex.getMessage()); }
            //setCursor(new Cursor(DEFAULT_CURSOR));
        }
        else if ("Save As Text".equals(arg))
        {
            try { save(false); }
            catch (Exception ex) { applet_.infoText.setText(ex.getMessage()); }
        }
        else if ("Show Statistics".equals(arg))
            showStatistics(imageCanvas.hdfData);
        else if ("Convert Data".equals(arg))
            convertData(imageCanvas.hdfData);
        else if (("Undo All".equals(arg)) || ("Undo".equals(arg)))
            handleEditMenuEvent(arg);
        else if (("New Palette".equals(arg)) || ("Set Palette".equals(arg)))
            handlePaletteMenuEvent(arg);
        else if ( ("Emboss".equals(arg))           ||
                  ("Smooth".equals(arg))           ||
                  ("Sharpen".equals(arg ))         ||
                  ("Rotate".equals(arg))           ||
                  ("Add Noise".equals(arg))        ||
                  ("Find Edge".equals(arg))        ||
                  ("Flip(Horizontal)".equals(arg)) ||
                  ("Flip(Vertical)".equals(arg))   ||
                  ("Polarize".equals(arg)) )
            handleFilterEvent(arg);
        else if ( ("Negative".equals(arg))           ||
                  ("Range Modification".equals(arg)) ||
                  ("RGB Balance".equals(arg))        ||
                  ("Brightness/Contrast".equals(arg))  )
            handleImageMenuEvent(arg);
        else if (("Level 3".equals(arg))||
                 ("Level 5".equals(arg))||
                 ("Level 9".equals(arg)))
            handleContourMenuEvent(arg);

        setCursor(new Cursor(Cursor.DEFAULT_CURSOR));
    }

    // Handle Edit menu events
    public void handleEditMenuEvent(String arg) {

	    // Undo All
	    if ("Undo All".equals(arg)) 
		imageCanvas.recoverImage();

	    // Undo ...
	    if ("Undo".equals(arg)) {
		spButton.setEnabled(false);
		jhvCanvas.setImage(imageCanvas.preImage);
		imageCanvas.setImage(imageCanvas.preImage);
	    }
    }

    // Handle View menu events
    public void handleViewMenuEvent( CheckboxMenuItem item) {

	    Object labels[] = item.getSelectedObjects();
	    if (labels == null) return;
	    String label = (String)labels[0];

	    // Pixel Value
	    if ("Pixel Value".equals(label)) {
		noneItem.setState(false);
	        digitItem.setState(false);
		pixelItem.setState(true);
		imageCanvas.dispImageDataMode = PIXEL_VALUE;
		imageCanvas.setShowCoordinate(true);
	    } else  
	      	if ("Digital Value".equals(label))  {
			noneItem.setState(false);
			digitItem.setState(true);
			pixelItem.setState(false);		
			imageCanvas.dispImageDataMode = DIGIT_VALUE;
			imageCanvas.setShowCoordinate(true);	
	    	} else {
	      	if ("Default".equals(label))  {
			noneItem.setState(true);
			digitItem.setState(false);
			pixelItem.setState(false);			
			imageCanvas.dispImageDataMode = NONE_VALUE;
			imageCanvas.setShowCoordinate(false);
		}
	       }	    
    }

    // Handle Plot menu events
    public void handlePlotMenuEvent( CheckboxMenuItem item) {

	    Object labels[] = item.getSelectedObjects();
	    if (labels == null) return;
	    String label = (String)labels[0];

	    // radial polts
	    if ("X-Y Plot".equals(label)) {
		noPlotItem.setState(false);
		radialItem.setState(true);
		histogramItem.setState(false);
		imageCanvas.imagePlotMode = RADIAL_PLOT;
	    } else  {
	      	if ("Histogram".equals(label))  {
			noPlotItem.setState(false);
			radialItem.setState(false);
			histogramItem.setState(true);
			imageCanvas.imagePlotMode = HISTOGRAM_PLOT;
	        }      	
	  	else if ("No Plot".equals(label))  {
			noPlotItem.setState(true);
			radialItem.setState(false);
			histogramItem.setState(false);
			imageCanvas.imagePlotMode = NONE_PLOT;
	       }
	   } // if ("Radial Plots".equals(label)) {
    }

    public void setNewPalette() {

	imageCanvas.setNewPalette();
    }

    public void openNewPalette() {

	imageCanvas.openNewPalette();
    }

    public void handlePaletteMenuEvent(String arg) {
	
	    if ("New Palette".equals(arg)) 
		openNewPalette();
 
	    if ("Set Palette".equals(arg)) 
		setNewPalette();
    }

 public void handleTextFieldEvent(TextField target, String arg) {
	// image number
	if(target instanceof TextField) {
   
 		TextField textFld = (TextField)target;
		String numStr = (String)textFld.getText();
		numPlane = Integer.parseInt(numStr);
		
		// get image number, then display it if possible
		if (numPlane>plane) {
                   numPlane = plane;
                   textFld.setText(String.valueOf(plane));
                }
                else if (numPlane<1)
                {
                   numPlane = 1;
                   textFld.setText(String.valueOf(1));
                }

                imgLabel.setText(numPlane + " of " + plane);

   		// set image for current canvas & repaint it
      		imageCanvas.setImage(imageCanvas.getImage(numPlane));
      	}  
 }

    public void setImageFilter(ImageFilter filter) {

	spButton.setEnabled(false);
	imageCanvas.setImageFilter(filter);
    }

    public void handleImageMenuEvent(String arg) {

  	    if ("Negative".equals(arg)) 
		setImageFilter((ImageFilter)new NegativeFilter());
	    else if ("Range Modification".equals(arg))   // image
	              sdsImageControl = new JHVSDSImageControl(imageCanvas);
 		 else if ("RGB Balance".equals(arg))  
	      		imageRGBControl = new JHVImageRGBControl(imageCanvas);
		      else if ("Brightness/Contrast".equals(arg))  
 			      imageBCControl = new JHVImageBCControl(imageCanvas);
    }

    public void handleFilterEvent(String arg) {

	    // Image Filter
	    if ("Emboss".equals(arg)) 
		setImageFilter((ImageFilter)new EmbossFilter());
     
	    if ("Smooth".equals(arg)) 
		setImageFilter((ImageFilter)new BlurFilter());
   
	    if ("Sharpen".equals(arg)) 
		setImageFilter((ImageFilter)new SharpFilter(3));
  
	    if ("Rotate".equals(arg)) 
		setImageFilter((ImageFilter)new RotateFilter((double)Math.PI/2.0d));
  
	    if ("Add Noise".equals(arg)) 
		setImageFilter((ImageFilter)new NoiseFilter());

   	    if ("Find Edge".equals(arg)) 
		setImageFilter((ImageFilter)new EdgeFilter());

  	    if ("Flip(Horizontal)".equals(arg))  // Horizontal ...
		setImageFilter((ImageFilter)new FlipFilter(FlipFilter.HORIZONTAL));
   
	    if ("Flip(Vertical)".equals(arg))  // Vertical ...
		setImageFilter((ImageFilter)new FlipFilter(FlipFilter.VERTICAL));

   	    if ("Polarize".equals(arg))  
		setImageFilter((ImageFilter)new PolarFilter());
    }

    public void handleContourMenuEvent(String arg) {

	    if ("Level 3".equals(arg)) 
		setImageFilter((ImageFilter)new ContourFilter(3));
	    if ("Level 5".equals(arg)) 
		setImageFilter((ImageFilter)new ContourFilter(5));
	    if ("Level 9".equals(arg)) 
		setImageFilter((ImageFilter)new ContourFilter(9));
  }

    /**
     * Handles Button Events
     */
    public void actionPerformed(ActionEvent e)
    {
        Object target = e.getSource();
        String arg = e.getActionCommand();
        statusBar.setText("");

        if (target instanceof TextField)
        {
            TextField textField = (TextField)target;
            handleTextFieldEvent(textField, arg);
        }
        else if ("Spreadsheet".equals(arg))
            imageCanvas.makeSpreadsheet();
        else if ("Animation".equals(arg))
        {
            if (jhvImageAnimation == null)
                jhvImageAnimation = new JHVImageAnimation(imageCanvas.getImages());
            else {
                ImageAnimation an = jhvImageAnimation.animatedImage;
                if (!an.isShowing()) an.setVisible(true);
            }
        }
        else if ("Next".equals(arg))
        {
            try { setImage(NEXT); }
            catch(Exception ex) {};
        }
        else if ("Previous".equals(arg))
        {
            try { setImage(PREVIOUS); }
            catch (Exception ex) {};
        }
        else if (target instanceof MenuItem)
            handleMenuEvent(arg);
  }

     public void setSubsetRange(Rectangle rect) {
	imageCanvas.setSubsetRange(rect);
    }

  public void itemStateChanged(ItemEvent e)  {
 
    plane = imageCanvas.numberOfImage;
    ItemSelectable target = e.getItemSelectable();
    
    if (target instanceof CheckboxMenuItem) { // view menu & plot menu
	Object labels[] = ((CheckboxMenuItem)target).getSelectedObjects();
	if (labels == null) return;
	String label = (String)labels[0];
 	if (("Pixel Value".equals(label))   ||
	    ("Digital Value".equals(label)) ||
	    ("Default".equals(label)) )
	   handleViewMenuEvent((CheckboxMenuItem)target);
	else 
	   handlePlotMenuEvent((CheckboxMenuItem)target);
    }
    else  if(target instanceof Checkbox) {
	Checkbox checkBox = (Checkbox)target;
	String label = checkBox.getLabel();
	
	if (label.equals("1x"))
	    zoomFactor = 1.0f;
	else if (label.equals("2x"))
	    zoomFactor = 2.0f;
	else if (label.equals("5x"))
	    zoomFactor = 5.0f;
        else if (label.equals("10x"))
	    zoomFactor = 10.0f;
    }
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
    Scrollbar target = (Scrollbar) e.getAdjustable();

    switch (type) { 
    case AdjustmentEvent.UNIT_INCREMENT:
    case AdjustmentEvent.UNIT_DECREMENT:
    case AdjustmentEvent.BLOCK_INCREMENT:
    case AdjustmentEvent.BLOCK_DECREMENT:
    case AdjustmentEvent.TRACK:

      // detect the vertical scrollbar
      if (target ==  vScrollbar) {
		
	// get translated value
	imageCanvas.ty = vScrollbar.getValue();
		
	// repaint the graphics
	imageCanvas.repaint();
	
      }
      // detect the horizontal scrollbar
      if (target ==  hScrollbar) {
	
	// get translated value
	imageCanvas.tx = hScrollbar.getValue();
	    
	// repaint the graphics
	imageCanvas.repaint();
      }
    }
  }

    public void setSliceInfo(SDSDimInfo info)
    {
        imageCanvas.setSliceInfo(info);
    }

    /** save the data into HDF file or ASCII file */
    public void save(boolean isHDF)
    {
        FileDialog fd = new FileDialog(this, "Save HDF Data", FileDialog.SAVE);
        fd.setDirectory(applet_.cDir);
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
                warning = fullname+"\n"+
                    "This file is read only.\n"+
                    "File save failed.";
                statusBar.setText(warning);
                id = new InfoDialog(applet_.jhvFrame, fd.getTitle(), warning, applet_.warningIcon, false);
                id.show();
                return;
            }
	}

        String workingFile = applet_.asciiFile;
        if (isHDF) workingFile = applet_.hdfFile;
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
            id = new InfoDialog(applet_.jhvFrame, fd.getTitle(),
                warning, applet_.warningIcon, true);
            id.setlabels("  Yes  ", "   No   ");
            id.show();
            if (!id.clickedOkButton()) return;
        }
        else if (!applet_.isMac() && !applet_.isWin())
        {
            if (outFile.exists()) {
                warning = fullname+"\n"+
                    "This file already exists.\n"+ "  \n"+
                    "Replace existing file ?";
                statusBar.setText(warning);
                id = new InfoDialog(applet_.jhvFrame, fd.getTitle(),
                    warning, applet_.warningIcon, true);
                    id.setlabels("  Yes  ", "   No   ");
                    id.show();
                if (!id.clickedOkButton()) return;
            }
        }

        // reset the current working directory in JHV
        applet_.cDir = dir;

        String nl = JHV.NL;
        HDFObjectNode node = imageCanvas.jhvCanvas.node;
        int dataType = imageCanvas.hdfDataType;
        int dims[] = {imageCanvas.imageWidth, imageCanvas.imageHeight};
        Object hdfData = imageCanvas.hdfData;

        // set file annotation
        workingFile = applet_.asciiFile;
        if (applet_.isHDF()) workingFile = applet_.hdfFile;
        String annotation = "";
        annotation +=    "File Name   : "+theFile;
        annotation += nl+"Data Object : "+node.getName().trim();
        annotation += nl+"Data Subset : ("+imageCanvas.subsetRange.x+", "+imageCanvas.subsetRange.y+") - (" +
            (imageCanvas.subsetRange.x+imageCanvas.subsetRange.width)+", "+
            (imageCanvas.subsetRange.y+imageCanvas.subsetRange.height)+")";
        annotation += nl+"Source File : "+workingFile;
        annotation += nl+"Created on  : "+(new java.util.Date()).toString();
        annotation += nl+"Created from: Java HDF Viewer";

        if (isHDF)
        {
            // write HDF file
            int[] start = {0, 0};
            int[] stride = {1, 1};
            HDFWriter writer = new HDFWriter(fullname);
            byte[] palette = imageCanvas.imagePalette;
            if (palette == null)
                palette = HDFObject.createRainbowPalette();

            try
            {
                writer.writeAnnotation(annotation, HDFConstants.DFACC_CREATE);
                switch(node.type)
                {
                    case HDFObjectNode.RIS8:
                    case HDFObjectNode.RIS24:
                    case HDFObjectNode.GRDATASET:
                        writer.writeGR(HDFConstants.DFACC_WRITE, node.getName(),
                            1, dataType, HDFConstants.MFGR_INTERLACE_PIXEL,
                            dims, start, null, 0, hdfData, palette);
                        break;
                    case HDFObjectNode.SDSDATASET:
                        imageCanvas.updateSDSdata();
                        int[] dimsds = {dims[1], dims[0]};
                        writer.writeSDS(HDFConstants.DFACC_WRITE, node.getName(),
                            2, dataType, dimsds, start, null, hdfData);
                        break;
                    case HDFObjectNode.Vdata:
                        // implementec in JHVVdataFrame
                        break;
                    default:
                }
            } catch (Exception ex) {
                warning = fullname+"\n"+ "The write failed.\n"+ "  \n"+ "Exception: "+ex.getMessage();
                statusBar.setText(warning);
                id = new InfoDialog(applet_.jhvFrame, fd.getTitle(),
                    warning, applet_.warningIcon, false);
                id.setlabels("  OK  ","");
                id.show();
                if (id.clickedOkButton()) return;
            }

        }
        else
        {
            // write ASCII file
            String format = HDFConstants.RI8;
            int rank = 2;

            if (node.type == HDFObjectNode.RIS24)
                format = HDFConstants.RI24;
            else if (node.type == HDFObjectNode.GRDATASET)
                format = HDFConstants.GR;
            else if ( node.type == HDFObjectNode.SDSDATASET)
                format = HDFConstants.SDS;
            else if (node.type == HDFObjectNode.Vdata ||
                node.type == HDFObjectNode.Annotation)
                return;

            ASCIIWriter writer = null;

            // create a new file or replace the old file
            try
            {
                writer = new ASCIIWriter(fullname);
                writer.writeAnnotation(annotation);
                writer.close();
            } catch (Exception ex) { statusBar.setText(ex.getMessage()); }

            // append to the old file
            try
            {
                writer = new ASCIIWriter(fullname,true);
                writer.setDelimiter(applet_.delimiter);
                Dimension d = new Dimension(dims[0], dims[1]);
                writer.write2DData(format, node.getName(), rank, d, dataType, hdfData);
                writer.close();
            } catch (Exception ex) {
                warning = fullname+"\n"+
                "The write failed.\n"+ "  \n"+ "Exception: "+ex.getMessage();
                statusBar.setText(warning);
                id = new InfoDialog(applet_.jhvFrame, fd.getTitle(),
                    warning, applet_.warningIcon, false);
                id.setlabels("  OK  ","");
                id.show();
                if (id.clickedOkButton()) return;
            }
        }

        statusBar.setText("Save data plane "+numPlane+" successful at "+fullname);
    }

    /** show the basic statictics for the given data array */
    public void showStatistics(Object data)
    {
        if (data == null) return;

        Object range = Tools.getRange(data);

        if (range == null) return;

        double mean = Tools.mean(data);
        double variance = Tools.var(data);

        String info = "";
        info += "Minimum  = "+Array.get(range, 0)+"\n";
        info += "Maximum  = "+Array.get(range, 1)+"\n";
        info += "Average  = "+mean+"\n";
        info += "Variance = "+variance;

        InfoDialog id = new InfoDialog(this, "Data Statistics", info, null, false);
        id.show();
    }

    /** convert data into another data set */
    public void convertData(Object data)
    {
        if (data == null || !data.getClass().isArray()) return;

        JHVDataConversion jdc = new JHVDataConversion(this,"Data Conversion",true);

        if (!jdc.needDataConversion()) return;

        double[] f = jdc.getFunction();
        int dataType = imageCanvas.hdfDataType;

        // convert data
        int dataSize = Array.getLength(data);
        double theData = 0;
        for (int i=0; i<dataSize; i++)
        {
            theData = Array.getDouble(data, i);
            try { theData = f[0]+f[1]*Math.pow(theData, f[2]); }
            catch (Exception e) { continue; }

            switch(dataType)
            {
                case HDFConstants.DFNT_CHAR:
                case HDFConstants.DFNT_UCHAR8:
                case HDFConstants.DFNT_UINT8:
                case HDFConstants.DFNT_INT8:
                    Array.setByte(data, i, (byte)theData);
                    break;
                case HDFConstants.DFNT_INT16:
                case HDFConstants.DFNT_UINT16:
                    Array.setShort(data, i, (short)theData);
                    break;
                case HDFConstants.DFNT_INT32:
                case HDFConstants.DFNT_UINT32:
                    Array.setInt(data, i, (int)theData);
                    break;
                case HDFConstants.DFNT_FLOAT32:
                    Array.setFloat(data, i, (float)theData);
                    break;
                case HDFConstants.DFNT_INT64:
                case HDFConstants.DFNT_UINT64:
                    Array.setLong(data, i, (long)theData);
                    break;
                default:
                    Array.setDouble(data, i, theData);
                    break;
            }
        }

        byte[] imageData = new byte[dataSize];
        double minmax[] = {0,0};
        Object range = Tools.getRange(data);
        minmax[0] = Array.getDouble(range, 0);
        minmax[1] = Array.getDouble(range, 1);
        HDFObject.object2byte(data, dataType, minmax, imageData);
        Image image = imageCanvas.createRasterImage(imageData,
            imageCanvas.imageWidth, imageCanvas.imageHeight, imageCanvas.imagePalette,1);
        imageCanvas.setImage(image);
    }

}



