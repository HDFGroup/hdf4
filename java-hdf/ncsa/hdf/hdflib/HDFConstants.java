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
 *  <p>
 *  This interface defines the values of constants defined
 *  by the HDF 4.1 API.
 * <p>
 *  For details of the HDF libraries, see the HDF Documentation at:
 *     <a href="http://hdf.ncsa.uiuc.edu">http://hdf.ncsa.uiuc.edu</a>
 */
public interface HDFConstants {

	// file access code definitions
	public static final int     DFACC_READ = 1;
	public static final int     DFACC_WRITE= 2;
	public static final int     DFACC_RDWR = 3;
	public static final int     DFACC_CREATE=4;
	public static final int     DFACC_RDONLY=DFACC_READ;
  
	public static final int     DFACC_DEFAULT=000;
	public static final int     DFACC_SERIAL=001;
	public static final int     DFACC_PARALLEL=011;


	/** annotation type in HDF */
	public static final int     AN_DATA_LABEL  = 0;
	public static final int     AN_DATA_DESC   = AN_DATA_LABEL + 1;
	public static final int     AN_FILE_LABEL  = AN_DATA_LABEL + 2;
	public static final int     AN_FILE_DESC   = AN_DATA_LABEL + 3;

	// HDF Tag Definations
	// HDF WILDCARD
	public static final int	    DFTAG_WILDCARD = 0;
	public static final int	    DFREF_WILDCARD = 0;
	// File identifier
	public static final int	    DFTAG_FID = 100;
	// File Description
	public static final int     DFTAG_FD  = 101;
	// Data Identifier Label
	public static final int     DFTAG_DIL = 104;
	// Data Identifier Annotation
	public static final int     DFTAG_DIA = 105;

	// 8-bits Raster image
	public static final int     DFTAG_RI8  = 202;
	public static final int     DFTAG_CI8  = 203;
	public static final int     DFTAG_II8  = 204;

	// 24-bits Raster image
	public static final int     DFTAG_RI  = 302;
	public static final int     DFTAG_CI  = 303;
	public static final int     DFTAG_RIG = 306;

	// SDS
	public static final int     DFTAG_SDG  = 700;
	public static final int     DFTAG_NDG  = 720;
	
	// Vgroup or Vdata
	public static final int     DFTAG_VH  = 1962;
	public static final int     DFTAG_VS  = 1963;
	public static final int     DFTAG_VG  = 1965;



	/** pixel interlacing scheme */
	public static  int   MFGR_INTERLACE_PIXEL = 0;

	/** line interlacing scheme */
	public static  int   MFGR_INTERLACE_LINE = MFGR_INTERLACE_PIXEL +1;

	/** component interlacing scheme */ 
	public static int MFGR_INTERLACE_COMPONENT = MFGR_INTERLACE_PIXEL +2;

	/** interlacing supported by the vset.*/
	public static final int FULL_INTERLACE = 0;
	public static final int NO_INTERLACE   = 1;

	/** unsigned char */
	public static final int DFNT_UCHAR8 = 3;
	public static final int DFNT_UCHAR  = 3;

	/** char */
	public static final int DFNT_CHAR8  = 4;
	public static final int DFNT_CHAR   = 4;

	/** No supported by HDF */
	public static final int DFNT_CHAR16 = 42;
	public static final int DFNT_UCHAR16= 43;


	/** float */
	public static final int  DFNT_FLOAT32   =  5;
	public static final int  DFNT_FLOAT     =  5 ;

	//** double */
	public static final int  DFNT_FLOAT64   =  6;
	public static final int  DFNT_FLOAT128  =  7 ; 
	public static final int  DFNT_DOUBLE    =  6  ;

	/** 8-bit integer */
	public static final int  DFNT_INT8      =  20;

	/** unsigned 8-bit interger */
	public static final int  DFNT_UINT8    =  21;

	/** short */
	public static final int  DFNT_INT16    =  22;

	/** unsigned interger */
	public static final int  DFNT_UINT16   =  23;

	/** interger */
	public static final int  DFNT_INT32    =  24;

	/** unsigned interger */
	public static final int  DFNT_UINT32   =  25;

	/** No supported */
	public static final int  DFNT_INT64    =  26;
	public static final int  DFNT_UINT64   =  27;
	public static final int  DFNT_INT128   =  28;
	public static final int  DFNT_UINT128  =  30;

	public static final int  DFNT_LITEND =  0x00004000;

	/** FAIL */
	public static int FAIL = -1;

	public static int DF_FORWARD  = 1;
	public static int  DFS_MAXLEN = 255;

	public static int COMP_NONE     =  0;
	public static int COMP_JPEG     =  2;
	public static int COMP_RLE      =  11;
	public static int COMP_IMCOMP   =  12;
	public static int COMP_CODE_NONE     =  0;
	public static int COMP_CODE_RLE     =  1;
	public static int COMP_CODE_NBIT     =  2;
	public static int COMP_CODE_SKPHUFF  =  3; 
	public static int COMP_CODE_DEFLATE  =  4;
	public static int COMP_CODE_INVALID  =  5;

	public static int COMP_MODEL_STDIO  =  0;


	/* Interlace schemes */
	public static int DFIL_PIXEL  = 0;  /* Pixel Interlacing */
	public static int DFIL_LINE   = 1;  /* Scan Line Interlacing */
	public static int DFIL_PLANE  = 2;  /* Scan Plane Interlacing */


	public static int SD_FILL  = 0;  
	public static int SD_NOFILL  = 0x100;
	public static int SD_DIMVAL_BW_COMP  = 1;  
	public static int SD_DIMVAL_BW_INCOMP  = 0;

	public static int HDF_NONE  = 0x0;
	public static int HDF_CHUNK  = 0x1;
	public static int HDF_COMP  = 0x3;
	public static int HDF_NBIT  = 0x5;

	public static int MAX_VAR_DIMS =32 ;

}
