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


// Function Declarations -------------------------------------------------------
char PEEK(uint address);
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
  
  PT3Play();  
      
  EI;
}






//
void main(void)
{
  COLOR(WHITE,DARK_BLUE,LIGHT_BLUE);
          
  SCREEN1();
  WIDTH(32);  
  
  LOCATE(0,0);
  PRINT(text01);
  PRINT("\n");
  PRINT(text02);  
  PRINT("\n\n");
  
  PRINT("Song name:\n >");
  PRINT(PT3_name);
  PRINT("\nAuthor:\n >");
  PRINT(PT3_author);
  PRINT("\n");
  
  INKEY();
      
  PT3Init((unsigned int) MAKISONG + 100,0);  // (unsigned int) Song data address ; (char) Loop - 0=off ; 1=on 
  
  install_isr(my_isr0);
  
  while(1)
  {
    HALT;  
  }
  
  uninstall_isr();
  
  CLS();
  PRINT("END");
  WAIT(30*5);  
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




