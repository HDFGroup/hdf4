File name: h4ex_VG_add_sds_to_vgroup.hdf 


Vgroup:0
     tag = 1965; reference = 3;
     name = SD Vgroup; class = Common Vgroups;
     number of entries = 1;
   number of attributes = 0 
Entries:-
     #0 (Numeric Data Group)
	tag = 720; reference = 2;


Vgroup:1
     tag = 1965; reference = 5;
     name = fakeDim0; class = Dim0.0;
     number of entries = 1;
   number of attributes = 0 
Entries:-
     #0 (Vdata)
	tag = 1962; reference = 4; 
	number of records = 1; interlace = 0;
	fields = [Values];
	record size (in bytes) = 4;
	name = fakeDim0; class = DimVal0.1;
	total number of attributes = 0.


Vgroup:2
     tag = 1965; reference = 8;
     name = Test SD; class = Var0.0;
     number of entries = 5;
   number of attributes = 0 
Entries:-
     #0 (Vgroup)
	tag = 1965; reference = 5;
	number of entries = 1;
	name = fakeDim0; class = Dim0.0
   number of attributes = 0 
     #1 (Vdata)
	tag = 1962; reference = 6; 
	number of records = 0; interlace = 0;
	fields = [SDS variable];
	record size (in bytes) = 4;
	name = <Undefined>; class = SDSVar;
	total number of attributes = 0.
     #2 (Number type)
	tag = 106; reference = 7;
     #3 (SciData dimension record)
	tag = 701; reference = 7;
     #4 (Numeric Data Group)
	tag = 720; reference = 2;


Vgroup:3
     tag = 1965; reference = 9;
     name = h4ex_VG_add_sds_to_vgroup.hdf; class = CDF0.0;
     number of entries = 2;
   number of attributes = 0 
Entries:-
     #0 (Vgroup)
	tag = 1965; reference = 5;
	number of entries = 1;
	name = fakeDim0; class = Dim0.0
   number of attributes = 0 
     #1 (Vgroup)
	tag = 1965; reference = 8;
	number of entries = 5;
	name = Test SD; class = Var0.0
   number of attributes = 0 


Graphical representation of the file:-
(vg#: vgroup;   vd: vdata)

   vg0 	-- Numeric Data Group  

   vg1 	-- vd  

   vg2 	-- vg1 	-- vd  
	-- vd  
	-- Number type  
	-- SciData dimension record  
	-- Numeric Data Group  

   vg3 	-- vg1 	-- vd  
	-- vg2 	-- vg1 	-- vd  
		-- vd  
		-- Number type  
		-- SciData dimension record  
		-- Numeric Data Group  

