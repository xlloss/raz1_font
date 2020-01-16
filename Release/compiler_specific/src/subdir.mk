################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
..\compiler_specific/src/gnu_io.c \
..\compiler_specific/src/mbed_sf_boot.c \
..\compiler_specific/src/resetprg.c \
..\compiler_specific/src/top_of_heap.c 

C_DEPS += \
./compiler_specific/src/gnu_io.d \
./compiler_specific/src/mbed_sf_boot.d \
./compiler_specific/src/resetprg.d \
./compiler_specific/src/top_of_heap.d 

OBJS += \
./compiler_specific/src/gnu_io.o \
./compiler_specific/src/mbed_sf_boot.o \
./compiler_specific/src/resetprg.o \
./compiler_specific/src/top_of_heap.o 


# Each subdirectory must supply rules for building sources it contributes
compiler_specific/src/%.o: ../compiler_specific/src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Compiler'
	@arm-none-eabi-gcc -Wa,-adlhn="$(basename $(notdir $<)).lst" @"compiler_specific/src/c.sub" -o "$@" "$<"
	@echo arm-none-eabi-gcc -c -x c  -Wa,-adlhn="$(basename $(notdir $<)).lst" -nostdinc -I"C:\WorkSpace\RZ\RZ_A1H_OpenVG_RSK\src\MP3LIB\inc" -I"C:\Workspace\RZ\RZ_A1H_OpenVG_RSK\src\FreeType2\include" -I"C:\Workspace\RZ\RZ_A1H_OpenVG_RSK\src\FreeType2\include\freetype" -I"C:\Workspace\RZ\RZ_A1H_OpenVG_RSK\src\FreeType2\include\freetype\internal" -I"C:\Workspace\RZ\RZ_A1H_OpenVG_RSK\src\FreeType2\include\freetype\config" -I"C:\Workspace\RZ\RZ_A1H_OpenVG_RSK\src\FreeType2\include\freetype\internal\services" -I"C:\Workspace\RZ\RZ_A1H_OpenVG_RSK\src\drivers\sdhi\src\sd\inc\access" -I"C:\Workspace\RZ\RZ_A1H_OpenVG_RSK\src\drivers\sdhi\inc" -I"C:\Workspace\RZ\RZ_A1H_OpenVG_RSK\src\drivers\sdhi\src\sd\inc" -I"C:\WorkSpace\RZ\RZ_A1H_OpenVG_RSK\src\drivers\rspi\inc" -I"C:\WorkSpace\RZ\RZ_A1H_OpenVG_RSK\src\RETWapp\Audio\parser\MP3\inc" -I"C:\WorkSpace\RZ\RZ_A1H_OpenVG_RSK\src\RETWapp\Audio\parser\ID3\inc" -I"C:\WorkSpace\RZ\RZ_A1H_OpenVG_RSK\src\RETWapp\Audio\parser\inc\CMN" -I"C:\WorkSpace\RZ\RZ_A1H_OpenVG_RSK\src\RETWapp\Audio\parser\inc\ID3" -I"C:\WorkSpace\RZ\RZ_A1H_OpenVG_RSK\src\RETWapp\Audio\parser\inc\MP3" -I"C:\WorkSpace\RZ\RZ_A1H_OpenVG_RSK\src\RETWapp\Audio\audio\inc" -I"C:\WorkSpace\RZ\RZ_A1H_OpenVG_RSK\src\RETWapp\Audio\command_dec\inc" -I"C:\WorkSpace\RZ\RZ_A1H_OpenVG_RSK\src\RETWapp\Audio\command_scux\inc" -I"C:\WorkSpace\RZ\RZ_A1H_OpenVG_RSK\src\RETWapp\Audio\parser\inc" -I"C:\Workspace\RZ\RZ_A1H_OpenVG_RSK\src\drivers\riic\inc" -I"C:\Workspace\RZ\RZ_A1H_OpenVG_RSK\src\drivers\scux\inc" -I"C:\Workspace\RZ\RZ_A1H_OpenVG_RSK\src\drivers\ssif\inc" -I"C:\Workspace\RZ\RZ_A1H_OpenVG_RSK\inc" -I"C:\Workspace\RZ\RZ_A1H_OpenVG_RSK\bin" -I"C:\Workspace\RZ\RZ_A1H_OpenVG_RSK\inc\iodefines" -I"C:\Workspace\RZ\RZ_A1H_OpenVG_RSK\compiler_specific\inc" -I"C:\Workspace\RZ\RZ_A1H_OpenVG_RSK\inc\iobitmasks" -I"C:\Workspace\RZ\RZ_A1H_OpenVG_RSK\src\EGL" -I"C:\Workspace\RZ\RZ_A1H_OpenVG_RSK\src\EGL\inc" -I"C:\Workspace\RZ\RZ_A1H_OpenVG_RSK\src\OpenVG\inc" -I"C:\Workspace\RZ\RZ_A1H_OpenVG_RSK\src\OpenVG" -I"C:\Workspace\RZ\RZ_A1H_OpenVG_RSK\src\RGPNCG\inc" -I"C:\Workspace\RZ\RZ_A1H_OpenVG_RSK\src\RGPNCG" -I"C:\Workspace\RZ\RZ_A1H_OpenVG_RSK\src\drivers\vdc5\include" -I"C:\Workspace\RZ\RZ_A1H_OpenVG_RSK\src\drivers\vdec\include" -I"C:\Workspace\RZ\RZ_A1H_OpenVG_RSK\src\sample\graphics\inc" -I"C:\PROGRA~1\KPIT\GNUARM~1.01-\ARM-NO~1\ARM-NO~1/arm-none-eabi/optlibinc" -I"C:\PROGRA~1\KPIT\GNUARM~1.01-\ARM-NO~1\ARM-NO~1/bin/newlib/include/machine" -DRENESAS_NATIVE -DNCG_DEBUG_API_LOG -DNCG_DEBUG_MEMORY_ANALYZE_LOG -DNCG_DEBUG_1 -DNCG_DEBUG_MEMORY_ANALYZE_1 -g2 -g -mcpu=cortex-a9 -march=armv7-a -mlittle-endian -marm -mthumb-interwork -mfloat-abi=hard -munaligned-access -mfpu=vfp -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo.

