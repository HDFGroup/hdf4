|  Created  |  Key  |  Summary  |
| --------- | ----- | --------- |
| 03/10/2016  | <a href="#HDFFR-1523">HDFFR-1523</a>  |  Update documentation for the library code and file format, and user documentation  |
| 10/23/2015  | <a href="#HDFFR-1518">HDFFR-1518</a>  |  RM entries need improvement and/or correction  |
| 09/02/2015  | <a href="#HDFFR-1517">HDFFR-1517</a>  |  Make sure hdp commands list options correctly  |
| 09/02/2015  | <a href="#HDFFR-1517">HDFFR-1517</a>  |  Make sure hdp commands list options correctly  |
| 12/22/2014  | <a href="#HDFFR-1478">HDFFR-1478</a>  |  VSinquire and vsfinq behavior  |
| 10/24/2013  | <a href="#HDFFR-1403">HDFFR-1403</a>  |  UG issues in the List of Examples  |
| 08/18/2013  | <a href="#HDFFR-1392">HDFFR-1392</a>  |  Document the interpretation of ncdump output  |
| 07/09/2013  | <a href="#HDFFR-1387">HDFFR-1387</a>  |  You cannot create an SDS with both chunking and unlimited dimension  |
| 07/05/2013  | <a href="#HDFFR-1386">HDFFR-1386</a>  |  HDFFR-1385 Identify all the places to document this issue and document them  |
| 11/30/2012  | <a href="#HDFFR-1344">HDFFR-1344</a>  |  hdfimport documentation does not correctly reflect how to create a 2D dataset with hdfimport  |
| 08/20/2012  | <a href="#HDFFR-1330">HDFFR-1344</a>  |  GrayJPEG5 (tag 16) LL Objects seen but not expected  |
| 05/31/2012  | <a href="#HDFFR-1324">HDFFR-1324</a>  |  Add details to hdp in UG  |
| 01/08/2012  | <a href="#HDFFR-1298">HDFFR-1298</a>  |  Improvements to the HDF4 docs  |
| 01/05/2012  | <a href="#HDFFR-1296">HDFFR-1296</a>  |  Hstartread entry missing from reference manual  |
| 09/27/2011  | <a href="#HDFFR-1289">HDFFR-1289</a>  |  HDF4 has default fill values for SDS elements but they are never documented  |
| 09/06/2011  | <a href="#HDFFR-1287">HDFFR-1287</a>  |  Behavior of software doesn't match documentation for empty SDS datasets, and is there a default fill value?  |
| 10/13/2011  | <a href="#HDFFR-1291">HDFFR-1291</a>  |  SDS with rank=0  |
| 05/13/2011  | <a href="#HDFFR-1265">HDFFR-1265</a>  |  Clear explanation regarding requirement of external file location  |
| 04/25/2011  | <a href="#HDFFR-1259">HDFFR-1259</a>  |  UG Section 8.6.2 has incorrect information that causes segfault  |
| 03/13/2011  | <a href="#HDFFR-996">HDFFR-996</a>  |  Documentation text and example for interlace modes in DF24 is very confusing (and wrong?)  |
| 03/09/2011  | <a href="#HDFFR-993">HDFFR-993</a>  |  palette not initialized in UG 6.4.2 Ex 2  |
| 03/09/2011  | <a href="#HDFFR-992">HDFFR-992</a>  |  confusing (or incorrect) description of dim order in DFR8putimage RM page  |
| 01/11/2011  | <a href="#HDFFR-734">HDFFR-734</a>  |  Error in Spec Section 10.4 External Elements  |
| 01/06/2011  | <a href="#HDFFR-733">HDFFR-733</a>  |  UG sections 8.11.x should tell current behavior, not designed behavior  |
| 01/06/2011  | <a href="#HDFFR-731">HDFFR-731</a>  |  compressing 8-bit raster example wrong (or confusing)  |
| 01/06/2011  | <a href="#HDFFR-732">HDFFR-732</a>  |  RM entry for GRwritelut should be improved  |
| 11/29/2010  | <a href="#HDFFR-728">HDFFR-728</a>  |  docs mention default fill values but never say what they are  |
| 07/27/2010  | <a href="#HDFFR-471">HDFFR-471</a>  |  ug: SDsetblocksize page had errors  |
| 06/11/2010  | <a href="#HDFFR-228">HDFFR-228</a>  |  VSwrite in RG confusing  |
| 05/06/2010  | <a href="#HDFFR-224">HDFFR-224</a>  |  Docs have VFfieldisize and VFfieldesize backwards  |
| 04/30/2010  | <a href="#HDFFR-223">HDFFR-223</a>  |  UG: Confusing information about supported data types  |
| 03/17/2010  | <a href="#HDFFR-208">HDFFR-208</a>  |  Feedback on Chapter 7 of Spec (6/25/08 draft version)  |
| 03/17/2010  | <a href="#HDFFR-209">HDFFR-209</a>  |  Feedback on Chapter 8 of Spec (6/25/08 draft)  |
| 07/16/2008  | <a href="#HDFFR-157">HDFFR-157</a>  |  GR: Add examples to doc on image store/retrieving  |
| 03/19/2007  | <a href="#HDFFR-612">HDFFR-612</a>  |  Add hrepack/hdiff info to documentation  |
| 09/22/2006  | <a href="#HDFFR-111">HDFFR-111</a>  |  UG: writing dimension scale w/DFSD api not clear  |

## Detail Section

<h3 id="HDFFR-1289"></h3>[HDFFR-1289](https://jira.hdfgroup.org/browse/HDFFR-1289)
*HDF4 has default fill values for SDS elements but they are never documented*

#### Description

HDF4 defines default fill values for SDS elements based on the type of the element. These are defined in netcdf.h.in and are returned by DFSDgetfillvalue() when there is no user-defined fill value. However... the existence of library-defined fill values is never documented anywhere (than I can find) in the HDF4 documents.

The RM entry for SDgetfillvalue() says Returns SUCCEED (or 0) if a fill value is retrieved and FAIL (or -1) otherwise, including when the fill value is not set. And, "SDgetfillvalue reads the fill value which has been set for the data set specified by the parameter sds_id"

This is not accurate. If no value has been set, then the default value is returned.

The RM entry for DFSDgetfillvalue() says The fill value is set by DFSDsetfillvalue and returned in the variable fill_value.

This is not accurate either, I suspect. The fill value can be set by the library if that other call is not made by the user.

There should also be a discussion of fill values being set by the library in the UG - probably in the sections where the fill value routines are documented.

#### Comments

---

<h3 id="HDFFR-1287"></h3>[HDFFR-1287](https://jira.hdfgroup.org/browse/HDFFR-1287)
*Behavior of software doesn't match documentation for empty SDS datasets, and is there a default fill value?*

#### Attachments

EmptyArrays.c     EmptyArrays.hdf     EmptyArrays3.c     undefined

#### Description

Joe raised some good questions about empty datasets in HDF4 in the context of the mapping project. In trying to carefully sort out the issues, I created a program that writes several SDSes that I consider to be empty to see what would happen if I used HDF4's routines to read the first element of each. The results are not what I would expect.

I'm attaching the source for my test program that creates then reads the SDSes, as well as the file that was created. It's interesting to view the file with hdp dumpsds and with hdfview. The output regarding the SDSes not consistent (at least not to me)

The first SDS – created with rank 0 – gives me a value and not an error when I try to read it, and this is not what I expect. The UG (page 3-27 in my version) says "HDF allows SDS to have rank 0. However, there is no intention for data to be written to this type of SDS, ...".

Read on second SDS returns an error, as I would expect.

Read on third SDS returns "32769", the default fill value for a uint16 array that has not been written to and for which the user has not specified a fill value. I can't find any information about default fill values in the HDF4 documentation. Does it exist? If not, I think it should. Otherwise, I would expect a request to read a non-written value to return an error, not a default fill value.

Read on 4th SDS returns "99", the fill value that I set. This is what I would expect.

I also expect to try similar tests for Vdata & for Rasters and would expect behavior to be "consistent". But, I haven't tried that yet.

#### Comments

***Ruth Aydt*** (Sep 07, 2011)

A few more thoughts on this...

1) Concept of an "empty" dataset, as in no "space" for any data. If I try to do a read anywhere, I will get an error. I think Cases 1&2 in example fall into this.

2) Concept of an "unpopulated" dataset, as in no data is ever written to any cells, but if I do a read I get a valid response - either the user-defined fill value or the system-defined fill value. This is what happens in 3&4 in the test code.

I think we kind of lump these together. Empty == unpopulated, but what if it's really "empty" (more like no cells)?

***BMR*** (Oct 02, 2011)

For the issue of rank=0, SDreaddata will be changed to fail. It will also fail when the SDS has unlimited dimension and no data written.

***Ruth Aydt*** Oct 03, 2011

I expanded my test program some based on feedback from Binh-Minh, and wanted to include this expanded test with the JIRA report.

In this test I read the information about each SDS before I try to read the data. I still try to read the data, but can print out more details. I also added a new 1-D dataset with an unlimited dimension to those written. This appears as the Second Array.

The output generated when the program runs and tries to read the datasets created is shown below. Based on Binh-Minh's comments, I think the behavior will only change for the read of Empty Array with rank 0. It will return a status of -1 (instead of 0) and will not change the value that was passed in to the call (now it changes it from 50 to the default of 32769).

~~~
>>> Begin Read phase  
SDstart returned sd_id 393216  
SDselect returned sds_id 262144  
SDgetinfo returned status 0  
Name: Empty Array with rank 0  
Rank 0; Dimension Sizes\[0\]\[0\]; Datatype 23; Number Attributes: 0  
SDreaddata returned status = 0  
First Array: value is 32769  
SDendaccess returned status = 0

SDselect returned sds_id 262145  
SDgetinfo returned status 0  
Name: Empty 1D Array with unlimited dimension  
Rank 1; Dimension Sizes\[0\]\[0\]; Datatype 23; Number Attributes: 0  
SDreaddata returned status = -1  
Second Array: value is 50  
SDendaccess returned status = 0

SDselect returned sds_id 262146  
SDgetinfo returned status 0  
Name: Empty 2D Array with unlimited dimension  
Rank 2; Dimension Sizes\[0\]\[2\]; Datatype 23; Number Attributes: 0  
SDreaddata returned status = -1  
Third Array: value is 50  
SDendaccess returned status = 0

SDselect returned sds_id 262147  
SDgetinfo returned status 0  
Name: Empty 2D Array with no user-defined fill value  
Rank 2; Dimension Sizes\[3\]\[5\]; Datatype 23; Number Attributes: 0  
SDreaddata returned status = 0  
Fourth Array: value is 32769  
SDendaccess returned status = 0

SDselect returned sds_id 262147  
SDgetinfo returned status 0  
Name: Empty 2D Array with no user-defined fill value  
Rank 2; Dimension Sizes\[3\]\[5\]; Datatype 23; Number Attributes: 0  
SDreaddata returned status = 0  
Fifth Array: value is 32769  
SDendaccess returned status = 0

SDend status 0
~~~

***Ruth Aydt*** (Oct 03, 2011)
Sorry, when I did a "drag and drop" of the new test program "EmptyArrays2.c" it got assigned the name "undefined" in the list of attachments & I don't know how to delete or edit it. so, that's where you can find the new expanded test.
***Ruth Aydt*** (Oct 05, 2011)
Copying this comment that Binh-Minh made in email here:

OK, here is the latest about rank=0 issue.

What I said in the original finding was still true, but there is more.

If SDwritedata had never been called, there would be no data. This is like in your sample program. In this case, SDgetdatainfo will return offset=0 and length=0.

If SDwritedata had been called, there would be one value stored in the file, no matter how big the buffer is when it is passed into SDwritedata. In this case, SDgetdatainfo will return a valid offset and length pair.

This doesn't mean SDwritedata behaves incorrectly, it just means that is the way it works right now. According to Elena's memory, rank=0 wasn't allowed originally, but then later was allowed to accommodate netCDF; yet, it was not handled properly. I need to investigate where things became tangled, and see what we can do about it. So, I'm not going to fix SDreaddata to fail when rank=0 yet; but if it does, SDwritedata should too, I would think.

Binh-Minh
***Ruth Aydt*** (Oct 05, 2011)
Copying this comment that Elena made in email here:

In NetCDF variable of rank 0 is used for attaching attributes.
***Ruth Aydt*** (Oct 05, 2011)
The HDF UG, in section 3.4.1, says "Note that, in order for HDF4 and NetCDF models to work together, HDF allows SDS to have rank 0. However, there is no intention for data to be written to this type of SDS, but only to store attribute as part of the data descriptor."

I interpret this to mean you can associate an attribute with a 0-rank SDS, but never should write data to it.

I have expanded my test program to add more cases.

One adds an attribute to a 0-rank SDS that never has SDwritedata call.

One makes an SDwritedata call to a 0-rank SDS with a NULL pointer to the data value. This gets an error on the write call, but not on a read later.

One makes an SDwrite data call to a 0-rank SDS with a pointer to a data value. This does not get an error on the write call or on the read call. But, I think it should be an error.

When I run h4mapwriter on the file produced I get a segfault.

---

<h3 id="HDFFR-1324"></h3>[HDFFR-1324](https://jira.hdfgroup.org/browse/HDFFR-1324)
*Add details to hdp in UG*

#### Description

More details about arguments need to be added. For examples, how to specify the names in: hdp dumpsds -n "name1 name2" filename.

#### Comments

***Barbara Jones*** (May 31, 2012)

The user had a dataset with a comma in the name. HDF4 allows the creation of a dataset with a comma in the name, but "hdp dumpsds -n" fails when it encounters this, because it interprets the comma as a delimiter between dataset names.

hdp dumpsds -n 'data1, test' sdtest.hdf SDS with name 'data1': not found SDS with name ' test': not found

We could either add a way to escape the comma in hdp, or document the limitation.

USER INFORMATION:

Giorgio Dall'Olmo  <br>Plymouth Marine Laboratory  <br>Prospect Place  <br>The Hoe  <br>Plymouth  <br>PL1 3DH  <br>UK

phone: +44-(0)1752-633429  <br>fax: +44-(0)1752-633101
***Barbara Jones*** (May 31, 2012)
See helpdesk issue,
***BMR*** (Feb 26, 2014)
We're considering these options:

1) Change hdp to allow ',' in names but then what can we use for delimiter? What will that change effect users?
2) Disallow ',' in SDS names. That also can have bad consequences.
3) Just document in hdp that SDS name with ',' will have this problem.

If we go with option #3, hdp can also be fixed to allow to specify the name partially, i.e., up to the ','; in this case, hdp will print all the SDSs whose name matches that part. This approach can be useful for times when users can't remember the exact names but a few letters at the beginning, then they would be able to see all the names that matches those letters and pick the data set they want.

Barbara's idea: add a "-delim" option, so the user can specify whatever they want for a delimiter? (kind of like %s in "vi")
***BMR*** (Apr 28, 2014)
Need RFC for new ideas first then go from there.

---

<h3 id="HDFFR-1298"></h3>[HDFFR-1298](https://jira.hdfgroup.org/browse/HDFFR-1298)
*Improvements to the HDF4 docs*

#### Description

See issue [OTHER-77](https://jira.hdfgroup.org/browse/OTHER-77) filed by Mike

TODO list:

  *	Spec: make pages landscape as appropriate, e.g., Figure 8ar
  *	UG: page number format consistencies

#### Comments

Comment by ***BMR*** (Mar 14, 2012)
Various formats were fixed in 4.2.7, including page numbers and bookmarks. There are still a lot of work to do for this report.
***BMR*** (Sep 12, 2012)
RM and UG were updated significantly in 4.2.7. The majority of work left for this issue are in the Spec.
***BMR*** (Oct 05, 2012)
Additional improvement: each UG chapter should have its own TOC.

---

<h3 id="HDFFR-1296"></h3>[HDFFR-1296](https://jira.hdfgroup.org/browse/HDFFR-1296)
*Hstartread entry missing from reference manual*

#### Description

Hstartread does not have an entry in the HDF4 reference manual, but it is referenced by several other entries (such as Hnextread). Open pdf and search for Hstartread to verify.

#### Comments

***Elena Pourmal*** (Jan 08, 2012)
Hstartread is a low level function and was not intended to be in RM. We need to find out why do we have other low level functions there and decided how to fix it.

***Ruth Aydt*** (Jan 09, 2012)
From the work on the mapping project, it seems to me that some LL routines are the only way to access certain information in HDF4 files - especially in the early files. If all the LL routines are taken out of the RM, then I want to make sure they are documented somewhere. Currently the documentation for them in the Spec is not complete.

***BMR*** (Sep 12, 2012)
One possibility is to move all the low-level functions to the Spec. Estimate is for this option, but we need to consider where in Spec, and maybe some discussion is needed.

***BMR*** (Oct 24, 2013)
We could also make a section at the end of the RM to house LL functions, then move all existing LL functions into that section and add missing ones gradually. I personally think this is where they should be instead of the Spec. If this is the case, we will also need to add a section for Low level functions to the User's Guide.

---

<h3 id="HDFFR-1265"></h3>[HDFFR-1265](https://jira.hdfgroup.org/browse/HDFFR-1265)
*Clear explanation regarding requirement of external file location*

#### Description

Message from Mike:

As for changing the docs, I like your change very much. I would rephrase it slightly: "Please be aware that the external data file must be kept with the original file. If the external file is not thus available, reading of any datasets with external elements will fail."

I haven't read the surrounding text, so these suggested changes may be inappropriate. Use them for what they are worth.

Also, I forget, but is it possible that the external link has a path of some sort, so that the external file might be in a different place from the parent file? If that is the case, we'll need to adjust the phrasing even more.

Mike

\--------------------------------------------------------  
Message from Ruth:

It is possible to specify an absolute path for the external file when the data is written. If a path is used, it - along with the file name - is saved in the HDF4 file as the "external file name".  
UPDATE: This is not true. "external file name" saved in the file only specifies the file name, not the path. -BMR

It is also possible that you can specify a "search path" where the external file can be found (see HDF4 docs for details). I think this is supposed to work for both the creation and the reading side.

So, in some ways, "kept with original file" isn't quite accurate – it need not be in the same directory as the original file in order for the HDF4 library to find it. IN the schema for the map file, I allowed for there to be a "location" where the file resides - if there is a smart map writer, it can fill in that location, which might be a URI .

---

<h3 id="HDFFR-1259"></h3>[HDFFR-1259](https://jira.hdfgroup.org/browse/HDFFR-1259)
*UG Section 8.6.2 has incorrect information that causes segfault*

#### Description

Section 8.6.2 of the UG (Compressing Raster Images: GRsetcompress) says that "If comp_type is set to COMP_CODE_NONE or COMP_CODE_RLE, the parameters c_info and comp_prm are not used; c_info can be set to NULL..."

I set called GRsetcompress with COMP_CODE_NONE and NULL as the 3rd argument (where c_info is), and got a segfault. I then declared c_info and passed &c_info to the routine and it worked.

The docs never say what c_info is, or how it is used – at least not in this section. I had to go look @ test code to find out. Should show what c_info is and maybe how to use it. At very least, don't indicate NULL Can be passed

#### Comments

***BMR*** (Mar 02, 2014)

Notes were added to Section 8.6.2.
In 4.2.13, GRsetcompress will be fixed to allow c_info parameter to be NULL when comp_type is NONE or RLE.

Update: This contradicts with UG. comp_type = COMP_CODE_NONE is not allowed, according to Section 8.6.2. Perhaps, UG is incorrect.
DF24setcompress(COMP_NONE, NULL); is in the tdatainfo.c
Maybe we should fix to allow:

 * COMP_CODE_NONE and c_info can be NULL

 * COMP_CODE_RLE and c_info can be NULL

---

<h3 id="HDFFR-996"></h3>[HDFFR-996](https://jira.hdfgroup.org/browse/HDFFR-996)
*Documentation text and example for interlace modes in DF24 is very confusing (and wrong?)*

**Attachments:** DF24_nocomp_pixel.c     RIS24_nocomp_line.hdf     RIS24_nocomp_line.hdf     RIS24_nocomp_line2.hdf     RIS24_nocomp_pixel.hdf     RIS24_nocomp_plane.hdf     RIS24_nocomp_plane.hdf     RIS24_nocomp_plane2.hdf |


#### Description

Created an attachment (id=606)  
source code to write test file in pixel interlace

UG and RM documentation and examples for interlace mode in for DF24 images is  
very confusing, and in some places I believe it is just plain wrong.

Example 2 in Section 7.4.2 of the UG declares the raster as  
\[HEIGHT\]\[WIDTH\]\[PIXEL_DEPTH\] (the same as in Example 1 in section 7.4.1.

Example 2 then does "Change interlace from pixel to scan-plane". Then writes  
the image.

The order of the (image) bytes in the HDF4 file is exactly the same with  
Example 1 and Example 2. The setting up of the raster in Example 2 should, I  
guess, reorder the dimensions in the raster when it is initialized because the  
Library doesn't do it when the data is written.

Section 7.2.2.2 does say "HDF stores a 24-bit image using the same interlace  
format as it has in memory". Really, HDF4 relies on the user to lay out the  
image correct on memory and then just writes out the data to the file in that  
order - regardless of what you've said about interlace mode. So, this  
section correctly describes what is happening, but Example 2 doesn't lay things  
out in scan-plane order.

All the interlace mode does is to "flip a bit" in the file saying what the user  
claimed the interlace mode to be. It doesn't rearrange the data in any way.

If you dump these two files with "hdp dumpgr" then you get the values reported  
as being the same for them. If you dump them with "hdp dumprig" then you get  
the values in different orders for the two files.

I kind of suspect the original DF24 interface reordered the bytes before  
storing, while the current version (that relies on the GR call to do interlace)  
changed the behavior. And, all the documentation and tools didn't catch up.  
I believe older files written with original DF24 interface will have the data  
in the file reordered based on interlace mode.

Example code writing test file in 3 interlace modes is attached.

#### Comments

***Ruth Aydt*** (Mar 13, 2011)

Attachment DF24_nocomp_pixel.c has been added with description: source code to write test file in pixel interlace
***Ruth Aydt*** (Mar 13, 2011)
Attachment RIS24_nocomp_plane.hdf has been added with description: source code to write test file in planeinterlace
***Ruth Aydt*** (Mar 13, 2011)
Created an attachment (id=607) source code to write test file in planeinterlace
***Ruth Aydt*** (Mar 13, 2011)
Attachment RIS24_nocomp_line.hdf has been added with description: source code to write test file in line interlace
***Ruth Aydt*** (Mar 13, 2011)
Created an attachment (id=608) source code to write test file in line interlace
***Ruth Aydt*** (Mar 13, 2011)
Created an attachment (id=609) source code to write test file in pixel interlace

Similar to Example 1 Section 7.4.1 in UG, but with values that are easier to track
***Ruth Aydt*** (Mar 13, 2011)
Attachment RIS24_nocomp_pixel.hdf has been added with description: source code to write test file in pixel interlace
***Ruth Aydt*** (Mar 13, 2011)
Attachment RIS24_nocomp_plane.hdf has been added with description: source code to write test file in plane interlace
***Ruth Aydt*** (Mar 13, 2011)
Created an attachment (id=610) source code to write test file in plane interlace

similar to Example 2 in UG section 7.4.2 but with values that are easier to track
***Ruth Aydt*** (Mar 13, 2011)
Attachment RIS24_nocomp_line.hdf has been added with description: source code to write test file in line interlace
***Ruth Aydt*** (Mar 13, 2011)
Created an attachment (id=611) source code to write test file in line interlace

similar to Examples 1 and 2 in UG sections 7.4.1 and 7.4.2 but with scan line interlace and values that make it easier to track.
***Ruth Aydt*** (Mar 13, 2011)
Attachment RIS24_nocomp_plane2.hdf has been added with description: revised source code to build raster in scan plane order & write it that way
***Ruth Aydt*** (Mar 13, 2011)
Created an attachment (id=612) revised source code to build raster in scan plane order & write it that way

Revised version of Example 2 in UG section 7.4.2 that builds Raster in memory in scan-plane order. Values assigned make it easier to see what is going on in the file.
***Ruth Aydt*** (Mar 13, 2011)
Created an attachment (id=613) revised source code to build raster in scan line order & write it that way

Here raster is built in scan-line order in memory. Then scan line interlace is selected for the write. Values for cells are as in all other examples and are hopefully easier to track for debugging.

(hundreds vary for rows, tens vary for columns, units vary for components).
***Ruth Aydt*** (Mar 13, 2011)
Attachment RIS24_nocomp_line2.hdf has been added with description: revised source code to build raster in scan line order & write it that way
***Ruth Aydt*** (Mar 13, 2011)
Played around some more and now convinced Example 2 is the real culprit in the confusion. See revised source code to build raster in scan plan order... attachment for way raster_data should be declared. Also, suggest picking values that are easier to detect what is going on (as in my sample code) rather than those used currently in the docs.
***Ruth Aydt*** (Mar 13, 2011)
regarding output from hdp dumpgr and hdp dumprig, if you run these on the files generated w/ the revised source...

hdp dumpgr reports the order of the data in the file and explicitly lists the interleave.

hdp dumprig doesn't report the interleave but instead reorders the data and always presents it in "pixel interleave" order on the printout.

---

<h3 id="HDFFR-993"></h3>[HDFFR-993](https://jira.hdfgroup.org/browse/HDFFR-993)
*palette not initialized in UG 6.4.2 Ex 2*

#### Description

The variable "colors" is declared but never initialized before being associated with the Raster.

I guess this is okay but you may get random values depending on what system you run on (no guarantee it's zeroed out). Also, kind of a boring palette.

#### Comments

***BMR*** (Oct 31, 2012)

Fix and improve the example while there.

---

<h3 id="HDFFR-992"></h3>[HDFFR-992](https://jira.hdfgroup.org/browse/HDFFR-992)
*confusing (or incorrect) description of dim order in DFR8putimage RM page*

#### Description

The routine signature shows width then height.

The Description says order is different in fortran and c. "f77 must have width as first dim and height as second, the reverse of the way it is done in C"

Fortran sig line also shows width then height.

I think C sig line at top of page is wrong.

I think it would benefit from code example rather than word description "on it's side"

I think code example in section 6.4.1 of UG is also confusing. array declared \[HEIGHT\]\[WIDTH} but then call is made WIDTH, HEIGHT. Fortran call is also WIDTH, HEIGHT. which doesn't agree w/ what RM says (to me)

Trying to read doc and look at example code in UG is very confusing. Would be good to use consistent (and correct) terms.

#### Comments

***BMR*** (Oct 31, 2012)

Will consult with Elena and make sure the terms used consistently throughout the RM and UG.

---

<h3 id="HDFFR-734"></h3>[HDFFR-734](https://jira.hdfgroup.org/browse/HDFFR-734)
*Error in Spec Section 10.4 External Elements*

#### Description

Figure 10f in the HDF4 Specification guide shows the layout for the External Element Descriptor Record incorrectly.

Instead of offset (32 bit integer) followed by filename (non-null terminated ASCII string) offset should be followed by filename_length (32 bit integer giving the length of the filename) and then by filename.

The code for this is in the source file hextelt.c in the routine HXcreate. Look for info->length_file_name.

The File Organization documentation at the beginning of the hextelt.c file is also wrong... there is only a single DD for the information (as shown in the spec documentation w/ the above correction), not 2 DDs as shown in the source code comments.

---

<h3 id="HDFFR-733"></h3>[HDFFR-733](https://jira.hdfgroup.org/browse/HDFFR-733)
*UG sections 8.11. should tell current behavior, not designed behavior*

#### Description

What is allowable for palettes via the GR interface is described as **designed** in this section of the UG, not as implemented. This isn't really very helpful for the reader. While the RM does talk about some restrictions, I think the actual behavior should also be in the UG.

8.11.2 - number of palettes will always be 0 or 1

8.11.4 - n_comps always 3; data_type always uint8, interlace_mode always PIXEL, num_entries always 256.

8.11.5 - see restrictions in 8.11.4

---

<h3 id="HDFFR-732"></h3>[HDFFR-732](https://jira.hdfgroup.org/browse/HDFFR-732)
*RM entry for GRwritelut should be improved*

#### Description

The RM entry for GRwritelut could be improved to let users know what the library does (and doesn't support).

Currently in the description it says "Currently only "old-sty'e pallets are supported. i.e., ncomp=3, num_entries=256, data_type=uint8."

It should also say in that list "interlace_mode=MFGR_INTERLACE_PIXEL".

And, I think it would be good in each of the following paragraphs to say what each parameters must be. For example "ncomp must be 3 in the current version".

Easy ( at least for me ) to read past the restrictions.

#### Comments

***BMR*** (Mar 02, 2014)

Need valid values for n_entries, are they \[1...256\]?

---

<h3 id="HDFFR-111"></h3>[HDFFR-111](https://jira.hdfgroup.org/browse/HDFFR-111)
*UG: writing dimension scale w/DFSD api not clear*

#### Description

Section 12.7.3.2 describes how to write a dimension scale  
with HDF4, using the DFSD interface. However, if you use  
just the two functions specified, it didn't work. I had  
to "set" the dimensions first:  
DFSDsetdims

Also, in the last sentence of this section, DFSDsetdimscale is  
spelled incorrectly as "DFSDsetdiscale".

---

<h3 id="HDFFR-612"></h3>[HDFFR-612](https://jira.hdfgroup.org/browse/HDFFR-612)
*Add hrepack/hdiff info to documentation*

**Duplicate**     |     |     |

is duplicated by [~~HDFFR-545~~](https://jira.hdfgroup.org/browse/HDFFR-545) | hdiff: User's Guide doc does not list... | Resolved |

#### Description

Add the information at the following location to the documentation:  
<http://www.hdfgroup.uiuc.edu/HDF4/doctools/>

There is extra information for hrepack and hdiff at this location.

#### Comments

***BMR*** (Dec 06, 2009)

Updated Section 15.3, hdiff, and Section 15.15, hrepack, in the HDF4 User's Guide, according to source code. There is some updating necessary to the link mentioned in the previous comment before this bug can be closed.
***BMR*** (Dec 06, 2009)
   

---

<h3 id="HDFFR-157"></h3>[HDFFR-157](https://jira.hdfgroup.org/browse/HDFFR-157)
*GR: Add examples to doc on image store/retrieving*

#### Description

GR Documentation:

As documentation goes, I think the HDF docs are among the best that I've seen.  
However, I would like to see more examples on this point about image store/retrieving.

I guess, in particular on this point of byte indices in to a color palette – which is fairly common situation

#### Comments

***BMR*** (Jun 24, 2016)

Might already be added.

---

<h3 id="HDFFR-208"></h3>[HDFFR-208](https://jira.hdfgroup.org/browse/HDFFR-208)
*Feedback on Chapter 7 of Spec (6/25/08 draft version)*

#### Description

Here are some things I noticed when reading. Most are very minor.

1) Fig 7a, rectangle isn't showing up around "A" in table (HDF Object)  
2) Text after Fib 7b "In object description figures, e.g.Figure 7i" should be "In... figures, e.g., Figure 7i"  
3) Last sentence before Fig 7d "... the H, V, and SD interfaces" should be "GR" instead of "SD"  
4) Table 7a - I wasn't sure why "contiguous" was spelled out for SDS, but for dimension variable "default" was used. Why aren't both "contiguous" or both "default". Also, The note "contiguous storage is the default layout and requires no special storage tag" has a \* but there is no \* in the table text.  
5) section 7.8 - first bullet in first list. should be ...or opened, not or open  
6) section 7.8 4th bullet in first list. I would say "global attributes are set for the SD collection" (not for the file) .... but, this terminology occurs many places, not just here.  
7) Section 7.8.1 "The objects are stored..." I wondered what objects this referred to.  
8) Figure 7u - I would have liked to see a dimension scale also shown w/ the SDS.. the way the "dimensions" and "dimension scales" relate is still somewhat fuzzy to me.  
9) Section 7.8.4 - another place where attributes are called "global".  
10) Figure 7x, 7y - another place I was not sure about the dimension scale... I think there's a dimension attribute but no dimension "data" - why do we have the values & list of dim indices? Are they just empty?  
11) Missing dimension scale vgroup - reported separately to BM because I wasn't sure I understood correctly what in fact was supposed to be there.  
12) Section 7.8.7 - first sentence "sets values to" should be "sets values of"

Wow - I learned a lot from this chapter!

---

<h3 id="HDFFR-209"></h3>[HDFFR-209](https://jira.hdfgroup.org/browse/HDFFR-209)
*Feedback on Chapter 8 of Spec (6/25/08 draft)*

#### Description

Some things I noticed when reading the draft.  

1) Page numbers should be "different". For example, in section 8.2.1, it talks aboue "in Figure 8j on page 9". I would like to see 8-9 or the whole manual numbered together. This may just be the draft version numbering.  
2) Figure 8l - I would also like to see how a palette is represented in the GR data set (left hand side of diagram)  
3) Section 8.5, paragraph that begins "An RI8 image is incorporated..." - last phrase in paragraph, should be either "there is no RIG object" or "there is not a RIG object"...  
4) Figures 8o and 8t are not formatted properly  
5) Would have liked a bit more discussion/detail about palettes (section 8.9.4) talks about palette dimension but what about palette data (or am I totally confused about there being data!)?

---

<h3 id="HDFFR-223"></h3>[HDFFR-223](https://jira.hdfgroup.org/browse/HDFFR-223)
*UG: Confusing information about supported data types*

#### Description

Section 3.7.2 (and I think quite a few others) say:

"The parameter data_type contains any type that HDF supports for the scientific data. Refer to table 2F on page 14".

In fact, Types in table 2G and 2H are also supported.

Table 2H (Little endian, IEEE I think, based on code read) should be readable on any system.)

Table 2G (Native) should be fine if you read the data on the same type of system you wrote it on.

Saying only types in 2F are supported is really confusing when the reader (me, in this case) needs to know exact information about what might be returned by a call in the data_type field.

Maybe say any of the types may be returned; NATIVE only supported on that platform. (after checking code to confirm this behavior)

#### Comments

***Ruth Aydt*** (Apr 30, 2010)

Thinking some more about how to address this confusion, I'm looking at the table 1A in the Ref Manual

In that table, the Data Type Definitions for "standard" and "Native" are combined in a single table. Little Endian isn't presented.

In UG, wonder if text for sections 2.5.2.1 could come first. Then 2.5.2.3. Then 2.5.2.2 – With a WARNING that data stored in the Native format to the file won't be accessible, except on the same type of system as where it was written.

Then – have a table that has all 3 types – kind of like 1A in RefManual.

Later text could refer to that big table – any of the types are possible. **but**, the disclaimer about how LE and NATIVE are treated in the file would be there in the 2.5.2 text. I don't recall reading anywhere that NATIVE types written to file would not be readable on any system.

(I may have missed it).
***BMR*** (Nov 01, 2013)
Must find all other occurrences as well.

---

<h3 id="HDFFR-731"></h3>[HDFFR-731](https://jira.hdfgroup.org/browse/HDFFR-731)
*compressing 8-bit raster example wrong (or confusing)*

**Unanet:** ORNL ECP-SDK (14102) |


#### Description

In UG, Chapter 6, example 4. "Compressing and Writing a 8-Bit Raster Image"  
–  
In Spec, says RIS8 has a single 8-bit component, with an optional palette.  
In UG example, shows a pixel_depth of 3 in the raster_data... I thought that would be a RIS24, not a RIS8. This doesn't make sense to me. Is it because it's JPEG compression that the raster data has 3 components?

No depth in example3 (that makes sense).

Also, the routine for reading data (DF48getimage) doesn't have any way to specify a pixel depth. I think example4 belongs in the RIS24 chapter, not in RIS8. Or, it needs better explanation if it's in RIS8.

---

<h3 id="HDFFR-228"></h3>[HDFFR-228](https://jira.hdfgroup.org/browse/HDFFR-228)
*VSwrite in RG confusing*

#### Description

I found the description of VSwrite in the reference guide to be quite confusing. I wanted to write a Vdata in NO_INTERLACE mode and there are few (no) examples of how to do this. The description on the VSwrite page also confused me.

In particular, end of Para 1 in Description says "The parameter interlace_mode defines the interlace mode of the vdata fields stored in the buffer databuf"

Then, the next paragraph says, "... Selecting FULL_INTERLACE fills databuf by record.... Specifying NO_INTERLACE causes databuf to be filled by field.

But - databuf is **already filled** when this call is made. Selecting the option just tells how it is filled.

This text needs to be fixed and an example would be really helpful. I **think** I figured it out via trial & error

(Call VSdefine, VSsetfields, VSsetinterlace(NO_INTERLACE), follow examples to fill and pack databuf in FULL_INTERLACE, call VSwrite w/ FULL_INTERLACE. )

---

<h3 id="HDFFR-728"></h3>[HDFFR-728](https://jira.hdfgroup.org/browse/HDFFR-728)
*docs mention default fill values but never say what they are*

#### Description

The UG and RM say that if you haven't set a fill value, the default fill value is used. But, I searched the PDFs for values of default fill values and they are never given.

Binh-Minh pointed me to them in the file mfhdf/libsrc/netcdf.h.in

I think they need to be in the docs.

I'm also a little confused about why they are in a netcdf.h header if used for HDF4 overall. And, it looks like there are 2 versions for the float/double values. How do you know what version to use when reading the file - possibly years later? Guess we'll just use the new ones.

#### Comments

***BMR*** (Mar 09, 2014)

The documents show the default fill values now, but there is some history about the old values that should be noted somehow, so the issue is left opened.

---

<h3 id="HDFFR-1291"></h3>[HDFFR-1291](https://jira.hdfgroup.org/browse/HDFFR-1291)
*SDS with rank=0*

#### Description

Elena wrote in regarding to whether SDS with rank=0 is allowed in HDF4:

"...depends on implementation of malloc(0). C standard allows it to be a NULL pointer or a pointer that is not recommended to be passed around except to free() call.

The HDF library code doesn't treat rank=0 as a special case, and as a result, we have APIs that depend on compiler implementation (i.e., SDcreate will fail for one system and succeed on another when rank=0 is used.)

I doubt that it was someone's intention. The code was developed with rank >= 1 in mind.

Bottom line is that we all agreed on:

1) SD interface will allow to create, write and read datasets with rank=0.  
Functions to set any "specialness" like compression will return fail for such datasets.  
Documentation will be updated accordingly.  
Tools will be reviewed and updated to make them all consistent.  
Source code will be cleaned up to resolve malloc issues when rank=0

2) DFSD interface will be cleaned up to resolve malloc issues; rank=0 will not be allowed."

---

<h3 id="HDFFR-471"></h3>[HDFFR-471](https://jira.hdfgroup.org/browse/HDFFR-471)
*ug: SDsetblocksize page had errors*

#### Description

arg 1 should be sds_id, not sd_id

arg summary should be:  
sds_id IN: "SDS interface identifier returned by SDcreate"

Later, in description, it doesn't set it for all data sets in the file, just for the dataset created with SDcreate. And, SDselect should be removed as if it's selectable then it's not new.

I tried coding it w/ the sd_id returned by SDstart and got -1 returned from call.

Coding w/ sds_id returned from SDcreate I got 0 returned from the call. But, that still didn't seem to change the block size in the file... guess that's another issue. Perhaps user error. Elena looked and the function isn't in the tests so not sure.

#### Comments

***BMR*** (Jan 02, 2014)
It's supposed to be sds_id. The document has been corrected. The function is being worked on.

***BMR*** (Jan 03, 2014)
It is true that the function doesn't always change the block size in the file. It'll only have the effect if the data set is written after SDsetblocksize is called and before closing and reopening the file. VSsetblocksize seems to have the same issue.

---

<h3 id="HDFFR-224"></h3>[HDFFR-224](https://jira.hdfgroup.org/browse/HDFFR-224)
*Docs have VFfieldisize and VFfieldesize backwards*

#### Description

This will only matter if the sizes of fields as stored in the file and as on the system where they are being accessed is different. typically that is not the case.

But...

The UG and RM for VFfieldesize and VFfieldisize say "esize" is about the size as stored on disk, and "isize" is about the size in memory.

The source code comments in vsfld.c and vgint.h (See VWRITELIST isize & esize) say the opposite. That "internal" is in the HDF file and "external" is local machine.

After more digging in code, I'm convinced that the manuals are wrong.

See file vio.c, routine vunpackvs, near the very end where esize is set. isize is what is in the file. esize is what is native to the machine.

#### Comments

***Ruth Aydt*** (May 12, 2010)

IT would also be good to say that the size is returned in units of Bytes. And, that it's for all components of the field. (on both pages and also when discussed in UG)


---

<h3 id="HDFFR-1403"></h3>[HDFFR-1403](https://jira.hdfgroup.org/browse/HDFFR-1403)
*UG issues in the List of Examples*

#### Description

I was looking through the List of Examples on this page of the HDF4  
User's Guide:  
<http://www.hdfgroup.org/release4/doc/UsrGuide_html/UG_htmlLOE.html>

I noticed these links were missing:

Chapter 7, 10, and 11 do not have links to the beginning of the chapter,  
whereas all of the other chapters do.

Also when I clicked on the examples to see the descriptions, I found  
some issues:

(Ch 5 VG) EXAMPLE 2: Click on link and it does not go to the example.  
(Ch 5 VG) EXAMPLE 3: Click on link and it does not go to the example.

Other minor issues are included below: <=== Done (Feb 8, 2014)

(Ch 10 AN) EXAMPLE 2: Last sentence in description has no period and spells  
"routine" incorrectly. Also "the" before "Section" can be removed:  
"This rouitne is described in the Section 10.6.1"

(Ch 10 AN) EXAMPLE 3: Change "identifiers" to "identifiers":  
Second paragraph, 3rd sentence:  
"..If there are any identifiers"

(Ch 8 GR) EXAMPLE 9: (missing from source - entered [HDFFR-1402](https://jira.hdfgroup.org/browse/HDFFR-1402) for this.)

(Ch 3 SD) EXAMPLE 3 Add "a" before "series" in this sentence:  
"This example shows how to fill a 3-dimensional SDS array with data by  
writing series of 2-dimensional slabs to it."

(Ch 3 SD) EXAMPLE 7: Change 'the' to 'an' before "External" as well as "external" in  
this sentence:  
"Moving Data to the External File.  
This example illustrates the use of the routine SDsetexternalfile/sfsextf to  
move the SDS data written in Example 2 to the external file."

(Ch 3 SD) EXAMPLE 8: First sentence, change "in the Example 7" to ".. "in Example 7"  
(Ch 3 SD) EXAMPLE 9: says it uses SDS created in Example 2, but it is created in Example 1.  
(Ch 3 SD) EXAMPLE 12: says it uses SDS created in Example 2, but it is created in Example 1.  
(Ch 3 SD) EXAMPLE 17: Change "transpose" to "transposed" in the sentence beginning:  
"Note that the Fortran example uses transpose data..."

(Ch 4 VD) EXAMPLE 4: 2nd paragraph, change "into a databuf" to "into a buffer"  
(Ch 4 VD) EXAMPLES 7,8,9: It looks like the HDF file is created in Example 1 and modified  
in Examples 2, and 3, but what is written there may be okay??


---

<h3 id="HDFFR-1330"></h3>[HDFFR-1330](https://jira.hdfgroup.org/browse/HDFFR-1330)
*GrayJPEG5 (tag 16) LL Objects seen but not expected*

**Attachments:** createRastersPalettes.c |


#### Description

The attached program creates a file that has 2 DDs with tag 16 - GRAYJPEG5 according to the include file htags.h.

This tag isn't documented in the UG appendix A or in the SPEC and I believe people thought it was not used. Apparently that isn't the case.

Note that we are not committed to mapping this, but I did not expect it to be in any files.

This was originally part of issue 995 (see 12/21 comment), but most of the issues there were resolved so breaking this out as separate report.

---

<h3 id="HDFFR-1344"></h3>[HDFFR-1344](https://jira.hdfgroup.org/browse/HDFFR-1344)
*hdfimport documentation does not correctly reflect how to create a 2D dataset with hdfimport*

#### Description

The hdfimport tool documentation does not clearly define how you should create a 2D dataset. You have to add a 3rd dimension of 1 after "TEXT" or the 2D dataset is not created properly.

For example, this creates a 2D dataset that is 4x3:

~~~
TEXT  
1  
4  
3  
1  
0  
1 2 3 4  
1 2 3  
1 1 0  
1 1 0  
0 0 0  
1 1 0
~~~

Also, it would be very helpful if there were example input files (with different datatypes) in the documentation as well as in the hdfimport usage statement, to help users understand how to use it.

Here is another example:

~~~
TEXT  
1  
10  
10  
2686  
2409  
180 181 182 183 184 185 186 187 188 189  
280 281 282 283 284 285 286 287 288 289  
2409 2438 2468 2500 2538 2569 2595 2611 2604 2586  
2416 2442 2483 2513 2549 2582 2612 2635 2638 2619  
2425 2448 2495 2558 2593 2628 2656 2665 2645 2626  
2444 2468 2500 2531 2564 2604 2646 2674 2682 2666  
2463 2485 2512 2542 2574 2611 2651 2678 2686 2676  
2463 2485 2512 2542 2574 2611 2651 2678 2686 2676  
2444 2468 2500 2531 2564 2604 2646 2674 2682 2666  
2425 2448 2495 2558 2593 2628 2656 2665 2645 2626  
2416 2442 2483 2513 2549 2582 2612 2635 2638 2619  
2409 2438 2468 2500 2538 2569 2595 2611 2604 2586
~~~

Here is a 3D example:

~~~
TEXT  
2 2 5  
.98 .90  
.98 .96  
.98 .96  
.98 .96 .94 .92 .90  
.42 .31 .24 .32 .29  
.31 .32 .32 .43 .28  
.30 .30 .30 .30 .30  
.30 .30 .30 .30 .30
~~~

#### Comments

***Barbara Jones*** (Nov 30, 2012)

See
***BMR*** (Dec 31, 2013)
Need more time for examples.


---

<h3 id="HDFFR-1386"></h3>[HDFFR-1386](https://jira.hdfgroup.org/browse/HDFFR-1386)
*Identify all the places to document this issue and document them*

#### Description

Excerpt of Mike's message:  
"Also, try to identify of all the places where it might be beneficial to document this issue, should it arise in other contexts, then do the documentation."


---

<h3 id="HDFFR-1387"></h3>[HDFFR-1387](https://jira.hdfgroup.org/browse/HDFFR-1387)
*You cannot create an SDS with both chunking and unlimited dimension*

**Attachments:** chunking_ex-unl.c |


#### Description

You cannot create an SDS with both chunking and unlimited dimension.

They are not supported together. This is the code in SDsetchunk to  
reject that combination:

~~~
for (i = 0; i < ndims; i++)  { /* get dimension length from shape arrays */
/* check if dimension in unlimited since we don't handle that yet */  
if (var->shape[i] != SD_UNLIMITED)  
    chunk[0].pdims[i].dim_length = (int32) var->shape[i];  
else { /* UNLIMITED dimension case */
    ret_value = FAIL;
    goto done;
}
~~~

The issue needs to be investigated and documented.  
See the attached program, in which SDsetchunk fails.

#### Comments

***BMR*** (Feb 22, 2024)

Mentioned in Section 3.12 of the UG, need to add to the SDsetchunk RM page, then close this issue.

---

<h3 id="HDFFR-1392"></h3>[HDFFR-1392](https://jira.hdfgroup.org/browse/HDFFR-1392)
*Document the interpretation of ncdump output*

#### Description

Document how the output for dimensions in ncdump are interpreted. This issue is a follow-up of [HDFFR-1391](https://jira.hdfgroup.org/browse/HDFFR-1391).


---

<h3 id="HDFFR-1478"></h3>[HDFFR-1478](https://jira.hdfgroup.org/browse/HDFFR-1478)
*VSinquire and vsfinq behavior*

#### Description

I noticed that the Fortran VD_get_vdata_info.f example fails to retrieve Vdata information of the first Vdata in the file (just check the return value of the vsfinq function):

~~~
File name: General_Vdatas.hdf

Vdata: 0  
tag = 1962; reference = 2;  
number of records = 0; interlace = FULL_INTERLACE (0);  
fields = <Undefined>;  
record size (in bytes) = 0;  
name = Vdata 1; class = <Undefined>;  
number of attributes = 0  
No data written
~~~

C code for VSinquire clearly states that function will return FAIL if one of the parameters cannot be retrieved (in this example it were two parameters - the total size of the fields and the Vdata name). The function does retrieve other parameters successfully. Documentation for the C function doesn't tell how the failed or successful parameters can be discovered.

Fortran wrapper function falls into this trap and doesn't return any parameters it could.

We need to decide if we want to change the behavior of the Fortran wrapper and document both C and Fortran functions accordingly.


---

<h3 id="HDFFR-1517"></h3>[HDFFR-1517](https://jira.hdfgroup.org/browse/HDFFR-1517)
*Make sure hdp commands list options correctly*

#### Description

hdp commands don't specify flags correctly when they are mutually exclusive or not.


---

<h3 id="HDFFR-1518"></h3>[HDFFR-1518](https://jira.hdfgroup.org/browse/HDFFR-1518)
*RM entries need improvement and/or correction*

#### Description

* GRwritelut
* SDgetdatainfo

#### Comments

***BMR*** (Oct 23, 2015)

Estimate is for all entries which will be added as they are found. Each entry should only take about .5h or less, except the ones that require investigation in the source code.


---

<h3 id="HDFFR-1523"></h3>[HDFFR-1523](https://jira.hdfgroup.org/browse/HDFFR-1523)
*Update documentation for the library code and file format, and user documentation*

#### Description

In order to effectively maintain the library, we must identify major areas of the library to document the design for future maintenance. I have done this work in some areas, but probably only mounts up to about 15% of the library. The material is in the HDF4 Specification document. The tasks for documentation are estimated as below:

* Refactor, cleanup, and document the library and test code
* Document the file format, including reviewing and updating various existing materials
* Update user documentation (UG and RM) and tutorials
