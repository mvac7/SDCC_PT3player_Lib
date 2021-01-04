/* =============================================================================
  Test PT3 player Library for SDCC
  Version: 1.1 (04/01/2021)
  Author: (test program) mvac7 <mvac7303b@gmail.com>
  Architecture: MSX
  Format: ROM 8K
  Programming language: C and Z80 assembler
   
  Description:

    
  History of versions:
    - 1.1 (04/01/2021) assigning the frequency table to NoteTable
    - 1.0 (28/5/2019)
============================================================================= */

#include "../include/newTypes.h"
#include "../include/msxSystemVariables.h"
#include "../include/msxBIOS.h"

#include "../include/textmode.h"
#include "../include/interrupt.h"

#include "../include/PT3player.h"
#include "../include/PT3player_NoteTable2.h"
#include "../include/A_funny_day_with_my_MSX.PT3.h"  //datas con el .PT3



#define  HALT __asm halt __endasm   //Z80 instruction: wait for the next interrupt


#define BASE13 0x1B00 // base 13 sprite attribute table
#define BASE14 0x3800 // base 14 sprite pattern table

#define VDPVRAM   0x98 //VRAM Data (Read/Write)
#define VDPSTATUS 0x99 //VDP Status Registers

 
 






// Function Declarations -------------------------------------------------------
void SetSpritesSize(char size);

char PEEK(uint address);
void POKE(uint address, char value);

//void CopyMEM(unsigned int source, unsigned int destination, unsigned int length);

char VPEEK(uint address);

char INKEY();

void WAIT(uint cicles);

char GetKeyMatrix(char line);


void ShowVumeter(char channel, char value);

void SetSPRITES();

void PT3Stop();



// constants  ------------------------------------------------------------------
const char text01[] = "Test PT3 player Lib for SDCC";
const char text02[] = "v1.1 (04/01/2021)";

const char presskey[] = "Press a key to Play";



// global variable definition --------------------------------------------------

char VALUE;

char SPRBUFFER[72];  //20*4 =72B

boolean keyB7semaphore;

char _isPT3play;

uint firstPATaddr;


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
  char keyPressed;
    
  uint conta = 0;
  uint songStep;
  
  keyB7semaphore=false;
  
  COLOR(WHITE,DARK_BLUE,LIGHT_BLUE);
          
  WIDTH(32);
  SCREEN1();    
  SetSpritesSize(1);  
  
  LOCATE(0,0);
  PRINT(text01);
  PRINT("\n");
  PRINT(text02);
    
  PRINT("\n\n");
  
  PRINT("Song name:\n ");
  PRINT(Song_name);
  
  PRINT("\n\n");
  
  PRINT("Author:\n ");
  PRINT(Song_author);
  
  //PRINT("\n\n");
  
  LOCATE(0,10);
  PRINT(presskey);
  
  //CopyMEM((unsigned int) NT,(unsigned int) NoteTable,96*2);   //Copy Note Table to the space reserved
  NoteTable = (unsigned int) NT;
     
  //PT3Init((unsigned int) SONG00 - 100,0); // Subtract 100 if you delete the header of the PT3 file.    
  PT3Init((unsigned int) SONG00 ,0);  // (unsigned int) Song data address ; (char) Loop - 0=off ; 1=on 
  _isPT3play=1;
  
  firstPATaddr = PT3_CrPsPtr;
  
  INKEY();
  
  LOCATE(0,10);
  PRINT("Press STOP for mute the song"); 
  
  SetSPRITES();
  
  install_isr(my_isr0);
  
  while(1)
  {
    HALT;
    LOCATE(0,12);
    
    songStep=PT3_CrPsPtr - firstPATaddr;
    PRINT("Step: ");
    PrintNumber(songStep);
        
    ShowVumeter(0,AYREGS[AR_AmplA]);
    ShowVumeter(1,AYREGS[AR_AmplB]);
    ShowVumeter(2,AYREGS[AR_AmplC]);
    
    
    

    keyPressed = GetKeyMatrix(7);  
    if (keyPressed!=0xFF)  //pressure control of the keys
    {
      if(keyB7semaphore==false)
      {
        if (keyPressed==0b11101111)  //STOP Key
        {
          keyB7semaphore=true;
          PT3Stop(); 
        }          
      }      
    }else{
      keyB7semaphore=false;        
    }
    
    
    if(_isPT3play==1) PT3Decode();

    
  }

/*  
  uninstall_isr();
  
  CLS();
  PRINT("END");
  WAIT(30*5);
*/  
}



void PT3Stop()
{
  if(_isPT3play==1){
    _isPT3play = 0;
    PT3Mute();
  }else _isPT3play = 1;  
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






char PEEK(uint address) __naked
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
  ret
__endasm;
}



void POKE(uint address, char value) __naked
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
  ret  
__endasm;
}



/* =============================================================================
   CopyMEM
 
   Function : Copy a block of memory to another address.
   Input    : [unsigned int] Source memory address
              [unsigned int] Destination RAM address
              [unsigned int] length 
   Output   : -
============================================================================= */
/*void CopyMEM(unsigned int source, unsigned int destination, unsigned int length) __naked
{
source;destination;length;
__asm
  push IX
  ld   IX,#0
  add  IX,SP
  
  ld   L,4(IX)
  ld   H,5(IX) ;source memory address
  
  ld   E,6(IX)
  ld   D,7(IX) ;Destination RAM address
  
  ld   C,8(IX)
  ld   B,9(IX) ;length
  
  ldir
  
  pop  IX
  ret
__endasm;
}*/




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
One character input (waiting)
============================================================================= */
char INKEY() __naked
{
__asm   
   call CHGET
   ld   L,A
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



/* =============================================================================
   GetKeyMatrix

   Function : Returns the value of the specified line from the keyboard matrix.
              Each line provides the status of 8 keys.
              To know which keys correspond, you will need documentation that 
              includes a keyboard table.
   Input    : [char] line 
   Output   : [char] state of the keys. 1 = not pressed; 0 = pressed
============================================================================= */
char GetKeyMatrix(char line) __naked
{
line;
__asm
  push IX
  ld   IX,#0
  add  IX,SP
     
  ld   A,4(IX)
  
  call SNSMAT
  
  ld   L,A
  
  pop  IX
  ret
__endasm;
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