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

public class WindowClosedProcess extends WindowAdapter {
  Frame frame = null;

  WindowClosedProcess()
  {
    super();
  }

  WindowClosedProcess(Frame frame)
  {
    super();
    this.frame = frame;
  }
  
  public void windowClosing(WindowEvent we)
  {
    if (frame != null)
       frame.dispose();
    else
       we.getWindow().dispose();
  }
}
