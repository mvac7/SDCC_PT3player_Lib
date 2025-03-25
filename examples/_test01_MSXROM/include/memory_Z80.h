/* =============================================================================
   Z80 Memory Library (fR3eL Project)

   Description:
     Library with basic functions for working with memory on Z80 processors
============================================================================= */

#ifndef __MEMORYZ80_H__
#define __MEMORYZ80_H__


/* =============================================================================
   PEEK
 
   Function : Read a 8 bit value from the RAM.
   Input    : [unsigned int] RAM address
   Output   : [char] value
============================================================================= */
char PEEK(unsigned int address);



/* =============================================================================
   PEEKW
 
   Function : Read a 16 bit value from the RAM
   Input    : [unsigned int] RAM address
   Output   : [unsigned int] value
============================================================================= */
unsigned int PEEKW(unsigned int address);



/* =============================================================================
   POKE
 
   Function : Write an 8 bit value in RAM
   Input    : [unsigned int] RAM address
              [char] value
   Output   : -
============================================================================= */
void POKE(unsigned int address, char value);



/* =============================================================================
   POKEW
 
   Function : Write an 16 bit value in RAM
   Input    : [unsigned int] RAM address
              [unsigned int] value
   Output   : -
============================================================================= */
void POKEW(unsigned int address, unsigned int value);



/* =============================================================================
   CopyRAM
 
   Function : Copy a block of memory to another address.
   Input    : [unsigned int] Source memory address
              [unsigned int] Destination RAM address
              [unsigned int] length 
   Output   : -
============================================================================= */
void CopyRAM(unsigned int source, unsigned int destination, unsigned int length);



/* =============================================================================
   FillRAM
 
   Function : Fills an area of the RAM with a value.
   Input    : [unsigned int] RAM address
              [unsigned int] length
              [char] value
   Output   : -
============================================================================= */
void FillRAM(unsigned int address, unsigned int length, char value);



#endif