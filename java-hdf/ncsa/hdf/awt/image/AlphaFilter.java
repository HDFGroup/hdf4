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

import java.awt.Color;
import java.awt.image.*;

/*
   class AlphaFilter - resets all alpha values to a specified percentage
         and/or sets a particular value (or index?) to fully transparent.
*/
public class AlphaFilter extends RGBImageFilter {
   int      alphaValue;             // transparency value for whole image
   int      alphaRGB;               // color for fully-transparent pixels

   boolean  alphaFlag = false;
   boolean  rgbFlag   = false;

   // single-color full transparency only
   public AlphaFilter ( int rgb ) {
      this();
      alphaValue = 0xff000000;
      alphaRGB   = rgb & 0xffffff;
      alphaFlag  = true;
   }

   public AlphaFilter ( ) {
      canFilterIndexColorModel = true;
      rgbFlag   = false;
      alphaRGB   = -1;
      alphaFlag = false;
      alphaValue = 0xff000000;
   }

   // whole-image partial percentage transparency
   public AlphaFilter ( Color color, float whole ) {
      this();
      if (whole > 0)
      {
          alphaValue = ((int) ((100-whole)*255/100.0)) << 24;
          alphaFlag  = true;
      }
      if (color != null)
      {
          alphaRGB   = color.getRGB() & 0xffffff;;
          rgbFlag    = true;
      }
   }
   public AlphaFilter ( Color color ) { this(color, -1); }
   public AlphaFilter ( float whole ) { this(null, whole); }

   // whole-image absolute partial transp plus single-color full transp
   public AlphaFilter ( int whole, int rgb ) {
      this();
      alphaValue = whole<<24;
      alphaFlag  = true;
      alphaRGB   = (rgb & 0xFFFFFF);
      rgbFlag    = true;
   }

   // whole-image partial percentage transp plus single-color full transp
   public AlphaFilter ( float whole, int rgb ) {
      this();
      alphaValue = ((int) ((100-whole)*255/100.0)) << 24;
      alphaFlag  = true;
      alphaRGB   = (rgb & 0xFFFFFF);
      rgbFlag    = true;
   }

   // set the alpha byte to the proper value
   public int filterRGB ( int x, int y, int rgb ) {

      int retVal ;
      /***************************************
      if (((rgb >> 24) & 0xFF) != 0xFF) {
	 // preserve any existing transparency
         return( rgb );
      }
      ****************************************/

      if ( rgbFlag & alphaFlag ) { 
	 if ( alphaRGB == (rgb & 0xFFFFFF)) // fully-trans pixel
	    retVal  =  rgb & 0xFFFFFF ;
	  else 
	    retVal  =  alphaValue | (rgb & 0xFFFFFF);
      }  else  if ( rgbFlag ) { // color trans
		  if ( alphaRGB == (rgb & 0xFFFFFF)) // fully-trans pixel

	    	       retVal  =  0xffffff ; 
	  	  else 
	    	       retVal  =  rgb ;  
		} else 
		       retVal = alphaValue | (rgb & 0xFFFFFF);
      return retVal;	  
   }
}
