
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
    Provides for the editing of a list of text strings. There is a text entry 
    Textfield where new string can be entered with an add and a delete button
    to add and remove things from the list.
/**
 *   Provides for the editing of a list of text strings. There is a text entry 
 *   Textfield where new string can be entered with an add and a delete button
 *   to add and remove things from the list.
 *
 *   @version   1.1.3 October 17 1997
 *   @author    Peter X. Cao
 */

package ncsa.hdf.awt;

import java.awt.*;
import java.awt.event.*;
import java.util.Vector;

public class HostList extends OutlinePanel
{
    final String INSTRUCTION   = "Enter machine_name:port_number, e.g. mymachine.edu:8181";
    final String ERROR   = "Error: invalid server host, Please check the entry format";

    protected List      fNewList;
    protected TextField fEnterDomain;
    protected TextField servletField;
    protected Button    fDeleteSelectedItems;
    protected Button    fAddNewItem;
    protected Button    fEditItem;
    protected TextArea  message;
    protected Checkbox  javaApplication;
    protected Checkbox  javaWebServer;
    protected Checkbox  otherServer;
 
    
    public HostList (String name)
    {
        super (name);

        fNewList = new List (6);
        fNewList.addMouseListener(this);
        this.setLayout (new GridBagLayout ());
        
       // Add the text entry field
        fEnterDomain = new TextField ();
        fEnterDomain.addKeyListener(this);
        OutlinePanel domainPanel = new OutlinePanel("Machine_name:port_number");
        domainPanel.setLayout (new GridBagLayout ());
        domainPanel.setConstraints(fEnterDomain,0, 0, 1, 1,GridBagConstraints.BOTH,
            GridBagConstraints.WEST, 1.0, 1.0,new Insets (0, 0, 0, 0));

        // Add the path field
        servletField = new TextField ("HDFServer");
        servletField.addKeyListener(this);
        servletField.setEnabled(false);
        OutlinePanel servletPanel = new OutlinePanel("Servelt Name");
        servletPanel.add(servletField);
        servletPanel.setLayout (new GridBagLayout ());
        servletPanel.setConstraints(servletField,0, 0, 1, 1,GridBagConstraints.BOTH,
            GridBagConstraints.WEST, 1.0, 1.0,new Insets (0, 0, 0, 0));


         // delete selected items from the list.
        fDeleteSelectedItems = new Button ("Delete");
        fDeleteSelectedItems.addMouseListener(this);
        fDeleteSelectedItems.setEnabled (false);
        
        // Add new user
        fAddNewItem = new Button ("  Add ");
        fAddNewItem.addMouseListener(this);
        fAddNewItem.setEnabled (false);
        
        // Add new user
        fEditItem = new Button ("Update");
        fEditItem.addMouseListener(this);
        fEditItem.setEnabled (false);

        //add message area
        message = new TextArea(INSTRUCTION, 1, 80, TextArea.SCROLLBARS_NONE);
        message.setForeground(Color.red);
        message.setEditable(false);

        // setup check boxes
        OutlinePanel checkBoxPanel = new OutlinePanel("Server type");
        CheckboxGroup cbg = new CheckboxGroup();
        javaApplication = new Checkbox("JavaApplication", cbg, true);
        javaWebServer = new Checkbox("JavaWebServer", cbg, false);
        otherServer = new Checkbox("Other", cbg, false);
        javaApplication.addMouseListener(this);
        javaWebServer.addMouseListener(this);
        otherServer.addMouseListener(this);
        checkBoxPanel.setLayout(new GridLayout(3, 1));
        checkBoxPanel.add(javaApplication);
        checkBoxPanel.add(javaWebServer);
        checkBoxPanel.add(otherServer);
 
        // Place all the objects in the grid bag layout.
        this.setConstraints (domainPanel,    0, 0, 2, 1, 
            GridBagConstraints.HORIZONTAL, GridBagConstraints.WEST, 0.0, 0.0,
            new Insets (12, 2, 2, 2));

        // Place all the objects in the grid bag layout.
        this.setConstraints (servletPanel,    2, 0, 1, 1, 
            GridBagConstraints.HORIZONTAL, GridBagConstraints.WEST, 0.0, 0.0,
            new Insets (12, 2, 2, 2));
        
        // Next row
        this.setConstraints (fAddNewItem,    0, 1, 1, 1, 
            GridBagConstraints.NONE, GridBagConstraints.NORTHWEST, 0.0, 0.0);
        this.setConstraints (fNewList,         1, 1, 1, 3, 
            GridBagConstraints.BOTH, GridBagConstraints.NORTH, 1.0, 1.0);
        
        // Next
        this.setConstraints (fDeleteSelectedItems, 0, 2, 1, 1, 
            GridBagConstraints.NONE, GridBagConstraints.NORTHWEST, 0.0, 0.0);
        this.setConstraints (fEditItem,         0, 3, 1, 3, 
            GridBagConstraints.NONE, GridBagConstraints.NORTHWEST, 0.0, 0.0);

        // the check boxes.
        this.setConstraints (checkBoxPanel, 2, 1, 1, 3, 
            GridBagConstraints.BOTH, GridBagConstraints.NORTH, 0.0, 0.0);

        // the message text area.
        this.setConstraints (message,    0, 5, 3, 1, 
            GridBagConstraints.HORIZONTAL, GridBagConstraints.SOUTH, 0.0, 0.0,
            new Insets (2, 2, 2, 2));

    }
    
    /**
        Handle events that occure in the list editor panel. If the delete button is
        clicked, delete all selected items form the list. If the add button is clicked, 
        add whatever text is in the text field to the list, and reset the text field.
        @param event the event that occured
    */
    public void mouseClicked(MouseEvent event)
    {
        int selectedIndex = fNewList.getSelectedIndex();
        Object source = event.getSource();

        if (source == fNewList) 
        {
            String selection = fNewList.getSelectedItem();
            if (selection != null)
            {
                String prefix = selection.substring(0, 7);
                if (prefix.equalsIgnoreCase("http://"))
                {
                    String suffix = selection.substring(7);
                    int servletIndex = suffix.indexOf("/servlet");
                    fEnterDomain.setText (suffix.substring(0, servletIndex));
                    javaWebServer.setState(true);
                    servletField.setEnabled(true);
                }
                else
                {
                    fEnterDomain.setText (selection);
                    javaApplication.setState(true);
                    servletField.setEnabled(false);
                }
                fDeleteSelectedItems.setEnabled (true);
                fEditItem.setEnabled (true);
                fAddNewItem.setEnabled (false);
            } else {
                fDeleteSelectedItems.setEnabled (false);
                fEditItem.setEnabled (false);
                fAddNewItem.setEnabled (false);
                javaApplication.setState(true);
            }

            return;
        }
        else if (source == fAddNewItem)
        {
            if (selectedIndex >= 0)
                fEditItem.setEnabled(true);

            String newEntry = fEnterDomain.getText().trim();
	if (newEntry.length() > 0)  {
            String prefix = newEntry.substring(0, 7);

            if (javaWebServer.getState())
            {
                if ( !prefix.equalsIgnoreCase("http://"))
                    newEntry = "http://"+newEntry;
                newEntry = newEntry+"/servlet/"+servletField.getText();
            }
            else if ( prefix.equalsIgnoreCase("http://"))
                newEntry = newEntry.substring(7);

            if (isNewEntry (newEntry))
            {
                if (!isValidHost (newEntry))
                    message.setText(ERROR); 
                else
                {
                    message.setText(INSTRUCTION);
                    fNewList.addItem (newEntry);
                    clean();
                }
            }
	} else {
                message.setText("ERROR: no selected item to be added"); }
        }
        else if (source == fEditItem)
        {
            if (selectedIndex < 0)
            {
                message.setText("ERROR: no selected item to be edited");
                return;
            }

            fEditItem.setEnabled(true);

            String newEntry = fEnterDomain.getText().trim();
            String prefix = newEntry.substring(0, 7);

            if (javaWebServer.getState())
            {
                if ( !prefix.equalsIgnoreCase("http://"))
                    newEntry = "http://"+newEntry;
                newEntry = newEntry+"/servlet/"+servletField.getText();
            }
            else if ( prefix.equalsIgnoreCase("http://"))
                newEntry = newEntry.substring(7);

            if (isNewEntry (newEntry))
            {
                if (!isValidHost (newEntry))
                    message.setText(ERROR); 
                else
                {
                    message.setText(INSTRUCTION);
                    fNewList.replaceItem(newEntry, selectedIndex);
                    clean();
                }
            }
            else clean();
        } 
        else if(source == fDeleteSelectedItems) 
        {
            if (selectedIndex < 0)
            {
                message.setText("ERROR: no selected item to be deleted");
                return;
            }

            fNewList.delItem(selectedIndex);
            clean();
        }
        else if(source == javaApplication)
        {
            servletField.setText ("HDFServer");
            servletField.setEnabled(false);
        }
        else if(source == javaWebServer) 
        {
            servletField.setText ("HDFServer");
            servletField.setEnabled(true);
        }
        else if(source == otherServer)
        {
            servletField.setText ("HDFServer");
            servletField.setEnabled(false);
        }

    }


    public void clean()
    {
        fDeleteSelectedItems.setEnabled (false);
        fEditItem.setEnabled (false);
        fAddNewItem.setEnabled (false);
        fEnterDomain.setText ("");
        servletField.setText ("HDFServer");
        message.setText(INSTRUCTION);
    }

    public void keyTyped(KeyEvent event)
    {
        if (event.getSource() == fEnterDomain)
            fAddNewItem.setEnabled (true);
    }

    boolean isValidHost (String host)
    {
        String jhost = "";
        String jport = "";
        int index;
        int port = -1;
        String scratch = host.trim();
        index = scratch.indexOf (":");

        if ( index <=0 ) return false;
        else
        {
            jhost = scratch.substring (0, index);
            if (javaWebServer.getState())
            {
                String scratch2 = scratch.substring(7);
                int portIndex = scratch2.indexOf (":");
                int servletIndex = scratch2.indexOf("/servlet");
                jport = scratch2.substring(portIndex+1, servletIndex);
            }
            else
                jport = scratch.substring (index + 1);
            try { port = Integer.parseInt(jport); }
            catch (NumberFormatException e) { System.out.println(e.toString());return false; }
        }

        return true;
    }

    /**
    *   check if the item is new
    */
    public boolean isNewEntry (String newEntry) 
    {
        int length = fNewList.getItemCount();
        for (int i=0; i<length; i++)
        {
            if (fNewList.getItem(i).trim().equals(newEntry.trim()))
                return false;
        }
        return true;
    }
    
    /**
        Get a vector containing the each string in the list.
        @returns a vector with the list of items..
    */
    public Vector getVector ()
    {
        int length = fNewList.getItemCount();
        Vector copyVector = new Vector (length);
        
        for (int i=0; i<length; i++)
        {
            String itemString = fNewList.getItem(i);
            copyVector.addElement (itemString);
        }
        return copyVector;
    }
    
    /**
        This method initializes the list with some list of strings
        @param listValues a list of strings to be inserted into the list.
    */
    public void setVector (Vector list)
    {
        int length;
        if (list != null)
        {
            length = fNewList.getItemCount(); 
            for (int i = 0; i < length; i++)
                fNewList.delItem(0);
                
            for (int i = 0; i < list.size (); i++)
                fNewList.addItem((String) list.elementAt (i));
        }
    }
    
    /**
        This method initializes the list with some list of strings
        @param listValues a list of strings to be inserted into the list.
    */
    public void listCopy (List list1)
    {
        int length;
        if (list1 != null)
        {
            length = list1.getItemCount(); 
            for (int i=0; i<length; i++)
                fNewList.addItem(list1.getItem(i));
        }
    }
    
    /**
        Get the list
        @returns returns the list.
    */
    public List getList ()
    {
        return fNewList;
    }
    
    /**
        This method will copy all the strings in the list to a vector, and return
        the vector.
        @returns a vector that contains a copy of each string in the list.
    */
    public Vector getResultList ()
    {
        // Add the string from the entry text to the list.
        Vector listItems = new Vector ();
        int length = fNewList.getItemCount();
        for (int i = 0; i < length; i++)
        {
            String newEntry = fNewList.getItem (i);
            listItems.addElement (newEntry);
        }
        return listItems;
    }

}
