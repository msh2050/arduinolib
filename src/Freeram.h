// keyboard.h
/*
Used to determine the amount of memory currently available
call it anywhere in your program like that: Serial.println(freeRam());
source: http://playground.arduino.cc/Code/AvailableMemory
date:- 3-1-2017
author:Mustafa Salaman
*/

#ifndef _C_Free_Ram_h
#define _C_Free_Ram_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

int freeRam () {
  extern int __heap_start, *__brkval; 
  int v; 
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval); 
}
	



#endif

