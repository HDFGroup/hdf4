/****************************************************************************
 * NCSA HDF                                                                 *
 * Software Development Group                                               *
 * National Center for Supercomputing Applications                          *
 * University of Illinois at Urbana-Champaign                               *
 * 605 E. Springfield, Champaign IL 61820                                   *
 *                                                                          *
 * For conditions of distribution and use, see the accompanying             *
 * hdf/COPYING file.                                                        *
 *                                                                          *
 ****************************************************************************/

#ifdef RCSID
static char RcsId[] = "@(#)$Revision$";
#endif

/* $Id$ */

/*

   A more portably useful Vset testing package

 */
#include "hdf.h"
#include "hfile.h"
#include "tproto.h"

#define VDATA_COUNT  256   /* make this many Vdatas to check for memory leaks */

#define FNAME0   "tvset.hdf"
#define FNAME1   "tvset1.hdf"
#define FNAME2   "tvset2.hdf"
#define EXTFNM	 "tvsetext.hdf"

#define FIELD1       "FIELD_name_HERE"
#define FIELD1_UPPER "FIELD_NAME_HERE"
#define FIELD2       "DIFFERENT_FIELD_NAME"

static int32 write_vset_stuff(void);
static int32 read_vset_stuff(void);

/* write some stuff to the file */
static int32
write_vset_stuff(void)
{
    int32       status;
    int32       fid, aid;
    int32       vg1, vg2;
    int32       vs1;
    int32       count, i, j, num, max_order;
    int32       ibuf[2000];     /* integer buffer */
    float32     fbuf[2000];     /* floating point buffer */
    char        gbuf[2000];     /* generic buffer */
    uint8       *gbuf1 = NULL;   /* buffer for uint8 */
    float32     *gbuf2 = NULL;   /* buffer for float32 */
    const char *name;
    char       *p;
    char8       c;
    float32     f;

    /* allocate these buffers dynamically and not off the stack
       as they were previously handled */
    if (gbuf1 == NULL)
     {
       gbuf1 = (uint8 *)HDmalloc(sizeof(uint8)*65536);
     }

    if (gbuf2 == NULL)
     {
       gbuf2 = (float32 *)HDmalloc(sizeof(float32)*20000);
     }


    fid = Hopen(FNAME0, DFACC_CREATE, 100);
    if (fid == FAIL)
      {
          num_errs++;
          return FAIL;
      }

    if (Vstart(fid) == FAIL)
      {
          num_errs++;
          return FAIL;
      }

    /*

     * Vgroup Generation routines
     *
     */

    /*
     *  start simple --- create a simple Vgroup
     */
    vg1 = Vattach(fid, -1, "w");
    if (vg1 == FAIL)
      {
          num_errs++;
          printf(">>> Failed creating initial Vgroup\n");
      }

    Vsetname(vg1, "Simple Vgroup");
    Vsetclass(vg1, "Test object");
    MESSAGE(5, printf("created Vgroup %s (empty)\n", "Simple Vgroup");
        );

    /*
     * Lets do some more complex ones now
     */
    vg2 = Vattach(fid, -1, "w");
    if (vg2 == FAIL)
      {
          num_errs++;
          printf(">>> Failed creating second Vgroup\n");
      }

    /* keep track of how many in Vgroup */
    num = 0;

    /* add first group into the other */
    status = Vinsert(vg2, vg1);
    if (status == FAIL)
      {
          num_errs++;
          printf(">>> Vinsert failed\n");
      }
    else
        num++;

    /* add a bogus element */
    status = Vaddtagref(vg2, (int32) 1000, (int32) 12345);
    if (status == FAIL)
      {
          num_errs++;
          printf(">>> Vaddtagref failed for bogus element\n");
      }
    else
        num++;

    /* create an element and insert that */
    aid = Hstartwrite(fid, (uint16) 123, (uint16) 1234, 10);
    if (aid == FAIL)
      {
          num_errs++;
          printf(">>> Hstartwrite failed\n");
      }
    Hendaccess(aid);

    /* add an existing HDF element */
    status = Vaddtagref(vg2, (int32) 123, (int32) 1234);
    if (status == FAIL)
      {
          num_errs++;
          printf(">>> Vaddtagref failed for legit element\n");
      }
    else
        num++;

#ifdef NO_DUPLICATES
    /* attempt to add an element already in the Vgroup */
    status = Vaddtagref(vg2, (int32) 123, (int32) 1234);
    if (status != FAIL)
      {
          num_errs++;
          printf(">>> Vaddtagref added a duplicate element\n");
      }

    /* check that the number is correct */
    if (num != Vntagrefs(vg2))
      {
          num_errs++;
          printf(">>> Vntagrefs returned %d was expecting %d\n", Vntagrefs(vg2), num);
      }
#endif /* NO_DUPLICATES */

    /* lets check the contents */
    /* look for a valid one first */
    if (Vinqtagref(vg2, 1000, 12345) == FALSE)
      {
          num_errs++;
          printf(">>> Vinqtagref couldn't find valid element\n");
      }

    /* look for a bogus one */
    if (Vinqtagref(vg2, 1000, 123456) != FALSE)
      {
          num_errs++;
          printf(">>> Vinqtagref found a bogus element\n");
      }

    Vsetname(vg2, "Second Vgroup");
    Vsetclass(vg2, "Test object");

    Vdetach(vg1);
    Vdetach(vg2);
    MESSAGE(5, printf("created Vgroup %s with %d elements\n", "Second Vgroup", (int) num);
        );

    /*

     * Vdata Generation routines
     *
     */

    /* Float32 Vdata */
    vs1 = VSattach(fid, -1, "w");
    CHECK(vs1,FAIL,"VSattach");

    name = "Float Vdata";
    status=VSsetname(vs1, name);
    CHECK(status,FAIL,"VSsetname");

    status=VSsetclass(vs1, "Test object");
    CHECK(status,FAIL,"VSsetclass");

    status=VSfdefine(vs1, FIELD1, DFNT_FLOAT32, 1);
    CHECK(status,FAIL,"VSfdefine");

    status = VSsetfields(vs1, FIELD1);
    if (status == FAIL)
      {
          num_errs++;
          printf(">>> Vsetfields failed for %s\n", name);
      }

    /* create some bogus data */
    for (i = 0, count = 100; i < count; i++)
        fbuf[i] = (float32) i;

    /* store it */
    VSwrite(vs1, (unsigned char *) fbuf, count, FULL_INTERLACE);
    VSdetach(vs1);
    MESSAGE(5, printf("created VDATA %s with %d elements\n", name, (int) count);
        );

    /* Int32 Vdata */
    vs1 = VSattach(fid, -1, "w");
    name = "Integer Vdata";
    VSsetname(vs1, name);
    VSsetclass(vs1, "Test object");
    VSfdefine(vs1, FIELD2, DFNT_INT32, 2);
    status = VSsetfields(vs1, FIELD2);
    if (status == FAIL)
      {
          num_errs++;
          printf(">>> Vsetfields failed for %s\n", name);
      }
    /* change this vdata to store in an external file */
    status = VSsetexternalfile(vs1, EXTFNM, (int32) 0);
    if (status == FAIL)
      {
          num_errs++;
          printf(">>> VSsetexternalfile failed\n");
      }

    /* create some bogus data */
    for (i = 0, count = 100; i < 2 * count; i++)
        ibuf[i] = i;

    /* store it */
    VSwrite(vs1, (unsigned char *) ibuf, count, FULL_INTERLACE);
    VSdetach(vs1);
    MESSAGE(5, printf("created VDATA %s with %d elements\n", name, (int) count);
        );

    /* Int32 and Float32 Vdata */
    vs1 = VSattach(fid, -1, "w");
    name = "Mixed Vdata";
    VSsetname(vs1, name);
    VSsetclass(vs1, "No class specified");
    VSfdefine(vs1, "A", DFNT_FLOAT32, 1);
    VSfdefine(vs1, "B", DFNT_INT32, 1);
    status = VSsetfields(vs1, "A, B");
    if (status == FAIL)
      {
          num_errs++;
          printf(">>> Vsetfields failed for %s\n", name);
      }

    /* create some bogus data */
    p = gbuf;
    for (i = 0, count = 100; i < count; i++)
      {
          float32     tf = (float32) (i * 2);
          HDmemcpy(p, &tf, sizeof(float32));
          p += sizeof(float32);
          HDmemcpy(p, &i, sizeof(int32));
          p += sizeof(int32);
      }

    /* store it */
    VSwrite(vs1, (unsigned char *) gbuf, count, FULL_INTERLACE);
    VSdetach(vs1);
    MESSAGE(5, printf("created VDATA %s with %d elements\n", name, (int) count);
        );

#define ST "STATION_NAME"
#define VL "VALUES"
#define FL "FLOATS"
#define MX "STATION_NAME,VALUES,FLOATS"

    /* mixed order Vdata */
    vs1 = VSattach(fid, -1, "w");
    name = "Multi-Order Vdata";
    VSsetname(vs1, name);
    VSsetclass(vs1, "No class specified");
    VSfdefine(vs1, ST, DFNT_CHAR8, 2);
    VSfdefine(vs1, VL, DFNT_INT32, 3);
    VSfdefine(vs1, FL, DFNT_FLOAT32, 1);
    status = VSsetfields(vs1, MX);
    if (status == FAIL)
      {
          num_errs++;
          printf(">>> Vsetfields failed for %s\n", name);
      }

    /* create some bogus data */
    p = gbuf;
    c = 'a';
    j = 0;
    f = (float32) 15.5;
    for (i = 0, count = 10; i < count; i++)
      {
          HDmemcpy(p, &c, sizeof(char8));
          p += sizeof(char8);
          c++;
          HDmemcpy(p, &c, sizeof(char8));
          p += sizeof(char8);
          c++;
          HDmemcpy(p, &j, sizeof(int32));
          p += sizeof(int32);
          j++;
          HDmemcpy(p, &j, sizeof(int32));
          p += sizeof(int32);
          j++;
          HDmemcpy(p, &j, sizeof(int32));
          p += sizeof(int32);
          j++;
          HDmemcpy(p, &f, sizeof(float32));
          p += sizeof(float32);
          f += (float32) 0.5;
      }

    /* store it */
    VSwrite(vs1, (unsigned char *) gbuf, count, FULL_INTERLACE);
    VSdetach(vs1);
    MESSAGE(5, printf("created VDATA %s with %d elements\n", name, (int) count);
        );

    /* test MAX_ORDER and MAX_FIELD_SIZE */
    vs1 = VSattach(fid, -1, "w");
    name = "Max_Order Vdata";
    VSsetname(vs1, name);
    VSfdefine(vs1, "max_order", DFNT_UINT8, MAX_ORDER);
    status = VSsetfields(vs1, "max_order");
    if (status == FAIL)
      {
          num_errs++;
          printf(">>> Vsetfields failed for %s\n", name);
      }

    /* create some bogus data */
    for (i = 0; i < MAX_ORDER; i++)
         gbuf1[i] = (uint8)(i % 256);
    VSwrite(vs1, (unsigned char *) gbuf1, 1, FULL_INTERLACE);
    VSdetach(vs1);
    MESSAGE(5, printf("created VDATA %s with %d order\n", name, (int)
 MAX_ORDER);
        );

   vs1 = VSattach(fid, -1, "w");
    name = "Max_Fldsize Vdata";
    VSsetname(vs1, name);
    max_order = MAX_FIELD_SIZE/SIZE_FLOAT32;
    VSfdefine(vs1, "max_fldsize", DFNT_FLOAT32, max_order);
    status = VSsetfields(vs1, "max_fldsize");
    if (status == FAIL)
      {
          num_errs++;
          printf(">>> Vsetfields failed for %s\n", name);
      }
    
    /* create some bogus data */
    for (i = 0; i < max_order; i++)
         gbuf2[i] = (float32)i * (float32)0.11; 
    VSwrite(vs1, (unsigned char *) gbuf2, 1, FULL_INTERLACE);
    VSdetach(vs1);
    MESSAGE(5, printf("created VDATA %s with %d order\n", name, (int)
 max_order);
        );

    /* create vdata exceeding MAX_FIELD_SIZE, should fail */
    vs1 = VSattach(fid, -1, "w");
    name = "Bad_Fldsize Vdata";
    VSsetname(vs1, name);
    max_order = MAX_FIELD_SIZE/SIZE_FLOAT32 + 1;
    status = VSfdefine(vs1, "bad_fldsize", DFNT_FLOAT32, max_order);
    if (status != FAIL)
      {
          num_errs++;
          printf(">>> Vsetfields failed for %s\n", name);
      }

    status = VSsetfields(vs1, "bad_fldsize");
    if (status != FAIL)
      {
          num_errs++;
          printf(">>> Vsetfields failed for %s\n", name);
      }
    VSdetach(vs1);
    /* create a whole bunch of Vdatas to check for memory leakage */
    for (i = 0; i < VDATA_COUNT; i++)
      {
          char        name2[80];
          vs1 = VSattach(fid, -1, "w");
          if (vs1 == FAIL)
            {
                num_errs++;
                printf(">>> Vsattach failed on loop %d\n", (int) i);
                continue;
            }
          sprintf(name2, "VdataLoop-%d", (int) i);
          VSsetname(vs1, name2);
          status = VSfdefine(vs1, "A", DFNT_CHAR8, 1);
          if (status == FAIL)
            {
                num_errs++;
                printf(">>> VSfdefine failed on loop %d\n", (int) i);
                continue;
            }
          status = VSsetfields(vs1, "A");
          if (status == FAIL)
            {
                num_errs++;
                printf(">>> VSsetfields failed on loop %d\n", (int) i);
                continue;
            }
          VSwrite(vs1, (unsigned char *) name2, 1, FULL_INTERLACE);
          VSdetach(vs1);
      }

    Vend(fid);
    Hclose(fid);
    HDfree(gbuf1);
    HDfree(gbuf2);
    return SUCCEED;

}   /* write_vset_stuff */

/* read everything back in and check it */
static int32
read_vset_stuff(void)
{
    int32       ret;            /* generic return value */
    int32       ibuf[2000];     /* integer buffer */
    float32     fbuf[2000];     /* floating point buffer */
    char        gbuf[2000];     /* generic buffer */
    int32       list[50];
    int32       tags[100], refs[100], tag, ref;
    char        name[512], class[512], fields[512];
    char       *p;
    int32       fid;
    int32       vg1;
    int32       vs1;
    int32       status, num, i, count, intr, sz;
    float32     fl_expected;
    int32       in_expected;
    char8       c_expected;

    fid = Hopen(FNAME0, DFACC_RDONLY, 0);
    if (fid == FAIL)
      {
          num_errs++;
          return FAIL;
      }

    Vstart(fid);

    /*

     *   Verify the Vgroups
     *
     */

    /* test Vlone */
    num = 1;
    status = Vlone(fid, list, 10);
    if (status != num)
      {
          num_errs++;
          printf(">>> Vlone found %d was expecting %d\n", (int) status, (int) num);
      }

    /* test Vgetname and Vgetclass */
    vg1 = Vattach(fid, list[0], "r");
    if (vg1 == FAIL)
      {
          num_errs++;
          printf(">>> Was not able to attach (r) Vgroup %d\n", (int) list[0]);
      }

    Vgetname(vg1, name);
    Vgetclass(vg1, class);

    if (HDstrcmp(name, "Second Vgroup"))
      {
          num_errs++;
          printf(">>> Got bogus Vgroup name : %s\n", name);
      }

    if (HDstrcmp(class, "Test object"))
      {
          num_errs++;
          printf(">>> Got bogus Vgroup class : %s\n", class);
      }

    num = 3;
    status = Vgettagrefs(vg1, tags, refs, 100);
    if (status == FAIL)
      {
          num_errs++;
          printf(">>> Vgettagrefs found %d was expecting %d\n", (int) status, (int) num);
      }

    for (i = 0; i < num; i++)
      {
          status = Vgettagref(vg1, i, &tag, &ref);
          if (status == FAIL)
            {
                num_errs++;
                printf(">>> Vgettagref failed on call %d\n", (int) i);
            }

          if (tag != tags[i])
            {
                num_errs++;
                printf(">>> Vgettagref Tag #%d disagrees %d %d\n", (int) i, (int) tag, (int) tags[i]);
            }

          if (ref != refs[i])
            {
                num_errs++;
                printf(">>> Vgettagref Ref #%d disagrees %d %d\n", (int) i, (int) ref, (int) refs[i]);
            }

      }
    ret = Vdetach(vg1);
    RESULT("Vdetach");

    /* test Vgetid */
    ref = Vgetid(fid, -1);
    if (ref == FAIL)
      {
          num_errs++;
          printf(">>> Vgetid was unable to find first Vgroup\n");
      }

    ref = Vgetid(fid, ref);
    if (ref != list[0])
      {
          num_errs++;
          printf(">>> Vgetid was unable to find second Vgroup (should have been first lone one)\n");
      }

    /*

     *   Verify the Vdatas
     *
     */

    /* test VSgetid */
    ref = VSgetid(fid, -1);
    if (ref == FAIL)
      {
          num_errs++;
          printf(">>> VSgetid was unable to find first Vdata\n");
      }

    /* read in the first data and verify metadata and contents */
    vs1 = VSattach(fid, ref, "r");

    VSgetname(vs1, name);
    VSgetclass(vs1, class);

    if (HDstrcmp(name, "Float Vdata"))
      {
          num_errs++;
          printf(">>> Got bogus Vdata name (VSgetname) : %s\n", name);
      }

    if (HDstrcmp(class, "Test object"))
      {
          num_errs++;
          printf(">>> Got bogus Vdata class : %s\n", class);
      }

    status = VSinquire(vs1, &count, &intr, fields, &sz, name);
    if (status == FAIL)
      {
          num_errs++;
          printf(">>> VSinquire failed on float Vdata\n");
      }

    if (HDstrcmp(name, "Float Vdata"))
      {
          num_errs++;
          printf(">>> Got bogus Float Vdata name (VSinquire) : %s\n", name);
      }

    if (count != 100)
      {
          num_errs++;
          printf(">>> Got wrong count %d expecting 100\n", (int) count);
      }

    if ((size_t)sz != sizeof(float32))
      {
          num_errs++;
          printf(">>> Got wrong data size %d should be sizeof(float32)\n", (int) sz);
      }

#ifndef VDATA_FIELDS_ALL_UPPER
    if (HDstrcmp(fields, FIELD1))
      {
          num_errs++;
          printf(">>> Got bogus field name %s\n", fields);
      }
#else
    if (HDstrcmp(fields, FIELD1_UPPER))
      {
          num_errs++;
          printf(">>> Got bogus field name %s\n", fields);
      }
#endif /* VDATA_FIELDS_ALL_UPPER */

    /* read it */
    VSsetfields(vs1, fields);
    for (i = 0; i < count; i++)
        fbuf[i] = (float32)0.0;
    VSread(vs1, (unsigned char *) fbuf, count, FULL_INTERLACE);

    /* verify */
    for (i = 0; i < count; i++)
      {
          if (fbuf[i] != (float32) i)
            {
                num_errs++;
                printf(">>> Float value %d was expecting %d got %f\n", (int) i, (int) i, fbuf[i]);
            }
      }

    ret=VSdetach(vs1);
    RESULT("VSdetach");

    /* Move to the next one (integers) */
    ref = VSgetid(fid, ref);
    if (ref == FAIL)
      {
          num_errs++;
          printf(">>> VSgetid was unable to find second Vdata\n");
      }

    /* read in the first data and verify metadata and contents */
    vs1 = VSattach(fid, ref, "r");

    VSgetname(vs1, name);
    VSgetclass(vs1, class);

    if (HDstrcmp(name, "Integer Vdata"))
      {
          num_errs++;
          printf(">>> Got bogus Vdata name (VSgetname) : %s\n", name);
      }

    if (HDstrcmp(class, "Test object"))
      {
          num_errs++;
          printf(">>> Got bogus Vdata class : %s\n", class);
      }

    status = VSinquire(vs1, &count, &intr, fields, &sz, name);
    if (status == FAIL)
      {
          num_errs++;
          printf(">>> VSinquire failed on float Vdata\n");
      }

    if (HDstrcmp(name, "Integer Vdata"))
      {
          num_errs++;
          printf(">>> Got bogus Integer Vdata name (VSinquire) : %s\n", name);
      }

    if (count != 100)
      {
          num_errs++;
          printf(">>> Got wrong count %d expecting 100\n", (int) count);
      }

    if ((size_t)sz != 2 * sizeof(int32))
      {
          num_errs++;
          printf(">>> Got wrong data size %d should be 2 * sizeof(int32)\n", (int) sz);
      }

    if (HDstrcmp(fields, FIELD2))
      {
          num_errs++;
          printf(">>> Got bogus field name %s\n", fields);
      }

    /* read it */
    VSsetfields(vs1, fields);
    for (i = 0; i < 2 * count; i++)
        ibuf[i] = 0;
    VSread(vs1, (unsigned char *) ibuf, count, FULL_INTERLACE);

    /* verify */
    for (i = 0; i < 2 * count; i++)
      {
          if (ibuf[i] != i)
            {
                num_errs++;
                printf(">>> Int value %d was expecting %d got %d\n", (int) i, (int) i, (int) ibuf[i]);
            }
      }

    ret=VSdetach(vs1);
    RESULT("VSdetach");

#ifndef HAVE_FMPOOL 
/* Commented out this test when using the file caching.This is beacause this 
   test opens the external file directly without using HDF calls. As a result
   the file memory pool buffer that was created for this external file will 
   not be shared with this low-level call as the low-level file cache open 
   creates a unique pool for every call. It is upto the programmer
   then to share the file pool. -GeorgeV
 */

    /* testing VSsetexternalfile by reading the external file directly */
    {   hdf_file_t fd;
        int j;
	int32 ival;


	fd = HI_OPEN(EXTFNM, DFACC_RDONLY);
	if (OPENERR(fd)){
	    num_errs++;
	    printf(">>> Reopen External file %s failed\n", EXTFNM);
	}
	else{
	    status = HI_READ(fd, gbuf, (2*count*DFKNTsize(DFNT_INT32)));
	    if (status == FAIL){
		num_errs++;
		printf(">>> Reading External file data failed\n");
	    }
	    else{

		j = 0;
		for (i = 0; i < 2 * count; i++)
		{
		    ival = 0xff & gbuf[j++];
		    ival = ival<<8 | (0xff & gbuf[j++]);
		    ival = ival<<8 | (0xff & gbuf[j++]);
		    ival = ival<<8 | (0xff & gbuf[j++]);

		    if (ival != i)
		    {
			num_errs++;
			printf(">>> External value %d was expecting %d got %d\n",
			    (int) i, (int) i, (int) ival);
		    }
		}
	    }
	    HI_CLOSE(fd);
	}
    }
#endif /* HAVE_FMPOOL */

    /* Move to the next one (integers + floats) */
    ref = VSgetid(fid, ref);
    if (ref == FAIL)
      {
          num_errs++;
          printf(">>> VSgetid was unable to find third Vdata\n");
      }

    /* read in the first data and verify metadata and contents */
    vs1 = VSattach(fid, ref, "r");

    VSgetname(vs1, name);
    VSgetclass(vs1, class);

    if (HDstrcmp(name, "Mixed Vdata"))
      {
          num_errs++;
          printf(">>> Got bogus Vdata name (VSgetname) : %s\n", name);
      }

    if (HDstrcmp(class, "No class specified"))
      {
          num_errs++;
          printf(">>> Got bogus Vdata class : %s\n", class);
      }

    status = VSinquire(vs1, &count, &intr, fields, &sz, name);
    if (status == FAIL)
      {
          num_errs++;
          printf(">>> VSinquire failed on float Vdata\n");
      }

    if (HDstrcmp(name, "Mixed Vdata"))
      {
          num_errs++;
          printf(">>> Got bogus Mixed Vdata name (VSinquire) : %s\n", name);
      }

    if (count != 100)
      {
          num_errs++;
          printf(">>> Got wrong count %d expecting 100\n", (int) count);
      }

    if ((size_t)sz != sizeof(int32) + sizeof(float32))
      {
          num_errs++;
          printf(">>> Got wrong data size %d should be sizeof(int32) + sizeof(float32)\n", (int) sz);
      }

    if (HDstrcmp(fields, "A,B"))
      {
          num_errs++;
          printf(">>> Got bogus field name %s\n", fields);
      }

    /* read it */
    VSsetfields(vs1, fields);
    for (i = 0; i < 1000; i++)
        gbuf[i] = 0;
    VSread(vs1, (unsigned char *) gbuf, count, FULL_INTERLACE);

    /* verify */
    p = gbuf;
    for (i = 0; i < count; i++)
      {
          float32     fl=(float32)0.0;
          int32       in=(int32)0;

          HDmemcpy(&fl, p, sizeof(float32));
          p += sizeof(float32);
          HDmemcpy(&in, p, sizeof(int32));
          p += sizeof(int32);

          if (in != i)
            {
                num_errs++;
                printf(">>> Mixed int value %d was expecting %d got %d\n", (int) i, (int) i, (int) in);
            }

          if (fl != (float32) (i * 2))
            {
                num_errs++;
                printf(">>> Mixed float value %d was expecting %d got %f\n", (int) i, (int) i, fl);
            }
      }

    ret=VSdetach(vs1);
    RESULT("VSdetach");

    /* Move to the next one (multi-order) */
    ref = VSgetid(fid, ref);
    if (ref == FAIL)
      {
          num_errs++;
          printf(">>> VSgetid was unable to find multi-order Vdata\n");
      }

    /* read in the first data and verify metadata and contents */
    vs1 = VSattach(fid, ref, "r");

    VSgetname(vs1, name);
    VSgetclass(vs1, class);

    if (HDstrcmp(name, "Multi-Order Vdata"))
      {
          num_errs++;
          printf(">>> Got bogus Vdata name (VSgetname) : %s\n", name);
      }

    if (HDstrcmp(class, "No class specified"))
      {
          num_errs++;
          printf(">>> Got bogus Vdata class : %s\n", class);
      }

    status = VSinquire(vs1, &count, &intr, fields, &sz, name);
    if (status == FAIL)
      {
          num_errs++;
          printf(">>> VSinquire failed on multi-order Vdata\n");
      }

    if (count != 10)
      {
          num_errs++;
          printf(">>> Got wrong count %d expecting 10\n", (int) count);
      }

    if (HDstrcmp(fields, MX))
      {
          num_errs++;
          printf(">>> Got bogus field name %s\n", fields);
      }

    /*
     * verify - read in all fields
     */

    /* read it */
    VSsetfields(vs1, fields);
    for (i = 0; i < 1000; i++)
        gbuf[i] = 0;
    VSread(vs1, (unsigned char *) gbuf, count, FULL_INTERLACE);

    p = gbuf;
    fl_expected = (float32) 15.5;
    in_expected = 0;
    c_expected = 'a';

    for (i = 0; i < count; i++)
      {
          float32     fl=(float32)0.0;
          int32       in=(int32)0;
          char8       c=(char8)0;

          /* read and verify characters */
          HDmemcpy(&c, p, sizeof(char8));
          p += sizeof(char8);

          if (c != c_expected)
            {
                num_errs++;
                printf(">>> Multi-order char value %d.0 was expecting %c got %c\n", (int) i, c_expected, c);
            }
          c_expected++;

          HDmemcpy(&c, p, sizeof(char8));
          p += sizeof(char8);

          if (c != c_expected)
            {
                num_errs++;
                printf(">>> Multi-order char value %d.1 was expecting %c got %c\n", (int) i, c_expected, c);
            }
          c_expected++;

          /* read and verify integers */
          HDmemcpy(&in, p, sizeof(int32));
          p += sizeof(int32);

          if (in != in_expected)
            {
                num_errs++;
                printf(">>> Multi-order int value %d.0 was expecting %d got %d\n", (int) i, (int) in_expected, (int) in);
            }
          in_expected++;
          HDmemcpy(&in, p, sizeof(int32));
          p += sizeof(int32);

          if (in != in_expected)
            {
                num_errs++;
                printf(">>> Multi-order int value %d.1 was expecting %d got %d\n", (int) i, (int) in_expected, (int) in);
            }
          in_expected++;
          HDmemcpy(&in, p, sizeof(int32));
          p += sizeof(int32);

          if (in != in_expected)
            {
                num_errs++;
                printf(">>> Multi-order int value %d.2 was expecting %d got %d\n", (int) i, (int) in_expected, (int) in);
            }
          in_expected++;

          /* read and verify floating point value */
          HDmemcpy(&fl, p, sizeof(float32));
          p += sizeof(float32);

          if (fl != fl_expected)
            {
                num_errs++;
                printf(">>> Multi-order float value %d was expecting %f got %f\n", (int) i, fl_expected, fl);
            }
          fl_expected += (float32) 0.5;

      }

    /*
     * verify - just read in the character field with FULL_INTERLACE
     */

    /* read it */
    VSseek(vs1, 0);
    VSsetfields(vs1, ST);
    for (i = 0; i < 1000; i++)
        gbuf[i] = 0;
    VSread(vs1, (unsigned char *) gbuf, count, FULL_INTERLACE);

    p = gbuf;
    c_expected = 'a';

    for (i = 0; i < count; i++)
      {
          char8       c='\0';

          /* read and verify characters */
          HDmemcpy(&c, p, sizeof(char8));
          p += sizeof(char8);

          if (c != c_expected)
            {
                num_errs++;
                printf(">>> FULL_INTERLACE read char value %d.0 (%c) got %c %d\n", (int) i, c_expected, c, c);
            }
          c_expected++;

          HDmemcpy(&c, p, sizeof(char8));
          p += sizeof(char8);

          if (c != c_expected)
            {
                num_errs++;
                printf(">>> FULL_INTERLACE read char value %d.1 (%c) %c got %c\n", (int) i, c_expected, c, c);
            }
          c_expected++;

      }

    /*
     * verify - just read in the character field with NO_INTERLACE
     */

    /* read it */
    VSseek(vs1, 0);
    VSsetfields(vs1, ST);
    for (i = 0; i < 1000; i++)
        gbuf[i] = 0;
    VSread(vs1, (unsigned char *) gbuf, count, NO_INTERLACE);

    p = gbuf;
    c_expected = 'a';

    for (i = 0; i < count; i++)
      {
          char8       c='\0';

          /* read and verify characters */
          HDmemcpy(&c, p, sizeof(char8));
          p += sizeof(char8);

          if (c != c_expected)
            {
                num_errs++;
                printf(">>> NO_INTERLACE read char value %d.0 (%c) got %c\n", (int) i, c_expected, c);
            }
          c_expected++;

          HDmemcpy(&c, p, sizeof(char8));
          p += sizeof(char8);

          if (c != c_expected)
            {
                num_errs++;
                printf(">>> NO_INTERLACE read char value %d.1 (%c) %c got\n", (int) i, c_expected, c);
            }
          c_expected++;

      }

    /* verify that VSfind does not mess up the AIDs of attached Vdatas */
    VSfind(fid, "foo");
    if (VSseek(vs1, 0) == FAIL)
      {
          num_errs++;
          printf(">>> VSseek failed after VSfind call\n");
      }

    ret=VSdetach(vs1);
    RESULT("VSdetach");

    ret=Vend(fid);
    RESULT("Vend");

    Hclose(fid);
    return SUCCEED;

}   /* read_vset_stuff */

/* main test driver */
void
test_vsets(void)
{
    int32       status;

    status = write_vset_stuff();
    if (status == FAIL)
        return;

    status = read_vset_stuff();
    if (status == FAIL)
        return;

}   /* test_vsets */
