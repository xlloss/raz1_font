/******************************************************************************
* Copyright(c) 2010-2012 Renesas Electronics Corporation. All rights reserved.
*
* brief  : NCG mutex operation functions header.
*
* author : Renesas Electronics Corporation
*
* history: 2010.10.08
*          - Created the initial code.
*          2012.08.22
*          - Updated coding format.
*
*******************************************************************************/

#ifndef NCG_MUTEX_H
#define NCG_MUTEX_H

#ifdef __cplusplus
extern "C" {
#endif

/*=============================================================================
 * Definitions
 */

/*
 *  Mode Flags
 */
#define NCGSYS_MUTEX_CALL_NORMAL     (0U)
#define NCGSYS_MUTEX_CALL_INTERRUPT  (1U)
#define NCGSYS_MUTEX_CALL_CALLBACK   (2U)
#define NCGSYS_MUTEX_CALL_MASK       (3U)


/*=============================================================================
 *  Structures
 */


/*=============================================================================
 *  Function prototyping
 */

/*
 *  Mutual exclusion control
 */

NCGint32
NCGSYS_CreateMutex (
    NCGvoid     **ppObj,
    NCGuint32   ui32MutexID
);

NCGint32
NCGSYS_DestroyMutex (
    NCGvoid     *pObj
);

NCGint32
NCGSYS_AcquireMutex (
    NCGvoid     *pObj,
    NCGuint32   ui32Flags,
    NCGuint32   ui32Timeout
);

NCGint32
NCGSYS_ReleaseMutex (
    NCGvoid     *pObj,
    NCGuint32   ui32Flags
);


#ifdef __cplusplus
}
#endif

#endif /* NCG_MUTEX_H */
