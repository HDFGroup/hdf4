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
#include "tproto.h"

extern int Verbocity;
extern int num_errs;

#define VDATA_COUNT  300 /* make this many Vdatas to check for memory leaks */

#define FNAME0   "tvset.hdf"
#define FNAME1   "tvset1.hdf"
#define FNAME2   "tvset2.hdf"

#define FIELD1       "FIELD_name_HERE"
#define FIELD1_UPPER "FIELD_NAME_HERE"
#define FIELD2       "DIFFERENT_FIELD_NAME"

/* write some stuff to the file */
int32 write_vset_stuff() {
    int32 status;
    int32 fid, aid;
    int32 vg1, vg2;
    int32 vs1;
    int32 count, i, j, num;
    int32   ibuf[2000]; /* integer buffer */
    float32 fbuf[2000]; /* floating point buffer */
    char    gbuf[2000]; /* generic buffer */
    char  * name;
    char  * p;
    char8   c;
    float32 f;
    
    fid = Hopen(FNAME0, DFACC_CREATE, 100);
    if(fid == FAIL) {
        num_errs++;
        return FAIL;
    }

    Vstart(fid);

    /*
     *
     * Vgroup Generation routines
     *
     */

    /* 
     *  start simple --- create a simple Vgroup
     */
    vg1 = Vattach(fid, -1, "w");
    if(vg1 == FAIL) {
        num_errs++;
        printf(">>> Failed creating initial Vgroup\n");
    }

    status = Vsetname (vg1, "Simple Vgroup");
    Vsetclass(vg1, "Test object");
    MESSAGE(5,printf("created Vgroup %s (empty)\n", "Simple Vgroup"););

    /*
     * Lets do some more complex ones now
     */
    vg2 = Vattach(fid, -1, "w");
    if(vg2 == FAIL) {
        num_errs++;
        printf(">>> Failed creating second Vgroup\n");
    }

    /* keep track of how many in Vgroup */
    num = 0;

    /* add first group into the other */
    status = Vinsert(vg2, vg1);
    if(status == FAIL) {
        num_errs++;
        printf(">>> Vinsert failed\n");
    } else
        num++;

    /* add a bogus element */
    status = Vaddtagref(vg2, (int32) 1000, (int32) 12345);
    if(status == FAIL) {
        num_errs++;
        printf(">>> Vaddtagref failed for bogus element\n");
    } else 
        num++;

    /* create an element and insert that */
    aid = Hstartwrite(fid, (uint16) 123, (uint16) 1234, 10);
    if(aid == FAIL) {
        num_errs++;
        printf(">>> Hstartwrite failed\n");
    }
    Hendaccess(aid);

    /* add an existing HDF element */
    status = Vaddtagref(vg2, (int32) 123, (int32) 1234);
    if(status == FAIL) {
        num_errs++;
        printf(">>> Vaddtagref failed for legit element\n");
    } else 
        num++;

#ifdef NO_DUPLICATES
    /* attempt to add an element already in the Vgroup */
    status = Vaddtagref(vg2, (int32) 123, (int32) 1234);
    if(status != FAIL) {
        num_errs++;
        printf(">>> Vaddtagref added a duplicate element\n");
    } 

    /* check that the number is correct */
    if(num != Vntagrefs(vg2)) {
        num_errs++;
        printf(">>> Vntagrefs returned %d was expecting %d\n", Vntagrefs(vg2), num);
    }
#endif /* NO_DUPLICATES */

    /* lets check the contents */
    /* look for a valid one first */
    if(Vinqtagref(vg2, 1000, 12345) == FALSE) {
        num_errs++;
        printf(">>> Vinqtagref couldn't find valid element\n");
    }

    /* look for a bogus one */
    if(Vinqtagref(vg2, 1000, 123456) != FALSE) {
        num_errs++;
        printf(">>> Vinqtagref found a bogus element\n");
    }

    Vsetname (vg2, "Second Vgroup");
    Vsetclass(vg2, "Test object");
        
    Vdetach(vg1);
    Vdetach(vg2);
    MESSAGE(5,printf("created Vgroup %s with %d elements\n", "Second Vgroup", num););


    /*
     *
     * Vdata Generation routines
     *
     */

    /* Float32 Vdata */
    vs1 = VSattach(fid, -1, "w");
    name = "Float Vdata";
    VSsetname (vs1, name);
    VSsetclass(vs1, "Test object");
    VSfdefine(vs1, FIELD1, DFNT_FLOAT32, 1);
    status = VSsetfields(vs1,FIELD1);
    if(status == FAIL) {
        num_errs++;
        printf(">>> Vsetfields failed for %s\n", name);
    }
    

    /* create some bogus data */
    for(i = 0, count = 100; i < count; i++) fbuf[i] = i;
    
    /* store it */
    VSwrite(vs1, (unsigned char *) fbuf, count, FULL_INTERLACE);
    VSdetach(vs1);
    MESSAGE(5,printf("created VDATA %s with %d elements\n", name, count););

    /* Int32 Vdata */
    vs1 = VSattach(fid, -1, "w");
    name = "Integer Vdata";
    VSsetname (vs1, name);
    VSsetclass(vs1, "Test object");
    VSfdefine(vs1, FIELD2, DFNT_INT32, 2);
    status = VSsetfields(vs1,FIELD2);
    if(status == FAIL) {
        num_errs++;
        printf(">>> Vsetfields failed for %s\n", name);
    }

    /* create some bogus data */
    for(i = 0, count = 100; i < 2 * count; i++) ibuf[i] = i;
    
    /* store it */
    VSwrite(vs1, (unsigned char *) ibuf, count, FULL_INTERLACE);
    VSdetach(vs1);
    MESSAGE(5,printf("created VDATA %s with %d elements\n", name, count););


    /* Int32 and Float32 Vdata */
    vs1 = VSattach(fid, -1, "w");
    name = "Mixed Vdata";
    VSsetname (vs1, name);
    VSsetclass(vs1, "No class specified");
    VSfdefine(vs1, "A", DFNT_FLOAT32, 1);
    VSfdefine(vs1, "B", DFNT_INT32, 1);
    status = VSsetfields(vs1, "A, B");
    if(status == FAIL) {
        num_errs++;
        printf(">>> Vsetfields failed for %s\n", name);
    }

    /* create some bogus data */
    p = gbuf;
    for(i = 0, count = 100; i < count; i++) {
        float32 f = i * 2;
        HDmemcpy(p, &f, sizeof(float32)); p += sizeof(float32);
        HDmemcpy(p, &i, sizeof(int32));   p += sizeof(int32);
    }
    
    /* store it */
    VSwrite(vs1, (unsigned char *) gbuf, count, FULL_INTERLACE);
    VSdetach(vs1);
    MESSAGE(5,printf("created VDATA %s with %d elements\n", name, count););


#define ST "STATION_NAME"
#define VL "VALUES"
#define FL "FLOATS"
#define MX "STATION_NAME,VALUES,FLOATS"

    /* mixed order Vdata */
    vs1 = VSattach(fid, -1, "w");
    name = "Multi-Order Vdata";
    VSsetname (vs1, name);
    VSsetclass(vs1, "No class specified");
    VSfdefine(vs1, ST, DFNT_CHAR8, 2);
    VSfdefine(vs1, VL, DFNT_INT32, 3);
    VSfdefine(vs1, FL, DFNT_FLOAT32, 1);
    status = VSsetfields(vs1, MX);
    if(status == FAIL) {
        num_errs++;
        printf(">>> Vsetfields failed for %s\n", name);
    }

    /* create some bogus data */
    p = gbuf;
    c = 'a';
    j = 0;
    f = 15.5;
    for(i = 0, count = 10; i < count; i++) {
        HDmemcpy(p, &c, sizeof(char8));   p += sizeof(char8); c++;
        HDmemcpy(p, &c, sizeof(char8));   p += sizeof(char8); c++;
        HDmemcpy(p, &j, sizeof(int32));   p += sizeof(int32); j++;
        HDmemcpy(p, &j, sizeof(int32));   p += sizeof(int32); j++;
        HDmemcpy(p, &j, sizeof(int32));   p += sizeof(int32); j++;
        HDmemcpy(p, &f, sizeof(float32)); p += sizeof(float32); f += 0.5;
    }
    
    /* store it */
    VSwrite(vs1, (unsigned char *) gbuf, count, FULL_INTERLACE);
    VSdetach(vs1);
    MESSAGE(5,printf("created VDATA %s with %d elements\n", name, count););


    /* create a whole bunch of Vdatas to check for memory leakage */
    for(i = 0; i < VDATA_COUNT; i++) {
        char name[80];
        vs1 = VSattach(fid, -1, "w");
        if(vs1 == FAIL) {
            num_errs++;
            printf(">>> Vsattach failed on loop %d\n", i);
            continue;
        }
        sprintf(name, "VdataLoop-%d", i);
        VSsetname (vs1, name);
        status = VSfdefine(vs1, "A", DFNT_CHAR8, 1);
        if(status == FAIL) {
            num_errs++;
            printf(">>> VSfdefine failed on loop %d\n", i);
            continue;
        }
        status = VSsetfields(vs1, "A");
        if(status == FAIL) {
            num_errs++;
            printf(">>> VSsetfields failed on loop %d\n", i);
            continue;
        }
        VSwrite(vs1, (unsigned char *) name, 1, FULL_INTERLACE);
        VSdetach(vs1);
    }



    Vend(fid);
    Hclose(fid);
    return SUCCEED;

} /* write_vset_stuff */


/* read everything back in and check it */
int32 read_vset_stuff() {
    int32   ibuf[2000]; /* integer buffer */
    float32 fbuf[2000]; /* floating point buffer */
    char    gbuf[2000]; /* generic buffer */
    int32   list[50];
    int32   tags[100], refs[100], tag, ref;
    char    name[512], class[512], fields[512];
    char  * p;
    int32   fid;
    int32   vg1;
    int32   vs1;
    int32   status, num, i, count, intr, sz;
    float32 fl_expected;
    int32   in_expected;
    char8   c_expected;
    

    fid = Hopen(FNAME0, DFACC_RDONLY, 0);
    if(fid == FAIL) {
        num_errs++;
        return FAIL;
    }

    Vstart(fid);


    /*
     *
     *   Verify the Vgroups
     *
     */

    /* test Vlone */
    num = 1;
    status = Vlone(fid, list, 10);
    if(status != num) {
        num_errs++;
        printf(">>> Vlone found %d was expecting %d\n", status, num);
    }
    
    /* test Vgetname and Vgetclass */
    vg1 = Vattach(fid, list[0], "r");
    if(vg1 == FAIL) {
        num_errs++;
        printf(">>> Was not able to attach (r) Vgroup %d\n", list[0]);
    }

    Vgetname (vg1, name);
    Vgetclass(vg1, class);

    if(HDstrcmp(name, "Second Vgroup")) {
        num_errs++;
        printf(">>> Got bogus Vgroup name : %s\n", name);
    }

    if(HDstrcmp(class, "Test object")) {
        num_errs++;
        printf(">>> Got bogus Vgroup class : %s\n", class);
    }

    num = 3;
    status = Vgettagrefs(vg1, tags, refs, 100);
    if(status == FAIL) {
        num_errs++;
        printf(">>> Vgettagrefs found %d was expecting %d\n", status, num);
    }

    for(i = 0; i < num; i++) {
        status = Vgettagref(vg1, i, &tag, &ref);
        if(status == FAIL) {
            num_errs++;
            printf(">>> Vgettagref failed on call %d\n", i);
        }
        
        if(tag != tags[i]) {
            num_errs++;
            printf(">>> Vgettagref Tag #%d disagrees %d %d\n", i, tag, tags[i]);
        }

        if(ref != refs[i]) {
            num_errs++;
            printf(">>> Vgettagref Ref #%d disagrees %d %d\n", i, ref, refs[i]);
        }

    }

    /* test Vgetid */
    ref = Vgetid(fid, -1);
    if(ref == FAIL) {
        num_errs++;
        printf(">>> Vgetid was unable to find first Vgroup\n");
    }

    ref = Vgetid(fid, ref);
    if(ref != list[0]) {
        num_errs++;
        printf(">>> Vgetid was unable to find second Vgroup (should have been first lone one)\n");
    }

    /*
     *
     *   Verify the Vdatas
     *
     */

    /* test VSgetid */
    ref = VSgetid(fid, -1);
    if(ref == FAIL) {
        num_errs++;
        printf(">>> VSgetid was unable to find first Vdata\n");
    }

    /* read in the first data and verify metadata and contents */
    vs1 = VSattach(fid, ref, "r");

    VSgetname (vs1, name);
    VSgetclass(vs1, class);

    if(HDstrcmp(name, "Float Vdata")) {
        num_errs++;
        printf(">>> Got bogus Vdata name (VSgetname) : %s\n", name);
    }

    if(HDstrcmp(class, "Test object")) {
        num_errs++;
        printf(">>> Got bogus Vdata class : %s\n", class);
    }

    status = VSinquire(vs1, &count, &intr, fields, &sz, name);
    if(status == FAIL) {
        num_errs++;
        printf(">>> VSinquire failed on float Vdata\n");
    }
    
    if(HDstrcmp(name, "Float Vdata")) {
        num_errs++;
        printf(">>> Got bogus Float Vdata name (VSinquire) : %s\n", name);
    }

    if(count != 100) {
        num_errs++;
        printf(">>> Got wrong count %d expecting 100\n", count);
    }

    if(sz != sizeof(float32)) {
        num_errs++;
        printf(">>> Got wrong data size %d should be sizeof(float32)\n", sz);
    }


#ifndef VDATA_FIELDS_ALL_UPPER
    if(HDstrcmp(fields, FIELD1)) {        
        num_errs++;
        printf(">>> Got bogus field name %s\n", fields);
    }
#else
    if(HDstrcmp(fields, FIELD1_UPPER)) {        
        num_errs++;
        printf(">>> Got bogus field name %s\n", fields);
    }
#endif /* VDATA_FIELDS_ALL_UPPER */    

    /* read it */
    VSsetfields(vs1, fields);
    for(i = 0; i < count; i++) fbuf[i] = 0;
    VSread(vs1, (unsigned char *) fbuf, count, FULL_INTERLACE);

    /* verify */
    for(i = 0; i < count; i++) {
        if(fbuf[i] != (float32) i) {
            num_errs++;
            printf(">>> Float value %d was expecting %d got %f\n", i, (float32) i, fbuf[i]);
        }
    }

    VSdetach(vs1);
    
    /* Move to the next one (integers) */
    ref = VSgetid(fid, ref);
    if(ref == FAIL) {
        num_errs++;
        printf(">>> VSgetid was unable to find second Vdata\n");
    }

    /* read in the first data and verify metadata and contents */
    vs1 = VSattach(fid, ref, "r");

    VSgetname (vs1, name);
    VSgetclass(vs1, class);

    if(HDstrcmp(name, "Integer Vdata")) {
        num_errs++;
        printf(">>> Got bogus Vdata name (VSgetname) : %s\n", name);
    }

    if(HDstrcmp(class, "Test object")) {
        num_errs++;
        printf(">>> Got bogus Vdata class : %s\n", class);
    }

    status = VSinquire(vs1, &count, &intr, fields, &sz, name);
    if(status == FAIL) {
        num_errs++;
        printf(">>> VSinquire failed on float Vdata\n");
    }
    
    if(HDstrcmp(name, "Integer Vdata")) {
        num_errs++;
        printf(">>> Got bogus Integer Vdata name (VSinquire) : %s\n", name);
    }

    if(count != 100) {
        num_errs++;
        printf(">>> Got wrong count %d expecting 100\n", count);
    }

    if(sz != 2 * sizeof(int32)) {
        num_errs++;
        printf(">>> Got wrong data size %d should be 2 * sizeof(int32)\n", sz);
    }

    if(HDstrcmp(fields, FIELD2)) {
        num_errs++;
        printf(">>> Got bogus field name %s\n", fields);
    }
    
    /* read it */
    VSsetfields(vs1, fields);
    for(i = 0; i < 2 * count; i++) ibuf[i] = 0;
    VSread(vs1, (unsigned char *) ibuf, count, FULL_INTERLACE);

    /* verify */
    for(i = 0; i < 2 * count; i++) {
        if(ibuf[i] != i) {
            num_errs++;
            printf(">>> Int value %d was expecting %d got %d\n", i, i, ibuf[i]);
        }
    }

    VSdetach(vs1);


    /* Move to the next one (integers + floats) */
    ref = VSgetid(fid, ref);
    if(ref == FAIL) {
        num_errs++;
        printf(">>> VSgetid was unable to find third Vdata\n");
    }

    /* read in the first data and verify metadata and contents */
    vs1 = VSattach(fid, ref, "r");

    VSgetname (vs1, name);
    VSgetclass(vs1, class);

    if(HDstrcmp(name, "Mixed Vdata")) {
        num_errs++;
        printf(">>> Got bogus Vdata name (VSgetname) : %s\n", name);
    }

    if(HDstrcmp(class, "No class specified")) {
        num_errs++;
        printf(">>> Got bogus Vdata class : %s\n", class);
    }

    status = VSinquire(vs1, &count, &intr, fields, &sz, name);
    if(status == FAIL) {
        num_errs++;
        printf(">>> VSinquire failed on float Vdata\n");
    }
    
    if(HDstrcmp(name, "Mixed Vdata")) {
        num_errs++;
        printf(">>> Got bogus Mixed Vdata name (VSinquire) : %s\n", name);
    }

    if(count != 100) {
        num_errs++;
        printf(">>> Got wrong count %d expecting 100\n", count);
    }

    if(sz != sizeof(int32) + sizeof(float32)) {
        num_errs++;
        printf(">>> Got wrong data size %d should be sizeof(int32) + sizeof(float32)\n", sz);
    }

    if(HDstrcmp(fields, "A,B")) {
        num_errs++;
        printf(">>> Got bogus field name %s\n", fields);
    }
    
    /* read it */
    VSsetfields(vs1, fields);
    for(i = 0; i < 1000; i++) gbuf[i] = 0;
    VSread(vs1, (unsigned char *) gbuf, count, FULL_INTERLACE);

    /* verify */
    p = gbuf;
    for(i = 0; i < count; i++) {
        float32 fl;
        int32   in;

        HDmemcpy(&fl, p, sizeof(float32)); p += sizeof(float32);
        HDmemcpy(&in, p, sizeof(int32));   p += sizeof(int32);
        
        if(in != i) {
            num_errs++;
            printf(">>> Mixed int value %d was expecting %d got %d\n", i, i, in);
        }

        if(fl != (float32) i * 2) {
            num_errs++;
            printf(">>> Mixed float value %d was expecting %d got %f\n", i, i, fl);
        }
    }

    VSdetach(vs1);

    /* Move to the next one (multi-order) */
    ref = VSgetid(fid, ref);
    if(ref == FAIL) {
        num_errs++;
        printf(">>> VSgetid was unable to find multi-order Vdata\n");
    }

    /* read in the first data and verify metadata and contents */
    vs1 = VSattach(fid, ref, "r");

    VSgetname (vs1, name);
    VSgetclass(vs1, class);

    if(HDstrcmp(name, "Multi-Order Vdata")) {
        num_errs++;
        printf(">>> Got bogus Vdata name (VSgetname) : %s\n", name);
    }

    if(HDstrcmp(class, "No class specified")) {
        num_errs++;
        printf(">>> Got bogus Vdata class : %s\n", class);
    }

    status = VSinquire(vs1, &count, &intr, fields, &sz, name);
    if(status == FAIL) {
        num_errs++;
        printf(">>> VSinquire failed on multi-order Vdata\n");
    }
    
    if(count != 10) {
        num_errs++;
        printf(">>> Got wrong count %d expecting 10\n", count);
    }

    if(HDstrcmp(fields, MX)) {
        num_errs++;
        printf(">>> Got bogus field name %s\n", fields);
    }
    

    /* 
     * verify - read in all fields 
     */


    /* read it */
    VSsetfields(vs1, fields);
    for(i = 0; i < 1000; i++) gbuf[i] = 0;
    VSread(vs1, (unsigned char *) gbuf, count, FULL_INTERLACE);

    p = gbuf;
    fl_expected = 15.5;
    in_expected = 0;
    c_expected = 'a';
    
    for(i = 0; i < count; i++) {
        float32 fl;
        int32   in;
        char8   c;
        

        /* read and verify characters */
        HDmemcpy(&c, p, sizeof(char8)); p += sizeof(char8);

        if(c != c_expected) {
            num_errs++;
            printf(">>> Multi-order char value %d.0 was expecting %c got %c\n", i, c_expected, c);
        }
        c_expected++;

        HDmemcpy(&c, p, sizeof(char8)); p += sizeof(char8);

        if(c != c_expected) {
            num_errs++;
            printf(">>> Multi-order char value %d.1 was expecting %c got %c\n", i, c_expected, c);
        }
        c_expected++;

        /* read and verify integers */
        HDmemcpy(&in, p, sizeof(int32));   p += sizeof(int32);
        
        if(in != in_expected) {
            num_errs++;
            printf(">>> Multi-order int value %d.0 was expecting %d got %d\n", i, in_expected, in);
        }
        in_expected++;
        HDmemcpy(&in, p, sizeof(int32));   p += sizeof(int32);
        
        if(in != in_expected) {
            num_errs++;
            printf(">>> Multi-order int value %d.1 was expecting %d got %d\n", i, in_expected, in);
        }
        in_expected++;
        HDmemcpy(&in, p, sizeof(int32));   p += sizeof(int32);
        
        if(in != in_expected) {
            num_errs++;
            printf(">>> Multi-order int value %d.2 was expecting %d got %d\n", i, in_expected, in);
        }
        in_expected++;

        /* read and verify floating point value */
        HDmemcpy(&fl, p, sizeof(float32)); p += sizeof(float32);

        if(fl != fl_expected) {
            num_errs++;
            printf(">>> Multi-order float value %d was expecting %f got %f\n", i, fl_expected, fl);
        }
        fl_expected += 0.5;

    }


    /* 
     * verify - just read in the character field with FULL_INTERLACE 
     */

    /* read it */
    VSseek(vs1, 0);
    VSsetfields(vs1, ST);
    for(i = 0; i < 1000; i++) gbuf[i] = 0;
    VSread(vs1, (unsigned char *) gbuf, count, FULL_INTERLACE);

    p = gbuf;
    c_expected = 'a';
    
    for(i = 0; i < count; i++) {
        char8   c;
        
        /* read and verify characters */
        HDmemcpy(&c, p, sizeof(char8)); p += sizeof(char8);

        if(c != c_expected) {
            num_errs++;
            printf(">>> FULL_INTERLACE read char value %d.0 (%c) got %c %d\n", i, c_expected, c, c);
        }
        c_expected++;

        HDmemcpy(&c, p, sizeof(char8)); p += sizeof(char8);

        if(c != c_expected) {
            num_errs++;
            printf(">>> FULL_INTERLACE read char value %d.1 (%c) %c got %c %d\n", i, c_expected, c, c);
        }
        c_expected++;

    }


    /* 
     * verify - just read in the character field with NO_INTERLACE 
     */

    /* read it */
    VSseek(vs1, 0);
    VSsetfields(vs1, ST);
    for(i = 0; i < 1000; i++) gbuf[i] = 0;
    VSread(vs1, (unsigned char *) gbuf, count, NO_INTERLACE);

    p = gbuf;
    c_expected = 'a';
    
    for(i = 0; i < count; i++) {
        char8   c;
        
        /* read and verify characters */
        HDmemcpy(&c, p, sizeof(char8)); p += sizeof(char8);

        if(c != c_expected) {
            num_errs++;
            printf(">>> NO_INTERLACE read char value %d.0 (%c) got %c\n", i, c_expected, c);
        }
        c_expected++;

        HDmemcpy(&c, p, sizeof(char8)); p += sizeof(char8);

        if(c != c_expected) {
            num_errs++;
            printf(">>> NO_INTERLACE read char value %d.1 (%c) %c got %c\n", i, c_expected, c);
        }
        c_expected++;

    }

    /* verify that VSfind does not mess up the AIDs of attached Vdatas */
    VSfind(fid, "foo");
    if(VSseek(vs1, 0) == FAIL) {
        num_errs++;
        printf(">>> VSseek failed after VSfind call\n");
    }

    VSdetach(vs1);

    Vend(fid);
    Hclose(fid);
    return SUCCEED;

} /* read_vset_stuff */


/* main test driver */
void test_vsets() {
    int32 status;
    
    status = write_vset_stuff();
    if(status == FAIL) return;
    
    status = read_vset_stuff();
    if(status == FAIL) return;
    
} /* test_vsets */
