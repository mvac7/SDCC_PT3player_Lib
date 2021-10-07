// MSX-DOS 2 function calls
// (c) MSX Assembly Page 
// http://map.grauw.nl/resources/dos2_functioncalls.php

#ifndef  __MSXDOS_H__
#define  __MSXDOS_H__




//MSX-DOS1
#define _TERM0  0x00	//Program terminate
#define _CONIN  0x01	//Console input
#define _CONOUT 0x02	//Console output
#define _AUXIN  0x03	//Auxiliary input
#define _AUXOUT 0x04	//Auxiliary output
#define _LSTOUT 0x05	//Printer output
#define _DIRIO  0x06	//Direct console I/O
#define _DIRIN  0x07	//Direct console input
#define _INNOE  0x08	//Console input without echo
#define _STROUT 0x09	//String output
#define _BUFIN  0x0A	//Buffered line input
#define _CONST  0x0B	//Console status
#define _CPMVER 0x0C	//Return version number
#define _DSKRST 0x0D	//Disk reset
#define _SELDSK 0x0E	//Select disk
#define _FOPEN  0x0F	//Open file (FCB)
#define _FCLOSE 0x10	//Close file (FCB)
#define _SFIRST 0x11	//Search for first entry (FCB)
#define _SNEXT  0x12	//Search for next entry (FCB)
#define _FDEL   0x13	//Delete file (FCB)
#define _RDSEQ  0x14	//Sequential read (FCB)
#define _WRSEQ  0x15	//Sequential write (FCB)
#define _FMAKE  0x16	//Create file (FCB)
#define _FREN   0x17	//Rename file (FCB)
#define _LOGIN  0x18	//Get login vector
#define _CURDRV 0x19	//Get current drive
#define _SETDTA 0x1A	//Set disk transfer address
#define _ALLOC  0x1B	//Get allocation information
#define _RDRND  0x21	//Random read (FCB)
#define _WRRND  0x22	//Random write (FCB)
#define _FSIZE  0x23	//Get file size (FCB)
#define _SETRND 0x24	//Set random record (FCB)
#define _WRBLK  0x26	//Random block write (FCB)
#define _RDBLK  0x27	//Random block read (FCB)
#define _WRZER  0x28	//Random write with zero fill (FCB)
#define _GDATE  0x2A	//Get date
#define _SDATE  0x2B	//Set date
#define _GTIME  0x2C	//Get time
#define _STIME  0x2D	//Set time
#define _VERIFY 0x2E	//Set/reset verify flag
#define _RDABS  0x2F	//Absolute sector read
#define _WRABS  0x30	//Absolute sector write


//MSX-DOS2
#define _DPARM    0x31	//Get disk parameters
#define _FFIRST   0x40	//Find first entry
#define _FNEXT    0x41	//Find next entry
#define _FNEW     0x42	//Find new entry
#define _OPEN     0x43	//Open file handle
#define _CREATE   0x44	//Create file handle
#define _CLOSE    0x45	//Close file handle
#define _ENSURE   0x46	//Ensure file handle
#define _DUP      0x47	//Duplicate file handle
#define _READ     0x48	//Read from file handle
#define _WRITE    0x49	//Write to file handle
#define _SEEK     0x4A	//Move file handle pointer
#define _IOCTL    0x4B	//I/O control for devices
#define _HTEST    0x4C	//Test file handle
#define _DELETE   0x4D	//Delete file or subdirectory
#define _RENAME   0x4E	//Rename file or subdirectory
#define _MOVE     0x4F	//Move file or subdirectory
#define _ATTR     0x50	//Get/set file attributes
#define _FTIME    0x51	//Get/set file date and time
#define _HDELETE  0x52	//Delete file handle
#define _HRENAME  0x53	//Rename file handle
#define _HMOVE    0x54	//Move file handle
#define _HATTR    0x55	//Get/set file handle attributes
#define _HFTIME   0x56	//Get/set file handle date and time
#define _GETDTA   0x57	//Get disk transfer address
#define _GETVFY   0x58	//Get verify flag setting
#define _GETCD    0x59	//Get current directory
#define _CHDIR    0x5A	//Change current directory
#define _PARSE    0x5B	//Parse pathname
#define _PFILE    0x5C  //Parse filename
#define _CHKCHR   0x5D	//Check character
#define _WPATH    0x5E	//Get whole path string
#define _FLUSH    0x5F	//Flush disk buffers
#define _FORK     0x60	//Fork a child process
#define _JOIN     0x61	//Rejoin parent process
#define _TERM     0x62	//Terminate with error code
#define _DEFAB    0x63	//Define abort exit routine
#define _DEFER    0x64	//Define disk error handler routine
#define _ERROR    0x65	//Get previous error code
#define _EXPLAIN  0x66	//Explain error code
#define _FORMAT   0x67	//Format a disk
#define _RAMD     0x68	//Create or destroy RAM disk
#define _BUFFER   0x69	//Allocate sector buffers
#define _ASSIGN   0x6A	//Logical drive assignment
#define _GENV     0x6B	//Get environment item
#define _SENV     0x6C	//Set environment item
#define _FENV     0x6D	//Find environment item
#define _DSKCHK   0x6E	//Get/set disk check status
#define _DOSVER   0x6F	//Get MSX-DOS version number
#define _REDIR    0x70	//Get/set redirection status

#endif