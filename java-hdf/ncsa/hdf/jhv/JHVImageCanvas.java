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
import ncsa.hdf.awt.palette.*;
import ncsa.hdf.awt.image.*;
import ncsa.hdf.awt.*;
import ncsa.hdf.awt.event.*;
import ncsa.hdf.awt.plots.*;
import ncsa.hdf.message.*;


/* This class implement full resolution image from HDF process,
  * by implementing the PaletteEditorAdapter interface the image
  * palette should be modified through  generic "Palette Editor"
  * implemented in seprate package -xlu 7/97 */

/** The image will be display on this canvas */
public class JHVImageCanvas extends Canvas implements PaletteEditorAdapter,
    Runnable, MouseListener, MouseMotionListener {

    /** the frame to involve the image canvas component */
    JHVImageFrame imageFrame;
  
    /** HDF Browser applet */
    JHV app;
 
    /** the canvas resonding to the selected HDF node  */
    JHVCanvas jhvCanvas;

    /** the selected HDF Object node */
    HDFObjectNode node;

    /** the current image */
    Image image    = null;
    Image preImage = null;

    /** a banch of images  for animation */
    Image[] images;

    /** image size  */
    int imageWidth, imageHeight;

    /** the raster image pixel value */
    byte imageData[];

    /** the palette value for current image */
    byte imagePalette[];

    /** the physical image data */
    Object hdfData;

    // dataset dimension  scale information
    Object xscale;
    Object yscale;
    int scaleDataType;

    boolean dataRangeFlag = false;

    // data range
    double min, max;

    /** the data number type */
    int hdfDataType = -1;

    /**   the total number of image */
    int numberOfImage = 1;
  
    /** current displayed image number */
    int imageNumber = 1;
  
    /** canvas size */
    int canvasWidth, canvasHeight;

    /** the indicator which the image frame has been popuped */
    boolean imageFrameDisplayed ;

    /** information for image canvas */
    static String infoStr = "Image Canvas";
          
    /** variables for duble-buffer  */
    Image offScreenImage = null;
    Graphics offGraphics;

    /** control of the annimation  */
    int actionFlag;

    /** the speed of animation */
    int speed = 0;

    /** the current thread for animation */
    Thread killer;

    /** the first coordinate value  of the displayed image */
    int startx = 1, starty = 1;
    
    /** translated variable for scroll panel */
    int tx=0,  ty=0;       // offset translate ..
    
    /** Scrollbar of the image canvas */
    Scrollbar hScrollbar, vScrollbar;
    
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
    public boolean paletteEditorOpen = false;

    // for image color model
    public ColorModel colorModel = null;
    public ColorModel oldColorModel = null;

    // for current color model palette
    public byte[] colorModelPalette  = null;

    // image producer ...
    public ImageProducer imageSource = null;
    public ImageProducer[] imageSources = null;

    // PaletteEditor instance ....
    PaletteEditor paletteEditor;

    // indicator to display the coordinates.
    boolean showCoordinate = false;

    // coordinate info.
    String[] coorInfo;

    // subset range of an image
    Rectangle subsetRange;

    // display image value mode, pixel or digital value
    int dispImageDataMode = JHVImageFrame.NONE_VALUE;
    int imagePlotMode = JHVImageFrame.NONE_PLOT;

    // plot of  histogram and radial plot
    XYPlot aPlot, hPlot;
   
    // sds slice info.
    SDSDimInfo sliceInfo;

    int lastModifiedNumber = -1;

    /**
     * new constructor for image canvas
     * @param frame the frame of this object
     */
    public JHVImageCanvas(JHVImageFrame frame)
    {
        imageFrame = frame;
        app = frame.applet_;
        imageFrameDisplayed = false;
        addMouseListener(this);
        addMouseMotionListener(this);
    }

    /** new constructor for image canvas
     * @param frame the frame of this object
     */
    public JHVImageCanvas(JHVImageFrame frame, Rectangle subset)
    {
        this(frame);
        this.subsetRange = subset;
    }

    /**
     * Initialize  HDF object based on the selected node.
     */
    public boolean initHDF()
    {
        boolean retVal = true;

        // disable adjustMenuItem
        imageFrame.adjustMenuItem.setEnabled(false);

        try {
            switch(node.type) {
                case HDFObjectNode.RIS8:
                    readRIS8();
                    break;
                case HDFObjectNode.RIS24:
                    readRIS24();
                    break;
                case HDFObjectNode.GRDATASET:
                    readGR();
                    break;
                case HDFObjectNode.SDSDATASET:
                    readSDSImages();
                    imageFrame.adjustMenuItem.setEnabled(true);
                    break;
                case HDFObjectNode.Vdata:
                case HDFObjectNode.Vgroup:
                    break;
            }
        } catch (HDFException e) {};

        System.gc();

        // set orignal image source
        imageSources = new ImageProducer[numberOfImage];
        for (int kk=0; kk<numberOfImage; kk++)
            imageSources[kk] = images[kk].getSource();

        return retVal;
    }

    /**
     * set the number of the image
     * @param num the number of the image
     */
    public void setNumberOfImage(int num) {
        numberOfImage = num;
    }


    /** create the raster image by  specified image data
     * @param imgData the image data(pixel value)
     * @param w the width of the image
     * @param h the height of the image
     * @param imgPalette the palette of the image
     * @param index      the plane number of the image
     * @return the image , otherwise null
     */
    public Image createRasterImage(byte[] imgData,
        int w, int h, byte[] imgPal,int index)
    {

        ImageDataConverter convert = new ImageDataConverter();
        Image img =  convert.createRasterImage(imgData,w,h,imgPal,index);
           
        int loopCount = 0;
        while ( img.getWidth(this) < 0 || img.getHeight(this) < 0 )
        {
            try { Thread.sleep(10); } 
            catch (Exception e) {};

             loopCount++;
             if (loopCount > 6) break;
        }  // until image construction is finished

        return (Image)img;   
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

    /**
     * set the current image & repaint it with current image 
     * This saves the current image as the previous image (in
     * a stack that is depth 1)
     * @param img the image
     */
    public  void setImage(Image img) {
        // keep current image as previous image
        preImage = image;

        //set image
        image = img;

        repaint();
    }

    public  void updateImage(Image img) {
        //set image
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

    public int[] getImagePixelValue(Image img, int x, int y, int w, int h)
    {

        if (((w*h)<=0) || ((x*y) <0)) return null;

        // return value
         int[] pixels = new int[w * h];

         PixelGrabber pg = new PixelGrabber(img, x, y, w, h, pixels, 0, w);
         try { pg.grabPixels(); }
         catch (InterruptedException e) {
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
    public void setImageFilter(ImageFilter imageFilter)
    {
    
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
    public synchronized Image getImage(int planeNumber)
    {
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
                 retImage = images[planeNumber-1];
                 break;
            case HDFObjectNode.Vdata:
            case HDFObjectNode.Vgroup:
                 break;
        }

        // set image frame
        setImageFrame(planeNumber);

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
        if (sp <= 0)  sp = 0;
        else if (sp >= 1000) sp = 1000;

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
    g.setFont(new Font("Courier", Font.PLAIN, 12));
    FontMetrics lFM = g.getFontMetrics();
    int w = lFM.stringWidth("012.3456789")*3 ;

     for (int i=0; i<lines; i++) {
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
      // String dispStr = "Image Number: " + Integer.toString(imageNumber);
      // g.drawString( dispStr, 1, h-6);   // default font height = 9
      // String dispStr = Integer.toString(imageNumber);
      // imageFrame.imgField.setText(dispStr);

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
                
        // String infoStr[] = new String[3];
        // infoStr[0] = coorInfo;
        String infoStr[] =  coorInfo;

        int lines = 3;
        if (infoStr[2].length()==0)
           lines = 2;

        // my:: the mouse position
        if (my<60) { // write in bottom
    
                // write info. on the top of the image
                // writeInfo(g, startx, h-30, infoStr,1);
                writeInfo(g, 2, h-60, infoStr,lines);
        } 
        else {
                // write info. on the top of the image
                // writeInfo(g, startx, 0, infoStr,1);
                writeInfo(g, 2, 0, infoStr,lines );
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
      try {
          readSDS(imageNumber);
          new JHVDataFrame(imageFrame, datasetRange);
      }
      catch (Exception ex) {}
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
                    radialPlots(new Rectangle(0,y-starty+ty,1, 0 )); 
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
        double[][] pArray = new double[2][rect.width];
        int firstPos = 0;

        for (int i=x; i<x+rect.width; i++) {
            double retDat = (double)getData(i, y);
            rArray[i-x] = retDat;
        }

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
           aPlot= new XYPlot(new Frame(), "Radial Plot", pArray);
           new Dialog(imageFrame, "what");
        }
        else {
           aPlot.setData(pArray);
           aPlot.show();
        }

        aPlot.setXAxisTag(xStr);
        aPlot.setYAxisTag("No Value");
        aPlot.setYAxisRange(0,0);

        if (dispImageDataMode == JHVImageFrame.PIXEL_VALUE)
        {
           aPlot.setYAxisTag("Pixel Value");
           aPlot.setYAxisRange(0,255);
        }
        else if (dispImageDataMode == JHVImageFrame.DIGIT_VALUE)
        {
            aPlot.setYAxisTag("Digital Value");
            if (dataRangeFlag)
                aPlot.setYAxisRange(min,max);
            else {
                double yMin, yMax, tmp;
                yMin = yMax = rArray[0];
                for (int i=0; i<rArray.length; i++) {
                    tmp = rArray[i];
                    yMin = Math.min(tmp, yMin);
                    yMax = Math.max(tmp, yMax);
                }
                aPlot.setYAxisRange(yMin, yMax);
            }
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
           hPlot = new XYPlot(new Frame(), "Histogram Plot", pArray);
           // hPlot = new XYPlot(imageFrame, "Histogram Plot", pArray);
           
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
        {
           hPlot.setXAxisRange(minVal,maxVal);
           hPlot.setXAxisTag("Digital Value");
        }
        else
        {
           hPlot.setXAxisRange(0,255);
           hPlot.setXAxisTag("Pixel Value");
        }

        hPlot.setYAxisTag("Number of Pixels");
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
           Rectangle  plotLine = new Rectangle(0,my-starty+ty,getImageWidth(), 0 );  
            // radial plots test
               radialPlots(plotLine); 
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
      if (mouseOnImageFlag) {

        if (lastModifiedNumber != imageNumber &&
            node.type == HDFObjectNode.SDSDATASET)
        {
            try {readSDS(imageNumber);}
            catch(Exception ex) {}
        }

        // get coordinate info.
         coorInfo =  getCoordinateInfo(mx,my);
       }
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
        if (lastModifiedNumber != imageNumber)
        {
            try {readSDS(imageNumber);}
            catch(Exception ex) {}
        }

        // open the seprate spreadsheet
        new JHVDataFrame(imageFrame, datasetRange);
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
  
   float retDat;
     
   // get the first address to get the raw data from hdfData
   int firstPos = 0;

   // digital value display
   if (dispImageDataMode == JHVImageFrame.DIGIT_VALUE)  
        retDat = getData(hdfData,hdfDataType,x,y);
   else if (dispImageDataMode == JHVImageFrame.PIXEL_VALUE) {
          
   // the image dataset range
   int w = getImageWidth();
   int h = getImageHeight();

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

  /** Return the coordinate data
   */
  float  getData(Object data, int nt, int x, int y) throws HDFException {

   float retDat;
    
   // the image dataset range
   int w = getImageWidth();
   int h = getImageHeight();
   
   // get the first address to get the raw data from hdfData
   int firstPos = 0;
 
    if ((nt == HDFObjectNode.RIS24) ||
        (nt == HDFObjectNode.GRDATASET)) {
         // adjust the first data position
         firstPos = w * h * (imageNumber - 1);
    }
   
    // first extract number position by row
    int pos = (y * w + x);

    // adjust the pos.
    pos += firstPos;
        
    return ImageDataConverter.getData(data,nt,pos);
  }

  /** Return the coordinate data
   */
  float  getScaleData(Object data, int nt, int pos) throws HDFException {

        return ImageDataConverter.getData(data,nt,pos);  
  }


    /**
     *  get the coordinate information
     *  @param x   the horizontal coordinate
     *  @param y   the vertical coordinate
     *  @return    the string array of the coordinates and value
     */
    String[]  getCoordinateInfo(int x, int y) throws HDFException
    {
        String retStr[] = new String[3];
        float coorData = getData(x+tx-startx, y+ty-starty);
        int cx = x+tx-startx+subsetRange.x;
        int cy = y+ty-starty+subsetRange.y;

        if (xscale==null) { // not found dimension scale value
            retStr[0] = "Coordinates: " + cx + ", " + cy;
            retStr[1] = "Value:       " + coorData;
            retStr[2] = "";
        }
        else {
            retStr[0] = "Coordinates: " + cx + ", " + cy;
            retStr[1] = "Dim Scales:  " + getScaleData(xscale,scaleDataType,cx)
                + ", " + getScaleData(yscale,scaleDataType,cy);
            retStr[2] = "Value:       " + coorData;
        }
        return retStr;
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
        Object datObject = data;
        return getSDSImage(datObject,min,max,datatype,w,h,imageData,palette);
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
    public Image getSDSImage(Object data, double min, double max, int datatype,
                             int w, int h,
                             byte[] imageData, byte[] palette) {

        Image retImage;

        // make image data
              boolean cvFlag = true;
            cvFlag = ImageDataConverter.makeImageData(data,min,max, datatype,
                                                       w,h,
                                                       0, imageData);

        if (!cvFlag) 
           retImage = null;
        else { // create new image 
           retImage = createRasterImage(imageData,w,h,palette,1);
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

  // set sds slice info.
  public void  setSliceInfo(SDSDimInfo info) {
        this.sliceInfo = info;
  }

  // return sds slice info.
  public SDSDimInfo  getSliceInfo( ) {
        return this.sliceInfo ;
  }


  // return images
  public Image[]  getImages( ) {
        return this.images;
  }

  // set image frame
  public void setImageFrame(int frame) {
     if (imageFrame!=null) {
        String dispStr = Integer.toString(frame);
        imageFrame.imgField.setText(dispStr);
     }
  }

    /**
     *  read and display SDS images for animation
     *  @auther  Peter Cao (xcao@ncsa.uiuc.edu)
     */
    public void readSDSImages() throws HDFException
    {
        // read the first and do initialization
        try { readSDS(1); }
        catch (Exception ex) {}

        images = new Image[numberOfImage];
        image = createRasterImage(imageData,imageWidth,imageHeight,imagePalette,1);
        images[0] = image;

        //get all the images
        node.isPreview = false;
        node.selectedRange = subsetRange;
        node.plane = numberOfImage+1;
        ncsa.hdf.message.HDFSDS sds = (ncsa.hdf.message.HDFSDS) app.getHDFObject(node);
        byte imageByteData[][] = sds.getImages();
        for (int i=1; i<numberOfImage; i++)
            images[i] = createRasterImage(imageByteData[i-1],imageWidth,imageHeight,imagePalette,1);
        node.plane = 1;

        // repaint image
        setImage(images[0]);

        // clear up unused spaces
        sds = null;

    }

    /**
     *  display the 8 bit raster image
     *  @auther   Peter Cao  (xcao@ncsa.uiuc.edu)
     */
    public void readRIS8() throws HDFException
    {
        // added by Peter Cao on 10-6-97 for remote access
        //HDFRIS8 ris8 = (HDFRIS8) node.hdfObject;
        HDFRIS8 ris8 = (HDFRIS8) app.getHDFObject(node);
        Object ris8Data = ris8.getImageData();
        int[] imageArgv = ris8.getImageArgv();
        int dataType = imageArgv[1];
        Dimension imageSize = ris8.getCurrentImageSize();
        int w = imageSize.width;
        int h = imageSize.height;
        imageData = new byte[w*h];
        //double minmax[] = {Double.MAX_VALUE, Double.MIN_VALUE};
        HDFObject.object2byte(ris8Data, dataType, null, 0, imageData);
        imagePalette = ris8.getImagePalette();

        // set image palette
        if (imagePalette == null)
        {
            imagePalette  = new byte[256*3];
            for (int i=0; i<256; i++)
                for (int j=0; j<3; j++)  
                    imagePalette[i*3+j] = (byte)i;
        }

        // set raw data
        hdfData = imageData;
        hdfDataType = HDFConstants.DFNT_UINT8;

        setNumberOfImage(1);
        images = new Image[1];
        images[0] = createRasterImage(imageData, imageWidth, imageHeight, imagePalette,1);          
        setImage(images[0]);
    }

    /**
     * read 24-raster  image data from the HDF file 
     * @auther  Peter Cao (xcao@ncsa.uiuc.edu)
     */
    public void readRIS24() throws HDFException
    {
        // added by Peter Cao on 10-6-97 for remote access
        //HDFRIS24 ris24 = (HDFRIS24) node.hdfObject;
        HDFRIS24 ris24 = (HDFRIS24) app.getHDFObject(node);
        Object ris24Data = ris24.getImageData();
        int[] imageArgv = ris24.getImageArgv();
        int dataType = imageArgv[1];
        Dimension imageSize = ris24.getCurrentImageSize();
        int w = imageSize.width;
        int h = imageSize.height;
        //imageData = new byte[w*h*3];
        imageData = new byte[w*h];
        //double minmax[] = {Double.MAX_VALUE, Double.MIN_VALUE};
        HDFObject.object2byte(ris24Data, dataType, null, 0, imageData);
        imagePalette = ris24.getImagePalette();
        hdfData = imageData;
        hdfDataType = HDFConstants.DFNT_UINT8;

        setNumberOfImage(1);
        images = new Image[1];
        images[0] = createRasterImage(imageData, imageWidth, imageHeight, imagePalette,1);
        setImage(images[0]);

        /*
        setNumberOfImage(3);
        images = new Image[3];

        for (int i=1; i<=3 ;i++)
          images[i-1] = createRasterImage(imageData, imageWidth, imageHeight, imagePalette,i);
        setImage(images[0]);
        */
    }

    /**
     * read and display the GR image
     * @auther Peter Cao (xcao@ncsa.uiuc.edu)
     */
    public void readGR() throws HDFException
    {
        // added by Peter Cao on 10-6-97 for remote access
        //double minmax[] = {Double.MAX_VALUE, Double.MIN_VALUE};
        node.isPreview = false;
        node.selectedRange = subsetRange;
        HDFGR gr = (HDFGR)app.getHDFObject(node);
        Object grData = gr.getImageData();
        hdfData = grData;
        imagePalette = gr.getImagePalette();
        int imageArgv[] = gr.getImageArgv();
        int dataType = imageArgv[1];
        node.isPreview = true;
        node.selectedRange = new Rectangle();
 
        this.hdfDataType = imageArgv[1];
        setNumberOfImage(imageArgv[0]);
        images = new Image[imageArgv[0]];
        int w = imageWidth;
        int h = imageHeight;
        imageData = new byte[w*h];

        images = new Image[numberOfImage];
        for (int i=1; i<=numberOfImage ;i++)
        {
            HDFObject.object2byte(grData, dataType, null, w*h*(i-1), imageData);
            images[i-1] = createRasterImage(imageData,imageWidth,imageHeight,imagePalette,1);
        }

        // repaint image
        setImage(images[0]);
    }

    /**
     *  read and display SDS image for a given plane
     *
     *  @param   plane  The plane number of the image
     *  @auther  Peter Cao (xcao@ncsa.uiuc.edu)
     */ 
    public void readSDS(int plane) throws HDFException
    {
        // added by Peter Cao on 10-10-97 for remote access
        ncsa.hdf.message.HDFSDS sds = null;
        imageData = new byte[imageHeight*imageWidth];
        Object sdsData = null;
        boolean cvFlag = false;
        int imageArgv[] = null;
        int dataType = -1;

        // the first call to the server will set the number of images
        node.isPreview = false;
        node.selectedRange = subsetRange;
        node.plane = plane;
        sds = (ncsa.hdf.message.HDFSDS) app.getHDFObject(node);
        imagePalette = sds.getImagePalette();
        sdsData = sds.getImageData();
        double range[] = sds.getDataRange();
        imageArgv = sds.getImageArgv();
        dataType = imageArgv[1];
        ncsa.hdf.message.HDFObject.object2byte(sdsData, dataType, range, imageData);
        setNumberOfImage(imageArgv[0]);
        this.hdfDataType = imageArgv[1];
        hdfData = sdsData;
        lastModifiedNumber = imageNumber;
    }

}
