#ifdef RCSID
static char RcsId[] = "@(#)$Revision$";
#endif
/*
$Header$

$Log$
Revision 1.8  1992/07/09 20:41:50  sxu
Added float64 test.
Added DFSDgetNT test.

 * Revision 1.7  1992/07/09  15:33:29  mfolk
 * Added header that explains what it tests.
 *
 * Revision 1.6  1992/06/26  20:35:06  chouck
 * Changed output names
 *
 * Revision 1.4  1992/05/31  15:32:37  mfolk
 * Change type of rank and dims[2] from int to int32 for Convex.
 *
 * Revision 1.3  1992/05/28  17:31:32  chouck
 * Changed output file names
 *
 * Revision 1.2  1992/03/24  20:42:04  sxu
 * Changed output file names
 *
 * Revision 1.1  1992/02/26  20:07:41  mfolk
 * Initial revision
 *
*/

/***********************************************************
*
* Test program to test use of different number types in SDS.
* Creates arrays of all types.
* Stores arrays in individual files and reads them back. 
* Stores all arrays in one file and reads them back.
*
* NOTE: Native mode not tested in this program.  See tsdnnt.c
*       for same tests applied to native mode.
*
*************************************************************/

#include "hdf.h"
#include "dfsd.h"

#define FILENAME "tsdnt.hdf"

float64 f64[10][10], tf64[10][10];
float32 f32[10][10], tf32[10][10];
int8 i8[10][10], ti8[10][10];
uint8 ui8[10][10], tui8[10][10];
int16 i16[10][10], ti16[10][10];
uint16 ui16[10][10], tui16[10][10];
int32 i32[10][10], ti32[10][10];
uint32 ui32[10][10], tui32[10][10];

int main()
{
    int i, j, err, ret;
    int32 rank, num_type;
    int32 dims[2];
    int number_failed = 0;

    rank = 2;
    dims[0] = 10;
    dims[1] = 10;

    printf("Creating arrays...\n");

    for (i=0; i<10; i++) {
        for (j=0; j<10; j++) {
            f64[i][j] = (i * 10) + j;
	    f32[i][j] = (i * 10) + j;	/* range: 0 ~ 4-billion */

	    i8[i][j] = (i * 10) + j;    /* range: 0 ~ 100 */
	    ui8[i][j] = (i * 20) + j;	/* range: 0 ~ 200 */

	    i16[i][j] = (i * 10) + j;	/* range: 0 ~ 30000 */
	    ui16[i][j] = (i * 20) + j;	/* range: 0 ~ 60000 */

	    i32[i][j] = (i * 10L) + j;	/* range: 0 ~ 2-billion */
	    ui32[i][j] = (i * 20) + j;	/* range: 0 ~ 4-billion */
	}
    }
    
    err = 0;
    ret = DFSDsetdims(rank, dims);
    err += ret;
    /* individual files */
    printf("Testing arrays in individual files...\n");

    ret = DFSDsetNT(DFNT_FLOAT64);
    err += ret;
    ret = DFSDadddata("co00.hdf", rank, dims, f64);
    err += ret;
    printf("Write: %d     ", err);		
    err = 0;
    ret = DFSDgetdata("co00.hdf", rank, dims, tf64);
    err += ret;
    HEprint(stderr, 0);
    ret = DFSDgetNT(&num_type);
    err += ret;
    if (num_type != DFNT_FLOAT64) err--;
    printf("Read: %d\n", err);
    err = 0;
    for (i=0; i<10; i++)
	for (j=0; j<10; j++) {
	    if (f64[i][j] != tf64[i][j])
	        err++ ;
	    tf64[i][j] = 0.0;
	}
    if (err != 0) {
	printf(">>> Test failed for float64 array.\n");
        number_failed++;
    }
    else
	printf("Test passed for float64 array.\n");	

    ret = DFSDsetNT(DFNT_FLOAT32);
    err += ret;
    ret = DFSDadddata("co0.hdf", rank, dims, f32);
    err += ret;
    printf("Write: %d     ", err);		
    err = 0;
    ret = DFSDgetdata("co0.hdf", rank, dims, tf32);
    err += ret;
    HEprint(stderr, 0);
    ret = DFSDgetNT(&num_type);
    err += ret;
    if (num_type != DFNT_FLOAT32) err--;
    printf("Read: %d\n", err);
    err = 0;
    for (i=0; i<10; i++)
	for (j=0; j<10; j++) {
	    if (f32[i][j] != tf32[i][j])
	        err++ ;
	    tf32[i][j] = 0.0;
	}
    if (err != 0) {
	printf(">>> Test failed for float32 array.\n");
        number_failed++;
    }
    else
	printf("Test passed for float32 array.\n");	
    
    err = 0;
    ret = DFSDsetNT(DFNT_INT8);
    err += ret;
    ret = DFSDadddata("co1.hdf", rank, dims, i8);
    err += ret;
    printf("Write: %d     ", err);
    err = 0;
    ret = DFSDgetdata("co1.hdf", rank, dims, ti8);
    err += ret;   
    ret = DFSDgetNT(&num_type);
    err += ret;
    if (num_type != DFNT_INT8) err--;
    printf("Read: %d\n", err);
    err = 0;
    for (i=0; i<10; i++)
	for (j=0; j<10; j++) {
	    if (i8[i][j] != ti8[i][j])
	        err = 1;
	    ti8[i][j] = 0;
	}
    if (err == 1) {
	printf(">>> Test failed for int8 array.\n");
    number_failed++;
    }
    else
	printf("Test passed for int8 array.\n");

    err = 0;
    ret = DFSDsetNT(DFNT_UINT8);
    err += ret;
    ret =  DFSDadddata("co2.hdf", rank, dims, ui8);
    err += ret;
    printf("Write: %d     ", err);
    err = 0;
    ret = DFSDgetdata("co2.hdf", rank, dims, tui8);
    err += ret;
    ret = DFSDgetNT(&num_type);
    err += ret;
    if (num_type != DFNT_UINT8) err--;
    printf("Read: %d\n", err);
    err = 0;
    for (i=0; i<10; i++) {
	  for (j=0; j<10; j++) {
	    if (ui8[i][j] != tui8[i][j])
	        err = 1;
	    tui8[i][j] = 0;
      }
	}
    if (err == 1 ) {
	printf(">>> Test failed for uint8 array.\n");
    number_failed++;
    }
    else
	printf("Test passed for uint8 array.\n");

    err = 0;
    ret = DFSDsetNT(DFNT_INT16);
    err += ret;
    ret =  DFSDadddata("co3.hdf", rank, dims, i16);
    err += ret;
    printf("Write: %d     ", err);
    err = 0;
    ret = DFSDgetdata("co3.hdf", rank, dims, ti16);
    err += ret;
    ret = DFSDgetNT(&num_type);
    err += ret;
    if (num_type != DFNT_INT16) err--;
    printf("Read: %d\n", err);
    err = 0;
    for (i=0; i<10; i++)
	for (j=0; j<10; j++) {
	    if (i16[i][j] != ti16[i][j])
	        err = 1;
	    ti16[i][j] = 0;
	}
    if (err == 1 ) {
	printf(">>> Test failed for int16 array.\n");
    number_failed++;
    }
    else
	printf("Test passed for int16 array.\n");

    err = 0;
    ret = DFSDsetNT(DFNT_UINT16);
    err += ret;
    ret = DFSDadddata("co4.hdf", rank, dims, ui16);
    err += ret;
    printf("Write: %d     ", err);
    err = 0;
    ret = DFSDgetdata("co4.hdf", rank, dims, tui16);
    err += ret;
    ret = DFSDgetNT(&num_type);
    err += ret;
    if (num_type != DFNT_UINT16) err--;
    printf("Read: %d\n", err);
    err = 0;
    for (i=0; i<10; i++)
	for (j=0; j<10; j++) {
	    if (ui16[i][j] != tui16[i][j])
	        err = 1;
	    tui16[i][j] = 0;
	}
    if (err == 1 ) {
	printf(">>> Test failed for uint16 array.\n");
    number_failed++;
    }
    else
	printf("Test passed for uint16 array.\n");

    err = 0;
    ret = DFSDsetNT(DFNT_INT32);
    err += ret;
    ret = DFSDadddata("co5.hdf", rank, dims, i32);
    err += ret;
    printf("Write: %d     ", err);
    err = 0;
    ret = DFSDgetdata("co5.hdf", rank, dims, ti32);
    err += ret;
    ret = DFSDgetNT(&num_type);
    err += ret;
    if (num_type != DFNT_INT32) err--;
    printf("Read: %d\n", err);
    err = 0;
    for (i=0; i<10; i++)
	for (j=0; j<10; j++) {
	    if (i32[i][j] != ti32[i][j])
	        err = 1;
	    ti32[i][j] = 0;
	}
    if (err == 1 ) {
	printf(">>> Test failed for int32 array.\n");
    number_failed++;
    }
    else
	printf("Test passed for int32 array.\n");

    err = 0;
    ret = DFSDsetNT(DFNT_UINT32);
    err += ret;
    ret = DFSDadddata("co6.hdf", rank, dims, ui32);
    err += ret;
    printf("Write: %d     ", err);
    err = 0;
    ret = DFSDgetdata("co6.hdf", rank, dims, tui32);
    err += ret;
    ret = DFSDgetNT(&num_type);
    err += ret;
    if (num_type != DFNT_UINT32) err--;
    printf("Read: %d\n", err);
    err = 0;
    for (i=0; i<10; i++) {
	  for (j=0; j<10; j++) {
	    if (ui32[i][j] != tui32[i][j])
	        err = 1;
	    tui32[i][j] = 0;
      }
	}
    if (err == 1 ) {
	printf(">>> Test failed for uint32 array.\n");
    number_failed++;
    }
    else
	printf("Test passed for uint32 array.\n");


    printf("Writing arrays to single file... ");
    DFSDsetNT(DFNT_FLOAT64);
    err = DFSDadddata(FILENAME, rank, dims, f64);
    printf("%d  ", err);
    DFSDsetNT(DFNT_FLOAT32);
    err = DFSDadddata(FILENAME, rank, dims, f32);
    printf("%d  ", err);
    DFSDsetNT(DFNT_INT8);
    err = DFSDadddata(FILENAME, rank, dims, i8);
    printf("%d  ", err);
    DFSDsetNT(DFNT_UINT8);
    err = DFSDadddata(FILENAME, rank, dims, ui8);
    printf("%d  ", err);
    DFSDsetNT(DFNT_INT16);
    err = DFSDadddata(FILENAME, rank, dims, i16);
    printf("%d  ", err);
    DFSDsetNT(DFNT_UINT16);
    err = DFSDadddata(FILENAME, rank, dims, ui16);
    printf("%d  ", err);
    DFSDsetNT(DFNT_INT32);
    err = DFSDadddata(FILENAME, rank, dims, i32);
    printf("%d  ", err);
    DFSDsetNT(DFNT_UINT32);
    err = DFSDadddata(FILENAME, rank, dims, ui32);
    printf("%d\n\n", err);

    printf("Reading arrays from single file... ");
    err = DFSDgetdata(FILENAME, rank, dims, tf64);
    printf("%d  ", err);
    err = DFSDgetdata(FILENAME, rank, dims, tf32);
    printf("%d  ", err);
    err = DFSDgetdata(FILENAME, rank, dims, ti8);
    printf("%d  ", err);
    err = DFSDgetdata(FILENAME, rank, dims, tui8);
    printf("%d  ", err);
    err = DFSDgetdata(FILENAME, rank, dims, ti16);
    printf("%d  ", err);
    err = DFSDgetdata(FILENAME, rank, dims, tui16);
    printf("%d  ", err);
    err = DFSDgetdata(FILENAME, rank, dims, ti32);
    printf("%d  ", err);
    err = DFSDgetdata(FILENAME, rank, dims, tui32);
    printf("%d\n", err);

    printf("Checking arrays from single file...\n\n");
    err = 0;
    for (i=0; i<10; i++)
	for (j=0; j<10; j++)
	   if (f64[i][j] != tf64[i][j])
	       err = 1;
    if (err == 1 ) {
	printf(">>> Test failed for float64 array.\n");
    number_failed++;
    }
    else
	printf("Test passed for float64 array.\n");

    err = 0;
    for (i=0; i<10; i++)
	for (j=0; j<10; j++)
	   if (f32[i][j] != tf32[i][j])
	       err = 1;
    if (err == 1 ) {
	printf(">>> Test failed for float32 array.\n");
    number_failed++;
    }
    else
	printf("Test passed for float32 array.\n");

    err = 0;
    for (i=0; i<10; i++)
	for (j=0; j<10; j++)
	   if (i8[i][j] != ti8[i][j])
	       err = 1;
    if (err == 1 ) {
	printf(">>> Test failed for int8 array.\n");
    number_failed++;
    }
    else
	printf("Test passed for int8 array.\n");

    err = 0;
    for (i=0; i<10; i++)
	for (j=0; j<10; j++)
	   if (ui8[i][j] != tui8[i][j])
	       err = 1;
    if (err == 1 ) {
	printf(">>> Test failed for uint8 array.\n");
    number_failed++;
    }
    else
	printf("Test passed for uint8 array.\n");

    err = 0;
    for (i=0; i<10; i++)
	for (j=0; j<10; j++)
	   if (i16[i][j] != ti16[i][j])
	       err = 1;
    if (err == 1 ) {
	printf(">>> Test failed for int16 array.\n");
    number_failed++;
    }
    else
	printf("Test passed for int16 array.\n");

    err = 0;
    for (i=0; i<10; i++)
	for (j=0; j<10; j++)
	   if (ui16[i][j] != tui16[i][j])
	       err = 1;
    if (err == 1 ) {
	printf(">>> Test failed for uint16 array.\n");
    number_failed++;
    }
    else
	printf("Test passed for uint16 array.\n");

    err = 0;
    for (i=0; i<10; i++)
	for (j=0; j<10; j++)
	   if (i32[i][j] != ti32[i][j])
	       err = 1;
    if (err == 1 ) {
	printf(">>> Test failed for int32 array.\n");
    number_failed++;
    }
    else
	printf("Test passed for int32 array.\n");

    err = 0;
    for (i=0; i<10; i++)
	for (j=0; j<10; j++)
	   if (ui32[i][j] != tui32[i][j])
	       err = 1;
    if (err == 1 ) {
	printf(">>> Test failed for uint32 array.\n");
    number_failed++;
    }
    else
	printf("Test passed for uint32 array.\n");

    if (number_failed > 0 ) {
	printf("\n\t>>> %d TESTS FAILED <<<\n\n", number_failed);
    }
    else
	printf("\n\t>>> ALL TESTS PASSED <<<\n\n");

}
