
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
 *  The Editor class displays a dialog window
 *  from which the user can edit a list (delete, replace, or add). 
 *
 *  @version   1.1.3 October 17 1997
 *  @author    Peter X. Cao
 */
 
package ncsa.hdf.awt;
 
import java.awt.*;
import java.awt.event.*;
  
/**
 *  this class gives a layout for text editor
 *
 * @version 1.0, 10/15/96
 * @author Xiangchi Cao
 */
public class Editor extends Dialog implements ActionListener
{
    // define GridBagConstraint constants
    protected final static int dC    = GridBagConstraints.CENTER;
    protected final static int dE    = GridBagConstraints.EAST;
    protected final static int dN    = GridBagConstraints.NORTH;
    protected final static int dNE   = GridBagConstraints.NORTHEAST;
    protected final static int dNW   = GridBagConstraints.NORTHWEST;
    protected final static int dS    = GridBagConstraints.SOUTH;
    protected final static int dSE   = GridBagConstraints.SOUTHEAST;
    protected final static int dSW   = GridBagConstraints.SOUTHWEST;
    protected final static int dW    = GridBagConstraints.WEST;     
    protected final static int fB    = GridBagConstraints.BOTH;      
    protected final static int fH    = GridBagConstraints.HORIZONTAL;
    protected final static int fN    = GridBagConstraints.NONE;      
    protected final static int fV    = GridBagConstraints.VERTICAL;  
    protected final static int gR    = GridBagConstraints.REMAINDER;
  
    protected Button    okButton, cancelButton;
    protected Panel     buttonPanel;
  
    public Editor(Frame parent, String title)
    {
        super(parent, title, true);
        this.makeView ();
    }

    /**
     *  make display window for the editor
     */
    public void makeView()
    {
        // set up button panel
        okButton     = new Button("  Ok  ");
        cancelButton = new Button("Cancel");
        okButton.addActionListener(this);
        cancelButton.addActionListener(this);
        buttonPanel = new Panel();
        buttonPanel.setLayout(new GridBagLayout());
        this.constrain(okButton,buttonPanel,0,0,1,1,fN,dC,1,0,5,5,0,0);
        this.constrain(cancelButton,buttonPanel,1,0,1,1,fN,dC,0,0,0,5,0,120);
    }
  
   
    /**
     *  setup gridbaglayout constrains
     */
    public void constrain(Component co,Container ct,int x,int y,int w,
        int h,int f,int a,double wx, double wy, int t,int l,int b,int r) 
    {
  
        GridBagConstraints gc = new GridBagConstraints();
        gc.gridx      = x;    gc.gridy      = y;
        gc.gridwidth  = w;    gc.gridheight = h;
        gc.fill       = f;    gc.anchor     = a;
        gc.weightx    = wx;   gc.weighty    = wy;
        if (t+b+l+r > 0)
        {
            gc.insets = new Insets(t,l,b,r);
        }
        ((GridBagLayout)ct.getLayout()).setConstraints(co,gc);
        ct.add(co);
    }
  
    /**
     *  return a copy of a source list
     *  
     *  @param source the source list
     *  @return       the copy the the source list
     */
    public List listCopy(List source)
    {
        if (source == null) 
            return null;

        int length = source.getItemCount();
        List acopy = new List(length);
        for (int i=0; i<length; i++)
            acopy.addItem(source.getItem(i));

        return acopy;
    }

    /** derived class should overwrite it */
    public void actionPerformed(ActionEvent e){}

  }
  
