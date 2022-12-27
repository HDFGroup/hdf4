/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Copyright by The HDF Group.                                               *
 * Copyright by the Board of Trustees of the University of Illinois.         *
 * All rights reserved.                                                      *
 *                                                                           *
 * This file is part of HDF.  The full HDF copyright notice, including       *
 * terms governing use, modification, and redistribution, is contained in    *
 * the COPYING file, which can be found at the root of the source code       *
 * distribution tree, or in https://support.hdfgroup.org/ftp/HDF/releases/.  *
 * If you do not have access to either file, you may request a copy from     *
 * help@hdfgroup.org.                                                        *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/* $Id$ */

/*
 *
 * Vset tests 
 *
 *
 * This file needs another pass at making sure all the return
 * values from function calls are checked in addtion to
 * verifying that the proper tests are performed on all Vxx fcns - GV 9/5/97
 *
 */
#include "hdf.h"
#include "hfile.h"
#include "tproto.h"

#define VDATA_COUNT  256   /* make this many Vdatas to check for memory leaks */

#define FNAME0		"tvset.hdf"
#define EXTFNM		"tvsetext.hdf"
#define EMPTYNM		"tvsempty.hdf"
#define LONGNAMES	"tlongnames.hdf"
#define LKBLK_FILE	"tvsblkinfo.hdf"

#define FIELD1		"FIELD_name_HERE"
#define FIELD1_UPPER	"FIELD_NAME_HERE"
#define FIELD2		"DIFFERENT_FIELD_NAME"

#define FIELD1_NAME	"Field1"	/* contains three integers */
#define FIELD2_NAME	"Field2"	/* contains one integer */
#define FIELD3_NAME	"Field3"	/* contains two integers */
#define	FIELD_NAME_LIST	"Field1,Field2,Field3"
#define ORDER_1 	3	/* order of first field of 1st vdata */
#define ORDER_2 	1	/* order of second field of 1st vdata */
#define ORDER_3 	2	/* order of third field of 1st vdata */
#define ST		"STATION_NAME"
#define VL		"VALUES"
#define FL		"FLOATS"
#define MX		"STATION_NAME,VALUES,FLOATS"
#define EMPTY_VDATA	"Empty"
#define VGROUP1		"VGROUP1"
#define VG_LONGNAME "Vgroup with more than 64 characters in length, 74 characters to be exact!"
#define VG_LONGCLASS "Very long class name to classify all Vgroups with more than 64 characters in name"
#define APPENDABLE_VDATA "Appendable"

static int32 write_vset_stuff(void);
static int32 read_vset_stuff(void);
static void test_vsdelete(void);
static void test_vdelete(void);
static void test_vdeletetagref(void);
static void test_emptyvdata(void);
static void test_vglongnames(void);
static void test_getvgroups(void);
static void test_getvdatas(void);
static void test_blockinfo_oneLB(void);
static void test_blockinfo_multLBs(void);
static void test_VSofclass(void);

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
    if(vg1 == FAIL)
      {
          num_errs++;
          printf(">>> Failed creating initial Vgroup\n");
      }

    status = Vsetname(vg1, "Simple Vgroup");
    CHECK(status,FAIL,"Vsetname:vg1");

    status = Vsetclass(vg1, "Test object");
    CHECK(status,FAIL,"Vsetclass:vg1");

    MESSAGE(5, printf("created Vgroup %s (empty)\n", "Simple Vgroup"););

    /*
     * Lets do some more complex ones now
     */
    vg2 = Vattach(fid, -1, "w");
    if(vg2 == FAIL)
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

    status = Hendaccess(aid);
    CHECK(status,FAIL,"Hendaccess:aid");

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

    status = Vsetname(vg2, "Second Vgroup");
    CHECK(status,FAIL,"Vsetname:for vg2");

    Vsetclass(vg2, "Test object");
    CHECK(status,FAIL,"Vsetclass: for vg2");

    status = Vdetach(vg1);
    CHECK(status,FAIL,"Vdetach:vg1");

    status = Vdetach(vg2);
    CHECK(status,FAIL,"Vdetach:vg2");

    MESSAGE(5, printf("created Vgroup %s with %d elements\n", "Second Vgroup", 
                      (int) num););

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

    /* Verify that VSsetfields will return FAIL when passing in a NULL
       for field name list (bug #554) - BMR 5/17/01 */
    status = VSsetfields(vs1, NULL);
    VERIFY(status, FAIL, "VSsetfields");

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
    status = VSwrite(vs1, (unsigned char *) fbuf, count, FULL_INTERLACE);
    CHECK(status,FAIL,"VSwrite:vs1");

    /* Test VSgetexternalfile on a vdata without external element */
    /*  status = VSgetexternalfile(vs1, 0, NULL, NULL);
    VERIFY_VOID(status, FAIL, "VSgetexternalfile");
 */ 

    /* Test VSgetexternalinfo on a vdata without external element */
    status = VSgetexternalinfo(vs1, 0, NULL, NULL, NULL);
    VERIFY(status, 0, "VSgetexternalinfo");

    status = VSdetach(vs1);
    CHECK(status,FAIL,"VSdetach:vs1");
    
    MESSAGE(5, printf("created VDATA %s with %d elements\n", name, (int) count););

    /* Int32 Vdata */
    vs1 = VSattach(fid, -1, "w");
    CHECK(vs1,FAIL,"VSattach:vs1");

    name = "Integer Vdata";
    status = VSsetname(vs1, name);
    CHECK(status,FAIL,"VSsetname:vs1");

    status = VSsetclass(vs1, "Test object");
    CHECK(status,FAIL,"VSsetclass:vs1");

    status = VSfdefine(vs1, FIELD2, DFNT_INT32, 2);
    CHECK(status,FAIL,"VSfdefine:vs1");

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
    status = VSwrite(vs1, (unsigned char *) ibuf, count, FULL_INTERLACE);
    CHECK(status,FAIL,"VSwrite:vs1");

    status = VSdetach(vs1);
    CHECK(status,FAIL,"VSdetach:vs1");

    MESSAGE(5, printf("created VDATA %s with %d elements\n", 
                      name, (int) count); );

    /* Int32 and Float32 Vdata */
    vs1 = VSattach(fid, -1, "w");
    CHECK(vs1,FAIL,"VSattach:vs1");

    name = "Mixed Vdata";
    status = VSsetname(vs1, name);
    CHECK(status,FAIL,"VSsetname:vs1");

    status = VSsetclass(vs1, "No class specified");
    CHECK(status,FAIL,"VSsetclass:vs1");

    status = VSfdefine(vs1, "A", DFNT_FLOAT32, 1);
    CHECK(status,FAIL,"VSfdefine:vs1");

    status = VSfdefine(vs1, "B", DFNT_INT32, 1);
    CHECK(status,FAIL,"VSfdefine:vs1");

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
    status = VSwrite(vs1, (unsigned char *) gbuf, count, FULL_INTERLACE);
    CHECK(status,FAIL,"VSwrite:vs1");

    status = VSdetach(vs1);
    CHECK(status,FAIL,"VSdetach:vs1");

    MESSAGE(5, printf("created VDATA %s with %d elements\n", 
                      name, (int) count););

    /* mixed order Vdata */
    vs1 = VSattach(fid, -1, "w");
    CHECK(vs1,FAIL,"VSattach:vs1");

    name = "Multi-Order Vdata";
    status = VSsetname(vs1, name);
    CHECK(status,FAIL,"VSsetname:vs1");

    status = VSsetclass(vs1, "No class specified");
    CHECK(status,FAIL,"VSsetclass:vs1");

    status = VSfdefine(vs1, ST, DFNT_CHAR8, 2);
    CHECK(status,FAIL,"VSfdefine:vs1");

    status = VSfdefine(vs1, VL, DFNT_INT32, 3);
    CHECK(status,FAIL,"VSfdefine:vs1");

    status = VSfdefine(vs1, FL, DFNT_FLOAT32, 1);
    CHECK(status,FAIL,"VSfdefine:vs1");

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
    status = VSwrite(vs1, (unsigned char *) gbuf, count, FULL_INTERLACE);
    CHECK(status,FAIL,"VSwrite:vs1");

    status = VSdetach(vs1);
    CHECK(status,FAIL,"VSdetach:vs1");

    MESSAGE(5, printf("created VDATA %s with %d elements\n", 
                      name, (int) count););

    /* test MAX_ORDER and MAX_FIELD_SIZE */
    vs1 = VSattach(fid, -1, "w");
    CHECK(vs1,FAIL,"VSattach:vs1");

    name = "Max_Order Vdata";
    status = VSsetname(vs1, name);
    CHECK(status,FAIL,"VSsetname:vs1");

    status = VSfdefine(vs1, "max_order", DFNT_UINT8, MAX_ORDER);
    CHECK(status,FAIL,"VSfdefine:vs1");

    status = VSsetfields(vs1, "max_order");
    if (status == FAIL)
      {
          num_errs++;
          printf(">>> Vsetfields failed for %s\n", name);
      }

    /* create some bogus data */
    for (i = 0; i < MAX_ORDER; i++)
         gbuf1[i] = (uint8)(i % 256);

    status = VSwrite(vs1, (unsigned char *) gbuf1, 1, FULL_INTERLACE);
    CHECK(status,FAIL,"VSwrite:vs1");

    status = VSdetach(vs1);
    CHECK(status,FAIL,"VSdetach:vs1");

    MESSAGE(5, printf("created VDATA %s with %d order\n", 
                      name, (int)MAX_ORDER););

    vs1 = VSattach(fid, -1, "w");
    CHECK(vs1,FAIL,"VSattach:vs1");

    name = "Max_Fldsize Vdata";
    status = VSsetname(vs1, name);
    CHECK(status,FAIL,"VSsetname:vs1");

    max_order = MAX_FIELD_SIZE/SIZE_FLOAT32;
    status = VSfdefine(vs1, "max_fldsize", DFNT_FLOAT32, max_order);
    CHECK(status,FAIL,"VSfdefine:vs1");

    status = VSsetfields(vs1, "max_fldsize");
    if (status == FAIL)
      {
          num_errs++;
          printf(">>> Vsetfields failed for %s\n", name);
      }
    
    /* create some bogus data */
    for (i = 0; i < max_order; i++)
         gbuf2[i] = (float32)i * (float32)0.11; 

    status = VSwrite(vs1, (unsigned char *) gbuf2, 1, FULL_INTERLACE);
    CHECK(status,FAIL,"VSwrite:vs1");

    status = VSdetach(vs1);
    CHECK(status,FAIL,"VSdetach:vs1");

    MESSAGE(5, printf("created VDATA %s with %d order\n", 
                      name, (int)max_order););

    /* create vdata exceeding MAX_FIELD_SIZE, should fail */
    vs1 = VSattach(fid, -1, "w");
    CHECK(vs1,FAIL,"VSattach:vs1");

    name = "Bad_Fldsize Vdata";
    status = VSsetname(vs1, name);
    CHECK(status,FAIL,"VSsetname:vs1");

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

    status = VSdetach(vs1);
    CHECK(status,FAIL,"VSdetach:vs1");

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
          status = VSsetname(vs1, name2);
          CHECK(status,FAIL,"VSsetname:vs1");

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
          status = VSwrite(vs1, (unsigned char *) name2, 1, FULL_INTERLACE);
          CHECK(status,FAIL,"VSwrite:vs1");

          status = VSdetach(vs1);
          CHECK(status,FAIL,"VSdetach:vs1");
      }

    status = Vend(fid);
    CHECK(status,FAIL,"Vend:fid");

    status = Hclose(fid);
    CHECK(status,FAIL,"Hclose:vs1");

    HDfree(gbuf1);
    HDfree(gbuf2);

    return SUCCEED;

}   /* write_vset_stuff */

/* read everything back in and check it */
static int32
read_vset_stuff(void)
{
    int32       ibuf[2000];     /* integer buffer */
    float32     fbuf[2000];     /* floating point buffer */
    char        gbuf[2000];     /* generic buffer */
    int32       list[50];
    int32       tags[100], refs[100], tag, ref;
    char        vsname[512], vsclass[512], fields[512];
    char       *vgname, *vgclass;
    char       *p;
    int32       fid;
    int32       vg1;
    int32       vs1;
    int32       status, num, i, count, intr, sz;
    float32     fl_expected;
    int32       in_expected;
    char8       c_expected;
    uint16	name_len;

    fid = Hopen(FNAME0, DFACC_RDONLY, 0);
    if (fid == FAIL)
      {
          num_errs++;
          return FAIL;
      }

    status = Vstart(fid);
    CHECK(status,FAIL,"Vstart:fid");

    /*
     *   Verify the Vgroups
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
    if(vg1 == FAIL)
      {
          num_errs++;
          printf(">>> Was not able to attach (r) Vgroup %d\n", (int) list[0]);
      }

    status = Vgetnamelen(vg1, &name_len);
    CHECK(status,FAIL,"Vgetnamelen:vg1");

    vgname = (char *) HDmalloc(sizeof(char *) * (name_len+1));
    CHECK_ALLOC(vgname, "vgname", "read_vset_stuff");
    
    status = Vgetname(vg1, vgname);
    CHECK(status,FAIL,"Vgetname:vg1");

    status = Vgetclassnamelen(vg1, &name_len);
    CHECK(status,FAIL,"Vgetclassnamelen:vg1");

    vgclass = (char *) HDmalloc(sizeof(char *) * (name_len+1));
    CHECK_ALLOC(vgclass, "vgclass", "read_vset_stuff");
    
    status = Vgetclass(vg1, vgclass);
    CHECK(status,FAIL,"Vgetclass:vg1");

    if (HDstrcmp(vgname, "Second Vgroup")) {
        num_errs++;
        printf(">>> Got bogus Vgroup name : %s\n", vgname);
    }

    if (vgname != NULL)
        HDfree(vgname);

    if (HDstrcmp(vgclass, "Test object")) {
        num_errs++;
        printf(">>> Got bogus Vgroup class : %s\n", vgclass);
    }

    if (vgclass != NULL)
        HDfree(vgclass);

    num = 3;
    status = Vgettagrefs(vg1, tags, refs, 100);
    if (status == FAIL) {
        num_errs++;
        printf(">>> Vgettagrefs found %d was expecting %d\n", (int) status,
                (int) num);
    }

    for (i = 0; i < num; i++) {
        status = Vgettagref(vg1, i, &tag, &ref);
        if (status == FAIL) {
            num_errs++;
            printf(">>> Vgettagref failed on call %d\n", (int) i);
        }

        if (tag != tags[i]) {
            num_errs++;
            printf(">>> Vgettagref Tag #%d disagrees %d %d\n", (int) i,
                    (int) tag, (int) tags[i]);
        }

        if (ref != refs[i]) {
            num_errs++;
            printf(">>> Vgettagref Ref #%d disagrees %d %d\n", (int) i,
                    (int) ref, (int) refs[i]);
        }

    }

    status = Vdetach(vg1);
    CHECK(status,FAIL,"Vdetach:vg1");

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
    CHECK(vs1,FAIL,"VSattach:vs1");

    status = VSgetname(vs1, vsname);
    CHECK(status,FAIL,"VSgetname:vs1");

    status = VSgetclass(vs1, vsclass);
    CHECK(status,FAIL,"VSgetclass:vs1");

    if (HDstrcmp(vsname, "Float Vdata"))
      {
          num_errs++;
          printf(">>> Got bogus Vdata name (VSgetname) : %s\n", vsname);
      }

    if (HDstrcmp(vsclass, "Test object"))
      {
          num_errs++;
          printf(">>> Got bogus Vdata class : %s\n", vsclass);
      }

    status = VSinquire(vs1, &count, &intr, fields, &sz, vsname);
    if (status == FAIL)
      {
          num_errs++;
          printf(">>> VSinquire failed on float Vdata\n");
      }

    if (HDstrcmp(vsname, "Float Vdata"))
      {
          num_errs++;
          printf(">>> Got bogus Float Vdata name (VSinquire) : %s\n", vsname);
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
    status = VSsetfields(vs1, fields);
    CHECK(status,FAIL,"VSsetfields:vs1");

    for (i = 0; i < count; i++)
        fbuf[i] = (float32)0.0;

    status = VSread(vs1, (unsigned char *) fbuf, count, FULL_INTERLACE);
    CHECK(status,FAIL,"VSread:vs1");

    /* verify */
    for (i = 0; i < count; i++)
      {
          if (fbuf[i] != (float32) i)
            {
                num_errs++;
                printf(">>> Float value %d was expecting %d got %f\n", (int) i, (int) i, fbuf[i]);
            }
      }

    status = VSdetach(vs1);
    CHECK(status,FAIL,"VSdetach:vs1");

    /* Move to the next one (integers) */
    ref = VSgetid(fid, ref);
    if (ref == FAIL)
      {
          num_errs++;
          printf(">>> VSgetid was unable to find second Vdata\n");
      }

    /* read in the first data and verify metadata and contents */
    vs1 = VSattach(fid, ref, "r");
    CHECK(vs1,FAIL,"VSattach:vs1");

    status = VSgetname(vs1, vsname);
    CHECK(status,FAIL,"VSgetname:vs1");

    status = VSgetclass(vs1, vsclass);
    CHECK(status,FAIL,"VSgetclass:vs1");

    if (HDstrcmp(vsname, "Integer Vdata"))
      {
          num_errs++;
          printf(">>> Got bogus Vdata name (VSgetname) : %s\n", vsname);
      }

    if (HDstrcmp(vsclass, "Test object"))
      {
          num_errs++;
          printf(">>> Got bogus Vdata class : %s\n", vsclass);
      }

    status = VSinquire(vs1, &count, &intr, fields, &sz, vsname);
    if (status == FAIL)
      {
          num_errs++;
          printf(">>> VSinquire failed on float Vdata\n");
      }

    if (HDstrcmp(vsname, "Integer Vdata"))
      {
          num_errs++;
          printf(">>> Got bogus Integer Vdata name (VSinquire) : %s\n", vsname);
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
    status = VSsetfields(vs1, fields);
    CHECK(status,FAIL,"VSsetfields:vs1");

    for (i = 0; i < 2 * count; i++)
        ibuf[i] = 0;

    status = VSread(vs1, (unsigned char *) ibuf, count, FULL_INTERLACE);
    CHECK(status,FAIL,"VSread:vs1");

    /* verify */
    for (i = 0; i < 2 * count; i++)
      {
          if (ibuf[i] != i)
            {
                num_errs++;
                printf(">>> Int value %d was expecting %d got %d\n", (int) i, (int) i, (int) ibuf[i]);
            }
      }

    status = VSdetach(vs1);
    CHECK(status,FAIL,"VSdetach:vs1");

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

        /* low level open of external file */
        fd = HI_OPEN(EXTFNM, DFACC_RDONLY);
        if (OPENERR(fd))
          {
              num_errs++;
              printf(">>> Reopen External file %s failed\n", EXTFNM);
          }
        else
          {
              status = HI_READ(fd, gbuf, (2*count*DFKNTsize(DFNT_INT32)));
              if (status == FAIL)
                {
                    num_errs++;
                    printf(">>> Reading External file data failed\n");
                }
              else
                {

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
              /* low level close of external file */
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
    CHECK(vs1,FAIL,"VSattach:vs1");

    status = VSgetname(vs1, vsname);
    CHECK(status,FAIL,"VSgetname:vs1");

    status = VSgetclass(vs1, vsclass);
    CHECK(status,FAIL,"VSgetclass:vs1");

    if (HDstrcmp(vsname, "Mixed Vdata"))
      {
          num_errs++;
          printf(">>> Got bogus Vdata name (VSgetname) : %s\n", vsname);
      }

    if (HDstrcmp(vsclass, "No class specified"))
      {
          num_errs++;
          printf(">>> Got bogus Vdata class : %s\n", vsclass);
      }

    status = VSinquire(vs1, &count, &intr, fields, &sz, vsname);
    if (status == FAIL)
      {
          num_errs++;
          printf(">>> VSinquire failed on float Vdata\n");
      }

    if (HDstrcmp(vsname, "Mixed Vdata"))
      {
          num_errs++;
          printf(">>> Got bogus Mixed Vdata name (VSinquire) : %s\n", vsname);
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
    status = VSsetfields(vs1, fields);
    CHECK(status,FAIL,"VSsetfields:vs1");

    for (i = 0; i < 1000; i++)
        gbuf[i] = 0;

    status = VSread(vs1, (unsigned char *) gbuf, count, FULL_INTERLACE);
    CHECK(status,FAIL,"VSread:vs1");

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

    status = VSdetach(vs1);
    CHECK(status,FAIL,"VSdetach:vs1");

    /* Move to the next one (multi-order) */
    ref = VSgetid(fid, ref);
    if (ref == FAIL)
      {
          num_errs++;
          printf(">>> VSgetid was unable to find multi-order Vdata\n");
      }

    /* read in the first data and verify metadata and contents */
    vs1 = VSattach(fid, ref, "r");
    CHECK(vs1,FAIL,"VSattach:vs1");

    status = VSgetname(vs1, vsname);
    CHECK(status,FAIL,"VSgetname:vs1");

    status = VSgetclass(vs1, vsclass);
    CHECK(status,FAIL,"VSgetclass:vs1");

    if (HDstrcmp(vsname, "Multi-Order Vdata"))
      {
          num_errs++;
          printf(">>> Got bogus Vdata name (VSgetname) : %s\n", vsname);
      }

    if (HDstrcmp(vsclass, "No class specified"))
      {
          num_errs++;
          printf(">>> Got bogus Vdata class : %s\n", vsclass);
      }

    status = VSinquire(vs1, &count, &intr, fields, &sz, vsname);
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
    status = VSsetfields(vs1, fields);
    CHECK(status,FAIL,"VSsetfields:vs1");

    for (i = 0; i < 1000; i++)
        gbuf[i] = 0;

    status = VSread(vs1, (unsigned char *) gbuf, count, FULL_INTERLACE);
    CHECK(status,FAIL,"VSread:vs1");

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
    status = VSseek(vs1, 0);
    CHECK(status,FAIL,"VSseek:vs1");

    status = VSsetfields(vs1, ST);
    CHECK(status,FAIL,"VSsetfields:vs1");

    for (i = 0; i < 1000; i++)
        gbuf[i] = 0;

    status = VSread(vs1, (unsigned char *) gbuf, count, FULL_INTERLACE);
    CHECK(status,FAIL,"VSread:vs1");

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
    status = VSseek(vs1, 0);
    CHECK(status,FAIL,"VSseek:vs1");

    status = VSsetfields(vs1, ST);
    CHECK(status,FAIL,"VSsetfields:vs1");

    for (i = 0; i < 1000; i++)
        gbuf[i] = 0;

    status = VSread(vs1, (unsigned char *) gbuf, count, NO_INTERLACE);
    CHECK(status,FAIL,"VSread:vs1");

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
    status = VSfind(fid, "foo");
    CHECK(status,FAIL,"VSfind:fid");

    if (VSseek(vs1, 0) == FAIL)
      {
          num_errs++;
          printf(">>> VSseek failed after VSfind call\n");
      }

    status = VSdetach(vs1);
    CHECK(status,FAIL,"VSdetach:vs1");

    status = Vend(fid);
    CHECK(status,FAIL,"Vend:fid");

    status = Hclose(fid);
    CHECK(status,FAIL,"Hclose:fid");

    return SUCCEED;
}   /* read_vset_stuff */

/*
   Testing VSdelete for vdatas.
   Modification:
	2011/12/22: added a test for VSgetexternalinfo on non-external vdata.
 */
static void
test_vsdelete(void)
{
#define FIELD_NAME     "Field Entries"
#define NUMBER_OF_ROWS 10
#define ORDER           3
    int32  fid;
    int32  vdata_id;
    int32  status;
    int32  num_of_elements;
    int16  vdata_buf[NUMBER_OF_ROWS * ORDER];
    int32  v_ref;
    intn   i;

    /* Open the HDF file. */
    fid = Hopen(FNAME0, DFACC_RDWR, 0);
    CHECK_VOID(fid,FAIL,"Hopen:tvset.hdf");

    /* Initialize HDF for subsequent vgroup/vdata access. */
    status = Vstart(fid);
    CHECK_VOID(status,FAIL,"Vstart:fid");

    /* Create a new vdata. */
    vdata_id = VSattach(fid, -1, "w");
    CHECK_VOID(vdata_id,FAIL,"VSattach:vdata_id");
          
    /* Define the field data name, type and order. */
    status = VSfdefine(vdata_id, FIELD_NAME, DFNT_INT16, ORDER);
    CHECK_VOID(status,FAIL,"VSfdefine:vdata_id");

    /* Specify the field(s) that will be written to. */
    status = VSsetfields(vdata_id, FIELD_NAME);
    CHECK_VOID(status,FAIL,"VSsetfields:vdata_id");

    /* Generate the Vset data. */
    for (i = 0; i < NUMBER_OF_ROWS * ORDER; i+=ORDER) 
      {
          vdata_buf[i] = i;
          vdata_buf[i + 1] = i + 1;
          vdata_buf[i + 2] = i + 2;
      }

    /* Write the data to the Vset. */
    num_of_elements = VSwrite(vdata_id, (const uint8 *)vdata_buf, 
                              NUMBER_OF_ROWS, FULL_INTERLACE);
    CHECK_VOID(num_of_elements,FAIL,"VSwrite:");

    /* Set the name and class. */
    status = VSsetname(vdata_id, "Vdata should have been deleted");
    CHECK_VOID(status,FAIL,"VSsetname:vdata_id");

    status = VSsetclass(vdata_id, "Vdata should have been deleted");
    CHECK_VOID(status,FAIL,"VSsetclass:vdata_id");

    /* get ref of Vdata */
    v_ref = VSQueryref(vdata_id);
    CHECK_VOID(v_ref,FAIL,"VSQueryref:vdata_id");

    /* Terminate access to the vdata. */
    status = VSdetach(vdata_id);
    CHECK_VOID(status,FAIL,"VSdetach:vdata_id");

    /* Terminate access to the Vxxx interface and close the file. */
    status = Vend(fid);
    CHECK_VOID(status,FAIL,"Vend:fid");

    status = Hclose(fid);
    CHECK_VOID(status,FAIL,"Hclose:fid");

    /* Now open the file again and delete the vdata */
    /* Open the HDF file. */
    fid = Hopen(FNAME0, DFACC_RDWR, 0);
    CHECK_VOID(fid,FAIL,"Hopen:tvset.hdf");

    /* Initialize HDF for subsequent vgroup/vdata access. */
    status = Vstart(fid);
    CHECK_VOID(status,FAIL,"Vstart:fid");

    /* attach to Vdata */
    vdata_id = VSattach(fid, v_ref, "w");
    CHECK_VOID(vdata_id,FAIL,"VSattach:vdata_id");

    /* Test VSgetexternalinfo on this vdata that doesn't have external
       element, should return 0 for length of external file name */
    {
	intn name_len = 0;
	name_len = VSgetexternalinfo(vdata_id, 0, NULL, NULL, NULL);
	VERIFY_VOID(name_len, 0, "VSgetexternalinfo:vdata_id");
    }

    /* Delete this Vdata */
    status = VSdelete(fid, v_ref);
    CHECK_VOID(status,FAIL,"VSdelete:vdata_id");

    /* Terminate access to the vdata. */
    status = VSdetach(vdata_id);
    CHECK_VOID(status,FAIL,"VSdetach:vdata_id");

    /* Terminate access to the Vxxx interface and close the file. */
    status = Vend(fid);
    CHECK_VOID(status,FAIL,"Vend:fid");

    status = Hclose(fid);
    CHECK_VOID(status,FAIL,"Hclose:fid");

    /* Now open file again and try to attach to vdata with 'v_ref'.
       The VSattach should fail. */
    /* Open the HDF file. */
    fid = Hopen(FNAME0, DFACC_RDONLY, 0);
    CHECK_VOID(fid,FAIL,"Hopen:tvset.hdf");

    /* Initialize HDF for subsequent the vgroup/vdata access. */
    status = Vstart(fid);
    CHECK_VOID(status,FAIL,"Vstart:fid");

    /* Try to attach to Vdata. This should fail now */
    vdata_id = VSattach(fid, v_ref, "w");
    if (vdata_id != FAIL)
      {
          num_errs++;
          printf(">>> VSdelete failed to delete vdata \n");
      }

    /* Terminate access to the Vxxx interface and close the file. */
    status = Vend(fid);
    CHECK_VOID(status,FAIL,"Vend:fid");

    status = Hclose(fid);
    CHECK_VOID(status,FAIL,"Hclose:fid");

} /* test_vsdelete */

/* Testing Vdelete for vgroups. */ 
static void
test_vdelete(void)
{
    int32 fid;
    int32 vgroup_id;
    int32 status;
    int32 vg_ref;

    /* Open the HDF file. */
    fid = Hopen(FNAME0, DFACC_RDWR, 0);
    CHECK_VOID(fid,FAIL,"Hopen:tvset.hdf");

    /* Initialize HDF for subsequent vgroup/vdata access. */
    status = Vstart(fid);
    CHECK_VOID(status,FAIL,"Vstart:fid");

    /* Create a new vgroup. */
    vgroup_id = Vattach(fid, -1, "w");
    CHECK_VOID(vgroup_id,FAIL,"Vattach:vgroup_id");
          
    /* Set the name and class. */
    status = Vsetname(vgroup_id, "Vgroup should have been deleted");
    CHECK_VOID(status,FAIL,"Vsetname:vgroup_id");

    status = Vsetclass(vgroup_id, "Vgroup should have been deleted");
    CHECK_VOID(status,FAIL,"Vsetclass:vgroup_id");

    /* get ref of vgroup */
    vg_ref = VQueryref(vgroup_id);
    CHECK_VOID(vg_ref,FAIL,"VQueryref:vgroup_id");

    /* Terminate access to the vgroup. */
    status = Vdetach(vgroup_id);
    CHECK_VOID(status,FAIL,"Vdetach:vgroup_id");
    
    /* Terminate access to the Vxxx interface and close the file. */
    status = Vend(fid);
    CHECK_VOID(status,FAIL,"Vend:fid");

    status = Hclose(fid);
    CHECK_VOID(status,FAIL,"Hclose:fid");

    /* Now open the file again and delete the vgroup */
    /* Open the HDF file. */
    fid = Hopen(FNAME0, DFACC_RDWR, 0);
    CHECK_VOID(fid,FAIL,"Hopen:tvset.hdf");

    /* Initialize HDF for subsequent vgroup/vdata access. */
    status = Vstart(fid);
    CHECK_VOID(status,FAIL,"Vstart:fid");

    /* attach to vgroup */
    vgroup_id = Vattach(fid, vg_ref, "w");
    CHECK_VOID(vgroup_id,FAIL,"Vattach:vgroup_id");

    /* delete this vgroup */
    status = Vdelete(fid, vg_ref);
    CHECK_VOID(status,FAIL,"Vdelete:vgroup_id");

    /* Terminate access to the vgroup. */
    status = Vdetach(vgroup_id);
    CHECK_VOID(status,FAIL,"VSdetach:vgroup_id");

    /* Terminate access to the Vxxx interface and close the file. */
    status = Vend(fid);
    CHECK_VOID(status,FAIL,"Vend:fid");

    status = Hclose(fid);
    CHECK_VOID(status,FAIL,"Hclose:fid");

    /* Now open file again and try to attach to vgroup with 'vg_ref'.
       The Vattach should fail. */
    /* Open the HDF file. */
    fid = Hopen(FNAME0, DFACC_RDONLY, 0);
    CHECK_VOID(fid,FAIL,"Hopen:tvset.hdf");

    /* Initialize HDF for subsequent the vgroup/vdata access. */
    status = Vstart(fid);
    CHECK_VOID(status,FAIL,"Vstart:fid");

    /* Try to attach to vgroup. This should fail now */
    vgroup_id = Vattach(fid, vg_ref, "w");
    if(vgroup_id != FAIL)
      {
          num_errs++;
          printf(">>> Vdelete failed to delete vdata \n");
      }

    /* Terminate access to the Vxxx interface and close the file. */
    status = Vend(fid);
    CHECK_VOID(status,FAIL,"Vend:fid");

    status = Hclose(fid);
    CHECK_VOID(status,FAIL,"Hclose:fid");

} /* test_vdelete */

/* Testing Vdeletetagref() for vgroups. */ 
static void
test_vdeletetagref(void)
{
    int32 fid;
    int32 vgroup_id;
    int32 status;
    int32 vg_ref;

    /* Open the HDF file. */
    fid = Hopen(FNAME0, DFACC_RDWR, 0);
    CHECK_VOID(fid,FAIL,"Hopen:tvset.hdf");

    /* Initialize HDF for subsequent vgroup/vdata access. */
    status = Vstart(fid);
    CHECK_VOID(status,FAIL,"Vstart:fid");

    /* Create a new vgroup. */
    vgroup_id = Vattach(fid, -1, "w");
    CHECK_VOID(vgroup_id,FAIL,"Vattach:vgroup_id");
          
    /* Set the name and class. */
    status = Vsetname(vgroup_id, "Vgroup to delete elements from");
    CHECK_VOID(status,FAIL,"Vsetname:vgroup_id");

    status = Vsetclass(vgroup_id, "Vgroup to delete elements from");
    CHECK_VOID(status,FAIL,"Vsetclass:vgroup_id");

    /* add a few tag/ref pairs to Vgroup */
    status = Vaddtagref(vgroup_id, 1000, 12345);
    CHECK_VOID(status,FAIL,"Vaddtagref");
    status = Vaddtagref(vgroup_id, 1000, 12346);
    CHECK_VOID(status,FAIL,"Vaddtagref");

#ifndef NO_DUPLICATES
    /* duplicate tag/ref pairs allowed. 
       So add a duplicate */
    status = Vaddtagref(vgroup_id, 1000, 12346);
    CHECK_VOID(status,FAIL,"Vaddtagref");

#endif /* NO_DUPLICATES */

    status = Vaddtagref(vgroup_id, 2000, 12345);
    CHECK_VOID(status,FAIL,"Vaddtagref");
    status = Vaddtagref(vgroup_id, 2000, 12346);
    CHECK_VOID(status,FAIL,"Vaddtagref");

    status = Vaddtagref(vgroup_id, 3000, 12345);
    CHECK_VOID(status,FAIL,"Vaddtagref");
    status = Vaddtagref(vgroup_id, 3000, 12346);
    CHECK_VOID(status,FAIL,"Vaddtagref");

    /* get ref of vgroup */
    vg_ref = VQueryref(vgroup_id);
    CHECK_VOID(vg_ref,FAIL,"VQueryref:vgroup_id");

    /* delete one item in vgroup during this round */
    status = Vdeletetagref(vgroup_id, 1000, 12346);
    CHECK_VOID(status,FAIL,"Vdeletetagref:vgroup_id");

    /* Terminate access to the vgroup. */
    status = Vdetach(vgroup_id);
    CHECK_VOID(status,FAIL,"Vdetach:vgroup_id");
    
    /* Terminate access to the Vxxx interface and close the file. */
    status = Vend(fid);
    CHECK_VOID(status,FAIL,"Vend:fid");

    status = Hclose(fid);
    CHECK_VOID(status,FAIL,"Hclose:fid");

    /* Now open the file again and delete two elements in the vgroup
       during this round. */

    /* Open the HDF file. */
    fid = Hopen(FNAME0, DFACC_RDWR, 0);
    CHECK_VOID(fid,FAIL,"Hopen:tvset.hdf");

    /* Initialize HDF for subsequent vgroup/vdata access. */
    status = Vstart(fid);
    CHECK_VOID(status,FAIL,"Vstart:fid");

    /* attach to vgroup */
    vgroup_id = Vattach(fid, vg_ref, "w");
    CHECK_VOID(vgroup_id,FAIL,"Vattach:vgroup_id");

#ifndef NO_DUPLICATES
    /* inquire about number of elments in Vgroup.
       There should only be 6 of them including one duplicate. */
    if (6 != Vntagrefs(vgroup_id))
      {
          num_errs++;
          printf(">>> Vntagrefs returned %d was expecting %d\n", 
                 (int)Vntagrefs(vgroup_id),6);
      }

    /* delete a duplicate in this vgroup */
    status = Vdeletetagref(vgroup_id, 1000, 12346);
    CHECK_VOID(status,FAIL,"Vdeletetagref:vgroup_id");
#else /* NO_DUPLICATES */
    /* inquire about number of elments in Vgroup.
       There should only be 5 of them since no duplicates . */
    if (5 != Vntagrefs(vgroup_id))
      {
          num_errs++;
          printf(">>> Vntagrefs returned %d was expecting %d\n", 
                 (int)Vntagrefs(vgroup_id),5);
      }
#endif /* NO_DUPLICATES */

    /* delete some tag/refs in this vgroup */
    status = Vdeletetagref(vgroup_id, 2000, 12346);
    CHECK_VOID(status,FAIL,"Vdeletetagref:vgroup_id");

    /* this should be the last element in the vgroup if I have
       the order right */
    status = Vdeletetagref(vgroup_id, 3000, 12346);
    CHECK_VOID(status,FAIL,"Vdeletetagref:vgroup_id");

    /* Terminate access to the vgroup. */
    status = Vdetach(vgroup_id);
    CHECK_VOID(status,FAIL,"VSdetach:vgroup_id");

    /* Terminate access to the Vxxx interface and close the file. */
    status = Vend(fid);
    CHECK_VOID(status,FAIL,"Vend:fid");

    status = Hclose(fid);
    CHECK_VOID(status,FAIL,"Hclose:fid");

    /* Now open file again and try to attach to vgroup with 'vg_ref'.
       There should only be 3 elements left in Vgroup left . */

    /* Open the HDF file. */
    fid = Hopen(FNAME0, DFACC_RDONLY, 0);
    CHECK_VOID(fid,FAIL,"Hopen:tvset.hdf");

    /* Initialize HDF for subsequent the vgroup/vdata access. */
    status = Vstart(fid);
    CHECK_VOID(status,FAIL,"Vstart:fid");

    /* attach to vgroup, read only */
    vgroup_id = Vattach(fid, vg_ref, "r");
    CHECK_VOID(vgroup_id,FAIL,"Vattach:vgroup_id");

    /* inquire about number of elments left in Vgroup.
       There should only be 3 of them now. */
    if (3 != Vntagrefs(vgroup_id))
      {
          num_errs++;
          printf(">>> Vntagrefs returned %d was expecting %d\n", 
                 (int)Vntagrefs(vgroup_id), 3);
      }

    /* check tag/ref pair of those 3 elements */
    if (Vinqtagref(vgroup_id, 1000, 12345) == FALSE)
      {
          num_errs++;
          printf(">>> Vinqtagref couldn't find valid element\n");
      }
    if (Vinqtagref(vgroup_id, 2000, 12345) == FALSE)
      {
          num_errs++;
          printf(">>> Vinqtagref couldn't find valid element\n");
      }
    if (Vinqtagref(vgroup_id, 3000, 12345) == FALSE)
      {
          num_errs++;
          printf(">>> Vinqtagref couldn't find valid element\n");
      }

    /* Terminate access to the Vxxx interface and close the file. */
    status = Vend(fid);
    CHECK_VOID(status,FAIL,"Vend:fid");

    status = Hclose(fid);
    CHECK_VOID(status,FAIL,"Hclose:fid");

} /* test_vdeletetagref */

static void
test_emptyvdata(void)
{
    int32 status;       /* Status values from routines */
    int32 fid;          /* File ID */
    int32 vs1;          /* Vdata ID */
    int32 ref;          /* Vdata ref */
    char  vsname[VSNAMELENMAX], fields[FIELDNAMELENMAX*VSFIELDMAX];

    /* Open the HDF file. */
    fid = Hopen(EMPTYNM, DFACC_CREATE, 0);
    CHECK_VOID(fid,FAIL,"Hopen");

    /* Initialize HDF for subsequent vgroup/vdata access. */
    status = Vstart(fid);
    CHECK_VOID(status,FAIL,"Vstart");

    /* Create a new vdata. */
    vs1 = VSattach(fid, -1, "w");
    CHECK_VOID(vs1,FAIL,"VSattach");

    status=VSsetname(vs1, EMPTY_VDATA);
    CHECK_VOID(status,FAIL,"VSsetname");

    status = VSdetach(vs1);
    CHECK_VOID(status,FAIL,"Vdetach");
    
    status = Vend(fid);
    CHECK_VOID(status,FAIL,"Vend");

    status = Hclose(fid);
    CHECK_VOID(status,FAIL,"Hclose");

    MESSAGE(5, printf("created empty VDATA %s\n", EMPTY_VDATA););

    /* Re-open the HDF file. */
    fid = Hopen(EMPTYNM, DFACC_RDWR, 0);
    CHECK_VOID(fid,FAIL,"Hopen");

    /* Initialize HDF for subsequent vgroup/vdata access. */
    status = Vstart(fid);
    CHECK_VOID(status,FAIL,"Vstart");

    /* Find the empty vdata. */
    ref=VSfind(fid,EMPTY_VDATA);
    CHECK_VOID(ref,FAIL,"VSfind");

    vs1 = VSattach(fid, ref, "r");
    CHECK_VOID(vs1,FAIL,"VSattach");

    status=VSgetname(vs1, vsname);
    CHECK_VOID(status,FAIL,"VSgetname");

    if (HDstrcmp(vsname, EMPTY_VDATA))
      {
          num_errs++;
          printf(">>> Got bogus Vdata name : %s\n", vsname);
      }

    status=VFnfields(vs1);
    VERIFY_VOID(status,0,"VFnfields");

    /* Verify that VSgetfields will return FAIL when passing in a NULL
       for field name list (from bug #554), although this might never 
       happen - BMR 5/17/01 */
    status = VSgetfields(vs1, NULL);
    VERIFY_VOID(status, FAIL, "VSgetfields");

    status=VSgetfields(vs1,fields);
    CHECK_VOID(status,FAIL,"VSgetfields");

    if (HDstrcmp(fields, ""))
      {
          num_errs++;
          printf(">>> Got bogus field names : %s\n", fields);
      }

    status = VSdetach(vs1);
    CHECK_VOID(status,FAIL,"Vdetach");
    
    status = Vend(fid);
    CHECK_VOID(status,FAIL,"Vend");

    status = Hclose(fid);
    CHECK_VOID(status,FAIL,"Hclose");

    MESSAGE(5, printf("read back in empty VDATA %s\n", EMPTY_VDATA););

    /* Re-open the HDF file. */
    fid = Hopen(EMPTYNM, DFACC_RDWR, 0);
    CHECK_VOID(fid,FAIL,"Hopen");

    /* Initialize HDF for subsequent vgroup/vdata access. */
    status = Vstart(fid);
    CHECK_VOID(status,FAIL,"Vstart");

    /* Find the empty vdata. */
    ref=VSfind(fid,EMPTY_VDATA);
    CHECK_VOID(ref,FAIL,"VSfind");

    vs1 = VSattach(fid, ref, "w");
    CHECK_VOID(vs1,FAIL,"VSattach");

    /* Write out simple vdata fields */
    status=VSfdefine(vs1, FIELD1, DFNT_FLOAT32, 1);
    CHECK_VOID(status,FAIL,"VSfdefine");

    status = VSfdefine(vs1, FIELD2, DFNT_INT32, 2);
    CHECK_VOID(status,FAIL,"VSfdefine");

    status = VSsetfields(vs1, FIELD1","FIELD2);
    if (status == FAIL)
      {
          num_errs++;
          printf(">>> Vsetfields failed for %s\n", vsname);
      }

    status = VSdetach(vs1);
    CHECK_VOID(status,FAIL,"Vdetach");
    
    status = Vend(fid);
    CHECK_VOID(status,FAIL,"Vend");

    status = Hclose(fid);
    CHECK_VOID(status,FAIL,"Hclose");

    MESSAGE(5, printf("changed empty VDATA %s to have two fields\n", EMPTY_VDATA););

    /* Re-open the HDF file. */
    fid = Hopen(EMPTYNM, DFACC_RDWR, 0);
    CHECK_VOID(fid,FAIL,"Hopen");

    /* Initialize HDF for subsequent vgroup/vdata access. */
    status = Vstart(fid);
    CHECK_VOID(status,FAIL,"Vstart");

    /* Find the empty vdata. */
    ref=VSfind(fid,EMPTY_VDATA);
    CHECK_VOID(ref,FAIL,"VSfind");

    vs1 = VSattach(fid, ref, "r");
    CHECK_VOID(vs1,FAIL,"VSattach");

    status=VFnfields(vs1);
    VERIFY_VOID(status,2,"VFnfields");

    status=VSgetfields(vs1,fields);
    CHECK_VOID(status,FAIL,"VSgetfields");

    if (HDstrcmp(fields, FIELD1","FIELD2))
      {
          num_errs++;
          printf(">>> Got bogus field names : %s\n", fields);
      }

    status = VSdetach(vs1);
    CHECK_VOID(status,FAIL,"Vdetach");
    
    status = Vend(fid);
    CHECK_VOID(status,FAIL,"Vend");

    status = Hclose(fid);
    CHECK_VOID(status,FAIL,"Hclose");

} /* test_emptyvdata() */

static void
test_vglongnames(void)
{
    int32 status;       /* Status values from routines */
    int32 fid;          /* File ID */
    int32 vg1;          /* Vdata ID */
    int32 ref;          /* Vdata ref */
    uint16 name_len;	/* Length of a vgroup's name or class name */
    char *vgname, *vgclass;

    /* Open the HDF file. */
    fid = Hopen(LONGNAMES, DFACC_CREATE, 0);
    CHECK_VOID(fid,FAIL,"Hopen");

    /* Initialize HDF for subsequent vgroup/vdata access. */
    status = Vstart(fid);
    CHECK_VOID(status,FAIL,"Vstart");

    /* Create a new vgroup. */
    vg1 = Vattach(fid, -1, "w");
    CHECK_VOID(vg1,FAIL,"VSattach");

    status=Vsetname(vg1, VG_LONGNAME);
    CHECK_VOID(status,FAIL,"VSsetname");

    status=Vsetclass(vg1, VG_LONGCLASS);
    CHECK_VOID(status,FAIL,"VSsetname");

    status = Vdetach(vg1);
    CHECK_VOID(status,FAIL,"Vdetach");
    
    /* Create another vgroup of the same class. */
    vg1 = Vattach(fid, -1, "w");
    CHECK_VOID(vg1,FAIL,"VSattach");

    status=Vsetname(vg1, VGROUP1);
    CHECK_VOID(status,FAIL,"VSsetname");

    status=Vsetclass(vg1, VG_LONGCLASS);
    CHECK_VOID(status,FAIL,"VSsetname");

    status = Vdetach(vg1);
    CHECK_VOID(status,FAIL,"Vdetach");
    
    status = Vend(fid);
    CHECK_VOID(status,FAIL,"Vend");

    status = Hclose(fid);
    CHECK_VOID(status,FAIL,"Hclose");

    /* Re-open the HDF file. */
    fid = Hopen(LONGNAMES, DFACC_RDWR, 0);
    CHECK_VOID(fid,FAIL,"Hopen");

    /* Initialize HDF for subsequent vgroup/vdata access. */
    status = Vstart(fid);
    CHECK_VOID(status,FAIL,"Vstart");

    /* Find the long name vgroup. */
    ref=Vfind(fid,VG_LONGNAME);
    CHECK_VOID(ref,FAIL,"VSfind");

    vg1 = Vattach(fid, ref, "r");
    CHECK_VOID(vg1,FAIL,"VSattach");

    /* get the vgroup's name */
    status = Vgetnamelen(vg1, &name_len);
    CHECK_VOID(status,FAIL,"Vgetnamelen");

    vgname = (char *) HDmalloc(sizeof(char *) * (name_len+1));
    CHECK_ALLOC(vgname, "vgname", "test_vglongnames" );

    status=Vgetname(vg1, vgname);
    CHECK_VOID(status,FAIL,"VSgetname");

    if (HDstrcmp(vgname, VG_LONGNAME)) {
        num_errs++;
        printf(">>> Got bogus Vgroup name : %s\n", vgname);
    }

    if (vgname != NULL)
        HDfree(vgname);

    /* get the vgroup's class */
    status = Vgetclassnamelen(vg1, &name_len);
    CHECK_VOID(status,FAIL,"Vgetnamelen");

    vgclass = (char *) HDmalloc(sizeof(char *) * (name_len+1));
    CHECK_ALLOC(vgclass, "vgclass", "test_vglongnames" );

    status=Vgetclass(vg1, vgclass);
    CHECK_VOID(status,FAIL,"VSgetclass");

    if (HDstrcmp(vgclass, VG_LONGCLASS)) {
        num_errs++;
        printf(">>> Got bogus Vgroup class : %s\n", vgclass);
    }

    if (vgclass != NULL)
        HDfree(vgclass);

    status = Vdetach(vg1);
    CHECK_VOID(status,FAIL,"Vdetach");
    
    /* Find the vgroup VGROUP1. */
    ref=Vfind(fid,VGROUP1);
    CHECK_VOID(ref,FAIL,"VSfind");

    vg1 = Vattach(fid, ref, "r");
    CHECK_VOID(vg1,FAIL,"VSattach");

    /* get the vgroup's name */
    status = Vgetnamelen(vg1, &name_len);
    CHECK_VOID(status,FAIL,"Vgetnamelen");

    vgname = (char *) HDmalloc(sizeof(char *) * (name_len+1));
    CHECK_ALLOC(vgname, "vgname", "test_vglongnames" );

    status=Vgetname(vg1, vgname);
    CHECK_VOID(status,FAIL,"VSgetname");

    if (HDstrcmp(vgname, VGROUP1)) {
        num_errs++;
        printf(">>> Got bogus Vgroup name : %s\n", vgname);
    }

    if (vgname != NULL)
        HDfree(vgname);

    /* get the vgroup's class */
    status = Vgetclassnamelen(vg1, &name_len);
    CHECK_VOID(status,FAIL,"Vgetnamelen");

    vgclass = (char *) HDmalloc(sizeof(char *) * (name_len+1));
    CHECK_ALLOC(vgclass, "vgclass", "test_vglongnames" );

    status=Vgetclass(vg1, vgclass);
    CHECK_VOID(status,FAIL,"VSgetclass");

    if (HDstrcmp(vgclass, VG_LONGCLASS)) {
        num_errs++;
        printf(">>> Got bogus Vgroup class : %s\n", vgclass);
    }

    if (vgclass != NULL)
        HDfree(vgclass);

    status = Vdetach(vg1);
    CHECK_VOID(status,FAIL,"Vdetach");
    
    status = Vend(fid);
    CHECK_VOID(status,FAIL,"Vend");

    status = Hclose(fid);
    CHECK_VOID(status,FAIL,"Hclose");

} /* test_vglongnames() */

#define USERVGROUPS "tuservgs.hdf"
#define NUM_VGROUPS 10
static void
test_getvgroups(void)
{
    int32 fid;          /* File ID */
    int32 vgroup_id, vgroup0_id, vgroup1_id, vgroup2_id,
	  vgroup3_id, vgroup4_id, vgroup5_id;	/* Various vgroup IDs */
    int32 vgroup_ref;   /* Vgroup ref */
    uintn n_vgs=0;
    uint16 *refarray;
    int32 ref_list[NUM_VGROUPS];
    char vgclass[20];
    int ii;
    int32 status;       /* Status values from routines */
    intn  status_n;	/* returned status for functions returning an intn  */


    /* Create HDF file and initialize the interface. */
    fid = Hopen(USERVGROUPS, DFACC_CREATE, 0);
    CHECK_VOID(fid, FAIL, "Hopen");
    status = Vstart(fid);
    CHECK_VOID(status, FAIL, "Vstart");

    /* Create NUM_VGROUPS vgroups and set classname */
    for (ii = 0; ii < NUM_VGROUPS; ii++) {
        /* Create a vgroup. */
        vgroup_id = Vattach(fid, -1, "w");
        CHECK_VOID(vgroup_id, FAIL, "Vattach");

        /* Record its reference number for later access */
        vgroup_ref = VQueryref(vgroup_id);
        CHECK_VOID(vgroup_ref, FAIL, "VQueryref:vgroup_id");
        ref_list[ii] = vgroup_ref;

        /* Set its class name */
        sprintf(vgclass, "VG-CLASS-%d", ii);
        status = Vsetclass(vgroup_id, vgclass);
        CHECK_VOID(status, FAIL, "Vsetclass");

        /* Detach it */
        status = Vdetach(vgroup_id);
        CHECK_VOID(status, FAIL, "Vdetach");
    }

    /* Insert some vgroups into some other vgroups to build some sort of
	vgroup structure */

    /* Insert "VG-CLASS-1" and "VG-CLASS-2" into "VG-CLASS-0" */
    vgroup0_id = Vattach(fid, ref_list[0], "w");
    CHECK_VOID(vgroup0_id, FAIL, "Vattach");
    vgroup1_id = Vattach(fid, ref_list[1], "w");
    CHECK_VOID(vgroup1_id, FAIL, "Vattach");
    vgroup2_id = Vattach(fid, ref_list[2], "w");
    CHECK_VOID(vgroup2_id, FAIL, "Vattach");
    status = Vinsert(vgroup0_id, vgroup1_id);
    CHECK_VOID(status, FAIL, "Vinsert vgroup1_id into vgroup0_id");
    status = Vinsert(vgroup0_id, vgroup2_id);
    CHECK_VOID(status, FAIL, "Vinsert vgroup2_id into vgroup0_id");

    /* Insert "VG-CLASS-3", "VG-CLASS-4", and "VG-CLASS-5" into "VG-CLASS-1" */
    vgroup3_id = Vattach(fid, ref_list[3], "w");
    CHECK_VOID(vgroup3_id, FAIL, "Vattach");
    vgroup4_id = Vattach(fid, ref_list[4], "w");
    CHECK_VOID(vgroup4_id, FAIL, "Vattach");
    vgroup5_id = Vattach(fid, ref_list[5], "w");
    CHECK_VOID(vgroup5_id, FAIL, "Vattach");
    status = Vinsert(vgroup1_id, vgroup3_id);
    CHECK_VOID(status, FAIL, "Vinsert vgroup3_id into vgroup1_id");
    status = Vinsert(vgroup1_id, vgroup4_id);
    CHECK_VOID(status, FAIL, "Vinsert vgroup4_id into vgroup1_id");
    status = Vinsert(vgroup1_id, vgroup5_id);
    CHECK_VOID(status, FAIL, "Vinsert vgroup5_id into vgroup1_id");

    /**************************************************************
	The vgroup structure should look like this:
		vg0	vg6  vg7  vg8  vg9
		 |
		/ \
	      vg1  vg2
               |
             / | \
            /  |  \
         vg3  vg4  vg5

    Calling Vgetvgroups on the file should return all ten vgroups.
    Calling Vgetvgroups on vg0 should return 2, vg1 and vg2.
    Calling Vgetvgroups on vg1 should return 3, vg3, vg4, and vg5
    Calling Vgetvgroups on vg6, vg7, vg8, and vg9 should return 0
    ***************************************************************/

    /* Get and verify the number of vgroups in the file */
    n_vgs = Vgetvgroups(fid, 0, 0, NULL);
    CHECK_VOID(n_vgs, FAIL, "Vgetvgroups fid");
    VERIFY_VOID(n_vgs, NUM_VGROUPS, "Vgetvgroups fid");

    /* Allocate space to retrieve the reference numbers of n_vgs vgroups */
    refarray = (uint16 *)HDmalloc(sizeof(uint16)*n_vgs);

    /* Get all the vgroups in the file */
    n_vgs = Vgetvgroups(fid, 0, n_vgs, refarray);
    CHECK_VOID(n_vgs, FAIL, "Vgetvgroups fid");
    VERIFY_VOID(n_vgs, NUM_VGROUPS, "Vgetvgroups fid");

    /* Verify refarray from this Vgetvgroups, it should contain:
	2  3  4  5  6  7  8  9  10  11 */ 
    {
        uint16 result[] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
        for (ii = 0; ii < n_vgs; ii++)
            if (refarray[ii] != result[ii])
                fprintf(stderr, "test_getvgroups: incorrect vgroup retrieved at line %d - ref# %d should be %d\n", __LINE__, refarray[ii], result[ii]);
    }

    /* Get 5 vgroups starting from vgroup number 5 */
    n_vgs = Vgetvgroups(fid, 5, 5, refarray);
    CHECK_VOID(n_vgs, FAIL, "Vgetvgroups fid (5,5)");
    VERIFY_VOID(n_vgs, 5, "Vgetvgroups fid (5,5)");

    /* Verify refarray from this Vgetvgroups, it should contain:
	7  8  9  10  11 */
    {
        uint16 result[] = {7, 8, 9, 10, 11};
        for (ii = 0; ii < n_vgs; ii++)
            if (refarray[ii] != result[ii])
                fprintf(stderr, "test_getvgroups: incorrect vgroup retrieved at line %d - ref# %d should be %d\n", __LINE__, refarray[ii], result[ii]);
    }

    /* Get and verify the number of vgroups in vgroup0_id */
    n_vgs = Vgetvgroups(vgroup0_id, 0, 0, NULL);
    CHECK_VOID(n_vgs, FAIL, "Vgetvgroups vgroup0_id");
    VERIFY_VOID(n_vgs, 2, "Vgetvgroups vgroup0_id");

    /* Get all the vgroups in vgroup0_id (refarray already allocated to max */
    n_vgs = Vgetvgroups(vgroup0_id, 0, n_vgs, refarray);
    CHECK_VOID(n_vgs, FAIL, "Vgetvgroups vgroup0_id");
    VERIFY_VOID(n_vgs, 2, "Vgetvgroups vgroup0_id");

    /* Verify refarray from this Vgetvgroups, it should contain: 3  4 */ 
    {
        uint16 result[] = {3, 4};
        for (ii = 0; ii < n_vgs; ii++)
            if (refarray[ii] != result[ii])
                fprintf(stderr, "test_getvgroups: incorrect vgroup retrieved at line %d - ref# %d should be %d\n", __LINE__, refarray[ii], result[ii]);
    }

    /* Get and verify the number of vgroups in vgroup1_id */
    n_vgs = Vgetvgroups(vgroup1_id, 0, 0, NULL);
    CHECK_VOID(n_vgs, FAIL, "Vgetvgroups vgroup1_id");
    VERIFY_VOID(n_vgs, 3, "Vgetvgroups vgroup1_id");

    /* Get all the vgroups in vgroup1_id */
    n_vgs = Vgetvgroups(vgroup1_id, 0, n_vgs, refarray);
    CHECK_VOID(n_vgs, FAIL, "Vgetvgroups vgroup1_id");
    VERIFY_VOID(n_vgs, 3, "Vgetvgroups vgroup1_id");

    /* Verify refarray from this Vgetvgroups, it should contain: 5  6  7 */ 
    {
        uint16 result[] = {5, 6, 7};
        for (ii = 0; ii < n_vgs; ii++)
            if (refarray[ii] != result[ii])
                fprintf(stderr, "test_getvgroups: incorrect vgroup retrieved at line %d - ref# %d should be %d\n", __LINE__, refarray[ii], result[ii]);
    }

    /* This vgroup should have no sub-vgroup */
    n_vgs = Vgetvgroups(vgroup5_id, 0, 0, NULL);
    CHECK_VOID(n_vgs, FAIL, "Vgetvgroups vgroup5_id");
    VERIFY_VOID(n_vgs, 0, "Vgetvgroups vgroup5_id");

    /* These vgroups are not needed anymore.  */
    status = Vdetach(vgroup2_id);
    CHECK_VOID(status, FAIL, "Vdetach vgroup2_id");
    status = Vdetach(vgroup3_id);
    CHECK_VOID(status, FAIL, "Vdetach vgroup3_id");
    status = Vdetach(vgroup4_id);
    CHECK_VOID(status, FAIL, "Vdetach vgroup4_id");
    status = Vdetach(vgroup5_id);
    CHECK_VOID(status, FAIL, "Vdetach vgroup5_id");

    /* Change class name of vg6 and vg7 to an internal class name to
       simulate that they are internally created by the library. */
    vgroup_id = Vattach(fid, ref_list[6], "w");
    CHECK_VOID(vgroup_id, FAIL, "Vattach vg6");
    status = Vsetclass(vgroup_id, _HDF_VARIABLE);
    CHECK_VOID(status, FAIL, "Vsetclass _HDF_VARIABLE");
    status = Vdetach(vgroup_id);
    CHECK_VOID(status, FAIL, "Vdetach vg6");

    vgroup_id = Vattach(fid, ref_list[7], "w");
    CHECK_VOID(vgroup_id, FAIL, "Vattach vg7");
    status = Vsetclass(vgroup_id, _HDF_DIMENSION);
    CHECK_VOID(status, FAIL, "Vsetclass _HDF_DIMENSION");
    status = Vdetach(vgroup_id);
    CHECK_VOID(status, FAIL, "Vdetach vg7");

    /**************************************************************
    Calling Vgetvgroups on the file now should return 8 because vg6
    and vg7 are no longer seen as user-created vgroups due to their
    class name change.
    ***************************************************************/

    /* Get the number of vgroups in the file, which shouldn't include the
	simulated internal vgroups */
    n_vgs = Vgetvgroups(fid, 0, 0, NULL);
    CHECK_VOID(n_vgs, FAIL, "Vgetvgroups fid");
    VERIFY_VOID(n_vgs, 8, "Vgetvgroups fid");

    /* Get these vgroups */
    n_vgs = Vgetvgroups(fid, 0, n_vgs, refarray);
    CHECK_VOID(n_vgs, FAIL, "Vgetvgroups fid");
    VERIFY_VOID(n_vgs, 8, "Vgetvgroups fid");

    /* Verify refarray from this Vgetvgroups, it should contain:
	2  3  4  5  6  7  10  11 */ 
    {
        uint16 result[] = { 2, 3, 4, 5, 6, 7, 10, 11 };
        for (ii = 0; ii < n_vgs; ii++)
            if (refarray[ii] != result[ii])
                fprintf(stderr, "test_getvgroups: incorrect vgroup retrieved at line %d - ref# %d should be %d\n", __LINE__, refarray[ii], result[ii]);
    }

    /* Get 5 vgroups starting from vgroup number 5, the result shouldn't
	include the simulated internal vgroups */
    n_vgs = Vgetvgroups(fid, 5, 5, refarray);
    CHECK_VOID(n_vgs, FAIL, "Vgetvgroups fid");
    VERIFY_VOID(n_vgs, 3, "Vgetvgroups fid");

    /* Verify refarray from this Vgetvgroups, it should contain: 7  10  11 */ 
    {
        uint16 result[] = { 7, 10, 11 };
        for (ii = 0; ii < n_vgs; ii++)
            if (refarray[ii] != result[ii])
                fprintf(stderr, "test_getvgroups: incorrect vgroup retrieved at line %d - ref# %d should be %d\n", __LINE__, refarray[ii], result[ii]);
    }

    /* Check on vgroup0_id again */
    n_vgs = Vgetvgroups(vgroup0_id, 0, 0, NULL);
    CHECK_VOID(n_vgs, FAIL, "Vgetvgroups vgroup0_id");
    VERIFY_VOID(n_vgs, 2, "Vgetvgroups vgroup0_id");

    n_vgs = Vgetvgroups(vgroup0_id, 0, n_vgs, refarray);
    CHECK_VOID(n_vgs, FAIL, "Vgetvgroups vgroup0_id");
    VERIFY_VOID(n_vgs, 2, "Vgetvgroups vgroup0_id");

    /* Verify refarray from this Vgetvgroups, it should contain: 3  4 */ 
    {
        uint16 result[] = { 3, 4 };
        for (ii = 0; ii < n_vgs; ii++)
            if (refarray[ii] != result[ii])
                fprintf(stderr, "test_getvgroups: incorrect vgroup retrieved at line %d - ref# %d should be %d\n", __LINE__, refarray[ii], result[ii]);
    }

    /* Passing in more info count (3) than the actual number of vgrous to
	be retrieved (1) */
    n_vgs = Vgetvgroups(vgroup1_id, 2, 3, refarray);
    CHECK_VOID(n_vgs, FAIL, "Vgetvgroups vgroup1_id");
    VERIFY_VOID(n_vgs, 1, "Vgetvgroups vgroup1_id");

    /* Verify refarray from this Vgetvgroups, it should contain: 7 */ 
    {
        uint16 result[] = { 7 };
        for (ii = 0; ii < n_vgs; ii++)
            if (refarray[ii] != result[ii])
                fprintf(stderr, "test_getvgroups: incorrect vgroup retrieved at line %d - ref# %d should be %d\n", __LINE__, refarray[ii], result[ii]);
    }

    /* Passing in info count as 0 for a non-null array, should fail */
    n_vgs = Vgetvgroups(fid, 0, 0, refarray);
    VERIFY_VOID(n_vgs, FAIL, "Vgetvgroups with info_count = 0");

    /* Passing in the starting vgroup beyond the number of user-created vgroups,
       should fail */
    n_vgs = Vgetvgroups(fid, 9, 3, refarray);
    VERIFY_VOID(n_vgs, FAIL, "Vgetvgroups with start_vg = 9");

    if (refarray != NULL)
        HDfree(refarray);

    /* Terminate access to the V interface and close the HDF file.  */
    status_n = Vend (fid);
    CHECK_VOID(status_n, FAIL, "Vend");
    status_n = Hclose (fid);
    CHECK_VOID(status_n, FAIL, "Hclose");
} /* test_getvgroups() */

intn check_vgs(int32 id,
	   uintn start_vg,
	   uintn n_vgs,
	   char *ident_text, /* just for debugging, remove when done */
	   uintn resultcount,   /* expected number of vgroups */
	   uint16 *resultarray) /* array containing expected values */
{
    uint16 *refarray=NULL;
    uintn count=0, ii;
    char message[30];
    intn ret_value=SUCCEED;

    HDstrcpy(message, "Vgetvgroups: ");
    HDstrcat(message, ident_text);

    /* Get and verify the number of vgroups in the file */
    count = Vgetvgroups(id, start_vg, n_vgs, NULL);
    CHECK(count, FAIL, "Vgetvgroups");
    VERIFY(count, resultcount, "Vgetvgroups");

    /* Allocate space to retrieve the reference numbers of 'count' vgroups */
    refarray = (uint16 *)HDmalloc(sizeof(uint16)*count);
    if (refarray == NULL) {
        fprintf(stderr, "check_vgs: Allocation refarray failed\n");
        return (-1);
    }

    /* Get all the vgroups in the file */
    count = Vgetvgroups(id, start_vg, count, refarray);
    CHECK(count, FAIL, "Vgetvgroups");
    VERIFY(count, resultcount, "Vgetvgroups");

    for (ii = 0; ii < count; ii++)
	if (refarray[ii] != resultarray[ii])
	    fprintf(stderr, "%s: at index %d - read value=%d, should be %d\n",
		ident_text, ii, refarray[ii], resultarray[ii]);

    if (refarray != NULL)
        HDfree(refarray);

    return ret_value;
}

intn check_vds(int32 id,
	   uintn start_vd,
	   uintn n_vds,
	   char *ident_text, /* just for debugging, remove when done */
	   uintn resultcount,   /* expected number of vdatas */
	   uint16 *resultarray) /* array containing expected values */
{
    uint16 *refarray=NULL;
    uintn count=0, ii;
    char message[30];
    intn ret_value=SUCCEED;

    HDstrcpy(message, "VSgetvdatas: ");
    HDstrcat(message, ident_text);

    /* Get and verify the number of vdatas in the file */
    count = VSgetvdatas(id, start_vd, n_vds, NULL);
    CHECK(count, FAIL, message);
    VERIFY(count, resultcount, message);

    /* Allocate space to retrieve the reference numbers of 'count' vdatas */
    refarray = (uint16 *)HDmalloc(sizeof(uint16)*count);
    if (refarray == NULL) {
        fprintf(stderr, "check_vds: Allocation refarray failed\n");
        return (-1);
    }

    /* Get all the vdatas in the file */
    count = VSgetvdatas(id, start_vd, count, refarray);
    CHECK(count, FAIL, message);
    VERIFY(count, resultcount, message);

    for (ii = 0; ii < count; ii++)
	if (refarray[ii] != resultarray[ii])
	    fprintf(stderr, "%s: at index %d - read value=%d, should be %d\n",
		ident_text, ii, refarray[ii], resultarray[ii]);

    if (refarray != NULL)
        HDfree(refarray);
    
    return ret_value;
}

#define USERVDATAS "tuservds.hdf"
#define NUM_VDATAS 8
static void
test_getvdatas(void)
{
    int32 vgroup_id;
    int32 vgroup_ref;
    int32 vdata_id;
    int32 vdata_ref;
    int32 fid;          /* File ID */
    int32 vgroup0_id, vgroup1_id, vgroup2_id, vgroup4_id,
	  vgroup6_id, vgroup7_id, vgroup9_id;	/* Various vgroup IDs */
    int32 vdata1_id, vdata2_id, vdata3_id, vdata4_id, vdata5_id,
	  vdata6_id, vdata7_id;    /* Various vdata IDs */
    uintn n_vgs=0;
    int32 ref_list[NUM_VGROUPS], vdref_list[NUM_VDATAS];
    char vgclass[20];
    int ii;
    int32 status;       /* Status values from routines */
    intn  status_n;	/* returned status for functions returning an intn  */

    /* Create HDF file and initialize the interface. */
    fid = Hopen(USERVDATAS, DFACC_CREATE, 0);
    CHECK_VOID(fid, FAIL, "Hopen");
    status = Vstart(fid);
    CHECK_VOID(status, FAIL, "Vstart");

    /* Create NUM_VGROUPS vgroups and set classname */
    for (ii = 0; ii < NUM_VGROUPS; ii++)
    {
	/* Create a vgroup. */
	vgroup_id = Vattach(fid, -1, "w");
	CHECK_VOID(vgroup_id, FAIL, "Vattach");

	/* Record its reference number for later access */
	vgroup_ref = VQueryref(vgroup_id);
	CHECK_VOID(vgroup_ref, FAIL, "VQueryref:vgroup_id");
	ref_list[ii] = vgroup_ref;

	/* Set its class name */
	sprintf(vgclass, "VG-CLASS-%d", ii);
	status = Vsetclass(vgroup_id, vgclass);
	CHECK_VOID(status, FAIL, "Vsetclass");

	/* Detach it */
	status = Vdetach(vgroup_id);
	CHECK_VOID(status, FAIL, "Vdetach");
    }

    /* Create NUM_VDATAS vgroups and set classname */
    for (ii = 0; ii < NUM_VDATAS; ii++)
    {
	/* Create a vdata. */
	vdata_id = VSattach(fid, -1, "w");
	CHECK_VOID(vdata_id, FAIL, "VSattach");

	/* Record its reference number for later access */
	vdata_ref = VSQueryref(vdata_id);
	CHECK_VOID(vdata_ref, FAIL, "VSQueryref:vdata_id");
	vdref_list[ii] = vdata_ref;

	/* Set its class name */
	sprintf(vgclass, "VS-CLASS-%d", ii);
	status = VSsetclass(vdata_id, vgclass);
	CHECK_VOID(status, FAIL, "VSsetclass");

	/* Detach it */
	status = VSdetach(vdata_id);
	CHECK_VOID(status, FAIL, "VSdetach");
    }

    /* Insert some vdatas/vgroups into some other vgroups to build some sort of
	vgroup/vdata structure */

    /* Insert "VD-CLASS-1" and "VD-CLASS-2" into "VG-CLASS-0" */
    vgroup0_id = Vattach(fid, ref_list[0], "w");  /* "VG-CLASS-0" */
    CHECK_VOID(vgroup0_id, FAIL, "Vattach");
    vdata1_id = VSattach(fid, vdref_list[1], "w");  /* "VD-CLASS-1" */
    CHECK_VOID(vdata1_id, FAIL, "VSattach");
    vdata2_id = VSattach(fid, vdref_list[2], "w");  /* "VD-CLASS-2" */
    CHECK_VOID(vdata2_id, FAIL, "VSattach");

    status = Vinsert(vgroup0_id, vdata1_id);
    CHECK_VOID(status, FAIL, "Vinsert vdata1_id into vgroup0_id");
    status = Vinsert(vgroup0_id, vdata2_id);
    CHECK_VOID(status, FAIL, "Vinsert vdata2_id into vgroup0_id");

    /* Insert "VG-CLASS-7" and "VG-CLASS-9" into "VG-CLASS-0" */
    vgroup7_id = Vattach(fid, ref_list[7], "w");
    CHECK_VOID(vgroup7_id, FAIL, "Vattach");
    vgroup9_id = Vattach(fid, ref_list[9], "w");
    CHECK_VOID(vgroup9_id, FAIL, "Vattach");

    status = Vinsert(vgroup0_id, vgroup7_id);
    CHECK_VOID(status, FAIL, "Vinsert vgroup7_id into vgroup0_id");
    status = Vinsert(vgroup0_id, vgroup9_id);
    CHECK_VOID(status, FAIL, "Vinsert vgroup9_id into vgroup0_id");

    /* Insert "VD-CLASS-3", "VD-CLASS-4", and "VD-CLASS-5" into "VG-CLASS-9" */
    vdata3_id = VSattach(fid, vdref_list[3], "w");
    CHECK_VOID(vdata3_id, FAIL, "Vattach");
    vdata4_id = VSattach(fid, vdref_list[4], "w");
    CHECK_VOID(vdata4_id, FAIL, "Vattach");
    vdata5_id = VSattach(fid, vdref_list[5], "w");
    CHECK_VOID(vdata4_id, FAIL, "Vattach");

    status = Vinsert(vgroup9_id, vdata3_id);
    CHECK_VOID(status, FAIL, "Vinsert vdata3_id into vgroup9_id");
    status = Vinsert(vgroup9_id, vdata4_id);
    CHECK_VOID(status, FAIL, "Vinsert vdata4_id into vgroup9_id");
    status = Vinsert(vgroup9_id, vdata5_id);
    CHECK_VOID(status, FAIL, "Vinsert vdata5_id into vgroup9_id");

    /* Insert "VG-CLASS-4", "VG-CLASS-6", and "VD-CLASS-7" into "VG-CLASS-1" */
    vgroup1_id = Vattach(fid, ref_list[1], "w");
    CHECK_VOID(vgroup1_id, FAIL, "Vattach");
    vgroup4_id = Vattach(fid, ref_list[4], "w");
    CHECK_VOID(vgroup4_id, FAIL, "Vattach");
    vgroup6_id = Vattach(fid, ref_list[6], "w");
    CHECK_VOID(vgroup6_id, FAIL, "Vattach");
    vdata7_id = VSattach(fid, vdref_list[7], "w");
    CHECK_VOID(vdata7_id, FAIL, "VSattach");

    status = Vinsert(vgroup1_id, vgroup4_id);
    CHECK_VOID(status, FAIL, "Vinsert vgroup4_id into vgroup1_id");
    status = Vinsert(vgroup1_id, vgroup6_id);
    CHECK_VOID(status, FAIL, "Vinsert vgroup6_id into vgroup1_id");
    status = Vinsert(vgroup1_id, vdata7_id);
    CHECK_VOID(status, FAIL, "Vinsert vdata7_id into vgroup1_id");

    /* Insert "VD-CLASS-6", "VG-CLASS-2" into "VG-CLASS-6" */
    vdata6_id = VSattach(fid, vdref_list[6], "w");
    CHECK_VOID(vdata6_id, FAIL, "VSattach");
    vgroup2_id = Vattach(fid, ref_list[2], "w");
    CHECK_VOID(vgroup2_id, FAIL, "Vattach");

    status = Vinsert(vgroup6_id, vgroup2_id);
    CHECK_VOID(status, FAIL, "Vinsert vgroup2_id into vgroup6_id");
    status = Vinsert(vgroup6_id, vdata6_id);
    CHECK_VOID(status, FAIL, "Vinsert vdata6_id into vgroup6_id");

    status = Vdetach(vgroup0_id);
    CHECK_VOID(status, FAIL, "Vdetach vgroup0_id");
    status = Vdetach(vgroup1_id);
    CHECK_VOID(status, FAIL, "Vdetach vgroup1_id");
    status = Vdetach(vgroup2_id);
    CHECK_VOID(status, FAIL, "Vdetach vgroup2_id");
    status = Vdetach(vgroup4_id);
    CHECK_VOID(status, FAIL, "Vdetach vgroup4_id");
    status = Vdetach(vgroup6_id);
    CHECK_VOID(status, FAIL, "Vdetach vgroup6_id");
    status = Vdetach(vgroup7_id);
    CHECK_VOID(status, FAIL, "Vdetach vgroup7_id");
    status = Vdetach(vgroup9_id);
    CHECK_VOID(status, FAIL, "Vdetach vgroup9_id");

    status = VSdetach(vdata1_id);
    CHECK_VOID(status, FAIL, "VSdetach vdata1_id");
    status = VSdetach(vdata2_id);
    CHECK_VOID(status, FAIL, "VSdetach vdata2_id");
    status = VSdetach(vdata3_id);
    CHECK_VOID(status, FAIL, "VSdetach vdata3_id");
    status = VSdetach(vdata4_id);
    CHECK_VOID(status, FAIL, "VSdetach vdata4_id");
    status = VSdetach(vdata5_id);
    CHECK_VOID(status, FAIL, "VSdetach vdata5_id");
    status = VSdetach(vdata6_id);
    CHECK_VOID(status, FAIL, "VSdetach vdata6_id");
    status = VSdetach(vdata7_id);
    CHECK_VOID(status, FAIL, "VSdetach vdata7_id");

    /* Terminate access to the V interface and close the HDF file.  */
    status_n = Vend (fid);
    CHECK_VOID(status_n, FAIL, "Vend");
    status_n = Hclose (fid);
    CHECK_VOID(status_n, FAIL, "Hclose");

    /**************************************************************
	The vgroup structure should look like this:
	       vg0	            vg1 
                |                    |
	   +--+-+-----+            / | \
           |          |           /  |  \
          /\         / \        vg4 vg6 vd7
         /  \       /   \            |
        vd1 vd2   vg7  vg9          / \  
                        |          /   \     
                      / | \       /     \     
                     /  |  \     vd6   vg2   
                  vd3  vd4  vd5
                  

    Calling Vgetvgroups on the file should return all NUM_VGROUPS vgroups
    Calling VSgetvdatas on the file should return all NUM_VDATAS vdatas
    Calling Vgetvgroups on vg0 should return 2, vg7 and vg9
    Calling VSgetvdatas on vg0 should return 2, vd1 and vd2
    Calling Vgetvgroups on vg1 should return 2, vg4 and vg6
    Calling VSgetvdatas on vg1 should return 1, vd7
    Calling Vgetvgroups on vg6 should return 1, vg2
    Calling VSgetvdatas on vg6 should return 1, vd6
    Calling Vgetvgroups on vg9 should return 0
    Calling Vgetvgroups on vg7 should return 0
    Calling VSgetvdatas on vg9 starting at 2, with n_vds=2, should return 1, vd5
    Calling Vgetvgroups on vg1 starting at 1, with n_vgs=3, should return 1, vg6

    ***************************************************************/

    /* Open the file to test Vgetvgroups and Vgetdatas */
    fid = Hopen(USERVDATAS, DFACC_RDWR, 0);
    CHECK_VOID(fid, FAIL, "Hopen");
    status = Vstart(fid);
    CHECK_VOID(status, FAIL, "Vstart");

    /* Test getting all vgroups in the file: fid, start_vg=0, n_vgs=0 */
    {
	uint16 result[] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
	status = check_vgs(fid, 0, 0, "file, 0, 0", NUM_VGROUPS, result);
	CHECK_VOID(status, FAIL, "Vgetvgroups fid");
    }

    /* Test getting all vdatas in the file: fid, start_vd=0, n_vds=0 */
    {
	uint16 result[] = {12, 13, 14, 15, 16, 17, 18, 19};
	status = check_vds(fid, 0, 0, "file, 0, 0", NUM_VDATAS, result);
	CHECK_VOID(status, FAIL, "VSgetvdatas fid");
    }

    vgroup0_id = Vattach(fid, ref_list[0], "w");  /* "VG-CLASS-0" */
    CHECK_VOID(vgroup0_id, FAIL, "Vattach vgroup0_id");

    /* Test getting vgroups in vg0: vgroup0_id, start_vd=0, n_vds=0 */
    {
	uint16 result[] = {9, 11};
	status = check_vgs(vgroup0_id, 0, 0, "vgroup0_id, 0, 0", 2, result);
	CHECK_VOID(status, FAIL, "VSgetvgroups vgroup0_id");
    }

    /* Test getting vdatas in vg0: vgroup0_id, start_vd=0, n_vds=0 */
    {
	uint16 result[] = {13, 14};
	status = check_vds(vgroup0_id, 0, 0, "vgroup0_id, 0, 0", 2, result);
	CHECK_VOID(status, FAIL, "VSgetvdatas fid");
    }

    vgroup1_id = Vattach(fid, ref_list[1], "w");  /* "VG-CLASS-1" */
    CHECK_VOID(vgroup1_id, FAIL, "Vattach vgroup1_id");

    /* Test getting vgroups in vg1: vgroup1_id, start_vd=0, n_vds=0 */
    {
	uint16 result[] = {6, 8}; /* vg4 and vg6 */
	status = check_vgs(vgroup1_id, 0, 0, "vgroup1_id, 0, 0", 2, result);
	CHECK_VOID(status, FAIL, "Vgetvgroups vgroup1_id");
    }

    /* Test getting vdatas in vg1: vgroup1_id, start_vd=0, n_vds=0 */
    {
	uint16 result[] = {19}; /* vd7 */
	status = check_vds(vgroup1_id, 0, 0, "vgroup1_id, 0, 0", 1, result);
	CHECK_VOID(status, FAIL, "VSgetvdata vgroup1_id");
    }

    vgroup6_id = Vattach(fid, ref_list[6], "w");  /* "VG-CLASS-6" */
    CHECK_VOID(vgroup6_id, FAIL, "Vattach vgroup6_id");

    /* Test getting vgroups in vg6: vgroup6_id, start_vd=0, n_vds=0 */
    {
	uint16 result[] = {4}; /* vg2 */
	status = check_vgs(vgroup6_id, 0, 0, "vgroup6_id, 0, 0", 1, result);
	CHECK_VOID(status, FAIL, "Vgetvgroups vgroup6_id");
    }

    /* Test getting vdatas in vg6: vgroup6_id, start_vd=0, n_vds=0 */
    {
	uint16 result[] = {18}; /* vd6 */
	status = check_vds(vgroup6_id, 0, 0, "vgroup6_id, 0, 0", 1, result);
	CHECK_VOID(status, FAIL, "VSgetvdata vgroup6_id");
    }

    /* Test getting vgroups in vg9: vgroup9_id, start_vd=0, n_vds=0 */
    vgroup9_id = Vattach(fid, ref_list[9], "w");  /* "VG-CLASS-9" */
    CHECK_VOID(vgroup9_id, FAIL, "Vattach vgroup9_id");
    {
	n_vgs = Vgetvgroups(vgroup9_id, 0, 0, NULL);
	CHECK_VOID(n_vgs, FAIL, "Vgetvgroups vgroup9_id");
	VERIFY_VOID(n_vgs, 0, "Vgetvgroups vgroup9_id");
    }

    /* Test getting vgroups in vg7: vgroup7_id, start_vd=0, n_vds=0 */
    vgroup7_id = Vattach(fid, ref_list[7], "w");  /* "VG-CLASS-7" */
    CHECK_VOID(vgroup7_id, FAIL, "Vattach vgroup7_id");
    {
	n_vgs = Vgetvgroups(vgroup7_id, 0, 0, NULL);
	CHECK_VOID(n_vgs, FAIL, "Vgetvgroups vgroup7_id");
	VERIFY_VOID(n_vgs, 0, "Vgetvgroups vgroup7_id");
    }

    /* Test getting vdatas in vg9: vgroup9_id, start_vd=2, n_vds=2 */
    {
	uint16 result[] = {17}; /* vd5 */
	status = check_vds(vgroup9_id, 2, 2, "vgroup9_id, 2, 2", 1, result);
	CHECK_VOID(status, FAIL, "VSgetvdata vgroup9_id");
    }

    /* Test getting vgroups in vg1: vgroup1_id, start_vd=1, n_vds=3 */
    {
	uint16 result[] = {8}; /* vg6 */
	status = check_vgs(vgroup1_id, 1, 3, "vgroup1_id, 1, 3", 1, result);
	CHECK_VOID(status, FAIL, "Vgetvgroups vgroup1_id");
    }

    status = Vdetach(vgroup0_id);
    CHECK_VOID(status, FAIL, "Vdetach vgroup0_id");
    status = Vdetach(vgroup1_id);
    CHECK_VOID(status, FAIL, "Vdetach vgroup1_id");
    status = Vdetach(vgroup6_id);
    CHECK_VOID(status, FAIL, "Vdetach vgroup6_id");
    status = Vdetach(vgroup7_id);
    CHECK_VOID(status, FAIL, "Vdetach vgroup7_id");
    status = Vdetach(vgroup9_id);
    CHECK_VOID(status, FAIL, "Vdetach vgroup9_id");

    /* Terminate access to the V interface and close the HDF file.  */
    status_n = Vend (fid);
    CHECK_VOID(status_n, FAIL, "Vend");
    status_n = Hclose (fid);
    CHECK_VOID(status_n, FAIL, "Hclose");
} /* test_getvgroups() */

/*************************** test_extfile ***************************

This test routine creates an hdf file, "tvset_ext.hdf" (excerpted and
modified from Ruth's program used in hmap project), and creates and
writes a vdata with external element.  The external file is named
Tables_External_File.

***********************************************************************/

#define EXTFILE		"tvset_ext.hdf"
#define EXTERNAL_FILE	"Tables_External_File"
#define MULTI_NAME	"Table AR with Attributes in External File"
#define CLASSMULTI_NAME	"Multi-Type, Multi-Entries per Cell, Store By Row in External File"
#define NROWS		5	/* number of records to be written to the
				   vdatas at every write */
static void
test_extfile(void) 
{
    int32   fid, vdata1_id,
	    vdata_ref = -1;  /* ref number of a vdata, set to -1 to create  */
    int32   vdata1_ref;
    int32   offset = -1, length = -1;
    char    hibuf[2] = "hi";
    char    byebuf[3] = "bye";
    char   *extfile_name;
    void*   columnPtrs[3]; 
    int	    bufsize;
    void*   databuf;
    void*   databuf2;
    intn    name_len = 0;
    intn    status_n;	/* returned status for functions returning an intn  */
    int32   status;	/* returned status for functions returning an int32 */
    char8   col1buf[NROWS][2] = { {'A', 'B'}, 
				      {'B', 'C'},
				      {'C', 'D'},
				      {'D', 'E'},
				      {'E', 'F'} };
    uint16  col2buf[NROWS] = {1, 2, 3, 4, 5};	
    float32 col3buf[NROWS][2] = { {.01, .1},
				    {.02, .2},
				    {.03, .3},
				    {.04, .4},
				    {.05, .5} };


    /* Create the HDF file for data used in this test routine */
    fid = Hopen (EXTFILE, DFACC_CREATE, 0);
    CHECK_VOID(fid, FAIL, "Hopen");

    /* Initialize the VS interface */
    status_n = Vstart (fid);
    CHECK_VOID(status_n, FAIL, "Vstart");

    /*
     * Compute the buffer size that will be needed to hold the data for the
     * mixed-data columns.   Allocate the buffers.
     */
    bufsize = (2*sizeof(char8) + sizeof(uint16) + 2*sizeof(float32)) * NROWS;
    databuf = malloc( (unsigned)bufsize );

    /* Initialize the pointers to the column data. */
    columnPtrs[0] = &col1buf[0][0];
    columnPtrs[1] = &col2buf[0];
    columnPtrs[2] = &col3buf[0][0];

    /* Create the first vdata */
    vdata1_id = VSattach(fid, vdata_ref, "w");
    CHECK_VOID(vdata1_id, FAIL, "VSattach");

    /* Set name and class name of the vdata. */
    status = VSsetname(vdata1_id, MULTI_NAME);
    CHECK_VOID(status, FAIL, "VSsetname");
    status = VSsetclass(vdata1_id, CLASSMULTI_NAME);
    CHECK_VOID(status, FAIL, "VSsetclass");

    status = VSsetexternalfile( vdata1_id, EXTERNAL_FILE, 10 );
    CHECK_VOID(status, FAIL, "VSsetexternalfile");

    /* Introduce each field's name, data type, and order.  This is the first
      part in defining a field.  */
    status_n = VSfdefine (vdata1_id, FIELD1_NAME, DFNT_CHAR8, ORDER_3);
    CHECK_VOID(status_n, FAIL, "VSfdefine");
    status_n = VSfdefine (vdata1_id, FIELD2_NAME, DFNT_UINT16, ORDER_2);
    CHECK_VOID(status_n, FAIL, "VSfdefine");
    status_n = VSfdefine (vdata1_id, FIELD3_NAME, DFNT_LFLOAT32, ORDER_3);
    CHECK_VOID(status_n, FAIL, "VSfdefine");

    /* Finalize the definition of the fields. */
    status_n = VSsetfields (vdata1_id, FIELD_NAME_LIST);
    CHECK_VOID(status_n, FAIL, "VSsetfields");

    /* Pack the buffer that will be used to write the data to the file. */
    status = VSfpack( vdata1_id, _HDF_VSPACK, NULL, databuf, bufsize, NROWS, NULL, columnPtrs);
    CHECK_VOID(status, FAIL, "VSfpack");

    /* Write to the vdata in FULL_INTERLACE */
    status = VSwrite( vdata1_id, databuf, NROWS, FULL_INTERLACE );
    CHECK_VOID(status, FAIL, "VSwrite");

    /* Add Attribute for vdata */
    status = VSsetattr( vdata1_id, _HDF_VDATA, "HDF4 Attribute Table",
                        DFNT_CHAR8, 2, &hibuf );
    CHECK_VOID(status, FAIL, "VSsetattr");

    /* Add Attribute for Column C */
    status = VSsetattr( vdata1_id, 2, "HDF4 Attribute Field 3",
                        DFNT_CHAR8, 3, &byebuf );
    CHECK_VOID(status, FAIL, "VSsetattr");

    /* Get vdata ref */
    vdata1_ref = VSQueryref(vdata1_id);
    CHECK_VOID(vdata1_ref, FAIL, "VSQueryref");

    status_n = VSdetach( vdata1_id );
    CHECK_VOID(status_n, FAIL, "VSdetach");

    HDfree(databuf);

    status_n = Vend (fid);
    CHECK_VOID(status_n, FAIL, "Vend");

    status = Hclose (fid);
    CHECK_VOID(status, FAIL, "Hclose");

    /* Reopen the file and the vdata and verify external file information */

    /* Open the HDF file */
    fid = Hopen (EXTFILE, DFACC_RDONLY, 0);
    CHECK_VOID(fid, FAIL, "Hopen");

    /* Initialize the VS interface */
    status_n = Vstart (fid);
    CHECK_VOID(status_n, FAIL, "Vstart");

    /* Create the first vdata */
    vdata1_id = VSattach(fid, vdata1_ref, "r");
    CHECK_VOID(vdata1_id, FAIL, "VSattach");

    { /* This is an old test, will be removed when VSgetexternalfile is */
    /* Get the length of the external file name first - VSgetexternalfile
       is deprecated as of 4.2.7 */
    name_len = VSgetexternalfile(vdata1_id, 0, NULL, NULL);
    VERIFY_VOID(name_len, (intn)HDstrlen(EXTERNAL_FILE), "VSgetexternalfile");

    extfile_name = (char *) HDmalloc(sizeof(char *) * (name_len+1));
    CHECK_ALLOC(extfile_name, "extfile_name", "test_extfile");
    
    /* Old function: Get the external file name - VSgetexternalfile
       is deprecated as of 4.2.7 */
    name_len = VSgetexternalfile(vdata1_id, name_len+1, extfile_name, &offset);
    VERIFY_VOID(name_len, (intn)HDstrlen(EXTERNAL_FILE), "VSgetexternalfile");
    VERIFY_CHAR_VOID(extfile_name, EXTERNAL_FILE, "VSgetexternalfile");
    HDfree(extfile_name);
    } /* old test */

    /* Get the length of the external file name first */
    name_len = VSgetexternalinfo(vdata1_id, 0, NULL, NULL, NULL);
    VERIFY_VOID(name_len, (intn)HDstrlen(EXTERNAL_FILE), "VSgetexternalinfo");

    extfile_name = (char *) HDmalloc(sizeof(char *) * (name_len+1));
    CHECK_ALLOC(extfile_name, "extfile_name", "test_extfile");
    
    /* Get the external file name */
    name_len = VSgetexternalinfo(vdata1_id, name_len+1, extfile_name, &offset, &length);
    VERIFY_VOID(name_len, (intn)HDstrlen(EXTERNAL_FILE), "VSgetexternalinfo");
    VERIFY_CHAR_VOID(extfile_name, EXTERNAL_FILE, "VSgetexternalinfo");
    HDfree(extfile_name);

    /* Test passing in smaller buffer for external file name than actual;
       name should be truncated */
    {
	/* Make a shorter string to verify later */
        char *short_name = (char *) HDmalloc(sizeof(char *) * (name_len));
        HDmemset(short_name, '\0', name_len);
        HDstrncpy(short_name, EXTERNAL_FILE, name_len-2);

	/* Prepare buffer for external file name in the following test */
	extfile_name = (char *) HDmalloc(sizeof(char *) * (name_len-1));
        HDmemset(extfile_name, '\0', name_len);

        /* Call VSgetexternalinfo again with smaller buffer size and make sure
           VSgetexternalinfo reads the name truncated to the given buffer size*/
        name_len = VSgetexternalinfo(vdata1_id, name_len-2, extfile_name, &offset, &length);
        VERIFY_VOID(name_len, (intn)HDstrlen(extfile_name), "VSgetexternalinfo");
        VERIFY_CHAR_VOID(extfile_name, short_name, "VSgetexternalinfo");
        HDfree(short_name);
        HDfree(extfile_name);
    }

    /* Release resources */
    status_n = VSdetach( vdata1_id );
    CHECK_VOID(status_n, FAIL, "VSdetach");

    status_n = Vend (fid);
    CHECK_VOID(status_n, FAIL, "Vend");

    status = Hclose (fid);
    CHECK_VOID(status, FAIL, "Hclose");

} /* test_extfile() */

/****************************************************************************
   Name: test_blockinfo_oneLB() - tests setting/getting block info in the
                           simple case, only one linked block storage occur

   Description:
	The test does the following steps:
	- Create the file and the first vdata, "Appendable Vdata"
	- Test calling VSsetblocksize with invalid values and verify that failures occur
	- Set block size and number of blocks to BLOCK_SIZE1 and NUM_BLOCKS for this vdata
	- Close the vdata and the file
	- Reopen the file and the first vdata, "Appendable Vdata"
	- Verify that the block size of this vdata remains the default,
  	  HDF_APPENDABLE_BLOCK_LEN.  This shows when no data and no linked-block
  	  storage created, the block size remains as the default.
	- Call VSsetblocksize again with BLOCK_SIZE1 and NUM_BLOCKS
	- Write 5 records to this vdata
	- Create and write 5 records to the second vdata, "Another Vdata".  The
  	  purpose of this second vdata is to cause the subsequent write to the
  	  first vdata to promote the first vdata's storage to a linked-block element.
	- Append 5 records to the first vdata. 
	- Verify that the block size is changed to BLOCK_SIZE1 and number of
  	  blocks to NUM_BLOCKS
	- Close both vdatas and the file.

   BMR - Jan 2014
 ****************************************************************************/
#define APPENDABLE_VD	"Appendable Vdata"
#define ANOTHER_VD	"Another Vdata"
#define CLASS_NAME	"Linked-block Vdata"
#define ANOTHER_FD	"Another field"	/* contains one integer */
#define	ANOTHER_FD_LIST	"Another field"
#define N_RECORDS	5	/* number of records to be written to the
				   vdatas at every write */
#define N_VALS_PER_REC_2   1    /* # of values per record in the 2nd vdata */
#define N_VALS_PER_REC_1 (ORDER_1+ORDER_2+ORDER_3) /* # of vals/rec. in 1st vd*/
#define	BLOCK_SIZE1	20	/* arbitrary number for block size */
#define	BLOCK_SIZE2	100	/* arbitrary number for block size */
#define	NUM_BLOCKS	8	/* arbitrary number for number of blocks */

static void
test_blockinfo_oneLB(void) 
{
    intn  status_n;	/* returned status for functions returning an intn  */
    int32 status;	/* returned status for functions returning an int32 */
    int16 rec_num;	/* current record number */
    int32 record_pos;
    int32 fid, vdata1_id, vdata2_id,
	  vdata_ref = -1,  /* ref number of a vdata, set to -1 to create  */
   	  num_of_records,  /* number of records actually written to vdata */
          data_buf1[N_RECORDS][N_VALS_PER_REC_1], /* for first vdata's data */
	  data_buf2[N_RECORDS][N_VALS_PER_REC_2], /* for second vdata's data */
	  block_size, num_blocks; /* retrieved by VSgetblockinfo */
    intn  n_vds = 0;
    uint16 *refarray = NULL;

    /* Create the HDF file for data used in this test routine */
    fid = Hopen (LKBLK_FILE, DFACC_CREATE, 0);
    CHECK_VOID(fid, FAIL, "Hopen");

    /* Initialize the VS interface */
    status_n = Vstart (fid);
    CHECK_VOID(status_n, FAIL, "Vstart");

    /* Create the first vdata */
    vdata1_id = VSattach(fid, vdata_ref, "w");
    CHECK_VOID(vdata1_id, FAIL, "VSattach");

    /* Set name and class name of the vdata. */
    status = VSsetname (vdata1_id, APPENDABLE_VD);
    CHECK_VOID(status, FAIL, "VSsetname");
    status = VSsetclass (vdata1_id, CLASS_NAME);
    CHECK_VOID(status, FAIL, "VSsetclass");

    /* Introduce each field's name, data type, and order.  This is the first
      part in defining a field.  */
    status_n = VSfdefine (vdata1_id, FIELD1_NAME, DFNT_INT32, ORDER_1);
    CHECK_VOID(status_n, FAIL, "VSfdefine");
    status_n = VSfdefine (vdata1_id, FIELD2_NAME, DFNT_INT32, ORDER_2);
    CHECK_VOID(status_n, FAIL, "VSfdefine");
    status_n = VSfdefine (vdata1_id, FIELD3_NAME, DFNT_INT32, ORDER_3);
    CHECK_VOID(status_n, FAIL, "VSfdefine");

    /* Finalize the definition of the fields. */
    status_n = VSsetfields (vdata1_id, FIELD_NAME_LIST);
    CHECK_VOID(status_n, FAIL, "VSsetfields");

    /* 
     * Buffer the data by the record for fully interlaced mode.  Note that the
     * first three elements contain the three values of the first field, the
     * fourth element contains the value of the second field, and the last two
     * elements contain the two values of the third field.
     */
    for (rec_num = 0; rec_num < N_RECORDS; rec_num++)
    {
        data_buf1[rec_num][0] = 1 + rec_num;
        data_buf1[rec_num][1] = 2 + rec_num;
        data_buf1[rec_num][2] = 3 + rec_num;
        data_buf1[rec_num][3] = 10 + rec_num;
        data_buf1[rec_num][4] = 10;
        data_buf1[rec_num][5] = 65;
    }

    status_n = VSgetblockinfo (vdata1_id, &block_size, NULL);
    CHECK_VOID(status_n, FAIL, "VSgetblockinfo");
    VERIFY_VOID(block_size, HDF_APPENDABLE_BLOCK_LEN, "VSgetblockinfo");

    /* Test for invalid arguments passed in these functions */
    status_n = VSsetblocksize(vdata1_id, -2);
    VERIFY_VOID(status_n, FAIL, "VSsetblocksize");
    status_n = VSsetnumblocks(vdata1_id, 0);
    VERIFY_VOID(status_n, FAIL, "VSsetnumblocks");

    /* Set the block size and the number of blocks the first vdata */
    status_n = VSsetblocksize(vdata1_id, BLOCK_SIZE1);
    CHECK_VOID(status_n, FAIL, "VSsetblocksize");
    status_n = VSsetnumblocks(vdata1_id, NUM_BLOCKS);
    CHECK_VOID(status_n, FAIL, "VSsetnumblocks");

    status = VSdetach (vdata1_id);
    CHECK_VOID(status, FAIL, "Vdetach");

    status_n = Vend (fid);
    CHECK_VOID(status_n, FAIL, "Vend");

    status = Hclose (fid);
    CHECK_VOID(status, FAIL, "Hclose");

    /******************************************************************
     * Reopen the file, and the vdata APPENDABLE_VDATA, verify that block
     * size is still the default, then set block size again, and attempt
     * to write data and test for block information.
     ******************************************************************/

    fid = Hopen(LKBLK_FILE, DFACC_RDWR, 0);
    CHECK_VOID(fid, FAIL, "Hopen");

    /* Initialize the VS interface */
    status_n = Vstart (fid);
    CHECK_VOID(status_n, FAIL, "Vstart");

    vdata_ref = -1;
    vdata_ref = VSfind(fid, APPENDABLE_VD);
    CHECK_VOID(vdata_ref, FAIL, "VSfind");

    vdata1_id = VSattach(fid, vdata_ref, "w");
    CHECK_VOID(vdata1_id, FAIL, "VSattach");

    status_n = VSgetblockinfo (vdata1_id, &block_size, NULL);
    CHECK_VOID(status_n, FAIL, "VSgetblockinfo");
    VERIFY_VOID(block_size, HDF_APPENDABLE_BLOCK_LEN, "VSgetblockinfo");

    /* Set the block size and the number of blocks the first vdata */
    status_n = VSsetblocksize(vdata1_id, BLOCK_SIZE1);
    CHECK_VOID(status_n, FAIL, "VSsetblocksize");
    status_n = VSsetnumblocks(vdata1_id, NUM_BLOCKS);
    CHECK_VOID(status_n, FAIL, "VSsetnumblocks");

    /* Write the data from data_buf1 to the vdata with full interlacing mode. */
    num_of_records = VSwrite(vdata1_id, (uint8 *)data_buf1, N_RECORDS, 
				FULL_INTERLACE);
    VERIFY_VOID(num_of_records, N_RECORDS, "VSwrite:vdata1_id");

    /* The block size should change properly */
    status_n = VSgetblockinfo (vdata1_id, &block_size, &num_blocks);
    CHECK_VOID(status_n, FAIL, "VSgetblockinfo");
    VERIFY_VOID(block_size, BLOCK_SIZE1, "VSgetblockinfo");

    /******************************************************************
     * Creates and writes another vdata right after APPENDABLE_VDATA.
     * This will cause the storage of APPENDABLE_VDATA to be promoted to a
     * linked-block element if a subsequent write to APPENDABLE_VDATA occurs. 
     ******************************************************************/
 
    /* Create another vdata. */
    vdata_ref = -1;
    vdata2_id = VSattach(fid, vdata_ref, "w");
    CHECK_VOID(vdata2_id, FAIL, "VSattach");

    /* Set name and class name of the vdata. */
    status = VSsetname (vdata2_id, ANOTHER_VD);
    CHECK_VOID(status, FAIL, "VSsetname");
    status = VSsetclass (vdata2_id, CLASS_NAME);
    CHECK_VOID(status, FAIL, "VSsetclass");

    /* Define the vdata's field. */
    status_n = VSfdefine (vdata2_id, ANOTHER_FD, DFNT_INT32, ORDER_2);
    CHECK_VOID(status_n, FAIL, "VSfdefine");
    status_n = VSsetfields (vdata2_id, ANOTHER_FD_LIST);
    CHECK_VOID(status_n, FAIL, "VSsetfields");

    /* Buffer the data for ANOTHER_VDATA */
    for (rec_num = 0; rec_num < N_RECORDS; rec_num++)
    {
        data_buf2[rec_num][0] = 100 + rec_num;
    }

    /* Write the data from data_buf2 to the second vdata with full 
       interlacing mode. */
    num_of_records = VSwrite(vdata2_id, (uint8 *)data_buf2, N_RECORDS, 
				FULL_INTERLACE);
    VERIFY_VOID(num_of_records, N_RECORDS, "VSwrite:vdata2_id");

    /******************************************************************
     * Writes more data to APPENDABLE_VDATA, i.e. first vdata.  Its
     * storage will be promoted to a linked-block element.
     ******************************************************************/

    for (rec_num = 0; rec_num < N_RECORDS; rec_num++)
    {
        data_buf1[rec_num][0] = 10 + rec_num;
        data_buf1[rec_num][1] = 20 + rec_num;
        data_buf1[rec_num][2] = 30 + rec_num;
        data_buf1[rec_num][3] = 100 + rec_num;
        data_buf1[rec_num][4] = 100;
        data_buf1[rec_num][5] = 650;
    }

    /* Append the data to the first vdata. */
    record_pos = VSseek(vdata1_id, N_RECORDS);
    num_of_records = VSwrite(vdata1_id, (uint8 *)data_buf1, N_RECORDS, 
				FULL_INTERLACE); 
    VERIFY_VOID(num_of_records, N_RECORDS, "VSwrite:vdata1_id");

    /* Retrieve the first vdata's block size and number of blocks and 
       verify them */
    status_n = VSgetblockinfo (vdata1_id, &block_size, &num_blocks);
    CHECK_VOID(status_n, FAIL, "VSgetblockinfo");
    VERIFY_VOID(block_size, BLOCK_SIZE1, "VSgetblockinfo");
    VERIFY_VOID(num_blocks, NUM_BLOCKS, "VSgetblockinfo");

    /* Terminate access to the vdatas and to the VS interface, then 
       close the HDF file. */
    status = VSdetach (vdata1_id);
    CHECK_VOID(status, FAIL, "Vdetach");

    status = VSdetach (vdata2_id);
    CHECK_VOID(status, FAIL, "Vdetach");

    /* Terminate access to the V interface and close the file. */
    status_n = Vend (fid);
    CHECK_VOID(status_n, FAIL, "Vend");
    status = Hclose (fid);
    CHECK_VOID(status, FAIL, "Hclose");

} /* test_blockinfo_oneLB() */


/****************************************************************************
   Name: test_blocksize_multLBs() - tests setting block info in the case of
                           multiple linked block element

   Description:
	The test opens the file again, open the first vdata, "Appendable Vdata",
	- Open the file and the first vdata, APPENDABLE_VD.
	- Verify that the block size had changed to BLOCK_SIZE1 and number of
  	  blocks to NUM_BLOCKS
	- Attempt to change the block size to something else, and verify that it
  	  is not changed, then close the vdata.
	- Open the second vdata, ANOTHER_VD.
	- Verify that the block size and number of blocks are the default values,
  	  HDF_APPENDABLE_BLOCK_LEN and HDF_APPENDABLE_BLOCK_NUM.
	- Change block size to BLOCK_SIZE2
	- Append 5 records to ANOTHER_VD, which triggers its storage to be
  	  converted into a linked-block element.
	- Close the vdata and the file.
	- Open the file to verify block info stay changed.
   BMR - Jan 2014
 ****************************************************************************/
static void
test_blockinfo_multLBs(void) 
{
    intn  status_n;	/* returned status for functions returning an intn  */
    int32 status;	/* returned status for functions returning an int32 */
    int16 rec_num;	/* current record number */
    int32 record_pos;
    int32 fid, vdata1_id, vdata2_id,
	  vdata_ref = -1,  /* ref number of a vdata, set to -1 to create  */
   	  num_of_records,  /* number of records actually written to vdata */
          data_buf1[N_RECORDS][N_VALS_PER_REC_1], /* for first vdata's data */
	  data_buf2[N_RECORDS][N_VALS_PER_REC_2], /* for second vdata's data */
	  block_size, num_blocks; /* retrieved by VSgetblockinfo */
    intn  n_vds = 0;
    uint16 *refarray = NULL;

    /******************************************************************
     * Reopen the file, and the vdata APPENDABLE_VDATA, then append more
     * data and test for block information.
     ******************************************************************/

    fid = Hopen(LKBLK_FILE, DFACC_RDWR, 0);
    CHECK_VOID(fid, FAIL, "Hopen");

    /* Initialize the VS interface */
    status_n = Vstart (fid);
    CHECK_VOID(status_n, FAIL, "Vstart");

    /* Locate and open vdata APPENDABLE_VD */
    vdata_ref = -1;
    vdata_ref = VSfind(fid, APPENDABLE_VD);
    CHECK_VOID(vdata_ref, FAIL, "VSfind");

    vdata1_id = VSattach(fid, vdata_ref, "w");
    CHECK_VOID(vdata1_id, FAIL, "VSattach");

    /* Retrieve the first vdata's block size and number of blocks and 
       verify them again.  This used to return the old value. (HDFFR-1357) */
    status_n = VSgetblockinfo (vdata1_id, &block_size, &num_blocks);
    CHECK_VOID(status_n, FAIL, "VSsetfields");
    VERIFY_VOID(block_size, BLOCK_SIZE1, "VSgetblockinfo");
    VERIFY_VOID(num_blocks, NUM_BLOCKS, "VSgetblockinfo");

    /* Set the block size to the first vdata again, but this is when vdata
       already has a linked-block element.  If the block size is different
       than the previously specified block size, then the change will not
       take effect. */
    status_n = VSsetblocksize(vdata1_id, BLOCK_SIZE1+100);
    CHECK_VOID(status_n, FAIL, "VSsetblocksize");

    /* Retrieve the first vdata's block size and verify that it did not
       change */
    status_n = VSgetblockinfo (vdata1_id, &block_size, NULL);
    CHECK_VOID(status_n, FAIL, "VSgetblockinfo");
    VERIFY_VOID(block_size, BLOCK_SIZE1, "VSgetblockinfo");

    /* Close the vdata */
    status = VSdetach (vdata1_id);
    CHECK_VOID(status, FAIL, "Vdetach");

    /* Next test:
	Open the second vdata, ANOTHER_VD, check block size, set block
	size, write to it, then check block size again */

    /* Locate and open the vdata ANOTHER_VD */
    vdata_ref = -1;
    vdata_ref = VSfind(fid, ANOTHER_VD);
    CHECK_VOID(vdata_ref, FAIL, "VSfind");
    vdata2_id = VSattach(fid, vdata_ref, "w");
    CHECK_VOID(vdata2_id, FAIL, "VSattach");

    /* Check block size of ANOTHER_VD */
    status_n = VSgetblockinfo (vdata2_id, &block_size, &num_blocks);
    VERIFY_VOID(block_size, HDF_APPENDABLE_BLOCK_LEN, "VSgetblockinfo");

    /* Set the block size to ANOTHER_VD to BLOCK_SIZE2 */
    status_n = VSsetblocksize(vdata2_id, BLOCK_SIZE2);
    CHECK_VOID(status_n, FAIL, "VSsetblocksize");

    /* Retrieve the block size of ANOTHER_VD */
    status_n = VSgetblockinfo (vdata2_id, &block_size, NULL);
    CHECK_VOID(status_n, FAIL, "VSgetblockinfo");
    VERIFY_VOID(block_size, BLOCK_SIZE2, "VSgetblockinfo");

    /* Buffer the data for ANOTHER_VDATA */
    for (rec_num = 0; rec_num < N_RECORDS; rec_num++)
    {
        data_buf2[rec_num][0] = 200 + rec_num;
    }

    /* Write the data from data_buf2 to the vdata with full interlacing mode. */
    record_pos = VSseek(vdata2_id, N_RECORDS);
    num_of_records = VSwrite(vdata2_id, (uint8 *)data_buf2, N_RECORDS, 
				FULL_INTERLACE);
    VERIFY_VOID(num_of_records, N_RECORDS, "VSwrite:vdata2_id");

    /* Retrieve the first vdata's block size and number of blocks and 
       verify them */
    status_n = VSgetblockinfo (vdata2_id, &block_size, &num_blocks);
    CHECK_VOID(status_n, FAIL, "VSsetfields");
    VERIFY_VOID(block_size, BLOCK_SIZE2, "VSgetblockinfo");

    /* Close the vdata and file */
    status = VSdetach (vdata2_id);
    CHECK_VOID(status, FAIL, "Vdetach");
    status_n = Vend (fid);
    CHECK_VOID(status_n, FAIL, "Vend");
    status = Hclose (fid);
    CHECK_VOID(status, FAIL, "Hclose");

    /*
     * Open the file one more time and verify the block size and number
     * of blocks of the vdatas that have linked-block storage.
     */
    /* Open file again. */
    fid = Hopen(LKBLK_FILE, DFACC_RDONLY, 0);
    CHECK_VOID(fid, FAIL, "Hopen");

    /* Initialize the VS interface */
    status_n = Vstart (fid);
    CHECK_VOID(status_n, FAIL, "Vstart");

    /* Locate and open vdata APPENDABLE_VD. */
    vdata_ref = -1;
    vdata_ref = VSfind(fid, APPENDABLE_VD);
    CHECK_VOID(vdata_ref, FAIL, "VSfind");
    vdata1_id = VSattach(fid, vdata_ref, "r");
    CHECK_VOID(vdata1_id, FAIL, "VSattach");

    /* Verify its block size and number of blks. */
    status_n = VSgetblockinfo (vdata1_id, &block_size, &num_blocks);
    CHECK_VOID(status_n, FAIL, "VSsetfields");
    VERIFY_VOID(block_size, BLOCK_SIZE1, "VSgetblockinfo");
    VERIFY_VOID(num_blocks, NUM_BLOCKS, "VSgetblockinfo");

    /* Close APPENDABLE_VD. */
    status = VSdetach (vdata1_id);
    CHECK_VOID(status, FAIL, "Vdetach");

    /* Locate and open vdata ANOTHER_VD, and verify its block size and number of blks. */
    vdata_ref = -1;
    vdata_ref = VSfind(fid, ANOTHER_VD);
    CHECK_VOID(vdata_ref, FAIL, "VSfind");
    vdata2_id = VSattach(fid, vdata_ref, "r");
    CHECK_VOID(vdata2_id, FAIL, "VSattach");

    /* Verify its block size and number of blks. */
    status_n = VSgetblockinfo (vdata2_id, &block_size, &num_blocks);
    CHECK_VOID(status_n, FAIL, "VSsetfields");
    VERIFY_VOID(block_size, BLOCK_SIZE2, "VSgetblockinfo");
    VERIFY_VOID(num_blocks, HDF_APPENDABLE_BLOCK_NUM, "VSgetblockinfo");

    /* Close ANOTHER_VD and the file. */
    status = VSdetach (vdata2_id);
    CHECK_VOID(status, FAIL, "Vdetach");
    status_n = Vend (fid);
    CHECK_VOID(status_n, FAIL, "Vend");
    status = Hclose (fid);
    CHECK_VOID(status, FAIL, "Hclose");

} /* test_blockinfo_multLBs() */

/*
 * This test is here to use the file LKBLK_FILE, but it will be moved to
 * another file in the future when more organization is done for tests.
 * This function tests that VSofclass gets the reference numbers of the
 * vdatas belonging to a class.
 */
static void
test_VSofclass()
{
    intn  status_n;	/* returned status for functions returning an intn  */
    int32 status;	/* returned status for functions returning an int32 */
    int16 rec_num;	/* current record number */
    int32 record_pos;
    int32 fid, vdata1_id, vdata2_id,
	  vdata_ref = -1,  /* ref number of a vdata, set to -1 to create  */
   	  num_of_records,  /* number of records actually written to vdata */
          data_buf1[N_RECORDS][N_VALS_PER_REC_1], /* for first vdata's data */
	  data_buf2[N_RECORDS][N_VALS_PER_REC_2], /* for second vdata's data */
	  block_size, num_blocks; /* retrieved by VSgetblockinfo */
    intn  n_vds = 0;
    uint16 *refarray = NULL;

    /* Open file LKBLK_FILE for reading. */
    fid = Hopen(LKBLK_FILE, DFACC_RDONLY, 0);
    CHECK_VOID(fid, FAIL, "Hopen");

    /* Initialize the VS interface */
    status_n = Vstart (fid);
    CHECK_VOID(status_n, FAIL, "Vstart");

    /* VSofclass returns the number of vdatas belonging to CLASS_NAME correctly */
    n_vds = VSofclass(fid, CLASS_NAME, 0, 0, NULL);
    VERIFY_VOID(n_vds, 2, "VSofclass");

    /* Allocate space for the ref array to pass into VSofclass. */
    refarray = (uint16 *)HDmalloc(sizeof(uint16) * n_vds);
    CHECK_ALLOC(refarray, "refarray", "test_blockinfo_multLBs" );

    /* The following tests rely on the reference numbers of the two vdatas of
       class CLASS_NAME.  If data is added to the file before these vdatas,
       the reference numbers (2 and 3 below) need to be adjusted accordingly or
       tests will fail -BMR (will have a better tests later) */
    n_vds = VSofclass(fid, CLASS_NAME, 0, n_vds, refarray);
    VERIFY_VOID(refarray [0], 2, "VSofclass");
    VERIFY_VOID(refarray [1], 3, "VSofclass");

    refarray [0] = refarray[1] = 0;
    n_vds = VSofclass(fid, CLASS_NAME, 0, 1, refarray);
    VERIFY_VOID(refarray [0], 2, "VSofclass");
    VERIFY_VOID(refarray [1], 0, "VSofclass");

    n_vds = VSofclass(fid, CLASS_NAME, 1, n_vds, refarray);
    VERIFY_VOID(refarray [0], 3, "VSofclass");
    VERIFY_VOID(refarray [1], 0, "VSofclass");

    /* Terminate access to the V interface and close the file. */
    status_n = Vend (fid);
    CHECK_VOID(status_n, FAIL, "Vend");
    status = Hclose (fid);
    CHECK_VOID(status, FAIL, "Hclose");

} /* test_VSofclass */

/*************************** test_blockinfo ***************************

   Name: ttest_blockinfo() - tests setting/getting block info for a vdata

   Description:
   This test creates an hdf file, "Block_info.hdf", and creates and
   writes two vdatas in a way that one of the vdatas, then later the
   other one, will be promoted to linked-block elements.

   The first vdata is named "Appendable Vdata", and belongs to a class,
   named "Linked-block Vdata".  The fields of the vdata include "Field1",
   "Field2", and "Field3" and all data are integer.  "Field1" has an order
   of 3, "Field2" has an order of 1, and "Field3" has an order of 2.

   The second vdata named "Another Vdata", and also belongs to class
   "Linked-block Vdata".  This vdata has only one field of order 1 and
   its data are integer.

   test_blockinfo is divided into two parts:
	test_blockinfo_oneLB
	test_blockinfo_multLBs

***********************************************************************/
    /* This test is perhaps more suitable somewhere else or in a file by
       itself, but keep it here for now since we're running out of time...*/
void
test_blockinfo(void)
{
    int32 status;

    /* test the case of setting block size doesn't have effect until linked-
       block element is created */
    test_blockinfo_oneLB();

    /* test functionality about set/get linked-block information */
    test_blockinfo_multLBs();

    /* test VSofclass; relies on the file created and written by the tests
       in test_blockinfo_oneLB and test_blockinfo_multLBs */
    test_VSofclass();

}   /* test_blockinfo */

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

    /* test VSdelete() */
    test_vsdelete();

    /* test Vdelete() */
    test_vdelete();
   
    /* test Vdeletetagref() */
    test_vdeletetagref();

    /* test Vdatas with no fields defined */
    test_emptyvdata();

    /* test Vgroups with name and class that have more than 64 characters */
    test_vglongnames();

    /* test functionality about set/get linked-block information */
    test_blockinfo();

    /* test Vgetgroups - getting user-created vgroups */
    test_getvgroups();

    /* test VSgetvdatas - getting user-created vdatas */
    test_getvdatas();

    /* test_extfile - getting external file information */
    test_extfile();
}   /* test_vsets */

/* TODO:
   - should either making all the other test functions to return status as
     read_vset_stuff() and write_vset_stuff, or making these two void.
   - review each test function and add header/comments
   BMR - Jan 16, 2014
*/
