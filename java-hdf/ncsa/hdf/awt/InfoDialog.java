
package ncsa.hdf.awt;

import java.awt.*;
import java.awt.event.*;

/**
 * InfoDialog(Frame parent, String title, String message)
 *
 * This class produces a dialog box
 * and displays the message in the box.
 *
 * @version 1.0, 01/13/98
 * @author Peter Cao (xcao@ncsa.uiuc.edu)
 */
public class InfoDialog extends Dialog implements ActionListener
{
    protected Button button = new Button("    OK    ");

    public InfoDialog(Frame parent, String title, String message, Image image)
    {
        super(parent, title, false);
        WindowListener adapter = new WindowAdapter()
        {
            public void windowClosing(WindowEvent e)
            {
                e.getWindow().dispose();
                ((Frame)getParent()).toFront();
            }
        };
        this.addWindowListener(adapter);

        button.addActionListener(this);
        this.setLayout(new BorderLayout(2, 2));
        MultiLineLabel ml = new MultiLineLabel(message, 20, 20);
        ml.setLogo(image);
        this.add("Center", ml);
        Panel p = new Panel();
        p.setLayout(new FlowLayout(FlowLayout.CENTER, 10, 10));
        p.add(button);
        this.add("South", p);

        // Resize the window to the preferred size of its components
        this.pack();
        setResizable(false);
    }
    
    /**
     *   handle editor events
     */
    public void actionPerformed(ActionEvent e)
    {
        if (e.getSource() instanceof Button)
        {
            this.dispose();
            ((Frame)getParent()).toFront();
            return;
        }
    }

}

