/* $Id$ */

#ifdef RCSID
static char RcsId[] = "@(#)$Revision$";
#endif

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

/*
 FILE
       tree.c
       Test HDF Threaded-Balanced-Binary Tree (tbbt) routines.

 REMARKS

 DESIGN

 BUGS/LIMITATIONS

 EXPORTED ROUTINES

 AUTHOR
       Quincey Koziol

 MODIFICATION HISTORY
   10/21/93 - Started coding.
*/

#include "tproto.h"
#include "tbbt.h"

#define MAX_TEST_SIZE 31   /* maximum number of elements to insert */
#define NUM_TEST_RUNS 100  /* number of times to insert & remove each size */

#define RandInt(a,b) ((rand()%(((b)-(a))+1))+(a))

extern int num_errs;
extern int Verbocity;

PRIVATE VOID swap_arr
    PROTO((int32 *arr,intn a,intn b));

PUBLIC intn tcompare
    PROTO((VOIDP k1,VOIDP k2,intn cmparg));

#ifdef PROTOTYPES
PRIVATE VOID swap_arr(int32 *arr,intn a,intn b) 
#else
PRIVATE VOID swap_arr(arr,a,b)
int32 *arr;
intn a,b;
#endif
{
    int32 t;

    if(a!=b) {
	t=arr[a];
        arr[a]=arr[b];
	arr[b]=t;
      } /* end if */
}  /* end swap_arr() */

#ifdef PROTOTYPES
PUBLIC intn tcompare(VOIDP k1,VOIDP k2,intn cmparg) 
#else
PUBLIC intn tcompare(k1,k2,cmparg)
VOIDP k1;
VOIDP k2;
intn cmparg;
#endif
{
    return((intn)((*(int32 *)k1) - (*(int32 *)k2)));
}

void test_tbbt()
{
    intn test_size;
    intn i,j;
    int32 *ins_arr;
    int32 *rem_arr;
    intn t;
    TBBT_TREE *tree;
    VOIDP *r;

    ins_arr=(int32 *)HDgetspace(sizeof(int32)*MAX_TEST_SIZE);
    rem_arr=(int32 *)HDgetspace(sizeof(int32)*MAX_TEST_SIZE);
    for(test_size=3; test_size<=MAX_TEST_SIZE; test_size++) {
	MESSAGE(7,printf("\nTesting trees with %d elements\n",test_size););
	MESSAGE(8,printf("Testing tree #:"););
	for(j=0; j<NUM_TEST_RUNS; j++) {
	    MESSAGE(8,printf(" %d",j););
	    for(i=0; i<test_size; i++) {  /* initialize the arrays */
		ins_arr[i]=i;
		rem_arr[i]=i;
	      } /* end for */
	    for(i=0; i<test_size; i++) {  /* shuffle the arrays */
		t=RandInt(i,test_size-1);
		swap_arr(ins_arr,i,t);
		t=RandInt(i,test_size-1);
		swap_arr(rem_arr,i,t);
	      } /* end for */

	    if(Verbocity>9) {
	        printf("ins_arr: \n");
	        for(i=0; i<test_size; i++)   /* print the arrays */
	            printf("%d \n",ins_arr[i]);
	        printf("\nrem_arr: \n");
	        for(i=0; i<test_size; i++)   /* print the arrays */
	            printf("%d \n",rem_arr[i]);
	        printf("\n");
	      } /* end if */

	    tree=tbbtdmake(tcompare,sizeof(int32));
	    for(i=0; i<test_size; i++) {
                MESSAGE(9,printf("inserting %d\n",ins_arr[i]););
	        tbbtdins(tree,(VOIDP)&ins_arr[i],NULL);
                MESSAGE(9,tbbtdump(tree,-1););
	      }
	    MESSAGE(9,tbbtdump(tree,-1););
	    for(i=0; i<test_size; i++) {
	        int32 key;

	        key=rem_arr[i];
	        r=(VOIDP *)tbbtdfind(tree,(VOIDP)&key,NULL);
                MESSAGE(9,printf("removing %d\n",key););
	        tbbtrem((TBBT_NODE **)tree,(TBBT_NODE *)r,NULL);
                MESSAGE(9,tbbtdump(tree,-1););
    	      } /* end for */
	  } /* end for */
      } /* end for */
    HDfreespace(ins_arr);
    HDfreespace(rem_arr);
} /* end test_tbbt() */

