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

package ncsa.hdf.awt.layer;

import java.awt.*;
import java.awt.event.*;
import java.awt.image.*;
import java.lang.String;
import java.lang.Thread;
import java.util.*;

// Image layer information object.
public class LayeredImageInfo  implements  ImageObserver  {

	String		name;
	Image 		image;
	ImageProducer	imageSource;
	ImageFilter	imageFilter;
	int 		width  = -1;
	int		height = -1;
	int 		layer  = 0;

    public LayeredImageInfo() {
    }

    public LayeredImageInfo(Image image, String name) {

	this.name  = name;
	this.image = image;

	if (image!=null) { 
	   imageSource = image.getSource();

	   int loopCount = 0;
	   while (((this.width  = image.getWidth(this)) < 0) || 
		  ((this.height = image.getHeight(this))< 0) ) {
 
	     try {
		// wait a minute
		Thread.sleep(20l);
	     } catch (Exception e) {};

	     loopCount++;
	     if (loopCount > 3)
		break;
	  }
	}
    }

    public LayeredImageInfo(Image image, String name,int layer) {

	this(image, name);
	setLayer(layer);	
    }

    public void setName(String name) {

	this.name = name;
    }

    public String getName() {

	return (name);
    }

    public void setImage(Image image) {

	this.image = image;
    }

    public void setFilter(ImageFilter filter) {

	// new filter
	this.imageFilter  = filter;

	// new image
	if (filter != null)  {
	
	    // get image source (image producer)
	    if ((imageSource == null) && (getImage() != null))
		imageSource = getImage().getSource();

	    Image newImage = null;

	    FilteredImageSource  src = null;
	    src = new FilteredImageSource(imageSource,filter);

	    newImage = Toolkit.getDefaultToolkit().createImage(src);
	    setImage(newImage);
	}
    }

    public Image getImage() {

	return ( this.image );
    }

    public void setLayer(int layer) {
	this.layer = layer;
    }

    public int getLayer() {

	return (this.layer);
    }

    public String getLayerInfo() {

	String info = "Layer: " + getLayer() + " ( " + getName() + " )";
	return (info);
    }

    public int getWidth() {

	return (this.width);
    }

    public int getHeight() {

	return (this.height);
    }

  public boolean imageUpdate ( Image img, int infoflags, int x, int y,
         int width, int height ) {
 
      if ((infoflags & (HEIGHT | WIDTH)) != 0) {
         if ((infoflags & HEIGHT) != 0) {    
   		// ...
         }

         if ((infoflags & WIDTH) != 0) {    
     		// ...
         }
      }

      if ((infoflags & (ALLBITS | ERROR)) != 0) {
     	// ....
      }
      return ((infoflags & (ALLBITS | ERROR)) == 0);     
   }

   // Returns a String object representing this LayeredImageInfo's values.
   public String toString ( ) {
      
      String s = "LayeredImageInfo: " ;
      if (getName() != null)
	s += getName();
      if (width != -1)
	s += " width: " + width;
   
      if (height != -1)
	s += " height: " + height;

      s += " Layer: " + getLayer();

      return( s );
   }

}
