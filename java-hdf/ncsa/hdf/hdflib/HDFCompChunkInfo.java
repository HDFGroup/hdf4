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
 *  compressed, chunked class.
 * <p>
 * In this case, the information includes the compression
 * type, the model type, an appropriate HDFCompInfo object
 * and an appropriate HDFConfModel object.
 * <p>
 *  For details of the HDF libraries, see the HDF Documentation at:
 *     <a href="http://hdf.ncsa.uiuc.edu">http://hdf.ncsa.uiuc.edu</a>
 */


public class HDFCompChunkInfo extends HDFChunkInfo {

	public int[] chunk_lengths = new int[HDFConstants.MAX_VAR_DIMS];
	public int comp_type = 0;
	public int model_type = 0;
	public HDFCompInfo cinfo = null;
	public HDFCompModel cmodel = null;

	public HDFCompChunkInfo() {
		ctype = HDFConstants.HDF_COMP;
	};

	public HDFCompChunkInfo( int[] cl, int ct, int mt, HDFCompInfo ci, HDFCompModel cm) {
		ctype = HDFConstants.HDF_COMP;
		chunk_lengths = cl;
		comp_type = ct;
		model_type = mt;
		cinfo = ci;
		cmodel = cm;
	}

}
