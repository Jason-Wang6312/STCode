#ifndef __LCD1602_H
#define __LCD1602_H
#include "STC12C5A.h"

sbit LCD1602_RS = P2^6;
sbit LCD1602_RW = P2^5;
sbit LCD1602_E  = P2^7;
sbit LCD1602_DB7 = P0^7;
sbit LCD1602_DB6 = P0^6;
sbit LCD1602_DB5 = P0^5;
sbit LCD1602_DB4 = P0^4;
#define LCD1602_DB  P0
#define LCD1602_4PIN 1

/* 打开光标的闪烁效果 */
void LcdOpenCursor();
/* 关闭光标显示 */
void LcdCloseCursor();
/* 初始化1602液晶 */
void InitLcd1602();
/* 在液晶上显示字符串，(x,y)-对应屏幕上的起始坐标，str-字符串指针 */
void LcdShowStr(unsigned char x, unsigned char y, unsigned char *str);
/* 在液晶上指定坐标上显示字符*/
void ledWriteSingeByte(unsigned char x, unsigned char y, unsigned char dat);

#endif
