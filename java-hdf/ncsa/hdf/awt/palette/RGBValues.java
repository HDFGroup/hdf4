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

// This class implement the RGB value holder
public class RGBValues {
	
	// red
	byte[]	red;

	// green
	byte[] 	green;

	// blue
	byte[]	blue;

    // default class
    public RGBValues() {
    }

    // default class
    public RGBValues(byte[] r, byte[] g, byte[] b) {
	
	// red
	if (r!=null) {
	   this.red   = new byte[r.length];
	   System.arraycopy(r,0,this.red,0,r.length);
	}
	else this.red = null;

	// green
	if (g!=null) {
	   this.green   = new byte[g.length];
	   System.arraycopy(g,0,this.green,0,g.length);
	}
	else this.green = null;

	// blue
	if (b!=null) {
	   this.blue   = new byte[b.length];
	   System.arraycopy(b,0,this.blue,0,b.length);
	}
	else this.blue = null;

    }

    public void setRed(byte[]  red) {

	this.red = red;
    }

    public void setGreen(byte[]  green) {

	this.green = green;
    }

    public void setBlue(byte[]  blue) {

	this.blue = blue;
    }

    public byte[] getRed() {

	return(this.red);
    }

    public byte[] getGreen() {

	return(this.green);
    }

    public byte[] getBlue() {

	return(this.blue);
    }
}
