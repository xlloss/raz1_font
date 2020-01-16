/******************************************************************************
* Copyright(c) 2010-2012 Renesas Electronics Corporation. All rights reserved.
*
* brief  : NCG User/Driver space bridge function.
*
* author : Renesas Electronics Corporation
*
* history: 2010.10.08
*          - Created the initial code.
*          2012.08.22
*          - Updated coding format.
*
*******************************************************************************/

#ifndef NCG_IOCTL_H
#define NCG_IOCTL_H

#ifdef __cplusplus
extern "C" {
#endif


/*=============================================================================
 * Definitions
 */

typedef NCGint32 (* NCGIOControlHandler)(NCGint32, NCGvoid *);


/*=============================================================================
 *  Structures
 */


/*=============================================================================
 *  Function prototyping
 */

NCGint32
NCGSYS_RegisterDevice (
    NCGuint32               ui32DeviceID,
    NCGIOControlHandler     pfnIOControl
);

NCGint32
NCGSYS_UnregisterDevice (
    NCGuint32   ui32DeviceID
);

NCGint32
NCGSYS_DeviceIOControl (
    NCGuint32   ui32DeviceID,
    NCGint32    i32FunctionID,
    NCGuint32   ui32InBufferSize,
    NCGvoid     *pInBuffer,
    NCGuint32   ui32OutBufferSize,
    NCGvoid     *pOutBuffer
);

#ifdef __cplusplus
}
#endif

#endif /* NCG_IOCTL_H */
