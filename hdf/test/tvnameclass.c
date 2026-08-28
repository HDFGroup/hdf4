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

/*
 * Tests for vgroup's and vdata's name and class name.
 */
#include "hdf.h"
#include "hfile_priv.h"
#include "testhdf.h"
#include "tutils.h"

#define LONGNAMES    "tlongnames.hdf"
#define NONAMECLASS  "tundefined.hdf"
#define VGROUP1      "VGROUP1"
#define VG_LONGNAME  "Vgroup with more than 64 characters in length, 74 characters to be exact!"
#define VG_LONGCLASS "Very long class name to classify all Vgroups with more than 64 characters in name"

static void
test_vglongnames(void)
{
    int32  file_id = FAIL; /* File ID */
    int32  vg1     = FAIL; /* Vdata ID */
    int32  ref;            /* Vdata ref */
    size_t buf_size = 0;   /* Size for name or class buffer */
    char  *vgname = NULL, *vgclass = NULL;
    int    is_internal;
    int32  status; /* Status values from routines */

    /* Open the HDF file. */
    file_id = Hopen(LONGNAMES, DFACC_CREATE, 0);
    CHECK(file_id, FAIL, "Hopen");

    /* Initialize HDF for subsequent vgroup/vdata access. */
    status = Vstart(file_id);
    CHECK(status, FAIL, "Vstart");

    /* Create a new vgroup. */
    vg1 = Vattach(file_id, -1, "w");
    CHECK(vg1, FAIL, "VSattach");

    status = Vsetname(vg1, VG_LONGNAME);
    CHECK(status, FAIL, "VSsetname");

    status = Vsetclass(vg1, VG_LONGCLASS);
    CHECK(status, FAIL, "VSsetname");

    status = Vdetach(vg1);
    CHECK(status, FAIL, "Vdetach");
    vg1 = FAIL;

    /* Create another vgroup of the same class. */
    vg1 = Vattach(file_id, -1, "w");
    CHECK(vg1, FAIL, "VSattach");

    status = Vsetname(vg1, VGROUP1);
    CHECK(status, FAIL, "VSsetname");

    status = Vsetclass(vg1, VG_LONGCLASS);
    CHECK(status, FAIL, "VSsetname");

    status = Vdetach(vg1);
    CHECK(status, FAIL, "Vdetach");
    vg1 = FAIL;

    status = Vend(file_id);
    CHECK(status, FAIL, "Vend");

    status = Hclose(file_id);
    CHECK(status, FAIL, "Hclose");
    file_id = FAIL;

    /* Re-open the HDF file. */
    file_id = Hopen(LONGNAMES, DFACC_RDWR, 0);
    CHECK(file_id, FAIL, "Hopen");

    /* Initialize HDF for subsequent vgroup/vdata access. */
    status = Vstart(file_id);
    CHECK(status, FAIL, "Vstart");

    /* Find the long name vgroup. */
    ref = Vfind(file_id, VG_LONGNAME);
    CHECK(ref, FAIL, "VSfind");

    vg1 = Vattach(file_id, ref, "r");
    CHECK(vg1, FAIL, "VSattach");

    /* Test Vgisinternal */
    is_internal = Vgisinternal(vg1);
    CHECK(is_internal, FAIL, "Vgisinternal");
    VERIFY(is_internal, FALSE, "Vgisinternal");

    /* Get the vgroup's name */
    status = Vgetname(vg1, NULL, &buf_size);
    CHECK(status, FAIL, "Vgetname");
    VERIFY(buf_size, strlen(VG_LONGNAME), "Vgetname");

    vgname = (char *)malloc(sizeof(char) * (buf_size + 1));
    CHECK_ALLOC(vgname, "vgname", "test_vglongnames");

    buf_size++;
    status = Vgetname(vg1, vgname, &buf_size);
    CHECK(status, FAIL, "Vgetname:vg1");
    VERIFY(buf_size, strlen(VG_LONGNAME), "Vgetname");
    VERIFY_CHAR(vgname, VG_LONGNAME, "Vgetname");

    free(vgname);
    vgname = NULL;

    /* Get the vgroup's class */
    status = Vgetclass(vg1, NULL, &buf_size);
    CHECK(status, FAIL, "Vgetclass");
    VERIFY(buf_size, strlen(VG_LONGCLASS), "Vgetclass");

    vgclass = (char *)malloc(sizeof(char) * (buf_size + 1));
    CHECK_ALLOC(vgclass, "vgclass", "test_vglongnames");

    buf_size++;
    status = Vgetclass(vg1, vgclass, &buf_size);
    CHECK(status, FAIL, "Vgetclass:vg1");
    VERIFY(buf_size, strlen(VG_LONGCLASS), "Vgetclass");
    VERIFY_CHAR(vgclass, VG_LONGCLASS, "Vgetclass");

    free(vgclass);
    vgclass = NULL;

    status = Vdetach(vg1);
    CHECK(status, FAIL, "Vdetach");
    vg1 = FAIL;

    /* Find the vgroup VGROUP1. */
    ref = Vfind(file_id, VGROUP1);
    CHECK(ref, FAIL, "VSfind");

    vg1 = Vattach(file_id, ref, "r");
    CHECK(vg1, FAIL, "VSattach");

    /* Get the vgroup's name */
    status = Vgetname(vg1, NULL, &buf_size);
    CHECK(status, FAIL, "Vgetname");
    VERIFY(buf_size, strlen(VGROUP1), "Vgetname");

    vgname = (char *)malloc(sizeof(char) * (buf_size + 1));
    CHECK_ALLOC(vgname, "vgname", "test_vglongnames");

    buf_size++;
    status = Vgetname(vg1, vgname, &buf_size);
    CHECK(status, FAIL, "Vgetname:vg1");
    VERIFY(buf_size, strlen(VGROUP1), "Vgetname");
    VERIFY_CHAR(vgname, VGROUP1, "Vgetname");

    /* Should have the same class */
    status = Vgetclass(vg1, NULL, &buf_size);
    CHECK(status, FAIL, "Vgetclass");
    VERIFY(buf_size, strlen(VG_LONGCLASS), "Vgetclass");

    vgclass = (char *)malloc(sizeof(char) * (buf_size + 1));
    CHECK_ALLOC(vgclass, "vgclass", "test_vglongnames");

    buf_size++;
    status = Vgetclass(vg1, vgclass, &buf_size);
    CHECK(status, FAIL, "Vgetclass:vg1");
    VERIFY(buf_size, strlen(VG_LONGCLASS), "Vgetclass");
    VERIFY_CHAR(vgclass, VG_LONGCLASS, "Vgetclass");

    if (strcmp(vgclass, VG_LONGCLASS)) {
        num_errs++;
        printf(">>> Got bogus Vgroup class : %s\n", vgclass);
    }

    free(vgname);
    free(vgclass);
    vgname  = NULL;
    vgclass = NULL;

    status = Vdetach(vg1);
    CHECK(status, FAIL, "Vdetach");
    vg1 = FAIL;

    status = Vend(file_id);
    CHECK(status, FAIL, "Vend");

    status = Hclose(file_id);
    CHECK(status, FAIL, "Hclose");
    file_id = FAIL;

done:
    /* Release resources */
    free(vgname);
    free(vgclass);
    if (vg1 != FAIL)
        Vdetach(vg1);
    if (file_id != FAIL) {
        Vend(file_id);
        Hclose(file_id);
    }
}

static void
test_undefined(void)
{
    int32  status;         /* Status values from routines */
    int32  file_id = FAIL; /* File ID */
    int32  vg1     = FAIL; /* Vdata ID */
    int32  ref;            /* Vdata ref */
    int    is_internal;    /* to test Vgisinternal */
    size_t buf_size = 0;   /* Size for name or class buffer */

    /* Open the HDF file. */
    file_id = Hopen(NONAMECLASS, DFACC_CREATE, 0);
    CHECK(file_id, FAIL, "Hopen");

    /* Initialize HDF for subsequent vgroup/vdata access. */
    status = Vstart(file_id);
    CHECK(status, FAIL, "Vstart");

    /* Create a vgroup which will have name set but not class */
    vg1 = Vattach(file_id, -1, "w");
    CHECK(vg1, FAIL, "VSattach");

    status = Vsetname(vg1, VG_LONGNAME);
    CHECK(status, FAIL, "VSsetname");

    status = Vdetach(vg1);
    CHECK(status, FAIL, "Vdetach");
    vg1 = FAIL;

    /* Create another vgroup which will have class but not name */
    vg1 = Vattach(file_id, -1, "w");
    CHECK(vg1, FAIL, "VSattach");

    status = Vsetclass(vg1, VG_LONGCLASS);
    CHECK(status, FAIL, "VSsetname");

    status = Vdetach(vg1);
    CHECK(status, FAIL, "Vdetach");
    vg1 = FAIL;

    status = Vend(file_id);
    CHECK(status, FAIL, "Vend");

    status = Hclose(file_id);
    CHECK(status, FAIL, "Hclose");
    file_id = FAIL;

    /* Re-open the HDF file. */
    file_id = Hopen(NONAMECLASS, DFACC_RDWR, 0);
    CHECK(file_id, FAIL, "Hopen");

    /* Initialize HDF for subsequent vgroup/vdata access. */
    status = Vstart(file_id);
    CHECK(status, FAIL, "Vstart");

    /* Find the long name vgroup. */
    ref = Vfind(file_id, VG_LONGNAME);
    CHECK(ref, FAIL, "VSfind");

    vg1 = Vattach(file_id, ref, "r");
    CHECK(vg1, FAIL, "VSattach");

    /* Test Vgisinternal */
    is_internal = Vgisinternal(vg1);
    CHECK(is_internal, FAIL, "Vgisinternal");
    VERIFY(is_internal, FALSE, "Vgisinternal");

    /* Test Vgetclass on vgroup with no class */
    status = Vgetclass(vg1, NULL, &buf_size);
    CHECK(status, FAIL, "Vgetclass");
    VERIFY(buf_size, 0, "VSgetclass");

    status = Vdetach(vg1);
    CHECK(status, FAIL, "Vdetach");
    vg1 = FAIL;

    /* Find the vgroup with class VG_LONGCLASS.  This vgroup doesn't have a name */
    ref = Vfindclass(file_id, VG_LONGCLASS);
    CHECK(ref, FAIL, "VSfindclass");

    vg1 = Vattach(file_id, ref, "r");
    CHECK(vg1, FAIL, "VSattach");

    /* Test Vgetname on vgroup with no name */
    status = Vgetname(vg1, NULL, &buf_size);
    CHECK(status, FAIL, "Vgetname");
    VERIFY(buf_size, 0, "Vgetname");

    status = Vdetach(vg1);
    CHECK(status, FAIL, "Vdetach");
    vg1 = FAIL;

    status = Vend(file_id);
    CHECK(status, FAIL, "Vend");

    status = Hclose(file_id);
    CHECK(status, FAIL, "Hclose");
    file_id = FAIL;

done:
    /* Release resources */
    if (vg1 != FAIL)
        Vdetach(vg1);
    if (file_id != FAIL) {
        Vend(file_id);
        Hclose(file_id);
    }
}

/****************************************************************************
 * test_vinquire_buf_size - tests Vinquire's buf_size handling
 *   - buf_size == NULL, Vinquire should fail
 *   - a non-NULL buffer with *buf_size == 0, Vinquire should behave as
 *     a length-only query
 *   - a too-small buffer, Vinquire should truncate the name while still
 *     reporting the actual name length and correct entry count.
 ****************************************************************************/

#define INQUIRETEST "tvinquire.hdf"

static void
test_vinquire_buf_size(void)
{
    int32  file_id = FAIL; /* File ID */
    int32  vg1     = FAIL; /* Vgroup ID */
    int32  vs1     = FAIL; /* Vdata ID, used as vgroup's one entry */
    int32  ref;            /* Vgroup ref */
    int32  nentries;       /* Number of entries returned by Vinquire */
    size_t buf_size;       /* Size for name buffer */
    char   smallbuf[10];   /* deliberately too-small buffer, for truncation test */
    int32  status;         /* Status values from routines */

    /* Create the HDF file and a vgroup with one entry and a known,
       long name for the truncation test. */
    file_id = Hopen(INQUIRETEST, DFACC_CREATE, 0);
    CHECK(file_id, FAIL, "Hopen");

    status = Vstart(file_id);
    CHECK(status, FAIL, "Vstart");

    vg1 = Vattach(file_id, -1, "w");
    CHECK(vg1, FAIL, "Vattach");

    status = Vsetname(vg1, VG_LONGNAME);
    CHECK(status, FAIL, "Vsetname");

    vs1 = VSattach(file_id, -1, "w");
    CHECK(vs1, FAIL, "VSattach");

    status = Vinsert(vg1, vs1);
    CHECK(status, FAIL, "Vinsert");

    status = VSdetach(vs1);
    CHECK(status, FAIL, "VSdetach");
    vs1 = FAIL;

    status = Vdetach(vg1);
    CHECK(status, FAIL, "Vdetach");
    vg1 = FAIL;

    status = Vend(file_id);
    CHECK(status, FAIL, "Vend");

    status = Hclose(file_id);
    CHECK(status, FAIL, "Hclose");
    file_id = FAIL;

    /* Re-open the file and attach the vgroup for read-only testing. */
    file_id = Hopen(INQUIRETEST, DFACC_RDWR, 0);
    CHECK(file_id, FAIL, "Hopen");

    status = Vstart(file_id);
    CHECK(status, FAIL, "Vstart");

    ref = Vfind(file_id, VG_LONGNAME);
    CHECK(ref, FAIL, "Vfind");

    vg1 = Vattach(file_id, ref, "r");
    CHECK(vg1, FAIL, "Vattach");

    /* Tests buf_size == NULL should fail, regardless of the buffer
       pointer. */
    nentries = FAIL;
    status   = Vinquire(vg1, &nentries, smallbuf, NULL);
    VERIFY(status, FAIL, "Vinquire:NULL buf_size");

    /* Tests a non-NULL buffer with *buf_size == 0 is a length-only
       query verifies the real name length */
    smallbuf[0] = 'X'; /* to confirm the buffer is untouched */
    buf_size    = 0;
    status      = Vinquire(vg1, &nentries, smallbuf, &buf_size);
    CHECK(status, FAIL, "Vinquire:query");
    VERIFY(buf_size, strlen(VG_LONGNAME), "Vinquire:query");
    VERIFY(nentries, 1, "Vinquire:query");
    VERIFY(smallbuf[0], 'X', "Vinquire:query must not write to buffer");

    /* Tests calling Vinquire with a buffer smaller than the actual
       name. The vgroup name should be truncated, while
       buf_size still reports the actual length. */
    buf_size = sizeof(smallbuf);
    nentries = FAIL;
    status   = Vinquire(vg1, &nentries, smallbuf, &buf_size);
    CHECK(status, FAIL, "Vinquire:truncate");
    VERIFY(buf_size, strlen(VG_LONGNAME), "Vinquire:truncate");
    VERIFY(nentries, 1, "Vinquire:truncate");
    if (strlen(smallbuf) != sizeof(smallbuf) - 1) {
        num_errs++;
        printf(">>> Vinquire:truncate - name not truncated to buffer size\n");
    }
    if (strncmp(smallbuf, VG_LONGNAME, sizeof(smallbuf) - 1)) {
        num_errs++;
        printf(">>> Vinquire:truncate - truncated name doesn't match\n");
    }

    status = Vdetach(vg1);
    CHECK(status, FAIL, "Vdetach");
    vg1 = FAIL;

    status = Vend(file_id);
    CHECK(status, FAIL, "Vend");

    status = Hclose(file_id);
    CHECK(status, FAIL, "Hclose");
    file_id = FAIL;

done:
    /* Release resources */
    if (vs1 != FAIL)
        VSdetach(vs1);
    if (vg1 != FAIL)
        Vdetach(vg1);
    if (file_id != FAIL) {
        Vend(file_id);
        Hclose(file_id);
    }
}

/****************************************************************************
 * test_vgetname_vgetclass_buf_size - tests Vgetname's and Vgetclass's
 * buf_size handling
 *   - buf_size == NULL, Vgetname/Vgetclass should fail
 *   - a non-NULL buffer with *buf_size == 0, Vgetname/Vgetclass should
 *     behave as a length-only query and must not write to the buffer
 *   - a too-small buffer, Vgetname/Vgetclass should truncate the
 *     name/class while still reporting the actual length
 ****************************************************************************/

#define GETNAMETEST "tgetname.hdf"

static void
test_vgetname_vgetclass_buf_size(void)
{
    int32  file_id = FAIL; /* File ID */
    int32  vg1     = FAIL; /* Vgroup ID */
    int32  ref;            /* Vgroup ref */
    size_t buf_size;       /* Size for name or class buffer */
    char   smallbuf[10];   /* deliberately too-small buffer, for truncation test */
    int32  status;         /* Status values from routines */

    /* Create the HDF file and a vgroup with known, long name and class
       name for the truncation test. */
    file_id = Hopen(GETNAMETEST, DFACC_CREATE, 0);
    CHECK(file_id, FAIL, "Hopen");

    status = Vstart(file_id);
    CHECK(status, FAIL, "Vstart");

    vg1 = Vattach(file_id, -1, "w");
    CHECK(vg1, FAIL, "Vattach");

    status = Vsetname(vg1, VG_LONGNAME);
    CHECK(status, FAIL, "Vsetname");

    status = Vsetclass(vg1, VG_LONGCLASS);
    CHECK(status, FAIL, "Vsetclass");

    status = Vdetach(vg1);
    CHECK(status, FAIL, "Vdetach");
    vg1 = FAIL;

    status = Vend(file_id);
    CHECK(status, FAIL, "Vend");

    status = Hclose(file_id);
    CHECK(status, FAIL, "Hclose");
    file_id = FAIL;

    /* Re-open the file and attach the vgroup for read-only testing. */
    file_id = Hopen(GETNAMETEST, DFACC_RDWR, 0);
    CHECK(file_id, FAIL, "Hopen");

    status = Vstart(file_id);
    CHECK(status, FAIL, "Vstart");

    ref = Vfind(file_id, VG_LONGNAME);
    CHECK(ref, FAIL, "Vfind");

    vg1 = Vattach(file_id, ref, "r");
    CHECK(vg1, FAIL, "Vattach");

    /* Tests buf_size == NULL should fail, for both Vgetname and
       Vgetclass, regardless of the buffer pointer. */
    status = Vgetname(vg1, smallbuf, NULL);
    VERIFY(status, FAIL, "Vgetname:NULL buf_size");

    status = Vgetclass(vg1, smallbuf, NULL);
    VERIFY(status, FAIL, "Vgetclass:NULL buf_size");

    /* Tests a non-NULL buffer with *buf_size == 0 is a length-only
       query verifies the real name/class length */
    smallbuf[0] = 'X'; /* to confirm the buffer is untouched */
    buf_size    = 0;
    status      = Vgetname(vg1, smallbuf, &buf_size);
    CHECK(status, FAIL, "Vgetname:query");
    VERIFY(buf_size, strlen(VG_LONGNAME), "Vgetname:query");
    VERIFY(smallbuf[0], 'X', "Vgetname:query must not write to buffer");

    smallbuf[0] = 'X';
    buf_size    = 0;
    status      = Vgetclass(vg1, smallbuf, &buf_size);
    CHECK(status, FAIL, "Vgetclass:query");
    VERIFY(buf_size, strlen(VG_LONGCLASS), "Vgetclass:query");
    VERIFY(smallbuf[0], 'X', "Vgetclass:query must not write to buffer");

    /* Tests calling the functions with a buffer smaller than the actual
       name/class name. The name/class name should be truncated, while
       buf_size still reports the actual length. */
    buf_size = sizeof(smallbuf);
    status   = Vgetname(vg1, smallbuf, &buf_size);
    CHECK(status, FAIL, "Vgetname:truncate");
    VERIFY(buf_size, strlen(VG_LONGNAME), "Vgetname:truncate");
    if (strlen(smallbuf) != sizeof(smallbuf) - 1) {
        num_errs++;
        printf(">>> Vgetname:truncate - name not truncated to buffer size\n");
    }
    if (strncmp(smallbuf, VG_LONGNAME, sizeof(smallbuf) - 1)) {
        num_errs++;
        printf(">>> Vgetname:truncate - truncated name doesn't match\n");
    }

    buf_size = sizeof(smallbuf);
    status   = Vgetclass(vg1, smallbuf, &buf_size);
    CHECK(status, FAIL, "Vgetclass:truncate");
    VERIFY(buf_size, strlen(VG_LONGCLASS), "Vgetclass:truncate");
    if (strlen(smallbuf) != sizeof(smallbuf) - 1) {
        num_errs++;
        printf(">>> Vgetclass:truncate - class not truncated to buffer size\n");
    }
    if (strncmp(smallbuf, VG_LONGCLASS, sizeof(smallbuf) - 1)) {
        num_errs++;
        printf(">>> Vgetclass:truncate - truncated class doesn't match\n");
    }

    status = Vdetach(vg1);
    CHECK(status, FAIL, "Vdetach");
    vg1 = FAIL;

    status = Vend(file_id);
    CHECK(status, FAIL, "Vend");

    status = Hclose(file_id);
    CHECK(status, FAIL, "Hclose");
    file_id = FAIL;

done:
    /* Release resources */
    if (vg1 != FAIL)
        Vdetach(vg1);
    if (file_id != FAIL) {
        Vend(file_id);
        Hclose(file_id);
    }
}

/****************************************************************************
 * test_vgisinternal - tests the API function Vgisinternal
 *   - Use an existing GR file created during the period when GR vgroup had no
 *	class name, and had name set to GR_NAME
 *   - Get each vgroup, verify that it is internal or not
 ****************************************************************************/

#define GR_FILE "test_files/grtdfui83.hdf"
static void
test_vgisinternal()
{
    int32       fid = FAIL, vgroup_id = FAIL;
    int         is_internal = FALSE;
    int32       vref        = -1;
    int         ii, status;
    const char *testfile           = get_srcdir_filename(GR_FILE);
    char        internal_array2[2] = {TRUE, TRUE};

    /* Use a GR file to test Vgisinternal on internal vgroups */

    /* Open the old GR file and initialize the V interface */
    fid = Hopen(testfile, DFACC_READ, 0);
    CHECK(fid, FAIL, "Hopen: grtdfui83.hdf");
    status = Vstart(fid);
    CHECK(status, FAIL, "Vstart");

    ii = 0;
    while ((vref = Vgetid(fid, vref)) != FAIL) { /* until no more vgroups */
        vgroup_id = Vattach(fid, vref, "r");     /* attach to vgroup */

        /* Test that the current vgroup is or is not internal as specified
           in the array internal_array2 */
        is_internal = Vgisinternal(vgroup_id);
        CHECK(is_internal, FAIL, "Vgisinternal");
        VERIFY(is_internal, internal_array2[ii], "Vgisinternal");

        status = Vdetach(vgroup_id);
        CHECK(status, FAIL, "Vdetach");
        vgroup_id = FAIL;

        ii++; /* increment vgroup index */
    }

    /* Terminate access to the V interface and close the file */
    status = Vend(fid);
    CHECK(status, FAIL, "Vend");
    status = Hclose(fid);
    CHECK(status, FAIL, "Hclose");
    fid = FAIL;
done:
    /* Release resources */
    if (vgroup_id != FAIL)
        Vdetach(vgroup_id);
    if (fid != FAIL) {
        Vend(fid);
        Hclose(fid);
    }
}

void
test_vnameclass(void)
{
    /* test Vgroups with name and class that have more than 64 characters */
    test_vglongnames();

    /* test Vgetname and Vgetclass when either name or class is not defined. */
    test_undefined();

    /* test Vinquire post new argument buf_size */
    test_vinquire_buf_size();

    /* test Vgetname and Vgetclass post new argument buf_size */
    test_vgetname_vgetclass_buf_size();

    /* test Vgisinternal when there is no class name */
    test_vgisinternal();
}
