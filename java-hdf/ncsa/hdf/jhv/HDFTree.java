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

import	ncsa.hdf.jhv.TreeNode;
import	java.awt.*;
import	java.util.Vector;
import java.awt.event.*;

/**
 * A HDFTree class is to display HDF Tree data structres like heirarchical
 * file systems. The Tree class was written by Sandip Chitale, we make a 
 * change to suite for our HDF file.
 * Upgraded to the JDK 1.1.1b Event Model by Apu.
 *
 * @version 1.02  xlu
 * @author HDF Group, NCSA
 */

public class HDFTree extends Panel implements MouseListener, 
                                              AdjustmentListener { 
                                                                                           
  /** the tree style mask */
  public static	int	TREE_HASLINES	= 1;

  /** the tree style */
  int	      treeStyle;
  
  /** the vector to store the whole tree node */
  Vector      treeVector;

  /** the vector to hold the displayed tree node */
  Vector      displayedVector;

  /** the current selected tree node */
  TreeNode    selectedNode;
	
  /** the maxium width of the panel */
  int	      treeWidth = -1;
		
  // Scrolling related stuff.
  int 	      treeVOffset = 0;
  Scrollbar   treeVScrollbar;
  int 	      treeHOffset = 0;
  Scrollbar   treeHScrollbar;		
	
  // constants related to tree displayed
  public	static int	CELLSIZE	= 30;
  public	static int	CELLOFFSET	= CELLSIZE/2;
  public	static int	IMAGEMARGIN     = 2;
  public	static int	IMAGESIZE	= CELLSIZE-(2 * IMAGEMARGIN);
  public	static int	TRIGGERMARGIN   = 10;
  public	static int	TRIGGERSIZE     = CELLSIZE-(2 *TRIGGERMARGIN);
   
  // variables for duble-buffer
  Image 	offScreenImage = null;
  Graphics 	offGraphics;
 
   // create the new font with the specified font size
   public static   int defaultFontSize = 10;//8;

   // Create the new font for displaying the string correctly 
   // in the specified canvas within the scrollbar
   Font dataFont        = new Font("Fixed", Font.PLAIN, defaultFontSize);

   public static   int fontWidth = 5;
   public static   int fontHeight= 9;

   // shift variables
   int tx=0;
   int ty=0;

  /**create new HDF tree 
   */
  public HDFTree()
  {  
    
    initialize();
   
  }

  

  /** create the new HDF Tree by specified tree style 
   * @param style the tree style  
   */
  public  HDFTree(int style)
  {
    this();
    treeStyle = style;
  }

  /** initialize some of variables related to the HDF Tree */
  public void  initialize()
  {
    treeStyle = 0;

    treeVector      = new Vector();
    displayedVector = new Vector();
    selectedNode    = null;

    // add vertical & horizintal scrollbar
    addScrollbar();	
    addMouseListener(this);
  }

  /** add scrollbar to canvas */
   public void addScrollbar()
   {
     setLayout(new BorderLayout());

     // vertical Scrollbar
     add("East",  treeVScrollbar = new Scrollbar(Scrollbar.VERTICAL));
     treeVScrollbar.addAdjustmentListener(this);
     // horizontal scrollbar
     add("South",  treeHScrollbar = new Scrollbar(Scrollbar.HORIZONTAL));
     treeHScrollbar.addAdjustmentListener(this);
   }

  /** add the tree node 
   * @param node the HDF tree node(HDFObjectNode)
   */
  public  void addTreeNode(TreeNode node)
  {
    //node.setLevel(0);
    treeVector.addElement(node);
    node.added(this);
  }

  /** add the tree node who has a child
  * @param parent the parent of the node
  * @param node the HDF tree node
  */
  public  void  addTreeNode(TreeNode parent, TreeNode node)
  {
    /* get the position of the parent */
    int pos = treeVector.indexOf(parent);
    
    if (pos == -1) { //  No found parent
      
      return;
    }
    /* get the node */
    if (treeVector.indexOf(node) != -1) {
      // node is already in the Vector */
      return;
    }
    /* get the level of the parent */
    int lLevel = parent.getLevel();

    //node.setLevel(((HDFObjectNode)(node.hdfObject)).getObjLevel());
    
    TreeNode lNode;
    int lCount = treeVector.size();
    int i;

    /* for each node */ 
    for (i = pos + 1; i < lCount; i++) {
      
      lNode = (TreeNode) treeVector.elementAt(i);
      if (lLevel == lNode.getLevel()) {
	// the same level 
	break;
      }
    }
    // the last node 
    if (i == lCount) {
      
      treeVector.addElement(node);
    }
    else {
      
      treeVector.insertElementAt(node, i);
    }
    node.added(this);
  }

  /** remove a TreeNode from tree 
   * @param node the HDF tree node 
   */
  public void  removeTreeNode(TreeNode node) 
  {
    int pos = treeVector.indexOf(node);
    
    if (pos == -1) {
      // no found of the tree node
      return;
    }
    /* remove the whole subtree */
    removeSubTreeNodes(node);

    treeVector.removeElementAt(pos);
    node.deleted(this);
    if (selectedNode == node) {
      // the removed node was selected
      selectedNode = null;
    }
  }

  /** remove all TreeNodes from tree 
   */
  public void  removeAllTreeNodes() 
  {
	// counts of tree nodes
	int count=treeVector.size();

	if (count!=0)
	   treeVector.removeAllElements();    

	treeHOffset = 0;
	treeVOffset = 0;	
  }

  /** remove the subtree of a TreeNode
   * @param node the tree node 
   */
  public  void	removeSubTreeNodes(TreeNode node)
  {
    int pos = treeVector.indexOf(node);
    
    if (pos == -1) {
      // node was not found
      return;
    }

    int lLevel = node.getLevel();
    
    TreeNode lNode;
    int lNodeLevel;
    int i;
    // for the rest node
    for (i = pos + 1; i < treeVector.size();) {
      
      lNode = (TreeNode) treeVector.elementAt(i);
      lNodeLevel = lNode.getLevel();
      if (lLevel == lNodeLevel) {
	
	break;
      }

      if (lLevel < lNodeLevel) {
	// remove the subnode of the tree		
	treeVector.removeElementAt(i);
	lNode.deleted(this);
	if (selectedNode == lNode) {
	  // remove the selected node 
	  selectedNode = null;
	}
      } 
    }  
  }

  /** adjust the Scrollbar and repaint the graphics */
  public void  refresh()
  {
  
    /* get the displayed node */
    computedDisplayTree();

    /* have the node displayed */
    invalidate();

    setBounds(getLocation().x, getLocation().y,
	      getSize().width, getSize().height);
    
    // repaint the graphics
    repaint();
  }

  public void  resetTreeOffset() {
  	treeHOffset = 0;
	treeVOffset = 0;
  }

  /** get the displayed tree node */
  protected void computedDisplayTree() {
  
    int			lCount;
    TreeNode		lNode;
    int			lLevel;
    int			lDepth = 0;
    
    /* initialize the displayed vector */
    displayedVector.removeAllElements();
		
    lCount	= treeVector.size();
    for (int i = 0; i < lCount; i++) {
      
      lNode = (TreeNode) treeVector.elementAt(i);
      lLevel = lNode.getLevel();

      // get the displayed node
      if (lLevel <= lDepth) {
	// add one node
	displayedVector.addElement(lNode);
	
	lDepth = lLevel;
	if (lNode.isExpandable()) {
	  // has child?
	  if (lNode.isExpanded()) {
	    // has been expanded
	    lDepth++;
	  }
	  else {
	    // nothing to do
	    ;
	  }
	}
	else {
	  // nothing to do
	  ;
	}
      }
    } 
  }

    public Dimension getMinimumSize() {
        return new Dimension(10, 10);
    }

 

    public Dimension getPreferredSize() {
        return getMinimumSize();
    }

 

   public void adjustmentValueChanged(AdjustmentEvent e)
   {
     int arg = e.getAdjustmentType();
     Scrollbar target = (Scrollbar) e.getAdjustable();
    
     if (target != null) {
	
       // detect the vertival scrollbar
       if (target ==  treeVScrollbar) {  
	
	 switch(arg) {
	   
	 case AdjustmentEvent.TRACK:
	 case AdjustmentEvent.BLOCK_INCREMENT:
	 case AdjustmentEvent.BLOCK_DECREMENT:
	 case AdjustmentEvent.UNIT_DECREMENT:
	 case AdjustmentEvent.UNIT_INCREMENT:
	   treeVOffset = target.getValue();
	 }	  
	 
	 // ty = treeVOffset;
	 
	 // repaint the graphics
	 update(getGraphics());
       }
	
         // detect the horizontal scrollbar
       if (target ==  treeHScrollbar) {
	 
	 switch(arg) {
	 case AdjustmentEvent.TRACK:
	 case AdjustmentEvent.BLOCK_INCREMENT:
	 case AdjustmentEvent.BLOCK_DECREMENT:
	 case AdjustmentEvent.UNIT_DECREMENT:
	 case AdjustmentEvent.UNIT_INCREMENT:
	   treeHOffset = target.getValue();
	 }
	 
 	  tx = treeHOffset;
	  
 	  // adjust the horizontal scrollbar
 	  setHScrollValue();
	  
 	  // repaint the graphics
 	  update(getGraphics());
 	}
     }
   }

  public void mouseClicked(MouseEvent e)
  {
    int x = e.getX();
    int y = e.getY();

    //    Point offsets = this.getScrollPosition();
    
    // treeVOffset = offsets.y;
    // treeHOffset = offsets.x;

    int lIndex = (y / CELLSIZE) + treeVOffset;

    // get the valid node
    if (lIndex < displayedVector.size()) {
      
      TreeNode lNode = (TreeNode) displayedVector.elementAt(lIndex);
      selectedNode   = lNode;
      int	lLevel = lNode.getLevel();
      Rectangle lRect  = new Rectangle((lLevel * CELLSIZE) + TRIGGERMARGIN
				      ,((y/CELLSIZE)*CELLSIZE)+ TRIGGERMARGIN
				      ,TRIGGERSIZE
				      ,TRIGGERSIZE);
      // point is "inside" rectangle
      if (lRect.contains(x, y)) {
	// has child
	if (lNode.isExpandable()) {
	  // make the icon changed
	  lNode.expandCollapse(this, 0);
	  refresh();
	}
      }
      else {
	// has child
	if (lNode.isExpandable()) {
	  // make the icon changed
	  lNode.expandCollapse(this, 0);
	  refresh();
	}
	else {
   	      // will do something for HDF
	 
	      lNode.select(this, 0);
	      repaint();
       	}
      } 
    }
  }

  public void mousePressed(MouseEvent e){}

  public void mouseReleased(MouseEvent e){}

  public void mouseEntered(MouseEvent e){}

  public void mouseExited(MouseEvent e){}

  /** reshap */
  public synchronized  void setBounds(int x, int y, int w, int h)   { 
    super.setBounds(x, y, w, h);
    
        if (getSize().width > CELLSIZE) {
      
          treeVScrollbar.setValues(treeVOffset
    			       ,(getSize().height/CELLSIZE)
    			       ,0
    			       ,displayedVector.size());
      
          treeVScrollbar.setBlockIncrement(getSize().height/CELLSIZE);
      
          // resize horizontal scrollbar
        setHScrollValue();
        }
  }
  
  

    /** Adjust the Horizontal Scrollbar value by the specifyed tree width. */
    void setHScrollValue() {

	// get current canvas size
        int canvasWidth = getSize().width;

	// canvas is valid?
        if (canvasWidth <= 0) {
            return;
        }

        //Shift everything to the right if we're displaying empty space
        //on the right side.
        if ((tx + canvasWidth) > treeWidth) {
            int newtx = treeWidth - canvasWidth;
            if (newtx < 0) {
                newtx = 0;
            }
            tx = newtx;
        }

int p = (int)(canvasWidth * 0.9);
int m = (int)(treeWidth - (canvasWidth - p));
          treeHScrollbar.setValues(//draw the part of the image that starts at this x:
	               tx, 
                       //amount to scroll for a "page":
	               p, //(int)(canvasWidth * 0.9), 
                       //minimum image x to specify:
	               0,
	               //maximum image x to specify:
	                     m); //treeWidth - canvasWidth);
	  //"visible" arg to setValues() has no effect after scrollbar is visible.
  treeVScrollbar.setBlockIncrement(p);//(int)(canvasWidth * 0.9));
        return;
    }

  /** 
    * Paints the component.
    * @param pg the specified Graphics window
    * 
    */
  public void paint(Graphics gc)
  {
    int			lCount;
    TreeNode		lNode;
    int			lLevel     = -1;
    int			lPrevLevel = -1;
    Image		lImage;
    int			lWidth;	
   
    // set the specified translated parameters 
    // and the subcomponents will be relative to this origin.
    gc.translate(-tx, -ty);

    //treeWidth = -1;	
    gc.setColor(Color.gray); 
	
    FontMetrics lFM = gc.getFontMetrics();

    // draw the frame area
    gc.drawRect(0,0, getSize().width-1, getSize().height-1);

    gc.setColor(Color.gray);

    if ((treeStyle & TREE_HASLINES) != 0) {
      
      int j = (getSize().height/CELLSIZE) + 1;
      for (int ii = 0; ii < j; ii++) {
	
	int i = ii - treeVOffset;
	gc.drawRect(0
		    ,(i * CELLSIZE)
		    ,getSize().width
		    ,CELLSIZE);
      }
    }

    lCount	= displayedVector.size();
    for (int ii = 0; ii < lCount; ii++) {
      
      int i = ii - treeVOffset;
      lNode = (TreeNode) displayedVector.elementAt(ii);
      lPrevLevel = lLevel;
      lLevel = lNode.getLevel();
      if (lNode == selectedNode) {
	// this object node has been selected
	gc.setColor(Color.black);
	gc.fillRect(((lLevel + 2) * CELLSIZE)
		    ,(i * CELLSIZE)
		    ,lFM.stringWidth(lNode.toString()) + (2 * TRIGGERMARGIN)
		    ,CELLSIZE);
      }
      
      gc.setColor(Color.gray);
      // draw line between nodes
      gc.drawLine((lLevel * CELLSIZE) + (CELLSIZE/2)
		  ,(i * CELLSIZE) + CELLSIZE/2
		  ,((lLevel + 1) * CELLSIZE) + CELLSIZE/2
		  ,(i * CELLSIZE) + CELLSIZE/2);

      if (ii + 1 < lCount) {
	// not the last node to be displayed	
	if (((TreeNode) displayedVector.elementAt(ii + 1)).getLevel()>=lLevel){
	  // if the level of the next node is not same as the current, 
	  // change to next level
	  if (((TreeNode) displayedVector.elementAt(ii+1)).getLevel()>lLevel) {
	    // draw the vertical line
	    gc.drawLine(((lLevel + 1) * CELLSIZE) + (CELLSIZE/2)
			,(i * CELLSIZE) + CELLSIZE/2
			,((lLevel + 1) * CELLSIZE) + CELLSIZE/2
			,((i+1) * CELLSIZE) + CELLSIZE/2);
	  }
		
	  int j, k;
	  for (j = ii + 1, k = -1; j < lCount; j++) {
	    // same level
	    if (((TreeNode) displayedVector.elementAt(j)).getLevel()==lLevel){
	      
	      k = j;
	      break;
	    }

	    if (((TreeNode) displayedVector.elementAt(j)).getLevel()<lLevel){
	      
	      break;
	    }
	  }

	  if (j < lCount) {
	    
	    if (k != -1) {
	      
	      gc.drawLine((lLevel * CELLSIZE) + (CELLSIZE/2)
			  ,(i * CELLSIZE) + CELLSIZE/2
			  ,((lLevel) * CELLSIZE) + CELLSIZE/2
			  ,((k - treeVOffset) * CELLSIZE) + CELLSIZE/2);
	    }
	  } 
	}  
      }   
      
      // draw the rectangle
      if (lNode.isExpandable()) {
	// has child
	gc.setColor(Color.white);
	gc.fillRect((lLevel * CELLSIZE) + TRIGGERMARGIN
		    ,(i * CELLSIZE) + TRIGGERMARGIN
		    ,TRIGGERSIZE
		    ,TRIGGERSIZE);

	//gc.setColor(Color.black);
	gc.setColor(Color.red);

	// set the edge of the rectangle
	gc.drawRect((lLevel * CELLSIZE) + TRIGGERMARGIN
		    ,(i * CELLSIZE) + TRIGGERMARGIN
		    ,TRIGGERSIZE
		    ,TRIGGERSIZE);

	gc.setColor(Color.black);
	//pg.setColor(Color.green);
	// draw horizonal line within the rectangle		
	gc.drawLine((lLevel * CELLSIZE) + TRIGGERMARGIN 
		    ,(i * CELLSIZE) + TRIGGERMARGIN + (TRIGGERSIZE/2)
		    ,(lLevel * CELLSIZE) + TRIGGERMARGIN + TRIGGERSIZE
		    ,(i * CELLSIZE) + TRIGGERMARGIN + (TRIGGERSIZE/2));
      }

      gc.setColor(Color.black);
      if (lNode.isExpanded()) {
	// get the default image & node has been expanded
	lImage = lNode.getCollapseImage();
      }
      else {
	
	if (lNode.isExpandable()) {
	  //pg.setColor(Color.green);
	  // draw the vertical line within the rectangle					
	  gc.drawLine((lLevel * CELLSIZE) + TRIGGERMARGIN + (TRIGGERSIZE/2)
		      ,(i * CELLSIZE) + TRIGGERMARGIN
		      ,(lLevel * CELLSIZE) + TRIGGERMARGIN + (TRIGGERSIZE/2)
		      ,(i * CELLSIZE) + TRIGGERMARGIN + TRIGGERSIZE);
	  lImage = lNode.getDefaultImage();  
	}
	else
	  lImage = lNode.getFileImage();
	}
      
      // draw the icon
      gc.drawImage(lImage
		   ,((lLevel + 1) * CELLSIZE) + IMAGEMARGIN
		   ,(i * CELLSIZE) + IMAGEMARGIN
		   ,IMAGESIZE
		   ,IMAGESIZE
		   ,this);
      // get the selected node to be displayed correctly
      if (lNode == selectedNode) {
	
	gc.setColor(Color.white);
      }
      else {
	
	gc.setColor(Color.black);
      }
      
      // draw the string
      gc.drawString(lNode.toString()
		    ,((lLevel + 2) * CELLSIZE) + TRIGGERMARGIN
		    ,((i + 1) * CELLSIZE) - TRIGGERMARGIN);

      // current canvas width
      lWidth = ((lLevel + 2) * CELLSIZE) + lFM.stringWidth(lNode.toString())
	       + (2 * TRIGGERMARGIN);

      if (treeWidth < lWidth)
	treeWidth = lWidth;
      
    } 

    // set the specified translated parameters 
    // and the subcomponents will be relative to this origin.
    gc.translate(tx, ty);

  }

/**
     * Updates the component. This method is called in
     * response to a call to repaint. You can assume that
     * the background is not cleared.
     * @param g the specified Graphics window
     * @see java.awt.Component#update
     */
    public void update(Graphics g) {

	Dimension d = getSize();

    	if (offScreenImage == null) {
	    // offScreenImage not created; create it.
	    offScreenImage = createImage(d.width*5, d.height*5);	

	    // get the off-screen graphics context    
	    offGraphics    = offScreenImage.getGraphics();
	
	    // set the font for offGraphics
	    offGraphics.setFont(getFont());	 
	}

	// paint the background on the off-screen graphics context
	offGraphics.setColor(getBackground());
	offGraphics.fillRect(0,0,d.width,d.height);    
	offGraphics.setColor(getForeground());


	// draw the current frame to the off-screen 
	paint(offGraphics);

	//then draw the image to the on-screen 
	g.drawImage(offScreenImage, 0, 0, null);
	
    }
}
