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

import ncsa.hdf.hdflib.HDFException;
import ncsa.hdf.hdflib.HDFLibrary;
import ncsa.hdf.hdflib.HDFConstants;

/**
 *  HDFSDS holds HDF Vgroup information
 
 *  @version   1.1.3 September 22 1997
 *  @author    Peter X. Cao
 */
public class HDFVgroup extends HDFObject
{
    /**
     *  Constructs an HDFVgroup with default parameters
     */
    public HDFVgroup ()
    {
        super();
    }

    /**
     *  Construct an HDFVgroup with specified HDFObjectNode and filename
     *
     *  @param node     the HDFObjectNode to be transfered to the server
     *  @param filename the requested HDF file
     */
    public HDFVgroup (HDFObjectNode node, String filename)
    {
        super(node, filename);
        information = "";
    }

    /**
     *  create a message for sending to the client
     *
     *  @return  the HDFMessage created by this object
     */
    public HDFMessage toClient()
    {
        HDFMessage message = new HDFMessage(getClass().getName());

        message.put("information", information);
        return message;
    }

    /**
     *  serve the client request on the server
     */
    public void service()
    {
        this.hdf =  new HDFLibrary();
        try { information = readInfo(hdf, filename,nodeObject); }
        catch (Exception ex)
        {
            information = "ERROR in HDFGroup.service(): " + ex.toString();
        }
    }

    /**
     *  process the message received from the server
     *
     *  @param message the HDFMessage received from the server
     */
    public void fromServer(HDFMessage message)
    {
        information = (String) message.get("information");
    }

    /**
     *  get HDF Vgroup information for the selected HDF file
     *
     *  @param filename   the string of the hdf file name
     *  @param hdf        the HDFLibrary
     *  @param node       the HDFObjectNode
     *  @return           the string containing the Vgroup information
     *
     *  @exception ncsa.hdf.hdflib.HDFException 
     *             should be thrown for errors in the
     *             HDF library call, but is not yet implemented.
     *
     */
    public static String readInfo (HDFLibrary hdf, String filename,
        HDFObjectNode node) throws HDFException
    {
        int fid = -1;
        int ref = node.ref;
        String info = new String();

        if ((fid = hdf.Hopen(filename)) <= 0)
        {
            return ("ERROR: Open " + filename + " failed.");
        }

        hdf.Vstart(fid);

        // start search          
        int vgroup_ref = -1;          
    
        // for Vgroup
        int [] n = new int[1];
        int vgNumber = hdf.Vlone(fid,n,0);

        if ( ref == -1 )
        {
            // Vgroup root ...
            if ( vgNumber > 0 )
            {
                int refArray[] = new int[vgNumber];
                int refNum;
   
                // for child of the Vgroup root
                if  ( (refNum = hdf.Vlone(fid, refArray, vgNumber)) > 0 )
                {
                    // each of vgroup (lonely)
                    for (int i=0; i<refNum; i++)
                    {
                        // reference .
                        vgroup_ref = refArray[i];
                        int vgtag = HDFConstants.DFTAG_VG;
                        int vg_id = hdf.Vattach(fid, vgroup_ref, "r"); 
                        String className = new String("");
                        String s[] = new String[1];
                        s[0] = className;
                        hdf.Vgetclass(vg_id,s);           
                        className = s[0];
                        String name = new String("");
                        s[0] = name;
                        hdf.Vgetname (vg_id,s);
                        name = s[0];
           
                        if ( className.indexOf("CDF0.0") == -1 )
                        { 
                            // get total numbers of tag/ref pairs id 
                            int npairs = hdf.Vntagrefs(vg_id);
                            info = info.concat("\nTag number       : " + vgtag);
                            info = info.concat("\nReference number : " + vgroup_ref);
                            info = info.concat("\nName             : " + name);
                            info = info.concat("\nClass Name       : " + className);
                            info = info.concat("\nEntries          : " + npairs);
                            hdf.Vdetach(vg_id);
                        } 
                    } 
                } 
            }
        }  
        else
        {
            // reference .
            vgroup_ref = ref;
            int vgtag = HDFConstants.DFTAG_VG;
            int vg_id = hdf.Vattach(fid, vgroup_ref, "r"); 
            String className = new String("");
            String s[] = new String[1];
            s[0] = className;
            hdf.Vgetclass(vg_id,s);           
            className = s[0];
            String name = new String("");
            s[0] = name;
            hdf.Vgetname (vg_id,s);
            name = s[0];
           
            // get total numbers of tag/ref pairs id 
            int npairs = hdf.Vntagrefs(vg_id);
            info = info.concat("\nTag number       : " + vgtag);
            info = info.concat("\nReference number : " + vgroup_ref);
            info = info.concat("\nName             : " + name);
            info = info.concat("\nClass Name       : " + className);
            info = info.concat("\nEntries          : " + npairs);
             
            // terminate this vgroup
            hdf.Vdetach(vg_id);
        }

        hdf.Vend(fid);
        hdf.Hclose(fid);
        return (info);
    }

}
