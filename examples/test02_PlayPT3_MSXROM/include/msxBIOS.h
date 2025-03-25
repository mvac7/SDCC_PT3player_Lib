/* -----------------------------------------------------------------------------
 MSX BIOS definitions v1.2 (06/12/2023)
  ___ ___ ___  ___     _      __    
 | _ )_ _/ _ \/ __| __| |___ / _|___
 | _ \| | (_) \__ \/ _` / -_)  _(_-<
 |___/___\___/|___/\__,_\___|_| /__/

 Definitions & descriptions by MSX Assembly Page
 Only includes descriptions. For details see the WEB document in:
 http://map.grauw.nl/resources/msxbios.php
----------------------------------------------------------------------------- */

#ifndef  __MSXBIOS_H__
#define  __MSXBIOS_H__

// MSX 1 BIOS ------------------------------------------------------------------
//More info (MSX Assembly Page): http://map.grauw.nl/resources/msxbios.php  

// RST-and other routines
#define BIOS_CHKRAM  0x0000 // (RST  0 ) Check RAM and sets slot for command area.
#define BIOS_SYNCHR  0x0008 // (RST  8h) Checks if then current character pointed by HL is one desired.
#define BIOS_RDSLT   0x000C // Reads the value of an address in another slot
#define BIOS_CHRGTR  0x0010 // (RST 10h) Gets the next character (or token) of the Basic-text
#define BIOS_WRSLT   0x0014 // Writes a value to an address in another slot.
#define BIOS_OUTDO   0x0018 // (RST 18h) Output to current outputchannel (printer, diskfile, etc.)
#define BIOS_CALSLT  0x001C // Executes inter-slot call.
#define BIOS_DCOMPR  0x0020 // (RST 20h) Compares HL with DE
#define BIOS_ENASLT  0x0024 // Switches indicated slot at indicated page on perpetual
#define BIOS_GETYPR  0x0028 // (RST 28h) Returns Type of DAC
#define BIOS_CALLF   0x0030 // (RST 30h) Executes an interslot call
#define BIOS_KEYINT  0x0038 // (RST 38h) Executes the timer interrupt process routine

// Initialization-routines
#define BIOS_INITIO  0x003B // Initialises the device
#define BIOS_INIFNK  0x003E // Initialises the contents of the function keys

// VDP routines
#define BIOS_DISSCR  0x0041 // inhibits the screen display
#define BIOS_ENASCR  0x0044 // displays the screen
#define BIOS_WRTVDP  0x0047 // write data in the VDP-register
#define BIOS_RDVRM   0x004A // Reads the content of VRAM
#define BIOS_WRTVRM  0x004D // Writes data in VRAM
#define BIOS_SETRD   0x0050 // Enable VDP to read
#define BIOS_SETWRT  0x0053 // Enable VDP to write
#define BIOS_FILVRM  0x0056 // fill VRAM with value
#define BIOS_LDIRMV  0x0059 // Block transfer to memory from VRAM
#define BIOS_LDIRVM  0x005C // Block transfer to VRAM from memory
#define BIOS_CHGMOD  0x005F // Switches to given screenmode
#define BIOS_CHGCLR  0x0062 // Changes the screencolors
#define BIOS_CLRSPR  0x0069 // Initialises all sprites
#define BIOS_INITXT  0x006C // Switches to SCREEN 0 (text screen with 40 * 24 characters)
#define BIOS_INIT32  0x006F // Switches to SCREEN 1 (text screen with 32*24 characters)
#define BIOS_INIGRP  0x0072 // Switches to SCREEN 2 (high resolution screen with 256*192 pixels)
#define BIOS_INIMLT  0x0075 // Switches to SCREEN 3 (multi-color screen 64*48 pixels)
#define BIOS_SETTXT  0x0078 // Switches to VDP in SCREEN 0 mode
#define BIOS_SETT32  0x007B // Switches VDP in SCREEN mode 1
#define BIOS_SETGRP  0x007E // Switches VDP to SCREEN 2 mode
#define BIOS_SETMLT  0x0081 // Switches VDP to SCREEN 3 mode
#define BIOS_CALPAT  0x0084 // Returns the address of the sprite pattern table
#define BIOS_CALATR  0x0087 // Returns the address of the sprite attribute table
#define BIOS_GSPSIZ  0x008A // Returns current sprite size
#define BIOS_GRPPRT  0x008D // Displays a character on the graphic screen
        
// PSG routines
#define BIOS_GICINI  0x0090 // Initialises PSG and sets initial value for the PLAY statement
#define BIOS_WRTPSG  0x0093 // Writes data to PSG-register
#define BIOS_RDPSG   0x0096 // Reads value from PSG-register
#define BIOS_STRTMS  0x0099 /* Tests whether the PLAY statement is being executed as 
							   a background task. If not, begins to execute the PLAY statement */

// Console routines
#define BIOS_CHSNS   0x009C // Tests the status of the keyboard buffer
#define BIOS_CHGET   0x009F // One character input (waiting)
#define BIOS_CHPUT   0x00A2 // Displays one character
#define BIOS_LPTOUT  0x00A5 // Sends one character to printer
#define BIOS_LPTSTT  0x00A8 // Tests printer status
#define BIOS_CNVCHR  0x00AB // tests for the graphic header and transforms the code
#define BIOS_PINLIN  0x00AE /* Stores in the specified buffer the character codes  
							   input until the return key or STOP key is pressed     */
#define BIOS_INLIN   0x00B1 // Same as PINLIN except that AUGFLG (#F6AA) is set
#define BIOS_QINLIN  0x00B4 // Prints a questionmark andone space
#define BIOS_BREAKX  0x00B7 // Tests status of CTRL-STOP 
#define BIOS_ISCNTC  0x00BA // Tests status of SHIFT-STOP 
#define BIOS_CKCNTC  0x00BD // Same as ISCNTC. used in Basic
#define BIOS_BEEP    0x00C0 // generates beep
#define BIOS_BCLS    0x00C3 // Clears the screen
#define BIOS_POSIT   0x00C6 // Places the cursor at the specified location
#define BIOS_FNKSB   0x00C9 /* Tests whether the function key display is active (FNKFLG)
							   If so, displays them, otherwise erase them */
#define BIOS_ERAFNK  0x00CC // Erase functionkey display
#define BIOS_DSPFNK  0x00CF // Displays the function keys
#define BIOS_TOTEXT  0x00D2 // Forces the screen to be in the text mode

// Controller routines                      
#define BIOS_GTSTCK  0x00D5 // Returns the joystick status
#define BIOS_GTTRIG  0x00D8 // Returns current trigger status
#define BIOS_GTPAD   0x00DB // Returns current touch pad status
#define BIOS_GTPDL   0x00DE // Returns currenct value of paddle

// Tape device routines                      
#define BIOS_TAPION  0x00E1 // Reads the header block after turning the cassette motor on
#define BIOS_TAPIN   0x00E4 // Read data from the tape
#define BIOS_TAPIOF  0x00E7 // Stops reading from the tape
#define BIOS_TAPOON  0x00EA // Turns on the cassette motor and writes the header
#define BIOS_TAPOUT  0x00ED // Writes data on the tape
#define BIOS_TAPOOF  0x00F0 // Stops writing on the tape
#define BIOS_STMOTR  0x00F3 // Sets the cassette motor action

// Queue routines                      
#define BIOS_LFTQ    0x00F6 // Gives number of bytes in queue
#define BIOS_PUTQ    0x00F9 // Put byte in queue

// Graphic routines                    
#define BIOS_RIGHTC  0x00FC // Shifts screenpixel to the right
#define BIOS_LEFTC   0x00FF // Shifts screenpixel to the left
#define BIOS_UPC     0x0102 // Shifts screenpixel up
#define BIOS_TUPC    0x0105 // Tests whether UPC is possible, if possible, execute UPC
#define BIOS_DOWNC   0x0108 // Shifts screenpixel down
#define BIOS_TDOWNC  0x010B // Tests whether DOWNC is possible, if possible, execute DOWNC
#define BIOS_SCALXY  0x010E // Scales X and Y coordinates
#define BIOS_MAPXY   0x0111 // Places cursor at current cursor address
#define BIOS_FETCHC  0x0114 // Gets current cursor addresses mask pattern
#define BIOS_STOREC  0x0117 // Record current cursor addresses mask pattern
#define BIOS_SETATR  0x011A // Set attribute byte
#define BIOS_READC   0x011E // Reads attribute byte of current screenpixel
#define BIOS_SETC    0x0120 // Returns currenct screenpixel of specificed attribute byte
#define BIOS_NSETCX  0x0123 // Set horizontal screenpixels
#define BIOS_GTASPC  0x0126 // Gets screen relations
#define BIOS_PNTINI  0x0129 // Initalises the PAINT instruction
#define BIOS_SCANR   0x012C // Scans screenpixels to the right
#define BIOS_SCANL   0x012F // Scans screenpixels to the left
                     
// Misc routines
#define BIOS_CHGCAP  0x0132 // Alternates the CAP lamp status
#define BIOS_CHGSND  0x0135 // Alternates the 1-bit sound port status
#define BIOS_RSLREG  0x0138 // Reads the primary slot register
#define BIOS_WSLREG  0x013B // Writes value to the primary slot register
#define BIOS_RDVDP   0x013E // Reads VDP status register
#define BIOS_SNSMAT  0x0141 // Returns the value of the specified line from the keyboard matrix
#define BIOS_PHYDIO  0x0144 // Executes I/O for mass-storage media like diskettes
#define BIOS_FORMAT  0x0147 // Initialises mass-storage media like formatting of diskettes
#define BIOS_ISFLIO  0x014A // Tests if I/O to device is taking place
#define BIOS_OUTDLP  0x014E // Printer output
#define BIOS_GETVCP  0x0150 // Returns pointer to play queue
#define BIOS_GETVC2  0x0153 // Returns pointer to variable in queue number VOICEN (byte op #FB38)
#define BIOS_KILBUF  0x0156 // Clear keyboard buffer
#define BIOS_CALBAS  0x0159 // Executes inter-slot call to the routine in BASIC interpreter


// MSX 2 BIOS ------------------------------------------------------------------
//More info (MSX Assembly Page): http://map.grauw.nl/resources/msxbios.php#msx2bios
#define BIOS_SUBROM  0x015C // Calls a routine in SUB-ROM
#define BIOS_EXTROM  0x015F // Calls a routine in SUB-ROM. Most common way
#define BIOS_CHKSLZ  0x0162 // Search slots for SUB-ROM
#define BIOS_CHKNEW  0x0165 // Tests screen mode > C-flag set if screenmode = 5, 6, 7 or 8
#define BIOS_EOL     0x0168 // Deletes to the end of the line
#define BIOS_BIGFIL  0x016B // fill VRAM with value (total VRAM can be reached) HL address, BC length, A data
#define BIOS_NSETRD  0x016E // Enable VDP to read.(with full 16 bits VRAM-address)
#define BIOS_NSTWRT  0x0171 // Enable VDP to write.(with full 16 bits VRAM-address) 
#define BIOS_NRDVRM  0x0174 // Reads VRAM like in RDVRM.(with full 16 bits VRAM-address)
#define BIOS_NWRVRM  0x0177 // Writes to VRAM like in WRTVRM.(with full 16 bits VRAM-address)


// MSX 2+ BIOS -----------------------------------------------------------------
//More info (MSX Assembly Page): http://map.grauw.nl/resources/msxbios.php#msx2pbios
#define BIOS_RDRES   0x017A // Read value of I/O port #F4
#define BIOS_WRRES   0x017D // Write value to I/O port #F4


// MSX turboR BIOS -------------------------------------------------------------
//More info (MSX Assembly Page): http://map.grauw.nl/resources/msxbios.php#msx2pbios
#define BIOS_CHGCPU  0x0180 // Changes CPU mode
#define BIOS_GETCPU  0x0183 // Returns current CPU mode
#define BIOS_PCMPLY  0x0186 // Plays specified memory area through the PCM chip
#define BIOS_PCMREC  0x0189 // Records audio using the PCM chip into the specified memory area

        
#endif