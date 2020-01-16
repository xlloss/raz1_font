################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_UPPER_SRCS += \
..\src/RETWHUD/GUICLOCK.C \
..\src/RETWHUD/GUIDashB0.C \
..\src/RETWHUD/GUIDashB4.C \
..\src/RETWHUD/GUIDashB5.C \
..\src/RETWHUD/GUIDemo0.C \
..\src/RETWHUD/GUIMETER.C \
..\src/RETWHUD/GUIMainUtility.C \
..\src/RETWHUD/GUIOpenMotion1280_BGR.C \
..\src/RETWHUD/GUIOpenMotion800x480.C 

C_SRCS += \
..\src/RETWHUD/GUIMainMenu.c 

C_UPPER_DEPS += \
./src/RETWHUD/GUICLOCK.d \
./src/RETWHUD/GUIDashB0.d \
./src/RETWHUD/GUIDashB4.d \
./src/RETWHUD/GUIDashB5.d \
./src/RETWHUD/GUIDemo0.d \
./src/RETWHUD/GUIMETER.d \
./src/RETWHUD/GUIMainUtility.d \
./src/RETWHUD/GUIOpenMotion1280_BGR.d \
./src/RETWHUD/GUIOpenMotion800x480.d 

C_DEPS += \
./src/RETWHUD/GUIMainMenu.d 

OBJS += \
./src/RETWHUD/GUICLOCK.o \
./src/RETWHUD/GUIDashB0.o \
./src/RETWHUD/GUIDashB4.o \
./src/RETWHUD/GUIDashB5.o \
./src/RETWHUD/GUIDemo0.o \
./src/RETWHUD/GUIMETER.o \
./src/RETWHUD/GUIMainMenu.o \
./src/RETWHUD/GUIMainUtility.o \
./src/RETWHUD/GUIOpenMotion1280_BGR.o \
./src/RETWHUD/GUIOpenMotion800x480.o 


# Each subdirectory must supply rules for building sources it contributes
src/RETWHUD/%.o: ../src/RETWHUD/%.C
	@echo 'Scanning and building file: $<'
	@echo 'Invoking: Scanner and Compiler'
	@arm-none-eabi-gcc -MM -MP -MF ""$(@:%.o=%.d)"" -MT"$(@:%.o=%.o)" -MT""$(@:%.o=%.d)"" @"src/RETWHUD/C.depsub" "$<"
	@echo	arm-none-eabi-gcc -MM -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -x c   -nostdinc -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\drivers\rspi\inc" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\drivers\rscan_rz\inc" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\drivers\riic\inc" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\drivers\scux\inc" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\drivers\ssif\inc" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\inc" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\bin" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\inc\iodefines" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\compiler_specific\inc" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\inc\iobitmasks" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\EGL" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\EGL\inc" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\OpenVG\inc" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\OpenVG" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\RGPNCG\inc" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\RGPNCG" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\drivers\vdc5\include" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\drivers\vdec\include" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\sample\graphics\inc" -I"C:\PROGRA~2\KPIT\GNUARM~1.01-\ARM-NO~1\ARM-NO~1/arm-none-eabi/optlibinc" -DRENESAS_NATIVE -DNCG_DEBUG_API_LOG -DNCG_DEBUG_MEMORY_ANALYZE_LOG -DNCG_DEBUG_1 -DNCG_DEBUG_MEMORY_ANALYZE_1 -g2 -g -mcpu=cortex-a9 -march=armv7-a -mlittle-endian -marm -mthumb-interwork -mfloat-abi=hard -mfpu=vfp "$<"
	@arm-none-eabi-gcc -Wa,-adlhn="$(basename $(notdir $<)).lst" @"src/RETWHUD/C.sub" -o "$(@:%.d=%.o)" "$<"
	@echo arm-none-eabi-gcc -c -x c  -Wa,-adlhn="$(basename $(notdir $<)).lst" -nostdinc -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\drivers\rspi\inc" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\drivers\rscan_rz\inc" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\drivers\riic\inc" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\drivers\scux\inc" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\drivers\ssif\inc" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\inc" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\bin" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\inc\iodefines" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\compiler_specific\inc" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\inc\iobitmasks" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\EGL" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\EGL\inc" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\OpenVG\inc" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\OpenVG" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\RGPNCG\inc" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\RGPNCG" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\drivers\vdc5\include" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\drivers\vdec\include" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\sample\graphics\inc" -I"C:\PROGRA~2\KPIT\GNUARM~1.01-\ARM-NO~1\ARM-NO~1/arm-none-eabi/optlibinc" -DRENESAS_NATIVE -DNCG_DEBUG_API_LOG -DNCG_DEBUG_MEMORY_ANALYZE_LOG -DNCG_DEBUG_1 -DNCG_DEBUG_MEMORY_ANALYZE_1 -g2 -g -mcpu=cortex-a9 -march=armv7-a -mlittle-endian -marm -mthumb-interwork -mfloat-abi=hard -mfpu=vfp -o "$(@:%.d=%.o)" "$<"
	@echo 'Finished scanning and building: $<'
	@echo.

src/RETWHUD/%.o: ../src/RETWHUD/%.c
	@echo 'Scanning and building file: $<'
	@echo 'Invoking: Scanner and Compiler'
	@arm-none-eabi-gcc -MM -MP -MF ""$(@:%.o=%.d)"" -MT"$(@:%.o=%.o)" -MT""$(@:%.o=%.d)"" @"src/RETWHUD/c.depsub" "$<"
	@echo	arm-none-eabi-gcc -MM -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -x c   -nostdinc -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\drivers\rspi\inc" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\drivers\rscan_rz\inc" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\drivers\riic\inc" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\drivers\scux\inc" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\drivers\ssif\inc" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\inc" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\bin" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\inc\iodefines" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\compiler_specific\inc" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\inc\iobitmasks" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\EGL" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\EGL\inc" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\OpenVG\inc" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\OpenVG" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\RGPNCG\inc" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\RGPNCG" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\drivers\vdc5\include" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\drivers\vdec\include" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\sample\graphics\inc" -I"C:\PROGRA~2\KPIT\GNUARM~1.01-\ARM-NO~1\ARM-NO~1/arm-none-eabi/optlibinc" -DRENESAS_NATIVE -DNCG_DEBUG_API_LOG -DNCG_DEBUG_MEMORY_ANALYZE_LOG -DNCG_DEBUG_1 -DNCG_DEBUG_MEMORY_ANALYZE_1 -g2 -g -mcpu=cortex-a9 -march=armv7-a -mlittle-endian -marm -mthumb-interwork -mfloat-abi=hard -mfpu=vfp "$<"
	@arm-none-eabi-gcc -Wa,-adlhn="$(basename $(notdir $<)).lst" @"src/RETWHUD/c.sub" -o "$(@:%.d=%.o)" "$<"
	@echo arm-none-eabi-gcc -c -x c  -Wa,-adlhn="$(basename $(notdir $<)).lst" -nostdinc -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\drivers\rspi\inc" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\drivers\rscan_rz\inc" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\drivers\riic\inc" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\drivers\scux\inc" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\drivers\ssif\inc" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\inc" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\bin" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\inc\iodefines" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\compiler_specific\inc" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\inc\iobitmasks" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\EGL" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\EGL\inc" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\OpenVG\inc" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\OpenVG" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\RGPNCG\inc" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\RGPNCG" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\drivers\vdc5\include" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\drivers\vdec\include" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\sample\graphics\inc" -I"C:\PROGRA~2\KPIT\GNUARM~1.01-\ARM-NO~1\ARM-NO~1/arm-none-eabi/optlibinc" -DRENESAS_NATIVE -DNCG_DEBUG_API_LOG -DNCG_DEBUG_MEMORY_ANALYZE_LOG -DNCG_DEBUG_1 -DNCG_DEBUG_MEMORY_ANALYZE_1 -g2 -g -mcpu=cortex-a9 -march=armv7-a -mlittle-endian -marm -mthumb-interwork -mfloat-abi=hard -mfpu=vfp -o "$(@:%.d=%.o)" "$<"
	@echo 'Finished scanning and building: $<'
	@echo.

src/RETWHUD/GUIOpenMotion1280_BGR.o: ../src/RETWHUD/GUIOpenMotion1280_BGR.C
	@echo 'Scanning and building file: $<'
	@echo 'Invoking: Scanner and Compiler'
	@arm-none-eabi-gcc -MM -MP -MF "src/RETWHUD/GUIOpenMotion1280_BGR.d" -MT"src/RETWHUD/GUIOpenMotion1280_BGR.o" -MT"src/RETWHUD/GUIOpenMotion1280_BGR.d" @"src/RETWHUD/GUIOpenMotion1280_BGR.depsub" "$<"
	@echo	arm-none-eabi-gcc -MM -MP -MF "src/RETWHUD/GUIOpenMotion1280_BGR.d" -MT"src/RETWHUD/GUIOpenMotion1280_BGR.o" -MT"src/RETWHUD/GUIOpenMotion1280_BGR.d" -x c   -nostdinc -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\drivers\rspi\inc" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\drivers\rscan_rz\inc" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\drivers\riic\inc" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\drivers\scux\inc" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\drivers\ssif\inc" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\inc" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\bin" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\inc\iodefines" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\compiler_specific\inc" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\inc\iobitmasks" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\EGL" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\EGL\inc" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\OpenVG\inc" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\OpenVG" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\RGPNCG\inc" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\RGPNCG" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\drivers\vdc5\include" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\drivers\vdec\include" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\sample\graphics\inc" -I"C:\PROGRA~2\KPIT\GNUARM~1.01-\ARM-NO~1\ARM-NO~1/arm-none-eabi/optlibinc" -DRENESAS_NATIVE -DNCG_DEBUG_API_LOG -DNCG_DEBUG_MEMORY_ANALYZE_LOG -DNCG_DEBUG_1 -DNCG_DEBUG_MEMORY_ANALYZE_1 -g2 -g -mcpu=cortex-a9 -march=armv7-a -mlittle-endian -marm -mthumb-interwork -mfloat-abi=hard -mfpu=vfp "$<"
	@arm-none-eabi-gcc -Wa,-adlhn="$(basename $(notdir $<)).lst" @"src/RETWHUD/GUIOpenMotion1280_BGR.sub" -o "$(@:%.d=%.o)" "$<"
	@echo arm-none-eabi-gcc -c -x c  -Wa,-adlhn="$(basename $(notdir $<)).lst" -nostdinc -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\drivers\rspi\inc" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\drivers\rscan_rz\inc" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\drivers\riic\inc" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\drivers\scux\inc" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\drivers\ssif\inc" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\inc" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\bin" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\inc\iodefines" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\compiler_specific\inc" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\inc\iobitmasks" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\EGL" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\EGL\inc" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\OpenVG\inc" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\OpenVG" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\RGPNCG\inc" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\RGPNCG" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\drivers\vdc5\include" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\drivers\vdec\include" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\sample\graphics\inc" -I"C:\PROGRA~2\KPIT\GNUARM~1.01-\ARM-NO~1\ARM-NO~1/arm-none-eabi/optlibinc" -DRENESAS_NATIVE -DNCG_DEBUG_API_LOG -DNCG_DEBUG_MEMORY_ANALYZE_LOG -DNCG_DEBUG_1 -DNCG_DEBUG_MEMORY_ANALYZE_1 -g2 -g -mcpu=cortex-a9 -march=armv7-a -mlittle-endian -marm -mthumb-interwork -mfloat-abi=hard -mfpu=vfp -o "$(@:%.d=%.o)" "$<"
	@echo 'Finished scanning and building: $<'
	@echo.

