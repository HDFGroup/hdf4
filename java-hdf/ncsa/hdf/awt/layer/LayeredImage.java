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


package ncsa.hdf.awt.layer;

import java.awt.*;
import java.awt.event.*;
import java.awt.image.*;
import java.lang.String;
import java.lang.Thread;
import java.util.*;
import ncsa.hdf.awt.*;
import ncsa.hdf.awt.event.*;
import ncsa.hdf.awt.image.*;

/**
 * Handle overlay images display
 */
public class LayeredImage extends Frame
    implements ActionListener, ItemListener
{
    List imageList;
    ScrollPane scrollPanel;
    Label listLabel, selectedImageLabel;
    Button dismissButton, applyButton, tansparentButton;
    TextArea imageLayersInfo;

    // a slider to handle a factor of image's alpha-filter
    Slider filterSlider;
    // a panel to handle multiple-layer image display
    LayeredImagePane layerPane;

    // layered image to be displayed
    LayeredImageInfo imagesInfo[];

    int selectImage = 0;    // current select index
    int selected[] = new int[0];

    public TextArea bgArea;

    // Default constructor
    public LayeredImage(String title)
    {
        super(title);
        WindowListener adapter = new WindowAdapter()
        {
            public void windowClosing(WindowEvent e)
            {
                unselectAll();
                e.getWindow().dispose();
            }
        };
        this.addWindowListener(adapter);

        setLayout(null);
        addNotify();
        setSize(getInsets().left + getInsets().right + 577,
                getInsets().top + getInsets().bottom + 398);

        scrollPanel = new java.awt.ScrollPane();
        scrollPanel.setBounds(getInsets().left + 12,
                            getInsets().top  + 12,350,376);
        add(scrollPanel);

        // image list label
        listLabel = new java.awt.Label("Raster Images");
        listLabel.setBounds(getInsets().left + 384,
                          getInsets().top  + 12,168,36);
        add(listLabel);
    
        // multiple selections  is applied
        imageList = new java.awt.List(0,true);
        add(imageList);
        imageList.setBounds(getInsets().left + 384,
                          getInsets().top  + 48,166,110);

        // image layers info label
        Label        imageLayersInfoLabel = new Label("Selected Images");
        add(imageLayersInfoLabel);
        imageLayersInfoLabel.setBounds( getInsets().left + 384,
                                          getInsets().top  + 160,166,20);
    
        // image layers info
        imageLayersInfo = new TextArea("",5,0,TextArea.SCROLLBARS_VERTICAL_ONLY);
        imageLayersInfo.setEditable(false);
        add(imageLayersInfo);
        imageLayersInfo.setBounds(getInsets().left + 384,
                                    getInsets().top  + 180,166,86);


        // selected image  label
        selectedImageLabel = new java.awt.Label("Layer: 0(image)");
        selectedImageLabel.setFont( new Font( "TimesRoman", Font.PLAIN, 12 ));
        selectedImageLabel.setBounds(getInsets().left + 384,
                                       getInsets().top  + 270,170,26);
        add(selectedImageLabel);

        Label label = new Label("Background");
        label.setBounds(getInsets().left+384,getInsets().top+300,70,25);
        add(label);

        bgArea = new TextArea("", 3, 2, TextArea.SCROLLBARS_NONE);
        bgArea.setEnabled(false);
        bgArea.setBounds(getInsets().left+455, getInsets().top+300,95,25);
        add(bgArea);

        Label  opaqueLabel = new Label("Opaque ");
        opaqueLabel.setBounds(getInsets().left+384,getInsets().top+330,70,25);
        add(opaqueLabel);

        filterSlider = new Slider();
        filterSlider.setSliderRange(0,255);
        filterSlider.setSliderValue(255);
        filterSlider.setBounds(getInsets().left+455,getInsets().top+330,95,25);
        add(filterSlider);

        applyButton = new java.awt.Button("Apply");
        applyButton.setBounds(getInsets().left+384,getInsets().top+362,70,25);
        add(applyButton);

        dismissButton = new java.awt.Button("Dismiss");
        dismissButton.setBounds(getInsets().left+480,getInsets().top+362,70,25);
        add(dismissButton);

        // listener
        dismissButton.addActionListener(this);
        applyButton.addActionListener(this);
        imageList.addItemListener(this);

        // scrollPane
        layerPane = new LayeredImagePane(this);
        scrollPanel.add(layerPane, this, 0);

        setResizable(false);
        pack();
        show();
    }

    public LayeredImage() {
        this("Image Overlay");
    }

    public void setLayeredImage(LayeredImageInfo imagesInfo[])
    {
        // image info
        this.imagesInfo = imagesInfo;
        imageList.removeAll();
        // add to list
        for (int kk=0; kk<imagesInfo.length; kk++) 
            imageList.add(imagesInfo[kk].getName());
    }

    public int getImageIndex() {
       return (int)selectImage;
    }

    public String getImagesLayerInfo()
    {

        StringBuffer info = new StringBuffer();

        // current selected items
        int[] selects = imageList.getSelectedIndexes();
    
        for (int i=0; i<selects.length; i++) {
            int index = selects[i];
            String imageInfo = imagesInfo[index].getLayerInfo();
            info.append(imageInfo+"\n");
        }
        return (info.toString());
    }

    /**
     * Set the cursor image to a predefined cursor.
     * @param cursorType one of the cursor constants defined above.
     */
   public void setCursor(Cursor cursor)
   {
        // change corsor type
        super.setCursor(cursor);

        // default toolkit
        Toolkit.getDefaultToolkit().sync();
    }

    /**
     * java.awt.event.ItemListener stuff
     */
    public void itemStateChanged(ItemEvent e)
    {
        if (e.getSource() instanceof List)
            handleListEvent(e);
    }

    /** button event handling 
     * @param target an  instance of a button
     * @param arg title of a button
     */
    public void handleButtonEvent(Button target)
    {
        if (target.equals(dismissButton))
            dispose();
        else if (target.equals(applyButton))
        {
            ImageFilter filter = null;
            double f = 0;

            // check to see if slider changed
            if (filterSlider.sliderChanged())
                f = 100-filterSlider.getValue();

            Color transColor = layerPane.getImageBackground();
            filter = (ImageFilter)new AlphaFilter(transColor,(float)f);
            layerPane.updateLayerFilter(getImageIndex(), filter);
        }
    }

    public void handleListEvent(ItemEvent e)
    {
        List target = (List) e.getSource();

        // only deal with the imageList so far
        if (target != imageList) return;

        selected = imageList.getSelectedIndexes();

        int changedItemIndex = ((Integer)e.getItem()).intValue();

        if (e.getStateChange() == ItemEvent.DESELECTED)
        {
            layerPane.removeImage(changedItemIndex);
            if (selected.length > 0)
                selectImage = selected[0];
            else
                selectImage = -1;
        }
        else
        {
            layerPane.addImage(imagesInfo[changedItemIndex], changedItemIndex);
            selectImage = changedItemIndex;
        }

        scrollPanel.doLayout();

        // modify the selectedimageLabel
        String lab = "Top Layer";
        if (selectImage >= 0)
        {
            lab = "Top " + imagesInfo[selectImage].getLayerInfo();
            layerPane.setTopImageIndex(selectImage);
        }

        imageLayersInfo.setText(getImagesLayerInfo());

        // reset the label message
        selectedImageLabel.setText(lab);

        layerPane.repaint();

    }

    public void unselectAll ()
    {
        int imgNumber = imageList.getItemCount();
        for (int kk=0; kk<imgNumber; kk++)
        {
            if (imageList.isIndexSelected(kk))
            {
                imageList.deselect(kk);
                layerPane.removeImage(kk); 
            }  
         }  
        selected = new int[0];         
    
    }
    
    /**
     * java.awt.event.ActionListener stuff
     */
    public void actionPerformed(ActionEvent evt)
    {
        Object target = evt.getSource();
        String arg = evt.getActionCommand();
        
        if(target instanceof Button)
        {
            Button button = (Button)target;
            handleButtonEvent(button);
        }  
    }
    
    public void processEvent(AWTEvent event)
    {
        if (event.getID() == Event.WINDOW_DESTROY)
        {
            setVisible(false);
            dispose();
        }
    }

    public int[] getImagePixelValue(Image img, int x, int y, int w, int h)
    {
        if (((w*h)<=0) || ((x*y) <0)) return null;
    
        // return value
        int[] pixels = new int[w * h];
    
        PixelGrabber pg = new PixelGrabber(img, x, y, w, h, pixels, 0, w);
        try { pg.grabPixels(); } 
        catch (InterruptedException e) { return null; }
        if ((pg.status() & ImageObserver.ABORT) != 0) {
            System.err.println("image fetch aborted or errored");
            return null;
        }
     
        return pixels;
    }

}

