/****************************************************************************
 * Copyright by The HDF Group.                                               *
 * Copyright by the Board of Trustees of the University of Illinois.         *
 * All rights reserved.                                                      *
 *                                                                           *
 * This file is part of HDF Java Products. The full HDF Java copyright       *
 * notice, including terms governing use, modification, and redistribution,  *
 * is contained in the file, COPYING.  COPYING can be found at the root of   *
 * the source code distribution tree. You can also access it online  at      *
 * https://www.hdfgroup.org/licenses.  If you do not have access to the      *
 * file, you may request a copy from help@hdfgroup.org.                      *
 ****************************************************************************/

package hdf.hdflib;

/**
 * <p>
 * This class is a container for the parameters to the HDF ``NBIT'' compression class.
 * <p>
 * In this case, the information needed is the number type, the sign extension, the fill bit, the
 * start bit, and the number of bits to store.
 */

public class HDFNBITCompInfo extends HDFNewCompInfo {

    /** number type of the data to encode */
    public int nt;
    /** whether to sign extend or not */
    public int sign_ext;
    /** whether to fill with 1's or 0's */
    public int fill_one;
    /** offset of the start bit in the data */
    public int start_bit;
    /** number of bits to store  */
    public int bit_len;

    /** */
    public HDFNBITCompInfo() { ctype = HDFConstants.COMP_CODE_NBIT; }

    /** */
    public HDFNBITCompInfo(int Nt, int Sign_ext, int Fill_one, int Start_bit, int Bit_len)
    {
        ctype = HDFConstants.COMP_CODE_NBIT;
    }
}
