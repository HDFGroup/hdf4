
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


package ncsa.hdf.awt.plots;

import  java.awt.*;
import  java.lang.*;
import  java.util.Vector;

import  java.awt.event.*;

// This class implements a generic means to make  xy-plots by providing dataset.
public class XYPlot extends Dialog  {

	// zoom state constants
	public static final int	ZOOM_IN	= 0;
	public static final int	ZOOM_OUT= 1;

	//  canvas that xy-plot can be painted on
	XYPlotPane      plotPane = null;

 	// Dataset being as an xy-plot
	double data[][];

	// current plots number along x-axis
	int	cxPlots;	
	
	// total plots number along x-axis
	int	xPlots;

	// current plots number along y-axis
	int	cyPlots;	
	
	// total plots number along y-axis
	int	yPlots;

	// check to see if  dataset range is found 
	boolean	isDataRangeFound = true;

	// indicator of xy-plot display
	boolean 	xyPlotDisplayed = false;

	// zoom state 
	int 	zoomState = ZOOM_IN;

	// Inner class for window closing
	class  XYPlotWindowClosedProcess extends WindowAdapter {
		public void windowClosing(WindowEvent e) {
			xyPlotExit();
		}
	}

    // default constructor
    public XYPlot(Frame f, String title) {

	super(f, title);
  
	// new plot canvas
    	plotPane = new XYPlotPane(this, data);
	
    	// set graphic panel
    	add("Center", plotPane);
 	show();

	// listener
	addWindowListener(new XYPlotWindowClosedProcess() );
    }

    // default constructor
    public XYPlot(Frame f, double[][] data) {
	this(f, "");

	// create xy-plot
	createXYPlot(data);
    }

    // default constructor
    public XYPlot(Frame f, String title, double[][] data) {
	this(f, title);

	// create xy-plot
	createXYPlot(data);
    }

    // create new xy-plot by specified dataset
    public  void createXYPlot(double data[][]) {

	// set data
	this.data = data;
	if (data != null) {
	   cxPlots = xPlots = data[0].length;
	   cyPlots = yPlots = data.length;
	}

	if (isDataRangeFound)  {   // need to find dataset range

	  double min=Double.MAX_VALUE;
	  double max=-min;

	  for (int kk=1; kk<yPlots; kk++) {
	      double[] tmpRange = getDataRange(data[kk]);
	      min = Math.min(min,tmpRange[0]);
	      max = Math.max(max,tmpRange[1]);
	  }

	  // reset y-axis range
	  plotPane.setYAxisRange(min,max);

          // set annotation for each xy-plot
	  plotPane.setAnnotation(null);

	} // if (isDataRangeFound)

	// check size
	plotPane.checkSize();

        // window listener
	addWindowListener(new XYPlotWindowClosedProcess());
    }

   public void setTitle(String title) {
   
    // set title	
    super.setTitle(title);
   }

  

 public void popup() {

    // default frame size
    setSize(500,400);

    // paint dataspread sheet
    plotPane.repaint();
    
  }

  // make xy-plot 
  public void makeXYPlot( int zoomFactor ) {
  
	int xOffset = 15;

 	//  x-axis plot number
	int  dataNumber = getXPlots();
	
	// adjust data number by zoom factor
	if (zoomFactor > 0) {
	   for (int kk=0; kk<zoomFactor; kk++)
		dataNumber /= 2;
	}
	else {
	   if (zoomFactor < 0)
	      for (int kk=0; kk<(-zoomFactor); kk++)
		  dataNumber *= 2;		
	}	   
	
	// adjust data number
	if ( dataNumber > xPlots ) 
	    dataNumber = xPlots;

	dataNumber -= xOffset;

	// At least one data is exist.
	if (dataNumber<=0) dataNumber = -dataNumber + 1;

	// need to update ........................................
	double tmpData[][] = new double[yPlots][dataNumber];

	for (int j=0; j<yPlots; j++)
	    for (int k=0; k<dataNumber; k++)
		tmpData[j][k] = data[j][k+xOffset];


	// set data
	plotPane.setData(tmpData);
	
	if (isDataRangeFound)  {   // need to find dataset range

	  double min=Double.MAX_VALUE;
	  double max=-min;

	  double[] tmpRange = getDataRange(data);

	  min = Math.min(min,tmpRange[0]);
	  max = Math.max(max,tmpRange[1]);

	  // reset y-axis range
	  plotPane.setYAxisRange(min,max);

          // set annotation for each xy-plot
	  plotPane.setAnnotation(null);

	} // if (isDataRangeFound)
  }

   //  Check zoom factor value to see if the xy-plot can be made by this factor
    public boolean isZoomReadable( int zoomFactor ) {
  
	boolean retValue = true;

 	//  xy-plot plot-data number by row
	int  dataNumber = getXPlots();
	
	// adjust data number by zoom factor
	if (zoomFactor > 0) {  // zoom-in
	   for (int kk=0; kk<zoomFactor; kk++)
		dataNumber /= 2;
	}
	else {
	   if (zoomFactor < 0)  // zoom-out
	      for (int kk=0; kk<(-zoomFactor); kk++)
		  dataNumber *= 2;		
	}	   
 
	// adjust plot-data number
	if  (dataNumber > 2*xPlots) 
	    retValue = false;

	// make sure it can be zoomed in.
	if  ((dataNumber < 5) && (zoomState == XYPlot.ZOOM_IN))
	    retValue = false;

	return retValue;
  }

   // find dataset range
   public double[] getDataRange(double data[][]) {

	if (data == null) 
	   return null;
	
	double range[] = new double[2];

  	// selected colnum data range for whole dataset
	double 	max, min;
	min = max =  data[0][0];

	for (int kk=0; kk<data[0].length; kk++) {
	  for (int jj=0; jj<data.length; jj++) {
	    double tmp = data[jj][kk];
	    min = Math.min(tmp,min);
	    max = Math.max(tmp,max);
	  }
	}
	
	range[0] = min;
	range[1] = max;

	return range;
     }

    // Get data range by specify dataset
    public double[] getDataRange(double data[]) {

	if (data == null) 
	   return null;
	
	double range[] = new double[2];
	
	int  dataLen = data.length;

  	// selected colnum data range for whole dataset
	double 	max, min;
	min = max =  data[0];

	for (int kk=1; kk<dataLen; kk++) {

	    double tmp = data[kk];
	    min = Math.min(tmp,min);
	    max = Math.max(tmp,max);
	}
	
	range[0] = min;
	range[1] = max;

	return range;
     }

   void setDataRangeFound(boolean flag) {

	this.isDataRangeFound = flag;
  }

 

    int getXPlots() {
        return cxPlots ;
    }
 
   

    int getYPlots() {
        return cyPlots ;
    }

   void  setXPlots(int n) {
        this.cxPlots = n ;
    }

      void  setYPlots(int n) {
        this.cyPlots = n ;
    }

  // new plots by specified dataset
  public void  setData(double dat[][]) {
	this.data = dat;
	if (dat != null) {
		this.xPlots = dat[0].length;
		this.yPlots = dat.length;
	}
	plotPane.setData(dat);

	if (isDataRangeFound) {
	   double min,max;
	   min=max=dat[1][0];
	   for (int kk=1; kk<dat.length; kk++) {
	   	double tmp[] = getDataRange(dat[kk]);
		min = Math.min(min, tmp[0]);
		max = Math.max(max, tmp[1]);
	   }
	   setYAxisRange(min, max);
	}
    }

    // Set X-Axis annotation(tag)
    public void setXAxisTag(String xTag) {
	plotPane.setXAxisTag(xTag);
    }

 

   // Set Y-Axis annotation(tag)
    public void setYAxisTag(String yTag) {
	plotPane.setYAxisTag(yTag);	
    }

   // Set x-axis data range 
   public void setXAxisRange(double xmin, double xmax) {
	plotPane.setXAxisRange(xmin,xmax);
    }

   // Set y-axis data range 
   public void setYAxisRange(double ymin, double ymax) {
	plotPane.setYAxisRange(ymin,ymax);
    }

   // Set colors for  each  point in one plot
    public void setColors(Color[] colors) {
	plotPane.pColors = colors;	
    }

   // set plot mode (line or plot)
    public void setPlotMode(int mode) {

	plotPane.setPlotMode(mode);
    }

 // xy-plot exit
 public void xyPlotExit() {
	xyPlotDisplayed = false;
	// close the window
	dispose();
 }
}
