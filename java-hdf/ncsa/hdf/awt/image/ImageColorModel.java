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


package ncsa.hdf.awt.image;
    
   import java.awt.image.IndexColorModel;

import java.awt.image.*;

/** set up the color model for displaying the   image
   *  created by the data sets from the file 
   */


   public class ImageColorModel   {
	
	private final static int bgGrey = 192;
	
	public byte red[] = new byte[256];
	public byte green[] = new byte[256];
	public byte blue[] = new byte[256];

 	private int Rl;
  	private int Gl;
   	private int Bl;

   /** Create the new color model
    * @param r the red value
    * @param g the green value
    * @param b the blue value
    */


   public ImageColorModel(int r, int g, int b) {
	this.Rl = r;
	this.Gl = g;
	this.Bl = b;
	setup();
    }

  

   /** default color model */

   public ImageColorModel() {
	this(255,100,200);
	//this(0,0,0);

    }

   /** create the new color model 
    * @param rgb rgb value
    */
   public ImageColorModel(byte[] rgb) { 
	
	for (int i=0; i<256; i++) {
	     red[i]   = (byte)rgb[3*i  ];
	     green[i] = (byte)rgb[3*i+1];
	     blue[i]  = (byte)rgb[3*i+2];
	}

    }

   /** create the new color model 
    * @param red The array of red color components.
    * @param green The array of green color components.
    * @param blue  The array of blue color components.
    */
   public ImageColorModel(byte r[], byte g[], byte b[]) { 
	//System.arraycopy(this.red, 0, r, 0, 256);
 	//System.arraycopy(this.green, 0, g, 0, 256);
	//System.arraycopy(this.blue, 0, b, 0, 256);
	for (int i=0; i<256; i++) {
	     red[i]   = r[i];
	     green[i] = g[i];
	     blue[i]  = b[i];
	}

    }

   /** create the new color model 
    * @param rgb rgb value
    */
   public ImageColorModel(char[] rgb) { 
	
	for (int i=0; i<256; i++) {
	     red[i]   = (byte)rgb[3*i  ];
	     green[i] = (byte)rgb[3*i+1];
	     blue[i]  = (byte)rgb[3*i+2];
	}

    }

   /** get  the  color model value */ 

   public IndexColorModel getColorModel() {

	return (new IndexColorModel(8,256,red,green,blue,0));
    }

   /** set up color model */

   private void setup() {
	

	red[0] = (byte) bgGrey;
	green[0] = (byte) bgGrey;
	blue[0] = (byte) bgGrey;
	
	float b = (float)1.0;

	for (int i=255; i>=1; i--) {
		float d = (float) i/255;
		
		red[i] = (byte) blend(blend(Rl, 255, d), bgGrey, b);
		green[i] = (byte) blend(blend(Gl, 255, d), bgGrey, b);
		blue[i] = (byte) blend(blend(Bl, 255, d), bgGrey, b);
	    }

    }

    public byte[] getRed() {
	return red;
    }

    public byte[] getGreen() {
	return green;
    }

    public byte[] getBlue() {
	return blue;
    }

   /** get the new value for the color value */

    private final int blend(int fg,int bg, float fgfactor) {

	return (int) (bg + (fg - bg) * fgfactor);
    }
}
