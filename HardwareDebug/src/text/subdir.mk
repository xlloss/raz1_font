################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
..\src/text/Font_24pt_PathData.c \
..\src/text/Font_36pt_PathData.c \
..\src/text/Font_72pt_PathData.c \
..\src/text/r_text_renderer.c 

C_DEPS += \
./src/text/Font_24pt_PathData.d \
./src/text/Font_36pt_PathData.d \
./src/text/Font_72pt_PathData.d \
./src/text/r_text_renderer.d 

OBJS += \
./src/text/Font_24pt_PathData.o \
./src/text/Font_36pt_PathData.o \
./src/text/Font_72pt_PathData.o \
./src/text/r_text_renderer.o 


# Each subdirectory must supply rules for building sources it contributes
src/text/%.o: ../src/text/%.c
	@echo 'Scanning and building file: $<'
	@echo 'Invoking: Scanner and Compiler'
	@arm-none-eabi-gcc -MM -MP -MF ""$(@:%.o=%.d)"" -MT"$(@:%.o=%.o)" -MT""$(@:%.o=%.d)"" @"src/text/c.depsub" "$<"
	@echo	arm-none-eabi-gcc -MM -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -x c   -nostdinc -I"C:\PROGRA~1\KPIT\GNUARM~1.02-\ARM-NO~1\ARM-NO~1/arm-none-eabi/include" -I"C:\PROGRA~1\KPIT\GNUARM~1.02-\ARM-NO~1\ARM-NO~1/lib/gcc/arm-none-eabi/4.9-GNUARM-NONE_v14.02/include" -I"C:\PROGRA~1\KPIT\GNUARM~1.02-\ARM-NO~1\ARM-NO~1/arm-none-eabi/optlibinc" -I"C:\Workspace\RZ\RZ_A1H_OpenVG_RSK\inc" -I"C:\Workspace\RZ\RZ_A1H_OpenVG_RSK\bin" -I"C:\Workspace\RZ\RZ_A1H_OpenVG_RSK\inc\iodefines" -I"C:\Workspace\RZ\RZ_A1H_OpenVG_RSK\compiler_specific\inc" -I"C:\Workspace\RZ\RZ_A1H_OpenVG_RSK\inc\iobitmasks" -I"C:\Workspace\RZ\RZ_A1H_OpenVG_RSK\src\EGL" -I"C:\Workspace\RZ\RZ_A1H_OpenVG_RSK\src\EGL\inc" -I"C:\Workspace\RZ\RZ_A1H_OpenVG_RSK\src\OpenVG\inc" -I"C:\Workspace\RZ\RZ_A1H_OpenVG_RSK\src\OpenVG" -I"C:\Workspace\RZ\RZ_A1H_OpenVG_RSK\src\RGPNCG\inc" -I"C:\Workspace\RZ\RZ_A1H_OpenVG_RSK\src\RGPNCG" -I"C:\Workspace\RZ\RZ_A1H_OpenVG_RSK\src\drivers\vdc5\include" -I"C:\Workspace\RZ\RZ_A1H_OpenVG_RSK\src\drivers\vdec\include" -I"C:\Workspace\RZ\RZ_A1H_OpenVG_RSK\src\sample\graphics\inc" -DRENESAS_NATIVE -DNCG_DEBUG_API_LOG -DNCG_DEBUG_MEMORY_ANALYZE_LOG -DNCG_DEBUG_1 -DNCG_DEBUG_MEMORY_ANALYZE_1 -g2 -g -mcpu=cortex-a9 -march=armv7-a -mlittle-endian -marm -mthumb-interwork -mfloat-abi=hard -munaligned-access -mfpu=vfp -fshort-wchar -mfpu=neon-fp16 "$<"
	@arm-none-eabi-gcc -Wa,-adlhn="$(basename $(notdir $<)).lst" @"src/text/c.sub" -o "$(@:%.d=%.o)" "$<"
	@echo arm-none-eabi-gcc -c -x c  -Wa,-adlhn="$(basename $(notdir $<)).lst" -nostdinc -I"C:\PROGRA~1\KPIT\GNUARM~1.02-\ARM-NO~1\ARM-NO~1/arm-none-eabi/include" -I"C:\PROGRA~1\KPIT\GNUARM~1.02-\ARM-NO~1\ARM-NO~1/lib/gcc/arm-none-eabi/4.9-GNUARM-NONE_v14.02/include" -I"C:\PROGRA~1\KPIT\GNUARM~1.02-\ARM-NO~1\ARM-NO~1/arm-none-eabi/optlibinc" -I"C:\Workspace\RZ\RZ_A1H_OpenVG_RSK\inc" -I"C:\Workspace\RZ\RZ_A1H_OpenVG_RSK\bin" -I"C:\Workspace\RZ\RZ_A1H_OpenVG_RSK\inc\iodefines" -I"C:\Workspace\RZ\RZ_A1H_OpenVG_RSK\compiler_specific\inc" -I"C:\Workspace\RZ\RZ_A1H_OpenVG_RSK\inc\iobitmasks" -I"C:\Workspace\RZ\RZ_A1H_OpenVG_RSK\src\EGL" -I"C:\Workspace\RZ\RZ_A1H_OpenVG_RSK\src\EGL\inc" -I"C:\Workspace\RZ\RZ_A1H_OpenVG_RSK\src\OpenVG\inc" -I"C:\Workspace\RZ\RZ_A1H_OpenVG_RSK\src\OpenVG" -I"C:\Workspace\RZ\RZ_A1H_OpenVG_RSK\src\RGPNCG\inc" -I"C:\Workspace\RZ\RZ_A1H_OpenVG_RSK\src\RGPNCG" -I"C:\Workspace\RZ\RZ_A1H_OpenVG_RSK\src\drivers\vdc5\include" -I"C:\Workspace\RZ\RZ_A1H_OpenVG_RSK\src\drivers\vdec\include" -I"C:\Workspace\RZ\RZ_A1H_OpenVG_RSK\src\sample\graphics\inc" -DRENESAS_NATIVE -DNCG_DEBUG_API_LOG -DNCG_DEBUG_MEMORY_ANALYZE_LOG -DNCG_DEBUG_1 -DNCG_DEBUG_MEMORY_ANALYZE_1 -g2 -g -mcpu=cortex-a9 -march=armv7-a -mlittle-endian -marm -mthumb-interwork -mfloat-abi=hard -munaligned-access -mfpu=vfp -fshort-wchar -mfpu=neon-fp16 -o "$(@:%.d=%.o)" "$<"
	@echo 'Finished scanning and building: $<'
	@echo.

