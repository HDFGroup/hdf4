package ncsa.hdf.jhv;

import ncsa.hdf.hdflib.*;
/** Convert scientific data into image data. 
 * @version 1.00 	9/12/97
 * @auther  Xinjian Lu
 */
public class ImageDataConverter {

   // default constructor 
   public ImageDataConverter() {
    }

     // convert data into image
     private static boolean dataToImage(float[] input, int w, int h, byte[] output) {
 
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
    public static boolean makeImageDataByRange(byte[] data,
		double minVal, double maxVal, 
		int nt, int w, int h, int pos,
		byte[] out) {

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
    	     retVal = dataToImage(buffer,(int)w, (int)h, out);
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
    	     retVal = dataToImage(buffer,(int)w, (int)h, out);
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
    	     retVal = dataToImage(buffer,(int)w, (int)h, out);
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
    	     retVal = dataToImage(buffer,(int)w, (int)h, out);
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
    	     retVal = dataToImage(buffer,(int)w, (int)h, out);
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
    public static boolean makeImageData(byte[] data,
		int nt, int w, int h, int pos,
		byte[] out) {
	boolean retVal = false;
	retVal = makeImageDataByRange(data,-1.2E+20d,Double.MAX_VALUE,nt,w,h,pos,out);
	return retVal;
     }

    // convert sci. data into image data
    public static boolean makeImageData(byte[] data,
		int nt, int w, int h,
		byte[] out) {
	boolean retVal = false;
	retVal = makeImageData(data,nt,w,h,0,out);
	return retVal;
     }

    // convert sci. data into image data
    public static boolean makeImageDataByRange(byte[] data,
		double min, double max,
		int nt, int w, int h,
		byte[] out) {
	boolean retVal = false;
	retVal = makeImageDataByRange(data,min,max,nt,w,h,0,out);
	return retVal;
     }
}
