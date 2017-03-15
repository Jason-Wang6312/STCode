#ifndef __DS18B20_H
#define __DS18B20_H

#include "STC12C5A.H"
#include "INTRINS.H"

/********reset var type*********/
#ifndef u8
#define u8 unsigned char
#endif

#ifndef u16
#define u16 unsigned int
#endif

/*****set pin********/
sbit DS18B20_DQ = P3^7;

bit DS18B20Init();
u16 getTemp();

#endif