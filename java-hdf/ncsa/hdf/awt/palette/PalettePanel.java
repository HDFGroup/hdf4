
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
import java.util.*;
import java.io.*;
import java.net.*;

import java.awt.image.*;
import java.awt.event.*;
import java.awt.Image;
import ncsa.hdf.awt.image.*;

// This class will let you edit the Palette value that can be used by some of image
public class PalettePanel extends Panel implements ActionListener, ItemListener
{
	// who is my parent, PaletteEditor?
	PaletteEditor  parent;

	// Variables for this panel

	// for palette value panel
	Label 		paletteLabel;
	PaletteCanvas 	paletteCanvas;
	Button 	        flipButton;
	Checkbox 	fiddleCheckbox;
	Button 	        randomButton;
	Button 	        undoButton;

	// for red palette 	 
	SingleColorCanvas 		redCanvas; // need to derive from other 
	Label 		redLabel;
	CheckboxGroup   redCheckboxGroup;
	Checkbox 	redLineCheckbox;
	Checkbox 	redPointCheckbox;

	// for green palette	  
	SingleColorCanvas 		greenCanvas; // need to derive from other
	Label 		greenLabel;
	CheckboxGroup   greenCheckboxGroup;
	Checkbox 	greenLineCheckbox;
	Checkbox 	greenPointCheckbox;

	// for blue palette	
	SingleColorCanvas 		blueCanvas; // need to derive from other 
	Label 		blueLabel;
	CheckboxGroup   blueCheckboxGroup;
	Checkbox 	blueLineCheckbox;
	Checkbox 	bluePointCheckbox;

	// default constructor
	public PalettePanel()
	{
		super();

		// interface
		palettePanelConstructor();

		flipButton.addActionListener(this);
		randomButton.addActionListener(this);
		undoButton.addActionListener(this);
	
		fiddleCheckbox.addItemListener(this);
		redLineCheckbox.addItemListener(this);
		redPointCheckbox.addItemListener(this);
	
		greenLineCheckbox.addItemListener(this);
		greenPointCheckbox.addItemListener(this);

		blueLineCheckbox.addItemListener(this);
		bluePointCheckbox.addItemListener(this);

	}

    // constructor by PaletteEditor 
    public PalettePanel(PaletteEditor parent) {

	this();
	this.parent = parent;
    }

	// Palette graphic interface creator
	private final void palettePanelConstructor()
	{
		// layout manager control(null)
		setLayout( null );

		// color control for panel
		setForeground( new Color( 0, 0, 0 ));
		setBackground( new Color( 192, 192, 192 ));

		// default size of the whole panel
		setSize( (getInsets().left + getInsets().right + 434), (getInsets().top + getInsets().bottom + 451) );

		// palette label
		paletteLabel = new Label( "Palette:", Label.CENTER );
		paletteLabel.setFont( new Font( "TimesRoman", Font.BOLD, 16 ));
		paletteLabel.setForeground( new Color( 64, 0, 0 ));
		add( paletteLabel );
		paletteLabel.setBounds( (getInsets().left + 19), (getInsets().top + 43), 55, 16 );

		// undo button  
		undoButton = new Button( "Undo");
		undoButton.setFont( new Font( "TimesRoman", Font.BOLD, 16 ));
		undoButton.setForeground( new Color( 64, 0, 0 ));
		add( undoButton );
		undoButton.setBounds( (getInsets().left + 19), (getInsets().top + 60), 55, 28 );
		
		// set undoButton is unavailable
		undoButton.setEnabled(false);
		
		// palette edited canvas
		paletteCanvas = new PaletteCanvas();
		paletteCanvas.setBackground( new Color( 128, 128, 128 ));
		add( paletteCanvas );
		paletteCanvas.setBounds( (getInsets().left + 85), (getInsets().top + 43), 256, 64 );

		paletteCanvas.repaint();
		
		// palette edited operation (flip)
		flipButton = new Button( " Flip" );
		flipButton.setFont( new Font( "TimesRoman", Font.BOLD, 14 ));
		add( flipButton );
		flipButton.setBounds( (getInsets().left + 355), (getInsets().top + 32), 61, 24 );

		// palette edited operation (fiddle)
		fiddleCheckbox = new Checkbox( "Fiddle" );
		fiddleCheckbox.setFont( new Font( "TimesRoman", Font.BOLD, 14 ));
		add( fiddleCheckbox );
		fiddleCheckbox.setBounds( (getInsets().left + 355), (getInsets().top + 71), 61, 18 );
			
		// palette edited operation (random)
		randomButton = new Button( " Random" );
		randomButton.setFont( new Font( "TimesRoman", Font.BOLD, 14 ));
		add( randomButton );
		randomButton.setBounds( (getInsets().left + 355), (getInsets().top + 95), 61, 24 );

		// red panel
		redLabel = new Label( "Red", Label.CENTER );
		redLabel.setFont( new Font( "TimesRoman", Font.BOLD, 16 ));
		redLabel.setForeground( new Color( 255, 0, 0 ));
		add( redLabel );
		redLabel.setBounds( (getInsets().left + 19), (getInsets().top + 148), 55, 16 );

		// red palette edited canvas
		redCanvas = new SingleColorCanvas(paletteCanvas);
		redCanvas.setCanvasType(SingleColorCanvas.RED);

		redCanvas.setBackground( new Color( 255, 151, 151 ));
		add( redCanvas );
		redCanvas.setBounds( (getInsets().left + 85), (getInsets().top + 148), 256, 64 );

		// set red values for redCanvas
		redCanvas.setData(paletteCanvas.getRedValues());
		
		// draw selection
		redCheckboxGroup = new CheckboxGroup();

		redPointCheckbox = new Checkbox( "Point", redCheckboxGroup, true );
		redPointCheckbox.setState( true );
		redPointCheckbox.setFont( new Font( "TimesRoman", Font.BOLD, 14 ));
		add( redPointCheckbox );
		redPointCheckbox.setBounds( (getInsets().left + 355), (getInsets().top + 153), 61, 18 );

		redLineCheckbox = new Checkbox( "Line", redCheckboxGroup, false );
		redLineCheckbox.setFont( new Font( "TimesRoman", Font.BOLD, 14 ));
		add( redLineCheckbox );
		redLineCheckbox.setBounds( (getInsets().left + 355), (getInsets().top + 179), 61, 18 );

		// green  panel	
		greenLabel = new Label( "Green", Label.CENTER );
		greenLabel.setFont( new Font( "TimesRoman", Font.BOLD, 16 ));
		greenLabel.setForeground( new Color( 0, 255, 0 ));
		add( greenLabel );
		greenLabel.setBounds( (getInsets().left + 19), (getInsets().top + 253), 55, 16 );
	
		// green palette edited canvas			
		greenCanvas = new SingleColorCanvas(paletteCanvas);
		greenCanvas.setCanvasType(SingleColorCanvas.GREEN);

		greenCanvas.setBackground( new Color( 168, 255, 168 ));
		add( greenCanvas );
		greenCanvas.setBounds( (getInsets().left + 85), (getInsets().top + 253), 256, 64 );
			
		// set green values for redCanvas
		greenCanvas.setData(paletteCanvas.getGreenValues());
	
		// draw selection
		greenCheckboxGroup = new CheckboxGroup();
		greenPointCheckbox = new Checkbox( "Point",greenCheckboxGroup,true );
		greenPointCheckbox.setState( true );
		greenPointCheckbox.setFont( new Font( "TimesRoman", Font.BOLD, 14 ));
		add( greenPointCheckbox );
		greenPointCheckbox.setBounds( (getInsets().left + 355), (getInsets().top + 261), 61, 18 );

		greenLineCheckbox = new Checkbox( "Line",greenCheckboxGroup,false );
		greenLineCheckbox.setFont( new Font( "TimesRoman", Font.BOLD, 14 ));
		add( greenLineCheckbox );
		greenLineCheckbox.setBounds( (getInsets().left + 355), (getInsets().top + 286), 61, 18 );

		// blue  panel
		blueLabel = new Label( "Blue", Label.CENTER );
		blueLabel.setFont( new Font( "TimesRoman", Font.BOLD, 16 ));
		blueLabel.setForeground( new Color( 0, 0, 255 ));
		add( blueLabel );
		blueLabel.setBounds( (getInsets().left + 19), (getInsets().top + 358), 54, 18 );

		// blue canvas
		blueCanvas = new SingleColorCanvas(paletteCanvas);
		blueCanvas.setCanvasType(SingleColorCanvas.BLUE);

		blueCanvas.setBackground( new Color( 151, 190, 244 ));
		add( blueCanvas );
		blueCanvas.setBounds( (getInsets().left + 85), (getInsets().top + 358), 256, 64 );
	
		// set blue values for blueCanvas
		blueCanvas.setData(paletteCanvas.getBlueValues());
	
		// draw operation
		blueCheckboxGroup = new CheckboxGroup();
		bluePointCheckbox = new Checkbox( "Point",blueCheckboxGroup,true);
		bluePointCheckbox.setState( true );
		bluePointCheckbox.setFont( new Font( "TimesRoman", Font.BOLD, 14 ));
		add( bluePointCheckbox );
		bluePointCheckbox.setBounds( (getInsets().left + 355), (getInsets().top + 368), 61, 16 );

		blueLineCheckbox = new Checkbox( "Line", blueCheckboxGroup, false);
		blueLineCheckbox.setFont( new Font( "TimesRoman", Font.BOLD, 14 ));
		add( blueLineCheckbox );
		blueLineCheckbox.setBounds( (getInsets().left + 355), (getInsets().top + 391), 61, 18 );
	}

	//  palette canvas has different value, separate canvas should be repaint
	protected void setRGBPanel()
	{	
		redCanvas.setData(paletteCanvas.getRedValues());
		greenCanvas.setData(paletteCanvas.getGreenValues());
		blueCanvas.setData(paletteCanvas.getBlueValues());
	}

    // RGB value has been change , the orignal image  should be changed ?
    public void colorModelChanged() {

	ColorModel cm = paletteCanvas.currentColorModel();

	if (cm != null) {
	    // redraw new image
	    parent.colorModelChanged(cm);

	}

    }

	// ready to change the image, 
	void PaletteCanvasMouseUp( AWTEvent event )
	{
		
		setUndoButtonState();
		
		// separate canvas(red,green,blue) should be changed
		redCanvas.setData(paletteCanvas.getRedValues());
		greenCanvas.setData(paletteCanvas.getGreenValues());
		blueCanvas.setData(paletteCanvas.getBlueValues());

		// test for image filter
		colorModelChanged();

	}

	// drag the panel , color model will changed
	protected void PaletteCanvasMouseDrag( AWTEvent event )
	{
		// 	change each rgb canvas
		redCanvas.setData(paletteCanvas.getRedValues());
		greenCanvas.setData(paletteCanvas.getGreenValues());
		blueCanvas.setData(paletteCanvas.getBlueValues());

	}

	protected void RedPointCheckboxClicked( AWTEvent event )
	{
		// 	set drawable model
		redCanvas.setDrawMode(redCanvas.POINT);
	}

	protected void RedLineCheckboxClicked( AWTEvent event )
	{
		// 	set drawable model
		redCanvas.setDrawMode(redCanvas.LINE);	
	}

	protected void GreenPointCheckboxClicked( AWTEvent event )
	{ 
		// 	set drawable model
		greenCanvas.setDrawMode(greenCanvas.POINT);	   
	}

	protected void GreenLineCheckboxClicked( AWTEvent event )
	{
	  	
	    // 	set drawable model
	    greenCanvas.setDrawMode(greenCanvas.LINE);	
	}

        protected void BluePointCheckboxClicked( AWTEvent event )
	{
	    // 	set drawable model
	    blueCanvas.setDrawMode(blueCanvas.POINT);		
	}

	protected void BlueLineCheckboxClicked( AWTEvent event )
	{ 
	    // 	set drawable model
	    blueCanvas.setDrawMode(blueCanvas.LINE);	
	}

	protected void FlipButtonPressed( AWTEvent event )
	{
		// new image, so ...
		paletteCanvas.pushPaletteValue();

		// 	paletteCanvas ...		
		paletteCanvas.flipPalette();
		
		redCanvas.setData(paletteCanvas.getRedValues());
		greenCanvas.setData(paletteCanvas.getGreenValues());
		blueCanvas.setData(paletteCanvas.getBlueValues());
	
		// color model changed, so ...
		colorModelChanged();
	}

	protected void undoButtonPressed( AWTEvent event )
	{

		// unselected fiddleButton
		fiddleCheckbox.setState(false);
		paletteCanvas.fiddleFlag = false;

		// 	paletteCanvas ...		
		paletteCanvas.undoPalette();
		
		redCanvas.setData(paletteCanvas.getRedValues());
		greenCanvas.setData(paletteCanvas.getGreenValues());
		blueCanvas.setData(paletteCanvas.getBlueValues());

		// color model changed, so ...
		colorModelChanged();
	}

	protected void setUndoButtonState(  )
	{

		if (paletteCanvas.paletteStack.empty()) 
		    undoButton.setEnabled(false);
		else
		    undoButton.setEnabled(true);
	}

	protected void RandomButtonPressed( AWTEvent event )
	{	// new image, so ...
		paletteCanvas.pushPaletteValue();
	
		// 	paletteCanvas ...		
		paletteCanvas.setRandomPalette();
		
		redCanvas.setData(paletteCanvas.getRedValues());
		greenCanvas.setData(paletteCanvas.getGreenValues());
		blueCanvas.setData(paletteCanvas.getBlueValues());
			
		// color model changed, so ...
		colorModelChanged();
	}

	// select or unselect the fiddle, this will change the way to
        // set the palette.
	protected void FiddleCheckboxClicked( AWTEvent event )
	{
		// 	paletteCanvas
		paletteCanvas.fiddleFlag = fiddleCheckbox.getState();
		
		// replace rgb with "fiddle" rgb
		paletteCanvas.setFiddleValues();
	}

    // RGB value has been change , the orignal image  should be changed ?
    // (redundant?)
    public void setImage() {

	ColorModel cm = paletteCanvas.currentColorModel();

	if (cm != null) {
		ImageProducer imageProducer = parent.object.getImage().getSource();

		Image newImage;

		// new filter (set to new RGB model)
		ColorModelFilter  imageFilter = new ColorModelFilter(cm);

		// imageFilter.setColorModel((IndexColorModel)cm);
		FilteredImageSource  src = new FilteredImageSource(imageProducer,imageFilter);

		newImage = Toolkit.getDefaultToolkit().createImage(src);
	
		// which object contains the image adjusted by palette editor
		// redraw new image
		parent.object.setImage(newImage);

	}

    }

    /**
     * java.awt.event.ActionListener stuff
     */
    public void actionPerformed(ActionEvent event)  {

	// modify the undoButton status
	setUndoButtonState();
		
	// for undo button
	if ((event.getSource()== undoButton) && 
	    (event.getID() == ActionEvent.ACTION_PERFORMED)) {
	    undoButtonPressed( event );
	}
	
	// for flip button
	if ((event.getSource()== flipButton) &&
	    (event.getID() == ActionEvent.ACTION_PERFORMED)) {
	    FlipButtonPressed( event );
	}

	// for random
	if ((event.getSource()== randomButton) &&
	    (event.getID() == ActionEvent.ACTION_PERFORMED)) {
	    RandomButtonPressed( event );
	}
    }

    /**
     * java.awt.event.ItemListener stuff
     */
    public void itemStateChanged(ItemEvent event)  {

	// modify the undoButton status
	setUndoButtonState();

	// for fiddle
	if ((event.getSource()== fiddleCheckbox) && (event.getID() == ItemEvent.ITEM_STATE_CHANGED))
	{
		FiddleCheckboxClicked( event );
	}

	// for red point 
	if ((event.getSource()== redPointCheckbox) && (event.getID() == ItemEvent.ITEM_STATE_CHANGED))
	{
		RedPointCheckboxClicked( event );
		//return true;
	}
	
	// for red line
	if ((event.getSource()== redLineCheckbox) && (event.getID() == ItemEvent.ITEM_STATE_CHANGED))
	{
		RedLineCheckboxClicked( event );
		//return true;
	}
		
	// green point
	if ((event.getSource()== greenPointCheckbox) && (event.getID() == ItemEvent.ITEM_STATE_CHANGED))
	{
		GreenPointCheckboxClicked( event );
		//return true;
	}

	// green line
	if ((event.getSource()== greenLineCheckbox) && (event.getID() == ItemEvent.ITEM_STATE_CHANGED))
	{
		GreenLineCheckboxClicked( event );
		//return true;
	}

	// blue point
	if ((event.getSource()== bluePointCheckbox) && (event.getID() == ItemEvent.ITEM_STATE_CHANGED))
	{
		BluePointCheckboxClicked( event );
		//return true;
	}

	// blue line
	if ((event.getSource()== blueLineCheckbox) && (event.getID() == ItemEvent.ITEM_STATE_CHANGED))
	{
		BlueLineCheckboxClicked( event );
		//return true;
	}
    }
}

// SuperCede Begin Properties
// 2
// 48
// 2 "Untitled" 100000 0 400 0 F F F "Dialog"
// 1187 200 F 11 8388736 8421440 F T T T F F 0 0 0 0
// 0 11907263
// 1 432
// 0
// 434 451 655360 0 0 12632256
// 5 0 1 1 0 "PalettePanel" 5 5 0 2 2 0 T T F T F
// ""
// F F F F F F F F F F F F F
// 16
// {839BC529-F563-11CF-B16F-0020AFDA72B7} 19 74 43 59
//   "Palette:" 1 3 "paletteLabel" 2 55 16 4 T T F 1 64 16777215  "TimesRoman" 160000 0 700 0 F F F F F F F F F F F F F -1 -1 1 1 F F 0 0 0 0 0 0 0 0 0 "" T
// {839BC533-F563-11CF-B16F-0020AFDA72B7} 85 341 43 107
//   3 "paletteCanvas" 2 256 64 4 T T T 0 0 8421504  "Dialog" 100000 0 400 0 F T T T T F T F F F F F F -1 -1 1 1 F F 0 0 0 0 0 0 0 0 0 "" F
// {839BC52D-F563-11CF-B16F-0020AFDA72B7} 355 416 49 67
//   " Flip" F T 3 "flipCheckbox" 2 61 18 4 T T T 1 0 16777215  "TimesRoman" 140000 0 700 0 F F F F F F F F F F F F F -1 -1 1 1 F F 0 0 0 0 0 0 0 0 0 "" T
// {839BC52D-F563-11CF-B16F-0020AFDA72B7} 355 416 71 89
//   "Fiddle" F T 3 "fiddleCheckbox" 2 61 18 4 T T T 1 0 16777215  "TimesRoman" 140000 0 700 0 F F F F F F F F F F F F F -1 -1 1 1 F F 0 0 0 0 0 0 0 0 0 "" T
// {839BC52D-F563-11CF-B16F-0020AFDA72B7} 355 416 153 171
//   "Point" T T 3 "redPointCheckbox" 2 61 18 4 T T T 1 0 16777215  "TimesRoman" 140000 0 700 0 F F F F F F F F F F F F F -1 -1 1 1 F F 0 0 0 0 0 0 0 0 0 "" T
// {839BC533-F563-11CF-B16F-0020AFDA72B7} 85 341 148 212
//   3 "redCanvas" 2 256 64 4 T T T 0 0 9934847  "Dialog" 100000 0 400 0 F T T T T F F F F F F F F -1 -1 1 1 F F 0 0 0 0 0 0 0 0 0 "" F
// {839BC529-F563-11CF-B16F-0020AFDA72B7} 19 74 148 164
//   "Red" 1 3 "redLabel" 2 55 16 4 T T F 1 255 16777215  "TimesRoman" 160000 0 700 0 F F F F F F F F F F F F F -1 -1 1 1 F F 0 0 0 0 0 0 0 0 0 "" T
// {839BC52D-F563-11CF-B16F-0020AFDA72B7} 355 416 179 197
//   "Line" F T 3 "redLineCheckbox" 2 61 18 4 T T T 1 0 16777215  "TimesRoman" 140000 0 700 0 F F F F F F F F F F F F F -1 -1 1 1 F F 0 0 0 0 0 0 0 0 0 "" T
// {839BC52D-F563-11CF-B16F-0020AFDA72B7} 355 416 261 279
//   "Point" T T 3 "greenPointCheckbox" 2 61 18 4 T T T 1 0 16777215  "TimesRoman" 140000 0 700 0 F F F F F F F F F F F F F -1 -1 1 1 F F 0 0 0 0 0 0 0 0 0 "" T
// {839BC533-F563-11CF-B16F-0020AFDA72B7} 85 341 253 317
//   3 "greenCanvas" 2 256 64 4 T T T 0 0 11075496  "Dialog" 100000 0 400 0 F T T T T F F F F F F F F -1 -1 1 1 F F 0 0 0 0 0 0 0 0 0 "" F
// {839BC529-F563-11CF-B16F-0020AFDA72B7} 19 74 253 269
//   "Green" 1 3 "greenLabel" 2 55 16 4 T T F 1 65280 16777215  "TimesRoman" 160000 0 700 0 F F F F F F F F F F F F F -1 -1 1 1 F F 0 0 0 0 0 0 0 0 0 "" T
// {839BC52D-F563-11CF-B16F-0020AFDA72B7} 355 416 286 304
//   "Line" F T 3 "greenLineCheckbox" 2 61 18 4 T T T 1 0 16777215  "TimesRoman" 140000 0 700 0 F F F F F F F F F F F F F -1 -1 1 1 F F 0 0 0 0 0 0 0 0 0 "" T
// {839BC52D-F563-11CF-B16F-0020AFDA72B7} 355 416 368 384
//   "Point" T T 3 "bluePointCheckbox" 2 61 16 4 T T T 1 0 16777215  "TimesRoman" 140000 0 700 0 F F F F F F F F F F F F F -1 -1 1 1 F F 0 0 0 0 0 0 0 0 0 "" T
// {839BC533-F563-11CF-B16F-0020AFDA72B7} 85 341 358 422
//   3 "blueCanvas" 2 256 64 4 T T T 0 0 16039575  "Dialog" 100000 0 400 0 F T T T T F F F F F F F F -1 -1 1 1 F F 0 0 0 0 0 0 0 0 0 "" F
// {839BC529-F563-11CF-B16F-0020AFDA72B7} 19 73 358 376
//   "Blue" 1 3 "blueLabel" 2 54 18 4 T T F 1 16711680 16777215  "TimesRoman" 160000 0 700 0 F F F F F F F F F F F F F -1 -1 1 1 F F 0 0 0 0 0 0 0 0 0 "" T
// {839BC52D-F563-11CF-B16F-0020AFDA72B7} 355 416 391 409
//   "Line" F T 3 "blueLineCheckbox" 2 61 18 4 T T T 1 0 16777215  "TimesRoman" 140000 0 700 0 F F F F F F F F F F F F F -1 -1 1 1 F F 0 0 0 0 0 0 0 0 0 "" T
// 16 0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15
// SuperCede End Properties
