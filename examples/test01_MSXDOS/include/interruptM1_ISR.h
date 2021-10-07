/* =============================================================================
  Interrupt Mode 1 MSX ISR SDCC Library (fR3eL Project)
  Version: 1.1 (1 September 2021)
  Author: mvac7
  Architecture: MSX
  Format: C Object (SDCC .rel)
  Programming language: C and Z80 assembler

  Description:
    Functions for the control of the ISR of the interrupt Mode 1 in MSX systems.  
  
History of versions:
- v1.1 (1 September 2021) More functions to control ISR

Based on the library created by Avelino Herrera 
                          http://msx.avelinoherrera.com/index_es.html#sdccmsxdos
============================================================================= */

#ifndef  __INTERRUPT_M1_ISR_H__
#define  __INTERRUPT_M1_ISR_H__



#ifndef _iBASICs_
#define _iBASICs_

#define  DisableI         __asm di __endasm
#define  EnableI          __asm ei __endasm

#define  HALT             __asm halt __endasm

#endif





// ----------------------------------------------------------------------------> ISR


/* =============================================================================
 Save_ISR

 Function : Save Old ISR vector
 Input    : -
 Output   : -
============================================================================= */
void Save_ISR();


/* =============================================================================
 Install_ISR

 Function : Set ISR vector  
 Input    : Function address
 Output   : -
============================================================================= */
void Install_ISR(void (*isr)(void));


/* =============================================================================
 Restore_ISR

 Function : Restore old ISR vector 
 Input    : -
 Output   : -
============================================================================= */
void Restore_ISR();


/* =============================================================================
 Disable_ISR

 Function : Disable ISR
            Install an ISR with the minimum code so as not to block the Z80.
 Input    : -
 Output   : -
============================================================================= */
void Disable_ISR();




/* =============================================================================
## Basic ISR for M1 interrupt of Z80

* Saves all Z80 registers on the stack.
* Calls the two hooks of the system: TIMI (VBLANK) and KEYI

Note: 
  You can optimize it, commenting on those records that you know you don't use 
  (such as alternate records) or removing the KEYI hook. 
============================================================================= */
void ISR_Basic();




#endif
