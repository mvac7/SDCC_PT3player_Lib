/* =============================================================================
  test PT3 Player Library for SDCC
  Version: 1.1 (07/10/2021)
  Author: mvac7
  Architecture: MSX
  Format: .COM (MSX-DOS)
  Programming language: C and Z80 assembler
  
  In examples/test_ROM software, PT3 songs:
   - "A funny day with my MSX" by Makinavaja (for AAMSX 50th RU invitro)
   - "Mathematical brain" by Makinavaja (for XI MICROCOMPO AY) 

    
History of versions:
- v1.1 (07/10/2021) Updates from v1.1.10 PT3player Lib 
- v1.0 (27/08/2021) First version
============================================================================= */

#include "../include/newTypes.h"
#include "../include/msxBIOS.h"
#include "../include/msxDOS.h"
#include "../include/msxSystemVariables.h"

#include "../include/interruptM1_ISR.h"
#include "../include/interruptM1_Hooks.h"
#include "../include/memory.h"
#include "../include/keyboard.h"

#include "../include/VDP_TMS9918A.h"
#include "../include/VDP_SPRITES.h"
#include "../include/VDP_PRINT.h"
#include "../include/unRLEWBtoVRAM.h"

#include "../include/AY38910BF.h"
#include "../include/PT3player.h"
#include "../include/PT3player_NoteTable2.h"

//song-data .PT3
#include "../include/maki_ru50inv_PT3.h"
#include "../include/maki_CompoAY19v2_PT3.h"

#include "../include/Test_GFX.h"



//#define  HALT __asm halt __endasm   //wait for the next interrupt



#define  SYSTEM 0x0005

#define  VUMETERSPRBUFF 12*4



//  definition of functions  ---------------------------------------------------

void Player();

void WAIT(uint cicles);

void System(char code);

void SCREEN0();
void SCREEN1();

void LOCATE(char x, char y);
void PRINT(char* text);


void my_TIMI();

void DumpOAM();

void ShowState(uint vaddr, boolean state);

void InitVumeter();
void ShowVumeter(char channel, char value);

void Pause();

void PlaySong(char songNumber);

void SwapLoop();
void ShowLoop();
void ShowPlayback();
void ShowENDsong();




// constants  ------------------------------------------------------------------
const char text01[] = "Test MSXDOS PT3player Library";
const char text02[] = "v1.0 (27/08/2021)";

const char presskey[] = "Press a key to Play";

const char aytypeSTRINGS[2][7]={"INTERN","EXTERN"};

const char SPRITE_DATA[]={
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x7F,0x7F,0x7F,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFE,0xFE,0xFE,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x7F,0x7F,0x7F,0x00,0x7F,0x7F,0x7F,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFE,0xFE,0xFE,0x00,0xFE,0xFE,0xFE,0x00,
0x00,0x00,0x00,0x00,0x7F,0x7F,0x7F,0x00,0x7F,0x7F,0x7F,0x00,0x7F,0x7F,0x7F,0x00,
0x00,0x00,0x00,0x00,0xFE,0xFE,0xFE,0x00,0xFE,0xFE,0xFE,0x00,0xFE,0xFE,0xFE,0x00,
0x7F,0x7F,0x7F,0x00,0x7F,0x7F,0x7F,0x00,0x7F,0x7F,0x7F,0x00,0x7F,0x7F,0x7F,0x00,
0xFE,0xFE,0xFE,0x00,0xFE,0xFE,0xFE,0x00,0xFE,0xFE,0xFE,0x00,0xFE,0xFE,0xFE,0x00};


// SPRITE ATRIBUTE DATAS #######################################################
// for adjust colors, changue the last valor in line
//Y,X,SPR,COLOR
const char VUMETER[]={
167,8,0,2,
151,8,0,2,
135,8,0,10,
119,8,0,8,
167,24,0,2,
151,24,0,2,
135,24,0,10,             
119,24,0,8, 
167,40,0,2,
151,40,0,2,
135,40,0,10,
119,40,0,8};
//END SPRITE ATRIBUTE DATAS ####################################################



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


//
void main(void)
{
  char colorInk=0;
  char colorBG=0;
  char colorBDR=0;
  char scrcolumns=0;
  
  colorInk=PEEK(FORCLR);
  colorBG=PEEK(BAKCLR);
  colorBDR=PEEK(BDRCLR);
  scrcolumns=PEEK(LINLEN);
  
  //save the old hooks vectors
  Save_ISR();
  Save_TIMI();   
  Save_KEYI(); 
  
  Disable_TIMI();
  Disable_KEYI();
  Disable_ISR();
  
//------------------------------------------------------------------------------   
 
  Player();
  
     
//return to MSXDOS ------------------------------------------------------------------
  //put the screen as it was.
  COLOR(colorInk,colorBG,colorBDR);
//  WIDTH(scrcolumns);

  if(scrcolumns<33) SCREEN1();
  else SCREEN0();
  //
  
  PRINT("End of the test...");
  
  Restore_ISR();
  Restore_TIMI();  
  Restore_KEYI();
    
  System(_TERM0); 
//--------------------------------------------------------------------- end EXIT

}



void Player()
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
                  
  InitVumeter();
  
  Install_TIMI(my_TIMI);
  Install_ISR(ISR_Basic);
  
  
  while(1)
  {
    HALT;
    
    if (PT3_state & PT3_PLAY)
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

 
  //uninstall_isr();

}



void System(char code)
{
code;
__asm
	push IX
	ld IX,#0
	add IX,SP

	ld C,4(ix)
	call SYSTEM

	pop IX
__endasm; 
}



void SCREEN0() __naked
{
__asm

  push IX
  
  ld   A,(#LINLEN)
  ld   (#LINL40),A   ;copy columns seting with WIDTH to LINL40 system var
   
  ld   IX,#INITXT
  ld   IY,(EXPTBL-1)
  call CALSLT
  ei
    
  pop  IX
  ret
__endasm;
}



void SCREEN1() __naked
{
__asm
     
  push IX
  
  ld   A,(#LINLEN)   ;get a last value set with WIDTH function 
  ld   (#LINL32),A   ;set system variable
   
  ld   IX,#INIT32
  ld   IY,(EXPTBL-1)
  call CALSLT
  ei
    
  pop  IX
  ret
__endasm;
}




// Generates a pause in the execution of n interruptions.
// PAL: 50=1second. ; NTSC: 60=1second. 
void WAIT(uint cicles)
{
  uint i;
  for(i=0;i<cicles;i++) HALT;
}





/* =============================================================================
   Set a position the cursor in to the specified location
   Posiciona el cursor hasta la ubicacion especificada
   x(byte) - column (0 to 31 or 39)
   y(byte) - line   (0 to 23)
============================================================================= */
void LOCATE(char x, char y)
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
     
  ;call POSIT
  ld   IX,#POSIT
  ld   IY,(#EXPTBL-1)
  call CALSLT ;acces to MSX BIOS
  
  pop  IX
__endasm;

}



/* =============================================================================
   Print a text in screen
============================================================================= */
void PRINT(char* text)
{ 
text;
__asm
  push IX
  ld   IX,#0
  add  IX,SP
  
  ld   L,4(ix)
  ld   H,5(ix)
  
nextCHAR:  
  ld   A,(HL)
  or   A
  jr   Z,ENDnext
     
  ;call CHPUT //Displays one character (BIOS)
  ld   IX,#CHPUT
  ld   IY,(#EXPTBL-1)
  call CALSLT ;acces to MSX BIOS
  
  inc  HL
  jr   nextCHAR
ENDnext:  
  pop  IX
__endasm; 
}







// Routine for Hook TIMI (VBLANK)
void my_TIMI() 
{  
  PlayAY();
  DumpOAM();
}



void DumpOAM() 
{  
    CopyToVRAM((unsigned int) SPRBUFFER, BASE13, VUMETERSPRBUFF);
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
  Player_Pause();
      
  Player_InitSong(songPT3Data[songNumber], (unsigned int) NT2, _loop);
  
  firstPATaddr = PT3_CrPsPtr;
  
  VPrintNumber(8,10,songNumber+1,1);
  
  VPRINT(8,11,"                       ");
  VPRINTN(8,11,(char*) songNames[songNumber],23);
  
  VPRINT(8,12,"                       ");
  VPRINTN(8,12,(char*) songAuthors[songNumber],23);
}




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




void ShowState(uint vaddr, boolean state)
{
    char tile;
    if (state==true) tile=189;
    else tile=221;
    VPOKE(vaddr++,tile++);
    VPOKE(vaddr++,tile++);
    VPOKE(vaddr,tile);
}



void InitVumeter()
{

  CopyToVRAM((unsigned int) SPRITE_DATA, BASE14, 32*5);
  
  CopyRAM((unsigned int) VUMETER, (unsigned int) SPRBUFFER, VUMETERSPRBUFF);

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





