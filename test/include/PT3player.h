/* =============================================================================
   PTplayer.h

   Description:
   library for play PT3 songs in SDCC
   Adaptación de la versión para MSX de MSXKun/Paxanga soft
   
   In this replayer:
   -No version detection (just for Vortex Tracker II and PT3.5).
   -No frequency table decompression (default is number 2). - Coger tabla segun quiera, en fichero aparte
   -No volume table decompression (Vortex Tracker II/PT3.5 volume table used).

   Warning!!! Delete 100 first bytes (header) from your PT3 module or add 100 to data addres. <<<<<<<<<<<<<<<<<<<<<< #####################

   - New added-

   Usable desde ROM (solo tiene en RAM area de trabajo, lo minimo posible).
 ============================================================================ */
#ifndef  __PT3_PLAYER_H__
#define  __PT3_PLAYER_H__


/* -----------------------------------------------------------------------------
PT3Init
(unsigned int) Song data address
(char) Loop - 0=off ; 1=on  (false = 0, true = 1));
----------------------------------------------------------------------------- */
void PT3Init(unsigned int,char);

void PT3Play();  //Play Song. Execute on each interruption of VBLANK

void PT3Stop();  //Stop Song.

//void PT3Loop(char);  //0=off ; 1=on  (false = 0, true = 1)

//void RESUME();
//void LOAD_SONG(char numpatt);
//void NEXT_PATTERN(char numpatt);
//void MUTE(void);


// mute functions, 0=off, other=on
//void muteChannelA(char value);
//void muteChannelB(char value);
//void muteChannelC(char value);


#endif