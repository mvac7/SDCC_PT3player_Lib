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

; Usable desde ROM (solo tiene en RAM area de trabajo, lo minimo posible).


;  --- ChannelsVars ---  (esto son equs, no posiciones RAM)

; - struc CHNPRM -

CHNPRM_PsInOr:	equ 0	;RESB 1
CHNPRM_PsInSm:	equ 1	;RESB 1
CHNPRM_CrAmSl:	equ 2	;RESB 1
CHNPRM_CrNsSl:	equ 3	;RESB 1
CHNPRM_CrEnSl:	equ 4	;RESB 1
CHNPRM_TSlCnt:	equ 5	;RESB 1
CHNPRM_CrTnSl:	equ 6	;RESW 1
CHNPRM_TnAcc:	equ 8	;RESW 1
CHNPRM_COnOff:	equ 10	;RESB 1
CHNPRM_OnOffD:	equ 11	;RESB 1
;IX for PTDECOD here (+12)
CHNPRM_OffOnD:	equ 12	;RESB 1
CHNPRM_OrnPtr:	equ 13	;RESW 1
CHNPRM_SamPtr:	equ 15	;RESW 1
CHNPRM_NNtSkp:	equ 17	;RESB 1
CHNPRM_Note:	equ 18	;RESB 1
CHNPRM_SlToNt:	equ 19	;RESB 1
CHNPRM_Env_En:	equ 20	;RESB 1
CHNPRM_Flags:	equ 21	;RESB 1
 ;Enabled - 0,SimpleGliss - 2
CHNPRM_TnSlDl:	equ 22	;RESB 1
CHNPRM_TSlStp:	equ 23	;RESW 1
CHNPRM_TnDelt:	equ 25	;RESW 1
CHNPRM_NtSkCn:	equ 27	;RESB 1
CHNPRM_Volume:	equ 28	;RESB 1
CHNPRM_Size:	equ 29	;RESB 1
; - endstruc -

;- struc AR -

AR_TonA:	equ 0	;RESW 1
AR_TonB:	equ 2	;RESW 1
AR_TonC:	equ 4	;RESW 1
AR_Noise:	equ 6	;RESB 1
AR_Mixer:	equ 7	;RESB 1
AR_AmplA:	equ 8	;RESB 1
AR_AmplB:	equ 9	;RESB 1
AR_AmplC:	equ 10	;RESB 1
AR_Env:		equ 11	;RESW 1
AR_EnvTp:	equ 13	;RESB 1
;endstruc


EnvBase:	equ VT_+14

T1_:		equ VT_+16 ;Tone tables data depacked here

T_OLD_1:	equ T1_
T_OLD_2:	equ T_OLD_1+24
T_OLD_3:	equ T_OLD_2+24
T_OLD_0:	equ T_OLD_3+2
T_NEW_0:	equ T_OLD_0
T_NEW_1:	equ T_OLD_1
T_NEW_2:	equ T_NEW_0+24
T_NEW_3:	equ T_OLD_3

Ampl:		equ AYREGS+AR_AmplC

VAR0END:	equ VT_+16 ;INIT zeroes from VARS to VAR0END-1


; --- Workarea --- (apunta a RAM que estaba antes en codigo automodificable)

; -El byte de estado en SETUP deberia ser algo asi (CH enable/disable no esta aun)
;|EP|0|0|0|CH3|CH2|CH1|LP|

;LP: Loop enable/disable. A 1 si queremos que el tema suene solo una vez. 
;EP: End point. A 1 cada vez que el tema acaba. 
;CH1-CH3: Channel enable/disable. A 1 si no queremos que suene el canal. (AUN  NO VA!!)

SETUP:		equ	@PT3WRK@ 	; set bit0 to 1, if you want to play without LOOPING
					; bit7 is set each time, when loop point is passed
CrPsPtr:	equ	@PT3WRK@+1  	; 2 bytes POSICION CURSOR EN PATTERN
@MODADDR@:	equ	@PT3WRK@+3	; 2 bytes dir musica guardada
@MDADDR2@:	equ	@PT3WRK@+5	; 2 bytes dir musica guardada 2 (? Creo que puedo usar el mismo siempre)
@SAMPTRS@:	equ	@PT3WRK@+7	; 2 bytes sample info?
@PDSP@:		equ	@PT3WRK@+9	; 2 bytes pilasave
@ORNPTRS@:	equ	@PT3WRK@+11	; 2 bytes Ornament pattern
@CSP@:		equ	@PT3WRK@+13	; 2 bytes pilsave2 (? puede usarse solo 1 tb??)
@ADINPTA@:	equ	@PT3WRK@+15	; 2 bytes play data pattern
@ADINPTB@:	equ	@PT3WRK@+17	; 2 bytes play data
@LPOSPTR@:	equ	@PT3WRK@+19	; 2 bytes Pos Ptr
@PATSPTR@:	equ	@PT3WRK@+21	; 2 bytes Pat Ptr
@PSP@:		equ	@PT3WRK@+23	; 2 bytes pilsave3 (? idem usar solo 1??)
@ADINPTC@:	equ	@PT3WRK@+25	; 2 bytes play data
@DELAY@:	equ	@PT3WRK@+27	; 1 byte delay
@ADDTOEN@:	equ	@PT3WRK@+28	; 1 byte Envelope data (No cal ya que no usa Envs??)
@ENVDEL@:	equ	@PT3WRK@+29	; 1 byte Envelope data (idem)
@ESLDADD@:	equ	@PT3WRK@+30	; 2 bytes Envelope data (idem)
	

; ================================================ REPLAYER =================================================

; PT3 JP Table (podria incluso ahorrarse)

~Pt3Init:	jp	init		; inicializa musica
~Pt3Play:	jp	play		; playea
~Pt3Stop:	jp	mute		; para

; -- replayer --

CHECKLP:	LD HL,SETUP
	SET 7,(HL)
	BIT 0,(HL)
	RET Z
	POP HL
	LD HL,DelyCnt
	INC (HL)
	LD HL,ChanA+CHNPRM_NtSkCn
	INC (HL)
MUTE:	XOR A
	LD H,A
	LD L,A
	LD (AYREGS+AR_AmplA),A
	LD (AYREGS+AR_AmplB),HL
	JP ROUT_A0

INIT:	;HL - AddressOfModule-100

	;ld hl,MDLADDR-100	; ## esto ahora se da como entrada al llamar a rutina
				; asi que la musica puede ir en cualquier lado ROM o RAM

	or	a		; # added
	ld	de,100		; # added
	sbc	hl,de		; # added

	LD (@MODADDR@),HL	; # chg
	LD (@MDADDR2@),HL	; # chg
	PUSH HL
	LD DE,100
	ADD HL,DE
	LD A,(HL)
	LD (@Delay@),A		; # chg
	PUSH HL
	POP IX
	ADD HL,DE
	LD (CrPsPtr),HL
	LD E,(IX+102-100)
	ADD HL,DE
	INC HL
	LD (@LPosPtr@),HL	; # chg
	POP DE
	LD L,(IX+103-100)
	LD H,(IX+104-100)
	ADD HL,DE
	LD (@PatsPtr@),HL	; # chg
	LD HL,169
	ADD HL,DE
	LD (@OrnPtrs@),HL	; # chg
	LD HL,105
	ADD HL,DE
	LD (@SamPtrs@),HL	; # chg
	LD HL,SETUP
	RES 7,(HL)

	ld	hl,VTcopy	; # added
	ld	de,VT_		; # copia tabla de volumen		
	ld	bc,256
	ldir
	xor	a
	
	LD HL,VARS
	LD (HL),A
	LD DE,VARS+1
	LD BC,VAR0END-VARS-1
	LDIR
	INC A
	LD (DelyCnt),A
	LD HL,$F001	;H - CHNPRM_Volume, L - CHNPRM_NtSkCn
	LD (ChanA+CHNPRM_NtSkCn),HL
	LD (ChanB+CHNPRM_NtSkCn),HL
	LD (ChanC+CHNPRM_NtSkCn),HL

	LD HL,EMPTYSAMORN
	LD (@AdInPtA@),HL ;ptr to zero  ; # chg
	LD (ChanA+CHNPRM_OrnPtr),HL ;ornament 0 is "0,1,0"
	LD (ChanB+CHNPRM_OrnPtr),HL ;in all versions from
	LD (ChanC+CHNPRM_OrnPtr),HL ;3.xx to 3.6x and VTII

	LD (ChanA+CHNPRM_SamPtr),HL ;S1 There is no default
	LD (ChanB+CHNPRM_SamPtr),HL ;S2 sample in PT3, so, you
	LD (ChanC+CHNPRM_SamPtr),HL ;S3 can comment S1,2,3; see
				    ;also EMPTYSAMORN comment
	JP ROUT_A0

;pattern decoder
PD_OrSm:	LD (IX-12+CHNPRM_Env_En),0
	CALL SETORN
	LD A,(BC)
	INC BC
	RRCA

PD_SAM:	ADD A,A
PD_SAM_:	LD E,A
	LD D,0
SamPtrs: ;equ $+1			; ## AUTOMODIF-1 !! (@SAMPTRS@)
	;LD HL,$2121
	ld	hl,(@SAMPTRS@)		; # chg
	ADD HL,DE
	LD E,(HL)
	INC HL
	LD D,(HL)
MODADDR: ;equ $+1			; ## AUTOMODIF-2 !! (@MODADDR@)
	;LD HL,$2121
	ld	hl,(@MODADDR@)		; # chg
	ADD HL,DE
	LD (IX-12+CHNPRM_SamPtr),L
	LD (IX-12+CHNPRM_SamPtr+1),H
	JR PD_LOOP

PD_VOL:	RLCA
	RLCA
	RLCA
	RLCA
	LD (IX-12+CHNPRM_Volume),A
	JR PD_LP2
	
PD_EOff:	LD (IX-12+CHNPRM_Env_En),A
	LD (IX-12+CHNPRM_PsInOr),A
	JR PD_LP2

PD_SorE:	DEC A
	JR NZ,PD_ENV
	LD A,(BC)
	INC BC
	LD (IX-12+CHNPRM_NNtSkp),A
	JR PD_LP2

PD_ENV:	CALL SETENV
	JR PD_LP2

PD_ORN:	CALL SETORN
	JR PD_LOOP
       
PD_ESAM:	LD (IX-12+CHNPRM_Env_En),A
	LD (IX-12+CHNPRM_PsInOr),A
	CALL NZ,SETENV
	LD A,(BC)
	INC BC
	JR PD_SAM_

PTDECOD: LD A,(IX-12+CHNPRM_Note)
	LD (PrNote+1),A

PD_LOOP:	LD DE,$2010
PD_LP2:	LD A,(BC)
	INC BC
	ADD A,E
	JR C,PD_OrSm
	ADD A,D
	JR Z,PD_FIN
	JR C,PD_SAM
	ADD A,E
	JR Z,PD_REL
	JR C,PD_VOL
	ADD A,E
	JR Z,PD_EOff
	JR C,PD_SorE
	ADD A,96
	JR C,PD_NOTE
	ADD A,E
	JR C,PD_ORN
	ADD A,D
	JR C,PD_NOIS
	ADD A,E
	JR C,PD_ESAM
	ADD A,A
	LD E,A
	
	PUSH DE
	LD DE,$DF20
	LD HL,SPCCOMS	
	ADD HL,DE
	POP DE
	
	ADD HL,DE
	LD E,(HL)
	INC HL
	LD D,(HL)
	PUSH DE
	JR PD_LOOP

PD_NOIS:	LD (Ns_Base),A
	JR PD_LP2

PD_REL:	RES 0,(IX-12+CHNPRM_Flags)
	JR PD_RES
	
PD_NOTE:	LD (IX-12+CHNPRM_Note),A
	SET 0,(IX-12+CHNPRM_Flags)
	XOR A

PD_RES:	;LD (PDSP_+1),SP			; ## AUTOMODIF-3 (@PDSP@) PilaSave !!
	ld	(@PDSP@),sp		; # chg
	LD SP,IX
	LD H,A
	LD L,A
	PUSH HL
	PUSH HL
	PUSH HL
	PUSH HL
	PUSH HL
	PUSH HL
PDSP_:	;LD SP,$3131			; ## AUTOMODIF-3 (@PDSP_@) PilaSave !!
	ld	sp,(@PDSP@)		; # chg

PD_FIN:	LD A,(IX-12+CHNPRM_NNtSkp)
	LD (IX-12+CHNPRM_NtSkCn),A
	RET

C_PORTM: RES 2,(IX-12+CHNPRM_Flags)
	LD A,(BC)
	INC BC
;SKIP PRECALCULATED TONE DELTA (BECAUSE
;CANNOT BE RIGHT AFTER PT3 COMPILATION)
	INC BC
	INC BC
	LD (IX-12+CHNPRM_TnSlDl),A
	LD (IX-12+CHNPRM_TSlCnt),A
	LD DE,NT_
	LD A,(IX-12+CHNPRM_Note)
	LD (IX-12+CHNPRM_SlToNt),A
	ADD A,A
	LD L,A
	LD H,0
	ADD HL,DE
	LD A,(HL)
	INC HL
	LD H,(HL)
	LD L,A
	PUSH HL
PrNote:	LD A,$3E
	LD (IX-12+CHNPRM_Note),A
	ADD A,A
	LD L,A
	LD H,0
	ADD HL,DE
	LD E,(HL)
	INC HL
	LD D,(HL)
	POP HL
	SBC HL,DE
	LD (IX-12+CHNPRM_TnDelt),L
	LD (IX-12+CHNPRM_TnDelt+1),H
	LD E,(IX-12+CHNPRM_CrTnSl)
	LD D,(IX-12+CHNPRM_CrTnSl+1)

OLDPRTM:	LD A,(BC) ;SIGNED TONE STEP
	INC BC
	EX AF,AF'
	LD A,(BC)
	INC BC
	AND A
	JR Z,NOSIG
	EX DE,HL
NOSIG:	SBC HL,DE
	JP P,SET_STP
	CPL
	EX AF,AF'
	NEG
	EX AF,AF'
SET_STP:	LD (IX-12+CHNPRM_TSlStp+1),A
	EX AF,AF'
	LD (IX-12+CHNPRM_TSlStp),A
	LD (IX-12+CHNPRM_COnOff),0
	RET

C_GLISS:	SET 2,(IX-12+CHNPRM_Flags)
	LD A,(BC)
	INC BC
	LD (IX-12+CHNPRM_TnSlDl),A
	LD (IX-12+CHNPRM_TSlCnt),A
	LD A,(BC)
	INC BC
	EX AF,AF'
	LD A,(BC)
	INC BC
	JR SET_STP

C_SMPOS:	LD A,(BC)
	INC BC
	LD (IX-12+CHNPRM_PsInSm),A
	RET

C_ORPOS:	LD A,(BC)
	INC BC
	LD (IX-12+CHNPRM_PsInOr),A
	RET

C_VIBRT:	LD A,(BC)
	INC BC
	LD (IX-12+CHNPRM_OnOffD),A
	LD (IX-12+CHNPRM_COnOff),A
	LD A,(BC)
	INC BC
	LD (IX-12+CHNPRM_OffOnD),A
	XOR A
	LD (IX-12+CHNPRM_TSlCnt),A
	LD (IX-12+CHNPRM_CrTnSl),A
	LD (IX-12+CHNPRM_CrTnSl+1),A
	RET

C_ENGLS:	LD A,(BC)
	INC BC
	LD (@EnvDel@),A			; # chg
	LD (CurEDel),A
	LD A,(BC)
	INC BC
	LD L,A
	LD A,(BC)
	INC BC
	LD H,A
	LD (@ESldAdd@),HL		; # chg
	RET

C_DELAY:	LD A,(BC)
	INC BC
	LD (@Delay@),A			; # chg
	RET
	
SETENV:	LD (IX-12+CHNPRM_Env_En),E
	LD (AYREGS+AR_EnvTp),A
	LD A,(BC)
	INC BC
	LD H,A
	LD A,(BC)
	INC BC
	LD L,A
	LD (EnvBase),HL
	XOR A
	LD (IX-12+CHNPRM_PsInOr),A
	LD (CurEDel),A
	LD H,A
	LD L,A
	LD (CurESld),HL
C_NOP:	RET

SETORN:	ADD A,A
	LD E,A
	LD D,0
	LD (IX-12+CHNPRM_PsInOr),D
OrnPtrs:	;equ $+1			; ## AUTOMOFIF-4 (@OrnPtrs@) Ornaments
	;LD HL,$2121
	ld	hl,(@ORNPTRS@)		; # chg
	ADD HL,DE
	LD E,(HL)
	INC HL
	LD D,(HL)
MDADDR2:	;equ $+1			; ## AUTOMOFIF-1b (@MODADR2@) Ornaments
	;LD HL,$2121
	ld	hl,(@MDADDR2@)		; # chg
	ADD HL,DE
	LD (IX-12+CHNPRM_OrnPtr),L
	LD (IX-12+CHNPRM_OrnPtr+1),H
	RET

;ALL 16 ADDRESSES TO PROTECT FROM BROKEN PT3 MODULES 

SPCCOMS: dw C_NOP			; ## COMPROBAR Q NO SEA AUTOMODIF
	dw C_GLISS			; (parece que no lo toca nada)
	dw C_PORTM
	dw C_SMPOS
	dw C_ORPOS
	dw C_VIBRT
	dw C_NOP
	dw C_NOP
	dw C_ENGLS
	dw C_DELAY
	dw C_NOP
	dw C_NOP
	dw C_NOP
	dw C_NOP
	dw C_NOP
	dw C_NOP

CHREGS:	XOR A
	LD (Ampl),A
	BIT 0,(IX+CHNPRM_Flags)
	PUSH HL
	JP Z,CH_EXIT
	;LD (CSP_+1),SP			; ## APUNTA A CODIGO AUTOMODIF
	ld	(@CSP@),sp		; # chg
	LD L,(IX+CHNPRM_OrnPtr)
	LD H,(IX+CHNPRM_OrnPtr+1)
	LD SP,HL
	POP DE
	LD H,A
	LD A,(IX+CHNPRM_PsInOr)
	LD L,A
	ADD HL,SP
	INC A
	CP D
	JR C,CH_ORPS
	LD A,E
CH_ORPS:	LD (IX+CHNPRM_PsInOr),A
	LD A,(IX+CHNPRM_Note)
	ADD A,(HL)
	JP P,CH_NTP
	XOR A
CH_NTP:	CP 96
	JR C,CH_NOK
	LD A,95
CH_NOK:	ADD A,A
	EX AF,AF'
	LD L,(IX+CHNPRM_SamPtr)
	LD H,(IX+CHNPRM_SamPtr+1)
	LD SP,HL
	POP DE
	LD H,0
	LD A,(IX+CHNPRM_PsInSm)
	LD B,A
	ADD A,A
	ADD A,A
	LD L,A
	ADD HL,SP
	LD SP,HL
	LD A,B
	INC A
	CP D
	JR C,CH_SMPS
	LD A,E
CH_SMPS:	LD (IX+CHNPRM_PsInSm),A
	POP BC
	POP HL
	LD E,(IX+CHNPRM_TnAcc)
	LD D,(IX+CHNPRM_TnAcc+1)
	ADD HL,DE
	BIT 6,B
	JR Z,CH_NOAC
	LD (IX+CHNPRM_TnAcc),L
	LD (IX+CHNPRM_TnAcc+1),H
CH_NOAC: EX DE,HL
	EX AF,AF'
	LD L,A
	LD H,0
	LD SP,NT_
	ADD HL,SP
	LD SP,HL
	POP HL
	ADD HL,DE
	LD E,(IX+CHNPRM_CrTnSl)
	LD D,(IX+CHNPRM_CrTnSl+1)
	ADD HL,DE
CSP_:	;LD SP,$3131			; ## AUTOMODIF-5 (@CSP_@) Mas pila
	ld	sp,(@CSP@)		; # chg
	EX (SP),HL
	XOR A
	OR (IX+CHNPRM_TSlCnt)
	JR Z,CH_AMP
	DEC (IX+CHNPRM_TSlCnt)
	JR NZ,CH_AMP
	LD A,(IX+CHNPRM_TnSlDl)
	LD (IX+CHNPRM_TSlCnt),A
	LD L,(IX+CHNPRM_TSlStp)
	LD H,(IX+CHNPRM_TSlStp+1)
	LD A,H
	ADD HL,DE
	LD (IX+CHNPRM_CrTnSl),L
	LD (IX+CHNPRM_CrTnSl+1),H
	BIT 2,(IX+CHNPRM_Flags)
	JR NZ,CH_AMP
	LD E,(IX+CHNPRM_TnDelt)
	LD D,(IX+CHNPRM_TnDelt+1)
	AND A
	JR Z,CH_STPP
	EX DE,HL
CH_STPP: SBC HL,DE
	JP M,CH_AMP
	LD A,(IX+CHNPRM_SlToNt)
	LD (IX+CHNPRM_Note),A
	XOR A
	LD (IX+CHNPRM_TSlCnt),A
	LD (IX+CHNPRM_CrTnSl),A
	LD (IX+CHNPRM_CrTnSl+1),A
CH_AMP:	LD A,(IX+CHNPRM_CrAmSl)
	BIT 7,C
	JR Z,CH_NOAM
	BIT 6,C
	JR Z,CH_AMIN
	CP 15
	JR Z,CH_NOAM
	INC A
	JR CH_SVAM
CH_AMIN:	CP -15
	JR Z,CH_NOAM
	DEC A
CH_SVAM:	LD (IX+CHNPRM_CrAmSl),A
CH_NOAM:	LD L,A
	LD A,B
	AND 15
	ADD A,L
	JP P,CH_APOS
	XOR A
CH_APOS:	CP 16
	JR C,CH_VOL
	LD A,15
CH_VOL:	OR (IX+CHNPRM_Volume)
	LD L,A
	LD H,0
	LD DE,VT_
	ADD HL,DE
	LD A,(HL)
CH_ENV:	BIT 0,C
	JR NZ,CH_NOEN
	OR (IX+CHNPRM_Env_En)
CH_NOEN:	LD (Ampl),A
	BIT 7,B
	LD A,C
	JR Z,NO_ENSL
	RLA
	RLA
	SRA A
	SRA A
	SRA A
	ADD A,(IX+CHNPRM_CrEnSl) ;SEE COMMENT BELOW
	BIT 5,B
	JR Z,NO_ENAC
	LD (IX+CHNPRM_CrEnSl),A
NO_ENAC:	;LD HL,AddToEn	; ## APUNTA A AUTOMODIF
	ld	hl,@ADDTOEN@	; # chg
	ADD A,(HL) ;BUG IN PT3 - NEED WORD HERE.
		   ;FIX IT IN NEXT VERSION?
	LD (HL),A
	JR CH_MIX
NO_ENSL: RRA
	ADD A,(IX+CHNPRM_CrNsSl)
	LD (AddToNs),A
	BIT 5,B
	JR Z,CH_MIX
	LD (IX+CHNPRM_CrNsSl),A
CH_MIX:	LD A,B
	RRA
	AND $48
CH_EXIT:	LD HL,AYREGS+AR_Mixer
	OR (HL)
	RRCA
	LD (HL),A
	POP HL
	XOR A
	OR (IX+CHNPRM_COnOff)
	RET Z
	DEC (IX+CHNPRM_COnOff)
	RET NZ
	XOR (IX+CHNPRM_Flags)
	LD (IX+CHNPRM_Flags),A
	RRA
	LD A,(IX+CHNPRM_OnOffD)
	JR C,CH_ONDL
	LD A,(IX+CHNPRM_OffOnD)
CH_ONDL:	LD (IX+CHNPRM_COnOff),A
	RET

PLAY:   XOR A
	LD (@AddToEn@),A
	LD (AYREGS+AR_Mixer),A
	DEC A
	LD (AYREGS+AR_EnvTp),A
	LD HL,DelyCnt
	DEC (HL)
	JP NZ,PL2	; # jr nz cambiado por jp nz pq no llega
	LD HL,ChanA+CHNPRM_NtSkCn
	DEC (HL)
	JR NZ,PL1B
AdInPtA:	;equ $+1			; ## AUTOMODIF-6 (@AdInPtA@)
	;LD BC,$0101
	ld	bc,(@ADINPTA@)		; # chg
	LD A,(BC)
	AND A
	JR NZ,PL1A
	LD D,A
	LD (Ns_Base),A
	LD HL,(CrPsPtr)
	INC HL
	LD A,(HL)
	INC A
	JR NZ,PLNLP
	CALL CHECKLP
LPosPtr:	;equ $+1			; ## AUTOMODIF-7 (@LPosPtr@)
	;LD HL,$2121
	ld	hl,(@LPOSPTR@)		; # chg
	LD A,(HL)
	INC A
PLNLP:	LD (CrPsPtr),HL
	DEC A
	ADD A,A
	LD E,A
	RL D
PatsPtr:	;equ $+1			; ## AUTOMODIF-8 (@PatsPtr@)
	;LD HL,$2121
	ld	hl,(@PATSPTR@)		; # chg
	ADD HL,DE
	LD DE,(@MODADDR@)		; # chg
	;LD (PSP_+1),SP
	ld	(@PSP@),sp		; # chg
	LD SP,HL
	POP HL
	ADD HL,DE
	LD B,H
	LD C,L
	POP HL
	ADD HL,DE
	LD (@AdInPtB@),HL		; # chg
	POP HL
	ADD HL,DE
	LD (@AdInPtC@),HL		; # chg
PSP_:	;LD SP,$3131			; ## AUTOMODIF-9 (@PSP@) Mas Pila
	ld	sp,(@PSP@)		; # chg
PL1A:	LD IX,ChanA+12
	CALL PTDECOD
	LD (@AdInPtA@),BC		; # chg

PL1B:	LD HL,ChanB+CHNPRM_NtSkCn
	DEC (HL)
	JR NZ,PL1C
	LD IX,ChanB+12
AdInPtB:	;equ $+1			; ## AUTOMODIF-10 (@AdInPtB@)
	;LD BC,$0101
	ld	bc,(@ADINPTB@)		; # chg
	CALL PTDECOD
	LD (@AdInPtB@),BC		; # chg

PL1C:	LD HL,ChanC+CHNPRM_NtSkCn
	DEC (HL)
	JR NZ,PL1D
	LD IX,ChanC+12
AdInPtC:	;equ $+1			; ## AUTOMODIF-11 (@AdInPtC@)
	;LD BC,$0101
	ld	bc,(@ADINPTC@)		; # chg
	CALL PTDECOD
	LD (@AdInPtC@),BC

Delay:	;equ $+1				; ## AUTOMODIF-12 (@Delay@)
PL1D:	;LD A,$3E
	ld	a,(@DELAY@)		; # chg
	LD (DelyCnt),A

PL2:	LD IX,ChanA
	LD HL,(AYREGS+AR_TonA)
	CALL CHREGS
	LD (AYREGS+AR_TonA),HL
	LD A,(Ampl)
	LD (AYREGS+AR_AmplA),A
	LD IX,ChanB
	LD HL,(AYREGS+AR_TonB)
	CALL CHREGS
	LD (AYREGS+AR_TonB),HL
	LD A,(Ampl)
	LD (AYREGS+AR_AmplB),A
	LD IX,ChanC
	LD HL,(AYREGS+AR_TonC)
	CALL CHREGS
	LD (AYREGS+AR_TonC),HL

	LD HL,(Ns_Base_AddToNs)
	LD A,H
	ADD A,L
	LD (AYREGS+AR_Noise),A

AddToEn: ;equ $+1			; ## AUTOMODIF-13 (@AddToEn@) ; Envelopes??
	;LD A,$3E
	ld	a,(@ADDTOEN@)		; # chg
	LD E,A
	ADD A,A
	SBC A,A
	LD D,A
	LD HL,(EnvBase)
	ADD HL,DE
	LD DE,(CurESld)
	ADD HL,DE
	LD (AYREGS+AR_Env),HL

	XOR A
	LD HL,CurEDel
	OR (HL)
	JR Z,ROUT_A0
	DEC (HL)
	JR NZ,ROUT
Env_Del:	;equ $+1			; ## AUTOMODIF-14 (@Env_Del@) ; Envelope??
	;LD A,$3E
	ld	a,(@ENVDEL@)		; # chg
	LD (HL),A
ESldAdd:	;equ $+1			; ## AUTOMODIF-15 (@ESldAdd@) ; Envelope??
	;LD HL,$2121
	ld	hl,(@ESLDADD@)		; # chg
	ADD HL,DE
	LD (CurESld),HL


~Rout:
ROUT:	XOR A

ROUT_A0:push	af
	ld	a,(AYREGS+7)			; # FIJA BUG DEL MIXER
	and	00111111b
	or	10000000b
	ld	(AYREGS+7),a
	pop	af

	LD C,$A0
	LD HL,AYREGS
LOUT:	OUT (C),A
	INC C
	OUTI 
	DEC C
	INC A
	CP 13
	JR NZ,LOUT
	OUT (C),A
	LD A,(HL)
	AND A
	RET M
	INC C
	OUT (C),A
	RET

NT_DATA:	db (T_NEW_0-T1_)*2
	db TCNEW_0-T_
	db (T_OLD_0-T1_)*2+1
	db TCOLD_0-T_
	db (T_NEW_1-T1_)*2+1
	db TCNEW_1-T_
	db (T_OLD_1-T1_)*2+1
	db TCOLD_1-T_
	db (T_NEW_2-T1_)*2
	db TCNEW_2-T_
	db (T_OLD_2-T1_)*2
	db TCOLD_2-T_
	db (T_NEW_3-T1_)*2
	db TCNEW_3-T_
	db (T_OLD_3-T1_)*2
	db TCOLD_3-T_

T_:

TCOLD_0:	db $00+1,$04+1,$08+1,$0A+1,$0C+1,$0E+1,$12+1,$14+1
	db $18+1,$24+1,$3C+1,0
TCOLD_1:	db $5C+1,0
TCOLD_2:	db $30+1,$36+1,$4C+1,$52+1,$5E+1,$70+1,$82,$8C,$9C
	db $9E,$A0,$A6,$A8,$AA,$AC,$AE,$AE,0
TCNEW_3:	db $56+1
TCOLD_3:	db $1E+1,$22+1,$24+1,$28+1,$2C+1,$2E+1,$32+1,$BE+1,0
TCNEW_0:	db $1C+1,$20+1,$22+1,$26+1,$2A+1,$2C+1,$30+1,$54+1
	db $BC+1,$BE+1,0
TCNEW_1: equ TCOLD_1
TCNEW_2:	db $1A+1,$20+1,$24+1,$28+1,$2A+1,$3A+1,$4C+1,$5E+1
	db $BA+1,$BC+1,$BE+1,0

EMPTYSAMORN: ;equ $-1			; ## AUTOMODIF-16 (@EMPTYSAMORN@)
	db 0,1,0,$90 ;delete $90 if you don't need default sample  ; # pongo el 0 aqui


; - tabla NOTAS, va en ROM pero podria ir en ram y asi tener todas packed y luego depack la que quiera -
		
NT_:	;Note table 2 (if you use another in Vortex Tracker II copy it and paste
	;it from TABLES.TXT)

	dw $0D10,$0C55,$0BA4,$0AFC,$0A5F,$09CA,$093D,$08B8,$083B,$07C5,$0755,$06EC
	dw $0688,$062A,$05D2,$057E,$052F,$04E5,$049E,$045C,$041D,$03E2,$03AB,$0376
	dw $0344,$0315,$02E9,$02BF,$0298,$0272,$024F,$022E,$020F,$01F1,$01D5,$01BB
	dw $01A2,$018B,$0174,$0160,$014C,$0139,$0128,$0117,$0107,$00F9,$00EB,$00DD
	dw $00D1,$00C5,$00BA,$00B0,$00A6,$009D,$0094,$008C,$0084,$007C,$0075,$006F
	dw $0069,$0063,$005D,$0058,$0053,$004E,$004A,$0046,$0042,$003E,$003B,$0037
	dw $0034,$0031,$002F,$002C,$0029,$0027,$0025,$0023,$0021,$001F,$001D,$001C
	dw $001A,$0019,$0017,$0016,$0015,$0014,$0012,$0011,$0010,$000F,$000E,$000D

; # Volume table original, a copiar a RAM

VTCopy:	;Volume Table for Vortex Tracker II/PT3.5
	db $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
	db $00,$00,$00,$00,$00,$00,$00,$00,$01,$01,$01,$01,$01,$01,$01,$01
	db $00,$00,$00,$00,$01,$01,$01,$01,$01,$01,$01,$01,$02,$02,$02,$02
	db $00,$00,$00,$01,$01,$01,$01,$01,$02,$02,$02,$02,$02,$03,$03,$03
	db $00,$00,$01,$01,$01,$01,$02,$02,$02,$02,$03,$03,$03,$03,$04,$04
	db $00,$00,$01,$01,$01,$02,$02,$02,$03,$03,$03,$04,$04,$04,$05,$05
	db $00,$00,$01,$01,$02,$02,$02,$03,$03,$04,$04,$04,$05,$05,$06,$06
	db $00,$00,$01,$01,$02,$02,$03,$03,$04,$04,$05,$05,$06,$06,$07,$07
	db $00,$01,$01,$02,$02,$03,$03,$04,$04,$05,$05,$06,$06,$07,$07,$08
	db $00,$01,$01,$02,$02,$03,$04,$04,$05,$05,$06,$07,$07,$08,$08,$09
	db $00,$01,$01,$02,$03,$03,$04,$05,$05,$06,$07,$07,$08,$09,$09,$0A
	db $00,$01,$01,$02,$03,$04,$04,$05,$06,$07,$07,$08,$09,$0A,$0A,$0B
	db $00,$01,$02,$02,$03,$04,$05,$06,$06,$07,$08,$09,$0A,$0A,$0B,$0C
	db $00,$01,$02,$03,$03,$04,$05,$06,$07,$08,$09,$0A,$0A,$0B,$0C,$0D
	db $00,$01,$02,$03,$04,$05,$06,$07,$07,$08,$09,$0A,$0B,$0C,$0D,$0E
	db $00,$01,$02,$03,$04,$05,$06,$07,$08,$09,$0A,$0B,$0C,$0D,$0E,$0F
