#ifndef __HC595_H
#define __HC595_H

//#include "STC15F2K.H"
#include "reg52.h"
#include "INTRINS.H"

sbit SH_CP = P1^1;
sbit ST_CP = P1^0;
sbit DS = P3^7;
sbit LED1 = P1^2;
sbit LED2 = P1^3;
sbit LED3 = P1^4;
sbit LED4 = P1^5;

void HC595In(unsigned char dat);
void HC595Out();

#endif