#ifndef __HC_SR04_H
#define __HC_SR04_H

#include "STC12C5A.H"
#include "INTRINS.H"

/****** reset var type*************/
#ifndef u8
	#define u8 unsigned char
#endif
#ifndef u16
	#define u16 unsigned char
#endif

/********** set pin ************/
sbit Trig = P2^1;
sbit Echo = P2^0;

/********* define function ******************/
	
u16 getDistance();
void HCSR04Init();

#endif