
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


package ncsa.hdf.awt.event;

import java.awt.AWTEvent;

public class SliderEvent extends AWTEvent {

	public static final int SLIDER_CHANGED = AWTEvent.RESERVED_ID_MAX + 1;
	protected	int	value;

    public SliderEvent(Object obj, int value) {
	super(obj, SLIDER_CHANGED);
	this.value = value;
    }

    public int getValue() {
	return this.value;
    }

    public String  toString() {
	return ("Slider: " + getValue());
    }
}
