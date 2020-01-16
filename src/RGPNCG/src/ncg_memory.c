/******************************************************************************
* Copyright(c) 2010-2012 Renesas Electronics Corporation. All rights reserved.
*
* brief  : NCG memory operation functions.
*
* author : Renesas Electronics Corporation
*
* history: 2010.10.08
*          - Created the initial code.
*          2012.08.23
*          - Created the initial code.
*          2013.01.08
*
*******************************************************************************/

/*=============================================================================
 * Includes
 */

#include <stdlib.h>

#include "ncg_defs.h"
#include "ncg_debug.h"
#include "ncg_memory.h"


/*=============================================================================
 * Internal definitions
 */

typedef struct T_NCG_FrameMemInfo {
    NCGuint32               ui32PhysAddr;
    NCGint32                i32Size;
    NCGvoid                 *pvCPUVAddr;
    NCGuint32               ui32DevVAddr;
    NCGuint32               ui32FBAddr;
} NCG_FrameMemInfo_t;

typedef struct T_NCG_MemBlock {
    NCGuint32               ui32StartAddr;
    NCGint32                i32Size;
    NCG_FrameMemInfo_t      *pFInfo;
    struct T_NCG_MemBlock   *pNext;
} NCG_MemBlock_t;


/*=============================================================================
 *  Prototyping of internal functions
 */

static NCG_MemBlock_t* CreateMemBlock( NCGuint32 ui32StartAddr, NCGint32 i32Size );


/*=============================================================================
 *  Private global variables
 */

NCGuint32           g_frameMemStart     = 0xffffffffU;
NCGuint32           g_frameMemSize      = 0;

NCG_MemBlock_t      *g_pFreeBlocks      = NCG_NULL;
NCG_MemBlock_t      *g_pAllocatedBlocks = NCG_NULL;


/*=============================================================================
 *  Debug code
 */
#ifdef NCG_DEBUG

#define NCG_DEBUG_MEMORY_ANALYZE

#ifdef NCG_DEBUG_MEMORY_ANALYZE

typedef struct T_NCG_DEBUG_HostMemHeader {
    NCGint32    i32Size;
    NCGuint32   ui32Flags;
    NCGuint32   ui32Reserved1;
    NCGuint32   ui32Reserved0;
} NCG_DEBUG_HostMemHeader;

/* Analyze parameters */
NCGint32        NCG_G_HostMem_Count         = 0;
NCGint32        NCG_G_HostMem_Peak_Count    = 0;
NCGint32        NCG_G_HostMem_Size          = 0;
NCGint32        NCG_G_HostMem_Peak_Size     = 0;

NCGint32        NCG_G_FrameMem_Count        = 0;
NCGint32        NCG_G_FrameMem_Peak_Count   = 0;
NCGint32        NCG_G_FrameMem_Size         = 0;
NCGint32        NCG_G_FrameMem_Peak_Size    = 0;

#endif /* #ifdef NCG_DEBUG_MEMORY_ANALYZE */

#endif /* #ifdef NCG_DEBUG */


/*=============================================================================
 *  Global Function
 */

/*----------------------------------------------------------------------------
 NAME       : NCGSYS_PurgeCPUOperandCache
 FUNCTION   : Purge the CPU operand cache.
 PARAMETERS : None.
 RETURN     : None.
------------------------------------------------------------------------------*/
NCGvoid 
NCGSYS_PurgeCPUOperandCache( void )
{
    NCG_DEBUG_PRINT_STRING( "[LOG] NCGSYS_PurgeCPUOperandCache" );

    /* Not suppoert. */

    return ;
}

/*----------------------------------------------------------------------------
 NAME       : NCGSYS_CPUVAddrToSysPAddr
 FUNCTION   : Get the system physical address of the given host memory block.
 PARAMETERS : ui32CPUAddr  : [IN ] The address of the host memory area.
 RETURN     : Phy.
------------------------------------------------------------------------------*/
NCGuint32 
NCGSYS_CPUVAddrToSysPAddr (
    NCGuint32   ui32CPUAddr )
{
    NCGuint32   ui32Ret_addr;

    ui32Ret_addr = ui32CPUAddr;

    return ui32Ret_addr;
}


/*----------------------------------------------------------------------------
 NAME       : NCGSYS_HostMemCreate
 FUNCTION   : Start managing the host memory area.
 PARAMETERS : ui32Size  : [IN ] The size of the host memory area.
              ui32Flags : [IN ] flags(fixed to 0)
 RETURN     : Error code of the NCG.
------------------------------------------------------------------------------*/
NCGint32
NCGSYS_HostMemCreate (
    NCGint32        i32Size,
    NCGuint32       ui32Flags)
{
    NCGint32 rc_val = NCG_no_err;

    NCG_UNREFERENCED_PARAMETER(i32Size);
    NCG_UNREFERENCED_PARAMETER(ui32Flags);

    NCG_DEBUG_PRINT_STRING( "[LOG] NCGSYS_HostMemCreate" );

#ifdef NCG_DEBUG_MEMORY_ANALYZE

    NCG_G_HostMem_Count         = 0;
    NCG_G_HostMem_Peak_Count    = 0;
    NCG_G_HostMem_Size          = 0;
    NCG_G_HostMem_Peak_Size     = 0;

#endif /* #ifdef NCG_DEBUG_MEMORY_ANALYZE */

    return rc_val;
}

/*----------------------------------------------------------------------------
 NAME       : NCGSYS_HostMemDestroy
 FUNCTION   : Finish managing the host memory area.
 PARAMETERS : None.
 RETURN     : Error code of the NCG.
------------------------------------------------------------------------------*/
NCGint32
NCGSYS_HostMemDestroy ( void )
{
    NCGint32 rc_val = NCG_no_err;

    /* do nothing */

#ifdef NCG_DEBUG_MEMORY_ANALYZE

#ifdef NCG_DEBUG_MEMORY_ANALYZE_LOG

    NCG_DEBUG_MAKE_MSG_START( NCG_G_MASSAGE_BUFF, "[NCG Host memory]%s" )
    NCG_DEBUG_MAKE_MSG_PARAMETER( NCG_CRLF )
    NCG_DEBUG_MAKE_MSG_END();
    NCG_DEBUG_PRINT_MSG( NCG_G_MASSAGE_BUFF );

    NCG_DEBUG_MAKE_MSG_START( NCG_G_MASSAGE_BUFF, "  Count      : %d%s" )
    NCG_DEBUG_MAKE_MSG_PARAMETER( NCG_G_HostMem_Count )
    NCG_DEBUG_MAKE_MSG_PARAMETER( NCG_CRLF )
    NCG_DEBUG_MAKE_MSG_END();
    NCG_DEBUG_PRINT_MSG( NCG_G_MASSAGE_BUFF );

    NCG_DEBUG_MAKE_MSG_START( NCG_G_MASSAGE_BUFF, "  Peak Count : %d%s" )
    NCG_DEBUG_MAKE_MSG_PARAMETER( NCG_G_HostMem_Peak_Count )
    NCG_DEBUG_MAKE_MSG_PARAMETER( NCG_CRLF )
    NCG_DEBUG_MAKE_MSG_END();
    NCG_DEBUG_PRINT_MSG( NCG_G_MASSAGE_BUFF );

    NCG_DEBUG_MAKE_MSG_START( NCG_G_MASSAGE_BUFF, "  Size       : %d [byte]%s" )
    NCG_DEBUG_MAKE_MSG_PARAMETER( NCG_G_HostMem_Size )
    NCG_DEBUG_MAKE_MSG_PARAMETER( NCG_CRLF )
    NCG_DEBUG_MAKE_MSG_END();
    NCG_DEBUG_PRINT_MSG( NCG_G_MASSAGE_BUFF );

    NCG_DEBUG_MAKE_MSG_START( NCG_G_MASSAGE_BUFF, "  Peak Size  : %d [byte]%s" )
    NCG_DEBUG_MAKE_MSG_PARAMETER( NCG_G_HostMem_Peak_Size )
    NCG_DEBUG_MAKE_MSG_PARAMETER( NCG_CRLF )
    NCG_DEBUG_MAKE_MSG_END();
    NCG_DEBUG_PRINT_MSG( NCG_G_MASSAGE_BUFF );

#endif /* #ifdef NCG_DEBUG_MEMORY_ANALYZE_LOG */

#endif /* #ifdef NCG_DEBUG_MEMORY_ANALYZE */

    return rc_val;
}

/*----------------------------------------------------------------------------
 NAME       : NCGSYS_HostMemAlloc
 FUNCTION   : Allocate a new memory block from the host memory.
 PARAMETERS : i32Size   : [IN ] The size of the memory to allocate[bytes].
              ui32Flags : [IN ] Allocation flags(fixed to 0).
              ppMemAddr : [OUT] The pointer to the allocated memory block.
 RETURN     : Error code of the NCG.
------------------------------------------------------------------------------*/
NCGint32
NCGSYS_HostMemAlloc (
    NCGint32        i32Size,
    NCGuint32       ui32Flags,
    NCGvoid         **ppMemAddr)
{
    NCGint32 rc_val = NCG_no_err;

    NCGvoid  *pvAlloc;

    NCG_UNREFERENCED_PARAMETER(ui32Flags);

    NCG_ASSERT( ppMemAddr != NCG_NULL );

#ifdef NCG_DEBUG_MEMORY_ANALYZE

    i32Size += sizeof(NCG_DEBUG_HostMemHeader);

#endif /* #ifdef NCG_DEBUG_MEMORY_ANALYZE */

    pvAlloc = (void *)malloc((size_t)i32Size);

    if (pvAlloc != NCG_NULL) {
#ifdef NCG_DEBUG_MEMORY_ANALYZE
        NCG_DEBUG_HostMemHeader     *ptHeader;
        NCGuint32                   ui32AddressWork;

        ptHeader = (NCG_DEBUG_HostMemHeader*)pvAlloc;
        ptHeader->i32Size   = i32Size - sizeof(NCG_DEBUG_HostMemHeader);
        ptHeader->ui32Flags = ui32Flags;

        NCG_G_HostMem_Count++;
        if ( NCG_G_HostMem_Peak_Count < NCG_G_HostMem_Count ) {
            NCG_G_HostMem_Peak_Count = NCG_G_HostMem_Count;
        }
        NCG_G_HostMem_Size += ptHeader->i32Size;
        if ( NCG_G_HostMem_Peak_Size < NCG_G_HostMem_Size ) {
            NCG_G_HostMem_Peak_Size = NCG_G_HostMem_Size;
        }

        ui32AddressWork = (unsigned long)ptHeader;
        pvAlloc = (void*)(ui32AddressWork + sizeof(NCG_DEBUG_HostMemHeader));

#endif /* #ifdef NCG_DEBUG_MEMORY_ANALYZE */

        *ppMemAddr = pvAlloc;
    }
    else {
        *ppMemAddr = NCG_NULL;
        rc_val = NCG_err_no_host_mem;
        NCG_DEBUG_MAKE_MSG_START( NCG_G_MASSAGE_BUFF, "[NCG][ERROR]NCGSYS_HostMemFree(2) Failed:malloc.%s" )
        NCG_DEBUG_MAKE_MSG_PARAMETER( NCG_CRLF )
        NCG_DEBUG_MAKE_MSG_END();
        NCG_DEBUG_PRINT_MSG( NCG_G_MASSAGE_BUFF );
    }

    return rc_val;
}

/*----------------------------------------------------------------------------
 NAME       : NCGSYS_HostMemFree
 FUNCTION   : Free a memory block allocated from the host memory.
 PARAMETERS : pMemAddr : [IN ] The pointer to the memory block.
 RETURN     : Error code of the NCG.
------------------------------------------------------------------------------*/
NCGint32
NCGSYS_HostMemFree (
    NCGvoid         *pMemAddr)
{
    NCGint32 rc_val = NCG_no_err;

    NCG_ASSERT( pMemAddr != NCG_NULL );

#ifdef NCG_DEBUG_MEMORY_ANALYZE
{
    NCG_DEBUG_HostMemHeader     *ptHeader;
    NCGuint32                   ui32AddressWork;

    ui32AddressWork  = (NCGuint32)pMemAddr;
    ui32AddressWork -= sizeof(NCG_DEBUG_HostMemHeader);

    ptHeader = (NCG_DEBUG_HostMemHeader*)ui32AddressWork;

    NCG_G_HostMem_Count--;
    NCG_G_HostMem_Size -= ptHeader->i32Size;

    pMemAddr = (NCGvoid*)ui32AddressWork;
}
#endif /* #ifdef NCG_DEBUG_MEMORY_ANALYZE */

    free(pMemAddr);

    return rc_val;
}


/*-----------------------------------------------------------------------------
 NAME       : NCGSYS_HostMemSet
 FUNCTION   : Fill the given memory block with the specified value.
 PARAMETERS : pMemAddr : [IN ] The pointer to the memory block.
              ui8c     : [IN ] Fill value.
              i32Count : [IN ] Fill count by byte.
 RETURN     : None.
-----------------------------------------------------------------------------*/
NCGvoid
NCGSYS_HostMemSet (
    NCGvoid         *pMemAddr,
    NCGuint8        ui8c,
    NCGint32        i32Count)
{
    NCGint32 i;
    NCGuint8 *pCur = (NCGuint8 *)pMemAddr;

    if (pMemAddr == NCG_NULL) {
        /* Do nothing, though it's an error case. */
    }
    else {
        for (i=0; i<i32Count; i++) {
            *(pCur++) = ui8c;
        }
    }

    return;
}


/*-----------------------------------------------------------------------------
 NAME       : NCGSYS_HostMemCopy
 FUNCTION   : Copy a memory block into another memory block.
 PARAMETERS : pDst     :[IN ] The pointer to the destination memory block.
              pSrc     :[IN ] The pointer to the source memory block.
              i32Count :[IN ] Specify the number of bytes to copy.
 RETURN     : None.
-----------------------------------------------------------------------------*/
NCGvoid
NCGSYS_HostMemCopy (
    NCGvoid         *pDst,
    NCGvoid         *pSrc,
    NCGint32        i32Count)
{
    NCGint32 i;
    NCGuint8 *pTo   = pDst;
    NCGuint8 *pFrom = pSrc;

    if ( (pDst == NCG_NULL) || (pSrc == NCG_NULL) ) {
        /* Do nothing, though it's an error case. */
    }
    else {
        for (i=0; i<i32Count; i++) {
            *(pTo++) = *(pFrom++);
        }
    }

    return;
}

/*----------------------------------------------------------------------------
 NAME       : NCGSYS_FrameMemCreate
 FUNCTION   : Initialize frame memory area.
 PARAMETERS : ui32Base  : [IN ] The base address of the frame memory area.
              i32Size   : [IN ] The size of the frame memory area.
              ui32Flags : [IN ] flags(fixed to 0)
 RETURN     : Error code of the NCG.
------------------------------------------------------------------------------*/
NCGint32
NCGSYS_FrameMemCreate (
    NCGuint32       ui32Base,
    NCGint32        i32Size,
    NCGuint32       ui32Flags)
{
    NCG_MemBlock_t * pBlk;

    NCG_UNREFERENCED_PARAMETER(ui32Flags);

    if (g_frameMemSize > 0) {
        NCG_DEBUG_MAKE_MSG_START( NCG_G_MASSAGE_BUFF, "[NCG][ERROR]NCGSYS_FrameMemCreate(1) Failed:Already initialized.%s" )
        NCG_DEBUG_MAKE_MSG_PARAMETER( NCG_CRLF )
        NCG_DEBUG_MAKE_MSG_END();
        NCG_DEBUG_PRINT_MSG( NCG_G_MASSAGE_BUFF );
        return NCG_err_already_created;
    }

    if (!(i32Size > 0)) {
        NCG_DEBUG_MAKE_MSG_START( NCG_G_MASSAGE_BUFF, "[NCG][ERROR]NCGSYS_FrameMemCreate(2) Failed:i32Size (0x%08x)%s" )
        NCG_DEBUG_MAKE_MSG_PARAMETER( i32Size )
        NCG_DEBUG_MAKE_MSG_PARAMETER( NCG_CRLF )
        NCG_DEBUG_MAKE_MSG_END();
        NCG_DEBUG_PRINT_MSG( NCG_G_MASSAGE_BUFF );
        return NCG_err_bad_parameter;
    }

    g_frameMemStart = ui32Base;
    g_frameMemSize  = i32Size;

    pBlk = CreateMemBlock(g_frameMemStart, (NCGint32)g_frameMemSize);
    if (pBlk == NCG_NULL) {
        NCG_DEBUG_MAKE_MSG_START( NCG_G_MASSAGE_BUFF, "[NCG][ERROR]NCGSYS_FrameMemCreate(3) Failed:CreateMemBlock.%s" )
        NCG_DEBUG_MAKE_MSG_PARAMETER( NCG_CRLF )
        NCG_DEBUG_MAKE_MSG_END();
        NCG_DEBUG_PRINT_MSG( NCG_G_MASSAGE_BUFF );
        return NCG_err_no_host_mem;
    }

    g_pFreeBlocks       = pBlk;
    g_pAllocatedBlocks  = NCG_NULL;

#ifdef NCG_DEBUG_MEMORY_ANALYZE

    NCG_G_FrameMem_Count        = 0;
    NCG_G_FrameMem_Peak_Count   = 0;
    NCG_G_FrameMem_Size         = 0;
    NCG_G_FrameMem_Peak_Size    = 0;

#endif /* #ifdef NCG_DEBUG_MEMORY_ANALYZE */

    return NCG_no_err;
}

/*----------------------------------------------------------------------------
 NAME       : NCGSYS_FrameMemDestroy
 FUNCTION   : Finalize frame memory area.
 PARAMETERS : None.
 RETURN     : Error code of the NCG.
------------------------------------------------------------------------------*/
NCGint32
NCGSYS_FrameMemDestroy ( void )
{
    NCG_MemBlock_t * pCur = g_pAllocatedBlocks;

    while (pCur != NCG_NULL) {
        NCG_MemBlock_t * pNext = pCur->pNext;
        NCGSYS_HostMemFree(pCur->pFInfo);
        NCGSYS_HostMemFree(pCur);
        pCur = pNext;
    }

    pCur = g_pFreeBlocks;
    while (pCur != NCG_NULL) {
        NCG_MemBlock_t * pNext = pCur->pNext;
        NCGSYS_HostMemFree(pCur);
        pCur = pNext;
    }

    g_pAllocatedBlocks = NCG_NULL;
    g_pFreeBlocks      = NCG_NULL;
    g_frameMemStart    = 0xffffffffU;
    g_frameMemSize     = 0;

#ifdef NCG_DEBUG_MEMORY_ANALYZE

#ifdef NCG_DEBUG_MEMORY_ANALYZE_LOG

    NCG_DEBUG_MAKE_MSG_START( NCG_G_MASSAGE_BUFF, "[NCG Frame memory]%s" )
    NCG_DEBUG_MAKE_MSG_PARAMETER( NCG_CRLF )
    NCG_DEBUG_MAKE_MSG_END();
    NCG_DEBUG_PRINT_MSG( NCG_G_MASSAGE_BUFF );

    NCG_DEBUG_MAKE_MSG_START( NCG_G_MASSAGE_BUFF, "  Count      : %d %s" )
    NCG_DEBUG_MAKE_MSG_PARAMETER( NCG_G_FrameMem_Count )
    NCG_DEBUG_MAKE_MSG_PARAMETER( NCG_CRLF )
    NCG_DEBUG_MAKE_MSG_END();
    NCG_DEBUG_PRINT_MSG( NCG_G_MASSAGE_BUFF );

    NCG_DEBUG_MAKE_MSG_START( NCG_G_MASSAGE_BUFF, "  Peak Count : %d %s" )
    NCG_DEBUG_MAKE_MSG_PARAMETER( NCG_G_FrameMem_Peak_Count )
    NCG_DEBUG_MAKE_MSG_PARAMETER( NCG_CRLF )
    NCG_DEBUG_MAKE_MSG_END();
    NCG_DEBUG_PRINT_MSG( NCG_G_MASSAGE_BUFF );

    NCG_DEBUG_MAKE_MSG_START( NCG_G_MASSAGE_BUFF, "  Size       : %d [byte]%s" )
    NCG_DEBUG_MAKE_MSG_PARAMETER( NCG_G_FrameMem_Size )
    NCG_DEBUG_MAKE_MSG_PARAMETER( NCG_CRLF )
    NCG_DEBUG_MAKE_MSG_END();
    NCG_DEBUG_PRINT_MSG( NCG_G_MASSAGE_BUFF );

    NCG_DEBUG_MAKE_MSG_START( NCG_G_MASSAGE_BUFF, "  Peak Size  : %d [byte]%s" )
    NCG_DEBUG_MAKE_MSG_PARAMETER( NCG_G_FrameMem_Peak_Size )
    NCG_DEBUG_MAKE_MSG_PARAMETER( NCG_CRLF )
    NCG_DEBUG_MAKE_MSG_END();
    NCG_DEBUG_PRINT_MSG( NCG_G_MASSAGE_BUFF );

#endif /* #ifdef NCG_DEBUG_MEMORY_ANALYZE_LOG */

#endif /* #ifdef NCG_DEBUG_MEMORY_ANALYZE */

    return NCG_no_err;
}

/*----------------------------------------------------------------------------
 NAME       : NCGSYS_FrameMemAlloc
 FUNCTION   : Allocate a new memory block from the frame memory.
 PARAMETERS : i32Size   : [IN ] The size of the memory to allocate[bytes].
              i32Align  : [IN ] Address alignment of the memory block.
              ui32Flags : [IN ] The frag.
              ppMem     : [OUT] The pointer to the allocated memory block.
 RETURN     : Error code of the NCG.
------------------------------------------------------------------------------*/
NCGint32
NCGSYS_FrameMemAlloc (
    NCGint32        i32Size,
    NCGint32        i32Align,
    NCGuint32       ui32Flags,
    NCGvoid         **ppMem)
{
    NCGint32            rc_val = NCG_no_err;
    NCG_FrameMemInfo_t  *pObj;
    NCG_MemBlock_t      *pCur = g_pFreeBlocks;
    NCG_MemBlock_t      *pAlloc = NCG_NULL;
    NCGuint32           ui32AlignMask = 0xffffffffU;

    NCG_UNREFERENCED_PARAMETER(ui32Flags);

    NCG_ASSERT( ppMem != NCG_NULL );
    NCG_ASSERT( i32Size > 0 );

    switch (i32Align) {
        case 0x0001: case 0x0002: case 0x0004: case 0x0008:
        case 0x0010: case 0x0020: case 0x0040: case 0x0080:
        case 0x0100: case 0x0200: case 0x0400: case 0x0800:
        case 0x1000: case 0x2000: case 0x4000: case 0x8000:
            ui32AlignMask = i32Align - 1;
            break;

        /* case 0x0000: */
        default:
            NCG_DEBUG_MAKE_MSG_START( NCG_G_MASSAGE_BUFF, "[NCG][ERROR]NCGSYS_FrameMemAlloc(3) Failed:i32Align (0x%08x)%s" )
            NCG_DEBUG_MAKE_MSG_PARAMETER( i32Align )
            NCG_DEBUG_MAKE_MSG_PARAMETER( NCG_CRLF )
            NCG_DEBUG_MAKE_MSG_END();
            NCG_DEBUG_PRINT_MSG( NCG_G_MASSAGE_BUFF );
            return NCG_err_bad_parameter;
    }

    /* Search and Compact memory blocks */
    while (pCur != NCG_NULL) {
        NCG_MemBlock_t * pNext = pCur->pNext;

        if (pNext != NCG_NULL &&
            (pCur->ui32StartAddr + pCur->i32Size) == pNext->ui32StartAddr) {
            /* Unite contiguous blocks */
            pCur->i32Size += pNext->i32Size;
            pCur->pNext = pNext->pNext;
            NCGSYS_HostMemFree(pNext);
        }
        else if (pNext != NCG_NULL && (pNext->i32Size == 0)) {
            /* Remove a zero size block */
            pCur->pNext = pNext->pNext;
            NCGSYS_HostMemFree(pNext);
        }
        else {
            if (pCur->i32Size >= i32Size) {
                /* Allocate a new block from the bottom of the current block */
                NCGuint32 ui32StartAddr   = pCur->ui32StartAddr + pCur->i32Size - i32Size;
                NCGuint32 ui32AlignedAddr = ui32StartAddr & (~ui32AlignMask);

                if (ui32AlignedAddr >= pCur->ui32StartAddr) {
                    rc_val = NCGSYS_HostMemAlloc(sizeof(NCG_MemBlock_t), 0, (NCGvoid **)&pAlloc);
                    if ( rc_val != NCG_no_err ) {
                        NCG_DEBUG_MAKE_MSG_START( NCG_G_MASSAGE_BUFF, "[NCG][ERROR]NCGSYS_FrameMemAlloc(4) Failed:NCGSYS_HostMemAlloc (0x%08x)%s" )
                        NCG_DEBUG_MAKE_MSG_PARAMETER( rc_val )
                        NCG_DEBUG_MAKE_MSG_PARAMETER( NCG_CRLF )
                        NCG_DEBUG_MAKE_MSG_END();
                        NCG_DEBUG_PRINT_MSG( NCG_G_MASSAGE_BUFF );
                        return rc_val;
                    }

                    pAlloc->ui32StartAddr = ui32AlignedAddr;
                    pAlloc->i32Size       = pCur->ui32StartAddr + pCur->i32Size - ui32AlignedAddr;
                    pAlloc->pNext = NCG_NULL;
                    break;
                }
            }
            pCur = pCur->pNext;
        }
    }

    if ( pAlloc == NCG_NULL ) {
        NCG_DEBUG_MAKE_MSG_START( NCG_G_MASSAGE_BUFF, "[NCG][ERROR]NCGSYS_FrameMemAlloc(5) Failed:Not enough frame memory.%s" )
        NCG_DEBUG_MAKE_MSG_PARAMETER( NCG_CRLF )
        NCG_DEBUG_MAKE_MSG_END();
        NCG_DEBUG_PRINT_MSG( NCG_G_MASSAGE_BUFF );
        return NCG_err_no_frame_mem;
    }

    rc_val = NCGSYS_HostMemAlloc(sizeof(NCG_FrameMemInfo_t), 0, (NCGvoid **)&pObj);
    if ( rc_val != NCG_no_err ) {
        NCGSYS_HostMemFree(pAlloc);
        NCG_DEBUG_MAKE_MSG_START( NCG_G_MASSAGE_BUFF, "[NCG][ERROR]NCGSYS_FrameMemAlloc(6) Failed:NCGSYS_HostMemAlloc (0x%08x)%s" )
        NCG_DEBUG_MAKE_MSG_PARAMETER( rc_val )
        NCG_DEBUG_MAKE_MSG_PARAMETER( NCG_CRLF )
        NCG_DEBUG_MAKE_MSG_END();
        NCG_DEBUG_PRINT_MSG( NCG_G_MASSAGE_BUFF );
        return rc_val;
    }

    pCur->i32Size = pCur->i32Size - pAlloc->i32Size;
    pAlloc->pNext = g_pAllocatedBlocks;
    g_pAllocatedBlocks = pAlloc;

    pObj->ui32PhysAddr = NCGSYS_CPUVAddrToSysPAddr( pAlloc->ui32StartAddr );
    pObj->i32Size      = pAlloc->i32Size;
    pObj->pvCPUVAddr   = (NCGvoid*)(pAlloc->ui32StartAddr);
    pObj->ui32DevVAddr = pAlloc->ui32StartAddr;
    pObj->ui32FBAddr   = pAlloc->ui32StartAddr;

    pAlloc->pFInfo = pObj;

    *ppMem = pObj;

#ifdef NCG_DEBUG_MEMORY_ANALYZE

    NCG_G_FrameMem_Count++;
    if ( NCG_G_FrameMem_Peak_Count < NCG_G_FrameMem_Count ) {
        NCG_G_FrameMem_Peak_Count = NCG_G_FrameMem_Count;
    }
    NCG_G_FrameMem_Size += pObj->i32Size;
    if ( NCG_G_FrameMem_Peak_Size < NCG_G_FrameMem_Size ) {
        NCG_G_FrameMem_Peak_Size = NCG_G_FrameMem_Size;
    }

#endif /* #ifdef NCG_DEBUG_MEMORY_ANALYZE */

    return rc_val;
}

/*----------------------------------------------------------------------------
 NAME       : NCGSYS_FrameMemFree
 FUNCTION   : Free a frame memory block.
 PARAMETERS : pMem : [IN ] The pointer to the memory block.
 RETURN     : Error code of the NCG.
------------------------------------------------------------------------------*/
NCGint32
NCGSYS_FrameMemFree (
    NCGvoid     *pMem)
{
    NCGint32            rc_val = NCG_no_err;
    NCG_FrameMemInfo_t  *pObj = (NCG_FrameMemInfo_t *)pMem;
    NCG_MemBlock_t      *pCur = g_pAllocatedBlocks;
    NCG_MemBlock_t      *pAlloc = NCG_NULL;
    NCGuint32           ui32StartAddr;

    NCG_ASSERT( pMem != NCG_NULL );

    ui32StartAddr = pObj->ui32DevVAddr;

    /* Search allocated block info */
    if ( (pCur != NCG_NULL) && (pCur->ui32StartAddr == ui32StartAddr) ) {
        g_pAllocatedBlocks = pCur->pNext;
        pAlloc = pCur;
    }
    else {
        while (pCur != NCG_NULL) {
            NCG_MemBlock_t * pNext = pCur->pNext;

            if (pNext != NCG_NULL && pNext->ui32StartAddr == ui32StartAddr) {
                pAlloc = pNext;
                pCur->pNext = pNext->pNext;
                break;
            }

            pCur = pCur->pNext;
        }
    }

    if (pAlloc == NCG_NULL) {
        /* Address not found... */
        NCG_DEBUG_MAKE_MSG_START( NCG_G_MASSAGE_BUFF, "[NCG][ERROR]NCGSYS_FrameMemFree(2) Failed:address is not found.%s" )
        NCG_DEBUG_MAKE_MSG_PARAMETER( NCG_CRLF )
        NCG_DEBUG_MAKE_MSG_END();
        NCG_DEBUG_PRINT_MSG( NCG_G_MASSAGE_BUFF );
        return NCG_err_bad_parameter;
    }

    /* Insert the block into the freelist. */
    ui32StartAddr = pAlloc->ui32StartAddr;
    pCur = g_pFreeBlocks;
    if ( (pCur == NCG_NULL) || (ui32StartAddr <= pCur->ui32StartAddr) ) {
        pAlloc->pNext = g_pFreeBlocks;
        g_pFreeBlocks = pAlloc;
    }
    else {
        while (pCur != NCG_NULL) {
            if (((pCur->pNext != NCG_NULL) &&
                 (pCur->ui32StartAddr <= ui32StartAddr) &&
                 (ui32StartAddr <= pCur->pNext->ui32StartAddr))
                || (pCur->pNext == NCG_NULL)) {

                pAlloc->pNext = pCur->pNext;
                pCur->pNext = pAlloc;
                break;
            }

            pCur = pCur->pNext;
        }
    }

    /* Free FrameMemInfo object */
#ifdef NCG_DEBUG_MEMORY_ANALYZE

    NCG_G_FrameMem_Count--;
    NCG_G_FrameMem_Size -= pObj->i32Size;

#endif /* #ifdef NCG_DEBUG_MEMORY_ANALYZE */

    /* Free FrameMemInfo object */
    rc_val = NCGSYS_HostMemFree(pObj);
    if ( rc_val != NCG_no_err ) {
        NCG_DEBUG_MAKE_MSG_START( NCG_G_MASSAGE_BUFF, "[NCG][ERROR]NCGSYS_FrameMemFree(3) Failed:NCGSYS_HostMemFree (0x%08x)%s" )
        NCG_DEBUG_MAKE_MSG_PARAMETER( rc_val )
        NCG_DEBUG_MAKE_MSG_PARAMETER( NCG_CRLF )
        NCG_DEBUG_MAKE_MSG_END();
        NCG_DEBUG_PRINT_MSG( NCG_G_MASSAGE_BUFF );
    }

    pAlloc->pFInfo = NCG_NULL;

    return rc_val;
}

/*----------------------------------------------------------------------------
 NAME       : NCGSYS_FrameMemWrap
 FUNCTION   : Create a frame memory object wrapping the specified memory block.
 PARAMETERS : pBase     : [IN ] Start address of the memory to be wrapped.
              i32Size   : [IN ] The size of the memory block.
              i32Align  : [IN ] Address alignment of the memory block.
              ui32Flags : [IN ] Flags(fixed to 0).
              ppMem     : [OUT] The pointer to the memory object.
 RETURN     : Error code of the NCG.
------------------------------------------------------------------------------*/
NCGint32
NCGSYS_FrameMemWrap (
    NCGvoid     *pBase,
    NCGint32    i32Size,
    NCGint32    i32Align,
    NCGuint32   ui32Flags,
    NCGvoid     **ppMem)
{
    NCGint32 rc_val = NCG_no_err;
    /* NCG_FrameMemInfo_t * pObj; */

    NCG_UNREFERENCED_PARAMETER(pBase);
    NCG_UNREFERENCED_PARAMETER(i32Size);
    NCG_UNREFERENCED_PARAMETER(i32Align);
    NCG_UNREFERENCED_PARAMETER(ui32Flags);
    NCG_UNREFERENCED_PARAMETER(ppMem);

    rc_val = NCG_err_not_implemented;

    return rc_val;
}

/*----------------------------------------------------------------------------
 NAME       : NCGSYS_FrameMemToCPUVAddr
 FUNCTION   : Get the CPU virtual address of the given frame memory block.
 PARAMETERS : pMem : [IN ] The pointer to the frame memory block.
 RETURN     : CPU virtual address.
------------------------------------------------------------------------------*/
NCGvoid *
NCGSYS_FrameMemToCPUVAddr (
    NCGvoid     *pMem)
{
    NCG_FrameMemInfo_t * pObj = (NCG_FrameMemInfo_t *)pMem;

    NCG_ASSERT( pObj != NCG_NULL );

    return pObj->pvCPUVAddr;
}

/*----------------------------------------------------------------------------
 NAME       : NCGSYS_FrameMemToDevVAddr
 FUNCTION   : Get the Device virtual address of the given frame memory block.
 PARAMETERS : pMem : [IN ] The pointer to the frame memory block.
 RETURN     : Device virtual address.
------------------------------------------------------------------------------*/
NCGuint32
NCGSYS_FrameMemToDevVAddr (
    NCGvoid     *pMem)
{
    NCG_FrameMemInfo_t *pObj = (NCG_FrameMemInfo_t *)pMem;

    NCG_ASSERT( pObj != NCG_NULL );

    return pObj->ui32DevVAddr;
}

/*----------------------------------------------------------------------------
 NAME       : NCGSYS_FrameMemToSysPAddr
 FUNCTION   : Get the system physical address of the given frame memory block.
 PARAMETERS : pMem : [IN ] The pointer to the frame memory block.
 RETURN     : System physical address.
------------------------------------------------------------------------------*/
NCGuint32
NCGSYS_FrameMemToSysPAddr (
    NCGvoid     *pMem)
{
    NCG_FrameMemInfo_t *pObj = (NCG_FrameMemInfo_t *)pMem;

    NCG_ASSERT( pObj != NCG_NULL );

    return (NCGuint32)pObj->ui32PhysAddr;
}

/*----------------------------------------------------------------------------
 NAME       : NCGSYS_FrameMemToFbAddr
 FUNCTION   : Get the framebuffer address of the given frame memory block.
 PARAMETERS : pMem : [IN ] The pointer to the frame memory block.
 RETURN     : Framebuffer address.
------------------------------------------------------------------------------*/
NCGuint32
NCGSYS_FrameMemToFbAddr (
    NCGvoid     *pMem)
{
    NCG_FrameMemInfo_t *pObj = (NCG_FrameMemInfo_t *)pMem;

    NCG_ASSERT( pObj != NCG_NULL );

    return pObj->ui32FBAddr;
}


/******************************************************************************
 * Internal functions
 */

/*----------------------------------------------------------------------------
 NAME       : CreateMemBlock
 FUNCTION   : Create frame memory block.
 PARAMETERS : ui32StartAddr : [IN ] Top address of the frame memory block.
              i32Size       : [IN ] Total size of the frame memory block.
 RETURN     : Pointer to the frame memory block.
------------------------------------------------------------------------------*/
static NCG_MemBlock_t *
CreateMemBlock(
    NCGuint32       ui32StartAddr,
    NCGint32        i32Size)
{
    NCGint32 ret;
    NCG_MemBlock_t * pBlk;

    ret = NCGSYS_HostMemAlloc(sizeof(NCG_MemBlock_t), 0, (NCGvoid **)&pBlk);
    if ( ret != NCG_no_err ) {
        NCG_DEBUG_MAKE_MSG_START( NCG_G_MASSAGE_BUFF, "[NCG][ERROR]CreateMemBlock Failed.%s" )
        NCG_DEBUG_MAKE_MSG_PARAMETER( NCG_CRLF )
        NCG_DEBUG_MAKE_MSG_END();
        NCG_DEBUG_PRINT_MSG( NCG_G_MASSAGE_BUFF );
        return NCG_NULL;
    }

    pBlk->ui32StartAddr = ui32StartAddr;
    pBlk->i32Size       = i32Size;
    pBlk->pNext         = NCG_NULL;

    return pBlk;
}

/* -- end of file -- */
