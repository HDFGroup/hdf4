
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
 * InfoDialog(Frame parent, String title, String message)
 *
 * This class produces a dialog box
 * and displays the message in the box.
 *
 * @version 1.0, 01/13/98
 * @author Peter Cao (xcao@ncsa.uiuc.edu)
 */
public class InfoDialog
    extends Dialog
    implements ActionListener
{

    /** if the clicked button is the okButton */
    private boolean isOkAction = true;

    /** if it has a cancel button */
    private boolean hasCancelButton = false;

    /** if it has a textField */
    private boolean hasTextField = false;

    /** the TextField to hold user's input */
    private TextField textField;

    /** the text message to be displayed */
    private String message;

    /** the image to be displayed */
    private Image image;

    protected Button okButton, cancelButton;

    /**
     *  Create a noew dialog box which dsiplyas text information and image.
     *  This dialog has two display options: with/without the cancel button.
     *
     *  @param  parent          the parent frame
     *  @param  title           the tile of the dialog box
     *  @param  message         the text message to be displayed in the dialog box
     *  @param  iamge           the iamge to be displayed in the dialog box
     *  @param  hasCancelButton the indicator if it has a cancel button
     */
    public InfoDialog(Frame parent, String title,
        String message, Image image, boolean hasCancelButton)
    {
        this(parent, title, message,image, hasCancelButton, false);
    }

    /**
     *  Create a noew dialog box which dsiplyas text information and image.
     *  This dialog has two display options: with/without the cancel button.
     *
     *  @param  parent          the parent frame
     *  @param  title           the tile of the dialog box
     *  @param  message         the text message to be displayed in the dialog box
     *  @param  iamge           the iamge to be displayed in the dialog box
     *  @param  hasCancelButton the indicator if it has a cancel button
     *  @param  hasTextField    the indicator if it has a TextFiled
     */
    public InfoDialog(Frame parent, String title, String message,
        Image image, boolean hasCancelButton, boolean hasTextField)
    {
        super(parent, title, true);

        this.message = message;
        this.image = image;
        this.hasCancelButton = hasCancelButton;
        this.hasTextField = hasTextField;

        WindowListener adapter = new WindowAdapter()
        {
            public void windowClosing(WindowEvent e)
            {
                e.getWindow().dispose();
                ((Frame)getParent()).toFront();
            }
        };

        this.addWindowListener(adapter);

        okButton     = new Button("  Ok  ");
        cancelButton = new Button("Cancel");
        textField = new TextField();
        okButton.addActionListener(this);
        cancelButton.addActionListener(this);
        textField.addActionListener(this);

        this.createDisplay();
    }

    public void createDisplay()
    {
        this.setLayout(new BorderLayout(2, 2));
        MultiLineLabel ml = new MultiLineLabel(message, 20, 20);
        ml.setLogo(image);
        this.add("Center", ml);

        Panel p = new Panel();
        p.setLayout(new FlowLayout(FlowLayout.CENTER, 10, 10));
        p.add(okButton);
        if (hasCancelButton) p.add(cancelButton);

        Panel bottomPanel = new Panel();
        bottomPanel.setLayout(new GridLayout(0, 1));
        if (hasTextField)
        {
            Panel txtPanel = new Panel();
            txtPanel.setLayout(new GridLayout(0, 1));
            txtPanel.add(new Label("Web Browser: "));
            txtPanel.add(textField);
            bottomPanel.add(txtPanel);
        }
        bottomPanel.add(p);

        this.add("South", bottomPanel);

        // Resize the window to the preferred size of its components
        this.pack();
        setResizable(false);
    }

    /**
     *   handle editor events
     */
    public void actionPerformed(ActionEvent e)
    {
        Object source = e.getSource();

        if (source.equals(okButton) || source.equals(textField))
            isOkAction = true;
        else
            isOkAction = false;

        this.dispose();
        ((Frame)getParent()).toFront();
    }

    /** check if the OkButton is clicked */
    public boolean clickedOkButton()
    {
        return isOkAction;
    }

    /**
     *  set okButton and cancelButton labels
     *
     *  @param okLabel     the label of the okButton
     *  @param cancelLabel the label of the cancelButton
      */
    public void setlabels(String okLabel, String cancelLabel)
    {
        if (okLabel!=null && okLabel.length() != 0)
            okButton.setLabel(okLabel);
        if (cancelLabel!=null && cancelLabel.length() != 0)
            cancelButton.setLabel(cancelLabel);
    }

    /** return the TextField which holds user's input message */
    public TextField getTextField() {return textField; }

}

