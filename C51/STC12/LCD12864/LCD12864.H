#ifndef __LCD12864_H
#define __LCD12864_H

#include "STC12C5A.H"
#include "INTRINS.H"

#ifndef u8
#define u8 unsigned char
#endif

#ifndef u16
#define u16 unsigned int
#endif

/*****set pin**********/
sbit LCD_CS  = P2^6;
sbit LCD_RW  = P2^5;
sbit LCD_E   = P2^7;
sbit LCD_PSB = P3^2;
sbit LCD_Data= P0;

void LCD_Init();
void LCDWriteChar(u8 str);
void LCDWriteStr(u8 x,u8 y,u8 *str);

#endif