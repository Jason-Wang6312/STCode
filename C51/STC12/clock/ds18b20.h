#ifndef __DS18B20_H
#define __DS18B20_H

#include "config.h"

/*****set pin********/
sbit DS18B20_DQ = P3^7;

//获取温度
u16 getTemp();

#endif
