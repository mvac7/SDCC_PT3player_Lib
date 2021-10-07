/* =============================================================================
   SDCC Keyboard Functions Library
   Version: 1.0 (7 March 2016)
   Author: mvac7 
   Architecture: MSX
   Programming language: C
   Format: C object (SDCC .rel)   
   mail: mvac7303b@gmail.com

   Description:
     Functions for reading the keyboard of MSX computers.     
============================================================================= */
#ifndef __KEYBOARD_H__
#define __KEYBOARD_H__



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


/* =============================================================================
   KillBuffer
 
   Function : Clear keyboard buffer
   Input    : -
   Output   : -
============================================================================= */
void KillBuffer();



/* =============================================================================
   INKEY
  
   Function : One character input (waiting) and return its code
   Input    : -
   Output   : [char] key code
============================================================================= */
char INKEY();



/* =============================================================================
   GetKeyMatrix

   Function : Returns the value of the specified line from the keyboard matrix.
              Each line provides the status of 8 keys.
              To know which keys correspond, you will need documentation that 
              includes a keyboard table.
   Input    : [char] row 
   Output   : [char] state of the keys. 1 = not pressed; 0 = pressed
============================================================================= */
char GetKeyMatrix(char row);




#endif