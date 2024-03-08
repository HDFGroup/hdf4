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
 * This interface defines the values of constants defined by the HDF 4.2 API.
 */
public class HDFConstants {
    /** FAIL */
    public static final int FAIL = -1;

    // file access code definitions
    /** */
    public static final int DFACC_READ = 1;
    /** */
    public static final int DFACC_WRITE = 2;
    /** */
    public static final int DFACC_RDWR = 3;
    /** */
    public static final int DFACC_CREATE = 4;
    /** */
    public static final int DFACC_RDONLY = DFACC_READ;
    /** */
    public static final int DFACC_DEFAULT = 000;
    /** */
    public static final int DFACC_SERIAL = 001;
    /** */
    public static final int DFACC_PARALLEL = 011;

    // annotation type in HDF
    /** */
    public static final int AN_DATA_LABEL = 0;
    /** */
    public static final int AN_DATA_DESC = AN_DATA_LABEL + 1;
    /** */
    public static final int AN_FILE_LABEL = AN_DATA_LABEL + 2;
    /** */
    public static final int AN_FILE_DESC = AN_DATA_LABEL + 3;

    // HDF Tag Definitions
    /** */
    public static final int DFREF_WILDCARD = 0;
    /** */
    public static final int DFTAG_WILDCARD = 0;
    /** used by mfhdf/src/putget.c */
    public static final int DFREF_NONE = 0;

    // tags and refs
    /** */
    public static final int DFTAG_NULL = 1;
    /** linked-block special element */
    public static final int DFTAG_LINKED = 20;
    /** */
    public static final int DFTAG_VERSION = 30;
    /** compressed special element */
    public static final int DFTAG_COMPRESSED = 40;
    /** variable-len linked-block header */
    public static final int DFTAG_VLINKED = 50;
    /** variable-len linked-block data */
    public static final int DFTAG_VLINKED_DATA = 51;
    /** chunked special element header */
    public static final int DFTAG_CHUNKED = 60;
    /** chunk element */
    public static final int DFTAG_CHUNK = 61;

    // utility set
    /** File identifier */
    public static final int DFTAG_FID = 100;
    /** File description */
    public static final int DFTAG_FD = 101;
    /** Tag identifier */
    public static final int DFTAG_TID = 102;
    /** Tag descriptor */
    public static final int DFTAG_TD = 103;
    /** data identifier label */
    public static final int DFTAG_DIL = 104;
    /** data identifier annotation */
    public static final int DFTAG_DIA = 105;
    /** number type */
    public static final int DFTAG_NT = 106;
    /** machine type */
    public static final int DFTAG_MT = 107;
    /** free space in the file */
    public static final int DFTAG_FREE = 108;

    // raster-8 set
    /** 8-bit Image dimension */
    public static final int DFTAG_ID8 = 200;
    /** 8-bit Image palette */
    public static final int DFTAG_IP8 = 201;
    /** Raster-8 image */
    public static final int DFTAG_RI8 = 202;
    /** RLE compressed 8-bit image */
    public static final int DFTAG_CI8 = 203;
    /** IMCOMP compressed 8-bit image */
    public static final int DFTAG_II8 = 204;

    // Raster Image set
    /** Image DimRec */
    public static final int DFTAG_ID = 300;
    /** Image Palette */
    public static final int DFTAG_LUT = 301;
    /** Raster Image */
    public static final int DFTAG_RI = 302;
    /** Compressed Image */
    public static final int DFTAG_CI = 303;
    /** New-format Raster Image */
    public static final int DFTAG_NRI = 304;

    /** Raster Image Group */
    public static final int DFTAG_RIG = 306;
    /** Palette DimRec */
    public static final int DFTAG_LD = 307;
    /** Matte DimRec */
    public static final int DFTAG_MD = 308;
    /** Matte Data */
    public static final int DFTAG_MA = 309;
    /** color correction */
    public static final int DFTAG_CCN = 310;
    /** color format */
    public static final int DFTAG_CFM = 311;
    /** aspect ratio */
    public static final int DFTAG_AR = 312;

    /** Draw these images in sequence */
    public static final int DFTAG_DRAW = 400;
    /** run this as a program/script */
    public static final int DFTAG_RUN = 401;

    /** x-y position */
    public static final int DFTAG_XYP = 500;
    /** machine-type override */
    public static final int DFTAG_MTO = 501;

    // Tektronix
    /** TEK 4014 data */
    public static final int DFTAG_T14 = 602;
    /** TEK 4105 data */
    public static final int DFTAG_T105 = 603;

    // Scientific Data set
    // Objects of tag 721 are never actually written to the file. The tag is
    // needed to make things easier mixing DFSD and SD style objects in the
    // same file

    /** Scientific Data Group */
    public static final int DFTAG_SDG = 700;
    /** Scientific Data DimRec */
    public static final int DFTAG_SDD = 701;
    /** Scientific Data */
    public static final int DFTAG_SD = 702;
    /** Scales */
    public static final int DFTAG_SDS = 703;
    /** Labels */
    public static final int DFTAG_SDL = 704;
    /** Units */
    public static final int DFTAG_SDU = 705;
    /** Formats */
    public static final int DFTAG_SDF = 706;
    /** Max/Min */
    public static final int DFTAG_SDM = 707;
    /** Coord sys */
    public static final int DFTAG_SDC = 708;
    /** Transpose */
    public static final int DFTAG_SDT = 709;
    /** Links related to the dataset */
    public static final int DFTAG_SDLNK = 710;
    /** Numeric Data Group */
    public static final int DFTAG_NDG = 720;
    /** Calibration information */
    public static final int DFTAG_CAL = 731;
    /** Fill Value information */
    public static final int DFTAG_FV = 732;
    /** Beginning of required tags */
    public static final int DFTAG_BREQ = 799;
    /** List of ragged array line lengths */
    public static final int DFTAG_SDRAG = 781;
    /** Current end of the range */
    public static final int DFTAG_EREQ = 780;

    // VSets
    /** Vgroup */
    public static final int DFTAG_VG = 1965;
    /** Vdata Header */
    public static final int DFTAG_VH = 1962;
    /** Vdata Storage */
    public static final int DFTAG_VS = 1963;

    // compression schemes
    /** run length encoding */
    public static final int DFTAG_RLE = 11;
    /** IMCOMP compression alias */
    public static final int DFTAG_IMC = 12;
    /** IMCOMP compression */
    public static final int DFTAG_IMCOMP = 12;
    /** JPEG compression (24-bit data) */
    public static final int DFTAG_JPEG = 13;
    /** JPEG compression (8-bit data) */
    public static final int DFTAG_GREYJPEG = 14;
    /** JPEG compression (24-bit data) */
    public static final int DFTAG_JPEG5 = 15;
    /** JPEG compression (8-bit data) */
    public static final int DFTAG_GREYJPEG5 = 16;

    /** pixel interlacing scheme */
    public static final int MFGR_INTERLACE_PIXEL = 0;

    /** line interlacing scheme */
    public static final int MFGR_INTERLACE_LINE = MFGR_INTERLACE_PIXEL + 1;

    /** component interlacing scheme */
    public static final int MFGR_INTERLACE_COMPONENT = MFGR_INTERLACE_PIXEL + 2;

    /** interlacing supported by the vset. */
    public static final int FULL_INTERLACE = 0;
    /** */
    public static final int NO_INTERLACE = 1;

    /** unsigned char */
    public static final int DFNT_UCHAR8 = 3;
    /** */
    public static final int DFNT_UCHAR = 3;

    /** char */
    public static final int DFNT_CHAR8 = 4;
    /** */
    public static final int DFNT_CHAR = 4;

    /** Not supported by HDF */
    public static final int DFNT_CHAR16 = 42;
    /** */
    public static final int DFNT_UCHAR16 = 43;

    /** float */
    public static final int DFNT_FLOAT32 = 5;
    /** */
    public static final int DFNT_FLOAT = 5;

    /** double */
    public static final int DFNT_FLOAT64 = 6;
    /** */
    public static final int DFNT_FLOAT128 = 7;
    /** */
    public static final int DFNT_DOUBLE = 6;

    /** 8-bit integer */
    public static final int DFNT_INT8 = 20;

    /** unsigned 8-bit integer */
    public static final int DFNT_UINT8 = 21;

    /** short */
    public static final int DFNT_INT16 = 22;

    /** unsigned integer */
    public static final int DFNT_UINT16 = 23;

    /** integer */
    public static final int DFNT_INT32 = 24;

    /** unsigned integer */
    public static final int DFNT_UINT32 = 25;

    /** Not supported */
    public static final int DFNT_INT64 = 26;
    /** */
    public static final int DFNT_UINT64 = 27;
    /** */
    public static final int DFNT_INT128 = 28;
    /** */
    public static final int DFNT_UINT128 = 30;

    /** */
    public static final int DFNT_HDF = 0x00000000;
    /** */
    public static final int DFNT_NATIVE = 0x00001000;
    /** */
    public static final int DFNT_CUSTOM = 0x00002000;
    /** */
    public static final int DFNT_LITEND = 0x00004000;
    /** */
    public static final int DFNT_MASK = 0x00000fff;

    /** */
    public static final int DF_FORWARD = 1;
    /** */
    public static final int DFS_MAXLEN = 255;

    /** */
    public static final int COMP_NONE = 0;
    /** */
    public static final int COMP_JPEG = 2;
    /** */
    public static final int COMP_RLE = 11;
    /** */
    public static final int COMP_IMCOMP = 12;
    /** */
    public static final int COMP_CODE_NONE = 0;
    /** */
    public static final int COMP_CODE_RLE = 1;
    /** */
    public static final int COMP_CODE_NBIT = 2;
    /** */
    public static final int COMP_CODE_SKPHUFF = 3;
    /** */
    public static final int COMP_CODE_DEFLATE = 4;
    /** */
    public static final int COMP_CODE_SZIP = 5;
    /** */
    public static final int COMP_CODE_INVALID = 6;
    /** */
    public static final int COMP_CODE_JPEG = 7;

    // Interlace schemes
    /** Pixel Interlacing */
    public static final int DFIL_PIXEL = 0;
    /** Scan Line Interlacing */
    public static final int DFIL_LINE = 1;
    /** Scan Plane Interlacing */
    public static final int DFIL_PLANE = 2;

    /** */
    public static final int SD_UNLIMITED = 0;
    /** */
    public static final int SD_FILL = 0;
    /** */
    public static final int SD_NOFILL = 0x100;
    /** */
    public static final int SD_DIMVAL_BW_COMP = 1;
    /** */
    public static final int SD_DIMVAL_BW_INCOMP = 0;

    /** */
    public static final int HDF_NONE = 0x0;
    /** */
    public static final int HDF_CHUNK = 0x1;
    /** */
    public static final int HDF_COMP = 0x3;
    /** */
    public static final int HDF_NBIT = 0x5;
    /** */
    public static final int MAX_VAR_DIMS = 32;

    // the names of the Vgroups created by the GR interface
    /** */
    public static final String GR_NAME = "RIG0.0";
    /** */
    public static final String RI_NAME = "RI0.0";
    /** */
    public static final String RIGATTRNAME = "RIATTR0.0N";
    /** */
    public static final String RIGATTRCLASS = "RIATTR0.0C";

    // names of classes of the Vdatas/Vgroups created by the SD interface
    /** */
    public static final String HDF_ATTRIBUTE = "Attr0.0";
    /** */
    public static final String HDF_VARIABLE = "Var0.0";
    /** */
    public static final String HDF_DIMENSION = "Dim0.0";
    /** */
    public static final String HDF_UDIMENSION = "UDim0.0";
    /** */
    public static final String DIM_VALS = "DimVal0.0";
    /** */
    public static final String DIM_VALS01 = "DimVal0.1";
    /** */
    public static final String HDF_CHK_TBL = "_HDF_CHK_TBL_";
    /** */
    public static final String HDF_SDSVAR = "SDSVar";
    /** */
    public static final String HDF_CRDVAR = "CoordVar";

    public static final String HDF_CDF = "CDF0.0";

    // names of data object types
    /** */
    public static final String ANNOTATION = "HDF_ANNOTATION";
    /** */
    public static final String RI8 = "HDF_RI8";
    /** */
    public static final String RI24 = "HDF_RI24";
    /** */
    public static final String GR = "HDF_GR";
    /** */
    public static final String SDS = "HDF_SDS";
    /** */
    public static final String VDATA = "HDF_VDATA";
    /** */
    public static final String VGROUP = "HDF_GROUP";

    // data types represented by Strings
    /** */
    public static final String UCHAR8 = "UCHAR8";
    /** */
    public static final String CHAR8 = "CHAR8";
    /** */
    public static final String UCHAR16 = "UCHAR16";
    /** */
    public static final String CHAR16 = "CHAR16";
    /** */
    public static final String FLOAT32 = "FLOAT32";
    /** */
    public static final String FLOAT64 = "FLOAT64";
    /** */
    public static final String FLOAT128 = "FLOAT128";
    /** */
    public static final String INT8 = "INT8";
    /** */
    public static final String UINT8 = "UINT8";
    /** */
    public static final String INT16 = "INT16";
    /** */
    public static final String UINT16 = "UINT16";
    /** */
    public static final String INT32 = "INT32";
    /** */
    public static final String UINT32 = "UINT32";
    /** */
    public static final String INT64 = "INT64";
    /** */
    public static final String UINT64 = "UINT64";
    /** */
    public static final String INT128 = "INT128";
    /** */
    public static final String UINT128 = "UINT128";

    /**
     * convert number type to string type
     *
     * @param type
     *            the number representing the data type
     * @return the string representing the data type
     */
    public static String getType(int type)
    {
        if (type == HDFConstants.DFNT_UCHAR8) {
            return HDFConstants.UCHAR8;
        }
        else if (type == HDFConstants.DFNT_CHAR8) {
            return HDFConstants.CHAR8;
        }
        else if (type == HDFConstants.DFNT_UCHAR16) {
            return HDFConstants.UCHAR16;
        }
        else if (type == HDFConstants.DFNT_CHAR16) {
            return HDFConstants.CHAR16;
        }
        else if (type == HDFConstants.DFNT_FLOAT32) {
            return HDFConstants.FLOAT32;
        }
        else if (type == HDFConstants.DFNT_FLOAT64) {
            return HDFConstants.FLOAT64;
        }
        else if (type == HDFConstants.DFNT_FLOAT128) {
            return HDFConstants.FLOAT128;
        }
        else if (type == HDFConstants.DFNT_INT8) {
            return HDFConstants.INT8;
        }
        else if (type == HDFConstants.DFNT_UINT8) {
            return HDFConstants.UINT8;
        }
        else if (type == HDFConstants.DFNT_INT16) {
            return HDFConstants.INT16;
        }
        else if (type == HDFConstants.DFNT_UINT16) {
            return HDFConstants.UINT16;
        }
        else if (type == HDFConstants.DFNT_INT32) {
            return HDFConstants.INT32;
        }
        else if (type == HDFConstants.DFNT_UINT32) {
            return HDFConstants.UINT32;
        }
        else if (type == HDFConstants.DFNT_INT64) {
            return HDFConstants.INT64;
        }
        else if (type == HDFConstants.DFNT_UINT64) {
            return HDFConstants.UINT64;
        }
        else if (type == HDFConstants.DFNT_INT128) {
            return HDFConstants.INT128;
        }
        else if (type == HDFConstants.DFNT_UINT128) {
            return HDFConstants.UINT128;
        }
        else {
            return "Undefined Data Type";
        }
    }

    /**
     * convert string type to number type
     *
     * @param type
     *            the string representing the data type
     * @return the integer representing the data type
     */
    public static int getType(String type)
    {
        if (type.equalsIgnoreCase(HDFConstants.UCHAR8)) {
            return HDFConstants.DFNT_UCHAR8;
        }
        else if (type.equalsIgnoreCase(HDFConstants.CHAR8)) {
            return HDFConstants.DFNT_CHAR8;
        }
        else if (type.equalsIgnoreCase(HDFConstants.UCHAR16)) {
            return HDFConstants.DFNT_UCHAR16;
        }
        else if (type.equalsIgnoreCase(HDFConstants.CHAR16)) {
            return HDFConstants.DFNT_CHAR16;
        }
        else if (type.equalsIgnoreCase(HDFConstants.FLOAT32)) {
            return HDFConstants.DFNT_FLOAT32;
        }
        else if (type.equalsIgnoreCase(HDFConstants.FLOAT64)) {
            return HDFConstants.DFNT_FLOAT64;
        }
        else if (type.equalsIgnoreCase(HDFConstants.FLOAT128)) {
            return HDFConstants.DFNT_FLOAT128;
        }
        else if (type.equalsIgnoreCase(HDFConstants.INT8)) {
            return HDFConstants.DFNT_INT8;
        }
        else if (type.equalsIgnoreCase(HDFConstants.UINT8)) {
            return HDFConstants.DFNT_UINT8;
        }
        else if (type.equalsIgnoreCase(HDFConstants.INT16)) {
            return HDFConstants.DFNT_INT16;
        }
        else if (type.equalsIgnoreCase(HDFConstants.UINT16)) {
            return HDFConstants.DFNT_UINT16;
        }
        else if (type.equalsIgnoreCase(HDFConstants.INT32)) {
            return HDFConstants.DFNT_INT32;
        }
        else if (type.equalsIgnoreCase(HDFConstants.UINT32)) {
            return HDFConstants.DFNT_UINT32;
        }
        else if (type.equalsIgnoreCase(HDFConstants.INT64)) {
            return HDFConstants.DFNT_INT64;
        }
        else if (type.equalsIgnoreCase(HDFConstants.UINT64)) {
            return HDFConstants.DFNT_UINT64;
        }
        else if (type.equalsIgnoreCase(HDFConstants.INT128)) {
            return HDFConstants.DFNT_INT128;
        }
        else if (type.equalsIgnoreCase(HDFConstants.UINT128)) {
            return HDFConstants.DFNT_UINT128;
        }
        else {
            return -1;
        }
    }

    /**
     * gets the size of the data type in bytes, e.g size of DFNT_FLOAT32 = 4
     *
     * @param type
     *            the number representing the data type
     * @return the size of the data type
     */
    public static int getTypeSize(int type)
    {
        int size = 0;

        switch (type) {
        case HDFConstants.DFNT_UCHAR16:
        case HDFConstants.DFNT_CHAR16:
        case HDFConstants.DFNT_INT16:
        case HDFConstants.DFNT_UINT16:
            size = 2;
            break;
        case HDFConstants.DFNT_FLOAT32:
        case HDFConstants.DFNT_INT32:
        case HDFConstants.DFNT_UINT32:
            size = 4;
            break;
        case HDFConstants.DFNT_FLOAT64:
        case HDFConstants.DFNT_INT64:
        case HDFConstants.DFNT_UINT64:
            size = 8;
            break;
        case HDFConstants.DFNT_FLOAT128:
        case HDFConstants.DFNT_INT128:
        case HDFConstants.DFNT_UINT128:
            size = 16;
            break;
        default:
            size = 1;
            break;
        }

        return size;
    }
}
