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

import java.awt.Rectangle;

public class JHVVdataPlot {

    /* The following is for displaying the multiple  colnum selection */
    // draw by colnum
    boolean   drawColnumFlag = false ;
    Rectangle drawRectangleByColnum = null ;

    // selected colnum
    int selectedColnum = -1 ;  

    // data to be drawable
    double    data[];

    /** Default class constructor */
    public JHVVdataPlot() {
    }

    public void setData(double data[]) {
	this.data = data;
    }

    public double[] getData() {
	return this.data;
    }

    public void setDrawableRectangle(Rectangle rect) {
	drawRectangleByColnum = rect;
    }

    public Rectangle  getDrawableRectangle() {
	return drawRectangleByColnum;
    }

    public void setSelectedColnum(int num) {
	selectedColnum = num;
    }

    public int  getSelectedColnum() {
	return selectedColnum ;
    }
}
