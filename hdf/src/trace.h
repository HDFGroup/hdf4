#ifdef HAVE_PABLO 
#include "ProcIDs.h"
#define HDF_TRACE_ON( eventID ) \
        if (procTrace & HDF_mask)  startHDFtraceEvent(eventID)
#define HDF_TRACE_OFF(  eventID, p1, p2, p3 ) \
        if (procTrace & HDF_mask)  endHDFtraceEvent(-eventID, p1, p2, p3 )
#define HDF_MASK_TRACE_ON( mask, eventID ) \
        if (procTrace & mask)  startHDFtraceEvent(eventID)
#define HDF_MASK_TRACE_OFF( mask, eventID, p1, p2, p3 ) \
        if (procTrace & mask)  endHDFtraceEvent(-eventID, p1, p2, p3 )
#define TRACE_ON(mask, eventID) \
        if (procTrace & mask) startHDFtraceEvent(eventID)
#define TRACE_OFF(mask, eventID ) \
        if (procTrace & mask)  endHDFtraceEvent(-eventID, NULL, 0, 0 )
#else
#define HDF_TRACE_ON( eventID )
#define HDF_TRACE_OFF( eventID, p1, p2, p3 )
#define HDF_MASK_TRACE_ON( mask, eventID )
#define HDF_MASK_TRACE_OFF( mask, eventID, p1, p2, p3 )
#define TRACE_ON(mask, eventID)
#define TRACE_OFF(mask, eventID )
#endif /* HAVE_PABLO */
