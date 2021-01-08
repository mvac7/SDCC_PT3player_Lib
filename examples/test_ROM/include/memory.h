/* =============================================================================
   SDCC Memory Functions Library (object type)
   Version: 1.1
   Date: 28/06/2018
   Author: mvac7/303bcn
   Architecture: MSX
   Format: C Object (SDCC .rel)
   Programming language: C
   WEB: 
   mail: mvac7303b@gmail.com

   Description:
     Memory access functions
   
   History of versions:
   - v1.1 (28/06/2018)< add slot access functions
   - v1.0 (01/03/2016) First version  
============================================================================= */

#ifndef __MEMORY_H__
#define __MEMORY_H__



// boolean
#ifndef _BOOLEAN
#define _BOOLEAN
  typedef enum {false = 0, true = 1} boolean;
#endif


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



/* =============================================================================
   GetPageSlot
 
   Function : Provide the slot of the indicated page.
   Input    : [char] page (0-3)              
   Output   : [char] slot (0-3)   
============================================================================= */
char GetPageSlot(char page);



/* =============================================================================
   GetPageSubslot
 
   Function : Provide the subslot of the indicated page.
   Input    : [char] page (0-3)              
   Output   : [char] subslot (0-3)   
============================================================================= */
char GetPageSubslot(char page);



/* =============================================================================
   SetPageSlot
 
   Function : Set a slot on the indicated page.
   Input    : [char] page (0-3)
              [char] slot (0-3)
   Output   : -

  
   More Info: (by MSX Assembly Page)  
   http://map.grauw.nl/resources/msx_io_ports.php
           Primary slot select register
               page 0 > 0x0000 - 0x3FFF
               page 1 > 0x4000 - 0x7FFF
               page 2 > 0x8000 - 0xBFFF
               page 3 > 0xC000 - 0xFFFF
============================================================================= */
void SetPageSlot(char page, char slot);



/* =============================================================================
   SetPageSubslot
 
   Function : Set a subslot on the indicated page (For expanded slots)
   Input    : [char] page (0-3)
              [char] subslot (0-3)
   Output   : -

  
   More Info: (by MSX Assembly Page)  
   http://map.grauw.nl/resources/msx_io_ports.php
               
           Secondary slot select register:
               The subslot select register can be found at memory address #FFFF:
               0-1 Subslot for page 0 (#0000-#3FFF)
               2-3 Subslot for page 1 (#4000-#7FFF)
               4-5 Subslot for page 2 (#8000-#BFFF)
               6-7 Subslot for page 3 (#C000-#FFFF)
============================================================================= */
void SetPageSubslot(char page, char subslot);



/* =============================================================================
   IsExpanded
 
   Function : Returns if the slot is of the expanded type.
   Input    : [char] slot (0-3)
   Output   : [boolean] true = Yes; false = No
   
        EXPTBL 0xFCC1 Slot 0 expanded?
               0xFCC2 Slot 1 expanded?
               0xFCC3 Slot 2 expanded?
               0xFCC4 Slot 3 expanded?
               Yes = 0x80 ; No = 0
============================================================================= */
boolean IsExpanded(char slot);


/*

char WhereIsRAMslot() or SearchRAMslot();

*/



#endif