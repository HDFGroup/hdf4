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

import  java.awt.*;
import  java.lang.*;

import  ncsa.hdf.hdflib.*;
import  ncsa.hdf.awt.*;
import  ncsa.hdf.awt.event.*;

//  Upgraded to the JDK 1.1.1b Event Model by Apu.
public class JHVSDSImageControl extends Frame implements Runnable {

	JHVImageCanvas imageCanvas;

	// Maxinum control;
	Slider	maxSlider;
	Slider	minSlider;

	double maxValue = 0.0d;
	double minValue = 0.0d;

	// two label, need to update when slider changed
	Label 	maxLabel = new Label("", Label.RIGHT);
	Label	minLabel = new Label("", Label.RIGHT);

	// thread of the control
	Thread	controlThread = null;

    public JHVSDSImageControl(JHVImageCanvas imageCanvas) {

	setTitle("New SDS Image Control");

	this.imageCanvas = imageCanvas;

	maxValue = imageCanvas.max;
	minValue = imageCanvas.min;

	maxSlider = new Slider();
	minSlider = new Slider();

	// get real max. min
	double[] minmax = null;
	minmax = ImageDataConverter.getDataRange(imageCanvas.hdfData, imageCanvas.hdfDataType);

	if (minmax != null) {

	   if (minValue == maxValue) { // no max. or min
		// default
		minValue = minmax[0];
		maxValue = minmax[1];
	   }

	   maxSlider.setSliderRange(minmax[0], minmax[1]);
	   maxSlider.setSliderValue(maxValue); // current max.
	   minSlider.setSliderRange(minValue, maxValue);
	   minSlider.setSliderValue(minValue); // current min.
	}

	// reset label
	maxLabel.setText(getString(maxValue,10));
	minLabel.setText(getString(minValue,10));

	displayItems();

	setSize(360, 120);
	show();
	this.addWindowListener(new WindowClosedProcess());
	// new thread
	controlThread = new Thread(this);
	controlThread.start();
    }

    public void displayItems() {

	// new user interface to control the dataset range
	setLayout(new GridLayout(5,3));

	Panel p1 = new Panel();
	p1.setLayout(new BorderLayout());
	p1.add("West", new Label("Maximum: "));
	p1.add("Center", maxSlider );
	p1.add("East", maxLabel );

	Panel p2 = new Panel();
	p2.setLayout(new BorderLayout());
	p2.add("West", new Label("Minimum: " ));
	p2.add("Center", minSlider );
	p2.add("East", minLabel );

	add(new Label(" "));
	add(p1);
	add(new Label(" "));
	add(p2);
	add(new Label(" "));

    }

    public boolean sliderChanged() {

	boolean retValue = false;

	// maximum value
	retValue = maxSlider.sliderChanged();

	if (!retValue) {

	   // minimun slider
	   retValue = minSlider.sliderChanged();
	   if  (retValue) { // minimum changed
		minValue = minSlider.getSliderValue();
	   }
	} 
	else {
	   maxValue = maxSlider.getSliderValue();
	}

	return retValue;   
	
    }

    // construct new string by given double number and length
    // return the specified length string
    public String getString(double value, int strLen) {

	String retStr="";
	if (value >= 0.0) 
	   retStr += "+";

	retStr += Double.toString(value);
	
	if (value == 0.0) 
	   retStr += "0000";

	int	len = retStr.length();
	if (len >= strLen)
	   return retStr.substring(0,strLen);
	else 
	   return (retStr + space(strLen-len));

    }

    // construct new blank string by given number
    // return the specified length blank 
    public String space(int num) {

	String  retStr = "";
	int	len = num;
	while(len-->0)
	  retStr += " ";

	return retStr;
    }

    /**
     * java.lang.Runnable stuff
     */
    public void run()  {

      while (controlThread != null) { // loop

	if (sliderChanged()) { // slider changed

	   // label changed
	   maxLabel.setText(getString(maxValue,10));
	   minLabel.setText(getString(minValue,10));

	   // reset slider
	   maxSlider.setSliderUnchanged();
	   minSlider.setSliderUnchanged();

	   imageCanvas.updateSDSImage(minValue,maxValue);

	}
	
	try {
	      Thread.sleep(10);
	} catch (Exception e) {;};

      } 

    }

    public void close() {
	
	    // terminate my controlThread
	    if (controlThread!=null)
	       controlThread.stop();
	    controlThread = null;

	    imageCanvas.imageFrame.sdsImageControl = null;

	    // close window
	    dispose();

    }
}
