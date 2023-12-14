File name: h4ex_GR_write_palette.hdf 


Vgroup:0
     tag = 1965; reference = 2;
     name = Image with Palette; class = RI0.0;
     number of entries = 4;
   number of attributes = 0 
Entries:-
     #0 (Image Dimensions)
	tag = 300; reference = 1;
     #1 (Raster Image Data)
	tag = 302; reference = 1;
     #2 (Palette Dimension)
	tag = 307; reference = 1;
     #3 (Image Palette)
	tag = 301; reference = 1;


Vgroup:1
     tag = 1965; reference = 3;
     name = RIG0.0; class = RIG0.0;
     number of entries = 1;
   number of attributes = 0 
Entries:-
     #0 (Vgroup)
	tag = 1965; reference = 2;
	number of entries = 4;
	name = Image with Palette; class = RI0.0
   number of attributes = 0 


Graphical representation of the file:-
(vg#: vgroup;   vd: vdata)

   vg0 	-- Image Dimensions  
	-- Raster Image Data  
	-- Palette Dimension  
	-- Image Palette  

   vg1 	-- vg0 	-- Image Dimensions  
		-- Raster Image Data  
		-- Palette Dimension  
		-- Image Palette  

