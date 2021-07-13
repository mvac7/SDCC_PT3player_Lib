/* =============================================================================
   AY38910BF
   PSG AY-3-8910 Buffer MSX SDCC Library (fR3eL Project)
   Version: 0.9b (7 July 2021)
   Author: mvac7
   Architecture: MSX
   Format: C Object (SDCC .rel)
   Programming language: C and Z80 assembler
   
   Description:                                                              
     Library for accessing the AY38910 through a buffer of registers.
     
     It does not use the BIOS so it can be used to program ROMs or 
     MSX-DOS executables. 
============================================================================= */

#ifndef  __AY38910BF_H__
#define  __AY38910BF_H__



// boolean
//#ifndef _BOOLEAN
//#define _BOOLEAN
//  typedef enum {false = 0, true = 1} boolean;
//#endif
#ifndef _SWITCHER
#define _SWITCHER
  typedef enum {OFF = 0, ON = 1} SWITCHER;
#endif




//envelope shapes
#define AY_ENV_LowerBeat      1  //(0,1,2,3 and 9)<-- 0 can be useful in case you need to control when the envelope is triggered
#define AY_ENV_Upper          4  //(4,5,6,7 and 15)
#define AY_ENV_LeftSaw        8
#define AY_ENV_LowerTriangle 10 
#define AY_ENV_LowerHold     11 
#define AY_ENV_RightSaw      12
#define AY_ENV_UpperHold     13
#define AY_ENV_UpperTriangle 14 
 


#ifndef AY_REGISTERS
#define AY_REGISTERS
#define AY_ToneA      0 //Channel A Tone Period (12 bits)
#define AY_ToneB      2 //Channel B Tone Period (12 bits)
#define AY_ToneC      4 //Channel C Tone Period (12 bits)
#define AY_Noise      6 //Noise Period (5 bits)
#define AY_Mixer      7 //Mixer
#define AY_AmpA       8 //Channel Volume A (4 bits + B5 active Envelope)
#define AY_AmpB       9 //Channel Volume B (4 bits + B5 active Envelope)
#define AY_AmpC      10 //Channel Volume C (4 bits + B5 active Envelope)
#define AY_EnvPeriod 11 //Envelope Period (16 bits)
#define AY_EnvShape  13 //Envelope Shape
#endif



// AY channels
#define AY_Channel_A 0
#define AY_Channel_B 1
#define AY_Channel_C 2


// AY type 
#define AY_INTERNAL 0
#define AY_EXTERNAL 1



extern char AY_TYPE;      // 1 = External AY  Else Internal AY

extern char AYREGS[14];   // buffer of AY registers





/* =============================================================================
 AY_Init

 Function : Initialize the buffer
 Input    : -
 Output   : -
============================================================================= */
void AY_Init();



/* =============================================================================
 SOUND(register, value)

 Function : Write into a register of PSG
 Input    : [char] register number (0 to 13)
            [char] value
 Output   : -
============================================================================= */
void SOUND(char reg, char value);



/* =============================================================================
 GetSound(register) 

 Function : Read PSG register value
 Input    : [char] register number (0 to 13)
 Output   : [char] value 
============================================================================= */
char GetSound(char reg);



/* =============================================================================
 SetTonePeriod(channel, period) 

 Function : Set Tone Period for any channel
 Input    : [char] channel (0, 1 or 2)
            [unsigned int] period (0 - 4095)
 Output   : -
============================================================================= */
void SetTonePeriod(char channel, unsigned int period);



/* =============================================================================
 SetNoisePeriod(period) 

 Function : Set Noise Period
 Input    : [char] Noise period (0 - 31) 
 Output   : - 
============================================================================= */
void SetNoisePeriod(char period);



/* =============================================================================
 SetEnvelopePeriod(period) 

 Function : Set Envelope Period
 Input    : [unsigned int] Envelope period (0 - 65535) 
 Output   : - 
============================================================================= */
void SetEnvelopePeriod(unsigned int period);



/* =============================================================================
 SetVolume(channel, volume) 

 Function : Set volume channel
 Input    : [char] channel (0, 1 or 2)
            [char] volume, 0 to 15 or 16 for activate envelope
 Output   : -
============================================================================= */
void SetVolume(char channel, char volume);



/* =============================================================================
 SetChannel(channel, isTone, isNoise)

 Function : Mixer. Enable/disable Tone and Noise channels.
 Input    : [char] channel (0, 1 or 2)
            [SWITCHER] tone state (ON=1;OFF=0)
            [SWITCHER] noise state (ON=1;OFF=0)
 Output   : -
============================================================================= */
void SetChannel(char channel, SWITCHER isTone, SWITCHER isNoise);



/* =============================================================================
 SetEnvelope(shape) 

 Function : Set envelope shape.
            Plays the sound on channels that have a volume of 16.
 Input    : [char] Envelope shape (0-15) (see envelope shapes definitions)
 Output   : -
============================================================================= */
void SetEnvelope(char shape);



/* =============================================================================
 PlayAY() 

 Function : Copy buffer to AY (internal or external).
 Input    : -
 Output   : -
============================================================================= */
void PlayAY();



#endif