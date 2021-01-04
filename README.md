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

#### asMSX PT3 player (by SapphiRe):        
http://www.z80st.es/blog/2008/11/19a-nueva-version-del-replayer-de-pt3        


## Sorry!: This text is pending correction of the English translation. <<<<<<<<


## History of versions:
- 1.2 (04/01/2021)>assignment of frequency table memory address to NoteTable (#2)
- 1.1 (28/05/2019) Adaptation to SDCC of asMSX version by SapphiRe.
- 1.0 (21/10/2016) Adaptation to SDCC of the ROM version by Kun.


## Introduction

Adaptation of the latest version of Vortex Tracker II PT3 Player for MSX (SapphiRe) for use in software development in C (SDCC).

Allows access to player variables.

As there are four tables of notes available in Vortex Tracker, this must be assigned externally, copying to the space reserved in the variable NoteTable.
The four tables are included in files <PT3player_NoteTableN.h>  

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

* void **PT3Init**(unsigned int, char) Init Song: (unsigned int) Song data address ;(char) Loop - 0=off ; 1=on
* void **PT3Decode**() Decode a frame from PT3 Song.
* void **PT3PlayAY**() Play Song. Execute on each interruption of VBLANK.
* void **PT3Mute**() Silence the PSG.



## How to use

Follow the next steps:

1) Assign the table of notes that corresponds to the one used in the song. 
2) Initialize the song to sound with **PT3Init**.
3) At each VBLANK interrupt, execute **PT3PlayAY**. This function dumps the AY record values and makes it sound.
4) Execute **PT3Decode** in your code in each frame, to process the song data.
5) As you want to stop the playback of the song, execute **PT3Mute** and stop calling **PT3Decode**.
6) To play another song, repeat these steps starting with number 2.