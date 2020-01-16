/******************************************************************************
* Copyright(c) 2012 Renesas Electronics Corporation. All rights reserved.
*
* brief  : Sample code of NCG memory operation functions header.
*
* author : Renesas Electronics Corporation
*
* history: 2013.01.08
*          - Created new code.
*
******************************************************************************/

#ifndef NCG_MEMORY_H
#define NCG_MEMORY_H

#ifdef __cplusplus
extern "C" {
#endif


/*=============================================================================
 * Definitions
 */

/* Allocate frame memory flag type */
#define NCG_FRM_MEM_ALLOC_NO_FLAG        (0x00000000U)
#define NCG_FRM_MEM_ALLOC_FRM_BUF_FLAG   (0x00000001U)
#define NCG_FRM_MEM_ALLOC_WB0_FLAG       (0x00000002U)
#define NCG_FRM_MEM_ALLOC_WB1_FLAG       (0x00000004U)
#define NCG_FRM_MEM_ALLOC_SBO0_FLAG      (0x00000008U)
#define NCG_FRM_MEM_ALLOC_SBO1_FLAG      (0x00000010U)
#define NCG_FRM_MEM_ALLOC_SBO2_FLAG      (0x00000020U)
#define NCG_FRM_MEM_ALLOC_DL_FLAG        (0x00000040U)


/*=============================================================================
 *  Structures
 */


/*=============================================================================
 *  Function prototyping
 */

/*
 * Host memory management
 */

NCGvoid 
NCGSYS_PurgeCPUOperandCache (
    void
);

NCGuint32 
NCGSYS_CPUVAddrToSysPAddr (
    NCGuint32   ui32CPUAddr
);

NCGint32 
NCGSYS_HostMemCreate (
    NCGint32    i32Size,
    NCGuint32   ui32Flags
);

NCGint32 
NCGSYS_HostMemDestroy (
    void
);

NCGint32 
NCGSYS_HostMemAlloc (
    NCGint32    i32Size,
    NCGuint32   ui32Flags,
    NCGvoid     **ppMemAddr
);

NCGint32 
NCGSYS_HostMemFree (
    NCGvoid     *pMemAddr
);

NCGvoid 
NCGSYS_HostMemSet (
    NCGvoid     *pMemAddr,
    NCGuint8    uc,
    NCGint32    count
);

NCGvoid 
NCGSYS_HostMemCopy (
    NCGvoid     *pDstAddr,
    NCGvoid     *pSrcAddr,
    NCGint32    i32Count
);


/*
 *  Memory mapping
 */

NCGvoid 
*NCGSYS_MapPAddrToVAddr (
    NCGvoid     *pPAddr,
    NCGint32    i32Size,
    NCGuint32   ui32Flags
);

NCGvoid 
NCGSYS_UnmapPAddrToVAddr (
    NCGvoid     *pVAddr,
    NCGint32    i32Size
);

NCGvoid*
NCGSYS_GetPAddrFromVAddr (
    NCGvoid     *pVAddr
);

NCGvoid*
NCGSYS_MapVAddrToVAddr (
    NCGvoid     *pVAddr
);

NCGvoid 
NCGSYS_UnmapVAddrToVAddr (
    NCGvoid     *pVAddr
);


/*
 *  Frame memory management
 */

NCGint32 
NCGSYS_FrameMemCreate (
    NCGuint32   ui32Base,
    NCGint32    i32Size,
    NCGuint32   ui32Flags
);

NCGint32 
NCGSYS_FrameMemDestroy (
    void
);

NCGint32 
NCGSYS_FrameMemAlloc (
    NCGint32    i32Size,
    NCGint32    i32Align,
    NCGuint32   ui32Flags,
    NCGvoid     **ppMem
);

NCGint32 
NCGSYS_FrameMemFree (
    NCGvoid     *pMem
);

NCGint32 
NCGSYS_FrameMemWrap (
    NCGvoid     *pBase,
    NCGint32    i32Size,
    NCGint32    i32Align,
    NCGuint32   ui32Flags,
    NCGvoid     **ppMem
);

NCGvoid* 
NCGSYS_FrameMemToCPUVAddr (
    NCGvoid     *pMem
);

NCGuint32 
NCGSYS_FrameMemToDevVAddr (
    NCGvoid     *pMem
);

NCGuint32 
NCGSYS_FrameMemToSysPAddr (
    NCGvoid     *pMem
);

NCGuint32 
NCGSYS_FrameMemToFbAddr (
    NCGvoid     *pMem
);


#ifdef __cplusplus
}
#endif

#endif /* NCG_MEMORY_H */
