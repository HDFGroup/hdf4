package ncsa.hdf.message;

import ncsa.hdf.hdflib.HDFException;
import ncsa.hdf.hdflib.HDFLibrary;
import ncsa.hdf.hdflib.HDFConstants;

/**
 *  HDFSDS holds HDF Vdata information
 
 *  @version   1.1.3 September 22 1997
 *  @author    Peter X. Cao
 */
public class HDFVdata extends HDFObject
{
    /**
     *  the range of records
     *  <pre>
     *   records[0] = the starting index of records
     *   records[1] = the ending index of records
     *  </pre>
     */
    private int[] records;

    /** the fields array */
    private int[] fields;

    /** the arry of field names */
    private String[] fieldNames;

    /** the array of field orders */
    private int[] fieldOrders;

    /** the array of field types */
    private int[] fieldTypes;

    /** the 2-D array of vdata */
    private String[][] vdata;

    /** the vdata name */
    private String vdataName;

    /**
     *  Constructs an HDFVdata with default parameters
     */
    public HDFVdata ()
    {
        super();
        records = null;
        fields = null;
        fieldNames = null;
        fieldOrders = null;
        fieldTypes = null;
        vdata = null;
        vdataName = null;
    }

    /**
     *  Construct an HDFVdata with specified HDFObjectNode and filename
     *
     *  @param node     the HDFObjectNode to be transfered to the server
     *  @param filename the requested HDF file
     */
    public HDFVdata (HDFObjectNode node, String filename)
    {
        super(node, filename);
        records = null;
        fields = null;
        fieldNames = null;
        fieldOrders = null;
        fieldTypes = null;
        vdata = null;
        vdataName = null;
    }

    /**
     *  create a message for sending to the client
     *
     *  @return  the HDFMessage created by this object
     */
    public HDFMessage toClient()
    {
        HDFMessage message = new HDFMessage(getClass().getName());

        message.put("information", information);
        if ( records != null ) message.put("records", records);
        if ( fields != null ) message.put("fields", fields);
        if ( fieldNames != null ) message.put("fieldNames", fieldNames);
        if ( fieldOrders != null ) message.put("fieldOrders", fieldOrders);
        if ( fieldTypes != null ) message.put("fieldTypes", fieldTypes);
        if ( vdata != null ) message.put("vdata", vdata);
        if ( vdataName != null ) message.put("vdataName", vdataName);
         return message;
    }

    /**
     *  serve the client request on the server
     */
    public void service()
    {
        try { information = getVdata (hdfFilename, nodeObject); }
        catch (Exception e) { information = "ERROR: exception in HDFVdata.getVdata()"; }
    }

    /**
     *  process the message received from the server
     *
     *  @param message the message from the server
     */
    public void fromServer(HDFMessage message)
    {
        information = (String) message.get("information");
        records = (int[]) message.get("records");
        fields = (int[]) message.get("fields");
        fieldNames = (String[]) message.get("fieldNames");
        fieldOrders = (int[]) message.get("fieldOrders");
        fieldTypes = (int[]) message.get("fieldTypes");
        vdata = (String[][]) message.get("vdata");
        vdataName = (String) message.get("vdataName");
    }

    /**
     *  get HDF Vdata image information for the selected HDF file
     *
     *  @param hdf        the HDFLibrary
     *  @param filename   the string of the hdf file name
     *  @param node       the HDFObjectNode
     *  @return           the string containing the HDF Vdata image information
     */
    public static String readInfo (HDFLibrary hdf, String filename,
        HDFObjectNode node) throws HDFException
    {
        int fid = -1;
        String info = new String();

        if ( (fid = hdf.Hopen(filename)) <= 0 )
        {
            return ("ERROR: Open " + filename + " failed.");
        }
        hdf.Vstart(fid);

        int vdata_ref = -1;         
        int [] n = new int[1];
        int vdNumber = hdf.VSlone(fid,n,0);
        int refArray[] = new int[vdNumber];
        int refNum;
   
        if  ( (refNum = hdf.VSlone(fid, refArray, vdNumber)) > 0 )
        {
            // each of vdata (lonely)
            for (int i=0; i<refNum; i++)
            {
                vdata_ref = refArray[i];
                int vstag = HDFConstants.DFTAG_VS;
                int vdata_id = hdf.VSattach(fid, vdata_ref, "r"); 
                String className = new String("");
                String s[] = new String[1];
                s[0] = className;
                hdf.VSgetclass(vdata_id,s);
                className = s[0];
                String name = new String("");
                s[0] = name;
                hdf.VSgetname (vdata_id,s);
                name = s[0];
    
                if ( className.indexOf("_HDF_CHK_TBL_0") == -1 )
                {
                    info = info.concat("\nVStag         : " + vstag);
                    info = info.concat("\nref           : " + vdata_ref);
                    info = info.concat("\nVdata Class   : " + className);
                    info = info.concat("\nVdata Name    : " + name);
                    info = info.concat("\nVdata records : " + hdf.VSelts(vdata_id));
                    info = info.concat("\nInterlace     : " + hdf.VSgetinterlace(vdata_id));

                    // test VSgetfields
                    String flist = new String("");
                    s[0] = flist;
                    int fieldNumber = hdf.VSgetfields(vdata_id, s);
                    flist = s[0];

                    info = info.concat("\nField number  : " + fieldNumber);
                    info = info.concat("\nField list    : " +  flist);       
                    info = info.concat("\nRecord size   : " + hdf.VSsizeof(vdata_id,flist) + " bytes");

                    // Check to see if this vdata is alone? ....
                    int  nFields = hdf.VFnfields(vdata_id);
                    if ( nFields > 0 ) info = info + "\n\nField name     Field order    Field type";
                    for (int kk=0; kk < nFields ; kk++)
                    {
                        String fName = hdf.VFfieldname(vdata_id, kk);
                        int spaces = 15 - fName.length();
                        for (int jj = 0; jj < spaces; jj++) fName = fName + " ";

                        String fOrder= String.valueOf( hdf.VFfieldorder(vdata_id, kk) );
                        spaces = 15 - fOrder.length();
                        for (int jj = 0; jj < spaces; jj++) fOrder = fOrder + " ";

                        int fType = hdf.VFfieldtype(vdata_id, kk);
                        String fTypeName = hdf.HDgetNTdesc(fType);

                        info = info +"\n" + fName + fOrder + fTypeName;
                    } 
                    hdf.VSdetach(vdata_id);
                }
            }
        }
    
        hdf.Vend(fid);
        hdf.Hclose(fid);
        return (info);
    }

    /**
     *  get the selected HDF Vdata (vertex data) image for the selected HDF file
     *
     *  @param filename   the string of the hdf file name
     *  @param node       the HDFObjectNode
     *  @return           the error message
     */
    private String getVdata (String filename, HDFObjectNode node)
        throws HDFException
    {
        int        request     = node.type;
        int        ref         = node.ref;
        int        attrIndex   = node.index;
        int fid = -1;
        String info = "";
        String vdata[][] = null;

        if ((fid = hdf.Hopen(filename)) <= 0)
        {
            return ("ERROR: Open " + filename + " failed.");
        }

        hdf.Vstart(fid);
      
        // Attaches to an existing Vdata
        int vdata_id = hdf.VSattach(fid, ref, "r");
        if (vdata_id == HDFConstants.FAIL)
        {
            hdf.Vend(fid);
            hdf.Hclose(fid);
            return ("ERROR: native call to HDFLibrary.VSattach() failed");
        }
    
        // set selected vdata field & record number
        int[] selectedRecords = new int[2];
        selectedRecords[0] = 1;
        selectedRecords[1] = hdf.VSelts(vdata_id);
        if ( selectedRecords[1] == HDFConstants.FAIL )
        {
            hdf.VSdetach(vdata_id);
            hdf.Vend(fid);
            hdf.Hclose(fid);
            return ("ERROR: native call to HDFLibrary.VSelts() failed");
        }
            
        // field
        int nfields = hdf.VFnfields(vdata_id);
        if (nfields == HDFConstants.FAIL)
        {
            hdf.VSdetach(vdata_id);
            hdf.Vend(fid);
            hdf.Hclose(fid);
            return ("ERROR: native call to HDFLibrary.VFnfields() failed");
        }

        int[] selectedFields = new int[nfields];
        for (int i=0; i < nfields; i++) 
            selectedFields[i] = i;

        info = getSpreadsheetData(vdata_id, selectedRecords, selectedFields);

        hdf.VSdetach(vdata_id);
        hdf.Vend(fid);
        hdf.Hclose(fid);
        return info;
    }

   /**
     *  get the spreadsheet data for the selected Vdata
     *
     *  @param  vdata_id   The id number of the Vdata
     *  @param  records    The selected Vdata records
     *  @param  fields     The selected Vdata fields
     *  @return            The error message
     */
    private String getSpreadsheetData(int vdata_id, int[] records, int[] fields) throws HDFException
    {
        int numberOfFields = fields.length;
        int numberOfRecords = records[1]-records[0]+1;
        String[][] vdata = new String[numberOfRecords][numberOfFields];
        String[] vdataFieldName = new String[numberOfFields];
        int[] vdataFieldOrder = new int[numberOfFields];
        int[] vdataFieldType = new int[numberOfFields];
        String vdataName= "";


        // vdata name
        String[] s = new String[1];
        s[0] = vdataName;
        hdf.VSgetname(vdata_id,s);
        vdataName = s[0];

        // get name, type and order for each field
        for (int i=0; i < numberOfFields; i++)
        {
            // get field name
            vdataFieldName[i] = new String(hdf.VFfieldname(vdata_id,i));

            // data type of a specified field
            vdataFieldType[i] = hdf.VFfieldtype(vdata_id,i);
            if ( vdataFieldType[i] == HDFConstants.FAIL )
                return ("ERROR: native call to HDFLibrary.VFfieldtype() failed");

            // order of a specified field
            vdataFieldOrder[i] = hdf.VFfieldorder(vdata_id,i);
            if ( vdataFieldOrder[i] == HDFConstants.FAIL)
                return ("ERROR: native call to HDFLibrary.VFfieldorder() failed");
        }

        int rows = 0;
        int count = records[1] -records[0] + 1;

        int  dataType, order, datasize, readNum, dispNumber;
        String tmpStr="";
        boolean status;

	// for each field of a vdata
	for (int i=0; i< numberOfFields; i++) {

	    // specify read vdata size
	    dataType = vdataFieldType[i];
	    if ((dataType & HDFConstants.DFNT_LITEND) != 0)
	       dataType -= HDFConstants.DFNT_LITEND;
	    order = vdataFieldOrder[i];
	    datasize = 1;
	    readNum = 0;
	    tmpStr="";

	    if (order>0) {

                dispNumber = order;

		// hdf.VSseek(vdata_id, kk-1);
		hdf.VSseek(vdata_id, 0);

		status = hdf.VSsetfields(vdata_id, vdataFieldName[i]);
		    
		datasize = order * hdf.DFKNTsize(dataType) * count;

		// assume repeat number is less than 3 -- only display
		//   first 3 fields

	        if ((order>3) && (dataType != HDFConstants.DFNT_CHAR8))
	           dispNumber = 3;
	
		// organize data
		switch(dataType) {
		case HDFConstants.DFNT_CHAR:  
		case HDFConstants.DFNT_UCHAR8:

			// this is assumed to be a string
			byte[] bdat = new byte[datasize];
			readNum = hdf.VSread(vdata_id, bdat, count, HDFConstants.FULL_INTERLACE);
			rows=0;
			for (int kk=records[0];
     				 kk<=records[1]; kk++, rows++)  
			    vdata[rows][i] = new String(bdat,order*rows,order);
			break;
		    
		case HDFConstants.DFNT_UINT8: 
		case HDFConstants.DFNT_INT8: {
			// signed integer (byte) array
			byte[] sbdat = new byte[datasize];
			readNum = hdf.VSread(vdata_id, sbdat, count, HDFConstants.FULL_INTERLACE);

			rows=0;
			for (int kk=records[0];
     				 kk<=records[1]; kk++, rows++)  { 

			tmpStr="";
			for (int j=0; j<dispNumber; j++) {
			    int tmpVal = (int)((byte)sbdat[j+rows*order]);
			    if ((dataType == HDFConstants.DFNT_UINT8) && (tmpVal <0)) {
				tmpVal += 256;
			    }
			    tmpStr += Integer.toString(tmpVal) +"  ";
			}

			if (vdataFieldOrder[i]>3)
			   tmpStr += "...";

			vdata[rows][i] = new String(tmpStr);
			}

			break;
		    }
			    
		case HDFConstants.DFNT_INT16:
		case HDFConstants.DFNT_UINT16:
		    {
			// short array
			short[] sdat = new short[order*count];
			readNum = hdf.VSread(vdata_id, sdat, count, HDFConstants.FULL_INTERLACE);

			rows=0;
			for (int kk=records[0];
     				 kk<=records[1]; kk++, rows++)  {

			tmpStr="";
			int pos = 0;
			for (int j=0; j<dispNumber; j++) {
			    int tmpVal = (int)sdat[j+order*rows];

			    if ((dataType == HDFConstants.DFNT_UINT16) && (tmpVal <0)) {
				tmpVal += 65536;
			    }

			    tmpStr += Integer.toString(tmpVal) +"  ";
			}			

			if (vdataFieldOrder[i]>3)
			   tmpStr += "...";

			vdata[rows][i] = new String(tmpStr);
			}

			break;
		    }		    
		case HDFConstants.DFNT_INT32:
		case HDFConstants.DFNT_UINT32:
		    {
			// integer	
			tmpStr="";
			int[] idat = new int[order*count];
			readNum = hdf.VSread(vdata_id, idat, count, HDFConstants.FULL_INTERLACE);

			rows=0;
			for (int kk=records[0];
     				 kk<=records[1]; kk++, rows++)  { 
			tmpStr="";
			for (int j=0; j<dispNumber; j++) {
			    int tmpVal = idat[j+order*rows];
			    if ((dataType == HDFConstants.DFNT_UINT32) && (tmpVal <0))  			
				tmpVal += 4294967296L;
		    
			    tmpStr += Integer.toString(tmpVal) +"  ";
			}
		        
			if (vdataFieldOrder[i]>3)
			   tmpStr += "...";

			vdata[rows][i] = new String(tmpStr);
			}

			break;
		    }
		
		        
		case HDFConstants.DFNT_FLOAT:
		//case HDFConstants.DFNT_FLOAT32:
			// Float	
		    {
			 
			float[] fdat = new float[order*count];
			readNum = hdf.VSread(vdata_id, fdat, count, HDFConstants.FULL_INTERLACE);
			rows=0;
			for (int kk=records[0];
     				 kk<=records[1]; kk++, rows++)  { 
			tmpStr="";
			for (int j=0; j<dispNumber; j++) {
			    float tmpVal = fdat[j+order*rows];
			    tmpStr += Float.toString(tmpVal) +"  ";
			}
			        
			if (vdataFieldOrder[i]>3)
			   tmpStr += "...";
	    		
		        vdata[rows][i] = new String(tmpStr);
			}

			break;
		    }
          
		case HDFConstants.DFNT_DOUBLE:
		//case HDFConstants.DFNT_FLOAT64:
		    {
			// Double	
			// tmpStr="";
			double[] ddat = new double[order*count];
			readNum = hdf.VSread(vdata_id, ddat, count, HDFConstants.FULL_INTERLACE);

			rows = 0;
			for (int kk=records[0];
     				 kk<=records[1]; kk++, rows++)  { 
			tmpStr="";
			for (int j=0; j<dispNumber; j++) {
			    
			    double tmpVal =ddat[j+order*rows];
			    tmpStr += Double.toString(tmpVal) +"  ";
			}
		        
			if (vdataFieldOrder[i]>3)
			   tmpStr += "...";

		        vdata[rows][i] = new String(tmpStr);
			}

			break;
		    }
		
		default:
		    vdata[rows][i] = new String("");
		}		
	    }	    
	}

        this.records = records;
        this.fields = fields;
        this.fieldNames = vdataFieldName;
        this.fieldOrders = vdataFieldOrder;
        this.fieldTypes = vdataFieldType;
        this.vdata = vdata;
        this.vdataName = vdataName;
        return "";
    }

    /** return the records range */
    public int[] getRecords() { return records; }

    /** return the Vdata fields */
    public int[] getFields() { return fields; }

    /** return the array of fields names */
    public String[] getFieldNames() { return fieldNames; }

    /** return the array of the field orders */
    public int[] getFieldOrders() { return fieldOrders; }

    /** return the array of the fields types */
    public int[] getFieldTypes() { return fieldTypes; }

    /** return the Vdata */
    public String[][] getVdata() { return vdata; }

    /** return the name of the Vdata */
    public String getVdataName() { return vdataName; }

}
