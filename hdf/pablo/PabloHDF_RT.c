/*  This file is part of the Pablo Performance Analysis Environment
// 
//           (R)
//  The Pablo    Performance Analysis Environment software is NOT in
//  the public domain.  However, it is freely available without fee for
//  education, research, and non-profit purposes.  By obtaining copies
//  of this and other files that comprise the Pablo Performance Analysis
//  Environment, you, the Licensee, agree to abide by the following
//  conditions and understandings with respect to the copyrighted software:
//  
//  1.  The software is copyrighted in the name of the Board of Trustees
//      of the University of Illinois (UI), and ownership of the software
//      remains with the UI. 
// 
//  2.  Permission to use, copy, and modify this software and its documentation
//      for education, research, and non-profit purposes is hereby granted
//      to Licensee, provided that the copyright notice, the original author's
//      names and unit identification, and this permission notice appear on
//      all such copies, and that no charge be made for such copies.  Any
//      entity desiring permission to incorporate this software into commercial
//      products should contact:
// 
//           Professor Daniel A. Reed                 reed@cs.uiuc.edu
//           University of Illinois
//           Department of Computer Science
//           2413 Digital Computer Laboratory
//           1304 West Springfield Avenue
//           Urbana, Illinois  61801
//           USA
// 
//  3.  Licensee may not use the name, logo, or any other symbol of the UI
//      nor the names of any of its employees nor any adaptation thereof in
//      advertizing or publicity pertaining to the software without specific
//      prior written approval of the UI.
// 
//  4.  THE UI MAKES NO REPRESENTATIONS ABOUT THE SUITABILITY OF THE
//      SOFTWARE FOR ANY PURPOSE.  IT IS PROVIDED "AS IS" WITHOUT EXPRESS
//      OR IMPLIED WARRANTY.
// 
//  5.  The UI shall not be liable for any damages suffered by Licensee from
//      the use of this software.
// 
//  6.  The software was developed under agreements between the UI and the
//      Federal Government which entitle the Government to certain rights.
// 
// *************************************************************************
// 
//  Developed by: The Pablo Research Group
//                University of Illinois at Urbana-Champaign
//                Department of Computer Science
//                1304 W. Springfield Avenue
//                Urbana, IL     61801
// 
//                http://www-pablo.cs.uiuc.edu
// 
//  Send comments to: pablo-feedback@guitar.cs.uiuc.edu
// 
//  Copyright (c) 1987-1998
//  The University of Illinois Board of Trustees.
//       All Rights Reserved.
// 
//  PABLO is a registered trademark of
//  The Board of Trustees of the University of Illinois
//  registered in the U.S. Patent and Trademark Office.
// 
//  Project Manager and Principal Investigator:
//       Daniel A. Reed (reed@cs.uiuc.edu)
//
// Funded in part by the Defense Advanced Research Projects Agency 
// under DARPA contracts DABT63-94-C0049 (SIO Initiative), 
// F30602-96-C-0161, and DABT63-96-C-0027 by the National Science 
// Foundation under the PACI program and grants NSF CDA 94-01124 and
// ASC 97-20202, and by the Department of Energy under contracts
// DOE B-341494, W-7405-ENG-48, and 1-B-333164.
*/ 
/*======================================================================*
// File:  PabloHDF_RT							*
// Purpose: support use of Pablo trace library to analyze HDF 		*
//	    performance							*
// Contents:								*
//  HDFinitTrace_RT        : initialize real-time tracing		*
//  HDFendTrace_RT         : complete trace 				*
//  initHDFProcTrace_RT    : sets up data structures at init time.	*
//  initproctracert_()	   : fortran interface				*
//  traceEvent_RT	   : called to record event information		*
//  HDFrecordSum 	   : adds fields of one record to those of 	*
//			     another					*
//  HDFnodeInit 	   : initializes linked list node		*
//  HDFrecordFileName	   : records named HDF identifiers 		*
//  HDFenqueue		   : adds node to linked list			*
//  BeginIOEventRecord     : initialize before I/O call			*
//  EndIOEventRecord 	   : finalize after I/O call			*
//  BeginHDFEventRecord    : initialize before HDF call			*
//  EndHDFEventRecord 	   : finalizie after HDF call			*
//  HDFrecordFileName	   : record named identifier information	*
//  HDFassignPabloIDs	   : assigns a number to named identifiers	*
//  writeHDFNamePacketsRT  : write SDDF packets for identifier names	*
//  HDFupdateProcLists     : adds records in queue to entries in 	*
//			     tables					*
//  HDFupdateProcs	   : called by HDFupdateProcLists to do 	*
//			     addition					*
//  HDFSummarySDDF	   : write SDDF event summary packets		*
//  HDFnodeInit 	   : initialize event node			*
//  HDFenqueue		   : add event node to queue for identification *
//  HDFrecordSum 	   : add one event record to another		*
//  getHDFFieldIndex	   : get Field Index for counts and times	*
//  getHDFByteFieldIndex   : get field index for bytes 			*
//  writeHDFRecDescrptrsRT : write HDF Record Descriptor packets	*
//  printFileMappingsRT	   : print map of named identifiers		*
//  _hdfNameDescriptor()   : writes SDDF descriptor packet for names	*
//======================================================================*/
#include "SystemDepend.h"
#include "Trace.h"
#include "TraceParam.h"
#include "ProcIDs.h"
#include "HDFFieldNames.h"
#include "HDFTrace.h"
#include "SDDFparam.h"
#include <string.h>
#include <stdio.h>
#include <fcntl.h>
/* on ipsc/860 don't include this or you'll get multiply defined SEEK_* */
#ifndef __NX
#include <unistd.h>
#endif
 
#ifndef SUCCESS
#define SUCCESS 0
#define FAILURE	1
#endif

#ifndef TRUE
#define TRUE 1
#define FALSE	0
#endif
#define NEG_THREAD_ID -999

#ifdef HAVE_PABLO

#include "HDFrecord_RT.h"

#else

#define returnRecord(x)    return x;

#endif

#ifndef TRgetThreadID
#define TRgetThreadID  TRgetNode
#endif

#ifndef TRnumNodes
#define TRnumNodes 1
#endif

#define AllThreads -1

#define set_c_mappedID( fd ) (fd)
#define c_mappedID( fd ) (fd)
/*======================================================================*
//  User output file pointer.						*
//======================================================================*/
FILE *outP;
/*======================================================================*
// Data Structures:							*
//
// HDFQueues:   an array of linked list.  Each list corresponds to an	*
//              HDF event and contains an entry for each different 	*
//		thread and data set referenced by a call to that event  *
//									*
// CallStacks:	a linked list of stacks.  There is a stack for each     *
//		thread that calls and HDF procedure.  At any given time,*
//		the stack represents the calling stack of the HDF 	*
//		routines.						*
//									*
// HDFfileList: a linked list of named identifiers and identifier 	*
//		numbers.  This is processed later to assign a single 	*
//		numbers to identifiers with the same name.		*
//======================================================================*/
HDFnode_t **HDFQueues;
QueuePtr_t CallStacks;
fileRec_t *HDFfileList;
/*=======================================================================
// Internal Function prototypes						*
//======================================================================*/
void HDFinitTrace_RT( char *, unsigned );
void HDFendTrace_RT( );
int initproctracert_();
int initHDFProcTrace_RT();
void traceEvent_RT( int , char *, unsigned ) ;
void BeginIOEventRecord ( int , HDFnode_t *, double ); 
void EndIOEventRecord ( int , HDFnode_t *, double , char *, unsigned , int );
void BeginHDFEventRecord( int , int , HDFnode_t *, double );
void EndHDFEventRecord ( int ,HDFnode_t *,double ,char *,unsigned ,int );
void HDFrecordFileName( HDFsetInfo * );
void HDFassignPabloIDs( int *, char *** );
void writeHDFNamePacketsRT( char **, int );
void HDFupdateProcLists( HDFnode_t *);
void HDFupdateProcs( HDFnode_t *, int );
void HDFSummarySDDF( HDFnode_t *, int );
void HDFnodeInit ( HDFnode_t * ) ;
void HDFenqueue( HDFnode_t *, HDFnode_t *, int );
void HDFrecordSum ( HDFrec_t *, HDFrec_t * );
int getHDFFieldIndex( int );
int getHDFByteFieldIndex( int );
void writeHDFRecDescrptrsRT();
void printFileMappingsRT( char *, char **, int );
void _hdfNameDescriptor();
/*=======================================================================
// External Function prototypes						*
//======================================================================*/
double clockToSeconds( CLOCK );
/*======================================================================*
// Node used in initializaton.						*
//======================================================================*/
HDFnode_t InitNode;
/*======================================================================*
// Node used to get total						*
//======================================================================*/
HDFrec_t Tally;
/*======================================================================*
// Name of trace file     					*
//======================================================================*/
char *FileName;
/*======================================================================*
// NAME									*
//     HDFinitTrace_RT-- initialize HDF real-time tracing		*
// USAGE								*
//     VOID HDFinitTrace_RT( fileName, procTraceMask)			*
//									*
//     char *fileName;		IN: name of output file			*
//     unsigned procTraceMask;	IN: families of procedures to trace	*
// RETURNS								*
//     None.								*
//======================================================================*/
void HDFinitTrace_RT( char *fileName, unsigned procTraceMask )
{
	int error;
	TR_LOCK	criticalSection;
	TRgetClock( &epoch );
	criticalSection = TRlock();
	error = initHDFProcTrace_RT() ;
    	procTrace = procTraceMask;
	TRunlock( criticalSection );
	if ( error != SUCCESS ) {
	   printf ("Unable to Initialize properly.  Exiting program\n");
	   exit(-1);
	}
	FileName = ( char * ) malloc ( strlen( fileName ) + 1 );
	strcpy( FileName, fileName ) ;
        setTraceFileName(FileName);
        basicLibraryInit( );         /* initialize the trace library */
}
/*======================================================================*
// NAME									*
//     HDFendTrace-- end HDF tracing					*
// USAGE								*
//     VOID HDFendTrace_RT(void)					*
// RETURNS								*
//     None.								*
//======================================================================*/
void HDFendTrace_RT( )
{
	int i, j, procIX, eventID, threadID, numSetIDs;
	HDFnode_t **ProcTally, *ThreadTally;
	HDFnode_t **NameTally;
	QueuePtr_t *P, *Q;
	char **Names;
	char* mapFile;
	fileRec_t *F;

	/*==============================================================*
	//  Assing pablo ids to named identifiers and tag records	*
	//==============================================================*/
	HDFassignPabloIDs( &numSetIDs, &Names );
	mapFile = (char *)malloc( strlen(FileName) + 5 );
	strcpy(mapFile,FileName);
	strcat(mapFile,".map");
        printFileMappingsRT( mapFile, Names, numSetIDs );
	/*==============================================================*
	// Print SDDF summary records					*
	//==============================================================*/
	writeHDFRecDescrptrsRT();
	writeHDFNamePacketsRT( Names, numSetIDs );
   	for ( j = 0; j < NumHDFProcs; ++j ) {
	   HDFSummarySDDF( HDFQueues[j], j );
	} 
	endTracing();
	/*==============================================================*
	// Clean up storage						*
	//==============================================================*/
	free( mapFile );
	for ( j = 0; j < numSetIDs; ++j ) {
	    if ( Names[j] != NULL ) {
               free(Names[j]);
	    }
	}
	free( Names );
	P = &CallStacks;
	while ( P != NULL && P->id <= threadID ) {
	   Q = P;
	   P = P->ptr;
	   free( Q->nodePtr);
	   free(Q);
	}
	free(HDFQueues) ;
}
/*======================================================================*
// initHFDProcTrace_RT							*
//	This function initializes data structures specific to		* 
//	the HDF real-time procedure entry/exit tracing extensions of 	*
//      the Pablo instrumentation library.  				*
//======================================================================*/
int initproctracert_()

{
	return initHDFProcTrace_RT();
}

int initHDFProcTrace_RT()

{
	int j, procIndex, size;
	int numProcs = NumHDFProcs;

        if ( traceProcessorNumber == -1 ) {
            traceProcessorNumber = TRgetDefaultProcessor();
        }
	/*==============================================================*
	// Initialize InitNode used for node initialization.		*
	//==============================================================*/
	InitNode.ptr = NULL;
	InitNode.fptr = NULL;
        InitNode.eventID = 0;             
        InitNode.record.nCalls = 0;             
        InitNode.record.firstCall = 6.0236e23;
        InitNode.record.lastCall = 0;
        InitNode.lastIOtime = 0;           
        InitNode.record.incDur = 0;              
        InitNode.record.excDur = 0;              
        InitNode.record.threadID = -1;
        InitNode.record.hdfID = 0;             
        InitNode.record.xRef = 0;             
	for ( j = 0; j < nTallyFields; ++j ) {
           InitNode.record.times[j] = 0; 
	}
	for ( j = 0; j < nTallyFields; ++j ) {
           InitNode.record.counts[j] = 0; 
	}
	for ( j = 0; j < nByteFields; ++j ) {
           InitNode.record.bytes[j] = 0; 
	}
	/*==============================================================*
	// initialize linked list used to keep track of named hdf 	*
	// identifiers.							*
	//==============================================================*/
	HDFfileList = NULL;
	/*==============================================================*
	// Allocate a one dimensional array of pointers to queues of 	*
	// HDFnodes.  There is one queue for each thread and one for 	*
	// each HDF procedure.  Each queue will be a list of summary 	*
	// records distinquished by file type and 			*
	//==============================================================*/
	size = numProcs*sizeof( HDFnode_t * );
	HDFQueues = (HDFnode_t **)malloc( size );
	if ( HDFQueues == NULL ) {
	   printf("Failed to allocate HDFQueues in initHDFProcTrace\n");
	   return FAILURE;
	}
	for ( j = 0; j < numProcs; ++j ) {
	   HDFQueues[j] = NULL;
	}
	/*==============================================================*
	// Allocate call stack and initialize.            		*
	//==============================================================*/
	CallStacks.ptr = NULL;
	CallStacks.nodePtr = (HDFnode_t *)malloc( sizeof( HDFnode_t) );
	*CallStacks.nodePtr = InitNode;
	CallStacks.id = 0;
	CallStacks.nodePtr->record.threadID = 0;
	return SUCCESS;
}
/*======================================================================*
// This is called from the HDF and I/O routines when real-time summary	*
// tracing is used.  It sets up a call stack for the specific thread in *
// use if no stack is yet set up.  It then calls calls a routine to 	*
// handle the event based on whether it is an I/O or HDF call.		*
//======================================================================*/
void traceEvent_RT( int eventType, char *dataPtr, unsigned dataLen ) 
{
	TR_LOCK	criticalSection;
	CLOCK	currentTime;
	double  seconds;
	int err, threadID;
	QueuePtr_t *P, *Q;
	HDFnode_t *Stack;

	criticalSection = TRlock();
	currentTime = getClock();
	threadID = TRgetThreadID();
	seconds = clockToSeconds( currentTime );

	err = FALSE;
	P = &CallStacks;
	Q = P->ptr;
	while ( Q != NULL && Q->id <= threadID ) {
	   P = Q;
	   Q = P->ptr;
	}
	if ( P->id != threadID ) {
	   Q = (QueuePtr_t *)malloc( sizeof( QueuePtr_t ) );
	   Q->ptr = P->ptr;
	   P->ptr = Q;
	   Q->id = threadID;
	   Q->nodePtr = (HDFnode_t *)malloc( sizeof (HDFnode_t) );
	   *(Q->nodePtr) = InitNode;
	   P = Q;
	}
	Stack = P->nodePtr;
	if ( isBeginIOEvent ( eventType ) ) {
	   BeginIOEventRecord ( eventType , Stack, seconds ) ;
	} else if ( isBeginHDFEvent( eventType ) ) { 
	   BeginHDFEventRecord ( eventType , threadID, Stack, seconds ) ;
	} else if ( isEndIOEvent( eventType ) ) {
	   EndIOEventRecord ( eventType, Stack, seconds, 
			      dataPtr,   dataLen,  err );
	} else if ( isEndHDFEvent( eventType ) ) {
	   EndHDFEventRecord ( eventType, Stack, seconds, 
			      dataPtr,   dataLen,  err );
	} else {
	   err = TRUE;
	}
	TRunlock( criticalSection );
	if ( err ) exit(-1) ;
}
/*======================================================================= 
// BeginIOEventRecord:                                               	*
//  This routine simply records the time in the record on the top of 	*
//  the stack corresponding to the threadID.  If no stack exists, one	*
//  is created.								*
//=====================================================================*/ 
void BeginIOEventRecord ( int eventType, HDFnode_t *Stk, double seconds  ) 
{
	/*===============================================================
	// save the time value temporarily in top of stack		*
	// When the end record is received, the duration can be 	*
	// computed.							*
	//=============================================================*/
	Stk->lastIOtime = seconds;
}
/*======================================================================= 
// EndIOEventRecord:
//  This routine retrieves the entry time saved on the top of the stack *
//  corresponding to this thread and computes the duration of the I/O	*
//  event.  This is added to the record field corresponding to this	*
//  type of I/O.  The Bytes field in the record is updated if this is 	*
//  a read or write operation.						*
//=====================================================================*/ 
void EndIOEventRecord ( int eventType, HDFnode_t *Stk, double seconds, 
	                char *dataPtr, unsigned datalen, int err )
{
	HDFnode_t *H;
	double incDur;
	int Field, ByteField;

	incDur = seconds - Stk->lastIOtime;
	/*==============================================================*
	// Stk->fptr points to the top of the stack.  If it is NULL	*	
	// then the stack is empty and this I/O is being done outside 	*
	// of HDF procedures.  On completion of the program, 		*
	// Stk->record should hold the total of all I/O done by this	*	
	// thread outside of HDF procedures.				*
	//==============================================================*/
	H = Stk->fptr;
	if ( H == NULL ) {
	   H = Stk;
	} 
	Field = getHDFFieldIndex( eventType ) ;
        H->record.times[Field] += incDur;
        ++H->record.counts[Field];
	ByteField = getHDFByteFieldIndex( Field ) ;
	if ( ByteField >= 0 ) {
	   int *bytes = (int *)dataPtr;
	   H->record.bytes[ByteField] += *bytes;
	}
}
/*======================================================================= 
//   BeginHDFEventRecord:						* 
// 	This function puts a trace record on the stack corresponding to	*
//   	this thread.  If no stack exists, one is created.  If no record	* 
//   	exist, a record is created.                                   	* 
//======================================================================*/ 
void BeginHDFEventRecord( int eventID, int tID, HDFnode_t *Stk, double secs )
{
	HDFnode_t *HDFrec;
	int procIndex;
	/*==============================================================*
	// Create a record. Push it onto the call stack.                *
	//==============================================================*/
        HDFrec = (HDFnode_t *)malloc( sizeof(HDFnode_t) );
        HDFnodeInit( HDFrec ) ;
	HDFrec->record.threadID = tID;
	HDFrec->eventID = eventID;
	HDFrec->fptr = Stk->fptr;
	Stk->fptr = HDFrec ;
	/*==============================================================*
	// save time stamp in record.					*
	//==============================================================*/
	HDFrec->record.lastCall = secs;
}
/*======================================================================* 
// EndHDFEventRecord:							*
//  This routine pops the HDF record from the top of the stack 		*
//  corresponding to this thread and computes the inclusive duration    *
//  and adds it to the inclusive duration field of this record and to   *
//  the HDF time field of the calling routines record.			*
//======================================================================*/ 
void EndHDFEventRecord ( int eventID, HDFnode_t *Stk,     double seconds, 
			 char *dataPtr, unsigned dataLen, int err       )
{
        HDFsetInfo 	*info;
	HDFnode_t	*HDFrec, *Top;
	double 		incSecs;
	static int	dummyIDs = -4;
	int		procIndex, setID, threadID;
	/*==============================================================*
	// Find the procIndex in the table.                          	* 
	//==============================================================*/
	procIndex = ProcIndexForHDFExit( eventID );
	/*==============================================================*
	// pop record from top of the stack, compute inclusive duration	*
	// and set the corresponding record field and increment nCalls.	*
	//==============================================================*/
	HDFrec = Stk->fptr;
	if ( HDFrec == NULL ) {
	   printf(">>> EndHDFEventRecord: Call Stack is empty. <<<\n");
	   err = TRUE;
	   return;
	}
	Stk->fptr = HDFrec->fptr;
	incSecs = seconds - HDFrec->record.lastCall;
	HDFrec->record.incDur = +incSecs;
	++HDFrec->record.nCalls;
   	/*==============================================================*
	// add old record to chain to have its xRef field tagged.	*
	//==============================================================*/
	HDFrec->ptr = Stk->ptr;
	Stk->ptr = HDFrec;
	/*==============================================================*
	// Add set ID information.					*
	//==============================================================*/
	if ( dataPtr != NULL ) {
	   info = (HDFsetInfo *)dataPtr;
	   if ( info->setName != NULL ) {
	      if ( info->setID == 0 ) {
	         info->setID = dummyIDs--;
	      }
	      HDFrecordFileName ( info );
	   }
	   HDFrec->record.hdfID = info->setID;
	}
	Top = Stk->fptr;
	/*==============================================================*
	// If there was a calling HDF procedure, update its HDF		*
	// duration and count fields.					*
	//==============================================================*/
	if ( Top != NULL ) {
	   Top->record.times[ HDF_ ] += incSecs ;
	   ++Top->record.counts[ HDF_ ] ;
	} else {
	   /*===========================================================*
	   // The stack is empty.  					*
	   //  - add duration to HDF total for this thread,  		*
	   //  - tag all of the xRef fields of the records queued to    *
	   //    HDFrec	and record all of the events in the maind	*
	   //    HDFQueues table.					*
	   //===========================================================*/
	   Stk->record.times[ HDF_ ] += incSecs ;
	   ++Stk->record.counts[ HDF_ ] ;
	   HDFupdateProcLists( Stk->ptr );
	   Stk->ptr = NULL;
	}
}
/*======================================================================* 
// This routine keeps track of the identifier names and tags.  Some	*
// names may be associated with more than one tag.  This will be 	*
// rectified when final tallies are done.				*
//======================================================================*/
void HDFrecordFileName( HDFsetInfo *info )
{
	fileRec_t *P;
	char *t;
	int match, id;
	P = HDFfileList;
	match = FALSE;
	id = info->setID;
	while ( P != NULL && match == FALSE ) {
	   if ( strcmp( P->fileName, info->setName ) != 0 && P->hdfID == id ) {
	      match = TRUE;
	   } else {
	      P = P->ptr;
	   }
	}
	if ( match == FALSE ) {
	   P = ( fileRec_t *) malloc( sizeof( fileRec_t ) );
	   P->ptr = HDFfileList;
	   HDFfileList = P;
	   t = (char *)malloc( strlen( info->setName ) + 1 );
	   strcpy ( t, info->setName ) ;
	   P->fileName = t;
	   P->hdfID = info->setID;
	   P->PabloID = 0;
	} 
}  
/*======================================================================* 
// This routine assigns a unique Pablo ID to each unique name 		*
// regardless of the HDF tag.						*
// It then goes through the HDFRecordQueue and marks each record with   *
// the PabloID corresponding to the hdfID and xRef fields or 0.		*
//======================================================================*/
void HDFassignPabloIDs( int *nSetIDs, char ***Names )
{
	fileRec_t *F, *G;
	HDFnode_t *P, *Q;
	int j, hdfID, xRef, PabloID = 1;
	char *fName, **T;

	F = HDFfileList;
        /*==============================================================*
        // Assign the same ID to identical names.			*
        //==============================================================*/
	while ( F != NULL ) {
	   if ( F->PabloID == 0 ) {
	      F->PabloID = PabloID++;
	      fName = F->fileName;
	      G = F->ptr;
	      while ( G != NULL ) {
	         if ( strcmp( G->fileName , fName ) == 0 ) {
	            G->PabloID = F->PabloID;
	         }
	         G = G->ptr;
	      }
	   }
	   F = F->ptr;
	}
	*nSetIDs = PabloID - 1;
        /*==============================================================*
	// Repace hdfID and xRef fields with corresponding Pablo ID	*
        //==============================================================*/
	for ( j = 0; j < NumHDFProcs; ++j ) {
	   P = HDFQueues[j] ;
	   if ( P != NULL ) {
	      P = P->ptr ;
	      while ( P != NULL ) {
		 Q = P;
	         while ( Q != NULL ) {
	            hdfID = Q->record.hdfID;
	            xRef = Q->record.xRef;
	            PabloID = 0;
	            if ( hdfID != 0 ) {
                       F = HDFfileList;
	               while ( F != NULL && PabloID == 0 ) {
                          if ( hdfID == F->hdfID ) {
	                     PabloID = F->PabloID;
	                  }
	                  F = F->ptr;
	               }
	            }
		    Q->record.hdfID = PabloID;
	            PabloID = 0;
	            if ( xRef != 0 ) {
                       F = HDFfileList;
	               while ( F != NULL && PabloID == 0 ) {
                          if ( xRef == F->hdfID ) {
	                     PabloID = F->PabloID;
	                  }
	                  F = F->ptr;
	               }
	            }
		    Q->record.xRef = PabloID;
	            Q = Q->fptr;
	         }
	         P = P->ptr;
	      }
	   }
	}
	T = ( char ** )malloc( (*nSetIDs+1) * sizeof( char * ) );
	for ( j = 0; j <= *nSetIDs; ++j ) {
	   T[j] = NULL;
	}
	F = HDFfileList;
	while ( F != NULL ) {
	   PabloID = F->PabloID  ;
	   if ( T[PabloID] == NULL ) {
	      T[PabloID] = ( char * )malloc( strlen( F->fileName ) + 1 );
	      strcpy( T[PabloID], F->fileName ) ;
	   }
	   free(F->fileName);
	   G = F;
	   F = F->ptr;
	   free ( G );
	}
	*Names = T;
}
/*======================================================================* 
// This routine writes SDDF packets to SDDF file containing information	*
// about the named identifiers found in the program.			*
//======================================================================*/
void writeHDFNamePacketsRT( char **Names, int numSetIDs )
{
	int j;
	HDFNamePacket_t NamePkt;
	char *BUFF, *fName;
	int buffSize;
	/*==============================================================*
	// Allocate a buffer to hold the packet.  Allow 80 chars for 	*
	// identifier name.						*
	//==============================================================*/
	buffSize = sizeof(HDFNamePacket_t) + 80;
	BUFF = (char *)malloc(buffSize);
	/*==============================================================*
	// Fill in constant information					*
	//==============================================================*/
	NamePkt.packetType = PKT_DATA;
	NamePkt.packetTag = FAMILY_NAME;
	/*==============================================================*
	// Fill in named identifier information and write to SDDF file	*
	//==============================================================*/
	for ( j = 1; j <= numSetIDs; ++j ) {
	   fName = Names[j];
	   NamePkt.packetLength = sizeof(NamePkt) + strlen(fName) ;
	   NamePkt.fileType = 0;		/* not currently used	*/
	   NamePkt.fileID = j;
	   NamePkt.nameLen = strlen(fName) ;
	   if ( buffSize < NamePkt.packetLength ) {
	      free(BUFF) ;
	      buffSize = NamePkt.packetLength + 80;
	      BUFF = (char *)malloc( buffSize ) ;
	   }
	   /*===========================================================*
	   // Copy packet data and tack on identifier name		*
	   //===========================================================*/
	   memcpy( BUFF, &NamePkt, sizeof(NamePkt) );
	   memcpy( BUFF + sizeof(NamePkt) , fName, strlen(fName) );
	   putBytes( BUFF , NamePkt.packetLength ) ;
	}
	free(BUFF);
}
/*======================================================================*
// Tag xRef field of all records in this queue with the hdfID of the 	*
// highest level caller. Also						*
// This routine takes the records after they have been tagged and adds  *
// their fields to the apporopriate position in the HDFQueues structure *
//======================================================================*/
void HDFupdateProcLists( HDFnode_t *Rec )
{
	HDFnode_t *P, *Q;
	int hdfID, threadID ; 

	hdfID = Rec->record.hdfID;
	threadID = Rec->record.threadID;
	P = Rec->ptr;
	HDFupdateProcs( Rec, threadID );
	while ( P != NULL ) {
	   P->record.xRef = hdfID;
	   Q = P->ptr;
	   HDFupdateProcs( P, threadID );
	   P = Q;
	}
}
/*======================================================================*
// This routine takes as input a node pointer P and threadID and looks  *
// for a record with this same eventID, threadID, hdfID and xRef, then  *
// adds P to the record.  If no such record exists, one is created.	*
//======================================================================*/
void HDFupdateProcs( HDFnode_t *P, int threadID )
{
	int j, match, procIndex, hdfID, xRef, eventID;
	static int count = 0;
	HDFnode_t *Q, *R, *S, *T;
	eventID = P->eventID;
	procIndex = ProcIndexForHDFEntry( eventID );
	Q = HDFQueues[ procIndex ];
	/*==============================================================*
	// First determine if a queue even exists for this eventID.    	*
	//==============================================================*/
	if ( Q == NULL ) {
	   /*===========================================================*
	   // No queue exits.  Create one.                              *
	   //===========================================================*/
	   Q = (HDFnode_t *)malloc( sizeof(HDFnode_t) );
	   Q->ptr = NULL;
	   Q->fptr = NULL;
	   Q->record = InitNode.record;
	   Q->eventID = eventID;
	   Q->record.threadID = NEG_THREAD_ID;
	   HDFQueues[ procIndex ] = Q;
	} 
        R = Q;
	T = R->ptr;
	while ( T != NULL && T->record.threadID <= threadID ) {
	   R = T;
	   T = T->ptr;
 	}
        if ( R->record.threadID < threadID ) {
	   /*===========================================================*
	   // No queue member has this thread id so create one    	*
	   // and put the record info from P into it.			*
	   //===========================================================*/
	   T = (HDFnode_t *)malloc( sizeof(HDFnode_t) );
	   *T = InitNode;
	   T->ptr = R->ptr;
	   R->ptr = T;
	   T->fptr = NULL;
	   T->eventID = eventID;
	   T->record.threadID = threadID;
	   T->record.hdfID = P->record.hdfID;
	   T->record.xRef = P->record.xRef;
           HDFrecordSum ( &T->record , &P->record );
	} else {
	   /*===========================================================*
	   // A queue member exists with the same thread ID.  Now 	*
	   // go through the subqueue to find a match for hdfID 	*
	   // and xRef fields.						*
	   //===========================================================*/
	   hdfID = P->record.hdfID;
	   xRef = P->record.xRef;
	   S = R;
           T = R;
	   while ( S != NULL 
              && ( S->record.hdfID != hdfID || S->record.xRef != xRef ) ) {
	      T = S;
	      S = S->fptr;
	   }
	   if ( S == NULL ) {
	      /*========================================================*
	      // There was no match on both the hdfID and xRef,so make	*
	      // a new link in the subchain and initialize if with the 	*
	      // the record info from P.				*
	      //========================================================*/
	      S = ( HDFnode_t *)malloc( sizeof(HDFnode_t) ) ;
	      *S = InitNode;
	      T->fptr = S;
	      S->fptr = NULL;
	      S->eventID = eventID;
	      S->record.threadID = threadID;
	      S->record.hdfID = P->record.hdfID;
	      S->record.xRef = P->record.xRef;
              HDFrecordSum ( &S->record , &P->record );
	   } else {
	      /*========================================================*
	      // We have an exact match, so add the information from P  *
	      // to that of the record.					*
	      //========================================================*/
              HDFrecordSum ( &S->record , &P->record );
	   } 
	}
	free(P);
}
/*======================================================================*
// Print SDDF records for all records in this linked list.		*
//======================================================================*/
void HDFSummarySDDF( HDFnode_t *P, int procIndex )
{
	int eventID;
	HDFSummaryPacket_t Packet;

	HDFnode_t *Q, *R, *S;
	if ( P == NULL ) {
	   return;
	}
	P = P->ptr;
	eventID = HDFProcIXtoEventID( procIndex );
	Packet.packetLength = sizeof( Packet );
	Packet.packetTag = ( procIndex + 1 )*HDF_FAMILY | SUMMARY_TRACE 
	                                                | RECORD_TRACE ;
	Packet.packetType = PKT_DATA;
	Packet.eventID =  eventID ;
        while ( P != NULL ) {
	   R = P;
	   Q = P->ptr;
	   while ( R != NULL ) {
	      Packet.record = R->record;
	      putBytes( &Packet, Packet.packetLength );
	      S = R;
	      R = R->fptr;
	      free( S );
	   }
	   P = Q;
	} 
}
/*======================================================================*
// Initialize a node.							*
//======================================================================*/
void HDFnodeInit ( HDFnode_t *S ) 
{
	int j;
	*S = InitNode;
}
/*======================================================================*
// Place a node on a queue according to its thread id.  The queue	*
// consisits of linked lists.  add an element to the linked list if 	*
// there is none correponding to the thread id.				*
//======================================================================*/
void HDFenqueue( HDFnode_t *S, HDFnode_t *ThreadTally, int threadID )
{
	HDFnode_t *P, *Q;
	P = ThreadTally;
	Q = P->ptr;
	while ( Q != NULL && Q->record.threadID <= threadID ) {
	   P = Q;
	   Q = P->ptr;
	}
	if ( P->record.threadID < threadID ) {
	   S->record.threadID = threadID ;
	   S->ptr = P->ptr;
	   P->ptr = S;
	   S->fptr = NULL;
	} else {
	   Q = P->fptr;
	   while ( Q != NULL ) {
	     P = Q;
	     Q = P->fptr;
	   }
	   P->fptr = S;
	   S->fptr = NULL;
	}
}
/*======================================================================*
//      Compute IO totals, exclusive durations of the input record T    *
//      then add the fields of T to that of S.                          *
//======================================================================*/
void HDFrecordSum ( HDFrec_t *S, HDFrec_t *T )
{
        int j, allIOCount;
        double allIOTime;
        allIOTime = 0;
        for ( j = FirstIO; j <= LastIO; ++j ) {
           allIOTime += T->times[j];
        }
        T->times[AllIO] = allIOTime;
 
        allIOCount = 0;
        for ( j = FirstIO; j <= LastIO; ++j ) {
           allIOCount += T->counts[j];
        }
        T->counts[AllIO] = allIOCount;
 
        T->excDur = T->incDur - ( T->times[HDF_] + T->times[MPI]
                                                 + T->times[AllIO] );
 
        S->nCalls    += T->nCalls;
        S->firstCall =  min( S->firstCall, T->lastCall );
        S->lastCall  =  max( S->lastCall, T->lastCall );
        S->incDur    += T->incDur;
        S->excDur    += T->excDur;
        for ( j = 0; j < nTallyFields; ++j ) {
           S->times[j] += T->times[j] ;
        }
        for ( j = 0; j < nTallyFields; ++j ) {
           S->counts[j] += T->counts[j] ;
        }
        for ( j = 0; j < nByteFields; ++j ) {
           S->bytes[j] += T->bytes[j] ;
        }
}
/*======================================================================*
// Return the field index corresponding to an IO event ID.  The fields  *
// are specified in an enum statement in an include file.		*
//======================================================================*/
int getHDFFieldIndex( int eventID )
{
	int result;
	switch ( eventID )
	{
		case openBeginID:
		case openEndID:
		case fopenBeginID:
		case fopenEndID:
			result = Open;
			break;
		case closeBeginID:
		case closeEndID:
		case fcloseBeginID:
		case fcloseEndID:
			result = Close;
			break;
		case readBeginID:
		case readEndID:
		case freadBeginID:
		case freadEndID:
			result = Read;
			break;
		case lseekBeginID:
		case lseekEndID:
		case fseekBeginID:
		case fseekEndID:
			result = Seek;
			break;
		case writeBeginID:
		case writeEndID:
		case fwriteBeginID:
		case fwriteEndID:
			result = Write;
			break;
		case fflushBeginID:
		case fflushEndID:
		case flushBeginID:
		case flushEndID:
			result = Misc;
			break;
		case rewindBeginID:
		case rewindEndID:
		case fsetposBeginID:
		case fsetposEndID:
			result = Misc;
			break;
#ifdef	creadBeginID
		case creadBeginID:
		case creadEndID:
		case creadvBeginID:
		case creadvEndID:
			result = Read;
 			break;
		case cwriteBeginID:
		case cwriteEndID:
		case cwritevBeginID:
		case cwritevEndID:
			result = Write;
 			break;
		case ireadBeginID:
		case ireadEndID:
		case ireadvBeginID:
		case ireadvEndID:
			result = ARead;
 			break;
		case iwriteBeginID:
		case iwriteEndID:
		case iwritevBeginID:
		case iwritevEndID:
			result = AWrite;
 			break;
		case iowaitBeginID:
		case iowaitEndID:
			result = Wait;
 			break;
		case iodoneBeginID:
		case iodoneEndID:
			result = Misc;
 			break;
		case gopenBeginID:
		case gopenEndID:
			result = Open;
 			break;
		case iomodeBeginID:
		case iomodeEndID:
		case setiomodeBeginID:
		case setiomodeEndID:
		case lsizeBeginID:
		case lsizeEndID:
		case forflushBeginID:
		case forflushEndID:
			result = Misc;
 			break;
#endif	
	}
	return result;
}
/*======================================================================*
// This routine determines the field index in the bytes array of the 	*
// HDF records which correspond to a given IO operation.  If the  	*
// operation does not transfer bytes, (e.g., open operation), -1 is	*
// returned.								*
//======================================================================*/
int getHDFByteFieldIndex( int Operation ) 
{
	int result;
	switch ( Operation )
	{
		case Read:
	 		result = ReadBytes;
			break;
		case Write:
	 		result = WriteBytes;
			break;
		case ARead:
	 		result = AReadBytes;
			break;
		case AWrite:
	 		result = AWriteBytes;
			break;
		default:
			result = -1;
			break;
	}
	return result;
}
/*======================================================================*
// This routine writes the SDDF packet descriptors for the HDF summary	*
// records to the output file.						*
//======================================================================*/
static void _hdfDescriptorRT( char *recordName, char *recordDescription, int recordFamily, int numberFields )
{

    static char	recordBuffer[ 4096 ];
    int		recordLength;
    char	*eventIdAttrPtr;
    char	*hdfRecordPointer;
    int		eventIdAttrCnt;
    char        cbuf[128]; 

    hdfRecordPointer = recordBuffer;
    /********************************************************************/
    /* Allow space at the beginning of the record for the packet 	*/
    /*length which will be computed after the packet is complete.	*/
    /********************************************************************/
    sddfWriteInteger( &hdfRecordPointer, 0 );
    /********************************************************************/
    /* The record type, tag, and name 					*/
    /********************************************************************/
    sddfWriteInteger( &hdfRecordPointer, PKT_DESCRIPTOR );
    sddfWriteInteger( &hdfRecordPointer, ( recordFamily | RECORD_TRACE ) );
    sddfWriteString( &hdfRecordPointer, recordName );
    /********************************************************************/
    /* The record attribute count and string pair 			*/
    /********************************************************************/
    sddfWriteInteger( &hdfRecordPointer, 1 );
    sddfWriteString( &hdfRecordPointer, "description" );
    sddfWriteString( &hdfRecordPointer, recordDescription );
    /********************************************************************/
    /* The record field count 						*/
    /********************************************************************/
    sddfWriteInteger( &hdfRecordPointer, numberFields );
   
    WRITE_HDF_FIELD( "Event Identifier",
                     "Event ID",
                     "Corresponding Event",
                     INTEGER, 0 );
    WRITE_HDF_FIELD( "First Call", 
	   	     "First Call", 
                     "Time of First Call to this Procedure", 
		      DOUBLE, 0 );
    WRITE_HDF_FIELD( "Last Call", 
	   	     "Last Call", 
                     "Time of Last Call to this Procedure", 
		      DOUBLE, 0 );
    WRITE_HDF_FIELD( "Inclusive Duration", 
  	             "Inclusive Duration", 
	             "Inclusive Duration of this Procedure",
		      DOUBLE, 0 );
    WRITE_HDF_FIELD( "Exclusive Duration", 
  	             "Exclusive Duration", 
	             "Excludes IO, MPI-IO and other HDF calls",
		      DOUBLE, 0 );
    WRITE_HDF_FIELD( "HDF Duration", 
 	             "HDF Duration", 
                     "Total Time of other HDF procs called by this Proc",
		      DOUBLE, 0 );
    WRITE_HDF_FIELD( "MPIIO Duration", 
		     "MPIIO Duration", 
	             "All MPI-IO time only in the procedure",
		      DOUBLE, 0 );
    WRITE_HDF_FIELD( "IO Duration", 
		     "IO Duration", 
	             "All IO time only in the procedure",
		      DOUBLE, 0 );
    WRITE_HDF_FIELD( "Open Duration", 
  	             "Open Duration", 
                     "Open and gOpen time only in this procedure",
		     DOUBLE, 0 );
    WRITE_HDF_FIELD( "Close Duration", 
	             "Close Duration", 
	             "Close time only in this procedure",
	             DOUBLE, 0 );
    WRITE_HDF_FIELD( "Read Duration", 
	             "Read Duration", 
	             "Read time only in this procedure",
		     DOUBLE, 0 );
    WRITE_HDF_FIELD( "Asynch Read Duration", 
	             "Asynch Read Duration", 
                     "Asynch Read time only in this procedure",
		     DOUBLE, 0 );
    WRITE_HDF_FIELD( "Write Duration", 
                     "Write Duration", 
                     "Write time only in this procedure",
		     DOUBLE, 0 );
    WRITE_HDF_FIELD( "Asynch Write Duration", 
		     "Asynch Write Duration", 
                     "Asynch Write time only in this procedure",
		     DOUBLE, 0 );
    WRITE_HDF_FIELD( "Seek Duration", 
                     "Seek Duration", 
                     "Seek time only in this procedure",
		     DOUBLE, 0 );
    WRITE_HDF_FIELD( "Wait Duration", 
                     "Wait Duration", 
                     "Wait time only in this procedure",
		     DOUBLE, 0 );
    WRITE_HDF_FIELD( "Misc IO Duration", 
                     "Misc IO Duration", 
                     "Misc IO time only in this procedure",
		     DOUBLE, 0 );
    WRITE_HDF_FIELD( "HDF Operation Count", 
		     "HDF Operation Count", 
	             "Count HDF calls in the procedure", 
		     INTEGER, 0 );
    WRITE_HDF_FIELD( "MPIIO Operation Count", 
		     "MPIIO Operation Count", 
	             "Count MPI-IO calls in this procedure", 
		     INTEGER, 0 );
    WRITE_HDF_FIELD( "IO Operation Count", 
		     "IO Operation Count", 
	             "Count All IO ops done in this procedure", 
		     INTEGER, 0 );
    WRITE_HDF_FIELD( "Open Count", 
		     "Open Count", 
	             "Count Opens done in this procedure",
		     INTEGER, 0 );
    WRITE_HDF_FIELD( "Close Count", 
		     "Close Count", 
	             "Count Closes done in this procedure",
		     INTEGER, 0 );
    WRITE_HDF_FIELD( "Read Count", 
		     "Read Count",
	             "Count Reads done in this procedure",
		     INTEGER, 0 );
    WRITE_HDF_FIELD( "Asynch Read Count", 
		     "Asynch Read Count", 
	             "Count Asynch Reads done in this procedure",
		     INTEGER, 0 );
    WRITE_HDF_FIELD( "Write Count", 
		     "Write Count",
	             "Count Writes done in this procedure",
		     INTEGER, 0 );
    WRITE_HDF_FIELD( "Asynch Write Count", 
		     "Asynch Write Count", 
	             "Count Asynch Writes done in this procedure",
		     INTEGER, 0 );
    WRITE_HDF_FIELD( "Seek Count", 
		     "Seek Count",
	             "Count Seeks done in this procedure",
		     INTEGER, 0 );
    WRITE_HDF_FIELD( "Wait Count", 
		     "Wait Count",
	             "Count Waits done in this procedure",
		     INTEGER, 0 );
    WRITE_HDF_FIELD( "Misc IO Count", 
		     "Misc IO Count",
	             "Count Misc IO ops done in this procedure",
		     INTEGER, 0 );
    WRITE_HDF_FIELD( "Read Number Bytes", 
		     "Read Number Bytes",
	             "Bytes Read in this procedure",
		     INTEGER, 0 );
    WRITE_HDF_FIELD( "Asynch Read Number Bytes", 
		     "Asynch Read Number Bytes", 
	             "Bytes Asynch Read in this procedure",
		     INTEGER, 0 );
    WRITE_HDF_FIELD( "Write Number Bytes", 
		     "Write Number Bytes",
	             "Bytes Written in this procedure",
		     INTEGER, 0 ); 
    WRITE_HDF_FIELD( "Asynch Write Number Bytes", 
		     "Asynch Write Number Bytes", 
	             "Bytes Asynch Written in this proc",
		     INTEGER, 0 ); 
    WRITE_HDF_FIELD( "Processor Number", 
		     "Node", 
		     "Processor number", 
		     INTEGER, 0 );
    WRITE_HDF_FIELD( "Number of Calls", 
		     "Number of Calls", 
		     "Number of Calls Summarized in this Packet", 
		     INTEGER, 0 );
    WRITE_HDF_FIELD2("HDF ID",
                     "HDF ID", "Identifier number",
                     "0", "No HDF ID specified",
                     INTEGER, 0 );
    WRITE_HDF_FIELD( "Xref ID",
                     "Cross Reference", 
                     "Index of related HDF ID or 0 if none",
		     INTEGER, 0 );
    /********************************************************************/
    /* The entire record descriptor packet has been written.		*/
    /* Compute and update the record length.				*/
    /* Write the completed record.					*/
    /********************************************************************/

    recordLength = hdfRecordPointer - recordBuffer;

    hdfRecordPointer = recordBuffer;
    sddfWriteInteger( &hdfRecordPointer, recordLength );

    putBytes( recordBuffer, (unsigned) recordLength );
}

/************************************************************************/
/*   Internal Routine:  writeHDFRecDescrptrsRT	                        */
/*                      Writes record descriptors for the HDF events.   */
/************************************************************************/
void writeHDFRecDescrptrsRT() 
{
	int j;
        char BUF1[256], BUF2[256] ;
	_hdfNameDescriptor();	/* Descriptor for named identifiers	*/
        for ( j = 0; j < NumHDFProcs; ++j ) {
           if ( HDFQueues[j] != NULL ) {
              strcpy( BUF2, "HDF ");
              strcat( BUF2, HDFProcNames[j] );
              strcat( BUF2, " Procedure Summary");
              strcpy( BUF1, BUF2 );
              strcat( BUF1, " Trace");
              _hdfDescriptorRT( BUF1, BUF2, 
                               (j+1)*HDF_FAMILY + SUMMARY_TRACE, 
                               SUMMARY_NUM_FIELDS );
           }
        }
        return;
}
/*======================================================================*
// This routine prints the Pablo IDs assigned to named HDF identifiers  *
//======================================================================*/
void printFileMappingsRT( char *mapFile, char **Names, int nPabloIDs )
{
	int i;
	FILE *ptr;
	ptr = fopen( mapFile, "w" );
 
        if ( ptr == NULL ) {
	    printf("Couldn't open map file %s - none created.\n",mapFile);
            return;
        }
 
	fprintf(ptr,"\n\nPablo ID to HDF Name mappings:\n");
	fprintf(ptr,"------------------------------\n");
        for ( i = 1; i <= nPabloIDs; i++ ) {
	     fprintf(ptr,"%4d %s\n",i,Names[i] );
	}
	fprintf(ptr,"\n\n");
	fclose( ptr );
}
/************************************************************************/
/*	_hdfNameDescriptor	     					*/
/*	   Generate a SDDF binary format record descriptor for the	*/
/*	   named identifiers used during execution.              	*/
/************************************************************************/
void _hdfNameDescriptor()
{
    static char recordBuffer[ 4096 ];
    int         recordLength;
    char	*hdfRecordPointer;
    char        cbuf[128];

#ifdef DEBUG
	fprintf( debugFile, "_hdfExitTraceDescriptor entered\n" );
	fflush( debugFile );
#endif /* DEBUG */
    hdfRecordPointer = recordBuffer;
    /********************************************************************/
    /* Allow space at the beginning of the record for the packet        */
    /*length which will be computed after the packet is complete.       */
    /********************************************************************/
    sddfWriteInteger( &hdfRecordPointer, 0 );
    /********************************************************************/
    /* The record type, tag, and name                                   */
    /********************************************************************/
    sddfWriteInteger( &hdfRecordPointer, PKT_DESCRIPTOR );
    sddfWriteInteger( &hdfRecordPointer, ( FAMILY_NAME ) );
    sddfWriteString( &hdfRecordPointer, "HDF Name Identifier Record" );
    /********************************************************************/
    /* The record attribute count and string pair                       */
    /********************************************************************/
    sddfWriteInteger( &hdfRecordPointer, 1 );
    sddfWriteString( &hdfRecordPointer, "description" );
    sddfWriteString( &hdfRecordPointer, "HDF Name Identifier Record" );
    /********************************************************************/
    /* The record field count                                           */
    /********************************************************************/
    sddfWriteInteger( &hdfRecordPointer, 3);
    /********************************************************************/
    /* Create fields                                               	*/
    /********************************************************************/
    WRITE_HDF_FIELD(  "Identifier Type", 
		      "Data Set Type", 
		      "Data Set Identifier Type", 
		      INTEGER, 0 );
    WRITE_HDF_FIELD2( "HDF ID",
                      "HDF ID", "File, Data Set or Dim Identifier number",
                      "0", "No HDF ID specified",
                      INTEGER, 0 ); 
    WRITE_HDF_FIELD( "HDF Name",
                     "HDF Name", "Name of File, Data Set or Dim",
                      CHARACTER, 1 );

    recordLength = hdfRecordPointer - recordBuffer;

    hdfRecordPointer = recordBuffer;
    sddfWriteInteger( &hdfRecordPointer, recordLength );

    putBytes( recordBuffer, (unsigned) recordLength );
}
