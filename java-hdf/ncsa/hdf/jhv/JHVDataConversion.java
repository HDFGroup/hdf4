
//Title:        Java HDF Viewer
//Version:
//Copyright:    Copyright (c) 1997
//Author:       HDF Java Team
//Company:      NCSA
//Description:  The project is set for the summer98 release.


package ncsa.hdf.jhv;

import java.awt.*;
import java.awt.event.*;

import ncsa.hdf.awt.OutlinePanel;
import ncsa.hdf.hdflib.HDFConstants;

public class JHVDataConversion extends Dialog
    implements ActionListener
{
    private OutlinePanel panel1 = new OutlinePanel();
    private Label a = new Label("Constant");
    private TextField A = new TextField("0.0");
    private Label b = new Label("Slope");
    private TextField B = new TextField("1.0");
    private Label p = new Label("Power");
    private TextField P = new TextField("1.0");
    private TextField function = new TextField("f(x) = Constant + Slope * x^Power");
    private Button ok = new Button("  Ok  ");
    private Button cancel = new Button("Cancel");

    /** array holds the function parameters
      * f[0] = constant
      * f[1] = slope
      * f[2] = power
      */
    private double[] f = {0.0, 1.0, 1.0};
    private boolean needDataConversion = false;

    public JHVDataConversion(Frame frame, String title, boolean modal)
    {
        super(frame, title, modal);

        WindowListener adapter = new WindowAdapter()
        {
             public void windowClosing(WindowEvent e) { e.getWindow().dispose(); }
        };
        this.addWindowListener(adapter);
        A.addActionListener(this);
        B.addActionListener(this);
        P.addActionListener(this);
        ok.addActionListener(this);
        cancel.addActionListener(this);

        try {
            init();
            add(panel1);
            pack();
            show();
        }
        catch (Exception e) { e.printStackTrace(); }
    }

    public void init() throws Exception
    {
        function.setEditable(false);
        panel1.setLayout(new GridBagLayout());
        panel1.setConstraints(a, 0, 0, 2, 1
            ,GridBagConstraints.NONE, GridBagConstraints.WEST, 0.0, 0.0);
        panel1.setConstraints(A, 3, 0, 1, 1
            ,GridBagConstraints.HORIZONTAL, GridBagConstraints.WEST, 1.0, 0.0);
        panel1.setConstraints(b, 0, 1, 2, 1
            ,GridBagConstraints.NONE, GridBagConstraints.WEST, 0.0, 0.0);
        panel1.setConstraints(B, 3, 1, 1, 1
            ,GridBagConstraints.HORIZONTAL, GridBagConstraints.WEST, 1.0, 0.0);
        panel1.setConstraints(p, 0, 2, 1, 1
            ,GridBagConstraints.NONE, GridBagConstraints.WEST, 0.0, 0.0);
        panel1.setConstraints(P, 3, 2, 1, 1
            ,GridBagConstraints.HORIZONTAL, GridBagConstraints.WEST, 1.0, 0.0);
        panel1.setConstraints(function, 0, 4, 4, 1
            ,GridBagConstraints.HORIZONTAL, GridBagConstraints.WEST, 1.0, 0.0);
        panel1.setConstraints(    ok, 0, 5, 2, 1
            ,GridBagConstraints.NONE, GridBagConstraints.EAST, 0.0, 0.0);
        panel1.setConstraints(cancel, 3, 5, 1, 1
            ,GridBagConstraints.NONE, GridBagConstraints.CENTER, 1.0, 0.0);
    }

    public void actionPerformed(ActionEvent e)
    {
        Object target = e.getSource();

        if (target.equals(ok))
        {
            updateFunction();
            dispose();
        }
        else if (target.equals(cancel))
        {
            needDataConversion = false;
            dispose();
        }
        else if (target.equals(A) || target.equals(B) || target.equals(P))
        {
            f[0] = (Double.valueOf(A.getText())).doubleValue();
            f[1] = (Double.valueOf(B.getText())).doubleValue();
            f[2] = (Double.valueOf(P.getText())).doubleValue();
            function.setText("f(x) = "+A.getText()+" + "+B.getText()+" * x^"+P.getText());
        }
    }

    private void updateFunction()
    {
        f[0] = (Double.valueOf(A.getText())).doubleValue();
        f[1] = (Double.valueOf(B.getText())).doubleValue();
        f[2] = (Double.valueOf(P.getText())).doubleValue();
        needDataConversion = !(f[0]==0 && f[1]==1 && f[2]==1);
    }

    public double[] getFunction() { return f; }
    public boolean needDataConversion() { return needDataConversion; }

}