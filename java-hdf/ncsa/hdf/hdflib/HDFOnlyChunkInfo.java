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
 *  uncompressed chunked class.
 * <p>
 * In this case, the information is the chunk lengths
 * <p>
 *  For details of the HDF libraries, see the HDF Documentation at:
 *     <a href="http://hdf.ncsa.uiuc.edu">http://hdf.ncsa.uiuc.edu</a>
 */

public class HDFOnlyChunkInfo extends HDFChunkInfo {

	public int[] chunk_lengths = new int[HDFConstants.MAX_VAR_DIMS];

	public HDFOnlyChunkInfo() {
		ctype = HDFConstants.HDF_CHUNK;
	};

	public HDFOnlyChunkInfo(int[] cl) {
		ctype = HDFConstants.HDF_CHUNK;
		chunk_lengths = cl;	
	}

}
