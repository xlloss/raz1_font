################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
..\src/RETWOpenVG/text/Arialbd_font_24pt_PathData.c \
..\src/RETWOpenVG/text/ChnSVGFontParsing.c \
..\src/RETWOpenVG/text/r_text_renderer.c 

C_DEPS += \
./src/RETWOpenVG/text/Arialbd_font_24pt_PathData.d \
./src/RETWOpenVG/text/ChnSVGFontParsing.d \
./src/RETWOpenVG/text/r_text_renderer.d 

OBJS += \
./src/RETWOpenVG/text/Arialbd_font_24pt_PathData.o \
./src/RETWOpenVG/text/ChnSVGFontParsing.o \
./src/RETWOpenVG/text/r_text_renderer.o 


# Each subdirectory must supply rules for building sources it contributes
src/RETWOpenVG/text/Arialbd_font_24pt_PathData.o: ../src/RETWOpenVG/text/Arialbd_font_24pt_PathData.c
	@echo 'Scanning and building file: $<'
	@echo 'Invoking: Scanner and Compiler'
	@arm-none-eabi-gcc -MM -MP -MF "src/RETWOpenVG/text/Arialbd_font_24pt_PathData.d" -MT"src/RETWOpenVG/text/Arialbd_font_24pt_PathData.o" -MT"src/RETWOpenVG/text/Arialbd_font_24pt_PathData.d" @"src/RETWOpenVG/text/Arialbd_font_24pt_PathData.depsub" "$<"
	@echo	arm-none-eabi-gcc -MM -MP -MF "src/RETWOpenVG/text/Arialbd_font_24pt_PathData.d" -MT"src/RETWOpenVG/text/Arialbd_font_24pt_PathData.o" -MT"src/RETWOpenVG/text/Arialbd_font_24pt_PathData.d" -x c   -nostdinc -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\drivers\rspi\inc" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\drivers\rscan_rz\inc" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\drivers\riic\inc" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\drivers\scux\inc" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\drivers\ssif\inc" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\inc" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\bin" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\inc\iodefines" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\compiler_specific\inc" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\inc\iobitmasks" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\EGL" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\EGL\inc" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\OpenVG\inc" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\OpenVG" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\RGPNCG\inc" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\RGPNCG" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\drivers\vdc5\include" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\drivers\vdec\include" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\sample\graphics\inc" -I"C:\PROGRA~2\KPIT\GNUARM~1.01-\ARM-NO~1\ARM-NO~1/arm-none-eabi/optlibinc" -DRENESAS_NATIVE -DNCG_DEBUG_API_LOG -DNCG_DEBUG_MEMORY_ANALYZE_LOG -DNCG_DEBUG_1 -DNCG_DEBUG_MEMORY_ANALYZE_1 -g2 -g -mcpu=cortex-a9 -march=armv7-a -mlittle-endian -marm -mthumb-interwork -mfloat-abi=hard -mfpu=vfp "$<"
	@arm-none-eabi-gcc -Wa,-adlhn="$(basename $(notdir $<)).lst" @"src/RETWOpenVG/text/Arialbd_font_24pt_PathData.sub" -o "$(@:%.d=%.o)" "$<"
	@echo arm-none-eabi-gcc -c -x c  -Wa,-adlhn="$(basename $(notdir $<)).lst" -nostdinc -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\drivers\rspi\inc" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\drivers\rscan_rz\inc" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\drivers\riic\inc" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\drivers\scux\inc" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\drivers\ssif\inc" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\inc" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\bin" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\inc\iodefines" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\compiler_specific\inc" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\inc\iobitmasks" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\EGL" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\EGL\inc" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\OpenVG\inc" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\OpenVG" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\RGPNCG\inc" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\RGPNCG" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\drivers\vdc5\include" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\drivers\vdec\include" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\sample\graphics\inc" -I"C:\PROGRA~2\KPIT\GNUARM~1.01-\ARM-NO~1\ARM-NO~1/arm-none-eabi/optlibinc" -DRENESAS_NATIVE -DNCG_DEBUG_API_LOG -DNCG_DEBUG_MEMORY_ANALYZE_LOG -DNCG_DEBUG_1 -DNCG_DEBUG_MEMORY_ANALYZE_1 -g2 -g -mcpu=cortex-a9 -march=armv7-a -mlittle-endian -marm -mthumb-interwork -mfloat-abi=hard -mfpu=vfp -o "$(@:%.d=%.o)" "$<"
	@echo 'Finished scanning and building: $<'
	@echo.

src/RETWOpenVG/text/%.o: ../src/RETWOpenVG/text/%.c
	@echo 'Scanning and building file: $<'
	@echo 'Invoking: Scanner and Compiler'
	@arm-none-eabi-gcc -MM -MP -MF ""$(@:%.o=%.d)"" -MT"$(@:%.o=%.o)" -MT""$(@:%.o=%.d)"" @"src/RETWOpenVG/text/c.depsub" "$<"
	@echo	arm-none-eabi-gcc -MM -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -x c   -nostdinc -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\drivers\rspi\inc" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\drivers\rscan_rz\inc" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\drivers\riic\inc" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\drivers\scux\inc" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\drivers\ssif\inc" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\inc" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\bin" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\inc\iodefines" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\compiler_specific\inc" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\inc\iobitmasks" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\EGL" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\EGL\inc" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\OpenVG\inc" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\OpenVG" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\RGPNCG\inc" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\RGPNCG" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\drivers\vdc5\include" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\drivers\vdec\include" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\sample\graphics\inc" -I"C:\PROGRA~2\KPIT\GNUARM~1.01-\ARM-NO~1\ARM-NO~1/arm-none-eabi/optlibinc" -DRENESAS_NATIVE -DNCG_DEBUG_API_LOG -DNCG_DEBUG_MEMORY_ANALYZE_LOG -DNCG_DEBUG_1 -DNCG_DEBUG_MEMORY_ANALYZE_1 -g2 -g -mcpu=cortex-a9 -march=armv7-a -mlittle-endian -marm -mthumb-interwork -mfloat-abi=hard -mfpu=vfp "$<"
	@arm-none-eabi-gcc -Wa,-adlhn="$(basename $(notdir $<)).lst" @"src/RETWOpenVG/text/c.sub" -o "$(@:%.d=%.o)" "$<"
	@echo arm-none-eabi-gcc -c -x c  -Wa,-adlhn="$(basename $(notdir $<)).lst" -nostdinc -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\drivers\rspi\inc" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\drivers\rscan_rz\inc" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\drivers\riic\inc" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\drivers\scux\inc" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\drivers\ssif\inc" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\inc" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\bin" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\inc\iodefines" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\compiler_specific\inc" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\inc\iobitmasks" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\EGL" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\EGL\inc" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\OpenVG\inc" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\OpenVG" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\RGPNCG\inc" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\RGPNCG" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\drivers\vdc5\include" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\drivers\vdec\include" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\sample\graphics\inc" -I"C:\PROGRA~2\KPIT\GNUARM~1.01-\ARM-NO~1\ARM-NO~1/arm-none-eabi/optlibinc" -DRENESAS_NATIVE -DNCG_DEBUG_API_LOG -DNCG_DEBUG_MEMORY_ANALYZE_LOG -DNCG_DEBUG_1 -DNCG_DEBUG_MEMORY_ANALYZE_1 -g2 -g -mcpu=cortex-a9 -march=armv7-a -mlittle-endian -marm -mthumb-interwork -mfloat-abi=hard -mfpu=vfp -o "$(@:%.d=%.o)" "$<"
	@echo 'Finished scanning and building: $<'
	@echo.

src/RETWOpenVG/text/r_text_renderer.o: ../src/RETWOpenVG/text/r_text_renderer.c
	@echo 'Scanning and building file: $<'
	@echo 'Invoking: Scanner and Compiler'
	@arm-none-eabi-gcc -MM -MP -MF "src/RETWOpenVG/text/r_text_renderer.d" -MT"src/RETWOpenVG/text/r_text_renderer.o" -MT"src/RETWOpenVG/text/r_text_renderer.d" @"src/RETWOpenVG/text/r_text_renderer.depsub" "$<"
	@echo	arm-none-eabi-gcc -MM -MP -MF "src/RETWOpenVG/text/r_text_renderer.d" -MT"src/RETWOpenVG/text/r_text_renderer.o" -MT"src/RETWOpenVG/text/r_text_renderer.d" -x c   -nostdinc -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\drivers\rspi\inc" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\drivers\rscan_rz\inc" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\drivers\riic\inc" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\drivers\scux\inc" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\drivers\ssif\inc" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\inc" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\bin" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\inc\iodefines" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\compiler_specific\inc" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\inc\iobitmasks" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\EGL" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\EGL\inc" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\OpenVG\inc" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\OpenVG" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\RGPNCG\inc" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\RGPNCG" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\drivers\vdc5\include" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\drivers\vdec\include" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\sample\graphics\inc" -I"C:\PROGRA~2\KPIT\GNUARM~1.01-\ARM-NO~1\ARM-NO~1/arm-none-eabi/optlibinc" -DRENESAS_NATIVE -DNCG_DEBUG_API_LOG -DNCG_DEBUG_MEMORY_ANALYZE_LOG -DNCG_DEBUG_1 -DNCG_DEBUG_MEMORY_ANALYZE_1 -g2 -g -mcpu=cortex-a9 -march=armv7-a -mlittle-endian -marm -mthumb-interwork -mfloat-abi=hard -mfpu=vfp "$<"
	@arm-none-eabi-gcc -Wa,-adlhn="$(basename $(notdir $<)).lst" @"src/RETWOpenVG/text/r_text_renderer.sub" -o "$(@:%.d=%.o)" "$<"
	@echo arm-none-eabi-gcc -c -x c  -Wa,-adlhn="$(basename $(notdir $<)).lst" -nostdinc -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\drivers\rspi\inc" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\drivers\rscan_rz\inc" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\drivers\riic\inc" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\drivers\scux\inc" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\drivers\ssif\inc" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\inc" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\bin" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\inc\iodefines" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\compiler_specific\inc" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\inc\iobitmasks" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\EGL" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\EGL\inc" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\OpenVG\inc" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\OpenVG" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\RGPNCG\inc" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\RGPNCG" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\drivers\vdc5\include" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\drivers\vdec\include" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\sample\graphics\inc" -I"C:\PROGRA~2\KPIT\GNUARM~1.01-\ARM-NO~1\ARM-NO~1/arm-none-eabi/optlibinc" -DRENESAS_NATIVE -DNCG_DEBUG_API_LOG -DNCG_DEBUG_MEMORY_ANALYZE_LOG -DNCG_DEBUG_1 -DNCG_DEBUG_MEMORY_ANALYZE_1 -g2 -g -mcpu=cortex-a9 -march=armv7-a -mlittle-endian -marm -mthumb-interwork -mfloat-abi=hard -mfpu=vfp -o "$(@:%.d=%.o)" "$<"
	@echo 'Finished scanning and building: $<'
	@echo.

