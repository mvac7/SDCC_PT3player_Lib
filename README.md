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

In examples/test_ROM software, PT3 songs:
- "A funny day with my MSX" by Makinavaja (for AAMSX 50th Users Meeting invitro)
- "Mathematical brain" by Makinavaja (for XI MICROCOMPO AY) 
```

#### PT3 player (Fixed Table) for asMSX cross-assembler (by SapphiRe): [Nueva version del replayer de PT3](http://www.z80st.es/blog/2008/11/19a-nueva-version-del-replayer-de-pt3)        


## Sorry!: This text is pending correction of the English translation. <<<<<<<<


## History of versions:
- 1.2 (?)>Assignment of frequency table memory address to NoteTable, PT3state, Player_Loop, Player_Pause, Player_Resume and Player_IsEnd.
- 1.1 (28/05/2019) Adaptation to SDCC of asMSX version by SapphiRe.
- 1.0 (21/10/2016) Adaptation to SDCC of the ROM version by Kun.


## Introduction

Adaptation of the latest version of Vortex Tracker II PT3 Player for MSX (SapphiRe) for use in software development in C (SDCC).

Allows access to player variables.

Allows you to use any of the four note/frequency tables available in Vortex Tracker. 
They are available in header format to be included in your program.  

In the source code (\examples), you can find applications for testing and learning purposes.

![TEST PT3player](https://raw.githubusercontent.com/mvac7/SDCC_PT3player/master/examples/test_ROM/GFX/TESTPT3.png)

This library is part of the [MSX fR3eL Project](https://github.com/mvac7/SDCC_MSX_fR3eL).

Enjoy it!



## License

Vortex Tracker II v1.0 PT3 player (c) 2004 S.V.Bulba [(Vortex Project)](https://bulba.untergrund.net/vortex_e.htm)



## Acknowledgments
  
I want to give a special thanks to all those who freely share their knowledge with the MSX developer community.

* S.V.Bulba > [(WEB)](http://bulba.at.kz) 
* Alfonso D. C. aka Dioniso
* MSXKun/Paxanga soft > [(WEB)](http://paxangasoft.retroinvaders.com/)
* Sapphire/Z80ST > [(WEB)](http://z80st.auic.es/)
* Makinavaja for his music. [(SoundCloud)](https://soundcloud.com/makimsx)
* Aoineko [(GitHub)](https://github.com/aoineko-fr)
* Avelino Herrera > [(WEB)](http://msx.atlantes.org/index_es.html)
* Nerlaska > [(Blog)](http://albertodehoyonebot.blogspot.com.es)
* Marq/Lieves!Tuore > [(Marq)](http://www.kameli.net/marq/) [(Lieves!Tuore)](http://www.kameli.net/lt/)
* [Fubukimaru](https://github.com/Fubukimaru) > [(Blog)](http://www.gamerachan.org/fubu/)
* Andrear > [(Blog)](http://andrear.altervista.org/home/msxsoftware.php)
* Ramones > [(MSXblog)](https://www.msxblog.es/tutoriales-de-programacion-en-ensamblador-ramones/) - [(MSXbanzai)](http://msxbanzai.tni.nl/dev/faq.html)
* Eric Boez > [(gitHub)](https://github.com/ericb59)
* MSX Assembly Page > [(WEB)](http://map.grauw.nl/resources/msxbios.php)
* Portar MSX Tech Doc > [(WEB)](https://problemkaputt.de/portar.htm)
* MSX Resource Center > [(WEB)](http://www.msx.org/)
* Karoshi MSX Community (RIP 2007-2020)
* BlueMSX emulator >> [(WEB)](http://www.bluemsx.com/)
* OpenMSX emulator >> [(WEB)](http://openmsx.sourceforge.net/)
* Meisei emulator >> ?



## Requirements

* Small Device C Compiler (SDCC) v3.9 http://sdcc.sourceforge.net/
* Hex2bin v2.5 http://hex2bin.sourceforge.net/ 
* Vortex Tracker II > https://bulba.untergrund.net/vortex_e.htm (for create PT3 songs)
* A software to dump a PT3 binary file in C format. [PT3toCdata mSXdevtools](https://github.com/mvac7/PT3toCdata) or Aoineko's [CMSXbin](https://github.com/aoineko-fr/CMSXbin)



## Functions

* **Player_Init()** Initialize the Player.
* **Player_InitSong(unsigned int songADDR, char loop)** Initialize song `(songADDR -> Song data address ; loop -> 0=off ; 1=on)`
* **PlayAY()** Send data from AYREGS buffer to AY registers. (Execute on each interruption of VBLANK)
* **Player_Decode()** Process the next step in the song sequence.
* **Player_Loop(char loop)** Change state of loop `(0=off ; 1=on)`
* **Player_Pause()** Pause song playback.
* **Player_Resume()** Resume song playback.
* **char Player_IsEnd()** Indicates whether the song has finished playing. `(0 = No, 1 = Yes)`



## How to use

Follow the next steps:

1) Create a song in PT3 format with the Vortex Tracker.
2) Dump the file into C code, in a constant array of char and save it as a header (.h).
3) Include the header with the song data in your main source. 
4) Initialize the player by executing `Player_Init()` and assign the frequency table `NoteTable = (unsigned int) NT;` 
5) Initialize the song to play with `Player_InitSong(songADDR,loop)`
6) At each VBLANK interrupt, execute `PlayAY()`. This function dumps the AY record values and makes it sound.
7) Execute `Player_Decode()` in your code in each frame, to process the song data.
8) You can stop song playback by executing `Player_Pause()` and resume with `Player_Resume()`
9) To play another song, repeat these steps starting with number 5.


### Example code

```
#include "../include/PT3player.h"
#include "../include/PT3player_NoteTable2.h"
#include "../include/SongData.PT3.h"


#define  HALT __asm halt __endasm   //Z80 instruction: wait for the next interrupt

void main(void)
{

    NoteTable = (unsigned int) NT;
    Player_Init();
    
    Player_InitSong((unsigned int) SONG00, Loop_OFF);
    
    while(1)
    {
      HALT;

      PlayAY();      
      Player_Decode();
      
      if (Player_IsEnd()==1) break;
      
      //your code here --->
            
    }
    
}
```



### How to convert PT3 binary file to C data

To include the songs in your program you need to convert the PT3s files to C data and save them as a header (.h).

To do this, you have the [PT3toCdata](https://github.com/mvac7/PT3toCdata) tool for Windows, designed for the format that this library needs, but if you cannot run it on your system, you will need an application that dumps binaries into C code. 
You can use Aoineko's [CMSXbin](https://github.com/aoineko-fr/CMSXbin) application.

The current version of the player is designed so that the data block does not contain the 100-byte header.
For it to work properly, you will have to remove it from the data obtained.

You can add the name of the song and the author if you need them to view them in an application like Jukebox/Music-Disk.

If you are going to use several songs in your program, you will need to differentiate the names of the constants, so that you do not get an error when compiling. 
You can use the "SONG00" tag and number each song (Look at the example).


```
// Vortex Tracker II 1.0 

const char SONG00_name[] = "A funny day with my MSX";  //optional data
const char SONG00_author[] = "Makinavaja";             //optional data

// maki_ru50inv.pt3
// Length: 2885
// Not contain the 100 Byte header
const char SONG00[]={
0x03,0x25,0x00,0xEF,0x00,0x00,0x00,0xBD,0x0A,0xCB,0x0A,0xD1,0x0A,0xD7,0x0A,0x99,
0x0B,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0xA3,0x0B,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0xA6,0x0B,0x00,0x00,0x03,0x06,0x03,0x06,0x09,0x0C,0x09,0x12,0x36,0x39,0x3C,
0x3F,0x42,0x45,0x42,0x4B,0x0F,0x00,0x15,0x18,0x1B,0x1E,0x21,0x24,0x27,0x2A,0x2D,
...};
```
