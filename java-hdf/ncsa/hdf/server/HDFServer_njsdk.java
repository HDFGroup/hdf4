package ncsa.hdf.server;

import java.net.Socket;
import java.net.ServerSocket;
import java.io.File;
import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.io.PrintWriter;
import ncsa.hdf.message.HDFObject;
import ncsa.hdf.message.HDFMessage;
import ncsa.hdf.message.HDFFileList;
import ncsa.hdf.message.HDFHierarchy;
import ncsa.hdf.message.HDFAnnotation;
import ncsa.hdf.message.HDFRIS8;
import ncsa.hdf.message.HDFRIS24;
import ncsa.hdf.message.HDFGR;
import ncsa.hdf.message.HDFSDS;
import ncsa.hdf.message.HDFVdata;
import ncsa.hdf.message.HDFVgroup;

/**
 *  DHFServer_njsdk sets up a server with the specified port and document root.
 *  The server is a stand-alone java application at whitch the main() function
 *  will be called to start the server thread. All HDF requests will be handled
 *  in class HDFServerConnection.
 *

 *  @version   1.1.3 September 8 1997
 *  @author    Peter X. Cao (xcao@ncsa.uiuc.edu)
 */
public class HDFServer_njsdk implements Runnable
{
    /** default HDFServer port number */
    private static final int    DEFAULT_PORT = 8181;

    /** default HDFServer document root directory */
    private static final String DEFAULT_ROOT = ".";

    /** debug flag */
    public static boolean debug = false;

    /** The document root directory for serving files. */
    protected String documentDir;

    /** the HDFServer port number */
    protected int port;

    /** the HDFServer main ServerSocket */
    protected ServerSocket mainSocket;

    /** the main server thread */
    protected Thread serverThread;

    /** Creates a new HDFServer without a ServerSocket */
    public HDFServer_njsdk () {}

    /**
     *  Creates a new HDFServer with specified document root directory and
     *  local port number which the ServerSocket is bonded on
     *
     *  @param serverPort the server port number
     *  @param docRoot    the server document root directory
     */
    public HDFServer_njsdk (int serverPort, String docRoot)
    {
        mainSocket = null;
        documentDir = docRoot;
        port = serverPort;

        try { mainSocket = new ServerSocket(port); }
        catch(Exception e) 
        {
           System.err.println(e);
           System.exit(1);
        }
        serverThread = new Thread(this);
        serverThread.start();
    }

    /**
     *  overwirte Thread. 
     *  Sets up a ServerSocket and listen to incoming request.
     *  spawns an HDFServerConnection to deal with the request.
     */
    public void run()
    {
        HDFServerConnection serverConnection = null;
        while (true)
        {
            try
            {
                Socket theConnection = mainSocket.accept();
                serverConnection = new HDFServerConnection(theConnection, documentDir);
                serverConnection.start();
            }
            catch(Exception e) { System.err.println("Exception:\n" + e); }
        }
    }

    /**
     *  return the document root of the server
     *
     *  @return the server document root
     */
    public String getDocRoot()
    {
        return documentDir;
    }

    /**
     *  The main() function sets up and runs the HDFServer.
     *
     *  @param args[]  the argument list (HDFServer options).
     *  <pre>
     *       -h               print out the help information.
     *       -debug           set the debug on.
     *       -p  port_number  binds the server to port_number.
     *       -d  doc_root     sets the document root directory to doc_root.
     *  </pre>
     */
    public static void main(String args[])
    {
        int serverPort = DEFAULT_PORT;
        String documentDir = DEFAULT_ROOT;
        
        for (int i = 0; i < args.length; i++)
        { 
            if ("-d".equalsIgnoreCase(args[i]))
            {
                try { if ( (new File (args[i+1])).exists() ) documentDir = args[i+1]; }
                catch (Exception e) {;}
            }
            else if ("-p".equalsIgnoreCase(args[i]))
            {
                try { serverPort = Integer.parseInt(args[i+1],10); }
                catch (Exception e) {;}
            }
            else if ("-debug".equalsIgnoreCase(args[i]))
            {
                debug = true;
            }
            else if ("-h".equalsIgnoreCase(args[i]))
            {
                System.out.println("    -h             print out this message");
                System.out.println("    -p             set the server port (8181 by default)");
                System.out.println("    -d             set the document root directory (. by default)");
                System.out.println("    -debug         set the debug on (false by default)");
                System.exit(0);
            }
        }
        try
        {
            File documentDirFile = new File (documentDir);
            documentDir = documentDirFile.getAbsolutePath();
            if ( !documentDirFile.canRead() )
            {
                System.out.println(documentDir +"is not readable");
                System.exit(1);
            }
        }
        catch (Exception e) { System.err.println(e); System.exit(1); }
        if (HDFServer_njsdk.debug) System.out.println("Server   port: " + serverPort);
        if (HDFServer_njsdk.debug) System.out.println("Document root: " + documentDir);
        new HDFServer_njsdk (serverPort, documentDir);
    }
}

