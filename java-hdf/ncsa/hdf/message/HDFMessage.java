package ncsa.hdf.message;

import java.util.Hashtable;

/**
 *  HDFMessage holds information to be transfered between the server and client.
 
 *  @version   1.1.3 September 2 1997
 *  @author    Peter X. Cao (xcao@ncsa.uiuc.edu)
 */
public class HDFMessage extends Hashtable
{
    /**
     *  the owner's name of the HDFMessage.
     *  the owner recognizes the HDFMessage and knows how to process the message
     */
    private String owner;

    /**
     *  constructs an HDFMessage without an owner.
     */
    public HDFMessage() { this(""); }

    /**
     *  constructs an HDFMessage with a specified owner.
     */
    public HDFMessage(String owner)
    {
        this.owner = owner;
    }

    /**
     *  get the owner of this message
     */
    public String getOwner() { return owner; }

    /**
     *  set the owner of the message
     */
    public void setOwner(String newOwner) { this.owner = newOwner; }

    /**
     * Returns the String representation of this message value.
     */
    public String toString()
    {
        return getClass().getName() + "\nowner = "+owner + "\ncontent = "+
        super.toString();
    }

}
