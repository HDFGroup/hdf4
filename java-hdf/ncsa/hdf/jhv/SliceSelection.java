package ncsa.hdf.jhv;

import java.awt.*;
import java.awt.event.*;

import ncsa.hdf.awt.Slider;
import ncsa.hdf.awt.OutlinePanel;
import ncsa.hdf.awt.event.*;

// Select 3-D SDS slice value
public class SliceSelection
    extends Frame
    implements ActionListener, SliderListener, ItemListener
{
    HDFTreeNode node;

    java.awt.CheckboxGroup  sliceCkbGroup;
    java.awt.Checkbox zSliceCkb;
    java.awt.Checkbox ySliceCkb;
    java.awt.Checkbox xSliceCkb;
    java.awt.TextField sliceField;
    Slider selectedSliceSlider;
    java.awt.Label sliceValueLabel;

    SDSDimInfo sliceInfo;

    // which operation right now
    public     static final int ZAXIS   = 0;
    public     static final int YAXIS   = 1;
    public     static final int XAXIS   = 2;

    int activeSlice;

    // Inner class for Window closing
    class SliceSelectionClosedProcess extends WindowAdapter {
        public void windowClosing(WindowEvent evt) { dispose(); }
    }

    public SliceSelection()
    {
        addNotify();
        addWindowListener(new SliceSelectionClosedProcess() );
        setTitle("SDS Slice Selection");

        // place slice checkbox
        Panel lPanel = new Panel();
        lPanel.setLayout(new GridLayout(3, 1));
        sliceCkbGroup = new java.awt.CheckboxGroup();
        xSliceCkb = new java.awt.Checkbox("X Slice", sliceCkbGroup, false);
        ySliceCkb = new java.awt.Checkbox("Y Slice", sliceCkbGroup, false);
        zSliceCkb = new java.awt.Checkbox("Z Slice", sliceCkbGroup, true);
        lPanel.add(xSliceCkb);
        lPanel.add(ySliceCkb);
        lPanel.add(zSliceCkb);

        // place selection field and slider
        Panel rPanel = new Panel();
        rPanel.setLayout(new GridLayout(3, 1));
        sliceValueLabel = new java.awt.Label("Slice Selection (000 - 000)",Label.LEFT);
        sliceField = new java.awt.TextField("1");
        selectedSliceSlider = new Slider();
        selectedSliceSlider.setDispMode(Slider.SLIDERVALUE);
        selectedSliceSlider.setSliderRange(0d,(double)28);
        rPanel.add(sliceValueLabel);
        rPanel.add(sliceField);
        rPanel.add(selectedSliceSlider);

        // place all components in the frame
        OutlinePanel panel = new OutlinePanel();
        panel.setLayout(new GridBagLayout());
        panel.setConstraints(lPanel, 0, 0, 2, 2
            ,GridBagConstraints.NONE, GridBagConstraints.WEST, 0.0, 0.0);
        panel.setConstraints(rPanel, 3, 0, 1, 2
            ,GridBagConstraints.HORIZONTAL, GridBagConstraints.WEST, 1.0, 0.0);
        add(panel);

        // add actions
        xSliceCkb.addItemListener(this);
        ySliceCkb.addItemListener(this);
        zSliceCkb.addItemListener(this);
        sliceField.addActionListener(this);
        selectedSliceSlider.addSliderListener(this);

        pack();
        setResizable(false);
    }

    public SliceSelection(String title) {
        this();
        setTitle(title);
    }

    public SliceSelection(SDSDimInfo slice) {
        this();
        this.sliceInfo = slice;
        int max = sliceInfo.getDim(getSelectedSlice())-1;
        sliceValueLabel.setText("Slice Selection (0 ~"+max+")");
    }

    public SliceSelection(HDFTreeNode node, SDSDimInfo slice) {
        this();
        this.sliceInfo = slice;
        this.node = node;
    
        // update z-axis range
        int aSlice = slice.getActiveSlice();
        selectedSliceSlider.setSliderRange(0,(double)slice.getDim(aSlice)-1);
        int max = sliceInfo.getDim(getSelectedSlice())-1;
        sliceValueLabel.setText("Slice Selection (0 - "+max+")");
    }

    public synchronized void setVisible() {
        setLocation(50, 50);
        super.setVisible(true);
    }

    static public void main(String args[]) {
        (new SliceSelection()).setVisible();
    }

    /**
     * SliderListener stuff
     */
    public void sliderStateChanged(SliderEvent evt)
    {
        Object target = evt.getSource();
        if (target instanceof Slider)
            handleSliderEvent((Slider)target);
    }

    /**
     * java.awt.event.ActionListener stuff
     */
    public void actionPerformed(ActionEvent evt)
    {
        Object target = evt.getSource();
        String arg = evt.getActionCommand();

        if(target instanceof Button) {
            Button button = (Button)target;
            handleButtonEvent(button, arg);
        }

        if(target instanceof TextField) {
            TextField textField = (TextField)target;
            handleTextFieldEvent(textField, arg);
        }
    }

    /**
     * java.awt.event.ItemListener stuff
     */
    public void itemStateChanged(ItemEvent e)
    {
        Object target =  e.getSource();
        if (target instanceof Checkbox)
            handleCheckboxEvent((Checkbox)target);
    }

    // update image by slice number
    public void handleTextFieldEvent(TextField target, String arg)
    {
        if(target instanceof TextField)
        {
            TextField textFld = (TextField)target;
            String sliceStr = (String)textFld.getText();
            int selection = Integer.parseInt(sliceStr);
            int max = sliceInfo.getDim(getSelectedSlice())-1;
            if (selection < 0 )
            {
                selection = 0;
                sliceField.setText(String.valueOf(0));
            }
            else if (selection > max )
            {
                selection = max;
                sliceField.setText(String.valueOf(max));
            }
            selectedSliceSlider.setSliderValue((double)selection);

            // update slice info
            sliceInfo.setSelectedValue(getSelectedSlice(), selection);

            // update image
            node.getHdfSDSimage(getSliceInfo());
        }
    }

    public void handleButtonEvent(Button target, String arg)
    {
        if ("Show Image".equals(arg))
            node.getHdfSDSimage(getSliceInfo());
    }

    public void handleCheckboxEvent(Checkbox target)
    {
        int aSlice = getSelectedSlice();

        // update active slice
        for (int kk=0; kk<sliceInfo.getRank(); kk++)
        {
            if (kk==aSlice) sliceInfo.setState(kk,true);
            else sliceInfo.setState(kk,false);
        }

        // update slice range
        int max = sliceInfo.getDim(aSlice)-1;
        selectedSliceSlider.setSliderRange(0,(double)max);
        sliceValueLabel.setText("Slice Selection (0 ~"+max+")");

        // modify the selected value
        int selection = sliceInfo.getSelectedValue(aSlice);
        // txtfield
        sliceField.setText(Integer.toString(selection));
        // slider
        selectedSliceSlider.setSliderValue((double)selection);

        // update image since slice info. has changed
        node.getHdfSDSimage(getSliceInfo());
    }

    public void handleSliderEvent(Slider target)
    {
        // update slice selected value
        if (selectedSliceSlider.sliderChanged())
        {
           int selection = (int)Math.rint((double)selectedSliceSlider.getSliderValue());
           // System.out.println("Slice: " + getSelectedSlice() + ", " + selection);
           sliceField.setText(Integer.toString(selection));

           // update slice info
           sliceInfo.setSelectedValue(getSelectedSlice(), selection);

           // update image since slider has been changed
           node.getHdfSDSimage(getSliceInfo());
        }
    }

    // get selected slice
    int getSelectedSlice()
    {
        int retVal = -1;

        // gets the current choice
        boolean state = xSliceCkb.getState();
        if (state) retVal = XAXIS;

        state = ySliceCkb.getState();
        if (state) retVal = YAXIS;

        state = zSliceCkb.getState();
        if (state) retVal = ZAXIS;

        return retVal;
    }

    public SDSDimInfo getSliceInfo()
    {
        return this.sliceInfo;
    }

}
