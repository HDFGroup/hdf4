
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

import  ncsa.hdf.awt.event.*;

public class SliderEventMulticaster implements SliderListener {

	protected  SliderListener 	a,b;

    public SliderEventMulticaster(SliderListener a, SliderListener b) {

	this.a = a;
	this.b = b;
    }

    static SliderListener add ( SliderListener a, SliderListener b)  {
 	if (a == null) 
	   return b;
	else if (b==null)
	     	 return a;
	     else
	  	return new SliderEventMulticaster(a,b);
    }

    static SliderListener remove ( SliderListener a, SliderListener b)  {
 	if ((a == null) || (a==b))
	   return null;
	else if (a instanceof SliderEventMulticaster)
		 return add (remove (((SliderEventMulticaster)a).a,b),
			     remove (((SliderEventMulticaster)a).b,b));
	     else
	     	 return a;
    }

    /**
     * SliderListener stuff
     */
    public void sliderStateChanged(SliderEvent evt)  {
	a.sliderStateChanged(evt);
	b.sliderStateChanged(evt);
    }
}
