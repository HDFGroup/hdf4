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

import  java.awt.image.*;
import  java.awt.*;
import  java.awt.event.*;
import  java.lang.*;

import  ncsa.hdf.hdflib.*;
import  ncsa.hdf.awt.*;
import  ncsa.hdf.awt.event.*;
import  ncsa.hdf.awt.image.*;

//  Upgraded to the JDK 1.1.1b Event Model by Apu
public class JHVImageRGBControl extends Frame implements Runnable {

	JHVImageCanvas imageCanvas;

	// Maxinum control;
	Slider	brightSlider = new Slider();
	Slider	darkerSlider = new Slider();
	Slider  rgbSlider    = new Slider();
	Slider  redSlider    = new Slider();
	Slider  greenSlider  = new Slider();
	Slider  blueSlider   = new Slider();
	
	// thread of the control
	Thread	controlThread = null;

    // Image contrast/rgb control. 	
    public JHVImageRGBControl(JHVImageCanvas imageCanvas) {

	setTitle("Image RGB Balance Control");

	// this instance hold everything to be proceeded.
	this.imageCanvas = imageCanvas;
	
	// update image source
	imageCanvas.updateImageSource();

	// interface
	displayItems();

	setSize(360, 240);
	show();
	this.addWindowListener(new WindowClosedProcess());

	// new thread to see if the slider changed
	controlThread = new Thread(this);
	controlThread.start();
    }

    public void displayItems() {

	// new user interface to control the dataset range
	setLayout(new GridLayout(8,0));
	
	Panel p1 = new Panel();
	p1.setLayout(new BorderLayout());
	Label 	bLabel = new Label("Bright :", Label.RIGHT);
	bLabel.setSize(80,30);
	p1.add("West", bLabel);
	p1.add("Center", brightSlider );
	
	Panel p2 = new Panel();
	p2.setLayout(new BorderLayout());

	Label 	dLabel = new Label("Darker:", Label.RIGHT); 
	dLabel.setSize(80,30);
	p2.add("West", dLabel);
	p2.add("Center", darkerSlider );

	Panel p3 = new Panel();
	p3.setLayout(new BorderLayout());

	Label 	rgbLabel = new Label("RGB    :", Label.RIGHT);
	rgbLabel.setSize(80,30);

	p3.add("West", rgbLabel);
	p3.add("Center", rgbSlider );
	
	Panel p4 = new Panel();
	p4.setLayout(new BorderLayout());

	Label 	rLabel = new Label("Red    :", Label.RIGHT);
	rLabel.setSize(80,30);

	p4.add("West", rLabel);
	p4.add("Center", redSlider );

	Panel p5 = new Panel();
	p5.setLayout(new BorderLayout());

	Label  gLabel = new Label("Green :", Label.RIGHT);
	gLabel.setSize(80,30);

	p5.add("West", gLabel);
	p5.add("Center", greenSlider );

	Panel p6 = new Panel();
	p6.setLayout(new BorderLayout());

	Label  uLabel = new Label("Blue   :", Label.RIGHT );
	uLabel.setSize(80,30);

	p6.add("West", uLabel);
	p6.add("Center", blueSlider );

	add(new Label(" "));
	add(p1);
	add(p2);
	add(p3);
	add(p4);
	add(p5);
	add(p6);
	add(new Label(" "));

    }

    public boolean sliderChanged() {

	boolean retValue = false;

	// bright value
	retValue = (brightSlider.sliderChanged() || 
		    darkerSlider.sliderChanged() ||
  		    rgbSlider.sliderChanged() ||
		    redSlider.sliderChanged() ||
		    greenSlider.sliderChanged() ||
		    blueSlider.sliderChanged() );

	return retValue;   
	
    }

   public void set_Cursor(int cursor) {

	// change corsor type
	((Component) this).setCursor(new Cursor(cursor));

	// default toolkit
	Toolkit.getDefaultToolkit().sync();
    }

    public void close() {
	
	    // terminate my controlThread
	    if (controlThread!=null)
	    	controlThread.stop();
	    controlThread = null;

	    imageCanvas.imageFrame.imageRGBControl = null;
	    // close window
	    dispose();

    }

    /**
     * java.lang.Runnable stuff
     */
    public void run()  {

      while (controlThread != null) {
	
	if (sliderChanged()) {
	
		((Component)imageCanvas.imageFrame).setCursor(new Cursor(Cursor.WAIT_CURSOR));
		set_Cursor(Cursor.WAIT_CURSOR);

		// determine which slider changed
		if (brightSlider.sliderChanged()) { 
		   brightSlider.setSliderUnchanged();
		   ImageFilter filter = (ImageFilter)new BrightColorFilter(true, brightSlider.getValue());
		   imageCanvas.setImageRGBFilter(filter);
		}
	
		if (darkerSlider.sliderChanged()) { 
		   darkerSlider.setSliderUnchanged();
		   ImageFilter filter = (ImageFilter)new BrightColorFilter(false, darkerSlider.getValue());
		   imageCanvas.setImageRGBFilter(filter);
		}

		if (rgbSlider.sliderChanged()) { 
		   rgbSlider.setSliderUnchanged();
		   int rgb = rgbSlider.getValue();

		   ImageFilter filter = (ImageFilter)new RGBColorFilter(rgb,rgb,rgb);
		   imageCanvas.setImageRGBFilter(filter);
		}

		if (redSlider.sliderChanged()) {
		   redSlider.setSliderUnchanged();
		   int red = redSlider.getValue();
		   ImageFilter filter = (ImageFilter)new RGBColorFilter(red,0,0);
		   imageCanvas.setImageRGBFilter(filter);
		}
	
		if (greenSlider.sliderChanged()) {
		   greenSlider.setSliderUnchanged();
		   int green = greenSlider.getValue();
		   ImageFilter filter = (ImageFilter)new RGBColorFilter(0,green,0);
		   imageCanvas.setImageRGBFilter(filter);
		}
	
		if (blueSlider.sliderChanged()) {
		   blueSlider.setSliderUnchanged();
		   int blue = blueSlider.getValue();
		   ImageFilter filter = (ImageFilter)new RGBColorFilter(0,0,blue);
		   imageCanvas.setImageRGBFilter(filter);
		}

		((Component)imageCanvas.imageFrame).setCursor(new Cursor(Cursor.DEFAULT_CURSOR));
		set_Cursor(Frame.DEFAULT_CURSOR);

	}
	
	try {
	      Thread.sleep(5);
	} catch (Exception e) {;};
	
      } 

    }
}
