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

import java.awt.*;
import java.awt.image.*;

import ncsa.hdf.hdflib.*;
import ncsa.hdf.awt.image.ImageColorModel;

/** Convert scientific data into image data. 
 * @version 1.00 	9/12/97
 * @auther  Xinjian Lu
 */
public class ImageDataConverter {

   // default constructor 
   public ImageDataConverter() {
    }

     // convert data into image
     private static boolean dataToImage(float input[], int w, int h, byte output[]) {
 
	if ((input == null) || (output == null) || ((w*h) <= 0))
	    return false;
	  
        float   dataMax,dataMin; 
	dataMax = input[0];
	dataMin = input[0];

	// find man. and max. value from input
	for (int i=0; i<w*h; i++) {
		float tmp = input[i];
		if (dataMax<tmp)
		   dataMax = tmp;
		else if (dataMin>tmp)
			dataMin = tmp;
	}

	float deta = dataMax-dataMin;
	float spread = (dataMax == dataMin) ? 1.00f : (float)(255.00/deta);

	// Is max-min big enough for converting scientific data to image data
 	if (spread > 255.00/2.1e9) {  
           for (int i=0; i<(w*h); i++) {
            output[i] = (byte)((input[i]-dataMin)*spread); 
	    if (output[i] == 0) output[i] = 1;
	   }
	} 
	else {
  	   for (int i=0; i<(w*h); i++)  
	       output[i] = 1;
	}
	return true;
     }

     // convert data into image based on specified dataset range
     private static boolean dataToImage(float input[], float dataMin, float dataMax, byte output[]) {
 
	if ((input == null) || (output == null))
	    return false;
	int size = output.length;

	float deta = dataMax-dataMin;

	if (deta>1.0e20) { // big enough, re-check the dataset range
	        dataMax = dataMin = input[0];
		// find man. and max. value from input
		for (int i=0; i<size; i++) {
			float tmp = input[i];
			if (dataMax<tmp)
		   	   dataMax = tmp;
			else if (dataMin>tmp)
			   dataMin = tmp;
		}
	}

	deta = dataMax-dataMin;
	float spread = (dataMax == dataMin) ? 1.00f : (float)(255.00/deta);
	
	// Is max-min big enough for converting scientific data to image data
 	if (spread > 255.00/2.1e9) {  
           for (int i=0; i<size; i++) {
            output[i] = (byte)((input[i]-dataMin)*spread); 
	    if (output[i] == 0) output[i] = 1;
	   }
	} 
	else {
  	   for (int i=0; i<size; i++)  
	       output[i] = 1;
	}
	return true;
     }

   /** 
    * convert scientific data to image data
    * @param data the scientific data
    * @param datatype the data type of the scientific data
    * @param range    the specified dataset range[min, max]
    * @param w the width of the converted image
    * @param h the height of the image
    * @param pos the first converted scientific data position
    * @param output the converted image data
    * @return TRUE if successed, or false
    */
    public  static boolean makeImageData(byte data[],
		double minVal, double maxVal, 
		int nt, int w, int h, int pos,
		byte out[]) {

	// data convert
	HDFNativeData convert = new HDFNativeData();
	int datasize = 1;
    	try {
      	    // data size for the original data based on the data number type
      	    datasize = HDFLibrary.DFKNTsize(nt);
    	} catch (HDFException e) {};

	boolean retVal = false;
	
	float min = (float)minVal;
	float max = (float)maxVal;

	// number of data 
 	int nelmt = (int)w*h;
  	int start = (int)pos;

	int size = w*h;
	float[] buffer = new float[size];

	if ((nt & HDFConstants.DFNT_LITEND) != 0) {
		//  this code probably will fail -- use HDFNative?
		nt -= HDFConstants.DFNT_LITEND;
	}
	// where should we  go?
	switch(nt) {
	// one bit char
	case HDFConstants.DFNT_CHAR:
	case HDFConstants.DFNT_UCHAR8:
	case HDFConstants.DFNT_UINT8:  
	     // for each of data value
	     for (int i=0; i<nelmt; i++)  {
      	     	buffer[i] = (float)(data[i+start]);
		if (buffer[i]<0) buffer[i] += 256.0f;
      	     	if (buffer[i]>max) 
		    buffer[i] = max;
      	     	else if (buffer[i]<min)
	  	         buffer[i] = min;
	     }
    	     // data converter */
	     for (int i=0; i<nelmt; i++) 
		  out[i] = (byte)buffer[i]; 
	     retVal = true;
    	     // retVal = dataToImageData(buffer,(int)w, (int)h, out);
    	     break;
	// signed integer (byte)	
	// case HDFConstants.DFNT_UINT8:  
	case HDFConstants.DFNT_INT8:
	     // for each of data value
	     for (int i=0; i<nelmt; i++)  {
      	     	buffer[i] = (float)(data[i+start]);
      	     	if (buffer[i]>max) 
		    buffer[i] = max;
      	     	else if (buffer[i]<min)
	  	         buffer[i] = min;
	     }
    	     // data converter */
    	     // retVal = dataToImage(buffer,(int)w, (int)h, out); 
	     retVal = dataToImage(buffer,min,max, out);
    	     break;
   	// short	
	case HDFConstants.DFNT_INT16:
	case HDFConstants.DFNT_UINT16:
	     // for each of data value
	     for (int i=0; i<nelmt; i++)  {
		  pos = i*datasize + start;
      	     	  buffer[i] = (float)(convert.byteToShort(data, pos));
      	     	  if (buffer[i]>max) 
		      buffer[i] = max;
      	     	  else if (buffer[i]<min)
	  	      	   buffer[i] = min;
	     }
    	     // data converter */
    	     // retVal = dataToImage(buffer,(int)w, (int)h, out);
	     retVal = dataToImage(buffer,min,max, out);
    	     break;

      	case HDFConstants.DFNT_INT32:
	case HDFConstants.DFNT_UINT32:
	     // for each of data value
	     for (int i=0; i<nelmt; i++)  {
		  pos = i*datasize + start;
      	     	  buffer[i] = (float)(convert.byteToInt(data, pos));
      	     	  if (buffer[i]>max) 
		      buffer[i] = max;
      	     	  else if (buffer[i]<min)
	  	      	   buffer[i] = min;
	     }
    	     // data converter */
    	     // retVal = dataToImage(buffer,w,h, out);
	     retVal = dataToImage(buffer,min,max, out);
    	     break;
 	case HDFConstants.DFNT_FLOAT32:
	     // for each of data value
	     for (int i=0; i<nelmt; i++)  {
		  pos = i*datasize + start;
      	     	  buffer[i] = (float)(convert.byteToFloat(data, pos));
      	     	  if (buffer[i]>max) 
		      buffer[i] = max;
      	     	  else if (buffer[i]<min)
	  	      	   buffer[i] = min;
	     }
    	     // data converter */
    	     // retVal = dataToImage(buffer,w,h, out);
	     retVal = dataToImage(buffer,min,max, out);
    	     break;
 	case HDFConstants.DFNT_FLOAT64:
	     // for each of data value
	     for (int i=0; i<nelmt; i++)  {
		  pos = i*datasize + start;
      	     	  buffer[i] = (float)(convert.byteToDouble(data, pos));
      	     	  if (buffer[i]>max) 
		      buffer[i] = max;
      	     	  else if (buffer[i]<min)
	  	      	   buffer[i] = min;
	     }
    	     // data converter */
    	     // retVal = dataToImage(buffer, w, h, out);
	     retVal = dataToImage(buffer,min,max, out);
    	     break;
	default:
	     retVal = false;
	}
	
	return retVal;
     }

   /** 
    * convert scientific data to image data
    * @param data the scientific data
    * @param datatype the data type of the scientific data
    * @param range    the specified dataset range[min, max]
    * @param w the width of the converted image
    * @param h the height of the image
    * @param pos the first converted scientific data position
    * @param output the converted image data
    * @return TRUE if successed, or false
    */
    public  static boolean makeImageData(Object data,
		double minVal, double maxVal, 
		int nt, int w, int h, int pos,
		byte out[]) {

	boolean retVal = false;
	
	float min = (float)minVal;
	float max = (float)maxVal;

	// number of data 
 	int nelmt = (int)w*h;
  	int start = (int)pos;

	int size = w*h;
	float buffer[] = new float[size];

	if ((nt & HDFConstants.DFNT_LITEND) != 0) {
		//  this code probably will fail -- use HDFNative?
		nt -= HDFConstants.DFNT_LITEND;
	}

	// where should we  go?
	switch(nt) {
	// one bit char
	case HDFConstants.DFNT_CHAR:
	case HDFConstants.DFNT_UCHAR8:
	case HDFConstants.DFNT_UINT8:  
	     byte bdat[] = (byte[])data;
	     // for each of data value
	     for (int i=0; i<nelmt; i++)  {
      	     	buffer[i] = (float)(bdat[i+start]);
		if (buffer[i]<0) buffer[i] += 256.0f;
      	     	if (buffer[i]>max) 
		    buffer[i] = max;
      	     	else if (buffer[i]<min)
	  	         buffer[i] = min;
	     }
    	     // data converter */
	     for (int i=0; i<nelmt; i++) 
		  out[i] = (byte)buffer[i]; 
	     retVal = true;
    	     break;
	// signed integer (byte)	
	// case HDFConstants.DFNT_UINT8:  
	case HDFConstants.DFNT_INT8:
	     byte b8dat[] = (byte[])data;
	     // for each of data value
	     for (int i=0; i<nelmt; i++)  {
      	     	buffer[i] = (float)(b8dat[i+start]);
      	     	if (buffer[i]>max) 
		    buffer[i] = max;
      	     	else if (buffer[i]<min)
	  	         buffer[i] = min;
	     }
    	     // data converter */
    	     // retVal = dataToImage(buffer,(int)w, (int)h, out); 
	     retVal = dataToImage(buffer,min,max, out);
    	     break;
   	// short	
	case HDFConstants.DFNT_INT16:
	case HDFConstants.DFNT_UINT16:

	     short sdat[] = (short[])data;
	     // for each of data value
	     for (int i=0; i<nelmt; i++)  {

      	     	  buffer[i] = (float)(sdat[i+start]);
      	     	  if (buffer[i]>max) 
		      buffer[i] = max;
      	     	  else if (buffer[i]<min)
	  	      	   buffer[i] = min;
	     }
    	     // data converter */
    	     // retVal = dataToImage(buffer,(int)w, (int)h, out);
	     retVal = dataToImage(buffer,min,max, out);
    	     break;

      	case HDFConstants.DFNT_INT32:
	case HDFConstants.DFNT_UINT32:
	     int idat[] = (int[])data;

	     // for each of data value
	     for (int i=0; i<nelmt; i++)  {
      	     	  buffer[i] = (float)(idat[i+start]);
      	     	  if (buffer[i]>max) 
		      buffer[i] = max;
      	     	  else if (buffer[i]<min)
	  	      	   buffer[i] = min;
	     }
    	     // data converter */
    	     // retVal = dataToImage(buffer,w,h, out);
	     retVal = dataToImage(buffer,min,max, out);
    	     break;
 	case HDFConstants.DFNT_FLOAT32:

	     float fdat[] = (float[])data;
	     // for each of data value
	     for (int i=0; i<nelmt; i++)  {
		  
      	     	  buffer[i] = (float)(fdat[i+start]);
      	     	  if (buffer[i]>max) 
		      buffer[i] = max;
      	     	  else if (buffer[i]<min)
	  	      	   buffer[i] = min;
	     }
    	     // data converter */
    	     // retVal = dataToImage(buffer,w,h, out);
	     retVal = dataToImage(buffer,min,max, out);
    	     break;
 	case HDFConstants.DFNT_FLOAT64:
	     double ddat[] = (double[])data;
	     // for each of data value
	     for (int i=0; i<nelmt; i++)  {
		  
      	     	  buffer[i] = (float)(ddat[i+start]);
      	     	  if (buffer[i]>max) 
		      buffer[i] = max;
      	     	  else if (buffer[i]<min)
	  	      	   buffer[i] = min;
	     }
    	     // data converter */
    	     // retVal = dataToImage(buffer, w, h, out);
	     retVal = dataToImage(buffer,min,max, out);
    	     break;
	default:
	     retVal = false;
	}
	
	return retVal;
     }

   /** 
    * convert scientific data to image data
    * @param data the scientific data
    * @param datatype the data type of the scientific data
    * @param w the width of the converted image
    * @param h the height of the image
    * @param pos the first converted scientific data position
    * @param output the converted image data
    * @return TRUE if successed, or false
    */
    public  static boolean makeImageData(byte data[],
		int nt, int w, int h, int pos,
		byte out[]) {
	boolean retVal = false;
	retVal = makeImageData(data,-1.2E+20d,1.2E+20d,nt,w,h,pos,out);
	return retVal;
     }

   /** 
    * convert scientific data to image data
    * @param data the scientific data
    * @param datatype the data type of the scientific data
    * @param w the width of the converted image
    * @param h the height of the image
    * @param pos the first converted scientific data position
    * @param output the converted image data
    * @return TRUE if successed, or false
    */
    public  static  boolean makeImageData(Object  data,
		int nt, int w, int h, int pos,
		byte out[]) {
	boolean retVal = false;
	retVal = makeImageData(data,-1.2E+20d,1.2E+20d,nt,w,h,pos,out);

	return retVal;
     }

    // convert sci. data into image data
    public  static  boolean makeImageData(byte  data[],
		int nt, int w, int h,
		byte  out[]) {
	boolean retVal = false;
	retVal = makeImageData(data,nt,w,h,0,out);
	return retVal;
     }

    // convert sci. data into image data
    public  static boolean makeImageData(Object  data,
		int nt, int w, int h,
		byte out[]) {
	boolean retVal = false;
	retVal = makeImageData(data,nt,w,h,0,out);
	return retVal;
     }

    // convert sci. data into image data
    public  static boolean makeImageData(byte data[],
		double min, double max,
		int nt, int w, int h,
		byte out[]) {
	boolean retVal = false;
	retVal = makeImageData(data,min,max,nt,w,h,0,out);
	return retVal;
     }

    // convert sci. data into image data
    public  static boolean makeImageData(Object data,
		double min, double max,
		int nt, int w, int h,
		byte out[]) {
	boolean retVal = false;
	retVal = makeImageData(data,min,max,nt,w,h,0,out);
	return retVal;
     }

  /** create the raster image br specified image data
   * @param imgData the image data(pixel value)
   * @param w the width of the image
   * @param h the height of the image
   * @param imgPalette the palette of the image
   * @param index      the plane number of the image
   * @return the image , otherwise null
   */
  public Image createRasterImage(byte[] imgData, int w, int h, byte[] imgPal,int index) {

    if ((w<=0)||(h<=0)||(index<1))
      return null;
    
    // set the created image data
    byte data[] = new byte[w*h];
    
    int pos = (index-1)*w*h;

    int size = w*h;
    for (int i=0; i<size; i++) 	     
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
    IndexColorModel cm = imagePalette.getColorModel();
    Toolkit  toolkit = Toolkit.getDefaultToolkit();
    Image img = toolkit.createImage(new MemoryImageSource(w,h,cm,data,0,w));
  
    return (Image)img;
  }

   /** create the raster image br specified image data
   * @param imgData the image data(pixel value)
   * @param w the width of the image
   * @param h the height of the image
   * @param imgPalette the palette of the image
   * @return the image , otherwise null
   */
  public Image createRasterImage(byte[]  imgData, int w, int h,byte[] imgPal) {

    return(createRasterImage(imgData,w,h,imgPal,1));
  }

   // find the range of a dataset
   public static double[] getDataRange(byte[] data, int nt, int size) {

    HDFNativeData convert = new HDFNativeData();

    // double max = Double.MIN_VALUE;
    // double min = Double.MAX_VALUE;
    double max = -1.2e+20d;
    double min = +1.2e+20d; 

    int datasize = 1;
    try {
      // data size for the original data based on the data number type
      datasize = HDFLibrary.DFKNTsize(nt);
    } catch (HDFException e) {};

    if ((nt & HDFConstants.DFNT_LITEND) != 0) {
	// will this code work now that it is written in java????
	nt -= HDFConstants.DFNT_LITEND;
    }
    int pos = 0;
    double tmp = 0;	
    switch(nt) {
	// one bit char
	case HDFConstants.DFNT_CHAR:
	case HDFConstants.DFNT_UCHAR8:
	case HDFConstants.DFNT_UINT8:
    	// extract the dataset to be processed
    	for (int i=0; i<size; i++) {
	   pos = i*datasize;
	   tmp = (double)((byte)data[pos]);
	  // convert to positive if the number is negative 
	  if (tmp < 0)  
	     tmp += 256.0d;
	  max = Math.max(tmp,max);
	  min = Math.min(tmp,min);
	}
  	break;	
	// signed integer (byte)	
	case HDFConstants.DFNT_INT8:
    	// extract the dataset to be processed
    	for (int i=0; i<size; i++) {
	  pos = i*datasize;	  
	  tmp = (double)((byte)data[pos]);
	  max = Math.max(tmp,max);
	  min = Math.min(tmp,min);
	}
	break;
	  
        // short	
	case HDFConstants.DFNT_INT16:
	case HDFConstants.DFNT_UINT16:
    	// extract the dataset to be processed
    	for (int i=0; i<size; i++) {	  
	  pos = i*datasize;	  
	  tmp = (double)convert.byteToShort(data, pos);
	  max = Math.max(tmp,max);
	  min = Math.min(tmp,min);
	}
	break;
	    
	case HDFConstants.DFNT_INT32:
	case HDFConstants.DFNT_UINT32:
   	// extract the dataset to be processed
    	for (int i=0; i<size; i++) {	  
	  pos = i*datasize;	  		
	  tmp = (double)convert.byteToInt(data, pos);
	  max = Math.max(tmp,max);
	  min = Math.min(tmp,min);
	}
	break;
		  
	//case HDFConstants.DFNT_FLOAT:
	case HDFConstants.DFNT_FLOAT32:
   	// extract the dataset to be processed
    	for (int i=0; i<size; i++) {	  
	  pos = i*datasize;	  			
	  tmp = (double)convert.byteToFloat(data, pos);
	  max = Math.max(tmp,max);
	  min = Math.min(tmp,min);
	}
	break;
	    
	//case HDFConstants.DFNT_DOUBLE:
	case HDFConstants.DFNT_FLOAT64:
   	// extract the dataset to be processed
    	for (int i=0; i<size; i++) {	  
	  pos = i*datasize;	  				
	  tmp = convert.byteToDouble(data, pos);
	  max = Math.max(tmp,max);
	  min = Math.min(tmp,min);
	}
	break;
	
	default:
	  min = 0;
	  max = 0;
    }	   

    double range[] = new double[2];    
    range[0] = min;
    range[1] = max;
    return range;
  } 

   // find the range of a dataset
   public static double[] getDataRange(byte[] data, int nt) {

    HDFNativeData convert = new HDFNativeData();

    // double max = Double.MIN_VALUE;
    // double min = Double.MAX_VALUE;
    double max = -1.2e+20d;
    double min = +1.2e+20d; 

    int datasize = 1;
    try {
      // data size for the original data based on the data number type
      datasize = HDFLibrary.DFKNTsize(nt);
    } catch (HDFException e) {};

    if ((nt & HDFConstants.DFNT_LITEND) != 0) {
	// will this code work now that it is written in java????
	nt -= HDFConstants.DFNT_LITEND;
    }
    int pos = 0;
    double tmp = 0;
    int size = 0;	
    switch(nt) {
	// one bit char
	case HDFConstants.DFNT_CHAR:
	case HDFConstants.DFNT_UCHAR8:
	case HDFConstants.DFNT_UINT8:
	size = data.length/datasize;
    	// extract the dataset to be processed
    	for (int i=0; i<size; i++) {
	   pos = i*datasize;
	   tmp = (double)((byte)data[pos]);
	  // convert to positive if the number is negative 
	  if (tmp < 0)  
	     tmp += 256.0d;
	  max = Math.max(tmp,max);
	  min = Math.min(tmp,min);
	}
  	break;	
	// signed integer (byte)	
	case HDFConstants.DFNT_INT8:
	size = data.length/datasize;
    	// extract the dataset to be processed
    	for (int i=0; i<size; i++) {
	  pos = i*datasize;	  
	  tmp = (double)((byte)data[pos]);
	  max = Math.max(tmp,max);
	  min = Math.min(tmp,min);
	}
	break;
	  
        // short	
	case HDFConstants.DFNT_INT16:
	case HDFConstants.DFNT_UINT16:
	size = data.length/datasize;
    	// extract the dataset to be processed
    	for (int i=0; i<size; i++) {	  
	  pos = i*datasize;	  
	  tmp = (double)convert.byteToShort(data, pos);
	  max = Math.max(tmp,max);
	  min = Math.min(tmp,min);
	}
	break;
	    
	case HDFConstants.DFNT_INT32:
	case HDFConstants.DFNT_UINT32:
	size = data.length/datasize;
   	// extract the dataset to be processed
    	for (int i=0; i<size; i++) {	  
	  pos = i*datasize;	  		
	  tmp = (double)convert.byteToInt(data, pos);
	  max = Math.max(tmp,max);
	  min = Math.min(tmp,min);
	}
	break;
		  
	//case HDFConstants.DFNT_FLOAT:
	case HDFConstants.DFNT_FLOAT32:
	size = data.length/datasize;
   	// extract the dataset to be processed
    	for (int i=0; i<size; i++) {	  
	  pos = i*datasize;	  			
	  tmp = (double)convert.byteToFloat(data, pos);
	  max = Math.max(tmp,max);
	  min = Math.min(tmp,min);
	}
	break;
	    
	//case HDFConstants.DFNT_DOUBLE:
	case HDFConstants.DFNT_FLOAT64:
 	size = data.length/datasize;
  	// extract the dataset to be processed
    	for (int i=0; i<size; i++) {	  
	  pos = i*datasize;	  				
	  tmp = convert.byteToDouble(data, pos);
	  max = Math.max(tmp,max);
	  min = Math.min(tmp,min);
	}
	break;
	
	default:
	  min = 0;
	  max = 0;
    }	   

    double range[] = new double[2];    
    range[0] = min;
    range[1] = max;
    return range;
  } 

   // find the range of a dataset
   public static double[] getDataRange(Object data, int nt, int size) {

    double max = -1.2e+20d;
    double min = +1.2e+20d;

    if ((nt & HDFConstants.DFNT_LITEND) != 0) {
	// will this code work now that it is written in java????
	nt -= HDFConstants.DFNT_LITEND;
    }

    double tmp = 0;	
    switch(nt) {
	// one bit char
	case HDFConstants.DFNT_CHAR:
	case HDFConstants.DFNT_UCHAR8:
	case HDFConstants.DFNT_UINT8:

        // extract the dataset to be processed
       	for (int i=0; i<size; i++) {

	  byte cdat[] = (byte[])data; 
	  tmp = (double)((byte)cdat[i]);
	  // convert to positive if the number is negative 
	  if (tmp < 0)  
	     tmp += 256.0d;	
	  max = Math.max(tmp,max);
	  min = Math.min(tmp,min);
	}
	break;
		
	// signed integer (byte)	
	case HDFConstants.DFNT_INT8:
        // extract the dataset to be processed
       	for (int i=0; i<size; i++) {

	  byte bdat[] = (byte[])data; 
	  tmp = (double)((byte)bdat[i]);
	  max = Math.max(tmp,max);
	  min = Math.min(tmp,min);
	}
	break;
	  
        // short	
	case HDFConstants.DFNT_INT16:
	case HDFConstants.DFNT_UINT16:
        // extract the dataset to be processed
       	for (int i=0; i<size; i++) {
	  short sdat[] = (short[])data;
	  tmp = (double)sdat[i];
	  max = Math.max(tmp,max);
	  min = Math.min(tmp,min);
	}
	break;
	    
	case HDFConstants.DFNT_INT32:
	case HDFConstants.DFNT_UINT32:
        // extract the dataset to be processed
       	for (int i=0; i<size; i++) {
	  int idat[] = (int[])data;	
	  tmp = (double)idat[i];
	  max = Math.max(tmp,max);
	  min = Math.min(tmp,min);
	}
	break;
		  
	//case HDFConstants.DFNT_FLOAT:
	case HDFConstants.DFNT_FLOAT32:
        // extract the dataset to be processed
       	for (int i=0; i<size; i++) {
	  float fdat[] = (float[])data;
	  tmp = (double)fdat[i];
	  max = Math.max(tmp,max);
	  min = Math.min(tmp,min);
	}
	break;
	    
	//case HDFConstants.DFNT_DOUBLE:
	case HDFConstants.DFNT_FLOAT64:
        // extract the dataset to be processed
       	for (int i=0; i<size; i++) {
	  double ddat[] = (double[])data;
	  tmp = ddat[i];
	  max = Math.max(tmp,max);
	  min = Math.min(tmp,min);
	}
	break;
	
	default:
	  max = 0;
	  min = 0;
	}

    double range[] = new double[2];    
    range[0] = min;
    range[1] = max;
    return range;
  } 

   // find the range of a dataset
   public static double[] getDataRange(Object data, int nt) {

    double max = -1.2e+20d;
    double min = +1.2e+20d;
    int size = 0;

    if ((nt & HDFConstants.DFNT_LITEND) != 0) {
	// will this code work now that it is written in java????
	nt -= HDFConstants.DFNT_LITEND;
    }

    double tmp = 0;	
    switch(nt) {
	// one bit char
	case HDFConstants.DFNT_CHAR:
	case HDFConstants.DFNT_UCHAR8:
	case HDFConstants.DFNT_UINT8:
	byte cdat[] = (byte[])data;
	size = cdat.length;
        // extract the dataset to be processed
       	for (int i=0; i<size; i++) {

	  // byte cdat[] = (byte[])data; 
	  tmp = (double)((byte)cdat[i]);
	  // convert to positive if the number is negative 
	  if (tmp < 0)  
	     tmp += 256.0d;	
	  max = Math.max(tmp,max);
	  min = Math.min(tmp,min);
	}
	break;
		
	// signed integer (byte)	
	case HDFConstants.DFNT_INT8:
	byte bdat[] = (byte[])data; 
	size = bdat.length;
        // extract the dataset to be processed
       	for (int i=0; i<size; i++) {

	  // byte bdat[] = (byte[])data; 
	  tmp = (double)((byte)bdat[i]);
	  max = Math.max(tmp,max);
	  min = Math.min(tmp,min);
	}
	break;
	  
        // short	
	case HDFConstants.DFNT_INT16:
	case HDFConstants.DFNT_UINT16:
	short sdat[] = (short[])data;
	size = sdat.length;
        // extract the dataset to be processed
       	for (int i=0; i<size; i++) {
	  // short bdat[] = (short[])data;
	  tmp = (double)sdat[i];
	  max = Math.max(tmp,max);
	  min = Math.min(tmp,min);
	}
	break;
	    
	case HDFConstants.DFNT_INT32:
	case HDFConstants.DFNT_UINT32:
	int idat[] = (int[])data;
	size = idat.length;
        // extract the dataset to be processed
       	for (int i=0; i<size; i++) {
	  // int idat[] = (int[])data;	
	  tmp = (double)idat[i];
	  max = Math.max(tmp,max);
	  min = Math.min(tmp,min);
	}
	break;
		  
	//case HDFConstants.DFNT_FLOAT:
	case HDFConstants.DFNT_FLOAT32:
	float fdat[] = (float[])data;
	size = fdat.length;
        // extract the dataset to be processed
       	for (int i=0; i<size; i++) {
	  // float fdat[] = (float[])data;
	  tmp = (double)fdat[i];
	  max = Math.max(tmp,max);
	  min = Math.min(tmp,min);
	}
	break;
	    
	//case HDFConstants.DFNT_DOUBLE:
	case HDFConstants.DFNT_FLOAT64:
	double ddat[] = (double[])data;
	size = ddat.length;
        // extract the dataset to be processed
       	for (int i=0; i<size; i++) {
	  // double ddat[] = (double[])data;
	  tmp = ddat[i];
	  max = Math.max(tmp,max);
	  min = Math.min(tmp,min);
	}
	break;
	
	default:
	  max = 0;
	  min = 0;
	}

    double range[] = new double[2];    
    range[0] = min;
    range[1] = max;
    return range;
  } 

 /** Return the RAINBOW palette 
   * rgb rgb rgb rgb rgb ....
   */
 public  static byte[] getPaletteOfRainbow()  {

	int[] rainbowValues = {  

   // rgb,rgb, ......
   0x00, 0x00, 0x00, 0x7c, 0x00, 0xff, 0x78, 0x00, 0xfe, 0x73, 0x00, 0xff,
   0x6f, 0x00, 0xfe, 0x6a, 0x00, 0xff, 0x66, 0x00, 0xfe, 0x61, 0x00, 0xff,
   0x5d, 0x00, 0xfe, 0x58, 0x00, 0xff, 0x54, 0x00, 0xfe, 0x4f, 0x00, 0xff,
   0x4b, 0x00, 0xfe, 0x46, 0x00, 0xff, 0x42, 0x00, 0xfe, 0x3d, 0x00, 0xff,
   0x39, 0x00, 0xfe, 0x34, 0x00, 0xff, 0x30, 0x00, 0xfe, 0x2b, 0x00, 0xff,
   0x27, 0x00, 0xfe, 0x22, 0x00, 0xff, 0x1e, 0x00, 0xfe, 0x19, 0x00, 0xff,
   0x15, 0x00, 0xfe, 0x10, 0x00, 0xff, 0x0c, 0x00, 0xfe, 0x07, 0x00, 0xff,
   0x03, 0x00, 0xfe, 0x00, 0x02, 0xff, 0x00, 0x06, 0xfe, 0x00, 0x0b, 0xff,
   0x00, 0x0f, 0xfe, 0x00, 0x14, 0xff, 0x00, 0x18, 0xfe, 0x00, 0x1d, 0xff,
   0x00, 0x21, 0xfe, 0x00, 0x26, 0xff, 0x00, 0x2a, 0xfe, 0x00, 0x2f, 0xff,
   0x00, 0x33, 0xfe, 0x00, 0x38, 0xff, 0x00, 0x3c, 0xfe, 0x00, 0x41, 0xff,
   0x00, 0x45, 0xfe, 0x00, 0x4a, 0xff, 0x00, 0x4e, 0xfe, 0x00, 0x53, 0xff,
   0x00, 0x57, 0xfe, 0x00, 0x5c, 0xff, 0x00, 0x60, 0xfe, 0x00, 0x65, 0xff,
   0x00, 0x69, 0xfe, 0x00, 0x6e, 0xff, 0x00, 0x72, 0xfe, 0x00, 0x77, 0xff,
   0x00, 0x7a, 0xfe, 0x00, 0x80, 0xff, 0x00, 0x83, 0xfe, 0x00, 0x89, 0xff,
   0x00, 0x8c, 0xfe, 0x00, 0x92, 0xff, 0x00, 0x95, 0xfe, 0x00, 0x9b, 0xff,
   0x00, 0x9e, 0xfe, 0x00, 0xa4, 0xff, 0x00, 0xa7, 0xfe, 0x00, 0xad, 0xff,
   0x00, 0xb0, 0xfe, 0x00, 0xb6, 0xff, 0x00, 0xb9, 0xfe, 0x00, 0xbf, 0xff,
   0x00, 0xc2, 0xfe, 0x00, 0xc8, 0xff, 0x00, 0xcb, 0xfe, 0x00, 0xd1, 0xff,
   0x00, 0xd4, 0xfe, 0x00, 0xda, 0xff, 0x00, 0xdd, 0xfe, 0x00, 0xe3, 0xff,
   0x00, 0xe6, 0xfe, 0x00, 0xec, 0xff, 0x00, 0xf0, 0xfe, 0x00, 0xf5, 0xff,
   0x00, 0xf9, 0xfe, 0x00, 0xfe, 0xff, 0x00, 0xfe, 0xfa, 0x00, 0xff, 0xf7,
   0x00, 0xfe, 0xf1, 0x00, 0xff, 0xee, 0x00, 0xfe, 0xe8, 0x00, 0xff, 0xe5,
   0x00, 0xfe, 0xdf, 0x00, 0xff, 0xdc, 0x00, 0xfe, 0xd6, 0x00, 0xff, 0xd3,
   0x00, 0xfe, 0xcd, 0x00, 0xff, 0xca, 0x00, 0xfe, 0xc4, 0x00, 0xff, 0xc1,
   0x00, 0xfe, 0xbb, 0x00, 0xff, 0xb8, 0x00, 0xfe, 0xb2, 0x00, 0xff, 0xaf,
   0x00, 0xfe, 0xa9, 0x00, 0xff, 0xa6, 0x00, 0xfe, 0xa0, 0x00, 0xff, 0x9d,
   0x00, 0xfe, 0x97, 0x00, 0xff, 0x94, 0x00, 0xfe, 0x8e, 0x00, 0xff, 0x8b,
   0x00, 0xfe, 0x85, 0x00, 0xff, 0x82, 0x00, 0xfe, 0x7d, 0x00, 0xff, 0x79,
   0x00, 0xfe, 0x74, 0x00, 0xff, 0x70, 0x00, 0xfe, 0x6b, 0x00, 0xff, 0x67,
   0x00, 0xfe, 0x62, 0x00, 0xff, 0x5e, 0x00, 0xfe, 0x59, 0x00, 0xff, 0x55,
   0x00, 0xfe, 0x50, 0x00, 0xff, 0x4c, 0x00, 0xfe, 0x47, 0x00, 0xff, 0x43,
   0x00, 0xfe, 0x3e, 0x00, 0xff, 0x3a, 0x00, 0xfe, 0x35, 0x00, 0xff, 0x31,
   0x00, 0xfe, 0x2c, 0x00, 0xff, 0x28, 0x00, 0xfe, 0x23, 0x00, 0xff, 0x1f,
   0x00, 0xfe, 0x1a, 0x00, 0xff, 0x16, 0x00, 0xfe, 0x11, 0x00, 0xff, 0x0d,
   0x00, 0xfe, 0x08, 0x00, 0xff, 0x04, 0x01, 0xfe, 0x00, 0x05, 0xff, 0x00,
   0x0a, 0xfe, 0x00, 0x0e, 0xff, 0x00, 0x13, 0xfe, 0x00, 0x17, 0xff, 0x00,
   0x1c, 0xfe, 0x00, 0x20, 0xff, 0x00, 0x25, 0xfe, 0x00, 0x29, 0xff, 0x00,
   0x2e, 0xfe, 0x00, 0x32, 0xff, 0x00, 0x37, 0xfe, 0x00, 0x3b, 0xff, 0x00,
   0x40, 0xfe, 0x00, 0x44, 0xff, 0x00, 0x49, 0xfe, 0x00, 0x4d, 0xff, 0x00,
   0x52, 0xfe, 0x00, 0x56, 0xff, 0x00, 0x5b, 0xfe, 0x00, 0x5f, 0xff, 0x00,
   0x64, 0xfe, 0x00, 0x68, 0xff, 0x00, 0x6d, 0xfe, 0x00, 0x71, 0xff, 0x00,
   0x76, 0xfe, 0x00, 0x7b, 0xff, 0x00, 0x7e, 0xfe, 0x00, 0x84, 0xff, 0x00,
   0x87, 0xfe, 0x00, 0x8d, 0xff, 0x00, 0x90, 0xfe, 0x00, 0x96, 0xff, 0x00,
   0x99, 0xfe, 0x00, 0x9f, 0xff, 0x00, 0xa2, 0xfe, 0x00, 0xa8, 0xff, 0x00,
   0xab, 0xfe, 0x00, 0xb1, 0xff, 0x00, 0xb4, 0xfe, 0x00, 0xba, 0xff, 0x00,
   0xbd, 0xfe, 0x00, 0xc3, 0xff, 0x00, 0xc6, 0xfe, 0x00, 0xcc, 0xff, 0x00,
   0xcf, 0xfe, 0x00, 0xd5, 0xff, 0x00, 0xd8, 0xfe, 0x00, 0xde, 0xff, 0x00,
   0xe1, 0xfe, 0x00, 0xe7, 0xff, 0x00, 0xea, 0xfe, 0x00, 0xf0, 0xff, 0x00,
   0xf3, 0xfe, 0x00, 0xf9, 0xff, 0x00, 0xfc, 0xfe, 0x00, 0xff, 0xfc, 0x00,
   0xfe, 0xf7, 0x00, 0xff, 0xf3, 0x00, 0xfe, 0xee, 0x00, 0xff, 0xea, 0x00,
   0xfe, 0xe5, 0x00, 0xff, 0xe1, 0x00, 0xfe, 0xdc, 0x00, 0xff, 0xd8, 0x00,
   0xfe, 0xd3, 0x00, 0xff, 0xcf, 0x00, 0xfe, 0xca, 0x00, 0xff, 0xc6, 0x00,
   0xfe, 0xc1, 0x00, 0xff, 0xbd, 0x00, 0xfe, 0xb8, 0x00, 0xff, 0xb4, 0x00,
   0xfe, 0xaf, 0x00, 0xff, 0xab, 0x00, 0xfe, 0xa6, 0x00, 0xff, 0xa2, 0x00,
   0xfe, 0x9d, 0x00, 0xff, 0x99, 0x00, 0xfe, 0x94, 0x00, 0xff, 0x90, 0x00,
   0xfe, 0x8b, 0x00, 0xff, 0x87, 0x00, 0xfe, 0x83, 0x00, 0xff, 0x7e, 0x00,
   0xfe, 0x7a, 0x00, 0xff, 0x75, 0x00, 0xfe, 0x71, 0x00, 0xff, 0x6c, 0x00,
   0xfe, 0x68, 0x00, 0xff, 0x63, 0x00, 0xfe, 0x5f, 0x00, 0xff, 0x5a, 0x00,
   0xfe, 0x56, 0x00, 0xff, 0x51, 0x00, 0xfe, 0x4d, 0x00, 0xff, 0x48, 0x00,
   0xfe, 0x44, 0x00, 0xff, 0x3f, 0x00, 0xfe, 0x3b, 0x00, 0xff, 0x36, 0x00,
   0xfe, 0x32, 0x00, 0xff, 0x2d, 0x00, 0xfe, 0x29, 0x00, 0xff, 0x24, 0x00,
   0xfe, 0x20, 0x00, 0xff, 0x1b, 0x00, 0xfe, 0x17, 0x00, 0xff, 0x12, 0x00,
   0xfe, 0x0e, 0x00, 0xff, 0x09, 0x00, 0xff, 0x05, 0x00, 0xff, 0xff, 0xff };

	byte[] retVal = new byte[768];
	for (int kk=0; kk<768; kk++) 
	    retVal[kk] = (byte)rainbowValues[kk];
	return retVal;
 }
 

 /* Return the RAINBOW palette 
   * rgb rgb rgb rgb rgb ....
   */
 public  static ColorModel getColorModelOfRainbow()  {

 	byte[]  values;	
	values = getPaletteOfRainbow();

 	// number of color
	int  ncolors = 256;
	
	// set the RGB
    	byte[] red   = new byte[256];
    	byte[] green = new byte[256];
    	byte[] blue  = new byte[256];

	for (int i=0; i<256; i++ ) {
	
	    red[i]  = (byte)values[i*3  ];
	    green[i]= (byte)values[i*3+1];
	    blue[i] = (byte)values[i*3+2];
	}

	return new IndexColorModel(8, 256, red, green, blue);
 }

  // Return the  data by index
  public static float  getData(Object data, int nt, int pos)  {

    float retDat = 0;
    int dt = nt;
    if ((dt & HDFConstants.DFNT_LITEND) != 0) {
      dt -= HDFConstants.DFNT_LITEND;
    }
 
    switch(dt) {

	// one bit char
	case HDFConstants.DFNT_CHAR:
	case HDFConstants.DFNT_UCHAR8:
	case HDFConstants.DFNT_UINT8:
	  byte bdat[] = (byte[])data;
	  retDat = (float)(bdat[pos]);
	  // convert to positive if the number is negative 
	  if (retDat < 0)  
	     retDat += 256.0;
	  break;
		
	// signed integer (byte)	
	case HDFConstants.DFNT_INT8:
	  byte bidat[] = (byte[])data;
	  retDat = (float)(bidat[pos]);
	  break;
	  
        // short	
	case HDFConstants.DFNT_INT16:
	case HDFConstants.DFNT_UINT16:
	  short sdat[] = (short[])data;   
	  retDat = (float)sdat[pos];
	  break;
	    
	case HDFConstants.DFNT_INT32:
	case HDFConstants.DFNT_UINT32:
	  int idat[] = (int[])data;	
	  retDat = (float)idat[pos];
	  break;
		  
	//case HDFConstants.DFNT_FLOAT:
	case HDFConstants.DFNT_FLOAT32:
	  float fdat[] = (float[])data;
	  retDat = (float)fdat[pos];
	  break;
	    
	//case HDFConstants.DFNT_DOUBLE:
	case HDFConstants.DFNT_FLOAT64:
	  double ddat[] = (double[])data;
	  retDat = (float)ddat[pos];
	  break;
	
	default:
	  retDat = 0;
    }
 
   return retDat;
  }

}
