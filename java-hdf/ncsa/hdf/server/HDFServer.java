/****************************************************************************
 * NCSA HDF                                                                 *
 * National Comptational Science Alliance                                   *
 * University of Illinois at Urbana-Champaign                               *
 * 605 E. Springfield, Champaign IL 61820                                   *
 *                                                                          *
 * hdf/COPYING file.                                                        *
 *                                                                          *
 ****************************************************************************/

package ncsa.hdf.server;

import java.net.Socket;
import java.net.ServerSocket;
import java.io.File;
import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.io.PrintWriter;
import javax.servlet.ServletConfig;
import javax.servlet.ServletException;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import javax.servlet.ServletOutputStream;
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
 *  DHFServer sets up a server with the specified port and document root. The
 *  server can be a stand-alone java application at whitch the main() function
 *  will be called to start the server thread. All HDF requests will be handled
 *  in class HDFServerConnection. The server can be also an HttpServlet, in which
 *  case, the init() method will be called to setup the server document root
 *  when the servlet is invoked by the first time. All the HDF requests will be
 *  handled in the service() method of the servlet.
 *

 *  @see       ncsa.hdf.server.FileServlet
 *  @version   1.1.3 September 8 1997
 *  @author    Peter X. Cao (xcao@ncsa.uiuc.edu)
 */
public class HDFServer extends FileServlet implements Runnable
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
    public HDFServer () {}

    /**
     *  Creates a new HDFServer with specified document root directory and
     *  local port number which the ServerSocket is bonded on
     *
     *  @param serverPort the server port number
     *  @param docRoot    the server document root directory
     */
    public HDFServer (int serverPort, String docRoot)
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
     *  overwrite HttpServlet.
     *  sets up the ServletConfig and HDFServer document root directory
     */
    public void init(ServletConfig conf) throws ServletException
    {
        //System.out.println("HDFServer.init()");

        super.init(conf);

        // set document root
        File rootPath = new File(getServletContext().getRealPath(""));
        documentDir = rootPath.getAbsolutePath();
    }

    /**
     *  overwrites FileServlet.
     */
    public void service( HttpServletRequest req, HttpServletResponse res )
        throws ServletException, IOException
    {
        String method = req.getMethod();
        if (method.equals("POST"))
            doPost(req, res);
        else if (method.equals("GET"))
            doGet(req, res);
        else
            super.service(req, res);
    }

    /**
     *  sends back an error message because HDF files can not be viewed
     *  from a regular browser for the current version of HDFServer.
     *
     *  @param req  encapsulates the request to the servlet
     *  @param resp encapsulates the response from the servlet
     */
    public void doGet( HttpServletRequest req, HttpServletResponse res )
        throws ServletException, IOException
    {
        ServletOutputStream out = res.getOutputStream();
        PrintWriter pw = new PrintWriter(out);

        // set content type and other response header fields first
        res.setContentType("text/html");

       // then write the data of the response
        pw.println("<HEAD><TITLE> HDFServer Error Message </TITLE></HEAD><BODY>");
        pw.println("<h1><font size=4 color=#FF1122> HDFServer Error Message </font></h1>");
        pw.println("<p>HDF files can not be viewed from this browser.");
        pw.println("<p>You need an HDF viewer to see the HDF files.");
        pw.println("</BODY>");
        pw.flush();
        out.close();
    }

    /**
     *  takes an HDF request from the received HDFMessage and sends back an HDF
     *  response by an HDFMessage through ObjectOutputStream
     *
     *  @param req  encapsulates the request to the servlet
     *  @param resp encapsulates the response from the servlet
     */
    public void doPost( HttpServletRequest req, HttpServletResponse res )
        throws ServletException, IOException
    {
        // read the HDFMessage from the ObjectInputStream
        ObjectInputStream  input = new ObjectInputStream(req.getInputStream());
        HDFMessage message = null;
        try { message = (HDFMessage)input.readObject(); }
        catch (Exception e){}
        input.close();

        // process the HDFMessage and write it to the ObjectOutputStream
        HDFObject hdfObject = null;
        ObjectOutputStream output = new ObjectOutputStream(res.getOutputStream());
        res.setContentType("application/octet-stream");
        if (message != null)
        {
            if      ( (hdfObject = new HDFFileList()).isMe(message) );
            else if ( (hdfObject = new HDFHierarchy()).isMe(message));
            else if ( (hdfObject = new HDFAnnotation()).isMe(message) );
            else if ( (hdfObject = new HDFRIS8()).isMe(message) );
            else if ( (hdfObject = new HDFRIS24()).isMe(message) );
            else if ( (hdfObject = new HDFGR()).isMe(message) );
            else if ( (hdfObject = new HDFSDS()).isMe(message) );
            else if ( (hdfObject = new HDFVdata()).isMe(message) );
            else if ( (hdfObject = new HDFVgroup()).isMe(message) );
            else
            {
                try { hdfObject = (HDFObject) (Class.forName(message.getOwner())).newInstance(); }
                catch (Exception e) {;}
            }
            hdfObject.fromClient(message, documentDir);
            message = hdfObject.toClient();
            output.writeObject (message);
        }
        output.close();
        //System.gc(); // clean up used memory
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
        if (HDFServer.debug) System.out.println("Server   port: " + serverPort);
        if (HDFServer.debug) System.out.println("Document root: " + documentDir);
        new HDFServer (serverPort, documentDir);
    }
}


