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

import java.util.Vector;
import java.io.File;

import ncsa.hdf.util.Tools;

/**
 *  HDFFileList holds the list of hdf files
 
 *  @version   1.1.3 September 22 1997
 *  @author    Peter X. Cao (xcao@ncsa.uiuc.edu)
 */
public class HDFFileList extends HDFObject
{
    /** the vector containing the file list */
    private String[] fileNames;

    /** indicates if the current file is a file or directory */
    private String fileFlag;

    /**
     *  Constructs an HDFFileList with default parameters
     */
    public HDFFileList ()
    {
        super();
        fileNames = null;
        fileFlag = "D";
    }

    /**
     *  Construct an HDFFileLis with specified HDFObjectNode and filename
     *
     *  @param node     the HDFObjectNode to be transfered to the server
     *  @param filename the requested HDF file
     */
    public HDFFileList (HDFObjectNode node, String filename)
    {
        super(node, filename);
        fileNames = null;
        fileFlag = "D";
    }

    /**
     *  create a message for sending to the client
     *
     *  @return  The HDFMessage created by this object
     */
    public HDFMessage toClient()
    {
        HDFMessage message      = new HDFMessage(getClass().getName());
        if (fileNames != null) message.put("fileNames", fileNames);
        if (fileFlag != null) message.put("fileFlag", fileFlag);
        return message;
    }

    /**
     *  serve the client request on the server
     */
    public void service()
    {
        //System.out.println("HDFFileList.service()");
        fileNames = getFileList(hdfFilename);
    }

    /**
     *  process the message received from the server
     *
     *  @param  message  The message received from the server
     */
    public void fromServer(HDFMessage message)
    {
        //System.out.println("HDFFileList.fromServer()");
        fileNames = (String[]) message.get("fileNames");
        fileFlag = (String) message.get("fileFlag");
    }

    /**
     *  get the list of HDF files and directories for a given path
     *
     *  @param   filename  the HDF filename
     *  @return  The vector containing the file list
     */
    private String[] getFileList (String filename)
    {
        String[] names = null;
        File file = new File(filename);
        if ( file.isDirectory() )
        {
            names = file.list();
            // sort the file list
            for (int i=0; i<names.length; i++)
            {
                if ((new File(filename+File.separatorChar+names[i])).isDirectory())
                    names[i] = "d"+names[i];
                else
                    names[i] = "f"+names[i];
            }
            Tools.sort(names);
        }
        else this.fileFlag = "F";

        return names;
    }
    
    /**
     *  return the vector containing the file list
     */
    public String[] getFileList () { return fileNames; }

    /** check if the requested file is a file or directory */
    public boolean isFile () { return (!fileFlag.equals("D")); }

}
