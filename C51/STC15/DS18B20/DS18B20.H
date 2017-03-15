#ifndef __DS18B20_H
#define __DS18B20_H
#include "init.h"

/********reset var type*********/
#ifndef u8
#define u8 unsigned char
#endif

#ifndef u16
#define u16 unsigned int
#endif

/*****set pin********/
sbit DS18B20_DQ = P5^5;

u16 getTemp();

#endif