package ncsa.hdf.message;

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
        HDFAnalyse analyseHdf   = new HDFAnalyse();
        analyseHdf.getHdfObject(hdfFilename, nodeQueue);

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
            switch(node.type)
            {
                case HDFObjectNode.RIS8:
                    try { information = HDFRIS8.readInfo (hdf, hdfFilename, node); }
                    catch (Exception e) {}
                    break;
                case HDFObjectNode.RIS24:
                    try { information = HDFRIS24.readInfo (hdf, hdfFilename, node); }
                    catch (Exception e) {}
                    break;
                case HDFObjectNode.SDS:
                case HDFObjectNode.SDSDATASET:
                    try { information = HDFSDS.readInfo (hdf, hdfFilename, node); }
                    catch (Exception e) {}
                    break;
                case HDFObjectNode.GR:
                case HDFObjectNode.GRDATASET:
                    try { information = HDFGR.readInfo (hdf, hdfFilename, node); }
                    catch (Exception e) {}
                    break;
                case HDFObjectNode.Vdata:
                    try { information = HDFVdata.readInfo (hdf, hdfFilename, node); }
                    catch (Exception e) {}
                    break;
                case  HDFObjectNode.Vgroup:
                    try { information = HDFVgroup.readInfo (hdf, hdfFilename, node); }
                    catch (Exception e) {}
                    break;
            }
            node.information = information;

            // no information for leaf nodes
            if (node.child != null)
            {
                subQueue = (Queue) (node.child);
                addNodeInformation(subQueue);
            }
        } 
    }  

}
