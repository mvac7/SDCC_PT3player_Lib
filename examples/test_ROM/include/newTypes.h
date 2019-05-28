// newtypes v1.0 - 12march2018
#ifndef  __NEWTYPES_H__
#define  __NEWTYPES_H__


#ifndef  NULL
#define  NULL  ((void *) 0)
#endif

// boolean
#ifndef _BOOLEAN
#define _BOOLEAN
  typedef enum {false = 0, true = 1} boolean;
#endif

// unsigned integer
#ifndef uint
typedef unsigned int uint;
#endif

// unsigned char
#ifndef byte
typedef unsigned char byte;
#endif

//definiciones de A.Herrera 
//http://msx.atlantes.org/index_es.html
#ifndef int8
typedef signed char int8;
#endif
//typedef unsigned char uint8;
//typedef signed int int16_t;
//typedef unsigned int uint16_t;
//typedef signed long int32_t;
//typedef unsigned long uint32_t;
//typedef float float32;

#endif