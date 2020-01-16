/******************************************************************************
* Copyright(c) 2010-2012 Renesas Electronics Corporation. All rights reserved.
*
* brief  : NCG thread/proccess related operations header.
*
* author : Renesas Electronics Corporation
*
* history: 2010.10.08
*          - Created the initial code.
*          2011.03.04
*          - The following functions is added.
*            - NCGSYS_CreateThread, NCGSYS_DestroyThread, NCGSYS_StartThread,
*              NCGSYS_StopThread
*          2011.03.05
*          - Added the definitions of thread priority
*          2012.08.22
*          - Updated coding format.
*
*******************************************************************************/

#ifndef NCG_THREAD_H
#define NCG_THREAD_H

#ifdef __cplusplus
extern "C" {
#endif


/*=============================================================================
 * Definitions
 */

/*
 *  Thread priority
 */
enum
{
    NCG_THREAD_PRI_1 = (0),                     /* Highest Priority */
    NCG_THREAD_PRI_2 = NCG_THREAD_PRI_1 + 1,    
    NCG_THREAD_PRI_3 = NCG_THREAD_PRI_1 + 2,    
    NCG_THREAD_PRI_4 = NCG_THREAD_PRI_1 + 3,    /* Middle Priority */
    NCG_THREAD_PRI_5 = NCG_THREAD_PRI_1 + 4,    
    NCG_THREAD_PRI_6 = NCG_THREAD_PRI_1 + 5,    
    NCG_THREAD_PRI_7 = NCG_THREAD_PRI_1 + 6     /* Lowest Priority */
};


/*=============================================================================
 *  Structures
 */


/*=============================================================================
 *  Function prototyping
 */

/*
 *  Process/Thread ID
 */
NCGint32 
NCGSYS_GetCurrentProcessID (
    void
);

NCGint32 
NCGSYS_GetCurrentThreadID (
    void
);

/*
 *  Thread Local Storage(TLS) operations
 */
NCGuint32 
NCGSYS_CreateTLS (
    void
);

NCGint32 
NCGSYS_DestroyTLS (
    NCGuint32   ui32TlsIndex
);

NCGint32 
NCGSYS_SetTLSValue (
    NCGuint32   ui32TlsIndex,
    NCGvoid     *pTlsValue
);

NCGvoid* 
NCGSYS_GetTLSValue (
    NCGuint32   ui32TlsIndex
);

NCGint32 
NCGSYS_CreateThread (
    NCGvoid     **ppObj,
    NCGfp       pfnThread,
    NCGvoid     *pParam,
    NCGint32    i32Priority
);

NCGint32 
NCGSYS_DestroyThread (
    NCGvoid     *pObj
);

NCGint32 
NCGSYS_StartThread (
    NCGvoid     *pObj
);

NCGint32 
NCGSYS_StopThread (
    NCGvoid     *pObj
);


#ifdef __cplusplus
}
#endif

#endif /* __NCG_THREAD_H__ */
