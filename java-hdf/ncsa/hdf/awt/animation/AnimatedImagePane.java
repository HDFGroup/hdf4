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
import ncsa.hdf.awt.event.SliderListener;

import ncsa.hdf.awt.palette.*;
import ncsa.hdf.awt.image.*;

/**
 *  This class handles hot to display animated image
 * @version 1.00  9/3/97
 * @auther  Xinjian Lu  		  
 */
public class AnimatedImagePane extends Canvas 
	implements Runnable, PaletteEditorAdapter {

	private Hashtable animatedImages;

	Dimension	panelSize = new Dimension(360,360);

 	// variables for double-buffer   
  	Image 		offScreenImage = null;
  	Graphics 	offGraphics;

	// current displayed image info.
	AnimatedImageInfo		activeImageInfo;
	
	// animated image info.
	AnimatedImageInfo		animatedImagesInfo[];

	// some of constants to define animation mode
	public static final int SINGLE	= 1;
	public static final int LOOP	= 0;
	public static final int BOUNCE	= 2;

	int		animatedMode = LOOP;

	// for animation
	public static final int 	BACKWARD = 1;
	public static final int 	FOREWARD = 2;

	// direction
	int	animatedDirection = BACKWARD;
	// speed
	int	animatedSpeed = 100; 

	// current active image number
	int	dispNumber = 0;

	Thread	killer = null;

	// slider for image frame count
	Slider	imageFrameSlider = new Slider();

	// variables to palette editor
 	// for image palette
  	public boolean 	paletteEditorOpen	= false;
 
	// for image color model
  	public ColorModel  	colorModel    = null;
  
	// PaletteEditor instance ....
  	PaletteEditor paletteEditor;

    // Constructs an AnimatedImagePane with default.
    public AnimatedImagePane() {
	animatedImages = new Hashtable();
	setSize(panelSize);
    }

    // Add an image with an animatedimageinfo and the index of the info.
    protected void addImage(AnimatedImageInfo imageInfo, int index) {
	
	if (imageInfo != null) {
	   int order = animatedImages.size();  // set current image layer

	   imageInfo.setOrder(order);
	   
	   animatedImages.put(Integer.toString(index), imageInfo);	
	   
	   // resize panel
	   if (panelSize.width<imageInfo.getWidth()) {
		panelSize.setSize(imageInfo.getWidth(), panelSize.width);
		setSize(panelSize);
	   }
	   if (panelSize.height<imageInfo.getHeight()) {
		panelSize.setSize(panelSize.width, imageInfo.getHeight());
	        setSize(panelSize);
	   }

	}
    }

 // return image number
 public int getImageNumber() {
 
	// number of image to be displayed for animation
	int imagesCount = animatedImages.size();

	return (imagesCount);	
 }

    // Remove an image with specified layer
    protected void removeImage(int index) {
 		
	int order = getOrder(index);  // image order (frame)
	if (order == -1) 
	   return;

	animatedImages.remove(Integer.toString(index));
	
	if (!animatedImages.isEmpty()) { // not empty
	   // should adjust frame (order)
	   for ( Enumeration keys = animatedImages.keys();
	  	 keys.hasMoreElements(); ) {
		String ind = (String)keys.nextElement();
		updateOrder(Integer.parseInt(ind),order);		
	   }
	}	
    }

  

   /** 
     * Returns the preferred size of panel.
     */
    public Dimension getPreferredSize() {
	return getMinimumSize();
    }

    /**
     * Returns the mininimum size of this component.
     */
    public Dimension getMinimumSize() {
      	return (panelSize);
    }

    // Sets the new order to an AnimatedImageInfo instance
    protected void setOrder(AnimatedImageInfo imageInfo, int order) {
	if (imageInfo != null) {
	   imageInfo.setOrder(order);
	}
    } 

    // Sets a new order for the AnimatedImageInfo instance, which has a specifial
    // index in animatedImages. 
    protected void setOrder(int index, int order) {
	if (animatedImages.containsKey(Integer.toString(index))) {
	   AnimatedImageInfo info = 
		(AnimatedImageInfo)(animatedImages.get(Integer.toString(index)));
	   setOrder(info,order);
	}
    } 

   // Gets an image display order for an AnimatedImageInfo, whose index is a 
   //  specified  index in Hashtabe variable animatedImages.
   public int getOrder(int index) {

	AnimatedImageInfo info;
	// Tests if the specified object is a key in animatedImages
	if (animatedImages.containsKey(Integer.toString(index))) {
	   info = (AnimatedImageInfo)animatedImages.get(Integer.toString(index));
	   return (info.getOrder());
	}
	return -1;
    }

  

 // display image by order number(frame number)
 public void setActiveImage(int order) {

	// set current display image
	this.dispNumber = order;

	for ( Enumeration imageIndexs = animatedImages.keys();
			  imageIndexs.hasMoreElements(); ) {

		// image index
		int index = Integer.parseInt((String)(imageIndexs.nextElement()));

		AnimatedImageInfo info = 
			(AnimatedImageInfo)getAnimatedImageInfo(index);
		if (info != null) {
		    int number         = info.getOrder();
		    if (number == order)
			setActiveImageInfo(info);		    
		}
	}
 }

 

    // returns the display order from the specified AnimatedImageInfo .
    public int getOrder(AnimatedImageInfo imageInfo) {
	return (imageInfo.getOrder());
    }

     // Sets a new AnimatedImageInfo whith a specified value
     public void setActiveImageInfo(AnimatedImageInfo  imageInfo) {
	this.activeImageInfo = imageInfo;
	repaint();
    } 

    // Sets new animated images information.
    public void setImagesInfo(AnimatedImageInfo  imageInfo[]) {
	this.animatedImagesInfo = imageInfo;
    } 

    // Gets current display image information.
    public AnimatedImageInfo  getActiveImageInfo() {
	return (this.activeImageInfo);
    } 

   // Gets display image information with specified index value from animatedImages.
   public Object getAnimatedImageInfo(int index) {

	AnimatedImageInfo info;
	// Tests if the specified object is a key in iamgeLayers
	if (animatedImages.containsKey(Integer.toString(index))) {
	   info = (AnimatedImageInfo)animatedImages.get(Integer.toString(index));
	   return (Object)(info);
	}
	return null;
    }

  // Update display image order.
  protected void updateOrder(int index, int order) {

	if (animatedImages.containsKey(Integer.toString(index))) {
	   AnimatedImageInfo info = 
		(AnimatedImageInfo)(animatedImages.get(Integer.toString(index)));

	   if (info.getOrder()>order) 
		info.setOrder(info.getOrder()-1);	   
	}
    } 

    // Update the image filter for the AnimatedImageInfo instance, which is from
    //  animatedImages with a specified index value
    protected void updateOrderFilter(int index, ImageFilter filter) {

	if (animatedImages.containsKey(Integer.toString(index))) {

	  AnimatedImageInfo info = 
		(AnimatedImageInfo)(animatedImages.get(Integer.toString(index)));

	   info.setFilter(filter);	   
	}

	repaint();
    } 

    // Update the current image's filter
    protected void updateActiveImageFilter(ImageFilter filter) {

	  AnimatedImageInfo info = 
		(AnimatedImageInfo)(getActiveImageInfo());
	  info.setFilter(filter);	   

	  repaint();
    } 

      // Update image's filter with the specified color model
      protected void updateActiveImageFilter(ColorModel cm) {

	  ColorModelFilter cmFilter = new ColorModelFilter(cm);
	  updateActiveImageFilter(cmFilter);
      } 

 // the interval of image animation
 private void delay(long n) {

   try {
	Thread.sleep(n);
   } catch (Exception e) {};

 }

    // Sets the mode of animation
    public void  setAnimatedMode(int mode)  {

	this.animatedMode = mode;
    }

    // Gets the animation mode 
    public int getAnimatedMode()  {
	return (this.animatedMode);
    }

  

 // Sets the current display image number
 public void setDispImageNumber(int order) {

	// set current display image
	this.dispNumber = order;
 }

 

 // Gets the current display image number
 public int getDispImageNumber() {
	return (this.dispNumber);
 }

  // Sets the image display speed
  public void setAnimatedInterval(int interval) {
  	this.animatedSpeed = interval;
  }

  // Sets a new slider to indicate the current image number
  public void setFrameSlider(Slider slider) {
  	this.imageFrameSlider = slider;
  }

 /**
  * Updates the component.
  */
 public void update(Graphics g) { 
	
    Dimension d = getSize();
   
    if (offScreenImage == null) {
      // offScreenImage not created; create it.
      offScreenImage = createImage(d.width , d.height );	
      
      // get the off-screen graphics context    
      offGraphics    = offScreenImage.getGraphics();
      
      // set the font for offGraphics
      offGraphics.setFont(getFont());	 
    }
    
    // paint the background on the off-screen graphics context
    offGraphics.setColor(getBackground());
    offGraphics.fillRect(0,0,d.width,d.height);    
    offGraphics.setColor(getForeground());
    
    // draw the current frame to the off-screen 
    paint(offGraphics);
    
    //then draw the image to the on-screen 
    g.drawImage(offScreenImage, 0, 0, null);
 }

    /**
     * Paints the component.
     */
    public void paint(Graphics g) {
	 
	// active image info.
	AnimatedImageInfo	imgInfo = getActiveImageInfo();
	if (imgInfo == null) return;

    	// get current Canvas size
    	int w = getSize().width;
    	int h = getSize().height;

    	// If the image size is greater than the Canvas make the image shrink
    	int width = imgInfo.getWidth();
    	int height= imgInfo.getHeight();
   
    	int startx =  (width>w)  ? 0 : (w-width)/2; 
    	int starty =  (height>h) ? 0 : (h-height)/2;
   
    	if ( imgInfo.getImage() == null) {
      	   super.paint(g);
     
      	   // info
	   String  infoStr = "No Active Image Found";
      	   g.setColor(Color.black);
      	   FontMetrics fm = g.getFontMetrics();
      	   int x = (w - fm.stringWidth(infoStr))/2;
      	   int y = h/2;
      	   g.drawString(infoStr, x, y); 

    	} else {
 	
      	   // image
      	   g.drawImage(imgInfo.getImage(), startx, starty,width, height, this);  
        }
   }

    /**
     * ncsa.hdf.palette.PaletteEditorAdapter stuff
     */
    public void setPaletteEditorOpen(boolean flag)  {
  	this.paletteEditorOpen = flag;
    }

    /**
     * ncsa.hdf.palette.PaletteEditorAdapter stuff
     */
    public void updateColorModel(ColorModel cm)  {

      this.colorModel = cm;
      // image source of current active image
      ImageProducer	imageSource = getActiveImageInfo().getImageSource();

      if (cm != null)  {
	// new color model
	updateActiveImageFilter(cm); 

	repaint();
      }		
    }

    /**
     * ncsa.hdf.palette.PaletteEditorAdapter stuff
     */
    public void setImage(Image img)  {
	
	getActiveImageInfo().setImage(img);
	repaint();
    }

    /**
     * ncsa.hdf.palette.PaletteEditorAdapter stuff
     */
    public Image getImage()  {

	return (getActiveImageInfo().getImage());
    }

  

  /**
   * java.lang.java.lang.Runnable stuff
   */
  public void run()  {

    int currentPlaneNumber = dispNumber;  
    int count = 0;
    while (killer != null ) {

      // "Forward" action
      if (animatedDirection == FOREWARD) {

	switch (animatedMode) {
	case SINGLE:  // single animation
		++count;
		break;
	case BOUNCE:
		if (currentPlaneNumber >= (getImageNumber()-1)) {
			animatedDirection = BACKWARD;
  			// wait a minute
	  		delay(animatedSpeed-90);
			continue;
		}
		break;
	}

	++currentPlaneNumber;

	if (currentPlaneNumber >= getImageNumber())
	  currentPlaneNumber = 0;
	
	try {
	  // get previous image
	   setActiveImage(currentPlaneNumber);

	   // set frame number
	  imageFrameSlider.setSliderValue((double)currentPlaneNumber);

	  // wait a minute
	  Thread.sleep(animatedSpeed-90);
		  
	} catch (Exception e) {};
      } 

      // "Backward action"
      if (animatedDirection == BACKWARD) {

	switch (animatedMode) {
	case SINGLE:  // single animation
		++count;
		break;
	case BOUNCE:
		if (currentPlaneNumber <=0 ) {
			animatedDirection = FOREWARD;
			continue;
		}
		break;
	}

	--currentPlaneNumber;
	if (currentPlaneNumber < 0)
	  currentPlaneNumber = getImageNumber()-1;
	
	try {
	  // get previous image
	  setActiveImage(currentPlaneNumber);
   
	  // set frame number
	  imageFrameSlider.setSliderValue((double)currentPlaneNumber);

	  // wait a minute
	  Thread.sleep(animatedSpeed-90);
	  
	} catch (Exception e) {};
      } // if backward ...	

      if (count > getImageNumber())
	 break;
	
    } // while (killer != null)
  }

  // Starts a new Thread
  public void start() {
    // do stop first
    stop();

    // new thread
    if (killer == null) {    
      killer = new Thread(this);
      killer.start();
    }
  }

 // Kill one thread
 public void stop() {

    if (killer != null) 
      killer.stop();
    killer = null;
  }

 // make image animation by specified operation(direction)
 public void makeAnimation(int action) {

	// animation action
	this.animatedDirection = action;
 
	// number of image to be displayed for animation
	int imagesCount = animatedImages.size();

	AnimatedImageInfo imagesInfo[] = new AnimatedImageInfo[imagesCount];

	for ( Enumeration imageIndexs = animatedImages.keys();
			  imageIndexs.hasMoreElements(); ) {

		// image index
		int index = Integer.parseInt((String)(imageIndexs.nextElement()));

		AnimatedImageInfo info = 
			(AnimatedImageInfo)getAnimatedImageInfo(index);
		if (info != null) {
		    int number         = info.getOrder();
		    imagesInfo[number] = info;		    
		}
	}

	setImagesInfo(imagesInfo);
 
	// new thread, then animates
	start();
 }

  // Assigns a new palette value to the image
  public void openNewPalette() {

	if (!paletteEditorOpen) {

		paletteEditorOpen = true;

		paletteEditor = new PaletteEditor(this);

		// set default palette value (array of rgb...)
		// paletteEditor.setColorModel(colorModelPalette);
		
		paletteEditor.show();
	}
    }

   // set a new filter for each of images
   public void setNewPalette() {

	if (paletteEditorOpen) {
		byte[] pal = paletteEditor.currentPaletteValue();		
		ColorModel  cm = new ImageColorModel(pal).getColorModel();
		// update image (or all images)
		// color model filter
  		ColorModelFilter cmFilter = new ColorModelFilter(cm);

		// number of image to be displayed for animation
		int imagesCount = animatedImages.size();

		AnimatedImageInfo imagesInfo[] = new AnimatedImageInfo[imagesCount];
		for ( Enumeration imageIndexs = animatedImages.keys();
			  	imageIndexs.hasMoreElements(); ) {

		    // image index
		    int index = Integer.parseInt((String)(imageIndexs.nextElement()));
		    AnimatedImageInfo info = 
			(AnimatedImageInfo)getAnimatedImageInfo(index);
		    if (info != null) {
		       int number         = info.getOrder();
		       imagesInfo[number] = info;		    
		   }
		}
		// current image info.
		setImagesInfo(imagesInfo);

		for (int kk=0; kk<getImageNumber(); kk++) {
			// for each of images
			imagesInfo[kk].setFilter(cmFilter);		
		}
	} 
    }
}
