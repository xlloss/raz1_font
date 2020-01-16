#ifndef __eglplatform_h_
#define __eglplatform_h_

/******************************************************************************
* Copyright(c) 2010-2013 Renesas Electronics Corporation. All rights reserved.
*
* author: Renesas Electronics Corporation
*
* history: 2010.10.20
*          - Added Renesas platform header.
*          2013.01.29
*          - Modified Renesas platform definition.
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
 ******************************************************************************
 * file: eglext.h
 *
 * Declaration of EGL externsion API
 * component: EGL
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
/*
** Copyright (c) 2007-2009 The Khronos Group Inc.
**
** Permission is hereby granted, free of charge, to any person obtaining a
** copy of this software and/or associated documentation files (the
** "Materials"), to deal in the Materials without restriction, including
** without limitation the rights to use, copy, modify, merge, publish,
** distribute, sublicense, and/or sell copies of the Materials, and to
** permit persons to whom the Materials are furnished to do so, subject to
** the following conditions:
**
** The above copyright notice and this permission notice shall be included
** in all copies or substantial portions of the Materials.
**
** THE MATERIALS ARE PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
** EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
** MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
** IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
** CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
** TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
** MATERIALS OR THE USE OR OTHER DEALINGS IN THE MATERIALS.
*/

/* Platform-specific types and definitions for egl.h
 * $Revision: 2444 $ on $Date: 2014-10-14 21:15:39 +0100 (Tue, 14 Oct 2014) $
 *
 * Adopters may modify khrplatform.h and this file to suit their platform.
 * You are encouraged to submit all modifications to the Khronos group so that
 * they can be included in future versions of this file.  Please submit changes
 * by sending them to the public Khronos Bugzilla (http://khronos.org/bugzilla)
 * by filing a bug against product "EGL" component "Registry".
 */

#include <KHR/khrplatform.h>

/* Macros used in EGL function prototype declarations.
 *
 * EGL functions should be prototyped as:
 *
 * EGLAPI return-type EGLAPIENTRY eglFunction(arguments);
 * typedef return-type (EXPAPIENTRYP PFNEGLFUNCTIONPROC) (arguments);
 *
 * KHRONOS_APICALL and KHRONOS_APIENTRY are defined in KHR/khrplatform.h
 */

#ifndef EGLAPI
#define EGLAPI KHRONOS_APICALL
#endif

#ifndef EGLAPIENTRY
#define EGLAPIENTRY  KHRONOS_APIENTRY
#endif
#define EGLAPIENTRYP EGLAPIENTRY*

/* The types NativeDisplayType, NativeWindowType, and NativePixmapType
 * are aliases of window-system-dependent types, such as X Display * or
 * Windows Device Context. They must be defined in platform-specific
 * code below. The EGL-prefixed versions of Native*Type are the same
 * types, renamed in EGL 1.3 so all types in the API start with "EGL".
 *
 * Khronos STRONGLY RECOMMENDS that you use the default definitions
 * provided below, since these changes affect both binary and source
 * portability of applications using EGL running on different EGL
 * implementations.
 */

#if defined(_WIN32) || defined(__VC32__) && !defined(__CYGWIN__) && !defined(__SCITECH_SNAP__) /* Win32 and WinCE */
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN 1
#endif
#include <windows.h>

typedef HDC     EGLNativeDisplayType;
typedef HBITMAP EGLNativePixmapType;
typedef HWND    EGLNativeWindowType;

#elif defined(__WINSCW__) || defined(__SYMBIAN32__)  /* Symbian */

typedef int   EGLNativeDisplayType;
typedef void *EGLNativeWindowType;
typedef void *EGLNativePixmapType;

#elif defined(__unix__)

/* X11 (tentative)  */
#include <X11/Xlib.h>
#include <X11/Xutil.h>

typedef Display *EGLNativeDisplayType;
typedef Pixmap   EGLNativePixmapType;
typedef Window   EGLNativeWindowType;

#elif defined(RENESAS_NATIVE)
/*
 * [REL] Renesas platform
 */

#include <EGL/native.h>

typedef int           EGLNativeDisplayType;
typedef NativePixmap  EGLNativePixmapType;
typedef NativeWindow  EGLNativeWindowType;

#else
#error "Platform not recognized"
#endif

/* EGL 1.2 types, renamed for consistency in EGL 1.3 */
typedef EGLNativeDisplayType NativeDisplayType;
typedef EGLNativePixmapType  NativePixmapType;
typedef EGLNativeWindowType  NativeWindowType;


/* Define EGLint. This must be a signed integral type large enough to contain
 * all legal attribute names and values passed into and out of EGL, whether
 * their type is boolean, bitmask, enumerant (symbolic constant), integer,
 * handle, or other.  While in general a 32-bit integer will suffice, if
 * handles are 64 bit types, then EGLint should be defined as a signed 64-bit
 * integer type.
 */
typedef khronos_int32_t EGLint;


/* Renesas Platform */
typedef enum {
    EGL_REL_NO_ERROR                = 0,
    EGL_REL_EGL_INIT_RESOURCE_ERROR = 1,
	EGL_REL_RESOURCE_QUERY_ERROR    = 2
} EGLErrorCodeREL;

/*-----------------------------------------------------------------------------
 NAME       : eglStartUp
 FUNCTION   : Start up the EGL module
 PARAMETERS : None
 RETURN     : Success : EGL_REL_NO_ERROR
              Failure : Not EGL_REL_NO_ERROR
-----------------------------------------------------------------------------*/
EGLAPI EGLErrorCodeREL EGLAPIENTRY eglStartUp (void);

/*-----------------------------------------------------------------------------
 NAME       : eglCleanUp
 FUNCTION   : Clean up the EGL module
 PARAMETERS : None
 RETURN     : None
-----------------------------------------------------------------------------*/
EGLAPI void       EGLAPIENTRY eglCleanUp (void);


/******************************************************************************
 * EGL_RE_query_resource
 * added by S.Tokunaga
 */
#ifndef EGL_RE_query_resoruce
#define EGL_RE_query_resoruce           1

#define EGL_RES_OBJECT_RE               0x3800 /* eglQueryResourceRE target    */
#define EGL_RES_BUFFER_RE               0x3801 /* eglQueryResourceRE target    */
#define EGL_RES_CURRENT_SIZE_RE         0x3810 /* eglQueryResourceRE attribute */
#define EGL_RES_PEAK_SIZE_RE            0x3811 /* eglQueryResourceRE attribute */

/*-----------------------------------------------------------------------------
 NAME       : eglQueryResourceRE
 FUNCTION   : Query resource information
 PARAMETERS : target    : [IN ] Resource target
              attribute : [IN ] Attribute to query
 RETURN     : The queried information
-----------------------------------------------------------------------------*/
#ifdef EGL_EGLEXT_PROTOTYPES
EGLAPI EGLint EGLAPIENTRY eglQueryResourceRE (unsigned int target, unsigned int attribute);
#endif /* EGL_EGLEXT_PROTOTYPES */
typedef EGLint (EGLAPIENTRYP PFNEGLQUERYRESOURCEREPROC) (unsigned int target, unsigned int attribute);
#endif

#endif /* __eglplatform_h */
