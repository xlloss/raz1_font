################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
..\src/RGPNCG/src/ncg_debug.c \
..\src/RGPNCG/src/ncg_du.c \
..\src/RGPNCG/src/ncg_du_isr.c \
..\src/RGPNCG/src/ncg_ioctl.c \
..\src/RGPNCG/src/ncg_memory.c \
..\src/RGPNCG/src/ncg_mutex.c \
..\src/RGPNCG/src/ncg_queue.c \
..\src/RGPNCG/src/ncg_register.c \
..\src/RGPNCG/src/ncg_state.c \
..\src/RGPNCG/src/ncg_thread.c \
..\src/RGPNCG/src/ncg_time.c \
..\src/RGPNCG/src/ncg_vg.c \
..\src/RGPNCG/src/ncg_vg_isr.c 

C_DEPS += \
./src/RGPNCG/src/ncg_debug.d \
./src/RGPNCG/src/ncg_du.d \
./src/RGPNCG/src/ncg_du_isr.d \
./src/RGPNCG/src/ncg_ioctl.d \
./src/RGPNCG/src/ncg_memory.d \
./src/RGPNCG/src/ncg_mutex.d \
./src/RGPNCG/src/ncg_queue.d \
./src/RGPNCG/src/ncg_register.d \
./src/RGPNCG/src/ncg_state.d \
./src/RGPNCG/src/ncg_thread.d \
./src/RGPNCG/src/ncg_time.d \
./src/RGPNCG/src/ncg_vg.d \
./src/RGPNCG/src/ncg_vg_isr.d 

OBJS += \
./src/RGPNCG/src/ncg_debug.o \
./src/RGPNCG/src/ncg_du.o \
./src/RGPNCG/src/ncg_du_isr.o \
./src/RGPNCG/src/ncg_ioctl.o \
./src/RGPNCG/src/ncg_memory.o \
./src/RGPNCG/src/ncg_mutex.o \
./src/RGPNCG/src/ncg_queue.o \
./src/RGPNCG/src/ncg_register.o \
./src/RGPNCG/src/ncg_state.o \
./src/RGPNCG/src/ncg_thread.o \
./src/RGPNCG/src/ncg_time.o \
./src/RGPNCG/src/ncg_vg.o \
./src/RGPNCG/src/ncg_vg_isr.o 


# Each subdirectory must supply rules for building sources it contributes
src/RGPNCG/src/%.o: ../src/RGPNCG/src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Compiler'
	@arm-none-eabi-gcc -Wa,-adlhn="$(basename $(notdir $<)).lst" @"src/RGPNCG/src/c.sub" -o "$@" "$<"
	@echo arm-none-eabi-gcc -c -x c  -Wa,-adlhn="$(basename $(notdir $<)).lst" -nostdinc -I"C:\WorkSpace\RZ\RZ_A1H_OpenVG_RSK\src\MP3LIB\inc" -I"C:\Workspace\RZ\RZ_A1H_OpenVG_RSK\src\FreeType2\include" -I"C:\Workspace\RZ\RZ_A1H_OpenVG_RSK\src\FreeType2\include\freetype" -I"C:\Workspace\RZ\RZ_A1H_OpenVG_RSK\src\FreeType2\include\freetype\internal" -I"C:\Workspace\RZ\RZ_A1H_OpenVG_RSK\src\FreeType2\include\freetype\config" -I"C:\Workspace\RZ\RZ_A1H_OpenVG_RSK\src\FreeType2\include\freetype\internal\services" -I"C:\Workspace\RZ\RZ_A1H_OpenVG_RSK\src\drivers\sdhi\src\sd\inc\access" -I"C:\Workspace\RZ\RZ_A1H_OpenVG_RSK\src\drivers\sdhi\inc" -I"C:\Workspace\RZ\RZ_A1H_OpenVG_RSK\src\drivers\sdhi\src\sd\inc" -I"C:\WorkSpace\RZ\RZ_A1H_OpenVG_RSK\src\drivers\rspi\inc" -I"C:\WorkSpace\RZ\RZ_A1H_OpenVG_RSK\src\RETWapp\Audio\parser\MP3\inc" -I"C:\WorkSpace\RZ\RZ_A1H_OpenVG_RSK\src\RETWapp\Audio\parser\ID3\inc" -I"C:\WorkSpace\RZ\RZ_A1H_OpenVG_RSK\src\RETWapp\Audio\parser\inc\CMN" -I"C:\WorkSpace\RZ\RZ_A1H_OpenVG_RSK\src\RETWapp\Audio\parser\inc\ID3" -I"C:\WorkSpace\RZ\RZ_A1H_OpenVG_RSK\src\RETWapp\Audio\parser\inc\MP3" -I"C:\WorkSpace\RZ\RZ_A1H_OpenVG_RSK\src\RETWapp\Audio\audio\inc" -I"C:\WorkSpace\RZ\RZ_A1H_OpenVG_RSK\src\RETWapp\Audio\command_dec\inc" -I"C:\WorkSpace\RZ\RZ_A1H_OpenVG_RSK\src\RETWapp\Audio\command_scux\inc" -I"C:\WorkSpace\RZ\RZ_A1H_OpenVG_RSK\src\RETWapp\Audio\parser\inc" -I"C:\Workspace\RZ\RZ_A1H_OpenVG_RSK\src\drivers\riic\inc" -I"C:\Workspace\RZ\RZ_A1H_OpenVG_RSK\src\drivers\scux\inc" -I"C:\Workspace\RZ\RZ_A1H_OpenVG_RSK\src\drivers\ssif\inc" -I"C:\Workspace\RZ\RZ_A1H_OpenVG_RSK\inc" -I"C:\Workspace\RZ\RZ_A1H_OpenVG_RSK\bin" -I"C:\Workspace\RZ\RZ_A1H_OpenVG_RSK\inc\iodefines" -I"C:\Workspace\RZ\RZ_A1H_OpenVG_RSK\compiler_specific\inc" -I"C:\Workspace\RZ\RZ_A1H_OpenVG_RSK\inc\iobitmasks" -I"C:\Workspace\RZ\RZ_A1H_OpenVG_RSK\src\EGL" -I"C:\Workspace\RZ\RZ_A1H_OpenVG_RSK\src\EGL\inc" -I"C:\Workspace\RZ\RZ_A1H_OpenVG_RSK\src\OpenVG\inc" -I"C:\Workspace\RZ\RZ_A1H_OpenVG_RSK\src\OpenVG" -I"C:\Workspace\RZ\RZ_A1H_OpenVG_RSK\src\RGPNCG\inc" -I"C:\Workspace\RZ\RZ_A1H_OpenVG_RSK\src\RGPNCG" -I"C:\Workspace\RZ\RZ_A1H_OpenVG_RSK\src\drivers\vdc5\include" -I"C:\Workspace\RZ\RZ_A1H_OpenVG_RSK\src\drivers\vdec\include" -I"C:\Workspace\RZ\RZ_A1H_OpenVG_RSK\src\sample\graphics\inc" -I"C:\PROGRA~1\KPIT\GNUARM~1.01-\ARM-NO~1\ARM-NO~1/arm-none-eabi/optlibinc" -I"C:\PROGRA~1\KPIT\GNUARM~1.01-\ARM-NO~1\ARM-NO~1/bin/newlib/include/machine" -DRENESAS_NATIVE -DNCG_DEBUG_API_LOG -DNCG_DEBUG_MEMORY_ANALYZE_LOG -DNCG_DEBUG_1 -DNCG_DEBUG_MEMORY_ANALYZE_1 -g2 -g -mcpu=cortex-a9 -march=armv7-a -mlittle-endian -marm -mthumb-interwork -mfloat-abi=hard -munaligned-access -mfpu=vfp -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo.

