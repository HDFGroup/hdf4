
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
    This simple class just draws an outline around itself. 
    
    Author:        Thomas L. Redman
    Date:        12/11/96
    Changes:
*/

package ncsa.hdf.awt;

import java.awt.*;

public class OutlinePanel extends EasyConstrainsPanel
{
    String      fLabel;
    static int  INSETS = 5;
    Insets      fInsets = null;
    Font        fTitleFont = null;
    
    public OutlinePanel ()
    {
        fLabel = new String ("");
        this.CreateTitleFont ();
    }
    public OutlinePanel (String label)
    {
        super ();
        fLabel = label;
        this.CreateTitleFont ();
    }
    
    
    /**
        This method will create the font we will use for drawing the groups
        title.
    */
    private void CreateTitleFont ()
    {
        if (getFont () != null)
            fTitleFont = new Font (getFont ().getName(), Font.ITALIC, getFont ().getSize());
    }
    
    /**
        Change the insets to those passed in
        @param newInsets the new insets.
    */
    public void setInsets (Insets newInsets)
    {
        fInsets = newInsets;
    }
    
    /**
        Get the border around this guy if there is one
    */
    public Insets getInsets()
    {
        if (fTitleFont == null)
            this.CreateTitleFont ();
            
        FontMetrics fm;
        if (fTitleFont != null)
            fm = getFontMetrics (fTitleFont);
        else
            fm = null;
        
        if (fInsets == null)
            if (fm == null)
                fInsets = new Insets (18, INSETS, INSETS, INSETS);
            else
                fInsets = new Insets (fm.getHeight() + 4, INSETS, INSETS, INSETS);
        return fInsets;
    }
    
    /**
        Paint a 3d border around the panel.
        @param g the graphics port.
    */
    public void paint (Graphics g)
    {
        if (fTitleFont == null)
            this.CreateTitleFont ();
            
        FontMetrics fm = getFontMetrics (fTitleFont);
        Color originalColor = this.getBackground ();
        Color lighterColor = originalColor.brighter();
        Color darkerColor = originalColor.darker();
        
        Insets border = new Insets (1, 1, getSize().height - 2, getSize().width - 2);
        g.setColor (darkerColor);
        // Draw the shadow edge
        g.drawLine (border.left, border.top, border.right, border.top);
        g.drawLine (border.right-1, border.top+2, border.right-1, border.bottom-1);
        g.drawLine (border.right-1, border.bottom-1, border.left-2, border.bottom-1);
        g.drawLine (border.left, border.bottom, border.left, border.top);
        
        g.setColor (lighterColor);
        g.drawLine (border.left+1, border.top+1, border.right-1, border.top+1);
        g.drawLine (border.right, border.top+1, border.right, border.bottom);
        g.drawLine (border.right, border.bottom, border.left-1, border.bottom);
        g.drawLine (border.left+1, border.bottom-2, border.left+1, border.top+1);
                
        g.setColor (darkerColor.darker().darker());
        g.drawString (fLabel, Math.max(5, (this.getSize().width - fm.stringWidth (fLabel))/2),
                fm.getAscent()+2);
        super.paint (g);
    }
    
}

