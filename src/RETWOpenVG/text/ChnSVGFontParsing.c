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

/******************************************************************************
Definitions
******************************************************************************/
/*
VG_MOVE_TO_ABS  { code:'M', command:'moveto', x:3, y:7 },
VG_LINE_TO_ABS  { code:'L', command:'lineto', x:5, y:-6 },
VG_LINE_TO_ABS  { code:'L', command:'lineto', x:1, y:7 },
VG_LINE_TO_ABS  { code:'L', command:'lineto', x:100, y:-0.4 },
VG_MOVE_TO_REL  { code:'m', command:'moveto', relative:true, x:-10, y:10 },
VG_LINE_TO_REL  { code:'l', command:'lineto', relative:true, x:10, y:0 },
VG_VLINE_TO_ABS  { code:'V', command:'vertical lineto', y:27 },
VG_VLINE_TO_ABS  { code:'V', command:'vertical lineto', y:89 },
VG_HLINE_TO_ABS  { code:'H', command:'horizontal lineto', x:23 },
VG_VLINE_TO_REL  { code:'v', command:'vertical lineto', relative:true, y:10 },
VG_HLINE_TO_REL  { code:'h', command:'horizontal lineto', relative:true, x:10 },
VG_CUBIC_TO_ABS  { code:'C', command:'curveto', x1:33, y1:43, x2:38, y2:47, x:43, y:47 },
VG_CUBIC_TO_REL  { code:'c', command:'curveto', relative:true, x1:0, y1:5, x2:5, y2:10, x:10, y:10 },
VG_SCUBIC_TO_ABS  { code:'S', command:'smooth curveto', x2:63, y2:67, x:63, y:67 },
VG_SCUBIC_TO_REL  { code:'s', command:'smooth curveto', relative:true, x2:-10, y2:10, x:10, y:10 },
VG_QUAD_TO_ABS  { code:'Q', command:'quadratic curveto', x1:50, y1:50, x:73, y:57 },
VG_QUAD_TO_REL  { code:'q', command:'quadratic curveto', relative:true, x1:20, y1:-5, x:0, y:-10 },
VG_SQUAD_TO_ABS  { code:'T', command:'smooth quadratic curveto', x:70, y:40 },
VG_SQUAD_TO_REL  { code:'t', command:'smooth quadratic curveto', relative:true, x:0, y:-15 },
  { code:'A', command:'elliptical arc', rx:5, ry:5, xAxisRotation:45, largeArc:true, sweep:false, x:40, y:20 },
  { code:'a', command:'elliptical arc', relative:true, rx:5, ry:5, xAxisRotation:20, largeArc:false, sweep:true, x:-10, y:-10 },

  VG_SCCWARC_TO_ABS                           = VG_SCCWARC_TO | VG_ABSOLUTE,
  VG_SCCWARC_TO_REL                           = VG_SCCWARC_TO | VG_RELATIVE,
  VG_SCWARC_TO_ABS                            = VG_SCWARC_TO  | VG_ABSOLUTE,
  VG_SCWARC_TO_REL                            = VG_SCWARC_TO  | VG_RELATIVE,
  VG_LCCWARC_TO_ABS                           = VG_LCCWARC_TO | VG_ABSOLUTE,
  VG_LCCWARC_TO_REL                           = VG_LCCWARC_TO | VG_RELATIVE,
  VG_LCWARC_TO_ABS                            = VG_LCWARC_TO  | VG_ABSOLUTE,
  VG_LCWARC_TO_REL                            = VG_LCWARC_TO  | VG_RELATIVE,
  
VG_CLOSE_PATH  { code:'Z', command:'closepath' }
	
*/
/******************************************************************************
Imported global variables and functions (from other files)
******************************************************************************/

/******************************************************************************
Exported global variables and functions (to be accessed by other files)
******************************************************************************/
 


#define 		SVG_DEBUG		 0

#define MaxChnCharNum 512
#define MaxChnStringNum 16

extern VGfloat 	G_CharScale;
extern VGfloat 	G_CharShearX;
extern VGfloat 	G_CharShearY;

typedef enum
{
    CHAR_CODE_0 = 0,
    CHAR_CODE_1,
    CHAR_CODE_2,
    CHAR_CODE_3,
    CHAR_CODE_4,
    CHAR_CODE_5,
    CHAR_CODE_6,
    CHAR_CODE_7,
    CHAR_CODE_8,
    CHAR_CODE_9,
    CHAR_CODE_10,
    CHAR_CODE_11,
    CHAR_CODE_NUM
} ChnCharSet ;

typedef enum
{
    StrNo_RZ_C1 =0,
    StrNo_RZ_C2,
    StrNo_RZ_C3,
    StrNo_RZ_C4,
    StrNo_RZ_C5,
    StrNo_RZ_C6,
    StrNo_RZ_C7,
    StrNo_Calendar,
    StrNo_MenuItem,
    StrNo_TemporatureHumility,
    StrNo_Meter,
    StrNo_Speed,
    StrNo_Clock,
    StrNo_MAX_NUM
} ChnStringNo ;

typedef struct
{
		uint32_t		UNICODE;
		uint32_t		CMD_NUM;
    uint32_t		CMD_ADDR;
    uint32_t		PATH_ADDR;
    uint32_t		ESCAPEMENT;
} st_CMDPATH;

typedef struct
{
		uint32_t		StringNo;
    uint32_t		Start_PTR;
    uint32_t		StringLength;
} st_DSPCHN_STR;

 
static VGuint SampleChnText[MaxChnCharNum];

static const uint32_t LUT_FastSearch[16]={
0,
0x10990,
0x5e3,
0x1f60,
0x218bb9,
0x4d1e73,
0x705b89,
0x982ea8,
0xc32ead,
0xec48cb,
0x86ff,
0x85cb,
0x5909,
0x5e44,
0x633d,
0x77fe};

/******************************************************************************
Private global variables and functions
******************************************************************************/
VGubyte SVG_CHNFONT_CMD_Tab[25600];				// __attribute((section("CHNFONT_BUF")));
VGfloat SVG_CHNFONT_PATH_Tab[51200];			//  __attribute((section("CHNFONT_BUF"))); 
uint32_t SVG_CHNFONT_CMD_ptr=0;
uint32_t SVG_CHNFONT_PATH_ptr=0; 
uint16_t CharNumtoBeDsp=0;
uint16_t CHNCHARCodeCnt=0;
uint16_t StrNo_Cnt=0;
uint16_t Str_fillPtr=0;
uint16_t LastStr_fillPtr=0;

st_CMDPATH svg_CMD_PATH_LUT[MaxChnCharNum];				// __attribute((section("CHNFONT_BUF")));							//Structure to store a character's path and command 
st_DSPCHN_STR DSPCHN_STR[StrNo_MAX_NUM];


#if SVG_DEBUG
int AcculatedPath=0;
#endif 
/****************************************************************************/
/* Extract all SVG string to command and path data table  									*/
/*		DspSTR	: start of Chn Char string to be display codeing by unicode   */
/*		Length : length of Chn Char string on unicode													*/			
/****************************************************************************/
int RETW_ChnSVGFontOpen( uint16_t *DspSTR, uint16_t Length, uint32_t FontTABStart, uint32_t FontTABSize) 
{
			int i,j;
			uint16_t Unicode;
    	VGPath path ;
    	VGPaint fill ;  
    	VGfloat Origin[] = { 0.0f, 0.0f } ;
    	VGfloat Escapement[] = { 32.0f, 0.0f } ;
    	char UnicodeStr[6];
			uint16_t *StrPtr;

			uint32_t Last_CMD_Ptr;
			uint32_t Last_PATH_Ptr;
			
//			SVG_CHNFONT_CMD_ptr =0;
//			SVG_CHNFONT_PATH_ptr =0;
			StrPtr = DspSTR;
#if SVG_DEBUG
			printf("Length is %d\n",Length);
#endif
			Unicode=*StrPtr++;
			
#if SVG_DEBUG
			DumpData(StrPtr,100);
#endif
			if(Unicode == 0xFeff)			//if this string is Unicode
				{	
#if 1
					DSPCHN_STR[StrNo_Cnt].StringNo = StrNo_Cnt;
					DSPCHN_STR[StrNo_Cnt].Start_PTR = CHNCHARCodeCnt;
					DSPCHN_STR[StrNo_Cnt].StringLength = Length-1;					//reduce 1, 0xFEFF header
		#if SVG_DEBUG
					printf("StrNo_Cnt %d, Start_PTR is %d, Length is %d\n", StrNo_Cnt,CHNCHARCodeCnt, DSPCHN_STR[StrNo_Cnt].StringLength );
		#endif
					StrNo_Cnt+=1;
#endif

				for(i=1;i<(Length);i++)
					{
					Unicode=*StrPtr++;
 					#if SVG_DEBUG
							printf("CHARACTER %d, unicode = %x ---", i,Unicode );
					#endif
					UnicodeStr[0]= '#';
					UnicodeStr[1]= 'x';
					UnicodeStr[2]= (char) RETW_HextoAscii(Unicode>>12);
					UnicodeStr[3]= (char) RETW_HextoAscii(Unicode>>8);
					UnicodeStr[4]= (char) RETW_HextoAscii(Unicode>>4);
					UnicodeStr[5]= (char) RETW_HextoAscii(Unicode);
					
					#if SVG_DEBUG
						for(j=0;j<6;j++)
 						IoPutchar(UnicodeStr[j]);
 						
 						IoPutchar(0x0d);
 						IoPutchar(0x0a);
 					#endif

					Last_CMD_Ptr = SVG_CHNFONT_CMD_ptr;
					Last_PATH_Ptr = SVG_CHNFONT_PATH_ptr;
 
 					RETW_SVGFontParsing(&UnicodeStr[0],6, FontTABStart, FontTABSize);

 					svg_CMD_PATH_LUT[CHNCHARCodeCnt].CMD_NUM = SVG_CHNFONT_CMD_ptr-Last_CMD_Ptr;
 					svg_CMD_PATH_LUT[CHNCHARCodeCnt].CMD_ADDR = &SVG_CHNFONT_CMD_Tab[Last_CMD_Ptr];
 					svg_CMD_PATH_LUT[CHNCHARCodeCnt].PATH_ADDR  = &SVG_CHNFONT_PATH_Tab[Last_PATH_Ptr];
 					svg_CMD_PATH_LUT[CHNCHARCodeCnt].ESCAPEMENT  = 32;

#if 0			//
 					SampleChnText[i-1] = CHNCHARCodeCnt;							//encoded cahrcode from 0...
#else
 					SampleChnText[CHNCHARCodeCnt] = CHNCHARCodeCnt;							//encoded cahrcode from 0...
#endif

     			CHNCHARCodeCnt +=1;     			
    			if(CHNCHARCodeCnt > MaxChnCharNum)
    					while(1)
    					printf("RETW_ChnSVGFontOpen Error!!!\n");
			#if SVG_DEBUG
					printf("Pasred %d-th ChnCharacters SVGFont,total CMD %d,Total Path %d\n",CHNCHARCodeCnt, SVG_CHNFONT_CMD_ptr, SVG_CHNFONT_PATH_ptr);
 
 					printf("Last CMD, current CMD, current PATH = %d, %d, %d\n",Last_CMD_Ptr, SVG_CHNFONT_CMD_ptr, SVG_CHNFONT_PATH_ptr);
 					printf("AppendPath = %x, %x, %x\n\n",SVG_CHNFONT_CMD_ptr-Last_CMD_Ptr, &SVG_CHNFONT_CMD_Tab[Last_CMD_Ptr], &SVG_CHNFONT_PATH_Tab[Last_PATH_Ptr]);
 			#endif
					}
 
				}
				else
				printf("The String is not unicode!!!!!!!!!\n");
				
				CharNumtoBeDsp = CHNCHARCodeCnt;
				return CHNCHARCodeCnt;
}



RETW_Demo()
{
	int i;
	
	uint32_t x,y;
	
	for(i=0;i<10;i++)
	{
		
		x=i*40;
		y=i*20;
		
		RETW_DrawingChineseOneChar(0,i, x, y);
	
	}
}




int RETW_SVGFontParsingTOOL()					//this is a tool to find start addres of #&1xxx,2xxx,3xxx,4xxx,5xxx,6xxx,7xxx,8xxx,9xxx
{
		uint32_t i,j;
		unsigned char TempChar;
		unsigned char TempChar2;
		unsigned char	CmpString[8];
		uint32_t  SVG_codesize;
		uint8_t * psrc;

    psrc = (uint32_t *)0x18800000;		//SVG font source address
    SVG_codesize = 18905364;					//JhengHeiBold binary code side
    
    CmpString[0]='&';
    CmpString[1]='#';
    CmpString[2]='x';
    
    for(i=1;i<16;i++)
    {
    	psrc = (uint32_t *)0x18800000;		//SVG font source address
    	if(i<10)
    	CmpString[3]=0x30+i;
    	else
    	CmpString[3]=0x61+(i-10);
    	
        while(1)
        {
  				if( strncmp( psrc ,&CmpString[0],4 ) == 0 )					//if got unicode match
  						{
 							printf("Found #&x%x at address %x\n",i, psrc-0x18800000);
    				  break;
    					}
    			psrc++;

					if( psrc > (0x18800000+SVG_codesize) )
    				{
    					printf("*** No matched unicode found\n");
    					return 0;
    				}    			
    		}
    }
}

int RETW_SVGFontParsing( char *Instring, int length , uint32_t TABStart, uint32_t TABSize)					//unicode and its length
{
		uint32_t i,j;
		uint32_t LineNo=1;
		unsigned char TempChar;
		unsigned char TempChar2;
		
		unsigned char	CmpString[8];
		
		uint32_t  SVG_codesize = TABSize;
		uint8_t * psrc;

    psrc = (uint32_t *)TABStart;						//SVG font source address

//		psrc =  (0x18800000+0x00681bA0);
		
//		SVG_CHNFONT_CMD_ptr =0;
//		SVG_CHNFONT_PATH_ptr =0; 
 
		for(i=0;i<length;i++)
				CmpString[i] = *Instring++;
 
        while(1)
        {
        		TempChar = (*psrc++);
        		i=0;
    				switch (TempChar)
    				{
    				    case 0x0A:
    				        	LineNo+=1;
    				    		break;    				    
    				    case 0x26:			// if it is '&'
 
//											printf(" Get & char %x at line %d from %x \n", TempChar, LineNo , psrc );
 
//  											for(j=0;j<0x20;j++)
//  											IoPutchar(*psrc++); 											
 
  										if( strncmp( psrc ,&CmpString[0],length ) == 0 )					//if got unicode match
//											if( RETW_StringCompare( psrc ,&CmpString[0],length ) == 0 )					//if got unicode match
													{
														#if SVG_DEBUG
														printf("Found matched &#x unicode at line %d\n",LineNo);
														#endif
														
														psrc+=6;
														
														while(strncmp( psrc ,"d=",2 ) != 0  ) {psrc++;}
 
 
														while(  (*psrc++) != 0x22 ){}							//search first '"'
 
														
 														while(1)
 														{
 																TempChar2 = (*psrc);
 																
 
 																
 																if(TempChar2==0x22) 									//found 2nd '"'
 																		{
 															 			IoPutchar(0x0d);
 															 			IoPutchar(0x0a); 																			
 																		break;											//end of string
 																		}
 															 	else if(TempChar2==0x0a)
 															 			{
 															 				#if SVG_DEBUG
    															 			IoPutchar(0x0d);
    															 			IoPutchar(0x0a);
																			#endif
 															 		  }
 															 	else
 															 			{
 															 			switch (TempChar2)
 															 					{
 															 						case 'M' :
 		 			
 															 											SVG_CHNFONT_CMD_Tab[SVG_CHNFONT_CMD_ptr] = (VG_MOVE_TO_ABS);
 															 											SVG_CHNFONT_CMD_ptr++;
																										RETW_SVGFontParsing_GetPath(TempChar2, psrc );
																										
 															 											break;
 															 						case 'm' :
 															 											SVG_CHNFONT_CMD_Tab[SVG_CHNFONT_CMD_ptr] = (VG_MOVE_TO_REL);
 															 											SVG_CHNFONT_CMD_ptr++;
																										RETW_SVGFontParsing_GetPath(TempChar2, psrc );
 															 											break;
 															 						case 'L' :
 															 											SVG_CHNFONT_CMD_Tab[SVG_CHNFONT_CMD_ptr] = (VG_LINE_TO_ABS);
 															 											SVG_CHNFONT_CMD_ptr++;			
																										RETW_SVGFontParsing_GetPath(TempChar2, psrc );							 							
 															 											break;
 															 						case 'l' :
 															 											SVG_CHNFONT_CMD_Tab[SVG_CHNFONT_CMD_ptr] = (VG_LINE_TO_REL);
 															 											SVG_CHNFONT_CMD_ptr++;
																										RETW_SVGFontParsing_GetPath(TempChar2, psrc );
 															 											break;															 						
  															 					case 'V' :
 															 											SVG_CHNFONT_CMD_Tab[SVG_CHNFONT_CMD_ptr] = (VG_VLINE_TO_ABS);
 															 											SVG_CHNFONT_CMD_ptr++;
																										RETW_SVGFontParsing_GetPath(TempChar2, psrc );
 															 											break;
 															 						case 'v' :
 															 											SVG_CHNFONT_CMD_Tab[SVG_CHNFONT_CMD_ptr] = (VG_VLINE_TO_REL);
 															 											SVG_CHNFONT_CMD_ptr++;
																										RETW_SVGFontParsing_GetPath(TempChar2, psrc );
 															 											break;															 						
  															 					case 'H' :
 															 											SVG_CHNFONT_CMD_Tab[SVG_CHNFONT_CMD_ptr] = (VG_HLINE_TO_ABS);
 															 											SVG_CHNFONT_CMD_ptr++;
																										RETW_SVGFontParsing_GetPath(TempChar2, psrc );								 															 							
 															 											break;
 															 						case 'h' :
 															 											SVG_CHNFONT_CMD_Tab[SVG_CHNFONT_CMD_ptr] = (VG_HLINE_TO_REL);
 															 											SVG_CHNFONT_CMD_ptr++;
																										RETW_SVGFontParsing_GetPath(TempChar2, psrc );												 											 															 							
 															 											break;
 															 						case 'C' :
 															 											SVG_CHNFONT_CMD_Tab[SVG_CHNFONT_CMD_ptr] = (VG_CUBIC_TO_ABS);
 															 											SVG_CHNFONT_CMD_ptr++; 	
																										RETW_SVGFontParsing_GetPath(TempChar2, psrc );			
 															 											break;
 															 						case 'c' :
 															 											SVG_CHNFONT_CMD_Tab[SVG_CHNFONT_CMD_ptr] = (VG_CUBIC_TO_REL);
 															 											SVG_CHNFONT_CMD_ptr++;	
																										RETW_SVGFontParsing_GetPath(TempChar2, psrc );	
 															 											break;
 															 						case 'S' :
 															 											SVG_CHNFONT_CMD_Tab[SVG_CHNFONT_CMD_ptr] = (VG_SCUBIC_TO_ABS);
 															 											SVG_CHNFONT_CMD_ptr++; 															 							
																										RETW_SVGFontParsing_GetPath(TempChar2, psrc );
 															 											break;
 															 						case 's' :															 						
 															 											SVG_CHNFONT_CMD_Tab[SVG_CHNFONT_CMD_ptr] = (VG_SCUBIC_TO_REL);
 															 											SVG_CHNFONT_CMD_ptr++;  															 							
																										RETW_SVGFontParsing_GetPath(TempChar2, psrc );			
 															 											break;
  															 					case 'Q' :

 															 											SVG_CHNFONT_CMD_Tab[SVG_CHNFONT_CMD_ptr] = (VG_QUAD_TO_ABS);
 															 											SVG_CHNFONT_CMD_ptr++;  	
																										RETW_SVGFontParsing_GetPath(TempChar2, psrc );
 															 											break;
 															 						case 'q' :
 															 											SVG_CHNFONT_CMD_Tab[SVG_CHNFONT_CMD_ptr] = (VG_QUAD_TO_REL);
 															 											SVG_CHNFONT_CMD_ptr++;  	
																										RETW_SVGFontParsing_GetPath(TempChar2, psrc );
 															 											break;									 						
  															 					case 'T' :
 															 											SVG_CHNFONT_CMD_Tab[SVG_CHNFONT_CMD_ptr] = (VG_SQUAD_TO_ABS);
 															 											SVG_CHNFONT_CMD_ptr++;  
																										RETW_SVGFontParsing_GetPath(TempChar2, psrc );											 							
 															 											break;
 															 						case 't' :
 															 											SVG_CHNFONT_CMD_Tab[SVG_CHNFONT_CMD_ptr] = (VG_SQUAD_TO_REL);
 															 											SVG_CHNFONT_CMD_ptr++;  
																										RETW_SVGFontParsing_GetPath(TempChar2, psrc );
 																										break;
 															 						case 'z' :
 															 											SVG_CHNFONT_CMD_Tab[SVG_CHNFONT_CMD_ptr] = (VG_CLOSE_PATH);
 															 											SVG_CHNFONT_CMD_ptr++;  
 															 											
 																										break;																								
 																					default :	
 																										#if SVG_DEBUG
 																											IoPutchar(TempChar2);
 																										#endif
 																										break;
 															 					}
 															 		}
 															 	psrc++;											
 														}			//while(1)		//process parsing after got unicode
 														
 														return 1;				// early ending before search till end of SVG
 														
													}	//if got unicode match 								
    				    		break;
    				    default:
    				    		break;
    				}	//switch 
    				
    				
					if( psrc > (TABStart+SVG_codesize) )
//				if(psrc> (0x18800000+ 0x1400000) )	
    				{
    					printf("*** No matched unicode found, Seraching till line %d\n",LineNo);
    					return 0;
    				}
        }		//while(1)
//        printf("Total Path %d, Path Ptr %d \n", AcculatedPath,SVG_CHNFONT_PATH_ptr);
}
void RETW_SVGFontParsing_GetPath(char Code, uint32_t Addr) 
{
			unsigned char MinusFlag=0;
			unsigned char TempChar;
			int PathDotCnt=0;
			int i,j;
			
			char *p_path;
			char Str_path[16];
 	
			float	Para=0;
			float result;
			
			p_path = Addr;			
			p_path++;
			i=0;

			while(1)
			{
				TempChar=(*p_path++);
				Str_path[i]= TempChar;
				i++;
				
				if( TempChar == 0x20)
				{
//DumpData(&Str_path[0], 16);printf("------------by 1 Space\n");	
//printf("i = %d\n",i);
					RETW_SVGFontParsing_ConvPath(&Str_path[0], i-1);
//DumpData(&Str_path[0], 16);printf("------------by 2 Space\n");	
					
					i=0;
					PathDotCnt+=1;
			  }
				else if(  ( (TempChar > 0x40) && (TempChar < 0x5B) ) || ( (TempChar > 0x60) && (TempChar < 0x7B) )  )
				{
//DumpData(&Str_path[0], 32);printf("------------by ABCD\n");
					RETW_SVGFontParsing_ConvPath(&Str_path[0], i-1);
					i=0;
					PathDotCnt+=1;
					break;
				}
			}

		#if SVG_DEBUG 
			
			IoPutchar(0x0d);
			IoPutchar(0x0a);
			AcculatedPath = AcculatedPath+PathDotCnt;
      printf("Para num %d ---  Accucated Path %d ",	PathDotCnt,AcculatedPath);
  		IoPutchar(Code);
  		
  	#endif
}

void RETW_SVGFontParsing_ConvPath(char *CharCode, uint32_t length)
{
			unsigned char MinusFlag=0;
			unsigned char DotFlag=0;
			unsigned char Divider=10;			
			unsigned char TempChar;
			char *p_path;
			float Para,result;
 			uint32_t Len = length;
 
			p_path = CharCode;
			Para =0;
			
			while(Len>0)
			{

//printf("len is %d\n",Len);
				
 					TempChar = *p_path;
 			
 					switch (TempChar)
 							{
 							case '-' : 
 											MinusFlag=1;
 							 				break;
 							case '.' :
 											DotFlag=1;
 											break;				
 							case ' ' :
// 														if(MinusFlag==1)
// 														result = 0-Para;
//printf("Met space\n");
											break;
 							default :
 											if( (TempChar > 0x2F) && (TempChar < 0x3A) )
 													{
 														if(DotFlag==0)
 														Para = Para*10 + (TempChar-0x30);  
 														else
 														{
 														Para = Para + ((TempChar-0x30)/Divider); 
 														Divider= Divider*10;
 														}
 													}
 											else if(  ( (TempChar > 0x40) && (TempChar < 0x5B) ) || ( (TempChar > 0x60) && (TempChar < 0x7B) )  )
 													{
 														if(MinusFlag==1)
 														result = 0-Para;
 													} 
 											break;
 							}
 				 	p_path++;
 				 	Len-=1;
 			} 
// 			printf("Finished PathConv-1\n");
 			if(MinusFlag==1)
 				result = 0-Para;
 			else
 				result = Para;
 				
 			result = result*32/2048;
 			
			SVG_CHNFONT_PATH_Tab[SVG_CHNFONT_PATH_ptr] =result;
			SVG_CHNFONT_PATH_ptr+=1;
			
//			printf("Finished PathConv-2\n");		
			return;
}
int TrVg_DrawingChineseText( void )
{
    VGFont FontHdl ;
    VGPath path ;
    VGPaint fill ;
    uint32_t AADV;
    
    VGfloat Origin[] = { 0.0f, 0.0f } ;
    VGfloat Escapement[] = { 32.0f, 0.0f } ;
    float FontScale;
#if 1
    /* Creates a new font object */
    FontHdl = vgCreateFont( CHAR_CODE_NUM ) ;
    if( FontHdl == VG_INVALID_HANDLE ){
        return 0 ;
    }
    /*** Glyph of the character 'Chiang' ***/
 
    /*** Glyph of the character 'Chun' ***/
    path = vgCreatePath( VG_PATH_FORMAT_STANDARD, VG_PATH_DATATYPE_F, 1.0f, 0.0f, 0, 0, VG_PATH_CAPABILITY_ALL ) ;
    /* Appends data to the path */
    vgAppendPathData( path,   SVG_CHNFONT_CMD_ptr ,  SVG_CHNFONT_CMD_Tab,  SVG_CHNFONT_PATH_Tab ) ;
    /* Creates a glyph and assigns the given path to a glyph that corresponds to the specified index */
    vgSetGlyphToPath( FontHdl, ( VGuint )CHAR_CODE_0, path, VG_FALSE, Origin, Escapement ) ;
    /* Destroy the path */
    vgDestroyPath( path );

#endif
				AADV =  R_ADC_Read();						//1024
				FontScale =  0.8;
				FontScale = AADV * 0.9;
				FontScale = FontScale/1024;
				
 				G_CharScale = 8.0 * FontScale;
 				
				G_CharShearX=0.0;
				G_CharShearY=0.0;	
 
    /*** Text color ***/
    /* Create a new paint object */
    fill = vgCreatePaint() ;
    /* Set the paint type to VG_PAINT_TYPE_COLOR */
    vgSetParameteri( fill, VG_PAINT_TYPE, VG_PAINT_TYPE_COLOR ) ;
    /* Set the color and alpha value in sRGBA8888 format to the paint object (shorthand form) */
    vgSetColor( fill, ( VGuint )0x0000FFFF ) ;          /* Blue */
    /* Paint object is set on the current context */
    vgSetPaint( fill, VG_FILL_PATH ) ;

    /* Set the matrix mode to allow manipulation of the glyph-user-to-surface transformation */
    vgSeti( VG_MATRIX_MODE, VG_MATRIX_GLYPH_USER_TO_SURFACE ) ;
    /* Set the current matrix to the identity matrix  */
 
    vgLoadIdentity() ;
    vgTranslate( 360.0f, 100.0f ) ;
    
    vgScale( G_CharScale, G_CharScale ) ;
//    vgSetColor( fill, ( VGuint )0xFF0000FF ) ;          /* Red */

    /* Set the glyph origin */
    vgSetfv( VG_GLYPH_ORIGIN, 2, Origin ) ;
    
    /* Renders a sequence of glyphs specified by the array of the indices */
    vgDrawGlyphs( FontHdl, ( VGint )( sizeof SampleChnText / sizeof ( VGuint )), SampleChnText, NULL, NULL, VG_FILL_PATH, VG_FALSE ) ;

    /* Destroy the paint objects */
    vgDestroyPaint( fill );

    /* Destroys the font object */
    vgDestroyFont( FontHdl );
}
int TrVg_DrawingChineseTextString( uint32_t DspSize )
{	
		int i,j;
    VGFont ChnFontHdl;
    
    float Loc_X=400, Loc_Y=70;
    
    VGPath path ;
    VGPaint fill ;
    uint32_t AADV;
    
    VGfloat Origin[] = { 0.0f, 0.0f } ;
    VGfloat Escapement[] = { 32.0f, 0.0f } ;
    float FontScale = 1.0;
    
#if 1
    /* Creates a new font object */
    ChnFontHdl = vgCreateFont( MaxChnCharNum ) ;
    if( ChnFontHdl == VG_INVALID_HANDLE ){
        return 0 ;
    }
#endif
					for(i=0;i<CharNumtoBeDsp;i++)					// Append Path data
					{		
    			/*** Glyph of the character 'Chun' ***/
    			path = vgCreatePath( VG_PATH_FORMAT_STANDARD, VG_PATH_DATATYPE_F, 1.0f, 0.0f, 0, 0, VG_PATH_CAPABILITY_ALL ) ;
    			/* Appends data to the path */
    			vgAppendPathData( path, svg_CMD_PATH_LUT[i].CMD_NUM , svg_CMD_PATH_LUT[i].CMD_ADDR,  svg_CMD_PATH_LUT[i].PATH_ADDR ) ;
    			/* Creates a glyph and assigns the given path to a glyph that corresponds to the specified index */
    			vgSetGlyphToPath( ChnFontHdl, ( VGuint )i, path, VG_FALSE, Origin, Escapement ) ;
    			/* Destroy the path */
    			vgDestroyPath( path );
 					}
#if 0
 				AADV =  R_ADC_Read();						//1024
 
 				FontScale = AADV * 1.0;
				FontScale = FontScale/1024;
				G_CharScale = 2.0 * FontScale;
				
				Loc_Y = Loc_Y + (AADV*(240-Loc_Y)/1024);
#endif
				
#if 0
				Loc_Y = Loc_Y + (DspSize*(240-Loc_Y)/2880);
 				G_CharScale = 1.0 * FontScale * DspSize/2880;						//max is 1.0
#endif
 
#if 1
 				G_CharScale = 1.0;
#endif 

				Loc_X = Loc_X - (CharNumtoBeDsp*32/2)*G_CharScale;
				Loc_Y = Loc_Y - (G_CharScale*32/2);
				
				G_CharShearX=0.0;
				G_CharShearY=0.0;	
 
    /*** Text color ***/
    /* Create a new paint object */
    fill = vgCreatePaint() ;
    /* Set the paint type to VG_PAINT_TYPE_COLOR */
    vgSetParameteri( fill, VG_PAINT_TYPE, VG_PAINT_TYPE_COLOR ) ;
    /* Set the color and alpha value in sRGBA8888 format to the paint object (shorthand form) */
    vgSetColor( fill, ( VGuint )0x0000F0FF ) ;          /* Blue */
    /* Paint object is set on the current context */
    vgSetPaint( fill, VG_FILL_PATH ) ;

    /* Set the matrix mode to allow manipulation of the glyph-user-to-surface transformation */
    vgSeti( VG_MATRIX_MODE, VG_MATRIX_GLYPH_USER_TO_SURFACE ) ;
    /* Set the current matrix to the identity matrix  */
 
    vgLoadIdentity() ;
//    vgTranslate( 300.0f, 70.0f );
			vgTranslate( Loc_X, Loc_Y );    
    vgScale( G_CharScale, G_CharScale ) ;
//    vgSetColor( fill, ( VGuint )0xFF0000FF ) ;          /* Red */
//    vgShear(G_CharShearX, G_CharShearY);
    
    
    /* Set the glyph origin */
    vgSetfv( VG_GLYPH_ORIGIN, 2, Origin ) ;
    
    /* Renders a sequence of glyphs specified by the array of the indices */
//    vgDrawGlyphs(  ChnFontHdl, ( VGint )( sizeof SampleChnText / sizeof( VGuint )),  SampleChnText, NULL, NULL, VG_FILL_PATH, VG_FALSE ) ;
    vgDrawGlyphs(  ChnFontHdl, ( VGint )CharNumtoBeDsp,  SampleChnText, NULL, NULL, VG_FILL_PATH, VG_FALSE ) ;
 
    /* Destroy the paint objects */
    vgDestroyPaint( fill ) ;

    /* Destroys the font object */
     vgDestroyFont( ChnFontHdl ) ;
}

int RETW_DrawingChineseTextString( uint32_t StrNo, uint32_t DrawCenterX,  uint32_t DrawCenterY)
{	
		int i,j;
    VGFont ChnFontHdl;
    
    float Loc_X=DrawCenterX, Loc_Y=DrawCenterY;
    
    VGPath path ;
    VGPaint fill ;
    uint32_t AADV;
    
    VGfloat Origin[] = { 0.0f, 0.0f } ;
    VGfloat Escapement[] = { 32.0f, 0.0f } ;
    float FontScale = 1.0;

		if(	DSPCHN_STR[StrNo].StringNo != StrNo)
				while(1)
				printf("StrNo is not matched\n");
#if 1
    /* Creates a new font object */
    ChnFontHdl = vgCreateFont( MaxChnCharNum ) ;
    if( ChnFontHdl == VG_INVALID_HANDLE ){
        return 0 ;
    }
#endif
					for(i=0;i<DSPCHN_STR[StrNo].StringLength;i++)					// Append Path data
					{		
    			/*** Glyph of the character 'Chun' ***/
    			path = vgCreatePath( VG_PATH_FORMAT_STANDARD, VG_PATH_DATATYPE_F, 1.0f, 0.0f, 0, 0, VG_PATH_CAPABILITY_ALL ) ;
    			/* Appends data to the path */
    			vgAppendPathData( path, svg_CMD_PATH_LUT[DSPCHN_STR[StrNo].Start_PTR+i].CMD_NUM , svg_CMD_PATH_LUT[DSPCHN_STR[StrNo].Start_PTR+i].CMD_ADDR,  svg_CMD_PATH_LUT[DSPCHN_STR[StrNo].Start_PTR+i].PATH_ADDR ) ;
    			/* Creates a glyph and assigns the given path to a glyph that corresponds to the specified index */
    			vgSetGlyphToPath( ChnFontHdl, ( VGuint )SampleChnText[DSPCHN_STR[StrNo].Start_PTR+i], path, VG_FALSE, Origin, Escapement );
    			/* Destroy the path */
    			vgDestroyPath( path );
 					} 
#if 1
 				G_CharScale = 1.0;
#endif 

				Loc_X = Loc_X - (DSPCHN_STR[StrNo].StringLength*32/2)*G_CharScale;
				Loc_Y = Loc_Y - (G_CharScale*32/2);
				
				G_CharShearX=0.0;
				G_CharShearY=0.0;	
 
    /*** Text color ***/
    /* Create a new paint object */
    fill = vgCreatePaint() ;
    /* Set the paint type to VG_PAINT_TYPE_COLOR */
    vgSetParameteri( fill, VG_PAINT_TYPE, VG_PAINT_TYPE_COLOR ) ;
    /* Set the color and alpha value in sRGBA8888 format to the paint object (shorthand form) */
    vgSetColor( fill, ( VGuint )0x0000F0FF ) ;          /* Blue */
    /* Paint object is set on the current context */
    vgSetPaint( fill, VG_FILL_PATH ) ;

    /* Set the matrix mode to allow manipulation of the glyph-user-to-surface transformation */
    vgSeti( VG_MATRIX_MODE, VG_MATRIX_GLYPH_USER_TO_SURFACE ) ;
    /* Set the current matrix to the identity matrix  */
 
    vgLoadIdentity() ;
//    vgTranslate( 300.0f, 70.0f );
			vgTranslate( Loc_X, Loc_Y );    
    vgScale( G_CharScale, G_CharScale ) ;
//    vgSetColor( fill, ( VGuint )0xFF0000FF ) ;          /* Red */
//    vgShear(G_CharShearX, G_CharShearY);
    
    
    /* Set the glyph origin */
    vgSetfv( VG_GLYPH_ORIGIN, 2, Origin ) ;
    
    /* Renders a sequence of glyphs specified by the array of the indices */
//    vgDrawGlyphs(  ChnFontHdl, ( VGint )( sizeof SampleChnText / sizeof( VGuint )),  SampleChnText, NULL, NULL, VG_FILL_PATH, VG_FALSE ) ;
    vgDrawGlyphs(  ChnFontHdl, ( VGint )DSPCHN_STR[StrNo].StringLength,  &SampleChnText[DSPCHN_STR[StrNo].Start_PTR], NULL, NULL, VG_FILL_PATH, VG_FALSE ) ;
 
    /* Destroy the paint objects */
    vgDestroyPaint( fill ) ;

    /* Destroys the font object */
     vgDestroyFont( ChnFontHdl ) ;
}

int RETW_DrawingChineseOneChar( uint32_t StrNo, uint32_t NumberChar, uint32_t DrawStartX,  uint32_t DrawStartY)
{	
		int i,j;
    VGFont ChnFontHdl;
    
    float Loc_X=DrawStartX, Loc_Y=DrawStartY;
    
    VGPath path ;
    VGPaint fill ;
 
    VGfloat Origin[] = { 0.0f, 0.0f } ;
    VGfloat Escapement[] = { 32.0f, 0.0f };
    float FontScale = 1.0;

		if( (	DSPCHN_STR[StrNo].StringNo != StrNo) | (NumberChar>DSPCHN_STR[StrNo].StringLength ) )
				while(1)
				printf("StrNo is not matched\n");
#if 1
    /* Creates a new font object */
    ChnFontHdl = vgCreateFont( MaxChnCharNum ) ;
    if( ChnFontHdl == VG_INVALID_HANDLE ){
        return 0 ;
    }
#endif

					i = NumberChar;	
    			/*** Glyph of the character 'Chun' ***/
    			path = vgCreatePath( VG_PATH_FORMAT_STANDARD, VG_PATH_DATATYPE_F, 1.0f, 0.0f, 0, 0, VG_PATH_CAPABILITY_ALL ) ;
    			/* Appends data to the path */
    			vgAppendPathData( path, svg_CMD_PATH_LUT[DSPCHN_STR[StrNo].Start_PTR+i].CMD_NUM , svg_CMD_PATH_LUT[DSPCHN_STR[StrNo].Start_PTR+i].CMD_ADDR,  svg_CMD_PATH_LUT[DSPCHN_STR[StrNo].Start_PTR+i].PATH_ADDR ) ;
    			/* Creates a glyph and assigns the given path to a glyph that corresponds to the specified index */
    			vgSetGlyphToPath( ChnFontHdl, ( VGuint )SampleChnText[DSPCHN_STR[StrNo].Start_PTR+i], path, VG_FALSE, Origin, Escapement );
    			/* Destroy the path */
    			vgDestroyPath( path );
 
#if 1
 				G_CharScale = 1.0;
#endif 
				
				G_CharShearX=0.0;
				G_CharShearY=0.0;	
 
    /*** Text color ***/
    /* Create a new paint object */
    fill = vgCreatePaint() ;
    /* Set the paint type to VG_PAINT_TYPE_COLOR */
    vgSetParameteri( fill, VG_PAINT_TYPE, VG_PAINT_TYPE_COLOR ) ;
    /* Set the color and alpha value in sRGBA8888 format to the paint object (shorthand form) */
    vgSetColor( fill, ( VGuint )0x0000F0FF ) ; 
    /* Paint object is set on the current context */
    vgSetPaint( fill, VG_FILL_PATH ) ;

    /* Set the matrix mode to allow manipulation of the glyph-user-to-surface transformation */
    vgSeti( VG_MATRIX_MODE, VG_MATRIX_GLYPH_USER_TO_SURFACE ) ;
    /* Set the current matrix to the identity matrix  */
 
    vgLoadIdentity() ;
//    vgTranslate( 300.0f, 70.0f );
			vgTranslate( Loc_X, Loc_Y );    
    vgScale( G_CharScale, G_CharScale ) ;
//    vgSetColor( fill, ( VGuint )0xFF0000FF ) ;          /* Red */
//    vgShear(G_CharShearX, G_CharShearY);
    
    
    /* Set the glyph origin */
//    vgSetfv( VG_GLYPH_ORIGIN, 2, Origin ) ;
    
    /* Renders a sequence of glyphs specified by the array of the indices */
//    vgDrawGlyphs(  ChnFontHdl, ( VGint )( sizeof SampleChnText / sizeof( VGuint )),  SampleChnText, NULL, NULL, VG_FILL_PATH, VG_FALSE ) ;
    vgDrawGlyphs(  ChnFontHdl, ( VGint )1,  &SampleChnText[DSPCHN_STR[StrNo].Start_PTR + NumberChar], NULL, NULL, VG_FILL_PATH, VG_FALSE ) ;
 
    /* Destroy the paint objects */
    vgDestroyPaint( fill ) ;

    /* Destroys the font object */
     vgDestroyFont( ChnFontHdl ) ;
}






int RETW_HextoAscii(int Num)
{
			int Temp = Num&0x000f;
			if(Temp>9)
					return ((Temp-10)+ 0x61);
			else
					return (Temp+0x30);
}
int RETW_StringCompare( char *str1, char *str2, int length)
{
		int i,j;
		char *straddr1;
		char *straddr2;		
		
		char strone[16];
		char strtwo[16];		
	
		straddr1 = str1;
		straddr2 = str2;		
 		j=0;
 		
		for(i=0;i<length;i++)
				{
					strone[i]= *straddr1++;
					strtwo[i]= *straddr2++;
				}
		for(i=0;i<length;i++)
				{
					if(strone[i] == strtwo[i])
						j+=1;
				}

//		printf("j=%d,length is %d\n",j,length);

 		if(j==length)
 		return 0;
 		else 
 		{
 				#if 0
					straddr1 = str1;
					straddr2 = str2;	
 					printf("Compare source is ");
 					for(i=0;i<length;i++)
 					IoPutchar(strone[i]);
 
 					printf("  Input is ");
 					for(i=0;i<length;i++)
 					IoPutchar(strtwo[i]);
 					printf("\n");
 				#endif
 				
 			return 1;
 		}
 
}
 