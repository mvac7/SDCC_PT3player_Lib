;
; - - - - - - - - - - - - - - - - - -  PT3  Replayer  - - - - - - - - - - - - - - - - - -
;

;  In this replayer:
; -No version detection (just for Vortex Tracker II and PT3.5).
; -No frequency table decompression (default is number 2). - Coger tabla segun quiera, en fichero aparte
; -No volume table decompression (Vortex Tracker II/PT3.5 volume table used).
;
; Warning!!! Delete 100 first bytes from your PT3 module.

; - New added-

; Usable desde ROM (solo tener en RAM area de trabajo)


		org	$c200		; DIRECCION RAM CON DATOS REPLAYER & MONOCORDER


; =============================================== RAM DATA =================================================

; - Workarea - (lo que antes estaba en codigo automodificable)

@PT3WRK@:	rb	32			; workarea que reemplaza a codigo automodificable

; - Channel Data -

VARS: 
ChanA:		rb CHNPRM_Size		; 29 bytes
ChanB:		rb CHNPRM_Size		; 29 bytes
ChanC:		rb CHNPRM_Size		; 29 bytes

; - GlobalVars -

DelyCnt:	rb 1			; 1
CurESld:	rw 1			; 2	
CurEDel:	rb 1			; 1
Ns_Base_AddToNs:
Ns_Base:	rb 1			; 1
AddToNs:	rb 1			; 1

; - Reg Data -

AYREGS:
VT_:	rb	256


		

