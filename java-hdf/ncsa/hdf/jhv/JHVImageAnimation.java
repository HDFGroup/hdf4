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
import  java.awt.image.*;

import  ncsa.hdf.hdflib.*;
import  ncsa.hdf.awt.image.*;
import  ncsa.hdf.awt.animation.*;

/**
 * This class retrieve GR information, then keep it in LayeredImageInfo instance
 * , which is for  image animation. 
 * @version 1.00	9/6/97 xlu
 * @auther  Xinjian Lu
 */
public class JHVImageAnimation {

    ImageAnimation animatedImage;

    // new instance using images
    public JHVImageAnimation(Image[] images) {

	if (images != null) { // image array
	   // image number
	   int imageNumber = images.length;
	
	   // variable to keep image info.
	   AnimatedImageInfo imageInfos[] = null;
	   imageInfos = new AnimatedImageInfo[imageNumber];

	   // retrieve images info.
	   setImagesInfo(images, imageInfos);

	   //  images display handler
	   // ImageAnimation animatedImage = new ImageAnimation();
	   if (animatedImage == null)
	      animatedImage = new ImageAnimation();

	   // add images to panel
	   animatedImage.setAnimatedImage(imageInfos);
	}	
    }

    // retrieve image(s) and "name" , then hold it for animation
    public void setImagesInfo(Image[] images, AnimatedImageInfo[] infos) {

	// for each dataset
	for (int kk=0; kk<infos.length; kk++) {

		   // raster image name
		   String name = "Image #" + kk;

		  // int w = images[kk].getWidth(this);
    		  // int h = images[kk].getHeight(this);

 		  Image image = images[kk];
		  if (image != null)
			infos[kk] = new AnimatedImageInfo(image, name);

    	} // for (int kk=0; kk<infos.length; kk++) {
    }

}
