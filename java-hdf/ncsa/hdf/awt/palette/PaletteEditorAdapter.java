
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

import  java.awt.image.*;
import  java.awt.Image;

// The interface for objects which implement image process and want to do palette
// modification for image
public interface PaletteEditorAdapter {

    // An object who implements image processing and want to do palette thing
    // such like palette modification should define one variables to indentify
    // the palette editor window open or not. 
    public void setPaletteEditorOpen(boolean flag);
    

    // colormodel  changed, how about the image? need to modify the image with
    // new color model
    public   void updateColorModel(ColorModel cm);

  // set current image 
  public  void setImage(Image img) ;

 

  // return current image 
  public  Image getImage();
}
