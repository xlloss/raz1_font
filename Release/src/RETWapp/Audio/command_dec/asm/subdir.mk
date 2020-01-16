################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_SRCS += \
..\src/RETWapp/Audio/command_dec/asm/sample_TestFile.s 

S_DEPS += \
./src/RETWapp/Audio/command_dec/asm/sample_TestFile.d 

OBJS += \
./src/RETWapp/Audio/command_dec/asm/sample_TestFile.o 


# Each subdirectory must supply rules for building sources it contributes
src/RETWapp/Audio/command_dec/asm/%.o: ../src/RETWapp/Audio/command_dec/asm/%.s
	@echo 'Building file: $<'
	@echo 'Invoking: Assembler'
	@arm-none-eabi-as -adlhn="$(basename $(notdir $<)).lis" @"src/RETWapp/Audio/command_dec/asm/s.sub" -o"$@" "$<"
	@echo arm-none-eabi-as -I"C:\Workspace\RZ\RZ_A1H_OpenVG_RSK/src" -I"C:\Workspace\RZ\RZ_A1H_OpenVG_RSK\bin" -adlhn="$(basename $(notdir $<)).lis" --gdwarf2 -mlittle-endian -mcpu=cortex-a9 -march=armv7-a -mfloat-abi=hard -mfpu=vfp -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo.

