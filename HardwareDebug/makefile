################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf *.lst *.lis *.lpp *.map RZ_A1H_OpenVG_RSK_HardwareDebug_auto.gsi

# All of the sources participating in the build are defined here
-include sources.mk
-include compiler_specific/asm/subdir.mk
-include compiler_specific/src/subdir.mk
-include src/RETWHUD/subdir.mk
-include src/RETWOpenVG/subdir.mk
-include src/RETWOpenVG/text/subdir.mk
-include src/RETWapp/subdir.mk
-include src/RETWgraphics/subdir.mk
-include src/RGPNCG/src/subdir.mk
-include src/board_settings/subdir.mk
-include src/common/subdir.mk
-include src/drivers/adc/subdir.mk
-include src/drivers/common/common_driver/subdir.mk
-include src/drivers/common/userdef/subdir.mk
-include src/drivers/intc/intc_driver/subdir.mk
-include src/drivers/intc/userdef/subdir.mk
-include src/drivers/keyscanuart/subdir.mk
-include src/drivers/ostm/subdir.mk
-include src/drivers/riic/riic_driver/subdir.mk
-include src/drivers/riic/userdef/subdir.mk
-include src/drivers/rscan_rz/src/subdir.mk
-include src/drivers/rspi/rspi_driver/subdir.mk
-include src/drivers/rspi/userdef/subdir.mk
-include src/drivers/rtc/subdir.mk
-include src/drivers/scif_uart/scif_uart_driver/subdir.mk
-include src/drivers/scif_uart/userdef/subdir.mk
-include src/drivers/scux/scux_driver/subdir.mk
-include src/drivers/ssif/src/subdir.mk
-include src/drivers/vdc5/src/subdir.mk
-include src/drivers/vdec/src/subdir.mk
-include src/subdir.mk
-include src/sample/graphics/common/subdir.mk
-include src/sample/graphics/common/lcd_panel/subdir.mk
-include src/sample/graphics/subdir.mk
-include src/sample/graphics/rendering/subdir.mk
-include src/sample/graphics/video_decoder/subdir.mk
-include src/sample/sample_vdc5/subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(CC_UPPER_DEPS)),)
-include $(CC_UPPER_DEPS)
endif
ifneq ($(strip $(C++_UPPER_DEPS)),)
-include $(C++_UPPER_DEPS)
endif
ifneq ($(strip $(CXX_UPPER_DEPS)),)
-include $(CXX_UPPER_DEPS)
endif
ifneq ($(strip $(CPP_UPPER_DEPS)),)
-include $(CPP_UPPER_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(SRC_UPPER_DEPS)),)
-include $(SRC_UPPER_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(SRC_DEPS)),)
-include $(SRC_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
LIBRARY_GENERATOR_OUTPUTTYPE_OUTPUTS += \
libRZ_A1H_OpenVG_RSK.a \

LINKER_OUTPUT_OUTPUTS += \
RZ_A1H_OpenVG_RSK.x \


# All Target
# Main-build Target
all: RZ_A1H_OpenVG_RSK.mot
	@echo 'Build complete.'

# Tool invocations
RZ_A1H_OpenVG_RSK.mot: $(LINKER_OUTPUT_OUTPUTS)
	@echo 'Invoking: Objcopy'
	@echo 'Building target:'
	arm-none-eabi-objcopy -O srec  $(LINKER_OUTPUT_OUTPUTS)"RZ_A1H_OpenVG_RSK.mot"
	@echo 'Finished building target:'
	@echo.
	$(MAKE) --no-print-directory post-build

libRZ_A1H_OpenVG_RSK.a: 
	@echo 'Invoking: Library Generator'
	@echo 'Building target:'
	arm-none-eabi-libgen --select-lib=optlib --header-files=all --compiler-options=-mcpu=cortex-a9,-march=armv7-a,-mlittle-endian,-mthumb-interwork,-mfloat-abi=hard,-mfpu=vfp,-Os,-ffunction-sections,-fdata-sections,-fno-function-cse,-fshort-wchar,-funsigned-char,-funit-at-a-time,-falign-jumps --assembler-options=-mcpu=cortex-a9,-march=armv7-a,-mlittle-endian,-mfloat-abi=hard,-mfpu=vfp -o "libRZ_A1H_OpenVG_RSK.a"
	@echo 'Finished building:'
	@echo.

RZ_A1H_OpenVG_RSK.x: $(OBJS) $(LIBRARY_GENERATOR_OUTPUTTYPE_OUTPUTS) $(ALL_ASMS)
	@echo 'Invoking: Linker'
	@echo 'Building target:'
	arm-none-eabi-ld -o "RZ_A1H_OpenVG_RSK.x" @"C:/work/rza1/OpenVG_ChnFont_Sample_code/RZ_A1H_OpenVG_RSK-CPT\HardwareDebug\LinkerSubCommand.tmp" -T"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\compiler_specific\GNU_e2studio-IRAM.ld" $(USER_OBJS) $(LIBS) $(LIBRARY_GENERATOR_OUTPUTTYPE_OUTPUTS) -M=RZ_A1H_OpenVG_RSK.map -L"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT\HardwareDebug" -L"C:\work\rza1\OpenVG_ChnFont_Sample_code\RZ_A1H_OpenVG_RSK-CPT" -L"C:\PROGRA~2\KPIT\GNUARM~1.01-\ARM-NO~1\ARM-NO~1/lib/gcc/arm-none-eabi/5.2-GNUARM-NONE_v16.01/fpu/interwork" --start-group -lRZ_A1H_OpenVG_RSK -lgcc --end-group -estart
	@echo 'Finished building:'
	@echo.

# Other Targets
clean:
ifneq ($(strip $(CC_UPPER_DEPS)),)
	-$(file > clean_CC_UPPER_DEPS.tmp, $(CC_UPPER_DEPS))
	-@ xargs -s 6000 -t -a clean_CC_UPPER_DEPS.tmp $(RM)
	-@$(RM) clean_CC_UPPER_DEPS.tmp
endif
ifneq ($(strip $(C++_UPPER_DEPS)),)
	-$(file > clean_C++_UPPER_DEPS.tmp, $(C++_UPPER_DEPS))
	-@ xargs -s 6000 -t -a clean_C++_UPPER_DEPS.tmp $(RM)
	-@$(RM) clean_C++_UPPER_DEPS.tmp
endif
ifneq ($(strip $(CXX_UPPER_DEPS)),)
	-$(file > clean_CXX_UPPER_DEPS.tmp, $(CXX_UPPER_DEPS))
	-@ xargs -s 6000 -t -a clean_CXX_UPPER_DEPS.tmp $(RM)
	-@$(RM) clean_CXX_UPPER_DEPS.tmp
endif
ifneq ($(strip $(CPP_UPPER_DEPS)),)
	-$(file > clean_CPP_UPPER_DEPS.tmp, $(CPP_UPPER_DEPS))
	-@ xargs -s 6000 -t -a clean_CPP_UPPER_DEPS.tmp $(RM)
	-@$(RM) clean_CPP_UPPER_DEPS.tmp
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
	-$(file > clean_C_UPPER_DEPS.tmp, $(C_UPPER_DEPS))
	-@ xargs -s 6000 -t -a clean_C_UPPER_DEPS.tmp $(RM)
	-@$(RM) clean_C_UPPER_DEPS.tmp
endif
ifneq ($(strip $(SRC_UPPER_DEPS)),)
	-$(file > clean_SRC_UPPER_DEPS.tmp, $(SRC_UPPER_DEPS))
	-@ xargs -s 6000 -t -a clean_SRC_UPPER_DEPS.tmp $(RM)
	-@$(RM) clean_SRC_UPPER_DEPS.tmp
endif
ifneq ($(strip $(S_DEPS)),)
	-$(file > clean_S_DEPS.tmp, $(S_DEPS))
	-@ xargs -s 6000 -t -a clean_S_DEPS.tmp $(RM)
	-@$(RM) clean_S_DEPS.tmp
endif
ifneq ($(strip $(C_DEPS)),)
	-$(file > clean_C_DEPS.tmp, $(C_DEPS))
	-@ xargs -s 6000 -t -a clean_C_DEPS.tmp $(RM)
	-@$(RM) clean_C_DEPS.tmp
endif
ifneq ($(strip $(SRC_DEPS)),)
	-$(file > clean_SRC_DEPS.tmp, $(SRC_DEPS))
	-@ xargs -s 6000 -t -a clean_SRC_DEPS.tmp $(RM)
	-@$(RM) clean_SRC_DEPS.tmp
endif
ifneq ($(strip $(LINKER_OUTPUT_OUTPUTS)),)
	-$(file > clean_LINKER_OUTPUT_OUTPUTS.tmp, $(LINKER_OUTPUT_OUTPUTS))
	-@ xargs -s 6000 -t -a clean_LINKER_OUTPUT_OUTPUTS.tmp $(RM)
	-@$(RM) clean_LINKER_OUTPUT_OUTPUTS.tmp
endif
ifneq ($(strip $(CC_DEPS)),)
	-$(file > clean_CC_DEPS.tmp, $(CC_DEPS))
	-@ xargs -s 6000 -t -a clean_CC_DEPS.tmp $(RM)
	-@$(RM) clean_CC_DEPS.tmp
endif
ifneq ($(strip $(C++_DEPS)),)
	-$(file > clean_C++_DEPS.tmp, $(C++_DEPS))
	-@ xargs -s 6000 -t -a clean_C++_DEPS.tmp $(RM)
	-@$(RM) clean_C++_DEPS.tmp
endif
ifneq ($(strip $(CXX_DEPS)),)
	-$(file > clean_CXX_DEPS.tmp, $(CXX_DEPS))
	-@ xargs -s 6000 -t -a clean_CXX_DEPS.tmp $(RM)
	-@$(RM) clean_CXX_DEPS.tmp
endif
ifneq ($(strip $(OBJS)),)
	-$(file > clean_OBJS.tmp, $(OBJS))
	-@ xargs -s 6000 -t -a clean_OBJS.tmp $(RM)
	-@$(RM) clean_OBJS.tmp
endif
ifneq ($(strip $(ASM_DEPS)),)
	-$(file > clean_ASM_DEPS.tmp, $(ASM_DEPS))
	-@ xargs -s 6000 -t -a clean_ASM_DEPS.tmp $(RM)
	-@$(RM) clean_ASM_DEPS.tmp
endif
ifneq ($(strip $(LIBRARY_GENERATOR_OUTPUTTYPE_OUTPUTS)),)
	-$(file > clean_LIBRARY_GENERATOR_OUTPUTTYPE_OUTPUTS.tmp, $(LIBRARY_GENERATOR_OUTPUTTYPE_OUTPUTS))
	-@ xargs -s 6000 -t -a clean_LIBRARY_GENERATOR_OUTPUTTYPE_OUTPUTS.tmp $(RM)
	-@$(RM) clean_LIBRARY_GENERATOR_OUTPUTTYPE_OUTPUTS.tmp
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
	-$(file > clean_S_UPPER_DEPS.tmp, $(S_UPPER_DEPS))
	-@ xargs -s 6000 -t -a clean_S_UPPER_DEPS.tmp $(RM)
	-@$(RM) clean_S_UPPER_DEPS.tmp
endif
ifneq ($(strip $(OBJCOPY_OUTPUTTYPE_OUTPUTS)),)
	-$(file > clean_OBJCOPY_OUTPUTTYPE_OUTPUTS.tmp, $(OBJCOPY_OUTPUTTYPE_OUTPUTS))
	-@ xargs -s 6000 -t -a clean_OBJCOPY_OUTPUTTYPE_OUTPUTS.tmp $(RM)
	-@$(RM) clean_OBJCOPY_OUTPUTTYPE_OUTPUTS.tmp
endif
ifneq ($(strip $(CPP_DEPS)),)
	-$(file > clean_CPP_DEPS.tmp, $(CPP_DEPS))
	-@ xargs -s 6000 -t -a clean_CPP_DEPS.tmp $(RM)
	-@$(RM) clean_CPP_DEPS.tmp
endif

	rm -rf ./"compiler_specific/asm"/*.tmp1 
	rm -rf ./"compiler_specific/src"/*.tmp1 
	rm -rf ./"src/RETWHUD"/*.tmp1 
	rm -rf ./"src/RETWOpenVG"/*.tmp1 
	rm -rf ./"src/RETWOpenVG/text"/*.tmp1 
	rm -rf ./"src/RETWapp"/*.tmp1 
	rm -rf ./"src/RETWgraphics"/*.tmp1 
	rm -rf ./"src/RGPNCG/src"/*.tmp1 
	rm -rf ./"src/board_settings"/*.tmp1 
	rm -rf ./"src/common"/*.tmp1 
	rm -rf ./"src/drivers/adc"/*.tmp1 
	rm -rf ./"src/drivers/common/common_driver"/*.tmp1 
	rm -rf ./"src/drivers/common/userdef"/*.tmp1 
	rm -rf ./"src/drivers/intc/intc_driver"/*.tmp1 
	rm -rf ./"src/drivers/intc/userdef"/*.tmp1 
	rm -rf ./"src/drivers/keyscanuart"/*.tmp1 
	rm -rf ./"src/drivers/ostm"/*.tmp1 
	rm -rf ./"src/drivers/riic/riic_driver"/*.tmp1 
	rm -rf ./"src/drivers/riic/userdef"/*.tmp1 
	rm -rf ./"src/drivers/rscan_rz/src"/*.tmp1 
	rm -rf ./"src/drivers/rspi/rspi_driver"/*.tmp1 
	rm -rf ./"src/drivers/rspi/userdef"/*.tmp1 
	rm -rf ./"src/drivers/rtc"/*.tmp1 
	rm -rf ./"src/drivers/scif_uart/scif_uart_driver"/*.tmp1 
	rm -rf ./"src/drivers/scif_uart/userdef"/*.tmp1 
	rm -rf ./"src/drivers/scux/scux_driver"/*.tmp1 
	rm -rf ./"src/drivers/ssif/src"/*.tmp1 
	rm -rf ./"src/drivers/vdc5/src"/*.tmp1 
	rm -rf ./"src/drivers/vdec/src"/*.tmp1 
	rm -rf ./"src"/*.tmp1 
	rm -rf ./"src/sample/graphics/common"/*.tmp1 
	rm -rf ./"src/sample/graphics/common/lcd_panel"/*.tmp1 
	rm -rf ./"src/sample/graphics"/*.tmp1 
	rm -rf ./"src/sample/graphics/rendering"/*.tmp1 
	rm -rf ./"src/sample/graphics/video_decoder"/*.tmp1 
	rm -rf ./"src/sample/sample_vdc5"/*.tmp1 
	rm -rf   RZ_A1H_OpenVG_RSK.mot

	@echo.
	@echo 'Clean complete.'

post-build:
	@echo.
	-arm-none-eabi-objcopy -O binary RZ_A1H_OpenVG_RSK.x  RZ_A1H_OpenVG_RSK.bin&
	@echo.

.PHONY: all clean dependents
.SECONDARY: post-build

-include ../makefile.targets
