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
import java.awt.event.*;
import java.awt.image.*;
import java.net.*;
import java.applet.*;
import java.io.*;
import java.util.*;
import java.net.*;

import ncsa.hdf.hdflib.*;
import ncsa.hdf.util.*;
import ncsa.hdf.awt.*;
import ncsa.hdf.awt.image.*;
import ncsa.hdf.message.*;
import ncsa.hdf.io.ASCII2HDF;

/**
 * JHV  displays the HDF tree structure and data content
 * Modifications:
 *     1. Keep track of current HDF directory, and the selected HDF file will be
 *        located under previous selected HDF file directory.  Also improve the
 *        "Load File" and let it do more work than "Open file".  08/08/97 xlu
 *     2. Support lagre HDF file access, more changes.    09/06/97 xlu
 *     3. Separate hdf file access from the UI. 01/14/98, Peter Cao
 *     4. Support remote file access. 01/14/98, Peter Cao
 *     5. Add documentation. 01/14/98, Peter Cao
 * @version 1.00   8/10/97
 * @author Xinjian LU, HDF Group, NCSA
 */
public class JHV extends Applet implements ActionListener,ItemListener {

    /** indicator for remote access */
    boolean isSuppportRemoteAccess = false;

    /** the local property file name */
    public static final String USER_PROPS = "jhv.props";

    /** the user's display level */
    public static final int USER      = 0;

    /** the developer's display level */
    public static final int DEVELOPER = 1;

    /** default view selection */
    public static final int  DEFAULT   = 0;

    /** animation view selection */
    public static final int  ANIMATION = 1;

    /** layer view selection */
    public static final int  LAYER     = 2;

    /** new line feed plus carrage return */
    public static char[] nl = {13, 10};

    /** the new line charactor */
    public static final String NL = new String(nl);

    /** data delimiter */
    public static String delimiter = "\t";

    /** hold the user properties */
    Properties props = null;

    /** the frame to hold this viewer */
    Frame jhvFrame;

    /** the HDFTree to display the HDF data hierarchy */
    HDFTree hdfTree;

    /** The TextArea to display text information */
    TextArea infoText;

    /** canvas to display the preview HDF image */
    JHVCanvas hdfCanvas;

    //JHVCanvas hdfIconCanvas;
    
    /** the HDF logo */
    Image  hdfIcon = null;
    
    /** the closed file folder icon */
    Image lDefault;

    /** the file icon */
    Image lFile;

    /** the opened file folder icon */
    Image lCollapse;

    /** the hdf annotation icon */
    Image annImg;
  
    /** the hdf raster image icon */
    Image risImg;
 
    /** the hdf sds icon */
    Image sdsImg;

    /** the hdf vdata icon */
    Image vdImg;

    /** the hdf Vgroup icon */
    Image  vgImg= null;

    /** the warngin icon */
    Image  warningIcon= null;

    /** indicating if icons are loaded */
    boolean iconLoaded = false;

    /** the hdf file source */
    TextField hdfFileText;

    /** the hdf file name */
    String hdfFile;

    /** the ascii file name */
    String asciiFile;

    /** the queue to store the hdf object */
    Queue hdfObjQueue = null;

    // variables for duble-buffer
    Image offScreenImage = null;
    Graphics offGraphics;
    
    /** indicating if the file is local or remote */
    boolean isLocal = true;
    
    /** the JHV home directory */
    String jhvDir = ".";

    /** the jhv incon directory */
    String jhvIconDir = ".";

    String uHome = null;

    /** indicating if the JHV starts as an applet */
    boolean isApplet = false;
    
    /** the Vdata frame */
    JHVVdataFrame  vdataFrame = null;

    /** to keep only on Vdata frame alive a time */
    boolean  vdataFrameDisplayed = false;

    // define a display level for HDF tree
    public int dispLevel   = USER;

    /** the current hdf file directory */
    String cDir;

    // menu of "view"
    CheckboxMenuItem defItem, anItem, layerItem;
    int dispMode = DEFAULT;
	
    //  JHVImageAnimation
    JHVImageAnimation animatedImages = null;

    //  JHVLayeredImage
    JHVLayeredImage layeredImages = null;

    /** the remote machine name the the server */
    private String serverHost = "";

    /** the port number of the server */
    private int serverPort = 8181;

    /** a list of server machines */
    private List hostList;

    /** the server host menu */
    private Menu hostMenu;

    /** indicates if the file is a hdf file or not */
    private boolean isHDF = true;

    /** the file save menuitem */
    private MenuItem menuSave = null;

    /**
     * It is called automatically by the system the application is started.
     */
    public static void main(String args[])
    {
        // create new jhv object
        JHV jhv = new JHV();
        Frame frame = new Frame("HDF Viewer");
        jhv.setJHVFrame(frame);

        String tmpHDFFile = "";
        File tmpFile = null;
        jhv.jhvIconDir = "";

        for (int i = 0; i < args.length; i++) {
            if ("-jhvpath".equalsIgnoreCase(args[i])) {
 	        try {
                    tmpFile = new File(args[i+1]);
                    if (tmpFile.isDirectory()) jhv.jhvDir = tmpFile.getPath();
                    else jhv.jhvDir = tmpFile.getParent();
                    i++;
                } catch (Exception e) {}
            }
            else if ("-jhviconspath".equalsIgnoreCase(args[i])) {
	        try {
                    tmpFile = new File(args[i+1]);
                    if (tmpFile.isDirectory()) jhv.jhvIconDir = tmpFile.getPath();
                    i++;
                }
                catch (Exception e) {}
            } else {
		if (args[i] != null)
		   tmpHDFFile = new String(args[i]);
	    }
        }

        jhv.props = new Properties();
	String libDir = null;
        tmpFile = new File(jhv.jhvDir);
	libDir = tmpFile.getParent();
        String propsfile = libDir + File.separator + "lib" + File.separator +JHV.USER_PROPS;
        try {
            FileInputStream fis = new FileInputStream(propsfile);
            jhv.props.load(fis);
            fis.close();
        } catch (Exception e) {}
//{System.out.println("Exception reading system properties "+e.getMessage());}

	jhv.uHome = System.getProperty("user.home");
	propsfile = jhv.uHome + File.separator + JHV.USER_PROPS;
        try {
            FileInputStream fis = new FileInputStream(propsfile);
            jhv.props.load(fis);
            fis.close();
        } catch (Exception e) {
		jhv.uHome = null;
/*System.out.println("Exception reading user properties "+ e.getMessage());*/}

        if (jhv.jhvIconDir == null || jhv.jhvIconDir.length() < 1 )
            jhv.jhvIconDir = jhv.jhvDir+File.separator+"lib"+File.separator+"hdficons";

        jhv.cDir = "."; //jhv.jhvDir;
        jhv.init(tmpHDFFile);

        WindowListener adapter = new WindowAdapter()
        {
            public void windowClosing(WindowEvent e)
            {
                System.exit(0);
            }
        };
        frame.addWindowListener(adapter);
        frame.setLayout(new BorderLayout());
        frame.setMenuBar(jhv.createMenuBar());
        frame.add("Center", jhv);
        frame.setVisible(true);
        frame.setSize(600,450);
        frame.show();
    }

    /**
     * Initialize jhv with specified hdf file
     * @param hdffile  the name of hdf file
     */
    public void init(String hdffile)
    {
        //System.out.println("JHV.init( "+hdffile+")");

        String dlm = props.getProperty("data.delimiter");
        if (dlm !=null) delimiter = dlm;
        hdfFile = new String(hdffile);
        hdfTree = new HDFTree();
        setLayout(new BorderLayout());

        // information panel
        infoText = new TextArea();
        infoText.setFont(new Font("Courier", Font.PLAIN, 12));
        infoText.setEditable(false);

        // load icon images
        if (!(iconLoaded)) {
            Toolkit toolkit = getToolkit();
            iconLoaded = true;

            String defaultImg = new String(jhvIconDir +"/default.gif");
            String fileImg = new String(jhvIconDir+"/file.gif");
            String collapseImg = new String(jhvIconDir+"/collapse.gif");
            String hdfStr= new String(jhvIconDir+"/hdf.gif");
            String annStr= new String(jhvIconDir+"/ann.gif");
            String risStr= new String(jhvIconDir+"/ris.gif");
            String sdsStr= new String(jhvIconDir+"/sds.gif");
            String vdStr = new String(jhvIconDir+"/vdata.gif");
            String vgStr = new String(jhvIconDir+"/vgroup.gif");
            String wnStr = new String(jhvIconDir+"/warning.gif");

            lDefault = toolkit.getImage(defaultImg );
            lFile    = toolkit.getImage(fileImg    );
            lCollapse= toolkit.getImage(collapseImg);
            hdfIcon= toolkit.getImage(hdfStr);
            annImg = toolkit.getImage( annStr);
            risImg = toolkit.getImage( risStr);
            sdsImg = toolkit.getImage( sdsStr);
            vdImg  = toolkit.getImage(  vdStr);
            warningIcon  = toolkit.getImage(  wnStr);
        }

        // orgnize the panel
        add("Center",createDisplayPanel());

        // set the frame incon only if the image exists
        if ((new File(jhvIconDir+File.separator+"hdf.gif")).exists())
            jhvFrame.setIconImage(hdfIcon);

        // get the initialized tree structure
        hdfTree.refresh();

        File tmpFile = null;
        try {
            tmpFile = new File (hdfFile);
        } catch (Exception ex) { return; }

	String curDir = null;
        if (tmpFile.isDirectory())
        {
            curDir = tmpFile.getPath();
            return;
        } else cDir = tmpFile.getParent();
        if (curDir != null) cDir = curDir;
        if (hdfFile !=null && hdfFile.length() > 1)
        {
            setup(hdfFile);
	    hdfTree.resetTreeOffset();
	    hdfTree.refresh();
            hdfCanvas.setImage(null);
            hdfCanvas.repaint();
        }
    }

    public Properties getProperties()
    {
        return props;
    }

    /**
     *  handles the viewer action events.
     *  modified by Peter Cao (xcao@ncsa.uiuc.edu) on Jan. 13, 1998
     */
    public void actionPerformed(ActionEvent e)
    {
        String arg = e.getActionCommand();
        Object source = e.getSource();

        if (source instanceof MenuItem)
            arg = ((MenuItem)source).getLabel();

        // load file from a typed in name
        if (source.equals(hdfFileText) ||
            arg.equals("Open File") )
        {
            hdfFile = hdfFileText.getText().trim();
            int portIndex = -1;
            if (hdfFile.length() > 6)
                portIndex = (hdfFile.substring(6)).indexOf(":");

            if (portIndex > 0)
            {
                this.isLocal = false;
                int fileIndex = -1;
                String path = "";
                hdfFile = hdfFile.replace('\\', '/');
                String location = hdfFile;

                String prefix = hdfFile.substring(0, 7);
                if ( prefix.equalsIgnoreCase("http://"))
                {
                    // java web server
                    String suffix = hdfFile.substring(7);
                    int doubleSlash = suffix.indexOf("//");
                    while (doubleSlash > 0)
                    {
                        // remove the extra slash character
                        suffix = suffix.substring(0, doubleSlash)+suffix.substring(doubleSlash+1);
                        doubleSlash = suffix.indexOf("//");
                    }
                    int servletIndex = suffix.indexOf("/servlet");
                    int pathIndex = suffix.indexOf("/");
                    if (servletIndex > 0)
                        pathIndex = suffix.indexOf("/", servletIndex+9);
                    else
                        pathIndex = suffix.indexOf("/", pathIndex+1);
                    if (pathIndex > 0)
                    {
                        location = "http://"+suffix.substring(0, pathIndex);
                        path = suffix.substring(pathIndex);
                    }
                }
                else
                {
                    // standlone server
                    fileIndex = hdfFile.indexOf("/");
                    if (fileIndex > 0)
                    {
                        location = hdfFile.substring(0, fileIndex);
                        path = hdfFile.substring(fileIndex);
                    }
                }
                if (path == null) path = "";

                hdfFile = null;
                openFile(location, path);
                if (hdfFile == null) return;
            }
            else
            {
               this.isLocal = true;
               if (!isFile(hdfFile))
               {
                    if (isDirectory(hdfFile))
                        cDir = hdfFile;
                    openFile();
               }
            }

            // load the HDF hierrchy tree
            infoText.setText("");
            setup(hdfFile);

	    hdfTree.resetTreeOffset();
	    hdfTree.refresh();
            hdfCanvas.setImage(null);
            hdfCanvas.repaint();
        }

        // clean the information area
        else if (arg.equals("Clean")) {
            infoText.setText("");
            infoText.select(0,0);
        }

        // open local file from a file manager
        else if (arg.equals("Open Local File"))
        {
            // an HDF file source
            openFile();
            setup(hdfFile);

            hdfTree.resetTreeOffset();
            hdfTree.refresh();
            hdfCanvas.setImage(null);
            hdfCanvas.repaint();
        }

        // open a remote file from a remote file dialog
        else if ( ( (Menu) ((MenuItem)source).getParent() )
            .getLabel().equals("Open Remote File"))
        {
            String selectedHost = "";
            isLocal = false;

            int length = hostList.getItemCount();
            for (int i = 0; i < length; i++)
            {
               if (arg.equals(hostList.getItem(i)))
               {
                  selectedHost = hostList.getItem(i);
                  i = length;
               }
            }
            hdfFile = null;
            openFile(selectedHost, "");
            if (hdfFile == null) return;

            // load the HDF hierrchy tree
            infoText.setText("");
            setup(hdfFile);

	    hdfTree.resetTreeOffset();
	    hdfTree.refresh();
	    hdfCanvas.setImage(null);
	    hdfCanvas.repaint();
        }

        // exit JHV
        else if (arg.equals("Exit"))
        {
            //remove the old temperal hdf file
            if (!isHDF)
            {
                File tmpFile = new File (asciiFile);
                String name = new String(tmpFile.getParent()+File.separator+"."+tmpFile.getName()+".hdf");
                try { (new File (name)).delete(); }
                catch (Exception ex) {}
            }

            System.exit(0);
        }

        // edit remote host list
        else if (arg.equals("HDF Server"))
        {
            HostEditor hostEditor = new HostEditor(jhvFrame, hostList);
            if (hostEditor.isChanged())
            {
                updateServerHosts(hostEditor.getHostList());
            }
        }

        // setup preferences
        else if (arg.equals("Preferences")) {}

        // display JHV version information
        else if (arg.equals("About")) {
            String about = "Java HDF Viewer\n"+
                           "Version 2.2\n"+
                           "Copyright "+'\u00a9'+" 1998 NCSA\n"+
                           "All rights reserved";

            InfoDialog id = new InfoDialog(jhvFrame, "About JHV", about, hdfIcon, false);
            id.show();
        }

        // save hdf file
        else if (arg.equals("Convert To HDF File"))
        {
            try { save(); }
            catch (Exception ex) { infoText.setText(ex.getMessage()); }
        }

        // open online user's guide
        else if (arg.equals("Online Guide"))
        {
            try { openOnlineGuide(); }
            catch (Exception ex) { infoText.setText(ex.getMessage()); }
        }

        // for futher implementation
        else {
            infoText.setText("This action is not implemented.");
        }

    }

    /**
     *  reset the viewer and display the HDF hierarchy 
     *  when a new file is selected.
     *  @param hdfFileName  the name of the hdf file
     */
    public void setup(String hdfFileName)
    {
        //System.out.println("JHV.setup()");

        //System.gc();
        if ( (new File (hdfFileName)).isDirectory())
            return;

        jhvFrame.setCursor(new Cursor(Cursor.WAIT_CURSOR));
        Vector hdfObjVec = new Vector();

       // clean up the HDF object Queue, and the HDF tree
        if (hdfObjQueue == null) hdfObjQueue = new Queue();
        else hdfObjQueue.removeAllElements();
        hdfTree.removeAllTreeNodes();

        //remove the old temporary hdf file
        if (!isHDF)
        {
            File tmpFile = new File (asciiFile);
            String name = new String(tmpFile.getParent()+File.separator+".~"+tmpFile.getName()+".hdf");
            try { (new File (name)).delete(); }
            catch (Exception e) {}
        }

        this.hdfFile = hdfFileName;


        // check if the file is an hdf file
        // if not, convert the file to hdf file first
        if (HDFObject.isHDF(hdfFile))
        {
            isHDF = true;
            if (menuSave != null) menuSave.setEnabled(false);
        }
        else
        {
            isHDF = false;
            if (menuSave != null) menuSave.setEnabled(true);
            asciiFile = hdfFile;
            File tmpFile = new File (asciiFile);
            hdfFile = new String(tmpFile.getParent()+File.separator+".~"+tmpFile.getName()+".hdf");
            ASCII2HDF converter = null;
            try { converter = new ASCII2HDF(asciiFile, hdfFile); }
            catch (Exception e) {}
            if (converter != null) converter.convert();
        }

        try {
            HDFHierarchy hierarchy = (HDFHierarchy) getHDFObject(null);
            hdfObjQueue = hierarchy.getQueue();
        }
        catch (Exception e){jhvFrame.setCursor(new Cursor(Cursor.DEFAULT_CURSOR));}

        if ( hdfObjQueue==null || hdfObjQueue.size()<1 )
        {
            jhvFrame.setCursor(new Cursor(Cursor.DEFAULT_CURSOR));
            return;
        }

        extractHdfObj(hdfObjQueue, hdfObjVec);

        // add the TreeNodes to the tree.
        TreeNode addedNode;
        for (int i = 0; i < hdfObjVec.size(); i++)
        {
            HDFObjectNode objNode;
            objNode = (HDFObjectNode)hdfObjVec.elementAt(i);
      
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
            jhvFrame.setCursor(new Cursor(Cursor.DEFAULT_CURSOR));
        }
    }

    /**
     *  create the new display panel for JHV
     *  @return the panel for HDF Browser
     */
    public Panel createDisplayPanel()
    {
        Panel dispPanel = new Panel();
        dispPanel.setLayout(new BorderLayout());

        // HDF file text field and file button
        hdfFileText = new TextField("No File Selected");
        if (hdfFile != null) hdfFileText.setText(hdfFile);
        hdfFileText.addActionListener(this);
        Button fileButton = new Button("Open File");
        fileButton.addActionListener(this);
        EasyConstrainsPanel panel0 = new EasyConstrainsPanel();
        panel0.setBackground(Color.lightGray);
        GridBagLayout gridBagLayout0 = new GridBagLayout();
        panel0.setLayout(gridBagLayout0);
        panel0.setConstraints(fileButton, 0, 0, 1, 1
            ,GridBagConstraints.NONE, GridBagConstraints.WEST, 0.0, 0.0);
        panel0.setConstraints(hdfFileText, 1, 0, 1, 1
            ,GridBagConstraints.HORIZONTAL, GridBagConstraints.WEST, 1.0, 0.0);

        //add the hdftree and the preview image canvas
        Panel  panel1 = new Panel();
        panel1.setLayout(new GridLayout(1, 2));
        hdfCanvas = new JHVCanvas(this);
        panel1.add(hdfTree);
        panel1.add(hdfCanvas);

        // do not add textfiled for Mac machine
        if (!isMac())
            dispPanel.add("North", panel0);

        dispPanel.add("Center", panel1);
        dispPanel.add("South", infoText);

        return dispPanel;
    }

    /** Is the current platform MacOS */
    public  boolean isMac() {
        String osName = System.getProperty("os.name");
        if (osName == null) return false;
        return  (osName.toLowerCase().startsWith("mac")) ;
    }

    /** Is the current platform Windows 95/NT */
    public  boolean isWin() {
        String osName = System.getProperty("os.name");
        if (osName == null) return false;
        return  (osName.toLowerCase().startsWith("win")) ;
    }


    /** Is local access */
    public  boolean isLocal() { return isLocal; }

    /** Returns the HDFTree */
    public HDFTree getTree() { return hdfTree; }

    /**
     * extract the HDF object node into Vector
     * @param hdfQueue  the HDF object node Queue
     * @param vec       the vector to store the HDF object node sequencely
     */
    public  void extractHdfObj(Queue  hdfQueue, Vector vec) {
        HDFObjectNode node = null;
        Queue subQueue = null;

        if (hdfQueue.size() == 0) return;

        for (int i=0; i<hdfQueue.size(); i++) {
            node = (HDFObjectNode)hdfQueue.elementAt(i);
            vec.addElement(node);
            if (node.child != null) {
                subQueue = (Queue) (node.child);
                extractHdfObj(subQueue, vec);
            }
        }
    }

    /** create menubar for jhv viewer */
    public MenuBar createMenuBar() {

        MenuBar  jhvMenuBar = new MenuBar();
        jhvMenuBar.add(createFileMenu("File"));
        if(isSuppportRemoteAccess)
            jhvMenuBar.add(createOptionMenu("Options"));
        jhvMenuBar.add(createViewMenu("View"));
        jhvMenuBar.add(createHelpMenu("Help"));

        return  jhvMenuBar;
    }

    /** create a open menu by provided title
     * @param menuTitle the menu title
     */
    public Menu createFileMenu(String  menuTitle) {

        Menu fileMenu = new Menu(menuTitle);
	MenuItem menuOpen = new MenuItem("Open Local File",new MenuShortcut(KeyEvent.VK_O));
	menuOpen.addActionListener(this);
        fileMenu.add(menuOpen);

        if (isSuppportRemoteAccess)
        {
            try {
                setHostList(props.getProperty("hdf.server"));
            } catch (Exception ex) {}
            hostMenu = new Menu("Open Remote File");
            hostMenu.addActionListener(this);
            fileMenu.addSeparator();
            fileMenu.add(hostMenu);
            addHostMenuItems();
        }

        menuSave = new MenuItem("Convert To HDF File",new MenuShortcut(KeyEvent.VK_S));
        if (isHDF) menuSave.setEnabled(false);
        else menuSave.setEnabled(true);
	menuSave.addActionListener(this);
        fileMenu.addSeparator();
        fileMenu.add(menuSave);

	MenuItem menuExit = new MenuItem("Exit",new MenuShortcut(KeyEvent.VK_E));
	menuExit.addActionListener(this);
        fileMenu.addSeparator();
        fileMenu.add(menuExit);

        return fileMenu;
    }

   /**
    * create an option menu with specific menu title
    * @param title the menu title
    * @return the option menu
    * @auther Peter Cao (xcao@ncsa.uiuc.edu)
    */
    public Menu createOptionMenu(String  title) {

        Menu menu = new Menu(title);
	MenuItem pref = new MenuItem("Preferences");
	MenuItem edit = new MenuItem("HDF Server");

	pref.addActionListener(this);
	edit.addActionListener(this);

        menu.add(pref);
        menu.addSeparator();
        menu.add(edit);

        return menu;
    }

    /** create a view menu by provided title
     * @param menuTitle the menu title
     */
    public Menu createViewMenu(String  menuTitle) {
        // new menu by specified menuTitle
        Menu viewMenu = new Menu(menuTitle);

	defItem = new CheckboxMenuItem("Default",true);
	defItem.addItemListener(this); 
        viewMenu.add(defItem);

	// add menuItem  
	anItem = new CheckboxMenuItem("Animation");
	anItem.addItemListener(this);  
	viewMenu.add(anItem);

	layerItem = new CheckboxMenuItem("Layer");
	layerItem.addItemListener(this); 
        viewMenu.add(layerItem);

        return viewMenu;      
    }

   /**
    * create a help menu with specific menu title
    * @param title the menu title
    * @return the option menu
    * @auther Peter Cao (xcao@ncsa.uiuc.edu)
    */
    public Menu createHelpMenu(String  title) {

        Menu menu = new Menu(title);
	MenuItem online = new MenuItem("Online Guide");
	online.addActionListener(this);
        menu.add(online);
        menu.addSeparator();
	MenuItem about = new MenuItem("About");
	about.addActionListener(this);
        menu.add(about);

        return menu;
    }

     /**
      * Determines whether the specified string is a local file
      * @param str the file string
      * @return true if str is a file, otherwise false
      */
    public boolean isFile(String str) {
         return((new File(str)).isFile());
    }

    public boolean isDirectory(String str) {
        return((new File(str)).isDirectory());
    }

    /**
     *  open a file from a remote machine
     *
     *  @param location  the remote machine
     *  @param afilename the requested hdf file
     */
    public void openFile(String location, String afilename)
    {
        String prefix = location.substring(0, 7);

        // java web server
        if ( prefix.equalsIgnoreCase("http://"))
        {
                serverHost = location;
                serverPort = -1;
        }
        else
        {
            int portPosition = location.indexOf(":");
            if (portPosition > 0)
            {
                serverHost = location.substring(0, portPosition);
                try { serverPort = Integer.parseInt(location.substring(portPosition+1),10); }
                catch (Exception e) {}
            }
        }

        // check if the connection is OK before doing the actually work
        // if the connection fails, stop accessing to the remote file.
        jhvFrame.setCursor(new Cursor(Cursor.WAIT_CURSOR));
        java.net.Socket server = null;
        if (serverPort > 0) {
            try { server = new java.net.Socket(serverHost, serverPort); }
            catch (Exception ex)
            {
                infoText.setText(ex.toString());
                jhvFrame.setCursor(new Cursor(Cursor.DEFAULT_CURSOR));
                return;
            }
            try { server.close(); }
            catch (Exception ex){}
        }
        else {
            try { new java.net.URL(serverHost); }
            catch (Exception ex)
            {
                infoText.setText(ex.toString());
                jhvFrame.setCursor(new Cursor(Cursor.DEFAULT_CURSOR));
                return;
            }
        }

        jhvFrame.setCursor(new Cursor(Cursor.DEFAULT_CURSOR));
        RemoteFileDialog dl = new RemoteFileDialog(jhvFrame, "Remote File Tree",
            serverHost, serverPort, lCollapse, lDefault, lFile);
        dl.init(afilename);

        String filename = dl.getFile();
        if (filename == null || filename.trim().length() == 0)
        {
            hdfFile = null;
            return;
        }

        // cut out junk stuff from a file name
        filename = filename.replace('/', File.separatorChar);
        filename = filename.replace('\\', File.separatorChar);
        String sepSep = File.separator+File.separator;
        int sepSepIndex = filename.indexOf(sepSep);
        while (sepSepIndex >= 0)
        {
            if (sepSepIndex == 0)
                filename = filename.substring(sepSepIndex+1);
            else
                filename = filename.substring(0,sepSepIndex)+filename.substring(sepSepIndex+1);
            sepSepIndex = filename.indexOf(sepSep);
        }

        hdfFile = filename;
        hdfFileText.setText(location+hdfFile);
    }

    /** opens a local file */
    public void openFile()
    {
        this.isLocal = true;
        Frame parent = getFrame();
        if (parent==null) parent = jhvFrame;
        if (parent==null) parent = new Frame("");

        FileDialog fd = new FileDialog(parent, "HDF File");

        fd.setDirectory(cDir);
        fd.show();

        hdfFile = null;

        String separator = File.separator;
        if ((fd.getDirectory() != null) && (fd.getFile() != null)) {
            hdfFile = fd.getDirectory() + fd.getFile();
            cDir = fd.getDirectory();

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
     * java.awt.event.ItemListener stuff
     */
    public void itemStateChanged(ItemEvent e)
    {
        Object target = e.getSource();

        if (target instanceof CheckboxMenuItem)
        {
            String menu = ((Menu) ((MenuItem)target).getParent()).getLabel();
            if  (menu.equals("View"))
            {
                String label = ((MenuItem)target).getLabel();

                defItem.setState(false);
                anItem.setState(false);
                layerItem.setState(false);

                if ("Default".equals(label)) {
                    defItem.setState(true);
                    dispMode = DEFAULT;
                } else if ("Animation".equals(label))  {
                    anItem.setState(true);
                    dispMode = ANIMATION;
                } else if ("Layer".equals(label))  {
                    layerItem.setState(true);
                    dispMode = LAYER;
                }
            }
        } // end of (target instanceof CheckboxMenuItem)
    }

    public List getHostList() { return hostList; }

    // add hosts into the host menu
    public void addHostMenuItems()
    {
        if (hostList==null) return;
        
        int length = hostList.getItemCount();
        for (int i=0; i<length; i++)
        {
            MenuItem theItem = new MenuItem(hostList.getItem(i));
            hostMenu.add(theItem);
            theItem.addActionListener(this);
        }
    }

    // remove hosts from the host menu
    public void removeHostMenuItems()
    {
        int length = hostMenu.getItemCount();
        for (int i = length - 1; i >= 0; i--)
        {
            MenuItem theItem = hostMenu.getItem(i);
            theItem.removeActionListener(this);
            hostMenu.remove(i);
        }
    }

    /**
     *  set up HDF server hosts from the user properties
     *  @param hosts  the string of hosts separated by space
     */
    public void setHostList(String hosts)
    {
        List list = new List();
        String separator = " ";
        String theHost = hosts.trim();

        int index = theHost.indexOf(separator);
        while (index >0)
        {
           list.addItem(theHost.substring(0, index));
           theHost = theHost.substring(index+1).trim();
           index = theHost.indexOf(separator);
        }
        if (theHost.length() > 1) list.addItem(theHost.trim());

        this.hostList = list;

    }

    /**
     *  updates the current host list and change the configuration file
     *  @param list  the new list of server hosts
     */
    public void updateServerHosts(List list)
    {
        removeHostMenuItems();
        hostList = list;
        addHostMenuItems();
        //writeHosts(jhvDir+File.separator+USER_PROPS, list);
	if (uHome != null) {
        writeHosts(uHome+File.separator+USER_PROPS, list);
	} else {
        writeHosts(jhvDir+File.separator+"lib"+File.separator+USER_PROPS, list);
	}
    }

    /**
     *  writes the server host list into the user property file
     *  which also deletes the old host list the user property file
     *
     *  @param filename  The name of the configuration file
     *  @param hosts     The list of server hosts
     */
    public void writeHosts(String filename, List hosts)
    {
        if (hosts==null) return;
        int n_hosts = hosts.getItemCount();
        if (n_hosts < 1) return;

	Properties ptmp = new Properties();

        try {
            FileInputStream fis = new FileInputStream(filename);
            ptmp.load(fis);
            fis.close();
        } catch (Exception e) {
/*System.out.println("Exception reading system properties "+e.getMessage());*/
        infoText.setText("Cannot read properties file to update: "+filename);
	}

        String separator = " ";
        String hostStr = hosts.getItem(0);
        for (int i=1; i<n_hosts; i++) {
            hostStr += separator+hosts.getItem(i);
        }
        ptmp.put("hdf.server", hostStr);

        try {
            FileOutputStream fos = new FileOutputStream(filename);
            String header = "hdf.server modified on";
            ptmp.save(fos, header);
            fos.close();
        } catch (Exception e) {
            infoText.setText("Cannot update properties file: "+filename);
	    return;
	}
        infoText.setText("Updated properties file: "+filename);

    }

    /**
     *  get the HDFObject from server
     *
     *  @param node        The selected node in the hdf hierarchy tree
     *  @return            The HDFObject containing the requested data
     *  @auther            Peter Cao (xcao@ncsa.uiuc.edu), 10/2/97
     */
    public HDFObject getHDFObject(HDFObjectNode node)
    {
        if (isLocal)
            return this.getHDFObject(null,-1,hdfFile,node);
        else
            return this.getHDFObject(serverHost,serverPort,hdfFile,node);
    }

    /**
     *  get the HDFObject from server
     *
     *  @param host        The name of the remote machine
     *  @param port        The port number of the server
     *  @param filename    The hdf file name
     *  @param node        The selected node in the hdf hierarchy tree
     *  @return            The HDFObject containing the requested data
     *  @auther            Peter Cao (xcao@ncsa.uiuc.edu), 10/2/97
     */
    public HDFObject getHDFObject(String host, int port, String filename,
        HDFObjectNode node)
    {
        //System.out.println("JHV.getHDFObject()");

        Socket server = null;
        ObjectOutputStream output = null;
        ObjectInputStream input = null;
        HDFObject hdfObject = null;
        HDFMessage message = null;

        if (isMac())
        {
            filename = filename.replace('\\', ':');
            filename = filename.replace('/', ':');
            if (filename.charAt(0) == ':')
                filename = filename.substring(1);
            else
                filename = ':'+filename;
        }

        //if (!isHDF) hdfObject = new HDFASCII (node, filename);
        //else if  (node == null)
        if  (node == null)
            hdfObject = new HDFHierarchy (node, filename);
        else if  (node.type == HDFObjectNode.Annotation)
            hdfObject = new HDFAnnotation(node, filename);
        else if  (node.type == HDFObjectNode.RIS8)
            hdfObject = new HDFRIS8(node, filename);
        else if  (node.type == HDFObjectNode.RIS24)
            hdfObject = new HDFRIS24(node, filename);
        else if ((node.type == HDFObjectNode.GRGLOBALATTR)  ||
            (node.type == HDFObjectNode.GRDATASETATTR)      ||
            (node.type == HDFObjectNode.GRDATASET)          ||
            (node.type == HDFObjectNode.GRDATASETAN)  )
            hdfObject = new HDFGR(node, filename);
        else if ((node.type == HDFObjectNode.SDSGLOBALATTR) ||
            (node.type == HDFObjectNode.SDSDATASETATTR)     ||
            (node.type == HDFObjectNode.SDSDATASET)         ||
            (node.type == HDFObjectNode.SDSDATASETAN))
            hdfObject = new HDFSDS(node, filename);
        else if  (node.type == HDFObjectNode.Vdata)
            hdfObject = new HDFVdata(node, filename);
        else if  (node.type == HDFObjectNode.Vgroup)
            hdfObject = new HDFVgroup(node, filename);
        else return hdfObject;

        // serve local file
        if (host == null || host.length() <1)
        {
            if (filename != null && filename.length() > 0)
                hdfObject.service();

            return hdfObject;
        }

        // serve remote file
        try {
            // Web server
            if (port < 1) {
                URL url = new URL(host);
                HttpURLConnection theConnection = (HttpURLConnection) url.openConnection();
                theConnection.setDoOutput(true);
                theConnection.setRequestProperty("Content-Type", "application/octet-stream");
                output = new ObjectOutputStream(theConnection.getOutputStream());
                output.writeObject(hdfObject.toServer());
                output.close();
                input = new ObjectInputStream (theConnection.getInputStream());
                message = (HDFMessage) input.readObject();
                input.close();
            }
            else {
                server = new Socket(host, port);
                output = new ObjectOutputStream (server.getOutputStream());
                input = new ObjectInputStream (server.getInputStream());
                output.writeObject(hdfObject.toServer());
                message = (HDFMessage) input.readObject();
                output.close();
                input.close();
                server.close();
            }

            if (message == null) return null;
            hdfObject.fromServer(message);
        }
        catch (Exception exception) {infoText.setText(exception.toString());}

        return hdfObject;
    }

    public boolean isHDF() {return isHDF; }
    
    /** save the data into HDF file or ASCII file */
    public void save()
    {
        // get the output file name
        FileDialog fd = new FileDialog(jhvFrame, "Save HDF File", FileDialog.SAVE);
        fd.setDirectory(cDir);
        fd.show();
        String theFile = fd.getFile();
        if (theFile == null) return;
        theFile = theFile.trim();
        String dir = fd.getDirectory();
        String fullname = dir + theFile;

        String warning = null;
        InfoDialog id = null;
        File outFile = new File(fullname);

        if (outFile.exists() ) {
            if (!outFile.canWrite())
            {
                warning = fullname+"\n"+ "This file is read only.\n"+ "File save failed.";
                infoText.setText(warning);
                id = new InfoDialog(jhvFrame, fd.getTitle(), warning, warningIcon, false);
                id.show();
                return;
            }
        }

        if (fullname.equals(hdfFile))
        {
            warning = fullname+"\n"+
                "This file is currently in use.\n"+
                "Overwriting the file causes inconsistency.\n"+
                "The JHV may not work until the file is reloaded.\n" +
                "  \n"+
                "Replace working file ?";
            infoText.setText(warning);
            id = new InfoDialog(jhvFrame, fd.getTitle(), warning, warningIcon, true);
            id.setlabels("  Yes  ", "   No   ");
            id.show();
            if (!id.clickedOkButton()) return;
        }
        else if (!isMac() && !isWin())
        {
	if (outFile.exists()) {
            warning = fullname+"\n"+
                "This file already exists.\n"+
                "  \n"+
                "Replace existing file ?";
            infoText.setText(warning);
            id = new InfoDialog(jhvFrame, fd.getTitle(), warning, warningIcon, true);
            id.setlabels("  Yes  ", "   No   ");
            id.show();
            if (!id.clickedOkButton()) return;
	}
        }

        // reset the current working directory in JHV
        cDir = dir;

        if (outFile.exists()) outFile.delete();

        (new File(hdfFile)).renameTo(outFile);
        infoText.setText("Convert file to "+fullname+" successful.");
        setup(fullname);
    }

    /**
     *  Open online user's guide
     */
    public void openOnlineGuide()
    {
        boolean findBrowser = false;
        Runtime rt = Runtime.getRuntime();
        String command = "";
        String ugURL = "";
        String thePath = "";
        List bPathList = new List();

        // add browser path from the JHV property file
        if (props != null)
        {
            thePath = props.getProperty("browser.path");
            ugURL = props.getProperty("jhv.guide");
        }

        if (thePath != null)
            bPathList.add(thePath);

        if (ugURL == null || ugURL.length() <= 0)
            ugURL = "http://hdf.ncsa.uiuc.edu/java-hdf-html/jhv/UsersGuide/JHV2.2/";

        // add browser path from system call "which netscape"
        try {
            BufferedReader d = new BufferedReader(new InputStreamReader(
                rt.exec("which netscape").getInputStream()));
            thePath = "";

            while (thePath != null)
            {
                if (thePath.length() > 0)
                    bPathList.add(thePath);
                thePath = d.readLine();
            }

            d.close();
        } catch (Exception e) {}

        // add default browser path
        bPathList.add("C:\\Program Files\\Netscape\\Communicator\\Program\\netscape.exe");
        bPathList.add("c:\\program files\\netscape\\navigator\\program\\netscape.exe");
        bPathList.add("c:\\program files\\Microsoft Internet\\Iexplore.exe");
        bPathList.add("c:\\winnt\\explorer.exe");
        bPathList.add("/usr/sbin/netscape");
        bPathList.add("/usr/bin/X11/netscape");
        bPathList.add("/usr/local/bin/netscape");
        bPathList.add("/usr/local/netscape/netscape");

        int length = bPathList.getItemCount();
        for (int i = 0;i<length;i++)
        {
            thePath = bPathList.getItem(i);
            if ((new File(thePath)).isFile())
            {
                command = thePath;
                findBrowser = true;
                break;
            }
        }

        InfoDialog  id;
        String warning = "Could not load web browser.\n"+
            "\nEnter your browser path\n"+
            "e.g. c:\\program files\\Microsoft Internet\\Iexplore.exe\n"+
            "        /usr/local/bin/netscape\n";

        if (!findBrowser)
        {
            id = new InfoDialog(jhvFrame, "Web Browser",
                    warning, warningIcon, true, true);
            id.show();

            if (id.clickedOkButton())
                command = id.getTextField().getText();
            else
                return;
        }

        thePath = command;
        command += " "+ugURL;
        try { rt.exec(command); }
        catch (IOException ex)
        {
            warning = "Could not load web browser from \n"+ thePath+"\n";
            infoText.setText(warning);
        }

    }
}
