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




// -------------------------------------------------
//
// Upgraded to 1.1.1 by Apu Kapadia.
// May 20, 1997.
//
// -------------------------------------------------



import  ncsa.hdf.hdflib.*;

import java.awt.*;
import java.awt.event.*;
import java.awt.image.*;
import java.net.*;
import java.applet.*;
import java.io.File;
import java.io.InputStream;
import java.io.IOException;
import java.util.*;
import java.net.*;

/**
 * JHV  is to display the HDF tree structure
 * Modification:
 * 	Keep track of current HDF directory, and selected HDF file will be
 * located under previous selected HDF file directory.  Also improve the  
 * "Load File" and let it do more work than "Open file".  08/08/97 xlu
 *
 * @author HDF Group, NCSA
 */
public class JHV extends Applet implements ActionListener {

 static {
                System.loadLibrary("hdf");
        }
 static {
                System.loadLibrary("jhv");
        }

  // my frame
  Frame    	jhvFrame;

  /** HDFTree object */
  HDFTree	hdfTree;

  /** information panel to be as a test area */
  TextArea      infoText;

  /** canvas to display the image responding to the appropriate tree node */
  JHVCanvas	hdfCanvas;

  JHVCanvas     hdfIconCanvas;
    
  // some of icons
  Image         hdfIcon = null;
    
  /** the default icon for the tree node that is not a final tree node */
  Image         lDefault;

  /** the icon for the tree node */
  Image         lFile;

  /** the icon for the tree node has been expended */
  Image         lCollapse;

  /** the hdf annotation icon */
  Image 	annImg;
  
  /** the hdf raster image icon */
  Image risImg;
 
  /** the hdf sds icon */
  Image  sdsImg;

  /** the hdf vdata icon */
  Image vdImg;

  /** the hdf Vgroup icon */
  Image  vgImg= null;

  // for setup
  boolean 	iconLoaded = false;

  /** the hdf file source */
  TextField	hdfFileText;

  /** hdf file name */
  String	hdfFile;

  boolean 	isLocalFile = false;
//public static final String  HDFFILENAME  = "/usr/tmp/tmp.hdf";

int fid;
	
  /** the queue to store the hdf object */
  public  Queue hdfObjQueue = null;
  
  /** the generic hdf object */
  public HDFLibrary 	hdf;

  // variables for duble-buffer
  Image 	offScreenImage = null;
  Graphics 	offGraphics;
    
  // where an HDF file is from?
  boolean isLocal = true;
    
  // Directory that JHV run
  String  jhvDir = ".";

  String  jhvIconDir = ".";
    
  // JHV as applet?
  boolean isApplet = false;
    
  // Vdata frame
  JHVVdataFrame  vdataFrame = null;
  
  boolean  vdataFrameDisplayed = false;

  // define display level
  public static final int USER      = 0;
  public static final int DEVELOPER = 1;

  // define a display level for HDF tree
  public 	int	dispLevel   = USER;

  // current hdf file directory
  String 	cDir;

  /**
   * It is called automatically by the system the application is started.
   */
  public static void main(String args[]) {
         	    	
    // create new jhv object 
    JHV	jhv = new JHV();

    String tmpHDFFile = "";
    int i;
    for (i = 0; i < args.length; i++) { 
       if ("-jhvpath".equalsIgnoreCase(args[i])) {
	    // get jhv directory
	    File tmpFile = new File(args[i+1]);
	    if (tmpFile.isDirectory()) 
		     jhv.jhvDir = tmpFile.getPath();
	     else
		     jhv.jhvDir = tmpFile.getParent();
	    i++;
	} else if ("-jhviconspath".equalsIgnoreCase(args[i])) {
	    File tmpFile = new File(args[i+1]);
	    if (tmpFile.isDirectory()) 
		     jhv.jhvIconDir = tmpFile.getPath();
	   i++;

	} else {
		if (args[i] != null) 
		   tmpHDFFile = new String(args[2]);    	
	}
    }

    //   set current hdf file directory
    jhv.cDir = jhv.jhvDir;

    jhv.init(tmpHDFFile);
    
    Frame frame = new Frame();
    frame.setTitle("HDF Viewer");
    WindowListener adapter = new WindowAdapter() 
    { public void windowClosing(WindowEvent e) {System.exit(0);} };
    frame.addWindowListener(adapter);
    
    frame.setLayout(new BorderLayout());
    // set menu bar
    frame.setMenuBar(jhv.createHdbMenuBar());
    
    frame.add("Center", jhv);
    
    frame.pack();
    
    frame.setSize(600,500);
    frame.show();
    
    // set my defuly frame
    jhv.setJHVFrame(frame);
    
  }

  public void actionPerformed(ActionEvent e)
  {
    String arg = e.getActionCommand();

     if (arg.equals("Open")) {
	
	// an HDF file source 
	openFileOnLocal();
	setup(hdfFile);
	hdfTree.refresh();
	
	// clean up the hdfCanvas
	hdfCanvas.setImage(null);
	hdfCanvas.repaint();		
	
      }
      
      else if (arg.equals("Exit")) {
	System.exit(0);
      }

    else if (arg.equals("Clean")) {
      
      infoText.setText("");
      infoText.select(0,0);
    }

    else {

    // open file
    if (arg.equals("Load File")) {
       hdfFile = new String(hdfFileText.getText());
       // an HDF file source 
       if (hdfFile.indexOf("://") !=  -1) {
          // supported by URL, This feature is not supported yet
          this.isLocal = false;
       }
       else  {
           this.isLocal = true;
           if (!isFile(hdfFile)) {
		if (isDirectory(hdfFile)) 
		   cDir = hdfFile;
                openFileOnLocal();
	   }
       }
      
      setup(hdfFile);
      if (!isLocal) {
	infoText.setText("Remote file access is not supported yet.");	
      }
      hdfTree.refresh();
      
      // clean up the hdfCanvas
      hdfCanvas.setImage(null);
      hdfCanvas.repaint();		
    }
   
    }
  }
  

  /**
   * Initialize the applet. Resize and load images.
   */
  public void init() {
    
    
    int width;
    int height;
 
    isApplet = true;
    
    if (!isLocal) {
	
	// for applet 
	String dataSource = getParameter("hdfFile");
	hdfFile = (dataSource != null) ? dataSource : "http://hdf.ncsa.uiuc.edu:4321/hdf/jet2.hdf";  
    
	String at = getParameter("width");
	width = (at !=null) ? Integer.parseInt(at):600;
	
	at = getParameter("height");
	height= (at != null) ? Integer.parseInt(at):400;
    }
    else {
	width = 600;
	height = 400;
	hdfFile = new String("/hdfscr2/xlu/tmp/testfile/hdf1.hdf" );
    }
	
    // create a HDFTree control
    hdfTree = new HDFTree();

    // set layout manager
    setLayout(new BorderLayout());
 
    // information panel
    infoText = new TextArea();
      
    // create hdf file by downloading the hdf file via http or file , ftp etc. 
    setup(hdfFile);

    // orgnize the panel
    add("Center",createDisplayPanel());
    if (!isLocal) {
	infoText.setText("Remote file access is not supported yet.");	
    }
  
    // resize the default size for the HDF Browser
    setSize(width,height);
    
    // get the initialized tree structure
    hdfTree.refresh();

  }
  
  

  /**
   * Initialize jhv with specified hdf file
   */
  public void init(String hdffile) {

    hdfFile = new String(hdffile);

    // create a HDFTree control
    hdfTree = new HDFTree();

    // set layout manager
    setLayout(new BorderLayout());
 
    // information panel
    infoText = new TextArea();
      
    // create hdf file by downloading the hdf file via http or file , ftp etc. 
    setup(hdfFile);

    // orgnize the panel
    add("Center",createDisplayPanel());
    if (!isLocal) {
	infoText.setText("Remote file access is not supported yet.");	
    }
  
    // resize the default size for the HDF Browser
    setSize(600,400);
      
    // display hdf icon
    hdfIconCanvas.setSize(32,32);
    hdfIconCanvas.setImageSize(30,30);
    hdfIconCanvas.setImage(hdfIcon);
    
    // get the initialized tree structure
    hdfTree.refresh();

  }
  
  

  /** initialize some variables
   * @param hdfFileName the file name 
   */
  public void setup(String hdfFileName) {
  
    // clean up the HDF object Queue
    if (hdfObjQueue == null)
      hdfObjQueue = new Queue();
    else
      hdfObjQueue.removeAllElements();
    
    // close the opened hdf file if possible	
    if (hdf != null) {
      if (fid != HDFConstants.FAIL) {
		try {
		    hdf.Hclose(fid);
		} catch (Exception e) {
			infoText.setText("Exception closing file: "+fid);
		//	System.out.println("Exception closing file: "+fid);
		}
	}
    } else {
	hdf = new HDFLibrary();
    }
    
    // set the hdf file
    if (hdfFileName.equals("")) {
	hdfFile = null;
    }
    else {
        setHdfFile(hdfFileName);
    }
    
    // load the icons into
    if (!(iconLoaded)) {
      
      iconLoaded = true;
      
      if (!isApplet) {
	    
	// load images

	String defaultImg = new String(jhvIconDir +"/default.gif");
	String fileImg = new String(jhvIconDir+"/file.gif"); 
	String collapseImg = new String(jhvIconDir+"/collapse.gif");
	
	// icons for hdf
	String hdfStr= new String(jhvIconDir+"/hdf.gif");
	String annStr= new String(jhvIconDir+"/ann.gif");
	String risStr= new String(jhvIconDir+"/ris.gif");
	String sdsStr= new String(jhvIconDir+"/sds.gif");
	String vdStr = new String(jhvIconDir+"/vdata.gif");
	String vgStr = new String(jhvIconDir+"/vgroup.gif");

	Toolkit toolkit = getToolkit();
	
        lDefault = toolkit.getImage(defaultImg );
        lFile    = toolkit.getImage(fileImg    );
        lCollapse= toolkit.getImage(collapseImg);
      
	hdfIcon= toolkit.getImage(hdfStr);
        annImg = toolkit.getImage( annStr);
        risImg = toolkit.getImage( risStr);
        sdsImg = toolkit.getImage( sdsStr);
        vdImg  = toolkit.getImage(  vdStr);
        vgImg  = toolkit.getImage(  vgStr);  
      }
      else { 
        // applet  running
	// load images
	String defaultImg = new String("hdficons/default.gif");
        String fileImg = new String("hdficons/file.gif");
        String collapseImg = new String("hdficons/collapse.gif");
      
        // icons
	String hdfStr= new String("hdficons/hdf.gif");
        String annStr= new String("hdficons/ann.gif");
        String risStr= new String("hdficons/ris.gif");
        String sdsStr= new String("hdficons/sds.gif");
        String vdStr = new String("hdficons/vdata.gif");
        String vgStr = new String("hdficons/vgroup.gif");  
     	
        lDefault = getImage(getDocumentBase(),defaultImg );
        lFile    = getImage(getDocumentBase(),fileImg    );
        lCollapse= getImage(getDocumentBase(),collapseImg);
        
	hdfIcon= getImage(getDocumentBase(), hdfStr);
        annImg = getImage(getDocumentBase(), annStr);
        risImg = getImage(getDocumentBase(), risStr);
        sdsImg = getImage(getDocumentBase(), sdsStr);
        vdImg  = getImage(getDocumentBase(),  vdStr);
        vgImg  = getImage(getDocumentBase(),  vgStr);
      }    
    }
    
    // get all hdf object
    Vector hdfObjVec = new Vector();
    getHdfObj(this.hdfFile, hdfObjVec);

    // get hdf object, default-> USER
    Vector objVec = getDispObjNode(hdfObjVec, dispLevel);

    //  cleanup HDF tree vector
    hdfTree.removeAllTreeNodes();
    
    // Declare a TreeNodes
    TreeNode addedNode;
    
    // add the TreeNodes to the tree.
    for (int i = 0; i < objVec.size(); i++) {
      
      HDFObjectNode objNode;
      objNode = (HDFObjectNode)objVec.elementAt(i);

      // node has child
      if (objNode.child != null)
	addedNode = new HDFTreeNode(objNode, lDefault, lCollapse, this);
      else  {
	
	Image img = null;
	switch (objNode.type) {
	case HDFObjectNode.Annotation: 
	case HDFObjectNode.GRGLOBALATTR:
	case HDFObjectNode.GRDATASETATTR:
	case HDFObjectNode.SDSGLOBALATTR:
	case HDFObjectNode.SDSDATASETATTR:
	  
	     img = annImg;
	     break;
	case HDFObjectNode.Palette: 
	     img = lFile;
	     break;
	case HDFObjectNode.RIS8: 
	case HDFObjectNode.RIS24:	    
	case HDFObjectNode.GRDATASET: 
	     img = risImg;
	     break;
	case HDFObjectNode.SDSDATASET: 
	     img = sdsImg;
	     break;
	case HDFObjectNode.Vdata: 
	     img = vdImg;
	     break;
	case HDFObjectNode.Vgroup: 
	     img = vgImg;
	     break;
	}

	if (img == null)
	   addedNode = new HDFTreeNode(objNode, lFile, this);
	else
	   addedNode = new HDFTreeNode(objNode, img, this);
      }
      
      /* set the level of the node */
      addedNode.setLevel(objNode.getObjLevel());
      
      hdfTree.addTreeNode(addedNode);
      
    }

    if (isLocal) {  /* remote access not supported yet */
    if (hdfFile != null) {
	    // open file if one has been given
	try {
	    fid = hdf.Hopen(this.hdfFile);   
	} catch (Exception e) {
		infoText.setText("Exception opening file: "+this.hdfFile+
                                 "\nPlease check if the file is an hdf file");
	    //System.out.println("Exception opening file: "+this.hdfFile);
	}
    }
    }
  
  }

  /** create the new display panel for JHV
   * @return the panel for HDF Browser
   */
  public Panel createDisplayPanel() {
  
    Panel 	dispPanel = new Panel();
    dispPanel.setLayout(new BorderLayout());
    
    // hdf hierarchy tree canvas & HDF canvas
    Panel  panel1 = new Panel();
    panel1.setLayout(new GridLayout(1, 2));
    
    // create hdf canvas;
    hdfCanvas = new JHVCanvas(this);
  
    Panel title = new Panel();
    title.setLayout(new BorderLayout());
    hdfIconCanvas = new JHVCanvas();
    
    title.add("West",  hdfIconCanvas);
    title.add("Center",new Label("Hierarchy of the HDF", Label.CENTER));
    
    // hdf tree panel
    Panel hdfTreePanel = new Panel();
    hdfTreePanel.setLayout(new BorderLayout());
    hdfTreePanel.add("North", title);
    hdfTreePanel.add("Center", hdfTree);
    
    
    panel1.add(hdfTreePanel);
    panel1.add(hdfCanvas = new JHVCanvas(this));
    
    // control panel (middle)
    Panel panel2 = new Panel();
    
    // botton or label
    Panel panel2_right = new Panel();
    panel2_right.setLayout(new FlowLayout(FlowLayout.CENTER));
    panel2_right.add(new Label(""));
    
    Button LoadFileButton = new Button("Load File");
    Button CleanButton = new Button("Clean");
    LoadFileButton.addActionListener(this);
    CleanButton.addActionListener(this);
    panel2_right.add(LoadFileButton);
    panel2_right.add(CleanButton);
    
    
    panel2.setLayout(new BorderLayout());
    
    // HDF file text field
    if (hdfFile != null) {
	hdfFileText = new TextField(hdfFile);
    } else {
	hdfFileText = new TextField("No File Selected");
    }
    
    panel2.add("Center", hdfFileText);
    panel2.add("East",   panel2_right);
    
    // info
    Panel panel3 = new Panel();
    panel3.setLayout(new BorderLayout());
    panel3.add("Center", infoText);
    
    // main panel
    Panel panel4 = new Panel();
    panel4.setLayout(new BorderLayout());
    panel4.add("North", panel2);
    panel4.add("Center",panel3);
    
    // display panel
    dispPanel.setLayout(new GridLayout(0,1));
    dispPanel.add("North", panel1);
    dispPanel.add("Center",panel4);
    
    return dispPanel;
  }   

  /* Is the current platform MacOS */
  public  boolean isMac() {
        String          osName;

        osName = System.getProperty("os.name");
        if (osName != null) {
	   if  (osName.toLowerCase().startsWith("mac")) 
                return true;
	   else
	   	return false;
        }
	else 
          return false;
    }

  /** Get the file name that is for MacOS
   * @param fileName an hdf file name
   * @return the MacOS-style file name
   */
  public String getFileNameForMacOS(String fileName) {

	String newStr;
   
   	// Ignore the first separator
	if (fileName.indexOf('/') == 0)
	   newStr = new String(fileName.substring(1));
	else
	   newStr = new String(fileName);

	
     	// replace "/" with ":"
	newStr = newStr.replace('/',':');

	return(newStr);
    
  }

  

 

  /** set hdf file name and create new hdf.
   * @param fileName hdf file name
   */
  public void setHdfFile(String fileName) {
   
    String newFileName;

    if (isMac())
	newFileName = new String(getFileNameForMacOS(fileName));
    else
	newFileName = fileName;	

    // create new hdf 
    // openHDF(newFileName);

    //if (!isLocal) 
     //  hdfFile = new String(HDFFILENAME);
    //else
     //  hdfFile = new String(newFileName);
       
  }

  

  /** get the HDF object from the HDF file and put in into the Vector
   * @param hdffile the HDF file name
   * @param hdfObjVector the vector to store the HDF object node
   */
  public void getHdfObj(String hdffile, Vector hdfObjVector  )  {
    
    if (!isLocal) return;
    if (hdffile == null) return;
    HDFAnalyse analyseHdf = new HDFAnalyse();
    analyseHdf.getHdfObject(hdffile, hdfObjQueue);
    
    // extract the HDF Object node from the Queue into Vector
    extractHdfObj(hdfObjQueue, hdfObjVector);
  }
  
  
  

  // get display object by display level
  public Vector getDispObjNode(Vector hdfObjVec, int level )  {
   
    // display HDF nodes
    Vector	objVec = new Vector();  

    if (level == USER) {
	
    	// display node by different level. (gv suggest)
   	// currently take all Reserved classes and names for vdatas/vgroups off
    	for (int i=0; i<hdfObjVec.size(); i++) {
   
      	    // get one node from the Queue
      	    HDFObjectNode node = (HDFObjectNode)hdfObjVec.elementAt(i);

	    if (node.isReadable())	// not readable
   	       objVec.addElement(node);
	}
   
       int vdataCount = 0;
       int vgroupCount= 0;
       for (int i=0; i<objVec.size(); i++) {

   	// get one node from the Queue
      	HDFObjectNode node = (HDFObjectNode)objVec.elementAt(i);
	if (node.type == node.Vdata)  ++vdataCount;
	if (node.type == node.Vgroup) ++vgroupCount; 

       }

       if (vdataCount == 1) // one vdata root or vdata in vgroup
 	  for (int i=0; i<objVec.size(); i++) {

   		// get one node from the Queue
      		HDFObjectNode node = (HDFObjectNode)objVec.elementAt(i);
		if ( (node.type == node.Vdata) && (node.level == 0) ) // root 
		   objVec.removeElementAt(i);
     	 }

      if (vgroupCount == 1) // one vgroup root
 	 for (int i=0; i<objVec.size(); i++) {

   		// get one node from the Queue
      		HDFObjectNode node = (HDFObjectNode)objVec.elementAt(i);
		if (node.type == node.Vgroup) 
		   objVec.removeElementAt(i);
     	}

   } else {
	// level DEVELOPER 
	for (int i=0; i<hdfObjVec.size(); i++) {
   
      	    HDFObjectNode node = (HDFObjectNode)hdfObjVec.elementAt(i);
   	    objVec.addElement(node);

	}
   
   }

   return (Vector)objVec;

 }
  
  
  

  /** create the HDF file by the specified string. Support most kinds of 
   * file  transfer protocol
   * @param hdfURL the string specified the location of the file
   */
  //  I think this needs to be massively redone -- REMcG 6.18.97
  public void openHDF(String hdfURL)  {
  
    /* hdf file is from server */
   if (!isLocal) {

	InputStream dataStream;
	try {
	    
	    URL 	hdfFileURL = new URL(hdfURL);
	    
	    URLConnection hdfURLConnection = hdfFileURL.openConnection();
	    int fileSize = hdfURLConnection.getContentLength();
	    String fileType = new String(hdfURLConnection.getContentType());
	

	    dataStream = hdfURLConnection.getInputStream();  
	    
	} catch (IOException e) {
	    dataStream = null;    
	}
    }
    else {
	// create HDF instance
	hdf = new HDFLibrary();
    }
  }
  

  /** extract the HDF object node into Vector
   * @param hdfQueue  the HDF object node Queue
   * @param vec       the vector to store the HDF object node sequencely
   */
  public  void extractHdfObj(Queue  hdfQueue, Vector vec) {
    HDFObjectNode node     = null;
    Queue         subQueue = null;
	
    if (hdfQueue.size() == 0)
      return;
    for (int i=0; i<hdfQueue.size(); i++) {
      
      // get one node from the Queue
      node = (HDFObjectNode)hdfQueue.elementAt(i);

      vec.addElement(node);
      
      // does this node has child
      if (node.child != null) { // has child & child is another Queue
	
	subQueue = (Queue) (node.child);
	extractHdfObj(subQueue, vec);
      }
    } 
    
  }  

    /** create menubar for jhv viewer */
    public MenuBar createHdbMenuBar() {
    
        MenuBar  jhvMenuBar = new MenuBar();
        jhvMenuBar.add(createFileMenu("File"));
    
        return  jhvMenuBar;
    }

   /** create a file menu by provoded title
    * @param menuTitle the menu title
    */
    public Menu createFileMenu(String  menuTitle) {
    
        Menu fileMenu = new Menu(menuTitle);
    
	MenuItem menuOpen = new MenuItem("Open");
	MenuItem menuExit = new MenuItem("Exit");
	menuOpen.addActionListener(this);
	menuExit.addActionListener(this);

        fileMenu.add(menuOpen);
    
        fileMenu.addSeparator();
    
        fileMenu.add(menuExit);
    
        return fileMenu;
    
    }

   /** Determines whether the specified string is a local file
    * @param str the file string
    * @return true if str is a file, otherwise false
    */
   public boolean isFile(String str) {
    
       File tmpFile = new File(str);
       return(tmpFile.isFile());
     
  }

   public boolean isDirectory(String str) {
    
       File tmpFile = new File(str);
       return(tmpFile.isDirectory());
     
  }

  public void openFileOnServer() {
  }

   /** open local file */

   public void openFileOnLocal() {
   
    this.isLocal = true;
				
    FileDialog fd = new FileDialog(getFrame(), "HDF File");

    // cDir is a directory of the last HDF file
    fd.setDirectory(cDir);		
    fd.show();
		
    hdfFile = null;

    // The system dependent file separator String.
    String separator = System.getProperty("file.separator");

	if ((fd.getDirectory() != null) && (fd.getFile() != null)) {
    if (isMac())
       hdfFile = fd.getDirectory() + separator + fd.getFile();
    else
       hdfFile = fd.getDirectory() + fd.getFile();
       cDir = fd.getDirectory()	;

	} else {
		hdfFile = new String("");
	}
	
    hdfFileText.setText(hdfFile);
    infoText.setText("");
    infoText.select(0,0);
    
  }

  public void setJHVFrame(Frame f) {

	jhvFrame = f;
  }

  public Frame getFrame() {
  
    Component c = (Component)this;
    while(c != null && !(c instanceof Frame))
      c = c.getParent();
    return (Frame)c;
  }  

  /**
   * Cleans up whatever resources are being held. If the applet is active
   * @see java.applet.Applet#destroy
   */
  public void destroy() {
    // close hdf file
	try {
    if (this.fid != HDFConstants.FAIL)
      hdf.Hclose(this.fid);
	} catch (HDFException e) {
		infoText.setText("Exception closing file: "+fid);
	};
    
    super.destroy();
    
  }

   public boolean makeImageDataByRange(byte[] data, double min, double max, 
                                int datatype, int w, int h, 
                                byte[] output)
   {

        return (makeImageDataByRange(data,min,max,datatype,w,h,0,output));
        
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
// can this be done in Java rather than C?  REMcG 6.18.97

   public native boolean makeImageData(byte[] data, int datatype, int w, int h, int pos,
                             byte[] output);
   

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

   public native boolean makeImageDataByRange(byte[] data, double min, double max,
                             int datatype, int w, int h, int pos,
                             byte[] output);
   
}

/** This class is actual HDF tree node derived from the HDFTreeNode and
 *  used by JHV. This class also define a bunch of methods to
 *  do something like the HDF operation 
 */
/* Date: 3-26-97
   For attribute of an HDF objects, add "Number type" & "Count" info.
   Need changed to the following methods:
	getHdfSDSgattr(), getHdfSDSattr(),
	getHdfGRgattr(),  getHdfGRattr(),
*/

class HDFTreeNode extends TreeNode
{
  // applet
  JHV 		applet_;
	
  // hdf 
  HDFLibrary		hdf;

  // the current generic image
  Image image = null;
  Image paletteImage = null;

  /** create the new HDF Tree node
   * @param obj  the generic object
   * @param img  the default icon for this node
   * @param app  the applet of this node that belongs to
   */
  public HDFTreeNode(Object obj, Image img, JHV app)
  {
    super(obj, img);
    applet_ = app;
  }

  /** create the new HDF Tree node for the node who has the child
   * @param obj  the generic object
   * @param img  the default icon for this node
   * @param app  the applet of this node that belongs to
   */
  public HDFTreeNode(Object obj, Image img, Image imgCollapsed, 
		     JHV app)
  {
    super(obj, img, imgCollapsed);
    applet_ = app;
  }

  /** add one HDF Tree
   * @param tree the HDF Tree
   */
  public void added(HDFTree tree)
  {
    super.added(tree);
  }

  /** delete one HDF Tree
   * @param tree the HDF Tree
   */
  public void  deleted(HDFTree tree)
  {
    super.added(tree);
  }

  /** select the node on the  HDF Tree
   * @param tree the HDF Tree
   */
  public  void  select(HDFTree tree, int modifier) {

   // set cursor type to "WAIT_CURSOR"
   ((Component)applet_.jhvFrame).setCursor(new Cursor(Cursor.WAIT_CURSOR));

    super.select(tree, modifier);


    // do something for HDF
    HDFObjectNode  node = (HDFObjectNode)(tree.selectedNode.hdfObject);
     
    // detect the type of the node
    int nodeType = node.getObjType();

    
    // open HDF file
    hdf = applet_.hdf;
	int fid = applet_.fid;

    try {
        // HDF Annotation node
        if (nodeType == node.Annotation)   
		dispHdfAnnotation(fid);
    	
        // HDF 8-raster image
        if (nodeType == node.RIS8) 
		dispHdfRis8(applet_.hdfFile, node);
    
        // HDF 24-raster image
        if (nodeType == node.RIS24) 
		dispHdfRis24(applet_.hdfFile, node);
    
        // HDF GR
        if ((nodeType == node.GRGLOBALATTR) 
		|| (node.type == node.GRDATASETATTR)
		|| (node.type == node.GRDATASET))  {
    
		dispHdfGR(fid, node);
        }
    
        // HDF SDS
        if ((nodeType == node.SDSGLOBALATTR) 
		|| (node.type == node.SDSDATASETATTR)
		|| (node.type == node.SDSDATASET)) {
    	 
		dispHdfSDS(applet_.hdfFile,node);
        }
       
        
        // HDF VDATA
        if (nodeType == node.Vdata) {
		// vdata processing
		dispHdfVdata(fid,node);
        }
    } catch (HDFException e) {
	System.out.println("Warning:  HDFException caught (select): "+e);	
    }
   
    // set cursor type to "DEFAULT_CURSOR"
    ((Component)applet_.jhvFrame).setCursor(new Cursor(Cursor.DEFAULT_CURSOR));

  }

  /** get the length of the char
   * @param chr the array of the char
   * @return the length of the array of the chr
   */
  public int getCharArrayLen(char[] chr)
  {
    int len = 0;	
    for (int i=0; i<chr.length; i++) {     
      if (chr[i] != '\0')
	++len;
      else
	break;
    }
    return len;   
  }
  

  /** clean up  the image panel */
  public void eraseImage() {
  
    applet_.hdfCanvas.setImage(null);
    
    applet_.hdfCanvas.repaint();
    
  }

  /** display the HDF file annotation in the information panel 
   * @param fid the HDF file identifier
   */
  public void dispHdfAnnotation(int fid) throws HDFException {
	
    // has File Annotation
    HDFLibrary hdf = applet_.hdf;
    
    int anid = hdf.ANstart(fid);

    if (anid == HDFConstants.FAIL) {
         hdf.ANend(anid);
        return;
	}
    
    hdf.ANend(anid);
    String an = new String(ANfileannotation());

    String annInfo = new String(" ==================== HDF File Annotation ==============\n");
    annInfo += an+"\n";
    annInfo += "==============================================\n";

    applet_.infoText.setText(annInfo);

    eraseImage();
  }
  

public String ANfileannotation() throws HDFException {

	HDFLibrary hdf = applet_.hdf;
	String retStr = new String(" ");
	int[] fileInfo = new int[4];
	int an_id =  hdf.ANstart(applet_.fid);

	if (hdf.ANfileinfo(an_id, fileInfo) == false ) {
		hdf.ANend(an_id);
	   return  retStr;
	}

	int numFileLabel = fileInfo[0];

	int numFileDesc  = fileInfo[1];

	int numFileAnn    =Math.max(numFileLabel, numFileDesc);

	for (int kk=0; kk < numFileAnn; kk++) {

		// get ann_id;
		int annid = hdf.ANselect(an_id, kk, HDFConstants.AN_FILE_LABEL);

 		if (annid != HDFConstants.FAIL)  {

		   retStr = retStr.concat("File Label #");
		   retStr = retStr.concat(Integer.toString(kk));
		   retStr = retStr.concat("\n\n");
	  	
		   // get the file label length
		   int fileLabelLen = hdf.ANannlen(annid)+1;
		   String fileLabelBuf = new String(" ");
		   String s[] = new String[1];
		   s[0] = fileLabelBuf;

		   if (fileLabelLen >0) {
	   	   	// get file label
	   		if (hdf.ANreadann(annid, s, fileLabelLen)) { 
			   retStr = retStr.concat(s[0]);
			}
		   }
		}

		hdf.ANendaccess(annid);

		annid = hdf.ANselect(an_id, kk, HDFConstants.AN_FILE_DESC);

		if (annid != HDFConstants.FAIL)  { 

		   retStr = retStr.concat("\nFile Desc. #");
		   retStr = retStr.concat(Integer.toString(kk));
		   retStr = retStr.concat("\n\n");
	  	
		   // get the file desc. length
		   int fileDescLen = hdf.ANannlen(annid)+1;

		   if (fileDescLen >0) {
	    	   	String fileDescBuf = new String(" ");
			String s[] = new String[1];
			s[0] = fileDescBuf;
	   	   	// get file desc.
	   		if (hdf.ANreadann(annid, s, fileDescLen))  {
			   retStr = retStr.concat(s[0]);
			}
		   }
		} 
		hdf.ANendaccess(annid);
	}
	hdf.ANend(an_id);
	return retStr;
   }

  /** display the HDF file 8-raster image generic information
   * @param filename the HDF file name
   * @param node the HDF Object node
   */
  public  void dispHdfRis8Info(String filename, HDFObjectNode node) throws HDFException {
	
	HDFLibrary hdf = new HDFLibrary(); 
	int argv[] = new int[2];
	boolean argB[] = new boolean[1];

    
    int numberOfImage = hdf.DFR8nimages(filename);
    String   info = new String(" ");
    
    // image number in HDF file
    info = info.concat("There are " + Integer.toString(numberOfImage) + " image(s) in " +
                filename + "\n");
    
    // for each image object, list image name & dimension

    hdf.DFR8restart();
    
    while (hdf.DFR8getdims(filename,argv,argB)) {
	
	int w = argv[0];
	int h = argv[1];
	boolean hasPalette = argB[0];
  
	int ref = hdf.DFR8lastref();
	
	info = info.concat("            Reference number: " + Integer.toString(ref) + "\n");
	info = info.concat("            Dimension    size:  " + Integer.toString(w) + "X" +
	                                             Integer.toString(h) + "\n");
	if (hasPalette)
	   info = info.concat("            Associated palette: TRUE \n");
	else
	   info = info.concat("            Associated palette: FALSE \n");

	info = info.concat(" -----------------------------------------------------------  \n ");
	
    }
    applet_.infoText.setText(info);
  }
  

  /** display the HDF file 8-raster image in the image canvas
   * @param filename the HDF file name
   * @param node the HDF Object node
   */
  public  void dispHdfRis8(String filename, HDFObjectNode node) throws HDFException {
	
	HDFLibrary hdf = new HDFLibrary(); 
	int argv[] = new int[2];
	boolean argB[] = new boolean[1];

    
    // set the reference number
    if (hdf.DFR8readref(filename, node.ref) == false)
      return;

    
    
    // get HDF image information
    if (hdf.DFR8getdims(filename,argv,argB) == false)
      return;
    
    int w = argv[0];
    int h = argv[1];
    boolean hasPalette = argB[0];
    
    
    byte imageData[]    = new byte[w*h];
    byte imagePalette[] = new byte[256*3];
    
    boolean readFlag;
    
    // read the image
    readFlag = hdf.DFR8getimage(filename, imageData,w,h,imagePalette);
    
    // set image palette
    if (hasPalette == false) {
      for (int i=0; i<256; i++)
	for (int j=0; j<3; j++)  
	  imagePalette[i*3+j] = (byte)i;
    }
    
    Image palImage = null;
    // create palette image
    palImage = createPaletteImage(imagePalette);
    
    Image img=null;
    if (readFlag) {
      img = createRasterImage(imageData,w,h,imagePalette);
    }
    
    // diaplay the raster image
    if (img != null) {
      
      applet_.hdfCanvas.setImageSize(w,h);
      
      applet_.hdfCanvas.setImage(img, palImage);
      
      applet_.hdfCanvas.repaint();
      
      // set the current HDF Object Node
      applet_.hdfCanvas.setObject(node);
    }
  }
  

  /** display the HDF file 24-raster image generic information
   * @param filename the HDF file name
   * @param node the HDF Object node
   */
  public  void dispHdfRis24Info(String filename, HDFObjectNode node) throws HDFException {
	
    HDFLibrary hdf = new HDFLibrary();
    int argv[] = new int[3];
    
    int numberOfImage = hdf.DF24nimages(filename);
    String   info = new String();
    
    // image number in HDF file
    info = info.concat("================== 24-bit raster image information ===============\n");
    info = info.concat("There are " + Integer.toString(numberOfImage) + " image(s) in " +
                filename + "\n");
    
    // for each image object, list image name & dimension

    hdf.DF24restart();
    
    while (hdf.DF24getdims(filename,argv)) {
	
	int w = argv[0];
	int h = argv[1];
	int interlace = argv[2];
  
	int ref = hdf.DF24lastref();
	
	info = info.concat("      Image reference number: " + Integer.toString(ref) + "\n");
	info = info.concat("            Dimension size: " + Integer.toString(w) + " by " +
	                                             Integer.toString(h) + "\n");
	info = info.concat("            Interlace       :" + Integer.toString(interlace)+ " \n");
	info = info.concat("  \n ");
	
    }
   applet_.infoText.setText(info);
  }
  

  /** display the HDF file 24-raster image in the image canvas
   * @param filename the HDF file name
   * @param node the HDF Object node
   */
  public void dispHdfRis24(String filename, HDFObjectNode node) throws HDFException {
	
    HDFLibrary hdf = new HDFLibrary(); 
    int argv[] = new int[3];
    
    // set the reference number
    if (!(hdf.DF24readref(filename, node.ref)))
      return;
    
    // get HDF image information
    if (hdf.DF24getdims(filename,argv) == false)
      return;
    
    int w = argv[0];
    int h = argv[1];
    int interlace = argv[2];
    
    byte imageData[]    = new byte[w*h*3];
    byte imagePalette[] = new byte[256*3];
    
    boolean readFlag;
    // set the interlace for reading by component interlacing scheme
    hdf.DF24reqil(HDFConstants.MFGR_INTERLACE_COMPONENT);
    
    readFlag = hdf.DF24getimage(filename, imageData,w,h);
    
    // set image palette
    boolean hasPalette = false;
    
    if (hasPalette == false) {
      for (int i=0; i<256; i++)
	for (int j=0; j<3; j++)  
	  imagePalette[i*3+j] = (byte)i;
    }
    
    Image palImage = null;
    // create palette image
    palImage = createPaletteImage(imagePalette);
    
    Image img=null;
    if (readFlag)
      img = createRasterImage(imageData,w,h,imagePalette);
    
    // diaplay the raster image
    if (img != null) {
      
      applet_.hdfCanvas.setImageSize(w,h);
      applet_.hdfCanvas.setImage(img);
      applet_.hdfCanvas.repaint();
      
      // set the current HDF Object Node
      applet_.hdfCanvas.setObject(node);
      
    }
  }

  /** display the HDF file GR generic information
   * @param fid HDF file id.
   * @param node the HDF Object node
   */
  public  void dispHdfGRInfo(int fid, HDFObjectNode node) throws HDFException {
	   
    // get ready to read the HDF GR interface
    int grid = -1;
    int riid = -1;
    int[] argv = new int[2];
    HDFLibrary hdf = (HDFLibrary)applet_.hdf;
   
    grid = hdf.GRstart(fid);
    if (grid == HDFConstants.FAIL) {
      hdf.GRend(grid);
      return;
    }

    // get HDF GR information
    
    if (hdf.GRfileinfo(grid, argv) == false) {
      // terminate the GR access
      hdf.GRend(grid);
      return;
    }
    
    String attrBuf = new String();	

    String   info = new String();
    
    // GR number in HDF file
    info = info.concat("================== GR information ===============\n");
  
    if (node.type == node.GR) { 

      info = info.concat("Number of Dataset: " + argv[0] + "\n");
      info = info.concat("Number of Global Attributes: " + argv[1] + "\n");
    }
    else {   
 
    	int ref = node.ref;
     
    	int index = hdf.GRreftoindex(grid, (short)ref);
 
    	if (index == HDFConstants.FAIL) {
      	   info = info.concat("GRreftoindex: FAIL\n"); 
    	} else {
		if ((riid = hdf.GRselect(grid,index)) == HDFConstants.FAIL) {
		   hdf.GRendaccess(riid);
		   info = info.concat("GRselect: FAIL\n"); 
      		}
     		else {		
			String gr_name = new String(" "); 
			String[] n = new String[1];
                        n[0] = gr_name;

			int[] imInfo = new int[4];
			int[]dim_sizes = new int[2];  /* ??? */

			if (hdf.GRgetiminfo( riid, n, imInfo, dim_sizes) ) {
				gr_name = n[0];
	
				// GR dataset name
				info = info.concat("Image Name : " + gr_name + "\n");
				info = info.concat("Image Index: " + Integer.toString(index) + "\n");

				// rank
				String rankInfo = Integer.toString(dim_sizes[0]);
				rankInfo += " by " ;
				rankInfo += Integer.toString(dim_sizes[1]);
				rankInfo += "\n";

				info = info.concat("Image Size : " + rankInfo);
				
				// nt
				info = info.concat("Data Number Type: " + hdf.HDgetNTdesc(imInfo[1]) + "\n" );
				info = info.concat("Num of Components: " + imInfo[0] + "\n" );
				info = info.concat("Interlace  : " + imInfo[2] + "\n" );

				// attributes
				info = info.concat("Num of Attributes: " + imInfo[3] + "\n" );

	  		} 
			else {
			  info = info.concat("GRgetinfo: FAIL\n"); 	
			}
		} 
    		hdf.GRendaccess(riid);
	} 
    }   
 
    info = info.concat("===============================================\n");
   
    hdf.GRend(grid);

    applet_.infoText.setText(info);
  }
  

  /** display the HDF file generic raster image in the image canvas
   * @param fid the file identifier
   * @param node the HDF Object node
   */
  public void dispHdfGR(int fid, HDFObjectNode node) throws HDFException {

    // get ready to read the HDF GR interface
    int grid = -1;
    int riid = -1;
    HDFLibrary hdf = (HDFLibrary)applet_.hdf;
    
    grid = hdf.GRstart(fid);
    if (grid == HDFConstants.FAIL) {
      hdf.GRend(grid);
      return;
    }
    
    // get HDF GR information
    int[] fileInfo = new int[2];
    
    if (hdf.GRfileinfo(grid, fileInfo) == false) {
      // terminate the GR access
      hdf.GRend(grid);
      return;
    }
    
    String attrBuf = new String();	
    
    if (node.type == node.GRGLOBALATTR) {
      // GR Global Attributes
      attrBuf = attrBuf.concat("\n======================== GR Global Attribute =================\n");
      // the whole Global Attribute
      attrBuf = attrBuf.concat(getHdfGRgattr(grid, node));
      attrBuf = attrBuf.concat("\n===================== End of GR Global Attribute ==============\n");
      
      // display the info.
      dispMessage(attrBuf); 
      
    }
    else {
      if (node.type == node.GRDATASETATTR) {
	// GR dataset attribute
	attrBuf = attrBuf.concat("\n=====================  GR Image Attribute  ================\n");
	// the whole Global Attribute 
	attrBuf = attrBuf.concat(getHdfGRattr(grid, node));
	attrBuf = attrBuf.concat("\n===================== End of GR Image Attribute ============\n");
	
	// display the info.
	dispMessage(attrBuf); 
      }
      else { 
	dispHdfGRimage(grid, node);
      }
    } 
    hdf.GRend(grid);
   }
  

  /** display the HDF file generic raster image in the image canvas
   * @param grid the GR identifier
   * @param node the HDF Object node
   */
  public void dispHdfGRimage(int grid, HDFObjectNode node) throws HDFException {
  
    int riid      = -1;
    String retStr = null;
    int w	      = 256;
    int h	      = 256;
    HDFLibrary hdf = (HDFLibrary)applet_.hdf;
    
    int ref = node.ref;
    
    int index = hdf.GRreftoindex(grid, (short)ref);
    
    if (index == HDFConstants.FAIL)  {
      retStr = new String ("GRreftoindex: FAIL");
    }
    else {
      if ((riid = hdf.GRselect(grid,index)) == HDFConstants.FAIL) {
	hdf.GRendaccess(riid);
	retStr = new String("GRselect: FAIL");
      }
      else { 
	String gr_name = new String(" "); 
	String[] n = new String[1];
	n[0] = gr_name;

	int[]dim_sizes = new int[2];  /* ??? */
	int[] argv = new int[4];

	if (hdf.GRgetiminfo(riid, n, argv, dim_sizes) ) {
		gr_name = n[0];
		w = dim_sizes[0];
		h = dim_sizes[1];
		int plane = 1;
		image = getHdfGRimage(riid, argv[0], argv[1], argv[2], w, h, plane);
	}
      } 
    } 
    
    // display the raster image
    if (image != null) {
      
      applet_.hdfCanvas.setImageSize(w,h);
      applet_.hdfCanvas.setImage(image,paletteImage);
      applet_.hdfCanvas.repaint();
      
      // set the current HDF Object Node
      applet_.hdfCanvas.setObject(node);
      
    }
    else {
      // display error
      dispMessage(retStr);
    }
    hdf.GRendaccess(riid);
  }
  

  /** Get the HDF  generic raster image by specified plane number
   * @param riid the raster image identifier 
   * @param imgInfo the image info. array
   * @param plane   the plane number to be retrived.
   * @return the image, otherwise null
   */
  public Image getHdfGRimage(int riid, int ncomp, int nt, int interlace, int w, int h, int plane) 
throws HDFException {

    HDFLibrary hdf = (HDFLibrary)applet_.hdf;
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
    
    // create palette image
    paletteImage = createPaletteImage(imagePalette);
    
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
	/* convert the data into a standard image */
      if (applet_.makeImageData(imageData,nt,w,h,(w*h*(plane-1)),output)) {
	image = createRasterImage(output,w,h,imagePalette);
	}
    }
    return image;
  }
  

  /** Process numeric attributes or strings, converting from bytes to Java
    *  types.
    *  This calls native routines to do the numeric conversions.
    */
String doattrs( byte[] buf, int buffsize, int NT, int count ) throws HDFException {

	HDFNativeData convert = new HDFNativeData();
	String lr         = new String("\t");
	String semicolon  = new String("; ");
	String str = new String(" ");

	int incr = hdf.DFKNTsize(NT);

	switch(NT) {
	case HDFConstants.DFNT_CHAR:
		// take it as char
		str = str.concat(lr);
		for (int jj=0; jj<count; jj+=incr) {
			String sval = new String(buf,jj,1);
			str = str.concat(sval);
		}
		break;
	case HDFConstants.DFNT_UCHAR8:
	//case HDFConstants.DFNT_UCHAR:
	case HDFConstants.DFNT_UINT8:
		str = str.concat(lr);
		for (int jj=0; jj<count; jj+=incr) {
		Byte bval = new Byte(buf[jj]);
		Short shval;
		if (bval.shortValue() < 0) {
			shval = new Short((short)(bval.intValue() + 256));
		} else {
			shval = new Short(bval.shortValue());
		}
		str = str.concat(shval.toString());
		str = str.concat(semicolon);
		}
		break;
	case HDFConstants.DFNT_INT8:
		str = str.concat(lr);
		for (int jj=0; jj<count; jj+=incr) {
			// convert to integer
			Byte bval = new Byte(buf[jj]);
			str = str.concat(bval.toString());
			str = str.concat(semicolon);
		}
		break;
	case HDFConstants.DFNT_INT16:
		str.concat(lr);
		for (int jj=0; jj<count; jj+=incr) {
			// convert to integer
			Short shval = new Short(convert.byteToShort(buf,jj));
			str = str.concat(shval.toString());
			str = str.concat(semicolon);
		}
		break;
	case HDFConstants.DFNT_UINT16:
		str = str.concat(lr);
		for (int jj=0; jj<count; jj+=incr) {
		Short shval = new Short(convert.byteToShort(buf,jj));
		Integer ival;
		if (shval.shortValue() < 0) {
			ival = new Integer((shval.intValue() + 65536));
		} else {
			ival = new Integer(shval.intValue());
		}
		str = str.concat(ival.toString());
		str = str.concat(semicolon);
		}
		break;
	case HDFConstants.DFNT_INT32:
		str = str.concat(lr);
		for (int jj=0; jj<count; jj+=incr) {
			// convert to integer
			Integer ival = new Integer(convert.byteToInt(buf,jj));
			str = str.concat(ival.toString());
			str = str.concat(semicolon);
		}
		break;

	case HDFConstants.DFNT_UINT32:
		str = str.concat(lr);
		for (int jj=0; jj<count; jj+=incr) {
		Integer ival = new Integer(convert.byteToInt(buf,jj));
		Float fval;
		if (ival.intValue() < 0) {
			fval = new Float((ival.floatValue() + (float)4294967295.0));
		} else {
			fval = new Float(ival.intValue());
		}
		str = str.concat(ival.toString());
		str = str.concat(semicolon);
		}
		break;

	//case HDFConstants.DFNT_FLOAT:
	case HDFConstants.DFNT_FLOAT32:
		str = str.concat(lr);
		for (int jj=0; jj<count; jj+=incr) {
			// convert to float
			Float fval = new Float(convert.byteToFloat(buf,jj));
			str = str.concat(fval.toString());
			str = str.concat(semicolon);
		}
		break;

	//case HDFConstants.DFNT_FLOAT64:
	case HDFConstants.DFNT_DOUBLE:
		str = str.concat(lr);
		for (int jj=0; jj<count; jj+=incr) {
			// convert to integer
			Double dval = new Double(convert.byteToDouble(buf,jj));
			str = str.concat(dval.toString());
			str = str.concat(semicolon);
		}
		break;
	} 
	return(str);
}

  /** display the HDF file generic raster image in the image canvas
   * @param grid  the GR identifier
   * @param nattr the number of the Global attribute.
   * @return the string of the global attribute.
   */
  public String getHdfGRgattr(int grid, HDFObjectNode node ) throws HDFException {
  
  String attr = new String();	
  String lr         = new String("\t");
  String semicolon  = new String("; ");
  
  int kk = node.index;
  
  // for each of the global attribute 
  String name = new String(" ");
  String[] n = new String[1];
  n[0] = name;
  int[] attrInfo = new int[2];
  
  if (hdf.GRattrinfo(grid, kk, n, attrInfo)) {
    name = n[0];
    attr = attr.concat("\nName:  ");
    attr = attr.concat(name);
    
    // read the attribute as bytes, call doattrs to convet to Java types.
    int attrSize = hdf.DFKNTsize(attrInfo[0])*attrInfo[1] + 1;
    byte[] buf = new byte[attrSize];
    if (hdf.GRgetattr(grid, kk, buf)) {
      return (attr);
    }
    buf[attrSize-1] = '\0';
     
    attr = attr.concat("\nType : " + hdf.HDgetNTdesc(attrInfo[0]));
    attr = attr.concat("\nCount: " + Integer.toString(attrInfo[1]));
    
    attr = attr.concat("\nValue: " );
    attr = attr.concat(doattrs( buf, attrSize, attrInfo[0], attrInfo[1] ));
  } 
  return attr;
}
  

  /** display the HDF file generic raster image in the image canvas
   * @param grid  the GR identifier
   * @param node  the HDF Object node
   * @return the string of the image attribute.
   */
public String getHdfGRattr(int grid, HDFObjectNode node) throws HDFException {
    
    int riid	  = -1;
    
    String attr = new String();	
    String lr         = new String("\n\t");
    String semicolon  = new String("; ");
    
    int ref = node.ref;
    
    int index = hdf.GRreftoindex(grid, (short)ref);
    if (index == HDFConstants.FAIL) 
      return ("GRreftoindex: FAIL");
    
    if ((riid = hdf.GRselect(grid,index)) == HDFConstants.FAIL) {
      hdf.GRendaccess(riid);
      return("GRselect: FAIL");
    }
    
    int kk 		= node.index; // the index of the dataset attr.
    String name = new String(" ");
    String[] n = new String[1];
    n[0] = name;
    int [] attrInfo = new int[2];

    if (hdf.GRattrinfo(riid, kk, n, attrInfo)) {
      name = n[0];
      attr = attr.concat("\nName:  ");
      attr = attr.concat(name);
      
      int attrSize = hdf.DFKNTsize(attrInfo[0])*attrInfo[1] + 1;
      byte[] buf = new byte[attrSize];
      if (!hdf.GRgetattr(riid, kk, buf)) {
	hdf.GRendaccess(riid);
	return(attr);
      }
         
      attr = attr.concat("\nType : " + hdf.HDgetNTdesc(attrInfo[0]));
      attr = attr.concat("\nCount: " + attrInfo[1]);

      attr = attr.concat("\nValue:");
      
      attr = attr.concat(doattrs( buf, attrSize, attrInfo[0], attrInfo[1] ));
    } 
    hdf.GRendaccess(riid);
    return attr;
  }
  

  /** display the HDF file SDS generic information
   * @param filename the HDF file name
   * @param node the HDF Object node
   */
public  void dispHdfSDSInfo(String filename, HDFObjectNode node) throws HDFException {
    int sdid  = -1 ;
    int sdsid = -1;
     
    sdid = hdf.SDstart(filename, HDFConstants.DFACC_RDONLY);
    
    if (sdid == HDFConstants.FAIL) {
      hdf.SDend(sdid);
      return;
    }

    int[] fileInfo = new int[2];
    
    if (hdf.SDfileinfo(sdid, fileInfo) == false) {
      hdf.SDend(sdid);
      return;
    }
 
    String   info = new String();
    
    // sds number in HDF file
    info = info.concat("================== SDS information ===============\n");
  
    if (node.type == node.SDS) { // not the SDS root

      info = info.concat("Number of Dataset: " + fileInfo[0] + "\n");
      info = info.concat("Number of Global Attributes: " + fileInfo[1] + "\n");
    }
    else {
 
    	int ref = node.ref;
    
    	int index = hdf.SDreftoindex(sdid, ref);
   
    	if (index == HDFConstants.FAIL)   
      	   info = info.concat("SDreftoindex: FAIL\n"); 
    	else {
      		if ((sdsid = hdf.SDselect(sdid,index)) == HDFConstants.FAIL) {
			hdf.SDendaccess(sdsid);
			info = info.concat("SDselect: FAIL\n"); 
      		}
      		else { 
			int[] SDInfo = new int[3];
                        String datasetname = new String(" ");
			String[] ss = new String[1];
			ss[0] = datasetname;
                        int  dimsize[]     = new int[16];
                        if (hdf.SDgetinfo(sdsid, ss, dimsize, SDInfo)) {
	
				datasetname = ss[0];
				info = info.concat("Dataset name: " + datasetname + "\n");
				String rankInfo = Integer.toString(SDInfo[0]);
				rankInfo += ", " ;

				for (int i=(SDInfo[0] - 1); i>0; i--) {
					rankInfo += Integer.toString(dimsize[i]);
					rankInfo += " by ";
				}
				rankInfo += Integer.toString(dimsize[0]);
				rankInfo += "\n";

				info = info.concat("Rank: " + rankInfo);
				
				info = info.concat("Data number type: " + hdf.HDgetNTdesc(SDInfo[1]) + "\n" );

				info = info.concat("Number of Attributes: " + SDInfo[2] + "\n" );
	  		} 
			else {
			  info = info.concat("SDgetinfo: FAIL\n");
			}
		} 
    		hdf.SDendaccess(sdsid);
	} 
    }   
 
    info = info.concat("===============================================\n");
 
    hdf.SDend(sdid);

    applet_.infoText.setText(info.toString());
  }
  

  /** display the HDF file sds information and converted image
   * @param filename the file name
   * @param node the HDF Object node
   */
public void dispHdfSDS(String filename, HDFObjectNode node) throws HDFException {
  
    int sdid  = -1 ;
    int sdsid = -1;
    
    sdid = hdf.SDstart(filename, HDFConstants.DFACC_RDONLY);
    
    if (sdid == HDFConstants.FAIL) {
      hdf.SDend(sdid);
      return;
    }
    
    // get HDF SDS information
    int[] fileInfo = new int[2];
    
    if (hdf.SDfileinfo(sdid, fileInfo ) == false) {
      hdf.SDend(sdid);
      return;
    }
    
    String attrBuf = new String();	
    
    if (node.type == node.SDSGLOBALATTR) {
      // SDS Global Attributes
      attrBuf = attrBuf.concat("\n======================== SDS Global Attribute =================\n");
      // the whole Global Attribute
      attrBuf = attrBuf.concat(getHdfSDSgattr(sdid, node));
      attrBuf = attrBuf.concat("\n===================== End of SDS Global Attribute ==============\n");
      
      // display the info.
      dispMessage(attrBuf); 
 
    }
    else {
      if (node.type == node.SDSDATASETATTR) {
	// SDS dataset attribute
	attrBuf = attrBuf.concat("\n=====================  SDS Dataset Attribute  ================\n");
	// the whole Global Attribute 
	attrBuf = attrBuf.concat(getHdfSDSattr(sdid, node));
	attrBuf = attrBuf.concat("\n===================== End of SDS Dataset Attribute ============\n");
	
	// display the info.
	dispMessage(attrBuf); 
	
      }
      else { 
	
	dispHdfSDSimage(sdid, node);
      }
    } 
    hdf.SDend(sdid);
  }
  

  /** display the HDF file sds information and converted image
   * @param sdid the SDS identifier
   * @param node the HDF Object node
   */
public void dispHDfSDS(int sdid, HDFObjectNode node) throws HDFException {
  
    int sdsid = -1;
    
    int[]fileInfo = new int[2];
    
    if (hdf.SDfileinfo(sdid, fileInfo) == false) {
      hdf.SDend(sdid);
      return;
    }
    
    String attrBuf = new String();	
    
    if (node.type == node.SDSGLOBALATTR) {
	attrBuf = attrBuf.concat("\n======================== SDS Global Attribute =================\n");
	attrBuf = attrBuf.concat(getHdfSDSgattr(sdid, node));
	attrBuf = attrBuf.concat("\n===================== End of SDS Global Attribute ==============\n");

	dispMessage(attrBuf);
    }
    else {
	if (node.type == node.SDSDATASETATTR) {
		attrBuf = attrBuf.concat("\n=====================  SDS Dataset Attribute  ================\n");
		attrBuf = attrBuf.concat(getHdfSDSattr(sdid, node));
		attrBuf = attrBuf.concat("\n===================== End of SDS Dataset Attribute ============\n");
		dispMessage(attrBuf);
	} else { 
		dispHdfSDSimage(sdid, node);
	}
    } 
    hdf.SDend(sdid);
  }
  

  /** display the HDF file SDS converted image
   * @param sdid the SD identifier
   * @param node the HDF Object node
   */
public void dispHdfSDSimage(int sdid, HDFObjectNode node) throws HDFException {
	
    int sdsid     = -1;
    String retStr = null;
    int w	      = 256;
    int h	      = 256;
    int ref = node.ref;
    int index = hdf.SDreftoindex(sdid, ref);
    
    if (index == HDFConstants.FAIL)  {
	retStr = new String ("SDreftoindex: FAIL");
    }
    else {
	if ((sdsid = hdf.SDselect(sdid,index)) == HDFConstants.FAIL) {
		hdf.SDendaccess(sdsid);
		retStr = new String("SDselect: FAIL");
	} else {
		// get sds info.
		int [] SDInfo = new int[3];
		String datasetname = new String(" ");
		String[] ss = new String[1];
		ss[0] = datasetname;
		int  dimsize[]     = new int[16];

		if (hdf.SDgetinfo(sdsid, ss, dimsize, SDInfo)) {
			datasetname = ss[0];
			int rank = SDInfo[0];
			if ((rank ==2) || (rank ==3)) {

				w = dimsize[rank-1];
				h = dimsize[rank-2];
				int plane = 1;
				image = getHdfSDSimage(sdsid, rank, SDInfo[1], dimsize, plane);
			} else {
				image = null;
			}

			if (image == null) {
				retStr = "Can't convert image from SDS";
			} else {
				retStr = "SDS Dimension Size > 3";
			}
		}
      }
    } 
    
    // display the raster image
    if (image != null) {
      
      applet_.hdfCanvas.setImageSize(w,h);
      applet_.hdfCanvas.setImage(image, paletteImage);
      applet_.hdfCanvas.repaint();
      
      // set the current HDF Object Node
      applet_.hdfCanvas.setObject(node);
      
    }
    else {
      dispMessage(retStr);
    }
    hdf.SDendaccess(sdsid);
  }
  

  /** Get the HDF  converted image by specified plane number
   * @param sdsid the SDS dataset identifier 
   * @param sdsInfo the sds info. array
   * @param dims    the sds dimension size
   * @param plane   the plane number to be retrived.
   * @return the image, otherwise null
   */

public Image getHdfSDSimage(int sdsid, int rank, int nt, int[] dims,int plane)
	throws HDFException {

    /****************************************************************
     Date: 3-12-97
      Method , getHdfSDSimage(), was modified so that we can created SDS 
      converted image by specified dataset range if sds has an dataset range.
      The dataset range may be smaller than the actual dataset range, fill the 
      value if bigger or smaller than specified dataset range.  Native method
      makeImageDataByRange() take charge of that. 
    **************************************************************/

    Image image_ = null;
    
    boolean hasPalette = false;
    
    
    // Is the rank valid?
    if ((rank<=1) || (rank>=4))
      return null;
    
    int w = dims[rank-1];
    
    int h = dims[rank-2];
    
    if ((w*h) < 0)
      return null;
    
    int dataSize = w*h*hdf.DFKNTsize(nt);
    
    byte sdsData[]    = new byte[dataSize];
    
    byte imagePalette[] = new byte[3*256];
    
    
    if (hasPalette == false)  { 
      // try rainbow
      imagePalette = null;
      imagePalette = getPaletteOfRainbow();
    }
    
    // create palette image
    paletteImage = null;
    paletteImage = createPaletteImage(imagePalette);
    
    // read sds data	
    int start[] = new int[3];
    int stride[]= new int[3];
    int count[] = new int[3];

    start[0] = 0;
    start[1] = 0;
    start[2] = 0;
    
    stride[0] = 1;
    stride[1] = 1;
    stride[2] = 1;

	//  ???? only two cases covered here!!!!  FIX!!  REMcG  6.18.97
    if (rank == 3) {
      count[0] = 1   ;
      count[1] = h   ;	
      count[2] = w   ;
    }
    else {
      count[0] = h   ;	
      count[1] = w   ;
    }
    
    // read flag
    boolean readFlag = hdf.SDreaddata(sdsid,start,stride,count,sdsData);
    
    // iamge data
    byte[] output = new byte[w*h];
    
    if (readFlag) {

      boolean cvFlag = true;
      double maxmin[] = new double[2];
      if (hdf.SDgetrange(sdsid, maxmin)) {
      	 cvFlag = applet_.makeImageDataByRange(sdsData,maxmin[1],
		maxmin[0],nt,w,h,(w*h*(plane-1)),output);
      } else {
	 cvFlag = applet_.makeImageData(sdsData,nt,w,h,(w*h*(plane-1)),output);
      }

      if (cvFlag) {

         // the following code make me confused, I don't know  why I can't get image 
         // painted under Win95. However if I changed pixel value for "pixel[0]" 
         // to zero(0), then it works.    
         // try to get OS name
         String osName = System.getProperties().getProperty("os.name");
         if (osName.toUpperCase().indexOf("WINDOW") != -1) // window 95 or nt
	   output[0] = 0;

	image_ = createRasterImage(output,w,h,imagePalette);
      }
    } 
    return image_;
  }
  

  /** display the HDF file SDS global attributes
   * @param sdid  the SDS identifier
   * @param nattr the number of the Global attribute.
   * @return the string of the global attribute.
   */  
public String getHdfSDSgattr(int sdid, HDFObjectNode node ) throws HDFException {
  
    String attr = new String();	
    String lr         = new String("\t");
    String semicolon  = new String("; ");
    
    int kk = node.index;
    
    // for each of the global attribute 
    String name = new String(" ");
    String[] ss = new String[1];
    ss[0] = name;
    int[] attrInfo = new int[2];

    if (hdf.SDattrinfo(sdid, kk, /*name*/ss, attrInfo/*NT, count*/)) {
      name = ss[0];
      attr = attr.concat("\nName:  ");
      attr = attr.concat(name);

      // read the attributes as bytes, use doattr() to convert to
	// java types
      
      int attrSize = hdf.DFKNTsize(attrInfo[0])*attrInfo[1] + 1;
      byte[] buf = new byte[attrSize];
      if (hdf.SDreadattr(sdid, kk, buf) == false) {
	return (attr);
      }
      buf[attrSize-1] = '\0';
      
      attr = attr.concat("\nType : " + hdf.HDgetNTdesc(attrInfo[0]));
      attr = attr.concat("\nCount: " + attrInfo[1]);

      attr = attr.concat("\nValue: " );
      
      
      attr = attr.concat(doattrs( buf, attrSize, attrInfo[0], attrInfo[1]));
      
    }
    return attr;
  }
  

  /** display the HDF file SDS dataset attributes
   * @param sdid  the SDS identifier
   * @param node  the HDF Object node
   * @return the string of the image attribute.
   */
public String getHdfSDSattr(int sdid, HDFObjectNode node) throws HDFException {
  
    int riid	  = -1;
    
    String attr = new String();	
    String lr         = new String("\t");
    String semicolon  = new String("; ");
    
    int ref = node.ref;
    
    int index = hdf.SDreftoindex(sdid, ref);
    if (index == HDFConstants.FAIL) 
      return ("SDreftoindex: FAIL");
    
    if ((riid = hdf.SDselect(sdid,index)) == HDFConstants.FAIL) {
      hdf.SDendaccess(riid);
      return("SDselect: FAIL");
    }
    
    // for each of the global attribute 
    int kk 		= node.index; 
    String name = new String(" ");
    String[] ss = new String[1];
    ss[0] = name;
    int attrInfo[] = new int[2];

    if (hdf.SDattrinfo(riid, kk, ss, attrInfo)) {
      name = ss[0];
    
      attr = attr.concat("\nName:  ");
      attr = attr.concat(name);
	
      attr = attr.concat("\nType : " + hdf.HDgetNTdesc( /*NT.intValue()*/attrInfo[0]));
      attr = attr.concat("\nCount: " + /*count.toString()*/attrInfo[1]);

     
      int attrSize = hdf.DFKNTsize(attrInfo[0])*attrInfo[1] + 1;
      byte[] buf = new byte[attrSize];
      if (hdf.SDreadattr(riid, kk, buf) == false) {
	hdf.SDendaccess(riid);
	return(attr);
      }
      
      attr = attr.concat("\nValue:");
      
	attr = attr.concat(doattrs( buf, attrSize, attrInfo[0], attrInfo[1] ));
    } 
    hdf.SDendaccess(riid);
    return attr;
  }
  

  /** display the HDF file vdata generic information
   * @param fid the HDF file id.
   * @param node the HDF Object node
   */
public  void dispHdfVdataInfo(int fid, HDFObjectNode node) throws HDFException {
 
       String   info = new String();

       hdf.Vstart(fid);

       int vdata_ref = -1;	 
  
       int [] n = new int[1];
       int vdNumber = hdf.VSlone(fid,n,0);
  
       int refArray[] = new int[vdNumber];
       int refNum;
   
       if  ((refNum = hdf.VSlone(fid, refArray, vdNumber)) > 0){
	
       info = info.concat("================== Vdata information ===============\n");
  
       // each of vdata (lonely)
       for (int i=0; i<refNum; i++) {
	      
	vdata_ref = refArray[i];

       	int vstag = HDFConstants.DFTAG_VS;
   
       	int vdata_id = hdf.VSattach(fid, vdata_ref, "r"); 
       	String className = new String(" ");
	String s[] = new String[1];
	s[0] = className;
	hdf.VSgetclass(vdata_id,s);
	className = s[0];
       	String name      = new String(" ");
	s[0] = name;
	hdf.VSgetname (vdata_id,s);
	name = s[0];
    
	if ( (className.indexOf("_HDF_CHK_TBL_0") == -1) ||  
	     (applet_.dispLevel == applet_.DEVELOPER) ) { // non-chunk node

		info = info.concat("VStag: "+vstag + ", ref: " + vdata_ref + "\n");
   
         	info = info.concat("Vdata Class: "+className + "\n");
        	info = info.concat("Vdata Name:  "+name + "\n");
      
		info = info.concat("Vdata records: "+hdf.VSelts(vdata_id) + "\n");

		info = info.concat("Interlace: "+hdf.VSgetinterlace(vdata_id)+"\n");

		// test VSgetfields
		String flist = new String(" ");
		s[0] = flist;
		int fieldNumber = hdf.VSgetfields(vdata_id, s);
		flist = s[0];

		info = info.concat("field number: "+ fieldNumber + "\n");
		info = info.concat("fields list: "+  flist + "\n");       
		info = info.concat("Sizes of a record: "+hdf.VSsizeof(vdata_id,flist) + " bytes\n");

		// Check to see if this vdata is alone? ....

		int  nFields = hdf.VFnfields(vdata_id);
		for (int kk=0; kk<nFields; kk++) {
  
			String fName = hdf.VFfieldname(vdata_id, kk);
			int	   fType = hdf.VFfieldtype(vdata_id, kk);
			String fTypeName = hdf.HDgetNTdesc(fType);
			int    fOrder= hdf.VFfieldorder(vdata_id, kk);

			info = info.concat("   " +  fName  + ",  " + fTypeName + 
				",  " + fOrder + "\n" );
		} 
  	     	 
		 info = info.concat("---------------------------------------------------\n");	
		hdf.VSdetach(vdata_id);
	}
      }
      }
    
      info = info.concat("=====================================================\n");
      hdf.Vend(fid);
      applet_.infoText.setText(info);
  }
  

 /** display the HDF Vdata information
   * @param fid the file identifier
   * @param node the HDF Object node
   */
  public void dispHdfVdata(int fid, HDFObjectNode node) throws HDFException {

    hdf.Vstart(fid);
      
    // Attaches to an existing Vdata
    int vdata_id = hdf.VSattach(fid, node.ref, "r");
    if (vdata_id != HDFConstants.FAIL) {
	
	// set selected vdata field & record number
	int[] selectedRecords = new int[2];
	selectedRecords[0] = 1;
	selectedRecords[1] = hdf.VSelts(vdata_id) ;
	    
	// field
	int nfields = hdf.VFnfields(vdata_id);
	    
	int[] selectedFields = new int[nfields];
	for (int i=0; i<nfields; i++) 
	    selectedFields[i] = i;
	
	if (applet_.vdataFrameDisplayed) {
	   // change to another vdata_id
	   applet_.vdataFrame.setup(vdata_id,selectedRecords, selectedFields); 
	}
	else {
	   // display vdata
	   applet_.vdataFrameDisplayed = true;    
	   applet_.vdataFrame = new JHVVdataFrame(applet_,hdf, vdata_id, 
	                                  selectedRecords, selectedFields);
	}
    }
    
    hdf.VSdetach(vdata_id);
    hdf.Vend(fid);
    
    return;
  }
  

 /** Return the RAINBOW palette 
   * rgb rgb rgb rgb rgb ....
   */
 public  ColorModel getColorModelOfRainbow()  {


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
	
	// set the RGB
    	byte[] red   = new byte[256];
    	byte[] green = new byte[256];
    	byte[] blue  = new byte[256];

	for (int i=0; i<256; i += 1) {
	
	    red[i]  = values[0][i];
	    green[i]= values[1][i];
	    blue[i] = values[2][i];
	}

	return new IndexColorModel(8, 256, red, green, blue);
	
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

	int  ncolors = 256;
	
	byte retPal[] = new byte[768];

	for (int i=0; i<256; i += 1) {
	
	    retPal[3*i]   = (byte)values[0][i];
	    retPal[3*i+1] = (byte)values[1][i];
	    retPal[3*i+2] = (byte)values[2][i];
	}

	return retPal;
	
 }
 

  /** display the HDF file vgroup generic information
   * @param fid the HDF file id.
   * @param node the HDF Object node
   */
  public  void dispHdfVgroupInfo(int fid, HDFObjectNode node) throws HDFException {
 
       String   info = new String();

       hdf.Vstart(fid);

       // start search 	 
       int vgroup_ref = -1;	  
    
       // for Vgroup
       int [] n = new int[1];
       int vgNumber = hdf.Vlone(fid,n,0);

       if (node.ref == -1) {   // Vgroup root ...

       	  if (vgNumber > 0)  {

       		int refArray[] = new int[vgNumber];
       		int refNum;
   
   		// for child of the Vgroup root
       		if  ((refNum = hdf.Vlone(fid, refArray, vgNumber)) > 0){
	
       		    info = info.concat("================== Vgroup information ===============\n");
  
       		    // each of vgroup (lonely)
       		    for (int i=0; i<refNum; i++) {
	      
			// reference .
			vgroup_ref = refArray[i];

       			int vgtag = HDFConstants.DFTAG_VG;
	 
       			int vg_id = hdf.Vattach(fid, vgroup_ref, "r"); 
       			String className = new String(" ");
			String s[] = new String[1];
			s[0] = className;
			hdf.Vgetclass(vg_id,s);	   
			className = s[0];
       			String name      = new String(" ");
			s[0] = name;
			hdf.Vgetname (vg_id,s);
			name = s[0];
           
			if ( (className.indexOf("CDF0.0") == -1) ||
			     (applet_.dispLevel == applet_.DEVELOPER) ) { // non-internel node

				// get total numbers of tag/ref pairs id 
				int npairs = hdf.Vntagrefs(vg_id);

				info = info.concat("     Tag/Ref: "+ vgtag +"/" + vgroup_ref + "\n" );
				info = info.concat("     Name: "+name + "\n"  );
				info = info.concat("     Class Name: "+className + "\n" );
				info = info.concat("     Entries: " + npairs + "\n" );

				// what about attributes? add as needed in future....
			
				// separator   
				info = info.concat("---------------------------------------------------\n");
	  
				// terminate this vgroup
				 hdf.Vdetach(vg_id);
       			} 
      		    } 
      		} 
      	 }
      }  
      else {

 	info = info.concat("================== Vgroup information ===============\n");

      	// reference .
	vgroup_ref = node.getObjRef();

	int vgtag = HDFConstants.DFTAG_VG;
	 
       	int vg_id = hdf.Vattach(fid, vgroup_ref, "r"); 
       	String className = new String(" ");
	String s[] = new String[1];
	s[0] = className;
	hdf.Vgetclass(vg_id,s);	   
	className = s[0];
       	String name      = new String(" ");
	s[0] = name;
	hdf.Vgetname (vg_id,s);
	name = s[0];
           
	// get total numbers of tag/ref pairs id 
  	int npairs = hdf.Vntagrefs(vg_id);

	info = info.concat("     Tag/Ref: "+ vgtag +"/" + vgroup_ref + "\n" );
   	info = info.concat("     Name: "+name + "\n"  );
   	info = info.concat("     Class Name: "+className + "\n" );
 	info = info.concat("     Entries: " + npairs + "\n" );
     	 
        // Add attributes here ? .........
  	     		
       	// terminate this vgroup
      	hdf.Vdetach(vg_id);

       	// separator   
       	info = info.concat("---------------------------------------------------\n");
  
      }
      info = info.concat("=====================================================\n");
  
      hdf.Vend(fid);

      applet_.infoText.setText(info);
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
  

  /** create the raster image br specified image data
   * @param imgData the image data(pixel value)
   * @param w the width of the image
   * @param h the height of the image
   * @param imgPalette the palette of the image
   * @param index      the plane number of the image
   * @return the image , otherwise null
   */
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
    return(applet_.createImage(new MemoryImageSource(w,h,imagePalette.getColorModel(),data,0,w)));
   
   
  }

  /** create the palette image
   * @param pal the palette value
   * @return the image , otherwise null
   */
  public Image createPaletteImage(byte[] pal) {
 
    // palette image
    byte[] palImageData = new byte[256*18];
    
    for (int i=0; i<256; i++) {
      for (int j=0; j<6; j++ ) {
	palImageData[i*12+j]    = (byte)i;
	palImageData[i*12+j+6] = (byte)i;
	palImageData[i*12+j+12] = (byte)i;
      }
    } 
    
    return(  createRasterImage(palImageData,12,256,pal));
    
  }

  /** select one HDF Tree node, this node is expandable(has one child at least)
   * @param tree the HDF Tree
   */
  public void	expandCollapse(HDFTree tree, int modifier) {
 
	int fid = applet_.fid;
  // set cursor type to "WAIT_CURSOR"
   ((Component)applet_.jhvFrame).setCursor(new Cursor(Cursor.WAIT_CURSOR));

    super.expandCollapse(tree,modifier);
    
    // clean up the image panel
    eraseImage();
    
    // do something for HDF
    HDFObjectNode  node = (HDFObjectNode)(tree.selectedNode.hdfObject);

    // detect the type of the node
    int nodeType = node.getObjType();

    hdf = applet_.hdf;

    try {
	    switch (nodeType) {
	    case node.RIS8: // HDF 8-raster image   
		dispHdfRis8Info(applet_.hdfFile, node);
		break;

	    case node.RIS24: // HDF 24-raster image
		dispHdfRis24Info(applet_.hdfFile, node);
		break;

	    case node.SDS:     // HDF SDS
	    case node.SDSDATASET: {	 
		//hdf   = new HDFSDS();  
		dispHdfSDSInfo(applet_.hdfFile,node);
		break;
	    }
	    case node.GR:         // HDF GR
	    case node.GRDATASET: {
		// create HDFgeneric raster image object
		//hdf= new HDFGR();

		dispHdfGRInfo(fid, node);
		break;
	    }
	    case node.Vdata:  {  // HDF VDATA
		// vdata processing
		dispHdfVdataInfo(fid,node);
		break;
	    }    
		
	    case  node.Vgroup:   // HDF VGROUP

		// vgroup processing
		dispHdfVgroupInfo(fid,node);
		break;
	    }
    } catch (HDFException e) {
	System.out.println("Warning:  Exception caught and ignored (expandCollapse)");
    }

    // set cursor type to "DEFAULT_CURSOR"
    ((Component)applet_.jhvFrame).setCursor(new Cursor(Cursor.DEFAULT_CURSOR));
  }
  

  /** Display the message 
   * @param msg the message to be displayed 
   */
  public void dispMessage(String msg)
  {
    if (msg != null) {
      // display the info.
      applet_.infoText.setText(msg); 

      // scroll
      int pos = applet_.infoText.getText().length() - 1;
      if (pos<0)
	pos = 0;
      applet_.infoText.select(pos,pos);
      
    }
  }
}
