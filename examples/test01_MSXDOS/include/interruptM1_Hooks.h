/* =============================================================================
  Interrupt Mode 1 MSX Hooks SDCC Library (fR3eL Project)
  Version: 1.1 (4 July 2021)
  Author: mvac7
  Architecture: MSX
  Format: C Object (SDCC .rel)
  Programming language: C and Z80 assembler

  Description:
    Functions to control of the hooks (TIMI and KEYI) used in the ISR of the 
    Mode 1 interrupt included in the MSX system (BIOS/MSX-DOS).    
  
  History of versions:
   - v1.1 (4 July 2021)
   - v1.0 (4 July 2011) Published in Avelino Herrera's WEB 
                        http://msx.avelinoherrera.com/index_es.html#sdccmsx
                        
Based on the MSX-DOS library created by Avelino Herrera  
                          http://msx.avelinoherrera.com/index_es.html#sdccmsxdos
============================================================================= */

#ifndef  __INTERRUPT_H__
#define  __INTERRUPT_H__


#ifndef _iBASICs_
#define _iBASICs_

#define  DisableI         __asm di __endasm
#define  EnableI          __asm ei __endasm

#define  HALT             __asm halt __endasm

#endif


#define  PUSH_AF          __asm push AF __endasm
#define  POP_AF           __asm pop  AF __endasm







/* =============================================================================
 Save_TIMI

 Function : Save TIMI hook vector
 Input    : -
 Output   : -
============================================================================= */
void Save_TIMI();



/* =============================================================================
 Install_TIMI

 Function : Set new TIMI hook vector  
 Input    : Function address
 Output   : -
============================================================================= */
void Install_TIMI(void (*func)(void));



/* =============================================================================
 Restore_TIMI

 Function : Restore old TIMI hook vector 
 Input    : -
 Output   : -
============================================================================= */
void Restore_TIMI();



/* =============================================================================
 Disable_TIMI

 Function : Disable the TIMI hook (Add a ret on the hook).
 Input    : -
 Output   : -
============================================================================= */
void Disable_TIMI();



/* =============================================================================
 Save_KEYI

 Function : Save KEYI hook vector
 Input    : -
 Output   : -
============================================================================= */
void Save_KEYI();



/* =============================================================================
 Install_KEYI

 Function : Set new KEYI hook vector  
 Input    : Function address
 Output   : -
============================================================================= */
void Install_KEYI(void (*func)(void));



/* =============================================================================
 Restore_KEYI

 Function : Restore old KEYI hook vector 
 Input    : -
 Output   : -
============================================================================= */
void Restore_KEYI();



/* =============================================================================
 Disable_KEYI

 Function : Disable the KEYI hook (Add a ret on the hook).
 Input    : -
 Output   : -
============================================================================= */
void Disable_KEYI();



#endif
