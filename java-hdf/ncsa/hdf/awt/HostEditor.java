
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

package ncsa.hdf.awt;

import java.awt.*;
import java.awt.event.*;

/**
 *  host list editor
 *  can add and delete hosts from the original host list
 *
 * @version 1.0, 10/15/96
 * @author Xiangchi Cao
 */
public class HostEditor extends Editor
{
    Frame parentFrame;
    List hostList;
    HostList  fHostList;
    boolean changed;
    
    public HostEditor(Frame parent, List list)
    {
        super(new Frame(""), "");
        setResizable(false);

        parentFrame = parent;
        WindowListener adapter = new WindowAdapter()
        {
            public void windowClosing(WindowEvent e)
            {
                e.getWindow().dispose();
                parentFrame.toFront();
            }
        };
        this.addWindowListener(adapter);

        hostList = listCopy(list);
        changed = false;
        this.setLayout (new GridBagLayout ());
        fHostList = new HostList ("Remote Host Editor");
        fHostList.listCopy(hostList);
        this.constrain (fHostList, this, 0, 0, 1, 1, GridBagConstraints.BOTH, GridBagConstraints.WEST, 1.0, 1.0, 2, 2, 2, 2);
        this.constrain (buttonPanel, this, 0, 1, 1, 1, GridBagConstraints.HORIZONTAL,  GridBagConstraints.WEST, 1.0, 0.0, 2, 2, 2, 2);
        setSize(450, 300);
        show();
    }

    /**
     *   handle editor events
     */
    public void actionPerformed(ActionEvent e)
    {
        if (e.getSource() == okButton)
        {
            hostList = fHostList.getList();
            changed = true;
            this.dispose();
            parentFrame.toFront();
            return;
        }
        else if (e.getSource() == cancelButton)
        {
            this.dispose();
            parentFrame.toFront();
            return;
        }
    }

    public boolean isChanged () { return changed; }
    public List getHostList() { return hostList; }

}
