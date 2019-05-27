;
; -- Pt3 Rom Example -
;

CPU Z80
FNAME		"RUTAQUEQUIERAS\PT3ROM.rom"

@HTIMI@:	equ	$fd9f				; hook de interrupcion


		org	$4000

; Rom Ejemplo

~Header:	db	"AB"				; byte cabecera ROM
		dw	~Init				; direccion inicio codigo
		ds	12				; bytes reservados!
		
~Init:		di
		im	1
		ld	sp,$c1f0			; pila
		
		ld	hl,$c000			; borrar ram por si acaso
		ld	de,$c001
		ld	bc,$0fff			; a ojo pa probar
		ld	(hl),0
		ldir
		
		ld	hl,@MUSICA@			; direccion con musica
		call	~Pt3Init
		
		ld	hl,@HTIMI@ 			; old hook save
		ld	de,$f87f
		ld	bc,5
		ldir

		ld	a,$c3				; =JP 
		ld	(@HTIMI@),a			; new hook set
		ld	hl,~PlayInt
		ld	(@HTIMI@+1),hl
		ei

		jp	$


~PlayInt:	di					; interrupcion
		push	af
		push	bc
		push	de
		push	hl

		;in	a,(VDPCW)			; pa resetear bit int?
		
		call	~Pt3Play

		pop	hl
		pop	de
		pop	bc
		pop	af
		ei
		ret		


; REPLAYER en ROM
include		"RUTAQUEQUIERAS\Replayer_ROM.asm"

@MUSICA@:	incbin "RUTAQUEQUIERAS\MUSICAQUESEA.PT3" ; recordar quitar 100 primeros bytes

@END@:		ds   $8000-$            ; pad with 0 until address 8000h

; Area Reservada en RAM (no crea nada, solo contiene direcciones)
include		"RUTAQUEQUIERAS\Replayer_RAM.asm"

