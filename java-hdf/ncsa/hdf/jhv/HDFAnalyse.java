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

package ncsa.hdf.jhv;

import java.util.Vector;

import java.util.Queue;

import ncsa.hdf.hdflib.*;

 /* This class will analyse the HDF file and put the HDF object into 
  * the Queue. Hdf object is the node create by HDFObjectNode class
  * Date: 3-12-97 Modified "getSDSObject()".
  * 		  Keep the name of an HDF object node to be more readable,
  *		  to be as the form look like "sdsname: long_name".
  *  		  
  */

public class HDFAnalyse {

	// HDF Object
	public HDFLibrary hdf;
	int fid = -1;
	String fileName = new String(" ");

   /** analyse the HDF file then put it into Queue 
     * @param hdffile   the HDF file name
     * @param hdfQueue  the queue to hold the HDF objects
     */
   public void getHdfObject(String hdffile, Queue hdfObjQueue) {

	this.hdf = new HDFLibrary();

	try {
            	if ((fid = this.hdf.Hopen(hdffile)) <= 0)
        	    return;
        
        	this.fileName = hdffile;
        	 
        	// another HDFObjectNode variable	
        	HDFObjectNode   parentNode, childNode, node;
        
        	// for HDF annotation;
        	if (ANhasFileAnn(fid)) {
        
        	    // HDF Object Annotation root node
        	    node = new HDFObjectNode();	
        	    node.setObjType(node.Annotation);
        	    node.setName("File Annotation");
        	    hdfObjQueue.put(node);
        	}
        
        	// for HDF 8-bit Raster Image
        	getR8Object(hdffile,hdfObjQueue );
        	
        	// for HDF 24-bit Raster Image
        	getR24Object(hdffile,hdfObjQueue );
        
        
        	// for GR
        	getGRObject(fid, hdfObjQueue);
        
        	// for SDS
        	getSDSObject(hdffile, hdfObjQueue);
        
        	// for Vdata
        	getVdataObject(fid, hdfObjQueue);
        
        	// for HDF Vgroup
        	getVgroupObject(fid, hdfObjQueue);
        
        	hdf.Hclose(fid);

	} catch (HDFException e) {
		System.out.println("Warning: caught HDFException (getHdfObject): "+e);
                return;
        }
    }

 /** put the 8-bit raster image object into HDF queue
   * @param filenae  HDF file name
   * @param objQueue the queue to store the HDF object node
   * @see HDFObjectNode
   */  

  public void getR8Object(String filename, Queue objQueue) throws HDFException {

	HDFObjectNode   parentNode, childNode, node;
	int argv[] = new int[2];
	boolean argB[] = new boolean[1];

	// start fron the beginner
	hdf.DFR8restart();

	// get raster image number
	int imgNumber = hdf.DFR8nimages(filename);

	if (imgNumber>0) {
	   
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

	    for (int i=0; i<imgNumber; i++) {
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
    }

 /** put the 24-bit raster image object into HDF queue
   * @param filenae  HDF file name
   * @param objQueue the queue to store the HDF object node
   * @see HDFObjectNode
   */  

  public void getR24Object(String filename, Queue objQueue) throws HDFException {

	HDFObjectNode   parentNode, childNode, node;
	int args[] = new int[3];
	
	// restart
	hdf.DF24restart();

	// get raster image number
	int imgNumber = hdf.DF24nimages(filename);

	if (imgNumber>0) {
	   
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

	    for (int i=0; i<imgNumber; i++) {
		hdf.DF24getdims(filename, args);
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
    }

 /** put the Vgroup object into HDF queue
   * @param fid  HDF file identifier by Hopen
   * @param objQueue the queue to store the HDF object node
   * @see HDFObjectNode
   */  

  public void getVgroupObject(int fid, Queue objQueue) throws HDFException {

	HDFObjectNode   parentNode, childNode, node;
	
	hdf.Vstart(fid);	

	int [] n = new int[1];
	int vgNumber = hdf.Vlone(fid,n,0);

	if (vgNumber> 0) {

	   // vgroup object root node
	   node = new HDFObjectNode();
	   node.setObjType(node.Vgroup);
	   node.setName("Vgroup");

	   node.setHasChild(true);
	   parentNode = node;
	   Queue vgQueue = new Queue(vgNumber);

	   parentNode.setChildObj(vgQueue);
	   objQueue.put(parentNode);
	   int refArray[] = new int[vgNumber];
	   int refNum;

	   if  ((refNum = hdf.Vlone(fid, refArray, vgNumber)) > 0){
		
		// object will be put into vgQueue
		
   	    	for (int i=0; i<refNum; i++) {	      

	       		int vgroup_ref = refArray[i];	       		
	       		int vgtag = HDFConstants.DFTAG_VG;
   
			// set Vgroup Name 
		 	String vgName = new String ("Vgroup[ref: ");
			vgName +=  Integer.toString(vgroup_ref) ;
			vgName += "]" ;

			 // create new HDF object
			node = new HDFObjectNode();
			node.setObjType(node.Vgroup);
			node.setTagRef(vgtag, vgroup_ref);
			node.setParentObj(parentNode);
			node.setObjLevel(1);
			node.setName(vgName);

			int vgroup_id = hdf.Vattach(fid, vgroup_ref, "r");
			if (vgroup_id == HDFConstants.FAIL) {
			   hdf.Vdetach(vgroup_id);
	 		   continue;
			}

			int tags[] = new int[100];
			int refs[] = new int[100];

			String vgclass = new String(" ");
			String s[] = new String[1];
			s[0] = vgName;
		        hdf.Vgetname(vgroup_id,s);
			vgName = s[0];
			s[0] = vgclass;
			hdf.Vgetclass(vgroup_id,s);
			vgclass = s[0];
			// reset the group name for vgroup node
			node.setName("Vg[ " + vgName + ": " + vgclass + " ]");

			int npairs = hdf.Vgettagrefs(vgroup_id, tags, refs, 100);
		
			// terminate the access to Vgroup
			hdf.Vdetach(vgroup_id);

			if (npairs > 0) { // this group has more tag/ref's
			   node.setHasChild(true);
			   Queue vgChildQueue = new Queue(npairs);
			   node.setChildObj(vgChildQueue);
			   vgQueue.put(node);
			   insertVg(node, vgChildQueue, fid, tags, refs, npairs,2);
			}
			else { 
			   vgQueue.put(node);
			}
			hdf.Vdetach(vgroup_id);
		   }
		}  
	}
	hdf.Vend(fid);
    }

 /** put the Vdata object into HDF queue
   * @param fid  HDF file identifier by Hopen
   * @param objQueue the queue to store the HDF object node
   * @see HDFObjectNode
   */  

  public void getVdataObject(int fid, Queue objQueue) throws HDFException {

	HDFObjectNode   parentNode, childNode, node;

	hdf.Vstart(fid);	

	// for Vdata
	int [] n = new int[1];
	int vdNumber = hdf.VSlone(fid,n,0);

	if (vdNumber> 0) {
	   
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
	   int refNum;

	   // for child of the Vdata root
	   if  ((refNum = hdf.VSlone(fid, refArray, vdNumber)) > 0){
		
		// object will be put into vdQueue
		
   	    	for (int i=0; i<refNum; i++) {	      

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
		}   
	}
	hdf.Vend(fid);
    }

     /** put the SDS object into HDF queue
       * @param filename  HDF file name
       * @param objQueue the queue to store the HDF object node
       * @see HDFObjectNode
       */  

      public void getSDSObject(String filename, Queue objQueue) throws HDFException {
	    HDFObjectNode   parentNode, childNode, node;
	    int[] fileInfo = new int[2];

	    int sdid;
	    sdid = hdf.SDstart(filename, HDFConstants.DFACC_RDONLY);	
	    if (sdid == HDFConstants.FAIL) {
		hdf.SDend(sdid);
	        return;
	    }

	    // get information about SDS
	    if (hdf.SDfileinfo(sdid, fileInfo) == false) {
	       hdf.SDend(sdid);
	       return;
	    }

	    /*  HDF SDS structure
		    SDS --------------  global attributes ----  1. 
							  ----  2.
							  ...... (Vdata)
			--------------  datasets 
					    if sds is dimension scale , ignore...
					    sds(1) --- attributes 
							    to be as vdata();
						   --- Dataset
							    sds object
					    sds(2) ---- .........

	    */   	

	    int sdsDatasetNumber    = fileInfo[0];
	    int sdsGlobalAttrNumber = fileInfo[1];

	    if (sdsDatasetNumber> 0) {

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
	       if (sdsGlobalAttrNumber > 0) {
		    // add SDS Global Attribute subnode

		   // HDF SDS Global Attribute object root node
		   node = new HDFObjectNode();
		   node.setObjType(node.SDSGLOBALATTR);
		   node.setObjLevel(1);
		   node.setName("Global Attributes");

		   node.setHasParent(true);
		   node.setParentObj(parentNode);
		   node.setHasChild(true);

		   HDFObjectNode attrParentNode = node;
		   Queue sdsGlobalQueue         = new Queue();

		   node.setChildObj(sdsGlobalQueue);

		   // Add sds global root  node
		   sdsQueue.put(node);   // for SDS Global Attributes
	
		   // for each global attribute
		   for (int i=0; i<sdsGlobalAttrNumber; i++) {
			// get attribute info.
			String attrname = new String(" ");
			String[] ss = new String[1];
			ss[0] = attrname;
			int[] SDattrInfo = new int[2];
			if (hdf.SDattrinfo(sdid, i, ss, SDattrInfo)) {
			       attrname = ss[0];

			       // HDF SDS Global Attribute object  node
			       node = new HDFObjectNode();
			       node.setObjType(node.SDSGLOBALATTR);
			       node.setObjLevel(2);
			       node.setIndex(i);
			       node.setName(attrname);
			       node.setHasParent(true);
			       node.setParentObj(attrParentNode);
			   
			       sdsGlobalQueue.put(node);
			}
		   } 
		} 

	        // for SDS Datasets	
		for (int i=0; i<sdsDatasetNumber; i++) {
		
		      // select dataset
		      int sdsid ;
		      sdsid = hdf.SDselect(sdid,i);

		      if (sdsid == HDFConstants.FAIL)
		         continue;
		       // check if the given SDS is the dimension scale
		      if (hdf.SDiscoordvar(sdsid)) {
			 hdf.SDendaccess(sdsid);
			 continue;
	 	       }

		       // add SDS dataset subnode
		       node = new HDFObjectNode();
		       node.setObjType(node.SDSDATASET);
		       node.setObjLevel(1);

		       node.setName("SDS Dataset");

		       node.setHasParent(true);
		       node.setParentObj(parentNode);
		       node.setHasChild(true);

		       HDFObjectNode datasetParentNode = node;
		       Queue sdsDatasetQueue         = new Queue();

		       node.setChildObj(sdsDatasetQueue);

		       // Add sds dataset root  node
		       sdsQueue.put(node);

	
			// get datsset info.
		       int [] SDInfo = new int[3];
		       String datasetname = new String(" ");
		       String[] ss = new String[1];
		       ss[0] = datasetname;
		       int  dimsize[]     = new int[10];
		       if (hdf.SDgetinfo(sdsid, ss, dimsize, SDInfo)) {
				datasetname = ss[0];

				// update node value ===>>> dataset-name instead of "SDS dataset"
				node.setName(datasetname);

				// get pre-defined "long_name", if exist, update it.
				int long_name_idx = hdf.SDfindattr(sdsid, "long_name");

				int [] SDattrInfo = new int[2];
				String name = new String(" ");
				ss[0] = name;
				hdf.SDattrinfo(sdsid, long_name_idx, ss,SDattrInfo);
				name = ss[0];
				if (long_name_idx != HDFConstants.FAIL) {
					String long_name = new String(" ");
					int attrSize = hdf.DFKNTsize(SDattrInfo[0])*SDattrInfo[1] + 1;
					byte[] buf = new byte[attrSize];
					if (hdf.SDreadattr(sdsid, long_name_idx, buf)) {
						long_name = new String(buf);
					   node.setName(node.getName()+": "+long_name);
					}
				}

			       // HDF SDS dataset object  node
				int tag = HDFConstants.DFTAG_NDG;
				int ref = hdf.SDidtoref(sdsid);

				// Also set ref to this node, so it is possible 
				// to retrieve the dataset info.
				node.setTagRef(-1,ref);

			       node = new HDFObjectNode();
			       node.setTagRef(tag,ref);
			       node.setObjType(node.SDSDATASET);
			       node.setObjLevel(2);
			       node.setIndex(i);
			       node.setName(datasetname);
			       node.setHasParent(true);
			       node.setParentObj(datasetParentNode);
			   
			       sdsDatasetQueue.put(node);

			       // for dataset attributes
			       int datasetAttrNumber = SDInfo[2];
			       if (datasetAttrNumber > 0) {

				   // HDF SDS dataset Attribute object   node
				   node = new HDFObjectNode();
				   node.setObjType(node.SDSDATASETATTR);
				   node.setObjLevel(2);

				   node.setName("SDS Dataset Attributes");

				   node.setHasParent(true);
				   node.setParentObj(datasetParentNode);
				   node.setHasChild(true);

				   HDFObjectNode datasetAttrParentNode = node;
				   Queue sdsDatasetAttrQueue           = new Queue();

				   node.setChildObj(sdsDatasetAttrQueue);

				   // Add sds dataset node
				   sdsDatasetQueue.put(node);

				   // for each dataset attribute
				   for (int kk=0; kk<datasetAttrNumber; kk++) {
					// get attribute info.

					String attrname = new String(" ");
					ss[0] = attrname;
					if (hdf.SDattrinfo( sdsid, kk, 
						ss, SDattrInfo)) {
						attrname = ss[0];

					       // HDF SDS dataset Attribute object  node
					       node = new HDFObjectNode();
					       node.setTagRef(-1,ref);
					       node.setObjType(node.SDSDATASETATTR);
					       node.setObjLevel(3);
					       node.setIndex(kk);
					       node.setName(attrname);
					       node.setHasParent(true);
					       node.setParentObj(datasetAttrParentNode);

					       sdsDatasetAttrQueue.put(node);
					}
				   }
				} 
		   	  } 
		  hdf.SDendaccess(sdsid);
		} 
	     } 
	    hdf.SDend(sdid);
	}
    

     /** put the GR object into HDF queue
       * @param fid  HDF file identifier by Hopen
       * @param objQueue the queue to store the HDF object node
       * @see HDFObjectNode
       */  

      public void getGRObject(int fid , Queue objQueue) throws HDFException {

	    HDFObjectNode   parentNode, childNode, node;

	    int grid;
	    grid = hdf.GRstart(fid);	
	    if (grid == HDFConstants.FAIL) {
		hdf.GRend(grid);
	        return;
	    }

	    // get information about GR
	    int[] argv = new int[2];
	    if (hdf.GRfileinfo(grid,argv ) == false) {
	       // terminte access to GR
	       hdf.GRend(grid);
	       return;
	    }
	    /*  HDF GR structure
		    GR --------------  global attributes ----  1. 
							  ----  2.
							  ...... (Vdata)
			--------------  datasets 
					    if gr is dimension scale , ignore...
					    gr(1) --- attributes 
							    to be as vdata();
						   --- Dataset
							    gr object
					    gr(2) ---- .........

	    */   	

	    int grDatasetNumber    = argv[0];
	    int grGlobalAttrNumber = argv[1];

	    if (grDatasetNumber> 0) {

	       // HDF GR object root node
	       node = new HDFObjectNode();
	       node.setObjType(node.GR);

	       node.setName("GR");

	       node.setHasChild(true);
	       parentNode = node;
	       Queue grQueue = new Queue();

	       parentNode.setChildObj(grQueue);

	       // Add gr root  node
	       objQueue.put(parentNode);

	       // for GR Global Attributes
	       if (grGlobalAttrNumber > 0) {
		    // add GR Global Attribute subnode

		   // HDF GR Global Attribute object root node
		   node = new HDFObjectNode();
		   node.setObjType(node.GRGLOBALATTR);
		   node.setObjLevel(1);
		   node.setName("GR Global Attributes");

		   node.setHasParent(true);
		   node.setParentObj(parentNode);
		   node.setHasChild(true);

		   HDFObjectNode attrParentNode = node;
		   Queue grGlobalQueue         = new Queue();

		   node.setChildObj(grGlobalQueue);

		   // Add gr global root  node
		   grQueue.put(node);   // for GR Global Attributes
	
		   // for each global attribute
		   for (int i=0; i<grGlobalAttrNumber; i++) {
			// get attribute info.
			String attrname = new String(" ");
			String [] n = new String[1];
			n[0] = attrname;
			int[] GRattrInfo = new int[2];
			// global attributes
			if (hdf.GRattrinfo(grid, i, n, GRattrInfo)) {
				attrname = n[0];

			       // HDF GR Global Attribute object  node
			       node = new HDFObjectNode();
			       node.setObjType(node.GRGLOBALATTR);
			       node.setObjLevel(2);
			       node.setIndex(i);
			       node.setName(attrname);
			       node.setHasParent(true);
			       node.setParentObj(attrParentNode);
			   
			       grGlobalQueue.put(node);
			}
		   } 
		} 


	        // for GR Datasets	
		for (int i=0; i<grDatasetNumber; i++) {
		
		     // select dataset
		     int riid;
		     riid = hdf.GRselect(grid,i);

		     if (riid == HDFConstants.FAIL)
			 continue;

		      // add GR dataset subnode

		       node = new HDFObjectNode();
		       node.setObjType(node.GRDATASET);
		       node.setObjLevel(1);

		       node.setName("GR Dataset");   // change to gr dataset name later

		       node.setHasParent(true);
		       node.setParentObj(parentNode);
		       node.setHasChild(true);

		       HDFObjectNode datasetParentNode = node;
		       Queue grDatasetQueue         = new Queue();

		       node.setChildObj(grDatasetQueue);

		       // Add gr dataset root  node
		       grQueue.put(node);

	
			// get datsset info.
			int  datasetinfo[] = new int[4];
			String datasetname = new String(" ");
			String[] n = new String[1];
			n[0] = datasetname;
			int  dimsize[]     = new int[32]; /* MAX DIM SIZE? */

			/* the following method need to be fixed it. */
			if (hdf.GRgetiminfo(riid,n,datasetinfo,dimsize)) {

				datasetname = n[0];
			        // HDF GR dataset object  node
				short tag = (short)HDFConstants.DFTAG_RIG;
				short ref = hdf.GRidtoref(riid);
								
				// update node value ===>>> dataset-name instead of "GR dataset"
				node.setName(datasetname); 

				// Also set ref to this node, so it is possible 
				// to retrieve the dataset info.
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

			       // for dataset attributes
			       int datasetAttrNumber = datasetinfo[3];
			       if (datasetAttrNumber > 0) {

				   // HDF GR dataset Attribute object   node
				   node = new HDFObjectNode();
				   node.setObjType(node.GRDATASETATTR);
				   node.setObjLevel(2);

				   node.setName("GR Dataset Attributes");

				   node.setHasParent(true);
				   node.setParentObj(datasetParentNode);
				   node.setHasChild(true);

				   HDFObjectNode datasetAttrParentNode = node;
				   Queue grDatasetAttrQueue    = new Queue();

				   node.setChildObj(grDatasetAttrQueue);

				   // Add gr dataset node
				   grDatasetQueue.put(node);

				   // for each dataset attribute
				   for (int kk=0; kk<datasetAttrNumber; kk++) {
					// get attribute info.
					int[] GRattrInfo = new int[2];
					String attrname = new String(" ");
					n[0] = attrname;
					if (hdf.GRattrinfo(riid, kk, n, GRattrInfo)) {
						attrname = n[0];

					       // HDF GR dataset Attribute object  node
					       node = new HDFObjectNode();
					       node.setTagRef(-1,ref);
					       node.setObjType(node.GRDATASETATTR);
					       node.setObjLevel(3);
					       node.setIndex(kk);
					       node.setName(attrname);
					       node.setHasParent(true);
					       node.setParentObj(datasetAttrParentNode);

					       grDatasetAttrQueue.put(node);
					}
				   } 
				} 
		   	  } 
			  hdf.GRendaccess(riid);
		} 
	     } 
	    hdf.GRend(grid);
	}
  

     /** put the SDS object into HDF queue
       * @param pnode  HDF object node
       * @param filename the HDF file name
       * @param ref  the reference number of the SDS;
       * @param level the hierachal level
       * @param objQueue the queue to store the HDF object node
       * @see HDFObjectNode
       */  

      public void getSDSObject(HDFObjectNode pnode, String filename, int ref, int level, Queue objQueue) throws HDFException {

	    HDFObjectNode   parentNode, childNode, node;
 	    int[] fileInfo = new int[2];


	    int sdid;
	    sdid = hdf.SDstart(filename, HDFConstants.DFACC_RDONLY);	
	    if (sdid == HDFConstants.FAIL) {
		hdf.SDend(sdid);
	        return;
	    }

	    // get information about SDS
	    if (hdf.SDfileinfo(sdid, fileInfo) == false) {
	       hdf.SDend(sdid);
	       return;
	    }
	
	   // add SDS dataset subnode
	   node = new HDFObjectNode();
	   node.setObjType(node.SDSDATASET);
	   node.setObjLevel(level);

	   node.setName("SDS Dataset");

	   node.setHasParent(true);
	   node.setParentObj(pnode);
	   node.setHasChild(true);

	   HDFObjectNode datasetParentNode = node;
	   Queue sdsDatasetQueue         = new Queue();

	   node.setChildObj(sdsDatasetQueue);

	   // Add sds dataset   node
	   objQueue.put(node);

           // select dataset
           int riid ;
           int index;
           index = hdf.SDreftoindex(sdid, ref);
           riid = hdf.SDselect(sdid,index);
           
           if (riid == HDFConstants.FAIL) {
              // terminate the HDF SDS access
              hdf.SDend(sdid);
              return;
           }
           
           // check if the given SDS is the dimension scale
           if (hdf.SDiscoordvar(riid))  {
           
              // terminate the HDF SDS access
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
           int  dimsize[]     = new int[32];
	   if (hdf.SDgetinfo(riid, ss, dimsize, SDInfo)) {
		datasetname = ss[0];

        	// update node value ===>>> dataset-name instead of "SDS dataset"
		node.setName(datasetname);  // how about (sdsname[long_name]);

		int long_name_idx = hdf.SDfindattr(riid, "long_name");

		int [] SDattrInfo = new int[2];
		String name = new String(" ");
		ss[0] = name;
		hdf.SDattrinfo(riid, long_name_idx, ss, SDattrInfo);
		name = ss[0];
		if (long_name_idx != HDFConstants.FAIL) {
			String long_name = new String(" ");
			int attrSize = hdf.DFKNTsize(SDattrInfo[0])* SDattrInfo[1] + 1;
			byte[] buf = new byte[attrSize];
			if (hdf.SDreadattr(riid, long_name_idx, buf)) {
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

	        // for dataset attributes
	        int datasetAttrNumber = SDInfo[2];
	        if (datasetAttrNumber > 0) {

		   // HDF SDS dataset Attribute object   node
		   node = new HDFObjectNode();
		   node.setObjType(node.SDSDATASETATTR);
		   node.setObjLevel(level+1);

		   node.setName("SDS Dataset Attributes");

		   node.setHasParent(true);
		   node.setParentObj(datasetParentNode);
		   node.setHasChild(true);

		   HDFObjectNode datasetAttrParentNode = node;
		   Queue sdsDatasetAttrQueue           = new Queue();

		   node.setChildObj(sdsDatasetAttrQueue);

		   // Add sds dataset node
		   sdsDatasetQueue.put(node);

		   // for each dataset attribute
		   for (int kk=0; kk<datasetAttrNumber; kk++) {
			// get attribute info.
			String attrname = new String(" ");
			ss[0] = attrname;
			if (hdf.SDattrinfo(riid, kk, ss, SDattrInfo)) {
				attrname = ss[0];
			       // HDF SDS dataset Attribute object  node
			       node = new HDFObjectNode();
			       node.setObjType(node.SDSDATASETATTR);
			       node.setTagRef(-1,ref);
			       node.setObjLevel(level+2);
			       node.setIndex(kk);
			       node.setName(attrname);
			       node.setHasParent(true);
			       node.setParentObj(datasetAttrParentNode);

			       sdsDatasetAttrQueue.put(node);
			}
		   } 
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
       * @see HDFObjectNode
       */  

      public void getGRObject(HDFObjectNode pnode, int fid, int ref, int level, Queue objQueue) throws HDFException {

	    HDFObjectNode   parentNode, childNode, node;

	    int grid;
	    grid = hdf.GRstart(fid);	
	    if (grid == HDFConstants.FAIL) {
	       hdf.GRend(grid);
	       return;
	    }

	    // get information about GR
 	    int[] argv = new int[2];
	    if (hdf.GRfileinfo(grid, argv) == false) {
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
	   Queue grDatasetQueue         = new Queue();

	   node.setChildObj(grDatasetQueue);

	   // Add gr dataset   node
	   objQueue.put(node);

	   // select dataset
	   int riid ;
	   int index;

	   /* GRreftoindex() need to be fixed */
	   index = hdf.GRreftoindex(grid, (short)ref);
	   riid  = hdf.GRselect(grid,index);
	   
	   if (riid == HDFConstants.FAIL)
	     return;
	   
	   // get datsset info.
	   int [] imInfo = new int [4];
	   String datasetname = new String(" ");
	   String[] n = new String[1];
	   n[0] = datasetname;
	   int  dimsize[]     = new int[32]; /* MAX DIM SIZE? */

	   if (hdf.GRgetiminfo(riid,n,imInfo,dimsize)) {
		datasetname = n[0];
		// update node value ===>>> dataset-name instead of "GR dataset"
		node.setName(datasetname); 

		// Also set ref to this node, so it is possible 
		// to retrieve the dataset info.
		node.setTagRef(-1,ref);

	       // HDF GR dataset object  node
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
	       if (datasetAttrNumber > 0) {

		   // HDF GR dataset Attribute object   node
		   node = new HDFObjectNode();
		   node.setObjType(node.GRDATASETATTR);
		   node.setObjLevel(level+1);

		   node.setName("GR Dataset Attributes");

		   node.setHasParent(true);
		   node.setParentObj(datasetParentNode);
		   node.setHasChild(true);

		   HDFObjectNode datasetAttrParentNode = node;
		   Queue grDatasetAttrQueue           = new Queue();

		   node.setChildObj(grDatasetAttrQueue);

		   // Add gr dataset node
		   grDatasetQueue.put(node);

		   // for each dataset attribute
		   for (int kk=0; kk<datasetAttrNumber; kk++) {
			// get attribute info.
			int[] GRattrInfo = new int[2];
			String attrname = new String(" ");
			n[0] = attrname;
			if (hdf.GRattrinfo(riid,kk,n,GRattrInfo)) {
			       attrname = n[0];

			       // HDF GR dataset Attribute object  node
			       node = new HDFObjectNode();
			       node.setObjType(node.GRDATASETATTR);
			       node.setTagRef(-1,ref);
			       node.setObjLevel(level+2);
			       node.setIndex(kk);
			       node.setName(attrname);
			       node.setHasParent(true);
			       node.setParentObj(datasetAttrParentNode);

			       grDatasetAttrQueue.put(node);
			}
		   }
		}
	  }
	  hdf.GRendaccess(riid);
   	  hdf.GRend(grid);
    }

   /** return the queue to store the  HDF object mode */ 
   public Queue getR8Object(String filename) throws HDFException  {

	HDFObjectNode   parentNode, childNode, node;
	int argv[] = new int[2];
	boolean argB[] = new boolean[1];
	
	Queue objQueue = new Queue();

	// get raster image number
	int imgNumber = hdf.DFR8nimages(filename);

	if (imgNumber>0) {
	   
	    // HDF Object RIS8 root node
	    node = new HDFObjectNode();	
	    node.setObjType(node.RIS8);
	    node.setObjLevel(0);
	    node.setHasChild(true);
	    Queue childQueue = new Queue();
	    node.setChildObj(childQueue);

	    objQueue.put(node);
	    parentNode = node;

	    for (int i=0; i<imgNumber; i++) {

		// for each raster image
		hdf.DFR8getdims(filename, argv, argB);
		int ref = hdf.DFR8lastref();
		int tag = HDFConstants.DFTAG_RIG;  
	
		// HDF Object RIS8 root node
		node = new HDFObjectNode();	
		node.setObjType(node.RIS8);
		node.setTagRef(tag,ref);
		node.setObjLevel(1);
		node.setHasParent(true);		
		node.setParentObj(parentNode);

		objQueue.put(node);
	   }
	}
	return objQueue;
    }

 

   /**  insert the HDF object belongs to specified HDF Vgroup
     * @param pnode  the HDF object node.
     * @param subQueue the queue to store the HDF object belongs to Vgroup
     * @param fid    the HDF file id.
     * @param tags   the tags value
     * @param refs    the reference number
     * @param npairs  the number of objects belongs to specified Vgroup
     * @param level   the level in the HDF Object
     */

  public  void insertVg(HDFObjectNode pnode, Queue subqueue, 
		int fid, int[] tags, int[] refs, int npairs, int level) 
		throws HDFException {
	
	HDFObjectNode node;

	if (npairs <= 0)
	   return;
	for (int i=0; i<npairs; i++) {

	int tag = tags[i];
	int ref = refs[i];

	switch(tag) {

	// ?????????????????     RIS8 RIS24 GR ?????????????????
	default:
	case HDFConstants.DFTAG_RIG: // Generic Raster Image
		// get GR
	 	//getGRObject(pnode,fid, ref, level, subqueue);
		break;


	// ?????????????????    SDS  ?????????????????
	case HDFConstants.DFTAG_SDG:
	case HDFConstants.DFTAG_NDG: 
	  	// get SDS
		getSDSObject(pnode,fileName, ref, level, subqueue);
		break;

	case HDFConstants.DFTAG_VH: // vdata
	        // create new node
		int vdataid;
		vdataid = hdf.VSattach(fid, ref, "r");
			
		node = new HDFObjectNode();
		node.setObjLevel(level);
		node.setTagRef(tag,ref);

		String s1 = new String(" ");
		String s[] = new String[1];
		s[0] = s1;
		hdf.VSgetname(vdataid,s);
		s1 = s[0];
		String s2 = new String(" ");
		s[0] = s2;
		hdf.VSgetclass(vdataid, s);
		s2 = s[0];
		// set Vdata name
		node.setName( "Vd[ " + s1 + ": " + s2 + " ]" );

		node.setObjType(node.Vdata);
		node.setHasParent(true);
		node.setParentObj(pnode);
		subqueue.put(node);
		// terminate the Vdata access
		hdf.VSdetach(vdataid);
		break;

    	case HDFConstants.DFTAG_VG: // vgroup
			
 		// create new node for vg
		node = new HDFObjectNode();
		node.setObjLevel(level);

		// set Vgroup Name 
		String vgName = new String ("Vgroup[ref: ");
		vgName += (Integer.toString(ref));
		vgName += ("]");

		node.setName(vgName);

		node.setTagRef(tag,ref);
		node.setObjType(node.Vgroup);
		node.setHasParent(true);
		node.setParentObj(pnode);
		subqueue.put(node);
		
		// for vgroup
		int vgroup_id = hdf.Vattach(fid, ref, "r");

		if (vgroup_id != HDFConstants.FAIL) {

		// reset vgroup name
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
		
		if (subnpairs > 0) {
	      	       Queue subsubqueue = new Queue(subnpairs);
	     	       node.setChildObj(subsubqueue);
	     	       node.setHasChild(true);

     		  // recur
     		      insertVg(node, subsubqueue, fid, subtags, subrefs, subnpairs, level+1);
	        }
	        hdf.Vdetach(vgroup_id);
	      }
	      break;
	  }
	} 
   }


  /** Does the HDF file have file label or file description.
     * @return true if the HDF file contains file label or desc. , 
     *         false otherwise
     */
     public boolean  ANhasFileAnn(int fid) throws HDFException {

        int anid = hdf.ANstart(fid);

        if (anid == HDFConstants.FAIL) {
           hdf.ANend(anid);
           return false;
        }
        else {
           int fileInfo[] = new int[4];
           fileInfo[0] = fileInfo[1] = 0;

	if ( hdf.ANfileinfo(anid, fileInfo)){
                if ((fileInfo[0] + fileInfo[1])>0) {
                   hdf.ANend(anid);
                   return true;
                }
                else {
                   hdf.ANend(anid);
                   return false;
                }
           }
           else {
                hdf.ANend(anid);
                return false;
           }
        }
   }
}
