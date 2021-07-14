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
- 1.2 (?) Release Version
- 1.1.9 (07/07/2021) Delete PlayAY() and AYREGS. Now need the AY38910BF library that manages hardware access.
- 1.1.8 (16/04/2021) add Player_IsEnd() function
- 1.1.7 (24/03/2021)
- 1.1.6 (15/02/2021) same function names in music libraries 
- 1.1.5 (22/01/2021) Adjusted to work without the 100 Byte header
- 1.1.4 (08/01/2021) PT3_Init and Bug #11 in loop
- 1.1.3 (05/01/2021) PT3state, PT3_Loop, PT3_Pause and PT3_Resume
- 1.1.2 (04/01/2021) assignment of frequency table memory address to NoteTable
- 1.1 (28/05/2019) Adaptation to SDCC of asMSX version by SapphiRe.
- 1.0 (21/10/2016) Adaptation to SDCC of the ROM version by Kun.


## Description

Adaptation of the latest version of Vortex Tracker II PT3 Player for MSX (SapphiRe) for use in software development in C (SDCC).

There are modifications on the original code, to add some extra functionality in the player control. 
Also part of the nomenclature has been changed to unify and simplify the use of different sound libraries.

To function it requires the AY38910BF library. The function that dumps the registers buffer to the sound processor (PlayAY), 
has been moved to this library because it is applicable to other music libraries (such as ayFXplayer).
You also get the possibility of selecting to which sound processor the data processed by the players should be directed (internal or external AY).

The AY38910BF together with the PT3player are designed to work together with the ayFXplayer so you will have a system to provide music and effects in the development of video games.

![AY Sound System](https://raw.githubusercontent.com/mvac7/SDCC_AY38910BF_Lib/master/doc/AYlibs.png)

Allows access to player variables.

Allows you to use any of the four note/frequency tables available in Vortex Tracker. 
They are available in header format to be included in your program.  

In the source code (\examples), you can find applications for testing and learning purposes.

![TEST PT3player](https://raw.githubusercontent.com/mvac7/SDCC_PT3player/master/examples/test_ROM/GFX/TESTPT3.png)

This library is part of the [MSX fR3eL Project](https://github.com/mvac7/SDCC_MSX_fR3eL).

Enjoy it!



## License

Vortex Tracker II v1.0 PT3 player (c) 2004 S.V.Bulba [(Vortex Project)](https://bulba.untergrund.net/vortex_e.htm)



## Requirements

* Small Device C Compiler (SDCC) v3.9 http://sdcc.sourceforge.net/
* Hex2bin v2.5 http://hex2bin.sourceforge.net/ 
* Vortex Tracker II > https://bulba.untergrund.net/vortex_e.htm (for create PT3 songs)
* A software to dump a PT3 binary file in C format. [PT3toCdata mSXdevtools](https://github.com/mvac7/PT3toCdata) or Aoineko's [CMSXbin](https://github.com/aoineko-fr/CMSXbin)
* [PSG AY-3-8910 BF MSX SDCC Library](https://github.com/mvac7/SDCC_AY38910BF_Lib)



## Functions

* **Player_Init()** Initialize the Player.
* **Player_InitSong(unsigned int songADDR, char loop)** Initialize song `(songADDR -> Song data address ; loop -> 0=off ; 1=on)`
* **Player_Decode()** Process the next step in the song sequence.
* **Player_Loop(char loop)** Change state of loop `(0=OFF ; 1=ON)` (AY38910BF library contains the SWITCHER Type with ON/OFF values)
* **Player_Pause()** Pause song playback.
* **Player_Resume()** Resume song playback.
* **char Player_IsEnd()** Indicates whether the song has finished playing. `(0 = No, 1 = Yes)`. It's only useful if you're playing a song with the loop turned off.



## Definitions

### AY Registers

Label | Value | Description
-- | -- | -- 
AY_ToneA     |  0 | Channel A Tone Period (12 bits)
AY_ToneB     |  2 | Channel B Tone Period (12 bits)
AY_ToneC     |  4 | Channel C Tone Period (12 bits)
AY_Noise     |  6 | Noise Period (5 bits)
AY_Mixer     |  7 | Mixer
AY_AmpA      |  8 | Channel Volume A (4 bits + B5 active Envelope)
AY_AmpB      |  9 | Channel Volume B (4 bits + B5 active Envelope)
AY_AmpC      | 10 | Channel Volume C (4 bits + B5 active Envelope)
AY_EnvPeriod | 11 | Envelope Period (16 bits)
AY_EnvShape  | 13 | Envelope Shape


---

## How to use

Follow the next steps:

1) Create a song in PT3 format with the Vortex Tracker.
2) Dump the file into C code, in a constant array of char and save it as a header (.h).
3) Includes the headers from the AY38910BF and PT3player libraries in the main source.
4) Includes the header with the song data. Example: `#include "../include/maki_CompoAY19v2_PT3.h"`
5) Includes the header of the note table used in the song. Example: `#include "../include/PT3player_NoteTable2.h"`
6) Initialize the player by executing `Player_Init()` and assign the frequency table `NoteTable = (unsigned int) NT;` 
7) Initialize the song to play with `Player_InitSong(songADDR,ON)`
8) At each VBLANK interrupt, execute `PlayAY()`. This function dumps the AY record values and makes it sound.
9) Execute `Player_Decode()` in your code in each frame, to process the song data.
10) You can control song playback using the **Player_Pause**, **Player_Resume**, **Player_Loop**, and **Player_IsEnd** functions.
11) To play another song, repeat these steps starting with number 7.

The player is based on a decoder that collects the data of a song step (frame) and translates it into data from the AY-3-8910 that writes in the `AYREGS` buffer. 
These data should be transferred to PSG, at the beginning of the VBLANK interruption through the `PlayAY()` function.

We will have control of the final sound if between the `Player_Decode()` and `PlayAY()` we modify the buffer. 
In this way we can mute channels, generate fades or launch sound effects on the song that is playing.


### Example code


In the following source you can see a simple example of its use. 
The usual thing is to program a function where the call to PlayAY is made and install it in the TIMI hook for the M1 interrupt of the Z80. 
You can see it in the example that accompanies the sources of this library.

```
#include "../include/AY38910BF.h"
#include "../include/PT3player.h"
#include "../include/PT3player_NoteTable2.h"
#include "../include/SongData.PT3.h"


#define  HALT __asm halt __endasm   //Z80 instruction: wait for the next interrupt

void main(void)
{

    NoteTable = (unsigned int) NT;
    Player_Init();
    
    Player_InitSong((unsigned int) SONG00, OFF);
    
    while(1)
    {
      HALT;

      PlayAY();   //<-- AY38910BF Library  
      Player_Decode();
      
      if (Player_IsEnd()==1) break;
      
      //your code here --->
            
    }
    
}
```



### How to convert PT3 binary file to C data

To include the songs in your program you need to convert the PT3s files to C data and save them as a header (.h).
To perform this task you will need an application that dumps the data from a binary file into C code.

You have the [PT3toCdata](https://github.com/mvac7/PT3toCdata) tool for Windows, designed for the format that this library needs.
You can also use Aoineko's [CMSXbin](https://github.com/aoineko-fr/CMSXbin) application.

The current version of the player is designed so that the data block does not contain the 100-byte header.
For it to work properly, you will have to remove it from the data obtained.

You can add the name of the song and the author if you need them to view them in an application like Jukebox/Music-Disk.

If you are going to use several songs in your program, you will need to differentiate the names of the constants, so that you do not get an error when compiling. 
You can use the "SONG00" tag and number each song (Look at the example).


```
// Vortex Tracker II 1.0 
// Song name: A funny day with my MSX
// Song author: Makinavaja
// Note Table: 2

const char SONG00_name[] = "A funny day with my MSX";
const char SONG00_author[] = "Makinavaja";

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


---

## Acknowledgments
  
I want to give a special thanks to all those who freely share their knowledge with the MSX developer community.

* S.V.Bulba > [(WEB)](http://bulba.at.kz) 
* Alfonso D. C. aka Dioniso
* MSXKun/Paxanga soft > [(WEB)](http://paxangasoft.retroinvaders.com/)
* Sapphire/Z80ST > [(WEB)](http://z80st.auic.es/)
* Makinavaja for his music. [(SoundCloud)](https://soundcloud.com/makimsx)
* Aoineko > [(GitHub)](https://github.com/aoineko-fr)
* Avelino Herrera > [(WEB)](http://msx.avelinoherrera.com)
* Nerlaska > [(Blog)](http://albertodehoyonebot.blogspot.com.es)
* Marq/Lieves!Tuore > [(Marq)](http://www.kameli.net/marq/) [(Lieves!Tuore)](http://www.kameli.net/lt/)
* Fubukimaru [(gitHub)](https://github.com/Fubukimaru)
* Andrear > [(Blog)](http://andrear.altervista.org/home/msxsoftware.php)
* Fernando García > [(Curso)](http://www.z80st.es/cursos/bitvision-assembler)
* Ramones > [(MSXblog)](https://www.msxblog.es/tutoriales-de-programacion-en-ensamblador-ramones/) - [(MSXbanzai)](http://msxbanzai.tni.nl/dev/faq.html)
* Eric Boez > [(gitHub)](https://github.com/ericb59)
* MSX Assembly Page > [(WEB)](http://map.grauw.nl/resources/msxbios.php)
* Portar MSX Tech Doc > [(WEB)](https://problemkaputt.de/portar.htm)
* MSX Resource Center > [Wiki](https://www.msx.org/wiki/)
* Karoshi MSX Community > [(WEB)](http://karoshi.auic.es/)
* BlueMSX emulator >> [(WEB)](http://www.bluemsx.com/)
* OpenMSX emulator >> [(WEB)](http://openmsx.sourceforge.net/)
* Meisei emulator >> ?
