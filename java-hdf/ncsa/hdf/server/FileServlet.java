
package ncsa.hdf.server;

import java.io.*;
import java.util.*;
import java.text.DateFormat;
import javax.servlet.*;
import javax.servlet.http.*;

import ncsa.hdf.util.Tools;

/**
 *  FileServlet implements the "GET" and "HEAD" methods for files and
 *  directories. It also handles index.html.

 *  @see       javax.servlet.http.HttpServlet
 *  @version   1.1.3 July 30 1997
 *  @author    Peter X. Cao  (xcao@ncsa.uiuc.edu)
 */
public class FileServlet extends HttpServlet
{

    /** the maximum string length of the displayed file names */
    public static final int DISPLAY_LENGTH = 20;

    /** Creates a new FileServlet with default parameters. */
    public FileServlet() {}

    /**
     * Returns a string containing information about the author, 
     * version, and copyright of the servlet.
     */
    public String getServletInfo()
    {
        return "A file servlet similar to a standard httpd";
    }


    /**
     * Services a single request from the client.
     * @param req the servlet request
     * @param res the servlet response
     * @exception ServletException when an exception has occurred
     */
    public void service( HttpServletRequest req, HttpServletResponse res ) 
        throws ServletException, IOException
    {
        //System.out.println("FileServlet::service()");

        boolean headOnly;
        if ( req.getMethod().equalsIgnoreCase( "get" ) )
            headOnly = false;
        else if ( ! req.getMethod().equalsIgnoreCase( "head" ) )
            headOnly = true;
        else
        {
            res.sendError( HttpServletResponse.SC_NOT_IMPLEMENTED );
            return;
        }

        String path = req.getRequestURI() ;
        if ( path == null || path.charAt( 0 ) != '/' )
        {
            res.sendError( HttpServletResponse.SC_BAD_REQUEST );
            return;
        }
        if ( path.indexOf( "/../" ) != -1 || path.endsWith( "/.." ) )
        {
            res.sendError( HttpServletResponse.SC_FORBIDDEN );
            return;
        }

        // Make a version without the leading /.
        String pathname = path.substring( 1 );
        if ( pathname.length() == 0 )
            pathname = "./";

        dispatchPathname( req, res, headOnly, path, pathname );
    }

    /**
     *  dispatch the path name for service
    
     *  @param req       the HttpServletRequest
     *  @param res       the HttpServletResponse
     *  @param headOnly  ask only the HEAD
     *  @param path      the string of the file path
     *  @param pathname  the string of the path name
     *  @exception       an IOException.
     */
    protected void dispatchPathname( HttpServletRequest req, HttpServletResponse res, 
        boolean headOnly, String path, String pathname ) throws IOException
    {
        //System.out.println("FileServlet::dispatchPathname()");

        String filename = pathname.replace( '/', File.separatorChar );
        if ( filename.endsWith(File.separator) )
            filename = filename.substring( 0, filename.length() - 1 );
        filename = getServletContext().getRealPath( filename );
        File file = new File( filename );
        if ( file.exists() )
        {
            if ( ! file.isDirectory() )
                serveFile( req, res, headOnly, path, filename, file );
            else
            {
                if ( !pathname.endsWith("/") )
                    redirectDirectory( req, res, path, file );
                else
                {
                    String indexFilename =
                    filename + File.separatorChar + "index.html";
                    File indexFile = new File( indexFilename );
                    if ( indexFile.exists() )
                        serveFile( req, res, headOnly, path, indexFilename, indexFile );
                    else
                        serveDirectory( req, res, headOnly, path, filename, file );
                }
            }
        }
        else
        {
            if ( pathname.endsWith( "/index.html" ) )
                dispatchPathname( req, res, headOnly, path, pathname.substring( 0, pathname.length() - 10 ) );
            else if ( pathname.equals( "index.html" ) )
                dispatchPathname( req, res, headOnly, path, "./" );
            else
                res.sendError( HttpServletResponse.SC_NOT_FOUND );
        }
    }

    /**
     *  serve a file request (GET or HEAD)
    
     *  @param req       the HttpServletRequest
     *  @param res       the HttpServletResponse
     *  @param headOnly  ask only the HEAD
     *  @param path      the string of the file path
     *  @param filename  the string of the file name
     *  @param file      the file requested
     *  @exception       an IOException.
     */
    protected void serveFile( HttpServletRequest req, HttpServletResponse res, boolean headOnly,
        String path, String filename, File file ) throws IOException
    {
        //System.out.println("FileServlet::serveFile()");

        log( "getting " + path );
        if ( ! file.canRead() )
        {
            res.sendError( HttpServletResponse.SC_FORBIDDEN );
            return;
        }

        // Handle If-Modified-Since.
        res.setStatus( HttpServletResponse.SC_OK );
        Date ifModSince = null;
        Date lastMod = new Date(file.lastModified());
        String ifModSinceStr = req.getHeader( "If-Modified-Since" );
        if ( ifModSinceStr != null )
        {
            int semi = ifModSinceStr.indexOf( ';' );
            if ( semi != -1 ) ifModSinceStr = ifModSinceStr.substring( 0, semi );
            try
            {
                ifModSince = DateFormat.getInstance().parse( ifModSinceStr );
            }
            catch ( Exception e ) {} 
        }

        if ( ifModSince != null && ifModSince.equals(lastMod) )
        {
            res.setStatus( HttpServletResponse.SC_NOT_MODIFIED );
            headOnly = true;
        }
        res.setContentType( getServletContext().getMimeType( filename ) );
        res.setContentLength( (int) file.length() );
        res.setDateHeader( "Last-modified", file.lastModified() );
        OutputStream out = res.getOutputStream();
        if ( ! headOnly )
        {
            InputStream in = new FileInputStream( file );
            copyStream( in, out );
            in.close();
        }
        out.close();

    }
    
    /**
     *  serve a direcotry request (GET or HEAD)
    
     *  @param req       the HttpServletRequest
     *  @param res       the HttpServletResponse
     *  @param headOnly  ask only the HEAD
     *  @param path      the string of the file path
     *  @param filename  the string of the file name
     *  @param file      the file requested
     *  @exception       an IOException.
     */
    protected void serveDirectory( HttpServletRequest req, HttpServletResponse res, boolean headOnly, 
        String path, String filename, File file ) throws IOException
    {

        log( "indexing " + path );
        if ( ! file.canRead() )
        {
            res.sendError( HttpServletResponse.SC_FORBIDDEN );
            return;
        }
        res.setStatus( HttpServletResponse.SC_OK );
        res.setContentType( "text/html" );
        OutputStream out = res.getOutputStream();
        if ( ! headOnly )
        {
            PrintWriter p = new PrintWriter( new BufferedOutputStream( out ) );
            p.println( "<HTML>" );
            p.println( "<HEAD><TITLE>Directory listing of " + path + "</TITLE></HEAD>" );
            p.println( "<BODY>" );
            p.println( "<H2>Directory listing of " + path + "</H2>" );
            p.println( "<PRE>" );
            String nameTag = "Name";
            while (nameTag.length() < DISPLAY_LENGTH) nameTag = nameTag+" ";
            p.println(nameTag + "\t" + "Mode\t" + "Size\t" + "Modified");
            p.println( "<HR>" );
            String[] names = file.list();

            // sort file names by folder and alphabetical order
            for ( int i = 0; i < names.length; ++i )
            {
                String aFilename = filename + File.separatorChar + names[i];
                File aFile = new File( aFilename );
                if ( aFile.isDirectory() ) names[i] = "d"+names[i];
                else names[i] = "f"+names[i];
            }
            Tools.sort( names );

            for ( int i = 0; i < names.length; ++i )
            {
                names[i] = names[i].substring(1);
                String aFilename = filename + File.separatorChar + names[i];
                File aFile = new File( aFilename );
                String aFileType;
                if ( aFile.isDirectory() ) aFileType = "d";
                else if ( aFile.isFile() ) aFileType = "-";
                else aFileType = "?";
                String aFileRead = ( aFile.canRead() ? "r" : "-" );
                String aFileWrite = ( aFile.canWrite() ? "w" : "-" );
                String aFileExe = "-";
                String aFileSize = formatFileLength( aFile.length() );
                String aFileDate = DateFormat.getInstance().format(new Date( aFile.lastModified()));
                String aFileDirsuf = ( aFile.isDirectory() ? "/" : "" );
                String aFileSuf = ( aFile.isDirectory() ? "/" : "" );
                String whiteSpaces = "";
                String displaydName= names[i];
                if (displaydName.length() < DISPLAY_LENGTH)
                {
                    for (int j=0; j<(DISPLAY_LENGTH-displaydName.length()-aFileSuf.length()); j++)
                        whiteSpaces = whiteSpaces + " ";
                }
                else if (displaydName.length() > DISPLAY_LENGTH)
                    displaydName = displaydName.substring(0, DISPLAY_LENGTH-aFileSuf.length())+"~"; 
                p.println(
                    "<A HREF=\"" + names[i] + aFileDirsuf + "\">" +displaydName + aFileSuf + "</A>" + whiteSpaces +"\t" +
                    aFileType + aFileRead + aFileWrite + aFileExe +"\t" +
                    aFileSize + "\t" +
                    aFileDate);
            }
            p.println( "</PRE>" );
            p.println( "<HR>" );
            p.println( "</BODY></HTML>" );
            p.flush();
        }
        out.close();
    }


    /**
     *  redirectory
    
     *  @param req       the HttpServletRequest
     *  @param res       the HttpServletResponse
     *  @param path      the string of the file path
     *  @param file      the file requested
     */
    protected void redirectDirectory( HttpServletRequest req, HttpServletResponse res,
        String path, File file ) throws IOException
    {
        log( "redirecting " + path );
        res.sendRedirect( path +"/" );
    }

    /**
     * copy byte streams from an InputStream to an OutputStream.
     * @parm in  the InputStream
     * @parm out the OutputStream
     */
    protected void copyStream( InputStream in, OutputStream out ) throws IOException
    {
        byte[] buf = new byte[4096];
        int len;
        while ( ( len = in.read( buf ) ) != -1 )
            out.write( buf, 0, len );
    }

    /**
     * format the file size in bytes, megabytes, gigabytes or tarabytes
     * @param fileLength   the orriginal file size in bytes
     * @return  the formated file size in bytes, megabytes, gigabytes or tarabytes
     */
    public String formatFileLength(long fileLength)
    {
        String fileUnit = "B";
        int shiftLoop = 0;
        for (shiftLoop=0; shiftLoop<4 && fileLength>1024; shiftLoop++)
            fileLength = fileLength >> 10;
        switch (shiftLoop)
        {
            case 1: 
                fileUnit = "KB"; break;
            case 2:
                fileUnit = "MB"; break;
            case 3:
                fileUnit = "GB"; break;
            case 4:
                fileUnit = "TB"; break;
            default:
            case 0:
                fileUnit = "B"; break;
        }
        return Long.toString ( fileLength )+fileUnit;
    }

}
