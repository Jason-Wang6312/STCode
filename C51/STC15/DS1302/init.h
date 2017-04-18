#ifndef _INIT_H
#define _INIT_H
#include "INTRINS.H"
#include "STC12C5A.H"
//#include "DS1302x.H"

typedef unsigned char u8;
typedef unsigned int u16;

sbit K1=P1^4;
sbit K2=P1^5;
sbit K3=P1^6;
sbit K4=P1^7;
sbit K5=P3^5;
sbit LED1=P1^0;
sbit LED2=P1^1;
sbit LED3=P1^2;
sbit LED4=P1^3;

#endif
