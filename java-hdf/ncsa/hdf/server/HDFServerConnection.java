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
 *  HDFServerConnection makes a Client-Server connection for each
 *  incoming request and serves the request. Once the service is 
 *  done, it closes the connection.
 */
public class HDFServerConnection extends Thread
{
    /** the socket serving the request */
    protected Socket theSocket;

    /** The document root directory for serving files. */
    protected String documentDir;

    /*
     * Creates an new HDFServerConnection with specified socket and document
     * root directory.
     *
     * @param s       The socket to server the request
     * @param docRoot The server document root directory
     */
    public HDFServerConnection(Socket s, String docRoot)
    {
        theSocket = s;
        documentDir = docRoot;
    }

    /*
     * Overwrites Thread.
     *
     *  process the request and close the socket
     */
    public void run()
    {
        serveRequest(theSocket);
        try { theSocket.close(); } catch (Exception e) {;}
        stop();
        System.gc(); // clean up used memory
    }

    /**
     *  This is where the server actually does its work. It may serve more than
     *  one requests at one connection.
     *
     *  @param socket   the socket for receiving message from the client and
     *                   sending message to the client
     */
    private void serveRequest(Socket socket)
    {
        ObjectInputStream input = null;
        ObjectOutputStream output;
        HDFObject hdfObject;
        HDFMessage message = null;

        try
        {
            input = new ObjectInputStream (theSocket.getInputStream());
            output= new ObjectOutputStream (theSocket.getOutputStream());
            while ( (message = (HDFMessage) input.readObject()) != null)
            {
                if      ( (hdfObject = new HDFFileList()).isMe(message) );
                else if ( (hdfObject = new HDFHierarchy()).isMe(message) );
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
        }
        catch(Exception e) {}
    }

}
