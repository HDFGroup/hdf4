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
 *   This is a generic class to represent the HDF chunk_info
 *   union, which contains parameters for the different
 *   chunking schemes.
 * <p>
 * The variant parameters are expressed as sub-classes of this
 * class.
 * <p>
 *  For details of the HDF libraries, see the HDF Documentation at:
 *     <a href="http://hdf.ncsa.uiuc.edu">http://hdf.ncsa.uiuc.edu</a>
 *
 */


public class HDFChunkInfo {
	public int ctype;
	public HDFChunkInfo() {
		ctype = HDFConstants.HDF_NONE;
	} ;
}


