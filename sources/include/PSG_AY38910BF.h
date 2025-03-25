/* =============================================================================
PSG_AY38910BF
PSG AY-3-8910 Buffer MSX SDCC Library version (fR3eL Project)
============================================================================= */

#ifndef  __AY38910BF_H__
#define  __AY38910BF_H__



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



//AY envelope shapes
#define AY_ENV_LowerBeat      1  //(0,1,2,3 and 9)<-- 0 can be useful in case you need to control when the envelope is triggered
#define AY_ENV_Upper          4  //(4,5,6,7 and 15)
#define AY_ENV_LeftSaw        8
#define AY_ENV_LowerTriangle 10 
#define AY_ENV_LowerHold     11 
#define AY_ENV_RightSaw      12
#define AY_ENV_UpperHold     13
#define AY_ENV_UpperTriangle 14 



//AY channels
#define AY_Channel_A 0
#define AY_Channel_B 1
#define AY_Channel_C 2



//AY port 
#define AY_INTERNAL 0xA0	//Internal MSX PSG
#define AY_EXTERNAL 0x10	//(MEGAFLASHROM SCC+, Flashjacks or others)




extern char AY_IOport;	//new (v1.7) AY first port

extern char AYREGS[14];		// buffer of AY registers




/* =============================================================================
InitAY

Function : Initialize the buffer
Input    : -
Output   : -
============================================================================= */
void InitAY(void);



/* =============================================================================
SOUND(register, value)

Function : Write into a register of PSG
Input    : 
			[char] register number (0 to 13)
			[char] value
Output   : -
============================================================================= */
void SOUND(char reg, char value);



/* =============================================================================
GetSound(register)

Function : Read PSG register value (from buffer)
Input    : [char] register number (0 to 13)
Output   : [char] value 
============================================================================= */
char GetSound(char reg);



/* =============================================================================
SilenceAY

Function:	Silences selected AY sound processor.
Input   :	[char] AY index port
Output  :	-
============================================================================= */
void SilenceAY(void);



/* =============================================================================
SilenceAYbyPort

Function : 
			Silences the indicated AY sound processor.
			Set to zero the amplitude value by writing directly to the AY registers.
			This is indicated for the case of playing sound dynamically between 
			AYs (Internal/External), so that the last written values ​​do not sound 
			infinitely.
Input    : [char] AY index port
Output   : -
============================================================================= */
void SilenceAYbyPort(char AY_port);



/* =============================================================================
PlayAY() 

Function : Copy buffer to selected AY (AY_IOport)
Input    : -
Output   : -
============================================================================= */
void PlayAY(void);



/* =============================================================================
Dump2AY() 

Function : Dump a buffer to the indicated AY
Input    : 
			[char] AY index port
			[unsigned int] buffer address of AY registers
Output   : -
============================================================================= */
void Dump2AY(char AY_port, unsigned int bufferADDR);



#endif