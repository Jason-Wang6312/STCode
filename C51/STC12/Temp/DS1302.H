#ifndef __DS1302_H
#define __DS1302_H

#include "STC12C5A.H"
#include "INTRINS.H"

/****** reset var type*************/
#ifndef u8
	#define u8 unsigned char
#endif
	
/********** set pin ************/
sbit DS1302_SCLK = P3^6;
sbit DS1302_IO = P3^4;
sbit DS1302_CE = P3^5;
	
/*********** define function ******************/
void DS1302Init();
void DS1302ReadTime();
	
extern u8 DS1302_Time[7];
	
#endif
