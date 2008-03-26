#ifndef _ES_INFO_H
#define _ES_INFO_H

#ifdef __cplusplus
extern "C" {
#endif
#ifdef LARGE_VOL_SUPPORT 
/* ************************************************** */
/*                                                    */
/* keep track of where we have scanned so far for the */
/* synchronous data we received                       */
/*                                                    */
/* ************************************************** */

typedef struct enumScanInfo {
   char *base;     /* base pointer to response data   */
   int   eodoff;   /* end of the response data so far */
   int   ssecoff;  /* start of section offset         */  
   int   curoff;   /* current offset                  */
   char *section;  /* buffer pointer                  */
   int   sectlen;  /* buffer length                   */
   int   prevtotl; /* previous total received bytes   */
   int   recdtotl; /* total received bytes enumwriteCB*/
   int   getnew; 
   int   parsestate;
} enumScanInfo ;
/*
 * parsing state definitions 
 *
 */
#define PARSTATE_INIT            00 /* parsing hasnot started yet          */
#define PARSTATE_STARTED         05 /* at least some data has been parsed  */
#define PARSTATE_COMPLETE        10 /* parseing is complete                */
#define PARSTATE_SERVER_TIMEOUT  20 /* timeout from getting data           */


struct asyncrespcntl{
        int          xfer_state ;   /* The state of the transfer           */
        int          eMethodType;   /* type of enumeration in progress     */
struct native_enum  *enmp ;         /* enumeration pointer                 */
pthread_mutex_t      escanlock ;    /* mutex lock                          */
pthread_mutex_t      xfer_cond_mutex ; 
pthread_cond_t       xfer_cond       ;
struct enumScanInfo  escanInfo;     /* response control                    */         	
} asyncrespcntl ;
/*
 * asyncrespcntl info
 *
 * escanlock - lock the use of escanInfo
 *
 * xfer_cond      -pthread condition variable which holds us from 
 *                 continuing until we have received something 
 *                 from the server .
 *                 wait in getEnumResonse
 *                 set in enumWriteHeaders
 *
 */


/*
 * xfer_states 
 */
#define XFER_NOT_STARTED   0       /* no valid data yet                */
#define XFER_GENRQST       5       /* we have gotten the request built */
                                   /* and will be sending it next.     */
#define XFER_RESP_TEC     10       /* we received a response header    */
                                   /* with Transfer Encoding: chunked  */
#define XFER_RESP_CL      15       /* we received a response header    */
                                   /* with content-length              */
#define XFER_DATA_RECVD   20       /* we have recieved some data       */
#define XFER_COMPLETE     25       /* xfer is complete                 */ 
#define XFER_ERROR        30       /* we had an error                  */   
/*
 * Enumeration request types 
 */
#define ENUMERATEINSTANCES       1
#define ENUMERATEINSTANCENAMES   2
#define ENUMERATECLASSES         3
#define ENUMERATECLASSNAMES      4

/*
 * macros and prototypes 
 */
#define B(x) (x->base)
#define O(x) (x->curoff)

inline char * CURPTR  (enumScanInfo * );
inline char * SSECPTR (enumScanInfo * );
inline char * LASTPTR (enumScanInfo * );
inline void   INCOFF  (enumScanInfo * );

#endif /* ifdef LARGE_VOL_SUPPORT */

#ifdef __cplusplus
 }
 
#endif

#endif
