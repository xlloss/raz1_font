/******************************************************************************
* Copyright(c) 2010-2012 Renesas Electronics Corporation. All rights reserved.
*
* brief  : NCG Message Queue control functions header.
*
* author : Renesas Electronics Corporation
*
* history: 2011.03.04
*          - Created the initial code.
*          2012.08.22
*          - Updated coding format.
*
*******************************************************************************/

#ifndef NCG_QUEUE_H
#define NCG_QUEUE_H

#ifdef __cplusplus
extern "C" {
#endif


/*=============================================================================
 * Definitions
 */


/*=============================================================================
 *  Structures
 */


/*=============================================================================
 *  Function prototyping
 */

NCGint32
NCGSYS_CreateQueue (
    NCGvoid     **ppObj,
    NCGuint32   ui32QueueNum,
    NCGuint32   ui32Flags
);

NCGint32
NCGSYS_DestroyQueue (
    NCGvoid     *pObj
);

NCGint32
NCGSYS_SendQueue (
    NCGvoid     *pObj,
    NCGuint32   ui32DataSize,
    NCGvoid     *pData,
    NCGuint32   ui32TimeOut
);

NCGint32
NCGSYS_ReceiveQueue (
    NCGvoid     *pObj,
    NCGuint32   *ui32RcvSize,
    NCGuint32   ui32DataSize,
    NCGvoid     **ppData,
    NCGuint32   ui32TimeOut
);

#ifdef __cplusplus
}
#endif

#endif /* NCG_QUEUE_H */
