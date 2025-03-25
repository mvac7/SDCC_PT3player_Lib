/* =============================================================================
VDP PRINT MSX SDCC Library (fR3eL Project)
Functions for display text strings in the graphic modes of the TMS9918A(G1 & G2)
https://github.com/mvac7/SDCC_MSX_fR3eL
============================================================================= */
#ifndef __VPRINT_H__
#define __VPRINT_H__


extern char VPRINT_ntext[6];
extern unsigned int VPRINT_vaddr;



/* =============================================================================
  VLOCATE
  
  Description:
    Moves the cursor to the specified location and optionally 
    provides the VRAM address from the pattern name table, 
    for the indicated screen position.
  
  Inputs:
    column (char) 0 - 31 (G1 or G2 modes) 0-39 (Text mode)
    line   (char) 0 - 23
    
  Outputs: -
============================================================================= */
void VLOCATE(char column, char line);




unsigned int GetStringLength(char* text);




/* =============================================================================
  VPRINT
  
  Description:
    Prints a string of characters on the screen.
    Places it in the position indicated by VLOCATE or in the last printed position.
    
 Inputs:
    [char*] string 
============================================================================= */
void VPRINT(char* text);



/* =============================================================================
  VPRINTN

  Description:
    Prints a character string with a limited length on the screen.

 Inputs:
    text (char*) string
    length (unsigned int) length of the string to print
============================================================================= */
void VPRINTN(char* text, unsigned int length);



/* =============================================================================
  VPrintNumber
  
  Description:
    Prints a number.
   
  Inputs:
    [unsigned int] number
    [char] length
============================================================================= */
void VPrintNumber(unsigned int value);



void VPrintFNumber(unsigned int value, char emptyChar, char length);



/* =============================================================================
  num2Dec16

  16-bit Integer to ASCII (decimal)
  original code by baze http://baze.sk/3sc/misc/z80bits.html#5.1
  (update) Add functionality to replace leading zeros by spaces.  
  
  Inputs:
    - value (unsigned int)  
    - *address (unsigned int) pointer to the string where the number is to be translated 
============================================================================= */
void num2Dec16(unsigned int aNumber, unsigned int variables);



//void VPRINTO(byte column, byte line, char* text, char offset);
//void vpokeBlockOffset(uint vaddr, char* text, char offset);
//void vprintLines(byte column, byte line, char* text)



#endif