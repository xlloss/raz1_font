@echo off
rem  This batch file will load the QSPI boot loader program.
rem  Please change the JLINK.EXE path below if it is different in your system. 
echo LOAD BOOT LOADER INTO QSPI FLASH (IC26)
echo ========================================================================
echo ***    Remember to update the JLink.exe path specified               ***
echo ***    in this batch file if using a different version than          ***
echo ***    that installed by the RSK+RZA1H_E2 installer.                 ***
echo ***    This batch file was tested and works with Ver 4.90e           ***
echo ***    of the JLink.exe                                              ***
echo ***                                                                  ***
echo ***    Please refer to the:                                          ***
echo ***    RSK+RZA1H QSPI Boot Loader Application Note (R01AN2141EG0100) *** 
echo ***    for more information.                                         ***
echo ========================================================================
echo .
echo The Load QSPI Boot Loader option will only load the QSPI boot loader  
echo program into the QSPI device (IC26). 
echo .
echo Please make sure you have read the Description.txt file in the doc 
echo folder before selecting an option.
echo ------------------------------------------------------------------------
echo .
"C:\Program Files\SEGGER\JLink_V512b\JLink.exe" -speed 12000 -if JTAG -device R7S721001 -CommanderScript LoadHardwareDebug0x18200000.txt
