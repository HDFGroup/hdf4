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

    // thiese lines may be deleted after the recoding is done
    //static { System.loadLibrary("hdf"); }
    int fid;
    boolean isSuppportRemoteAccess = false;

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
    public static String NL = new String(nl);

    /** the host header in the configuration file */
    public static String HOSTHEADER = "Server";

    /** the frame to hold this viewer */
    Frame jhvFrame;

    /** the HDFTree to display the HDF data hierarchy */
    HDFTree hdfTree;

    /** The TextArea to display text information */
    TextArea infoText;

    /** canvas to display the preview HDF image */
    JHVCanvas hdfCanvas;

    JHVCanvas hdfIconCanvas;
    
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

    /** indicating if icons are loaded */
    boolean iconLoaded = false;

    /** the hdf file source */
    TextField hdfFileText;

    /** the hdf file name */
    String hdfFile;

    /** the queue to store the hdf object */
    public  Queue hdfObjQueue = null;

    /** the generic hdf object */
    public HDFLibrary hdf;

    // variables for duble-buffer
    Image offScreenImage = null;
    Graphics offGraphics;
    
    /** indicating if the file is local or remote */
    boolean isLocal = true;
    
    /** the JHV home directory */
    String jhvDir = ".";

    /** the jhv incon directory */
    String jhvIconDir = ".";

    /** the name of jhv configuration file */
    String configFilename = "jhv.ini";
    
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

    /**
     * It is called automatically by the system the application is started.
     */
    public static void main(String args[])
    {
        // create new jhv object
        JHV jhv = new JHV();

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
		   tmpHDFFile = new String(args[2]);
	    }
        }

        if (jhv.jhvIconDir == null || jhv.jhvIconDir.length() < 1 )
            jhv.jhvIconDir = jhv.jhvDir+File.separator+"lib"+File.separator+"hdficons";
        jhv.cDir = jhv.jhvDir;

        jhv.init(tmpHDFFile);

        Frame frame = new Frame("HDF Viewer");
        WindowListener adapter = new WindowAdapter()
        { public void windowClosing(WindowEvent e) {System.exit(0);} };
        frame.addWindowListener(adapter);
        frame.setLayout(new BorderLayout());
        frame.setMenuBar(jhv.createHdbMenuBar());
        frame.add("Center", jhv);
        frame.pack();
        frame.setSize(600,500);
        frame.show();
        jhv.setJHVFrame(frame);
    }

    /**
     * Initialize jhv with specified hdf file
     * @param hdffile  the name of hdf file
     */
    public void init(String hdffile) {

        hdfFile = new String(hdffile);

        // create a HDFTree control
        hdfTree = new HDFTree();

        // set layout manager
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

        //setup(hdfFile);

        // orgnize the panel
        add("Center",createDisplayPanel());

        // resize the default size for the HDF Browser
        setSize(600,400);

        // display hdf icon
        hdfIconCanvas.setSize(32,32);
        hdfIconCanvas.setImageSize(30,30);
        hdfIconCanvas.setImage(hdfIcon);

        // get the initialized tree structure
        hdfTree.refresh();
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
            arg.equals("Load File") )
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
        else if (arg.equals("Exit")) {
            System.exit(0);
        }

        // edit remote host list
        else if (arg.equals("Remote Host"))
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
                              "Version 2.1\n"+
                              "Copyright "+'\u00a9'+" 1998 NCSA";

            InfoDialog id = new InfoDialog(jhvFrame, "About JHV", about, hdfIcon);
            id.show();
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
    public void setup(String hdfFileName) {
    
        System.gc();

        jhvFrame.setCursor(new Cursor(Cursor.WAIT_CURSOR));
        Vector hdfObjVec = new Vector();

       // clean up the HDF object Queue, and the HDF tree
        if (hdfObjQueue == null) hdfObjQueue = new Queue();
        else hdfObjQueue.removeAllElements();
        hdfTree.removeAllTreeNodes();

        this.hdfFile = hdfFileName;
        try {
            HDFHierarchy hierarchy = (HDFHierarchy) getHDFObject(null);
            hdfObjQueue = hierarchy.getQueue();
        }
        catch (Exception e){jhvFrame.setCursor(new Cursor(Cursor.DEFAULT_CURSOR));}
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
    public Panel createDisplayPanel() {
    
        Panel dispPanel = new Panel();
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
        hdfFileText.addActionListener(this);
         
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

    /** Is the current platform MacOS */
    public  boolean isMac() {
        String osName = System.getProperty("os.name");
        if (osName == null) return false;
        return  (osName.toLowerCase().startsWith("mac")) ;
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
    public MenuBar createHdbMenuBar() {

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
            String configPath = jhvDir+File.separator;
            hostList = readHosts(configPath+configFilename);
            if (hostList.getItemCount()<1)
            {
                configPath += "bin"+File.separator;
                hostList = readHosts(configPath+configFilename);
            }
            configFilename = configPath+configFilename;
            hostMenu = new Menu("Open Remote File");
            hostMenu.addActionListener(this);
            fileMenu.addSeparator();
            fileMenu.add(hostMenu);
            addHostMenuItems();
        }

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
	MenuItem edit = new MenuItem("Remote Host");
	MenuItem server = new MenuItem("Local Server");

	pref.addActionListener(this);
	edit.addActionListener(this);
	server.addActionListener(this);

        menu.add(pref);
        menu.addSeparator();
        menu.add(edit);
        menu.add(server);

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
	MenuItem about = new MenuItem("About");
	about.addActionListener(this);
        menu.add(about);
        menu.addSeparator();

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
        FileDialog fd = new FileDialog(getFrame(), "HDF File");

        fd.setDirectory(cDir);
        fd.show();

        hdfFile = null;

        String separator = System.getProperty("file.separator");
        if ((fd.getDirectory() != null) && (fd.getFile() != null)) {
            if (isMac())
                hdfFile = fd.getDirectory() + separator + fd.getFile();
            else
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

    public void setHostList(List list) { this.hostList = list; }
    public List getHostList() { return hostList; }

    // add hosts into the host menu
    public void addHostMenuItems()
    {
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
     *  updates the current host list and change the configuration file
     *  @param list  the new list of server hosts
     */
    public void updateServerHosts(List list)
    {
        removeHostMenuItems();
        hostList = list;
        addHostMenuItems();
        writeHosts(configFilename, list);
    }

    /** 
     *  reads server host list from jhv configuration file
     *  
     *  @param filename  The name of the configuration file
     *  @param config    The configurations except the server hosts
     *  @return          The list of server hosts
     */
    public List readHosts(String filename)
    {
        RandomAccessFile raf = null;
        List hosts = new List();
        String line = new String();
        long pos = 0;
        long length = 0;

        // Read the config file.
        try {
            raf = new RandomAccessFile(filename, "r");
            length = raf.length();
        }
        catch (IOException e) { return hosts;}

        while (pos < length)
        {
            try { line = raf.readLine(); }
            catch (IOException e) { return hosts;}

             // read hosts
            line = line.trim();
            if (line.startsWith(HOSTHEADER))
                 hosts.addItem(line.substring(HOSTHEADER.length()).trim());
            try{ pos = raf.getFilePointer(); }
            catch (IOException e) { return hosts;}
        }
        try { raf.close(); }
        catch (IOException e) {}

        return hosts;
    }

    /**
     *  writes the server host list into the jhv configuration file
     *  which also deletes the old host list from the configuration file
     *
     *  @param filename  The name of the configuration file
     *  @param hosts     The list of server hosts
     */
    public void writeHosts(String filename, List hosts)
    {
        RandomAccessFile raf = null;
        String config = new String(); //The configurations except the server hosts
        String line = new String();
        String hostStr = "";
        File configFile = null;       // the current configuration file
        File backupFile = null;       // the backup of the original configuration file
        long pos = 0;
        long length = 0;

        int n_hosts = hosts.getItemCount();
        for (int i=0; i<n_hosts; i++) {
            hostStr += HOSTHEADER+" "+hosts.getItem(i) + NL;
        }
        // Read the config file.
        try {
            raf = new RandomAccessFile(filename, "r");
            length = raf.length();
            while (pos < length)
            {
                line = raf.readLine().trim();
                if (!line.startsWith(HOSTHEADER))
                   config += line + NL;
                pos = raf.getFilePointer();
            }
            raf.close();
        }
        catch (IOException e) {}

        try {
            configFile = new File(filename);
            backupFile = new File(filename+".bak");
        }
        catch (NullPointerException e) { return;}

        backupFile.delete();
        configFile.renameTo(new File(filename+".bak"));

        // write the config file.
        try {
            raf = new RandomAccessFile(filename, "rw");
            raf.writeBytes(config);
            raf.writeBytes(hostStr);
        }
        catch (IOException e) { return; }

        try { raf.close(); }
        catch (IOException e) { }
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
        Socket server = null;
        ObjectOutputStream output = null;
        ObjectInputStream input = null;
        HDFObject hdfObject = null;
        HDFMessage message = null;

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


}
