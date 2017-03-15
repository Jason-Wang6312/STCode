#ifndef __DS1302_H
#define __DS1302_H

#include "config.h"

sbit DS1302_CE = P1^7;
sbit DS1302_CK = P3^5;
sbit DS1302_IO = P3^4;

/* DS1302初始化，如发生掉电则重新设置初始时间 */
void InitDS1302();
/* 设定实时时间，时间结构体格式的设定时间转换为数组并写入DS1302 */
void SetRealTime(struct sTime *time);
/* 获取实时时间，即读取DS1302当前时间并转换为时间结构体格式 */
void GetRealTime(struct sTime *time);

#endif
