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

package ncsa.hdf.jhv;

import java.awt.*;
import java.awt.event.*;

public class JHVWindowAdapter extends WindowAdapter {
  JHV jhv;
 
  JHVWindowAdapter(JHV jhv)
  {
    this.jhv = jhv;
  }

  public void windowClosing(WindowEvent we)
  {
    jhv.vdataFrameDisplayed = false;
    we.getWindow().dispose();
  }
}
