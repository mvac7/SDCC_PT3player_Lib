# SDCC PT3 player Library for MSX

#### Version: 
1.1

#### Date: 
23/05/2019

#### Architecture: 
MSX

#### Programming language: 
C & Assembler

#### Authors: 

- Vortex Tracker II v1.0 PT3 player for ZX Spectrum by S.V.Bulba <vorobey@mail.khstu.ru> http://bulba.at.kz
- Adapted to MSX by Alfonso D. C. aka Dioniso <dioniso072@yahoo.es>
- Arrangements for MSX ROM: MSXKun/Paxanga soft > http://paxangasoft.retroinvaders.com/
- Improvements and latest changes: SapphiRe > http://www.z80st.es/
- Adapted to SDCC: mvac7/303bcn > <mvac7303b@gmail.com>

#### In Test/Example software:

- PT3 song:  
  "A funny day with my MSX" from 50RU MSX invitro (AAMSX)
  by Makinavaja 


#### asMSX player (by SapphiRe):        
http://www.z80st.es/blog/2008/11/19a-nueva-version-del-replayer-de-pt3        


### History of versions:

- 


## 1. Introduction



  
Includes applications for testing and learning purposes.


## 2. License

Vortex Tracker II v1.0 PT3 player (c) 2004 S.V.Bulba



## 3. Acknowledgments
  
Thanks for information, help or your contribution to the MSX community:

* S.V.Bulba > <vorobey@mail.khstu.ru> http://bulba.at.kz
* Dioniso > <dioniso072@yahoo.es>
* MSXKun/Paxanga soft > http://paxangasoft.retroinvaders.com/
* SapphiRe/Z80ST > http://www.z80st.es/
* Avelino Herrera > http://msx.atlantes.org/index_es.html
* Nerlaska > http://albertodehoyonebot.blogspot.com.es
* Fubu > http://www.gamerachan.org/fubu/
* Marq/Lieves!Tuore > http://www.kameli.net/lt/
* Pentacour > http://pentacour.com/
* JamQue/TPM > http://www.thepetsmode.com/
* Andrear > http://andrear.altervista.org/home/msxsoftware.php
* Konamiman > https://www.konamiman.com
* MSX Assembly Page > http://map.grauw.nl/resources/msxbios.php
* Portar MSX Tech Doc > http://nocash.emubase.de/portar.htm
* MSX Resource Center > http://www.msx.org/
* Karoshi MSX Community > http://karoshi.auic.es/
* BlueMSX >> http://www.bluemsx.com/
* OpenMSX >> http://openmsx.sourceforge.net/
* Meisei  >> ?



## 4. Requirements

For C:

 * Small Device C Compiler (SDCC) v3.6 > http://sdcc.sourceforge.net/
     
 * Hex2bin v2.2 > http://hex2bin.sourceforge.net/

 * Vortex Tracker II > https://bulba.untergrund.net/vortex_e.htm
   (for create a PT3 song)



## 5. Functions

* void PT3Init(unsigned int,char) Init Song: (unsigned int) Song data address ;(char) Loop - 0=off ; 1=on
* void PT3Play() Play song. Execute on each interruption of VBLANK.
* void PT3Stop() Stop Song.