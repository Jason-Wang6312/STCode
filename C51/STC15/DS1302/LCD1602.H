#ifndef __LCD1602_H
#define __LCD1602_H

#include "conf.h"

/****** reset var type*************/


/********** set pin ************/
#define LCD1602_DB P0
sbit LCD1602_RW = P2^5;
sbit LCD1602_RS = P2^6;
sbit LCD1602_E = P2^7;

void LCDInit();
void LCDShowStr(u8 x,u8 y,u8 *str);

#endif