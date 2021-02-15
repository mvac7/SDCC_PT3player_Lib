/* =============================================================================
  Test PT3 player Library for SDCC
  Version: 1.6 (15/02/2021)
  Author: (test program) mvac7 <mvac7303b@gmail.com>
  Architecture: MSX
  Format: ROM 16K
  Programming language: C and Z80 assembler
   
  Description:

    
  History of versions:
    - 1.6 (15/02/2021) Adaptation to PT3_Player v1.1.6 (function names)
    - 1.5 (28/01/2021) support for multiple songs
    - 1.4 (07/01/2021)
    - 1.3 (06/01/2021) Test PT3_Loop
    - 1.2 (05/01/2021) Updates related to v1.3 of the PT3player Lib
    - 1.1 (04/01/2021) assigning the frequency table to NoteTable
    - 1.0 (28/5/2019)
============================================================================= */

#include "../include/newTypes.h"
#include "../include/msxSystemVariables.h"
#include "../include/msxBIOS.h"

#include "../include/memory.h"
#include "../include/keyboard.h"
#include "../include/textmode.h"
#include "../include/interrupt.h"

#include "../include/PT3player.h"
#include "../include/PT3player_NoteTable2.h"

//song-data .PT3
#include "../include/maki_ru50inv_PT3.h"
#include "../include/maki_CompoAY19v2_PT3.h"


#define  HALT __asm halt __endasm   //Z80 instruction: wait for the next interrupt


#define BASE13 0x1B00 // base 13 sprite attribute table
#define BASE14 0x3800 // base 14 sprite pattern table

#define VDPVRAM   0x98 //VRAM Data (Read/Write)
#define VDPSTATUS 0x99 //VDP Status Registers

 
 






// Function Declarations -------------------------------------------------------
void SetSpritesSize(char size);

char VPEEK(uint address);

void WAIT(uint cicles);


void ShowVumeter(char channel, char value);

void SetSPRITES();

void Pause();

void PlaySong(char songNumber);

void SwapLoop();
void ShowLoop();


void PrintF(char* text, char length);



// constants  ------------------------------------------------------------------
const char text01[] = "Test PT3player v1.2 Lib";
const char text02[] = "v1.6 (15/02/2021)";

const char presskey[] = "Press a key to Play";



// global variable definition --------------------------------------------------

char VALUE;

char SPRBUFFER[72];  //20*4 =72B


boolean Row6pressed;
boolean Row7pressed;

uint firstPATaddr;

uint songNames[2];
uint songAuthors[2];
uint songPT3Data[2];

char _loop;


// Functions -------------------------------------------------------------------


// Routine for Hook TIMI (FD9Fh)
void my_isr0(void) __interrupt 
{
	//DI;
	//READ_VDP_STATUS;     <<---- It is not necessary as the ISR in the BIOS does.

__asm push  AF __endasm;
  
    PlayAY();

__asm  
  ;vuelca a VRAM buffer atributos sprites
  ld   HL,#_SPRBUFFER
  ld   DE,#BASE13  
  ld   BC,#20*4
  call 0x005C  
__endasm;

__asm pop   AF __endasm;       

  //EI;
}




//
void main(void)
{
  char keyPressed;
    
  uint conta = 0;
  uint songStep;
  
  Row6pressed=false;
  Row7pressed=false;
  
  songNames[0] = (unsigned int) SONG00_name;
  songNames[1] = (unsigned int) SONG01_name;
  songAuthors[0] = (unsigned int) SONG00_author;
  songAuthors[1] = (unsigned int) SONG01_author;  
  songPT3Data[0] = (unsigned int) SONG00;
  songPT3Data[1] = (unsigned int) SONG01;
  
   
  COLOR(WHITE,DARK_BLUE,LIGHT_BLUE);
          
  WIDTH(32);
  SCREEN1();    
  SetSpritesSize(1);  
  
  LOCATE(0,0);
  PRINT(text01);
  PRINT("\n");
  PRINT(text02);
  
  // Initialize the Player
  NoteTable = (unsigned int) NT;
  Player_Init();
  //
  
    
  _loop = Loop_OFF;
  
            
  LOCATE(0,3);
  PRINT("F1 or F2 for play a song\n");
  //PRINT("1 to 7 for play a FX\n"); 
  //PRINT("RETURN Play the song\n");
  PRINT("STOP   Pause/Resume playback\n");
  PRINT("TAB    Change Loop mode\n"); 
  
  SetSPRITES();
  
  install_isr(my_isr0);
  
  while(1)
  {
    HALT;
      
    
    if ((PT3state & Bit1))
    {
        songStep=PT3_CrPsPtr - firstPATaddr;
        LOCATE(7,13);
        PrintFNumber(songStep,32,3);
        //LOCATE(25,13);
        //PrintFNumber(PEEKW(PT3_LPosPtr),32,5);
    }

    ShowVumeter(0,AYREGS[AR_AmplA]);
    ShowVumeter(1,AYREGS[AR_AmplB]);
    ShowVumeter(2,AYREGS[AR_AmplC]);    

    
    // Keyboard row 6
    keyPressed = GetKeyMatrix(6);  
    if (keyPressed!=0xFF)  //pressure control of the keys
    {
      if(Row6pressed==false)
      {
        if (!(keyPressed & Bit5)){Row6pressed=true;PlaySong(0);} //F1 Key
        if (!(keyPressed & Bit6)){Row6pressed=true;PlaySong(1);} //F2 Key
        //if (!(keyPressed & Bit7)){keyB6pressStatus=true;PlaySong(3);} //F3 Key        
      }      
    }else{
      Row6pressed=false;        
    }   
    
    
    // Keyboard row 7
    keyPressed = GetKeyMatrix(7);
    if (keyPressed!=0xFF)
    {
      if(Row7pressed==false)
      {
        //if (!(keyPressed&Bit0)) {Row7pressed=true;}; // [F4]
        //if (!(keyPressed&Bit1)) {Row7pressed=true;}; // [F5]
        //if (!(keyPressed&Bit2)) {Row7pressed=true;}; // [ESC]
        if (!(keyPressed&Bit3)) {Row7pressed=true;SwapLoop();}; // [TAB]
        if (!(keyPressed&Bit4)) {Row7pressed=true;Pause();}; // [STOP]
        //if (!(keyPressed&Bit5)) {Row7pressed=true;}; // [BS]
        //if (!(keyPressed&Bit6)) {Row7pressed=true;}; // [SELECT]
        //if (!(keyPressed&Bit7)) {Row7pressed=true;PlaySong(0);}; // [RETURN]
      }      
    }else Row7pressed=false;
    
    
    Player_Decode();  //Process the next step in the song sequence

    
  }

/*  
  uninstall_isr();
  
  CLS();
  PRINT("END");
  WAIT(30*5);
*/  
}



void Pause()
{
    
   if ((PT3state & Bit1)) Player_Pause();
   else Player_Resume();
   
}


void SwapLoop()
{
  _loop = !_loop;
  
  Player_Loop(_loop);
  ShowLoop();
}



void PlaySong(char songNumber)
{ 
  //PT3Init((unsigned int) SONG00 - 100,0); // Subtract 100 if you delete the header of the PT3 file.    
  Player_InitSong(songPT3Data[songNumber], _loop);  // (unsigned int) Song data address ; (char) Loop - 0=off ; 1=on
  
  firstPATaddr = PT3_CrPsPtr;
  
  LOCATE(0,9);  
  PRINT("Song  :   ");
  LOCATE(7,9);
  PrintFNumber(songNumber+1,32,1);
  
  LOCATE(0,10);
  PRINT("Name  :                         ");
  LOCATE(7,10);
  PrintF((char*) songNames[songNumber],25);
  
  LOCATE(0,11);
  PRINT("Author:                         ");
  LOCATE(7,11);
  PrintF((char*) songAuthors[songNumber],25);
  
  LOCATE(0,13);    
  PRINT("Pos.  :    ");
  
  //PRINT("\nTempo : ");
  //PrintFNumber(TEMPO,32,2);
  
  ShowLoop();
}



void PrintF(char* text, char length)
{
  DI;  
  while(*(text) && length-->0)  bchput(*(text++));
  EI;  
}



void ShowLoop()
{   
  //if (_loop==Loop_ON) PRINT("ON "); 
    
  LOCATE(0,12);
  if ((PT3state & Bit4)) PRINT("Loop  :ON ");
  else PRINT("Loop  :OFF");
}


/* =============================================================================
 SetSpritesSize
 Description: Set size type for the sprites.
 Input:       [char] size: 0=8x8; 1=16x16
 Output:      -
============================================================================= */ 
void SetSpritesSize(char size) __naked
{
size;
__asm
  push IX
  ld   IX,#0
  add  IX,SP
  
  ld   HL,#RG0SAV+1 ; --- read vdp(1) from mem
  ld   B,(HL)

  ld   A,4(ix)    
  cp   #1
  jr   NZ,size8
  
  set  1,B ; 16x16
  jr   setSize
  
size8:
  res  1,B  ; 8x8    

setSize:  
  ld   C,#0x01
  call writeVDP
  
  pop  IX
  ret
  
  
writeVDP:

  ld   IY,#RG0SAV
  ld   E,C
  ld   D,#0
  add  IY,DE
  ld   (IY),B ;save copy of vdp value in system var
  
  ld   A,B
  di
  out	 (#VDPSTATUS),A
  ld   A,C
  or   #0x80            ;add 128 to VDP reg value
  out	 (#VDPSTATUS),A
  ei
  ret

__endasm;
}




char VPEEK(uint address) __naked
{
address;
__asm
  push IX
  ld   IX,#0
  add  IX,SP
    
  ld   L,4(IX)
  ld   H,5(IX) 
   
  call RDVRM
  
  ld   L,A
  pop  IX
  ret
__endasm;
}



/* =============================================================================
   WAIT
   Generates a pause in the execution of n interruptions.
   PAL: 50=1second. ; NTSC: 60=1second.
   Input    : [unsigned int] cicles 
   Output   : -
============================================================================= */ 
void WAIT(uint cicles)
{
  uint i;
  for(i=0;i<cicles;i++) HALT;
  return;
}



void ShowVumeter(char channel, char value)
{
channel;value;
__asm

  push IX
  ld   IX,#0
  add  IX,SP
    
  ld   C,4(IX)
  ld   A,5(IX)


;C = num channel
;A = value  
;showVumeter:

	ld   (_VALUE),A

	SLA  C
	SLA  C

	ld	 B,#0
L00107:
	ld	a,c
	ld	l,a
	rla
	sbc	a, a
	ld	h,a
	add	hl,hl
	add	hl,hl

  ld   DE,#_SPRBUFFER
  ADD  HL,DE
  ex   DE,HL
  
	inc	 DE
	inc	 DE

	ld   A,(_VALUE)
	cp	 #0
	jr	 NZ,L00102
	xor  A
	ld	 (DE),A
	jr   L00105
  
L00102:                       
  ld   A,(_VALUE)
	cp   #4
	jr	 C,L00104
	ld	 A,#16
	ld	 (DE),A

	ld    A,(_VALUE)
  sub   #4
	ld   (_VALUE),A
	jr	 L00105
L00104:
  ld   A,(_VALUE)
	add	 a,a
	add	 a,a
	ld	(DE),A
	xor  A
	ld   (_VALUE),A
L00105:
	inc	 C
	inc	 B
	ld	 A,B
  cp   #4
	jr	C,L00107
  

  pop  IX
;  ret
  
__endasm;
}



void SetSPRITES() __naked
{
__asm


  ld   HL,#SPRITE_DATA
  ld   DE,#BASE14  
  ld   BC,#32*5
  call 0x005C

  ld   DE,#_SPRBUFFER
  ld   HL,#VUMETER
  ld   BC,#64
  ldir
  
  ret
  

SPRITE_DATA:
; 0-vum0
.db 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
.db 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
; 1-vum1
.db 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x7F,0x7F,0x7F,0x00
.db 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFE,0xFE,0xFE,0x00
; 2-vum2
.db 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x7F,0x7F,0x7F,0x00,0x7F,0x7F,0x7F,0x00
.db 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFE,0xFE,0xFE,0x00,0xFE,0xFE,0xFE,0x00
; 3-vum3
.db 0x00,0x00,0x00,0x00,0x7F,0x7F,0x7F,0x00,0x7F,0x7F,0x7F,0x00,0x7F,0x7F,0x7F,0x00
.db 0x00,0x00,0x00,0x00,0xFE,0xFE,0xFE,0x00,0xFE,0xFE,0xFE,0x00,0xFE,0xFE,0xFE,0x00
; 4-vum4
.db 0x7F,0x7F,0x7F,0x00,0x7F,0x7F,0x7F,0x00,0x7F,0x7F,0x7F,0x00,0x7F,0x7F,0x7F,0x00
.db 0xFE,0xFE,0xFE,0x00,0xFE,0xFE,0xFE,0x00,0xFE,0xFE,0xFE,0x00,0xFE,0xFE,0xFE,0x00



;SPRITE ATRIBUTE DATAS #########################################################
; for adjust colors, changue the last valor in line


;Y,X,SPR,COLOR
VUMETER:
.db 167,8,0,2
.db 151,8,0,2
.db 135,8,0,10
.db 119,8,0,8

.db 167,24,0,2
.db 151,24,0,2
.db 135,24,0,10             
.db 119,24,0,8 

.db 167,40,0,2
.db 151,40,0,2
.db 135,40,0,10
.db 119,40,0,8
             
.db 167,56,0,2
.db 151,56,0,2
.db 135,56,0,10
.db 119,56,0,8
;END SPRITE ATRIBUTE DATAS #####################################################
__endasm;
}