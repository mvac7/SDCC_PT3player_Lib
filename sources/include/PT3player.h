/* =============================================================================
   SDCC Vortex Tracker II PT3 player for MSX

   Version: 1.1.10 (05/10/2021)
   Architecture: MSX
   Format: C Object (SDCC .rel)
   Programming language: C and Z80 assembler

   Authors:
    - Vortex Tracker II v1.0 PT3 player for ZX Spectrum by S.V.Bulba 
      <vorobey@mail.khstu.ru> http://bulba.at.kz
    - (09-Jan-05) Adapted to MSX by Alfonso D. C. aka Dioniso 
      <dioniso072@yahoo.es>
    - Arrangements for MSX ROM: MSXKun/Paxanga soft > 
      http://paxangasoft.retroinvaders.com/
    - asMSX version: SapphiRe > http://www.z80st.es/
    - Adapted to SDCC: mvac7/303bcn > <mvac7303b@gmail.com>

   Description:
     Adaptation of the Vortex Tracker II PT3 Player for MSX to be used in 
     software development in C (SDCC). 
     
     Requires the AY38910BF library  https://github.com/mvac7/SDCC_AY38910BF_Lib 

In this replayer:

Dioniso version:
 - No version detection (just for Vortex Tracker II and PT3.5).
 - No frequency table decompression (default is number 2). 
   Coger tabla segun quiera, en fichero aparte
 - No volume table decompression (Vortex Tracker II/PT3.5 volume table used).


msxKun version:
 - Usable desde ROM (solo tiene en RAM area de trabajo, lo minimo posible).

SapphiRe version:
 This version of the replayer uses a fixed volume and note table, if you need a 
 different note table you can copy it from TABLES.TXT file, distributed with the
 original PT3 distribution. This version also allows the use of PT3 commands.

 PLAY and PSG WRITE routines seperated to allow independent calls


mvac7 version:
 Adaptation to C (SDCC) of the SapphiRe version.
 
============================================================================= */
#ifndef  __PT3_PLAYER_H__
#define  __PT3_PLAYER_H__





#ifndef _SWITCHER
#define _SWITCHER
  typedef enum {OFF = 0, ON = 1} SWITCHER;
#endif







/* --- Workarea --- (apunta a RAM que estaba antes en codigo automodificable)
 -El byte de estado en SETUP deberia ser algo asi (CH enable/disable no esta aun)
|EP|0|0|0|CH3|CH2|CH1|LP|

LP: Loop enable/disable. A 1 si queremos que el tema suene solo una vez. 
EP: End point. A 1 cada vez que el tema acaba. 
CH1-CH3: Channel enable/disable. A 1 si no queremos que suene el canal. (AUN  NO VA!!)
*/
//extern char PT3_SETUP;   set bit0 to 1, if you want to play without looping
//				           bit7 is set each time, when loop point is passed  
/*            
Switches: 1=ON; 0=OFF
- BIT 0 = ?
- BIT 1 = PLAYER ON/OFF
- BIT 2 = ?
- BIT 3 = ?
- BIT 4 = LOOP ON/OFF
- BIT 7 = is END? YES/NO
*/
extern char PT3_state; //before called PT3_SETUP

//#define Bit0 1      //(0)
#define PT3_PLAY   2  //(1) ON/OFF
//#define Bit2 4
//#define Bit3 8
#define PT3_LOOP  16  //(4) ON/OFF
//#define Bit5 32
//#define Bit6 64
#define PT3_END  128   //(7) is END? YES/NO

//example: if (PT3_state & PT3_PLAY) Player_Decode();



extern unsigned int PT3_MODADDR;  //direccion datos canción
extern unsigned int PT3_CrPsPtr;  //POSICION CURSOR EN PATTERN
extern unsigned int PT3_SAMPTRS;  //sample info?
extern unsigned int PT3_OrnPtrs;  //Ornament pattern

extern unsigned int PT3_PDSP;     //pilasave
extern unsigned int PT3_CSP;      //pilsave2
extern unsigned int PT3_PSP;      //pilsave3
  
extern char PT3_PrNote;
extern unsigned int PT3_PrSlide;
  
extern unsigned int PT3_AdInPtA;  //play data pattern
extern unsigned int PT3_AdInPtB;  //play data
extern unsigned int PT3_AdInPtC;  //play data
  
extern unsigned int PT3_LPosPtr;  //Position Ptr?
extern unsigned int PT3_PatsPtr;  //Pat Ptr

extern char PT3_Delay;            //delay
extern char PT3_AddToEn;          //Envelope data (No cal ya que no usa Envs??)
extern char PT3_Env_Del;          //Envelope data (idem)
extern unsigned int PT3_ESldAdd;  //Envelope data (idem)



//extern unsigned int NoteTable;   //note table memory address
//extern char NoteTable[192];       //Note table




/* =============================================================================
 Player_Init
 Description: Initialize the Player
 Input:       -
 Output:      -
============================================================================= */
void Player_Init();



/* =============================================================================
 Player_Loop
 Description: Change loop state
 Input:       - (char or SWITCHER definition) 0=OFF ; 1=ON
 Output:      -
============================================================================= */
void Player_Loop(char loop); 



/* =============================================================================
 Player_Pause
 Description: Pause song playback
 Input:       -
 Output:      -
============================================================================= */
void Player_Pause();



/* =============================================================================
 Player_Resume
 Description: Resume song playback
 Input:       -
 Output:      -
============================================================================= */  	
void Player_Resume();



/* -----------------------------------------------------------------------------
 Player_InitSong
 Description: Initialize song
 Input: (unsigned int) Song data address. 
                       If the PT3 binary contains the header it will require 
                       subtracting 100 from this value.
        (unsigned int) Note Table address.
        (char) Loop - 0=off ; 1=on  (false = 0, true = 1));
 Output:      -
----------------------------------------------------------------------------- */
void Player_InitSong(unsigned int songADDR, unsigned int notetableADDR, char loop);



/* -----------------------------------------------------------------------------
 Player_Decode
 Description: Process the next step in the song sequence
 Input:       -
 Output:      - 
----------------------------------------------------------------------------- */
void Player_Decode(); 



/* -----------------------------------------------------------------------------
 Player_IsEnd
 Description: Indicates whether the song has finished playing
 Input:       -
 Output:      [char] 0 = No, 1 = Yes 
----------------------------------------------------------------------------- */
char Player_IsEnd();



// mute functions, 0=off, other=on
//void muteChannelA(char value);
//void muteChannelB(char value);
//void muteChannelC(char value);


#endif