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

import  ncsa.hdf.layer.*;
import  ncsa.hdf.hdflib.*;
import  java.awt.*;
import  java.awt.image.*;
import  ncsa.hdf.java.awt.image.*;

/**
 * This class retrieve GR information, then keep it in LayeredImageInfo instance
 * , which is for "overlay" image display. 
 * @version 1.00	9/1/97 xlu
 * @auther  Xinjian Lu
 */
public class JHVLayeredImage {

	// HDF file
	String	hdfFileName;

	// HDF instance
	HDFLibrary  hdf = new HDFLibrary();

    // new JHVLayeredImage instance by HDF file
    public JHVLayeredImage(String  file) {

	this.hdfFileName = file;
	if ((file != null) && (file.length()!=0) ) { // valid HDF file
		
	   int fid = -1;
	   try {
	    	fid = hdf.Hopen(this.hdfFileName);   
	   } catch (Exception erro) { 
		System.out.println("Exception: " + erro);
	   }

	   int imageNumber = getHDFGRImageNumber(fid);

	   // variable to keep image info.
	   LayeredImageInfo imageInfos[] = new LayeredImageInfo[imageNumber];

	   // retrieve images info.
	   setHDFGRImagesInfo(fid, imageInfos);

	   // layer images display handler
	   LayeredImage layeredImage = new LayeredImage();

	   // add images to panel
	   layeredImage.setLayeredImage(imageInfos);
	}
	
    }

    // return GR dataset number, otherwise -1
    public int getHDFGRImageNumber(int fid) {

     try {
	// get ready to read the HDF GR interface
    	int grid = -1;
    	int riid = -1;

    	grid = hdf.GRstart(fid);
	
    	if (grid == HDFConstants.FAIL) {
      	   hdf.GRend(grid);
      	   return -1;
    	}
    
    	// get HDF GR information
    	int[] fileInfo = new int[2];
    
    	if (hdf.GRfileinfo(grid, fileInfo) == false) {
      	   // terminate the GR access
      	   hdf.GRend(grid);
      	   return -1;
    	}

	// terminate GR access
	hdf.GRend(grid);
	 
 	//  GR dataset number (image(s));
      	return fileInfo[0];
      } catch (HDFException e) {};

      return -1;
    }

  /** Get the HDF  generic raster image by specified plane number
   * @param riid the raster image identifier 
   * @param imgInfo the image info. array
   * @param plane   the plane number to be retrived.
   * @return the image, otherwise null
   */
  public Image getHdfGRimage(int riid, int ncomp, int nt, int interlace, int w, int h, int plane) 
throws HDFException {

    Image image = null;
    
    boolean hasPalette = false;
    
    // image data size;
    int dataSize = w*h*ncomp*hdf.DFKNTsize(nt);
    
    // specify the image data storage	
    byte imageData[]    = new byte[dataSize];
    
    // image palette
    byte imagePalette[] = new byte[3*256];
    
    // set the interlace for reading by component interlacing scheme
    if (hdf.GRreqimageil(riid, HDFConstants.MFGR_INTERLACE_COMPONENT) == false)
      return null;
    
    // get palette info.
    int lutid = hdf.GRgetlutid(riid, 0);
    
    if (lutid == HDFConstants.FAIL)
	   hasPalette = false;
    else
      hasPalette = true;
    
	// specify palette info.
    int[] lutInfo = new int[4];
    
    hdf.GRreqlutil(riid, HDFConstants.MFGR_INTERLACE_PIXEL);	
    
    if ((hasPalette) && (hdf.GRreadlut(lutid, imagePalette))) {
      
      // get palette (easy processing)
      ;  	
    }
    else  { // default	
      for (int i=0; i<256; i++)
	for (int j=0; j<3; j++)  
	  imagePalette[i*3+j] = (byte)i;
    }
 
    
    // read image data	
    int start[] = new int[2];
    int stride[]= new int[2];
    int count[] = new int[2];
    
    start[0] = 0;
    start[1] = 0;
    
    stride[0] = 1;
    stride[1] = 1;
    
    count[0] = w  ;  // x
    count[1] = h   ;  // y
    
    boolean readFlag = hdf.GRreadimage(riid,start,stride,count,imageData);
    
    // image data
    byte[] output = new byte[w*h];
    
    if (readFlag) {
      // convert the data into a standard image  
      if (ImageDataConverter.makeImageData(imageData,nt,w,h,(w*h*(plane-1)),output)) 
	image = createRasterImage(output,w,h,imagePalette);
    }
    return image;
  }
  

    // retrieve GR image(s) and "name" , then hold it for layer display
    public void setHDFGRImagesInfo(int fid, LayeredImageInfo[] infos) {

     try {
	// get ready to read the HDF GR interface
    	int grid = -1;
    	int riid = -1;
        try {
    	    grid = hdf.GRstart(fid);
	} catch (HDFException e) {};

    	if (grid == HDFConstants.FAIL) {
      	   hdf.GRend(grid);
      	   return ;
    	}
    
	// for each dataset
	for (int kk=0; kk<infos.length; kk++) {
	
	        if ((riid = hdf.GRselect(grid,kk)) == HDFConstants.FAIL) { 
		   hdf.GRendaccess(riid);
		   return;
      		}
      		else { 
		   // GR name
		   String[] name = new String[1];
		   name[0] = "";

		   int[] dim_sizes = new int[2];   
		   int[] imgInfo   = new int[4];

		   if (hdf.GRgetiminfo(riid, name, imgInfo, dim_sizes) ) {

			int w = dim_sizes[0];
			int h = dim_sizes[1];
			int plane = 1;
			Image image = getHdfGRimage(riid, imgInfo[0], imgInfo[1], imgInfo[2], w, h, plane);

			if (image != null)
			   infos[kk] = new LayeredImageInfo(image, name[0]);

		   }
      		} // if ((riid = hdf.GRselect(grid,index)) == ...
    	} // for (int kk=0; kk<infos.length; kk++) {

	// terminate GR access
	hdf.GRend(grid);
      } catch (HDFException e) {}
    }

  public Image createRasterImage(byte[]  imgData, int w, int h,byte[] imgPal) {
    
    return(createRasterImage(imgData,w,h,imgPal,1));
  }
 

 public Image createRasterImage(byte[]  imgData, int w, int h,
				 byte[] imgPal,int index) {
 
    if ((w*h)<=0)
      return null;
    
    if (index < 1 )
      return null;
    
    // set the created image data
    byte data[] = new byte[w*h];
    
    int pos = (index-1)*w*h;
    
    for (int i=0; i<(w*h); i++) 	     
      data[i] = (byte)imgData[pos++];
    
    // set the RGB
    byte[] red   = new byte[256];
    byte[] green = new byte[256];
    byte[] blue  = new byte[256];
    
    for (int i=0; i<256; i++) {
      red[i]   = (byte)(imgPal[i*3]);
      green[i] = (byte)(imgPal[i*3+1]);
      blue[i]  = (byte)(imgPal[i*3+2]);
    }
    
    // set the color model
    ImageColorModel imagePalette;
    imagePalette = new ImageColorModel(red,green,blue);

    // create the image
    return(Toolkit.getDefaultToolkit().createImage(new MemoryImageSource(w,h,imagePalette.getColorModel(),data,0,w)));
   
  }
}
