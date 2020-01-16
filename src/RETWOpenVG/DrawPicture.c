/******************************************************************************
 *
 * $Rev$
 * $Date::                           #$
 *
 *****************************************************************************/
/******************************************************************************
* DISCLAIMER

* This software is supplied by Renesas Electronics Corp. and is only 
* intended for use with Renesas products. No other uses are authorized.

* This software is owned by Renesas Electronics Corp. and is protected under 
* all applicable laws, including copyright laws.

* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES
* REGARDING THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, 
* INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
* PARTICULAR PURPOSE AND NON-INFRINGEMENT.  ALL SUCH WARRANTIES ARE EXPRESSLY 
* DISCLAIMED.

* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS 
* ELECTRONICS CORP. NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE 
* FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES 
* FOR ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS 
* AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.

* Renesas reserves the right, without notice, to make changes to this 
* software and to discontinue the availability of this software.  
* By using this software, you agree to the additional terms and 
* conditions found by accessing the following link:
* http://www.renesas.com/disclaimer
******************************************************************************
 * Copyright (C) 2011 Renesas Electronics Corp. All Rights Reserved
 * AND Renesas Solutions Corp. All rights reserved.
*******************************************************************************/
/**************************************************************************//**
 * @file         DrawText.c
 * @version      1.00
 * @brief        
 *****************************************************************************/
/*****************************************************************************
* History : DD.MM.YYYY Version Description
*         : 11.11.2011 1.00    First Release
******************************************************************************/

/******************************************************************************
Includes
******************************************************************************/
#include    <stdio.h>
#include    <string.h>

#include    "r_typedefs.h"

#include    "VG/openvg.h"
#include    "VG/vgu.h"


#define			sWidth					800
#define			sHeight					480
/******************************************************************************
Definitions
******************************************************************************/
 

/******************************************************************************
Imported global variables and functions (from other files)
******************************************************************************/
extern uint16_t Graphics_ImageData[];
extern uint32_t Graphics_ImageData32[];					/* 32bits= B,G,R,00*/
extern uint8_t __image_btn1[];

/******************************************************************************
Exported global variables and functions (to be accessed by other files)
******************************************************************************/
int32_t TrVg_DrawingPicture (uint32_t dx_size, uint32_t dy_size );

/******************************************************************************
Private global variables and functions
******************************************************************************/
/* Path data of character 'S' */
 
/* Path command of character 'S' */
 
/* Path data of character 'H' */
 
/* Path command of character 'H' */
 
/* Sample text */
 


/**************************************************************************//**
 * Function Name : TrVg_Drawing
 * @brief       
 * @param       [in]void
 * @retval      int     : Return '0', if the error occurred.
 *****************************************************************************/
int32_t TrVg_DrawingPicture (uint32_t dx_size, uint32_t dy_size )
{
    VGImage Image;
    VGint   width;
    VGint   height;
    VGfloat scale;

		VGfloat matrix[9];
		
    width   = 200;										//same as Image_data.c
    height  = 200;										//same as Image_data.c
 
    vgSeti(VG_MATRIX_MODE, VG_MATRIX_IMAGE_USER_TO_SURFACE);
		vgLoadIdentity(); 
		
    vgTranslate(800-10-160, 480-10-160);
		scale = 1.0f;																		//no effective if using vguComputeWarpQuadToQuad
    vgScale(scale, scale);

    /* Set resampling quality */
    vgSeti(VG_IMAGE_QUALITY, VG_IMAGE_QUALITY_BETTER);
 
    /* Create a new image */
    Image = vgCreateImage(VG_lXRGB_8888, dx_size, dy_size, VG_IMAGE_QUALITY_BETTER);
    if (Image == VG_INVALID_HANDLE)
    {
    		printf("vgCreateImage  error\n");
        return -1;
    }
        
    /* Append the image data to the image object. If necessary, data format conversion will be done. */
    vgImageSubData(
        Image,
        Graphics_ImageData32,
        width * (sizeof (uint32_t)),
        VG_lXRGB_8888,																	//VG_sRGB_565,
        (VGint)0,												
        (VGint)0,																
        dx_size,
        dy_size);
#if 0
 		vguComputeWarpQuadToQuad( 400, 240, 		600,240 ,  	400, 360, 		600, 360, 									//dx
															0, 0, 	159, 0, 	0, 159,		159, 159, 															//sx
															matrix);
 

// 		vguComputeWarpQuadToQuad( 0, 0, 799,0 ,  799, 479, 0, 479, 											//dx
//															0, 0, 	639, 0, 	639, 479,		0, 479, 								//sx
//															matrix);
															
		vgLoadMatrix(matrix);	/* set the matrix */
		
#endif
 
    /* Draw the image to the current drawing surface */
    vgDrawImage(Image);
 
    /* Destroy the image */
    vgDestroyImage(Image);
 
 		
    return 0;
}   /* End of function DrawImage() */



int32_t TrVg_DrawingPictureOld (uint32_t framenum)
{
    VGImage Image;
    VGint   width;
    VGint   height;
    VGfloat scale;

		VGfloat matrix[9];
		 
    width   = 200;
    height  = 200;

    vgSeti(VG_MATRIX_MODE, VG_MATRIX_IMAGE_USER_TO_SURFACE);
		vgLoadIdentity();

    vgTranslate(200.0f, 40.0f);

		scale = 1.0f;
    
    vgScale(scale, scale);

    /* Set resampling quality */
    vgSeti(VG_IMAGE_QUALITY, VG_IMAGE_QUALITY_BETTER);

    /* Create a new image */
    Image = vgCreateImage(VG_lXRGB_8888, width, height, VG_IMAGE_QUALITY_BETTER);
    if (Image == VG_INVALID_HANDLE)
    {
        return -1;
    }
    /* Append the image data to the image object. If necessary, data format conversion will be done. */
    vgImageSubData(
        Image,
        Graphics_ImageData32,
        width * (sizeof (uint32_t)),
        VG_lXRGB_8888,																	//VG_sRGB_565,
        (VGint)0,
        (VGint)0,
        width,
        height);


    vgTranslate(10.0f, 10.0f);
    
#if 0 
		vguComputeWarpQuadToQuad( 400, 240, 			600, 240 , 			400, 360, 		600, 370, 

															0, 0, 159, 0, 0, 159,159, 159, 
															matrix);
		vgLoadMatrix(matrix);	/* set the matrix */
#endif

    /* Draw the image to the current drawing surface */
    vgDrawImage(Image);
    
    
    vgTranslate(400.0f, 240.0f);
    
#if 0 
		vguComputeWarpQuadToQuad( sWidth/2,sHeight/2, sWidth/2 -100, sHeight/2-50 ,  sWidth/2+100, sHeight/2-100,			sWidth/2+30, sHeight/2-180, 
															0, 0, 199, 0, 0, 199,199, 199, 
															matrix);
		vgLoadMatrix(matrix);	/* set the matrix */
#endif

    /* Draw the image to the current drawing surface */
    vgDrawImage(Image);    
    
 
    /* Destroy the image */
    vgDestroyImage(Image);
    
#if 0		//RETW try to draw screen by software

		if(framenum == 10)
		seccnt+=1;
		
		if(seccnt > 2)
		{
    			vgFinish();
					FillScreen(0x80000000);				//clear screen
					FillScreenColorBar();
					while(1){}
		}

#endif

    return 0;
}   /* End of function DrawImage() */

 