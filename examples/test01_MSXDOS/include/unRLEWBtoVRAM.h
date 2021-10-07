/* =============================================================================
                                                                           
unRLEWBtoVRAM.h                                                                                                                                

 WB RLE  v1.1

 $80 nn dd            ; run of n consecutive identical bytes ($1>$FE), value dd
 $80 $0               ; for one $80 value
 $80 $FF              ; end of data block
 <any other value>    ; raw data                                                               

============================================================================= */
#ifndef  __UNWBRLE2VRAM_H__
#define  __UNWBRLE2VRAM_H__



/* =============================================================================
 Function : unpack RLE-WB to VRAM
  
 Input    : [unsigned int] source RAM RLE data address
            [unsigned int] target VRAM address       
 Output   : -                  
============================================================================= */
extern void unRLEWBtoVRAM (unsigned int, unsigned int);



/* =============================================================================
unRLEWBtoVR (for Assembler in-line)

 Function : unpack RLE-WB to VRAM
     
 Input    : HL - source RAM RLE data address
            DE - target VRAM address
 Output   : -                 
============================================================================= */



#endif