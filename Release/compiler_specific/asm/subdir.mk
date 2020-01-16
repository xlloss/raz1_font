################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_UPPER_SRCS += \
..\compiler_specific/asm/access.S \
..\compiler_specific/asm/helper_asm.S \
..\compiler_specific/asm/initsct.S \
..\compiler_specific/asm/irqfiq_handler.S \
..\compiler_specific/asm/isr.S \
..\compiler_specific/asm/l1_cache_init.S \
..\compiler_specific/asm/reset_handler.S \
..\compiler_specific/asm/stack.S \
..\compiler_specific/asm/start.S \
..\compiler_specific/asm/ttb_init.S \
..\compiler_specific/asm/user_prog_jmp.S 

SRC_SRCS += \
..\compiler_specific/asm/binary.src 

SRC_DEPS += \
./compiler_specific/asm/binary.d 

OBJS += \
./compiler_specific/asm/access.o \
./compiler_specific/asm/binary.o \
./compiler_specific/asm/helper_asm.o \
./compiler_specific/asm/initsct.o \
./compiler_specific/asm/irqfiq_handler.o \
./compiler_specific/asm/isr.o \
./compiler_specific/asm/l1_cache_init.o \
./compiler_specific/asm/reset_handler.o \
./compiler_specific/asm/stack.o \
./compiler_specific/asm/start.o \
./compiler_specific/asm/ttb_init.o \
./compiler_specific/asm/user_prog_jmp.o 

S_UPPER_DEPS += \
./compiler_specific/asm/access.d \
./compiler_specific/asm/helper_asm.d \
./compiler_specific/asm/initsct.d \
./compiler_specific/asm/irqfiq_handler.d \
./compiler_specific/asm/isr.d \
./compiler_specific/asm/l1_cache_init.d \
./compiler_specific/asm/reset_handler.d \
./compiler_specific/asm/stack.d \
./compiler_specific/asm/start.d \
./compiler_specific/asm/ttb_init.d \
./compiler_specific/asm/user_prog_jmp.d 


# Each subdirectory must supply rules for building sources it contributes
compiler_specific/asm/%.o: ../compiler_specific/asm/%.S
	@echo 'Building file: $<'
	@echo 'Invoking: Compiler'
	@arm-none-eabi-gcc -Wa,-adlhn="$(basename $(notdir $<)).lst" @"compiler_specific/asm/S.sub" -o "$@" "$<"
	@echo arm-none-eabi-gcc -Wa,-gdwarf2 -c -x assembler-with-cpp  -Wa,-adlhn="$(basename $(notdir $<)).lst" -nostdinc -I"C:\WorkSpace\RZ\RZ_A1H_OpenVG_RSK\src\MP3LIB\inc" -I"C:\Workspace\RZ\RZ_A1H_OpenVG_RSK\src\FreeType2\include" -I"C:\Workspace\RZ\RZ_A1H_OpenVG_RSK\src\FreeType2\include\freetype" -I"C:\Workspace\RZ\RZ_A1H_OpenVG_RSK\src\FreeType2\include\freetype\internal" -I"C:\Workspace\RZ\RZ_A1H_OpenVG_RSK\src\FreeType2\include\freetype\config" -I"C:\Workspace\RZ\RZ_A1H_OpenVG_RSK\src\FreeType2\include\freetype\internal\services" -I"C:\Workspace\RZ\RZ_A1H_OpenVG_RSK\src\drivers\sdhi\src\sd\inc\access" -I"C:\Workspace\RZ\RZ_A1H_OpenVG_RSK\src\drivers\sdhi\inc" -I"C:\Workspace\RZ\RZ_A1H_OpenVG_RSK\src\drivers\sdhi\src\sd\inc" -I"C:\WorkSpace\RZ\RZ_A1H_OpenVG_RSK\src\drivers\rspi\inc" -I"C:\WorkSpace\RZ\RZ_A1H_OpenVG_RSK\src\RETWapp\Audio\parser\MP3\inc" -I"C:\WorkSpace\RZ\RZ_A1H_OpenVG_RSK\src\RETWapp\Audio\parser\ID3\inc" -I"C:\WorkSpace\RZ\RZ_A1H_OpenVG_RSK\src\RETWapp\Audio\parser\inc\CMN" -I"C:\WorkSpace\RZ\RZ_A1H_OpenVG_RSK\src\RETWapp\Audio\parser\inc\ID3" -I"C:\WorkSpace\RZ\RZ_A1H_OpenVG_RSK\src\RETWapp\Audio\parser\inc\MP3" -I"C:\WorkSpace\RZ\RZ_A1H_OpenVG_RSK\src\RETWapp\Audio\audio\inc" -I"C:\WorkSpace\RZ\RZ_A1H_OpenVG_RSK\src\RETWapp\Audio\command_dec\inc" -I"C:\WorkSpace\RZ\RZ_A1H_OpenVG_RSK\src\RETWapp\Audio\command_scux\inc" -I"C:\WorkSpace\RZ\RZ_A1H_OpenVG_RSK\src\RETWapp\Audio\parser\inc" -I"C:\Workspace\RZ\RZ_A1H_OpenVG_RSK\src\drivers\riic\inc" -I"C:\Workspace\RZ\RZ_A1H_OpenVG_RSK\src\drivers\scux\inc" -I"C:\Workspace\RZ\RZ_A1H_OpenVG_RSK\src\drivers\ssif\inc" -I"C:\Workspace\RZ\RZ_A1H_OpenVG_RSK\inc" -I"C:\Workspace\RZ\RZ_A1H_OpenVG_RSK\bin" -I"C:\Workspace\RZ\RZ_A1H_OpenVG_RSK\inc\iodefines" -I"C:\Workspace\RZ\RZ_A1H_OpenVG_RSK\compiler_specific\inc" -I"C:\Workspace\RZ\RZ_A1H_OpenVG_RSK\inc\iobitmasks" -I"C:\Workspace\RZ\RZ_A1H_OpenVG_RSK\src\EGL" -I"C:\Workspace\RZ\RZ_A1H_OpenVG_RSK\src\EGL\inc" -I"C:\Workspace\RZ\RZ_A1H_OpenVG_RSK\src\OpenVG\inc" -I"C:\Workspace\RZ\RZ_A1H_OpenVG_RSK\src\OpenVG" -I"C:\Workspace\RZ\RZ_A1H_OpenVG_RSK\src\RGPNCG\inc" -I"C:\Workspace\RZ\RZ_A1H_OpenVG_RSK\src\RGPNCG" -I"C:\Workspace\RZ\RZ_A1H_OpenVG_RSK\src\drivers\vdc5\include" -I"C:\Workspace\RZ\RZ_A1H_OpenVG_RSK\src\drivers\vdec\include" -I"C:\Workspace\RZ\RZ_A1H_OpenVG_RSK\src\sample\graphics\inc" -I"C:\PROGRA~1\KPIT\GNUARM~1.01-\ARM-NO~1\ARM-NO~1/arm-none-eabi/optlibinc" -I"C:\PROGRA~1\KPIT\GNUARM~1.01-\ARM-NO~1\ARM-NO~1/bin/newlib/include/machine" -DRENESAS_NATIVE -DNCG_DEBUG_API_LOG -DNCG_DEBUG_MEMORY_ANALYZE_LOG -DNCG_DEBUG_1 -DNCG_DEBUG_MEMORY_ANALYZE_1 -g2 -g -mcpu=cortex-a9 -march=armv7-a -mlittle-endian -marm -mthumb-interwork -mfloat-abi=hard -munaligned-access -mfpu=vfp -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo.

compiler_specific/asm/%.o: ../compiler_specific/asm/%.src
	@echo 'Building file: $<'
	@echo 'Invoking: Assembler'
	@arm-none-eabi-as -adlhn="$(basename $(notdir $<)).lis" @"compiler_specific/asm/src.sub" -o"$@" "$<"
	@echo arm-none-eabi-as -I"C:\Workspace\RZ\RZ_A1H_OpenVG_RSK/src" -I"C:\Workspace\RZ\RZ_A1H_OpenVG_RSK\bin" -adlhn="$(basename $(notdir $<)).lis" --gdwarf2 -mlittle-endian -mcpu=cortex-a9 -march=armv7-a -mfloat-abi=hard -mfpu=vfp -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo.

