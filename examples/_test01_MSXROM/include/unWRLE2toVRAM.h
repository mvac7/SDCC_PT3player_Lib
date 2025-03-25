/*============================================================================== 
 unWRLE2toVRAM
 Decompress WonderRLE2 (WRLE2) data to VRAM
                                                              
-------------------------------------------------------------------------------
 CD = Control Digit = $80

 CD + $00				--> End
 CD + ($01-$03)			--> write Control Digit ($80) 1,2 or 3 times.
 CD + ($04-$FF) + val	--> repeats the defined value, n times
 val  (!= CD)			--> Values ​​without repetition or 
							that are repeated less than 4 times			                             
==============================================================================*/
#ifndef  __UNWRLE2VRAM_H__
#define  __UNWRLE2VRAM_H__



/* =============================================================================
 unWRLE2toVRAM16
 Function : Decompress WRLE2 data to 16K VRAM (TMS9918A)
  
 Input    : [unsigned int] dataAddr <- source RLEWB data address
            [unsigned int] VRAMaddr <- target VRAM address 14bits (16K)
 or call unWRLE2toVRAM from assembler in line:
			HL - source RLEWB data address
			DE - target VRAM address  
 Output   : -                  
============================================================================= */
void unWRLE2toVRAM16(unsigned int dataAddr, unsigned int VRAMaddr);



/* =============================================================================
 unWRLE2toVRAM128
 Function : Decompress WRLE2 data to 128K VRAM (V9938 or higher)
  
 Input    : [unsigned int] dataAddr <- source RLEWB data address
            [unsigned int] VRAMaddr <- target VRAM address 16bits 
			[char] VRAM bank - First or second 64K bank of VRAM (0 or 1)
 Output   : -                  
============================================================================= */
void unWRLE2toVRAM128(unsigned int dataAddr, unsigned int VRAMaddr, char bank);



#endif