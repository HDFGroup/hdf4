
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

// This class specifies new object to hold the sds slice information used by
// image animation . 
public class SDSDimInfo {
	
	// sds rank
	int	rank;
	// dimension size
	int	dims[];
	// states of each slice, which means which slice is active and 
	// animation is based on this active slice.
	boolean	states[];
	// current slices value, which means which slice is selected for one 
	// specified dimension. To pro-view the image this value is required.
	int	selected[];

    // default constructor
    public SDSDimInfo() {
    }

    // default constructor
    public SDSDimInfo(int rank, int[] dims, boolean[] states, int[] selects) {
	
	// rank of sds
	this.rank = rank;

	// dimension size
	this.dims = dims;

	// states of each slice
	this.states = states;

	// selected value of a slice
	this.selected = selects;
    }

    // default constructor
    public SDSDimInfo(int rank, int[] dims) {
	
	this(rank,dims,null,null);
    }

    // set slice selected value
    public void setSelectedValue(int[] values) {
	
	this.selected = values;
    }

    // set slice selected value
    public void setSelectedValue(int index, int value) {
	
	this.selected[index] = value;
    }

    // get rank
    public int getRank() {
	
	return this.rank;
    }

    // get dimension
    public int[] getDims() {
	
	return this.dims;
    }

    // get dimension
    public int getDim(int index) {
	
	return this.dims[index];
    }

    // get  selected  slice value
    public int[] getSelectedValues() {
	
	return this.selected ;
    }

    // get  selected  slice value with specified number
    public int  getSelectedValue( int index) {
	// based on zero
	return this.selected[index];
    }

    // set  slice state
    public void setStates(boolean states[]) {
	
	this.states = states;
    }

    // set  slice state
    public void setState(int index, boolean state) {
	
	this.states[index] = state;
    }

    // get  slice states
    public boolean[] getStates() {
	
	return this.states;
    }

    // get  slice state by specified  number
    public boolean getState(int num) {
	return this.states[num];
    }

    // get  first active slice
    public int getActiveSlice() {
	int ind = -1;
	int num = this.rank;
	for (int i=0; i<num; i++) {
		if (getState(i)) {
		   ind = i;
		   break;
		}
	}
	return ind;
    }

    // get image width
    public int getWidth() {
	int w = -1;
	if (getRank() == 2) 
	   w = this.dims[1];
	else {
	   int aSlice = getActiveSlice();
	   if  (aSlice <= 1)
		w = this.dims[2];
	   else
		w = this.dims[1];
	}
	return w;
    }

    // get image height
    public int getHeight() {
	int h = -1;
	if (getRank() == 2) 
	   h = this.dims[0]; // y
	else {
	   int aSlice = getActiveSlice();
	   if  (aSlice >= 1) // x, y
		h = this.dims[0];
	   else
		h = this.dims[1];
	}
	return h;
    }
}
