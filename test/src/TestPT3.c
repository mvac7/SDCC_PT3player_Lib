/* =============================================================================
  Test PT3 player Library for SDCC (object type)
  Version: 1.2 (3/4/2018)
  Author: (test program) mvac7/303bcn
  Architecture: MSX
  Format: C Object (SDCC .rel)
  Programming language: C
  WEB: 
  mail: mvac7303b@gmail.com
   
  Description:

    
  History of versions:

============================================================================= */

#include "../include/newTypes.h"
#include "../include/msxSystemVariables.h"
#include "../include/msxBIOS.h"

#include "../include/textmode.h"
#include "../include/interrupt.h"
#include "../include/PT3player.h"

#include "../include/PT3data.h"



#define  HALT __asm halt __endasm   //wait for the next interrupt


#define BASE13 0x1B00 // base 13 sprite attribute table
#define BASE14 0x3800 // base 14 sprite pattern table

#define VDPVRAM   0x98 //VRAM Data (Read/Write)
#define VDPSTATUS 0x99 //VDP Status Registers


 

// ----------------------------------------------------------------------------- PT3 player
#define VARS    0xE200
#define AYREGS  VARS+93
#define _PT3WRK AYREGS+256

#define PT3_SETUP   _PT3WRK /* set bit0 to 1, if you want to play without LOOPING
                                 bit7 is set each time, when loop point is passed */
#define PT3_MODADDR _PT3WRK+1	  // 2 bytes dir musica guardada
#define PT3_CrPsPtr _PT3WRK+3   // 2 bytes POSICION CURSOR EN PATTERN
#define PT3_SAMPTRS _PT3WRK+5	  // 2 bytes sample info?
#define PT3_OrnPtrs _PT3WRK+7   // 2 bytes Ornament pattern
#define PT3_PDSP    _PT3WRK+9	  // 2 bytes pilasave
#define PT3_CSP     _PT3WRK+11	// 2 bytes pilsave2
#define PT3_PSP     _PT3WRK+13	// 2 bytes pilsave3

#define PT3_PrNote  _PT3WRK+15  //	1
#define PT3_PrSlide _PT3WRK+16  //	2
                    
#define PT3_AdInPtA _PT3WRK+18  //2 bytes play data pattern
#define PT3_AdInPtB _PT3WRK+20  //2 bytes play data
#define PT3_AdInPtC _PT3WRK+22  //2 bytes play data
                    
#define PT3_LPosPtr _PT3WRK+24  //2 bytes Pos Ptr
#define PT3_PatsPtr _PT3WRK+26  //2 bytes Pat Ptr

#define PT3_Delay   _PT3WRK+28  //1 byte delay
#define PT3_AddToEn _PT3WRK+29  //1 byte Envelope data (No cal ya que no usa Envs??)
#define PT3_Env_Del _PT3WRK+31  //1 byte Envelope data (idem)
#define PT3_ESldAdd _PT3WRK+32  //2 bytes Envelope data (idem) 
// -----------------------------------------------------------------------------




// Function Declarations -------------------------------------------------------
void SetSpritesSize(char size);

char PEEK(uint address);
unsigned int PEEKW(unsigned int address);

void POKE(uint address, char value);

char VPEEK(uint address);

char INKEY();

void WAIT(uint cicles);


void ShowVumeter(char channel, char value);

void SetSPRITES();



// constants  ------------------------------------------------------------------
const char text01[] = "Test PT3 player Lib for SDCC";
const char text02[] = "v1.0 (22/05/2019)";

const char presskey[] = "Press a key to continue";



// global variable definition --------------------------------------------------

char VALUE;

char SPRBUFFER[72];  //20*4 =72B

// Functions -------------------------------------------------------------------


void my_isr0(void) __interrupt 
{
	DI;
	READ_VDP_STATUS;
  
  PT3PlayAY();

__asm  
  ;vuelca a VRAM buffer atributos sprites
  ld   HL,#_SPRBUFFER
  ld   DE,#BASE13  
  ld   BC,#20*4
  call 0x005C  
__endasm;
      
  EI;
}






//
void main(void)
{
  uint conta = 0;
  
  COLOR(WHITE,DARK_BLUE,LIGHT_BLUE);
          
  WIDTH(32);
  SCREEN1();    
  SetSpritesSize(1);  
  
  LOCATE(0,0);
  PRINT(text01);
  PRINT("\n");
  PRINT(text02);  
  PRINT("\n");
  
  PRINT("\nSong name:\n ");
  PRINT(PT3_name);
  PRINT("\nAuthor:\n ");
  PRINT(PT3_author);
  PRINT("\n");
   
  
  //PT3Init((unsigned int) MAKISONG + 100,0);    
  PT3Init((unsigned int) MAKISONG,0);  // (unsigned int) Song data address ; (char) Loop - 0=off ; 1=on 
  
  
  INKEY();
  
  SetSPRITES();
  
  install_isr(my_isr0);
  
  while(1)
  {
    HALT;
    LOCATE(0,10);
    PrintNumber(PEEK(PT3_CrPsPtr)-33);
    
    ShowVumeter(0,PEEK(AYREGS+8));
    ShowVumeter(1,PEEK(AYREGS+9));
    ShowVumeter(2,PEEK(AYREGS+10));
    
    PT3Run();  
  }

/*  
  uninstall_isr();
  
  CLS();
  PRINT("END");
  WAIT(30*5);
*/  
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






char PEEK(uint address)
{
address;
__asm
  push IX
  ld   IX,#0
  add  IX,SP
    
  ld   L,4(IX)
  ld   H,5(IX)
  ld   A,(HL)

  ld   L,A
  pop  IX
__endasm;
}



unsigned int PEEKW(unsigned int address)
{
address;
__asm
  push IX
  ld   IX,#0
  add  IX,SP
     
  ld   L,4(IX)
  ld   H,5(IX)
  ld   E,(HL)
  inc  HL
  ld   D,(HL)
  ex   DE,HL
  
  pop  IX
__endasm;
}


void POKE(uint address, char value)
{
address;value;
__asm
  push IX
  ld   IX,#0
  add  IX,SP
    
  ld   L,4(IX)
  ld   H,5(IX)
  ld   A,6(IX)
  ld   (HL),A

  pop  IX  
__endasm;
}



char VPEEK(uint address)
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
__endasm;
}



/* =============================================================================
One character input (waiting)
============================================================================= */
char INKEY(){
__asm   
   call CHGET
   ld   L,A
__endasm;
}



// Generates a pause in the execution of n interruptions.
// PAL: 50=1second. ; NTSC: 60=1second. 
void WAIT(uint cicles)
{
  uint i;
  for(i=0;i<cicles;i++) HALT;
  return;
}












/*void testPRINT()
{
  PRINT(">Test PRINT():\n");
  PRINT(testString);
  PRINT("\n\n>Test PRINT() with Escape Sequences:");
  PRINT("\n\tLine 1\n\tLine 2\n\tLine 3"); // \t Horizontal Tab
  PRINT("\r");  // Carriage Return
  PRINT(">\n");
  PRINT("\\");  // Backslash
  PRINT("\a");  // Beep
  PRINT("\n");  // Newline
  //PRINT("\f");  // Formfeed (CLS)
  //PRINT("\v");  // Place the cursor at the top of the screen
  PRINT("\'Single quotation mark\'\n");  // Single quotation mark
  PRINT("\"Double quotation mark\"\n");         // \" Double quotation mark
  PRINT("Question mark\?\n");  // Question mark
  
  LOCATE(0,22);
  PRINT(presskey);
  INKEY();
}*/




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