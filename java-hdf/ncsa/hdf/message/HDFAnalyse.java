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

import ncsa.hdf.util.Queue;
import ncsa.hdf.hdflib.*;

/* This class will analyse the HDF file and put the HDF object into 
 * the Queue. Hdf object is the node created by HDFObjectNode class
 * Date: 3-12-97 Modified "getSDSObject()".
 *    Keep the name of an HDF object node to be more readable,
 *    to be as the form look like "sdsname: long_name".
 * @version 2.00  9/3/97
 * @auther  Xinjian Lu, modified by Peter Cao on 4-14-98
 */
public class HDFAnalyse
{
    // HDF Object
    public HDFLibrary hdf;
    int fid = -1;
    String fileName = "";

    /** analyse the HDF file then put it into Queue
     * @param hdffile   the HDF file name
     * @param hdfQueue  the queue to hold the HDF objects
     */
    public void getHdfObject(String hdffile, Queue hdfObjQueue)
    {
        this.hdf = new HDFLibrary();
	try
        {
            if ((fid = hdf.Hopen(hdffile)) <= 0)
                return;
            this.fileName = hdffile;

            // another HDFObjectNode variable
            HDFObjectNode   parentNode, childNode, node;

            // for HDF annotation;
            if (ANhasFileAnn(fid))
            {
                // HDF Object Annotation root node
                node = new HDFObjectNode();
                node.setObjType(node.Annotation);
                node.setName("File Annotation");
                hdfObjQueue.put(node);
            }

            // for HDF Objects
            getR8Object(hdffile,hdfObjQueue );
            getR24Object(hdffile,hdfObjQueue );
            getGRObject(fid, hdfObjQueue);
            getSDSObject(hdffile, hdfObjQueue);
            getVdataObject(fid, hdfObjQueue);
            getVgroupObject(fid, hdfObjQueue);

            hdf.Hclose(fid);
        } catch (HDFException e) {}
    }

    /** put the 8-bit raster image object into HDF queue
     * @param filenae  HDF file name
     * @param objQueue the queue to store the HDF object node
     *
     *  @exception ncsa.hdf.hdflib.HDFException 
     *             should be thrown for errors in the
     *             HDF library call, but is not yet implemented.
     *
     * @see HDFObjectNode
     */
    public void getR8Object(String filename, Queue objQueue)
        throws HDFException
    {
        //System.out.println("HDFAnalyse.getR8Object()");
        
        //restart raster image
        if(!hdf.DFR8restart()) return;

        HDFObjectNode parentNode, childNode, node;
        int argv[] = new int[2];
        boolean argB[] = new boolean[1];

        boolean validArgument = hdf.DFR8getdims(filename, argv, argB);
        if (!validArgument) return;

        //restart raster image
        if(!hdf.DFR8restart()) return;

        // get raster image number
        int imgNumber = hdf.DFR8nimages(filename);

        if (imgNumber <= 0)  return;

        // HDF Object RIS8 root node
        node = new HDFObjectNode();
        node.setObjType(node.RIS8);
        node.setObjLevel(0);
        node.setName("8-bit Raster Image");
        node.setHasChild(true);
        Queue childQueue = new Queue();
        node.setChildObj(childQueue);
        objQueue.put(node);
        parentNode = node;

        for (int i=0; i<imgNumber; i++)
        {
            hdf.DFR8getdims(filename,  argv, argB);
            int ref = hdf.DFR8lastref();
            int tag = HDFConstants.DFTAG_RIG;
            String rName = new String("8-bit Raster Image #");
            rName  += Integer.toString(i);

            // HDF Object RIS8 root node
            node = new HDFObjectNode();
            node.setObjType(node.RIS8);
            node.setTagRef(tag,ref);
            node.setObjLevel(1);
            node.setName(rName);
            node.setHasParent(true);
            node.setParentObj(parentNode);
            childQueue.put(node);
        }
    }

    /** put the 24-bit raster image object into HDF queue
     * @param filenae  HDF file name
     * @param objQueue the queue to store the HDF object node
     *
     *  @exception ncsa.hdf.hdflib.HDFException 
     *             should be thrown for errors in the
     *             HDF library call, but is not yet implemented.
     *
     * @see HDFObjectNode
     */
    public void getR24Object(String filename, Queue objQueue)
        throws HDFException
    {
        HDFObjectNode   parentNode, childNode, node;
        int args[] = new int[3];
        boolean validArgument = hdf.DF24getdims(filename, args);

        if (!validArgument) return;

        // restart
        hdf.DF24restart();

        // get raster image number
        int imgNumber = hdf.DF24nimages(filename);

	if (imgNumber <= 0) return;

        // HDF Object RIS24 root node
        node = new HDFObjectNode();
        node.setObjType(node.RIS24);
        node.setObjLevel(0);
        node.setName("24-bit Raster Image");
        node.setHasChild(true);
        Queue childQueue = new Queue();
        node.setChildObj(childQueue);
        objQueue.put(node);
        parentNode = node;

        for (int i=0; i<imgNumber; i++)
        {
            int ref = hdf.DF24lastref();
            int tag = HDFConstants.DFTAG_RIG;
            String rName = new String("24-bit Raster Image #");
            rName +=  Integer.toString(i) ;

            // HDF Object RIS24 root node
            node = new HDFObjectNode();
            node.setObjType(node.RIS24);
            node.setTagRef(tag,ref);
            node.setObjLevel(1);
            node.setName(rName);
            node.setHasParent(true);
            node.setParentObj(parentNode);
            childQueue.put(node);
        }
    }

    /** put the Vgroup object into HDF queue
     * @param fid  HDF file identifier by Hopen
     * @param objQueue the queue to store the HDF object node
     *
     *  @exception ncsa.hdf.hdflib.HDFException 
     *             should be thrown for errors in the
     *             HDF library call, but is not yet implemented.
     *
     * @see HDFObjectNode
     */
    public void getVgroupObject(int fid, Queue objQueue)
        throws HDFException
    {
        HDFObjectNode parentNode, childNode, node;
        int[] n = new int[1];
        boolean hasData = false;

        hdf.Vstart(fid);
        int vgNumber = hdf.Vlone(fid,n,0);

        if (vgNumber <= 0)
        {
            hdf.Vend(fid);
            return;
        }

        // vgroup object root node
        node = new HDFObjectNode();
        node.setObjType(node.Vgroup);
        node.setName("Vgroup");
        node.setHasChild(true);
        parentNode = node;
        Queue vgQueue = new Queue(vgNumber);
        parentNode.setChildObj(vgQueue);
        int refArray[] = new int[vgNumber];
        int refNum = hdf.Vlone(fid, refArray, vgNumber);

        if (vgNumber <= 0)
        {
            hdf.Vend(fid);
            return;
        }

        for (int i=0; i<refNum; i++)
        {
            int vgroup_ref = refArray[i];
            int vgtag = HDFConstants.DFTAG_VG;
            int vgroup_id = hdf.Vattach(fid, vgroup_ref, "r");
            if (vgroup_id == HDFConstants.FAIL)
            {
                hdf.Vdetach(vgroup_id);
                continue;
            }

            int tags[] = new int[100];
            int refs[] = new int[100];

            String s[] = {""};
            hdf.Vgetname(vgroup_id,s);
            String vgName = s[0].trim();
            hdf.Vgetclass(vgroup_id,s);
            String vgclass = s[0].trim();

            if ( vgclass.equalsIgnoreCase(HDFConstants.GR_NAME) ||
                 vgclass.equalsIgnoreCase(HDFConstants.HDF_CDF) )
                continue;

            hasData = true;

            // create new HDF object
            node = new HDFObjectNode();
            node.setObjType(node.Vgroup);
            node.setTagRef(vgtag, vgroup_ref);
            node.setParentObj(parentNode);
            node.setObjLevel(1);
            node.setName("Vg[ " + vgName + ": " + vgclass + " ]");

            int npairs = hdf.Vgettagrefs(vgroup_id, tags, refs, 100);
            if (npairs > 0)
            { // this group has more tag/ref's
                node.setHasChild(true);
                Queue vgChildQueue = new Queue(npairs);
                node.setChildObj(vgChildQueue);
                vgQueue.put(node);
                insertVg(node, vgChildQueue, fid, tags, refs, npairs,2);
            } else
                vgQueue.put(node);

            hdf.Vdetach(vgroup_id);
        }

        if (hasData) objQueue.put(parentNode);

        hdf.Vend(fid);
    }

    /** put the Vdata object into HDF queue
     * @param fid  HDF file identifier by Hopen
     * @param objQueue the queue to store the HDF object node
     *
     *  @exception ncsa.hdf.hdflib.HDFException 
     *             should be thrown for errors in the
     *             HDF library call, but is not yet implemented.
     *
     * @see HDFObjectNode
     */
    public void getVdataObject(int fid, Queue objQueue)
        throws HDFException
    {
        HDFObjectNode   parentNode, childNode, node;
        hdf.Vstart(fid);

        // for Vdata
        int [] n = new int[1];
        int vdNumber = hdf.VSlone(fid,n,0);

        if (vdNumber <= 0)
        {
            hdf.Vend(fid);
            return;
        }

        // Vdata object root node
        node = new HDFObjectNode();
        node.setObjType(node.Vdata);
        node.setName("Vdata");

        node.setHasChild(true);
        parentNode = node;
        Queue vdQueue = new Queue(vdNumber);
        parentNode.setChildObj(vdQueue);
        objQueue.put(parentNode);

        int refArray[] = new int[vdNumber];
        int refNum = hdf.VSlone(fid, refArray, vdNumber);

        if (refNum <= 0)
        {
            hdf.Vend(fid);
            return;
        }

        for (int i=0; i<refNum; i++)
        {
            int vdata_ref = refArray[i];
            int vdtag = HDFConstants.DFTAG_VS;

            int vdata_id;
            vdata_id = hdf.VSattach(fid, vdata_ref, "r");

            // create new HDF object
            node = new HDFObjectNode();
            node.setObjType(node.Vdata);
            node.setTagRef(vdtag, vdata_ref);
            String vdName = new String(" ");
            String[] s = new String[1];
            s[0] = vdName;
            hdf.VSgetname(vdata_id,s);
            vdName = s[0];
            String s1 = new String(" ");
            s[0] = s1;
            hdf.VSgetclass(vdata_id,s);
            String vdClassName = new String(s[0]);
	
            if (vdName.length() == 0)
                node.setName("unknown Vdata Name");
            else
                node.setName(vdName+": " + vdClassName);

            node.setHasParent(true);
            node.setParentObj(parentNode);
            node.setObjLevel(1);
            vdQueue.put(node);

            hdf.VSdetach(vdata_id);
        }

	hdf.Vend(fid);
    }

    /** put the SDS object into HDF queue
     * @param filename  HDF file name
     * @param objQueue the queue to store the HDF object node
     *
     *  @exception ncsa.hdf.hdflib.HDFException 
     *             should be thrown for errors in the
     *             HDF library call, but is not yet implemented.
     *
     * @see HDFObjectNode
     */
    public void getSDSObject(String filename, Queue objQueue)
        throws HDFException
    {

        HDFObjectNode   parentNode, childNode, node;
        int[] fileInfo = new int[2];

        int sdid;
        sdid = hdf.SDstart(filename, HDFConstants.DFACC_RDONLY);

        if (sdid == HDFConstants.FAIL)
        {
            hdf.SDend(sdid);
            return;
        }

        // get information about SDS
        if (hdf.SDfileinfo(sdid, fileInfo) == false)
        {
            hdf.SDend(sdid);
            return;
        }

        int sdsDatasetNumber    = fileInfo[0];
        int sdsGlobalAttrNumber = fileInfo[1];

        if (sdsDatasetNumber <= 0)
        {
            hdf.SDend(sdid);
            return;
        }

        // HDF SDS object root node
        node = new HDFObjectNode();
        node.setObjType(node.SDS);
        node.setName("SDS");
        node.setHasChild(true);
        parentNode = node;
        Queue sdsQueue = new Queue();
        parentNode.setChildObj(sdsQueue);

        // Add sds root  node
        objQueue.put(parentNode);

        // for SDS Global Attributes
        if (sdsGlobalAttrNumber > 0)
        {
            // HDF SDS Global Attribute object root node
            node = new HDFObjectNode();
            node.setObjType(node.SDSGLOBALATTR);
            node.setObjLevel(1);
            node.setName("Global Attributes");
            node.setHasParent(true);
            node.setParentObj(parentNode);
            sdsQueue.put(node);

        } // end of if (sdsGlobalAttrNumber > 0)

        // for SDS Datasets
        Queue sdsDatasetQueue = new Queue();;
        HDFObjectNode datasetParentNode = null;
        for (int i=0; i<sdsDatasetNumber; i++)
        {
            // select dataset
            int sdsid = hdf.SDselect(sdid,i);

            if (sdsid == HDFConstants.FAIL) continue;

            int tag = HDFConstants.DFTAG_NDG;
            int ref = hdf.SDidtoref(sdsid);

            // check if the given SDS has dimension metadata
            // Coordinate variables are created to store metadata associated
            // with dimensions. To ensure compatibility with netCDF, coordinate v
            // ariables are implemented as data sets
            if (hdf.SDiscoordvar(sdsid))
            {
                // do not display dimension metadata
                continue;
            }

            // get datsset info.
            int [] SDInfo = new int[3];
            String datasetname = new String(" ");
            String[] ss = new String[1];
            ss[0] = datasetname;
            int  dimsize[] = new int[10];
            if (!hdf.SDgetinfo(sdsid, ss, dimsize, SDInfo)) continue;
            datasetname = ss[0];

            // get full name of the data set
            int long_name_idx = hdf.SDfindattr(sdsid, "long_name");
            int [] SDattrInfo = new int[2];
            String name = new String(" ");
            ss[0] = name;
            hdf.SDattrinfo(sdsid, long_name_idx, ss,SDattrInfo);
            name = ss[0];
            if (long_name_idx != HDFConstants.FAIL)
            {
                String long_name = new String(" ");
                int attrSize = hdf.DFKNTsize(SDattrInfo[0])*SDattrInfo[1];
                byte[] buf = new byte[attrSize];
                if (hdf.SDreadattr(sdsid, long_name_idx, buf))
                {
                    long_name = new String(buf);
                    datasetname += ": "+long_name;
                }
            }

            // add SDS dataset subnode
            node = new HDFObjectNode();
            node.setObjType(node.SDSDATASET);
            node.setObjLevel(1);
            node.setName(datasetname);
            node.setHasParent(true);
            node.setParentObj(parentNode);
            node.setHasChild(true);
            datasetParentNode = node;
            sdsDatasetQueue = new Queue();
            node.setChildObj(sdsDatasetQueue);
            sdsQueue.put(node);
            node.setTagRef(tag,ref);

            // Also set ref to this node, so it is possible
            // to retrieve the dataset info.
            node = new HDFObjectNode();
            node.setTagRef(tag,ref);
            node.setObjType(node.SDSDATASET);
            node.setObjLevel(2);
            node.setIndex(i);
            node.setName(datasetname);
            node.setHasParent(true);
            node.setParentObj(datasetParentNode);
            sdsDatasetQueue.put(node);

            // Date: 6-12-97:
            // Added following code to handle object annotation. but it is limited.
            // For SDS, take "Number Dataset Group(720)" as an example
            // for dataset object annotation
            if ( ( (ANgetobjectlabel(tag,ref)).length() > 0 ) ||
                 ( (ANgetobjectdesc(tag,ref)).length() > 0 )   )
            {
                // HDF SDS dataset annotation   node
                node = new HDFObjectNode();
                node.setObjType(node.SDSDATASETAN);
                node.setObjLevel(2);
                node.setTagRef(tag,ref);
                node.setName("Dataset Annotation");
                node.setHasParent(true);
                node.setParentObj(datasetParentNode);

                // Add gr dataset node
                sdsDatasetQueue.put(node);
            }

            // for dataset attributes
            int datasetAttrNumber = SDInfo[2];
            if (datasetAttrNumber > 0)
            {
                // HDF SDS dataset Attribute object   node
                node = new HDFObjectNode();
                node.setObjType(node.SDSDATASETATTR);
                node.setObjLevel(2);
                node.setName("Dataset Attributes");
                node.setHasParent(true);
                node.setParentObj(datasetParentNode);
                node.setIndex(i);
                sdsDatasetQueue.put(node);
            } // end of if (datasetAttrNumber > 0)

            hdf.SDendaccess(sdsid);

        } // end of for (int i=0; i<sdsDatasetNumber; i++)

        hdf.SDend(sdid);

    }

    /** put the GR object into HDF queue
     * @param fid  HDF file identifier by Hopen
     * @param objQueue the queue to store the HDF object node
     *
     *  @exception ncsa.hdf.hdflib.HDFException 
     *             should be thrown for errors in the
     *             HDF library call, but is not yet implemented.
     *
     * @see HDFObjectNode
     */
    public void getGRObject(int fid , Queue objQueue)
        throws HDFException
    {
        HDFObjectNode   parentNode, childNode, node;
        int grid = hdf.GRstart(fid);

        if (grid == HDFConstants.FAIL)
        {
            hdf.GRend(grid);
            return;
        }

        // get information about GR
        int[] argv = new int[2];
        if (hdf.GRfileinfo(grid,argv ) == false)
        {
            hdf.GRend(grid);
            return;
        }

        int grDatasetNumber    = argv[0];
        int grGlobalAttrNumber = argv[1];

        if (grDatasetNumber <=0 )
        {
            hdf.GRend(grid);
            return;
        }

        // HDF GR object root node
        node = new HDFObjectNode();
        node.setObjType(node.GR);
        node.setName("GR");
        node.setHasChild(true);
        parentNode = node;
        Queue grQueue = new Queue();
        parentNode.setChildObj(grQueue);
        objQueue.put(parentNode);

        // for GR Global Attributes
        if (grGlobalAttrNumber > 0)
        {
            // add GR Global Attribute subnode
            node = new HDFObjectNode();
            node.setObjType(node.GRGLOBALATTR);
            node.setObjLevel(1);
            node.setName("Global Attributes");
            node.setHasParent(true);
            node.setParentObj(parentNode);
            grQueue.put(node);

        }

        // for GR Datasets
        for (int i=0; i<grDatasetNumber; i++)
        {
            // select dataset
            int riid = hdf.GRselect(grid,i);

            if (riid == HDFConstants.FAIL) continue;

            // add GR dataset subnode
            node = new HDFObjectNode();
            node.setObjType(node.GRDATASET);
            node.setObjLevel(1);
            node.setName("GR Dataset");
            node.setHasParent(true);
            node.setParentObj(parentNode);
            node.setHasChild(true);
            HDFObjectNode datasetParentNode = node;
            Queue grDatasetQueue = new Queue();
            node.setChildObj(grDatasetQueue);
            grQueue.put(node);

            // get datsset info.
            int  datasetinfo[] = new int[4];
            String datasetname = "";
            String[] n = new String[1];
            n[0] = datasetname;
            int  dimsize[] = new int[32]; /* MAX DIM SIZE? */

            /* the following method need to be fixed it. */
            if (hdf.GRgetiminfo(riid,n,datasetinfo,dimsize))
            {
                datasetname = n[0];
                // HDF GR dataset object  node
                short tag = (short)HDFConstants.DFTAG_RIG;
                short ref = hdf.GRidtoref(riid);

                node.setName(datasetname);
                node.setTagRef(-1,ref);
                node = new HDFObjectNode();
                node.setTagRef(tag,ref);
                node.setObjType(node.GRDATASET);
                node.setObjLevel(2);
                node.setIndex(i);
                node.setName(datasetname);
                node.setHasParent(true);
                node.setParentObj(datasetParentNode);
                grDatasetQueue.put(node);

                // Date: 6-12-97:
                // Added following code to handle object annotation. but it is limited.
                // For R8, R24 take 306 as tag for object annotation.
                // For GR, take "Raster Image Group(306)" as an example
                // for dataset object annotation
                if ( ( (ANgetobjectlabel(tag,ref)).length() > 0 ) ||
                     ( (ANgetobjectdesc(tag,ref)).length() > 0 ) )
                {
                    // HDF GR dataset annotation   node
                    node = new HDFObjectNode();
                    node.setObjType(node.GRDATASETAN);
                    node.setObjLevel(2);
                    node.setTagRef(tag,ref);
                    node.setName("GR Dataset Annotation");
                    node.setHasParent(true);
                    node.setParentObj(datasetParentNode);
                    grDatasetQueue.put(node);
                }

                // for dataset attributes
                int datasetAttrNumber = datasetinfo[3];
                if (datasetAttrNumber > 0)
                {
                    // HDF GR dataset Attribute object   node
                    node = new HDFObjectNode();
                    node.setObjType(node.GRDATASETATTR);
                    node.setObjLevel(2);
                    node.setIndex(i);
                    node.setName("Dataset Attributes");
                    node.setHasParent(true);
                    node.setParentObj(datasetParentNode);
                    grDatasetQueue.put(node);
                }
            }
            hdf.GRendaccess(riid);
        }
        hdf.GRend(grid);
    }


    /** put the SDS object into HDF queue
     * @param pnode  HDF object node
     * @param filename the HDF file name
     * @param ref  the reference number of the SDS;
     * @param level the hierachal level
     * @param objQueue the queue to store the HDF object node
     *
     *  @exception ncsa.hdf.hdflib.HDFException 
     *             should be thrown for errors in the
     *             HDF library call, but is not yet implemented.
     *
     * @see HDFObjectNode
     */
    public void getSDSObject(HDFObjectNode pnode, String filename,
        int ref, int level, Queue objQueue)
        throws HDFException
    {
        //System.out.println("HDFAnalyse.getSDSObject()");

        HDFObjectNode   parentNode, childNode, node;
        int[] fileInfo = new int[2];
        int sdid = hdf.SDstart(filename, HDFConstants.DFACC_RDONLY);

        if (sdid == HDFConstants.FAIL)
        {
            hdf.SDend(sdid);
            return;
        }

        // get information about SDS
        if (hdf.SDfileinfo(sdid, fileInfo) == false)
        {
            hdf.SDend(sdid);
            return;
        }

        // add SDS dataset subnode
        node = new HDFObjectNode();
        node.setObjType(node.SDSDATASET);
        node.setObjLevel(level);
        node.setName("Dataset");
        node.setHasParent(true);
        node.setParentObj(pnode);
        node.setHasChild(true);
        HDFObjectNode datasetParentNode = node;
        Queue sdsDatasetQueue = new Queue();
        node.setChildObj(sdsDatasetQueue);
        objQueue.put(node);

        // select dataset
        int index = hdf.SDreftoindex(sdid, ref);
        int riid = hdf.SDselect(sdid,index);

        if (riid == HDFConstants.FAIL)
        {
            hdf.SDend(sdid);
            return;
        }

        // check if the given SDS is the dimension scale
        if (hdf.SDiscoordvar(riid))
        {
            hdf.SDend(sdid);
           // modify the node info. (this is dim. )
            node.setHasChild(false);
            node.setChildObj(null);
            node.setName("Dimension Var.");
            return;
        }
           
        // get datsset info.
        String datasetname = new String(" ");
        String[] ss = new String[1];
        ss[0] = datasetname;
        int[] SDInfo = new int[3];
        int  dimsize[] = new int[32];

        if (hdf.SDgetinfo(riid, ss, dimsize, SDInfo))
        {
            datasetname = ss[0];

            // update node value ===>>> dataset-name instead of "SDS dataset"
            node.setName(datasetname);

            int long_name_idx = hdf.SDfindattr(riid, "long_name");
            int [] SDattrInfo = new int[2];

            String name = "";
            ss[0] = name;
            hdf.SDattrinfo(riid, long_name_idx, ss, SDattrInfo);
            name = ss[0];
            if (long_name_idx != HDFConstants.FAIL)
            {
                String long_name = new String(" ");
                int attrSize = hdf.DFKNTsize(SDattrInfo[0])* SDattrInfo[1] + 1;
                byte[] buf = new byte[attrSize];
                if (hdf.SDreadattr(riid, long_name_idx, buf))
                {
                    long_name = new String(buf);
                    node.setName(node.getName()+": "+long_name);
                }
            }

            // Also set ref to this node, so it is possible
            // to retrieve the dataset info.
            node.setTagRef(-1,ref);

            // HDF SDS dataset object  node
            int tag = HDFConstants.DFTAG_RIG;

            node = new HDFObjectNode();
            node.setTagRef(tag,ref);
            node.setObjType(node.SDSDATASET);
            node.setObjLevel(level+1);
            node.setIndex(index);
            node.setName(datasetname);
            node.setHasParent(true);
            node.setParentObj(datasetParentNode);

            sdsDatasetQueue.put(node);
	
            // Date: 6-12-97:
            // Added following code to handle object annotation. but it is limited.
            // For SDS, take "Number Dataset Group(720)" as an example
            // for dataset object annotation
            if ( ( (ANgetobjectlabel(tag,ref)).length() > 0 ) ||
                 ( (ANgetobjectdesc(tag,ref)).length() > 0 ) )
            {
                // HDF SDS dataset annotation   node
                node = new HDFObjectNode();
                node.setObjType(node.SDSDATASETAN);
                node.setObjLevel(level+1);
                node.setTagRef(tag,ref);
                node.setName("SDS Dataset Annotation");
                node.setHasParent(true);
                node.setParentObj(datasetParentNode);
                sdsDatasetQueue.put(node);
            }

            // for dataset attributes
            int datasetAttrNumber = SDInfo[2];
            if (datasetAttrNumber > 0)
            {
                // HDF SDS dataset Attribute object   node
                node = new HDFObjectNode();
                node.setObjType(node.SDSDATASETATTR);
                node.setObjLevel(level+1);
                node.setName("Dataset Attributes");
                node.setHasParent(true);
                node.setParentObj(datasetParentNode);
                sdsDatasetQueue.put(node);
            }
        }
        hdf.SDendaccess(riid);
        hdf.SDend(sdid);
    }

    /** put the GR object into HDF queue
     * @param pnode  HDF object node
     * @param fid  the HDF file identifier
     * @param ref  the reference number of the GR;
     * @param level the hierachal level
     * @param objQueue the queue to store the HDF object node
     *
     *  @exception ncsa.hdf.hdflib.HDFException 
     *             should be thrown for errors in the
     *             HDF library call, but is not yet implemented.
     *
     * @see HDFObjectNode
     */
    public void getGRObject(HDFObjectNode pnode, int fid,
        int ref, int level, Queue objQueue)
        throws HDFException
    {
        HDFObjectNode   parentNode, childNode, node;
        int grid = hdf.GRstart(fid);

        if (grid == HDFConstants.FAIL)
        {
            hdf.GRend(grid);
            return;
        }

        // get information about GR
        int[] argv = new int[2];
        if (hdf.GRfileinfo(grid, argv) == false)
        {
            hdf.GRend(grid);
            return;
        }
	
        // add GR dataset subnode
        node = new HDFObjectNode();
        node.setObjType(node.GRDATASET);
        node.setObjLevel(level);
        node.setName("GR Dataset");
        node.setHasParent(true);
        node.setParentObj(pnode);
        node.setHasChild(true);
        HDFObjectNode datasetParentNode = node;
        Queue grDatasetQueue = new Queue();
        node.setChildObj(grDatasetQueue);
        objQueue.put(node);

        /* GRreftoindex() need to be fixed */
        int index = hdf.GRreftoindex(grid, (short)ref);
        int riid  = hdf.GRselect(grid,index);
	   
        if (riid == HDFConstants.FAIL) return;
	   
        // get datsset info.
        int [] imInfo = new int [4];
        String datasetname = "";
        String[] n = new String[1];
        n[0] = datasetname;
        int  dimsize[] = new int[32]; /* MAX DIM SIZE? */

        if (hdf.GRgetiminfo(riid,n,imInfo,dimsize))
        {
            datasetname = n[0];
            node.setName(datasetname);
            node.setTagRef(-1,ref);
            int tag = HDFConstants.DFTAG_RIG;
            node = new HDFObjectNode();
            node.setTagRef(tag,ref);
            node.setObjType(node.GRDATASET);
            node.setObjLevel(level+1);
            node.setIndex(index);
            node.setName(datasetname);
            node.setHasParent(true);
            node.setParentObj(datasetParentNode);
            grDatasetQueue.put(node);

            // for dataset attributes
            int datasetAttrNumber = imInfo[3];
            if (datasetAttrNumber > 0)
            {
                // HDF GR dataset Attribute object   node
                node = new HDFObjectNode();
                node.setObjType(node.GRDATASETATTR);
                node.setObjLevel(level+1);
                node.setName("Dataset Attributes");
                node.setHasParent(true);
                node.setParentObj(datasetParentNode);
                grDatasetQueue.put(node);
            }
        }
        hdf.GRendaccess(riid);
        hdf.GRend(grid);
    }

    /**  insert the HDF object belongs to specified HDF Vgroup
     * @param pnode  the HDF object node.
     * @param subQueue the queue to store the HDF object belongs to Vgroup
     * @param fid    the HDF file id.
     * @param tags   the tags value
     * @param refs    the reference number
     * @param npairs  the number of objects belongs to specified Vgroup
     * @param level   the level in the HDF Object
     *
     *  @exception ncsa.hdf.hdflib.HDFException 
     *             should be thrown for errors in the
     *             HDF library call, but is not yet implemented.
     *
     */
    public  void insertVg(HDFObjectNode pnode, Queue subqueue,
        int fid, int[] tags, int[] refs, int npairs, int level)
        throws HDFException
    {
        HDFObjectNode node;

        if (npairs <= 0) return;

        for (int i=0; i<npairs; i++)
        {
            int tag = tags[i];
            int ref = refs[i];

            switch(tag)
            {
                // ?????????????????     RIS8 RIS24 GR ?????????????????
                case HDFConstants.DFTAG_RIG:
                    getGRObject(pnode,fid, ref, level, subqueue);
                    break;
                // ?????????????????    SDS  ?????????????????
                case HDFConstants.DFTAG_SDG:
                case HDFConstants.DFTAG_NDG:
                    getSDSObject(pnode,fileName, ref, level, subqueue);
                    break;
                case HDFConstants.DFTAG_VH: // vdata
                    int vdataid = hdf.VSattach(fid, ref, "r");
                    node = new HDFObjectNode();
                    node.setObjLevel(level);
                    node.setTagRef(tag,ref);
                    String s1 = "";
                    String s[] = new String[1];
                    s[0] = s1;
                    hdf.VSgetname(vdataid,s);
                    s1 = s[0];
                    String s2 = "";
                    s[0] = s2;
                    hdf.VSgetclass(vdataid, s);
                    s2 = s[0];
                    node.setName( "Vd[ " + s1 + ": " + s2 + " ]" );
                    node.setObjType(node.Vdata);
                    node.setHasParent(true);
                    node.setParentObj(pnode);
                    subqueue.put(node);
                    hdf.VSdetach(vdataid);
                    break;
                case HDFConstants.DFTAG_VG: // vgroup
                    node = new HDFObjectNode();
                    node.setObjLevel(level);
                    String vgName = new String ("Vgroup[ref: ");
                    vgName += (Integer.toString(ref));
                    vgName += ("]");
                    node.setName(vgName);
                    node.setTagRef(tag,ref);
                    node.setObjType(node.Vgroup);
                    node.setHasParent(true);
                    node.setParentObj(pnode);
                    subqueue.put(node);
                    int vgroup_id = hdf.Vattach(fid, ref, "r");
                    if (vgroup_id != HDFConstants.FAIL)
                    {
                        String vgname = new String(" ");
                        String vgclass = new String(" ");
                        s = new String[1];
                        s[0] = vgname;
                        hdf.Vgetname(vgroup_id, s);
                        vgname = s[0];
                        s[0] = vgclass;
                        hdf.Vgetclass(vgroup_id, s);
                        vgclass = s[0];
                        node.setName("Vg[ " + vgname + ": " + vgclass + " ]");
                        int subtags[] = new int[10]; /* some MAX constant ?*/
                        int subrefs[] = new int[10];
                        int subnpairs = hdf.Vgettagrefs(vgroup_id, subtags, subrefs, 10);
                        if (subnpairs > 0)
                        {
                            Queue subsubqueue = new Queue(subnpairs);
                            node.setChildObj(subsubqueue);
                            node.setHasChild(true);
                            insertVg(node, subsubqueue, fid, subtags, subrefs, subnpairs, level+1);
                        }
                        hdf.Vdetach(vgroup_id);
                    }
                    break;
                default:
            } // end of switch(tag)
        } // end of for (int i=0; i<npairs; i++)
    }

    /** Does the HDF file have file label or file description.
     *
     *  @exception ncsa.hdf.hdflib.HDFException 
     *             should be thrown for errors in the
     *             HDF library call, but is not yet implemented.
     *
     * @return true if the HDF file contains file label or desc. , 
     *         false otherwise
     */
    public boolean  ANhasFileAnn(int fid)
        throws HDFException
    {
        int anid = hdf.ANstart(fid);
        boolean retValue = false;

        int fileInfo[] = new int[4];
        fileInfo[0] = fileInfo[1] = 0;

        if ( (anid != HDFConstants.FAIL)     &&
              hdf.ANfileinfo(anid, fileInfo) &&
             ((fileInfo[0] + fileInfo[1])>0))
            retValue = true;

        hdf.ANend(anid);
        return retValue;
    }

    /** get object  label from HDF file
     * @param tag  the tag of the HDF object.
     * @param ref  the reference number
     *
     *  @exception ncsa.hdf.hdflib.HDFException 
     *             should be thrown for errors in the
     *             HDF library call, but is not yet implemented.
     *
     * @return the string look like text of the object label 
     */
    public String ANgetobjectlabel(int tag,int ref)
        throws HDFException
    {
        StringBuffer retStr = new StringBuffer();
        int[] fileInfo = new int[4];
        int an_id =  hdf.ANstart(this.fid);

        if (hdf.ANfileinfo(an_id, fileInfo) == false )
        {
            hdf.ANend(an_id);
            return  retStr.toString();
        }

        int anntype = HDFConstants.AN_DATA_LABEL;
        int numObjectLabel = hdf.ANnumann(an_id, anntype, (short)tag, (short)ref);
        if (numObjectLabel == HDFConstants.FAIL) return "";
        int ann_id_list[] = new int[numObjectLabel];
        int numAnnid = hdf.ANannlist(an_id, anntype, tag, ref, ann_id_list);
        if (numAnnid == HDFConstants.FAIL) return "";
	
        for (int i=0; i<numObjectLabel; i++)
        {
            int annid = ann_id_list[i];
            int objectLabelLen = hdf.ANannlen(annid);
            if (objectLabelLen >0)
            {
                String objectLabelBuf[] = new String[1];
                objectLabelBuf[0] = new String("");
                if (hdf.ANreadann(annid, objectLabelBuf, objectLabelLen))
                {
                    retStr.append("  ");
                    retStr.append((new String(objectLabelBuf[0])).trim());
                    retStr.append("           \n");
                }
            } // if (objectLabelLen > 0)
        }  // for ()

        hdf.ANend(an_id);
        return retStr.toString();

    }

    /** get the object desc from HDF file
     * @param tag  the tag of the HDF object.
     * @param ref  the reference number
     * @return the string look like text of the object desc 
     *
     *  @exception ncsa.hdf.hdflib.HDFException 
     *             should be thrown for errors in the
     *             HDF library call, but is not yet implemented.
     *
     */
    public String ANgetobjectdesc(int tag,int ref)
        throws HDFException
    {
        StringBuffer retStr = new StringBuffer();
        int[] fileInfo = new int[4];
        int anntype = HDFConstants.AN_DATA_DESC;
        int an_id =  hdf.ANstart(this.fid);

        if (hdf.ANfileinfo(an_id, fileInfo) == false )
        {
            hdf.ANend(an_id);
            return  "";
        }

        int numObjectDesc = hdf.ANnumann(an_id, anntype, (short)tag, (short)ref);
        if (numObjectDesc == HDFConstants.FAIL) return "";
	int ann_id_list[] = new int[numObjectDesc];
	int numAnnid = hdf.ANannlist(an_id, anntype, tag, ref, ann_id_list);
	if (numAnnid == HDFConstants.FAIL) return"";

        for (int i=0; i<numObjectDesc; i++)
        {
            int annid = ann_id_list[i];
            int objectDescLen = hdf.ANannlen(annid);
            if (objectDescLen >0)
            {
                String objectDescBuf[] = new String[1];
                objectDescBuf[0] = new String("");
                if (hdf.ANreadann(annid, objectDescBuf, objectDescLen))
                {
                    retStr.append("  ");
                    retStr.append((new String(objectDescBuf[0])).trim());
                    retStr.append("   \n");
                }
            } // if (objectDescLen > 0)
        }  // for ()
	
        hdf.ANend(an_id);
        return retStr.toString();
    }


}
