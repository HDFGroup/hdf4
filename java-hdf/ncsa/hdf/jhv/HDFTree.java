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
import java.util.Vector;
import java.awt.event.*;
import java.lang.reflect.Method;

/**
 * A HDFTree class is to display HDF Tree data structres like heirarchical
 * file systems. The Tree class was written by Sandip Chitale, we make a 
 * change to suite for our HDF file.
 * Upgraded to the JDK 1.1.1b Event Model by Apu.
 *
 * @version 1.02  xlu
 * @author HDF Group, NCSA
 */

public class HDFTree extends Panel implements MouseListener, AdjustmentListener
{ 
                                                                                           
    /** the tree style mask */
    public static int TREE_HASLINES        = 1;

    // constants related to tree displayed
    public static int CELLSIZE = 30;
    public static int CELLOFFSET = CELLSIZE/2;
    public static int IMAGEMARGIN = 2;
    public static int IMAGESIZE = CELLSIZE-(2 * IMAGEMARGIN);
    public static int TRIGGERMARGIN = 10;
    public static int TRIGGERSIZE = CELLSIZE-(2 *TRIGGERMARGIN);

    public static   int defaultFontSize = 10;//8;
    public static   int fontWidth = 5;
    public static   int fontHeight= 9;

    /** the tree style */
    int treeStyle;
  
    /** the vector to store the whole tree node */
    Vector treeVector;

    /** the vector to hold the displayed tree node */
    Vector displayedVector;

    /** the current selected tree node */
    TreeNode selectedNode;
        
    /** the maxium width of the panel */
    int treeWidth = -1;
                
    // Scrolling related stuff.
    int treeVOffset = 0;
    Scrollbar treeVScrollbar;
    int treeHOffset = 0;
    Scrollbar treeHScrollbar;                
        
    // variables for duble-buffer
    Image offScreenImage = null;
    Graphics offGraphics;
 
    // Create the new font for displaying the string correctly 
    // in the specified canvas within the scrollbar
    Font dataFont = new Font("Fixed", Font.PLAIN, defaultFontSize);

    // shift variables
    int tx=0;
    int ty=0;

    /** create new HDF tree */
    public HDFTree() { initialize(); }

    /**
     * create the new HDF Tree with specified tree style 
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
        treeVector = new Vector();
        displayedVector = new Vector();
        selectedNode = null;

        // add vertical & horizintal scrollbar
        addScrollbar();
        addMouseListener(this);
    }

    /** add scrollbar to canvas */
    public void addScrollbar()
    {
        setLayout(new BorderLayout());

        // add Scrollbars
        add("East",  treeVScrollbar = new Scrollbar(Scrollbar.VERTICAL));
        treeVScrollbar.addAdjustmentListener(this);
        add("South",  treeHScrollbar = new Scrollbar(Scrollbar.HORIZONTAL));
        treeHScrollbar.addAdjustmentListener(this);
    }

    /** add the tree node 
     * @param node the HDF tree node(HDFObjectNode)
     */
    public  void addTreeNode(TreeNode node)
    {
        treeVector.addElement(node);
        node.added(this);
    }

    /** add the tree node who has a child
     * @param parent the parent of the node
     * @param node the HDF tree node
     */
    public  void  addTreeNode(TreeNode parent, TreeNode node)
    {
        int pos = treeVector.indexOf(parent);
        if (pos <0 )  return; // no parent

        if (treeVector.indexOf(node) >= 0)
            return; // node is already in the Vector */

        int lLevel = parent.getLevel();

        TreeNode lNode;
        int lCount = treeVector.size();
        int i;

        // find the position to add the node 
        for (i = pos + 1; i < lCount; i++) {
            lNode = (TreeNode) treeVector.elementAt(i);
            if (lLevel == lNode.getLevel())
                break; // the same level 
        }

        // the last node 
        if (i == lCount)
            treeVector.addElement(node);
        else
            treeVector.insertElementAt(node, i);

        node.added(this);
    }

    /** remove a TreeNode from the tree 
     * @param node the HDF tree node 
     */
    public void  removeTreeNode(TreeNode node) 
    {
        int pos = treeVector.indexOf(node);

        // the node is not found in the tree    
        if (pos == -1) return; 

        // remove the whole subtree
        removeSubTreeNodes(node);

        treeVector.removeElementAt(pos);
        node.deleted(this);

        if (selectedNode == node)
            selectedNode = null;
    }

    /** remove all TreeNodes from tree */
    public void  removeAllTreeNodes() 
    {
        int count=treeVector.size();

        if (count != 0)
           treeVector.removeAllElements();    

        treeHOffset = 0;
        treeVOffset = 0;        
    }

    /** remove the subtree of a TreeNode
     * @param node the tree node 
     */
    public void removeSubTreeNodes(TreeNode node)
    {
        int pos = treeVector.indexOf(node);
    
        // the node is not found in the tree    
        if (pos == -1) return; 

        int lLevel = node.getLevel();
    
        TreeNode lNode;
        int lNodeLevel;
        int i;

        // for the rest node
        for (i = pos + 1; i < treeVector.size();)
        {
            lNode = (TreeNode) treeVector.elementAt(i);
            lNodeLevel = lNode.getLevel();

            if (lLevel == lNodeLevel) break;

            if (lLevel < lNodeLevel)
            {
                // remove the subnode of the tree                
                treeVector.removeElementAt(i);
                lNode.deleted(this);

                if (selectedNode == lNode) selectedNode = null;
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

        setBounds(getLocation().x,getLocation().y,getSize().width, getSize().height);
    
        // repaint the graphics
        repaint();
    }

    public void  resetTreeOffset() {
        treeHOffset = 0;
        treeVOffset = 0;
    }

    /** get the displayed tree node */
    protected void computedDisplayTree()
    {
  
        int lCount;
        TreeNode lNode;
        int lLevel;
        int lDepth = 0;
    
        /* initialize the displayed vector */
        displayedVector.removeAllElements();
                
        lCount = treeVector.size();
        for (int i = 0; i < lCount; i++)
        {
            lNode = (TreeNode) treeVector.elementAt(i);
            lLevel = lNode.getLevel();

            // get the displayed node
            if (lLevel <= lDepth) {
                // add one node
                displayedVector.addElement(lNode);
                lDepth = lLevel;

                if (lNode.isExpandable() && lNode.isExpanded())
                    lDepth++;
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
        //System.out.println("HDFTree.adjustmentValueChanged()");

        int arg = e.getAdjustmentType();
        Scrollbar target = (Scrollbar) e.getAdjustable();

        if (target == null) return;
        
        // detect the vertival scrollbar
        if (target ==  treeVScrollbar)
        {  
            switch(arg)
            {
                case AdjustmentEvent.TRACK:
                case AdjustmentEvent.BLOCK_INCREMENT:
                case AdjustmentEvent.BLOCK_DECREMENT:
                case AdjustmentEvent.UNIT_DECREMENT:
                case AdjustmentEvent.UNIT_INCREMENT:
                    treeVOffset = target.getValue();
            }          
            update(getGraphics());
        }
        
        // detect the horizontal scrollbar
        else if (target ==  treeHScrollbar)
        {
            switch(arg)
            {
                case AdjustmentEvent.TRACK:
                case AdjustmentEvent.BLOCK_INCREMENT:
                case AdjustmentEvent.BLOCK_DECREMENT:
                case AdjustmentEvent.UNIT_DECREMENT:
                case AdjustmentEvent.UNIT_INCREMENT:
                    treeHOffset = target.getValue();
            }
         
            tx = treeHOffset;
            setHScrollValue();
            update(getGraphics());
        }
    }

    public void mouseClicked(MouseEvent e)
    {
        int x = e.getX();
        int y = e.getY();
        int lIndex = (y / CELLSIZE) + treeVOffset;

        if (lIndex >= displayedVector.size()) return;

        TreeNode lNode = (TreeNode) displayedVector.elementAt(lIndex);
        selectedNode   = lNode;
        int lLevel = lNode.getLevel();
        Rectangle lRect  = new Rectangle((lLevel * CELLSIZE) + TRIGGERMARGIN,
            ((y/CELLSIZE)*CELLSIZE)+ TRIGGERMARGIN ,TRIGGERSIZE ,TRIGGERSIZE);

        // dynamically invoke the methods of TreeNode subclasses
        Class nodeClass = lNode.getClass();
        Class[] params = {this.getClass()};
        Object[] args = {this};
        Method expandCollapse=null, select=null;
        try {
            expandCollapse = nodeClass.getDeclaredMethod("expandCollapse", params);
            select = nodeClass.getDeclaredMethod("select", params);
        }
        catch (Exception ex) {};

        if (lNode.isExpandable())
        {
            try {expandCollapse.invoke(lNode, args);}
            catch (Exception ex) {}
            //lNode.expandCollapse(this);
            refresh();
        }
        else if (!lRect.contains(x, y))
        {
            try {select.invoke(lNode, args);}
            catch (Exception ex) {}
            //lNode.select(this);
            repaint();
        }
    }

    public void mousePressed(MouseEvent e){}
    public void mouseReleased(MouseEvent e){}
    public void mouseEntered(MouseEvent e){}
    public void mouseExited(MouseEvent e){}

    public synchronized  void setBounds(int x, int y, int w, int h)
    { 
        //System.out.println("HDFTree.setBounds()");

        super.setBounds(x, y, w, h);
        Dimension d = getSize();
        int visible = d.height/CELLSIZE;
        int treeSize = displayedVector.size();

        if (d.width > CELLSIZE)
        {
            // fix the bug that the vertical scrollbar does not roll up
            // if the visible tree size is samller than tree panel size
            // while the top part of the tree is hidden
            // Peter Cao, June 8, 1998
            //treeVScrollbar.setValues(treeVOffset,visible, 0, treeSize);
            treeVScrollbar.setValues(treeVOffset,visible, 0,
                treeVOffset+Math.max(treeSize,visible));
            treeVScrollbar.setBlockIncrement(visible);
      
            // resize horizontal scrollbar
            setHScrollValue();
        }
    }

    /** Adjust the Horizontal Scrollbar value by the specifyed tree width. */
    void setHScrollValue()
    {
        // get current canvas size
        int canvasWidth = getSize().width;

        // canvas is valid?
        if (canvasWidth <= 0) return;

        //Shift everything to the right for empty space
        if ((tx + canvasWidth) > treeWidth) {
            int newtx = treeWidth - canvasWidth;
            if (newtx < 0)  newtx = 0;
            tx = newtx;
        }

        int p = (int)(canvasWidth * 0.9);
        int m = (int)(treeWidth - (canvasWidth - p));
        treeHScrollbar.setValues(tx, p, 0, m);

        //"visible" arg to setValues() has no effect after scrollbar is visible.
        treeVScrollbar.setBlockIncrement(p);
        return;
    }

    /** 
     * Paints the component.
     * @param pg the specified Graphics window
     */
    public void paint(Graphics gc)
    {
        int lCount;
        TreeNode lNode;
        int lLevel     = -1;
        int lPrevLevel = -1;
        Image lImage;
        int lWidth;        
   
        gc.translate(-tx, -ty);
        gc.setColor(Color.gray); 
        FontMetrics lFM = gc.getFontMetrics();
        gc.drawRect(0,0, getSize().width-1, getSize().height-1);
        gc.setColor(Color.gray);

        if ((treeStyle & TREE_HASLINES) != 0)
        {
            int j = (getSize().height/CELLSIZE) + 1;
            for (int ii = 0; ii < j; ii++) {
                int i = ii - treeVOffset;
                gc.drawRect(0 ,(i * CELLSIZE) ,getSize().width ,CELLSIZE);
            }
        }

        lCount = displayedVector.size();
        for (int ii = 0; ii < lCount; ii++)
        {
            int i = ii - treeVOffset;
            lNode = (TreeNode) displayedVector.elementAt(ii);
            lPrevLevel = lLevel;
            lLevel = lNode.getLevel();

            if (lNode == selectedNode)
            {
                // this object node has been selected
                gc.setColor(Color.black);
                gc.fillRect(((lLevel + 2) * CELLSIZE) ,(i * CELLSIZE)
                    ,lFM.stringWidth(lNode.toString()) + (2 * TRIGGERMARGIN)
                    ,CELLSIZE);
            }
      
            gc.setColor(Color.gray);
            // draw line between nodes
            gc.drawLine((lLevel * CELLSIZE) + (CELLSIZE/2)
                  ,(i * CELLSIZE) + CELLSIZE/2
                  ,((lLevel + 1) * CELLSIZE) + CELLSIZE/2
                  ,(i * CELLSIZE) + CELLSIZE/2);

            if (ii + 1 < lCount)
            {
                // not the last node to be displayed        
                if (((TreeNode) displayedVector.elementAt(ii + 1)).getLevel()>=lLevel)
                {
                    // if the level of the next node is not same as the current, 
                    // change to next level
                    if (((TreeNode) displayedVector.elementAt(ii+1)).getLevel()>lLevel)
                    {
                        // draw the vertical line
                        gc.drawLine(((lLevel + 1) * CELLSIZE) + (CELLSIZE/2)
                            ,(i * CELLSIZE) + CELLSIZE/2
                            ,((lLevel + 1) * CELLSIZE) + CELLSIZE/2
                            ,((i+1) * CELLSIZE) + CELLSIZE/2);
                    }
                
                    int j, k;
                    for (j = ii + 1, k = -1; j < lCount; j++)
                    {
                        // same level
                        if (((TreeNode) displayedVector.elementAt(j)).getLevel()==lLevel)
                        {
                            k = j;
                            break;
                        }
                        if (((TreeNode) displayedVector.elementAt(j)).getLevel()<lLevel)
                            break;
                    }

                    if (j < lCount && k != -1)
                    {
                        gc.drawLine((lLevel * CELLSIZE) + (CELLSIZE/2)
                            ,(i * CELLSIZE) + CELLSIZE/2
                            ,((lLevel) * CELLSIZE) + CELLSIZE/2
                            ,((k - treeVOffset) * CELLSIZE) + CELLSIZE/2);
                    } 
                }  
            }   

            // draw the rectangle
            if (lNode.isExpandable())
            {
                gc.setColor(Color.white);
                gc.fillRect((lLevel * CELLSIZE) + TRIGGERMARGIN
                    ,(i * CELLSIZE) + TRIGGERMARGIN
                    ,TRIGGERSIZE ,TRIGGERSIZE);
                gc.setColor(Color.red);
                gc.drawRect((lLevel * CELLSIZE) + TRIGGERMARGIN
                    ,(i * CELLSIZE) + TRIGGERMARGIN
                    ,TRIGGERSIZE ,TRIGGERSIZE);
                gc.setColor(Color.black);
                gc.drawLine((lLevel * CELLSIZE) + TRIGGERMARGIN 
                    ,(i * CELLSIZE) + TRIGGERMARGIN + (TRIGGERSIZE/2)
                    ,(lLevel * CELLSIZE) + TRIGGERMARGIN + TRIGGERSIZE
                    ,(i * CELLSIZE) + TRIGGERMARGIN + (TRIGGERSIZE/2));
            }

            gc.setColor(Color.black);
            if (lNode.isExpanded()) 
                lImage = lNode.getCollapseImage();
            else
            {
                if (lNode.isExpandable())
                {
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
            gc.drawImage(lImage, ((lLevel + 1) * CELLSIZE) + IMAGEMARGIN
                ,(i * CELLSIZE) + IMAGEMARGIN,IMAGESIZE, IMAGESIZE, this);

            // get the selected node to be displayed correctly
            if (lNode == selectedNode) gc.setColor(Color.white);
            else gc.setColor(Color.black);
      
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

        if (offScreenImage == null)
        {
            offScreenImage = createImage(d.width*5, d.height*5);
            offGraphics    = offScreenImage.getGraphics();
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
