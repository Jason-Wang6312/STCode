#ifndef __HC_SR04_H
#define __HC_SR04_H

#include "STC12C5A.H"
#include "INTRINS.H"
#include "STDIO.H"

/****** reset var type*************/
#ifndef u8
	#define u8 unsigned char
#endif
#ifndef u16
	#define u16 unsigned int
#endif

/********** set pin ************/
sbit Trig = P2^1;
sbit Echo = P2^0;

/********* define function ******************/
	
u16 getDistance();
void delay20us();
void Timer0Init(void);
extern void sendStr(unsigned char *str);
extern void sendData(unsigned char dat);
#endif