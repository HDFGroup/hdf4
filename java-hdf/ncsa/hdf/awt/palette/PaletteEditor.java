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

//     Palette editor
package ncsa.hdf.awt.palette;

import java.awt.image.ColorModel;
import java.awt.image.*;
import java.awt.*;
import java.util.*;
import java.io.*;
import java.net.*;
import java.awt.event.*;

// class to privide the palette editor
public class PaletteEditor extends Frame  implements  ActionListener
{
	// Specify the menu for this interface(need?)
	Menu fileMenu;
	Menu paletteMenu;
	
	// object to be communicated 
	PaletteEditorAdapter	object;
	
	// Panel to be the color editor
	public PalettePanel palettePanel;

	// Inner class for window closing
	class  PaletteEditorWindowClosedProcess extends WindowAdapter {
		public void windowClosing(WindowEvent e) {
			selectedExit();
		}
	}

	// Default constructor
	public PaletteEditor()
	{
		super( "Palette Editor" );

		// new menu bar
		MenuBar menuBar = new MenuBar();

		// Menu items (file)
		fileMenu = new Menu( "File" );
		fileMenu.add( new MenuItem( "Exit" ));
		menuBar.add( fileMenu );

		// Menu items (palette)
		paletteMenu = new Menu( "Palette" );
		paletteMenu.add( new MenuItem( "GrayScale" ));
		paletteMenu.add( new MenuItem( "GrayScale-Banded" ));
		paletteMenu.add( new MenuItem( "GrayScale-Inverted" ));
		paletteMenu.add( new MenuItem( "Rainbow" ));
		paletteMenu.add( new MenuItem( "Rainbow-Banded" ));
		paletteMenu.add( new MenuItem( "Rainbow-Inverted" ));
		paletteMenu.add( new MenuItem( "Rainbow-Striped" ));
		menuBar.add( paletteMenu );

		// set menu bar
		setMenuBar( menuBar );

		// create  user interface
		PanelConstructor();

		// exit
		for (int i=0; i<fileMenu.getItemCount(); i++) {
		     ((MenuItem)fileMenu.getItem(i)).addActionListener(this);
		}

		// palette
		for (int i=0; i<paletteMenu.getItemCount(); i++) {
		     ((MenuItem)paletteMenu.getItem(i)).addActionListener(this);
		}
		
		// window listener
		addWindowListener(new PaletteEditorWindowClosedProcess());
	}

    public PaletteEditor(Object object) {

	this();
	this.object = (PaletteEditorAdapter)object;
    }

	// 	The editor will rewrite the contents of this section each time 
        //      the form is saved.
	private final void PanelConstructor()
	{
		setResizable( false );
		// setCursor( Frame.DEFAULT_CURSOR );
		setLayout( null );
		setForeground( new Color( 255,255,255 ));
		setBackground( Color.lightGray);
		addNotify();
		setSize( (getInsets().left + getInsets().right + 445), (getInsets().top + getInsets().bottom + 445) );

		palettePanel = new PalettePanel(this);
		palettePanel.setBackground( new Color( 192, 192, 192 ));
		add( palettePanel );
		palettePanel.setBounds( (getInsets().left + 5), (getInsets().top + 5), 435, 435 );
	}

    // return the current color model
    public ColorModel currentColorModel() {

	// return the current color model
	return(palettePanel.paletteCanvas.currentColorModel());
    }

    // return the current palette value(RGB,RGB,...)
    public byte[] currentPaletteValue() {

	// return the current color model
	return(palettePanel.paletteCanvas.currentPaletteValue());
    }

    // set new palette model 
    public void setColorModel(byte[] rgb) {
	if (rgb != null) {

	   // set rainbow palette
	   palettePanel.paletteCanvas.setPalette(rgb);
				
	   // color model changed
	   colorModelChanged();
	}
    }

//
// Not sure why this is necessary.  This causes
// problems if the colors are declared 'byte'
//  Omitted for now. 
//
    // set new palette model 
 //   public void setColorModel(char[] rgb) {
//
//	if (rgb != null) {
//	   int len = rgb.length;
//	   char[] crgb = new char[len];
//	   for (int i=0; i<len; i++)
//	 	crgb[i] = (char)rgb[i];
//
//	   // set rainbow palette
//	   palettePanel.paletteCanvas.setPalette(crgb);
//				
//	   // color model changed
//	   colorModelChanged();
//	}
 //   }

    // RGB value has been change , the orignal image  should be changed ?
    public void colorModelChanged() {

	// who is my parent? I have a new color model .
	// unselected
	palettePanel.fiddleCheckbox.setState(false);
	palettePanel.paletteCanvas.fiddleFlag = false;

	ColorModel cm = currentColorModel();

	// redraw new image
	if (object != null)
	   object.updateColorModel(cm);

    }

    // RGB value has been change , the orignal image  should be changed ?
    public void colorModelChanged(ColorModel cm) {

	// who is my parent? I have a new color model .
	
	// redraw new image
	object.updateColorModel(cm);

    }

	public void selectedExit()
	{
		// who is my parent? (need to modify)			
		object.setPaletteEditorOpen(false);
		dispose();
	}

	public void handleMenuEvent(ActionEvent  event)
	{
		if (event.getSource() instanceof MenuItem)
		{
			// file
			String label = (String) event.getActionCommand();

			if (label.equalsIgnoreCase( "Exit" ))
			{
				selectedExit();
			}

			if (label.equalsIgnoreCase( "Rainbow" ))
			{
				// set rainbow palette
				palettePanel.paletteCanvas.setRainbowPalette();
				
				// color model changed
				colorModelChanged();

			}

		
			if (label.equalsIgnoreCase( "Rainbow-Banded" ))
			{
				// set rainbow palette
				palettePanel.paletteCanvas.setBandedRainbowPalette();
				
				// color model changed
				colorModelChanged();

			}


			if (label.equalsIgnoreCase( "Rainbow-Inverted" ))
			{
				// set rainbow palette
				palettePanel.paletteCanvas.setInvertedRainbowPalette();

				// color model changed
				colorModelChanged();

			}


			if (label.equalsIgnoreCase( "Rainbow-Striped" ))
			{
				// set rainbow palette
				palettePanel.paletteCanvas.setStripedRainbowPalette();

				// color model changed
				colorModelChanged();

			}
	
			if (label.equalsIgnoreCase( "GrayScale" ))
			{
				// set rainbow palette
				palettePanel.paletteCanvas.setGrayScalePalette();

				// color model changed
				colorModelChanged();

			}


			if (label.equalsIgnoreCase( "GrayScale-Banded" ))
			{
				// set rainbow palette
				palettePanel.paletteCanvas.setBandedGrayScalePalette();
				
				// color model changed
				colorModelChanged();

			}


			if (label.equalsIgnoreCase( "GrayScale-Inverted" ))
			{
				// set rainbow palette
				palettePanel.paletteCanvas.setInvertedGrayScalePalette();
				// color model changed
				colorModelChanged();
			}

		   // set RGB panel
		   palettePanel.setRGBPanel();

		}
	}

	// test
	public static void main( String args[] )
	{
		// 	You can add code anywhere in this method.

		PaletteEditor app = new PaletteEditor();
		app.setVisible(true);
	}

    /**
     * java.awt.event.ActionListener stuff
     */
    public void actionPerformed(ActionEvent evt)  {

	if (evt.getSource() instanceof MenuItem) 
	    handleMenuEvent(evt);
    }
}
