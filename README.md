# PT3 Player MSX SDCC Library (fR3eL Project)

```
Architecture: MSX
Format: C Object (SDCC .rel)
Programming language: C and Z80 assembler
```

### Authors: 
- Vortex Tracker II v1.0 PT3 player for ZX Spectrum by S.V.Bulba <vorobey@mail.khstu.ru> http://bulba.at.kz
- Adapted to MSX by Alfonso D. C. aka Dioniso <dioniso072@yahoo.es>
- Arrangements for MSX ROM: MSXKun/Paxanga soft > http://paxangasoft.retroinvaders.com/
- asMSX version: SapphiRe > http://www.z80st.es/
- Adapted to SDCC: mvac7/303bcn > <mvac7303b@gmail.com>

In examples/test_ROM software, PT3 songs:
- "A funny day with my MSX" by Makinavaja `for AAMSX 50th Users Meeting invitro`
- "Mathematical brain" by Makinavaja [`for XI MICROCOMPO AY`](http://culturachip.org/compos/?r=31) 


#### PT3 player (Fixed Table) for asMSX cross-assembler (by SapphiRe): [`z80st-Blog > Nueva version del replayer de PT3`](http://www.z80st.es/blog/2008/11/19a-nueva-version-del-replayer-de-pt3)        


## Sorry!: This text is pending correction of the English translation. <<<<<<<<<


## History of versions:
- 1.2 (?) Release Version
- 1.1.10 (05/10/2021) Add Note Table address in Player_InitSong and bug fixed in Player_Resume
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

Adaptation of the latest version of Vortex Tracker II PT3 Player for MSX `by SapphiRe` for use in software development in C (SDCC).

There are modifications on the original code, to add some extra functionality in the player control. 
Also part of the nomenclature has been changed to unify and simplify the use of different sound libraries.

To function it requires the AY38910BF library. The function that dumps the registers buffer to the sound processor `PlayAY`, 
has been moved to this library because it is applicable to other music libraries (such as ayFXplayer).
You also get the possibility of selecting to which sound processor the data processed by the players should be directed (internal or external AY).

Allows access to player variables.

Allows you to use any of the four note/frequency tables available in Vortex Tracker. 
They are available in header format to be included in your program.

This project is an Open Source library. 
You can add part or all of this code in your application development or include it in other libraries/engines.

You can access the documentation here with [`How to use the library`](docs/HOWTO.md).  

In the source code [`examples/`](examples/), you can find applications for testing and learning purposes.

![TEST PT3player](https://github.com/mvac7/SDCC_PT3player/raw/master/examples/test01_MSXROM/GFX/TESTPT3.png)

[`TEST PT3player in WebMSX`](https://webmsx.org/?ROM=https://github.com/mvac7/SDCC_PT3player/raw/master/examples/test01_MSXROM/bin/TESTPT3.ROM)

This library is part of the [MSX fR3eL Project](https://github.com/mvac7/SDCC_MSX_fR3eL).

Enjoy it!

<br/>

---

## License

Vortex Tracker II v1.0 PT3 player (c) 2004 S.V.Bulba [`Vortex Project`](https://bulba.untergrund.net/vortex_e.htm)

<br/>

---

## Requirements

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

## AY Sound System

The [`AY38910BF`](https://github.com/mvac7/SDCC_AY38910BF_Lib), [`PT3player`](https://github.com/mvac7/SDCC_AY38910BF_Lib) and [`ayFXplayer`](https://github.com/mvac7/SDCC_ayFXplayer) libraries are designed to work together, so you will have a system to provide music and effects in game development.

![AY Sound System](https://raw.githubusercontent.com/mvac7/SDCC_AY38910BF_Lib/master/docs/AYlibs.png)


