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

import java.awt.*;
import java.awt.image.*;

/**
 * A TreeNode class was written by Sandip Chitale. This class will be used as 
 * a base clas to implemnt a node of the tree data structres like heirarchical
 * file systems.
 *
 * @author  HDF Group, NCSA
 */
public class TreeNode
{
  /** The object is the Generic object. 
     For our HDF it should be HDFObjectNode */
  Object	 hdfObject;

  /** the default icon of the node */
  Image		defaultIcon;

  /** the node level in the tree */ 
  int		objLevel;

  /** the icon of the node when it was expanded */
  Image		expandedIcon;

  boolean	nodeIsExpanded;
  
  /** the icon of the final node */
  Image		fileIcon;

  /** create the new tree node 
   * @param obj the HDF object node
   * @param pDefaultImage   the default image
   * @param pCollapseImage  the collapse image
   */

  public TreeNode(Object obj
		  ,Image	defaultImage
		  ,Image	expandedImage)
  {
    hdfObject		= obj;
    defaultIcon	        = defaultImage;
    objLevel		= 10000;
    expandedIcon	= expandedImage;
    nodeIsExpanded	= false;
  }

  /** create the tree node by object and the default node image
   * @param obj the HDF object node
   * @param the default image
   */

  public TreeNode(Object obj ,Image defaultImage)
  {
    hdfObject		= obj;
    defaultIcon	        = defaultImage;
    objLevel		= 10000;
    expandedIcon	= null;
    nodeIsExpanded	= false;
    fileIcon	        = defaultImage;
  }

  /** add one tree 
   * @param objTree the tree
   */
  public  void  added(HDFTree	objTree) {
  
  }

  // derived class should override this
  public  void   deleted(HDFTree	objTree) {
	
  }

  // derived class should override this
  public  void  select(HDFTree	objTree, int modifiers) {
  
  }

  // derived class should override this
  public  void  expandCollapse(HDFTree objTree, int modifiers) {

    // the node has child
    if (isExpandable()) {      
      toggleExpanded();
    }
  }

  // various accesor fuctions
  public  boolean  isExpandable() {
	
  return(!(expandedIcon == null));
  }

  /** set the collapse image
   * @param expandedImage the collapse image
   */

  public  void  setExpandable(Image expandedImage) {	
    expandedIcon	= expandedImage;
  }

  /** unselect the collapse image */
  public  void  unsetExpandable() {	
    expandedIcon	= null;
  }

  public  boolean  isExpanded() {
    
    return(nodeIsExpanded);
  }

  /** set the node to be expanded */
  public  void  setExpanded() {
	
    if (isExpandable()) {
      
      nodeIsExpanded = true;
    }
  }

  /** set the node not to be expanded */
  public  void   unsetExpanded() {
  
    if (isExpandable()) {
      
      nodeIsExpanded = false;
    }
  }

  public  void toggleExpanded() {
    
    if (isExpanded()) {
      
      unsetExpanded();
    }
    else {
      
      setExpanded();
    }
  }

  /** get the object */
  public Object  getObject() {
  
    return(hdfObject);
  }

  /** get the default image */
  public  Image  getDefaultImage() {
	
    return(defaultIcon);
  }

  /** set the default image */
  public  void  setDefaultImage(Image defaultImage)
  {
    defaultIcon = defaultImage;
  }

  /** set the file image */
  public  void  setFileImage(Image img)
  {
    fileIcon = img;
  }

  /** get the file image */
  public  Image  getFileImage()
  {
    return(fileIcon);
    
  }

  /** get the node level */
  public  int  getLevel()
  {
    return(objLevel);
  }

  /** set the level of the node 
   * @param level the level value of the node 
   */
  public void setLevel(int level)
  {
    objLevel = level;
  }

  /** get the collapse image(expanded).
   *@return the image 
   */
  public  Image getCollapseImage()
  {
    return(expandedIcon);
  }

  /**
   * Converts to a rather lengthy String.
   */
  
  public String toString()
  {
    return(hdfObject.toString());
  }
  

  /** compare the two nodes 
   * @param pOther the tree node 
   * @return true if the two object is the same, or false
   */
  public  boolean   equals(TreeNode pOther)
  {
    return(hdfObject.equals(pOther.getObject()));
  }
}
