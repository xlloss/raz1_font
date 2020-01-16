/******************************************************************************
* Copyright(c) 2010-2013 Renesas Electronics Corporation. All rights reserved.
*
* author: Renesas Electronics Corporation.
*
* history: 2010.10.08
*          - Added Renesas platform header.
*          2010.10.22
*          - Modified prototye of vgQueryResourceRE function
*          2011.01.20
*          - Added VG_QUERY_RESOURCE_WORK_SIZE macro.
*          - Modified the enumeration type of VGResAttributeRE.
*          2011.03.09
*          - Removed VG_QUERY_RESOURCE_WORK_SIZE macro.
*          - Added VG_QUERY_RESOURCE_WB_STRIDE macro.
*          2011.08.29
*          - Added VGParamTypeREL.
*          2011.09.30
*          - Corresponded to MISRA-C Rule 10.6
*          2013.04.08
*          - Added VG_ERROR_CODE_REL_FORCE_SIZE to enumeration type of
*            VGErrorCodeREL.
*          - Added VG_PARAM_TYPE_REL_FORCE_SIZE to enumeration type of
*            VGParamTypeREL.
*
*******************************************************************************/
/******************************************************************************
 *
 * Copyright (c) 2009-2010 System Management Incorporated
 *
 * This software is protected by the law and the agreement concerning
 * a Japanese country copyright method, an international agreement,
 * and other intellectual property right and may be used and copied
 * only in accordance with the terms of such license and with the inclusion
 * of the above copyright notice.
 *
 * This software or any other copies thereof may not be provided
 * or otherwise made available to any other person. No title to and
 * ownership of the software is hereby transferred.
 *
 *******************************************************************************
 * file: vgext.h
 *
 * Declaration of OpenVG Extension API
 * component: VG
 *
 * author: Sosuke Tokunaga
 *         / System Management Incorporated
 *
 * history: 14 May 2010 by Sosuke Tokunaga
 *          - Modified resource query extension API
 *
 *          07 Oct 2010 by Sosuke Tokunaga
 *          - Updated last-modified-year of copyright notice
 *
 *****************************************************************************/

/* $Revision: 2444 $ on $Date:: 2014-10-14 21:15:39 +0100 #$ */

/*------------------------------------------------------------------------
 *
 * VG platform specific header Reference Implementation
 * ----------------------------------------------------
 *
 * Copyright (c) 2008 The Khronos Group Inc.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and /or associated documentation files
 * (the "Materials "), to deal in the Materials without restriction,
 * including without limitation the rights to use, copy, modify, merge,
 * publish, distribute, sublicense, and/or sell copies of the Materials,
 * and to permit persons to whom the Materials are furnished to do so,
 * subject to the following conditions: 
 *
 * The above copyright notice and this permission notice shall be included 
 * in all copies or substantial portions of the Materials. 
 *
 * THE MATERIALS ARE PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
 * OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE MATERIALS OR
 * THE USE OR OTHER DEALINGS IN THE MATERIALS.
 *
 *//**
 * \file
 * \brief VG platform specific header
 *//*-------------------------------------------------------------------*/

#ifndef _VGPLATFORM_H
#define _VGPLATFORM_H

#ifdef __cplusplus
extern "C" {
#endif

#ifndef VG_API_CALL 
#if defined(OPENVG_STATIC_LIBRARY)
#	define VG_API_CALL
#else
#	if defined(_WIN32) || defined(__VC32__)				/* Win32 */
#		if defined (OPENVG_DLL_EXPORTS)
#			define VG_API_CALL __declspec(dllexport)
#		else
#			define VG_API_CALL __declspec(dllimport)
#		endif
#	else 
#		define VG_API_CALL extern
#	endif /* defined(_WIN32) ||... */
#endif /* defined OPENVG_STATIC_LIBRARY */
#endif /* ifndef VG_API_CALL */

#ifndef VGU_API_CALL 
#if defined(OPENVG_STATIC_LIBRARY)
#	define VGU_API_CALL
#else
#	if defined(_WIN32) || defined(__VC32__)				/* Win32 */
#		if defined (OPENVG_DLL_EXPORTS)
#			define VGU_API_CALL __declspec(dllexport)
#		else
#			define VGU_API_CALL __declspec(dllimport)
#		endif
#	else 
#		define VGU_API_CALL extern
#	endif /* defined(_WIN32) ||... */
#endif /* defined OPENVG_STATIC_LIBRARY */
#endif /* ifndef VGU_API_CALL */


#ifndef VG_API_ENTRY
#define VG_API_ENTRY
#endif

#ifndef VG_API_EXIT
#define VG_API_EXIT
#endif

#ifndef VGU_API_ENTRY
#define VGU_API_ENTRY
#endif

#ifndef VGU_API_EXIT
#define VGU_API_EXIT
#endif

typedef float          VGfloat;
typedef signed char    VGbyte;
typedef unsigned char  VGubyte;
typedef signed short   VGshort;
typedef signed int     VGint;
typedef unsigned int   VGuint;
typedef unsigned int   VGbitfield;

#ifndef VG_VGEXT_PROTOTYPES
#define VG_VGEXT_PROTOTYPES
#endif 

/* Renesas Platform */
#ifndef VGAPI
#define VGAPI   VG_API_CALL
#endif /* VGAPI */

#ifndef VGUAPI
#define VGUAPI  VGU_API_CALL
#endif /* VGUAPI */

#ifndef VGAPIENTRY
#define VGAPIENTRY  VG_API_ENTRY
#endif /* VGAPIENTRY */

#ifndef VGUAPIENTRY
#define VGUAPIENTRY VGU_API_ENTRY
#endif /* VGUAPIENTRY */


typedef enum {
    VG_REL_NO_ERROR               = 0,
    VG_REL_VG_INIT_RESOURCE_ERROR = 1,
    VG_REL_GPU_INIT_ERROR         = 2,
    VG_REL_RESOURCE_QUERY_ERROR   = 4,

    VG_ERROR_CODE_REL_FORCE_SIZE  = 0x7FFFFFFF
} VGErrorCodeREL;

/*-----------------------------------------------------------------------------
 NAME       : vgStartUp
 FUNCTION   : Start up the OpenVG module
 PARAMETERS : None
 RETURN     : Success : VG_REL_NO_ERROR
              Failure : Not VG_REL_NO_ERROR
-----------------------------------------------------------------------------*/
VGAPI VGErrorCodeREL VGAPIENTRY vgStartUp (void);

/*-----------------------------------------------------------------------------
 NAME       : vgCleanUp
 FUNCTION   : Clean up the OpenVG module
 PARAMETERS : None
 RETURN     : None
-----------------------------------------------------------------------------*/
VGAPI void      VGAPIENTRY vgCleanUp (void);

/* Renesas extension function */
#ifndef VG_API_ENTRYP
#   define VG_API_ENTRYP VG_API_ENTRY*
#endif

#ifndef VG_RE_query_resource
#define VG_RE_query_resource 1
typedef enum {
    VG_RES_OBJECT_RE            = 0x3800,
    VG_RES_BUFFER_RE            = 0x3801,
    VG_RES_GPU_RE               = 0x3802,

    VG_RES_TARGET_FORCE_SIZE    = 0x7FFFFFFF
} VGResTargetRE;

typedef enum {
    VG_RES_CURRENT_SIZE_RE      = 0x3810,
    VG_RES_PEAK_SIZE_RE         = 0x3811,
    VG_RES_SBO0_PEAK_SIZE_RE    = 0x3812,
    VG_RES_SBO1_PEAK_SIZE_RE    = 0x3813,
    VG_RES_DL_PEAK_SIZE_RE      = 0x3814,
    VG_RES_EDGE_COUNT_PEAK_RE   = 0x3815,
    VG_RES_EDGE_COUNT_CURRENT_RE= 0x3816,

    VG_RES_ATTRIBUTE_FORCE_SIZE = 0x7FFFFFFF
} VGResAttributeRE;

/*-----------------------------------------------------------------------------
 NAME       : vgQueryResourceRE
 FUNCTION   : Query resource information
 PARAMETERS : target    : [IN ] Resource target
              attribute : [IN ] Attribute to query
 RETURN     : The queried information
-----------------------------------------------------------------------------*/
#ifdef VG_VGEXT_PROTOTYPES
VGAPI VGint VGAPIENTRY vgQueryResourceRE (VGResTargetRE target, VGResAttributeRE attribute);
#endif
typedef VGint (VG_API_ENTRYP PFNVGQUERYRESOURCEREPROC) (VGResTargetRE target, VGResAttributeRE attribute);
#endif /* VG_RE_query_resource */

/******************************************************************************
 * Macros
 */
/*-----------------------------------------------------------------------------
 NAME       : VG_QUERY_RESOURCE_WB_STRIDE
 FUNCTION   : Macro to calculate stride of work buffer
 PARAMETERS : count    : [IN ] Number of edge
 RETURN     : Work Buffer stride necessary to store the edge information.
-----------------------------------------------------------------------------*/
#define VG_QUERY_RESOURCE_WB_STRIDE(edge)   (((((edge) << 1U) + 8U) + 63U) & ~63U)

/*******************************************************************************/
#ifndef RGPVG_API_CALL
#define RGPVG_API_CALL extern
#endif

#ifndef RGPVG_API_ENTRY
#define RGPVG_API_ENTRY
#endif

#ifndef RGPVG_API_EXIT
#define RGPVG_API_EXIT
#endif

typedef float          RGPVGfloat;
typedef signed char    RGPVGbyte;
typedef unsigned char  RGPVGubyte;
typedef signed short   RGPVGshort;
typedef signed int     RGPVGint;
typedef unsigned int   RGPVGuint;
typedef unsigned int   RGPVGbitfield;
typedef unsigned int   RGPVGboolean;

#define RGPVG_FALSE     0
#define RGPVG_TRUE      1

#define RGPVG_NONE                      0x0000FFFFU  /* Attrib list terminator */

#define RGPVG_WORK_BUFFER_HEIGHT        0x00000001
#define RGPVG_WORK_BUFFER_STRIDE        0x00000002
#define RGPVG_STREAM_BUFFER_0_SIZE      0x00000010
#define RGPVG_STREAM_BUFFER_1_SIZE      0x00000020
#define RGPVG_DISPLAY_LIST_BUFFER_SIZE  0x00000100

/*-----------------------------------------------------------------------------
 NAME       : rgpvgSetInitBufferSize
 FUNCTION   : Set buffer size for R-GPVG.
 PARAMETERS : attrib_list : [IN ] Attribute list
 RETURN     : Success : RGPVG_TRUE
              Failure : RGPVG_FALSE
-----------------------------------------------------------------------------*/
RGPVG_API_CALL RGPVGboolean RGPVG_API_ENTRY rgpvgSetInitBufferSize( const RGPVGuint *attrib_list ) RGPVG_API_EXIT;

/*******************************************************************************/
#ifndef VG_REL_dither
#define VG_REL_dither 1
typedef enum {
    VG_DITHER_REL                = 0x11A0,

    VG_PARAM_TYPE_REL_FORCE_SIZE = 0x7FFFFFFF
} VGParamTypeREL;
#endif

#ifdef __cplusplus 
} /* extern "C" */
#endif

#endif /* _VGPLATFORM_H */
