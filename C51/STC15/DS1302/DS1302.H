#ifndef __DS1302_H
#define __DS1302_H
#include "STC12C5A.H"
#include "INTRINS.H"

//硬件IO口定义
sbit DS1302_SCLK = P3^6;
sbit DS1302_IO = P3^4;
sbit DS1302_CE = P3^5;

//定义变量
extern struct sTime
{
	unsigned int year;
	unsigned char month;
	unsigned char day;
	unsigned char hour;
	unsigned char minute;
	unsigned char second;
	unsigned char week;
};

//声明函数
void InitDS1302();
void GetRealTime(struct sTime *time);
void SetRealTime(struct sTime *time);
void DS1302BurstRead(unsigned char *dat);
void DS1302BurstWrite(unsigned char *dat);
void DS1302SingleWrite(unsigned char reg,unsigned char dat);
unsigned char DS1302SingleRead(unsigned char reg);

#endif
