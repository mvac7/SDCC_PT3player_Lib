;
; - - - - - - - - - - - - - - - - - -  PT3  Replayer  - - - - - - - - - - - - - - - - - -
;

;Vortex Tracker II v1.0 PT3 player for MSX
;Adapted by Alfonso D. C. aka Dioniso <dioniso072@yahoo.es>
;09-Jan-05
;From:
;Vortex Tracker II v1.0 PT3 player for ZX Spectrum
;(c)2004 S.V.Bulba <vorobey@mail.khstu.ru> http://bulba.at.kz


;  In this replayer:
; -No version detection (just for Vortex Tracker II and PT3.5).
; -No frequency table decompression (default is number 2). - Coger tabla segun quiera, en fichero aparte
; -No volume table decompression (Vortex Tracker II/PT3.5 volume table used).


; Warning!!! Delete 100 first bytes (header) from your PT3 module or add 100 to data addres. <<<<<<<<<<<<<<<<<<<<<< ##################### WARNING!!!

; - New added-

; Usable desde ROM (solo tiene en RAM area de trabajo, lo minimo posible).


;  --- ChannelsVars ---  (esto son equs, no posiciones RAM)

; - struc CHNPRM -



	.area _DATA


  .area _CODE


initVARs = 0xE200       ;256B

VARS  = initVARs 
ChanA = VARS        ;rb CHNPRM_Size		; 29 bytes
ChanB =	ChanA + 29	;rb CHNPRM_Size		; 29 bytes
ChanC = ChanB + 29	;rb CHNPRM_Size		; 29 bytes

; - GlobalVars -

DelyCnt = ChanC + 29  ;	rb 1			; 1
CurESld = DelyCnt + 1 ;	rw 1			; 2	
CurEDel = CurESld + 2 ;	rb 1			; 1

Ns_Base_AddToNs = CurEDel + 1
Ns_Base = Ns_Base_AddToNs ;	rb 1			; 1
AddToNs = Ns_Base + 1    ;	rb 1			; 1

VT_ = AddToNs + 1  ;256B
AYREGS = VT_       ;14B ?
_PT3WRK = VT_+256  ;rb	32

;total mem = 0x17D

; ------------------------------------------------------------------------------
; Constants

;ChannelsVars
;struc	CHNPRM
;reset group
CHNPRM_PsInOr = 0	 ;RESB 1
CHNPRM_PsInSm = 1	 ;RESB 1
CHNPRM_CrAmSl = 2	 ;RESB 1
CHNPRM_CrNsSl = 3	 ;RESB 1
CHNPRM_CrEnSl = 4	 ;RESB 1
CHNPRM_TSlCnt = 5	 ;RESB 1
CHNPRM_CrTnSl = 6	 ;RESW 1
CHNPRM_TnAcc  = 8	 ;RESW 1
CHNPRM_COnOff = 10 ;RESB 1
;reset group

CHNPRM_OnOffD = 11 ;RESB 1

;IX for PTDECOD here (+12)
CHNPRM_OffOnD = 12 ;RESB 1
CHNPRM_OrnPtr = 13 ;RESW 1
CHNPRM_SamPtr = 15 ;RESW 1
CHNPRM_NNtSkp = 17 ;RESB 1
CHNPRM_Note   = 18 ;RESB 1
CHNPRM_SlToNt = 19 ;RESB 1
CHNPRM_Env_En = 20 ;RESB 1
CHNPRM_Flags  = 21 ;RESB 1

;Enabled - 0,SimpleGliss - 2
CHNPRM_TnSlDl = 22 ;RESB 1
CHNPRM_TSlStp = 23 ;RESW 1
CHNPRM_TnDelt = 25 ;RESW 1
CHNPRM_NtSkCn = 27 ;RESB 1
CHNPRM_Volume = 28 ;RESB 1
CHNPRM_Size   = 29 ;RESB 1
; endstruc

;- struc AR -
AR_TonA  = 0	;RESW 1
AR_TonB  = 2	;RESW 1
AR_TonC  = 4	;RESW 1
AR_Noise = 6	;RESB 1
AR_Mixer = 7	;RESB 1
AR_AmplA = 8	;RESB 1
AR_AmplB = 9	;RESB 1
AR_AmplC = 10	;RESB 1
AR_Env   = 11	;RESW 1
AR_EnvTp = 13	;RESB 1
;endstruc


EnvBase = VT_+14

T1_ = VT_+16 ;Tone tables data depacked here

T_OLD_1 = T1_
T_OLD_2 = T_OLD_1+24
T_OLD_3 = T_OLD_2+24
T_OLD_0 = T_OLD_3+2
T_NEW_0 = T_OLD_0
T_NEW_1 = T_OLD_1
T_NEW_2 = T_NEW_0+24
T_NEW_3 = T_OLD_3

Ampl = AYREGS+AR_AmplC

VAR0END = VT_+16 ;INIT zeroes from VARS to VAR0END-1


; --- Workarea --- (apunta a RAM que estaba antes en codigo automodificable)

; -El byte de estado en SETUP deberia ser algo asi (CH enable/disable no esta aun)
;|EP|0|0|0|CH3|CH2|CH1|LP|

;LP: Loop enable/disable. A 1 si queremos que el tema suene solo una vez. 
;EP: End point. A 1 cada vez que el tema acaba. 
;CH1-CH3: Channel enable/disable. A 1 si no queremos que suene el canal. (AUN  NO VA!!)

SETUP     =	_PT3WRK 	  ; set bit0 to 1, if you want to play without LOOPING
					              ; bit7 is set each time, when loop point is passed
CrPsPtr   =	_PT3WRK+1  	; 2 bytes POSICION CURSOR EN PATTERN
_MODADDR =	_PT3WRK+3	  ; 2 bytes dir musica guardada
_MDADDR2 =	_PT3WRK+5	  ; 2 bytes dir musica guardada 2 (? Creo que puedo usar el mismo siempre)
_SAMPTRS =	_PT3WRK+7	  ; 2 bytes sample info?
_PDSP    =	_PT3WRK+9	  ; 2 bytes pilasave
_ORNPTRS =	_PT3WRK+11	; 2 bytes Ornament pattern
_CSP     =	_PT3WRK+13	; 2 bytes pilsave2 (? puede usarse solo 1 tb??)
_ADINPTA =	_PT3WRK+15	; 2 bytes play data pattern
_ADINPTB =	_PT3WRK+17	; 2 bytes play data
_LPOSPTR =	_PT3WRK+19	; 2 bytes Pos Ptr
_PATSPTR =	_PT3WRK+21	; 2 bytes Pat Ptr
_PSP     =	_PT3WRK+23	; 2 bytes pilsave3 (? idem usar solo 1??)
_ADINPTC =	_PT3WRK+25	; 2 bytes play data
_DELAY   =	_PT3WRK+27	; 1 byte delay
_ADDTOEN =	_PT3WRK+28	; 1 byte Envelope data (No cal ya que no usa Envs??)
_ENVDEL  =	_PT3WRK+29	; 1 byte Envelope data (idem)
_ESLDADD =	_PT3WRK+30	; 2 bytes Envelope data (idem)
	

; ================================================ REPLAYER =================================================

; PT3 JP Table (podria incluso ahorrarse)

;_Pt3Init::	jp	INIT		; inicializa musica
;_PT3Play::	jp	PLAY		; playea
;_PT3Stop::	jp	MUTE		; para

; -- replayer --

CHECKLP:	
  LD   HL,#SETUP
	SET  7,(HL)
	BIT  0,(HL)
	RET  Z
	POP  HL
	LD   HL,#DelyCnt
	INC  (HL)
	LD   HL,#ChanA+CHNPRM_NtSkCn
	INC  (HL)

  
MUTE:
_PT3Stop::	
  XOR  A
	LD   H,A
	LD   L,A
	LD   (#AYREGS+AR_AmplA),A
	LD   (#AYREGS+AR_AmplB),HL
	JP   ROUT_A0


; void PT3Init(unsigned int,char)
_PT3Init::
  push IX
  ld   IX,#0
  add  IX,SP
  
  ld   HL,#SETUP
  ld   A,6(IX)
  or   A
  jr   NZ,SongLoop
  set  0,(HL)  ;not loop
  jr   initSong
  
SongLoop:  
  res  0,(HL)  ;loop

initSong:  
  ld   L,4(IX)
  ld   H,5(IX)
  call playerINIT
  
  pop  IX
  ret
  


;-------------------------------------------------------------------------------
_PT3Play::   
  push IX
  
  call PLAY
  
  pop  IX
  ret


;void PT3Loop(char)
;_PT3Loop::
;  push IX
;  ld   HL,#SETUP
;  ld   A,4(IX)
;  or   A
;  jr   NZ,SongLoop
;  set  0,(HL)  ;not loop
;  pop  IX
;  ret
;SongLoop:  
;  res  0,(HL)  ;loop
;  pop  IX
;  ret


; ------------------------------------------------------------------------------
playerINIT::
	;HL - AddressOfModule-100
	;ld hl,MDLADDR-100	; ## esto ahora se da como entrada al llamar a rutina
				; asi que la musica puede ir en cualquier lado ROM o RAM

	or	 A		; # added
	ld	 DE,#100		; # added
	sbc	 HL,DE		; # added

	LD   (#_MODADDR),HL	; # chg
	LD   (#_MDADDR2),HL	; # chg
	PUSH HL
	LD   DE,#100
	ADD  HL,DE
	LD   A,(HL)
	LD   (#_DELAY),A		; # chg
	PUSH HL
	POP  IX      ; <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<------------------------------- ???
	ADD  HL,DE
	LD   (#CrPsPtr),HL
	LD   E,102-100(IX)
	ADD  HL,DE
	INC  HL
	LD   (#_LPOSPTR),HL	; # chg
	POP  DE      ; <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<------------------------------- ???
	LD   L,103-100(IX)
	LD   H,104-100(IX)
	ADD  HL,DE
	LD   (#_PATSPTR),HL	; # chg
	LD   HL,#169
	ADD  HL,DE
	LD   (#_ORNPTRS),HL	; # chg
	LD   HL,#105
	ADD  HL,DE
	LD   (#_SAMPTRS),HL	; # chg
	LD   HL,#SETUP
	RES  7,(HL)

	ld	 HL,#VTcopy	; # added
	ld	 DE,#VT_		; # copia tabla de volumen		
	ld	 BC,#256
	ldir
	xor	 A
	
	LD   HL,#VARS
	LD   (HL),A
	LD   DE,#VARS+1
	LD   BC,#VAR0END - VARS -1
	LDIR
	INC  A
	LD   (#DelyCnt),A
	LD   HL,#0xF001	;H - CHNPRM_Volume, L - CHNPRM_NtSkCn
	LD   (#ChanA+CHNPRM_NtSkCn),HL
	LD   (#ChanB+CHNPRM_NtSkCn),HL
	LD   (#ChanC+CHNPRM_NtSkCn),HL

	LD   HL,#EMPTYSAMORN
	LD   (#_ADINPTA),HL ;ptr to zero  ; # chg
	LD   (#ChanA+CHNPRM_OrnPtr),HL ;ornament 0 is "0,1,0"
	LD   (#ChanB+CHNPRM_OrnPtr),HL ;in all versions from
	LD   (#ChanC+CHNPRM_OrnPtr),HL ;3.xx to 3.6x and VTII

	LD   (#ChanA+CHNPRM_SamPtr),HL ;S1 There is no default
	LD   (#ChanB+CHNPRM_SamPtr),HL ;S2 sample in PT3, so, you
	LD   (#ChanC+CHNPRM_SamPtr),HL ;S3 can comment S1,2,3; see
				    ;also EMPTYSAMORN comment
	ret
;  JP   ROUT_A0   ;<------------------------------------------------------------ aorante, no ha de empezar a sonar en la inicializacion
  
  
  

;pattern decoder
PD_OrSm:	
  LD   -12+CHNPRM_Env_En(IX),#0
	CALL SETORN
	LD   A,(BC)
	INC  BC
	RRCA

PD_SAM:	
  ADD  A,A
PD_SAM_:	
  LD   E,A
	LD   D,#0
SamPtrs: ;equ $+1			; ## AUTOMODIF-1 !! (_SAMPTRS)
	;LD HL,$2121
	ld	 HL,(#_SAMPTRS)		; # chg
	ADD  HL,DE
	LD   E,(HL)
	INC  HL
	LD   D,(HL)
MODADDR: ;equ $+1			; ## AUTOMODIF-2 !! (_MODADDR)
	;LD HL,$2121
	ld	 HL,(#_MODADDR)		; # chg
	ADD  HL,DE
	LD   -12+CHNPRM_SamPtr(IX),L
	LD   -12+CHNPRM_SamPtr+1(IX),H
	JR   PD_LOOP

PD_VOL:	
  RLCA
	RLCA
	RLCA
	RLCA
	LD   -12+CHNPRM_Volume(IX),A
	JR   PD_LP2
	
PD_EOff:	
  LD   -12+CHNPRM_Env_En(IX),A
	LD   -12+CHNPRM_PsInOr(IX),A
	JR   PD_LP2

PD_SorE:	
  DEC  A
	JR   NZ,PD_ENV
	LD   A,(BC)
	INC  BC
	LD   -12+CHNPRM_NNtSkp(IX),A
	JR   PD_LP2

PD_ENV:	
  CALL SETENV
	JR   PD_LP2

PD_ORN:	
  CALL SETORN
	JR   PD_LOOP
       
PD_ESAM:	
  LD   -12+CHNPRM_Env_En(IX),A
	LD   -12+CHNPRM_PsInOr(IX),A
	CALL NZ,SETENV
	LD   A,(BC)
	INC  BC
	JR   PD_SAM_

PTDECOD: 
  LD   A,-12+CHNPRM_Note(IX)
	LD   (#PrNote+1),A

PD_LOOP:	
  ld   DE,#0x2010
PD_LP2:	
  ld   A,(BC)
	inc  BC
	ADD  A,E
	JR   C,PD_OrSm
	ADD  A,D
	JR   Z,PD_FIN
	JR   C,PD_SAM
	ADD  A,E
	JR   Z,PD_REL
	JR   C,PD_VOL
	ADD  A,E
	JR   Z,PD_EOff
	JR   C,PD_SorE
	ADD  A,#96
	JR   C,PD_NOTE
	ADD  A,E
	JR   C,PD_ORN
	ADD  A,D
	JR   C,PD_NOIS
	ADD  A,E
	JR   C,PD_ESAM
	ADD  A,A
	LD   E,A
	
	PUSH DE
	LD   DE,#0xDF20
	LD   HL,#SPCCOMS	
	ADD  HL,DE
	POP  DE
	
	ADD  HL,DE
	LD   E,(HL)
	INC  HL
	LD   D,(HL)
	PUSH DE
	JR   PD_LOOP

PD_NOIS:	
  LD   (#Ns_Base),A
	JR   PD_LP2

PD_REL:	
  RES  0,-12+CHNPRM_Flags(IX)
	JR   PD_RES
	
PD_NOTE:	
  ld   -12+CHNPRM_Note(IX),A
	SET  0,-12+CHNPRM_Flags(IX)
	XOR  A

PD_RES:	
  ;LD (PDSP_+1),SP			; ## AUTOMODIF-3 (_PDSP) PilaSave !!
	ld	 (#_PDSP),SP		; # chg
	LD   SP,IX
	LD   H,A
	LD   L,A
	PUSH HL
	PUSH HL
	PUSH HL
	PUSH HL
	PUSH HL
	PUSH HL
PDSP_:	
  ;LD SP,$3131			; ## AUTOMODIF-3 (@PDSP_@) PilaSave !!
	ld	SP,(#_PDSP)		; # chg

PD_FIN:	
  ld   A,-12+CHNPRM_NNtSkp(IX)
	ld   -12+CHNPRM_NtSkCn(IX),A
	ret

C_PORTM: 
  RES  2,-12+CHNPRM_Flags(IX)
	LD   A,(BC)
	INC  BC
;SKIP PRECALCULATED TONE DELTA (BECAUSE
;CANNOT BE RIGHT AFTER PT3 COMPILATION)
	INC  BC
	INC  BC
	LD   -12+CHNPRM_TnSlDl(IX),A
	LD   -12+CHNPRM_TSlCnt(IX),A
	LD   DE,#NT_
	LD   A,-12+CHNPRM_Note(IX)
	LD   -12+CHNPRM_SlToNt(IX),A
	ADD  A,A
	LD   L,A
	LD   H,#0
	ADD  HL,DE
	LD   A,(HL)
	INC  HL
	LD   H,(HL)
	LD   L,A
	PUSH HL
PrNote:	
  LD   A,#0x3E
	LD   -12+CHNPRM_Note(IX),A
	ADD  A,A
	LD   L,A
	LD   H,#0
	ADD  HL,DE
	LD   E,(HL)
	INC  HL
	LD   D,(HL)
	POP  HL
	SBC  HL,DE
	LD   -12+CHNPRM_TnDelt(IX),L
	LD   -12+CHNPRM_TnDelt+1(IX),H
	LD   E,-12+CHNPRM_CrTnSl(IX)
	LD   D,-12+CHNPRM_CrTnSl+1(IX)

OLDPRTM:	
  LD   A,(BC) ;SIGNED TONE STEP
	INC  BC
	EX   AF,AF
	LD   A,(BC)
	INC  BC
	AND  A
	JR   Z,NOSIG
	EX   DE,HL
NOSIG:	
  SBC  HL,DE
	JP   P,SET_STP
	CPL
	EX   AF,AF
	NEG
	EX   AF,AF
SET_STP:	
  LD   -12+CHNPRM_TSlStp+1(IX),A
	EX   AF,AF
	ld   -12+CHNPRM_TSlStp(IX),A
	ld   -12+CHNPRM_COnOff(IX),#0
	ret

C_GLISS:	
  SET  2,-12+CHNPRM_Flags(IX)
	LD   A,(BC)
	INC  BC
	LD  -12+CHNPRM_TnSlDl(IX),A
	LD  -12+CHNPRM_TSlCnt(IX),A
	LD   A,(BC)
	INC  BC
	EX   AF,AF
	LD   A,(BC)
	INC  BC
	JR   SET_STP

C_SMPOS:	
  LD   A,(BC)
	INC  BC
	LD   -12+CHNPRM_PsInSm(IX),A
	RET

C_ORPOS:	
  LD   A,(BC)
	INC  BC
	LD   -12+CHNPRM_PsInOr(IX),A
	RET

C_VIBRT:	
  LD   A,(BC)
	INC  BC
	LD   -12+CHNPRM_OnOffD(IX),A
	LD   -12+CHNPRM_COnOff(IX),A
	LD   A,(BC)
	INC  BC
	LD   -12+CHNPRM_OffOnD(IX),A
	XOR  A
	LD   -12+CHNPRM_TSlCnt(IX),A
	LD   -12+CHNPRM_CrTnSl(IX),A
	LD   -12+CHNPRM_CrTnSl+1(IX),A
	RET

C_ENGLS:	
  LD   A,(BC)
	INC  BC
	LD   (#_ENVDEL),A			; # chg
	LD   (#CurEDel),A
	LD   A,(BC)
	INC  BC
	LD   L,A
	LD   A,(BC)
	INC  BC
	LD   H,A
	LD   (#_ESLDADD),HL		; # chg
	RET

C_DELAY:	
  LD   A,(BC)
	INC  BC
	LD   (#_DELAY),A			; # chg
	RET
	
SETENV:	
  LD   -12+CHNPRM_Env_En(IX),E
	LD   (#AYREGS+AR_EnvTp),A
	LD   A,(BC)
	INC  BC
	LD   H,A
	LD   A,(BC)
	INC  BC
	LD   L,A
	LD   (#EnvBase),HL
	XOR  A
	LD   -12+CHNPRM_PsInOr(IX),A
	LD   (#CurEDel),A
	LD   H,A
	LD   L,A
	LD   (#CurESld),HL
C_NOP:	
  RET

SETORN:	
  ADD  A,A
	LD   E,A
	LD   D,#0
	LD   -12+CHNPRM_PsInOr(IX),D
OrnPtrs:	
  ;equ $+1			; ## AUTOMOFIF-4 (_ORNPTRS) Ornaments
	;LD HL,$2121
	ld	 HL,(#_ORNPTRS)		; # chg
	ADD  HL,DE
	LD   E,(HL)
	INC  HL
	LD   D,(HL)
MDADDR2:
	;equ $+1			; ## AUTOMOFIF-1b (@MODADR2@) Ornaments
	;LD HL,$2121
	ld   HL,(#_MDADDR2)		; # chg
	ADD  HL,DE
	LD   -12+CHNPRM_OrnPtr(IX),L
	LD   -12+CHNPRM_OrnPtr+1(IX),H
	RET

;ALL 16 ADDRESSES TO PROTECT FROM BROKEN PT3 MODULES 

SPCCOMS: 
.dw C_NOP			  ; ## COMPROBAR Q NO SEA AUTOMODIF
.dw C_GLISS			; (parece que no lo toca nada)
.dw C_PORTM
.dw C_SMPOS
.dw C_ORPOS
.dw C_VIBRT
.dw C_NOP
.dw C_NOP
.dw C_ENGLS
.dw C_DELAY
.dw C_NOP
.dw C_NOP
.dw C_NOP
.dw C_NOP
.dw C_NOP
.dw C_NOP


CHREGS:	
  XOR  A
	LD   (#Ampl),A
	BIT   0,CHNPRM_Flags(IX)
	PUSH  HL
	JP    Z,CH_EXIT
	;LD (CSP_+1),SP			; ## APUNTA A CODIGO AUTOMODIF
	ld	 (#_CSP),sp		; # chg
	LD   L,CHNPRM_OrnPtr(IX)
	LD   H,CHNPRM_OrnPtr+1(IX)
	LD   SP,HL
	POP  DE
	LD   H,A
	LD   A,CHNPRM_PsInOr(IX)
	LD   L,A
	ADD  HL,SP
	INC  A
	CP   D
	JR   C,CH_ORPS
	LD   A,E
CH_ORPS:	
  LD   CHNPRM_PsInOr(IX),A
	LD   A,CHNPRM_Note(IX)
	ADD  A,(HL)
	JP   P,CH_NTP
	XOR  A
CH_NTP:	
  CP   #96
	JR   C,CH_NOK
	LD   A,#95
CH_NOK:	
  ADD  A,A
	EX   AF,AF
	LD   L,CHNPRM_SamPtr(IX)
	LD   H,CHNPRM_SamPtr+1(IX)
	LD   SP,HL
	POP  DE
	LD   H,#0
	LD   A,CHNPRM_PsInSm(IX)
	LD   B,A
	ADD  A,A
	ADD  A,A
	LD   L,A
	ADD  HL,SP
	LD   SP,HL
	LD   A,B
	INC  A
	CP   D
	JR   C,CH_SMPS
	LD   A,E
CH_SMPS:	
  LD   CHNPRM_PsInSm(IX),A
	POP  BC
	POP  HL
	LD   E,CHNPRM_TnAcc(IX)
	LD   D,CHNPRM_TnAcc+1(IX)
	ADD  HL,DE
	BIT  6,B
	JR   Z,CH_NOAC
	LD   CHNPRM_TnAcc(IX),L
	LD   CHNPRM_TnAcc+1(IX),H
CH_NOAC: 
  EX   DE,HL
	EX   AF,AF
	LD   L,A
	LD   H,#0
	LD   SP,#NT_
	ADD  HL,SP
	LD   SP,HL
	POP  HL
	ADD  HL,DE
	LD   E,CHNPRM_CrTnSl(IX)
	LD   D,CHNPRM_CrTnSl+1(IX)
	ADD  HL,DE
CSP_:	
;LD SP,$3131			; ## AUTOMODIF-5 (@CSP_@) Mas pila
	ld	 SP,(#_CSP)		; # chg
	EX   (SP),HL
	XOR  A
	OR   CHNPRM_TSlCnt(IX)
	JR   Z,CH_AMP
	DEC  CHNPRM_TSlCnt(IX)
	JR   NZ,CH_AMP
	LD   A,CHNPRM_TnSlDl(IX)
	LD   CHNPRM_TSlCnt(IX),A
	LD   L,CHNPRM_TSlStp(IX)
	LD   H,CHNPRM_TSlStp+1(IX)
	LD   A,H
	ADD  HL,DE
	LD   CHNPRM_CrTnSl(IX),L
	LD   CHNPRM_CrTnSl+1(IX),H
	BIT  2,CHNPRM_Flags(IX)
	JR   NZ,CH_AMP
	LD   E,CHNPRM_TnDelt(IX)
	LD   D,CHNPRM_TnDelt+1(IX)
	AND  A
	JR   Z,CH_STPP
	EX   DE,HL
CH_STPP: 
  SBC  HL,DE
	JP   M,CH_AMP
	LD   A,CHNPRM_SlToNt(IX)
	LD   CHNPRM_Note(IX),A
	XOR  A
	LD   CHNPRM_TSlCnt(IX),A
	LD   CHNPRM_CrTnSl(IX),A
	LD   CHNPRM_CrTnSl+1(IX),A
CH_AMP:	
  LD   A,CHNPRM_CrAmSl(IX)
	BIT  7,C
	JR   Z,CH_NOAM
	BIT  6,C
	JR   Z,CH_AMIN
	CP   #15
	JR   Z,CH_NOAM
	INC  A
	JR   CH_SVAM
CH_AMIN:	
  CP   #-15
	JR   Z,CH_NOAM
	DEC  A
CH_SVAM:	
  LD   CHNPRM_CrAmSl(IX),A
CH_NOAM:	
  LD   L,A
	LD   A,B
	AND  #15
	ADD  A,L
	JP   P,CH_APOS
	XOR  A
CH_APOS:	
  CP   #16
	JR   C,CH_VOL
	LD   A,#15
CH_VOL:	
  OR   CHNPRM_Volume(IX)
	LD   L,A
	LD   H,#0
	LD   DE,#VT_
	ADD  HL,DE
	LD   A,(HL)
CH_ENV:	
  BIT  0,C
	JR   NZ,CH_NOEN
	OR   CHNPRM_Env_En(IX)
CH_NOEN:	
  LD   (#Ampl),A
	BIT  7,B
	LD   A,C
	JR   Z,NO_ENSL
	RLA
	RLA
	SRA  A
	SRA  A
	SRA  A
	ADD  A,CHNPRM_CrEnSl(IX) ;SEE COMMENT BELOW
	BIT  5,B
	JR   Z,NO_ENAC
	LD   CHNPRM_CrEnSl(IX),A
NO_ENAC:	
	ld	 HL,#_ADDTOEN	; # chg
	ADD  A,(HL) ;BUG IN PT3 - NEED WORD HERE.
		   ;FIX IT IN NEXT VERSION?
	LD   (HL),A
	JR   CH_MIX
NO_ENSL: 
  RRA
	ADD  A,CHNPRM_CrNsSl(IX)
	LD   (#AddToNs),A
	BIT  5,B
	JR   Z,CH_MIX
	LD   CHNPRM_CrNsSl(IX),A
CH_MIX:	
  LD   A,B
	RRA
	AND  #0x48
CH_EXIT:	
  LD   HL,#AYREGS+AR_Mixer
	OR   (HL)
	RRCA
	LD   (HL),A
	POP  HL
	XOR  A
	OR   CHNPRM_COnOff(IX)
	RET  Z
	DEC  CHNPRM_COnOff(IX)
	RET  NZ
	XOR  CHNPRM_Flags(IX)
	LD   CHNPRM_Flags(IX),A
	RRA
	LD   A,CHNPRM_OnOffD(IX)
	JR   C,CH_ONDL
	LD   A,CHNPRM_OffOnD(IX)
CH_ONDL:	
  LD   CHNPRM_COnOff(IX),A
	RET




PLAY:
  XOR  A
	LD   (#_ADDTOEN),A
	LD   (#AYREGS+AR_Mixer),A
	DEC  A
	LD   (#AYREGS+AR_EnvTp),A
	LD   HL,#DelyCnt
	DEC  (HL)
	JP   NZ,PL2	; # jr nz cambiado por jp nz pq no llega
	LD   HL,#ChanA+CHNPRM_NtSkCn
	DEC  (HL)
	JR   NZ,PL1B
AdInPtA:	
;equ $+1			; ## AUTOMODIF-6 (_ADINPTA)
	;LD BC,$0101
	ld	 BC,(#_ADINPTA)		; # chg
	LD   A,(BC)
	AND  A
	JR   NZ,PL1A
	LD   D,A
	LD   (#Ns_Base),A
	LD   HL,(#CrPsPtr)
	INC  HL
	LD   A,(HL)
	INC  A
	JR   NZ,PLNLP
	CALL CHECKLP
LPosPtr:	
;equ $+1			; ## AUTOMODIF-7 (_LPOSPTR)
;LD HL,$2121
	ld	 HL,(#_LPOSPTR)		; # chg
	LD   A,(HL)
	INC  A
PLNLP:	
  LD   (#CrPsPtr),HL
	DEC  A
	ADD  A,A
	LD   E,A
	RL   D
PatsPtr:	
;equ $+1			; ## AUTOMODIF-8 (_PATSPTR)
	;LD HL,$2121
	ld   HL,(#_PATSPTR)		; # chg
	ADD  HL,DE
	LD   DE,(#_MODADDR)		; # chg
	;LD (PSP_+1),SP
	ld   (#_PSP),sp		; # chg
	LD   SP,HL
	POP  HL
	ADD  HL,DE
	LD   B,H
	LD   C,L
	POP  HL
	ADD  HL,DE
	LD   (#_ADINPTB),HL		; # chg
	POP  HL
	ADD  HL,DE
	LD   (#_ADINPTC),HL		; # chg
PSP_:	
;LD SP,$3131			; ## AUTOMODIF-9 (_PSP) Mas Pila
	ld   SP,(#_PSP)		; # chg
PL1A:	
  LD   IX,#ChanA+12
	CALL PTDECOD
	LD   (#_ADINPTA),BC		; # chg

PL1B:	
  LD   HL,#ChanB+CHNPRM_NtSkCn
	DEC  (HL)
	JR   NZ,PL1C
	LD   IX,#ChanB+12
AdInPtB:	
;equ $+1			; ## AUTOMODIF-10 (_ADINPTB)
	;LD BC,$0101
	ld   BC,(#_ADINPTB)		; # chg
	CALL PTDECOD
	LD   (#_ADINPTB),BC		; # chg

PL1C:	
  LD   HL,#ChanC+CHNPRM_NtSkCn
	DEC  (HL)
	JR   NZ,PL1D
	LD   IX,#ChanC+12
AdInPtC:	
;equ $+1			; ## AUTOMODIF-11 (_ADINPTC)
	;LD BC,$0101
	ld   BC,(#_ADINPTC)		; # chg
	CALL PTDECOD
	LD   (#_ADINPTC),BC

Delay:	
;equ $+1				; ## AUTOMODIF-12 (_DELAY)
PL1D:	
;LD A,$3E
	ld   A,(#_DELAY)		; # chg
	ld   (#DelyCnt),A

PL2:	
  LD   IX,#ChanA
	LD   HL,(#AYREGS+AR_TonA)
	CALL CHREGS
	LD   (#AYREGS+AR_TonA),HL
	LD   A,(#Ampl)
	LD   (#AYREGS+AR_AmplA),A
	LD   IX,#ChanB
	LD   HL,(#AYREGS+AR_TonB)
	CALL CHREGS
	LD   (#AYREGS+AR_TonB),HL
	LD   A,(#Ampl)
	LD   (#AYREGS+AR_AmplB),A
	LD   IX,#ChanC
	LD   HL,(#AYREGS+AR_TonC)
	CALL CHREGS
	LD   (#AYREGS+AR_TonC),HL

	LD   HL,(#Ns_Base_AddToNs)
	LD   A,H
	ADD  A,L
	LD   (#AYREGS+AR_Noise),A

AddToEn: 
;equ $+1			; ## AUTOMODIF-13 (_ADDTOEN) ; Envelopes??
	;LD A,$3E
	ld   A,(#_ADDTOEN)		; # chg
	LD   E,A
	ADD  A,A
	SBC  A,A
	LD   D,A
	LD   HL,(#EnvBase)
	ADD  HL,DE
	LD   DE,(#CurESld)
	ADD  HL,DE
	LD  (#AYREGS+AR_Env),HL

	XOR  A
	LD   HL,#CurEDel
	OR   (HL)
	JR   Z,ROUT_A0
	DEC  (HL)
	JR   NZ,ROUT
Env_Del:	
;equ $+1			; ## AUTOMODIF-14 (@Env_Del@) ; Envelope??
	;LD A,$3E
	ld   A,(#_ENVDEL)		; # chg
	LD   (HL),A
ESldAdd:	
;equ $+1			; ## AUTOMODIF-15 (_ESLDADD) ; Envelope??
	;LD HL,$2121
	ld   HL,(#_ESLDADD)		; # chg
	ADD  HL,DE
	LD   (#CurESld),HL


ROUT:	
  XOR  A

ROUT_A0:
  ; FIJA BUG DEL MIXER
  LD	 HL,#AYREGS+AR_Mixer
	set	 7,(HL)
	res	 6,(HL)
  
	ld   C,#0xA0
	ld   HL,#AYREGS
  
LOUT:	
  OUT  (C),A
	INC  C
	OUTI 
	DEC  C
	INC  A
	CP   #13
	JR   NZ,LOUT
	OUT  (C),A
	LD   A,(HL)
	AND  A
	RET  M
	INC  C
	OUT  (C),A
	RET

NT_DATA:
.db (T_NEW_0-T1_)*2
.db TCNEW_0-T_
.db (T_OLD_0-T1_)*2+1
.db TCOLD_0-T_
.db (T_NEW_1-T1_)*2+1
.db TCNEW_1-T_
.db (T_OLD_1-T1_)*2+1
.db TCOLD_1-T_
.db (T_NEW_2-T1_)*2
.db TCNEW_2-T_
.db (T_OLD_2-T1_)*2
.db TCOLD_2-T_
.db (T_NEW_3-T1_)*2
.db TCNEW_3-T_
.db (T_OLD_3-T1_)*2
.db TCOLD_3-T_

T_:

TCOLD_0:
.db 0x00+1,0x04+1,0x08+1,0x0A+1,0x0C+1,0x0E+1,0x12+1,0x14+1
.db 0x18+1,0x24+1,0x3C+1,0
TCOLD_1:
.db 0x5C+1,0
TCOLD_2:
.db 0x30+1,0x36+1,0x4C+1,0x52+1,0x5E+1,0x70+1,0x82,0x8C,0x9C
.db 0x9E,0xA0,0xA6,0xA8,0xAA,0xAC,0xAE,0xAE,0
TCNEW_3:
.db 0x56+1
TCOLD_3:
.db 0x1E+1,0x22+1,0x24+1,0x28+1,0x2C+1,0x2E+1,0x32+1,0xBE+1,0
TCNEW_0:	
.db 0x1C+1,0x20+1,0x22+1,0x26+1,0x2A+1,0x2C+1,0x30+1,0x54+1
.db 0xBC+1,0xBE+1,0
TCNEW_1 = TCOLD_1
TCNEW_2:
.db 0x1A+1,0x20+1,0x24+1,0x28+1,0x2A+1,0x3A+1,0x4C+1,0x5E+1
.db 0xBA+1,0xBC+1,0xBE+1,0

EMPTYSAMORN: ;equ $-1			; ## AUTOMODIF-16 (@EMPTYSAMORN@)
.db 0,1,0,0x90 ;delete $90 if you don't need default sample  ; # pongo el 0 aqui


; - tabla NOTAS, va en ROM pero podria ir en ram y asi tener todas packed y luego depack la que quiera -
		
NT_:	;Note table 2 (if you use another in Vortex Tracker II copy it and paste
	;it from TABLES.TXT)

.dw 0x0D10,0x0C55,0x0BA4,0x0AFC,0x0A5F,0x09CA,0x093D,0x08B8,0x083B,0x07C5,0x0755,0x06EC
.dw 0x0688,0x062A,0x05D2,0x057E,0x052F,0x04E5,0x049E,0x045C,0x041D,0x03E2,0x03AB,0x0376
.dw 0x0344,0x0315,0x02E9,0x02BF,0x0298,0x0272,0x024F,0x022E,0x020F,0x01F1,0x01D5,0x01BB
.dw 0x01A2,0x018B,0x0174,0x0160,0x014C,0x0139,0x0128,0x0117,0x0107,0x00F9,0x00EB,0x00DD
.dw 0x00D1,0x00C5,0x00BA,0x00B0,0x00A6,0x009D,0x0094,0x008C,0x0084,0x007C,0x0075,0x006F
.dw 0x0069,0x0063,0x005D,0x0058,0x0053,0x004E,0x004A,0x0046,0x0042,0x003E,0x003B,0x0037
.dw 0x0034,0x0031,0x002F,0x002C,0x0029,0x0027,0x0025,0x0023,0x0021,0x001F,0x001D,0x001C
.dw 0x001A,0x0019,0x0017,0x0016,0x0015,0x0014,0x0012,0x0011,0x0010,0x000F,0x000E,0x000D

; # Volume table original, a copiar a RAM

VTcopy:	;Volume Table for Vortex Tracker II/PT3.5
.db 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
.db 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01
.db 0x00,0x00,0x00,0x00,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x02,0x02,0x02,0x02
.db 0x00,0x00,0x00,0x01,0x01,0x01,0x01,0x01,0x02,0x02,0x02,0x02,0x02,0x03,0x03,0x03
.db 0x00,0x00,0x01,0x01,0x01,0x01,0x02,0x02,0x02,0x02,0x03,0x03,0x03,0x03,0x04,0x04
.db 0x00,0x00,0x01,0x01,0x01,0x02,0x02,0x02,0x03,0x03,0x03,0x04,0x04,0x04,0x05,0x05
.db 0x00,0x00,0x01,0x01,0x02,0x02,0x02,0x03,0x03,0x04,0x04,0x04,0x05,0x05,0x06,0x06
.db 0x00,0x00,0x01,0x01,0x02,0x02,0x03,0x03,0x04,0x04,0x05,0x05,0x06,0x06,0x07,0x07
.db 0x00,0x01,0x01,0x02,0x02,0x03,0x03,0x04,0x04,0x05,0x05,0x06,0x06,0x07,0x07,0x08
.db 0x00,0x01,0x01,0x02,0x02,0x03,0x04,0x04,0x05,0x05,0x06,0x07,0x07,0x08,0x08,0x09
.db 0x00,0x01,0x01,0x02,0x03,0x03,0x04,0x05,0x05,0x06,0x07,0x07,0x08,0x09,0x09,0x0A
.db 0x00,0x01,0x01,0x02,0x03,0x04,0x04,0x05,0x06,0x07,0x07,0x08,0x09,0x0A,0x0A,0x0B
.db 0x00,0x01,0x02,0x02,0x03,0x04,0x05,0x06,0x06,0x07,0x08,0x09,0x0A,0x0A,0x0B,0x0C
.db 0x00,0x01,0x02,0x03,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0A,0x0B,0x0C,0x0D
.db 0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E
.db 0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F
