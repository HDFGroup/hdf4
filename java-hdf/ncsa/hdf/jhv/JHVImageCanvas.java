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

import java.util.*;
import java.util.Hashtable;
import java.awt.image.*;
import java.awt.*;
import java.awt.event.*;

import ncsa.hdf.hdflib.*;
import ncsa.hdf.palette.*;
import ncsa.hdf.java.awt.image.*;
import ncsa.hdf.java.awt.*;
import ncsa.hdf.java.awt.event.*;

import ncsa.hdf.plots.*;

/* This class implement full resolution image from HDF process,
  * by implementing the PaletteEditorAdapter interface the image
  * palette should be modified through  generic "Palette Editor"
  * implemented in seprate package -xlu 7/97 */

/** The image will be display on this canvas */
public class JHVImageCanvas extends Canvas
       implements PaletteEditorAdapter, Runnable,
                MouseListener, MouseMotionListener {

  /** the frame to involve the image canvas component */
  JHVImageFrame	imageFrame;
  
  /** HDF Browser applet */	
  JHV 		app;
 
  /** the canvas resonding to the selected HDF node  */
  JHVCanvas	jhvCanvas;

  /** HDF Object */
  HDFLibrary		hdf;

  /** the HDF file identifier */
  int 		fid = -1;

  /** the selected HDF Object node */
  HDFObjectNode	node;

  /** the current image */
  Image		image    = null;
  Image		preImage = null;

  /** a banch of images  for animation */
  Image[] images;

  /** image size  */
  int		imageWidth,	imageHeight;

  /** the raster image pixel value */
  byte 		imageData[];

  /** the palette value for current image */
  byte		imagePalette[];

  /** the physical image data */
  byte[] 		hdfData;

  // data range
  boolean 	dataRangeFlag  = false;
  // double[]	dataRange = new double[2];
  double min;
  double max;

  /** the data number type */
  int		hdfDataType = -1;

  /**   the total number of image */
  int 		numberOfImage = 1;
  
  /** current displayed image number */	
  int 		imageNumber = 1;
  
  /** canvas size */	
  int 		canvasWidth, canvasHeight;

  /** the indicator which the image frame has been popuped */
  boolean		imageFrameDisplayed ;

  /** information for image canvas */
  static String infoStr = "Image Canvas";
  	
  /** variables for duble-buffer  */
  Image 	offScreenImage = null;
  Graphics 	offGraphics;

  /** control of the annimation  */
  int		actionFlag;

  /** the speed of animation */
  int 		speed = 0;

  /** the current thread for animation */
  Thread 		killer;
 
  /** the first coordinate value  of the displayed image */
  int startx = 1, starty = 1;
    
  /** translated variable for scroll panel */
  int tx=0,  ty=0;       // offset translate ..
    
  /** Scrollbar of the image canvas */
  Scrollbar      hScrollbar,      vScrollbar;
    
  /** the indicator that the mouse has been draged */
  boolean  dragFlag  =  false;

  /** the indicator of the mouse position */
  boolean mouseOnImageFlag = false;
    
  /** the rectangle of the valid draged area */
  Rectangle  dragArea; 

  /** the rectangle of the current draged area */
  Rectangle  currentDragArea;
        
  /** the draged image area */
  Rectangle  imageArea = new Rectangle();
    
  /** the dataset  range for the selected area */
  Rectangle  datasetRange;
    
  /** the mouse position */
  int mx = 0, my = 0;
    
  /** draw the subset area flag */
  boolean  drawSubsetFlag = false;
    
  /** draw subset area */
  Rectangle  drawSubsetArea = new Rectangle();

  // for image palette
  public boolean 	paletteEditorOpen	= false;

  // for image color model
  public ColorModel  	colorModel    = null;
  public ColorModel  	oldColorModel = null;

  // for current color model palette
  public byte[] colorModelPalette  = null;

  // image producer ...
  public ImageProducer     imageSource    = null;
  public ImageProducer[]   imageSources   = null;


  // PaletteEditor instance ....
  PaletteEditor paletteEditor;

  // indicator to display the coordinates.
  boolean	showCoordinate = false;

  // coordinate info.
  String	coorInfo = "";

  // subset range of an image
  Rectangle 	subsetRange;

  // display image value mode, pixel or digital value
  int		dispImageDataMode = JHVImageFrame.NONE_VALUE;
  int		imagePlotMode     = JHVImageFrame.NONE_PLOT;

  // plot of  histogram and radial plot
  XYPlot	aPlot, hPlot;

  /** new constructor for image canvas
   * @param frame the frame of this object
   */
  public JHVImageCanvas(JHVImageFrame frame) {

    // set  image frame
    imageFrame = frame;
	
    // set applet
    app = frame.applet_;
	
    imageFrameDisplayed = false;

    // get hdf object
    hdf = app.hdf;

    // get file id.
    fid = app.fid;

    addMouseListener(this);
    addMouseMotionListener(this);
  }

  /** new constructor for image canvas
   * @param frame the frame of this object
   */
  public JHVImageCanvas(JHVImageFrame frame, Rectangle subset) {

    this(frame);

    // keep subset image range
    this.subsetRange = subset;
  }

  /**
   * Initialize  HDF object based on the selected node
   */
  public boolean initHDF() {

    // set returned value
    boolean retVal = true;

    // disable adjustMenuItem
    imageFrame.adjustMenuItem.setEnabled(false);

    try {
      switch(node.type) {
      case HDFObjectNode.RIS8:
	readRIS8(app.hdfFile);
	break;
      case HDFObjectNode.RIS24:
	readRIS24(app.hdfFile);	     
	break;
      case HDFObjectNode.GRDATASET:
	readGR(app.fid);
	break;
      case HDFObjectNode.SDSDATASET:
	
	initSDS();
	
	// enable adjustMenuItem
	imageFrame.adjustMenuItem.setEnabled(true);
	break;
	
      case HDFObjectNode.Vdata:
      case HDFObjectNode.Vgroup:
	break;
      }
    } catch (HDFException e) {};
    
    // set orignal image source
    imageSources = new ImageProducer[numberOfImage];
    for (int kk=0; kk<numberOfImage; kk++)
	imageSources[kk] = images[kk].getSource();
    
    return retVal;
  }
  
  

  /** Initialize the HDF sds */ 
  public void initSDS() throws HDFException {
    
    /****************************************************************
      Date: 3-12-97
      Method , initSDS(), was modified so that we can created SDS 
      converted image by specified dataset range if SDS has an dataset range.
      The dataset range may be smaller than the actual dataset range, fill the 
      value if bigger or smaller than specified dataset range.  Native method
      makeImageDataByRange() take charge of that. 
      Date: 9-16-97
      Native method makeImageDataByRange() has been taken off and 
      **************************************************************/
    HDFLibrary hdf = app.hdf;
    
    // specify imageData;
    imageData = new byte[imageHeight*imageWidth];
    
    if(readSDS(app.hdfFile, 1)) {
      
  	// make image data
      	boolean cvFlag = true;
      	if (dataRangeFlag) {
	  cvFlag = ImageDataConverter.makeImageDataByRange( hdfData, min, max,
			     hdfDataType, imageWidth, imageHeight, (int)0,  imageData);
        }
        else {
	  cvFlag = ImageDataConverter.makeImageData(hdfData,hdfDataType,
				     imageWidth,imageHeight,
			       		 0, imageData);
	}
	
	if (cvFlag) { // succed to convert dataset.
	  // the following code make me confused, I don't know  why I can't get image 
	  // painted under Win95. However if I changed pixel value for "pixel[0]" 
	  // to zero(0), then it works.    
	  // try to get OS name
	  String osName = System.getProperties().getProperty("os.name");
	  if (osName.toUpperCase().indexOf("WINDOW") != -1) // window 95 or nt
	    imageData[0] = 0;
	  
	  // create the raster image 
	  image = createRasterImage(imageData,imageWidth,imageHeight,
				    imagePalette,1);
	}
    }
    
    // specify the Image 
    images = new Image[numberOfImage];
    
    // Assign the image
    images[0] = image;
    
    // if the sds is 3-d
    for (int i=2; i<=numberOfImage; i++) {
      
      // i-th plane
      if(readSDS(app.hdfFile, i)) {

	// convert identifier
    	boolean cvFlag = true;
      	if (dataRangeFlag) {   
	
	  cvFlag = ImageDataConverter.makeImageDataByRange(hdfData,min, max,
			hdfDataType, imageWidth,imageHeight,
					    0, imageData);
        }
        else {
	  cvFlag = ImageDataConverter.makeImageData(hdfData,hdfDataType,
				     imageWidth,imageHeight,
				     0, imageData);
	}
	
 	if (cvFlag) { // succed to convert dataset.
	  
	  // the following code make me confused, I don't know  why I can't get image 
	  // painted under Win95. However if I changed pixel value for "pixel[0]" 
	  // to zero(0), then it works.    
	  // try to get OS name
	  String osName = System.getProperties().getProperty("os.name");
	  if (osName.toUpperCase().indexOf("WINDOW") != -1) // window 95 or nt
	    imageData[0] = 0;
	  
	  // create raster image for which plane number is i
	  images[i-1] = createRasterImage(imageData,imageWidth,imageHeight,
					  imagePalette,1);
        }
      }
    }// for (int i=2; i<=numberOfImage; i++) {
 
    // repaint image
    setImage(images[0]);

  }
  

  /** read 8-raster  image data from the HDF file 
   * @param filename the HDF file name
   */
  public void readRIS8(String filename) throws HDFException {

    HDFLibrary hdf = new HDFLibrary();
    
    // set the reference number
    if (hdf.DFR8readref(filename, node.ref) == false)
      return;

    int[] args= new int[2];
    boolean[] barg= new boolean[1];
    // get HDF image information
    if (hdf.DFR8getdims(filename, args, barg) == false)
      return;
	
    int w = args[0];
    int h = args[1];
    boolean hasPalette = barg[0];

    imageData     = new byte[w*h];

    if (imagePalette == null)
       imagePalette  = new byte[256*3];
	
    boolean readFlag;
    
    // read the image
    readFlag = hdf.DFR8getimage(filename, imageData,w,h,imagePalette);
    
    // set data type;
    this.hdfDataType = HDFConstants.DFNT_UINT8;

    // set image palette
    if (hasPalette == false) {
      for (int i=0; i<256; i++)
	for (int j=0; j<3; j++)  
	  imagePalette[i*3+j] = (byte)i;
    }
    
    // set raw data
    hdfData = imageData;
    hdfDataType = HDFConstants.DFNT_UINT8;
	
    // set number of the image
    setNumberOfImage(1);

    // for animation
    images = new Image[1];

    for (int i=1; i<=1 ;i++)  {
      images[i-1] = createRasterImage(imageData, imageWidth, imageHeight,
				      imagePalette,i);	  
    }
    setImage(images[0]);
  }

  /** read 24-raster  image data from the HDF file 
   * @param filename the HDF file name
   */
  public void readRIS24(String filename) throws HDFException {

    HDFLibrary hdf = new HDFLibrary();
	
    // set the reference number
    if (hdf.DF24readref(filename, node.ref) == false)
      return;
    
    int argv[] = new int[3];
     // get HDF image information
    if (hdf.DF24getdims(filename,argv) == false)
      return;
    int w = argv[0];
    int h = argv[1];
    int interlace = argv[2];
    
    imageData     = new byte[w*h*3];
    imagePalette  = new byte[256*3];
    
    boolean readFlag;
    // set the interlace for reading by component interlacing scheme
    hdf.DF24reqil(HDFConstants.MFGR_INTERLACE_COMPONENT);
    
    // read the image
    readFlag = hdf.DF24getimage(filename, imageData,w,h);
    
    // set data type;
    this.hdfDataType = HDFConstants.DFNT_UINT8;
    
    // set raw data
    hdfData = imageData;
    
    // set image palette
    boolean hasPalette = false;
    
    if (hasPalette == false) {
      for (int i=0; i<256; i++)
	for (int j=0; j<3; j++)  
	  imagePalette[i*3+j] = (byte)i;
    }
	
	// ??? why is this hard coded to be 3D????  REMcG 6.18.97 

    // set number of the image
    setNumberOfImage(3);
    
    // for animation
    images = new Image[3];
    
    for (int i=1; i<=3 ;i++)  
      images[i-1] = createRasterImage(imageData, imageWidth, imageHeight,
				      imagePalette,i);	
    }

    /** Read  HDF file contains generic raster image
      * @param fid the HDF file identifier
      */
    public void readGR(int fid) throws HDFException {
      int grid = -1;
      grid = hdf.GRstart(fid);
      if (grid == HDFConstants.FAIL) {
	hdf.GRend(grid);
	return;
      }
      readHdfGRimage(grid,node);
      hdf.GRend(grid);
    }

  /** read the HDF file generic raster image in the image canvas
   * @param grid the GR identifier
   * @param node the HDF Object node
   */
  public void readHdfGRimage(int grid, HDFObjectNode node) throws HDFException {
	
    int riid      = -1;
    String retStr = null;	
    
    // get the reference 
    int ref = node.ref;
    
    int index = hdf.GRreftoindex(grid, (short)ref);
    
    if (index == HDFConstants.FAIL)  {
      retStr = new String ("GRreftoindex: FAIL");
    }
    else {
      //index = 0;
      if ((riid = hdf.GRselect(grid,index)) == HDFConstants.FAIL) {
	hdf.GRendaccess(riid);
	retStr = new String("GRselect: FAIL");
      }
      else { 
	// get image info.
	String gr_name = new String(" ");
	String[] n = new String[1];
	n[0] = gr_name;

	int[]dim_sizes = new int[2];  /* ??? */
	int [] imInfo = new int[4];

	if (hdf.GRgetiminfo( riid, n, imInfo, dim_sizes) ) {
		gr_name = n[0];
		  // read HDF GR image data
		readHdfGRimageData(riid, imInfo[0], imInfo[1], imInfo[2], dim_sizes);
	}
      } 
    }
    hdf.GRendaccess(riid);
  }

  /** Read the HDF  generic raster image  
   * @param riid the raster image identifier 
   * @param imgInfo the image info. array
   */
  public void readHdfGRimageData(int riid, int ncomp, int nt, int interlace,
                int []dim_sizes) throws HDFException {

    // set image palette
    boolean hasPalette = false;
    
    // image width
    // int w = dim_sizes[0];
    int w = subsetRange.width;

    // image height;
    // int h = dim_sizes[1];
    int h = subsetRange.height;

    // image data size;
    int dataSize = w*h*ncomp*hdf.DFKNTsize(nt);
    
    // specify the image data storage	
    hdfData   = new byte[dataSize];
    
    // image palette (default -- easy to process)
    imagePalette  = new byte[3*256];
    
    // set the interlace for reading by component interlacing scheme
    if (hdf.GRreqimageil(riid, HDFConstants.MFGR_INTERLACE_COMPONENT) == false)
      return ;
	
    // get palette info.
    int lutid = hdf.GRgetlutid(riid, 0);
    
    if (lutid == HDFConstants.FAIL)
      hasPalette = false;
    else
      hasPalette = true;
    
    int[] lutInfo = new int[4];
    // Reminder to myselef.
    // GRgetlutinfo() does work when having aa access to the HDF file
    // created by DFR8 or DF24 interface.
    /*******************************************************
    if( hdf.GRgetlutinfo( lutid, lutInfo)) {
      hasPalette = true;
    } else {
      hasPalette = false;
    }
    
    // easy to process
    if ((lutInfo[0] != 3) || (lutInfo[3] != 256)) {
      hasPalette = false;
    } else {
      hasPalette = true;
    }
 
    // assign the palette
    //int lutSize = lutInfo[0] * lutInfo[3] * hdf.DFKNTsize(lutInfo[1]);
    //char[] lutDat = new char[lutSize];
    *******************************************************/

    // set interlace to read	
    hdf.GRreqlutil(riid, HDFConstants.MFGR_INTERLACE_PIXEL);	
    
    if ((hasPalette) && (hdf.GRreadlut(lutid, imagePalette))) {
      
      // get palette (easy processing)
      ;  	
    }
    else  { // default	   
      // try rainbow
      imagePalette = null;
      imagePalette = getPaletteOfRainbow();
    }
    
    // read image data	
    int start[] = new int[2];
    int stride[]= new int[2];
    int count[] = new int[2];
     
    start[0] = subsetRange.x;
    start[1] = subsetRange.y;
 
     stride[0] = 1;
    stride[1] = 1;
    
    // count[0] = w   ;
    // count[1] = h   ;
    count[0] = subsetRange.width;
    count[1] = subsetRange.height;
    
    // read flag
    boolean readFlag = hdf.GRreadimage(riid,start,stride,count,hdfData);
    
    // set data type;
    this.hdfDataType = nt;
    
    // set number of the image
    setNumberOfImage(ncomp);
    
    // for animation
    images = new Image[ncomp];
    
    // specify imageData;
    imageData = new byte[imageHeight*imageWidth];
    
    for (int i=1; i<=numberOfImage ;i++) {
      if (ImageDataConverter.makeImageData(hdfData,hdfDataType,
			      imageWidth,imageHeight,
			      (imageWidth*imageHeight*(i-1)),
			      imageData)) {
	images[i-1] = createRasterImage(imageData, imageWidth, imageHeight,
					imagePalette,1);	
	}
    }
    // repaint image
    setImage(images[0]);   
  }
  

  /** Read  the first plane of the HDF contains SDS.
   * @param filename the HDF file name
   * @return true if succeed, or false
   */
  public boolean readSDS(String filename) throws HDFException {
    return(readSDS(filename, 1));
  }
  

  /** Read  HDF file contains SDS
   * @param filename the HDF file name
   * @param plane    the plane number
   * @return true if succeed, or false;
   */
  public boolean readSDS(String filename, int plane) throws HDFException {
  
    // get ready to read the HDF SDS interface
    int sdid  = -1;
    
    sdid = hdf.SDstart(filename, HDFConstants.DFACC_RDONLY);
    
    if (sdid == HDFConstants.FAIL) {
      hdf.SDend(sdid);
      return false;
    }
    boolean readFlag = readHdfSDSimage(sdid,node.ref,plane);
    hdf.SDend(sdid);
    return (readFlag);
  }

  /** Read  HDF file contains SDS
   * @param sdid     the HDF file sds identifier  
   * @param plane    the plane number
   * @return true if succeed, or false
   */
  public boolean readSDS(int sdid, int plane) throws HDFException {

    if (sdid == HDFConstants.FAIL)  
      return false;
    else
      return(readHdfSDSimage(sdid,node.ref,plane));  
  }

  /**
   * Read hdf dataset by the specified plane number
   * @param planeNumber the image plane number
   * @return true if succeed, otherwise false;
   */
  public boolean readDataset(int planeNumber) {
      
    // set number of the image
    this.imageNumber = planeNumber;
    
    // return value
    boolean retVal = true;
    try {
      switch(node.type) {
      case HDFObjectNode.RIS8:
      case HDFObjectNode.RIS24:
	
	break;
	
      case HDFObjectNode.GRDATASET:
	
	break;
	
      case HDFObjectNode.SDSDATASET:
	
	// read data
	if(readSDS(app.hdfFile, planeNumber))  {
	 retVal = ImageDataConverter.makeImageData(hdfData,hdfDataType,
				    imageWidth,imageHeight,
				    0,   // start position
				    imageData);
	} else {
	  retVal = false;
	}
	break;

      case HDFObjectNode.Vdata:
      case HDFObjectNode.Vgroup:
	break;
      }
      } catch (HDFException e) {};
      return retVal;
  }
  

  /**
   * set the number of the image
   * @param num the number of the image
   */
  public void setNumberOfImage(int num) {
    numberOfImage = num;
  }
  

  /** read the HDF file SDS and converted to image  
   * @param sdid the SDS identifier
   * @param ref  the reference number
   * @param plane the plane number
   * return true, or false
   */
  public boolean readHdfSDSimage(int sdid, int ref, int plane)  throws HDFException {
    
    /****************************************************************
      Date: 3-12-97
      Add on line to  read the dataset range so that we can create image
      by dataset range if dataset range is exist! 
      Date: 6-2-97
      Add a possiblity to handle subset process of an SDS.
    **************************************************************/
    
    boolean retVal = false;
    int sdsid      = -1;
    String retStr  = null;	
    
    // ref. to index;
    int index = hdf.SDreftoindex(sdid, ref);
    
    if (index == HDFConstants.FAIL)  {
      retStr = new String ("SDreftoindex: FAIL");
    }
    else {
      //index = 0;
      if ((sdsid = hdf.SDselect(sdid,index)) == HDFConstants.FAIL) {
	// fail to SDselect
	hdf.SDendaccess(sdsid);
	retStr = new String("SDselect: FAIL");
      }
      else { 
	
	// get sds info.
	int[] SDInfo = new int[3];
	String name = new String(" ");
	String[] ss = new String[1];
	ss[0] = name;
	int  dimsize[]     = new int[16];
	if (hdf.SDgetinfo(sdsid, ss, dimsize, SDInfo)) {
	  name = ss[0];
	  retVal = readSDSData(sdsid, SDInfo[0], SDInfo[1], dimsize, plane);  

	  double mm[] = new double[2];
	  // check to see max. or min.
	  dataRangeFlag = hdf.SDgetrange(sdsid, mm);
	
	  // if pre-defined range attribute of SDS is not exist, gets it from 
   	  // the value calculated before displaying the preview image
	  if (!dataRangeFlag) {
	    dataRangeFlag = true;
	    //dataRange[0] = app.hdfCanvas.getMaxValue();
	    //dataRange[1] = app.hdfCanvas.getMinValue();
 	    max = app.hdfCanvas.getMaxValue();
	    min = app.hdfCanvas.getMinValue();
	  }
	  else {
	  	max = mm[0];
	  	min = mm[1]; 
	  } 
	} // if (hdf.SDgetinfo(sdsid, ss, dimsize, SDInfo)) {
      } 
    }  // if (index == HDFConstants.FAIL) 

    hdf.SDendaccess(sdsid);
    return (retVal);
  }

  /** create the raster image by specified image data
   * @param imgData the image data(pixel value)
   * @param w the width of the image
   * @param h the height of the image
   * @return the image , otherwise null
   */
  public Image createRasterImage(byte[] imgData, int w, int h) {
    
    if ((w*h)<=0)
      return null;
      
    // set the created image data
    byte data[] = new byte[w*h];
    
    int pos = 0;
    
    for (int i=0; i<(w*h); i++) 	     
      data[i] = (byte)imgData[pos++];
      
    // create the image
    return(getToolkit().createImage(new MemoryImageSource(w,h,ColorModel.getRGBdefault(),data,0,w)));
    
  }
  

  /** Return the RAINBOW palette 
   * rgb rgb rgb rgb rgb ....
   */
 public  byte[] getPaletteOfRainbow()  {

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
 

  /** create the raster image by  specified image data
   * @param imgData the image data(pixel value)
   * @param w the width of the image
   * @param h the height of the image
   * @param imgPalette the palette of the image
   * @param index      the plane number of the image
   * @return the image , otherwise null
   */
  public Image createRasterImage(byte[] imgData, int w, int h,
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
    ImageColorModel imp;
    imp = new ImageColorModel(red,green,blue);
  
    // create the image
    return(getToolkit().createImage(new MemoryImageSource(w,h,imp.getColorModel(),data,0,w)));
    
  }

  /** Read the HDF  SDS data
   * @param sdsid   the SDS dataset identifier 
   * @param sdsInfo the sds info. array
   * @param dims    the sds dimension size
   * @param plane   the plane number
   * @return true if succeed, otherwise false
   */
  public boolean readSDSData(int sdsid, int rank, int nt, int []dims, int plane)  throws HDFException {
    // set image palette
    boolean hasPalette = false;
    
	// ??? Why are we restricted to 3-D here???
    // Is the rank valid?
    if ((rank<=1) || (rank>=4) || (plane <= 0))
      return false;
    
    // image width
    // int w = dims[rank-1];
    int w = subsetRange.width;

    // image height;
    // int h = dims[rank-2];
    int h = subsetRange.height;

    if ((w*h) < 0)
      return false;
    
    // sds data size;
    int dataSize = w*h*hdf.DFKNTsize(nt);
    
    // specify the image data storage
    hdfData    = new byte[dataSize];
    
    // image palette
    imagePalette  = new byte[3*256];	
	
    if (hasPalette == false)  {
      // try rainbow as default
      imagePalette = null;
      imagePalette = getPaletteOfRainbow();
    }
    
    // read sds data	
    int start[] = new int[3];
    int stride[]= new int[3];
    int count[] = new int[3];
    
    stride[0] = 1;
    stride[1] = 1;
    stride[2] = 1;
     
    if (rank == 3) {
	
      start[0] = plane -1;
      // start[1] = 0;
      // start[2] = 0;	
      start[1] = subsetRange.y;
      start[2] = subsetRange.x;
 
      count[2] = w   ;  // x 
      count[1] = h   ;  // y
      count[0] = 1   ;
	
    }
    else {
      // start[0] = 0;
      // start[1] = 0;
         
      start[0] = subsetRange.y;
      start[1] = subsetRange.x;
 
      count[0] = h   ;  // y 
      count[1] = w   ;  // x
    }
    
    boolean readFlag = hdf.SDreaddata(sdsid,start,stride,count,hdfData);
    // System.out.println("readSDS: " + readFlag);

    // set data type;
    this.hdfDataType = nt;
    
    // set number of the image(converted)
    if (rank==2) {
      setNumberOfImage(1);
    } else {
      setNumberOfImage(dims[rank-3]);
    }
    
    return (true);
  }

  /** set canvas size
   * @param w the width
   * @param h the height
   */
  public void setCanvasSize(int w,int h) {
    canvasWidth = w;
    canvasHeight= h;
  }
  

  /** set image size 
   * @param w the image width
   * @param h the image height
   */  
  public void setImageSize(int w,int h) {
    imageWidth = w;
    imageHeight= h;
  }

 /** return the current image height  */
  public int getImageHeight() {
	return imageHeight;     
  }

 /** return the current image width  */
  public int getImageWidth() {
	return imageWidth;     
  }

  /** set the current image & repaint it with current image 
   * This saves the current image as the previous image (in
   * a stack that is depth 1)
   * @param img the image
   */
  public  void setImage(Image img) {

    // keep current image as previous image
    preImage = null;
    preImage = image;

    //set image
    image = null;
    image = img;

    repaint();
  }

  public  void updateImage(Image img) {

    //set image
    image = null;
    image = img;

    repaint();
  }

  /** get the  current image 
   */
  public  Image getImage() {
	return (Image)image;
  }

  /** update the  current image source
   */
  public void updateImageSource() {
	this.imageSource = getImage().getSource();
  }

 public int[] getImagePixelValue(Image img, int x, int y, int w, int h) {

	if (((w*h)<=0) || ((x*y) <0)) return null;

	// return value
 	int[] pixels = new int[w * h];

 	PixelGrabber pg = new PixelGrabber(img, x, y, w, h, pixels, 0, w);
 	try {
 	    pg.grabPixels();
 	} catch (InterruptedException e) {
 	    System.err.println("interrupted waiting for pixels!");
 	    return null;
 	}
 	if ((pg.status() & ImageObserver.ABORT) != 0) {
 	    System.err.println("image fetch aborted or errored");
 	    return null;
 	}
 	
	return pixels;
 }
 

    // image filter  changed, how about the image?
    public void setImageFilter(ImageFilter imageFilter) {
    
	if (imageFilter != null)  {
	
	    // flush image
	    imageSource = null;

	    // get image source (image producer)
	    if ((imageSource == null) && (getImage() != null))
		imageSource = getImage().getSource();

	   Image newImage = null;

	   FilteredImageSource  src = null;
	   src = new FilteredImageSource(imageSource,imageFilter);

	   // new image by the image filter, but when repainting the image
	   // image consumer still need to get the pixel value from the original
	   // image, maybe.
	   newImage = createImage(src);

	   int w = getImageWidth();
	   int h = getImageHeight();

	   // To assign an image filter in the image will generate a new image,
  	   // but that the pixel value of that image have not been modified,
	   // if we assign another image filter to the new image the generated 
  	   // image will do all of the image filter previously. Try to use the
	   // PixelGraper to holder the pixel value and create the new image by 
	   // using MemorySource() to make sure that the new image's pixel value
	   // has been changed permently.

	   // grap the image pixels value 
	   int[] pixels = getImagePixelValue(newImage, 0,  0, w, h);

           newImage = null;
	   newImage = getToolkit().createImage(new MemoryImageSource(w,h,pixels,0,w));

	   // redraw new image 	
	   setImage(newImage);	
 
	}					  
	
    }

    // image filter  changed, how about the image?
    public void updateImageFilter(ImageFilter imageFilter) {
    
	if (imageFilter != null)  {

	    // get image source (image producer)
	    if ((imageSource == null) && (getImage() != null))
		imageSource = getImage().getSource();

	   Image newImage = null;

	   FilteredImageSource  src = null;
	   src = new FilteredImageSource(imageSource,imageFilter);

	   // new image by the image filter, but when repainting the image
	   // image consumer still need to get the pixel value from the original
	   // image, maybe.
	   newImage = createImage(src);

	   int w = getImageWidth();
	   int h = getImageHeight();

	   // To assign an image filter in the image will generate a new image,
  	   // but that the pixel value of that image have not been modified,
	   // if we assign another image filter to the new image the generated 
  	   // image will do all of the image filter previously. Try to use the
	   // PixelGraper to holder the pixel value and create the new image by 
	   // using MemorySource() to make sure that the new image's pixel value
	   // has been changed permently.

	   // grap the image pixels value 
	   int[] pixels = getImagePixelValue(newImage, 0,  0, w, h);

           newImage = null;
	   newImage = getToolkit().createImage(new MemoryImageSource(w,h,pixels,0,w));
	
	   // redraw new image 	
	   updateImage(newImage);	
 
	}					  
	
    }

    // image RGB filter  changed, how about the image?
    public void setImageRGBFilter(ImageFilter imageFilter) {
    
 	// same as setImageFIlter method, maybe not if need a special process.
	updateImageFilter(imageFilter);

    }

    // colormodel  changed, how about the image?
    public  synchronized void updateColorModel(ColorModel cm) {
    
	if (cm != null)  {
	
	    // get image source (image producer)
	    if ((imageSource == null) && (getImage() != null)) {

	      	imageSource = null;
		// flushing ...
		image.flush();
	      	imageSource = getImage().getSource();
	    }


	Image newImage;

	// new filter (set to new RGB model)
	ColorModelFilter  imageFilter = new ColorModelFilter(cm);

	FilteredImageSource  src = new FilteredImageSource(imageSource,imageFilter);

	newImage = createImage(src);

	// redraw new image
	setImage(newImage);

	// keep the original image ?
	
	// new color model
	setColorModel(cm);  
	
	}					  
    }

    // set new color model for current image
    public void setColorModel(ColorModel cm) {
	oldColorModel = colorModel;
	colorModel    = cm;	
    }

  /**
   * get image by specified plane number
   * @param planeNumber the image plane number
   * @return the image
   */
  public synchronized Image getImage(int planeNumber) {
      
    // set number of the image
    this.imageNumber = planeNumber;

    // set return image
    Image retImage = null;

    switch(node.type) {
    case HDFObjectNode.RIS8:
    case HDFObjectNode.RIS24:
      
      retImage = images[planeNumber-1];
      break;

    case HDFObjectNode.GRDATASET:
      retImage = images[planeNumber-1];
      break;

    case HDFObjectNode.SDSDATASET:

      // update dataset
      readDataset(planeNumber);

      retImage = images[planeNumber-1];
      break;
      
    case HDFObjectNode.Vdata:
    case HDFObjectNode.Vgroup:
      break;
    }
    return retImage;
  }
  

  /** which action will be taken
   * @param actFlag the action flag
   */
  public void setAction(int actFlag) {
    // set action 
    this.actionFlag = actFlag;
    
    // the status of the current thread
    if (killer == null) {
      start();
    } else {
      stop();
      start();
    }
  }
  

  /** set up the image anamination speed(delay time between the image display) 
   * @param sp  the delay time 
   */
  public void setSpeeds(int sp) {
    // set speed for animation
    if (speed <= 0) {
      speed = 0;
    } else if (speed >= 1000) {
      speed = 1000;
    }
 
    this.speed = sp;
  }

  /** Reshapes the Component to the specified bounding box. */
  public synchronized  void setBounds(int x, int y, int w, int h) {
    
    super.setBounds(x, y, w, h);
    // resize horizontal scrollbar
    setHScrollValue();
    // resize the vertical scrollbar
    setVScrollValue();
  }

  /**
   *  set Scrollbar
   * @param h the horizontal scrollbar
   * @param v the vertical scrollbar
   */
  public  void  setScrollbar(Scrollbar h, Scrollbar v) {
    // set scrollbar value
    this.hScrollbar = h;
    this.vScrollbar = v;
  }

  /**
   * set HDF Object Node
   * @param obj HDF Object Node
   */
  public void setHDFObjectNode(HDFObjectNode node) {
    this.node = node; 
  }
  

  /** Adjust the Horizontal Scrollbar value by the specifyed  width. */
  void setHScrollValue() {

    int canvasWidth = getSize().width;
    
    // canvas is valid?
    if (canvasWidth <= 0) {
      return;
    }
    
    //Shift everything to the right if we're displaying empty space
    //on the right side.
    if ((tx + canvasWidth) > imageWidth) {
      int newtx = imageWidth - canvasWidth;
      if (newtx < 0) {
	newtx = 0;
      }
      tx = newtx;
    }
    
    int p = (int)(canvasWidth * 0.9);
    int m = (int)(imageWidth - (canvasWidth - p) + 2);
    hScrollbar.setValues(//draw the part of the image that starts at this x:
			 tx, 
			 //amount to scroll for a "page":
			 p,
			 //minimum image x to specify:
			 0,
			 //maximum image x to specify:
			 m);
    
    //"visible" arg to setValues() has no effect after scrollbar is visible.
    hScrollbar.setBlockIncrement(p);
  
    return;
  }
  

  /** Adjust the Vertical Scrollbar value by the specifyed  width. */
  void setVScrollValue() {
    
    int canvasHeight = getSize().height;
    
    // canvas is valid?
    if (canvasHeight <= 0) {
      return;
    }
    
    //Shift everything to the top if we're displaying empty space
    //on the top side.
    if ((ty + canvasHeight) > imageHeight) {
      int newty = imageHeight - canvasHeight;
      if (newty < 0) {
	newty = 0;
      }
      ty = newty;
    }
    
    int p = (int)(canvasHeight * 0.9);
    int m = (int)(imageHeight - (canvasHeight - p) + 2);
    vScrollbar.setValues(//draw the part of the image that starts at this y:
			 ty, 
			 //amount to scroll for a "page":
			 p,
			 //minimum image y to specify:
			 0,
			 //maximum image y to specify:
			 m);
    
    //"visible" arg to setValues() has no effect after scrollbar is visible.
    vScrollbar.setBlockIncrement(p);
    
    return;
  }

  /**
   * Called to start the applet. You never need to call this method
   * directly, it is called when the applet's document is visited.
   * @see java.applet.Applet#start
   */
  public void start() {

    if (killer == null) {    
      killer = new Thread(this);
      killer.start();
    }
  }

  /**
   * Called to stop the applet. It is called when the applet's document is
   * no longer on the screen. It is guaranteed to be called before destroy()
   * is called. You never need to call this method directly.
   * @see java.applet.Applet#stop
   */
  public void stop() {
    if (killer != null) {
      killer.stop();
    }
    killer = null;
  }

  // delay n seconds 
  public void delay(int n) {
	try {
		Thread.sleep(n);
	} catch (Exception e) {};
  }

  /**
   * java.lang.Runnable stuff
   */
  public void run()  {
			   
    int currentPlaneNumber = imageNumber;
    
    while (killer != null ) {
      // "Forward" action
      if (actionFlag == imageFrame.FORWARD) {
	
	++currentPlaneNumber;
	if (currentPlaneNumber > numberOfImage)
	  currentPlaneNumber = 1;
	
	try {
	  // get previous image
	  image = images[currentPlaneNumber-1];
	  	  
	  // set image number
	  imageNumber = currentPlaneNumber;
	  imageFrame.numPlane = currentPlaneNumber;

	  // set image for hdfCanvas
	  // app.hdfCanvas.setImage(image);
	  
	  // set image for current canvas
	  setImage(image);

	  // wait a minute
	  Thread.sleep(10);
	  Thread.sleep(speed);
	  
	} catch (Exception e) {};
      }

      // "Backward action"
      if (actionFlag == imageFrame.BACKWARD) {
	
	--currentPlaneNumber;
	if (currentPlaneNumber < 1)
	  currentPlaneNumber = numberOfImage;
	
	try {
	  // get previous image
	  image = images[currentPlaneNumber-1];

	  // set image number
	  imageNumber = currentPlaneNumber;
  	  imageFrame.numPlane = currentPlaneNumber;

	  // set image for hdfCanvas
	  // app.hdfCanvas.setImage(image);
	  
	  // set image for current canvas
	  setImage(image);

	  // wait a minute
	  Thread.sleep(10);
	  Thread.sleep(speed);
	  
	} catch (Exception e) {};
      } 
    } 
  } 

  /**
   * Updates the component. This method is called in
   * response to a call to repaint. You can assume that
   * the background is not cleared.
   * @param g the specified Graphics window
   * @see java.awt.Component#update
   */
  public   void update(Graphics g) {

    Dimension d = getSize();
    setCanvasSize(d.width, d.height);
    
    if (offScreenImage == null) {
      // offScreenImage not created; create it.
      offScreenImage = createImage(d.width*4, d.height*4);	
      
      // get the off-screen graphics context    
      offGraphics    = offScreenImage.getGraphics();
      
      // set the font for offGraphics
      offGraphics.setFont(getFont());	 
    }
    
    // paint the background on the off-screen graphics context
    offGraphics.setColor(getBackground());
    offGraphics.fillRect(0,0,d.width,d.height);    
    offGraphics.setColor(getForeground());
    
    // draw the current frame to the off-screen 
    paint(offGraphics);
    
    //then draw the image to the on-screen 
    g.drawImage(offScreenImage, 0, 0, null);
    
  }

 /** Write the information on the top of the image
   * @param g the graphics
   * @param x the x   coordinate
   * @param y the y coordinate
   * @param info the strings array
   * @param lines the number of informations.
   */
  public void writeInfo(Graphics g , int x , int y , String[] info, int lines){
    
    // get width and height
    FontMetrics lFM = g.getFontMetrics();
    
    int w = lFM.stringWidth(info[0].toString()) + lFM.stringWidth("AB");
    for (int i=1; i<lines; i++) {
	int strWidth = lFM.stringWidth(info[i].toString()) + lFM.stringWidth("AB");
	w = Math.max(w, strWidth);	
    }
    
    int h = lFM.getHeight()*(2 + lines);
    
    // get position to display the string
    int xpos = x + lFM.stringWidth("A");
    int ypos = y + lFM.getHeight() + lFM.getHeight()/2;
    
    // set the background
    g.setColor(Color.black);
    
    // draw the oval to display the string
    g.fillRoundRect(x,y,w,h,16,16);
    
    // set the foreground
    g.setColor(Color.white);

    for (int i=0; i<lines; i++) {
	g.drawString(info[i], xpos,ypos );
	ypos += lFM.getHeight();
    }
  }
  

  /**
   * Paints the component.
   * @param g the specified Graphics window
   * @see java.awt.Component#paint
   */
  public  void paint(Graphics g) {

    // get current Canvas size
    int w = getSize().width;
    int h = getSize().height;
    
    // get the approciate position to display the image
    startx=1;
    starty=1;
 
    // set the specified translated parameters 
    // and the subcomponents will be relative to this origin.
    g.translate(-tx, -ty);
    
    if (imageWidth>w)
      startx = 1;
    else
      startx = (w-imageWidth)/2;
    
    if (imageHeight>h)
      starty = 1;
    else
      starty = (h-imageHeight)/2;

    if (image == null) {

      // frame
      g.setColor(Color.red);
      g.drawRect(0,0,getSize().width-2, getSize().height-2);
      
      // info
      g.setColor(Color.black);
      FontMetrics fm = g.getFontMetrics();
      int x = (w - fm.stringWidth(infoStr))/2;
      int y = h/2;
      g.drawString(infoStr, x, y);
      
    } else {
      // draw the frame
      g.setColor(Color.red);
      // g.drawRect(startx-1,starty-1,imageWidth+1, imageHeight+1);
      
      // image
      g.drawImage(image,startx,starty,this);

      // image number
      String dispStr = "Image Number: " + Integer.toString(imageNumber);
      g.drawString( dispStr, 1, h-6);   // default font height = 9

      // set image area;
      imageArea.setBounds(startx, starty, imageWidth, imageHeight);
   
      // mouse pressed
      if (dragFlag) { 
         if ((imagePlotMode != JHVImageFrame.RADIAL_PLOT) && (dragArea != null))
	 	g.drawRect(dragArea.x, dragArea.y, dragArea.width, dragArea.height);
      	 else 
	 	g.drawRect(startx+tx, my-1+ty, getImageWidth(), 2); 
      }
      // draw the subset area to response the data selected
      if (drawSubsetFlag) {
	g.draw3DRect(drawSubsetArea.x,
	             drawSubsetArea.y,
	             drawSubsetArea.width,
	             drawSubsetArea.height, 
	             true);
	}
    }
    
    // set the specified translated parameters 
    // and the subcomponents will be relative to this origin.
    g.translate(tx, ty);

    // show coordinates
    if ((showCoordinate) && (mouseOnImageFlag)) {
		
	String infoStr[] = new String[3];
        infoStr[0] = coorInfo;
	
	// my:: the mouse position
	if (my<50) { // write in bottom
    
		// write info. on the top of the image
		writeInfo(g, startx, h-30, infoStr,1);
	} 
	else {
		// write info. on the top of the image
		writeInfo(g, startx, 0, infoStr,1);
	}
    } 
  }

  /**
   * If the size of the image is appropriate the image will be displayed
   */
  synchronized void checkSize() {
    
    // resize the canvas
    setSize(canvasWidth, canvasHeight);
    
    if (!imageFrameDisplayed) {
      
      // popup frame
      imageFrame.popup();
      imageFrameDisplayed = true;
	
      // set button status   
      if (numberOfImage > 1)
	imageFrame.buttonEnable();
      else
	imageFrame.buttonDisable();
    
    } 
  }

  /** Set canvas size at SxS */
  public Dimension getMinimumSize(){
    return new Dimension(canvasWidth, canvasHeight);
  }

  /** set canvas preferred size */
  public Dimension getPreferredSize() {
    return getMinimumSize();
  }

  public void mouseClicked(MouseEvent e) {}

  /**
   * Called if the mouse is up.
   */
  public void mouseReleased(MouseEvent e)
  {
    int x = e.getX();
    int y = e.getY();
  
    if (imagePlotMode == JHVImageFrame.RADIAL_PLOT)
	dragFlag = false;

    if (dragArea == null)
	return;    
 
    // set show coordinate flag
    // setShowCoordinate(false);

    // compute the dataset range based on the draged area & scroll offset
    // if the dragArea is inside the image area
    // set datasetArea.
    datasetRange = new Rectangle(dragArea.x - startx , 
				 dragArea.y - starty ,
				 dragArea.width,      dragArea.height);
    
    // make the draw area valid
    int dx = dragArea.x - startx;
    int dy = dragArea.y - starty;
    int w  = dragArea.width;
    int h  = dragArea.height;
    
    if (dx <0) {
      w += dx;
      dx = 0;
    }
    
    if (dy <0) {
      h += dy;
      dy = 0;
    }
    
    if ( (dx+w) > imageArea.width)
      w = imageArea.width - dx;
    if ( (dy+h) > imageArea.height)
      h = imageArea.height - dy;
    
    // reset draw flag when subset area changed
    drawSubsetFlag = false;
    
    // reset the datasetRange
    datasetRange.setBounds(dx,dy,w,h);
    
    boolean spreadFlag = false;
    if (spreadFlag) {
      // read the current image's raw dataset 
      if (readDataset(imageNumber)) {
	// open the separate spreadsheet window
	new JHVDataFrame(imageFrame, datasetRange);
      }
    }
    else { 
      dispZoomImage(datasetRange, this.image);
    }
  }
  

  /**
   * Called if the mouse is down.
   */
  public void mousePressed(MouseEvent e)
  {
    int x = e.getX();
    int y = e.getY();

    // keep the mouse position
    currentDragArea = new Rectangle(x+tx, y+ty, 0,0);
    // check to see if the mouse is on the image	
    checkMousePosition(x,y);
    if (imagePlotMode == JHVImageFrame.RADIAL_PLOT)  // radial plots
	if (mouseOnImageFlag)  // radial plots test
    		radialPlots(new Rectangle(0,x-starty,1, 0 )); 
    else if (imagePlotMode == JHVImageFrame.HISTOGRAM_PLOT)
	     plotHistogram(new Rectangle(1,1));
    
  }
  

   // make a radial plot for a selectd line of an image,
   // the horizontal line looks like (x,y, x+w, y)
   void radialPlots(Rectangle rect) {

     // working on horizontal line
     try {
	// System.out.println(rect);
	int x = rect.x;
	int y = rect.y;

	int w = imageWidth;
    	int h = imageHeight; 
	
	double[] rArray = new double[rect.width];

	int firstPos = 0;
	
	for (int i=x; i<x+rect.width; i++) {
		double retDat = (double)getData(i, y);
		rArray[i-x] = retDat;
	}

	double[][] pArray = new double[2][rect.width];

	for (int k =0; k<rect.width; k++) {
	    pArray[1][k] = (double)rArray[k];
	    pArray[0][k] = (double)(k+x);
	}

	int fx = subsetRange.x;
	int fy = subsetRange.y;

	String xStr = "(";
	xStr = xStr + Integer.toString(x+fx) + "," + Integer.toString(y+fy) + ") ~ (" ;
	xStr = xStr + Integer.toString(x+w+fx-1) + "," + Integer.toString(y+fy) + ")";

	if (aPlot==null)  {
	   aPlot= new XYPlot(new Frame(), pArray);
	   // aPlot= new XYPlot((Frame)imageFrame, pArray);
	}
	else {
	   aPlot.setData(pArray);
   	   aPlot.show();
	}

	// set title
	aPlot.setTitle("Radial Plot of an Image");
	aPlot.setXAxisTag(xStr);
	aPlot.setYAxisTag("Pixel Value");

	if (dispImageDataMode != JHVImageFrame.DIGIT_VALUE)
	   // image mode and pixel value is at (0,255)
	   aPlot.setYAxisRange(0,255);
	else
	   if (dataRangeFlag)
		aPlot.setYAxisRange(min,max);
	   else {

		double yMin, yMax;
		yMin = yMax = rArray[0];
		for (int i=0; i<rArray.length; i++) {
	    		double tmp = rArray[i];

	    		yMin = Math.min(tmp, yMin);
	    		yMax = Math.max(tmp, yMax);
		}
		aPlot.setYAxisRange(yMin, yMax);
	    }
       } catch (HDFException e) {};
    }

   // make a histogram graphic by selected area of an image, and
   // histogram display is based on image's palette
   void plotHistogram(Rectangle rect) {

	// selected range
	int x = rect.x;
	int y = rect.y;

	int w = imageWidth;
    	int h = imageHeight;
	
	// distributed data value
	double pArray[][] = new double[2][256];

	// retrieve data distribution
	int firstPos = 0;	
	for (int i=x; i<x+rect.width; i++) {
	    for (int j=y; j<y+rect.height; j++) {

		int pos = (j * w + i);
		pos += firstPos;
		int retDat = ((byte)imageData[pos]);
		if (retDat < 0)  retDat += 256;

		pArray[1][retDat] += 1;
	   }
	}

	double maxVal = max;
	double minVal = min;
	if  (maxVal == minVal) {
	    maxVal = 255;
	    minVal = 0;
	}
	// scale value of x-axis of histogram
	for (int k =0; k<256; k++) {
		if (dispImageDataMode != JHVImageFrame.DIGIT_VALUE)
	 	   pArray[0][k] = (double)k;   // pixcel value
		else {
		   // physical value
		   double deta = (maxVal-minVal)/255;
		   pArray[0][k] = minVal +(double)deta*k;
		}
	}

	if (hPlot==null) {

	   // new instance of histogram
	   //hPlot= new XYPlot(pArray);
	   hPlot = new XYPlot(new Frame(), pArray);
	   // hPlot = new XYPlot((Frame)imageFrame, pArray);
	   
  	   Color colors[] = new Color[256];
    	   for (int ci=0; ci<256; ci++) {
		int red = (byte)imagePalette[ci*3];
		if (red<0) red += 256;
		int green = (byte)imagePalette[ci*3+1];
		if (green<0) green += 256;
		int blue = (byte)imagePalette[ci*3+2];
		if (blue<0) blue += 256;

		colors[ci] = new Color(red,green,blue);
		hPlot.setColors(colors);
    	   }
	} 
	else {
	   hPlot.setData(pArray);
	   hPlot.show();
	}

	if (dispImageDataMode == JHVImageFrame.DIGIT_VALUE)  
	   hPlot.setXAxisRange(minVal,maxVal);
	else
	   hPlot.setXAxisRange(0,255);

	// set title
	hPlot.setTitle("Histogram of an Image");
	hPlot.setXAxisTag("Pixel Value");
	hPlot.setYAxisTag("Pixel Number");

	// hPlot.setPlotMode(hPlot.plotPane.HISTOGRAM);
 	hPlot.setPlotMode(XYPlotPane.HISTOGRAM);

     }

  /**
   * Called if the mouse is dragged (the mouse button is down)
   */
  public void mouseDragged(MouseEvent e) {

    int x = e.getX();
    int y = e.getY();
    dragFlag = true;
    
    // set mouse point
    mx = x;
    my = y;

    if (imagePlotMode == JHVImageFrame.RADIAL_PLOT)  { // radial plots
	// radial plots
	dragArea = null;  // force to skip when mouse released  
	checkMousePosition(mx,my);

	if (mouseOnImageFlag) {	
	   repaint();
    	   // radial plots test
    	   radialPlots(new Rectangle(0,my-starty+ty,getImageWidth(), 0 )); 
	} 
    } 
    else  {

    // resize the draged area
    currentDragArea.setSize(x-currentDragArea.x + tx, y-currentDragArea.y + ty);
    
    // get the rectangle that can be drawable(the value is valid);
    // get current canvas size 
    Dimension d = getSize();
    
    // make sure that drag area can be drawable
    dragArea = getDrawableRect(currentDragArea, d);
       
    // set show coordinate flag
    // setShowCoordinate(false);
    mouseOnImageFlag = false; // force not to display coord. value

    // repaint to display the info. or not. 		
    repaint();

    // test for radial plots
    datasetRange = new Rectangle(dragArea.x - startx , 
				 dragArea.y - starty ,
				 dragArea.width,      dragArea.height);
    // make the draw area valid
    int dx = dragArea.x - startx;
    int dy = dragArea.y - starty;
    int w  = dragArea.width;
    int h  = dragArea.height;
    
    if (dx <0) {	
	w += dx;
	dx = 0;
    }
    
    if (dy <0) {
	h += dy;
	dy = 0;
    }
    
    if ( (dx+w) > imageArea.width)
       w = imageArea.width - dx;
    if ( (dy+h) > imageArea.height)
       h = imageArea.height - dy;

    // reset the datasetRange
    datasetRange.setBounds(dx,dy,w,h);
    // draw histogram plot if possible
    if (imagePlotMode == JHVImageFrame.HISTOGRAM_PLOT)  
    	plotHistogram(datasetRange);

    } // if (dispImageDataMode == JHVImageFrame.RADIAL_PLOT)  
  }

  /**
   * Called when the mouse enters the component.
   */
  public void mouseEntered(MouseEvent e)
  {
    int x = e.getX();
    int y = e.getY();
    
    // check mouse position, then do something to that.
    checkMousePosition(x,y);
    
  }

  // check mouse position, then do something to that.
  public void checkMousePosition(int x, int y) {
	
    // check if the point(x,y) is inside the image area
    if (imageArea.contains(x+tx,y+ty)) {
  
      // make annotation (Click to get the orignal image)
      mouseOnImageFlag = true;

      // set show coordinate flag
      // setShowCoordinate(true);
	 
     // change the cursor type to "cross" if possible to show the coordinate
     ((Component)imageFrame).setCursor(new Cursor(Cursor.CROSSHAIR_CURSOR));    

    }
    else {

	mouseOnImageFlag = false;
    	// set show coordinate flag
      	// setShowCoordinate(false);
	 
     	// change the cursor type to "default" if possible to show the coordinate
       	((Component)imageFrame).setCursor(new Cursor(Cursor.DEFAULT_CURSOR));
    }

    // keep current mouse position.
    mx=x; my=y;

  }
  

   /**
   * Called if the mouse moves (the mouse button is up)
   */
  public void mouseMoved(MouseEvent e)
  {
    int x = e.getX();
    int y = e.getY();

    // check mouse position, then do something to that.
    checkMousePosition(x,y);
    
    try{
      if ((showCoordinate) && (mouseOnImageFlag)) 
	// get coordinate info.
	coorInfo = "" + getCoordinateInfo(mx,my);
    } catch (HDFException ex) {}
    
    if (imageFrame.spButton.isEnabled()) {
   	   // repaint to display the info. or not. 		
      repaint();
    }
  }

    /**
     * Called when the mouse exits the component.
     */
  public void mouseExited(MouseEvent e)
  {
    int x = e.getX();
    int y = e.getY();
    
    mouseOnImageFlag = false;
    repaint();
    
    // change the cursor type to "cross" if possible to show the coordinate
    // if (showCoordinate)
    ((Component) imageFrame).setCursor(new Cursor(Cursor.DEFAULT_CURSOR));    
    
      // set show coordinate flag
    // setShowCoordinate(false);
  }

/** make spreadsheet for draged area of the image */
public void makeSpreadsheet() {

    if ((dragFlag) && (datasetRange != null)) {
    
	// read the current image's raw dataset 
	if (readDataset(imageNumber)) {
	    // open the seprate spreadsheet
	    new JHVDataFrame(imageFrame, datasetRange);
	}
    }
}

 /** Draw the rectangle area on the current image
  * @ param rect the rectangle to redraw
  */
public void drawRectangle(Rectangle rect) {
    
    // set draw subset area flag
    drawSubsetFlag = true;
    // set draw rectangle area
    drawSubsetArea.setBounds(startx+rect.x, starty + rect.y, 
                           rect.width, rect.height);
    repaint(); 
}
  

  /** Return the new drawable rectangle based on the anonther rectangle &
   *  currnet canvas size.
   * @param originalRect the rectangle
   * @param drawingArea  the size can be painted.
   * @return the new rectangle
   */
  Rectangle getDrawableRect(Rectangle originalRect, Dimension drawingArea) {
    int x = originalRect.x;
    int y = originalRect.y;
    int width = originalRect.width ;
    int height = originalRect.height ;
    
    //Make sure rectangle width and height are positive.
    if (width < 0) {
      width = 0 - width;
      x = x - width + 1;
      if (x < 0) {
	width += x;
	x = 0;
      }
    }
    if (height < 0) {
      height = 0 - height;
      y = y - height + 1;
      if (y < 0) {
	height += y;
	y = 0;
      }
    }
    
    //The rectangle shouldn't extend past the drawing area.
    if ((x + width - tx) > drawingArea.width) {
      width = drawingArea.width - x + tx;
    }
    if ((y + height - ty)> drawingArea.height) {
      height = drawingArea.height - y + ty;
    }
    return new Rectangle(x, y, width, height);;
  }

   public void setShowCoordinate(boolean bvalue) {
	this.showCoordinate = bvalue;
    }

  /** Return the coordinate data
   */
  float  getData(int x, int y) throws HDFException {

   HDFNativeData convert = new HDFNativeData();
   float retDat;

   // digital value display
   if (dispImageDataMode == JHVImageFrame.DIGIT_VALUE) {

    // data size for the original data based on the data number type
    int datasize = hdf.DFKNTsize(hdfDataType);
    
    // the image dataset range
    int w = imageWidth;
    int h = imageHeight;

    // get the first address to get the raw data from hdfData
    int firstPos = 0;
    if ((node.type == HDFObjectNode.RIS24) ||
        (node.type == HDFObjectNode.GRDATASET)) {
         // adjust the first data position
         firstPos = w * h * datasize * (imageNumber - 1);
    }
 	
    // first extract number position by row
    int pos = (y * w + x) * datasize;

    // adjust the pos.
    pos += firstPos;
	
    switch(hdfDataType) {

	// one bit char
	case HDFConstants.DFNT_CHAR:
	case HDFConstants.DFNT_UCHAR8:
	case HDFConstants.DFNT_UINT8:
	  retDat = (float)(hdfData[pos]);
	  // convert to positive if the number is negative 
	  if (retDat < 0)  
	     retDat += 256.0f;	
	  break;
		
	// signed integer (byte)	
	case HDFConstants.DFNT_INT8:
	  
	  retDat = (float)(hdfData[pos]);
	  break;
	  
        // short	
	case HDFConstants.DFNT_INT16:
	case HDFConstants.DFNT_UINT16:
	      
	  Short shval = new Short(convert.byteToShort(hdfData,pos));
	  retDat = shval.floatValue();
	  break;
	    
	case HDFConstants.DFNT_INT32:
	case HDFConstants.DFNT_UINT32:
		
	  Integer ival = new Integer(convert.byteToInt(hdfData,pos));
	  retDat = ival.floatValue();
	  break;
		  
	//case HDFConstants.DFNT_FLOAT:
	case HDFConstants.DFNT_FLOAT32:
	
	  Float fval = new Float(convert.byteToFloat(hdfData,pos));
	  retDat = fval.floatValue();
	  break;
	    
	//case HDFConstants.DFNT_DOUBLE:
	case HDFConstants.DFNT_FLOAT64:
	
	  Double dval = new Double(convert.byteToDouble(hdfData,pos));
	  retDat = dval.floatValue();
	  break;
	
	default:
	  retDat = 0;
    }
   } else if (dispImageDataMode == JHVImageFrame.PIXEL_VALUE) {

    // the image dataset range
    int w = imageWidth;
    int h = imageHeight;

    // adjust the first data position
    int firstPos = 0 ;
 	
    // first extract number position by row
    int pos = (y * w + x);

    // adjust the pos.
    pos += firstPos;
    retDat = (float)((byte)imageData[pos]);
    // convert to positive if the number is negative 
    if (retDat < 0)  retDat += 256.0f;	
 
   } else retDat = 0.0f;
   return retDat;
  }

  // Return the coordinate info.
  String  getCoordinateInfo(int x, int y) throws HDFException {
   	
	// keep the dataset position
    	Rectangle datPos = null;
	datPos	         = new Rectangle(x+tx-startx, y+ty-starty, 0,0);

	// data value
	float	coorData = getData(x+tx-startx, y+ty-starty);

	return  ("[" + Integer.toString(x+tx-startx+subsetRange.x) + " , "
		+ Integer.toString(y+ty-starty+subsetRange.y)  + "]= " + Float.toString(coorData));

  }

 /**
   * Get subset image 
   */
   public Image getSubImage( Rectangle rect, Image imageSource) {
    
      Image       retImage = null;
      int         newWidth, newHeight;
      int         cropX, cropY, cropWidth, cropHeight;

      // check value?
      cropX = rect.x;
      cropY = rect.y;
   
      cropWidth = rect.width;
      cropHeight= rect.height;
   
      CropImageFilter cropFilter = new
                      CropImageFilter( cropX, cropY, cropWidth, cropHeight );

      retImage = createImage(
	       new FilteredImageSource(imageSource.getSource(), cropFilter ));
  
      return retImage;
   }

 /**
   * prepare zoom image & display it
   */
 public void dispZoomImage( int x, int y, Image img) {
    
    // create default subset area
    Rectangle rect = new Rectangle(0,0,50,50);
   
    // image area contains a point(x,y)
    if (imageArea.contains(x,y)) {
	
	int xPos = 0;
	int yPos = 0;
	
	int w = 50; 
	int h = 50;
	
	if ((x+tx+startx)>25)
	   xPos = x + tx -25 -startx;
	if ((y+ty+starty)>25)
	   yPos = y + ty -25 -starty;
	
	if ((xPos+50)>imageWidth) 
	   w = imageWidth - xPos;
	if ((yPos+50)>imageHeight)
	   h = imageHeight - yPos;
    
	// set rect
	rect.setBounds(xPos, yPos, w, h);
	
	// get zoom image
	Image zoomImage = getSubImage(rect, img);

	// set zoom image for zoom panel
	imageFrame.zoomImagePanel.setImage(zoomImage,50,50);

    }
   }

 /**
   * prepare zoom image & display it
   */
 public void dispZoomImage( Rectangle rect, Image img) {
        
    // get zoom image
    Image zoomImage = getSubImage(rect, img);

    int w = (int)(rect.width * imageFrame.zoomFactor);
    int h = (int)(rect.height * imageFrame.zoomFactor);
        
    // set zoom image for zoom panel
    imageFrame.zoomImagePanel.setImage(zoomImage, w,h);
	
   }

   public void openNewPalette() {

	if (!paletteEditorOpen) {

		paletteEditorOpen = true;

		paletteEditor = new PaletteEditor(this);

		// set new palette
		// reset current image source
		imageSource = imageSources[imageNumber-1];

		if (colorModelPalette == null)		  
		   paletteEditor.setColorModel(imagePalette);
		else
		   paletteEditor.setColorModel(colorModelPalette);
		
		paletteEditor.show();
	}
    }

    // image filter  changed, how about the image?
    public void updateImage(ImageFilter imageFilter) {
    
        // new FilteredImageSource instance
	FilteredImageSource  src = null;
	for (int i=0; i<images.length; i++) {

		// get image source (image producer)	
		imageSource = null;
		imageSource = imageSources[i];		
		src = new FilteredImageSource(imageSource,imageFilter);
		images[i] = null;
		images[i] = createImage(src);
	}
	
	// current image
	setImage(images[imageNumber-1]);
    }

   public void setNewPalette() {

	if (paletteEditorOpen) {

		// set new image palette value
		if (colorModelPalette == null)
		   colorModelPalette = new byte[768];

		byte pal[] = null;
		pal = paletteEditor.currentPaletteValue();
		for (int i=0; i<768; i++)
		    colorModelPalette[i] = pal[i];
		
		ColorModel  cm = new ImageColorModel(pal).getColorModel();

		updateImage((ImageFilter)new ColorModelFilter(cm));
		
	}
    }

   public void recoverImage() {

	// jhvCanvas.setImage(images[imageNumber-1]);

	// recover the orignal image
	setImage(images[imageNumber-1]);

    }

   /* get new SDS image by different range and update the image data
    * @param data the original SDS dataset
    * @param datatype dataset number type
    * @param range the range of the dataset
    * @param w width of the image
    * @param h the height of the image
    * @param imageData image data
    * @param palette  palette to be used when creating new image
    * @return the image from SDS by given dataset range
    */
    public Image getSDSImage(byte[] data, double min, double max, int datatype,
			     int w, int h, 
			     byte[] imageData, byte[] palette) {

	Image retImage;

	// make image data
      	boolean cvFlag = true;
    	   cvFlag = ImageDataConverter.makeImageDataByRange(data,min,max, datatype,
			       		        w,h,
			       		        0, imageData);

	if (!cvFlag) 
	   retImage = null;
	else { // create new image 
	   retImage = createRasterImage(imageData,w,h,
				        palette,1);
	}

	return retImage;

    }

    public void updateSDSImage(double min, double max) {
	
	Image sdsImage = null;
	sdsImage = getSDSImage( hdfData, min,max, hdfDataType, 
				imageWidth, imageHeight,
				imageData,  imagePalette);
	setImage(sdsImage);
	updateImageSource();

    }

     /**
      * ncsa.hdf.palette.PaletteEditorAdapter flag
      */
     public void setPaletteEditorOpen(boolean flag)  {
       this.paletteEditorOpen = flag;
    }

      public void setSubsetRange(Rectangle rect) {

	subsetRange = rect;
    }
}
