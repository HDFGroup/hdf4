
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


/**
	This is a panel, but it includes methods that make use of grid bad layouts easier.
	
	Author:		Thomas L. Redman
	Date:		12/13/96
	Changes:
*/

package ncsa.hdf.awt;

import java.awt.*;
import java.awt.event.*;

public class EasyConstrainsPanel extends Panel implements KeyListener, MouseListener  
{
    // This is the background to be used by all joule applications and applets.
   
    /**
        Create and set up a grid bag constraint with the values given.
    */
    GridBagConstraints getConstraints (int x, int y, int width, int height, int fill,
         int anchor, double weightX, double weightY)
    {
        GridBagConstraints c = new GridBagConstraints ();
        c.gridx     = x;
        c.gridy     = y;
        c.gridwidth = width;
        c.gridheight = height;
        c.fill         = fill;
        c.anchor     = anchor;
        c.weightx    = weightX;
        c.weighty     = weightY;
        return c;
    }
    
    /**
        Set up the grid bag constraints fo the object
    */
    public void setConstraints (Component it, int x, int y, int width, int height, int fill,
         int anchor, double weightX, double weightY)
    {
        GridBagConstraints c = this.getConstraints (x, y, width, height, fill, anchor, weightX, weightY);
        c.insets = new Insets (2, 2, 2, 2);
        ((GridBagLayout) this.getLayout()).setConstraints (it, c);
        this.add (it);
    }
    
    /**
        Set up the grid bag constraints fo the object
    */
    public void setConstraints (Component it, int x, int y, int width, int height, int fill,
         int anchor, double weightX, double weightY, Insets insets)
    {
        GridBagConstraints c = this.getConstraints (x, y, width, height, fill, anchor, weightX, weightY);
        c.insets     = insets;
        ((GridBagLayout) this.getLayout()).setConstraints (it, c);
        this.add (it);
    }

    public void keyPressed(KeyEvent e) {}
    public void keyReleased(KeyEvent e) {}
    public void keyTyped(KeyEvent e) {}
    public void mouseClicked(MouseEvent e) {} 
    public void mouseEntered(MouseEvent e) {}
    public void mouseExited(MouseEvent e)  {}
    public void mousePressed(MouseEvent e) {}
    public void mouseReleased(MouseEvent e){} 

}
