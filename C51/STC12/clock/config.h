#ifndef __CONFIG_H
#define __CONFIG_H
#include "STC12C5A.H"
#include "INTRINS.H"

#ifndef u8
    typedef unsigned char u8;
    typedef unsigned int u16;
#endif

void Delay1ms();
void DelayXms(u16 n);
void Delay1us();
void DelayXus(u16 n);


#endif
