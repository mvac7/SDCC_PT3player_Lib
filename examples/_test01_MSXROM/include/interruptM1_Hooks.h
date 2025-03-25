/* =============================================================================
Z80 interrupt M1 Hooks MSX SDCC Library (fR3eL Project)
Library with basic functions to control the MSX system hooks called from the 
Z80 Mode 1 interrupt ISR (Interrupt Service Routine).  
https://github.com/mvac7/SDCC_MSX_fR3eL
============================================================================= */

#ifndef  __INTERRUPT_M1_HOOKS_H__
#define  __INTERRUPT_M1_HOOKS_H__


#ifndef _Z80interruptDefs_
#define _Z80interruptDefs_

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
void Save_TIMI(void);



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
void Restore_TIMI(void);



/* =============================================================================
 Disable_TIMI

 Function : Disable the TIMI hook (Add a ret on the hook).
 Input    : -
 Output   : -
============================================================================= */
void Disable_TIMI(void);



/* =============================================================================
 Save_KEYI

 Function : Save KEYI hook vector
 Input    : -
 Output   : -
============================================================================= */
void Save_KEYI(void);



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
void Restore_KEYI(void);



/* =============================================================================
 Disable_KEYI

 Function : Disable the KEYI hook (Add a ret on the hook).
 Input    : -
 Output   : -
============================================================================= */
void Disable_KEYI(void);




#endif
