#ifndef __Infrared_H
#define __Infrared_H
#include "STC12C5A.H"

#ifndef u8
#define u8 unsigned char
#define u16 unsigned int
#endif

sbit InfIO = P3^2;

//检测数据正确性
u8 checkData(u8 _userCode);
//解析红外数据
void analysisData();
//检测引导码是否正确
void guideCheck();

#endif

