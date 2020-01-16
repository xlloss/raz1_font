################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
SRC_SRCS += \
..\src/RETWFreeType/font_bin.src 

C_SRCS += \
..\src/RETWFreeType/cv_UTF8toUCS2.c \
..\src/RETWFreeType/cv_txtrender.c \
..\src/RETWFreeType/draw_font_vg_sample.c \
..\src/RETWFreeType/ft2_txtrender.c \
..\src/RETWFreeType/txtrender.c \
..\src/RETWFreeType/vg_txtrender.c 

C_DEPS += \
./src/RETWFreeType/cv_UTF8toUCS2.d \
./src/RETWFreeType/cv_txtrender.d \
./src/RETWFreeType/draw_font_vg_sample.d \
./src/RETWFreeType/ft2_txtrender.d \
./src/RETWFreeType/txtrender.d \
./src/RETWFreeType/vg_txtrender.d 

SRC_DEPS += \
./src/RETWFreeType/font_bin.d 

OBJS += \
./src/RETWFreeType/cv_UTF8toUCS2.o \
./src/RETWFreeType/cv_txtrender.o \
./src/RETWFreeType/draw_font_vg_sample.o \
./src/RETWFreeType/font_bin.o \
./src/RETWFreeType/ft2_txtrender.o \
./src/RETWFreeType/txtrender.o \
./src/RETWFreeType/vg_txtrender.o 


# Each subdirectory must supply rules for building sources it contributes
src/RETWFreeType/%.o: ../src/RETWFreeType/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Compiler'
	@arm-none-eabi-gcc -Wa,-adlhn="$(basename $(notdir $<)).lst" @"src/RETWFreeType/c.sub" -o "$@" "$<"
	@echo arm-none-eabi-gcc -c -x c  -Wa,-adlhn="$(basename $(notdir $<)).lst" -nostdinc -I"C:\WorkSpace\RZ\RZ_A1H_OpenVG_RSK\src\MP3LIB\inc" -I"C:\Workspace\RZ\RZ_A1H_OpenVG_RSK\src\FreeType2\include" -I"C:\Workspace\RZ\RZ_A1H_OpenVG_RSK\src\FreeType2\include\freetype" -I"C:\Workspace\RZ\RZ_A1H_OpenVG_RSK\src\FreeType2\include\freetype\internal" -I"C:\Workspace\RZ\RZ_A1H_OpenVG_RSK\src\FreeType2\include\freetype\config" -I"C:\Workspace\RZ\RZ_A1H_OpenVG_RSK\src\FreeType2\include\freetype\internal\services" -I"C:\Workspace\RZ\RZ_A1H_OpenVG_RSK\src\drivers\sdhi\src\sd\inc\access" -I"C:\Workspace\RZ\RZ_A1H_OpenVG_RSK\src\drivers\sdhi\inc" -I"C:\Workspace\RZ\RZ_A1H_OpenVG_RSK\src\drivers\sdhi\src\sd\inc" -I"C:\WorkSpace\RZ\RZ_A1H_OpenVG_RSK\src\drivers\rspi\inc" -I"C:\WorkSpace\RZ\RZ_A1H_OpenVG_RSK\src\RETWapp\Audio\parser\MP3\inc" -I"C:\WorkSpace\RZ\RZ_A1H_OpenVG_RSK\src\RETWapp\Audio\parser\ID3\inc" -I"C:\WorkSpace\RZ\RZ_A1H_OpenVG_RSK\src\RETWapp\Audio\parser\inc\CMN" -I"C:\WorkSpace\RZ\RZ_A1H_OpenVG_RSK\src\RETWapp\Audio\parser\inc\ID3" -I"C:\WorkSpace\RZ\RZ_A1H_OpenVG_RSK\src\RETWapp\Audio\parser\inc\MP3" -I"C:\WorkSpace\RZ\RZ_A1H_OpenVG_RSK\src\RETWapp\Audio\audio\inc" -I"C:\WorkSpace\RZ\RZ_A1H_OpenVG_RSK\src\RETWapp\Audio\command_dec\inc" -I"C:\WorkSpace\RZ\RZ_A1H_OpenVG_RSK\src\RETWapp\Audio\command_scux\inc" -I"C:\WorkSpace\RZ\RZ_A1H_OpenVG_RSK\src\RETWapp\Audio\parser\inc" -I"C:\Workspace\RZ\RZ_A1H_OpenVG_RSK\src\drivers\riic\inc" -I"C:\Workspace\RZ\RZ_A1H_OpenVG_RSK\src\drivers\scux\inc" -I"C:\Workspace\RZ\RZ_A1H_OpenVG_RSK\src\drivers\ssif\inc" -I"C:\Workspace\RZ\RZ_A1H_OpenVG_RSK\inc" -I"C:\Workspace\RZ\RZ_A1H_OpenVG_RSK\bin" -I"C:\Workspace\RZ\RZ_A1H_OpenVG_RSK\inc\iodefines" -I"C:\Workspace\RZ\RZ_A1H_OpenVG_RSK\compiler_specific\inc" -I"C:\Workspace\RZ\RZ_A1H_OpenVG_RSK\inc\iobitmasks" -I"C:\Workspace\RZ\RZ_A1H_OpenVG_RSK\src\EGL" -I"C:\Workspace\RZ\RZ_A1H_OpenVG_RSK\src\EGL\inc" -I"C:\Workspace\RZ\RZ_A1H_OpenVG_RSK\src\OpenVG\inc" -I"C:\Workspace\RZ\RZ_A1H_OpenVG_RSK\src\OpenVG" -I"C:\Workspace\RZ\RZ_A1H_OpenVG_RSK\src\RGPNCG\inc" -I"C:\Workspace\RZ\RZ_A1H_OpenVG_RSK\src\RGPNCG" -I"C:\Workspace\RZ\RZ_A1H_OpenVG_RSK\src\drivers\vdc5\include" -I"C:\Workspace\RZ\RZ_A1H_OpenVG_RSK\src\drivers\vdec\include" -I"C:\Workspace\RZ\RZ_A1H_OpenVG_RSK\src\sample\graphics\inc" -I"C:\PROGRA~1\KPIT\GNUARM~1.01-\ARM-NO~1\ARM-NO~1/arm-none-eabi/optlibinc" -I"C:\PROGRA~1\KPIT\GNUARM~1.01-\ARM-NO~1\ARM-NO~1/bin/newlib/include/machine" -DRENESAS_NATIVE -DNCG_DEBUG_API_LOG -DNCG_DEBUG_MEMORY_ANALYZE_LOG -DNCG_DEBUG_1 -DNCG_DEBUG_MEMORY_ANALYZE_1 -g2 -g -mcpu=cortex-a9 -march=armv7-a -mlittle-endian -marm -mthumb-interwork -mfloat-abi=hard -munaligned-access -mfpu=vfp -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo.

src/RETWFreeType/%.o: ../src/RETWFreeType/%.src
	@echo 'Building file: $<'
	@echo 'Invoking: Assembler'
	@arm-none-eabi-as -adlhn="$(basename $(notdir $<)).lis" @"src/RETWFreeType/src.sub" -o"$@" "$<"
	@echo arm-none-eabi-as -I"C:\Workspace\RZ\RZ_A1H_OpenVG_RSK/src" -I"C:\Workspace\RZ\RZ_A1H_OpenVG_RSK\bin" -adlhn="$(basename $(notdir $<)).lis" --gdwarf2 -mlittle-endian -mcpu=cortex-a9 -march=armv7-a -mfloat-abi=hard -mfpu=vfp -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo.

