# SDCC PT3 player Library for MSX

Version: 1.0

Date: 20/04/2019

Authors: 

- Adapted to MSX by Alfonso D. C. aka Dioniso <dioniso072@yahoo.es>
- From: Vortex Tracker II v1.0 PT3 player for ZX Spectrum
(c)2004 S.V.Bulba <vorobey@mail.khstu.ru> http://bulba.at.kz
- Arrangements for MSX ROM: MSXKun/Paxanga soft
- Adapted to SDCC: mvac7/303bcn
        
        

Architecture: MSX

Programming language: C & Assembler

WEB:
 
mail: mvac7303b@gmail.com


History of versions:




## 1. Introduction

This project is an Open Source library.


  
Includes applications for testing and learning purposes.



## 2. Acknowledgments
  
Thanks for Info & help, to:

* S.V.Bulba > <vorobey@mail.khstu.ru> http://bulba.at.kz
* Dioniso > <dioniso072@yahoo.es>
* Avelino Herrera > http://msx.atlantes.org/index_es.html
* Nerlaska > http://albertodehoyonebot.blogspot.com.es
* Fubu > http://www.gamerachan.org/fubu/
* Marq/Lieves!Tuore > http://www.kameli.net/lt/
* Sapphire/Z80ST > http://z80st.auic.es/
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



## 3. Requirements

For C:

 * Small Device C Compiler (SDCC) v3.6 http://sdcc.sourceforge.net/
     
 * Hex2bin v2.2 http://hex2bin.sourceforge.net/






## 4. Functions

* void PT3Init(unsigned int,char) Init Song: (unsigned int) Song data address ;(char) Loop - 0=off ; 1=on
* void PT3Play() Play song. Execute on each interruption of VBLANK.
* void PT3Stop() Stop Song.