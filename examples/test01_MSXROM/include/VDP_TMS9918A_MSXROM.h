/* =============================================================================
  VDP TMS9918A MSX ROM SDCC Library (fR3eL Project)
============================================================================= */
#ifndef  __VDP_TMS9918A_H__
#define  __VDP_TMS9918A_H__



//VDP Ports  
#define VDPVRAM   0x98  //VRAM Data (Read/Write)
#define VDPSTATUS 0x99  //VDP Status Registers



// screen modes
#define TXT40  0  // text mode       (sc0)
#define TXT32  1  // graphics 1 mode (sc1)
#define GRAPH1 2  // graphics 2 mode (sc2)
#define GRAPH2 3  // multicolor mode (sc3)


//screen modes 2
#define T1 0  //screen 0
#define G1 1  //screen 1
#define G2 2  //screen 2
#define MC 3  //screen 3



// TMS9918 palette color codes
#ifndef _COLORS
#define _COLORS
#define TRANSPARENT   0
#define BLACK         1
#define GREEN         2
#define LIGHT_GREEN   3
#define DARK_BLUE     4
#define LIGHT_BLUE    5
#define DARK_RED      6
#define CYAN          7
#define RED           8
#define LIGHT_RED     9
#define DARK_YELLOW  10
#define LIGHT_YELLOW 11
#define DARK_GREEN   12
#define MAGENTA      13
#define GRAY         14
#define GREY         14
#define WHITE        15
#endif


// VRAM address tables screen 0 TXT40
#define BASE0 0x0000 // base 0 name table
#define BASE2 0x0800 // base 2 character pattern table

// VRAM address tables screen 1 TXT32
#define BASE5 0x1800 // base 5 name table
#define BASE6 0x2000 // base 6 color table
#define BASE7 0x0000 // base 7 character pattern table
#define BASE8 0x1B00 // base 8 sprite attribute table
#define BASE9 0x3800 // base 9 sprite pattern table

// VRAM address tables screen 2 GRAPH1
#define BASE10 0x1800 // base 10 name table
#define BASE11 0x2000 // base 11 color table
#define BASE12 0x0000 // base 12 character pattern table
#define BASE13 0x1B00 // base 13 sprite attribute table
#define BASE14 0x3800 // base 14 sprite pattern table

// VRAM address tables screen 3 GRAPH2
#define BASE15 0x0800 // base 15 name table
#define BASE17 0x0000 // base 17 character pattern table
#define BASE18 0x1B00 // base 18 sprite attribute table
#define BASE19 0x3800 // base 19 sprite pattern table

//G2 BANKs addends
#define BANK0  0x0000
#define BANK1  0x0800
#define BANK2  0x1000




// boolean
#ifndef _BOOLEAN
#define _BOOLEAN
  typedef enum {false = 0, true = 1} boolean;
#endif




/* =============================================================================
 SCREEN
 Description: Sets the display mode of the screen. 
 Input      : [char] number of screen mode
 Output     : -
============================================================================= */
void SCREEN(char mode);



/* =============================================================================
 SetSpritesSize
 Description: Set size type for the sprites.
 Input:       [char] size: 0=8x8; 1=16x16
 Output:      -
============================================================================= */ 
void SetSpritesSize(char size);



/* =============================================================================
 SetSpritesZoom
 Description: Set zoom type for the sprites.
 Input:       [char] zoom: false/0 = x1; true/1 = x2
 Output:      -
============================================================================= */
void SetSpritesZoom(boolean zoom);



/* =============================================================================
 COLOR
 Description: Specifies the ink, foreground and background colors. 
 Input      : [char] ink color
              [char] background color
              [char] border color
 Output     : -     
============================================================================= */
void COLOR(char ink, char background, char border);



/* =============================================================================
 VPOKE
 Description: Writes a byte to the video RAM. 
 Input      : [unsigned int] VRAM address
              [char] value
 Output     : - 
============================================================================= */
void VPOKE(unsigned int vaddr, char value);



/* =============================================================================
 VPEEK
 Description: Reads data from the video RAM. 
 Input      : [unsigned int] VRAM address
 Output     : [char] value
============================================================================= */ 
char VPEEK(unsigned int vaddr);



/* =============================================================================
 FillVRAM                               
 Description: Fill a large area of the VRAM of the same byte.
 Input      : [unsigned int] address of VRAM
              [unsigned int] blocklength
              [char] Value to fill.
 Output     : - 
============================================================================= */
void FillVRAM(unsigned int vaddr, unsigned int length, char value);



/* =============================================================================
 CopyToVRAM
 Description: Block transfer from memory to VRAM 
 Input      : [unsigned int] address of RAM
              [unsigned int] address of VRAM
              [unsigned int] blocklength
 Output     : - 
============================================================================= */
void CopyToVRAM(unsigned int addr, unsigned int vaddr, unsigned int length);



/* =============================================================================
 CopyFromVRAM
 Description: Block transfer from VRAM to memory
 Input      : [unsigned int] address of VRAM                     
              [unsigned int] address of RAM
              [unsigned int] blocklength
 Output     : -             
============================================================================= */
void CopyFromVRAM(unsigned int vaddr, unsigned int addr, unsigned int length);



/* =============================================================================
 SetVDP
 Description: Writes a value in VDP registers
 Input      : [char] VDP register                     
              [char] value
 Output     : -             
============================================================================= */
void SetVDP(char reg, char value);



#endif