/* =============================================================================
TestPlayAY
Version:	1.0 (01/03/2025)
Author:		mvac7/303bcn
Architecture: MSX
Format:		ROM 16K
language:	C and Z80 assembler
Compiler:	SDCC 4.4 or newer

PT3 songs:
- "Mathematical brain" by Makinavaja (XI MICROCOMPO AY)

Description:
 Application for test AY38910BF MSX Library with PT3player Library.
    
History of versions: (dd/mm/yyyy)
- v1.0 (01/03/2025)
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
#include "../include/maki_CompoAY19v2_PT3.h"	//Song00
#include "../include/maki_ru50inv_PT3.h"		//Song01
#include "../include/maki_truintroFULL_PT3.h"	//Song02

#include "../include/Test_GFX.h"






#define VUMETERSPRBUFF 12*4



#ifndef  __BITVALUES__
#define  __BITVALUES__
#define Bit0 1
#define Bit1 2
#define Bit2 4
#define Bit3 8
#define Bit4 16
#define Bit5 32
#define Bit6 64
#define Bit7 128
#endif




// Function Declarations -------------------------------------------------------
void my_TIMI(void);


//void SilenceAY(char AYport);


void Copy2firstAY(void);
void Copy2secondAY(void);


void DumpOAM(void);

void SelectAY(char type);

void BackSong(void);
void NextSong(void);
void ShowCursor(void);

void WAIT(uint cicles);
void LOCATE(char x, char y);

void ShowState(uint vaddr, boolean state);

void ShowVumeter(char channel, char value);

void Pause(void);

void PlaySong(void);
//void PlaySong(char songNumber);

void SwapLoop(void);
void ShowLoop(void);
void ShowPlayback(void);
void ShowENDsong(void);


char INKEY(void);
char GetKeyMatrix(char row);



// constants  ------------------------------------------------------------------
const char text01[] = "Test AY38910BF + PT3player";
//const char text02[] = "v1.0 (1/3/2025)";

const char presskey[] = "Press a key to Play";

const char VumeterTile[8]={128,128,128,128,136,136,144,144};
const char channelPos[11]={0,1,2, 4,5,6, 8,9,10,11,12};	//AY1(3), AY2(3), SCC(5)



// global variable definition --------------------------------------------------

char selectedSong;

uint cursor_vaddr;

char VALUE;

char SPRBUFFER[VUMETERSPRBUFF];

//boolean Row6pressed;
boolean Row7pressed;
boolean Row8pressed;

uint firstPATaddr;

uint songNames[3];
uint songAuthors[3];
uint songPT3Data[3];

char _PSGtype;

switcher _loop;

char GUI_Playback;
char GUI_Loop;
char GUI_End;

//char _currentSong;

char AYREGS_AY1[14];	// buffer for first  AY
char AYREGS_AY2[14];	// buffer for second AY
     

// Functions -------------------------------------------------------------------



void main(void)
{
	char keyPressed;  
	char i;

	uint conta=0;
	uint songStep;

//	_currentSong=0;

//	Row6pressed=false;
	Row7pressed=false;
	Row8pressed=false;
	
	selectedSong=0;
	cursor_vaddr=0x19D5;
	
	GUI_Playback = 128;
	GUI_Loop = 128;
	GUI_End = 128;
	
	_loop = OFF;
	

	songNames[0] = (unsigned int) SONG00_name;
	songNames[1] = (unsigned int) SONG01_name;
	songNames[2] = (unsigned int) SONG02_name;

	songAuthors[0] = (unsigned int) SONG00_author;
	songAuthors[1] = (unsigned int) SONG01_author;
	songAuthors[2] = (unsigned int) SONG02_author;  

	songPT3Data[0] = (unsigned int) SONG00;
	songPT3Data[1] = (unsigned int) SONG01;
	songPT3Data[2] = (unsigned int) SONG02;


	// Initialize the Player
	Player_Init();
	//AY_Init();
	//
		
	POKE(LINL32,32); //Width(32)
	COLOR(WHITE,DARK_BLUE,LIGHT_BLUE);     
	SCREEN(1);    
	SetSpritesSize(1);

	unWRLE2toVRAM16((uint) GUI_MAP,BASE5);			//show screen map
	unWRLE2toVRAM16((uint) GUI_G1_TSET_COL,BASE6);
	unWRLE2toVRAM16((uint) GUI_G1_TSET_PAT,BASE7);  

	VLOCATE(1,1);
	VPRINT(text01);
//	VLOCATE(1,2);
//	VPRINT(text02);

	SelectAY(1);
	
	for(i=0;i<3;i++)
	{
		VLOCATE(22,14+i);
		VPRINT("Track_0");
		VPrintFNumber(i+1,0x20,1);
	}
	
	ShowCursor();

	Install_TIMI(my_TIMI);

	while(1)
	{
		HALT;

		if (PT3_state & PT3_PLAY)
		{
			songStep=PT3_CrPsPtr - firstPATaddr;
			VLOCATE(14,10);
			VPrintFNumber(songStep,0x20,3);
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

		//AY1
		ShowVumeter(0,AYREGS_AY1[AY_AmpA]);
		ShowVumeter(1,AYREGS_AY1[AY_AmpB]);
		ShowVumeter(2,AYREGS_AY1[AY_AmpC]); 
		
		//AY2
		ShowVumeter(3,AYREGS_AY2[AY_AmpA]);
		ShowVumeter(4,AYREGS_AY2[AY_AmpB]);
		ShowVumeter(5,AYREGS_AY2[AY_AmpC]);

		//VLOCATE(2,22);
		//VPrintFNumber(AYREGS[AY_AmpB],0x20,3);		


		// Keyboard row 6
/*		keyPressed = GetKeyMatrix(6);  
		if (keyPressed!=0xFF)  //pressure control of the keys
		{
			if(Row6pressed==false)
			{
				if (!(keyPressed&Bit5)){Row6pressed=true;PlaySong(0);} //F1 Key
				if (!(keyPressed&Bit6)){Row6pressed=true;PlaySong(1);} //F2 Key
				//if (!(keyPressed&Bit7)){keyB6pressStatus=true;PlaySong(3);} //F3 Key        
			}      
		}else Row6pressed=false;
*/   


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
				if (!(keyPressed&Bit6)) {Row7pressed=true;SelectAY(++_PSGtype);}			//[SELECT]
				if (!(keyPressed&Bit7)) {Row7pressed=true;PlaySong();}	//[RETURN]
			}      
		}else Row7pressed=false;
		
		
		// Keyboard row 8
		keyPressed = GetKeyMatrix(8);
		if (keyPressed!=0xFF)
		{
			if(Row8pressed==false)
			{
				//if (!(keyPressed&Bit0)) {Row8pressed=true;}; // [SPACE]
				//if (!(keyPressed&Bit1)) {Row8pressed=true;}; // [HOME]
				//if (!(keyPressed&Bit2)) {Row8pressed=true;}; // [INS]
				//if (!(keyPressed&Bit3)) {Row8pressed=true;}; // [DEL]
				//if (!(keyPressed&Bit4)) {Row8pressed=true;}; // [LEFT]
				if (!(keyPressed&Bit5)) {Row8pressed=true;BackSong();} // [UP]
				if (!(keyPressed&Bit6)) {Row8pressed=true;NextSong();} // [DOWN]
				//if (!(keyPressed&Bit7)) {Row8pressed=true;}; // [RIGHT]
			}      
		}else Row8pressed=false;

		if (_PSGtype&2) Copy2secondAY();	//It is placed before the Decode to create a small sound Delay (1 frame)
		Player_Decode();  //Process the next step in the song sequence
		if (_PSGtype&1) Copy2firstAY();

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

	if (_PSGtype&1) Dump2AY(AY_INTERNAL,(unsigned int) AYREGS_AY1);
	if (_PSGtype&2) Dump2AY(AY_EXTERNAL,(unsigned int) AYREGS_AY2);
	AYREGS_AY1[13]=0b10000000;	//disable enveloppe
	AYREGS_AY2[13]=0b10000000;	//disable enveloppe
	//PlayAY();

	DumpOAM();	//Vumeter - dump to VRAM sprite attributes

	POP_AF;
}



void DumpOAM(void)
{
__asm
	ld   HL,#_SPRBUFFER
	ld   DE,#BASE13  
	ld   BC,#VUMETERSPRBUFF
	call BIOS_LDIRVM	;Copy a block from memory to VRAM
__endasm;
}



void SelectAY(char type)
{
	if(type==0 || type>3) type=1;
	_PSGtype = type;
	
	//if(type&1)	AY_IOport = AY_INTERNAL;
	//if(type==2) AY_IOport = AY_EXTERNAL;
	
	if(type==1){ClearAYbuffer((unsigned int) AYREGS_AY2);SilenceAYbyPort(AY_EXTERNAL);}
	if(type==2){ClearAYbuffer((unsigned int) AYREGS_AY1);SilenceAYbyPort(AY_INTERNAL);}
	
	VLOCATE(4,23);
	//VPrintFNumber(type,0x20,3);
	if(type&1) VPRINT(PSGtypeTiles[0]);
	if(type&2) VPRINT(PSGtypeTiles[1]);	
	if(type!=3) VPRINT("   ");
}



void BackSong(void)
{
	if(selectedSong==0) selectedSong=2;
	else selectedSong--;
	ShowCursor();
}



void NextSong(void)
{
	if(selectedSong==2) selectedSong=0;
	else selectedSong++;
	ShowCursor();
}


void ShowCursor(void)
{
	VPOKE(cursor_vaddr,32);
	cursor_vaddr=0x19D5+(selectedSong*32);
	VPOKE(cursor_vaddr,11);
}


/*void SilenceAY(char AY_port) __naked
{
AY_port;		//A
__asm
	ld   E,A
	ld   D,#AY_AmpA
	xor  A
	ld   B,#3

AY_ampLOOP:
	ld   C,E	;set AY index port
	out  (C),D	;AY register
	inc  D
	inc  C		;go to AY write port
	out  (C),A	;write to AY reg
	djNZ AY_ampLOOP
	ret
__endasm;	
}*/



void Copy2firstAY(void)
{
__asm
    LD   HL,#_AYREGS
    LD   DE,#_AYREGS_AY1
    LD   BC,#14
    LDIR
__endasm;
}


void Copy2secondAY(void)
{
__asm
    LD   HL,#_AYREGS
    LD   DE,#_AYREGS_AY2
    LD   BC,#14
    LDIR
__endasm;
}




void Pause(void)
{
	if (PT3_state & PT3_END) return;	//(Player_IsEnd()==true)
	if (PT3_state & PT3_PLAY) Player_Pause();
	else Player_Resume();
}



void SwapLoop(void)
{
	_loop = !_loop;
	Player_Loop(_loop);
}



void PlaySong(void)
{
	uint songADDR;	
	char clearText[] = "                         ";
	
	songADDR = songPT3Data[selectedSong];
	
	VLOCATE(6,10);
	VPrintFNumber(selectedSong+1,0x20,2);
	
	VLOCATE(6,11);
	VPRINT(clearText);	//clear text space
	VLOCATE(6,11);
	VPRINTN((char*) songNames[selectedSong],25);

	VLOCATE(6,12);
	VPRINT(clearText);	//clear text space
	VLOCATE(6,12);
	VPRINTN((char*) songAuthors[selectedSong],25);
	
	Player_InitSong(songADDR, (unsigned int) NT2, _loop);
	firstPATaddr = PT3_CrPsPtr;
}



void ShowPlayback(void)
{
	boolean SwitherStatus = false;
	uint vaddr;
	SwitherStatus = (PT3_state & PT3_PLAY);
	
	if (SwitherStatus==GUI_Playback) return;
	GUI_Playback = SwitherStatus;
	vaddr = BASE5 + (23*32) + 19;
	
	ShowState(vaddr,SwitherStatus);
	//else ShowState(vaddr,false);
}



void ShowLoop(void)
{   
//	uint vaddr = BASE5 + (23*32) + 25;
//	if (PT3_state & Bit4) ShowState(vaddr,true);
//	else ShowState(vaddr,false);
	
	boolean SwitherStatus = false;
	uint vaddr;
	SwitherStatus = (PT3_state & PT3_LOOP);
	
	if (SwitherStatus==GUI_Loop) return;
	GUI_Loop = SwitherStatus;
	
	vaddr = BASE5 + (23*32) + 25;	
	ShowState(vaddr,SwitherStatus);
}



void ShowENDsong(void)
{   
//	uint vaddr = BASE5 + (23*32) + 30;
//	if (Player_IsEnd()==true) ShowState(vaddr,true);
//	else ShowState(vaddr,false);
	
	boolean SwitherStatus = false;
	uint vaddr;
	
	SwitherStatus = (PT3_state & PT3_END);	//Player_IsEnd();
	
	if (SwitherStatus==GUI_End) return;
	GUI_End = SwitherStatus;
	
	vaddr = BASE5 + (23*32) + 30;	
	ShowState(vaddr,SwitherStatus);
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
/*    if (state==true) tile=157;
    else tile=189;
    VPOKE(vaddr++,tile++);
    VPOKE(vaddr++,tile++);
    VPOKE(vaddr,tile);*/
	
	if (state==true) tile=156;
    else tile=188;
	VPOKE(vaddr,tile);
}



void ShowVumeter(char channel, char value)
{
	uint vaddr;	

	char tile;
	char i=0;
	char migval;
	
	vaddr=0x1A81 + channelPos[channel];
	if(value>16) value=16;
	
	if(value>0)
	{
		value--;
		if(value>1)
		{
			migval=value>>1;	//div per 2
			//if(migval>7) migval=7;
			for(i=0;i<migval;i++)
			{
				tile=VumeterTile[i];
				VPOKE(vaddr,tile);
				vaddr-=32;
			}
		}

		tile=VumeterTile[i];
		if (!(value&1)) tile++;
		VPOKE(vaddr,tile);
		vaddr-=32;
		i++;
	}
	
	while(i<8)
	{
		VPOKE(vaddr,32);
		vaddr-=32;
		i++;
	}
}
