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
 *  HDFAnnotation holds HDF annotation information

 *  @version   1.1.3 September 22 1997
 *  @author    Peter X. Cao  (xcao@ncsa.uiuc.edu)
 */
public class HDFAnnotation extends HDFObject
{
    /** the string containing the annotation information */
    private String annotation;

    /**
     *  Constructs an HDFAnnotation with default parameters
     */
    public HDFAnnotation ()
    {
        super();
        annotation = "";
    }

    /**
     *  Construct an HDFAnnotation with specified HDFObjectNode and filename
     *
     *  @param node     the HDFObjectNode to be transfered to the server
     *  @param filename the requested HDF file
     */
    public HDFAnnotation (HDFObjectNode node, String filename)
    {
        super(node, filename);
        annotation = "";
    }

    /**
     *  create a message for sending to the client
     *
     *  @return  The HDFMessage created by this object
     */
    public HDFMessage toClient()
    {
        HDFMessage message = new HDFMessage(getClass().getName());

        message.put("annotation", annotation);
        return message;
    }

    /**
     *  serve the client request on the server
     */
    public void service()
    {
        this.hdf =  new HDFLibrary();
        try { annotation = getAnnotation (filename, nodeObject); }
        catch (Exception e) { annotation = "ERROR: exception in HDFAnnotation.getAnnotation()"; }
    }

    /**
     *  process the message received from the server
     *
     *  @param message  The HDFMessage from the server
     */
    public void fromServer(HDFMessage message)
    {
        annotation = (String) message.get("annotation");
    }

    /**
     *  get HDF file annotation information for the selected HDF file
     *
     *  @param filename   the string of the hdf file name
     *  @param node       the HDFObjectNode
     *  @return           the string containing the annotation information
     *
     *  @exception ncsa.hdf.hdflib.HDFException 
     *             should be thrown for errors in the
     *             HDF library call, but is not yet implemented.
     *
     */
    private String getAnnotation (String filename, HDFObjectNode node)
        throws HDFException
    {
        int        request     = node.type;
        int        ref         = node.ref;
        int        attrIndex   = node.index;
        int        fid         = -1;
        int        anid        = -1;
        int[]      fileInfo    = new int[4];
        String     annotation  = new String ("");
 
        if ((fid = hdf.Hopen(filename)) <= 0)
        {
            return ("ERROR: Open " + filename + " failed.");
        }
 
        anid = hdf.ANstart(fid);
        if (anid == HDFConstants.FAIL)
        {
            hdf.ANend(anid);
            hdf.Hclose(fid);
            return ("ERROR: native call to HDFLibrary.ANstart() failed.");
	}
 
        if ( !hdf.ANfileinfo(anid, fileInfo) )
        {
            hdf.ANend(anid);
            hdf.Hclose(fid);
            return ("ERROR: native call to HDFLibrary.ANfileinfo() failed.");
	}
    
        int numFileLabel = fileInfo[0];
        int numFileDesc  = fileInfo[1];
        int numFileAnn   = Math.max(numFileLabel, numFileDesc);

        for (int kk=0; kk < numFileAnn; kk++)
        {
            // get ann_id;
            int annid = hdf.ANselect(anid, kk, HDFConstants.AN_FILE_LABEL);
            if (annid != HDFConstants.FAIL)
            {
                annotation = annotation.concat("File Label #");
                annotation = annotation.concat(Integer.toString(kk));
                annotation = annotation.concat("\n\n");
                  
                // get the file label length
                int fileLabelLen = hdf.ANannlen(annid)+1;
                String fileLabelBuf = new String(" ");
                String s[] = new String[1];
                s[0] = fileLabelBuf;

                if (fileLabelLen >0)
                {
                    // get file label
                    if (hdf.ANreadann(annid, s, fileLabelLen))
                    { 
                        annotation = annotation.concat(s[0]);
                    }
                }
            }

            hdf.ANendaccess(annid);
            annid = hdf.ANselect(anid, kk, HDFConstants.AN_FILE_DESC);
            if (annid != HDFConstants.FAIL)
            {
                annotation = annotation.concat("\nFile Desc. #");
                annotation = annotation.concat(Integer.toString(kk));
                annotation = annotation.concat("\n\n");

                // get the file desc. length
                int fileDescLen = hdf.ANannlen(annid)+1;

                if (fileDescLen >0)
                {
                    String fileDescBuf = new String(" ");
                    String s[] = new String[1];
                    s[0] = fileDescBuf;

                    // get file desc.
                    if (hdf.ANreadann(annid, s, fileDescLen))
                    {
                        annotation = annotation.concat(s[0]);
                    }
                }
            }
            hdf.ANendaccess(annid);

        }

        hdf.ANend(anid);
        hdf.Hclose(fid);
        return annotation;
    }

    /**
     *  return the  annotation
     */
    public String getAnnotation ()
    {
        return annotation;
    }
    
}
