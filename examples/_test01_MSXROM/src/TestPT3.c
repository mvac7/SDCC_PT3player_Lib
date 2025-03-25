/* =============================================================================
Test PT3player Library for SDCC
Version: 2.2 (11/02/2025)
Author: mvac7
Architecture: MSX
Format: ROM 16K
Programming language: C and Z80 assembler
Compiler: SDCC 4.4 or newer

In examples/test_ROM software, PT3 songs:
- "A funny day with my MSX" by Makinavaja (for AAMSX 50th RU invitro)
- "Mathematical brain" by Makinavaja (for XI MICROCOMPO AY) 

Description:
Application to test the functioning of the PT3player library.
    
History of versions: (dd/mm/yyyy)
- v2.2 (11/02/2025) update to SDCC (4.1.12) Z80 calling conventions
- v2.1 (07/10/2021) Updates from v1.1.10 PT3player Lib
- v2.0 (12/07/2021) New GUI + test extern AY
- v1.9 (07/07/2021) Update interruptM1_Hooks and AY38910BF Libraries 
- v1.8 (08/06/2021) Bug #21 correction - First song fails 
- v1.7 (16/04/2021) SC1 Tileset + Test Player_IsEnd() + show PT3_state
- v1.6 (15/02/2021) Adaptation to PT3_Player v1.1.6 (function names)
- v1.5 (28/01/2021) support for multiple songs
- v1.4 (07/01/2021)
- v1.3 (06/01/2021) Test PT3_Loop
- v1.2 (05/01/2021) Updates related to v1.3 of the PT3player Lib
- v1.1 (04/01/2021) assigning the frequency table to NoteTable
- v1.0 (28/05/2019)
============================================================================= */

#include "../include/newTypes.h"
#include "../include/msxSystemVariables.h"
#include "../include/msxBIOS.h"

#include "../include/interruptM1_Hooks.h"
#include "../include/memory_Z80.h"
#include "../include/keyboard_MSX.h"
#include "../include/VDP_TMS9918A_MSXBIOS.h"
#include "../include/VDP_PRINT.h"
#include "../include/unWRLE2toVRAM.h"

#include "../include/PSG_AY38910BF.h"
#include "../include/PT3player.h"
#include "../include/PT3player_NoteTable2.h"

//song-data .PT3
#include "../include/maki_ru50inv_PT3.h"
#include "../include/maki_CompoAY19v2_PT3.h"

#include "../include/Test_GFX.h"






#define VUMETERSPRBUFF 12*4



// Function Declarations -------------------------------------------------------
void my_TIMI(void);

void WAIT(uint cicles);
void LOCATE(char x, char y);

void ShowState(uint vaddr, boolean state);

void ShowVumeter(char channel, char value);

void SetSPRITES(void);

void Pause(void);

void PlaySong(char songNumber);

void SwapLoop(void);
void ShowLoop(void);
void ShowPlayback(void);
void ShowENDsong(void);




// constants  ------------------------------------------------------------------
const char text01[] = "Test PT3player Library";
const char text02[] = "v2.2 (11/02/2025)";

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

char _currentSong;
     

// Functions -------------------------------------------------------------------



void main(void)
{
	char keyPressed;  
	char result;
	char AYtype = 0;

	uint conta=0;
	uint songStep;

	_currentSong=0;

	Row6pressed=false;
	Row7pressed=false;

	songNames[0] = (unsigned int) SONG00_name;
	songNames[1] = (unsigned int) SONG01_name;
	songAuthors[0] = (unsigned int) SONG00_author;
	songAuthors[1] = (unsigned int) SONG01_author;  
	songPT3Data[0] = (unsigned int) SONG00;
	songPT3Data[1] = (unsigned int) SONG01;

	// Initialize the Player
	Player_Init();
	//AY_Init();
	//
		
	POKE(LINL32,32); //Width(32)
	COLOR(WHITE,DARK_BLUE,LIGHT_BLUE);     
	SCREEN(1);    
	SetSpritesSize(1);

	unWRLE2toVRAM16((uint) Frame_MAP,BASE5);
	unWRLE2toVRAM16((uint) GUI_G1_TSET_COL,BASE6);
	unWRLE2toVRAM16((uint) GUI_G1_TSET_PAT,BASE7);  

	VLOCATE(1,1);
	VPRINT(text01);
	VLOCATE(1,2);
	VPRINT(text02);

	VLOCATE(1,8);
	VPRINT("Do you want to use the");
	VLOCATE(1,9);
	VPRINT("external AY? ([Y] for Yes)");

	LOCATE(1,10);
	result = INKEY();
  
	if (result==89 || result==121){
			AY_IOport=AY_EXTERNAL;
			AYtype++;
	}else	AY_IOport=AY_INTERNAL;
  
	unWRLE2toVRAM16((uint) GUI_MAP,BASE5+96); //show screen map

	VLOCATE(4,8);
	VPRINT(aytypeSTRINGS[AYtype]);

	_loop = OFF; 
				  
	SetSPRITES();

	Install_TIMI(my_TIMI);

	while(1)
	{
		HALT;      

		if (PT3_state & PT3_PLAY)
		{
			songStep=PT3_CrPsPtr - firstPATaddr;
			VLOCATE(8,13);
			VPrintFNumber(songStep,0x20,3);	//3
			// -------------------------------------- test vars
			//VPrintNumber(14,13,PEEK(77,25),5); //num of pattern?        
			// -------------------------------------- END test vars
		}

		/*    else{        
			if (Player_IsEnd() && _loop==OFF)
			{
				_currentSong++;
				if(_currentSong>1) _currentSong=0; 
				PlaySong(_currentSong);            
			}   
		}*/

		ShowVumeter(0,AYREGS[AY_AmpA]);
		ShowVumeter(1,AYREGS[AY_AmpB]);
		ShowVumeter(2,AYREGS[AY_AmpC]);    


		// Keyboard row 6
		keyPressed = GetKeyMatrix(6);  
		if (keyPressed!=0xFF)  //pressure control of the keys
		{
			if(Row6pressed==false)
			{
				if (!(keyPressed&Bit5)){Row6pressed=true;PlaySong(0);} //F1 Key
				if (!(keyPressed&Bit6)){Row6pressed=true;PlaySong(1);} //F2 Key
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
				//if (!(keyPressed&Bit0)) {Row7pressed=true;}			//[F4]
				//if (!(keyPressed&Bit1)) {Row7pressed=true;}			//[F5]
				//if (!(keyPressed&Bit2)) {Row7pressed=true;}			//[ESC]
				if (!(keyPressed&Bit3)) {Row7pressed=true;SwapLoop();}	//[TAB]
				if (!(keyPressed&Bit4)) {Row7pressed=true;Pause();}		//[STOP]
				//if (!(keyPressed&Bit5)) {Row7pressed=true;}			//[BS]
				//if (!(keyPressed&Bit6)) {Row7pressed=true;}			//[SELECT]
				if (!(keyPressed&Bit7)) {Row7pressed=true;PlaySong(0);}	//[RETURN]
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
void my_TIMI(void) 
{
	PUSH_AF;

	PlayAY();

__asm
;Vumeter
;vuelca a VRAM buffer atributos sprites
	ld   HL,#_SPRBUFFER
	ld   DE,#BASE13  
	ld   BC,#VUMETERSPRBUFF
	call 0x005C  
__endasm;

	POP_AF;
}




void Pause(void)
{
	if ((PT3_state & Bit1)) Player_Pause();
	else Player_Resume();
}



void SwapLoop(void)
{
	_loop = !_loop;

	Player_Loop(_loop);
	//ShowLoop();
}



void PlaySong(char songNumber)
{
	uint songADDR;

//	_currentSong=songNumber;

	Player_Pause();
  
	songADDR = songPT3Data[songNumber];
	
	VLOCATE(8,11);
	VPRINT("                       ");	//clear text space
	VLOCATE(8,11);
	VPRINTN((char*) songNames[songNumber],23);

	VLOCATE(8,12);
	VPRINT("                       ");	//clear text space
	VLOCATE(8,12);
	VPRINTN((char*) songAuthors[songNumber],23);  
	
	VLOCATE(8,10);
	VPrintFNumber(songNumber+1,0x20,3);
	
	
      
	Player_InitSong(songADDR, (unsigned int) NT2, _loop);

	firstPATaddr = PT3_CrPsPtr;

}



void ShowPlayback(void)
{   
  if ((PT3_state & Bit1)) ShowState(BASE5+744,true);  //741 = 23*32 + 5
  else ShowState(BASE5+744,false);
}



void ShowLoop(void)
{   
  if ((PT3_state & Bit4)) ShowState(BASE5+754,true);  //751 = 23*32 + 15
  else ShowState(BASE5+754,false);
}



void ShowENDsong(void)
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
x;	//A
y;	//L
__asm
	push IX

	inc  A       ;incrementa las posiciones para que se situen correctamente en la pantalla
	ld   H,A
	inc  L
	call BIOS_POSIT

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
channel;	//A
value;		//L
__asm

    
  ld   C,A
  ld   A,L


;C = num channel
;A = value  
;showVumeter:

	ld   (_VALUE),A

	SLA  C
	SLA  C

	ld	 B,#0
L00107:
	ld	 a,c
	ld	 l,a
	rla
	sbc	 a,a
	ld	 h,a
	add	 hl,hl
	add	 hl,hl

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
	jr	 C,L00107
  

	ret
  
__endasm;
}



void SetSPRITES(void) __naked
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