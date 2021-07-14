/* =============================================================================
  Test PT3 player Library for SDCC
  Version: 2.0 (12/07/2021)
  Author: (test program) mvac7 <mvac7303b@gmail.com>
  Architecture: MSX
  Format: ROM 16K
  Programming language: C and Z80 assembler
  
  In examples/test_ROM software, PT3 songs:
   - "A funny day with my MSX" by Makinavaja (for AAMSX 50th RU invitro)
   - "Mathematical brain" by Makinavaja (for XI MICROCOMPO AY) 
   
  Description:
    Application to test the functioning of the PT3player library.
    
  History of versions:
    - 2.0 (12/07/2021) New GUI + test extern AY
    - 1.9 (07/07/2021) Update interruptM1_Hooks and AY38910BF Libraries 
    - 1.8 (08/06/2021) Bug #21 correction - First song fails 
    - 1.7 (16/04/2021) SC1 Tileset + Test Player_IsEnd() + show PT3_state
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

#include "../include/interruptM1_Hooks.h"
#include "../include/memory.h"
#include "../include/keyboard.h"
#include "../include/VDP_TMS9918A_MSXROM.h"
#include "../include/VDP_PRINT.h"
#include "../include/unRLEWBtoVRAM.h"

#include "../include/AY38910BF.h"
#include "../include/PT3player.h"
#include "../include/PT3player_NoteTable2.h"

//song-data .PT3
#include "../include/maki_ru50inv_PT3.h"
#include "../include/maki_CompoAY19v2_PT3.h"

#include "../include/Test_GFX.h"






#define VUMETERSPRBUFF 12*4



// Function Declarations -------------------------------------------------------
void my_TIMI();

void WAIT(uint cicles);
void LOCATE(char x, char y);

void ShowState(uint vaddr, boolean state);

void ShowVumeter(char channel, char value);

void SetSPRITES();

void Pause();

void PlaySong(char songNumber);

void SwapLoop();
void ShowLoop();
void ShowPlayback();
void ShowENDsong();




// constants  ------------------------------------------------------------------
const char text01[] = "Test PT3player Library";
const char text02[] = "v2.0 (12/07/2021)";

const char presskey[] = "Press a key to Play";

const char aytypeSTRINGS[2][7]={"INTERN","EXTERN"};



// global variable definition --------------------------------------------------

char VALUE;

char SPRBUFFER[VUMETERSPRBUFF];


boolean Row6pressed;
boolean Row7pressed;

uint firstPATaddr;

uint songNames[2];
uint songAuthors[2];
uint songPT3Data[2];

char _loop;


// Functions -------------------------------------------------------------------



void main(void)
{
  char keyPressed;
  
  char result;
    
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
  
  // Initialize the Player
  NoteTable = (unsigned int) NT;
  Player_Init();
  //AY_Init();
  //
        
  POKE(LINL32,32); //Width(32)
  COLOR(WHITE,DARK_BLUE,LIGHT_BLUE);     
  SCREEN(1);    
  SetSpritesSize(1);
  
  unRLEWBtoVRAM((uint) Frame_MAP,BASE5);
  unRLEWBtoVRAM((uint) GUI_G1_TSET_COLOR,BASE6);
  unRLEWBtoVRAM((uint) GUI_G1_TSET,BASE7);  
  
  VPRINT(1,1,text01);
  VPRINT(1,2,text02);
  
  VPRINT(1,8,"Do you want to use the");
  VPRINT(1,9,"external AY? ([Y] for Yes)");
  
  LOCATE(1,10);
  result = INKEY();
  
  if (result==89 || result==121) AY_TYPE=AY_EXTERNAL;
  else AY_TYPE=AY_INTERNAL;
  
  unRLEWBtoVRAM((uint) GUI_MAP,BASE5+96); //show screen map
  
  VPRINT(4,8,aytypeSTRINGS[AY_TYPE]);
  
  _loop = OFF; 
                  
  SetSPRITES();
  
  Install_TIMI(my_TIMI);
  
  while(1)
  {
    HALT;      
    
    if (PT3_state & Bit1)
    {
        songStep=PT3_CrPsPtr - firstPATaddr;
        VPrintNumber(8,13,songStep,3);
        //LOCATE(25,13);
        //PrintFNumber(PEEKW(PT3_LPosPtr),32,5);
    }

    ShowVumeter(0,AYREGS[AY_AmpA]);
    ShowVumeter(1,AYREGS[AY_AmpB]);
    ShowVumeter(2,AYREGS[AY_AmpC]);    

    
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
    
    ShowPlayback();
    ShowLoop();
    ShowENDsong();
    
  }

/*  
  uninstall_isr();
  
  CLS();
  PRINT("END");
  WAIT(30*5);
*/  
}



// Routine for Hook TIMI (FD9Fh)
void my_TIMI() 
{
  PUSH_AF;
  
  PlayAY();

__asm  
  ;vuelca a VRAM buffer atributos sprites
  ld   HL,#_SPRBUFFER
  ld   DE,#BASE13  
  ld   BC,#VUMETERSPRBUFF
  call 0x005C  
__endasm;

  POP_AF;
}




void Pause()
{
    
   if ((PT3_state & Bit1)) Player_Pause();
   else Player_Resume();
   
}



void SwapLoop()
{
  _loop = !_loop;
  
  Player_Loop(_loop);
  //ShowLoop();
}



void PlaySong(char songNumber)
{
  //DI;
  
  Player_Pause();
      
  Player_InitSong(songPT3Data[songNumber], _loop);  // (unsigned int) Song data address ; (char) Loop - 0=off ; 1=on
  
  firstPATaddr = PT3_CrPsPtr;
  
  VPrintNumber(8,10,songNumber+1,1);
  
  VPRINT(8,11,"                       ");
  VPRINTN(8,11,(char*) songNames[songNumber],23);
  
  VPRINT(8,12,"                       ");
  VPRINTN(8,12,(char*) songAuthors[songNumber],23);
  
  //EI;
}



/*void PrintF(char* text, char length)
{ 
  while(*(text) && length-->0)  bchput(*(text++));
}*/




void ShowPlayback()
{   
  if ((PT3_state & Bit1)) ShowState(BASE5+744,true);  //741 = 23*32 + 5
  else ShowState(BASE5+744,false);
}



void ShowLoop()
{   
  if ((PT3_state & Bit4)) ShowState(BASE5+754,true);  //751 = 23*32 + 15
  else ShowState(BASE5+754,false);
}



void ShowENDsong()
{   
  if ((Player_IsEnd()==true)) ShowState(BASE5+763,true);  //760 = 23*32 + 24
  else ShowState(BASE5+763,false);
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



/* =============================================================================
  LOCATE
 
  Description: 
            Moves the cursor to the specified location.
  Input:    (char) Position X of the cursor. (0 to 31 or 79)
            (char) Position Y of the cursor. (0 to 23)         
  Output:   -
============================================================================= */
void LOCATE(char x, char y) __naked
{
x;y;
__asm
  push IX
  ld   IX,#0
  add  IX,SP
  
  ld   A,4(IX) ;x
  inc  A       ;incrementa las posiciones para que se situen correctamente en la pantalla
  ld   H,A
  ld   A,5(IX) ;y
  inc  A
  ld   L,A   
  call POSIT
  
  pop  IX
  ret
__endasm;

}




void ShowState(uint vaddr, boolean state)
{
    char tile;
    if (state==true) tile=189;
    else tile=221;
    VPOKE(vaddr++,tile++);
    VPOKE(vaddr++,tile++);
    VPOKE(vaddr,tile);
}



void ShowVumeter(char channel, char value) __naked
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
  ret
  
__endasm;
}



void SetSPRITES() __naked
{
__asm

  push IX
  
  ld   HL,#SPRITE_DATA
  ld   DE,#BASE14  
  ld   BC,#32*5
  call 0x005C

  ld   DE,#_SPRBUFFER
  ld   HL,#VUMETER
  ld   BC,#VUMETERSPRBUFF
  ldir
  
  pop  IX
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
;END SPRITE ATRIBUTE DATAS #####################################################
__endasm;
}