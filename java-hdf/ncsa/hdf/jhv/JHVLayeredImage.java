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
import  ncsa.hdf.awt.layer.*;
import  ncsa.hdf.awt.image.*;

/**
 * This class retrieve GR information, then keep it in LayeredImageInfo instance
 * , which is for "overlay" image display. 
 * @version 1.00	9/1/97 xlu
 * @auther  Xinjian Lu
 */
public class JHVLayeredImage {

    LayeredImage layeredImage = null;

    // new instance using images
    public JHVLayeredImage(Image[] images)
    {
        if (images != null)
        {
           int imageNumber = images.length;
           LayeredImageInfo imageInfos[] = null;
           imageInfos = new LayeredImageInfo[imageNumber];

           // retrieve images info.
           setImagesInfo(images, imageInfos);

           if (layeredImage == null)
              layeredImage = new LayeredImage();

           // add images to panel
           layeredImage.setLayeredImage(imageInfos);
        }        
    }

    public void setImagesInfo(Image[] images, LayeredImageInfo[] infos)
    {
        String name = "";
        Image image = null;

        for (int kk=0; kk<infos.length; kk++)
        {
            name = "Image #" + kk;
            image = images[kk];
            if (image != null)
                infos[kk] = new LayeredImageInfo(image, name);
        }
    }
    
}
