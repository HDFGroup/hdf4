#ifdef RCSID
static char RcsId[] = "@(#)$Revision$";
#endif
/*
$Header$

$Log$
Revision 1.2  1992/05/31 15:25:21  mfolk
Changed declaration of rank and dims[2] to int32 for Convex.

 * Revision 1.1  1992/02/26  20:08:43  mfolk
 * Initial revision
 *
*/
#include "hdf.h"
#include "dfsd.h"

float32 f32[10][10], tf32[10][10];
float32 f32scale[10], tf32scale[10];
float32 f32max = 40.0, f32min = 0.0;
float32 tf32max, tf32min;

int8 i8[10][10], ti8[10][10];
int8 i8scale[10], ti8scale[10];
int8 i8max = 127, i8min = -128;
int8 ti8max, ti8min;

uint8 ui8[10][10], tui8[10][10];
uint8 ui8scale[10], tui8scale[10];
uint8 ui8max = 255, ui8min = 0;
uint8 tui8max, tui8min;

int16 i16[10][10], ti16[10][10];
int16 i16scale[10], ti16scale[10];
int16 i16max = 1200, i16min = -1200;
int16 ti16max, ti16min;

uint16 ui16[10][10], tui16[10][10];
uint16 ui16scale[10], tui16scale[10];
uint16 ui16max = 20000, ui16min = 0;
uint16 tui16max, tui16min;

int32 i32[10][10], ti32[10][10];
int32 i32scale[10], ti32scale[10];
int32 i32max = 99999999, i32min = -999999999;
int32 ti32max, ti32min;

uint32 ui32[10][10], tui32[10][10];
uint32 ui32scale[10], tui32scale[10];
uint32 ui32max = 999999999, ui32min = 0;
uint32 tui32max, tui32min;

int main()
{
    int i, j, err, err1, err2, err3;
    int32 rank, dims[2];
    int number_failed = 0;


    rank = 2;
    dims[0] = 10;
    dims[1] = 10;

    printf("Creating arrays...\n");

    for (i=0; i<10; i++) {
        for (j=0; j<10; j++) {
	    f32[i][j] = (i * 40) + j;	/* range: 0 ~ 4-billion */

	    i8[i][j] = (i * 10) + j;		/* range: 0 ~ 100 */
	    ui8[i][j] = (i * 20) + j;		/* range: 0 ~ 200 */

	    i16[i][j] = (i * 3000) + j;		/* range: 0 ~ 30000 */
	    ui16[i][j] = (i * 6000) + j;	/* range: 0 ~ 60000 */

	    i32[i][j] = (i * 20) + j;	/* range: 0 ~ 2-billion */
	    ui32[i][j] = (i * 40) + j;	/* range: 0 ~ 4-billion */
	}

	f32scale[i] = (i * 40) + j;	/* range: 0 ~ 4-billion */

	i8scale[i] = (i * 10) + j;		/* range: 0 ~ 100 */
	ui8scale[i] = (i * 20) + j;		/* range: 0 ~ 200 */

	i16scale[i] = (i * 3000) + j;		/* range: 0 ~ 30000 */
	ui16scale[i] = (i * 6000) + j;	/* range: 0 ~ 60000 */

	i32scale[i] = (i * 20) + j;	/* range: 0 ~ 2-billion */
	ui32scale[i] = (i * 40) + j;	/* range: 0 ~ 4-billion */
    }

    DFSDsetdims(rank, dims);

    printf("Writing arrays to single file...\n");
    DFSDsetNT(DFNT_FLOAT32);
    err3=DFSDsetdimscale(1, (int32)10, (void *)f32scale);
    err2=DFSDsetmaxmin(&f32max, &f32min);
    err = DFSDadddata("ntcheck.hdf", rank, dims, f32);
    if (err3==FAIL || err2==FAIL || err==FAIL) 
        number_failed++;
    printf("%d, %d, %d\n", err3, err2, err);

    DFSDsetNT(DFNT_INT8);
    err3=DFSDsetdimscale(1, (int32)10, (void *)i8scale);
    err2=DFSDsetmaxmin(&i8max, &i8min);
    err = DFSDadddata("ntcheck.hdf", rank, dims, i8);
    if (err3==FAIL || err2==FAIL || err==FAIL) 
        number_failed++;
    printf("%d, %d, %d\n", err3, err2, err);

    DFSDsetNT(DFNT_UINT8);
    err3=DFSDsetdimscale(1, (int32)10, (void *)ui8scale);
    err2=DFSDsetmaxmin(&ui8max, &ui8min);
    err = DFSDadddata("ntcheck.hdf", rank, dims, ui8);
    if (err3==FAIL || err2==FAIL || err==FAIL) 
        number_failed++;
    printf("%d, %d, %d\n", err3, err2, err);

    DFSDsetNT(DFNT_INT16);
    err3=DFSDsetdimscale(1, (int32)10, (void *)i16scale);
    err2=DFSDsetmaxmin(&i16max, &i16min);
    err = DFSDadddata("ntcheck.hdf", rank, dims, i16);
    if (err3==FAIL || err2==FAIL || err==FAIL) 
        number_failed++;
    printf("%d, %d, %d\n", err3, err2, err);

    DFSDsetNT(DFNT_UINT16);
    err3=DFSDsetdimscale(1, (int32)10, (void *)ui16scale);
    err2=DFSDsetmaxmin(&ui16max, &ui16min);
    err = DFSDadddata("ntcheck.hdf", rank, dims, ui16);
    if (err3==FAIL || err2==FAIL || err==FAIL) 
        number_failed++;
    printf("%d, %d, %d\n", err3, err2, err);

    DFSDsetNT(DFNT_INT32);
    err3=DFSDsetdimscale(1, (int32)10, (void *)i32scale);
    err2=DFSDsetmaxmin(&i32max, &i32min);
    err = DFSDadddata("ntcheck.hdf", rank, dims, i32);
    if (err3==FAIL || err2==FAIL || err==FAIL) 
        number_failed++;
    printf("%d, %d, %d\n", err3, err2, err);

    DFSDsetNT(DFNT_UINT32);
    err3= DFSDsetdimscale(1, (int32)10, (void *)ui32scale);
    err2=DFSDsetmaxmin(&ui32max, &ui32min);
    err = DFSDadddata("ntcheck.hdf", rank, dims, ui32);
    if (err3==FAIL || err2==FAIL || err==FAIL) 
        number_failed++;
    printf("%d, %d, %d\n", err3, err2, err);

    printf("Reading arrays from single file...\n");

    err = DFSDgetdata("ntcheck.hdf", rank, dims, tf32);
    err2 = DFSDgetdimscale(1, (int32)10, (void *)tf32scale);
    err3 = DFSDgetmaxmin(&tf32max, &tf32min);
    if (err3==FAIL || err2==FAIL || err==FAIL) 
        number_failed++;
    printf("%d, %d, %d\n", err3, err2, err);

    err = DFSDgetdata("ntcheck.hdf", rank, dims, ti8);
    err2 = DFSDgetdimscale(1, (int32)10, (void *)ti8scale);
    err3 = DFSDgetmaxmin(&ti8max, &ti8min);
    if (err3==FAIL || err2==FAIL || err==FAIL) 
        number_failed++;
    printf("%d, %d, %d\n", err, err2, err3);

    err = DFSDgetdata("ntcheck.hdf", rank, dims, tui8);
    err2 = DFSDgetdimscale(1, (int32)10, (void *)tui8scale);
    err3 = DFSDgetmaxmin(&tui8max, &tui8min);
    if (err3==FAIL || err2==FAIL || err==FAIL) 
        number_failed++;
    printf("%d, %d, %d\n", err, err2, err3);

    err = DFSDgetdata("ntcheck.hdf", rank, dims, ti16);
    err2 = DFSDgetdimscale(1, (int32)10, (void *)ti16scale);
    err3 = DFSDgetmaxmin(&ti16max, &ti16min);
    if (err3==FAIL || err2==FAIL || err==FAIL) 
        number_failed++;
    printf("%d, %d, %d\n", err, err2, err3);

    err = DFSDgetdata("ntcheck.hdf", rank, dims, tui16);
    err2 = DFSDgetdimscale(1, (int32)10, (void *)tui16scale);
    err3 = DFSDgetmaxmin(&tui16max, &tui16min);
    if (err3==FAIL || err2==FAIL || err==FAIL) 
        number_failed++;
    printf("%d, %d, %d\n", err, err2, err3);

    err = DFSDgetdata("ntcheck.hdf", rank, dims, ti32);
    err2 = DFSDgetdimscale(1, (int32)10, (void *)ti32scale);
    err3 = DFSDgetmaxmin(&ti32max, &ti32min);
    if (err3==FAIL || err2==FAIL || err==FAIL) 
        number_failed++;
    printf("%d, %d, %d\n", err, err2, err3);

    err = DFSDgetdata("ntcheck.hdf", rank, dims, tui32);
    err2 = DFSDgetdimscale(1, (int32)10, (void *)tui32scale);
    err3 = DFSDgetmaxmin(&tui32max, &tui32min);
    if (err3==FAIL || err2==FAIL || err==FAIL) 
        number_failed++;
    printf("%d, %d, %d\n", err, err2, err3);

    printf("Checking arrays from single file...\n\n");
    err = 0;
    err1 = 0;
    err2 = 0;
    for (i=0; i<10; i++) {
	for (j=0; j<10; j++)
	   if (f32[i][j] != tf32[i][j])
	       err = 1;
	if (f32scale[i] != tf32scale[i])
	    err2 = 1;
    }
    if ((f32max != tf32max) || (f32min != tf32min)) 
        err1 = 1;

    number_failed += err + err1 + err2;
    if (err == 1) 
	printf(">>> Test failed for float32 array.\n");
    else 
	printf("Test passed for float32 array.\n");
    if (err2 == 1) 
	printf(">>> Test failed for float32 scales.\n");
    else 
	printf("Test passed for float32 scales.\n");
    if (err1 == 1)
	printf(">>> Test failed for float32 max/min.\n");
    else 
	printf("Test passed for float32 max/min.\n");

    err = 0;
    err1 = 0;
    err2 = 0;
    for (i=0; i<10; i++) {
	for (j=0; j<10; j++)
	   if (i8[i][j] != ti8[i][j])
	       err = 1;
	if (i8scale[i] != ti8scale[i])
	    err2 = 1;
    }
    if ((i8max != ti8max) || (i8min != ti8min)) 
        err1 = 1;

    number_failed += err + err1 + err2;
    if (err == 1)
	printf(">>> Test failed for int8 array.\n");
    else
	printf("Test passed for int8 array.\n");
    if (err2 == 1)
	printf(">>> Test failed for int8 scales.\n");
    else
	printf("Test passed for int8 scales.\n");
    if (err1 == 1)
	printf(">>> Test failed for int8 max/min.\n");
    else
	printf("Test passed for int8 max/min.\n");

    err = 0;
    err1 = 0;
    err2 = 0;
    for (i=0; i<10; i++) {
	for (j=0; j<10; j++)
	   if (ui8[i][j] != tui8[i][j])
	       err = 1;
	if (ui8scale[i] != tui8scale[i])
	    err2 = 1;
    }
    if ((ui8max != tui8max) || (ui8min != tui8min))
        err1 = 1;

    number_failed += err + err1 + err2;
    if (err == 1)
	printf(">>> Test failed for uint8 array.\n");
    else
	printf("Test passed for uint8 array.\n");
    if (err2 == 1)
	printf(">>> Test failed for uint8 scales.\n");
    else
	printf("Test passed for uint8 scales.\n");
    if  (err1 == 1)
	printf(">>> Test failed for uint8 max/min.\n");
    else
	printf("Test passed for uint8 max/min.\n");

    err = 0;
    err1 = 0;
    err2 = 0;
    for (i=0; i<10; i++) {
	for (j=0; j<10; j++)
	   if (i16[i][j] != ti16[i][j])
	       err = 1;
	if (i16scale[i] != ti16scale[i])
	    err2 = 1;
    }
    if ((i16max != ti16max) || (i16min != ti16min))
        err1 = 1;

    number_failed += err + err1 + err2;
    if (err == 1)
	printf(">>> Test failed for int16 array.\n");
    else
	printf("Test passed for int16 array.\n");
    if (err2 == 1)
	printf(">>> Test failed for int16 scales.\n");
    else
	printf("Test passed for int16 scales.\n");
    if (err1 == 1)
	printf(">>> Test failed for int16 max/min.\n");
    else
	printf("Test passed for int16 max/min.\n");

    err = 0;
    err1 = 0;
    err2 = 0;
    for (i=0; i<10; i++) {
	for (j=0; j<10; j++)
	   if (ui16[i][j] != tui16[i][j])
	       err = 1;
	if (ui16scale[i] != tui16scale[i])
	    err2 = 1;
    }
    if ((ui16max != tui16max) || (ui16min != tui16min))
        err1 = 1;

    number_failed += err + err1 + err2;
    if (err == 1)
	printf(">>> Test failed for uint16 array.\n");
    else
	printf("Test passed for uint16 array.\n");
    if (err2 == 1)
	printf(">>> Test failed for uint16 scales.\n");
    else
	printf("Test passed for uint16 scales.\n");
    if (err1 == 1)
	printf(">>> Test failed for uint16 max/min.\n");
    else
	printf("Test passed for uint16 max/min.\n");

    err = 0;
    err1 = 0;
    err2 = 0;
    for (i=0; i<10; i++) {
	for (j=0; j<10; j++)
	   if (i32[i][j] != ti32[i][j])
	       err = 1;
	if (i32scale[i] != ti32scale[i])
	    err2 = 1;
    }
    if ((i32max != ti32max) || (i32min != ti32min))
        err1 = 1;

    number_failed += err + err1 + err2;
    if (err == 1)
	printf(">>> Test failed for int32 array.\n");
    else
	printf("Test passed for int32 array.\n");
    if (err2 == 1)
	printf(">>> Test failed for int32 scales.\n");
    else
	printf("Test passed for int32 scales.\n");
    if (err1 == 1)
	printf(">>> Test failed for int32 max/min.\n");
    else
	printf("Test passed for int32 max/min.\n");

    err = 0;
    err1 = 0;
    err2 = 0;
    for (i=0; i<10; i++) {
	for (j=0; j<10; j++)
	   if (ui32[i][j] != tui32[i][j])
	       err = 1;
	if (ui32scale[i] != tui32scale[i])
	    err2 = 1;
    }
    if ((ui32max != tui32max) || (ui32min != tui32min))
        err1 = 1;

    number_failed += err + err1 + err2;
    if (err == 1)
	printf(">>> Test failed for uint32 array.\n");
    else
	printf("Test passed for uint32 array.\n");
    if (err2 == 1)
	printf(">>> Test failed for uint32 scales.\n");
    else
	printf("Test passed for uint32 scales.\n");
    if (err1 == 1)
	printf(">>> Test failed for uint32 max/min.\n");
    else
	printf("Test passed for uint32 max/min.\n");

    if (number_failed > 0 ) {
        printf("\n\t>>> %d TESTS FAILED <<<\n\n", number_failed);
    }
    else
        printf("\n\t>>> ALL TESTS PASSED <<<\n\n");

}
