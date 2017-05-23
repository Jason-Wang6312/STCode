#ifndef __DS3231_H
#define __DS3231_H
#include "init.h"

sbit DS3231_SDA = P1^0;
sbit DS3231_SCL = P1^1;

//struct DSTime(){
//    unsigned int year; //年
//    unsigned char month; //月
//    unsigned char day; //日
//    unsigned char hour; //时
//    unsigned char minute; //分
//    unsigned char second; //秒
//    unsigned char week; //星期
//    unsigned char alarm1; //闹钟1
//    unsigned char alarm2; //闹钟2
//}

//读时间
void Read_RTC();
//设置时间，初始化
void Set_RTC();
//写入一个字节到DS3231
void DS3231_Write_Single_Byte(unsigned char addr,unsigned char dat);
//读取DS3231一个字节
unsigned char DS3231_Read_Single_Byte(unsigned char addr);

#endif