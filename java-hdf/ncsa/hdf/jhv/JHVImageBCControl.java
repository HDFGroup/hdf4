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
import java.awt.image.*;


//----------------------------------------------------------------------------------
//
//  Upgraded to the JDK 1.1.1b Event Model
//    - Apu Kapadia May 22nd, 1997.
//
//----------------------------------------------------------------------------------
public class JHVImageBCControl  extends Frame implements ActionListener, ItemListener,
  AdjustmentListener {

	JHVImageCanvas imageCanvas;

	// DECLARE_CONTROLS

	java.awt.Label brightnessLabel;
	java.awt.Label channelLabel;
	java.awt.Label contrastLabel;
	java.awt.Button okButton;
	java.awt.Button cancelButton;
	java.awt.Button previewButton;
	java.awt.Checkbox allCheckbox;	
	java.awt.Checkbox redCheckbox;
	CheckboxGroup channelGroup;
	java.awt.Checkbox greenCheckbox;
	java.awt.Checkbox blueCheckbox;
	java.awt.TextField brightTextField;
	java.awt.Scrollbar brightScrollbar;
	java.awt.Label brightRangeLabel;
	java.awt.TextField contrastTextField;
	java.awt.Scrollbar contrastScrollbar;
	java.awt.Label contrastRangeLabel;

	// Channel State
	public 	static final int CH_ALL   = 0;
	public 	static final int CH_RED   = CH_ALL+1;
	public 	static final int CH_GREEN = CH_ALL+2;
	public 	static final int CH_BLUE  = CH_ALL+3;

	// which operation right now
	public 	static final int BRIGHTNESS   = 0;
	public 	static final int CONTRAST     = 1;

	// which filter 
	int	filterMode  = BRIGHTNESS;

	// mode to draw image
	public 	static final int CONFIRM      = 0;
	public 	static final int PREVIEW      = 1;

	// mode to draw image
	int 	imageMode = PREVIEW;

	// two range value
	int 	brightRangeValue, contrastRangeValue;

	int 	brightValue, contrastValue;

	// thread of the control
	Thread	controlThread = null;

	// control flag
	boolean		controlChangedFlag = false;

	// some of image
	Image	origImage    = null;

 	 public JHVImageBCControl() {

		setLayout(null);
		addNotify();
		setSize(getInsets().left + getInsets().right + 431,getInsets().top + getInsets().bottom + 322);
		brightnessLabel = new java.awt.Label("Brightness:");
		brightnessLabel.setBounds(getInsets().left + 42,getInsets().top + 38,119,30);
		brightnessLabel.setFont(new Font("TimesRoman", Font.PLAIN, 24));
		add(brightnessLabel);

		channelLabel = new java.awt.Label("Channel:");
		channelLabel.setBounds(getInsets().left + 273,getInsets().top + 38,119,30);
		channelLabel.setFont(new Font("TimesRoman", Font.PLAIN, 24));
		add(channelLabel);

		contrastLabel = new java.awt.Label("Contrast:");
		contrastLabel.setBounds(getInsets().left + 49,getInsets().top + 150,119,30);
		contrastLabel.setFont(new Font("TimesRoman", Font.PLAIN, 24));
		add(contrastLabel);

		okButton = new java.awt.Button("Confirm");
		okButton.addActionListener(this);
		okButton.setBounds(getInsets().left + 30,getInsets().top + 240,105,41);
		okButton.setFont(new Font("TimesRoman", Font.PLAIN, 18));
		add(okButton);

		cancelButton = new java.awt.Button("Cancel");
		cancelButton.addActionListener(this);
		cancelButton.setBounds(getInsets().left + 163,getInsets().top + 240,105,41);
		cancelButton.setFont(new Font("TimesRoman", Font.PLAIN, 18));
		add(cancelButton);

		previewButton = new java.awt.Button("Preview");
		previewButton.addActionListener(this);
		previewButton.setBounds(getInsets().left + 296,getInsets().top + 240,105,41);
		previewButton.setFont(new Font("TimesRoman", Font.PLAIN, 18));
		add(previewButton);

		channelGroup = new CheckboxGroup();
		allCheckbox = new java.awt.Checkbox("All", channelGroup, true);
		allCheckbox.addItemListener(this);
		allCheckbox.setBounds(getInsets().left + 308,getInsets().top + 73,77,36);
		allCheckbox.setFont(new Font("TimesRoman", Font.PLAIN, 18));
		add(allCheckbox);
		
		redCheckbox = new java.awt.Checkbox("Red", channelGroup, false);
		redCheckbox.addItemListener(this);
		redCheckbox.setBounds(getInsets().left + 308,getInsets().top + 108,77,36);
		redCheckbox.setFont(new Font("TimesRoman", Font.PLAIN, 18));
		redCheckbox.setForeground(new Color(16711680)); // red
		add(redCheckbox);

		greenCheckbox = new java.awt.Checkbox("Green", channelGroup, false);
		greenCheckbox.addItemListener(this);
		greenCheckbox.setBounds(getInsets().left + 308,getInsets().top + 143,77,36);
		greenCheckbox.setFont(new Font("TimesRoman", Font.PLAIN, 18));
		greenCheckbox.setForeground(new Color(65280)); //green
		add(greenCheckbox);

		blueCheckbox = new java.awt.Checkbox("Blue", channelGroup, false);
		blueCheckbox.addItemListener(this);
		blueCheckbox.setBounds(getInsets().left + 308,getInsets().top + 180,77,36);
		blueCheckbox.setFont(new Font("TimesRoman", Font.PLAIN, 18));
		blueCheckbox.setForeground(new Color(255));   // blue
		add(blueCheckbox);

		brightTextField = new java.awt.TextField();
		brightTextField.addActionListener(this);
		brightTextField.setText("0");
		brightTextField.setBounds(getInsets().left + 56,getInsets().top + 83,63,30);
		brightTextField.setBackground(Color.gray);
		add(brightTextField);

		brightScrollbar = new java.awt.Scrollbar(Scrollbar.VERTICAL,100,10,0,200);
		brightScrollbar.addAdjustmentListener(this);
		brightScrollbar.setBounds(getInsets().left + 119,getInsets().top + 83,21,30);
		add(brightScrollbar);

		brightRangeLabel = new java.awt.Label("%(-100 ... 100)");
		brightRangeLabel.setBounds(getInsets().left + 147,getInsets().top + 83,108,30);
		brightRangeLabel.setFont(new Font("TimesRoman", Font.PLAIN, 16));
		add(brightRangeLabel);

		contrastTextField = new java.awt.TextField();
		contrastTextField.addActionListener(this);
		contrastTextField.setText("0");
		contrastTextField.setBounds(getInsets().left + 56,getInsets().top + 186,63,30);
		contrastTextField.setBackground(Color.gray);
		add(contrastTextField);

		contrastScrollbar = new java.awt.Scrollbar(Scrollbar.VERTICAL,100,10,0,200);
		contrastScrollbar.addAdjustmentListener(this);
		contrastScrollbar.setBounds(getInsets().left + 119,getInsets().top + 186,21,30);
		add(contrastScrollbar);

		contrastRangeLabel = new java.awt.Label("%(-100 ... 100)");
		contrastRangeLabel.setBounds(getInsets().left + 147,getInsets().top + 186,108,30);
		contrastRangeLabel.setFont(new Font("TimesRoman", Font.PLAIN, 16));
		add(contrastRangeLabel);

		setTitle("Brightness/Contrast Control Panel");

		// get range
		contrastRangeValue = contrastScrollbar.getMaximum() -
				     contrastScrollbar.getMinimum();
		
		brightRangeValue = brightScrollbar.getMaximum() -
				   brightScrollbar.getMinimum();
		
		addWindowListener(new WindowClosedProcess());
	}

 	 public JHVImageBCControl(JHVImageCanvas imageCanvas ) {
	
		this();

		// this instance hold everything to be proceeded.
		this.imageCanvas = imageCanvas;	
		
		// update image source
		imageCanvas.updateImageSource();

		origImage = imageCanvas.getImage();

		_show();
		addWindowListener(new WindowClosedProcess());
	 }

         public JHVImageBCControl(String title) {
	        this();
		setTitle(title);
		addWindowListener(new WindowClosedProcess());
	 }

         public synchronized void _show() {
	        setLocation(50, 50);
		super.setVisible(true);
	 }

 

  public void set_Cursor(int cursor) {

	// change corsor type
	((Component) this).setCursor(new Cursor(cursor));

	// default toolkit
	Toolkit.getDefaultToolkit().sync();
    }

    public void setImageMode(int mode) {
	this.imageMode = mode;
    }
 

    public int getImageMode() {
	return this.imageMode ;
    }
 

    public void setFilterMode(int mode) {
	this.filterMode = mode;
    }
 

    public int getFilterMode() {
	return this.filterMode ;
    }
 

  public ImageFilter getImageFilter() {

	// determine filter mode
	ContrastColorFilter 	contrastFilter;
	BrightColorFilter	brightFilter;

	if (filterMode == CONTRAST) {

	   double f = (double)contrastValue/(double)contrastRangeValue;
	   f *= 2.0;
	   contrastFilter = new ContrastColorFilter(f);

	   // select channel
	   int chVal = getChannelValue();

	   if (chVal == CH_RED) 
		contrastFilter.setFilterMask(false,true,true);
  	   else if (chVal == CH_GREEN) 
		   contrastFilter.setFilterMask(true,false,true);
   		else if (chVal == CH_BLUE) 
		   	contrastFilter.setFilterMask(true,true,false);

  	   return (ImageFilter)contrastFilter;

	}

	if (filterMode == BRIGHTNESS) {

	   if (brightValue >0) { // bright
		brightFilter = new BrightColorFilter(true, brightValue);
		   
		// select channel
	   	int chVal = getChannelValue();
	   	if (chVal == CH_RED) 
		     brightFilter.setFilterMask(false,true,true);
  	   	else if (chVal == CH_GREEN) 
		   	brightFilter.setFilterMask(true,false,true);
   		     else if (chVal == CH_BLUE) 
		   	     brightFilter.setFilterMask(true,true,false);

  	        return (ImageFilter)brightFilter;

	   } else { // black

		int blackValue = 0 - brightValue;		
		brightFilter = new BrightColorFilter(false, blackValue);
		   
		// select channel
	   	int chVal = getChannelValue();
	   	if (chVal == CH_RED) 
		     brightFilter.setFilterMask(false,true,true);
  	   	else if (chVal == CH_GREEN) 
		   	brightFilter.setFilterMask(true,false,true);
   		     else if (chVal == CH_BLUE) 
		   	     brightFilter.setFilterMask(true,true,false);

  	        return (ImageFilter)brightFilter;

	  } 
	}
	return null;
  }

	void setControlChanged() {

	     controlChangedFlag = true;
	}

     public boolean controlChanged() {	

	return controlChangedFlag;	
    }

	void unsetControlChanged() {

	     controlChangedFlag = false;
	}

     public void close() {
	
	    imageCanvas.imageFrame.imageRGBControl = null;
	    // close window
	    dispose();

    }

   int getChannelValue() {  // which channel is selected?
	
	int retVal = -1;

	// getsthe current choice 
	boolean state = allCheckbox.getState();
	if (state) retVal = CH_ALL;

	state = redCheckbox.getState();
	if (state) retVal = CH_RED;

	state = greenCheckbox.getState();	
	if (state) retVal = CH_GREEN;

	state = blueCheckbox.getState();
	if (state) retVal = CH_BLUE;	
	
	return retVal;
	
   }

  public void handleButtonEvent(Button target, String arg) {

	setControlChanged();
			
	((Component)imageCanvas.imageFrame).setCursor(new Cursor(Cursor.WAIT_CURSOR));
	set_Cursor(Frame.WAIT_CURSOR);

	if ("Confirm".equals(arg)) { // Confirm
	   setImageMode(CONFIRM);
	   ImageFilter filter = getImageFilter();
	   imageCanvas.setImageRGBFilter(filter);
	   close();
	}

	if ("Preview".equals(arg)) { // Preview	   

	   ImageFilter filter = getImageFilter();
	   imageCanvas.updateImageFilter(filter);

	   setImageMode(PREVIEW);
	}

	((Component)imageCanvas.imageFrame).setCursor(new Cursor(Cursor.DEFAULT_CURSOR));
	set_Cursor(Frame.DEFAULT_CURSOR);

	if ("Cancel".equals(arg)) { // Cancel
	   // recover image
	   imageCanvas.setImage(origImage);
	   close();
	}

  }

  public void handleScrollbarEvent(Scrollbar target, Object arg) {
	
	setControlChanged();

   	// detect the bright scrollbar
      	if (target ==  brightScrollbar) {
	
		// set filter mode
		setFilterMode(BRIGHTNESS);
		int ty = brightRangeValue/2 - brightScrollbar.getValue();

                // update scrollbar value
                brightScrollbar.setValue(brightScrollbar.getValue());
		
		// set the value in related textField
		brightTextField.setText(Integer.toString(ty));

		brightValue = ty;	
        }

 	// detect the contrast scrollbar
      	if (target ==  contrastScrollbar) {
			
		// set filter mode
		setFilterMode(CONTRAST);

		int ty = contrastScrollbar.getValue();
		contrastValue = contrastScrollbar.getMaximum()-ty;

		ty = contrastRangeValue/2 - ty;
 
                // update scrollbar value
                contrastScrollbar.setValue(contrastScrollbar.getValue());
		
		// set the value in related textField
		contrastTextField.setText(Integer.toString(ty));
        }
  }

  public void handleCheckboxEvent(Checkbox target) {

  }

  public void handleTextFieldEvent(TextField target, String arg) {

	setControlChanged();

	if (target == brightTextField) {

    	   // set filter mode
           setFilterMode(BRIGHTNESS);

	   int val = Integer.parseInt(brightTextField.getText()) + brightRangeValue/2;

	   if ((val>=0) && (val<=brightRangeValue)) 
		brightScrollbar.setValue(val);

	   brightValue = Integer.parseInt(brightTextField.getText());
	}
	
	if (target == contrastTextField) {

	   // set filter mode
           setFilterMode(CONTRAST);

	   int val = Integer.parseInt(contrastTextField.getText()) + contrastRangeValue/2;

	   if ((val>=0) && (val<=contrastRangeValue)) 
		contrastScrollbar.setValue(val);

	   contrastValue = val;
	}
  }



  public void actionPerformed(ActionEvent e)
  {
    Object target = e.getSource();
    String arg = e.getActionCommand();
    
    if(target instanceof Button)
      {
 	Button button = (Button)target;
 	handleButtonEvent(button, arg);
      }
    if(target instanceof TextField)
      {
 	TextField textField = (TextField)target;
 	handleTextFieldEvent(textField, arg);
      }
    
    
  }
  

  public void itemStateChanged(ItemEvent e)
  {

    Checkbox target = (Checkbox) e.getSource();
    
    handleCheckboxEvent(target);

  }

  public void adjustmentValueChanged(AdjustmentEvent e)
  {
    int type = e.getAdjustmentType();
    Scrollbar target = (Scrollbar) e.getAdjustable();
     //  detect the scrollbar event
    switch (type) { 
    case AdjustmentEvent.UNIT_INCREMENT:
    case AdjustmentEvent.UNIT_DECREMENT:
    case AdjustmentEvent.BLOCK_INCREMENT:
    case AdjustmentEvent.BLOCK_DECREMENT:
    case AdjustmentEvent.TRACK:
      handleScrollbarEvent(target,null);
    }


  }

	static public void main(String args[]) {
	    (new JHVImageBCControl())._show();
	}

    /**
     * java.lang.Runnable stuff
     */
    public void run()  {

     while (controlThread != null) {

	if (controlChanged()) { 

	   // unset control
	   unsetControlChanged();

	   ImageFilter filter = getImageFilter();
	   imageCanvas.setImageRGBFilter(filter);

	}
	
	try {
	      Thread.sleep(10);
	} catch (Exception e) {;};

      }
    }
}
