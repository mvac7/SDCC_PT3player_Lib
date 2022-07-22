# How to use the PT3 Player MSX SDCC Library (fR3eL Project)



## Sorry!: This text is pending correction of the English translation. <<<<<<<<<



---

## Index
- [1 Description](#1-Description)
- [2 Requirements](#2-Requirements)
- [3 AY Sound System](#3-AY-Sound-System)
- [4 Definitions](#4-Definitions)
   - [4.1 SWITCHER (type)](#41-SWITCHER-(type))
   - [4.2 PT3 State Bits](#42-PT3-State-Bits)
- [5 Variables](#5-Variables)
   - [5.1 PT3_state](#51-PT3_state)
   - [5.2 Other PT3player variables](#52-Other-PT3player-variables)
- [6 Functions](#6-Functions)
   - [6.1 Player_Init](#61-Player_Init)
   - [6.2 Player_InitSong](#62-Player_InitSong)
   - [6.3 Player_Decode](#63-Player_Decode)
   - [6.4 Player_Loop](#64-Player_Loop)
   - [6.5 Player_Pause](#65-Player_Pause)
   - [6.6 Player_Resume](#66-Player_Resume)
   - [6.7 Player_IsEnd](#67-Player_IsEnd)
- [7 How to use](#7-How-to-use)
- [8 How to convert PT3 binary file to C data](#8-How-to-convert-PT3-binary-file-to-C-data)
- [9 Bonus Source Code](#9-Bonus-Source-Code)
   - [9.1 Fade Out](#9-Fade-Out)
   
<br/>

---


## 1 Description

Adaptation of the latest version of Vortex Tracker II PT3 Player for MSX `by SapphiRe` for use in software development in C (SDCC).

There are modifications on the original code, to add some extra functionality in the player control. 
Also part of the nomenclature has been changed to unify and simplify the use of different sound libraries.

To function it requires the AY38910BF library. The function that dumps the registers buffer to the sound processor `PlayAY`, 
has been moved to this library because it is applicable to other music libraries (such as ayFXplayer).
You also get the possibility of selecting to which sound processor the data processed by the players should be directed (internal or external AY).

The AY38910BF together with the PT3player are designed to work together with the ayFXplayer so you will have a system to provide music and effects in the development of video games.

Allows access to player variables.

Allows you to use any of the four note/frequency tables available in Vortex Tracker. 
They are available in header format to be included in your program.

In the source code `\examples`, you can find applications for testing and learning purposes.

This library is part of the [MSX fR3eL Project](https://github.com/mvac7/SDCC_MSX_fR3eL).

Enjoy it!


<br/>

---

## 2 Requirements

### For compile.
- [Small Device C Compiler (SDCC) v4.1](http://sdcc.sourceforge.net/)
- [Hex2bin v2.5](http://hex2bin.sourceforge.net/)
- [PSG AY-3-8910 BF MSX SDCC Library](https://github.com/mvac7/SDCC_AY38910BF_Lib)

### For create PT3 songs.
- [Vortex Tracker II](https://bulba.untergrund.net/vortex_e.htm) by S.V. Bulba 
- [Vortex Tracker 2.5](https://github.com/z00m128/vortextracker25) by Ivan Pirog

### For generate C datas 
- [PT3toCdata mSXdevtools](https://github.com/mvac7/PT3toCdata) 
- Aoineko's [CMSXbin](https://github.com/aoineko-fr/CMSXbin)


<br/>

---

## 3 AY Sound System

The [`AY38910BF`](https://github.com/mvac7/SDCC_AY38910BF_Lib), [`PT3player`](https://github.com/mvac7/SDCC_AY38910BF_Lib) and [`ayFXplayer`](https://github.com/mvac7/SDCC_ayFXplayer) libraries are designed to work together, so you will have a system to provide music and effects in game development.

![AY Sound System](https://raw.githubusercontent.com/mvac7/SDCC_AY38910BF_Lib/master/docs/AYlibs.png)

<br/>

---


## 4 Definitions


### 4.1 SWITCHER (type)

Used to control the state of the loop.

Label | Value
:---  | ---:  
OFF   | 0 
ON    | 1 


### 4.2 PT3 State Bits

They contain the position of the control bits found in the state variable [`PT3_state`](#41-PT3_state).

Label    | Description
:---     | :--- 
PT3_PLAY | Playback (0=Off/1=On)
PT3_LOOP | Loop (0=Off/1=On)
PT3_END  | Song ended? (0=No/1=Yes)

#### Example of use:

```c
   if (PT3_state & PT3_PLAY) ShowVumeter();
   if (!(PT3_state & PT3_END)) PlayAY();
```




## 5 Variables

### 5.1 PT3_state

Type: `char`

It is used as switches to control different aspects of the player.

Name | Description
:--- | :---  
Bit0 | - 
Bit1 | Playback (0=Off/1=On)
Bit2 | -
Bit3 | - 
Bit4 | Loop (0=Off/1=On)
Bit5 | -
Bit6 | -
Bit7 | song ended? (0=No/1=Yes)



### 5.2 Other PT3player variables

Variables that the player uses to control the decoding of the sequence data.

Information can be extracted from some variables, which can be useful for some cases (jukebox?).

Name | Type | Description
:--- | :--- | :---  
PT3_MODADDR  | unsigned int | ? 
PT3_CrPsPtr  | unsigned int | ?
PT3_SAMPTRS  | unsigned int | ? 
PT3_OrnPtrs  | unsigned int | ?
PT3_PDSP     | unsigned int | ?
PT3_CSP      | unsigned int | ?
PT3_PSP      | unsigned int | ?
PT3_PrNote   | char | ?
PT3_PrSlide  | unsigned int | ?
PT3_AdInPtA  | unsigned int | ?
PT3_AdInPtB  | unsigned int | ?
PT3_AdInPtC  | unsigned int | ?
PT3_LPosPtr  | unsigned int | ?
PT3_PatsPtr  | unsigned int | ?
PT3_Delay    | char | ?
PT3_AddToEn  | char | ?
PT3_Env_Del  | char | ?
PT3_ESldAdd  | unsigned int | ?



## 6 Functions

### 6.1 Player_Init

<table>
<tr><th colspan=3 align="left">Player_Init</th></tr>
<tr><td colspan=3>Initialize the Player</td></tr>
<tr><th>Function</th><td colspan=2>Player_Init()</td></tr>
<tr><th>Input</th><td colspan=2> --- </td></tr>
<tr><th>Output</th><td colspan=2> --- </td></tr>
<tr><th>Examples:</th><td colspan=2><code>Player_Init();</code></td></tr>
</table>


### 6.2 Player_InitSong

<table>
<tr><th colspan=3 align="left">Player_InitSong</th></tr>
<tr><td colspan=3>Initialize song</td></tr>
<tr><th>Function</th><td colspan=2>Player_InitSong(unsigned int songADDR, unsigned int NoteTable, char loop)</td></tr>
<tr><th rowspan=3>Input</th><td>unsigned int</td><td>Song data address.<br/>If the PT3 binary contains the header it will require<br/>subtracting 100 from this value.</td></tr>
<tr><td>unsigned int</td><td>Note Table address</td></tr>
<tr><td>char</td><td>Loop (0=off ; 1=on)</td></tr>
<tr><th>Output</th><td colspan=2> --- </td></tr>
<tr><th>Examples:</th><td colspan=2>
<code>Player_InitSong((unsigned int) SONG00, (unsigned int) NT2, OFF);</code>
</td></tr>
</table>


### 6.3 Player_Decode

<table>
<tr><th colspan=3 align="left">Player_Decode</th></tr>
<tr><td colspan=3>Process the next step in the song sequence.</td></tr>
<tr><th>Function</th><td colspan=2>Player_Decode()</td></tr>
<tr><th>Input</th><td colspan=2> --- </td></tr>
<tr><th>Output</th><td colspan=2> --- </td></tr>
<tr><th>Examples:</th><td colspan=2><code>Player_Decode();</code></td></tr>
</table>


### 6.4 Player_Loop

<table>
<tr><th colspan=3 align="left">Player_Loop</th></tr>
<tr><td colspan=3>Change loop state</td></tr>
<tr><th>Function</th><td colspan=2>Player_Loop(char loop)</td></tr>
<tr><th>Input</th><td>char<br/>SWITCHER</td><td>Loop (0=OFF ; 1=ON)</td></tr>
<tr><th>Output</th><td colspan=2> --- </td></tr>
<tr><th>Examples:</th><td colspan=2>
<code>Player_Loop(ON);</code>
</td></tr>
</table>


### 6.5 Player_Pause

<table>
<tr><th colspan=3 align="left">Player_Pause</th></tr>
<tr><td colspan=3>Pause song playback</td></tr>
<tr><th>Function</th><td colspan=2>Player_Pause()</td></tr>
<tr><th>Input</th><td colspan=2> --- </td></tr>
<tr><th>Output</th><td colspan=2> --- </td></tr>
<tr><th>Examples:</th><td colspan=2><code>Player_Pause();</code></td></tr>
</table>


### 6.6 Player_Resume

<table>
<tr><th colspan=3 align="left">Player_Resume</th></tr>
<tr><td colspan=3>Resume song playback</td></tr>
<tr><th>Function</th><td colspan=2>Player_Resume()</td></tr>
<tr><th>Input</th><td colspan=2> --- </td></tr>
<tr><th>Output</th><td colspan=2> --- </td></tr>
<tr><th>Examples:</th><td colspan=2><code>Player_Resume();</code></td></tr>
</table>


### 6.7 Player_IsEnd

<table>
<tr><th colspan=3 align="left">Player_IsEnd</th></tr>
<tr><td colspan=3>Indicates whether the song has finished playing</td></tr>
<tr><th>Function</th><td colspan=2>char Player_IsEnd()</td></tr>
<tr><th>Input</th><td colspan=2> --- </td></tr>
<tr><th>Output</th>><td>char</td><td>(0=no ; 1=Yes)</td></tr>
<tr><th>Examples:</th><td colspan=2><code>if (Player_Resume()==1) break;</code></td></tr>
</table>


---

## 7 How to use

Follow the next steps:

1) Create a song in PT3 format with the Vortex Tracker.
2) Dump the file into C code, in a constant array of char and save it as a header (.h).
3) Includes the headers from the AY38910BF and PT3player libraries in the main source.
4) Includes the header with the song data. Example: `#include "../include/maki_CompoAY19v2_PT3.h"`
5) Includes the header of the note table used in the song. Example: `#include "../include/PT3player_NoteTable2.h"`
6) Initialize the player by executing `Player_Init()`
7) Initialize the song to play with `Player_InitSong(SongDataADDR, NoteTableADDR, LOOP)`
8) At each VBLANK interrupt, execute `PlayAY()`. This function dumps the AY registers values and makes it sound.
9) Execute `Player_Decode()` in your code in each frame, to process the song data.
10) You can control song playback using the **Player_Pause**, **Player_Resume**, **Player_Loop**, and **Player_IsEnd** functions.
11) To play another song, repeat these steps starting with number 7.

The player is based on a decoder that collects the data of a song step (frame) and translates it into data from the AY-3-8910 that writes in the `AYREGS` buffer. 
These data should be transferred to PSG, at the beginning of the VBLANK interruption through the `PlayAY()` function (from AY38910BF_Lib).

We will have control of the final sound if between the `Player_Decode()` and `PlayAY()` we modify the buffer. 
In this way we can mute channels, generate fades or launch sound effects on the song that is playing.


### Example code

In the following source you can see a simple example of its use. 
The usual thing is to program a function where the call to PlayAY is made and install it in the TIMI hook for the M1 interrupt of the Z80. 
You can see it in the example that accompanies the sources of this library.

```c
#include "../include/AY38910BF.h"
#include "../include/PT3player.h"
#include "../include/PT3player_NoteTable2.h"
#include "../include/SongData.PT3.h"


#define  HALT __asm halt __endasm   //Z80 instruction: wait for the next interrupt

void main(void)
{

    Player_Init();
    
    Player_InitSong((unsigned int) SONG00, (unsigned int) NT2, OFF);
    AY_TYPE=AY_INTERNAL;
    
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



## 8 How to convert PT3 binary file to C data

To include the songs in your program you need to convert the PT3s files to C data and save them as a header (.h).
To perform this task you will need an application that dumps the data from a binary file into C code.

You have the [`PT3toCdata`](https://github.com/mvac7/PT3toCdata) tool for Windows, designed for the format that this library needs.
You can also use Aoineko's [`CMSXbin`](https://github.com/aoineko-fr/CMSXbin) application.

The current version of the player is designed so that the data block does not contain the 100-byte header.
For it to work properly, you will have to remove it from the data obtained.

You can add the name of the song and the author if you need them to view them in an application like Jukebox/Music-Disk.

If you are going to use several songs in your program, you will need to differentiate the names of the constants, so that you do not get an error when compiling. 
You can use the "SONG00" tag and number each song (Look at the example).


```c
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

<br/>

---







## 9 Bonus Source Code

### 9.1 Fade Out

The amplitude fading effect can be useful for situations where we have to interrupt a song and we don't want it to be done abruptly.

The system used by some players (such as PT3player or WYZplayer), based on a buffer, allows us to modify the records before sending it to PSG. We will use this feature to make our fade-out effect.

```c
void FadeOUT()
{
    char time=0;
    char i;
    char fadeout_ampA;
    char fadeout_ampB;
    char fadeout_ampC;
    
    fadeout_ampA = AYREGS[AR_AmplA];
    if (fadeout_ampA>15) fadeout_ampA=15;
    
    fadeout_ampB = AYREGS[AR_AmplB];
    if (fadeout_ampB>15) fadeout_ampB=15;
    
    fadeout_ampC = AYREGS[AR_AmplC];
    if (fadeout_ampC>15) fadeout_ampC=15;    
    
    for(i=0;i<160;i++)
    {
        HALT;        
        time++;
        
        if(time>10)
        {
            time=0;
            if (fadeout_ampA>0) fadeout_ampA--;
            if (fadeout_ampB>0) fadeout_ampB--;
            if (fadeout_ampC>0) fadeout_ampC--;    
        }
    
        Player_Decode();
        AYREGS[AR_AmplA] = (AYREGS[AR_AmplA] * fadeout_ampA)/16;
        AYREGS[AR_AmplB] = (AYREGS[AR_AmplB] * fadeout_ampB)/16;
        AYREGS[AR_AmplC] = (AYREGS[AR_AmplC] * fadeout_ampC)/16;
    } 
    
}

```

<br/>

---

![Creative Commons License](https://i.creativecommons.org/l/by-nc/4.0/88x31.png) 
<br/>This document is licensed under a [Creative Commons Attribution-NonCommercial 4.0 International License](http://creativecommons.org/licenses/by-nc/4.0/).