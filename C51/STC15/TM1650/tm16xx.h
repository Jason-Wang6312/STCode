#ifndef __TM16XX_H
#define __TM16XX_H
#include "STC15Wxx.h"
#include "INTRINS.H"

sbit TM_SCL = P5^5;
sbit TM_SDA = P5^4;

void TMxxShow(unsigned char index,unsigned char dat);

#endif
