/****************************************************************************
 * Copyright by The HDF Group.                                               *
 * Copyright by the Board of Trustees of the University of Illinois.         *
 * All rights reserved.                                                      *
 *                                                                           *
 * This file is part of HDF Java Products. The full HDF Java copyright       *
 * notice, including terms governing use, modification, and redistribution,  *
 * is contained in the file, COPYING.  COPYING can be found at the root of   *
 * the source code distribution tree. You can also access it online  at      *
 * http://www.hdfgroup.org/products/licenses.html.  If you do not have       *
 * access to the file, you may request a copy from help@hdfgroup.org.        *
 ****************************************************************************/

package hdf.hdflib;

/**
 * <p>
 *   This is a generic class to represent the HDF chunk_info
 *   union, which contains parameters for the different
 *   chunking schemes.
 * <p>
 * The variant parameters are expressed as sub-classes of this
 * class.
 */


public class HDFChunkInfo {
	public int ctype;
	public int[] chunk_lengths = new int[HDFConstants.MAX_VAR_DIMS];
	public int comp_type = HDFConstants.COMP_CODE_NONE;
	public HDFCompInfo cinfo = null;

	public HDFChunkInfo() {
		ctype = HDFConstants.HDF_NONE;
	}

	public HDFChunkInfo( int[] cl, int ct, HDFCompInfo ci ) {
		if (ct == HDFConstants.COMP_CODE_NONE) {
			ctype = HDFConstants.HDF_CHUNK;
		} else {
			ctype = HDFConstants.HDF_COMP | HDFConstants.HDF_CHUNK;
		}
		chunk_lengths = cl;
		comp_type = ct;
		cinfo = ci;
	}

	public HDFChunkInfo(int[] cl) {
		ctype = HDFConstants.HDF_CHUNK;
		chunk_lengths = cl;
	}
}
