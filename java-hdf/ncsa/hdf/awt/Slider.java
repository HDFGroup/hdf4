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
package ncsa.hdf.awt;

import ncsa.hdf.awt.event.*;
import java.awt.*;

import java.awt.event.*;
import java.util.Vector;
import java.lang.Math;

/**
 * A Slider is a widget that varies between a minimum and a maximum
 * value. Draging on a "thumb" can change the current value. 
 * @author Xinjian Lu (xlu@ncsa.uiuc.edu) HDF Group, NCSA
 * @version 1.00
 */
public class Slider extends Canvas implements MouseListener, 
  MouseMotionListener {

    private final static int THUMB_SIZE = 14;
    private final static int BUFFER = 2;

    private final static int DEFAULT_WIDTH = 100;
    private final static int DEFAULT_HEIGHT = 15;

    private final static int MIN_WIDTH = 2 * (THUMB_SIZE + BUFFER + 1);
    private final static int MIN_HEIGHT = 2 * (BUFFER + 1);

    private final static int DEFAULT_MIN = 0;
    private final static int DEFAULT_MAX = 100;  
  
    public final static int PERCENTAGE = 1;
    public final static int SLIDERVALUE= 2;  
  
    int		base = 1;
    
    int		maxValue;
    int		minValue;

    Color bgColor,  thumbColor, barColor, slashColor;
  
    /** the slide value */
    int value;
    
    // percent of the slider
    double 	percent;

    // the value that slider understand...
    double 	sliderMaxValue;
    double 	sliderMinValue;

    int minThumbPos, maxThumbPos;
    int thumbPos;
    
    // for offscreen buffer
    	
    /** variables for duble-buffer  */
    Image 	offScreenImage = null;
    Graphics 	offGraphics;

   // slider value before changing the slider value
   int 	preValue;

   // indicator to see if the slider has been changed
   boolean 	sliderChangedFlag = false;

   // display mode of slider value
   int		dispMode = PERCENTAGE;

   protected SliderListener sliderListener;

    /**
     * It is called automatically by the system the application is started.
     */
    public static void main(String args[]) {

	Frame  f = new Frame();
	Slider s = new Slider();

	s.setSliderRange(-100.001, 302.345);
	s.setSliderValue(0);

	f.setLayout(new BorderLayout());
	f.add("Center", s);
	f.setSize(200,50);
	f.show();

    }

  /**
   * Constructs a slider.
   */
  public Slider() {

	// set the default value
	minValue = DEFAULT_MIN;
	maxValue = DEFAULT_MAX;

	sliderMaxValue = (double)maxValue;
	sliderMinValue = (double)minValue;

        // resize the canvas
	setSize(DEFAULT_WIDTH, DEFAULT_HEIGHT);

    	// set the background
	bgColor    = Color.lightGray;
          
	// get the color for the "thumb"
	thumbColor = Color.lightGray;
    
        // get the bar color
	barColor   = Color.lightGray.darker();
  
	// get the slash color
	slashColor = Color.black;

        // set value
	setValue(minValue);
	addMouseListener(this);
	addMouseMotionListener(this);
  }

    /**
     * set the background color for the slider. 
     * @param color the new background color.
     */
    public void setBackgroundColor(Color color) {
	bgColor = color;
	repaint();
    }

    /**
     * set the color for the slider's thumb.
     * @param color the new thumb color.
     */
    public void setThumbColor(Color color) {
	thumbColor = color;
	repaint();
    }

   /**
     * set the color for the slider's bar. 
     * @param color  new bar color.
     */
    public void setBarColor (Color color) {
	barColor  = color;
	repaint();
    }

   /**
     * resize the canvas.
     */
    public void setBounds(int x, int y, int width, int height) {

	super.setBounds(x, y, width, height);

	minThumbPos = THUMB_SIZE + BUFFER;
	maxThumbPos = width - THUMB_SIZE - BUFFER - 1;

	adjustThumbPos();

    }
    

   /**
     * Sets the slash color for the slider. 
     * @param color The new slash color.
     */
    public void setSlashColor(Color color) {
	slashColor  = color;
	repaint();
    }

  /** set the slider width 
   * @param val the slider width to set
   */
   public void setWidth(int w) {

    // value is valid?
    if (w < MIN_WIDTH)   w  = MIN_WIDTH;
  
    setSize(w, getSize().height);
    
    // repaint the canvas
    repaint();
    
    }

  

  /** set the slider height 
   * @param val the slider height to set
   */
   public void setHeight(int h) {

    // value is valid?
    if (h < MIN_HEIGHT)   h  = MIN_HEIGHT;
  
    setSize(getSize().width, h);
    
    // repaint the canvas
    repaint();
    
    }

   /**
     * adjust thumb position
     */
    public void adjustThumbPos() {

	double valDeta = (double)(maxValue - minValue);
	double thumbDeta = (double)(maxThumbPos - minThumbPos);
	/*****************************
	if (value > minThumbPos)
	    thumbPos = (int)(Math.round(Math.abs((double)(value - minValue)/valDeta)*thumbDeta) +	
			     minThumbPos);
	else
	    thumbPos = minThumbPos;
	************************************/
	thumbPos = (int)(Math.round(Math.abs((double)(value - minValue)/valDeta)*thumbDeta) +	
		   minThumbPos); 
    }
    

  /** set the base number which slider can stand for
   * @param val the base value  
   */
   public void setBaseValue(int val) {
  
    	base	= val;

    	// repaint the canvas
    	repaint();
    
    }

  

  /** set display mode of slider value
   * @param val the display mode
   */
   public void setDispMode(int val) {
  
  	this.dispMode = val;
	repaint();  
    }

  

  /** set the slider range
   * @param min  the range value   
   * @param max  the range value  
   */
   public void setSliderRange(double min, double max) {
  
    	sliderMaxValue = max;
	sliderMinValue = min;
    
    }

  

  /** set  the value that slider replace.
   */
   public  void setSliderValue(double val) {
  
   	double ratio = (val - sliderMinValue) / (sliderMaxValue - sliderMinValue);

	int retVal = (int) (ratio * (maxValue - minValue + 1));
	retVal += minValue;

	setValue(retVal);
    }

  

  /** return  the value that slider replace.
   */
   public double getSliderValue() {
  
   	double deta = sliderMaxValue - sliderMinValue;

	double retVal = sliderMinValue;
    	retVal +=  deta * getRatio();

	return retVal;
    }

  

  /** set the slider value 
   * @param val the slider value  
   */
   public void setValue(int val) {
  
    	// value is valid?
    	if (val < minValue)   val = minValue;
    	
	// value is greater than default maxinum value? too big.
    	if (val > maxValue) val = maxValue;
  
  	// set value
    	this.value = val;

	adjustThumbPos();

    	// repaint the canvas
    	repaint();
    
    }

  

    /**
     * Returns the current value of the slider.
     * @return The current value of the slider.
     */
    public int getValue () {
	return value;
    }

    /**
     * Returns the current ratio for the current position in the slider.
     * @return  the current ratio for the current position in the slider.
     */
    public double getRatio () {

	double retVal = (double)((value-minValue+1)/(float)(maxValue-minValue+1));
	
	return retVal;
    }

    /**
     * sets the maximum value for the slider.
     * @param num the new maximum value.
     */
    public void setMaximum (int num) {
	// set maxinum value
	maxValue = num;
	
	// valid?
	if (maxValue < minValue) {
	    int    tmp = minValue;
	    minValue   = maxValue;
	    maxValue   = tmp;
	}
	
	selfChangedValue();
    }
    

    /**
     * return the maximum value for the slider.
     */
    public int getMaximum () {
	
	return maxValue;
	
    }
    

    /**
     * sets the minimum value for the slider.
     * @param num the new minimum value.
     */
    public void setMinimum (int num) {
	// set mininum value
	minValue = num;
	
	// valid?
	if (maxValue < minValue) {
	    int tmp    = minValue;
	    minValue   = maxValue;
	    maxValue   = tmp;
	}
	
	selfChangedValue();
    }
    

    /**
     * return the mininum value for the slider.
     */
    public int getMinimum () {
	
	return minValue;
	
    }
    

   public void selfChangedValue() {
	setValue(value);
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
	    // make the off-screen image big enough
	    if ((d.width*d.height) < 800 * 100)
	       offScreenImage = createImage(800,100);
	    else
	       offScreenImage = createImage(d.width, d.height);	
	    
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

    /**
     * Paints the component.
     * @param g the specified Graphics window
     * @see java.awt.Component#paint
     */
    public void paint(Graphics g) {
		
	// canvas size
	int w = getSize().width;	
	int h = getSize().height;

	// color of the background
	g.setColor(bgColor);

	// for slider
	g.setColor(barColor);
	g.fill3DRect(0, 0, w, h, false);
		     
	// for thumb
	g.setColor(thumbColor);	
	g.fill3DRect(thumbPos - THUMB_SIZE, BUFFER,
		     THUMB_SIZE * 2 + 1, h - 2 * BUFFER,
		     true);

	// for slash
	g.setColor(slashColor);
	g.drawLine(thumbPos, BUFFER + 1,
		   thumbPos, h - 2 * BUFFER);

	// print value
	g.setColor(Color.red);

	String valStr;
	if (dispMode == PERCENTAGE) {
	   valStr = String.valueOf(value*base);
	   valStr += "%";
	}
	else {
	   valStr = String.valueOf((int)Math.round(getSliderValue()));
	}
	g.drawString(valStr, 1,15);

    }

    /** Handle the mouse event
      * @param x the slide position
      */
     public void handleMouseEvent(int x) {

	int w    = getSize().width;

	thumbPos = Math.max(x, minThumbPos);
	thumbPos = Math.min(thumbPos, maxThumbPos);

	if (thumbPos > minThumbPos)
	    percent = (((double)thumbPos - minThumbPos) /
		       (maxThumbPos - minThumbPos));
	else
	    percent = 0;
	
	value = (int)(Math.round(percent * (double)(maxValue - minValue))) + minValue;
	
	repaint();
    }
    

  public void mousePressed(MouseEvent me)
  {
    int x = me.getX();
    handleMouseEvent(x);
    doMouseDown();
    
  }

  public void mouseReleased(MouseEvent me)
  {
    int x = me.getX();
    handleMouseEvent(x);
    doMouseRelease();
  } 
    

  public void mouseEntered(MouseEvent me) {}

  public void mouseExited(MouseEvent me) {}

  public void mouseClicked(MouseEvent me) {}

  public void mouseMoved(MouseEvent me) {}
    

  public void mouseDragged(MouseEvent me)
  {
    int x = me.getX();
    handleMouseEvent(x);
    doMouseDrag();
  }
    

   /**
    * This method is called when the "thumb" is dragged by
    * the user. Be overridden for some behavior.
    */
   public void doMouseDrag () { ; }

   /**
    * This method is called when the mouse is pressed on the the "thumb" .
    * Remember to  overridden if you want to have some action for that.
    */
   public void doMouseDown () { 

	sliderChangedFlag = false;

	// keep current value;
	preValue = getValue();

   }

  /**
    * This method is called when the mouse  is released.
    * Be overridden for some action to do.
    */
   public void doMouseRelease () { 

	// if (preValue != getValue())
	setSliderChanged();

  }
    

    public boolean sliderChanged() {

	return sliderChangedFlag;
    }

    public void setSliderChanged() {

	   sliderChangedFlag = true;
	   postSliderEvent();
    }

    public void setSliderUnchanged() {

	   sliderChangedFlag = false;
    }

  

    public synchronized void addSliderListener(SliderListener l) {
        sliderListener = SliderEventMulticaster.add(sliderListener, l);
    }

  

    public synchronized void removeSliderListener(SliderListener l) {
        sliderListener = SliderEventMulticaster.remove(sliderListener, l);
    }

  

    protected  void  postSliderEvent() {

	SliderEvent e = new SliderEvent(this, (int)getSliderValue());
	Toolkit	toolkit = getToolkit();
	EventQueue  queue = toolkit.getSystemEventQueue();
	queue.postEvent(e);
    }

    // DIspatch SliderEvent into  SliderListeners
    protected  void  processEvent(AWTEvent evt) {

	if (evt instanceof SliderEvent) {

	   // invoke sliderStateChanged()
	   if (sliderListener != null)
		sliderListener.sliderStateChanged((SliderEvent)evt);
	}
	else
	   super.processEvent(evt);
    }
}
