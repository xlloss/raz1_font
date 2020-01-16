################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
..\compiler_specific/src/gnu_io.c \
..\compiler_specific/src/resetprg.c \
..\compiler_specific/src/top_of_heap.c 

C_DEPS += \
./compiler_specific/src/gnu_io.d \
./compiler_specific/src/resetprg.d \
./compiler_specific/src/top_of_heap.d 

OBJS += \
./compiler_specific/src/gnu_io.o \
./compiler_specific/src/resetprg.o \
./compiler_specific/src/top_of_heap.o 


# Each subdirectory must supply rules for building sources it contributes
compiler_specific/src/gnu_io.o: ../compiler_specific/src/gnu_io.c
	@echo 'Scanning and building file: $<'
	@echo 'Invoking: Scanner and Compiler'
	@arm-none-eabi-gcc -MM -MP -MF "compiler_specific/src/gnu_io.d" -MT"compiler_specific/src/gnu_io.o" -MT"compiler_specific/src/gnu_io.d" @"compiler_specific/src/gnu_io.depsub" "$<"
	@echo	arm-none-eabi-gcc -MM -MP -MF "compiler_specific/src/gnu_io.d" -MT"compiler_specific/src/gnu_io.o" -MT"compiler_specific/src/gnu_io.d" -x c   -nostdinc -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\drivers\rspi\inc" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\drivers\rscan_rz\inc" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\drivers\riic\inc" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\drivers\scux\inc" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\drivers\ssif\inc" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\inc" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\bin" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\inc\iodefines" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\compiler_specific\inc" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\inc\iobitmasks" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\EGL" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\EGL\inc" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\OpenVG\inc" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\OpenVG" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\RGPNCG\inc" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\RGPNCG" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\drivers\vdc5\include" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\drivers\vdec\include" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\sample\graphics\inc" -I"C:\PROGRA~2\KPIT\GNUARM~1.01-\ARM-NO~1\ARM-NO~1/arm-none-eabi/optlibinc" -DRENESAS_NATIVE -DNCG_DEBUG_API_LOG -DNCG_DEBUG_MEMORY_ANALYZE_LOG -DNCG_DEBUG_1 -DNCG_DEBUG_MEMORY_ANALYZE_1 -g2 -g -mcpu=cortex-a9 -march=armv7-a -mlittle-endian -marm -mthumb-interwork -mfloat-abi=hard -mfpu=vfp "$<"
	@arm-none-eabi-gcc -Wa,-adlhn="$(basename $(notdir $<)).lst" @"compiler_specific/src/gnu_io.sub" -o "$(@:%.d=%.o)" "$<"
	@echo arm-none-eabi-gcc -c -x c  -Wa,-adlhn="$(basename $(notdir $<)).lst" -nostdinc -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\drivers\rspi\inc" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\drivers\rscan_rz\inc" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\drivers\riic\inc" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\drivers\scux\inc" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\drivers\ssif\inc" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\inc" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\bin" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\inc\iodefines" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\compiler_specific\inc" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\inc\iobitmasks" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\EGL" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\EGL\inc" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\OpenVG\inc" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\OpenVG" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\RGPNCG\inc" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\RGPNCG" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\drivers\vdc5\include" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\drivers\vdec\include" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\sample\graphics\inc" -I"C:\PROGRA~2\KPIT\GNUARM~1.01-\ARM-NO~1\ARM-NO~1/arm-none-eabi/optlibinc" -DRENESAS_NATIVE -DNCG_DEBUG_API_LOG -DNCG_DEBUG_MEMORY_ANALYZE_LOG -DNCG_DEBUG_1 -DNCG_DEBUG_MEMORY_ANALYZE_1 -g2 -g -mcpu=cortex-a9 -march=armv7-a -mlittle-endian -marm -mthumb-interwork -mfloat-abi=hard -mfpu=vfp -o "$(@:%.d=%.o)" "$<"
	@echo 'Finished scanning and building: $<'
	@echo.

compiler_specific/src/resetprg.o: ../compiler_specific/src/resetprg.c
	@echo 'Scanning and building file: $<'
	@echo 'Invoking: Scanner and Compiler'
	@arm-none-eabi-gcc -MM -MP -MF "compiler_specific/src/resetprg.d" -MT"compiler_specific/src/resetprg.o" -MT"compiler_specific/src/resetprg.d" @"compiler_specific/src/resetprg.depsub" "$<"
	@echo	arm-none-eabi-gcc -MM -MP -MF "compiler_specific/src/resetprg.d" -MT"compiler_specific/src/resetprg.o" -MT"compiler_specific/src/resetprg.d" -x c   -nostdinc -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\drivers\rspi\inc" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\drivers\rscan_rz\inc" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\drivers\riic\inc" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\drivers\scux\inc" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\drivers\ssif\inc" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\inc" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\bin" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\inc\iodefines" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\compiler_specific\inc" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\inc\iobitmasks" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\EGL" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\EGL\inc" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\OpenVG\inc" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\OpenVG" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\RGPNCG\inc" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\RGPNCG" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\drivers\vdc5\include" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\drivers\vdec\include" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\sample\graphics\inc" -I"C:\PROGRA~2\KPIT\GNUARM~1.01-\ARM-NO~1\ARM-NO~1/arm-none-eabi/optlibinc" -DRENESAS_NATIVE -DNCG_DEBUG_API_LOG -DNCG_DEBUG_MEMORY_ANALYZE_LOG -DNCG_DEBUG_1 -DNCG_DEBUG_MEMORY_ANALYZE_1 -g2 -g -mcpu=cortex-a9 -march=armv7-a -mlittle-endian -marm -mthumb-interwork -mfloat-abi=hard -mfpu=vfp "$<"
	@arm-none-eabi-gcc -Wa,-adlhn="$(basename $(notdir $<)).lst" @"compiler_specific/src/resetprg.sub" -o "$(@:%.d=%.o)" "$<"
	@echo arm-none-eabi-gcc -c -x c  -Wa,-adlhn="$(basename $(notdir $<)).lst" -nostdinc -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\drivers\rspi\inc" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\drivers\rscan_rz\inc" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\drivers\riic\inc" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\drivers\scux\inc" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\drivers\ssif\inc" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\inc" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\bin" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\inc\iodefines" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\compiler_specific\inc" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\inc\iobitmasks" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\EGL" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\EGL\inc" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\OpenVG\inc" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\OpenVG" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\RGPNCG\inc" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\RGPNCG" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\drivers\vdc5\include" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\drivers\vdec\include" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\sample\graphics\inc" -I"C:\PROGRA~2\KPIT\GNUARM~1.01-\ARM-NO~1\ARM-NO~1/arm-none-eabi/optlibinc" -DRENESAS_NATIVE -DNCG_DEBUG_API_LOG -DNCG_DEBUG_MEMORY_ANALYZE_LOG -DNCG_DEBUG_1 -DNCG_DEBUG_MEMORY_ANALYZE_1 -g2 -g -mcpu=cortex-a9 -march=armv7-a -mlittle-endian -marm -mthumb-interwork -mfloat-abi=hard -mfpu=vfp -o "$(@:%.d=%.o)" "$<"
	@echo 'Finished scanning and building: $<'
	@echo.

compiler_specific/src/top_of_heap.o: ../compiler_specific/src/top_of_heap.c
	@echo 'Scanning and building file: $<'
	@echo 'Invoking: Scanner and Compiler'
	@arm-none-eabi-gcc -MM -MP -MF "compiler_specific/src/top_of_heap.d" -MT"compiler_specific/src/top_of_heap.o" -MT"compiler_specific/src/top_of_heap.d" @"compiler_specific/src/top_of_heap.depsub" "$<"
	@echo	arm-none-eabi-gcc -MM -MP -MF "compiler_specific/src/top_of_heap.d" -MT"compiler_specific/src/top_of_heap.o" -MT"compiler_specific/src/top_of_heap.d" -x c   -nostdinc -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\drivers\rspi\inc" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\drivers\rscan_rz\inc" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\drivers\riic\inc" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\drivers\scux\inc" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\drivers\ssif\inc" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\inc" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\bin" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\inc\iodefines" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\compiler_specific\inc" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\inc\iobitmasks" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\EGL" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\EGL\inc" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\OpenVG\inc" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\OpenVG" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\RGPNCG\inc" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\RGPNCG" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\drivers\vdc5\include" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\drivers\vdec\include" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\sample\graphics\inc" -I"C:\PROGRA~2\KPIT\GNUARM~1.01-\ARM-NO~1\ARM-NO~1/arm-none-eabi/optlibinc" -DRENESAS_NATIVE -DNCG_DEBUG_API_LOG -DNCG_DEBUG_MEMORY_ANALYZE_LOG -DNCG_DEBUG_1 -DNCG_DEBUG_MEMORY_ANALYZE_1 -g2 -g -mcpu=cortex-a9 -march=armv7-a -mlittle-endian -marm -mthumb-interwork -mfloat-abi=hard -mfpu=vfp "$<"
	@arm-none-eabi-gcc -Wa,-adlhn="$(basename $(notdir $<)).lst" @"compiler_specific/src/top_of_heap.sub" -o "$(@:%.d=%.o)" "$<"
	@echo arm-none-eabi-gcc -c -x c  -Wa,-adlhn="$(basename $(notdir $<)).lst" -nostdinc -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\drivers\rspi\inc" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\drivers\rscan_rz\inc" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\drivers\riic\inc" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\drivers\scux\inc" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\drivers\ssif\inc" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\inc" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\bin" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\inc\iodefines" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\compiler_specific\inc" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\inc\iobitmasks" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\EGL" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\EGL\inc" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\OpenVG\inc" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\OpenVG" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\RGPNCG\inc" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\RGPNCG" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\drivers\vdc5\include" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\drivers\vdec\include" -I"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\src\sample\graphics\inc" -I"C:\PROGRA~2\KPIT\GNUARM~1.01-\ARM-NO~1\ARM-NO~1/arm-none-eabi/optlibinc" -DRENESAS_NATIVE -DNCG_DEBUG_API_LOG -DNCG_DEBUG_MEMORY_ANALYZE_LOG -DNCG_DEBUG_1 -DNCG_DEBUG_MEMORY_ANALYZE_1 -g2 -g -mcpu=cortex-a9 -march=armv7-a -mlittle-endian -marm -mthumb-interwork -mfloat-abi=hard -mfpu=vfp -o "$(@:%.d=%.o)" "$<"
	@echo 'Finished scanning and building: $<'
	@echo.

