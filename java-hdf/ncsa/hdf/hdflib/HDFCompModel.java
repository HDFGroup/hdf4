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

package ncsa.hdf.hdflib;

/**
 * <p>
 *  This class is a container for the parameters to the HDF
 *  compression model structure.
 * <p>
 *  The compression model encapsulates the data space to
 *  be compressed:  the number type, number of dimensions,
 *  and size of each dimension. 
 * <p>
 *  For details of the HDF libraries, see the HDF Documentation at:
 *     <a href="http://hdf.ncsa.uiuc.edu">http://hdf.ncsa.uiuc.edu</a>
 */

public class HDFCompModel {

	public int comp_model;

	public int nt;
	public int ndim;
	public int [] dims; /* this is length 'ndims' */

	public HDFCompModel() {
		comp_model =  -1;

		nt = -1;
		ndim = -1;
		dims = null;
	}

	public HDFCompModel(int cm, int Nt, int nd, int [] Dims) {
		comp_model =  cm;

		nt = Nt;
		ndim = nd;
		dims = Dims;
	}
	
	public int[] getDims() {
		return dims;
	}

	public int getNDim() {
		return ndim;
	}
}
