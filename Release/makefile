################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf *.lst *.lis *.lpp *.map

# All of the sources participating in the build are defined here
-include sources.mk
-include src/sample/sample_vdc5/subdir.mk
-include src/sample/graphics/subdir.mk
-include src/sample/graphics/video_decoder/subdir.mk
-include src/sample/graphics/rendering/subdir.mk
-include src/sample/graphics/common/subdir.mk
-include src/sample/graphics/common/lcd_panel/subdir.mk
-include src/subdir.mk
-include src/drivers/vdec/src/subdir.mk
-include src/drivers/vdc5/src/subdir.mk
-include src/drivers/ssif/src/subdir.mk
-include src/drivers/sdhi/userdef/subdir.mk
-include src/drivers/sdhi/src/sd/sdio/subdir.mk
-include src/drivers/sdhi/src/sd/access/subdir.mk
-include src/drivers/scux/scux_driver/subdir.mk
-include src/drivers/scif_uart/userdef/subdir.mk
-include src/drivers/scif_uart/scif_uart_driver/subdir.mk
-include src/drivers/rtc/subdir.mk
-include src/drivers/rspi/userdef/subdir.mk
-include src/drivers/rspi/rspi_driver/subdir.mk
-include src/drivers/riic/userdef/subdir.mk
-include src/drivers/riic/riic_driver/subdir.mk
-include src/drivers/ostm/subdir.mk
-include src/drivers/keyscanuart/subdir.mk
-include src/drivers/intc/userdef/subdir.mk
-include src/drivers/intc/intc_driver/subdir.mk
-include src/drivers/common/userdef/subdir.mk
-include src/drivers/common/common_driver/subdir.mk
-include src/drivers/adc/subdir.mk
-include src/common/subdir.mk
-include src/board_settings/subdir.mk
-include src/RGPNCG/src/subdir.mk
-include src/RETWgraphics/subdir.mk
-include src/RETWapp/Audio/subdir.mk
-include src/RETWapp/Audio/parser/subdir.mk
-include src/RETWapp/Audio/parser/MP3/src/subdir.mk
-include src/RETWapp/Audio/parser/ID3/src/subdir.mk
-include src/RETWapp/Audio/command_scux/subdir.mk
-include src/RETWapp/Audio/command_dec/subdir.mk
-include src/RETWapp/Audio/command_dec/asm/subdir.mk
-include src/RETWapp/Audio/command/subdir.mk
-include src/RETWapp/Audio/audio/subdir.mk
-include src/RETWOpenVG/text/subdir.mk
-include src/RETWOpenVG/subdir.mk
-include src/RETWHUD/subdir.mk
-include src/RETWFreeType/subdir.mk
-include compiler_specific/src/subdir.mk
-include compiler_specific/asm/subdir.mk
-include objects.mk

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
	arm-none-eabi-objcopy -O srec  $(LINKER_OUTPUT_OUTPUTS)"RZ_A1H_OpenVG_RSK.mot"
	@echo 'Finished building target:'
	@echo.
	$(MAKE) --no-print-directory post-build

libRZ_A1H_OpenVG_RSK.a: 
	@echo 'Invoking: Library Generator'
	arm-none-eabi-libgen --select-lib=optlib --header-files=all --compiler-options=-mcpu=cortex-a9,-march=armv7-a,-mlittle-endian,-mthumb-interwork,-mfloat-abi=hard,-munaligned-access,-mfpu=vfp,-Os,-ffunction-sections,-fdata-sections,-fno-function-cse,-fshort-wchar,-funsigned-char,-funit-at-a-time,-falign-jumps --assembler-options=-mcpu=cortex-a9,-march=armv7-a,-mlittle-endian,-mthumb-interwork,-mfloat-abi=hard,-mfpu=vfp -o "libRZ_A1H_OpenVG_RSK.a"
	@echo 'Finished building:'
	@echo.

RZ_A1H_OpenVG_RSK.x: $(OBJS) $(LIBRARY_GENERATOR_OUTPUTTYPE_OUTPUTS) $(ALL_ASMS)
	@echo 'Invoking: Linker'
	@echo 'Building target:'
	arm-none-eabi-ld -o "RZ_A1H_OpenVG_RSK.x" -T"C:/Workspace/RZ/RZ_A1H_OpenVG_RSK\Release\LinkerSubCommand.tmp" -T"C:\Workspace\RZ\RZ_A1H_OpenVG_RSK\compiler_specific\GNU_e2studio-QSPI.ld" $(USER_OBJS) $(LIBS) $(LIBRARY_GENERATOR_OUTPUTTYPE_OUTPUTS) -M=RZ_A1H_OpenVG_RSK.map -L"C:\Workspace\RZ\RZ_A1H_OpenVG_RSK\Release" -L"C:\Workspace\RZ\RZ_A1H_OpenVG_RSK" -L"C:\PROGRA~1\KPIT\GNUARM~1.01-\ARM-NO~1\ARM-NO~1/lib/gcc/arm-none-eabi/4.9-GNUARM-NONE_v15.01/fpu/interwork" --start-group -lRZ_A1H_OpenVG_RSK -lgcc --end-group -estart
	@echo 'Finished building:'
	@echo.

# Other Targets
clean:
	-$(RM) $(LINKER_OUTPUT_OUTPUTS)$(LIBRARY_GENERATOR_OUTPUTTYPE_OUTPUTS)$(OBJCOPY_OUTPUTTYPE_OUTPUTS)
	rm -rf ./"src/sample/sample_vdc5"/*.o ./"src/sample/sample_vdc5"/*.d 
	rm -rf ./"src/sample/graphics"/*.o ./"src/sample/graphics"/*.d 
	rm -rf ./"src/sample/graphics/video_decoder"/*.o ./"src/sample/graphics/video_decoder"/*.d 
	rm -rf ./"src/sample/graphics/rendering"/*.o ./"src/sample/graphics/rendering"/*.d 
	rm -rf ./"src/sample/graphics/common"/*.o ./"src/sample/graphics/common"/*.d 
	rm -rf ./"src/sample/graphics/common/lcd_panel"/*.o ./"src/sample/graphics/common/lcd_panel"/*.d 
	rm -rf ./"src"/*.o ./"src"/*.d 
	rm -rf ./"src/drivers/vdec/src"/*.o ./"src/drivers/vdec/src"/*.d 
	rm -rf ./"src/drivers/vdc5/src"/*.o ./"src/drivers/vdc5/src"/*.d 
	rm -rf ./"src/drivers/ssif/src"/*.o ./"src/drivers/ssif/src"/*.d 
	rm -rf ./"src/drivers/sdhi/userdef"/*.o ./"src/drivers/sdhi/userdef"/*.d 
	rm -rf ./"src/drivers/sdhi/src/sd/sdio"/*.o ./"src/drivers/sdhi/src/sd/sdio"/*.d 
	rm -rf ./"src/drivers/sdhi/src/sd/access"/*.o ./"src/drivers/sdhi/src/sd/access"/*.d 
	rm -rf ./"src/drivers/scux/scux_driver"/*.o ./"src/drivers/scux/scux_driver"/*.d 
	rm -rf ./"src/drivers/scif_uart/userdef"/*.o ./"src/drivers/scif_uart/userdef"/*.d 
	rm -rf ./"src/drivers/scif_uart/scif_uart_driver"/*.o ./"src/drivers/scif_uart/scif_uart_driver"/*.d 
	rm -rf ./"src/drivers/rtc"/*.o ./"src/drivers/rtc"/*.d 
	rm -rf ./"src/drivers/rspi/userdef"/*.o ./"src/drivers/rspi/userdef"/*.d 
	rm -rf ./"src/drivers/rspi/rspi_driver"/*.o ./"src/drivers/rspi/rspi_driver"/*.d 
	rm -rf ./"src/drivers/riic/userdef"/*.o ./"src/drivers/riic/userdef"/*.d 
	rm -rf ./"src/drivers/riic/riic_driver"/*.o ./"src/drivers/riic/riic_driver"/*.d 
	rm -rf ./"src/drivers/ostm"/*.o ./"src/drivers/ostm"/*.d 
	rm -rf ./"src/drivers/keyscanuart"/*.o ./"src/drivers/keyscanuart"/*.d 
	rm -rf ./"src/drivers/intc/userdef"/*.o ./"src/drivers/intc/userdef"/*.d 
	rm -rf ./"src/drivers/intc/intc_driver"/*.o ./"src/drivers/intc/intc_driver"/*.d 
	rm -rf ./"src/drivers/common/userdef"/*.o ./"src/drivers/common/userdef"/*.d 
	rm -rf ./"src/drivers/common/common_driver"/*.o ./"src/drivers/common/common_driver"/*.d 
	rm -rf ./"src/drivers/adc"/*.o ./"src/drivers/adc"/*.d 
	rm -rf ./"src/common"/*.o ./"src/common"/*.d 
	rm -rf ./"src/board_settings"/*.o ./"src/board_settings"/*.d 
	rm -rf ./"src/RGPNCG/src"/*.o ./"src/RGPNCG/src"/*.d 
	rm -rf ./"src/RETWgraphics"/*.o ./"src/RETWgraphics"/*.d 
	rm -rf ./"src/RETWapp/Audio"/*.o ./"src/RETWapp/Audio"/*.d 
	rm -rf ./"src/RETWapp/Audio/parser"/*.o ./"src/RETWapp/Audio/parser"/*.d 
	rm -rf ./"src/RETWapp/Audio/parser/MP3/src"/*.o ./"src/RETWapp/Audio/parser/MP3/src"/*.d 
	rm -rf ./"src/RETWapp/Audio/parser/ID3/src"/*.o ./"src/RETWapp/Audio/parser/ID3/src"/*.d 
	rm -rf ./"src/RETWapp/Audio/command_scux"/*.o ./"src/RETWapp/Audio/command_scux"/*.d 
	rm -rf ./"src/RETWapp/Audio/command_dec"/*.o ./"src/RETWapp/Audio/command_dec"/*.d 
	rm -rf ./"src/RETWapp/Audio/command_dec/asm"/*.o ./"src/RETWapp/Audio/command_dec/asm"/*.d 
	rm -rf ./"src/RETWapp/Audio/command"/*.o ./"src/RETWapp/Audio/command"/*.d 
	rm -rf ./"src/RETWapp/Audio/audio"/*.o ./"src/RETWapp/Audio/audio"/*.d 
	rm -rf ./"src/RETWOpenVG/text"/*.o ./"src/RETWOpenVG/text"/*.d 
	rm -rf ./"src/RETWOpenVG"/*.o ./"src/RETWOpenVG"/*.d 
	rm -rf ./"src/RETWHUD"/*.o ./"src/RETWHUD"/*.d 
	rm -rf ./"src/RETWFreeType"/*.o ./"src/RETWFreeType"/*.d 
	rm -rf ./"compiler_specific/src"/*.o ./"compiler_specific/src"/*.d 
	rm -rf ./"compiler_specific/asm"/*.o ./"compiler_specific/asm"/*.d  RZ_A1H_OpenVG_RSK.mot
	@echo.
	@echo 'Clean complete.'

post-build:
	@echo.
	-arm-none-eabi-objcopy -O binary RZ_A1H_OpenVG_RSK.x  RZ_A1H_OpenVG_RSK.bin&
	@echo.

.PHONY: all clean dependents
.SECONDARY: post-build

-include ../makefile.targets
