
#include    <stdio.h>
#include    <string.h>
#include    <math.h>
#include    "r_typedefs.h"

#include    "VG/openvg.h"
#include    "VG/vgu.h"
#include 		"rtc.h"
 
#include 		"RETWvg.h"

#define PI 3.14159265

#define CIRCLE_DEG 2880



extern const unsigned int SINtable[];
extern const unsigned int COStable[];

extern int GlobalSecDeg;
extern int GlobalMinDeg;
extern int GlobalHourDeg;
extern int GlobalSecDeg_HRS;
extern int GlobalMinDeg_HRS;
extern int GlobalHourDeg_HRS;

extern st_RTCTIME	SYSTEMTIME;

extern VGint ClockShiftX;
extern VGint ClockShiftDir;

extern VGint ClockCenterX;
extern VGint ClockCenterY;

extern VGint CoordXSrc;
extern VGint CoordYSrc;
extern VGint CoordXDest;
extern VGint CoordYDest;
 
extern int Polycon_DrawPointNum;
extern VGfloat Polycon_DataTab[];
extern VGfloat Polycon_linearGradient[]; 
extern VGfloat  Polycon_RampStopTab[];
extern VGfloat Polycon_RadGradient[] ;      /* Center (x, y), Focal (x, y), Radius r */

extern VGfloat rampStops[];
extern VGfloat linearGradient[];

extern uint8_t rtc_halfsecond_flag;
extern uint8_t rtc_halfsecond_flag_CNT;
extern uint32_t Milisec_ModeEntry;
extern uint32_t Milisec_SWMeterEntry;
extern uint32_t Milisec_SignFlashCount;
extern uint32_t Milisec_VirtualEngineTickCount;
extern uint32_t Milisec_IndicatorMotion;
extern uint32_t FrameCountPs;
extern uint32_t FrameRate;
extern uint32_t EngineRPM;

extern VGfloat 	G_CharScale;
extern VGfloat 	G_CharShearX;
extern VGfloat 	G_CharShearY;
 
extern uint32_t totalframecnt;

extern uint8_t _RLOGO_ADDR[];
extern uint32_t VG_Back_FrameBuffer;

extern uint32_t ModeEntryCnt;

extern uint8_t _RZ1_ADDR[];
extern uint8_t _RZ2_ADDR[];
extern uint8_t _RZ3_ADDR[];
extern uint8_t _RZ4_ADDR[];
extern uint8_t _RZ5_ADDR[];
extern uint8_t _RZ6_ADDR[];
extern uint8_t _RUS_ADDR[];
extern uint8_t _RZS_ADDR[];

extern st_VG_ObjProperty 		MeterObj[];
extern st_VirtualEngine_Info	VirtualEngine;

extern uint32_t	CharPosOffsetX;
extern uint32_t	CharPosOffsetY;	

extern st_ClockIndiSet ClockIndicator;