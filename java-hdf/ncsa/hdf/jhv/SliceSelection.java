package ncsa.hdf.jhv;

import java.awt.*;
import java.awt.event.*;

import ncsa.hdf.awt.Slider;
import ncsa.hdf.awt.event.*;

// Select 3-D SDS slice value
 public class SliceSelection extends Frame
	implements ActionListener, SliderListener, ItemListener  {

	HDFTreeNode	node;

	java.awt.CheckboxGroup  sliceCkbGroup;
	java.awt.Checkbox zSliceCkb;
	java.awt.Checkbox ySliceCkb;
	java.awt.Checkbox xSliceCkb;
	// java.awt.Button showImageButton;
	// java.awt.Button cancelButton;
	java.awt.TextField sliceField;
	Slider selectedSliceSlider;
	java.awt.Label sliceValueLabel;

	SDSDimInfo	sliceInfo; 

	// which operation right now
	public 	static final int ZAXIS   = 0;
	public 	static final int YAXIS   = 1;
	public 	static final int XAXIS   = 2;

	int		activeSlice;

	// Inner class for Window closing
	class SliceSelectionClosedProcess extends WindowAdapter {
		public void windowClosing(WindowEvent evt) {
			// do something related to window closed
			dispose();
		}
	}

	public SliceSelection() {

		setLayout(null);
		addNotify();
		setSize(getInsets().left + getInsets().right + 405,getInsets().top + getInsets().bottom + 205);

		// checkbox group
		sliceCkbGroup = new java.awt.CheckboxGroup();

		// z slice
		zSliceCkb = new java.awt.Checkbox("Z Slice", sliceCkbGroup, true);
		zSliceCkb.setBounds(getInsets().left + 30,getInsets().top + 156,100,40);
		zSliceCkb.setFont(new Font("TimesRoman", Font.BOLD, 14));
		add(zSliceCkb);

		// x slice
		xSliceCkb = new java.awt.Checkbox("X Slice", sliceCkbGroup, false);
		xSliceCkb.setBounds(getInsets().left + 30,getInsets().top + 36,100,40);
		xSliceCkb.setFont(new Font("TimesRoman", Font.BOLD, 14));
		add(xSliceCkb);
	
		// y slice
		ySliceCkb = new java.awt.Checkbox("Y Slice", sliceCkbGroup, false);
		ySliceCkb.setBounds(getInsets().left + 30,getInsets().top + 96,100,40);
		ySliceCkb.setFont(new Font("TimesRoman", Font.BOLD, 14));
		add(ySliceCkb);

		// label of Selection
		sliceValueLabel = new java.awt.Label("Selection:",Label.RIGHT);
		sliceValueLabel.setBounds(getInsets().left + 134,getInsets().top + 61,84,35);
		sliceValueLabel.setFont(new Font("TimesRoman", Font.BOLD, 14));
		add(sliceValueLabel);

		// slice value
		sliceField = new java.awt.TextField();
		sliceField.setBounds(getInsets().left + 236,getInsets().top + 58,150,33);
		add(sliceField);

		// slider of slice selection
		selectedSliceSlider = new Slider();
		selectedSliceSlider.setDispMode(Slider.SLIDERVALUE);
		selectedSliceSlider.setBounds(getInsets().left + 158,getInsets().top + 131,230,34);
		add(selectedSliceSlider);
		selectedSliceSlider.setSliderRange(0d,(double)28);

		/******************* cut off ************************************
		// action button
		showImageButton = new java.awt.Button("Show Image");
		showImageButton.setBounds(getInsets().left + 53,getInsets().top + 240,122,40);
		showImageButton.setFont(new Font("TimesRoman", Font.BOLD, 14));
		add(showImageButton);

		cancelButton = new java.awt.Button("Cancel");
		cancelButton.setBounds(getInsets().left + 236,getInsets().top + 239,122,40);
		cancelButton.setFont(new Font("TimesRoman", Font.BOLD, 14));
		add(cancelButton);
		******************* cut off ************************************/

		setTitle("SDS Slice Selection");

		// item listener
		xSliceCkb.addItemListener(this);
		ySliceCkb.addItemListener(this);
		zSliceCkb.addItemListener(this);
		
		// action listener
		sliceField.addActionListener(this);
		// showImageButton.addActionListener(this);
		// cancelButton.addActionListener(this);

		// selected image listener
		selectedSliceSlider.addSliderListener(this);

		// window closed
		addWindowListener(new SliceSelectionClosedProcess() );
	}

	public SliceSelection(String title) {
	    this();
	    setTitle(title);
	}

	public SliceSelection(SDSDimInfo slice) {
	    this();
	    this.sliceInfo = slice;
	}

	public SliceSelection(HDFTreeNode node, SDSDimInfo slice) {
	    this();
	    this.sliceInfo = slice;
	    this.node = node;
	
	    // update z-axis range
	    int aSlice = slice.getActiveSlice();
	    selectedSliceSlider.setSliderRange(0,(double)slice.getDim(aSlice)-1);  
	}

    public synchronized void setVisible() {
    	setLocation(50, 50);
    	super.setVisible(true);
    }

	static public void main(String args[]) {
	    (new SliceSelection()).setVisible();
	}

    /**
     * SliderListener stuff
     */
    public void sliderStateChanged(SliderEvent evt)  {

	Object target = evt.getSource();
	if (target instanceof Slider)
	   handleSliderEvent((Slider)target);
    }

    /**
     * java.awt.event.ActionListener stuff
     */
    public void actionPerformed(ActionEvent evt)  {
		
	Object target = evt.getSource();
    	String arg = evt.getActionCommand();
    
	if(target instanceof Button) {
 		Button button = (Button)target;
 		handleButtonEvent(button, arg);
      	}
    	if(target instanceof TextField) {
 		TextField textField = (TextField)target;
 		handleTextFieldEvent(textField, arg);
      	}

    }

    /**
     * java.awt.event.ItemListener stuff
     */
    public void itemStateChanged(ItemEvent e)  {
	   
	Object target =  e.getSource(); 
	if (target instanceof Checkbox)
   	   handleCheckboxEvent((Checkbox)target);
    }

 // update image by slice number
 public void handleTextFieldEvent(TextField target, String arg) {

	if(target instanceof TextField) {
   
 		TextField textFld = (TextField)target;
 		// System.out.println(textFld);
		String sliceStr = (String)textFld.getText();
		int selection = Integer.parseInt(sliceStr);
		selectedSliceSlider.setSliderValue((double)selection);
			   
		// update slice info
	   	sliceInfo.setSelectedValue(getSelectedSlice(), selection);
		
		// update image
		node.getHdfSDSimage(getSliceInfo());
      	}  
 }

 public void handleButtonEvent(Button target, String arg) {

	// System.out.println(target);
	if ("Show Image".equals(arg)) {
		node.getHdfSDSimage(getSliceInfo());
	}
 }

  public void handleCheckboxEvent(Checkbox target) {

	int aSlice = getSelectedSlice();
	// update active slice
	for (int kk=0; kk<sliceInfo.getRank(); kk++) 
	    if (kk==aSlice)
		sliceInfo.setState(kk,true);
	    else
		sliceInfo.setState(kk,false);

	// update slice range
	selectedSliceSlider.setSliderRange(0,(double)sliceInfo.getDim(aSlice)-1);

	// modify the selected value
	int selection = sliceInfo.getSelectedValue(aSlice);
	// txtfield
	sliceField.setText(Integer.toString(selection));
	// slider
	selectedSliceSlider.setSliderValue((double)selection);
	
	// update image since slice info. has changed
 	node.getHdfSDSimage(getSliceInfo());
  }

  public void handleSliderEvent(Slider target) {

	// System.out.println(target);
	// update slice selected value
	if (selectedSliceSlider.sliderChanged()) {
	   int selection = (int)Math.rint((double)selectedSliceSlider.getSliderValue());
	   // System.out.println("Slice: " + getSelectedSlice() + ", " + selection);
	   sliceField.setText(Integer.toString(selection));
	   
	   // update slice info
	   sliceInfo.setSelectedValue(getSelectedSlice(), selection);

	   // update image since slider has been changed 
	   node.getHdfSDSimage(getSliceInfo());
	} 
  }

 // get selected slice
 int getSelectedSlice() {  // which slice is selected?
	
	int retVal = -1;

	// gets the current choice 
	boolean state = xSliceCkb.getState();
	if (state) retVal = XAXIS;

	state = ySliceCkb.getState();
	if (state) retVal = YAXIS;

	state = zSliceCkb.getState();	
	if (state) retVal = ZAXIS;
	
	return retVal;	
   }

 public SDSDimInfo getSliceInfo() {   
	
	return this.sliceInfo;	
   }
}
