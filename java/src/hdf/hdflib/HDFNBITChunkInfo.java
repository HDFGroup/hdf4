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
 *  This class is a container for the parameters to the HDF
 *  NBIT compressed chunked class.
 * <p>
 * In this case, the information is the start bit, len, sign extension
 * and fill.
 */


public class HDFNBITChunkInfo extends HDFChunkInfo {

    public int[] chunk_lengths = new int[HDFConstants.MAX_VAR_DIMS];
    public int start_bit = 0;
    public int bit_len = 0;
    public int sign_ext = 0;
    public int fill_one = 0;

    public HDFNBITChunkInfo() {
        ctype = HDFConstants.HDF_NBIT;
    };

    public HDFNBITChunkInfo( int[] cl, int sb, int bl, int se, int fo) {
        ctype = HDFConstants.HDF_NBIT;
        chunk_lengths = cl;
        start_bit = sb;
        bit_len = bl;
        sign_ext = se;
        fill_one = fo;
    }

}
