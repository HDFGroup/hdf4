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

import  java.util.Random;
import java.awt.image.*;

public class NoiseFilter extends RGBImageFilter {

	// new Random instance
	Random random = new Random();

    // default constructor
    public NoiseFilter() {
	
	canFilterIndexColorModel = true;
    }

   public int filterRGB(int x,int y,int rgb) {

	int alpha = (rgb >> 24) & 0xff;

	// add noise to red component
	int r =   (rgb >> 16) & 0xff;
	r  += (int)(getRandomInt()&0x3f - 0x20);

	// add noise to green component
	int g  = (rgb >>  8) & 0xff;
	g +=  (int)(getRandomInt()&0x3f - 0x20);

	// add noise to blue component
	int b = (rgb      ) & 0xff;
	b +=  (int)(getRandomInt()&0x3f - 0x20);

	r = range(r,0,255);
	g = range(g,0,255);
	b = range(b,0,255);

	// rgb value
	return( alpha<<24 | r<<16 | g<<8 | b );
	      
	      
    }

   public int range(int a, int b, int c) {
	
	// set a to b & c
	if (a<b) 
	   a = b;

	if (a>c)
	   a = c;
	  
	return a;    
    }

   public int  getRandomInt() {

	int  num = Math.abs(random.nextInt());

	return(num);
   }
}
