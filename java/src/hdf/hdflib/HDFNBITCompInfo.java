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
 *  ``NBIT'' compression class.
 * <p>
 * In this case, the information needed is the number type,
 * the sign extension, the fill bit, the start bit, and the
 * number of bits to store.
 */

public class HDFNBITCompInfo extends HDFNewCompInfo {

    public int   nt;          /* number type of the data to encode */
    public int    sign_ext;   /* whether to sign extend or not */
    public int    fill_one;   /* whether to fill with 1's or 0's */
    public int    start_bit;  /* offset of the start bit in the data */
    public int    bit_len;    /* number of bits to store */

    public HDFNBITCompInfo() {
        ctype = HDFConstants.COMP_CODE_NBIT;
    }

    public HDFNBITCompInfo(
            int   Nt,
            int    Sign_ext,
            int    Fill_one,
            int    Start_bit,
            int    Bit_len) {
        ctype = HDFConstants.COMP_CODE_NBIT;
    }

}
