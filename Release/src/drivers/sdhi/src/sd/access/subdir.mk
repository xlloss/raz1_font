################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
..\src/drivers/sdhi/src/sd/access/sd_cd.c \
..\src/drivers/sdhi/src/sd/access/sd_cmd.c \
..\src/drivers/sdhi/src/sd/access/sd_format.c \
..\src/drivers/sdhi/src/sd/access/sd_init.c \
..\src/drivers/sdhi/src/sd/access/sd_int.c \
..\src/drivers/sdhi/src/sd/access/sd_mount.c \
..\src/drivers/sdhi/src/sd/access/sd_read.c \
..\src/drivers/sdhi/src/sd/access/sd_trns.c \
..\src/drivers/sdhi/src/sd/access/sd_upcase_table.c \
..\src/drivers/sdhi/src/sd/access/sd_util.c \
..\src/drivers/sdhi/src/sd/access/sd_write.c 

C_DEPS += \
./src/drivers/sdhi/src/sd/access/sd_cd.d \
./src/drivers/sdhi/src/sd/access/sd_cmd.d \
./src/drivers/sdhi/src/sd/access/sd_format.d \
./src/drivers/sdhi/src/sd/access/sd_init.d \
./src/drivers/sdhi/src/sd/access/sd_int.d \
./src/drivers/sdhi/src/sd/access/sd_mount.d \
./src/drivers/sdhi/src/sd/access/sd_read.d \
./src/drivers/sdhi/src/sd/access/sd_trns.d \
./src/drivers/sdhi/src/sd/access/sd_upcase_table.d \
./src/drivers/sdhi/src/sd/access/sd_util.d \
./src/drivers/sdhi/src/sd/access/sd_write.d 

OBJS += \
./src/drivers/sdhi/src/sd/access/sd_cd.o \
./src/drivers/sdhi/src/sd/access/sd_cmd.o \
./src/drivers/sdhi/src/sd/access/sd_format.o \
./src/drivers/sdhi/src/sd/access/sd_init.o \
./src/drivers/sdhi/src/sd/access/sd_int.o \
./src/drivers/sdhi/src/sd/access/sd_mount.o \
./src/drivers/sdhi/src/sd/access/sd_read.o \
./src/drivers/sdhi/src/sd/access/sd_trns.o \
./src/drivers/sdhi/src/sd/access/sd_upcase_table.o \
./src/drivers/sdhi/src/sd/access/sd_util.o \
./src/drivers/sdhi/src/sd/access/sd_write.o 


# Each subdirectory must supply rules for building sources it contributes
src/drivers/sdhi/src/sd/access/%.o: ../src/drivers/sdhi/src/sd/access/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Compiler'
	@arm-none-eabi-gcc -Wa,-adlhn="$(basename $(notdir $<)).lst" @"src/drivers/sdhi/src/sd/access/c.sub" -o "$@" "$<"
	@echo arm-none-eabi-gcc -c -x c  -Wa,-adlhn="$(basename $(notdir $<)).lst" -nostdinc -I"C:\WorkSpace\RZ\RZ_A1H_OpenVG_RSK\src\MP3LIB\inc" -I"C:\Workspace\RZ\RZ_A1H_OpenVG_RSK\src\FreeType2\include" -I"C:\Workspace\RZ\RZ_A1H_OpenVG_RSK\src\FreeType2\include\freetype" -I"C:\Workspace\RZ\RZ_A1H_OpenVG_RSK\src\FreeType2\include\freetype\internal" -I"C:\Workspace\RZ\RZ_A1H_OpenVG_RSK\src\FreeType2\include\freetype\config" -I"C:\Workspace\RZ\RZ_A1H_OpenVG_RSK\src\FreeType2\include\freetype\internal\services" -I"C:\Workspace\RZ\RZ_A1H_OpenVG_RSK\src\drivers\sdhi\src\sd\inc\access" -I"C:\Workspace\RZ\RZ_A1H_OpenVG_RSK\src\drivers\sdhi\inc" -I"C:\Workspace\RZ\RZ_A1H_OpenVG_RSK\src\drivers\sdhi\src\sd\inc" -I"C:\WorkSpace\RZ\RZ_A1H_OpenVG_RSK\src\drivers\rspi\inc" -I"C:\WorkSpace\RZ\RZ_A1H_OpenVG_RSK\src\RETWapp\Audio\parser\MP3\inc" -I"C:\WorkSpace\RZ\RZ_A1H_OpenVG_RSK\src\RETWapp\Audio\parser\ID3\inc" -I"C:\WorkSpace\RZ\RZ_A1H_OpenVG_RSK\src\RETWapp\Audio\parser\inc\CMN" -I"C:\WorkSpace\RZ\RZ_A1H_OpenVG_RSK\src\RETWapp\Audio\parser\inc\ID3" -I"C:\WorkSpace\RZ\RZ_A1H_OpenVG_RSK\src\RETWapp\Audio\parser\inc\MP3" -I"C:\WorkSpace\RZ\RZ_A1H_OpenVG_RSK\src\RETWapp\Audio\audio\inc" -I"C:\WorkSpace\RZ\RZ_A1H_OpenVG_RSK\src\RETWapp\Audio\command_dec\inc" -I"C:\WorkSpace\RZ\RZ_A1H_OpenVG_RSK\src\RETWapp\Audio\command_scux\inc" -I"C:\WorkSpace\RZ\RZ_A1H_OpenVG_RSK\src\RETWapp\Audio\parser\inc" -I"C:\Workspace\RZ\RZ_A1H_OpenVG_RSK\src\drivers\riic\inc" -I"C:\Workspace\RZ\RZ_A1H_OpenVG_RSK\src\drivers\scux\inc" -I"C:\Workspace\RZ\RZ_A1H_OpenVG_RSK\src\drivers\ssif\inc" -I"C:\Workspace\RZ\RZ_A1H_OpenVG_RSK\inc" -I"C:\Workspace\RZ\RZ_A1H_OpenVG_RSK\bin" -I"C:\Workspace\RZ\RZ_A1H_OpenVG_RSK\inc\iodefines" -I"C:\Workspace\RZ\RZ_A1H_OpenVG_RSK\compiler_specific\inc" -I"C:\Workspace\RZ\RZ_A1H_OpenVG_RSK\inc\iobitmasks" -I"C:\Workspace\RZ\RZ_A1H_OpenVG_RSK\src\EGL" -I"C:\Workspace\RZ\RZ_A1H_OpenVG_RSK\src\EGL\inc" -I"C:\Workspace\RZ\RZ_A1H_OpenVG_RSK\src\OpenVG\inc" -I"C:\Workspace\RZ\RZ_A1H_OpenVG_RSK\src\OpenVG" -I"C:\Workspace\RZ\RZ_A1H_OpenVG_RSK\src\RGPNCG\inc" -I"C:\Workspace\RZ\RZ_A1H_OpenVG_RSK\src\RGPNCG" -I"C:\Workspace\RZ\RZ_A1H_OpenVG_RSK\src\drivers\vdc5\include" -I"C:\Workspace\RZ\RZ_A1H_OpenVG_RSK\src\drivers\vdec\include" -I"C:\Workspace\RZ\RZ_A1H_OpenVG_RSK\src\sample\graphics\inc" -I"C:\PROGRA~1\KPIT\GNUARM~1.01-\ARM-NO~1\ARM-NO~1/arm-none-eabi/optlibinc" -I"C:\PROGRA~1\KPIT\GNUARM~1.01-\ARM-NO~1\ARM-NO~1/bin/newlib/include/machine" -DRENESAS_NATIVE -DNCG_DEBUG_API_LOG -DNCG_DEBUG_MEMORY_ANALYZE_LOG -DNCG_DEBUG_1 -DNCG_DEBUG_MEMORY_ANALYZE_1 -g2 -g -mcpu=cortex-a9 -march=armv7-a -mlittle-endian -marm -mthumb-interwork -mfloat-abi=hard -munaligned-access -mfpu=vfp -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo.

