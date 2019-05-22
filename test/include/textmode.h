/* =============================================================================
  SDCC MSX TEXTMODE Functions Library (object type)
  Version: 1.2
  Author: mvac7/303bcn
  Architecture: MSX
  Format: C Object (SDCC .rel)
  Programming language: C
  WEB: 
  mail: mvac7303b@gmail.com

  Description:
    Open Source library of functions for creating aplications in text mode. 
    
  History of versions:
    v1.2 ( 3/4/2018) current version
    v1.1 (27/2/2017)
============================================================================= */
#ifndef __TEXTMODE_H__
#define __TEXTMODE_H__



// palette color codes
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


/* =============================================================================
 SCREEN0
 
 Description: 
           Switch to T1 or T2 mode (SCREEN 0), 40 or 80 columns x 24 lines.
           Notice: To set the T2 mode, you must first set 80 columns with the 
           WIDTH instruction.
 Input:    -
 Output:   -
============================================================================= */
void SCREEN0();



/* =============================================================================
 SCREEN1
 
 Description: 
           Switch to G1 mode (SCREEN 1), 32 columns x 24 lines.
 Input:    -
 Output:   -
============================================================================= */
void SCREEN1();



/* =============================================================================
 WIDTH
 
 Description: 
           Specifies the number of characters per line in text mode.
 Input:     1 to 40 in T1 40 columns mode
           41 to 80 in T2 80 columns mode (only MSX with V9938 and a BIOS that 
                                           supports this mode)
            1 to 32 in G1 mode
 Output:   - 
============================================================================= */
void WIDTH(char columns);



/* =============================================================================
 COLOR
 
 Description: 
           Specifies the colors of the foreground, background, and border area.
 Input:    (char) ink (0 to 15)
           (char) background (0 to 15)
           (char) border (0 to 15)
 Output:   -
============================================================================= */
void COLOR(char ink, char background, char border);



/* =============================================================================
 CLS
 
 Description: 
           Clear the contents of the screen.
           Fill screen map with 0x20 character.
 Input:    -        
 Output:   - 
============================================================================= */
void CLS();



/* =============================================================================
 LOCATE
 
 Description: 
           Moves the cursor to the specified location.
 Input:    (char) Position X of the cursor. (0 to 31 or 79)
           (char) Position Y of the cursor. (0 to 23)         
 Output:   -
============================================================================= */
void LOCATE(char x, char y);



/* =============================================================================
 PRINT
  
 Description: 
           Displays a text string on the screen.             
                        
 Input:    (char*) String    
 Output:   -
 Notes:
            Supports escape sequences:
             \a (0x07) - Beep
             \b (0x08) - Backspace. Cursor left, wraps around to previous line, 
                         stop at top left of screen.
             \t (0x09) - Horizontal Tab. Tab, overwrites with spaces up to next 
                         8th column, wraps around to start of next line, scrolls
                         at bottom right of screen.
             \n (0x0A) - Newline > Line Feed and Carriage Return (CRLF) 
                         Note: CR added in this Lib.
             \v (0x0B) - Cursor home. Place the cursor at the top of the screen.
             \f (0x0C) - Formfeed. Clear screen and place the cursor at the top. 
             \r (0x0D) - CR (Carriage Return)
            
             \" (0x22) - Double quotation mark
             \' (0x27) - Single quotation mark
             \? (0x3F) - Question mark
             \\ (0x5C) - Backslash
             \xhh      - Print in the output the character/code given in the 
                         hexadecimal value (hh).
============================================================================= */
void PRINT(char* text);



//void printf(char* text);



/* =============================================================================
 PrintNumber

 Description: 
           Prints an unsigned integer on the screen.  
 Input:    (unsigned int) numeric value          
 Output:   -
============================================================================= */
void PrintNumber(unsigned int value);




/* =============================================================================
 PrintFNumber

 Description: 
           Prints an unsigned integer on the screen with formatting parameters.
 Input:    (unsigned int) numeric value
           (char) empty Char: (32=' ', 48='0', etc.)
           (char) length: 1 to 5          
 Output:   -  
============================================================================= */
void PrintFNumber(unsigned int value, char emptyChar, char length);



/* =============================================================================
 num2Dec16
 
 Description: 
           16-bit Integer to ASCII (decimal)
           Based on the code by baze.
 Input:    (unsigned int) a number
           (char*) Address where the output string is provided.
           (char) empty Char: 32=space, 48=zero
============================================================================= */
void num2Dec16(unsigned int aNumber, char *address, char emptyChar);



/* =============================================================================
 bchput
 
 Description: 
         Displays one character (MSX BIOS CHPUT).
 Input:   (char) text char
 Output:  -
============================================================================= */
void bchput(char value);



/* =============================================================================
   Current row-position of the cursor
============================================================================= */
//char GetRow()



/* =============================================================================
Current column-position of the cursor
============================================================================= */
//char GetColumn()



/* =============================================================================
   Displays the function keys
============================================================================= */
//void KEYON()



/* =============================================================================
   Erase functionkey display
============================================================================= */
//void KEYOFF()



#endif