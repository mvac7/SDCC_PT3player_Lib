/* =============================================================================
  VDP PRINT MSX SDCC Library (fR3eL Project)

  Description:
     Functions Library for display text strings in the graphic modes of the 
     TMS9918A (G1 and G2).   
============================================================================= */
#ifndef __VPRINT_H__
#define __VPRINT_H__



/* =============================================================================
 It provides the address of the video memory map tiles, from the screen position
 indicated.
 
 Inputs:
   column (byte) 0 - 31
   line (byte) 0 - 23
============================================================================= */
unsigned int GetVRAMaddressByPosition(char column, char line);



/* =============================================================================
 Prints a string at the indicated screen position
 
 Inputs:
   column (byte) 0 - 31
   line (byte) 0 - 23
   text (char*) string
============================================================================= */
void VPRINT(char column, char line, char* text);



/* =============================================================================
 Prints a string at the indicated screen position
 
 Inputs:
   column (byte) 0 - 31
   line (byte) 0 - 23
   text (char*) string
   length (uint) length of the string to print
============================================================================= */
void VPRINTN(char column, char line, char* text, unsigned int length);



/* =============================================================================
 Dump the contents of an array of char in a position shown in the video memory

 Inputs:
   vaddress (uint) 
   text (char*) array
============================================================================= */
void VPrintString(unsigned int vaddress, char* text);



/* =============================================================================
   VPrintNumber
   Prints a number at the specified position on the screen.
   
   Inputs:
     [char] column 0 - 31
     [char] line   0 - 23
     [unsigned int] number
     [char] length
============================================================================= */
void VPrintNumber(char column, char line, unsigned int value, char length);



/* =============================================================================
   VPrintNum
   Prints a number at the specified position on the screen.
   
   Inputs:
     [unsigned int] VRAM address in Pattern Name Table.
     [unsigned int] number
     [char] length
============================================================================= */
void VPrintNum(unsigned int vaddr, unsigned int value, char length);



/* =============================================================================
 16-bit Integer to ASCII (decimal)
 original code by baze http://baze.sk/3sc/misc/z80bits.html#5.1
 (update) Add functionality to replace leading zeros by spaces.  
 Input: HL = number to convert, DE = location of ASCII string
 Output: ASCII string at (DE)
============================================================================= */
void num2Dec16(unsigned int aNumber, char *address);



//void VPRINTO(byte column, byte line, char* text, char offset);
//void vpokeBlockOffset(uint vaddr, char* text, char offset);
//void vprintLines(byte column, byte line, char* text)



#endif