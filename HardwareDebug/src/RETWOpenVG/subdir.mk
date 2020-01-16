################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
..\src/RETWOpenVG/ColorPaint.c \
..\src/RETWOpenVG/DrawPicture.c \
..\src/RETWOpenVG/DrawText.c \
..\src/RETWOpenVG/Ellipses.c \
..\src/RETWOpenVG/EllipticalArcs.c \
..\src/RETWOpenVG/Graphiclib.c \
..\src/RETWOpenVG/ImageFilters.c \
..\src/RETWOpenVG/Lines.c \
..\src/RETWOpenVG/MaskingImage.c \
..\src/RETWOpenVG/PoliconFill.c \
..\src/RETWOpenVG/RadialGradientPaint.c \
..\src/RETWOpenVG/RenderingQuality.c \
..\src/RETWOpenVG/StraightLines.c \
..\src/RETWOpenVG/Stroke.c \
..\src/RETWOpenVG/TextImage.c 

C_DEPS += \
./src/RETWOpenVG/ColorPaint.d \
./src/RETWOpenVG/DrawPicture.d \
./src/RETWOpenVG/DrawText.d \
./src/RETWOpenVG/Ellipses.d \
./src/RETWOpenVG/EllipticalArcs.d \
./src/RETWOpenVG/Graphiclib.d \
./src/RETWOpenVG/ImageFilters.d \
./src/RETWOpenVG/Lines.d \
./src/RETWOpenVG/MaskingImage.d \
./src/RETWOpenVG/PoliconFill.d \
./src/RETWOpenVG/RadialGradientPaint.d \
./src/RETWOpenVG/RenderingQuality.d \
./src/RETWOpenVG/StraightLines.d \
./src/RETWOpenVG/Stroke.d \
./src/RETWOpenVG/TextImage.d 

OBJS += \
./src/RETWOpenVG/ColorPaint.o \
./src/RETWOpenVG/DrawPicture.o \
./src/RETWOpenVG/DrawText.o \
./src/RETWOpenVG/Ellipses.o \
./src/RETWOpenVG/EllipticalArcs.o \
./src/RETWOpenVG/Graphiclib.o \
./src/RETWOpenVG/ImageFilters.o \
./src/RETWOpenVG/Lines.o \
./src/RETWOpenVG/MaskingImage.o \
./src/RETWOpenVG/PoliconFill.o \
./src/RETWOpenVG/RadialGradientPaint.o \
./src/RETWOpenVG/RenderingQuality.o \
./src/RETWOpenVG/StraightLines.o \
./src/RETWOpenVG/Stroke.o \
./src/RETWOpenVG/TextImage.o 


# Each subdirectory must supply rules for building sources it contributes
src/RETWOpenVG/%.o: ../src/RETWOpenVG/%.c
	@echo 'Scanning and building file: $<'
	@echo 'Invoking: Scanner and Compiler'
	@arm-none-eabi-gcc -MM -MP -MF ""$(@:%.o=%.d)"" -MT"$(@:%.o=%.o)" -MT""$(@:%.o=%.d)"" @"src/RETWOpenVG/c.depsub" "$<"
	@echo	arm-none-eabi-gcc -MM -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -x c   -nostdinc -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\drivers\rspi\inc" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\drivers\rscan_rz\inc" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\drivers\riic\inc" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\drivers\scux\inc" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\drivers\ssif\inc" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\inc" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\bin" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\inc\iodefines" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\compiler_specific\inc" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\inc\iobitmasks" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\EGL" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\EGL\inc" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\OpenVG\inc" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\OpenVG" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\RGPNCG\inc" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\RGPNCG" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\drivers\vdc5\include" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\drivers\vdec\include" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\sample\graphics\inc" -I"C:\PROGRA~2\KPIT\GNUARM~1.01-\ARM-NO~1\ARM-NO~1/arm-none-eabi/optlibinc" -DRENESAS_NATIVE -DNCG_DEBUG_API_LOG -DNCG_DEBUG_MEMORY_ANALYZE_LOG -DNCG_DEBUG_1 -DNCG_DEBUG_MEMORY_ANALYZE_1 -g2 -g -mcpu=cortex-a9 -march=armv7-a -mlittle-endian -marm -mthumb-interwork -mfloat-abi=hard -mfpu=vfp "$<"
	@arm-none-eabi-gcc -Wa,-adlhn="$(basename $(notdir $<)).lst" @"src/RETWOpenVG/c.sub" -o "$(@:%.d=%.o)" "$<"
	@echo arm-none-eabi-gcc -c -x c  -Wa,-adlhn="$(basename $(notdir $<)).lst" -nostdinc -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\drivers\rspi\inc" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\drivers\rscan_rz\inc" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\drivers\riic\inc" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\drivers\scux\inc" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\drivers\ssif\inc" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\inc" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\bin" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\inc\iodefines" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\compiler_specific\inc" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\inc\iobitmasks" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\EGL" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\EGL\inc" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\OpenVG\inc" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\OpenVG" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\RGPNCG\inc" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\RGPNCG" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\drivers\vdc5\include" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\drivers\vdec\include" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\sample\graphics\inc" -I"C:\PROGRA~2\KPIT\GNUARM~1.01-\ARM-NO~1\ARM-NO~1/arm-none-eabi/optlibinc" -DRENESAS_NATIVE -DNCG_DEBUG_API_LOG -DNCG_DEBUG_MEMORY_ANALYZE_LOG -DNCG_DEBUG_1 -DNCG_DEBUG_MEMORY_ANALYZE_1 -g2 -g -mcpu=cortex-a9 -march=armv7-a -mlittle-endian -marm -mthumb-interwork -mfloat-abi=hard -mfpu=vfp -o "$(@:%.d=%.o)" "$<"
	@echo 'Finished scanning and building: $<'
	@echo.

