

#define BPP565		0
#define BPP888		1

#define sysBPP		BPP888

#define TRUE 			1
#define FALSE 		0
#define REPLY 		1
#define NOREPLY 	0
#define success 		1
#define fail 	0

#define FontSizeX		8
#define FontSizeY		16
 

#define cRED	1
#define cGREEN	2
#define cBLUE	3
#define cWHITE	7

#define	WHITE	(0xFFFF)
#define	RED	(0xF800)
#define	GREEN	(0x07E0)
#define	BLUE	(0x001F)
#define	BLACK	(0x0000)
#define	YELLOW	(0xFFE0)
#define	CYAN	(0x07FF)
#define	MAGENTA	(0xF81F)


#define CharColorBLACK	0x0000
#define CharColorBLUE	0x001f
#define CharColorGREEN	0x07e0
#define CharColorRED	0xf800
#define CharColorYELLOW	0xffe0               
#define CharColorGB	0x07ff
#define CharColorRG	0xffe0
#define CharColorRB	0xf81f
#define CharColorWHITE	0xffff

#define ColorBLUE6bit	0x0000003f
#define ColorGREEN6bit	0x00000FC0
#define ColorRED6bit	0x0003f000
#define ColorBLACK6bit	0
#define ColorGB6bit	ColorBLUE6bit|ColorGREEN6bit
#define ColorRG6bit	ColorRED6bit|ColorGREEN6bit
#define ColorRB6bit	ColorRED6bit|ColorBLUE6bit
#define ColorWHITE6bit	ColorRED6bit|ColorGREEN6bit|ColorBLUE6bit

#define CharColorGRAYWHITE 0xc618

#define CharColorGRAY	0x8000 | 0x0400 | 0x0010
 
#define TRANSPARENT	0x8000
#define KEEPSAME	0xc000


 
