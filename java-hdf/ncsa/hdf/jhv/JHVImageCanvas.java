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

//-------------------------------------------------------------------------------------
//
//  Upgraded to the JDK 1.1.1b Event Model.
//   - Apu Kapadia May 21st, 1997.
//
//--------------------------------------------------------------------------------------
 /* This class implement full resolution image from HDF process,
  * by implementing the PaletteEditorAdapter interface the image
  * palette should be modified through  generic "Palette Editor"
  * implemented in seprate package -xlu 7/97*/

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
  boolean	showCoordinate = true;

  // coordinate info.
  String	coorInfo = "";

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
      **************************************************************/
    HDFLibrary hdf = app.hdf;
    
    // specify imageData;
    imageData = new byte[imageHeight*imageWidth];
    
    if(readSDS(app.hdfFile, 1)) {
      
  	// make image data
      	boolean cvFlag = true;
      	if (dataRangeFlag) {
	  cvFlag = app.makeImageDataByRange( hdfData, min, max,
			     hdfDataType, imageWidth, imageHeight, (int)0,  imageData);
        }
        else {
	  cvFlag = app.makeImageData(hdfData,hdfDataType,
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
	
	  cvFlag = app.makeImageDataByRange(hdfData,min, max,
			hdfDataType, imageWidth,imageHeight,
					    0, imageData);
        }
        else {
	  cvFlag = app.makeImageData(hdfData,hdfDataType,
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
    }
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
    int w = dim_sizes[0];
    
    // image height;
    int h = dim_sizes[1];
    
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
    // set interlace to read	
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
    
    count[0] = w   ;
    count[1] = h   ;
    
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
      if (app.makeImageData(hdfData,hdfDataType,
			      imageWidth,imageHeight,
			      (imageWidth*imageHeight*(i-1)),
			      imageData)) {
	images[i-1] = createRasterImage(imageData, imageWidth, imageHeight,
					imagePalette,1);	
	}
    }
    
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
	 retVal = app.makeImageData(hdfData,hdfDataType,
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
      
    } catch (HDFException e) {
	System.out.println("Warning: Exception caught and ignored (readDataset)");
    }
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
	  max = mm[0];
	  min = mm[1];  
	}
      } 
    } 
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
    
    
    byte[][]  values={	
      
      {
	0,  4,  9, 13, 18, 22, 27, 31, 36, 40, 45, 50, 54, 58, 61, 64,
	68, 69, 72, 74, 77, 79, 80, 82, 83, 85, 84, 86, 87, 88, 86, 87,
	87, 87, 85, 84, 84, 84, 83, 79, 78, 77, 76, 71, 70, 68, 66, 60,
	58, 55, 53, 46, 43, 40, 36, 33, 25, 21, 16, 12,  4,  0,  0,  0,
	0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  4,
	8, 12, 21, 25, 29, 33, 42, 46, 51, 55, 63, 67, 72, 76, 80, 89,
	93, 97,101,110,114,119,123,-125,-121,-116,-112,-103,-99,-95,-91,-87,
-78,-74,-69,-65,-57,-53,-48,-44,-35,-31,-27,-23,-14,-10,-6,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1
      },
      
      {
	0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  4,  8,
	16, 21, 25, 29, 38, 42, 46, 51, 55, 63, 67, 72, 76, 84, 89, 93,
	97,106,110,114,119,127,-125,-121,-116,-112,-104,-99,-95,-91,-82,-78,
-74,-69,-61,-57,-53,-48,-40,-36,-31,-27,-23,-14,-10,-6,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-6,-14,-18,-23,-27,-35,-40,-44,-48,-57,-61,-65,-69,-78,-82,-86,
-91,-95,-103,-108,-112,-116,-125,127,123,119,110,106,102, 97, 89, 85,
	80, 76, 72, 63, 59, 55, 51, 42, 38, 34, 29, 21, 17, 12,  8,  0
      },
      
      {
	0,  3,  7, 10, 14, 19, 23, 28, 32, 38, 43, 48, 53, 59, 63, 68,
	72, 77, 81, 86, 91, 95,100,104,109,113,118,122,127,-124,-120,-115,
-111,-106,-102,-97,-93,-88,-83,-79,-74,-70,-65,-61,-56,-52,-47,-42,
-38,-33,-29,-24,-20,-15,-11,-6,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-10,
-14,-18,-23,-31,-36,-40,-44,-53,-57,-61,-65,-69,-78,-82,-86,-91,
-99,-104,-108,-112,-121,-125,127,123,114,110,106,102, 97, 89, 84, 80,
	76, 67, 63, 59, 55, 46, 42, 38, 34, 25, 21, 16, 12,  8,  0,  0,
	0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
      }
      
    };
    
    
    // number of color
    int  ncolors = 256;
    
    byte retPal[] = new byte[768];
    
    for (int i=0; i<256; i += 1) {
      
      retPal[3*i]   = (byte)values[0][i];
      retPal[3*i+1] = (byte)values[1][i];
      retPal[3*i+2] = (byte)values[2][i];
    }
    
    return retPal;
    
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
    int w = dims[rank-1];
    
    // image height;
    int h = dims[rank-2];
    
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
      start[1] = 0;
      start[2] = 0;	
      
      count[2] = w   ;  // x 
      count[1] = h   ;  // y
      count[0] = 1   ;
    }
    else {
      start[0] = 0;
      start[1] = 0;
      
      count[0] = h   ;  // y 
      count[1] = w   ;  // x
    }
    
    boolean readFlag = hdf.SDreaddata(sdsid,start,stride,count,hdfData);
    
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
	  app.hdfCanvas.setImage(image);
	  
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
	  app.hdfCanvas.setImage(image);
	  
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
      
      if (dragFlag)
	g.drawRect(dragArea.x, dragArea.y, dragArea.width, dragArea.height);

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
   * @param evt the event
   * @param x the x coordinate
   * @param y the y coordinate
   * @see java.awt.Component#mouseUp
   */
  public void mouseReleased(MouseEvent e)
  {
    int x = e.getX();
    int y = e.getY();

    if (dragArea == null)
	return;    
 
    // set show coordinate flag
    setShowCoordinate(false);

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
   * @param evt the event
   * @param x the x coordinate
   * @param y the y coordinate
   * @see java.awt.Component#mouseDown
   */
  public void mousePressed(MouseEvent e)
  {
    int x = e.getX();
    int y = e.getY();

    // keep the mouse position
    currentDragArea = new Rectangle(x+tx, y+ty, 0,0);

  }
  

  /**
   * Called if the mouse is dragged (the mouse button is down)
   * @param evt the event
   * @param x the x coordinate
   * @param y the y coordinate
   * @see java.awt.Component#mouseDrag
   */
  public void mouseDragged(MouseEvent e) 
  {
    int x = e.getX();
    int y = e.getY();
    dragFlag = true;
    
    // resize the draged area
    currentDragArea.setSize(x-currentDragArea.x + tx, y-currentDragArea.y + ty);
    
    // get the rectangle that can be drawable(the value is valid);
    // get current canvas size 
    Dimension d = getSize();
    
    // make sure that drag area can be drawable
    dragArea = getDrawableRect(currentDragArea, d);
    
    
    // set show coordinate flag
    setShowCoordinate(false);
    
    // repaint to display the info. or not. 		
    repaint();
  }

  /**
   * Called when the mouse enters the component.
   * @param evt the event
   * @param x the x coordinate
   * @param y the y coordinate
   * @see java.awt.Component#mouseEnter
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
      setShowCoordinate(true);
	 
     // change the cursor type to "cross" if possible to show the coordinate
     ((Component)imageFrame).setCursor(new Cursor(Cursor.CROSSHAIR_CURSOR));    

    }
    else {

	mouseOnImageFlag = false;
    	// set show coordinate flag
      	setShowCoordinate(false);
	 
     	// change the cursor type to "default" if possible to show the coordinate
       	((Component)imageFrame).setCursor(new Cursor(Cursor.DEFAULT_CURSOR));
    }

    // keep current mouse position.
    mx=x; my=y;

  }
  

   /**
   * Called if the mouse moves (the mouse button is up)
   * @param evt the event
   * @param x the x coordinate
   * @param y the y coordinate
   * @see java.awt.Component#mouseMove
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
     * @param evt the event
     * @param x the x coordinate
     * @param y the y coordinate
     * @see java.awt.Component#mouseExit
     */
  public void mouseExited(MouseEvent e)
  {
    int x = e.getX();
    int y = e.getY();
    
    mouseOnImageFlag = false;
    repaint();
    
    // change the cursor type to "cross" if possible to show the coordinate
    if (showCoordinate)
      ((Component) imageFrame).setCursor(new Cursor(Cursor.DEFAULT_CURSOR));    
    
      // set show coordinate flag
    setShowCoordinate(false);
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
    return retDat;
  }

  /** Return the coordinate info.
   */
  String  getCoordinateInfo(int x, int y) throws HDFException {
   
	// keep the dataset position
    	Rectangle datPos = null;
	datPos	         = new Rectangle(x+tx-startx, y+ty-starty, 0,0);

	// data value
	float	coorData = getData(x+tx-startx, y+ty-starty);

	return  ("[" + Integer.toString(x+tx-startx) + " , "
		+ Integer.toString(y+ty-starty)  + "]= " + Float.toString(coorData));
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

	jhvCanvas.setImage(images[imageNumber-1]);

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
    	   cvFlag = app.makeImageDataByRange(data,min,max, datatype,
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
}
