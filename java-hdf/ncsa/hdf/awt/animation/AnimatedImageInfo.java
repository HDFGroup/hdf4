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

package ncsa.hdf.awt.animation;

import java.awt.*;
import java.awt.event.*;
import java.awt.image.*;
import java.lang.String;
import java.lang.Thread;
import java.util.*;

/**
 *  This class define an animated image object, which keeps the image name,
 *  image source, and image transformation filter if possible.
 * @version 2.00  9/3/97
 * @auther  Xinjian Lu  		  
 */
public class AnimatedImageInfo implements  ImageObserver  {
	String		name;
	Image 		image;
	ImageProducer	imageSource;
	int 		width  = -1;
	int		height = -1;

	int 		order  = 0;
	// filter
	ImageFilter	imageFilter;

    // Constructs an AnimatedImageInfo with default.
    public AnimatedImageInfo() {
    }

    // Constructs an AnimatedImageInfo with the specifial image and image name.
    public AnimatedImageInfo(Image image, String name) {

	this.name  = name;
	this.image = image;
	if (image!=null) {
	   imageSource = image.getSource();

	   int loopCount = 0;
	   while (((this.width  = image.getWidth(this)) < 0) || 
		  ((this.height = image.getHeight(this))< 0) ){
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

    // Constructs an AnimatedImageInfo with the specifial image, image name and
    // the order which this image si to be displayed.
    public AnimatedImageInfo(Image image, String name, int order) {

	this(image, name);
	setOrder(order);	
    }

 

   // Sets image's name to the specified name
   public void setName(String name) {
	this.name = name;
    }

  // Gets the name of the image
  public String getName() {

	return (name);
    }

   // Sets the image of this AnimatedImageInfo to specified image
   public void setImage(Image image) {
	this.image = image;
    }

  // Gets the image of this AnimatedImageInfo.
  public Image getImage() {
	return ( this.image );
    }

  // Gets the image producer(the source of an image)
  public ImageProducer getImageSource() {

	return ( this.imageSource );
    }

  // Sets the image filter 
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

  // Sets the animated order to specified order
  public void setOrder(int order) {
	this.order = order;
  }

  // Gets the display order of this animated iamge
  public int getOrder() {
	return this.order ;
  }

 // Return the order string for this animated image information  
 public String getOrderInfo() {

	String info = "Order: " + getOrder() + " ( " + getName() + " )";
	return (info);
    }

    // Return the width of an image
    public int getWidth() {
	return (this.width);
    }

    // Return the height of an image
    public int getHeight() {
	return (this.height);
    }

   // Returns a String object representing this AnimatedImageInfo's values.
   public String toString ( ) {
      
      String s = "AnimatedImageInfo: " ;
      if (getName() != null)
	s += getName();
      if (width != -1)
	s += " width: " + width;
   
      if (height != -1)
	s += " height: " + height;

      s += " Order: " + getOrder();

      return( s );
   }

  // An implemented interface of ImageObserver
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

}
