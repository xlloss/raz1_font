/******************************************************************************
* Copyright(c) 2010-2013 Renesas Electronics Corporation. All rights reserved.
*
* brief  : NCG Display Unit interrupt handling functions header.
*
* author : Renesas Electronics Corporation
*
* history: 2010.10.08
*          - Created the initial code.
*          2012.08.22
*          - Updated coding format.
*          2013.01.08
*          - Applied to VDC5 driver for Aragon.
*          2013.02.21
*          - Modified the value of NCGDU_VDC5_INT_PRIORITY
*
*******************************************************************************/


#ifndef NCG_DU_ISR_H
#define NCG_DU_ISR_H

#ifdef __cplusplus
extern "C" {
#endif


/*=============================================================================
 * Definitions
 */

#define NCGDU_VDC5_INT_PRIORITY         (14)


/*=============================================================================
 *  Structures
 */


/*=============================================================================
 *  Function prototyping
 */

/*
 *  DU interrupt handling
 */

NCGint32
NCGDU_Attach_ISR (
    NCGbitfield ui32Display,
    NCGDUISRfp  pfnInterrupt
);

NCGint32
NCGDU_Detach_ISR (
    NCGbitfield ui32Display,
    NCGDUISRfp  pfnInterrupt
);


#ifdef __cplusplus
}
#endif

#endif /* NCG_DU_ISR_H */
