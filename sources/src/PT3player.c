/* =============================================================================
   SDCC Vortex Tracker II PT3 player for MSX

   Version: 1.1.10 (05/10/2021)
   Architecture: MSX
   Format: C Object (SDCC .rel)
   Programming language: C and Z80 assembler

   Authors:
    - Vortex Tracker II v1.0 PT3 player for ZX Spectrum by S.V.Bulba 
      <vorobey@mail.khstu.ru> http://bulba.at.kz
    - (09-Jan-05) Adapted to MSX by Alfonso D. C. aka Dioniso 
      <dioniso072@yahoo.es>
    - Arrangements for MSX ROM: MSXKun/Paxanga soft > 
      http://paxangasoft.retroinvaders.com/
    - asMSX version: SapphiRe > http://www.z80st.es/
    - Adapted to SDCC: mvac7/303bcn > <mvac7303b@gmail.com>

   Description:
     Adaptation of the Vortex Tracker II PT3 Player for MSX to be used in 
     software development in C (SDCC).
     
     Requires the AY38910BF library  https://github.com/mvac7/SDCC_AY38910BF_Lib 
     
   History of versions:
    - 1.1.10 (05/10/2021) note table in Player_InitSong and Bug in Player_Resume
    - 1.1.9 (07/07/2021) Delete PlayAY() and AYREGS (need the AY38910BF) 
    - 1.1.8 (16/04/2021) add Player_IsEnd() function
    - 1.1.7 (24/03/2021)
    - 1.1.6 (15/02/2021) same function names in music libraries 
    - 1.1.5 (22/01/2021) Adjusted to work without the 100 Byte header
    - 1.1.4 (08/01/2021) PT3_Init and Bug #11 in loop
    - 1.1.3 (05/01/2021) PT3state, PT3_Loop, PT3_Pause and PT3_Resume
    - 1.1.2 (04/01/2021) assignment of frequency table memory address to NoteTable 
    - 1.1   (28/05/2019) Adaptation to SDCC of asMSX version by SapphiRe.
    - 1.0   (21/10/2016) Adaptation to SDCC of the ROM version by Kun.

In this replayer:

Dioniso version:
 - No version detection (just for Vortex Tracker II and PT3.5).
 - No frequency table decompression (default is number 2). 
   Coger tabla segun quiera, en fichero aparte
 - No volume table decompression (Vortex Tracker II/PT3.5 volume table used).


msxKun version:
 - Usable desde ROM (solo tiene en RAM area de trabajo, lo minimo posible).

SapphiRe version:
 This version of the replayer uses a fixed volume and note table, if you need a 
 different note table you can copy it from TABLES.TXT file, distributed with the
 original PT3 distribution. This version also allows the use of PT3 commands.

 PLAY and PSG WRITE routines seperated to allow independent calls


mvac7 version:
 Adaptation to C (SDCC) of the SapphiRe version.

============================================================================= */

#include "../include/AY38910BF.h"
#include "../include/PT3player.h"


//ChannelsVars
//struc	CHNPRM
//reset group
#define CHNPRM_PsInOr 0	 //RESB 1
#define CHNPRM_PsInSm 1	 //RESB 1
#define CHNPRM_CrAmSl 2	 //RESB 1
#define CHNPRM_CrNsSl 3	 //RESB 1
#define CHNPRM_CrEnSl 4	 //RESB 1
#define CHNPRM_TSlCnt 5	 //RESB 1
#define CHNPRM_CrTnSl 6	 //RESW 1
#define CHNPRM_TnAcc  8	 //RESW 1
#define CHNPRM_COnOff 10 //RESB 1
//reset group

#define CHNPRM_OnOffD 11 //RESB 1

//IX for PTDECOD here (+12)
#define CHNPRM_OffOnD 12 //RESB 1
#define CHNPRM_OrnPtr 13 //RESW 1
#define CHNPRM_SamPtr 15 //RESW 1
#define CHNPRM_NNtSkp 17 //RESB 1
#define CHNPRM_Note   18 //RESB 1
#define CHNPRM_SlToNt 19 //RESB 1
#define CHNPRM_Env_En 20 //RESB 1
#define CHNPRM_Flags  21 //RESB 1

//Enabled - 0,SimpleGliss - 2
#define CHNPRM_TnSlDl 22 //RESB 1
#define CHNPRM_TSlStp 23 //RESW 1
#define CHNPRM_TnDelt 25 //RESW 1
#define CHNPRM_NtSkCn 27 //RESB 1
#define CHNPRM_Volume 28 //RESB 1
#define CHNPRM_Size   29 //RESB 1
// endstruc




//VARS:
char ChanA[29]; //CHNPRM_Size
char ChanB[29];
char ChanC[29];			


char DelyCnt;
unsigned int CurESld;		
char CurEDel;

//Ns_Base_AddToNs:
char Ns_Base;		
char AddToNs;		

//AYREGS::
//char AYREGS[14];  //VT_[14];
unsigned int EnvBase;
char VTABLE[240];   //VAR0END




/*            
Switches: 1=ON; 0=OFF
- BIT 0 = ?
- BIT 1 = PLAYER ON/OFF
- BIT 2 = ?
- BIT 3 = ?
- BIT 4 = LOOP ON/OFF
- BIT 5 = ?
- BIT 6 = ? 
- BIT 7 = is END? 0=No, 1=Yes 
*/
char PT3_state;  //before called PT3_SETUP

/* --- Workarea --- (apunta a RAM que estaba antes en codigo automodificable)
 -El byte de estado en SETUP deberia ser algo asi (CH enable/disable no esta aun)
|EP|0|0|0|CH3|CH2|CH1|LP|

LP: Loop enable/disable. A 1 si queremos que el tema suene solo una vez. 
EP: End point. A 1 cada vez que el tema acaba. 
CH1-CH3: Channel enable/disable. A 1 si no queremos que suene el canal. (AUN  NO VA!!)
*/
//char PT3_SETUP;  set bit0 to 1, if you want to play without looping
//				       bit7 is set each time, when loop point is passed
           
unsigned int PT3_MODADDR;  //direccion datos canci�n
unsigned int PT3_CrPsPtr;  //POSICION CURSOR EN PATTERN
unsigned int PT3_SAMPTRS;  //sample info?
unsigned int PT3_OrnPtrs;  //Ornament pattern

unsigned int PT3_PDSP;     //pilasave
unsigned int PT3_CSP;      //pilsave2
unsigned int PT3_PSP;      //pilsave3
  
char PT3_PrNote;
unsigned int PT3_PrSlide;
  
unsigned int PT3_AdInPtA;  //play data pattern
unsigned int PT3_AdInPtB;  //play data
unsigned int PT3_AdInPtC;  //play data
  
unsigned int PT3_LPosPtr;  //Position Ptr?
unsigned int PT3_PatsPtr;  //Pat Ptr

char PT3_Delay;            //delay
char PT3_AddToEn;          //Envelope data (No cal ya que no usa Envs??)
char PT3_Env_Del;          //Envelope data (idem)
unsigned int PT3_ESldAdd;  //Envelope data (idem)


unsigned int NoteTable;   //note table memory address





	

// ================================== REPLAYER =================================



/* =============================================================================
 Player_Init
 Description: Initialize the Player
 Input:       -
 Output:      -
============================================================================= */
void Player_Init() __naked
{
__asm
  push IX
  
; Create Volume Table for Vortex Tracker II/PT3.5
; (c) Ivan Roshin, adapted by SapphiRe ---
  ld   HL,#0x11
  ld   D,H
  ld   E,H
  ld   IX,#_VTABLE  ;_VT_+16
  ld   B,#15
INITV1:	
  push HL
  add  HL,DE
  ex   DE,HL
  sbc  HL,HL
  ld   C,B 
  ld   B,#16
INITV2:	
  ld   A,L
  rla
  ld   A,H
  adc  A,#0
  ld   (IX),A
  inc  IX
  add  HL,DE
  djnz INITV2
  pop  HL
  ld   A,E
  cp   #0x77
  jr   NZ,INITV3
  inc  E
INITV3:	
  ld   B,C
  djnz INITV1

  
; Clear AY registers: 
  xor  A  
  ld   (#_PT3_state),A

  call _AY_Init   ;can be found in the AY38910BF library
  
;  LD   HL,#_AYREGS
;  LD   DE,#_AYREGS+1
;  LD   BC,#13
;  LD   (HL),A
;  LDIR  
  
  pop  IX
  ret
__endasm;
}




/* =============================================================================
 Player_Pause
 Description: Pause song playback
 Input:       -
 Output:      -
============================================================================= */
void Player_Pause() __naked
{
__asm
  LD   HL,#_PT3_state       
  RES  1,(HL)
   
MUTE:	
  XOR  A
  LD   (#_AYREGS+AY_AmpA),A
  LD   (#_AYREGS+AY_AmpB),A
  LD   (#_AYREGS+AY_AmpC),A
  
  //JP   _PlayAY                ;ROUT_A0
  ret
  
__endasm;
}
// ----------------------------------------------------------------------------- <<< END Player_Pause



/* =============================================================================
 Player_Resume
 Description: Resume song playback
 Input:       -
 Output:      -
============================================================================= */  	
void Player_Resume() __naked
{
__asm
   LD   HL,#_PT3_state
   BIT  7,(HL)      ;if the song is finished it does not activate the player
   ret  NZ       
   SET  1,(HL)      ;PLAYER ON
   
   ret
__endasm;
}
// ----------------------------------------------------------------------------- <<< END Player_Resume



/* -----------------------------------------------------------------------------
 Player_IsEnd
 Description: Indicates whether the song has finished playing
 Input:       -
 Output:      [char] 0 = No, 1 = Yes 
----------------------------------------------------------------------------- */
char Player_IsEnd() __naked
{
__asm
    LD   HL,#_PT3_state
    BIT  7,(HL)
    jr   Z,notFinished
    
    ;Yes
    ld   L,#1
    ret
    
notFinished:    
    ld   L,#0
    ret  
__endasm;
}





/* =============================================================================
 Player_Loop
 Description: Change loop state
 Input:       - (char or SWITCHER definition) 0=OFF ; 1=ON
 Output:      -
============================================================================= */
void Player_Loop(char loop) __naked
{
loop;
__asm 
  push IX
  ld   IX,#0
  add  IX,SP
    
  ld   HL,#_PT3_state
  
  ld   A,4(IX)
  
  or   A
  jr   NZ,LoopON
  res  4,(HL)  ;not loop
  
  pop  IX
  ret
  
LoopON:  
  set  4,(HL)  ;loop
  
  pop  IX
  ret
__endasm;
}





/* -----------------------------------------------------------------------------
 Player_InitSong
 Description: Initialize song
 Input: (unsigned int) Song data address. 
                       If the PT3 binary contains the header it will require 
                       subtracting 100 from this value.
        (unsigned int) Note Table address.
        (char) Loop - 0=off ; 1=on  (false = 0, true = 1));
 Output:      -
----------------------------------------------------------------------------- */
void Player_InitSong(unsigned int songADDR, unsigned int notetableADDR, char loop) __naked
{
songADDR;
notetableADDR;
loop;
__asm

  push IX
  ld   IX,#0
  add  IX,SP

  ld   L,6(IX)
  ld   H,7(IX)
  ld   (#_NoteTable),HL 
  
  xor  A
  ld   HL,#_PT3_state
  ld   (HL),A
  
  SET  1,(HL)      ;PLAYER ON  
  
  ld   A,8(IX)
  or   A
  jr   NZ,SongLoop
  res  4,(HL)  ;not loop
  jr   initSong
  
SongLoop:  
  set  4,(HL)  ;loop

initSong:  
  ld   L,4(IX)
  ld   H,5(IX)
  call playerINIT
  
  pop  IX
  ret


; HL - AddressOfModule
playerINIT:
  ld   DE,#100
  sbc  HL,DE
  
  LD   (#_PT3_MODADDR),HL
   
  PUSH HL 
  
  ;LD   DE,#100
  ADD  HL,DE
  
  LD   A,(HL)            ;+1B Delay
  LD   (#_PT3_Delay),A
  
  PUSH HL
  POP  IX                 ;<<-- IX = HL = PT3_MODADDR (+ 100 Header)
  
  ADD  HL,DE              ;PT3_MODADDR + 200
  LD   (#_PT3_CrPsPtr),HL ;Cr Ps Ptr data
  
  LD   E,2(IX)
  ADD  HL,DE
  INC  HL
  LD   (#_PT3_LPosPtr),HL  	
    
  POP  DE                 ;<<-- DE = _PT3_MODADDR
  
  LD   L,3(IX)
  LD   H,4(IX)
  ADD  HL,DE
  LD   (#_PT3_PatsPtr),HL
  
  LD   HL,#169
  ADD  HL,DE
  LD   (#_PT3_OrnPtrs),HL
  
  LD   HL,#105
  ADD  HL,DE
  LD   (#_PT3_SAMPTRS),HL
  
  ;LD   HL,#_PT3_state
  ;RES  7,(HL)
  
  
; --- INITIALIZE PT3 VARIABLES -------------------------------------------------  
  xor  A	
  LD   HL,#_ChanA     ;VARS
  LD   (HL),A
  LD   DE,#_ChanA+1   ;VARS+1
  LD   BC,#_VTABLE - _ChanA -1    ;_AYREGS - _ChanA -1
  LDIR
    
  INC  A
  LD   (#_DelyCnt),A
  LD   HL,#0xF001	;H - CHNPRM_Volume, L - CHNPRM_NtSkCn
  LD   (#_ChanA+CHNPRM_NtSkCn),HL
  LD   (#_ChanB+CHNPRM_NtSkCn),HL
  LD   (#_ChanC+CHNPRM_NtSkCn),HL
  
  LD   HL,#EMPTYSAMORN
  LD   (#_PT3_AdInPtA),HL ;ptr to zero  ; # chg
  LD   (#_ChanA+CHNPRM_OrnPtr),HL ;ornament 0 is "0,1,0"
  LD   (#_ChanB+CHNPRM_OrnPtr),HL ;in all versions from
  LD   (#_ChanC+CHNPRM_OrnPtr),HL ;3.xx to 3.6x and VTII
  
  LD   (#_ChanA+CHNPRM_SamPtr),HL ;S1 There is no default
  LD   (#_ChanB+CHNPRM_SamPtr),HL ;S2 sample in PT3, so, you
  LD   (#_ChanC+CHNPRM_SamPtr),HL ;S3 can comment S1,2,3; see
  			    ;also EMPTYSAMORN comment
  ret
  
__endasm;
}
// ----------------------------------------------------------------------------- END playerINIT






/* -----------------------------------------------------------------------------
 Player_Decode
 Description: Process the next step in the song sequence
 Input:       -
 Output:      - 
----------------------------------------------------------------------------- */
void Player_Decode() __naked
{
__asm   
  push IX
  
  call _PT3_PLAY
  
  pop  IX
  ret

_PT3_PLAY:  
  LD   HL,#_PT3_state       ;PLAY BIT 1 ON?
  BIT  1,(HL)
  RET  Z
 
  XOR  A
  LD   (#_PT3_AddToEn),A
  LD   (#_AYREGS+AY_Mixer),A
  DEC  A
  LD   (#_AYREGS+AY_EnvShape),A ;255 change in PlayAY()
  
  LD   HL,#_DelyCnt
  DEC  (HL)
  JP   NZ,PL2
   
  LD   HL,#_ChanA+CHNPRM_NtSkCn
  DEC  (HL)
  JR   NZ,PL1B
  
  ld   BC,(#_PT3_AdInPtA)
  LD   A,(BC)
  AND  A
  JR   NZ,PL1A
  
  LD   D,A
  LD   (#_Ns_Base),A
  LD   HL,(#_PT3_CrPsPtr)
  INC  HL
  LD   A,(HL)
  INC  A
  JR   NZ,PLNLP    ;if not end of song jump to PLNLP
  
  CALL CHECK_LOOP  ;CHECKLP
    
  ld   HL,(#_PT3_LPosPtr)
  LD   A,(HL)
  INC  A
  
PLNLP:	
  LD   (#_PT3_CrPsPtr),HL
  DEC  A
  ADD  A,A
  LD   E,A
  RL   D
  ld   HL,(#_PT3_PatsPtr)
  ADD  HL,DE
  LD   DE,(#_PT3_MODADDR)
  ld   (#_PT3_PSP),SP
  LD   SP,HL
  POP  HL
  ADD  HL,DE
  LD   B,H
  LD   C,L
  POP  HL
  ADD  HL,DE
  LD   (#_PT3_AdInPtB),HL
  POP  HL
  ADD  HL,DE
  LD   (#_PT3_AdInPtC),HL
  ld   SP,(#_PT3_PSP)
  
PL1A:	
  LD   IX,#_ChanA+12
  CALL PTDECOD
  LD   (#_PT3_AdInPtA),BC
  
PL1B:	
  LD   HL,#_ChanB+CHNPRM_NtSkCn
  DEC  (HL)
  JR   NZ,PL1C
  LD   IX,#_ChanB+12
  ld   BC,(#_PT3_AdInPtB)
  CALL PTDECOD
  LD   (#_PT3_AdInPtB),BC

PL1C:	
  LD   HL,#_ChanC+CHNPRM_NtSkCn
  DEC  (HL)
  JR   NZ,PL1D
  LD   IX,#_ChanC+12
  ld   BC,(#_PT3_AdInPtC)
  CALL PTDECOD
  LD   (#_PT3_AdInPtC),BC

PL1D:	
  ld   A,(#_PT3_Delay)
  ld   (#_DelyCnt),A

PL2:	
  LD   IX,#_ChanA
  LD   HL,(#_AYREGS+AY_ToneA)
  CALL CHREGS
  LD   (#_AYREGS+AY_ToneA),HL
  LD   A,(#_AYREGS+AY_AmpC)
  LD   (#_AYREGS+AY_AmpA),A
  LD   IX,#_ChanB
  LD   HL,(#_AYREGS+AY_ToneB)
  CALL CHREGS
  LD   (#_AYREGS+AY_ToneB),HL
  LD   A,(#_AYREGS+AY_AmpC)
  LD   (#_AYREGS+AY_AmpB),A
  LD   IX,#_ChanC
  LD   HL,(#_AYREGS+AY_ToneC)
  CALL CHREGS
  LD   (#_AYREGS+AY_ToneC),HL
  
  LD   HL,(#_Ns_Base)    ;Ns_Base_AddToNs
  LD   A,H
  ADD  A,L
  LD   (#_AYREGS+AY_Noise),A
  
  ld   A,(#_PT3_AddToEn)
  LD   E,A
  ADD  A,A
  SBC  A,A
  LD   D,A
  LD   HL,(#_EnvBase)
  ADD  HL,DE
  LD   DE,(#_CurESld)
  ADD  HL,DE
  LD  (#_AYREGS+AY_EnvPeriod),HL
  
  XOR  A
  LD   HL,#_CurEDel
  OR   (HL)
  JR   Z,IFSTOP
  ;RET  Z
  
  DEC  (HL)
  ;RET  NZ
  JR   NZ,IFSTOP
  
  LD   A,(#_PT3_Env_Del)
  LD   (HL),A
  LD   HL,(#_PT3_ESldAdd)
  ADD  HL,DE
  LD   (#_CurESld),HL
  
  RET

IFSTOP:  
  LD   HL,#_PT3_state
  BIT  1,(HL)   ; pause mode
  JP   Z,MUTE
  
  RET


;CHECKLP
CHECK_LOOP:	
  LD   HL,#_PT3_state
  ;SET  7,(HL)  ;loop control
  BIT  4,(HL)   ;loop bit 
  RET  NZ
  
;=0 - No loop
  RES  1,(HL)   ;set pause mode
  SET  7,(HL)   ;END song
  
;  POP  HL
;  LD   HL,#_DelyCnt
;  INC  (HL)
;  LD   HL,#_ChanA+CHNPRM_NtSkCn
;  INC  (HL)
  
  ret  
;  JP   MUTE



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
  ld   HL,(#_PT3_SAMPTRS)
  ADD  HL,DE
  LD   E,(HL)
  INC  HL
  LD   D,(HL)
  ld   HL,(#_PT3_MODADDR)
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
  LD   (#_PT3_PrNote),A           ;LD   (#PrNote+1),A
  LD   L,CHNPRM_CrTnSl-12(IX)
  LD   H,CHNPRM_CrTnSl+1-12(IX)
  LD  (#_PT3_PrSlide),HL

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
	
  LD   HL,#(SPCCOMS + 0xDF20)  ;LD HL,((SPCCOMS+$DF20) % 65536)
;	PUSH DE
;	LD   DE,#0xDF20
;	LD   HL,#SPCCOMS	
;	ADD  HL,DE
;	POP  DE	
  
  ADD  HL,DE
  LD   E,(HL)
  INC  HL
  LD   D,(HL)
  PUSH DE
  
  JR   PD_LOOP

PD_NOIS:	
  LD   (#_Ns_Base),A
  JR   PD_LP2

PD_REL:	
  RES  0,-12+CHNPRM_Flags(IX)
  JR   PD_RES
	
PD_NOTE:	
  ld   -12+CHNPRM_Note(IX),A
  SET  0,-12+CHNPRM_Flags(IX)
  XOR  A

PD_RES:	
  LD   (#_PT3_PDSP),SP
  LD   SP,IX
  LD   H,A
  LD   L,A
  PUSH HL
  PUSH HL
  PUSH HL
  PUSH HL
  PUSH HL
  PUSH HL
  LD   SP,(#_PT3_PDSP)

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
  LD   DE,(#_NoteTable)
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
  LD   A,(#_PT3_PrNote)            ;<--- LD   A,#0x3E
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
  LD   DE,(#_PT3_PrSlide)             ;<--- change to Kun version
  LD   -12+CHNPRM_CrTnSl(IX),E       ;<---
  LD   -12+CHNPRM_CrTnSl+1(IX),D     ;<---
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
  LD   (#_PT3_Env_Del),A
  LD   (#_CurEDel),A
  LD   A,(BC)
  INC  BC
  LD   L,A
  LD   A,(BC)
  INC  BC
  LD   H,A
  LD   (#_PT3_ESldAdd),HL
  RET

C_DELAY:	
  LD   A,(BC)
  INC  BC
  LD   (#_PT3_Delay),A
  RET
	
SETENV:	
  LD   -12+CHNPRM_Env_En(IX),E
  LD   (#_AYREGS+AY_EnvShape),A
  LD   A,(BC)
  INC  BC
  LD   H,A
  LD   A,(BC)
  INC  BC
  LD   L,A
  LD   (#_EnvBase),HL
  XOR  A
  LD   -12+CHNPRM_PsInOr(IX),A
  LD   (#_CurEDel),A
  LD   H,A
  LD   L,A
  LD   (#_CurESld),HL
  
C_NOP:	
  RET

SETORN:	
  ADD  A,A
  LD   E,A
  LD   D,#0
  LD   -12+CHNPRM_PsInOr(IX),D
  ld   HL,(#_PT3_OrnPtrs) 
  ADD  HL,DE
  LD   E,(HL)
  INC  HL
  LD   D,(HL)
  ld   HL,(#_PT3_MODADDR) 
  ADD  HL,DE
  LD   -12+CHNPRM_OrnPtr(IX),L
  LD   -12+CHNPRM_OrnPtr+1(IX),H
  RET





;-------------------------------------------------------------------------------
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
  LD   (#_AYREGS+AY_AmpC),A
  BIT   0,CHNPRM_Flags(IX)
  PUSH  HL
  JP    Z,CH_EXIT
  ld	 (#_PT3_CSP),sp
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
  LD   SP,(#_NoteTable)
  ADD  HL,SP
  LD   SP,HL
  POP  HL
  ADD  HL,DE
  LD   E,CHNPRM_CrTnSl(IX)
  LD   D,CHNPRM_CrTnSl+1(IX)
  ADD  HL,DE
  ld	 SP,(#_PT3_CSP)
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
  LD   DE,#_VTABLE-16    ;_AYREGS (searches the position of the volume table from a specific position in RAM) 
  ADD  HL,DE
  LD   A,(HL)
CH_ENV:	
  BIT  0,C
  JR   NZ,CH_NOEN
  OR   CHNPRM_Env_En(IX)
CH_NOEN:	
  LD   (#_AYREGS+AY_AmpC),A
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
  ld	 HL,#_PT3_AddToEn 
  ADD  A,(HL) ;BUG IN PT3 - NEED WORD HERE.
  	          ;FIX IT IN NEXT VERSION?
  LD   (HL),A
  JR   CH_MIX
NO_ENSL: 
  RRA
  ADD  A,CHNPRM_CrNsSl(IX)
  LD   (#_AddToNs),A
  BIT  5,B
  JR   Z,CH_MIX
  LD   CHNPRM_CrNsSl(IX),A
CH_MIX:	
  LD   A,B
  RRA
  AND  #0x48
CH_EXIT:	
  LD   HL,#_AYREGS+AY_Mixer
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



;------------------------------------------------------------------------------- DATAS

EMPTYSAMORN: 
  .db 0,1,0,0x90 
;delete $90 if you dont need default sample  ; # pongo el 0 aqui



; As there are four tables of notes available in Vortex Tracker, 
; this must be assigned externally, copying to the space reserved in the 
; variable NoteTable.

;Note table 2
;NoteTable:	
;  .dw 0x0D10,0x0C55,0x0BA4,0x0AFC,0x0A5F,0x09CA,0x093D,0x08B8,0x083B,0x07C5,0x0755,0x06EC
;  .dw 0x0688,0x062A,0x05D2,0x057E,0x052F,0x04E5,0x049E,0x045C,0x041D,0x03E2,0x03AB,0x0376
;  .dw 0x0344,0x0315,0x02E9,0x02BF,0x0298,0x0272,0x024F,0x022E,0x020F,0x01F1,0x01D5,0x01BB
;  .dw 0x01A2,0x018B,0x0174,0x0160,0x014C,0x0139,0x0128,0x0117,0x0107,0x00F9,0x00EB,0x00DD
;  .dw 0x00D1,0x00C5,0x00BA,0x00B0,0x00A6,0x009D,0x0094,0x008C,0x0084,0x007C,0x0075,0x006F
;  .dw 0x0069,0x0063,0x005D,0x0058,0x0053,0x004E,0x004A,0x0046,0x0042,0x003E,0x003B,0x0037
;  .dw 0x0034,0x0031,0x002F,0x002C,0x0029,0x0027,0x0025,0x0023,0x0021,0x001F,0x001D,0x001C
;  .dw 0x001A,0x0019,0x0017,0x0016,0x0015,0x0014,0x0012,0x0011,0x0010,0x000F,0x000E,0x000D



__endasm;
}
// ----------------------------------------------------------------------------- END

