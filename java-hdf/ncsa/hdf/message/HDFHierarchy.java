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

package ncsa.hdf.message;

import ncsa.hdf.hdflib.HDFLibrary;
import ncsa.hdf.util.Queue;

/**
 *  HDFHierarchy holds HDF Hierarchical tree information
 
 *  @version   1.1.3 September 22 1997
 *  @author    Peter X. Cao
 */
public class HDFHierarchy extends HDFObject
{
    /** the Queue containing the node information */
    private Queue nodeQueue;

    /**
     *  Constructs an HDFHierarchy with default parameters
     */
    public HDFHierarchy ()
    {
        super();
        nodeQueue = new Queue();
    }

    /**
     *  Construct an HDFHierarchy with specified HDFObjectNode and filename
     *
     *  @param node     the HDFObjectNode to be transfered to the server
     *  @param filename the requested HDF file
     */
    public HDFHierarchy (HDFObjectNode node, String filename)
    {
        super(node, filename);
        nodeQueue = new Queue();
    }

    /**
     *  create a message for sending to the client
     *
     *  @return  The HDFMessage created by this object
     */
    public HDFMessage toClient()
    {
        HDFMessage message = new HDFMessage(getClass().getName());

        message.put("nodeQueue", nodeQueue);
        return message;
    }

    /**
     *  serve the client request on the server
     */
    public void service()
    {
        this.hdf =  new HDFLibrary();
        HDFAnalyse analyseHdf   = new HDFAnalyse();
        analyseHdf.getHdfObject(filename, nodeQueue);
        addNodeInformation(nodeQueue);
    }

    /**
     *  process a message receiving from the server
     *
     *  @param message  the HDFMessage received from the server
     */
    public void fromServer(HDFMessage message)
    {
        nodeQueue = (Queue) message.get("nodeQueue");
    }

    /** return the Queue containing the HDF hierarchy information */
    public Queue getQueue() { return nodeQueue; }

    /**
     *  add the text information into the hierarchy tree nodes
     *
     *  @param hdfQueue  the HDF object node Queue
     */
    private void addNodeInformation(Queue hdfQueue)
    {
        HDFObjectNode node        = null;
        Queue         subQueue    = null;
        String        information = "";
        
        // empty queue
        if (hdfQueue.size() == 0) return;

        for (int i=0; i<hdfQueue.size(); i++)
        {
            // get one node from the Queue
            node = (HDFObjectNode)hdfQueue.elementAt(i);

            if (!node.hasChild()) continue;

            switch(node.type)
            {
                case HDFObjectNode.RIS8:
                    try { information = HDFRIS8.readInfo (hdf, filename, node); }
                    catch (Exception e) {}
                    break;
                case HDFObjectNode.RIS24:
                    try { information = HDFRIS24.readInfo (hdf, filename, node); }
                    catch (Exception e) {}
                    break;
                case HDFObjectNode.SDS:
                case HDFObjectNode.SDSDATASET:
                    try { information = HDFSDS.readInfo (hdf, filename, node); }
                    catch (Exception e) {}
                    break;
                case HDFObjectNode.GR:
                case HDFObjectNode.GRDATASET:
                    try { information = HDFGR.readInfo (hdf, filename, node); }
                    catch (Exception e) {}
                    break;
                //case HDFObjectNode.Vdata:
                //    try { information = HDFVdata.readInfo (hdf, filename, node); }
                //    catch (Exception e) {}
                //    break;
                case  HDFObjectNode.Vgroup:
                    try { information = HDFVgroup.readInfo (hdf, filename, node); }
                    catch (Exception e) {}
                    break;
            }
            node.information = information;

            subQueue = (Queue) (node.child);
            addNodeInformation(subQueue);
        }
    }  

}
