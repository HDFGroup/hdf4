package ncsa.hdf.message;

/**
 *  Messagable is an interface to be implemented by HDF objects.
 *  This interface defines a protocol for bidirectional communication
 *  between HDF java server and HDF java client.
 
 *  @version   1.1.3 September 2 1997
 *  @author    Peter X. Cao
 */
public interface Messageable
{
    /** check if the object is me */
    public boolean isMe(HDFMessage message);

    /** create a message for sending to the server */
    public HDFMessage toServer();

    /** create a message for sending to the client */
    public HDFMessage toClient();

    /** process the message received from the server */
    public void fromServer(HDFMessage message);

    /** process the message received from the client */
    public void fromClient(HDFMessage message);

    /** serve the client request on the server */
    public void service();

}
