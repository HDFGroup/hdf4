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
import java.awt.Rectangle;
import ncsa.hdf.util.Queue;

/** This class define the contants for each HDF object.
    * @author HDF group, NCSA
    */

  public class HDFObjectNode implements java.io.Serializable, Cloneable{
	
	/** define the type of HDF object */
	public static final  int    Annotation =  1;
	public static final  int    Palette    = Annotation + 1;
	public static final  int    RIS8       = Annotation + 2;
	public static final  int    RIS24      = Annotation + 3;
	public static final  int    GR         = Annotation + 4;
	// for GR object
	public static final  int    GRGLOBALATTR     = GR*10 +1;
	public static final  int    GRDATASETATTR    = GR*10 +2;
	public static final  int    GRDATASET        = GR*10 +3;
	public static final  int    GRDATASETAN      = GR*10 +4;

	public static final  int    SDS        = Annotation + 5;
	// for sds object
	public static final  int    SDSGLOBALATTR     = SDS*10 +1;
	public static final  int    SDSDATASETATTR    = SDS*10 +2;
	public static final  int    SDSDATASET        = SDS*10 +3;
	public static final  int    SDSDATASETAN      = SDS*10 +4;
	
	public static final  int    Vdata      = Annotation + 6;
     	public static final  int    Vgroup     = Annotation + 7;	

	/** the tag of the object */
	public   int tag;

	/** the reference number of the object */
	public   int ref;

	/** the type of the object */
	public   int type;

	/** the level of the object */
	public   int level;

	/** the index of the attributes */
	public   int index;

	/**  the name of the node */
	String   name;

	/** the expandable of the object */
	public   boolean hasChild;

	/** the ancestor  of the object */
	public   boolean hasParent;
	
	/** define the parent object */
	public HDFObjectNode   parent;

	/** define the child object */
	public Queue   child;

        // added by Peter Cao on 10-3-97 for remote access
        /** text information about the node */
        public String information = "";

        /** true if only read preview data */
        public boolean isPreview = true;

        /** the selected image range */
        public Rectangle selectedRange = null;

        /** prefered image size */
        public int preferedImageSize = 256;

        /** the image plane number for animated image */
        public int plane = 1;

        /** the orientation and slice selection of SDS image */
        public int sdsSelection[] = null;

   /** the new HDF object */

    public HDFObjectNode() {
	tag = -1;
	ref = -1;
	type= -1;
	level = 0;
	index = -1;
	name = null;
	hasChild = false;
	hasChild = false;
	parent = null;
	child = null;
        selectedRange = new Rectangle();
    }

   /** set object type 
     * @param obj_type the object's type
     */

    public void setObjType(int obj_type) {
	
	this.type = obj_type;
    }

   /** get object type 
     * @return the object type
     */

    public int getObjType() {
	
	return this.type;
	
    }

   /** set object level
     * @param obj_level the object's level
     */

    public void setObjLevel(int obj_level) {
	
	this.level = obj_level;
	
    }

   /** get  object level
     * @return the object's level
     */

    public int getObjLevel() {
	
	return this.level;
	
    }

   /** set   object name
     *  
     */

    public void setName(char name[]) {

	String tmpStr = new String(name);

	this.name = new String(tmpStr.trim());
	
    }

   /** set   object name
     *  
     */

    public void setName(String name) {
	
	this.name = new String(name);
	
    }

   /** set   object index value when node is attribute.
     *  
     */

    public void setIndex(int idx) {
	
	this.index = idx;
	
    }

   /** get    object index value when node is attribute.
     *  @return the index value of the attribute
     */

    public int  getIndex() {
	
	return(this.index);
	
    }

   /** get   object name
     * @return the node name
     */

    public String getName() {
	
	return(this.name);
	
    }

   /** set object tag and ref 
     * @param tag  the object's tag
     * @param ref the object's reference
     */

    public void setTagRef(int tag, int ref) {
	
	this.tag = tag;
	this.ref = ref;
	
    }

   /** get object tag    
     * @return the object's tag
     */

    public int getObjTag()  {
	return this.tag;
	
    }

   /** get object reference    
     * @return the object's reference number
     */

    public int getObjRef()  {
	return this.ref;
	
    }

   /** set expandable  flag 
     * @param hasChild  the value defined object's child
     */

    public void setHasChild(boolean hasChild) {
	
	this.hasChild = hasChild;
	
    }

  

  /** test if object has child
     * @return true if the object has child associated with it
     */

    public boolean hasChild() {
	
	if (hasChild)
	   return true;
	else
	  return false;
	
    }

   /** set ancestor  flag 
     * @param hasParent  the value defined this object's ancestor
     */

    public void setHasParent(boolean hasParent) {
	
	this.hasParent = hasParent;
	
    }

   /** test if object has prent
     * @return true if the object has parent associated with it
     */

    public boolean hasParent() {
	
	if (hasParent)
	   return true;
	else
	  return false;
	
    }

   /** set child object 
     * @param obj the HDF object
     */

    public void setChildObj(Queue que) {
	
	this.child = que;
    }

   /** get child object 
     * @return the HDF object
     */

    public Queue getChildObj() {
	
	return this.child ;
	
    }

   /** set parent object 
     * @param obj the HDF object
     */

    public void setParentObj(HDFObjectNode obj) {

	this.parent = obj;

    }

   /** get parent object
     * @return the HDF object
     */

    public HDFObjectNode getParentObj() {
	
	return(this.parent);
	
    }

   // determine if node is readable, if not take it off from the tree
  public  boolean isReadable() {

	boolean retValue = true;

	String nodeName = getName();

	// Part of SDS for current node. ?
	if ( (nodeName.indexOf("CDF0.0")	!=-1)  || 
	     (nodeName.indexOf("Attr0.0")	!=-1)  ||
	     (nodeName.indexOf("Var0.0")	!=-1)  || 
	     (nodeName.indexOf("UDim0.0")	!=-1)  ||
 	     (nodeName.indexOf("Dim0.0")	!=-1)  ||
	     (nodeName.indexOf("DimVal0.0")     !=-1)  ||
     	     (nodeName.indexOf("DimVal0.1")     !=-1)  ||
	     (nodeName.indexOf("Data0.0")	!=-1)  ||
	     (nodeName.indexOf("_HDF_CHK_TBL_0")!=-1) )

	   retValue =  false;
	
	return retValue;
  }

   /**
     * Converts the vector to a string. Useful for debugging.
     */
    public final synchronized String toString() {
	String str = new String();

	switch(this.type) {
	case this.Annotation:
	    str = "Ann";
	    break;
	case this.Palette:
	    str = "Pal";
	    break;
	case this.RIS8:
	    str = "R8";
	    break;
	case this.RIS24:
	    str = "R24";
	    break;
	case GR:
	    str = "GR";
	    break;
	case this.GRGLOBALATTR:
	    str = "GRgattr";
	    break;
	case this.GRDATASET:
	    str = "GRimg";
	    break;

	case this.GRDATASETATTR:
	    str = "GRdattr";
	    break;
	case this.SDS:
	    str = "SDS";
	    break;
	case this.SDSGLOBALATTR:
	    // str = "SDSgattr";
	    str = "";
	    break;
	case this.SDSDATASETATTR:
	    // str = "SDSdattr";
	    str = "";
	    break;
	case this.SDSDATASET:
	    // str = "SDSdata";
	    str = "";
	    break;
	case this.Vdata:
	    str = "Vdata";
	    break;
	case this.Vgroup:
	    str = "Vgroup";
	    break;
	}
	
	str = this.name;
	return (str);
    }

}
