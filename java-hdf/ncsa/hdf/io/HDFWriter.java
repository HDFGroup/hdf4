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

package ncsa.hdf.io;

import ncsa.hdf.hdflib.HDFLibrary;
import ncsa.hdf.hdflib.HDFConstants;
import ncsa.hdf.hdflib.HDFException;
import ncsa.hdf.hdflib.HDFTable;
import ncsa.hdf.hdflib.HDFNativeData;

/**
 *  HDFWriter writes HDF Objects into an HDF file
 
 *  @version   1.1.3 December 11 1997
 *  @author    Peter X. Cao
 *  @see       java.io.Writer
 */
public class HDFWriter
{
    /** the HDFLibrary to access HDF files */
    HDFLibrary hdf = null;

    /** the name of the HDF file */
    String filename = "";

    /** creates a HDFWriter with specific filename */
    public HDFWriter(String filename)
    {
        hdf = new HDFLibrary();
        this.filename = filename;
    }

    /**
     *  creates a new HDF file
     *
     *  @exception ncsa.hdf.hdflib.HDFException 
     *             thrown when the file cannot be openned
     *
     */
    public boolean createHDF() throws HDFException
    {
        int fid = hdf.Hopen(filename, HDFConstants.DFACC_CREATE);
        if ( fid <= 0 ) { throw new HDFException ("Create Failed");} 

        hdf.Hclose(fid);
        return true;
    }


    /**
     *  write file description
     *
     *  @annotation  the file annotation
     *  @access      the access mode
     *  @return      true if the access to the HDF file is secceded
     *
     *  @exception ncsa.hdf.hdflib.HDFException 
     *             thrown when file cannot be written
     *
     */
    public boolean writeAnnotation(String annotation, int access)
        throws HDFException
    {
        int fid  = -1;
        int anid = HDFConstants.FAIL;
        int iid  = HDFConstants.FAIL;
        boolean retValue = false;

	//try {
		fid = hdf.Hopen(filename, access);
	//} catch (Exception e) { System.out.println("Hopen failed: "+e.getMessage());}
//        fid = hdf.Hopen(filename, access);
        if (fid <= 0 ) { throw new HDFException ("Create Failed");}//return false;

        iid = hdf.ANstart(fid);
        if (iid != HDFConstants.FAIL)
        {
            anid = hdf.ANcreatef(iid, (short)HDFConstants.AN_FILE_LABEL);
            if (anid != HDFConstants.FAIL)
                retValue = hdf.ANwriteann(anid, annotation, annotation.length());
        }

        hdf.ANendaccess(anid);
        hdf.ANend(iid);
        hdf.Hclose(fid);

        return retValue;

    }

    /**
     *  write GR image data
     *
     *  @access      the access mode
     *  @name        the name of the GR data
     *  @ncomp       the number of components
     *  @type        the type of the GR data
     *  @interlace   the interlacing scheme of the GR image
     *  @dimension   the dimesion of the GR image
     *  @start       the starting points
     *  @data        the object array holding the GR image data
     *  @palette     the palette data of the GR image
     *  @index       the index of image
     *  @return      true if the access to the HDF file is succeded
     *
     *  @exception ncsa.hdf.hdflib.HDFException 
     *             thrown when file cannot be written
     *
     */
    public boolean writeGR(int access, String name, int ncomp, int type,
        int interlace, int[] dimension, int[] start, int[] stride, int index,
        Object data, byte[] palette)
        throws HDFException
    {
        int fid  = -1;
        int grid = HDFConstants.FAIL;
        int iid  = HDFConstants.FAIL;
        boolean retValue = false;

        fid = hdf.Hopen(filename, access);
        if (fid <= 0 ) { throw new HDFException ("Create Failed");}//return false;

        iid = hdf.GRstart(fid);
        if (iid != HDFConstants.FAIL)
        {
            grid = hdf.GRcreate(iid, name, ncomp, type, interlace, dimension);
            if (grid != HDFConstants.FAIL) {

                // write image data to a file
                if (data != null) {
                    int[] edges = new int[dimension.length];
                    for (int i=0; i<dimension.length; i++)
                    {
                        if (stride == null || stride[i] == 0)
                            edges[i] = dimension[i];
                        else
                            edges[i] = dimension[i]/stride[i];
                    }
                    retValue = hdf.GRwriteimage(grid, start, stride, edges, data);
                }
                 /* Write the palette data to a file. */
                if (palette == null) palette = createRainbowPalette();
                int pal_id = hdf.GRgetlutid(grid, index);
	        retValue = hdf.GRwritelut(pal_id,3,HDFConstants.DFNT_UINT8,interlace, 256, palette);
            }
        }

        hdf.GRendaccess(grid);
        hdf.GRend(iid);
        hdf.Hclose(fid);

        return retValue;
    }

    /**
     *  write SDS data
     *
     *  @access      the access mode
     *  @name        the name of the GR data
     *  @rank        the rank
     *  @type        the type of the GR data
     *  @dimension   the dimesion of the GR image
     *  @start       the starting points
     *  @stride      the stride
     *  @data        the object array holding the GR image data
     *  @return      true if the access to the HDF file is succeded
     *
     *  @exception ncsa.hdf.hdflib.HDFException 
     *             thrown when file cannot be written
     *
     */
    public boolean writeSDS(int access, String name, int rank, int type,
        int[] dimension, int[] start, int[] stride, Object data)
        throws HDFException
    {
        int sdsid = HDFConstants.FAIL;
        int iid  = HDFConstants.FAIL;
        boolean retValue = false;

        iid = hdf.SDstart(filename, access);
        if (iid != HDFConstants.FAIL)
        {
            sdsid = hdf.SDcreate(iid, name, type, rank, dimension);
            if (sdsid != HDFConstants.FAIL) {

                // write image data to a file
                if (data != null) {
                    int[] count = new int[dimension.length];
                    for (int i=0; i<dimension.length; i++)
                    {
                        if (stride == null || stride[i] == 0)
                            count[i] = dimension[i];
                        else
                            count[i] = dimension[i]/stride[i];
                    }
                    retValue = hdf.SDwritedata(sdsid, start, stride, count, data);
                }
            }
        }

        hdf.SDendaccess(sdsid);
        hdf.SDend(iid);

        return retValue;
    }


    /**
     *  Writes a single data record to the specified vdata
     *
     *  @access      the access mode
     *  @name        the name of the GR data
     *  @ref         Reference number for the vdata to open
     *  @type        the type of data
     *  @data        Buffer of records to be stored in the vdata
     *  @n_records   Number of records to be stored
     *  @return      true if the access to the HDF file is succeded
     *
     *  @exception ncsa.hdf.hdflib.HDFException 
     *             thrown when file cannot be written
     *
     */
    public boolean writeVdata(int access, String name, int ref, int type,
        String fieldName, int fieldOrder, Object data, int n_records)
        throws HDFException
    {
        int fid = HDFConstants.FAIL;
        int vid = HDFConstants.FAIL;
        boolean retValue = false;

        if (data == null) return false;

        fid = hdf.Hopen(filename, access);
        if (fid <= 0 ) { throw new HDFException ("Create Failed");} //return false;

        if (hdf.Vstart(fid))
        {
            vid = hdf.VSattach(fid, ref, "w");
            hdf.VSfdefine(vid, fieldName, type, fieldOrder);
            hdf.VSsetfields(vid, fieldName);
            retValue = (hdf.VSwrite(vid, data, n_records,
            HDFConstants.FULL_INTERLACE) <=0 ) ? false : true;

           if (retValue && name !=null && name.length() > 0)
            {
                hdf.VSsetname(vid, name);
                //hdf.VSsetclass(vid, name);
            }
            hdf.VSdetach(vid);
        }

        hdf.Vend(fid);
        hdf.Hclose(fid);

        return retValue;
    }

    /**
     *  Writes all data records to the specified vdata
     *
     *  @access      the access mode
     *  @name        the name of the GR data
     *  @ref         Reference number for the vdata to open
     *  @type        the type of data
     *  @data        Buffer of records to be stored in the vdata
     *  @n_records   Number of records to be stored
     *  @interlace   the type of interlace
     *  @return      true if the access to the HDF file is succeded
     *
     *  @exception ncsa.hdf.hdflib.HDFException 
     *             thrown when file cannot be written
     *
     */
    public boolean writeVdata(int access, String name, int ref, int[] types,
        String[] fieldNames, int[] fieldOrders, Object data, int n_records,
        int interlace)
        throws HDFException
    {
        int fid = HDFConstants.FAIL;
        int vid = HDFConstants.FAIL;
        boolean retValue = false;

        if (types == null       ||
            fieldNames == null  ||
            fieldOrders == null ||
            data == null)
            return false;

        int n_fields = types.length;

        fid = hdf.Hopen(filename, access);
        if (fid <= 0 ) { throw new HDFException ("Create Failed");} //return false;

        if (hdf.Vstart(fid))
        {
            vid = hdf.VSattach(fid, ref, "w");
            String allNames = "";
            for (int i=0; i<n_fields; i++) {
                hdf.VSfdefine(vid, fieldNames[i], types[i], fieldOrders[i]);
                allNames += ","+ fieldNames[i];
            }
            hdf.VSsetfields(vid, allNames.substring(1));
            retValue = (hdf.VSwrite(vid, data, n_records, interlace) <=0 )
                ? false : true;

           if (retValue && name !=null && name.length() > 0)
            {
                hdf.VSsetname(vid, name);
                //hdf.VSsetclass(vid, name);
            }
            hdf.VSdetach(vid);
        }

        hdf.Vend(fid);
        hdf.Hclose(fid);

        return retValue;
    }

    /**
     *  Return the RAINBOW palette
     *  rgb rgb rgb rgb rgb ....
     */
    public static byte[] createRainbowPalette()
    {
        int[] rainbowValues = {
        0x00, 0x00, 0x00, 0x7c, 0x00, 0xff, 0x78, 0x00, 0xfe, 0x73, 0x00, 0xff,
        0x6f, 0x00, 0xfe, 0x6a, 0x00, 0xff, 0x66, 0x00, 0xfe, 0x61, 0x00, 0xff,
        0x5d, 0x00, 0xfe, 0x58, 0x00, 0xff, 0x54, 0x00, 0xfe, 0x4f, 0x00, 0xff,
        0x4b, 0x00, 0xfe, 0x46, 0x00, 0xff, 0x42, 0x00, 0xfe, 0x3d, 0x00, 0xff,
        0x39, 0x00, 0xfe, 0x34, 0x00, 0xff, 0x30, 0x00, 0xfe, 0x2b, 0x00, 0xff,
        0x27, 0x00, 0xfe, 0x22, 0x00, 0xff, 0x1e, 0x00, 0xfe, 0x19, 0x00, 0xff,
        0x15, 0x00, 0xfe, 0x10, 0x00, 0xff, 0x0c, 0x00, 0xfe, 0x07, 0x00, 0xff,
        0x03, 0x00, 0xfe, 0x00, 0x02, 0xff, 0x00, 0x06, 0xfe, 0x00, 0x0b, 0xff,
        0x00, 0x0f, 0xfe, 0x00, 0x14, 0xff, 0x00, 0x18, 0xfe, 0x00, 0x1d, 0xff,
        0x00, 0x21, 0xfe, 0x00, 0x26, 0xff, 0x00, 0x2a, 0xfe, 0x00, 0x2f, 0xff,
        0x00, 0x33, 0xfe, 0x00, 0x38, 0xff, 0x00, 0x3c, 0xfe, 0x00, 0x41, 0xff,
        0x00, 0x45, 0xfe, 0x00, 0x4a, 0xff, 0x00, 0x4e, 0xfe, 0x00, 0x53, 0xff,
        0x00, 0x57, 0xfe, 0x00, 0x5c, 0xff, 0x00, 0x60, 0xfe, 0x00, 0x65, 0xff,
        0x00, 0x69, 0xfe, 0x00, 0x6e, 0xff, 0x00, 0x72, 0xfe, 0x00, 0x77, 0xff,
        0x00, 0x7a, 0xfe, 0x00, 0x80, 0xff, 0x00, 0x83, 0xfe, 0x00, 0x89, 0xff,
        0x00, 0x8c, 0xfe, 0x00, 0x92, 0xff, 0x00, 0x95, 0xfe, 0x00, 0x9b, 0xff,
        0x00, 0x9e, 0xfe, 0x00, 0xa4, 0xff, 0x00, 0xa7, 0xfe, 0x00, 0xad, 0xff,
        0x00, 0xb0, 0xfe, 0x00, 0xb6, 0xff, 0x00, 0xb9, 0xfe, 0x00, 0xbf, 0xff,
        0x00, 0xc2, 0xfe, 0x00, 0xc8, 0xff, 0x00, 0xcb, 0xfe, 0x00, 0xd1, 0xff,
        0x00, 0xd4, 0xfe, 0x00, 0xda, 0xff, 0x00, 0xdd, 0xfe, 0x00, 0xe3, 0xff,
        0x00, 0xe6, 0xfe, 0x00, 0xec, 0xff, 0x00, 0xf0, 0xfe, 0x00, 0xf5, 0xff,
        0x00, 0xf9, 0xfe, 0x00, 0xfe, 0xff, 0x00, 0xfe, 0xfa, 0x00, 0xff, 0xf7,
        0x00, 0xfe, 0xf1, 0x00, 0xff, 0xee, 0x00, 0xfe, 0xe8, 0x00, 0xff, 0xe5,
        0x00, 0xfe, 0xdf, 0x00, 0xff, 0xdc, 0x00, 0xfe, 0xd6, 0x00, 0xff, 0xd3,
        0x00, 0xfe, 0xcd, 0x00, 0xff, 0xca, 0x00, 0xfe, 0xc4, 0x00, 0xff, 0xc1,
        0x00, 0xfe, 0xbb, 0x00, 0xff, 0xb8, 0x00, 0xfe, 0xb2, 0x00, 0xff, 0xaf,
        0x00, 0xfe, 0xa9, 0x00, 0xff, 0xa6, 0x00, 0xfe, 0xa0, 0x00, 0xff, 0x9d,
        0x00, 0xfe, 0x97, 0x00, 0xff, 0x94, 0x00, 0xfe, 0x8e, 0x00, 0xff, 0x8b,
        0x00, 0xfe, 0x85, 0x00, 0xff, 0x82, 0x00, 0xfe, 0x7d, 0x00, 0xff, 0x79,
        0x00, 0xfe, 0x74, 0x00, 0xff, 0x70, 0x00, 0xfe, 0x6b, 0x00, 0xff, 0x67,
        0x00, 0xfe, 0x62, 0x00, 0xff, 0x5e, 0x00, 0xfe, 0x59, 0x00, 0xff, 0x55,
        0x00, 0xfe, 0x50, 0x00, 0xff, 0x4c, 0x00, 0xfe, 0x47, 0x00, 0xff, 0x43,
        0x00, 0xfe, 0x3e, 0x00, 0xff, 0x3a, 0x00, 0xfe, 0x35, 0x00, 0xff, 0x31,
        0x00, 0xfe, 0x2c, 0x00, 0xff, 0x28, 0x00, 0xfe, 0x23, 0x00, 0xff, 0x1f,
        0x00, 0xfe, 0x1a, 0x00, 0xff, 0x16, 0x00, 0xfe, 0x11, 0x00, 0xff, 0x0d,
        0x00, 0xfe, 0x08, 0x00, 0xff, 0x04, 0x01, 0xfe, 0x00, 0x05, 0xff, 0x00,
        0x0a, 0xfe, 0x00, 0x0e, 0xff, 0x00, 0x13, 0xfe, 0x00, 0x17, 0xff, 0x00,
        0x1c, 0xfe, 0x00, 0x20, 0xff, 0x00, 0x25, 0xfe, 0x00, 0x29, 0xff, 0x00,
        0x2e, 0xfe, 0x00, 0x32, 0xff, 0x00, 0x37, 0xfe, 0x00, 0x3b, 0xff, 0x00,
        0x40, 0xfe, 0x00, 0x44, 0xff, 0x00, 0x49, 0xfe, 0x00, 0x4d, 0xff, 0x00,
        0x52, 0xfe, 0x00, 0x56, 0xff, 0x00, 0x5b, 0xfe, 0x00, 0x5f, 0xff, 0x00,
        0x64, 0xfe, 0x00, 0x68, 0xff, 0x00, 0x6d, 0xfe, 0x00, 0x71, 0xff, 0x00,
        0x76, 0xfe, 0x00, 0x7b, 0xff, 0x00, 0x7e, 0xfe, 0x00, 0x84, 0xff, 0x00,
        0x87, 0xfe, 0x00, 0x8d, 0xff, 0x00, 0x90, 0xfe, 0x00, 0x96, 0xff, 0x00,
        0x99, 0xfe, 0x00, 0x9f, 0xff, 0x00, 0xa2, 0xfe, 0x00, 0xa8, 0xff, 0x00,
        0xab, 0xfe, 0x00, 0xb1, 0xff, 0x00, 0xb4, 0xfe, 0x00, 0xba, 0xff, 0x00,
        0xbd, 0xfe, 0x00, 0xc3, 0xff, 0x00, 0xc6, 0xfe, 0x00, 0xcc, 0xff, 0x00,
        0xcf, 0xfe, 0x00, 0xd5, 0xff, 0x00, 0xd8, 0xfe, 0x00, 0xde, 0xff, 0x00,
        0xe1, 0xfe, 0x00, 0xe7, 0xff, 0x00, 0xea, 0xfe, 0x00, 0xf0, 0xff, 0x00,
        0xf3, 0xfe, 0x00, 0xf9, 0xff, 0x00, 0xfc, 0xfe, 0x00, 0xff, 0xfc, 0x00,
        0xfe, 0xf7, 0x00, 0xff, 0xf3, 0x00, 0xfe, 0xee, 0x00, 0xff, 0xea, 0x00,
        0xfe, 0xe5, 0x00, 0xff, 0xe1, 0x00, 0xfe, 0xdc, 0x00, 0xff, 0xd8, 0x00,
        0xfe, 0xd3, 0x00, 0xff, 0xcf, 0x00, 0xfe, 0xca, 0x00, 0xff, 0xc6, 0x00,
        0xfe, 0xc1, 0x00, 0xff, 0xbd, 0x00, 0xfe, 0xb8, 0x00, 0xff, 0xb4, 0x00,
        0xfe, 0xaf, 0x00, 0xff, 0xab, 0x00, 0xfe, 0xa6, 0x00, 0xff, 0xa2, 0x00,
        0xfe, 0x9d, 0x00, 0xff, 0x99, 0x00, 0xfe, 0x94, 0x00, 0xff, 0x90, 0x00,
        0xfe, 0x8b, 0x00, 0xff, 0x87, 0x00, 0xfe, 0x83, 0x00, 0xff, 0x7e, 0x00,
        0xfe, 0x7a, 0x00, 0xff, 0x75, 0x00, 0xfe, 0x71, 0x00, 0xff, 0x6c, 0x00,
        0xfe, 0x68, 0x00, 0xff, 0x63, 0x00, 0xfe, 0x5f, 0x00, 0xff, 0x5a, 0x00,
        0xfe, 0x56, 0x00, 0xff, 0x51, 0x00, 0xfe, 0x4d, 0x00, 0xff, 0x48, 0x00,
        0xfe, 0x44, 0x00, 0xff, 0x3f, 0x00, 0xfe, 0x3b, 0x00, 0xff, 0x36, 0x00,
        0xfe, 0x32, 0x00, 0xff, 0x2d, 0x00, 0xfe, 0x29, 0x00, 0xff, 0x24, 0x00,
        0xfe, 0x20, 0x00, 0xff, 0x1b, 0x00, 0xfe, 0x17, 0x00, 0xff, 0x12, 0x00,
        0xfe, 0x0e, 0x00, 0xff, 0x09, 0x00, 0xff, 0x05, 0x00, 0xff, 0xff, 0xff };

        byte[] retVal = new byte[768];
        for (int kk=0; kk<768; kk++)
            retVal[kk] = (byte)rainbowValues[kk];
     	return retVal;
    }

}


