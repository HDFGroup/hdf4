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


package ncsa.hdf.awt.animation;

import java.awt.*;
import java.awt.event.*;
import java.awt.image.*;
import java.lang.String;
import java.lang.Thread;
import java.util.*;

import ncsa.hdf.awt.Slider;
import ncsa.hdf.awt.event.*;

/**
 *  This main class handles an general image animation.
 *  @version 1.00 	08/25/97
 *  @author Xinjian LU
 */
public class ImageAnimation  extends Frame 
				implements ActionListener, ItemListener , 
					   SliderListener {
	ScrollPane	animatedPane;
	java.awt.Label 	animatedImageLabel;
	java.awt.List 	imageList;
	java.awt.Button unselectAllButton;
	java.awt.Button dismissButton;
	java.awt.Button selectAllButton;
	java.awt.Label 	animatedOperationLabel;

	Choice		animatedModeChoice;

	Button		paletteApply;
	Button		paletteUpdate;

	Slider		selectedImageSlider;
	Slider		animatedSpeed;
	java.awt.Button backwardButton;
	java.awt.Button previousButton;
	java.awt.Button stopButton;
	java.awt.Button nextButton;
	java.awt.Button forewardButton;
	java.awt.Label 	animatedImageListLabel;

	// a panel to handle multiple-layer image display
	AnimatedImagePane	animatedImagePane;

	// animated image to be displayed
	AnimatedImageInfo	imagesInfo[];

	int			selectImage = 0;    // current select index
	int			selected[] = new int[0];

	// Inner class for Window closing
	class AnimatedWindowClosedProcess extends WindowAdapter {
		public void windowClosing(WindowEvent evt) {
			// do something related to window closed
			dispose();
		}
	}

	// initial user interface & some of variables
	public ImageAnimation() {

		setLayout(null);
		addNotify();
		setSize(getInsets().left + getInsets().right + 736,
		        getInsets().top + getInsets().bottom + 454);

		// amimated image label
		animatedImageLabel = new java.awt.Label("Animated Image:");
		animatedImageLabel.setBounds(getInsets().left + 12,
					     getInsets().top + 12,144,24);
		animatedImageLabel.setFont(new Font("TimesRoman", Font.BOLD, 16));
		// animatedImageLabel.setBackground(new Color(12632256));
		add(animatedImageLabel);

		// animated images panel
		animatedPane = new java.awt.ScrollPane();
		animatedPane.setBounds(getInsets().left + 12,
				       getInsets().top + 48,360,384);
		add(animatedPane);

		// animated image list label	
		animatedImageListLabel = new java.awt.Label("Animated Image List:");
		animatedImageListLabel.setBounds(getInsets().left + 396,
						 getInsets().top + 12,180,24);
		animatedImageListLabel.setFont(new Font("TimesRoman", Font.BOLD, 16));
		add(animatedImageListLabel);
	
		// animated images list
		imageList = new java.awt.List(0,true); 
		add(imageList);
		imageList.setBounds(getInsets().left + 408,
				    getInsets().top + 48,300,180);

		// select all buttons		
		selectAllButton = new java.awt.Button("Select All");
		selectAllButton.setBounds(getInsets().left + 408,
					  getInsets().top + 248,74,32);
		selectAllButton.setFont(new Font("TimesRoman", Font.BOLD, 12));
		add(selectAllButton);

		// unselect all button
		unselectAllButton = new java.awt.Button("Unselect All");
		unselectAllButton.setBounds(getInsets().left + 524,
					    getInsets().top + 248,74,32);
		unselectAllButton.setFont(new Font("TimesRoman", Font.BOLD, 12));
		add(unselectAllButton);

		// dismiss button
		dismissButton = new java.awt.Button("Dismiss");
		dismissButton.setBounds(getInsets().left + 628,
					getInsets().top + 248,74,32);
		dismissButton.setFont(new Font("TimesRoman", Font.BOLD, 12));
		add(dismissButton);
	
		// animated operation label	
		animatedOperationLabel = new java.awt.Label("Animated Operations:");
		animatedOperationLabel.setBounds(getInsets().left + 408,
						 getInsets().top + 298,108,36);
		animatedOperationLabel.setFont(new Font("TimesRoman", Font.PLAIN, 12));
		add(animatedOperationLabel);
	
		// animated operation choices	
		animatedModeChoice = new Choice();
		animatedModeChoice.setBounds(getInsets().left + 526,
						  getInsets().top + 306,70,30);
		animatedModeChoice.setFont(new Font("TimesRoman", Font.BOLD, 12));
		add(animatedModeChoice);

		// add animation mode
		animatedModeChoice.add("Loop  ");
		animatedModeChoice.add("Single");
		animatedModeChoice.add("Bounce");
	
		// set palette
		paletteApply = new Button("Palette");
		paletteApply.setBounds( getInsets().left+ 600,
					getInsets().top + 306,50,24);
		paletteApply.setFont(new Font("TimesRoman", Font.PLAIN, 12));
		add(paletteApply);
	
		// update palette
		paletteUpdate = new Button("Update");
		paletteUpdate.setBounds( getInsets().left+ 650,
					getInsets().top + 306,50,24);
		paletteUpdate.setFont(new Font("TimesRoman", Font.PLAIN, 12));
		add(paletteUpdate);

		// animated operation button
		// backward
		backwardButton = new java.awt.Button("Backward");
		backwardButton.setBounds(getInsets().left + 408,
					 getInsets().top + 336,52,32);
		backwardButton.setFont(new Font("TimesRoman", Font.BOLD, 10));
		add(backwardButton);

		// previous
		previousButton = new java.awt.Button("Previous");
		previousButton.setBounds(getInsets().left + 468,
					 getInsets().top + 336,52,32);
		previousButton.setFont(new Font("TimesRoman", Font.BOLD, 10));
		add(previousButton);

		// stop
		stopButton = new java.awt.Button("Stop");
		stopButton.setBounds(getInsets().left + 528,
				     getInsets().top + 336,52,32);
		stopButton.setFont(new Font("TimesRoman", Font.BOLD, 10));
		add(stopButton);

		// next
		nextButton = new java.awt.Button("Next");
		nextButton.setBounds(getInsets().left + 588,
				     getInsets().top + 336,52,32);
		nextButton.setFont(new Font("TimesRoman", Font.BOLD, 10));
		add(nextButton);

		// foreward
		forewardButton = new java.awt.Button("Foreward");
		forewardButton.setBounds(getInsets().left + 648,
					 getInsets().top + 336,52,32);
		forewardButton.setFont(new Font("TimesRoman", Font.BOLD, 10));
		add(forewardButton);
	
		// selected image frame number:
		Label animatedImageFrameLabel = new java.awt.Label("Frame(s):");
		animatedImageFrameLabel.setBounds(getInsets().left + 408,
						  getInsets().top + 380,80,24);
		animatedImageFrameLabel.setFont(new Font("TimesRoman", Font.BOLD, 12));
		add(animatedImageFrameLabel);

		// select image slider	
		selectedImageSlider = new Slider();
		selectedImageSlider.setBounds(getInsets().left + 500,
					      getInsets().top + 380,200,20);
		add(selectedImageSlider);
		selectedImageSlider.setDispMode(Slider.SLIDERVALUE);

		// animated images speed
		Label animatedImageSpeedLabel = new java.awt.Label("Speeds of 1/1000th (s):");
		animatedImageSpeedLabel.setBounds(getInsets().left + 408,
						  getInsets().top + 408,120,24);
		animatedImageSpeedLabel.setFont(new Font("TimesRoman", Font.BOLD, 12));
		add(animatedImageSpeedLabel);
			
		// speed slider
		animatedSpeed = new Slider();
		animatedSpeed.setBounds(getInsets().left + 540,
					getInsets().top + 408,160,20);
		add(animatedSpeed);
		animatedSpeed.setDispMode(Slider.SLIDERVALUE);
		animatedSpeed.setSliderRange(91d, 1000d);

		setTitle("Image Animation");
		setResizable(false);

		// listener of button
		selectAllButton.addActionListener(this);
		unselectAllButton.addActionListener(this);
		dismissButton.addActionListener(this);

		// animation
		previousButton.addActionListener(this);
		backwardButton.addActionListener(this);
		stopButton.addActionListener(this);
		nextButton.addActionListener(this);
		forewardButton.addActionListener(this);
	
		// palette apply
		paletteApply.addActionListener(this);
		paletteUpdate.addActionListener(this);

		// animated mode choice
		animatedModeChoice.addItemListener(this);

		// image list selection
		imageList.addItemListener(this);

		// speed listener
		animatedSpeed.addSliderListener(this);
		// selected image listener
		selectedImageSlider.addSliderListener(this);

		// window closed
		addWindowListener(new AnimatedWindowClosedProcess() );

		// scrollPane
		animatedImagePane = new AnimatedImagePane();
		animatedPane.add(animatedImagePane, this, 0);
		
		// set component visable
		showComponent();
	}

	// Construct new ImageAnimation with specified title
	public ImageAnimation(String title) {
	    this();
	    setTitle(title);
	}

    // Shows animation window
    public void showComponent() {

    	setLocation(50, 50);
	invalidate();

	// force not ot resize component
	setResizable(false);
	pack();
    	setVisible(true);
    }

    // set animated image info.
    public void setAnimatedImage(AnimatedImageInfo imagesInfo[]) {
    	
	// image info
	this.imagesInfo = imagesInfo;
	imageList.removeAll();
	// add to list
	for (int kk=0; kk<imagesInfo.length; kk++) 
		imageList.add(imagesInfo[kk].getName());

	if (imagesInfo.length>0) {
		// default selection
		imageList.select(0);
		// create new ItemEvent
		ItemEvent evt = new ItemEvent(  imageList, ItemEvent.ITEM_FIRST,
						imageList.getSelectedItem(), 
						ItemEvent.SELECTED);
		// fire it up
		imageList.dispatchEvent(evt);
	}
    }

	// update image frame number
	public void updateFrameNumber() {
	   	// set frames number
		selectedImageSlider.setSliderRange(0d,(double)(selected.length-1));
	}

	// return current selected image  
	public int getImageIndex() {
	   return (int)selectImage;
	}

    /**
     * Set the cursor image to a predefined cursor.
     * @param cursorType one of the cursor constants defined above.
     */
   public void setCursor(Cursor cursor) {

	// change corsor type
	super.setCursor(cursor);

	// default toolkit
	Toolkit.getDefaultToolkit().sync();
    }

 

   // Sets an image animation mode
   public void setAnimatedMode(int mode) {

	animatedImagePane.setAnimatedMode(mode);
    }

 

    // Gets an image animation mode
    public int getAnimatedMode() {

	return (animatedImagePane.getAnimatedMode());
    }

	// update next action
	public void dispNextImage() {
		
		int dispImageNumber = animatedImagePane.getDispImageNumber();
		++dispImageNumber;

	   	 // update current display frame
		 if (dispImageNumber >= selected.length)
		    dispImageNumber = 0;
	
		 selectedImageSlider.setSliderValue((double)dispImageNumber);
		 // display image  
		 animatedImagePane.setActiveImage(dispImageNumber);
	}

 

	// update previous action
	public void dispPrevImage() {
   
		int dispImageNumber = animatedImagePane.getDispImageNumber();
		 --dispImageNumber;
	   	 // update current display frame
		 if (dispImageNumber < 0)
		    dispImageNumber = selected.length-1;

		 selectedImageSlider.setSliderValue((double)dispImageNumber);
		 // display image  
		 animatedImagePane.setActiveImage(dispImageNumber);
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
    }

  /** button event handling 
   * @param target an  instance of a button
   * @param arg title of a button
   */
  public void handleButtonEvent(Button target, String arg) {
			
	// setCursor(new Cursor(Cursor.WAIT_CURSOR));
	if ("Select All".equals(arg)) {
		// number of image
		int imgNumber = imageList.getItemCount();
		for (int kk=0; kk<imgNumber; kk++) {
		    // select  for each items
		    if (!imageList.isIndexSelected(kk)) {
		    	imageList.select(kk);
	      	    	animatedImagePane.addImage(imagesInfo[kk], kk);
		    }   
		}

		// current selected items
		selected = imageList.getSelectedIndexes();
		updateFrameNumber();

		if (imgNumber>=0)
		   animatedImagePane.setActiveImageInfo(imagesInfo[0]);
	}
	else {
	  // setCursor(new Cursor(Cursor.WAIT_CURSOR));
	  if ("Unselect All".equals(arg)) {
		// number of image
		int imgNumber = imageList.getItemCount();
		for (int kk=0; kk<imgNumber; kk++) {
		    // deselect  for each items
		    if (imageList.isIndexSelected(kk)) {
		    	imageList.deselect(kk);
		    	animatedImagePane.removeImage(kk); 
		    }  
		}  
		// reset current selection indexs
		selected = new int[0]; 	
		updateFrameNumber();

		animatedImagePane.setActiveImageInfo(null);
	  }
	}

	if ("Backward".equals(arg)) { 
		animatedImagePane.makeAnimation(animatedImagePane.BACKWARD);
		animatedImagePane.setFrameSlider(selectedImageSlider);
	} else  
	  if ("Foreward".equals(arg)) {
 		animatedImagePane.setFrameSlider(selectedImageSlider);
		animatedImagePane.makeAnimation(animatedImagePane.FOREWARD);
	  } else 
	    if ("Stop".equals(arg)) {
		animatedImagePane.stop();
	    } else 	
	      if ("Next".equals(arg)) {
		 dispNextImage();
	      } else {	
		if ("Previous".equals(arg)) {
		   dispPrevImage();
		}
	      } 

	if ("Dismiss".equals(arg)) {
 		dispose();
	}

	if ("Palette".equals(arg)) { // palette editor for current image
 
	   // do sth. about image palette here
	   animatedImagePane.openNewPalette();
	}	
	else if ("Update".equals(arg)) { // update pallete
 
	   // do sth. about image palette here
	   animatedImagePane.setNewPalette();
	}
  }

    /**
     * java.awt.event.ItemListener stuff
     */
    public void itemStateChanged(ItemEvent e)  {

     Object target = e.getSource();
     if ( target instanceof List) {
	// current select state
	boolean selectState = true; 

	// current selected items
	int[] selects = imageList.getSelectedIndexes();

	int selectedIndex   = -1;
	int deselectedIndex = -1;

	if (selects.length > selected.length){ // new item selected

	   selectState = true;
 
	   //  find new selected item
	   for (int i=0; i<selects.length; i++) {
		boolean found = false;
		for (int j=0; j<selected.length; j++)  
		    if (selects[i] == selected[j])
			found = true;
		if (!found) {
		   selectedIndex = selects[i];
		   break;
		}
	   }
	}
	else {   // which item has been un-selected

	   selectState = false;

	   for (int i=0; i<selected.length; i++) {
		boolean found = false;
		for (int j=0; j<selects.length; j++)  
		    if (selected[i] == selects[j])
			found = true;
		if (!found) {
		   deselectedIndex = selected[i];
		   break;
		}
	   }
	}

	// confirm selected item 
	if (!selectState) { // deselect state
	   animatedImagePane.removeImage(deselectedIndex);   
	} else {
	   if (selectedIndex < imagesInfo.length)
	      animatedImagePane.addImage(imagesInfo[selectedIndex], selectedIndex);   
	}
	
	// current select index
	selectImage = selectedIndex;

	// modify the selectedimageLabel
	String lab = "Active Order: ";
	if (selectImage>=0) 
	   lab = "Active " + imagesInfo[selectImage].getOrderInfo();

	if (selectImage>=0) 
	   animatedImagePane.setActiveImageInfo(imagesInfo[selectImage]);
	else
	   animatedImagePane.setActiveImageInfo(null);

	// keep current selected items
	selected = selects;
	
	// update frames
	updateFrameNumber();
      } 
      if (target instanceof Choice ) {
	 setAnimatedMode(animatedModeChoice.getSelectedIndex());
      }
    }

   

    /**
     * SliderListener stuff
     */
    public void sliderStateChanged(SliderEvent evt)  { 
		
	// update animated interval
	if (animatedSpeed.sliderChanged()) {
	   animatedImagePane.setAnimatedInterval((int)animatedSpeed.getSliderValue());
	} 
	if (selectedImageSlider.sliderChanged())  {
	   int sp = (int)java.lang.Math.round(selectedImageSlider.getSliderValue());
	   animatedImagePane.setActiveImage(sp);
	}
    }

   // event processing
   public void processEvent(AWTEvent event) {
    	if (event.getID() == Event.WINDOW_DESTROY) {
    	    dispose();
    	}
   }

   // closing window
   public void dispose() {
    	animatedImagePane.stop();

	if (animatedImagePane.paletteEditor != null)
	    animatedImagePane.paletteEditor.selectedExit();

   	super.dispose();      // free the system resources
   }
}
