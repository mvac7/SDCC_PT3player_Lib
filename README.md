# PT3 Player MSX SDCC Library (fR3eL Project)

```
Architecture: MSX
Format: C Object (SDCC .rel)
Programming language: C and Z80 assembler


Authors: 
- Vortex Tracker II v1.0 PT3 player for ZX Spectrum by S.V.Bulba <vorobey@mail.khstu.ru> http://bulba.at.kz
- Adapted to MSX by Alfonso D. C. aka Dioniso <dioniso072@yahoo.es>
- Arrangements for MSX ROM: MSXKun/Paxanga soft > http://paxangasoft.retroinvaders.com/
- asMSX version: SapphiRe > http://www.z80st.es/
- Adapted to SDCC: mvac7/303bcn > <mvac7303b@gmail.com>

In Test/Example software:
- "A funny day with my MSX" PT3 song by Makinavaja 
```

#### PT3 player for asMSX cross-assembler (by SapphiRe): [Nueva version del replayer de PT3](http://www.z80st.es/blog/2008/11/19a-nueva-version-del-replayer-de-pt3)        


## Sorry!: This text is pending correction of the English translation. <<<<<<<<


## History of versions:
- 1.2 (??/01/2021)>Assignment of frequency table memory address to NoteTable, PT3state, PT3_Loop, PT3_Pause and PT3_Resume.
- 1.1 (28/05/2019) Adaptation to SDCC of asMSX version by SapphiRe.
- 1.0 (21/10/2016) Adaptation to SDCC of the ROM version by Kun.


## Introduction

Adaptation of the latest version of Vortex Tracker II PT3 Player for MSX (SapphiRe) for use in software development in C (SDCC).

Allows access to player variables.

Allows you to use any of the four note/frequency tables available in Vortex Tracker. 
They are available in header format to be included in your program.  

In the source code (\examples), you can find applications for testing and learning purposes.

This library is part of the [MSX fR3eL Project](https://github.com/mvac7/SDCC_MSX_fR3eL).

Enjoy it!



## License

Vortex Tracker II v1.0 PT3 player (c) 2004 S.V.Bulba



## Acknowledgments
  
I want to give a special thanks to all those who freely share their knowledge with the MSX developer community.

* S.V.Bulba > [WEB](http://bulba.at.kz)
* Alfonso D. C. aka Dioniso
* MSXKun/Paxanga soft > [WEB](http://paxangasoft.retroinvaders.com/)
* Sapphire/Z80ST > [WEB](http://z80st.auic.es/)
* Avelino Herrera > [WEB](http://msx.atlantes.org/index_es.html)
* Nerlaska > [Blog](http://albertodehoyonebot.blogspot.com.es)
* Marq/Lieves!Tuore > [Marq](http://www.kameli.net/marq/) [Lieves!Tuore](http://www.kameli.net/lt/)
* [Fubukimaru](https://github.com/Fubukimaru) > [Blog](http://www.gamerachan.org/fubu/)
* Andrear > [Blog](http://andrear.altervista.org/home/msxsoftware.php)
* Ramones > [MSXblog](https://www.msxblog.es/tutoriales-de-programacion-en-ensamblador-ramones/) - [MSXbanzai](http://msxbanzai.tni.nl/dev/faq.html)
* Fernando García > [youTube](https://www.youtube.com/user/bitvision)
* Eric Boez > [gitHub](https://github.com/ericb59)
* MSX Assembly Page > [WEB](http://map.grauw.nl/resources/msxbios.php)
* Portar MSX Tech Doc > [WEB](https://problemkaputt.de/portar.htm)
* MSX Resource Center > [WEB](http://www.msx.org/)
* Karoshi MSX Community (RIP 2007-2020)
* BlueMSX emulator >> [WEB](http://www.bluemsx.com/)
* OpenMSX emulator >> [WEB](http://openmsx.sourceforge.net/)
* Meisei emulator >> ?




## Requirements

* Small Device C Compiler (SDCC) v3.9 http://sdcc.sourceforge.net/
* Hex2bin v2.5 http://hex2bin.sourceforge.net/ 
* Vortex Tracker II > https://bulba.untergrund.net/vortex_e.htm (for create PT3 songs)



## Functions

* **PT3_Init**() Initialize the Player.
* **PT3_InitSong**(unsigned int songADDR, char loop)Init Song: (unsigned int) Song data address ;(char) Loop - 0=off ; 1=on
* **PT3_PlayAY**() Play Song. Execute on each interruption of VBLANK.
* **PT3_Decode**() Process the next step in the song sequence.
* **PT3_Mute**() Silence the PSG.
* **PT3_Loop**(char loop) Change state of loop.
* **PT3_Pause**() Pause song playback.
* **PT3_Resume**() Resume song playback.



## How to use

Follow the next steps:

1) Initialize the player by executing **PT3_Init()** and assign the frequency table (__NoteTable = (unsigned int) NT;__). 
2) Initialize the song to play with **PT3_InitSong**.
3) At each VBLANK interrupt, execute **PT3_PlayAY()**. This function dumps the AY record values and makes it sound.
4) Execute **PT3_Decode()** in your code in each frame, to process the song data.
5) You can stop song playback by executing **PT3_Pause()** and resume with **PT3_Resume()**.
6) To play another song, repeat these steps starting with number 2.


### Example code

```
#include "../include/PT3player.h"
#include "../include/PT3player_NoteTable2.h"
#include "../include/SongData.PT3.h"


#define  HALT __asm halt __endasm   //Z80 instruction: wait for the next interrupt

void main(void)
{

    NoteTable = (unsigned int) NT;
    PT3_Init();
    
    PT3_InitSong((unsigned int) SONG00, Loop_OFF);
    
    while(1)
    {
      HALT;

      PT3_PlayAY();      
      PT3_Decode();
      
      //your code here --->
            
    }
    
}
```
