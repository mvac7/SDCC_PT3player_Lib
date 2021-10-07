@echo off
REM MAKEFILE for MSXDOS .COM using SDCC compiler
REM CFILENAME = Source name without extension
SET CFILENAME=TestPT3
SET OUTPUTFILENAME=TESTPT3
SET CRT=libs\crt0_MSXDOS.rel
SET LIB0=libs\interruptM1_ISR.rel
SET LIB1=libs\interruptM1_Hooks.rel
SET LIB2=libs\memory.rel
SET LIB3=libs\keyboard_MSXDOS.rel
SET LIB4=libs\VDP_TMS9918A.rel
SET LIB5=libs\VDP_SPRITES.rel
SET LIB6=libs\VDP_PRINT.rel
SET LIB7=libs\unRLEWBtoVRAM.rel
SET LIB8=libs\AY38910BF.rel
SET LIB9=libs\PT3player.rel
SET LIB10=
SET LIB11=
SET LIB12=
if exist build\ goto NEXTSTEP1
echo MAKEDIR build
md build
:NEXTSTEP1
if exist bin\ goto NEXTSTEP2
echo MAKEDIR bin
md bin
:NEXTSTEP2
REM get SDCC version
sdcc -v
if not exist build\%CFILENAME%.ihx goto COMPILA
del build\%CFILENAME%.ihx
:COMPILA
echo Compiling...
sdcc -mz80 -o build\ --code-loc 0x0170 --data-loc 0 --use-stdout --no-std-crt0 %CRT% %LIB0% %LIB1% %LIB2% %LIB3% %LIB4% %LIB5% %LIB6% %LIB7% %LIB8% %LIB9% %LIB10% %LIB11% %LIB12% src\%CFILENAME%.c
if not exist build\%CFILENAME%.ihx goto ERROROUTPUT
hex2bin -e BIN build\%CFILENAME%.ihx
copy build\%CFILENAME%.BIN bin\%OUTPUTFILENAME%.COM
pause
exit
:ERROROUTPUT
echo ERROR
pause
exit