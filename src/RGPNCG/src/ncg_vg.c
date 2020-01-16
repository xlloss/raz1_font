/******************************************************************************
* Copyright(c) 2010-2013 Renesas Electronics Corporation. All rights reserved.
*
* brief  : R-GPVG control functions.
*
* author : Renesas Electronics Corporation
*
* history: 2011.01.20
*          - Created the initial code.
*          2012.08.22
*          - Added return value to "NCGVG_InitResourceSize"
*          2013.05.13
*          - Added function of initialization.
*          - Added function of finalization.
*          2013.06.10
*          - Applied to new VDC5 driver (Version 0.03.0046).
*          2013.11.07
*          - Modified value of NCG_VG_SBO1_SIZE to (4U) from (0U).
*
*******************************************************************************/

/*=============================================================================
 * Includes
 */

#include "ncg_defs.h"
#include "ncg_debug.h"

#include "ncg_vg.h"

/* Depending on the build environment */
#include "r_typedefs.h"

#include "iodefine.h"
#include "rza_io_regrw.h"


/*=============================================================================
 * Internal definitions
 */

/* TODO:
    Please change these parameters in accordance with your system. 
 */
#define NCG_VG_WB_STRIDE    ((64U   +  63U) & ~63U)
#define NCG_VG_WB_HEIGHT    ((480U   +  7U)  & ~7U)

#define NCG_VG_SBO0_SIZE    (((16384U) + 4U) & ~4U)
#define NCG_VG_SBO1_SIZE    (4U)
#define NCG_VG_DLB_SIZE     ((5120U  + 31U)  & ~31U)

/*=============================================================================
 *  Prototyping of internal functions
 */
#define MSTP100         (0x01u)
#define STBRQ20         (0x01u)


/*=============================================================================
 *  Private global variables and functions
 */


/*=============================================================================
 *  Global Function
 */

/*----------------------------------------------------------------------------
 NAME       : NCGVG_Init
 FUNCTION   : Initialize R-GPVG.
 PARAMETERS : pVGInfo : [IN ] The pointer to the initialization information struct.
 RETURN     : None.
------------------------------------------------------------------------------*/
NCGvoid
NCGVG_Init (
    PNCGVGINFO pVGInfo)
{
    /* Depending on the build environment */
    volatile uint8_t    reg_value;

    NCG_UNREFERENCED_PARAMETER( pVGInfo );

    NCG_DEBUG_PRINT_STRING( "[LOG] NCGVG_Init" );

    reg_value = CPG.STBCR10;
    if ( CPG.STBCR10 & MSTP100 )
    {
        /* Standby control register 10 (STBCR10) : 0 : R-GPVG enable */
        CPG.STBCR10 &= ~MSTP100;
        /* dummy read */
        reg_value = CPG.STBCR10;
    }

    return ;
}

/*----------------------------------------------------------------------------
 NAME       : NCGVG_DeInit
 FUNCTION   : Finalize R-GPVG.
 PARAMETERS : pVGInfo : [IN ] The pointer to the finalization information struct.
 RETURN     : None.
------------------------------------------------------------------------------*/
NCGvoid 
NCGVG_DeInit (
    PNCGVGINFO      pVGInfo )
{
    /* Depending on the build environment */

    NCG_UNREFERENCED_PARAMETER(pVGInfo);

    NCG_DEBUG_PRINT_STRING( "[LOG] NCGVG_DeInit" );

    if ( (CPG.STBCR10 & MSTP100) != MSTP100 )
    {
        CPG.STBREQ2 |= STBRQ20;
        while ( (CPG.STBACK2 & CPG_STBREQ2_STBRQ20) == 0  ) ;

        /* Standby control register 10 (STBCR10) : 0 : R-GPVG disable */
        CPG.STBCR10 |= MSTP100;
    }

    return ;
}

/*----------------------------------------------------------------------------
 NAME       : NCGVG_InitResourceSize
 FUNCTION   : Initialize VG resource size.
 PARAMETERS : pRGPInfo : [IN ] The pointer to the initialization information struct.
 RETURN     : 0 is success.
------------------------------------------------------------------------------*/
NCGint32
NCGVG_InitResourceSize (
    PNCGRGPINFO pRGPInfo)
{
    pRGPInfo->ui32WorkBufferStride      = NCG_VG_WB_STRIDE;
    pRGPInfo->ui32WorkBufferHeight      = NCG_VG_WB_HEIGHT;
    pRGPInfo->ui32StreamBuffer0Size     = NCG_VG_SBO0_SIZE;
    pRGPInfo->ui32StreamBuffer1Size     = NCG_VG_SBO1_SIZE;
    pRGPInfo->ui32DisplayListBufferSize = NCG_VG_DLB_SIZE;

    return 0;
}

/* -- end of file -- */
