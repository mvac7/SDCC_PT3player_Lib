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





// Function Declarations -------------------------------------------------------
char PEEK(uint address);
unsigned int PEEKW(unsigned int address);

void POKE(uint address, char value);

char VPEEK(uint address);

char INKEY();

void WAIT(uint cicles);


// constants  ------------------------------------------------------------------
const char text01[] = "Test PT3 player Lib for SDCC";
const char text02[] = "v1.0 (22/05/2019)";

const char presskey[] = "Press a key to continue";
// global variable definition --------------------------------------------------




// Functions -------------------------------------------------------------------


void my_isr0(void) __interrupt 
{
	DI;
	READ_VDP_STATUS;
  
  PT3PlayAY();  
      
  EI;
}






//
void main(void)
{
  uint conta = 0;
  
  COLOR(WHITE,DARK_BLUE,LIGHT_BLUE);
          
  SCREEN1();
  WIDTH(32);  
  
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
  
  install_isr(my_isr0);
  
  while(1)
  {
    HALT;
    LOCATE(0,10);
    PrintNumber(PEEK(PT3_CrPsPtr)-33);
    
    PT3Run();  
  }

/*  
  uninstall_isr();
  
  CLS();
  PRINT("END");
  WAIT(30*5);
*/  
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




