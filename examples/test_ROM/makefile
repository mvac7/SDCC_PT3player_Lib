# MAKEFILE for 8/16K ROMs using SDCC compiler
CFILENAME = Source name without extension
ROMFILENAME = ROM file name without extension
ROMSIZE = 2000 for 8KBytes ROM or 4000 for 16KBytes ROM
CFILENAME=TestPT3
ROMFILENAME=TESTPT3
CODELOC=0x4020
ROMSIZE=4000
CRT=libs/crt0msx.16k.4000.rel
LIB0=libs/interruptM1_Hooks.rel
LIB1=libs/memory.rel
LIB2=libs/keyboard_MSXROM.rel
LIB3=libs/VDP_TMS9918A_MSXROM.rel
LIB4=libs/VDP_PRINT.rel
LIB5=libs/unRLEWBtoVRAM.rel
LIB6=libs/AY38910BF.rel
LIB7=libs/PT3player.rel
LIB8=
LIB9=
LIB10=
LIB11=
LIB12=

bin/${ROMFILENAME}.ROM:
	hex2bin -e bin -l ${ROMSIZE} build/${CFILENAME}.ihx
	cp build/${CFILENAME}.bin bin/${ROMFILENAME}.ROM

build/${CFILENAME}.ihx:
	mkdir -p build
	mkdir -p bin
	sdcc -mz80 -o build/ --code-loc ${CODELOC} --data-loc 0xC000 --use-stdout \
		--no-std-crt0 ${CRT} ${LIB0} ${LIB1} ${LIB2} ${LIB3} ${LIB4} ${LIB5} \
		${LIB6} ${LIB7} ${LIB8} ${LIB9} ${LIB10} ${LIB11} ${LIB12} \
		src/${CFILENAME}.c


