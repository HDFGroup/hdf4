package ncsa.hdf.jhv;

import java.util.Vector;
import java.awt.Frame;
import java.awt.Dialog;
import java.awt.Image;
import java.net.Socket;
import java.net.URL;
import java.net.HttpURLConnection;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowListener;
import java.awt.event.WindowEvent;
import ncsa.hdf.message.HDFMessage;
import ncsa.hdf.message.HDFFileList;
 
/**
 *  The RemoteFileDialog class displays a dialog window
 *  from which the user can select a file from remote machine. 
 *  Since it is a modal dialog, when the application calls its 
 *  show method to display the dialog, it blocks the rest of the
 *  application until the user has chosen a file. 
 *
 *  @version   1.1.3 October 15 1997
 *  @author    Peter X. Cao
 */
public class RemoteFileDialog extends Dialog
{
    HDFTree tree;
    Frame parentFrame;
    Image imgExpanded;
    Image imgFile;
    Image imgCollapsed;
    String filename;
    String serverHost = "";
    int serverPort = -1;

    public RemoteFileDialog(Frame parent, String title, String host, int port,
        Image expandedImg, Image collapsedImg, Image fileImg)
    {
        super(new Frame(""), title, true);
        this.parentFrame = parent;
        this.serverHost = host;
        this.serverPort = port;
        this.imgExpanded  = expandedImg;
        this.imgFile = fileImg;
        this.imgCollapsed = collapsedImg;
        this.filename = null;

        WindowListener adapter = new WindowAdapter()
        {
            public void windowClosing(WindowEvent e)
            {
                e.getWindow().dispose();
                parentFrame.toFront();
            }
        };
        this.addWindowListener(adapter);
    }

    /** initialize the tree */
    public void init(String path)
    {
        setSize(420, 260);
        setBackground(java.awt.Color.white);
        setResizable(false);

        tree = new HDFTree();
        tree.setBackground(java.awt.Color.white);

        FileTreeNode root = new FileTreeNode(this, path, imgCollapsed, imgExpanded);
        root.setLevel(0);
        getFiles(root, path);
        addTreeNode(tree, root);
        add(new java.awt.Label(""));
        add(tree);

        root.setExpanded();
        tree.refresh();

        // no file is set
        if (filename == null) show();
    }

    /** printout the tree for degub purpose */
    public void printTree(FileTreeNode node, String indent)
    {
        System.out.println(indent+node.toString());
        if (!node.isLeaf())
        {
            Vector children = node.getChildren();
            int size = children.size();
            FileTreeNode theNode = null;
            for (int i=0; i<size; i++)
            {
                theNode = (FileTreeNode) children.elementAt(i);
                printTree(theNode, indent+"    ");
            }
        }
    }


    /**
     *    Gets the selected file of this file dialog. 
     */
    public String getFile()
    {
        return filename;
    }

    /**
     *    return the parent frame of the dialog. 
     */
    public Frame getParentFrame()
    {
        return parentFrame;
    }

    /**
     *  Sets the selected file
     *
     *  @param file  the file being set. 
     */
    public synchronized void setFile(String file)
    {
        this.filename = file;
    }

    /**
     *  add a node and all the subnodes to the tree
     *
     *  @param tree    the HDFTree
     *  @param node    The FileTreeNode
     */
    public void addTreeNode (HDFTree tree, FileTreeNode node)
    {
        if (node.isRoot())
            tree.addTreeNode(node);
        else
            tree.addTreeNode(node.getParent(), node);

        if (!node.isLeaf())
        {
            Vector children = node.getChildren();
            int size = children.size();
            for (int i=0; i< size; i++)
                addTreeNode(tree, ((FileTreeNode) children.elementAt(i)));
        }
    }

    public void getFiles(FileTreeNode parent, String path)
    {
        ObjectInputStream  input = null;
        ObjectOutputStream output = null;
        Socket server = null;
        HDFMessage message = null;
        HDFFileList fileList = new HDFFileList (null, path);

        // get file list from the remote machine
        try {
            // Web server
            if (serverPort < 1 )
            {
                // check authentication for the given url path
                /*********************************************
                int index1 = serverHost.indexOf(":",8);
                String tmp = serverHost.substring(index1);
                int index2 = tmp.indexOf("/");
                tmp = tmp.substring(0, index2);
                String serverBase = serverHost.substring(0, index1)+tmp;
                AuthChecker checker = new AuthChecker((Frame)getParent(), serverBase+"/"+path);
                if (!checker.isAuthorized()) return;
                *********************************************/

                // make connection and get data
                URL url = new URL(serverHost);
                HttpURLConnection theConnection = (HttpURLConnection) url.openConnection();
                theConnection.setDoOutput(true);
               theConnection.setRequestProperty("Content-Type", "application/octet-stream");
                output = new ObjectOutputStream(theConnection.getOutputStream());
                output.writeObject(fileList.toServer());
                output.close();
                input = new ObjectInputStream (theConnection.getInputStream());
                message = (HDFMessage) input.readObject();
                input.close();
           }
            else {
                server = new Socket(serverHost, serverPort);
                output = new ObjectOutputStream (server.getOutputStream());
                output.writeObject(fileList.toServer());
                input = new ObjectInputStream (server.getInputStream());
                message = (HDFMessage) input.readObject();
                output.close();
                input.close();
                server.close();
            }

            if (message == null) return;
            fileList.fromServer(message);
            // do not need the file dialog if the path is a file
            if (fileList.isFile())
            {
                setFile(path);
                return;
            }

            String[] filenames = fileList.getFileList();
            FileTreeNode child = null;
            String afilename = null;
            for (int i = 0; i < filenames.length; i++)
            {
                afilename = filenames[i];
                if (afilename.charAt(0) == 'd')
                    child = new FileTreeNode(this, afilename.substring(1), imgCollapsed, imgExpanded);
                else
                    child = new FileTreeNode(this, afilename.substring(1), imgFile);
                parent.add(child);
                if (!parent.isRoot()) tree.addTreeNode(parent, child);
            }
        }
        catch (Exception e)
        {
            setFile("");
        }
    }

}

class FileTreeNode extends TreeNode
{
    /** the parent of the node */
    FileTreeNode parent;

    /** the children of the node */
    Vector children;

    /** the RemoteFileDialog */
    RemoteFileDialog fileDialog;

    /**
     *  Constructor with a given node object, collapsed image,
     *  expanded image, and a parent
     *
     *  @param obj        the HDF object node
     *  @param collapsed  the collapsed folder image
     *  @param parent     The parent of the node
     */
    public FileTreeNode(RemoteFileDialog dialog, FileTreeNode parent,
        Object obj, Image collapsed ,Image expanded)
    {
        super(obj, collapsed, expanded);
        this.parent = parent;
        children = new Vector ();
        fileDialog = dialog;
    }

    public FileTreeNode(RemoteFileDialog dialog, Object obj ,Image collapsed ,Image expanded)
    {
        this(dialog, null, obj, collapsed, expanded);
    }

    public FileTreeNode(RemoteFileDialog dialog, Object obj ,Image collapsed)
    {
        this(dialog, null, obj, collapsed, null);
    }
 
    public FileTreeNode(RemoteFileDialog dialog, FileTreeNode parent,
        Object obj ,Image collapsed)
    {
        this(dialog, parent, obj, collapsed, null);
    }

    public boolean isLeaf () { return (children.size() == 0); }
    public boolean isRoot () { return (parent == null); }
    public FileTreeNode getParent() { return parent; }
    public Vector getChildren() { return children; }

    public FileTreeNode getRoot ()
    {
        if (this.isRoot ()) return this;
        else return parent.getRoot ();
    }

    public void add (FileTreeNode child)
    {
        children.addElement (child);
        child.parent = this;
        child.setLevel(getLevel()+1);
    }

    public void add (Object obj ,Image collapsed ,Image expanded)
    {
        add (new FileTreeNode (fileDialog, obj, collapsed, expanded));
    }

    public void add (Object obj ,Image collapsed)
    {
        add (new FileTreeNode (fileDialog, obj, collapsed));
    }

    public void printPath ()
    {
        System.out.print (this);
        if (parent != null)
        {
            System.out.print ("/");
            parent.printPath();
        }
    }

    /**
     *  Returns the path, from the root, to get to this node
     */
    public FileTreeNode[] getPath()
    {
        Vector vector = new Vector();
        vector.addElement(this);

        FileTreeNode node = this;
        while (!node.isRoot())
        {
            node = node.parent;
            vector.addElement(node);
        }

        int size = vector.size();
        FileTreeNode list[] = new FileTreeNode[size];
        FileTreeNode theNode = null;
        for (int i=0; i<size; i++)
        {
            theNode = (FileTreeNode) vector.elementAt(size-1-i);
            if (theNode == null) return null;
            else list[i] = theNode;
        }

        return list;   
    }

    /**
     *    return the full file path of the selected node
     */
    public String getFilePath()
    {
        String fullPath = "";
        FileTreeNode node = this;

        while (!node.isRoot())
        {
            fullPath  = java.io.File.separator+node.toString()+fullPath;
            node = node.parent;
        }
        fullPath  = java.io.File.separator+node.toString()+fullPath;

        return fullPath;   
    }
 
    /**
     *   Overwrite the TreeNode
     */
    public void select(HDFTree tree)
    {
        if (fileDialog != null)
            fileDialog.setFile(getFilePath());
        fileDialog.dispose();
        fileDialog.getParentFrame().toFront();
    }

    /**
     *   Overwrite the TreeNode
     */
    public  void  expandCollapse(HDFTree objTree)
    {
        if (isExpandable())
            toggleExpanded();
        if (isLeaf())
            fileDialog.getFiles(this, getFilePath());
   }

}
